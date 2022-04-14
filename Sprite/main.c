#include "../nes.h"

#define USE_READ_VRAM

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

  #define SPR_ATTR 0x0/*(OAM_FLIP_V | OAM_FLIP_H | OAM_BEHIND)*/
  uint8_t Oam_id = 0;
  int16_t X = NT_WIDTH >> 1, Y = NT_HEIGHT >> 1;
  int8_t VX = 0, VY = 0;
  #define SPR_W 8
  #define SPR_H 8

  //!< パレット
  {
    pal_all((const char*)Palette);
  }

#ifdef USE_READ_VRAM
  put_str(NTADR_A(2, 2), "RGBRGBRGBRGBRGB");
  put_str(NTADR_A(2, 3), "RRRRRRRRRRRRRRR");
  put_str(NTADR_A(2, 4), "GGGGGGGGGGGGGGG");
  put_str(NTADR_A(2, 5), "BBBBBBBBBBBBBBB");
#endif

  pal_bright(Fade >> 4);

  //!< スプライト
  {
    oam_clear();
    oam_size(OAM_SIZE_8x8);
  }

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

      X = MIN(MAX(X + VX, 0), NT_WIDTH - SPR_W); 
      Y = MIN(MAX(Y + VY, 0), NT_HEIGHT - SPR_H); 
    }
    
    //!< スプライト
    {
      oam_clear();
      Oam_id = oam_spr(X, Y, 16, SPR_ATTR, Oam_id);
      if(Oam_id) { oam_hide_rest(Oam_id); }
    }
    
    ppu_wait_nmi();

#ifdef USE_READ_VRAM
    {
      uint8_t read;
      vram_adr(NTADR_A(X >> 3, Y >> 3));
      vram_read(&read, sizeof(read));
      //!< VRAM を読み込んでアルファベット('A')だったら背景色を変更する
      if(read == 'R') {
        pal_col(0, 0x16);
      } else if(read == 'G') {
        pal_col(0, 0x1a);
      } else if(read == 'B') {
        pal_col(0, 0x12);
      } else {
        pal_col(0, 0x03);
      }
      //!< VRAM アドレスは戻しておく必要がある
      vram_adr(0x0);
    }
#endif
  }

  ppu_off();
}

	
	