/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21sch.c,v $
 * Revision 1.2.745.2  1996/02/18  18:17:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:31  marty]
 *
 * Revision 1.2.745.1  1995/12/08  15:54:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:37  root]
 * 
 * Revision 1.2.743.3  1994/05/10  15:54:11  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:33:15  marrek]
 * 
 * Revision 1.2.743.2  1994/03/23  15:14:38  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:29:10  keutel]
 * 
 * Revision 1.2.743.1  1994/02/22  18:43:30  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:35:59  marrek]
 * 
 * Revision 1.2.741.2  1993/10/14  17:27:32  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:25:54  keutel]
 * 
 * Revision 1.2.741.1  1993/10/13  17:32:06  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:41:16  keutel]
 * 
 * Revision 1.2.6.2  1993/08/11  06:37:07  marrek
 * 	time limit is exceeded also if max_time=curr_time.
 * 	[1993/08/03  09:24:55  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/30  10:59:37  marrek]
 * 
 * Revision 1.2.4.5  1993/02/02  10:16:39  marrek
 * 	Insert <pthread.h>
 * 	use const decalartions
 * 	introduce D2_IA5
 * 	[1993/02/02  09:26:44  marrek]
 * 
 * Revision 1.2.4.4  1992/12/31  19:48:46  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:26:13  bbelch]
 * 
 * Revision 1.2.4.3  1992/12/17  09:56:22  marrek
 * 	Fix time limit problem
 * 	[1992/12/16  10:22:37  marrek]
 * 
 * Revision 1.2.4.2  1992/11/27  21:37:10  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:42:33  marrek]
 * 
 * Revision 1.2.2.3  1992/06/02  03:35:01  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:38:26  zeliff]
 * 
 * Revision 1.2.2.2  1992/04/27  19:57:47  melman
 * 	Files from Helmut's visit
 * 	[1992/04/27  19:52:35  melman]
 * 
 * Revision 1.2  1992/01/19  22:13:12  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21sch.c,v $ $Revision: 1.2.745.2 $ $Date: 1996/02/18 18:17:39 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*
* daton **************************************************************
*                                                                    *
* TYPE         :  MODUL                                              *
*                                                                    *
* NAME         :  d21sch.c                                           *
*                                                                    *
* AUTHOR       :  J.Forster                                          *
* DATE         :   1.02.88                                           *
*                                                                    *
* COMPONENT    :  Directory Service-V2 / Search-Operation            *
*                                                                    *
* DOK.-REF.    :  Design-Specification,                              *
*                                                                    *
* PRD#/VERS.   :                                                     *
*                                                                    *
* DESCRIPTION  :  subroutines for search-operation                   *
*                                                                    *
* D2_SYSTEM-DEPENDENCES:                                             *
*                                                                    *
* HISTORY      :                                                     *
*                                                                    *
* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   *
* datoff *************************************************************
*/

#include <gds.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <d21dua.h>

/******** LOCAL *********/

static d2_ret_val d21_sc_realloc(char **,char **,char **,signed32,D2_pbhead *);
static void d21_sc4_new_addr ( D2_a_value * , signed32 );
static void d21_sc5_orname_new_addr ( D2_OR_name ** , signed32 );
static void d21_sc6_oraddr_new_addr ( D2_OR_address ** , signed32 );
static void d21_sc7_str_list_new_addr ( D2_str_list * , signed32 );
static void d21_sc8_criteria_new_addr ( D2_criteria * , signed32 );

static void d21_sc20_certificate_new_addr ( Certificate ** , signed32 );
static void d21_sc21_cert_pair_new_addr ( Cert_pair ** , signed32 );
static void d21_sc22_cert_list_new_addr ( Cert_list ** , signed32 );
static void d21_sc24_alg_id_new_addr ( Alg_id * , signed32 );
static void d21_sc25_dist_name_new_addr ( D2_name_string * , signed32 );
static void d21_sc26_utc_time_new_addr ( UTC_time_string * , signed32 );
static void d21_sc27_bit_str_new_addr ( D2_bit_str * , signed32 );
static void d21_sc28_rev_certs_new_addr (Rev_certs **, signed32 );

/************************************************************************/

d2_ret_val d21_sc01_check_params(
  D21_p5 * parblk)
{
signed16           errval = D2_NO_ERR;
signed32            sc;

do
{
/* check version */
if (parblk->d21_5pbhead->d2_version != D21V022)
	{			/* illegal version supplied		*/
	errval = D2_ILL_VERS;
	break;
	}
/*  check service controls  */
if (parblk->d21_5c_a == NULL)
	{
	       /* no service controls supplied, set errorcode  */
	errval = D21_NO_SC;
	break;
	}
else    {
	sc = parblk->d21_5c_a->d2_serv_cntrls;
	}

if (   ((sc & D2_NOCACHE) && !(sc & D2_USEDSA))
    || (!(sc & D2_USEDSA) && !(sc & D2_DUACACHE))  )
	{       /* sc1 */
	errval = D21_NO_SELECTION;
	break;
	}

if ((sc & D2_FILE_MODE) && (parblk->d21_5file == NULL))
       {
	errval = D21_BAD_ARGUMENT;
	break;
       }
if ( (parblk->d21_5base_obj != NULL) &&
    (strlen((char *)parblk->d21_5base_obj) > D2_DNL_MAX))
	{
	errval = D21_BAD_NAME;
	break;
	}
if ((parblk->d21_5r_info == NULL) || (parblk->d21_5cres == NULL))
	{
	errval = D21_BAD_ARGUMENT;
	break;
	}
if ((parblk->d21_5srch_al != TRUE) && (parblk->d21_5srch_al != FALSE))
	{
	errval = D21_BAD_ARGUMENT;
	break;
	}
if (parblk->d21_5r_info->d2_r_no_at < D2_ALL_ATT_REQ)
	{
	errval = D21_INV_R_NO_AT;
	break;
	}
if ((parblk->d21_5r_info->d2_r_no_at > 0) &&
    (parblk->d21_5r_info->d2_r_type_at == NULL))
       {
	errval = D21_BAD_ARGUMENT;
	break;
	}
if (parblk->d21_5r_info->d2_r_no_at > D2_AT_MAX)
	{
	errval = D21_R_NO_AT_TOO_BIG;
	break;
	}

if (! ((parblk->d21_5r_info->d2_r_val_at == D2_R_TYP)
     || (parblk->d21_5r_info->d2_r_val_at == D2_R_VAL_TYP)))
	{
	errval = D21_INV_R_VAL_AT;
	break;
	}

if ((parblk->d21_5subset != D2_WHOLE_SUBTREE) &&
    (parblk->d21_5subset != D2_ONE_LEVEL) &&
    (parblk->d21_5subset != D2_BASE_OBJECT))
      {
	errval = D21_BAD_ARGUMENT;
	break;
	}

if (sc & D2_DUACACHE)
	{
	 if (((parblk->d21_5base_obj == NULL) &&
	      (parblk->d21_5subset != D2_WHOLE_SUBTREE))
	     || ((parblk->d21_5base_obj != NULL) &&
		 (parblk->d21_5subset != D2_BASE_OBJECT)))
	   {
	    errval = D21_INV_SEARCH_SEL;
	    break;
	   }
	}
/*
valid cache-storage-class ?
*/
if (   !(sc & D2_DONT_STORE) && !(sc & D2_RESIDENT_CLASS) &&
       !(sc & D2_PRIVILEGED_CLASS) && !(sc & D2_NORMAL_CLASS)  )
	{
	if (sc & D2_USEDSA)
		{
		errval = D21_NO_SELECTION;
		}
	break;
	}
}
while (FALSE); /* end of do-while loop */

if (errval != D2_NO_ERR)
	{
	if (errval != D2_ILL_VERS)
		{
		parblk->d21_5pbhead->d2_retcode = D2_CALL_ERR;
		parblk->d21_5pbhead->d2_errclass = D2_PAR_ERR;
		parblk->d21_5pbhead->d2_errvalue = errval;
		}
	else
		{
		parblk->d21_5pbhead->d2_retcode = D2_CALL_ERR;
		parblk->d21_5pbhead->d2_errclass = D2_ILL_VERS;
		parblk->d21_5pbhead->d2_errvalue = D2_NOT_SPEC;
		}
	return (D2_ERROR);
	}
else
	{
	return(D2_NOERROR);
	}
} /* end of check params */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_sc02_len_insearch()                            */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_sc02_len_insearch(parblk)                  */
/*                    D21_p5 *parblk;                                 */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 SEARCH REQUEST in DUA.                             */
/*                                                                    */
/* INPUT-PARAMETER :  parblk : ptr to parameter block                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                 -1,  if failure                                    */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/
/*                                                                    */

signed32 d21_sc02_len_insearch(
  D21_p5 * parblk )
{
signed32 d23_len;

d23_len = D23_S_INSRCH + D23_S_EOM + d21_ca_calc_carglen(parblk->d21_5c_a)
	  + sizeof(D23_srcharg)
	  + d21_ca_calc_req_len(parblk->d21_5r_info,
				parblk->d21_5c_a->d2_serv_cntrls);

if (parblk->d21_5base_obj != NULL) {
	d23_len += D23_S_NM
	+ D2_ALIGN(strlen ((char *) parblk->d21_5base_obj)+1);
}
else
   d23_len += D23_S_NM;

if (d21_ca_calc_fi_len(parblk->d21_5f_pattern,&d23_len)==D2_ERROR) {
	return(D2_ERROR);
}

return(D2_ALIGN(d23_len));
}

d2_ret_val d21_sc03_build_req(
  D21_p5 * parblk,
  char * req,
  signed16 dir_id,
  signed16 bind_id )
{
char           *next = req;
D23_insearch    *mp_head;

/*
build first part of request
*/
mp_head = (D23_insearch *)req;
mp_head->d23_Ihdr.d23_versno = D23_APDUV22;
mp_head->d23_Ihdr.d23_dirid  = dir_id;
mp_head->d23_Ihdr.d23_bindid = bind_id;

next += D23_S_INSRCH;
d21_cm03_build_comarg( parblk->d21_5c_a, &next);

((D23_srcharg *)next)->d23_Itag = D23_ITAG;
((D23_srcharg *)next)->d23_Iali = parblk->d21_5srch_al;
((D23_srcharg *)next)->d23_Iobjsub = parblk->d21_5subset;
next += D23_S_SCHARG;

/*
move Distinguished name
*/
if (parblk->d21_5base_obj != NULL)
	{
	d21_cab_dsname_iput(parblk->d21_5base_obj,next,&next);
	}
else
	{
	((D23_ds_name *) next)->d23_dstag = D23_NAMTAG;
	((D23_ds_name *) next)->d23_dscomplete = TRUE;
	((D23_ds_name *) next)->d23_dslen = 0;
	((D23_ds_name *) next)->d23_dsoff = 0;
	next += D23_S_NM;
	}
/*
move requested attribute types
     (with or without "pseudo-attribute")
*/
if ((parblk->d21_5c_a == NULL) ||
    ((parblk->d21_5c_a->d2_serv_cntrls & D2_USEDSA) &&
    !(parblk->d21_5c_a->d2_serv_cntrls & D2_DONT_STORE)))
	{       /* with pseudo_attribute */
	d21_rat_iput(parblk->d21_5r_info,next,&next,TRUE);
	}
else
	{
	d21_rat_iput(parblk->d21_5r_info,next,&next,FALSE);
	}
/*
move filters
*/
if ((D2_Fi_filter *) parblk->d21_5f_pattern != NULL)
{  if (d21_fi_iput( (D2_Fi_filter *)parblk->d21_5f_pattern,
			(char *)next,(char **)&next) != D2_NOERROR)
	  {   parblk->d21_5pbhead->d2_retcode = D2_CALL_ERR;
	      parblk->d21_5pbhead->d2_errclass = D2_PAR_ERR;
	      parblk->d21_5pbhead->d2_errvalue = D21_BAD_ARGUMENT;
	      return(D2_ERROR);
	      /* no memory allocated by DUA, return immediately ! */
	    }
}
/*
and eom-tag
*/
d21_ca9_eom_iput(next,&next);
return(D2_NOERROR);
} /* end of build request with new filter (201a)*/


d2_ret_val d21_sc04_cache_search(
  D21_p5 * parblk,
  char * req_ptr,
  signed32 req_len,
  char ** res_ptr,
  signed32 * res_len )
{
d21_ipc_info     *ipc_info;
D21_req_info            req_info;
d2_ret_val                   res;

/*
build association to CACHE
*/
if (d21_bh4_i_bind(D23_DCASID,&ipc_info,parblk->d21_5pbhead) != D2_NOERROR)
	{
	return (D2_ERROR);
	}
/*
set ipc-request block
*/
req_info.d21_r_opcode = D23_SEARCH;
req_info.d21_r_bufapdu = req_ptr;
req_info.d21_r_lenapdu = req_len;
req_info.d21_r_ipcinfo = ipc_info;
/*
send request to cache
*/
if (d21_send_invoke(&req_info,ipc_info->d21_i_assid,
				parblk->d21_5pbhead) != D2_NOERROR)
	{
	return (D2_ERROR);
	}
/*
get result from cache
*/
req_info.d21_r_bufapdu = *res_ptr;
req_info.d21_r_lenapdu = *res_len;
res = d21_recv_result(&req_info,ipc_info->d21_i_assid,parblk->d21_5pbhead);
/*
release association to cache immediatly
*/
d21_bh5_i_unbind(ipc_info->d21_i_assid);
*res_ptr = req_info.d21_r_bufapdu;
*res_len = req_info.d21_r_lenapdu;
if (res != D2_NOERROR)
	{
	return (D2_ERROR);
	}
 /* read from DUA-Cache  */
return (D2_NOERROR);
}

/* daton **************************************************************/
/*                                                                    */
/* NAME         :  d21_sc_realloc()                                   */
/*                                                                    */
/* AUTHOR       :   G.Loose                                           */
/* DATE         :   21.11.90                                          */
/*                                                                    */
/* SYNTAX :   d2_ret_val d21_sc_realloc(first_ei,last_ei,more_mem,pbhead)    */
/*                                                                    */
/* DESCRIPTION  :  this function reallocates memory for the ei_info   */
/*                                                                    */
/* INPUT-PARAMETERS :                                                 */
/*             char  *first_ei   pointer to ei_info-structure        */
/*             char  *last_ei    pointer to end of memory            */
/*             signed32    more_mem   additionally needed memory          */
/*             D2_pbhead *pbhead  pointer to pbhead-structure         */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*             char  *first_ei   new pointer to the ei_info          */
/*             char  *last_ei    pointer to new first free address   */
/*                                behind the structure                */
/*                                                                    */
/*                                                                    */
/**********************************************************************/
/* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   */
/*          |          |                             |    |           */
/* datoff *************************************************************/

static d2_ret_val d21_sc_realloc(
  char ** first_ei,
  char ** link,
  char ** last_ei,
  signed32 more_mem,
  D2_pbhead * pbhead )
{
 char * mem;
 signed32 offset;
 D2_ei_info  * ei_info;
 D2_a_info * a_info;
 signed16 i;

 if ((mem = D27_REALLOC(svc_c_sev_warning,*first_ei,
	   (size_t) (*last_ei - *first_ei + more_mem))) == NULL)
  {
   pbhead->d2_retcode   = D2_TEMP_ERR;
   pbhead->d2_errclass  = D2_SYS_ERR;
   pbhead->d2_errvalue  = D21_ALLOC_ERROR ;
  return(D2_ERROR);
  }
 else
 {
  offset = (mem - *first_ei);
  ei_info = (D2_ei_info *) mem;
  while (ei_info != NULL)
  {
   if (ei_info->d2_ei_entry != NULL)
    ei_info->d2_ei_entry = (D2_name_string)
			   ((char *) ei_info->d2_ei_entry + offset);

   if(ei_info->d2_ei_ai != NULL)
     ei_info->d2_ei_ai = (D2_a_info *) ((char *) ei_info->d2_ei_ai + offset);
   a_info = ei_info->d2_ei_ai;
   while (a_info != NULL)
   {
    a_info->d2_a_type.d2_type =
		(Ob_id_string) ((char *) a_info->d2_a_type.d2_type + offset);
    if (a_info->d2_a_no_val > 0) a_info->d2_a_val =
			 (D2_a_value *) ((char *) a_info->d2_a_val + offset);
    for (i=0; i < a_info->d2_a_no_val; i++)
      {
      d21_sc4_new_addr ( (a_info->d2_a_val+i) , offset );
      }
    if(a_info->d2_a_next != NULL)
     a_info->d2_a_next = (D2_a_info *) ((char *) a_info->d2_a_next + offset);
    a_info = a_info->d2_a_next;
   }
   if (ei_info->d2_ei_next != NULL)
     {ei_info->d2_ei_next =
		     (D2_ei_info *) ((char *) ei_info->d2_ei_next + offset);}
   else
     { *link = (char *) ei_info;}
   ei_info = ei_info->d2_ei_next;
  }
 }
 *first_ei = (char *) mem;
 *last_ei  += D2_ALIGN(offset);
 return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/* NAME         :  d21_sc4_new_addr()                                 */
/*                                                                    */
/* AUTHOR       :   Jochen Keutel                                     */
/* DATE         :   08.09.92                                          */
/*                                                                    */
/* DESCRIPTION  : This function sets all pointers of the D2_a_value   */
/*		  structure to the new address after reallocation     */
/*		  in d21_sc_realloc				      */
/*                                                                    */
/* INPUT-PARAMETERS :                                                 */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/**********************************************************************/

static void d21_sc4_new_addr (
  D2_a_value * av,
  signed32 off )
{
switch (av->d2_a_rep)
  {
  case D2_T61:
	av->d2_at_value.d2_t61_a_v = (T61_string)
		((char *) av->d2_at_value.d2_t61_a_v + off);
	break;
  case D2_OCTET:
  case D2_ACCL:
	av->d2_at_value.d2_oct_a_v = (Octet_string)
		((char *) av->d2_at_value.d2_oct_a_v + off);
	break;
  case D2_PRINTABLE:
	av->d2_at_value.d2_prt_a_v = (Print_string)
		((char *) av->d2_at_value.d2_prt_a_v + off);
	break;
  case D2_DISTNAME:
	av->d2_at_value.d2_obj_name_a_v = (D2_name_string)
		((char *) av->d2_at_value.d2_obj_name_a_v + off);
	break;
  case D2_OBJ_IDENT:
	av->d2_at_value.d2_obid_a_v = (Ob_id_string)
		((char *) av->d2_at_value.d2_obid_a_v + off);
	break;
  case D2_NUMERIC:
	av->d2_at_value.d2_num_a_v = (Numeric_string)
		((char *) av->d2_at_value.d2_num_a_v + off);
	break;
  case D2_IA5:
	av->d2_at_value.d2_ia5_a_v = (IA5_string)
		((char *) av->d2_at_value.d2_ia5_a_v + off);
	break;
  case D2_TIME:
	av->d2_at_value.d2_utc_a_v = (UTC_time_string)
		((char *) av->d2_at_value.d2_utc_a_v + off);
	break;
  case D2_PSAP_STX:
	av->d2_at_value.d2_psap_a_v = (Psap_string)
		((char *) av->d2_at_value.d2_psap_a_v + off);
	break;
  case D2_ASN1:
	av->d2_at_value.d2_asn1_a_v = (Asn1_string)
		((char *) av->d2_at_value.d2_asn1_a_v + off);
	break;
  case D2_GUIDE:
	av->d2_at_value.d2_guide_a_v = (D2_guide *)
		((char *) av->d2_at_value.d2_guide_a_v + off);
	av->d2_at_value.d2_guide_a_v->d2_obj.d2_type = (Ob_id_string)
		((char *) av->d2_at_value.d2_guide_a_v->d2_obj.d2_type + off);
	d21_sc8_criteria_new_addr (
		&av->d2_at_value.d2_guide_a_v->d2_crit , off);
	break;
  case D2_POST_AD_STX:
  case D2_T61_PR_LIST:
	{
	D2_str_list * str_list;

	av->d2_at_value.d2_post_a_v = (D2_str_list *)
		((char *) av->d2_at_value.d2_post_a_v + off);
	str_list = av->d2_at_value.d2_post_a_v;

	do
	  {
	  str_list->d2_str.d2_value += off;
	  if (str_list->d2_next != NULL)
	    {
	    str_list->d2_next = (D2_str_list *)
					((char *)str_list->d2_next + off);
	    }
	  
	  str_list = str_list->d2_next;
	  }
	while (str_list != NULL);
	break;
	}
  case D2_PRT_LIST:
  case D2_T61_LIST:
	{
	signed32 i;
	D2_str * d2_str;
	av->d2_at_value.d2_t61lst_a_v = (D2_T61_seq *)
		((char *) av->d2_at_value.d2_t61lst_a_v + off);
	av->d2_at_value.d2_t61lst_a_v->d2_t61_item = (D2_str *)
		((char *) av->d2_at_value.d2_t61lst_a_v->d2_t61_item + off);
	d2_str = av->d2_at_value.d2_t61lst_a_v->d2_t61_item;
	for (i=0;i<av->d2_at_value.d2_t61lst_a_v->d2_nb_item;i++)
	  {
	  d2_str->d2_value += off;
	  d2_str++;
	  }
	break;
	}
  case D2_TTXID_STX:
	av->d2_at_value.d2_ttxid_a_v = (D2_ttxid *)
		((char *) av->d2_at_value.d2_ttxid_a_v + off);
	if (av->d2_at_value.d2_ttxid_a_v->d2_terminal.d2_size > 0)
	  {
	  av->d2_at_value.d2_ttxid_a_v->d2_terminal.d2_value += off;
	  }
	if (av->d2_at_value.d2_ttxid_a_v->d2_controls.d2_size > 0)
	  {
	  av->d2_at_value.d2_ttxid_a_v->d2_controls.d2_value += off;
	  }
	if (av->d2_at_value.d2_ttxid_a_v->d2_graphics.d2_size > 0)
	  {
	  av->d2_at_value.d2_ttxid_a_v->d2_graphics.d2_value += off;
	  }
	if (av->d2_at_value.d2_ttxid_a_v->d2_miscel.d2_size > 0)
	  {
	  av->d2_at_value.d2_ttxid_a_v->d2_miscel.d2_value += off;
	  }
	if (av->d2_at_value.d2_ttxid_a_v->d2_pages.d2_size > 0)
	  {
	  av->d2_at_value.d2_ttxid_a_v->d2_pages.d2_value += off;
	  }
	if (av->d2_at_value.d2_ttxid_a_v->d2_privates.d2_size > 0)
	  {
	  av->d2_at_value.d2_ttxid_a_v->d2_privates.d2_value += off;
	  }
	break;
  case D2_TLXNR_STX:
	av->d2_at_value.d2_tlxnr_a_v = (D2_telex_number *)
		((char *) av->d2_at_value.d2_tlxnr_a_v + off);
	if (av->d2_at_value.d2_tlxnr_a_v->d2_t_number.d2_size > 0)
	  {
	  av->d2_at_value.d2_tlxnr_a_v->d2_t_number.d2_value += off;
	  }
	if (av->d2_at_value.d2_tlxnr_a_v->d2_c_code.d2_size > 0)
	  {
	  av->d2_at_value.d2_tlxnr_a_v->d2_c_code.d2_value += off;
	  }
	if (av->d2_at_value.d2_tlxnr_a_v->d2_answer.d2_size > 0)
	  {
	  av->d2_at_value.d2_tlxnr_a_v->d2_answer.d2_value += off;
	  }
	break;
  case D2_FAXNR_STX:
	av->d2_at_value.d2_faxnr_a_v = (D2_faxnumber *)
		((char *) av->d2_at_value.d2_faxnr_a_v + off);
	if (av->d2_at_value.d2_faxnr_a_v->d2_tel_nr.d2_size > 0)
	  {
	  av->d2_at_value.d2_faxnr_a_v->d2_tel_nr.d2_value += off;
	  }
	break;
  case D2_CERTIFICATE:
	d21_sc20_certificate_new_addr(&(av->d2_at_value.d2_cert_a_v),off);
	break;
  case D2_CERT_PAIR:
	d21_sc21_cert_pair_new_addr(&(av->d2_at_value.d2_ctpair_a_v),off);
	break;
  case D2_CERT_LIST:
	d21_sc22_cert_list_new_addr(&(av->d2_at_value.d2_ctlist_a_v),off);
	break;
  case D2_INT_LIST:
	av->d2_at_value.d2_intlist_a_v = (signed32 *)
		((char *) av->d2_at_value.d2_intlist_a_v + off);
	break;
  case D2_MHS_ORADR_STX:
	d21_sc6_oraddr_new_addr(&(av->d2_at_value.d2_or_addr_av),off);
	break;
  case D2_MHS_ORNAME_STX:
	d21_sc5_orname_new_addr(&(av->d2_at_value.d2_or_name_av),off);
	break;
  case D2_MHS_DLSP_STX:
	av->d2_at_value.d2_dl_s_p_av = (D2_dl_submit_perm *)
		((char *) av->d2_at_value.d2_dl_s_p_av + off);
	switch (av->d2_at_value.d2_dl_s_p_av->d2_perm_type)
	  {
	  case D21_DLSP_INDIV:
		d21_sc5_orname_new_addr(
	       &(av->d2_at_value.d2_dl_s_p_av->d2_choice.d2_individual),off);
		break;
	  case D21_DLSP_MEM_DL:
		d21_sc5_orname_new_addr(
	       &(av->d2_at_value.d2_dl_s_p_av->d2_choice.d2_member_of_dl),off);
		break;
	  case D21_DLSP_PATT:
		d21_sc5_orname_new_addr(
	       &(av->d2_at_value.d2_dl_s_p_av->d2_choice.d2_pattern_match),off);
		break;
	  case D21_DLSP_MEM_G:
		av->d2_at_value.d2_dl_s_p_av->d2_choice.d2_member_of_group =
		(D2_name_string) ((char *)
		av->d2_at_value.d2_dl_s_p_av->d2_choice.d2_member_of_group+off);
		break;
	  }
	break;
  } /* end of switch */
}	/* end of d21_sc4_new_addr */

/***********************************************************************/
/*                                                                     */
/* NAME         :  d21_sc5_orname_new_addr			       */
/*                                                                     */
/* AUTHOR       :   Jochen Keutel                                      */
/* DATE         :   09.09.92                                           */
/*                                                                     */
/* DESCRIPTION  : This function sets all pointers of the D2_OR_name    */
/*		  structure to the new address after reallocation      */
/*		  in d21_sc_realloc				       */
/*                                                                     */
/* INPUT-PARAMETERS :                                                  */
/*                                                                     */
/* OUTPUT-PARAMETERS:                                                  */
/*                                                                     */
/***********************************************************************/

static void d21_sc5_orname_new_addr (
  D2_OR_name ** orn,
  signed32 off )
{
if ( (*orn)->d2_dir_name != NULL )
  {
  (*orn)->d2_dir_name = (D2_name_string) ((char *) (*orn)->d2_dir_name + off);
  }
if ( (*orn)->d2_or_address != NULL )
  {
  d21_sc6_oraddr_new_addr(&((*orn)->d2_or_address), off );
  }
} /* end of d21_sc5_orname_new_addr */

/***********************************************************************/
/*                                                                     */
/* NAME         :  d21_sc6_oraddr_new_addr			       */
/*                                                                     */
/* AUTHOR       :   Jochen Keutel                                      */
/* DATE         :   08.09.92                                           */
/*                                                                     */
/* DESCRIPTION  : This function sets all pointers of the D2_OR_address */
/*		  structure to the new address after reallocation      */
/*		  in d21_sc_realloc				       */
/*                                                                     */
/* INPUT-PARAMETERS :                                                  */
/*                                                                     */
/* OUTPUT-PARAMETERS:                                                  */
/*                                                                     */
/***********************************************************************/

static void d21_sc6_oraddr_new_addr (
  D2_OR_address ** ora,
  signed32 off )
{
*ora = (D2_OR_address *)  ((char *) *ora + off);

if ((*ora)->d2_admd_name != NULL)
  {
  (*ora)->d2_admd_name = (D2_str_list *)((char *)(*ora)->d2_admd_name + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_admd_name,off);
  }
if ((*ora)->d2_common_name != NULL)
  {
  (*ora)->d2_common_name = (D2_str_list *)((char *)(*ora)->d2_common_name + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_common_name,off);
  }
if ((*ora)->d2_country_name != NULL)
  {
  (*ora)->d2_country_name = (D2_str_list *)((char *)(*ora)->d2_country_name + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_country_name,off);
  }
if ((*ora)->d2_domain_type1 != NULL)
  {
  (*ora)->d2_domain_type1 = (D2_str_list *)((char *)(*ora)->d2_domain_type1 + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_domain_type1,off);
  }
if ((*ora)->d2_domain_type2 != NULL)
  {
  (*ora)->d2_domain_type2 = (D2_str_list *)((char *)(*ora)->d2_domain_type2 + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_domain_type2,off);
  }
if ((*ora)->d2_domain_type3 != NULL)
  {
  (*ora)->d2_domain_type3 = (D2_str_list *)((char *)(*ora)->d2_domain_type3 + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_domain_type3,off);
  }
if ((*ora)->d2_domain_type4 != NULL)
  {
  (*ora)->d2_domain_type4 = (D2_str_list *)((char *)(*ora)->d2_domain_type4 + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_domain_type4,off);
  }
if ((*ora)->d2_domain_value1 != NULL)
  {
  (*ora)->d2_domain_value1 = (D2_str_list *)((char *)(*ora)->d2_domain_value1 + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_domain_value1,off);
  }
if ((*ora)->d2_domain_value2 != NULL)
  {
  (*ora)->d2_domain_value2 = (D2_str_list *)((char *)(*ora)->d2_domain_value2 + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_domain_value2,off);
  }
if ((*ora)->d2_domain_value3 != NULL)
  {
  (*ora)->d2_domain_value3 = (D2_str_list *)((char *)(*ora)->d2_domain_value3 + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_domain_value3,off);
  }
if ((*ora)->d2_domain_value4 != NULL)
  {
  (*ora)->d2_domain_value4 = (D2_str_list *)((char *)(*ora)->d2_domain_value4 + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_domain_value4,off);
  }
if ((*ora)->d2_generation != NULL)
  {
  (*ora)->d2_generation = (D2_str_list *)((char *)(*ora)->d2_generation + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_generation,off);
  }
if ((*ora)->d2_given_name != NULL)
  {
  (*ora)->d2_given_name = (D2_str_list *)((char *)(*ora)->d2_given_name + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_given_name,off);
  }
if ((*ora)->d2_initials != NULL)
  {
  (*ora)->d2_initials = (D2_str_list *)((char *)(*ora)->d2_initials + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_initials,off);
  }
if ((*ora)->d2_isdn_number != NULL)
  {
  (*ora)->d2_isdn_number=(Numeric_string)((char *)(*ora)->d2_isdn_number+off);
  }
if ((*ora)->d2_isdn_subaddress != NULL)
  {
  (*ora)->d2_isdn_subaddress=(Numeric_string)((char *)(*ora)->d2_isdn_subaddress+off);
  }
if ((*ora)->d2_numeric_user_id != NULL)
  {
  (*ora)->d2_numeric_user_id=(Numeric_string)((char *)(*ora)->d2_numeric_user_id+off);
  }
if ((*ora)->d2_org_name != NULL)
  {
  (*ora)->d2_org_name = (D2_str_list *)((char *)(*ora)->d2_org_name + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_org_name,off);
  }
if ((*ora)->d2_org_unit_name1 != NULL)
  {
  (*ora)->d2_org_unit_name1 = (D2_str_list *)((char *)(*ora)->d2_org_unit_name1+off);
  d21_sc7_str_list_new_addr ((*ora)->d2_org_unit_name1,off);
  }
if ((*ora)->d2_org_unit_name2 != NULL)
  {
  (*ora)->d2_org_unit_name2 = (D2_str_list *)((char *)(*ora)->d2_org_unit_name2+off);
  d21_sc7_str_list_new_addr ((*ora)->d2_org_unit_name2,off);
  }
if ((*ora)->d2_org_unit_name3 != NULL)
  {
  (*ora)->d2_org_unit_name3 = (D2_str_list *)((char *)(*ora)->d2_org_unit_name3+off);
  d21_sc7_str_list_new_addr ((*ora)->d2_org_unit_name3,off);
  }
if ((*ora)->d2_org_unit_name4 != NULL)
  {
  (*ora)->d2_org_unit_name4 = (D2_str_list *)((char *)(*ora)->d2_org_unit_name4+off);
  d21_sc7_str_list_new_addr ((*ora)->d2_org_unit_name4,off);
  }
if ((*ora)->d2_post_addr_details != NULL)
  {
  (*ora)->d2_post_addr_details = (D2_str_list *)((char *)(*ora)->d2_post_addr_details
									+ off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_addr_details,off);
  }
if ((*ora)->d2_post_addr_in_full != NULL)
  {
  (*ora)->d2_post_addr_in_full=(T61_string)((char *)(*ora)->d2_post_addr_in_full+off);
  }
if ((*ora)->d2_post_code != NULL)
  {
  (*ora)->d2_post_code = (D2_str_list *)((char *)(*ora)->d2_post_code + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_code,off);
  }
if ((*ora)->d2_post_country_name != NULL)
  {
  (*ora)->d2_post_country_name = (D2_str_list *)((char *)(*ora)->d2_post_country_name
									+ off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_country_name,off);
  }
if ((*ora)->d2_post_deliv_pt_name != NULL)
  {
  (*ora)->d2_post_deliv_pt_name=(D2_str_list *)((char *)(*ora)->d2_post_deliv_pt_name
									+ off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_deliv_pt_name,off);
  }
if ((*ora)->d2_post_deliv_sys_name != NULL)
  {
  (*ora)->d2_post_deliv_sys_name=(Print_string)((char *)(*ora)->d2_post_deliv_sys_name
									+ off);
  }
if ((*ora)->d2_post_gen_deliv_addr != NULL)
  {
  (*ora)->d2_post_gen_deliv_addr=(D2_str_list *)((char*)(*ora)->d2_post_gen_deliv_addr
									+ off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_gen_deliv_addr,off);
  }
if ((*ora)->d2_post_locale != NULL)
  {
  (*ora)->d2_post_locale=(D2_str_list *)((char *)(*ora)->d2_post_locale + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_locale,off);
  }
if ((*ora)->d2_post_office_box_nbr != NULL)
  {
  (*ora)->d2_post_office_box_nbr=(D2_str_list *)((char*)(*ora)->d2_post_office_box_nbr
									+ off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_office_box_nbr,off);
  }
if ((*ora)->d2_post_office_name != NULL)
  {
  (*ora)->d2_post_office_name=(D2_str_list *)((char *)(*ora)->d2_post_office_name
									+ off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_office_name,off);
  }
if ((*ora)->d2_post_office_nbr != NULL)
  {
  (*ora)->d2_post_office_nbr=(D2_str_list *)((char *)(*ora)->d2_post_office_nbr
									+ off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_office_nbr,off);
  }
if ((*ora)->d2_post_org_name != NULL)
  {
  (*ora)->d2_post_org_name=(D2_str_list *)((char *)(*ora)->d2_post_org_name + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_org_name,off);
  }
if ((*ora)->d2_post_patron_details != NULL)
  {
  (*ora)->d2_post_patron_details=(D2_str_list *)((char*)(*ora)->d2_post_patron_details
									+ off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_patron_details,off);
  }
if ((*ora)->d2_post_patron_name != NULL)
  {
  (*ora)->d2_post_patron_name=(D2_str_list *)((char *)(*ora)->d2_post_patron_name
									+ off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_patron_name,off);
  }
if ((*ora)->d2_post_street_addr != NULL)
  {
  (*ora)->d2_post_street_addr=(D2_str_list *)((char *)(*ora)->d2_post_street_addr
									+ off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_street_addr,off);
  }
if ((*ora)->d2_presentation_addr != NULL)
  {
  (*ora)->d2_presentation_addr=(Psap_string)((char *)(*ora)->d2_presentation_addr
									+ off);
  }
if ((*ora)->d2_prmd_name != NULL)
  {
  (*ora)->d2_prmd_name = (D2_str_list *)((char *)(*ora)->d2_prmd_name + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_prmd_name,off);
  }
if ((*ora)->d2_surname != NULL)
  {
  (*ora)->d2_surname = (D2_str_list *)((char *)(*ora)->d2_surname + off);
  d21_sc7_str_list_new_addr ((*ora)->d2_surname,off);
  }
if ((*ora)->d2_terminal_id != NULL)
  {
  (*ora)->d2_terminal_id=(Print_string)((char *)(*ora)->d2_terminal_id + off);
  }
if ((*ora)->d2_x121_addr != NULL)
  {
  (*ora)->d2_x121_addr=(Numeric_string)((char *)(*ora)->d2_x121_addr + off);
  }
if ((*ora)->d2_post_addr_in_lines != NULL)
  {
  (*ora)->d2_post_addr_in_lines=(D2_str_list *)((char *)(*ora)->d2_post_addr_in_lines
									+ off);
  d21_sc7_str_list_new_addr ((*ora)->d2_post_addr_in_lines,off);
  }

}

/***********************************************************************/
/*                                                                     */
/* NAME         :  d21_sc7_str_list_new_addr			       */
/*                                                                     */
/* AUTHOR       :   Jochen Keutel                                      */
/* DATE         :   08.09.92                                           */
/*                                                                     */
/* DESCRIPTION  : This function sets all pointers of the D2_str_list   */
/*		  structure to the new address after reallocation      */
/*		  in d21_sc_realloc				       */
/*                                                                     */
/* INPUT-PARAMETERS :                                                  */
/*                                                                     */
/* OUTPUT-PARAMETERS:                                                  */
/*                                                                     */
/***********************************************************************/

static void d21_sc7_str_list_new_addr (
  D2_str_list * sl,
  signed32 off )
{
if ( sl->d2_str.d2_value != NULL )
  {
  sl->d2_str.d2_value += off;
  }
if ( sl->d2_next != NULL )
  {
  sl->d2_next = (D2_str_list *)((char *)(sl->d2_next) + off);
  d21_sc7_str_list_new_addr ( sl->d2_next, off );
  }
}

/***********************************************************************/
/*                                                                     */
/* NAME         :  d21_sc8_criteria_new_addr			       */
/*                                                                     */
/* AUTHOR       :   Jochen Keutel                                      */
/* DATE         :   09.09.92                                           */
/*                                                                     */
/* DESCRIPTION  : This function sets all pointers of the D2_criteria   */
/*		  structure to the new address after reallocation      */
/*		  in d21_sc_realloc				       */
/*                                                                     */
/* INPUT-PARAMETERS :                                                  */
/*                                                                     */
/* OUTPUT-PARAMETERS:                                                  */
/*                                                                     */
/***********************************************************************/

static void d21_sc8_criteria_new_addr (
  D2_criteria * crit,
  signed32 off )
{
signed32 i;

switch (crit->d2_c_mode)
  {
  case D2_AND:
  case D2_OR:
	crit->d2_crit_val.d2_ci_and_or = (D2_crit_set *)
			((char *) crit->d2_crit_val.d2_ci_and_or + off);
	crit->d2_crit_val.d2_ci_and_or->and_or = (D2_criteria *)
		((char *) crit->d2_crit_val.d2_ci_and_or->and_or + off);
	for (i=0;i<crit->d2_crit_val.d2_ci_and_or->and_or_nr;i++)
	  {
	  d21_sc8_criteria_new_addr(crit->d2_crit_val.d2_ci_and_or->and_or + i,
									off);
	  }
	break;
  case D2_ITEM:
	crit->d2_crit_val.type = (D2_crit_item *)
			((char *) crit->d2_crit_val.type + off);
	crit->d2_crit_val.type->d2_ci_type.d2_type = (Ob_id_string)
		((char *) crit->d2_crit_val.type->d2_ci_type.d2_type + off);
	break;
  case D2_NOT:
	crit->d2_crit_val.not = (D2_criteria *)
			((char *) crit->d2_crit_val.not + off);
	d21_sc8_criteria_new_addr (crit->d2_crit_val.not , off);
	break;
  } /* end of switch */
} /* end of d21_sc8_criteria_new_addr */

/**********************************************************************/
/*                                                                    */
/* NAME         :  d21_sc05_move_ei_info_to_mem()                     */
/*                                                                    */
/* AUTHOR       :                                                     */
/* DATE         :                                                     */
/*                                                                    */
/* DESCRIPTION  :  						      */
/*                                                                    */
/* INPUT-PARAMETERS :                                                 */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/**********************************************************************/

d2_ret_val d21_sc05_move_ei_info_to_mem(
  D21_p5 * parblk,
  char * result,
  signed32 res_len,
  char ** next_mem,
  D23_rf_info ** ref_ptr,
  char ** link,
  signed16 * nr_entries )
{
D2_pbhead       *pbhead;
signed32             mem_len;
char           *from = result;
char           * mem;
Bool            first_res = FALSE;
signed16           namlen1 = 0;
register D23_ds_name     *r_name;
D23_en_info     *en_info;
D2_ei_info      * mem1 = parblk->d21_5res;
D2_a_info       *mem2;
signed16           i=0;
unsigned16          tag;
D23_com_res * cm_res ;

pbhead    = parblk->d21_5pbhead;
if (*link ==  NULL)
       {
	first_res = TRUE;
	}
 /*
  base object name ?
 */
 r_name = (D23_ds_name *)from;
 tag    = r_name->d23_dstag;
 if (tag == D23_BONTAG)
	{
	namlen1 = r_name->d23_dsoff;
	from = (char *)++r_name;
	if (first_res == TRUE)
	       {
		/*
		 name of base_object will be set only in first pass !!
		*/
		if ((parblk->d21_5dist_name = D27_MALLOC(svc_c_sev_warning,
							namlen1)) == NULL)
		{   pbhead->d2_retcode = D2_TEMP_ERR;
		    pbhead->d2_errclass = D2_SYS_ERR;
		    pbhead->d2_errvalue = D21_ALLOC_ERROR;
		    return(D2_ERROR);
		    /* no memory allocated by DUA, return immediately ! */
		  }
		else    { /* move DN */
			strcpy((char *)parblk->d21_5dist_name,from);
			}
	       }
	/*
	next tag
	*/
	from = from + D2_ALIGN(namlen1);
	}

 en_info = (D23_en_info *) from;
 tag = en_info->d23_entag;
 if (tag == D23_ENTTAG)
 {
  mem_len   = d21_ca_len_ei_info( from);
  if (*link !=  NULL)
    {
       if (d21_sc_realloc((char **)&(parblk->d21_5res), link,
		next_mem, mem_len, parblk->d21_5pbhead) == D2_ERROR)
	{
	  return(D2_ERROR); }
       mem1 = ((D2_ei_info *)(*link))->d2_ei_next = (D2_ei_info *)(*next_mem);
       mem = *next_mem;
     }
  else
    {
      /*
       set d21_5res in first pass
      */
      /* Allocate memory for results of IAPL* */
      if ((mem = D27_MALLOC(svc_c_sev_warning,mem_len)) == NULL)
      {   pbhead->d2_retcode = D2_TEMP_ERR;
	  pbhead->d2_errclass = D2_SYS_ERR;
	  pbhead->d2_errvalue = D21_ALLOC_ERROR;
	  return(D2_ERROR);
		    /* no memory allocated by DUA, return immediately ! */
       }

      *next_mem = mem + D2_ALIGN(mem_len);
      mem1 = parblk->d21_5res = (D2_ei_info *)mem;
    }
  mem    += sizeof (D2_ei_info);

  /*
   analyze result
  */

 for (i=1;;i++)
 {
  mem1 ->d2_ei_next = NULL;
  mem1->d2_ei_ai = NULL;
  mem1->d2_ei_fromentry = (Bool) (en_info->d23_enmas);
  from   += D23_S_EN;        /* point to next tag */
  r_name = (D23_ds_name *)from;
  tag    = r_name->d23_dstag;

  if (tag != D23_NAMTAG)   /* in entry info, name tag must come */
       {
	pbhead->d2_retcode  = D2_SYS_ERR;
	pbhead->d2_errclass = D2_INT_ERR;
	pbhead->d2_errvalue = D21_RESULTAPDU_INVALID;
	return(D2_ERROR);       /* there must be an ent-tag !!! */
	}

   mem1->d2_ei_entry = (D2_name_string)mem;
   strcpy((char *)mem, (char *)(r_name+1));
   mem += r_name->d23_dsoff;
   from += r_name->d23_dsoff + D23_S_NM;

  /* next free memory for attribute information */
  mem2 = (D2_a_info *)mem;
  tag = ((D23_at_type *)from)->d23_atttag;

  if (tag == D23_ATTTAG)
     {
	    /* move attribute-informations */
	 d21_ca2_attr_iget(&from, mem2,&mem);
	 mem1->d2_ei_ai = mem2 == (D2_a_info *)mem ? NULL : mem2;
       }
  else
      {
	/*
	no attribute-info returned
	*/
	mem1->d2_ei_ai = NULL;

	if ((tag == D23_CRSTAG) || (tag == D23_OQTAG))
	{
		   break;   /* from for ever */
	}
	else
	{   if (tag != D23_ENTTAG)
		{
		/*
		unexpected tag
		*/
		pbhead->d2_retcode  = D2_SYS_ERR;
		pbhead->d2_errclass = D2_INT_ERR;
		pbhead->d2_errvalue = D21_RESULTAPDU_INVALID;
		return(D2_ERROR);
		}
	}
      }
  en_info = (D23_en_info *) from;
  tag = en_info->d23_entag;
  if (tag == D23_ENTTAG)
	{
         mem1 = mem1->d2_ei_next = (D2_ei_info *)mem;
	 mem += sizeof(D2_ei_info);
	}
  else
     {  
       mem1->d2_ei_next = NULL;
       break; }
  }
  *link = (char *) mem1;
  *next_mem = mem;
 }

*nr_entries += i;

/* Handle part-oq, if any */

*ref_ptr = NULL;

if (tag == D23_OQTAG)
  {
      parblk->d21_5uav_crit_ext = ((D23_part_oq *)from)->d23_oq_uncrit;
      parblk->d21_5lim_prob = ((D23_part_oq *)from)->d23_oq_limit;
      from += D23_S_POQ;
      if (((D23_rf_info *)from)->d23_rftag == D23_REFTAG)
	 { *ref_ptr = (D23_rf_info *)from;
	   d21_skip_referral(&from);
	  }
   }

cm_res = (D23_com_res *) from;

if (first_res == TRUE && cm_res->d23_crtag == D23_CRSTAG)
{
/* First Search-result, copy common results */

  cm_res = (D23_com_res *) from;
  parblk->d21_5cres->d2_al_deref = cm_res->d23_ali_deref;

    /* performer is always NULL, as results are not signed */
  parblk->d21_5cres->d2_performer = NULL;
}

return(D2_NOERROR);
}  /* end of sc05_move_ei_info_to_mem  */


signed32 d21_sc06_dsa_only_search(
  Bool adm_syntax,
  D21_p5 * parblk,
  char * req_ptr,
  signed32 req_len,
  char ** res_ptr,
  signed32 * res_len,
  d21_bind_info * bt_ptr)
{

D21_req_info            req_info;
signed32                   res;

/* set ipc-request block */

req_info.d21_r_opcode = adm_syntax? D23_SEARCH+D2_ADM_OPID : D23_SEARCH;
req_info.d21_r_bufapdu = req_ptr;
req_info.d21_r_lenapdu = req_len;
req_info.d21_r_ipcinfo = bt_ptr->d21_b_ipcinf;

/* send request to dsa */

res = d21_send_invoke(&req_info,bt_ptr->d21_b_usr_id,parblk->d21_5pbhead);
if (res != D2_NOERROR)
	{
	return (res);
	}

/* get result from dsa */

req_info.d21_r_bufapdu = *res_ptr;
req_info.d21_r_lenapdu = *res_len;

res = d21_recv_result(&req_info,bt_ptr->d21_b_usr_id,parblk->d21_5pbhead);

*res_ptr = req_info.d21_r_bufapdu;
*res_len = req_info.d21_r_lenapdu;

if (res != D2_NOERROR)
	{
	return (res);
	}

return (D2_NOERROR);
}

d2_ret_val d21_sc08_handle_ref(
  D21_lb_info * lb_info,
  signed16 * lb_nr,
  Bool adm_syntax,
  D21_p5 * parblk,
  char * req_ptr,
  signed32 req_len,
  char ** res_ptr,
  signed32 * res_len,
  time_t maxtime,
  signed16 maxentries,
  char ** next_mem,
  D23_rf_info ** r_ptr,
  char ** link,
  signed16 * entries_found,
  d21_bind_info * bt_ptr)
{
time_t            curr_time;
d2_ret_val           ret_value = D2_NOERROR;
D23_rf_info     *ref_ptr = *r_ptr;
D2_pbhead       ref_pbhead;
				/* indicates whether search on a referral
				   DSA was at least one time successful or
				   not
				*/

ref_pbhead.d2_retcode = ref_pbhead.d2_errclass = ref_pbhead.d2_errvalue =
			D2_NO_ERR;
parblk->d21_5pbhead->d2_retcode  = parblk->d21_5pbhead->d2_errclass =
			    parblk->d21_5pbhead->d2_errvalue = D2_NO_ERR;

/*
   return referrals to application ?
*/
if ((parblk->d21_5lim_prob != D2_NO_LIMIT_EX) ||
    ((parblk->d21_5c_a->d2_serv_cntrls & D2_NOREFERRAL) != D2_NOREFERRAL ))
{   /* return referrals to application */

    if (d21_copy_referral (&ref_ptr, &parblk->d21_5rf,
				parblk->d21_5pbhead) != D2_NOERROR)
    {
	/* set return values */
	ret_value = D2_ERROR;
    }
    ret_value = D2_NOERROR;
}
else
{
    /*
       start handling of referrals
    */
    while (((D23_rf_info *)*r_ptr)->d23_rftag == D23_REFTAG)
      {
	/*
	   enough entries ? Is it really needed ? (duplicating DSA effort)
	*/
	if ( (maxentries != D2_S_UNLIMITED) &&
	     (*entries_found >= maxentries) )
	{   ret_value = D2_NOERROR;
	    parblk->d21_5lim_prob = D2_SIZE_LIMIT_EX;
    	    if (d21_copy_referral (&ref_ptr, &parblk->d21_5rf,
					 parblk->d21_5pbhead) != D2_NOERROR)
    	     {
		     /* set return values */
		     ret_value = D2_ERROR;
    	     }
	     break;
	}
	/*
	   enough time ? Is it really needed ? (duplicating DSA effort)
	*/
	curr_time = time(0L);
	if ((maxtime != D2_T_UNLIMITED) && (curr_time >= maxtime))
	{   /* set time_limit_exceeded and return */
	    parblk->d21_5lim_prob = D2_TIME_LIMIT_EX;
	    ret_value = D2_NOERROR;
    	    if (d21_copy_referral (&ref_ptr, &parblk->d21_5rf,
					 parblk->d21_5pbhead) != D2_NOERROR)
    	     {
		     /* set return values */
		     ret_value = D2_ERROR;
    	     }
	    break;
	}

	/*
	   set size-limit in request and send request to dsa
	*/
	if (maxentries != D2_S_UNLIMITED)
	 { ((D23_com_arg *)(req_ptr+D23_S_HDR))->d23_cmctr.d23_scslm =
				maxentries - *entries_found;
	  }

	if ( ( ((D23_rf_info *)*r_ptr)->d23_rf_entry_only == TRUE) &&
	      			(parblk->d21_5subset == D2_ONE_LEVEL) )
	  {
	  ((D23_srcharg *)(req_ptr+D23_S_HDR+D23_S_CARG))->d23_Iobjsub
							= D2_BASE_OBJECT;
	  }

	/*
	   follow the referral
	*/
	ret_value = d21_handle_ref(lb_info,lb_nr,
			(D23_rf_info **)r_ptr,&ref_pbhead,maxtime,
		       parblk->d21_5bind_id,
			adm_syntax? D23_SEARCH+D2_ADM_OPID : D23_SEARCH,
			res_ptr,res_len,
		       req_ptr,req_len,bt_ptr);
	/*
	   analyze result of request
	*/
	if (ret_value != D2_NOERROR)
	{   /* error from dsa */
            ret_value = D2_NOERROR;
	 }
	else
	{   /* DSA returned  result */
	    if ((ret_value = d21_sc05_move_ei_info_to_mem(parblk,*res_ptr,
				      *res_len,next_mem,&ref_ptr,
				link, entries_found)) != D2_NOERROR)
	    {   
		ret_value = D2_ERROR;
		break;
	    }
	    else    /* handle further referrals */
	       if (ref_ptr != NULL) {
 		ret_value = d21_sc08_handle_ref(lb_info,lb_nr,
					adm_syntax,parblk,req_ptr,
					req_len,res_ptr, res_len, maxtime,
					maxentries,next_mem,&ref_ptr,
			     		link,entries_found,bt_ptr);
		if (ret_value == D2_ERROR)
			break;
		}
	 }

      } /* of while */

}
if (*entries_found == 0  &&
    parblk->d21_5pbhead->d2_retcode == D2_NO_ERR &&
    parblk->d21_5pbhead->d2_errclass == D2_NO_ERR &&
    parblk->d21_5pbhead->d2_errvalue == D2_NO_ERR &&
    (ref_pbhead.d2_retcode != D2_NO_ERR ||
     ref_pbhead.d2_errclass != D2_NO_ERR ||
     ref_pbhead.d2_errvalue != D2_NO_ERR))
 {   /*
	exchange error values from referral-DSA
     */
     parblk->d21_5pbhead->d2_retcode =  ref_pbhead.d2_retcode;
     parblk->d21_5pbhead->d2_errclass = ref_pbhead.d2_errclass;
     parblk->d21_5pbhead->d2_errvalue = ref_pbhead.d2_errvalue;
     ret_value = D2_ERROR;
   }

return(ret_value);

} /* end of d21_sc08_handle_ref() */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_sc20_certificate_new_addr			*/
/*									*/
/* Description	: This function sets all pointers of the Certificate	*/
/*		  structure to the new address after reallocation	*/
/*		  in d21_sc_realloc					*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_sc20_certificate_new_addr(
  Certificate ** cert,	/* IN/OUT - pointer to the attribute value at IAPL */
  signed32 off)		/* IN - offset that has to be added to all pointers */
{
Cert_unsigned * cert_unsigned;

*cert = (Certificate *) ((char *)*cert + off);

cert_unsigned = &((*cert)->to_be_signed);

/* signature (algorithm identifier) */
d21_sc24_alg_id_new_addr(&(cert_unsigned->signature),off);

/* issuer (distinguished name) */
d21_sc25_dist_name_new_addr(&(cert_unsigned->issuer),off);

/* validity (two UTC time strings) */
if (cert_unsigned->validity.not_before != NULL)
  {
  d21_sc26_utc_time_new_addr(&(cert_unsigned->validity.not_before),off);
  }
if (cert_unsigned->validity.not_after != NULL)
  {
  d21_sc26_utc_time_new_addr(&(cert_unsigned->validity.not_after),off);
  }

/* subject (distinguished name) */
d21_sc25_dist_name_new_addr(&(cert_unsigned->subject),off);

/* subject public key info (algorithm identifier and bit string) */
	/* algorithm identifier */
d21_sc24_alg_id_new_addr(
		&(cert_unsigned->subj_public_key_info.algorithm),off);
	/* bit string */
d21_sc27_bit_str_new_addr(
		&(cert_unsigned->subj_public_key_info.subject_key),off);

/* SIGNED */
	/* algorithm identifier */
d21_sc24_alg_id_new_addr(&((*cert)->alg),off);
	/* bit string */
d21_sc27_bit_str_new_addr(&((*cert)->encr_str),off);

} /* end of d21_sc20_certificate_new_addr */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_sc21_cert_pair_new_addr			*/
/*									*/
/* Description	: This function sets all pointers of the Cert_pair	*/
/*		  structure to the new address after reallocation	*/
/*		  in d21_sc_realloc					*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_sc21_cert_pair_new_addr(
  Cert_pair ** cert_pair,/* IN/OUT - pointer to the attribute value at IAPL */
  signed32 off)		/* IN - offset that has to be added to all pointers */
{
*cert_pair = (Cert_pair *) ((char *)*cert_pair + off);

if ((*cert_pair)->forward != NULL)
  {
  d21_sc20_certificate_new_addr(&((*cert_pair)->forward),off);
  }
if ((*cert_pair)->reverse != NULL)
  {
  d21_sc20_certificate_new_addr(&((*cert_pair)->reverse),off);
  }

} /* end of d21_sc21_cert_pair_new_addr */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_sc22_cert_list_new_addr			*/
/*									*/
/* Description	: This function sets all pointers of the Cert_list	*/
/*		  structure to the new address after reallocation	*/
/*		  in d21_sc_realloc					*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_sc22_cert_list_new_addr(
  Cert_list ** cert_list, /* IN/OUT - pointer to the attribute value at IAPL */
  signed32 off)		 /* IN - offset that has to be added to all pointers */
{
Cert_list_unsigned * cert_list_unsigned;

*cert_list = (Cert_list *) ((char *)*cert_list + off);
cert_list_unsigned = &((*cert_list)->to_be_signed);

/* signature (algorithm identifier) */
d21_sc24_alg_id_new_addr(&(cert_list_unsigned->signature),off);

/* issuer (distinguished name) */
d21_sc25_dist_name_new_addr(&(cert_list_unsigned->issuer),off);

/* last update (UTC time string) */
if (cert_list_unsigned->rev_certs != NULL)
  {
  d21_sc28_rev_certs_new_addr(&(cert_list_unsigned->rev_certs),off);
  }

/* SIGNED */
	/* algorithm identifier */
d21_sc24_alg_id_new_addr(&((*cert_list)->alg),off);
	/* bit string */
d21_sc27_bit_str_new_addr(&((*cert_list)->encr_str),off);

} /* end of d21_sc22_cert_list_new_addr */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_sc24_alg_id_new_addr			*/
/*									*/
/* Description	: This function sets all pointers of the Alg_id		*/
/*		  structure to the new address after reallocation	*/
/*		  in d21_sc_realloc					*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_sc24_alg_id_new_addr(
  Alg_id * alg_id,	/* IN - pointer to the attribute value at IAPL */
  signed32 off)		/* IN - offset that has to be added to all pointers */
{
if (alg_id->algorithm.d2_type != NULL)
  {
  alg_id->algorithm.d2_type = (Ob_id_string)
				((char *)alg_id->algorithm.d2_type + off);
  }
} /* end of d21_sc24_alg_id_new_addr */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_sc25_dist_name_new_addr			*/
/*									*/
/* Description	: This function sets all pointers of the D2_name_string	*/
/*		  structure to the new address after reallocation	*/
/*		  in d21_sc_realloc					*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_sc25_dist_name_new_addr(
  D2_name_string * name, /* IN - pointer to the attribute value at IAPL */
  signed32 off)		/* IN - offset that has to be added to all pointers */
{
*name = (D2_name_string) ((char *)*name + off);
} /* end of d21_sc25_dist_name_new_addr */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_sc26_utc_time_new_addr			*/
/*									*/
/* Description	: This function sets all pointers of the UTC_time_string*/
/*		  structure to the new address after reallocation	*/
/*		  in d21_sc_realloc					*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_sc26_utc_time_new_addr(
  UTC_time_string * utc_time, /* IN - pointer to the attribute value at IAPL */
  signed32 off)		/* IN - offset that has to be added to all pointers */
{
*utc_time = (UTC_time_string) ((char *)*utc_time + off);
} /* end of d21_sc26_utc_time_new_addr */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_sc27_bit_str_new_addr			*/
/*									*/
/* Description	: This function sets all pointers of the D2_bit_str	*/
/*		  structure to the new address after reallocation	*/
/*		  in d21_sc_realloc					*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_sc27_bit_str_new_addr(
  D2_bit_str * bit_str,	/* IN - pointer to the attribute value at IAPL */
  signed32 off)		/* IN - offset that has to be added to all pointers */
{
if (bit_str->d2_value != NULL)
  {
  bit_str->d2_value = (unsigned char * ) ((char *)bit_str->d2_value + off);
  }
} /* end of d21_sc27_bit_str_new_addr */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_sc28_rev_certs_new_addr			*/
/*									*/
/* Description	: This function sets all pointers of the Rev_certs	*/
/*		  structure to the new address after reallocation	*/
/*		  in d21_sc_realloc					*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : December 06, 1993					*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

static void d21_sc28_rev_certs_new_addr(
  Rev_certs ** rev_certs,  /* IN - pointer to the attribute value at IAPL */
  signed32 off)		/* IN - offset that has to be added to all pointers */
{
signed32 i;
Rev_certs_unsigned * rev_certs_unsigned;
Rev_cert_body * rev_cert_body;

*rev_certs = (Rev_certs *)((char *)*rev_certs + off);
rev_certs_unsigned = &((*rev_certs)->to_be_signed);
rev_certs_unsigned->rev_cert = (Rev_cert_body *)
				((char *)(rev_certs_unsigned->rev_cert) + off);

for (i=0; i<rev_certs_unsigned->no_rev_certs; i++)
  {
  rev_cert_body = rev_certs_unsigned->rev_cert + i;

  /* signature (algorithm identifier) */
  d21_sc24_alg_id_new_addr(&(rev_cert_body->signature),off);

  /* issuer (distinguished name) */
  d21_sc25_dist_name_new_addr(&(rev_cert_body->issuer),off);

  /* revocation date (UTC time string) */
  d21_sc26_utc_time_new_addr(&(rev_cert_body->rev_date),off);
  }

/* SIGNED */
	/* algorithm identifier */
d21_sc24_alg_id_new_addr(&((*rev_certs)->alg),off);
	/* bit string */
d21_sc27_bit_str_new_addr(&((*rev_certs)->encr_str),off);
} /* end of d21_sc28_rev_certs_new_addr */
