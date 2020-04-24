/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22insobj.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:54  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:31  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:02  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:57  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:19  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:23  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:30  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:54:17  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:42:46  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:14:57  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:41:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:56  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:30:41  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:37:46  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22insobj.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:54 $";
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
/* NAME         : d22insobj.c                                         */
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
/*  FUNCTION:  d22_insert_obj ()                                             */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function inserts a directory object into the 2nd level cache.    */
/*      It looks for an empty hash table entry (which is required to store   */
/*      the directory object X) by generating a hash index from the given    */
/*      object name. The generation is done by using an appropriate hash     */
/*      function hp(X). If the found hash table entry isn't empty, but       */
/*      occupied by a directory object which has the same hash index         */
/*      (-> collision), the function searches sequentielly for an empty      */
/*      entry. This hash algorithm is also called 'open addressing by        */
/*      collision resolution through linear probing'. The predefined         */
/*      number of directory objects which are not referenced for the longest */
/*      time are deleted, if the hash table is full. The new directory       */
/*      object is stored in the 2nd level cache (in the DUA-cache files) and */
/*      also in the 1st level cache (in main memory).                        */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      stclass     = Storage class at which the directory object should     */
/*                    be stored into the DUA-cache.                          */
/*      admptr      = Pointer to the actual DUA-cache administration table.  */
/*      nameptr     = Pointer to the name of the directory object which      */
/*                    should be stored in the DUA-cache.                     */
/*      objptr      = Pointer to the attribute information of the directory  */
/*                    object.                                                */
/*      objsize     = Length of the attribute information block.             */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D22_OK      = The function has been performed successfully.          */
/*      D22_ERROR   = A system error or an internal error is occurred during */
/*                    operation (the variable 'errno' contains an error no.).*/
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 30.11.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <time.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>


/* ------------------------------------------------------------------------- */

signed32 d22_insert_obj (signed16 stclass, D22_admin_tab *admptr,
	     D2_name_string nameptr, D22_attr_entry *objptr, signed16 objsize) {

    signed32		hsh_index, result ;
    D22_hash_entry	*p ;
    off_t		offset ;
    boolean		del_occurred = FALSE ;

    /* look for a full hash table */
    switch ((int) stclass) {
    case D22_RESIDENT:
	if (admptr->d22_desctab->d22_ract_entries >=
					admptr->d22_desctab->d22_rmax_entries) {
	    /* remove the oldest directory objects assigned to */
	    /* the storage class RESIDENT from the DUA-cache */
	    if ((result = d22_mdelete_obj (D22_RESIDENT, admptr, FALSE)) ==
								      D22_ERROR)
		return (D22_ERROR) ;
	    del_occurred = TRUE ;
	}
	break ;
    case D22_PRIVILEGED:
	if (admptr->d22_desctab->d22_pact_entries >=
					admptr->d22_desctab->d22_pmax_entries) {
	    /* remove the oldest directory objects assigned to */
	    /* the storage class PRIVILEGED from the DUA-cache */
	    if ((result = d22_mdelete_obj (D22_PRIVILEGED, admptr, FALSE)) ==
								      D22_ERROR)
		return (D22_ERROR) ;
	    del_occurred = TRUE ;
	}
	break ;
    case D22_NORMAL:
	if (admptr->d22_desctab->d22_nact_entries >=
					admptr->d22_desctab->d22_nmax_entries) {
	    /* remove the oldest directory objects assigned to */
	    /* the storage class NORMAL from the DUA-cache */
	    if ((result = d22_mdelete_obj (D22_NORMAL, admptr, FALSE)) ==
								      D22_ERROR)
		return (D22_ERROR) ;
	    del_occurred = TRUE ;
	}
	break ;
    }

    /* generate hash index */
    hsh_index = d22_hash_index (nameptr, admptr->d22_desctab->d22_hmax_entries) ;

    /* verify for an empty entry and do linear probing if a collision */
    /* is detected */
    for (;;)
	if ((p = admptr->d22_hshtab + hsh_index)->d22_state == D22_NO_ENTRY)
	    break ;
	else
	    hsh_index = ++hsh_index % admptr->d22_desctab->d22_hmax_entries ;

    /* get file offset within DUA-cache attribute file for storing */
    /* object information */
    if (d22_getfileoffset (admptr, objsize, &offset) == D22_ERROR)
	return (D22_ERROR) ;

    /* write object information into DUA-cache attribute file */
	if (d22_cfwrite (admptr->d22_attr_fd, (char *) objptr, objsize,
							   offset) == D22_ERROR)
	    return (D22_ERROR) ;

    /* insert entry information into hash table entry */
    switch ((int) stclass) {
    case D22_RESIDENT:
	p->d22_state = D22_RESENTRY ; 
	break ;
    case D22_PRIVILEGED:
	p->d22_state = D22_PRIENTRY ; 
	break ;
    case D22_NORMAL:
	p->d22_state = D22_NORENTRY ; 
	break ;
    }
    p->d22_lreftime = time ((time_t) 0)/D22_TIMEBASE ;
    p->d22_obsize = objsize ;
    p->d22_oboffset = offset ;

    /* update DUA-cache hash file */
    if (del_occurred) {
	/* update complete DUA-cache hash file */
	if (d22_cfwrite (admptr->d22_hsh_fd, (void *) admptr->d22_hshtab,
	    (size_t) (admptr->d22_desctab->d22_hmax_entries *
			      sizeof (D22_hash_entry)), (off_t) 0) == D22_ERROR)
	    return (D22_ERROR) ;
    } else {
	/* update single DUA-cache hash file entry */	
	if (d22_cfwrite (admptr->d22_hsh_fd, (void *) p,
	    (size_t) sizeof (D22_hash_entry), (off_t) (hsh_index *
					 sizeof (D22_hash_entry))) == D22_ERROR)
	    return (D22_ERROR) ;
    }

    /* update DUA-cache description table and the corresponding file */
    switch ((int) stclass) {
    case D22_RESIDENT:
	admptr->d22_desctab->d22_ract_entries++ ;
	break ;
    case D22_PRIVILEGED:
	admptr->d22_desctab->d22_pact_entries++ ;
	break ;
    case D22_NORMAL:
	admptr->d22_desctab->d22_nact_entries++ ;
	break ;
    }
    /* update DUA-cache description file */
    if (d22_cfwrite (admptr->d22_desc_fd, (void *) admptr->d22_desctab,
			(size_t) sizeof (D22_desc_tab), (off_t) 0) == D22_ERROR)
	return (D22_ERROR) ;

    /* insert directory object into 1st level cache */
    d22_iinsert_obj (admptr, p, objptr) ;

    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
	 svc_c_debug2, GDS_S_CACHE_OBJ_INSERTED2, p->d22_lreftime, hsh_index)) ;
    return (D22_OK) ;
}
