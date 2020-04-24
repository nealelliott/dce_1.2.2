/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21spals.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:47  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:55:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:49  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:44:13  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:36:47  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:41:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:00:54  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:49:02  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:26:28  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  21:38:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:43:46  marrek]
 * 
 * Revision 1.1.4.2  1992/10/06  18:38:59  sommerfeld
 * 	Flush #ident.
 * 	[1992/09/30  21:31:53  sommerfeld]
 * 
 * Revision 1.1.2.4  1992/07/17  19:59:16  weisman
 * 	Checked in at OSF for SNI
 * 	[1992/07/17  19:27:55  weisman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21spals.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:51 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21spals                                            */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 22.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The module computes the lengths of the special      */
/*                attributes (in struct.-format)                      */
/*                                                                    */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <d21dua.h>

/******** LOCAL FUNCTIONS ********/

static signed32 d21_138_crit_struct_len ( char ** );
static signed32 d21_139_item_struct_len ( char ** );

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_131_ttxid_struct_len    (in C-module  d21spals)   */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        22.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of the ttxid-attr.   */
/*              (as structures)                                       */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: pointer to ttxid-message              */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of ttxid-attributes (struct.)                */
/*                -1 in case of error (i.e. first type != D2_TERM     */
/*                                     or other typenot in type-list  */
/*                                     or str_len <= 0              ) */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_131_ttxid_struct_len(
  char * in_msg)
{
signed32 return_value;
D23_av_value * in_av = (D23_av_value *)in_msg;
char *in_msg_p = in_msg+D23_S_ATV;
char *in_msg_endp = in_msg+D23_S_ATV+in_av->d23_avlen;
d21_ttxid    *ttxid_struct;
signed16 str_len;

return_value = sizeof (D2_ttxid);

/*----------------------------------------------------------------*/
/* incremenet return-value for each ttxid-entry                   */
/*----------------------------------------------------------------*/

ttxid_struct = (d21_ttxid *) in_msg_p;
in_msg_p += sizeof(d21_ttxid);

str_len = ttxid_struct->d2_str_len;
if (ttxid_struct->d2_str_type != D2_TERM  ||
    ttxid_struct->d2_offset == 0           )
   return (-1);
return_value += D2_ALIGN(str_len);
in_msg_p += ttxid_struct->d2_offset;

while ( in_msg_p < in_msg_endp )
  {
     ttxid_struct = (d21_ttxid *) in_msg_p;
     in_msg_p += sizeof(d21_ttxid);

     str_len = ttxid_struct->d2_str_len;

     if ( str_len == 0           ||
	 (ttxid_struct->d2_str_type != D2_CTRL  &&
	  ttxid_struct->d2_str_type != D2_GRPH  &&
	  ttxid_struct->d2_str_type != D2_MISC  &&
	  ttxid_struct->d2_str_type != D2_PAGE  &&
	  ttxid_struct->d2_str_type != D2_PRIV    )  )
	return (-1);

     return_value += D2_ALIGN(str_len);
     in_msg_p += ttxid_struct->d2_offset;
  }

return (D2_ALIGN(return_value));

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_132_guide_struct_len     (in C-module  d21spals)  */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        08.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of search guide      */
/*              (as structures)                                       */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: pointer to guide-message              */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of search-guide-attributes (struct.)         */
/*                -1 in case of error (i.e. d2_obj.d2_typ_len < 0     */
/*                                     or d118_return = -1          ) */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_132_guide_struct_len(
  char * in_msg)
{
signed32 return_value;

d21_obj_len * object_len;
signed32 crit_length;
char      * in_msg_p = in_msg + D23_S_ATV;
char      ** inout;

return_value = sizeof (D2_guide) - sizeof (D2_criteria);

/*--------------------------------------------------------------*/
/* length of  object-identifier                                 */
/*--------------------------------------------------------------*/
object_len = (d21_obj_len *) in_msg_p;
in_msg_p += sizeof (d21_obj_len);

if (object_len->d21_length > 0 )
   {
      return_value += D2_ALIGN(object_len->d21_length);
      in_msg_p += object_len->d21_offset;
   }

/*--------------------------------------------------------------*/
/* length of  criteria                                          */
/*--------------------------------------------------------------*/
inout = &in_msg_p;
crit_length =  d21_138_crit_struct_len (inout);

if (crit_length > 0)
   return_value += D2_ALIGN(crit_length);
else
   return (-1);

return (D2_ALIGN(return_value));
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_138_crit_struct_len      (in C-module  d21spals)  */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        12.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of a criterion       */
/*              (as structures)                                       */
/*                                                                    */
/* INPUT PARAMETERS:     inout: pointo criteria-string                */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of criterion (struct.)                       */
/*                -1 in case of error (i.e. choice not in list        */
/*                                     or count <= 0                  */
/*                                                                    */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d21_138_crit_struct_len(
  char ** inout)
{
signed32 return_value;

d21_crdesc * critdesc;
signed32 nb_crits, crit_length, i;

return_value = sizeof (D2_criteria);

/*--------------------------------------------------------------*/
/* length of  choice und count                                  */
/*--------------------------------------------------------------*/
critdesc = (d21_crdesc *) *inout;
*inout += sizeof (d21_crdesc);

/*--------------------------------------------------------------*/
/* length of arguments                                          */
/*--------------------------------------------------------------*/
switch (critdesc->d21_choice)
   {
       case D2_ITEM:
	       crit_length = d21_139_item_struct_len (inout);
	       if (crit_length <= 0)
		  return (-1);
	       else
		  return_value += D2_ALIGN(crit_length);
	       break;
       case D2_NOT:
	       crit_length = d21_138_crit_struct_len (inout);
	       if (crit_length <= 0)
		  return (-1);
	       else
		  return_value += D2_ALIGN(crit_length);
	       break;
       case D2_AND:
       case D2_OR:
	       return_value += sizeof (D2_crit_set);
	       nb_crits = critdesc->d21_count;
	       if (nb_crits <= 0)
		  return (-1);
	       for (i=0; i<nb_crits; i++)
		 {
		   crit_length = d21_138_crit_struct_len (inout);
		   if (crit_length <= 0)
		      return (-1);
		   else
		      return_value += D2_ALIGN(crit_length);
		 }
	       break;
       default:
	    return (-1);
   }

return (D2_ALIGN(return_value));
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_139_item_struct_len      (in C-module  d21spals)  */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        12.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of an item           */
/*              (as structures)                                       */
/*                                                                    */
/* INPUT PARAMETERS:    inout: ptr to ptr to criteria-item-string     */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN V6LUE : length of item (struct.)                            */
/*                -1 in case of error(i.e. matching code not in list) */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d21_139_item_struct_len(
  char ** inout)
{
signed32 return_value;
d21_itdesc * itemdesc;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
return_value = sizeof (D2_crit_item);

itemdesc = (d21_itdesc *) *inout;
*inout += sizeof (d21_itdesc);

if (itemdesc->d21_match != D2_EQUAL  &&
    itemdesc->d21_match != D2_SUBSTRINGS &&
    itemdesc->d21_match != D2_GTEQ &&
    itemdesc->d21_match != D2_LTEQ &&
    itemdesc->d21_match != D2_PRESENT &&
    itemdesc->d21_match != D2_APPROX_MATCH )
  return (-1);

if (itemdesc->d21_length == 0)
   return (-1);

return_value += D2_ALIGN(itemdesc->d21_length);
*inout += itemdesc->d21_offset;


return (D2_ALIGN(return_value));
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_133_T61seq_struct_len    (in C-module  d21spals)  */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        05.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of t61_seq. attr.    */
/*              (as structures)                                       */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: pointer to T61_seq-message            */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of t61_seq-attributes (structa.)             */
/*                -1 in case of error (i.e. d2_nb_item <= 0           */
/*                                          or d2_size <= 0 )         */
/*exoff ***************************************************************/

signed32 d21_133_T61seq_struct_len(
  char * in_msg)
{
signed32            return_value, nb_nulls=0;
signed16 		i;
D23_av_value * in_av = (D23_av_value *) in_msg;
char        *in_msg_p = in_msg + D23_S_ATV;

return_value = sizeof (D2_T61_seq);

for (i=0; i<in_av->d23_avlen; i++)
    if (*(in_msg_p+i)=='\0')
	nb_nulls ++;

return_value += (nb_nulls+1) * sizeof (D2_str);

return_value +=  D2_ALIGN(in_av->d23_avlen) + (nb_nulls+1)*4*sizeof(char);

return (D2_ALIGN(return_value));
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_134_telexnr_struct_len   (in C-module  d21spals)  */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        05.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of telex-nr. attr.   */
/*              (as structures)                                       */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: pointer to telex_number-message       */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of telexnr-attributes (struct.)              */
/*                -1 in case of error (i.e. d2_telex_nr.d2_size <= 0) */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_134_telexnr_struct_len(
  char * in_msg)
{
signed32 return_value;
D23_av_value * in_av = (D23_av_value *) in_msg;

return_value = sizeof (D2_telex_number);

return_value +=  D2_ALIGN(in_av->d23_avlen);

return (D2_ALIGN(return_value));
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_135_faxnr_struct_len     (in C-module  d21spals)  */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        05.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of faxnr-attributes  */
/*              (as structures)                                       */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: pointer to faxnr-message              */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of faxnr-attributes (struct.)                */
/*                -1 in case of error (i.e. d2_size < 0)              */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_135_faxnr_struct_len(
  char * in_msg)
{
signed32 return_value;

d21_faxnr * faxnr_struct;
char * in_msg_p = in_msg + D23_S_ATV;

return_value = sizeof (D2_faxnumber);

faxnr_struct = (d21_faxnr *) in_msg_p;

if (faxnr_struct->d2_str_len == 0)
   return (-1);

if (faxnr_struct->d2_offset == -1 )
   return_value += D2_ALIGN(faxnr_struct->d2_str_len);
else
   return_value += faxnr_struct->d2_offset;


return (D2_ALIGN(return_value));
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_136_post_ad_struct_len   (in C-module  d21spals)  */
/*                                                                    */
/* AUTHOR:      Jochen Keutel                                         */
/* DATE:        7.4.93                                                */
/*                                                                    */
/* DESCRIPTION: The function computes the length of postal address    */
/*		attributes (as structures).                           */
/*		The function does not check if the correct	      */
/*		representations in APDU are used.		      */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: pointer to post_ad-message            */
/*                                                                    */
/* RETURN VALUE : length of post_ad-attributes (struct.)              */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_136_post_ad_struct_len(
  char * in_msg)
{
signed16 i;
signed32 ret_value;
signed32 nb_nulls = 0;
D23_av_value * in_av = (D23_av_value *)in_msg;

ret_value = sizeof(D2_str_list);

for (i=0; i<in_av->d23_avlen; i++)
    if (*(in_msg + i)=='\0')
	nb_nulls ++;

ret_value += nb_nulls * sizeof(D2_str_list);
ret_value += D2_ALIGN(in_av->d23_avlen) + (nb_nulls+1)*4*sizeof(char);
return (ret_value);
}

