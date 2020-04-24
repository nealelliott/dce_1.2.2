/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnbind.c,v $
 * Revision 1.1.96.2  1996/02/18  00:01:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:17  marty]
 *
 * Revision 1.1.96.1  1995/12/08  00:16:52  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:29 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/02  01:13 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:17 UTC  lmm
 * 	add memory allocation failure detection
 * 	[1995/12/07  23:57:20  root]
 * 
 * Revision 1.1.94.1  1994/01/21  22:33:51  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:53:34  cbrooks]
 * 
 * Revision 1.1.8.2  1993/08/26  18:05:35  tatsu_s
 * 	Bug 8104 - Added the dummy rpc__cn_binding_cross_fork().
 * 	[1993/08/24  16:06:01  tatsu_s]
 * 
 * Revision 1.1.5.5  1993/03/08  18:35:18  weisman
 * 	Ignore previous branch, inserting code for OT 7441.
 * 	[1993/03/08  15:03:19  weisman]
 * 
 * 	Fix for OT 7441
 * 	[1993/03/07  18:46:54  weisman]
 * 
 * Revision 1.1.5.4  1993/01/03  22:58:08  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:58:34  bbelch]
 * 
 * Revision 1.1.5.3  1992/12/23  20:13:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:30:21  zeliff]
 * 
 * Revision 1.1.5.2  1992/12/18  19:52:58  wei_hu
 * 	      3-aug-92   dm       Modify rpc__cn_binding_inq_addr() to return
 * 	                     rpc_s_connection_closed instead of
 * 	                     rpc_s_assoc_grp_not_found if an assoc group
 * 	                     was not found.
 * 	[1992/12/18  19:42:43  wei_hu]
 * 
 * Revision 1.1.2.2  1992/05/01  18:37:39  rsalz
 * 	"Changed as part of rpc6 drop."
 * 	[1992/05/01  18:26:01  rsalz]
 * 
 * 	 3-mar-92    wh      DCE 1.0.1 merge.
 * 	17-dec-91    ko      Bug fix for binding serialization. Clear being_
 * 	                     resolved flag when client binding is allocated.
 * 	[1992/05/01  18:06:21  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:09:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cnbind.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  The NCA Connection Protocol Service's Binding Service.
**
**
*/

#include <commonp.h>    /* Common declarations for all RPC runtime */
#include <com.h>        /* Common communications services */
#include <comprot.h>    /* Common protocol services */
#include <cnp.h>        /* NCA Connection private declarations */
#include <cnpkt.h>	/* NCA Connection packet encoding */
#include <cnassoc.h>    /* NCA Connection association service */
#include <cnid.h>       /* NCA Connection local id service */
#include <cnbind.h>     


/*
**++
**
**  ROUTINE NAME:       rpc__cn_binding_alloc
**
**  SCOPE:              PRIVATE - declared in cnbind.h
**
**  DESCRIPTION:
**
**      This routine will allocate a binding rep data structure. A
**      binding rep contains a protocol specific part and therefore
**      must be allocated by the protocol service.
**
**  INPUTS:
**
**      is_server       Boolean indicating server-side binding handle should
**                      be created.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      return          The allocated binding rep structure.
**      rpc_s_ok
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE rpc_binding_rep_t *rpc__cn_binding_alloc 
#ifdef _DCE_PROTO_
(
  boolean32               is_server,
  unsigned32              *st
)
#else
(is_server, st)

boolean32               is_server;
unsigned32              *st;
#endif
{
    rpc_cn_binding_rep_p_t brp;

    CODING_ERROR (st);

    /*
     * A server binding is allocated from the receiver thread which
     * has already acquired the CN global mutex.
     */
    if (!is_server)
    {
        RPC_CN_LOCK ();
    }
    else
    {
        RPC_CN_LOCK_ASSERT ();
    }

    /*
     * Allocate a binding rep off the lookaside list.
     */
    brp = (rpc_cn_binding_rep_p_t) 
        rpc__list_element_alloc (&rpc_g_cn_binding_lookaside_list,
                                 true);
    if (brp == NULL){
	*st = rpc_s_no_memory;
	if (!is_server)
	    RPC_CN_UNLOCK();
	return(NULL);
    }	
    if (!is_server)
    {
        ((rpc_cn_binding_rep_t *)brp)->being_resolved = false;
        RPC_CN_UNLOCK ();
    }

    brp->common.rpc_addr = NULL;
    brp->common.auth_info = NULL;

    *st = rpc_s_ok;
    return ((rpc_binding_rep_p_t) brp);
}


/*
**++
**
**  ROUTINE NAME:       rpc__cn_binding_init
**
**  SCOPE:              PRIVATE - declared in cnbind.h
**
**  DESCRIPTION:
**
**      This routine will initialize the protocol specific part of a
**      previously allocated binding rep data structure. For the NCA
**      Connection protocol this means locating an association group
**      whose primary address matches that in the binding rep. Note
**      that this can only be done if an RPC address is supplied.
**
**  INPUTS:
**
**      binding_r       The binding rep to be initialized.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:    
**
**      rpc_s_coding_error
**      rpc_s_ok
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__cn_binding_init 
#ifdef _DCE_PROTO_
(
 rpc_binding_rep_p_t     binding_r,
 unsigned32              *st
)
#else
(binding_r, st)

 rpc_binding_rep_p_t     binding_r;
 unsigned32              *st;
#endif
{
    unsigned32                  type;
    rpc_cn_local_id_t           grp_id;

    CODING_ERROR (st);

    /*
     * Determine the type of association group we are looking for.
     */
    if (RPC_BINDING_IS_SERVER (binding_r))
    {
        type = RPC_C_CN_ASSOC_GRP_SERVER;
    }
    else
    {
        type = RPC_C_CN_ASSOC_GRP_CLIENT;

        /*
         * Use the RPC address contained in the binding rep to find an
         * association group.
         */
        RPC_CN_LOCK ();
        grp_id = rpc__cn_assoc_grp_lkup_by_addr (binding_r->rpc_addr,
                                                 type,
                                                 st);

        /*
         * Record its the association group id returned into the
         * binding rep. Note that the group id returned may be invalid.
         */
        ((rpc_cn_binding_rep_t *)binding_r)->grp_id = grp_id;
        RPC_CN_UNLOCK ();
    }
        
    /*
     * Return a good status code no matter what happened.
     */
    *st = rpc_s_ok;
}


/*
**++
**
**  ROUTINE NAME:       rpc__cn_binding_reset
**
**  SCOPE:              PRIVATE - declared in cnbind.h
**
**  DESCRIPTION:
**
**      This routine will clear the endpoint in the RPC address
**      contained in the binding rep data structure given.
**
**  INPUTS:
**
**      binding_r       The binding rep containing the RPC address
**                      to be reset.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     
**
**      rpc_s_coding_error
**      rpc_s_ok
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__cn_binding_reset 
#ifdef _DCE_PROTO_
(
  rpc_binding_rep_p_t     binding_r,
  unsigned32              *st
)
#else
(binding_r, st)

rpc_binding_rep_p_t     binding_r;
unsigned32              *st;
#endif
{
    CODING_ERROR (st);

    /*
     * The endpoint in the binding is being reset. Since we assume
     * endpoints are tied to address spaces and an association group
     * id identifies an address space we should reset the group ID.
     * When the endpoint is added to the binding
     * rpc__cn_binding_init should be called again to set the group id.
     */
    RPC_CN_LOCAL_ID_CLEAR (((rpc_cn_binding_rep_t *)binding_r)->grp_id);

    *st = rpc_s_ok;
}


/*
**++
**
**  ROUTINE NAME:       rpc__cn_binding_changed
**
**  SCOPE:              PRIVATE - declared in cnbind.h
**
**  DESCRIPTION:
**
**      This routine is called when something in the common part of
**      the binding rep given has changed.
**
**  INPUTS:
**
**      binding_r       The binding rep which changed.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:    
**
**      rpc_s_coding_error
**      rpc_s_ok
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__cn_binding_changed 
#ifdef _DCE_PROTO_
(
  rpc_binding_rep_p_t     binding_r,
  unsigned32              *st
)
#else
(binding_r, st)

rpc_binding_rep_p_t     binding_r;
unsigned32              *st;
#endif
{
    CODING_ERROR (st);

    /*
     * The exact field which changed is not known. Init the binding.
     */
    rpc__cn_binding_init (binding_r, st);
}


/*
**++
**
**  ROUTINE NAME:       rpc__cn_binding_free
**
**  SCOPE:              PRIVATE - declared in cnbind.h
**
**  DESCRIPTION:
**
**      This routine frees the binding rep data structure given.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      binding_r       The binding rep to be freed
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     
**
**      rpc_s_coding_error
**      rpc_s_ok
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__cn_binding_free 
#ifdef _DCE_PROTO_
(
  rpc_binding_rep_p_t     *binding_r,
  unsigned32              *st
)
#else
(binding_r, st)

rpc_binding_rep_p_t     *binding_r;
unsigned32              *st;
#endif
{
    CODING_ERROR (st);

    RPC_CN_LOCK_ASSERT ();

    /*
     * Put the binding rep back on the lookaside list.
     */
    rpc__list_element_free (&rpc_g_cn_binding_lookaside_list, 
                            (pointer_t) *binding_r);

    /*
     * Null the caller's pointer to this memory.
     */
    *binding_r = NULL;
    *st = rpc_s_ok;
}


/*
**++
**
**  ROUTINE NAME:       rpc__cn_binding_inq_addr
**
**  SCOPE:              PRIVATE - declared in cnbind.h
**
**  DESCRIPTION:
**
**      This routine allocates and returns the RPC address
**      associated with the binding rep data structure given.
**
**  INPUTS:
**
**      binding_r       The binding rep to be freed
**
**  INPUTS/OUTPUTS:
**
**      rpc_addr        The RPC address which is allocated.
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     
**
**      rpc_s_coding_error
**      rpc_s_ok
**      rpc_s_assoc_grp_not_found
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__cn_binding_inq_addr 
#ifdef _DCE_PROTO_
(
  rpc_binding_rep_p_t     binding_r,
  rpc_addr_p_t            *rpc_addr,
  unsigned32              *st
)
#else
(binding_r, rpc_addr, st)

rpc_binding_rep_p_t     binding_r;
rpc_addr_p_t            *rpc_addr;
unsigned32              *st;
#endif
{
    unsigned32          type;
    rpc_cn_local_id_t   grp_id;
    rpc_cn_assoc_grp_t  *assoc_grp;
    rpc_protseq_id_t    protseq_id;

    CODING_ERROR (st);
    
    /*
     * Determine the type of association group we are looking for.
     */
    if (RPC_BINDING_IS_SERVER (binding_r))
    {
        type = RPC_C_CN_ASSOC_GRP_SERVER;
    }
    else
    {
        type = RPC_C_CN_ASSOC_GRP_CLIENT;
    }
        
    /*
     * Use the group id contained in the binding rep to find an
     * association group.
     */
    RPC_CN_LOCK ();
    grp_id = rpc__cn_assoc_grp_lkup_by_id (
                 ((rpc_cn_binding_rep_t *)binding_r)->grp_id,
                 type,
                 st);

    /*
     * Check whether an association group was found or not.
     */
    if (RPC_CN_LOCAL_ID_VALID (grp_id))
    {
        /*
         * An association group was found. Check whether it
         * contains an RPC address.
         */
        assoc_grp = RPC_CN_ASSOC_GRP (grp_id);
        if (assoc_grp->grp_address != NULL)
        {
            /*
             * The association group contains an RPC address.
             * Copy it into the binding.
             */
            rpc__naf_addr_copy (assoc_grp->grp_address,
                                rpc_addr,
                                st);
        }
        else
        {
            /*
             * The association doesn't have an RPC address. Find
             * it out using one of the connections attached to the
             * association group.
             */
            rpc__naf_desc_inq_protseq_id 
                (((rpc_cn_assoc_t *)assoc_grp->grp_assoc_list.next)->cn_ctlblk.cn_sock,
                 RPC_C_PROTOCOL_ID_NCACN,
                 &protseq_id,
                 st);
            if (*st == rpc_s_ok)
            {
                rpc__naf_desc_inq_peer_addr 
                    (((rpc_cn_assoc_t *)assoc_grp->grp_assoc_list.next)->cn_ctlblk.cn_sock,
                     protseq_id, 
                     rpc_addr,
                     st);
                if (*st == rpc_s_ok)
                {
                    rpc__naf_addr_copy (*rpc_addr,
                                        &assoc_grp->grp_address,
                                        st);
                }
            }
        }
        binding_r->rpc_addr = *rpc_addr;
    }
    else
    {
        *st = rpc_s_connection_closed;
    }
    RPC_CN_UNLOCK ();
}


/*
**++
**
**  ROUTINE NAME:       rpc__cn_binding_inq_client
**
**  SCOPE:              PRIVATE - declared in cnbind.h
**
**  DESCRIPTION:
**
**      This routine returns the client process identifier, i.e. the
**      association group id, for a given binding rep.
**
**  INPUTS:
**
**      binding_r       The binding rep to be freed
**
**  INPUTS/OUTPUTS:
**
**      rpc_addr        The RPC address which is allocated.
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     
**
**      rpc_s_coding_error
**      rpc_s_ok
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__cn_binding_inq_client 
#ifdef _DCE_PROTO_
(
  rpc_binding_rep_p_t     binding_r,
  rpc_client_handle_t     *client_h,
  unsigned32              *st
)
#else
(binding_r, client_h, st)

rpc_binding_rep_p_t     binding_r;
rpc_client_handle_t     *client_h;
unsigned32              *st;
#endif
{
    rpc_cn_local_id_t   grp_id;

    CODING_ERROR (st);

    /*
     * Get the association group id from the binding rep.
     */
    grp_id = ((rpc_cn_binding_rep_t *)binding_r)->grp_id;

    /*
     * Make sure the group id is valid.
     */
    *client_h = (rpc_client_handle_t) grp_id.all;
    if (RPC_CN_LOCAL_ID_VALID (grp_id))
    {
        *st = rpc_s_ok;
    }
    else
    {
        *st = rpc_s_invalid_binding;
    }
}

/*
**++
**
**  ROUTINE NAME:       rpc__cn_binding_copy
**
**  SCOPE:              PRIVATE - declared in cnbind.h
**
**  DESCRIPTION:
**
**      This routine will init the CN specific fields of a copied
**      binding rep.
**
**  INPUTS:
**
**      src_binding_r   The binding rep to be copied.
**      dst_binding_r   The binding rep being copied into.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     
**
**      rpc_s_coding_error
**      rpc_s_ok
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__cn_binding_copy 
#ifdef _DCE_PROTO_
(
  rpc_binding_rep_p_t     src_binding_r,
  rpc_binding_rep_p_t     dst_binding_r,
  unsigned32              *st
)
#else
(src_binding_r, dst_binding_r, st)

rpc_binding_rep_p_t     src_binding_r;
rpc_binding_rep_p_t     dst_binding_r;
unsigned32              *st;
#endif
{
    CODING_ERROR (st);

    /*
     * Get the association group id from the src binding rep and put
     * it in the dst binding rep.
     */
    ((rpc_cn_binding_rep_t *)dst_binding_r)->grp_id = 
        ((rpc_cn_binding_rep_t *)src_binding_r)->grp_id;

    *st = rpc_s_ok;
}


/*
**++
**
**  ROUTINE NAME:       rpc__cn_binding_cross_fork
**
**  SCOPE:              PRIVATE - declared in cnbind.h
**
**  DESCRIPTION:
**
**      This routine makes it possible for children of forks to use
**      binding handles inherited from their parents.
**
**  INPUTS:
**
**      binding_r       The binding rep to be inherited
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     
**
**      rpc_s_coding_error
**      rpc_s_ok
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__cn_binding_cross_fork 
#ifdef _DCE_PROTO_
(
  rpc_binding_rep_p_t     binding_r,
  unsigned32              *st
)
#else
(binding_r, st)

rpc_binding_rep_p_t     binding_r;
unsigned32              *st;
#endif
{
    CODING_ERROR (st);

    /*
     * This is a dummy function to avoid the null reference
     * causing a core dump.
     */

    *st = rpc_s_ok;
}
