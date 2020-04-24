/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client.c,v $
 * Revision 1.1.9.2  1996/02/17  23:27:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:06  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:43:16  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  19:39 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  17:57  giza
 * 	merge kk & hpdce01
 * 	[1995/12/11  22:20:16  root]
 * 
 * Revision 1.1.4.2  1993/08/24  18:43:10  giza
 * 	Added error logging and remote test status reporting.
 * 	[1993/08/24  18:42:38  giza]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/04  22:10:22  cjd]
 * 
 * Revision 1.1.1.5  1993/05/04  22:21:51  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.2.3  1993/02/05  15:21:44  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:42:07  cjd]
 * 
 * Revision 1.1.2.2  1992/09/25  19:20:03  eperkins
 * 	Initial version
 * 	[1992/09/25  19:19:41  eperkins]
 * 
 * $EndLog$
 */
/*
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/utsname.h>
#include <dce/cma.h>
#include <dce/rpcexc.h>
#include <dce/cmalib_crtlx.h>       /* C RTL wrapper routines (thread-safe) */
#include <dce/pthread_exc.h>
#include "pipetest.h"
#include "tet_code.h"

#define make_test_id(id) pipetest ## id
#define BINDING_LEN 256

extern char *error_text();
extern void print_exception();
extern int client_aux();

FILE *fp;
int Pass;

main(ac, av)
int ac;
char *av[];
{
    error_status_t st;
    rpc_binding_handle_t h = NULL;
    ndr_char *string_binding;
    static ndr_char nil_string[] = "";
    int pass, passes=10, failures = 0, pass_accum = 0;
    /*
     * DB specifics
     */
    unsigned32 my_addr, status, namelen, file_len;  
    uuid_t db_handle;
    char  my_name[64];
    struct hostent *hp;
    unsigned_char_p_t uuid_string;
    struct utsname utslist;
    char *inetAddr;
    char clientLogfile[256];
    
    /*sprintf(clientLogfile,"%s/client_%d.log", getenv("PWD"), getpid());*/
    sprintf(clientLogfile,"client_%d.log", getpid());
    file_len = strlen(clientLogfile);
    fp = fopen(clientLogfile, "w+a");
    if (!fp) {
        perror("fopen");
        fprintf(stderr, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }

        

    TRY
#ifdef PRT
    {
    globalref make_test_id(_v0_0_epv_t);
#ifndef NO_M_EPV
    globalref make_test_id(_v0_0_m_epv);
#endif

    cma_init();
    rpc_server_register_if(
                    make_test_id(_v0_0_s_ifspec),
                    NULL,
#ifdef NO_M_EPV
                    NULL,
#else
                    (rpc_mgr_epv_t)&make_test_id(_v0_0_m_epv),
#endif
                    &st);
    }

    if (st != error_status_ok) 
    {
        fprintf(fp, "Can't register interface - %s\n", error_text(st));
        fprintf(fp, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }
    if (ac >= 2) passes = atoi(av[1]);
    if ((passes < 1) || (passes > 100)) passes = 10;

    /* Set handle to something plausible so rpc_binding_dup succeeds. */
    h = (rpc_binding_handle_t)12345678;
#else

  /* If no parameters are specified, assume auto_handle for now */
  if (ac != 2)
  {

    if (ac != 5) 
    {
        fprintf(fp, "usage: %s protseq hostid endpoint passes\n", av[0]);
        fprintf(fp, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }

    passes = atoi(av[4]);

    rpc_string_binding_compose(NULL,        /* uuid */
                               (ndr_char *)av[1], /* protocol sequence */
                               (ndr_char *)av[2], /* netaddr */
                               (ndr_char *)av[3],
                               NULL,        /* options */
                               &string_binding,
                               &st);
    if (st != error_status_ok) 
    {
        fprintf(fp, "Can't compose string binding %s\n", 
                error_text(st));
        fprintf(fp, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }

    rpc_binding_from_string_binding(string_binding, &h, &st);
    if (st != error_status_ok) 
    {
        fprintf(fp, "Can't get binding from string binding %s - %s\n", 
                string_binding, error_text(st));
        fprintf(fp, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }
  }

# if !defined(PRT) && defined(RT_DEBUG)
    rpc__dbg_set_switches("0-4.3", &st);
# endif
#endif

   /*
    * Gather the information that we'll need to create a record in the
    * database.  We'll need the name of the host on which we're running,
    * along with its IP address.
    */
    sethostent(1);
    uname(&utslist);
    if ( &utslist == NULL ) {
        perror("uname");
        strcpy(my_name, "unknown");
    }
    else {
        hp = gethostbyname(utslist.nodename);
        if ( hp == NULL ) {
            perror("gethostbyname");
            fprintf(fp, "tet_result = NET_FAIL\n");
            exit(TET_NET_FAIL);
        }
        my_addr = *(unsigned long *) hp->h_addr;
        inetAddr = inet_ntoa((struct in_addr *) my_addr);
        strcpy(my_name, hp->h_name);
        namelen = strlen(my_name);
        endhostent();
    }

    /*
     * printf("%s@(%s)\n", my_name, inetAddr);
     * Now, do the create record call.  
     */
    fprintf(fp, "Creating a record for %s@(%s)\n", my_name, inetAddr);
    create_record(h, (idl_byte *) my_name, namelen, my_addr, &db_handle, &status);
    if (status != 0)
    {
        fprintf(fp, "couldn't create record, status = %d\n", status);
        fprintf(fp, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
        exit(-1);
    }
    else                          
    {
        uuid_to_string(&db_handle, &uuid_string, &status);
        /* printf("record created, handle = %s\n", uuid_string);
         */
        rpc_string_free(&uuid_string, &status);
    }

    for (pass = 0; pass < passes; pass++)
    {
        Pass = pass;
        /* printf("LOOP %d\n", pass);
         */
        failures += client_aux(h, passes);
    }
    pass_accum = passes - failures;
    fprintf(fp, "Summary: %d passes, %d failures\n", pass_accum, failures);

    /*
     * And update the database.
     */
    reportTestStatus(h, &db_handle, pass_accum, failures, clientLogfile, file_len, &st);

    CATCH_ALL
        print_exception(THIS_CATCH);
        failures += 1;
        fflush(fp);
    ENDTRY

    fprintf(fp, "tet_result = %s\n", failures ? "FAIL" : "PASS" );
    fclose(fp);
    exit ( failures ? TET_FAIL : TET_PASS );
}







