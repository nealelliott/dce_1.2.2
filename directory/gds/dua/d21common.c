/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21common.c,v $
 * Revision 1.2.10.2  1996/02/18  18:16:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:28  marty]
 *
 * Revision 1.2.10.1  1995/12/08  15:53:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:54  root]
 * 
 * Revision 1.2.8.3  1994/05/10  15:53:51  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:31:49  marrek]
 * 
 * Revision 1.2.8.2  1994/03/23  15:14:29  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:28:32  keutel]
 * 
 * Revision 1.2.8.1  1994/02/22  18:05:50  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:32:09  marrek]
 * 
 * Revision 1.2.6.2  1993/08/11  06:13:09  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:54:18  marrek]
 * 
 * Revision 1.2.4.4  1993/02/02  10:15:07  marrek
 * 	Fixes for OT 6609
 * 	insert <pthread.h>
 * 	introduce D2_IA5
 * 	[1993/02/02  09:12:45  marrek]
 * 
 * Revision 1.2.4.3  1992/12/31  19:47:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:24:53  bbelch]
 * 
 * Revision 1.2.4.2  1992/11/27  21:11:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:37:21  marrek]
 * 
 * Revision 1.2.2.3  1992/06/02  03:21:39  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:37:11  zeliff]
 * 
 * Revision 1.2.2.2  1992/04/27  19:56:51  melman
 * 	Files from Helmut's visit
 * 	[1992/04/27  19:51:51  melman]
 * 
 * Revision 1.2  1992/01/19  22:13:10  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21common.c,v $ $Revision: 1.2.10.2 $ $Date: 1996/02/18 18:16:53 $";
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
* NAME         :  d21_common.c                                       *
*                                                                    *
* AUTHOR       :  Albert Biber                                       *
* DATE         :  22.01.88                                           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <d21dua.h>

/********** GLOBAL **********/

#ifdef THREADSAFE
	pthread_key_t d21_old_cancel_key;
#else

/********** LOCAL **********/

	static volatile boolean d21_abandon_flag = FALSE;
#endif /* THREADSAFE */


static d2_ret_val d21_ca_copy_at_problem( char * , char * );

/**********************************************************************/

void d21_cm_cpyobj(
  D2_a_type * obj_id,
  char ** next)
{
D23_at_type     *mp_at = (D23_at_type *) *next;

      mp_at->d23_atttag = D23_ATTTAG;
      mp_at->d23_atlen = obj_id->d2_typ_len;
      mp_at->d23_atoff = D2_ALIGN(mp_at->d23_atlen);
      *next = (char *)(mp_at + 1);
      memcpy(*next, obj_id->d2_type, mp_at->d23_atlen);
      *next += mp_at->d23_atoff;
}

void d21_cm01_serv_ctrl(
  signed32 sc,
  signed16 * opmode)
{
  *opmode = D21_UNDEF;

  if ( !(sc & D2_NOCACHE) && !(sc & D2_USEDSA) && (sc & D2_DUACACHE) )
   {   *opmode = D21_CACHE_ONLY;
    }

  if (   ( (sc & D2_NOCACHE) && (sc & D2_USEDSA) )
      || (!(sc & D2_NOCACHE) && (sc & D2_USEDSA) && !(sc & D2_DUACACHE) )
      || (!(sc & D2_NOCACHE) && (sc & D2_USEDSA) && (sc & D2_DUACACHE)
	  && !(sc & D2_DUAFIRST) ))
   {   *opmode = D21_DSA_ONLY;
    }

  if (!(sc & D2_NOCACHE) && (sc & D2_USEDSA) && (sc & D2_DUACACHE)
			 && (sc & D2_DUAFIRST))
   {   *opmode = D21_CACHE_FIRST;
    }

  if (!(sc & D2_NOCACHE) && (sc & D2_USEDSA) && (sc & D2_DUACACHE)
			 && !(sc & D2_DUAFIRST))
   {   *opmode = D21_DSA_FIRST;
    }

}

void d21_check_comarg(
  D2_c_arg * c_arg,
  signed16 * errval,
  const char * function_name,
  signed16 bind_id)
{
signed32 sc;

/*  check service controls  */
if (c_arg == NULL)
	       /* no service controls supplied, set errorcode  */
	*errval = D21_NO_SC;
else    {
	if (!(((c_arg->d2_op_progress.d2_nr_phase == D2_PROCEEDING)
	         && (c_arg->d2_op_progress.d2_next_rdn >= 0)
	         && (c_arg->d2_op_progress.d2_next_rdn <= D2_NP_MAX))
	    || (c_arg->d2_op_progress.d2_nr_phase == D2_NOT_STARTED)
		   || (c_arg->d2_op_progress.d2_nr_phase == D2_COMPLETED)))
			*errval = D21_BAD_ARGUMENT;
	if (c_arg->d2_al_rdn < 0)
		*errval = D21_BAD_ARGUMENT;
	sc = c_arg->d2_serv_cntrls;
        if (!((c_arg->d2_priority == D2_PRIO_LOW) 
  	    || (c_arg->d2_priority == D2_PRIO_MEDIUM)
	    || (c_arg->d2_priority == D2_PRIO_HIGH)))
		*errval = D21_BAD_ARGUMENT;
	}
}

void d21_cm02_build_hdr( 
  D23_header * hdr,
  d21_bind_info * bt_ptr)
{
 if (hdr == NULL)
  {
    hdr->d23_versno = D23_APDUV22;
    hdr->d23_dirid  = D2_NOT_DEFINED ;
    hdr->d23_bindid = D2_NOT_DEFINED ;
   }
 else
  {
    hdr->d23_versno = D23_APDUV22;
    hdr->d23_dirid  = bt_ptr->d21_b_dir_id;
    hdr->d23_bindid = 0;
   }

}

/* DEFAULT values for common Arguments
  D2_extension   * extension;        NULL- by default
  D2_op_progress   op_progress;      default NameResolution not started
					     next_rdn - 0
  signed16             priority;         D2_MEDIUM
  signed16             time_limit;       D2_T_UNLIMITED
  signed16             size_limit;       D2_S_UNLIMITED
  signed16             scope_ref;        D2_WORLD_SCOPE
  Bool              chain_prohibited; TRUE by default
  Bool              prefer_chain;     FALSE by default
  Bool              d_ref_alias;      FALSE by default
  Bool              dont_use_copy;    FALSE by default
  Bool              local_scope;      FALSE by default
  Bool              asynchronous;     always FALSE
  Bool              auto_continue;    TRUE by default
  Bool              duafirst;         Private extension default FALSE
  Bool              resident_class;   Private extension default FALSE
  Bool              priv_class;       Private extension default FALSE
  Bool              normal_class;     Private extension default FALSE
  Bool              dont_store;       Private extension default TRUE
  Bool              cache;            Private extension -FALSE by Default
				       part of d21_1c_a sevice_controls
  Bool              usedsa;           Private extension -TRUE by Default
				       part of d21_1c_a sevice_controls

*/

void d21_cm03_build_comarg(
  D2_c_arg * c_arg,
  char ** req)
{
 D23_com_arg * comarg = (D23_com_arg *) *req;

  if (c_arg != NULL)
   {
     comarg->d23_cmtag = D23_CMTAG;
     comarg->d23_cmali = c_arg->d2_al_rdn;
     comarg->d23_cmctr.d23_scopt = c_arg->d2_serv_cntrls;
     comarg->d23_cmctr.d23_scref = c_arg->d2_scope_ref;
     comarg->d23_cmctr.d23_scpri = c_arg->d2_priority;
     comarg->d23_cmctr.d23_sctlm = c_arg->d2_time_limit;
     comarg->d23_cmctr.d23_scslm = c_arg->d2_size_limit;
     comarg->d23_cmopr.d23_opnm_rs = c_arg->d2_op_progress.d2_nr_phase;
     comarg->d23_cmopr.d23_op_nxtrdn = c_arg->d2_op_progress.d2_next_rdn;
    }
   else
   {
     comarg->d23_cmtag = D23_CMTAG;
     comarg->d23_cmali = 0;
     comarg->d23_cmctr.d23_scopt = D2_NOCHAINING | D2_NOREFERRAL |
				   D2_USEDSA     | D2_DONT_STORE;
     comarg->d23_cmctr.d23_scref = D2_WORLD_SCOPE ;
     comarg->d23_cmctr.d23_scpri = D2_PRIO_MEDIUM ;
     comarg->d23_cmctr.d23_sctlm = D2_T_UNLIMITED ;
     comarg->d23_cmctr.d23_scslm = D2_S_UNLIMITED ;
     comarg->d23_cmopr.d23_opnm_rs = D2_NOT_STARTED;
     comarg->d23_cmopr.d23_op_nxtrdn = 0;
    }
   /* Security parameter is not supported */
   /* requestor name is not supported by DUA */
   /* extensions are not supported by DUA */
  *req += D23_S_CARG;
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_cab_dsname_iput()                              */
/*                                                                    */
/* AUTHOR       :  J. Heigert,    Softlab GmbH.                       */
/* DATE         :  24.12.87                                           */
/*                                                                    */
/* SYNTAX       :  Void d21_cab_dsname_iput(from,to,end)              */
/*                    D2_name_string from;                            */
/*                    char *to,                                      */
/*                          **end;                                    */
/*                                                                    */
/* DESCRIPTION  :  copy dsname to specified location (in ipc-format)  */
/*                                                                    */
/* INPUT-PARAMETER :  from: ptr to source dsname (may be NULL)        */
/*                    to: ptr to destination location                 */
/*                                                                    */
/* OUTPUT-PARAMETER :  end: ptr to ptr to location after the last     */
/*                          copied byte (aligned)                     */
/*                                                                    */
/* RETURN-VALUE :  none                                               */
/*                                                                    */
/* COMMENTS : depends on apdu-format of ds-name                       */
/*                                                                    */
/* exoff **************************************************************/

void d21_cab_dsname_iput(
  D2_name_string from,
  char * to,
  char ** end)
{
register signed32	 	i;
register char 		*t;
register D23_ds_name	*name_ptr;

  name_ptr = (D23_ds_name *) to;
  name_ptr->d23_dstag = D23_NAMTAG;
  t = (char *) (name_ptr + 1);
  if (from == NULL)
	i = 0;
   else
      { i = strlen((char *)from) + 1;
	strcpy(t, (char *)from);
       }
  name_ptr->d23_dslen = i;
  *end = (t + D2_ALIGN(i));
  name_ptr->d23_dsoff = *end - t;
  name_ptr->d23_dscomplete = TRUE;
}	/* d21_cab_dsname_iput() */

boolean d21_ca_aval_iput(
  D2_a_value * atp,
  char ** from,
  char ** to)
{
 D23_av_value   * avp = (D23_av_value *) *from;
 register char * ttp = *to;
 register cpy = TRUE;

     if ((unsigned16)(avp->d23_avlen) < D23_MINTAG )
      {
       atp->d2_a_v_len = avp->d23_avlen;
       atp->d2_a_rep = avp->d23_avrep;
       switch (avp->d23_avrep)
       {
	 case  D2_DISTNAME :
	     atp->d2_at_value.d2_obj_name_a_v = (D2_name_string)ttp; break;

	 case  D2_OBJ_IDENT:
	    atp->d2_at_value.d2_obid_a_v = (Ob_id_string)ttp; break;

	 case  D2_PRINTABLE:
	    atp->d2_at_value.d2_prt_a_v = (Print_string) ttp; break;

	 case  D2_NUMERIC  :
	    atp->d2_at_value.d2_num_a_v = (Numeric_string) ttp; break;

	 case  D2_IA5  :
	    atp->d2_at_value.d2_ia5_a_v = (IA5_string) ttp; break;

	 case  D2_BOOLEAN  :
	    atp->d2_at_value.d2_bool_a_v = *((Bool *)(*from + D23_S_ATV)); 
	    cpy = FALSE; break;

	 case  D2_INTEGER  :
	    atp->d2_at_value.d2_int_a_v = *((signed32 *)(*from + D23_S_ATV)); 
	    cpy = FALSE; break;

	 case  D2_OCTET    :
	 case  D2_ACCL     :
	    atp->d2_at_value.d2_oct_a_v = (Octet_string)ttp; break;

	 case  D2_TIME     :
	    atp->d2_at_value.d2_utc_a_v = (UTC_time_string) ttp; break;

	 case  D2_GUIDE    :
	    atp->d2_at_value.d2_guide_a_v = (D2_guide *)ttp;
	    d21_122_guide_fr_ipc(from, to,TRUE);
	    return(TRUE);

	 case  D2_POST_AD_STX:
	 case D2_T61_PR_LIST:
	    atp->d2_at_value.d2_post_a_v = (D2_str_list *)ttp;
	    if (d21_126_post_ad_fr_ipc(from,to) != D2_NOERROR)
		{
		return (FALSE);
		}
	    return(TRUE);

	 case  D2_PRT_LIST :
	 case  D2_T61_LIST :
	    atp->d2_at_value.d2_t61lst_a_v = (D2_T61_seq *)ttp;
	    d21_123_T61seq_fr_ipc(from,to,TRUE);
	    return(TRUE);

	 case  D2_TTXID_STX :
	    atp->d2_at_value.d2_ttxid_a_v = (D2_ttxid *) ttp;
	    d21_121_ttxid_fr_ipc(from,to,TRUE);
	    return(TRUE);

	 case  D2_TLXNR_STX :
	    atp->d2_at_value.d2_tlxnr_a_v = (D2_telex_number *)ttp;
	    d21_124_telexnr_fr_ipc(from,to,TRUE);
	    return(TRUE);

	 case  D2_FAXNR_STX :
	    atp->d2_at_value.d2_faxnr_a_v = (D2_faxnumber *) ttp;
	    d21_125_faxnr_fr_ipc(from,to,TRUE);
	    return(TRUE);

	 case  D2_INT_LIST :
	    atp->d2_at_value.d2_intlist_a_v = (signed32 *)ttp; break;

	 case  D2_PSAP_STX  :
	    atp->d2_at_value.d2_psap_a_v = (Psap_string)ttp; break;

	 case  D2_CERTIFICATE:
	    atp->d2_at_value.d2_cert_a_v = (Certificate *)ttp;
	    d21_500_certificate_from_apdu((char **)from,(char **)to);
	    return(TRUE);

	 case  D2_CERT_PAIR  :
	    atp->d2_at_value.d2_ctpair_a_v = (Cert_pair *)ttp;
	    d21_501_cert_pair_from_apdu((char **)from,(char **)to);
	    return(TRUE);

	 case  D2_CERT_LIST  :
	    atp->d2_at_value.d2_ctlist_a_v = (Cert_list *)ttp;
	    d21_502_cert_list_from_apdu((char **)from,(char **)to);
	    return(TRUE);

	 case  D2_T61        :
	    atp->d2_at_value.d2_t61_a_v = (T61_string)ttp; break;

	 case  D2_ASN1      :
	    atp->d2_at_value.d2_asn1_a_v = (Asn1_string)ttp; break;

	 case  D2_MHS_ORADR_STX :
	    atp->d2_at_value.d2_or_addr_av = (D2_OR_address *) ttp;
	    d21_221_or_addr_fr_ipc(from,to,TRUE);
	    return(TRUE);

	 case  D2_MHS_ORNAME_STX :
	    atp->d2_at_value.d2_or_name_av = (D2_OR_name *) ttp;
	    d21_222_or_name_fr_ipc(from,to,TRUE);
	    return(TRUE);

	 case  D2_MHS_DLSP_STX :
	    atp->d2_at_value.d2_dl_s_p_av = (D2_dl_submit_perm *) ttp;
	    d21_223_sub_perm_fr_ipc(from,to,TRUE);
	    return(TRUE);

	 default :
	     DUA_TRACE_ATTRIBUTE_PROBLEM_IN_APDU(avp->d23_avrep)
	     return (FALSE);

       } /* of switch */

       *from += D23_S_ATV;
       if (cpy == TRUE) {
	memcpy(ttp, *from, avp->d23_avlen);
	*to += avp->d23_avoff;
	}
       *from += avp->d23_avoff;
       return(TRUE);
      } /* of if < MINTAG */

return(FALSE);
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca2_attr_iget()                                */
/*                                                                    */
/* AUTHOR       :  J. Heigert,    Softlab GmbH.                       */
/* DATE         :  20.01.88                                           */
/*                                                                    */
/* SYNTAX       :  Void d21_ca2_attr_iget(from, to, next_to)          */
/*                    D23_at_info *from;                              */
/*                    D2_a_info *to;                                  */
/*                    char *   *next_to;                             */
/*                                                                    */
/* DESCRIPTION  :  copy a sequence of attributes (in ipc-format)      */
/*                 to new location (in ptr-format)                    */
/*                                                                    */
/* INPUT-PARAMETER :  from: source location of attr-sequence          */
/*                    to: target location                             */
/*                                                                    */
/* RETURN-VALUE :  none                                               */
/*                                                                    */
/* COMMENTS :  Assumption: from is in correct form                    */
/*                         there is at least one attribute            */
/*             Depends on attribute-apdu-format!                      */
/*                                                                    */
/* exoff **************************************************************/

void d21_ca2_attr_iget(
  char ** from,
  D2_a_info * to,
  char ** next_to)
{
register signed16 i;
char  * ttp = *next_to, * ffp = *from;
D2_a_info       * tp;
D2_a_info       * old_tp = NULL;
D23_at_type     * fp;
D23_av_info     * favp;
signed16		num;

tp = to;
fp = (D23_at_type *) *from;

				/*					*/
				/* loop over attributes in sequence	*/
				/*					*/
   for (D2_EVER)
      {
      /*
      do not move D2_CACHE_ATTR
      */
      if (!memcmp(fp+1, D2_CACHE_ATTR, fp->d23_atlen))
	      {
	      ffp += D23_S_ATT + fp->d23_atoff;
		      /* skip over D2_CACHE_ATTR */
	      fp = (D23_at_type *) ffp;
	      if (fp->d23_atttag != D23_ATTTAG)
		      {
			if(old_tp != NULL)
			{
			    old_tp->d2_a_next = NULL;
			}
			break;  /* end of sequence -> exit */
		      }
	      }
      tp->d2_a_type.d2_typ_len = fp->d23_atlen;
      ffp += D23_S_ATT;
      tp->d2_a_type.d2_type = (Ob_id_string)(ttp + sizeof(D2_a_info));
      memcpy(tp->d2_a_type.d2_type, ffp, fp->d23_atlen);
      ffp += fp->d23_atoff;
      ttp += sizeof(D2_a_info) + D2_ALIGN(tp->d2_a_type.d2_typ_len);
      tp->d2_a_val = (D2_a_value *) ttp;
			   /* count attr values and copy attr lengths */
      favp = (D23_av_info *) ffp;
      if (favp->d23_avtag == D23_AVTAG)
       {           /* d23_avnum is the max no. of attr. values */
	 ffp += D23_S_AV;
	 if ( favp->d23_asyntx == D2_INT_LIST )
          {
	    num = ((D23_av_value *)ffp)->d23_avoff/sizeof(signed32);
	    ffp += D23_S_ATV;
	    ttp += num * sizeof(D2_a_value);
	    for ( i=0 ; i < num ; i++ , ffp += sizeof(signed32) )
	     {
	       (tp->d2_a_val+i)->d2_a_v_len = sizeof(signed32);
	       (tp->d2_a_val+i)->d2_a_rep = D2_ENUM;
	       (tp->d2_a_val+i)->d2_at_value.d2_int_a_v = *(signed32 *)ffp;
	     }
            tp->d2_a_no_val = num;
	  }
	 else
	  {
	    ttp += favp->d23_avnum * sizeof(D2_a_value);
	    for (i=0; i < favp->d23_avnum; i++)
	         {
	        if (d21_ca_aval_iput(tp->d2_a_val+i, &ffp, &ttp) != TRUE)
	         {
		   break;
	          } /* of if (aval_iput() != TRUE) */
	       } /* of for loop with avnum */
	      tp->d2_a_no_val = i ;
	  }
        }  /* of if for AVTAG */
       else
	{ tp->d2_a_no_val = 0;
	  tp->d2_a_val = (D2_a_value *) NULL;
	}
				/* move to next attribute in list	*/
	fp = (D23_at_type *) ffp;
	if (fp->d23_atttag == D23_ATTTAG)
		{		/* more attributes			*/
		tp->d2_a_next = (D2_a_info *) ttp;
		old_tp = tp;
		tp = tp->d2_a_next;
		}
	else	{		/* no more attributes, finish		*/
		tp->d2_a_next = NULL;
		break;		/* exit forever loop			*/
		}
      }        /* for ever */

*from = ffp;
*next_to = ttp;
}	/* d21_ca2_attr_iget() */

d2_ret_val d21_copy_referral(
  D23_rf_info ** res_ptr,
  D2_ref_inf ** ref_ptr,
  D2_pbhead * pbhead)
{
char * rf_ptr = (char *)*res_ptr;
signed32 mem_len;

    mem_len = d21_ca_len_ref_inf (&rf_ptr);
    /* Allocate memory for results of IAPL* */
    if ((*ref_ptr = D27_MALLOC(svc_c_sev_warning,mem_len)) == NULL)
    {   pbhead->d2_retcode = D2_TEMP_ERR;
	pbhead->d2_errclass = D2_SYS_ERR;
	pbhead->d2_errvalue = D21_ALLOC_ERROR;
	return(D2_ERROR);
		  /* no memory allocated by DUA, return immediately ! */
     }

    if (d21_cac_copy_referral ((char **)res_ptr, *ref_ptr) != D2_NOERROR)
    {
	/* set return values */
	pbhead->d2_retcode  = D2_PERM_ERR;
	pbhead->d2_errclass = D2_INT_ERR;
	pbhead->d2_errvalue = D21_RESULTAPDU_INVALID;
	return(D2_ERROR);
    }

    return(D2_NOERROR);
}

d2_ret_val d21_cac_copy_error(
  char * res_ptr,
  D2_pbhead * pbhead,
  D2_error ** error)
{
  signed32 mem_len;
  d2_ret_val res=D2_NOERROR;
  signed16 e_type=D2_NO_ERR;
  D23_rserror * rs_error = (D23_rserror *) res_ptr;
  char *from, *to;
  D23_ds_name * dsname;

  if (pbhead->d2_errvalue == D2_REF_ERROR ||
      pbhead->d2_errvalue == D2_ATT_ERROR ||
      pbhead->d2_errvalue == D2_NFOUND ||
      pbhead->d2_errvalue == D2_ON_ALIAS_NOT_ALLOWED ||
      pbhead->d2_errvalue == D2_NMSYNTAX_ERROR ||
      pbhead->d2_errvalue == D2_ALIAS_ERROR)
   {
    mem_len = d21_ca_len_error( res_ptr, &e_type);
    /* Allocate memory for results of IAPL* */
    if ((e_type != D2_NO_ERR) &&
	 (*error = D27_MALLOC(svc_c_sev_warning,mem_len)) == NULL)
     {   pbhead->d2_retcode = D2_TEMP_ERR;
	 pbhead->d2_errclass = D2_SYS_ERR;
	 pbhead->d2_errvalue = D21_ALLOC_ERROR;
	 return(D2_ERROR);
		/* no memory allocated by DUA, return immediately ! */
      }

    to = (char *) ((*error) +1);
    from = res_ptr + D23_S_RSERR;

    switch (e_type) {
	case D2_REF_PROBLEM :
	  (*error)->d2_e_type = e_type;
	  (*error)->d2_e_problem.d2_e_referral = (D2_ref_inf *) ((*error)+1);
	  res = d21_cac_copy_referral(&from,
				      (*error)->d2_e_problem.d2_e_referral);
	  break;

       case D2_NM_PROBLEM :    /* Name Error */
	  (*error)->d2_e_type = e_type;
	  (*error)->d2_e_problem.d2_e_nmproblem = (D2_n_error *)(to);
	  (*error)->d2_e_problem.d2_e_nmproblem->d2_ne_prob
						= rs_error->d23_Zerrval;
	  to += sizeof(D2_n_error);
	  (*error)->d2_e_problem.d2_e_nmproblem->d2_ne_match
                                                = (D2_name_string)to;
	  if ( ((D23_ds_name *)from)->d23_dslen == 0)
	    {
	    *to = '\0';
	    }
	  else
	    {
	    memcpy(to, (from+D23_S_NM), ((D23_ds_name *)from)->d23_dslen);
	    }
	  break;

       case D2_AT_PROBLEM :    /* Attribute Error */
	  (*error)->d2_e_type = e_type;
	  (*error)->d2_e_problem.d2_e_atproblem = (D2_a_error *)(to);
	  to += sizeof(D2_a_error);
	  (*error)->d2_e_problem.d2_e_atproblem->d2_ae_name = (D2_name_string)to;
	  dsname = (D23_ds_name *)from;
	  from += D23_S_NM;
	  memcpy(to, from, dsname->d23_dslen);
	  to += dsname->d23_dsoff;
	  from += dsname->d23_dsoff;
	  (*error)->d2_e_problem.d2_e_atproblem->d2_ae_prob = (D2_a_problem *)to;
	  res = d21_ca_copy_at_problem(to, from);
	  break;

       default: *error = (D2_error *) NULL;
	/* all other ERRORS require only pbhead */
	}
      return(res);
   }
   else
   {
    *error = (D2_error *)NULL;
    return(D2_NOERROR);
    }
}

static d2_ret_val d21_ca_copy_at_problem(
  char * to,
  char * from)
{
 D2_a_problem * atprob_to = (D2_a_problem *)to;
 D23_at_prob  * at_prob = (D23_at_prob *)from;
 D23_at_type * at_type;
 unsigned16       syntx;
 signed32 num , i;

    for (; ;)
       {
	    atprob_to->d2_ap_next = (D2_a_problem *) NULL;
	    atprob_to->d2_ap_prob = at_prob->d23_apval;
	    to += sizeof(D2_a_problem);
	    from += D23_S_ATPROB;
	    atprob_to->d2_ap_info.d2_a_type.d2_type = (Ob_id_string)to;
	    at_type = (D23_at_type *)from;
	    from += D23_S_ATT;
            memcpy(atprob_to->d2_ap_info.d2_a_type.d2_type,
					from, at_type->d23_atlen);
	    from += at_type->d23_atoff;
	    atprob_to->d2_ap_info.d2_a_type.d2_typ_len = at_type->d23_atlen;
	    to += at_type->d23_atoff;
	    atprob_to->d2_ap_info.d2_a_next = (D2_a_info *) NULL;
	    atprob_to->d2_ap_info.d2_a_no_val = 0;
	    if (((D23_av_info *)from)->d23_avtag == D23_AVTAG)
	    {
	     syntx = ((D23_av_info *)from)->d23_asyntx;
	     atprob_to->d2_ap_info.d2_a_val = (D2_a_value *) to;
	     from += D23_S_AV;
	     /* COPY D2_a_value NOW */

	    if ( syntx == D2_INT_LIST )
             {
	       num = ((D23_av_value *)from)->d23_avoff/sizeof(signed32);
	       from += D23_S_ATV;
	       to += num * sizeof(D2_a_value);
	       for ( i=0 ; i < num ; i++ , from += sizeof(signed32) )
	        {
	          (atprob_to->d2_ap_info.d2_a_val+i)->d2_a_v_len = sizeof(signed32);
	          (atprob_to->d2_ap_info.d2_a_val+i)->d2_a_rep = D2_ENUM;
	          (atprob_to->d2_ap_info.d2_a_val+i)->d2_at_value.d2_int_a_v = *(signed32 *)from;
	        }
                atprob_to->d2_ap_info.d2_a_no_val = num;
	     }
            else
             {
   
	        to += sizeof(D2_a_value);
	        if (d21_ca_aval_iput(atprob_to->d2_ap_info.d2_a_val,
				        &from, &to) != TRUE)
	         {
	          return (D2_ERROR);
	          } /* of if (aval_iput() != TRUE) */
	        else
	         {
		    atprob_to->d2_ap_info.d2_a_no_val = 1;
	          }
              }
	     }
	    else
	     {
	       atprob_to->d2_ap_info.d2_a_val = (D2_a_value *)NULL;
	     }
	     at_prob = (D23_at_prob *)from;

	  /* Next pointer is handled */

	     if (at_prob->d23_aptag == D23_APTAG)
	      {
	       atprob_to = atprob_to->d2_ap_next = (D2_a_problem *)to;
		}
	     else
	      {
	       atprob_to->d2_ap_next = (D2_a_problem *) NULL;
               break;
		}
        }
        return(D2_NOERROR);
}
	
/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_cac_copy_referral                              */
/*                                                                    */
/* AUTHOR       :  Albert Biber,  Softlab GmbH.                       */
/* DATE         :  04.02.87                                           */
/*                                                                    */
/* SYNTAX       :  signed16 d21_cac_copy_referral (from, to, to_len)       */
/*                   char     ** from;                               */
/*                   D2_ref_inf  *to;                                 */
/*                   signed32         to_len;                             */
/*                                                                    */
/* DESCRIPTION  :  copies the whole referral-information (in ipc-     */
/*                 block-format) to the a given location (in dua-     */
/*                 ptr-format).                                       */
/*                                                                    */
/* INPUT-PARAMETER :                                                  */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  D2_NO_ERR    function terminated successfully      */
/*                 D2_MEMORY_INSUFFICIENT                             */
/*                              the whole referral-information could  */
/*                              not be copied into the given buffer   */
/*                 D21_RESULTAPDU_INVALID                             */
/*                              from has wrong format                 */
/*                                                                    */
/* COMMENTS : depends on quite correct format of from-apdu!           */
/*                                                                    */
/* exoff **************************************************************/

signed16 d21_cac_copy_referral(
  char ** from,
  D2_ref_inf * to)
{
D23_rf_info  * rf_info;
D23_ds_name  * ds_name;
D2_acc_point * acc_to;
D23_av_value * av_ptr;
D23_acc_info * acc_from;
signed16 i;

if (((D23_rf_info *)*from)->d23_rftag != D23_REFTAG)
  {
  return(D21_RESULTAPDU_INVALID);
  }

for (;;)
  {
  rf_info = (D23_rf_info *)*from;

  to->d2_al_rdns = rf_info->d23_rf_alirdn;
  to->d2_rdn_res = rf_info->d23_rf_rdnres;
  to->d2_ref_type = rf_info->d23_rf_type;
  to->d2_op_progr.d2_nr_phase = rf_info->d23_op_pr.d23_opnm_rs;
  to->d2_op_progr.d2_next_rdn = rf_info->d23_op_pr.d23_op_nxtrdn;
  *from += D23_S_REF;
  ds_name = (D23_ds_name *) (*from);
  if (ds_name->d23_dstag != D23_NAMTAG)
	return(D21_RESULTAPDU_INVALID);
  *from += D23_S_NM;

	/*
	Copy TARGET-NAME
	*/
  to->d2_target = (D2_name_string) (to+1);
  strcpy ((char *)(to->d2_target), *from);
  *from += ds_name->d23_dsoff;
  to->d2_acc_point = acc_to = (D2_acc_point *)
			    ((char *)to->d2_target + ds_name->d23_dsoff);

  for (i=0; i< rf_info->d23_nm_acc; i++)
    {
	acc_from = (D23_acc_info *) (*from);
	if (acc_from->d23_acctag != D23_ACCTAG)
	    return(D21_RESULTAPDU_INVALID);
	*from += D23_S_ACC;
	ds_name = (D23_ds_name *) (*from);
	if (ds_name->d23_dstag != D23_NAMTAG)
		return(D21_RESULTAPDU_INVALID);

		/*
		Copy DSA-NAME
		*/
	acc_to->d2_dsa_ap = (D2_name_string) (acc_to+1);
	strcpy ((char *)(acc_to->d2_dsa_ap), (char *) (ds_name+1));

		/*
		Copy DSA-Address
		*/
	acc_to->d2_adr_ap = (Psap_string)((char *) (acc_to+1) + ds_name->d23_dsoff);
	*from += D23_S_NM + ds_name->d23_dsoff;
	av_ptr = (D23_av_value *) (*from);
	if (av_ptr->d23_avoff <= 0)
		return(D21_RESULTAPDU_INVALID);

	  /* copy PSAP ADDRESS */
	memcpy(acc_to->d2_adr_ap, (char *)(av_ptr+1), av_ptr->d23_avoff);
	*from = ((char *) (av_ptr+1)) + av_ptr->d23_avoff;
        if (i == rf_info->d23_nm_acc - 1)
         {
 	   acc_to->d2_next_ap = NULL;
          }
        else
         {
	   acc_to = acc_to->d2_next_ap = (D2_acc_point *)
		 (((char *)acc_to->d2_adr_ap) + av_ptr->d23_avoff);
         }
    }

  if (((D23_rf_info *) *from)->d23_rftag != D23_REFTAG)
    {
    to->d2_next_ref = NULL;
    break;
    }

  to = to->d2_next_ref = (D2_ref_inf *)
			(((char *)acc_to->d2_adr_ap) + av_ptr->d23_avoff);
  }

return (D2_NO_ERR);
}	/* d21_cac_copy_referal() */

void d21_skip_referral(
  char ** from)
{
   char       * scan = *from;
   D23_ds_name         * ds_name;

     while (((D23_rf_info *)scan)->d23_rftag == D23_REFTAG)
      {
       scan += D23_S_REF;
       ds_name = (D23_ds_name *)scan;
       if (ds_name->d23_dstag == D23_NAMTAG && ds_name->d23_dsoff != 0)
	  scan += ds_name->d23_dsoff + D23_S_NM;
       else
	  scan += D23_S_NM;
       while (((D23_acc_info *)scan)->d23_acctag == D23_ACCTAG)
       {
	  scan +=D23_S_ACC;
	  ds_name = (D23_ds_name *)scan;
	  if (ds_name->d23_dstag == D23_NAMTAG && ds_name->d23_dsoff != 0)
	     scan += ds_name->d23_dsoff + D23_S_NM;
	  else
	     scan += D23_S_NM;
	  scan += ((D23_av_value *)scan)->d23_avoff + D23_S_ATV;
       }
      }

     *from = scan;
}

#ifdef THREADSAFE

int
d21_old_cancel_state(void)
{
	pthread_addr_t	result;

	if (pthread_getspecific(d21_old_cancel_key,&result) != 0) {
		result = CANCEL_OFF; /* to be on the safe side */
	} /* endif */
	return (int)result;
} /* end d21_old_cancel_state */

void
d21_store_old_cancel_state(int old_state)
{
	assert (pthread_setspecific(d21_old_cancel_key,(void*)old_state) == 0);
} /* end d21_old_cancel_state */
#else /* THREADSAFE */

boolean
d21_abandon_is_indicated(void)
{
	return (d21_abandon_flag);
} /* end d21_abandon_is_indicated */

void
d21_reset_abandon_indication(void)
{
	d21_abandon_flag = FALSE;
} /* end d21_reset_abandon_indication */

void
d21_set_abandon_indication(void)
{
	d21_abandon_flag = TRUE;
} /* end d21_set_abandon_indication */

#endif /* THREADSAFE */
