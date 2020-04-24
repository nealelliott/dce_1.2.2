/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25aadd.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:34  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:18  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:26:55  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:20  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:15:12  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:51:00  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:41:53  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:49:00  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:34:29  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:46  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:07  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:57:16  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:50:14  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:05:47  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:02:13  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25aadd.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:53 $";
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
/* NAME         : d25aadd.c                                           */
/*								      */
/* AUTHOR       : Schmid, DI AP 11                                    */
/* DATE         : 26. 2. 1990                                         */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to (THORN)-IDS interface.           */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-02-09| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a076_ids_add_arg_apdu()                               */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function transforms add_arg's from IDS interface to APDU */
/*      format.                                                       */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the add arguments.                     */
/*      apdu_len    = APDU length for the add arguments.              */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      add_arg   = add arguments in IDS_interface.                 */
/*      com_arg   = Common arguments of IDS_interface.              */
/*      err_apdu    = error APDU (memory error or syntax error)       */
/*      apdu_len    = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu with length = *apdu_len is returned)*/
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: W. Schmid                                                 */
/**********************************************************************/

signed32 d2a076_ids_add_arg_apdu(
  byte           **apdu,
  byte           **beg_apdu,
  signed32              *apdu_len,
  ids_add_arg      *add_arg,
  ids_common_arg   *com_arg,
  byte            *err_apdu )
{
ids_att         *ids_attr;
signed32              i;
byte           *act_apdu = *apdu;
char      err_comment[200];
const char function_name[] = "d2a076_ids_add_arg_apdu";

if (d2a123_mk_apdu_cmarg(&act_apdu,com_arg, beg_apdu, apdu_len,
	    err_apdu, apdu_len ) == D2_ERROR )
	{
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}

/* convert DIST-NAME from ids-struc. to ids-struct. */

if (d2a122_dn_apdu( D23_NAMTAG, &act_apdu, &add_arg->object,
	   beg_apdu, apdu_len, err_apdu, apdu_len ) == D2_ERROR)
   { 
     D2_ASN1_ERR_RETURN_WITH_TRACE
    };

/*
   generate APDU from ids attributes
*/
for (i = 0, ids_attr = add_arg->atts; i < add_arg->nb_att;
     i++, ids_attr++)
{
	if (ids_attr->nb_values <= 0)
	  {
	  sprintf (err_comment,
		"error in 076 : no of attribute values = %d in %d st attribute",
			ids_attr->nb_values, (i+1));
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	
	if (d2a060_mk_att_apdu (ids_attr, &act_apdu, beg_apdu,
				apdu_len, err_apdu, apdu_len) != D2_NOERROR)
	   { 
	     D2_ASN1_ERR_RETURN_WITH_TRACE
	     }
}

((D23_eom *) act_apdu)->d23_eomtag = D23_EOMTAG;
*apdu = (act_apdu + sizeof(D23_eom));

return(D2_NOERROR);
}
