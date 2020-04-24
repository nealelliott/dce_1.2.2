/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22cadd.c,v $
 * Revision 1.1.12.3  1996/02/18  19:43:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:25  marty]
 *
 * Revision 1.1.12.2  1995/12/08  15:26:25  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/04/26  19:41 UTC  jrr
 * 	Include errno.h for AIX.
 * 	[1995/12/08  14:54:13  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:40  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:12:35  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:50:55  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:28  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:52:24  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:51:15  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:19:45  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:11:31  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:39:53  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:12:41  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:06:46  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  16:27:20  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:13:09  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:05:32  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22cadd.c,v $ $Revision: 1.1.12.3 $ $Date: 1996/02/18 19:43:29 $";
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
/* NAME         : d22cadd.c                                           */
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
/*  FUNCTION:  d22_cadd ()                                                   */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function inserts a given directory object into the DUA-cache.    */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      admptr      = Pointer to the actual DUA-cache administration table.  */
/*      rqptr       = Pointer to the request parameter block.                */
/*      rsptr       = Pointer to the result parameter block which must be    */
/*                    used to return a result or error message.              */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      D22_OK      = The function has been performed successfully.          */
/*      D22_ERROR   = A system error or an internal error is occurred during */
/*                    execution of the function (the variable 'd22_errno'    */
/*                    contains an error no.).                                */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 18.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d23apdu.h>
#include <d22cache.h>

/* -------------------- declaration of global data ------------------------- */

extern signed32	d22_errno ;	/* error variable */

/* ------------------------------------------------------------------------- */

signed32 d22_cadd (D22_admin_tab *admptr, D23_wtinpb *rqptr, D23_rsrqpb *rsptr) {

    D23_inaddobjt	*pbptr ;
    D23_com_arg		*comptr ;
    D23_ds_name		*nmptr ;
    D23_at_type		*atptr,	*attrptr[D2_AT_MAX] ;
    D23_av_info		*aviptr ;
    D23_av_value	*avptr ;
    D22_hash_entry	*hshptr = (D22_hash_entry *) NULL ;
    D22_iobj_entry	*ioptr ;
    D22_attr_entry	*actaptr, *iactaptr, *imaxptr ;
    D22_fmem_area	frmem ;
    D22_naming_ref	idnref ;
    D22_size_info	*hptr ;
    off_t		offset ;
    signed16		i, j, index, stclass, avval_no ;
    signed16		size, insattr_no = 0, actrqsize = 0 ;
    byte		*maxpbptr, *pbstart ;
    char		dname[D2_DNL_MAX] ;


    /* allocate memory to hold the request buffer informations */
    if ((pbstart = (byte *) malloc ((size_t) (D22_RQMAXSIZE + D2_NP_MAX *
         (sizeof (D22_attr_entry) + D2_ALIGN (D2_NP_LEN))))) == (byte *) NULL) {
	errno = D22_NOMEM_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
		svc_c_sev_warning, GDS_S_NOMEMORY, D22_RQMAXSIZE +
		D2_NP_MAX * (sizeof (D22_attr_entry) + D2_ALIGN (D2_NP_LEN))) ;
	return (D22_ERROR) ;
    }
    pbptr = (D23_inaddobjt *) (pbstart + D2_NP_MAX * (sizeof (D22_attr_entry) +
							D2_ALIGN (D2_NP_LEN))) ;
    do {
	if (actrqsize > 0) {
	    /* get next message */
	    if (d23_waitin (rqptr) == D23_ERROR) {
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""),
		       GDS_S_GENERAL, svc_c_sev_fatal, GDS_S_CACHE_WAITIN_ERR) ;
		exit (1) ;
	    }
	    if (rqptr->d23_2oprid == D23_ABORT) {
		free (pbstart) ;
		return (D22_SPEC_ERROR) ;
	    }
	}

	if (rqptr->d23_2lnidt + actrqsize > D22_RQMAXSIZE) {
	    /* error -> maximal request message size exceeded */
	    free (pbstart) ;
	    errno = D22_MSGSIZE_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_REQSZ_ERR,
				rqptr->d23_2lnidt + actrqsize, D22_RQMAXSIZE) ;
	    return (D22_ERROR) ;
	}
	/* transfer invoke message fragment into request buffer */
	memcpy ((void *) ((char *) pbptr + actrqsize),
		       (void *) rqptr->d23_2rfidt, (size_t) rqptr->d23_2lnidt) ;
	actrqsize += rqptr->d23_2lnidt ;
    } while (rqptr->d23_2imdat == D23_MRDATA) ;

    /* verify request parameter */
    maxpbptr = (byte *) pbptr + actrqsize ;
    if (d22_check_par (pbptr->d23_Mhdr.d23_versno, pbptr->d23_Mhdr.d23_dirid,
		  (D23_ds_name *) NULL, (D22_naming_ref *) NULL) == D22_ERROR) {
	free (pbstart) ;
	return (D22_ERROR) ;
    }

    /* examine storage class */
    comptr = (D23_com_arg *) (pbptr + 1) ;
    if (comptr->d23_cmtag != D23_CMTAG) {
	free (pbstart) ;
	errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				comptr->d23_cmtag, D23_CMTAG) ;
	return (D22_ERROR) ;
    }
    if ((comptr->d23_cmctr.d23_scopt & D2_NORMAL_CLASS) == D2_NORMAL_CLASS)
    	stclass = D22_NORMAL ;
    else {
	if ((comptr->d23_cmctr.d23_scopt & D2_PRIVILEGED_CLASS) ==
							    D2_PRIVILEGED_CLASS)
	    stclass = D22_PRIVILEGED ;
	else {
	    if ((comptr->d23_cmctr.d23_scopt & D2_RESIDENT_CLASS) ==
							      D2_RESIDENT_CLASS)
		stclass = D22_RESIDENT ;
	    else {
		free (pbstart) ;
		d22_errno = D22_MSYNTAX_ERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVSTC_ERR,
				comptr->d23_cmctr.d23_scopt) ;
		return (D22_ERROR) ;
	    }
	}
    }

    /* examine and verify object name */
    /* (but first skip irrelevant common argument informations) */
    nmptr = (D23_ds_name *) (comptr + 1) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_SECTAG, D23_S_SEP, 0) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_RQTAG, D23_S_NM, nmptr->d23_dsoff) ;
    D22_SKIPTAG (D23_ds_name, nmptr, D23_BITTAG, D23_S_BIT,
					((D23_bit_info *)nmptr)->d23_boff) ;

    if (nmptr->d23_dstag != D23_NAMTAG) {
	free (pbstart) ;
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				nmptr->d23_dstag, D23_NAMTAG) ;
	return (D22_ERROR) ;
    }
	
    idnref.d22_nmaptr = (D22_attr_entry *) pbstart ;
    if (d22_verify_dn ((D2_name_string) (nmptr + 1), &idnref) == D22_ERROR) {
	free (pbstart) ;
	return (D22_ERROR) ;
    }
    strcpy ((char *) dname, (char *) (nmptr + 1)) ;

    /* verify attribute information */
    atptr = (D23_at_type *) ((char *) nmptr + D23_S_NM + nmptr->d23_dsoff) ;
    while (atptr->d23_atttag == D23_ATTTAG && atptr < (D23_at_type *) maxpbptr &&
				  (idnref.d22_nonp + insattr_no) <= D2_AT_MAX) {
	if (atptr->d23_atlen < 1 || atptr->d23_atlen > D2_OBJ_IDL_MAX) {
	    /* error -> max. size of attribute type value exceeded */
	    free (pbstart) ;
	    d22_errno = D22_ATTSIZE_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVOIDLEN_ERR,
				atptr->d23_atlen, D2_OBJ_IDL_MAX) ;
	    return (D22_ERROR) ;
	}
	aviptr = (D23_av_info *) ((char *) atptr + D23_S_ATT + atptr->d23_atoff) ;
	if (aviptr->d23_avtag != D23_AVTAG) {
	    free (pbstart) ;
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				aviptr->d23_avtag, D23_AVTAG) ;
	    return (D22_ERROR) ;
	}
	avptr = (D23_av_value *) (aviptr + 1) ;	

	/* verify attribut value(s) */
	avval_no = 0 ;
	while (*(unsigned16 *) avptr < D23_MINTAG &&
					    avptr < (D23_av_value *) maxpbptr) {
	    if (avptr->d23_avlen < 1 || avptr->d23_avlen > D2_ATL_MAX) {
		/* error -> max. size of attribute value exceeded */
		free (pbstart) ;
		d22_errno = D22_ATTSIZE_ERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"),
				GDS_S_GENERAL, svc_c_sev_warning,
				GDS_S_CACHE_INVAVLEN_ERR,
				avptr->d23_avlen, D2_ATL_MAX) ;
		return (D22_ERROR) ;
	    }
	    avval_no++ ;
	    avptr = (D23_av_value *) ((char *) avptr + D23_S_ATV +
							     avptr->d23_avoff) ;
	}
	if (avval_no > (signed16) aviptr->d23_avnum) {
	    free (pbstart) ;
	    d22_errno = D22_MSYNTAX_ERR ;
	    dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				  svc_c_sev_warning, GDS_S_CACHE_INVATTRS_ERR) ;
	    return (D22_ERROR) ;
	}
	aviptr->d23_avnum = avval_no ;
	attrptr[insattr_no++] = atptr ;
	atptr = (D23_at_type *) avptr ;
    }
    if ((idnref.d22_nonp + insattr_no) > D2_AT_MAX) {
	/* error -> min. or max. number of attributes exceeded */	
	free (pbstart) ;
	d22_errno = D22_NOATTR_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				  svc_c_sev_warning, GDS_S_CACHE_INVATTRS_ERR) ;
	return (D22_ERROR) ;
    }
    if (((D23_eom *) atptr)->d23_eomtag != D23_EOMTAG) {
	free (pbstart) ;
	d22_errno = D22_MSYNTAX_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"), GDS_S_GENERAL,
				svc_c_sev_warning, GDS_S_CACHE_INVMSGTAG_ERR,
				((D23_eom *) atptr)->d23_eomtag, D23_EOMTAG) ;
	return (D22_ERROR) ;
    }

    /* verify attributes types */
    for (i = 0; i < insattr_no; i++)
	for (j = i + 1; j < insattr_no; j++)
	    if (attrptr[i]->d23_atlen == attrptr[j]->d23_atlen &&
		memcmp ((void *) (attrptr[i] + 1), (void *) (attrptr[j] + 1),
					 (size_t) attrptr[i]->d23_atlen) == 0) {
		/* error -> attribute type multiple available */
		free (pbstart) ;
		d22_errno = D22_MUATTR_ERR ;
		dce_svc_printf (DCE_SVC (gds_svc_handle, ""), GDS_S_GENERAL,
				  svc_c_sev_warning, GDS_S_CACHE_INVATTRS_ERR) ;
		return (D22_ERROR) ;
	    }
	
    /* activate specific DUA-cache if required */
    if (pbptr->d23_Mhdr.d23_dirid != admptr->d22_act_cache_id &&
	d22_activate_cache (pbptr->d23_Mhdr.d23_dirid, admptr) == D22_ERROR) {
	free (pbstart) ;
	return (D22_ERROR) ;
    }

    /* search directory object */
    if (d22_search_obj (admptr, (D2_name_string) dname, &hshptr, &index,
							   TRUE) == D22_ERROR) {
	if (d22_errno != D22_NOEXOBJ_ERR) {
	    free (pbstart) ;
	    return (D22_ERROR) ;
	}
    }

    /* generate directory object information block using the request buffer */
    /* which contains already the naming attributes */

    actaptr = (D22_attr_entry *) (pbstart + idnref.d22_nmsize) ;
    for (i = 0; i < insattr_no; i++) {
	/* store attribute type and syntax */
	actaptr->d22_aetype = 0 ;
	actaptr->d22_aidsize = attrptr[i]->d23_atlen ;
	memset ((void *) actaptr->d22_aident, (int) 0,
						      (size_t) D2_OBJ_IDL_MAX) ;
	memcpy ((void *) actaptr->d22_aident, (void *) (attrptr[i] + 1),
					       (size_t) attrptr[i]->d23_atlen) ;

	avptr = (D23_av_value *) ((char *) attrptr[i] + D23_S_ATT +
							attrptr[i]->d23_atoff) ;
	actaptr->d22_aidsyn = ((D23_av_info *) avptr)->d23_asyntx ;
	actaptr->d22_avalno = avval_no = ((D23_av_info *) avptr)->d23_avnum ;
	avptr = (D23_av_value *) ((char *) avptr + D23_S_AV) ;

	/* store attribute value(s) */
	if (avval_no > 1) {
	    /* recurring attribute */
	    for (hptr = (D22_size_info *) (actaptr + 1), j = 0; j < avval_no;
									  j++) {
		hptr->d22_voff = avptr->d23_avoff ;
		hptr->d22_vlen = avptr->d23_avlen ;
		hptr->d22_vrep = avptr->d23_avrep ;
		memcpy ((void *) (hptr + 1), (void *) (avptr + 1),
						    (size_t) avptr->d23_avlen) ;
		hptr = (D22_size_info *) ((char *) hptr +
				      sizeof (D22_size_info) + hptr->d22_voff) ;
		avptr = (D23_av_value *) ((char *) avptr +
						 D23_S_ATV + avptr->d23_avoff) ;
	    }
	    actaptr->d22_aetype = D22_REC_FLAG ;
	    actaptr->d22_asizes.d22_voff = (signed16) ((char *) hptr -
						       (char *) (actaptr + 1)) ;
	    actaptr = (D22_attr_entry *) hptr ;
	} else {
	    actaptr->d22_asizes.d22_voff = avptr->d23_avoff ;
	    actaptr->d22_asizes.d22_vlen = avptr->d23_avlen ;
	    actaptr->d22_asizes.d22_vrep = avptr->d23_avrep ;
	    memcpy ((void *) (actaptr + 1), (void *) (avptr + 1),
						    (size_t) avptr->d23_avlen) ;
	    actaptr = (D22_attr_entry *) ((char *) actaptr +
		       sizeof (D22_attr_entry) + actaptr->d22_asizes.d22_voff) ;
	}
    }

    if (hshptr != (D22_hash_entry *) NULL) {
	/* directory object exists in DUA-cache - existing */
	/* attributes are accepted at the new information block */
	/* if not available there */
	ioptr = admptr->d22_iobjtab + (hshptr->d22_state & D22_IINDEXMASK) ;
	iactaptr = ioptr-> d22_iattrptr ;
	imaxptr = (D22_attr_entry *) ((char *) iactaptr + hshptr->d22_obsize) ;
	/* Note: the first attributes are always the naming attributes */
	/* which are skipped */
	while ((iactaptr = (D22_attr_entry *) ((char *) iactaptr +
	  sizeof (D22_attr_entry) + iactaptr->d22_asizes.d22_voff)) < imaxptr) {
	    if ((iactaptr->d22_aetype & D22_NAME_AFLAG) == D22_NAME_AFLAG)
		continue ;
	    for (i = 0; i < insattr_no; i++)
		if (attrptr[i]->d23_atlen == iactaptr->d22_aidsize &&
		    memcmp ((void *) (attrptr[i] + 1),
			    (void *) (iactaptr->d22_aident),
					   (size_t) iactaptr->d22_aidsize) == 0)
		    break ;
	    if (i == insattr_no) {
		/* attribute not available - copy it into the */
		/* new directory object information block */
		size = sizeof (D22_attr_entry) + iactaptr->d22_asizes.d22_voff ;
		if ((signed16) ((byte *) actaptr - pbstart) + size >
								D22_RQMAXSIZE) {
		    /* error -> max. information block size exceeded */
		    free (pbstart) ;
		    d22_errno = D22_INFSIZE_ERR ;
		    dce_svc_printf (DCE_SVC (gds_svc_handle, "%d%d"),
			GDS_S_GENERAL, svc_c_sev_warning, GDS_S_CACHE_INFSZ_ERR,
			((byte *) actaptr - pbstart) + size, D22_RQMAXSIZE) ;
		    return (D22_ERROR) ;
		}
		memcpy ((void *) actaptr, (void *) iactaptr, (size_t) size) ;
		actaptr = (D22_attr_entry *) ((char *) actaptr + size) ;
	    }
	}
    }

    /* get size of new directory object information block and release */
    /* unused memory */
    size = (signed16) ((char *) actaptr - (char *) pbstart) ;
    if ((actaptr = (D22_attr_entry *) realloc ((void *) pbstart,
				   (size_t) size)) == (D22_attr_entry *) NULL) {
	d22_errno = D22_NOMEM_ERR ;
	dce_svc_printf (DCE_SVC (gds_svc_handle, "%d"), GDS_S_GENERAL,
				      svc_c_sev_warning, GDS_S_NOMEMORY, size) ;
	return (D22_ERROR) ;
    }

    if (hshptr == (D22_hash_entry *) NULL) {
	/* insert new directory object into DUA-cache */
	if (d22_insert_obj (stclass, admptr, (D2_name_string) dname, actaptr,
							   size) == D22_ERROR) {
	    free (actaptr) ;
	    return (D22_ERROR) ;
	}
    } else {
	/* directory object already exists - update DUA-cache information */
	/* update 'internally stored'-table entry */
	free (ioptr->d22_iattrptr) ;
	ioptr->d22_iattrptr = actaptr ;
	ioptr->d22_ilreftime = time ((time_t) 0) ;

	/* Note: From this point all occurring errors are system errors */
	/* which are handled as fatal errors -> thus, allocated memory */
	/* isn't released explicitly */

	/* release old attribute information block */
	admptr->d22_desctab->d22_aact_areas++ ;
	admptr->d22_desctab->d22_fact_memsize += hshptr->d22_obsize ;
	frmem.d22_frsize = hshptr->d22_obsize ;
	frmem.d22_froffset = hshptr->d22_oboffset ;

	/* update DUA-cache reorganisation file */
	if (d22_cfwrite (admptr->d22_rorg_fd, (char *) &frmem,
				  sizeof (D22_fmem_area), D22_EOF) == D22_ERROR)
	    return (D22_ERROR) ;

	/* get offset for new directory object information block */
	if (d22_getfileoffset (admptr, size, &offset) == D22_ERROR)
	    return (D22_ERROR) ;
	
	/* store directory object information block in DUA-cache attribute file */
	if (d22_cfwrite (admptr->d22_attr_fd, (void *) actaptr, (size_t) size,
						           offset) == D22_ERROR)
	    return (D22_ERROR) ;

	/* update DUA-cache hash table */
	hshptr->d22_obsize = size ;
	hshptr->d22_lreftime = ioptr->d22_ilreftime/D22_TIMEBASE ;
	hshptr->d22_oboffset = offset ;

	/* update DUA-cache if required */
	if (admptr->d22_desctab->d22_aact_areas >=
					  admptr->d22_desctab->d22_amax_areas ||
	    admptr->d22_desctab->d22_fact_memsize >=
					admptr->d22_desctab->d22_fmax_memsize) {
	    if (d22_reorg_cache (admptr) == D22_ERROR)
		return (D22_ERROR) ;

	    /* update complete DUA-cache hash file */
	    if (d22_cfwrite (admptr->d22_hsh_fd, (void *) admptr->d22_hshtab,
				(size_t) (admptr->d22_conftab->d22_hsh_max *
			      sizeof (D22_hash_entry)), (off_t) 0) == D22_ERROR)
		return (D22_ERROR) ;
	} else {
	    /* update DUA-cache hash file - single entry */
	    if (d22_cfwrite (admptr->d22_hsh_fd, (void *) hshptr,
			(size_t) sizeof (D22_hash_entry), (off_t) (index *
					 sizeof (D22_hash_entry))) == D22_ERROR)
		return (D22_ERROR) ;
	}
	/* update DUA-cache description file */
	if (d22_cfwrite (admptr->d22_desc_fd, (void *) admptr->d22_desctab,
			(size_t) sizeof (D22_desc_tab), (off_t) 0) == D22_ERROR)
	    return (D22_ERROR) ;
    }

    /* generate result message */
    rsptr->d23_3rmdat = D23_LASTFG ;
    rsptr->d23_3rfrdt = rqptr->d23_2rfidt ;
    rsptr->d23_3lnrdt = sizeof (D23_eom) ;
    ((D23_eom *) rsptr->d23_3rfrdt)->d23_eomtag = D23_EOMTAG ;

    return (D22_OK) ;
}
