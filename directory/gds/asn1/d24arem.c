/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24arem.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:38  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:30  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:14  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:26:35  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:20  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:52  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:50:52  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:41:08  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:44:37  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:33:24  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:36  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:09:53  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:55:49  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:49:11  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:05:18  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:01:53  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d24arem.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:48 $";
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
/* NAME         : d24arem.c                                         */
/*								      */
/* AUTHOR       :  Helmut Volpers                                     */
/* DATE         :  26.02.90                                           */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.           */
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
/*  FUNCTION: d2a067_apdu_rem_arg_ids()                               */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms remove_arg's from APDU format to*/
/*  the remove arguments and common arguments of the ids interface.   */
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

signed32 d2a067_apdu_rem_arg_ids(
  byte * apdu,
  signed32 * apdu_len,
  ids_remove_arg * remove_arg,
  ids_common_arg * com_arg,
  byte * err_apdu )
{
byte * scan = apdu;
#ifndef DSA
signed32 version = ((D23_header *)apdu)->d23_versno;
#endif
const char function_name[] = "d2a_200_certificate_from_apdu";

#ifndef DSA

    if                              /*  IPC version is not ok         */
      (!D2_CHECK_MINMAX(D23_APDUV22,D23_APDUV22,version))
    {                               /*  reset to 0 and return error   */
      d27_010_set_error_apdu((D23_rserror *)apdu,D2_CALL_ERR,D2_ILL_VERS,
			     D2_NOT_SPEC,apdu_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
    }                               /*                                */
    scan += D23_S_REMOBJ;
#endif

    d2a080_mk_ids_comarg(&scan,com_arg);

     if (d2a121_dn_ids(&scan, &remove_arg->object, err_apdu,apdu_len)
			      == D2_ERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

return(D2_NOERROR);
}
