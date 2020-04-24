/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: globals.h,v $
 * Revision 1.1.73.1  1996/10/02  17:02:47  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:25  damon]
 *
 * $EndLog$
*/
/*
 * Copyright (C) 1991, 1996 Transarc Corporation
 * All Rights Reserved
 */
/*
 * globals.h -- global configuration info for bakserver
 *
 */

#define LOGPATH         "/var/dfs/adm/"
#define	DEFAULT_LOGNAME	"BakLog"            	/* log file */
#define DEFAULT_DBPATH	"/var/dfs/backup/"	/* database dir. */
#define	DEFAULT_DBPREFIX "bkdb"			/* db name prefix */
#define DEFAULT_ADMIN_FILENAME "admin.bak"

/* debug and test flags */
#define	DF_SMALLHT		0x4	/* use small hash tables */
#define	DF_TRUNCATEDB		0x8	/* truncate database on startup */

/* Error Test --VA */
#define IS_PTHR_ERROR(status) (status == -1 ? 1 : 0)
#define IS_COMM_ERROR(status) (status == rpc_s_ok ? 0 : 1)

/* Thread Cancel Protection Macros */
#define BAK_NOCANCELS		0
#define BAK_CANCELS	 	1

struct buServerConfS
{
    /* global configuration */
    char *databaseDirectory;		/* where database is placed */
    char *databaseName;			/* database file name */
    char *databaseExtension;		/* extension (depends on ubik) */
    /* error logging */
    FILE *log;				/* log file for status/errors */
    struct ubik_dbase	*database;	/* handle for the database */
    /* debug and test */
    u_long	debugFlags;
};
typedef	struct buServerConfS	buServerConfT;
typedef	buServerConfT		*buServerConfP;

/* for synchronizing the threads dumping the database */

#define	DS_WAITING	1		/* reader/writer sleeping */
#define	DS_DONE		2		/* finished */

#define	DUMP_TTL_INC	30		/* secs. before dump times out */

typedef struct dumpSyncS
{
    osi_dlock_t ds_lock;
    long 	statusFlags;		/* 0, or 1 for dump in progress */
    int	 	pipeFid[2];		/* pipe between threads */
    pthread_t   dumper;
    pthread_t   watcher;
    struct      ubik_trans *ut;		/* dump db transaction */
    long 	timeToLive;		/* time. After this, kill the dump */
} dumpSyncT, *dumpSyncP;

#define BUDB_UBIK_N_BUFFERS	1024	/* 1024 byte buffers per transaction */
#define BUDB_UBIK_MAX_VOLS	64	/* max volumes changed/transaction */
