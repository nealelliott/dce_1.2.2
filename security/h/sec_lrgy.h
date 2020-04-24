/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_lrgy.h,v $
 * Revision 1.1.8.1  1996/11/13  18:01:11  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/10/28  15:10 UTC  mullan_s  /main/mullan_dce12_tp_preauth/3]
 *
 * 	Fix EXTERN/INITVAL duplicate macro errors.
 * 	[1996/10/24  17:01 UTC  mullan_s  /main/mullan_dce12_tp_preauth/2]
 *
 * 	Add sec_lrgy_encode_kdc_rep.
 * 	[1996/10/14  14:34 UTC  mullan_s  /main/mullan_dce12_tp_preauth/1]
 *
 * 	Add prototype for sec_lrgy_krbtgt_lookup.
 * 	[1996/02/18  23:01:13  marty  1.1.6.2]
 *
 * Revision 1.1.6.2  1996/02/18  23:01:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:35  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:29:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:22  root]
 * 
 * Revision 1.1.4.2  1992/12/29  13:27:45  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:47:13  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/31  22:30:38  burati
 * 	CR2419 Initial version of local registry header file
 * 	[1992/03/30  21:41:12  burati]
 * 
 * $EndLog$
 */

/*  sec_lrgy.h V=1 03/30/92 //littl/prgy/src2/security/h
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *  Local Registry - Base datatypes and API
 */

/* multiple inclusion guard */
#ifndef sec_lrgy__included
#define sec_lrgy__included

/*
 *     Local registry structures and import/export routines are similar to
 * login context structures/import/export routines, but must also save the
 * encrypted TGT, and are organized differently to allow simpler/faster
 * searches by global_principal_name.
 *
 * Local registry layout:
 *      - data file contains Unix & login context info
 *      - tgt file contains encrypted TGTs
 *
 *  <LRGY_DATA_FILE>
 *      <lrgy_header> [ <lrgy_entry> ... ]
 *  where
 *      <lrgy_header> :=> max_entries, num_entries, lifespan
 *      <lrgy_entry>  :=> <fixed_data> (contains lengths for cell,princ_name)
 *                        <cell_name><principal_name>
 *                        <varying_data>
 *
 *  <LRGY_TGT_FILE>
 *      <lrgy_tgt_header> [ <lrgy_tgt_entry> ... ]
 *  where
 *      <lrgy_tgt_header>   :=> <num_entries>
 *      <lrgy_tgt_entry>    :=> <cell_name_len>,
 *                              <principal_name_len>,
 *                              <options>,
 *                              <enc_type>,
 *                              <nonce>,
 *                              <reply_len>,
 *                              <varying_data> 
 *      <varying_data>      :=> <cell_name>,
 *                              <principal_name>,
 *                              <reply_data>
 */

#include <krb5/base-defs.h>
#include <krb5/krb5.h>
#include <dce/rgybase.h>
#include <dce/id_base.h>
#include <initstring.h>
#include <un_io.h>
#include <dce/lbase.h>
#include <sec_login_pvt.h>

#undef EXTERN
#undef INITVAL
#ifdef SEC_LRGY_MAIN
#   define EXTERN
#   define INITVAL(X) X
#else
#   define EXTERN extern
#   define INITVAL(X)
#endif


/*
 * Define constants for file locations.
 */
#if defined(__STDC__) && !defined(_K_R_PORT_CHECK_)
#   define LRGY_DIR         DCELOCAL_PATH "/var/security"
#   define LRGY_FILE        DCELOCAL_PATH "/var/security/lrgy_data"
#   define LRGY_FILE_BAK    DCELOCAL_PATH "/var/security/lrgy_data.bak"
#   define LRGY_NEW_FILE    DCELOCAL_PATH "/var/security/lrgy_data.new"
#   define LRGY_LOCK_FILE   DCELOCAL_PATH "/var/security/lrgy_lock"
#   define LRGY_TGT_FILE    DCELOCAL_PATH "/var/security/lrgy_tgts"
#   define LRGY_TGT_FILE_BAK    DCELOCAL_PATH "/var/security/lrgy_tgts.bak"
#   define LRGY_NEW_TGT_FILE    DCELOCAL_PATH "/var/security/lrgy_tgts.new"
#else

EXTERN char *lrgy_dir_path INITVAL (= NULL);
#   define LRGY_DIR \
    STRING_CONCAT(lrgy_dir_path, DCELOCAL_PATH, "/var/security", "")

EXTERN char *lrgy_data_path INITVAL (= NULL);
#   define LRGY_FILE \
    STRING_CONCAT(lrgy_data_path, DCELOCAL_PATH, "/var/security/lrgy_data", "")

EXTERN char *lrgy_data_bak_path INITVAL (= NULL);
#   define LRGY_FILE_BAK \
    STRING_CONCAT(lrgy_data_bak_path, DCELOCAL_PATH, "/var/security/lrgy_data.bak", "")

EXTERN char *lrgy_data_new_path INITVAL (= NULL);
#   define LRGY_NEW_FILE \
    STRING_CONCAT(lrgy_data_new_path, DCELOCAL_PATH, "/var/security/lrgy_data.new", "")

EXTERN char *lrgy_lock_path INITVAL (= NULL);
#   define LRGY_LOCK_FILE \
    STRING_CONCAT(lrgy_lock_path, DCELOCAL_PATH, "/var/security/lrgy_lock", "")

EXTERN char *lrgy_tgt_path INITVAL (= NULL);
#   define LRGY_TGT_FILE \
    STRING_CONCAT(lrgy_tgt_path, DCELOCAL_PATH, "/var/security/lrgy_tgts", "")

EXTERN char *lrgy_tgt_new_path INITVAL (= NULL);
#   define LRGY_NEW_TGT_FILE \
    STRING_CONCAT(lrgy_tgt_new_path, DCELOCAL_PATH, "/var/security/lrgy_tgts.new", "")

EXTERN char *lrgy_tgt_bak_path INITVAL (= NULL);
#   define LRGY_TGT_FILE_BAK \
    STRING_CONCAT(lrgy_tgt_bak_path, DCELOCAL_PATH, "/var/security/lrgy_tgts.bak", "")

#endif
#define LRGY_DIR_PROT   S_IRWXU
#define LRGY_FILE_PROT  S_IRUSR|S_IWUSR

/*
 * Local Registry - Header, property information
 */
typedef struct _sec_lrgy_hdr_t {
    unsigned32              max_entries;
    unsigned32              num_entries;
    sec_timeval_period_t    lifespan;
} sec_lrgy_hdr_t;

/* Default Local Registry: max_entries = 25, 0 entries, lifespan = 21 days */
static sec_lrgy_hdr_t sec_lrgy_default_hdr = { 25, 0, 1814400 };

/*
 * Local Registry - Fixed size entry data (and lengths for varying data)
 */
typedef struct _sec_lrgy_entry_t {
        /* unix identity */
    signed32                pw_uid;
    signed32                pw_gid;
    time_t                  pw_change;
    time_t                  pw_expire;
    unsigned32              num_unix_groups;

        /* admin info */
    sec_timeval_sec_t       acct_exp;
    sec_timeval_sec_t       passwd_exp;
    sec_timeval_sec_t       identity_exp;
    boolean32               passwd_valid;

       /* network identity */
    unsigned32              ppac_len;

        /* varying size data */
    char                    *cell;
    char                    *principal;
    char                    *pw_name;
    char                    *pw_passwd;
    char                    *pw_class;
    char                    *pw_gecos;
    char                    *pw_dir;
    char                    *pw_shell;
    char                    *groups;
    char                    *ppac;
} sec_lrgy_entry_t;


/*
 * Local Registry - Fixed data for TGT entry (and lengths for varying data)
 */
typedef struct _sec_lrgy_tgt_entry_t {
    char                    *cell;
    char                    *principal;
    krb5_flags              options;
    krb5_enctype            enc_type;
    krb5_int32              nonce;
    int                     reply_len;
    char                    *reply;
} sec_lrgy_tgt_entry_t;


/*
 * Local Registry API
 */


/*
 * sec_lrgy_properties_get_info - returns local registry properties.
 */
void sec_lrgy_properties_get_info(
#ifdef __STDC__
    sec_lrgy_hdr_t          *properties,    /* [out] */
    error_status_t          *status         /* [out] */
#endif
);


/*
 * sec_lrgy_properties_tgt_get_info - returns local rgy tgt properties.
 */
void sec_lrgy_properties_tgt_get_info(
#ifdef __STDC__
    unsigned32              *num_entries,   /* [out] */
    error_status_t          *status         /* [out] */
#endif
);


/*
 * sec_lrgy_properties_set_info - sets local registry properties.
 */
void sec_lrgy_properties_set_info(
#ifdef __STDC__
    sec_lrgy_hdr_t          *properties,    /* [in]  */
    error_status_t          *status         /* [out] */
#endif
);


/*
 * sec_lrgy_entry_store - Add/Replace Unix and login context data.
 */
void sec_lrgy_entry_store(
#ifdef __STDC__
    sec_lrgy_entry_t        *entry,         /* [in]  */
    error_status_t          *status         /* [out] */
#endif
);


/*
 * sec_lrgy_entry_lookup - Attempt to lookup an entry by globalname.
 *
 *  Caller must free varying entries after processing.
 */
void sec_lrgy_entry_lookup(
#ifdef __STDC__
    unsigned_char_p_t       global_name,    /* [in]  */
    sec_lrgy_entry_t        *entry,         /* [out] */
    error_status_t          *status         /* [out] */
#endif
);


/*
 * sec_lrgy_entry_cursor_init
 */
void sec_lrgy_entry_cursor_init(
#ifdef __STDC__
    void                    **cursor,       /* [out] */
    error_status_t          *status         /* [out] */
#endif
);


/*
 * sec_lrgy_entry_get_next
 *
 *  Caller must free varying entries after processing.
 */
void sec_lrgy_entry_get_next(
#ifdef __STDC__
    void                    *cursor,        /* [in]  */
    sec_lrgy_entry_t        *entry,         /* [out] */
    sec_timeval_sec_t       *timestamp,     /* [out] */
    error_status_t          *status         /* [out] */
#endif
);


/*
 * sec_lrgy_entry_cursor_release
 */
void sec_lrgy_entry_cursor_release(
#ifdef __STDC__
    void                    **cursor,       /* [in]  */
    boolean32               *modified,      /* [out] */
    error_status_t          *status         /* [out] */
#endif
);


/*
 * sec_lrgy_entry_delete - Delete a specific lrgy account entry.
 */
void sec_lrgy_entry_delete(
#ifdef __STDC__
    unsigned_char_p_t       global_name,        /* [in]  */
    error_status_t          *status             /* [out] */
#endif
);


/*
 * sec_lrgy_tgt_store - Add/Replace tgt data.
 */
void sec_lrgy_tgt_store(
#ifdef __STDC__
    sec_lrgy_tgt_entry_t    *entry,         /* [in]  */
    error_status_t          *status         /* [out] */
#endif
);


/*
 * sec_lrgy_tgt_lookup - Attempt to lookup a tgt by globalname.
 *
 *  Caller must free varying entries after processing.
 */
void sec_lrgy_tgt_lookup(
#ifdef __STDC__
    unsigned_char_p_t       global_name,    /* [in]  */
    sec_lrgy_tgt_entry_t    *entry,         /* [out] */
    error_status_t          *status         /* [out] */
#endif
);


/*
 * sec_lrgy_tgt_delete - Attempt to delete a tgt by globalname.
 */
void sec_lrgy_tgt_delete(
#ifdef __STDC__
    unsigned_char_p_t       global_name,    /* [in]  */
    error_status_t          *status         /* [out] */
#endif
);


/*
 * sec_lrgy_purge_expired
 *
 *  Purge entries that have expired, according to the lifespan property.
 */
void sec_lrgy_purge_expired(
#ifdef __STDC__
    unsigned32              *num_purged,        /* [out] */
    unsigned32              *num_tgts_purged,   /* [out] */
    error_status_t          *status             /* [out] */
#endif
);

void sec_lrgy_krbtgt_lookup (
    krb5_principal          client,         /* [in]  */
    krb5_data               *reply,         /* [out] */
    error_status_t          *status         /* [out] */
);

void sec_lrgy_encode_kdc_rep (
    sec_login_context_t     *lcp,           /* [in,out]  */
    krb5_kdc_rep            *as_reply,      /* [in]  */
    sec_passwd_type_t	    keytype,	    /* [in]  */
    void const * volatile   keyseed,        /* [in]  */
    error_status_t	    *status	    /* [out] */
);

#endif /* sec_lrgy__included */
