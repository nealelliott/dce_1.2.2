/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: server.h,v $
 * Revision 1.1.2.1  1996/10/03  20:25:33  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:20:17  arvind]
 *
 * 	OSF DCE 1.2.2 Drop 4
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:18 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:37 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	GLORIA (R1.2.2) revision.  (farrell, zee)
 * 	[1996/06/27  23:16:32  zee]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1990-1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */
/*
 * MODULE DESCRIPTION:
 *
 * Header file for CDS Server data structures and routines
 */
#ifndef _SERVER_H
#define _SERVER_H

#include <pthread.h>
#include <dce/dce.h>
#include <dce/assert.h>
#include <dce/dce_msg.h>
#include <dce/dce_error.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <stdlib.h>
#include <string.h>
#include <dns_malloc.h>
#include <dce/sys_time.h>
#include <threads_lib.h>
#include <dce/dns_record.h>
#include <string_lib.h>
#include <dce/attributes.h>
#include <dce/rpcbase.h>
#include <dce/dnsclerk.h>

#include <gdb.h>

#if defined(DCE_SEC)
# include <dce/id_base.h>
# include <dce/sec_login.h>
#endif
#include <dce/deb_ascii.h>

#ifndef _DNS_STATUS_T_
# define _DNS_STATUS_T_ 1
  typedef unsigned int dns_status_t;
#endif

/* Database constants */

#define DATABASE_VERSION_MAJOR	5
#define DATABASE_VERSION_MINOR	0

#define BLOCK_SIZE 512

/*
 * Case masks for SimpleNames and AttributeNames
 */
typedef	byte_t		AttributeNameMask_t;
typedef	bytes_u		AttributeNameMask_u[(ATTRIBNAMEMAX+7)/8];
#define LEN_AttributeNameMask(ptr) LEN_bytes(ptr,sizeof(AttributeNameMask_u))
#define SKIP_AttributeNameMask(ptr) SKIP_bytes((ptr),LEN_AttributeNameMask(ptr))
#define COPY_AttributeNameMask(from,to) \
    COPY_bytes((from),(to),LEN_AttributeNameMask(from))
#define ZERO_AttributeNameMask(ptr) ZERO_bytes((ptr),LEN_AttributeNameMask(ptr))

typedef	byte_t		SimpleNameMask_t;
typedef	bytes_u		SimpleNameMask_u[(SIMPLENAMEMAX+7)/8];
#define LEN_SimpleNameMask(ptr) LEN_bytes(ptr,sizeof(SimpleNameMask_u))
#define SKIP_SimpleNameMask(ptr) SKIP_bytes((ptr),LEN_SimpleNameMask(ptr))
#define COPY_SimpleNameMask(from,to) \
    COPY_bytes((from),(to),LEN_SimpleNameMask(from))
#define ZERO_SimpleNameMask(ptr) ZERO_bytes((ptr),LEN_SimpleNameMask(ptr))


/*
 *
 *    A T T R I B U T E   S E T   &   V A L U E   S T R U C T U R E S
 *    
 *        as stored internally in the nameserver
 */                                                    
#define	DBSET_VERSION	2

/*
 * Internal database set
 *
 * This is being migrated towards and aligned structure.
 * When aligned, the order will be important for close packing.
 * Therefore, put large aligned stuff at the beginning, and smaller
 * character stuff at the end.
 */                                                                 
typedef struct DBSet {
	unsigned int	ds_setl;	/* length of set */
	longword_u	ds_casemask;	/* Casemask for attribute name */
	Timestamp_u	ds_ts;		/* time-stamp for last change */
	byte_u		ds_flag;	/* flags */
#define	DS_present	 (1<<0)
#define	DSX_absent	 (0<<0)
	AttributeType_u	ds_type;	/* type of the attribute */
	unsigned short	ds_ocnt;	/* number of offsets */
	unsigned short	ds_hcnt;	/* number of delete holders */
	unsigned int	ds_offp[1];	/* offsets to values */
	/* followed by an array of DBSetMember_u */
} DBSet_t;                         
typedef bytes_u DBSet_u[sizeof(unsigned int)+sizeof(longword_u)+sizeof(Timestamp_u)+sizeof(byte_u)+sizeof(AttributeType_u)+2*sizeof(unsigned short)+sizeof(unsigned int)];
typedef bytes_u DBSet_overhead[sizeof(DBSet_u) - sizeof(unsigned int)];
#define NUM_DBSet(ptr) ((DBSet_t *)(ptr))->ds_ocnt
#define LEN_DBSet(ptr) ((DBSet_t *)(ptr))->ds_setl
#define SKIP_DBSet(ptr) SKIP_bytes((ptr),LEN_DBSet(ptr))
#define COPY_DBSet(from,to) COPY_bytes((from),(to),LEN_DBSet(from))
#define MOVE_DBSet(from,to) MOVE_bytes((from),(to),LEN_DBSet(from))
#define DATA_DBSet(ptr) ((NUM_DBSet_Delete(ptr)) \
    ? SKIP_bytes(DATA_DBSet_Delete(ptr), LEN_DBSet_Delete(ptr)) \
    : (byte_t *)&((DBSet_t *)(ptr))->ds_offp[NUM_DBSet(ptr)])
/* TEMPORARY */
#define INDEX_DBSet(ptr,idx) ((NUM_DBSet(ptr) <= idx) ? SKIP_DBSet(ptr) : \
    SKIP_bytes(ptr, ((DBSet_t *)(ptr))->ds_offp[idx]))

#define deb_ascii_DBSet(b,p) deb_ascii_rec(b,p,"NULL DBSet_t")
#define DEB_ASCII_LEN_DBSet DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_DBSet(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_DBSet(b) LOG_ASCII_BUF_rec(b)

/*
 * Delete holders
 *
 * When an attribute is deleted, keep a record of the fact
 * hidden in front of the first item in the DBSet.
 */
#define NUM_DBSet_Delete(ptr) ((DBSet_t *)(ptr))->ds_hcnt
#define LEN_DBSet_Delete(ptr) (NUM_DBSet_Delete(ptr)*sizeof(delete_holder_t))
#define DATA_DBSet_Delete(ptr) \
    ALIGN_PTR(&((DBSet_t *)(ptr))->ds_offp[NUM_DBSet(ptr)])

#define deb_ascii_DBSet_Delete(b,p) deb_ascii_rec(b,p,"NULL DBSet_Delete_t")
#define DEB_ASCII_LEN_DBSet_Delete DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_DBSet_Delete(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_DBSet_Delete(b) LOG_ASCII_BUF_rec(b)

/*                                                                 
 * Internal database set member
 *
 * This is being migrated towards and aligned structure.
 * When aligned, the order will be important for close packing.
 * Therefore, put large aligned stuff at the beginning, and smaller
 * character stuff at the end.
 */
typedef struct DBSetMember {
	unsigned int	dm_length;	/* length of dm_data */
	Timestamp_u	dm_ts;		/* time-stamp for last change */
	byte_u		dm_flag;	/* flags */
#define	DM_present	 UD_present
#define	DM_absent	 UD_absent
#define DM_remove        UD_present + UD_absent + 1
	AttributeType_u	dm_type;	/* Type of attribute */
	BOOLEAN_u	dm_valid;	/* Is data currently valid */
	BOOLEAN_u	dm_value_present; /* Was there any data */
	byte_u          dm_valuetype;   /* Type of data value */
	bytes_u		dm_data[1];	/* value of set member */
} DBSetMember_t;                                  
 typedef bytes_u DBSetMember_u[sizeof(unsigned int)+sizeof(Timestamp_u)+2*sizeof(BOOLEAN_u)+sizeof(AttributeType_u)+3*sizeof(byte_u)];
typedef bytes_u DBSetMember_overhead[sizeof(DBSetMember_u) - sizeof(byte_u)];
#define LEN_DBSetMemberData(ptr) ((DBSetMember_t *)(ptr))->dm_length
#define LEN_DBSetMember(ptr) DIFF_bytes(SKIP_DBSetMember(ptr), ptr)
#define SKIP_DBSetMember(ptr) SKIP_bytes(((DBSetMember_t *)(ptr))->dm_data, \
    LEN_DBSetMemberData(ptr))
#define COPY_DBSetMember(from,to) COPY_bytes((from),(to),LEN_DBSetMember(from))
#define MOVE_DBSetMember(from,to) MOVE_bytes((from),(to),LEN_DBSetMember(from))

#define deb_ascii_DBSetMember(b,p) deb_ascii_rec(b,p,"NULL DBSetMember_t")
#define DEB_ASCII_LEN_DBSetMember DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_DBSetMember(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_DBSetMember(b) LOG_ASCII_BUF_rec(b)

/* Internal operations */
#define PROPAGATE_OP	  100
#define MBX_PROP_OP       101
#define MBX_SKULK_OP      102
#define CH_ON_OP          103
#define CH_SHUT_OP        104
#define CH_OFF_OP         105
#define NS_ON_OP          106
#define NS_SHUT_OP        107
#define NS_OFF_OP         108
#define ADD_REP_OP        109
#define SOLICIT_MBX_OP   111
#define NS_DUMP_MBX_OP   112
#define DEF_DUMP_MBX_OP  113
#define LOAD_MBX_OP      114
#define CH_TIMER_OP      115
#define CLIENT_EVENT_OP  116    /* log client event to decnet eventlogger */
#define CLIENT_TRACE_OP  117    /* log client trace message to file */
#define CLIENT_CLO_TR_OP 118    /* close client trace message file */
#define CREATE_NS_OP     119
#define CREATE_CH_OP 	 120
#define DELETE_CH_OP     121
#define ADD_CH_OP        122
#define REMOVE_CH_OP	 123
#define REBUILD_NS_OP	 124
#define SHOW_NS_OP	 126
#define SHOW_CH_OP	 127

/*
 *    I M P L E M E N T A T I O N   C O N S T A N T S
 */

/* Comparison values for UIDs */
#define AFTER   1
#define BEFORE -1
#define EQUIV   0

/* Walktree function values */
#define WALKTREE_DELETE_OP		1
#define WALKTREE_CREATE_OP		2
#define WALKTREE_UPDATE_OP		3
#define WALKTREE_LOOKUP_OP		4 
#define WALKTREE_SKULK_OP		5
#define WALKTREE_LINK_OP		6

/* 
 * This generic structure is used to queue structures with next/prev pointer
 * at the beginning
 */
typedef struct list_header
{
  struct list_header *next_p;
  struct list_header *prev_p;
} list_header_t;

#define	LIST_HEADER(type_t,name) struct { type_t *next_p, *prev_p; } name
#define	LIST_ELEMENT(type_t,name) struct { type_t *next_p, *prev_p; } name
#define	LIST_FIRST(list_p) \
	((((list_header_t *)list_p)->next_p != (list_header_t *)list_p) \
	    ? ((list_header_t *)list_p)->next_p : 0)
#define	LIST_NEXT(list_p,wait_p) \
	((((list_header_t *)wait_p)->next_p != (list_header_t *)list_p) \
	    ? ((list_header_t *)wait_p)->next_p : 0)

/* Macros to manipulate generic lists */
#define	empty_list(list_p) \
	((((list_header_t *)list_p)->next_p == (list_header_t *)list_p) && \
	 (((list_header_t *)list_p)->prev_p == (list_header_t *)list_p) )

#define	init_list(list_p) \
	{ ((list_header_t *)list_p)->next_p = (list_header_t *)list_p; \
	  ((list_header_t *)list_p)->prev_p = (list_header_t *)list_p; }

#define insert_list(list_p, wait_p) \
	{ ((list_header_t *)wait_p)->prev_p = \
		((list_header_t *)list_p)->prev_p; \
	((list_header_t *)list_p)->prev_p->next_p = (list_header_t *)wait_p; \
	((list_header_t *)wait_p)->next_p = (list_header_t *)list_p; \
	((list_header_t *)list_p)->prev_p = (list_header_t *)wait_p; \
	}

#define remove_list(wait_p) \
    if (((list_header_t *)wait_p)->next_p) \
	{ ((list_header_t *)wait_p)->prev_p->next_p = \
		((list_header_t *)wait_p)->next_p; \
	((list_header_t *)wait_p)->next_p->prev_p = \
		((list_header_t *)wait_p)->prev_p; \
    	((list_header_t *)wait_p)->next_p = NULL; \
	((list_header_t *)wait_p)->prev_p = NULL; \
	}

/* prepend_list splices from_list onto front of to_list */
/* and leaves from_list header empty */
/* order of assigns is such that empty lists are OK */
#define prepend_list(to_list_p, from_list_p) \
	{ \
	(from_list_p)->prev_p->next_p = (to_list_p)->next_p; \
	(to_list_p)->next_p->prev_p = (from_list_p)->prev_p; \
	(from_list_p)->next_p->prev_p = to_list_p; \
	(to_list_p)->next_p = (from_list_p)->next_p; \
	(from_list_p)->next_p = (from_list_p)->prev_p = from_list_p; \
	}

/* append_list splices from_list onto tail of to_list */
/* and leaves from_list header empty */
/* order of assigns is such that empty lists are OK */
#define append_list(to_list_p, from_list_p) \
	{ \
	(from_list_p)->next_p->prev_p = (to_list_p)->prev_p; \
	(to_list_p)->prev_p->next_p = (from_list_p)->next_p; \
	(from_list_p)->prev_p->next_p = to_list_p; \
	(to_list_p)->prev_p = (from_list_p)->prev_p; \
	(from_list_p)->next_p = (from_list_p)->prev_p = from_list_p; \
	}

/* Determine blocks required to store bytes, round up to next block */
#define dns_roundup(len) ((len + BLOCK_SIZE - 1)/BLOCK_SIZE)

/* Generic Management State */
typedef enum {
    dns_Off = 0,		/* Not available */
    dns_Initial = 1,		/* In the process of coming up */
    dns_On = 2,			/* Running and available */
    dns_Shut = 3,		/* In the process of graceful shutdown */
    dns_Broken = 4		/* In a fatal error condition */
} CDSMgmt_State_t;

#ifdef DCE_SEC
/* State of server with respect to security initialization */
typedef enum {
  cds_sec_Off = 0,             /* uninitialized */
  cds_sec_On  = 1              /* initialized   */
} CDS_sec_state_t;
#endif

typedef	dns_status_t	DNSException_t;

/*
 * Diagnostic/Instrumentation Support
 */

/* Generic diagnostic switch settings */

typedef enum {
    diag_off = 1,
    diag_on,
    diag_inhibit,
    diag_normal,
    diag_auto,
    diag_strobe_off,
    diag_strobe_on,
    diag_strobe_normal,
    diag_strobe_auto,
    diag_force
} diag_state_t;





/*
 * V2.0 Management structure for a Nameserver 
 */
typedef struct nsgbl_ns
    {
    list_header_t	ns_clearinghouses;
    int			ns_clearinghouse_count;
    pthread_mutex_t	ns_mutex;		/* lock for structure */
    CDSMgmt_State_t	ns_state;			/* Attribute  7 */
    pthread_cond_t	ns_state_changed;	/* if the above has changed */
    utc_t 		ns_create_time;			/* Attribute  0 */
    ClassVersion_t 	ns_min_protocol;		/* Attribute  1 */
    ClassVersion_t 	ns_max_protocol;		/* Attribute  2 */
    time_quad_t		ns_ReassemblyTimer;		/* Attribute  4 */
    time_quad_t		ns_future_skew;			/* Attribute  6 */
    time_quad_t		ns_skulk_time;
    char		*ns_mgmt_directory;	/* directory for server files */
    char		*ns_diag_name_p;	/* diagnostics mailbox name */
    int                 Authenticated;          /* True if following valid */
#ifdef DCE_SEC
    pthread_mutex_t     ns_sec_mutex;           /* lock for security state */
    /* this mutex protects the data within this #ifdef */
    CDS_sec_state_t     ns_sec_state;           /* security state */
    pthread_cond_t	ns_sec_state_changed;	/* if the above has changed */
    uuid_t              ns_root_dir_uuid;       /* UUID of root directory */
    uuid_t              ns_Principal_uuid;      /* Principal uuid */
    uuid_t              ns_Realm_uuid;	        /* Cell/Realm uuid */
    uuid_t		ns_Locksmith_realm;	/* cell uuid */
    uuid_t		ns_Locksmith_princ;	/* user uuid */
    sec_login_handle_t  ns_login_context;       /* server's login context */
    int			ns_Locksmith_enabled;	/* use locksmith? */
    unsigned char       ns_Principal_Global[DNS_STR_FNAME_MAX+1];/* globalname */
    unsigned char       ns_Principal[DNS_STR_FNAME_MAX+1];/* cell relativename */
    unsigned char       ns_Realm[DNS_STR_FNAME_MAX+1]; /* Server Realm name */
    unsigned char	ns_Locksmith[DNS_STR_FNAME_MAX+1]; /* Global name */
#endif
    void		*ns_ta_id;		/* ta receiver id */
    void		*ns_advertiser;		/* advertiser data handle */
    void		*ns_loop_first;		/* active testgroup data */
    void		*ns_loop_free;		/* free testgroup data */
    mgmt_counter_t	ns_IncompatibleProtocolErrors;	/* Attribute  8 */
    mgmt_counter_t	ns_AuthenticationFailures;	/* Attribute  9 */
    mgmt_counter_t	ns_ReadAccesses;		/* Attribute 11 */
    mgmt_counter_t	ns_WriteAccesses;		/* Attribute 12 */
    mgmt_counter_t	ns_SkulksInitiated;		/* Attribute 13 */
    mgmt_counter_t	ns_SkulksCompleted;		/* Attribute 14 */
    mgmt_counter_t	ns_TimesLookupPathBroken;	/* Attribute 15 */
    mgmt_counter_t	ns_PossibleCycles;		/* Attribute 16 */
    mgmt_counter_t	ns_CrucialReplicaRemovalsBackedOut; /* Attribute 17 */
    mgmt_counter_t	ns_ChildPointerUpdateFailures;	/* Attribute 18 */
    mgmt_counter_t	ns_SecurityFailures;		/* Attribute 19 */
    rpc_binding_vector_p_t ns_bind_vector;              /* binding vectors*/
    } nsgbl_ns_t;

char *deb_ascii_nsgbl_ns(
    char *const      ,
    const void *const); /* instrumentation displays ns_server_name_p */
#define	DEB_ASCII_LEN_nsgbl_ns (DEB_ASCII_LEN_ptr+DEB_ASCII_LEN_SimpleName+4)
#define	DEB_ASCII_BUF_nsgbl_ns(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_nsgbl_ns)
#define	LOG_ASCII_BUF_nsgbl_ns(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_nsgbl_ns)

/*
 * some macros to use counting the management counters
 */
#define INC_CH_CTR(ch_p, acc) \
    { int myLock = pthread_mutex_trylock(&(ch_p)->ch_mutex); \
    INC_MGMT_COUNTER(&(ch_p)->acc); \
    if (myLock) (void)pthread_mutex_unlock(&(ch_p)->ch_mutex); }

#define INC_SRV_CTR(ns_p, acc) \
    { int myLock = pthread_mutex_trylock(&(ns_p)->ns_mutex); \
    INC_MGMT_COUNTER(&(ns_p)->acc); \
    if (myLock) (void)pthread_mutex_unlock(&(ns_p)->ns_mutex); }

 
/*
 * V2.0 Management structure for a clearinghouse entry
 */
#define MAX_FILE_SIZE 512

typedef struct alt_chcts_list
    {
    list_header_t       alt_chctses;
    Timestamp_t         alt_chcts;
    } alt_chcts_list_t;

/*
 *    Clearinghouse Database Structures and Constants
 */


/*
 * Memory resident description of file
 * There are one of these per clearinghouse, representing
 * the clearinghouse checkpoint and transaction files.
 */

typedef enum {
    file_init = 0,		/* creating, no users */
    file_loaded,		/* loaded, no users */
    file_open,			/* loaded, users allowed */
    file_rundown,		/* unloading users */
    file_badclk_repair		/* BadClock repair, no users */
} ch_state_t;

typedef enum {
    file_prop_Off = 0,		/* inactive */
    file_prop_On,		/* thread running */
    file_prop_Rundown,		/* waiting for thread to stop */
    file_prop_Stopped,		/* propagation stopped */
    file_prop_Cancel		/* waiting for thread cancel to complete */
} file_prop_state_t;

#define OP_CARDINALITY 36	/* Max number of operations (OP_*) */

typedef struct current_op_info {
    pthread_cond_t   currop_sop_completed;  /* SpecialOp completed bell */
    int              currop_count[OP_CARDINALITY]; /* Stats of each type */
    int              currop_current;        /* Count of ops running */
    int              currop_special;        /* Count of special ops running */
} current_op_info_t;


typedef struct ch_info {		    /* longword aligned */

    LIST_ELEMENT(struct ch_info, ch_queue);
					    /* List of other clearinghouses */
    nsgbl_ns_t         *ch_nameserver;
    pthread_mutex_t     ch_mutex;	    /* General structure mutex */

    /* Clearinghouse entity management */
    CDSMgmt_State_t     ch_state;	    /* Ch entity state */
    pthread_cond_t      ch_state_changed;   /* if the above has changed */
    FullName_t          ch_name;
    Timestamp_t         ch_cts;
    ObjUID_t            ch_uid;                         /* dns_Objectuid */
    SimpleName_t        ch_nsnickname;
    char               *ch_cellname_p;                 /* CDS$CellName */
    FullName_t          ch_cellname;                    /* opaque version */
    Timestamp_t         ch_nscts;                       /* dns_nscts */
    ObjUID_t            ch_nsuid;                       /* dns_nsobjectuid */
    char                ch_filename[MAX_FILE_SIZE];
    mgmt_counter_t      ch_ReadAccesses;                /* Attribute 11 */
    mgmt_counter_t      ch_WriteAccesses;               /* Attribute 12 */
    mgmt_counter_t      ch_ReferencesReturned;          /* Attribute 13 */
    mgmt_counter_t      ch_TimesRootNotReachable;       /* Attribute 14 */
    mgmt_counter_t      ch_DataCorruptions;             /* Attribute 15 */
    mgmt_counter_t      ch_SkulkFailures;               /* Attribute 16 */
    mgmt_counter_t      ch_TimesClearinghouseEntryMissing; /* Attribute 17 */
    mgmt_counter_t      ch_UpgradesNotPossible;         /* Attribute 18 */
    mgmt_counter_t      ch_EnableCounts;                /* Attribute 20 */
    mgmt_counter_t      ch_DisableCounts;               /* Attribute 21 */
    list_header_t       ch_alt_chcts;

    /* Database management */
    ch_state_t        file_state;	  /* Database state */
    unsigned int      file_users;	  /* Number of users of this struct */
    int		      file_write_count;	  /* Count of writers to file */
    pthread_cond_t    file_state_changed; /* If the above changes */
    current_op_info_t file_op_info;	  /* Operation information */

    storage_handle_t  dir_store;	  /* Handle to DB directories data */
    storage_handle_t  ns_store;		  /* Handle to DB namespace data */

    short            file_root_dir_present; /* Do we store a root replica? */

    file_prop_state_t file_prop_state;
    LIST_HEADER(struct prop_queue_element, file_prop_wait);
					    /* List of directories to propagate */
    pthread_t	     file_propagator;       /* propagate thread */

    pthread_t	     file_back_activator;   /* background activator thread */
    void            *file_back_data_p;      /* background private data */
    short	     file_back_force;  	    /* force background thread to run */
    short            file_back_inhibit;     /* inhibit background from running */
    short            file_back_once;        /* for guaranteed background pass */
    pthread_t	     file_back_thread;      /* background thread */
    pthread_cond_t   file_back_done;        /* when the background is done */
    time_quad_t	     file_back_next;	    /* next time to run background */

} ch_info_t;
#define NS_CH_SIZ sizeof(struct ch_info)

char *
deb_ascii_ch_info (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_ch_info (DEB_ASCII_LEN_ptr+DEB_ASCII_LEN_FullName+4)
#define DEB_ASCII_BUF_ch_info(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_ch_info)
#define LOG_ASCII_BUF_ch_info(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_ch_info)


/*                                                                
 *
 *    structures used by shareable library routines
 */            
 
typedef struct user_descriptor
    {
    pthread_mutex_t	ud_mutex;		/* do we need this? */
    nsgbl_ns_t		*ud_ns_p;		/* nameserver */
    ch_info_t		*ud_ch_p;		/* clearinghouse */
    ObjUID_t		*ud_chuid_p;		/* accessed clearinghouse */
    protocolVersion_t	*ud_protocol_p;		/* client clerk's version */
    byte_t		*ud_rqst_p;		/* request pointer */
    byte_t		*ud_rqst_end_p;		/* after rqst msg */
    unsigned int	ud_rqstLength;		/* length of rqst msg */
    byte_t		*ud_resp_p;		/* where to place resp */
    byte_t		*ud_resp_end_p;		/* end of resp buffer */
    unsigned int	ud_respLength;		/* length of resp msg */
    Timestamp_u		ud_transaction;		/* transaction id */
    byte_t		ud_msgType;		/* rqst/resp msgType */
    byte_t		ud_operation;		/* rqst/resp operation */
    ObjUID_u		ud_Clearinghouse;	/* accessed clearinghouse */
    byte_t		ud_protocol_version;	/* ud_protocol_p->pv_major */
#if defined(DCE_SEC)
    boolean32          ud_is_pac;              /* non-0 if ud_pac is present*/
    sec_id_pac_t       ud_pac;                 /* v1.0 PAC for convenience */
    rpc_authz_cred_handle_t ud_cred;            /* user credentials */
#endif
    } user_descriptor_t;

char *deb_ascii_user_descriptor(
    char *const       ,
    const void  *const); 
#define	DEB_ASCII_LEN_user_descriptor (DEB_ASCII_LEN_ptr+DEB_ASCII_LEN_FullName+4)
#define	DEB_ASCII_BUF_user_descriptor(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_user_descriptor)
#define	LOG_ASCII_BUF_user_descriptor(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_user_descriptor)
    
typedef struct entrylist
    {
    unsigned int el_type;
    unsigned int el_len;
    byte_t *el_value_p;
    } entrylist_t;

#define deb_ascii_entrylist(b,p) deb_ascii_rec(b,p,"NULL entrylist")
#define DEB_ASCII_LEN_entrylist DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_entrylist(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_entrylist(b) LOG_ASCII_BUF_rec(b)

/*
 *      Key to lookup up an entry in the Namespace Database
 */
typedef struct entry_key { /* NOT aligned */
   ObjUID_u     key_uuid;  /* parent uuid */
   byte_u	key_type;  /* CH, dir, object, softlink or childpointer */
   SimpleName_u	key_name;  /* Not present on directory or clearinghouse */
} entry_key_t;

/*
 * Directory key variation on above 
 *
 * This is used only in places we'd otherwise be allocating a entry_key
 * and taking up space for a simplename that won't be used
 */
typedef struct dir_key {
   ObjUID_u     key_uuid;       /* parent uuid */
   byte_u	key_type;	/* CH, dir, object, softlink or childpointer */
} dir_key_t;

/*
 * Figure length of the entry key, depending on if it's includes a simplename
 */
#define	LEN_entry_key(ptr) \
    ( (((entry_key_t *) ptr)->key_type[0] == ET_directory) || \
      (((entry_key_t *) ptr)->key_type[0] == ET_clearinghouse)) ? \
    sizeof(dir_key_t) : DIFF_bytes(SKIP_SimpleName(((entry_key_t *) ptr)->key_name), ptr)

#define	MOVE_entry_key(from,to) MOVE_bytes(from,to,LEN_entry_key(from))
#define	COPY_entry_key(from,to) COPY_bytes(from,to,LEN_entry_key(from))


/*
 * Deleted record holder when new record with same name is created
 */

typedef struct delete_holder {		/* NOT aligned */
    Timestamp_u		del_uid;	/* old entry uid */
    Timestamp_u		del_ts;  	/* timestamp of delete */
} delete_holder_t;
#define LEN_delete_holder(ptr) LEN_bytes(ptr,sizeof(delete_holder_t))
#define SKIP_delete_holder(ptr) SKIP_bytes((ptr),LEN_delete_holder(ptr))
#define COPY_delete_holder(from,to) \
    COPY_bytes((from),(to),LEN_delete_holder(from))
#define MOVE_delete_holder(from,to) \
    MOVE_bytes((from),(to),LEN_delete_holder(from))


/*
 * Directory Info
 *
 * Used by the CDBI and above layers.  It defines the structure of an entry
 * in the Directory Store.  The Directory Store is indexed by fullname and
 * directory UUID.  It contains information about skulk status, and a copy
 * of the key that will get the user to the associated directory entry in
 * the Namespace Database.
 */
typedef enum {
    dir_init = 0,
    dir_on,
    dir_rundown
} dir_state_t;


typedef struct dir_info {		    /* longword aligned */

    /* Bookkeeping for replication & skulking */
    pthread_mutex_t  dir_skulk_lock;
    int              dir_skulk_in_progress;
    pthread_cond_t   dir_skulk_completed;
    unsigned int     dir_skulk_last_status; /* Remember failures */
    ErrSet_t        *dir_skulkinfo_p;	    /* Skulk state/fail info */
    unsigned int     dir_prop_status;	    /* Reset after any propagate */
    Timestamp_t      dir_prop_time;	    /* Time of last propagate */

    /* Flag to say their are deleted entries in this directory that 
       have yet to be deleted from the database */
    int              dir_deleted_entries;

    /* Access into the namespace database */
    dir_key_t        dir_key;

    /* Fullname of directory for uuid -> fname translation */
    FullName_t       dir_fullname;
} dir_info_t;

#define LEN_dir_info(x) (sizeof(dir_info_t) - sizeof(FullName_t) + LEN_FullName(&((x)->dir_fullname)))

char *deb_ascii_dir_info ( char *const, const void *const );
#define DEB_ASCII_LEN_dir_info (DEB_ASCII_LEN_ptr_TO+DEB_ASCII_LEN_FullName)
#define DEB_ASCII_BUF_dir_info(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_dir_info)
#define LOG_ASCII_BUF_dir_info(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_dir_info)

/*
 * db_data - descriptor structure for entry data.
 *
 * Whenever we read in an entry from the database, we use this
 * descriptor to describe it.
 */
typedef struct db_data {		/* longword aligned */
    byte_t              data_type;      /* type of data described here */
    unsigned int        data_buf_len;	/* Length of data_p */
    SimpleName_t        data_name;      /* entry simple name */
    char	       *data_p;
    Timestamp_u         data_cts;	/* DNS$CTS of record for easy access */
    ObjUID_u            data_uid;	/* DNS_ObjUID of record for easy access */
    ObjUID_u            data_dir_uuid;  /* directory uuid 
					 * for type=ET_directory or 
					 *          ET_clearinghouse = data_uid
					 * otherwise, = parent dir uuid         */
} db_data_t;

#define deb_ascii_db_data(b,p) deb_ascii_rec(b,p,"NULL db_data_t")
#define DEB_ASCII_LEN_db_data DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_db_data(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_db_data(b) LOG_ASCII_BUF_rec(b)


#define SERVER_LOCK(mutex) \
 { int thstatus;  if ((thstatus = pthread_mutex_lock(&(mutex))) < 0) { \
    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long) thstatus); } }

#define SERVER_END_LOCK(mutex) \
 { int thstatus;  if ((thstatus = pthread_mutex_unlock(&(mutex))) < 0) { \
    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long) thstatus); } }

/*
 * Global server mutex 
 */
extern pthread_mutex_t dns_server_mutex;


/**************************

    Function Prototypes

**************************/

/* #include <ta_proto.h> */
#include <db_proto.h>
#include <misc_proto.h>


#endif  /* #ifndef _SERVER_H */

