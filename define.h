#define u8 char
#define u16 unsigned short

#define COUNTOF(x) sizeof(x) / sizeof(x[0])

#define BIT_ON(x) (1 << x)

//!< PPU(Picture Processing Unit)
//!< CPUメモリ空間
#define PPUCTRL *(u8 *)0x2000
    //!< Bit7 Vブランク割込み許可、不許可
    #define PPUCTRL_VBLANK_INT_MASK BIT_ON(7)
    #define PPUCTRL_VBLANK_INT_ON BIT_ON(7)
    //!< Bit6
    #define PPUCTRL_EXT_MASK BIT_ON(6)    
    #define PPUCTRL_EXT_INPUT 0
    #define PPUCTRL_EXT_OUTPUT BIT_ON(6)
    //!< Bit5 スプライトサイズ (8x8 or 8x16)
    #define PPUCTRL_SPR_SIZE_MASK BIT_ON(5)
    #define PPUCTRL_SPR_SIZE_8X8 0
    #define PPUCTRL_SPR_SIZE_8X16 BIT_ON(5)
    //!< Bit4 BGパターンテーブルのアドレス (0x0000 or 0x1000)
    #define PPUCTRL_BG_PAT_TBL_MASK BIT_ON(4)
    #define PPUCTRL_BG_PAT_TBL_0000 0
    #define PPUCTRL_BG_PAT_TBL_1000 BIT_ON(4)
    //!< Bit3 スプライトパターンテーブルのアドレス (0x0000 or 0x1000)
    #define PPUCTRL_SPR_PAT_TBL_MASK BIT_ON(3)
    #define PPUCTRL_SPR_PAT_TBL_0000 0
    #define PPUCTRL_SPR_PAT_TBL_1000 BIT_ON(3)
    //!< Bit2 PPUDATA 読み書き時の PPUADDR アドレス加算値 (+1 or +32)
    #define PPUCTRL_ADDR_ADD_MASK BIT_ON(2)
    #define PPUCTRL_ADDR_ADD_1 0
    #define PPUCTRL_ADDR_ADD_32 BIT_ON(2)
    //!< Bit[0, 1] BGの画面選択 (A or B or C or D)
    //!< A B
    //!< C D
    //!< それぞれ 256x240(32x30セル)
    //!< Vミラーリング時 : A, B が有効
    //!< Hミラーリング時 : C, D が有効
    #define PPUCTRL_BG_MASK (BIT_ON(0) | BIT_ON(1))
    #define PPUCTRL_BG_A 0
    #define PPUCTRL_BG_B 1
    #define PPUCTRL_BG_C 2
    #define PPUCTRL_BG_D 3
    #define PPUCTRL_BG_H(x) ((x) > 255 ? PPUCTRL_BG_B : PPUCTRL_BG_A);
    #define PPUCTRL_BG_V(y) ((y) > 239 ? PPUCTRL_BG_C : PPUCTRL_BG_A);

#define PPUMASK *(u8 *)0x2001
    //!< Bit7 青を強調表示
    #define PPUMASK_BLUE_EMP_MASK BIT_ON(7)
    #define PPUMASK_BLUE_EMP_ON BIT_ON(7)
    //!< Bit6 緑を強調表示
    #define PPUMASK_GREEN_EMP_MASK BIT_ON(6)
    #define PPUMASK_GREEN_EMP_ON BIT_ON(6)
    //!< Bit5 赤を強調表示
    #define PPUMASK_RED_EMP_MASK BIT_ON(5)
    #define PPUMASK_RED_EMP_ON BIT_ON(5)
    //!< Bit4 スプライトの表示
    #define PPUMASK_SPR_DISP_MASK BIT_ON(4)
    #define PPUMASK_SPR_DISP_ON BIT_ON(4)
    //!< Bit3 BG の表示
    #define PPUMASK_BG_DISP_MASK BIT_ON(3)
    #define PPUMASK_BG_DISP_ON BIT_ON(3)
    //!< Bit2 画面左端 8 ピクセルのスプライト表示
    #define PPUMASK_SPR_EDGE_MASK BIT_ON(2)
    #define PPUMASK_SPR_EDGE_ON BIT_ON(2)
    //!< Bit1 画面左端 8 ピクセルの BG 表示
    #define PPUMASK_BG_EDGE_MASK BIT_ON(1)
    #define PPUMASK_BG_EDGE_ON BIT_ON(1)
    //!< Bit0 グレースケール
    #define PPUMASK_GRAYSCALE_MASK BIT_ON(0)
    #define PPUMASK_GRAYSCALE_ON BIT_ON(0)

//!< Vブランク発生前 : PPU を直接制御しない処理を行う
//!< Vブランク発生中 : PPU を直接制御する処理を行う 
#define PPUSTATUS *(u8 *)0x2002
    //!< Bit7 Vブランク中かどうか
    #define PPUSTATUS_VBLANK_MASK BIT_ON(7)
    #define PPUSTATUS_IN_VBLANK BIT_ON(7)
    //!< Bit6 ラスタータイミングで使われる?
    #define PPUSTATUS_SPR_COLLIDE_MASK BIT_ON(6)
    #define PPUSTATUS_SPR_COLLIDE_ON BIT_ON(6)
    //!< Bit5 同じラインに 8 個以上のスプライト
    #define PPUSTATUS_SPR_OVERFLOW_MASK BIT_ON(5)
    #define PPUSTATUS_SPR_OVERFLOW_ON BIT_ON(5)
    //!< Bit[0, 4] 画面描画ラインの状態
    #define PPUSTATUS_LINE_MASK BIT_ON(4)

//!< OAM(Object Attribute Memory)
//!< 1. ワークアドレス(下位 8 ビットが 0x00 であること)に必要な情報を書き込んでおく
//!< 2. Vブランクを待つ
//!< 3. OAMADDR = 0 を書き込む
//!< 4. OAMDMA にワークアドレスの上位 8 ビットを書き込む
//!< 5. DMA 転送(256 バイト)が開始される、終了まで CPU は待機 
#define OAMADDR *(u8 *)0x2003
#define OAMDATA *(u8 *)0x2004
    #define OAM_DMA_START(x) OAMADDR = 0; OAMDMA = (x) >> 8
    //!< Bit7 上下反転
    #define SPR_ATTR_VFLIP_MASK BIT_ON(7)
    #define SPR_ATTR_VFLIP_ON BIT_ON(7)
    //!< Bit6 左右反転
    #define SPR_ATTR_HFLIP_MASK BIT_ON(6)
    #define SPR_ATTR_HFLIP_ON BIT_ON(6)
    //!< Bit5 プライオリティ
    #define SPR_ATTR_PRI_MASK BIT_ON(5)
    #define SPR_ATTR_PRI_FRONT_OF_BG 0
    #define SPR_ATTR_PRI_BEHIND_OF_BG BIT_ON(5)
    //!< Bit[2, 4] 未使用
    //!< Bit[0, 1] カラーパレット番号
    #define SPR_ATTR_COL_PLT_MASK (BIT_ON(0) | BIT_ON(1))
    #define SPR_ATTR_COL_PLT(x) (x) & SPR_ATTR_COL_PLT_MASK

#define PPUSCROLL *(u8 *)0x2005
    #define PPUSCROLL_BG(x, y) PPUSCROLL = (x) & 0xff; PPUSCROLL = (y)

//!< CPU から PPU のメモリー空間にアクセス
//!< 1. PPUADDR に書き込み先のアドレスを設定し
//!< 2. PPUDATA へ書き込むという手順を踏む
//!< PPUDATA に読み書きをすると、自動的に PPUADDR に加算(デフォルトは1、32にすることも可能)されるため連続的に読み書き可能 
#define PPUADDR *(u8 *)0x2006
    #define SET_PPUADDR(x) PPUADDR = (x) >> 8; PPUADDR = (x) & 0xff
#define PPUDATA *(u8 *)0x2007

#define PULSE1CTR1 *(u8 *)0x4000
     //!< Bit[6, 7] デューティー比
    #define PULSE1CTR1_MASK (BIT_ON(6) | BIT_ON(7))
    #define PULSE1CTR1_DUTY_12_5 (0 << 6)
    #define PULSE1CTR1_DUTY_25 (1 << 6)
    #define PULSE1CTR1_DUTY_50 (2 << 6)
    #define PULSE1CTR1_DUTY_75 (3 << 6)
    //!< Bit5 エンベロープループ
    #define PULSE1CTR1_ENVLOOP_MASK BIT_ON(5)
    #define PULSE1CTR1_ENVLOOP_ON BIT_ON(5)
    //!< Bit4 エンベロープ値(音を揺らす、フェードアウト)を使用 or ボリューム値を使用
    #define PULSE1CTR1_ENV_OR_VOL_MASK BIT_ON(4)
    #define PULSE1CTR1_VOL_ON BIT_ON(4)
    #define PULSE1CTR1_ENV_ON BIT_ON(4)
    //!< Bit[0, 3] エンベロープ or ボリュームの値 [0, 15]
    #define PULSE1CTR1_ENVVOLVAL_MASK (BIT_ON(0) | BIT_ON(1) | BIT_ON(2) | BIT_ON(3))
    #define PULSE1CTR1_ENVVOLVAL(x) ((x << 0) & PULSE1CTR1_ENVVOLVAL_MASK)

#define PULSE1CTR2 *(u8 *)0x4001
    //!< Bit7 周波数の変化 使用 or 未使用
    #define PULSE1CTR2_FREQ_CHG_MASK BIT_ON(7)
    #define PULSE1CTR2_FREQ_CHG_ON BIT_ON(7)
    //!< Bit[4, 6] 周波数の変化周期 [0, 7]
    #define PULSE1CTR2_FREQ_CHG_PERIOD_MASK (BIT_ON(4) | BIT_ON(5) | BIT_ON(6))
    #define PULSE1CTR2_FREQ_CHG_PERIOD(x) ((x << 4) & PULSE1CTR2_FREQ_CHG_PERIOD_MASK)
    //!< Bit3 変化の向き 低音に変化 or 高音に変化
    #define PULSE1CTR2_FREQ_CHG_DIR_MASK BIT_ON(3)
    #define PULSE1CTR2_FREQ_CHG_DIR_LOW 0
    #define PULSE1CTR2_FREQ_CHG_DIR_HIGH BIT_ON(3)
    //!< Bit[0, 2] 周波数変化量 [0, 7]
    #define PULSE1CTR2_FREQ_CHG_VAL_MASK (BIT_ON(0) | BIT_ON(1) | BIT_ON(2))
    #define PULSE1CTR2_FREQ_CHG_VAL(x) ((x << 0) & PULSE1CTR2_FREQ_CHG_VAL_MASK)

//!< 設定値 = ((1790000 / 16) / 音の周波数) - 1
#define PULSE1FREQ1 *(u8 *)0x4002
    //!< Bit[0, 7] 周波数値 [0, 255]
    #define PULSE1FREQ1_FREQ_VAL_MASK 0xff
    #define PULSE1FREQ1_FREQ_VAL(x) ((x) & PULSE1FREQ1_FREQ_VAL_MASK)

//!< 設定値 = ((1790000 / 16) / 音の周波数) - 1
#define PULSE1FREQ2 *(u8 *)0x4003
    //!< Bit[3, 7] 長さ [0, 31]
    #define PULSE1FREQ2_LEN_VAL_MASK (BIT_ON(3) | BIT_ON(4) | BIT_ON(5) | BIT_ON(6)| BIT_ON(7))
    #define PULSE1FREQ2_LEN_VAL(x) (((x) << 3) & PULSE1FREQ2_LEN_VAL_MASK)
    //!< Bit[0, 2] 周波数値 [0, 7]
    #define PULSE1FREQ2_FREQ_VAL_MASK (BIT_ON(0) | BIT_ON(1) | BIT_ON(2))
    #define PULSE1FREQ2_FREQ_VAL(x) ((x) & PULSE1FREQ2_FREQ_VAL_MASK)

#define OAMDMA *(u8 *)0x4014

#define SNDCHANNEL *(u8 *)0x4015
    //!< Bit[5, 7] NOT USED
    //!< Bit4 DMC(Delta Modulation Channel) 使用 or 未使用
    #define SNDCHANNEL_DMC_MASK BIT_ON(4)
    #define SNDCHANNEL_DMC_ON BIT_ON(4)
    //!< Bit3 ノイズ 使用 or 未使用
    #define SNDCHANNEL_NOISE_MASK BIT_ON(3)
    #define SNDCHANNEL_NOISE_ON BIT_ON(3)
    //!< Bit2 三角波 使用 or 未使用
    #define SNDCHANNEL_TRIANGLE_MASK BIT_ON(2)
    #define SNDCHANNEL_TRIANGLE_ON BIT_ON(2)
    //!< Bit1 矩形波2 使用 or 未使用
    #define SNDCHANNEL_SQUARE2_MASK BIT_ON(1)
    #define SNDCHANNEL_SQUARE2_ON BIT_ON(1)   
    //!< Bit0 矩形波1 使用 or 未使用
    #define SNDCHANNEL_SQUARE1_MASK BIT_ON(0)
    #define SNDCHANNEL_SQUARE1_ON BIT_ON(0)

//!< 1. JOYPAD1 = 1, 0 の順で書き込むとボタンの状態を記憶するようになる
//!< 2. JOYPAD1 の値を 8 回連続で読み込むと A, B, SELECT, START, UP, DOWN, LEFT, RIGHT の順で取得できる
#define JOYPAD1 *(u8 *)0x4016
#define JOYPAD2 *(u8 *)0x4017
    #define BUTTON_A        BIT_ON(0)
    #define BUTTON_B        BIT_ON(1)
    #define BUTTON_SELECT   BIT_ON(2)
    #define BUTTON_START    BIT_ON(3)
    #define BUTTON_UP       BIT_ON(4)
    #define BUTTON_DOWN     BIT_ON(5)
    #define BUTTON_LEFT     BIT_ON(6)
    #define BUTTON_RIGHT    BIT_ON(7)

//!< カラー
#define COLOR_BLACK      0x0f
#define COLOR_RED        0x16
#define COLOR_BLUE       0x12
#define COLOR_GREEN      0x1A
#define COLOR_YELLOW     0x28
#define COLOR_SKYBLUE    0x22 
#define COLOR_LIMEGREEN  0x29
#define COLOR_GRAY       0x00
#define COLOR_LIGHTGRAY  0x10 
#define COLOR_WHITE      0x30

