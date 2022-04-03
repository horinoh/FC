#include <stdint.h>
#include <string.h>
#include <peekpoke.h>

#define USE_NESLIB2

#ifdef USE_NESLIB2
#include "../neslib2/neslib.h"
#else
#include "../neslib/neslib.h"
//!< oam_off
#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")
uint8_t oam_off;
#pragma data-name(pop)
#pragma bss-name (pop)
#endif

#ifndef USE_NESLIB2
//!< PPU マスク
#define MASK_TINT_RED		0x20
#define MASK_TINT_BLUE		0x40
#define MASK_TINT_GREEN		0x80
#define MASK_MONO		0x01
#endif

//!< VRAM バッファ
//!< スタック [0x100, 0x1ff] の内 [0x100, 0x17f](128byte) を VRAM バッファとして使用することにする
#define VramBuffer ((uint8_t *)0x100)
uint8_t VramIndex = 0;
//!< NMI の間に扱えるのは精々 140 byte くらいなので 128 byte としておく
//!< 他に色々やっていると 128 byte もフルには使えない？
uint8_t VramBufferSize = 64; //128;
//!< VRAM バッファのフォーマット
//!< アドレス上位 | (NT_UPD_HORZ or NT_UPD_VERT), 
//!< アドレス下位, 
//!< 長さ, 
//!< ..., 
//!< NT_UPD_EOF
#define VRAMBUF_SIZE(size) VramBufferSize = size
#define VRAMBUF_DATA(data) VramBuffer[VramIndex++] = data
#define VRAMBUF_DATA_LEN(data, len) memcpy(&VramBuffer[VramIndex], data, len); VramIndex += len
#define VRAMBUF_END VramBuffer[VramIndex] = NT_UPD_EOF;
#define VRAMBUF_HEADER(adr, len, flags) VRAMBUF_DATA((adr >> 8) | flags); VRAMBUF_DATA(adr); VRAMBUF_DATA(len)
#define VRAMBUF_CLEAR VramIndex = 0; VRAMBUF_END
#define VRAMBUF_FLUSH VRAMBUF_END; ppu_wait_frame(); VRAMBUF_CLEAR
void vrambuf_put(const uint16_t adr, register const char* str, const uint8_t flags)
{
    const uint8_t len = strlen(str);
    if(VramBufferSize < VramIndex + len + 4) {
        VRAMBUF_FLUSH;
    }
    VRAMBUF_HEADER(adr, len, flags);
    VRAMBUF_DATA_LEN(str, len);
    VRAMBUF_END;
}
void vrambuf_put_h(const uint16_t adr, register const char* str) { vrambuf_put(adr, str, NT_UPD_HORZ); }
void vrambuf_put_v(const uint16_t adr, register const char* str) { vrambuf_put(adr, str, NT_UPD_VERT); }

//!< MMC(Memory Management Controller) ... コンフィグファイルでMMC3を有効にしておく必要がある
//!< スキャンラインをカウントして、指定のカウントで IQR を発行可能
//!< レジスタA の値を任意のアドレスへ
#define STROBE(adr) __asm__ ("sta %w", adr)
#define MMC3_IRQ_SET_VALUE(n)   POKE(0xc000, (n));
#define MMC3_IRQ_RELOAD()       STROBE(0xc001)
#define MMC3_IRQ_DISABLE()      STROBE(0xe000)
#define MMC3_IRQ_ENABLE()       STROBE(0xe001)
#define MMC3_IRQ_RESET()        MMC3_IRQ_DISABLE();MMC3_IRQ_ENABLE()

//!< 6502 の割り込みフラグをクリア、これにより IQR が有効になる
#define ENABLE_CPU_IRQ __asm__ ("cli")
//!< この割込みが IQR か MNI か
#define IS_IRQ (__A__ & 0x80)

#define MMC3_BANK_SEL 0x8000
#define MMC3_BANK_DATA 0x8001
#define MMC3_MIRRORING 0xa000
#define MMC3_PRG_RAM 0xa001
#define MMC3_IRQ_LATCH 0xc000
// #define MMC3_IRQ_RELOAD 0xc001
// #define MMC3_IRQ_DISABLE 0xe000
// #define MMC3_IRQ_ENABLE 0xe001

#define MMC_MODE 0x00
#define MMC3_SET_REG(r, bank) POKE(MMC3_BANK_SEL, MMC_MODE | (r)); POKE(MMC3_BANK_DATA, (bank))

/*
#define MMC3_CHR_0000(n) MMC3_SET_REG(0, n)
#define MMC3_CHR_0800(n) MMC3_SET_REG(1, n)
#define MMC3_CHR_1000(n) MMC3_SET_REG(2, n)
#define MMC3_CHR_1400(n) MMC3_SET_REG(3, n)
#define MMC3_CHR_1800(n) MMC3_SET_REG(4, n)
#define MMC3_CHR_1C00(n) MMC3_SET_REG(5, n)
#define MMC3_PRG_8000(n) MMC3_SET_REG(6, n)
#define MMC3_PRG_A000(n) MMC3_SET_REG(7, n)
*/

#define MMC3_PRG_SELECT_BANK0 MMC3_SET_REG(6, 0)
#define MMC3_PRG_SELECT_BANK1 MMC3_SET_REG(6, 1)
#define MMC3_PRG_SELECT_BANK2 MMC3_SET_REG(6, 2)

#define MMC3_PRG_SELECT_BANK3 MMC3_SET_REG(7, 3)
#define MMC3_PRG_SELECT_BANK4 MMC3_SET_REG(7, 4)
#define MMC3_PRG_SELECT_BANK5 MMC3_SET_REG(7, 5)

//!< ミラーリング
#define MMC3_MIRROR_VERT POKE(MMC3_MIRRORING, 0)
#define MMC3_MIRROR_HORZ POKE(MMC3_MIRRORING, 1)

/*
CPU
[0x0000, 0x00ff]    ゼロページRAM
[0x0100, 0x01ff]    スタックRAM
[0x0200, 0x07ff]    RAM
[0x2000, 0x2007]    PPUレジスタ
[0x4000, 0x401f]    APU, IOレジスタ
[0x6000, 0x7fff]    バックアップRAM
[0x8000, 0xffff]    ROM      
*/
/*
PPU
[0x0000, 0x0fff]    パターンテーブル0
[0x1000, 0x1fff]    パターンテーブル1
[0x2000, 0x23bf]    ネームテーブル0
[0x23c0, 0x23ff]    アトリビュートテーブル0
[0x2400, 0x27bf]    ネームテーブル1
[0x27c0, 0x27ff]    アトリビュートテーブル1
[0x2800, 0x2bbf]    ネームテーブル2
[0x2bc0, 0x2bff]    アトリビュートテーブル2
[0x2c00, 0x2fbf]    ネームテーブル3
[0x2fc0, 0x2fff]    アトリビュートテーブル3
[0x3f00, 0x3f0f]    BG パレット
[0x3f10, 0x3f1f]    スプライトパレット

画面        256 x 240
色          64 色(56 ユニーク)
タイル      8 x 8、3 + 1 色
パターン    512
マップ      32 x 60 or 64 x 30
スプライト  8 x 8 or 8 x 16、64 枚
*/

/*
MMC(Memory Management Controller)3

MMC3 を有効にするには、コンフィグファイルで MMC を使用する設定にする必要がある
SYMBOLS {
    NES_MAPPER:	  type = weak, value = 4; # 4 = MMC3 txrom
}

CHR
[0x0000, 0x07ff] 2kb (128タイル)
[0x0800, 0x0fff] 2kb (128タイル)
[0x1000, 0x13ff] 2kb (128タイル)
[0x1400, 0x17ff] 1kb (64タイル)
[0x1800, 0x1bff] 1kb (64タイル)
[0x1c00, 0x1fff] 1kb (64タイル)

PRG
[0x6000, 0x7fff] ワークRAM
[0x8000, 0x9fff] CODE[012] (スイッチャブル) 
[0xa000, 0xbfff] CODE[345] (スイッチャブル) 
[0xc000, 0xdfff] CODE6, const, スタートアップ (固定)
[0xe000, 0xffff] CODE, Cライブラリ, 割り込み([0xfffa, 0xffff]) (固定)

現在のセグメントを変更する例)
#pragma rodata-name("CODE1")
const char TEXT1[] = { "Hello Bank1 @ 0x8000" };
#pragma rodata-name("CODE5")
const char TEXT5[] = { "Hello Bank5 @ 0xa000" };

#pragma code-name("CODE1")
void draw_text(uint16_t adr, const char* text){}
#pragma code-name("CODE") // 元に戻す

//!< 関数呼び出しの前にアドレスを選択する
MMC3_PRG_SELECT_BANK1; //!< 関数用にバンク1を選択
MMC3_PRG_SELECT_BANK5; //!< 引数のテキスト用にバンク5を選択
draw_text(NTADR_A(2, 3), TEXT5)
*/

/*
NAMETABLE
A, B, C, D それぞれ 256 x 240、全体で 512 x 480
A B
C D

水平ミラーリング (縦スクロールで使われる)
A A
C C

垂直ミラーリング (横スクロールで使われる)
A B
A B

ミラーリングの指定はコンフィグファイルで行う
SYMBOLS {
    NES_MIRRORING: type = weak, value = 0; 	# 0 horizontal, 1 vertical, 8 four screen
}
*/

/*
OAM

スプライト数    64
OAMバッファ     256 byte
    Y
    TileIndex
    Attribute : パレット([0, 3])、反転(OAM_FLIP_V, OAM_FLIP_H)、プライオリティ(OAM_BEHIND)指定
    X

エッジを隠せない設定の場合、左側、上側ではスプライトが部分的に隠れることができない
左側、上側を隠さない(デフォルト)場合は ppu_mask() に MASK_EDGE_XXX を指定する、隠す設定の場合は MASK_EDGE_XXX は指定しない
ppu_mask(MASK_BG | MASK_SPR | MASK_EDGE_BG | MASK_EDGE_SPR);
*/

#define COUNTOF(x) sizeof(x) / sizeof(x[0])

#define NT_TILE_WIDTH 32
#define NT_TILE_HEIGHT 30
#define NT_WIDTH (NT_TILE_WIDTH << 3)
#define NT_HEIGHT (NT_TILE_HEIGHT << 3)
#define NT_WIDTH2 (NT_WIDTH << 1)
#define NT_HEIGHT2 (NT_HEIGHT << 1)