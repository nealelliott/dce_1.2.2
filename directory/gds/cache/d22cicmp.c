/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22cicmp.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:32  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:26:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:18  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:24:46  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:12:53  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:01  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:16:42  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:04  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:51:59  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:25:16  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:12:24  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:40:18  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:13:00  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:13:53  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:05:54  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22cicmp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:36 $";
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
/* NAME         : d22cicmp.c                                          */
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
/*  FUNCTION:  d22_cignore_comp ()                                           */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function verifies two given strings for equality by comparing    */
/*      them by following the matching rule 'case ignore string' (means      */
/*      upper and lower case characters are equal, e.g. 'STRING' is equal    */
/*      to 'string').                                                        */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      p1          = Pointer to the first string.                           */
/*      p2          = Pointer to the second string.                          */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      TRUE        = The two strings are equal.                             */
/*      FALSE       = The two strings are not equal.                         */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 11.12.87     */
/*exoff **********************************************************************/

#include <gds.h>
#include <string.h>
#include <dce/d2dir.h>
#include <d23apdu.h>
#include <d23ipc.h>
#include <d22cache.h>


/* ------------------------------------------------------------------------- */

boolean d22_cignore_comp (char *p1, char *p2) {

    signed16	i, len ;

    if ((len = strlen (p1)) != strlen (p2))
	return (FALSE) ;

    for (i = 0; i < len; i++, p1++, p2++) {
	if ((*p1 == *p2) || (*p1 >= D2_RG1 && *p1 <= D2_RG2 &&
	    *p1 - *p2 == D2_ULDIFF) || (*p2 >= D2_RG1 && *p2 <= D2_RG2 &&
							*p2 - *p1 == D2_ULDIFF))
	    continue ;
	else {
	    /* ignore representation delimiter */
	    if ((*p1 == D2_T61_REP || *p1 == D2_PRTBL_REP ||
		 *p1 == D2_IA5_REP || *p1 == D2_NUM_REP) &&
		(*p2 == D2_T61_REP || *p2 == D2_PRTBL_REP ||
		 *p2 == D2_IA5_REP || *p2 == D2_NUM_REP))
		continue ;
	}
	return (FALSE) ;
    }
    return (TRUE) ;
}
