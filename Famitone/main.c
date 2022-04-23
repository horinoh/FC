 #include "../fc.h"

//!< FamiTracker : http://famitracker.com/

//!< Famitone
void __fastcall__ famitone_update();
extern char music0[];
extern char music1[];
extern char sfx[];

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
  bool Pause = false;

  //!< パレット
  {
    pal_all((const char*)Palette);
  }

  put_str(NTADR_A(2, 2), "A    : MUSIC");
  put_str(NTADR_A(2, 3), "UDLR : SFX");

  pal_bright(Fade >> 4);

  //!< PPU 制御
  {
    ppu_mask(MASK_BG | MASK_SPR);

    ppu_on_all();
  }

  //!< Famitone
  {
    famitone_init(music0);
    //famitone_init(music1);
    sfx_init(sfx);
    nmi_set_callback(famitone_update);
  }

  while (1) {
    //!< フェード [0, 8]
    {
      Fade = MIN(++Fade, 64);
      pal_bright(Fade >> 4);
    }

   //!< パッド
    {
      const uint8_t Pre = pad_state(0);
      const uint8_t Cur = pad_poll(0);
      const uint8_t Chg = Pre ^ Cur;

#define TRG(x) (Chg & (x)) & (Cur & (x))
      if(TRG(PAD_A)) { music_play(0); }
      if(TRG(PAD_B)) { music_stop(); }
      if(TRG(PAD_START)) { Pause = !Pause; music_pause(Pause); }
      if(TRG(PAD_UP)) { sfx_play(0, SFX_CH0); }
      if(TRG(PAD_DOWN)) { sfx_play(1, SFX_CH1); }
      if(TRG(PAD_LEFT)) { sfx_play(2, SFX_CH2); }
      if(TRG(PAD_RIGHT)) { sfx_play(3, SFX_CH3); }
#undef TRG
    }

    //!< PAL, NTSC を問わず、次のフレームまでウエイト
    ppu_wait_nmi();
  }

  ppu_off();
}

	
	