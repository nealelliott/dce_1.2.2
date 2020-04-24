/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bc.p.h,v $
 * Revision 1.1.77.1  1996/10/02  16:57:45  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:41  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All Rights Reserved
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/bak/bc.p.h,v 1.1.77.1 1996/10/02 16:57:45 damon Exp $ */

#include <dcedfs/tcdata.h>
#include <dce/rpc.h>
#include <dcedfs/lock.h>

#define IS_COMM_ERROR(s) ((s == rpc_s_ok) ? 0 : 1)
#define IS_PTHR_ERROR(s) ((s == 0 ) ? 0 : 1)

/*
 * Represents a host in the config database.
 */
struct bc_hostEntry {
    struct bc_hostEntry	*next;	/*Ptr to next record*/
    char *name;       /* Name space entry to which bindings assoc. */
    rpc_binding_handle_t binding; /* Corresponding binding handle */
    char *uuid;                /* uuid associated with a particular butc */
    long portOffset;		/*Port=standardPort+portOffset-allows multiple TC on a host*/
};

/*
 * Global bak program configuration information.
 */
struct bc_config {
    struct lock_data hostLock;		/*serialize access to host list*/
    struct bc_hostEntry	*tapeHosts;	/*Hosts providing the tape drives*/
    struct lock_data vsetLock;		/*serialize access to fset fams*/
    struct bc_volumeSet	*vset;		/*List of all volume sets*/
    struct lock_data dschedLock;	/*serialize access to dump scheds*/
    struct bc_dumpSchedule *dsched; 	/*Dump schedule list*/
    udbClientTextT configText[TB_NUM];  /*configuration text handles*/
};

/*
  * Central status information relating to per-opcode routine information.
  */
struct bc_opstatus {
    int	isCmdLineOpcode;    /*Is this the opcode being done for the command line?*/
    int forcedBCExit;	    /*Should program exit now?*/
    int	forcedOpcodeExit;   /*Should this opcode routine exit now?*/
};

/*
 * Representation of a Volume Set, namely the specification for a group of
 * related volumes. Each volume set has a name and a list of volume
 * descriptions, one for each line in the volumeset configuration file.
 */
struct bc_volumeSet {
    struct bc_volumeSet	*next;		/*Ptr to next volume set record*/
    char *name;				/*Volume set name*/
    struct bc_volumeEntry *ventries;	/*List of component volume entries*/
};

/*
 * Represents the name of a volume specifier in a volume set.
 */
struct bc_volumeEntry {
    struct bc_volumeEntry *next;/*Ptr to next record in list*/
    char *serverName;		/*Host name for volume spec*/
    afsNetAddr server;	   	/*Host addr for volume spec*/
    char *partname;		/*Partition pattern name*/
    long partition;		/*Partition number for volume spec*/
    char *name;			/*Volume pattern name*/
};

/*
 * Represents an individual volume to be dumped, not a collection.
 */
struct bc_volumeDump {
    struct bc_volumeDump *next;	    /*Ptr to next record*/
    afs_hyper_t vid;		    /*Volume id, or 0 if not known*/
    struct bc_volumeEntry *entry;	/*Back pointer: information about 
					  server (obsolete?) */
    char *name;		    /*Individual volume name*/
    Date date;		    /*From date (for full, incremental or whatever)*/
    Date  cloneDate;	    /* time of this volume's snapshot */
    long partition;	    /* partition containing this volume */
    afsNetAddr server;	    /* server to obtain data from */
};

/*
 * Represents a dump schedule node, representing one type of dump (e.g. the
 * safe5 daily incremental)
 */
struct bc_dumpSchedule {
    struct bc_dumpSchedule *next;	/*Ptr to next record*/
    char *name;				/*Dump sched name*/
    long period;			/*Period in minutes*/
    long periodType;			/*Qualifier on above, 
					  for exceptions like 'monthly'*/
    struct bc_dumpSchedule *parent;	/*These are built at run-time*/
    struct bc_dumpSchedule *firstChild;
    struct bc_dumpSchedule *nextSibling;
    long level;				/*Level of the dump*/
    long expDate;			/* expiration date */
    long expType;			/* absolute or relative expiration */
};

/*
 * Private: represents a queued dump/restore item.
 */
struct bc_dumpTask {
    struct bc_config *config;
    struct bc_volumeDump *volumes;
    char *dumpName;	    	/* Dump name we're doing*/
    char *volSetName;	    	/* Volume set we're dumping*/
    long flags;
    long dumpID;	    	/* Dump ID we're running*/
    int oldFlag;	    	/* if restore, are we doing it to same vol#s */
    afsNetAddr destServer;	/* destination server for restore command */
    long destPartition;		/* destination partition for restore command */
    long fromDate;		/* date from which to do a restore command */
    long parentDumpID;		/* parent dump ID */
    long dumpLevel;		/* dump level being performed */
    char *newExt;		/* new volume extension, if oldFlag is false */
    long portOffset;		/* used to derive the port of the TC */
    long expDate;		/* for dumps - expiration date */
    long expType;		/* for dumps - abs. or relative expiration */
    char dontExecute;		/* do we do the dump/restore or not? */
    char spare1;		/* spares for alignment */
    char spare2;
    char spare3;
};

#define	BC_DI_INUSE	1	/* entry in use */
#define	BC_DI_ERROR	2	/* op failed */

#define	BC_MAXSIMDUMPS	    64
#define BC_MAXPORTOFFSET    1023
#define BC_REFCOUNT	    (nonInteractiveBak?3:2)

/*
 * stack size for threads started by bak
 */
#define BC_DEFAULT_STACK_SIZE	(150*1024)

/* The lock_Status and unlock_Status routines in status.c are now replaced by
 * actual calls to lock in bak and butc. They need access to the following
 * global lock.
 */
extern osi_dlock_t statusQueueLock;

/* Prototypes */
long bc_jobNumber();
