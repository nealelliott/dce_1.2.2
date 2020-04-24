/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21mhstm.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:05  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:19  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:41:55  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:34:33  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:28:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:57:52  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:48:14  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:25:41  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  18:08:20  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	[1992/12/04  11:06:34  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  21:34:55  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:40:41  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  19:40:37  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:49:34  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:37:13  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21mhstm.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:19 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21mhstm                                            */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 05.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The module converts the mhs special attributes      */
/*                from iapl to apdu-format.                           */
/*                                                                    */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <d21dua.h>

/******** LOCAL FUNCTIONS ********/

static d2_ret_val d21_206_convert_or_name ( D2_OR_name * , char ** );
static d2_ret_val d21_207_convert_or_addr ( D2_OR_address * , char ** );
static void d21_208_convert_addrpart_string ( char * , char ** );
static void d21_209_convert_addrpart_strlist ( D2_str_list * , char ** );

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_201_or_addr_to_ipc                                */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        28.10.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the OR_address attribute from   */
/*              iapl to apdu format                                   */
/*                                                                    */
/* INPUT PARAMETERS:  in: pointer to OR-address attribute (structure) */
/*                                                                    */
/* INOUT PARAMETERS:  out:                                            */
/*                    INPUT   ptr-ptr to OR-address-message,          */
/*                            preceeded by structure D23_av_value     */
/*                    OUTPUT  ptr-ptr to first offset after message   */
/*                                                                    */
/* RETURN VALUE :       D2_NOERROR      for no error                  */
/*                      D2_ERROR for syntax error             */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_201_or_addr_to_ipc(
  D2_OR_address * in,
  char ** out)
{
char * out_p = *out + D23_S_ATV;
D23_av_value * av_out = (D23_av_value *) *out;

if ( d21_207_convert_or_addr (in, &out_p) != D2_NOERROR )
	{
	return (D2_ERROR);
	}

av_out->d23_avlen = out_p - (*out + D23_S_ATV);
av_out->d23_avoff = D2_ALIGN (av_out->d23_avlen);
*out += D23_S_ATV + av_out->d23_avoff;

return (D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_202_or_name_to_ipc                                */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        28.10.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the OR_name attribute from      */
/*              iapl to apdu format                                   */
/*                                                                    */
/* INPUT PARAMETERS:  in: pointer to OR-name attribute (structure)    */
/*                                                                    */
/* INOUT PARAMETERS:  out:                                            */
/*                    INPUT   ptr-ptr to OR-name-message,             */
/*                            preceeded by structure D23_av_value     */
/*                    OUTPUT  ptr-ptr to first offset after message   */
/*                                                                    */
/* RETURN VALUE :     D2_NOERROR      for no error                    */
/*                    D2_ERROR for syntax error               */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_202_or_name_to_ipc(
  D2_OR_name * in,
  char ** out)
{
char * out_p = *out + D23_S_ATV;
D23_av_value * av_out = (D23_av_value *) *out;

if ( d21_206_convert_or_name (in, &out_p) != D2_NOERROR )
	{
	return(D2_ERROR);
	}

av_out->d23_avlen = out_p - (*out + D23_S_ATV);
av_out->d23_avoff = D2_ALIGN(av_out->d23_avlen);
*out += D23_S_ATV + av_out->d23_avoff;

return (D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_203_sub_perm_to_ipc                               */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        28.10.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads the submit-permission-attribute in */
/*              apdu format and writes it in iapl format to the       */
/*              output-message.                                       */
/*                                                                    */
/* INPUT PARAMETERS:  in:    pointer to submit-permission attribute   */
/*                           (structure D2_OR_dl_submit_perm)         */
/*                                                                    */
/* INOUT PARAMETERS:  out:                                            */
/*                    INPUT   ptr-ptr to submit-permission-message,   */
/*                            preceeded by structure D23_av_value     */
/*                    OUTPUT  ptr-ptr to first offset after message   */
/*                            (structure D21_DL_sub_perm, followed by */
/*                            a OR-name-attr. or a ds-name-string)    */
/*                                                                    */
/* RETURN VALUE :       D2_NOERROR      for no error                  */
/*                      D2_ERROR for syntax error             */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_203_sub_perm_to_ipc(
  D2_dl_submit_perm * in,
  char ** out)
{
char * out_p = *out + D23_S_ATV;
D23_av_value * av_out = (D23_av_value *) *out;

D21_DL_sub_perm     *sub_perm_p = (D21_DL_sub_perm *) out_p;
signed16                 i;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
out_p += sizeof (D21_DL_sub_perm);

sub_perm_p -> d21_dlsp_tag = in -> d2_perm_type;
sub_perm_p -> d21_dlsp_fil = 0;

switch (in -> d2_perm_type)
   {
      case D21_DLSP_INDIV:
      case D21_DLSP_PATT:
      case D21_DLSP_MEM_DL:
	 /*-----------------------------------------------------------*/
	 /* convert D2_OR_name                                        */
	 /*-----------------------------------------------------------*/
	 if ( d21_206_convert_or_name (in->d2_choice.d2_member_of_dl, &out_p)
				!= D2_NOERROR)
	 {
	 return(D2_ERROR);
	 }

	 sub_perm_p->d21_dlsp_len = out_p - (*out+D23_S_ATV)
					  - sizeof (D21_DL_sub_perm);
	 sub_perm_p->d21_dlsp_off = D2_ALIGN (sub_perm_p->d21_dlsp_len);
	 break;
      case D21_DLSP_MEM_G:
	 /*-----------------------------------------------------------*/
	 /* convert D2_name_string                                    */
	 /*-----------------------------------------------------------*/
	 sub_perm_p->d21_dlsp_len =
		   strlen ((char *)in->d2_choice.d2_member_of_group) + 1;
	 sub_perm_p->d21_dlsp_off = D2_ALIGN (sub_perm_p->d21_dlsp_len);
	 memcpy (out_p, in->d2_choice.d2_member_of_group,
					      sub_perm_p->d21_dlsp_len - 1);
	 for (i=sub_perm_p->d21_dlsp_len - 1; i<sub_perm_p->d21_dlsp_off; i++)
	     *(out_p+i) = '\0';
	 out_p += sub_perm_p->d21_dlsp_off;
	 break;
      default:
	 return (D2_ERROR);
   }


av_out->d23_avlen = out_p - (*out + D23_S_ATV);
av_out->d23_avoff = D2_ALIGN(av_out->d23_avlen);
*out += D23_S_ATV + av_out->d23_avoff;

return (D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_206_convert_or_name                               */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        28.10.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads the OR_name attribute in iapl      */
/*              format from the input-structure and writes it in apdu */
/*              format to the output-message.                         */
/*                                                                    */
/* INPUT PARAMETERS:  in: pointer to OR-name attribute structure      */
/*                                                                    */
/* INOUT PARAMETERS:  out:                                            */
/*                    INPUT   ptr-ptr to OR-name-message              */
/*                    OUTPUT  ptr-ptr to first offset after message   */
/*                            (one or two structures D21_OR_name, each*/
/*                            followed either by a ds-name-string or  */
/*                            by an OR-address).                      */
/*                                                                    */
/* RETURN VALUE :       D2_NOERROR      for no error                  */
/*                      D2_ERROR for syntax error             */
/*                                                                    */
/*exoff ***************************************************************/

static d2_ret_val d21_206_convert_or_name (
  D2_OR_name * in,
  char ** out)
{
char           * out_p_sav;
signed16             i;
D21_OR_name     *or_name_p = (D21_OR_name *) *out;

if (in -> d2_dir_name != NULL)
   {
    /*----------------------------------------------------------------*/
    /* convert Name                                                   */
    /*----------------------------------------------------------------*/
      or_name_p->d21_orn_tag = D21_ORN_DN;
      or_name_p->d21_orn_fil = 0;
      or_name_p->d21_orn_len = strlen ((char *)in->d2_dir_name) + 1;
      or_name_p->d21_orn_off = D2_ALIGN (or_name_p->d21_orn_len);
      *out += sizeof (D21_OR_name);

      memcpy (*out, in -> d2_dir_name, or_name_p->d21_orn_len - 1);
      for (i=or_name_p->d21_orn_len - 1; i<or_name_p->d21_orn_off; i++)
	  *(*out+i) = '\0';
      *out += or_name_p->d21_orn_off;
   }

if (in -> d2_or_address != NULL)
   {
    /*----------------------------------------------------------------*/
    /* convert OR-Address                                             */
    /*----------------------------------------------------------------*/
      or_name_p = (D21_OR_name *) *out;
      *out += sizeof (D21_OR_name);
      out_p_sav = *out;
      if (d21_207_convert_or_addr (in->d2_or_address, out) != D2_NOERROR)
	{
	return(D2_ERROR);
	}

      or_name_p->d21_orn_tag = D21_ORN_ORA;
      or_name_p->d21_orn_fil = 0;
      or_name_p->d21_orn_len = *out - out_p_sav;
      or_name_p->d21_orn_off = D2_ALIGN (or_name_p->d21_orn_len);
   }

else
   {
      or_name_p = (D21_OR_name *) *out;
      *out += sizeof (D21_OR_name);

      or_name_p->d21_orn_tag = D21_ORN_ORA;
      or_name_p->d21_orn_fil = 0;
      or_name_p->d21_orn_len = 0;
      or_name_p->d21_orn_off = 0;
   }
    
return (D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_207_convert_or_addr                               */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        28.10.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads the OR_address attribute in iapl   */
/*              format from the input-structure and writes it in apdu */
/*              format to the output-message.                         */
/*                                                                    */
/* INPUT PARAMETERS:  in: pointer to OR-address attribute (structure) */
/*                                                                    */
/* INOUT PARAMETERS:  out:                                            */
/*                    INPUT   ptr-ptr to OR-address-message           */
/*                    OUTPUT  ptr-ptr to first offset after last      */
/*                            address-part of OR_address-message      */
/*                            (list of structures D21_OR_addr_part,   */
/*                            each followed by a string or stringlist)*/
/*                                                                    */
/* RETURN VALUE :       D2_NOERROR      for no error                  */
/*                      D2_ERROR for syntax error             */
/*                                                                    */
/*exoff ***************************************************************/

static d2_ret_val d21_207_convert_or_addr(
  D2_OR_address * in,
  char ** out)
{
D21_OR_address_part  *addr_part_p;
D2_str_list          *strlist_p;
struct t_adrdesc     *t_adrdesc_p;

char                *out_save;
signed16                  i;

if (in -> d2_country_name != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_country_name (D2_str_list)                                */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_CTRY_N;
      d21_209_convert_addrpart_strlist (in->d2_country_name, out);
   }

if (in -> d2_admd_name != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_admd_name (D2_str_list) d21spafm.c                        */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_ADMD_N;
      d21_209_convert_addrpart_strlist (in->d2_admd_name, out);
   }

if (in -> d2_x121_addr != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_x121_addr (Numeric_string)                                */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_X121_A;
      d21_208_convert_addrpart_string (in->d2_x121_addr, out);
   }

if (in -> d2_terminal_id != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_terminal_id (Print_string)                                */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_T_ID;
      d21_208_convert_addrpart_string (in->d2_terminal_id, out);
   }


if (in -> d2_prmd_name  != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_prmd_name (D2_str_list)                                   */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_PRMD_N;
      d21_209_convert_addrpart_strlist (in->d2_prmd_name, out);
   }

if (in -> d2_common_name != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_common_name (D2_str_list)                                 */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_COM_N;
      d21_209_convert_addrpart_strlist (in -> d2_common_name, out);
   }

if (in -> d2_org_name != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_org_name (D2_str_list)                                    */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_ORG_N;
      d21_209_convert_addrpart_strlist (in -> d2_org_name, out);
   }

if (in -> d2_numeric_user_id != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_numeric_user_id (Numeric_string)                          */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_NUM_ID;
      d21_208_convert_addrpart_string (in->d2_numeric_user_id, out);
   }

if (in -> d2_surname != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_surname (D2_str_list)                                     */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_SURNAME;
      d21_209_convert_addrpart_strlist (in -> d2_surname, out);
   }

if (in -> d2_given_name != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_given_name (D2_str_list)                                  */
      /*--------------------------------------------------------------*/

      if (in -> d2_surname == NULL)
	{
	return (D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_GIV_N;
      d21_209_convert_addrpart_strlist (in -> d2_given_name, out);
   }

if (in -> d2_initials != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_initials (D2_str_list)                                    */
      /*--------------------------------------------------------------*/

      if (in -> d2_surname == NULL)
	{
	return (D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_INIT;
      d21_209_convert_addrpart_strlist (in -> d2_initials, out);
   }

if (in -> d2_generation != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_generation (D2_str_list)                                  */
      /*--------------------------------------------------------------*/

      if (in -> d2_surname == NULL)
	{
	return (D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_GEN;
      d21_209_convert_addrpart_strlist (in -> d2_generation, out);
   }

if (in -> d2_org_unit_name1 != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_org_unit_name1 (D2_str_list)                              */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_ORGU_N1;
      d21_209_convert_addrpart_strlist (in -> d2_org_unit_name1, out);
   }

if (in -> d2_org_unit_name2 != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_org_unit_name2 (D2_str_list)                              */
      /*--------------------------------------------------------------*/

      if (in -> d2_org_unit_name1 == NULL)
	{
	return(D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_ORGU_N2;
      d21_209_convert_addrpart_strlist (in -> d2_org_unit_name2, out);
   }

if (in -> d2_org_unit_name3 != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_org_unit_name3 (D2_str_list)                              */
      /*--------------------------------------------------------------*/

      if (in -> d2_org_unit_name1 == NULL ||
          in -> d2_org_unit_name2 == NULL)
	{
	return(D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_ORGU_N3;
      d21_209_convert_addrpart_strlist (in -> d2_org_unit_name3, out);
   }

if (in -> d2_org_unit_name4 != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_org_unit_name4 (D2_str_list)                              */
      /*--------------------------------------------------------------*/

      if (in -> d2_org_unit_name1 == NULL ||
          in -> d2_org_unit_name2 == NULL ||
          in -> d2_org_unit_name3 == NULL)
	{
	return(D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_ORGU_N4;
      d21_209_convert_addrpart_strlist (in -> d2_org_unit_name4, out);
   }

if (in -> d2_domain_type1 != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_domain_type1 (D2_str_list)                                */
      /*--------------------------------------------------------------*/

      if (in -> d2_domain_value1 == NULL)
	{
	return (D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_DOM_T1;
      d21_209_convert_addrpart_strlist (in -> d2_domain_type1, out);
   }

if (in -> d2_domain_value1 != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_domain_value1 (D2_str_list)                               */
      /*--------------------------------------------------------------*/

      if ( in -> d2_domain_type1 == NULL )
	{
	return(D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_DOM_V1;
      d21_209_convert_addrpart_strlist (in -> d2_domain_value1, out);
   }

if (in -> d2_domain_type2 != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_domain_type2 (D2_str_list)                                */
      /*--------------------------------------------------------------*/

      if ( in -> d2_domain_type1 == NULL ||
	   in -> d2_domain_value1 == NULL ||
	   in -> d2_domain_value2 == NULL )
	{
	return(D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_DOM_T2;
      d21_209_convert_addrpart_strlist (in -> d2_domain_type2, out);
   }

if (in -> d2_domain_value2 != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_domain_value2 (D2_str_list)                               */
      /*--------------------------------------------------------------*/

      if ( in -> d2_domain_type1 == NULL ||
	   in -> d2_domain_value1 == NULL ||
	   in -> d2_domain_type2 == NULL )
	{
	return (D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_DOM_V2;
      d21_209_convert_addrpart_strlist (in -> d2_domain_value2, out);
   }

if (in -> d2_domain_type3 != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_domain_type3 (D2_str_list)                                */
      /*--------------------------------------------------------------*/

      if ( in -> d2_domain_type1 == NULL ||
	   in -> d2_domain_value1 == NULL ||
	   in -> d2_domain_type2 == NULL ||
	   in -> d2_domain_value2 == NULL ||
	   in -> d2_domain_value3 == NULL )
	{
	return (D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_DOM_T3;
      d21_209_convert_addrpart_strlist (in -> d2_domain_type3, out);
   }

if (in -> d2_domain_value3 != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_domain_value3 (D2_str_list)                               */
      /*--------------------------------------------------------------*/

      if ( in -> d2_domain_type1 == NULL ||
	   in -> d2_domain_value1 == NULL ||
	   in -> d2_domain_type2 == NULL ||
	   in -> d2_domain_value2 == NULL ||
	   in -> d2_domain_type3 == NULL )
	{
	return (D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_DOM_V3;
      d21_209_convert_addrpart_strlist (in -> d2_domain_value3, out);
   }

if (in -> d2_domain_type4 != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_domain_type4 (D2_str_list)                                */
      /*--------------------------------------------------------------*/

      if ( in -> d2_domain_type1 == NULL ||
	   in -> d2_domain_value1 == NULL ||
	   in -> d2_domain_type2 == NULL ||
	   in -> d2_domain_value2 == NULL ||
	   in -> d2_domain_type3 == NULL ||
	   in -> d2_domain_value3 == NULL ||
	   in -> d2_domain_value4 == NULL )
	{
	return (D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_DOM_T4;
      d21_209_convert_addrpart_strlist (in -> d2_domain_type4, out);
   }

if (in -> d2_domain_value4 != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_domain_value4 (D2_str_list)                               */
      /*--------------------------------------------------------------*/

      if ( in -> d2_domain_type1 == NULL ||
	   in -> d2_domain_value1 == NULL ||
	   in -> d2_domain_type2 == NULL ||
	   in -> d2_domain_value2 == NULL ||
	   in -> d2_domain_type3 == NULL ||
	   in -> d2_domain_value3 == NULL ||
	   in -> d2_domain_type4 == NULL )
	{
	return (D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_DOM_V4;
      d21_209_convert_addrpart_strlist (in -> d2_domain_value4, out);
   }

if (in -> d2_post_deliv_sys_name != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_deliv_sys_name (Print_string)                        */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_D_S_N;
      d21_208_convert_addrpart_string (in->d2_post_deliv_sys_name, out);
   }

if (in -> d2_post_country_name != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_country_name (D2_str_list)                           */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_CTRY;
      d21_209_convert_addrpart_strlist (in->d2_post_country_name, out);
   }

if (in -> d2_post_code != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_code (D2_str_list)                                   */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_CODE;
      d21_209_convert_addrpart_strlist (in->d2_post_code, out);
   }

if (in -> d2_post_office_name != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_office_name (D2_str_list)                            */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_O_N;
      d21_209_convert_addrpart_strlist (in->d2_post_office_name, out);
   }

if (in -> d2_post_office_nbr != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_office_nbr (D2_str_list)                             */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_O_NUM;
      d21_209_convert_addrpart_strlist (in->d2_post_office_nbr, out);
   }

if (in -> d2_post_addr_details != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_addr_details  (D2_str_list)                          */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_A_D;
      d21_209_convert_addrpart_strlist (in -> d2_post_addr_details, out);
   }

if (in -> d2_post_patron_name != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_patron_name (D2_str_list)                            */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_P_N;
      d21_209_convert_addrpart_strlist (in->d2_post_patron_name, out);
   }

if (in -> d2_post_org_name != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_org_name (D2_str_list)                               */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_ORG_N;
      d21_209_convert_addrpart_strlist (in->d2_post_org_name, out);
   }

if (in -> d2_post_patron_details != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_patron_details (D2_str_list)                         */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_P_D;
      d21_209_convert_addrpart_strlist (in->d2_post_patron_details,out);
   }

if (in -> d2_post_addr_in_full  != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_addr_in_full (T61_string)                            */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_A_F;
      d21_208_convert_addrpart_string ((char *)in->d2_post_addr_in_full, out);
   }

if (in -> d2_post_addr_in_lines != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_addr_in_lines (D2_str_list)                          */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      *out += sizeof (D21_OR_address_part);

      out_save = *out;

      strlist_p = in->d2_post_addr_in_lines;
      while (strlist_p != NULL)
	 {
	     memcpy (*out, strlist_p->d2_str.d2_value,
			     strlist_p->d2_str.d2_size);
	     *out += strlist_p->d2_str.d2_size;
	     **out = '\0';
	     (*out)++;
	     strlist_p = strlist_p->d2_next;
	 }

      addr_part_p -> d21_ora_tag = D21_ORA_P_A_L;
      addr_part_p -> d21_ora_len = *out - out_save;
      addr_part_p -> d21_ora_off = D2_ALIGN (addr_part_p->d21_ora_len);
      addr_part_p -> d21_ora_fil = 0;

      for (i=addr_part_p->d21_ora_len; i<addr_part_p->d21_ora_off; i++)
	 {
	    **out = '\0';
	    (*out)++;
	 }
   }

if (in -> d2_post_street_addr != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_street_addr (D2_str_list)                            */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_S_A;
      d21_209_convert_addrpart_strlist (in->d2_post_street_addr, out);
   }

if (in -> d2_post_office_box_nbr != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_office_box_nbr (D2_str_list)                         */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_O_B_N;
      d21_209_convert_addrpart_strlist (in->d2_post_office_box_nbr,out);
   }

if (in -> d2_post_gen_deliv_addr != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_gen_deliv_addr (D2_str_list)                         */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_G_D_A;
      d21_209_convert_addrpart_strlist (in->d2_post_gen_deliv_addr, out);
   }

if (in -> d2_post_deliv_pt_name != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_deliv_pt_name (D2_str_list)                          */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_D_P_N;
      d21_209_convert_addrpart_strlist (in->d2_post_deliv_pt_name,out);
   }

if (in -> d2_post_locale != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_post_locale (D2_str_list)                                 */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_P_LOC;
      d21_209_convert_addrpart_strlist (in->d2_post_locale, out);
   }

if (in -> d2_presentation_addr != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_presentation_addr (Psap_string)                           */
      /*--------------------------------------------------------------*/
      t_adrdesc_p = (struct t_adrdesc *) in->d2_presentation_addr;

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_PRES_A;
      addr_part_p -> d21_ora_fil = 0;
      addr_part_p -> d21_ora_len = t_adrdesc_p->t_translng;
      addr_part_p -> d21_ora_off = D2_ALIGN (addr_part_p->d21_ora_len);

      *out += sizeof (D21_OR_address_part);
      memcpy (*out, (String) in -> d2_presentation_addr,
					  addr_part_p -> d21_ora_len);
      for (i=addr_part_p->d21_ora_len; i<addr_part_p->d21_ora_off; i++)
	  *(*out+i) = '\0';

      *out += addr_part_p -> d21_ora_off;
   }

if (in -> d2_isdn_number != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_isdn_number (Numeric_string)                              */
      /*--------------------------------------------------------------*/

      if ( in -> d2_presentation_addr != NULL )
	{
	return (D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_ISDN_N;
      d21_208_convert_addrpart_string (in->d2_isdn_number, out);
   }

if (in -> d2_isdn_subaddress != NULL)
   {
      /*--------------------------------------------------------------*/
      /* d2_isdn_subaddress (Numeric_string)                          */
      /*--------------------------------------------------------------*/

      if ( in -> d2_presentation_addr != NULL ||
	   in -> d2_isdn_number == NULL )
	{
	return (D2_ERROR);
	}

      addr_part_p = (D21_OR_address_part *) *out;
      addr_part_p -> d21_ora_tag = D21_ORA_ISDN_S;
      d21_208_convert_addrpart_string (in->d2_isdn_subaddress, out);
   }

if (in -> d2_terminal_type != 0)
   {
      /*--------------------------------------------------------------*/
      /* d2_terminal_type (Int)                                       */
      /*--------------------------------------------------------------*/
      addr_part_p = (D21_OR_address_part *) *out;

      addr_part_p -> d21_ora_tag = D21_ORA_T_T;
      addr_part_p -> d21_ora_fil = 0;
      addr_part_p -> d21_ora_len = sizeof (signed32);
      addr_part_p -> d21_ora_off = D2_ALIGN (addr_part_p->d21_ora_len);

      *out += sizeof (D21_OR_address_part);
      memcpy (*out, &(in->d2_terminal_type), sizeof (signed32));
      *out += D2_ALIGN (sizeof (signed32));
   }

return(D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_208_convert_addrpart_string                       */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        31.10.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads one OR-address-part (type "string")*/
/*              in iapl format from the input-pointer and writes it   */
/*              in apdu format to the output-message.                 */
/*              note: d21_OR_address_part.d21_ora_tag is  n o t       */
/*                    assigned by this function (should be assigned   */
/*                    before call).                                   */
/*                                                                    */
/* INPUT PARAMETERS:  in: pointer to string                           */
/*                                                                    */
/* INOUT PARAMETERS:  out:                                            */
/*                    INPUT   ptr-ptr to first offset after preceeding*/
/*                            address-part of input-message           */
/*                    OUTPUT  ptr-ptr to first offset after current   */
/*                            address-part of input-message           */
/*                            (structure d21_OR_address_part, followed*/
/*                            by one string)                          */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

static void d21_208_convert_addrpart_string(
  char * in,
  char ** out)
{
D21_OR_address_part *D21_OR_address_part_p = (D21_OR_address_part *)*out;
signed16                 i;

D21_OR_address_part_p -> d21_ora_fil = 0;
D21_OR_address_part_p -> d21_ora_len = strlen (in) + 1;
D21_OR_address_part_p -> d21_ora_off =
			D2_ALIGN (D21_OR_address_part_p->d21_ora_len);

*out += sizeof (D21_OR_address_part);

memcpy (*out, in, D21_OR_address_part_p->d21_ora_len);
for (i=D21_OR_address_part_p->d21_ora_len;
			     i<D21_OR_address_part_p->d21_ora_off; i++)
    *(*out+i) = '\0';

*out += D21_OR_address_part_p->d21_ora_off;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_209_convert_addrpart_strlist                      */
/*                                                                    */
/* AUTHOR:      G. Rustemeyer D AP 11                                 */
/* DATE:        31.10.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads one OR-address-part (type str-list)*/
/*              in iapl format from the input-structure and writes it */
/*              in apdu format to the output-message.                 */
/*              note: d21_OR_address_part.d21_ora_tag is  n o t       */
/*                    assigned by this function (should be assigned   */
/*                    before call).                                   */
/*                                                                    */
/* INPUT PARAMETERS:  in: pointer to string-list (struct D2_str_list) */
/*                                                                    */
/* INOUT PARAMETERS:  out:                                            */
/*                    INPUT   ptr-ptr to first offset after preceeding*/
/*                            address-part of OR_address-message      */
/*                    OUTPUT  ptr-ptr to first offset after current   */
/*                            address-part of input-message           */
/*                            (structure d21_OR_address_part, followed*/
/*                            by one or more structures d21_str_list, */
/*                            each followed by a string)              */
/*exoff ***************************************************************/

static void d21_209_convert_addrpart_strlist(
  D2_str_list * in,
  char ** out)
{
D21_OR_address_part *D21_OR_address_part_p = (D21_OR_address_part *)*out;
D21_str_list        *D21_str_list_p;
char               *out_sav;
signed16 i;

*out += sizeof (D21_OR_address_part);
out_sav = *out;

while (in != NULL)
   {
    /*----------------------------------------------------------------*/
    /*  copy size, offset and d2_rep to msg (*out)                    */
    /*----------------------------------------------------------------*/
      D21_str_list_p = (D21_str_list *) *out;

      D21_str_list_p->d21_strl_off = D2_ALIGN (in->d2_str.d2_size + 1);
      D21_str_list_p->d21_strl_len = in->d2_str.d2_size + 1;
      D21_str_list_p->d21_strl_rep = in -> d2_rep;
      D21_str_list_p->d21_strl_fil = 0;

      *out += sizeof (D21_str_list);

    /*----------------------------------------------------------------*/
    /*  copy string to msg (*out)                                     */
    /*----------------------------------------------------------------*/
      memcpy (*out, in->d2_str.d2_value, D21_str_list_p->d21_strl_len - 1);

      for (i=D21_str_list_p->d21_strl_len - 1;
				  i < D21_str_list_p->d21_strl_off; i++)
	  *(*out+i) = '\0';

      *out += D21_str_list_p->d21_strl_off;
      in =  in -> d2_next;

   }

D21_OR_address_part_p -> d21_ora_fil = 0;
D21_OR_address_part_p -> d21_ora_off = D2_ALIGN (*out - out_sav);
D21_OR_address_part_p -> d21_ora_len = *out - out_sav;
}
