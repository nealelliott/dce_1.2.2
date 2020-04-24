/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dlg_int.c,v $
 * Revision 1.1.9.1  1996/10/17  18:18:54  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:50:18  damon]
 *
 * Revision 1.1.4.2  1994/08/10  19:23:06  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:21  annie]
 * 
 * Revision 1.1.4.1  1994/07/13  22:33:06  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:17:45  devsrc]
 * 
 * 	Delegation Functional Tests - Initial Submit
 * 	[1994/06/08  18:30:24  delgado]
 * 
 * $EndLog$
 */

/* Adapting HP's delegation test for DFS */

#include <stdio.h>
#include <locale.h>
#include <errno.h>

#include <string.h>
#include <macros.h>
#include <dce/dce_error.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>
#include <dce/passwd.h>
#include <dce/daclif.h>
#include <dfsdlg_test.h>
#include <dfsdlg_int.h>

#define LOGFILE "/var/dfs/adm/dlg_int.log"
FILE                    *log;

static sec_login_handle_t   lc;
static unsigned_char_t      entry[256];
extern boolean32            debug;

dfsdlgint_v0_0_epv_t dfsdlgint_v0_0_epv = {dfsdlgint_has_admin_access};

static void usage
#ifndef __STDC__
    (name)
    char *name;
#else
  (
    char *name
  )
#endif
{
    fprintf(stderr,
        "Usage: %s [-p server_princ_name][-f key_table_name][-e name][-d]\n",
        name);
}


boolean32 dfsdlgint_has_admin_access(
    handle_t                 h,
    error_status_t           *stp
)
{
    rpc_authz_cred_handle_t     caller;
    unsigned_char_p_t           pname;
    boolean32                   access;
    unsigned32                  plvl, an, az, su, nt;
    error_status_t              tmp_st;
    uuid_t                      mgr_type;
    handle_t                    tgh;
    sec_login_handle_t          dlh;
    
    CLEAR_STATUS(stp);
    access = FALSE;
    fprintf(stderr, "DEBUG: Entered dfsdlg_int_admin_access\n");

    rpc_binding_inq_auth_caller(h, &caller, &pname, &plvl, &an, &az, stp);
    if (BAD_STATUS(stp)) {
        if (debug)
          fprintf(stderr, "Error from rpc_binding_inq_auth_caller: %x\n",*stp);
        return access;
    }
    
    dlh = sec_login_become_delegate(caller, lc, sec_id_deleg_type_traced,
                                    NULL, NULL, NULL, NULL,
                                    sec_id_compat_mode_initiator, stp);
    if (BAD_STATUS(stp)) {
        if (debug)
            fprintf(stderr, "Error from sec_login_become_delegate: %x\n",*stp);
        return access;
    }

    /* Get a binding to the DFS Test Target server with the delegated context */
    /* Returns TRUE if we have access, FALSE otherwise */
    bind_to_svr(&tgh, (unsigned_char_p_t)TARGET_NSENTRY_NAME, stp);
    if (*stp == error_status_ok)
        access = dfsdlg_target_admin_access(tgh, stp);
    fprintf(log,"dfsdlg_target_admin_access returns %ld\n",access);
test_access_return:
    sec_login_purge_context(&dlh, &tmp_st);
    if (debug && BAD_STATUS(&tmp_st)) {
        fprintf(stderr, "Error purging delegation context: %x\n", *stp);
    }
    fflush(log);
    return access;
}


unsigned char dfsdlg_str_uuid[]="eb63c53a-70ee-11cd-ba62-080009320e12";
char logfile[1024];

int main
#ifndef __STDC__
    (argc, argv) 
    int     argc;
    char    *argv[];
#else
  (
    int     argc,
    char    *argv[]
  )
#endif
{
    error_status_t          st, tmp_st;
    dce_error_string_t      dce_error;
    char                    *keytable;
    char                    keytabname[BUFSIZ];
    unsigned_char_p_t       server_princ_name = (unsigned_char_p_t) INTEMEDIARY_PRINCIPAL_NAME;
    sec_passwd_str_t        pbuf;
    int                     i;

    setlocale(LC_ALL, "");
    strcpy(logfile, DCELOCAL_PATH);
    strcpy(&logfile[strlen(logfile)],LOGFILE);
    log = freopen(logfile,"w+", stderr);
    if (log == NULL){
        printf("Can't open logfile %s \n",logfile);
        exit(errno);
    }
    keytable = NULL;
    CLEAR_STATUS(&st);
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -p requires principal name argument\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
            server_princ_name = (unsigned_char_p_t) argv[i];

        } else if (strcmp(argv[i], "-f") == 0) {
            if ((i+1) >= argc) {
                fprintf(stderr,"Option -f requires keytable name argument\n");
                usage(argv[0]);
                exit(1);
            }
            i++;
            strcpy(keytabname, argv[i]);
            keytable = &keytabname[0];
        } else if (strncmp(argv[i], "-d", 2) == 0) {

            debug = true;

        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            usage(argv[0]);
            exit(1);
        }
    }
    debug=true; 
       /* Do we need this uuid stuff? */
    strcpy((char *) pbuf, INTEMEDIARY_PRINCIPAL_NAME);
    register_server(INTEMEDIARY_NSENTRY_NAME, INTEMEDIARY_PRINCIPAL_NAME,
                    TRUE, dfsdlgint_v0_0_s_ifspec,
                    &dfsdlgint_v0_0_epv, dfsdlg_str_uuid,
                    "DFS Delegation Intermediary Test Server", NULL, &st);
    EXIT_ON_ERROR(st);
    log_in(&lc, (unsigned_char_p_t)INTEMEDIARY_PRINCIPAL_NAME, pbuf, &st);
    EXIT_ON_ERROR(st);
    
    fprintf(log,"Listening...\n");

    rpc_server_listen((unsigned32)5, &st);

    if (st != error_status_ok) {
        dce_error_inq_text(st, dce_error, (int *) &tmp_st);
        fprintf(log, "Error: %s\n", dce_error);
    }
    sec_login_purge_context(&lc, &tmp_st);
}














