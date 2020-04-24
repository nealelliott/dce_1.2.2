/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: server.c,v $
 * Revision 1.1.9.2  1996/02/17  23:28:00  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:09  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:43:32  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/01  14:07 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  18:24  giza
 * 	merge kk & hpdce01
 * 	[1995/12/11  22:20:24  root]
 * 
 * Revision 1.1.4.2  1993/08/24  19:02:20  giza
 * 	Added error logging to local file.
 * 	[1993/08/24  19:01:50  giza]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/04  22:10:58  cjd]
 * 
 * Revision 1.1.1.5  1993/05/04  22:21:56  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.7.2  1993/10/15  21:15:54  ohara
 * 	SVR4 porting changes - 8596
 * 	[1993/10/15  16:35:23  ohara]
 * 
 * Revision 1.1.2.4  1993/03/26  18:23:36  eperkins
 * 	Added casts to eliminate compiler warnings.
 * 	[1993/03/26  18:23:14  eperkins]
 * 
 * Revision 1.1.2.3  1993/02/05  15:21:56  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:42:30  cjd]
 * 
 * Revision 1.1.2.2  1992/09/25  19:26:41  eperkins
 * 	Initial version
 * 	[1992/09/25  19:26:09  eperkins]
 * 
 * $EndLog$
 */
#include <stdio.h>
#ifdef VMS
#include <file.h>
#else
#ifdef SNI_SVR4
#include <sys/types.h>
#endif /* SNI_SVR4 */
#include <sys/file.h>
#endif
#include <test_common.h>
#include "pipetest.h"
#include "tet_code.h"

#define make_test_id(id) pipetest ## id
#ifndef NO_M_EPV
globalref make_test_id(_v0_0_m_epv);
#endif

extern pthread_mutex_t database_mutex;
extern char *error_text();
globaldef handle_t server_binding;

FILE *fp;
static pthread_once_t one_init_block = pthread_once_init;

void init()
{
    pthread_keycreate(&thread_gdata_key, (pthread_destructor_t) free);
}

int
main(ac, av)
int ac;
char *av[];
{
    int i;
    char *binding_file = "binding.dat";
    error_status_t st;
    boolean32 validfamily;
    char *string_binding;
    rpc_binding_vector_p_t bvec;
    char *serverLogfile = "server.log";

    fp = fopen(serverLogfile, "w+a");
    if (!fp) {
        perror("fopen");
        fprintf(stderr, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }
 
    /* Check for "server -f <binding_file_name> <family>" */
    if ( (ac < 4) || !(av[1][0] == '-' && av[1][1] == 'f')) {
        fprintf(fp, "Usage: %s -f <binding-file-name> family\n", av[0]);
        fprintf(fp, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }
    else {
	binding_file = av[2];
	av += 2;
    }
	    
    pthread_once( &one_init_block, init);

# if !defined(PRT) && defined(RT_DEBUG)
    rpc__dbg_set_switches("0-4.3", &st);
# endif
    cma_init();

    TRY
    validfamily = rpc_network_is_protseq_valid((unsigned_char_t *) av[1], &st);

    if (st !=  error_status_ok) {
        fprintf(fp, "Can't check protocol sequence - %s\n", error_text(st));
        fprintf(fp, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }

    if (!validfamily) {
        fprintf(fp, "Protocol sequence %s is not valid\n", av[1]);
        fprintf(fp, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }

    rpc_server_use_protseq((unsigned_char_t *) av[1], (unsigned32) 5, &st);
    if (st != error_status_ok) {
        fprintf(fp, "Can't use protocol sequence - %s\n", error_text(st));
        fprintf(fp, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }

    rpc_server_register_if(
                    make_test_id(_v0_0_s_ifspec), 
                    NULL,
#ifdef NO_M_EPV
                    NULL,
#else
                    (rpc_mgr_epv_t)&make_test_id(_v0_0_m_epv),
#endif
                    &st);
    if (st != error_status_ok) {
        fprintf(fp, "Can't register interface - %s\n", error_text(st));
        fprintf(fp, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }

    rpc_server_inq_bindings(&bvec, &st);
    if (st != error_status_ok) {
        fprintf(fp, "Can't inquire bindings - %s\n", error_text(st));
        fprintf(fp, "tet_result = UNINITIATED\n");
        exit(TET_UNINITIATED);
    }

    for (i = 0; i < bvec->count; i++) {
            
        char *uuid,*protseq, *netaddr, *endpoint,*netopt;

        rpc_binding_to_string_binding(
                                      (rpc_binding_handle_t) bvec->binding_h[i],
                                      (unsigned_char_t **) &string_binding,
                                      &st);
        if (st != error_status_ok) {
            fprintf(fp, "Can't get string binding - %s\n", error_text(st));
            fprintf(fp, "tet_result = UNINITIATED\n");
            exit(TET_UNINITIATED);
        }
        
        rpc_string_binding_parse((unsigned_char_p_t) string_binding,
                      		(unsigned_char_p_t *)&uuid, 
				(unsigned_char_p_t *)&protseq, 
				(unsigned_char_p_t *)&netaddr, 
				(unsigned_char_p_t *)&endpoint, 
				(unsigned_char_p_t *)&netopt, 
				&st);
        if (st != error_status_ok) {
            fprintf(fp, "Can't parse string binding - %s\n", error_text(st));
            fprintf(fp, "tet_result = UNINITIATED\n");
            exit(TET_UNINITIATED);
        }

        {
            FILE *binding = 0;
            
            if (binding_file)
	        binding = fopen(binding_file, "w");

	    if (binding) {
	        fprintf(binding," %s %s %s \n", protseq,netaddr,endpoint);
	        fclose(binding);
            }
	    else {
	        perror("fopen");
                fprintf(fp, "tet_result = UNINITIATED\n");
                exit(TET_UNINITIATED);
            }
        }
    }
    server_binding = bvec->binding_h[0];
#ifdef EP
    rpc_ep_register(
                    make_test_id(_v0_0_s_ifspec), 
                    bvec,
                    NULL, 
                    "Test Server",
                    &st);
    if (st != error_status_ok) {
        fprintf(fp, "Can't register ep - %s\n", error_text(st));
    }
#endif

    /*
     * Initialize the mutex that protects the database.
     */
    pthread_mutex_init(&database_mutex, pthread_mutexattr_default);

    rpc_server_listen((long)1, &st);
    if (st != error_status_ok) {
        fprintf(fp, "Error: %s\n", error_text(st));
        fflush(fp);
        exit(TET_UNINITIATED);
    }

    CATCH_ALL
#ifdef EP
      rpc_ep_unregister(
                    make_test_id(_v0_0_s_ifspec), 
                    bvec,
                    NULL, 
                    &st);
      if (st != error_status_ok) fprintf(fp, "Can't unregister ep - %s\n", error_text(st));
#endif
        print_exception(THIS_CATCH);
    ENDTRY

    exit(TET_PASS);
}

