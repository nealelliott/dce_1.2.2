/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21mhslm.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:57  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:14  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:06:07  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:34:00  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:25:33  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:57:00  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:48:03  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:25:31  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  18:07:54  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	[1992/12/04  11:05:34  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  21:33:42  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:39:52  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  19:40:02  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:49:14  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:35:32  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21mhslm.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:12 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21mhslm.c                                          */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 19.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The module computes the lengths of the mhs special  */
/*                attributes (in apdu-format)                         */
/*                                                                    */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <d21dua.h>

/******** LOCAL FUNCTIONS ********/

static signed32  d21_214_strlist_msglen ( D2_str_list * , signed16 );

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_211_or_addr_msglen  (in C-module  d21mhslm)       */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        13.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads an OR-address-attribute in iapl-   */
/*              format and computes the length of the corresponding   */
/*              message (apdu-format)                                 */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to OR-address-attribute (struct)  */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE :  length of OR-address-message                       */
/*                 D2_ERROR in case of error (that is, if one of the  */
/*                          octet-strings has length 0 or one of the  */
/*                          string-lists is not correct)              */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_211_or_addr_msglen(
  D2_OR_address * in)
{
signed32 msglen = 0;
struct t_adrdesc     *t_adrdesc_p;
D2_str_list          *strlist_p;
signed32                   strlist_len, str_len;

/*-----------------------------------------------------------------*/
/* d2_admd_name (D2_str_list, type 0)                              */
/*-----------------------------------------------------------------*/
if (in -> d2_admd_name != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_admd_name,0))
							== D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_common_name (D2_str_list, type 2)                            */
/*-----------------------------------------------------------------*/
if (in -> d2_common_name != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_common_name,2))
							== D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_country_name (D2_str_list, type 1)                           */
/*-----------------------------------------------------------------*/
if (in -> d2_country_name != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_country_name,1))
							== D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_domain_type1 (D2_str_list, type 2)                           */
/*-----------------------------------------------------------------*/
if (in -> d2_domain_type1 != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_domain_type1,2))
							== D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_domain_type2 (D2_str_list, type 2)                           */
/*-----------------------------------------------------------------*/
if (in -> d2_domain_type2 != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_domain_type2,2))
							== D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_domain_type3 (D2_str_list, type 2)                           */
/*-----------------------------------------------------------------*/
if (in -> d2_domain_type3 != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_domain_type3,2))
							== D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_domain_type4 (D2_str_list, type 2)                           */
/*-----------------------------------------------------------------*/
if (in -> d2_domain_type4 != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_domain_type4,2))
							== D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_domain_value1 (D2_str_list, type 2)                          */
/*-----------------------------------------------------------------*/
if (in -> d2_domain_value1 != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_domain_value1,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_domain_value2 (D2_str_list, type 2)                          */
/*-----------------------------------------------------------------*/
if (in -> d2_domain_value2 != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_domain_value2,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_domain_value3 (D2_str_list, type 2)                          */
/*-----------------------------------------------------------------*/
if (in -> d2_domain_value3 != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_domain_value3,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_domain_value4 (D2_str_list, type 2)                          */
/*-----------------------------------------------------------------*/
if (in -> d2_domain_value4 != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_domain_value4,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_generation (D2_str_list, type 2)                             */
/*-----------------------------------------------------------------*/
if (in -> d2_generation != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_generation,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_given_name (D2_str_list, type 2)                             */
/*-----------------------------------------------------------------*/
if (in -> d2_given_name != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_given_name,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_initials (D2_str_list, type 2)                               */
/*-----------------------------------------------------------------*/
if (in -> d2_initials != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_initials,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_isdn_number (Numeric_string)                                 */
/*-----------------------------------------------------------------*/
if (in -> d2_isdn_number != NULL)
   if ((str_len = strlen (in->d2_isdn_number)) == 0)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + D2_ALIGN (str_len+1);

/*-----------------------------------------------------------------*/
/* d2_isdn_subaddress (Numeric_string)                             */
/*-----------------------------------------------------------------*/
if (in -> d2_isdn_subaddress != NULL)
   if ((str_len = strlen (in->d2_isdn_subaddress)) == 0)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + D2_ALIGN (str_len+1);

/*-----------------------------------------------------------------*/
/* d2_numeric_user_id (Numeric_string)                             */
/*-----------------------------------------------------------------*/
if (in -> d2_numeric_user_id != NULL)
   if ((str_len = strlen (in->d2_numeric_user_id)) == 0)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + D2_ALIGN (str_len+1);

/*-----------------------------------------------------------------*/
/* d2_org_name (D2_str_list, type 2)                               */
/*-----------------------------------------------------------------*/
if (in -> d2_org_name != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_org_name,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_org_unit_name1 (D2_str_list, type 2)                         */
/*-----------------------------------------------------------------*/
if (in -> d2_org_unit_name1 != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_org_unit_name1,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_org_unit_name2 (D2_str_list, type 2)                         */
/*-----------------------------------------------------------------*/
if (in -> d2_org_unit_name2 != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_org_unit_name2,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_org_unit_name3 (D2_str_list, type 2)                         */
/*-----------------------------------------------------------------*/
if (in -> d2_org_unit_name3 != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_org_unit_name3,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_org_unit_name4 (D2_str_list, type 2)                         */
/*-----------------------------------------------------------------*/
if (in -> d2_org_unit_name4 != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_org_unit_name4,2))
							  == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_addr_details  (D2_str_list, type 2)                     */
/*-----------------------------------------------------------------*/
if (in -> d2_post_addr_details != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_addr_details,2))
							    == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_addr_in_full (T61_string)                               */
/*-----------------------------------------------------------------*/
if (in -> d2_post_addr_in_full  != NULL)
   if ((str_len = strlen ((char *)in->d2_post_addr_in_full)) == 0)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + D2_ALIGN (str_len+1);

/*-----------------------------------------------------------------*/
/* d2_post_code (D2_str_list, type 1)                              */
/*-----------------------------------------------------------------*/
if (in -> d2_post_code != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_code,1))
							== D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_country_name (D2_str_list, type 1)                      */
/*-----------------------------------------------------------------*/
if (in -> d2_post_country_name != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_country_name,1))
							== D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_deliv_pt_name (D2_str_list, type 2)                     */
/*-----------------------------------------------------------------*/
if (in -> d2_post_deliv_pt_name != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_deliv_pt_name,2))
							     == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_deliv_sys_name (Print_string)                           */
/*-----------------------------------------------------------------*/
if (in -> d2_post_deliv_sys_name != NULL)
   if ((str_len = strlen (in->d2_post_deliv_sys_name)) == 0)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + D2_ALIGN (str_len+1);

/*-----------------------------------------------------------------*/
/* d2_post_gen_deliv_addr (D2_str_list, type 2)                    */
/*-----------------------------------------------------------------*/
if (in -> d2_post_gen_deliv_addr != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_gen_deliv_addr,2))
							      == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_locale (D2_str_list, type 2)                            */
/*-----------------------------------------------------------------*/
if (in -> d2_post_locale != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_locale,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_office_box_nbr (D2_str_list, type 2)                    */
/*-----------------------------------------------------------------*/
if (in -> d2_post_office_box_nbr != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_office_box_nbr,2))
							      == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_office_name (D2_str_list, type 2)                       */
/*-----------------------------------------------------------------*/
if (in -> d2_post_office_name != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_office_name,2))
							   == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_office_nbr (D2_str_list, type 2)                        */
/*-----------------------------------------------------------------*/
if (in -> d2_post_office_nbr != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_office_nbr,2))
							  == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_org_name (D2_str_list, type 2)                          */
/*-----------------------------------------------------------------*/
if (in -> d2_post_org_name != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_org_name,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_patron_details (D2_str_list, type 2)                    */
/*-----------------------------------------------------------------*/
if (in -> d2_post_patron_details != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_patron_details,2))
							      == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_patron_name (D2_str_list, type 2)                       */
/*-----------------------------------------------------------------*/
if (in -> d2_post_patron_name != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_patron_name,2))
							   == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_post_street_addr (D2_str_list, type 2)                       */
/*-----------------------------------------------------------------*/
if (in -> d2_post_street_addr != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_post_street_addr,2))
							   == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_presentation_addr (Psap_string)                              */
/*-----------------------------------------------------------------*/
if (in -> d2_presentation_addr != NULL)
   {
      t_adrdesc_p = (struct t_adrdesc *) in->d2_presentation_addr;
      msglen += sizeof (D21_OR_address_part) +
		D2_ALIGN (t_adrdesc_p->t_translng);
   }

/*-----------------------------------------------------------------*/
/* d2_prmd_name (D2_str_list, type 1)                              */
/*-----------------------------------------------------------------*/
if (in -> d2_prmd_name != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_prmd_name,1))
							== D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_surname (D2_str_list, type 2)                                */
/*-----------------------------------------------------------------*/
if (in -> d2_surname != NULL)
   if ((strlist_len = d21_214_strlist_msglen (in->d2_surname,2))
							 == D2_ERROR)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + strlist_len;

/*-----------------------------------------------------------------*/
/* d2_terminal_id (Print_string)                                   */
/*-----------------------------------------------------------------*/
if (in -> d2_terminal_id != NULL)
   if ((str_len = strlen (in->d2_terminal_id)) == 0)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + D2_ALIGN (str_len+1);

/*-----------------------------------------------------------------*/
/* d2_terminal_type (Int)                                          */
/*-----------------------------------------------------------------*/
if ( in -> d2_terminal_type != 0 )
   {
      if (in -> d2_terminal_type < D2_MHS_T_T_LBOUND  ||
	  in -> d2_terminal_type > D2_MHS_T_T_UBOUND    )
	 return (D2_ERROR);
      msglen += sizeof (D21_OR_address_part) +
		D2_ALIGN (sizeof(signed32));
   }

/*-----------------------------------------------------------------*/
/* d2_x121_addr (Numeric_string)                                   */
/*-----------------------------------------------------------------*/
if (in -> d2_x121_addr != NULL)
   if ((str_len = strlen (in->d2_x121_addr)) == 0)
      return (D2_ERROR);
   else
      msglen += sizeof (D21_OR_address_part) + D2_ALIGN (str_len+1);

/*-----------------------------------------------------------------*/
/* d2_post_addr_in_lines (sequence of T61-strings)                 */
/*-----------------------------------------------------------------*/
if (in -> d2_post_addr_in_lines != NULL)
   {
      msglen += sizeof (D21_OR_address_part);
      strlist_p = in->d2_post_addr_in_lines;
      while (strlist_p != NULL)
	 {
	    msglen += strlist_p->d2_str.d2_size + 1;
	    strlist_p = strlist_p->d2_next;
	 }
   }

return (D2_ALIGN(msglen));
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_212_or_name_msglen  (in C-module  d21mhslm)       */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        13.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads an OR-name-attribute in iapl-      */
/*              format and computes the length of the corresponding   */
/*              message (apdu-format)                                 */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to OR-name-attribute (struct)     */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE :  length of OR-name-message                          */
/*                 D2_ERROR in case of error (that is, if both        */
/*                          pointers of struct. D2_OR_name have value */
/*                          null or the name-string has length 0 or   */
/*                          the D2_OR_address-struct. is not correct  */
/*                          or OR-address is null)                    */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_212_or_name_msglen(
  D2_OR_name * in )
{
signed32        msglen = 0;
signed32        str_len, addr_len;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
if (in->d2_dir_name == NULL  &&  in->d2_or_address == NULL)
   return (D2_ERROR);

if (in->d2_dir_name != NULL)
   {
      if ((str_len = strlen ((char *)in->d2_dir_name)) == 0)
	 return (D2_ERROR);
      else
	 msglen += sizeof (D21_OR_name) + D2_ALIGN (str_len+1);
   }

if (in->d2_or_address != NULL)
   {
      if ((addr_len=d21_211_or_addr_msglen(in->d2_or_address)) == D2_ERROR)
	 return (D2_ERROR);
      else
	 msglen += sizeof (D21_OR_name) + addr_len;
   }
else
   msglen += sizeof (D21_OR_name);   

return (msglen);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_213_sub_perm_msglen  (in C-module  d21mhslm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        13.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads a submit-perm-attribute in iapl-   */
/*              format and computes the length of the corresponding   */
/*              message (apdu-format)                                 */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to sub-perm-attribute (struct)    */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE :  length of submit-permission-message                */
/*                 D2_ERROR in case of error (that is, if a           */
/*                          D2_OR_name-structure is not correct or    */
/*                          the name-string has length 0, or the      */
/*                          representation is unknown)                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_213_sub_perm_msglen(
  D2_dl_submit_perm * in)
{
signed32       msglen = 0;
signed32       str_len, nam_len;

switch (in->d2_perm_type)
   {
      case D21_DLSP_INDIV:
      case D21_DLSP_PATT:
      case D21_DLSP_MEM_DL:
	 /*-----------------------------------------------------------*/
	 /* length of D2_OR_name                                      */
	 /*-----------------------------------------------------------*/
	 if ((nam_len=d21_212_or_name_msglen(in->d2_choice.d2_individual))
							      == D2_ERROR)
	    return (D2_ERROR);
	 else
	    msglen = sizeof (D21_DL_sub_perm) + nam_len;
	 break;
      case D21_DLSP_MEM_G:
	 /*-----------------------------------------------------------*/
	 /* length of D2_name_string                                  */
	 /*-----------------------------------------------------------*/
	 if ((str_len = strlen ((char *)in->d2_choice.d2_member_of_group)) == 0)
	    return (D2_ERROR);
	 else
	 msglen = sizeof (D21_DL_sub_perm) + D2_ALIGN (str_len+1);
	 break;
      default:
	 return (D2_ERROR);
   }

return (msglen);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_214_strlist_msglen  (in C-module  d21mhslm)       */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        13.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads a list of structures D2_str_list   */
/*              and computes the length of the corresponding string-  */
/*              list-message (apdu-format)                            */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to first structure D2_str_list    */
/*                      type: type of stringlist:                     */
/*                            0 -> D2_PRINTABLE or D2_NUMERIC-string, */
/*                                 Nullstring allowed                 */
/*                            1 -> D2_PRINTABLE or D2_NUMERIC-string, */
/*                                 Nullstring not allowed             */
/*                            2 -> D2_PRINTABLE and/or D2_T61-string  */
/*                            6 -> up to 6 D2_T61-strings             */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE :  length of stringlist-message                       */
/*                 D2_ERROR in case of error (that is if the number   */
/*                          of structures D2_str_list exceeds the     */
/*                          limit given by "type"  or                 */
/*                          a representation appears too often)       */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d21_214_strlist_msglen(
  D2_str_list * in,
  signed16 type)
{
signed32       msglen = 0;
signed32       str_size;
signed32       str_count=0, prt_count=0, num_count=0, t61_count=0;
signed32       max_nb_str, max_nb_prt, max_nb_num, max_nb_t61;

/*----------------------------------------------------------------*/
/* set upper limits for number of strings                         */
/*----------------------------------------------------------------*/
switch (type)
   {
      case 0:
      case 1:
	 max_nb_str = 1;
	 max_nb_prt = 1;
	 max_nb_t61 = 0;
	 max_nb_num = 1;
	 break;
      case 2:
	 max_nb_str = 2;
	 max_nb_prt = 1;
	 max_nb_t61 = 1;
	 max_nb_num = 0;
	 break;
      case 6:
	 max_nb_str = 6;
	 max_nb_prt = 0;
	 max_nb_t61 = 6;
	 max_nb_num = 0;
	 break;
      default:
	 return (D2_ERROR);
   }

/*----------------------------------------------------------------*/
/* add lengths of all structures D21_str_list and strings         */
/*----------------------------------------------------------------*/
while (in != NULL)
   {
      str_count ++;
      switch (in->d2_rep)
	 {
	    case  D2_PRINTABLE:
	       prt_count ++;
	       break;
	    case D2_T61:
	       t61_count ++;
	       break;
	    case D2_NUMERIC:
	       num_count ++;
	       break;
	    default:
		 return (D2_ERROR);
	 }

      /*----------------------------------------------------------*/
      /* size 1 (i.e. strlen 0 + '\0') is allowed for type 0 only */
      /*----------------------------------------------------------*/
      str_size = in->d2_str.d2_size;
      if ( type != 0  &&  str_size  < 1)
	 return (D2_ERROR);

      msglen += sizeof (D21_str_list) + D2_ALIGN (str_size + 1);
      in =  in -> d2_next;
   }


/*----------------------------------------------------------------*/
/* test number of strings                                         */
/*----------------------------------------------------------------*/
if (str_count > max_nb_str     ||
    prt_count > max_nb_prt     ||
    num_count > max_nb_num     ||
    t61_count > max_nb_t61        )
   return (D2_ERROR);

return (msglen);
}


