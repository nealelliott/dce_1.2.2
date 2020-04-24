/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cache.h,v $
 * Revision 1.1.11.2  1996/02/18  23:33:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:01  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:14:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:12  root]
 * 
 * Revision 1.1.8.10  1994/08/16  17:27:19  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Collapse locks the right way:
 * 	  replace {CA_GLOBAL,CA_CLIENT,CA_LIST} by CA_DATA_LOCK
 * 	  also collapse routines.
 * 	[1994/08/16  15:30:56  peckham]
 * 
 * Revision 1.1.8.9  1994/08/11  20:35:14  peckham
 * 	Add CA_delete_server() prototype.
 * 	[1994/08/11  18:44:32  peckham]
 * 
 * 	Add casrv.c global prototypes.
 * 	[1994/08/11  04:48:09  peckham]
 * 
 * Revision 1.1.8.8  1994/08/03  19:01:57  mccann
 * 	fix asserts
 * 	[1994/07/21  16:51:27  mccann]
 * 
 * Revision 1.1.8.7  1994/07/13  15:23:47  peckham
 * 	Collapse CA_GLOBAL CA_CLIENT CA_LIST_LOCK to one lock.
 * 	[1994/07/11  18:29:24  peckham]
 * 
 * Revision 1.1.8.6  1994/06/30  19:15:21  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:36:33  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.8.5  1994/06/23  18:30:15  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:17:16  mccann]
 * 
 * Revision 1.1.8.4  1994/06/09  18:38:31  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:57  devsrc]
 * 
 * Revision 1.1.8.3  1994/05/12  21:11:43  peckham
 * 	Remove unnecessary conditionals.
 * 	[1994/05/12  19:16:10  peckham]
 * 
 * Revision 1.1.8.2  1994/04/14  14:38:13  peckham
 * 	Remove VMS and UNIX conditionals.
 * 	Replace CA_abort() with CA_snapshot_cache().
 * 	Remove CA_ABORT* definitions.
 * 	[1994/04/11  18:14:32  peckham]
 * 
 * Revision 1.1.8.1  1994/03/12  22:02:08  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:11:38  peckham]
 * 
 * Revision 1.1.5.4  1993/03/09  19:45:07  peckham
 * 	OT#7411: Request given to gda when other clearinghouse
 * 	available. Indicate GDA clearinghouses with a flag in CleFlags.
 * 	Add a tag to the CleFlags union to allow sharing with clerk.
 * 	Reorganize flags and add several flags needed by the clerk.
 * 	Bump cache version to 2.5.
 * 	[1993/03/09  19:20:15  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  13:28:15  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:45:32  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  19:13:33  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:12:35  weisman]
 * 
 * Revision 1.1.2.2  1992/03/22  22:00:40  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:25:19  weisman]
 * 
 * Revision 1.1  1992/01/19  15:16:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CACHE_H
#define _CACHE_H
/*
 * Module: cache.h
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
 * ABSTRACT:
 *	This contains the cds cache definitions used by the Clerk and
 *	Advertiser.
 */

#include <dce/dce.h>
#include <dce/dce_msg.h>	/* must be before cache.h */
#include <dce/dcesvcmsg.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <pthread.h>

#define CA_PRIVATE static

#if defined(DCE_SEC)
# include <stdio.h>  /* for 'FILENAME_MAX' */
#endif

/* Include file for cache data structures */
#include <dce/dns_record.h>
#include <dce/dnsclerk.h>
#include <dns_malloc.h>
#include <dce/sys_time.h>
#include <dnsnormal.h>
#include <dce/utc.h>
#include <dce/rpc.h>
#include <dce/deb_ascii.h>

/* Migration definition: remove when in dnsmessage.h */
#ifndef _DNS_STATUS_T_
# define _DNS_STATUS_T_ 1
  typedef unsigned int dns_status_t;
#endif


/*
 * The VMS system service can NOT allocate writeable storage.
 * However, the advertiser can. For the system service
 * CAfixed_p is defined to be the global cache pointer in the data
 * structure pointed to by CTL$GL_DNSPTR. This allows the cache code to
 * work without modifications for VMS.
 */
extern struct CacheFixed *CAfixed_p;

# define INIT_ID 6732		/* Initial key value for IPC calls */
# define DNS_SHMID_FILE "cdscache.shmid"

/* Cache version */
#define MAJOR 2
#define MINOR 5

/*
 * Associations between access entries and links/objects are made
 * through tokens, which are the time portion of timestamps.
 */
typedef Time_t token_t;
typedef Time_u token_u;
#define EQ_token(p1,p2) EQ_Time(p1,p2)
#define NE_token(p1,p2) NE_Time(p1,p2)
#define COPY_token(from,to) COPY_Time(from,to)
#define MAKE_token(to) CA_CreateToken((token_t *)(to))

/*
 * fix these later
 */
#define deb_ascii_token(b,p) "*token*"
#define DEB_ASCII_LEN_token 0
#define DEB_ASCII_BUF_token(b)
#define LOG_ASCII_BUF_token(b)

/*
 * A per-request context structure
 */
#ifdef DCE_SEC  /* If security enabled */
  typedef byte_t *cds_user_t;  /* a user has an opaque login context */
# define deb_ascii_user(b,p) deb_ascii_seclogin(b,p)
# define DEB_ASCII_LEN_user DEB_ASCII_LEN_seclogin
# define DEB_ASCII_BUF_user(b) DEB_ASCII_BUF_seclogin(b)
# define LOG_ASCII_BUF_user(b) LOG_ASCII_BUF_seclogin(b)
#else
  typedef SimpleName_t *cds_user_t; /* else a user = name */
# define deb_ascii_user(b,p) deb_ascii_SimpleName(b,p)
# define DEB_ASCII_LEN_user DEB_ASCII_LEN_SimpleName
# define DEB_ASCII_BUF_user(b) DEB_ASCII_BUF_SimpleName(b)
# define LOG_ASCII_BUF_user(b) LOG_ASCII_BUF_SimpleName(b)
#endif

/* lock types */
/**** used in cafreelist.c calock.c ****/
#define CA_HEADER_LOCK 0	/* protect the freelist */
#define CA_DATA_LOCK 1		/* protect global data */
#define SEM_CNT CA_DATA_LOCK + 1
#define CA_ATOMIC_LOCK 2	/* disable cache checking during atomic ops */

/* Loop limit when reclaiming space */
#define MAX_RECLAIM_TRIES 10

/* Types of cache entries, used when reclaiming cache space */
enum CacheType {
  Othertype,			/* Entries not on LRU list */
  DHEntry,			/* DirEntry */
  CHEntry,			/* CleEntry */
  LHEntry,                      /* Softlinks */
  NHEntry,			/* NameEntry */
  AHEntry,			/* AttrEntry */
  VHEntry,			/* ValueEntry */
  UHEntry,			/* UserEntry */
  UAEntry,			/* UserAccessEntry */
  UGEntry			/* UserGroupEntry */

};

/*
 * First element of Hash table array elements and the free and lru lists.
 * Links are stored as offsets in CacheHeader units from the free list header
 */
typedef struct CacheLinkedList {
  int ll_flink;   /* Forward link */
  int ll_blink;   /* Backward link */
} CacheLinkedList_t;

/*
 * A cache header is included at the front of every cache entry.  It is
 * used to control free space allocation, free list pointers
 * and lru list pointers.
 */

/* Use in memory allocation to force longword alignment */
#define	PATTERN	0x5A5A5A5A
typedef union {
  struct {
    CacheLinkedList_t list;
    enum CacheType type;
    unsigned int prevsize;
    unsigned int pattern;
    unsigned int size;
    short refs;			/* (refs < 0) if on Lru, (0 < refs) if locked */
    char inuse;
  } hdr;
  long x;			/* This field is used to force alignment */
} CacheHeader_t;

#define CacheHeader_round(s) \
 ((s + sizeof(CacheHeader_t) - 1)/sizeof(CacheHeader_t))

/*
 * Macros to ease access burden.
 */
#define CacheHeader_p(p) (&((CacheHeader_t *)(p))[-1])
#define CA_refs(p) CacheHeader_p(p)->hdr.refs

#define CA_locked(p) (0 < CA_refs(p))
#define CA_lock(p) { if (CA_refs(p) < 0) CA_LruRemove(p); CA_refs(p)++; }
#define CA_unlock(p) { if (--CA_refs(p) == 0) CA_LruInsert(p); }
#define CA_copy_lock(from,to) \
 { if (CA_refs(to) < 0) CA_LruRemove(to); CA_refs(to) = CA_refs(from); }

/* Cached directory to replica set, pointed to by the DirHash array
 * in the cache header
 */
typedef struct DirEntry {
  CacheLinkedList_t dh_collision;
  ObjUID_u dh_id;              /* Directory CTS or UUID */
  FullName_u dh_name;           /* Directory fullname */
  Set_u dh_rpset;               /* Replica set */
} DirEntry_t;

/* Cached softlinks previously resolved */
typedef struct LinkEntry {
  CacheLinkedList_t lh_collision;
  utc_u lh_time;	      /* Length of time link can be cached */
  token_u lh_token;           /* For access matching */
  utc_t lh_inCache;           /* Time added to cache */
  FullName_u lh_name;         /* Name of softlink */
  FullName_u lh_target;       /* Name link points to */
} LinkEntry_t;

/* Flags used */
struct CleFlags {
  union CleFlags_n {
    unsigned int init;
    struct {
      unsigned int addressused : 1;	/* Is address for real */
      unsigned int onLAN : 1;		/* Same ethernet - set by advertiser */
      unsigned int sticky : 1;		/* not subject to LRU replacement */
      unsigned int ok : 1;		/* not known to be not good */
      unsigned int gda : 1;		/* clearinghouse of last resort */
      unsigned int :3;
      /* for use by clerk */
      unsigned int inarea : 1;		/* Same decnet area  */
      unsigned int inidp  : 1;		/* Same decnet idp */
    } flags;
  } status;
};
/* Cached clearinghouse to address set, pointed to by the CleHash array
 * in the cache header.
 */

typedef struct CleEntry {
  CacheLinkedList_t ch_collision;
  struct CleFlags ch_flags;
  unsigned long ch_writes;	/* write counter */
  unsigned long ch_reads;	/* read counter */
  unsigned long ch_miscl;	/* miscl operations count */
  uuid_t         ch_createuid;
  utc_t          ch_createtime;
  ObjUID_u ch_id;		/* Clearinghouse CTS or UUID */
  FullName_u ch_name;		/* Clearinghouse fullname */
  Set_u ch_set;			/* Clearinghouse address */
  SimpleName_u ch_mgmt_name;    /* Clearinghouse management name */
} CleEntry_t;

/* Holds data to be added to cle cache */
typedef struct CleUsed {
  struct CleUsed *next_p;
  byte_t     cnttype;
  ReplicaType_t type;
  ObjUID_t   id;
  FullName_t name;
  Set_t      address;
} CleUsed_t;

/*
 * values in cnttype (match clerk definitions)
 */
#define CA_CNT_NONE	0	/* CH_NONE */
#define CA_CNT_READ	1	/* CH_read */
#define CA_CNT_WRITE	2	/* CH_write */
#define CA_CNT_MISC	3	/* CH_miscl */

/* Cached entry names and frequently accessed attributes, pointed to
 * by the NameHash array in the cache header
 */
typedef struct NameEntry {
  CacheLinkedList_t nh_collision;
  CacheLinkedList_t nh_attributes;
  token_u nh_token;		/* Local mapping for this name */
  EntryType_u nh_type;		/* type of entry */
  FullName_u nh_name;            /* Entry name */
} NameEntry_t;

#define dns_attname_upper(n1, n2) (dns_sname_upper((SimpleName_t *)n1, (SimpleName_t *)n2))

/*
 * Cached attribute values, linked to nameentry
 * The token is a mapping of attribute name to a smaller value
 */
typedef struct ValueEntry {
  CacheLinkedList_t av_collision;
  token_u  av_token;		/* Local mapping of entry fullname */
  utc_t av_inCache;             /* Time added to cache */
  byte_t av_wholeset;		/* Is whole set cached */
  AttributeContents_u av_value;
} ValueEntry_t;

/*
 * Cached attribute names.  Each name maps to a token which will
 * be smaller in length.
 * the hash table is in the cache header.
 */
typedef struct AttrEntry {
  CacheLinkedList_t ah_collision;
  token_u  ah_token;		/* Local mapping of entry fullname */
  AttributeName_u ah_name;
} AttrEntry_t;

#define USERGROUP_HASH_LEN 23
#define USERACCESS_HASH_LEN 47
#define MAX_USER_NAME_LEN 40

/* Cached data about clients, pointed to by the UserHash array in
 * the cache header
 */
typedef struct UserEntry {
  CacheLinkedList_t uh_collision;
  /* names access is allowed to */
  int  uh_access;
  /* group/members found */
  int uh_group;
#ifdef DCE_SEC
/* Principal's profile
 * Currently, this is his login context which is represented as a filename.
 * The context is preceded by its length.
 * NOTE: this is a placeholder. It MUST be the last member of the
 * structure, as it is allocated based on the length of the profile.
*/
  bytes_u uh_profile[sizeof(field16)];/* just enough for the length prefix*/
# define deb_ascii_UserEntry(b,p) deb_ascii_user(b,p->uh_profile)
# define DEB_ASCII_LEN_UserEntry DEB_ASCII_LEN_user
# define DEB_ASCII_BUF_UserEntry(b) DEB_ASCII_BUF_user(b)
# define LOG_ASCII_BUF_UserEntry(b) LOG_ASCII_BUF_user(b)
#else
  SimpleName_u uh_sname;		           /* name */
# define deb_ascii_UserEntry(b,p) deb_ascii_user(b,p->uh_sname)
# define DEB_ASCII_LEN_UserEntry DEB_ASCII_LEN_user
# define DEB_ASCII_BUF_UserEntry(b) DEB_ASCII_BUF_user(b)
# define LOG_ASCII_BUF_UserEntry(b) LOG_ASCII_BUF_user(b)
#endif
} UserEntry_t;

/*
 * Cached names of entries a user has access to, pointed to by
 * the UserAccess array in the UserEntry
 */
typedef struct UserAccessEntry {
  CacheLinkedList_t ua_collision;
  token_u ua_token;		/* Local mapping to name and type */
} UserAccessEntry_t;

/* Cached group/membership results, pointed to by the
 * GroupHash array in the UserHashEntry
 */
typedef struct UserGroupEntry  {
  CacheLinkedList_t ug_collision;
  byte_t ug_inoutdirect;	/* if result=TRUE, where it was found */
  Timeout_u ug_timeout;		/* Length of time membership can be cached */
  utc_t ug_inCache;		/* Time added to cache */
  FullName_u ug_name;		/* Name of group */
  FullName_u ug_member;		/* Name of member */
} UserGroupEntry_t;

/* Namespace entry Flags used */
struct NseFlags {
  union {
    byte_t init;
    struct {
      unsigned int ambiguous : 1; /* TRUE if Namespace is ambiguous */
    } flags;
  } status;
};

typedef struct NameSpaceEntry {
  CacheLinkedList_t ns_collision;
  struct NseFlags ns_flags;
  ObjUID_u ns_id;		/* namespace cts or uuid*/
  SimpleName_u ns_name;		/* Namespace nickname */
} NameSpaceEntry_t;

/* Area of cache to maintain clerk state, counters and tables */
#define DIR_HASH_LEN 23
#define LINK_HASH_LEN 47
#define CLE_HASH_LEN 13
#define CA_NAME_HASH_LEN 137	/* NAME_HASH_LEN defined in dbdef.h */
#define ATTR_HASH_LEN 23
#define USER_HASH_LEN 47

#define CacheNameSize  14
extern bytes_u CacheName[CacheNameSize]; /* "DnsClerkCache" */

struct CacheFixed {
  version_t ca_version;         /* Cache version for backward compatibility */
  bytes_u ca_id[CacheNameSize];
  int   ca_size;		/* Byte count of cache */
  int   ca_total;		/* Total units available for allocation */
  int   ca_Cused;		/* Total units allocated client cache */
  int   ca_Gused;		/* Total units allocated for global cache */
  int	ca_semid;		/* number of semaphore */
  int   ca_lock;		/* Lock flags for entire cache */
  int   ca_ClerkTimeout;        /* Default timeout value */
  ClerkStates ca_state;
  int ca_protocolerrors;
  int ca_authenticationfailure;
  int ca_hits;
  int ca_bypasses;
  int ca_readoperations;
  int ca_writeoperations;
  int ca_miscoperations;
  /* The hash tables are arrays of linked lists */
  /* Global hash tables */
  int ca_dir;			/* offset to array of collision lists */
  int ca_dir_hash_len;
  int ca_link;
  int ca_link_hash_len;
  int ca_cle;
  int ca_cle_hash_len;
  /* User hash tables */
  int  ca_user;
  int ca_user_hash_len;
  int ca_name;
  int ca_name_hash_len;
  int ca_attr;
  int ca_attr_hash_len;
  int default_ns;	        /* Relative pointer to the default ns entry */
  int last_pid;			/* PID of last to lock the cache - if needed */
  int ca_celldir;		/* Relative pointer to cell dir entry */
  time_quad_t ca_lasttime;	/* Last time for cts's */
  time_quad_t ca_nexttime;	/* Last time + clock ticks */
  Node_t ca_802addr;		/* 802 address for the node (vms use only) */
  CacheLinkedList_t ca_ns;	/* List of known namespaces */
  CacheHeader_t ca_Clru;        /* List of in-use client cache blocks  */
  CacheHeader_t ca_Glru;        /* List of in-use global cache blocks  */
/* Add new locations BEFORE ca_free */
  CacheHeader_t ca_free;        /* Free memory blocks */
/* followed directly by the free pool */
};

/* System independent delete file definition*/

#define CAdelete(file)  unlink(file)

/* Convert address into an offset for cache pointers */
#define     CA_GET_OFFSET(addr) ((unsigned char *)(addr) - (unsigned char *)CAfixed_p)
#define CAUSER_GET_OFFSET(addr) ((unsigned char *)(addr) - (unsigned char *)CAuser_p)

/* Convert byte offset into an CacheLinkedList_t * */
#define     CA_GET_LLPOINTER(offset) ((CacheLinkedList_t *)((unsigned char *)CAfixed_p + offset))
#define CAUSER_GET_LLPOINTER(offset) ((CacheLinkedList_t *)((unsigned char *)CAuser_p + offset))

/* Convert byte offset into an DirEntry_t * */
#define     CA_GET_DHPOINTER(offset) ((DirEntry_t *)((unsigned char *)CAfixed_p + offset))
#define CAUSER_GET_DHPOINTER(offset) ((DirEntry_t *)((unsigned char *)CAuser_p + offset))

/* Convert byte offset into an LinkEntry_t * */
#define     CA_GET_LHPOINTER(offset) ((LinkEntry_t *)((unsigned char *)CAfixed_p + offset))
#define CAUSER_GET_LHPOINTER(offset) ((LinkEntry_t *)((unsigned char *)CAuser_p + offset))

/* Convert byte offset into an UserEntry_t * */
#define     CA_GET_UHPOINTER(offset) ((UserEntry_t *)((unsigned char *)CAfixed_p + offset))
#define CAUSER_GET_UHPOINTER(offset) ((UserEntry_t *)((unsigned char *)CAuser_p + offset))

/* Convert byte offset into an UserGroupEntry_t * */
#define     CA_GET_UGPOINTER(offset) ((UserGroupEntry_t *)((unsigned char *)CAfixed_p + offset))
#define CAUSER_GET_UGPOINTER(offset) ((UserGroupEntry_t *)((unsigned char *)CAuser_p + offset))

/* Convert byte offset into an UserAccessEntry_t * */
#define     CA_GET_UAPOINTER(offset) ((UserAccessEntry_t *)((unsigned char *)CAfixed_p + offset))
#define CAUSER_GET_UAPOINTER(offset) ((UserAccessEntry_t *)((unsigned char *)CAuser_p + offset))

/* Convert byte offset into an NameEntry_t * */
#define     CA_GET_NHPOINTER(offset) ((NameEntry_t *)((unsigned char *)CAfixed_p + offset))
#define CAUSER_GET_NHPOINTER(offset) ((NameEntry_t *)((unsigned char *)CAuser_p + offset))

/* Convert byte offset into an AttrEntry_t * */
#define     CA_GET_AHPOINTER(offset) ((AttrEntry_t *)((unsigned char *)CAfixed_p + offset))
#define CAUSER_GET_AHPOINTER(offset) ((AttrEntry_t *)((unsigned char *)CAuser_p + offset))

/* Convert byte offset into an ValueEntry_t * */
#define     CA_GET_AVPOINTER(offset) ((ValueEntry_t *)((unsigned char *)CAfixed_p + offset))
#define CAUSER_GET_AVPOINTER(offset) ((ValueEntry_t *)((unsigned char *)CAuser_p + offset))

/* Convert byte offset into an CleEntry_t * */
#define     CA_GET_CHPOINTER(offset) ((CleEntry_t *)((unsigned char *)CAfixed_p + offset))
#define CAUSER_GET_CHPOINTER(offset) ((CleEntry_t *)((unsigned char *)CAuser_p + offset))

/* Convert byte offset into a CacheHeader_t * */
#define     CA_GET_CAPOINTER(offset) ((CacheHeader_t *)((unsigned char *)CAfixed_p + offset))
#define CAUSER_GET_CAPOINTER(offset) ((CacheHeader_t *)((unsigned char *)CAuser_p + offset))

/* Convert byte offset into a NameSpaceEntry_t * */
#define     CA_GET_NSPOINTER(offset) ((NameSpaceEntry_t *)((unsigned char *)CAfixed_p + offset))
#define CAUSER_GET_NSPOINTER(offset) ((NameSpaceEntry_t *)((unsigned char *)CAuser_p + offset))

#define INIT_HASH_TABLE(entrycnt, table) { int i; for (i=0; i<entrycnt; ++i) { (table+i)->ll_flink = CA_GET_OFFSET(table + i); (table+i)->ll_blink = CA_GET_OFFSET(table + i);};}

/*
 * Global prototypes by cache module
 */

/* caaccess.c */

int
CA_LookupAccess (
  cds_user_t  ,
  token_t *);

dns_status_t
CA_InsertAccess (
  cds_user_t  ,
  token_t *);

/* caattributes.c */

AttrEntry_t *
CA_LookupAttr (
     AttributeName_t *);

dns_status_t
CAReadAttribute (
     cds_user_t          ,
     unsigned int        ,
     FullName_t          *,
     EntryType_t         *,
     AttributeName_t     *,
     Timestamp_t         *,
     int                 ,
     AttributeContents_t *,
     byte_t              *,
     int                 *,
     byte_t              *);

/* cache.c */

void
CA_CollisionInsert (
   CacheLinkedList_t *const ,
   CacheLinkedList_t *const );

void
CA_CollisionRemove (
  CacheLinkedList_t *const );

void
CA_LruInsert (
  void *);

void
CA_LruRemove (
  void *);

void
CALruMove (
  void *);

/* cacle.c */

dns_status_t
CACleSetOk (
  FullName_t *,
  int         );

CleEntry_t *
CANextCle (
    CleEntry_t *,
    int        *);

CleEntry_t *
CAFindCle (
  FullName_t *,
  int         );

dns_status_t
CARemoveClearinghouse (
  FullName_t *,
  int         );

dns_status_t
CAInsertClearinghouse (
  FullName_t      *,
  ObjUID_t        *,
  struct CleFlags *,
  int             ,
  Set_t           *,
  SimpleName_t    *);

CleEntry_t *
CALookupClearinghouse (
  FullName_t *);

CleEntry_t *
CA_LookupClearinghouse (
  FullName_t   *,
  dns_status_t *);

dns_status_t
CAClearinghouseSet (
  Set_t *);

dns_status_t
CA_ClearinghouseSet (
  Set_t *);

dns_status_t
CAClearinghousesUsed (
  CleUsed_t *);

/* cadir.c */

DirEntry_t *
CAFindDir (
  FullName_t *,
  int         );

DirEntry_t *
CAFindCell (
  void);

dns_status_t
CAInsertDirectory (
  FullName_t *,
  ObjUID_t   *,
  Set_t      *);

DirEntry_t *
CALookupDirectory (
  FullName_t *);

DirEntry_t *
CA_LookupDirectory (
  FullName_t   *,
  dns_status_t *);

dns_status_t
CARemoveReplica (
  FullName_t *,
  ObjUID_t   *);

dns_status_t
CAAddReplicas (
  FullName_t *,
  ObjUID_t   *,
  Set_t      *,
  int         );

/* cadump.c */

dns_status_t
cadump (
    int     ,
    char   *[],
    int     );

/* cafreelist.c */

void 
CA_snapshot_cache(void);

#ifdef CACHE_PARANOID
  int CA_hdr_corrupt();
  int CA_cache_corrupt();
#else
# define CA_hdr_corrupt(x) (FALSE)
# define CA_cache_corrupt() (FALSE)
#endif


void
CA_FreeListInit (
  struct CacheFixed *const ,
  int                      );

void *
CA_ClientAlloc (
  int            ,
  enum CacheType );

void *
CA_GlobalAlloc (
  int            ,
  enum CacheType );

void
CA_FreeListFree (
  void *);

void *
CA_FreeListAlloc (
  int            ,
  enum CacheType );

/* cagroup.c */

dns_status_t
CAInsertGroup (
  cds_user_t  ,
  FullName_t *,
  FullName_t *,
  int        ,
  Timeout_t  *);

int
CATestGroup (
  cds_user_t   ,
  unsigned int ,
  FullName_t   *,
  FullName_t   *,
  int direct   ,
  byte_t       *,
  Timeout_t    *,
  int          *,
  byte_t       *);

/* cahash.c */

int
CAHashDir (
  FullName_t *);

int
CAHashCle (
  FullName_t *);

int
CAHashUser (
  cds_user_t );

int
CAHashName (
  FullName_t *);

int
CAHashLink (
  FullName_t *);

int
CAHashGroup (
  FullName_t *,
  FullName_t *);

int
CAHashAttr (
  AttributeName_t *);

int
CAHashAccess (
  token_t *);

/* cainsertatt.c */

dns_status_t
CAInsertAttribute (
     cds_user_t           ,
     FullName_t          *,
     EntryType_t         *,
     AttributeName_t     *,
     Timestamp_t         *,
     AttributeContents_t *,
     int                  );

/* calink.c */

dns_status_t
CAInsertLink (
  cds_user_t  ,
  FullName_t *,
  FullName_t *,
  Timeout_t  *);

int
CA_ResolveName (
     cds_user_t   ,
     unsigned int ,
     FullName_t   *,
     FullName_t   *,
     dns_status_t *);

int
CAResolveName (
  cds_user_t   ,
  unsigned int ,
  FullName_t   *,
  FullName_t   *);

dns_status_t
CA_RemoveLink (
  FullName_t *);

/* caload.c */

dns_status_t
CAFlush (
  void);

int
CALoad (
  int size);

void
CAUnLoad (
  int id);

/*
 * calock.c
 * calock_vms.c
 */

void
CASetUp (
  void);

dns_status_t
CAInitLocks (
  void);

void
CACleanupLocks (
  void);

void
CAWriteLockCache (
  void);

void
CAWriteUnlockCache (
  void);

void
CALock (
  int num);

void
CAUnlock (
  int num);

#define CAHeaderLock() CALock(CA_HEADER_LOCK)
#define CAHeaderUnlock() CAUnlock(CA_HEADER_LOCK)
#define CALockData() CALock(CA_DATA_LOCK)
#define CAUnlockData() CAUnlock(CA_DATA_LOCK)

/* No contention on CA_ATOMIC_LOCK */
#ifdef CACHE_PARANOID
# define CAAtomicLock() CA_AtomicLock(CA_ATOMIC_LOCK)
# define CAAtomicUnlock() CA_AtomicUnlock(CA_ATOMIC_LOCK)
#else
# define CAAtomicLock()
# define CAAtomicUnlock()
#endif

#ifdef CACHE_PARANOID

 int
 CA_Locked (
    int num);

# define CA_HeaderLocked() CA_Locked(CA_HEADER_LOCK)
# define CA_DataLocked() CA_Locked(CA_DATA_LOCK)
# define CA_AtomicLocked() (CAfixed_p->ca_lock & (1<<CA_ATOMIC_LOCK))
#endif

/* camiscl.c */

int
SetsEqual (
  Set_t *,
  Set_t *);

void
CACheckArea (
  Set_t           *,
  struct CleFlags *);

void
CACheckConnectison (
  Set_t           *,
  struct CleFlags *);

void
dns_sname_upper (
  SimpleName_t *,
  SimpleName_t *);

/* caname.c */

NameEntry_t *
CA_LookupName (
  FullName_t  *,
  EntryType_t *);

NameEntry_t *
CA_InsertName (
  FullName_t   *,
  EntryType_t  *,
  dns_status_t *);

void
CADeleteName (
  FullName_t  *,
  FullName_t  *,
  EntryType_t *);

/* caremoveatt.c */

void
CARemoveAttribute (
     FullName_t      *,
     EntryType_t     *,
     AttributeName_t *);

/* casrv.c */

void *
CASysServersOpen (
    int);

int
CASysServersRead (
    void	*,
    char	*,
    char	*);

void
CASysServersWrite (
    void	*,
    char	*,
    char	*);

void
CASysServersClose (
    void	*);

void
CA_write_servers (
    void);

void
CA_delete_server (
    const char *const name_p);

/* casys.c */

int
CheckCacheVersion (
    struct CacheFixed *);

void
CASysUnmap (
  void);

void
CASysDelete (
  int );

int
CASysMap (
  int );

int
CASysFindMem (
  void);

int
CASysCreateMem (
   int );

int
CASysFindFile (
  void);

int
CASysRead (
  int);

void
CASysWrite (
  void);

int 
CAUserWrite (
  void);

int
CASysCreate (
  int);

/* casysinfo.c */

int
CAsysinfo (
    int *,
    int *,
    int *);

/* catestatt.c */

dns_status_t
CATestAttribute (
  cds_user_t      ,
  unsigned int    ,
  FullName_t      *,
  EntryType_t     *,
  AttributeName_t *,
  AtomicValue_t   *,
  int             *,
  byte_t          *);

/* catime.c */

int
CATimedOut (
  void *);

void
CASetExpiration (
  void *,
  void *);

int
CAStale (
  unsigned int ,
  utc_t        *);

dns_status_t
CACreateTimestamp (
    Timestamp_t *);

void
CA_CreateToken (
    token_t	*);

/* causer.c */

UserEntry_t *
CA_InsertUser (
  cds_user_t    ,
  dns_status_t *);

UserEntry_t *
CA_LookupUser (
 cds_user_t );

#endif  /* #ifndef _CACHE_H */
