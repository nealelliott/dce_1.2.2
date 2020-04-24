/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_login_util.h,v $
 * Revision 1.1.5.1  1996/05/17  15:14:15  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/05/16  21:53 UTC  psn
 * 	Merge fix for CHFts15637
 * 	[1995/08/22  21:07 UTC  mullan_s  /main/HPDCE02/4]
 *
 * 	Add more comments.
 * 	[1995/08/11  15:27 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 *
 * 	Add sec_login_util_fetch_epac_and_stktcreds.
 * 	[1995/07/26  18:04 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 *
 * 	Merge
 * 	[1995/05/26  19:37 UTC  mullan_s  /main/HPDCE02/3]
 *
 * 	Add stored_epac argument to sec_login_util_store_ptgtcreds_and_epac.
 * 	[1995/05/26  18:57 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat11/1]
 *
 * 	Merge
 * 	[1995/05/22  17:06 UTC  mullan_s  /main/HPDCE02/2]
 *
 * 	Add sec_login_util_fetch_db_entry, sec_login_util_fetch_db_entries,
 * 	sec_login_util_store_db_entries, sec_login_util_store_ptgtcreds_and_epac_chain.
 * 	[1995/05/22  16:44 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 *
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:02 UTC  mullan_s  /main/HPDCE02/1]
 *
 * 	Add sec_login_util_get_v1_pac_flag
 * 	[1995/03/20  14:49 UTC  mullan_s  /main/mullan_mothra_bin_compat2/2]
 *
 * 	Add privs & pac args to sec_login_util_get_krb_ptgt.
 * 	[1995/03/09  19:52 UTC  mullan_s  /main/mullan_mothra_bin_compat2/1]
 *
 * 	Add ptgt_uuid argument to sec_login_util_fetch_epac_and_stktcreds.
 * 	[1996/04/14  20:27 UTC  mullan_s  /main/HPDCE02/bartf_chfts16961/1]
 *
 * 	Merge fix for CHFts15637
 * 	[1995/08/22  21:07 UTC  mullan_s  /main/HPDCE02/4]
 *
 * 	Add more comments.
 * 	[1995/08/11  15:27 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 *
 * 	Add sec_login_util_fetch_epac_and_stktcreds.
 * 	[1995/07/26  18:04 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 *
 * 	Merge
 * 	[1995/05/26  19:37 UTC  mullan_s  /main/HPDCE02/3]
 *
 * 	Add stored_epac argument to sec_login_util_store_ptgtcreds_and_epac.
 * 	[1995/05/26  18:57 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat11/1]
 *
 * 	Merge
 * 	[1995/05/22  17:06 UTC  mullan_s  /main/HPDCE02/2]
 *
 * 	Add sec_login_util_fetch_db_entry, sec_login_util_fetch_db_entries,
 * 	sec_login_util_store_db_entries, sec_login_util_store_ptgtcreds_and_epac_chain.
 * 	[1995/05/22  16:44 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 *
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:02 UTC  mullan_s  /main/HPDCE02/1]
 *
 * 	Add sec_login_util_get_v1_pac_flag
 * 	[1995/03/20  14:49 UTC  mullan_s  /main/mullan_mothra_bin_compat2/2]
 *
 * 	Add privs & pac args to sec_login_util_get_krb_ptgt.
 * 	[1995/03/09  19:52 UTC  mullan_s  /main/mullan_mothra_bin_compat2/1]
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:07 UTC  psn
 * 	Merge fix for CHFts15637
 * 	[1995/08/22  21:07 UTC  mullan_s  /main/HPDCE02/4]
 *
 * 	Add more comments.
 * 	[1995/08/11  15:27 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 *
 * 	Add sec_login_util_fetch_epac_and_stktcreds.
 * 	[1995/07/26  18:04 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 *
 * 	Merge
 * 	[1995/05/26  19:37 UTC  mullan_s  /main/HPDCE02/3]
 *
 * 	Add stored_epac argument to sec_login_util_store_ptgtcreds_and_epac.
 * 	[1995/05/26  18:57 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat11/1]
 *
 * 	Merge
 * 	[1995/05/22  17:06 UTC  mullan_s  /main/HPDCE02/2]
 *
 * 	Add sec_login_util_fetch_db_entry, sec_login_util_fetch_db_entries,
 * 	sec_login_util_store_db_entries, sec_login_util_store_ptgtcreds_and_epac_chain.
 * 	[1995/05/22  16:44 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 *
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:02 UTC  mullan_s  /main/HPDCE02/1]
 *
 * 	Add sec_login_util_get_v1_pac_flag
 * 	[1995/03/20  14:49 UTC  mullan_s  /main/mullan_mothra_bin_compat2/2]
 *
 * 	Add privs & pac args to sec_login_util_get_krb_ptgt.
 * 	[1995/03/09  19:52 UTC  mullan_s  /main/mullan_mothra_bin_compat2/1]
 *
 * 	Make sec_login_util_ptgt_compat() take a target-cell
 * 	parameter, since the compatibility calculation varies
 * 	depending on whether the EPTGT in question is for a
 * 	local or foreign cell.
 * 	[1994/08/16  18:20:10  greg]
 *
 * 	intercell fixes for DCE1.1 beta
 * 	[1994/06/03  20:48:24  greg]
 *
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  20:46:20  ahop]
 *
 * 	hp_sec_to_osf_2 drop
 *
 * 	hp_sec_to_osf_2 drop
 * 	[1994/05/11  16:50:46  ahop]
 *
 * Revision 1.1.2.2  1992/12/29  13:02:19  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:25  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:46:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1994
** Unpublished work. All Rights Reserved.
**
*/
/* Security Login facility - authenticated RPC tie in
**
** This interface provides the hooks for the NCS 2.0 authenticated rpc runtime
** to get information for the sec_login facility.   These are implementation 
** interfaces - not architectural.
*/

#ifndef __sec_login_info_h__
#define __sec_login_info_h__

#ifdef __cplusplus
    extern "C" {
#endif

#include <dce/id_base.h>
#include <dce/sec_login.h>
#include <krb5/krb5.h>
#include <sec_login_db.h>


typedef void * handle_parm;
typedef void * krb_credentials;

/*
 * Various states the credential files can be in.
 * This is used by sec_authn.c, sec_krb_get_cred()
 * to determine what tickets are needed to authorize
 * the user.
 */
typedef unsigned32  sec_cred_state_t;
#define creds_found		0x1  /* Has both PTGT/EPAC and STKT */
#define epac_not_found		0x2  /* Needs PTGT/EPAC */
#define stkt_not_found		0x4  /* Needs STKT */

/*
 * Functions
 */

/* s e c _ l o g i n _ u t i l _ g e t _ k r b _ c a c h e
 *
 * Retrieve the Kerberos Ticket cache handle from the sec_login context
 */

handle_parm sec_login_util_get_krb_cache (
#ifdef __STDC__
    sec_login_handle_t  security_context  /* [in] */
#endif
);

/* s e c _ l o g i n _ u t i l _ g e t _  p a c
 *
 * Retrieve the pac from the sec_login context.
 */

sec_id_pac_t * sec_login_util_get_pac (
    sec_login_handle_t  security_context
);
    
void  sec_login_util_get_krb_ptgt (
    sec_login_handle_t  security_context,
    unsigned_char_p_t   target_cell,
    unsigned32          authn_svc,
    unsigned32          authz_svc,
    sec_bytes_t		*privs,
    error_status_t      *stp
);

/* if non-NULL, the return value should be freed by the caller */
char * sec_login_util_get_pepper (
    sec_login_handle_t  security_context,
    error_status_t      *stp
);

void sec_login_util_reference ( 
    sec_login_handle_t  in,
    sec_login_handle_t  *out,
    error_status_t      *stp
);   

void  sec_login_util_release (
    sec_login_handle_t  security_context
);

void sec_login_util_bind_to_sec_clientd (
    rpc_binding_handle_t *handle,    /* OUT */
    error_status_t       *stp        /* OUT */
);

void sec_login_util_encrypt (
    krb5_encrypt_block          *eblock,
    krb5_data                   *in,
    krb5_data                   *out,
    error_status_t              *stp
);

boolean32 sec_login_util_ptgt_compat(
    sec_login_handle_t lh,
    unsigned_char_p_t  target_cell,
    error_status_t      *stp
);

unsigned char * sec_login_util_get_cell (
    sec_login_handle_t  security_context
);

sec_bytes_t * sec_login_util_get_epac(
    sec_login_handle_t  security_context
);

boolean32 sec_login_util_get_v1_pac_flag(
    sec_login_handle_t  security_context
);

void sec_login_util_fetch_db_entry(
    sec_login_handle_t          security_context,       /* [in]  */
    char                        *datafile,              /* [in]  */
    sec_login_db_entry_t        *db_entry,              /* [out] */
    error_status_t              *stp                    /* [out] */
);

void sec_login_util_fetch_db_entries(
    sec_login_handle_t          security_context,       /* [in]  */
    char                        *datafile,              /* [in]  */
    unsigned32                  num_db_entries,         /* [in]  */
    sec_login_db_enum_t         db_type[],              /* [in]  */
    sec_login_db_entry_t        **db_entry,             /* [out] */
    unsigned32                  *num_fetched,           /* [out] */
    unsigned32                  *num_not_fetched,       /* [out] */
    sec_login_db_enum_t         **not_fetched,          /* [out] */
    error_status_t              *stp                    /* [out] */
);

void sec_login_util_store_db_entries(
    sec_login_handle_t          security_context,       /* [in]  */
    char                        *datafile,              /* [in]  */
    unsigned32                  num_db_entries,         /* [in]  */
    sec_login_db_entry_t        *db_entry,              /* [in]  */
    error_status_t              *stp                    /* [out] */
);

void sec_login_util_store_ptgtcreds_and_epac(
    sec_login_handle_t          security_context,       /* [in]  */
    char                        *datafile,              /* [in]  */
    sec_db_authdata_contents_t  v1_authdata,            /* [in]  */
    krb5_ccache                 ccache,                 /* [in]  */
    krb5_creds                  *creds,                 /* [in]  */
    sec_bytes_t			*stored_epac,		/* [out] */
    error_status_t              *stp                    /* [out] */
);

sec_cred_state_t sec_login_util_fetch_epac_and_stktcreds(
    sec_login_handle_t          security_context,       /* [in]  */
    char                        *datafile,              /* [in]  */
    unsigned_char_t             *target_cell,           /* [in]  */
    krb5_ccache                 krb_ccache,             /* [in]  */
    krb5_creds                  *scred,                 /* [in,out]  */
    sec_bytes_t                 *epac_chain,            /* [out] */
    uuid_t			*ptgt_uuid,		/* [out] */
    error_status_t              *stp                    /* [out] */
);

#ifdef __cplusplus
    }
#endif

#endif /* multiple inclusion guard */
