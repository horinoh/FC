#include "../fclib.h"

#define SPR_WORK_ADDR 0x700

const u8 Palette[] = { 
    COLOR_SKYBLUE, //!< 背景色
    COLOR_RED, COLOR_GREEN, COLOR_BLUE, //!< BG パレット 0
    0, //!< 未使用
    0, 0, 0, //!< BG パレット 1
    0, //!< 未使用
    0, 0, 0, //!< BG パレット 2
    0, //!< 未使用
    0, 0, 0, //!< BG パレット 3
};

//!< BG 用 8x8 x 256 種
const u8 PatternBG[] = {
    #if 1
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    #else
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,

    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01,
    #endif
};

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

    //!< BG クリア
    SET_PPUADDR(PPU_BG_A_NAMETBL_ADDR);
    for(i = 0;i < (TILE_WIDTH * (TILE_HEIGHT + 1)) << 1;++i) {
         PPUDATA = 0;
    }
    //!< BG スクロール
    PPUSCROLL_BG(0, 0);
    PPUCTRL = PPUCTRL_SPR_PAT_TBL_1000 | PPUCTRL_BG_H(0);

    //!< BG, SPR パレット
    SET_PPUADDR(PPU_BG_PALETTE_ADDR);
    for(i = 0;i < COUNTOF(Palette);++i) {
        PPUDATA = Palette[i];
    }
    //!< BG パターン
    SET_PPUADDR(PPU_PATTERN_ADDR);
    for(i = 0;i < COUNTOF(PatternBG);++i) {
        PPUDATA = PatternBG[i];
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