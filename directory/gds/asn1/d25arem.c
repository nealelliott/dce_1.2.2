/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25arem.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:56  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:43  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:27  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:27:25  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:30  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:17  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:36:52  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:43:22  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:58:52  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:36:27  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:13:11  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:45  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:59:19  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:52:10  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:08:22  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:03:05  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25arem.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:05 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYP		: C-FILE					      */
/*								      */
/* NAME         : d25arem.c                                           */
/*								      */
/* AUTHOR       :  Helmut Volpers                                     */
/* DATE         :  26.02.90                                           */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.                   */
/*                ( for remove)                                       */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-02-09| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a068_ids_rem_arg_apdu()                               */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms remove_arg's from ids format to */
/*  the remove arguments and common arguments of the APDU interface.  */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the remove arguments.                  */
/*      apdu_len    = APDU length for the remove arguments.           */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      remove_arg  = remove arguments in IDS_interface.            */
/*      com_arg   = Common arguments of IDS_interface.              */
/*      err_apdu    = error APDU (memory error or syntax error)       */
/*      apdu_len     = length of error APDU                           */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Helmut Volpers                                            */
/**********************************************************************/


signed32 d2a068_ids_rem_arg_apdu(
  byte ** apdu,
  byte ** beg_apdu,
  signed32 * apdu_len,
  ids_remove_arg * remove_arg,
  ids_common_arg * com_arg,
  byte          * err_apdu )
{
signed32 max_length=*apdu_len;
byte * scan = *apdu;
const char function_name[] = "d2a068_ids_rem_arg_apdu";

if (d2a123_mk_apdu_cmarg(&scan,com_arg, beg_apdu, apdu_len,
	    err_apdu,apdu_len) == D2_ERROR )
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}

if (d2a122_dn_apdu(D23_NAMTAG, &scan, &remove_arg->object,
	       beg_apdu, &max_length, err_apdu,apdu_len)==D2_ERROR)
	{ 
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	 };

((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;
((D23_eom *) scan)->d23_eomfil = D2_ALIGN(D23_EOMTAG);
*apdu = (scan + sizeof(D23_eom));

return(D2_NOERROR);
}
