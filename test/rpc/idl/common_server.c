/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: common_server.c,v $
 * Revision 1.1.8.2  1996/02/17  23:13:05  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:05  marty]
 *
 * Revision 1.1.8.1  1995/12/11  19:59:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:10  root]
 * 
 * Revision 1.1.6.1  1994/05/24  17:57:49  tom
 * 	Fix up some compiler warnings.
 * 	[1994/05/24  17:56:34  tom]
 * 
 * Revision 1.1.3.4  1993/02/01  21:33:20  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  21:03:44  hinman]
 * 
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  16:29:39  hinman]
 * 
 * Revision 1.1.3.3  1993/01/07  19:10:38  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:29:08  bbelch]
 * 	Revision 1.1.4.2  1993/01/11  16:14:39  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 9.5.1.4  92/10/28  09:51:31  greg
 * 	bump the rpc thread stacksize in the common manager code to avoid
 * 	stack overflow in calls that marshall huge datatypes.
 * 
 * Revision 9.5.3.2  92/10/26  16:54:47  greg
 * 	Bump the stacksize for server manager call threads up to 96k.  A few
 * 	of the tests were passing huge data types causing stack overflow.  This
 * 	approach fixes them all in one place (the previous fix redefined constants
 * 	in at least four different idl files.  [greg]
 * 
 * Revision 9.5.1.3  92/09/30  16:48:47  root
 * 	Acceptance of OSF rev 1.1.3.2
 * 
 * Revision 9.12.1.2  92/09/30  10:19:48  hinman
 * 	Auto checkin of OSF rev 1.1.3.2
 * 
 * Revision 1.1.3.2  1992/09/29  21:20:42  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:46:05  weisman]
 * 
 * Revision 1.1  1992/01/19  03:34:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#ifdef VMS
#include <file.h>
#else
#ifdef SNI_SVR4
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#endif
#include <test_common.h>

/*globalref make_test_id(_v0_0_epv_t); */
#ifndef NO_M_EPV
globalref make_test_id(_v0_0_m_epv);
#endif
extern char *error_text();
globaldef handle_t server_binding;

main(ac, av)
int ac;
char *av[];
{
    int i;
    char *binding_file = "binding.dat";
    error_status_t st;
    boolean32 validfamily;
    unsigned char *string_binding;
    rpc_binding_vector_p_t bvec;

    if (ac != 2
	/* Check for "server - <family>" */
	&& !(ac == 3 && av[1][0] == '-' && av[1][1] == '\0')
	/* Check for "server -f <binding_file_name> <family>" */
	&& !(ac == 4 && av[1][0] == '-' && av[1][1] == 'f'))
    {
        fprintf(stderr, "Usage: %s [- | -f <binding-file-name>] family\n", av[0]);
        exit(1);
    }
    else {
	    if (ac == 3) {
		    av++;
		    binding_file = 0;
	    }
	    else if (ac == 4) {
		    binding_file = av[2];
		    av += 2;
	    }
    }
	    

# if !defined(PRT) && defined(RT_DEBUG)
    rpc__dbg_set_switches("0-4.3", &st);
# endif

    TRY
    cma_init();
    validfamily = rpc_network_is_protseq_valid(av[1], &st);
    if (st != error_status_ok) 
    {
        fprintf(stderr, "Can't check protocol sequence - %s\n", 
                error_text(st));
        exit(1);
    }
    if (!validfamily) 
    {
        printf("Protocol sequence %s is not valid\n", av[1]);
        exit(1);
    }

    rpc_server_use_protseq(av[1], 5, &st);
    if (st != error_status_ok) 
    {
        fprintf(stderr, "Can't use protocol sequence - %s\n", error_text(st));
        exit(1);
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
    if (st != error_status_ok) 
    {
        printf("Can't register interface - %s\n", error_text(st));
        exit(1);
    }

    rpc_server_inq_bindings(&bvec, &st);
    if (st != error_status_ok) 
    {
        printf("Can't inquire bindings - %s\n", error_text(st));
        exit(1);
    }

    for (i = 0; i < bvec->count; i++)
    {
        unsigned char *uuid,*protseq, *netaddr, *endpoint,*netopt;
        rpc_binding_to_string_binding(
                                      bvec->binding_h[i],
                                      &string_binding,
                                      &st);
        if (st != error_status_ok) 
        {
            printf("Can't get string binding - %s\n", error_text(st));
            exit(1);
        }
        
        rpc_string_binding_parse(string_binding,
                      &uuid, &protseq, &netaddr, &endpoint, &netopt, &st);
        if (st != error_status_ok) 
        {
            printf("Can't parse string binding - %s\n", error_text(st));
            exit(1);
        }

        {
        FILE *binding = 0;
	if (binding_file)
		binding = fopen(binding_file, "w");

	if (binding) {
		fprintf(binding," %s %s %s \n", protseq,netaddr,endpoint);
		fclose(binding);
	}
	else
		printf(" %s %s %s \n", protseq,netaddr,endpoint);
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
    if (st != error_status_ok) 
    {
        printf("Can't register ep - %s\n", error_text(st));
    }
#endif

#ifdef SNI_SVR4
    rpc_mgmt_set_server_stack_size (96000, &st);
    if (st != error_status_ok) 
    {
        printf("SNI SVR4: Can't set server thread stacksize - %s\n", error_text(st));
    }
#endif
    
    rpc_server_listen((long)11, &st);
    if (st != error_status_ok)
        fprintf(stderr, "Error: %s\n", error_text(st));

    CATCH_ALL
#ifdef EP
      rpc_ep_unregister(
                    make_test_id(_v0_0_s_ifspec), 
                    bvec,
                    NULL, 
                    &st);
      if (st != error_status_ok) printf("Can't unregister ep - %s\n", error_text(st));
#endif
      print_exception(THIS_CATCH);
    ENDTRY

    exit(0);
}
