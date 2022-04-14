#include "../nes.h"

#define SPR_ATTR_DEFAULT 0x0/*(OAM_FLIP_V | OAM_FLIP_H | OAM_BEHIND)*/
#define OAM_ANIM_16X16(name, pattern, attr) const uint8_t name[] = {\
  0, 0, (pattern) + 0, (attr),\
  0, 8, (pattern) + 1, (attr),\
  8, 0, (pattern) + 2, (attr),\
  8, 8, (pattern) + 3, (attr),\
  OAM_END_OF_META_SPR,\
}
#define OAM_ANIM_16X16_FLIP_H(name, pattern, attr) const uint8_t name[] = {\
  8, 0, (pattern) + 0, (attr) | OAM_FLIP_H,\
  8, 8, (pattern) + 1, (attr) | OAM_FLIP_H,\
  0, 0, (pattern) + 2, (attr) | OAM_FLIP_H,\
  0, 8, (pattern) + 3, (attr) | OAM_FLIP_H,\
  OAM_END_OF_META_SPR,\
}
OAM_ANIM_16X16(RStand, 0xd8, SPR_ATTR_DEFAULT);
OAM_ANIM_16X16(RRun1, 0xdc, SPR_ATTR_DEFAULT);
OAM_ANIM_16X16(RRun2, 0xe0, SPR_ATTR_DEFAULT);
OAM_ANIM_16X16(RRun3, 0xe4, SPR_ATTR_DEFAULT);
OAM_ANIM_16X16(RJump, 0xe8, SPR_ATTR_DEFAULT);
OAM_ANIM_16X16(RClimb, 0xec, SPR_ATTR_DEFAULT);
OAM_ANIM_16X16(RSad, 0xf0, SPR_ATTR_DEFAULT);

OAM_ANIM_16X16_FLIP_H(LStand, 0xd8, SPR_ATTR_DEFAULT);
OAM_ANIM_16X16_FLIP_H(LRun1, 0xdc, SPR_ATTR_DEFAULT);
OAM_ANIM_16X16_FLIP_H(LRun2, 0xe0, SPR_ATTR_DEFAULT);
OAM_ANIM_16X16_FLIP_H(LRun3, 0xe4, SPR_ATTR_DEFAULT);
OAM_ANIM_16X16_FLIP_H(LJump, 0xe8, SPR_ATTR_DEFAULT);
OAM_ANIM_16X16_FLIP_H(LClimb, 0xec, SPR_ATTR_DEFAULT);
OAM_ANIM_16X16_FLIP_H(LSad, 0xf0, SPR_ATTR_DEFAULT);

//!< アニメーションシーケンス 8 要素にすることで X & 7 で参照できるようにしている
const uint8_t* RRunAnimSeq[] = { 
  //RRun1, RRun2, RRun3, RRun1, RRun2, RRun3, RRun1, RRun2,
  RRun1, RRun1, RRun1, RRun2, RRun2, RRun2, RRun3, RRun3,
};
const uint8_t* LRunAnimSeq[] = { 
  //LRun1, LRun2, LRun3, LRun1, LRun2, LRun3, LRun1, LRun2,
  LRun1, LRun1, LRun1, LRun2, LRun2, LRun2, LRun3, LRun3,
};
const uint8_t* ClimbAnimSeq[] = {
  RClimb, RClimb, RClimb, RClimb, LClimb, LClimb, LClimb, LClimb,
};

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

  int16_t X = NT_WIDTH >> 1, Y = NT_HEIGHT >> 1;
  int8_t VX = 0, VY = 0;
  uint8_t AnimSeq = 0;
  #define SPR_W 16
  #define SPR_H 16

  //!< パレット
  {
    pal_all((const char*)Palette);
  }

  pal_bright(Fade >> 4);

  //!< スプライト
  {
    oam_clear();
    oam_size(OAM_SIZE_8x8);
  }

  //!< PPU 制御
  {
    //!< エッジを隠さない設定
    //ppu_mask(MASK_BG | MASK_SPR | MASK_EDGE_BG | MASK_EDGE_SPR);
    //!< エッジを隠す設定
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

    //!< メタスプライト
    {
      oam_clear_fast();
      if(VX) {
        oam_meta_spr_clip(X, Y, (VX > 0 ? RRunAnimSeq : LRunAnimSeq)[X & 7]);
      } else if(VY) {
        oam_meta_spr_clip(X, Y, ClimbAnimSeq[Y & 7]);
      }
      else {
        oam_meta_spr_clip(X, Y, (rand8() & 1) ? RSad : LSad);
      }
    }

    ppu_wait_nmi();
  }

  ppu_off();
}

	
	