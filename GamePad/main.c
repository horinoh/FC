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
  uint8_t Fade = 0;

  //!< パレット
  {
    pal_all((const char*)Palette);
  }

  pal_bright(Fade >> 4);

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
      const uint8_t Pre = pad_state(0);
      const uint8_t Cur = pad_poll(0);
      const uint8_t Chg = Pre ^ Cur;

#define TRG(x) (Chg & (x)) & (Cur & (x));
#define REL(x) (Chg & (x)) & (Pre & (x));
      if(Cur & PAD_A) {}
      if(Cur & PAD_B) {}
      if(Cur & PAD_SELECT) {}
      if(Cur & PAD_START) {}
      if(Cur & PAD_UP) {}
      if(Cur & PAD_DOWN) {}
      if(Cur & PAD_LEFT) {}
      if(Cur & PAD_RIGHT) {}
#undef REL
#undef TRG
    }

    ppu_wait_nmi();
  }

  ppu_off();
}

	
	