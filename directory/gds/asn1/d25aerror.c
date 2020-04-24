/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25aerror.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:50  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:38  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:22  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:27:09  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:24  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:03  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:51:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:42:38  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:53:43  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:35:25  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:12:57  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:10:25  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  15:58:12  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:51:09  marrek]
 * 
 * Revision 1.1.4.2  1992/09/23  11:05:17  marrek
 * 	Insert break; for case D2_ON_ALIAS_NOT_ALLOWED.
 * 	[1992/09/23  11:04:07  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:06:26  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:02:37  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25aerror.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:59 $";
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
/* NAME         : d25aerror.c                                         */
/*								      */
/* AUTHOR       : Praveen Gupta, Siemens India                        */
/* DATE         : 9. 1. 1990                                          */
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
#include <stdlib.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL *********/

static void d2a036_put_nb_att_pb ( byte * , signed32 * );
static signed32 d2a037_put_att_pb_list (ids_att_pb *,byte **,signed32,byte *, signed32 *);
static signed32 d2a038_put_attr_error (ids_att_error *,byte *,byte *,signed32 *);

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a015_error_cod_ids()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms errors in APDU format to        */
/*  Errors in IDS structure.                                          */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = Error APDU with rs_error.                       */
/*      apdu_len    = APDU length.                                    */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      err_buf     = error structure in IDS Format.                  */
/*      error       = contain the type of error which is in IDS str.  */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/


signed32 d2a015_error_cod_ids(
  byte * apdu,
  signed32 apdu_len,                            
  signed32 * error,   /* indicting the error_type e.g. serv_error,upd_error etc */
  ids_error * d_error )
{
D23_rserror * rserror = (D23_rserror * ) apdu;
byte err_apdu[D2A_ERR_APDU_SIZE];
signed32 er_len;
byte * scan = apdu + D23_S_RSERR;
const char function_name[] = "d2a015_error_cod_ids";

   if (rserror->d23_Zerrval == D2_ATT_ERROR)
    {
	*error = ATT_ERROR;
	d_error->error_code = ATT_ERROR;
	if (d2a038_put_attr_error(&d_error->contents.att,scan,
				  err_apdu,&er_len) == D2_ERROR)
	    	{
		D2_ASN1_ERR_RETURN_WITH_TRACE
		}

     }
   else
    if (rserror->d23_Zerrval == D2_REF_ERROR)
    {
	*error = REFERRAL;
	d_error->error_code = REFERRAL;
	if (d2a039_put_referral(&d_error->contents.ref.candidate,
				&scan, err_apdu, &er_len) == D2_ERROR)
	    {
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
     }
    else
     {
      /* OTHER THAN ATTRIBUTE ERRORS OR A REFERRAL*/
      switch (rserror->d23_Zerrval)
       {
	case D2_CREDENTIALS_INVALID :
	     *error = SEC_ERROR;
	     d_error->error_code = SEC_ERROR;
	     d_error->contents.sec.pb = INV_CREDENTIALS;
	     break;
	case D2_DSA_UNKNOWN :
	     *error = SEC_ERROR;
	     d_error->error_code = SEC_ERROR;
	     d_error->contents.sec.pb = NO_INFORMATION;
	     break;
	case D2_INAPPR_AUTH :
	     *error = SEC_ERROR;
	     d_error->error_code = SEC_ERROR;
	     d_error->contents.sec.pb = INAPP_AUTHEN;
	     break;
	case D2_INVALID_SIGNATURE :
	     *error = SEC_ERROR;
	     d_error->error_code = SEC_ERROR;
	     d_error->contents.sec.pb = INV_SIGNATURE;
	     break;
	case D2_PROTECTION_REQUIRED:
	     *error = SEC_ERROR;
	     d_error->error_code = SEC_ERROR;
	     d_error->contents.sec.pb = PROTECT_REQUIRED;
	     break;
	case D2_ACCESS_RIGHTS_INSUFFICIENT :
	     *error = SEC_ERROR;
	     d_error->error_code = SEC_ERROR;
	     d_error->contents.sec.pb = INS_ACC_RIGHTS;
	     break;

	case D2_NAMING_VIOLATION:
	     *error = UPDATE_ERROR;
	     d_error->error_code = UPDATE_ERROR;
	     d_error->contents.serv.pb = NAMING_VIOL;
	     break;

	case D2_OBJ_CLS_VIOLATION:
	     *error = UPDATE_ERROR;
	     d_error->error_code = UPDATE_ERROR;
	     d_error->contents.serv.pb = OBJ_CLASS_VIOL;
	     break;

	case D2_ALREADY_EXISTS:
	     *error = UPDATE_ERROR;
	     d_error->error_code = UPDATE_ERROR;
	     d_error->contents.serv.pb = ENTRY_ALREADY_EXISTS;
	     break;

	case D2_ORPHAN:
	     *error = UPDATE_ERROR;
	     d_error->error_code = UPDATE_ERROR;
	     d_error->contents.serv.pb = NOT_ALL_ON_NON_LEAF;
	     break;

	case D2_NOT_ALLOWED_ON_RDN:
	     *error = UPDATE_ERROR;
	     d_error->error_code = UPDATE_ERROR;
	     d_error->contents.serv.pb = NOT_ALL_ON_RDN;
	     break;

	case D2_AFFECTS_MULTIPLE_DSAS:
	     *error = UPDATE_ERROR;
	     d_error->error_code = UPDATE_ERROR;
	     d_error->contents.serv.pb = AFF_MULT_DSAS;
	     break;

	case D2_OBJCLS_MOD_PROHIBITED:
	     *error = UPDATE_ERROR;
	     d_error->error_code = UPDATE_ERROR;
	     d_error->contents.serv.pb = OBJ_CLASS_MOD_PROHIB;
	     break;

	case D2_MEMORY_INSUFFICIENT:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = UNWILL_TO_PERFORM;
	     break;

	case D2_UNWILLING:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = UNWILL_TO_PERFORM;
	     break;

	case D2_TOO_BUSY:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = BUSY;
	     break;

	case D2_TIME_LIMIT_EXCEEDED:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = TIME_LIMIT_EXC;
	     break;

	case D2_CHAINING_REQUIRED:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = CHAIN_REQUIRED;
	     break;

	case D2_UNABLE_TO_PROCEED:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = UNABLE_TO_PROCEED;
	     break;

	case D2_INVALID_REFERENCE:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = INVALID_REF;
	     break;

	case D2_ADM_LIM_EXCEEDED:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = ADM_SIZE_LIMIT_EXC;
	     break;

	case D2_LOOP_DETECTED:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = LOOP_DETECTED;
	     break;

	case D2_UNAV_CRIT_EXTENSION:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = UN_CRITIC_EXTENSION;
	     break;

	case D2_OUT_OF_SCOPE:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = OUT_OF_SCOPE;
	     break;

	case D2_DIT_INCONSISTENT:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = DIT_ERROR;
	     break;

	case D2_NO_RESPONSE:
	     *error = SERV_ERROR;
	     d_error->error_code = SERV_ERROR;
	     d_error->contents.serv.pb = UNAVAILABLE;
	     break;

	case D2_NO_SUCH_OPERATION:
	     *error = ABAN_FAILED;
	     d_error->error_code = ABAN_FAILED;
	     d_error->contents.aban_failed.pb = NO_SUCH_OP;
	     d_error->contents.aban_failed.inv_id = *((signed32 *)scan);
	     break;

	case D2_TOO_LATE:
	     *error = ABAN_FAILED;
	     d_error->error_code = ABAN_FAILED;
	     d_error->contents.aban_failed.pb = TOO_LATE;
	     d_error->contents.aban_failed.inv_id = *((signed32 *)scan);
	     break;

	case D2_CANNOT_ABANDON:
	     *error = ABAN_FAILED;
	     d_error->error_code = ABAN_FAILED;
	     d_error->contents.aban_failed.pb = CANNOT_ABANDON;
	     d_error->contents.aban_failed.inv_id = *((signed32 *)scan);
	     break;

       case D2_NFOUND :
	       *error = NAME_ERROR;
	       d_error->error_code = NAME_ERROR;
	       d_error->contents.name.pb = NO_SUCH_OBJECT;
	       if (d2a121_dn_ids(&scan,&d_error->contents.name.matched,
				 err_apdu, &er_len) == D2_ERROR)
		   {
		   D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
	       break;
       case D2_ALIAS_ERROR :
	       *error = NAME_ERROR;
	       d_error->error_code = NAME_ERROR;
	       d_error->contents.name.pb = ALIAS_PB;
	       if (d2a121_dn_ids(&scan,&d_error->contents.name.matched,
				 err_apdu, &er_len) == D2_ERROR)
		   {
		   D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
	       break;
       case  D2_NMSYNTAX_ERROR :
	      *error = NAME_ERROR;
	      d_error->error_code = NAME_ERROR;
	      d_error->contents.name.pb = INV_ATT_SYNTAX_NAME_PB;
	      if (d2a121_dn_ids(&scan,&d_error->contents.name.matched,
				 err_apdu, &er_len) == D2_ERROR)
		   {
		   D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
	      break;
       case D2_ON_ALIAS_NOT_ALLOWED :
	      *error = NAME_ERROR;
	      d_error->error_code = NAME_ERROR;
	      d_error->contents.name.pb = ALIAS_DEREF_PB;
	      if (d2a121_dn_ids(&scan,&d_error->contents.name.matched,
				 err_apdu, &er_len) == D2_ERROR)
		   {
		   D2_ASN1_ERR_RETURN_WITH_TRACE
		   }
	      break;
       default :
	      D2_ASN1_ERR_RETURN_WITH_TRACE
       }

     }

return (D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a036_put_nb_att_pb()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function calculates the number of attribute problems*/
/*  in the APDU of attribute error.                                   */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = Error APDU with rs_error.                       */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      nb_info     = Number of attribute errors in apdu.             */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/


static void d2a036_put_nb_att_pb(
  byte * apdu,
  signed32   * nb_info )
{
signed32             i;
D23_at_prob    * at_prob = (D23_at_prob *) apdu;
D23_at_type    * at_type;
D23_av_value   * at_val;
D23_av_info    * av_info;

    for (i = 0,at_prob = (D23_at_prob * ) apdu ;
	 at_prob->d23_aptag == D23_APTAG;
	 i++,at_prob = (D23_at_prob * ) apdu )
    {
       apdu += D23_S_ATPROB;
       at_type = (D23_at_type *) apdu;
       apdu += D23_S_ATT + at_type->d23_atoff;
       av_info = (D23_av_info *) apdu;
       apdu += D23_S_AV;
       if (av_info->d23_avtag == D23_AVTAG)
	   for (; av_info->d23_avnum < 1; av_info->d23_avnum--)
	    { at_val=(D23_av_value *) apdu;
	      apdu += D23_S_ATV + at_val->d23_avoff; /* scan recurring values */
	     }
     }

*nb_info = i;

}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a038_put_attr_error()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function put attribute errors(recurringly) in IDS   */
/*  structure form APDU format.                                       */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      aterror     = Attribute error in IDS structure.               */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      scan        = APDU with at_info structures (recurring).       */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static signed32 d2a038_put_attr_error(
  ids_att_error * aterror,
  byte * scan,
  byte * err_apdu,
  signed32 * apdu_len )
{
signed32             i, err, no_info;
ids_att_pb    * att_list;
D23_at_prob   * at_prob;
const char function_name[] = "d2a038_put_attr_error";

    if (d2a121_dn_ids(&scan, &aterror->object,err_apdu,apdu_len) == D2_ERROR)
      { 
	d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_PERM_ERR,
		    D2_SYS_ERR, D2_NMSYNTAX_ERROR,apdu_len);
	D2_ASN1_ERR_RETURN_WITH_TRACE
       }
    d2a036_put_nb_att_pb(scan, &no_info);
    aterror->nb_pb = no_info;
    if ((aterror->att_pb_list =
	      D27_MALLOC(svc_c_sev_warning,no_info*sizeof(ids_att_pb))) == NULL)
      { 
	d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
	      D2_SYS_ERR, D21_ALLOC_ERROR,apdu_len);
	D2_ASN1_ERR_RETURN_WITH_TRACE
       }
    att_list = aterror->att_pb_list;
    for (i = 0; i < no_info; i++, att_list++)
    {
       at_prob = (D23_at_prob * ) scan ;
       switch (at_prob->d23_apval)
       {
	case D2_ATTRIBUTE_MISSING:
		     err = NO_SUCH_ATT;
		     break;
	case D2_ATSYNTAX_ERROR:
		     err = INV_ATT_SYNTAX_ATT_PB;
		     break;
	case D2_UNDEFINED_ATTR_TYPE:
		     err = UNDEF_ATT_TYPE;
		     break;
	case D2_INAPPR_MATCH:
		     err = INAPP_MATCHING;
		     break;
	case D2_CONSTRAINT_VIOLATION:
		     err = CONSTRAINT_VIOL;
		     break;
	case D2_AT_VALUE_EXISTS:
		      err = ATT_VAL_EXISTS;
       }
       scan += D23_S_ATPROB;
       if ( d2a037_put_att_pb_list(att_list, &scan,
		  err, err_apdu, apdu_len) == D2_ERROR )
	 { free(aterror->att_pb_list);
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
    } /* of for loop */

return (D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a037_put_att_pb_list()                                */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms attribute error in IDS structure*/
/*  to attribute error(only one) in APDU format.                      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      att_list     = Attribute error in IDS Format.                 */
/*        att_pb     = indicating attribute problem code.             */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = Error APDU with at_error.                       */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static signed32 d2a037_put_att_pb_list (
  ids_att_pb * att_list,
  byte     ** apdu,
  signed32          att_pb,
  byte      * err_apdu,
  signed32        * err_len )
{
byte           * p_mess = *apdu;
D23_av_info     * av_info;
D23_av_value    * at_value;
signed32               syntax;
dua_att_value     att_value;
const char function_name[] = "d2a037_put_att_pb_list";

    if (d2a120_copy_att_type(&p_mess, &att_list->att_type, err_apdu, err_len)
	  == D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
    att_list->att_pb = att_pb;
    av_info = (D23_av_info *) p_mess;
    if (av_info->d23_avtag != D23_AVTAG)
	att_list->att_value_defined = FALSE;
    else
    {   att_list->att_value_defined = TRUE;
        p_mess += D23_S_AV;
        syntax =  av_info->d23_asyntx;
        at_value = (D23_av_value *) p_mess;
        p_mess += D23_S_ATV;
	/*
	   copy attribute value from APDU
	*/
	if (d2a031_cp_attr(at_value->d23_avrep, at_value->d23_avlen, p_mess,
				&att_value, err_apdu, err_len) != D2_NOERROR)
	    {   
		D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
	if (dua_att_value_ids(&att_value, &att_list->att_value) == D2_ERROR)
	    {   
		D2_ASN1_ERR_RETURN_WITH_TRACE
	    }

	if (syntax != D2_DISTNAME)
		dua_att_value_free( &att_value);
	p_mess += at_value->d23_avoff;
     }
 
  *apdu = p_mess;

return(D2_NOERROR);
}
