/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: encodeoct.c,v $
 * Revision 1.1.8.2  1996/02/18  18:20:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:39  marty]
 *
 * Revision 1.1.8.1  1995/12/08  16:07:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:04  root]
 * 
 * Revision 1.1.6.2  1994/09/06  12:26:13  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:23:21  keutel]
 * 
 * Revision 1.1.6.1  1994/02/22  16:11:09  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:01:18  marrek]
 * 
 * Revision 1.1.4.2  1993/08/11  12:39:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:30:35  marrek]
 * 
 * Revision 1.1.2.3  1992/12/31  21:29:41  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:34:17  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/04  18:11:11  marrek
 * 	November 1992 code drop
 * 	Creation
 * 	[1992/12/04  12:09:26  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: encodeoct.c,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 18:20:53 $";
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
/* NAME         : encodeoct.c                                         */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 30.03.90                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     :                                                     */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES:                                               */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <ctype.h>
#include <stdio.h>
#include <dce/osiaddr.h>


/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  encode_semi_octets ()                                         */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*    The function encodes a semi-octet string into a octet_field. If        */
/*    the size of the semi-octet string remains under the max. size of       */
/*    octet-field, then semi-octet string is right adjusted.                 */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      semi_value  =  Pointer to the semi-octet string                      */
/*                     (ASCII-representation).                               */ 
/*      max_valsize =  Max. size of the octet-value field                    */
/*		       (no. of semi-octets).                                 */
/*      octet_value =  Pointer to the octet-value field where the encoded    */
/*                     semi-octets should be stored.                         */
/*  OUTPUT-PARAMETER:                                                        */
/*     *octet_value =  Encoded semi-octets.                                  */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      > 0         =  No. of the occupied octets.                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 30.03.90     */
/*exoff **********************************************************************/

signed32 encode_semi_octets (char *semi_value, signed32 max_valsize,
							    byte *octet_value) {

    signed32	i, stype, valsize ;
    byte	binval ;

    valsize = strlen (semi_value) ;
    stype = (max_valsize - valsize) & 1 ;
    octet_value += (max_valsize - valsize) / 2 ;
    for (i = 0; i < valsize; i++, stype %= 2) {
	binval = (isdigit (*semi_value)) ? *semi_value++ - DIGITBASE :
		 (isupper (*semi_value)) ? *semi_value++ - XDIGITBASE :
					  	   *semi_value++ - xDIGITBASE ;
	*octet_value = (stype == 0) ? binval << 4 : *octet_value | binval ;
	octet_value += stype++ ;
    }
    return ((max_valsize + 1) / 2) ;	
}
