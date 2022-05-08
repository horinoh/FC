#include "../fc.h"

//!< [ MMC3 ] 以下のファイルを使用してビルドする
//!<  config/mmc3.cfg
//!<  bank/mmc3.o (BuildBank.bat で予めビルドしておく)

//!< #TODO 

#pragma rodata-name("BANK00")
const char TEXT00[] = { "bank00" };
#pragma rodata-name("BANK01")
const char TEXT01[] = { "bank01" };
#pragma rodata-name("BANK02")
const char TEXT02[] = { "bank02" };
#pragma rodata-name("BANK03")
const char TEXT03[] = { "bank03" };
#pragma rodata-name("BANK00")

#pragma code-name("BANK00")
void put_str00(const uint16_t adr, const char* str) 
{
    vram_adr(adr);
    vram_write((const uint8_t*)str, strlen(str));
}
#pragma code-name("BANK01")
void put_str01(const uint16_t adr, const char* str) 
{
    vram_adr(adr);
    vram_write((const uint8_t*)str, strlen(str));
}

//!< 元のバンクに戻しておくこと
#pragma code-name("BANK00")

void __fastcall__ nmi() {}

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

  MMC3_PRG_8000(0);
  MMC3_PRG_8000(0);
  put_str00(NTADR_A(2, 2), TEXT02);

  pal_bright(Fade >> 4);

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

    ppu_wait_nmi();
  }

  ppu_off();
}

	
	