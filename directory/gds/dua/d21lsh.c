/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21lsh.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:46  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:07  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:58  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:32:21  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:31  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:28:40  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:06:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:33:14  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:20:03  marrek
 * 	time limit is exceeded also if max_time=curr_time.
 * 	[1993/08/03  09:22:30  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/30  10:55:50  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:47:44  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:25:15  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  09:56:02  marrek
 * 	Fix the time limit problem
 * 	[1992/12/16  10:21:26  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  21:32:00  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:38:41  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:53:01  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:21:52  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:26:15  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:37:40  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21lsh.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:05 $";
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
* TYPE         :  MODUL                                              *
*                                                                    *
* NAME         :  d21lsh.c                                           *
*                                                                    *
* AUTHOR       :  J.Forster                                          *
* DATE         :  22.04.88                                           *
*                                                                    *
* COMPONENT    :  Directory Service-V2 / LIST-Operation              *
*                                                                    *
* DOK.-REF.    :                                                     *
*                                                                    *
* PRD#/VERS.   :                                                     *
*                                                                    *
* DESCRIPTION  :  subroutines                                        *
*                                                                    *
* SYSTEM-DEPENDENCES:                                                *
*                                                                    *
* HISTORY      :                                                     *
*                                                                    *
* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   *
*    0.1   | 88-04-24 | Erstellung                  | jf |           *
*    1.2   | 88-08-30 | logging service-controls    | jf |           *
* datoff *************************************************************
*/

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <d21dua.h>

/* LOCAL FUNCTIONS */

static d2_ret_val d21_ls_realloc( char ** , char ** , char ** , signed32 ,
								D2_pbhead * );

/************************************************************************/

d2_ret_val d21_ls01_check_params(
  D21_p4 * parblk)
{
d2_ret_val           errval = D2_NOERROR;
signed32            sc;

do
{
/* check version */
if (!D2_CHECK_MINMAX(D21MIN1,D21MAX1,parblk->d21_4pbhead->d2_version))
	{			/* illegal version supplied		*/
	errval = D2_ILL_VERS;
	break;
	}
/* check service controls */
if (parblk->d21_4c_a == NULL)
	{
		  /* no service controls supplied, set errorcode  */
	errval = D21_NO_SC;
	break;
	}
else    {
	sc = parblk->d21_4c_a->d2_serv_cntrls;
	}

if ( (sc & D2_DUACACHE) || (sc & D2_DUAFIRST) )
  {
  errval = D21_SERV_CTRLS_INVALID;
  break;
  }

if (   ((sc & D2_NOCACHE) && !(sc & D2_USEDSA))
    || (!(sc & D2_USEDSA) && !(sc & D2_DUACACHE))  )
	{       /* sc1 */
	errval = D21_NO_SELECTION;
	break;
	}

if (!(sc & D2_NOCACHE) && (sc & D2_USEDSA) && (sc & D2_DUACACHE)
		       && (sc & D2_DUAFIRST))
	{  /* CACHE-FIRST not allowed !! */
	   errval = D21_NO_SELECTION;
	   break;
	}

if ((sc & D2_FILE_MODE) && (parblk->d21_4file == NULL))
       {
	errval = D21_BAD_ARGUMENT;
	break;
       }

if ( !(parblk->d21_4parent == NULL) &&
    (strlen((char *)(parblk->d21_4parent)) > D2_DNL_MAX))
	{
	errval = D21_BAD_NAME;
	break;
	}

if (parblk->d21_4cres == NULL)
	{
	errval = D21_BAD_ARGUMENT;
	break;
	}

/* valid cache-storage-class ? */
if (   !(sc & D2_DONT_STORE) && !(sc & D2_RESIDENT_CLASS) &&
       !(sc & D2_PRIVILEGED_CLASS) && !(sc & D2_NORMAL_CLASS)  )
	{
	if (sc & D2_USEDSA)
		{
		errval = D21_NO_SELECTION;
		}
	break;
	}
}
while (FALSE); /* end of do-while loop */

if (errval != D2_NOERROR)
	{
	if (errval != D2_ILL_VERS)
		{
		parblk->d21_4pbhead->d2_retcode = D2_CALL_ERR;
		parblk->d21_4pbhead->d2_errclass = D2_PAR_ERR;
		parblk->d21_4pbhead->d2_errvalue = errval;
		}
	else
		{
		parblk->d21_4pbhead->d2_retcode = D2_CALL_ERR;
		parblk->d21_4pbhead->d2_errclass = D2_ILL_VERS;
		parblk->d21_4pbhead->d2_errvalue = D2_NOT_SPEC;
		}
	return (D2_ERROR);
	}
else
	{
	return(D2_NOERROR);
	}

} /* end of check params */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_len_inlist()                                */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* SYNTAX       :  int d21_ca_len_inlist(parblk)                      */
/*                    D21_p4 *parblk;                                 */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 LIST REQUEST in DUA.                               */
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
/*                                                                    */

signed32 d21_ca_len_inlist(
    D21_p4 * parblk)
{
	signed32 d23_len;

	d23_len = D23_S_INL + D23_S_EOM + d21_ca_calc_carglen(parblk->d21_4c_a);

	if (parblk->d21_4parent != NULL) {
		d23_len += D23_S_NM
		+ D2_ALIGN(strlen ((char *) parblk->d21_4parent)+1);
	}
	else
	  d23_len += D23_S_NM;

	return(D2_ALIGN(d23_len));
}

void d21_ls03_build_req(
  D21_p4 * parblk,
  char * req,
  signed16 dir_id,
  signed16 bind_id)
{
char            *next = req;
D23_inlist      *mp_head;

/* first part of request */
mp_head = (D23_inlist *)req;
mp_head->d23_Ghdr.d23_versno = D23_APDUV22;
mp_head->d23_Ghdr.d23_dirid  = dir_id;
mp_head->d23_Ghdr.d23_bindid = bind_id;

next += D23_S_INL;
d21_cm03_build_comarg(parblk->d21_4c_a, &next);

/* move Distinguished name of parent entry */
if (parblk->d21_4parent != NULL)
    {   d21_cab_dsname_iput(parblk->d21_4parent,next,&next);
    }
   else
    {
     ((D23_ds_name *) next)->d23_dstag = D23_NAMTAG;
     ((D23_ds_name *) next)->d23_dscomplete = TRUE;
     ((D23_ds_name *) next)->d23_dslen = 0;
     ((D23_ds_name *) next)->d23_dsoff = 0;
     next += D23_S_NM;
    }


/* and eom-tag */
d21_ca9_eom_iput(next,&next);
} /* end of build request */

d2_ret_val d21_ls05_move_names_to_mem(
  D21_p4 * parblk,
  char * result,
  signed16 res_len,
  char ** next_mem,
  D23_rf_info ** ref_ptr,
  char ** link,
  signed16 * entries_found)
{
D2_pbhead       *pbhead;
char            *from = result;
char           *mem;
signed32            mem_len;
D2_l_info       *mem1;
D23_subord_info *sub_info;
D23_ds_name     *r_name;
D23_com_res     *cm_res ;
unsigned16          tag;
signed16          namlen;
signed16           entries;
Bool            analyze = TRUE;
Bool            first_res = FALSE;
signed32             i=0;

pbhead = parblk->d21_4pbhead;

/* calculate needed memory size */

entries = 0;

if (*link == NULL)
       {
	first_res = TRUE;
	}

/* distinguished name of parent node ? */
r_name = (D23_ds_name *)from;
tag    = r_name->d23_dstag;

if (tag == D23_BONTAG)
	{
	namlen = r_name->d23_dsoff;
	from = (char *)++r_name;
	if (first_res == TRUE)
	       {
		/* name of base_object will be set only in first pass !! */
		if ((parblk->d21_4dist_name = 
			D27_MALLOC(svc_c_sev_warning,namlen)) == NULL)
		{   pbhead->d2_retcode = D2_TEMP_ERR;
		    pbhead->d2_errclass = D2_SYS_ERR;
		    pbhead->d2_errvalue = D21_ALLOC_ERROR;
		    return(D2_ERROR);
		    /* no memory allocated by DUA, return immediately ! */
		  }
		else    { /* move DN */
		    strcpy((char *)(parblk->d21_4dist_name),from);
			 }
	       }
	/* next tag */
	from += namlen;
	}


sub_info = (D23_subord_info *) from;
tag = sub_info->d23_sotag;

if (tag == D23_SOITAG)
{
    mem_len   = d21_ca_len_l_info(from);

	     /* link last entry info except for first pass */
    if (*link !=  NULL)
    {
	 if (d21_ls_realloc((char **)&(parblk->d21_4res), link, next_mem,
				mem_len, parblk->d21_4pbhead) == D2_ERROR)
	 {
	      return(D2_ERROR);
	 }
	 mem1 = ((D2_l_info *)(*link))->d2_l_next = (D2_l_info *)(*next_mem);
	 mem = (*next_mem);
    }
    else
    {       /* set d21_4res and link in first pass */
	    /* Allocate memory for results of IAPL* */
	if ((mem = D27_MALLOC(svc_c_sev_warning,mem_len)) == NULL)
	{   pbhead->d2_retcode = D2_TEMP_ERR;
	    pbhead->d2_errclass = D2_SYS_ERR;
	    pbhead->d2_errvalue = D21_ALLOC_ERROR;
	    return(D2_ERROR);
			/* no memory allocated by DUA, return immediately ! */
	}
    *next_mem = mem + mem_len;
    mem1 = parblk->d21_4res = (D2_l_info *)mem;
    }
    mem += sizeof (D2_l_info);
}
else
{
     if ((tag == D23_CRSTAG) || (tag == D23_OQTAG))
     {    /* only referrals are returned */
	  analyze = FALSE;
     }
}

if (analyze == TRUE)
{    /* analyze result */
     for (i=1;;i++)
     {
	 mem1 ->d2_l_next = NULL;
	 mem1->d2_l_alias = (Bool) (sub_info->d23_soali);
	 mem1->d2_l_fromentry = (Bool) (sub_info->d23_somas);
	 from += D23_S_SUBORD;
	 r_name = (D23_ds_name *)from;
	 tag    = r_name->d23_dstag;

	 if (tag == D23_NAMTAG)   /* in subord info, name tag must come */
	 {
	      mem1->d2_l_rdn = (D2_name_string)mem;
	      strcpy(mem, (char *)(r_name+1));
	      mem += D2_ALIGN(r_name->d23_dslen);
	      from += r_name->d23_dsoff + D23_S_NM;
	  }
	  else
	  {
	      pbhead->d2_retcode  = D2_SYS_ERR;
	      pbhead->d2_errclass = D2_INT_ERR;
	      pbhead->d2_errvalue = D21_RESULTAPDU_INVALID;
	      return(D2_ERROR);       /* there must be an ds-tag !!! */
	   } /* end of namtag */

	   sub_info = (D23_subord_info *) from;
	   tag = sub_info->d23_sotag;
	   if (tag == D23_SOITAG)
	   {
	       mem1 = mem1->d2_l_next = (D2_l_info *)mem;
	       mem += sizeof(D2_l_info);
	   }
	   else
	   {   if ((tag == D23_CRSTAG) || (tag == D23_OQTAG))
	       {
		    mem1->d2_l_next = (D2_l_info *) NULL;
		    break;   /* from for ever */
	       }
	   }
     } /* end of analyze result */
*link = (char *)mem1;
*next_mem = mem;
}

entries += i;

/* Handle part-oq, if any */
*ref_ptr = NULL;

if (tag == D23_OQTAG)
  {
      parblk->d21_4uav_crit_ext = ((D23_part_oq *)from)->d23_oq_uncrit;
      parblk->d21_4lim_prob = ((D23_part_oq *)from)->d23_oq_limit;
      from += D23_S_POQ;
      if (((D23_rf_info *)from)->d23_rftag == D23_REFTAG)
      {
	    *ref_ptr = (D23_rf_info *)from;
	    d21_skip_referral(&from);
      }
   }

cm_res = (D23_com_res *) from;

if (first_res == TRUE && cm_res->d23_crtag == D23_CRSTAG)
{
/* First Search-result, copy common results */

  cm_res = (D23_com_res *) from;
  parblk->d21_4cres->d2_al_deref = cm_res->d23_ali_deref;

  /* performer is always NULL, as results are not signed */
  parblk->d21_4cres->d2_performer = NULL;
}


*entries_found += entries;

return(D2_NOERROR);

}  /* end of ls05_move_names_to_mem  */


signed32 d21_ls07_dsa_list(
  D21_p4 * parblk,
  char * req_ptr,
  signed32 req_len,
  char ** res_ptr,
  signed32 * res_len,
  d21_bind_info * bt_ptr)  /* bind table entry                     */
{
D21_req_info            req_info;
signed32                   res;

/* set ipc-request block */

req_info.d21_r_opcode = D23_LIST;
req_info.d21_r_bufapdu = req_ptr;
req_info.d21_r_lenapdu = req_len;
req_info.d21_r_ipcinfo = bt_ptr->d21_b_ipcinf;

/* send request to DSA */
res = d21_send_invoke(&req_info,bt_ptr->d21_b_usr_id,parblk->d21_4pbhead);
if (res != D2_NOERROR)
	{
	return(res);
	}

/* get result from DSA */

req_info.d21_r_bufapdu = *res_ptr;
req_info.d21_r_lenapdu = *res_len;

res = d21_recv_result(&req_info,bt_ptr->d21_b_usr_id,parblk->d21_4pbhead);

*res_ptr = req_info.d21_r_bufapdu;
*res_len = req_info.d21_r_lenapdu;

if (res != D2_NOERROR)
	{
	return (res);
	}

return (D2_NOERROR);

} /* end of ls07_dsa_list  */


d2_ret_val d21_ls08_handle_ref(
  D21_lb_info * lb_info,
  signed16 * lb_nr,
  D21_p4 * parblk,
  char * req_ptr,
  signed32 req_len,
  char ** res_ptr,
  signed32 * res_len,
  time_t maxtime,
  signed16 maxentries,
  char ** next_mem,
  D23_rf_info ** r_ptr,
  char ** link,
  signed16 * entries_found,
  d21_bind_info * bt_ptr)
{
time_t            curr_time;
d2_ret_val           ret_value = D2_NOERROR;
D23_rf_info     *ref_ptr = (D23_rf_info *)*r_ptr;
D2_pbhead       ref_pbhead;
				/* indicates whether search on a referral
				   DSA was at least one time successful or
				   not
				*/

ref_pbhead.d2_retcode = ref_pbhead.d2_errclass = ref_pbhead.d2_errvalue =
			D2_NO_ERR;
parblk->d21_4pbhead->d2_retcode  = parblk->d21_4pbhead->d2_errclass =
			    parblk->d21_4pbhead->d2_errvalue = D2_NO_ERR;

/*
   return referrals to application ?
*/
if ((parblk->d21_4lim_prob != D2_NO_LIMIT_EX) ||
    ((parblk->d21_4c_a->d2_serv_cntrls & D2_NOREFERRAL) != D2_NOREFERRAL ))
{   /* return referrals to application */

    if (d21_copy_referral (&ref_ptr, &parblk->d21_4rf, parblk->d21_4pbhead)
								 != D2_NOERROR)
    {
	/* set return values */
	ret_value = D2_ERROR;
    }
    ret_value = D2_NOERROR;
}
else
{
    /*
       start handling of referrals
    */
    while ((*r_ptr)->d23_rftag == D23_REFTAG)
      {
	/*
	   enough entries ? Is it really needed ? (duplicating DSA effort)
	*/
	if ( (maxentries != D2_S_UNLIMITED) &&
	     (*entries_found >= maxentries) )
	{   ret_value = D2_NOERROR;
	    parblk->d21_4lim_prob = D2_SIZE_LIMIT_EX;
    	    if (d21_copy_referral (&ref_ptr, &parblk->d21_4rf, 
				parblk->d21_4pbhead) != D2_NOERROR)
    	    {
		    /* set return values */
		    ret_value = D2_ERROR;
    	    }
	    break;
	}
	/*
	   enough time ? Is it really needed ? (duplicating DSA effort)
	*/
	curr_time = time(0L);
	if ((maxtime != D2_T_UNLIMITED) && (curr_time >= maxtime))
	{   /* set time_limit_exceeded and return */
	    parblk->d21_4lim_prob = D2_TIME_LIMIT_EX;
    	    if (d21_copy_referral (&ref_ptr, &parblk->d21_4rf, 
				parblk->d21_4pbhead) != D2_NOERROR)
    	    {
		    /* set return values */
		    ret_value = D2_ERROR;
    	    }
	    ret_value = D2_NOERROR;
	    break;
	}

	/*
	   set size-limit in request and send request to dsa
	*/
	if (maxentries != D2_S_UNLIMITED)
	  {
	  ((D23_com_arg *)(req_ptr+D23_S_HDR))->d23_cmctr.d23_scslm =
				maxentries - *entries_found;
	  }

	/*
	   follow the referral
	*/
	ret_value = d21_handle_ref(lb_info,lb_nr,r_ptr,
			&ref_pbhead,maxtime,
		       parblk->d21_4bind_id,D23_LIST,res_ptr,res_len,
		       req_ptr,req_len,bt_ptr);
	/*
	   analyze result of request
	*/
	if (ret_value != D2_NOERROR)
	{   /* error from dsa */
            ret_value = D2_NOERROR;
	    break;
	}
	if (ref_pbhead.d2_retcode == D2_NO_ERR &&
	    ref_pbhead.d2_errclass == D2_NO_ERR &&
	    ref_pbhead.d2_errvalue == D2_NO_ERR)
	{   /* DSA returned  result */
	    if ((ret_value = d21_ls05_move_names_to_mem (parblk,*res_ptr,
				      *res_len,next_mem,&ref_ptr,link,
				      entries_found)) != D2_NOERROR)
	    {   
		ret_value = D2_ERROR;
		break;
	    }
	    else    /* handle further referrals */
	       if (ref_ptr != NULL)
		  {
		    ret_value = d21_ls08_handle_ref(lb_info,lb_nr,parblk,
			req_ptr,req_len, res_ptr,res_len,maxtime,maxentries,
			next_mem,r_ptr, link,entries_found,bt_ptr);
		    if (ret_value == D2_ERROR)
			  break;
		    }
	 }
	else
	 break;
      } /* of while */

}
if (*entries_found == 0  &&
    parblk->d21_4pbhead->d2_retcode == D2_NO_ERR &&
    parblk->d21_4pbhead->d2_errclass == D2_NO_ERR &&
    parblk->d21_4pbhead->d2_errvalue == D2_NO_ERR &&
    (ref_pbhead.d2_retcode != D2_NO_ERR ||
     ref_pbhead.d2_errclass != D2_NO_ERR ||
     ref_pbhead.d2_errvalue != D2_NO_ERR))
 {   /*
	exchange error values from referral-DSA
     */
     parblk->d21_4pbhead->d2_retcode =  ref_pbhead.d2_retcode;
     parblk->d21_4pbhead->d2_errclass = ref_pbhead.d2_errclass;
     parblk->d21_4pbhead->d2_errvalue = ref_pbhead.d2_errvalue;
     ret_value = D2_ERROR;
   }

return(ret_value);

} /* end of d21_ls08_handle_ref() */

/* daton **************************************************************/
/*                                                                    */
/* NAME         :  d21_ls_realloc()                                   */
/*                                                                    */
/* AUTHOR       :   G.Loose                                           */
/* DATE         :   21.11.90                                          */
/*                                                                    */
/* SYNTAX :   int d21_ls_realloc(first_ptr,link,last_ptr,more_mem,    */
/*                               pbhead)                              */
/*                                                                    */
/* DESCRIPTION  :  this function reallocates memory for the l_info    */
/*                                                                    */
/* INPUT-PARAMETERS :                                                 */
/*             char  *first_ptr  pointer to subordinfo-structure     */
/*             char  *last_ptr   pointer to end of memory            */
/*             signed32    more_mem   additionally needed memory          */
/*             D2_pbhead *pbhead  pointer to pbhead-structure         */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*             char  *first_ptr  new pointer to the subordinfo       */
/*             char  *last_ptr   pointer to new first free address   */
/*                                behind the structure                */
/*                                                                    */
/*                                                                    */
/**********************************************************************/
/* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   */
/*          |          |                             |    |           */
/* datoff *************************************************************/

static d2_ret_val d21_ls_realloc(
  char ** first_ptr,
  char ** link,
  char ** last_ptr,
  signed32 more_mem,
  D2_pbhead * pbhead)
{
 char   *mem;
 signed32    offset;
 D2_l_info   *l_info;

 if ((mem = D27_REALLOC(svc_c_sev_warning,*first_ptr,
	   (size_t) (*last_ptr - *first_ptr + more_mem))) == NULL)
   {
      pbhead->d2_retcode   = D2_TEMP_ERR;
      pbhead->d2_errclass  = D2_SYS_ERR;
      pbhead->d2_errvalue  = D21_ALLOC_ERROR ;
      return(D2_ERROR);
   }
 else
   {
      offset = (signed32) (mem - *first_ptr);
      l_info = (D2_l_info *) mem;
      while (l_info != NULL)
      {
	  if (l_info->d2_l_rdn != NULL)
	  l_info->d2_l_rdn = (D2_name_string)
				((char *) l_info->d2_l_rdn + offset);

	  if (l_info->d2_l_next != NULL)
	  {
		l_info->d2_l_next =
			(D2_l_info *) ((char *) l_info->d2_l_next + offset);
	  }
	  else
	  {
		*link = (char *) l_info;
	  }
	  l_info = l_info->d2_l_next;
      }
   }
 *first_ptr = mem;
 *last_ptr  += offset;
 return(D2_NOERROR);
}
