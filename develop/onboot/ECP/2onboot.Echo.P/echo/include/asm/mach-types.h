/*
 * This was automagically generated from arch/arm/tools/mach-types!
 * Do NOT edit
 */

#ifndef __ASM_ARM_MACH_TYPE_H
#define __ASM_ARM_MACH_TYPE_H

#include <linux/config.h>

#ifndef __ASSEMBLY__
/* The type of machine we're running on */
extern unsigned int __machine_arch_type;
#endif

// Samsung SoC Platform(v) Machine numbering
// SOC_TYPE [31:28], SOC_NAME[27:12], SOC_REV[11:10], MACH(board)[9:0]
#define SPM_SOC_TYPE(a)		((a & 0xF) << 28)
#define SPM_SOC_NAME(b)		((b & 0xFFFF) << 12)
#define SPM_SOC_REV(c)		((c & 0x3) << 10)
#define SPM_MACH_TYPE(d)	(d & 0x3FF)

#define SPM_TYPE_S5H2X		0
#define SPM_TYPE_SDP		1
#define SPM_TYPE_S4		2

/* see arch/arm/kernel/arch.c for a description of these */
#define MACH_TYPE_SSA1FPGA	(SPM_SOC_TYPE(SPM_TYPE_SDP) | \
				SPM_SOC_NAME(75) | \
				SPM_SOC_REV(0) | \
				SPM_MACH_TYPE(0))

#define MACH_TYPE_ADONIS	(SPM_SOC_TYPE(SPM_TYPE_SDP) | \
				SPM_SOC_NAME(75) | \
				SPM_SOC_REV(0) | \
				SPM_MACH_TYPE(1))

#define MACH_TYPE_URANUS	(SPM_SOC_TYPE(SPM_TYPE_S5H2X) | \
				SPM_SOC_NAME(2150) | \
				SPM_SOC_REV(0) | \
				SPM_MACH_TYPE(1))

#define MACH_TYPE_BHLCD		(SPM_SOC_TYPE(SPM_TYPE_S5H2X) | \
				SPM_SOC_NAME(2150) | \
				SPM_SOC_REV(0) | \
				SPM_MACH_TYPE(12))

#if 0
#define MACH_TYPE_CRUX		(SPM_SOC_TYPE(SPM_TYPE_SDP) | \
				SPM_SOC_NAME(76) | \
				SPM_SOC_REV(0) | \
				SPM_MACH_TYPE(1))

#define MACH_TYPE_BHPLCD	(SPM_SOC_TYPE(SPM_TYPE_SDP) | \
				SPM_SOC_NAME(76) | \
				SPM_SOC_REV(0) | \
				SPM_MACH_TYPE(1))
#else
#define MACH_TYPE_CRUX		0x20003000

#define MACH_TYPE_BHPLCD	0x20003000
#endif
#define MACH_TYPE_SATURN	(SPM_SOC_TYPE(SPM_TYPE_S5H2X) | \
				SPM_SOC_NAME(2111) | \
				SPM_SOC_REV(0) | \
				SPM_MACH_TYPE(1))
#ifdef CONFIG_ARCH_SSA1FPGA
# ifdef machine_arch_type
#  undef machine_arch_type
#  define machine_arch_type	__machine_arch_type
# else
#  define machine_arch_type	MACH_TYPE_SSA1FPGA
# endif
# define machine_is_ssa1fpga()	(machine_arch_type == MACH_TYPE_SSA1FPGA)
#else
# define machine_is_ssa1fpga()	(0)
#endif

#ifdef CONFIG_ARCH_ADONIS
# ifdef machine_arch_type
#  undef machine_arch_type
#  define machine_arch_type	__machine_arch_type
# else
#  define machine_arch_type	MACH_TYPE_ADONIS
# endif
# define machine_is_adonis()	(machine_arch_type == MACH_TYPE_ADONIS)
#else
# define machine_is_adonis()	(0)
#endif

#ifdef CONFIG_MACH_URANUS
# ifdef machine_arch_type
#  undef machine_arch_type
#  define machine_arch_type	__machine_arch_type
# else
#  define machine_arch_type	MACH_TYPE_URANUS
# endif
# define machine_is_uranus()	(machine_arch_type == MACH_TYPE_URANUS)
#else
# define machine_is_uranus()	(0)
#endif

#ifdef CONFIG_MACH_BHLCD
# ifdef machine_arch_type
#  undef machine_arch_type
#  define machine_arch_type	__machine_arch_type
# else
#  define machine_arch_type	MACH_TYPE_BHLCD
# endif
# define machine_is_bhlcd()	(machine_arch_type == MACH_TYPE_BHLCD)
#else
# define machine_is_bhlcd()	(0)
#endif

#ifdef CONFIG_MACH_CRUX
# ifdef machine_arch_type
#  undef machine_arch_type
#  define machine_arch_type	__machine_arch_type
# else
#  define machine_arch_type	MACH_TYPE_CRUX
# endif
# define machine_is_crux()	(machine_arch_type == MACH_TYPE_CRUX)
#else
# define machine_is_crux()	(0)
#endif

#ifdef CONFIG_MACH_BHPLCD
# ifdef machine_arch_type
#  undef machine_arch_type
#  define machine_arch_type	__machine_arch_type
# else
#  define machine_arch_type	MACH_TYPE_BHPLCD
# endif
# define machine_is_crux()	(machine_arch_type == MACH_TYPE_BHPLCD)
#else
# define machine_is_crux()	(0)
#endif

#ifdef CONFIG_ARCH_SATURN
# ifdef machine_arch_type
#  undef machine_arch_type
#  define machine_arch_type	__machine_arch_type
# else
#  define machine_arch_type	MACH_TYPE_SATURN
# endif
# define machine_is_bhlcd()	(machine_arch_type == MACH_TYPE_SATURN)
#else
# define machine_is_bhlcd()	(0)
#endif

#ifndef machine_arch_type
#define machine_arch_type	__machine_arch_type
#endif

#endif
