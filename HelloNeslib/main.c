#include <string.h>
#include <stdint.h>

#include "../neslib/neslib.h"

//!< oam_off
typedef uint8_t u8;
#pragma bss-name (push,"ZEROPAGE")
#pragma data-name (push,"ZEROPAGE")
uint8_t oam_off;
#pragma data-name(pop)
#pragma bss-name (pop)

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

void main()
{
    const char* Str = "Hello World";

    pal_col(0, 0x02);
    pal_col(1, 0x14);
    pal_col(2, 0x20);
    pal_col(3, 0x30);

    //!< 書き込み先の VRAM
    //!< NTADR_A : ネームテーブルA (2, 2) のアドレス
    vram_adr(NTADR_A(2, 2));
    vram_write((const uint8_t*)Str, strlen(Str));

    //!< 描画をオンにする
    ppu_on_all();

    while(1) {}

    //!< 描画をオフにする
    ppu_off();
}