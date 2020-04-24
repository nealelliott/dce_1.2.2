/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22mdelobj.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:56  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:32  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:03  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:59  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:20  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:26  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:34  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:54:36  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:44:26  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:15:17  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:41:38  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:14:04  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:30:58  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:37:55  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22mdelobj.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:55 $";
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
/* NAME         : d22mdelobj.c                                        */
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
/*  FUNCTION:  d22_mdelete_obj ()                                            */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function deletes the n directory objects of a given storage      */
/*      class from the hash table which are not referenced for the longest   */
/*      time. The number n is specific to each storage class and is          */
/*      contained in the DUA-cache description table. If n has the value     */
/*      zero (means no entry can be deleted), an error is reported, because  */
/*      no new directory object assigned to this storage class can be stored */
/*      in the DUA-cache.                                                    */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      stclass     = Storage class at which the deletion should be done.    */
/*      admptr      = Pointer to the actual DUA-cache administration table.  */
/*      fupdate     = Mode of update:                                        */
/*                    TRUE:    The DUA-cache description and the DUA-cache   */
/*                             hash file should be updated at the end of     */
/*                             the delete operation.                         */
/*                    FALSE:   No file update should be done.                */
/*                    Note: The DUA-cache reorganization file is always up-  */
/*                          dated.                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
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
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

signed32 d22_mdelete_obj (signed16 stclass, D22_admin_tab *admptr,
							      boolean fupdate) {
    signed16		i, index, fnd_cnt, del_cnt, act_cnt, del_index ;
    unsigned16		stc_type ;
    time_t		reftime ;
    D22_hash_entry	*p, *p1 ;

    /* examine no. of directory objects which can be deleted */
    switch ((int) stclass) {
    case D22_RESIDENT:
	del_cnt = admptr->d22_desctab->d22_rdel_entries ;
	act_cnt = admptr->d22_desctab->d22_ract_entries ;
	stc_type = D22_RESENTRY ;
	break ;
    case D22_PRIVILEGED:
	del_cnt = admptr->d22_desctab->d22_pdel_entries ;
	act_cnt = admptr->d22_desctab->d22_pact_entries ;
	stc_type = D22_PRIENTRY ;
	break ;
    case D22_NORMAL:
	del_cnt = admptr->d22_desctab->d22_ndel_entries ;
	act_cnt = admptr->d22_desctab->d22_nact_entries ;
	stc_type = D22_NORENTRY ;
	break ;
    }
    if (del_cnt == 0) {
	/* error -> hash table is full relative to this storage class */
	d22_errno = D22_HSHTABFULL_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"),
	   GDS_S_GENERAL, svc_c_sev_notice, GDS_S_CACHE_HSHFULL_ERR, stc_type) ;
	return (D22_ERROR) ;
    }	

    for (i = 0; i < del_cnt; i++, act_cnt--) {
	/* search and delete oldest directory object entries */
	reftime = (time_t) 0 ;
	for (index = fnd_cnt = 0, p = admptr->d22_hshtab; fnd_cnt < act_cnt;
								 p++, index++) {
	    if ((p->d22_state & D22_STCLMASK) != stc_type)
		continue ;
	    fnd_cnt++ ;
	    if (reftime == (time_t) 0 || reftime > p->d22_lreftime) {
		p1 = p ;
		del_index = index ;
		reftime = p->d22_lreftime ;
	    }
	}
	if (d22_delete_obj (stclass, admptr, p1, del_index, FALSE) == D22_ERROR)
	    return (D22_ERROR) ;
    }

    /* update DUA-cache files if required */
    if (fupdate) {
	/* update DUA-cache description file */
	if (d22_cfwrite (admptr->d22_desc_fd, (void *) admptr->d22_desctab,
			(size_t) sizeof (D22_desc_tab), (off_t) 0) == D22_ERROR)
	    return (D22_ERROR) ;

	/* update DUA-cache hash file */
	if (d22_cfwrite (admptr->d22_hsh_fd, (void *) admptr->d22_hshtab,
	    (size_t) (admptr->d22_desctab->d22_hmax_entries *
			      sizeof (D22_hash_entry)), (off_t) 0) == D22_ERROR)
	    return (D22_ERROR) ;	
    }
    return (D22_OK) ;
}
