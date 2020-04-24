/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_c20.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:04  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:53:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:37  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:53:43  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:31:04  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:22  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:30:20  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:02:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:51:28  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:13:26  marrek
 * 	insert <pthread.h>
 * 	introduce D2_IA5
 * 	[1993/02/01  18:09:51  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:46:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:24:16  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:08:24  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:34:54  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  03:12:44  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:36:22  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_c20.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:32 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*
* daton **************************************************************
*                                                                    *
* NAME         :  d21_c20.c                                          *
*                                                                    *
* AUTHOR       :  Albert Biber                                       *
* DATE         :  19.01.88                                           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <stdio.h>

#include <d21dua.h>

/********** LOCAL ************/

static d2_ret_val d21_enum(D2_a_value * , char * , char ** );
static signed16 d21_ca3_attr_iput( Bool , D2_a_info * , char * , char ** );

/*
* exon ***************************************************************
*                                                                    *
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_c22_attr_iput                                  *
*                                                                    *
* AUTHOR       :  Albert Biber,  Softlab GmbH.                       *
* DATE         :  4.1.88                                             *
*                                                                    *
* RETURN-VALUE :                                                     *
*                     D21_BAD_ARGUMENT                               *
*                                                                    *
* exoff **************************************************************
*/

signed16 d21_c22_attr_iput(
  Bool is_attr,
  D2_a_info * from,
  char * to,
  char ** end )
{
  signed16 ret_value;

  ret_value = D2_NO_ERR;

  while (from != NULL)
    {
      ret_value = d21_ca3_attr_iput (is_attr, from, *end, end);
      if (ret_value != D2_NO_ERR)
	  break;
      from = from->d2_a_next;
    }

  return(ret_value);
}                            /* end of d21_c22_entr_iput */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca3_attr_iput()                                */
/*                                                                    */
/* AUTHOR       :  J. Heigert,    Softlab GmbH.                       */
/* DATE         :  24.12.87                                           */
/*                                                                    */
/* DESCRIPTION  :  copy one attribute (in ptr-format) to given        */
/*                 location (in ipc-format)                           */
/*                                                                    */
/* INPUT-PARAMETER :  from: ptr to source attribute                   */
/*                    to:   ptr to target location                    */
/*                                                                    */
/* OUTPUT-PARAMETER :  end: ptr to ptr to location after the          */
/*                          copied-to structure                       */
/*                                                                    */
/* RETURN-VALUE :                                                     */
/*                     D21_BAD_ARGUMENT                               */
/*                                                                    */
/* COMMENTS :  from may be NULL                                       */
/*             depends on attribute apdu structure                    */
/*                                                                    */
/* exoff **************************************************************/

static signed16 d21_ca3_attr_iput(
  Bool is_attr,
  D2_a_info * from,
  char * to,
  char ** end)
{
register signed16 i;
signed16 a_no_val;
register D23_at_type    *at_type;
register D23_av_info    *av_info;
register D23_av_value   *av_value;
signed16                     ret_value;

ret_value = D2_NO_ERR;

if (from == NULL)
	{
	*end = to;
	return(ret_value);
	}
				/* copy attr head info			*/
at_type = (D23_at_type *) to;
at_type->d23_atttag = D23_ATTTAG;
at_type->d23_atlen = from->d2_a_type.d2_typ_len;
at_type->d23_atoff = D2_ALIGN(at_type->d23_atlen);

to += D23_S_ATT;


memcpy (to, from->d2_a_type.d2_type, at_type->d23_atlen);

to += at_type->d23_atoff;

a_no_val = from->d2_a_no_val;

av_info = (D23_av_info *) to;

if (a_no_val == 0)
  {
  if (is_attr == TRUE)
    {
    ret_value = D21_BAD_ARGUMENT;
    }
  else
    {
    *end = to;
    }
  }
else
  {   if (a_no_val < 0)
	{			/* no attr values, same as 1 value of	*/
				/* return D2_ATTRIBUTE_MISSING          */
      ret_value = D21_BAD_ARGUMENT;
	}
    else {                      /* at least 1 value: 1st value must be  */
				/* treated differently			*/
      av_info->d23_avtag = D23_AVTAG;
      to += D23_S_AV;

      if ( from->d2_a_val->d2_a_rep == D2_ENUM )
	{
	av_info->d23_asyntx = D2_INT_LIST;
	av_info->d23_avnum  = 1 ;
	av_value = (D23_av_value *) to;
	av_value->d23_avlen = a_no_val * sizeof (signed32);
	av_value->d23_avoff = D2_ALIGN(av_value->d23_avlen);
	av_value->d23_avrep = av_info->d23_asyntx;
	av_value->d23_avfil = 0;
	to += D23_S_ATV;

				  /* now copy the values          */
	for (i=0 ; i < a_no_val ; i++ )
	  if ( (ret_value = d21_enum ( &(from->d2_a_val[i]), to, &to ))
		!= D2_NOERROR )
	    {
	    break;
	    }
	}
      else
	{
        av_info->d23_avnum = a_no_val;
        av_info->d23_asyntx = from->d2_a_val->d2_a_rep;

				  /* now copy the values          */
        for (i=0; i<a_no_val; i++)
	  {
	  ret_value = d21_ca5_attr_val_iput(&(from->d2_a_val[i]), to, &to, av_info->d23_asyntx);
	  if (ret_value != D2_NO_ERR)
	      break;
	   }
	}

				  /* set end parameter			*/
      *end = to;
  
    }       /* else */
  }

if (ret_value != D2_NOERROR)
  {
  ret_value = D21_BAD_ARGUMENT;
  }
return(ret_value);

} /* end of d21_ca3_attr_iput() */


d2_ret_val d21_ca5_attr_val_iput(
  D2_a_value * from,
  char * to,
  char ** end,
  unsigned16 syntax)
{
char * val;
register D23_av_value * av_value;
d2_ret_val ret_value;

ret_value = D2_NOERROR;

       if (from == NULL)
	 {
	  *end = to;
	  return(ret_value);
	  }

	av_value = (D23_av_value *) to;
	av_value->d23_avlen = from->d2_a_v_len;
	av_value->d23_avoff = D2_ALIGN(av_value->d23_avlen);
	av_value->d23_avrep = syntax;
	av_value->d23_avfil = 0;
	to += D23_S_ATV;
				/* a switch over all representations */
	switch (syntax)
	     {
	     case  D2_DISTNAME :
		   val = (char *)from->d2_at_value.d2_obj_name_a_v;
		   break;

	     case  D2_OBJ_IDENT:
		   val = (char *)from->d2_at_value.d2_obid_a_v;
		   break;

	     case  D2_PRINTABLE:
		   val = (char *)from->d2_at_value.d2_prt_a_v;
		   break;

	     case  D2_NUMERIC  :
		   val = (char *)from->d2_at_value.d2_num_a_v;
		   break;

	     case  D2_IA5  :
		   val = (char *)from->d2_at_value.d2_ia5_a_v;
		   break;

	     case  D2_BOOLEAN  :
		   val = (char *) &from->d2_at_value.d2_bool_a_v;
		   av_value->d23_avlen = sizeof(Bool);
		   av_value->d23_avoff = D2_ALIGN(av_value->d23_avlen);
		   break;

	     case  D2_INTEGER  :
		   val = (char *) &from->d2_at_value.d2_int_a_v;
		   av_value->d23_avlen = sizeof(signed32);
		   av_value->d23_avoff = D2_ALIGN(av_value->d23_avlen);
		   break;

	     case  D2_ACCL       :
	     case  D2_OCTET    :
		   val = (char *)from->d2_at_value.d2_oct_a_v;
		   break;

	     case  D2_TIME     :
		   val = (char *)from->d2_at_value.d2_utc_a_v;
		   break;

	     case  D2_GUIDE    :
		   if (d21_102_guide_to_ipc(from->d2_at_value.d2_guide_a_v, end)
					!= D2_NOERROR)
			{
			DUA_TRACE_ATTRIBUTE_PROBLEM_AT_IAPL(syntax)
			return(D2_ERROR);
			}
		   return(D2_NOERROR);

	     case  D2_POST_AD_STX:
	     case  D2_T61_PR_LIST:
		    if (d21_106_post_ad_to_ipc(from->d2_at_value.d2_post_a_v,
					end) != D2_NOERROR)
			{
			DUA_TRACE_ATTRIBUTE_PROBLEM_AT_IAPL(syntax)
			return (D2_ERROR);
			}
		    return(D2_NOERROR);

	     case  D2_T61_LIST :
	     case  D2_PRT_LIST :
		    d21_103_T61seq_to_ipc(from->d2_at_value.d2_t61lst_a_v, end);
		    return(D2_NOERROR);

	     case  D2_TTXID_STX :
		    d21_101_ttxid_to_ipc(from->d2_at_value.d2_ttxid_a_v, end);
		    return(D2_NOERROR);

	     case  D2_TLXNR_STX :
		    d21_104_telexnr_to_ipc(from->d2_at_value.d2_tlxnr_a_v, end);
		    return(D2_NOERROR);

	     case  D2_FAXNR_STX :
		    d21_105_faxnr_to_ipc(from->d2_at_value.d2_faxnr_a_v, end);
		    return(D2_NOERROR);

	     case  D2_PR_DM_STX :
	     case  D2_INT_LIST :
		   val = (char *)from->d2_at_value.d2_intlist_a_v;
		   break;

	     case  D2_PSAP_STX  :
		   val = (char *)from->d2_at_value.d2_psap_a_v;
		   break;

	     case  D2_T61        :
		   val = (char *)from->d2_at_value.d2_t61_a_v;
		   break;

	     case  D2_ASN1      :
		   val = (char *)from->d2_at_value.d2_asn1_a_v;
		   break;

	     case  D2_MHS_ORADR_STX :
		    if ((ret_value = d21_201_or_addr_to_ipc
			  (from->d2_at_value.d2_or_addr_av, end)) == D2_NOERROR)
				{
		    		return(D2_NOERROR);
				}
		    break;

	     case  D2_MHS_ORNAME_STX :
		    if ((ret_value = d21_202_or_name_to_ipc
			  (from->d2_at_value.d2_or_name_av, end)) == D2_NOERROR)
				{
		    		return(D2_NOERROR);
				}
		    break;

	     case  D2_MHS_DLSP_STX :
		    if ((ret_value = d21_203_sub_perm_to_ipc
			  (from->d2_at_value.d2_dl_s_p_av, end)) == D2_NOERROR)
				{
				return(D2_NOERROR);
				}
		    break;

	     case  D2_CERTIFICATE :
		    d21_510_certificate_to_apdu(from->d2_at_value.d2_cert_a_v,
							(char **)end);
		    return(D2_NOERROR);

	     case  D2_CERT_PAIR :
		    d21_511_cert_pair_to_apdu(from->d2_at_value.d2_ctpair_a_v,
							(char **)end);
		    return(D2_NOERROR);

	     case  D2_CERT_LIST :
		    d21_512_cert_list_to_apdu(from->d2_at_value.d2_ctlist_a_v,
							(char **)end);
		    return(D2_NOERROR);

	     default :            /* error illegal syntax   */
		   ret_value = D2_ERROR;

	} /* of switch */

	if (ret_value != D2_NOERROR)
	    {
	     *end = to;
	     DUA_TRACE_ATTRIBUTE_PROBLEM_AT_IAPL(syntax)
	     return(ret_value);
	    }

	memcpy (to, val,av_value->d23_avlen);

	to += av_value->d23_avoff;
	*end = to;
       return(ret_value);
}


/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca4_check_sc()                                 */
/*                                                                    */
/* AUTHOR       :  H. Volpers                                         */
/* DATE         :  23.4.91                                            */
/*                                                                    */
/* DESCRIPTION  :  check the service controls for update operations   */
/*                                                                    */
/* INPUT-PARAMETER :  com_arg:  common arguments                      */
/*                                                                    */
/* OUTPUT-PARAMETER :                                                 */
/*                                                                    */
/* exoff **************************************************************/

signed16 d21_ca4_check_sc(
  D2_c_arg * com_arg)
{
signed32 sc;

/*  check service controls  */
if (com_arg == NULL)
	       /* no service controls supplied, set errorcode  */
      return(D21_NO_SC);
else
      sc = com_arg->d2_serv_cntrls;

/*
Check if Service-Controls correct
*/
if ((sc & (D2_DUACACHE|D2_USEDSA)) == (D2_DUACACHE|D2_USEDSA)
    || (sc & (D2_DUACACHE|D2_USEDSA)) == 0 )
{
    return (D21_NO_SELECTION);
}

if (sc & D2_USEDSA)  {
    if ( ((sc & D2_NOCACHE) && (sc & D2_LOCALSCOPE)) ||
	 (!(sc & D2_NOCACHE) && !(sc & D2_LOCALSCOPE))  ) {
	     return(D21_NO_SELECTION);
	}
}

return (D2_NO_ERR);
}

static d2_ret_val d21_enum(
  D2_a_value * from,
  char * to,
  char ** end)
{
if (from->d2_a_rep != D2_ENUM ) return (D2_ERROR);
*((signed32 *)to) = from->d2_at_value.d2_int_a_v;
to += sizeof(signed32);
*end = to;
return (D2_NOERROR);
}
