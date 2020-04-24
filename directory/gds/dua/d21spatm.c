/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21spatm.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:51  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:55:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:52  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:54:16  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:33:33  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:44:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:37:02  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:43:32  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:01:18  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:49:07  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:26:33  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:39:10  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:44:11  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  03:37:41  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:38:52  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21spatm.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:52 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21spatm                                            */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 05.03.91                                            */
/*                                                                    */
/* DESCRIPTION  : The module converts the attributes of ttxid,        */
/*                t61_seq, telex-number, fax-number and search guide  */
/*                to string format                                    */
/*                                                                    */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <d21dua.h>

/******** LOCAL FUNCTIONS ********/

static void  d21_109_item_to_ipc ( D2_crit_item * , char ** );

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_101_ttxid_to_ipc    (in C-module  d21spatm)       */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        08.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the ttxid attributes to         */
/*              string format                                         */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to ttxid-attributes               */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr to ptr to message                    */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d21_101_ttxid_to_ipc(
  D2_ttxid * in,
  char ** out)
{
char *out_p = *out + D23_S_ATV;
D23_av_value * av_out = (D23_av_value *) *out;

char *in_str_p;
D2_str        *D2_str_p;
d21_ttxid     *ttxid_p = (d21_ttxid *)out_p;
signed32            i_size, i_count, i;
unsigned16         ttxid_kz;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/


D2_str_p = (D2_str *) in;

for (i_count=0; i_count < 6; i_count++)
  {
    /*----------------------------------------------------------------*/
    /* assign output-message                                          */
    /*----------------------------------------------------------------*/
    i_size = D2_str_p -> d2_size;

    if (i_size > 0)
      {
	 /*-----------------------------------------------------------*/
	 /* assign       "string-type"                                */
	 /*-----------------------------------------------------------*/
	 switch (i_count)
	   {
	      case 0: ttxid_kz = D2_TERM; break;
	      case 1: ttxid_kz = D2_CTRL; break;
	      case 2: ttxid_kz = D2_GRPH; break;
	      case 3: ttxid_kz = D2_MISC; break;
	      case 4: ttxid_kz = D2_PAGE; break;
	      case 5: ttxid_kz = D2_PRIV; break;
	      default: break;
	   }
	 /*-----------------------------------------------------------*/
	 /* assign   d21_ttxid and copy to output-string              */
	 /*-----------------------------------------------------------*/
	 ttxid_p->d2_str_type = ttxid_kz;
	 ttxid_p->d2_filler  = 0;
	 ttxid_p->d2_str_len = i_size;
	 ttxid_p->d2_offset = D2_ALIGN(i_size);
	 out_p += sizeof (d21_ttxid)     ;

	 /*-----------------------------------------------------------*/
	 /* copy string to output-string and assign \0 up to offset   */
	 /*-----------------------------------------------------------*/
	 in_str_p = (char *)(D2_str_p->d2_value);
	 (void) memcpy (out_p,in_str_p, i_size);

	 for (i=i_size; i<D2_ALIGN(i_size); i++)
	      *(out_p+i) = '\0';

	 out_p += D2_ALIGN(i_size);
	 ttxid_p = (d21_ttxid *) out_p;
      }
    D2_str_p++;

  }

av_out->d23_avlen = out_p - (*out + D23_S_ATV);
av_out->d23_avoff = D2_ALIGN(av_out->d23_avlen);
*out += D23_S_ATV + av_out->d23_avoff;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_102_guide_to_ipc     (in C-module  d21spatm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        08.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the guide attributes to         */
/*              string format                                         */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to guide-attributes               */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr to ptr to message                    */
/*                                                                    */
/* RETURN VALUE :       ERROR / NOERROR				      */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_102_guide_to_ipc(
  D2_guide * in,
  char ** out)
{
char *out_p = *out + D23_S_ATV;
D23_av_value * av_out = (D23_av_value *) *out;

D2_criteria   *criteria_p;
struct {
	 signed16 offset;
	 signed16 length;
       } object_len;
char *in_str_p;
signed32            i_size, nb_fillers, i;

if (in -> d2_obj_defined == TRUE)
   {
      i_size = in -> d2_obj.d2_typ_len;
      in_str_p = (char *)in -> d2_obj.d2_type;
   }
else
   i_size = 0;

object_len.length = i_size;
object_len.offset = D2_ALIGN (i_size);

memcpy( out_p, (char *) &object_len, sizeof(object_len));
out_p += sizeof(object_len);

if (in -> d2_obj_defined == TRUE)
   {
      (void) memcpy (out_p,in_str_p, i_size);
      out_p += i_size;
   }

nb_fillers = D2_ALIGN (i_size) - i_size;
for (i=0; i < nb_fillers; i++)
   {
      *out_p='\0';
      out_p++;
   }

/*--------------------------------------------------------------*/
/* copy criteria to output-string                                */
/*--------------------------------------------------------------*/
criteria_p = &(in -> d2_crit);
if (d21_108_crit_to_ipc (criteria_p, &out_p) != D2_NOERROR)
  {
  return(D2_ERROR);
  }

av_out->d23_avlen = out_p - (*out + D23_S_ATV);
av_out->d23_avoff = D2_ALIGN(av_out->d23_avlen);
*out += D23_S_ATV + av_out->d23_avoff;
return(D2_NOERROR);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_108_crit_to_ipc      (in C-module  d21spatm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        12.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the crit. attributes to         */
/*              string format                                         */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to criteria-attributes            */
/*                                                                    */
/* OUTPUT PARAMETERS:   inout_p: pointer to pointer to String         */
/*                                                                    */
/* RETURN VALUE :       ERROR / NOERROR				      */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_108_crit_to_ipc(
  D2_criteria * in,
  char ** inout_p )
{
D2_criteria   * criteria_p;
D2_crit_item  * crit_item_p;
struct {
	 signed16 choice;
	 signed16 count;
       } critdesc;
signed32            nb_crits, i;

/*--------------------------------------------------------------*/
/* copy choice and count to output-string                       */
/*--------------------------------------------------------------*/

critdesc.choice = in -> d2_c_mode;

if (critdesc.choice == D2_AND  ||  critdesc.choice == D2_OR)
  {
  if ( in->d2_crit_val.d2_ci_and_or == 0 ) 
    {
    return(D2_ERROR);
    }
  critdesc.count = in -> d2_crit_val.d2_ci_and_or -> and_or_nr;
  }
else
   critdesc.count = 0;

memcpy(*inout_p, (char *) &critdesc, sizeof (critdesc));
*inout_p += sizeof (critdesc);

/*--------------------------------------------------------------*/
/* next criterion rsp. item                                     */
/*--------------------------------------------------------------*/
switch (in -> d2_c_mode)
   {
       case D2_ITEM:
	       if ( in->d2_crit_val.type == 0 )
		 {
		 return(D2_ERROR);
		 }
	       crit_item_p = in -> d2_crit_val.type;
	       d21_109_item_to_ipc (crit_item_p, inout_p);
	       break;
       case D2_NOT:
	       if ( in->d2_crit_val.not == 0 )
		 {
		 return(D2_ERROR);
		 }
	       criteria_p = in -> d2_crit_val.not;
	       if (d21_108_crit_to_ipc (criteria_p, inout_p) != D2_NOERROR)
		 {
		 return(D2_ERROR);
		 }
	       break;
       case D2_AND:
       case D2_OR:
	       if ( in->d2_crit_val.d2_ci_and_or == 0 )
		 {
		 return(D2_ERROR);
		 }
	       criteria_p = in -> d2_crit_val.d2_ci_and_or -> and_or;
	       nb_crits = in -> d2_crit_val.d2_ci_and_or -> and_or_nr;
	       for (i=0; i<nb_crits; i++)
		 {
	         if (d21_108_crit_to_ipc (criteria_p, inout_p) != D2_NOERROR)
		   {
		   return(D2_ERROR);
		   }
		 criteria_p++;
		 }
	       break;
       default:
	    break;
   }

return(D2_NOERROR);
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_109_item_to_ipc      (in C-module  d21spatm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        12.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the item  attributes to         */
/*              string format                                         */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to criteria-item-attributes       */
/*                                                                    */
/* OUTPUT PARAMETERS:   inout_p: pointer to pointer to String         */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

static void d21_109_item_to_ipc(
  D2_crit_item * in,
  char ** inout_p)
{
struct {
	 signed16 match;
	 signed16 filler;
	 signed16 offset;
	 signed16 length;
       } itemdesc;
char * in_str_p;
signed32 i_size, i;

i_size = in->d2_ci_type.d2_typ_len;
in_str_p = (char *)in->d2_ci_type.d2_type;

/*--------------------------------------------------------------*/
/* copy matching-rule and lengths to output-string              */
/*--------------------------------------------------------------*/
itemdesc.match = in -> d2_ci_mode;
itemdesc.length = i_size;
itemdesc.offset = D2_ALIGN (i_size);

memcpy( *inout_p, (char *) &itemdesc, sizeof (itemdesc));
*inout_p += sizeof (itemdesc);

/*--------------------------------------------------------------*/
/* object-identifier-string to output-string                    */
/*--------------------------------------------------------------*/
(void) memcpy (*inout_p,in_str_p , i_size);
for (i=i_size; i<itemdesc.offset; i++)
    *(*inout_p+i) = '\0';
*inout_p += itemdesc.offset;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_103_T61seq_to_ipc    (in C-module  d21spatm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        05.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the T61_seq attributes to       */
/*              string format                                         */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to T61_seq-attributes             */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr to ptr to message                    */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d21_103_T61seq_to_ipc(
  D2_T61_seq * in,
  char ** out)
{
char *out_p = *out + D23_S_ATV;
D23_av_value * av_out = (D23_av_value *) *out;
D2_str *D2_str_p;
char *in_str_p;
signed32 i_size,j_number;

D2_str_p = in-> d2_t61_item;

/*---------------------------------------------------------*/
/* copy attributes to output-string (null-terminated str.) */
/*---------------------------------------------------------*/
for (j_number=0; j_number < in->d2_nb_item - 1; j_number++)
   {
       i_size = D2_str_p -> d2_size;
       in_str_p = (char *)(D2_str_p->d2_value);

       (void) memcpy (out_p,in_str_p, i_size);

       out_p += i_size;
       *out_p = '\0';
       out_p++;

       D2_str_p++;
   }

/*---------------------------------------------------------*/
/* copy last attribute to output-str (without '\0')        */
/*---------------------------------------------------------*/
j_number =  in -> d2_nb_item - 1;

       i_size = D2_str_p->d2_size;
       in_str_p = (char *)D2_str_p->d2_value;

       (void) memcpy (out_p,in_str_p, i_size);

       out_p += i_size;

av_out->d23_avlen = out_p - (*out + D23_S_ATV);
av_out->d23_avoff = D2_ALIGN(av_out->d23_avlen);
*out += D23_S_ATV + av_out->d23_avoff;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_104_telexnr_to_ipc   (in C-module  d21spatm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        05.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the telex_number attributes to  */
/*              string format                                         */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to telex_number-attributes        */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr to ptr to message                    */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d21_104_telexnr_to_ipc(
  D2_telex_number * in,
  char ** out)
{
char *out_p = *out + D23_S_ATV;
D23_av_value * av_out = (D23_av_value *) *out;
char *in_str;
signed32 i_size;

/*---------------------------------------------------*/
/* copy   telex-number to output-string              */
/*---------------------------------------------------*/
i_size = in -> d2_t_number.d2_size;
in_str = (char *)in->d2_t_number.d2_value;

(void) memcpy (out_p,in_str,i_size);

out_p += i_size;
*out_p = '\0';
out_p++;

/*---------------------------------------------------*/
/*        c-code to output-string                    */
/*---------------------------------------------------*/
i_size = in -> d2_c_code.d2_size;
in_str = (char *)in->d2_c_code.d2_value;

(void) memcpy (out_p,in_str,i_size);

out_p += i_size;
*out_p = '\0';
out_p++;

/*---------------------------------------------------*/
/*        answer to output-string                    */
/*---------------------------------------------------*/
i_size = in -> d2_answer.d2_size;
in_str = (char *)in->d2_answer.d2_value;

(void) memcpy (out_p,in_str,i_size);

out_p += i_size;

av_out->d23_avlen = out_p - (*out + D23_S_ATV);
av_out->d23_avoff = D2_ALIGN(av_out->d23_avlen);

*out += D23_S_ATV + av_out->d23_avoff;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_105_faxnr_to_ipc     (in C-module  d21spatm)      */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        05.03.91                                              */
/*                                                                    */
/* DESCRIPTION: The function converts the faxnr attributes to         */
/*              string format                                         */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to faxnr-attributes               */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr to ptr to message                    */
/*                                                                    */
/* RETURN VALUE :       NONE                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d21_105_faxnr_to_ipc(
  D2_faxnumber * in,
  char ** out)
{
char *out_p = *out + D23_S_ATV;
D23_av_value * av_out = (D23_av_value *) *out;

char * in_str_p;
signed32            i, i_size, nb_fillers;
d21_faxnr      faxnr_struct;

in_str_p = (char *)in->d2_tel_nr.d2_value;
i_size = in->d2_tel_nr.d2_size;

faxnr_struct.d2_str_len = i_size;

/*-----------------------------------------------------------*/
/* assign and copy d21_faxnr_struct                          */
/*-----------------------------------------------------------*/
if (in -> d2_par_defined == TRUE)
    faxnr_struct.d2_offset = D2_ALIGN(i_size);
else
    faxnr_struct.d2_offset = -1;

memcpy(out_p, (char *) &faxnr_struct, sizeof(faxnr_struct));
out_p += sizeof(faxnr_struct);

/*-----------------------------------------------------------*/
/* copy octet-string                                         */
/*-----------------------------------------------------------*/
(void) memcpy (out_p,in_str_p,i_size);

out_p += i_size;

/*-----------------------------------------------------------*/
/* copy non-basic parameter to output-string                 */
/*-----------------------------------------------------------*/
if (in -> d2_par_defined == TRUE)
   {
      nb_fillers = D2_ALIGN(i_size) - i_size;
      for (i=0; i<nb_fillers; i++)
	 {
	    *out_p = '\0';
	    out_p++;
	 }
      memcpy( out_p, (char *) &(in->d2_par), sizeof(signed32));
      out_p += sizeof(signed32);
   }

av_out->d23_avlen = out_p - (*out + D23_S_ATV);
av_out->d23_avoff = D2_ALIGN(av_out->d23_avlen);
*out += D23_S_ATV + av_out->d23_avoff;
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d21_106_post_ad_to_ipc     (in C-module  d21spatm)    */
/*                                                                    */
/* AUTHOR:      Jochen Keutel                                         */
/* DATE:        7.4.93                                                */
/*                                                                    */
/* DESCRIPTION: The function converts the postal address attributes   */
/* 		from structure format to string format                */
/*                                                                    */
/* INPUT PARAMETERS:    in: pointer to post_ad-attributes             */
/*                                                                    */
/* OUTPUT PARAMETERS:   out: ptr to ptr to message                    */
/*                                                                    */
/* RETURN VALUE :       D2_ERROR or D2_NOERROR                        */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_106_post_ad_to_ipc(
  D2_str_list * in,
  char ** out)
{
char *out_p = *out + D23_S_ATV;
D23_av_value * av_out = (D23_av_value *) *out;
signed32 len = 0;

do
  {
  switch (in->d2_rep)
    {
    case D2_T61:
	*out_p++ = D2_T61_REP;
	break;
    case D2_PRINTABLE:
	*out_p++ = D2_PRTBL_REP;
	break;
    default:
	return (D2_ERROR);
    }

  memcpy (out_p,(char *)(in->d2_str.d2_value),in->d2_str.d2_size);
  out_p += in->d2_str.d2_size;
  *out_p++ = '\0';
  len += in->d2_str.d2_size + 2;
  in = in->d2_next;
  }
while (in != NULL);
	
av_out->d23_avlen = len - 1;
av_out->d23_avoff = D2_ALIGN(av_out->d23_avlen);

*out += av_out->d23_avoff + D23_S_ATV;
return (D2_NOERROR);
}

