#ifndef _MMC3_H_
#define _MMC3_H_

//#define MMC3_PRG_RAM 0xa001

#include "irq.h"

#include "bank_mmc3.h"

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