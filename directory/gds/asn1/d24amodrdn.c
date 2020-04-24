/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24amodrdn.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:36  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:29  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:13  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:26:30  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:19  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:46  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:50:47  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:40:45  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:42:49  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:33:02  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:31  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:09:45  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:55:23  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:48:49  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:05:03  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:01:42  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d24amodrdn.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:45 $";
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
/* NAME         : d24amodrdn.c                                        */
/*								      */
/* AUTHOR       :  Praveen Gupta                                      */
/* DATE         :  28.08.90                                           */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.                   */
/*                ( for modifyRDN entry )                             */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-03-01| Original                       | as |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a127_modrdn_arg_ids()                            */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*       This function transforms modifyRDN_arg's from APDU format to */
/*        the modifyRDN arguments and common arguments of the IDS     */
/*        interface.                                                  */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      modrdn_arg  = modRDN arguments in IDS_interface.              */
/*      com_arg   = Common arguments of IDS_interface.                */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = APDU for the modify arguments.                  */
/*      apdu_len    = APDU length for the modify arguments.           */
/*      err_apdu    = error APDU (memory error or syntax error)       */
/*      apdu_len    = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a127_modrdn_arg_ids(
  byte                *apdu,
  signed32                  *apdu_len,
  ids_modify_rdn_arg   *modrdn_arg,
  ids_common_arg       *com_arg,
  byte                *err_apdu )
{
D23_repinfo    *inrepRDN ;
ids_dist_name     dn;
byte            * act_apdu = apdu;
#ifndef DSA
signed32 version = ((D23_header *)apdu)->d23_versno;
#endif
const char function_name[] = "d2a127_modrdn_arg_ids";

#ifndef DSA

/*   do we need to check APDU-Version */

    if                              /*  IPC version is not ok         */
      (!D2_CHECK_MINMAX(D23_APDUV22,D23_APDUV22,version))
    {                               /*  reset to 0 and return error   */
      d27_010_set_error_apdu((D23_rserror *)apdu,D2_CALL_ERR,D2_ILL_VERS,
			     D2_NOT_SPEC,apdu_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
    }                               /*                                */
    act_apdu += D23_S_RPRDN;
#endif
d2a080_mk_ids_comarg(&act_apdu,com_arg);

inrepRDN = (D23_repinfo *) act_apdu ;
if (inrepRDN->d23_delordn == TRUE )
    modrdn_arg->delet = TRUE;
else
    modrdn_arg->delet = FALSE;

act_apdu += D23_S_REP;

/*
   generate DN from DUA dist. name
*/
if (d2a121_dn_ids(&act_apdu, &modrdn_arg->object,
		   err_apdu, apdu_len ) == D2_ERROR)
  { 
    D2_ASN1_ERR_RETURN_WITH_TRACE
   }

/*
   generate NEW-RDN
*/
if (d2a121_dn_ids(&act_apdu, &dn,
		   err_apdu, apdu_len ) == D2_ERROR)
  { 
    D2_ASN1_ERR_RETURN_WITH_TRACE
   }

modrdn_arg->new_rdn.nb_assertions = dn.rdnames->nb_assertions;
modrdn_arg->new_rdn.assertions = dn.rdnames->assertions;
free(dn.rdnames);

return(D2_NOERROR);
}

