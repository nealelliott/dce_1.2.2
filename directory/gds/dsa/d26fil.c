/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26fil.c,v $
 * Revision 1.1.720.2  1996/02/18  19:45:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:21  marty]
 *
 * Revision 1.1.720.1  1995/12/08  15:44:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:45  root]
 * 
 * Revision 1.1.718.3  1994/06/21  14:45:22  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:52:49  marrek]
 * 
 * Revision 1.1.718.2  1994/05/10  15:52:08  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:21:10  marrek]
 * 
 * Revision 1.1.718.1  1994/02/22  16:05:36  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:11:40  marrek]
 * 
 * Revision 1.1.716.2  1993/10/14  17:16:21  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:09:06  keutel]
 * 
 * Revision 1.1.716.1  1993/10/13  17:30:32  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:26:39  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  13:58:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:07:11  marrek]
 * 
 * Revision 1.1.4.9  1993/02/02  14:45:28  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.8  1993/02/02  11:17:41  marrek
 * 	Introduce D2_IA5
 * 	[1993/02/01  14:38:21  marrek]
 * 
 * Revision 1.1.4.7  1992/12/31  19:39:42  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:19:15  bbelch]
 * 
 * Revision 1.1.4.6  1992/12/07  11:10:44  marrek
 * 	November 1992 code drop
 * 	Wrong logic in filter representation check
 * 	[1992/12/07  10:11:34  marrek]
 * 
 * Revision 1.1.4.5  1992/12/02  10:25:32  marrek
 * 	November 1992 code drop
 * 	Changes for case sensitive printable strings
 * 	[1992/12/02  10:18:49  marrek]
 * 
 * Revision 1.1.4.4  1992/11/27  19:41:35  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:08:59  marrek]
 * 
 * Revision 1.1.4.3  1992/10/06  18:37:47  sommerfeld
 * 	Remove #ident directive.
 * 	[1992/09/30  21:29:05  sommerfeld]
 * 
 * Revision 1.1.4.2  1992/09/28  12:26:38  marrek
 * 	Return D2_OBJ_CLS_VIOLATION and use d27_010_set_error_apdu()
 * 	in case fct_tag == D27_FUPDATE .
 * 	[1992/09/28  12:23:05  marrek]
 * 
 * Revision 1.1.2.3  1992/07/17  19:58:48  weisman
 * 	Checked in at OSF for SNI
 * 	[1992/07/17  19:26:59  weisman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26fil.c,v $ $Revision: 1.1.720.2 $ $Date: 1996/02/18 19:45:10 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : d26fil.c        [fil]                               */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 21.02.89                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the utilities used by the     */
/*                  internal search function concerning filters of    */
/*                  version 2.1A-syntax.                              */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 21.02.89| birth                          | ek |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*                                                                    */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <stdlib.h>

/*****  external Includes    *****/

#include <d2shm.h>
#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d26schema.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** IMPORT ********/

static signed16 d26_f02_check_filter_item(D2_name_string b_obj,
	signed16 *no_items, D23_fi1_info **f_item, byte **output,
	signed32 *output_len, signed32 *remlen, signed16 fct_tag);
static signed16 d26_f03_check_filter_value(D2_name_string b_obj,
	D26_at *at, unsigned16 rep, byte *fival, signed16 *filen,
	D2_a_match m_rule, signed16 position, byte **output,
	signed32 *output_len, signed32 *remlen);
static signed16 d26_f04_grep(byte **str, signed16 *len, D27_codeset *cs,
	D23_fs1_info *f_substr);
static signed16 d26_f05_bingrep(byte **str, signed16 *len, signed16 position,
	byte *substr, signed16 sublen);
static signed16 d26_f06_check_filter_substrings(D2_name_string b_obj,
	D26_at *at, D23_fs1_info *f_substr, byte **output,
	signed32 *output_len, signed32 *remlen);
static signed16 d26_f08_cmp_fitem(D23_fi1_info *f_item, signed16 file_nr,
	char *record, D2_name_string dname, D26_at *at, signed16 pos);
static signed16 d26_f10_analyse_filter_item(signed16 ob_nr, signed32 *ob_cl,
	D23_fi1_info *f_item, D26_attr_info *attr_info);
static void d26_f11_get_attr_info(signed16 ob_nr, signed32 *ob_cl,
	D23_ft1_info **f_info, D26_attr_info **attr_info);
static signed16 d26_f12_fa1_check(char *record, signed16 file_nr,
	signed16 ob_nr, D2_name_string dname, D26_attr_info **at_info,
	D26_acl *acl, D23_ft1_info **f_info, D23_fi1_info **f_err_item,
	boolean acl_only, signed16 fct_tag);
static signed16 d26_f13_cmp_dnlist_item(D23_fi1_info *f_item);
static boolean  d26_f15_matching_rule_ok(signed16 syntax, D2_a_match rule);
static signed16 d26_f16_check_equal_item(D26_at *at, byte **output,
	D2_name_string b_obj, D23_av_value *f_value, signed32 *output_len,
	signed32 *remlen, signed16 fct_tag);
static signed16 d26_f17_check_substrings_item(D26_at *at, byte **output,
	D2_name_string b_obj, D23_fs1_info *f_substr, signed32 *output_len,
	signed32 *remlen);
static signed16 d26_f18_check_ordering_item(D26_at *at, byte **output,
	D2_name_string b_obj, D23_av_value *f_value, signed32 *output_len,
	signed32 *remlen);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f01_check_filter                                */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 25.02.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will check   the filter information   */
/*                from message.                          If it        */
/*                contains too much filters, wrong operation mode,    */
/*                wrong attribute syntax or wrong substring sequence, */
/*                the error message is written and a specific return  */
/*                value is set.                                       */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   signed16        matching objects may exist        */
/*      D2_ATT_ERROR                any attribute problem occurred    */
/*      D2_ADM_LIM_EXCEEDED         too much filter items             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_f01_check_filter(
			/* IN    -  DN of base object of search       */
    D2_name_string b_obj,
			/* INOUT -  number of filter items            */
    signed16       *no_items,
			/* INOUT -  pointer to filter information in  */
    D23_ft1_info   **f_info,        /* message                        */
			/* INOUT -  pointer to beginning of message   */
    byte           **output,
			/* IN    -  subset flag                       */
    D2_subset      subset,
			/* INOUT -  maximum length of result message  */
    signed32       *output_len,
			/* INOUT -  remaining length for result data  */
    signed32       *remlen,
			/* IN    -  indicates calling function        */
    signed16       fct_tag)

{                                   /* d26_f01_check_filter           */

  signed16 ret_value = D2_NO_ERR;   /* return value                   */

  register signed16 i;              /* loop variable                  */
  register signed16 count;          /* count of filter structures     */

  switch((*f_info)->d23_fp1mod)
  {
    case D2_AND:
    case D2_OR:
      count = (*f_info)->d23_fp1count;
      *f_info += 1;
      for(i = 0; i < count; i++)
      {
	if((ret_value = d26_f01_check_filter(b_obj,no_items,f_info,output,
	   subset,output_len,remlen,fct_tag)) != D2_NO_ERR)
	{
	  return(ret_value);
	}
      }
      return(D2_NO_ERR);
    case D2_NOT:
      *f_info += 1;
      return(d26_f01_check_filter(b_obj,no_items,f_info,output,subset,
				  output_len,remlen,fct_tag));
    case D2_ITEM:
      *f_info += 1;
      return(d26_f02_check_filter_item(b_obj,no_items,
	(D23_fi1_info **)f_info,output,output_len,remlen,fct_tag));
  }

  return(D2_NO_ERR);                /* makes compiler happy           */

}                                   /* d26_f01_check_filter           */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f02_check_filter_item                           */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 25.02.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will check the filter item information*/
/*                from message in format of version 2.1. If it        */
/*                contains wrong attribute syntax or wrong substring  */
/*                sequence, the error message is written and a        */
/*                specific return value is set.                       */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   signed16        matching objects may exist        */
/*      D2_ATT_ERROR                any attribute problem occurred    */
/*      D2_ADM_LIM_EXCEEDED         too much filter items             */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_f02_check_filter_item(
			/* IN    -  DN of base object of search       */
    D2_name_string b_obj,
			/* INOUT -  number of filter items            */
    signed16       *no_items,
			/* INOUT -  pointer to filter item in message */
    D23_fi1_info   **f_item,
			/* INOUT -  pointer to beginning of message   */
    byte           **output,
			/* INOUT -  maximum length of result message  */
    signed32       *output_len,
			/* INOUT -  remaining length for result data  */
    signed32       *remlen,
			/* IN    -  indicates calling function        */
    signed16       fct_tag)

{                                   /* d26_f02_check_filter_item      */

				    /* return value                   */
  signed16          ret_value = D2_NO_ERR;
  D23_at_type       *apdu_f_type;   /* filter type in apdu format     */
  D2_a_type         f_type;         /* filter attribute type          */
  D23_fs1_info      *f_substr;      /* filter substrings structure    */
  D23_av_value      *f_value;       /* filter value                   */
  D26_at            *at;            /* pointer to AT-entry            */
  signed32          meslen = 0;     /* actual message length          */

  register signed16 idx;            /* index of attribute in at       */

  apdu_f_type       = (D23_at_type *)(*f_item + 1);
  f_type.d2_typ_len = apdu_f_type->d23_atlen;
  f_type.d2_type    = (Ob_id_string)(apdu_f_type + 1);

  if                    /* number of items too big                    */
    (++(*no_items) > D2_AT_MAX)
  {                     /* return error                               */
    DCE_SVC_LOG((GDS_S_TOO_MUCH_FILTER_ITEMS_MSG,D2_AT_MAX));
    d27_010_set_error_apdu((D23_rserror *)*output,D2_CALL_ERR,D2_PAR_ERR,
      D2_ADM_LIM_EXCEEDED,&meslen);
    *remlen -= meslen;
    return(D2_ADM_LIM_EXCEEDED);
  }
  else if             /* attribute not in schema                      */
	 ((idx = d26_u03_attr_search(&f_type)) < 0)
  {                               /*  return error                    */
    DCE_SVC_LOG((GDS_S_UNKNOWN_ATYPE_IN_FILTER_MSG,f_type.d2_typ_len,
      f_type.d2_type));
    if                  /* update function has called                 */
      (fct_tag == D23_MODENTR)
    {                   /* write attribute error                      */
      d26_u15_attr_error(output,b_obj,1,D2_UNDEFINED_ATTR_TYPE,&f_type,0,0,0,
			 (byte *)"",output_len,&meslen);
      *remlen -= meslen;
      return(D2_ATT_ERROR);
    }
    else
    {                   /*  only skip the filter item                 */
      *f_item = d26_f07_next_fitem(*f_item);
      return(D2_NO_ERR);
    }
  }
  else
  {                     /* set pointers to AT-entry, filter value and */
			/* substrings                                 */
    at = d26_at + idx;
    f_substr = (D23_fs1_info *)((char *)(apdu_f_type + 1) +
      apdu_f_type->d23_atoff);
    f_value  = (D23_av_value *)((char *)(apdu_f_type + 1) +
      apdu_f_type->d23_atoff);
			/* check matching rule                         */
    switch((*f_item)->d23_fi1match)
    {
      case D2_APPROX_MATCH:
      case D2_EQUAL:
	if((ret_value = d26_f16_check_equal_item(at,output,b_obj,f_value,
	   output_len,remlen,fct_tag)) != D2_NO_ERR)
	{
	  return(ret_value);
	}
	break;
      case D2_SUBSTRINGS:
	if((ret_value = d26_f17_check_substrings_item(at,output,b_obj,
	   f_substr,output_len,remlen)) != D2_NO_ERR)
	{
	  return(ret_value);
	}
	break;
      case D2_GTEQ:
      case D2_LTEQ:
	if((ret_value = d26_f18_check_ordering_item(at,output,b_obj,f_value,
	   output_len,remlen)) != D2_NO_ERR)
	{
	  return(ret_value);
	}
	break;
      case D2_PRESENT:
	ret_value = D2_NO_ERR;
	break;
    }   
    *f_item = d26_f07_next_fitem(*f_item);
    return(ret_value);
  }                             /*                                    */

}                                   /*  d26_f02_check_filter_item     */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f03_check_filter_value()                        */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 25.02.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will check the filter value coming    */
/*                from message in format of version 2.0. If it is ok, */
/*                it will be normed and blanks will be removed.       */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   Short           no error                          */
/*      D2_ATT_ERROR                wrong syntax on filter value      */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_f03_check_filter_value(
			/* IN    -  DN of base object of search       */
    D2_name_string b_obj,
    D26_at         *at, /* IN    -  pointer to attribute table entry  */
    unsigned16     rep, /* IN    -  ASN1-representation               */
			/* IN    -  filter value to check             */
    byte           *fival,
			/* INOUT -  length of filter value            */
    signed16       *filen,
			/* IN    -  indicates matching rule           */
    D2_a_match     m_rule,
			/* IN    -  indicates position of substring   */
    signed16       position,
			/* INOUT -  pointer to beginning of message   */
    byte           **output,
			/* INOUT -  maximum length of result message  */
    signed32       *output_len,
			/* INOUT -  remaining length for result data  */
    signed32       *remlen)
{                                   /*  d26_f03_check_filter_value    */

  signed32 meslen = 0;              /* message length                 */

  if                            /* error from syntax check            */
    (d26_u10_syntax_check(at,rep,fival,*filen) == D26_ERROR)
  {                             /* return syntax error                */
    DCE_SVC_LOG((GDS_S_WR_SYNTAX_IN_FILTER_VAL_MSG,*filen,fival));
    d26_u15_attr_error(output,b_obj,1,D2_ATSYNTAX_ERROR,&at->d26a_type,
	    at->d26a_rep,rep,*filen,fival,output_len,&meslen);
    *remlen -= meslen;
    return(D2_ATT_ERROR);
  }
  else if                       /* password is in filter              */
	 (at->d26a_syntax == D2_PASSWD_STX && at->d26a_filenr < 0)
  {                             /* scramble password in message       */
    d26_u18_scramble_string(fival,fival,*filen);
  }

  switch(at->d26a_syntax)
  {
    case D2_CIGNORE    :
      *filen = d27_012_remove_blanks((char *)fival,*filen,(char *)fival);
      d27_651_norm_T61String(fival,fival,*filen);
      break;
    case D2_CIGN_LIST  :
    case D2_POST_AD_STX:
      *filen = d27_012_remove_blanks((char *)fival,*filen,(char *)fival);
      d26_usp_norm_ci_list(fival,fival,*filen);
      break;
    case D2_COUNTRY_STX:
    case D2_IA5        :
      *filen = d27_012_remove_blanks((char *)fival,*filen,(char *)fival);
      d27_031_norm_pr_string(fival,fival,*filen);
      break;
    case D2_DISTNAME   :
      *filen = d27_012_remove_blanks((char *)fival,*filen,(char *)fival);
      d27_007_norm_name((char *)fival,*filen,(char *)fival);
      d27_018_sort_name((D2_name_string)fival);
      break;
    case D2_ACCL    :
      *filen = d27_012_remove_blanks((char *)fival,*filen,(char *)fival);
      d26_uso_norm_acl(fival,fival);
      break;
    case D2_CEXACT:
    case D2_PRINTABLE:
    case D2_TLXNR_STX:
      *filen = d27_012_remove_blanks((char *)fival,*filen,(char *)fival);
      break;
    case D2_NUMERIC:
    case D2_TELNR_STX:
      *filen = d26_u28_norm_numbers((char *)fival,*filen,(char *)fival);
      break;
    case D2_MHS_ORADR_STX:
      *filen = d26_un1_norm_or_address((char *)fival,*filen,(char *)fival);
      break;
    case D2_MHS_ORNAME_STX:
      *filen = d26_un2_norm_or_name((char *)fival,*filen,(char *)fival);
      break;
    case D2_MHS_DLSP_STX:
      *filen = d26_un3_norm_submit_perm((char *)fival,*filen,(char *)fival);
      break;
    default:
      break;
  }

  return(D2_NO_ERR);

}                                   /* d26_f03_check_filter_value     */

/*exon ****************************************************************/
/*                                                                    */
/* NAME         : d26_f04_grep                                        */
/*                                                                    */
/* DATE         : 05/31/94                                            */
/*                                                                    */
/* DESCRIPTION  : The Function checks, whether a substrings occurs    */
/*                in a given string.                                  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       signed16        match was found               */
/*      D2_NFOUND                       no match                      */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_f04_grep(
    byte         **str, /* INOUT -  string to be checked for substr.  */
    signed16     *len,  /* INOUT -  length of string to be checked    */
    D27_codeset  *cs,   /* INOUT -  actual codeset of string          */
			/* IN    -  filter substrings structure       */
    D23_fs1_info *f_substr)

{                   /* d26_f04_grep                                   */

  switch(f_substr->d23_fs1rep)
  {
    case D2_OCTET:
    case D2_NUMERIC:
    case D2_IA5:
      return(d26_f05_bingrep(str,len,f_substr->d23_fs1position,
	(byte *)(f_substr + 1),f_substr->d23_fs1len));
    case D2_PRINTABLE:
    case D2_T61:
      return(d27_691_t61grep(str,len,cs,f_substr->d23_fs1position,
	(byte *)(f_substr + 1),f_substr->d23_fs1len));
    case D2_T61_PR_LIST:
    default:
      return(D2_NFOUND);
  }

}               /* d26_f04_grep                                       */

/*exon ****************************************************************/
/*                                                                    */
/* NAME         : d26_f05_bingrep                                     */
/*                                                                    */
/* DATE         : 05/31/94                                            */
/*                                                                    */
/* DESCRIPTION  : The Function checks, whether a binary substring     */
/*                occurs in a given binary string.                    */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       signed16        match was found               */
/*      D2_NFOUND                       no match                      */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_f05_bingrep(
    byte         **str, /* INOUT -  string to be checked for substr.  */
    signed16     *len,  /* INOUT -  length of string to be checked    */
			/* IN    -  position of proposed substring    */
    signed16     position,
			/* IN    -  proposed substring                */
    byte         *substr,
    signed16     sublen)/* IN    -  length of proposed substring      */

{                   /* d26_f05_bingrep                                */

  register signed16 i;              /* loop variable                  */
  register signed16 range;          /* difference of lengths          */

  switch(position)
  {
    case D2_INITIAL:
      if            /* substring longer than string to check          */
	(sublen > *len)
      {
	return(D2_NFOUND);
      }
      else if       /* initial substring does not match               */
	(memcmp(*str,substr,sublen))
      {
	return(D2_NFOUND);
      }
      else
      {             /* reset string pointer and length                */
	*str += sublen;
	*len -= sublen;
      }
      break;
    case D2_ANY:
      if            /* substring longer than string to check          */
	((range = *len - sublen) < 0)
      {             /* return no match                                */
	return(D2_NFOUND);
      }
      else
      {             /* grep for substring                             */
	for
	   (i = 0; i <= range; i++)
	{
	  if        /* match is found                                 */
	    (memcmp(*str + i,substr,sublen) == 0)
	  {         /* reset string pointer and length                */
	    *str += i + sublen;
	    *len -= i + sublen;
	    break;
	  }
	}
	if(i > range)
	{
	  return(D2_NFOUND);
	}
      }
      break;
    case D2_FINAL:
      if                /* substring longer than string to check      */
	(sublen > *len)
      {
	return(D2_NFOUND);
      }
      else if           /* substring does not match                   */
	(memcmp(*str + *len - sublen,substr,sublen))
      {
	return(D2_NFOUND);
      }
      else
      {
	*str += sublen;
	*len -= sublen;
      }
      break;
  }

  return(D2_NO_ERR);                /* return match                   */

}               /* d26_f05_bingrep                                    */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f06_check_filter_substrings                     */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 25.02.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will check the filter substrings      */
/*                from message in format of version 2.1A              */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   Short           no error                          */
/*      D2_ATT_ERROR                wrong syntax on filter value or   */
/*                                  wrong filter position detected    */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_f06_check_filter_substrings(
			/* IN    -  DN of base object of search       */
    D2_name_string b_obj,
    D26_at         *at, /* IN    -  pointer to attribute table entry  */
			/* IN    -  filter substrings to check        */
    D23_fs1_info   *f_substr,
			/* INOUT -  pointer to beginning of message   */
    byte           **output,
			/* INOUT -  maximum length of result message  */
    signed32       *output_len,
			/* INOUT -  remaining length for result data  */
    signed32       *remlen)
{                                   /* d26_f06_check_filter_substrings*/
  signed16 mode   = D2_INITIAL;     /* reached mode                   */
  boolean  error  = FALSE;          /* error indicator                */
  signed32 meslen = 0;              /* message length                 */

  while                             /*  still substring in message    */
       (f_substr->d23_fs1tag == D23_FISTAG)
  {                                 /* check representations          */
    if              /* representations of schema and substring differ */
      (at->d26a_rep == D2_T61_PR && f_substr->d23_fs1rep != D2_T61 &&
       f_substr->d23_fs1rep != D2_PRINTABLE ||
       at->d26a_rep != D2_T61_PR && at->d26a_rep != f_substr->d23_fs1rep)
    {               /* create attribute error                         */
      DCE_SVC_LOG((GDS_S_WR_REP_IN_FILTER_MSG,at->d26a_type.d2_typ_len,
	at->d26a_type.d2_type,f_substr->d23_fs1rep));
      d26_u15_attr_error(output,b_obj,1,D2_ATSYNTAX_ERROR,&at->d26a_type,
	    at->d26a_rep,f_substr->d23_fs1rep,f_substr->d23_fs1len,
	    (byte *)(f_substr + 1),output_len,&meslen);
      *remlen -= meslen;
      return(D2_ATT_ERROR);
    }
    else
    {               /* check position of substring                    */
      switch(f_substr->d23_fs1position)
      {
	case D2_INITIAL:
	  if(mode != D2_INITIAL)
	  {
	    error = TRUE;
	  }
	  mode = D2_ANY;
	  if(error == FALSE)
	  {
	    break;
	  }
	case D2_ANY:
	  if(mode == D2_FINAL)
	  {
	    error = TRUE;
	  }
	  mode = D2_ANY;
	  if(error == FALSE)
	  {
	    break;
	  }
	case D2_FINAL:
	  if(mode == D2_FINAL)
	  {
	    error = TRUE;
	  }
	  mode = D2_FINAL;
	  if(error == FALSE)
	  {
	    break;
	  }
	default:
	  DCE_SVC_LOG((GDS_S_WR_POS_IN_FILTER_MSG,at->d26a_type.d2_typ_len,
	    at->d26a_type.d2_type,f_substr->d23_fs1position));
	  d26_u15_attr_error(output,b_obj,1,D2_INAPPR_MATCH,&at->d26a_type,
	    at->d26a_rep,f_substr->d23_fs1rep,f_substr->d23_fs1len,
	    (byte *)(f_substr + 1),output_len,&meslen);
	  *remlen -= meslen;
	  return(D2_ATT_ERROR);
      }
      if                        /* error from analysing filter value  */
	(d26_f03_check_filter_value(b_obj,at,f_substr->d23_fs1rep,
	 (byte *)(f_substr + 1),&f_substr->d23_fs1len,D2_SUBSTRINGS,
	 f_substr->d23_fs1position,output,output_len,remlen) == D2_ATT_ERROR)
      {                         /* return error                       */
	return(D2_ATT_ERROR);
      }
      f_substr = (D23_fs1_info *)((char *)(f_substr + 1) +
	f_substr->d23_fs1off);
    }
  }

  return(D2_NO_ERR);

}                               /* d26_f06_check_filter_substrings    */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f07_next_fitem()                                */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.03.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will return a pointer to the byte     */
/*                after the given filter item message part.           */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*              D23_fi1_info *              pointer to byte after     */
/*                                          the given filter item     */
/*                                          message part.             */
/*                                                                    */
/*exoff ***************************************************************/

D23_fi1_info *d26_f07_next_fitem(
			/* IN   -   pointer to filter item information*/
    D23_fi1_info *f_item)           /* in message                     */

{                                   /*  d26_f07_next_fitem            */

  D23_at_type  *f_type;         /* pointer to attribute type structure*/
  D23_av_value *f_value;        /* pointer to attribute value struct. */
  D23_fs1_info *f_substr;       /* pointer to substrings structure    */

  f_type = (D23_at_type *)(f_item + 1);

  switch(f_item->d23_fi1match)
  {
    case D2_GTEQ:
    case D2_LTEQ:
    case D2_APPROX_MATCH:
    case D2_EQUAL:
      f_value = (D23_av_value *)((char *)(f_type + 1) + f_type->d23_atoff);
      return((D23_fi1_info *)((char *)(f_value + 1) + f_value->d23_avoff));
    case D2_SUBSTRINGS:
      f_substr = (D23_fs1_info *)((char *)(f_type + 1) + f_type->d23_atoff);
      while(f_substr->d23_fs1tag == D23_FISTAG)
      {
	f_substr = (D23_fs1_info *)((char *)(f_substr + 1)
	  + f_substr->d23_fs1off);
      }
      return((D23_fi1_info *)f_substr);
    case D2_PRESENT:
      return((D23_fi1_info *)((char *)(f_type + 1) + f_type->d23_atoff));
    default:
      return(NULL);
  }

}                                   /*  d26_f07_next_fitem            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f08_cmp_fitem()                                 */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 02.03.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will look whether any filter item     */
/*                from message matches the attribute in record.       */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR               Short   object matches filter item    */
/*      D26_NOFILTERMATCH               filter doesn't match attribute*/
/*                                      on object, for retrieval fct. */
/*      D2_ATTRIBUTE_MISSING            filter doesn't match attribute*/
/*                                      on object for update function */
/*                                      or attribute not present for  */
/*                                      compare                       */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_f08_cmp_fitem(
			/* IN    -  information about filter item     */
    D23_fi1_info   *f_item,
			/* IN    -  number of object file             */
    signed16       file_nr,
			/* IN    -  record containing base object     */
    char           *record,
			/* IN    -  distinguished name of object found*/
    D2_name_string dname,
    D26_at         *at, /* IN    -  description of filter attribute   */
    signed16       pos) /* IN    -  position of attribute in record   */

{                                   /* d26_f08_cmp_fitem              */

  if                                /* attribute is stored at object  */
    (at->d26a_filenr < 0)
  {                                 /* compare single filter          */
    return(d26_i79_cmp_sg_item(f_item,file_nr,record,dname,at,pos));
  }
  else if                           /* DN-List Attribute of own DSA   */
				    /* has to be compared             */
	 (!D2_ATTCMP(&at->d26a_type,D2_DNLIST) && !strcmp((char *)dname,
	  (char *)d26_myname))
  {                                 /* compare DN-list                */
    return(d26_f13_cmp_dnlist_item(f_item));
  }
  else
  {                                 /* compare recurring filter       */
    return(d26_i81_cmp_rec_item(f_item,file_nr,record,dname,at,pos));
  }

}                                   /* d26_f08_cmp_fitem              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f09_analyse_filter_object()                     */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 20.02.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will analyse the filter information   */
/*                from message in format of version 2.1. If it        */
/*                contains any attribute, that may not be assigned to */
/*                any object, an indicator for this object will be set*/
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   signed16                matching objects may exist*/
/*      D2_CONSTRAIN_VIOLATION              too much filters or filter*/
/*                                          attributes too long       */
/*      D2_INAPPR_MATCH                     phonetic comparison       */
/*                                          required although not     */
/*                                          allowed or illegal        */
/*                                          matching rule             */
/*      D2_UNDEFINED_ATTR_TYPE              no such attribute in at  */
/*      D2_ATSYNTAX_ERROR                   wrong syntax on filter val*/
/*      D26_ERROR                           illegal operation         */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_f09_analyse_filter_object(
    signed16     ob_nr, /* IN   -   object number                     */
    signed32     *ob_cl,/* IN   -   object class array                */
			/* IN   -   pointer to filter information in  */
    D23_ft1_info **f_info,            /* message                        */
			/* IN   -   pointer to erroneous filter item  */
    D23_fi1_info **f_error)           /* in message                     */

{                                   /* d26_f09_analyse_filter_object  */

  signed16 ret_value = D2_NO_ERR;   /* return value                   */
  signed16 error = D2_NO_ERR;       /* error from called function     */
  D23_fi1_info *f_errtmp;           /* indicates erroneous filter item*/
  D26_attr_info dummy;              /* dummy to write information     */
				    /* which is not needed            */

  register signed16 i;              /* loop variable                  */
  register signed16 count;          /* count of filter structures     */

  switch((*f_info)->d23_fp1mod)
  {
    case D2_AND:
      count = (*f_info)->d23_fp1count;
      *f_info += 1;
      for(i = 0; i < count; i++)
      {
	if((error = d26_f09_analyse_filter_object(ob_nr,ob_cl,f_info,
	   &f_errtmp)) != D2_NO_ERR && ret_value == D2_NO_ERR)
	{
	  ret_value = error;
	  *f_error = f_errtmp;
	}
      }
      return(ret_value);
    case D2_OR:
      count = (*f_info)->d23_fp1count;
      *f_info += 1;
      ret_value = D26_ERROR;
      for(i = 0; i < count; i++)
      {
	if((error = d26_f09_analyse_filter_object(ob_nr,ob_cl,f_info,
	   &f_errtmp)) == D2_NO_ERR)
	{
	  ret_value = D2_NO_ERR;
	}
      }
      if(ret_value == D26_ERROR)
      {
	*f_error = f_errtmp;
	return(error);
      }
      else
      {
	*f_error = NULL;
	return(D2_NO_ERR);
      }
    case D2_NOT:
      *f_info += 1;
      return(d26_f09_analyse_filter_object(ob_nr,ob_cl,f_info,f_error));
    case D2_ITEM:
      *f_info += 1;
      if((d26_f10_analyse_filter_item(ob_nr,ob_cl,(D23_fi1_info *)*f_info,
	 &dummy)) >= 0)
      {
	*f_error = NULL;
	error    = D2_NO_ERR;
      }
      else
      {
	*f_error = (D23_fi1_info *)*f_info;
	error    = D2_UNDEFINED_ATTR_TYPE;
      }
      *f_info = (D23_ft1_info  *)d26_f07_next_fitem((D23_fi1_info *)*f_info);
      return(error);
    default:
      return(D2_NO_ERR);
  }

}                                   /* d26_f09_analyse_filter_object  */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f10_analyse_filter_item()                       */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 22.02.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will analyse the filter item coming   */
/*                from message. If it contains any attribute, that may*/
/*                not be assigned to objects with the given object    */
/*                classes in ob_cl, an error is returned, the AT-index*/
/*                otherwise.                                          */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      >= 0        signed16        AT-index of filter attribute      */
/*      D26_IDX_NOT_DEF             filter attribute is not contained */
/*                                  in object class nor a naming      */
/*                                  attribute.                        */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_f10_analyse_filter_item(
    signed16     ob_nr, /* IN   -   object number                     */
    signed32     *ob_cl,/* IN   -   object class array                */
			/* IN   -   pointer to filter item information*/
    D23_fi1_info *f_item,             /* in message                     */
			/* IN   -   pointer to attribute information  */
    D26_attr_info *attr_info)

{                                   /* d26_f10_analyse_filter_item    */


  D23_at_type *apdu_f_type;         /* filter type in apdu format     */
  D2_a_type   f_type;               /* filter attribute type          */

  register    D26_srt *srt;         /* pointer to SRT                 */
  register    D26_nam_at *nam_at;   /* pointer to naming attr. list   */
  register    signed16 idx;         /* index of attribute in AT       */

  apdu_f_type       = (D23_at_type *)(f_item + 1);
  f_type.d2_typ_len = apdu_f_type->d23_atlen;
  f_type.d2_type    = (Ob_id_string)(apdu_f_type + 1);

  attr_info->d26_at_idx  = attr_info->d26_srt_idx = attr_info->d26_nam_idx
			 = D26_IDX_NOT_DEF;
  attr_info->d26_in_ocls = FALSE;

  if                                /* attribute type is found in AT  */
    ((idx = d26_u03_attr_search(&f_type)) >= 0)
  {                                 /* check whether matching rule is */
				    /* ok                             */
    if                              /* matching rule is ok for syntax */
      (d26_f15_matching_rule_ok(d26_at[idx].d26a_syntax,f_item->d23_fi1match)
       == TRUE)
    {                               /* check attribute belongs to the */
				    /* given object classes           */
      if                            /* object class contains attribute*/
	(d26_u59_attr_in_obj_class(ob_cl,idx) == TRUE)
      {                             /* set the AT-index               */
	attr_info->d26_at_idx  = idx;
	attr_info->d26_in_ocls = TRUE;
      }

      for                           /* structure rule and all the     */
				    /* superior structure rules       */
	 (srt = d26_srt + ob_nr; ob_nr != D26_ROOT;
	  ob_nr = srt->d26s_par_nr, srt = d26_srt + ob_nr)
      {                             /* scan the naming attributes     */
	for                         /* all naming attribute structures*/
	   (nam_at = srt->d26s_nam_at; nam_at->d26s_index != -1; nam_at++)
	{                           /* compare naming attribute type  */
	  if                        /* attribute is naming            */
	    (nam_at->d26s_index == idx)
	  {                         /* return AT-index                */
	    attr_info->d26_at_idx  = idx;
	    attr_info->d26_srt_idx = ob_nr;
	    attr_info->d26_nam_idx = nam_at - srt->d26s_nam_at;
	    break;
	  }
	}
	if                          /* attribute was found as naming  */
	  (nam_at->d26s_index != -1)
	{                           /* break loop                     */
	  break;
	}
      }
      return(attr_info->d26_at_idx);/* return AT-index                */
    }
  }

  return(D26_IDX_NOT_DEF);          /* return error                   */

}                                   /* d26_f10_analyse_filter_item    */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f11_get_attr_info()                             */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 27.02.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will assign the at-indices of the     */
/*                filter items from f_info                            */
/*                to the index components of the at_info structures.  */
/*                                                                    */
/* RETURN VALUE :   None                                              */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_f11_get_attr_info(
    signed16     ob_nr, /* IN   -   object number                     */
    signed32     *ob_cl,/* IN   -   object class array                */
			/* IN   -   pointer to filter information in  */
    D23_ft1_info **f_info,          /* message                        */
			/* IN   -   pointer to attribute information  */
    D26_attr_info **at_info)
{                                   /* d26_f11_get_attr_info          */

  D23_fi1_info *f_item;             /* pointer to filter item         */

  register signed16 i;              /* loop variable                  */
  register signed16 count;          /* count of filter structures     */

  switch((*f_info)->d23_fp1mod)
  {
    case D2_AND:
    case D2_OR:
      count = (*f_info)->d23_fp1count;
      *f_info += 1;
      for(i = 0; i < count; i++)
      {
	d26_f11_get_attr_info(ob_nr,ob_cl,f_info,at_info);
      }
      return;
    case D2_NOT:
      *f_info += 1;
      d26_f11_get_attr_info(ob_nr,ob_cl,f_info,at_info);
      return;
    case D2_ITEM:
      f_item = (D23_fi1_info *)(*f_info + 1);
      d26_f10_analyse_filter_item(ob_nr,ob_cl,f_item,*at_info);
      (*at_info)++;
      *f_info = (D23_ft1_info *)d26_f07_next_fitem(f_item);
      return;
    default:
      return;
  }

}                                   /* d26_f11_get_attr_info          */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f12_fa1_check                                   */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 01.03.89                                            */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION  : This function will look whether the filter          */
/*                attributes in Version 2.1A syntax match to the      */
/*                object read already.                                */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   Short             object matches filter           */
/*      D2_UNDEFINED_ATTR_TYPE        attribute type does not belong  */
/*                                    to object or illegal matching   */
/*      D2_ACCESS_RIGHTS_INSUFFICIENT access is not given on all      */
/*                                    filter attributes               */
/*      D26_NOFILTERMATCH             filter doesn't match attribute  */
/*                                    on object, but:                 */
/*      D2_ATTRIBUTE_MISSING          filter doesn't match attribute  */
/*                                    on object (only for update      */
/*                                    function)                       */
/*      D2_AT_VALUE_EXISTS            filter doesn't match attribute  */
/*                                    on object (only for update      */
/*                                    function  with negated filter   */
/*                                    item)                           */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_f12_fa1_check(
			/* IN    -  record containing base object     */
    char           *record,
			/* IN    -  file number for object            */
    signed16       file_nr,
			/* IN    -  object number                     */
    signed16       ob_nr,
			/* IN    -  distinguished name of object found*/
    D2_name_string dname,
			/* INOUT -  Array containing the attribute    */
    D26_attr_info  **at_info,       /* types of filters               */
			/* IN    -  acl read from record              */
    D26_acl        *acl,
			/* INOUT -  information about filter          */
    D23_ft1_info   **f_info,        /* attributes                     */
			/* OUT   -  erroneous filter item             */
    D23_fi1_info   **f_err_item,
			/* IN    -  indicates that only acl has to be */
    boolean        acl_only,        /* checked                        */
			/* IN    -  indicates calling function        */
    signed16       fct_tag)

{                                   /* d26_f12_fa1_check              */

  D26_at   *at;                     /* pointer to AT entry            */
  D26_srt  *srt;                    /* SRT entry                      */
  signed16 count;                   /* count of filters combined by   */
				    /* and or or                      */
  signed16 error = D2_NO_ERR;       /* error from subordinate calls   */
  signed16 ret_value = D2_NO_ERR;   /* return value                   */
  D23_fi1_info *f_item;             /* pointer to filter item         */
  signed16 pos;                     /* position of attribute value in */
				    /* record                         */
				    /* temporary pointer to erroneous */
  D23_fi1_info *f_err_tmp;          /* filter item                    */

  register signed16   i;            /* loop variable                  */
  register signed16   idx;          /* index in SRT                   */
  register D26_nam_at *nam_at;      /* pointer to naming attr. list   */

  switch((*f_info)->d23_fp1mod)
  {
    case D2_AND:
      count = (*f_info)->d23_fp1count;
      *f_info += 1;
      for(i= 0; i < count; i++)
      {
	error = d26_f12_fa1_check(record,file_nr,ob_nr,dname,at_info,acl,
	  f_info,&f_err_tmp,acl_only,fct_tag);
	if(error != D2_NO_ERR)
	{
	  *f_err_item = f_err_tmp;
	  ret_value = error;
	}
      }
      break;
    case D2_OR:
      count = (*f_info)->d23_fp1count;
      *f_info += 1;
      if(count == 0)
      {
	ret_value = D2_NO_ERR;
      }
      else
      {
	ret_value = D26_ERROR;
	for(i= 0; i < count; i++)
	{
	  error = d26_f12_fa1_check(record,file_nr,ob_nr,dname,at_info,acl,
	    f_info,&f_err_tmp,acl_only,fct_tag);
	  if(error == D2_NO_ERR)
	  {
	    ret_value = D2_NO_ERR;
	  }
	  else
	  {
	    *f_err_item = f_err_tmp;
	  }
	}
	ret_value = ret_value == D2_NO_ERR ? ret_value : error;
      }
      break;
    case D2_NOT:
      *f_info += 1;
      switch(d26_f12_fa1_check(record,file_nr,ob_nr,dname,at_info,acl,f_info,
	     &f_err_tmp,acl_only,fct_tag))
      {
	case D2_NO_ERR:
	  ret_value   = fct_tag == D23_MODENTR ?
			D2_AT_VALUE_EXISTS : D26_NOFILTERMATCH;
	  *f_err_item = f_err_tmp;
	  break;
	case  D2_UNDEFINED_ATTR_TYPE:
	  ret_value   = D2_UNDEFINED_ATTR_TYPE;
	  *f_err_item = f_err_tmp;
	  break;
	case  D2_ACCESS_RIGHTS_INSUFFICIENT:
	  ret_value   = D2_ACCESS_RIGHTS_INSUFFICIENT;
	  *f_err_item = f_err_tmp;
	  break;
	case  D2_AT_VALUE_EXISTS:
	case  D2_ATTRIBUTE_MISSING:
	case  D26_NOFILTERMATCH:
	  ret_value   = D2_NO_ERR;
	  break;
      }
      break;
    case D2_ITEM:
      ret_value = D2_UNDEFINED_ATTR_TYPE;
      f_item = (D23_fi1_info *)(*f_info + 1);
      if                            /* filter belongs to object       */
	((*at_info)->d26_at_idx != D26_IDX_NOT_DEF)
      {                             /* check access-rights            */
	if                          /* no access to attribute         */
	  ((*at_info)->d26_read_ok != TRUE)
	{                           /* set filter error indicator     */
	  ret_value = D2_ACCESS_RIGHTS_INSUFFICIENT;
	}
	else if                     /* filter value is to be checked  */
	       (acl_only == FALSE)
	{                           /* compare filter                 */
	  at  = d26_at + (*at_info)->d26_at_idx;
	  pos = at->d26a_field_pos[file_nr];
	  if                        /*  position is valid             */
	    (pos > 0 && ob_nr != (*at_info)->d26_srt_idx)
	  {                         /* compare with filter item       */
	    ret_value = d26_f08_cmp_fitem(f_item,file_nr,record,dname,at,
	      pos);
	  }
	  for                       /* all RDNs                       */
	     (idx = ob_nr, srt = d26_srt + idx;
	      idx != D26_ROOT && ret_value != D2_NO_ERR &&
	      (idx == ob_nr || fct_tag == D23_SEARCH);
	      idx = srt->d26s_par_nr, srt = d26_srt + idx)
	  {                         /* scan the naming attributes     */
	    for                     /* all naming attributes          */
	       (nam_at = srt->d26s_nam_at;
		nam_at->d26s_index != -1 && ret_value != D2_NO_ERR;
		nam_at++)
	    {                       /* compare AT indices             */
	      if                    /* indices match                  */
		(nam_at->d26s_index == (*at_info)->d26_at_idx)
	      {                     /* compare filters                */
		pos = nam_at->d26s_position[file_nr];
		ret_value = d26_f08_cmp_fitem(f_item,file_nr,record,dname,at,
		  pos);
	      }
	    }
	  }
	}
      }
      if                            /* no filter match for update     */
				    /* function                       */
	(ret_value == D26_NOFILTERMATCH && fct_tag == D23_MODENTR)
      {                             /* reset return value             */
	ret_value = D2_ATTRIBUTE_MISSING;
      }
      else if                       /* attribute missing for search   */
	     (ret_value == D2_ATTRIBUTE_MISSING && fct_tag == D23_SEARCH)
      {                               /* reset return value           */
	ret_value = D26_NOFILTERMATCH;
      }
      *f_err_item = f_item;
      *f_info = (D23_ft1_info *)d26_f07_next_fitem(f_item);
      (*at_info)++;
      break;
  }

  return(ret_value);

}                                   /* d26_f12_fa1_check              */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f13_cmp_dnlist_item()                           */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 22.09.89                                            */
/*                                                                    */
/* DESCRIPTION  : This function will look whether the recurring       */
/*                Distinguished-Name-List Attribute attribute from    */
/*                message matches for the own DSA.                    */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR       signed16        object matches filter item    */
/*      D26_NOFILTERMATCH               filter doesn't match attribute*/
/*                                      on object                     */
/*      D2_ATTRIBUTE_MISSING            filter attribute not present  */
/*                                      on object                     */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_f13_cmp_dnlist_item(
			/* IN    -  information about filter item     */
    D23_fi1_info *f_item)

{                                   /* d26_f13_cmp_dnlist_item        */

  signed16     ret_value;           /* return value                   */
  D23_at_type  *f_type;             /* type of filter attribute       */
  D23_av_value *f_value;            /* filter value                   */
  D23_fs1_info *f_substr;           /* filter substrings              */

  register signed16   idx;          /* loop variable                  */
  register D26_dndesc *ptr_dnlist;  /* pointer to distinguished name  */

  ret_value = D2_ATTRIBUTE_MISSING;

  for                               /* all DN-List entries            */
     (idx = 0; idx <= d26_maxidx_dnlist; idx++)
  {                             /* set pointer to next DN-List entry  */
    ptr_dnlist = d26_dnlist + idx;
    if                          /* entry is not a copy from other DSA */
				/* and is valid                       */
      (ptr_dnlist->d26_dnint != D2_COPY && ptr_dnlist->d26_occurrency > 0)
    {                           /* check matching rule                */
      ret_value = D26_NOFILTERMATCH;
      f_type = (D23_at_type *)(f_item + 1);
      switch(f_item->d23_fi1match)
      {
	case D2_APPROX_MATCH:
	case D2_EQUAL:
	  f_value = (D23_av_value *)((char *)(f_type + 1) +
	    f_type->d23_atoff);
	  if(!strcmp((char *)ptr_dnlist->d26_dnname,(char *)(f_value + 1)))
	  {
	    return(D2_NO_ERR);
	  }
	  break;
	case D2_SUBSTRINGS:
	  f_substr = (D23_fs1_info *)((char *)(f_type + 1) +
		     f_type->d23_atoff);
	  if(!strncmp((char *)ptr_dnlist->d26_dnname,(char *)(f_substr + 1),
	     f_substr->d23_fs1len - 1))
	  {
	    return(D2_NO_ERR);
	  }
	  break;
	case  D2_PRESENT:
	  return(D2_NO_ERR);
      }
    }
  }

				    /* return value                   */
  return(ret_value);

}                                   /* d26_f13_cmp_dnlist_item        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_f14_multigrep                                   */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 02.03.89                                            */
/*                                                                    */
/* DESCRIPTION  : The Function checks, whether the substrings         */
/*                described by f_substr occur in string str of length */
/*                len.                                                */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       Short           match was found               */
/*      D2_NFOUND                       no match                      */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_f14_multigrep(
    byte         *str,  /* IN    -  string to be checked for substr.  */
    signed16     len,   /* IN    -  length of string to be checked    */
			/* IN    -  filter substrings structure       */
    D23_fs1_info *f_substr)

{                   /* d26_f14_multigrep                              */

  D27_codeset codeset = D27_CS_103;

  while                             /* still substring to find        */
       (f_substr->d23_fs1tag == D23_FISTAG)
  {                                 /* check substrings position      */
    if                  /* substring does not match                   */
      (d26_f04_grep(&str,&len,&codeset,f_substr) == D2_NFOUND)
    {                   /* return no match                            */
      return(D2_NFOUND);
    }
    else
    {                   /* reset substring structure pointer          */
      f_substr = (D23_fs1_info *)((char *)(f_substr + 1) +
	f_substr->d23_fs1off);
    }
  }

  return(D2_NO_ERR);                /* return match                   */

}               /* d26_f14_multigrep                                  */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_f15_matching_rule_ok                            */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 30.09.92                                            */
/*                                                                    */
/* DESCRIPTION  : The Function checks, whether the given matching rule*/
/*                is ok for the specified attribute syntax.           */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      TRUE        boolean         matching rule is ok               */
/*      FALSE                       matching rule is not ok           */
/*                                                                    */
/*inoff ***************************************************************/

static boolean d26_f15_matching_rule_ok(
    signed16   syntax,  /* IN    -  attribute syntax                  */
    D2_a_match rule)    /* IN    -  matching rule                     */

{                   /* d26_f15_matching_rule_ok                       */

		    /* check matching rule                            */
  switch(rule)
  {
    case D2_PRESENT:
      return(TRUE);
    case D2_EQUAL:
    case D2_APPROX_MATCH:
		    /* check attribute-syntax                         */
      return(syntax == D2_ANY_STX || syntax == D2_GUIDE ||
	     syntax == D2_CERTIFICATE || syntax == D2_CERT_PAIR ||
	     syntax == D2_CERT_LIST ||
	     syntax == D2_PR_DM_STX || syntax == D2_ASN1 ? FALSE : TRUE);
    case D2_SUBSTRINGS:
      return(syntax == D2_ANY_STX || syntax == D2_OBJ_IDENT ||
	     syntax == D2_BOOLEAN || syntax == D2_INTEGER ||
	     syntax == D2_TIME || syntax == D2_GUIDE ||
	     syntax == D2_POST_AD_STX || syntax == D2_TLXNR_STX ||
	     syntax == D2_TTXID_STX || syntax == D2_FAXNR_STX ||
	     syntax == D2_PR_DM_STX || syntax == D2_PSAP_STX ||
	     syntax == D2_COUNTRY_STX || syntax == D2_PASSWD_STX ||
	     syntax == D2_CERTIFICATE || syntax == D2_CERT_PAIR ||
	     syntax == D2_CERT_LIST || syntax == D2_ACCL ||
	     syntax == D2_MHS_DLSP_STX || syntax == D2_MHS_ORADR_STX ||
	     syntax == D2_MHS_ORNAME_STX || syntax == D2_MHS_PR_DM_STX ||
	     syntax == D2_ASN1 ? FALSE : TRUE);
    case D2_GTEQ:
    case D2_LTEQ:
      return(syntax == D2_ANY_STX || syntax == D2_DISTNAME ||
	     syntax == D2_OBJ_IDENT || syntax == D2_CEXACT ||
	     syntax == D2_CIGNORE || syntax == D2_PRINTABLE ||
	     syntax == D2_IA5 || syntax == D2_NUMERIC ||
	     syntax == D2_CIGN_LIST || syntax == D2_BOOLEAN ||
	     syntax == D2_TELNR_STX || syntax == D2_GUIDE ||
	     syntax == D2_POST_AD_STX || syntax == D2_TLXNR_STX ||
	     syntax == D2_TTXID_STX || syntax == D2_FAXNR_STX ||
	     syntax == D2_PR_DM_STX || syntax == D2_PSAP_STX ||
	     syntax == D2_COUNTRY_STX || syntax == D2_PASSWD_STX ||
	     syntax == D2_CERTIFICATE || syntax == D2_CERT_PAIR ||
	     syntax == D2_CERT_LIST || syntax == D2_ACCL ||
	     syntax == D2_MHS_DLSP_STX || syntax == D2_MHS_ORADR_STX ||
	     syntax == D2_MHS_ORNAME_STX || syntax == D2_MHS_PR_DM_STX ||
	     syntax == D2_ASN1 ? FALSE : TRUE);
    default:
      return(FALSE);
  }

}                   /* d26_f15_matching_rule_ok                       */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_f16_check_equal_item                            */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 02.01.91                                            */
/*                                                                    */
/* DESCRIPTION  : The Function checks, whether given filter item is ok*/
/*                for equality    matching for the given AT-entry.    */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR   Short           match was found                   */
/*      D2_ATT_ERROR                attribute error                   */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_f16_check_equal_item(
    D26_at         *at, /* IN    -  pointer to attribute table entry  */
			/* INOUT -  pointer to beginning of message   */
    byte           **output,
			/* IN    -  DN of base object of search       */
    D2_name_string b_obj,
			/* IN    -  pointer to filter value in message*/
    D23_av_value   *f_value,
			/* INOUT -  maximum length of result message  */
    signed32       *output_len,
			/* INOUT -  remaining length for result data  */
    signed32       *remlen,
			/* IN    -  indicates calling function        */
    signed16       fct_tag)

{                   /* d26_f16_check_equal_item                       */

  signed16 element_size;    /* size of element for preferred-delivery-*/
			    /* method only                            */
  signed16 max_len;         /* maximum length of attribute value in   */
			    /* message format                         */
  signed32 meslen = 0;      /* message length                         */

  if                /* representations of schema and value differ     */
    (at->d26a_rep == D2_T61_PR &&
     f_value->d23_avrep != D2_T61 && f_value->d23_avrep != D2_PRINTABLE ||
     at->d26a_rep != D2_T61_PR && at->d26a_rep != f_value->d23_avrep)
  {                 /* create attribute error                         */
    DCE_SVC_LOG((GDS_S_WR_REP_IN_FILTER_MSG,at->d26a_type.d2_typ_len,
      at->d26a_type.d2_type,f_value->d23_avrep));
    d26_u15_attr_error(output,b_obj,1,D2_ATSYNTAX_ERROR,&at->d26a_type,
	  at->d26a_rep,f_value->d23_avrep,f_value->d23_avlen,
	  (byte *)(f_value + 1),output_len,&meslen);
    *remlen -= meslen;
    return(D2_ATT_ERROR);
  }
  else
  {                 /* check calling function                         */
    if              /* search has not called                          */
      (fct_tag != D23_SEARCH)
    {               /* set element size and maximum length            */
      switch(at->d26a_syntax)
      {
	case D2_PR_DM_STX:
	case D2_MHS_PR_DM_STX:
	  element_size = sizeof(signed32);
	  max_len      = at->d26a_ubound;
	  break;
	case D2_TLXNR_STX:
	  element_size = 1;
	  max_len      = at->d26a_ubound + D26_NR_COMP_TELEX_NR - 1;
	  break;
	case D2_POST_AD_STX:
	  element_size = 1;
	  max_len      = at->d26a_ubound + D26_UB_POSTAL_LINE - 1;
	  break;
	default:
	  element_size = 1;
	  max_len      = at->d26a_ubound;
	  break;
      }
      if            /* equality matching rule not ok for attribute    */
	(d26_f15_matching_rule_ok(at->d26a_syntax,D2_EQUAL) == FALSE)
      {             /* create attribute error                         */
	DCE_SVC_LOG((GDS_S_WR_MATCH_MSG,D2_EQUAL,at->d26a_syntax));
	d26_u15_attr_error(output,b_obj,1,D2_INAPPR_MATCH,&at->d26a_type,
	    at->d26a_rep,f_value->d23_avrep,f_value->d23_avlen,
	    (byte *)(f_value + 1),output_len,&meslen);
	*remlen -= meslen;
	return(D2_ATT_ERROR);
      }
      else if       /* wrong length of filter value                   */
	     (f_value->d23_avlen < at->d26a_lbound * element_size ||
	      f_value->d23_avlen > max_len * element_size)
      {             /* create attribute error                         */
	DCE_SVC_LOG((GDS_S_WR_AVLENGTH_MSG,f_value->d23_avrep,
	  f_value->d23_avlen,(byte *)(f_value + 1)));
	d26_u15_attr_error(output,b_obj,1,D2_CONSTRAINT_VIOLATION,
	  &at->d26a_type,at->d26a_rep,f_value->d23_avrep,f_value->d23_avlen,
	  (byte *)(f_value + 1),output_len,&meslen);
	*remlen -= meslen;
	return(D2_ATT_ERROR);
      }
    }
    return(d26_f03_check_filter_value(b_obj,at,f_value->d23_avrep,
	   (byte *)(f_value + 1),&f_value->d23_avlen,D2_EQUAL,0,output,
	   output_len,remlen));
  }

}               /* d26_f16_check_equal_item                           */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_f17_check_substrings_item                       */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 02.01.91                                            */
/*                                                                    */
/* DESCRIPTION  : The Function checks, whether given filter item is ok*/
/*                for equality    matching for the given AT-entry.    */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR   signed16        match was found                   */
/*      D2_ATT_ERROR                attribute error                   */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_f17_check_substrings_item(
    D26_at         *at, /* IN    -  pointer to attribute table entry  */
			/* INOUT -  pointer to beginning of message   */
    byte           **output,
			/* IN    -  DN of base object of search       */
    D2_name_string b_obj,
			/* IN    -  pointer to filter substrings in   */
    D23_fs1_info   *f_substr,       /* message                        */
			/* INOUT -  maximum length of result message  */
    signed32       *output_len,
			/* INOUT -  remaining length for result data  */
    signed32       *remlen)

{                   /* d26_f17_check_substrings_item                  */

  if                /* substrings matching rule not ok for attribute  */
    (d26_f15_matching_rule_ok(at->d26a_syntax,D2_SUBSTRINGS) == FALSE)
  {                 /* filter item will evaluate as undefined         */
    DCE_SVC_LOG((GDS_S_WR_MATCH_MSG,D2_SUBSTRINGS,at->d26a_syntax));
    return(D2_NO_ERR);
  }
  else
  {                 /* check whether substrings are OK                */
    return(d26_f06_check_filter_substrings(b_obj,at,f_substr,output,
      output_len,remlen));
  }

}                   /* d26_f17_check_substrings_item                  */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_f18_check_ordering_item                         */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 02.01.91                                            */
/*                                                                    */
/* DESCRIPTION  : The Function checks, whether given filter item is ok*/
/*                for ordering    matching for the given AT-entry.    */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR   signed16        match was found                   */
/*      D2_ATT_ERROR                attribute error                   */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_f18_check_ordering_item(
    D26_at         *at, /* IN    -  pointer to attribute table entry  */
			/* INOUT -  pointer to beginning of message   */
    byte           **output,
			/* IN    -  DN of base object of search       */
    D2_name_string b_obj,
			/* IN    -  pointer to filter value in message*/
    D23_av_value   *f_value,
			/* INOUT -  maximum length of result message  */
    signed32       *output_len,
			/* INOUT -  remaining length for result data  */
    signed32       *remlen)

{                   /* d26_f18_check_ordering_item                    */

  signed32 meslen = 0;  /* message length                             */

  if                /* representations of schema and value differ     */
    (at->d26a_rep == D2_T61_PR &&
     f_value->d23_avrep != D2_T61 && f_value->d23_avrep != D2_PRINTABLE ||
     at->d26a_rep != D2_T61_PR && at->d26a_rep != f_value->d23_avrep)
  {                 /* create attribute error                         */
    DCE_SVC_LOG((GDS_S_WR_REP_IN_FILTER_MSG,at->d26a_type.d2_typ_len,
      at->d26a_type.d2_type,f_value->d23_avrep));
    d26_u15_attr_error(output,b_obj,1,D2_ATSYNTAX_ERROR,&at->d26a_type,
	  at->d26a_rep,f_value->d23_avrep,f_value->d23_avlen,
	  (byte *)(f_value + 1),output_len,&meslen);
    *remlen -= meslen;
    return(D2_ATT_ERROR);
  }
  else if           /* ordering matching rule not ok for attribute    */
	 (d26_f15_matching_rule_ok(at->d26a_syntax,D2_GTEQ) == FALSE)
  {                 /* filter item will evaluate as undefined         */
    DCE_SVC_LOG((GDS_S_WR_MATCH_MSG,D2_GTEQ,at->d26a_syntax));
    return(D2_NO_ERR);
  }
  else
  {
    return(d26_f03_check_filter_value(b_obj,at,f_value->d23_avrep,
	   (byte *)(f_value + 1),&f_value->d23_avlen,D2_EQUAL,0,output,
	   output_len,remlen));
  }

}                   /* d26_f18_check_ordering_item                    */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f19_filter_attr_check                           */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 05.02.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function will look whether the filter          */
/*                attributes match to the object read already.        */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR       signed16      object matches filter           */
/*      D2_UNDEFINED_ATTR_TYPE        attribute type does not belong  */
/*                                    to object or illegal matching   */
/*      D2_ACCESS_RIGHTS_INSUFFICIENT access is not given on all      */
/*                                    filter attributes               */
/*      D26_NOFILTERMATCH             filter doesn't match attribute  */
/*                                    on object, but:                 */
/*      D2_ATTRIBUTE_MISSING          filter doesn't match attribute  */
/*                                    on object                       */
/*                                    (only for modify entry)         */
/*      D2_AT_VALUE_EXISTS            filter doesn't match attribute  */
/*                                    on object                       */
/*                                    (only for modify entry with     */
/*                                    negated filter item             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_f19_filter_attr_check(
			/* IN    -  record containing base object     */
    char           *record,
			/* IN    -  file number for object            */
    signed16       file_nr,
			/* IN    -  object number                     */
    signed16       ob_nr,
			/* IN    -  distinguished name of object found*/
    D2_name_string dname,
			/* INOUT -  Array containing the attribute    */
    D26_attr_info  *at_info,        /* types of filters               */
			/* IN    -  acl read from record              */
    D26_acl        *acl,
			/* INOUT -  information about filter          */
    D23_ft1_info   *f_info,         /* attributes                     */
			/* OUT   -  erroneous filter item             */
    D23_fi1_info   **f_err_item,
			/* IN    -  indicates calling function        */
    signed16       fct_tag,
			/* IN    -  indicates that only acl has to be */
    boolean        acl_only)        /* checked                        */

{                                   /* d26_f19_filter_attr_check      */

				    /* information for naming attr.   */
  D26_attr_info naming_attr[D2_NO_AVA + 1];
  signed16      ret_value;          /* return value                   */

  register D26_nam_at    *nam_at;   /* loop pointer to naming attrib. */
  register D26_attr_info *tmpati;   /* loop variable                  */

  fct_tag = fct_tag > D2_ADM_OPID ? fct_tag - D2_ADM_OPID : fct_tag;

  for                               /* all naming attributes          */
     (nam_at = d26_srt[ob_nr].d26s_nam_at, tmpati = naming_attr;
      nam_at->d26s_index != -1; nam_at++, tmpati++)
  {                                 /* set attribute index            */
    tmpati->d26_at_idx = nam_at->d26s_index;
  }

  tmpati->d26_at_idx = D26_EOL;


  if                                /* no access for naming attribute */
				    /* is given                       */
    (fct_tag != D23_BIND &&
     d26_u05_acl_check(dname,D26_READ,naming_attr,acl) != D2_NO_ERR)
  {                                 /* return error                   */
    ret_value = D2_ACCESS_RIGHTS_INSUFFICIENT;
  }
  else if                           /* no filter present              */
	 (f_info->d23_fp1tag != D23_PATTAG)
  {                                 /* return no error                */
    ret_value = D2_NO_ERR;
  }
  else
  {
    if                              /* function is not called by BIND */
				    /* or update function             */
      (fct_tag != D23_BIND && fct_tag != D23_MODENTR)
    {                               /* check acl                      */
      d26_u05_acl_check(dname,D26_READ,at_info,acl);
    }
    else
    {                               /* set all attributes to read ok  */
      for                           /* all filter attributes          */
	 (tmpati = at_info; tmpati->d26_at_idx != D26_EOL; tmpati++)
      {                             /* set read ok flag               */
	tmpati->d26_read_ok = TRUE;
      }
    }

				    /* check filters for access and   */
				    /* matching                       */
    ret_value = d26_f12_fa1_check(record,file_nr,ob_nr,dname,&at_info,acl,
      &f_info,f_err_item,acl_only,fct_tag);
  }

  DCE_SVC_LOG((GDS_S_FAT_CHECK_MSG,ret_value));
  return(ret_value);

}                                   /* d26_f19_filter_attr_check      */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f20_calc_fat_info                               */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 18.02.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function will calculate the indices of filter  */
/*                attributes in AT  belonging to the object class or  */
/*                one of the auxiliary object classes or to a naming  */
/*                attribute of a superior node to ob_nr.              */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   signed16        object matches filter             */
/*      D2_CONSTRAIN_VIOLATION      filter attributes too long        */
/*      D2_INAPPR_MATCH             phonetic comparison required      */
/*                                  although not allowed or illegal   */
/*                                  matching rule                     */
/*      D2_UNDEFINED_ATTR_TYPE      no such attribute in AT           */
/*                                                                    */
/*exoff ***************************************************************/

void d26_f20_calc_fat_info(
    signed16      ob_nr,/* IN    -  object number                     */
			/* IN    -  object class                      */
    signed32      *ob_cl,
			/* IN    -  information about filter          */
    D23_ft1_info  *f_info,          /* attributes                     */
			/* OUT   -  Array containing the attribute    */
    D26_attr_info *at_info)         /* types of filters               */

{                                   /* d26_f20_calc_fat_info          */


  D23_ft1_info *f_info1;            /* pointer to filter information  */

  if                                /* filter is present              */
    (f_info->d23_fp1tag == D23_PATTAG)
  {                                 /* write attribute informations   */
				    /* for filters                    */
    f_info1 = (D23_ft1_info *)f_info;
    d26_f11_get_attr_info(ob_nr,ob_cl,&f_info1,&at_info);
  }

  at_info->d26_at_idx = D26_EOL;

}                                   /* d26_f20_calc_fat_info          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_f22_is_ocl_leaf                                 */
/*                                                                    */
/* AUTHOR       : Kraemer, AP 11                                      */
/* DATE         : 22.09.92                                            */
/*                                                                    */
/* DESCRIPTION  : This function determines whether the object class   */
/*                denoted by the object identifier ob_id is a leaf or */
/*                not.                                                */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      TRUE        boolean         object class is leaf ...          */
/*      FALSE                       object class is not a leaf ...    */
/*                                  ... in object class tree          */
/*                                                                    */
/*exoff ***************************************************************/

boolean d26_f22_is_ocl_leaf(
    D2_obj_id *ob_id,   /* IN    -  object identifier for object class*/
    signed16  *oct_idx) /* OUT   -  OCT index of object class         */

{                                   /*  d26_f22_is_ocl_leaf           */

  signed16 ob_cl;                   /* Object class index             */

  register signed16 i;              /* loop variable                  */
  register D26_oct  *oct;           /* loop variable for OCT          */
  register signed16 *sup_cl;        /* loop pointer to superclass     */

  if                /* object identifier not in OCT                   */
    ((ob_cl = d26_u14_get_obj_class_idx(ob_id)) == D26_ERROR)
  {                 /* return no leaf                                 */
    return(FALSE);
  }
  else
  {                 /* search for subclasses                          */
    for             /* all OCT entries                                */
       (i = 0, oct = d26_oct; i < d26_nooct; i++, oct++)
    {               /* check whether it is immediate subordinate of   */
		    /* the given object class                         */
      for           /* all superclasses                               */
	 (sup_cl = oct->d26c_sup_cl; *sup_cl != -1; sup_cl++)
      {             /* compare superclass with given object class     */
	if          /* given object class is immediate superior       */
	  (*sup_cl == ob_cl)
	{           /* return no leaf                                 */
	  return(FALSE);
	}
      }
    }
		    /* return leaf                                    */
    *oct_idx = ob_cl;
    return(TRUE);
  }

}                                   /* d26_f22_is_ocl_leaf            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_f23_bo_matches_equal                              */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        05.10.92                                              */
/*                                                                    */
/* DESCRIPTION: This function checks, whether a filter value matches  */
/*              one of the nameparts of the base object of equality   */
/*              or approximately.                                     */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      TRUE        boolean         filter value matches base object  */
/*      FALSE                       filter value does not match       */
/*                                                                    */
/*exoff ***************************************************************/

boolean d26_f23_bo_matches_equal(
    D26_dninfo *dninfo, /* IN    -  information about found object    */
    signed16   np_count,/* IN    -  count of nameparts in base object */
    D2_a_match m_rule,  /* IN    -  matching rule from filter         */
    D26_at     *at,     /* IN    -  at entry of filter item           */
    byte       *value,  /* IN    -  value for index                   */
    signed16   length)  /* IN    -  length of value                   */

{                       /* d26_f23_bo_matches_equal                   */

				/* space to write the phonetic value  */
  byte np_phon[D26_LPHON];      /* ... of a namepart                  */
  byte fv_phon[D26_LPHON];      /* ... of a filter value              */

  register signed16 i,j;        /* loop variables                     */
  register D26_rdninfo *rdn;    /* loop pointer to RDNs of base object*/
  register D26_ava *ava;        /* loop pointer to AVAs of RDN        */

  if                    /* attribute admits phonetical matching       */
    (at->d26a_phon == TRUE)
  {                     /* write phonetical filter value              */
    d26_soundex(value,length,fv_phon);
  }

  for                   /* all nameparts of the base object           */
     (i = 0, rdn = dninfo->d26_rdn; i < np_count; i++, rdn++)
  {                     /* scan the AVAs of the RDN                   */
    for                 /* all AVAs of RDN                            */
       (j = 0, ava = rdn->d26_r_ava; j < rdn->d26_r_nava; j++, ava++)
    {                   /* compare the attribute types                */
      if                /* attribute types match                      */
	(d26_at + ava->d26_a_idx == at)
      {                 /* compare attribute values                   */
	switch(m_rule)
	{
	  case D2_APPROX_MATCH:
	    if          /* attribute admits phonetical matching       */
	      (at->d26a_phon == TRUE)
	    {           /* compare phonetical strings                 */
	      d26_soundex(ava->d26_a_val,ava->d26_a_len,np_phon);
	      if        /* phonetical strings match                   */
		(!memcmp(np_phon,fv_phon,D26_LPHON))
	      {         /* return match                               */
		return(TRUE);
	      }
	      break;
	    }
			/* compare for equality otherwise             */
	  case D2_EQUAL:
	    if          /* attribute values match                     */
	      (ava->d26_a_len == length &&
	       !memcmp(ava->d26_a_val,value,length))
	    {           /* return match                               */
	      return(TRUE);
	    }
	    break;
	}
      }
    }
  }

			/* return no match                            */
  return(FALSE);

}                       /* d26_f23_bo_matches_equal                   */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_f24_bo_matches_substrings                         */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        05.10.92                                              */
/*                                                                    */
/* DESCRIPTION: This function checks, whether a filter substrings     */
/*              one of the nameparts of the base object.              */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      TRUE        boolean         filter value matches base object  */
/*      FALSE                       filter value does not match       */
/*                                                                    */
/*exoff ***************************************************************/

boolean d26_f24_bo_matches_substrings(
    D26_dninfo *dninfo, /* IN    -  information about found object    */
    signed16   np_count,/* IN    -  count of nameparts in base object */
    D26_at     *at,     /* IN    -  at entry of filter item           */
			/* IN    -  filter stubstrings                */
    D23_fs1_info *f_substr)

{                                   /* d26_f24_bo_matches_substrings  */

  register signed16 i,j;        /* loop variables                     */
  register D26_rdninfo *rdn;    /* loop pointer to RDNs of base object*/
  register D26_ava *ava;        /* loop pointer to AVAs of RDN        */

  for                   /* all nameparts of the base object           */
     (i = 0, rdn = dninfo->d26_rdn; i < np_count; i++, rdn++)
  {                     /* scan the AVAs of the RDN                   */
    for                 /* all AVAs of RDN                            */
       (j = 0, ava = rdn->d26_r_ava; j < rdn->d26_r_nava; j++, ava++)
    {                   /* compare the attribute types                */
      if                /* attribute types match                      */
	(d26_at + ava->d26_a_idx == at)
      {                 /* compare the filter substrings with namep.  */
	if              /* filter substrings match                    */
	  (d26_f14_multigrep(ava->d26_a_val,ava->d26_a_len,f_substr)
	   == D2_NO_ERR)
	{               /* return match                               */
	  return(TRUE);
	}
      }
    }
  }

			/* return no match                            */
  return(FALSE);

}                       /* d26_f24_bo_matches_substrings              */
