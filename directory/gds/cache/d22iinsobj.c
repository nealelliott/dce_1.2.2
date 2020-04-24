/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22iinsobj.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:53  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:30  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:00  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:53  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:18  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:20  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:29  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:54:08  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:41:52  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:14:48  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:41:22  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:53  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:30:31  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:37:41  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22iinsobj.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:53 $";
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
/* NAME         : d22iinsobj.c                                        */
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
/*  FUNCTION:  d22_iinsert_obj ()                                            */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function inserts the informations about the given directory      */
/*      object into the 1st level cache. If the 1st level cache is full,     */
/*      the directory object which has not been referenced for the longest   */
/*      time is removed.                                                     */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      admptr      = Pointer to the DUA-cache administration information    */
/*                    table.                                                 */
/*      hshptr      = Pointer to the hash table entry of the directory       */
/*                    object of which the informations must be inserted in   */
/*                    the 1st level cache.                                   */
/*      objinfo     = pointer to the informations which must be inserted.    */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    NONE                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    NONE                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 26.11.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <time.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>


/* ------------------------------------------------------------------------- */

void d22_iinsert_obj (D22_admin_tab *admptr, D22_hash_entry *hshptr,
						      D22_attr_entry *objinfo) {

    D22_iobj_entry	*p = admptr->d22_iobjtab, *p1 ;
    signed16		i, act_index = 0 ;

    if (admptr->d22_iobjcnt == admptr->d22_desctab->d22_imax_entries) {
	/* 1st level cache is full - remove oldest entry */
	for (i = 1, p1 = p + 1; i < admptr->d22_desctab->d22_imax_entries;
								      i++, p1++)
	    if (p->d22_ilreftime < p1->d22_ilreftime) {
		p = p1 ;
		act_index = i ;
	    }
	free (p->d22_iattrptr) ;
	p->d22_hshptr->d22_state &= ~(D22_INSTORED | D22_IINDEXMASK) ;
	p->d22_ilreftime = D22_IZERO ;
	admptr->d22_iobjcnt-- ;	
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
		    svc_c_debug2, GDS_S_CACHE_OBJ_REMOVED1, p->d22_ilreftime)) ;
    } else {
	/* 1st level cache isn't full - look for an empty entry */
	for (i = 0; i < admptr->d22_desctab->d22_imax_entries; i++, p++) {
	    if (p->d22_ilreftime == D22_IZERO)
		break ;
	}
	act_index = i ;
    }
    /* insert new directory object into 1st level cache */	
    p->d22_ilreftime = time ((time_t) 0) ;
    p->d22_hshptr = hshptr ;
    p->d22_iattrptr = objinfo ;
    hshptr->d22_state |= (D22_INSTORED | act_index) ;
    admptr->d22_iobjcnt++ ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
	svc_c_debug2, GDS_S_CACHE_OBJ_INSERTED1, p->d22_ilreftime, act_index)) ;
}
