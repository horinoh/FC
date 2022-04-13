#include "../fclib.h"

//!< OAM DMA 転送元となるワーク、下位 8 ビットが 0x00 である必要がある
//!< ここでは [0x700, 0x7ff] を使用することとする
#define SPR_WORK_ADDR 0x700

extern const u8 palette[16][16];

//!<
//!< 【パターン】 0x8 x 256 個まで
//!< 
#define TR 0
#define C0 1
#define C1 2
#define C2 3

#define M0 1
#define M1 2

#define P(c, m, b) PATTERN(c, m, b)
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
    //!< パターン 0, プレーン0
    P(TR, M0, 7) | P(TR, M0, 6) | P(TR, M0, 5) | P(TR, M0, 4) | P(TR, M0, 3) | P(C0, M0, 2) | P(C0, M0, 1) | P(C0, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(TR, M0, 5) | P(TR, M0, 4) | P(C0, M0, 3) | P(C0, M0, 2) | P(C0, M0, 1) | P(C0, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(TR, M0, 5) | P(TR, M0, 4) | P(C2, M0, 3) | P(C2, M0, 2) | P(C2, M0, 1) | P(C1, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(TR, M0, 5) | P(C2, M0, 4) | P(C1, M0, 3) | P(C2, M0, 2) | P(C1, M0, 1) | P(C1, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(TR, M0, 5) | P(C2, M0, 4) | P(C1, M0, 3) | P(C2, M0, 2) | P(C2, M0, 1) | P(C1, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(TR, M0, 5) | P(C2, M0, 4) | P(C2, M0, 3) | P(C1, M0, 2) | P(C1, M0, 1) | P(C1, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(TR, M0, 5) | P(TR, M0, 4) | P(TR, M0, 3) | P(C1, M0, 2) | P(C1, M0, 1) | P(C1, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(TR, M0, 5) | P(TR, M0, 4) | P(C2, M0, 3) | P(C2, M0, 2) | P(C0, M0, 1) | P(C2, M0, 0), 
    //!< パターン 0, プレーン 1
    P(TR, M1, 7) | P(TR, M1, 6) | P(TR, M1, 5) | P(TR, M1, 4) | P(TR, M1, 3) | P(C0, M1, 2) | P(C0, M1, 1) | P(C0, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(TR, M1, 5) | P(TR, M1, 4) | P(C0, M1, 3) | P(C0, M1, 2) | P(C0, M1, 1) | P(C0, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(TR, M1, 5) | P(TR, M1, 4) | P(C2, M1, 3) | P(C2, M1, 2) | P(C2, M1, 1) | P(C1, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(TR, M1, 5) | P(C2, M1, 4) | P(C1, M1, 3) | P(C2, M1, 2) | P(C1, M1, 1) | P(C1, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(TR, M1, 5) | P(C2, M1, 4) | P(C1, M1, 3) | P(C2, M1, 2) | P(C2, M1, 1) | P(C1, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(TR, M1, 5) | P(C2, M1, 4) | P(C2, M1, 3) | P(C1, M1, 2) | P(C1, M1, 1) | P(C1, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(TR, M1, 5) | P(TR, M1, 4) | P(TR, M1, 3) | P(C1, M1, 2) | P(C1, M1, 1) | P(C1, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(TR, M1, 5) | P(TR, M1, 4) | P(C2, M1, 3) | P(C2, M1, 2) | P(C0, M1, 1) | P(C2, M1, 0), 

    //!< パターン 1
    P(C0, M0, 7) | P(C0, M0, 6) | P(TR, M0, 5) | P(TR, M0, 4) | P(TR, M0, 3) | P(TR, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(C0, M0, 7) | P(C0, M0, 6) | P(C0, M0, 5) | P(C0, M0, 4) | P(C0, M0, 3) | P(TR, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(C1, M0, 7) | P(C2, M0, 6) | P(C1, M0, 5) | P(TR, M0, 4) | P(TR, M0, 3) | P(TR, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(C1, M0, 7) | P(C2, M0, 6) | P(C1, M0, 5) | P(C1, M0, 4) | P(C1, M0, 3) | P(TR, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(C1, M0, 7) | P(C1, M0, 6) | P(C2, M0, 5) | P(C1, M0, 4) | P(C1, M0, 3) | P(C1, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(C1, M0, 7) | P(C2, M0, 6) | P(C2, M0, 5) | P(C2, M0, 4) | P(C2, M0, 3) | P(TR, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(C1, M0, 7) | P(C1, M0, 6) | P(C1, M0, 5) | P(C1, M0, 4) | P(TR, M0, 3) | P(TR, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(C2, M0, 7) | P(C2, M0, 6) | P(TR, M0, 5) | P(TR, M0, 4) | P(TR, M0, 3) | P(TR, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),

    P(C0, M1, 7) | P(C0, M1, 6) | P(TR, M1, 5) | P(TR, M1, 4) | P(TR, M1, 3) | P(TR, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(C0, M1, 7) | P(C0, M1, 6) | P(C0, M1, 5) | P(C0, M1, 4) | P(C0, M1, 3) | P(TR, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(C1, M1, 7) | P(C2, M1, 6) | P(C1, M1, 5) | P(TR, M1, 4) | P(TR, M1, 3) | P(TR, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(C1, M1, 7) | P(C2, M1, 6) | P(C1, M1, 5) | P(C1, M1, 4) | P(C1, M1, 3) | P(TR, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(C1, M1, 7) | P(C1, M1, 6) | P(C2, M1, 5) | P(C1, M1, 4) | P(C1, M1, 3) | P(C1, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(C1, M1, 7) | P(C2, M1, 6) | P(C2, M1, 5) | P(C2, M1, 4) | P(C2, M1, 3) | P(TR, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(C1, M1, 7) | P(C1, M1, 6) | P(C1, M1, 5) | P(C1, M1, 4) | P(TR, M1, 3) | P(TR, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(C2, M1, 7) | P(C2, M1, 6) | P(TR, M1, 5) | P(TR, M1, 4) | P(TR, M1, 3) | P(TR, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),

    //!< パターン 2
    P(TR, M0, 7) | P(TR, M0, 6) | P(TR, M0, 5) | P(C2, M0, 4) | P(C2, M0, 3) | P(C2, M0, 2) | P(C0, M0, 1) | P(C2, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(C2, M0, 5) | P(C2, M0, 4) | P(C2, M0, 3) | P(C2, M0, 2) | P(C0, M0, 1) | P(C0, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(C1, M0, 5) | P(C1, M0, 4) | P(C2, M0, 3) | P(C0, M0, 2) | P(C1, M0, 1) | P(C0, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(C1, M0, 5) | P(C1, M0, 4) | P(C1, M0, 3) | P(C0, M0, 2) | P(C0, M0, 1) | P(C0, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(C1, M0, 5) | P(C1, M0, 4) | P(C0, M0, 3) | P(C0, M0, 2) | P(C0, M0, 1) | P(C0, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(TR, M0, 5) | P(TR, M0, 4) | P(C0, M0, 3) | P(C0, M0, 2) | P(C0, M0, 1) | P(TR, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(TR, M0, 5) | P(C2, M0, 4) | P(C2, M0, 3) | P(C2, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0), 
    P(TR, M0, 7) | P(TR, M0, 6) | P(C2, M0, 5) | P(C2, M0, 4) | P(C2, M0, 3) | P(C2, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0), 

    P(TR, M1, 7) | P(TR, M1, 6) | P(TR, M1, 5) | P(C2, M1, 4) | P(C2, M1, 3) | P(C2, M1, 2) | P(C0, M1, 1) | P(C2, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(C2, M1, 5) | P(C2, M1, 4) | P(C2, M1, 3) | P(C2, M1, 2) | P(C0, M1, 1) | P(C0, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(C1, M1, 5) | P(C1, M1, 4) | P(C2, M1, 3) | P(C0, M1, 2) | P(C1, M1, 1) | P(C0, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(C1, M1, 5) | P(C1, M1, 4) | P(C1, M1, 3) | P(C0, M1, 2) | P(C0, M1, 1) | P(C0, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(C1, M1, 5) | P(C1, M1, 4) | P(C0, M1, 3) | P(C0, M1, 2) | P(C0, M1, 1) | P(C0, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(TR, M1, 5) | P(TR, M1, 4) | P(C0, M1, 3) | P(C0, M1, 2) | P(C0, M1, 1) | P(TR, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(TR, M1, 5) | P(C2, M1, 4) | P(C2, M1, 3) | P(C2, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0), 
    P(TR, M1, 7) | P(TR, M1, 6) | P(C2, M1, 5) | P(C2, M1, 4) | P(C2, M1, 3) | P(C2, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0), 

    //!< パターン 3
    P(C2, M0, 7) | P(C0, M0, 6) | P(C2, M0, 5) | P(C2, M0, 4) | P(C2, M0, 3) | P(TR, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(C0, M0, 7) | P(C0, M0, 6) | P(C2, M0, 5) | P(C2, M0, 4) | P(C2, M0, 3) | P(C2, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(C0, M0, 7) | P(C1, M0, 6) | P(C0, M0, 5) | P(C2, M0, 4) | P(C1, M0, 3) | P(C1, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(C0, M0, 7) | P(C0, M0, 6) | P(C0, M0, 5) | P(C1, M0, 4) | P(C1, M0, 3) | P(C1, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(C0, M0, 7) | P(C0, M0, 6) | P(C0, M0, 5) | P(C0, M0, 4) | P(C1, M0, 3) | P(C1, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(TR, M0, 7) | P(C0, M0, 6) | P(C0, M0, 5) | P(C0, M0, 4) | P(TR, M0, 3) | P(TR, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(TR, M0, 7) | P(TR, M0, 6) | P(C2, M0, 5) | P(C2, M0, 4) | P(C2, M0, 3) | P(TR, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),
    P(TR, M0, 7) | P(TR, M0, 6) | P(C2, M0, 5) | P(C2, M0, 4) | P(C2, M0, 3) | P(C2, M0, 2) | P(TR, M0, 1) | P(TR, M0, 0),

    P(C2, M1, 7) | P(C0, M1, 6) | P(C2, M1, 5) | P(C2, M1, 4) | P(C2, M1, 3) | P(TR, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(C0, M1, 7) | P(C0, M1, 6) | P(C2, M1, 5) | P(C2, M1, 4) | P(C2, M1, 3) | P(C2, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(C0, M1, 7) | P(C1, M1, 6) | P(C0, M1, 5) | P(C2, M1, 4) | P(C1, M1, 3) | P(C1, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(C0, M1, 7) | P(C0, M1, 6) | P(C0, M1, 5) | P(C1, M1, 4) | P(C1, M1, 3) | P(C1, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(C0, M1, 7) | P(C0, M1, 6) | P(C0, M1, 5) | P(C0, M1, 4) | P(C1, M1, 3) | P(C1, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(TR, M1, 7) | P(C0, M1, 6) | P(C0, M1, 5) | P(C0, M1, 4) | P(TR, M1, 3) | P(TR, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(TR, M1, 7) | P(TR, M1, 6) | P(C2, M1, 5) | P(C2, M1, 4) | P(C2, M1, 3) | P(TR, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
    P(TR, M1, 7) | P(TR, M1, 6) | P(C2, M1, 5) | P(C2, M1, 4) | P(C2, M1, 3) | P(C2, M1, 2) | P(TR, M1, 1) | P(TR, M1, 0),
};
#undef P
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
    Sprite16 sp16[SPR_MAX >> 2];

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
    #if 1
    SET_PPUADDR(PPU_SPR_PALETTE_ADDR);
    for(i = 0;i < COUNTOF(palette);++i) {
        PPUDATA = palette[i];
    }
    #endif
    //!< パターン
    #if 0
    SET_PPUADDR(PPU_PATTERN_ADDR);
    for(i = 0;i < COUNTOF(Patterns);++i) {
        PPUDATA = Patterns[i];
    }
    #endif
    for(i = 0;i < SPR_MAX;++i) {
        SpriteRemove(&sp[i]);
        sp[i].TileNo = i & 3;
        //sp[i].Attribute = SPR_ATTR_PALETTE(0) | SPR_ATTR_PRI_FRONT_OF_BG;
        //sp[i].Attribute = SPR_ATTR_PALETTE(0) | SPR_ATTR_PRI_FRONT_OF_BG | SPR_ATTR_HFLIP_ON;
        //sp[i].Attribute = SPR_ATTR_PALETTE(0) | SPR_ATTR_PRI_FRONT_OF_BG | SPR_ATTR_VFLIP_ON;
        //sp[i].Attribute = SPR_ATTR_PALETTE(0) | SPR_ATTR_PRI_FRONT_OF_BG | SPR_ATTR_HFLIP_ON | SPR_ATTR_VFLIP_ON;
        sp[i].Attribute = SPR_ATTR_PALETTE(0) | SPR_ATTR_PRI_FRONT_OF_BG | ((i & 4) ? SPR_ATTR_HFLIP_ON : 0);
    }

    x = y = 0;
    for(i = 0;i < COUNTOF(sp16);++i){
        Sprite16Init(&sp16[i], &sp[i << 2]);
        Sprite16SetPosCC(&sp16[i], i << 4, (i << 3));
    }

    //!< 表示オン
    PPUMASK = PPUMASK_SPR_DISP_ON | PPUMASK_SPR_EDGE_ON;
   
    while(1){
        KeyState = ReadJoypad1();
        if(BUTTON_UP & KeyState){ --y; }
        if(BUTTON_DOWN & KeyState){ ++y; }
        if(BUTTON_LEFT & KeyState){ --x; }
        if(BUTTON_RIGHT & KeyState){ ++x; }

        for(i = 0;i < COUNTOF(sp16);++i) {
            Sprite16SetPosCC(&sp16[i], x + (i << 4), y + (i << 3));
        }

        VSYNC();

        //!< スプライトの DMA 転送
        OAM_DMA_START(SPR_WORK_ADDR);
    }
    return 0;
}