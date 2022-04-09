#include "../fclib.h"

#define SPR_WORK_ADDR 0x700

//!<
//!< 【パレット】
//!< 
#define BGCOLOR COLOR_SKYBLUE
#define NOTUSED 0
const u8 Palettes[] = { 
    BGCOLOR, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, //!< パレット 0
    NOTUSED, 0, 0, 0, //!< パレット 1
    NOTUSED, 0, 0, 0, //!< パレット 2
    NOTUSED, 0, 0, 0, //!< パレット 3
};
#undef NOTUSED
#undef BGCOLOR

//!<
//!< 【パターン】 0x8 x 256 個まで
//!< 
#define BK 0
#define C0 1
#define C1 2
#define C2 3
// 01234567890123456789012345
// ---DE--H---L--O--R----W---   
const u8 Patterns[] = {
    //!< パターン 0
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    //!< パターン 1
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    //!< パターン 2
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    //!< パターン 3 (D)
};
#undef C2
#undef C1
#undef C0
#undef BK

u8 NesMain()
{
    int i;

    VSYNC();

    //!< 表示オフ
    PPUMASK = 0;
    
    //!< サウンド初期化
    PULSE1_CTRL1 = 0;
    PULSE1_CTRL2 = 0;
    PULSE1_CTRL1 = 0;
    PULSE1_CTRL2 = 0;
    SNDCHANNEL = 0;
    
    //!< ネームテーブルクリア
    BGNameTableClear(PPU_BG_A_NAMETBL_ADDR);
    BGNameTableClear(PPU_BG_B_NAMETBL_ADDR);

    //!< BG スクロール
    PPUSCROLL_BG(0, 0);
    PPUCTRL = PPUCTRL_SPR_PAT_TBL_1000 | PPUCTRL_BG_H(0);

    //!< パレット
    SET_PPUADDR(PPU_BG_PALETTE_ADDR);
    for(i = 0;i < COUNTOF(Palettes);++i) {
        PPUDATA = Palettes[i];
    }

    //!< パターン
    SET_PPUADDR(PPU_PATTERN_ADDR);
    for(i = 0;i < COUNTOF(Patterns);++i) {
        PPUDATA = Patterns[i];
    }

    //!< 表示オン
    PPUMASK = PPUMASK_BG_DISP_ON | PPUMASK_BG_EDGE_ON;

    while(1){

        VSYNC();

        //!< Vブランク中

        //!< PPU 制御後、BG スクロール座標が変わってしまうので再設定する必要がある
        PPUSCROLL_BG(0, 0);
        PPUCTRL = PPUCTRL_SPR_PAT_TBL_1000 | PPUCTRL_BG_H(0);
    }
    return 0;
}