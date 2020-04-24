/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22filter.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:50  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:27:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:28  root]
 * 
 * Revision 1.1.8.2  1994/09/06  12:24:57  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:45  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:23  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:53:42  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:39:21  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:14:19  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:41:08  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:42  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:29:59  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:37:25  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22filter.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:49 $";
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
/* NAME         : d22filter.c                                         */
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
/*  FUNCTION:  d22_evalfilter ()                                             */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function verifies whether the attribute informations of a        */
/*      directory object are passing the set of given filter attributes.     */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      filter      = Pointer to the reference of the beginning of the       */
/*                    filter information block.                              */
/*      filtereptr  = Pointer to the end of the filter information block.    */
/*      attrptr     = Pointer to the attribute information block of a        */
/*                    directory object.                                      */
/*      attrsize    = Size of the attribute information block.               */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      TRUE        = The directory object passes the filter.                */
/*      FALSE       = The directory object doesn't pass the filter.          */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 11.1.87      */
/*exoff **********************************************************************/

#include <gds.h>
#include <string.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>


/* ------------------------------------------------------------------------- */

boolean d22_evalfilter (D23_ft1_info **filter, D23_ft1_info *filtereptr,
				   D22_attr_entry *attrptr, signed32 attrsize) {

    D22_attr_entry	*maxaptr = (D22_attr_entry *) ((char *) attrptr +
								     attrsize) ;
    D23_fi1_info	*fiptr ;
    D23_fs1_info	*fsptr, *fssptr ;
    D23_at_type		*tpptr ;
    D23_av_value	*avptr ;
    D2_a_match		fmatch ;
    boolean		compare_complete, naming_id ;
    signed16		i, nofilter, fvalsize, pos ;
    byte		*fvalptr, *valptr, *maxvptr ;


    if (*filter >= filtereptr)
	return (TRUE) ;

    switch ((*filter)->d23_fp1mod) {
    case D2_ITEM:
	fiptr = (D23_fi1_info *) ((*filter) + 1) ;
	tpptr = (D23_at_type *) (fiptr + 1) ;
	fssptr = fsptr = (D23_fs1_info *) ((char *) tpptr + D23_S_ATT +
							     tpptr->d23_atoff) ;
	avptr = (D23_av_value *) fsptr ;

	/* set filter pointer to next filter */
	if ((fmatch = fiptr->d23_fi1match) == D2_SUBSTRINGS) {
	    do {
	   	fsptr = (D23_fs1_info *) ((char *) fsptr +
						D23_S_FS1 + fsptr->d23_fs1off) ;
	    } while (fsptr->d23_fs1tag == D23_FISTAG) ;
	    *filter = (D23_ft1_info *) fsptr ;
	} else {
	    *filter = (fmatch == D2_PRESENT) ? (D23_ft1_info *) avptr :
	      (D23_ft1_info *) ((char *) avptr + D23_S_ATV + avptr->d23_avoff) ;
	}

	/* search corresponding attribute type */
	while (attrptr < maxaptr) {
	    if (attrptr->d22_aidsize == tpptr->d23_atlen &&
		memcmp ((void *) attrptr->d22_aident, (void *) (tpptr + 1),
					      (size_t) tpptr->d23_atlen) == 0) {

		naming_id = ((attrptr->d22_aetype & D22_NAME_AFLAG) ==
						D22_NAME_AFLAG) ? TRUE : FALSE ;
		if (fmatch == D2_PRESENT)
		    return (TRUE) ;

		/* compare filter attribute values and */
		/* attribute values of the object */
		if ((attrptr->d22_aetype & D22_REC_FLAG) == D22_REC_FLAG) {
		    /* recurring attribute -> compare all values */
		    valptr = (unsigned char *) (attrptr + 1) ;
		    maxvptr = valptr + attrptr->d22_asizes.d22_voff ;
		} else {
		    valptr = (unsigned char *) &attrptr->d22_asizes ;
		    maxvptr = valptr + sizeof (D22_size_info) +
						  attrptr->d22_asizes.d22_voff ;
		}

		do {
		    fsptr = fssptr ;
		    compare_complete = TRUE ;
		    do {
			if (fmatch == D2_SUBSTRINGS) {
			    fvalptr = (unsigned char *) (fsptr + 1) ;
			    fvalsize = fsptr->d23_fs1len ;
			    pos = fsptr->d23_fs1position ;
			} else {
			    fvalptr = (unsigned char *) (avptr + 1) ;
			    fvalsize = avptr->d23_avlen ;
			}
								
			if (!d22_scompare (fmatch, pos, fvalsize, fvalptr,
				   ((D22_size_info *)valptr)->d22_vlen,
				   valptr + sizeof (D22_size_info), naming_id)) {
			    compare_complete = FALSE ;
			    break ;
			}

			fsptr = (D23_fs1_info *) ((char *) fsptr +
						D23_S_FS1 + fsptr->d23_fs1off) ;
		    } while (fmatch == D2_SUBSTRINGS &&
					      fsptr->d23_fs1tag == D23_FISTAG) ;
		    if (compare_complete)
			return (TRUE) ;
		    valptr = valptr + sizeof (D22_size_info) +
					  ((D22_size_info *) valptr)->d22_voff ;
		} while (valptr < maxvptr) ;
		if (!naming_id)
		    break ;
	    }
    	    attrptr = (D22_attr_entry *) ((char *) attrptr +
		      sizeof (D22_attr_entry) + attrptr->d22_asizes.d22_voff) ;
	}
	return (FALSE) ;
    case D2_AND:
	nofilter = (*filter)++->d23_fp1count ;
	for (i = 0; i < nofilter; i++) {
	    if (!d22_evalfilter (filter, filtereptr, attrptr, attrsize))
		return (FALSE) ;
	}
	return (TRUE) ;
    case D2_OR:
	nofilter = (*filter)++->d23_fp1count ;
	for (i = 0; i < nofilter; i++) {
	    if (d22_evalfilter (filter, filtereptr, attrptr, attrsize))
		return (TRUE) ;
	}
	return (FALSE) ;
    case D2_NOT:
	(*filter)++ ;
	return ((d22_evalfilter (filter, filtereptr, attrptr, attrsize)) ?
								 FALSE : TRUE) ;
    default:
	/* should never occur */
	return (FALSE) ;
    }
}
