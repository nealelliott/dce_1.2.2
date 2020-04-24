/*
 * Copyright (C) 1994 Transarc Corporation
 * All rights reserved.
 */

/*
 * allocinfo -- Utility to get file allocation information
 * 
 * Example output
 * 
 *  File zerofill.data, length 0x12000. Block map is
 *       Offset          Length            Type
 *
 *          0x0          0xa000       Allocated
 *       0xa000          0x6000       Hole
 *      0x10000          0x2000       Cow
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/errno.h>

#include <dcedfs/stds.h>		/* For RCSID */
#include <dcedfs/syscall.h>		/* For afs_syscall */
#include <dcedfs/episode/efs_opcode.h>	/* For EPIOP_DEBUG */
#include <dcedfs/episode/efs_debug.h>	/* For EFS_DEBUG_VNOPS */

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/sautils/RCS/allocinfo.c,v 1.3 1996/05/02 21:21:59 rajesh Exp $")

char *progName;

void Usage()
{
    fprintf(stderr, "Usage: %s <fileName>\n", progName);
    exit(1);
}

void PErrorExit(char *msg) {
    perror(msg);
    exit(1);
}

main(int argc, char **argv)
{
    char *fileName;
    int fd;
    long len;
    struct stat st;
    int code = 0; 	/* Does not matter now but initialize it anyway */

    progName = argv[0];

    if (argc < 2) {
	Usage();
	/* NOTREACHED */
    }

    fileName = argv[1];

    fd = open(fileName, O_RDONLY);
    if (fd < 0) {
	PErrorExit("open");
	/* NOTREACHED */
    }

    code = fstat(fd, &st);
    if (code < 0) {
	PErrorExit("fstat");
	/* NOTREACHED */
    }

    len = st.st_size;
    printf("File %s, length %#x. Allocation map is\n", fileName, len);
    code = GetAllocInfo(fd, len);
    return code;
}

#define NUM_REGIONS 10

int GetAllocInfo(int fd, long len)
{
    struct efs_allocInfo allocInfo;
    char *type;
    int code = 0, header = 0;

    allocInfo.regions = (struct efs_allocRegionInfo *)
	malloc(NUM_REGIONS * sizeof(struct efs_allocRegionInfo));
    if (allocInfo.regions == NULL)
	return ENOMEM;

    allocInfo.offset = 0;
    while (allocInfo.offset < len) {
	int i;

	allocInfo.numRegions = NUM_REGIONS;
	code = afs_syscall(AFSCALL_EPISODE, EPIOP_DEBUG, EFS_DEBUG_VNOPS,
		    EFS_DEBUG_VNOPS_GET_ALLOCINFO, fd, (int)&allocInfo);
	if (code < 0) {
	    PErrorExit("get allocation info system call");
	    /* NOTREACHED */
	}
	if (!header) {
	    printf("%10s\t%10s\t%10s\t%10s\n\n", 
		   "Offset", "Length", "Type", "Start Disk block");
	    header = 1;
	}
	for (i = 0; i < allocInfo.numRegions; i++) {
	    printf("%#10x\t%#10x\t", allocInfo.regions[i].offset, 
		   allocInfo.regions[i].length);
	    switch (allocInfo.regions[i].type) {
	      case efs_regionAllocated: 
		printf("%10s\t%#10x", "Allocated", allocInfo.regions[i].dblk);
		break;
	      case efs_regionCow:
		printf("%10s\t%#10x", "COW", allocInfo.regions[i].dblk);
		break;
	      case efs_regionHole: 
		printf("%10s", "Hole");
		break;
	      case efs_regionInline:
		printf("%10s", "Inline");
		break;
	      case efs_regionPurged:
		printf("%10s", "Purged");
		break;
	      default:
		fprintf(stderr, "Unknown region type %d\n", 
			(int)allocInfo.regions[i].type);
		exit(1);
	    }
	    printf("\n");
	}
    }
    return code;
}
	

