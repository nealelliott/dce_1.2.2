/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: uc_child.h,v $
 * Revision 1.1.17.3  1996/02/18  23:34:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:44  marty]
 *
 * Revision 1.1.17.2  1995/12/08  15:18:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:00  root]
 * 
 * Revision 1.1.13.1  1994/10/25  16:17:34  proulx
 * 	CR#5915 - insure that a cached handle is released
 * 	if access to the clearinghouse fails with PDU_UNKNOWNCLEARINGHOUSE.
 * 	[1994/10/25  15:02:17  proulx]
 * 
 * Revision 1.1.10.5  1994/08/03  19:02:51  mccann
 * 	include file cleanup
 * 	[1994/07/19  18:01:30  mccann]
 * 
 * Revision 1.1.10.4  1994/06/23  18:30:41  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:18:52  mccann]
 * 
 * Revision 1.1.10.3  1994/06/09  18:39:43  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:25  devsrc]
 * 
 * Revision 1.1.10.2  1994/04/14  14:41:22  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:28:48  peckham]
 * 
 * Revision 1.1.10.1  1994/03/12  22:04:45  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:21:02  peckham]
 * 
 * Revision 1.1.7.1  1993/10/14  16:06:06  peckham
 * 	CR#7759: Cannot use more than 2 clearinghouses on one host.
 * 	Add confidence argument to clerk_find_ch().
 * 	Add replicaPointer argument to sendCH().
 * 	[1993/10/11  03:54:45  peckham]
 * 
 * Revision 1.1.4.3  1993/03/09  19:45:21  peckham
 * 	OT#7411: Request given to gda when other clearinghouse
 * 	available. Use union from CleFlags for flags in CHlist_t.
 * 	This will make the gda flag available to the clerk.
 * 	[1993/03/09  19:23:09  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:51:27  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:55:31  zeliff]
 * 
 * Revision 1.1.2.3  1992/04/14  20:50:50  mfox
 * 	Fixes to allow CDS to compile cleanly with RIOS ANSI C compiler
 * 	[1992/04/14  19:41:07  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:12:12  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:29:12  weisman]
 * 
 * Revision 1.1  1992/01/19  15:17:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _UC_CHILD_H
#define _UC_CHILD_H
/* MODULE:	uc_child.h
 * PROJECT:	ULTRIX DNS CLERK
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
 * Networks & Communications Software Engineering
 * 
 */

#include <time.h>
#include <threads_lib.h>
#include <dce/dns_record.h>
#include <uc_parse.h>
#include <cache.h>

#include <dce/cds_clerkserver.h>
#include <dce/rpc_cvt.h>
#ifdef DCE_SEC
#include <dce/sec_login.h>
#endif

/*
 * LOCAL DEFINITIONS
 */

/* 
 * Each output parameter requires 6 bytes of overhead.  This value is
 * used to define the maximum buffer size for the enumerate or
 * read call.  Their is a maximum of 5 paremters plus their value lenghts
 * of fromcache(4), linked(4), wholentry(1)
 * and returnclass(1).
 */
#define ENUM_READ_OVERHEAD (6*5 + 2*sizeof(int) + 2*sizeof(byte))
#define MAX_ARGS 16

/*
 * LOCAL STRUCTURES
 */

typedef struct CHlist {
  struct CHlist *ch_next;
  struct CHlist *ch_prev;
  union CleFlags_n ch_flags;
  int ch_count;			/* Count of address sets being returned */
  int last_failure;
  ObjUID_u ch_id;
  FullName_u ch_name;
  Set_u ch_set;			/* As many sets as ch_count indicates  */
} CHlist_t;
typedef bytes_u CHlist_overhead[sizeof(CHlist_t) - sizeof(FullName_u) - sizeof(Set_u)];

#define deb_ascii_CHlist(b,p) deb_ascii_rec(b,p,"NULL CHlist_t")
#define DEB_ASCII_LEN_CHlist DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_CHlist(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_CHlist(b) LOG_ASCII_BUF_rec(b)

/*
 * Progress is used to keep track of resolved and unresolved names
 */

typedef struct Progress {
    CHlist_t    *Replica;
    FullName_t	*Resolved;
    FullName_t	*Unresolved;
    Timeout_u	Timeout;
    byte_u	flags;
    char	chlist_indiscriminate;
} progress_t;

/*
 * State is used for determining status of softlink loop detection.
 */
struct State {

    int		iteration;
    FullName_t	*prec;
    FullName_t	*prec2;
};

/* 
 * These values indicate the type of operation 
 * performed at the clearinghouse.  They are used to update the
 * clearinghouses mgmt. counters and clerk counters. 
 */
#define CH_NONE 0
#define CH_read 1
#define CH_write 2
#define CH_miscl 3

struct deadnames {
  int cnt;			/* Number of tower sets following name */
  int len;			/* Current name_p buffer length */
  byte_t *name_p;		/* Fullname followed by tower sets */
};

/*
 * A clerk-wide structure
 */

typedef struct clerk {
    dthread_mutex_t	mutex;
    dthread_t		monitor_thread;
    dthread_t		listener_thread;
    enum {
	clerk_Off = 0,
	clerk_Init,
	clerk_On,
	clerk_Rundown,
	clerk_Cancel
    }			clerk_state;
    int			handler_count;	/* number of active links */
    dthread_cond_t	state_changed;
    int			listenerFd;
    struct link		*link_p;	/* list of active links */
    unsigned long	Reads;          /* Count of alloc_child */
    unsigned long	Writes;         /* Count of free_child */
    byte_t		IPprefer;
    char		socket_name[128];
} clerk_t;

#define HANDLE_REJECTED(th_p) ((th_p->taRspType == MT_Exception) && \
  (EXT32(th_p->taRspMsg_p) == PDU_UnknownClearinghouse))
#define CACHE_HANDLE(fromcache, h, th_p) \
  if (fromcache) if (!HANDLE_REJECTED(th_p)) release_cached_handle(th_p, h); \
    else free_cached_handle(th_p, h); \
  else if (!HANDLE_REJECTED(th_p)) cache_handle(th_p, h); \
    else {error_status_t castatus; rpc_binding_free(&(h), &castatus);}
#define MAX_HANDLES 10
typedef struct handle_hdr {
    struct handle_cache *next_p;        /* list of caches */
    struct handle_cache *prev_p;
} handle_hdr_t;

typedef struct handle_cache {
    handle_hdr_t     hdr;
    int              usecount;		/* count of users using this handle */
    int              killit;            /* marked for delete */
    int              UnAuth;            /* fsUnAuth from creator */
    int              TrustAll;          /* fsTrustAll from creator */
    rpc_binding_handle_t h;		/* The actual binding handle  */
#ifdef DCE_SEC
    sec_login_handle_t login_context;   /* login context cached */
#endif
} handle_cache_t;

/*
 * A per-link context structure
 */
typedef struct link {
    struct link		*next_p;
    struct link		*prev_p;
    clerk_t		*ll_clerk_p;	/* parent listener socket */
    dthread_mutex_t	ll_mutex;
    dthread_t		ll_thread;
    int			ll_fd;		/* Listener socket descriptor */
    enum {				/* listener state */
	ll_Off = 0,
	ll_Init,
	ll_On,
	ll_Rundown,
	ll_Cancel
    }			ll_state;
    int			ll_handler_count; /* number of active requests */
    struct thread	*ll_writer_p;	/* thread writing response */
    dthread_cond_t	ll_state_changed; /* when any of the above changes */
    struct thread	*ll_thread_p;	/* list of active threads */
    ucp_rqst_header	ll_reqHdr;	/*  Request header */
    byte_t		*ll_reqData_p;	/*  Request data */
    struct thread	*ll_waiter_p;
#ifdef DCE_SEC
    byte_t              *ll_opq_login_context; /*opaque login context*/
#endif
    int                 ll_binding_count;     /* cached handle count */
    handle_hdr_t        ll_binding_hdr;
} link_t;


typedef struct thread {
    struct thread	*next_p;
    struct thread	*prev_p;
    struct link		*link_p;	/* parent link */
    dthread_mutex_t	mutex;
    dthread_t		request_thread;
    ucp_rqst_header	clerkReq;	/*  Request header */
    byte_t	*clerkReqBuf_p;		/*  Request buffer */
    byte_t	*clerkArgs[MAX_ARGS];	/*  pointers to request arguments */
    byte_t	*clerkResLength_p;	/*  copy of clerkReq.resLen_p */
    byte_t	*clerkPartialRes;	/*  copy of clerkReq.partialResults */
    byte_t	*clerkFromCache_p;	/*  copy of clerkReq.fromCache_p */
    byte_t	*clerkOutLink_p;	/*  copy of clerkReq.outLink_p */
    int		clerkResLength;		/*  copy of clerkReq.resLen */
    int		clerkCacheTimeout;	/*  copy of clerkReq.cacheTimeout */
    int		clerkWait;		/*  copy of clerkReq.wait */
    byte_t	*clerkRspBuf_p;		/*  Response buffer */
    byte_t	*clerkRspMsg_p;		/*  Pointer past response header */
    byte_t	*clerkRspMsgEnd_p;	/*  end of message during build */
    dns_status_t clerkStatus;		/*  Response status */
    int         clerkRspBufLen;         /*  size of buffer for readatt  */
    progress_t	taProgress;
    byte_t	*taReqBuf_p;		/*  Request buffer */
    byte_t	*taReqMsgEnd_p;		/*  end of request message */
    byte_t	*taRspBuf_p;		/*  Response buffer */
    int		taRspBufLen;		/*  length of buffer/message */
    byte_t	*taRspMsg_p;		/*  pointer past response header */
    byte_t	*taRspMsgPtr_p;		/*  pointer to buffer during parse */
    int		(*taBuildReq)(struct thread  *); /*  request msg parameter builder */
    void	(*taParseRsp)(struct thread  *); /*  response msg parameter parser */
    int         (*taRPC)(struct thread  *,       /* RPC routine */
                         Set_t          *,
                         ObjUID_t       *);
    CleUsed_t    *cle_p;
    dthread_cond_t taRspCond;		/*  waiting for ta to respond */
    int		taRspSeq;		/* A unique number for ident */
    cds_user_t	user;			/* user name or uuid */
    ObjUID_u	lastch;			/* last ch used */
    int         DefaultTimeout;         /* Per operation default */
    byte_t	clerkDontCache;		/*  copy of clerkReq.dontCache */
    byte_t	clerkConf;		/*  copy of clerkReq.conf */
    byte_t	taOperation;		/* local copy of clerkReq.op */
    byte_t	taObj;			/* connect object type */
    byte_t      OPtype;	                /* read,write or miscl for counters */
    byte_t	taVersion;		/* Server version */
    byte_t	taRspType;		/*  Parsed response message type */
    byte_t	outLinked;              /* returned to dnsFlagStat */
    byte_t	clerkFromCache;		/* returned to dnsFlagStat */
    byte_t      level;			/* count of nested find_ch calls */
    byte_t	cache_the_dir;
    byte_t      used_cached_replica;	/* Replica set found in cache */
    byte_t      skipResolveNameCache;	/* Don't resolve the name in the cache */
    byte_t      NoReplicas;
    struct deadnames dead;
    time_local_t expire;                /* when this request expires */
#ifdef DCE_SEC
    sec_login_handle_t login_context;	/* secure login context */
#endif
} thread_t;

#define deb_ascii_thread(b,p) deb_ascii_rec(b,p,"NULL thread_t")
#define DEB_ASCII_LEN_thread DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_thread(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_thread(b) LOG_ASCII_BUF_rec(b)

/*
 * Function prototypes
 */

void
clerkAddReplica (
    thread_t	*);

void
clerkAllowClearinghouses (
    thread_t	*);

void
clerkCreateCTS (
    thread_t	*);

void
clerkCreateDirectory (
    thread_t	*);

void
clerkCreateLink (
    thread_t	*);

void
clerkCreateObject (
    thread_t	*);

void
clerkDeleteDirectory (
    thread_t	*);

void
clerkDeleteLink (
    thread_t	*);

void
clerkDeleteObject (
    thread_t	*);

void
clerkDisallowClearinghouses (
    thread_t	*);

void
clerkEnumerateAttributes (
    thread_t	*);

void
clerkEnumerateChildren (
    thread_t	*);

void
clerkEnumerateObjects (
    thread_t	*);

void
clerkEnumerateSoftLinks (
    thread_t	*);

void
clerkModifyAttribute (
    thread_t	*);

void
clerkNewEpoch (
    thread_t	*);

void
clerkRemoveReplica (
    thread_t	*);

/*
 * clerkresolvename.c
 */

void
clerkResolveName (
    thread_t	*);

int
clerk_ResolveName (
    thread_t	*,
     FullName_t	*,
     FullName_t	*);

void
clerkSkulk (
    thread_t	*);

void
clerkTestAttribute (
    thread_t	*);

void
clerkTestGroup (
    thread_t	*);

void
nsclerkCombine (
    thread_t	*);

void
nsclerkCreateChildEntry (
    thread_t	*);

void
nsclerkDeleteChildEntry (
    thread_t	*);

void
nsclerkDoUpdate (
    thread_t	*);

void
nsclerkLinkReplica (
    thread_t	*);

void
nsclerkModifyReplica (
    thread_t	*);

/*
 * clerkprogressrpc.c
 */

dns_status_t
clerkProgressRPC (
     thread_t	    *,
     Set_t	    *,
     ObjUID_t	    *,
     error_status_t (*)(rpc_binding_handle_t  , 
                        cds_Progress_t        *,
                        cds_status_t          *));


/*
 * clerkreadattribute.c
 */

byte_t *
index_att (
    AttributeContents_t *,
    int		        );

int
read_att (
    thread_t		*,
    FullName_t		*,
    int			,
    AttributeName_t	*,
    Timestamp_t		*,
    int			,
    int			,
    AttributeContents_t	*,
    int			,
    int			);

void
clerkReadAttribute (
    thread_t	*);

void
clerkReadReplica (
    thread_t	*);

byte_t *
clerkgetch (
    thread_t	*);

/*
 * clerk_find_ch.c
 */

int
clerk_find_ch (
    thread_t   *,
    FullName_t *,
    int		,
    CHlist_t   **);

/* 
 * clerk_choose_ch.c
 */

CHlist_t *
BuildCHlist (
    Set_t    *,
    thread_t *);

CHlist_t *
ChooseInitCh  (
     thread_t   *,
     FullName_t *,
     int        );

CHlist_t *
ChooseCh (
  thread_t *,
  CHlist_t **,
  int      );

CHlist_t *
ChooseMaster  (
     thread_t   *,
     FullName_t *);


/*
 * clerk_client.c
 */

int
start_client (
    clerk_t	*,
    int		);

void
kill_client (
    link_t	*);

void
kill_clients (
    clerk_t	*);

void 
QueueClearinghouse (
  thread_t   *,
  FullName_t *,
  ObjUID_t   *,
  int        ,
  Set_t      *);

/*
 * clerk_gen_msg.c
 */

void
sendToClearinghouse (
    thread_t	*,
    byte_t	*);

void
sendToReplica (
    thread_t	     *,
    ReplicaPointer_t *);

void
sendCH (
    thread_t	     *,
    byte_t	     *,
    ReplicaPointer_t *);


/*
 * clerk_init_cle.c
 */


/*
 * clerk_listener.c
 */

int
start_listener (
    clerk_t	*);

void
kill_listener (
    clerk_t	*);

thread_t *
alloc_child (
    link_t	*,
    thread_t	*);

void
free_child (
    thread_t	*);

/*
 * clerk_miscel.c
 */

int
childStripRSimple (
    FullName_t		*);

idl_void_p_t 
rpc_ss_allocate (
    idl_size_t);

void 
rpc_ss_free (
    idl_void_p_t);

int
cds_convert_rpc_status (
    error_status_t	);


/*
 * clerk_main.c
 */

extern dthread_mutex_t dns_clerk_mutex;

void
clerkGetInfo (
    thread_t	*);

/*
 * clerk_prm.c
 */

byte_t *
prm_out_value (
    const thread_t *const ,
    const void     *const );

char *
deb_ascii_prm_buffer (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_prm_buffer (3+DEB_ASCII_LEN_u_16+DEB_ASCII_LEN_ptr)
#define DEB_ASCII_BUF_prm_buffer(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_prm_buffer)
#define LOG_ASCII_BUF_prm_buffer(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_prm_buffer)


typedef	unsigned char	prm_in_t;
typedef	unsigned char	prm_inout_t;
typedef	unsigned char	prm_out_t;
typedef	unsigned char	prm_ret_t;

int
prm_in_length (
    prm_in_t	*);

int
prm_out_length (
    prm_out_t	*);

#define	prm_ret_length prm_out_length

void
prm_begin (
    thread_t	*);

void
prm_end (
    thread_t	*);

void
prm_copy (
    thread_t  *,
    prm_out_t *,
    byte_t    *,
    int       );

void
prm_out_AttributeContents (
    thread_t	*,
    prm_out_t	*,
    byte_t	*);

void
prm_out_BOOLEAN (
    thread_t	*,
    prm_out_t	*,
    byte_t	*);

void
prm_out_Timeout (
    thread_t	*,
    prm_out_t	*,
    byte_t	*);

void
prm_out_VersionNumber (
    thread_t	*,
    prm_out_t	*,
    byte_t	*);

void
prm_out_Integer (
    thread_t	*,
    prm_out_t	*,
    byte_t	*);

void
ta_begin (
    thread_t	*);

void
ta_end (
    thread_t	*);

byte_t *
ta_AttributeContents (
    thread_t	*,
    prm_out_t	*);

byte_t *
ta_BOOLEAN (
    thread_t	*,
    prm_out_t	*);

byte_t *
ta_Progress (
    thread_t	*);

byte_t *
ta_Progress_resolvedName (
    thread_t	*,
    prm_out_t	*);

byte_t
ta_ReadAttrResponse_retToClerk (
    thread_t	*);

byte_t *
ta_Set (
    thread_t	*,
    prm_out_t	*);

byte_t *
ta_Timeout (
    thread_t	*,
    prm_out_t	*);

byte_t *
ta_ObjUID (
    thread_t	*,
    prm_out_t	*);

byte_t *
ta_Timestamp (
    thread_t	*,
    prm_out_t	*);

byte_t *
ta_VersionNumber (
    thread_t	*,
    prm_out_t	*);

byte_t *
ta_WholeEntry (
    thread_t	*,
    prm_out_t	*);

/*
 * clerk_progress.c
 */

void
proc_link (
    thread_t     *,
    Progress_t   *);

int
proc_prog (
    thread_t	*,
    CHlist_t    *,
    Set_t       *,
    Progress_t  *);

/*
 * clerk_send_recv.c
 */

int
clerk_send_recv (
    thread_t	*,
    Set_t	*,
    byte_t	*);

#define Address_nowhere 0
#define Address_inarea 1
#define Address_inidp 2

int
clerk_area_idp (
    Set_t	*);

int
clerk_connected (
    thread_t *,
    ObjUID_t *);

/*
 * clerk_socket.c
 */

int
clerk_socket_open (
    const char	*name_p);

int
clerk_socket_accept (
    int		cfd);

/*
 * clerk_tree_walk.c
 */

int
buildProgress (
    thread_t	*);

void
sendToWalkTree (
    thread_t	*,
    byte_t	*);

void
rem_reps (
    CHlist_t **);

void
ClerkCounter (
     int );

/* createuid.c -> uid.h */

int
ts_new (
     Timestamp_t *);


/* clerk_bind.c */

int
get_rpc_handle (
    rpc_binding_handle_t *,
    MemberValue_t	 *,
    ObjUID_t		 *,
    thread_t	 	 *,
    int                  *);

void
maximize_rpc_error (
     thread_t       *,
     error_status_t ,
     int            *);

void
delete_cache_handle (
     link_t         *,
     handle_cache_t *);

void 
free_cached_handle (
     thread_t             *,
     rpc_binding_handle_t );

void
free_inuse_cached_handle (thread_t 	 *,
                          handle_cache_t *);

void 
release_cached_handle (
     thread_t             *,
     rpc_binding_handle_t );

int
is_handle_cached (
     thread_t *,
     uuid_t   *);

int
get_cached_handle (
     rpc_binding_handle_t *,
     uuid_t               *,
     thread_t             *);
void
cache_handle (
     thread_t             *,
     rpc_binding_handle_t );



extern int clerk_def_stk;


#endif  /* #ifndef _UC_CHILD_H */
