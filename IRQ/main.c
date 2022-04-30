#include "../fc.h"

//!< [ MMC3 ] 以下のファイルを使用してビルドする
//!<  config/mmc3.cfg
//!<  bank/mmc3.o (BuildBank.bat で予めビルドしておく)

//!< [ スキャンラインカウンタ ]
//!<  スキャンライン毎にデクリメントされる内部カウンタ(レジスタを介してセット可能)を持つ
//!<  カウンタがゼロになった時に IRQ が発行されカウンタはリロードされる

//!< スクロール量、(ここでは) 8 スキャンライン毎なので 30 あれば十分
uint16_t ScrollAmount[32];
//!< (ここでは) 8 スキャンライン毎なので [0, 30]
uint8_t IRQCount = 0;

//!< IRQ, NMI 割込みをまとめて扱う
void __fastcall__ nmi() 
{
  if(IS_IRQ){
    //!< [ IRQ の場合 ]
    
    //!< スクロール X
    PPU.scroll = ScrollAmount[IRQCount & (COUNTOF(ScrollAmount) - 1)] >> 8; 
    //!< スクロール Y
    PPU.scroll = 0;                                             

    ++IRQCount;

    //!< IRQ は一度発行されたら終わり、再度 IRQ を発行する為に Disable, Enable する必要がある
    MMC3_IRQ_DISABLE();
    MMC3_IRQ_ENABLE();
  } else {
    //!< [ NMI の場合 ]
    
    //!< IRQ でないのでカウンタはリロードされない、明示的にリロードする
    MMC3_IRQ_RELOAD();
    
    IRQCount = 0;
  }
}

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
  uint8_t i;
  char Str[8];

  {
    set_ppu_ctrl_var(get_ppu_ctrl_var() | 0x08);
    //POKE(0xA001, 0x80);

    //!< 水平ミラーリング
    MMC3_MIRROR_HORZ;
  
    //!< ここでは 8 スキャンライン毎に IRQ を発行する設定
    MMC3_IRQ_SET_VALUE(7);
    MMC3_IRQ_RELOAD();
    MMC3_IRQ_ENABLE();

    //!< IRQ を有効にする
    ENABLE_CPU_IRQ;
  }

  //!< パレット
  {
    pal_all((const char*)Palette);
  }

  //!< ネームテーブル
  {
    vram_adr(NTADR_A(0, 0));
    vram_fill(20, NT_TILE_WIDTH * NT_TILE_HEIGHT);
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

    //!< スクロール量の計算
    for (i = 0; i < COUNTOF(ScrollAmount); i++) {
      ScrollAmount[i] += i << 4;
    }

    //!< スクロール量のデバッグ表示
    {
      sprintf(Str, "%u", ScrollAmount[10] >> 8);
      vrambuf_put_h(NTADR_A(0, 10), Str);

      sprintf(Str, "%u", ScrollAmount[20] >> 8);
      vrambuf_put_h(NTADR_A(0, 20), Str);
    }

    ppu_wait_nmi();
  }

  ppu_off();
}

	
	