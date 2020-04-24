/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2acommon.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:05  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:49  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:32  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:28:02  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:34  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:17:11  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:37:48  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:46:28  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:05:39  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:38:16  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:10:22  marrek
 * 	Introduce IA5_SYNTAX and T61_SYNTAX
 * 	[1993/02/01  17:52:02  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:13:26  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:11:05  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  16:01:56  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  12:53:45  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:54:58  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:33:59  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  20:09:14  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:03:41  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d2acommon.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:12 $";
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
/* NAME         : d2acommon.c                                        */
/*								      */
/* AUTHOR       : Praveen Gupta, Siemens India                        */
/* DATE         : 9. 1. 1990                                          */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.                   */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-02-09| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/******** LOCAL *********/

static void d2a032_cpy ( byte * , byte * , signed32 , Bool );
static void d2a034_mk_ids_svctrl ( D23_serv_ctrl * , ids_serv_controls * );
static void d2a035_mk_apdu_svctrl ( D23_serv_ctrl * , ids_serv_controls * );
static void d2a132_guide_fr_ipc ( byte ** , guide ** );
static signed32 d2a133_guide_to_ipc ( guide * , byte ** );
static void d2a135_faxnr_fr_ipc ( byte ** , faxNumber ** );
static void d2a145_faxnr_to_ipc ( faxNumber * , byte ** );

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a120_copy_att_type()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      copies attribute type from IDS to APDU-format.                */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*     ids_atype : pointer to ids-structure for attribute-type;       */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu : pointer to APDU-address which points to end of APDU    */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR      function successful                           */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: W. Schmid                                                 */
/**********************************************************************/

signed32 d2a120_copy_att_type(
  byte **apdu,
  ids_att_type * ids_atype,
  byte        * err_apdu,
  signed32       * err_len )
{
const char function_name[] = "d2a120_copy_att_type";
  D23_at_type  * at_type = (D23_at_type *) *apdu;
  byte        * scan = *apdu + D23_S_ATT;

  ids_atype->l = at_type->d23_atlen;
  if ((ids_atype->v = D27_MALLOC(svc_c_sev_warning,ids_atype->l)) == NULL)
   {
      d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
   }
   memcpy(ids_atype->v, scan, ids_atype->l);
   *apdu = scan += at_type->d23_atoff;
   return(D2_NOERROR);
}

signed32 d2a124_copy_attyp_apdu(
  byte ** apdu,
  ids_att_type * ids_atype,
  byte ** beg_apdu,
  signed32   * max_apdu_len,
  byte * err_apdu,
  signed32   * err_len )
{
const char function_name[] = "d2a124_copy_attyp_apdu";
  D23_at_type  * at_type;
  byte        * scan;

    if (d2a033_check_memsize (D23_S_ATT+D2_ALIGN(ids_atype->l), beg_apdu, apdu,
				 max_apdu_len, (D23_rserror *) err_apdu,
				 err_len) != D2_NOERROR)
       {
	 /* not enough memory  */
	 D2_ASN1_ERR_RETURN_WITH_TRACE
        } /* if (rv != D2_NOERROR) */

    at_type = (D23_at_type *) *apdu;
    scan = *apdu + D23_S_ATT;
    at_type->d23_atttag = D23_ATTTAG;
    at_type->d23_atlen = ids_atype->l ;
    memcpy(scan, ids_atype->v, ids_atype->l);
    at_type->d23_atoff = D2_ALIGN(at_type->d23_atlen);
    *apdu = scan += at_type->d23_atoff;
    return(D2_NOERROR);
}

void d2a130_mk_apdu_comres(
  ids_common_res *  com_res,
  byte          ** apdu )
{
 D23_com_res  * c_res = (D23_com_res *) *apdu;

  c_res->d23_crtag = D23_CRSTAG;   /*NOT SUPPORTED IN THIS VERSION */
  c_res->d23_ali_deref = com_res->alias_deref;
  /* SECURITY IS NOT SUPPORTED IN THIS VERSION */

  *apdu += D23_S_CRES;
 }

void d2a081_mk_ids_comres(
  byte          ** apdu,
  ids_common_res *  com_res )
{
 D23_com_res  * c_res = (D23_com_res *) *apdu;

  com_res->requestor_defined = FALSE;   /*NOT SUPPORTED IN THIS VERSION */
  com_res->alias_deref = c_res->d23_ali_deref;
  /* SECURITY IS NOT SUPPORTED IN THIS VERSION */

  *apdu += D23_S_CRES;
 }

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d2a033_check_memsize                                  */
/*                                                                    */
/* SYNTAX:      d2a033_check_memsize (usedmem, begin, act,            */
/*                                    max_len, error, err_len)        */
/*                                                                    */
/*              signed32        usedmem;          ->                      */
/*              byte        **begin,          <>                     */
/*                          **act;            <>                      */
/*              signed32         *max_len;         <>                      */
/*              D23_rserror *error;           <-                      */
/*              signed32         *err_len;         <-                      */
/*                                                                    */
/* DESCRIPTION: This function checks whether there is enough memory   */
/*              for the data  which should be copied into the         */
/*              message (APDU). If there isn't enough memory the      */
/*              function allocates new using                          */
/*              the function re-alloc so that the data  which were     */
/*              already copied are further available. The new pointers*/
/*              to the data were returned. In case of errors an error */
/*              apdu is returned.                                     */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*     signed32     usedmem                size of memory which is        */
/*                                     is required for the data which */
/*                                     should be copied into the      */
/*                                     message                        */
/*                                                                    */
/*     char     **begin                INPUT OUTPUT parameter         */
/*                                     begin of memory                */
/*                                                                    */
/*     char     **act                  INPUT OUTPUT parameter         */
/*                                     pointer to the begin of the    */
/*                                     memory for the new data        */
/*                                                                    */
/*     signed32      *max_len               INPUT OUTPUT parameter         */
/*                                     size of allocted memory        */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*     D23_rserror   *error            error message (contains return */
/*                                     codes in case of errors)       */
/*                                                                    */
/*     signed32           *l_err            length of error message        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    signed32      D2_NOERROR   no error occurred                         */
/*                                                                    */
/*             D2_ERROR     any error occurred                        */
/*                          (e.g. re-allocation of memory fails        */
/*                          error apdu and length of error apdu are   */
/*                          set)                                      */
/*                                                                    */
/* NOTES: The memory for the message must be allocated before         */
/*        calling this function.                                      */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d2a033_check_memsize (
  signed32        usedmem,
  byte       **begin,
  byte       **act,
  signed32         *max_len,
  D23_rserror *error,
  signed32         *err_len )
{
const char function_name[] = "d2a033_check_memsize";
  byte     *end = *begin + *max_len;
  signed32      offset,help;     /* offset to begin of old memory        */
  byte     *new_mem;

  if ((*act + usedmem) >= end) 
     {
     /* not enough memory for new data */
     offset = *act - *begin; /* offset of new data to begin of memory*/
     help = usedmem - (end - *act) +1; 
     help = (help > D2A_MORE_MEM) ? help : D2A_MORE_MEM;
     *max_len += D2_ALIGN(help);     /* calculate new memory size            */
     /* re-alloc new memory */
     new_mem = D27_REALLOC (svc_c_sev_warning,(void *)*begin, *max_len);
     if (new_mem == NULL) 
	{
	/* ERROR: no memory available (must be traced !) */
	d27_010_set_error_apdu (error, D2_TEMP_ERR, D2_SYS_ERR,
				D21_ALLOC_ERROR, err_len);
	D2_ASN1_ERR_RETURN_WITH_TRACE
        } /* re-allocation of memory fails */

     *begin = new_mem;        /* new begin of memory */
     *act = *begin + offset;   /* new begin of memory for new data */
     } /* if (*act + used_mem) ... */

  return (D2_NOERROR);
} /* End d2a033_check_memsize */

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a031_cp_attr()                                        */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Copies the attribute value from APDU in THORN-IDS format      */
/*      ("dua_att_value")                                             */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*                                                                    */
/*      syntax      = attribute syntax                                */
/*      at_len      = attribute length                                */
/*      at_val      = attribute value (in APDU)                       */
/*      valptr      = pointer to THORN IDS attribute value            */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      err_apdu    = error APDU                                      */
/*      err_len     = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        function fails                                */
/*      D2_NOERROR      function successful                           */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/


signed32 d2a031_cp_attr(
signed32              syntax,
signed16           at_len,
byte           *at_val,
dua_att_value   *valptr,
byte           *err_apdu,
signed32             *err_len )
{
const char function_name[] = "d2a031_cp_attr";
byte   *val_ptr = at_val - D23_S_ATV;
byte   *v_ptr;
signed32     return_value = D2_NOERROR;
signed32     * int_val;
signed16  len=at_len;
Bool    * bool_val;
signed16 offset;

if (syntax == D2_ASN1)
  {
  valptr->mode = D2_ASN1;
  valptr->value.asn1_value.l = len;
  valptr->value.asn1_value.v = (asn1)at_val;
  return(D2_NOERROR);
  }

/*
   allocate memory for attribute values
*/

switch (syntax) {
    case D2_DISTNAME :
   	len = sizeof (dua_dist_name);
        break;
    case D2_GUIDE :
	len = d21_132_guide_struct_len((char *)val_ptr);
	break;
    case D2_TTXID_STX  :
	len = d21_131_ttxid_struct_len((char *)val_ptr);
	break;
    case D2_TLXNR_STX :
	len = d21_134_telexnr_struct_len((char *)val_ptr);
	break;
    case D2_FAXNR_STX :
	len = d21_135_faxnr_struct_len((char *)val_ptr);
	break;
    case D2_T61_LIST :
    case D2_PRT_LIST  :
	len = d21_133_T61seq_struct_len((char *)val_ptr);
	break;
    case D2_POST_AD_STX :
    case D2_T61_PR_LIST :
	len = d2a170_t61_pr_seq_len(val_ptr);
	break;
    default :
	break;
}

if (len < 0)
       {
       d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
		     D2_PAR_ERR, D21_BAD_ARGUMENT, err_len);
       D2_ASN1_ERR_RETURN_WITH_TRACE
       }

if (syntax != D2_PSAP_STX 
    && syntax != D2_INTEGER && syntax != D2_BOOLEAN
    && syntax != D2_MHS_DLSP_STX && syntax != D2_MHS_ORADR_STX 
    && syntax != D2_MHS_ORNAME_STX && syntax != D2_CERTIFICATE
    && syntax != D2_CERT_PAIR && syntax != D2_CERT_LIST)
  {
	/* because boolean is 4-bytes in dua_mvr.h */
	/* 4bytes more are allocated this affects guide and fax only  */
  if ((v_ptr = D27_MALLOC(svc_c_sev_warning,len + 4)) == NULL)
    {
      d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR, err_len);
      D2_ASN1_ERR_RETURN_WITH_TRACE
   }
 }

valptr->mode = syntax;


switch (valptr->mode)
{
    case D2_CEXACT :
    case D2_CIGNORE :
    case D2_PRINTABLE :
    case D2_NUMERIC :
    case D2_CIGN_LIST :
    case D2_TELNR_STX :
    case D2_COUNTRY_STX :
    case D2_T61:
    case D2_IA5:
    case D2_T61_PR:
	    valptr->value.str_value = (string) v_ptr;
	    d2a032_cpy(v_ptr, at_val, len, (Bool)TRUE) ;
	    break ;

    case D2_TIME :
	    valptr->value.utc_time_value = (dua_utc_time) v_ptr ;
	    d2a032_cpy(v_ptr, at_val, len, (Bool)TRUE) ;
	    break ;

    case D2_ACCL:
    case D2_PASSWD_STX:
    case D2_OCTET :
	    valptr->value.oct_value.nb = len ;
	    valptr->value.oct_value.contents = v_ptr ;
	    d2a032_cpy(v_ptr, at_val, len, (Bool)FALSE) ;
	    break ;

    case D2_PSAP_STX:
	    *err_len = len;
	    return_value = d2a049_put_psap (at_val,&(valptr->value.psap_value),
					    err_apdu, err_len);
	    break;

    case D2_DISTNAME:
	    valptr->value.name_value = (dua_dist_name *) v_ptr ;
	    return_value = d2a_016_str_to_dn ((char *)at_val,
				valptr->value.name_value, err_apdu, err_len);
	    break;

    case D2_OBJ_IDENT:
	    valptr->value.oid_value.contents.oid_asn1.v = v_ptr;
	    memcpy (v_ptr, at_val, len);
	    valptr->value.oid_value.is_oid_ints = FALSE;
	    valptr->value.oid_value.contents.oid_asn1.l = len;
	    break;

    case D2_BOOLEAN:
		bool_val    =  (Bool *)at_val;
		valptr->value.int_value = * bool_val;
		break;

    case D2_INTEGER:
		int_val    =  (signed32 *) at_val;
		valptr->value.int_value = * int_val;
		break;

    case D2_GUIDE:
	    valptr->value.guide_value = (guide *) v_ptr ;
	    d2a132_guide_fr_ipc (&val_ptr,
				 &valptr->value.guide_value);
	    break;

    case D2_TLXNR_STX:
	    valptr->value.tlx_value = (tlx_number *) v_ptr ;
	    d21_124_telexnr_fr_ipc ((char **)&val_ptr,
			(char **)&valptr->value.tlx_value,FALSE);
	    break;

    case D2_TTXID_STX:
	    valptr->value.ttxid_value = (teletex_id *) v_ptr ;
	    d21_121_ttxid_fr_ipc ((char **)&val_ptr,
			(char **)&valptr->value.ttxid_value,FALSE);
	    break;

    case D2_FAXNR_STX:
	    valptr->value.fax_value = (faxNumber *) v_ptr ;
	    d2a135_faxnr_fr_ipc (&val_ptr,
				&valptr->value.fax_value);
	    break;

    case D2_MHS_PR_DM_STX:
    case D2_PR_DM_STX:
    case D2_INT_LIST:
	    valptr->value.int_seq_value.nb_item = len/sizeof(signed32) ;
	    valptr->value.int_seq_value.int_item = (int *)v_ptr ;
	    d2a032_cpy(v_ptr, at_val, len, FALSE) ;
	    break;

    case D2_POST_AD_STX:
    case D2_T61_PR_LIST :
	    valptr->value.post_value = (t61_pr_seq *) v_ptr;
	    d2a171_t61_pr_seq_fr_ipc(&val_ptr,
			      valptr->value.post_value);
	    break;

    case D2_T61_LIST:
	    valptr->value.t61_seq_value = (t61_seq *) v_ptr ;
	    d21_123_T61seq_fr_ipc((char **)&val_ptr,
			      (char **)&v_ptr,FALSE);
	    break;

    case D2_PRT_LIST:
	    valptr->value.pr_seq_value = (print_seq *) v_ptr ;
	    d21_123_T61seq_fr_ipc((char **)&val_ptr,
			      (char **)&v_ptr,FALSE);
	    break;

    case D2_MHS_DLSP_STX:
	    offset = ((D23_av_value *)val_ptr)->d23_avoff;
	    if (d2a136_dlsp_fr_ipc (offset,at_val, &valptr->value.dls_perm,
				err_apdu, err_len) == D2_ERROR)
   		{
      		  D2_ASN1_ERR_RETURN_WITH_TRACE
   		}
	    break;

    case D2_MHS_ORADR_STX:
	    offset = ((D23_av_value *)val_ptr)->d23_avoff;
	    if (d2a137_oradr_fr_ipc (offset,at_val, &valptr->value.or_address,
				err_apdu, err_len) == D2_ERROR)
   		{
      		  D2_ASN1_ERR_RETURN_WITH_TRACE
   		}
	    break;

    case D2_MHS_ORNAME_STX:
	    offset = ((D23_av_value *)val_ptr)->d23_avoff;
	    if (d2a138_orname_fr_ipc (offset,at_val, &valptr->value.or_name,
				err_apdu, err_len) == D2_ERROR)
   		{
      		  D2_ASN1_ERR_RETURN_WITH_TRACE
   		}
	    break;

    case D2_CERTIFICATE:
	if (d2a_200_certificate_from_apdu(
				(char **)&val_ptr,&(valptr->value.certificate))
		== D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	break;
    case D2_CERT_PAIR:
	if (d2a_201_cert_pair_from_apdu(
				(char **)&val_ptr,&(valptr->value.cert_pair))
		== D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	break;
    case D2_CERT_LIST:
	if (d2a_202_cert_list_from_apdu(
				(char **)&val_ptr,&(valptr->value.cert_list))
		== D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	break;

    default :
	       return_value = D2_ERROR;
   }   /* of switch */

if (return_value != D2_NOERROR) {
       d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_CALL_ERR,
		     D2_PAR_ERR, D21_BAD_ARGUMENT, err_len);
	    }

return(return_value);
}


/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a032_cpy ()                                           */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Copies n bytes from "from" to "to".  If is_string == TRUE  */
/*      "to" is ended with \0.                                        */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      from        = pointer to the source                           */
/*      n           = nr. of bytes to be copied                       */
/*      is_string   = boolean; if true the sink will be ended with \0 */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*                    pointer to the first byte after the end of "to" */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: T. Dumitrescu                                             */
/**********************************************************************/

static void d2a032_cpy(
  byte		*to,
  byte		*from,
  signed32   	n,
  Bool		is_string )
{
if (n > 0)
    {
    while(n--)
	{
        *to++ = *from++ ;
	}
    }
if(is_string == TRUE)
    { 
    *to++ = D2_EOS;
    }
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a123_mk_apdu_cmarg ()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms common-argument in              */
/*  ids interface to the Common arguments in APDU format .            */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      i_comarg = Common arguments in ids format.                    */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu = pointer to octet-string where Common-arguments in APDU */
/*             format is mapped.                                      */
/*  beg_apdu = pointer to begining of APDU 			      */
/*  max_len  = pointer to integer to indicate max. APDUlen            */
/*  err_apdu = pointer to Error-APDU (in case of Error in function)   */
/*  apdu_len = returns length of err_apdu (in case of error)          */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

signed32 d2a123_mk_apdu_cmarg (
  byte   ** apdu,
  ids_common_arg  * i_comarg,
  byte   ** beg_apdu,
  signed32    * max_len,
  byte  * err_apdu,
  signed32 * apdu_len )
{
const char function_name[] = "d2a123_mk_apdu_cmarg";
 byte        * scan = *apdu;
 D23_com_arg  * a_cmarg = (D23_com_arg *) scan;

    a_cmarg->d23_cmtag = D23_CMTAG;
    a_cmarg->d23_cmopr.d23_opnm_rs = i_comarg->op_prog.name_res_ph;
    if (i_comarg->op_prog.next_rdn_defined == TRUE)
       { a_cmarg->d23_cmopr.d23_op_nxtrdn = i_comarg->op_prog.next_rdn; }
     else
       { a_cmarg->d23_cmopr.d23_op_nxtrdn = FALSE; }

    d2a035_mk_apdu_svctrl(&a_cmarg->d23_cmctr, &i_comarg->controls);

    if (i_comarg->alias_rdn_defined == TRUE)
       { a_cmarg->d23_cmali = i_comarg->alias_rdn; }
     else
       { a_cmarg->d23_cmali = FALSE; }

    /* NO EXTENSIONS ARE SUPPORTED  */

    /* NO SECURITY PARAMETERS ARE SUPPORTED */

    scan += D23_S_CARG;
    if ((i_comarg->requestor_defined == TRUE)
         && (d2a122_dn_apdu(D23_RQTAG, &scan, &i_comarg->requestor,
			beg_apdu, max_len, err_apdu, apdu_len) == D2_ERROR))
	  { 
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	   };
    *apdu = scan;
    return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a035_mk_apdu_svctrl()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms service control argument in     */
/*  ids interface to the Common arguments in APDU format .            */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      ctrls = service Controls argument in ids format.              */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      a_svctrl = service controls in APDU Format.                   */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static void d2a035_mk_apdu_svctrl(
  D23_serv_ctrl     * a_svctrl,
  ids_serv_controls * ctrls )
{
    a_svctrl->d23_sctlm = D2_T_UNLIMITED;

    if (ctrls->time_limit_defined == TRUE)
    a_svctrl->d23_sctlm = ctrls->time_limit;

    a_svctrl->d23_scslm = D2_S_UNLIMITED;
    if (ctrls->size_limit_defined == TRUE)
    a_svctrl->d23_scslm = ctrls->size_limit;

    a_svctrl->d23_scpri = ctrls->priority;

    a_svctrl->d23_scref = D2_WORLD_SCOPE;
    if (ctrls->scope_ref_defined == TRUE)
    a_svctrl->d23_scref = ctrls->scope_ref;

    a_svctrl->d23_scopt = D2_DONT_STORE;

    if (ctrls->options != IDS_UNDEFINED)
    {
       if ((ctrls->options & DONT_USE_COPY) ==  DONT_USE_COPY)
       a_svctrl->d23_scopt= a_svctrl->d23_scopt | D2_NOCACHE;
   
       if ((ctrls->options & LOCAL_SCOPE) == LOCAL_SCOPE)
       a_svctrl->d23_scopt = a_svctrl->d23_scopt | D2_LOCALSCOPE;
   
       if ((ctrls->options & PREFER_CHAINING) == PREFER_CHAINING)
	a_svctrl->d23_scopt = a_svctrl->d23_scopt | D2_PREF_CHAINING;

       if ((ctrls->options & CHAINING_PROHIBITED) == CHAINING_PROHIBITED)
       a_svctrl->d23_scopt = a_svctrl->d23_scopt | D2_NOCHAINING ;
    
       a_svctrl->d23_scopt = a_svctrl->d23_scopt | D2_DONT_STORE ;

       if ((ctrls->options & DONT_DEREFERENCE_ALIASES) == DONT_DEREFERENCE_ALIASES)
       a_svctrl->d23_scopt = a_svctrl->d23_scopt | D2_DREFALIAS;

    }
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a106_ids_atval_to_atval()                             */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms the ids_att_value structure from THORN-IDS format  */
/*      to APDU format (D23_at_info).                                 */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      attr_val      = attribute value        in THORN-IDS format    */
/*                      (ids_attvalue structure)                      */
/*      at_len        = pointer to write the attribute length to      */
/*      at_off        = pointer to write the attribute offset to      */
/*      at_rep        = pointer to write the attribute repr. to       */
/*      max_apdu_len  = max. length of allocated APDU memory          */
/*      act_apdu      = pointer to the APDU where to write the        */
/*                      attribute value                               */
/*      beg_apdu      = start address of APDU                         */
/*      max_apdu_len  = max. length of allocated APDU memory          */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      at_len        = pointer to write the attribute length to      */
/*      at_off        = pointer to write the attribute offset to      */
/*      act_apdu    = pointer to next tag in the APDU where to write  */
/*                    the next block                                  */
/*      beg_apdu    = (new) start address of APDU 		      */
/*      max_apdu_len= (new) max. length of allocated APDU memory      */
/*      err_apdu    = error APDU (memory error)                       */
/*      err_len     = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        function fails                                */
/*      D2_NOERROR      function successful                           */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR:    Praveen Gupta                                          */
/**********************************************************************/

signed32 d2a106_ids_atval_to_atval(
  ids_att_value       *ids_val,
  signed32                 syntax,
  signed16              * val_len,
  signed16              * val_off,
  unsigned16	      * val_rep,
  byte               **act_apdu,
  byte               **beg_apdu,
  signed32                 *max_apdu_len,
  byte               *err_apdu,
  signed32                 *err_len )
{
const char function_name[] = "d2a106_ids_atval_to_atval";
  signed32             rv;
  register boolean             cp_val = TRUE;
  register signed32             return_value = D2_NOERROR;
  Bool         bool_val;
  dua_att_value1   attr_val;
  char      *from_val; /* pointer to value in ids_info */
  byte * sav_apdu = *act_apdu;
  signed32 len;

switch (syntax)
  {
  case D2_ASN1 :
		*val_rep = D2_ASN1;
		break;
  case D2_DISTNAME :
		if (dua_dist_name_syntax_dua(ids_val,&attr_val.value.name_value)
						!= D2_NOERROR)
		  {
		  *val_rep = D2_ASN1;
		  }
		else
		  {
		  *val_rep = D2_DISTNAME;
		  }
		break;
  default:
		if (dua_att_value_dua( ids_val, &attr_val, syntax) == D2_ERROR)
		  {
		  syntax = *val_rep = D2_ASN1;
		  }
		else
		  {
		  *val_rep = attr_val.mode;
		  }
}

if ( (*val_rep == D2_GUIDE) ||
     (*val_rep == D2_FAXNR_STX)    ||
     (*val_rep == D2_POST_AD_STX)  ||
     (*val_rep == D2_T61_PR_LIST) )
  {
#define D2A_BASE_MEMORY 1024
  rv = d2a033_check_memsize(4*D2A_BASE_MEMORY, beg_apdu,
		act_apdu,max_apdu_len,(D23_rserror *)err_apdu,err_len);
  if (rv != D2_NOERROR)
    {
    /* not enough memory for attribute value */
    D2_ASN1_ERR_RETURN_WITH_TRACE
    } /* if (rv != D2_NOERROR) */
  } /* end of special syntaxes */

	      /* calculate length of value and copy value */
switch (*val_rep)
{
  case D2_CEXACT :
  case D2_CIGNORE :
  case D2_PRINTABLE :
  case D2_NUMERIC :
  case D2_CIGN_LIST :
  case D2_TELNR_STX :
  case D2_COUNTRY_STX :
  case D2_T61:
  case D2_IA5:
  case D2_T61_PR:
       from_val = attr_val.value.str_value;
       if (from_val != (string)0)
       	  *val_len = strlen (from_val);
       else
	  *val_len = 0;
       break ;
  case D2_TIME :
       from_val = attr_val.value.utc_time_value;
       if (from_val != (string)0)
          *val_len = strlen (from_val);
       else
	  *val_len = 0;
       break ;
  case D2_ACCL:
  case D2_PASSWD_STX:
  case D2_OCTET :
       if (attr_val.value.oct_value.nb != 0)
       {
         from_val = (char *)(attr_val.value.oct_value.contents);
         *val_len = attr_val.value.oct_value.nb;
	}
       else
         *val_len = 0;
       break;
  case D2_PSAP_STX:
       cp_val = FALSE;
       *act_apdu -= sizeof(D23_av_value);
       if ((return_value = d2a061_get_psap (
			   &(attr_val.value.psap_value),
			   TRUE,
			   act_apdu, beg_apdu, max_apdu_len,
			   err_apdu, err_len) ) != D2_NOERROR )
	  {
          *act_apdu = sav_apdu;
	  }
	else
	  {
	  dua_att_value1_free(&attr_val);
	  }
       break;
  case D2_DISTNAME:
       cp_val = FALSE;
       *act_apdu -= sizeof(D23_av_value);
       if ((return_value = d2a017_dn_to_str(D23_EOMTAG,
				  attr_val.value.name_value,
				  act_apdu, beg_apdu, max_apdu_len,
				  err_apdu, err_len) ) != D2_NOERROR )
	  {
          *act_apdu = sav_apdu;
	  }
       break;
  case D2_OBJ_IDENT:
       from_val = (char *)attr_val.value.oid_value.contents.oid_asn1.v;
       *val_len = attr_val.value.oid_value.contents.oid_asn1.l;
       break ;

  case D2_BOOLEAN:
	bool_val = (Bool) attr_val.value.int_value;
        from_val = (char *)&bool_val;
        *val_len = sizeof(Bool);
	break;
  case D2_INTEGER:
	from_val = (char *)&attr_val.value.int_value;
        *val_len = sizeof(signed32);
	break;

  case D2_GUIDE:
        *act_apdu -= sizeof(D23_av_value);
        cp_val = FALSE;
	if ((return_value = d2a133_guide_to_ipc(&attr_val.value.guide_value,
			act_apdu) ) != D2_NOERROR)
	  {
	  *act_apdu=sav_apdu;
	  }
	else
	  {
	  dua_att_value1_free(&attr_val);
	  }
	break;

  case D2_TLXNR_STX:
       *act_apdu -= sizeof(D23_av_value);
	len = d21_114_telexnr_msglen(
			(D2_telex_number *)&attr_val.value.tlx_value);
	rv = d2a033_check_memsize(len+sizeof(D23_av_value), beg_apdu,
	       act_apdu,max_apdu_len,(D23_rserror *)err_apdu,err_len);
	if (rv != D2_NOERROR)
	  {
	  /* not enough memory for attribute value */
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  } /* if (rv != D2_NOERROR) */

	d21_104_telexnr_to_ipc((D2_telex_number *)&attr_val.value.tlx_value,
			(char **)act_apdu);
	cp_val = FALSE;
	dua_att_value1_free(&attr_val);
	break;

  case D2_TTXID_STX:
       *act_apdu -= sizeof(D23_av_value);
	len = d21_111_ttxid_msglen(
			(D2_ttxid *)&attr_val.value.ttxid_value);
	rv = d2a033_check_memsize(len+sizeof(D23_av_value), beg_apdu,
	       act_apdu,max_apdu_len,(D23_rserror *)err_apdu,err_len);
	if (rv != D2_NOERROR)
	  {
	  /* not enough memory for attribute value */
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  } /* if (rv != D2_NOERROR) */

	d21_101_ttxid_to_ipc((D2_ttxid *)&attr_val.value.ttxid_value,
			(char **)act_apdu);
	cp_val = FALSE;
	dua_att_value1_free(&attr_val);
	break;

  case D2_FAXNR_STX:
       *act_apdu -= sizeof(D23_av_value);
	d2a145_faxnr_to_ipc(&attr_val.value.fax_value,
			act_apdu);
	cp_val = FALSE;
	dua_att_value1_free(&attr_val);
	break;

  case D2_MHS_PR_DM_STX:
  case D2_PR_DM_STX:
  case D2_INT_LIST:
       from_val = (char *)attr_val.value.int_seq_value.int_item;
       *val_len = attr_val.value.int_seq_value.nb_item * sizeof(signed32);
       break;  

  case D2_POST_AD_STX:
  case D2_T61_PR_LIST:
       *act_apdu -= sizeof(D23_av_value);
	d2a172_t61_pr_seq_to_ipc(
		&attr_val.value.post_value, act_apdu);
	cp_val = FALSE;
	dua_att_value1_free(&attr_val);
	break;

  case D2_T61_LIST:
       *act_apdu -= sizeof(D23_av_value);
	len = d21_113_T61seq_msglen(
			(D2_T61_seq *)&attr_val.value.t61_seq_value);
	rv = d2a033_check_memsize(len+sizeof(D23_av_value), beg_apdu,
	       act_apdu,max_apdu_len,(D23_rserror *)err_apdu,err_len);
	if (rv != D2_NOERROR)
	  {
	  /* not enough memory for attribute value */
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  } /* if (rv != D2_NOERROR) */

	d21_103_T61seq_to_ipc((D2_T61_seq *)&attr_val.value.t61_seq_value,
			(char **)act_apdu);
	cp_val = FALSE;
	dua_att_value1_free(&attr_val);
	break;

  case D2_PRT_LIST:
       *act_apdu -= sizeof(D23_av_value);
	len = d21_113_T61seq_msglen(
			(D2_T61_seq *)&attr_val.value.pr_seq_value);
	rv = d2a033_check_memsize(len+sizeof(D23_av_value), beg_apdu,
	       act_apdu,max_apdu_len,(D23_rserror *)err_apdu,err_len);
	if (rv != D2_NOERROR)
	  {
	  /* not enough memory for attribute value */
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  } /* if (rv != D2_NOERROR) */

	d21_103_T61seq_to_ipc((D2_T61_seq *)&attr_val.value.pr_seq_value,
			(char **)act_apdu);
	cp_val = FALSE;
	dua_att_value1_free(&attr_val);
	break;

  case D2_MHS_DLSP_STX:
       *act_apdu -= sizeof(D23_av_value);
       cp_val = FALSE;
       if ((return_value = d2a139_dlsp_to_ipc(&attr_val.value.dls_perm, 
				  act_apdu, beg_apdu, max_apdu_len,
				  err_apdu) ) != D2_NOERROR )
	  {
          *act_apdu = sav_apdu;
	  }
	else
	  {
	  dua_att_value1_free(&attr_val);
	  }
	break;

  case D2_MHS_ORADR_STX:
       *act_apdu -= sizeof(D23_av_value);
       cp_val = FALSE;
       if ((return_value = d2a140_oradr_to_ipc(&attr_val.value.or_address, 
				  act_apdu, beg_apdu, max_apdu_len,
				  err_apdu) ) != D2_NOERROR )
	  {
          *act_apdu = sav_apdu;
	  }
	else
	  {
	  dua_att_value1_free(&attr_val);
	  }
	break;

  case D2_MHS_ORNAME_STX:
       *act_apdu -= sizeof(D23_av_value);
       cp_val = FALSE;
       if ((return_value = d2a141_orname_to_ipc(&attr_val.value.or_name,
				  act_apdu, beg_apdu, max_apdu_len,
				  err_apdu) ) != D2_NOERROR )
	  {
          *act_apdu = sav_apdu;
	  }
	else
	  {
	  dua_att_value1_free(&attr_val);
	  }
	break;

  case D2_ASN1:
	*val_len = ids_val->l;
	from_val = (char *)ids_val->v;
	break;

  case D2_CERTIFICATE:
	*act_apdu -= sizeof(D23_av_value);
	len = d2a_250_certificate_msg_len(
			&(attr_val.value.certificate));
	rv = d2a033_check_memsize(len+sizeof(D23_av_value), beg_apdu,
	       act_apdu,max_apdu_len,(D23_rserror *)err_apdu,err_len);
	if (rv != D2_NOERROR)
	  {
	  /* not enough memory for attribute value */
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  } /* if (rv != D2_NOERROR) */

	if (d2a_210_certificate_to_apdu(
			&(attr_val.value.certificate),(char **)act_apdu)
						== D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	cp_val = FALSE;
	dua_att_value1_free(&attr_val);
	break;
  case D2_CERT_PAIR:
	*act_apdu -= sizeof(D23_av_value);
	len = d2a_251_cert_pair_msg_len(
			&(attr_val.value.cert_pair));
	rv = d2a033_check_memsize(len+sizeof(D23_av_value), beg_apdu,
	       act_apdu,max_apdu_len,(D23_rserror *)err_apdu,err_len);
	if (rv != D2_NOERROR)
	  {
	  /* not enough memory for attribute value */
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  } /* if (rv != D2_NOERROR) */

	if (d2a_211_cert_pair_to_apdu(
			&(attr_val.value.cert_pair),(char **)act_apdu)
						== D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	cp_val = FALSE;
	dua_att_value1_free(&attr_val);
	break;
  case D2_CERT_LIST:
	*act_apdu -= sizeof(D23_av_value);
	len = d2a_252_cert_list_msg_len(
			&(attr_val.value.cert_list));
	rv = d2a033_check_memsize(len+sizeof(D23_av_value), beg_apdu,
	       act_apdu,max_apdu_len,(D23_rserror *)err_apdu,err_len);
	if (rv != D2_NOERROR)
	  {
	  /* not enough memory for attribute value */
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  } /* if (rv != D2_NOERROR) */

	if (d2a_212_cert_list_to_apdu(
			&(attr_val.value.cert_list),(char **)act_apdu)
						== D2_ERROR)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	cp_val = FALSE;
	dua_att_value1_free(&attr_val);
	break;
  default :
       D2_ASN1_ERR_RETURN_WITH_TRACE
} /* switch (mode) */

if ( return_value != D2_NOERROR )
  {
  *val_rep = syntax = D2_ASN1;
  *val_len = ids_val->l;
  from_val = (char *)ids_val->v;
  cp_val = TRUE;
  }

*val_off = D2_ALIGN (*val_len);

	      /* copy value if necessary */
if ( cp_val == TRUE && *val_len != 0)
  {
      /* check memory length */
      rv = d2a033_check_memsize((*val_off)+sizeof(D23_av_value), beg_apdu,
	       act_apdu,max_apdu_len,(D23_rserror *)err_apdu,err_len);
      if (rv != D2_NOERROR)
        {
	 /* not enough memory for attribute value */
	 D2_ASN1_ERR_RETURN_WITH_TRACE
        } /* if (rv != D2_NOERROR) */

      /* copy value */
      d2a032_cpy (*act_apdu, (byte *)from_val, *val_len, FALSE);
      if ( syntax != D2_ASN1 )
	{
        dua_att_value1_free(&attr_val);
	}
      *act_apdu += *val_off;

  } /* if (cp_val == TRUE) */
else
  {
  if ( syntax == D2_DISTNAME )
    {
    d_name_free(attr_val.value.name_value);
    free(attr_val.value.name_value);
    }
  }

return(D2_NOERROR);
} /* End d2a106_ids_atval_to_atval*/

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a080_mk_ids_comarg()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms common argument in APDU format  */
/*  to the common argument of the ids interface.                      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      a_comarg = Common arguments in APDU Format.                   */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      com_arg = Common argument in ids format.                    */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

void d2a080_mk_ids_comarg(
  byte ** scan,
  ids_common_arg * com_arg )
{
D23_com_arg * a_comarg = (D23_com_arg *) *scan;

    com_arg->requestor_defined = FALSE;    /* requestor is never given
					      by siemens interface     */
    com_arg->op_prog.name_res_ph = a_comarg->d23_cmopr.d23_opnm_rs;
    com_arg->op_prog.next_rdn_defined = FALSE;
    if ((a_comarg->d23_cmopr.d23_opnm_rs == D2_PROCEEDING) 
        && (a_comarg->d23_cmopr.d23_op_nxtrdn != 0))
     {
       com_arg->op_prog.next_rdn_defined = TRUE;
       com_arg->op_prog.next_rdn = a_comarg->d23_cmopr.d23_op_nxtrdn;
      }

    com_arg->alias_rdn_defined = FALSE;
    if (a_comarg->d23_cmali != D2_NOT_DEFINED)
     {
       com_arg->alias_rdn_defined = TRUE;
       com_arg->alias_rdn = a_comarg->d23_cmali;
      }

    com_arg->extensions_defined = FALSE;    /* extensions are never given
					       by siemens interface     */
    d2a034_mk_ids_svctrl(&a_comarg->d23_cmctr, &com_arg->controls);

    *scan += D23_S_CARG;
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a034_mk_ids_svctrl()                                  */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*           This function transforms common argument in APDU format  */
/*  to the service Control argument of the ids interface.             */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      a_comarg = Service-controls in APDU Format.                   */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      ctrls = Service-controls in ids format.                       */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        transformation fails                          */
/*                      (err_apdu and err_len will be returned)       */
/*      D2_NOERROR      transformation is successful                  */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Praveen Gupta                                             */
/**********************************************************************/

static void d2a034_mk_ids_svctrl(
  D23_serv_ctrl * a_svctrl,
  ids_serv_controls * ctrls )
{
    ctrls->time_limit_defined = (a_svctrl->d23_sctlm > 0);
    ctrls->size_limit_defined = (a_svctrl->d23_scslm > 0);
    ctrls->scope_ref_defined = FALSE;
    if (a_svctrl->d23_scref != D2_WORLD_SCOPE)
     {
	ctrls->scope_ref_defined = TRUE;
	ctrls->scope_ref = a_svctrl->d23_scref;
      }

    ctrls->priority = a_svctrl->d23_scpri;
    ctrls->time_limit = a_svctrl->d23_sctlm;
    ctrls->size_limit = a_svctrl->d23_scslm;

    ctrls->options = 0;

    if ((a_svctrl->d23_scopt & D2_NOCACHE) == D2_NOCACHE)
    ctrls->options = ctrls->options | DONT_USE_COPY;

    if ((a_svctrl->d23_scopt & D2_LOCALSCOPE) == D2_LOCALSCOPE)
    ctrls->options = ctrls->options | LOCAL_SCOPE;

    if ((a_svctrl->d23_scopt & D2_NOCHAINING) == D2_NOCHAINING)
    ctrls->options = ctrls->options | CHAINING_PROHIBITED;

    if ((a_svctrl->d23_scopt & D2_PREF_CHAINING) == D2_PREF_CHAINING)
    ctrls->options = ctrls->options | PREFER_CHAINING;

    if ((a_svctrl->d23_scopt & D2_DREFALIAS) == D2_DREFALIAS)
    ctrls->options = ctrls->options | DONT_DEREFERENCE_ALIASES;

    if (ctrls->options == 0)
	ctrls->options = IDS_UNDEFINED;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d2a132_guide_fr_ipc     (in C-module  d2acommon)     */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        02.04.91                                              */
/*                                                                    */
/* SYNTAX       :      void d2a132_guide_fr_ipc (in_msg, out)         */
/*                      byte        **in_msg;                        */
/*                      guide        **out;                           */
/*                                                                    */
/* DESCRIPTION: The function converts the guide attributes from       */
/*              string format to structure format                     */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: ptr-ptr to guide-message              */
/*                              (preceded by structure D23_av_value)  */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr-ptr to guide-attributes (struct.)    */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/* STANDARD-HEADER:     None                                          */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                  memcpy()                                          */
/*                  d21_128_crit_fr_ipc()                             */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static void d2a132_guide_fr_ipc(
  byte **in_msg,
  guide **out )
{
/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/
byte          *in_msg_p;
byte          *out_p = (byte *) *out;
byte          *mem_ptr;
D23_av_value   *in_av = (D23_av_value *) *in_msg;
d21_obj_len    *d21_object_len;
guide          * dua_guide;
signed16            i_size, i_offset;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
in_msg_p = *in_msg + D23_S_ATV;             /* Startaddr. (read)      */
dua_guide = *out;
/*--------------------------------------------------------------*/
/* assignment of obj_defined and obj                      */
/*--------------------------------------------------------------*/
d21_object_len = (d21_obj_len *) in_msg_p;
out_p += sizeof (guide);          /* out_p to  obj.v*/
in_msg_p += sizeof (d21_obj_len);    /* in_msg_p to object_id   */

i_size = d21_object_len -> d21_length;
i_offset = d21_object_len -> d21_offset;

if (i_offset > 0)
   {
      dua_guide->obj_defined = TRUE;
      dua_guide->obj.l = i_size;
      dua_guide->obj.v = out_p;
      (void) memcpy (dua_guide->obj.v, in_msg_p, i_size);
      out_p += i_size;
      in_msg_p += i_offset;
   }
else
   {
      dua_guide->obj_defined = FALSE;
      dua_guide->obj.l = 0;
      dua_guide->obj.v = NULL;
   }

dua_guide ->crit.value.c_type = (crit_item *) out_p;

/*--------------------------------------------------------------*/
/* call function d21_128_crit_fr_ipc                            */
/*--------------------------------------------------------------*/
mem_ptr = (byte *) &dua_guide->crit;
d21_128_crit_fr_ipc ((char **)&in_msg_p, (char **)&mem_ptr);

*in_msg += D23_S_ATV + in_av->d23_avoff;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d2a133_guide_to_ipc     (in C-module  d2acommon)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        08.03.91                                              */
/*                                                                    */
/* SYNTAX       :      signed32 d2a133_guide_to_icp (in, out)              */
/*                      guide    *in;                                 */
/*                      byte       **out;                            */
/*                                                                    */
/* DESCRIPTION: The function converts the guide attributes to         */
/*              string format                                         */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to guide-attributes               */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr to ptr to message                    */
/*                                                                    */
/* RETURN VALUE :       ERROR / NOERROR				      */
/*                                                                    */
/* STANDARD-HEADER:     None                                          */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                  memcpy()                                          */
/*                  d21_108_crit_to_ipc()                             */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d2a133_guide_to_ipc(
  guide *in,
  byte * *out )
{
const char function_name[] = "d2a133_guide_to_ipc";
byte *out_p = *out + D23_S_ATV;
D23_av_value * av_out = (D23_av_value *) *out;

auto D2_criteria   *criteria_p;
struct {
	 signed16 offset;
	 signed16 length;
       } object_len;
byte *in_str_p;
signed32            i_size, nb_fillers, i;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------*/
/* Object-identifier in Ausgabestring uebertragen               */
/*--------------------------------------------------------------*/
if (in -> obj_defined == TRUE)
   {
      i_size = in -> obj.l;
      in_str_p = in -> obj.v;
   }
else
   i_size = 0;

object_len.length = i_size;
object_len.offset = D2_ALIGN (i_size);

(void) memcpy
   ((char *) out_p, (char *) &object_len, sizeof(object_len));
out_p += sizeof(object_len);

if (in -> obj_defined == TRUE)
   {
      (void) memcpy ((char *) out_p, (char *) in_str_p, i_size);
      out_p += i_size;
   }

nb_fillers = D2_ALIGN (i_size) - i_size;
for (i=0; i < nb_fillers; i++)
   {
      *out_p='\0';
      out_p++;
   }

/*--------------------------------------------------------------*/
/* copy criteria to output-string                                */
/*--------------------------------------------------------------*/
criteria_p = (D2_criteria *) &(in -> crit);
if (d21_108_crit_to_ipc (criteria_p, (char **)&out_p) != D2_NOERROR )
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

av_out->d23_avlen = out_p - (*out + D23_S_ATV);
av_out->d23_avoff = D2_ALIGN(av_out->d23_avlen);
*out += D23_S_ATV + av_out->d23_avoff;
return(D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d2a135_faxnr_fr_ipc     (in C-module  d2acommon)     */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        28.03.91                                              */
/*                                                                    */
/* SYNTAX       :      void d2a135_faxnr_fr_ipc (in_msg, out)         */
/*                      byte        **in_msg;                        */
/*                      faxNumber        **out;                       */
/*                                                                    */
/* DESCRIPTION: The function converts the faxnr attributes from       */
/*              string format to structure format                     */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: ptr-ptr to faxnr-message              */
/*                              (preceded by structure D23_av_value)  */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr-ptr to faxnr-attributes (struct.)    */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/* STANDARD-HEADER:     None                                          */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                  memcpy()                                          */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static void d2a135_faxnr_fr_ipc (
  byte * *in_msg,
  faxNumber * *out )
{
/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/
byte          *out_p, *in_msg_p = *in_msg + D23_S_ATV;
D23_av_value   *in_av = (D23_av_value *) *in_msg;
signed16            i_size, i_offset;
d21_faxnr      * faxnr_p = (d21_faxnr *) in_msg_p;
signed32           *non_basic_parm;
faxNumber   * faxnr = *out;
/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
out_p = (byte *) *out;                       /* Startaddr. (write)   */
i_size =  faxnr_p -> d2_str_len;
i_offset = faxnr_p -> d2_offset;

out_p += sizeof (faxNumber);
in_msg_p += sizeof (d21_faxnr);

faxnr-> tel_nr.nb = i_size;
faxnr-> tel_nr.contents = out_p;
(void) memcpy ((char *)out_p, (char *) in_msg_p, i_size);
out_p += i_size;
if (i_offset > 0)
   {
      in_msg_p += i_offset;
      non_basic_parm = (signed32 *) in_msg_p;
      faxnr->G3_par  = 0;
      if ((*non_basic_parm & D2_A3_WIDTH) ==  D2_A3_WIDTH)
       faxnr->G3_par = faxnr->G3_par | A3_WIDTH;
   
       if ((*non_basic_parm & D2_B4_LENGTH) == D2_B4_LENGTH)
       faxnr->G3_par = faxnr->G3_par | B4_LENGTH;
   
       if ((*non_basic_parm & D2_B4_WIDTH) == D2_B4_WIDTH)
       faxnr->G3_par = faxnr->G3_par | B4_WIDTH ;
    
       if ((*non_basic_parm & D2_FINE_RESOLUTION) == D2_FINE_RESOLUTION)
       faxnr->G3_par = faxnr->G3_par | FINE_RES ;
    
       if ((*non_basic_parm & D2_TWO_DIMENSIONAL) == D2_TWO_DIMENSIONAL)
       faxnr->G3_par = faxnr->G3_par | TWO_DIM ;
    
       if ((*non_basic_parm & D2_UNCOMPRESSED) == D2_UNCOMPRESSED)
       faxnr->G3_par = faxnr->G3_par | UNCOMPRESSED ;
    
       if ((*non_basic_parm & D2_UNLIMITED_LENGTH) == D2_UNLIMITED_LENGTH)
       faxnr->G3_par = faxnr->G3_par | UNLIMITED ;
    
   }
else
   {
      faxnr-> G3_par = OPTIONAL_UNDEF;
   }

*in_msg += D23_S_ATV + in_av->d23_avoff;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d2a145_faxnr_to_ipc     (in C-module  d2acommon)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        05.03.91                                              */
/*                                                                    */
/* SYNTAX       :      void d2a145_faxnr_to_icp (in, out)             */
/*                      faxnumber *in;                                */
/*                      byte       **out;                            */
/*                                                                    */
/* DESCRIPTION: The function converts the faxnr attributes to         */
/*              string format                                         */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to faxnr-attributes               */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr to ptr to message                    */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/* STANDARD-HEADER:     None                                          */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                  memcpy()                                          */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

static void d2a145_faxnr_to_ipc(
  faxNumber *in,
  byte * *out )
{
/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/
/* Laufvariable fuer Zuweisungen */
byte *out_p = *out + D23_S_ATV;
D23_av_value * av_out = (D23_av_value *) *out;

byte *in_str_p;
signed32            i, i_size, nb_fillers;
d21_faxnr      faxnr_struct;
signed32         * non_basic_parm;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
in_str_p=in->tel_nr.contents;

i_size = in -> tel_nr.nb;

faxnr_struct.d2_str_len = i_size;

/*-----------------------------------------------------------*/
/* assign and copy d21_faxnr_struct                          */
/*-----------------------------------------------------------*/
if (in -> G3_par != OPTIONAL_UNDEF)
    faxnr_struct.d2_offset = D2_ALIGN(i_size);
else
    faxnr_struct.d2_offset = -1;

(void) memcpy
   ((char *) out_p, (char *) &faxnr_struct, sizeof(faxnr_struct));
out_p += sizeof(faxnr_struct);

/*-----------------------------------------------------------*/
/* copy octet-string                                         */
/*-----------------------------------------------------------*/
(void) memcpy ((char *) out_p, (char *) in_str_p,i_size);

out_p += i_size;

/*-----------------------------------------------------------*/
/* copy non-basic parameter to output-string                 */
/*-----------------------------------------------------------*/
if (in -> G3_par != OPTIONAL_UNDEF)
   {
      nb_fillers = D2_ALIGN(i_size) - i_size;
      for (i=0; i<nb_fillers; i++)
	 {
	    *out_p = '\0';
	    out_p++;
	 }

      non_basic_parm  = (signed32 *) out_p;
      *non_basic_parm  = 0;
      if ((in->G3_par & A3_WIDTH) ==  A3_WIDTH)
       *non_basic_parm = *non_basic_parm | D2_A3_WIDTH;
   
       if ((in->G3_par & B4_LENGTH) == B4_LENGTH)
       *non_basic_parm = *non_basic_parm | D2_B4_LENGTH;
   
       if ((in->G3_par & B4_WIDTH) == B4_WIDTH)
       *non_basic_parm = *non_basic_parm | D2_B4_WIDTH ;
    
       if ((in->G3_par & FINE_RES) == FINE_RES)
       *non_basic_parm = *non_basic_parm | D2_FINE_RESOLUTION ;
    
       if ((in->G3_par & TWO_DIM) == TWO_DIM)
       *non_basic_parm = *non_basic_parm | D2_TWO_DIMENSIONAL ;
    
       if ((in->G3_par & UNCOMPRESSED) == UNCOMPRESSED)
       *non_basic_parm = *non_basic_parm | D2_UNCOMPRESSED ;
    
       if ((in->G3_par & UNLIMITED) == UNLIMITED)
       *non_basic_parm = *non_basic_parm | D2_UNLIMITED_LENGTH ;
      out_p += sizeof(signed32);
   }

av_out->d23_avlen = out_p - (*out + D23_S_ATV);
av_out->d23_avoff = D2_ALIGN(av_out->d23_avlen);
*out += D23_S_ATV + av_out->d23_avoff;
}
