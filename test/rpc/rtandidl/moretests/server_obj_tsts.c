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
 * $Log: server_obj_tsts.c,v $
 * Revision 1.1.5.2  1996/02/17  23:22:07  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:06  marty]
 *
 * Revision 1.1.5.1  1995/12/11  20:09:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:57  root]
 * 
 * Revision 1.1.3.3  1993/01/11  23:40:03  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:13:32  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:22:53  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:48:01  weisman]
 * 
 * Revision 1.1  1992/01/19  03:38:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  %a%copyright(,**  )
**
**  NAME
**
**      server_obj_tsts.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This module contains the set of routines which comprise
**  the top level of the Server Object test group. These
**  routines are provided as a tool which tests the functional 
**  and pathological contingencies that user applications present.
**  These tests are designed to be run from the Tshell, 'tsh'.
**  All inputs are in the form of character strings in the,
**  "argv, argc", format.  Specific tests are implemented in
**  a 'tsh' script, with definitions in a 'tsh' include file:
**  file.tsh and file.tshrc.  These routines are linked with
**  testdriver.c which provides the 'main' calling program.
**
**  This test group is designed as the 'server-side', of a
**  client/server partnership.  The routines which comprise
**  the client side are contained in comtest_client_xfr.c.
**  The top level, main, (contained in testdriver.c), is 
**  identical for both the client and the server.  
**  The server-side executable is 'tstsrvr', and the
**  client-side executable  is 'tstdriver'.  These names are
**  delineated at linkage time by maketstdrvr.ult.
**  In order to run these tests as a client/server pair the 'tsh'
**  must have invoked tstdriver prior to invoking tstsrvr. Once
**  tstdriver is running it 'forks' a child process and the parent
**  returns to the 'tsh' where the first test is began immediately
**  by invoking tstsrvr with the proper arguments to run the desired
**  test. The client/server pair communicate through a set of
**  temporary files.  A wait parameter is provided in the client
**  to allow the server to output to be invoked, initialize the 
**  test, and output its parameters to temp. files.  Once the
**  server calls rpc_server_listen.  It sleeps, waiting for RPCs
**  from the client.
**
**  Tests may also be conducted passively, (i.e., the server return to
**  the 'tsh' upon completion of the test), in order to test simple
**  functionality without waiting for actual RPCs from a client.  The
**  'server_wait' keyword controls whether the server actually calls
**  rpc_server_listen and waits for RPCs.  Tests should be grouped in
**  the 'tsh' command script so that passive tests are conducted first,
**  followed by the invocation of the client and then a series of
**  client/server RPC exchanges.
**

**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**  30-sep-91 bmartin   Remove inclusion of com.h and replace with sysconf.h 
**                      and stubbase.h so we can build against shareable image.
**  25-jul-91 bmartin   Correct mgr epv initialization in 
**                      comtest_server_parse_command; Was not being setup
**                      for the "generated" epv case.
**  23-jul-91 bmartin   shorten comtest_server_xxx function names to
**                      comtst_svr_xxx for portability; correct string_buf[0]
**                      initialization to NUL character ('\0') not NULL pointer.
**  23-apr-91 woodbury  de-TABBED
**  18-mar-91 woodbury  Provide corect return for pathological cases 
**                      (when *status != rpc_s_ok) in use_protseq tests.
**  05-mar-91 burton    fixed problem with signal dispatcher
**  11-feb-91 burton    fixed parser more than one interface
**  09-feb-91 burton    extraifs became interfaceX and interfaceY
**  02-feb-91 burton    change to register type_uuid only once
**  01-feb-91 burton    add epv to comtest_command_t
**  01-feb-91 burton    add interface to struct comtest_command_t
**  29-jan-91 burton    add rpc_object_set_type to comtest_server_register_if
**  28-jan-91 burton    remove longjump
**  27-jan-91 burton    add another pass of args for generated if
**  25-jan-91 burton    add comtest_parse_command
**  17-jan-91 burton    fix  pointer in comtest_use_all_protseqs
**  10-jan-91 burton    fix argv pointer to run on mips
**  08-jan-91 burton    wait capability to comtest_server_use_protseq_ep
**  08-jan-91 burton    wait capability to comtest_server_use_protseq
**  07-jan-91 burton    add communication of pid to client
**  02-jan-91 burton    place both generated and default functions in
**                      comtest_server_register_if
**  29-dec-90 burton    fix comtest_register_if, default and generated
**  13-nov-90 woodbury  fix comtest_register_if so it calls
**                      rpc__mgmt_inq_if_ids
**  25-oct-90 woodbury  added stubs for comtest_register_if subroutines
**                      & made it relatively 'lint'-free
**  24-oct-90 woodbury  add headers, use of macros.
**                      add comtest_server_inq_bindings.
**  15-oct-90 burton    original
**
**  %a%private_end  
**
*/

/*#include    <sysconf.h>   */          /* System definitions for RPC runtime */
#include    <dce/rpc.h>             /* Public definitions for RPC runtime */
#include    <dce/stubbase.h>        /* Private definitions for RPC runtime */
#include    <tstdriver.h>           
#include    <comtest.h>
#include    <server_obj_tsts.h>

/*
**++
**
**  ROUTINE NAME:       comtst_svr_register_if
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**  comtst_svr_register_if is called on the server side
**  for testing client/server operations.
**  RPC routines exercised:
**
**  rpc_server_register_if
**  rpc_server_use_all_protseqs
**  rpc_object_set_type
**  rpc_string_free
**  rpc_mgmt_inq_if_ids
**  uuid_to_string
**  rpc_string_free
**  rpc_mgmt_inq_if_ids
**  uuid_create_nil
**  uuid_equal
**  
**  This routine is designed to be test the registeration
**  of various interface specificatins together with various
**  manager objects.  Interfaces are registered through calls
**  to rpc_server_register_if  with a specified 'type uuid'
**  and manager entry-point-vector. Once an interface is
**  registered with the RPC runtime, one or more calls are
**  made to rpc_object_set_type to register objects uuids.
**  
**  
**  The routine is designed to be run from the 'tsh' by passing
**  arguments in the form of various keywords.
**
**  INPUTS:
**
**      argc            argument count
**      argv            array of pointers to character
**                      string input arguments
**
**  The calling syntax:
**
**      comtst_svr_register_if generated/default \
**                          [server_wait] [bogus_obj] \
**                    [( type_uuid1 object_uuid1 epv_index if_index)\
**                    (.....) ....]
**
**  Keywords:
**             default      specified to run tests with a server supplied
**                          interface specification and a null epv.n
**
**           generated      specified to run tests with interfacees
**                          generated by the IDL compiler.  Two IDL generated
**                          interfaces are available to emulate this function,
**                          refered to as interface_x and interface_y
**                          
**         server_wait      use this keyword only if you want the server
**                          to call rpc_server_listen and wait for the client
**                          to send RPCs.  If a return to the 'tsh' is desired
**                          omit this keyword.
**       
**           bogus_obj      this keyword forces the client to send RPCs for
**                          with an object uuid which is not registered on the
**                          server-side.  This keyword should be used with a
**                          normal, (type_uuid object_uuid epv_indexif_index).
**   
**          ( type_uuid object_uuid epv_index if_index )  One or more of these
**          strings are provided to specify, the manager type UUID, the object UUID,
**          the manager EPV, and the IDL supplied interface which is to be
**          registered.  UUIDs are specified either as a uuid string generated
**          by the 'uuid_gen' utility, or the keyword, 'nil_uuid', which will
**          cause a NIL UUID, either type or object, to be registered.  The
**          'epv_index', presently will be either 1 or 2 and the interface
**          index will be either X or Y.  Two IDL generated interfaces are
**          provided, (interface_X or interface_Y) along with two managers
**          having two implementations each.  Also, each manager
**          implementation provides two routines, op1 and op2., this numerous test
**          combinations may be implemented by changing the 'tsh' command script.
**          
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:
**
**           When the -v 'verbose' option is used,  various information
**           about the command line arguments, the reception of RPCs from
**           the client, etc are printed to 'standard out'.
**
**
**       COMTST_protseqs_tmpfile - a list of protocol sequence strings
**                                 supported on the local host
**
**       COMTST_pid_tmpfile -      a contains the pid of the server process
**                                 available to the client to send signals
**                                 to the server
**
**       COMTST_obj_tmpfile -      a list of object uuid strings
**                                 to which a client may send RPCs
**
**
**
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      return          an integer specifying success or some type of failure
**      
**
**  SIDE EFFECTS:       none
**
**--
**/


comtst_svr_register_if (argc, argv, status)

unsigned32 argc;
char *argv[];
unsigned32  *status;

{
    FILE                     *objfile;
    uuid_t                   uuid_nil;
    rpc_binding_vector_p_t   bind_v;
    rpc_if_handle_t          interface;
    comtest_command_p_t      comm;
    comtest_command_p_t      n_comm;
    rpc_if_id_vector_t       *if_vec;
    unsigned16               nreg;
    register_if_test_type    test;
    boolean32                listen;
    boolean32                nil_flag;
    rpc_if_rep_p_t           if_rep_p;
    unsigned int             i;
    unsigned int             retval;
    unsigned_char_p_t        obj;
    unsigned_char_p_t        iface;
    int                      x_argc;
    char                     **x_argv;
    
    
    
    
    CHECK_ARGS(SERVER_REGISTER_IF_ARGS);
    PRINT_ARGS;
    
    x_argc = --argc;
    x_argv = ++argv;
    listen = COMTST_server_return;
    
    /*
     * scan through arg list set flags
     */
    for ( i = 0; x_argc; i++ )
    {
        if ((strcmp (*x_argv, REGISTER_DEFAULT_EPV)) == 0)
        {
            test = DEFAULT;
        }

        if ((strcmp (argv[i], REGISTER_GENERATED_EPV)) == 0)
        {
            test = GENERATED;
        }
        
        if ((strcmp (argv[i], SERVER_RPC_WAIT)) == 0)
        {
            listen = COMTST_server_wait;
        }
        else if ((strcmp (argv[i], "null_test")) == 0)
        {
            listen = 3;
        }
        
        if ((strcmp (argv[i], BOGUS_OBJ_UUID)) == 0)
        {
            bogus_obj = COMTST_bogus_obj_set;
        }
        
        --x_argc; ++x_argv;
        continue;
    }
    
     /*
      * get all the protocol sequence strings supported on
      * the local host
      */
    rpc_server_use_all_protseqs (COMTST_max_calls, status);

    if (*status != rpc_s_ok)
    {
        return (COMTEST_FAILED);
    }

    /*
     * convert all protocol sequences to string bindings
     * and output to file COMTST_protseqs_tmpfile. 
[No point
     * in doing it if we are not going to listen for client.]

     * just open COMTST_obj_tmpfile for later use.
     */   
    if (listen >= COMTST_server_wait)
    {
        comtst_svr_get_binding_strings ( &bind_v, status );
        CHECK_STATUS;
        
        if ((objfile = fopen (COMTST_obj_tmpfile, "w+")) == NULL)
        {
            return (COMTEST_FAILED);
        }
    }   
    
    /*
     * which test default or generated
     */

    switch (test)
    {
        /*
         *  The DEFAULT server supplied interface spec.
         */
        case DEFAULT:
        /* 
         * Set up a NIL uuid
         */
        uuid_create_nil (&uuid_nil, status);
        
        /*
         * register the interface (with a nil type and null mepv), should get
		 * rpc_s_no_mepv, since we compiled with -no_mepv
         */              

		in_status = rpc_s_no_mepv;
        rpc_server_register_if (
                                comtest_server_v2_0_s_ifspec,
                                &uuid_nil,
                                (rpc_mgr_epv_t) NULL,
                                status);
        
        if (*status != rpc_s_ok)
        {
            if (*status == in_status)
            {
                return (COMTEST_OK);
            }
            else
            {
                return (RPC_CALL_FAILED);
            }
        }                              


        
        /*
         *  get a vector of registered interfaces
         */
        rpc_mgmt_inq_if_ids (NULL, &if_vec, status);

        if (*status != rpc_s_ok)
        {
            return (RPC_CALL_FAILED);
        }
        

        /*
         * try to find the interface we just registered
         */

        nreg = if_vec->count;
        if (nreg > 0)
        {
            if_rep_p = (rpc_if_rep_p_t) comtest_server_v2_0_s_ifspec;
            for (i = 0; i < nreg; i++)
            {
                if (uuid_equal (&if_vec->if_id[i]->uuid,
                                &if_rep_p->id, status))
                {
                    retval = COMTEST_OK;
                }


            }
            retval = COMTEST_OK;
        }

        /*
         *  now unregister the interface
         */
        rpc_server_unregister_if (comtest_server_v2_0_s_ifspec,
                                            (uuid_p_t) NULL, status);

        if (*status != rpc_s_ok)
        {
            return (RPC_CALL_FAILED);
        }
        
        return (retval);
        
        break;


        /*
         *  The GENERATED or IDL  supplied interface spec.
         */
        
        case GENERATED:
        
        /*
         * parse the input command line supplied by the tsh
         * get all bindings supported on this host and output
         * binding strings to tmp file
         */

        comm = comtst_svr_parse_command ( argc, argv, status );
        CHECK_STATUS;
        
        /*
         * for each type uuid, manager epv, object uuid, and
         * interface specification, supplied on input. try to
         * call rpc_server_register_if and rpc_object_set_type
         */

        for ( n_comm = comm;  n_comm;
             n_comm = (comtest_command_p_t)n_comm->next )
        {
            
            /*
             * if requested print command line info
             */

            if (verbose)
            {
                uuid_to_string (&n_comm->uuid_type, &iface, status);
                uuid_to_string (&n_comm->uuid_obj, &obj, status);
                fprintf (stdout, "\n type uuid %s\n obj uuid %s\n",
                        iface, obj);
                rpc_string_free (&obj, status);
                rpc_string_free (&iface, status);
            }
            
            /*
             * get the interface we want to register
             */
            interface = n_comm->interface;
            if_rep_p = (rpc_if_rep_p_t) n_comm->interface;
            
            /*
             * register only the first manager type and epv
             * in a series.  If this is the first one of this type
             * register it
             */

            if (!(uuid_equal (&n_comm->uuid_type,
                              &previous_type_uuid, status)))
            {
                rpc_server_register_if (
                                        interface,
                                        (uuid_p_t) &n_comm->uuid_type,
                                        (rpc_mgr_epv_t) n_comm->epv,
                                        status);
                
                if (*status != rpc_s_ok)
                {
                    if (*status == in_status)
                    {
                        return (COMTEST_OK);
                    }
                    else
                    {
                        return (RPC_CALL_FAILED);
                    }
                }
                previous_type_uuid = n_comm->uuid_type;     
            }

            /*
             * If this is not a NIL object uuid, try to register the
             * object UUID supplied on the command line. we can override
             * this and test trying to register it anyway by using -c xxx
             * where xxx is the expected error return from rpc_object_set_type
             */
            if (in_status ||
                ((nil_flag =
                  uuid_is_nil (&n_comm->uuid_obj, status)) == false))
            {
                rpc_object_set_type ((uuid_p_t) &n_comm->uuid_obj,
                                     (uuid_p_t) &n_comm->uuid_type, status);
                
                if (*status != rpc_s_ok)
                {
                    if (*status == in_status)
                    {
                        return (COMTEST_OK);
                    }
                    else
                    {
                        return (RPC_CALL_FAILED);
                    }
                }
            }       

            /*
             * output an object UUID to the tmp file
             * COMTST_obj_tmpfile.  if it is a nil,
             * uuid - output string 'nil_uuid', also
             * provision for a bogus object uuid.
             * 
[There is no reason to do it if we aren't
             * going to listen for a client]

             */     

            if (listen >= COMTST_server_wait)
            {
                if (bogus_obj == COMTST_bogus_obj_set)
                {
                    fprintf (objfile, "%s  %c\n", COMTST_bogus_obj_uuid,
                             n_comm->if_desig);
                }
                else
                {
                    if ((nil_flag =
                         uuid_is_nil (&n_comm->uuid_obj, status)) == true)
                    {
                        fprintf (objfile, "%s\n", NIL_UUID);
                    }
                    else
                    {
                        uuid_to_string (&n_comm->uuid_obj, &obj, status);
                        fprintf (objfile, "%s  %c\n", obj,
                                 n_comm->if_desig);
                        rpc_string_free (&obj, status);
                        if (*status != rpc_s_ok)
                        {
                            return (RPC_CALL_FAILED);
                        }
                    }
                }
            }
        }
        retval = COMTEST_OK;    

        /*
         * if we expect RPCs from a client
         * close the object file
         */
        if (listen >= COMTST_server_wait)
        {
            fflush (objfile);
            fclose (objfile);
        }   

        /*
         * get a vector of registered interfaces and
         * see if we can locate one
         */     
        rpc_mgmt_inq_if_ids (NULL, &if_vec, status);
        if (*status != rpc_s_ok)
        {
            return (RPC_CALL_FAILED);
        }

        /*
         * now if we are not going to wait for a client
         * try to unregister each interface 
         */
        if (listen == COMTST_server_return)
        {
            previous_type_uuid = uuid_nil;          
            for (i = 0,nreg = if_vec->count; nreg > 0; nreg--,i++ )
            {
                for ( n_comm = comm;  n_comm;
                     n_comm = (comtest_command_p_t)n_comm->next )
                {
                    if (!(uuid_equal (&n_comm->uuid_type,
                                      &previous_type_uuid, status)))
                    {
                        if_rep_p = (rpc_if_rep_p_t) n_comm->interface;
                        if (uuid_equal (&if_vec->if_id[i]->uuid,
                                        &if_rep_p->id, status))
                        {
                            rpc_server_unregister_if ((rpc_if_handle_t) if_rep_p,
                                                      (uuid_p_t)&n_comm->uuid_type,
                                                      status);
                            if (*status != rpc_s_ok)
                            {
                                retval = RPC_CALL_FAILED;
                            }
                            previous_type_uuid = n_comm->uuid_type;         
                            break;
                        }
                    }
                }
            }
        }
        else if (listen == 3)
        {
            rpc_server_unregister_if (NULL, NULL, status);
        }
        /*
         * free the space for the interface vector
         */     
        rpc_if_id_vector_free (&if_vec,  status);
        if (*status != rpc_s_ok)
        {
            retval = RPC_CALL_FAILED;
        }
        /*
         * get a vector of registered interfaces and
         * see if we can locate one
         */     
        rpc_mgmt_inq_if_ids (NULL, &if_vec, status);
        if (verbose && (*status == rpc_s_no_interfaces))
        {
            printf ("interfaces sucessfully unregistered\n");
        }
        if (*status == rpc_s_ok)
        {
            rpc_if_id_vector_free (&if_vec,  status);
            if (*status != rpc_s_ok)
            {
                retval = RPC_CALL_FAILED;
            }
        }

        break;
    }
    /*
     * if listen flag is set to wait return immediately
     * otherwise rpc_server_listen is called and we
     * never return here
     */    
    comtst_svr_wait ( listen, status );
    if (*status != in_status)
    {
        return (COMTEST_FAILED);
    }

    return (retval);
}


/*
**++
**
**  ROUTINE NAME:       comtst_svr_inq_bindings
**
**  SCOPE:              global to test program
**
**  DESCRIPTION:
**      
**  test rpc_server_inq_bindings without previously
**  creating any bindings. the test should return
**  an error code which matches the error code supplied
**  throug the -c argument on the input command
**  
**  INPUTS:             none 
**
**
**  INPUTS/OUTPUTS:     none 
**
**
**  OUTPUTS:            
**
**              status  - code returned from the
**                        call to rpc_server_inq_bindings
**
**  IMPLICIT INPUTS:    
**                      it is assumed that the caller
**                      supplies the expected error return
**                      with the -c error_code on the input
**                      from the tsh
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     integer
**
**      return          an integer specifying success or some type of failure
**
**  SIDE EFFECTS:
**
**--
**/


comtst_svr_inq_bindings (argc, argv, status)

int argc;
char *argv[];
unsigned32  *status;

{
    rpc_binding_vector_t    *binding_vec_ptr;
    unsigned32              temp_status;
    
    CHECK_ARGS(SERVER_INQ_BINDINGS_ARGS);
    
    PRINT_ARGS;
    
    rpc_server_inq_bindings (&binding_vec_ptr, status);
    
    if (*status == in_status)
    {
          return (COMTEST_OK);
    }
    return (COMTEST_FAILED);
}


/*
**++
**
**  ROUTINE NAME:       comtst_svr_use_all_protseqs
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**  Test rpc_server_use_all_protseqs then proceed to
**  acquire the bindings and convert to string bindings
**  report bindings found and output bindings to tmp
**  file 
**
**  INPUTS:             none
**
**      argc            argument count
**      argv            array of pointers to character
**                      string input arguments
**
**  The calling syntax:
**
**         server_wait  keyword supplied as input if we
**                      intend to call rpc_server_listen
**                      and wait for response from a client
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:
**
**
**       COMTST_protseqs_tmpfile - a list of protocol sequence strings
**                                 supported on the local host
**
**       COMTST_pid_tmpfile -      a contains the pid of the server process
**                                 available to the client to send signals
**                                 to the server
**
**
**  IMPLICIT INPUTS:
**
**  IMPLICIT OUTPUTS:
**
**  FUNCTION VALUE:
**
**      return          an integer specifying success or some type of failure
**
**  SIDE EFFECTS:       none
**
**--
**/

comtst_svr_use_all_protseqs (argc, argv, status)

int argc;
char *argv[];
unsigned32  *status;

{
    rpc_binding_vector_p_t  bind_v;
    unsigned_char_p_t       stringb;
    unsigned16              bv_index;
    boolean32               listen;
    char                    **x_argv;



    CHECK_ARGS(SERVER_USE_ALL_PROTSEQS_ARGS);

    PRINT_ARGS;

    /*
     * call rpc_server_use_all_protseqs to instruct
     * the run time to initialize binding handles for
     * all the protocols supported on the local host
     */

    rpc_server_use_all_protseqs (COMTST_max_calls, status);
    if (*status != in_status)
    {
        return RPC_CALL_FAILED;
    }

    /*
     * go get a vector of all supported bindings and
     * create string bindings for each and output to
     * tmp file COMTST_protseqs_tmpfile for use by a
     * client
     */
    comtst_svr_get_binding_strings ( &bind_v, status );
    CHECK_STATUS;

    /*
     * register our server supplied interface with
     * the runn time
     */

    rpc_server_register_if (comtest_server_v2_0_s_ifspec, NULL,
                            (rpc_mgr_epv_t) &comtest_server_epv,
                            status);

    if (*status != in_status)
    {
        return RPC_CALL_FAILED;
    }

    x_argv = ++argv;

    /*
     * do we want to wait for a client
     */

    if (--argc && (strcmp (*x_argv, SERVER_RPC_WAIT)) == 0)
    {
        comtst_svr_wait ( COMTST_server_wait, status );
    }
    else
    {
        comtst_svr_wait ( COMTST_server_return, status );
    }

    /*
     * if we got here we didn't want to wait
     */    

    if (*status != in_status)
    {
        return (COMTEST_FAILED);
    }

    return (COMTEST_OK);
}


/*
**++
**
**  ROUTINE NAME:       comtst_svr_use_all_protseqs_if
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**  with a series of specified protocol sequence and endpoint
**  strings, test rpc_server_use_all_protseqs_if after which
**  handles to the resulting bindings are acquired and converted
**  to string bindings. these bindings are reported and output
**  to bindings to a tmp file, COMTST_protseq_tmpfile. Optionally
**  the process may enter a wait state and wait for expected RPCs
**  from a client which is assumed to be invoked by the 'tsh'.
**
**
**  INPUTS:             none
**
**      argc            argument count
**      argv            array of pointers to character
**                      string input arguments
**
**  The calling syntax:
**
**      protseq endpoint [protseq endpoint] [..]...
**
**                      where 'protseq' is any valid protocol sequence
**                      string currently supported on the local host
**                      and 'endpoint' is any endpoint string valid with
**                      the proceeding protocol sequence string.  multiple
**                      bindings may be allocated by expressing a series
**                      of these 'protseq endpoint' pairs
**
**
**         server_wait  keyword supplied as input if we
**                      intend to call rpc_server_listen
**                      and wait for response from a client
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:
**
**       COMTST_protseqs_tmpfile - a list of protocol sequence strings
**                                 supported on the local host
**
**       COMTST_pid_tmpfile -      a contains the pid of the server process
**                                 available to the client to send signals
**                                 to the server
**
**
**  IMPLICIT INPUTS:
**
**  IMPLICIT OUTPUTS:
**
**  FUNCTION VALUE:
**
**      return          an integer specifying success or some type of failure
**
**  SIDE EFFECTS:       none
**
**--
**/


comtst_svr_use_all_protseqs_if (argc, argv, status)

int argc;
char *argv[];
unsigned32  *status;

{
    rpc_binding_vector_p_t  bind_v;
    unsigned_char_p_t       stringb;
    unsigned16              index;
    boolean32               listen;
    int        x_argc;
    char       **x_argv;


    CHECK_ARGS(SERVER_USE_ALL_PROTSEQS_ARGS);

    PRINT_ARGS;

    if (verbose != 0) fprintf(stdout, "%s - ENTER\n", argv[0]);


    x_argc = --argc;
    x_argv = ++argv;

    if ((strcmp (*x_argv, SERVER_RPC_WAIT)) == 0)
    {
        listen = COMTST_server_wait;
        --x_argc; ++x_argv;
    }
    else
    {
        listen = COMTST_server_return;
    }

    comtst_svr_update_endpoint_vect ( x_argc, x_argv,
                         comtest_server_v2_0_s_ifspec, status );
    
    CHECK_STATUS;
    
    rpc_server_use_all_protseqs_if (COMTST_max_calls,
        comtest_server_v2_0_s_ifspec, status);

    if (*status != in_status)
    {
        return RPC_CALL_FAILED;
    }

    comtst_svr_get_binding_strings ( &bind_v, status );
    CHECK_STATUS;

    rpc_server_register_if (comtest_server_v2_0_s_ifspec, NULL,
                            (rpc_mgr_epv_t) &comtest_server_epv,
                            status);

    if (*status != in_status)
    {
        return RPC_CALL_FAILED;
    }

    comtst_svr_wait ( listen, status );

    if (*status != in_status)
    {
        return (COMTEST_FAILED);
    }
    return (COMTEST_OK);
}


/*
**++
**
**  ROUTINE NAME:       comtst_svr_use_protseq
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**  with a specified protocol sequence string,
**  test rpc_server_use_protseq after which handles
**  to the resulting bindings are acquired and converted
**  to string bindings. these bindings are reported and output
**  to bindings to a tmp file, COMTST_protseq_tmpfile. Optionally
**  the process may enter a wait state and wait for expected RPCs
**  from a client which is assumed to be invoked by the 'tsh'.
**
**
**  INPUTS:
**
**
**      argc            argument count
**      argv            array of pointers to character
**                      string input arguments
**
**  The calling syntax:
**
**            protseq   where 'protseq' is any valid protocol sequence
**                      string currently supported on the local host
**
**         server_wait  keyword supplied as input if we
**                      intend to call rpc_server_listen
**                      and wait for response from a client
**
**
**  INPUTS/OUTPUTS:
**
**
**  OUTPUTS:
**
**
**       COMTST_protseqs_tmpfile - a list of protocol sequence strings
**                                 supported on the local host
**
**       COMTST_pid_tmpfile -      a contains the pid of the server process
**                                 available to the client to send signals
**                                 to the server
**
**
**
**  IMPLICIT INPUTS:
**
**  IMPLICIT OUTPUTS:
**
**  FUNCTION VALUE:
**
**      return          an integer specifying success or some type of failure
**
**  SIDE EFFECTS:       none
**
**--
**/

comtst_svr_use_protseq (argc, argv, status)
int argc;
char *argv[];
unsigned32  *status;
{
    rpc_binding_vector_p_t  bind_v;
    unsigned_char_p_t       stringb;
    char                   *protseq;
    unsigned16             index;
    boolean32                listen;
    int        x_argc;
    char       **x_argv;

    
    
    CHECK_ARGS(SERVER_USE_PROTSEQ_ARGS);
    
    PRINT_ARGS;
    
    if (verbose != 0) fprintf(stdout, "%s - ENTER\n", argv[0]);    

    x_argc = --argc;
    x_argv = ++argv;

    if ((strcmp (* x_argv, SERVER_RPC_WAIT)) == 0)
    {
        listen = COMTST_server_wait;
        --x_argc; ++x_argv;
    }
    else
    {
        listen = COMTST_server_return;
    }

    comtst_svr_protseq_supported ( x_argv,  status );
    CHECK_STATUS;
    
    rpc_server_use_protseq (*x_argv, COMTST_max_calls,  status);
    if (*status != in_status)
    {
        return RPC_CALL_FAILED;
    }
    if (*status != rpc_s_ok)
    {
        return (COMTEST_OK);
    }

    comtst_svr_get_binding_strings ( &bind_v, status );
    CHECK_STATUS;

    rpc_server_register_if (comtest_server_v2_0_s_ifspec, NULL,
                            (rpc_mgr_epv_t) &comtest_server_epv,
                            status);
    if (*status != in_status)
    {
        return RPC_CALL_FAILED;
    }
    
    comtst_svr_wait ( listen, status );
    
    return (COMTEST_OK);
}


/*
**++
**
**  ROUTINE NAME:       comtst_svr_use_protseq_if
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**  Test rpc_server_use_protseq_if then proceed to
**  acquire the bindings and convert to string bindings
**  report bindings found and output bindings to tmp
**  file 
**
**
**  INPUTS:
**
**
**      argc            argument count
**      argv            array of pointers to character
**                      string input arguments
**
**  The calling syntax:
**
**
**            protseq   where 'protseq' is any valid protocol sequence
**                      string currently supported on the local host
**
**         server_wait  keyword supplied as input if we
**                      intend to call rpc_server_listen
**                      and wait for response from a client
**
**
**  INPUTS/OUTPUTS:      none
**
**
**  OUTPUTS:
**
**
**       COMTST_protseqs_tmpfile - a list of protocol sequence strings
**                                 supported on the local host
**
**       COMTST_pid_tmpfile -      a contains the pid of the server process
**                                 available to the client to send signals
**                                 to the server
**
**
**
**  IMPLICIT INPUTS:
**
**  IMPLICIT OUTPUTS:
**
**  FUNCTION VALUE:
**
**      return          an integer specifying success or some type of failure
**
**  SIDE EFFECTS:
**
**--
**/

comtst_svr_use_protseq_if (argc, argv, status)
int argc;
char *argv[];
unsigned32  *status;
{
    rpc_if_handle_t         ifspec;
    rpc_binding_vector_p_t  bind_v;
    unsigned_char_p_t       stringb;
    unsigned16              index;
    boolean32               listen;
    int        x_argc;
    char       **x_argv;


    CHECK_ARGS(SERVER_USE_PROTSEQ_IF_ARGS);

    PRINT_ARGS;

    if (verbose != 0) fprintf(stdout, "%s - ENTER\n", argv[0]);

    x_argc = --argc;
    x_argv = ++argv;

    if ((strcmp (* x_argv, SERVER_RPC_WAIT)) == 0)
    {
        listen = COMTST_server_wait;
        --x_argc; ++x_argv;
    }
    else
    {
        listen = COMTST_server_return;
    }

    comtst_svr_protseq_supported ( x_argv,  status );
    CHECK_STATUS;
    
    rpc_server_use_protseq_if (*x_argv, COMTST_max_calls,
                          comtest_server_v2_0_s_ifspec, status);

    if (*status != in_status)
    {
        return RPC_CALL_FAILED;
    }
    if (*status != rpc_s_ok)
    {
        return (COMTEST_OK);
    }

    comtst_svr_get_binding_strings ( &bind_v, status );
    CHECK_STATUS;
    
    rpc_server_register_if (comtest_server_v2_0_s_ifspec, NULL,
                            (rpc_mgr_epv_t) &comtest_server_epv,
                            status);

    if (*status != in_status)
    {
        return RPC_CALL_FAILED;
    }
    
    comtst_svr_wait ( listen, status );
    
    return (COMTEST_OK);

}


/*
**++
**
**  ROUTINE NAME:       comtst_svr_use_protseq_ep
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**  with a specified protocol sequence string,
**  test rpc_server_use_protseq_ep after which handles
**  to the resulting bindings are acquired and converted
**  to string bindings. these bindings are reported and output
**  to bindings to a tmp file, COMTST_protseq_tmpfile. Optionally
**  the process may enter a wait state and wait for expected RPCs
**  from a client which is assumed to be invoked by the 'tsh'.
**
**  INPUTS:
**
**
**      argc               argument count
**      argv               array of pointers to character
**                         string input arguments
**
**  The calling syntax:
**
**
**      protseq endpoint   where 'protseq' is any valid protocol sequence
**                         string currently supported on the local host
**                         and 'endpoint' is any endpoint string valid with
**                         the proceeding protocol sequence string.
**
**
**         server_wait      keyword supplied as input if we
**                          intend to call rpc_server_listen
**                          and wait for response from a client
**
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:
**
**
**       COMTST_protseqs_tmpfile - a list of protocol sequence strings
**                                 supported on the local host
**
**       COMTST_pid_tmpfile -      a contains the pid of the server process
**                                 available to the client to send signals
**                                 to the server
**
**  IMPLICIT INPUTS:
**
**  IMPLICIT OUTPUTS:
**
**  FUNCTION VALUE:
**
**      return          an integer specifying success or some type of failure
**
**  SIDE EFFECTS:
**
**--
**/

comtst_svr_use_protseq_ep (argc, argv, status)
int argc;
char *argv[];
unsigned32  *status;
{
    rpc_if_handle_t         ifspec;
    rpc_binding_vector_p_t  bind_v;
    unsigned_char_p_t       stringb;
    unsigned16              index;
    boolean32               listen;
    int        x_argc;
    char       **x_argv;



    CHECK_ARGS(SERVER_USE_PROTSEQ_EP_ARGS);

    PRINT_ARGS;

    if (verbose != 0) fprintf(stdout, "%s - ENTER\n", argv[0]);

    x_argc = --argc;
    x_argv = ++argv;

    if ((strcmp (* x_argv, SERVER_RPC_WAIT)) == 0)
    {
        listen = COMTST_server_wait;
        --x_argc; ++x_argv;
    }
    else
    {
        listen = COMTST_server_return;
    }

    comtst_svr_protseq_supported ( x_argv,  status );
    CHECK_STATUS;
 
    rpc_server_use_protseq_ep (*x_argv, COMTST_max_calls,
                                             x_argv[1], status);

    if (*status != in_status)
    {
        return RPC_CALL_FAILED;
    }
    if (*status != rpc_s_ok)
    {
        return (COMTEST_OK);
    }

    comtst_svr_get_binding_strings ( &bind_v, status );
    CHECK_STATUS;
    
    rpc_server_register_if (comtest_server_v2_0_s_ifspec, NULL,
                            (rpc_mgr_epv_t) &comtest_server_epv,
                            status);

    if (*status != in_status)
    {
        return RPC_CALL_FAILED;
    }
    
    comtst_svr_wait ( listen, status );
    
    return (COMTEST_OK);

}


/*
**++
**
**  ROUTINE NAME:       comtst_svr_update_endpoint_vect
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**  allocate memory for an endpoint vector which is initialized
**  by the protocol sequences supplied on input, then attach
**  this endpoint vector to the interface specification whose
**  address is supplied as input. 
**
**  INPUTS:
**
**      argc            argument count
**      argv            array of pointers to character
**                      string input arguments
**
**
**  The calling syntax:
**
**            protseq  [protseq]...
**                      where each 'protseq' is any valid
**                      protocol sequence string. a series
**                      of these strings may be specified
**
**
**  INPUTS/OUTPUTS:
**
**
**              ifspec  address of an interface specification
**                      to which the initialized endpoint vector
**                      will be attached
**
**  OUTPUTS:
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      return          an integer specifying success or some type of failure
**
**  SIDE EFFECTS:       none
**
**--
**/

comtst_svr_update_endpoint_vect ( argc, argv, if_spec, status )
int                     argc;
char                    *argv[];
rpc_if_handle_t         if_spec;
unsigned32              *status;

{
    unsigned32               i, j, endps;
    rpc_protseq_vector_p_t          psvp;
    rpc_if_rep_p_t                ifspec;
    rpc_endpoint_vector_elt_p_t new_epvs;
    int        x_argc;
    char       **x_argv;
    
    
    
    x_argc = argc;
    x_argv = argv;
    
    for (endps = 0;  x_argc; )
    {
        comtst_svr_protseq_supported ( x_argv,  status );
        CHECK_STATUS;
        endps++;
        x_argc -= 2; x_argv += 2;
    }
    
    if (endps == (unsigned32) NULL)
    {
        return (COMTEST_FAILED);
    }

    (new_epvs) = (rpc_endpoint_vector_elt_p_t)malloc((sizeof
                                      (rpc_endpoint_vector_elt_t)
                                              * (endps +1)));

    if (new_epvs == NULL)
    {
        return (COMTEST_FAILED);
    }
    x_argc = argc;
    x_argv = argv;
    
    for (i = 0;  x_argc; i++)
    {
        new_epvs[i].rpc_protseq = (unsigned_char_p_t) *x_argv++;
        new_epvs[i].endpoint = (unsigned_char_p_t) *x_argv++;
        x_argc -= 2;
    }
    ifspec = (rpc_if_rep_p_t)if_spec;
    ifspec->endpoint_vector.count = endps;
    ifspec->endpoint_vector.endpoint_vector_elt = new_epvs;
    return (COMTEST_OK);
}

/*
**++
**
**  Routine NAME:       comtst_svr_protseq_supported
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**  get a vector of all the protocol sequences currently
**  supported on the local host then with the protocol
**  sequence string supplied as argv input search the
**  vector list to detemin whether it is valid
**  
**
**  INPUTS:             none [or...]
**
**      argv            array of pointers to character
**                      string input arguments
**
**  The calling syntax:
**
**            protseq   where 'protseq' is any valid protocol sequence
**                      string currently supported on the local host
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:
**                      status  indicates whether the specified
**                              protocol sequence is supported
**
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**      return          an integer specifying success or some type of failure
**
**  SIDE EFFECTS:       none
**
**--
**/

comtst_svr_protseq_supported ( argv, status )

char         **argv;
unsigned32   *status;

{
    unsigned32    i;
    char pseqbuf[BUFSIZ];    


    
    if ( !COMTST_svr_supported_protseqs)
    {
        rpc_network_inq_protseqs ( &COMTST_server_psvp,  status );
        CHECK_STATUS;
        COMTST_svr_supported_protseqs++;
    }
    
    for (i = 0; i < COMTST_server_psvp->count; i++)
    {
        if (verbose != 0) 
        {
            fprintf (stdout, "(%d) - %s\n",(i + 1),
                     COMTST_server_psvp->protseq[i]);
        }
        if (!strcmp(COMTST_server_psvp->protseq[i], *argv))
        {
            if (verbose !=0) fprintf (stdout, "(%d) - %s\n",i,
                     *argv);
            status = rpc_s_ok;      
            return (COMTEST_OK);
        }
    }
    *status = rpc_s_protseq_not_supported;          
    return (COMTEST_FAILED);
}

/*
**++
**
**  Routine NAME:       comtst_svr_get_binding_strings
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**   allocate a vector of binding strings currently available
**   under the run time  return a vector of these strings also
**   output a list of these binding strings to tmp file
**
**  INPUTS:             none
**
**
**  INPUTS/OUTPUTS:     none
**
**                      bind_v    the address that will be returned
**                                of a binding vector
**
**  OUTPUTS:
**
**
**       COMTST_protseqs_tmpfile - a list of protocol sequence strings
**                                 supported on the local host
**
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**      return          an integer specifying success or some type of failure
**
**  SIDE EFFECTS:       none
**
**--
**/

comtst_svr_get_binding_strings ( bind_v, status )

rpc_binding_vector_p_t  bind_v;
unsigned32   *status;

{
    FILE *protseqfil;
    FILE *pidfil;
    unsigned_char_p_t stringb;
    unsigned16        bv_index;
    unsigned32        i;
    
    
    
    rpc_server_inq_bindings (&bind_v, status);
    
    if (*status != in_status)
    {
        return (COMTEST_FAILED);
    }

    comtst_svr_open_outfile (&protseqfil, &pidfil, status);
    CHECK_STATUS;
    
    fprintf(pidfil, "%d\n", getpid());
    fclose (pidfil);

    for ( bv_index = 0; bv_index < bind_v->count; bv_index++)
    {
        
        rpc_binding_to_string_binding (bind_v->binding_h[bv_index],
                                       &stringb, status);
        if (*status != in_status)
        {
            return (COMTEST_FAILED);
        }
        
        if (verbose !=0) fprintf (stdout, "%s\n", stringb);

        fprintf(protseqfil, "%s\n", stringb);
        
        rpc_string_free (&stringb, status);
        
        if (*status != in_status)
        {
            return (COMTEST_FAILED);
        }
        
    }
    fclose (protseqfil);
    return (COMTEST_OK);    
}

/*
**++
**
**  Routine NAME:       comtst_svr_parse_command
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**  parse the input command strings in the form of argv argc
**  arguments and produce a linked list of comtest_command_t
**  
**
**  
**
**  INPUTS:             none
**
**      argc            argument count
**      argv            array of pointers to character
**                      string input arguments
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**          global pointer comm  a global pointer to a linked list
**                               of comtest_command_t packets each
**                               containing: type uuid, object uuid,
**                               epv, and interface specification
**                               built from the input command string
**
**  SIDE EFFECTS:       none
**
**--
**/

comtest_command_p_t
comtst_svr_parse_command ( argc, argv, status )

int     argc;
char    *argv[];
unsigned32   *status;

{
    unsigned_char_t       *uuid_string;
    unsigned32            i;
    unsigned32            epv;
    unsigned16            inparin = 0;
    unsigned16            command_allo = 0;
    comtest_command_p_t   comm = 0;
    comtest_command_p_t   l_comm;
    int                   x_argc;
    char                  **x_argv;
    
    
    
    l_comm = comm = (comtest_command_p_t)
        malloc (sizeof(comtest_command_t));
    comm->next = (struct comtest_command_t *)NULL;
    l_comm->interface = interface_X_v2_0_s_ifspec;
    l_comm->if_desig  = 'X';
    l_comm->epv  =  (rpc_mgr_epv_t * ) interfaceX_epv_list[0];
    command_allo++;
    
    for (x_argc = --argc,x_argv = argv, i = 1; x_argc;)
    {
        if ( **x_argv == RPARN )
        {
            --x_argc; ++x_argv;
            i = 1;
            if (l_comm->if_desig == 'Y')
            {
                l_comm->interface = interface_Y_v2_0_s_ifspec;
                l_comm->epv  =  (rpc_mgr_epv_t * ) interfaceY_epv_list[epv];
            }
            else if (l_comm->if_desig == 'X')
            {
                l_comm->interface = interface_X_v2_0_s_ifspec;
                l_comm->epv  =  (rpc_mgr_epv_t * ) interfaceX_epv_list[epv];
            }
            inparin = 0;
            command_allo = 0;
            continue;
        }
        if ( **x_argv == LPARN || inparin )
        {
            if (!command_allo)
            {
                l_comm->next = (struct comtest_command_t *)
                    malloc (sizeof(comtest_command_t));
                l_comm = (comtest_command_p_t)l_comm->next;
                l_comm->next = (struct comtest_command_t *)NULL;
                
                l_comm->interface = interface_X_v2_0_s_ifspec;
                l_comm->if_desig  = 'X';
                l_comm->epv  =  (rpc_mgr_epv_t * ) interfaceX_epv_list[0];
                command_allo++;
                --x_argc;
            }
            
            inparin++;
            
            switch (i)
            {
                case 1:
                uuid_string = (unsigned_char_t *)*++x_argv;
                if ((strcmp (uuid_string, NIL_UUID)) == 0)
                {
                    uuid_create_nil (&(l_comm->uuid_type), status);
                    if (*status != rpc_s_ok) printf("failed\n");
                } else
                {
                    uuid_from_string (uuid_string, &(l_comm->uuid_type), status);
                    if (*status != rpc_s_ok) printf("failed\n");
                }
                break;
                
                case 2:
                uuid_string = (unsigned_char_t *)*x_argv;
                if ((strcmp (uuid_string, NIL_UUID)) == 0)
                {
                    uuid_create_nil (&(l_comm->uuid_obj), status);
                    if (*status != rpc_s_ok) printf("failed\n");
                } else
                {
                    uuid_from_string (uuid_string, &(l_comm->uuid_obj), status);
                    if (*status != rpc_s_ok) printf("failed\n");
                }
                break;

                case 3:
                epv = ( atoi (*x_argv) -1 );
                break;

                case 4:
                l_comm->if_desig  = **x_argv;
                break;
            }
            
            --x_argc; ++x_argv; i++;
            continue;
        }

        if ((strcmp (*x_argv, SERVER_RPC_WAIT)) == 0)
        {
            --x_argc; ++x_argv;
            continue;
        }
        
        if ((strcmp (*x_argv, REGISTER_DEFAULT_EPV)) == 0)
        {
            --x_argc; ++x_argv;
            continue;
        }

        if ((strcmp (*x_argv, REGISTER_GENERATED_EPV)) == 0)
        {
            --x_argc; ++x_argv;
            continue;
        }

        if ((strcmp (*x_argv, BOGUS_OBJ_UUID)) == 0)
        {
            --x_argc; ++x_argv;
            continue;

        }

        if ((strcmp (*x_argv, "null_test")) == 0)
        {
            --x_argc; ++x_argv;
            continue;

        }

        
    }
    return ((comtest_command_p_t)comm);
}


/*
**++
**
**  Routine NAME:       comtst_svr_wait
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**    called by a proocess which has set the boolean flag
**    'listen' to the desired state.  if listen is clear
**    return immediately to the calling process.  if listen
**    is set call rpc_server_listen, from which we never
**    return.  we go into a state waiting to receive RPCs
**    from a client process.  it is assumed that the calling
**    process has properly registered an interface spec and
**    objects for which it wishes to receive RPCs
**
**    the process also sets up signal handeling to catch
**    signals before entering the listen state.
**
**  INPUTS:
**
**        listen  a boolean flag set by the calling
**                process to  COMTST_server_wait if
**                the caller wishes to call rpc_server_listen
**                otherwise it should by set to
**                COMTST_server_return to return to
**                the calling process
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**      return          an integer specifying success or some type of failure
**                      this is only returned of the listen flag is clear
**
**  SIDE EFFECTS:
**
**   NOTE: if the 'listen' indicates we want to wait
**   there is no return from this routine
**        
**
**--
**/


comtst_svr_wait ( listen, status )
unsigned32             *status;
boolean32               listen;
{
    unsigned32 i;
    unsigned32 omask;
    
    
    if ( listen >= COMTST_server_return )
    {
        *status = in_status;
        return (COMTEST_OK);
    }

    /*
     * catch signals
     */
/*
    comtst_sigvec.sa_handler = comtst_svr_signal_dispatch;
    sigaction (SIGINT,  &comtst_sigvec, NULL);
    sigaction (SIGQUIT, &comtst_sigvec, NULL);
    sigaction (SIGTERM, &comtst_sigvec, NULL);
    sigaction (SIGALRM, &comtst_sigvec, NULL);
*/
#if defined(SNI_SVR4_POSIX)
    sigset (SIGINT, (void *) comtst_svr_signal_dispatch);
    sigset (SIGQUIT, (void *) comtst_svr_signal_dispatch);
    sigset (SIGTERM, (void *) comtst_svr_signal_dispatch);
    sigset (SIGALRM, (void *) comtst_svr_signal_dispatch);

	/*
	*	Explicitly unblock SIGALRM so that if alarm() 
	*	generates it, then the process will actually
	*	receive it.
	*/

	{
	sigset_t	sigset;

	if ( sigemptyset( &sigset ) != 0 )
		{
		perror("sigemptyset");
		exit(1);
		}
	if ( sigaddset( &sigset, SIGALRM ) != 0 )
		{
		perror("sigaddset");
		exit(1);
		}
	if ( sigprocmask( SIG_UNBLOCK, &sigset, (sigset_t *)NULL ) != 0 )
		{
		perror("sigprocmask");
		exit(1);
		}
	}

#else
    signal (SIGINT, (void *) comtst_svr_signal_dispatch);
    signal (SIGQUIT, (void *) comtst_svr_signal_dispatch);
    signal (SIGTERM, (void *) comtst_svr_signal_dispatch);
    signal (SIGALRM, (void *) comtst_svr_signal_dispatch);
#endif

    /*
     * set a timeout in case no client appears
     */
    alarm (COMTST_timeout_wait);

    rpc_server_listen ( COMTST_max_calls,
                       status );
}
 


/*
**++
**
**  Routine NAME:       comtst_svr_xfr
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**  server side manager routine which will
**  receive RPC calls from a client and report results
**  this is a generic manager routine for all the
**  interfaces  results are sent to the server process
**  stanadard out
**
**  it is from this routine that the server process exits
**  itself upon receiving a SIGINT signal from the client
**  process. in order for this to occur the client must
**  have sent an RPC with the 'imsg' argument set to
**  COMTST_client_nomore_tsts.
**
**  INPUTS
**
**              h   a binding handle over whih the
**                  RPC arrived
**
**            imsg  is set by the calling client to
**                  COMTST_client_more_tsts for each
**                  test RPC.  the caller must make
**                  a final call with this parameter
**                  set to COMTST_client_nomore_tsts
**                  to signal the server that testing
**                  is complete and that the server
**                  process may exit itself upon receiving
**                  a SIGINT signal.
**
**                  the client RPC call
**              sp  any character string which the
**                  calling client wishes to the server
**                  and be echoed to the standard output
**
**
**  INPUTS/OUTPUTS:
**
**
**  OUTPUTS:
**
**            omsg  any value in the form of an
**                  unsigned long integer passed from
**                  the client RPC call
**
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**      return          an integer specifying success or some type of failure
**
**  SIDE EFFECTS:
**
**      NOTE: unless this process receives an RPC with arg
**            'imsg' set to COMTST_client_nomore_tsts the server
**            will set around inside the rpc_server_listen loop
**            and have to be manually killed
**
**--
**/

void comtst_svr_xfr (h, imsg, sp, omsg, status)

handle_t                h;
unsigned long           imsg;
cstring_t               *sp;
unsigned long           *omsg;
unsigned32              *status;

{

    if (imsg == COMTST_client_more_tsts)
    {
        fprintf(stdout,
                "%s\n",sp);
        COMTST_test_complete = 0;
        *omsg = 1;
    }
    else if (imsg == COMTST_client_nomore_tsts)
    {
        fprintf(stdout,
                "%s\n\n",sp);
        COMTST_test_complete = 1;
        *omsg = 0;
    }
    *status = rpc_s_ok;
}


/*
**++
**
**  Routine NAME:       comtst_svr_open_outfile
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**   open temporary files COMTST_protseqs_tmpfile, and
**   COMTST_pid_tmpfile on the local host and return
**   file descriptors for these files
**
**  INPUTS:             none
**
**
**  INPUTS/OUTPUTS:     none
**
**
**        fp   address of file descriptor for
**             COMTST_protseqs_tmpfile
**
**        pp   address of file descriptor for
**             COMTST_pid_tmpfile
**
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**      return          an integer specifying success or some type of failure
**
**  SIDE EFFECTS:       none
**
**--
**/


comtst_svr_open_outfile (fp, pp, status)

FILE           **fp;
FILE           **pp;
unsigned32     *status;

{
    FILE *protseqfil;


    if ((*pp = fopen (COMTST_pid_tmpfile, "w+")) == NULL)
    {
        return (COMTEST_FAILED);
    }

    if ((*fp = fopen (COMTST_protseqs_tmpfile, "w+")) == NULL)
    {
        return (COMTEST_FAILED);
    }
    return (COMTEST_OK);
}


/*
**++
**
**  Routine NAME:       comtst_svr_signal_dispatch
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**   signal dispatcher - report the reception of
**                       signals
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none 
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      return          an integer specifying success or some type of failure
**
**  SIDE EFFECTS:       none
**
**--
**/


int comtst_svr_signal_dispatch ()

{
    unsigned32     status;
    
    /*
     * The code path followed when verbose is true is not the norm for 
     * automated regression tests. We don't need all this diagnostic 
     * output for these tests, and there are better places to do i/o 
     * than in an interrupt handler.
     */
    if (verbose) 
    {
        fprintf(stdout, "comtst_svr_signal_dispatch - SIGNAL");    
        /*
         * empty all system buffers
         */
        fflush (NULL);
    }

    if(COMTST_test_complete)
    {
        if (verbose)
            fprintf(stdout, 
                "\n\n===============================================\n\n");    

        rpc_mgmt_stop_server_listening (NULL, &status);
        
        if ((status != rpc_s_ok) && verbose)
        {
            fprintf(stdout,
                "rpc_mgmt_stop_server_listening failed\n\n");
        }

        /*
         * empty all system buffers
         */
        if (verbose) fflush (NULL);
    }
    return (0);
}

/*
**++
**
**  Routine NAME:       comtest_cat_id
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**   concatinate character string s2 onto string
**   s1, leave the resultant string in global
**   buffer
**
**  INPUTS:             none
**
**      s1     address of character string
**
**      s2     address of character string
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      return          a pointer to the character string
**
**  SIDE EFFECTS:       none
**
**--
**/

sstring_t  *comtest_cat_id (s1, s2)

sstring_t               *s1;
sstring_t               *s2;

{
    string_buf[0] = '\0';
    strcat (string_buf, *s1);
    strcat (string_buf, *s2);
    return ((sstring_t *) string_buf);
}
