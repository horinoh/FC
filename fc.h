#ifndef _NES_H_
#define _NES_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <peekpoke.h>
#include <nes.h>
#include <stdlib.h>

#include "mmc3.h"
#include "apu.h"

//#define USE_OLD_NESLIB

#ifdef USE_OLD_NESLIB
#include "../neslib/neslib.h"
//!< oam_off
#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")
uint8_t oam_off;
#pragma data-name(pop)
#pragma bss-name (pop)
#define MASK_TINT_RED		0x20
#define MASK_TINT_BLUE		0x40
#define MASK_TINT_GREEN		0x80
#define MASK_MONO		0x01
#else
#include "../neslib/neslib.h"
#endif

void put_str(const uint16_t adr, const char* str) 
{
    vram_adr(adr);
    vram_write((const uint8_t*)str, strlen(str));
}

//!< VRAM バッファ
//!< スタック [0x100, 0x1ff] の内 [0x100, 0x17f](128byte) を VRAM バッファとして使用することにする
#define VramBuffer ((uint8_t *)0x100)
uint8_t VramIndex = 0;
//!< NMI の間に扱えるのは精々 140 byte くらいなので ここでは 128 byte としておく
//!< 他に色々やっていると 128 byte もフルには使えないので調整して使う
uint8_t VramBufferSize = 128;
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

//!< RLE
#define RLE_TAG 0x1
#define RLE_COUNT(x) RLE_TAG, x

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

/*
パフォーマンス
__fastcall__ がついているアセンブラで書かれた関数は、スタックではなく A, X レジスタを使用するため高速
スタック(ローカル変数、引数)は、グローバル変数より 5倍遅い
i++ よりも ++i を使う
*(p + 1) よりも p[1] を使う
*/

/*
インラインアセンブラ
asm("...");
*/

#define COUNTOF(x) sizeof(x) / sizeof(x[0])

#define PATTERN_TABLE_0 0x0000
#define PATTERN_TABLE_1 0x1000
#define PTADR_0(x) (PATTERN_TABLE_0 | ((x) << 4))
#define PTADR_1(x) (PATTERN_TABLE_1 | ((x) << 4))

#define NT_TILE_WIDTH 32
#define NT_TILE_HEIGHT 30
#define NT_TILE_WIDTH2 (NT_TILE_WIDTH << 1)
#define NT_TILE_HEIGHT2 (NT_TILE_HEIGHT << 1)
#define NT_TILE_WIDTH_LIMIT(x) while(x < 0) { x += NT_TILE_WIDTH2; } x &= (NT_TILE_WIDTH2 - 1)
#define NT_TILE_HEIGHT_LIMIT(y) while(y < 0) { y += NT_TILE_HEIGHT2; } y %= NT_TILE_HEIGHT2

#define NT_WIDTH (NT_TILE_WIDTH << 3)
#define NT_HEIGHT (NT_TILE_HEIGHT << 3)
#define NT_WIDTH2 (NT_WIDTH << 1)
#define NT_HEIGHT2 (NT_HEIGHT << 1)
#define NT_WIDTH_LIMIT(x) while(x < 0) { x += NT_WIDTH2; } x &= (NT_WIDTH2 - 1)
#define NT_HEIGHT_LIMIT(y) while(y < 0) { y += NT_HEIGHT2; } y %= NT_HEIGHT2

#define NTADR_VERT(x, y) x < NT_TILE_WIDTH ? NTADR_A(x, y) : NTADR_B(x & (NT_TILE_WIDTH - 1), y)
#define NTADR_HORZ(x, y) y < NT_TILE_HEIGHT ? NTADR_A(x, y) : NTADR_C(x, y % NT_TILE_HEIGHT)

#define ATTR_LT(x) (x)
#define ATTR_RT(x) ((x) << 2)
#define ATTR_LB(x) ((x) << 4)
#define ATTR_RB(x) ((x) << 6)
#define ATTR(lt, rt, lb, rb) ATTR_LT(lt) | ATTR_RT(rt) | ATTR_LB(lb) | ATTR_RB(rb)

#define OAM_SIZE_8x8 0
#define OAM_SIZE_8x16 0

#define OAM_END_OF_META_SPR 128

//!< サウンド
#define SFX_CH0 0
#define SFX_CH1 1
#define SFX_CH2 2
#define SFX_CH3 3

#endif //!< _NES_H_