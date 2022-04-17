#include "../fc.h"

void apu_init()
{
  const uint8_t APUINIT[] = {
    0x30,0x08,0x00,0x00,
    0x30,0x08,0x00,0x00,
    0x80,0x00,0x00,0x00,
    0x30,0x00,0x00,0x00,
    0x00,0x00,0x00
  };
  memcpy((void *)0x4000, APUINIT, sizeof(APUINIT));
  APU.fcontrol = 0x40;
  APU.status = ENABLE_PULSE0 | ENABLE_PULSE1 | ENABLE_TRIANGLE | ENABLE_NOISE /*| ENABLE_DMC*/;
}

typedef struct PULSE_PARAM
{
  uint8_t channel;  //!< [PULS_CH0, PULS_CH1]

  int16_t period;  //!< [0, 2047]
  int16_t duty;     //!< [DUTY_12, DUTY_25, DUTY_50, DUTY_75]
  int8_t decay;    //!< [0, 15]
  int8_t length;   //!< [0, 31]
  int8_t volume;   //!< [0, 15]
} PULSE_PARAM;
typedef struct PULSE_SWEEP_PARAM
{
  uint8_t channel;  //!< [PULS_CH0, PULS_CH1]
  
  uint8_t period;   //!< [0, 7]
  uint8_t shit;     //!< [0, 7]
  uint8_t up;       //!< [0, 1]
} PULSE_SWEEP_PARAM;

typedef struct TRIANGLE_PARAM
{
  int16_t period;  //!< [0, 2047]     
  int8_t length;   //!< [0, 31]
} TRIANGLE_PARAM;

typedef struct NOISE_PARAM
{
  int16_t period;  //!< [0, 15]
  int8_t decay;    //!< [0, 15]
  int8_t length;   //!< [0, 15] 
  int8_t volume;   //!< [0, 15]
} NOISE_PARAM;

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

  PULSE_PARAM PulseParam[] = {
    { PULSE_CH0, 0x7ff, DUTY_75, 0x0f, 0x0f, 0x0f },
    { PULSE_CH1, 0x7ff, DUTY_75, 0x0f, 0x0f, 0x0f },
  };
  PULSE_SWEEP_PARAM PulseSweepParam[] = {
    { PULSE_CH0, 0x07, 0x07, 1 },
    { PULSE_CH1, 0x07, 0x07, 1 },
  };
  TRIANGLE_PARAM TriangleParam = { 0x7ff, 0x0f };
  NOISE_PARAM NoiseParam = { 0x0f, 0x0f, 0x0f, 0x0f };
  
  const char* SoundStr[] = { "PULSE0  ", "PULSE1  ", "TRIANGLE", "NOISE   ", };
#define SND_PULSE0 0
#define SND_PULSE1 1
#define SND_TRIANGLE 2
#define SND_NOISE 3
#define SND_COUNT 4
  uint8_t SoundKind = SND_PULSE0;

  const char* SoundPluseParamStr[] = { " Period", " Duty  ", " Decay ", " Length", " Volume",/*"Period", "Shift", "Up"*/ };
  const char* SoundTriangleParamStr[] = { " Period", " Length", };
  const char* SoundNoiseParamStr[] = { " Period", " Decay ", " Length", " Volume", };
  int8_t SoundParamIndex = 0;

  char Str[32] = "0";
  uint8_t i;

  //!< パレット
  {
    pal_all((const char*)Palette);
  }

  pal_bright(Fade >> 4);

  //!< VRAM バッファ
  {
    VramBufferSize = 64;
    VRAMBUF_CLEAR;
    set_vram_update(VramBuffer);
  }

  //!< PPU 制御
  {
    ppu_mask(MASK_BG | MASK_SPR);

    ppu_on_all();
  }

  //!< サウンド初期化
  apu_init();

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

      int8_t Val = 0;

      if(TRG(PAD_START)) {
        ++SoundKind;
        SoundKind %= SND_COUNT;
        SoundParamIndex = 0;

        ppu_off();
        ppu_on_all();
      }

      //!< パラメータ選択
      if(TRG(PAD_UP)) { --SoundParamIndex; }
      if(TRG(PAD_DOWN)) { ++SoundParamIndex; }
      SoundParamIndex = MAX(SoundParamIndex, 0);
      switch (SoundKind) {
      case SND_PULSE0:
      case SND_PULSE1:
        SoundParamIndex = MIN(SoundParamIndex, COUNTOF(SoundPluseParamStr) - 1);
        break;    
      case SND_TRIANGLE:
        SoundParamIndex = MIN(SoundParamIndex, COUNTOF(SoundTriangleParamStr) - 1);
        break;    
      case SND_NOISE:
        SoundParamIndex = MIN(SoundParamIndex, COUNTOF(SoundNoiseParamStr) - 1);
        break;    
      }

      //!< パラメータ値変更
      if(TRG(PAD_LEFT)) { Val = -1; }
      if(TRG(PAD_RIGHT)) { Val = 1; }
      if(Val) {
        switch (SoundKind) {
        case SND_PULSE0:
        case SND_PULSE1:
          switch (SoundParamIndex) {
          case 0:
            PulseParam[SoundKind].period = MIN(MAX(PulseParam[SoundKind].period + Val * ((Cur & PAD_SELECT) ? 100 : 1), 0), 2047);
            break;
          case 1:          
            PulseParam[SoundKind].duty = MIN(MAX(PulseParam[SoundKind].duty + Val * 0x40, DUTY_12), DUTY_75);
            break;
          case 2:          
            PulseParam[SoundKind].decay = MIN(MAX(PulseParam[SoundKind].decay + Val, 0), 15);
            break;
          case 3:          
            PulseParam[SoundKind].length = MIN(MAX(PulseParam[SoundKind].length + Val, 0), 31);
            break;
          case 4:          
            PulseParam[SoundKind].volume = MIN(MAX(PulseParam[SoundKind].volume + Val, 0), 15);
            break;
          }
          break;    
        case SND_TRIANGLE:
          switch (SoundParamIndex) {
          case 0:
            TriangleParam.period = MIN(MAX(TriangleParam.period + Val * ((Cur & PAD_SELECT) ? 100 : 1), 0), 2047);
            break;
          case 1:          
            TriangleParam.length = MIN(MAX(TriangleParam.length + Val, 0), 31);
            break;
          }
          break;    
        case SND_NOISE:
         switch (SoundParamIndex) {
          case 0:
            NoiseParam.period = MIN(MAX(NoiseParam.period + Val, 0), 15);
            break;
          case 1:          
            NoiseParam.decay = MIN(MAX(NoiseParam.decay + Val, 0), 15);
            break;
          case 2:          
            NoiseParam.length = MIN(MAX(NoiseParam.length + Val, 0), 15);
            break;
          case 3:          
            NoiseParam.volume = MIN(MAX(NoiseParam.volume + Val, 0), 15);
            break;
          }
          break;    
        }
      }

      //!< サウンド開始 (A : DECAY, A + SELECT : SUSTAIN)
      if(TRG(PAD_A)) {
        switch (SoundKind) {
        case SND_PULSE0:
          if(Cur & PAD_SELECT) {
            //!< 鳴り続ける
            APU_PULSE_SUSTAIN(PulseParam[0].channel, PulseParam[0].period, PulseParam[0].duty, PulseParam[0].volume);
          } else {
            //!< 減衰していく
            APU_PULSE_DECAY(PulseParam[0].channel, PulseParam[0].period, PulseParam[0].duty, PulseParam[0].decay, PulseParam[0].length);
          }
           //APU_PULSE_SET_DECAY(PulseParam[0].channel, PulseParam[0].duty, PulseParam[0].decay);
           //APU_PULSE_SET_VOLUME(PulseParam[0].channel, PulseParam[0].duty, PulseParam[0].volume);
          break;    
        case SND_PULSE1:
          if(Cur & PAD_SELECT) {
            APU_PULSE_SUSTAIN(PulseParam[1].channel, PulseParam[1].period, PulseParam[1].duty, PulseParam[1].volume);
          } else {
            APU_PULSE_DECAY(PulseParam[1].channel, PulseParam[1].period, PulseParam[1].duty, PulseParam[1].decay, PulseParam[1].length);
          }
           //APU_PULSE_SET_DECAY(PulseParam[1].channel, PulseParam[1].duty, PulseParam[1].decay);
           //APU_PULSE_SET_VOLUME(PulseParam[1].channel, PulseParam[1].duty, PulseParam[1].volume);
          break;    
        case SND_TRIANGLE:
          if(Cur & PAD_SELECT) {
            APU_TRIANGLE_SUSTAIN(TriangleParam.period);
          } else {
            APU_TRIANGLE_LENGTH(TriangleParam.period, TriangleParam.length);
          }
          break;    
        case SND_NOISE:
          if(Cur & PAD_SELECT) {
            APU_NOISE_SUSTAIN(NoiseParam.period, NoiseParam.volume);
          } else {
            APU_NOISE_DECAY(NoiseParam.period, NoiseParam.decay, NoiseParam.length);
          }
          break;    
        }
      }
      //!< スイープ (矩形波)
      if(TRG(PAD_B)) {
        switch (SoundKind) {
        case SND_PULSE0:
          APU_PULSE_SWEEP(PulseSweepParam[0].channel, PulseSweepParam[0].period, PulseSweepParam[0].shit, PulseSweepParam[0].up);
          //APU_PULSE_SWEEP_DISABLE(PulseParam[0].channel);
          break;    
        case SND_PULSE1:
          APU_PULSE_SWEEP(PulseSweepParam[1].channel, PulseSweepParam[1].period, PulseSweepParam[1].shit, PulseSweepParam[1].up);
          //APU_PULSE_SWEEP_DISABLE(PulseParam[1].channel);
          break;    
        }
      }
#undef REL
#undef TRG
    }

    //!< サウンド種の表示
    vrambuf_put_h(NTADR_A(2, 2), SoundStr[SoundKind]);

    //!< パラメータの表示
    switch (SoundKind) {
    case SND_PULSE0:
    case SND_PULSE1:
      for(i=0;i<COUNTOF(SoundPluseParamStr);++i) {
        vrambuf_put_h(NTADR_A(1, 4 + i), SoundPluseParamStr[i]);
        switch (i) {
        case 0:
          sprintf(Str, "%04d", PulseParam[SoundKind].period);
          break;
        case 1:
          switch (PulseParam[SoundKind].duty) {
          case DUTY_12: sprintf(Str, "12.5"); break;          
          case DUTY_25: sprintf(Str, "25  "); break;   
          case DUTY_50: sprintf(Str, "50  "); break;   
          case DUTY_75: sprintf(Str, "75  "); break;   
          }
          break;
        case 2:          
          sprintf(Str, "%04d", PulseParam[SoundKind].decay);
          break;
        case 3:          
          sprintf(Str, "%04d", APULengthTable[PulseParam[SoundKind].length]);
          break;
        case 4:          
          sprintf(Str, "%04d", PulseParam[SoundKind].volume);
          break;
        }
        vrambuf_put_h(NTADR_A(10, 4 + i), Str);
      }
      break;    
    case SND_TRIANGLE:
      vrambuf_put_h(NTADR_A(1, 4 + COUNTOF(SoundTriangleParamStr) + 0), "                 ");
      vrambuf_put_h(NTADR_A(1, 4 + COUNTOF(SoundTriangleParamStr) + 1), "                 ");
      vrambuf_put_h(NTADR_A(1, 4 + COUNTOF(SoundTriangleParamStr) + 2), "                 ");
      for(i=0;i<COUNTOF(SoundTriangleParamStr);++i) {
        vrambuf_put_h(NTADR_A(1, 4 + i), SoundTriangleParamStr[i]);
        switch (i) {
        case 0:
          sprintf(Str, "%04d", TriangleParam.period);
          break;
        case 1:          
          sprintf(Str, "%04d", APULengthTable[TriangleParam.length]);
          break;
        }
        vrambuf_put_h(NTADR_A(10, 4 + i), Str);
      }
      break;    
    case SND_NOISE:
      for(i=0;i<COUNTOF(SoundNoiseParamStr);++i) {
        vrambuf_put_h(NTADR_A(1, 4 + i), SoundNoiseParamStr[i]);
        switch (i) {
        case 0:
          sprintf(Str, "%04d", NoiseParam.period);
          break;
        case 1:          
          sprintf(Str, "%04d", NoiseParam.decay);
          break;
        case 2:
          sprintf(Str, "%04d", APULengthTable[NoiseParam.length]);
          break;
        case 3:          
          sprintf(Str, "%04d", NoiseParam.volume);
          break;
        }
        vrambuf_put_h(NTADR_A(10, 4 + i), Str);
      }
      break;    
    }
    vrambuf_put_h(NTADR_A(1, 4 + SoundParamIndex), "*");
    
    //!< APU status
    sprintf(Str, "APU.status = %04d", APU.status);
    vrambuf_put_h(NTADR_A(2, 15), Str);

    //!< ヘルプ
    vrambuf_put_h(NTADR_A(2, 20), "START      : CHANGE SOUND");
    vrambuf_put_h(NTADR_A(2, 21), "A          : PLAY (DECAY)");
    vrambuf_put_h(NTADR_A(2, 22), "A + SELECT : PLAY (SUSTAIN)");
    vrambuf_put_h(NTADR_A(2, 23), "U, D, L, R : CHANGE PARAM");
    if(SoundKind == SND_PULSE0 || SoundKind == SND_PULSE1) {
      vrambuf_put_h(NTADR_A(2, 24), "B          : SWEEP");
    } else {
      vrambuf_put_h(NTADR_A(2, 24), "                      ");
    }

    ppu_wait_nmi();
  }

  ppu_off();
}

	
	