/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26mhs.c,v $
 * Revision 1.1.324.2  1996/02/18  19:45:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:49  marty]
 *
 * Revision 1.1.324.1  1995/12/08  15:45:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:13  root]
 * 
 * Revision 1.1.322.2  1994/06/21  14:45:35  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:53:26  marrek]
 * 
 * Revision 1.1.322.1  1994/02/22  16:06:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:14:35  marrek]
 * 
 * Revision 1.1.320.2  1993/10/14  17:16:34  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:10:41  keutel]
 * 
 * Revision 1.1.320.1  1993/10/13  17:30:54  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:33:22  keutel]
 * 
 * Revision 1.1.2.2  1993/08/10  14:18:41  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:12:29  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26mhs.c,v $ $Revision: 1.1.324.2 $ $Date: 1996/02/18 19:45:33 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : d26mhs                                              */
/*                                                                    */
/* AUTHOR       : E. Kraemer, AP 11                                   */
/* DATE         : 19.04.93                                            */
/*                                                                    */
/* COMPONENT    : D2                                                  */
/*                                                                    */
/* DOC.-NR.     : D2-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*                                                                    */
/* DESCRIPTION  : The module contains functions, that norm the        */
/*                MHS attribute values in APDU format.                */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*      1.0 | 19.04.93| Birth                          | gr |         */
/*datoff **************************************************************/

/*--------------------------------------------------------------------*/
/*                        I N C L U D E S                             */
/*--------------------------------------------------------------------*/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

/*****  external Includes    *****/

#include <dce/d2dir.h>
#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d21spat.h>

/*--------------------------------------------------------------------*/
/*                        D E F I N E S                               */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                 T Y P E -  D E F I N I T I O N S                   */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                 D E C L A R A T I O N S        DATA                */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*--------------------------------------------------------------------*/

static void d26_un4_norm_strlist(char **in, char **out);
static void d26_un5_norm_string(char **in, char **out, unsigned16 rep);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_un1_norm_or_address                               */
/*                                                                    */
/* AUTHOR:      E. Kraemer    D AP 11                                 */
/* DATE:        19.04.93                                              */
/*                                                                    */
/* DESCRIPTION: The function norms the OR_address attribute value in  */
/*              APDU format                                           */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      >= 0        signed16        length of normed OR address       */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_un1_norm_or_address(
    char     *in,       /* IN    -  OR address to norm                */
    signed16 len,       /* IN    -  length of OR address              */
    char     *out)      /* OUT   -  normed OR-address                 */

{                       /* d26_un1_norm_or_address                    */

  char *end_of_in_msg    = in + len;
  char *start_of_out_msg = out;

  while                 /* end of input message not reached           */
       (in < end_of_in_msg)
  {                     /* check type of address part                 */
    switch(((D21_OR_address_part *)in)->d21_ora_tag)
    {
      case D21_ORA_ADMD_N:
      case D21_ORA_COM_N:
      case D21_ORA_CTRY_N:
      case D21_ORA_DOM_T1:
      case D21_ORA_DOM_T2:
      case D21_ORA_DOM_T3:
      case D21_ORA_DOM_T4:
      case D21_ORA_DOM_V1:
      case D21_ORA_DOM_V2:
      case D21_ORA_DOM_V3:
      case D21_ORA_DOM_V4:
      case D21_ORA_GEN:
      case D21_ORA_GIV_N:
      case D21_ORA_INIT:
      case D21_ORA_ORG_N:
      case D21_ORA_ORGU_N1:
      case D21_ORA_ORGU_N2:
      case D21_ORA_ORGU_N3:
      case D21_ORA_ORGU_N4:
      case D21_ORA_P_A_D:
      case D21_ORA_P_CODE:
      case D21_ORA_P_CTRY:
      case D21_ORA_P_D_P_N:
      case D21_ORA_P_G_D_A:
      case D21_ORA_P_LOC:
      case D21_ORA_P_O_B_N:
      case D21_ORA_P_O_N:
      case D21_ORA_P_O_NUM:
      case D21_ORA_P_ORG_N:
      case D21_ORA_P_P_D:
      case D21_ORA_P_P_N:
      case D21_ORA_P_S_A:
      case D21_ORA_PRMD_N:
      case D21_ORA_SURNAME:
	      /*---------------------------------------------------*/
	      /* string lists                                      */
	      /*---------------------------------------------------*/
	d26_un4_norm_strlist(&in,&out);
	break;

      case D21_ORA_ISDN_N:
      case D21_ORA_ISDN_S:
      case D21_ORA_NUM_ID:
      case D21_ORA_X121_A:
	      /*---------------------------------------------------*/
	      /* single numeric string                             */
	      /*---------------------------------------------------*/
	d26_un5_norm_string(&in,&out,D2_NUMERIC);
	break;
      case D21_ORA_P_A_F:
	      /*---------------------------------------------------*/
	      /* single T61_string                                 */
	      /*---------------------------------------------------*/
	d26_un5_norm_string(&in,&out,D2_T61);
	break;
      case D21_ORA_P_D_S_N:
      case D21_ORA_T_ID:
      case D21_ORA_P_A_L:
	      /*---------------------------------------------------*/
	      /* single printable string                           */
	      /*---------------------------------------------------*/
	d26_un5_norm_string(&in,&out,D2_PRINTABLE);
	break;
      case D21_ORA_PRES_A:
	      /*---------------------------------------------------*/
	      /* d2_presentation_addr (Psap_string)                */
	      /*---------------------------------------------------*/
      case D21_ORA_T_T:
	      /*---------------------------------------------------*/
	      /* d2_terminal_type (Int)                            */
	      /*---------------------------------------------------*/
	*(D21_OR_address_part *)out = *(D21_OR_address_part *)in;
	memcpy(out + sizeof(D21_OR_address_part),
	       in + sizeof(D21_OR_address_part),
	       ((D21_OR_address_part *)in)->d21_ora_len);
	d26_u02_fill_field(D2_EOS,0,"",
	       ((D21_OR_address_part *)out)->d21_ora_off -
	       ((D21_OR_address_part *)out)->d21_ora_len,
	       out + sizeof(D21_OR_address_part) +
	       ((D21_OR_address_part *)out)->d21_ora_len);
	in += sizeof(D21_OR_address_part) +
	       ((D21_OR_address_part *)in)->d21_ora_off;
	out += ((D21_OR_address_part *)out)->d21_ora_off;
	break;
      default:
	break;
    }
  }

  return((signed16)(out - start_of_out_msg));

}                       /* d26_un1_norm_or_address                    */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_un2_norm_or_name                                  */
/*                                                                    */
/* AUTHOR:      E. Kraemer, AP 11                                     */
/* DATE:        20.04.93                                              */
/*                                                                    */
/* DESCRIPTION: The function reads the OR_name attribute in apdu      */
/*              format from the input-message, norms it and writes it */
/*              in iapl format to the output-memory.                  */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      >= 0        signed16        length of normed OR name          */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_un2_norm_or_name(
    char     *in,       /* IN    -  OR name to norm                   */
    signed16 len,       /* IN    -  length of OR name                 */
    char     *out)      /* OUT   -  normed OR-name                    */

{                       /* d26_un2_norm_or_name                       */

  char *end_of_in_msg    = in + len;
  char *start_of_out_msg = out;

  while(in < end_of_in_msg)
  {
    *(D21_OR_name *)out = *(D21_OR_name *)in;

    switch(((D21_OR_name *)in)->d21_orn_tag)
    {
      case D21_ORN_DN:
	((D21_OR_name *)out)->d21_orn_len = d27_012_remove_blanks(
	  in + sizeof(D21_OR_name),((D21_OR_name *)in)->d21_orn_len,
	  out + sizeof(D21_OR_name));
	d27_007_norm_name(out + sizeof(D21_OR_name),
	  ((D21_OR_name *)out)->d21_orn_len,out + sizeof(D21_OR_name));
	break;
      case D21_ORN_ORA:
	((D21_OR_name *)out)->d21_orn_len = d26_un1_norm_or_address(
	  in + sizeof(D21_OR_name),((D21_OR_name *)in)->d21_orn_len,
	  out + sizeof(D21_OR_name));
	break;
      default:
	break;
    }
    in += sizeof(D21_OR_name) + ((D21_OR_name *)in)->d21_orn_off;
    ((D21_OR_name *)out)->d21_orn_off =
      D2_ALIGN(((D21_OR_name *)out)->d21_orn_len);
    d26_u02_fill_field(D2_EOS,0,"",((D21_OR_name *)out)->d21_orn_off -
      ((D21_OR_name *)out)->d21_orn_len,
      out + sizeof(D21_OR_name) + ((D21_OR_name *)out)->d21_orn_len);
    out += sizeof(D21_OR_name) + ((D21_OR_name *)out)->d21_orn_off;
  }

  return((signed16)(out - start_of_out_msg));

}                       /* d26_un2_norm_or_name                       */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_un3_norm_submit_perm                              */
/*                                                                    */
/* AUTHOR:      E. Kraemer, AP 11                                     */
/* DATE:        20.04.93                                              */
/*                                                                    */
/* DESCRIPTION: The function reads the submit-permission-attribute    */
/*              value from apdu, norms it and writes it to output     */
/*              message.                                              */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      in          char *      submit permission to norm             */
/*      len         unsigned short  length of submit permission       */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      out         char *      normed submit permission              */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      >= 0        unsigned short  length of normed submit permission*/
/*                                                                    */
/* STANDARD-HEADER:     None                                          */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_un3_norm_submit_perm(
    char     *in,       /* IN    -  submit permission to norm         */
    signed16 len,       /* IN    -  length of submit permission       */
    char     *out)      /* OUT   -  normed submit permission          */

{                       /* d26_un3_norm_submit_perm                   */

  char *start_of_out_msg = out;

  *(D21_DL_sub_perm *)out = *(D21_DL_sub_perm *)in;

  switch(((D21_DL_sub_perm *)in)->d21_dlsp_tag)
  {
    case D21_DLSP_INDIV:
    case D21_DLSP_PATT:
    case D21_DLSP_MEM_DL:
       /*-----------------------------------------------------------*/
       /* noorm D2_OR_name                                          */
       /*-----------------------------------------------------------*/
      ((D21_DL_sub_perm *)out)->d21_dlsp_len = d26_un2_norm_or_name(
	in + sizeof(D21_DL_sub_perm),((D21_DL_sub_perm *)in)->d21_dlsp_len,
	out + sizeof(D21_DL_sub_perm));
      break;
    case D21_DLSP_MEM_G:
       /*-----------------------------------------------------------*/
       /* norm D2_name_string                                       */
       /*-----------------------------------------------------------*/
      ((D21_DL_sub_perm *)out)->d21_dlsp_len = d27_012_remove_blanks(
	in + sizeof(D21_DL_sub_perm),((D21_DL_sub_perm *)in)->d21_dlsp_len,
	out + sizeof(D21_DL_sub_perm));
      d27_007_norm_name(out + sizeof(D21_DL_sub_perm),
	((D21_DL_sub_perm *)out)->d21_dlsp_len,out + sizeof(D21_DL_sub_perm));
      break;
    default:
      break;
  }

  in += sizeof(D21_DL_sub_perm) + ((D21_DL_sub_perm *)in)->d21_dlsp_off;
  ((D21_DL_sub_perm *)out)->d21_dlsp_off =
    D2_ALIGN(((D21_DL_sub_perm *)out)->d21_dlsp_len);
  d26_u02_fill_field(D2_EOS,0,"",((D21_DL_sub_perm *)out)->d21_dlsp_off -
    ((D21_DL_sub_perm *)out)->d21_dlsp_len,
    out + sizeof(D21_DL_sub_perm) + ((D21_DL_sub_perm *)out)->d21_dlsp_len);
  out += sizeof(D21_DL_sub_perm) + ((D21_DL_sub_perm *)out)->d21_dlsp_off;

  return((signed16)(out - start_of_out_msg));

}                       /* d26_un3_norm_submit_perm                   */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_un4_norm_strlist                                  */
/*                                                                    */
/* AUTHOR:      E. Kraemer, AP 11                                     */
/* DATE:        20.04.93                                              */
/*                                                                    */
/* DESCRIPTION: The function reads one address-part (type stringlist) */
/*              in apdu format from the input-message, normalizes and */
/*              copies all parts to the output message.               */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_un4_norm_strlist(
    char **in,          /* IN    -  string list to be normalized      */
    char **out)         /* OUT   -  normalized string list            */

{                   /* d26_un4_norm_strlist                           */

  char *outp;

  char *end_of_in_msg_p = *in + sizeof(D21_OR_address_part) +
    ((D21_OR_address_part *)*in)->d21_ora_off;

  *(D21_OR_address_part *)*out = *(D21_OR_address_part *)*in;
  *in += sizeof(D21_OR_address_part);
  outp = *out + sizeof(D21_OR_address_part);

  while (*in < end_of_in_msg_p)
  {
    *(D21_str_list *)outp = *(D21_str_list *)*in;
    if              /* numeric string encountered                     */
      (((D21_str_list *)*in)->d21_strl_rep == D2_NUMERIC)
    {               /* normalize the number                           */
      ((D21_str_list *)outp)->d21_strl_len = d26_u28_norm_numbers(
	*in + sizeof(D21_str_list),((D21_str_list *)*in)->d21_strl_len,
	outp + sizeof(D21_str_list)) + 1;
      outp[sizeof(D21_str_list) + ((D21_str_list *)outp)->d21_strl_len - 1] =
	D2_EOS;
    }
    else
    {               /* remove the blanks and normalize                */
      ((D21_str_list *)outp)->d21_strl_len = d27_012_remove_blanks(
	*in + sizeof(D21_str_list),((D21_str_list *)*in)->d21_strl_len,
	outp + sizeof(D21_str_list));
      switch(((D21_str_list *)*in)->d21_strl_rep)
      {
	case D2_PRINTABLE:
	  d27_031_norm_pr_string((byte *)outp + sizeof(D21_str_list),
	    (byte *)outp + sizeof(D21_str_list),
	    ((D21_str_list *)outp)->d21_strl_len);
	  break;
	case D2_T61:
	  d27_651_norm_T61String((byte *)outp + sizeof(D21_str_list),
	    (byte *)outp + sizeof(D21_str_list),
	    ((D21_str_list *)outp)->d21_strl_len);
	  break;
      }
    }
    *in  += sizeof(D21_str_list) + ((D21_str_list *)*in)->d21_strl_off;
    ((D21_str_list *)outp)->d21_strl_off =
      D2_ALIGN(((D21_str_list *)outp)->d21_strl_len);
    d26_u02_fill_field(D2_EOS,0,"",((D21_str_list *)outp)->d21_strl_off -
      ((D21_str_list *)outp)->d21_strl_len,
      outp + sizeof(D21_str_list) + ((D21_str_list *)outp)->d21_strl_len);
    outp += sizeof(D21_str_list) + ((D21_str_list *)outp)->d21_strl_off;
  }

  ((D21_OR_address_part *)*out)->d21_ora_len =
    ((D21_OR_address_part *)*out)->d21_ora_off =
      (outp - *out) - sizeof(D21_OR_address_part);

  *out = outp;

}                   /* d26_un4_norm_strlist                           */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_un5_norm_string                                   */
/*                                                                    */
/* AUTHOR:      E. Kraemer, AP 11                                     */
/* DATE:        20.04.93                                              */
/*                                                                    */
/* DESCRIPTION: The function reads one address-part (type string)     */
/*              in apdu format from the input-message, normalizes and */
/*              copies it to the output message.                      */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_un5_norm_string(
    char       **in,    /* IN    -  string to be normalized           */
    char       **out,   /* OUT   -  normalized string                 */
    unsigned16 rep)     /* IN    -  representation of input string    */

{                   /* d26_un5_norm_string                            */

  *(D21_OR_address_part *)*out = *(D21_OR_address_part *)*in;

  if                /* numeric string encountered                     */
    (rep == D2_NUMERIC)
  {               /* normalize the number                           */
    ((D21_OR_address_part *)*out)->d21_ora_len = d26_u28_norm_numbers(
	*in + sizeof(D21_OR_address_part),
	((D21_OR_address_part *)*in)->d21_ora_len,
	*out + sizeof(D21_OR_address_part)) + 1;
    (*out)[sizeof(D21_OR_address_part) +
	 ((D21_OR_address_part *)*out)->d21_ora_len - 1] = D2_EOS;
  }
  else
  {               /* remove the blanks and normalize                */
    ((D21_OR_address_part *)*out)->d21_ora_len = d27_012_remove_blanks(
	*in + sizeof(D21_OR_address_part),
	((D21_OR_address_part *)*in)->d21_ora_len,
	*out + sizeof(D21_OR_address_part));
    switch(rep)
    {
      case D2_PRINTABLE:
	d27_031_norm_pr_string((byte *)*out + sizeof(D21_OR_address_part),
	  (byte *)*out + sizeof(D21_OR_address_part),
	  ((D21_OR_address_part *)*out)->d21_ora_len);
	break;
      case D2_T61:
	d27_651_norm_T61String((byte *)*out + sizeof(D21_OR_address_part),
	  (byte *)*out + sizeof(D21_OR_address_part),
	  ((D21_OR_address_part *)*out)->d21_ora_len - 1);
	break;
    }
  }
  *in  += sizeof(D21_OR_address_part) +
      ((D21_OR_address_part *)*in)->d21_ora_off;
  ((D21_OR_address_part *)*out)->d21_ora_off =
      D2_ALIGN(((D21_OR_address_part *)*out)->d21_ora_len);
  d26_u02_fill_field(D2_EOS,0,"",((D21_OR_address_part *)*out)->d21_ora_off -
      ((D21_OR_address_part *)*out)->d21_ora_len,
      *out + sizeof(D21_OR_address_part) +
      ((D21_OR_address_part *)*out)->d21_ora_len);
  *out += sizeof(D21_OR_address_part) +
      ((D21_OR_address_part *)*out)->d21_ora_off;

}                   /* d26_un5_norm_string                            */
