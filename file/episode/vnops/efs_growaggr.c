/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_growaggr.c,v $
 * Revision 1.1.85.1  1996/10/02  17:43:59  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:49  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1994-1990 Transarc Corporation - All rights reserved. */

/* growaggr.c -- Program to make grow Episode aggregate. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/cmd.h>
#include <dcedfs/astab.h>
#include <dcedfs/sysincludes.h>

#ifdef AFS_AIX31_ENV
#include <sys/mntctl.h>
#include <sys/statfs.h>
#endif /* AFS_AIX31_ENV */

#ifdef AFS_HPUX_ENV
#include <sys/vfs.h>
#endif /* AFS_HPUX_ENV */

#include <dcedfs/syscall.h>
#include <dcedfs/dir_buffer.h>
#include <dcedfs/episode/boot.h>
#include <efs_opcode.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_growaggr.c,v 1.1.85.1 1996/10/02 17:43:59 damon Exp $")

#ifdef	USE_FSCNTL
#define BLOCKSIZE 512
#else
#define BLOCKSIZE 1024
#endif

osi_cred_t Ucred;

/* find a mount point for the specified device */
static char *getmnt(dev)
char *dev;
{
#ifdef AFS_AIX31_ENV
    int fscount;
    int size = 5120;
    static char fsname[128];
    struct vmount *v;

    while(1)
    {
	if (!(v = (struct vmount *)malloc(size)))
	    return(NULL);
	if ((fscount = mntctl(MCTL_QUERY, size, (char *)v)) > 0)
	    break;
	else if (fscount == 0)
	{
	    size = *(int *)v;	/* need more space */
	    free(v);
	    continue;
	}
	else
	{
	    free(v);
	    return(NULL);
	}
    }
    /* we now have a set of vmount structures */
    while(fscount--)
    {
	if (strcmp(dev, vmt2dataptr(v, VMT_OBJECT)) == 0)
	{
	    (void) strcpy(fsname, vmt2dataptr(v, VMT_STUB));
	    return(fsname);
	}
	v = (struct vmount *)((char *)v + v->vmt_length);
    }
#endif /* AFS_AIX31_ENV */
    return(NULL);
}

/* count the blocks on the specified device */
static u_long countBlocks(RawDev, rfd, allocp, minAllocP)
char *RawDev;
int rfd;
u_long *allocp;
u_long *minAllocP;
{
    struct PartData EpiData;
    int EpiRes;
    u_long SBAddr;
    u_long DiskBlocks;

    bzero((caddr_t)&EpiData, sizeof(EpiData));
    EpiRes = edsk_CheckAsEpisode(rfd, RawDev, &EpiData, &SBAddr);
    close(rfd);

    if (EpiRes != edsk_EMPTY && EpiRes != edsk_FULL) 
    {
	(void) fprintf(stderr, "Specified device does not contain Episode aggregate\n");
	exit(1);
    }
    *allocp = (EpiData.TotalBlocks + EpiData.FirstBlock) *
	(EpiData.BlockSize / BLOCKSIZE);
    *minAllocP = EpiData.BlockSize * 3 / BLOCKSIZE;	/* minimum 3 blocks */
    DiskBlocks = EpiData.minBlkCount / (BLOCKSIZE / EpiData.minBlkSize);
    return(DiskBlocks);
}

static long growaggr (as, arock)
  IN struct cmd_syndesc *as;
  IN char *arock;
{
    u_long blocks;
    char Device[MAXPATHLEN + 1];
    char *MountPoint;
    int noAction;
    osi_statfs_t fs;
    u_long current;	/* current allocation in .5k blocks */
    u_long blocksWanted;
    char *bwP;
    char devname[1024];	/* device name */
    u_long minAllocation;
    struct astab astab;
    struct ufs_astab ufstab;
    FILE *filep;
    extern char *AstabFileName;
    char buffer[256];
    char RawDev[MAXPATHLEN + 1];
    dev_t DevNum;
    int rfd;

    noAction = (as->parms[2].items != 0);

    rfd = edsk_GetDevice(as->parms[0].items->data, RawDev, &DevNum, Device);
    if (rfd < 0) exit (1);

    blocks = countBlocks(RawDev, rfd, &current, &minAllocation);

    if (as->parms[1].items)
    {
	errno = 0;
	blocksWanted = strtoul(as->parms[1].items->data,&bwP,0);
	if ((errno == ERANGE) || (bwP == as->parms[1].items->data)) {
	    (void) fprintf(stderr, "Invalid argument to -aggrsize\n");
	    exit(1);
	}
	if (blocksWanted > blocks)
	{
	    (void) fprintf(stderr, "Requested blocks (%u) exceeds device capacity (%u)\n",
			   blocksWanted, blocks);
	    exit(1);
	}
	if (blocksWanted < current)
	{
	    (void) fprintf(stderr,
			   "Requested blocks (%u) less than current (%u)\n",
			   blocksWanted, current);
	    (void) fprintf(stderr, "Minimum allocation id %u\n", 
			   current+minAllocation);
	    exit(1);
	}
	blocks = blocksWanted;
    }
    if (noAction)
	(void) printf("aggregate %s holds %u (%dB) blocks, %u assigned\n", 
		      Device, blocks, BLOCKSIZE, current);
    else {

	if (blocks < current + minAllocation)
	{
	    (void) fprintf(stderr, "Not enough space: minimum allocation is %u (%dB) blocks\n",
			   current + minAllocation, BLOCKSIZE);
	    exit(1);
	}
	
	/*
	 * Two possible implementation methods at this point:
	 *
	 * - (Older method) Use fscntl system call.  Drawback:  this syscall exists
	 *   only on AIX.
	 *
	 * - Use afs_syscall.
	 */
	
#ifdef	USE_FSCNTL
	
#ifdef AFS_AIX31_ENV
#ifndef FS_EXTENDFS
#define FS_EXTENDFS 1
#endif
	if (!(MountPoint = getmnt(Device)))
	{
	    (void) fprintf(stderr, "No volumes mounted for %s\n", Device);
	    exit(1);
	}
	
	if (statfs(MountPoint, &fs) < 0)
	{
	    (void) fprintf(stderr, "Unable to statfs() %s\n", MountPoint);
	    exit(1);
	}
	if (fscntl(fs.f_vfsnumber, FS_EXTENDFS, blocks, sizeof(long)) < 0)
	{
	    (void) fprintf(stderr, "Extend failed\n");
	    exit(1);
	}
#else /* AFS_AIX31_ENV */
	(void) fprintf(stderr, "Feature not supported on this architecture\n");
	exit(1);
#endif /* AFS_AIX31_ENV */
	
#else	/* USE_FCNTL */
	
	if (afs_syscall(
	    AFSCALL_EPISODE, EPIOP_GROWAGGR, (int)Device, blocks, 0, 0) < 0) {
	    (void) fprintf (stderr, "Extend failed\n");
	    exit (1);
	}
	
#endif	/* USE_FCNTL */
	
    }
    return 0;
}

main(argc, argv)
int argc;
char *argv[];
{
    struct cmd_syndesc *ts;

    osi_setlocale(LC_ALL, "");

    dfs_define_vop_message_tables();

    ts = cmd_CreateSyntax((char *)0, (int (*)())growaggr, (char *)0,
			  "Grow an existing aggregate\n" );
    cmd_AddParm(ts, "-aggregate", CMD_SINGLE, 0, "name");
    cmd_AddParm (ts, "-aggrsize", CMD_SINGLE, CMD_OPTIONAL, "blocks");
    cmd_AddParm(ts, "-noaction", CMD_FLAG, CMD_OPTIONAL,
		"don't actually modify device");

    return cmd_Dispatch(argc, argv);
}
