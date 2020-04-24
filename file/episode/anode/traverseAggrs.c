/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* Copyright (C) 1995, 1994 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: traverseAggrs.c,v $
 * Revision 1.1.6.1  1996/10/02  17:19:34  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:06  damon]
 *
 * $EndLog$
 */

/* 
 * A hack to print out struct aggregates (aggregate.c) for the
 * attached aggregates. Used by RCX tests.  * * Only tested on Solaris.
 *
 * An example of the output on a system with one attached aggregate is
 * given below:
 *
[anode] $epiobj/anode/traverseAggrs
Aggr at 0xfc86670c, flags 0x4, ref count 2, avl anode 0xfc7a60e4, open vols 1
Aggr AVL anode at 0xfc7a60e4, refCount 1, AVL VT index 0
*/

#include <stdio.h>
#include <nlist.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <dcedfs/param.h>
#include <dcedfs/osi_device.h>
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/episode/instrument.h>
#include <dcedfs/episode/logbuf.h>
#include "anode.h"
#include "anode_block.h"
#include "fixed_anode.h"
#include "block_alloc.h"
#include "volume_table.h"
#include "volume.h"
#include "volreg.h"
#include "aggregate.h"
#include "badblock.h"
#include "file.h"

#define KSYMFILE "/dev/ksyms"
#define KMEMFILE "/dev/kmem"

/* Define data structures not available in header files */
struct diskAggregateHeader {
    int32 magic;
    u_int32 flags;
    u_int32 blockSize;
    u_int32 fragmentSize;
    u_int32 firstBlock;			/* first device block usable */
    u_int32 nBlocks;			/* nBlocks in allocation pool 
					 * from firstBlock onwards */
    u_int32 nLogBlocks;			/* size of log */
    u_int32 reservedIndexes;		/* for now this is AVL_INDEX */
    u_int32 bitmapIndex, logIndex;	/* indexes for these containers */
    u_int32 badblocksIndex;
    u_int32 timeCreated;
    u_int32 creatingProgram;		/* program that created the aggr */
    u_int32 diskFormatMinorVersion;	/* disk format minor version */
    int32 spare[10];			/* returned by GetAggregateStatus */
};

struct aggregate {
    struct aggregate *next;
    int tlock[6];
    long flags;
    long refCount;
    epi_anode_t avl;			/* corresponding avl */
    struct async_device *device;
    struct diskAggregateHeader hdr;	/* 24: hdr from superblock */
    epi_bitmap_t allocationHandle;
    u_long timeInited;
    u_long openVolumes;
};

void kreadm(off_t off, void *data, int len) 
{
    int fd;

    fd = open(KMEMFILE, O_RDONLY);
    lseek(fd, (off_t) off, SEEK_SET);
    read(fd, data, len);
    close(fd);
}

void displayAggr (unsigned long addr, struct aggregate aggr) 
{
    printf("Aggr at %#x, flags %#x, ref count %d, avl anode %#x, open vols %d\n",
	   addr, aggr.flags, aggr.refCount, aggr.avl, aggr.openVolumes);
}

void displayAnode(unsigned long addr, struct epia_anode anode) 
{
    printf("Aggr AVL anode at %#x, refCount %d, AVL VT index %d\n\n",
	   addr, anode.refCount, anode.volIndex);
}

main()
{
    int i;
    long data;
    struct nlist cnl[2] = { {"firstAggregate"}, {0} };
    unsigned long aggrAddr, anodeAddr;
    struct aggregate aggr;
    struct epia_anode anode;
    epi_anode_t anodeP;

    nlist(KSYMFILE, cnl);

    aggrAddr = cnl[0].n_value;
    kreadm(aggrAddr, &data, sizeof(long));
    aggrAddr = data;
    if (aggrAddr) {
	while (aggrAddr != 0) {
	    kreadm(aggrAddr, &aggr, sizeof(struct aggregate));
	    displayAggr(aggrAddr, aggr);
	    anodeAddr = (unsigned long) aggr.avl;
	    kreadm(anodeAddr, &anode, sizeof(struct epia_anode));
	    displayAnode(anodeAddr, anode);
	    aggrAddr = (unsigned long) aggr.next;
	}
    } else {
	printf("No attached aggregates\n");
    }
    return 0;
}

