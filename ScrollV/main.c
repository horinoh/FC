#include "../fc.h"

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
  const uint8_t AttributesA[] = {
    ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), ATTR(0, 1, 2, 3), //!< [ 0,  3] 行
    ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), ATTR(3, 0, 1, 2), //!< [ 4,  7] 行
    ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), ATTR(2, 3, 0, 1), //!< [ 8, 11] 行
    ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), ATTR(1, 2, 3, 0), //!< [12, 15] 行
    ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), ATTR(1, 1, 1, 1), //!< [16, 19] 行
    ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), ATTR(2, 2, 2, 2), //!< [20, 23] 行
    ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), ATTR(3, 3, 3, 3), //!< [24, 27] 行
    ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), ATTR(0, 0, 0, 0), //!< [28, 29] 行
  };
  const uint8_t AttributesC[] = {
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
  int16_t Y = 0;
  int8_t VY = 0;
  int16_t TileY = 0;

  //!< パレット
  {
    pal_all((const char*)Palette);
  }

  //!< ネームテーブル、アトリビュート
  {
    //!< A
    vram_adr(NAMETABLE_A);
    vram_fill('A', NT_TILE_WIDTH * NT_TILE_HEIGHT);
    vram_write(AttributesA, sizeof(AttributesA));

    //!< C
    vram_adr(NAMETABLE_C);
    vram_fill('C', NT_TILE_WIDTH * NT_TILE_HEIGHT);
    vram_write(AttributesC, sizeof(AttributesC));
  }
scroll(0, 30);

  pal_bright(Fade >> 4);

  //!< VRAM バッファ
  {
    VramBufferSize = 64;
    VRAMBUF_CLEAR;
    //!< VRAM バッファのアドレスを知らせておく
    set_vram_update(VramBuffer);
  }

  //!< PPU 制御
  {
    //!< エッジを隠さない設定
    //ppu_mask(MASK_BG | MASK_SPR | (MASK_EDGE_BG | MASK_EDGE_SPR));
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

      VY = 0;
      if(Cur & PAD_UP) { VY = -1; }
      if(Cur & PAD_DOWN) { VY = 1; }
      if(Cur & PAD_B) { VY <<= 1; }
    
      Y += VY;
      NT_HEIGHT_LIMIT(Y);

      TileY = (Y >> 3);
    }

    //!< スクロールインしてくるタイルを書き換える
    if(0 < VY) {
      #if 1
      TileY += NT_TILE_HEIGHT - 1;
      #else
      TileY += NT_TILE_HEIGHT - 2; //!< 書き換えが見えるようにする場合
      #endif

      NT_TILE_HEIGHT_LIMIT(TileY);
      vrambuf_put_h(NTADR_HORZ(0, TileY), "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv"); //!< 1つ足りなくしている(わざと)
    } else if(0 > VY) {
      #if 1
      TileY += 1;
      #else
      TileY += 2; //!< 書き換えが見えるようにする場合
      #endif

      NT_TILE_HEIGHT_LIMIT(TileY);
      vrambuf_put_h(NTADR_HORZ(0, TileY), "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"); //!< 1つ足りなくしている(わざと)
    }

    //!< スクロール
    scroll(0, Y);

    ppu_wait_nmi();
  }

  ppu_off();
}

	
	