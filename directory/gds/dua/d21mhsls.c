/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21mhsls.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:01  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:16  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:41:42  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:34:15  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:27:12  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:57:24  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:48:09  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:25:36  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:34:15  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:40:14  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  19:40:18  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:49:25  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:37:00  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21mhsls.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:16 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21mhsls.c                                          */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 22.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The module computes the lengths of the mhs special  */
/*                attributes (in iapl-format)                         */
/*                                                                    */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <d21dua.h>

/******** LOCAL FUNCTIONS ********/

static signed32 d21_236_or_name_len ( char * , signed16 );
static signed32 d21_237_or_addr_len ( char * , signed16 );
static signed32 d21_238_strlist_len( char * , signed16 , signed16 );

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_231_or_addr_struct_len   (in C-module  d21mhsls)  */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        15.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads the OR-address in apdu-format and  */
/*              computes the length of the corresponding attribute    */
/*              in iapl-format.                                       */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: pointer to OR_addr-message,           */
/*                              preceeded by a struct D23_av_value    */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of OR_addr-attribute in iapl-format          */
/*                D2_ERROR in case of error                           */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_231_or_addr_struct_len(
  char * in_msg)
{
signed32 structlen;
D23_av_value * in_av = (D23_av_value *)in_msg;
char * in_msg_p = in_msg + D23_S_ATV;

if ((structlen=d21_237_or_addr_len (in_msg_p,in_av->d23_avlen)) == D2_ERROR)
   return (D2_ERROR);
else
   return (D2_ALIGN(structlen));
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_232_or_name_struct_len   (in C-module  d21mhsls)  */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        15.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads the OR-name in apdu-format and     */
/*              computes the length of the corresponding attribute    */
/*              in iapl-format.                                       */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: pointer to OR_name-message,           */
/*                              preceeded by a struct D23_av_value    */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of OR_name-attribute in iapl-format          */
/*                D2_ERROR in case of error                           */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_232_or_name_struct_len(
  char * in_msg)
{
signed32 structlen;
D23_av_value * in_av = (D23_av_value *)in_msg;
char * in_msg_p = in_msg + D23_S_ATV;

if ((structlen=d21_236_or_name_len (in_msg_p,in_av->d23_avlen)) == D2_ERROR)
   return (D2_ERROR);
else
   return (D2_ALIGN(structlen));
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_233_sub_perm_struct_len  (in C-module  d21mhsls)  */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        15.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads the subm-perm in apdu-format and   */
/*              computes the length of the corresponding attribute    */
/*              in iapl-format.                                       */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: pointer to submit-permission-message  */
/*                              (structure D21_DL_sub_perm, followed  */
/*                               by an OR_name or by a ds-name),      */
/*                              preceeded by a struct D23_av_value    */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of submit-perm-attribute in iapl-format      */
/*                     (size of structure D2_dl_sub_perm and size     */
/*                      of OR-name rsp. name-string)                  */
/*                D2_ERROR in case of error, that is wrong name-struct*/
/*                      or wrong length-value or NULL-String          */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_233_sub_perm_struct_len(
  char * in_msg)
{
signed32 structlen;
char * in_msg_p = in_msg + D23_S_ATV;

D21_DL_sub_perm   *sub_perm_p = (D21_DL_sub_perm *) in_msg_p;

signed32 namelen;

structlen = sizeof (D2_dl_submit_perm);
in_msg_p += sizeof (D21_DL_sub_perm);

switch (sub_perm_p->d21_dlsp_tag)
   {
       case D21_DLSP_INDIV:
       case D21_DLSP_MEM_DL:
       case D21_DLSP_PATT:
	  if ((namelen =
		  d21_236_or_name_len (in_msg_p, sub_perm_p->d21_dlsp_len))
								 == D2_ERROR)
	     return (D2_ERROR);
	  else
	     structlen += D2_ALIGN(namelen);
	  break;
       case D21_DLSP_MEM_G:
	  namelen = sub_perm_p->d21_dlsp_len;
	  if (namelen <= 1)
	     return (D2_ERROR);
	  else
	     structlen += D2_ALIGN(namelen);
	  break;
       default:
	  return (D2_ERROR);
   }

return (D2_ALIGN(structlen));
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_236_or_name_len          (in C-module  d21mhsls)  */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        15.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads the OR-name in apdu-format and     */
/*              computes the length of the corresponding attribute    */
/*              in iapl-format.                                       */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: pointer to OR_name-message            */
/*                              (one or two structures D21_OR_name,   */
/*                              each followed by a name-string resp.  */
/*                              by an OR-address)                     */
/*                      msg_len: length of input-message              */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of OR_name-attribute in iapl-format          */
/*                    (size of structure D2_OR_name, incremented      */
/*                     by size of name-string and/or OR-address)      */
/*                D2_ERROR in case of error, that is more than two    */
/*                    structures D21_OR_name or wrong tag or tag      */
/*                    appears twice or wrong length-value or string   */
/*                    has length 0 or OR-addr-tag doesn't occur.      */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d21_236_or_name_len(
  char  * in_msg,
  signed16 msg_len)
{
signed32 structlen;
char * end_of_in_msg = in_msg + msg_len;

D21_OR_name *D21_OR_name_p;
signed32          addrlen;
signed32          addr_count=0, name_count=0;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
structlen = sizeof (D2_OR_name);

while (in_msg < end_of_in_msg)
   {
      D21_OR_name_p = (D21_OR_name *) in_msg;
      in_msg += sizeof (D21_OR_name);

      switch (D21_OR_name_p->d21_orn_tag)
	 {
	    case D21_ORN_DN:
	      if (addr_count > 0)     /* name must occur before(!) addr */
		 return (D2_ERROR);
	      name_count += 1;
	      structlen +=D2_ALIGN(D21_OR_name_p->d21_orn_len);
	      in_msg += D21_OR_name_p->d21_orn_off;
	      break;
	    case D21_ORN_ORA:
	      addr_count += 1;
	      if ((addrlen =
		d21_237_or_addr_len (in_msg, D21_OR_name_p->d21_orn_len))
								== D2_ERROR)
		 return (D2_ERROR);
	      structlen += D2_ALIGN(addrlen);
	      in_msg += D21_OR_name_p->d21_orn_off;
	      break;
	     default:
		return (D2_ERROR);
	 }

   }

if (name_count > 1  ||  addr_count != 1)
   return (D2_ERROR);

return (D2_ALIGN(structlen));
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_237_or_addr_len          (in C-module  d21mhsls)  */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        15.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads the OR-address in apdu-format and  */
/*              computes the length of the corresponding attribute    */
/*              in iapl-format.                                       */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: pointer to OR_addr-message            */
/*                              (that is one or more structures       */
/*                              D21_addr_part, each followed by a     */
/*                              stringlist or a string)               */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of OR_addr-attribute in iapl-format          */
/*                    (=size of struct D2_OR_address and all          */
/*                     structures D2_str_list and all strings)        */
/*                D2_ERROR in case of error, that is wrong address-tag*/
/*                     or error in string-list or wrong value for     */
/*                     string-length or string has length 0.          */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d21_237_or_addr_len(
  char * in_msg,
  signed16 msg_len)
{
signed32 structlen;
char * end_of_in_msg = in_msg + msg_len;

D21_OR_address_part   *addr_part_p;

char                 *string_p, *end_of_strings;
signed32                    string_len, strlist_len;

structlen = sizeof (D2_OR_address);

while (in_msg < end_of_in_msg)
   {
      addr_part_p = (D21_OR_address_part *) in_msg;
      in_msg += sizeof (D21_OR_address_part);
      switch (addr_part_p->d21_ora_tag)
	 {
	    case D21_ORA_ADMD_N:
	       /*---------------------------------------------------*/
	       /* type D2_str_list (type 0)                         */
	       /*---------------------------------------------------*/
	       if ( (strlist_len = d21_238_strlist_len
		   (in_msg, addr_part_p->d21_ora_len, 0)) == D2_ERROR )
		  return (D2_ERROR);
	       structlen += strlist_len;
	       in_msg += addr_part_p->d21_ora_off;
	       break;
	    case D21_ORA_CTRY_N:
	    case D21_ORA_P_CODE:
	    case D21_ORA_P_CTRY:
	    case D21_ORA_PRMD_N:
	       /*---------------------------------------------------*/
	       /* type D2_str_list (type 1)                         */
	       /*---------------------------------------------------*/
	       if ( (strlist_len = d21_238_strlist_len
		   (in_msg, addr_part_p->d21_ora_len, 1)) == D2_ERROR )
		  return (D2_ERROR);
	       structlen += strlist_len;
	       in_msg += addr_part_p->d21_ora_off;
	       break;
	    case D21_ORA_COM_N:
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
	    case D21_ORA_SURNAME:
	       /*---------------------------------------------------*/
	       /* type D2_str_list (type 2)                         */
	       /*---------------------------------------------------*/
	       if ( (strlist_len = d21_238_strlist_len
		   (in_msg, addr_part_p->d21_ora_len, 2)) == D2_ERROR )
		  return (D2_ERROR);
	       structlen += strlist_len;
	       in_msg += addr_part_p->d21_ora_off;
	       break;
	    case D21_ORA_P_A_L:
	       /*---------------------------------------------------*/
	       /* sequence of T61-strings                           */
	       /*---------------------------------------------------*/
	       string_p = in_msg;
	       end_of_strings = in_msg + addr_part_p->d21_ora_len;
	       while (string_p < end_of_strings)
		  {
		     string_len = strlen ((char *)string_p) + 1;
		     string_p += string_len;
		     structlen += sizeof (D2_str_list) + D2_ALIGN(string_len);
		  }
	       in_msg += addr_part_p->d21_ora_off;
	       break;

	    case D21_ORA_P_D_S_N:
	    case D21_ORA_T_ID:
	    case D21_ORA_ISDN_N:
	    case D21_ORA_ISDN_S:
	    case D21_ORA_NUM_ID:
	    case D21_ORA_X121_A:
	    case D21_ORA_P_A_F:
	    case D21_ORA_PRES_A:
	       /*-------------------------------------------------------*/
	       /* type one string					*/
	       /*-------------------------------------------------------*/
	       structlen += addr_part_p->d21_ora_off;
	       in_msg += addr_part_p->d21_ora_off;
	       break;

	    case D21_ORA_T_T:
	       /*---------------------------------------------------*/
	       /* type Int                                          */
	       /*---------------------------------------------------*/
	       if ( *((signed32 *)in_msg) < D2_MHS_T_T_LBOUND  ||
		    *((signed32 *)in_msg) > D2_MHS_T_T_UBOUND    )
		  return (D2_ERROR);
	       structlen += addr_part_p->d21_ora_off;
	       in_msg += addr_part_p->d21_ora_off;
	       break;

	    default:
	       return (D2_ERROR);
	 }
   }


return (D2_ALIGN(structlen));
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_238_strlist_len     (in C-module  d21mhslm)       */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        13.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function reads a list of structures D21_str_list, */
/*              each followed by a string, and computes the length of */
/*              the corresponding stringlist in apdu-format           */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to first structure D21_str_list   */
/*                      len: length of input-message                  */
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
/* RETURN VALUE :  length of stringlist in apdu-format                */
/*                 D2_ERROR in case of error (that is if the number   */
/*                          of structures D21_str_list exceeds the    */
/*                          limit given by "type"  or a representation*/
/*                          appears too often or stringlength 0)      */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d21_238_strlist_len(
  char * in,
  signed16 len,
  signed16 type)
{
signed32       structlen = 0;
signed32       str_count=0, prt_count=0, num_count=0, t61_count=0;
signed32       max_nb_str, max_nb_prt, max_nb_num, max_nb_t61;

D21_str_list  * strlist_p;
char         * end_of_stringlist = in + len;

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
/* add lengths of all structures D2_str_list and strings          */
/*----------------------------------------------------------------*/
while (in < end_of_stringlist)
   {
      strlist_p = (D21_str_list *) in;

      /*----------------------------------------------------------*/
      /* assign variables for number of strings                   */
      /*----------------------------------------------------------*/
      str_count ++;
      switch (strlist_p -> d21_strl_rep)
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

      structlen += sizeof (D2_str_list) + strlist_p -> d21_strl_off;
      in += sizeof (D21_str_list) + strlist_p -> d21_strl_off;
   }


/*----------------------------------------------------------------*/
/* test number of strings                                         */
/*----------------------------------------------------------------*/
if (str_count > max_nb_str     ||
    prt_count > max_nb_prt     ||
    num_count > max_nb_num     ||
    t61_count > max_nb_t61        )
   return (D2_ERROR);

return (D2_ALIGN(structlen));
}

