/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21mhsfm.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:53  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:11  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:06:05  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:33:45  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:23:25  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:56:34  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:47:56  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:25:25  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:33:04  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:39:26  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  19:39:43  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:49:03  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:35:19  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21mhsfm.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:09 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21mhsfm.c                                          */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 21.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The module converts the mhs special attributes      */
/*                from apdu to iapl-format                            */
/*                                                                    */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <d21dua.h>

/******** LOCAL FUNCTIONS ********/

static void  d21_226_convert_or_name ( char ** , char ** , signed16 );
static void  d21_227_convert_or_addr ( char ** , char ** , signed16 );
static void  d21_228_convert_addrpart_string ( char ** , char ** );
static void  d21_229_convert_addrpart_strlist ( char ** , char ** );

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_221_or_addr_fr_ipc                                */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        05.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the OR_address attribute from   */
/*              apdu to iapl format                                   */
/*                                                                    */
/* INPUT PARAMETERS:  chng_out: flag to indicate if out-ptr is to     */
/*                              be changed                            */
/*                                                                    */
/* INOUT PARAMETERS:  in_msg:                                         */
/*                    INPUT   ptr-ptr to OR-address-message,          */
/*                            preceeded by structure D23_av_value     */
/*                    OUTPUT  ptr-ptr to first offset after input-msg */
/*                            (if chng_out is set)                    */
/*                                                                    */
/*                    out:                                            */
/*                    INPUT   ptr-ptr to struct D2_OR_address         */
/*                    OUTPUT  ptr-ptr to next free byte after struct  */
/*                            D2_OR_address and all succeeding        */
/*                            address strings                         */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d21_221_or_addr_fr_ipc(
  char ** in_msg,
  char ** out,
  Bool chng_out)
{
char         *out_p=*out;          /* Startaddr. (write)   */
char         *in_msg_p = *in_msg+D23_S_ATV;  /* Startaddr.  (read)   */
D23_av_value  *in_av = (D23_av_value *)*in_msg;
 
d21_227_convert_or_addr (&in_msg_p, &out_p, in_av->d23_avlen);

if (chng_out == TRUE)
  *out = out_p ;
*in_msg += D23_S_ATV + in_av->d23_avoff;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_222_or_name_fr_ipc                                */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        07.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the OR_name attribute from      */
/*              apdu to iapl format                                   */
/*                                                                    */
/* INPUT PARAMETERS:  chng_out: flag to indicate if out-ptr is to     */
/*                              be changed                            */
/*                                                                    */
/* INOUT PARAMETERS:  in_msg:                                         */
/*                    INPUT   ptr-ptr to OR-name-message              */
/*                            preceeded by structure D23_av_value     */
/*                    OUTPUT  ptr-ptr to first offset after input-msg */
/*                            (if chng_out is set)                    */
/*                                                                    */
/*                    out:                                            */
/*                    INPUT   ptr-ptr to struct D2_OR_name            */
/*                    OUTPUT  ptr-ptr to next free byte after struct  */
/*                            D2_OR_name and the succeeding           */
/*                            OR-address and/or the ds-name-string    */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d21_222_or_name_fr_ipc(
  char ** in_msg,
  char ** out,
  Bool chng_out )
{
char         *out_p=*out;          /* Startaddr. (write)   */
char         *in_msg_p = *in_msg+D23_S_ATV;  /* Startaddr.  (read)   */
D23_av_value  *in_av = (D23_av_value *)*in_msg;

d21_226_convert_or_name (&in_msg_p, &out_p, in_av->d23_avlen);

if (chng_out == TRUE)
  *out = out_p;
*in_msg += D23_S_ATV + in_av->d23_avoff;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_223_sub_perm_fr_ipc                               */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        28.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the submit-permission-attribute */
/*              from apdu to iapl format                              */
/*                                                                    */
/* INPUT PARAMETERS:  chng_out: flag to indicate if out-ptr is to     */
/*                              be changed                            */
/*                                                                    */
/* INOUT PARAMETERS:  in_msg:                                         */
/*                    INPUT   ptr-ptr to submit-perm-message          */
/*                            (structure D21_DL_sub_perm, followed by */
/*                            a OR-name-attr. or a ds-name-string),   */
/*                            preceeded by structure D23_av_value     */
/*                    OUTPUT  ptr-ptr to first offset after input-msg */
/*                            (if chng_out is set)                    */
/*                                                                    */
/*                    out:                                            */
/*                    INPUT   ptr to ptr to struct D2_dl_submit_perm  */
/*                    OUTPUT  ptr-ptr to next free byte after struct  */
/*                            D2_dl_submit_perm and the succeeding    */
/*                            D2_OR_name or name-string               */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d21_223_sub_perm_fr_ipc(
  char ** in_msg,
  char ** out,
  Bool chng_out)
{
char         *out_p=*out;          /* Startaddr. (write)   */
char         *in_msg_p = *in_msg+D23_S_ATV;  /* Startaddr.  (read)   */
D23_av_value  *in_av = (D23_av_value *)*in_msg;

D21_DL_sub_perm    *D21_sub_perm_p = (D21_DL_sub_perm *) in_msg_p;
D2_dl_submit_perm  *D2_sub_perm_p = (D2_dl_submit_perm*) out_p;
 
in_msg_p += sizeof (D21_DL_sub_perm);
out_p += sizeof (D2_dl_submit_perm);

D2_sub_perm_p->d2_perm_type = D21_sub_perm_p->d21_dlsp_tag;

switch (D21_sub_perm_p->d21_dlsp_tag)
   {
      case D21_DLSP_INDIV:
      case D21_DLSP_PATT:
      case D21_DLSP_MEM_DL:
	 /*-----------------------------------------------------------*/
	 /* convert D2_OR_name                                        */
	 /*-----------------------------------------------------------*/
	 D2_sub_perm_p->d2_choice.d2_individual = (D2_OR_name *) out_p;
	 d21_226_convert_or_name
		      (&in_msg_p, &out_p, D21_sub_perm_p->d21_dlsp_len);
	 break;
      case D21_DLSP_MEM_G:
	 /*-----------------------------------------------------------*/
	 /* convert D2_name_string                                    */
	 /*-----------------------------------------------------------*/
	 D2_sub_perm_p->d2_choice.d2_member_of_group = (D2_name_string)out_p;
	 memcpy (out_p, in_msg_p, D21_sub_perm_p->d21_dlsp_len);
	 out_p += D21_sub_perm_p->d21_dlsp_off;
	 in_msg_p += D21_sub_perm_p->d21_dlsp_off;
	 break;
      default:
	 break;
   }

if (chng_out == TRUE)
  *out = out_p;
*in_msg += D23_S_ATV + in_av->d23_avoff;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_226_convert_or_name                               */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        07.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads the OR_name attribute in apdu      */
/*              format from the input-message and writes it in iapl   */
/*              format to the output-memory.                          */
/*                                                                    */
/* INPUT PARAMETERS:  len: length of OR-name-message                  */
/*                                                                    */
/* INOUT PARAMETERS:  in:                                             */
/*                    INPUT   ptr-ptr to OR-name-message:             */
/*                            one or two structures D21_OR_name, each */
/*                            followed either by a ds-name-string or  */
/*                            by an OR-address.                       */
/*                    OUTPUT  ptr-ptr to first offset after input-msg */
/*                                                                    */
/*                    out:                                            */
/*                    INPUT   ptr-ptr to struct D2_OR_name            */
/*                    OUTPUT  ptr-ptr to next free byte after struct  */
/*                            D2_OR_name and the succeeding           */
/*                            OR-address and/or the ds-name-string    */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

static void d21_226_convert_or_name(
  char ** in,
  char ** out,
  signed16 len)
{
char          *end_of_in_msg = *in + len;
D21_OR_name    *D21_OR_name_p;
D2_OR_name     *D2_OR_name_p = (D2_OR_name *)*out;

/*---------------------------------------------------------------*/
/* initialize D2_OR_name                                         */
/*---------------------------------------------------------------*/
D2_OR_name_p->d2_dir_name = NULL;
D2_OR_name_p->d2_or_address = NULL;

*out += sizeof (D2_OR_name);

while (*in < end_of_in_msg)
  {
    D21_OR_name_p = (D21_OR_name *) *in;
    *in += sizeof (D21_OR_name);

    switch (D21_OR_name_p->d21_orn_tag)
      {
	case D21_ORN_DN:
	   D2_OR_name_p->d2_dir_name = (D2_name_string) *out;
	   memcpy (*out, *in, D21_OR_name_p->d21_orn_len);
	   *out += D21_OR_name_p->d21_orn_off;
	   *in += D21_OR_name_p->d21_orn_off;
	   break;
	case D21_ORN_ORA:
	   if (D21_OR_name_p->d21_orn_len != 0)
		{
	   	D2_OR_name_p->d2_or_address = (D2_OR_address *) *out;
	   	d21_227_convert_or_addr (in, out, D21_OR_name_p->d21_orn_len);
		}

	   break;
	default:
	   break;
      }
  }
} /* end of d21_226_convert_or_name */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_227_convert_or_addr                               */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        28.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads the OR_address attribute in apdu   */
/*              format from the input-message and writes it in iapl   */
/*              format to the output-memory.                          */
/*                                                                    */
/* INPUT PARAMETERS:  len: length of OR-address-message               */
/*                                                                    */
/* INOUT PARAMETERS:  in:                                             */
/*                    INPUT   ptr-ptr to OR-address-message:          */
/*                            list of structures D21_OR_addr_part,    */
/*                            each followed by a string or stringlist */
/*                    OUTPUT  ptr-ptr to first offset after last      */
/*                            address-part of input-msg               */
/*                                                                    */
/*                    out:                                            */
/*                    INPUT   ptr-ptr to struct D2_OR_address         */
/*                    OUTPUT  ptr-ptr to next free byte after struct  */
/*                            D2_OR_address and all succeeding        */
/*                            address parts                           */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

static void d21_227_convert_or_addr(
  char ** in,
  char ** out,
  signed16 len)
{
D2_OR_address *D2_OR_address_p = (D2_OR_address *) *out;
char         *end_of_in_msg = *in + len;
D21_OR_address_part *addr_part_p;
D2_str_list   *strlist_p;

char         *string_p, *end_of_strings;
signed32            str_len;

static D2_OR_address D2_OR_address_init;

/*---------------------------------------------------------------*/
/* initialize output-structure D2_OR_address                     */
/*---------------------------------------------------------------*/
*D2_OR_address_p = D2_OR_address_init;

/*---------------------------------------------------------------*/
/* set *out to first write-position (for addr-part-strings)      */
/*---------------------------------------------------------------*/
*out += sizeof (D2_OR_address);

/*---------------------------------------------------------------*/
/* assign values to output-structure D2_OR_address               */
/*---------------------------------------------------------------*/
while (*in < end_of_in_msg)
  {
    addr_part_p = (D21_OR_address_part *) *in;
    switch (addr_part_p->d21_ora_tag)
      {
	 case D21_ORA_ADMD_N:
	    /*---------------------------------------------------*/
	    /* d2_admd_name (D2_str_list)                        */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_admd_name = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_COM_N:
	    /*---------------------------------------------------*/
	    /* d2_common_name (D2_str_list)                      */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_common_name = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_CTRY_N:
	    /*---------------------------------------------------*/
	    /* d2_country_name (D2_str_list)                     */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_country_name = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_DOM_T1:
	    /*---------------------------------------------------*/
	    /* d2_domain_type1 (D2_str_list)                     */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_domain_type1 = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_DOM_T2:
	    /*---------------------------------------------------*/
	    /* d2_domain_type2 (D2_str_list)                     */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_domain_type2 = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_DOM_T3:
	    /*---------------------------------------------------*/
	    /* d2_domain_type3 (D2_str_list)                     */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_domain_type3 = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_DOM_T4:
	    /*---------------------------------------------------*/
	    /* d2_domain_type4 (D2_str_list)                     */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_domain_type4 = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_DOM_V1:
	    /*---------------------------------------------------*/
	    /* d2_domain_value1 (D2_str_list)                    */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_domain_value1 = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_DOM_V2:
	    /*---------------------------------------------------*/
	    /* d2_domain_value2 (D2_str_list)                    */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_domain_value2 = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_DOM_V3:
	    /*---------------------------------------------------*/
	    /* d2_domain_value3 (D2_str_list)                    */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_domain_value3 = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_DOM_V4:
	    /*---------------------------------------------------*/
	    /* d2_domain_value4 (D2_str_list)                    */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_domain_value4 = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_GEN:
	    /*---------------------------------------------------*/
	    /* d2_generation (D2_str_list)                       */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_generation = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_GIV_N:
	    /*---------------------------------------------------*/
	    /* d2_given_name (D2_str_list)                       */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_given_name = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_INIT:
	    /*---------------------------------------------------*/
	    /* d2_initials (D2_str_list)                         */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_initials = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_ISDN_N:
	    /*---------------------------------------------------*/
	    /* d2_isdn_number (Numeric_string)                   */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_isdn_number = (Numeric_string)*out;
	    d21_228_convert_addrpart_string (in, out);
	    break;
	 case D21_ORA_ISDN_S:
	    /*---------------------------------------------------*/
	    /* d2_isdn_subaddress (Numeric_string)               */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_isdn_subaddress = (Numeric_string)*out;
	    d21_228_convert_addrpart_string (in, out);
	    break;
	 case D21_ORA_NUM_ID:
	    /*---------------------------------------------------*/
	    /* d2_numeric_user_id (Numeric_string)               */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_numeric_user_id = (Numeric_string)*out;
	    d21_228_convert_addrpart_string (in, out);
	    break;
	 case D21_ORA_ORG_N:
	    /*---------------------------------------------------*/
	    /* d2_org_name (D2_str_list)                         */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_org_name = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_ORGU_N1:
	    /*---------------------------------------------------*/
	    /* d2_org_unit_name1 (D2_str_list)                   */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_org_unit_name1 = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_ORGU_N2:
	    /*---------------------------------------------------*/
	    /* d2_org_unit_name2 (D2_str_list)                   */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_org_unit_name2 = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_ORGU_N3:
	    /*---------------------------------------------------*/
	    /* d2_org_unit_name3 (D2_str_list)                   */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_org_unit_name3 = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_ORGU_N4:
	    /*---------------------------------------------------*/
	    /* d2_org_unit_name4 (D2_str_list)                   */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_org_unit_name4 = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_A_D:
	    /*---------------------------------------------------*/
	    /* d2_post_addr_details  (D2_str_list)               */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_addr_details = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_A_F:
	    /*---------------------------------------------------*/
	    /* d2_post_addr_in_full (T61_string)                 */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_addr_in_full = (T61_string)*out;
	    d21_228_convert_addrpart_string (in, out);
	    break;
	 case D21_ORA_P_CODE:
	    /*---------------------------------------------------*/
	    /* d2_post_code (D2_str_list)                        */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_code = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_CTRY:
	    /*---------------------------------------------------*/
	    /* d2_post_country_name (D2_str_list)                */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_country_name = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_D_P_N:
	    /*---------------------------------------------------*/
	    /* d2_post_deliv_pt_name (D2_str_list)               */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_deliv_pt_name = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_D_S_N:
	    /*---------------------------------------------------*/
	    /* d2_post_deliv_sys_name (Print_string)             */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_deliv_sys_name = (Print_string)*out;
	    d21_228_convert_addrpart_string (in, out);
	    break;
	 case D21_ORA_P_G_D_A:
	    /*---------------------------------------------------*/
	    /* d2_post_gen_deliv_addr (D2_str_list)              */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_gen_deliv_addr = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_LOC:
	    /*---------------------------------------------------*/
	    /* d2_post_locale (D2_str_list)                      */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_locale = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_O_B_N:
	    /*---------------------------------------------------*/
	    /* d2_post_office_box_nbr (D2_str_list)              */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_office_box_nbr = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_O_N:
	    /*---------------------------------------------------*/
	    /* d2_post_office_name (D2_str_list)                 */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_office_name = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_O_NUM:
	    /*---------------------------------------------------*/
	    /* d2_post_office_nbr (D2_str_list)                  */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_office_nbr = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_ORG_N:
	    /*---------------------------------------------------*/
	    /* d2_post_org_name (D2_str_list)                    */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_org_name = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_P_D:
	    /*---------------------------------------------------*/
	    /* d2_post_patron_details (D2_str_list)              */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_patron_details = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_P_N:
	    /*---------------------------------------------------*/
	    /* d2_post_patron_name (D2_str_list)                 */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_patron_name = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_P_S_A:
	    /*---------------------------------------------------*/
	    /* d2_post_street_addr (D2_str_list)                 */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_street_addr = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_PRES_A:
	    /*---------------------------------------------------*/
	    /* d2_presentation_addr (Psap_string)                */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_presentation_addr = (Psap_string)*out;
	    addr_part_p = (D21_OR_address_part *) *in;
	    *in += sizeof (D21_OR_address_part);
	    memcpy (*out, *in, addr_part_p->d21_ora_len);
	    *in +=  addr_part_p->d21_ora_off;
	    *out += addr_part_p->d21_ora_off;
	    break;
	 case D21_ORA_PRMD_N:
	    /*---------------------------------------------------*/
	    /* d2_prmd_name (D2_str_list)                        */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_prmd_name = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_SURNAME:
	    /*---------------------------------------------------*/
	    /* d2_surname (D2_str_list)                          */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_surname = (D2_str_list *) *out;
	    d21_229_convert_addrpart_strlist (in, out);
	    break;
	 case D21_ORA_T_ID:
	    /*---------------------------------------------------*/
	    /* d2_terminal_id (Print_string)                     */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_terminal_id = (Print_string)*out;
	    d21_228_convert_addrpart_string (in, out);
	    break;
	 case D21_ORA_T_T:
	    /*---------------------------------------------------*/
	    /* d2_terminal_type (Int)                            */
	    /*---------------------------------------------------*/
	    addr_part_p = (D21_OR_address_part *) *in;
	    *in += sizeof (D21_OR_address_part);
	    D2_OR_address_p->d2_terminal_type = * ((signed32 *) *in);
	    *in += addr_part_p->d21_ora_off;
	    break;
	 case D21_ORA_X121_A:
	    /*---------------------------------------------------*/
	    /* d2_x121_addr (Numeric_string)                     */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_x121_addr = (Numeric_string)*out;
	    d21_228_convert_addrpart_string (in, out);
	    break;
	 case D21_ORA_P_A_L:
	    /*---------------------------------------------------*/
	    /* d2_post_addr_in_lines (D2_str_list)               */
	    /*---------------------------------------------------*/
	    D2_OR_address_p->d2_post_addr_in_lines = (D2_str_list *) *out;

	    string_p = *in + sizeof (D21_OR_address_part);
	    end_of_strings = string_p + addr_part_p->d21_ora_len;

	    while (string_p < end_of_strings)
	       {
		  strlist_p = (D2_str_list *) *out;
		  *out += sizeof (D2_str_list);
		  str_len = strlen ((char *)string_p);
		  strlist_p->d2_rep = D2_T61;
		  strlist_p->d2_str.d2_size = str_len;
		  strlist_p->d2_str.d2_value = (unsigned char *)*out;
		  memcpy (*out, string_p, str_len+1);
		  *out += D2_ALIGN(str_len+1);

		  string_p += str_len + 1;

		  if (string_p < end_of_strings)
		     strlist_p->d2_next = (D2_str_list *) *out;
		  else
		     strlist_p->d2_next = NULL;
	       }

	    *in += sizeof (D21_OR_address_part) + addr_part_p->d21_ora_off;
	    break;

	 default:
	    break;
      }
  }
} /* end of d21_227_convert_or_addr */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_228_convert_addrpart_string                       */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        05.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads one address-part (of type "string")*/
/*              in apdu format from the input-message and writes the  */
/*              address-string (without '\0') to the output-memory.   */
/*                                                                    */
/* INOUT PARAMETERS:  in:                                             */
/*                    INPUT   ptr-ptr to address-part-message:        */
/*                            structure D21_OR_address_part followed  */
/*                            by one string.                          */
/*                    OUTPUT  ptr-ptr to first offset after address-  */
/*                            part-struct and addr-part-string        */
/*                                                                    */
/*                    out:                                            */
/*                    INPUT   ptr-ptr to next free byte after         */
/*                            D2_OR_address and preceeding address-   */
/*                            strings                                 */
/*                    OUTPUT  INPUT incremented by lenght of the      */
/*                            current address-part-string             */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

static void d21_228_convert_addrpart_string(
  char ** in,
  char ** out)
{
D21_OR_address_part *addr_part_p;

addr_part_p = (D21_OR_address_part *) *in;
*in += sizeof (D21_OR_address_part);

memcpy (*out, *in, addr_part_p->d21_ora_len);

*in +=  addr_part_p->d21_ora_off;
*out += addr_part_p->d21_ora_off;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_229_convert_addrpart_strlist                      */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        31.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads one address-part (type stringlist) */
/*              in apdu format from the input-message and writes all  */
/*              chained addr-structures (D2_str_list) and all address-*/
/*              strings ('\0'-terminated) to the output-memory.       */
/*                                                                    */
/* INOUT PARAMETERS:  in:                                             */
/*                    INPUT   ptr-ptr to address-part-message:        */
/*                            structure D21_OR_address_part followed  */
/*                            by one or more structures D21_str_list, */
/*                            each followed by one string.            */
/*                    OUTPUT  ptr-ptr to first offset after address-  */
/*                            part-struct and string-list             */
/*                                                                    */
/*                    out:                                            */
/*                    INPUT   ptr-ptr to next free byte after D2_OR_- */
/*                            address and preceeding address-strings  */
/*                    OUTPUT  INPUT incremented by lenghts of all     */
/*                            address-part-strings of the current     */
/*                            string-list                             */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*                                                                    */
/*exoff ***************************************************************/

static void d21_229_convert_addrpart_strlist(
  char ** in,
  char ** out)
{
D21_OR_address_part *addr_part_p = (D21_OR_address_part *) *in;
D21_str_list        *D21_str_list_p;
D2_str_list         *D2_str_list_p;

char               *in_str_list = *in + sizeof (D21_OR_address_part);
char               *end_of_in_msg_p = in_str_list+addr_part_p->d21_ora_len;

while (in_str_list < end_of_in_msg_p)
   {
      D21_str_list_p = (D21_str_list *) in_str_list;
      D2_str_list_p = (D2_str_list *) *out;
      *out += sizeof (D2_str_list);

      D2_str_list_p->d2_rep = D21_str_list_p->d21_strl_rep;
      D2_str_list_p->d2_str.d2_size = D21_str_list_p->d21_strl_len - 1;
      D2_str_list_p->d2_str.d2_value = (unsigned char *)*out;

      in_str_list += sizeof (D21_str_list);
      memcpy (*out, in_str_list, D21_str_list_p->d21_strl_len);
      in_str_list += D21_str_list_p->d21_strl_off;
      *out += D21_str_list_p->d21_strl_off;

      if (in_str_list < end_of_in_msg_p)
	 D2_str_list_p->d2_next = (D2_str_list *) *out;
      else
	 D2_str_list_p->d2_next = NULL;
   }


*in += sizeof (D21_OR_address_part) + addr_part_p->d21_ora_off;
}
