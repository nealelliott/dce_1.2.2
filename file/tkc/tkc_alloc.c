/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1994, 1989 Transarc Corporation - All rights reserved */

#include <tkc.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tkc/RCS/tkc_alloc.c,v 4.8 1994/11/04 22:39:28 cfe Exp $")

/*
 * Poor man's fixed size storage allocation: Speedy by avoiding calls to osi_{Alloc/Free}
 * whenever possible is the key goal here.
 */
			/* Size of largest struct alloced (tkc_tokenList) */
#define	MIN_ALLOCSIZE	(sizeof(struct tkc_tokenList))


static struct tkcSpace {
    struct tkcSpace *next;
} *freeSpaceList = 0;
static struct lock_data tcalloc_lock;


/* 
 * tkc_InitSpace -- Initialize space allocator. 
 */
void tkc_InitSpace()
{
    lock_Init(&tcalloc_lock);
}

/*
 * When no more available space in <freeSpaceList> allocate it via osi_Alloc
 */
char * tkc_AllocSpace() {
    register struct tkcSpace *tcp;

    lock_ObtainWrite(&tcalloc_lock);
    tkc_stats.spaceAlloced++;
    if (!freeSpaceList) {
	lock_ReleaseWrite(&tcalloc_lock);
	return (char *)osi_Alloc(MIN_ALLOCSIZE);
    }
    tcp = freeSpaceList;
    freeSpaceList = tcp->next;
    lock_ReleaseWrite(&tcalloc_lock);
    return (char *) tcp;
}


/*
 * Put the space back to <freeSpaceList> for later consumption
 */
void tkc_FreeSpace(datap)
    register char *datap;    
{
    lock_ObtainWrite(&tcalloc_lock);
    tkc_stats.spaceAlloced--;
    ((struct tkcSpace *)datap)->next = freeSpaceList;
    freeSpaceList = ((struct tkcSpace *)datap);
    lock_ReleaseWrite(&tcalloc_lock);
}
