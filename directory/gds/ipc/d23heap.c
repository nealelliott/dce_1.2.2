/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d23heap.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:22  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:03:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:15:01  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:21  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:15:04  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:56:25  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:57  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  19:21:34  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:52:11  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:07:16  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:25:33  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:44:46  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:32:01  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:53:50  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:40:29  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d23heap.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:35 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    module                                                         */
/*                                                                           */
/*   NAME:    d23heap.c                                                      */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       This module contains all functions which are required to handle     */
/*       the heap which is contained in the shared memory area of a          */
/*       directory system installation. This heap is used to store general   */
/*       informations (e.g. user credentials) of variable length. The heap   */
/*       consists of a configurable no. of small heap buffers, each of a     */
/*       constant size (D23_HPBFSZ).                                         */
/*       The following functions are available                               */
/*                                                                           */
/*           * d23_shmalloc - allocate memory from heap                      */
/*           * d23_shmfree  - free allocated memory from heap                */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*    AUTHOR: R.Horn                                        DATE: 17.5.91    */
/*datoff *********************************************************************/

#include <gds.h>
#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23ipc.h>

/* ------------------- DECLARATION OF GLOBAL DATA -------------------------- */

extern D23_iprocinfo	d23_info ;	/* process specific IPC-information */

/* ------------------------------------------------------------------------- */

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_shmalloc ()                                                */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function allocates the requested size of memory from heap.      */
/*       This is done by setting the appropriate no. (depends on the         */
/*       requested memory size) of successive heap buffers to a busy state.  */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       rqsize      = Requested size of memory within the heap.             */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       > 0         = Offset to the beginning of the allocated heap buffer  */
/*       D23_ERROR  =  An internal error is occurred during execution of     */
/*                     the function.                                         */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

signed32 d23_shmalloc (signed32 rqsize) {

    unsigned32		rqbufmsk, bufmsk, *bufsptr ;
    signed32		bufidx, rqbufno ;	
    D23_hpbufinfo	*bufptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				    svc_c_debug1, GDS_S_CSSHMA_ENTRY, rqsize)) ;

    /* verify the requested heap buffer size */
    if (rqsize <= 0 || rqsize > (sizeof (D23_hpbufinfo) * D23_FLDSIZE) -
							    sizeof (signed32)) {
	/* invalid requested buffer size */
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			       svc_c_sev_notice, GDS_S_IPCLIMHEAP_ERR, rqsize) ;
	return (D23_ERROR) ;
    }

    /* generate search-mask for allocating the requested heap buffer(s) */
    rqsize += sizeof (D23_hpbufinfo) - D23_HPBFSZ ;
    rqbufno = ((rqsize % sizeof (D23_hpbufinfo)) != 0) ?
	       (rqsize / sizeof (D23_hpbufinfo)) + 1 :
		rqsize / sizeof (D23_hpbufinfo) ;
    rqbufmsk = bufmsk = (1 << rqbufno) - 1 ;

    /* allocate the requested heap buffer(s) */
    for (bufsptr = d23_info.d23_genarea->d23_hpstate, bufidx = 0;
			   bufidx < d23_info.d23_genarea->d23_mhpno; bufidx++) {
	if ((*bufsptr & bufmsk) == 0)
	    break ;
	if ((bufmsk & D23_HPMSKLM) == D23_HPMSKLM) {
	    /* go to the next buffer block */
	    bufmsk = rqbufmsk ;
	    bufsptr++ ;
	    bufidx |= D23_FLDSIZE - 1 ;
	} else
	    bufmsk <<= 1 ;
    }

    if (bufidx == d23_info.d23_genarea->d23_mhpno) {
	/* requested heap buffer(s) not available */
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
			       svc_c_sev_notice, GDS_S_IPCLIMHEAP_ERR, rqsize) ;
	return (D23_ERROR) ;
    }

    /* initialize allocated heap buffer(s) */
    bufptr = (D23_hpbufinfo *) ((byte *) d23_info.d23_hparea +
					    (sizeof (D23_hpbufinfo) * bufidx)) ;
    bufptr->d23_hpbufno = rqbufno ;
    *bufsptr |= bufmsk ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
       svc_c_debug1, GDS_S_CSSHMA_EXIT, (signed32) ((byte *) bufptr->d23_hpbuf -
                                              (byte *) d23_info.d23_hparea) )) ;
    return ((signed32) ((byte *) bufptr->d23_hpbuf -
						(byte *) d23_info.d23_hparea)) ;
}

/*exon ***********************************************************************/
/*                                                                           */
/*   TYPE:    function                                                       */
/*                                                                           */
/*   NAME:    d23_shmfree ()                                                 */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*       The function releases allocated memory from the heap. This is done  */
/*       by setting the appropriate no. of allocated heap buffers to a free  */
/*       state.                                                              */
/*                                                                           */
/*   INPUT-PARAMETER:                                                        */
/*       offset      = Offset to the beginning of the allocated memory       */
/*                     within the heap.                                      */
/*                                                                           */
/*   OUTPUT-PARAMETER:                                                       */
/*                     None                                                  */
/*                                                                           */
/*   RETURN-VALUE:                                                           */
/*       D23_SUCES   = The function has been performed successfully.         */
/*       D23_ERROR  =  An internal error is occurred during execution of     */
/*                     the function.                                         */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   AUTHOR: R.Horn                                        DATE: 17.5.91     */
/*exoff **********************************************************************/

signed32 d23_shmfree (signed32 offset) {

    signed32		bufidx, bufstate ;
    unsigned32		bufmsk ;
    D23_hpbufinfo	*bufptr ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				    svc_c_debug1, GDS_S_CSSHMF_ENTRY, offset)) ;

    /* verify heap buffer offset */
    offset -= sizeof (D23_hpbufinfo) - D23_HPBFSZ ;
    if (offset < 0 ||
	offset >= d23_info.d23_genarea->d23_mhpno * sizeof (D23_hpbufinfo) ||
				       (offset % sizeof (D23_hpbufinfo)) != 0) {
	/* invalid offset value */
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				svc_c_sev_fatal, GDS_S_IPCHEAPREL_ERR, offset) ;
	return (D23_ERROR) ;
    }

    /* get no. of allocated heap buffers */
    bufptr = (D23_hpbufinfo *) ((byte *) d23_info.d23_hparea + offset) ;
    bufidx = offset / sizeof (D23_hpbufinfo) ;
    bufstate = bufidx / D23_FLDSIZE ;
    if ((d23_info.d23_genarea->d23_hpstate[bufstate] &
					 (1 << (bufidx % D23_FLDSIZE))) == 0 ) {
	/* heap buffer(s) not allocated */
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_IPC,
				svc_c_sev_fatal, GDS_S_IPCHEAPREL_ERR, offset) ;
	return (D23_ERROR) ;
    }

    /* generate heap buffer mask */
    bufmsk = ((1 << bufptr->d23_hpbufno) - 1) << (bufidx % D23_FLDSIZE) ;

    /* set the allocated heap buffer(s) to the 'free'-state */
    d23_info.d23_genarea->d23_hpstate[bufstate] &= ~bufmsk ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_IPC,
					     svc_c_debug1, GDS_S_CSSHMF_EXIT)) ;
    return (D23_SUCES) ;
}
