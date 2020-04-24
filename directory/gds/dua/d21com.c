/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21com.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:25  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:53:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:52  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:28  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:28:25  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:40  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:31:52  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:11:34  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:53:50  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  10:14:47  marrek
 * 	Fixes for OT 6609.
 * 	[1993/02/02  09:52:17  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:47:13  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:24:48  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/27  21:11:05  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:36:58  marrek]
 * 
 * Revision 1.1.4.2  1992/10/05  09:54:06  marrek
 * 	Changes for Bug 5205.
 * 	[1992/10/05  09:52:52  marrek]
 * 
 * Revision 1.1.2.4  1992/07/06  18:55:13  melman
 * 	Added case D2_INT_LIST in switch in d21_ca4_attr_ilen_get().
 * 	Added casts in calls to strcpy and strlen of (char *) in d21_c30_get_sup().
 * 	[1992/07/06  18:54:27  melman]
 * 
 * Revision 1.1.2.3  1992/07/01  16:52:48  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:21:36  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:21:33  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:37:04  zeliff]
 * 
 * $EndLog$
 */

#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$Id: d21com.c,v 1.1.10.2 1996/02/18 18:16:51 marty Exp $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/* daton **************************************************************/
/*                                                                    */
/* NAME         :  d21com.c                                           */
/*                                                                    */
/* AUTHOR       :  J.Heigert                                          */
/* DATE         :  03.02.88                                           */
/*                                                                    */
/* DESCRIPTION  :  Auxiliary routines for attribute handling          */
/*                                                                    */
/* datoff *************************************************************/

#include <gds.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <d21dua.h>

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_c30_get_sup()                                  */
/*                                                                    */
/* AUTHOR       :  W. Schmid                                          */
/* DATE         :  25.10.90                                           */
/*                                                                    */
/* SYNTAX       :  void d21_c30_get_sup(obj_name, sup_name)           */
/*                 D2_name_string    obj_name;                        */
/*                 D2_name_string    sup_name;                        */
/*                                                                    */
/* DESCRIPTION  :  This function generates for a given object name    */
/*                 it's superior name.                                */
/*                                                                    */
/* INPUT-PARAMETER :                                                  */
/*                 D2_name_string    obj_name;                        */
/*                                                                    */
/* OUTPUT-PARAMETER :                                                 */
/*                 D2_name_string    sup_name;                        */
/*                                                                    */
/* RETURN-VALUE :  void                                               */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/

void d21_c30_get_sup(
  D2_name_string obj_name,
  D2_name_string sup_name)
{
D2_name_string rdn;

/* get DN of superior entry  */
strcpy((char *)sup_name, (char *)obj_name);

for (rdn = sup_name + strlen((char *)sup_name) - 1; 
		rdn > sup_name; rdn--)
{   if (*rdn == D2_RDN_SEP)
    {
	*rdn = D2_EOS;
	break;
    }
}
if (rdn == sup_name)    /* superior is ROOT  */
{   *rdn = D2_EOS;
}

}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca4_attr_ilen_get()                            */
/*                                                                    */
/* AUTHOR       :  J. Heigert,    Softlab GmbH.                       */
/* DATE         :  24.12.87                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca4_attr_ilen_get(attr_seq)               */
/*                    D23_at_info *attr_seq;                          */
/*                                                                    */
/* DESCRIPTION  :  calculate length needed to store given sequence    */
/*                 (in ipc-format) to D2_a_info                       */
/*                                                                    */
/* INPUT-PARAMETER :  attr_seq : ptr to attributes                    */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :  Depends on apdu-format for attributes (D23_at_info)    */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca4_attr_ilen_get(
  char ** attr_seq)
{
	D23_at_type  * ap;
	D23_av_value * vp;
	D23_av_info  * ip;
	signed32 d2_len=0;
	signed32 len;
	signed16 i;

	for (D2_EVER)       /* walk through the sequence */ {
		ap = (D23_at_type *) *attr_seq;

		if (ap->d23_atttag != D23_ATTTAG) {
			break;                         /* end of sequence */
		}

		d2_len+= ap->d23_atoff + sizeof(D2_a_info);
		*attr_seq+= D23_S_ATT + ap->d23_atoff;
		ip = (D23_av_info *) *attr_seq;

		if (ip->d23_avtag == D23_AVTAG) {
			*attr_seq+= D23_S_AV;
			d2_len += ip->d23_avnum * sizeof(D2_a_value);
			for (i=0;i < ip->d23_avnum;i++) {
				vp = (D23_av_value *) *attr_seq;
				if ((unsigned16)(vp->d23_avlen) < D23_MINTAG)
				{
				 switch(vp->d23_avrep){
				  case D2_GUIDE:
					len = d21_132_guide_struct_len(
						*attr_seq); break;

				  case D2_TTXID_STX: 
					len = d21_131_ttxid_struct_len( 
						*attr_seq); break;

				  case D2_TLXNR_STX: 
					len = d21_134_telexnr_struct_len(
					  *attr_seq); break;

				  case D2_FAXNR_STX:
					len = d21_135_faxnr_struct_len(
					  *attr_seq); break;

				  case D2_POST_AD_STX:
				  case D2_T61_PR_LIST:
					len = d21_136_post_ad_struct_len(
					 *attr_seq); break;

				  case D2_PRT_LIST:
				  case D2_T61_LIST: 
					len = d21_133_T61seq_struct_len(
					 *attr_seq); break;

				  case D2_MHS_ORADR_STX:
					len = d21_231_or_addr_struct_len(
					 *attr_seq); break;

				  case D2_MHS_ORNAME_STX:
					len = d21_232_or_name_struct_len(
					 *attr_seq); break;

				  case D2_MHS_DLSP_STX:
					len = d21_233_sub_perm_struct_len(
					 *attr_seq); break;

				  case D2_INT_LIST:
					len = (vp->d23_avoff/sizeof(signed32)) *
						sizeof(D2_a_value);
					break;

				  case D2_CERTIFICATE:
					len = d21_540_certificate_struct_len(
						(char *) *attr_seq);
					break;

				  case D2_CERT_PAIR:
					len = d21_541_cert_pair_struct_len(
						(char *) *attr_seq);
					break;

				  case D2_CERT_LIST:
					len = d21_542_cert_list_struct_len(
						(char *) *attr_seq);
					break;

				default: len = vp->d23_avoff;
			  } /* of switch */
			  d2_len+= len;
			  *attr_seq+= D23_S_ATV + vp->d23_avoff;
			 }
			} /* end for */
		} /* end if */
	} /* end for */
	return (d2_len);
}			 /* d21_ca4_attr_ilen_get() */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_len_ei_info()                               */
/*                                                                    */
/* AUTHOR       :  G.Loose                                            */
/* DATE         :  18.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_len_ei_info(ei_info)                    */
/*                    char *ei_info;                                 */
/*                                                                    */
/* DESCRIPTION  :  calculate length needed to store given sequence    */
/*                 (in ipc-format) in ptr-format                      */
/*                                                                    */
/* INPUT-PARAMETER :  ei_info : ptr to entry-information              */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :  Depends on apdu-format for attributes (D23_ei_info)    */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca_len_ei_info(
  char * en_info)
{
	D23_en_info  *ep;
	D23_ds_name  *dp;
	signed32         d2_len;

	d2_len = 0;

	for (D2_EVER)                 /* walk through the sequence */ 
	   {
		ep = (D23_en_info *) en_info;

		if (ep->d23_entag != D23_ENTTAG) {
			break;
		}

		/* end of sequence */

		d2_len+= D2_ALIGN(sizeof(D2_ei_info));
		en_info+= D23_S_EN;
		dp = (D23_ds_name *) en_info;

		if (dp->d23_dstag == D23_NAMTAG) {
			d2_len+= dp->d23_dsoff;                 /* length of D2_ds_name */
			en_info+= dp->d23_dsoff + D23_S_NM;
		}

		/* end if */

		d2_len+=D2_ALIGN(d21_ca4_attr_ilen_get(&en_info));
	}

	/* end for */

	return(d2_len);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_len_l_info()                                */
/*                                                                    */
/* AUTHOR       :  G.Loose                                            */
/* DATE         :  18.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_len_l_info(l_info)                      */
/*                    char *l_info;                                  */
/*                                                                    */
/* DESCRIPTION  :  calculate length needed to store given sequence    */
/*                 (in ipc-format) in ptr-format                      */
/*                                                                    */
/* INPUT-PARAMETER :  l_info : ptr to l-info                          */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :  Depends on apdu-format for attributes (D23_l_info)     */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca_len_l_info(
  char *l_info)
{
	D23_subord_info *sp;
	D23_ds_name     *dp;
	signed32             d2_len;

	d2_len = 0;

	for (D2_EVER)      /* walk through the sequence */ {
		sp = (D23_subord_info *) l_info;

		if (sp->d23_sotag != D23_SOITAG) {
			break;
		}

		/* end of sequence */

		d2_len+= sizeof(D2_l_info);
		l_info+= D23_S_SUBORD;
		dp = (D23_ds_name *) l_info;

		if (dp->d23_dstag == D23_NAMTAG)  /* length of D2_ds_name */ {
			d2_len+= dp->d23_dsoff;
			l_info+= dp->d23_dsoff + D23_S_NM;
		}

		/* end if */
	}

	/* end for */

	return(d2_len);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_len_ref_inf()                               */
/*                                                                    */
/* AUTHOR       :  G.Loose                                            */
/* DATE         :  18.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_len_ref_inf(ref_inf)                    */
/*                    char **ref_inf;                                */
/*                                                                    */
/* DESCRIPTION  :  calculate length needed to store given sequence    */
/*                 (in ipc-format) in ptr-format                      */
/*                                                                    */
/* INPUT-PARAMETER :  ref_inf : ptr to referral-informations          */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :  Depends on apdu-format for attributes (D23_ref_inf)    */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca_len_ref_inf(
  char ** ref_inf)
{
D23_rf_info  *rp;
D23_ds_name  *dp;
D23_acc_info *acp;
D23_av_value *avp;
signed32 d2_len;
signed16 i;

d2_len = 0;

for (D2_EVER)    /* walk through the sequence */
	{
		rp = (D23_rf_info *) *ref_inf;

		if (rp->d23_rftag != D23_REFTAG) {
			break;
		}

		/* end of sequence */

		d2_len+= sizeof(D2_ref_inf);
		*ref_inf+= D23_S_REF;

		dp = (D23_ds_name *) *ref_inf;

		if (dp->d23_dstag == D23_NAMTAG)        /* length of D2_ds_name */ {
			d2_len+= dp->d23_dsoff;
			*ref_inf+= dp->d23_dsoff + D23_S_NM;
		}

		/* end if */

			/* sequence of access points */
		for (i=0;i < rp->d23_nm_acc;i++) {
			acp = (D23_acc_info *) *ref_inf;

			if (acp->d23_acctag == D23_ACCTAG) {
				d2_len+= sizeof(D2_acc_point);
				*ref_inf+= D23_S_ACC;
				dp = (D23_ds_name *) *ref_inf;

 					/* length of D2_ds_name */
				if (dp->d23_dstag == D23_NAMTAG) {
					d2_len+= dp->d23_dsoff;
					*ref_inf+= dp->d23_dsoff + D23_S_NM;
				}

				/* end if */

				avp = (D23_av_value *) *ref_inf;
				d2_len+= avp->d23_avoff;  /* Psap_string */
				*ref_inf+= avp->d23_avoff + D23_S_ATV;
			} /* end if */
		} /* end for */
	} /* end for ever */

return(d2_len);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_len_error()                                */
/*                                                                    */
/* AUTHOR       :  G.Loose                                            */
/* DATE         :  18.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_len_error(error,e_type)                 */
/*                    char *error;                                   */
/*                     signed32  *e_type;                                  */
/*                                                                    */
/* DESCRIPTION  :  calculate length needed to store given sequence    */
/*                 (in ipc-format) in ptr-format                      */
/*                                                                    */
/* INPUT-PARAMETER :  error : ptr to D23_at_prob                      */
/*                    e_type: ptr to error_type                       */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :  Depends on apdu-format for attributes (D23_l_info)     */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca_len_error(
  char * error,
  signed16 * e_type)
{
	D23_rserror  *rsp;
	D23_ds_name  *dp;
	D23_at_prob  *ap;
	signed32          d2_len = 0;

	rsp = (D23_rserror *) error;
	d2_len+= sizeof(D2_error);

	error+= D23_S_RSERR;
	dp = (D23_ds_name *) error;

	switch (dp->d23_dstag) {
	case  D23_EOMTAG :
		       d2_len = 0;
		      *e_type = D2_NO_ERR;
		       break;
	case D23_NAMTAG :
		if ( dp->d23_dslen == 0 ) /* root matches in name error */
		  {
		  d2_len += sizeof (char);
		  }
		else
		  {
		  d2_len+= dp->d23_dsoff;
		  }

		error+= dp->d23_dsoff + D23_S_NM;
		ap = (D23_at_prob *) error;
		if (ap->d23_aptag == D23_APTAG)
		   {
			while (ap->d23_aptag == D23_APTAG) {
			   *e_type = D2_AT_PROBLEM;
			   d2_len+= sizeof(D2_a_error);
				  /* d2_e_problem = d2_e_atproblem */
			   error+= D23_S_ATPROB;
			   d2_len+= sizeof(D2_a_problem);
			   d2_len+= d21_ca4_attr_ilen_get(&error)-sizeof(D2_a_info)+sizeof(D2_ava);
			   ap = (D23_at_prob *) error;
			} /* end while */
		    }
		else
		    {
			*e_type = D2_NM_PROBLEM;
			d2_len+= sizeof(D2_n_error);
                                  /* d2_e_problem = d2_e_nmproblem */
		     }/* end if */
		break;
	case D23_REFTAG:
		*e_type = D2_REF_PROBLEM;       /* d2_e_problem = d2_e_referral */
		d2_len+= d21_ca_len_ref_inf(&error);
	      }

	return(d2_len);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_calc_av_info()                              */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_calc_av_info(parblk)                    */
/*                    D2_a_value *parblk;                             */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 attribute-value in DUA.                            */
/*                                                                    */
/* INPUT-PARAMETER :  parblk : ptr to parameter block                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca_calc_av_info(
  D2_a_value * value,
  signed32 no_val)
{
	signed32 d23_len,i,av_len;

	d23_len = D23_S_AV;

	if (value == NULL) {
	   return (d23_len);
	}

	for (i=0;i< no_val;i++)
	  {
	  if ((value + i)->d2_a_rep !=  D2_ENUM)
	    {
	    break;
	    }
	  }
	if (i == no_val)
	  {
	  d23_len += D23_S_ATV;
	  }
	for (i=0;i< no_val;i++) {
	  switch ((value+i)->d2_a_rep){
		case D2_GUIDE : av_len = d21_112_guide_msglen(
					  (value+i)->d2_at_value.d2_guide_a_v);
				(value+i)->d2_a_v_len = av_len;
				   break;
		case D2_TTXID_STX: av_len = d21_111_ttxid_msglen(
					  (value+i)->d2_at_value.d2_ttxid_a_v);
				(value+i)->d2_a_v_len = av_len;
				   break;
		case D2_TLXNR_STX: av_len = d21_114_telexnr_msglen(
					  (value+i)->d2_at_value.d2_tlxnr_a_v);
				(value+i)->d2_a_v_len = av_len;
				   break;
		case D2_FAXNR_STX: av_len = d21_115_faxnr_msglen(
					  (value+i)->d2_at_value.d2_faxnr_a_v);
				(value+i)->d2_a_v_len = av_len;
				   break;
		case D2_POST_AD_STX:
		case D2_T61_PR_LIST: av_len = d21_116_post_ad_msglen(
					 (value+i)->d2_at_value.d2_post_a_v);
				(value+i)->d2_a_v_len = av_len;
				   break;
		case D2_PRT_LIST:
		case D2_T61_LIST: av_len = d21_113_T61seq_msglen(
					 (value+i)->d2_at_value.d2_t61lst_a_v);
				(value+i)->d2_a_v_len = av_len;
				   break;
		case D2_CERTIFICATE:
		     av_len = d21_550_certificate_msg_len(
					(value+i)->d2_at_value.d2_cert_a_v);
		     break;
		case D2_CERT_PAIR:
		     av_len = d21_551_cert_pair_msg_len(
					(value+i)->d2_at_value.d2_ctpair_a_v);
		     break;
		case D2_CERT_LIST:
		     av_len = d21_552_cert_list_msg_len(
					(value+i)->d2_at_value.d2_ctlist_a_v);
		     break;
		case D2_MHS_ORADR_STX:
		      av_len = d21_211_or_addr_msglen(
				    (value+i)->d2_at_value.d2_or_addr_av);
		      (value+i)->d2_a_v_len = av_len;
		      break;
		case D2_MHS_ORNAME_STX:
		      av_len = d21_212_or_name_msglen(
				    (value+i)->d2_at_value.d2_or_name_av);
		      (value+i)->d2_a_v_len = av_len;
		      break;
		case D2_MHS_DLSP_STX:
		      av_len = d21_213_sub_perm_msglen(
				    (value+i)->d2_at_value.d2_dl_s_p_av);
		      (value+i)->d2_a_v_len = av_len;
		      break;
		case D2_INTEGER :
		      av_len = sizeof(signed32);
		      (value+i)->d2_a_v_len = av_len;
		      break;
		case D2_BOOLEAN :
		      av_len = sizeof(Bool);
		      (value+i)->d2_a_v_len = av_len;
		      break;

		default: av_len = (value+i)->d2_a_v_len;
		} /* of switch */
   	if (av_len != -1)
 		d23_len += D23_S_ATV + D2_ALIGN(av_len);
   	else
	 { d23_len = D2_ERROR; break; }

   } /* of for */

   return (d23_len);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_calc_carglen()                              */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_calc_carglen(parblk)                    */
/*                    D2_c_arg *parblk;                               */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 common arguments in DUA.                           */
/*                                                                    */
/* INPUT-PARAMETER :  parblk : ptr to parameter block                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca_calc_carglen(
  D2_c_arg * c_arg)
{
	signed32 d23_len;

	d23_len = D23_S_CARG;

	if (c_arg != NULL)
	    {
	    if (c_arg->d2_extension != NULL)
		{
		d23_len += D23_S_BIT;	/* not yet implemented */
		}
	    }
	return(D2_ALIGN(d23_len));
}


/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_calc_req_len()                              */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_calc_req_len(parblk)                    */
/*                    D2_req_info *parblk;                            */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 requested information in DUA.                      */
/*                                                                    */
/* INPUT-PARAMETER :  parblk : ptr to parameter block                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca_calc_req_len(
  D2_req_info * req_info,
  signed32 sc)
{
	signed32 d23_len;
	signed16 i;

	d23_len = D23_S_REQ;

	if ((sc & D2_USEDSA) && !(sc & D2_DONT_STORE))
	{ 
	d23_len += D23_S_ATT + D2_ALIGN(sizeof(D2_CACHE_ATTR)-1);
	}

	for (i=0;i< req_info->d2_r_no_at;i++) {
	d23_len += D23_S_ATT + D2_ALIGN((req_info->d2_r_type_at+i)->d2_typ_len);
	}

	return(D2_ALIGN(d23_len));
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_calc_atinfo_len()                           */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_calc_atinfo_len(parblk)                 */
/*                    D2_a_info  *parblk;                             */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 attribute information in DUA.                      */
/*                                                                    */
/* INPUT-PARAMETER :  parblk : ptr to parameter block                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca_calc_atinfo_len(
  D2_a_info * info)
{
	signed32 d23_len = 0, len;

	while (info != NULL) {
		if ((len=d21_ca_calc_av_info(info->d2_a_val,info->d2_a_no_val))
			== D2_ERROR) return(D2_ERROR);

		d23_len += D23_S_ATT + D2_ALIGN(info->d2_a_type.d2_typ_len)
		           + len;
		info = info->d2_a_next;
	}

	return(D2_ALIGN(d23_len));
}


/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca9_eom_iput()                                 */
/*                                                                    */
/* AUTHOR       :  J. Heigert,    Softlab GmbH.                       */
/* DATE         :  24.12.87                                           */
/*                                                                    */
/* SYNTAX       :  void d21_ca9_eom_iput(to, end)                     */
/*                    char *to,                                       */
/*                         **end;                                     */
/*                                                                    */
/* DESCRIPTION  :  put EOM-tag to specified location                  */
/*                                                                    */
/* INPUT-PARAMETER :  to: ptr to location to be copied to             */
/*                                                                    */
/* OUTPUT-PARAMETER :  end: ptr to ptr to location after the EOM-tag  */
/*                                                                    */
/* RETURN-VALUE :  none                                               */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/

void d21_ca9_eom_iput(
  char * to,
  char ** end)
{
	register D23_eom *eom_ptr;

	/* copy eom tag                         */
	eom_ptr = (D23_eom *) to;
	eom_ptr->d23_eomtag = D23_EOMTAG;
	/* set end parameter                    */
	*end = (char *) (eom_ptr + 1);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_len_inread()                                */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_len_inread(parblk)                      */
/*                    D21_p3 *parblk;                                 */
/*                                                                    */
/* DESCRIPTION  :  calculate length needed to store given pointer     */
/*                 in sequence                                        */
/*                                                                    */
/* INPUT-PARAMETER :  parblk : ptr to parameter block                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca_len_inread(
 D21_p3 * parblk)
{
	signed32 d23_len;
	signed32 sv;

	d23_len = D23_S_INR + D23_S_NM + d21_ca_calc_carglen(parblk->d21_3c_a);

	if (parblk->d21_3entry != NULL) {
		d23_len += D2_ALIGN(strlen ((char *) parblk->d21_3entry)+1);
	}

	if (parblk->d21_3c_a == NULL)
	    sv = 0;
	else
	    sv = parblk->d21_3c_a->d2_serv_cntrls;

	d23_len += d21_ca_calc_req_len(parblk->d21_3r_info, 
			       sv) + D23_S_EOM;

	return(D2_ALIGN(d23_len));
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_len_incompare()                             */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_len_incompare(parblk)                   */
/*                    D21_p6 *parblk;                                 */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 COMPARE REQUEST in DUA.                            */
/*                                                                    */
/* INPUT-PARAMETER :  parblk : ptr to parameter block                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca_len_incompare(
 D21_p6 * parblk)
{
	signed32 d23_len=0,len=0;

	d23_len = D23_S_INCOMP + D23_S_EOM + D23_S_NM
		  + d21_ca_calc_carglen(parblk->d21_6c_a);

	if (parblk->d21_6entry != NULL) {
		d23_len += D2_ALIGN(strlen ((char *) parblk->d21_6entry) + 1);
	}

	if ((len = d21_ca_calc_av_info(&parblk->d21_6comp->d2_avaval,1)) 
                       == D2_ERROR) return(D2_ERROR);

	d23_len += len + D23_S_AV + D23_S_ATT  
                   + parblk->d21_6comp->d2_avatype.d2_typ_len;
	return(D2_ALIGN(d23_len));
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_len_inremove()                              */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* SYNTAX       :  signed32 d21_ca_len_inremove(parblk)                    */
/*                    D21_p8 *parblk;                                 */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 REMOVE REQUEST in DUA.                             */
/*                                                                    */
/* INPUT-PARAMETER :  parblk : ptr to parameter block                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca_len_inremove(
  D21_p8 * parblk)
{
	signed32 d23_len;

	d23_len = D23_S_REMOBJ + D23_S_EOM + D23_S_NM
		  + d21_ca_calc_carglen(parblk->d21_8c_a);

	if (parblk->d21_8entry != NULL) {
		d23_len += D2_ALIGN(strlen ((char *) parblk->d21_8entry) + 1);
	}

	return(D2_ALIGN(d23_len));
}


