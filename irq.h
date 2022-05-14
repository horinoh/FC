#ifndef _IRQ_H_
#define _IRQ_H_

//!<    スキャンラインをカウントして、指定のカウントで IQR を発行可能
//!< レジスタA の値を任意のアドレスへ
#define MMC3_IRQ_LATCH 0xc000
#define MMC3_IRQ_RELOAD 0xc001
#define MMC3_IRQ_DISABLE 0xe000
#define MMC3_IRQ_ENABLE 0xe001
//#define STROBE(adr) __asm__ ("sta %w", adr)
#define IRQ_SET_VALUE(n)   POKE(MMC3_IRQ_LATCH, (n));
#define IRQ_RELOAD()       __asm__ ("sta %w", MMC3_IRQ_RELOAD)
#define IRQ_DISABLE()      __asm__ ("sta %w", MMC3_IRQ_DISABLE)
#define IRQ_ENABLE()       __asm__ ("sta %w", MMC3_IRQ_ENABLE)
//!< 6502 の割り込みフラグをクリア、これにより IQR が有効になる
#define ENABLE_CPU_IRQ __asm__ ("cli")
//!< この割込みが IQR か MNI か (IRQ の場合には A レジスタの上位ビットが立つ)
#define IS_IRQ (__A__ & 0x80)

#endif //!< _IRQ_H_