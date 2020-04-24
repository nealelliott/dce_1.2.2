/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: sigcatch.c,v $
 * Revision 1.1.4.2  1996/02/17  23:28:53  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:42  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:46:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:53  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:46:42  agd
 * 	expand copyright
 * 	[1994/10/06  14:32:58  agd]
 * 
 * Revision 1.1.2.1  1994/09/19  18:20:32  bhaim
 * 	multidelegate test
 * 	[1994/09/19  17:51:47  bhaim]
 * 
 * $EndLog$
 */
/*

#include <pthread.h>
#include <stdio.h>
#include <dce/rpcexc.h>
#include <dce/sec_login.h>
 
#include <dce/dce_error.h>
*/
/*********************************************************************
**
** S I G C A T C H
**
** This is the routine that the signal catching thread executes.  It waits
** for a fatal asynchronous signal, then tells the server to stop listening
** for further requests and to allow current requests to finish before
** returning from rpc_server_listen.
*********************************************************************/

#include <signal.h>

pthread_addr_t sigcatch(arg)
pthread_addr_t arg;
{
  error_status_t _ignore;           /* status code we intend to ignore */
  int which_sig;                    /* which signal woke us up */
  char *sig_name;
  sigset_t mask;                    /* bit mask of signals for which we want
                                       notification */

 /* initialize the signal mask.  Turn on the bits corresponding to
   * SIGTERM, SIGINT, and SIGHUP.
   */
  sigemptyset(&mask);
  sigaddset(&mask, SIGHUP);

  sigaddset(&mask, SIGINT);

  sigaddset(&mask, SIGTERM);

#if 0
  sigaddset(&mask, SIGUSR1);
  sigaddset(&mask, SIGUSR2);
  sigaddset(&mask, SIGLOST);
#endif

    /*
    ** Calling sigwait will cause this thread to block until the
    ** process receives one of the signals in the mask.  In this
    ** case the mask contains SIGTERM, SIGINT, or SIGHUP.  If no
    ** threads were waiting for these fatal asynchronous signals and
    ** such a signal were received, the process would die
    ** immediately without giving the server a chance to unregister
    ** its bindings with the endpoint mapper.  sigwait is the only
    ** way to catch a fatal asynchronous signal and have the
    ** opportunity to cleanup before exiting.
    */

    while (true) {
#ifdef TRACING
        TR_PRINTF (tr_handle, 8, 1,
            (
            "calling sigwait() for signals 1, 2, 15\n"
            )
        );
#endif

        which_sig = sigwait(&mask);

        switch (which_sig) {
        case SIGHUP:
            sig_name = "hangup";
            break;
        case SIGINT:
            sig_name = "interrupt";
            break;
        case SIGTERM:
            sig_name = "software termination";
            break;
        case SIGUSR1:
            sig_name = "user defined";
            break;
        case SIGUSR2:
            sig_name = "user defined";
            break;
        case SIGLOST:
            sig_name = "NFS file lock lost";
            break;
        default:
            sig_name = "unexpected";
            break;
        } /* end switch (which_sig) */


 /*       if (options.verbose) { */
            fprintf (stderr,
                "caught %s signal (signum %d)\n",
 /*               options.cmnd_name, */ 
                sig_name,
                which_sig
            );
/*       } */
 
#ifdef TRACING
        TR_PRINTF (tr_handle, 8, 1,
            (
            "caught %s signal (signum %d)\n",
            /* options.cmnd_name, */ 
            sig_name,
            which_sig
            )
        );
#endif

        /* Stop the server from listening for more requests, and let current
        * requests run to completion.  This routine causes rpc_server_listen
        * to return.
        *
        * Notice that if there are pending RPC's that don't complete in a timely
        * manner, another fatal signal will kill the server.  We no longer have
        * a thread to catch fatal signals.
        */
        rpc_mgmt_stop_server_listening(NULL, &_ignore);

    } /* end while (true) */


/*    if (options.verbose) { */
        fprintf (stderr,
            "Warning -- the signal catching thread has terminated.\n"
/*            options.cmnd_name */ 
        );
/*    } */
#ifdef TRACING
    TR_PRINTF (tr_handle, 6, 1,
        (
        "Warning -- the signal catching thread is terminating.\n"
        )
    );
#endif

} /* end sigcatch() */



/*
**********************************************************************
**
** S E T U P _ S I G C A T C H
**
** This routine sets up the signal catching thread.
**********************************************************************
*/
void setup_sigcatch
#ifdef PROTOTYPES
(
   /* [out] */ pthread_t *sigcatch_thread
)
#else
   (sigcatch_thread)
   /* [out] */ pthread_t *sigcatch_thread;
#endif
{
   int                         thread_status;

#ifdef TRACING
    TR_PRINTF (tr_handle, 8, 1,
        ("Set up signal catching thread\n")
    );
#endif


   thread_status = pthread_create (
      sigcatch_thread,      /* store thread id to detach */
      pthread_attr_default,  /* use default thread attributes */
      sigcatch,              /* start executing in the sigcatch routine */
      NULL                   /* argument to sigcatch routine */
   );

    if (thread_status != 0) {
        fprintf(stderr,
            "pthread_create for sigcatch failed: errno = %d\n",
            errno
        );
        fprintf (stderr,
            "exiting.\n"
/*            options.cmnd_name */ 
        );
        exit(1);
    }

   /*
   ** We're not interested in a return value from sigcatch_thread,
   ** so allow the runtime to reclaim storage when this thread finishes.
   */
   thread_status = pthread_detach(sigcatch_thread);

    if (thread_status != 0) {
        fprintf(stderr,
            "pthread_detach for sigcatch thread failed: errno = %d\n",
            errno
        );
        fprintf (stderr,
            "exiting.\n"
/*            options.cmnd_name */ 
        );
        exit(1);
    } /* end if (thread_status != 0) */

   /*
   ** Give the sigcatch thread a chance to do its work before we
   ** continue.  Force a context switch to the signal catching thread.
   ** The signal catching thread will yield back to us when it calls
   ** sigwait().
   */
   pthread_yield();

   return;
} /* end setup_sigcatch() */

