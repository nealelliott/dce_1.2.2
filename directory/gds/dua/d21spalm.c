/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21spalm.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:55:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:47  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:43:58  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:36:34  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:40:25  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:00:30  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:48:58  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:26:23  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:38:20  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:43:23  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:53:10  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:22:03  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:37:15  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:38:39  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21spalm.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:48 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21spalm                                            */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 19.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The module computes the lengths of the special      */
/*                attributes (in message-format)                      */
/*                                                                    */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <d21dua.h>

/******** LOCAL FUNCTIONS ********/

static signed32  d21_118_crit_msglen ( D2_criteria * );
static signed32  d21_119_item_msglen ( D2_crit_item * );

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_111_ttxid_msglen    (in C-module  d21spalm)       */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        19.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of the ttxid-attr.   */
/*              (in message-format)                                   */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to ttxid-attributes               */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE :  length of ttxid-message                            */
/*                 -1 in case of error (i.e. d2_terminal.d2_size <=0  */
/*                                      or d2_controls.d2_size,...<0) */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_111_ttxid_msglen(
  D2_ttxid * in)
{
signed32 return_value = 0;

D2_str * D2_str_p;
signed16 i_size;
signed16 i_count;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
D2_str_p = (D2_str *) in;

/*----------------------------------------------------------------*/
/* for each ttxid-entry incrementation of return-value by         */
/* sizeof (ttxid-struct) + stringlength                           */
/* error-handling: d2_terminal.d2_size <= 0 or d2_xxx.d2_size < 0 */
/*                 => return_value -1                             */
/*----------------------------------------------------------------*/
if (D2_str_p -> d2_size <= 0)
   return (-1);

for (i_count=0; i_count < 6; i_count++)
  {
    i_size = D2_str_p -> d2_size;

    if (i_size > 0)
	return_value += sizeof (d21_ttxid) + D2_ALIGN(i_size);

    if (i_size < 0)
	return (-1);

    D2_str_p++;
  }

return (return_value);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_112_guide_msglen     (in C-module  d21spalm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        08.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of search guide      */
/*              (in message-format)                                   */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to guide-attributes               */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of search-guide-message                      */
/*                -1 in case of error (i.e. d2_obj_defined != TRUE,*/
/*                                                           FALSE */
/*                                     or d2_obj.d2_typ_len < 0       */
/*                                     or d118_return = -1          ) */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_112_guide_msglen(
  D2_guide * in)
{
signed32 return_value = 0;

D2_criteria   *criteria_p;
signed32             object_offset;
signed32                i_size, crit_length;

/*--------------------------------------------------------------*/
/* length of  object-identifier                                 */
/*--------------------------------------------------------------*/
return_value = sizeof (d21_obj_len);
if (in -> d2_obj_defined == TRUE)
   {
      i_size = in -> d2_obj.d2_typ_len;
      if (i_size < 0)
	  return (-1);
      object_offset = D2_ALIGN (i_size);
      return_value += object_offset;
   }
else
   if (in -> d2_obj_defined != FALSE)
      return (-1);

/*--------------------------------------------------------------*/
/* length of  criteria                                          */
/*--------------------------------------------------------------*/
criteria_p = &(in -> d2_crit);
crit_length = d21_118_crit_msglen (criteria_p);

if (crit_length > 0)
   return_value += crit_length;
else
   return (-1);

return (return_value);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_118_crit_msglen      (in C-module  d21spalm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        12.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of a criterion       */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to criteria-attributes            */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of criterion                                 */
/*                -1 in case of error(i.e. D2_criteria.d2_c_mode not  */
/*                                                          in list   */
/*                                    or D2_crit_set.and_or_nr <= 0   */
/*                                    or d119_return = -1           ) */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d21_118_crit_msglen(
  D2_criteria * in)
{
signed32 return_value = 0;

D2_criteria   * criteria_p;
D2_crit_item       * crit_item_p;
signed32 nb_crits, crit_length, i;

/*--------------------------------------------------------------*/
/* length of  choice  +  count                                  */
/*--------------------------------------------------------------*/
return_value += sizeof (d21_crdesc);

/*--------------------------------------------------------------*/
/* length of arguments                                          */
/*--------------------------------------------------------------*/
switch (in -> d2_c_mode)
   {
       case D2_ITEM:
	       if (in->d2_crit_val.type == NULL) return (D2_ERROR);
	       crit_item_p = in -> d2_crit_val.type;
	       crit_length = d21_119_item_msglen (crit_item_p);
	       if (crit_length <= 0)
		  return (D2_ERROR);
	       else
		  return_value += crit_length;
	       break;
       case D2_NOT:
	       if (in->d2_crit_val.not == NULL) return (D2_ERROR);
	       criteria_p = in -> d2_crit_val.not;
	       crit_length = d21_118_crit_msglen (criteria_p);
	       if (crit_length <= 0)
		  return (D2_ERROR);
	       else
		  return_value += crit_length;
	       break;
       case D2_AND:
       case D2_OR:
	       if (in->d2_crit_val.d2_ci_and_or == NULL ) return (D2_ERROR);
	       criteria_p = in -> d2_crit_val.d2_ci_and_or -> and_or;
	       nb_crits = in -> d2_crit_val.d2_ci_and_or -> and_or_nr;
	       if (nb_crits <= 0)
		  return (D2_ERROR);
	       for (i=0; i<nb_crits; i++)
		 {
	           if (criteria_p == NULL) return (D2_ERROR);
		   crit_length = d21_118_crit_msglen (criteria_p);
		   if (crit_length <= 0)
		      return (D2_ERROR);
		   else
		      return_value += crit_length;
		   criteria_p++;
		 }
	       break;
       default:
	    return (D2_ERROR);
   }

return (return_value);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_119_item_msglen      (in C-module  d21spalm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        12.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of an item           */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to criteria-item-attributes       */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of item                                      */
/*                -1 in case of error(i.e. D2_crit_item.d2_c_mode not */
/*                                                          in list  )*/
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d21_119_item_msglen(
  D2_crit_item * in)
{
signed32 return_value = 0;

signed32            i_size, ci_mode;

i_size = in->d2_ci_type.d2_typ_len;
if (i_size <= 0)
   return (-1);
ci_mode = in -> d2_ci_mode;
if (ci_mode != D2_EQUAL  &&
    ci_mode != D2_SUBSTRINGS &&
    ci_mode != D2_GTEQ &&
    ci_mode != D2_LTEQ &&
    ci_mode != D2_PRESENT &&
    ci_mode != D2_APPROX_MATCH )
  return (-1);

/*--------------------------------------------------------------*/
/* length of  matching rule, filler, offset, length       +     */
/* length of  object-identifier-string                          */
/*--------------------------------------------------------------*/
return_value += sizeof (d21_itdesc) + D2_ALIGN(i_size);

return (return_value);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_113_T61seq_msglen    (in C-module  d21spalm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        05.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of t61_seq. attr.    */
/*              (in message-format)                                   */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to T61_seq-attributes             */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of t61_seq-message                           */
/*                -1 in case of error (i.e. d2_nb_item <= 0           */
/*                                          or d2_size <= 0 )         */
/*exoff ***************************************************************/

signed32 d21_113_T61seq_msglen(
  D2_T61_seq * in)
{

signed32 return_value = 0;
D2_str * D2_str_p;
signed32 i_size, nb_item, i;

D2_str_p = in-> d2_t61_item;

/*---------------------------------------------------------*/
/* lengths of    item-strings (incl \0)                    */
/*---------------------------------------------------------*/
nb_item = in -> d2_nb_item;
if (nb_item <= 0)
   return (-1);
for (i=0; i < nb_item; i++)
   {
       i_size = D2_str_p -> d2_size;
       if (i_size <= 0)
	  return (-1);
       else
	  return_value += i_size + 1;
       D2_str_p++;
   }

return_value -= 1;             /* subtract last null-byte */

return (return_value);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_114_telexnr_msglen   (in C-module  d21spalm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        05.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of telex-nr. attr.   */
/*              (in message-format)                                   */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to telex_number-attributes        */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of telexnr-message                           */
/*                -1 in case of error (i.e. d2_telex_nr.d2_size <= 0) */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_114_telexnr_msglen(
  D2_telex_number * in)
{
signed32 return_value = 0;

/*---------------------------------------------------*/
/* lengths of  telexnr, ccode and answer (incl \0)   */
/*---------------------------------------------------*/
if (in -> d2_t_number.d2_size <= 0 ||
    in -> d2_c_code.d2_size <= 0   ||
    in -> d2_answer.d2_size <= 0     )
   return (-1);
return_value += in -> d2_t_number.d2_size + 1;
return_value += in -> d2_c_code.d2_size + 1;
return_value += in -> d2_answer.d2_size;

return (return_value);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_115_faxnr_msglen     (in C-module  d21spalm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        05.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function computes the length of faxnr-attributes  */
/*              (in message-format)                                   */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to faxnr-attributes               */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE : length of faxnr-message                             */
/*                -1 in case of error (i.e. d2_size < 0               */
/*                                     or d2_par_defined != TRUE,  */
/*                                                          FALSE) */
/*exoff ***************************************************************/

signed32 d21_115_faxnr_msglen(
  D2_faxnumber * in)
{
signed32 return_value = 0;

signed32            i_size;
signed16         d21_offset;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
i_size = in -> d2_tel_nr.d2_size;
if (i_size < 0)
   return (-1);
d21_offset = D2_ALIGN(i_size);

/*------------------------------------------*/
/* length of  fax-struct + octet-string,    */
/* rsp. of  fax-struct + offset + parm      */
/*------------------------------------------*/

if (in -> d2_par_defined != FALSE  &&
    in -> d2_par_defined != TRUE     )
   return (-1);

if (in -> d2_par_defined == FALSE)
      return_value =  sizeof (d21_faxnr) + i_size;
else
      return_value =  sizeof (d21_faxnr) + d21_offset + sizeof(signed32);

return (return_value);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_116_post_ad_msglen     (in C-module  d21spalm)    */
/*                                                                    */
/* AUTHOR:      Jochen Keutel                                         */
/* DATE:        7.4.93                                                */
/*                                                                    */
/* DESCRIPTION: The function computes the length of post_ad-attributes*/
/*              (in message-format)                                   */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to post_ad-attributes             */
/*                                                                    */
/* RETURN VALUE : length of post_ad-message                           */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d21_116_post_ad_msglen(
  D2_str_list * in)
{
signed32 ret_value;

ret_value = in->d2_str.d2_size + 2; /* string + rep. + \0 */

while (in->d2_next != NULL)
  {
  in = in->d2_next;
  ret_value += in->d2_str.d2_size + 2; /* string + rep. + \0 */
  }

return(ret_value - 1);
}
