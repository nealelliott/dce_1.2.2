/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21spafm.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:41  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:44  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:54:14  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:33:26  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:43:44  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:36:16  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:38:52  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:00:04  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:17:02  marrek
 * 	Fixes for OT 6570
 * 	insert <pthread.h>
 * 	[1993/02/02  09:27:51  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:48:53  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:26:18  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:37:47  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:43:00  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  20:31:50  melman
 * 	Fixes CR 3975 client to client/server configs
 * 	[1992/07/01  20:27:53  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:35:14  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:38:33  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21spafm.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:46 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21spafm                                            */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 21.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The module converts the special attributes (ttxid,  */
/*                t61_seq, telex-number, fax-number and search guide) */
/*                from message format to structure format             */
/*                                                                    */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <d21dua.h>

/******** LOCAL FUNCTIONS ********/

static void  d21_129_item_fr_ipc ( char ** , char ** );

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_121_ttxid_fr_ipc    (in C-module  d21spafm)       */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        26.03.91                                              */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION: The function converts the ttxid attributes from       */
/*              string format to structure format                     */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: ptr-ptr to ttxid-message              */
/*                              (preceded by structure D23_av_value)  */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr-ptr to ttxid-attributes (struct.)    */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d21_121_ttxid_fr_ipc(
  char ** in_msg,
  char ** out,
  Bool chng_out)   /* Flag to indicate that out ptr is to be changed or not */
{
D23_av_value   *in_av = (D23_av_value *)*in_msg;
char          *in_msg_p = *in_msg + D23_S_ATV;
char          *in_msg_endp = *in_msg + D23_S_ATV + in_av->d23_avlen;
char          *out_string_p;
d21_ttxid      *ttxid_struct;
D2_ttxid       *ttxid;
D2_str         * str_ptr;
signed16         str_len;

/*----------------------------------------------------------------*/
/* Set D2_ttxid to NULL-values                                    */
/*----------------------------------------------------------------*/
ttxid = (D2_ttxid *) *out;
ttxid->d2_terminal.d2_size = 0;
ttxid->d2_controls.d2_size = 0;
ttxid->d2_graphics.d2_size = 0;
ttxid->d2_miscel.d2_size = 0;
ttxid->d2_pages.d2_size = 0;
ttxid->d2_privates.d2_size = 0;
/*----------------------------------------------------------------*/
/* assign ttxid-entries to output-string                          */
/*----------------------------------------------------------------*/
out_string_p = *out + sizeof (D2_ttxid);

while ( in_msg_p < in_msg_endp )
  {
     ttxid_struct = (d21_ttxid *)in_msg_p;
     in_msg_p += sizeof(d21_ttxid);

     /*-----------------------------------------------------------*/
     /* position of D2_str_p                                      */
     /*-----------------------------------------------------------*/
     switch (ttxid_struct->d2_str_type)
       {
	  case D2_TERM:
		str_ptr = &ttxid->d2_terminal;
	       break;
	  case D2_CTRL:
		str_ptr = &ttxid->d2_controls;
	       break;
	  case D2_GRPH:
		str_ptr = &ttxid->d2_graphics;
	       break;
	  case D2_MISC:
		str_ptr = &ttxid->d2_miscel;
	       break;
	  case D2_PAGE:
		str_ptr = &ttxid->d2_pages;
	       break;
	  case D2_PRIV:
		str_ptr = &ttxid->d2_privates;
	       break;
	  default:
	       break;
       }

     /*-----------------------------------------------------------*/
     /* assignment of d2_size and d2_value                        */
     /*-----------------------------------------------------------*/
     str_len = ttxid_struct->d2_str_len;
     str_ptr->d2_size = str_len;
     str_ptr->d2_value = (unsigned char *)out_string_p;
     (void) memcpy (out_string_p,in_msg_p,str_len);

     out_string_p += D2_ALIGN(str_len);
     in_msg_p += ttxid_struct->d2_offset;
  }

if (chng_out == TRUE)
  *out = out_string_p;
*in_msg = in_msg_endp;
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_122_guide_fr_ipc     (in C-module  d21spafm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        02.04.91                                              */
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
/*exoff ***************************************************************/

void d21_122_guide_fr_ipc(
  char ** in_msg,
  char ** out,
  Bool chng_out)   /* Flag to indicate that out ptr is to be changed or not */
{
char          *in_msg_p;
char          *out_p = *out;
char          *mem_ptr;
D23_av_value   *in_av = (D23_av_value *) *in_msg;
d21_obj_len    *d21_object_len;
D2_guide       * guide;
signed16            i_size, i_offset;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
in_msg_p = *in_msg + D23_S_ATV;             /* Startaddr. (read)      */
guide = (D2_guide *) *out;
/*--------------------------------------------------------------*/
/* assignment of d2_obj_defined and d2_obj                      */
/*--------------------------------------------------------------*/
d21_object_len = (d21_obj_len *) in_msg_p;
out_p += sizeof (D2_guide);          /* out_p to  d2_obj.d2_type*/
in_msg_p += sizeof (d21_obj_len);    /* in_msg_p to object_id   */

i_size = d21_object_len -> d21_length;
i_offset = d21_object_len -> d21_offset;

if (i_offset > 0)
   {
      guide->d2_obj_defined = TRUE;
      guide->d2_obj.d2_typ_len = i_size;
      guide->d2_obj.d2_type = (Ob_id_string)out_p;
      (void) memcpy ((char *)(guide->d2_obj.d2_type), in_msg_p, i_size);
      out_p += D2_ALIGN(i_size);
      in_msg_p += i_offset;
   }
else
   {
      guide->d2_obj_defined = FALSE;
      guide->d2_obj.d2_typ_len = 0;
      guide->d2_obj.d2_type = NULL;
   }

guide -> d2_crit.d2_crit_val.type = (D2_crit_item *) out_p;

/*--------------------------------------------------------------*/
/* call function d21_128_crit_fr_ipc                            */
/*--------------------------------------------------------------*/
mem_ptr = (char *) &guide->d2_crit;
d21_128_crit_fr_ipc (&in_msg_p, &mem_ptr);

if (chng_out == TRUE)
  *out = mem_ptr;
*in_msg += D23_S_ATV + in_av->d23_avoff;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_128_crit_fr_ipc      (in C-module  d21spafm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        02.04.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the crit. attributes from       */
/*              string format to structure format                     */
/*                                                                    */
/* INPUT PARAMETERS: inout_msg: ptr to ptr to criteria (msg-format)   */
/*                   inout_dua: pointer to pointer crit (dua-fmt)     */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d21_128_crit_fr_ipc(
  char ** inout_msg,
  char ** inout_dua)
{
D2_criteria       *criteria_p;
D2_crit_set       *crit_set_p;
d21_crdesc        *d21_critdesc;
signed16                nb_crits, i;

/*--------------------------------------------------------------*/
/* assign D2_criteria                                           */
/*--------------------------------------------------------------*/
d21_critdesc = (d21_crdesc *) *inout_msg;
criteria_p = (D2_criteria *) *inout_dua;
criteria_p -> d2_c_mode = d21_critdesc -> d21_choice;

*inout_msg += sizeof (d21_crdesc);
*inout_dua = (char *) criteria_p -> d2_crit_val.type;

/*--------------------------------------------------------------*/
/* switch choice                                                */
/*--------------------------------------------------------------*/
switch (d21_critdesc -> d21_choice)
   {
       case D2_ITEM:
	       d21_129_item_fr_ipc (inout_msg, inout_dua);
	       break;
       case D2_NOT:
	       criteria_p = (D2_criteria *) *inout_dua;
	       criteria_p -> d2_crit_val.not = criteria_p + 1;
	       d21_128_crit_fr_ipc (inout_msg, inout_dua);
	       break;
       case D2_AND:
       case D2_OR:
	       crit_set_p = (D2_crit_set *) *inout_dua;
	       crit_set_p -> and_or_nr = d21_critdesc -> d21_count;
	       *inout_dua += sizeof (D2_crit_set);
	       crit_set_p -> and_or = (D2_criteria *) *inout_dua;

	       nb_crits = d21_critdesc -> d21_count;
	       for (i=0; i<nb_crits; i++)
		 {
		   criteria_p = (D2_criteria *) *inout_dua;
		   criteria_p -> d2_crit_val.not = criteria_p + 1;
		   d21_128_crit_fr_ipc (inout_msg, inout_dua);
		 }
	       break;
       default:
	    break;
   }

}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_129_item_fr_ipc      (in C-module  d21spafm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        02.04.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the item  attributes from       */
/*              string format to structure format                     */
/*                                                                    */
/* INPUT PARAMETERS: inout_msg: pointer to ptr to item (msg-format)   */
/*                   inout_dua: pointer to pointer item (dua-fmt)     */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

static void d21_129_item_fr_ipc (
  char ** inout_msg,
  char ** inout_dua)
{
D2_crit_item     *crit_item_p;
d21_itdesc       *d21_itemdesc;
signed16 i_size;
unsigned16 i_match;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
crit_item_p = (D2_crit_item *) *inout_dua;
d21_itemdesc = (d21_itdesc *) *inout_msg;
i_size = d21_itemdesc -> d21_length;
i_match = d21_itemdesc -> d21_match;

/*--------------------------------------------------------------*/
/* assign strukture crit_item                                   */
/*--------------------------------------------------------------*/
*inout_msg += sizeof (d21_itdesc);
*inout_dua += sizeof (D2_crit_item);
crit_item_p -> d2_ci_mode  =  i_match;
crit_item_p -> d2_ci_type.d2_typ_len  =  i_size;
crit_item_p -> d2_ci_type.d2_type  =  (Ob_id_string) *inout_dua;

(void) memcpy (*inout_dua, *inout_msg, i_size);
*inout_dua += D2_ALIGN(i_size);
*inout_msg += d21_itemdesc->d21_offset;

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_123_T61seq_fr_ipc    (in C-module  d21spafm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        27.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the T61_seq attributes from     */
/*              string format to structure format                     */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: ptr-ptr to t61seq-message             */
/*                              (preceded by structure D23_av_value)  */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr-ptr to t61_seq-attributes (struct.)  */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d21_123_T61seq_fr_ipc(
  char ** in_msg,
  char ** out,
  Bool chng_out)   /* Flag to indicate that out ptr is to be changed or not */
{
char           *out_p, *in_msg_p;
D2_str          *D2_str_p;
D23_av_value    *in_av ;
D2_T61_seq      *T61_seq_p;
signed32             i_count, nb_str, str_len, str_sum=0;
signed16             max_len;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
in_av = (D23_av_value *)*in_msg;
in_msg_p = *in_msg+D23_S_ATV;           /* Startaddr. (read)          */
out_p= *out;                   /* Startaddr.  (write)        */

/*---------------------------------------------------------*/
/* compute number of strings, assign output-string         */
/*---------------------------------------------------------*/
nb_str = 0;
i_count = 0;
max_len = in_av -> d23_avlen;
while (i_count < max_len)
   {
      nb_str++;
      str_len = strlen (in_msg_p);
      in_msg_p += str_len + 1;
      i_count  += str_len + 1;
   }
T61_seq_p = (D2_T61_seq *) out_p;
out_p += sizeof (D2_T61_seq);
T61_seq_p -> d2_nb_item = nb_str;
T61_seq_p -> d2_t61_item = (D2_str *) out_p;
D2_str_p = (D2_str *) out_p;

/*---------------------------------------------------------*/
/* position of pointer for output-string                   */
/*---------------------------------------------------------*/
out_p += nb_str * sizeof (D2_str);

/*---------------------------------------------------------*/
/* null-terminated T61_seq-strings                         */
/*---------------------------------------------------------*/
in_msg_p = *in_msg+D23_S_ATV;
for (i_count=0; i_count < nb_str-1; i_count++)
   {
      str_len = strlen (in_msg_p);
      D2_str_p -> d2_size = str_len;
      D2_str_p -> d2_value = (unsigned char *)out_p;
      (void) memcpy(out_p,in_msg_p, str_len);

      str_sum += str_len + 1;
      in_msg_p += str_len + 1;
      out_p += D2_ALIGN(str_len);
      D2_str_p ++;
   }
/*------------------------------------------------------*/
/* last T61_seq-"string"                                */
/*------------------------------------------------------*/
str_len = in_av->d23_avlen - str_sum;
D2_str_p -> d2_size = str_len;
D2_str_p -> d2_value = (unsigned char *)out_p;
(void) memcpy(out_p,in_msg_p, str_len);
 
if (chng_out == TRUE)
  *out = out_p + D2_ALIGN(str_len);
*in_msg += D23_S_ATV + in_av->d23_avoff;

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_124_telexnr_fr_ipc   (in C-module  d21spafm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        05.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the telex_number attributes from*/
/*              string format to structure format                     */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: ptr-ptr to telexnr-message            */
/*                              (preceded by structure D23_av_value)  */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr-ptr to telexnr-attributes (struct)   */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/* STANDARD-HEADER:     None                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d21_124_telexnr_fr_ipc (
  char ** in_msg,
  char ** out,
  Bool chng_out)   /* Flag to indicate that out ptr is to be changed or not */
{
char         *out_str_p= *out;      /* Startaddr. (write)   */
char         *in_msg_p = *in_msg+D23_S_ATV;  /* Startaddr.  (read)   */
D2_telex_number   *tlx_number = (D2_telex_number *) *out;
signed16           str_len1;
signed16           str_len2;
signed16           str_len3;
D23_av_value  *in_av = (D23_av_value *)*in_msg;

out_str_p += sizeof(D2_telex_number);

/*---------------------------------------------------------*/
/* assign D2_str-structures                                */
/*---------------------------------------------------------*/
str_len1 = strlen (in_msg_p);
tlx_number->d2_t_number.d2_size = str_len1;
tlx_number->d2_t_number.d2_value = (unsigned char *)out_str_p;
(void) memcpy (out_str_p,in_msg_p, str_len1);

in_msg_p += str_len1 + 1;
out_str_p += str_len1;

str_len2 = strlen (in_msg_p);
tlx_number->d2_c_code.d2_size = str_len2;
tlx_number->d2_c_code.d2_value = (unsigned char *)out_str_p;
(void) memcpy (out_str_p,in_msg_p, str_len2);

in_msg_p += str_len2 + 1;
out_str_p += str_len2;

str_len3 = *in_msg + D23_S_ATV + in_av->d23_avlen - in_msg_p;
tlx_number->d2_answer.d2_size = str_len3;
tlx_number->d2_answer.d2_value = (unsigned char *)out_str_p;
(void) memcpy (out_str_p,in_msg_p, str_len3);

if (chng_out == TRUE)
  *out += sizeof(D2_telex_number) + D2_ALIGN(str_len1 + str_len2 + str_len3);
*in_msg += D23_S_ATV + in_av->d23_avoff;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_125_faxnr_fr_ipc     (in C-module  d21spafm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        28.03.91                                              */
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
/*exoff ***************************************************************/

void d21_125_faxnr_fr_ipc(
  char ** in_msg,
  char ** out,
  Bool chng_out)   /* Flag to indicate that out ptr is to be changed or not */
{
char          *out_p, *in_msg_p =  *in_msg + D23_S_ATV;
D23_av_value   *in_av = (D23_av_value *) *in_msg;
signed16            i_size, i_offset;
d21_faxnr      * faxnr_p = (d21_faxnr *) in_msg_p;
signed32           *non_basic_parm;
D2_faxnumber   * D2_faxnr_p = (D2_faxnumber *) *out;
/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
out_p = *out;                       /* Startaddr. (write)   */
i_size = faxnr_p -> d2_str_len;
i_offset = faxnr_p -> d2_offset;

out_p += sizeof (D2_faxnumber);
in_msg_p += sizeof (d21_faxnr);

D2_faxnr_p -> d2_tel_nr.d2_size = i_size;
D2_faxnr_p -> d2_tel_nr.d2_value = (unsigned char *)out_p;
(void) memcpy (out_p,in_msg_p, i_size);
out_p += D2_ALIGN(i_size);
if (i_offset > 0)
   {
      D2_faxnr_p -> d2_par_defined = TRUE;
      in_msg_p += i_offset;
      non_basic_parm = (signed32 *) in_msg_p;
      D2_faxnr_p -> d2_par = *non_basic_parm;
   }
else
   {
      D2_faxnr_p -> d2_par_defined = FALSE;
      D2_faxnr_p -> d2_par = 0;
   }

*in_msg += D23_S_ATV + in_av->d23_avoff;
if (chng_out == TRUE)
  *out = out_p;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_126_post_ad_fr_ipc     (in C-module  d21spafm)    */
/*                                                                    */
/* AUTHOR:      Jochen Keutel                                         */
/* DATE:        7.4.93                                                */
/*                                                                    */
/* DESCRIPTION: The function converts the postal address attributes   */
/* 		from string format to structure format                */
/*                                                                    */
/* INPUT PARAMETERS:    in_msg: ptr-ptr to post_ad-message            */
/*                              (preceded by structure D23_av_value)  */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr-ptr to post_ad-attributes (struct.)  */
/*                                                                    */
/* RETURN VALUE :       D2_ERROR or D2_NOERROR                        */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_126_post_ad_fr_ipc (
  char ** in_msg,
  char ** out)
{
char           *in_msg_p;
char		*part_ptr;
D23_av_value    *in_av ;
D2_str_list     *str_list;
signed16           len,part_len;
signed16             max_len;
unsigned16 rep;

in_av = (D23_av_value *)*in_msg;
in_msg_p = *in_msg+D23_S_ATV;          

len = 0;
part_len = 0;
max_len = in_av -> d23_avlen;
while (len < max_len)
  {
  if (part_len == 0) /* representation identifier has to come */
    {
    switch (*in_msg_p)
      {
      case D2_T61_REP:
	rep = D2_T61;
	break;
      case D2_PRTBL_REP:
	rep = D2_PRINTABLE;
	break;
      default:
	return (D2_ERROR);
      }
    part_ptr = in_msg_p + 1;
    } /* if part_len == 0 */
  len++;
  if ( (*in_msg_p == D2_EOS) || (len == max_len) )
    {
    if (len == max_len) part_len++;
    str_list = (D2_str_list *) *out;
    str_list->d2_rep = rep;
    *out += sizeof (D2_str_list);
    str_list->d2_str.d2_value = (unsigned char *)*out;
    memcpy(str_list->d2_str.d2_value,part_ptr,part_len-1);
    str_list->d2_str.d2_size = part_len - 1;
    *out += D2_ALIGN(part_len - 1);
    str_list->d2_next = (D2_str_list *) *out;
    part_len = 0;
    }
  else
    {
    part_len++;
    }

  in_msg_p++;
  }

*in_msg += D23_S_ATV + in_av->d23_avoff;
str_list->d2_next = (D2_str_list *) 0;
return (D2_NOERROR);
}
