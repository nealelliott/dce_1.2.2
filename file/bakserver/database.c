/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: database.c,v $
 * Revision 1.1.24.1  1996/10/02  16:59:42  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:12  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1996 Transarc Corporation
 * All Rights Reserved
 */
/*
 * database.c -- bak database manipulation routines
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bakserver/database.c,v 1.1.24.1 1996/10/02 16:59:42 damon Exp $")

#include <sys/types.h>
#include <netinet/in.h>
#include <dcedfs/param.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/lock.h>
#include <budb_errs.h>
#include <database.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

struct memoryDB db;			/* really allocate it here */

void db_panic (reason)
  char *reason;
{
    LogError(0, "db_panic: %s\n", reason);
    exit (-1);
}

long InitDB ()
{   long  code;

    bzero ((char *)&db, sizeof(db));
    lock_Init(&(db.lock));
    lock_ObtainWrite(&(db.lock));
    if ((code = InitDBalloc ()) ||
	(code = InitDBhash ())) {
      lock_ReleaseWrite(&(db.lock));
      return code;
    }
    lock_ReleaseWrite(&(db.lock));
    return 0;
}

/* package up seek and write into one procedure for ease of use */

/* dbwrite
 * 	write a portion of the database
 * entry:
 *	pos - offset into the database (disk address). If this is in the
 *		database header, then buff must be a ptr to a portion of
 *		the in-core header
 *	buff - the information to write
 *	len - size of the write
 * other:
 *	routines calling dbwrite and dbread should hold the db.lock for read.
 */

long dbwrite (ut, pos, buff, len)
  struct ubik_trans *ut;
  long  pos;
  char *buff;
  long  len;
{   long code;

    if (((pos < sizeof(db.h)) && (buff != (char *)&db.h + pos)) ||
	(pos >= ntohl(db.h.eofPtr))) {
	Log("dbwrite: Illegal attempt to write at location 0 or past EOF\n");
	return BUDB_IO;
    }
    code = ubik_Seek(ut, 0, pos);
    if (code)
    {
	Log("dbwrite: ubik_Seek to %d failed\n", pos);
	return code;
    }
    code = ubik_Write(ut,buff,len);
    if ( code )
	Log("dbwrite: ubik_Write failed\n");
    return code;
}

/* same thing for read */

long dbread (ut, pos, buff, len)
  struct ubik_trans *ut;
  long  pos;
  char *buff;
  long  len;
{   long code;

    if (pos >= ntohl(db.h.eofPtr))
    {
	LogError(0, "dbread: Attempt to read @%d (past EOF)\n", pos);
	return BUDB_IO;
    }
    code = ubik_Seek(ut, 0, pos);
    if (code)
    {
	Log("dbread: ubik_Seek to %d failed\n", pos);
	return code;
    }
    code = ubik_Read(ut, buff, len);
    if (code)
	Log("dbread: ubik_Read pos %d, buff %d, len %d\n", pos, buff, len);
    return code;
}

/* check that the database has been initialized.  Be careful to fail in a safe
   manner, to avoid bogusly reinitializing the db.  */

long
CheckInit (ut, db_init)
     struct ubik_trans *ut;
     int (*db_init)();		/* procedure to call if rebuilding DB */
{
    long code;
    long          iversion;
    long          tversion;

#ifdef notdef /* Removed because its aborts may leave header in bad state */
    /* Don't read header if not necessary */
    if (!ubik_CacheUpdate (ut))
    	return 0;
#endif 
    lock_ObtainWrite(&(db.lock));
    db.h.eofPtr = htonl(sizeof(db.h));	/* for sanity check in dbread */

    code = dbread(ut, 0, (char *) &db.h, sizeof(db.h));
    if (code) {
	if (code == UEOF) {
	  LogDebug(2, "CheckInit: No data base, code %ld\n", code);
	  LogError(code, "CheckInit: No data base, code %ld\n", code);
	}
	else {
	  LogDebug(2, "CheckInit: I/O Error, code %ld\n", code);
	  LogError(code, "CheckInit: I/O Error, code %ld\n", code);
	}
    }  else {
      /* version numbers read correctly */
      iversion = ntohl(db.h.version);	
      tversion = ntohl(db.h.checkVersion);
	  
      if ((iversion == CUR_BAK_DUMPDB_VERSION) &&
	  (tversion == CUR_BAK_DUMPDB_VERSION))
	{
	  db.readTime = time(0);
	  ht_Reset (&db.volName);
	  ht_Reset (&db.tapeName);
	  ht_Reset (&db.dumpName);
	  ht_Reset (&db.dumpIden);

	} else {
	  LogDebug(2,"CheckInit: DB version should be %d; Initial = %d; Terminal = %d\n",
		   CUR_BAK_DUMPDB_VERSION, iversion, tversion);
	  LogError(0,"CheckInit: DB version should be %d; Initial = %d; Terminal = %d\n",
		   CUR_BAK_DUMPDB_VERSION, iversion, tversion);
	  code = BUDB_IO;
	}
    };

    if (code == 0) {
      lock_ReleaseWrite(&(db.lock));
      return 0;
    }

    /* if here, we have no version number or the wrong version number in the
     * file
     */
    if ( (code == UEOF)
    ||   ((iversion == 0) && (tversion == 0))
       )
    {
	code = BUDB_EMPTY;
    }
    else
    	code = BUDB_IO;

    if ((db_init == 0) || (code == BUDB_IO)) {
      lock_ReleaseWrite(&(db.lock));
      return code;
    }
    LogError(0, "Error discovered in header, rebuilding.\n");
    LogDebug(1, "Error discovered in header, rebuilding.\n");

    /* try to write a good header */
    bzero((char *)&db.h,sizeof(db.h));
    db.h.version = htonl(CUR_BAK_DUMPDB_VERSION);
    db.h.checkVersion = htonl(CUR_BAK_DUMPDB_VERSION);
    db.h.lastUpdate = db.h.lastDumpId = htonl(time(0));
    db.h.eofPtr = htonl(sizeof(db.h));

    /* text ptrs cleared by bzero */
    ht_DBInit ();

    code = dbwrite(ut, 0, (char *) &db.h, sizeof(db.h));
    lock_ReleaseWrite(&(db.lock));
    if (code)
    	return BUDB_IO;		/* return the error code */

    return db_init (ut);		/* initialize the db */
}
