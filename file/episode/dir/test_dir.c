/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: test_dir.c,v $
 * Revision 1.1.64.1  1996/10/02  17:20:37  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:43  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1994 Transarc Corporation - All rights reserved */

#include <string.h>
#include <dcedfs/osi_device.h>
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <com_err.h>
#include <dcedfs/episode/critical.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/logbuf.h>

/*  #include "volreg.h"  */

/* user space I/O simulation */
#include <dcedfs/episode/us.h>

#include <dcedfs/episode/boot.h>
#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/fid.h>
#include <dcedfs/episode/volume.h>
#include <dcedfs/episode/file.h>
#include <dcedfs/episode/aggregate.h>

#include <dir.h>
#include <dir_buffer.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/dir/test_dir.c,v 1.1.64.1 1996/10/02 17:20:37 damon Exp $")

EXPORT int afsdb_episode_anode_test = 0;
/* this over rides DBprint from fixed_anode.h */
#undef DBprint
#define DBprint(p) dprintf (afsdb_episode_anode_test, p)
#define ALWAYSprint(p) dprintf (1, p)

#define ONE_MEG (1024*1024)

#define BIG_PRIME 1257056887
#define NewRandom(v) (randomSeed = randomSeed*BIG_PRIME + (v))

/* partitions we're working with */
static struct partition {
    struct async_device *device;
    epi_anode_t avl;
    u_long nBlocks;
    u_long fsize;
} partition[US_MAX_MINOR_DEV];

#define FRAGS_PER_BLOCK 4
#define FRAGMENT_SIZE 1024
#define BLOCK_SIZE (FRAGS_PER_BLOCK*FRAGMENT_SIZE)

/* for now these are just random constants */
#define FIRST_BLOCK 4			/* first legal block number */
#define N_LOG_BLOCKS 10

extern struct dh_ops dho;

long fidCounter = 0;

struct dirhandle {
    struct dh_ops *dh_ops;
    epi_anode_t fd;
    int uniq;
} dirhandle;

int Uniq;

char *whoami = "test_dir";

elbt_TranRec_t logb_zeroTrans = 0;

char *PrintedFid (fid)
  IN epi_volFileId_t *fid;
{
    static char fidPrintName[32];
    sprintf (fidPrintName, "(%d,%d)", epid_ToIndex(fid), epid_ToUnique(fid));
    return fidPrintName;
}

char *PrintedHyper (h)
  IN afs_hyper_t h;
{
    static char hPrintName[32];
    return dfsh_HyperToStr(&h, hPrintName);
}

long GetCurrentAVL (devIndex, deviceP, avlP)
  IN u_int devIndex;
  OUT struct async_device **deviceP;
  OUT epi_anode_t *avlP;
{
    long code;

    if (!(*deviceP = partition[devIndex].device)) {
	com_err (whoami, 0, "Device %d never opened", devIndex);
	return -1;
    }
    if (*avlP = partition[devIndex].avl) return 0;

    code = epig_InitAggregate (US_MAJOR_DEV, devIndex, deviceP, avlP);
    if (code) {
	com_err (whoami, code, "initializing aggregate");
	return code;
    }
    
    code = epig_AttachAggregate (*avlP);
    if (code) {
	com_err (whoami, code, "attaching aggregate");
	return code;
    }

    partition[devIndex].avl = *avlP;
    return 0;
}

long GetCurrentVol (avl, volId, volP)
  IN epi_anode_t avl;
  IN afs_hyper_t volId;			/* current volume id */
  INOUT epi_volume_t *volP;
{
    long code;
    epi_anode_t otherAvl;
    u_long volIndex;

    if (*volP) return 0;
    if (AFS_hiszero(volId)) {
	com_err (whoami, code = EPI_E_NOENT, "No current volume");
	return code;
    }

    code = vr_LookupVolumeId (volId, &otherAvl, &volIndex);
    if (code) {
	com_err (whoami, code,
		 "looking up current volume %s", PrintedHyper (volId));
	return code;
    }
    if (otherAvl != avl) {
	fprintf (stderr, "Volume id %s in unexpected aggregate\n", PrintedHyper(volId));
	return EPI_E_VOLIDEXISTS;
    }
	
    code = epig_OpenVolume (avl, volIndex, volP);
    if (code) {
	com_err (whoami, code,
		 "opening volume %s at index %d", PrintedHyper(volId), volIndex);
	return code;
    }
    return 0;
}

long GetCurrentFile (vol, fid, fileP)
  IN epi_volume_t vol;
  IN epi_volFileId_t *fid;
  INOUT epi_anode_t *fileP;
{
    long code;

    if (*fileP) return 0;
    if (epid_IsInvalid(fid)) {
	com_err (whoami, code = EPI_E_NOENT, "No current file");
	return code;
    }
    code = epif_Open (vol, fid, fileP);
    if (code) {
	com_err (whoami, code, "opening an anode in a volume");
	return code;
    }
    return 0;
}

PRIVATE long ClosePreviousAggr (devIndex)
  IN int devIndex;
{
    long code;
    struct async_device *device;
    epi_anode_t avl;

    device = partition[devIndex].device;
    if (!device) return 0;
    avl = partition[devIndex].avl;
    if (!avl) return 0;
    code = epig_CloseAggregate (avl);
    if (code) {
	com_err (whoami, code, "closing avl");
	return code;
    }
    partition[devIndex].avl = 0;
    return 0;
}

long ClosePreviousVol (volP, volIdP)
  INOUT epi_volume_t *volP;
  INOUT afs_hyper_t *volIdP;
{
    long code = 0;

    if (*volP) {
	WITH_TRANS("ClosePreviousVol", epia_AnodeDevice (*volP),
		   code = epiv_Close (trans, *volP));
	if (code) {
	    com_err (whoami, code, "closing previous volume %s",
		     PrintedHyper(*volIdP));
	}
    }
    AFS_hzero (*volIdP);
    *volP = 0;
    return code;
}

long ClosePreviousFile (fileP, fid)
  INOUT epi_anode_t *fileP;
  INOUT epi_volFileId_t *fid;
{
    long code = 0;

    if (*fileP) {
	WITH_TRANS ("ClosePreviousFile", epia_AnodeDevice (*fileP),
		    code = epif_Close (trans, *fileP));
	if (code) {
	    com_err (whoami, code,
		     "closing previous file %s", PrintedFid(fid));
	}
    }
    *fileP = 0;
    epid_MakeInvalid (&fid);
    dirhandle.fd = 0;
    return code;
}

PRIVATE u_long randomValueIndex, randomSeed;

PRIVATE void InitRandomValues (volId, fid, offset)
  afs_hyper_t volId;
  epi_volFileId_t *fid;
  u_long offset;
{
    randomSeed = AFS_hgethi(volId);
    NewRandom (AFS_hgetlo(volId));
    NewRandom (epid_ToIndex(fid));
    NewRandom (epid_ToUnique(fid));
    NewRandom (offset);
    randomValueIndex = 0;
}

PRIVATE void GetRandomValues (len, buf)
  int len;
  char *buf;
{
    static u_long lastWord;
    static int bytesLeft = 0;

    char *lastByte = (char *)&lastWord + sizeof(lastWord) - bytesLeft;

    while (len > 0) {
	if (bytesLeft == 0) {
	    lastWord = NewRandom (randomValueIndex++);
	    bytesLeft = sizeof(lastWord);
	    lastByte = (char *)&lastWord;
	}
	*buf++ = *lastByte++;
	len--;
	bytesLeft--;
    }
}

long RunTestV1 (tests, device, avl)
  IN FILE *tests;
  IN struct async_device *device;
  IN epi_anode_t avl;
{
    long code;
    int devIndex = 0;			/* current device number */
    epi_volume_t vol = 0;		/* current volume handle */
    afs_hyper_t volId;			/* current volume id */
    epi_anode_t file = 0;		/* current file handle */
    epi_volFileId_t fid;		/* current file id */

    AFS_hzero (volId);
    epid_MakeInvalid (&fid);
    while (1) {
	char op[32];			/* operation/command name */
	int oplen;
	char line[256];			/* line following cmd name */
	int linelen;
	int count;			/* count of args scanned in line */
	int existOkay;			/* exist vs create */

	code = 0;
	if (fscanf (tests, "%s", op) != 1) break;
	if (!fgets (line, sizeof(line), tests)) {
	    fprintf (stderr, "%s encountered while reading command %s\n",
		     (feof(tests)?"EOF":"ERROR"), op);
	    return -1;
	}
	linelen = strlen (line);
	if (line[linelen-1] != '\n') {
	    fprintf (stderr,
		     "command %s has too long an argument list ('%s')\n",
		     op, line);
	    return -1;
	}
	line[--linelen] = 0;

	{   char *comment;		/* beginning of comment string */
	    /* check for "blank" line */
	    if (op[0] == '#') continue;
	    /* shorten op name and line as appropriate */
	    if (comment = strchr(op, '#')) {
		*comment = 0;
		line[0] = 0;
	    }
	    else if (comment = strchr(line, '#')) *comment = 0;
	    linelen = strlen (line);
	}
	while ((line[linelen-1] == ' ') || (line[linelen-1] == '\t'))
	    line[--linelen] = 0;
#define NO_ARGS \
    if (linelen > 0) { \
        fprintf(stderr, "command %s doesn't take any arguments\n", op); \
	return -1; }

	lcstring (op, op, sizeof(op));
	oplen = strlen (op);
	DBprint (("%s %s", op, line));
#define IsOp(a) (strncmp(op, (a), oplen) == 0)

	if (IsOp("open_device")) {
	    u_long nBlocks;
	    u_long bsize;
	    u_long fsize;

	    if (code = ClosePreviousFile (&file, &fid)) return code;
	    if (code = ClosePreviousVol (&vol, &volId)) return code;
	    if (code = ClosePreviousAggr (devIndex)) return code;

	    count = sscanf (line, "%u %lu %lu %lu",
			    &devIndex, &bsize, &fsize, &nBlocks);
	    if (count < 1) {
od_usage:
		fprintf (stderr,
			 "Usage error: %s <minor dev number> [<block size> [<fragment size> [<n blocks>]]]\n",
			 op);
		return -1;
	    }
	    if ((devIndex < 0) || (devIndex > US_MAX_MINOR_DEV)) {
		fprintf (stderr,
			 "minor device number %d out of range\n", devIndex);
		return -1;
	    }
	    if (count == 1) { /* just switch default device */
		if (partition[devIndex].device == 0) {
		    fprintf (stderr,
			     "device never opened; must specify size\n");
		    goto od_usage;
		}
		if (!partition[devIndex].fsize) {
		    fprintf (stderr, "No fragment size specified\n");
		    goto od_usage;
		}
	    }
	    else { /* create new device */
		dev_t dev;
		osi_off_t size;

		if (partition[devIndex].device) {
		    fprintf (stderr, "device number %d exists\n", devIndex);
		    return -1;
		}

		code = asevent_InitDevice (US_MAJOR_DEV, devIndex, bsize,
					   &partition[devIndex].device);
		if (code) {
		    com_err (whoami, code, "calling async_InitDevice");
		    return code;
		}
		
		dev = osi_makedev(US_MAJOR_DEV, devIndex);
		code = us_ioctl (dev, EPIU_IOCTL_SET_DIR, "/tmp");
		if (code) {
		    com_err (whoami, code, "calling us_ioctl to set path");
		    return code;
		}
		code = us_open (dev, 0);
		if (code) {
		    com_err (whoami, code, "calling us_open");
		    return code;
		}
		if (count > 3) {
		    size = ((osi_off_t) nBlocks) * ((osi_off_t) bsize);
		    code = us_ioctl (dev, EPIU_IOCTL_SET_MAX_OFFSET, (caddr_t) &size);
		    if (code) {
			com_err (whoami, code, "calling us_ioctl to size");
			return code;
		    }
		    partition[devIndex].nBlocks = nBlocks;
		}
		if ((count > 2) && fsize)
		    partition[devIndex].fsize = fsize;
	    }
	    device = partition[devIndex].device;
	}
	else if (IsOp("check_device")) {
	    NO_ARGS;
	    if (partition[devIndex].device == 0) {
		fprintf (stderr, "device never opened\n");
		return -1;
	    }
	    code = epig_VerifyExistingAggregate(device,
				EPISODE_SUPERBLOCK_ADDR / device->blkSize,
						partition[devIndex].fsize, 0);
	    if (code) {
		/* code of -1 means that the err msg was already issued. */
		if (code != -1) com_err (whoami, code, "verifying an aggregate");
		return code;
	    }

	}
	else if (IsOp("salvage_device")) {
	    NO_ARGS;
	    if (partition[devIndex].device == 0) {
		fprintf (stderr, "device never opened\n");
		return -1;
	    }
	    code = epig_VerifyExistingAggregate(device,
				EPISODE_SUPERBLOCK_ADDR / device->blkSize,
						partition[devIndex].fsize, 1);
	    if (code) {
		/* code of -1 means that the err msg was already issued. */
		if (code != -1) com_err (whoami, code, "salvaging an aggregate");
		return code;
	    }

	}
	else if (IsOp("create_aggregate")) {
	    NO_ARGS;
	    if (partition[devIndex].avl) {
		fprintf (stderr, "AVL for device %d exists\n", devIndex);
		return -1;
	    }
	    if (!partition[devIndex].nBlocks) {
		fprintf (stderr, "No block size specified for device\n");
		return -1;
	    }
	    code = epig_CreateAggregate
		(device, partition[devIndex].fsize,
		 FIRST_BLOCK,
		 EPISODE_SUPERBLOCK_ADDR / device->blkSize,
		 partition[devIndex].nBlocks - FIRST_BLOCK,
		 N_LOG_BLOCKS, /*nExceptions*/0,0);
	    if (code) {
		com_err (whoami, code, "creating new aggregate");
		return code;
	    }
	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	}
	else if (IsOp("close_aggregate")) {
	    NO_ARGS;
	    if (code = ClosePreviousFile (&file, &fid)) return code;
	    if (code = ClosePreviousVol (&vol, &volId)) return code;
	    if (code = ClosePreviousAggr (devIndex)) return code;
	}
	else if (IsOp("create_volume") ||
		 (existOkay = IsOp("exist_volume"))) {
	    struct epiv_status status;
	    u_long volIndex;
	    u_long hi,lo;
	    afs_hyper_t id;
	    u_long quota;

	    if (code = ClosePreviousFile (&file, &fid)) return code;
	    if (code = ClosePreviousVol (&vol, &volId)) return code;

	    bzero (&status, sizeof(status));
	    count = sscanf (line, "%s %lu,,%lu %lu",
			    status.ident.name, &hi,&lo, &quota);
	    if ((count < 1) || (count == 2)) {
		fprintf (stderr,
			 "Usage error: %s <name> [hi,,lo [<quota>]]\n", op);
		return -1;
	    }
	    if (count < 4) quota = 100;
	    if (count < 2) AFS_hzero(id);
	    else AFS_hset64(id,hi,lo);

	    if (AFS_hiszero(id)) {		/* find unused id */
		while (code == 0) {
		    AFS_hset64 (id, 0, random());
		    code = vr_LookupVolumeId (id, 0, &volIndex);
		}
		if (code != EPI_E_NOENT) {
		    com_err (whoami, code,
			     "looking up volume %s", PrintedHyper(id));
		    return code;
		}
	    }

	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
		    
	    status.quota.limit = quota;
	    strncpy (status.statusMsg, line, sizeof(status.statusMsg));
	    status.ident.id = id;
	    WITH_TRANS ("create_volume", device, 
			code = epig_CreateVolume (trans, avl, 0,
						  &status, &volIndex));
	    if ((code == EPI_E_VOLIDEXISTS) && existOkay) {
		DBprint (("Volume %s now exists",
			  PrintedHyper(status.ident.id)));
	    } else if (code) {
		com_err (whoami, code, "creating a volume");
		return code;
	    }
	    else {
		struct epif_status fileStatus;
		elbt_TranRec_t trans;

		DBprint (("Created volume %s with id %s",
			  status.ident.name, PrintedHyper(status.ident.id)));
		GetCurrentVol (avl, id, &vol);
		bzero (&fileStatus, sizeof(fileStatus));
		fileStatus.type = (short) VDIR;
		fileStatus.mode = 0777;
		fileStatus.cid = fileStatus.oid = 100;
		fileStatus.gid = 200;

		elbb_StartTran ("make_root_vnode", 0, device, &trans);
		code = epif_Create (trans, vol, EPIF_CFLAGS_VOLUMEROOT,
				    /*estLength*/ 0,
				    &fileStatus, /*acl,plist*/ 0,0,
				    &fid);

		if (code) {
		    com_err (whoami, code, "creating a root dir in a volume");
		    return code;
		}
		elbb_EndTran (trans);
		epid_MakeInvalid (&fid);
	    }

	    volId = status.ident.id;
	}
	else if (IsOp("open_volume")) {
	    u_long hi,lo;

	    if (code = ClosePreviousFile (&file, &fid)) return code;
	    if (code = ClosePreviousVol (&vol, &volId)) return code;

	    count = sscanf (line, "%lu,,%lu", &hi,&lo);
	    if (count != 2) {
ov_usage:
		fprintf (stderr, "Usage error: %s [hi,,lo]\n", op);
		return -1;
	    }
	    AFS_hset64 (volId, hi,lo);
	    if (AFS_hiszero(volId)) goto ov_usage;

	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol(avl, volId, &vol)) return code;
	}
	else if (IsOp("create_file")) {
	    char name[32];		/* ignored for now */
	    struct epif_status fileStatus;

	    if (code = ClosePreviousFile (&file, &fid)) return code;

	    count = sscanf (line, "%s", name);
	    if (count < 1) {
		fprintf (stderr,
			 "Usage error: %s <name>\n", op);
		return (-1);
	    }

	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol(avl, volId, &vol)) return code;

	    bzero (&fileStatus, sizeof(fileStatus));
	    WITH_TRANS ("create_file", device,
			code = epif_Create (trans, vol,
					    /*flags*/0, /*estLength*/0,
					    &fileStatus, /*acl,plist*/0,0,
					    &fid));
	    if (code) {
		com_err (whoami, code, "creating an anode in a volume");
		return code;
	    }
	    DBprint (("fid is %s", PrintedFid (&fid)));
	}		
	else if (IsOp("open_file")) {
	    long a,b;

	    if (code = ClosePreviousFile (&file, &fid)) return code;

	    count = sscanf (line, " (%d,%d)", &a,&b);
	    if (count < 2) {
		fprintf (stderr,
			 "Usage error: %s (<n>,<n>)\n", op);
		return (-1);
	    }

	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol(avl, volId, &vol)) return code;

	    epid_Assemble (&fid, a,b);
	    code = epif_Open (vol, &fid, &file);
	    if (code) {
		com_err (whoami, code, "opening file %s", PrintedFid(&fid));
		return code;
	    }
	    dirhandle.dh_ops = &dho;
	    dirhandle.fd = file;
	    dirhandle.uniq = ++Uniq;
	}		
	else if (IsOp("write_file")) {
	    u_long offset;
	    u_long length;
	    char ranBuf[1234];
	    int useRandomValues;
	    int value;			/* value to check for */
	    int i;
	    struct uio uio;
	    struct iovec iov;

	    count = sscanf (line, "%lu %lu %d", &offset, &length, &value);
	    if (count < 2) {
wf_usage:
		fprintf (stderr,
			 "Usage error: %s <offset> <length> [<value>]\n", op);
		return (-1);
	    }
	    if (count == 2) useRandomValues = 1;
	    else useRandomValues = 0;
		    
	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol (avl, volId, &vol)) return code;
	    if (code = GetCurrentFile (vol, &fid, &file)) return code;

	    if (useRandomValues) InitRandomValues (volId, &fid, offset);
	    else for (i=0; i<sizeof(ranBuf); i++) ranBuf[i] = (char)value;

	    while (length > 0) {
		u_long len = sizeof(ranBuf);

		if (len > length) len = length;
		if (useRandomValues) GetRandomValues (len, ranBuf);

		epia_InitUio (&uio, &iov, offset, len, (opaque)ranBuf);
		code = epia_Write (logb_zeroTrans, file, &uio);
		if (code) {
		    com_err (whoami, code,
			     "writing to file %s", PrintedFid (&fid));
		    return code;
		}
		length -= len;
		offset += len;
	    }
	}
	else if (IsOp("read_file")) {
	    u_long offset;
	    u_long length;
	    char page[2143];
	    long pageLen = sizeof(page);
	    char ranBuf[sizeof(page)];
	    int useRandomValues;
	    int value;			/* value to check for */
	    int i;
	    struct uio uio;
	    struct iovec iov;

	    count = sscanf (line, "%lu %lu %d", &offset, &length, &value);
	    if (count < 2) goto wf_usage;
	    if (count == 2) useRandomValues = 1;
	    else useRandomValues = 0;

	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol (avl, volId, &vol)) return code;
	    if (code = GetCurrentFile (vol, &fid, &file)) return code;

	    if (useRandomValues) InitRandomValues (volId, &fid, offset);
	    else for (i=0; i<sizeof(ranBuf); i++) ranBuf[i] = (char)value;

	    while (length > 0) {
		if (pageLen > length) pageLen = length;
		epia_InitUio (&uio, &iov, offset, pageLen, page);
		code = epia_Read (file, &uio);
		if (code) {
		    com_err (whoami, code,
			     "reading %d bytes at offset %d from file %s",
			     pageLen, offset, PrintedFid (&fid));
		    return code;
		}
		if (useRandomValues) GetRandomValues (pageLen, ranBuf);

		if (bcmp(page, ranBuf, pageLen) != 0) {
		    fprintf (stderr, "error at offset=%d in %d bytes.\n",
			     offset, pageLen);
		    code = -1;
		    break;
		}
		length -= pageLen;
		offset += pageLen;
	    }
	    if (code) return code;
	}
	else if (IsOp("delete_file")) {
	    NO_ARGS;
	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol (avl, volId, &vol)) return code;
	    if (code = GetCurrentFile (vol, &fid, &file)) return code;

	    code = epif_ChangeLink(elbb_NullTransId, file, -1, 1, NULL);
	    if (code) {
		if (code == EPI_E_HASLINKS) {
		    long code2;
		    code2 = epif_Close(file);
		    afsl_MBZ(code2);
		}
		com_err (whoami, code,
			 "Failed to delete %s", PrintedFid(&fid));
		return code;
	    }
	    file = 0;
	    epid_MakeInvalid (&fid);
	}
	else if (IsOp("get_info")) {
	    char s[2][16];
	    struct epia_info info;
	    struct epif_status status;

	    count = sscanf (line, "%s %s", s[0], s[1]);

	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol (avl, volId, &vol)) return code;
	    if (code = GetCurrentFile (vol, &fid, &file)) return code;

	    code = epia_GetInfo (file, &info);
	    if (code) {
		com_err (whoami, code, "calling epia_GetInfo from get_info");
		return code;
	    }
	 /* code = epif_GetStatus (file, &status); */
	    if (code) {
		com_err (whoami, code, "calling epif_GetStatus from get_info");
		return code;
	    }
	    if (count < 1) {
		printf ("For %s: length=%s, allocated=%d\n",
			PrintedFid (&fid), PrintedHyper (info.length),
			info.allocated);
		printf ("  linkCount=%d, type=%d, mode=%d\n",
			status.linkCount, status.type, status.mode);
	    } else {
		int i;
		for (i=0; i<count; i++) {
		    char c = s[i][0];
		    long testValue;
		    long actual;
		    char *sp;

		    if (s[i][1] != '=') {
gi_usage:
			fprintf (stderr, "Illegal format in test string %s\n",
				 s[i]);
			return -1;
		    }
		    testValue = strtol(s[i]+2, &sp, 0);
		    if (*sp) goto gi_usage;
		    switch (c) {
		      case 'l':
			if (!AFS_hfitsinu32(info.length)) {
			    fprintf (stderr,
				     "File length longer than 2^32: %s",
				     PrintedHyper(info.length));
			    return -1;
			}
			actual = AFS_hgetlo(info.length);
			break;
		      case 'z':		/* siZe */
			actual = info.allocated;
			break;
		      case 'h':		/* Hard link count */
			actual = status.linkCount;
			break;
		      case 't':
			actual = status.type;
			break;
		      case 'p':		/* Protection bits */
			actual = status.mode;
			break;
		      default:
			goto gi_usage;
		    }
		    if (testValue != actual) {
			fprintf (stderr, "test %s fails, actual was %d\n",
				 s[i], actual);
			return -1;
		    }
		} /* for each test */
	    }
	}
	else if (IsOp("create_dir")) {
	    char ename[80];
	    char tbuffer[200];
	    register long i, code;
	    int nfiles, count;
	    long offset;

	    count = sscanf(line, "%s %d", ename, &nfiles);
	    if(count < 1 || count > 2) goto wf_usage;
	    if(count == 1) nfiles = 1;
	    code = dir_MakeDir(&dirhandle, &fid, &fid, 0);
	    if (code) {
		fprintf(stderr,"unsuccessful dir_MakeDir, code %d\n", code);
		return -1;
	    }
	    for(i=0;i<nfiles;i++) {
		epi_volFileId_t dummyfid;
		long offset;
		sprintf(tbuffer, "%s%d", ename, i);
		epid_Assemble (&fid, fidCounter, 2);
		fidCounter++;
		code = dir_Lookup(&dirhandle, tbuffer, &dummyfid, &offset);
		code = dir_Create(&dirhandle, tbuffer, &fid, offset, 0, 0);
		if (code) {
		    fprintf(stderr,"code for '%s' is %d\n", tbuffer, code);
		    return -1;
		}
	    }
	    printf("create directory entries\n");
	}
	else if (IsOp("list_dir")) {
	     u_long pieceno = 0;
	     struct Dir_Entry *entP;
	     struct Dir_Page *dpP;

	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol (avl, volId, &vol)) return code;
	    if (code = GetCurrentFile (vol, &fid, &file)) return code;

	     while ((pieceno = dir_PieceScan(&dirhandle, pieceno)) != 0) {
		 entP = (dir_GetPiece(&dirhandle,pieceno,&dpP));
		 printf("%s\t%d\t%d\n", entP->name, epid_ToIndex(&entP->fid),
			epid_ToUnique (&entP->fid));
		 pieceno += entP->length;
		 dirb_Release(dpP);
	     }
	}
	else if (IsOp("check_dir")) {

	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol (avl, volId, &vol)) return code;
	    if (code = GetCurrentFile (vol, &fid, &file)) return code;
	    
	    if(dirs_OK(&dirhandle, printf)) printf("Directory ok.\n");
	    else {
		fprintf(stderr,"Directory bad\n");
		return -1;
	    }
	}
	else if (IsOp("salvage_dir")) {
	    struct dirhandle newdir;
	    char dirname;

	    fprintf(stderr,"salvage_dir operation not supported (yet).");
	    return -1;
	    /* NOTREACHED */
#if 0
	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol (avl, volId, &vol)) return code;
	    if (code = GetCurrentFile (vol, &fid, &file)) return code;

	    count = sscanf (line, "%s", dirname);
	    if ((count < 1)) {
		fprintf (stderr,
			 "Usage error: %s <name>", op);
		return -1;
	    }
	    newdir.dh_ops = &dho;
/*	    newdir.fd = OPEN ANOTHER ANODE */
	    newdir.uniq = Uniq++;
	    
	    dirs_Salvage(&dirhandle, &newdir, printf);
#endif /* 0 */
	}
	else if (IsOp("lookup_dir")) {
	    char name[80];
	    epi_volFileId_t efid;
	    long offset;

	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol (avl, volId, &vol)) return code;
	    if (code = GetCurrentFile (vol, &fid, &file)) return code;

	    count = sscanf (line, "%s", name);
	    if ((count < 1)) {
		fprintf (stderr,
			 "Usage error: %s <name>", op);
		return -1;
	    }
	    code = dir_Lookup(&dirhandle, name, &efid, &offset);
	    if(!code)
		printf("Found fid %d.%d for file '%s'\n",
		       epid_ToIndex (&efid), epid_ToUnique (&efid), name);
	}
	else if (IsOp("add_dir_entry")) {
	    char name[80];
	    epi_volFileId_t efid;
	    long offset;

	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol (avl, volId, &vol)) return code;
	    if (code = GetCurrentFile (vol, &fid, &file)) return code;

	    count = sscanf (line, "%s", name);
	    if ((count < 1)) {
		fprintf (stderr,
			 "Usage error: %s <name>", op);
		return -1;
	    }
	    code = dir_Lookup(&dirhandle, name, &efid, &offset);
	    if(!code) code = EEXIST;
	    if(code == ENOENT) {
		epid_Assemble(&efid, fidCounter, 3);
		fidCounter++;
		code = dir_Create(&dirhandle, name, &efid, offset, 0, 0);
	    }
	}
	else if (IsOp("is_empty_dir")) {

	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol (avl, volId, &vol)) return code;
	    if (code = GetCurrentFile (vol, &fid, &file)) return code;

	    if (dir_IsEmpty(&dirhandle))
		printf("Directory is empty.\n");
	    else
		printf("Directory is not empty.\n");
	}
	else if (IsOp("pscan_dir")) {
	    int pieceno;
	    
	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol (avl, volId, &vol)) return code;
	    if (code = GetCurrentFile (vol, &fid, &file)) return code;

	    count = sscanf (line, "%d", &pieceno);
	    if ((count < 1)) {
		fprintf (stderr,
			 "Usage error: %s <piece number>", op);
		return -1;
	    }
	    printf("New piece Number: %d\n", dir_PieceScan(&dirhandle,
							   pieceno));
	}
	else if (IsOp("fixdd_dir")) {
	    epi_volFileId_t efid;
	    if (code = GetCurrentAVL(devIndex, &device, &avl)) return code;
	    if (code = GetCurrentVol (avl, volId, &vol)) return code;
	    if (code = GetCurrentFile (vol, &fid, &file)) return code;

	    epid_Assemble(&efid, fidCounter, 9);
	    fidCounter++;
	    code = dir_FixDotDot(&dirhandle, &efid, 0);
	    if(code) {
		com_err (whoami, code, "fixing dot dot dir");
		return code;
	    }
	}
	else if (IsOp("delete_dir")) {
	    char name[80];
	    int code, count;
	    long offset;

	    count = sscanf (line, "%s", name);
	    if ((count < 1)) {
		fprintf (stderr,
			 "Usage error: %s <name>", op);
		return -1;
	    }
	    code = dir_Lookup(&dirhandle, name, &fid, &offset);
	    if (!code) code = dir_Delete(&dirhandle, offset, 0);
	    if (code) printf("delete code is %d\n", code);
	    else printf("directory entry deleted:\t%d\t%d\n",
			epid_ToIndex (&fid), epid_ToUnique (&fid));

	}
	else if (IsOp("end")) {
	    NO_ARGS;
	    if (code = ClosePreviousFile (&file, &fid)) return code;
	    if (code = ClosePreviousVol (&vol, &volId)) return code;
	    if (code = ClosePreviousAggr (devIndex)) return code;

	    /* close other devices also */
	    for (devIndex=0; devIndex<US_MAX_MINOR_DEV; devIndex++)
		if (code = ClosePreviousAggr (devIndex)) return code;

	    return 0;
	}
	else {
	    fprintf (stderr, "Unknown command %s %s\n", op, line);
	    return EPI_E_NOENT;
	}
	fflush(stdout);
    }
    return code;
}

main(argc, argv)
  int   argc;
  char *argv[];
{
    char *bufs;
    dev_t dev;
    osi_off_t size;
    struct async_device *device;
    epi_anode_t avl;
    long code;
    int i, debugArg;

    int a;
    int newFS = 0;
    int newVol = 0;
    int newFile = 0;
    int checkFS = 0;
    char *testFile = 0;
    int nHandles = 1000;
    char *sp;

    initialize_epi_error_table();
    initialize_asy_error_table();
    initialize_lgb_error_table();

    dirb_Init(6);
    for (a=1; a<argc; a++) {
	int arglen = strlen(argv[a]);
	char arg[256];
	lcstring (arg, argv[a], sizeof(arg));
#define IsArg(a) (strncmp (arg,a, arglen) == 0)
	if (IsArg("-nonewfs")) newFS = 0;
	else if (IsArg("-newfs")) newFS = 1;
	else if (IsArg("-newvol")) newVol = 1;
	else if (IsArg("-newfile")) newFile = atoi(argv[++a]);
	else if (IsArg("-checkfs")) checkFS = 1;
	else if (IsArg("-fixfs")) checkFS = 2;
	else if (IsArg("-runtest")) testFile = argv[++a];
	else if (IsArg("-nhandles")) nHandles = atoi(argv[++a]);
	else if (IsArg("-settestdebug")) {
	    afsdb_episode_anode_test = strtol(argv[++a], &sp, 0);
	    if (*sp) goto usage;
	}
	else if (IsArg("-setasyncdebug")) {
	    afsdb_episode_async = strtol(argv[++a], &sp, 0);
	    if (*sp) goto usage;
	}
	else if (IsArg("-errorcode")) {
	    code = atoi(argv[++a]);
	    printf ("Message for %d is '%s'\n", code, dfs_dceErrTxt (code));
	    exit (0);
	}
	else {
	    fprintf (stderr, "unexpected arg '%s'\n", arg);
	  usage:
	    fprintf (stderr, "Usage is: '%s [-[no]newFS] [-newVol] \
[-newFile <volId>] [-nHandles <n>] [-runtest <testfile>] [-checkFS|-fixFS] \
[-set{,test,verify,async}debug <n>]\n", whoami);
	    exit (1);
	}
    }
    if (afsdb_episode_anode_test ||
	afsdb_episode_async) {
	printf ("Running with debuging on %#x (test), %#x (async).\n",
		afsdb_episode_anode_test, afsdb_episode_async);
    }

    {
    	struct epig_params epigParams;

	epigParams.elbbParams.bufferPool = (char *)malloc(ONE_MEG);
	epigParams.elbbParams.byteCount = ONE_MEG;
	epigParams.maxHandles = nHandles;
	if (code = epig_Init(&epigParams)) {
	    com_err (whoami, code, "during module initialization");
	    exit (2);
	}
    }

    for (i=0; i<US_MAX_MINOR_DEV; i++)
	partition[i].device = 0, partition[i].avl = 0;

    device = 0;
    avl = 0;
    if (newFS || newVol || newFile || checkFS) {
	code = asevent_InitDevice (US_MAJOR_DEV, 0, BLOCK_SIZE, &device);
	if (code) {
	    com_err (whoami, code, "calling async_InitDevice");
	    exit (3);
	}
	
	dev = makedev(device->majdev, device->mindev);
	code = us_ioctl (dev, EPIU_IOCTL_SET_DIR, "/tmp");
	if (code) {
	    com_err (whoami, code, "calling us_ioctl to set path");
	    exit (3);
	}
	code = us_open (dev, 0);
	if (code) {
	    com_err (whoami, code, "calling us_open");
	    exit (3);
	}
#define N_BLOCKS 1000
	size = ((osi_off_t) N_BLOCKS) * ((osi_off_t) BLOCK_SIZE);
	code = us_ioctl (dev, EPIU_IOCTL_SET_MAX_OFFSET, (caddr_t) &size);
	if (code) {
	    com_err (whoami, code, "calling us_ioctl to set path");
	    exit (3);
	}

	partition[0].device = device;
	partition[0].nBlocks = N_BLOCKS;
    }

    if (newFS) {
	    code = epig_CreateAggregate
		(device, FRAGMENT_SIZE,
		 FIRST_BLOCK,
		 EPISODE_SUPERBLOCK_ADDR / device->blkSize,
		 N_BLOCKS-FIRST_BLOCK, N_LOG_BLOCKS,
		 /*nExceptions*/0,0);
	    if (code) {
		com_err (whoami, code, "creating new aggregate");
		exit (4);
	    }
	}
	
    if (newVol || newFile) {
	code = epig_InitAggregate (US_MAJOR_DEV, 0, &device, &avl);
	if (code) {
	    com_err (whoami, code, "initializing aggregate");
	    exit (4);
	}
	
	debugArg = 1;				/* turn on */
	code = us_ioctl (dev, EPIU_IOCTL_SET_DEBUG, (caddr_t) &debugArg);
	if (code) {
	    com_err (whoami, code, "calling us_ioctl to turn on debugging");
	    exit (3);
	}
	
	code = epig_AttachAggregate (avl);
	if (code) {
	    com_err (whoami, code, "attaching aggregate");
	    exit (4);
	}
	
	partition[0].avl = avl;
	partition[0].fsize = FRAGMENT_SIZE;
    }

    srandom (1);
    if (newVol) {
	struct epiv_status status;
	u_long volIndex;

	bzero (&status, sizeof(status));
	strcpy (status.ident.name, "foo");
	status.quota.limit = 100;
	strcpy (status.statusMsg, "This is a test volume");
	do {
	    AFS_hset64(status.ident.id, 0, random());
	    code = epig_CreateVolume (logb_zeroTrans, avl, 0,
				      &status, &volIndex);
	} while (code == EPI_E_VOLIDEXISTS);
	if (code) {
	    com_err (whoami, code, "creating a volume");
	    exit (7);
	}
	fprintf (stderr, "Created volume %s\n", PrintedHyper(status.ident.id));
    }

    if (newFile) {
	u_long volIndex;
	epi_anode_t otherAvl;
	epi_anode_t vol;
	struct epif_status fileStatus;
	epi_volFileId_t fid;
	epi_anode_t file;
	afs_hyper_t volId;
	
	AFS_hset64 (volId, 0, newFile);
	code = vr_LookupVolumeId (volId, &otherAvl, &volIndex);
	if (code) {
	    com_err (whoami, code,
		     "looking up volume %s", PrintedHyper(volId));
	    exit (7);
	}
	code = epig_OpenVolume (avl, volIndex, &vol);
	if (code) {
	    com_err (whoami, code, "opening volume at index %d", volIndex);
	    exit (7);
	}
	bzero (&fileStatus, sizeof(fileStatus));
	code = epif_Create (logb_zeroTrans, vol, /*flags*/0, /*estLength*/0,
			    &fileStatus, /*acl,plist*/0,0, &fid);
	if (code) {
	    com_err (whoami, code, "creating an anode in a volume");
	    exit (7);
	}
	fprintf (stderr, "fid is (%d,%d)\n",
		 epid_ToIndex(&fid), epid_ToUnique(&fid));

	code = epif_Open (vol, &fid, &file);
	if (code) {
	    com_err (whoami, code, "opening an anode in a volume");
	    exit (7);
	}
	{
	    struct uio uio; struct iovec iov;
	    epia_InitUio (&uio, &iov, BLOCK_SIZE-10, 20,
			    "abcdefghijklmnopqrstuvwxyz");
	    code = epia_Write (logb_zeroTrans, file, &uio);
	}
	if (code) {
	    com_err (whoami, code, "writing to an anode");
	    exit (7);
	}
    }

    if (testFile) {    
	FILE *tests;
	char name[32];			/* test name */
	int version;			/* request tester version */

	tests = fopen (testFile, "r");
	if (!tests) {
	    com_err (whoami, errno, "Can't open file of tests %s", testFile);
	    exit (9);
	}
	fscanf (tests, "%s %d", name, &version);
	if (version != 1) {
	    com_err (whoami, 0, "Unknown tester version %d in test %s from file %s", version, name, testFile);
	    exit (9);
	}
	fprintf (stdout, "Running test %s (version %d) from file %s\n",
		 name, version, testFile);

	code = RunTestV1 (tests, device, avl);
	fclose (tests);
	if (code) {
	    printf ("Test %s was FAILED\n", name);
	    exit (9);
	}
	printf ("Test %s completed successfully\n", name);
    }


    if (checkFS) {
	code = epig_VerifyExistingAggregate(device,
		EPISODE_SUPERBLOCK_ADDR / device->blkSize,
		FRAGMENT_SIZE, (checkFS == 2 ? 1 : 0));
	if (code) {
	    /* code of -1 means that the err msg was already issued. */
	    if (code != -1) com_err (whoami, code, (checkFS == 2 ? "salvaging an aggregate" : "verifying an aggregate"));
	    exit (10);
	}
    }
    
    exit (0);
}
