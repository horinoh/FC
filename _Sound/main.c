#include "../fclib.h"

u8 NesMain()
{
    int hz = 1;
    u8 PrevKeyState = 0;
    u8 KeyState = 0;
   
    VSYNC();
    
    //!< 表示オフ
    PPUMASK = 0;
    
    //!< サウンド初期化
    PULSE1_CTRL1 = PULSE_CTRL1_ENVVOLVAL(1/*[0, 15]*/) | PULSE_CTRL1_ENV_ON | PULSE_CTRL1_ENVLOOP_ON | PULSE_CTRL1_DUTY_50;
    PULSE1_CTRL2 = PULSE_CTRL2_FREQ_CHG_VAL(2/*[0, 7]*/) | PULSE_CTRL2_FREQ_CHG_DIR_HIGH | PULSE_CTRL2_FREQ_CHG_PERIOD(4/*[0, 7]*/) | PULSE_CTRL2_FREQ_CHG_ON;
    PULSE2_CTRL1 = PULSE_CTRL1_ENVVOLVAL(1) | PULSE_CTRL1_ENV_ON | PULSE_CTRL1_ENVLOOP_ON | PULSE_CTRL1_DUTY_50;
    PULSE2_CTRL2 = PULSE_CTRL2_FREQ_CHG_VAL(2) | PULSE_CTRL2_FREQ_CHG_DIR_HIGH | PULSE_CTRL2_FREQ_CHG_PERIOD(4) | PULSE_CTRL2_FREQ_CHG_ON;
    TRIANGLE_CTRL = 0;
    NOISE_CTRL = 0;
    SNDCHANNEL = SNDCHANNEL_PULSE1_ON | SNDCHANNEL_PULSE2_ON | SNDCHANNEL_TRIANGLE_ON | SNDCHANNEL_NOISE_ON | SNDCHANNEL_DMC_ON;

    while(1){
        PrevKeyState = KeyState;
        KeyState = ReadJoypad1();

        if(BUTTON_UP & KeyState) { hz += 10; }
        if(BUTTON_DOWN & KeyState) { hz -= 10; }
        if(BUTTON_LEFT & KeyState) { ++hz; }
        if(BUTTON_RIGHT & KeyState) { --hz; }
        hz = MIN(hz, 2047); 
        hz = MAX(hz, 1);
        if((BUTTON_A & KeyState) && !(BUTTON_A & PrevKeyState)){ Pulse1(hz);  }
        if((BUTTON_B & KeyState) && !(BUTTON_B & PrevKeyState)){ Pulse2(2048 - hz); }
        if((BUTTON_SELECT & KeyState) && !(BUTTON_SELECT & PrevKeyState)){ }
        if((BUTTON_START & KeyState) && !(BUTTON_START & PrevKeyState)){ }

        VSYNC();
    }
    return 0;
}