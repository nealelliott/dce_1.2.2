/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2aattr.c,v $
 * Revision 1.1.10.2  1996/02/18  19:43:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:24:02  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:24:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:47  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:44:31  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:27:50  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:33  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:16:58  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:37:34  marrek
 * 	February 1994 code drop
 * 	[1994/02/17  15:18:24  marrek]
 * 
 * 	Fix memory leak problem.
 * 	[1994/02/10  10:43:36  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:46:07  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:03:48  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:37:45  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:13:23  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:11:00  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  16:01:10  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  13:04:12  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:09:01  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:03:30  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d2aattr.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:43:10 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYP		: C-FILE					      */
/*								      */
/* NAME         : d2aattr.c                                         */
/*								      */
/* AUTHOR       : W. Schmid, DI AP 11                                 */
/* DATE         : 9. 1. 1990                                          */
/*								      */
/* DESCRIPTION  : This source files contains subroutiones to          */
/*                transform APDUs to IDS interface.           */
/*								      */
/* SYSTEM DEPENDENCIES:  COMMON                                       */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    | 90-01-09| Original                       | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <d2asn1.h>
#include <d2asn1_int.h>

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a029_entry_info_to_at_info ()                         */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms the ids_info structure from THORN-IDS format to    */
/*      APDU format (D23_at_info).                                    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      nb_attr_infos = number of attribute informations in THORN-IDS */
/*                      format (number of ids_info structures)        */
/*      attr_infos    = attribute informations in THORN-IDS format    */
/*                      (pointer to first structure, nb_attr_infos    */
/*                       follows)                                     */
/*      act_apdu      = pointer to the APDU where to write the        */
/*                      attribute info (D23_at_info)                  */
/*      beg_apdu      = start address of APDU                         */
/*      max_apdu_len  = max. length of allocated APDU memory          */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      act_apdu    = pointer to next tag in the APDU where to write  */
/*                    the next block                                  */
/*      beg_apdu    = (new) start address of APDU 		      */
/*      max_apdu_len= (new) max. length of allocated APDU memory      */
/*      err_apdu    = error APDU (memory error)                       */
/*      err_len     = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        function fails                                */
/*      D2_NOERROR      function successful                           */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: W. Schmid                                                 */
/**********************************************************************/

signed32 d2a029_entry_info_to_at_info (
  signed32                 nb_attr_infos,
  ids_info            *attr_infos,
  byte               **act_apdu,
  byte               **beg_apdu,
  signed32              *max_apdu_len,
  byte               *err_apdu,
  signed32              *err_len )
{

ids_info        *act_ids_info;
signed32             i;
signed32             error = FALSE;
signed32             return_value = D2_NOERROR;
const char function_name[] = "d2a_200_certificate_from_apdu";

  /* for all attribute informations */
  for (act_ids_info = attr_infos, i = 0;
       ((i < nb_attr_infos) && (error == FALSE));
       i++, act_ids_info++) {

      /* set parameters for function d2a060 */
      if (act_ids_info->type_only == TRUE) {
	 return_value = d2a124_copy_attyp_apdu(act_apdu,
			&act_ids_info->contents.type,
			beg_apdu, max_apdu_len,
			err_apdu, err_len);
      } else {
	if (act_ids_info->contents.attribute.nb_values <= 0)
	  {
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }
	  
	return_value= d2a060_mk_att_apdu(&(act_ids_info->contents.attribute),
			   		act_apdu,beg_apdu, max_apdu_len,
						err_apdu, err_len);
      } /* if (act_ids_info->type_only == TRUE) */

      if (return_value != D2_NOERROR) {
	 error = TRUE;
	 break;
      }
  } /* for all attributes */

return(return_value);
} /* End d2a029_entry_info_to_at_info */


/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a030_at_info_to_entry_info()                          */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms the attribute infos from APDU format to IDS        */
/*      format ("ids_info")                                           */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = pointer to the APDU containing requested        */
/*                    attributes                                      */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      apdu        = pointer to next tag in the APDU                 */
/*      nb_infos    = number of generated attribute infos in IDS      */
/*                    format                                          */
/*      infos       = generated list of attribute info in IDS format  */
/*      err_apdu    = error APDU (memory error)                       */
/*      err_len     = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        function fails                                */
/*      D2_NOERROR      function successful                           */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: W. Schmid                                                 */
/**********************************************************************/

signed32 d2a030_at_info_to_entry_info(
  byte              **apdu,
  signed32                *nb_infos,
  ids_info           **infos,
  byte              *err_apdu,
  signed32                *err_len )
{
signed32              i=0;
signed32              j;
byte           *p_mess = *apdu;
D23_av_info     *av_info;
ids_info        *act_info;
ids_att_value   *ids_valptr;
dua_att_value   valptr;
signed32              syntax;
D23_av_value    *av_value;
const char function_name[] = "d2a030_at_info_to_entry_info";

/* allocate memory for "ids_info" structures - if there are attributes */

if (((D23_at_type *)p_mess)->d23_atttag == D23_ATTTAG && *nb_infos > 0)
  {
  if ((*infos = D27_MALLOC(svc_c_sev_warning,*nb_infos * sizeof(ids_info))) == NULL)
    {
    d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR, D2_SYS_ERR,
			   D21_ALLOC_ERROR, err_len);
    D2_ASN1_ERR_RETURN_WITH_TRACE
    }

  for (i = 0, act_info = *infos;
     ((D23_at_type *)p_mess)->d23_atttag == D23_ATTTAG && i < *nb_infos;
     i++, act_info++)
    {   /* allocate memory for attribute type */
    if (d2a120_copy_att_type(&p_mess, &(act_info->contents.type), err_apdu,
	     err_len ) == D2_ERROR) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}
    av_info = (D23_av_info *) p_mess;
    if (av_info->d23_avtag != D23_AVTAG )
	act_info->type_only = TRUE;
    else
      if (av_info->d23_avnum == 0)
	act_info->type_only = TRUE;
      else
        {
        act_info->type_only = FALSE;
	syntax = av_info->d23_asyntx;
	p_mess += D23_S_AV;
	   /* allocate memory for attribute values */

	if ((ids_valptr = act_info->contents.attribute.values =
	       D27_MALLOC (svc_c_sev_warning,av_info->d23_avnum * sizeof(ids_att_value)))
		== NULL)
	  {
	  d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
				      D2_SYS_ERR, D21_ALLOC_ERROR,
				      err_len);
	  D2_ASN1_ERR_RETURN_WITH_TRACE
	  }

	for (j = 0, av_value = (D23_av_value *) p_mess, p_mess += D23_S_ATV;
		j < av_info->d23_avnum && (unsigned16) (av_value->d23_avlen) <
							   D23_MINTAG;
		j++, ids_valptr++, p_mess += D23_S_ATV)
	  {   /* copy attribute value from APDU */

	  if (d2a031_cp_attr(av_value->d23_avrep, av_value->d23_avlen,
			p_mess, &valptr, err_apdu, err_len) != D2_NOERROR)
	    {
	    free(act_info->contents.attribute.values);
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	    }
	  if (dua_att_value_ids ( &valptr, ids_valptr) == D2_ERROR)
	    {
	    free(act_info->contents.attribute.values);
	    D2_ASN1_ERR_RETURN_WITH_TRACE
	    }

	  if (syntax != D2_DISTNAME)
		{
	 	dua_att_value_free( &valptr);
		}
	  else
		{
		d_name_free(valptr.value.name_value);
		free(valptr.value.name_value);
		}

	       /*
		   take next recurring value
	       */
	  p_mess += av_value->d23_avoff;
	  av_value = (D23_av_value *) p_mess;
	  }
	p_mess -= D23_S_ATV;
	act_info->contents.attribute.nb_values = j;
        }
    } /* of for loop */

  } /* end of : attributes are there */

*nb_infos = i;
*apdu = p_mess;

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a060_mk_att_apdu ()                                   */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms the ids_att  structure from IDS format to          */
/*      APDU format (D23_at_info).                                    */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      attr_info     = attribute information  in THORN-IDS format    */
/*                      (ids_att structure)                           */
/*      act_apdu      = pointer to the APDU where to write the        */
/*                      attribute info (D23_at_info)                  */
/*      beg_apdu      = start address of APDU                         */
/*      max_apdu_len  = max. length of allocated APDU memory          */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      act_apdu    = pointer to next tag in the APDU where to write  */
/*                    the next block                                  */
/*      beg_apdu    = (new) start address of APDU 		      */
/*      max_apdu_len= (new) max. length of allocated APDU memory      */
/*      err_apdu    = error APDU (memory error)                       */
/*      err_len     = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        function fails                                */
/*      D2_NOERROR      function successful                           */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: W. Schmid                                                 */
/**********************************************************************/

signed32 d2a060_mk_att_apdu (
  ids_att             *attr_info,
  byte               **act_apdu,
  byte               **beg_apdu,
  signed32                 *max_apdu_len,
  byte               *err_apdu,
  signed32                 *err_len )
{ 
D23_av_info     *av_info;
D23_av_value    *av_value;
ids_att_value   *ids_attr_val;
signed32             j, no_attr_vals, rv;
byte        *  scan;
signed32             return_value;
signed32     at_info_size = sizeof (D23_at_type)+ D2_OBJ_IDL_MAX +
			  sizeof(D23_av_info)+sizeof(D23_av_value);

signed16    *val_len;
signed16    *val_off;
unsigned16    *val_rep;
signed32       syntax;
const char function_name[] = "d2a060_mk_att_apdu";

      /* set structure D23_at_type */
      rv = d2a033_check_memsize (at_info_size, beg_apdu, act_apdu,
				 max_apdu_len, (D23_rserror *) err_apdu,
				 err_len);
      if (rv != D2_NOERROR) {
	 /* not enough memory for D23_at_info */
	 *act_apdu=scan;
	 D2_ASN1_ERR_RETURN_WITH_TRACE
      } /* if (rv != D2_NOERROR) */

      scan = *act_apdu;
      if (d2a124_copy_attyp_apdu(&scan,&attr_info->type, beg_apdu, max_apdu_len,
	     				err_apdu, err_len) != D2_NOERROR ) 
	{
	D2_ASN1_ERR_RETURN_WITH_TRACE
	}

      if (attr_info->nb_values != 0) {
	 /* MAP all attribute values */
        av_info = (D23_av_info *) scan;
        av_info->d23_avtag = D23_AVTAG;
        av_info->d23_asyntx = syntax = recover_att_syntax(&attr_info->type);
        if (syntax == D2_ERROR) av_info->d23_asyntx = D2_ASN1;
        av_info->d23_avnum = attr_info->nb_values;
        scan += sizeof(D23_av_info);

	 no_attr_vals = attr_info->nb_values ;

	 for (j = 0, ids_attr_val = attr_info->values;
	      j < no_attr_vals; j++, ids_attr_val++)
	  {
	    av_value = (D23_av_value *) scan;
	    val_len = &(av_value->d23_avlen);
	    val_off = &(av_value->d23_avoff);
	    val_rep = &(av_value->d23_avrep);
	    av_value->d23_avfil = 0;
	    scan += sizeof(D23_av_value);
	    if              /* error from converting attribute value  */
	      ((return_value=d2a106_ids_atval_to_atval(ids_attr_val,
		syntax,val_len,val_off,val_rep,&scan,beg_apdu,max_apdu_len,
		err_apdu,err_len)) != D2_NOERROR)
		{
		*act_apdu=scan;
		return(return_value);
		 };
	 } /* for all values */
      } /* if (attr_info->nb_values == 0) */

*act_apdu = scan;

return(D2_NOERROR);
} /* End d2a060_mk_att_apdu */

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: d2a062_mk_apdu_att ()                                   */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      Transforms the attribute infos from APDU format to IDS        */
/*      format ("ids_att")                                            */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      apdu        = pointer to the APDU containing                  */
/*                    attributes                                      */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      ids_attr    = generated list of attribute info in IDS format  */
/*      nb_attr     = number of generated attribute infos in IDS      */
/*                    format                                          */
/*                    0: no attribute information converted           */
/*                    1: one attribute information converted          */
/*      err_apdu    = error APDU (memory error)                       */
/*      err_len     = length of error APDU                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_ERROR        function fails                                */
/*      D2_NOERROR      function successful                           */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: W. Schmid                                                 */
/**********************************************************************/

signed32 d2a062_mk_apdu_att(
byte              **apdu,
ids_att             *ids_attr,
signed32                 *nb_attr,
byte               *err_apdu,
signed32                 *err_len )
{
signed32              j;
byte           *p_mess = *apdu;
D23_av_info     *av_info;
ids_att_value   *ids_valptr;
dua_att_value   valptr;
D23_av_value    * av_value;
signed32              syntax;
const char function_name[] = "d2a062_mk_apdu_att";

if (d2a120_copy_att_type(apdu, &ids_attr->type, err_apdu, err_len)
	 == D2_ERROR)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }

av_info = (D23_av_info *)*apdu;
p_mess = *apdu + D23_S_AV;
*nb_attr = 0;

if (av_info->d23_avnum <= 0)
  {
  D2_ASN1_ERR_RETURN_WITH_TRACE
  }
if ((ids_valptr = ids_attr->values =
	     D27_MALLOC(svc_c_sev_warning,av_info->d23_avnum * sizeof(ids_att_value))) == NULL)
{   
    d27_010_set_error_apdu((D23_rserror *) err_apdu, D2_TEMP_ERR,
			     D2_SYS_ERR, D21_ALLOC_ERROR,
			     err_len);
    D2_ASN1_ERR_RETURN_WITH_TRACE
}

/*
   get attribute syntax
*/

syntax = av_info->d23_asyntx;

for (j = 0, av_value = (D23_av_value *)p_mess;
     (unsigned16)(av_value->d23_avlen) < D23_MINTAG;
     av_value = (D23_av_value *)p_mess,j++,ids_valptr++)
{   /*
       copy attribute value from APDU
    */
    p_mess += D23_S_ATV;
    if (d2a031_cp_attr(av_value->d23_avrep, av_value->d23_avlen, p_mess,
				&valptr, err_apdu, err_len) != D2_NOERROR)
      { 
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    if (dua_att_value_ids(&valptr, ids_valptr)  == D2_ERROR)
      {
      D2_ASN1_ERR_RETURN_WITH_TRACE
      }

    if (syntax != D2_DISTNAME)
    	dua_att_value_free(&valptr);
    else
	free(valptr.value.name_value);

    p_mess += av_value->d23_avoff;

}
*nb_attr = ids_attr->nb_values = j;
*apdu = p_mess;

return(D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: signed32 d2a170_t61_pr_seq_len ()                            */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/* 	computes the length of the t61_pr_seq stucture in DUA	      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*	pointer to APDU						      */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*	length							      */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Jochen Keutel (6.4.93)                                    */
/**********************************************************************/

signed32 d2a170_t61_pr_seq_len (
  byte * apdu)
{
signed32 i;
signed32 ret_value;
signed32 nb_strings = 0;
D23_av_value * av_v = (D23_av_value *) apdu;

ret_value = sizeof(t61_pr_seq);

apdu += sizeof (D23_av_value);

for (i=0; i< av_v->d23_avlen; i++)
  {
  if ( (*(apdu + i) == D2_T61_REP) || (*(apdu + i) == D2_PRTBL_REP) )
	nb_strings ++;
  }

ret_value += nb_strings * sizeof (str_PT_choice);
ret_value += D2_ALIGN(av_v->d23_avlen + nb_strings);

return(ret_value);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: signed32 d2a171_t61_pr_seq_fr_ipc ()                         */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*	transforms postal address from APDU to t61_pr_seq structure   */
/*		(used in DUA interface)				      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*	apdu : pointer to APDU					      */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*	apdu : pointer to rest of APDU				      */
/*	t_pr_seq : pointer to filled t61_pr_seq structure	      */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*	D2_ERROR or D2_NOERROR					      */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Jochen Keutel (6.4.93)                                    */
/**********************************************************************/

signed32 d2a171_t61_pr_seq_fr_ipc (
  byte ** apdu,
  t61_pr_seq * t_pr_seq)
{
const char function_name[] = "d2a171_t61_pr_seq_fr_ipc";
byte * part_ptr;
D23_av_value * av_v = (D23_av_value *) *apdu;
signed32 len = 0;
signed32 part_len = 0;
signed32 nb = 0;
char * out = (char *) t_pr_seq;
byte * in;

out += sizeof (t61_pr_seq);
t_pr_seq->v = (str_PT_choice *)out;

*apdu += sizeof (D23_av_value);
in = *apdu;

/* compute the number of lines */
while (len < av_v->d23_avlen)
  {
  in++;
  len ++;
  if ((*in == D2_EOS) || (len == av_v->d23_avlen))
    {
    nb++;
    }
  }

/* fill t61_pr_seq structure */
out += nb * sizeof (str_PT_choice);

len = 0;
nb = 0;
while (len < av_v->d23_avlen)
  {
  if (part_len == 0) /* representation identifier has to come */
    {
    switch (**apdu)
      {
      case D2_T61_REP:
	t_pr_seq->v[nb].mode = T61;
	break;
      case D2_PRTBL_REP:
	t_pr_seq->v[nb].mode = PRINTABLE;
	break;
      default:			
	D2_ASN1_ERR_RETURN_WITH_TRACE
      }
    t_pr_seq->v[nb].str_val = (string) out;
    part_ptr = *apdu + 1;
    } /* if part_len == 0 */
  len++;
  if ( ((**apdu) == D2_EOS) || (len == av_v->d23_avlen) )
    {
    if ( len ==  av_v->d23_avlen ) part_len ++;
    memcpy(t_pr_seq->v[nb].str_val,part_ptr,part_len - 1);
    *(t_pr_seq->v[nb].str_val + (part_len-1)) = '\0';
    if ( len != av_v->d23_avlen )
				out += part_len;
    nb++;
    part_len = 0;
    }
  else
    {
    part_len++;
    }

  (*apdu)++;
  }

t_pr_seq->nb = nb;
return (D2_NOERROR);
}

/**********************************************************************/
/*                                                                    */
/*  FUNCTION: signed32 d2a172_t61_pr_seq_to_ipc ()                         */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*	transforms postal address from t61_pr_seq structure 	      */
/*		(used in DUA interface) to APDU			      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*	t_pr_seq : pointer to filled t61_pr_seq structure	      */
/*	apdu : pointer to APDU					      */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*	apdu : pointer to rest of APDU				      */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*	D2_ERROR or D2_NOERROR					      */
/*                                                                    */
/* -------------------------------------------------------------------*/
/*  AUTHOR: Jochen Keutel (6.4.93)                                    */
/**********************************************************************/

signed32 d2a172_t61_pr_seq_to_ipc (
  t61_pr_seq * t_pr_seq,
  byte ** apdu)
{
const char function_name[] = "d2a172_t61_pr_seq_to_ipc";
signed32 i;
signed32 len;
byte * apdu_save;
D23_av_value * av_out = (D23_av_value *) *apdu;

*apdu += sizeof (D23_av_value);
apdu_save = *apdu;

for (i=0; i<t_pr_seq->nb; i++)
  {
  switch (t_pr_seq->v[i].mode)
    {
    case PRINTABLE:
	**apdu = D2_PRTBL_REP;
	(*apdu)++;
	break;
    case T61:
	**apdu = D2_T61_REP;
	(*apdu)++;
	break;
    default:
	D2_ASN1_ERR_RETURN_WITH_TRACE
    }
  len = strlen(t_pr_seq->v[i].str_val);
  memcpy((char *)*apdu,t_pr_seq->v[i].str_val,len);
  *apdu += len ;
  if ((i+1) < t_pr_seq->nb)
    {
    **apdu = '\0';
    (*apdu)++;
    }
  } /* of for loop */

av_out->d23_avlen = *apdu - apdu_save;
av_out->d23_avoff = D2_ALIGN(av_out->d23_avlen);
*apdu = apdu_save + av_out->d23_avoff;
return (D2_NOERROR);
}
