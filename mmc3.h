#ifndef _MMC3_H_
#define _MMC3_H_

#define MMC3_PRG_RAM 0xa001

//!< MMC(Memory Management Controller) ... コンフィグファイルでMMC3を有効にしておく必要がある
//!< [ IRQ ]
//!<    スキャンラインをカウントして、指定のカウントで IQR を発行可能
//!< レジスタA の値を任意のアドレスへ
#define MMC3_IRQ_LATCH 0xc000
#define MMC3_IRQ_RELOAD 0xc001
#define MMC3_IRQ_DISABLE 0xe000
#define MMC3_IRQ_ENABLE 0xe001
#define STROBE(adr) __asm__ ("sta %w", adr)
#define IRQ_SET_VALUE(n)   POKE(MMC3_IRQ_LATCH, (n));
#define IRQ_RELOAD()       STROBE(MMC3_IRQ_RELOAD)
#define IRQ_DISABLE()      STROBE(MMC3_IRQ_DISABLE)
#define IRQ_ENABLE()       STROBE(MMC3_IRQ_ENABLE)
//!< 6502 の割り込みフラグをクリア、これにより IQR が有効になる
#define ENABLE_CPU_IRQ __asm__ ("cli")
//!< この割込みが IQR か MNI か (IRQ の場合には A レジスタの上位ビットが立つ)
#define IS_IRQ (__A__ & 0x80)

//!< [ バンク切替え ]
#define MMC3_BANK_SEL 0x8000
#define MMC3_BANK_DATA 0x8001
#define MMC_MODE 0x00
#define MMC3_SET_REG(reg, bank) POKE(MMC3_BANK_SEL, MMC_MODE | (reg)); POKE(MMC3_BANK_DATA, (bank))

#define MMC3_CHR_0000(b) MMC3_SET_REG(0, b) //!< 0x0800 (2KB) 128 tiles
#define MMC3_CHR_0800(b) MMC3_SET_REG(1, b) //!< 0x0800 (2KB)
#define MMC3_CHR_1000(b) MMC3_SET_REG(2, b) //!< 0x0400 (1KB)
#define MMC3_CHR_1400(b) MMC3_SET_REG(3, b) //!< 0x0400 (1KB)
#define MMC3_CHR_1800(b) MMC3_SET_REG(4, b) //!< 0x0400 (1KB)
#define MMC3_CHR_1C00(b) MMC3_SET_REG(5, b) //!< 0x0400 (1KB)

//!< [0x8000, 0x9fff] 0, 1, 2
//!< [0xa000, 0xbfff] 3, 4, 5
//!< [0xc000, 0xdfff], [0xe000, 0xffff] 最後の2バンクは固定
#define MMC3_PRG_8000(b) MMC3_SET_REG(6, b) //!< 0x2000 (8KB)
#define MMC3_PRG_A000(b) MMC3_SET_REG(7, b) //!< 0x2000 (8KB)

//!< [ ミラーリング ]
#define MMC3_MIRRORING 0xa000
#define MMC3_MIRROR_VERT POKE(MMC3_MIRRORING, 0)
#define MMC3_MIRROR_HORZ POKE(MMC3_MIRRORING, 1)

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
#endif //!< _MMC3_H_