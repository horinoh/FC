#include "../fc.h"

//!< [ UXROM ]
//!< CHAR RAM を使用するには、NES_CHR_BANKS を 0 にする  

//!< [ モノビットマップ ]
//!< 256 x 240 のフレームバッファを模した描画、1色 + 背景色のみ
//!<  スクリーンを4つのセクションに分ける
//!<  [0] CHAR RAM バンク = 0x0000, パレット = 0 ... Line[  0,  63] -> 256 x 64 ピクセル = 64 x 8 タイル = 256 タイル -> ネームテーブル[0, 255] で賄える
//!<  [1] CHAR RAM バンク = 0x0000, パレット = 1 ... Line[ 64, 127]
//!<  ------------------------------------------------------------ スキャンライン中にバンク切替え
//!<  [2] CHAR RAM バンク = 0x1000, パレット = 0 ... Line[128, 191]
//!<  [3] CHAR RAM バンク = 0x1000, パレット = 1 ... Line[192, 223]
//!<    セクション 0, 1 (2, 3) は同じCHAR RAM を使用するが、パレットが異なる事により別の絵を表現

//!< 5 サイクル
#define DELAYLOOP(n) __asm__("ldy #%b", n); __asm__("@1: dey"); __asm__("bne @1")
//!< スキャンライン中に CHAR RAM バンクを切り替える
void bitmap_split() 
{
  split(0, 0);

  //!< スキャンライン中に切り替えるには正確なタイミングでやらなければならない
  DELAYLOOP(15); //!< スキャンラインの最後まで待つ

  //!< neslib は NMI 毎に CHAR バンクを 0 にリセットするので、明示的に CHAR バンク1 (0x1000) へ変更する
  //!< PPUCTRL 5ビット目は BGパターンテーブルの選択 (0 : 0x0000, 1 : 0x1000)
  PPU.control ^= 0x10;
}

bool bitmap_draw_mode_ppu_on = false;

//!< Y座標からパレットを算出
#define PIXEL_PAL(y) (((y) & 0x40) >> 3)
//!< Y座標からパターンテーブルを算出
#define PIXEL_PATTBL(y) (((y) & 0x80) << 5)
//!< X, Y座標から、セクション、タイル番号、ビットプレーンを考慮した、パターンテーブルのアドレスを算出する
#define PIXEL_ADR(x, y) (((x) >> 3) << 4) | ((((y) & 0x3f) >> 3) << 9) | ((y) & 0x7) | PIXEL_PAL(y) | PIXEL_PATTBL(y)
void bitmap_set_pixel(const uint8_t x, const uint8_t y)
{
  uint16_t adr = PIXEL_ADR(x, y);
  uint8_t data;
  vram_adr(adr); vram_read(&data, sizeof(data)); {
    //!< ピクセルの描画
    data |= 0x80 >> (x & 0x7);
  } vram_adr(adr); vram_put(data);
}
void bitmap_draw_pixel(const uint8_t x, const uint8_t y)
{
  if(bitmap_draw_mode_ppu_on) {
    //!< PPU がオンの場合は、次のフレームまで待つ
    ppu_wait_nmi();

    bitmap_set_pixel(x, y);

    //!< PPU がオンの場合は、アドレスをリセットして、スキャンライン中にバンクを切り替える
    vram_adr(0x0000);
    bitmap_split();
  } else {
    bitmap_set_pixel(x, y);
  }  
}
void bitmap_draw_line(int x0, int y0, int x1, int y1)
{
  int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
  int err = dx + dy, e2;
 
  for(;;) {
    bitmap_draw_pixel(x0, y0);
    if (x0 == x1 && y0 == y1) { break; }
    e2 = err << 1;
    if (e2 >= dy) { err += dy; x0 += sx; }
    if (e2 <= dx) { err += dx; y0 += sy; }
  }
}
void bitmap_draw_circle(int cx, int cy, int r)
{
   int x = -r, y = 0, err = 2 - (r << 1);
   do {
      bitmap_draw_pixel(cx - x, cy + y);
      bitmap_draw_pixel(cx - y, cy - x); 
      bitmap_draw_pixel(cx + x, cy - y); 
      bitmap_draw_pixel(cx + y, cy + x);
      r = err;
      if (r >  x) { err += (++x << 1) + 1; }
      if (r <= y) { err += (++y << 1) + 1; }
   } while (x < 0);
}
void bitmap_setup()
{
  uint16_t i;

  //!< セットアップは PPU オフで行うこと
  bitmap_draw_mode_ppu_on = false;

  //!< パターンテーブルをクリア
  //!<  パターンテーブル 0 : [0x0000, 0x0fff] 0x1000 = 4096 = 16 byte * 256
  //!<  パターンテーブル 1 : [0x1000, 0x1fff]
  vram_adr(PATTERN_TABLE_0);
  vram_fill(0x0, 0x2000); //!< パターンテーブル 0, 1 トータルサイズ 0x2000

  //!< ネームテーブル (32 x 30 = 960 = 256 + 256 + 256 + 192)
  //!<  セクション毎に、連番 [0, 255] で埋め尽くす
  vram_adr(NAMETABLE_A);
  for (i = 0;i < 256; i++) { vram_put(i); }
  for (i = 0;i < 256; i++) { vram_put(i); }
  for (i = 0;i < 256; i++) { vram_put(i); }
  for (i = 0;i < 192; i++) { vram_put(i); }
  //!< アトリビュートテーブル (64要素、4セクションに分けて(64 / 4 = 16)、パレット0, 1 をセクション毎に交互に設定する)
  //vram_adr(NAMETABLE_A + 0x3c0); //!< 0x3c0 = 960 = 32 x 30
  vram_fill(ATTR(0, 0, 0, 0), 16); 
  vram_fill(ATTR(1, 1, 1, 1), 16);
  vram_fill(ATTR(0, 0, 0, 0), 16);
  vram_fill(ATTR(1, 1, 1, 1), 16); 

  // vram_adr(NAMETABLE_B);
  // for (i = 0;i < 256; i++) { vram_put(i); }
  // for (i = 0;i < 256; i++) { vram_put(i); }
  // for (i = 0;i < 256; i++) { vram_put(i); }
  // for (i = 0;i < 192; i++) { vram_put(i); }
  // vram_fill(ATTR(0, 0, 0, 0), 16); 
  // vram_fill(ATTR(1, 1, 1, 1), 16);
  // vram_fill(ATTR(0, 0, 0, 0), 16); 
  // vram_fill(ATTR(1, 1, 1, 1), 16); 

  //!< BG = バンク0, スプライト = バンク1
  bank_bg(0);
  bank_spr(1);

  //!< (分割用) スプライト 0 の設定
  #define SPR_X NT_WIDTH - 8 - 1
  #define SPR_Y 125
  #define SPR_PAT_ID 255
  #define SPR_ATTR 0x0
  #define OAM_ID 0
  oam_clear();
  oam_size(OAM_SIZE_8x8);
  oam_spr(SPR_X, SPR_Y, SPR_PAT_ID, SPR_ATTR, OAM_ID);

  //!< スプライト 0 が衝突するように BG にピクセルを描画しておく
  bitmap_draw_pixel(SPR_X, SPR_Y + 1);

  //!< SPR_PAT_ID のスプライトを、最上部 8 ドットのラインパターンとする
  vram_adr(PTADR_1(SPR_PAT_ID)); //!< パターンテーブル 1 の最後(SPR_PAT_ID) のパターンのアドレス
  vram_fill(0xff, 1/*16*/); //!< パターン0 プレーン0 の最初の要素だけを 0xff にすることで、スプライトは最上部 8 ドットのラインパターンとなる
}

void bitmap_draw()
{
  uint8_t i;

  for(i=0;i<8;++i) {
    bitmap_draw_circle(rand8(), rand8() % NT_HEIGHT, (rand8() & 15) + 2);
  }
  for(i=0;i<8;++i) {
    bitmap_draw_line(rand8(), rand8() % NT_HEIGHT, rand8(), rand8() % NT_HEIGHT);
  }
  for(i = 0;i < 32;++i) {
    bitmap_draw_pixel(rand8(), rand8() % NT_HEIGHT);
  }
}

void __fastcall__ nmi() {}

void main() 
{
  const uint8_t Palette[] = {
    0x03, 0x30, 0x03, 0x30, //!< BG パレット0
    0x00, 0x03, 0x30, 0x30, //!< BG パレット1
    0x00, 0x00, 0x00, 0x00, //!< BG パレット2 (未使用)
    0x00, 0x00, 0x00, 0x00, //!< BG パレット3 (未使用)
    0x03, 0x30, 0x03, 0x30, //!< スプライト パレット0
    0x00, 0x00, 0x00, 0x00, //!< スプライト パレット1 (未使用)
    0x00, 0x00, 0x00, 0x00, //!< スプライト パレット2 (未使用)
    0x00, 0x00, 0x00, 0x00, //!< スプライト パレット3 (未使用)
  };
  uint8_t Fade = 0;

  //!< パレット
  {
    pal_all((const char*)Palette);
  }

  pal_bright(Fade >> 4);

  //!< ビットマップのセットアップ、オフライン描画
  {
    bitmap_setup();
    bitmap_draw();
  }

  //!< PPU 制御
  {
    ppu_mask(MASK_BG | MASK_SPR);
    ppu_on_all();
  }

  //!< NMI コールバック
  nmi_set_callback(nmi);

  //!< VRAM バッファ
  {
    VramBufferSize = 64;
    VRAMBUF_CLEAR;
    //!< VRAM バッファのアドレスを知らせておく
    set_vram_update(VramBuffer);
  }

  while (1) {
    //!< フェード
    {
      Fade = MIN(++Fade, 64);
      pal_bright(Fade >> 4);
    }
    
    if (pad_trigger(0)) {
      ppu_off();{
        bitmap_setup();
      } ppu_on_all();

      //!< オンライン描画
      bitmap_draw_mode_ppu_on = true;
      bitmap_draw();
    }

    ppu_wait_nmi();
    bitmap_split();
  }

  ppu_off();
}
