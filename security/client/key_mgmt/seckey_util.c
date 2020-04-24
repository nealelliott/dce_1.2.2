/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: seckey_util.c,v $
 * Revision 1.1.10.1  1996/10/03  14:44:25  arvind
 * 	Silence compiler warning and use strchr instead of index.
 * 	[1996/09/16  20:04 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.8.2  1996/03/11  13:27:43  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:33  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  18:03:46  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/08  18:26 UTC  burati
 * 	Merge fix for OT9109 from mb_mothra6
 * 
 * 	HP revision /main/mb_mothra6/1  1995/08/11  19:04 UTC  burati
 * 	OT9109 off-by-one error in sec_key_mgmt__resolve_mode()
 * 	[1995/12/08  16:31:10  root]
 * 
 * Revision 1.1.5.5  1993/02/10  19:22:39  burati
 * 	CR7182 New param "update" to sec_key_mgmt__bind_rgy causes it
 * 	to call sec_rgy_site_bind_update() if set to true.
 * 	[1993/02/10  18:45:17  burati]
 * 
 * Revision 1.1.5.4  1993/02/08  19:44:13  sommerfeld
 * 	[OT7153] Fix status code conversion.
 * 	[1993/02/08  18:30:23  sommerfeld]
 * 
 * Revision 1.1.5.3  1992/12/29  12:43:19  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:38:15  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  21:15:11  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:19:09  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/26  21:30:57  pato
 * 	OT#3770 Keytab file only needs read access for query functions
 * 	the previous change did not really address OT 3620.
 * 	[1992/05/22  20:36:15  pato]
 * 
 * 	OT#3620 default RPC key retrieval should not need keytab type prefix.
 * 	[1992/05/22  14:04:13  pato]
 * 
 * Revision 1.1  1992/01/19  14:48:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1993, 1995
** Unpublished work. All Rights Reserved.
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

/* 
 *  DCE Security key management interface
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
"$Id: seckey_util.c,v 1.1.10.1 1996/10/03 14:44:25 arvind Exp $";
#endif

#define _SEC_KEY_ALLOCATE_GLOBALS_
#include <pthread.h>
#include <stdio.h>

#include <seckey_util.h>
#include <krb5/osconf.h>
#include <dce/rpcbase.h>
#include <dce/rpc.h>
#include <dce/secidmap.h>
#include <dce/dce_cf.h>

#include <un_strng.h>
#include <un_maloc.h>
#include <un_time.h>
#include <sys/file.h>
#include <un_io.h>

/*
 * Error status reported from sec_key_mgmt__doinit
 */
static krb5_error_code  init_retval;
extern krb5_kt_ops      krb5_ktf_writable_ops;
static char *local_cell_name = NULL;
static int local_cell_len;

PRIVATE void sec_key_mgmt__doinit ( void )
{
    krb5_enctype        etype;

    /* Setup keytab operation table and cryptosystem entry */
    if (init_retval = krb5_kt_register(&krb5_ktf_writable_ops)) {
        return;
    }

    /*
     * We only support one keytype in DCE 1.0
     */
    master_keyblock.keytype = KEYTYPE_DES;

    if (!valid_keytype(master_keyblock.keytype)) {
        /*
         * This should never occur!
         */
        init_retval = KRB5_PROG_KEYTYPE_NOSUPP;
        return;
    }

    etype = krb5_keytype_array[master_keyblock.keytype]->system->proto_enctype;

    if (!valid_etype(etype)) {
        /*
         * This should never occur!
         */
        init_retval = KRB5_PROG_KEYTYPE_NOSUPP;
        return;
    }

    master_encblock.crypto_entry = krb5_csarray[etype]->system;

    {
        error_status_t  st;
        dce_cf_get_cell_name (&local_cell_name, &st);
        if (st != rpc_s_ok) {
            init_retval = st;
            return;
        }
        local_cell_len = strlen(local_cell_name);
    }
}


#ifdef SEC_DEBUG
void sec_key_mgmt__message
  (
    char            *prefix,
    char            *msg,
    char            *postfix
  )
{
    struct timeval      now;
    struct timezone     tz;
    char                tbuf[128];
    struct tm           time_info;
    static              boolean32 debug_state;
    static              boolean32 env_checked = false;

    if (!env_checked) {
        if (getenv("SEC_KEY_MGMT_DEBUG") != NULL) {
            debug_state = true;
        } else {
            debug_state = false;
        }
        env_checked = true;
    }

    if (!debug_state) {
        return;
    }

#ifdef SNI_SVR4_POSIX
    gettimeofday(&now);
#else
    gettimeofday(&now, &tz);
#endif /* SNI_SVR4_POSIX */
    time_info = *localtime(&now.tv_sec);
    strftime(tbuf, sizeof(tbuf), "%x %X", &time_info);

    fprintf(stderr, "%s SEC_KEY_MGMT - %s %s %s\n", tbuf, prefix, msg, postfix);
}
#endif /* SEC_DEBUG */


/*
 * s e c _ k e y _ m g m t _ _ i n i t
 *
 * Make sure the sec_key_mgmt package is initialized - actual initialization
 * occurs above in sec_key_mgmt__doinit, which is invoked via pthread_once
 * to guarantee we only execute the initialization sequence once.
 */
void sec_key_mgmt__init
  (
    error_status_t  *st
  )
{
    static pthread_once_t   key_mgmt_pthread_init_block = pthread_once_init;

    if (!sec_key__inited) {
        pthread_once(&key_mgmt_pthread_init_block, sec_key_mgmt__doinit);
        if (!init_retval) {
            sec_key__inited = true;
        }
        *st = (error_status_t) init_retval;
    } else {
        *st = error_status_ok;
    }
}


/*
 * s e c _ k e y _ m g m t _ _ r e s o l v e _ m o d e
 *
 * Obtain a keytab id for the requested readonly or writable keytab type.
 */
boolean32 sec_key_mgmt__resolve_mode
  (
    void            * arg,
    boolean32       readonly,
    krb5_keytab     * ktid,
    error_status_t  * st
  )
{
    char    *keytab = (char *) arg;
    char    ktname[2*sizeof(DEFAULT_KEYTAB_NAME)+sizeof("WRFILE:")+1];
    char    *p;
    char    *prefix;
    int     prefix_len;

    if (readonly) {
        prefix_len = strlen(krb5_kt_dfl_ops.prefix) + 1;
        prefix = krb5_kt_dfl_ops.prefix;
    } else {
        prefix_len = strlen(krb5_ktf_writable_ops.prefix) + 1;
        prefix = krb5_ktf_writable_ops.prefix;
    }

    if (keytab) {
        /*
         * Caller supplied a keytab (type?) filename.  We try to
         * resolve it.  If it fails to resolve, we prepend the
         * default type name to the string.
         */
#ifdef SEC_DEBUG
        sec_key_mgmt__message("Attempting to open", keytab, "");
#endif
        if ((*st = krb5_kt_resolve(keytab, ktid)) == KRB5_KT_BADNAME) {
            if (strlen(keytab) + prefix_len >= sizeof(ktname)) {
                p = malloc(strlen(keytab) + prefix_len + 2);
            } else {
                p = ktname;
            }
            if (p != NULL) {
                strcpy(p, prefix);
                p[prefix_len-1] = ':';
                strcpy(&p[prefix_len], keytab);
#ifdef SEC_DEBUG
                sec_key_mgmt__message("Second attempt to open", p, "");
#endif
                *st = krb5_kt_resolve(p, ktid);
                if (p != ktname) {
                    free(p);
                }
            } else {
                *st = sec_s_no_memory;
            }
        }
    } else {
        strcpy(ktname, prefix);
        ktname[prefix_len-1] = ':';
        p = strchr(DEFAULT_KEYTAB_NAME, ':');
        if (p) {
            p++;
        } else {
            p = DEFAULT_KEYTAB_NAME;
        }
        strcpy(&ktname[prefix_len], p);
#ifdef SEC_DEBUG
        sec_key_mgmt__message("Attempt to open default table", ktname, "");
#endif
        *st = krb5_kt_resolve(ktname, ktid);
    }

    if (*st) {
#ifdef SEC_DEBUG
        sec_key_mgmt__message("Failure", "", "");
#endif
        return false;
    } else {
#ifdef SEC_DEBUG
        sec_key_mgmt__message("Success", "", "");
#endif
        return true;
    }
}


/*
 * s e c _ k e y _ m g m t _ _ r e s o l v e
 *
 * Obtain a keytab id for the requested keytab type.
 */
boolean32 sec_key_mgmt__resolve
  (
    void            * arg,
    krb5_keytab     * ktid,
    error_status_t  * st
  )
{
    return sec_key_mgmt__resolve_mode(arg, false, ktid, st);
}


void sec_key_mgmt__bind_rgy
  (
    unsigned char                   * principal_name,
    sec_rgy_bind_auth_info_type_t   auth_type,
    sec_login_handle_t              lc,
    sec_rgy_name_t                  rel_princ_name,
    unsigned char                   * cell,
    boolean32                       update,
    sec_rgy_handle_t                * rgy_h,
    error_status_t                  * st
  )
{
    sec_rgy_bind_auth_info_t    auth_info;
    error_status_t              lerr;
    sec_rgy_name_t              lcell;

    if (cell == NULL) {
        cell = lcell;
    }

    /*
     * Parse the global name into a <cell> <principal> pair
     */
    auth_info.info_type = sec_rgy_bind_auth_none;
    sec_rgy_cell_bind((unsigned_char_p_t)"", &auth_info, rgy_h, st);
    if (GOOD_STATUS(st)) {
        sec_id_parse_name(*rgy_h, principal_name, cell, NULL, rel_princ_name, 
                            NULL, st);
        sec_rgy_site_close(*rgy_h, &lerr);

        /*
         * bind to the principal's home cell
         */
        switch (auth_type) {
            case sec_rgy_bind_auth_dce:
                auth_info.info_type = sec_rgy_bind_auth_dce;
                auth_info.tagged_union.dce_info.identity = lc;
                auth_info.tagged_union.dce_info.authn_level =
                                            rpc_c_authn_level_pkt_integrity;
                auth_info.tagged_union.dce_info.authn_svc =
                                            rpc_c_authn_dce_secret;
                auth_info.tagged_union.dce_info.authz_svc =
                                            rpc_c_authz_dce;
                break;

            default:
                auth_info.info_type = sec_rgy_bind_auth_none;
                break;
        }
        if (update) {
            sec_rgy_site_bind_update(cell, &auth_info, rgy_h, st);
        } else {
            sec_rgy_cell_bind(cell, &auth_info, rgy_h, st);
        }
    }
}


/*
 * sec_key_mgmt__convert_status:
 * 
 * Convert status code from keytab space to key_mgmt space.
 */
error_status_t sec_key_mgmt__convert_status
  (
    error_status_t err
  )
{
    switch (err) 
    {
    case EACCES:
	return sec_key_mgmt_e_unauthorized;
    case ENOENT:
    case KRB5_KT_NOFILE:
	return sec_key_mgmt_e_keytab_not_found;
    default:
	return err;
    }
}
