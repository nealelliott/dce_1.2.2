/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21cph.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:31  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:53:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:57  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:53:52  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:31:54  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:30  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:28:35  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:05:53  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:32:18  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:14:25  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:54:35  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:15:19  marrek
 * 	Fix for OT 6616
 * 	insert <pthread.h>
 * 	[1993/02/02  09:19:31  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:47:26  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:24:57  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:17:16  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:37:37  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  03:21:49  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:37:16  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21cph.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:55 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         :  MODUL                                              */
/*                                                                    */
/* NAME         :  d21cph.c                                           */
/*                                                                    */
/* AUTHOR       :  J.Forster                                          */
/* DATE         :  4.1.88                                             */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <d21dua.h>

/************************************************************************/

d2_ret_val d21_cp01_check_params(
  D21_p6 *parblk)
{
signed16           errval = D2_NO_ERR;
signed32            sc;
const char function_name[] = "d21_cp01" ;

do
{
/* check version */
if (!D2_CHECK_MINMAX(D21MIN1,D21MAX1,parblk->d21_6pbhead->d2_version))
	{			/* illegal version supplied		*/
	errval = D2_ILL_VERS;
	break;
	}

/*  check service controls  */
d21_check_comarg(parblk->d21_6c_a, &errval, function_name, parblk->d21_6bind_id);

sc = parblk->d21_6c_a->d2_serv_cntrls;

/* valid DIRID for cache request ? */
if (!(sc &D2_NOCACHE) && !(sc & D2_USEDSA) && (sc & D2_DUACACHE)
       && (parblk->d21_6bind_id> D2CACHE_MAXDIR_ID))
	{
	errval = D21_C_MAXDIR;
	break;
	}

/*  valid source of information ?  */
if  (   !((sc & D2_DUACACHE) || (sc & D2_USEDSA))
      || ((sc & D2_NOCACHE) && !(sc & D2_USEDSA)) )
	{
	errval = D21_NO_SELECTION;
	break;
	}

if ( !(parblk->d21_6entry == NULL) &&
    ((int)strlen((char *)parblk->d21_6entry) > D2_DNL_MAX))
	{
	errval = D21_BAD_NAME;
	break;
	}

/* valid cache-storage-class ? */
if (   !(sc & D2_DONT_STORE) && !(sc & D2_RESIDENT_CLASS) &&
       !(sc & D2_PRIVILEGED_CLASS) && !(sc & D2_NORMAL_CLASS)  )
	{
	if (sc & D2_USEDSA)
		{
		errval = D21_NO_SELECTION ;
		break;
		}
	break;
	}
} 
while (FALSE); /* end of do-while loop */

if (errval != D2_NO_ERR)
	{
	if (errval != D2_ILL_VERS)
		{
		parblk->d21_6pbhead->d2_retcode = D2_CALL_ERR;
		parblk->d21_6pbhead->d2_errclass = D2_PAR_ERR;
		parblk->d21_6pbhead->d2_errvalue = errval;
		}
	else
		{
		parblk->d21_6pbhead->d2_retcode = D2_CALL_ERR;
		parblk->d21_6pbhead->d2_errclass = D2_ILL_VERS;
		parblk->d21_6pbhead->d2_errvalue = D2_NOT_SPEC;
		}
	return (D2_ERROR);
	}
else
	{
	return(D2_NOERROR);
	}

} /* end of check params */


d2_ret_val d21_cp03_build_req(
  D21_p6 * parblk,
  char * req,
  signed16 dir_id,
  signed16 bind_id )
{
char           *next = req;
D23_incompare   *mp_head;
D23_at_type     *mp_attype;
D23_av_info     *mp_avinfo;
signed16          at_len;
unsigned16             syntx;
D2_ava          *from;

/* first part of request */

mp_head = (D23_incompare *)req;
mp_head->d23_Khdr.d23_versno = D23_APDUV22;
mp_head->d23_Khdr.d23_dirid  = dir_id;
mp_head->d23_Khdr.d23_bindid = bind_id;

next += D23_S_INCOMP;
d21_cm03_build_comarg( parblk->d21_6c_a, &next);

/* move Distinguished name of entry */
d21_cab_dsname_iput(parblk->d21_6entry,next,&next);

/* move attribute-value-assertion */
mp_attype = (D23_at_type *)next;
from  = parblk->d21_6comp;

mp_attype->d23_atttag = D23_ATTTAG;
at_len = mp_attype->d23_atlen = from->d2_avatype.d2_typ_len;
mp_attype->d23_atoff = D2_ALIGN(at_len);
next += D23_S_ATT;
memcpy(next, from->d2_avatype.d2_type, at_len);
next += mp_attype->d23_atoff;

mp_avinfo = (D23_av_info *)next;
mp_avinfo->d23_avtag = D23_AVTAG;
mp_avinfo->d23_avnum = 1;
syntx = mp_avinfo->d23_asyntx = from->d2_avaval.d2_a_rep;
next += D23_S_AV;

/* move aval */
if (d21_ca5_attr_val_iput(&from->d2_avaval, next, &next, syntx) != D2_NOERROR)
  {
  return(D2_ERROR);
  }

/* and eom-tag */
d21_ca9_eom_iput(next,&next);

return(D2_NOERROR);
} /* end of build request */

d2_ret_val d21_cp04_cache_compare(
  D21_p6 * parblk,
  char * req_ptr,
  signed32 req_len,
  char ** res_ptr,
  signed32 * res_len )
{
D21_req_info            req_info;
d21_ipc_info     *ipc_info;
d2_ret_val                   res;

/* build association to CACHE  */

if (d21_bh4_i_bind(D23_DCASID,&ipc_info,parblk->d21_6pbhead) != D2_NOERROR)
	{
	/* final values in pbhead already set */
	return(D2_ERROR);
	}

/* set ipc-request block */
req_info.d21_r_opcode = D23_COMPARE;
req_info.d21_r_bufapdu = req_ptr;
req_info.d21_r_lenapdu = req_len;
req_info.d21_r_ipcinfo = ipc_info;

/* send request to cache */
if (d21_send_invoke(&req_info,ipc_info->d21_i_assid,
					parblk->d21_6pbhead) != D2_NOERROR)
	{
	/*
	final values in pbhead already set
	*/
	return(D2_ERROR);
	}

/* get result from cache */
req_info.d21_r_bufapdu = *res_ptr;
req_info.d21_r_lenapdu = *res_len;
res = d21_recv_result(&req_info,ipc_info->d21_i_assid,parblk->d21_6pbhead);

/* release association to cache immediatly */
d21_bh5_i_unbind(ipc_info->d21_i_assid);


*res_ptr = req_info.d21_r_bufapdu;
*res_len = req_info.d21_r_lenapdu;

if (res != D2_NOERROR)
	{
	return(D2_ERROR);
	}

return(D2_NOERROR);
}

d2_ret_val d21_cp05_set_result(
  D21_p6 * parblk,
  char * result)
{
char              *from = result;
D2_pbhead         *pbhead;
D23_rscompinfo    *res;
D23_ds_name       *res_name;
D23_com_res       *cm_res;
unsigned16            tag;
signed16            namlen;
char              *to;

/* initialize variables */
pbhead    = parblk->d21_6pbhead;

/* move match-sign */
res = (D23_rscompinfo *)from;
tag = res->d23_Ltag;
if (tag != D23_LTAG)
{       pbhead->d2_retcode  = D2_SYS_ERR;
	pbhead->d2_errclass = D2_INT_ERR;
	pbhead->d2_errvalue = D21_RESULTAPDU_INVALID;
	return(D2_ERROR);       /* there must be an comp-tag !!! */
}
else
{       parblk->d21_6match = res->d23_Lmatsgn;
	parblk->d21_6from_entry = res->d23_Lfrmen;
}

/* analyze other components of result-message */
from   += D23_S_COMRS;        /* point to next tag */
res_name = (D23_ds_name *) from;
tag = res_name->d23_dstag;

if (tag == D23_NAMTAG)
{
	namlen = res_name->d23_dslen;
	if ((to = D27_MALLOC(svc_c_sev_warning,namlen)) != NULL)
	{
	      parblk->d21_6dist_name = (D2_name_string) to;
	      strcpy(to, (char *)(res_name + 1));
	      from += res_name->d23_dsoff + D23_S_NM;
	}
	else
	{
	      pbhead->d2_retcode  = D2_SYS_ERR;
	      pbhead->d2_errclass = D2_INT_ERR;
	      pbhead->d2_errvalue = D21_ALLOC_ERROR;
	      return(D2_ERROR);
		    /* no memory allocated by DUA, return immediately ! */
	}
}
else
{
	parblk->d21_6dist_name = NULL;
}

cm_res = (D23_com_res *) from;
parblk->d21_6cres->d2_al_deref = cm_res->d23_ali_deref;

/* performer is always NULL, as results are not signed */
parblk->d21_6cres->d2_performer = NULL;

return(D2_NOERROR);

}  /* end of cp05_set_result  */

signed32 d21_cp06_dsa_compare(
  D21_p6 * parblk,
  char * req_ptr,
  signed32 req_len,
  char ** res_ptr,
  signed32 *res_len,
  d21_bind_info *bt_ptr)
{
D21_req_info            req_info;
signed32                   res;

/* set ipc-request block */

req_info.d21_r_opcode = D23_COMPARE;
req_info.d21_r_bufapdu = req_ptr;
req_info.d21_r_lenapdu = req_len;
req_info.d21_r_ipcinfo = bt_ptr->d21_b_ipcinf;

/* send request to dsa */

res=d21_send_invoke(&req_info,bt_ptr->d21_b_usr_id,parblk->d21_6pbhead);
if (res != D2_NOERROR)
	{
	return (res);
	}

/* get result from dsa */

req_info.d21_r_bufapdu = *res_ptr;
req_info.d21_r_lenapdu = *res_len;

res = d21_recv_result(&req_info,bt_ptr->d21_b_usr_id,parblk->d21_6pbhead);

*res_ptr = req_info.d21_r_bufapdu;
*res_len = req_info.d21_r_lenapdu;

if (res != D2_NOERROR)
	{
	return (res);
	}
return(D2_NOERROR) ;
}

