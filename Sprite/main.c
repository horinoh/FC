#include "../fclib.h"

//!< OAM DMA 転送元となるワーク、下位 8 ビットが 0x00 である必要がある
//!< ここでは [0x700, 0x7ff] を使用することとする
#define SPR_WORK_ADDR 0x700

//!<
//!< 【パレット】
//!< 
#define TRCOLOR COLOR_SKYBLUE //!< 透明色
#define NOTUSED 0
const u8 Palettes[] = { 
    TRCOLOR, COLOR_RED, COLOR_BEIGE, COLOR_BROWN, //!< パレット 0
    NOTUSED, 0, 0, 0, //!< パレット 1
    NOTUSED, 0, 0, 0, //!< パレット 2
    NOTUSED, 0, 0, 0, //!< パレット 3

    TRCOLOR, COLOR_RED, COLOR_BEIGE, COLOR_BROWN, //!< パレット 0
    NOTUSED, 0, 0, 0, //!< パレット 1
    NOTUSED, 0, 0, 0, //!< パレット 2
    NOTUSED, 0, 0, 0, //!< パレット 3
};
#undef NOTUSED
#undef BGCOLOR

//!<
//!< 【パターン】 0x8 x 256 個まで
//!< 
#define TR 0
#define C0 1
#define C1 2
#define C2 3
#define M0 1
#define M1 2
//0,0,0,0,0,1,1,1, 1,1,0,0,0,0,0,0,
//0,0,0,0,1,1,1,1, 1,1,1,1,1,0,0,0,
//0,0,0,0,3,3,3,2, 2,3,2,0,0,0,0,0,
//0,0,0,3,2,3,2,2, 2,3,2,2,2,0,0,0,
//0,0,0,3,2,3,3,2, 2,2,3,2,2,2,0,0,
//0,0,0,3,3,2,2,2, 2,3,3,3,3,0,0,0,
//0,0,0,0,0,2,2,2, 2,2,2,2,0,0,0,0,
//0,0,0,0,3,3,1,3, 3,3,0,0,0,0,0,0,

//0,0,0,3,3,3,1,3, 3,1,3,3,3,0,0,0,
//0,0,3,3,3,3,1,1, 1,1,3,3,3,3,0,0,
//0,0,2,2,3,1,2,1, 1,2,1,3,2,2,0,0,
//0,0,2,2,2,1,1,1, 1,1,1,2,2,2,0,0,
//0,0,2,2,1,1,1,1, 1,1,1,1,2,2,0,0,
//0,0,0,0,1,1,1,0, 0,1,1,1,0,0,0,0,
//0,0,0,3,3,3,0,0, 0,0,3,3,3,0,0,0,
//0,0,3,3,3,3,0,0, 0,0,3,3,3,3,0,0,
const u8 Patterns[] = {
#if 0
    0x03,0x07,0x07,0x07,0x07,0x07,0x03,0x3d,
    0x00,0x00,0x01,0x03,0x01,0x01,0x01,0x3f, 
    
    0xe0,0xf0,0xe0,0xe0,0xf0,0xf3,0xc7,0xbe,
    0x00,0x00,0xe0,0xa0,0xf0,0xf3,0xc7,0xfe, 
    
    0x78,0xe0,0xc7,0x00,0x7c,0x7c,0x60,0x40,
    0x7f,0xe7,0xc0,0x07,0x1e,0x3c,0x20,0x00, 
    
    0x1c,0x00,0xe8,0x0c,0x0c,0x0e,0x0f,0x00,
    0xfc,0xe0,0x18,0xfc,0x0c,0x06,0x00,0x00, 
#else
    //!< パターン 0
    (TR & M0) << 7 | (TR & M0) << 6 | (TR & M0) << 5 | (TR & M0) << 4 | (TR & M0) << 3 | (C0 & M0) << 2 | (C0 & M0) << 1 | (C0 & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (TR & M0) << 5 | (TR & M0) << 4 | (C0 & M0) << 3 | (C0 & M0) << 2 | (C0 & M0) << 1 | (C0 & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (TR & M0) << 5 | (TR & M0) << 4 | (C2 & M0) << 3 | (C2 & M0) << 2 | (C2 & M0) << 1 | (C1 & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (TR & M0) << 5 | (C2 & M0) << 4 | (C1 & M0) << 3 | (C2 & M0) << 2 | (C1 & M0) << 1 | (C1 & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (TR & M0) << 5 | (C2 & M0) << 4 | (C1 & M0) << 3 | (C2 & M0) << 2 | (C2 & M0) << 1 | (C1 & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (TR & M0) << 5 | (C2 & M0) << 4 | (C2 & M0) << 3 | (C1 & M0) << 2 | (C1 & M0) << 1 | (C1 & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (TR & M0) << 5 | (TR & M0) << 4 | (TR & M0) << 3 | (C1 & M0) << 2 | (C1 & M0) << 1 | (C1 & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (TR & M0) << 5 | (TR & M0) << 4 | (C2 & M0) << 3 | (C2 & M0) << 2 | (C0 & M0) << 1 | (C2 & M0) << 0, 

    (TR & M1) << 7 | (TR & M1) << 6 | (TR & M1) << 5 | (TR & M1) << 4 | (TR & M1) << 3 | (C0 & M1) << 2 | (C0 & M1) << 1 | (C0 & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (TR & M1) << 5 | (TR & M1) << 4 | (C0 & M1) << 3 | (C0 & M1) << 2 | (C0 & M1) << 1 | (C0 & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (TR & M1) << 5 | (TR & M1) << 4 | (C2 & M1) << 3 | (C2 & M1) << 2 | (C2 & M1) << 1 | (C1 & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (TR & M1) << 5 | (C2 & M1) << 4 | (C1 & M1) << 3 | (C2 & M1) << 2 | (C1 & M1) << 1 | (C1 & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (TR & M1) << 5 | (C2 & M1) << 4 | (C1 & M1) << 3 | (C2 & M1) << 2 | (C2 & M1) << 1 | (C1 & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (TR & M1) << 5 | (C2 & M1) << 4 | (C2 & M1) << 3 | (C1 & M1) << 2 | (C1 & M1) << 1 | (C1 & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (TR & M1) << 5 | (TR & M1) << 4 | (TR & M1) << 3 | (C1 & M1) << 2 | (C1 & M1) << 1 | (C1 & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (TR & M1) << 5 | (TR & M1) << 4 | (C2 & M1) << 3 | (C2 & M1) << 2 | (C0 & M1) << 1 | (C2 & M1) << 0, 

    //!< パターン 1
    (C0 & M0) << 7 | (C0 & M0) << 6 | (TR & M0) << 5 | (TR & M0) << 4 | (TR & M0) << 3 | (TR & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (C0 & M0) << 7 | (C0 & M0) << 6 | (C0 & M0) << 5 | (C0 & M0) << 4 | (C0 & M0) << 3 | (TR & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (C1 & M0) << 7 | (C2 & M0) << 6 | (C1 & M0) << 5 | (TR & M0) << 4 | (TR & M0) << 3 | (TR & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (C1 & M0) << 7 | (C2 & M0) << 6 | (C1 & M0) << 5 | (C1 & M0) << 4 | (C1 & M0) << 3 | (TR & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (C1 & M0) << 7 | (C1 & M0) << 6 | (C2 & M0) << 5 | (C1 & M0) << 4 | (C1 & M0) << 3 | (C1 & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (C1 & M0) << 7 | (C2 & M0) << 6 | (C2 & M0) << 5 | (C2 & M0) << 4 | (C2 & M0) << 3 | (TR & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (C1 & M0) << 7 | (C1 & M0) << 6 | (C1 & M0) << 5 | (C1 & M0) << 4 | (TR & M0) << 3 | (TR & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (C2 & M0) << 7 | (C2 & M0) << 6 | (TR & M0) << 5 | (TR & M0) << 4 | (TR & M0) << 3 | (TR & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,

    (C0 & M1) << 7 | (C0 & M1) << 6 | (TR & M1) << 5 | (TR & M1) << 4 | (TR & M1) << 3 | (TR & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (C0 & M1) << 7 | (C0 & M1) << 6 | (C0 & M1) << 5 | (C0 & M1) << 4 | (C0 & M1) << 3 | (TR & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (C1 & M1) << 7 | (C2 & M1) << 6 | (C1 & M1) << 5 | (TR & M1) << 4 | (TR & M1) << 3 | (TR & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (C1 & M1) << 7 | (C2 & M1) << 6 | (C1 & M1) << 5 | (C1 & M1) << 4 | (C1 & M1) << 3 | (TR & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (C1 & M1) << 7 | (C1 & M1) << 6 | (C2 & M1) << 5 | (C1 & M1) << 4 | (C1 & M1) << 3 | (C1 & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (C1 & M1) << 7 | (C2 & M1) << 6 | (C2 & M1) << 5 | (C2 & M1) << 4 | (C2 & M1) << 3 | (TR & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (C1 & M1) << 7 | (C1 & M1) << 6 | (C1 & M1) << 5 | (C1 & M1) << 4 | (TR & M1) << 3 | (TR & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (C2 & M1) << 7 | (C2 & M1) << 6 | (TR & M1) << 5 | (TR & M1) << 4 | (TR & M1) << 3 | (TR & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,

    //!< パターン 2
    (TR & M0) << 7 | (TR & M0) << 6 | (TR & M0) << 5 | (C2 & M0) << 4 | (C2 & M0) << 3 | (C2 & M0) << 2 | (C0 & M0) << 1 | (C2 & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (C2 & M0) << 5 | (C2 & M0) << 4 | (C2 & M0) << 3 | (C2 & M0) << 2 | (C0 & M0) << 1 | (C0 & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (C1 & M0) << 5 | (C1 & M0) << 4 | (C2 & M0) << 3 | (C0 & M0) << 2 | (C1 & M0) << 1 | (C0 & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (C1 & M0) << 5 | (C1 & M0) << 4 | (C1 & M0) << 3 | (C0 & M0) << 2 | (C0 & M0) << 1 | (C0 & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (C1 & M0) << 5 | (C1 & M0) << 4 | (C0 & M0) << 3 | (C0 & M0) << 2 | (C0 & M0) << 1 | (C0 & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (TR & M0) << 5 | (TR & M0) << 4 | (C0 & M0) << 3 | (C0 & M0) << 2 | (C0 & M0) << 1 | (TR & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (TR & M0) << 5 | (C2 & M0) << 4 | (C2 & M0) << 3 | (C2 & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0, 
    (TR & M0) << 7 | (TR & M0) << 6 | (C2 & M0) << 5 | (C2 & M0) << 4 | (C2 & M0) << 3 | (C2 & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0, 

    (TR & M1) << 7 | (TR & M1) << 6 | (TR & M1) << 5 | (C2 & M1) << 4 | (C2 & M1) << 3 | (C2 & M1) << 2 | (C0 & M1) << 1 | (C2 & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (C2 & M1) << 5 | (C2 & M1) << 4 | (C2 & M1) << 3 | (C2 & M1) << 2 | (C0 & M1) << 1 | (C0 & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (C1 & M1) << 5 | (C1 & M1) << 4 | (C2 & M1) << 3 | (C0 & M1) << 2 | (C1 & M1) << 1 | (C0 & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (C1 & M1) << 5 | (C1 & M1) << 4 | (C1 & M1) << 3 | (C0 & M1) << 2 | (C0 & M1) << 1 | (C0 & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (C1 & M1) << 5 | (C1 & M1) << 4 | (C0 & M1) << 3 | (C0 & M1) << 2 | (C0 & M1) << 1 | (C0 & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (TR & M1) << 5 | (TR & M1) << 4 | (C0 & M1) << 3 | (C0 & M1) << 2 | (C0 & M1) << 1 | (TR & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (TR & M1) << 5 | (C2 & M1) << 4 | (C2 & M1) << 3 | (C2 & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0, 
    (TR & M1) << 7 | (TR & M1) << 6 | (C2 & M1) << 5 | (C2 & M1) << 4 | (C2 & M1) << 3 | (C2 & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0, 

    //!< パターン 3
    (C2 & M0) << 7 | (C0 & M0) << 6 | (C2 & M0) << 5 | (C2 & M0) << 4 | (C2 & M0) << 3 | (TR & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (C0 & M0) << 7 | (C0 & M0) << 6 | (C2 & M0) << 5 | (C2 & M0) << 4 | (C2 & M0) << 3 | (C2 & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (C0 & M0) << 7 | (C1 & M0) << 6 | (C0 & M0) << 5 | (C2 & M0) << 4 | (C1 & M0) << 3 | (C1 & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (C0 & M0) << 7 | (C0 & M0) << 6 | (C0 & M0) << 5 | (C1 & M0) << 4 | (C1 & M0) << 3 | (C1 & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (C0 & M0) << 7 | (C0 & M0) << 6 | (C0 & M0) << 5 | (C0 & M0) << 4 | (C1 & M0) << 3 | (C1 & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (TR & M0) << 7 | (C0 & M0) << 6 | (C0 & M0) << 5 | (C0 & M0) << 4 | (TR & M0) << 3 | (TR & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (TR & M0) << 7 | (TR & M0) << 6 | (C2 & M0) << 5 | (C2 & M0) << 4 | (C2 & M0) << 3 | (TR & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,
    (TR & M0) << 7 | (TR & M0) << 6 | (C2 & M0) << 5 | (C2 & M0) << 4 | (C2 & M0) << 3 | (C2 & M0) << 2 | (TR & M0) << 1 | (TR & M0) << 0,

    (C2 & M1) << 7 | (C0 & M1) << 6 | (C2 & M1) << 5 | (C2 & M1) << 4 | (C2 & M1) << 3 | (TR & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (C0 & M1) << 7 | (C0 & M1) << 6 | (C2 & M1) << 5 | (C2 & M1) << 4 | (C2 & M1) << 3 | (C2 & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (C0 & M1) << 7 | (C1 & M1) << 6 | (C0 & M1) << 5 | (C2 & M1) << 4 | (C1 & M1) << 3 | (C1 & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (C0 & M1) << 7 | (C0 & M1) << 6 | (C0 & M1) << 5 | (C1 & M1) << 4 | (C1 & M1) << 3 | (C1 & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (C0 & M1) << 7 | (C0 & M1) << 6 | (C0 & M1) << 5 | (C0 & M1) << 4 | (C1 & M1) << 3 | (C1 & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (TR & M1) << 7 | (C0 & M1) << 6 | (C0 & M1) << 5 | (C0 & M1) << 4 | (TR & M1) << 3 | (TR & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (TR & M1) << 7 | (TR & M1) << 6 | (C2 & M1) << 5 | (C2 & M1) << 4 | (C2 & M1) << 3 | (TR & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
    (TR & M1) << 7 | (TR & M1) << 6 | (C2 & M1) << 5 | (C2 & M1) << 4 | (C2 & M1) << 3 | (C2 & M1) << 2 | (TR & M1) << 1 | (TR & M1) << 0,
#endif
};
#undef M1
#undef M0
#undef C2
#undef C1
#undef C0
#undef TR

u8 NesMain()
{
    int i;
    u8 KeyState = 0;
    u8 x, y;
    Sprite* sp = (Sprite *)SPR_WORK_ADDR;
    Sprite16 sp16[1];

    VSYNC();
    
    //!< 表示オフ
    PPUMASK = 0;
    
    //!< サウンド初期化
    PULSE1_CTRL1 = 0;
    PULSE1_CTRL2 = 0;
    PULSE1_CTRL1 = 0;
    PULSE1_CTRL2 = 0;
    TRIANGLE_CTRL = 0;
    NOISE_CTRL = 0;
    SNDCHANNEL = 0;

    //!< BG ネームテーブルクリア
    BGNameTableClear(PPU_BG_A_NAMETBL_ADDR);
    BGNameTableClear(PPU_BG_B_NAMETBL_ADDR);

    //!< BG スクロール
    PPUSCROLL_BG(0, 0);
    PPUCTRL = 0;

    //!< スプライトクリア (個別に消す方法は無いため、画面外に飛ばす)
    SpriteClear(sp);

    //!< パレット
    SET_PPUADDR(PPU_SPR_PALETTE_ADDR);
    for(i = 0;i < COUNTOF(Palettes);++i) {
        PPUDATA = Palettes[i];
    }
    //!< パターン
    SET_PPUADDR(PPU_PATTERN_ADDR);
    for(i = 0;i < COUNTOF(Patterns);++i) {
        PPUDATA = Patterns[i];
    }
    for(i = 0;i < SPR_MAX;++i) {
        SpriteRemove(&sp[i]);
        sp[i].TileNo = i & 3;
        sp[i].Attribute = SPR_ATTR_PALETTE(0) | SPR_ATTR_PRI_FRONT_OF_BG;
        //sp[i].Attribute = SPR_ATTR_PALETTE(0) | SPR_ATTR_PRI_FRONT_OF_BG | SPR_ATTR_HFLIP_ON;
        //sp[i].Attribute = SPR_ATTR_PALETTE(0) | SPR_ATTR_PRI_FRONT_OF_BG | SPR_ATTR_VFLIP_ON;
        //sp[i].Attribute = SPR_ATTR_PALETTE(0) | SPR_ATTR_PRI_FRONT_OF_BG | SPR_ATTR_HFLIP_ON | SPR_ATTR_VFLIP_ON;
    }

    x = BG_WIDTH >> 1;
    y = BG_HEIGHT >> 1;
    Sprite16Init(&sp16[0], &sp[0]);
    Sprite16SetPosCC(& sp16[0], x, y);

    //!< 表示オン
    PPUMASK = PPUMASK_SPR_DISP_ON | PPUMASK_SPR_EDGE_ON;
   
    while(1){
        KeyState = ReadJoypad1();
        if(BUTTON_UP & KeyState){ --y; }
        if(BUTTON_DOWN & KeyState){ ++y; }
        if(BUTTON_LEFT & KeyState){ --x; }
        if(BUTTON_RIGHT & KeyState){ ++x; }

        for(i = 0;i < COUNTOF(sp16);++i) {
            Sprite16SetPosCC(&sp16[i], x, y);
        }

        VSYNC();

        //!< スプライトの DMA 転送
        OAM_DMA_START(SPR_WORK_ADDR);
    }
    return 0;
}