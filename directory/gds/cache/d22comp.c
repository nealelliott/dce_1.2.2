/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22comp.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:35  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:26:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:20  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:48  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:13:04  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:04  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:50  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:08  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:52:17  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:27:11  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:12:44  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:40:26  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:08  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:14:09  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:06:04  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22comp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:38 $";
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
/* NAME         : d22comp.c                                           */
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
/*  FUNCTION:  d22_scompare ()                                               */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function compares two given strings for equality, where the      */
/*      matching rules resp. the first string can be controlled by a type    */
/*      parameter. The type 'D2_EQUAL' means that the two strings must be    */
/*      exactly the same. The type 'D2_SUBSTRINGS' means that the first      */
/*      string can be a substring at the beginning, at the end or within     */
/*      the second string (depends from the parameter 'position').           */
/*      NOTE: The matching rules 'D2_GTEQ', 'D2_LTEQ' and 'D2_APPROX_MATCH'  */
/*      are mapped to the matching rule 'D2_EQUAL'.                          */
/*      The following strings are matching:                                  */
/*                                                                           */
/*           string  =  string       (if type is D2_EQUAL)                   */
/*           string  =  substring    (if type is D2_SUBSTRINGS and position  */
/*                                    is D2_FINAL)                           */
/*           string  =  strings      (if type is D2_SUBSTRINGS and position  */
/*                                    is D2_INITIAL)                         */
/*           string  =  substrings   (if type is D2_SUBSTRINGS and position  */
/*                                    is D2_ANY)                             */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      type        =  Type of the first string.                             */
/*      position    =  Position of the first string.                         */
/*      lg1         =  Length of the first string.                           */ 
/*      str1        =  Pointer to the first string.                          */
/*      lg2         =  Length of the second string.                          */
/*      str2        =  Pointer to the second string.                         */
/*      caseignore  =  Compare mode:                                         */
/*                     TRUE    = comparison should be done by following the  */
/*                               'case ignore'-rules (IA5-strings assumed).  */
/*                     FALSE   = comparison should be done by following the  */
/*                               'case exact'-rules.                         */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      TRUE        = The operation has been performed successfully (the     */
/*                    first string is equal to the second string resp. is    */
/*                    contained in the second string -> semantic depends     */
/*                    on the operation mode).                                */
/*      FALSE       = The operation hasn't been performed successfully       */
/*                    (-> string not found).                                 */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 17.11.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d23apdu.h>
#include <d22cache.h>


/* ------------------------------------------------------------------------- */

boolean d22_scompare (D2_a_match type, signed16 position, signed16 lg1,
		     byte *str1, signed16 lg2, byte *str2, boolean caseignore) {

    byte	*p1, *p2 ;
    signed16	lghlp ;

    if (lg1 > lg2 || (type != D2_SUBSTRINGS && lg1 != lg2))
	return (FALSE) ;

    if (type != D2_SUBSTRINGS ||
			    (type == D2_SUBSTRINGS && position == D2_INITIAL)) {
	if (caseignore) {
	    while (lg1-- > 0) {
		if ((*str1 == *str2) || (*str1 >= D2_RG1 && *str1 <= D2_RG2 &&
		     *str1 - *str2 == D2_ULDIFF) || (*str2 >= D2_RG1 &&
			       *str2 <= D2_RG2 && *str2 - *str1 == D2_ULDIFF)) {
		    str1++ ; str2++ ;
		} else
		    return (FALSE) ;
	    }
	} else {
	    while (lg1-- > 0)
		if (*str1++ != *str2++)
		    return (FALSE) ;
	}
    } else {
	if (position == D2_FINAL) {	
	    str1 += lg1 - 1 ;
	    str2 += lg2 - 1 ;
	    if (caseignore) {
		while (lg1-- > 0) {
		    if ((*str1 == *str2) || (*str1 >= D2_RG1 &&
			*str1 <= D2_RG2 && *str1 - *str2 == D2_ULDIFF) ||
			(*str2 >= D2_RG1 && *str2 <= D2_RG2 &&
						  *str2 - *str1 == D2_ULDIFF)) {
			str1-- ; str2-- ;
		} else
		    return (FALSE) ;
			}
		} else {
		    while (lg1-- > 0)
			if (*str1-- != *str2--) return (FALSE) ;
		}
	    } else {
		/* position is D2_ANY */
		if (caseignore) {
		    while (lg1 <= lg2) {
			p1 = str1 ; p2 = str2 ; lghlp = lg1 ;
			while (lghlp > 0) {
			    if ((*p1 == *p2) || (*p1 >= D2_RG1 &&
				 *p1 <= D2_RG2 && *p1 - *p2 == D2_ULDIFF) ||
				 (*p2 >= D2_RG1 && *p2 <= D2_RG2 &&
						      *p2 - *p1 == D2_ULDIFF)) {
				p1++ ;
				p2++ ;
				lghlp-- ;
			    } else
				break ;
			}
			if (lghlp == 0)
			    return (TRUE) ;
			str2++ ;
			lg2-- ;
		    }
		} else {
		    while (lg1 <= lg2) {
			p1 = str1 ; p2 = str2 ; lghlp = lg1 ;
			while (lghlp > 0) {
			    if (*p1++ != *p2++)
				break ;
			    lghlp-- ;
			}
			if (lghlp == 0)
			    return (TRUE) ;
			str2++ ;
			lg2-- ;
		    }
		}
	    return (FALSE) ;
	}
    }
	return (TRUE) ;
}	
