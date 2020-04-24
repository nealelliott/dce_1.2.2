/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: keymgmt.c,v $
 * Revision 1.1.13.2  1996/10/15  20:47:59  arvind
 * 	Complete the time init fix.
 * 	[1996/03/28  20:13 UTC  jrr  /main/HPDCE02/jrr_davis_1/2]
 *
 * 	Initialize timer in sec_key_mgmt_manage_key() to TEN_MINUTES, not 0.
 * 	[1996/03/25  21:34 UTC  jrr  /main/HPDCE02/jrr_davis_1/1]
 *
 * Revision 1.1.13.1  1996/10/03  14:43:59  arvind
 * 	Silence compiler warning on uninit variable.
 * 	[1996/09/16  20:02 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.11.2  1996/03/11  13:27:38  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:28  marty]
 * 
 * Revision 1.1.11.1  1995/12/08  18:03:21  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/10/19  17:18 UTC  burati
 * 	Merge fix for CHFts16610 from mb_mothra7/1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/12  19:35 UTC  burati
 * 	Merge fix for CHFts16036 from mb_mothra6
 * 
 * 	HP revision /main/mb_mothra6/6  1995/09/08  20:52 UTC  burati
 * 	Get next kvno from the master
 * 
 * 	HP revision /main/mb_mothra6/6  1995/09/08  20:52 UTC  burati
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/08  18:13 UTC  burati
 * 	Merge fix for CHFts16035 from mb_mothra6
 * 
 * 	HP revision /main/mb_mothra6/5  1995/08/21  22:58 UTC  burati
 * 	one last fix
 * 
 * 	HP revision /main/mb_mothra6/4  1995/08/21  21:32 UTC  burati
 * 	Final fixes
 * 
 * 	HP revision /main/mb_mothra6/3  1995/08/16  16:50 UTC  burati
 * 	More fixes to sec_key_mgmt_manage_key()
 * 
 * 	HP revision /main/mb_mothra6/2  1995/08/15  20:13 UTC  burati
 * 	More fixes.
 * 
 * 	HP revision /main/mb_mothra6/1  1995/08/11  19:29 UTC  burati
 * 	Fix exp time checking in ...manage_key()
 * 	[1995/12/08  16:31:03  root]
 * 
 * Revision 1.1.8.1  1994/05/11  16:48:59  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Pass sec_login_machine_princ flag to sec_login_setup_identity.
 * 	[1994/04/29  20:44:57  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 	Pass sec_login_machine_princ flag to sec_login_setup_identity.
 * 
 * 	hp_sec_to_osf_2 drop
 * 	Pass sec_login_machine_princ flag to sec_login_setup_identity.
 * 
 * Revision 1.1.6.6  1993/02/10  19:22:05  burati
 * 	CR7182 Call sec_key_mgmt__bind_rgy with update set to true
 * 	[1993/02/10  18:46:37  burati]
 * 
 * Revision 1.1.6.5  1992/12/29  12:42:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:37:58  zeliff]
 * 
 * Revision 1.1.6.4  1992/11/19  21:13:43  burati
 * 	CR5865 Need to use key_vno that was passed in to ...change_key()
 * 	[1992/11/19  18:40:33  burati]
 * 
 * Revision 1.1.6.3  1992/10/19  15:32:13  burati
 * 	CR5133 Check DES key parity before update in sec_key_mgmt_change_key
 * 	[1992/10/16  22:22:23  burati]
 * 
 * Revision 1.1.6.2  1992/09/29  21:14:56  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:18:52  sekhar]
 * 
 * Revision 1.1.2.4  1992/05/01  18:21:52  ahop
 * 	 CR 3145: Change &lc to lc in purge_context call in sec_key_mgmt__login.
 * 	[1992/04/30  21:16:58  ahop]
 * 
 * Revision 1.1.2.3  1992/03/18  19:17:23  burati
 * 	Free oldkey in sec_key_mgmt_change_key.
 * 	Free net_info in sec_key_mgmt_manage_key.
 * 	Free keydata from gen_rand_key in generate_key and manage_key
 * 	Don't purge bogus login handles in sec_key_mgmt__login
 * 	[1992/03/18  17:00:36  burati]
 * 
 * Revision 1.1.2.2  1992/01/22  23:04:45  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:21:16  melman]
 * 
 * $EndLog$
 */

/*  keymgmt.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1993, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *  DCE Security key management interface
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)keymgmt.c	26 - 02/24/92";
#endif

#include <dce/keymgmt.h>
#include <dce/rgybase.h>
#include <dce/rpc.h>
#include <dce/rpcpvt.h>
#include <dce/acct.h>
#include <dce/misc.h>
#include <dce/policy.h>
#include <dce/sec_login.h>
#include <dce/binding.h>
#include <dce/secidmap.h>
#include <dce/sec_authn.h>
#include <stdio.h>
#include <sys/time.h>

#include <seckey_util.h>
#include <sec_krb_util.h>
#include <krb5/des_int.h>
#include <un_time.h>
#include <un_maloc.h>
#include <u_str.h>
#include <rgymacro.h>

#define ONE_MINUTE  60
#define TEN_MINUTES 600

#define SEC_VERIFY_INIT() \
    { \
        rpc_binding_handle_t  XXXXh1 = NULL, XXXXh2 = NULL; \
        error_status_t        XXXXst; \
        (void) rpc_binding_handle_equal(XXXXh1, XXXXh2, &XXXXst); \
    }


PRIVATE void sec_key_mgmt__compute_next_kvno
  (
    unsigned32  cur_kvno,      /* [in] */
    unsigned32  *next_kvno_p   /* [out] */
  )
{
    *next_kvno_p = cur_kvno + 1;
    /*
     * Kerberos V5 keys are only 8 bits wide. (and 0 is a distinguished
     * value.)
     */
     *next_kvno_p = *next_kvno_p & 0xFF;
     if (*next_kvno_p == 0) {
        *next_kvno_p = 1;
    }
}


PRIVATE void sec_key_mgmt__login
  (
    unsigned_char_p_t           princ,          /* [in] */
    sec_key_mgmt_authn_service  authn_service,  /* [in] */
    void                        *arg,           /* [in] */
    unsigned32                  *cur_vno,        /* [in] */
    unsigned32                  *next_vno,      /* [out] */
    sec_login_handle_t          *lc,            /* [out] */
    error_status_t              *err            /* [out] */
  )
{
    unsigned32              tmp_vno = *cur_vno;
    boolean32               reset;
    sec_login_auth_src_t    auth_src;
    error_status_t          lst;

    sec_login_setup_identity(princ, (sec_login_machine_princ | 
	sec_login_master_rgy), lc, err);
    if (GOOD_STATUS(err)) {
        sec_login_valid_from_keytable(*lc, authn_service, arg, tmp_vno,
                                      cur_vno, &reset, &auth_src, err);
        if (GOOD_STATUS(err)) {
            if (next_vno) {
                sec_key_mgmt__compute_next_kvno(*cur_vno, next_vno);
            }
        } else {
            sec_login_purge_context (lc, &lst);
        }
    }

    if (STATUS_EQUAL(err, sec_rgy_passwd_invalid)) {
        SET_STATUS(err, sec_key_mgmt_e_key_unavailable);
    }
}


/* s e c _ k e y _ m g m t _ c h a n g e _ k e y
 *
 * Perform all activities necessary to update a principal's key to
 * the specified value.  This includes updating any local storage
 * for the principal's key and also performing any remote operations
 * needed to keep the authentication service current.  Old keys for the
 * principal are garbage collected if appropriate.
 *
 * Argument Usage:
 *      INPUT
 *  authn_service   **  select the underlying authentication scheme
 *  arg             **  key acquisition function argument (same value
 *                      that is passed to rpc_server_register_auth_info.)
 *  principal_name  **  names the principal whose key is to be changed
 *  key_vno             the new key version number. (0 may be specified
 *                      to indicate that the runtime should select the
 *                      next appropriate key version number)
 *  keydata         **  the new key value.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  garbage_collect_time    informs the caller of how many seconds in the
 *                      future the garbage collection function must be
 *                      called to get rid of any remaining obsolete keys.
 *                      a 0 return value indicates there is no need to
 *                      call the garbage collection function.
 *  err                 error status if any.
 *
 * Any error condition will leave the key state unchanged.
 *
 * Errors:
 *  sec_key_mgmt_e_key_unavailable      Old key not present, unable to use
 *                                      it to set a client side authentication
 *                                      context.
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid.
 *  sec_key_mgmt_e_authn_unavailable    Authentication service unavailable
 *                                      unable to update network database or
 *                                      obtain necessary network credentials.
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op.
 *  sec_key_mgmt_e_key_unsupported      Key type not supported.
 *  sec_key_mgmt_e_key_version_exists   Key with this version number already
 *                                      exists.
 */
void sec_key_mgmt_change_key
  (
    sec_key_mgmt_authn_service  authn_service,
    void                        * arg,
    unsigned char               * principal_name,
    unsigned32                  key_vno,
    void                        * keydata,
    sec_timeval_period_t        * garbage_collect_time,
    error_status_t              * err
  )
{
    unsigned32                  old_vno = sec_passwd_c_version_none, next_vno;
    sec_passwd_rec_t            *oldkey;
    sec_passwd_rec_t            *newkey = (sec_passwd_rec_t *) keydata;
    sec_login_handle_t          lc;
    error_status_t              lst;
    sec_rgy_login_name_t        login_name;
    sec_rgy_handle_t            rgy_h = NULL;
    sec_rgy_name_t              cell;
    char                        *prim_princ;

    SEC_VERIFY_INIT();

    /* DES keys must pass a parity check */
    if (newkey->key.key_type == sec_passwd_des) {
        if (!mit_des_check_key_parity(newkey->key.tagged_union.des_key)) {
            SET_STATUS(err, sec_s_bad_key_parity);
            return;
        }
    }

    /* login as the specified principal */
    sec_key_mgmt__login(principal_name, authn_service, arg, &old_vno,
                        NULL, &lc, err);

    if (GOOD_STATUS(err)) {
        /* If key_vno was specified, attempt to use it */
        if (key_vno != sec_passwd_c_version_none) {
            next_vno = key_vno;
        } else {
            /* Didn't ask for next_vno from sec_key_mgmt__login because it
	     * doesn't yet ensure that we bound to the master for the login,
	     * thus it could have used an older key to validate us with.
	     * So, we must ask for next_vno from the master now.
             */
            sec_key_mgmt_get_next_kvno(authn_service, arg, principal_name,
                &newkey->key.key_type, &old_vno, &next_vno, err);

            /* Next vno picked by server, so there can't be an existing valid
             * key with this vno.  Attempt a delete for this vno to make sure.
             */
            sec_key_mgmt_delete_key(authn_service, arg, principal_name,
                                    next_vno, &lst);
        }
	
        if (GOOD_STATUS(err)) {
            ((sec_passwd_rec_t *)keydata)->version_number = next_vno;

            /*
             * Bind to principal's home cell
             */
            sec_key_mgmt__bind_rgy(principal_name, sec_rgy_bind_auth_dce, lc,
                                    login_name.pname, cell, true, &rgy_h, err);
        }

        if (GOOD_STATUS(err)) {
            sec_krb_string_from_dce_pair((char *) cell,
                                            (char *) login_name.pname,
                                            &prim_princ, err);
            if (GOOD_STATUS(err)) {
                /* Set the key locally */
                sec_key_mgmt_set_key(authn_service, arg,
                                        (unsigned char *) prim_princ,
                                        next_vno, keydata, err);
                free(prim_princ);

                if (GOOD_STATUS(err)) {
                    login_name.gname[0] = login_name.oname[0] = '\0';

                    /*
                     * If asked for garbage_collect_time return
                     * max_ticket_lifetime or error
                     */
                    if (garbage_collect_time) {
                        sec_rgy_plcy_auth_t     auth_policy;

                        sec_rgy_auth_plcy_get_effective(rgy_h, &login_name,
                                                        &auth_policy, err);
                        if (GOOD_STATUS(err)) {
                            *garbage_collect_time =
                                    auth_policy.max_ticket_lifetime;
                        }
                    }

                    /* Get the current key, since validate_ident free'd it */
                    if (GOOD_STATUS(err)) {
                        sec_key_mgmt_get_key(authn_service, arg, principal_name,
                                                old_vno, (void **) &oldkey, err);
                    }

                    /* Set the key in the registry */
                    if (GOOD_STATUS(err)) {
                        sec_rgy_acct_passwd(rgy_h, &login_name, oldkey,
                                            (sec_passwd_rec_t *) keydata,
                                            sec_passwd_des, &next_vno, err);
                        sec_key_mgmt_free_key(oldkey, &lst);
                    }
                }
            } /* set_key succeeded */

            sec_rgy_site_close(rgy_h, &lst);
        } /* cell_bind succeeded */

        /* Release storage for the login context and network credentials */
        sec_login_purge_context (&lc, &lst);
    } /* setup_ident and validate_ident succeeded */

    if (GOOD_STATUS(err)) {
        /* Perform any necessary garbage collection */
        sec_key_mgmt_garbage_collect(authn_service, arg, principal_name, err);
    }
}


/* s e c _ k e y _ m g m t _ f r e e _ k e y
 *
 * Release any storage allocated for this key data by sec_key_mgmt_get_key
 * or sec_key_mgmt_get_nth_key.
 *
 * Argument Usage:
 *      INPUT
 *  keydata         **  the key value extracted.
 *
 *      OUTPUT
 *  err                 error status if any.
 *
 * Errors:
 *      < NONE >
 */
void sec_key_mgmt_free_key
  (
    void                        * keydata,
    error_status_t              * err
  )
{
    if (keydata) {
        free((char *) keydata);
    }

    CLEAR_STATUS(err);
}


/* s e c _ k e y _ m g m t _ g e n e r a t e _ k e y
 *
 * Perform all activities necessary to generate a new random key for
 * the named  principal.  The behavior for this function is the same as
 * sec_key_mgmt_change_key, except that the new key is randomly selected
 * by the system.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 * Errors:
 *  sec_key_mgmt_e_key_unavailable      Old key not present, unable to use
 *                                      it to set a client side authentication
 *                                      context.
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid.
 *  sec_key_mgmt_e_authn_unavailable    Authentication service unavailable
 *                                      unable to update network database or
 *                                      obtain necessary network credentials.
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op.
 *  sec_key_mgmt_e_key_version_exists   Key with this version number already
 *                                      exists.
 */
void sec_key_mgmt_generate_key
  (
    sec_key_mgmt_authn_service  authn_service,
    void                        * arg,
    unsigned char               * principal_name,
    unsigned32                  key_vno,
    sec_timeval_period_t        * garbage_collect_time,
    error_status_t              * err
  )
{
    error_status_t      lst;
    unsigned32          next_key_vno;
    sec_passwd_type_t   keytype = sec_passwd_des;
    sec_passwd_rec_t    *keydata;

    SEC_VERIFY_INIT();

    if (garbage_collect_time) {
        *garbage_collect_time = 0;
    }

    sec_key_mgmt_get_next_kvno(authn_service, arg, principal_name,
                                &keytype, NULL, &next_key_vno, err);

    if (GOOD_STATUS(err)) {
        sec_key_mgmt_gen_rand_key(authn_service, arg, principal_name, &keytype,
                                    next_key_vno, (void **) &keydata, err);
    }

    if (GOOD_STATUS(err)) {
        sec_key_mgmt_change_key(authn_service, arg, principal_name,
                                next_key_vno, keydata,
                                garbage_collect_time, err);
        sec_key_mgmt_free_key(keydata, &lst);
    }
}


/* s e c _ k e y _ m g m t _ m a n a g e _ k e y
 *
 * Change the principal's key on a regular basis determined by the local
 * cell's policy.  This function will run indefinitely - it will never
 * return during normal operation.  It should be invoked from a thread
 * that has been devoted to managing keys.
 *
 * This function queries the DCE registry to determine the passwd
 * expiration policy that applies to the named principal.  It then
 * idles until a short time before the current key is due to expire
 * and then uses the sec_key_mgmt_generate_key function to produce
 * a new random key.  (This function updates both the local key store
 * and the DCE registry).  This function also invokes
 * sec_key_mgmt_garbage_collect as needed.
 *
 * Argument Usage:
 *      INPUT
 *  authn_service   **  select the underlying authentication scheme
 *  arg             **  key acquisition function argument (same value
 *                      that is passed to rpc_server_register_auth_info.)
 *  principal_name  **  names the principal whose key is to be changed
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  err                 error status if any.
 *
 * Errors:
 *  sec_key_mgmt_e_key_unavailable      Old key not present, unable to use
 *                                      it to set a client side authentication
 *                                      context.
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid.
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op.
 */
void sec_key_mgmt_manage_key
  (
    sec_key_mgmt_authn_service  authn_service,
    void                        * arg,
    unsigned char               * princ,
    error_status_t              * err
  )
{
    unsigned32              old_kvno = sec_passwd_c_version_none;
    sec_timeval_period_t    garbage_collect_time;
    sec_passwd_rec_t        *keydata;
    sec_passwd_type_t       keytype = sec_passwd_des;
    sec_login_net_info_t    net_info;
    sec_login_handle_t      lc;
    error_status_t          lst;
    struct timespec         timer;
    struct timeval          now;
    boolean32               do_change = false;
    sec_passwd_rec_t        *oldkey;
    signed32                valid_time;

    SEC_VERIFY_INIT();

    timer.tv_nsec   = 0;
    CLEAR_STATUS(err);

    while (GOOD_STATUS(err)) {
        timer.tv_sec    = TEN_MINUTES;

        /* login as the specified principal */
        sec_key_mgmt__login(princ, authn_service, arg, &old_kvno,
                            NULL, &lc, err);

        if (GOOD_STATUS(err)) {
            sec_login_inquire_net_info(lc, &net_info, err);

            if (GOOD_STATUS(err) ||
                STATUS_EQUAL(err, sec_login_s_not_certified)) {

                CLEAR_STATUS(err);

                /* What time is it? */
#ifdef SNI_SVR4_POSIX
                gettimeofday(&now);
#else
                gettimeofday(&now, 0);
#endif /* SNI_SVR4_POSIX */

                if ((net_info.passwd_expiration_date == 0) ||
                    ((net_info.passwd_expiration_date - now.tv_sec) >
			(2*TEN_MINUTES) )) {

                    /* Long or no expiration, but only sleep for ten minutes
                     * and check again, in case things change.
                     */
                    do_change = false;
                    timer.tv_sec = TEN_MINUTES;
                } else {
                    /* otherwise, it expires within the next 20 mins, sleep for
		     * the appropriate amount of time then change it.
		     */
                    do_change = true;


                    /* We should wake up within 10 minutes before expiration to
		     * start the attempt to change the key.
		     */
		    valid_time = net_info.passwd_expiration_date - now.tv_sec;
		    if (valid_time > TEN_MINUTES)
                        timer.tv_sec = valid_time - TEN_MINUTES;
                    else 
                        timer.tv_sec = 0;

                }
                sec_login_free_net_info(&net_info);
            }
            sec_login_purge_context (&lc, &lst);
        }

        if (timer.tv_sec > 0) {
            pthread_delay_np(&timer);
        }

        /* Only change if above succeeded and exp time wasn't 0 (forever) */
        if (GOOD_STATUS(err) && do_change) {
            sec_key_mgmt_gen_rand_key(authn_service, arg, princ, &keytype,
                sec_passwd_c_version_none, (void **) &keydata, err);
            if (GOOD_STATUS(err)) {
                /* let change_key choose the next key version number */
                sec_key_mgmt_change_key(authn_service, arg, princ,
                                            sec_passwd_c_version_none,
                                            keydata, &garbage_collect_time,
                                            err);
                sec_key_mgmt_free_key(keydata, &lst);
            }
        }
    } /* End of while GOOD_STATUS(err) */
}


/* s e c _ k e y _ m g m t _ g e t _ n e x t _ k v n o
 *
 * Determine the next eligible key version number for a given keytype
 * for a given principal.
 *
 * Argument Usage:
 *      INPUT
 *  authn_service   **  select the underlying authentication scheme
 *  arg             **  key acquisition function argument (same value
 *                      that is passed to rpc_server_register_auth_info.)
 *  principal_name  **  names the principal whose key is to be changed
 *  keytype         **  the keytype
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  key_vno             the next eligible key version number
 *  err                 error status if any.
 *
 * Errors:
 *  sec_key_mgmt_e_key_unavailable      Old key not present, unable to use
 *                                      it to set a client side authentication
 *                                      context.
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid.
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op.
 */

void sec_key_mgmt_get_next_kvno
  (
    sec_key_mgmt_authn_service  authn_service,
    void                        * arg,
    unsigned char               * principal_name,
    void                        * keytype,
    unsigned32                  * key_vno,
    unsigned32                  * next_key_vno,
    error_status_t              * err
  )
{
    sec_rgy_login_name_t            name_in;
    sec_rgy_cursor_t                cursor;
    sec_rgy_login_name_t            name_out;
    sec_rgy_acct_user_t             user_part;
    sec_rgy_handle_t                rgy_h = NULL;
    error_status_t                  lerr;
    sec_rgy_bind_auth_info_type_t   auth_type;
    sec_login_handle_t              lc;

    SEC_VERIFY_INIT();

    sec_login_get_current_context(&lc, err);
    if (STATUS_EQUAL(err, sec_login_s_no_current_context)) {
        auth_type = sec_rgy_bind_auth_none;
        lc = NULL;
    } else {
        auth_type = sec_rgy_bind_auth_dce;
    }

    sec_key_mgmt__bind_rgy(principal_name, auth_type, lc,
                           name_in.pname, NULL, true, &rgy_h, err);
    if (BAD_STATUS(err)) {
        return;
    }

    name_in.gname[0] = '\0';
    name_in.oname[0] = '\0';

    sec_rgy_cursor_reset(&cursor);
    sec_rgy_acct_lookup(rgy_h, &name_in, &cursor, &name_out, NULL, NULL,
                             NULL, &user_part, NULL, err);
    sec_rgy_site_close(rgy_h, &lerr);

    if (GOOD_STATUS(err)) {
        if (key_vno) {
            *key_vno = user_part.passwd_version_number;
        }
        if (next_key_vno) {
            sec_key_mgmt__compute_next_kvno(user_part.passwd_version_number,
                                            next_key_vno);
        }
    }
}


/* s e c _ k e y _ m g m t _ g e n _ r a n d _ k e y
 *
 * Generate a new random key of the specified type.  The output keydata
 * is suitable for use by sec_key_mgmt_change_key or sec_key_mgmt_set_key.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 * Errors:
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid.
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op.
 */
void sec_key_mgmt_gen_rand_key
  (
    sec_key_mgmt_authn_service  authn_service,
    void                        * arg,
    unsigned char               * principal_name,
    void                        * keytype,
    unsigned32                  key_vno,
    void                        ** keydata,
    error_status_t              * err
  )
{
    sec_des_key                 random_key;

    SEC_VERIFY_INIT();

    *keydata = NULL;

    if (*(sec_passwd_type_t *)keytype == sec_passwd_des) {
        *err = sec_des_new_random_key(&random_key);
        if (GOOD_STATUS(err)) {
            *keydata = (sec_passwd_rec_t *) malloc(sizeof(sec_passwd_rec_t));
            if (*keydata) {
                ((sec_passwd_rec_t *)*keydata)->pepper = NULL;
                ((sec_passwd_rec_t *)*keydata)->version_number = key_vno;
                ((sec_passwd_rec_t *)*keydata)->key.key_type =
                                                            sec_passwd_des;
                 memcpy(( (sec_passwd_rec_t*)
                                *keydata )->key.tagged_union.des_key,
                             random_key.bits, sec_passwd_c_des_key_size);
            } else {
                *err = sec_s_no_memory;
            }
        }
    } else {
        if (err) {
            *err = sec_key_mgmt_e_not_implemented;
        }
    }
}
