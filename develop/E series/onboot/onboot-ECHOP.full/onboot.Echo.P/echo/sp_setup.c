#include <sp_setup.h>

struct tag * setup_start_tag(struct tag *params)
{
	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size(tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	return (tag_next(params));
}

struct tag * setup_commandline_tag(struct tag *params, char *commandline)
{
	char *p;
	int i;
	volatile int cnt;

	if(!commandline)
		return 0;

	/* eat leading white space */
	for(p=commandline; *p==' '; p++);

	/* skip non-existent command lines so the kernel will still
	 * use its default command line.
	 */
	if(*p == '\0')
		return 0;

	for(cnt=0; *p!='\0'; p++, cnt++);
	p -= cnt;

	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size = (sizeof(struct tag_header) + cnt + 1 + 4) >> 2;

	for(i=0; i<cnt; i++)
	{
		params->u.cmdline.cmdline[i] = p[i];
	}

	return (tag_next(params));
}

struct tag * setup_end_tag(struct tag *params)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
	
	return (tag_next(params));
}

void memcpy(void *pDst, void *pSrc, UINT32 nLen)
{
    register int  nCnt;
    register UINT8  *pD8, *pS8;
    register int  nL = nLen;
    register UINT32 *pD32, *pS32;

    pD8 = (UINT8*)pDst;
    pS8 = (UINT8*)pSrc;

    if ( ((int)pD8 % sizeof(UINT32)) == ((int)pS8 % sizeof(UINT32)) )
    {
        while ( (int)pD8 % sizeof(UINT32) )
        {
                *pD8++ = *pS8++;
                nL--;

                if( nL <= 0 )
                    return;
        }

        pD32 = (UINT32*)pD8;
        pS32 = (UINT32*)pS8;

        for (nCnt = 0; nCnt <(int)(nL / sizeof(UINT32)); nCnt++)
                *pD32++ = *pS32++;

        pD8 = (UINT8*)pD32;
        pS8 = (UINT8*)pS32;

        while( nL % sizeof(UINT32) )
        {
                *pD8++ = *pS8++;
                nL--;
        }
    }
    else
    {
        for( nCnt = 0; nCnt < nL; nCnt++)
                *pD8++ = *pS8++;
    }
}
void memset(void *pDst, UINT8 nV, UINT32 nLen)
{
    register UINT8  *pD8;
    register UINT32 *pD32;
    register UINT8  nV8 = nV;
    register UINT32 nV32 = (UINT32)( nV << 24 | nV << 16 | nV << 8 | nV );
    register int  nL = (int)nLen;
    register UINT32 nCnt;

    pD8 = (UINT8*)pDst;

    while ( (int)pDst % sizeof(UINT32) )
    {
        *pD8++ = nV8;
        nL--;

        if( nL <= 0 )
            return;
    }

    pD32 = (UINT32*)pD8;
    for (nCnt = 0; nCnt <(int)(nL / sizeof(UINT32)); nCnt++)
        *pD32++ = nV32;

    pD8 = (UINT8*)pD32;
    while( nL % sizeof(UINT32) )
    {
        *pD8++ = nV8;
        nL--;
    }

    return;
}