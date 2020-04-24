/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_text.c,v $
 * Revision 1.1.91.1  1996/10/02  16:59:57  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:18  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1994, 1996 Transarc Corporation
 * All Rights Reserved
 */
/*
 * db_text.c -- text code
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <sys/param.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/file.h>
#include <dcedfs/param.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/lock.h>
#include <budb_errs.h>
#include <database.h>
#include <error_macros.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <globals.h>
/* The bakserver database defined in database.c */
extern struct memoryDB db;

/* --------------------------------
 * interface & support code to manage text blocks within the
 * 	database
 * --------------------------------
 */

/* BUDB_GetText
 * notes:
 *	routine mallocs storage for charListPtr, freed by stub
 */

error_status_t
BUDB_GetText(h, lockHandle, textType, maxLength, offset, nextOffset,
	     charListPtr)
  handle_t h;
     long lockHandle;
     long textType;
     long maxLength;
     long offset;
     long *nextOffset;
     charListT *charListPtr;
{
    struct ubik_trans *ut;
    struct block block;
    long transferSize, chunkSize;
    long blockOffset;
    dbadr lastBlockAddr;
    long nblocks;
    struct textBlock *tbPtr;
    long textRemaining;
    char *textPtr;
    long code;
    char *rtn_Name="BUDB_GetText";
    int cancelState, cancelCode;

    
    /* Init charListPtr to prevent exception in marshalling 
     on error return */

    charListPtr->charListT_len = 0;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if (!callPermitted(h)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_NOTPERMITTED;
    }
    /* check parameters */
    if ( (offset < 0)
    ||   (textType < 0)
    ||   (textType >= TB_NUM)
       )
    {
        code = BUDB_BADARGUMENT;
	bak_enableCancel(rtn_Name, cancelState, &cancelCode);
	return(code);
    }

    /* start the transaction */
    code = InitRPC(&ut, LOCKWRITE, (int *) 1);
    if (code) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return (code);
    }
    LogDebug(2, "Entering BUDB_GetText: \n");
    LogDebug(2,"input parameters: text type %ld, max length %ld, offset %ld; ",
	     textType, maxLength, offset);
    LogDebug(1, "retrieving text of type %ld, length %ld from database\n",
	     textType, maxLength);
    lock_ObtainRead(&(db.lock));
    /* fetch the lock state */
    if ( checkLockHandle(ut, lockHandle) == 0) {
	code = BUDB_NOTLOCKED;
	goto no_xfer_abort;
    }

    tbPtr = &db.h.textBlock[textType];

    if ( (ntohl(tbPtr->size) > 0)
    &&   (offset >= ntohl(tbPtr->size))
       )
    {
    	code = BUDB_BADARGUMENT;
	goto no_xfer_abort;
    }

    /* compute minimum of remaining text or user buffer */
    textRemaining = ntohl(tbPtr->size) - offset;
    transferSize = MIN(textRemaining, maxLength);

    charListPtr->charListT_len = transferSize;
    bzero((char *)charListPtr->charListT_val, transferSize);
    textPtr = (char *)charListPtr->charListT_val;
    *nextOffset = offset + transferSize;

    LogDebug(1, "text size read is %ld, next offset is %ld; ", transferSize,
	     *nextOffset);
    /* setup the datablock. read and discard all blocks up to the one the
     * offset specifies
     */
    nblocks = offset/BLOCK_DATA_SIZE;
    lastBlockAddr = ntohl(tbPtr->textAddr);

    while ( nblocks-- )
    {
	code = dbread(ut, lastBlockAddr, (char *) &block, sizeof(block));
	if ( code )
	  ABORT(BUDB_IO);
	lastBlockAddr = ntohl(block.h.next);
	LogDebug(3, "\nfetched block %d; ", lastBlockAddr);
    }

    while ( transferSize > 0 )
    {
	code = dbread(ut, lastBlockAddr, (char *) &block, sizeof(block));
	if ( code )
	  ABORT(BUDB_IO);

	LogDebug(3, "fetched block %d; ", lastBlockAddr);

	/* compute the data size to extract */
	blockOffset = offset % BLOCK_DATA_SIZE;
	textRemaining = BLOCK_DATA_SIZE - blockOffset;
	chunkSize = MIN(textRemaining, transferSize);

	bcopy(&block.a[blockOffset], textPtr, chunkSize);

	transferSize -= chunkSize;
	offset += chunkSize;
	textPtr += chunkSize;

	if ( transferSize )
	{
	    /* setup lastBlockAddr */
	    lastBlockAddr = ntohl(block.h.next);
	}
    }
    LogDebug(1, "\n");
    if ( *nextOffset == ntohl(tbPtr->size) )
    {
	/* all done */
	*nextOffset = -1;
	LogDebug(3, "complete block read\n");
    }

error_exit:
    lock_ReleaseRead(&(db.lock));
    code = ubik_EndTrans(ut);
    LogDebug(2, "Leaving BUDB_GetText, code %ld\n", code);
    if (code)
      LogDebug(1, "failed to get text, code=%ld\n", code);
    else
      LogDebug(1, "successfully obtained text\n");
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);

no_xfer_abort:
abort_exit:
    lock_ReleaseRead(&(db.lock));
    ubik_AbortTrans(ut);
    LogDebug(2, "Leaving BUDB_GetText, code %ld\n", code);
    LogDebug(1, "failed to get text, code=%ld\n", code);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}

freeOldBlockChain(ut, diskAddr)
     struct ubik_trans *ut;
     dbadr diskAddr;
{
    struct blockHeader blockHeader;
    dbadr nextDiskAddr;
    long code=0;

    while ( diskAddr != 0 )
    {
	/* read in the header */
	code = dbread(ut, diskAddr, (char *) &blockHeader,sizeof(blockHeader));
	if ( code )
		ABORT(code);
	nextDiskAddr = ntohl(blockHeader.next);
	code = FreeBlock(ut, &blockHeader, diskAddr);
	if ( code )
		ABORT(code);
	diskAddr = nextDiskAddr;
    }
abort_exit:
    return(code);
}

/* BUDB_GetTextVersion
 *	get the version number for the specified text block
 */

error_status_t
BUDB_GetTextVersion(h, textType,  tversion)
  handle_t h;
     long textType;
     long *tversion;
{
    long code;
    struct ubik_trans *ut;
    char *rtn_Name="BUDB_GetTextVersion";
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if (!callPermitted(h)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_NOTPERMITTED;
    }
    if ( (textType < 0) || (textType >= TB_NUM) ) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_BADARGUMENT);
    }
    code = InitRPC(&ut, LOCKREAD, (int *) 1);
    if (code) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(code);
    }
    LogDebug(2, "Entering BUDB_GetTextVersion: \n");
    LogDebug(2, "input parameter: text type %ld; ", textType);
    lock_ObtainRead(&(db.lock));
    *tversion = ntohl(db.h.textBlock[textType].version);
    lock_ReleaseRead(&(db.lock));
    code = ubik_EndTrans(ut);
    LogDebug(1, "text version is %ld\n", *tversion);
    LogDebug(2, "Leaving BUDB_GetTextVersion, code %ld\n", code);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}

/* text blocks
 *	next - next disk addr
 * host/network ordering????
 */

/* BUDB_SaveText
 * notes:
 *	charListPtr storage automatically malloced and freed
 */

error_status_t
BUDB_SaveText(h, lockHandle, textType, offset, flags, charListPtr)
  handle_t h;
     long lockHandle;
     long textType;
     long offset;
     long flags;
     charListT *charListPtr;
{
    struct ubik_trans *ut;
    struct block diskBlock;
    dbadr diskBlockAddr;
    long remainingInBlock, chunkSize;
    struct textBlock *tbPtr;
    long textLength = charListPtr->charListT_len;
    char *textptr = (char *)charListPtr->charListT_val;
    long code;
    char *rtn_Name="BUDB_SaveText";
    int cancelState, cancelCode;

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    if (!callPermitted(h)) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return BUDB_NOTPERMITTED;
    }

    if ( (textLength > BLOCK_DATA_SIZE) || textLength < 0 ||(offset < 0) )
    {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(BUDB_BADARGUMENT);
    }

    code = InitRPC((struct ubik_trans **)&ut, LOCKWRITE, (int *) 1);
    if (code) {
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      return(code);
    }
    LogDebug(2, "Entering BUDB_SaveText: \n");
    LogDebug(2, "input parameters: lock handle %ld, text type %ld, offset %ld, flags %ld, text length %ld\n",lockHandle, textType, offset, flags, textLength);
    LogDebug(1, "Saving text of type %ld length %ld\n", textType, textLength);
    lock_ObtainWrite(&(db.lock));
    /* fetch the lock state */
    if ( checkLockHandle(ut, lockHandle) == 0)
      ABORT(BUDB_NOTLOCKED);

    if ( (textType < 0)
    ||   (textType >= TB_NUM)
       )
    	ABORT(BUDB_BADARGUMENT);

    tbPtr = &db.h.textBlock[textType];

    if ( offset == 0 )
    {
	/* release any blocks from previous transactions */
	diskBlockAddr = ntohl(tbPtr->newTextAddr);
	if ( code = freeOldBlockChain(ut, diskBlockAddr) )
	  ABORT(code);

	code = AllocBlock(ut, &diskBlock, &diskBlockAddr);
        if ( code )
	  ABORT(code);

	/* set block type */
	diskBlock.h.type = text_BLOCK;

	/* save it in the database header */
	tbPtr->newsize = 0;
	tbPtr->newTextAddr = htonl(diskBlockAddr);
        LogDebug(1, "no text blocks left, allocated a new one at %ld; ",
		 diskBlockAddr);
	if (code = dbwrite(ut, (char *) tbPtr - (char *) &db.h, (char *) tbPtr,
		sizeof(struct textBlock)))
	  ABORT(code);
    }
    else
    {
	/* non-zero offset */
	int nblocks;

        if ( offset != ntohl(tbPtr->newsize) )
	    ABORT(BUDB_BADARGUMENT);

	/* locate the block to which offset refers */
	nblocks = offset/BLOCK_DATA_SIZE;

	diskBlockAddr = ntohl(tbPtr->newTextAddr);
	if ( diskBlockAddr == 0 )
		ABORT(BUDB_BADARGUMENT);
	LogDebug(1, "partial text block left at %ld; ", diskBlockAddr);
	code = dbread(ut, diskBlockAddr, (char *) &diskBlock,
		      sizeof(diskBlock));
	if ( code )
		ABORT(code);

	while ( nblocks-- )
	{
	    diskBlockAddr = ntohl(diskBlock.h.next);
	    code = dbread(ut, diskBlockAddr, (char *) &diskBlock,
			  sizeof(diskBlock));
	    if ( code )
	    	ABORT(code);
	}
    }

    LogDebug(2, "got text block at %ld, start writing text; ", diskBlockAddr);
    /* diskBlock and diskBlockAddr now point to the last block in the chain */


    do {
	/* compute the transfer size */
	remainingInBlock = (BLOCK_DATA_SIZE - (offset % BLOCK_DATA_SIZE));
	chunkSize = MIN(remainingInBlock, textLength);

	/* copy in the data */
	bcopy(textptr, &diskBlock.a[offset % BLOCK_DATA_SIZE], chunkSize);
	textLength -= chunkSize;
	textptr += chunkSize;
	offset += chunkSize;
	tbPtr->newsize = htonl(ntohl(tbPtr->newsize) + chunkSize);

	if ( textLength > 0 )
	{
	    long prevBlockAddr;
	    long linkOffset;
	    long linkValue;

	    /* have to add another block to the chain */
	    LogDebug(2, "writing text block at %ld; ", diskBlockAddr);
	    code = dbwrite(ut, diskBlockAddr, (char *) &diskBlock,
			   sizeof(diskBlock));
	    if (code)
	    	ABORT(code);

	    prevBlockAddr = (long) diskBlockAddr;
	    code = AllocBlock(ut, &diskBlock, &diskBlockAddr);
	    if ( code )
	    	ABORT(code);
	    /* set block type */
	    diskBlock.h.type = text_BLOCK;

	    /* now have to update the previous block's link */
	    linkOffset = (long) &diskBlock.h.next - (long) &diskBlock;
	    linkValue = htonl(diskBlockAddr);

	    code = dbwrite(ut, (long) prevBlockAddr + linkOffset,
			   (char *) &linkValue, sizeof(long));
	    if (code)
	    	ABORT(code);
	}
	else
	{
	    LogDebug(1, "no more text left, write the partial block at %ld; ",
		     diskBlockAddr);
	    /* just write the old block */
	    code = dbwrite(ut, diskBlockAddr, (char *) &diskBlock,
			   sizeof(diskBlock));
	    if (code)
	    	ABORT(code);
	}
    } while ( textLength != 0 );

    if ( flags & BUDB_TEXT_COMPLETE )			/* done */
    {
        LogDebug(1, "last chunk of text written; ");
	/* this was the last chunk of text */
	diskBlockAddr = ntohl(tbPtr->textAddr);
	if (code = freeOldBlockChain(ut, diskBlockAddr))
	  ABORT(code);
	tbPtr->textAddr = tbPtr->newTextAddr;
	tbPtr->newTextAddr = 0;
	tbPtr->size = tbPtr->newsize;
	tbPtr->newsize = 0;
	tbPtr->version = htonl(ntohl(tbPtr->version) + 1);

	/* saveTextToFile(ut, tbPtr); */
    }

    LogDebug(2, "update size to %ld, text addr to %ld, version to %ld\n",
	     ntohl(tbPtr->size), ntohl(tbPtr->textAddr),ntohl(tbPtr->version));
    /* update size and other text header info */
    code = dbwrite(ut, (char *) tbPtr - (char *) &db.h, (char *) tbPtr,
		   sizeof(struct textBlock));
    if ( code )
    	ABORT(code);

error_exit:
    lock_ReleaseWrite(&(db.lock));
    code = ubik_EndTrans(ut);
    LogDebug(2, "Leaving BUDB_SaveText, code %ld\n", code);
    if (code)
      LogDebug(1, "failed to save text, code=%ld\n", code);
    else
      LogDebug(1, "successfully saved text\n");
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);

abort_exit:
    lock_ReleaseWrite(&(db.lock));
    ubik_AbortTrans(ut);
    LogDebug(2, "Leaving BUDB_SaveText, code %ld\n", code);
    LogDebug(1, "failed to save text, code=%ld\n", code);
    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
    return(code);
}

/* debug support */

saveTextToFile(ut, tbPtr)
     struct ubik_trans *ut;
     struct textBlock *tbPtr;

{
    long blockAddr;
    struct block block;

    long size, chunkSize;
    char filename[64];
    int fid;

    extern int mkstemp();

    strcpy(filename, "/tmp/dbg_XXXXXX");
    fid = mkstemp(filename);

    size = ntohl(tbPtr->size);
    blockAddr = ntohl(tbPtr->textAddr);
    while ( size )
    {
	chunkSize = MIN(BLOCK_DATA_SIZE, size);
	dbread(ut, blockAddr, (char *) &block, sizeof(block));
	write(fid, &block.a[0], chunkSize);
	blockAddr = ntohl(block.h.next);
	size -= chunkSize;
    }
    close(fid);
    printf("wrote debug file %s\n", filename);
}


#ifdef AFS_HPUX_ENV

/* mkstemp
 * entry:
 *	st - string containing template for a tmp file name
 * exit:
 *	-1 - failed
 *	0-n - open file descriptor
 * notes:
 *	1) missing in HP/UX environment
 *	    Note added 9/13/94: HP/UX has this function.
 *	    On the other hand, the man page says that it's obsolete
 *	    and you're supposed to use tmpfile().  For now, leaving as is.
 *      2) iterate some number of times to alleviate the race?
 */

int
mkstemp(st)
     char *st;
{
    int retval = -1;

    retval = open(mktemp(st), O_RDWR|O_CREAT|O_EXCL, 0600);

error_exit:
    return(retval);
}

#endif /* AFS_HPUX_ENV */
