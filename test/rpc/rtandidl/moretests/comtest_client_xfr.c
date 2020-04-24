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
 * $Log: comtest_client_xfr.c,v $
 * Revision 1.1.12.2  1996/02/17  23:20:21  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:01  marty]
 *
 * Revision 1.1.12.1  1995/12/11  20:03:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:42:59  root]
 * 
 * Revision 1.1.9.2  1993/08/30  17:25:37  tom
 * 	Bug 8516 - Fix compile errors on RIOS.
 * 	[1993/08/30  17:25:16  tom]
 * 
 * Revision 1.1.7.2  1993/06/24  20:43:00  hinman
 * 	[hinman] - Make typedef available to all
 * 	[1993/06/22  11:50:11  hinman]
 * 
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:41:35  hinman]
 * 
 * Revision 1.1.5.3  1993/01/11  22:12:34  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:04:37  bbelch]
 * 
 * Revision 1.1.5.2  1992/09/29  21:22:35  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:47:43  weisman]
 * 
 * Revision 1.1.2.2  1992/07/15  01:22:18  smith
 * 	xlc compiler strictness required changes for compile.
 * 	[1992/07/15  01:20:40  smith]
 * 
 * Revision 1.1  1992/01/19  03:38:32  devrcs
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
**      comtest_client_xfr.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**  
**  This module contains two test routines which
**  provide client side support for the "server
**  object" test group found in server_obj_tsts.c.
**  Found here are comtest_client_xfr and
**  comtest_object_xfr.  Both these routines are
**  called from the top level routine, testdriver
**  which is passed command line args in the "argv,
**  argc" format by the 'tsh'.  It is assumed this
**  client side will be started up as a background
**  process by the 'tsh' just prior to inoking the
**  server side, 'tstsrvr'.  These client side 
**  routines wait a fixed period of time before they
**  expect to see test information conveyed to them
**  by the server side in the form of three files:
**  COMTST_pid_tmpfile the process id of the server,
**  COMTST_protseq_tmpfile, contains a list of 
**  protocol sequences over which RPCs are to be
**  exercised.  The routine, comtest_object_xfr, also
**  expects to see a file, COMTST_obj_tmpfile,
**  containing a list of object UUIDs to which RPCs
**  are to be sent.  Together with these files both
**  routines themselves create a COMTST_client_lockfile,
**  to insure that only one client process is currently
**  running as a background process.
**  
**  These client routines both wait for a default
**  period of 2 seconds to allow the server process to
**  start up and create these files.  This time may
**  be varied, (it must be increased on slower processors),
**  with the '-w' option.
**
**

**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**  14-aug-91 bmartin   Remove RPC_VERIFY_INIT call.  There are not private 
**                      calls here.
**  25-jul-91 bmartin   Correct type of object_ptr in comtest_object_xfr().
**  22-jul-91 bmartin   do not fork if building for vms;
**                      rid of pid_t type - unix only;
**                      use remove() instead of unlink();
**                      change comtest_server_xxx functions to comtst_svr_xxx.
**  23-apr-91 woodbury  de-TABBED
**  05-mar-91 burton    fixed bug in comtest_client_create_lock
**  11-feb-91 burton    fixed parser more than one interface
**  09-feb-91 burton    extraifs became interfaceX and interfaceY
**  07-feb-91 burton    add comtest_object_xfr
**  12-jan-91 burton    fork() child process and return to tsh
**  12-jan-91 burton    continuous testing until last server test
**  07-jan-91 burton    add communication of pid to client
**  17-dec-90 burton    original
**
**  %a%private_end  
**
*/

/*#include <sysconf.h>  */            /* System definitions for RPC Runtime system */
#ifdef SNI_SVR4
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#include <sys/errno.h>
#include <dce/rpc.h>              /* Public definitions for RPC Runtime system */
#include <comtest_client_xfr.h>   /* Externals exported by this module */

typedef void (*vararg_fn_t)(handle_t, ... );

extern int errno;

/*
**++
**
**  ROUTINE NAME:       comtest_client_xfr
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
**  Client side for IDL default, segment of the server
**  object test group.
**  RPC routines exercised:
**
**  uuid_create_nil
**  uuid_from_string
**  rpc_binding_from_string_binding
**  rpc_binding_set_object    
**  rpc_binding_free
**
**  First, comtest_client_create_lock is called to create
**  lockfile, COMTST_client_lockfile to insure that only
**  one cient process is runnig simultaneously.  Next the
**  parent process, which is assumed to have been invoked
**  from the 'tsh', forks a child copy of itself so that
**  the parent may return immediately to the 'tsh' and 
**  then server side tests can begin to be invoked. This
**  forking may be overridden by the '-d' option.
**  Next, comtest_client_open_environ is called to open the
**  input files, COMTST_pid_tmpfile, and COMTST_protseqs_tmpfile.
**  By default there is a wait of 2 seconds to provide sufficient
**  time for the server side to create these files.  This wait
**  time may be varied with the '-w wait_time' option.
**  It will have to be adjusted according to the speed of
**  the host processor. If these files are successfully opend
**  We then begin reading from the COMTST_protseqs_tmpfile, a list
**  of protocol sequence strings.  For each protocol sequence
**  which the server has indicated to be supported on the local
**  processor, we attempt create a binding handle and send a
**  RPC.  After sending a series of RPCs, one final RPC is sent
**  indicating to the server that this particular test is
**  complete.  Then a SIGINT is sent to force the server to exit
**  itself and the 'tsh' may subsequently begin another server
**  object test.
**
**  It isn't necessary to reinvoke this client side if another
**  test is to begin immediately.  It will wait for the period 
**  indicated to try to reopen the input files.
**
**      
**
**  INPUTS:             
**
**      argc            argument count
**      argv            array of pointers
**                      to string input arguments
**
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
**  INPUTS/OUTPUTS:     none
**
**      
**
**  OUTPUTS:
**
**       COMTST_client_lockfile    a sentinel file to insure only one
**                                 client is running
**
**      status                     an rpc status code
**
**  IMPLICIT INPUTS:    none 
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     int
**
**      return          an integer specifying success or
**                      some type of failure
**                      
**
**  SIDE EFFECTS:       none 
**
**--
**/ 

extern int errno;

comtest_client_xfr (argc, argv, status)

unsigned32              argc;
char                    *argv[];
unsigned32              *status;

{
    FILE                    *prottf;
    FILE                    *pidtf;
    FILE                    *objtf;
    rpc_binding_handle_t    binding_h;
    unsigned   long         omsg;
    unsigned   long         spid;
    unsigned32              j;
    int                     child;
    char                    *cp;

    unsigned32  debug =     (unsigned32) NULL;   
    unsigned32  wait_time = COMTST_default_wait;
    
    
    
    CHECK_ARGS(COMTEST_SERVER_XFR_ARGS);
    PRINT_ARGS;

    /*
     * search through the input options
     */
    argc--, argv++;
    while (argc > 0 && **argv == '-') 
    {
        for (cp = *argv + 1; *cp; cp++)
        {
            switch (*cp) 
            {
                /*
                 * debug mode - don't fork
                 */
                case 'd':
                debug = 1;
                break;  
                /*
                 * time to wait
                 */
                case 'w':
                wait_time = atoi(*argv);
                argc--, argv++;
                break;  
            }
        }
        argc--, argv++;
    }

    /*
     * try to create a lock file if not already locked
     */
    comtest_client_create_lock (status);
    CHECK_STATUS;

#ifndef vms

    /*
     * if not debug mode fork and return to tsh
     */
    if (!debug)
    {
        if ((child = fork()) != (unsigned32) NULL)
        {
            if (child == -1)
            {
                fprintf (stdout,
                         "comtest_client_xfr: - fork() call failed\n");
                return(COMTEST_FAILED);
            }
            else
            {
                *status = rpc_s_ok;
                return(COMTEST_OK);
            }
        }
    }

#endif

    /*
     *  try to open the input file - after 'wait_time'
     */
    while ((comtest_client_open_environ
            (&prottf, &pidtf, &objtf, wait_time)) == 3)
    {  
        /*
         * get the process id file from the server
         */
        if (j = fscanf (pidtf, "%s", COMTST_protseq_buf) == EOF)
            return(COMTEST_FAILED);
        spid = atoi(COMTST_protseq_buf);
        fclose (pidtf);
        
        /*
         * get each protocol sequence from input file
         * try to send an RPC on each protocol sequence
         */
        while (j = fscanf (prottf, "%s", COMTST_protseq_buf) != EOF)
        {
            fprintf (stdout, "%s\n", COMTST_protseq_buf);
            
            rpc_binding_from_string_binding ( 
                                             COMTST_protseq_buf,
                                             &binding_h, 
                                             status);
            CHECK_STATUS;
            
            rpc_binding_set_object (binding_h,
                                    &COMTST_obj_id,
                                    status);
            CHECK_STATUS;

            comtst_svr_xfr (binding_h,
                                COMTST_client_more_tsts,
                                (cstring_t *) COMTST_protseq_buf,
                                &omsg,
                                status);

            if (*status != rpc_s_ok)
            {
                return RPC_CALL_FAILED;
            }
            
            rpc_binding_free (&binding_h, status);
            CHECK_STATUS;
            
        }    
        fclose (prottf);

        /*
         * send last RPC telling server test complete
         */     
        rpc_binding_from_string_binding ( 
                                         COMTST_protseq_buf,
                                         &binding_h, 
                                         status);
        CHECK_STATUS;
        
        rpc_binding_set_object (binding_h,
                                &COMTST_obj_id,
                                status);
        CHECK_STATUS;
        
        comtst_svr_xfr (binding_h,
                            COMTST_client_nomore_tsts,
                            (cstring_t *)"Test Complete",
                            &omsg,
                            status);

        if (*status != rpc_s_ok)
        {
            return RPC_CALL_FAILED;
        }
        
        /*
         * test that we are able to free all the bindings
         */

        rpc_binding_free (&binding_h, status);
        CHECK_STATUS;

        /*
         * send signal to server - its ok to go
         * on to next test
         */
        kill (spid, SIGINT);

        /*
         * remove input files
         */
        remove (COMTST_pid_tmpfile);
        remove (COMTST_protseqs_tmpfile);
    }
    remove (COMTST_client_lockfile);
    if (debug) return(COMTEST_OK);
    exit (0);
}

/*
**++
**
**  ROUTINE NAME:       comtest_object_xfr
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
**  Client side for IDL generated, segment of the server
**  object test group.
**  RPC routines exercised:
**
**  uuid_create_nil
**  uuid_from_string
**  rpc_binding_from_string_binding
**  rpc_binding_set_object    
**  rpc_binding_free
**
**  First, comtest_client_create_lock is called to create
**  lockfile, COMTST_client_lockfile to insure that only
**  one cient process is runnig simultaneously.  Next the
**  parent process, which is assumed to have been invoked
**  from the 'tsh', forks a child copy of itself so that
**  the parent may return immediately to the 'tsh' and 
**  then server side tests can begin to be invoked. This
**  forking may be overridden by the '-d' option.
**  Next, comtest_client_open_environ is called to open the
**  input files, COMTST_pid_tmpfile, COMTST_obj_tmpfile,
**  and COMTST_protseqs_tmpfile.  By default there is a wait
**  of 2 seconds to provide sufficient time for the server
**  side to create these files.  This wait time may be varied
**  with the '-w wait_time' option.  It will have to be adjusted
**  according to the speed of the host processor.
**  If these files are successfully opend We then begin reading
**  a from the COMTST_obj_tmpfile file, a list of object UUIDs
**  and from the COMTST_protseqs_tmpfile, a list of protocol
**  sequence strings.  For each object which the server has
**  indicated, we attempt create a binding handle and send a
**  RPC over each network protocol which the server has found
**  to be supported on the local host processor.
**  After sending a series of RPCs, one final rpc is sent
**  indicating to the server that this particular test is
**  complete.  Then a SIGINT is sent to force the server to exit
**  itself and the 'tsh' may subsequently begin another server
**  object test.
**
**  It isn't necessary to reinvoke this client side if another
**  test is to begin immediately.  It will wait for the period 
**  indicated to try to reopen the input files.
**  
**  INPUTS:             
**
**      argc            argument count
**      argv            array of pointers to string input arguments
**
**       COMTST_obj_tmpfile      - a list of object UUID strings
**                                 for which RPCs are to be sent
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
**  INPUTS/OUTPUTS:     none
**
**      
**
**  OUTPUTS:
**
**       COMTST_client_lockfile    a sentinel file to insure only one
**                                 client is running
**
**      status                     an rpc status code
**
**
**  IMPLICIT INPUTS:    none 
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     int
**
**      return          an integer specifying success or some
**                      type of failure
**                      
**
**  SIDE EFFECTS:       none 
**
**--
**/

comtest_object_xfr (argc, argv, status)

unsigned32                     argc;
char                           *argv[];
unsigned32                     *status;

{
    FILE                       *prottf;
    FILE                       *pidtf;
    FILE                       *objtf;
    uuid_t                     obj_uuid;
    rpc_binding_handle_t       binding_h;
    unsigned   long            omsg;
    unsigned   long            spid;
    char                       *cp;

    unsigned32  debug =        (unsigned32) NULL; 
    unsigned32  wait_time =    COMTST_default_wait;
    int                        child;
    rpc_mgr_epv_t              object_ptr,
                               sav_ptr;
    rpc_if_handle_t            if_rep_p;
    unsigned32                 obj_idx;
    unsigned32                 j;
    unsigned32                 k;    
    
    
    CHECK_ARGS(COMTEST_SERVER_XFR_ARGS);
    PRINT_ARGS;
    argc--, argv++;

    /*
     * search through the input options
     */
    while (argc > 0 && **argv == '-') 
    {
        for (cp = *argv + 1; *cp; cp++)
        {
            switch (*cp) 
            {
                /*
                 * debug mode - don' fork
                 */
                case 'd':
                debug = 1;
                break;  
                /*
                 * wait time
                 */
                case 'w':
                wait_time = atoi(*argv);
                argc--, argv++;
                break;  
            }
        }
        argc--, argv++;
    }

    /*
     * try to create a lock file if not already locked
     */
    comtest_client_create_lock (status);
    CHECK_STATUS;


#ifndef vms

    /*
     * if not debug mode fork and return to tsh
     */
    if (!debug)
    {
        if ((child = fork()) != (unsigned32) NULL)
        {
            if (child == -1)
            {
                fprintf (stdout,
                         "comtest_client_xfr: - fork() call failed\n");
                return(COMTEST_FAILED);
            }
            else
            {
                *status = rpc_s_ok;
                return(COMTEST_OK);
            }
        }
    }

#endif
    
    /*
     *  try to open the input file - after 'wait_time'
     */
    while ((comtest_client_open_environ
            (&prottf, &pidtf, &objtf, wait_time)) == 3)
    {  
        /*
         * get the process id file from the server
         */
        if (j = fscanf (pidtf, "%s", COMTST_protseq_buf) == EOF)
        {
            return(COMTEST_FAILED);
        }
        spid = atoi(COMTST_protseq_buf);
        fclose (pidtf);
        
        /*
         * stay in this loop as long as we are able
         * to read object UUIDs from input file.  send
         * RPCs bound to each object UUID we receive
         */     
        do
        {       
            for (obj_idx = 0;
                 obj_idx < COMTST_object_vec_siz;
                 obj_idx++, object_ptr++)
            {
                /*
                 * read an object UUID and interface indicator
                 */
                if (k = fscanf (objtf,
                                "%s %c", COMTST_obj_buf,
                                &COMTST_interface_buf) != EOF)
                {
                    /*
                     * is it a nil uuid requested?
                     */
                    if ((strcmp (COMTST_obj_buf, NIL_UUID)) == 0)
                    {
                        uuid_create_nil (&obj_uuid, status);
                        
                        if (*status != rpc_s_ok)
                        {
                            return RPC_CALL_FAILED;
                        }
                    } else
                        /*
                         * no - regular uuid
                         */
                    {
                        uuid_from_string ((unsigned_char_t *)COMTST_obj_buf,
                                          &obj_uuid, status );
                        if (*status != rpc_s_ok)
                        {
                            return RPC_CALL_FAILED;
                        }
                    }               

                    switch (COMTST_interface_buf)
                    {
                        case 'X':
                        if_rep_p = interface_X_v2_0_c_ifspec;
                        object_ptr = (rpc_mgr_epv_t) COMTST_object_vec_X;
                        break;
                        case 'Y':
                        if_rep_p = interface_Y_v2_0_c_ifspec;
                        object_ptr =
                            (rpc_mgr_epv_t) COMTST_object_vec_Y;
                    }

                    sav_ptr = object_ptr;

                    while (j = fscanf
                           (prottf, "%s", COMTST_protseq_buf) != EOF)
                    {
                        rpc_binding_from_string_binding ( 
                                                         COMTST_protseq_buf,
                                                         &binding_h, 
                                                         status);
                        if (*status != rpc_s_ok)
                        {
                            return RPC_CALL_FAILED;
                        }
                        
                        rpc_binding_set_object (binding_h,
                                                &obj_uuid,
                                                status);
                        
                        if (*status != rpc_s_ok)
                        {
                            return RPC_CALL_FAILED;
                        }

                        {
			    vararg_fn_t temp;

			    temp = (vararg_fn_t) (*object_ptr);
                            (*temp) (binding_h,
                                        COMTST_client_more_tsts,
                                        (sstring_t *)COMTST_obj_buf,
                                        &omsg,
                                        status);
			}

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
                        
                        rpc_binding_free (&binding_h, status);
                        
                        if (*status != rpc_s_ok)
                        {
                            return RPC_CALL_FAILED;
                        }

                    } /* end while */
                    
                    rewind (prottf);

                } /* end if */

                else
                {
                    rpc_binding_from_string_binding ( 
                                                     COMTST_protseq_buf,
                                                     &binding_h, 
                                                     status);
                    
                    if (*status != rpc_s_ok)
                    {
                        return RPC_CALL_FAILED;
                    }
                    
                    rpc_binding_set_object (binding_h,
                                            &obj_uuid,
                                            status);
                    if (*status != rpc_s_ok)
                    {
                        return RPC_CALL_FAILED;
                    }
                    
		    {
			vararg_fn_t temp;

			temp = (vararg_fn_t) (*sav_ptr);
                        (*temp) (binding_h,
                                COMTST_client_nomore_tsts,
                                (sstring_t *)"Test Complete",
                                &omsg,
                                status);
		    }

                    if (*status != rpc_s_ok)
                    {
                        return RPC_CALL_FAILED;
                    }
                    
                    rpc_binding_free (&binding_h, status);
                    
                    if (*status != rpc_s_ok)
                    {
                        return RPC_CALL_FAILED;
                    }
                    break;

                } /* end else */
            }   /* end for */
        }   /* end do */
        while (k);      
        
        fclose (prottf);
        fclose (objtf);
        kill (spid, SIGINT);
        remove (COMTST_pid_tmpfile);
        remove (COMTST_protseqs_tmpfile);
    }
    remove (COMTST_client_lockfile);
    if (debug) return(COMTEST_OK);
    exit (0);
}


/*
**++
**
**  Routine NAME:       comtest_client_open_environ
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**   first sleep for 'wait_time' seconds this is adjustable
**   by the '-w time' option on input from the 'tsh' to provide
**   sufficient time for the server to initialize itself and
**   create the input files.  we also return here for the begining
**   for the next test in a series of tests in a 'tsh' script.
**   after the wait open input files COMTST_pid_tmpfile,
**   COMTST_protseqs_tmpfile, and COMTST_obj_tmpfile and return
**   the file descriptors for each of these files to the addresses
**   provided.
**
**  INPUTS:
**
**          wait  - time to sleep before trying to
**                  open input files gives server time
**                  to start up before each test
**
**  INPUTS/OUTPUTS:
**
**            fp - file descriptor for COMTST_protseqs_tmpfile
**
**            pp - file descriptor for COMTST_pid_tmpfile
**
**            op - file descriptor for COMTST_obj_tmpfile
**
**
**  OUTPUTS:
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**         rstatus      this should be 3 if all three files are
**                      opened successfully, less indicates 
**                      failure
**
**  SIDE EFFECTS:       none
**
**--
**/

comtest_client_open_environ (fp, pp, op, wait)

FILE          **fp;
FILE          **pp;
FILE          **op;
unsigned32    wait;

{
unsigned32    rstatus;    
    
    rstatus = (unsigned32) NULL;
    sleep (wait);
    
    if ((*pp = fopen (COMTST_pid_tmpfile, "r")) == NULL)
    {
        return (rstatus);
    }
    rstatus++;
    if ((*fp = fopen (COMTST_protseqs_tmpfile, "r")) == NULL)
    {
        return (rstatus);
    }
    rstatus++;
    if ((*op = fopen (COMTST_obj_tmpfile, "r")) == NULL)
    {
        return(rstatus);
    }
    rstatus++;
    return (rstatus);
}


/*
**++
**
**  Routine NAME:       comtest_client_create_lock
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**  open a sentinel file 'COMTST_client_lockfile' to 
**  indicate that a client process is presently running
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:            none
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

comtest_client_create_lock (status)

unsigned32       *status;

{
    unsigned32  ii;
    unsigned32   pid;
    int          siz;
    int          fd;  
    char         pid_buf[30];    

    

    if ((fd = open (COMTST_client_lockfile,
                   (O_CREAT|O_EXCL|O_RDWR), 0666)) >=  0)
    {
        pid = getpid();
        sprintf (pid_buf, "%d", pid);
        siz = strlen(pid_buf);
        close (fd);
        *status = rpc_s_ok;
    }
    else if (errno == EEXIST)
        
    {
        fprintf(stdout,
                "COMTST_client_lockfile: - client already locked\n");
        return (COMTEST_FAILED);
    }
    else
    {
        fprintf(stdout,
                "failed to open lockfile COMTST_client_lockfile\n");
        return (COMTEST_FAILED);
    }
    
    return (COMTEST_OK);
}
