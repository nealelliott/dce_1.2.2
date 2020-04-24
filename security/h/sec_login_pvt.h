/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_login_pvt.h,v $
 * Revision 1.1.22.2  1996/11/13  18:00:52  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/10/15  13:24 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/1]
 *
 * 	Remove prototype for sec_login_pvt_get_preauth.
 * 	[1996/05/24  23:15 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.22.1  1996/06/04  21:58:36  arvind
 * 	merge policy_user_to_user_flag  addition from mb_u2u
 * 	[1996/05/06  19:47 UTC  burati  /main/DCE_1.2/3]
 * 
 * 	merge u2u work
 * 	[1996/04/29  19:28 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	Merge fix for CHFts16467 from mb_mothra7
 * 	[1995/10/23  17:17 UTC  burati  /main/HPDCE02/12]
 * 
 * 	Merge fix for CHFts16546
 * 	[1995/10/04  19:15 UTC  mullan_s  /main/HPDCE02/11]
 * 
 * 	Add pac_to_epac_chain_bytes.
 * 	[1995/10/04  18:28 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/1]
 * 
 * 	Merge CHFts15409
 * 	[1995/06/05  17:26 UTC  mullan_s  /main/HPDCE02/8]
 * 
 * 	Add prototype for sec_login_pvt_open_namecache
 * 	[1995/06/05  14:35 UTC  mullan_s  /main/HPDCE02/mullan_mothra_chfts15409/1]
 * 
 * 	Merge CHFts15049
 * 	[1995/05/31  14:48 UTC  mullan_s  /main/HPDCE02/7]
 * 
 * 	Merge out.
 * 	[1995/05/30  18:14 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat3/2]
 * 
 * 	Merge
 * 	[1995/05/22  17:04 UTC  mullan_s  /main/HPDCE02/6]
 * 
 * 	Add db_mutex to sec_login_context_t.
 * 	[1995/05/22  15:44 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 * 
 * 	Add sec_login_pvt_get_krb5_kt_vno.
 * 	[1995/04/14  20:54 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat3/1]
 * 
 * 	add cache_purged and magic cookie to sec_login_context_t
 * 	[1995/04/04  21:05 UTC  cuti  /main/HPDCE02/5]
 * 
 * 	-c
 * 	[1995/04/04  17:42 UTC  cuti  /main/HPDCE02/cuti_machine_context/4]
 * 
 * 	Change magic to a pointer
 * 	[1995/04/03  14:53 UTC  cuti  /main/HPDCE02/cuti_machine_context/3]
 * 
 * 	Add magic to sec_login_context_t for checking valid handle
 * 	[1995/04/03  14:18 UTC  cuti  /main/HPDCE02/cuti_machine_context/2]
 * 
 * 	Add force_cache_remove flag to sec_login_context_t
 * 	[1995/04/01  22:03 UTC  cuti  /main/HPDCE02/cuti_machine_context/1]
 * 
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:04 UTC  mullan_s  /main/HPDCE02/4]
 * 
 * 	CHFts14138: create namecache file with secure mode
 * 	[1995/03/31  13:13 UTC  aha  /main/HPDCE02/3]
 * 
 * 	Make declaration of sec__id_free_cache fit definition
 * 	[1995/03/28  17:04 UTC  aha  /main/HPDCE02/aha_mothra4/1]
 * 
 * 	Merge unintegrated
 * 	[1994/11/17  18:19 UTC  mullan_s  /main/HPDCE02/2]
 * 
 * 	Merge from rodan
 * 	[1994/11/16  14:24 UTC  mothra  /main/HPDCE02/1]
 * 
 * 	merge sec_login PAG inheritance changes into Rodan(mainline)
 * 	[1994/05/16  19:30 UTC  rose_l  /main/HPDCE01/2]
 * 
 * 	change INHPAG define to something a bit more specific DCE_SEC_INHPAG
 * 	[1994/05/16  19:21 UTC  rose_l  /main/HPDCE01/rodan_initial/2]
 * 
 * 	add SEC_LOGIN_PAG_ENV string for PAG inheritance work...
 * 	[1994/04/21  12:31 UTC  rose_l  /main/HPDCE01/rodan_initial/1]
 * 
 * 	merge
 * 	[1994/01/26  20:01  mdf  /main/HPDCE01/1]
 * 
 * 	Loading drop DCE1_0_2FNL
 * 	[1993/05/04  20:29:02  mgm  1.1.1.5]
 * 
 * Revision 1.1.20.3  1996/02/18  23:01:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:29  marty]
 * 
 * Revision 1.1.20.2  1995/12/08  17:29:24  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/12  1995/10/23  17:17 UTC  burati
 * 	Merge fix for CHFts16467 from mb_mothra7
 * 
 * 	HP revision /main/HPDCE02/11  1995/10/04  19:15 UTC  mullan_s
 * 	Merge fix for CHFts16546
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts16496/1  1995/10/04  18:28 UTC  mullan_s
 * 	Add pac_to_epac_chain_bytes.
 * 
 * 	HP revision /main/HPDCE02/10  1995/09/08  17:26 UTC  sommerfeld
 * 	Make gen_export_token faster..
 * 	[1995/08/31  03:14 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	HP revision /main/HPDCE02/9  1995/07/11  21:16 UTC  sommerfeld
 * 	add sec_login_pvt_unset_default_context();
 * 	[1995/05/02  19:43 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts13948/1]
 * 
 * 	HP revision /main/HPDCE02/8  1995/06/05  17:26 UTC  mullan_s
 * 	Merge CHFts15409
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_chfts15409/1  1995/06/05  14:35 UTC  mullan_s
 * 	Add prototype for sec_login_pvt_open_namecache
 * 
 * 	HP revision /main/HPDCE02/7  1995/05/31  14:48 UTC  mullan_s
 * 	Merge CHFts15049
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat3/2  1995/05/30  18:14 UTC  mullan_s
 * 	Merge out.
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/22  17:04 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat9/1  1995/05/22  15:44 UTC  mullan_s
 * 	Add db_mutex to sec_login_context_t.
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat3/1  1995/04/14  20:54 UTC  mullan_s
 * 	Add sec_login_pvt_get_krb5_kt_vno.
 * 
 * 	HP revision /main/HPDCE02/5  1995/04/04  21:05 UTC  cuti
 * 	add cache_purged and magic cookie to sec_login_context_t
 * 
 * 	HP revision /main/HPDCE02/cuti_machine_context/4  1995/04/04  17:42 UTC  cuti
 * 	-c
 * 
 * 	HP revision /main/HPDCE02/cuti_machine_context/3  1995/04/03  14:53 UTC  cuti
 * 	Change magic to a pointer
 * 
 * 	HP revision /main/HPDCE02/cuti_machine_context/2  1995/04/03  14:18 UTC  cuti
 * 	Add magic to sec_login_context_t for checking valid handle
 * 
 * 	HP revision /main/HPDCE02/cuti_machine_context/1  1995/04/01  22:03 UTC  cuti
 * 	Add force_cache_remove flag to sec_login_context_t
 * 
 * 	HP revision /main/HPDCE02/4  1995/04/03  22:04 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 
 * 	HP revision /main/HPDCE02/3  1995/03/31  13:13 UTC  aha
 * 	CHFts14138: create namecache file with secure mode
 * 
 * 	HP revision /main/HPDCE02/aha_mothra4/1  1995/03/28  17:04 UTC  aha
 * 	Make declaration of sec__id_free_cache fit definition
 * 
 * 	HP revision /main/HPDCE02/2  1994/11/17  18:19 UTC  mullan_s
 * 	Merge unintegrated
 * 	[1995/12/08  16:54:20  root]
 * 
 * Revision 1.1.13.2  1994/10/25  22:18:09  mullan_s
 * 	[OT12495] Plaintext third party preauth fixes
 * 	[1994/10/25  22:17:41  mullan_s]
 * 
 * Revision 1.1.13.1  1994/10/24  18:32:55  greg
 * 	CR12159: Add a generation uuid to the login context and incorporate it
 * 	into the login context export token.  This allows the  CDS clerk to
 * 	recognize changes to the *fffffff* host machine credentials immediately.
 * 	[1994/10/24  18:06:13  greg]
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/16  14:24 UTC  mothra
 * 	Merge from rodan
 * 
 * 	HP revision /main/HPDCE01/2  1994/05/16  19:30 UTC  rose_l
 * 	merge sec_login PAG inheritance changes into Rodan(mainline)
 * 
 * 	HP revision /main/HPDCE01/rodan_initial/2  1994/05/16  19:21 UTC  rose_l
 * 	change INHPAG define to something a bit more specific DCE_SEC_INHPAG
 * 
 * 	HP revision /main/HPDCE01/rodan_initial/1  1994/04/21  12:31 UTC  rose_l
 * 	add SEC_LOGIN_PAG_ENV string for PAG inheritance work...
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  20:01  mdf
 * 	merge
 * 
 * Revision 1.1.5.2  1993/07/08  11:18:07  root
 * 	Initial King Kong branch
 * 	[1993/07/08  11:17:26  root]
 * 
 * Revision 1.1.3.4  1993/05/18  14:44:23  tmm
 * 	Back in remote authentication changes
 * 	[1993/05/18  14:41:04  tmm]
 * 
 * Revision 1.1.3.3  1993/05/14  23:20:55  tmm
 * 	Back out remote authentication changes
 * 	[1993/05/14  22:07:56  tmm]
 * 
 * Revision 1.1.1.5  1993/05/04  20:29:02  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.7.5  1994/09/16  21:51:44  sommerfeld
 * 	[OT11915] store pag in login context.
 * 	[1994/09/16  21:48:03  sommerfeld]
 * 
 * Revision 1.1.7.4  1994/06/10  15:07:15  greg
 * 	intercell fixes for DCE1.1 beta
 * 	[1994/06/03  20:49:00  greg]
 * 
 * Revision 1.1.7.3  1994/06/02  21:40:48  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:37  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.5.2  1993/07/08  11:18:07  root
 * 	Initial King Kong branch
 * 	[1993/07/08  11:17:26  root]
 * 
 * Revision 1.1.3.4  1993/05/18  14:44:23  tmm
 * 	Back in remote authentication changes
 * 	[1993/05/18  14:41:04  tmm]
 * 
 * Revision 1.1.3.3  1993/05/14  23:20:55  tmm
 * 	Back out remote authentication changes
 * 	[1993/05/14  22:07:56  tmm]
 * 
 * Revision 1.1.1.5  1993/05/04  20:29:02  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.3.7  1993/03/16  22:45:58  sommerfeld
 * 	[7144] add declaration of sec_login_pag_new_tgt().
 * 	[1993/03/16  22:33:14  sommerfeld]
 * 
 * Revision 1.1.3.6  1993/02/16  22:42:19  sommerfeld
 * 	[OT6556] Add fields to the in-core login context structure to
 * 	cache the "last seen mod time" of the ticket file.
 * 	[1993/02/09  02:37:03  sommerfeld]
 * 
 * Revision 1.1.3.5  1992/12/29  13:27:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:47:09  zeliff]
 * 
 * Revision 1.1.3.4  1992/12/15  18:17:04  burati
 * 	CR4851 New rtn sec_login_pvt_set_context prototype
 * 	[1992/12/15  17:13:26  burati]
 * 
 * Revision 1.1.3.3  1992/12/04  21:27:22  burati
 * 	CR5978 Handle local OS concept of struct passwd correctly
 * 	[1992/12/04  16:44:48  burati]
 * 
 * Revision 1.1.3.2  1992/10/07  20:29:22  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:13:35  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:47:51  burati
 * 	 Second replication code drop: bl5
 * 	 Add sec_login_pvt_create_raw_context
 * 
 * $EndLog$
 */

/* 
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1996
 * Unpublished work. All Rights Reserved.
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *     DCE Security Credential Management - Private declarations
 */

#ifndef sec_login_pvt__included
#define sec_login_pvt__included

#include <sys/errno.h>
#include <sec_pwd.h>
#include <pthread.h>

#include <krb5/base-defs.h>
#include <krb5/krb5.h>

#include <dce/rgybase.h>
#include <dce/sec_login.h>
#include <dce/oride_base.h>
#include <dce/id_base.h>

#include <dce/sec_login_file_loc.h>
#include <dce/sec_login_encode.h>

/* macro to set global unix errno and error_status_t variable for memory
* allocation failure
*/
#define MEM_FAILURE(stp) \
{ \
    extern int errno; \
    errno = ENOMEM; \
    SET_STATUS(stp, sec_login_s_no_memory); \
}

/*
 * PAG environment variable name
 */
#define SEC_LOGIN_PAG_ENV "DCE_SEC_INHPAG"

#define SEC_LOGIN_CCACHE_ENV "KRB5CCNAME"
#ifdef SNI_SVR4
        /*
         * SEC_LOGIN_ENV_NAME_LENGTH shortened since for s5, the filenames
         * were shortened by 5 characters.
         * the #define's for CREDFNAME, SEC_LOGIN_ENV_NAME_LENGTH, and
         * DATAFILE_SUFFIX all have to kept in sync with each other -- rcc
         */
#define SEC_LOGIN_ENV_NAME_LENGTH 15
#else
#define SEC_LOGIN_ENV_NAME_LENGTH 20
#endif /* SNI_SVR4 */
#define SEC_LOGIN_NO_PAG -1

/*
** Internal sec_login types
*/

/* The unix info will differ from the network info if the principal's
 * home cell is not the machine host cell (e.g., "user@foo" is logging
 * on to machine "host@bar").  This is necessary because unix id's are
 * not unique across cells, (if we used user@foo's unix ids from cell
 * foo, we might be giving them giving them local access as user@bar,
 * where user@bar is some random user in cell bar whose unix ids happen
 * to be the same as those for user@foo.
 *
 * The solution: user's from foreign cells are all given the unix
 * credentials of their home cell.  These are obtained by looking
 * up the cross-registration info for the foreign cell.  As the
 * person info only contains a uid, we use a canned gid for all foreign
 * users.
 */

typedef struct _unix_login_info {
    /* pw_name contains local cell-relative name */
    struct sec_passwd      *pwent;
    /* local OS copy of struct passwd data */
    void                   *os_pwent;
    unsigned16             num_groups;
    signed32               *groups;
} unix_login_info_t;


typedef enum _login_info_state {
    info_none,        /* no login info for context */
    info_unauth,      /* login info acquired, but not authentic */
    info_auth,        /* login info obtained with an uncertified TGT */
    info_certified    /*   "    "      "      "   a certified   TGT */
} login_info_state_t;

/* policy flag bitset */
typedef  unsigned32  policy_flags_t;
#define  policy_passwd_valid   0x1   /* does the passwd need to be changed */
#define  policy_acct_valid     0x2   /* is the acct valid for login */
#define  policy_user_to_user   0x4   /* server must use user-to-user protocol*/

typedef struct _policy_info {
    sec_timeval_sec_t  acct_exp;      /* account expiration date */
    sec_timeval_sec_t  passwd_exp;    /* passwd expiration date */
    sec_timeval_sec_t  identity_exp;  /* identity expiration date */
    policy_flags_t     pflags;        /* account policy flags */
} policy_info_t;


typedef struct _login_info {
    sec_id_pac_t           net_creds; /* network credentials */
    unix_login_info_t      unix_info; /* unix credentials */
    policy_info_t          policy;
    login_info_state_t     state;
} login_info_t;


typedef struct _tgt_request_data_t {
    krb5_data      reply;       /* encrypted AS_REPLY, if present */
    krb5_flags     options;
    krb5_address   **addrs;
    krb5_enctype   enc_type;
    krb5_creds     creds;
    krb5_int32     nonce;
} tgt_request_data_t;

typedef struct _krb_info_t {
    krb5_ccache         cache;          /* kerberos V5 credential cache */
    krb5_pa_data        pepper;         /* "pepper" for key-crunching, if any */
    int                 num_caches;     /* count of previous caches */
    char                **prev_caches;  /* list of previous cache names */
    tgt_request_data_t  *tgt_data;
} krb_info_t;

typedef struct _file_modtimes 
{
    time_t		datafile;
    time_t		cachefile;
    time_t		namefile;
} sec_file_modtimes_t;

/*!!! ORDER OF ENUM MEMBERS IS CRITICAL - the code makes use
 * of ordinal values to speed up context validity checks
 */
typedef enum _context_state {
    none,
    allocated,  /* has cell and principal string names */
    semisetup,  /* has cell and principal names and unix info */
    setup,      /* has cell and principal string names, and an
                 *  encrypted TGT */
    valid,      /* TGT has been sucessfully decrypted */
    certified   /* TGT has been used successfully to authenticate
                 * to the host machine principal
                 */
} context_state_t;


/*
 * The internal structure of a login context.
 */
typedef struct sec_login_context {
    void                    *magic;
    sec_rgy_name_t          cell;
    sec_rgy_name_t          principal;
    login_info_t            *identity;
    krb_info_t              krb_info;
    context_state_t         state;
    sec_login_auth_src_t    auth_src;
    sec_login_flags_t       flags;
    boolean32               local;       /* true if net_creds are for a principal
                                            in the host cell */
    boolean32               refreshed;   /* true if context has been used in a
                                            successfull call to refresh_context.
                                            reset by validation routines */
    int                      refcount;
    struct sec_login_context *cache_link_next; /* next in cache */
    struct sec_login_context *cache_link_prev; /* prev in cache */
    char *context_name;     /* name of context (for import/export) */
    int context_name_len;   /* length of context name */
    int mark;               /* "mark bit" for cache sanity check */
    void *namecache;        /* name cache */
    char *namecache_name;   /* filename of name cache */
    sec_file_modtimes_t	    modtimes;

        /* persistent v1.1 info */
    sec_login__v1_1_info_t  v1_1_info; 

        /*  transient v1.1 info */
    boolean32                  cred_h_valid;
    rpc_authz_cred_handle_t    cred_h;
    sec_login_tkt_info_t       tkt_request;
    uuid_t                     generation_id;
    boolean32                  cache_purged;
    pthread_mutex_t	db_mutex;	/* Mutex for .data.db creds file */
    unsigned32		exptoken_mtime; /* mod time of .data.db file  */
    char 		*exptoken; /* export token string */
    int 		exptoken_len; /* and length ... */
    signed32		owner_uid; /* owner of the credential files */
} sec_login_context_t, *sec_login_context_p_t;

#define NET_CREDS(lcp) ((lcp)->identity->net_creds)
#define UNIX_CREDS(lcp) ((lcp)->identity->unix_info)
#define INFO_STATE(lcp) ((lcp)->identity->state)
#define POLICY_INFO(lcp) ((lcp)->identity->policy)
#define KRB_INFO(lcp) ((lcp)->krb_info)
#define KRB_REQUEST_INFO(lcp) ((lcp)->krb_info).tgt_data
/* 
 * The following refer to login context info added at v1.1
 */
#define EPAC_CHAIN(lcp) ((lcp)->v1_1_info.epac_chain)
#define DLG_REQ_INFO(lcp) ((lcp)->v1_1_info.dlg_req_info.info)
#define DLG_REQ_TYPE(lcp) ((lcp)->v1_1_info.dlg_req_type)
#define COMPAT_MODE(lcp) ((lcp)->v1_1_info.compat_mode)
#define GEN_V1_PAC(lcp) ((lcp)->v1_1_info.gen_v1_pac)

#define DEFAULT_HANDLE(lh) ((lh) == sec_login_default_handle)

/*
** Function Prototypes
*/

void sec_login_pvt_lrgy_store(
#ifdef __STDC__
    sec_login_context_t *lcp,   /* [in]  */
    error_status_t      *stp    /* [out] */
#endif
);

sec_login_context_p_t sec_login_pvt_check_handle (
#ifdef __STDC__
    sec_login_handle_t  login_context,   /* [in] */
    context_state_t     state,           /* [in] */
    error_status_t      *stp             /* [out] */
#endif
);

boolean32  sec_login_pvt_handle_equal (
#ifdef __STDC__
    sec_login_handle_t  handle1,   /* [in] */
    sec_login_handle_t  handle2    /* [in] */
#endif
);

void sec_login_pvt_set_context (
#ifdef __STDC__
    sec_login_handle_t  handle,        /* [in]  */
    boolean32           set_default,   /* [in]  */
    error_status_t      *stp           /* [out] */
#endif
);

void sec_login_pvt_set_default_context (
#ifdef __STDC__
    sec_login_handle_t  handle,    /* [in]  */
    error_status_t      *stp       /* [out] */
#endif
);

void sec_login_pvt_unset_default_context (
#ifdef __STDC__
    void
#endif
);
int resetpag (
#ifdef __STDC__
    void
#endif
);

sec_login_handle_t  sec_login_pvt_get_default_context (
#ifdef __STDC__
    error_status_t  *stp
#endif
);

boolean32 sec_login_pvt_purge_default_context (
#ifdef __STDC__
    sec_login_handle_t  handle    /* [in]  */
#endif
);

void  sec_login_pvt_allocate_login_context (
#ifdef __STDC__
    boolean32               override,       /* [in] */
    unsigned_char_p_t       user,           /* [in] */
    char                    *cache_name,    /* [in] */
    sec_login_context_t     **lcap,         /* [out] */
    error_status_t          *stp            /* [out] */
#endif
);

void  sec_login_pvt_allocate_first (
#ifdef __STDC__
    char                    *cache_name,    /* [in] */
    sec_login_context_t     **lcap,         /* [out] */
    error_status_t          *stp            /* [out] */
#endif
);

void  sec_login_pvt_purge_context (
#ifdef __STDC__
    sec_login_context_t     *lcp,           /* [in] */
    error_status_t          *stp            /* [out] */
#endif
);

void  sec_login_pvt_release_context (
#ifdef __STDC__
    sec_login_context_t     *lcp,               /* [in] */
    boolean32               force_cache_remove  /* [in] */
#endif
);


void sec_login_pvt_get_and_process_krb_tgt (
#ifdef __STDC__
    sec_login_context_t     *lcp,           /* [in] */
    sec_passwd_rec_t        *key,           /* [in] */
    boolean32               refresh,        /* [in] */
    error_status_t          *stp            /* [out] */
#endif
);


void  sec_login_pvt_get_login_info (
#ifdef __STDC__
    boolean32               override,       /* [in] */
    sec_login_context_t     *lcp,           /* [in, out] */
    sec_override_fields_t   *overridden,    /* [out] */
    error_status_t          *stp            /* [out] */
#endif
);


void  sec_login_pvt_get_lrgy_login_info (
#ifdef __STDC__
    boolean32               override,       /* [in] */
    sec_login_context_t     *lcp,           /* [in, out] */
    error_status_t          *stp            /* [out] */
#endif
);


void  sec_login_pvt_override_all (
#ifdef __STDC__
    sec_login_context_t     *lcp,           /* [in, out] */
    error_status_t          *stp            /* [out] */
#endif
);


void  sec_login_pvt_get_local_pwent (
#ifdef __STDC__
    sec_login_context_t     *lcp,
    error_status_t          *stp
#endif
);


int sec_login_pvt_new_pag (
#ifdef __STDC__
    sec_login_context_t     *lcp,
    unsigned32 *pag
#endif
);

void sec_login_pvt_link_ccache(
#ifdef __STDC__
    unsigned32      pag,
    boolean32       gen_pag,
    char            *pagenv,
    char            *prev_cfn,
    error_status_t  *stp
#endif
);

void  sec_login_pvt_check_policy (
#ifdef __STDC__
    sec_login_context_t     *lcp,           /* [in,out] */
    boolean32               *reset_passwd,  /* [out]    */
    error_status_t          *stp            /* [out]    */
#endif
);

void  sec_login_pvt_certify_context (
#ifdef __STDC__
    sec_login_context_t     *lcp,           /* [in,out] */
    error_status_t          *stp            /* [out]    */
#endif
);


sec_login_context_t * sec_login_pvt_setup_refresh_context (
#ifdef __STDC__
    sec_login_context_t     *lcp,           /* [in] */
    error_status_t          *stp
#endif
);

void   sec_login_pvt_synch_refresh_context (
#ifdef __STDC__
    sec_login_context_t     *lcp,           /* [in, out] */
    sec_login_context_t     *rcp,           /* [in] */
    error_status_t          *stp
#endif
);

void  sec_login_pvt_purge_refresh_context (
#ifdef __STDC__
    sec_login_context_t     *lcp,
    error_status_t          *stp
#endif
);

void  sec_login_pvt_inq_identity_expiration (
#ifdef __STDC__
    sec_login_context_t     *lcp,
    unsigned32              *exp_time,
    error_status_t          *stp
#endif
);

void  sec_login_pvt_inq_pac (
#ifdef __STDC__
    sec_login_context_t     *lcp,
    sec_id_pac_t            *pac,
    error_status_t          *stp
#endif
);

void  sec_login_pvt_convert_local (
#ifdef __STDC__
    sec_login_context_t     *lcp,           /* [in, out] */
    error_status_t          *stp
#endif
);

void  sec_login_pvt_create_restricted_context (
#ifdef __STDC__
    sec_login_context_t     *base_lc,
    unsigned32              num_new_groups,
    sec_id_t                *new_groups,
    sec_login_context_t     **restricted_lc,
    error_status_t          *stp
#endif
);

void  sec_login_pvt_synch_first (
#ifdef __STDC__
    sec_login_context_t     *lcp,           /* [in] */
    error_status_t          *stp
#endif
);

void sec_login_pvt_ccache_gennew (
#ifdef __STDC__
    krb5_ccache           *id,              /* [out] */
    error_status_t        *stp
#endif
);


void sec_login_cache_enter (
#ifdef __STDC__
    sec_login_context_t     *lcp
#endif
);

sec_login_context_t *sec_login_cache_lookup (
    int buf_len,           /* [in] buf_len */
    char *buf,             /* [in] buf */
    uuid_t *generation_id  /* [in] */
);

void sec_login_pvt_free_login_context (
#ifdef __STDC__
    sec_login_context_t *lcp    /* [in] lcp */
#endif
);

void sec_login_pvt_get_namecache (
#ifdef __STDC__
    sec_login_context_t *lcp,   /* [in] lcp */
    void **cache,       /* [out] cache */
    char **filename     /* [out] filename */
#endif
);

void sec_login_pvt_set_namecache (
#ifdef __STDC__
    sec_login_context_t *lcp,   /* [in] lcp */
    void *cache         /* [in] cache */
#endif
);

void sec_login_pvt_open_namecache(
    sec_login_context_t *lcp,		/* [in]  */
    char                *ncname,	/* [in]  */
    int                 oflag,		/* [in]  */
    int                 *cache_fd,	/* [out] */
    error_status_t      *stp		/* [out] */
);

/* 
 * free cache
 */

void sec__id_free_cache (
#ifdef __STDC__
    sec_login_handle_t *lcp    /* [in] lhp */
#endif
);

/* sec_login_pvt_create_raw_context
 *
 * Create a "raw" context using info supplied by the caller, without any verification.
 * It's up to the caller to ensure that the context will perform robustly in
 * all circumstances (i.e., the caller had better understand, in detail, the
 * implementation of login handles and their use in authenticated RPCs).
 *
 * This routine is provided to support context creation for the user-to-user
 * authentication in security server replication.  
 */
void  sec_login_pvt_create_raw_context (
#ifdef __STDC__
    char                  *cell,          /* [in] */
    char                  *principal,     /* [in] */
    sec_id_pac_t          *net_creds,     /* [in] */
    krb5_ccache           ccache,         /* [in] */
    sec_login_flags_t     flags,          /* [in] */
    sec_login_auth_src_t  auth_src,       /* [in] */
    context_state_t       state,          /* [in] */
    login_info_state_t    info_state,     /* [in] */
    boolean32             local,          /* [in] */
    policy_info_t         *policy_info,   /* [in] */
    uuid_t                *generation_id, /* [in] */
    sec_login_handle_t    *lcap,          /* [out] */
    error_status_t        *status         /* [out] */
#endif
);

/*
 * inform kernel that current tgt will expire at time `expiration'.
 */
void sec_login_pag_new_tgt (
#ifdef __STDC__
    unsigned32 pag,
    unsigned32 expiration
#endif
);

/*
 * retrieve pepper from login context or create default if NULL.
 */
char * sec_login_pvt_get_pepper (
    sec_login_context_t *lcp,
    error_status_t *stp
);

/*
 * This routine constructs the trusted portion of 
 * preauthentication data required for type 
 * KRB5_PADATA_ENC_THIRD_PARTY.
 */
void sec_login_pvt_get_trusted_preauth (
    krb5_principal       client,
    krb5_data            **machine_tgt,
    krb5_data            **enc_data,
    krb5_keyblock        *random_key,
    error_status_t       *stp
);

sec_login_handle_t sec_login_pvt_set_ext_attrs(
#ifdef __STDC__
    sec_login_context_t      *lcp,
    unsigned32               num_attributes,
    sec_attr_t               *attributes,
    error_status_t           *stp
#endif
);

sec_login_handle_t sec_login_pvt_become_initiator(
    sec_login_context_t         *lcp,
    sec_id_delegation_type_t    dlg_type,
    sec_id_restriction_set_t    *dlg_rstrs,
    sec_id_restriction_set_t    *tgt_rstrs,
    sec_id_opt_req_t            *opt_rstrs,
    sec_id_opt_req_t            *req_rstrs,
    sec_id_compatibility_mode_t compat_mode,
    error_status_t              *stp
);

sec_login_handle_t sec_login_pvt_do_delegation_request(
    rpc_authz_cred_handle_t     callers_identity,
    sec_login_context_t         *lcp,
    sec__login_dlg_req_type_t   dlg_op_requested,
    sec_id_delegation_type_t    dlg_type_permitted,
    sec_id_restriction_set_t    *dlg_rstrs,
    sec_id_restriction_set_t    *tgt_rstrs,
    sec_id_opt_req_t            *opt_rstrs,
    sec_id_opt_req_t            *req_rstrs,
    sec_id_compatibility_mode_t compat_mode,
    error_status_t              *stp
);

sec_login_handle_t sec_login_pvt_disable_dlg(
    sec_login_context_t         *lcp,
    error_status_t              *stp
);

void sec_login_pvt_gen_export_token (
    sec_login_context_t  *lc_p,          /* [in] */
    unsigned32           token_buf_size, /* [in] */
    ndr_byte             token_buf[],    /* [out] */
    unsigned32           *token_len_p,   /* [out] */
    error_status_t       *st_p           /* [out] */
);

void sec_login_pvt_parse_export_token(
    ndr_char        buf[],             /* [in] */
    unsigned32      buf_len,           /* [in] */
    char            **cname_p,         /* [out] */
    int             *cname_len_p,      /* [out] */
    uuid_t          *generation_id_p,  /* [out] */ 
    error_status_t  *st_p              /* [out] */
); 

void  sec_login_pvt_init_v1_1_info (
    sec_bytes_t     		*epac_chain,		/* [in]  */
    char	    		*prev_cfn,		/* [in]  */
    sec_login__v1_1_info_t  	*v1_1_info		/* [out] */
);

void sec_login_pvt_copy_v1_1_info(
    sec_login__v1_1_info_t      *to_v1_1_info,          /* [out] */
    sec_login__v1_1_info_t      *from_v1_1_info,        /* [in]  */
    error_status_t              *stp                    /* [out] */
);

void sec_login_pvt_get_dce_cf_data(
    char                *key,           /* [in]  */
    char                **data,         /* [out] */
    error_status_t      *stp            /* [out] */
);

void sec_login_pvt_get_krb5_ccache_vno(
    int                 *version        /* [out] */
);

void sec_login_pvt_get_gen_v1_pac_flag(
    boolean32           *gen_v1_pac     /* [out] */
);

void sec_login_pvt_get_krb5_kt_vno(
    krb5_int16		*version	/* [out] */
);

void pac_to_epac (
    sec_id_pac_t                *pac,
    sec_id_epac_t               *epac,
    unsigned32                  num_attr_keys,
    sec_attr_t                  *attr_keys,
    sec_id_delegation_type_t    dlg_type,
    sec_id_compatibility_mode_t compat_mode,
    error_status_t              *stp
);

void pac_to_epac_chain_bytes (
    sec_id_pac_t                *pac,
    sec_bytes_t                 *epac,
    unsigned32                  num_attr_keys,
    sec_attr_t                  *attr_keys,
    sec_id_delegation_type_t    dlg_type,
    sec_id_compatibility_mode_t compat_mode,
    error_status_t              *stp
);

#endif /* sec_login_pvt__included */
