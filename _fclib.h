#ifndef __FCLIB_H__
#define __FCLIB_H__

#include "define.h"

//!< BG
void BGNameTableClear(const u16 addr)
{
    int i;
    SET_PPUADDR(addr);
    for(i = 0;i < TILE_WIDTH * TILE_HEIGHT;++i) {
         PPUDATA = 0;
    }
}
void BGPutNameTable(const u16 addr, const u16 x, const u16 y, const u8 patternNo)
{
    SET_PPUADDR(addr + x + ((u16)(y<<5)));
    PPUDATA = patternNo;
}

//!< スプライト
struct Sprite
{
    u8 Y;
    u8 TileNo;
    u8 Attribute;
    u8 X;
};
typedef struct Sprite Sprite;
void SpriteSetPosLT(Sprite* sp, const u8 x, const u8 y) { sp->X = x; sp->Y = y; }
void SpriteSetPosRT(Sprite* sp, const u8 x, const u8 y) { SpriteSetPosLT(sp, x - 8, y); }
void SpriteSetPosLB(Sprite* sp, const u8 x, const u8 y) { SpriteSetPosLT(sp, x, y - 8); }
void SpriteSetPosRB(Sprite* sp, const u8 x, const u8 y) { SpriteSetPosLT(sp, x - 8, y - 8); }
void SpriteSetPosCC(Sprite* sp, const u8 x, const u8 y) { SpriteSetPosLT(sp, x - 4, y - 4); }
void SpriteRemove(Sprite* sp) { SpriteSetPosLT(sp, 0, BG_HEIGHT); } //!< Y 軸画面外へ
void SpriteClear(Sprite* sp)
{
    u8 i;
    for(i = 0;i < SPR_MAX;++i){
        SpriteRemove(&sp[i]);
    }
}
struct Sprite16
{
    Sprite* sprites[4];
};
typedef struct Sprite16 Sprite16;
void Sprite16Init(Sprite16* sp16, Sprite* sp)
{
    sp16->sprites[0] = &sp[0];
    sp16->sprites[1] = &sp[1];
    sp16->sprites[2] = &sp[2];
    sp16->sprites[3] = &sp[3];
}
void Sprite16SetPosLT(Sprite16* sp16, const u8 x, const u8 y)
{
    const u8 h = (sp16->sprites[0]->Attribute & SPR_ATTR_HFLIP_ON) >> 6;
    const u8 v = (sp16->sprites[0]->Attribute & SPR_ATTR_VFLIP_ON) >> 7;
    SpriteSetPosLT(sp16->sprites[h | v << 1], x, y);
    SpriteSetPosLT(sp16->sprites[!h | v << 1], x + 8, y);
    SpriteSetPosLT(sp16->sprites[h | !v << 1], x, y + 8);
    SpriteSetPosLT(sp16->sprites[!h | !v << 1], x + 8, y + 8);
}
void Sprite16SetPosRT(Sprite16* sp16, const u8 x, const u8 y) { Sprite16SetPosLT(sp16, x - 16, y); }
void Sprite16SetPosLB(Sprite16* sp16, const u8 x, const u8 y) { Sprite16SetPosLT(sp16, x, y - 16); }
void Sprite16SetPosRB(Sprite16* sp16, const u8 x, const u8 y) { Sprite16SetPosLT(sp16, x - 16, y - 16); }
void Sprite16SetPosCC(Sprite16* sp16, const u8 x, const u8 y) { Sprite16SetPosLT(sp16, x - 8, y - 8); }
void Sprite16Remove(Sprite16* sp16) 
{
    SpriteRemove(sp16->sprites[0]);
    SpriteRemove(sp16->sprites[1]);
    SpriteRemove(sp16->sprites[2]);
    SpriteRemove(sp16->sprites[3]);
}

//!< パッド
u8 ReadJoypad1()
{
    u8 i;
    u8 keys = 0;
    JOYPAD1 = 1;
    JOYPAD1 = 0;
    for(i = 0; i < 8; i++) {
        if(JOYPAD1 & 1) {
            keys |= 1 << i;
        }
    }
    return keys;
}
u8 ReadJoypad2()
{
    u8 i;
    u8 keys = 0;
    JOYPAD2 = 1;
    JOYPAD2 = 0;
    for(i = 0; i < 8; i++) {
        if(JOYPAD2 & 1) {
            keys |= 1 << i;
        }
    }
    return keys;
}

//!< サウンド
//!< 矩形波1
void Pulse1(u16 hz)
{
    //!< (1790000 / 16) / 音の周波数 - 1
    const u16 tmp = (1790000 >> 4) / hz - 1;
    PULSE1_FREQ1 = SOUND_FREQ_L8(tmp); 
    PULSE1_FREQ2 = SOUND_FREQ_U3(tmp) | SOUND_LENGTH_VAL(0/*[0, 31]*/);
}
//!< 矩形波2
void Pulse2(u16 hz)
{
    const u16 tmp = (1790000 >> 4) / hz - 1;
    PULSE2_FREQ1 = SOUND_FREQ_L8(tmp); 
    PULSE2_FREQ2 = SOUND_FREQ_U3(tmp) | SOUND_LENGTH_VAL(0);
}
#endif //!< __FCLIB_H__