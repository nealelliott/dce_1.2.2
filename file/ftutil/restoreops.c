/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <sys/types.h>

#include <dcedfs/stds.h>
#include <dcedfs/common_data.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/syscall.h>
#include <dcedfs/pipe.h>
#include <dcedfs/async_queue.h>
#include <dcedfs/aclint.h>
#include <dcedfs/vol_init.h>
#include <dcedfs/xvfs_vnode.h>

#include <ftutil.h>
#include <ftu_dumpops.h>
#include <ftu_private.h>
#include <ftu_syscalls.h>
#include <ftu_trace.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftlmac.h>
#include <dfsftlsvc.h>
#include <dfsftlmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/ftutil/RCS/restoreops.c,v 1.56 1996/06/13 20:26:51 andi Exp $")

static int verbose = 0;

#define TSIZE	512			/* bytes in skipping unrecognized opcodes */

/*
 * async buffer queue definitions
 */
#define RESTORE_BUFCOUNT	3		/* number async buffers */
#define RESTORE_BUFSIZE		(512*1024)	/* async buffer size */

/*
 * restore data buffer definitions. Data buffers are used in
 * vols_RestoreDir to store the directory data and name strings,
 * so we need to make sure we allocate enough space
 */
#define RESTORE_DIRSIZE		(8*1024)	/* directory buffer size */
#define RESTORE_DATASIZE	(64*1024)	/* data buffer size */

/* async pipe I/O routine */

PRIVATE void asyncPipeRead(elemP, argP)
asyncElement_t * volatile elemP;
void *argP;
{
    afsPipe_t * volatile pipeP = (afsPipe_t *)argP;
    char *excNameP;
    char excBuff[50];
    extern char * ExceptionToString();

    TRY {
        elemP->count = POP_READ(pipeP, elemP->data, elemP->size);
	if (elemP->count != elemP->size) {
	    POP_STAT(pipeP, PFL_READ, &elemP->error);
	    elemP->last = 1;
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
    elemP->offset = 0;
    return;
}

/* read the next chunk of data using the async queue */
PRIVATE long vols_RestoreBytes(asyncP, bufp, len)
    asyncQueue_t *asyncP;
    char *bufp;
    long len;
{
    long cnt = 0;
    long error = 0;
    long tlen;
    asyncElement_t *elemP;

    while (cnt < len && error == 0) {
	/*
	 * The current queue element is kept in the callerState field.
	 * If there is no current element, get one from the queue.
	 */
        elemP = (asyncElement_t *)asyncP->callerState;
        if (elemP == NULL) {
	    /* get an input queue element */
	    elemP = getAsyncElement(asyncP);
	    if (elemP == NULL) {
		return(EPIPE);
	    }
	    asyncP->callerState = (void *)elemP;
	    if (elemP->error) return(elemP->error);
	}

	/*
	 * Copy as much as possible into the current buffer.
	 */
	tlen = elemP->count - elemP->offset;
	if (tlen > len - cnt) tlen = len - cnt;
	bcopy(elemP->data + elemP->offset, bufp + cnt, tlen);
	elemP->offset = elemP->offset + tlen;
	cnt = cnt + tlen;

	/*
	 * if the buffer is empty, put it onto the outpue queue
	 */
	if (elemP->offset == elemP->count) {
	    relAsyncElement(asyncP, elemP);
	    asyncP->callerState = NULL;
	}
    }
    return 0;
}

#define TB_HASH_SIZE 256

/* This function is called when an unrecognized, but still legal, field type
 * is encountered by a dump.  The idea is that we can skip over the data we
 * don't understand, and thus allow a reasonable level of upwards-compatability
 * when adding new status fields to our dump format over time.
 */
static long vols_SkipMysterious(asyncP, opcodep)
    asyncQueue_t *asyncP;
    struct vols_opcode *opcodep; 
{
    long code;
    char buffer[TSIZE];
    long tsize, total;

    if ((opcodep->opcode & 0xf000) == 0xf000)
	total = opcodep->f.l.length;
    else
	total = opcodep->f.s.length;
    code = 0;
    while (total > 0) {
	tsize = (total > TSIZE ? TSIZE : total);
	code = vols_RestoreBytes(asyncP, (char *)buffer, tsize);
	if (code)
	    break;
	total -= tsize;
    }
    return code;
}


/* Read a null-terminated string from the input.  amaxLength gives the max # of
 * bytes we might see, a buffer of this size is allocated if *astringpp is 
 * null. Otherwise *astringpp is assumed to point to a buffer to use to store
 * the string whose size is at least as large as amaxLength. 
 * In other words, a buffer is allocated iff astringpp is null and the call 
 * does not fail.  If a buffer is allocated, it is the caller's responsibility
 * to free it.
 */
static long vols_RestoreString(asyncP, astringpp, amaxLength, nullTerm)
    char **astringpp;
    long amaxLength;
    asyncQueue_t *asyncP;
    int nullTerm;
{
    char tc;
    long code = 0, nread;
    register long totread, charsleft;
    register char *tp;
    char *freeOnErrP;
    
    /* make sure buffer exists; return buffer otherwise */
    if (!(tp = *astringpp)) {
	/* *astringpp is zero, so we allocate a buffer ourselves */
	*astringpp = freeOnErrP = tp = (char *) osi_Alloc(amaxLength == 0 ? 1 : amaxLength);
    } else {
	/* remember not to free the buffer, it was passed into us */
	freeOnErrP = (char *) 0;
    }
    /* some bad format in the dump source, but what can you do? */
    if (amaxLength == 0 && nullTerm != 0) {
	ftu_svc_LogPrintf(ftl_s_ro_zero_length);
	*tp++ = '\0';	/* read it as a null string */
	return 0;
    }
    
    totread = 0;
    charsleft = amaxLength;
    if (nullTerm != 0) {
	for (;;) {
	    if (charsleft-- <= 0) {
		ftu_svc_LogPrintf(ftl_s_ro_no_null, amaxLength);
		code = FTU_E_BAD_DUMP; 	/* not null-terminated */
		break;
	    }
	    code = vols_RestoreBytes(asyncP, (char *)&tc, 1);
	    if (code)
		goto done;
	    ++totread;
	    *tp++ = tc;
	    if (!tc) break;	/* null-termination, all done */
	}
	if (totread != amaxLength) {
	    ftu_svc_LogPrintf(ftl_s_ro_restorestring_expected, totread, amaxLength);
	    /* code = FTU_E_BAD_DUMP; */
	}
    } else {
	/* just read it straight without regard for NUL characters. */
	code = vols_RestoreBytes(asyncP, (char *)tp, (unsigned)charsleft);
	if (code)
	    goto done;
    }

    /* come here if all done.  If code is set, something went wrong, and we
     * free any space we may have allocated.
     * Otherwise, we return the good code.
     */
  done:
    if (code) {
	/* an error occurred */
	if (freeOnErrP) osi_Free(freeOnErrP,
				 (amaxLength == 0 ? 1 : amaxLength));
	*astringpp = (char *) 0;
    }
    return code;
}


static long vols_RestoreInt16(asyncP, ashortp, alen)
    asyncQueue_t *asyncP;
    u_short *ashortp;
    u_short alen;
{
    long code;
    unsigned short xshort;

    if (alen != sizeof(u_short)) {
	ftu_svc_LogPrintf(ftl_s_ro_restoreshort_expected, sizeof(u_short), alen);
	return FTU_E_BAD_DUMP;
    }
    code = vols_RestoreBytes(asyncP, (char *) &xshort, sizeof(xshort));
    if (code)
	return code;
    *ashortp = ntohs(xshort);
    return 0;
}


static long vols_RestoreInt32(asyncP, alongp, alen)
    asyncQueue_t *asyncP;
    long *alongp;
    u_short alen;
{
    long code;
    unsigned long xlong;

    if (alen != sizeof(u_long)) {
	ftu_svc_LogPrintf(ftl_s_ro_restorelong_expected, sizeof(u_long), alen);
	return FTU_E_BAD_DUMP;
    }
    code = vols_RestoreBytes(asyncP, (char *) &xlong, sizeof(xlong));
    if (code)
	return code;
    *alongp = (long) ntohl(xlong);
    return 0;
}


/* restore a pair of longs as a afs_hyper_t */
static long vols_RestoreInt64(asyncP, ahp, alen)
    asyncQueue_t *asyncP;
    afs_hyper_t *ahp;
    u_short alen;
{
    register long code;
    long high, low;

    if (alen != (2 * sizeof(unsigned long))) {
	ftu_svc_LogPrintf(ftl_s_ro_restorehyper_expected,
			  2*sizeof(unsigned long), alen);
	return FTU_E_BAD_DUMP;
    }
    if (code = vols_RestoreInt32(asyncP, &high,
				 sizeof(unsigned long)))
	return code;
    code = vols_RestoreInt32(asyncP, &low, sizeof(unsigned long));
    if (code)
	return code;
    AFS_hset64(*ahp, high, low);
    return 0;
}


/* restore a pair of longs as a timeval */
static long vols_RestoreTime(asyncP, atp, alen)
    asyncQueue_t *asyncP;
    afsTimeval *atp;
    u_short alen;
{
    register long code;

    if (alen != (2 * sizeof(unsigned long))) {
	ftu_svc_LogPrintf(ftl_s_ro_time_expected,
			  2*sizeof(unsigned long), alen);
	return FTU_E_BAD_DUMP;
    }
    if (code = vols_RestoreInt32(asyncP, &atp->sec, sizeof(unsigned long)))
	return code;
    code = vols_RestoreInt32(asyncP, &atp->usec, sizeof(unsigned long));
    return code;
}


static long vols_RestoreOpcode(asyncP, opcodep)
  asyncQueue_t *asyncP;
  struct vols_opcode *opcodep;
{
    u_short topcode, hibytes, length;
    long code, lowbytes;

    if (code = vols_RestoreInt16(asyncP, &topcode, sizeof(unsigned short)))
	return code;

    if ((topcode & 0xf000) == 0xf000) {
	/* this is a long-form opcode, with a 48-bit length field encoded
	 * as the next 48 bits in the input stream.  We separate these
	 * into two fields for the convenience of those who want to assume
	 * for now that files are < 2**32 bytes.
	 */
	if (code = vols_RestoreInt16(asyncP, &hibytes, sizeof(unsigned short)))
	    return code;
	opcodep->f.l.high = hibytes;

	if (code = vols_RestoreInt32(asyncP, &lowbytes, sizeof(unsigned long)))
	    return code;
	opcodep->f.l.length = lowbytes;
    } else {
	/* short form opcode: 16 bits of unsigned length */
	if (code = vols_RestoreInt16(asyncP, &length, sizeof(unsigned short)))
	    return code;
	opcodep->f.s.length = length;
    }
    opcodep->opcode = topcode;
    return 0;
}


static long vols_SmashVnode(asyncP, vdesc, alen)
     asyncQueue_t *asyncP; 
     long vdesc;
     u_short alen;
{
    long code, vnode, unique;

    if (alen != (2 * sizeof(unsigned long))) {
	ftu_svc_LogPrintf(ftl_s_ro_vnode_expected,
			  2*sizeof(unsigned long), alen);
	return FTU_E_BAD_DUMP;
    }
    /*
     * Read the Hyper including the vnode/unique for the DELETED vnode
     */
    if (code = vols_RestoreInt32(asyncP, &vnode, sizeof(unsigned long)))
	return code;
    if (code = vols_RestoreInt32(asyncP, &unique, sizeof(unsigned long)))
	return code;
    if (verbose) dce_svc_printf(FTL_S_RO_SM_VNODE_MSG, vnode);

    /* Delete the thing.  Point to the right place, then delete it. */
    code = VOL_SCAN(vdesc, vnode);
    if (code != 0) {
	if (code == ENOENT || code == VOL_ERR_EOF || code == VOL_ERR_EOW) {
	    if (verbose) dce_svc_printf(FTL_S_RO_SM_SCAN_CODE_MSG, vnode, code);
	    return 0;
	} else {
	    if (verbose) dce_svc_printf(FTL_S_RO_SCAN_CODE_MSG, vnode, code);
	    return code;
	}
    }
    code = VOL_DELETE(vdesc);
    if (verbose) dce_svc_printf(FTL_S_RO_SM_CODE_MSG, vnode, code);
    return code;
}


static long vols_SmashAllVnodes(asyncP, vdesc, alen)
     asyncQueue_t *asyncP; 
     long vdesc;
     u_short alen;
{
    long code, vnode, unique;
    register long vIndex;

    if (alen != (2 * sizeof(unsigned long))) {
	ftu_svc_LogPrintf(ftl_s_ro_allvnode_expected,
			  2*sizeof(unsigned long), alen);
	return FTU_E_BAD_DUMP;
    }
    /*
     * Read the index for the start of the DELETED vnodes
     */
    if (code = vols_RestoreInt32(asyncP, &vnode, sizeof(unsigned long)))
	return code;
    if (code = vols_RestoreInt32(asyncP, &unique, sizeof(unsigned long)))
	return code;
    if (verbose) dce_svc_printf(FTL_S_RO_SAV_MSG, vnode);

    for (vIndex = vnode; ; ++vIndex) {
	/* Delete the thing.  Point to the right place, then delete it. */
	code = VOL_SCAN(vdesc, vIndex);
	if (code == VOL_ERR_EOW || code == VOL_ERR_EOF) {
	    code = 0;	/* we're done */
	    break;
	} else if (code == ENOENT || code == VOL_ERR_DELETED) {
	    /* if (verbose) dce_svc_printf(FTL_S_RO_SAV_SCAN_CODE_MSG, vIndex, code); */
	    code = 0;
	    continue;
	} else if (code != 0) {
	    if (verbose) dce_svc_printf(FTL_S_RO_SAV_CODE_MSG, vIndex, code);
	    break;	/* return with failure indication */
	}
	code = VOL_DELETE(vdesc);
	if (verbose) dce_svc_printf(FTL_S_RO_VDESC_CODE_MSG, vIndex, code);
	if (code) break;
    }
    return code;
}



/* 
 * Read the data bytes of a vnode into a file.  File's volume is
 * described by vdesc. Opcodep's length field tells us how many bytes
 * are to be found in the data stream. Callp represents the data
 * stream (although it is an rx call structure). 
 */
static long vols_RestoreData(asyncP, bufP, vdesc, opcodep)
     asyncQueue_t *asyncP; 
     char *bufP;
     long vdesc;
     register struct vols_opcode *opcodep;
{
    long code, temp, tsize;
    long bytesToRead;
    afs_hyper_t filePos, newFilePos;
    struct io_rwHyperDesc iodesc;

    if (opcodep->f.l.length < (2*sizeof(unsigned long))) {
	ftu_svc_LogPrintf(ftl_s_ro_restore_expected, 2*sizeof(unsigned long), opcodep->f.l.length);
	return FTU_E_BAD_DUMP;
    }
    if (code = vols_RestoreInt64(asyncP, &filePos, sizeof(filePos)))
	return code;

    /* opcodep->l.length tells us how many bytes we should read in. */
    bytesToRead = opcodep->f.l.length - (2*sizeof(long));
    iodesc.bufferp = bufP;
    /* TODO: replace this code */
    if (verbose)
	dce_svc_printf(FTL_S_RO_RD_BPOS_MSG,
		       AFS_HGETBOTH(filePos), bytesToRead);
    while (bytesToRead > 0) {
	tsize = MIN(bytesToRead, RESTORE_DATASIZE);
	code = vols_RestoreBytes(asyncP, bufP, tsize);
	if (code)
	    break;
	if (verbose)
	    dce_svc_printf(FTL_S_RO_RD_W_MSG, AFS_HGETBOTH(filePos), tsize);

	newFilePos = filePos;
	AFS_hadd32(newFilePos, tsize);
	if (AFS_hcmp(newFilePos, osi_maxFileServer) >= 0) {
	    code = EFBIG;
	    break;
	}

	/* otherwise, write the buffer to the disk file */
	iodesc.position = filePos;
	iodesc.length = tsize;
	if (code = VOL_WRITE(vdesc, &iodesc))
	    break;
	filePos = newFilePos;		/* wrote the bytes correctly */
	bytesToRead -= tsize;		/* and thus have fewer to go */
    }
    if (verbose) dce_svc_printf(FTL_S_RO_RD_E_MSG, code);
    return code;
}


/* 
 * Read the directory data
 */
static long vols_RestoreDir(asyncP, bufP, vdesc, opcodep, vnode,
			    preserveOffsets)
     asyncQueue_t *asyncP; 
     char *bufP;
     long vdesc;
     register struct vols_opcode *opcodep;
     long vnode;
     int preserveOffsets;
{
    long code;
    register char *tBufP, *tEndP;
    u_short version;
    struct vol_dirent dirent;
    int32 offset, vnodeNum;
    u_long thisTag;
    char *name;
    afs_hyper_t hOffset;
    unsigned nameLen;
    struct io_rwHyperDesc iodesc;
    u_long numEntries = 0;
    
    if (opcodep->f.l.length !=  sizeof(u_int16)) {
	ftu_svc_LogPrintf(ftl_s_ro_restoredir_expected, sizeof(u_int16), opcodep->f.l.length);
	return FTU_E_BAD_DUMP;
    }

    if (code = vols_RestoreInt16(asyncP, &version, sizeof(unsigned short)))
	return code;

    switch (version) {
      case VOLS_INITIAL_DIRDUMP_VERSION:
	break;

      default:
	ftu_svc_LogPrintf(ftl_s_ro_restore_unkown, (unsigned)version);
	return FTU_E_BAD_DUMP;
    }

    tBufP = bufP;
    tEndP = bufP + RESTORE_DIRSIZE;
    name = bufP + RESTORE_DIRSIZE;
    bzero((char *)&iodesc, sizeof(iodesc));
    iodesc.bufferp = bufP;
    iodesc.length = RESTORE_DIRSIZE;

    if (code = vols_RestoreOpcode(asyncP, opcodep))
	goto done;
    while (opcodep->opcode == VOLS_DUMP_VNODE_DIR_ENTRY) {
	if (opcodep->f.s.length <= (sizeof(afs_hyper_t) + 2*sizeof(u_int32))) {
	    ftu_svc_LogPrintf(ftl_s_ro_restoredir_atexpected,
			      (sizeof(afs_hyper_t) + 2*sizeof(u_int32) + 1),
			      opcodep->f.s.length);
	    code = FTU_E_BAD_DUMP;
	    goto done;
	}

	if (code = vols_RestoreInt64(asyncP, &hOffset, sizeof(hOffset)))
	    goto done;
	offset = AFS_hgetlo(hOffset);

	if (code = vols_RestoreInt32(asyncP, &vnodeNum, sizeof(vnodeNum)))
	    goto done;

	if (code = vols_RestoreInt32(asyncP, &thisTag, sizeof(thisTag)))
	    goto done;

	nameLen = opcodep->f.s.length - (sizeof(afs_hyper_t) + 2*sizeof(u_int32));
	if (nameLen > OSI_MAXNAMLEN + 1) {
	    ftu_svc_LogPrintf(ftl_s_ro_restoredir_dumpname, nameLen - 1, OSI_MAXNAMLEN);
	    code = EINVAL;
	    goto done;
	}
	if (code = vols_RestoreString(asyncP, &name, nameLen, 1))
	    goto done;
	
	VOL_FILL_DIRENT(dirent, offset, vnodeNum, thisTag, name);
	
	if (tBufP + dirent.reclen >= tEndP) {
	    if (code = VOL_APPENDDIR(vdesc, &iodesc, numEntries,
				     preserveOffsets))
		goto done;
	    tBufP = bufP;
	    numEntries = 0;
	}
	
	bcopy((char *)&dirent, tBufP, dirent.reclen);
	tBufP += dirent.reclen;
	numEntries++;

	if (code = vols_RestoreOpcode(asyncP, opcodep))
	    goto done;
    }

    if (opcodep->opcode != VOLS_DUMP_VNODE_DIR_END) {
	code = FTU_E_BAD_DUMP;
	goto done;
    }

    if (numEntries > 0)
	code = VOL_APPENDDIR(vdesc, &iodesc, numEntries, preserveOffsets);

done:
    return code;
}

/*
 * Restore an individual vnode from a dump.  We assume that
 * vols_Restore has already read the vnode opcode for us (that's how
 * it knew to call us). We assume that nothing past the opcode has
 * been read, that is, the parameters to VOLS_DUMP_HEADER_VNODE are
 * still in the pipeline. We restore into the appropriate vnode, and
 * then return.  We change the vnode's status right at the end, after
 * filling in the data.
 */
static long vols_RestoreVnode(asyncP, bufP, vdesc, alen, sameFSType,
			      hasSetAclP, restoreBaseTypeP, flags)
    asyncQueue_t *asyncP;
    char *bufP;
    long vdesc;
    u_short alen;
    int sameFSType;
    int *hasSetAclP;
    afs_hyper_t *restoreBaseTypeP;
    unsigned long flags;
{
    long hitEOF, vnodeType, unixModeBits, author, owner, group, linkCount;
    long clientBits, vnodeRdev, vnode, unique, code, xcode;
    struct vols_opcode topcode;
    struct xvfs_attr fattr, readattrs;
    afsTimeval serverModTime, accessTime, changeTime, modTime;
    afs_hyper_t dataVersion, volVersion, fileSize;
    afs_hyper_t h0;
    static long aclTypes[] = {
	VNX_ACL_REGULAR_ACL,
	VNX_ACL_DEFAULT_ACL,
	VNX_ACL_INITIAL_ACL
    };
#define numAclTypes (sizeof(aclTypes) / sizeof(aclTypes[0]))
    char *aclStrings[numAclTypes];
    unsigned long aclStrLens[numAclTypes];
    afs_hyper_t aclSrcVUs[numAclTypes];
    long aclSrcWhiches[numAclTypes];
#define src_noAcl 0
#define src_strAcl 1
#define src_srcAcl 2
    long aclSrcs[numAclTypes];
    unsigned long whichAcl, whichAclSrc, partLen;
    afs_hyper_t aclVUDum;
    struct dfs_acl oneAcl;
    char *aclStrDum;
    register int i;
    int preserveOffsets;

    /* initialize important locals */
    bzero((char *)aclStrings, sizeof(aclStrings));
    bzero((char *)aclStrLens, sizeof(aclStrLens));
    bzero((char *)aclSrcs, sizeof(aclSrcs));
    clientBits = 0xffffffff; /* give it a value if there's not one coming in */

    /* read the remainder of the basic vnode fields associated with the
     * VOLS_DUMP_HEADER_VNODE opcode.
     */
    if (alen != (2 * sizeof(unsigned long))) {
	ftu_svc_LogPrintf(ftl_s_ro_restore_vnode_expected,
			  2*sizeof(unsigned long), alen);
	return FTU_E_BAD_DUMP;
    }
    if (code = vols_RestoreInt32(asyncP, &vnode, sizeof(unsigned long)))
	return code;
    if (code = vols_RestoreInt32(asyncP, &unique, sizeof(unsigned long)))
	return code;

    /* 
     * Now, start scanning dump, handling all of the status information
     * at the end, and reading the data directly in.
     */
    hitEOF = 0;
    while (!hitEOF) {
	if (code = vols_RestoreOpcode(asyncP, &topcode))
	    break;	/* done early, cleanup */
	switch (topcode.opcode) {
	  case VOLS_DUMP_VNODE_TYPE:
	    code = vols_RestoreInt32(asyncP, &vnodeType, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_RDEV:
	    code = vols_RestoreInt32(asyncP, &vnodeRdev, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_LINKCOUNT:
	    code = vols_RestoreInt32(asyncP, &linkCount, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_DATAVERSION:
	    code = vols_RestoreInt64(asyncP, &dataVersion, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_VOLVERSION:
	    code = vols_RestoreInt64(asyncP, &volVersion, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_ACCESSTIME:
	    code = vols_RestoreTime(asyncP, &accessTime, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_MODTIME:
	    code = vols_RestoreTime(asyncP, &modTime, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_CHANGETIME:
	    code = vols_RestoreTime(asyncP, &changeTime, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_SERVERMODTIME:
	    code = vols_RestoreTime(asyncP, &serverModTime, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_GROUP:
	    code = vols_RestoreInt32(asyncP, &group, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_AUTHOR:
	    code = vols_RestoreInt32(asyncP, &author, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_OWNER:
	    code = vols_RestoreInt32(asyncP, &owner, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_UNIXMODE:
	    code = vols_RestoreInt32(asyncP, &unixModeBits, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_CLIENTATTRS:
	    code = vols_RestoreInt32(asyncP, &clientBits, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_ACL:
	    code = vols_RestoreString(asyncP, &aclStrings[0],
				      topcode.f.s.length, 0);
	    aclStrLens[0] = topcode.f.s.length;
	    aclSrcs[0] = src_strAcl;
	    break;
	  case VOLS_DUMP_VNODE_SIZE:
	    code = vols_RestoreInt64(asyncP, &fileSize, topcode.f.s.length);
	    break;
	  case VOLS_DUMP_VNODE_ACL_INLINE:
	    partLen = topcode.f.s.length;
	    if (partLen < sizeof(unsigned long)) {
		ftu_svc_LogPrintf(ftl_s_ro_restoreacl_expected,
				  sizeof(unsigned long), topcode.f.s.length);
		code = FTU_E_BAD_DUMP; 
		break;
	    }
	    if (code = vols_RestoreInt32(asyncP, &whichAcl,
					sizeof(unsigned long)))
		break;
	    partLen -= sizeof(unsigned long);
	    aclStrDum = 0;
	    if (code = vols_RestoreString(asyncP, &aclStrDum, partLen, 0))
		break;
	    for (i = 0; i < numAclTypes; ++i) {
		if (whichAcl == aclTypes[i]) break;
	    }
	    if (i < numAclTypes) {
		aclStrings[i] = aclStrDum;	/* transfer ownership */
		aclStrLens[i] = partLen;
		aclSrcs[i] = src_strAcl;	/* say where to find the ACL */
	    } else {
		/* can't use it! */
		osi_Free(aclStrDum, (partLen ? partLen : 1)); 
	    }
	    break;
	  case VOLS_DUMP_VNODE_ACL_COPY:
	    partLen = 2*sizeof(unsigned long) + sizeof(afs_hyper_t);
	    if (topcode.f.s.length != partLen) {
		ftu_svc_LogPrintf(ftl_s_ro_restoreacl_copy_expected,
				  partLen, topcode.f.s.length);
		code = FTU_E_BAD_DUMP; 
		break;
	    }
	    if (code = vols_RestoreInt32(asyncP, &whichAcl,
					sizeof(unsigned long)))
		break;
	    if (code = vols_RestoreInt32(asyncP, &whichAclSrc,
					sizeof(unsigned long)))
		break;
	    if (code = vols_RestoreInt64(asyncP, &aclVUDum,
					 sizeof(afs_hyper_t)))
		break;
	    for (i = 0; i < numAclTypes; ++i) {
		if (whichAcl == aclTypes[i]) break;
	    }
	    if (i < numAclTypes) {
		aclSrcVUs[i] = aclVUDum;
		aclSrcWhiches[i] = whichAclSrc;
		aclSrcs[i] = src_srcAcl;	/* say where to find the ACL */
	    }
	    /* else don't know anything about this ACL. */
	    break;
	  case VOLS_DUMP_VNODE_DATA:
	    hitEOF = 1;
	    break;
	  case VOLS_DUMP_VNODE_DIR:
	    hitEOF = 1;
	    break;
	  case VOLS_DUMP_VNODE_EOF:
	    hitEOF = 1;
	    break;
	  default:
	    code = vols_SkipMysterious(asyncP, &topcode);
	    break;
	}
	if (code) 
	    break;
    }	/* while !hitEOF */

    /* if here, either code == 0 and we're all done, or we got an error
     * and code tells us what went wrong.  
     */
    if (code) goto done;

    if (verbose) dce_svc_printf(FTL_S_RO_VNODE_SIZE_MSG,
				vnode, AFS_HGETBOTH(fileSize));
    /* 
     * If we make it here, we're about done.  We restored the data to the right
     * file, but still have to do a store status of all of the status
     * information that we've seen.  We fill in the appropriate block and make
     * the appropriate volsys call.
     */

    bzero((char *)&fattr, sizeof(fattr));
    osi_vattr_null(&fattr.vattr);

    fattr.vattr.va_type = afsObject_to_vattr_type (vnodeType);
    if ((fattr.vattr.va_type == VBLK)
	|| (fattr.vattr.va_type == VCHR))       /* get the device number*/
    {
	fattr.vattr.va_rdev = (dev_t) vnodeRdev;
	if (fattr.vattr.va_rdev != vnodeRdev)   /* different sizes for dev_t?*/
	{
	    code = EIO;
	    goto done;
	}
    }
    
    fattr.vattr.va_mode = unixModeBits;	/* 12 bits: not including type bits */
    fattr.vattr.va_uid = owner;
    fattr.vattr.va_gid = group;
    fattr.vattr.va_nlink = linkCount;
    fattr.vattr.va_size = AFS_hgetlo(fileSize);

    if (accessTime.usec >= 1000000) {
	ftu_svc_LogPrintf(ftl_s_ro_restorevnode_overflow, accessTime.sec, accessTime.usec);
	accessTime.usec = 0;
    }
    if (changeTime.usec >= 1000000) {
	ftu_svc_LogPrintf(ftl_s_ro_restorevnode_change, changeTime.sec, changeTime.usec);
	changeTime.usec = 0;
    }
    if (modTime.usec >= 1000000) {
	ftu_svc_LogPrintf(ftl_s_ro_restorevnode_modtime, modTime.sec, modTime.usec);
	modTime.usec = 0;
    }
    osi_SubFromUTime(fattr.vattr.va_atime,accessTime);
    osi_SubFromUTime(fattr.vattr.va_ctime,changeTime);
    osi_SubFromUTime(fattr.vattr.va_mtime,modTime);
    /*
     * Set the "additional" xattr attributes
     */
    fattr.xvattr.dataVersion = dataVersion;
    fattr.xvattr.volVersion = volVersion;
    fattr.xvattr.author = author;
    fattr.xvattr.callerAccess = 0xffffffff;	/* ignored */
    fattr.xvattr.anonAccess = 0xffffffff;	/* ignored by SetAttr */
    fattr.xvattr.parentVnode = 0;		/* ignored by setattr */
    fattr.xvattr.parentUnique = 0;		/* ignored by setattr */
    fattr.xvattr.serverModTime = serverModTime;
    fattr.xvattr.clientOnlyAttrs = clientBits;	/* copy 'em in */

    /* Set unique for real now (in Episode at least). */
    AFS_hset64(fattr.xvattr.fileID, 0, unique); 

    /* Get a handle on the vnode that we're dealing with */
    code = VOL_SCAN(vdesc, vnode);
    if (code != 0 && code != ENOENT && code != VOL_ERR_EOF) {
	goto done;
    }

    /*
     * There are a number of attributes that cannot changed with
     * VOL_SETATTR namely :
     *                              vattr.va_uid
     *                              vattr.va_gid
     *                              vattr.va_type
     *                              xvattr.fileID (the uniquifier, in low part)
     *                              vattr.va_nlink????
     *
     * In the PAST, because of this always delete the old inode and
     * create a new one with the desired attributes.
     *
     * NOW we rely on filesystem dependent VOL_CREATE doing the right
     * thing - if it cannot change certain fields, it needs to delete
     * the file before creating it.  Also the file needs to be truncated 
     * if it already exists.
     */
    
    code = VOL_CREATE(vdesc, vnode, &fattr);
    if (code)
	goto done;

    /*
     * Unfortunately we need to collect the actual data at the end (it's
     * dumped at the end anyway) since in order to call VOL_CREATE above
     * we need to get some valid vnode attributes. Also VOL_CREATE needs to 
     * truncate the file to zero length, if the file already exists.
     */

    if (topcode.opcode == VOLS_DUMP_VNODE_DATA) {
	if (vnodeType == VDIR && !sameFSType) {
	    code = FTU_E_BAD_DUMP;
	    goto done;
	}
	do {
	    if (code = vols_RestoreData(asyncP, bufP, vdesc, &topcode))
		goto done;
	    if (code = vols_RestoreOpcode(asyncP, &topcode))
		goto done;
	} while (topcode.opcode == VOLS_DUMP_VNODE_DATA);
    } else if (topcode.opcode == VOLS_DUMP_VNODE_DIR){
	/*
	 * Use `sameFSType' as the `preseveOffsets' parameter to
	 * appenddir, since we should be able to preserve offsets when
	 * restoring into the same type of filesystem.
	 */
	if (flags & FTU_RESTORE_FORCE_PRESERVE)
	    preserveOffsets = 1;
	else
	    preserveOffsets = sameFSType;
	if (code = vols_RestoreDir(asyncP, bufP, vdesc, &topcode, vnode,
				   preserveOffsets))
	    goto done;
	if (code = vols_RestoreOpcode(asyncP, &topcode))
	    goto done;
    }
    if (topcode.opcode != VOLS_DUMP_VNODE_EOF) {
	code = EINVAL;
	goto done;
    }
    /*
     * Now set the vnode's proper attributes
     */
    code = 0;
    if (vnodeType == VDIR) {
	fattr.vattr.va_size = -1;
    }

    /* Set the file's ACLs, if any. Do the SETs first, then the COPYs,
     * in case a COPY refers to one of the ACLs being set. */
    if (*hasSetAclP) {
	for (i = 0; i < numAclTypes; ++i) {
	    if (aclSrcs[i] == src_strAcl) {
		/* Set aclTypes[i] from aclStrings[i] (length aclStrLens[i]) */
		if (aclStrLens[i] > MAXDFSACL) {
		    code = FTU_E_BAD_DUMP; 
		} else {
		    bzero((char *)&oneAcl, sizeof(oneAcl));
		    oneAcl.dfs_acl_len = aclStrLens[i];
		    bcopy(aclStrings[i], oneAcl.dfs_acl_val, aclStrLens[i]);
		    code = VOL_SETACL(vdesc, &oneAcl, aclTypes[i], 0);
		    if (code == EDOM || code == ESPIPE) {
			if (code == EDOM)
			    code = FTU_E_RESTORING_BAD_ACL;
			else
			    code = FTU_E_RESTORING_BAD_ACL_ENTRYTYPE;
			ftu_svc_LogPrintf(ftl_s_ro_vols_restore_no_set_acl,
					  aclTypes[i], vnode, code);
			icl_Trace3(_ftu_iclSetP,
				   FTU_TRACE_RESTORE_FAILED_SETACL,
				   ICL_TYPE_LONG, aclTypes[i],
				   ICL_TYPE_LONG, vnode,
				   ICL_TYPE_LONG, code);
		    }
		}
		if (code != 0) break;
	    }
	}
	for (i = 0; i < numAclTypes; ++i) {
	    if (code != 0) break;
	    if (aclSrcs[i] == src_srcAcl) {
		/* Set aclTypes[i] from aclSrcVUs[i] and aclSrcWhiches[i] */
		code = VOL_COPYACL(vdesc, aclTypes[i],
				   AFS_hgethi(aclSrcVUs[i]), aclSrcWhiches[i]);
	    }
	}
	/* Tolerate systems that don't have ACLs even if the source had them.
	 * ...but this should always work on Episode!. */

	/* Someday this hasSetAcl==0 case might return a soft-failure
	 * indication, or the caller can impute it because it's a 
	 * cross-representation restoration. */
	if (code != 0 && !AG_TYPE_SUPPORTS_EFS(AFS_hgethi(*restoreBaseTypeP))) {
	    if (code == EOPNOTSUPP || code == ENOTTY || code == ENOSYS) {
		*hasSetAclP = 0;
		code = 0;
	    }
	}
    }
    if (code == 0) {
	/* We count on this to set the file's copy of the VV. */
	code = VOL_SETATTR(vdesc, &fattr);
	if (verbose) {
	    xcode = VOL_GETATTR(vdesc, &readattrs);
	    if (xcode != 0) {
		dce_svc_printf(FTL_S_RO_CANT_GET_ATTRS_MSG, vnode, unique, xcode);
	    } else {
		if (!AFS_hsame(fattr.xvattr.volVersion,
			   readattrs.xvattr.volVersion)) {
		    dce_svc_printf(FTL_S_RO_RESTORE_SET_VV_MSG, vnode, unique,
				   AFS_HGETBOTH(fattr.xvattr.volVersion),
				   AFS_HGETBOTH(readattrs.xvattr.volVersion));
		}
	    }
	}
    }
done:
    /*
     * XXX Should we also destroy the newly created anode (via VOL_CREATE)
     * in case we got an error later on!!! XXX
     */
    for (i = 0; i < numAclTypes; ++i) {
	if (aclStrings[i]) osi_Free(aclStrings[i],
				    (aclStrLens[i] ? aclStrLens[i] : 1));
    }
    return code;
}

/*
 * Thread to periodally call the keep-alive routine for the restore
 * transaction. Doing this in a thread will assure that we stay 
 * if the restore is blocked waiting for a tape mount and we are unable
 * to transfer data.
 */

#define KA_DELAY 10		/* seconds between keep alive calls */

typedef struct vols_kaRestore {
    long term;
    long done;
    osi_dlock_t lock;
    ftu_kaProc_t kaProc;
    void *kaArg;
} vols_kaRestore_t;

void vols_RestoreKeepAlive(argp, unused)
void *argp;
void *unused;
{
    vols_kaRestore_t *kaBuf = (vols_kaRestore_t *)argp;
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
 * Restore a volume dump (which can be read from the call at
 * dataPipeP) into the volume, already open in write mode Returns 0
 * for success if the volume is restored properly, otherwise returns
 * the error that aborted the restore.  The volume may be in an
 * arbitrarily inconsistent state after a restore failure. The
 * algorithm is to scan the vnodes one by one, zapping those that the
 * dump says should be zapped, and overwritting those that are changed
 * by this dump. 
 */
SHARED long vols_RestoreVolume(fsetDesc, baseTypeP, afsPipeP, restoreFlags,
			       kaProc, kaArg)
     ftu_fsetDesc_t fsetDesc;
     afs_hyper_t *baseTypeP;
     afsPipe_t *afsPipeP;
     unsigned long restoreFlags;
     ftu_kaProc_t kaProc;
     void *kaArg;
{ 
    long temp, i, minQuota, volOwner, maxNodeIx, volUniqueGen;
    afs_hyper_t visLimit, allocLimit;
    afs_hyper_t junkHyper;
    unsigned long codesetTag;
    afsTimeval creationDate, updateDate, accessDate, copyDate;
    register long code, hitEOF;
    unsigned long flags, mask, xcode;
    struct vols_opcode vopcode;
    long uidgen[VOLS_DUMP_NUIDS];
    char *motd;
    char *bufP = NULL;
    int motdLen;
    char tc;	/* for testing EOF */
    long nread;
    afs_hyper_t dumpBaseType;
    struct vol_status vstatus, returnedVstatus;
    afs_hyper_t volVersion, firstVolVersion;
    ftu_dumpCriteria_t dumpCriteria;
    int didStatusInit;
    int sameFSType = 0;
    int seenEndVnodes = 0;
    int hasSetAcl;
    int isInconsistent = 0;
    asyncQueue_t *asyncP = NULL;
    asyncElement_t *elemP = NULL;
    vols_kaRestore_t *kaBuf = NULL;

    /* initialize some locals */
    motd = (char *) 0;	/* so if non-null, we can free it */
    didStatusInit = 0;
    flags = 0;
    volOwner = 0;
    AFS_hzero(visLimit);
    AFS_hzero(allocLimit);
    codesetTag = 0;
    AFS_hzero(firstVolVersion);

    /* start a thread to do the transaction keepalives */
    kaBuf = (vols_kaRestore_t *)osi_Alloc(sizeof(vols_kaRestore_t));
    if (kaBuf == NULL) {
	code = FTU_E_NO_MEMORY;
	goto done;
    }
    lock_Init(&kaBuf->lock);
    kaBuf->done = 0;
    kaBuf->term = 0;
    kaBuf->kaProc = kaProc;
    kaBuf->kaArg = kaArg;
    osi_ThreadCreate(vols_RestoreKeepAlive, (void *)kaBuf, 0, 0, "ka", code);
    if (code) {
	osi_Free(kaBuf, sizeof(vols_kaRestore_t));
	kaBuf = NULL;
	goto done;
    }

    /* allocate a data buffer */
    bufP = osi_Alloc(RESTORE_DATASIZE);
    if (!bufP) {
	code = FTU_E_NO_MEMORY;
	goto done;
    }

    /* Initially, assume that the target file system handles ACLs */
    hasSetAcl = 1;

    /* initialize the async input queue */
    asyncP = initAsyncQueue(RESTORE_BUFCOUNT, RESTORE_BUFSIZE,
			    asyncPipeRead, NULL, afsPipeP);
    if ( !asyncP ) {
	code = EPIPE;
	goto done;
    }


    /* Get the initial status so that we can check for incremental
     * dump non-overlap. */
    code = VOL_GETSTATUS(fsetDesc, &returnedVstatus);
    if (code)
	goto done;
    /* Now, scan the basic header */
    if (code = vols_RestoreOpcode(asyncP, &vopcode))
	goto done;
    if (vopcode.opcode != VOLS_DUMP_HEADER_START) {
 	code = FTU_E_BAD_DUMP;
	goto done;
    }
    if (code = vols_RestoreInt32(asyncP, &temp, sizeof(unsigned long)))
	goto done;
    if (temp != VOLS_DUMP_HEADER_STARTMAGIC) {
	code = FTU_E_BAD_DUMP; 
	goto done;
    }
    AFS_hset64(dumpBaseType, ((unsigned long) -1), ((unsigned long) -1));

    /* now scan through all of the vnodes in the restore dump, doing
     * the right thing for each.  For vnodes that are marked as
     * deleted, we obliterate them.  For vnodes with updated status or
     * data, we call the vnode restoring code.  We do this until the
     * end of the dump. */
    code = 0;
    hitEOF = 0;
    while (!hitEOF) {
	/* restore the basic opcode.  At this point in the dump, we're
	 * expecting either a VOLS_DUMP_HEADER_END, symbolizing the
	 * end of a dump, a VOLS_DUMP_HEADER_VNODE, symbolizing the
	 * start of a new vnode, or a VOLS_DUMP_HEADER_NOVNODE,
	 * symbolizing a known deleted slot. We also could see any of
	 * the VOLS_DUMP_HEADER_* fields that describe the volume in
	 * more detail (quota, etc).  We keep track of these updates
	 * during the restore, and set the volume status when we're
	 * all done. */
	if (code = vols_RestoreOpcode(asyncP, &vopcode))
	    goto done;
	switch(vopcode.opcode) {
	  case VOLS_DUMP_HEADER_START:
	    /* we scan the start opcode above, so by the time we get here,
	     * there shouldn't be any more in the dump stream. */
	    code = FTU_E_BAD_DUMP;
	    break;
	  case VOLS_DUMP_HEADER_UNIQUEGEN:
	    /* this opcode is followed by 8 longs, which are to be
	     * passed into the virtual file system for its unique
	     * ID generator, if it wants one. */
	    if (vopcode.f.s.length !=
		VOLS_DUMP_NUIDS * sizeof(unsigned long)) {
		ftu_svc_LogPrintf(ftl_s_ro_restoreunique_expected,
				  VOLS_DUMP_NUIDS*sizeof(unsigned long),
				  vopcode.f.s.length);
		code = FTU_E_BAD_DUMP;
		break;
	    }
	    for (i = 0; i < VOLS_DUMP_NUIDS; i++) {
		if (code = vols_RestoreInt32(asyncP, &uidgen[i],
					    sizeof(unsigned long)))
		    break;
	    }
	    break;
	  case VOLS_DUMP_HEADER_N_UNIQUEGEN:
	    /* this opcode is followed by some number of longs, which are
	     * to be passed into the virtual file system for its unique ID
	     * generator, if it wants one.
	     */
	    if (vopcode.f.s.length % 4) {
		ftu_svc_LogPrintf(ftl_s_ro_vols_restore_no_multiple_four,
				  vopcode.f.s.length);
		code = FTU_E_BAD_DUMP;
		break;
	    }
	    for (i = (vopcode.f.s.length / 4); i > 0; --i) {
		if (code = vols_RestoreInt32(asyncP, &volUniqueGen,
					    sizeof(volUniqueGen)))
		    break;
	    }
	    /* This implementation only really deals with a single long */
	    if (vopcode.f.s.length == 4) flags |= VOL_STAT_UNIQUE;
	    break;
	  case VOLS_DUMP_HEADER_MOTD_CODESET:
	    code = vols_RestoreInt32(asyncP, &codesetTag, 
				    vopcode.f.s.length);
	    break;
	  case VOLS_DUMP_HEADER_MINQUOTA:
	    code = vols_RestoreInt32(asyncP, &minQuota,
				    vopcode.f.s.length);
	    flags |= VOL_STAT_MINQUOTA;
	    break;
	  case VOLS_DUMP_HEADER_VISQUOTALIMIT:
	    code = vols_RestoreInt64(asyncP, &visLimit,
				     vopcode.f.s.length);
	    flags |= VOL_STAT_VISLIMIT;
	    break;
	  case VOLS_DUMP_HEADER_ALLOCLIMIT:
	    code = vols_RestoreInt64(asyncP, &allocLimit,
				     vopcode.f.s.length);
	    flags |= VOL_STAT_ALLOCLIMIT;
	    break;
	  case VOLS_DUMP_HEADER_OWNER:
	    code = vols_RestoreInt32(asyncP, &volOwner,
				    vopcode.f.s.length);
	    flags |= VOL_STAT_OWNER;
	    break;
	  case VOLS_DUMP_HEADER_CREATION_DATE:
	    code = vols_RestoreTime(asyncP, &creationDate,
				    vopcode.f.s.length);
	    flags |= VOL_STAT_CREATEDATE;
	    break;
	  case VOLS_DUMP_HEADER_ACCESS_DATE:
	    code = vols_RestoreTime(asyncP, &accessDate,
				    vopcode.f.s.length);
	    flags |= VOL_STAT_ACCESSDATE;
	    break;
	  case VOLS_DUMP_HEADER_UPDATE_DATE:
	    code = vols_RestoreTime(asyncP, &updateDate,
				    vopcode.f.s.length);
	    flags |= VOL_STAT_UPDATEDATE;
	    break;
	  case VOLS_DUMP_HEADER_INDEX_MAX:
	    code = vols_RestoreInt32(asyncP, &maxNodeIx,
				    vopcode.f.s.length);
	    flags |= VOL_STAT_NODEMAX;
	    break;
	  case VOLS_DUMP_HEADER_MOTD:
	    code = vols_RestoreString(asyncP, &motd,
				      /* max length */ vopcode.f.s.length,
				      1);
	    motdLen = vopcode.f.s.length;
	    flags |= VOL_STAT_STATUSMSG;
	    break;
	  case VOLS_DUMP_HEADER_VNODE:
	    code = 0;
	    if (didStatusInit == 0) {
		/* Try to ensure that the incoming dump is OK with
		 * this fileset. */
		if (AG_TYPE_SUPPORTS_EFS(AFS_hgethi(*baseTypeP))
		    && (restoreFlags & FTU_RESTORE_DISJOINT_OK) == 0) {
		    switch (dumpCriteria.dc_type) {
		      case FTU_DUMP_TYPE_DATE_WITH_DIRS:
		      case FTU_DUMP_TYPE_DATE:
			/* Incremental by date. */
			/* Source was from date dumpCriteria.dc_date.*/
			/* Existing fileset was restored from a
			   dump made on the primary on copyDate. */
			if (returnedVstatus.vol_dy.copyDate.sec <
			    dumpCriteria.dc_date.sec) {
			    /* Non-overlap. */
			    code = FTU_E_DISJOINT_DATE;
			}
			break;
		      case FTU_DUMP_TYPE_VERS_WITH_DIRS:
		      case FTU_DUMP_TYPE_VERS:
			/* Incremental by volversion. */
			/* Source was from version 
			 * dumpCriteria.dc_version. */
			if (AFS_hcmp(returnedVstatus.vol_dy.volversion,
				 dumpCriteria.dc_version) < 0) {
			    /* Non-overlap. */
			    code = FTU_E_DISJOINT_VERSION;
			}
			break;
		    }
		    /* Ensure that this dump won't move copyDate
		     * backwards in time. */
		    
		    /* reversion on full dump is OK */
		    if (dumpCriteria.dc_type != FTU_DUMP_TYPE_FULL &&
			((flags & VOL_STAT_COPYDATE) != 0) &&
			(returnedVstatus.vol_dy.copyDate.sec >
			 copyDate.sec)) {
			code = FTU_E_RETROGRADE_DATE;
		    }
		    /* Ensure that this dump won't move backwards
		     * in version numbers. */
		    
		    /* reversion on full dump is OK */
		    if (dumpCriteria.dc_type != FTU_DUMP_TYPE_FULL &&
			((flags & VOL_STAT_VERSION) != 0) &&
			(AFS_hcmp(returnedVstatus.vol_dy.volversion,
				  volVersion) > 0)) {
			code = FTU_E_RETROGRADE_VERSION;
		    }
		    if (code) {
			break;
		    }
		}
		mask = 0;
		bzero((char *)&vstatus, sizeof(vstatus));
		/* set a couple of fields before writing in the volume */
		if (flags & VOL_STAT_VERSION) {
		    vstatus.vol_dy.volversion = volVersion;
		    mask |= VOL_STAT_VERSION;
		}
		
		/*
		 * Temporarily set limits artificially high to prevent
		 * restore from failing.  We'll reset them to their
		 * intended values below.
		 */
		if (flags & VOL_STAT_VISLIMIT) {
		    vstatus.vol_dy.visQuotaLimit = ftu_unlimitedQuota;
		    mask |= VOL_STAT_VISLIMIT;
		}
		if (flags & VOL_STAT_ALLOCLIMIT) {
		    vstatus.vol_dy.allocLimit = ftu_unlimitedQuota;
		    mask |= VOL_STAT_ALLOCLIMIT;
		}
		if (flags & VOL_STAT_NODEMAX) {
		    vstatus.vol_dy.nodeMax = maxNodeIx;
		    mask |= VOL_STAT_NODEMAX;
		}
		code = 0;
		if (mask != 0)
		    code = VOL_SETSTATUS(fsetDesc, mask, &vstatus, 0);
		if (code)
		    break;
		didStatusInit = 1;
	    }
	    if (seenEndVnodes) {
		code = FTU_E_BAD_DUMP;
		break;
	    }
	    code = vols_RestoreVnode(asyncP, bufP, fsetDesc, vopcode.f.s.length,
				     sameFSType, &hasSetAcl, baseTypeP, 
				     restoreFlags);
	    break;
	  case VOLS_DUMP_HEADER_NOVNODE:
	    if (seenEndVnodes) {
		code = FTU_E_BAD_DUMP;
		break;
	    }
	    code = vols_SmashVnode(asyncP, fsetDesc, vopcode.f.s.length);
	    break;
	  case VOLS_DUMP_HEADER_ENDVNODES:
	    code = vols_SmashAllVnodes(asyncP, fsetDesc, vopcode.f.s.length);
	    seenEndVnodes = 1;
	    break;
	  case VOLS_DUMP_HEADER_VOLVERSION:
	    code = vols_RestoreInt64(asyncP, &volVersion,
				     vopcode.f.s.length);
	    if ((flags & VOL_STAT_VERSION) == 0)
		firstVolVersion = volVersion;
	    flags |= VOL_STAT_VERSION;
	    break;
	  case VOLS_DUMP_HEADER_BASETYPE:
	    code = vols_RestoreInt64(asyncP, &dumpBaseType,
				     vopcode.f.s.length);
	    /*
	     * Allow restores from different filesystems, but remember
	     * if the types match.
	     */
	    if (!AFS_hiszero(dumpBaseType) && !AFS_hiszero(*baseTypeP)
		&& AFS_hsame(dumpBaseType, *baseTypeP)) {
		sameFSType = 1;
	    }
	    break;
	  case VOLS_DUMP_HEADER_DUMP_DATE:
	    /* This is when the dump was made on the primary. */
	    /* We'll record it as this fileset's copyDate. */
	    code = vols_RestoreTime(asyncP, &copyDate, vopcode.f.s.length);
	    flags |= VOL_STAT_COPYDATE;
	    break;
	  case VOLS_DUMP_HEADER_DUMP_STYLE:
	    bzero((char *)&dumpCriteria, sizeof dumpCriteria);
	    if (vopcode.f.s.length != (11*sizeof(unsigned long))) {
		ftu_svc_LogPrintf(ftl_s_ro_restoredumpstyle_expected,
				  11*sizeof(unsigned long),
				  vopcode.f.s.length);
		code = FTU_E_BAD_DUMP;
		goto done;
	    }
	    if (code = vols_RestoreInt32(asyncP, &dumpCriteria.dc_type,
					sizeof(long)))
		goto done;
	    if (code = vols_RestoreTime(asyncP, &dumpCriteria.dc_date,
					2*sizeof(long)))
		goto done;
	    if (code = vols_RestoreInt64(asyncP,
					 &dumpCriteria.dc_version,
					 2*sizeof(long)))
		goto done;
	    for (i = 3; i > 0; --i) {
		if (code = vols_RestoreInt64(asyncP, &junkHyper,
					     sizeof(junkHyper)))
		    goto done;
	    }
	    break;
	  case VOLS_DUMP_HEADER_INCONSISTENT:
	    /* This is opcode's data isn't currently used */
	    if (code = vols_RestoreInt32(asyncP, &temp, vopcode.f.s.length))
		goto done;
	    isInconsistent = 1;
	    break;
	  case VOLS_DUMP_HEADER_END:
	    if (code = vols_RestoreInt32(asyncP, &temp, vopcode.f.s.length))
		goto done;
	    if (temp != VOLS_DUMP_HEADER_ENDMAGIC) {
		code = FTU_E_BAD_DUMP; 
		goto done;
	    }
	    hitEOF = 1;
	    break;
	  case VOLS_DUMP_PAD:
	  default:
	    code = vols_SkipMysterious(asyncP, &vopcode);
	    break;
	}
	if (code)
	    break;
    }	/* while loop */


    /* if here, either code == 0 and we're all done, or we got an error
     * and code tells us what went wrong.  If we won, we set the volume
     * status, otherwise, we just return the error code.
     */
    if (code) goto done;

    /* set volume status block */
    if (code == 0) {
	mask = 0;
	bzero((char *)&vstatus, sizeof(vstatus));
	/* update the changed fields and set again */
	if (flags & VOL_STAT_VERSION) {
	    vstatus.vol_dy.volversion = volVersion;
	    mask |= VOL_STAT_VERSION;
	}
	if (flags & VOL_STAT_CREATEDATE) {
	    vstatus.vol_dy.creationDate.sec = creationDate.sec;
	    vstatus.vol_dy.creationDate.usec = creationDate.usec;
	    mask |= VOL_STAT_CREATEDATE;
	}
	if (flags & VOL_STAT_UPDATEDATE) {
	    vstatus.vol_dy.updateDate.sec = updateDate.sec;
	    vstatus.vol_dy.updateDate.usec = updateDate.usec;
	    mask |= VOL_STAT_UPDATEDATE;
	}
	if (flags & VOL_STAT_ACCESSDATE) {
	    vstatus.vol_dy.accessDate.sec = accessDate.sec;
	    vstatus.vol_dy.accessDate.usec = accessDate.usec;
	    mask |= VOL_STAT_ACCESSDATE;
	}
	if (flags & VOL_STAT_COPYDATE) {
	    vstatus.vol_dy.copyDate.sec = copyDate.sec;
	    vstatus.vol_dy.copyDate.usec = copyDate.usec;
	    mask |= VOL_STAT_COPYDATE;
	}
	if (flags & VOL_STAT_VISLIMIT) {
	    vstatus.vol_dy.visQuotaLimit = visLimit;
	    mask |= VOL_STAT_VISLIMIT;
	}
	if (flags & VOL_STAT_ALLOCLIMIT) {
	    vstatus.vol_dy.allocLimit = allocLimit;
	    mask |= VOL_STAT_ALLOCLIMIT;
	}
	if (flags & VOL_STAT_STATUSMSG) {
	    vstatus.vol_dy.tag = codesetTag;
	    strncpy(vstatus.vol_dy.statusMsg, motd,
		    sizeof(vstatus.vol_dy.statusMsg));
	    mask |= VOL_STAT_STATUSMSG;
	}
	if (flags & VOL_STAT_MINQUOTA) {
	    vstatus.vol_dy.minQuota = minQuota;
	    mask |= VOL_STAT_MINQUOTA;
	}
	if (flags & VOL_STAT_OWNER) {
	    vstatus.vol_dy.owner = volOwner;
	    mask |= VOL_STAT_OWNER;
	}
	if (flags & VOL_STAT_UNIQUE) {
	    vstatus.vol_dy.unique = volUniqueGen;
	    mask |= VOL_STAT_UNIQUE;
	}
	code = VOL_SETSTATUS(fsetDesc, mask, &vstatus, 0);
	if (verbose) {
	    if (code != 0)
		dce_svc_printf(FTL_S_RO_VOL_SETSTATUS_ERR_MSG, code);
	    xcode = VOL_GETSTATUS(fsetDesc, &returnedVstatus);
	    if (xcode != 0) {
		dce_svc_printf(FTL_S_RO_VOL_GETSTATUS_ERR_MSG, code);
	    } else if (!AFS_hsame(vstatus.vol_dy.volversion,
			      returnedVstatus.vol_dy.volversion)) {
		dce_svc_printf(FTL_S_RO_SET_VV_GET_MSG,
			       AFS_HGETBOTH(vstatus.vol_dy.volversion),
			       AFS_HGETBOTH(returnedVstatus.vol_dy.volversion));
	    }
	}
    }

  done:
    /* terminate the async input queue */
    if ( asyncP ) {
	elemP = (asyncElement_t *)asyncP->callerState;
	do {
	    if (elemP) relAsyncElement(asyncP, elemP);
	    elemP = getAsyncElement(asyncP);
	} while(elemP);
	termAsyncQueue(asyncP);
    }

    /* we must check this parameter in case we fall through here from
     * the initial creation code. */
    if (afsPipeP->pipe_rele) {
	(void)POP_RELE(afsPipeP);
    }
    if (motd) osi_Free(motd, motdLen);

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
	osi_Free(kaBuf, sizeof(vols_kaRestore_t));
    }

    /* free the data buffer */
    if (bufP) {
        osi_Free(bufP, RESTORE_DATASIZE);
    }
    
    if (code == 0 && isInconsistent) {
	/* If everything is otherwise alright but the fileset was inconsistent
	 * when it was dumped, notify our caller via a distinguished error
	 * code.  This gives them the opportunity to leave the fileset
	 * inconsistent by aborting the open instead of closing the fileset.
	 */
	code = FTU_E_RESTORED_INCONSISTENT_FSET;
    }

    /* clean up and return code */
    ftu_svc_LogPrintf(ftl_s_ro_vols_restore_returns, code);
    return code;
}
