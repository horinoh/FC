#include "../nes.h"

#define USE_SPLIT
//#define USE_VRAM_BUFFER

void main() 
{
  const uint8_t Palette[] = {
    0x03, 0x11, 0x30, 0x27, //!< BG パレット0
    0x00, 0x1c, 0x20, 0x2c, //!< BG パレット1
    0x00, 0x00, 0x10, 0x20, //!< BG パレット2
    0x00, 0x06, 0x16, 0x26, //!< BG パレット3

    0x00, 0x16, 0x35, 0x24, //!< スプライト パレット0
    0x00, 0x00, 0x37, 0x25, //!< スプライト パレット1
    0x00, 0x0d, 0x2d, 0x3a, //!< スプライト パレット2
    0x00, 0x0d, 0x27, 0x2a, //!< スプライト パレット3
  };
  //!< ATTR(LT, RT, LB, RB) 
  //!< それぞれの引数 LT, RT, LB, RB が 2x2=4 タイル (16x16ピクセル) 分のパレット番号 (1 タイル毎にパレット番号は指定できず、4 タイルはまとめて同じパレット番号になる)
  //!< ATTR() は 4タイル x 4 = 16 タイルをまとめたもの (32x32ピクセル)
  const uint8_t AttributesA[] = {
    //!< [ 0,  3] 列  [ 4,  7] 列       [ 8, 11] 列       [12, 15] 列       [16, 19] 列       [20, 23] 列       [24, 27] 列       [28, 31] 列
    ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), //!< [ 0,  3] 行
    ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), //!< [ 4,  7] 行
    ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), //!< [ 8, 11] 行
    ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), //!< [12, 15] 行
    ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), //!< [16, 19] 行
    ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), //!< [20, 23] 行
    ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), //!< [24, 27] 行
    ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), //!< [28, 29] 行
  };
  const uint8_t AttributesB[] = {
    ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), 
    ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), 
    ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), 
    ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), 
    ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), 
    ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), 
    ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), 
    ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), 
  };
  uint8_t Fade = 0;
  int16_t X = 0, Y = 0;
  int8_t VX = 0, VY = 0;

  //!< パレット
  {
    pal_all((const char*)Palette);
  }

  //!< ネームテーブル、アトリビュート
  {
    //!< A
    vram_adr(NAMETABLE_A);
    vram_fill(0x14, NT_TILE_WIDTH * NT_TILE_HEIGHT);
    //!< ネームテーブルの直後にアトリビュートテーブルのアドレスがあるので、このケースでは明示的に vram_adr() を指定する必要は無い
    //vram_adr(NAMETABLE_A + NT_TILE_WIDTH * NT_TILE_HEIGHT);
    vram_write(AttributesA, sizeof(AttributesA));

    //!< B
    vram_adr(NAMETABLE_B);
    vram_fill(0x16, NT_TILE_WIDTH * NT_TILE_HEIGHT);
    vram_write(AttributesB, sizeof(AttributesB));

#ifdef USE_SPLIT
    //!< コリジョンが発生するように、左下のピクセルが透明色でないパターンを置いておく
    put_str(NTADR_A(1, 3), "]");
#endif
  }
  
#ifdef USE_SPLIT
  {
    oam_clear();
    //!< スプライト0 と BG のピクセル(透明色でない事)が衝突した時に PPU_STATUS レジスタがセットされる
    //!< パターン0xa0 は 上部に水平の1ドットラインがあり、残りは透明色になっているようなパターン
    oam_spr(1, 30, 0xa0, OAM_BEHIND, 0);
  }
#endif

  pal_bright(Fade >> 4);

#ifdef USE_VRAM_BUFFER
  //!< VRAM バッファ
  {
    VramBufferSize = 64;
    VRAMBUF_CLEAR;
    //!< VRAM バッファのアドレスを知らせておく
    set_vram_update(VramBuffer);
  }
#endif

  //!< PPU 制御
  {
    ppu_mask(MASK_BG | MASK_SPR);

    ppu_on_all();
  }

  while (1) {
    //!< フェード
    {
      Fade = MIN(++Fade, 64);
      pal_bright(Fade >> 4);
    }

    //!< パッド
    {
      const uint8_t Cur = pad_poll(0);

      VX = VY = 0;
      if(Cur & PAD_UP) { VY = -1; }
      if(Cur & PAD_DOWN) { VY = 1; }
      if(Cur & PAD_LEFT) { VX = -1; }
      if(Cur & PAD_RIGHT) { VX = 1; }
      if(Cur & PAD_B) { VX <<= 1; }

      X = MIN(MAX(X + VX, 0), NT_WIDTH2 - 1); 
      Y = MIN(MAX(Y + VY, 0), NT_HEIGHT2 - 1); 
    }

#ifdef USE_VRAM_BUFFER
    {
      vrambuf_put(NTADR_A(2, 2), "HELLO", NT_UPD_HORZ);
      vrambuf_put(NTADR_A(2, 2), "HELLO", NT_UPD_VERT);
    }
#endif

#ifdef USE_SPLIT
    //!< スプライト0 とBGパターンとのコリジョンを待ってから、x スクロール値をセットする(y スクロール値は無視される)
    //!< コリジョンを待つため、分割は画面上部の方が良く、画面下の方である程無駄に待つことになる、コリジョンが発生しないと何も描画されない事になるので注意
    split(X, Y);
#else
    //!< スクロール
    scroll(X, Y);
#endif

    ppu_wait_nmi();
  }

  ppu_off();
}

	
	