/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnfbuf.c,v $
 * Revision 1.1.61.2  1996/02/18  00:01:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:31  marty]
 *
 * Revision 1.1.61.1  1995/12/08  00:17:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:32  root]
 * 
 * Revision 1.1.59.1  1994/01/21  22:34:13  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:53:49  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  22:58:37  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:59:21  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  20:15:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:31:07  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  18:38:10  rsalz
 * 	"Changed as part of rpc6 drop."
 * 	[1992/05/01  18:26:35  rsalz]
 * 
 * 	10-apr-92    af      Use macro RPC_CN_ALIGN_PTR to align pointers
 * 	                     on specified boundaries.
 * 	 1-apr-92    ko      Change DEBUG to MAX_DEBUG for clearing
 * 	                     fragbufs
 * 	[1992/05/01  18:07:06  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:07:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cnfbuf.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Fragment buffer management routines for connection based 
**  protocol services.
**
**
*/

#include <commonp.h>    /* Common declarations for all RPC runtime */
#include <com.h>        /* More common declarations */
#include <cnp.h>        /* Connection common declarations */
#include <cnfbuf.h>	/* Fragment buffer declarations */

GLOBAL unsigned32 rpc_g_cn_large_frag_size = RPC_C_CN_LARGE_FRAG_SIZE;


/*
**++
**
**  ROUTINE NAME:       rpc__cn_fragbuf_free
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**      
**  Deallocates a large fragment buffer.
**
**  INPUTS:
**
**      buffer_p        Pointer to the large fragment buffer which is to be
**                      deallocated.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    lg_fragbuf_list  
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__cn_fragbuf_free 
#ifdef _DCE_PROTO_
(
   rpc_cn_fragbuf_p_t buffer_p 
)
#else
(buffer_p)
rpc_cn_fragbuf_p_t            buffer_p;
#endif
{
#ifdef MAX_DEBUG
    memset ((char *) buffer_p->data_area, 0, rpc_g_cn_large_frag_size);
    memset ((char *) buffer_p, 0, sizeof (rpc_cn_fragbuf_t));
#endif
    rpc__list_element_free (&rpc_g_cn_lg_fbuf_lookaside_list, 
                            (pointer_t) buffer_p);
}

/*
**++
**
**  ROUTINE NAME:       rpc__cn_smfragbuf_free
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**      
**  Deallocates a small fragment buffer.
**
**  INPUTS:
**
**      buffer_p        Pointer to the small fragment buffer which is to be
**                      deallocated.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    sm_fragbuf_list  
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__cn_smfragbuf_free 
#ifdef _DCE_PROTO_
(
   rpc_cn_fragbuf_p_t      buffer_p
)
#else
(buffer_p)
rpc_cn_fragbuf_p_t      buffer_p;
#endif
{
#ifdef MAX_DEBUG
    memset ((char *) buffer_p->data_area, 0, RPC_C_CN_SMALL_FRAG_SIZE);
    memset ((char *) buffer_p, 0, sizeof (rpc_cn_fragbuf_t));
#endif
    rpc__list_element_free (&rpc_g_cn_sm_fbuf_lookaside_list, 
                            (pointer_t) buffer_p );
}

/*
**++
**
**  ROUTINE NAME:       rpc__cn_fragbuf_alloc
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**      
**  Allocates a fragment buffer and returns a pointer to it.
**
**  INPUTS:
**
**      alloc_large_buf If TRUE, then allocates a large fragment
**                      buffer.  Otherwise, allocates a small one.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     Address of the allocated fragment buffer.
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE rpc_cn_fragbuf_p_t rpc__cn_fragbuf_alloc 
#ifdef _DCE_PROTO_
(
    boolean32               alloc_large_buf
)
#else
(alloc_large_buf)
boolean32               alloc_large_buf;
#endif
{
    rpc_cn_fragbuf_p_t  fbp;

    /*
     * Get a fragment buffer from the appropriate lookaside list
     */

    if (alloc_large_buf)
    {
    	fbp = (rpc_cn_fragbuf_p_t) 
            rpc__list_element_alloc (&rpc_g_cn_lg_fbuf_lookaside_list, 
                                     true);  
        if (fbp != NULL)
        {
            fbp->fragbuf_dealloc = rpc__cn_fragbuf_free;
            fbp->max_data_size = rpc_g_cn_large_frag_size;
        }
        else
        {
            return (NULL);
        }
    }
    else
    {
        fbp = (rpc_cn_fragbuf_p_t) 
            rpc__list_element_alloc (&rpc_g_cn_sm_fbuf_lookaside_list,
                                     true);  
        if (fbp != NULL)
        {
            fbp->fragbuf_dealloc = rpc__cn_smfragbuf_free;
            fbp->max_data_size = RPC_C_CN_SMALL_FRAG_SIZE;
        }
        else
        {
            return (NULL);
        }
    }

    /*
     * Set the data pointer to an 8 byte aligned boundary.
     */

    fbp->data_p = (pointer_t) RPC_CN_ALIGN_PTR(fbp->data_area, 8);
    
    /*
     * Set up the size of the data being pointed to.
     */
    fbp->data_size = 0;

    /*
     * Return a pointer to the "filled-in" fragment buffer
     */
    return (fbp);
}
