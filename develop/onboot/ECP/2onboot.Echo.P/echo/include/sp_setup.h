#ifndef __ASM_SP_TYPES_H
#define __ASM_SP_TYPES_H

#include <types.h>

//#define TAG_ADDR	(CFG_TEXT_BASE&0xF0000000+0x100)
#define ATAG_NONE	0x00000000
#define ATAG_CORE	0x54410001
#define ATAG_CMDLINE	0x54410009
#define TAG_SIZE	256

#define USE_UIMAGE_HEADER

struct tag_header {
	u32 size;
	u32 tag;
};

struct tag_core {
	u32 flags;
	u32 pagesize;
	u32 rootdev;
};

struct tag_cmdline {
	char cmdline[128];
};

struct tag {
	struct tag_header hdr;
	union {
		struct tag_core core;
		struct tag_cmdline cmdline;
	} u;
};

#define tag_next(t)     ((struct tag *)((u32 *)(t) + (t)->hdr.size))
#define tag_size(type)	((sizeof(struct tag_header) + sizeof(struct type)) >> 2)

struct tag * setup_start_tag(struct tag *params);
struct tag * setup_commandline_tag(struct tag *params, char *commandline);
struct tag * setup_end_tag(struct tag *params);
void memcpy(void *pDst, void *pSrc, UINT32 nLen);
void memset(void *pDst, UINT8 nV, UINT32 nLen);


#ifdef USE_UIMAGE_HEADER

#define IH_NMLEN        32      /* Image Name Length        */
typedef unsigned int  __u32;

/*
 * all data in network byte order (aka natural aka bigendian)
 */
typedef struct image_header {
	unsigned int    ih_magic;   /* Image Header Magic Number    */
	unsigned int    ih_hcrc;    /* Image Header CRC Checksum    */
	unsigned int    ih_time;    /* Image Creation Timestamp */
	unsigned int    ih_size;    /* Image Data Size      */
	unsigned int    ih_load;    /* Data  Load  Address      */
	unsigned int    ih_ep;      /* Entry Point Address      */
	unsigned int    ih_dcrc;    /* Image Data CRC Checksum  */
	unsigned char   ih_os;      /* Operating System     */
	unsigned char   ih_arch;    /* CPU architecture     */
	unsigned char   ih_type;    /* Image Type           */
	unsigned char   ih_comp;    /* Compression Type     */
	unsigned char   ih_name[IH_NMLEN];  /* Image Name       */
} image_header_t;

#define ___swab32(x) \
	((__u32)( \
		(((__u32)(x) & (__u32)0x000000ffUL) << 24) | \
		(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) | \
		(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) | \
		(((__u32)(x) & (__u32)0xff000000UL) >> 24) ))

#define    BLK_SIZE_SLC		(128*1024)  /* onenand block size */
#define    BLK_SIZE_MMC		(512)  /* LBA block size */
#define    BLK_SIZE_MMC_STRING	"512Byte"	/* MMC block size */

#endif /* end of USE_UIMAGE_HEADER */

#define __arch_getl(a)          (*(volatile unsigned int *)(a))
#define __arch_putl(v,a)        (*(volatile unsigned int *)(a) = (v))

#define readl(a)            __arch_getl(a)
#define writel(v,a)         __arch_putl(v,a)
#endif
