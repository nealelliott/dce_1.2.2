/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22srchobj.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:59  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:28:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:34  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:06  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:14:09  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:24  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:32  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:36  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:54:55  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:46:11  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:15:36  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:41:51  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:14:12  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:31:17  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:38:08  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22srchobj.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:58 $";
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
/* NAME         : d22srchobj.c                                        */
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
/*  FUNCTION:  d22_search_obj ()                                             */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function searches in the DUA-cache hash table for a given        */
/*      directory object. If the object is found in the 2nd level cache,     */
/*      then it is also inserted into the 1st level cache (if specified      */
/*      by parameter 'store_int' and if not already available).              */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      admptr      = Pointer to the actual DUA-cache administration table.  */
/*      nameptr     = Pointer to the distinguished name of the directory     */
/*                    object which should be searched.                       */
/*      hshptr      = Address of a variable where the pointer of the hash    */
/*                    table entry which matches to the searched directory    */
/*                    object can be stored.                                  */
/*      index       = Address of a variable where the index of the hash      */
/*                    table entry which matches to the searched directory    */
/*                    object can be stored.                                  */
/*      store_int   = Store in 1st level cache-Flag                          */
/*                    TRUE    =  Store the found directory object in the     */
/*                               1st level cache.                            */
/*                    FALSE    = Don't store the found directory object in   */
/*                               the 1st level cache.                        */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      *hshptr     = Pointer to the found hash table entry.                 */
/*      *index      = Index of the found hash table entry.                   */
/*                    Notes: All of this output parameters are only relevant */
/*                    if the result of the operation is D22_OK.              */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D22_OK      = The function has been performed successfully.          */
/*      D22_ERROR   = A system error or an internal error is occurred during */
/*                    operation ('d22_errno' contains an error no.).         */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE:  8.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

signed32 d22_search_obj (D22_admin_tab *admptr, D2_name_string nameptr,
		  D22_hash_entry **hshptr, signed16 *index, boolean store_int) {

    signed16		hsh_index, cnt ;
    D22_hash_entry	*p ;
    D22_attr_entry	*p1 ;
    char		tmpdn[D2_DNL_MAX] ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%s"), GDS_S_GENERAL,
			       svc_c_debug2, GDS_S_CACHE_SOBJ_ENTRY, nameptr)) ;

    /* determine hash index of the directory object */
    hsh_index = d22_hash_index (nameptr, admptr->d22_desctab->d22_hmax_entries) ;
    for (;;) {
	p = admptr->d22_hshtab + hsh_index ;
	if (p->d22_state == D22_NO_ENTRY)
	    break ;
	if ((p->d22_state & D22_INSTORED) == D22_INSTORED) {
	    p1 = (admptr->d22_iobjtab +
				(p->d22_state & D22_IINDEXMASK))->d22_iattrptr ;
	    /* convert naming attributes into a distinguished name string */
	    d22_convert_to_dn (tmpdn, p1, p->d22_obsize) ;
	    /* compare distinguished names */
	    if (d22_cignore_comp ((char *) nameptr, tmpdn)) {
		*hshptr = p ;
		*index = hsh_index ;
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_debug2, GDS_S_CACHE_SOBJ_EXIT, *index,
				(p->d22_state & D22_IINDEXMASK))) ;
		return (D22_OK) ;
	    }
	} else {
	    /* attribute information not internally available -> */
	    /* read from DUA-cache attribute file */
	    if ((p1 = (D22_attr_entry *) malloc (p->d22_obsize)) ==
						      (D22_attr_entry *) NULL) {
		d22_errno = D22_NOMEM_ERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_NOMEMORY, p->d22_obsize) ;
		return (D22_ERROR) ;
	    }
	    if ((cnt = d22_cfread (admptr->d22_attr_fd, (void *) p1,
	    	   (size_t) p->d22_obsize, p->d22_oboffset)) != p->d22_obsize) {
		if (cnt != D22_ERROR) {
		    d22_errno = D22_INFSIZE_ERR ;
		    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"),
			GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_CACHE_INFO_SZ_ERR,
							   cnt, p->d22_obsize) ;
		}
		return (D22_ERROR) ;
	    }
	    /* convert naming attributes into a distinguished name string */
	    d22_convert_to_dn (tmpdn, p1, p->d22_obsize) ;
	    /* compare distinguished names */
	    if (d22_cignore_comp ((char *) nameptr, tmpdn)) {
		/* directory object found - insert into 1st level cache */
		/* if required */
		if (store_int)
		    d22_iinsert_obj (admptr, p, p1) ;
		else
		    /* release attribute information block */
		    free (p1) ;
		*hshptr = p ;
		*index = hsh_index ;
		DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				svc_c_debug2, GDS_S_CACHE_SOBJ_EXIT1, *index)) ;
		return (D22_OK) ;
	    }
	    /* release attribute information block */
	    free (p1) ;
	}
	hsh_index = ++hsh_index % admptr->d22_desctab->d22_hmax_entries ;
    }
    /* error -> directory object doesn't exist */
    d22_errno = D22_NOEXOBJ_ERR ;
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
					svc_c_debug2, GDS_S_CACHE_SOBJ_EXIT2)) ;
    return (D22_ERROR) ;
}
