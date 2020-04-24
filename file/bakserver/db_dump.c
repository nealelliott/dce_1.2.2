/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_dump.c,v $
 * Revision 1.1.97.1  1996/10/02  16:59:50  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:15  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * db_dump.c -- bak database dump
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bakserver/db_dump.c,v 1.1.97.1 1996/10/02 16:59:50 damon Exp $")
/*
 * Dump is made to a local file. Structures are dumped in network byte order
 * for transportability between hosts
 */

#include <stdio.h>
#include <sys/param.h>
#include <dcedfs/param.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/lock.h>
#include <pthread.h>
#include <database.h>
#include <error_macros.h>
#include <budb_errs.h>
#include <globals.h>
/* The bakserver database defined in database.c */
extern struct memoryDB db;

/* dump ubik database - routines to scan the database and dump all
 * 	the information
 */
/* notes:
 *	ut - setup and pass down
 */

/* writeStructHeader
 *	write header appropriate for requested structure type
 */

long
writeStructHeader(fid, type)
     int fid;
     long type;
{
    structDumpHeader hostDumpHeader, netDumpHeader;

    hostDumpHeader.type = type;
    hostDumpHeader.structversion = 1;

    switch ( type )
    {
      case SD_DBHEADER:
	hostDumpHeader.size = sizeof(DbHeader);
	break;

      case SD_DUMP:
	hostDumpHeader.size = sizeof(budb_dumpEntry);
	break;

      case SD_TAPE:
	hostDumpHeader.size = sizeof(budb_tapeEntry);
	break;

      case SD_VOLUME:
	hostDumpHeader.size = sizeof(budb_dbVolume_t);
	break;

      case SD_END:
	hostDumpHeader.size = 0;
	break;

      default:
	LogError(0, "writeStructHeader: invalid type %d\n", type);
	exit(1);
    }

    structDumpHeader_hton(&hostDumpHeader, &netDumpHeader);

    if ( write(fid, &netDumpHeader, sizeof(netDumpHeader)) !=
		sizeof(netDumpHeader) )
      return(BUDB_DUMPFAILED);
    return(0);
}

/* writeTextHeader
 *	write header appropriate for requested structure type
 */

long
writeTextHeader(fid, type)
     int fid;
     long type;
{
    structDumpHeader hostDumpHeader, netDumpHeader;
    hostDumpHeader.structversion = 1;

    switch ( type )
    {
      case TB_DUMPSCHEDULE:
	hostDumpHeader.type = SD_TEXT_DUMPSCHEDULE;
	break;

      case TB_VOLUMESET:
	hostDumpHeader.type = SD_TEXT_VOLUMESET;
	break;

      case TB_TAPEHOSTS:
	hostDumpHeader.type = SD_TEXT_TAPEHOSTS;
	break;

      default:
	LogError(0, "writeTextHeader: invalid type %d\n", type);
	exit(1);
    }
    hostDumpHeader.size = ntohl(db.h.textBlock[type].size);
    structDumpHeader_hton(&hostDumpHeader, &netDumpHeader);
    if ( write(fid, &netDumpHeader, sizeof(netDumpHeader)) !=
		sizeof(netDumpHeader) )
	return(BUDB_DUMPFAILED);
    return(0);
}

long
writeDbHeader(fid)
     int fid;
{
    DbHeader header;
    long curtime;
    long code = 0;

    extern struct memoryDB db;
    /* check the memory database header for integrity */
    if ( db.h.version != db.h.checkVersion ) {
      ERROR(BUDB_DATABASEINCONSISTENT);
    }

    curtime = time(0);

    /* copy selected fields. Source is in xdr format. */
    header.dbversion = db.h.version;
    header.created = htonl(curtime);
    strcpy((char *)header.cell, "");
    header.lastDumpId = db.h.lastDumpId;
    header.lastInstanceId = db.h.lastInstanceId;
    header.lastTapeId = db.h.lastTapeId;

    code = writeStructHeader(fid, SD_DBHEADER);
    if ( code )
	ERROR(code);
    if ( write(fid, &header, sizeof(header)) != sizeof(header) )
        ERROR(BUDB_DUMPFAILED);
error_exit:
    return(code);
}

/* writeDump
 *	write out a dump entry structure
 */

long
writeDump(fid, dumpPtr)
     int fid;
     dbDumpP dumpPtr;
{
    budb_dumpEntry dumpEntry;
    long code = 0;
    code = dumpToBudbDump(dumpPtr, &dumpEntry);
    if ( code )
	ERROR(code);

    writeStructHeader(fid, SD_DUMP);
    if ( write(fid, &dumpEntry, sizeof(dumpEntry)) != sizeof(dumpEntry) )
	ERROR(BUDB_DUMPFAILED);
error_exit:

    return(code);
}

long
writeTape(fid, tapePtr, dumpid)
     int fid;
     struct tape *tapePtr;
     long dumpid;
{
    budb_tapeEntry tapeEntry;
    long code = 0;
    code = writeStructHeader(fid, SD_TAPE);
    if ( code )
	ERROR(code);

    tapeToBudbTape(tapePtr, &tapeEntry);

    tapeEntry.dump = htonl(dumpid);
    if ( write(fid, &tapeEntry, sizeof(tapeEntry)) != sizeof(tapeEntry) )
	ERROR(BUDB_DUMPFAILED);
error_exit:

    return(code);
}

/* combines volFragment and volInfo */

long
writeVolume(ut, fid, volFragmentPtr, dumpid, tapeName)
     struct ubik_trans *ut;
     int fid;
     struct volFragment *volFragmentPtr;
     long dumpid;
     char *tapeName;
{
    struct volInfo volInfo;
    budb_dbVolume_t budbVolume;
    long code = 0;

    code = dbread(ut, ntohl(volFragmentPtr->vol), &volInfo, sizeof(volInfo));
    if ( code )
        ERROR(code);

    volsToBudbVol(volFragmentPtr, &volInfo, &budbVolume);

    budbVolume.dump = htonl(dumpid);
    strcpy((char *)budbVolume.tape, tapeName);

    writeStructHeader(fid, SD_VOLUME);
    if ( write(fid, &budbVolume, sizeof(budbVolume)) != sizeof(budbVolume) )
        ERROR(code);
error_exit:

    return(code);
}

/* -------------------
 * handlers for the text blocks
 * -------------------
 */

/* checkLock
 *	make sure a text lock is NOT held
 * exit:
 *	0 - not held
 *	n - error
 */

long
checkLock(textType)
     long textType;
{
    db_lockP	lockPtr;

    if ( (textType < 0) || (textType > TB_NUM-1) )
	return(BUDB_BADARGUMENT);

    lockPtr = &db.h.textLocks[textType];

    if ( lockPtr->lockState != 0 )
	return(BUDB_LOCKED);
    return(0);
}

/* checkText
 *	check the integrity of the specified text type
 */

checkText(ut, textType)
     struct ubik_trans *ut;
     long textType;
{
    struct textBlock *tbPtr;
    long nBytes = 0;			/* accumulated actual size */
    long size;
    struct block block;
    dbadr blockAddr;

    long code;

    tbPtr = &db.h.textBlock[textType];
    blockAddr = ntohl(tbPtr->textAddr);
    size = ntohl(tbPtr->size);

    while ( blockAddr != 0 )
    {
	/* read the block */
	code = dbread(ut, blockAddr, (char *) &block, sizeof(block));
	if ( code )
	    ERROR(code);

	/* check its type */
	if ( block.h.type != text_BLOCK )
	    ERROR(BUDB_DATABASEINCONSISTENT);

	/* add up the size */
	nBytes += BLOCK_DATA_SIZE;

	blockAddr = ntohl(block.h.next);
    }

    /* ensure that we have at least the expected amount of text */
    if ( nBytes < size )
	ERROR(BUDB_DATABASEINCONSISTENT);

error_exit:
    return(code);
}

/* writeText
 * entry:
 *	textType - type of text block, e.g. TB_DUMPSCHEDULE
 */

long
writeText(ut, fid, textType)
     struct ubik_trans *ut;
     int fid;
     int textType;
{
    struct textBlock *tbPtr;
    long textSize, writeSize;
    dbadr dbAddr;
    struct block block;
    long code = 0;

    /* check lock is free */
    code = checkLock(textType);
    if ( code )
	ERROR(code);

     /* ensure that this block has the correct type */
    code = checkText(ut, textType);
    if ( code )
    {
	LogError(0, "writeText: text type %d damaged\n",
	       textType);
	ERROR(code);
    }

    writeTextHeader(fid, textType);

    tbPtr = &db.h.textBlock[textType];
    textSize = ntohl(tbPtr->size);
    dbAddr = ntohl(tbPtr->textAddr);

    while ( dbAddr )
    {
	code = dbread(ut, dbAddr, (char *) &block, sizeof(block));
	if (code)
	    ERROR(code);

	writeSize = MIN(textSize, BLOCK_DATA_SIZE);
	if ( writeSize == 0 )
	    break;
	if ( write(fid, &block.a[0], writeSize) != writeSize )
	    ERROR(BUDB_IO);
	dbAddr = ntohl(block.h.next);
    }

error_exit:

    return(code);
}

long
writeDatabase(ut, fid)
     struct ubik_trans *ut;
     int fid;
{
    static char rtn_Name[] = "writeDatabase";
    dbadr dbAddr;
    struct dump diskDump;
    dbadr tapeAddr;
    struct tape diskTape;
    dbadr volFragAddr;
    struct volFragment diskVolFragment;
    int length, hash;
    int old = 0;
    int entrySize;
    volatile long code = 0;
    struct memoryHashTable *mht;
    char *excNameP, excString[20];
    volatile int cancelState, cancelCode, exceptionCaught;
    extern char * ExceptionToString();

    LogDebug(3, "Entering writeDatabase: \n");

    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
    if (cancelCode)
      return(cancelCode);

    lock_ObtainShared(&(db.lock));
    /* write out a header identifying this database etc */

    exceptionCaught = 0;
    TRY {
      code = writeDbHeader(fid);
    }
    CATCH (exc_SIGPIPE_e) {
      LogError(EPIPE,
	       "writeDatabase: broken pipe, terminating database dump\n");
      lock_ReleaseShared(&(db.lock));
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      exceptionCaught = 1;
      code = EPIPE;
    }
    CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      LogError(0,
       "%s: unexpected exception '%s'; errno=%d, terminating database dump\n",
       rtn_Name, excNameP, errno);
      lock_ReleaseShared(&(db.lock));
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      exceptionCaught = 1;
      code = EPIPE;
    }
    ENDTRY
    if (exceptionCaught)
	return code;
    if ( code )
	ERROR(code);

    /* write out the tree of dump structures */

    lock_UpgradeSToW(&(db.lock));
    mht = ht_GetType (HT_dumpIden_FUNCTION, &entrySize);
    lock_ConvertWToS(&(db.lock));
    if ( mht == 0 )
	ERROR(BUDB_BADARGUMENT);
    for ( old = 0; old <= 1; old++ )
    {
        /*
	 * Check for any pending cancels. Ensure that we don't hang on to
	 * locks or other resources while checking for pending cancels.
	 */
        lock_ReleaseShared(&(db.lock));
        bak_enableCancel(rtn_Name, cancelState, &cancelCode);
	pthread_testcancel();
	bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
	if (cancelCode)
	  return(cancelCode);
	lock_ObtainShared(&(db.lock));

	/* only two states, old or not old */
	if ( old == 0 )
	    length = mht->length;
	else
	    length = mht->oldLength;

	if ( length == 0 )
	    continue;

	for ( hash = 0; hash < length; hash++ )
	{ /*f*/
  	    /*
	     * Check for any pending cancels. Ensure that we don't hang on to
	     * locks or other resources while checking for pending cancels.
	     */
	    lock_ReleaseShared(&(db.lock));
	    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
	    pthread_testcancel();
	    bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
	    if (cancelCode)
	      return(cancelCode);
	    lock_ObtainWrite(&(db.lock)); /* get write for the ht_ routine */

	    dbAddr = ht_LookupBucket (ut, mht, hash, old);
	    lock_ConvertWToS(&(db.lock));

	    /* dump all the items in this bucket */
	    while ( dbAddr )
	    { /*w*/
	      /*
	       * Check for any pending cancels. Ensure that we don't hang on to
	       * locks or other resources while checking for pending cancels.
	       */
	      lock_ReleaseShared(&(db.lock));
	      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
	      pthread_testcancel();
	      bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
	      if (cancelCode)
		return(cancelCode);
	      lock_ObtainShared(&(db.lock));

	      /* now check if this dump had any errors/inconsistencies.
	       * If so, don't dump it
	       */

	      if ( badEntry(dbAddr) )
		{
		  LogError(0, "writeDatabase: damaged dump entry at %d\n",
			   dbAddr);
		  continue;
		}

	      code = dbread(ut, dbAddr, &diskDump, sizeof(diskDump));
	      if ( code )
		ERROR(code);
	      exceptionCaught = 0;
	      TRY {
		code = writeDump(fid, &diskDump);
	      }
	      CATCH (exc_SIGPIPE_e) {
		LogError(EPIPE,
		    "writeDatabase: broken pipe, terminating database dump\n");
		lock_ReleaseShared(&(db.lock));
		bak_enableCancel(rtn_Name, cancelState, &cancelCode);
		exceptionCaught = 1;
		code = EPIPE;
	      }
	      CATCH_ALL {
		excNameP = ExceptionToString(THIS_CATCH);
		if (excNameP == NULL) {
		  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
		  excNameP = excString;
		}
		LogError(0,
			 "%s: unexpected exception '%s'; errno=%d, terminating database dump\n",
			 rtn_Name, excNameP, errno);
		lock_ReleaseShared(&(db.lock));
		bak_enableCancel(rtn_Name, cancelState, &cancelCode);
		exceptionCaught = 1;
		code = EPIPE;
	      }
	      ENDTRY
	      if (exceptionCaught)
		  return code;
	      if ( code )
		ERROR(code);

	      tapeAddr = ntohl(diskDump.firstTape);
	      while ( tapeAddr ) {
		lock_ReleaseShared(&(db.lock));
		bak_enableCancel(rtn_Name, cancelState, &cancelCode);
		pthread_testcancel();
		bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
		if (cancelCode)
		  return(cancelCode);
		lock_ObtainShared(&(db.lock));

		/* dump out all the tapes in this dump */
		code = dbread(ut, tapeAddr, &diskTape, sizeof(diskTape));

		exceptionCaught = 0;
		TRY {
		  writeTape(fid, &diskTape, ntohl(diskDump.id));
		}
		CATCH (exc_SIGPIPE_e) {
		  LogError(EPIPE,
		   "writeDatabase: broken pipe, terminating database dump\n");
		  lock_ReleaseShared(&(db.lock));
		  bak_enableCancel(rtn_Name, cancelState, &cancelCode);
		  exceptionCaught = 1;
		  code = EPIPE;
		}
		CATCH_ALL {
		  excNameP = ExceptionToString(THIS_CATCH);
		  if (excNameP == NULL) {
		    sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
		    excNameP = excString;
		  }
		  LogError(0,
			   "%s: unexpected exception '%s'; errno=%d, terminating database dump\n",
			   rtn_Name, excNameP, errno);
		  lock_ReleaseShared(&(db.lock));
		  bak_enableCancel(rtn_Name, cancelState, &cancelCode);
		  exceptionCaught = 1;
		  code = EPIPE;
		}
		ENDTRY
		if (exceptionCaught)
		    return code;
		volFragAddr = ntohl(diskTape.firstVol);
		while ( volFragAddr ) {
		  lock_ReleaseShared(&(db.lock));
		  bak_enableCancel(rtn_Name, cancelState, &cancelCode);
		  pthread_testcancel();
		  bak_disableCancel(rtn_Name, &cancelState, &cancelCode);
		  if (cancelCode)
		    return(cancelCode);
		  lock_ObtainShared(&(db.lock));
		  /* dump all the volumes on this tape */
		  code = dbread(ut, volFragAddr, &diskVolFragment,
				sizeof(diskVolFragment));
		  if ( code )
		    ERROR(code);

		  exceptionCaught = 0;
		  TRY {
		    code = writeVolume(ut, fid, &diskVolFragment,
				       ntohl(diskDump.id), diskTape.name);
		  }
		  CATCH (exc_SIGPIPE_e) {
		    LogError(EPIPE,
		    "writeDatabase: broken pipe, terminating database dump\n");
		    lock_ReleaseShared(&(db.lock));
		    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
		    exceptionCaught = 1;
		    code = EPIPE;
		  }
		  CATCH_ALL {
		    excNameP = ExceptionToString(THIS_CATCH);
		    if (excNameP == NULL) {
		      sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
		      excNameP = excString;
		    }
		    LogError(0,
			     "%s: unexpected exception '%s'; errno=%d, terminating database dump\n",
			     rtn_Name, excNameP, errno);
		    lock_ReleaseShared(&(db.lock));
		    bak_enableCancel(rtn_Name, cancelState, &cancelCode);
		    exceptionCaught = 1;
		    code = EPIPE;
		  }
		  ENDTRY
		  if (exceptionCaught)
		      return code;
		  volFragAddr = ntohl(diskVolFragment.sameTapeChain);
		}
		tapeAddr = ntohl(diskTape.nextTape);
	      }
	      dbAddr = ntohl(diskDump.idHashChain);
	    } /*w*/
	} /*f*/
    } /* old */

    exceptionCaught = 0;
    TRY {
      /* write out the textual configuration information */
      code = writeText(ut, fid, TB_DUMPSCHEDULE);
      if (!code)
	  code = writeText(ut, fid, TB_VOLUMESET);
      if (!code)
	  code = writeText(ut, fid, TB_TAPEHOSTS);
      if (!code) {
	  code = writeStructHeader(fid, SD_END);
	  code = writeDbHeader(fid);
      }
    }
    CATCH (exc_SIGPIPE_e) {
      LogError(EPIPE,
	       "writeDatabase: broken pipe, terminating database dump\n");
      lock_ReleaseShared(&(db.lock));
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      exceptionCaught = 1;
      code = EPIPE;
    }
    CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      LogError(0,
       "%s: unexpected exception '%s'; errno=%d, terminating database dump\n",
       rtn_Name, excNameP, errno);
      lock_ReleaseShared(&(db.lock));
      bak_enableCancel(rtn_Name, cancelState, &cancelCode);
      exceptionCaught = 1;
      code = EPIPE;
    }
    ENDTRY
    if (exceptionCaught)
	return code;

error_exit:
    lock_ReleaseShared(&(db.lock));
    return(code);
}
