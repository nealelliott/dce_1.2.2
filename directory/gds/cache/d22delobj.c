/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22delobj.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:24  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:53  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:28  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:12  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:05  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:17  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:53:07  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:31:52  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:13:40  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:40:46  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:27  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:14:46  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:06:31  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22delobj.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:44 $";
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
/* NAME         : d22delobj.c                                         */
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
/*  FUNCTION:  d22_delete_obj ()                                             */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function deletes one directory object from the hash table and    */
/*      updates the complete hash table if collisions with other entries     */
/*      are detected.                                                        */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      stclass     = Storage class at which the deletion should be done.    */
/*      admptr      = Pointer to the actual DUA-cache administration table.  */
/*      hshptr      = Pointer to the hash table entry which should be        */
/*                    deleted.                                               */
/*      index       = Index of the hash table entry which should be deleted. */
/*      fupdate     = Mode of update:                                        */
/*                    TRUE:     The DUA-cache description and the DUA-cache  */
/*                              hash file should be updated at the end of    */
/*                              the delete-operation.                        */
/*                    FALSE:    No file update should be done.               */
/*                    Note: The DUA-cache reorganisation file is always      */
/*                          updated.                                         */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
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
#include <stdio.h>
#include <d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

signed32 d22_delete_obj (signed16 stclass, D22_admin_tab *admptr,
		      D22_hash_entry *hshptr, signed16 index, boolean fupdate) {

    D22_hash_entry	*phlp ;
    D22_fmem_area	f_area ;
    D22_iobj_entry	*pi ;
    D22_attr_entry	*pa ;
    signed16		cnt, hsh_index, del_index ;
    boolean		clear_entry = TRUE ;
    char		tmpdn[D2_DNL_MAX] ;

    /* save relevant entry informations */
    f_area.d22_frsize = hshptr->d22_obsize ;
    f_area.d22_froffset = hshptr->d22_oboffset ;

    if (hshptr->d22_state & D22_INSTORED) {
	/* remove object from 1st level cache */
	pi = admptr->d22_iobjtab + (hshptr->d22_state & D22_IINDEXMASK) ;
	DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
		   svc_c_debug2, GDS_S_CACHE_OBJ_REMOVED1, pi->d22_ilreftime)) ;
	free (pi->d22_iattrptr) ;
	pi->d22_ilreftime = D22_IZERO ;
	pi->d22_hshptr = (D22_hash_entry *) NULL ;
	pi->d22_iattrptr = (D22_attr_entry *) NULL ;	
	admptr->d22_iobjcnt-- ;
    }

    /* update DUA-cache hash table */
    for (;;) {
	if (clear_entry) {
	    /* delete hash table entry */
	    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
					svc_c_debug2, GDS_S_CACHE_OBJ_REMOVED2,
					hshptr->d22_lreftime, index)) ;
	    hshptr->d22_state = D22_NO_ENTRY ;
	    hshptr->d22_lreftime = (time_t) 0 ;
	    hshptr->d22_obsize = 0 ;
	    hshptr->d22_oboffset = (off_t) 0 ;
	    del_index = index ;
	    clear_entry = FALSE ;
	}
	/* look for collision entries and update if required */
	index = (index + 1) % admptr->d22_desctab->d22_hmax_entries ;
	hshptr = admptr->d22_hshtab + index ;
	if (hshptr->d22_state == D22_NO_ENTRY)
	    break ;

	/* read entry attribute information block */
	if ((pa = (D22_attr_entry *) malloc (hshptr->d22_obsize)) ==
						      (D22_attr_entry *) NULL) {
	    d22_errno = D22_NOMEM_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			svc_c_sev_warning, GDS_S_NOMEMORY, hshptr->d22_obsize) ;
	    return (D22_ERROR) ;
	}
	if ((cnt = d22_cfread (admptr->d22_attr_fd, (void *) pa,
	    (size_t) hshptr->d22_obsize, hshptr->d22_oboffset)) !=
							   hshptr->d22_obsize) {
	    if (cnt != D22_ERROR) {
		d22_errno = D22_INFSIZE_ERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"),
			GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_CACHE_INFO_SZ_ERR,
						      cnt, hshptr->d22_obsize) ;
	    }
	    return (D22_ERROR) ;
	}

	/* generate hash index from distinguished name */
	d22_convert_to_dn (tmpdn, pa, hshptr->d22_obsize) ;
	hsh_index = d22_hash_index ((D2_name_string) tmpdn,
					admptr->d22_desctab->d22_hmax_entries) ;

	free (pa) ;
	if (!((del_index < index && hsh_index <= index && hsh_index > del_index) ||
	    (del_index > index && (hsh_index > del_index || hsh_index <= index)))) {
	    /* collision detected -> transfer hash table entry */
	    phlp = admptr->d22_hshtab + del_index ;
	    *phlp = *hshptr ;
	    if (hshptr->d22_state & D22_INSTORED) {
		/* update reference to 1st level cache entry */
		pi = admptr->d22_iobjtab + (hshptr->d22_state & D22_IINDEXMASK) ;	
		pi->d22_hshptr = phlp ;
	    }
	    clear_entry = TRUE ;
	}
    }

    /* update DUA-cache description table */
    admptr->d22_desctab->d22_aact_areas++ ;
    admptr->d22_desctab->d22_fact_memsize += f_area.d22_frsize ;
    switch ((int) stclass) {
    case D22_RESIDENT:
	admptr->d22_desctab->d22_ract_entries-- ;
	break ;
    case D22_PRIVILEGED:
	admptr->d22_desctab->d22_pact_entries-- ;
	break ;
    case D22_NORMAL:
	admptr->d22_desctab->d22_nact_entries-- ;
	break ;
    }

    /* write information about the new free memory area into the DUA- */
    /* cache reorganization file */
    if (d22_cfwrite (admptr->d22_rorg_fd, (void *) &f_area,
		 (size_t) sizeof (D22_fmem_area), (off_t) D22_EOF) == D22_ERROR)
	return (D22_ERROR) ;

    if (admptr->d22_desctab->d22_fact_memsize >=
					admptr->d22_desctab->d22_fmax_memsize ||
	admptr->d22_desctab->d22_aact_areas >=
					    admptr->d22_desctab->d22_amax_areas)
	/* reorganize DUA-cache attribute file */
	if (d22_reorg_cache (admptr) == D22_ERROR)
	    return (D22_ERROR) ;

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
