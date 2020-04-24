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
 * $Log: dlg_target.c,v $
 * Revision 1.1.9.1  1996/10/17  18:18:57  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:50:20  damon]
 *
 * Revision 1.1.4.2  1994/08/10  19:23:07  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:17:23  annie]
 * 
 * Revision 1.1.4.1  1994/07/13  22:33:08  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:17:47  devsrc]
 * 
 * 	Delegation Functional Tests - Initial Submit
 * 	[1994/06/08  18:30:27  delgado]
 * 
 * $EndLog$
 */

/*
 * The target server for the DFS delegation user space testing.
 * 
 */
#include <dce/uuid.h>
#include <dce/dce_error.h>
#include <dce/sec_login.h>
#include <dcedfs/dfsauth.h>
#include <dfsdlg_target.h>
#include <dfsdlg_test.h>

#define ADMIN_LIST_NAME "admin.dfsdlg_target"
#define LOGFILE "/var/dfs/adm/dfsdlg_target.log"

unsigned char dfsdlg_str_uuid[] = "0823c9ae-70ef-11cd-8bf5-080009320e12";
char logfile[1024];
FILE *log;

dfsdlg_target_v0_0_epv_t dfsdlg_target_v0_0_epv = { dfsdlg_target_admin_access};

main (argc, argv)
    int argc;
    char *argv[];

{
    unsigned32 code=0, st, tmp_st;
    uuid_t dfsdlg_target_objid, uuid_nil;
    dce_error_string_t    dce_error;
    
    strcpy(logfile, DCELOCAL_PATH);
    strcpy(&logfile[strlen(logfile)],LOGFILE);
    log = freopen(logfile,"w+", stderr);
    if (log == NULL){
        printf("Can't open logfile %s \n",logfile);
        exit(errno);
    }
    code = dfsauth_server_InitAuthentication(TARGET_PRINCIPAL_NAME, 
                                             ADMIN_LIST_NAME, NULL);

/*    code = dfsauth_client_AutoRefreshLocalAuthContext(); */

    register_server(TARGET_NSENTRY_NAME, TARGET_PRINCIPAL_NAME, FALSE, 
                           dfsdlg_target_v0_0_s_ifspec,
                           &dfsdlg_target_v0_0_epv, dfsdlg_str_uuid,
                           "DFS Delegation Target Test Server", NULL, &st);
    fprintf(log,"Listening .... \n");
    rpc_server_listen((unsigned32)5, &st);
    if (st != error_status_ok){
        dce_error_inq_text(st, dce_error, (int *)&tmp_st);
        fprintf(stderr, "Error: rpc_server_listen %s \n", dce_error);
    }
}


boolean32
dfsdlg_target_admin_access(h, st)
    handle_t h;
    error_status_t *st;
{
    long result;

    *st = error_status_ok;
    fprintf(log,"dfsdlg_target_admin_access entered \n");
    result = dfsauth_server_CheckAuthorization((rpc_binding_handle_t *)&h, 
                             (dacl_permset_t *)NULL, 
                             (char *)NULL, (uuid_t *)NULL);
    fprintf(log,"dfsauth_server_CheckAuthorization returns %ld\n", result);                       fflush(log);
    if (result)
        return FALSE;
    else
        return TRUE;
   
}






