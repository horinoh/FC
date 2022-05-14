#include "../fc.h"

//!< [ MMC3 ] 以下のファイルを使用してビルドする
//!<  config/mmc3.cfg
//!<  bank/mmc3.o (BuildBank.bat で予めビルドしておく)

#pragma rodata-name(push, "PBANK00")
const char TEXT00[] = { "Bank00" };
#pragma rodata-name("PBANK01")
const char TEXT01[] = { "Bank01" };
#pragma rodata-name("PBANK02")
const char TEXT02[] = { "Bank02" };
#pragma rodata-name("PBANK03")
const char TEXT03[] = { "Bank03" };
#pragma rodata-name("PBANK04")
const char TEXT04[] = { "Bank04" };
#pragma rodata-name("PBANK05")
const char TEXT05[] = { "Bank05" };
//!< 元のバンクに戻しておくこと
#pragma rodata-name(pop)

#pragma code-name(push, "PBANK00")
void put_str00(const uint16_t adr, const char* str) 
{
    uint8_t i;
    char tmp[32];
    for(i = 0;i < strlen(str);++i){
      tmp[i] = tolower(str[i]);
    }

    vram_adr(adr);
    vram_write((const uint8_t*)tmp, strlen(tmp));
}
#pragma code-name("PBANK01")
void put_str01(const uint16_t adr, const char* str) 
{
    uint8_t i;
    char tmp[32];
    for(i = 0;i < strlen(str);++i){
      tmp[i] = toupper(str[i]);
    }

    vram_adr(adr);
    vram_write((const uint8_t*)tmp, strlen(tmp));
}
//!< 元のバンクに戻しておくこと
#pragma code-name(pop)

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

  put_str(NTADR_A(2, 2), "UDLR+A : PRG CHANGE");
  put_str(NTADR_A(2, 3), "UDLR+B : CHR CHANGE");

  //!< バンク切替
  {
    MMC3_PRG_8000(0);
    put_str(NTADR_A(2, 8), TEXT00);

    MMC3_PRG_8000(1);
    put_str(NTADR_A(2, 9), TEXT01);

    MMC3_PRG_8000(2);
    put_str(NTADR_A(2, 10), TEXT02);

    MMC3_PRG_A000(3);
    put_str(NTADR_A(2, 11), TEXT03);

    MMC3_PRG_A000(4);
    put_str(NTADR_A(2, 12), TEXT04);

    MMC3_PRG_A000(5);
    put_str(NTADR_A(2, 13), TEXT05);

    MMC3_PRG_8000(0); //!< 関数
    MMC3_PRG_A000(4); //!< 引数
    put_str00(NTADR_A(2, 15), TEXT04);

    MMC3_PRG_8000(1);
    MMC3_PRG_A000(5);
    put_str01(NTADR_A(2, 16), TEXT05);
  }

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

    //!< パッド
    {
      const uint8_t Pre = pad_state(0);
      const uint8_t Cur = pad_poll(0);
      const uint8_t Chg = Pre ^ Cur;

#define TRG(x) (Chg & (x)) & (Cur & (x))
#define REL(x) (Chg & (x)) & (Pre & (x))
      //!< PRG バンク切替
      if(TRG(PAD_A)) {
        if(Cur & PAD_UP) {
          MMC3_PRG_8000(0);
          vrambuf_put_h(NTADR_A(20, 8), TEXT00);
        }
        if(Cur & PAD_DOWN) {
          MMC3_PRG_8000(1);
          vrambuf_put_h(NTADR_A(20, 8), TEXT01);
        }
        if(Cur & PAD_LEFT) {
          MMC3_PRG_8000(2);
          vrambuf_put_h(NTADR_A(20, 8), TEXT02);
        }
        if(Cur & PAD_RIGHT) {
          MMC3_PRG_A000(3);
          vrambuf_put_h(NTADR_A(20, 8), TEXT03);
        }
      }
      //!< CHR バンク切替 (引数の値は mmc3.s 参照)
      if(Cur & PAD_B) {
        if(Cur & PAD_UP) {
          MMC3_CHR_0000(0); //!< 先頭なので 0
        }
        if(Cur & PAD_DOWN) {
          MMC3_CHR_0000(8); //!< 2つ目までのデータサイズが 8 KB なので
        }
        if(Cur & PAD_LEFT) {
          MMC3_CHR_0000(16); //!< 3つ目までのデータサイズが 8 + 8 = 16 KB なので
        }
        if(Cur & PAD_RIGHT) {
          MMC3_CHR_0000(20); //!< 4つ目までのデータサイズが 8 + 8 + 4 = 20 KB なので
        }
      }
#undef REL
#undef TRG
    }

    ppu_wait_nmi();
  }

  ppu_off();
}

	
	