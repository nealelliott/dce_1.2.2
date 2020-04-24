/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22reorg.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:58  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:33  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:04  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:14:03  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:21  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:29  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:35  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:54:45  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:45:18  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:15:26  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:41:43  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:14:08  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:31:08  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:38:03  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22reorg.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:56 $";
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
/* NAME         : d22reorg.c                                          */
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
/*  FUNCTION:  d22_reorg_cache ()                                            */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function reorganizes the DUA-cache attribute file by using the   */
/*      informations contained in the DUA-cache reorganization file. The     */
/*      reorganization is done by moving all valid information blocks from   */
/*      the DUA-cache attribute file to a temporary file (all invalid in-    */
/*      formation blocks are removed on this way). Then the temporary file   */
/*      becomes the new DUA-cache attribute file after deleting the old      */
/*      one and the informations of the DUA-cache reorganization file are    */
/*      released. Note: Updates are made on the DUA-cache hash table and on  */
/*      the DUA-cache description table but not on the corresponding DUA-    */
/*      cache files.                                                         */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      admptr      = Pointer to the actual DUA-cache administration table.  */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D22_OK      = The function has been performed successfully.          */
/*      D22_ERROR   = An system error or an internal error is ocurred during */
/*                    operation ('d22_errno' contains an error no.).         */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE:  9.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

signed32 d22_reorg_cache (D22_admin_tab *admptr) {

    off_t		start_offset, end_offset, eof_offset ;
    off_t		obj_offset, act_size ;
    int			tmp_fd ;
    signed32		cnt ;
    char		*buf, cname[D27_LFILE_NAME] ;
    char		ctname[D27_LFILE_NAME] ;
    signed16		fr_cnt = admptr->d22_desctab->d22_aact_areas ;
    D22_fmem_area	tmp_buf ;
    D22_fmem_area	*p, *p1, *ph ;
    D22_hash_entry	*p2, *pmax = admptr->d22_hshtab +
					 admptr->d22_desctab->d22_hmax_entries ;

    /* read free memory area information from DUA-cache reorganization file */
    if ((cnt = d22_cfread (admptr->d22_rorg_fd, (void *) admptr->d22_rorgtab,
	(size_t) ((fr_cnt + 1) * sizeof (D22_fmem_area)), (off_t) 0)) !=
					      fr_cnt * sizeof (D22_fmem_area)) {
	if (cnt != D22_ERROR) {
	    /* error -> wrong size of the DUA-cache reorganization file */
	    d22_errno = D22_FLSIZE_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_fatal, GDS_S_CACHE_REORG_SZ_ERR, cnt,
				fr_cnt * sizeof (D22_fmem_area)) ;
	}
	return (D22_ERROR) ;
    }

    /* sort free memory area information (ascending order of offsets) */
    for (p = admptr->d22_rorgtab; p < admptr->d22_rorgtab + fr_cnt ; p++) {
	for (ph = p, p1 = p + 1; p1 < admptr->d22_rorgtab + fr_cnt; p1++)
	    if (p1->d22_froffset < ph->d22_froffset)
		ph = p1 ;
	if (ph != p) {
	    tmp_buf = *p ;
	    *p = *ph ;
	    *ph = tmp_buf ;
	}
    }

    /* create temporary DUA-cache attribute file */
    sprintf (ctname, "%s%s%d%s", admptr->d22_conftab->d22_base_path,
		      D22_CACHEDIR, admptr->d22_act_cache_id, D22_TMPATTRFILE) ;
    if ((tmp_fd = d22_cfopen (ctname,
			 O_RDWR | O_SYNC | O_CREAT, D22_FILEMODE)) == D22_ERROR)
	return (D22_ERROR) ;

    /* transfer  valid information blocks contained in the DUA-cache */
    /* attribute file through a buffer into the temporary file for */
    /* removing invalid information blocks */

    /* allocate memory for transfer buffer */
    if ((buf = (char *) malloc (D22_TRANSSIZE)) == (char *) NULL) {
	d22_errno = D22_NOMEM_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
			     svc_c_sev_warning, GDS_S_NOMEMORY, D22_TRANSSIZE) ;
	return (D22_ERROR) ;
    }
    /* determine the size of the DUA-cache attribute file */
    if ((eof_offset = d22_cflseek (admptr->d22_attr_fd, (off_t) 0, D22_ESEEK)) ==
								      D22_ERROR)
	return (D22_ERROR) ;
    p = admptr->d22_rorgtab ;
    start_offset = act_size = 0L ;

    do {
	end_offset = (p < admptr->d22_rorgtab + fr_cnt) ?
						  p->d22_froffset : eof_offset ;
	if (start_offset < end_offset) {
	    if (act_size > 0L) {
		/* update the offset of relevant DUA-cache hash table entries */
		for (p2 = admptr->d22_hshtab; p2 < pmax; p2++)
		    if (p2->d22_state != D22_NO_ENTRY && (obj_offset =
			p2->d22_oboffset) >= start_offset &&
				    			obj_offset < end_offset)
			p2->d22_oboffset = obj_offset - act_size ;
	    }
	    while (start_offset < end_offset) {
		/* transfer a valid information block */
		cnt = (end_offset - start_offset > D22_TRANSSIZE) ?
			     D22_TRANSSIZE : (int) (end_offset - start_offset) ;
		if ((cnt = d22_cfread (admptr->d22_attr_fd, (void *) buf,
				    (size_t) cnt, start_offset)) == D22_ERROR) {
		    free (buf) ;
		    return (D22_ERROR) ;
		}
		if (d22_cfwrite (tmp_fd, (void *) buf, (size_t) cnt,
						(off_t) D22_EOF) == D22_ERROR) {
		    free (buf) ;
		    return (D22_ERROR) ;
		}
		start_offset += cnt ;
	    }
	}
	/* note that in some circumstances the additional and invalid */
	/* entry of the DUA-reorganization table is referenced also */
	start_offset += p->d22_frsize ;
	act_size += p++->d22_frsize ;
    } while (start_offset < eof_offset) ;
    free (buf) ;

    /* clear all informations contained in the DUA-cache reorganisation file */
    d22_cfclose (admptr->d22_rorg_fd) ;
    sprintf (cname, "%s%s%d%s", admptr->d22_conftab->d22_base_path,
			D22_CACHEDIR, admptr->d22_act_cache_id, D22_REORGFILE) ;
    if ((admptr->d22_rorg_fd = d22_cfopen (cname,
			 O_RDWR | O_SYNC | O_TRUNC, D22_FILEMODE)) == D22_ERROR)
	return (D22_ERROR) ;

    /* update DUA-cache description table */
    admptr->d22_desctab->d22_fact_memsize = 0 ;
    admptr->d22_desctab->d22_aact_areas = 0 ;

    /* delete the former DUA-cache attribute file and rename the */
    /* temporary file */
    if (d22_cfclose (admptr->d22_attr_fd) == D22_ERROR)
	return (D22_ERROR) ;
    if (d22_cfclose (tmp_fd) == D22_ERROR)
	return (D22_ERROR) ;

    sprintf (cname, "%s%s%d%s", admptr->d22_conftab->d22_base_path,
			 D22_CACHEDIR, admptr->d22_act_cache_id, D22_ATTRFILE) ;
    if (d22_cfunlink (cname) == D22_ERROR)
	return (D22_ERROR) ;
    if (d22_cflink (ctname, cname) == D22_ERROR)
	return (D22_ERROR) ;
    if (d22_cfunlink (ctname) == D22_ERROR)
	return (D22_ERROR) ;
    if ((admptr->d22_attr_fd = d22_cfopen (cname,
				   O_RDWR | O_SYNC, D22_FILEMODE)) == D22_ERROR)
	return (D22_ERROR) ;

    return (D22_OK) ;
}
