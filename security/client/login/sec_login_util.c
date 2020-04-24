/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_login_util.c,v $
 * Revision 1.1.16.2  1996/05/17  15:13:56  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/3  1996/05/16  21:37 UTC  psn
 * 	Merge Sean's fix for sec_secrefresh.
 *
 * 	HP revision /main/DCE_1.2/arvind_checkout/1  1996/05/16  21:35 UTC  psn
 * 	Fix memory corruption via incorrectly placed call to
 * 	sec_login_db_free_entry_contents() in
 * 	sec_login_util_fetch_epac_and_stktcreds().
 * 	[1996/02/09  16:56 UTC  jrr  /main/HPDCE02/jrr_mothra_1/1]
 *
 * 	HP revision /main/DCE_1.2/mullan_dce121_secrefresh/1  1996/04/16  17:13 UTC  mullan_s
 * 	Merge fix for CHFts15637
 * 	[1995/08/22  21:07 UTC  mullan_s  /main/HPDCE02/4]
 *
 * 	Add svc handle to dce_assert call in fetch_epac_and_stkt_creds.
 * 	[1995/08/21  21:01 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/7]
 *
 * 	Update calls to priv_krb_util_{parse|construct}_v1_{stkt|ptgt}_authdata.
 * 	[1995/08/15  17:09 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/6]
 *
 * 	Add more comments to fetch_epac_and_stktcreds
 * 	[1995/08/11  15:27 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/5]
 *
 * 	fetch_epac_and_stktcreds: ensure stkt/epac uuids match
 * 	[1995/08/10  18:18 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/4]
 *
 * 	Comapare uuids.
 * 	[1995/08/04  13:38 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/3]
 *
 * 	fetch_epac_and_stktcreds : NULL out epac_chain->bytes after freeing
 * 	[1995/07/31  16:45 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 *
 * 	CHFts15637: Fix "mismatched seal" race conditions by adding routine
 * 	to retrieve EPAC/STKT pair within lock (sec_login_util_fetch_epac_and_stktcreds).
 * 	[1995/07/26  19:34 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 *
 * 	Merge
 * 	[1995/05/26  19:38 UTC  mullan_s  /main/HPDCE02/3]
 *
 * 	In sec_login_util_store_ptgtcreds_and_epac, return stored_epac if found.
 * 	[1995/05/26  18:56 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat11/1]
 *
 * 	Merge
 * 	[1995/05/22  17:06 UTC  mullan_s  /main/HPDCE02/2]
 *
 * 	Provide thread-safe routines for storing and retrieving data
 * 	from the .data.db creds file.
 * 	[1995/05/22  16:42 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 *
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:01 UTC  mullan_s  /main/HPDCE02/1]
 *
 * 	Add sec_login_util_get_v1_pac_flag.
 * 	[1995/03/20  14:49 UTC  mullan_s  /main/mullan_mothra_bin_compat2/2]
 *
 * 	Add privs & pac args to sec_login_util_get_krb_ptgt.
 * 	[1995/03/09  19:51 UTC  mullan_s  /main/mullan_mothra_bin_compat2/1]
 *
 * 	HP revision /main/HPDCE02/bartf_chfts16961/1  1996/04/14  20:32 UTC  mullan_s
 * 	sec_login_util_util_fetch_epac_and_stktcreds: add ptgt_uuid argument,
 * 	and return to caller to be used in subsequent authorization data
 * 	matching.
 *
 * 	HP revision /main/HPDCE02/5  1996/02/14  21:01 UTC  jrr
 * 	Fix memory corruption via incorrectly placed call to
 * 	sec_login_db_free_entry_contents() in
 * 	sec_login_util_fetch_epac_and_stktcreds().
 * 	[1996/02/09  16:56 UTC  jrr  /main/HPDCE02/jrr_mothra_1/1]
 *
 * 	HP revision /main/HPDCE02/4  1995/08/22  21:07 UTC  mullan_s
 * 	Merge fix for CHFts15637
 *
 * 	HP revision /main/HPDCE02/mullan_chfts15637/7  1995/08/21  21:01 UTC  mullan_s
 * 	Add svc handle to dce_assert call in fetch_epac_and_stkt_creds.
 *
 * 	HP revision /main/HPDCE02/mullan_chfts15637/6  1995/08/15  17:09 UTC  mullan_s
 * 	Update calls to priv_krb_util_{parse|construct}_v1_{stkt|ptgt}_authdata.
 *
 * 	HP revision /main/HPDCE02/mullan_chfts15637/5  1995/08/11  15:27 UTC  mullan_s
 * 	Add more comments to fetch_epac_and_stktcreds
 *
 * 	HP revision /main/HPDCE02/mullan_chfts15637/4  1995/08/10  18:18 UTC  mullan_s
 * 	fetch_epac_and_stktcreds: ensure stkt/epac uuids match
 *
 * 	HP revision /main/HPDCE02/mullan_chfts15637/3  1995/08/04  13:38 UTC  mullan_s
 * 	Comapare uuids.
 *
 * 	HP revision /main/HPDCE02/mullan_chfts15637/2  1995/07/31  16:45 UTC  mullan_s
 * 	fetch_epac_and_stktcreds : NULL out epac_chain->bytes after freeing
 *
 * 	HP revision /main/HPDCE02/mullan_chfts15637/1  1995/07/26  19:34 UTC  mullan_s
 * 	CHFts15637: Fix "mismatched seal" race conditions by adding routine
 * 	to retrieve EPAC/STKT pair within lock (sec_login_util_fetch_epac_and_stktcreds).
 *
 * 	HP revision /main/HPDCE02/3  1995/05/26  19:38 UTC  mullan_s
 * 	Merge
 *
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat11/1  1995/05/26  18:56 UTC  mullan_s
 * 	In sec_login_util
 *
 * Revision 1.1.7.7  1994/08/18  20:25:18  greg
 * 	sec_login_util_ptgt_compat() now returns "true" when
 * 	the login context has extended attributes associated
 * 	with it.  See comments in sec_login_util_ptgt_compat()
 * 	for details.
 * 	[1994/08/17  22:42:36  greg]
 * 
 * 	Modularize login handle validation in a single routine.
 * 	Remove sec_login_util_get_delegation_chain().  Make
 * 	sec_login_util_ptgt_compat() calculate compatibility
 * 	for real.
 * 	[1994/08/16  18:15:16  greg]
 * 
 * Revision 1.1.7.6  1994/07/21  20:26:32  greg
 * 	include sec_krb_util.h so macros get expanded into
 * 	real function calls.
 * 	[1994/07/21  14:11:45  greg]
 * 
 * 	sec_login_util_get_delegation_chain() should NOT go remote to
 * 	get a PTGT.  It should stop at the credential cache.  Otherwise,
 * 	it may cause a remote operation when called from down in the guts
 * 	of the RPC code.
 * 	[1994/07/20  18:13:38  greg]
 * 
 * Revision 1.1.7.5  1994/06/10  15:07:09  greg
 * 	intercell fixes for DCE1.1 beta
 * 	[1994/06/03  20:48:01  greg]
 * 
 * Revision 1.1.7.4  1994/06/02  21:57:33  mdf
 * 	Merged with changes from 1.1.7.3
 * 	[1994/06/02  21:56:08  mdf]
 * 
 * 	add impersonation support (greg)
 * 	Real delegation support in RPC/sec_authn. (sommerfeld)
 * 	make PTGT refreshes recognize delegation. (greg)
 * 	[1994/05/24  15:14:02  mdf]
 * 
 * 	add impersonation support (greg)
 * 	Real delegation support in RPC/sec_authn. (sommerfeld)
 * 	make PTGT refreshes recognize delegation. (greg)
 * 
 * Revision 1.1.7.3  1994/06/02  20:21:10  mdf
 * 	add impersonation support (greg)
 * 	Real delegation support in RPC/sec_authn. (sommerfeld)
 * 	make PTGT refreshes recognize delegation. (greg)
 * 	[1994/05/24  15:14:02  mdf]
 * 
 * 	add impersonation support (greg)
 * 	Real delegation support in RPC/sec_authn. (sommerfeld)
 * 	make PTGT refreshes recognize delegation. (greg)
 * 
 * Revision 1.1.7.1  1994/05/11  16:50:23  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Add sec_login_util_ptgt_compat
 * 	Add sec_login_util_encrypt.
 * 	[1994/04/29  20:46:00  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 	Add sec_login_util_ptgt_compat
 * 	Add sec_login_util_encrypt.
 * 
 * 	hp_sec_to_osf_2 drop
 * 	Add sec_login_util_ptgt_compat
 * 	Add sec_login_util_encrypt.
 * 
 * Revision 1.1.5.2  1992/12/29  13:02:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:40:10  zeliff]
 * 
 * Revision 1.1.2.4  1992/07/13  20:00:21  ahop
 * 	 3903: args to memcpy of pepper were interchanged in sec_login_util_get_pepper
 * 	[1992/06/30  22:27:44  ahop]
 * 
 * Revision 1.1.2.3  1992/05/08  20:53:44  sommerfeld
 * 	Remove sec_login_util_reference, sec_login_util_release
 * 	(now in sec_login_cache.c).
 * 	[1992/04/29  03:15:29  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/03/31  21:54:55  burati
 * 	CR2419 Merged in local registry code
 * 	[1992/03/31  01:30:19  burati]
 * 
 * Revision 1.1  1992/01/19  14:47:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 * History:
 *  01/30/92    burati  Use 1.0.1 local registry scheme
 *  11/23/90    pato    creation
 *
 * Security Login facility - authenticated RPC tie in
 *
 * This interface provides the hooks for the NCS 2.0 authenticated rpc runtime
 * to get information for the sec_login facility.   These are implementation
 * interfaces - not architectural.
 */

#include <dce/assert.h>
#include <krb5/krb5.h>
#include <priv.h>
#include <pthread.h>
#include <bstring.h>
#include <rgymacro.h>
#include <un_maloc.h>
#include <sec_login_util.h>
#include <sec_login_pvt.h>
#include <priv_krb_util.h>
#include <sec_krb_util.h>
#include <sec_encode.h>
#include <sec_login_db.h>
#include <sec_svc.h>

#include <scd_loc.h>

/* Internal state */
static boolean32             sec_clientd_bound = false,
                             sec_clientd_mutex_inited = false;
static rpc_binding_handle_t  sec_clientd_handle;
static pthread_mutex_t       sec_clientd_bind_lock;
static pthread_once_t        sec_clientd_mutex_once = pthread_once_init;

/*
 * Private functions
 */

static sec_login_context_t *
sec___login_util_check_context (
    sec_login_handle_t  security_context,
    error_status_t      *stp
)
{
    sec_login_context_t *lcp = NULL;

    CLEAR_STATUS(stp);

    if (DEFAULT_HANDLE(security_context)) {

        /*
         * sec_login_pvt_get_default will construct the default login
         * context from the process environment, if no context has been
         * set explicitly
         */
        security_context = sec_login_pvt_get_default_context(stp);

        if (GOOD_STATUS(stp)) {
            lcp = sec_login_pvt_check_handle(security_context, none, stp);
        }

    } else {
       lcp = sec_login_pvt_check_handle(security_context, valid, stp);
    }

    return lcp;
}
    
    
/*
 * Functions
 */

/* s e c _ l o g i n _ u t i l _ g e t _ k r b _ c a c h e
 *
 * Retrieve the Kerberos Ticket cache handle from the sec_login context
 *
 * If the security context is the default context, construct a new
 * cache handle from the cache name stored in the process environment.
 * If there is no such environment, return NULL.
 *
 * If the security context is not the default, and is a valid context, then
 * construct a duplicate handle from the cache in the context.
 *
 */

handle_parm sec_login_util_get_krb_cache (
    sec_login_handle_t  security_context
)
{
    error_status_t      st;
    sec_login_context_t *lcp;

    CLEAR_STATUS(&st);

    lcp = sec___login_util_check_context(security_context, &st);
    if (BAD_STATUS(&st)) {
	return NULL;
    }

    return lcp->krb_info.cache;
}


/* s e c _ l o g i n _ u t i l _ g e t _  p a c
 *
 * Retrieve the pac from the sec_login context.
 */
sec_id_pac_t * sec_login_util_get_pac(
    sec_login_handle_t  security_context
)
{
    sec_login_context_t  *lcp;
    sec_id_pac_t         *pac = NULL;
    error_status_t       st;

    CLEAR_STATUS(&st);
    
    lcp = sec___login_util_check_context(security_context, &st);
    
    
    if (GOOD_STATUS(&st)) {
        pac = &NET_CREDS(lcp);
    }
    
    return pac;
}

/* s e c _ l o g i n _ u t i l _ g e t _  e p a c
 *
 * Retrieve the encoded epac from the sec_login context.
 */
sec_bytes_t * sec_login_util_get_epac(
    sec_login_handle_t  security_context
)
{
    sec_login_context_t  *lcp;
    sec_bytes_t          *epac = NULL;
    error_status_t       st;

    CLEAR_STATUS(&st);

    lcp = sec___login_util_check_context(security_context, &st);
    if (GOOD_STATUS(&st)) {
        epac = &EPAC_CHAIN(lcp);
    }
    
    return epac;
}

/* s e c _ l o g i n _ u t i l _ g e t _ c e l l
 *
 * Retrieve the encoded epac from the sec_login context.
 */
unsigned char * sec_login_util_get_cell(
    sec_login_handle_t  security_context
)
{
    sec_login_context_t  *lcp;
    error_status_t       st;
    unsigned char *      cp = NULL;

    CLEAR_STATUS(&st);

    lcp = sec___login_util_check_context(security_context, &st);

    if (GOOD_STATUS(&st)) {
        cp = &(lcp->cell[0]);
    }

    return cp;
}


void  sec_login_util_get_krb_ptgt(
    sec_login_handle_t  security_context,
    unsigned_char_p_t   target_cell,
    unsigned32          authn_svc,
    unsigned32          authz_svc,
    sec_bytes_t		*privs,
    error_status_t      *stp
)
{
    sec_login_context_t *lcp;
    krb5_timestamp        now;
    boolean32             expired = true;
    unsigned32            i;
    CLEAR_STATUS(stp);

    lcp = sec___login_util_check_context(security_context, stp);

    if (BAD_STATUS(stp)) {
	return;
    }

    /* 
     * 
     */
    switch (DLG_REQ_TYPE(lcp)) {
    case sec__login_c_dlg_req_none:
    case sec__login_c_dlg_req_init:
	sec_priv_get_eptgt(security_context, target_cell, authn_svc, authz_svc,
			   privs, stp);
	break;
    case sec__login_c_dlg_req_traced:
	sec_priv_become_delegate(lcp, target_cell,
				 rpc_c_authn_dce_secret,
				 rpc_c_authz_dce, 
				 DLG_REQ_INFO(lcp).dlg_type, 
				 &DLG_REQ_INFO(lcp).dlg_rstrs,
				 &DLG_REQ_INFO(lcp).tgt_rstrs,
				 &DLG_REQ_INFO(lcp).opt_rstrs,
				 &DLG_REQ_INFO(lcp).req_rstrs,
				 COMPAT_MODE(lcp),
				 &DLG_REQ_INFO(lcp).dlg_chain, 
				 &EPAC_CHAIN(lcp),
				 &DLG_REQ_INFO(lcp).dlg_token_set,
                                 privs,
				 stp);
	break;
    case sec__login_c_dlg_req_imp:
	sec_priv_become_impersonator(lcp, target_cell,
				     rpc_c_authn_dce_secret,
				     rpc_c_authz_dce, 
				     DLG_REQ_INFO(lcp).dlg_type, 
				     &DLG_REQ_INFO(lcp).dlg_rstrs,
				     &DLG_REQ_INFO(lcp).tgt_rstrs,
				     &DLG_REQ_INFO(lcp).opt_rstrs,
				     &DLG_REQ_INFO(lcp).req_rstrs,
				     &DLG_REQ_INFO(lcp).dlg_chain, 
				     &EPAC_CHAIN(lcp),
				     &DLG_REQ_INFO(lcp).dlg_token_set,
                                     privs,
				     stp);
	break;
    default:
	/* 
	 * if we don't understand the delegation type return an
	 * invalid delegation type error.  Maybe we've latched on
	 * to a login context that  was created by a newer rev of
	 * the software that has added new delegation types.
	 */
	SET_STATUS(stp, sec_login_s_invalid_deleg_type);
	break;
    }
}

char * sec_login_util_get_pepper (
    sec_login_handle_t  security_context,
    error_status_t      *stp
)
{
    char                *pepper = NULL;
    sec_login_context_t *lcp;

    CLEAR_STATUS(stp);

    lcp = sec___login_util_check_context(security_context, stp);

    if (BAD_STATUS(stp)) {
	return NULL;
    }

    pepper = sec_login_pvt_get_pepper(lcp, stp);
    if (BAD_STATUS(stp)) {
        return NULL;
    }

    return pepper;
} 


PRIVATE void sec_clientd_mutex_init(
    void
)
{
    pthread_mutex_init(&sec_clientd_bind_lock, pthread_mutexattr_default);
    sec_clientd_mutex_inited = true;
}


/* 
 * This routine attempts to bind to sec_clientd and if successful,
 * returns a binding handle.
 */
void sec_login_util_bind_to_sec_clientd (
    rpc_binding_handle_t  *handle,    /* OUT */
    error_status_t        *stp        /* OUT */
)
{
    FILE                  *fptr;
    boolean32             got_binding = false;
    char                  buff[1024];
    unsigned_char_p_t     binding_real, protseq_real, endpoint_real;
    unsigned_char_p_t     obj         = NULL;
    unsigned_char_p_t     protseq     = NULL;
    unsigned_char_p_t     netaddr     = NULL;
    unsigned_char_p_t     endpoint    = NULL;
    unsigned_char_p_t     options     = NULL;
    unsigned_char_p_t     binding     = NULL;
    error_status_t        lst;
    char                  *cp;

    if (!sec_clientd_mutex_inited)
        pthread_once(&sec_clientd_mutex_once, sec_clientd_mutex_init);

    pthread_mutex_lock(&sec_clientd_bind_lock);

    CLEAR_STATUS(stp);

    /* check if there is a binding handle already available */
    if (sec_clientd_bound) {
        rpc_binding_copy(sec_clientd_handle, handle, stp);
        pthread_mutex_unlock(&sec_clientd_bind_lock);
        return;
    }

    /* just in case */
    if (sec_clientd_handle != NULL)
        rpc_binding_free(&sec_clientd_handle, &lst);

    if ((fptr = fopen(SEC_CLIENTD_BINDING, "r")) != NULL) {
        while ((!got_binding) && (fgets(buff, 1024, fptr) != NULL)) {
            /* fgets keeps newlines, so get rid of manually */
            cp = strchr(buff, '\n');
            if (cp != NULL)
                *cp = '\0';
            binding_real = (unsigned_char_p_t) buff;
            rpc_string_binding_parse((unsigned_char_p_t) buff, &obj,
                        &protseq, &netaddr, &endpoint, &options, stp);
            if (GOOD_STATUS(stp)) {
                /*
                 * If a simple name was used (unadorned with the proper
                 * protocol sequence), we try a default sequence.
                 */
                if (NULL_STRING(protseq)) {
                    protseq_real = (unsigned_char_p_t) "ip";
                } else
                    protseq_real = protseq;

                if (NULL_STRING(endpoint)) {
                    endpoint_real = (unsigned_char_p_t) "";
                } else
                    endpoint_real = endpoint;

                rpc_string_binding_compose(obj, protseq_real, netaddr,
                                            endpoint_real, options,
                                            &binding, stp);
                if (GOOD_STATUS(stp)) {
                    binding_real = binding;
                    rpc_binding_from_string_binding(binding_real,
                            &sec_clientd_handle, stp);
                }
            }   /* end of if rpc_string_binding_parse */
            if (GOOD_STATUS(stp))
                got_binding = true;
        }       /* end of if fgets */
        fclose(fptr);
    } else {       /* end of if fopen */
        SET_STATUS(stp, sec_login_s_config);
        pthread_mutex_unlock(&sec_clientd_bind_lock);
        return;
    }

    /*
     * Free the strings that may have been allocated by the RPC runtime
     */
    if (obj != NULL) {
        rpc_string_free(&obj, &lst);
    }
    if (protseq != NULL) {
        rpc_string_free(&protseq, &lst);
    }
    if (netaddr != NULL) {
        rpc_string_free(&netaddr, &lst);
    }
    if (endpoint != NULL) {
        rpc_string_free(&endpoint, &lst);
    }
    if (options != NULL) {
        rpc_string_free(&options, &lst);
    }
    if (binding != NULL) {
        rpc_string_free(&binding, &lst);
    }

    if (sec_clientd_handle != NULL) 
        rpc_binding_copy(sec_clientd_handle, handle, stp);

    /*
     * Set internal flag so binding handle can be reused.
     */
    if (GOOD_STATUS(stp))
        sec_clientd_bound = true;

    pthread_mutex_unlock(&sec_clientd_bind_lock);
}


/*
 *  s e c _ l o g i n _ u t i l _ e n c r y p t
 *
 *  This code copied from kdb/encrypt_key.c and rs/rs_mkey.c.
 *
 *  Encrypt data. "eblock" is used to encrypt the data in "in"
 *  into "out"; the storage pointed to by "out" is allocated
 *  before use.
 */
void sec_login_util_encrypt (
    krb5_encrypt_block          *eblock,
    krb5_data                   *in,
    krb5_data                   *out,
    error_status_t              *st_p
)
{
    CLEAR_STATUS(st_p);

    out->length = krb5_encrypt_size(in->length, eblock->crypto_entry);

    /* add padding area, and zero it */
    in->data = realloc(in->data, out->length);
    if (in->data == NULL) {
        SET_STATUS(st_p, sec_login_s_no_memory);
        return;
    }

    memset(in->data + in->length, 0, out->length - in->length);
    out->data = malloc(out->length);
    if (out->data == NULL) {
        SET_STATUS(st_p, sec_login_s_no_memory);
        return;
    }

    /* call the encryption routine */
    *st_p = krb5_encrypt((krb5_pointer) in->data,
                          (krb5_pointer) out->data,
                          in->length, eblock, 0);
    if (BAD_STATUS(st_p)) {
        free((char *)out->data);
    }
    return;
}


/*
 * Does the login context allow support for backward compatibility
 */
boolean32 sec_login_util_ptgt_compat(
    sec_login_handle_t lh,
    unsigned_char_p_t  target_cell,
    error_status_t      *stp
)
{
    sec_login_context_t  *lcp;
    boolean32            local, compat;
    sec_id_epac_set_t    epac_set;
    sec_id_epac_data_t   epac_data;
    CLEAR_STATUS(stp);

    lcp = sec___login_util_check_context(lh, stp);
    if (BAD_STATUS(stp)) {
	return false;
    }

    /* 
     * In a local request, no delegation operations are
     * compatible.
     *
     * In an intercell request, compatibility is based on the
     * compat mode in effect for the login context.
     */
    if (strcmp((char *)target_cell, (char *)lcp->cell) == 0) {  /* local case */
	switch(DLG_REQ_TYPE(lcp)) {
	default:
	    /*
	     * Shouldn't ever happen
	     */
	    compat = false;
	    break;
	case sec__login_c_dlg_req_none:
	    /* 
	     * If delegation has not been enabled, the only
	     * 1.1 feature left that might be in effect
	     * is extended attributes.  And since there is
	     * no guarantee that the requested attributes
	     * will actually be in the EPAC even when the 
	     * security servers are at 1.1, we consider this
	     * compatible.
	     */
	    compat = true;
	    break;
	case sec__login_c_dlg_req_init:
	    /*FALLTHROUGH*/
	case sec__login_c_dlg_req_traced:
	    /*FALLTHROUGH*/
	case sec__login_c_dlg_req_imp:
	    compat = false;
	    break;
	}
    } else { /* intercell case */
	if (COMPAT_MODE(lcp) == sec_id_compat_mode_none){
	    compat = false;
	} else {
	    compat = true;
	}
    }

    if (STATUS_OK(stp) && !compat) {
	SET_STATUS(stp, sec_s_v1_1_no_support);
    }

    return compat;
}    

/*
 * s e c _ l o g i n _ u t i l _ g e t _ v 1 _ p a c _ f l a g
 *
 * This routine retrieves the value of the gen_v1_pac flag from
 * the login context.
 */
boolean32 sec_login_util_get_v1_pac_flag(
    sec_login_handle_t  security_context
)
{
    sec_login_context_t  *lcp;
    boolean32		 gen_v1_pac = true;
    error_status_t       st;

    CLEAR_STATUS(&st);

    lcp = sec___login_util_check_context(security_context, &st);
    if (GOOD_STATUS(&st)) {
        gen_v1_pac = GEN_V1_PAC(lcp);
    }

    return gen_v1_pac;
}

void sec_login_util_fetch_db_entry(
    sec_login_handle_t  	security_context, 	/* [in]  */
    char                        *datafile,        	/* [in]  */
    sec_login_db_entry_t        *db_entry,        	/* [out] */
    error_status_t              *stp              	/* [out] */
)
{
    sec_login_context_t  *lcp = (sec_login_context_t *) security_context;

    CLEAR_STATUS(stp);

    pthread_mutex_lock(&lcp->db_mutex);

    sec_login_db_fetch_entry(datafile, db_entry, stp);

    pthread_mutex_unlock(&lcp->db_mutex);
}

void sec_login_util_fetch_db_entries(
    sec_login_handle_t  	security_context, 	/* [in]  */
    char                        *datafile,              /* [in]  */
    unsigned32                  num_db_entries,         /* [in]  */
    sec_login_db_enum_t         db_type[],              /* [in]  */
    sec_login_db_entry_t        **db_entry,             /* [out] */
    unsigned32                  *num_fetched,           /* [out] */
    unsigned32                  *num_not_fetched,       /* [out] */
    sec_login_db_enum_t         **not_fetched,          /* [out] */
    error_status_t              *stp                    /* [out] */
)
{
    sec_login_context_t  *lcp = (sec_login_context_t *) security_context;

    CLEAR_STATUS(stp);

    pthread_mutex_lock(&lcp->db_mutex);

    sec_login_db_fetch_entries(datafile, num_db_entries, db_type,
	db_entry, num_fetched, num_not_fetched, not_fetched, stp);

    pthread_mutex_unlock(&lcp->db_mutex);
}

void sec_login_util_store_db_entries(
    sec_login_handle_t  	security_context, 	/* [in]  */
    char                        *datafile,      	/* [in]  */
    unsigned32                  num_db_entries, 	/* [in]  */
    sec_login_db_entry_t        *db_entry,      	/* [in]  */
    error_status_t              *stp            	/* [out] */
)
{
    sec_login_context_t  *lcp = (sec_login_context_t *) security_context;

    CLEAR_STATUS(stp);

    pthread_mutex_lock(&lcp->db_mutex);

    sec_login_db_store_entries(datafile, num_db_entries, db_entry, stp);

    pthread_mutex_unlock(&lcp->db_mutex);
}

void sec_login_util_store_ptgtcreds_and_epac(
    sec_login_handle_t  	security_context, 	/* [in]  */
    char                        *datafile,      	/* [in]  */
    sec_db_authdata_contents_t  v1_authdata,		/* [in]  */
    krb5_ccache			ccache,			/* [in]  */
    krb5_creds			*creds,			/* [in]  */
    sec_bytes_t			*stored_epac,		/* [out] */
    error_status_t		*stp			/* [out] */
)
{
    sec_login_context_t  	*lcp = (sec_login_context_t *) security_context;
    dce_db_handle_t	 	dbh;
    error_status_t		db_st;

    CLEAR_STATUS(stp);

    pthread_mutex_lock(&lcp->db_mutex);

    /*
     * Open and lock .db file. This is important because
     * we need to prevent other processes from storing a PTGT
     * simultaneously to avoid duplicate PTGTs. We must not
     * release this lock until we have stored the new PTGT
     * and the epac chain.
     */
    sec_login_db_open_and_lock(datafile, db_c_index_by_uuid, &dbh, stp);

    /* Check if someone beat us to refreshing the PTGT */
    if (STATUS_OK(stp)) {
        /*
         * !!! NEVER, EVER change the 1st argument (login context) to
         * sec_priv_ptgt_exists to be NON-NULL. Deadlock will occur
         * if you do!
         */
        if (sec_priv_ptgt_exists(NULL, v1_authdata.cell, 
	    	ccache, stored_epac, stp)
        || !STATUS_OK(stp)) {
            sec_login_db_unlock_and_close(&dbh,
                    (STATUS_OK(stp)) ? stp : &db_st);
            pthread_mutex_unlock(&lcp->db_mutex);
	    return;
        }
    } else {
        pthread_mutex_unlock(&lcp->db_mutex);
	return;
    }

    /*
     * Search through array of epacs and append the new
     * entry (v1_authdata) if an entry is not found for
     * the target cell. If an entry is found for the
     * target cell, replace it with the new entry.
     */
    sec_login_db_search_and_store_entry(dbh,
            epac_enum, (void *) &v1_authdata,
            &sec_login_db_authdata_search, stp);

    /*
     * cache the credentials we just constructed in the
     * caller's credential cache
     */
    if (STATUS_OK(stp)) {
        *stp = krb5_cc_store_cred(ccache, creds);
    }

    DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
	"sec_login_util_store_ptgtcreds_and_epac: stored PTGT in datafile %s\n", 
 	datafile));

    /* unlock and close db file */
    sec_login_db_unlock_and_close(&dbh,
                    (STATUS_OK(stp)) ? stp : &db_st);

    pthread_mutex_unlock(&lcp->db_mutex);
}

/*
 * sec_login_util_fetch_epac_and_stktcreds
 *
 * Fetches a matching epac/stkt creds pair. Retrieves the epac from
 * the .db credentials file and the stkt creds from the krb5 credentials
 * file. Fetches are locked to prevent other threads/processes from
 * adding or removing an epac/stkt which may cause "mismatched seal"
 * errors. If there is a uuid in the stkt and epac authdata, then
 * they must match because this proves that they are a matching pair. 
 * If they don't a new stkt should be requested in order to avoid
 * "mismatched seal/who are you" errors.
 *
 * Returns: a state identifying what is needed.
 *
 * NOTE: If the stkt creds are found, the screds passed in are
 * freed and reallocated with the fetched creds.
 */
sec_cred_state_t sec_login_util_fetch_epac_and_stktcreds(
    sec_login_handle_t  	security_context, 	/* [in]  */
    char                        *datafile,        	/* [in]  */
    unsigned_char_t		*target_cell,		/* [in]  */
    krb5_ccache			krb_ccache,		/* [in]  */
    krb5_creds			*screds,		/* [in, out]  */
    sec_bytes_t			*epac_chain,		/* [out] */
    uuid_t			*ptgt_uuid,		/* [out] */
    error_status_t              *stp              	/* [out] */
)
{
    sec_login_context_t *lcp = (sec_login_context_t *) security_context;
    dce_db_handle_t		dbh;
    krb5_flags      		matchflags = 0;
    sec_login_db_entry_t	db_entry;
    error_status_t		db_st, lst;
    sec_cred_state_t		state = epac_not_found | stkt_not_found;
    boolean32			epac_found = false;
    krb5_creds			mcreds, ncreds;
    sec_bytes_t			authbytes;
    boolean32			more;

    CLEAR_STATUS(stp);
    dce_assert(sec_svc_handle, !screds || !screds->server || !screds->client);

    pthread_mutex_lock(&lcp->db_mutex);

    uuid_create_nil(ptgt_uuid, &lst);
    memset(&db_entry, 0, sizeof(db_entry));
    memset(&authbytes, 0, sizeof(authbytes));
    if (epac_chain) {
	epac_chain->bytes = NULL;
	epac_chain->num_bytes = 0;
    }

    /*
     * Open and lock .db file. This is important because
     * we need to prevent other processes from storing a PTGT or STKT
     * simultaneously to avoid mismatched seal errors. We do not
     * release this lock until we have fetched the epac chain
     * and the matching service credentials.
     */
    sec_login_db_open_and_lock(datafile, (db_c_index_by_uuid | db_c_readonly), &dbh, stp);
    if (!STATUS_OK(stp)) {
	pthread_mutex_unlock(&lcp->db_mutex);
	return state;
    }

    db_entry.entry_type = epac_enum;
    sec_login_db_handle_fetch_entry(dbh, &db_entry, stp);
    if (!STATUS_OK(stp)) {
        /* not found isn't bad */
        if (STATUS_EQUAL(stp, db_s_key_not_found) ||
            STATUS_EQUAL(stp, db_s_open_failed_enoent)) {
            CLEAR_STATUS(stp);
        }
	goto out;
    }

    /* call priv_krb routine to get correct epac */
    epac_found = priv_krb_util_epac_search(db_entry, malloc_shim, free, target_cell, 
		      epac_chain, &authbytes, stp);
    if (!STATUS_OK(stp) || !epac_found) {
	goto out;
    }
    sec_login_db_free_entry_contents(rpc_ss_client_free, &db_entry, -1);

    /* epac located, so turn off flag */
    state &= ~epac_not_found;

    /* try to retrieve stkt */
    memset(&mcreds, 0, sizeof(mcreds));
    mcreds.server = screds->server;
    mcreds.client = screds->client;
    mcreds.times.endtime = screds->times.endtime;
    mcreds.keyblock = screds->keyblock;
    mcreds.authdata = screds->authdata;
    matchflags = matchflags | KRB5_TC_MATCH_TIMES | KRB5_TC_MATCH_AUTHDATA;
    *stp = krb5_cc_retrieve_cred(krb_ccache, matchflags, &mcreds, &ncreds);
    if (!STATUS_OK(stp)) {
	/* not found isn't bad */
        if (*stp == KRB5_CC_NOTFOUND) {
	    CLEAR_STATUS(stp);
	}
        if (epac_chain && epac_chain->bytes) {
            free(epac_chain->bytes);
            epac_chain->bytes = NULL;
            epac_chain->num_bytes = 0;
        } 
	goto out;
    }

    /*
     * Compare stkt uuid with ptgt uuid. Only do this if
     * we are dealing with a 1.1 security server. We know
     * this if we were able to fetch an epac chain above.
     */ 
    if ((epac_chain->bytes != NULL) && (epac_chain->num_bytes > 0)) {
	krb5_authdata	**ptgt_authdata = NULL;
	error_status_t	lst;
	priv_krb_util_bytes_to_authdata(&authbytes, NULL, &ptgt_authdata, &lst);
        if (!priv_krb_util_compare_authdata_uuids(ptgt_authdata,
		ncreds.authdata, false, &lst) || !STATUS_OK(&lst)) {
	    /*
             * They don't match. So we need to get a new STKT. Since
             * the current STKT is still good, we insert the PTGT/epac uuid
             * into the service ticket authdata. This will force any subsequent
	     * credential cache lookups for this STKT to fail because the
	     * authdata will not match. Thus, a new STKT will be requested
	     * from the KDC.
             */
	    if (screds->authdata) {
		if (priv_krb_util_parse_v1_ptgt_authdata(ptgt_authdata, NULL, NULL,
			ptgt_uuid, stp) && GOOD_STATUS(stp)) {
		    krb5_free_authdata(screds->authdata);
		    screds->authdata = NULL;
		    priv_krb_util_construct_v1_stkt_authdata(ptgt_uuid, NULL,
			&screds->authdata, stp);
		}
	    }
	    krb5_free_cred_contents(&ncreds);
	    if (ptgt_authdata)
		krb5_free_authdata(ptgt_authdata);
            if (epac_chain && epac_chain->bytes) {
                free(epac_chain->bytes);
                epac_chain->bytes = NULL;
                epac_chain->num_bytes = 0;
            } 
            DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug2,
    	        "sec_login_util_fetch_epac_and_stktcreds: epac/stkt uuid mismatch (file:%s)\n", 
 	        datafile));
            goto out;
	}
	if (ptgt_authdata)
	    krb5_free_authdata(ptgt_authdata);
    }

    krb5_free_cred_contents(screds);
    *screds = ncreds;

    /* stkt located, so all creds were found */
    state = creds_found;

out:
    /* unlock and close db file */
    sec_login_db_unlock_and_close(&dbh,
                    (STATUS_OK(stp)) ? stp : &db_st);

    pthread_mutex_unlock(&lcp->db_mutex);
    if (authbytes.bytes)
        free(authbytes.bytes);
    DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug2,
        "sec_login_util_fetch_epac_and_stktcreds: (file:%s, state:%d)\n", 
        datafile, state));
    return state;
}
