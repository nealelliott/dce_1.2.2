/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: dtssdef.h,v $
 * Revision 1.1.13.2  1996/02/18  23:11:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:27  marty]
 *
 * Revision 1.1.13.1  1995/12/08  18:07:07  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/26  13:06 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:23:51  root]
 * 
 * Revision 1.1.11.8  1994/08/23  20:31:53  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:56:43  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:52:25  cbrooks]
 * 
 * Revision 1.1.11.7  1994/08/16  18:12:48  cbrooks
 * 	CR 11494
 * 	[1994/08/09  18:59:56  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  14:36 UTC  pare
 * 	Merge changes from Hpdce02
 * 
 * Revision 1.1.11.6  1994/06/29  17:55:27  cbrooks
 * 	CR 11085 - add missing typedef for TPmsgStatus
 * 	[1994/06/29  17:55:16  cbrooks]
 * 
 * Revision 1.1.11.5  1994/06/28  21:46:56  cbrooks
 * 	CR11085 - duplicate definitions in dtssdef.h
 * 	[1994/06/27  14:42:35  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  20:05 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/2  1994/03/09  14:25 UTC  pare
 * 	merge back changes
 * 
 * 	HP revision /main/HPDCE01/pare_rodan/1  1994/03/09  14:21 UTC  pare
 * 	Change DCESHARED_PATH to /opt/dcelocal
 * 
 * Revision 1.1.7.4  1993/11/24  13:23:14  pare
 * 	Define DCESHARED_PATH for HP-UX 10+ from /opt/dcelocal to /opt/dce
 * 	[1993/11/24  13:22:42  pare]
 * 
 * Revision 1.1.7.3  1993/09/23  17:26:10  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/23  17:22:44  pare]
 * 
 * Revision 1.1.8.3  1993/09/15  17:46:18  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.2  1993/07/08  19:37:27  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:36:55  root]
 * 
 * Revision 1.1.7.2  1993/06/10  19:29:05  sommerfeld
 * 	Change tick adjustment rate to 4%, one part on 25, for HP-UX
 * 	[1993/04/29  18:47:06  truitt]
 * 
 * Revision 1.1.5.2  1993/04/05  17:37:06  truitt
 * 	Recreate GAMERA branch.
 * 	[1993/04/05  17:36:19  truitt]
 * 
 * Revision 1.1.5.2  1993/03/19  14:15:35  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/19  14:14:30  truitt]
 * 
 * Revision 1.1.1.8  1993/03/11  22:31:15  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.11.4  1994/05/12  15:17:13  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:25  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:29  rhw]
 * 
 * Revision 1.1.11.3  1994/04/22  18:05:05  cbrooks
 * 	look in <dce/...> for includes rather than <...>
 * 	[1994/04/22  15:26:29  cbrooks]
 * 
 * Revision 1.1.11.2  1994/04/21  22:05:07  cbrooks
 * 	CR 8353,3231
 * 	[1994/04/21  21:23:49  cbrooks]
 * 
 * 	CR 3231, 8353
 * 	[1994/04/21  20:05:15  cbrooks]
 * 
 * Revision 1.1.11.1  1994/04/07  10:22:35  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:18:21  kevins]
 * 
 * Revision 1.1.7.2  1993/06/10  19:29:05  sommerfeld
 * 	Change tick adjustment rate to 4%, one part on 25, for HP-UX
 * 	[1993/04/29  18:47:06  truitt]
 * 
 * Revision 1.1.3.3  1992/12/30  20:39:21  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:07:52  htf]
 * 
 * Revision 1.1.3.2  1992/10/30  20:17:39  smythe
 * 	added hpux tick adjustment rate of 4% for clock changes
 * 	[1992/10/30  20:15:03  smythe]
 * 
 * 	DCE 1.0.1 Release
 * 	[1992/10/30  20:01:13  smythe]
 * 
 * Revision 1.1.3.2  1992/09/29  20:44:38  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:02:35  sekhar]
 * 
 * Revision 1.1.1.7  1992/09/21  18:54:02  osfrcs
 * 	    Loading drop Dce1_0_1fnl
 * 
 * Revision 1.1  1992/01/19  15:32:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * 	Module: DTSSDEF
 */
/*
 * Copyright (c) 1989,1990,1991 by
 * Digital Equipment Corporation, Maynard, Mass.
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:    Portable DTSS implementation defined defintions and
 *		structures.
 *
 * ENVIRONMENT:	all
 *
 * AUTHORS:	Carol Frampton
 *		Distributed Systems Services (DSS) 
 * 
 * DATE:	February 15, 1989
 *	
 */


#ifndef _DTSSDEF_H
#define _DTSSDEF_H

/*
 * SNI_DTS_MERGE : The following code needs to be revisited
 */

#define  VOID  void

# include <dce/dtssdef_ultrix.h>

# include <sys/types.h>
# include <netinet/in.h>

#include <dce/utc.h>
#include <dce/dtss_mgmt_support.h>

# include <dce/nbase.h>	/* types used by RPC-based applications */

/*
 *	DEFINES
 */

#if __hpux1000p
#if !defined(DCESHARED_PATH)
# define DCESHARED_PATH "/opt/dcelocal"
#endif /* !defined(DCESHARED_PATH) */
#endif

/* DTSS flag constants */
#define DTSS_SERVICE   0x0
#define K_NUM_SERVICES 1

/* 
 * DTSS status conditions.
 */
#define DTSS_ERROR		(0)
#define DTSS_SUCCESS		(1)
#define DTSS_TIMEOUT		(2)

/* 
 * status errors coming from other facilities, returned as a status
 * value from API calls to such facility.
 */
#define K_DNS_ERROR             (3)  /* error returned by a DNS call */
#define K_DNA_ERROR             (4)  /* error retuened by a DNA call */
#define K_SYS_ERROR             (5)  /* system error */
#define K_DCE_ERROR             (6)  /* error from a call to a DCE component */


/*
 * Maximum buffer sizes
 */
#define K_MAX_TSSERVERS (25)	/* maximum number of active servers
				   which can be displayed by the CP */
#define K_MAX_TSSERVERNAMELEN (35) /* maximum server name length
				   which can be displayed by the CP */

#define K_MAX_MSGFILE_RECORD (512)     /* max. record length to message file */
#define K_MAX_OS_MSG (256)             /* max. size of OS text message */
#define K_MAX_PRINC_NAME_LEN (1024) /* max. length of server's principal name in 
			               the security context.                  */


#define K_ETHERNET_ADDR_SIZE (6)       /* # bytes in an ethernet address */
#define K_SYSID_SIZE (K_ETHERNET_ADDR_SIZE) /* # bytes in an ethernet address */
#define K_DECNET_ADDR_SIZE   (2)       /* # bytes in a DECnet address */
#define	K_IEEE_PROT_ID_SIZE  (5)       /* protocol id */
#define K_MAX_802E_MSG_SIZE (1492)     /* max size of 802E message size */
    
/*
 * Conversion factors : Units in 100 nanoseconds
 */
#define K_100NS_PER_SEC (10000000)
#define K_100NS_PER_MS  (10000)
#define K_100NS_PER_US  (10)

/*
 * One billion 10**9 as a constant
 */
#define K_ONE_BILLION (1000000000)

/*
 * Infinite inaccuracy - all 48 bits are 1's
 */
#define K_INFINITE_INACC_LO 0xFFFFFFFFUL	/* low 32 bits all 1s */
#define K_INFINITE_INACC_HI 0x0000FFFF		/* high 16 bits all 1s */

/* control prog protocol number supported
 */
#define K_CTL_PROT_VERSION      1       /* CP to TS protocol version */

/*
 * Implementation adjustment rate
 */
#ifdef __hpux
#define K_TICKADJ_RATE         25      /* 4% adjustment, 1 in 25 */
#else /* #ifdef __hpux */
#define K_TICKADJ_RATE        100      /* standard adjustemt 1%, 1 in 100 */
#endif /* #ifdef __hpux */


/*
 * ARCHITECTURAL CONSTANTS
 */

/*
 * Default inaccuracy supplied when a user supplies a time value without
 * an inacuracy.  Default = 5 seconds.
 */
#define K_DEF_MGMT_INACC_LO (5 * K_100NS_PER_SEC)
#define K_DEF_MGMT_INACC_HI 0x00000000

/*
 * Class and version of time service objects in the DNS naming service.
 */
#define K_TS_DNS_CLASS "DTSS$TimeServer"
#define K_TS_DNS_CLASS_MAJ_VERS 1
#define K_TS_DNS_CLASS_MIN_VERS 0
#define K_DNS_ATTR_CLASS "DNS$Class"

/*
 * Multicast address assigned to TS: 09-00-2B-02-01-02.
 *
 * Use the test multicast address when building test systems so
 * that the users of DTSS on the LAN do not get bad time from
 * a test system.  We choose between the test and release mcast addr 
 * at run time in TRANSPORT.C.
 */

#define K_TS_MULTICAST_RELEASE { 0x09, 0x00, 0x2b, 0x02, 0x01, 0x02 }
#define K_TS_MULTICAST_TEST    { 0x09, 0x00, 0x2b, 0x00, 0x00, 0x1F } 

/*
 * Session control number assigned to TS.
 */
#define K_TS_OBJTYPE 70


/*
 * IEEE 'SNAP SAP' assigned to TS: 08-00-2B-80-3E.
 */
#define K_TS_PROT_ID {0x08,0x00,0x2B,0x80,0x3E}	 

/*
 * Number of seconds between server cache flushes.  For the messaging-only
 * version, this is 12 hours.  For versions with RPC transport, this is 2
 * hours, because the cache does not get refilled automatically via
 * advertisements, as in the messaging version.
 */

#define K_CACHE_FLUSH_INTERVAL (2*3600)		/* 2 hours */


/* 
 * Version numbers
 * 
 * For the released product, the protocol version becomes 1.0 and the 
 * test protocol version becomes 0.5.
 */
#define K_PROT_VERSION_MAJOR      1    /* packet protocol major version # */
#define K_PROT_VERSION_MINOR      0    /* packet protocol minor version # */

#define K_TEST_PROT_VERSION_MAJOR 0    /* packet protocol major version # */
#define K_TEST_PROT_VERSION_MINOR 5    /* packet protocol minor version # */

#define K_PROT_VERSION_MINMAJOR   1    /* minimum major version supported */

#define K_BINTIME_VERSION         1    /* DTSS Binary Time Format version */

#  define K_CONFIG_FILE_NAME   "/usr/var/dss/dtss.conf"


/*
 *	TYPEDEFS
 */


/*
 * Status block for returning a generic status and an OS specific status
 */
typedef struct StatusBlk
{                 
    unsigned long status;           /* DTSS status */         
    unsigned long sysStatus;        /* O/S specific status */
} StatusBlk;

/*
 * Structure to hold 64-bit values.
 */

typedef unsigned64	Bits64;

/*
 * Structure to hold 128-bit values.
 */

typedef unsigned128       Bits128;

/*
 * Architected Configuration type for Time Service
 */
typedef enum VariantType
{
    VTserver = 0,			/* time maintainer or time provider */
    VTclerk = 1				/* time clerk */
} VariantType;

typedef enum ServerSetType
{
    SRVglobal = 0,			/* global server */
    SRVlocal = 1,			/* local server */
    SRVempty				/* used in shmem to indicate that a */
					/* server entry isn't being used */
} ServerSetType;


/*
 * DNS full name with namespace name.  The namespace name can be up to 255
 * characters, 1 for the colon seperator, 255 for the object name and 1 more
 * for the NULL terminator.
 *
 *	**** Use the DNS constant when available. ****
 */
typedef char DNSname[255+1+255+1];

/*
 * Structure to hold the node address for phase IV / Ethernet
 * SysIdType is the same union to make manipulation in the code consistent
 * since Phase IV DECnet addresses, Ethernet addresses and sys ids all
 * have the same form.
 */
typedef struct
    {
	unsigned char bytes[K_ETHERNET_ADDR_SIZE];
    } EthernetAddr;

typedef union NodeAddr
{
    EthernetAddr ethernetAddr;
    struct
    {
        unsigned char hiOrd[K_ETHERNET_ADDR_SIZE - K_DECNET_ADDR_SIZE];
        unsigned char decnetAddr[K_DECNET_ADDR_SIZE];
    } decnet48;

} NodeAddr, SysIdType;

/*
 * IEEE protocol id - SNAP SAP
 */
typedef unsigned char ProtocolId[K_IEEE_PROT_ID_SIZE];

typedef struct VersionType
{
    unsigned char  dtss_support;        /* support char  */
    unsigned short dtss_major;		/* major verson  */
    unsigned short dtss_minor;		/* minor version */ 
    unsigned short dtss_level;          /* support level */
} VersionType;

/*
 * Management version array.  Indices into a 4 character array that
 * specs. a major, minor version, ie T1.0.1.
 */
#define K_DTSS_VERSION_LETTER 'T'
#define K_VER_LET 0
#define K_VER_MAJ 1
#define K_VER_MIN 2
#define K_VER_SUB 3



/*
 * TIME DATA TYPES
 */


/*
 * UTC time in 100 naonsecond units.  This is defined in terms of Bits64
 * so we can do arithmetic involving utcs, tdfs, and inaccuracies without
 * doing explicit type conversion or memory copies into variables of the
 * appropriate types.
 */
typedef unsigned64 UTCValue;

/* 
 * Inaccuracy is also expresed as an intergral number of 100ns units.
 * All one's in the inaccuracy field denotes an inaccuracy of infinity.
 * Unknown inaccuracies or inaccuracies greater than 48-bits are regarded
 * as infinite.  It is a 48-bit integer.
 */
typedef unsigned48 Inaccuracy; 

/*
 * Define these all using the same structure tag so we can do arithmetic with
 * mixed types without having to do casting or memory copies.  This is
 * needed for the inaccuracy calculation.
 *
 * Note that UTCValue is also defined in terms of Bits64.
 */ 
typedef unsigned64	ExtTDF, ExtInacc;



/*
 * EndPointElements are used to hold the times of the servers involved in a
 * synchronization.  The end points are sorted and then the correct time
 * interval is computed using algorithm 3.10.2 in the architecture.
 */
typedef enum IntervalSize
{
    non_infinite,
    infinite
} IntervalSize;

typedef enum EndPointType 
{ 
    min, 				/* lower bound of time interval */
    max 				/* upper bound of time interval */
} EndPointType;

typedef struct EndPointElement
{
    UTCValue		value;		/* value of the endpoint if finite */
    IntervalSize	intervalSize;	/* finite or infinite interval */
    EndPointType	type;		/* min or max enpoint */
} EndPointElement;

typedef unsigned int	epoch_t;	/* Epoch nunber */



/************************************************
 *	BASE TIMES
 ************************************************
 */

/*
 * Times used in synch process which must be updated atomically
 */
typedef struct BaseTimes
{
    UTCValue Tbase;		/* time at begining of adjustment process */
				/* following the last synch */
    ExtInacc Ibase;		/* inaccuracy at time, Tbase */

    UTCValue nextLS;		/* the time of the next leap second
				   after the last synch */
} BaseTimes;


/*
 *      Clock Parameters
 */
typedef struct ClockParams 
{
    unsigned long clockResolution;    /* Software clock increment in 
					 Parts per billion, (NanoSeconds
					 per clock tick) */
    unsigned long clockDrift;         /* Clock accuracy in Parts per
				         billion */

} ClockParams;


/****************************************************
 * 	DTSS MANAGEMENT PARAMETERS
 ****************************************************
 */

/*
 * State of time service
 *
 */

/*
 * The trace flags, used by the TraceIf macro (in dtssmacros.h).
 */

#define M_TRACE_MESSAGES   ((unsigned long)( 1<<0 ))  /* network messages */
#define M_TRACE_STATES	   ((unsigned long)( 1<<1 ))  /* state machine */
#define M_TRACE_ARITH      ((unsigned long)( 1<<2 ))  /* synch arithmetic */
#define M_TRACE_CTLMSGS	   ((unsigned long)( 1<<3 ))  /* CP messages */
#define M_TRACE_GRAPH	   ((unsigned long)( 1<<4 ))  /* make inacc graph output */
#define M_TRACE_THREADS	   ((unsigned long)( 1<<5 ))  /* thread events */
#define M_TRACE_DECNET	   ((unsigned long)( 1<<6 ))  /* decnet events */
#define M_TRACE_GLOBAL     ((unsigned long)( 1<<7 ))  /* trace WAN events */
#define M_TRACE_ALL	   ((unsigned long)( 0xFFFFFFFF ))

/*
 * Time service entity characteristic attributes from architecture
 *
 * Note:
 *	All UTCValues are relative times.
 */
typedef struct MgtParams
{
    UTCValue advertiseInt;		/* the av. period between
					   advertisements */
    UTCValue checkInt;			/* the av. interval between checking
				  	   for faulty servers */

    int courierRole;			/* server's courier role */
    int	myCourierRole; 		        /* setting of this node's courier,
					   acting courier role */

    epoch_t epochNumber;		/* epoch number of server */
    UTCValue errorTolerance;		/* the max error that will be tolerated
					   before a faulty clock is reset with
					   the correct time rather than
					   drifting to the correct time */
    UTCValue maxInacc;			/* desired bound on the inaccuracy
					   that the clock would like to
					   maintain */
    int	minServers;			/* number of servers required by a
					   clerk to synch */
    int repetitions;			/* number of times an entity queries
					   a server before assuming the server
					   is no longer available */
    UTCValue LANtimeOut;		/* max time to wait for a response to a
					   given request on a LAN */
    UTCValue WANtimeOut;		/* max time to wait for a response to a
					   given request on a WAN */
    UTCValue syncHold;			/* limits the frequency at which a
					   entities can synch when it cannot
					   satisfy it's inaccuracy constraint */
    char    serverTrace;		/* tracing service states */
    char    commTrace;		        /* tracing messages */
    char    syncTrace;			/* tracing arithmetic/synchs */


    VersionType   DNAversion;		/* arch. version */
    VersionType   DECdts;		/* Dtss version */
    VersionType   timeVersion;		/* time stamp version */
    uuid_t     	  uid;			/* DTSS uid */

    utc_t         creationTime;		/* DTSS creation time */


    /*
     * Server's principal name, CDS entry name and 
     * security group name to which this server belongs.
     */
    unsigned char servPrincName[K_MAX_PRINC_NAME_LEN];
    unsigned char servEntryName[K_MAX_PRINC_NAME_LEN];
    unsigned char groupName[K_MAX_PRINC_NAME_LEN];

    VariantType variant;		/* server or clerk */

    unsigned long servType;             /* server type (Global or Local) */

/*
 * Unarchitected control program settings
 */

    int TSstate;			/* TS operating state  */
    int  hasProvider;		        /* TRUE if server and TP available */
    unsigned long traceFlags;		/* trace bits for debugging */
    int newTraceFile;		        /* TRUE to start a new trace file */
    int autoTDF;	                /* TRUE to enable auto tdf change */

} MgtParams;


/*
 * 	EVENT COUNTERS
 */
typedef Bits64 EventCounters[K_NUM_COUNTERS];	/* one counter for each event */



/*
 *	All times are in UTC.
 */
typedef struct SHMBUF
{
    unsigned long shmsize;      /* number of bytes of shared memory */
    VersionType shmver;         /* shared memory version number */

    unsigned char shmSem;       /* binary semaphore - 0 or 1 - set when the
				 * TS is writing to shmem
				 */
    unsigned char shmSerialNum;
                                /* must be 1 byte to avoid interlocked
				 * instruction - this is incremented
				 * when the TS finishes writing to
				 * shmem
				 */
    BaseTimes baseTimes;	/* buffer for times used in last synch
				 */
    NodeAddr myNodeAddr;        /* DECnet address of this node */

    SysIdType myNodeId;        /* Unique Id of this node, init'd in port init */

    EventCounters counters;     /* event counters */

    MgtParams mgtParams;      	/* current parameter settings */

    ClockParams clockParams;    /* current clock adjustment/resolution 
				   settings */
} ShmBuf;


/**************************************************** 
 * Control program data types
 ****************************************************
 */

/*
 * Control program functions.  Function codes used in the messages passed
 * from the CP to the TS.
 */
#define K_CTL_SHOW_ALL_COUNTERS    200
#define K_CTL_SHOW_ALL_CHAR        201
#define K_CTL_SHOW_ALL_STATUS      202
#define K_CTL_SHOW_GLOBAL_SERVERS  203
#define K_CTL_SHOW_LOCAL_SERVERS   204

#define K_TYPE_COURIER_ROLE       9000
#define K_TYPE_VARIANT            9001
#define K_TYPE_STATE              9002
#define K_TYPE_STRING             9003

#define K_MAX_CTL_STR             512

/*
 * The status of a user TP message.
 */
typedef enum TPmsgStatus
{
    TPmsgValid,			/* A valid message */
    TPmsgSyncSerMismatch,	/* Mismatch synchronization Serial number */
    TPmsgTypeMismatch,		/* message type mismatch */
    TPmsgInvalid,		/* An invalid message */
    TPmsgNoResponse		/* An RPC comm. failure occured (we couldn't
				   contact the TP at all) */
}   TPmsgStatus;


#endif  /* #ifndef _DTSSDEF_H */

