/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsinit.c,v $
 * Revision 1.1.325.2  1996/02/18  00:05:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:07  marty]
 *
 * Revision 1.1.325.1  1995/12/08  00:21:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:04  root]
 * 
 * Revision 1.1.323.3  1994/07/29  16:19:21  tom
 * 	Remove DNSPI_V2 ifdef.
 * 	[1994/07/28  22:34:12  tom]
 * 
 * Revision 1.1.323.2  1994/06/21  21:54:12  hopkins
 * 	More serviceability
 * 	[1994/06/08  21:33:06  hopkins]
 * 
 * Revision 1.1.323.1  1994/01/21  22:38:35  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:00  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:53:47  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:10:27  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:13:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:42:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:12:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      nsinit.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  NS Initialization module.
**
**
*/

#include <commonp.h>    /* Internals for RPC runtime system              */
#include <com.h>        /* Externals for Common Services component       */
#include <comcthd.h>    /* Externals for call thread services component  */
#include <ns.h>         /* Externals for Name Services component         */
#include <nsp.h>        /* Internals for Name Services component         */

INTERNAL void rpc__ns_init_once _DCE_PROTOTYPE_ ((void));

INTERNAL void rpc__ns_specific_free _DCE_PROTOTYPE_((pointer_t *));

/*
 * The structure that defines the one-time initialization code. This
 * structure will be associated with ns_init_once.
 */
INTERNAL pthread_once_t rpc__ns_init_once_block = pthread_once_init;

/*
 * The value that indicates whether or not the RPC runtime is currently
 * being initialized.
 */
INTERNAL boolean        rpc__ns_init_in_progress = false;

/*
 * The id of the thread that is executing (has executed) the NS
 * initialization code.
 */
GLOBAL   pthread_t      rpc_g_ns_init_thread;

/*
**++
**  ROUTINE NAME:       rpc__ns_init
**
**  SCOPE:              PRIVATE - declared in nsinit.h
**
**  DESCRIPTION:
**      
**  rpc__ns_init() is called to intialize the runtime.  It can safely be
**  called at any time, though it is typically called only be
**  RPC__NS_VERIFY_INIT() in order to minimize overhead.  Upon return from
**  this routine, the name service will be initialized.
**
**  Prevent rpc__ns_init() (actually pthread_once() of rpc__ns_init_once()) 
**  recursive call deadlocks by the thread that is actually performing the
**  initialization (see rpc__ns_init_once()).
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
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__ns_init(void)
{
    pthread_t       current_thread;
    

    if (rpc__ns_init_in_progress)
    {
        current_thread = pthread_self();
        
        if (pthread_equal(rpc_g_ns_init_thread, current_thread))
        {
            /*
             * We're the thread in the middle of initialization 
             * (rpc__ns_init_once).
             * Assume it knows what it's doing and prevent a deadlock.
             */
            return;
        }
    }

    pthread_once (&rpc__ns_init_once_block, (pthread_initroutine_t) rpc__ns_init_once);
}


/*
**++
**
**  ROUTINE NAME:       rpc__ns_init_once
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**      
**  rpc__ns_init_once() is invoked only once. It performs the basic 
**  initialization functions for the NS Services.
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
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__ns_init_once(void)

{
    unsigned32              ns;
    unsigned32              i;

    /*
     * These two operations (and their order) are critical to creating
     * a deadlock-safe environment for init processing.
     */
    rpc_g_ns_init_thread = pthread_self();
    rpc__ns_init_in_progress = true;

    /* 
     * Initialize the global table for the DNS attributes.
     */
    for (ns = 0; ns < RPC_C_NS_MAX; ns++)
    {
        for (i = 0; i < RPC_C_ATTR_MAX; i++)
        {

            /* 
             * Get the string attribute to its name service representation.
             */

            strcpy ((char *) rpc_g_attr_table[ns][i].attr_name.name,
                    (char *) rpc_g_attr_table[ns][i].attr_string);

        }
    }

    /*
     * initialize any free lists that need it
     */
    rpc__list_desc_init (&rpc_g_free_if_list, RPC_C_IF_LIST_MAX,
        sizeof (rpc_if_list_elt_t), RPC_C_MEM_IF_REP, NULL, NULL, NULL, NULL);
        
    rpc__list_desc_init (&rpc_g_free_members, RPC_C_MEMBERS_LIST_MAX,
        sizeof (rpc_lkup_mbr_t), RPC_C_MEM_LKUP_MBR, NULL, NULL, NULL, NULL);
        
    rpc__list_desc_init (&rpc_g_free_lookup_nodes, RPC_C_LOOKUP_NODES_LIST_MAX,
        sizeof (rpc_lkup_node_t), RPC_C_MEM_LKUP_NODE, NULL, NULL, NULL, NULL);

   /*
    * Initialize a global ns mutex lock
    */
   RPC_MUTEX_INIT (ns_lock);


   /*
    * Randomly generate the global default expiration age.
    */
   rpc_g_ns_exp_age = RPC_RANDOM_GET ( RPC_C_GLB_EXP_AGE_LOWER_LIMIT,
                                              RPC_C_GLB_EXP_AGE_UPPER_LIMIT);

   /*
    * initialize the indirection pointer to the binding ns_specific free fn.
    */
   rpc_g_ns_specific_free_fn = rpc__ns_specific_free;
                                                     
   rpc__ns_init_in_progress = false;
   rpc_ns_initialized = true;
}

/*
**++
**
**  ROUTINE NAME:       rpc__ns_specific_free
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**      
**  Free the NS specific storage refered to in a binding handle.
**  This routine is invoked through the runtime's indirection pointer
**  "rpc_g_ns_specific_free_fn".
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     ns_specific pointer to binding's ns_specific field
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__ns_specific_free 
#ifdef _DCE_PROTO_
(
    pointer_t *ns_specific
)
#else
(ns_specific)
pointer_t *ns_specific;
#endif
{
   rpc_nsentry_p_t nsentry = (rpc_nsentry_p_t) *ns_specific;

   RPC_MEM_FREE (nsentry->name, RPC_C_MEM_NSENTRY);
   RPC_MEM_FREE (nsentry, RPC_C_MEM_NSENTRY);
   *ns_specific = NULL;
}

