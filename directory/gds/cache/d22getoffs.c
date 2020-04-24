/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22getoffs.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:51  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:29  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:59  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:48  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:17  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:16  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:25  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:53:51  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:40:11  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:14:29  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:41:13  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:46  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:30:08  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:37:31  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22getoffs.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:51 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d22getoffs.c                                        */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 10.05.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d22_getfileoffset ()                                          */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function searches a segment within the DUA-cache attribute       */
/*      file for storing a specific attribute information block and returns  */
/*      the file offset value of that segment. The required segment is       */
/*      allocated from one of the free memory areas described in the DUA-    */
/*      cache reorganization file. If this is impossible, the end of the     */
/*      DUA-cache attribute file is determined for this segment.             */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      admptr      = Pointer to the actual DUA-cache administration table.  */
/*      objsize     = Size of the attribute information block for which a    */
/*                    segment is required.                                   */
/*      offsetptr   = Pointer to a variable where the found offset value     */
/*                    should be returned.                                    */
/*  OUTPUT-PARAMETER:                                                        */
/*      offsetptr   = Offset value.                                          */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D22_OK      = The function has been performed successfully.          */
/*      D22_ERROR   = A system error or an internal error is occurred during */
/*                    operation ('d22_errno' contains an error no.).         */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE:  1.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

signed32 d22_getfileoffset (D22_admin_tab *admptr, signed16 objsize,
							     off_t *offsetptr) {

    signed32		i, cnt ;
    D22_fmem_area	*p ;

    if (admptr->d22_desctab->d22_aact_areas > 0) {
	/* read informations about free memory areas */
	if ((cnt = d22_cfread (admptr->d22_rorg_fd, (void *) admptr->d22_rorgtab,
			(size_t) ((admptr->d22_desctab->d22_aact_areas + 1) *
			sizeof (D22_fmem_area)), (off_t) 0)) !=
		 admptr->d22_desctab->d22_aact_areas * sizeof (D22_fmem_area)) {
	    if (cnt != D22_ERROR) {
		/* error -> wrong size of DUA-cache reorganization file */
		d22_errno = D22_FLSIZE_ERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_fatal, GDS_S_CACHE_REORG_SZ_ERR, cnt,
				admptr->d22_desctab->d22_aact_areas *
						       sizeof (D22_fmem_area)) ;
	    }				
	    return (D22_ERROR) ;
	}
	/* search for an appropriate segment */
	cnt /= sizeof (D22_fmem_area) ;
	for (p = admptr->d22_rorgtab, i = 0; i < cnt; i++, p++) {
	    if (p->d22_frsize >= objsize) {
		/* segment found -> update DUA-cache reorganization file */
		p->d22_frsize -= objsize ;
		*offsetptr = p->d22_froffset ;
		p->d22_froffset = *offsetptr + objsize ; 
		if (d22_cfwrite (admptr->d22_rorg_fd, (void *) p,
		    (size_t) sizeof (D22_fmem_area), (off_t) (i *
					  sizeof (D22_fmem_area))) == D22_ERROR)
		    return (D22_ERROR) ;
		/* update DUA-cache description table */
		admptr->d22_desctab->d22_fact_memsize -= objsize ;
		return (D22_OK) ;
	    }
	}
    }

    /* no free memory areas available or no appropriate segment found - */
    /* take end of DUA-cache attribute file */
    if ((*offsetptr = d22_cflseek (admptr->d22_attr_fd, (off_t) 0,
						       D22_ESEEK)) == D22_ERROR)
	return (D22_ERROR) ;
    return (D22_OK) ;
}
