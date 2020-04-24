/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: decodeoct.c,v $
 * Revision 1.1.10.2  1996/02/18  18:20:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:33  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:06:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:58  root]
 * 
 * Revision 1.1.8.2  1994/09/06  12:26:11  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:22:59  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:11:05  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:00:46  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:35:20  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:29:48  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:47:47  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:34:01  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:00:21  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:52:16  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: decodeoct.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:20:48 $";
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
/* NAME         : decodeoct.c                                         */
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
#include <stdio.h>
#include <dce/osiaddr.h>


/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  decode_semi_octets ()                                         */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*    The function decodes any no. of semi-octets which are represented      */
/*    through an octet-string.                                               */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      semi_value  = Pointer to the location at which the decoded semi-     */
/*                    octet string should be stored.                         */
/*      val_size    = No. of the semi-octets which should be decoded.        */
/*      max_valsize = Max. size of the location which contains the semi-     */
/*                    octets (right adjusted).                               */
/*      octet_value = Pointer to the location where the semi-octets are      */
/*                    stored.                                                */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*     *semi_value  = Decoded semi-octet string (ASCII representation).      */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    NONE                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 30.03.90     */
/*exoff **********************************************************************/

void decode_semi_octets (char *semi_value, signed32 val_size,
				      signed32 max_valsize, byte *octet_value) {

    signed32	i, stype ;
    byte	binval ;

    stype = (max_valsize - val_size) & 1 ;
    octet_value += (max_valsize - val_size) / 2 ;
    for (i = 0; i < val_size; i++, stype %= 2) {
	binval = (stype == 0) ? (*octet_value >> 4) & 0x0F :
							   *octet_value & 0x0F ;
	*semi_value++ = (binval > 9) ? binval + XDIGITBASE :
							    binval + DIGITBASE ;
	octet_value += stype++ ;
    }
    *semi_value = '\0' ;
}
