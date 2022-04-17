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
    //pal_bg((const char*)BGPalette);
    //pal_spr((const char*)SPRPalette);
    pal_all((const char*)Palette);
  }

  put_str(NTADR_A(2, 2), "HELLO WORLD");

  pal_bright(Fade >> 4);

  //!< PPU 制御
  {
    //!< モノクロ
    //ppu_mask(MASK_BG | MASK_SPR | MASK_MONO);
    //!< 淡色
    //ppu_mask(MASK_BG | MASK_SPR | (MASK_TINT_RED | MASK_TINT_GREEN | MASK_TINT_BLUE));
    //!< エッジを隠さない設定
    //ppu_mask(MASK_BG | MASK_SPR | (MASK_EDGE_BG | MASK_EDGE_SPR));
    //!< 画面左、上でキャラクターの一部が隠れるようにしたい場合はエッジを隠す設定(MASK_EDGE_*を使用しない)にする
    ppu_mask(MASK_BG | MASK_SPR);

    ppu_on_all();
    //ppu_on_bg();
    //ppu_on_spr();
  }

  while (1) {
    //!< フェード [0, 8]
    //!< pal_bright(0) == ブラックアウト
    //!< pal_bright(4) == 通常色
    //!< pal_bright(8) == ホワイトアウト
    {
      Fade = MIN(++Fade, 64);
      pal_bright(Fade >> 4);
      //pal_bg_bright(Fade >> 4)
      //pal_spr_bright(Fade >> 4);
    }

    //!< PAL, NTSC を問わず、次のフレームまでウエイト
    ppu_wait_nmi();
  }

  ppu_off();
}

	
	