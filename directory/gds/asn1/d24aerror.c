/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d24aerror.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:32  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:26  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:10  marrek
 * 	June 1994 code submission.
 * 	[1994/06/21  09:25:43  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:16  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:37  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:50:40  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:40:18  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:20:31  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:32:22  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:12:23  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:09:32  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/02  10:24:47  marrek
 * 	November 1992 code drop
 * 	return correct error number
 * 	[1992/12/02  09:51:47  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  15:54:40  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:48:11  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:04:42  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:01:24  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d24aerror.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:41 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/*daton	***************************************************************/
/*								      */
/* TYP		: C-FILE					      */
/*								      */
/* NAME         : d24aerror.c                                         */
/*								      */
/* AUTHOR       : Praveen Gupta, Siemens India                        */
/* DATE         : 9. 1. 1990                                          */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to (THORN)-IDS interface for CStub  */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-02-09| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL *********/

static signed32 d2a050_mk_att_error ( ids_att_error * , byte ** , signed32 * ,
					byte * , signed32 * );
static signed32 d2a051_put_nm_error ( ids_name_error * , byte ** , signed32 * ,
					byte * , signed32 * );
static void d2a052_put_srv_error ( ids_serv_error * , byte ** );
static void d2a054_put_sec_error ( ids_sec_error * , byte ** );
static void d2a055_put_upd_error ( ids_upd_error * , byte ** );

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a014_error_dec_apdu()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms errors in IDS structure to      */
/*  Error in APDU format.                                             */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      err_buf     = error structure in IDS Format.                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = Error APDU with rs_error.                       */
/*      apdu_len    = APDU length.                                    */
/*                                                                    */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/



signed32 d2a014_error_dec_apdu(
  byte ** apdu,
  signed32 * apdu_len,
  ids_error * err_buf,
  byte * err_apdu )
{
   signed32  max_len;
   D23_rserror * rs_error;
   byte * scan;
const char function_name[] = "d2a014_error_dec_apdu";

    if ((scan = D27_MALLOC(svc_c_sev_warning,D2A_APDU_MEM))==NULL)
	{
	  d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
		D2_SYS_ERR,D21_ALLOC_ERROR, apdu_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    *apdu = scan;
    max_len = D2A_APDU_MEM;
    rs_error = (D23_rserror *) scan;
    rs_error->d23_Zversion = D23_V1988;

  switch (err_buf->error_code)
    {

     case ATT_ERROR : /* returns the first att_problem back in apdu */
		{
		   if (d2a050_mk_att_error(&err_buf->contents.att,&scan,
			&max_len, err_apdu, apdu_len) == D2_ERROR)
		    {
		      free( *apdu);
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		    }
		   else break;
		 }
     case NAME_ERROR :
		{
		   if (d2a051_put_nm_error(&err_buf->contents.name, &scan,
			&max_len, err_apdu, apdu_len) == D2_ERROR)
		    {
		      free( *apdu);
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		    }
		   break;
		 }
     case SERV_ERROR :
		{
		   d2a052_put_srv_error(&err_buf->contents.serv, &scan);
		   break;
		 }
     case REFERRAL :
		{
		  rs_error = (D23_rserror *) *apdu;
		  rs_error->d23_Zretcod = D2_CALL_ERR;
		  rs_error->d23_Zerrcls = D2_PAR_ERR;
		  rs_error->d23_Zerrval = D2_REF_ERROR;
		  scan += sizeof(D23_rserror);
		  if (d2a053_put_ref(&err_buf->contents.ref.candidate,
			&scan, apdu, &max_len, err_apdu, apdu_len)== D2_ERROR)
		    {
		      free( *apdu);
		      D2_ASN1_ERR_RETURN_WITH_TRACE
		    }
		   else break;
		 }
     case SEC_ERROR :
		{
		   d2a054_put_sec_error(&err_buf->contents.sec, &scan);
		   break;
		 }
     case UPDATE_ERROR :
		{
		  d2a055_put_upd_error(&err_buf->contents.upd, &scan);
		   break;
		 }
     case ABAN_ERROR :
		{
		  rs_error = (D23_rserror *) *apdu;
		  rs_error->d23_Zretcod = D2_TEMP_ERR;
		  rs_error->d23_Zerrcls = D2_REM_ERR;
		  rs_error->d23_Zerrval = D2_ABANDONED;
		  break;
		}
		
	 default :;

	}
    ((D23_eom *) scan)->d23_eomtag = D23_EOMTAG;
    ((D23_eom *) scan)->d23_eomfil = D2_ALIGN(D23_EOMTAG);
    *apdu_len = (scan + sizeof(D23_eom)) - *apdu;

return(D2_NOERROR);

}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a050_mk_att_error()                                   */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms att. errors in IDS structure to */
/*  Error in APDU format.                                             */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      att_err     = att_error structure in IDS Format.              */
/*      max_len     = length of APDU-memory allocated;                */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = APDU with att_error.                            */
/*      apdu_len    = APDU length				      */
/*			   (in case of error in func, len of err_apdu)*/
/*      err_apdu    = Error APDU in case of error in Func.            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static signed32 d2a050_mk_att_error(
  ids_att_error * att_err,
  byte ** apdu,
  signed32 * max_len,
  byte * err_apdu,
  signed32 * apdu_len )
{
  byte * scan= * apdu;
  byte ** beg_apdu = apdu; /* starting addr. of APDU */
  ids_att_pb    * i_att_pb;
  ids_att         attribute;
  D23_rserror   * rs_error = (D23_rserror *) *apdu;
  D23_at_prob   * at_prob;
  signed32             i;
const char function_name[] = "d2a050_mk_att_error";

  rs_error->d23_Zretcod = D2_CALL_ERR;
  rs_error->d23_Zerrcls = D2_PAR_ERR;
  rs_error->d23_Zerrval = D2_ATT_ERROR;
  scan += sizeof(D23_rserror);

  if (d2a122_dn_apdu( D23_NAMTAG, &scan, &att_err->object,
		   apdu, max_len, err_apdu, apdu_len) == D2_ERROR)
       {
       D2_ASN1_ERR_RETURN_WITH_TRACE
       }

  i_att_pb = att_err->att_pb_list;
  for (i=0; i<att_err->nb_pb; i++, i_att_pb++)
  {
   at_prob = (D23_at_prob *) scan;
   at_prob->d23_aptag = D23_APTAG;
   switch (i_att_pb->att_pb)
      {
	case NO_SUCH_ATT:
		   {  at_prob->d23_apval = D2_ATTRIBUTE_MISSING;
		      break;
		    }
	case INV_ATT_SYNTAX_ATT_PB:
		   {  at_prob->d23_apval = D2_ATSYNTAX_ERROR;
		      break;
		    }
	case UNDEF_ATT_TYPE :
		   {  at_prob->d23_apval = D2_UNDEFINED_ATTR_TYPE;
		      break;
		    }
	case INAPP_MATCHING:
		   {  at_prob->d23_apval = D2_INAPPR_MATCH;
		      break;
		    }
	case CONSTRAINT_VIOL:
		   {  at_prob->d23_apval = D2_CONSTRAINT_VIOLATION;
		      break;
		    }
	case ATT_VAL_EXISTS: at_prob->d23_apval = D2_AT_VALUE_EXISTS;
       }
     scan += D23_S_ATPROB;
     attribute.type.l = i_att_pb->att_type.l;
     attribute.type.v = i_att_pb->att_type.v;
     if (i_att_pb->att_value_defined)
	attribute.nb_values = 1;
     else
	attribute.nb_values = 0;
     attribute.values = &(i_att_pb->att_value);

    /*
      copy attribute value to APDU
    */
    if (d2a060_mk_att_apdu(&attribute, &scan, beg_apdu,
			   max_len, err_apdu, apdu_len) == D2_ERROR)
      {   
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	 }
   }
  *apdu = scan;
  *apdu_len = scan - *beg_apdu;

  return(D2_NOERROR);

}


/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a051_put_nm_error()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms name error in APDU format to    */
/*  Name error in IDS structure.                                      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = Error APDU with name_error.                     */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      name_err    = Name error structure in IDS Format.             */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static signed32 d2a051_put_nm_error(
  ids_name_error * name_err,
  byte ** apdu,
  signed32   *  max_len,
  byte *  err_apdu,
  signed32 * apdu_len )
{
  D23_rserror * rs_error = (D23_rserror *) *apdu;
  byte * scan = *apdu;
const char function_name[] = "d2a051_put_nm_error";

  rs_error->d23_Zretcod = D2_CALL_ERR;
  rs_error->d23_Zerrcls = D2_PAR_ERR;
  scan += sizeof(D23_rserror);

  switch (name_err->pb )
    {
      case NO_SUCH_OBJECT:
		 {  rs_error->d23_Zerrval = D2_NFOUND;
		    break;
		  }
      case INV_ATT_SYNTAX_NAME_PB:
		 {  rs_error->d23_Zerrval = D2_NMSYNTAX_ERROR;
		    break;
		  }
      case ALIAS_DEREF_PB:
		 {  rs_error->d23_Zerrval = D2_ON_ALIAS_NOT_ALLOWED;
		    break;
		  }
      case ALIAS_PB: rs_error->d23_Zerrval = D2_ALIAS_ERROR;
     }

if (d2a122_dn_apdu( D23_NAMTAG, &scan, &name_err->matched,
		   apdu, max_len, err_apdu, apdu_len) == D2_ERROR)
       {
       D2_ASN1_ERR_RETURN_WITH_TRACE
       }

*apdu = scan;

return (D2_NOERROR);
}


/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a052_put_srv_error()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms service errors in APDU format to*/
/*  service error in IDS structure.                                   */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = Error APDU with service error.                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      serv_err    = service error structure in IDS Format.          */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static void d2a052_put_srv_error(
  ids_serv_error * serv_err,
  byte ** apdu )
{
  D23_rserror * rs_error = (D23_rserror *) *apdu;
  byte * scan = *apdu;

  scan += sizeof(D23_rserror);
  rs_error->d23_Zretcod = D2_CALL_ERR;
  rs_error->d23_Zerrcls = D2_PAR_ERR;

  switch (serv_err->pb )
    {
      case BUSY:
		 {  rs_error->d23_Zerrval = D2_TOO_BUSY;
		    rs_error->d23_Zretcod = D2_TEMP_ERR;
		    rs_error->d23_Zerrcls = D2_REM_ERR;
		    break;
		  }
      case UNAVAILABLE:
		 {  rs_error->d23_Zerrval = D2_NO_RESPONSE;
		    rs_error->d23_Zretcod = D2_TEMP_ERR;
		    rs_error->d23_Zerrcls = D2_REM_ERR;
		    break;
		  }
      case CHAIN_REQUIRED:
		 {  rs_error->d23_Zerrval = D2_CHAINING_REQUIRED;
		    break;
		  }
      case UNWILL_TO_PERFORM:
		 {  rs_error->d23_Zerrval = D2_UNWILLING;
		    rs_error->d23_Zretcod = D2_TEMP_ERR;
		    rs_error->d23_Zerrcls = D2_REM_ERR;
		    break;
		  }
      case UNABLE_TO_PROCEED:
		 {  rs_error->d23_Zerrval = D2_UNABLE_TO_PROCEED;
		    break;
		  }
      case INVALID_REF:
		 {  rs_error->d23_Zerrval = D2_INVALID_REFERENCE;
		    break;
		  }
      case ADM_SIZE_LIMIT_EXC:
		 {  rs_error->d23_Zerrval = D2_ADM_LIM_EXCEEDED;
		    break;
		  }
      case LOOP_DETECTED:
		 {  rs_error->d23_Zerrval = D2_LOOP_DETECTED;
		    break;
		  }
      case UN_CRITIC_EXTENSION:
		 {  rs_error->d23_Zerrval = D2_UNAV_CRIT_EXTENSION;
		    break;
		  }
      case OUT_OF_SCOPE:
		 {  rs_error->d23_Zerrval = D2_OUT_OF_SCOPE;
		    rs_error->d23_Zretcod = D2_PERM_ERR;
		    rs_error->d23_Zerrcls = D2_REM_ERR;
		    break;
		  }
      case DIT_ERROR:
		 {  rs_error->d23_Zerrval = D2_DIT_INCONSISTENT;
		    break;
		  }
     case TIME_LIMIT_EXC:
		 {  rs_error->d23_Zerrval = D2_TIME_LIMIT_EXCEEDED;
		    rs_error->d23_Zretcod = D2_TEMP_ERR;
		    rs_error->d23_Zerrcls = D2_REM_ERR;
		  }
     }

*apdu = scan;

 }


/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a054_put_sec_error()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms security error in IDS structure */
/*  to security error in rs_error of APDU. 			      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      sec_err     = security error structure in IDS Format.         */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = Error APDU with rs_error having update err.     */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static void d2a054_put_sec_error(
  ids_sec_error * sec_err,
  byte ** apdu )
{
  D23_rserror * rs_error = (D23_rserror *) *apdu;
  byte * scan = *apdu;

  rs_error->d23_Zretcod = D2_CALL_ERR;
  rs_error->d23_Zerrcls = D2_PAR_ERR;
  scan += sizeof(D23_rserror);

  switch (sec_err->pb )
    {
      case INAPP_AUTHEN:
		 {  rs_error->d23_Zerrval = D2_INAPPR_AUTH;
		    break;
		  }
      case INV_CREDENTIALS:
		 {  rs_error->d23_Zerrval = D2_CREDENTIALS_INVALID;
		    break;
		  }
      case INS_ACC_RIGHTS:
		 {  rs_error->d23_Zerrval = D2_ACCESS_RIGHTS_INSUFFICIENT;
		    break;
		  }
      case INV_SIGNATURE:
		 {  rs_error->d23_Zerrval = D2_INVALID_SIGNATURE;
		    break;
		  }
      case PROTECT_REQUIRED:
		 {  rs_error->d23_Zerrval = D2_PROTECTION_REQUIRED;
		    break;
		  }
      case NO_INFORMATION: rs_error->d23_Zerrval = D2_DSA_UNKNOWN;
     }

*apdu = scan;

}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a055_put_upd_error()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms update error in IDS structure   */
/*  to update error in rs_error of APDU. 			      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      upd_err     = update error structure in IDS Format.           */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = Error APDU with rs_error having update err.     */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static void d2a055_put_upd_error(
  ids_upd_error * upd_err,
  byte ** apdu )
{
  D23_rserror * rs_error = (D23_rserror *) *apdu;
  byte * scan = *apdu;

  rs_error->d23_Zretcod = D2_CALL_ERR;
  rs_error->d23_Zerrcls = D2_PAR_ERR;
  scan += sizeof(D23_rserror);

  switch (upd_err->pb )
    {
      case NAMING_VIOL:
		 {  rs_error->d23_Zerrval = D2_NAMING_VIOLATION;
		    break;
		  }
      case NOT_ALL_ON_NON_LEAF:
		 {  rs_error->d23_Zerrval = D2_ORPHAN;
		    break;
		  }
      case NOT_ALL_ON_RDN:
		 {  rs_error->d23_Zerrval = D2_NOT_ALLOWED_ON_RDN;
		    break;
		  }
      case OBJ_CLASS_VIOL:
		 {  rs_error->d23_Zerrval = D2_OBJ_CLS_VIOLATION;
		    break;
		  }
      case ENTRY_ALREADY_EXISTS:
		 {  rs_error->d23_Zerrval = D2_ALREADY_EXISTS;
		    break;
		  }
      case AFF_MULT_DSAS:
		 {  rs_error->d23_Zerrval = D2_AFFECTS_MULTIPLE_DSAS;
		    break;
		  }
      case OBJ_CLASS_MOD_PROHIB:
		    rs_error->d23_Zerrval = D2_OBJCLS_MOD_PROHIBITED;
     }

*apdu = scan;

}
