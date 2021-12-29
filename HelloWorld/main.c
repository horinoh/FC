#include "../define.h"

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

void VSync()
{
    //!< 描画中でなくなるまで待つ
     while(!(PPUSTATUS & PPUSTATUS_IN_VBLANK)) {}
}

//!< OAM DMA 転送元となるワーク、下位 8 ビットが 0x00 である必要がある
//!< ここでは [0x700, 0x7ff] を使用することとする
#define SPR_WORK_ADDR 0x700

const u8 ColorTable[] = { COLOR_BLUE, 0, 0, 0 };
const u8 PatternBG[] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
};

u8 NesMain()
{
    int i;

    VSync();
    
    //!< 表示オフ
    PPUMASK = 0;
    
    //!< サウンド初期化
    PULSE1CTR1 = PULSE1CTR1_ENVVOLVAL(8) | PULSE1CTR1_ENV_ON | PULSE1CTR1_ENVLOOP_ON | PULSE1CTR1_DUTY_50;
    PULSE1CTR2 = PULSE1CTR2_FREQ_CHG_VAL(2) | PULSE1CTR2_FREQ_CHG_DIR_HIGH | PULSE1CTR2_FREQ_CHG_PERIOD(4) | PULSE1CTR2_FREQ_CHG_ON;
    SNDCHANNEL = SNDCHANNEL_SQUARE1_ON;

    //!< BG クリア
    SET_PPUADDR(0x2000);
    for(i = 0;i < 0x800;++i) { PPUDATA = 0; }
    //!< BG スクロール
    PPUSCROLL_BG(0, 0);
    PPUCTRL = PPUCTRL_BG_PAT_TBL_1000 | PPUCTRL_BG_H(0);

    //!< スプライトクリア (消す方法は無いため、画面外に飛ばす)
    for(i = 0;i < 16;++i) {
    }

    //!< パレット
    SET_PPUADDR(0x3f00);
    for(i = 0;i < COUNTOF(ColorTable);++i) {
        PPUDATA = ColorTable[i];
    }

    //!< パターン
    SET_PPUADDR(0x0000);
    for(i = 0;i < COUNTOF(PatternBG);++i) {
        PPUDATA = PatternBG[i];
    }
    
    //!< 表示オン
    PPUMASK = PPUMASK_BG_DISP_ON | PPUMASK_SPR_DISP_ON | PPUMASK_BG_EDGE_ON | PPUMASK_SPR_EDGE_ON;

    while(1){
        VSync();
        //OAM_DMA_START(SPR_WORK_ADDR);
    }
    return 0;
}