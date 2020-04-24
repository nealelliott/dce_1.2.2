/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: rs_login.c,v $
 * Revision 1.1.12.3  1996/02/18  00:18:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:04  marty]
 *
 * Revision 1.1.12.2  1995/12/08  17:52:32  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/20  18:50 UTC  mullan_s
 * 
 * 	HP revision /main/mullan_dfsbind_mem_leaks/1  1994/12/20  18:34 UTC  mullan_s
 * 	[OT 12768]: memory leak
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/15  03:29 UTC  rps
 * 
 * 	HP revision /main/rps_mothra/1  1994/12/15  00:05 UTC  rps
 * 	[OT12669] cell renaming
 * 	[1995/12/08  17:20:42  root]
 * 
 * Revision 1.1.7.3  1994/08/25  20:02:20  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  20:01:51  sekhar]
 * 
 * Revision 1.1.7.2  1994/08/04  16:13:31  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:28  mdf]
 * 
 * Revision 1.1.7.1  1994/05/11  19:30:27  ahop
 * 	hp_sec_to_osf_2 drop
 * 	In calls to sec_login_setup_identity, pass sec_login_machine_princ
 * 	flag to avoid third party preauth.
 * 	[1994/04/29  21:44:22  ahop]
 * 
 * Revision 1.1.5.2  1993/08/04  21:11:30  cuti
 * 	 Change enable_nsi to false to bypass CDS lookup while starting secd.
 * 	[1993/08/04  21:10:37  cuti]
 * 
 * Revision 1.1.2.8  1993/04/21  19:10:26  emartin
 * 	CR 7751.  start login context refresh task from routine that
 * 	invokes context setup (ie. move startup of rs_login_pvt_refresh_task
 * 	from rs_login_pvt_setup_context to rs_login_setup_host_identity and
 * 	rs_login_become_rgy).
 * 	[1993/04/21  17:35:42  emartin]
 * 
 * 	CR 7751.  rs_login_inherit_host_identity, rs_login_setup_host_identity,
 * 	rs_login_become_rgy - use temp variable for login context until all
 * 	work is done; then fill host/rgy_ctx_info.
 * 	[1993/04/21  14:32:26  emartin]
 * 
 * Revision 1.1.2.7  1993/03/08  14:37:51  ahop
 * 	7340: initialized slave should construct and maintain host machine context
 * 	[1993/03/04  21:30:38  ahop]
 * 
 * Revision 1.1.2.6  1992/12/29  16:35:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:24:15  zeliff]
 * 
 * Revision 1.1.2.5  1992/10/21  18:43:11  sommerfeld
 * 	Clean up code (change gettimeofday to time).
 * 	Prev fix was for OT5645.
 * 	[1992/10/20  20:04:50  sommerfeld]
 * 
 * 	Fix uninitialized field of "timer" struct.
 * 	[1992/10/20  19:50:49  sommerfeld]
 * 
 * Revision 1.1.2.4  1992/10/07  20:33:49  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:17:09  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  17:01:19  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1.2.3  1992/09/29  20:12:49  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:37:06  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:24:46  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  17:53:38  sekhar]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992, 1993
 * Unpublished work. All Rights Reserved.
 */
/*
 * ABSTRACT:
 *
 *     Registry login context management
 *
 *     A slave runs under the host machine identity for the purposes
 *     of communicating with the master.  All replicas run as the
 *     security service principal for the purposes of registering
 *     towers in the namespace.
 *
 *     It is crucial that a slave inheriting the machine identity
 *     do so before logging in as the security service principal.
 *     Therefore, we kill the server immediately when we detect
 *     a bad ordering of calls to the login module.
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_login.c,v 1.1.12.3 1996/02/18 00:18:19 marty Exp $";
#endif

#include <dce/dce.h>
#include <pthread.h>
#include <un_time.h>
#include <stdio.h>

#include <macros.h>
#include <dce/rpcbase.h>
#include <dce/dce_cf.h>
#include <dce/binding.h>
#include <dce/sec_login.h>
#include <dce/rplbase.h>
#include <krb5/base-defs.h>     
#include <krb5/krb5.h>     
#include <sec_krb_util.h>
#include <crypt.h>

#include <rs_pvt.h>
#include <rs_task.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_util.h>
#include <rs_state.h>
#include <rsdb_acct.h>
#include <rs_login.h>

#include <sec_svc.h>

typedef struct rs_context_info_t {
    unsigned_char_t     *identity;
    sec_login_handle_t  context;
} rs_context_info_t, *rs_context_info_p_t;

PRIVATE rs_context_info_t       rgy_ctx_info = { 0, 0 };
PRIVATE rs_context_info_t       host_ctx_info = { 0, 0 };
PRIVATE rs_context_info_p_t     rgy_ctx_info_p = &rgy_ctx_info;
PRIVATE rs_context_info_p_t     host_ctx_info_p = &host_ctx_info;
PRIVATE boolean32               rgy_ctx_refresh_task_started = false;
PRIVATE boolean32               host_ctx_refresh_task_started = false;
PRIVATE sec_rgy_name_t          host_princ_name = { 0 };

/* rs_login_pvt_refresh_context
 *
 * Run in a separate thread to refresh the input 
 * identity's context as necessary.   
 *
 * Cloned from machine context refresh logic in sec_clientd
 */
PRIVATE  void  *rs_login_pvt_refresh_context
#ifndef __STDC__
    ( arg ) 
    void *arg;
#else
  (
    void *arg
  )
#endif
{
    krb5_keyblock           *key;    
    sec_passwd_rec_t        *passwd;
    error_status_t          st;
    struct timespec         timer;
    boolean32               reset_passwd;
    sec_login_auth_src_t    auth_src;
    signed32                expiration, valid_time;
    struct timeval          now;
    struct timezone         tz;
    signed32                half_valid_time;
    signed32                delay_time;
    boolean32               default_pepper;
    krb5_kvno               mkey_version;
    rs_context_info_p_t     context_info_p;
    char                    buf[128];
                  
#define ONE_MINUTE          60
#define FIVE_MINUTES        300
#define TEN_MINUTES         600

    dce_svc_printf(SEC_RS_THR_STARTED_MSG , RS_LOGIN_THR);

    CLEAR_STATUS(&st);
    context_info_p = (rs_context_info_p_t)arg;

    if(context_info_p->context == 0) {
        dce_svc_printf(SEC_RS_LOGIN_NULL_HANDLE_MSG);
        return;
    }
    if(context_info_p->identity == 0 || *(context_info_p->identity) == '\0') {
        dce_svc_printf(SEC_RS_LOGIN_NULL_NAME_MSG);
        return;
    }
    
    /* Refresh the context when necessary */
    while (true) {

        /* decide how long to sleep
         */
        delay_time = 0;

        if (BAD_STATUS(&st)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_login,
                           svc_c_sev_error,
                           st);
            dce_svc_printf(SEC_RS_LOGIN_CANT_REFRESH_MSG,
                           context_info_p->identity);
            delay_time = TEN_MINUTES;
        }
        else {
            /* Does the context need renewing */
            sec_login_get_expiration(context_info_p->context, &expiration, &st);
            if (GOOD_STATUS(&st)
            || STATUS_EQUAL(&st, sec_login_s_not_certified)) {
                /* Pretend it expires before it really does, so we'll have time
                 * to recover from network problems or clock skew.  Subtract the
                 * smaller of (valid_time/2) and TEN_MINUTES, from the valid_time
                 * and sleep for that number of seconds.
                 */

#ifdef SNI_SVR4_POSIX
                gettimeofday(&now);
#else
                gettimeofday(&now, &tz);
#endif
                valid_time = expiration - now.tv_sec;
                half_valid_time = valid_time/2;
                delay_time = valid_time -
                                (MIN( TEN_MINUTES, half_valid_time));
            } 
            else { 
                /* couldn't get expiration time */
                delay_time = TEN_MINUTES;
            }
        }
    
        if (delay_time > 0) {
            timer.tv_sec = delay_time;
	    timer.tv_nsec = 0;
            dce_svc_printf(SEC_RS_LOGIN_REFRESH_WAIT_MSG,
                           context_info_p->identity,
                           timer.tv_sec);
            pthread_delay_np(&timer);
        }

        /* exit task if process is exiting
         */
        if (rs_state_exiting()) break;
 
        dce_svc_printf(SEC_RS_LOGIN_REFRESH_MSG, context_info_p->identity);

        CLEAR_STATUS(&st);

        /* Refresh the context */
        sec_login_refresh_identity(context_info_p->context, &st);
        if (GOOD_STATUS(&st)) {
            BEGIN_READ(lock_db, &st) {
                rsdb_acct_get_auth_key_by_name(
                                        (unsigned char *)context_info_p->identity, 
					NULL,
                                        KEYTYPE_DES, sec_passwd_c_version_none, 
					&key, &default_pepper, NULL,
                                        &mkey_version, &st);

            } END_READ
        }

        if (GOOD_STATUS(&st)) {
            passwd = krb_to_sec_key(key, &st);
            /* done with the krb5 key, regardless */
            krb5_free_keyblock(key);
            if (GOOD_STATUS(&st)) {                        
                sec_login_validate_identity(context_info_p->context, passwd,
                                            &reset_passwd,
                                            &auth_src, &st);
                sec_crypt_free_key(&passwd);
            }
        }

    }  /* while (true) */
    dce_svc_printf(SEC_RS_THR_EXITING_MSG , RS_LOGIN_THR);
}


/* rs_login_pvt_setup_context
 *
 * establish a login context for the input identity.
 * start a thread to refresh this context when necessary.
 *
 * This routine performs a database lookup to obtain the registry service
 * principal's secret key.  We rely on the caller to provide the appropriate 
 * concurrency control.
 */
PRIVATE void rs_login_pvt_setup_context
#ifndef __STDC__
    ( context_info_p, st_p )
    rs_context_info_p_t context_info_p;  /* [in] */
    error_status_t      *st_p;
#else
    (
    rs_context_info_p_t context_info_p,
    error_status_t      *st_p
    )
#endif
{
    krb5_keyblock           *key;    
    boolean32               default_pepper;
    krb5_kvno               mkey_version;
    sec_passwd_rec_t        *passwd;
    boolean32               reset;
    error_status_t          lst;
    sec_login_auth_src_t    src;
    boolean32               enable_nsi = false;

    CLEAR_STATUS(st_p);

    if(context_info_p->identity == 0 || *(context_info_p->identity) == '\0') {
        SET_STATUS(st_p, sec_s_pgmerr);
        SEC_SVC_PRINT_MSG_ST(SEC_RS_LOGIN_BAD_NAME_MSG, sec_s_pgmerr);
        return;
    }
    /*
     * To avoid having client security functions use CDS, set enable_nsi to false.
     * If this is done, call sec_rgy_enable_nsi(true) afterward.
     */
    (void) sec_rgy_enable_nsi(enable_nsi); 
    
    if (context_info_p->context == 0) {
        sec_login_setup_identity((unsigned_char_p_t)context_info_p->identity,
                                 sec_login_machine_princ, &context_info_p->context,
                                 st_p);
        if (GOOD_STATUS(st_p)) {
            /*
             * retrieve the rgy principal's key from the database
             */
            rsdb_acct_get_auth_key_by_name(
                                (unsigned_char_p_t)context_info_p->identity, 
				NULL,
                                KEYTYPE_DES, sec_passwd_c_version_none, 
                                &key, &default_pepper, NULL,
                                &mkey_version, st_p);

            if (GOOD_STATUS(st_p)) {
                passwd = krb_to_sec_key(key, st_p);
                /* done with the krb5 key, regardless */
                krb5_free_keyblock(key);
                
                if (GOOD_STATUS(st_p)) {
                    sec_login_validate_identity(context_info_p->context, passwd, 
                                                &reset, &src, st_p);
                    /* done with passwd, regardless */
                    sec_crypt_free_key(&passwd);
                }
            }


            /*
             * cleanup context if something went wrong after setup.
             * Use a local status so failures in the purge don't
             * obscure the original cause of failure
             */
            if (BAD_STATUS(st_p)) {
                sec_login_purge_context(&(context_info_p->context), &lst);
                context_info_p->context = 0;
            }
        }
    }

}
                      


/* rs_login_inherit_host_identity
 *
 * Called by an uninitialized slave to inherit the host machine identity.  
 * Obtain and release the current login context first so we're sure we're 
 * not inheriting a logged-in administrator's identity.
 *
 * NOTE: this routine *must* be called *before* rs_login_become_rgy.  To do 
 * otherwise is a fatal error.
 */
PUBLIC void rs_login_inherit_host_identity
#ifndef __STDC__
    ( st_p )
    error_status_t  *st_p;
#else
    (
    error_status_t  *st_p
    )
#endif
{
    unsigned_char_t     *hostname_p = 0;
    rs_context_info_t   ctx_info;
    rs_context_info_p_t ctx_info_p = &ctx_info;
    error_status_t      lst;
    
    CLEAR_STATUS(st_p);

    ctx_info_p->identity = 0;
    ctx_info_p->context = 0;

    /* 
     * don't bother if the host identity has been inherited previously.
     */
    if (host_ctx_info_p->context == 0) { 
        /*
         * if the rgy_context is not NULL, then someone has already 
         * called rs_login_become_rgy to login as the registry service 
         * principal and we would have to destroy that identity in
         * order to inherit the host identity, so go fatal.
         */   
        if (rgy_ctx_info_p->context != 0) {
            dce_svc_printf(SEC_RS_LOGIN_WRONG_CALL_MSG);  /* causes a fatal exit */
        } else {
            /* 
             * get rid of any existing default context so we can be 
             * sure we are inheriting the machine context
             */
            sec_login_get_current_context(&(ctx_info_p->context), st_p);
            if (GOOD_STATUS(st_p)) {
                sec_login_release_context(&(ctx_info_p->context), st_p);
            } else {
                /*
                 * no context yet which is ok
                 */
                CLEAR_STATUS(st_p);
            }
     
            /* now attempt to inherit the machine context */
            if (GOOD_STATUS(st_p)) {
                sec_login_get_current_context(&(ctx_info_p->context),st_p);
         
                if (GOOD_STATUS(st_p)) {
                    /* Get the local host principal name */
                    dce_cf_prin_name_from_host(0, (char **)&hostname_p, &lst);
                    if(BAD_STATUS(&lst)) { 
                        host_princ_name[0] = '\0';
                    } else {
                        if(u_strncmp(CELL_DIR_ROOT, hostname_p, CELL_DIR_ROOT_LEN) == 0) {
                            u_strcpy(host_princ_name, hostname_p + CELL_DIR_ROOT_LEN);
                        } else {
                            u_strcpy(host_princ_name, hostname_p);
                        }
                        free(hostname_p);
                    }
                    ctx_info_p->identity = (unsigned_char_t *)host_princ_name;
                }
            }
    
            if (GOOD_STATUS(st_p)) {
                host_ctx_info_p->identity = ctx_info_p->identity;
                host_ctx_info_p->context = ctx_info_p->context;
            }
        }
    } 
}
  
/* rs_login_setup_host_identity
 *
 * Called by an initialized slave to setup and maintain the host machine identity
 * which it can do because its database contains the host principal's key.
 * By doing this, the slave is not dependent on sec_clientd for this task.
 */
PUBLIC void rs_login_setup_host_identity
#ifndef __STDC__
    ( st_p )
    error_status_t  *st_p;
#else
    (
    error_status_t  *st_p
    )
#endif
{
    rs_context_info_t   ctx_info;
    rs_context_info_p_t ctx_info_p = &ctx_info;
    unsigned_char_t     *hostname_p = 0;
    rs_task_handle_t    task_handle;
    char                buf[64];

    CLEAR_STATUS(st_p);

    /* Get the local host principal name */
    dce_cf_prin_name_from_host(0, (char **)&hostname_p, st_p);
    if(BAD_STATUS(st_p)) { 
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_GET_LOCAL_HOST_PRINC_MSG, *st_p);
    } else {
        if(u_strncmp(CELL_DIR_ROOT, hostname_p, CELL_DIR_ROOT_LEN) == 0) {
            u_strcpy(host_princ_name, hostname_p + CELL_DIR_ROOT_LEN);
        } else {
            u_strcpy(host_princ_name, hostname_p);
        }
        free(hostname_p);
    }

    /* use a temp. variable in case things go bad.
     * host_ctx_info_p->context should be 0 at this point,
     * if it isn't rs_login_pvt_setup_context won't do anything.
     */
    ctx_info_p->identity = (unsigned_char_t *)host_princ_name;
    ctx_info_p->context = host_ctx_info_p->context;
 
    rs_login_pvt_setup_context(ctx_info_p, st_p);

    if (GOOD_STATUS(st_p)) {
        host_ctx_info_p->identity = ctx_info_p->identity;
        host_ctx_info_p->context = ctx_info_p->context;

        if (!host_ctx_refresh_task_started) {
            sprintf(buf, 
                "SECD - %s context refresh", host_ctx_info_p->identity);
            if (! rs_task_create(&task_handle, rs_login_pvt_refresh_context,
                 buf, false, (void *)host_ctx_info_p) ) {
                 dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                                sec_s_rs_login,
                                svc_c_sev_fatal | svc_c_action_abort,
                                sec_rs_thr_exit_creat_fail);
            }
            host_ctx_refresh_task_started = true;
        }
    }
}


/* rs_login_host_set_auth_info
 *
 * Called by a slave to attach the host machine auth info to its 
 * binding handle to the named server
 *
 */
PUBLIC void rs_login_host_set_auth_info
#ifndef __STDC__
    ( rpc_handle, server_name, st_p )
    rpc_binding_handle_t    rpc_handle;
    unsigned_char_t         *server_name;
    error_status_t          *st_p;
#else
    (
    rpc_binding_handle_t    rpc_handle,
    unsigned_char_t         *server_name,
    error_status_t          *st_p
    )
#endif
{
    CLEAR_STATUS(st_p);

    if (host_ctx_info_p->context == 0) {
        SET_STATUS(st_p, sec_rgy_host_context_not_avail);
        return;
    }

    rpc_binding_set_auth_info(rpc_handle, server_name,
                              rpc_c_protect_level_pkt_integ,
                              rpc_c_authn_dce_secret,
                              host_ctx_info_p->context,
                              rpc_c_authz_dce,
                              st_p);
}




/* rs_login_get_host_login_context
 *
 * Obtain the host machine login context inherited by a previous call to
 * rs_login_inherit_host_identity. 
 */
PUBLIC void rs_login_get_host_login_context
#ifndef __STDC__
    ( host_login_context, st_p )
    sec_login_handle_t  *host_login_context;
    error_status_t      *st_p;
#else
    (
    sec_login_handle_t  *host_login_context,
    error_status_t      *st_p
    )
#endif
{
    CLEAR_STATUS(st_p);

    if (host_ctx_info_p->context == 0) {
        SET_STATUS(st_p, sec_rgy_host_context_not_avail);
    }

    *host_login_context = host_ctx_info_p->context;
}



/* rs_login_become_rgy
 *
 * set the default process identity to the architecturally-defined DCE registry 
 * service principal.  If succesful, a background thread is started to refresh 
 * the dce-rgy identity as appropriate. 
 *
 * This routine performs a database lookup to obtain the registry service
 * principal's secret key.  We rely on the caller to provide the appropriate 
 * concurrency control.
 */
PUBLIC void rs_login_become_rgy
#ifndef __STDC__
    ( st_p )
    error_status_t  *st_p;
#else
    (
    error_status_t  *st_p
    )
#endif
{
    error_status_t      lst;
    rs_context_info_t   ctx_info;
    rs_context_info_p_t ctx_info_p = &ctx_info;
    rs_task_handle_t    task_handle;
    char                buf[64];

    /* use a temp. variable in case things go bad.
     * rgy_ctx_info_p->context should be 0 at this point,
     * if it isn't rs_login_pvt_setup_context won't do anything.
     */
    ctx_info_p->identity = (unsigned_char_t *)SEC_RGY_SERVER_NAME;
    ctx_info_p->context = rgy_ctx_info_p->context;
    rs_login_pvt_setup_context(ctx_info_p, st_p);

    if (GOOD_STATUS(st_p)) {
        /* 
         * set the process identity using the context created
         *  above.
         */
        sec_login_set_context(ctx_info_p->context, st_p);
        /*
         * cleanup context if something went wrong after setup.
         * Use a local status so failures in the purge don't
         * obscure the original cause of failure
         */
        if (BAD_STATUS(st_p)) {
            sec_login_purge_context(&(ctx_info_p->context), &lst);
            ctx_info_p->context = 0;
        }
    }

    if (GOOD_STATUS(st_p)) {
        rgy_ctx_info_p->identity = ctx_info_p->identity;
        rgy_ctx_info_p->context = ctx_info_p->context;

        if (!rgy_ctx_refresh_task_started) {
            sprintf(buf, 
                "SECD - %s context refresh", rgy_ctx_info_p->identity);
            if (! rs_task_create(&task_handle, rs_login_pvt_refresh_context,
                 buf, false, (void *)rgy_ctx_info_p) ) {
                 dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                                sec_s_rs_login,
                                svc_c_sev_fatal | svc_c_action_abort,
                                sec_rs_thr_exit_creat_fail);
            }
            rgy_ctx_refresh_task_started = true;
        }
    }
}
                      

/* rs_login_shutdown
 *
 * cleans up any persistent data associated with the identities managed by
 * the rs_login_ module.  Should be called by server shutdown routines.
 */
PUBLIC void rs_login_shutdown( 
#ifndef __STDC__
    void
#endif
)
{
   error_status_t  st;

   if (rgy_ctx_info_p->context != 0) {
       sec_login_purge_context(&(rgy_ctx_info_p->context), &st);
   }
}
