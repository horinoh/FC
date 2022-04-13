#include "../nes.h"

extern const uint8_t chr[];

void main() 
{
  const uint8_t Palette[] = {
        0x03, 0x11, 0x30, 0x27, //!< BG Pal0
        0x00, 0x1c, 0x20, 0x2c, //!< BG Pal1
        0x00, 0x00, 0x10, 0x20, //!< BG Pal2
        0x00, 0x06, 0x16, 0x26, //!< BG Pal3

        0x00, 0x16, 0x35, 0x24, //!< SP Pal0
        0x00, 0x00, 0x37, 0x25, //!< SP Pal1
        0x00, 0x0d, 0x2d, 0x3a, //!< SP Pal2
        0x00, 0x0d, 0x27, 0x2a, //!< SP Pal3
  };
  uint8_t Fade = 0;

  #define SPR_ATTR 0x0/*(OAM_FLIP_V | OAM_FLIP_H | OAM_BEHIND)*/
  const uint8_t MetaSprite[] = {
    0, 0, 0xd8 + 0, SPR_ATTR, 
    0, 8, 0xd8 + 1, SPR_ATTR, 
    8, 0, 0xd8 + 2, SPR_ATTR, 
    8, 8, 0xd8 + 3, SPR_ATTR, 
    OAM_END_OF_META_SPR,
  };
  uint8_t Oam_id = 0;

  //!< パレット
  {
    pal_all((const char*)Palette);
  }

  pal_bright(Fade >> 4);

  oam_clear();
  oam_size(OAM_SIZE_8x8);

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

    Oam_id = oam_meta_spr(32, 32, Oam_id, MetaSprite);
    if(Oam_id) {
      oam_hide_rest(Oam_id);
    }

    ppu_wait_nmi();
  }

  ppu_off();
}

	
	