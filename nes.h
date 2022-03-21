#include <stdint.h>
#include <string.h>

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

#define COUNTOF(x) sizeof(x) / sizeof(x[0])

#define NT_TILE_WIDTH 32
#define NT_TILE_HEIGHT 30
#define NT_WIDTH (NT_TILE_WIDTH << 3)
#define NT_HEIGHT (NT_TILE_HEIGHT << 3)
#define NT_WIDTH2 (NT_WIDTH << 1)
#define NT_HEIGHT2 (NT_HEIGHT << 1)