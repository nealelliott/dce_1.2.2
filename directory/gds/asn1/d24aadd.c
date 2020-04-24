/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24aadd.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:26  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:22  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:06  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:26:02  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:12  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:23  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:36:39  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:39:36  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:16:05  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:31:28  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:12:12  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:09:15  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:53:36  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:47:14  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:04:12  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:00:59  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d24aadd.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:36 $";
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
/* NAME         : d24aadd.c                                           */
/*								      */
/* AUTHOR       : Praveen Gupta, Siemens India                        */
/* DATE         : 26. 2. 1990                                         */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.                   */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a075_apdu_add_arg_ids()                               */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function transforms add_arg's from APDU format to        */
/*      the add arguments and common arguments of the ids interface.  */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = APDU for the add arguments.                     */
/*      apdu_len    = APDU length for the add arguments.              */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      add_arg   = add arguments in IDS_interface.                   */
/*      com_arg   = Common arguments of IDS_interface.                */
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

signed32 d2a075_apdu_add_arg_ids(
  byte           *apdu,
  signed32             *apdu_len,
  ids_add_arg     *add_arg,
  ids_common_arg  *com_arg,
  byte           *err_apdu )
{
byte           *sav_apdu;
byte           *p_mess = apdu;
signed32              i;
signed32              nb_attr;
D23_at_type     *at_type;
D23_av_value    *av_value;
ids_att         *ids_attr;
#ifndef DSA
signed16 version = ((D23_header *)apdu)->d23_versno;
#endif
const char function_name[] = "d2a075_apdu_add_arg_ids";

#ifndef DSA

    p_mess += sizeof(D23_inaddobjt);

    if                              /*  IPC version is not ok         */
      (!D2_CHECK_MINMAX(D23_APDUV22,D23_APDUV22,version))
    {                               /*  reset to 0 and return error   */
      d27_010_set_error_apdu((D23_rserror *)apdu,D2_CALL_ERR,D2_ILL_VERS,
			     D2_NOT_SPEC,apdu_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
    }                               /*                                */
#endif

d2a080_mk_ids_comarg(&p_mess, com_arg);

/* Name should at least have one name part */

if (d2a121_dn_ids(&p_mess, &add_arg->object, err_apdu,apdu_len)
					== D2_ERROR)
  { 
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

sav_apdu = p_mess;
				    /* look for all different attributes */
for (add_arg->nb_att = 0; ; add_arg->nb_att++)
{   at_type = (D23_at_type *) p_mess;
    if (at_type->d23_atttag != D23_ATTTAG)
    {   /*
	   no more attributes in APDU
	*/
	break;
    }

    /*
       check for recurring values
    */
    p_mess += sizeof(D23_at_type) + at_type->d23_atoff + D23_S_AV;
    for (av_value = (D23_av_value *) p_mess;
	 (unsigned16)(av_value->d23_avlen) < D23_MINTAG; )
    {   p_mess += sizeof(D23_av_value) + av_value->d23_avoff;
	av_value = (D23_av_value *) p_mess;
    }
}

/*
   allocate memory for "ids_att" structures
*/
if ((add_arg->atts = (ids_att *) D27_MALLOC
		(svc_c_sev_warning,add_arg->nb_att * sizeof(ids_att))) == NULL)
{   
    d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR, D2_SYS_ERR,
			   D21_ALLOC_ERROR, apdu_len);
    D2_ASN1_ERR_RETURN_WITH_TRACE
}

/*
   generate "ids_att" structures from APDU
*/
for (i = 0, ids_attr = add_arg->atts, p_mess = sav_apdu;
     i < add_arg->nb_att; i++, ids_attr++)
{   if (d2a062_mk_apdu_att(&p_mess, ids_attr,
			  &nb_attr, err_apdu, apdu_len) != D2_NOERROR)
    {   D2_ASN1_ERR_RETURN_WITH_TRACE
    }
}

return(D2_NOERROR);
}

