/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bubasics.h,v $
 * Revision 1.1.13.1  1996/10/02  17:05:21  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:14  damon]
 *
 * Revision 1.1.7.1  1994/06/09  13:52:43  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:36  annie]
 * 
 * Revision 1.1.5.5  1993/02/16  15:09:08  jaffe
 * 	Transarc delta: vijay-ot6621-bak-reset-version-change-magic-numbers 1.1
 * 	  Selected comments:
 * 
 * 	    Reset version numbers in backup.
 * 	    Change magic numbers to something different from AFS backup.
 * 	Transarc delta: vijay-ot6739-bak-readlabel-times-out 1.2
 * 	  Selected comments:
 * 
 * 	    bak readlabel command is synchronous, and returns the label name of the tape.
 * 	    The problem with this scheme is that, the tape read operation may take quite
 * 	    a while on 8mm tape drives, and the RPC that returns the tape label name times
 * 	    out in the meantime. The solution to this problem would be to make the
 * 	    readlabel operation asynchronous.
 * 	    Making readlabel asynchronous would make the readlabel RPC return after
 * 	    creating a thread to do the read label. This RPC can no longer return the
 * 	    label as a parameter. The bak status watcher monitors the read label thread
 * 	    in BUTC and reports completion of the read operation. It also prints the
 * 	    tape label to the bak console. Some operations were added to the bak read
 * 	    label routine to pick this information up from the status watcher, just in
 * 	    case we need to get a hold of the label information.
 * 	    There is an interface change to the BUTC_ReadLabel RPC. There is no change
 * 	    to the documentation though.
 * 	    changes to the status type definition to include tapename and size.
 * 	    This is the second part of this delta that fills in the remaining code to
 * 	    implement the new readlabel mechanism. Quite a few files got affected
 * 	    because the new mechanism affects not just readlabel but all other commands
 * 	    too. The nature of the change is quite small though. This delta is now
 * 	    ready to be exported.
 * 	[1993/02/15  15:21:47  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/28  14:54:18  shl
 * 	Transarc delta: vijay-ot6724-bak-resolve-host-name-issues 1.2
 * 	  Selected comments:
 * 
 * 	    There are many places in backup where host names are used. The problem here
 * 	    is which host name format to use. There are DCE format host names, Internet
 * 	    format host names, and Internet addresses (dotted notation). This delta
 * 	    makes backup accept host names in the above three formats. DCE path names
 * 	    of the form /.: are expanded. IP addresses are converted to Internet host
 * 	    names.
 * 	    The other major change this delta makes is to use afsNetAddr structure
 * 	    instead of IP addresses (long) or sockaddrs in the bakserver and butc
 * 	    interfaces.
 * 	    There are a few minor changes in this delta. While reviewing the host name
 * 	    issues, I noticed some obsolete/unused data structures, routines and files.
 * 	    These were removed. There were some minor bug fixes - readlabel does not
 * 	    initialize the returned parameters, and some format changes - the string
 * 	    "TapeHosts:" is not printed unless there are some hosts in the database.
 * 	    Host name/addr handling routines from the libvolc.a library (fts) are used
 * 	    in backup now. Also, hostutil_* functions from libafsutil.a are used in
 * 	    backup. This should avoid duplicating efforts regarding host name/addr
 * 	    conversions.
 * 	    There is one case when parsing fileset family entries where the server
 * 	    name could be the wildcard ".*". We shouldn't be trying to get the address
 * 	    of the server with this name. When we get to implementing regular expression
 * 	    strings for server names, we would have to change this code to check for
 * 	    arbitrary regular expressions rather than a simple string comparision with
 * 	    ".*".
 * 	[1993/01/27  15:44:55  shl]
 * 
 * Revision 1.1.5.3  1993/01/18  20:47:48  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  17:33:04  cjd]
 * 
 * Revision 1.1.5.2  1993/01/13  15:53:51  shl
 * 	Transarc delta: vijay-ot6663-bak-remove-obsolete-files 1.1
 * 	  Selected comments:
 * 
 * 	    Remove some obsolete files. Reorganize some of the header files which caused
 * 	    some header files to go obsolete. Handle a merge error in budb.idl and also
 * 	    in procs.c.
 * 	[1993/01/12  18:39:11  shl]
 * 
 * Revision 1.1.3.5  1992/05/05  02:13:45  mason
 * 	delta tu-ot2780-set-systemtime 1.1
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 
 * 	delta vijay-butc-compile-error 1.2
 * 	[1992/04/24  21:40:09  mason]
 * 
 * Revision 1.1.3.4  1992/04/15  19:19:52  garyf
 * 	work around for compilation warnings
 * 	leave it to transarc to remove one of the
 * 	redundant definitions
 * 	[1992/04/15  19:19:06  garyf]
 * 
 * Revision 1.1.3.3  1992/01/24  01:49:09  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:04:10  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:06:17  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:21:36  zeliff]
 * 	Revision 1.1.1.2  1992/01/22  19:31:11  devsrc
 * 	Bring in dfs6.3 sources
 * 
 * Revision 1.1  1992/01/19  02:57:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
*/
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1989
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#ifndef __BUBASIC__
#define __BUBASIC__

/* version numbers. This version information is accessed from more than
 * one module. Version information is kept for:
 *	bak 	<-> butc interface
 *	butc 	<-> butm interface
 *	bak/butc<-> budb interface
 * 	bakserver database
 *	tape format
 */

/* 
 * Version of butc <-> butm interface
 */
#define BUTM_VERSIONFLAG_1 1           		/* initial version */
#define CUR_BUTM_VERSION BUTM_VERSIONFLAG_1	/* current version */

/* 
 * Version of bak <-> butc interface 
 */
#define BUTC_VERSION_1		1		/* initial version */
#define	CUR_BUTC_VERSION	BUTC_VERSION_1	/* current version */

/* 
 * Version of butc/bak <-> bakserver interface 
 */
#define BUDB_VERSION_1		1		/* initial version */
#define	CUR_BUDB_VERSION	BUDB_VERSION_1	/* current version */

/* 
 * Version of tape format
 */
#define TAPE_VERSION_1          1
#define CUR_TAPE_VERSION        TAPE_VERSION_1

/* 
 * Version of bak dump schedules
 */
#define BAK_SCHEDULE_MAGIC       0x74327286      /* magic # for schedules */
#define BAK_SCHEDULE_VERSION_1   1                       /* initial version */
#define CUR_BAK_SCHEDULE_VERSION BAK_SCHEDULE_VERSION_1   /* current version */

/* 
 * Version of bakserver ubik database
 */
#define BAK_DUMPDB_MAGIC         0x10381646      /* magic # for dump db */
#define BAK_DUMPDB_VERSION_1     1               /* initial version */
#define CUR_BAK_DUMPDB_VERSION   BAK_DUMPDB_VERSION_1  /* current version */

/* maximums for various text strings 
 * DON'T alter these values until all disk/tape structures can be handled
 * correctly. In particular, volume names are 128.
 */
#ifndef BU_MAXNAMELEN
#define BU_MAXNAMELEN 		(128)	/* max for misc. names: eg volumes */
#endif /* BU_MAXNAMELEN */
#ifndef BU_MAXTAPELEN
#define BU_MAXTAPELEN 		(256)	/* for tape names */
#endif /* BU_MAXTAPELEN */
#ifndef BU_MAXHOSTLEN			/* should track MAXHOSTNAMELEN */
#define BU_MAXHOSTLEN 		(128)	/* for server (machine) names */
#endif /* BU_MAXHOSTLEN */
#ifndef BU_MAXTOKENLEN
#define BU_MAXTOKENLEN 		(16)	/* identifiers */
#endif /* BU_MAXTOKENLEN */
#ifndef BU_MAXUNAMELEN
#define BU_MAXUNAMELEN 		(256)	/* length of a user name */
#endif /* BU_MAXUNAMELEN */
#ifndef BU_MAXCELLLEN 
#define BU_MAXCELLLEN 		(256)	/* length of a cell name */
#endif /* BU_MAXCELLLEN */

/* proposed maximum name lengths  PA */
#ifndef BU_MAX_NAME 
#define	BU_MAX_NAME		(128)	/* misc. names */
#endif /* BU_MAX_NAME */
#ifndef BU_MAX_DUMP_PATH
#define BU_MAX_DUMP_PATH 	(256)	/* length of schedule path name */
#endif /* BU_MAX_DUMP_PATH */

/* for expiration date processing */
#define	BC_NO_EXPDATE		0	/* no expiration date */
#define	BC_ABS_EXPDATE		1	/* absolute expiration date */
#define	BC_REL_EXPDATE		2	/* relative expiration date */

/*macro which generates tape names from tapeSetName */
#define tc_MakeTapeName(name,set,seq) \
    sprintf (name, (char *)(set)->format, (seq)*(set)->a + (set)->b)

/* common structure definitions */
struct dlqlink
{
    struct dlqlink	*dlq_next;
    struct dlqlink	*dlq_prev;
    long		dlq_type;
    char *		dlq_structPtr;		/* enclosing structure */
};

typedef	struct dlqlink	dlqlinkT;
typedef	dlqlinkT	*dlqlinkP;


/* invariants */
#define	DLQ_HEAD	1

/* simple assertion - for testing, don't halt processing */

#define	DLQASSERT(condition)					\
	if ( (condition) == 0 )					\
	{							\
	    printf("DLQASSERT: %s failed\n", #condition);	\
	}

/* function typing */
extern dlqlinkP dlqFront();
extern dlqlinkP dlqUnlinkb();
extern dlqlinkP dlqUnlinkf();

/* -----------------------------
 * Status management
 * -----------------------------
 */

/* status management flags
 *
 * Key to comments:
 *	C - client side only
 *	S - server side only
 *	B - client or server
 */
 
#define STARTING	0x1     /* C; still setting up, no server task yet */
#define	ABORT_REQUEST	0x2	/* B; user requested abort */
#define	ABORT_SENT	0x4     /* C; abort sent to server (REQ cleared) */
#define	ABORT_DONE	0x8	/* S; abort complete on server */
#define	ABORT_LOCAL	0x10	/* C; abort local task if contact lost */
#define	TASK_DONE 	0x20   	/* B; task complete */
#define	SILENT		0x400	/* C; don't be verbose about termination */

/* comm status */
#define	CONTACT_LOST	0x40    /* B; contact lost */

/* errror handling */
#define	TASK_ERROR	0x80     /* S; had fatal error, will terminate */

/* general status - copied back to client for jobs status */
#define	DRIVE_WAIT	0x100	/* S; waiting for drive to become free */
#define	OPR_WAIT	0x200	/* S; waiting for operator action */
#define CALL_WAIT       0x800   /* S; waiting for callout routine completion */


struct statusS
{
  /* general info */
    dlqlinkT	link;
    unsigned32	taskId;				/* task identifier */
    unsigned32	flags;				/* as above */
    signed32	lastPolled;			/* last successful poll */
    signed32	opType;				/* type of operation */

  /* op specific stuff */
    unsigned32	nKBytes;			/* bytes xferred */
    char	volumeName[BU_MAXNAMELEN];	/* current volume (if any) */
    signed32	volsFailed;			/* # operation failures */    
    char 	tapeName[BU_MAXTAPELEN];	/* tape name */
    unsigned32  tapeSize;
    unsigned32  tapeSize_ext;

  /* bak local */
    char	taskName[64];			/* type of task */
    long	port;	
    long	jobNumber;
    long	volsTotal;			/* total # vols */
    long	refCount;			/* multiple references */
    long	defunct;			/* bad status node */
};

typedef	struct statusS	statusT;
typedef statusT		*statusP;

#endif
