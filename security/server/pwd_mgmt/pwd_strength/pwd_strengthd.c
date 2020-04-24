/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pwd_strengthd.c,v $
 * Revision 1.1.14.1  1996/06/04  22:04:00  arvind
 * 	Fix for OT 13270: Merge this fix from mullan_chfts15858. Other file is security/server/rs/rs_pwd_mgmt.c.
 * 	[1996/01/19  18:10 UTC  psn  /main/final_1.2.1_merge/1]
 *
 * 	CHFts15858: additional changes based on code review.
 * 	[1995/10/24  15:44 UTC  mullan_s  /main/mullan_chfts15858/2]
 *
 * 	CHFts15858: Allow clients to make remote calls with PAC-based
 * 	(DCE) authorization.
 * 	[1995/10/23  19:13 UTC  mullan_s  /main/mullan_chfts15858/1]
 *
 * Revision 1.1.12.3  1996/02/18  00:16:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:42  marty]
 * 
 * Revision 1.1.12.2  1995/12/08  17:47:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:26  root]
 * 
 * Revision 1.1.8.1  1994/10/25  20:04:31  aha
 * 	CR12184: export binding information to /.:/subsys/dce/pwd_mgmt/pwd_strength.
 * 	[1994/10/25  20:02:28  aha]
 * 
 * Revision 1.1.2.8  1994/10/25  17:17:35  aha
 * 	CR12184: export binding information to /.:/subsys/dce/pwd_mgmt/pwd_strength.
 * 	[1994/10/25  17:13:10  aha]
 * 
 * Revision 1.1.2.7  1994/09/09  20:41:05  kline_s
 * 	OT 12028: I18N code cleanup for Security.
 * 	[1994/09/09  20:30:37  kline_s]
 * 
 * Revision 1.1.2.6  1994/08/31  16:06:49  mullan_s
 * 	[OT11775] Initialize contents of gen_info_out, str_info_out.
 * 	[1994/08/31  16:06:19  mullan_s]
 * 
 * Revision 1.1.2.5  1994/08/19  17:59:22  mdf
 * 	Bug fixes from Hewlett Packard
 * 	[1994/08/19  13:10:48  mdf]
 * 
 * Revision 1.1.3.5  94/08/18  17:13:05  mullan_s
 * 	[OT 11634] On strength check, do not check password cache if pwd_val_type=2.
 * 
 * Revision 1.1.3.4  1994/08/18  19:36:53  mullan_s
 * 	Make log file read/write only by root.
 * 
 * Revision 1.1.3.3  1994/08/18  16:53:44  mullan_s
 * 	Remove references to pwd_checkd, support options in pwd_strengthd.
 * 
 * Revision 1.1.3.2  1994/08/15  17:38:11  mullan_s
 * 	[OT11657] Fix pwd_strengthd crash when running in background.
 * 	Also allow pwd_strengthd to be started remotely via remsh by
 * 	redirecting stdin, stdout, & stderr to /dev/null.
 * 
 * Revision 1.1.2.3  1994/07/15  15:01:00  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:18:34  mdf]
 * 
 * Revision 1.1.2.2  1994/06/17  18:42:42  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:14:19  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/11  19:26:31  ahop
 * 	move PWD_STRENGTHD_LOG to sec_login_file_loc.h
 * 	hp_sec_to_osf_2 drop
 * 	Password strength server
 * 	[1994/05/06  20:52:42  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:26:52  ahop]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1994, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * Password Strength server
 */

#include <errno.h>
#include <fcntl.h>
#include <locale.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include <dce/dce.h>
#include <dce/dce_cf.h>
#include <dce/dce_error.h>
#include <dce/id_base.h>
#include <dce/keymgmt.h>
#include <dce/rgynbase.h>
#include <dce/rpc.h>
#include <dce/rsec_pwd_mgmt.h>
#include <dce/sec_login.h>

#include <gen.h>
#include <pwd_cache.h>

#define NOT_USED -1

/*
 * global variables
 */
sec_login_handle_t      login_context;
int                     ten_minutes = 10*60;
boolean32  debug_mode   = false;
boolean32  verbose      = false;
FILE       *pd_fp      = NULL;
signed32   ovrd_min_len;
boolean32  ovrd_len     = false;
boolean32  ovrd_all_spaces = NOT_USED;
boolean32  ovrd_alpha_num  = NOT_USED;
char       *server_name;
signed32   cache_size      = 100; /* default cache size for password cache */
signed32   timeout         = 1800; /* maximum number of seconds a generated password
                                      should be cached before flushing from memory */
pthread_mutex_t cache_mutex;  /* Mutex lock used when writing to cache */

enum { none, user_select, user_can_select, generation_required };


#define SERVER_PNAME "pwd_strength"
#define PWD_STRENGTHD_LOG    DCELOCAL_PATH "/var/security/pwd_strengthd.log"

/* maximum number of passwords server will generate per request */
#define MAX_PWDS     10


/*
 * error messaging routine
 */
void error (
    error_status_t  *st,
    char            *msg
)
{
    dce_error_string_t  err_string;
    int                 lst;
    struct timeval      now;
    struct timezone     tz;
    char                tbuf[128];
    struct tm           time_info;
    FILE                *efp = NULL;

#ifdef SNI_SVR4_POSIX
    gettimeofday(&now);
#else
    gettimeofday(&now, &tz);
#endif /* SNI_SVR4_POSIX */

    if (!debug_mode)
        efp = pd_fp;
    else
        efp = stderr;

    time_info = *localtime((time_t *) &now.tv_sec);
    strftime(tbuf, sizeof(tbuf), "%x %X", &time_info);

    if (*st != error_status_ok) {
        dce_error_inq_text(*st, err_string, &lst);
        fprintf(efp, "%s %s - %s\n... %s\n", server_name, tbuf, msg,
                    err_string);
    } else {
        fprintf(efp, "%s %s - %s\n", server_name, tbuf, msg);
    }
}

/*
 * Generic messaging routine.
 */
static void message (
    char *msg
)
{
    struct timeval      now;
    struct timezone     tz;
    char                tbuf[128];
    struct tm           time_info;
    FILE                *mfp = NULL;

    if (!debug_mode)
        mfp = pd_fp;
    else
        mfp = stdout;

#ifdef SNI_SVR4_POSIX
    gettimeofday(&now);
#else
    gettimeofday(&now, &tz);
#endif /* SNI_SVR4_POSIX */

    time_info = *localtime((time_t *) &now.tv_sec);
    strftime(tbuf, sizeof(tbuf), "%x %X", &time_info);

    fprintf(mfp, "%s %s - %s\n", server_name, tbuf, msg);
}

/* matches command line arguments */
static boolean32 match_arg (
    char            *key,
    char            *arg,
    int             min
)
{
    int i = 0;

    if (*key) while (*key == *arg) {
        i++;
        key++;
        arg++;
        if (*arg == '\0' || *key == '\0')
            break;
    }
    if (*arg == '\0' && i >= min)
        return true;
    return false;
}

/*
 * Parses command line arguments.
 */
static boolean32 process_args (
    int             argc,
    char            *argv[]
)
{
    int             i;
    boolean32       purge = false;

    /* determine program name */
    server_name = strrchr(argv[0], '/');
    if (server_name == NULL) {
        server_name = argv[0];
    } else {
        server_name++;
    }

    for (i = 1; i < argc; i++) {
        if (match_arg("-verbose", argv[i], 2)) {
            verbose = true;
        } else if (match_arg("-debug", argv[i], 2)) {
            debug_mode = true;
        } else if (match_arg("-min_len", argv[i], 2)) {
            /* get value! */
            if (++i == argc) {
                fprintf(stderr, "-m[in_len] option requires an argument\n");
                goto usage;
            }
            ovrd_min_len = atol(argv[i]);
            ovrd_len = true;
        } else if (match_arg("-all_spaces", argv[i], 4)) {
            ovrd_all_spaces = false;
        } else if (match_arg("+all_spaces", argv[i], 4)) {
            ovrd_all_spaces = true;
        } else if (match_arg("-alpha_num", argv[i], 4)) {
            ovrd_alpha_num = false;
        } else if (match_arg("+alpha_num", argv[i], 4)) {
            ovrd_alpha_num = true;
        } else if (match_arg("-cache_size", argv[i], 2)) {
            /* get value */
            if (++i == argc) {
                fprintf(stderr, "-c[ache_size] option requires an argument\n");
                goto usage;
            }
            cache_size = atol(argv[i]);
        } else if (match_arg("-timeout", argv[i], 2)) {
            /* get value */
            if (++i == argc) {
                fprintf(stderr, "-t[imeout] option requires an argument\n");
                goto usage;
            }
            timeout = atol(argv[i]) * 60;
        } else {
            fprintf(stderr, "Unknown command option %s\n", argv[i]);
            goto usage;
        }
    }

    return(true);

usage:
    fprintf(stderr, "Usage: %s [-m[in_len]] password_min_length [+/-all[_spaces]] ", argv[0]);
    fprintf(stderr, "[+/-alp[ha_num]] [-c[ache_size]] size [-d[ebug]] [-t[imeout]] minutes [-v[erbose]]\n");
    return(false);
}


/* concatenates 3 substrings */
static void string_concat (
    char                    **buf,
    char                    *prefix,
    char                    *str,
    char                    *suffix
)
{
    int len1 = 0;
    int len2 = 0;
    int len3 = 0;
    int len;
    char *p;
    char *q;

    if (prefix) {
        len1 = strlen(prefix);
    }

    if (str) {
        len2 = strlen(str);
    }

    if (suffix) {
        len3 = strlen(suffix);
    }

    len = len1 + len2 + len3;

    p = malloc(len + 1);
    if (p) {
        q = p;
        strcpy(q, prefix);
        q += len1;
        strcpy(q, str);
        q += len2;
        strcpy(q, suffix);
    }

    *buf = p;
}


/*
 * check_auth_client ensures that the client is properly
 * authenticated and authorized. 
 */
static void check_auth_client (
    handle_t                 handle,
    unsigned32               *protect_level,
    char                     *exp_client_princ,
    error_status_t           *stp
)
{
    rpc_authz_handle_t       privs;
    unsigned_char_p_t        svr_princ_name = NULL,
                             client_princ_full_name = NULL,
                             pwd_strength_princ_full_name = NULL,
                             cell_name = NULL;
    unsigned32               authn_svc,
                             authz_svc;
    error_status_t           lst;
    sec_rgy_name_t           client_princ;
    int                      len, clen;
    sec_id_pac_t             *pac;

    /* get clients authorization and authentication information */
    rpc_binding_inq_auth_client(handle, &privs, &svr_princ_name,
        protect_level, &authn_svc, &authz_svc, stp);
    if (*stp != error_status_ok) {
        error(stp, "rpc_binding_inq_auth_client failed");
        if (*stp == rpc_s_binding_has_no_auth)
            *stp = sec_pwd_mgmt_not_authorized;
        else
            *stp = sec_pwd_mgmt_svr_error;
        return;
    }

    /* XXX - How do we handle default protection levels,
       authn_svc, etc .. */

    /* check required protection level */
    if (*protect_level != rpc_c_protect_level_pkt_privacy &&
	  *protect_level != rpc_c_protect_level_pkt_integ) {
        *stp = sec_pwd_mgmt_not_authorized;
	goto check_auth_client_cleanup;
    }

    /* check expected authentication service - only
     * accept DCE shared-secret key authentication for now.
     */
    if (authn_svc != rpc_c_authn_dce_secret) {
        *stp = sec_pwd_mgmt_not_authorized;
	goto check_auth_client_cleanup;
    }

    /*
     * check expected authorization service 
     */
    if (authz_svc != rpc_c_authz_dce && 
	  authz_svc != rpc_c_authz_name) {
        *stp = sec_pwd_mgmt_not_authorized;
	goto check_auth_client_cleanup;
    }

    /* check expected server principal name */
    dce_cf_get_cell_name((char **) &cell_name, stp);
    if (*stp != error_status_ok) {
        error(stp, "dce_cf_get_cell_name failed");
        *stp = sec_pwd_mgmt_svr_error;
        goto check_auth_client_cleanup;
    }
    string_concat((char **)&pwd_strength_princ_full_name,
                  (char *)cell_name, "/", "pwd_strength");
    if (strcmp((char *)svr_princ_name, 
               (char *)pwd_strength_princ_full_name) != 0) {
        *stp = sec_pwd_mgmt_not_authorized;
        goto check_auth_client_cleanup;
    }

    /* check expected client principal */
    if ((*stp == error_status_ok) && exp_client_princ != NULL) {
	if (authz_svc == rpc_c_authz_name)
            strcpy((char *)client_princ, (char *)privs);
	else if (authz_svc == rpc_c_authz_dce) {
	    pac = (sec_id_pac_t *)privs;
	    sec_id_gen_name(NULL, pac->realm, pac->principal,
		&client_princ, NULL, NULL, stp);
	    if (*stp != error_status_ok) {
		error(stp, "Unable to generate name from uuid");
		goto check_auth_client_cleanup;
	    }
        }
        string_concat((char **)&client_princ_full_name,
                      (char *)cell_name, "/", exp_client_princ);
        len = strlen((char *)client_princ_full_name);
        clen = strlen((char *)client_princ);
        if (! (clen == len && strcmp((char *)client_princ,
              (char *)client_princ_full_name) == 0)) {
            *stp = sec_pwd_mgmt_not_authorized;
        }
    }

check_auth_client_cleanup:
    if (pwd_strength_princ_full_name)
        free((char *)pwd_strength_princ_full_name);
    if (svr_princ_name) {
        rpc_string_free(&svr_princ_name, &lst);
        if (lst != error_status_ok)
          error(&lst, "Unable to free rpc string");
    }
    if (client_princ_full_name)
        free((char *)client_princ_full_name);
    if (cell_name)
        free((char *)cell_name);
}


/* 
 * check_plain_pwd checks a plaintext password
 * against the minimum length of a password and
 * the password text flags.
 */
static boolean32 check_plain_pwd (
    char                     *pwd_text,
    signed32                 pwd_textlen,
    signed32                 pwd_min_len,
    sec_rgy_plcy_pwd_flags_t pwd_flags
)
{
    boolean32    all_spaces = true,
                 alpha_num  = true;
    int          i;

    /* check if server is overriding default registry policy */
    if (ovrd_len)
        pwd_min_len = ovrd_min_len;

    if (pwd_textlen < pwd_min_len)
        return false;

    /* step through each character in the passwd_text looking for a non-space
     * or a non-alpha-numeric character and set the value of all_spaces
     * and alpha_num based on what we find
     */
    for (i = 0; i < pwd_textlen && (all_spaces || alpha_num);
         i++, pwd_text++ ) {
        if (!isdcepcs(*pwd_text)) {
            /* not ascii ==> not blank or alpha-num */
            alpha_num = false;
            all_spaces = false;
        } else {
            if (!isalnum(*pwd_text)) {
                /* not alpha-num but might be blank */
                alpha_num = false;
                if (!(*pwd_text == ' ')) /* nope, not a blank either */
                    all_spaces = false;
            } else                    /* is alpha-num ==> not a blank */
                all_spaces = false;
        }
    }

    if (ovrd_alpha_num != NOT_USED) {
        if (alpha_num && !ovrd_alpha_num)
            return false;
    } else {
        if (((pwd_flags & sec_rgy_plcy_pwd_non_alpha) != 0)
            && (alpha_num))
            return false;
    }

    if (ovrd_all_spaces != NOT_USED) {
        if (all_spaces && !ovrd_all_spaces)
            return false;
    } else {
        if (((pwd_flags & sec_rgy_plcy_pwd_no_spaces) != 0)
            && (all_spaces))
            return false;
    }

    return true;
}


void rsec_pwd_mgmt_gen_pwd (
    handle_t                 pwd_mgmt_svr_h,           /* [in]  */
    sec_rgy_name_t           princ_name,               /* [in]  */
    unsigned32               plcy_args,                /* [in]  */
    sec_attr_t               plcy[],                   /* [in]  */
    sec_bytes_t              gen_info_in,              /* [in]  */
    unsigned32               num_pwds,                 /* [in]  */
    unsigned32               *num_returned,            /* [out] */
    sec_passwd_rec_t         gen_pwd_set[],            /* [out] */
    sec_bytes_t              *gen_info_out,            /* [out] */
    error_status_t           *stp                      /* [out] */
)
{
    char buf[sec_rgy_name_t_size + 100];
    unsigned32               protect_level;
    int                      x;

    *stp = error_status_ok;
    *num_returned = 0;
    gen_info_out->num_bytes = 0;
    gen_info_out->bytes = NULL;

    if (verbose) {
        sprintf(buf, "rsec_pwd_mgmt_gen_pwd request: princ- %s", princ_name);
        message(buf);
    }

    /* 
     * Check authorization and authentication of client
     * NOTE : We do not care who is making the call to the
     * generator, thus the NULL argument for the principal.
     * XXX - However, we may want to limit the number of
     * times a principal can call the generator successively.
     * (except if cell_admin).
     */
    check_auth_client(pwd_mgmt_svr_h, &protect_level, NULL, stp);
    if (*stp != error_status_ok) {
        return;
    }

    /*
     * generate password(s)
     */
    for (x = 0; x < num_pwds && x < MAX_PWDS; x++) {
        gen_pwd_set[x].version_number = 0;
        gen_pwd_set[x].pepper = NULL;
        gen_pwd_set[x].key.key_type = sec_passwd_plain;
        generate_pwd(plcy_args, plcy, 
                     &gen_pwd_set[x].key.tagged_union.plain, stp);
        if (*stp != error_status_ok) {
            error(stp, "Unable to generate password");
            *stp = sec_pwd_mgmt_svr_error;
            break;
        }
        (*num_returned)++;
    }
    
    /*
     * cache generated passwords
     */
    if (*stp == error_status_ok) {
        create_record((char *) princ_name, *num_returned, 
                      gen_pwd_set, stp);
        if (*stp != error_status_ok) {
            error(stp, "Unable to create record");
            *stp = sec_pwd_mgmt_svr_error;
        }
    }

    if (verbose && (*stp == error_status_ok)) {
        sprintf(buf, "rsec_pwd_mgmt_gen_pwd: princ- %s", princ_name);
        message(buf);
    }

    /* 
     * XXX - 
     * if RPC protection level privacy is not available,
     * we should not send the password back in the clear.
     * Therefore we must send the password to a trusted proxy and
     * inform the client to get the password from the proxy.
     * However support for proxies has not been implemented yet,
     * so we will send the password back unencrypted for the
     * time being. A pickle type should be defined which holds
     * the information needed by the client to retrieve the 
     * password from the proxy. This pickle should be sent in
     * the gen_info_out parameter.
     *
     * if (protect_level == rpc_c_protect_level_pkt_integ) {
     *      *
     *      * Send password to proxy.
     *      * Encode necessary information into gen_info_out pickle.
     *      * 
     *  
     *      * destroy password * 
     *     if (gen_pwd->key.tagged_union.plain)
     *         rpc_ss_free(gen_pwd->key.tagged_union.plain);
     *     *stp = sec_pwd_mgmt_proxy_req;
     * }
     */
}


/*
 * rsec_pwd_mgmt_str_chk
 */
boolean32 rsec_pwd_mgmt_str_chk (
    handle_t                 handle,                   /* [in] */
    sec_rgy_name_t           princ,                    /* [in] */
    sec_passwd_rec_t         *pwd,                     /* [in] */
    signed32                 pwd_val_type,             /* [in] */
    unsigned32               plcy_args,                /* [in] */
    sec_attr_t               plcy[],                   /* [in] */
    sec_bytes_t              str_info_in,              /* [in] */
    sec_bytes_t              *str_info_out,            /* [out] */
    error_status_t           *stp                      /* [out] */
)
{
    long                     passwd_len;
    char                     buf[sec_rgy_name_t_size + 100];
    unsigned32               protect_level;
    boolean32                record_exists;

    *stp = error_status_ok;
    str_info_out->num_bytes = 0;
    str_info_out->bytes = NULL;

    if (verbose) {
        sprintf(buf, "rsec_pwd_mgmt_str_chk request: princ- %s", princ);
        message(buf);
    }

    /* Check authorization and authentication of client */
    check_auth_client(handle, &protect_level, SEC_RGY_SERVER_NAME, stp);
    if (*stp != error_status_ok) {
        return false;
    }

    /*
     * XXX - If RPC transmission used anything other than packet level
     * privacy, the password to be strength checked should not have
     * been sent over the wire unencrypted. A proxy mechanism should
     * have been used (see rsec_pwd_mgmt.idl for more info). However,
     * since this functionality is not available, the password *is*
     * sent over the wire unencrypted.
     */

    /*
     * If password is DES, just return. 
     * XXX - check DES passwords to make sure they are the
     * same ones generated. Check against weak DES keys.
     * Also, check des key bit on account to see if user is
     * allowed to specify a DES password.
     */
    if (pwd->key.key_type == sec_passwd_des)
        return true;

    /*
     * If password was generated, look in cache to make sure
     * that user sent the password which was generated!
     */
    if (pwd_val_type == generation_required) {
        return check_cache((char *) princ, pwd, &record_exists, stp);
    }

    /*
     * If pwd_val_type is user_can_select, we do not care if the
     * user selected their own password or asked for a generated
     * password. Also, we do not *know* whether the user selected
     * their own password or not, so if we did check the generated
     * password cache and found a password that did not match, we
     * cannot be assured that the user simply generated a password
     * but then decided to pick their own anyway. In this scenario,
     * the user should not fail the strength check. So, to solve
     * this, we never check the generated password cache at all
     * because we really don't care if the user didn't use the generated
     * password. The generated passwords will get flushed after a while.
     */

    /* check password against effective policy */
    passwd_len = strlen((char *)pwd->key.tagged_union.plain);
    return check_plain_pwd((char *) pwd->key.tagged_union.plain, passwd_len,
        plcy[0].attr_value.tagged_union.signed_int,
        plcy[1].attr_value.tagged_union.signed_int);
}

/*
 * sigcatch() -- Catch and handle asynchronous signals for the server.  This
 * function waits a separate thread using the sigwait system call.  When an
 * asynchronous signal the thread is waiting for comes in the thread is
 * scheduled again and tells the server to stop listening, causing a return
 * from the rpc_server_listen() above.  This thread then exits.
 */
static pthread_addr_t sigcatch(pthread_addr_t arg)
{
    error_status_t      st, _ignore;            /* returned by DCE calls */
    dce_error_string_t  dce_err_string;         /* text describing error code */
    sigset_t            mask;                   /* signal values to wait for */
    int                 signo;

    /*
     * Create the signal mask -- only the following signals will be caught.
     * See signal(5).
     */
    sigemptyset(&mask);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
#ifdef  _INCLUDE_POSIX_SOURCE
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
#endif  /* _INCLUDE_POSIX_SOURCE */

    /*
     * Calling sigwait will cause this thread to block until the process
     * receives one of the signals in the mask.  If no threads were waiting
     * for these fatal asynchronous signals and such a signal were received,
     * the process would die immediately without giving the server a chance to
     * unregister its bindings with the endpoint mapper.  sigwait is the only
     * way to catch a fatal asynchronous signal and have the opportunity to
     * cleanup before exiting.
     */
    signo = sigwait(&mask);

    /* XXX  - Need some way to tell user what signal was caught */

    /*  rpc_mgmt_stop_server_listening() --
     *
     * Stop the server from listening for more requests, but let current
     * requests run to completion.  The first parameter is the binding
     * handle associated with the server we wish to make stop listening.  A
     * value of NULL means to stop this (local) server from listening.  The
     * second parameter is the DCE error status.
     *
     * Note that if there are pending RPC's that don't complete in a timely
     * manner, another asynchronous signal will kill the server process: we
     * will no longer have a thread to catch these signals!
     */
    rpc_mgmt_stop_server_listening(NULL, &st);
    if (st != error_status_ok) {
        error(&st, "Unable to stop server listening");
    }
    pthread_exit((pthread_addr_t) NULL);
}

/*
 * terminate_thread() -- Terminate the current thread.  This is called in
 * place of exit in any routines that could be called by manager functions
 * so the server process can clean up and exit without leaving any orphaned
 * clients.  If the server simply exits any other manager threads would
 * disappear, causing clients to timeout with partially executed requests.
 */
static void terminate_thread(void)
{
    error_status_t      st, _ignore;            /* returned by DCE calls */

    /*
     * Stop the server from listening for more requests, but let current
     * requests run to completion.
     */
    rpc_mgmt_stop_server_listening(NULL, &st);
    if (st != rpc_s_ok) {
        error(&st, "Unable to stop server listening");
    }
    pthread_exit((pthread_addr_t)NULL);
}


/*
 * manage_key manages the server principal's key, by
 * changing it every so often.
 */
static pthread_addr_t manage_key(pthread_addr_t arg)
{
    error_status_t  st;
    struct timespec timer;

    timer.tv_nsec = 0;

    while (1) {
 
        sec_key_mgmt_manage_key(rpc_c_authn_dce_secret, 
            (void *) "FILE:/krb5/pwd_strength_tab", (idl_char *) SERVER_PNAME, &st);
        if (st != error_status_ok) {
            error(&st, "Server principal key management failure");
            terminate_thread();
        }

        timer.tv_sec = ten_minutes;
        pthread_delay_np(&timer);
    }
}


/*
 * cleanup_cache removes any stale entries from the password cache
 * by checking it every so often (determined by cache timeout
 * value).
 */
static pthread_addr_t cleanup_cache(pthread_addr_t arg)
{
    struct timespec ts;
    error_status_t  lst;

    /* set timer to value of cache timeout */
    ts.tv_sec = timeout;
    ts.tv_nsec = 0;
    while (1) {

        pthread_delay_np(&ts);

        if (verbose) {
            message("Cleaning up password cache");
        }

        cleanup_table(&lst);
        if (lst != error_status_ok) {
            error(&lst, "Unable to cleanup password cache");
            terminate_thread();
        }
    }
}


/*
 * refresh_context manages the server's identity, by refreshing
 * and revalidating it before it expires.
 */
static pthread_addr_t refresh_context(pthread_addr_t arg)
{
    signed32        expiration;
    error_status_t  st, lst;
    time_t          now;
    int             seconds;
    struct timespec ts;
    boolean32       reset_passwd;
    sec_login_auth_src_t auth_src;
    sec_passwd_rec_t *keydata;

    ts.tv_sec = ten_minutes;
    ts.tv_nsec = 0;

    while (1) {

        /* get the absolute expiration time expressed in seconds */
        sec_login_get_expiration(login_context, &expiration, &st);
        if (st != error_status_ok) {
            error(&st, "Unable to get expiration of server identity");
            terminate_thread();
        }

        time(&now);
        seconds = expiration - now;
        if (seconds > ten_minutes) {
            ts.tv_sec = seconds - ten_minutes;
            pthread_delay_np(&ts);
        }

        if (verbose) {
            message("Refreshing server context");
        }

        sec_login_refresh_identity(login_context, &st);
        if (st != error_status_ok) {
            error(&st, "Unable to refresh server identity");
            terminate_thread();
        }

        /* get key */
        sec_key_mgmt_get_key(rpc_c_authn_dce_secret, "FILE:/krb5/pwd_strength_tab",
            (idl_char *) SERVER_PNAME, 0, (void **) &keydata, &st);
        if (st != error_status_ok) {
            error(&st, "Unable to get server key");
            terminate_thread();
        }

        /* validate login context */
        sec_login_validate_identity(login_context, keydata, &reset_passwd,
            &auth_src, &st);
        /* XXX - There may be a chance that the key changed since the get_key */
        if (st != error_status_ok) {
            sec_key_mgmt_free_key(keydata, &lst);
            error(&st, "Unable to validate identity");
            terminate_thread();
        }

        sec_key_mgmt_free_key(keydata, &lst);

        /* certify login context */
        sec_login_certify_identity(login_context, &st);
        if (st != error_status_ok) {
            error(&st, "Unable to certify identity");
            terminate_thread();
        }
    }
}


static void become_server(void)
{
    int child;
    int fd;

    if (!debug_mode) {

        child = fork();   

        /* parent */
        if (child != 0) {
            exit(0);
        } else {
            /* Disassociate from controlling terminal */
#if defined(_POSIX_SOURCE)
            if (setsid() == -1) {
                fprintf(stderr, "Cannot disassociate from controlling terminal\n");
                perror("setsid");
                exit(1);
            }
#else
            if (setpgrp(0, getpid()) == -1) {
                fprintf(stderr, "Cannot disassociate from controlling terminal\n");
                perror("setpgrp");
                exit(1);
            }
#endif

            /* redirect stdin, stdout, & stderr so pwd_strengthd can be started remotely */
            if ((fd = open("/dev/null", O_RDWR,
                       S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)) != -1)
            {
                (void)dup2(fd,0);
                (void)dup2(fd,1);
                (void)dup2(fd,2);
                (void)close(fd);
            }
        }
    }
}

/*
 * Main routine
 */
int main (
    int argc,
    char *argv[]
)
{
    error_status_t          status, lst;
    dce_error_string_t      err_string;
    rpc_binding_vector_p_t  bindings;
    unsigned_char_p_t       annotation = (unsigned_char_p_t)
                                "pwd_strength - rsec_pwd_mgmt interface";
    boolean32               reset_passwd;
    sec_login_auth_src_t    auth_src;
    char                    *cds_path = "/.:/subsys/dce/pwd_mgmt/pwd_strength";
    pthread_t               sig_thread,
                            refresh_thread,
                            manage_key_thread,
                            cleanup_cache_thread;
    sec_passwd_rec_t        *keydata;
    int                     pd_fd;

    setlocale(LC_ALL, "");
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    /* Must be run by privileged user */
    if (setuid(0) == -1) {
        fprintf(stderr, "Must be the privileged user\n");
        exit(1);
    }

    /* Process command line arguments */
    if (!process_args(argc, argv)) {
        exit(1);
    }

    /* Open log file */
    if (!debug_mode) {
        pd_fd = open((char *) PWD_STRENGTHD_LOG, (O_WRONLY | O_APPEND | 
                     O_CREAT), (mode_t) (S_IRUSR | S_IWUSR));
        if (pd_fd == -1) {
            fprintf(stderr, "Cannot open log file\n");
            perror("open");
            exit(1);
        }
        pd_fp = fdopen(pd_fd, "a");
        if (pd_fp == (FILE *) NULL) {
            fprintf(stderr, "Cannot open log file\n");
            perror("fdopen");
            exit(1);
        }
        setbuf(pd_fp, NULL);
    }

    /* become server, disconnect from tty, etc ... */
    (void) become_server();

    /* use all protocol sequences */
    rpc_server_use_all_protseqs(rpc_c_protseq_max_reqs_default, &status);
    if (status != error_status_ok) {
        error(&status, "Unable to use all protseqs");
        exit(1);
    }

    /* obtain vector of server binding handles */
    rpc_server_inq_bindings(&bindings, &status);
    if (status != error_status_ok) {
        error(&status, "Unable to obtain bindings");
        exit(1);
    }

    if (verbose) {
        message("Registering rsec_pwd_mgmt interface");
    }

    /* register rsec_pwd_mgmt interface with RPC runtime */
    rpc_server_register_if(rsec_pwd_mgmt_v1_0_s_ifspec, (uuid_t *) NULL,
        (rpc_mgr_epv_t) NULL, &status);
    if (status != error_status_ok) {
        error(&status, "Unable to register rsec_pwd_mgmt interface");
        exit(1);
    }

    /* export binding information to name service */
    rpc_ns_binding_export(rpc_c_ns_syntax_default,
                          (unsigned_char_p_t) cds_path,
                          rsec_pwd_mgmt_v1_0_s_ifspec,
                          bindings,
                          NULL, &status);
    if (status != error_status_ok) {
        error(&status, "Unable to export binding information");
        exit(1);
    }

    /* register endpoints in local host's endpoint map */
    rpc_ep_register(rsec_pwd_mgmt_v1_0_s_ifspec, bindings,
        (uuid_vector_t *) NULL, annotation, &status);
    if (status != error_status_ok) {
        error(&status, "Unable to register endpoints");
        exit(1);
    }

    /* 
     * Register authentication info
     * 
     * NOTE : the client encrypts any password arguments before
     * sending them to this server. (see rs_pwd_mgmt.c)
     * So there is no need for a get_key function.
     */
    rpc_server_register_auth_info((unsigned_char_p_t) SERVER_PNAME,
        rpc_c_authn_dce_secret, (rpc_auth_key_retrieval_fn_t) NULL, 
        (void *) "FILE:/krb5/pwd_strength_tab", &status);
    if (status != error_status_ok) {
        error(&status, "Unable to register authentication info");
        exit(1);
    }

    if (verbose) {
        message("Setting up server context");
    }

    /* setup login context */
    sec_login_setup_identity((unsigned_char_p_t) SERVER_PNAME, sec_login_no_flags, 
        &login_context, &status);
    if (status != error_status_ok) {
        error(&status, "Unable to setup identity");
        exit(1); 
    } 

    /* get key  -- key must have been created via rgy_edit ktadd */ 
    sec_key_mgmt_get_key(rpc_c_authn_dce_secret, "FILE:/krb5/pwd_strength_tab",
        (idl_char *) SERVER_PNAME, 0, (void **) &keydata, &status); 
    if (status != error_status_ok) { 
        error(&status, "Unable to get server key"); 
        sec_login_purge_context(&login_context, &lst); 
        exit(1);
    }

    /* validate login context */
    sec_login_validate_identity(login_context, keydata, &reset_passwd,
        &auth_src, &status);
    if (status != error_status_ok) {
        error(&status, "Unable to validate identity");
        sec_login_purge_context(&login_context, &lst);
        exit(1);
    }

    sec_key_mgmt_free_key(keydata, &lst);

    /* certify login context */
    sec_login_certify_identity(login_context, &status);
    if (status != error_status_ok) {
        error(&status, "Unable to certify identity");
        sec_login_purge_context(&login_context, &lst);
    }
       
    /* set context */
    sec_login_set_context(login_context, &status);
    if (status != error_status_ok) {
        error(&status, "Unable to set server context");
        sec_login_purge_context(&login_context, &lst);
        exit(1);
    }

    /* create mutex for writing to cache */
    if (pthread_mutex_init(&cache_mutex, pthread_mutexattr_default) < 0) {
        error(NULL, "Unable to create mutex");
        sec_login_purge_context(&login_context, &lst);
        exit(1);
    }

    /* create cache table */
    create_table(&status);
    if (status != error_status_ok) {
        error(&status, "Unable to initialize password cache");
        sec_login_purge_context(&login_context, &lst);
        exit(1);
    }

    /* start a thread to manage key */
    if (pthread_create(&manage_key_thread, pthread_attr_default,
                       manage_key, 0) < 0) {
        error(NULL, "Unable to start the manage key thread");
        sec_login_purge_context(&login_context, &lst);
        exit(1);
    }

    /* start a thread to refresh identity */
    if (pthread_create(&refresh_thread, pthread_attr_default,
                       refresh_context, 0) < 0) {
        error(NULL, "Unable to start the refresh context thread");
        sec_login_purge_context(&login_context, &lst);
        exit(1);
    }

    /* start a thread to cleanup cache */
    if (pthread_create(&cleanup_cache_thread, pthread_attr_default,
                       cleanup_cache, 0) < 0) {
        error(NULL, "Unable to start the cleanup cache thread");
        sec_login_purge_context(&login_context, &lst);
        exit(1);
    }

    /* start a thread to catch asynchronous signals */
    if (pthread_create(&sig_thread, pthread_attr_default,
                       sigcatch, 0) < 0) {
        error(NULL, "Unable to start the signal catching thread");
        sec_login_purge_context(&login_context, &lst);
        exit(1);
    }

    TRY {
        if (verbose) {
            message("Listening on rsec_pwd_mgmt interface");
        }

        /* listen for requests */
        rpc_server_listen(rpc_c_listen_max_calls_default, &status);

        error(&status, "Unable to listen for requests");

    } FINALLY {

        /* cleanup endpoints */
        rpc_ep_unregister(rsec_pwd_mgmt_v1_0_s_ifspec, bindings,
            (uuid_vector_t *) NULL, &lst);

        /* cleanup binding vector */
        rpc_binding_vector_free(&bindings, &lst);

        /* unregister the interface from the RPC runtime */
        rpc_server_unregister_if(rsec_pwd_mgmt_v1_0_s_ifspec, NULL, &lst);

        /* purge login context */
        sec_login_purge_context(&login_context, &lst);

        /* close file */
        if (!debug_mode)
            fclose(pd_fp);

    } ENDTRY;

    exit(0);
}
