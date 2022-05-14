#ifndef _BANK_MMC3_H_
#define _BANK_MMC3_H_

#define MMC3_BANK_SEL 0x8000
#define MMC3_BANK_DATA 0x8001
#define MMC3_SET_REG(sel, data) POKE(MMC3_BANK_SEL, (sel)); POKE(MMC3_BANK_DATA, (data))

#define MMC3_SEL_CHR0000 0 //!< 2KB (128 tiles)
#define MMC3_SEL_CHR0800 1 //!< 2KB
#define MMC3_SEL_CHR1000 2 //!< 1KB
#define MMC3_SEL_CHR1400 3 //!< 1KB
#define MMC3_SEL_CHR1800 4 //!< 1KB
#define MMC3_SEL_CHR1C00 5 //!< 1KB

#define MMC3_SEL_PRG8000 6 //!< 8KB
#define MMC3_SEL_PRGA000 7 //!< 8KB

#define MMC3_CHR_0000(data) MMC3_SET_REG(MMC3_SEL_CHR0000, data) 
#define MMC3_CHR_0800(data) MMC3_SET_REG(MMC3_SEL_CHR0800, data) 
#define MMC3_CHR_1000(data) MMC3_SET_REG(MMC3_SEL_CHR1000, data) 
#define MMC3_CHR_1400(data) MMC3_SET_REG(MMC3_SEL_CHR1400, data) 
#define MMC3_CHR_1800(data) MMC3_SET_REG(MMC3_SEL_CHR1800, data)
#define MMC3_CHR_1C00(data) MMC3_SET_REG(MMC3_SEL_CHR1C00, data)

#define MMC3_PRG_8000(data) MMC3_SET_REG(MMC3_SEL_PRG8000, data)
#define MMC3_PRG_A000(data) MMC3_SET_REG(MMC3_SEL_PRGA000, data) 

#endif //!< _BANK_MMC3_H_