/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dumpops.c,v $
 * $EndLog$
 */

/*
 * (C) Copyright 1996, 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <sys/types.h>

#include <dcedfs/stds.h>
#include <dcedfs/syscall.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/pipe.h>
#include <dcedfs/aclint.h>
#include <dcedfs/vol_init.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/async_queue.h>

#include <ftutil.h>
#include <ftu_dumpops.h>
#include <ftu_private.h>
#include <ftu_syscalls.h>
#include <ftu_trace.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftlmac.h>
#include <dfsftlsvc.h>
#include <dfsftlmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/ftutil/RCS/dumpops.c,v 1.72 1996/05/01 18:52:35 hyer Exp $")

struct aclHashEnt {
    struct aclHashEnt *next;
    struct dfs_acl *aclp;
    afs_hyper_t VU;
    long sourceWhich;
};
#define AHTSIZE 509
struct aclHashTable {
    struct aclHashEnt *Tbl[AHTSIZE];
};

static int verbose = 0;

#define DUMP_BUFCOUNT	3		/* number output buffers */
#define DUMP_BUFSIZE	(512*1024)	/* async buffer size */
#define DUMP_MINSIZE	(64*1024)	/* miniumum size to allocate */
#define DUMP_DIRSIZE	(64*1024)	/* directory buffer size */
#define DUMP_BUFALIGN	(2*sizeof(long))/* buffers are aligned on doublewords */

/* ACL hash table routines. */

PRIVATE int aclHashVal(aclp)
/* Compute the hash value for the given ACL. */
struct dfs_acl *aclp;
{
    unsigned int result;
    unsigned char *cp;
    unsigned int count;

    result = 0;
    cp = (unsigned char *) &aclp->dfs_acl_val[0];
    for (count = aclp->dfs_acl_len; count > 0; ++cp, --count) {
	result <<= 1;
	if (result >= AHTSIZE) result -= AHTSIZE;
	result += *cp++;
	if (result >= AHTSIZE) result -= AHTSIZE;
    }
    if (result > AHTSIZE) result = (result % AHTSIZE);
    return(result);
}

PRIVATE void addHashTable(htp, hv, aclp, vup, which)
/* Add aclp/vup/which to table htp. */
/* Don't care if we fail--just continue anyway. */
struct aclHashTable *htp;
int hv;
struct dfs_acl *aclp;
afs_hyper_t *vup;
long which;
{
    struct aclHashEnt *hep;
    struct dfs_acl *copyaclp;

    hep = (struct aclHashEnt *) osi_Alloc(sizeof (struct aclHashEnt));
    if (hep == NULL) return;
    /* don't allocate the whole thing */
    copyaclp = (struct dfs_acl *) osi_Alloc(aclp->dfs_acl_len +
					    sizeof(aclp->dfs_acl_len));
    if (copyaclp == NULL) {
	osi_Free(hep, sizeof(struct aclHashEnt));
	return;
    }
    bzero((char *)hep, sizeof(struct aclHashEnt));
    copyaclp->dfs_acl_len = aclp->dfs_acl_len;
    bcopy(aclp->dfs_acl_val, copyaclp->dfs_acl_val, copyaclp->dfs_acl_len);
    hep->aclp = copyaclp;
    hep->VU = *vup;
    hep->sourceWhich = which;
    /* link it in */
    hep->next = htp->Tbl[hv];
    htp->Tbl[hv] = hep;
}

PRIVATE struct aclHashEnt *lookHashTable(htp, aclp, hv)
/* Lookup aclp in htp. */
struct aclHashTable *htp;
struct dfs_acl *aclp;
int hv;
{
    struct aclHashEnt *hep;

    for (hep = htp->Tbl[hv]; hep != NULL; hep = hep->next) {
	if (aclp->dfs_acl_len == hep->aclp->dfs_acl_len
	    && bcmp(aclp->dfs_acl_val, hep->aclp->dfs_acl_val,
		    aclp->dfs_acl_len) == 0) {
	    return hep;
	}
    }
    return NULL;
}

/* async pipe I/O routine */
PRIVATE void asyncPipeWrite(elemP, argP)
asyncElement_t * volatile elemP;
void *argP;
{
    afsPipe_t * volatile pipeP = (afsPipe_t *)argP;
    char *excNameP;
    char excBuff[50];
    extern char * ExceptionToString();

    if ( elemP->count > 0 ) {
	TRY {
            if (POP_WRITE(pipeP, elemP->data, elemP->count) != elemP->count) {
	        elemP->last = 1;
	        elemP->error = EPIPE;
	    }
        } CATCH_ALL {
	    excNameP = ExceptionToString(THIS_CATCH);
	    if (excNameP == NULL) {
	        sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
	        excNameP = excBuff;
	    }
	    ftu_svc_LogPrintf(ftl_s_do_dump_exception, excNameP);
	    icl_Trace1(_ftu_iclSetP, FTU_TRACE_VOLS_DUMP_VOLUME_EXCEPTION,
		       ICL_TYPE_STRING, excNameP);
	    elemP->error = EPIPE;
        } ENDTRY
    } 
    elemP->offset = 0;
    elemP->count = 0;
    return;
}

/*
 * Dump the next chunk of data using the async queue
 *
 * This routine is intended to work with very large buffer sizes
 * and fairly small chunks. Large chunks should be dumped using the
 * vols_DumpAlloc/vols_DumpPush routines.
 */
PRIVATE long vols_DumpBytes(asyncP, bufp, len)
    asyncQueue_t *asyncP;
    char *bufp;
    long len;
{
    asyncElement_t *elemP;

    /*
     * if the current element doesn't have enough space then release it
     */
    elemP = (asyncElement_t *)asyncP->callerState;
    if (elemP != NULL && len > elemP->size - elemP->count) {
	relAsyncElement(asyncP, elemP);
	asyncP->callerState = NULL;
	elemP = NULL;
    }

    /*
     * if we don't have an element then get one
     */
     if (elemP == NULL) {
	elemP = getAsyncElement(asyncP);
	if (!elemP)
	    return EPIPE;
	asyncP->callerState = (void *)elemP;
	if (elemP->error)
	    return(elemP->error);
     }

    /*
     * this routine expects the data to fit in a single buffer
     */
    osi_assert((len <= elemP->size));
	
    /*
     * Copy the data into the current buffer.
     */
    bcopy(bufp, elemP->data + elemP->count, len);
    elemP->count += len;

    /*
     * if the current element is full, release it
     */
    if (elemP->count == elemP->size) {
	relAsyncElement(asyncP, elemP);
	asyncP->callerState = NULL;
    }

    return 0;
}

/*
 * Get a pointer into an async buffer element for reading fileset
 * data directly into the queue buffer.
 */

PRIVATE long vols_DumpAlloc(asyncP, hdrSize, inSize, outSizeP, outBufPP)
    asyncQueue_t *asyncP;
    long hdrSize;
    long inSize;
    long *outSizeP;
    char **outBufPP;
{
    asyncElement_t *elemP;
    long padSize;
    long minSize;
    long code, vols_DumpShortOpcode();

    /*
     * if the current element doesn't have enough space then release it
     */
    elemP = (asyncElement_t *)asyncP->callerState;
    if (elemP != NULL) {
	padSize = (elemP->count + hdrSize) % DUMP_BUFALIGN;
	if (padSize) {
	    padSize = DUMP_BUFALIGN - padSize;
	    if (padSize < sizeof(long))
		padSize += DUMP_BUFALIGN;
	    padSize -= sizeof(long);
	}
	minSize = padSize + hdrSize + DUMP_MINSIZE;
	if (minSize > elemP->size - elemP->count) {
	    relAsyncElement(asyncP, elemP);
	    asyncP->callerState = NULL;
	    elemP = NULL;
	}
    }

    /*
     * if we don't have an element then get one
     */
    if (elemP == NULL) {
	elemP = getAsyncElement(asyncP);
	if (!elemP)
	    return EPIPE;
	asyncP->callerState = (void *)elemP;
	if (elemP->error)
	    return(elemP->error);
	padSize = (elemP->count + hdrSize) % DUMP_BUFALIGN;
	if (padSize) {
	    padSize = DUMP_BUFALIGN - padSize;
	    if (padSize < sizeof(long))
		padSize += DUMP_BUFALIGN;
	    padSize -= sizeof(long);
	}
    }

    /*
     * pad the data stream so buffers are doubleword aligned.
     */
    if (padSize) {
	code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_PAD, padSize);
	if (code)
	    return code;
	elemP->count += padSize;
    }

    /*
     * return a pointer into the current element, if we don't have
     * enough to satify the request then return a multiple of DUMP_MINSIZE
     */
    *outSizeP = elemP->size - elemP->count - hdrSize;
    if (*outSizeP < inSize) {
	*outSizeP -= *outSizeP % DUMP_MINSIZE;
    } else {
	*outSizeP = inSize;
    }
    *outBufPP = elemP->data + elemP->count + hdrSize;
    return 0;
}

/*
 * release a buffer obtained with vols_DumpAlloc
 */
PRIVATE long vols_DumpPush(asyncP, bufSize, bufP)
asyncQueue_t *asyncP;
long bufSize;
char *bufP;
{
    asyncElement_t *elemP;

    /*
     * the buffer must start at the current position in this element
     * and cannot overrun the current buffer
     */
    elemP = (asyncElement_t *)asyncP->callerState;
    if (!elemP ||
        bufP != elemP->data + elemP->count ||
	elemP->count + bufSize > elemP->size) {
	return EPIPE;
    }

    /*
     * update the current element, if it is full then release it
     */
    elemP->count += bufSize;
    if (elemP->count == elemP->size) {
	relAsyncElement(asyncP, elemP);
	asyncP->callerState = NULL;
    }

    return 0;
}

/* dump a null-terminated string to the output */
PRIVATE long vols_DumpString(asyncP, stringp)
    asyncQueue_t *asyncP; 
    char *stringp;
{
    long tlen;
    long retval;

    tlen = strlen(stringp)+1;
    retval = vols_DumpBytes(asyncP, (char *)stringp, (unsigned)tlen);
    return (retval);
}


PRIVATE long vols_DumpShort(asyncP, ashort)
     asyncQueue_t *asyncP;
     unsigned short ashort; 
{
    unsigned short xshort;
    long retval;

    xshort = htons(ashort);
    retval = vols_DumpBytes(asyncP, (char *)&xshort, (unsigned)sizeof(xshort));
    return (retval);
}

PRIVATE long vols_DumpLong(asyncP, along)
    asyncQueue_t * asyncP;
    unsigned long along; 
{
    unsigned long xlong;
    long retval;

    xlong = htonl(along);
    retval = vols_DumpBytes(asyncP, (char *)&xlong, (unsigned)sizeof(xlong));
    return (retval);
}

/* Dump a afs_hyper_t as a pair of longs */
PRIVATE long vols_DumpHyper(asyncP, hp)
     asyncQueue_t *asyncP;
     afs_hyper_t *hp; 
  {
      long code;
      
      if (code = vols_DumpLong(asyncP, AFS_hgethi(*hp)))
	  return code;
      return (vols_DumpLong(asyncP, AFS_hgetlo(*hp)));
  }

/* dump a pair of longs as a timeval */
PRIVATE long vols_DumpTime(asyncP, timevalP)
     asyncQueue_t *asyncP;
     afsTimeval *timevalP; 
  {
      long code;
      
      if (code = vols_DumpLong(asyncP, (u_long) timevalP->sec))
	  return code;
      return (vols_DumpLong(asyncP, (u_long) timevalP->usec));
  }


PRIVATE long vols_DumpShortOpcode(asyncP, opcode, abytes)
    asyncQueue_t * asyncP;
    u_short opcode;
    u_short abytes; 
{
    long code;

    afsl_PAssertTruth((opcode & 0xf000) != 0xf000, ("Bad short opcode"));

    if (code = vols_DumpShort(asyncP, opcode))
	return code;
    if (code = vols_DumpShort(asyncP, abytes))
	return code;
    return 0;
}


PRIVATE long vols_DumpLongOpcode(asyncP, opcode, ahighBytes, alowBytes)
    asyncQueue_t * asyncP;
    u_short opcode;
    u_short ahighBytes; 
    u_long alowBytes;
{
    long code;

    afsl_PAssertTruth((opcode & 0xf000) == 0xf000, ("Bad long opcode"));

    if (code = vols_DumpShort(asyncP, opcode))	/* "short" opcode */
	return code;
    if (code = vols_DumpShort(asyncP, ahighBytes))/* high 16 bits of len. */
	return code; 
    if (code = vols_DumpLong(asyncP, alowBytes)) /* low 32 bits of length */
	return code; 
    return 0;
}

/*
 * Dump data bytes for this file.  Write out the data opcode, too.
 */
PRIVATE long vols_DumpData(asyncP, asize, vdesc)
    asyncQueue_t *asyncP;
    unsigned long asize;
    long vdesc;
{
    long code = 0;
    long dLen;
    afs_hyper_t dumpOff, dumpLen, xferLen;
    struct vol_ReadHole readHoleDesc;
    int end = 0;
    char *bufP;
    long hdrSize = 2*sizeof(short) + 3*sizeof(long);
    long tlen;

    if (asize == 0) 
	return 0;

    bzero((char *)&readHoleDesc, sizeof(readHoleDesc));
    AFS_hzero(dumpOff);
    AFS_hset64(dumpLen, 0, asize);
    while (1) {
	/* 
	 * XXX Could optimize out use of hypers as loop control variables.
	 */
	dLen = AFS_hgetlo(dumpLen);
	code = vols_DumpAlloc(asyncP, hdrSize, dLen, &tlen, &bufP);
	readHoleDesc.buffer = bufP;
	readHoleDesc.length = tlen;
	readHoleDesc.offset = dumpOff;
	code = VOL_READHOLE(vdesc, &readHoleDesc);
	if (code)
	    break;
	if (readHoleDesc.length != 0) {
	    code = vols_DumpLongOpcode(asyncP, VOLS_DUMP_VNODE_DATA, 0,
				       (u_long) (readHoleDesc.length + 
				       2 * sizeof(long)));
	    if (code) 
		break;
	    code = vols_DumpHyper(asyncP, &readHoleDesc.offset);
	    if (code) 
		break;
	    code = vols_DumpPush(asyncP, readHoleDesc.length,
				 readHoleDesc.buffer);
	    if (code)
		break;
	}
	xferLen = readHoleDesc.hole.holeLen;
	AFS_hadd32(xferLen, readHoleDesc.length);
	if (AFS_hcmp(dumpLen, xferLen) <= 0)
	    break;
	AFS_hsub(dumpLen, xferLen);
	AFS_hadd(dumpOff, xferLen);
    }
    return code;
}

/*
 * Dump a directory
 */
PRIVATE long vols_DumpDir(asyncP, bufP, vdesc)
    asyncQueue_t * asyncP;
    char *bufP;
    long vdesc;
{
    long code = 0;
    struct io_rwHyperDesc iodesc;
    u_long numEntries, i;
    struct vol_dirent *entryP;
    u_short entrySize;
    afs_hyper_t hOffset;
    int firstTimeAround = 1;

    AFS_hzero(iodesc.position);
    iodesc.bufferp = bufP;
    iodesc.length = DUMP_DIRSIZE;

    do {
	if ((code = VOL_READDIR(vdesc, &iodesc, &numEntries)) &&
	    code != VOL_ERR_EOF)
	    goto done;
	/*
	 * If code == VOL_ERR_EOF then numEntries is guaranteed to be 0,
	 * so we just fall through to the code after the do-while
	 */

	if (firstTimeAround) {
	    /*
	     * Put this off until we know that the VOL_READDIR call
	     * succeeded.  This allows us to take evasive action in
	     * vols_DumpVnode() in the event that VOL_READDIR is not
	     * implemented yet.
	     */
	    code = vols_DumpLongOpcode(asyncP, VOLS_DUMP_VNODE_DIR,
				       0, sizeof(u_int16));
	    if (code)
		goto done;

	    code = vols_DumpShort(asyncP,(u_int16)VOLS_INITIAL_DIRDUMP_VERSION);
	    if (code)
		goto done;

	    firstTimeAround = 0;
	}

	entryP = (struct vol_dirent *)iodesc.bufferp;
	for (i = 0; i < numEntries; i++) {
	    if (entryP->namelen > OSI_MAXNAMLEN) {
		ftu_svc_LogPrintf(ftl_s_do_name_too_long, entryP->namelen, OSI_MAXNAMLEN);
		code = EINVAL;
		goto done;
	    }

	    code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_DIR_ENTRY,
					sizeof(afs_hyper_t) + 2*sizeof(u_int32) + entryP->namelen+1);
	    if (code)
		goto done;

	    AFS_hset64(hOffset, 0, entryP->offset);
	    if (code = vols_DumpHyper(asyncP, &hOffset))
		goto done;

	    if (code = vols_DumpLong(asyncP, entryP->vnodeNum))
		goto done;

	    if (code = vols_DumpLong(asyncP, entryP->codesetTag))
		goto done;

	    if (code = vols_DumpString(asyncP, entryP->name))
		goto done;

	    entryP = (struct vol_dirent *)((char*)entryP + entryP->reclen);
	}
    } while (code != VOL_ERR_EOF);

    code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_DIR_END, 0);

 done:
    return code;
}

/* Translate an vattr type to an intermediate object */

SHARED ulong vattr_to_afsObject_type(vnodeType)
    enum vtype vnodeType;
{
    switch (vnodeType) {
      case VNON:
	return VOLS_VTYPE_NON;
      case VREG:
	return VOLS_VTYPE_REG;
      case VDIR:
	return VOLS_VTYPE_DIR;
      case VBLK:
	return VOLS_VTYPE_BLK;
      case VCHR:
	return VOLS_VTYPE_CHR;
      case VLNK:
	return VOLS_VTYPE_LNK;
      case VFIFO:
	return VOLS_VTYPE_FIFO;
#ifndef AFS_SUNOS5_ENV
      case VSOCK:
	return VOLS_VTYPE_SOCK;
#endif
#ifdef AFS_AIX31_ENV
      case VMPC:
	return VOLS_VTYPE_MPC;
#endif
      case VBAD:
      default:
	return VOLS_VTYPE_BAD;
    }
}

/* Translate from the intermediate object back to the vattr type */

SHARED enum vtype afsObject_to_vattr_type(type)
    ulong type;
{
    switch (type) {
      case VOLS_VTYPE_NON:
	return VNON;
      case VOLS_VTYPE_REG:
	return VREG;
      case VOLS_VTYPE_DIR:
	return VDIR;
      case VOLS_VTYPE_BLK:
	return VBLK;
      case VOLS_VTYPE_CHR:
	return VCHR;
      case VOLS_VTYPE_LNK:
	return VLNK;
      case VOLS_VTYPE_FIFO:
	return VFIFO;
#ifndef AFS_SUNOS5_ENV
      case VOLS_VTYPE_SOCK:
	return VSOCK;
#endif
#ifdef AFS_AIX31_ENV
      case VOLS_VTYPE_MPC:
	return VMPC;
#endif
      case VOLS_VTYPE_BAD:
      default:
	return VBAD;
    }
}

/*
 * Dump an individual vnode to a dump.
 */
PRIVATE long vols_DumpVnode(asyncP, bufP, vdesc, index, attrp, dumpBaseTypeP,
			    htp)
    asyncQueue_t * asyncP;
    char *bufP;
    long vdesc;
    long index;
    struct xvfs_attr *attrp;
    afs_hyper_t *dumpBaseTypeP;
    struct aclHashTable *htp;
{
    long code;
    int ix;
    /* struct xvfs_attr nattr; */
    struct dfs_acl acl;
    afs_hyper_t hypersize;
    static long aclTypes[] = {
	VNX_ACL_REGULAR_ACL,
	VNX_ACL_DEFAULT_ACL,
	VNX_ACL_INITIAL_ACL
    };
#define numAclTypes (sizeof(aclTypes) / sizeof(aclTypes[0]))
    int hv;
    struct aclHashEnt *hep;
    long afsType;
    afsTimeval timeDum;

    /* Dump the afs_hyper_t representing vnode / unique.  The header_vnode
     * field must come first in an individual vnode's dump, and the
     * dump, you'l see below, must end with a VOLS_DUMP_VNODE_EOF
     * field.
     */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_VNODE, 8))
	goto done;

    /* use logical index instead of actual one */
    AFS_hset64(attrp->xvattr.fileID, index, AFS_hgetlo(attrp->xvattr.fileID));

    if (code = vols_DumpHyper(asyncP, &attrp->xvattr.fileID))
	goto done;

    /* next, we dump all of the attributes we can find in the status, in
     * any order we feel like.  Status is already in attrp.
     */
    if (verbose) dce_svc_printf(FTL_S_DO_STATUS_ATTRIBUTES_MSG, attrp->vattr.va_type, AFS_hgethi(attrp->xvattr.fileID));
    /* dump the vnode's type field */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_TYPE, 4))
	goto done;
    afsType = vattr_to_afsObject_type(attrp->vattr.va_type);
    if (code = vols_DumpLong(asyncP, afsType))
	goto done;

    /* dump the vnode's hard link count */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_LINKCOUNT, 4))
	goto done;
    if (code = vols_DumpLong(asyncP, (u_long)attrp->vattr.va_nlink))
	goto done;

    /* dump the vnode's data version */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_DATAVERSION, 8))
	goto done;
    if (code = vols_DumpHyper(asyncP, &attrp->xvattr.dataVersion))
	goto done;

    /* dump the vnode's volume version # */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_VOLVERSION, 8))
	goto done;
    if (code = vols_DumpHyper(asyncP, &attrp->xvattr.volVersion))
	goto done;

    /* dump various times from the vnode and extended vnode attrs */

    /* access time */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_ACCESSTIME, 8))
	goto done;
    osi_UTimeFromSub(timeDum, attrp->vattr.va_atime);
    if (code = vols_DumpTime(asyncP, &timeDum))
	goto done;

    /* mod time */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_MODTIME, 8))
	goto done;
    osi_UTimeFromSub(timeDum, attrp->vattr.va_mtime);
    if (code = vols_DumpTime(asyncP, &timeDum))
	goto done;

    /* inode change time */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_CHANGETIME, 8))
	goto done;
    osi_UTimeFromSub(timeDum, attrp->vattr.va_ctime);
    if (code = vols_DumpTime(asyncP, &timeDum))
	goto done;

    /* server mod time; from extended attrs */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_SERVERMODTIME, 8))
	goto done;
    if (code = vols_DumpTime(asyncP, &attrp->xvattr.serverModTime))
	goto done;

    /* now dump various owner-like fields */

    /* the group */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_GROUP, 4))
	goto done;
    if (code = vols_DumpLong(asyncP, (u_long)attrp->vattr.va_gid))
	goto done;
    
    /* the author */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_AUTHOR, 4))
	goto done;
    if (code = vols_DumpLong(asyncP, (u_long)attrp->xvattr.author))
	goto done;
    
    /* the owner */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_OWNER, 4))
	goto done;
    if (code = vols_DumpLong(asyncP, (u_long)attrp->vattr.va_uid))
	goto done;
    
    /* the unix mode bits (the low order 12 bits thereof (the others are the file type) */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_UNIXMODE, 4))
	goto done;
    if (code = vols_DumpLong(asyncP, (u_long)attrp->vattr.va_mode))
	goto done;

    /* the client-only attribute bits */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_CLIENTATTRS, 4))
	goto done;
    if (code = vols_DumpLong(asyncP, (u_long)attrp->xvattr.clientOnlyAttrs))
	goto done;

    /* all ACLs */
    if (attrp->vattr.va_type == VDIR)
	ix = numAclTypes-1;	/* all ACL types */
    else
	ix = 0;			/* just the regular ACL */
    for (; ix >= 0; --ix) {
	code = VOL_GETACL(vdesc, &acl, aclTypes[ix]);
	if (!code) {
	    if (verbose)
		dce_svc_printf(FTL_S_DO_ACL_MSG,
			       AFS_hgethi(attrp->xvattr.fileID),
			       aclTypes[ix], acl.dfs_acl_len);
	    if (acl.dfs_acl_len > sizeof(acl.dfs_acl_val)) {
		dce_svc_printf(FTL_S_DO_FTSERVER_DUMP_MSG, aclTypes[ix],
			       AFS_hgethi(attrp->xvattr.fileID),
			       acl.dfs_acl_len, sizeof(acl.dfs_acl_val));
		ftu_svc_LogPrintf(ftl_s_do_dump_too_big, aclTypes[ix],
				  AFS_hgethi(attrp->xvattr.fileID),
				  acl.dfs_acl_len, sizeof(acl.dfs_acl_val));
	    } else {
		/* a good ACL */
		if (acl.dfs_acl_len == 0) {
		    hep = NULL;	/* always SET, never COPY, null ACLs */
		} else {
		    hv = aclHashVal(&acl);
		    hep = lookHashTable(htp, &acl, hv);
		}
		if (hep == NULL) {
		    /* Not in hash table.  Put it there first. */
		    if (acl.dfs_acl_len != 0) {
			/* store only non-zero-length ACLs */
			addHashTable(htp, hv, &acl, &attrp->xvattr.fileID,
				     aclTypes[ix]);
		    }
		    if (code = vols_DumpShortOpcode(asyncP,
						    VOLS_DUMP_VNODE_ACL_INLINE,
						    acl.dfs_acl_len+sizeof(long)))
			goto done;
		    if (code = vols_DumpLong(asyncP, (u_long)aclTypes[ix]))
			goto done;
		    if (code = vols_DumpBytes(asyncP, acl.dfs_acl_val,
					      acl.dfs_acl_len))
			goto done;
		} else {
		    /* In hash table.  Get dest to copy the ACL. */
		    if (code = vols_DumpShortOpcode(asyncP,
						    VOLS_DUMP_VNODE_ACL_COPY,
						    2*sizeof(long) + sizeof(afs_hyper_t)))
			goto done;
		    /* args: destWhich, sourceWhich, (vnode, unique) */
		    if (code = vols_DumpLong(asyncP, (u_long)aclTypes[ix]))
			goto done;
		    if (code = vols_DumpLong(asyncP, (u_long) hep->sourceWhich))
			goto done;
		    if (code = vols_DumpHyper(asyncP, &hep->VU))
			goto done;
		}
	    }
	} else {
	    if (AG_TYPE_SUPPORTS_EFS(AFS_hgethi(*dumpBaseTypeP)))
		goto done;		/* should always work on Episode */
	    if (code != EOPNOTSUPP && code != EINVAL && code != ENOTTY && code != ENOSYS)
		goto done;
	    /* fall through otherwise. */
	}
    }

    /* the file size */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_SIZE, 8))
	goto done;
    AFS_hset64 (hypersize, 0, (long)attrp->vattr.va_size);
    if (code = vols_DumpHyper(asyncP, &hypersize))
	goto done;

    /* finally, dump the file's data bytes or the directory contents */
    if (attrp->vattr.va_type != VDIR) {
	if (attrp->vattr.va_type == VCHR ||
	    attrp->vattr.va_type == VBLK)
	{    /* dump the device number*/
	    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_RDEV, 4))
		goto done;
	    if (code = vols_DumpLong(asyncP, ((u_long) attrp->vattr.va_rdev)))
		goto done;
	} else
	    if (code = vols_DumpData(asyncP, (long)attrp->vattr.va_size,
				     vdesc))
		goto done;
    } else
	if (code = vols_DumpDir(asyncP, bufP, vdesc)) {
	    if (code != ENOSYS) 
		goto done;

	    /* The new readdir volop isn't done yet, so use the old method */
	    if (code = vols_DumpData(asyncP, (long)attrp->vattr.va_size,
				     vdesc))
		goto done;
	}

    /* finally, we write the EOF for the vnode */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_VNODE_EOF, 0))
	goto done;

done:
    return code;
}

/*
 * Thread to periodally call the keep-alive routine for the dump
 * transaction. Doing this in a thread will assure that we stay 
 * if the dump is blocked waiting for a tape mount and we are unable
 * to transfer data.
 */

#define KA_DELAY 10		/* seconds between keep alive calls */

typedef struct vols_kaDump {
    long term;
    long done;
    osi_dlock_t lock;
    ftu_kaProc_t kaProc;
    void *kaArg;
} vols_kaDump_t;

void vols_DumpKeepAlive(argp, unused)
void *argp;
void *unused;
{
    vols_kaDump_t *kaBuf = (vols_kaDump_t *)argp;
    struct timespec absTime;
    int timeout;

    while(1) {
	absTime.tv_sec = osi_Time() + KA_DELAY;
	absTime.tv_nsec = 0;
	timeout = 0;
        lock_ObtainWrite(&kaBuf->lock);
	while (!timeout && !kaBuf->term) {
	    osi_TimedSleepW(kaBuf, &kaBuf->lock, &absTime, &timeout);
	    lock_ObtainWrite(&kaBuf->lock);
	}
	if (kaBuf->term) {
	    kaBuf->done = 1;
	    osi_Wakeup(kaBuf);
	    lock_ReleaseWrite(&kaBuf->lock);
	    return;
	}
	lock_ReleaseWrite(&kaBuf->lock);
	CALL_KA_PROC(kaBuf->kaProc, kaBuf->kaArg);
    }
}


/* 
 * Dump an entire volume since a particular time.  Start off by dumping the
 * basic volume info, and then proceed by iterating through all of the vnodes
 * contained in the volume.  Dump each one individually (checking its version
 * if need be, if this is incremental).  Finally, dump the volume EOF label
 * and we're done.
 */
SHARED long vols_DumpVolume(fsetDesc, dumpCriteriaP, pipeP, aggrType,
			    kaProc, kaArg)
     ftu_fsetDesc_t fsetDesc;
     ftu_dumpCriteria_t *dumpCriteriaP;
     afsPipe_t *pipeP;
     afs_hyper_t *aggrType;
     ftu_kaProc_t kaProc;
     void *kaArg;
  {
    long code;
    long temp, xcode, vdesc;
    long uids[VOLS_DUMP_NUIDS];
    struct xvfs_attr attr;
    afs_hyper_t thyper;
    int delFlag, dumpFlag;
    struct vol_status volStatus;
    long i, delIx;
    long firstDeletedIx;	/* [firstDeletedIx..i) should be deleted */
    char *excNameP;
    char excBuff[50];
    afs_hyper_t origVV, fileOrigVV, maxPreFileVV, maxPostFileVV;
    volatile struct aclHashTable ht;
    struct aclHashEnt *hep, *nhep;
    extern char * ExceptionToString();
    struct afsTimeval currTime;
    asyncQueue_t * volatile asyncP;
    asyncElement_t * volatile elemP;
    char *bufP = NULL;
    vols_kaDump_t *kaBuf = NULL; 

    /* start a thread to do the transaction keepalives */
    kaBuf = (vols_kaDump_t *)osi_Alloc(sizeof(vols_kaDump_t));
    if (kaBuf == NULL) {
	code = FTU_E_NO_MEMORY;
	goto done;
    }
    lock_Init(&kaBuf->lock);
    kaBuf->done = 0;
    kaBuf->term = 0;
    kaBuf->kaProc = kaProc;
    kaBuf->kaArg = kaArg;
    osi_ThreadCreate(vols_DumpKeepAlive, (void *)kaBuf, 0, 0, "ka", code);
    if (code) {
	osi_Free(kaBuf, sizeof(vols_kaDump_t));
	kaBuf = NULL;
	goto done;
    }

    /* allocate a data buffer */
    bufP = osi_Alloc(DUMP_DIRSIZE+OSI_MAXNAMLEN+1);
    if ( bufP == NULL ) {
	code = FTU_E_NO_MEMORY;
	goto done;
    }

    /* initialize the async output queue */
    asyncP = initAsyncQueue(DUMP_BUFCOUNT, DUMP_BUFSIZE,
			    asyncPipeWrite, NULL, pipeP);
    if ( !asyncP ) {
	code = EPIPE;
	goto done;
    }

    bzero((char *)&ht, sizeof(ht));
    TRY {
    /* dump the volume header */
    if (code = VOL_GETSTATUS(fsetDesc, &volStatus))
	goto done;

    /* save a copy for the terminal check */
    origVV = volStatus.vol_dy.volversion;
    AFS_hzero(maxPreFileVV);
    AFS_hzero(maxPostFileVV);
    osi_GetTime((struct timeval *)&currTime);

    /* dump the dump header field */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_START, 4))
	goto done;
    temp = VOLS_DUMP_HEADER_STARTMAGIC;
    if (code = vols_DumpLong(asyncP, (u_long) temp))
	goto done;

    /* dump the NUIDS */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_UNIQUEGEN, VOLS_DUMP_NUIDS*4))
	goto done;
    bzero((char *)uids, sizeof(uids));
    uids[0] = AFS_hgethi(volStatus.vol_dy.volversion);
    uids[1] = AFS_hgetlo(volStatus.vol_dy.volversion);
    for (i = 0; i < VOLS_DUMP_NUIDS; i++) {
	if (code = vols_DumpLong(asyncP, (u_long) uids[i]))
	    goto done;
    }

    /* write the visible quota limit */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_VISQUOTALIMIT, 8))
	goto done;
    if (code = vols_DumpHyper(asyncP, &volStatus.vol_dy.visQuotaLimit))
	goto done;

    /* write the allocation quota limit */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_ALLOCLIMIT, 8))
	goto done;
    if (code = vols_DumpHyper(asyncP, &volStatus.vol_dy.allocLimit))
	goto done;

    /* write the min quota */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_MINQUOTA, 4))
	goto done;
    if (code = vols_DumpLong(asyncP, (u_long) volStatus.vol_dy.minQuota))
	goto done;

    /* don't write DISKUSED or FILECOUNT, since setting it is inappropriate. */

    /* dump the volume owner */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_OWNER, 4))
	goto done;
    if (code = vols_DumpLong(asyncP, (u_long) volStatus.vol_dy.owner))
	goto done;

    /* dump the volume's creation date */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_CREATION_DATE, 8))
	goto done;
    if (code = vols_DumpTime(asyncP, &volStatus.vol_dy.creationDate))
	goto done;

    /* dump the volume's access date */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_ACCESS_DATE, 8))
	goto done;
    if (code = vols_DumpTime(asyncP, &volStatus.vol_dy.accessDate))
	goto done;

    /* dump the volume's update date */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_UPDATE_DATE, 8))
	goto done;
    if (code = vols_DumpTime(asyncP, &volStatus.vol_dy.updateDate))
	goto done;

    /* write the max index */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_INDEX_MAX, 4))
	goto done;
    if (code = vols_DumpLong(asyncP, (u_long) volStatus.vol_dy.nodeMax))
	goto done;

    /* dump the fileset uniquifier */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_N_UNIQUEGEN, 4))
	goto done;
    if (code = vols_DumpLong(asyncP, (u_long) volStatus.vol_dy.unique))
	goto done;

    /* dump the MOTD */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_MOTD_CODESET, 4))
	goto done;
    if (code = vols_DumpLong(asyncP, volStatus.vol_dy.tag))
	goto done;
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_MOTD,
				  strlen(volStatus.vol_dy.statusMsg)+1))
	goto done;
    if (code = vols_DumpString(asyncP, volStatus.vol_dy.statusMsg))
	goto done;

    /* dump the volume version number */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_VOLVERSION, 8))
	goto done;
    if (code = vols_DumpHyper(asyncP, &volStatus.vol_dy.volversion))
	goto done;

    /* Dump the code for the type of underlying aggregate. */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_BASETYPE, 8))
	goto done;
    if (code = vols_DumpHyper(asyncP, aggrType))
	goto done;

    /* dump the current date */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_DUMP_DATE, 8))
	goto done;
    if (code = vols_DumpTime(asyncP, &currTime))
	goto done;

    /* Dump this dump's source style. */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_DUMP_STYLE, 11*4))
	goto done;
    if (code = vols_DumpLong(asyncP, (u_long) dumpCriteriaP->dc_type))
	goto done;
    if (code = vols_DumpTime(asyncP, &dumpCriteriaP->dc_date))
	goto done;
    if (code = vols_DumpHyper(asyncP, &dumpCriteriaP->dc_version))
	goto done;
    if (code = vols_DumpHyper(asyncP, &dumpCriteriaP->dc_spare1))
	goto done;
    if (code = vols_DumpHyper(asyncP, &dumpCriteriaP->dc_spare2))
	goto done;
    if (code = vols_DumpHyper(asyncP, &dumpCriteriaP->dc_spare3))
	goto done;

    /* Dump the inconsistent opcode if the source fileset is inconsistent */
    if (volStatus.vol_st.states & VOL_DELONSALVAGE) {
	code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_INCONSISTENT, 4);
	if (code)
	    goto done;
	/* Dump an unused long here in case we would like to use the space
	 * someday */
	if (code = vols_DumpLong(asyncP, -1))
	    goto done;
    }

    /* now, we iterate through all of the files in the volume, either dumping
     * them, or indicating that they've been nuked.  If this is an incremental
     * dump and the file hasn't been modified since the appropriate version,
     * then if it exists, we don't dump it.
     */
    firstDeletedIx = VOL_ROOTINO;
    /* Loop invariant: [firstDeletedIx..i) should be deleted, unless we ``goto done''. */
    for (i = VOL_ROOTINO;; ++i) {
	delFlag = 0;
	code = VOL_SCAN(fsetDesc, i);
	if (code) {
	    /* something went wrong, but it could just be an EOF */
	    if (code == VOL_ERR_EOF || code == VOL_ERR_EOW) {
		code = 0;
		break;
	    } else if (code == VOL_ERR_DELETED || code == ENOENT) {
		delFlag = 1;
	    } else {
		if (verbose) dce_svc_printf(FTL_S_DO_SCAN_ERR_MSG, i, code);
		goto done;
		/* continue; */	/* XXXXXXXXXXXXXXXXXXXXXXXXXXX */
	    }
	}
	if (!code) {
	    /* 
	     * Next, stat the file so we can compare the date.  This may also return
	     * the EOF indication, for some volume types (implementations).
	     */
	    if (code = VOL_GETATTR(fsetDesc, &attr)) {
		if (code == VOL_ERR_EOF || code == VOL_ERR_EOW) {
		    code = 0;
		    break;
		} else if (code == VOL_ERR_DELETED || code == ENOENT || code == ENOTTY) {
		    delFlag = 1;
		} else {
		    if (verbose) dce_svc_printf(FTL_S_DO_GETATTR_ERR_MSG, i, code);
		    goto done;
		}
	    }
	}
	/* now, do those version comparisons */
	/* assert(firstDeletedIx <= i); */
	if (delFlag) {
	    /* Just let firstDeletedIx get further away from the value of i. */
	} else {
	    /* 
	     * Otherwise, we dump the file itself, perhaps 
	     */
	    /* This index may have been preceded by deleted indices. */
	    for (delIx = firstDeletedIx; delIx < i; ++delIx) {
		if (code = vols_DumpShortOpcode(asyncP,
						VOLS_DUMP_HEADER_NOVNODE, 8))
		    goto done;
		/* fake file ID for deleted file as slot and 0 */
		AFS_hset64(thyper, delIx, 0);
		code = vols_DumpHyper(asyncP, &thyper);
		if (code)
		    goto done;
	    }
	    /* Now, make [firstDeletedIx..i) be empty this time and next. */
	    firstDeletedIx = i+1;
	    /* First check for out-of-sequence vol versions */
	    fileOrigVV = attr.xvattr.volVersion;
	    if (AFS_hcmp(attr.xvattr.volVersion, volStatus.vol_dy.volversion) > 0) {
		if (verbose) {
		    dce_svc_printf(FTL_S_DO_DUMP_VOL_MSG,
				   AFS_HGETBOTH(volStatus.vol_st.volId), i,
				   AFS_HGETBOTH(attr.xvattr.volVersion),
				   AFS_HGETBOTH(volStatus.vol_dy.volversion));
		}
		ftu_svc_LogPrintf(ftl_s_do_dump_vol_id,
				  AFS_HGETBOTH(volStatus.vol_st.volId), i,
				  AFS_HGETBOTH(attr.xvattr.volVersion),
				  AFS_HGETBOTH(volStatus.vol_dy.volversion));
	    }
	    if (verbose)
		dce_svc_printf(FTL_S_DO_DUMP_FILE_VOL_MSG,
			       AFS_HGETBOTH(volStatus.vol_st.volId), i,
			       AFS_HGETBOTH(attr.xvattr.volVersion),
			       AFS_HGETBOTH(volStatus.vol_dy.volversion));
	    /* Decide whether to dump this file. */
	    switch (dumpCriteriaP->dc_type) {
		case FTU_DUMP_TYPE_DATE_WITH_DIRS:
		    /* dump files dated newer than the cutoff */
		    /* but always dump directories */
		    dumpFlag = 0;
		    if (attr.vattr.va_type == VDIR) dumpFlag = 1;
		    if (attr.xvattr.serverModTime.sec >=
			dumpCriteriaP->dc_date.sec) dumpFlag = 1;
		    break;
		case FTU_DUMP_TYPE_VERS_WITH_DIRS:
		    /* dump files with versions later than the cutoff */
		    /* but always dump directories */
		    dumpFlag = 0;
		    if (attr.vattr.va_type == VDIR) dumpFlag = 1;
		    if (AFS_hiszero(attr.xvattr.volVersion)) dumpFlag = 1;
		    if (AFS_hcmp(attr.xvattr.volVersion,
			     dumpCriteriaP->dc_version) > 0) dumpFlag = 1;
		    break;
		case FTU_DUMP_TYPE_DATE:
		    /* dump files dated newer than the cutoff */
		    dumpFlag = 0;
		    if (attr.xvattr.serverModTime.sec >=
			dumpCriteriaP->dc_date.sec) dumpFlag = 1;
		    break;
		case FTU_DUMP_TYPE_VERS:
		    /* dump files with versions later than the cutoff */
		    dumpFlag = 0;
		    if (AFS_hiszero(attr.xvattr.volVersion)) dumpFlag = 1;
		    if (AFS_hcmp(attr.xvattr.volVersion,
			     dumpCriteriaP->dc_version) > 0) dumpFlag = 1;
		    break;
		case FTU_DUMP_TYPE_FULL:
		default:
		    /* always dump everything */
		    dumpFlag = 1;
		    break;
	    }
	    if (dumpFlag) {
		/* this file was modified later than the incremental date,
		 * or this is not an incremental dump.  Thus, dump the file.
		 */
		if (code = vols_DumpVnode(asyncP, bufP, fsetDesc, i, &attr,
					  aggrType, &ht))
		    goto done;
		if (verbose) {
		    xcode = VOL_GETATTR(fsetDesc, &attr);	/* second try */
		    if (xcode != 0) {
			dce_svc_printf(FTL_S_DO_DUMP_ERR_MSG,
				       AFS_HGETBOTH(volStatus.vol_st.volId), i,
				       xcode);
		    } else {
			if (!AFS_hsame(fileOrigVV, attr.xvattr.volVersion))
			    dce_svc_printf(FTL_S_DO_DUMP_VV_CHANGED_MSG,
			        AFS_HGETBOTH(volStatus.vol_st.volId), i,
				AFS_HGETBOTH(fileOrigVV),
				AFS_HGETBOTH(attr.xvattr.volVersion));
			if (AFS_hcmp(maxPreFileVV, fileOrigVV) < 0)
			    maxPreFileVV = fileOrigVV;
			if (AFS_hcmp(maxPostFileVV, attr.xvattr.volVersion) < 0)
			    maxPostFileVV = attr.xvattr.volVersion;
		    }
		}
	    }
	}
    }
    /* Dump a clear-to-end-of-volume indication. */
    if (code = vols_DumpShortOpcode(asyncP,
				     VOLS_DUMP_HEADER_ENDVNODES, 8))
	goto done;
    /* fake file ID for deleted file as slot and 0 */
    AFS_hset64(thyper, firstDeletedIx, 0);
    code = vols_DumpHyper(asyncP, &thyper);
    if (code)
	goto done;
    /* Get a fresh copy of the volume's volume version number, in case it went up */
    if (code = VOL_GETSTATUS(fsetDesc, &volStatus))
	goto done;
    if (!AFS_hsame(origVV, volStatus.vol_dy.volversion)) {
	/* dump the new volume version number */
	if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_VOLVERSION, 8))
	    goto done;
	if (code = vols_DumpHyper(asyncP, &volStatus.vol_dy.volversion))
	    goto done;
	if (verbose)
	    dce_svc_printf(FTL_S_DO_DUMP_CURR_VV_MSG,
			   AFS_HGETBOTH(volStatus.vol_st.volId),
			   AFS_HGETBOTH(origVV),
			   AFS_HGETBOTH(volStatus.vol_dy.volversion));
    }
    if (verbose) {
	if (!AFS_hsame(origVV, volStatus.vol_dy.volversion)
	    || !AFS_hsame(maxPreFileVV, maxPostFileVV)
	    || AFS_hcmp(origVV, maxPostFileVV) < 0) {
	    dce_svc_printf(FTL_S_DO_DUMP_ORIG_VV_MSG,
			   AFS_HGETBOTH(volStatus.vol_st.volId),
			   AFS_HGETBOTH(origVV),
			   AFS_HGETBOTH(maxPreFileVV));
	    dce_svc_printf(FTL_S_DO_MAX_VV_MSG, AFS_HGETBOTH(maxPostFileVV),
			   AFS_HGETBOTH(volStatus.vol_dy.volversion));
	}
    }
    /* if we made it here, we're all done with the vnodes, so just write out
     * the volume dump trailer and we're done.
     */
    if (code = vols_DumpShortOpcode(asyncP, VOLS_DUMP_HEADER_END, 4))
	goto done;
    temp = VOLS_DUMP_HEADER_ENDMAGIC;
    if (code = vols_DumpLong(asyncP, (u_long) temp))
	goto done;

done:;
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	    sprintf(excBuff, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excBuff;
	}
	ftu_svc_LogPrintf(ftl_s_do_dump_exception, excNameP);
	icl_Trace1(_ftu_iclSetP, FTU_TRACE_VOLS_DUMP_VOLUME_EXCEPTION,
		   ICL_TYPE_STRING, excNameP);
	code = EPIPE;
    } ENDTRY

    /* terminate the async output queue */
    if ( asyncP ) {
	/* send any buffered data */
	if ( asyncP->callerState != NULL ) {
	    ((asyncElement_t *)asyncP->callerState)->last = 1;
	    relAsyncElement(asyncP, (asyncElement_t *)asyncP->callerState);
	}
	/*
	 * check the error codes from the queued writes and
	 * shutdown the output queue
	 */
	do {
	    elemP = getAsyncElement(asyncP);
	    if ( elemP != NULL ) {
		if ( !code && elemP->error ) code = elemP->error;
		elemP->count = -1;
		elemP->last = 1;
		relAsyncElement(asyncP, elemP);
	    }
	} while ( elemP != NULL);
	termAsyncQueue(asyncP);
    }

    /* terminate the keep-alive thread */
    if (kaBuf) {
	lock_ObtainWrite(&kaBuf->lock);
	kaBuf->term = 1;
	osi_Wakeup(kaBuf);
	while (!kaBuf->done) {
	    osi_SleepW(kaBuf, &kaBuf->lock);
	    lock_ObtainWrite(&kaBuf->lock);
	}
	lock_ReleaseWrite(&kaBuf->lock);
	osi_Free(kaBuf, sizeof(vols_kaDump_t));
    }

    /* free the data buffer */
    if ( bufP ) {
	osi_Free(bufP,DUMP_DIRSIZE+OSI_MAXNAMLEN+1);
    }

    /* Reclaim storage used by ACL hash table. */
    for (i = 0; i < AHTSIZE; ++i) {
	for (hep = ht.Tbl[i]; hep != NULL; hep = nhep) {
	    nhep = hep->next;
	    osi_Free(hep->aclp, hep->aclp->dfs_acl_len +
		     sizeof(hep->aclp->dfs_acl_len));
	    osi_Free(hep, sizeof(struct aclHashEnt));
	}
    }

    /* clean up and return code */
    return code;
}
