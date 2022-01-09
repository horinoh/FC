#include "../fclib.h"

//!<
//!< 【パレット】
//!< 
#define BGCOLOR COLOR_SKYBLUE
#define NOTUSED 0
const u8 Palettes[] = { 
    BGCOLOR, 0x2b, 0x1b, 0x0b, //!< BGパレット0 : 緑系
    NOTUSED, 0x28, 0x18, 0x08, //!< BGパレット1 : 黄系
    NOTUSED, 0x26, 0x16, 0x06, //!< BGパレット2 : 赤系
    NOTUSED, 0x24, 0x14, 0x04, //!< BGパレット3 : 紫系
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
#define M0 1
#define M1 2
//0, 0, 0, 1, 1, 0, 0, 0,
//0, 0, 1, 1, 1, 1, 0, 0,
//0, 1, 1, 1, 1, 1, 1, 0,
//1, 1, 0, 1, 1, 0, 1, 1,
//1, 1, 1, 1, 1, 1, 1, 1,
//0, 0, 1, 0, 0, 1, 0, 0,
//0, 1, 0, 1, 1, 0, 1, 0,
//1, 0, 1, 0, 0, 1, 0, 1,

//0, 0, 0, 0, 1, 1, 0, 0,
//0, 0, 0, 0, 1, 0, 0, 0,
//0, 1, 1, 1, 1, 1, 1, 0,
//1, 1, 1, 1, 1, 1, 1, 1,
//1, 0, 1, 1, 1, 1, 1, 1,
//1, 0, 1, 1, 1, 1, 1, 1,
//1, 1, 1, 1, 1, 1, 1, 1,
//0, 1, 1, 1, 1, 1, 1, 0,
const u8 Patterns[] = {
    //!< パターン 0
    (BK & M0) << 7 | (BK & M0) << 6 | (BK & M0) << 5 | (C0 & M0) << 4 | (C0 & M0) << 3 | (BK & M0) << 2 | (BK & M0) << 1 | (BK & M0) << 0,
    (BK & M0) << 7 | (BK & M0) << 6 | (C0 & M0) << 5 | (C0 & M0) << 4 | (C0 & M0) << 3 | (C0 & M0) << 2 | (BK & M0) << 1 | (BK & M0) << 0,
    (BK & M0) << 7 | (C0 & M0) << 6 | (C0 & M0) << 5 | (C0 & M0) << 4 | (C0 & M0) << 3 | (C0 & M0) << 2 | (C0 & M0) << 1 | (BK & M0) << 0,
    (C0 & M0) << 7 | (C0 & M0) << 6 | (BK & M0) << 5 | (C0 & M0) << 4 | (C0 & M0) << 3 | (BK & M0) << 2 | (C0 & M0) << 1 | (C0 & M0) << 0,
    (C0 & M0) << 7 | (C0 & M0) << 6 | (C0 & M0) << 5 | (C0 & M0) << 4 | (C0 & M0) << 3 | (C0 & M0) << 2 | (C0 & M0) << 1 | (C0 & M0) << 0,
    (BK & M0) << 7 | (BK & M0) << 6 | (C0 & M0) << 5 | (BK & M0) << 4 | (BK & M0) << 3 | (C0 & M0) << 2 | (BK & M0) << 1 | (BK & M0) << 0,
    (BK & M0) << 7 | (C0 & M0) << 6 | (BK & M0) << 5 | (C0 & M0) << 4 | (C0 & M0) << 3 | (BK & M0) << 2 | (C0 & M0) << 1 | (BK & M0) << 0,
    (C0 & M0) << 7 | (BK & M0) << 6 | (C0 & M0) << 5 | (BK & M0) << 4 | (BK & M0) << 3 | (C0 & M0) << 2 | (BK & M0) << 1 | (C0 & M0) << 0,

    (BK & M1) << 7 | (BK & M1) << 6 | (BK & M1) << 5 | (C0 & M1) << 4 | (C0 & M1) << 3 | (BK & M1) << 2 | (BK & M1) << 1 | (BK & M1) << 0,
    (BK & M1) << 7 | (BK & M1) << 6 | (C0 & M1) << 5 | (C0 & M1) << 4 | (C0 & M1) << 3 | (C0 & M1) << 2 | (BK & M1) << 1 | (BK & M1) << 0,
    (BK & M1) << 7 | (C0 & M1) << 6 | (C0 & M1) << 5 | (C0 & M1) << 4 | (C0 & M1) << 3 | (C0 & M1) << 2 | (C0 & M1) << 1 | (BK & M1) << 0,
    (C0 & M1) << 7 | (C0 & M1) << 6 | (BK & M1) << 5 | (C0 & M1) << 4 | (C0 & M1) << 3 | (BK & M1) << 2 | (C0 & M1) << 1 | (C0 & M1) << 0,
    (C0 & M1) << 7 | (C0 & M1) << 6 | (C0 & M1) << 5 | (C0 & M1) << 4 | (C0 & M1) << 3 | (C0 & M1) << 2 | (C0 & M1) << 1 | (C0 & M1) << 0,
    (BK & M1) << 7 | (BK & M1) << 6 | (C0 & M1) << 5 | (BK & M1) << 4 | (BK & M1) << 3 | (C0 & M1) << 2 | (BK & M1) << 1 | (BK & M1) << 0,
    (BK & M1) << 7 | (C0 & M1) << 6 | (BK & M1) << 5 | (C0 & M1) << 4 | (C0 & M1) << 3 | (BK & M1) << 2 | (C0 & M1) << 1 | (BK & M1) << 0,
    (C0 & M1) << 7 | (BK & M1) << 6 | (C0 & M1) << 5 | (BK & M1) << 4 | (BK & M1) << 3 | (C0 & M1) << 2 | (BK & M1) << 1 | (C0 & M1) << 0,

    //!< パターン 1
    (BK & M0) << 7 | (BK & M0) << 6 | (BK & M0) << 5 | (BK & M0) << 4 | (C1 & M0) << 3 | (C1 & M0) << 2 | (BK & M0) << 1 | (BK & M0) << 0,
    (BK & M0) << 7 | (BK & M0) << 6 | (BK & M0) << 5 | (BK & M0) << 4 | (C1 & M0) << 3 | (BK & M0) << 2 | (BK & M0) << 1 | (BK & M0) << 0,
    (BK & M0) << 7 | (C1 & M0) << 6 | (C1 & M0) << 5 | (C1 & M0) << 4 | (C1 & M0) << 3 | (C1 & M0) << 2 | (C1 & M0) << 1 | (BK & M0) << 0,
    (C1 & M0) << 7 | (C1 & M0) << 6 | (C1 & M0) << 5 | (C1 & M0) << 4 | (C1 & M0) << 3 | (C1 & M0) << 2 | (C1 & M0) << 1 | (C1 & M0) << 0,
    (C1 & M0) << 7 | (BK & M0) << 6 | (C1 & M0) << 5 | (C1 & M0) << 4 | (C1 & M0) << 3 | (C1 & M0) << 2 | (C1 & M0) << 1 | (C1 & M0) << 0,
    (C1 & M0) << 7 | (BK & M0) << 6 | (C1 & M0) << 5 | (C1 & M0) << 4 | (C1 & M0) << 3 | (C1 & M0) << 2 | (C1 & M0) << 1 | (C1 & M0) << 0,
    (C1 & M0) << 7 | (C1 & M0) << 6 | (C1 & M0) << 5 | (C1 & M0) << 4 | (C1 & M0) << 3 | (C1 & M0) << 2 | (C1 & M0) << 1 | (C1 & M0) << 0,
    (BK & M0) << 7 | (C1 & M0) << 6 | (C1 & M0) << 5 | (C1 & M0) << 4 | (C1 & M0) << 3 | (C1 & M0) << 2 | (C1 & M0) << 1 | (BK & M0) << 0,

    (BK & M1) << 7 | (BK & M1) << 6 | (BK & M1) << 5 | (BK & M1) << 4 | (C1 & M1) << 3 | (C1 & M1) << 2 | (BK & M1) << 1 | (BK & M1) << 0,
    (BK & M1) << 7 | (BK & M1) << 6 | (BK & M1) << 5 | (BK & M1) << 4 | (C1 & M1) << 3 | (BK & M1) << 2 | (BK & M1) << 1 | (BK & M1) << 0,
    (BK & M1) << 7 | (C1 & M1) << 6 | (C1 & M1) << 5 | (C1 & M1) << 4 | (C1 & M1) << 3 | (C1 & M1) << 2 | (C1 & M1) << 1 | (BK & M1) << 0,
    (C1 & M1) << 7 | (C1 & M1) << 6 | (C1 & M1) << 5 | (C1 & M1) << 4 | (C1 & M1) << 3 | (C1 & M1) << 2 | (C1 & M1) << 1 | (C1 & M1) << 0,
    (C1 & M1) << 7 | (BK & M1) << 6 | (C1 & M1) << 5 | (C1 & M1) << 4 | (C1 & M1) << 3 | (C1 & M1) << 2 | (C1 & M1) << 1 | (C1 & M1) << 0,
    (C1 & M1) << 7 | (BK & M1) << 6 | (C1 & M1) << 5 | (C1 & M1) << 4 | (C1 & M1) << 3 | (C1 & M1) << 2 | (C1 & M1) << 1 | (C1 & M1) << 0,
    (C1 & M1) << 7 | (C1 & M1) << 6 | (C1 & M1) << 5 | (C1 & M1) << 4 | (C1 & M1) << 3 | (C1 & M1) << 2 | (C1 & M1) << 1 | (C1 & M1) << 0,
    (BK & M1) << 7 | (C1 & M1) << 6 | (C1 & M1) << 5 | (C1 & M1) << 4 | (C1 & M1) << 3 | (C1 & M1) << 2 | (C1 & M1) << 1 | (BK & M1) << 0,

    //!< ...
    //!< パターン 256
};
#undef M1
#undef M0
#undef C2
#undef C1
#undef C0
#undef BK

//!<
//!< 【ネームテーブル】パターンのマップ配置
//!< 
#define PT00 0x00
#define PT01 0x01
//...
#define PTff 0xff
//!< 32x30
const u8 NameTablesA[] = {
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 

    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
    PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00,  PT00, PT00, PT00, PT00, 
};
const u8 NameTablesB[] = {
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 

    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
    PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01,  PT01, PT01, PT01, PT01, 
};
#undef PTff
//...
#undef PT01
#undef PT00

//!<
//!< 【アトリビュート】ネームテーブルに対するパレットの指定
//!< 
//!<    BG_ATTR() 1つで、ネームテーブルの一区画(4x4)分 を指定
//!<    BG_ATTR(lt, rt, lb, rb) の各引数で、区画(4xr4)内の 2x2 部分を指定、どのパレットの連番4色を使用するかを指定している
#define PL0 BG_PAL0
#define PL1 BG_PAL1
#define PL2 BG_PAL2
#define PL3 BG_PAL3
const u8 AttributesA[] = {
    BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL0, PL1, PL2, PL3), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL3, PL2, PL1, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL0, PL1, PL2, PL3), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL3, PL2, PL1, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
};
const u8 AttributesB[] = {
    BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL1, PL1, PL1, PL1), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL2, PL2, PL2, PL2), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL3, PL3, PL3, PL3), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL1, PL1, PL1, PL1), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL2, PL2, PL2, PL2), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
    BG_ATTR(PL3, PL3, PL3, PL3), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0), BG_ATTR(PL0, PL0, PL0, PL0),
};
#undef PL3
#undef PL2
#undef PL1
#undef PL0

u8 NesMain()
{
    int i;
    u8 KeyState = 0;
    u8 x, y;

    VSYNC();
    
    //!< 表示オフ
    PPUMASK = 0;
    
    //!< サウンド初期化
    PULSE1_CTRL1 = 0;
    PULSE1_CTRL2 = 0;
    PULSE2_CTRL1 = 0;
    PULSE2_CTRL2 = 0;
    TRIANGLE_CTRL = 0;
    NOISE_CTRL = 0;
    SNDCHANNEL = 0;

    //!< ネームテーブル(マップ)クリア
    BGNameTableClear(PPU_BG_A_NAMETBL_ADDR);
    BGNameTableClear(PPU_BG_B_NAMETBL_ADDR);

    //!< スクロール
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

    //!< ネームテーブル(マップ)配置
    SET_PPUADDR(PPU_BG_A_NAMETBL_ADDR);
    for(i = 0;i < COUNTOF(NameTablesA);++i){
        PPUDATA = NameTablesA[i];
    }
    SET_PPUADDR(PPU_BG_B_NAMETBL_ADDR);
    for(i = 0;i < COUNTOF(NameTablesB);++i){
        PPUDATA = NameTablesB[i];
    }

    //!< アトリビュート (パレット指定)
    SET_PPUADDR(PPU_BG_A_ATTR_ADDR);
    for(i = 0;i < COUNTOF(AttributesA);++i) {
        PPUDATA = AttributesA[i];
    }
    SET_PPUADDR(PPU_BG_B_ATTR_ADDR);
    for(i = 0;i < COUNTOF(AttributesB);++i) {
        PPUDATA = AttributesB[i];
    }

    x = TILE_WIDTH >> 1;
    y = TILE_HEIGHT >> 1;
    BGPutNameTable(PPU_BG_A_NAMETBL_ADDR, x - 1, y, 1);
    BGPutNameTable(PPU_BG_A_NAMETBL_ADDR, x + 0, y, 1);
    BGPutNameTable(PPU_BG_A_NAMETBL_ADDR, x + 1, y, 1);
    BGPutNameTable(PPU_BG_A_NAMETBL_ADDR, x, y - 1, 1);
    BGPutNameTable(PPU_BG_A_NAMETBL_ADDR, x, y + 0, 1);
    BGPutNameTable(PPU_BG_A_NAMETBL_ADDR, x, y + 1, 1);

    //!< 表示オン
    PPUMASK = PPUMASK_BG_DISP_ON | PPUMASK_BG_EDGE_ON;

    x = y = 0;
    while(1){
        KeyState = ReadJoypad1();
        if(BUTTON_UP & KeyState) { --y; }
        if(BUTTON_DOWN & KeyState) { ++y; }
        if(BUTTON_LEFT & KeyState) { --x; }
        if(BUTTON_RIGHT & KeyState){ ++x; }
        if(BUTTON_A & KeyState){}
        if(BUTTON_B & KeyState){}
        if(BUTTON_SELECT & KeyState){}
        if(BUTTON_START & KeyState){}

        VSYNC();

        BGPutNameTable(PPU_BG_A_NAMETBL_ADDR,  8, 8, 1);
        BGPutNameTable(PPU_BG_A_NAMETBL_ADDR, 16, 8, 1);
        BGPutNameTable(PPU_BG_A_NAMETBL_ADDR, 24, 8, 1);

        //!< PPU 制御後、BG スクロール座標が変わってしまうので再設定する必要がある
        PPUSCROLL_BG(x, y);
        PPUCTRL = PPUCTRL_SPR_PAT_TBL_1000 | PPUCTRL_BG_H(x);
    }
    return 0;
}