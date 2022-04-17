#ifndef _APU_H_
#define _APU_H_

#define ENABLE_PULSE0	0x1
#define ENABLE_PULSE1	0x2
#define ENABLE_TRIANGLE	0x4
#define ENABLE_NOISE	0x8
#define ENABLE_DMC	0x10

#define DUTY_12 0x00
#define DUTY_25 0x40
#define DUTY_50 0x80
#define DUTY_75 0xc0

#define PULSE_ENVLOOP	0x20
#define PULSE_CONSTVOL	0x10
#define PULSE_VOLENVMASK 0xf
#define PULSE_CH0	0
#define PULSE_CH1	1
#define TRIANGLE_LC_HALT	0x80
#define TRIANGLE_LC_MASK	0x7f
#define NOISE_ENVLOOP	0x20
#define NOISE_CONSTVOL	0x10
#define NOISE_VOLENVMASK 0xf
#define NOISE_PERIOD_BUZZ	0x80

/*
ch      [PULS_CH0, PULS_CH1]                  矩形波は2チャンネルある
period  [0, 2047]                             
duty    [DUTY_12, DUTY_25, DUTY_50, DUTY_75]  
decay   [0, 15]                               減衰
len     [0, 31]
vol     [0, 15]
*/
#define APU_PULSE_DECAY(ch, period, duty, decay, len)\
  APU.pulse[ch].period_low = (period) & 0xff;\
  APU.pulse[ch].len_period_high = (((period) >> 8) & 7) | ((len) << 3);\
  APU.pulse[ch].control = (duty) | (decay)
#define APU_PULSE_SUSTAIN(ch, period, duty, vol)\
  APU.pulse[ch].period_low = (period) & 0xff;\
  APU.pulse[ch].len_period_high = (((period) >> 8)& 7);\
  APU.pulse[ch].control = (duty) | (vol) | (PULSE_CONSTVOL | PULSE_ENVLOOP)
#define APU_PULSE_SET_DECAY(ch, duty, decay) APU.pulse[ch].control = (duty) | (decay)
#define APU_PULSE_SET_VOLUME(ch, duty, vol) APU.pulse[ch].control = (duty) | (vol) | (PULSE_CONSTVOL | PULSE_ENVLOOP)

#define APU_TRIANGLE_LENGTH(period, len)\
  APU.triangle.counter = 0x7f;\
  APU.triangle.period_low = (period) & 0xff;\
  APU.triangle.len_period_high = (((period) >> 8) & 7) | ((len) << 3)
#define APU_TRIANGLE_SUSTAIN(period)\
  APU.triangle.counter = 0xff;\
  APU.triangle.period_low = (period) & 0xff;\
  APU.triangle.len_period_high = (((period) >> 8) & 7)

/*
ch      [PULSE_CH0, PULSE_CH1]
perioid [0, 7]
shift   [0, 7]
up      [0, 1]
*/
#define APU_PULSE_SWEEP(ch, period, shift, up) APU.pulse[ch].ramp = 0x80 | (period << 4) | (up ? 8 : 0) | shift
#define APU_PULSE_SWEEP_DISABLE(ch) APU.pulse[ch].ramp = 0

/*
period  [0, 15]
vol     [0, 15]
decay   [0, 15]
len     [0, 15]
引数に _ を付けないと上手くいかないので注意
*/
#define APU_NOISE_SUSTAIN(_period, _vol)\
    APU.noise.control = (_vol) | (NOISE_ENVLOOP | NOISE_CONSTVOL);\
    APU.noise.period = (_period)
#define APU_NOISE_DECAY(_period, _decay, _len)\
    APU.noise.control = (_decay);\
    APU.noise.period = (_period);\
    APU.noise.len = (_len)

const uint8_t APULengthTable[] = {
  10, 254, 20, 2, 40, 4, 80, 6, 160, 8, 60, 10, 14, 12, 26, 14, 12, 16, 24, 18, 48, 20, 96, 22, 192, 24, 72, 26, 16, 28, 32, 30,  
};
const uint8_t APULengthTableEven[] = {
  //!< Faster Music
  //!< 1/16, 1/8, 1/4, 1/2, 1, 3/8, 1/12, 1/6
  10, 20, 40, 80, 160, 60, 14, 26,
  //!< Normal Music
  12, 24, 48, 96, 192, 72, 16, 32, 
};
const uint8_t APULengthTableOdd[] = {
  //!< Lnear values
  254, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
};
#endif //!< _APU_H_