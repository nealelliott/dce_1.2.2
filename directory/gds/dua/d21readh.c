/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21readh.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:13  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:25  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:54:04  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:32:36  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:34  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:28:53  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:42:23  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:34:59  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:31:41  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:58:30  marrek]
 * 
 * Revision 1.1.4.4  1993/02/02  10:16:08  marrek
 * 	insert <pthread.h>
 * 	use const declarations
 * 	[1993/02/02  09:24:29  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:48:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:25:51  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:35:46  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:41:21  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:58:53  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:39:20  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:29:11  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:37:58  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21readh.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:24 $";
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
* NAME         :  d21readh.c                                         *
*                                                                    *
* AUTHOR       :  J.Forster                                          *
* DATE         :  4.1.88                                             *
*                                                                    *
* COMPONENT    :  Directory Service-V2 / xxxx                        *
*                                                                    *
* DOK.-REF.    :  Design-Specification, Leistungsbeschreibung        *
*                                                                    *
* PRD#/VERS.   :                                                     *
*                                                                    *
* DESCRIPTION  :  Hilfsfunktionen fuer DS_READ                       *
*                                                                    *
* D2_SYSTEM-DEPENDENCES:                                             *
*                                                                    *
* HISTORY      :                                                     *
*                                                                    *
* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   *
*    1.1   | 88-01-04 | Erstellung                  | jf |           *
*    1.2   | 88-08-23 | message-reduction           | jf |           *
*    1.3   | 90-01-23 | new version V2.2            | as |           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <stdio.h>

#include <d21dua.h>

/*
**********************************************************************
*                                                                    *
*           D E F I N E S                                            *
*                                                                    *
**********************************************************************
*/

#define D2_DSC_READ (0 | D2_DUACACHE | D2_USEDSA | D2_DUAFIRST)

/************************************************************************/

d2_ret_val d21_rd01_check_params(
  D21_p3 *parblk)
{
signed16           errval = D2_NO_ERR;
signed32            sc;
const char function_name[] = "d21_rd01" ;

do
{
/* check version */

if (!D2_CHECK_MINMAX(D21V022,D21V022,parblk->d21_3pbhead->d2_version))
	{                       /* illegal version supplied             */
	errval = D2_ILL_VERS;
	break;
	}
/*  check service controls  */
/* no service controls supplied, set errorcode  */

d21_check_comarg(parblk->d21_3c_a,&errval,function_name,parblk->d21_3bind_id);
if (errval != D2_NOERROR) break;

sc = parblk->d21_3c_a->d2_serv_cntrls;
/* valid DIRID for cache request ? */

if (!(sc &D2_NOCACHE) && !(sc & D2_USEDSA) && (sc & D2_DUACACHE)
       && (parblk->d21_3bind_id> D2CACHE_MAXDIR_ID))
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

/* check other parameters */

if ( !(parblk->d21_3entry == NULL) &&
    (strlen((char *)(parblk->d21_3entry)) > D2_DNL_MAX))
	{
	errval = D21_BAD_NAME;
	break;
	}
if (parblk->d21_3r_info->d2_r_no_at < D2_ALL_ATT_REQ)
	{
	errval = D21_INV_R_NO_AT;
	break;
	}
if (parblk->d21_3r_info->d2_r_no_at > D2_AT_MAX)
	{
	errval = D21_R_NO_AT_TOO_BIG;
	break;
	}
if (! ((parblk->d21_3r_info->d2_r_val_at == D2_R_TYP)
     || (parblk->d21_3r_info->d2_r_val_at == D2_R_VAL_TYP)))
	{
	errval = D21_INV_R_VAL_AT;
	break;
	}
/*
valid cache-storage-class ?
*/
if (   !(sc & D2_DONT_STORE) && !(sc & D2_RESIDENT_CLASS) &&
       !(sc & D2_PRIVILEGED_CLASS) && !(sc & D2_NORMAL_CLASS)  )
	{
	if (sc & D2_USEDSA)
		{
		errval = D21_NO_SELECTION ;
		}
	}
}
while (FALSE); /* end of do-while loop */

if (errval != D2_NOERROR)
	{
	if (errval != D2_ILL_VERS)
		{
		parblk->d21_3pbhead->d2_retcode = D2_CALL_ERR;
		parblk->d21_3pbhead->d2_errclass = D2_PAR_ERR;
		parblk->d21_3pbhead->d2_errvalue = errval;
		}
	else
		{
		parblk->d21_3pbhead->d2_retcode = D2_CALL_ERR;
		parblk->d21_3pbhead->d2_errclass = D2_ILL_VERS;
		parblk->d21_3pbhead->d2_errvalue = D2_NOT_SPEC;
		}
	return (D2_ERROR);
	}

return(D2_NOERROR);
} /* end of check params */

signed32 d21_rd06_dsa_read(
  Bool	          adm_syntax,
  D21_p3          *parblk,
  char            *req_ptr,
  signed32           req_len,
  char            **res_ptr,
  signed32           *res_len,
  d21_bind_info  *bt_ptr  ) /* bind table entry          */
{
D21_req_info            req_info;
signed32                   res;

/*
set ipc-request block
*/
req_info.d21_r_opcode = adm_syntax? D23_READ + D2_ADM_OPID : D23_READ;
req_info.d21_r_bufapdu = req_ptr;
req_info.d21_r_lenapdu = req_len;
req_info.d21_r_ipcinfo = bt_ptr->d21_b_ipcinf;
/*
send request to DSA
*/
if((res = d21_send_invoke(&req_info,bt_ptr->d21_b_usr_id,
					parblk->d21_3pbhead)) != D2_NOERROR)
	{
	return (res);
	}
/*
get result from DSA in result area
*/
req_info.d21_r_bufapdu = *res_ptr;
req_info.d21_r_lenapdu = *res_len;
res = d21_recv_result(&req_info,bt_ptr->d21_b_usr_id,parblk->d21_3pbhead);
*res_ptr = req_info.d21_r_bufapdu;
*res_len = req_info.d21_r_lenapdu;

if (res != D2_NOERROR)
	{
	return (res);
	}

return (D2_NOERROR);
}

/* This function checks for no. of attr. requested to be equal to
   no. of attr. returned from cache
 */
d2_ret_val d21_rd07_check_attr(
  D21_p3          *parblk,
  char            *res_ptr,
  signed16           res_len )
{
d2_ret_val           ret_value;
signed16           j=0,nr_at,nr_match;
signed16 		i;
D2_a_type       *in_att;
D23_en_info     *en_ptr;
char            *h_ptr;
D23_at_type     *c_at_ptr;
D23_av_info     *avp;
D23_ds_name     * nm_ptr;

ret_value = D2_NOERROR;
nr_match  = 0;
nr_at = parblk->d21_3r_info->d2_r_no_at;

if (nr_at < 1)
	{       /* result definitly "complete" */
	ret_value = D2_NOERROR;
	}
else
	{
	in_att = parblk->d21_3r_info->d2_r_type_at;
	/* go to first attribute-information block in result  */
	en_ptr = (D23_en_info *)res_ptr;
	if (en_ptr->d23_entag != D23_ENTTAG)
		{       /* no entry info and thus no attr-info present */
		ret_value = D2_ERROR;
		}
	else
		{
		h_ptr    = (char *)++en_ptr;
		nm_ptr  = (D23_ds_name *) h_ptr;
		if (nm_ptr->d23_dstag == D23_NAMTAG)
		 {
		    h_ptr += D23_S_NM + nm_ptr->d23_dsoff;
		  }
		c_at_ptr = (D23_at_type *)h_ptr;
		 for (D2_EVER)
		       {
			 if (c_at_ptr->d23_atttag != D23_ATTTAG)
				 {    /* last attr. in result processed */
				 if (nr_at == nr_match)
				   {
				       ret_value = D2_NOERROR;
				    }
				 else
				   {
				       ret_value = D2_ERROR;
				    }
				 break;
				 }
			 if ((memcmp(c_at_ptr+1, in_att[j].d2_type,
				      in_att[j].d2_typ_len)) ||
			      (memcmp(in_att[j].d2_type , D2_CACHE_ATTR,
					in_att[j].d2_typ_len)))
				 {
				 nr_match++;
				 }

			 j+=1;
			 h_ptr += D23_S_ATT + c_at_ptr->d23_atoff;
			 avp = (D23_av_info *)h_ptr;
			 h_ptr += D23_S_AV;

			 /* look for next attr_info */
			 for (i=0; i< avp->d23_avnum; i++)
			  {
			  if ((unsigned16)(((D23_av_value *)h_ptr)->d23_avlen)
								< D23_MINTAG)
			       {
			       h_ptr += D23_S_ATV
				     + ((D23_av_value *)h_ptr)->d23_avoff;
			       }
			   }
			 c_at_ptr = (D23_at_type *) avp;

		       } /* end of for_D2_EVER_loop */
		}
	}

return(ret_value);
} /* end of d21_rd07_check_attr */

