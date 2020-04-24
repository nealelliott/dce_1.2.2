/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: server.h,v $
 * Revision 1.1.10.1  1996/08/09  11:58:20  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:34 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Add DM_remove definition, part of sets_lib.c optimization from Janet.
 * 	[1996/02/18  23:34:20  marty  1.1.8.2]
 *
 * Revision 1.1.8.2  1996/02/18  23:34:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:21  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  15:17:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:42  root]
 * 
 * Revision 1.1.6.9  1994/09/15  15:21:46  mccann
 * 	add prototype for ot12161
 * 	[1994/09/15  15:16:29  mccann]
 * 
 * Revision 1.1.6.8  1994/09/06  17:37:03  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:29:43  proulx]
 * 
 * 	fix timestamps
 * 
 * 	fix timestamps
 * 
 * Revision 1.1.6.7  1994/08/25  19:30:44  proulx
 * 	delegation support.
 * 	[1994/08/25  19:15:33  proulx]
 * 
 * Revision 1.1.6.6  1994/08/24  20:14:07  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:07:36  mccann]
 * 
 * Revision 1.1.6.5  1994/08/03  19:02:39  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:59:45  mccann]
 * 
 * Revision 1.1.6.4  1994/06/30  19:15:41  mccann
 * 	sams cleanup drop 2
 * 	[1994/06/30  18:01:48  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:39:29  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:10  devsrc]
 * 
 * Revision 1.1.6.2  1994/03/22  20:00:07  griffin
 * 	HCell BL3 support: HCell entry points, etc.
 * 	[1994/03/22  17:56:06  griffin]
 * 
 * Revision 1.1.6.1  1994/03/12  22:04:09  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:19:10  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:34:03  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:53:40  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:10:32  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:28:39  weisman]
 * 
 * Revision 1.1  1992/01/19  15:17:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _SERVER_H
#define _SERVER_H
/*
 * MODULE: server.h
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */

#include <dce/dce.h>
#include <dce/assert.h>
#include <dce/dce_msg.h>
#include <dce/dce_error.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <pthread.h>
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

#if defined(DCE_SEC)
# include <dce/id_base.h>
# include <dce/sec_login.h>
#endif
#include <dce/deb_ascii.h>

#ifndef _DNS_STATUS_T_
# define _DNS_STATUS_T_ 1
  typedef unsigned int dns_status_t;
#endif

/*
 * Serviceability wrappers
 */

#define CDS_SVC(fmt) DCE_SVC(cds__svc_handle,fmt),cds_svc_server

#define CDS_DBG(lvl) cds__svc_handle,cds_svc_server,svc_c_debug##lvl

/* 
 * Since unlink is defined in POSIX while not for VMS, let's make the
 * definition once here as opposed to ifdef'ing around UNLINKs many 
 * times in the code.
 */
#if _DNS_OS_ == _DNS__VMS
# define unlink(path)	remove(path)
#endif /* VMS */

typedef struct {
	bytes_u		 x[16];
} dnaUid_t; /* temporary until get decnet */

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
} DNSMgmt_State_t;

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
    dthread_mutex_t	ns_mutex;		/* lock for structure */
    DNSMgmt_State_t	ns_state;			/* Attribute  7 */
    dthread_cond_t	ns_state_changed;	/* if the above has changed */
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
    dthread_mutex_t     ns_sec_mutex;           /* lock for security state */
    /* this mutex protects the data within this #ifdef */
    CDS_sec_state_t     ns_sec_state;           /* security state */
    dthread_cond_t	ns_sec_state_changed;	/* if the above has changed */
    uuid_t              ns_Principal_uuid;       /* Principal uuid */
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
    { int myLock = dthread_trylock(&(ch_p)->ch_mutex); \
    INC_MGMT_COUNTER(&(ch_p)->acc); \
    if (myLock) (void)dthread_unlock(&(ch_p)->ch_mutex); }
    
#define INC_SRV_CTR(ns_p, acc) \
    { int myLock = dthread_trylock(&(ns_p)->ns_mutex); \
    INC_MGMT_COUNTER(&(ns_p)->acc); \
    if (myLock) (void)dthread_unlock(&(ns_p)->ns_mutex); }

 
/*
 * V2.0 Management structure for a clearinghouse entry
 */
#define MAX_FILE_SIZE 512

typedef struct alt_chcts_list
    {
    list_header_t       alt_chctses;
    Timestamp_t         alt_chcts;
    } alt_chcts_list_t;


typedef struct nsgbl_ch
    {
    list_header_t	ch_clearinghouses;
    nsgbl_ns_t		*ch_nameserver;
    dthread_mutex_t	ch_mutex;
    DNSMgmt_State_t	ch_state;		       
    dthread_cond_t	ch_state_changed;	/* if the above has changed */
    FullName_t		ch_name;			
    Timestamp_t		ch_cts;				
    ObjUID_t		ch_uid;				/* dns_Objectuid */
    SimpleName_t	ch_nsnickname;
    char		*ch_cellname_p;			/* CDS$CellName */
    FullName_t		ch_cellname;			/* opaque version */
    Timestamp_t		ch_nscts;                       /* dns_nscts */
    ObjUID_t		ch_nsuid;                       /* dns_nsobjectuid */
							
    char 		ch_filename[MAX_FILE_SIZE];
    mgmt_counter_t	ch_ReadAccesses;		/* Attribute 11 */
    mgmt_counter_t	ch_WriteAccesses;		/* Attribute 12 */
    mgmt_counter_t	ch_ReferencesReturned;		/* Attribute 13 */
    mgmt_counter_t	ch_TimesRootNotReachable;	/* Attribute 14 */
    mgmt_counter_t	ch_DataCorruptions;		/* Attribute 15 */
    mgmt_counter_t	ch_SkulkFailures;		/* Attribute 16 */
    mgmt_counter_t	ch_TimesClearinghouseEntryMissing; /* Attribute 17 */
    mgmt_counter_t	ch_UpgradesNotPossible;		/* Attribute 18 */
    mgmt_counter_t	ch_EnableCounts;		/* Attribute 20 */
    mgmt_counter_t	ch_DisableCounts;		/* Attribute 21 */
    list_header_t       ch_alt_chcts;
    } nsgbl_ch_t;
#define NS_CH_SIZ sizeof(struct nsgbl_ch)

char *deb_ascii_nsgbl_ch(
    char *const      ,
    const void *const); /* instrumentation displays ch_name */
#define	DEB_ASCII_LEN_nsgbl_ch (DEB_ASCII_LEN_ptr+DEB_ASCII_LEN_FullName+4)
#define	DEB_ASCII_BUF_nsgbl_ch(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_nsgbl_ch)
#define	LOG_ASCII_BUF_nsgbl_ch(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_nsgbl_ch)

/*                                                                
 *
 *    M E S S A G E S  (network and mailbox)
 *    continued
 */ 

/*
 * mm_tofrom field definitions
 */

#define CLIENT_TO_TA           0
#define CLIENT_FROM_TA         1
#define CLIENT_TO_BACK         2
#define CLIENT_FROM_BACK       3
#define CLIENT_CH_NAME_TO_ADDR 4
#define CLIENT_DIR_NAME_TO_UID 5
#define CLIENT_UID_TO_CH_LIST  6
#define CLIENT_NICK_TO_UID     7
#define CLIENT_UID_TO_NICK     8

/*
 * mailbox message datastructures  
 */                                            

#define MBX_NAME_LEN             20
typedef struct mbx_message
    {
    int			mm_mbx_id;
    byte_t		mm_type;
    byte_t		mm_operation;
    Timestamp_t		mm_chuid;            
    Timestamp_t		mm_dir_uid;             
    Timestamp_t		mm_update_ts;             
    byte_t		mm_element_type;             
    Timestamp_t		mm_element_uid;             
    byte_t		mm_name_len;  /* MAILBOX TO RECEIVE RESPONSE */
    byte_t		mm_name[MBX_NAME_LEN];
    Timestamp_t		mm_ns_uid;  /* namespace UID */
    SimpleName_t	mm_ns_nickname;  /* namespace nickname */
    FullName_t		mm_chname;  /* clearinghouse name */
    } mbx_message_t;

                                      

/*                                                                
 *
 *    structures used by shareable library routines
 */            
 
typedef struct user_descriptor
    {
    dthread_mutex_t	ud_mutex;		/* do we need this? */
    nsgbl_ns_t		*ud_ns_p;		/* nameserver */
    nsgbl_ch_t		*ud_ch_p;		/* clearinghouse */
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

/* server_main.c */
void
server_shutdown(
		nsgbl_ns_t *);
/*
 * dns_service_ncl.c
 */

nsgbl_ch_t *
nsgbl_ch_malloc (
    nsgbl_ns_t		*);

void
nsgbl_ch_free (
    nsgbl_ch_t          *);

nsgbl_ch_t *
nsgbl_ch_next (
    nsgbl_ns_t		*,
    nsgbl_ch_t		*);

int 
dns_initialize (
    nsgbl_ns_t     *,
    SimpleName_t   *,
    ObjUID_t       *);

int 
dns_enable (
    nsgbl_ns_t     *);


/*
 * dns_service_rpc.c
 */
void 
cdscp_server_init (
    nsgbl_ns_t  *);

/*
 * mgmt_file.c
 */

void
sys_mgmt_file_load (
    nsgbl_ns_t		*);

dns_status_t
sys_mgmt_file_add (
    nsgbl_ns_t		*,
    char		*);

dns_status_t
sys_mgmt_create_new_file (
    nsgbl_ns_t		*);

/*
 * mgmt_init.c
 */
void
mgmt_init (
    nsgbl_ns_t		*);

/*
 * server_diags.c
 */
void *
server_diag_start (
    nsgbl_ns_t		*);


void
server_diag_stop (
    void		*);

int
server_diag_check_setting (
    diag_state_t *);

char *
server_diag_setting_ascii (
    diag_state_t );

/*
 * server_globals.c
 */

void
init_global_structures (
    void);

void
get_database_version (
    VersionNumber_t	*);

void
get_directory_version (
    VersionNumber_t	*);

void
set_directory_version (
    byte_t,
    byte_t);


void
get_server_version (
    VersionNumber_t     *);

void
get_ckpt_parameters (
    unsigned long	*,
    unsigned long	*);


/*
 * rpc_dispatch.c
 */
error_status_t
server_get_client_creds (handle_t      ,
                       rpc_authz_cred_handle_t *,
                       sec_id_pac_t  *,
                       boolean32     *);

error_status_t
get_user_creds (handle_t            ,
              user_descriptor_t  *);

#if _DNS_OS_ == _DNS__UNIX
/*
 * unix_cds_net.c
 */

/* original nsi state for security bindings (formally 'BIND_PE_SITE') */
extern boolean32 orig_sec_nsi_state; 

dns_status_t
net_LocalName (
    ObjUID_t		*,
    FullName_t		*);

DBSet_t *
net_get_rpc_towers (
    void		*);

int
net_export_ch (
    nsgbl_ch_t		*);

int
net_re_export_ch (
    nsgbl_ch_t		*);

void
rpc_register_ch (
    void		*,
    nsgbl_ch_t		*);

void
rpc_unregister_ch (
    void		*,
    ObjUID_t		*);

dns_status_t
net_open (
    nsgbl_ns_t		*,
    void		**);

dns_status_t
net_close (
    void		*);

#  define net_diag_dump(a,b,c,d,e) -1
#  define net_diag_set_logging_state(a,b) -1
#  define net_diag_close_logging_file()
#  define net_diag_set_janitor_inhibit(a,b) -1
#elif _DNS_OS_ == _DNS__VMS
/*
 * vms_net.c
 */

int
net_diag_dump (
    int			,
    int			,
    void		*,
    int			(*)(),
    void		*);

/* cannot prototype, argument declared with private data structure */
int
net_diag_set_logging_state ( );

void
net_diag_close_logging_file (
    );

int
net_diag_set_janitor_inhibit (
    );

unsigned int
net_open (
    nsgbl_ns_t		*,
    unsigned char	,
    int			,
    int			,
    int			,
    int			(*)(),
    int			(*)(),
    void		**);

int
net_close (
    void		*,
    int			);

dns_status_t
net_uid_create (
    dnaUid_t		*,
    Timestamp_t		*);

dns_status_t
net_verify_tower (
    byte_t		*,
    int			);

int
net_client_node_synonym (
    void		*,
    SimpleName_t	*);

int
net_verify_node (
    void		*);

int
net_src_tower (
    void	       *,
    void           **);

int
net_LocalName (
    ObjUID_t		*,
    FullName_t		*);

int
net_local_node_connection (
    void		*);

#else
   !!! /* Network support not available for foreign _DNS_OS_ = */ _DNS_OS_
#endif

/*
 * unix_towerset_build.c
 */

int
towerset_build (
    byte_t		*);

/*
 * update_lib.c
 */

int
update_delete_build (
    Update_t		*,
    Timestamp_t		*);

int
update_build (
    Update_t		*,
    int			,
    Timestamp_t		*,
    int			,
    AttributeName_t	*,
    unsigned int	*,
    int			,
    int			,
    int			,
    byte_t		*);

int
update_dns_build (
    Update_t		   *,
    int			   ,
    Timestamp_t		   *,
    attribute_descriptor_t *,
    int			   ,
    int			   ,
    byte_t		   *);

/*
 * Global server mutex
 *
 * *** We're compiling this out for the VMS/Alpha compiler as a workaround
 * *** to how it is handling externs.    The VMS merge team mentioned
 * *** something about uid_lib, but it isn't terribly clear just what the
 * *** problem is/was.
 */

#if !(defined(__alpha) && defined(vms))
extern dthread_mutex_t dns_server_mutex;
#endif

#include <ta.h>

#define SERVER_LOCK(mutex) \
 { int thstatus;  if ((thstatus = dthread_lock(&(mutex))) < 0) { \
    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long) thstatus); } }

#define SERVER_END_LOCK(mutex) \
 { int thstatus;  if ((thstatus = dthread_unlock(&(mutex))) < 0) { \
    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long) thstatus); } }



#endif  /* #ifndef _SERVER_H */
