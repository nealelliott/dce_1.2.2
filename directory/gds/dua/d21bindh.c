/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21bindh.c,v $
 * Revision 1.1.743.2  1996/02/18  18:16:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:21  marty]
 *
 * Revision 1.1.743.1  1995/12/08  15:53:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:48  root]
 * 
 * Revision 1.1.741.5  1994/07/06  15:07:16  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:15:21  marrek]
 * 
 * Revision 1.1.741.4  1994/06/21  14:46:39  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:38:59  marrek]
 * 
 * Revision 1.1.741.3  1994/05/10  15:53:49  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:46:50  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:31:39  marrek]
 * 
 * Revision 1.1.741.2  1994/03/23  15:14:26  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:28:17  keutel]
 * 
 * Revision 1.1.741.1  1994/02/22  17:38:36  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:31:34  marrek]
 * 
 * Revision 1.1.739.2  1993/10/14  17:27:29  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:25:35  keutel]
 * 
 * Revision 1.1.739.1  1993/10/13  17:32:01  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:40:58  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  06:09:38  marrek
 * 	Temporarily no atexit().
 * 	[1993/08/04  11:13:42  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/30  10:53:14  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  10:14:17  marrek
 * 	insert <pthread.h> and <D00_dua_protect.h>
 * 	fixes for OT 6565
 * 	[1993/02/02  08:13:46  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:46:59  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:24:42  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  18:07:05  marrek
 * 	November 1992 code drop
 * 	signal handling for d23_waitrs
 * 	[1992/12/04  11:02:29  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  21:10:37  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:36:27  marrek]
 * 
 * Revision 1.1.2.4  1992/07/01  16:52:43  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:21:29  melman]
 * 
 * Revision 1.1.2.3  1992/06/30  20:58:46  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:39:12  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:20:03  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:36:55  zeliff]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: d21bindh.c,v $ $Revision: 1.1.743.2 $ $Date: 1996/02/18 18:16:47 $";
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
* NAME         :  d21bindh.c                                         *
*                                                                    *
* AUTHOR       :  J.Heigert                                          *
* DATE         :  09.12.87                                           *
*                                                                    *
* COMPONENT    :  Directory Service-V2 / xxxx                        *
*                                                                    *
* PRD#/VERS.   :                                                     *
*                                                                    *
* DESCRIPTION  :  Low level bind / unbind functions                  *
*                                                                    *
* SYSTEM-DEPENDENCES:                                                *
*                                                                    *
* HISTORY      :                                                     *
*                                                                    *
* Vers.Nr. |   Date   |   Updates                   | KZ | CR# FM#   *
* datoff *************************************************************
*/

/*
**********************************************************************
*                                                                    *
*       I N C L U D E S                                              *
*                                                                    *
**********************************************************************
*/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <d21dua.h>
#include <gds_sec.h>

/*
**********************************************************************
*                                                                    *
*       D E K L A R A T I O N of data                                *
*                                                                    *
**********************************************************************
*/

Bool attached_to_ipc = FALSE;

				/* table for info on ipc associations	*/
d21_ipc_info  d21_ipc_tab[D23_MXCLTPROC];

				/* table for info on bind connections	*/
static d21_bind_info d21_bind_tab[D21_BIND_MAX];

/********** LOCAL FUNCTIONS ***********/

static signed32 d21_ca_len_inbind ( D2_a_value * , signed16 , signed32 );
static d21_ipc_info * d21_bh7_get_ipc_tab ( void );
static D2_credent * d21_ca6_cred_alloc ( D2_credent * );
static boolean d21_ca7_cred_cmp ( signed16 , D2_credent * ,
				  signed16 , D2_credent * ) ;
static boolean d21_caa_dsname_cmp ( D2_name_string , D2_name_string );
static d2_ret_val d21_bh6_i_wait (D2_pbhead *,d21_ipc_info *,D23_wtrspb *);

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_bh0_init_bind_handler()                        */
/*                                                                    */
/* AUTHOR       :  J.Heigert,     Softlab GmbH.                       */
/* DATE         :  15.01.88                                           */
/*                                                                    */
/* SYNTAX       :  void d21_bh0_init_bind_handler()                   */
/*                                                                    */
/* DESCRIPTION  :  initializes bind table                             */
/*                                                                    */
/* GLOBAL DATA (modified) :  d21_bind_tab[]                           */
/*                                                                    */
/* exoff **************************************************************/

void d21_bh0_init_bind_handler(void)
{
register signed16 i;
const char function_name[] = "d21_bh0_init_bind_handler";

				/* initialize bind-tab			*/
for (i=0; i<D21_BIND_MAX; i++)
	d21_bind_tab[i].d21_b_used = 0;

DUA_TRACE_BIND_TAB_INITIALIZED

}	/* d21_bh0_init_bind_handler() */

/*exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_bh7_get_ipc_tab()                              */
/*                                                                    */
/* AUTHOR       :  Jochen Keutel                                      */
/* DATE         :  26.02.93                                           */
/*                                                                    */
/* SYNTAX       :  d21_ipc_info *d21_bh7_get_ipc_tab()	      */
/*                                                                    */
/* DESCRIPTION  :  returns ptr to entry in ipc-table                  */
/*                                                                    */
/* INPUT-PARAMETER : none					      */
/*                                                                    */
/* RETURN-VALUE :  ptr to ipc-info                                    */
/*                                                                    */
/* GLOBAL DATA (used) :  d21_ipc_tab[]                                */
/*                                                                    */
/*exoff **************************************************************/

static d21_ipc_info *d21_bh7_get_ipc_tab(void)
{
d21_ipc_info *result;
signed16 i;
const char function_name[] = "d21_bh7_get_ipc_tab";

for (i=0; i<D23_MXCLTPROC; i++)
  {
  if (d21_ipc_tab[i].d21_i_used == FALSE)
    {
    result=&d21_ipc_tab[i];
    break;
    }
  } /* end of for */

if (i == D23_MXCLTPROC)
  {
  result = NULL;
  DUA_TRACE_IPC_TABLE_OVERFLOW
  }

return (result);
}	/*  d21_bh7_get_ipc_tab() */

/*exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_bh8_get_bind_info()                            */
/*                                                                    */
/* AUTHOR       :  Jochen Keutel				      */
/* DATE         :  15.03.93                                           */
/*                                                                    */
/* SYNTAX       :  d2_ret_val d21_bh8_get_bind_info(		              */
/*                    short bind_id,                                  */
/*		      d21_bind_info * bt_ptr,		      */
/*		      Bool * new_ipc,				      */
/*		      D2_pbhead *pbhead,			      */
/*		      Bool ipc_needed)				      */
/*                                                                    */
/* DESCRIPTION  :  The function returns copy of the entry in 	      */
/*		   bind-table corresponding to supplied bind_id.      */
/*		   If ipc_needed is false the ipc_used flag does not  */
/*		   change. If necessary it opens a new ipc connection.*/
/*		   This is necessary only if more than one thread     */
/*		   work on the same directory bind and is indicated   */
/*		   by new_ipc.					      */
/*                                                                    */
/* INPUT-PARAMETER :  bind-id,ipc_needed                              */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR if entry for bind_id exists	      */
/*                 D2_ERROR for invalid bind_id or if no ipc	      */
/*				connection available		      */
/*                                                                    */
/* GLOBAL DATA (used) :  d21_bind_tab[]                               */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21_bh8_get_bind_info(
  signed16 bind_id,
  d21_bind_info * bt_ptr,
  Bool * new_ipc,
  D2_pbhead * pbhead,
  Bool ipc_needed)
{
d2_ret_val ret_value = D2_NOERROR;
const char function_name[] = "d21_bh8_get_bind_info";

*new_ipc = FALSE;

if ((bind_id < 0) || (bind_id >= D21_BIND_MAX) ||
	(d21_bind_tab[bind_id].d21_b_used == 0))
  {
        /* bind_id out of range         */
  pbhead->d2_retcode = D2_CALL_ERR;
  pbhead->d2_errclass = D2_PAR_ERR;
  pbhead->d2_errvalue = D2_NO_BIND;
  return(D2_ERROR);
  } /* end if */

if (d21_bind_tab[bind_id].d21_b_ipcinf == NULL)
  {
        /* no ipc because of dummy bind to CDS before */
  pbhead->d2_retcode = D2_CALL_ERR;
  pbhead->d2_errclass = D2_PAR_ERR;
  pbhead->d2_errvalue = D21_NO_DSA_BIND;
  return(D2_ERROR);
  }

BEGIN_d21_bind_tab_LOCK

*bt_ptr = d21_bind_tab[bind_id];

if (ipc_needed == TRUE)
{
if (bt_ptr->d21_b_ipcinf->d21_i_aborted == TRUE)
  {
  pbhead->d2_retcode = D2_TEMP_ERR;
  pbhead->d2_errclass = D2_REM_ERR;
  pbhead->d2_errvalue = D2_NO_RESPONSE;
  ret_value = D2_ERROR;
  }
else
  {
  if (bt_ptr->d21_b_ipc_used == TRUE)
    {
    if (d21_bh4_i_bind(bt_ptr->d21_b_ipcinf->d21_i_servid,
			    &bt_ptr->d21_b_ipcinf,pbhead) != D2_NOERROR)
      {
      ret_value = D2_ERROR;
      }
    *new_ipc = TRUE;
    } /* if (ipc connection already used) */
  else
    {
    d21_bind_tab[bind_id].d21_b_ipc_used = TRUE;
    }
  } /* if not aborted */
} /* of ipc_needed == true */

END_d21_bind_tab_LOCK

return (ret_value);
}	/*  d21_bh8_get_bind_info() */

/*exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_bh4_i_bind()                                   */
/*                                                                    */
/* AUTHOR       :  J.Heigert,     Softlab GmbH.                       */
/* DATE         :  15.01.88                                           */
/*                                                                    */
/* SYNTAX       :  short d21_bh4_i_bind(                              */
/*		     short serv_id,				      */
/*                   d21_ipc_info ** ipc_ptr,                  */
/*                   D2_pbhead *pbhead)                               */
/*                                                                    */
/* DESCRIPTION  :  opens a ipc-association to given server            */
/*                                                                    */
/* INPUT-PARAMETER :  pbhead: ptr to std-head                         */
/*                                                                    */
/* OUTPUT-PARAMETER :  pbhead: error code                             */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR: no error                               */
/*                 D2_ERROR: error                                    */
/*                     (Service-Error will be set)                    */
/*                                                                    */
/* GLOBAL DATA (modified) :  d21_ipc_tab[]                            */
/*                                                                    */
/* USED FUNCTIONS :  d23_bind()                                       */
/*                                                                    */
/*exoff **************************************************************/

d2_ret_val d21_bh4_i_bind(
  signed16 serv_id,
  d21_ipc_info ** ipc_ptr,
  D2_pbhead *pbhead)
{
register d21_ipc_info * ii_ptr;
D23_bindpb i_bindpb;
d2_ret_val ret_value;
const char function_name[] = "d21_bh4_i_bind";

/* attach client if necessary */

ret_value = d21_att_cl (pbhead);

if (ret_value != D2_NOERROR)
  {
  return (D2_ERROR);
  }

BEGIN_d21_ipc_tab_LOCK 

ii_ptr = d21_bh7_get_ipc_tab();	

if (ii_ptr == NULL)
  {
  pbhead->d2_retcode  = D2_TEMP_ERR;
  pbhead->d2_errclass = D2_REM_ERR;
  pbhead->d2_errvalue = D2_NO_RESPONSE;
  ret_value = D2_ERROR;
  }
else				/* ii_ptr points to free ipc_tab entry	*/
  {
  i_bindpb.d23_4hdinfo.d2_version = D23_V02;
  i_bindpb.d23_4svrid = serv_id;
  if (d23_bind(&i_bindpb) == 0)
	{
			/* fill in ipc_tab entry		*/
	ii_ptr->d21_i_used = TRUE;
	ii_ptr->d21_i_servid = serv_id;
	ii_ptr->d21_i_assid = i_bindpb.d23_4assid;
	ii_ptr->d21_i_refms = i_bindpb.d23_4refms;
	ii_ptr->d21_i_lenms = i_bindpb.d23_4lenms;
	ii_ptr->d21_i_aborted = FALSE;

	*ipc_ptr = ii_ptr;
	ret_value = D2_NOERROR;
	}
  else
	{		/* error with ipc-bind, return error	*/
	DUA_TRACE_IPC_BIND_FAILED
	/* set : Service-Problem */
	pbhead->d2_retcode  = D2_TEMP_ERR;
	pbhead->d2_errclass = D2_REM_ERR;
	pbhead->d2_errvalue = D2_NO_RESPONSE;
	ret_value = D2_ERROR;
	}

  } /* end of if-else free ipc tab entry */
END_d21_ipc_tab_LOCK 

return (ret_value);
}	/* d21_bh4_i_bind() */

/*exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_bh5_i_unbind()                                 */
/*                                                                    */
/* AUTHOR       :  J.Heigert,     Softlab GmbH.                       */
/* DATE         :  15.01.88                                           */
/*                                                                    */
/* DESCRIPTION  :  closes a ipc-association                           */
/*                                                                    */
/* INPUT-PARAMETER :  ass_id: association-id                          */
/*                                                                    */
/* GLOBAL DATA (modified) :  d21_ipc_tab[]                            */
/*                                                                    */
/* USED FUNCTIONS :  d23_unbind()                                     */
/*                                                                    */
/*exoff **************************************************************/

void d21_bh5_i_unbind(
  signed16 ass_id )
{
register d21_ipc_info	*ii_ptr;
register d21_ipc_info    *max_ptr;
D23_ubndpb			i_ubndpb;
const char function_name[] = "d21_bh5_i_unbind";

BEGIN_d21_ipc_tab_LOCK

	/* look for element in IPC-table */
for (ii_ptr = &d21_ipc_tab[0],
	 max_ptr = &d21_ipc_tab[D23_MXCLTPROC]; ii_ptr < max_ptr; ii_ptr++)
    {   if (ii_ptr->d21_i_assid == ass_id)
	{   break ;
	}
    }
if (ii_ptr != max_ptr)
  {
                               /* release ipc-association              */
  if (ii_ptr->d21_i_used == TRUE && ii_ptr->d21_i_aborted == FALSE)
    {   i_ubndpb.d23_5hdinfo.d2_version = D23_V02;
	i_ubndpb.d23_5assid = ii_ptr->d21_i_assid;
	(void)d23_unbind(&i_ubndpb);
    }
  ii_ptr->d21_i_used = FALSE;
  }

END_d21_ipc_tab_LOCK

}	/* d21_bh5_i_unbind() */

/*
* exon ***************************************************************
*                                                                    *
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_bh1_exec_bind()                                *
*                                                                    *
* AUTHOR       :  J.Heigert,     Softlab GmbH.                       *
* DATE         :  15.12.87                                           *
*                                                                    *
* SYNTAX       :  Short d21_bh1_exec_bind (parblk,ref_bind)          *
*                   D21_p1 *parblk;                                  *
*                   Bool   ref_bind;                                 *
*                                                                    *
* DESCRIPTION  :  Eroeffnen einer Verbindung mit einem Directory     *
*                 System                                             *
*                                                                    *
* INPUT-PARAMETER : parblk                                           *
*                   ref_bind: TRUE: attempt bind also as anonymous   *
*                                   user                             *
*                                                                    *
* OUTPUT-PARAMETER : parblk                                          *
*                                                                    *
* RETURN-VALUE :  D2_NOERROR : ok				     *
*                 D2_ERROR : some error				     *
*                                                                    *
* exoff **************************************************************
*/

d2_ret_val d21_bh1_exec_bind (
   D21_p1 * parblk,
   Bool ref_bind)
{
register signed16            new_bid;
signed16                buf_len;
char                    *c_ptr;
signed16                   serv_id;
Bool                    try_anonym;
d21_bind_info	*bt_ptr;
d21_ipc_info	*ii_ptr;
D21_req_info		req_info;
D23_av_value            *i_av_value_ptr;
D23_inbind		*i_inbind_ptr;
D23_inbind		*i_inbind_ptr_hlp;
D23_rsbind		*res_bind_ptr;
D23_rserror		*res_err_ptr;
D23_pstatpb		  pstat_pb;
signed16                   dsa_name_len;
char                    *buffer;
D21_p2          unbd_parblk;
d2_ret_val           ret_value = D2_NOERROR;
char    sspec_field[100];

D2_cred_handle_t	cred_handle;
D2_sec_status	sec_status;
unsigned32		sec_ctx_id;
D2_str		sec_token;
D2_str		return_sec_token;


const char function_name[] = "d21_bh1_exec_bind";
unsigned16  contid;
D23_strcr_info * strcr_info;
D23_extcr_info * extcr_info;
D2_name_string target_dsa;
Bool master_bind = FALSE;
boolean pstate_set_to_bind_exists = FALSE;
boolean ipc_bind_exists = FALSE;

/* initialize "error-bindid" */
parblk->d21_1bind_id = -1;

/* set server-ID for request */
if (parblk->d21_1addr_dsa == NULL)
  {                             /* bind to coresident dsa requested     */
				/* compute correct server-id		*/
  serv_id = parblk->d21_1dir_id + D23_DS0SID;
  DUA_TRACE_BIND_TO_LOCAL_DSA(parblk->d21_1dir_id)
  }
else
  {                             /* bind to remote dsa requested         */
				/* go via C-stub			*/
  serv_id = D23_CSTSID;
  DUA_TRACE_BIND_TO_REMOTE_DSA
  }

BEGIN_d21_bind_tab_LOCK

/*
check if appropriate bind already exists (only if request is not for
CSTUB)
*/
if (serv_id != D23_CSTSID)
  {
  for (new_bid=0; new_bid<D21_BIND_MAX; new_bid++)
    {
    bt_ptr = &d21_bind_tab[new_bid];
    if (bt_ptr->d21_b_used > 0)
      {
      if ( (d21_c82_dsname_cmp(bt_ptr->d21_b_name_dsa,
			     parblk->d21_1name_dsa) == TRUE) &&
	   (d21_ca7_cred_cmp(bt_ptr->d21_b_auth_mech,
			     bt_ptr->d21_b_credent,
			     parblk->d21_1auth_mech,
			     parblk->d21_1u_credent) == TRUE)  &&
	   (bt_ptr->d21_b_dir_id == parblk->d21_1dir_id) )
	{
	break;
	}
      } /* end of if */
    } /* end of for loop */

  if (new_bid < D21_BIND_MAX)
    {                           /* matching entry found                 */
    d21_bind_tab[new_bid].d21_b_used++;
				/* return success			*/
    parblk->d21_1bind_id = new_bid;
    parblk->d21_1pbhead->d2_retcode = D2_NO_ERR;

    DUA_TRACE_USING_EXISTING_BIND_ID(new_bid)
	/*
	try to return name of dsa
	*/
    buffer = D27_MALLOC(svc_c_sev_warning,
				strlen((char *)bt_ptr->d21_b_name_dsa)+1);
    if ( buffer != NULL)
      {
      parblk->d21_1ret_dsa = (D2_name_string) buffer;
      strcpy((char *)parblk->d21_1ret_dsa,(char *)bt_ptr->d21_b_name_dsa);
      }
    ret_value = D2_NOERROR;
    goto do_unlock;
    } /* of if : matching entry found in bind tab */
  } /* of not C-Stub */

/*
appropriate bind does not exist, new bind to dsa necessary
	find free bind-tab entry
*/
for(new_bid=0;
	new_bid<D21_BIND_MAX && d21_bind_tab[new_bid].d21_b_used > 0;
								new_bid++)
  ;

if ( new_bid == D21_BIND_MAX)
  {		/* bind table overflow		*/
  parblk->d21_1pbhead->d2_retcode = D2_TEMP_ERR;
  parblk->d21_1pbhead->d2_errclass = D2_SYS_ERR;
  parblk->d21_1pbhead->d2_errvalue = D21_BINDS_TOO_MANY;
  DUA_TRACE_BIND_TABLE_OVERFLOW
  ret_value = D2_ERROR;
  goto do_unlock;
  }

				/* bt_ptr points to free bind_tab entry	*/
bt_ptr = &d21_bind_tab[new_bid];
DUA_TRACE_USING_NEW_BIND_ID(new_bid)

/* getting credentials.	*/
if ((ret_value = dua_acquire_cred(parblk->d21_1auth_mech, 
			  parblk->d21_1u_credent,&master_bind,&cred_handle, 
				  &sec_status)) == D2_ERROR)
	{
	parblk->d21_1pbhead->d2_retcode = sec_status.d2_retcode;
	parblk->d21_1pbhead->d2_errclass = sec_status.d2_errclass;
	parblk->d21_1pbhead->d2_errvalue = sec_status.d2_errvalue;
	goto do_unlock;
	}

/* getting security context.	*/
if ((ret_value = dua_init_sec_ctx(parblk->d21_1auth_mech, 
			  parblk->d21_1name_dsa,
			  parblk->d21_1dir_id,
			  parblk->d21_1addr_dsa == NULL? NULL:
				parblk->d21_1addr_dsa->d2_at_value.d2_psap_a_v,
			  parblk->d21_1addr_dsa == NULL? 0:
				parblk->d21_1addr_dsa->d2_a_v_len,
			  cred_handle, 
			  &sec_ctx_id,
			  &sec_token,
			  &sec_status)) == D2_ERROR)
	{
	parblk->d21_1pbhead->d2_retcode = sec_status.d2_retcode;
	parblk->d21_1pbhead->d2_errclass = sec_status.d2_errclass;
	parblk->d21_1pbhead->d2_errvalue = sec_status.d2_errvalue;
	goto do_unlock;
	}

				/* allocate memory for credentials      */
if (parblk->d21_1u_credent == NULL)
	{
	if (bt_ptr->d21_b_credent != NULL)
	  {
	  free(bt_ptr->d21_b_credent);
	  }
	bt_ptr->d21_b_credent = NULL;
	}
else
	{
	if (( bt_ptr->d21_b_credent =
	      d21_ca6_cred_alloc(parblk->d21_1u_credent)) == NULL)
		{
		parblk->d21_1pbhead->d2_retcode = D2_TEMP_ERR;
		parblk->d21_1pbhead->d2_errclass = D2_SYS_ERR;
		parblk->d21_1pbhead->d2_errvalue = D21_ALLOC_ERROR;
		ret_value = D2_ERROR;
		goto do_unlock;
		}
	}

ret_value = d21_bh4_i_bind(serv_id, &ii_ptr, parblk->d21_1pbhead) ;
if (ret_value != D2_NOERROR)
	{
	if (bt_ptr->d21_b_credent != NULL)
	  {
	  free(bt_ptr->d21_b_credent);
	  bt_ptr->d21_b_credent = NULL;
	  }
	ret_value = D2_ERROR;
	goto do_unlock;
	}

ipc_bind_exists = TRUE;
				/* ii_ptr points to ipc_tab entry       */
				/* send bind-request to dsa via ipc	*/

if (parblk->d21_1addr_dsa != NULL &&
    parblk->d21_1addr_dsa->d2_a_rep != D2_PSAP_STX)
  {
  d21_bh5_i_unbind(ii_ptr->d21_i_assid);
  ipc_bind_exists = FALSE;
  parblk->d21_1pbhead->d2_retcode = D2_CALL_ERR;
  parblk->d21_1pbhead->d2_errclass = D2_PAR_ERR;
  parblk->d21_1pbhead->d2_errvalue = D21_BAD_ARGUMENT;
  ret_value = D2_ERROR;
  goto do_unlock;
  }
				/* allocate space for apdu, at least	*/
				/* ipc-buffer size			*/
buf_len = d21_ca_len_inbind (parblk->d21_1addr_dsa, parblk->d21_1auth_mech,
							sec_token.d2_size);

if (ii_ptr->d21_i_lenms > buf_len)
	{
	buf_len = ii_ptr->d21_i_lenms;
	}
				/* heap space alloc will be freed within*/
				/* this function again			*/
if (((i_inbind_ptr = D27_MALLOC(svc_c_sev_warning,buf_len)) == NULL) ||
    ((res_bind_ptr = D27_MALLOC(svc_c_sev_warning,buf_len)) == NULL))
	{			/* alloc error				*/
        d21_bh5_i_unbind(ii_ptr->d21_i_assid);
  	ipc_bind_exists = FALSE;
	if (bt_ptr->d21_b_credent != NULL)
	  {
	  free(bt_ptr->d21_b_credent);
	  bt_ptr->d21_b_credent = NULL;
	  }
	if (i_inbind_ptr != NULL)
	  {
	  free(i_inbind_ptr);
	  i_inbind_ptr = NULL;
	  }
	parblk->d21_1pbhead->d2_retcode = D2_TEMP_ERR;
	parblk->d21_1pbhead->d2_errclass = D2_SYS_ERR;
	parblk->d21_1pbhead->d2_errvalue = D21_ALLOC_ERROR;
	ret_value = D2_ERROR;
	goto do_unlock;
	}

				/* fill in apdu for invoke(BIND)	*/
memset((void *)i_inbind_ptr,0,(size_t)buf_len);

c_ptr = (char *)i_inbind_ptr;

if (parblk->d21_1addr_dsa != NULL)
  {
  i_av_value_ptr = (D23_av_value *)c_ptr;
  i_av_value_ptr->d23_avlen = parblk->d21_1addr_dsa->d2_a_v_len;
  i_av_value_ptr->d23_avoff = D2_ALIGN(i_av_value_ptr->d23_avlen);
  i_av_value_ptr->d23_avrep = D2_PSAP_STX;
  i_av_value_ptr->d23_avfil = 0;

  c_ptr += sizeof(D23_av_value);

  memcpy (c_ptr,(char *)(parblk->d21_1addr_dsa->d2_at_value.d2_psap_a_v) ,
		    i_av_value_ptr->d23_avlen);
  c_ptr += i_av_value_ptr->d23_avoff;
  }

i_inbind_ptr_hlp = (D23_inbind *) c_ptr;

i_inbind_ptr_hlp->d23_Aversno = D23_APDUV22;
i_inbind_ptr_hlp->d23_Adirid = parblk->d21_1dir_id;

switch (parblk->d21_1auth_mech)
  {
  case D2_ANONYMOUS:
	i_inbind_ptr_hlp->d23_Acrkind = D23_NO_CRED;
	break;
  case D2_SIMPLE:
	i_inbind_ptr_hlp->d23_Acrkind = D23_SIMPLE_CRED;
	break;
  case D2_DCE_AUTH:
	i_inbind_ptr_hlp->d23_Acrkind = D23_EXTERNAL_CRED;
	break;
  case D2_STRONG:
	i_inbind_ptr_hlp->d23_Acrkind = D23_STRONG_CRED;
	break;
  default:
	d21_bh5_i_unbind(ii_ptr->d21_i_assid);
  	ipc_bind_exists = FALSE;
	if (i_inbind_ptr != NULL) free(i_inbind_ptr);
	if (res_bind_ptr != NULL) free(res_bind_ptr);
	parblk->d21_1pbhead->d2_retcode = D2_CALL_ERR;
	parblk->d21_1pbhead->d2_errclass = D2_PAR_ERR;
	parblk->d21_1pbhead->d2_errvalue = D21_BAD_ARGUMENT;
	DUA_TRACE_WRONG_CREDENTIAL_KIND(parblk->d21_1auth_mech)
	ret_value = D2_ERROR;
	goto do_unlock;
  } /* end of switch */

c_ptr = (char *)(i_inbind_ptr_hlp + 1);

switch (parblk->d21_1auth_mech)
  {
  case D2_ANONYMOUS:
	break;
  case D2_SIMPLE:
	((D23_simcr_info *) c_ptr)->d23_scrtag = D23_SIMTAG;
	c_ptr += sizeof(D23_simcr_info);
	break;
   case D2_DCE_AUTH:
	extcr_info = (D23_extcr_info *) c_ptr;
	extcr_info->d23_exttag = D23_EXTTAG;
	extcr_info->d23_extkind = D2_DCE_AUTH;
	extcr_info->d23_extlen = sec_token.d2_size;
	extcr_info->d23_extoff = D2_ALIGN(sec_token.d2_size);
	c_ptr += sizeof(D23_extcr_info);
	break;
  case D2_STRONG:
	strcr_info = (D23_strcr_info *) c_ptr;
	strcr_info->d23_strtag = D23_STRTAG;
	strcr_info->d23_strfil = 0;
	strcr_info->d23_strlen = sec_token.d2_size;
	strcr_info->d23_stroff = D2_ALIGN(sec_token.d2_size);
	c_ptr += sizeof(D23_strcr_info);
	break;
  default:
	d21_bh5_i_unbind(ii_ptr->d21_i_assid);
  	ipc_bind_exists = FALSE;
	if (i_inbind_ptr != NULL) free(i_inbind_ptr);
	if (res_bind_ptr != NULL) free(res_bind_ptr);
	parblk->d21_1pbhead->d2_retcode = D2_CALL_ERR;
	parblk->d21_1pbhead->d2_errclass = D2_PAR_ERR;
	parblk->d21_1pbhead->d2_errvalue = D21_BAD_ARGUMENT;
	DUA_TRACE_WRONG_CREDENTIAL_KIND(parblk->d21_1auth_mech)
	ret_value = D2_ERROR;
	goto do_unlock;
  } /* end of switch */

if (sec_token.d2_size > 0)
  {
  memcpy(c_ptr,sec_token.d2_value,sec_token.d2_size);
  }

c_ptr += D2_ALIGN(sec_token.d2_size);

d21_ca9_eom_iput(c_ptr,&c_ptr);

req_info.d21_r_opcode = D23_BIND;
req_info.d21_r_bufapdu = (char *) i_inbind_ptr;
req_info.d21_r_lenapdu = c_ptr - (char *) i_inbind_ptr;
req_info.d21_r_ipcinfo = ii_ptr;

/* disable some signals */
d21_disable_signals();
				/* send the invoke(BIND)		*/
if (d21_send_invoke(&req_info,ii_ptr->d21_i_assid,parblk->d21_1pbhead) !=
								D2_NOERROR)
	{			/* error				*/
	d21_bh5_i_unbind(ii_ptr->d21_i_assid);
  	ipc_bind_exists = FALSE;
	if (bt_ptr->d21_b_credent != NULL)
	  {
	  free(bt_ptr->d21_b_credent);
	  bt_ptr->d21_b_credent = NULL;
	  }
	free(i_inbind_ptr);
	i_inbind_ptr = NULL;
	/*
	final values in pbhead already set
	*/
	ret_value = D2_ERROR;
	goto do_unlock;
	}
/*
   set IPC-state
*/
sprintf(sspec_field, "%c%d", D23_RSTATE, D21_DIR_BIND_EXISTS);
pstat_pb.d23_Passid = ii_ptr->d21_i_assid;
pstat_pb.d23_Ppstate = sspec_field;
pstat_pb.d23_Phdinfo.d2_version = D23_V02;
(void)d23_set_pstate(&pstat_pb);
pstate_set_to_bind_exists = TRUE;

/* restore old signal state */
d21_enable_signals();

/*
wait for response
*/
req_info.d21_r_bufapdu = (char *) res_bind_ptr;
req_info.d21_r_lenapdu = buf_len;
req_info.d21_r_ipcinfo = ii_ptr;


if (d21_recv_bind_res(&req_info,parblk->d21_1pbhead,&contid) != D2_NOERROR)
	{			/* some error with ipc-wait		*/
	res_bind_ptr = (D23_rsbind *) req_info.d21_r_bufapdu;
	d21_bh5_i_unbind(ii_ptr->d21_i_assid);
  	ipc_bind_exists = FALSE;
	if (bt_ptr->d21_b_credent != NULL)
	  {
	  free(bt_ptr->d21_b_credent);
	  bt_ptr->d21_b_credent = NULL;
	  }
	free(i_inbind_ptr);
	free(res_bind_ptr);
	i_inbind_ptr = NULL;
	res_bind_ptr = NULL;
	/*
	final values in pbhead already set
	*/
	ret_value = D2_ERROR;
	goto do_unlock;
	}

res_bind_ptr = (D23_rsbind *) req_info.d21_r_bufapdu;

/* confirm and delete the security context.	*/
if (req_info.d21_r_opcode == D23_RREJECT)
	{
	return_sec_token.d2_size  = 0;
	return_sec_token.d2_value = NULL;
	}
else
	{
	switch(parblk->d21_1auth_mech)
		{
		case D2_ANONYMOUS:
			return_sec_token.d2_size = 0;
			return_sec_token.d2_value = NULL;
			break;
		case D2_SIMPLE:
			return_sec_token.d2_size = req_info.d21_r_lenapdu -
					(sizeof(D23_rsbind) + D23_S_SIMCR);
			return_sec_token.d2_value = (unsigned char *)
				((char *)(res_bind_ptr + 1) + D23_S_SIMCR);
			break;
		case D2_DCE_AUTH:
			return_sec_token.d2_size = 
			((D23_extcr_info *)(res_bind_ptr + 1))->d23_extlen;
			return_sec_token.d2_value = (unsigned char *)
				((char *)(res_bind_ptr + 1) + D23_S_EXTCR);
			break;
		case D2_STRONG:
			return_sec_token.d2_size = 
			((D23_strcr_info *)(res_bind_ptr + 1))->d23_strlen;
			return_sec_token.d2_value = (unsigned char *)
				((char *)(res_bind_ptr + 1) + D23_S_SSCR);
			break;
		default:
			DUA_TRACE_WRONG_CREDENTIAL_KIND(parblk->d21_1auth_mech)
			return_sec_token.d2_size = 0;
			return_sec_token.d2_value = NULL;
			break;
			
		}
	}

if (	((target_dsa = D27_MALLOC(svc_c_sev_warning,D2_DNL_MAX)) == NULL) ||
	((ret_value = dua_conf_sec_ctx(parblk->d21_1auth_mech, 
				  sec_ctx_id,
				  &return_sec_token,
				  &target_dsa,
			  	  &sec_status)) == D2_ERROR))
	{
	d21_bh5_i_unbind(ii_ptr->d21_i_assid);
  	ipc_bind_exists = FALSE;
	if (bt_ptr->d21_b_credent != NULL)
	  {
	  free(bt_ptr->d21_b_credent);
	  bt_ptr->d21_b_credent = NULL;
	  }
	free(i_inbind_ptr);
	free(res_bind_ptr);
	i_inbind_ptr = NULL;
	res_bind_ptr = NULL;

	parblk->d21_1pbhead->d2_retcode = sec_status.d2_retcode;
	parblk->d21_1pbhead->d2_errclass = sec_status.d2_errclass;
	parblk->d21_1pbhead->d2_errvalue = sec_status.d2_errvalue;
	goto do_unlock;
	}

try_anonym = FALSE;

if (req_info.d21_r_opcode == D23_RREJECT)
	{
	res_err_ptr = (D23_rserror *) req_info.d21_r_bufapdu;
	if ( (res_err_ptr->d23_Zerrval == D2_CREDENTIALS_INVALID) &&
			    ( ref_bind == TRUE ) )
		{                       /* BIND as anonymous below        */
		try_anonym = TRUE;   /* user-initiated BIND will never */
		}                       /* try BIND as anonymous, because */
					/* exec_bind is called with       */
					/* ref_bind == FALSE           */

	else	{			/* no bind as anonymous requested, */
					/* errval == D2_CRED_INVALID     */
		switch(res_err_ptr->d23_Zerrval)
		     {
		     case D2_TOO_BUSY:
			parblk->d21_1pbhead->d2_retcode = D2_TEMP_ERR;
			parblk->d21_1pbhead->d2_errclass = D2_INT_ERR;
			parblk->d21_1pbhead->d2_errvalue = D2_TOO_BUSY;
			break;
		     case D2_NO_RESPONSE:
			parblk->d21_1pbhead->d2_retcode = D2_TEMP_ERR;
			parblk->d21_1pbhead->d2_errclass = D2_REM_ERR;
			parblk->d21_1pbhead->d2_errvalue =
					       D2_NO_RESPONSE;
			break;
		     case D2_CREDENTIALS_INVALID:
		     case D2_NMSYNTAX_ERROR:
			parblk->d21_1pbhead->d2_retcode = D2_CALL_ERR;
			parblk->d21_1pbhead->d2_errclass = D2_PAR_ERR;
			parblk->d21_1pbhead->d2_errvalue =
						   D2_CREDENTIALS_INVALID;
			break;
		     default:
			parblk->d21_1pbhead->d2_retcode = D2_CALL_ERR;
			parblk->d21_1pbhead->d2_errclass = D2_PAR_ERR;
			parblk->d21_1pbhead->d2_errvalue = D2_NOT_SPEC;
			break;
		     }
		    d21_bh5_i_unbind(ii_ptr->d21_i_assid);
  		    ipc_bind_exists = FALSE;
		    if (bt_ptr->d21_b_credent != NULL)
	  	      {
	  	      free(bt_ptr->d21_b_credent);
	  	      bt_ptr->d21_b_credent = NULL;
	  	      }
		    free(i_inbind_ptr);
		    free(res_bind_ptr);
		    i_inbind_ptr = NULL;
		    res_bind_ptr = NULL;
		    ret_value = D2_ERROR;
		    goto do_unlock;
		}
	}

				/*					*/
				/* if requested, BIND as anonymous user	*/
				/*					*/
if (try_anonym)
	{		

	DUA_TRACE_TRY_BIND_AS_ANONYMOUS

	parblk->d21_1pbhead->d2_retcode =
	parblk->d21_1pbhead->d2_errclass =
	parblk->d21_1pbhead->d2_errvalue = D2_NO_ERR;

	memset((void *)i_inbind_ptr,0,(size_t)buf_len);
				/* fill in apdu for invoke(BIND)	*/

	c_ptr = (char *)i_inbind_ptr;

	if (parblk->d21_1addr_dsa != NULL)
	  {
	  i_av_value_ptr = (D23_av_value *)c_ptr;
	  i_av_value_ptr->d23_avlen = parblk->d21_1addr_dsa->d2_a_v_len;
	  i_av_value_ptr->d23_avoff = D2_ALIGN(i_av_value_ptr->d23_avlen);
	  i_av_value_ptr->d23_avrep = D2_PSAP_STX;
	  i_av_value_ptr->d23_avfil = 0;

	  c_ptr += sizeof(D23_av_value);

	  memcpy (c_ptr,
		(char *)(parblk->d21_1addr_dsa->d2_at_value.d2_psap_a_v) ,
		    i_av_value_ptr->d23_avlen);
	  c_ptr += i_av_value_ptr->d23_avoff;
	  }

	i_inbind_ptr_hlp = (D23_inbind *) c_ptr;
	i_inbind_ptr_hlp->d23_Aversno = D23_APDUV22;
	i_inbind_ptr_hlp->d23_Adirid = parblk->d21_1dir_id;
	i_inbind_ptr_hlp->d23_Acrkind = D23_NO_CRED;

	c_ptr = (char *)(i_inbind_ptr_hlp + 1);
	d21_ca9_eom_iput(c_ptr,&c_ptr);


	req_info.d21_r_opcode = D23_BIND;
	req_info.d21_r_bufapdu = (char *) i_inbind_ptr;
	req_info.d21_r_lenapdu = c_ptr - (char *) i_inbind_ptr;
	req_info.d21_r_ipcinfo = ii_ptr;
				/* send the invoke(BIND)		*/
	if (d21_send_invoke(&req_info,ii_ptr->d21_i_assid,
					parblk->d21_1pbhead) != D2_NOERROR)
		{			/* error		*/
		d21_bh5_i_unbind(ii_ptr->d21_i_assid);
  		ipc_bind_exists = FALSE;
		free(i_inbind_ptr);
		free(req_info.d21_r_bufapdu);
		i_inbind_ptr = NULL;
		req_info.d21_r_bufapdu = NULL;
		/*
		  final values in pbhead already set
		*/
		ret_value = D2_ERROR;
		goto do_unlock;
		}

				/*					*/
				/* wait for response			*/
				/*					*/
	req_info.d21_r_ipcinfo = ii_ptr;
	req_info.d21_r_bufapdu = (char *) res_bind_ptr;
	req_info.d21_r_lenapdu = buf_len;

	if (d21_recv_bind_res(&req_info,parblk->d21_1pbhead,&contid) != 0)
		{			/* some error with ipc-wait */
		d21_bh5_i_unbind(ii_ptr->d21_i_assid);
  		ipc_bind_exists = FALSE;
		free(i_inbind_ptr);
		free(req_info.d21_r_bufapdu);
		/*
		  final values in pbhead already set
		*/
		ret_value = D2_ERROR;
		goto do_unlock;
		}
					/* check if anonymous BIND ok   */
	if (req_info.d21_r_opcode == D23_RREJECT)
		{
					/* return error			*/
		res_err_ptr = (D23_rserror *) req_info.d21_r_bufapdu;
		parblk->d21_1pbhead->d2_retcode = res_err_ptr->d23_Zretcod;
		parblk->d21_1pbhead->d2_errclass = res_err_ptr->d23_Zerrcls;
		parblk->d21_1pbhead->d2_errvalue = res_err_ptr->d23_Zerrval;
		d21_bh5_i_unbind(ii_ptr->d21_i_assid);
  		ipc_bind_exists = FALSE;
		free(i_inbind_ptr);
		free(res_err_ptr);
		ret_value = D2_ERROR;
		goto do_unlock;
		}
					/* BIND as anonymous o.k.	*/
	}	/* if (try_anonymous) */


				/* 					*/
				/* BIND succeeded, fill in bind_tab 	*/
				/* and return dsa-name			*/
/*
prepare for return of dsa-name
*/
if (strcmp((char *)target_dsa,"") != 0)
  {   /* DSA-Name is returned */
  c_ptr = (char *) target_dsa;
  }
else
  {   /* same DSA-Name will be returned */
  c_ptr = (char *) parblk->d21_1name_dsa;
  }

dsa_name_len = (c_ptr != NULL) ? (strlen(c_ptr) + 1) : 1;
bt_ptr->d21_b_name_dsa = D27_MALLOC(svc_c_sev_warning,dsa_name_len) ;
if (bt_ptr->d21_b_name_dsa == NULL)
{       parblk->d21_1pbhead->d2_retcode = D2_TEMP_ERR ;
	parblk->d21_1pbhead->d2_errclass = D2_SYS_ERR ;
	parblk->d21_1pbhead->d2_errvalue = D21_ALLOC_ERROR ;
	free(i_inbind_ptr) ;
	free(res_bind_ptr) ;
	i_inbind_ptr = NULL;
	res_bind_ptr = NULL;
	unbd_parblk.d21_2pbhead = parblk->d21_1pbhead;
	unbd_parblk.d21_2bind_id = res_bind_ptr->d23_Bhdr.d23_bindid;
	d21_bh2_exec_unbind(&unbd_parblk);
	ret_value = D2_ERROR;
	goto do_unlock;
}
else
{       if (c_ptr != NULL)
	{   strcpy((char *)bt_ptr->d21_b_name_dsa , c_ptr) ;
	    if (target_dsa != NULL)
		{
	    	free(target_dsa);
		}
	}
	else
	{   *(bt_ptr->d21_b_name_dsa) = D2_EOS;
	}
}


bt_ptr->d21_b_used = 1;
bt_ptr->d21_b_dir_id = parblk->d21_1dir_id;
bt_ptr->d21_b_ipcinf = ii_ptr;
bt_ptr->d21_b_usr_id = ii_ptr->d21_i_assid;
bt_ptr->d21_b_syntax = D23_GTS(contid);
bt_ptr->d21_b_ipc_used = FALSE;
bt_ptr->d21_b_auth_mech = parblk->d21_1auth_mech;

parblk->d21_1bind_id = new_bid;
free(i_inbind_ptr);
i_inbind_ptr = NULL;

/*
return name of dsa
*/
buffer = D27_MALLOC(svc_c_sev_warning,strlen((char *)bt_ptr->d21_b_name_dsa)+1);


if ( buffer != NULL)
     {
      parblk->d21_1ret_dsa = (D2_name_string)buffer;
      strcpy((char *)parblk->d21_1ret_dsa , (char *)bt_ptr->d21_b_name_dsa) ;
     }

free(res_bind_ptr);
res_bind_ptr = NULL;

do_unlock:
		;

END_d21_bind_tab_LOCK

if (ret_value != D2_NOERROR && pstate_set_to_bind_exists == TRUE &&
						ipc_bind_exists == TRUE)
  {
  /*
     set IPC-state back to D21_NO_DIR_BIND
  */
  sprintf(sspec_field, "%c%d", D23_RSTATE, D21_NO_DIR_BIND);
  pstat_pb.d23_Passid = ii_ptr->d21_i_assid;
  pstat_pb.d23_Ppstate = sspec_field;
  pstat_pb.d23_Phdinfo.d2_version = D23_V02;
  (void)d23_set_pstate(&pstat_pb);
  }
return(ret_value);
		
}	/* d21_bh1_exec_bind () */

/*exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_bh6_i_wait()                                   */
/*                                                                    */
/* AUTHOR       :  J.Heigert,     Softlab GmbH.                       */
/* DATE         :  15.01.87                                           */
/*                                                                    */
/* SYNTAX       :  Short d21_bh6_i_wait(pbhead,ipc_info,i_wtrspb_ptr) */
/*                    D2_pbhead	*pbhead;                              */
/*                    d21_ipc_info *ipc_info;                  */
/*                    D23_wtrspb *i_wtrspb_ptr;                       */
/*                                                                    */
/* DESCRIPTION  :  calls ipc-wait-result, builds respective parameter */
/*                 block, checks the following error cases:           */
/*                     ipc-errror, error-result-returned,             */
/*                     invalid-invoke-id                              */
/*                                                                    */
/* INPUT-PARAMETER :  pbhead: ptr to std-head                         */
/*                    ipc-info: ptr to info on existing               */
/*                              ipc-association                       */
/*                    i_wtrspb_ptr: ptr to                            */
/*                                  ipc-wait-result-parameter-block   */
/*                                                                    */
/* OUTPUT-PARAMETER :  pbhead: std-head, depending on errors          */
/*                     i_wtrspb_ptr: parameter-block as returned by   */
/*                                   ipc                              */
/*                                                                    */
/* RETURN-VALUE :  0: no error                                        */
/*                 -1: error                                          */
/*                                                                    */
/* STANDARDHEAD :                                                     */
/*                                                                    */
/* USED FUNCTIONS/PROGRAMS/MACROS :                                   */
/*                 d23_waitrs()                                       */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/*exoff **************************************************************/

static d2_ret_val d21_bh6_i_wait(
  D2_pbhead * pbhead,
  d21_ipc_info * ipc_info,
  D23_wtrspb * i_wtrspb_ptr)
{
D23_rserror	*res_err_ptr;
D23_pstatpb	pstat_pb;
signed16	serv_id;
char            sspec_field[100];
signed32	res;

serv_id = ipc_info->d21_i_servid;

				/* fill in ipc-wait parameter block	*/
i_wtrspb_ptr->d23_7hdinfo.d2_version = D23_V02;
i_wtrspb_ptr->d23_7assid = ipc_info->d21_i_assid;
i_wtrspb_ptr->d23_7wtime = D23_INFIN;

				/* exec ipc-wait			*/
while ((res = d21_ipcreceive(NULL,i_wtrspb_ptr,D23_LASTFG)) == D23_SUCES &&
		i_wtrspb_ptr->d23_7rltid == D23_NOOPR) ;
if (res != D23_SUCES)
	{			/* ipc error				*/
	*pbhead = i_wtrspb_ptr->d23_7hdinfo;

	return(D2_ERROR);
	}

				/* check if result ok			*/
if (i_wtrspb_ptr->d23_7rltid != D23_RESULT) {

     if (i_wtrspb_ptr->d23_7rltid == D23_ABORT ||
		(i_wtrspb_ptr->d23_7rltid >= D23_RREJECT && 
                i_wtrspb_ptr->d23_7rltid <= D23_RREJECT + D23_MXRREJ)) {

      	pbhead->d2_retcode = D2_TEMP_ERR;
	pbhead->d2_errclass = D2_REM_ERR;
      	pbhead->d2_errvalue = D2_NO_RESPONSE;

	if (i_wtrspb_ptr->d23_7rltid == D23_ABORT)
	{   ipc_info->d21_i_aborted = TRUE;
	    /*
	       set IPC-state
	    */
	    sprintf(sspec_field, "%c%d", D23_RSTATE, D21_NO_DIR_BIND);
	    pstat_pb.d23_Passid = ipc_info->d21_i_assid;
	    pstat_pb.d23_Ppstate = sspec_field;
	    pstat_pb.d23_Phdinfo.d2_version = D23_V02;
            (void)d23_set_pstate(&pstat_pb);
	}

      	return (D2_ERROR);
     } else {

	/* result error				*/
	res_err_ptr = (D23_rserror *) i_wtrspb_ptr->d23_7rfrdt;
	pbhead->d2_retcode = res_err_ptr->d23_Zretcod;
	pbhead->d2_errclass = res_err_ptr->d23_Zerrcls;
	pbhead->d2_errvalue = res_err_ptr->d23_Zerrval;
	return(D2_ERROR);
     }
}

				/* return success			*/
pbhead->d2_retcode = D2_NO_ERR;
return(D2_NOERROR);
}	/* d21_bh6_i_wait() */

/*exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_bh2_exec_unbind()                              */
/*                                                                    */
/* AUTHOR       :  J.Heigert,     Softlab GmbH.                       */
/* DATE         :  15.01.88                                           */
/*                                                                    */
/* SYNTAX       :  void d21_bh2_exec_unbind(parblk)                   */
/*                    D21_p2 *parblk;                                 */
/*                                                                    */
/* DESCRIPTION  :  does the UNBIND to DSA , updates bind-tables       */
/*                                                                    */
/* INPUT-PARAMETER :                                                  */
/*                                                                    */
/* RETURN-VALUE :  0:  unbind o.k.                                    */
/*                 -1: error                                          */
/*                                                                    */
/* GLOBAL DATA (modified) :  d21_ipc_tab[]                            */
/*                           d21_bind_tab[]                           */
/*                                                                    */
/* USED FUNCTIONS/PROGRAMS/MACROS :                                   */
/*                 d21_bh5_i_unbind()                                 */
/*                 d21_bh6_i_wait()                                   */
/*                 d23_invoke()                                       */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/*exoff **************************************************************/

void d21_bh2_exec_unbind(
	D21_p2 *parblk)
{
d21_bind_info *bind_info;
d21_ipc_info *ipc_info;
D23_inrqpb		i_inrqpb;
D23_wtrspb		i_wtrspb;
D23_inunbind		*i_inunbind_ptr;
D23_pstatpb		pstat_pb;

const char function_name[] = "d21_bh2";
char    sspec_field[100];

parblk->d21_2pbhead->d2_retcode = D2_NO_ERR;

BEGIN_d21_bind_tab_LOCK 

if ((parblk->d21_2bind_id < 0) || (parblk->d21_2bind_id >=D21_BIND_MAX) ||
	(d21_bind_tab[parblk->d21_2bind_id].d21_b_used == 0))
    {
    parblk->d21_2pbhead->d2_retcode = D2_CALL_ERR;
    parblk->d21_2pbhead->d2_errclass = D2_PAR_ERR;
    parblk->d21_2pbhead->d2_errvalue = D2_NO_BIND;
    DUA_TRACE_NO_BIND(parblk->d21_2bind_id)
    goto do_unlock;
    }

bind_info = &d21_bind_tab[parblk->d21_2bind_id];
				/*					*/
				/* check if dummy bind to CDS		*/
				/*					*/
if ( bind_info->d21_b_ipcinf == NULL)
	{
	if (bind_info->d21_b_used > 1)
		{
		bind_info->d21_b_used--;
		}
	else	{
		bind_info->d21_b_used = 0;
		}
        goto do_unlock;
	}
				/*					*/
				/* check if multiple dua-server bind    */
				/* if so then decrement counter		*/
				/*       else send DS_UNBIND		*/
				/*					*/
if (bind_info->d21_b_used > 1)
{   bind_info->d21_b_used--;
}
else
{
        ipc_info = bind_info->d21_b_ipcinf;
			    /* clear bind-tab-entry                 */
	bind_info->d21_b_used = 0;
	if (bind_info->d21_b_credent != (D2_credent *) NULL)
	  {
	  free(bind_info->d21_b_credent);
	  bind_info->d21_b_credent = NULL;
	  }
	free(bind_info->d21_b_name_dsa) ;
	bind_info->d21_b_name_dsa = NULL;
				/* send UNBIND 				*/
	if (ipc_info->d21_i_aborted == FALSE)
	{               /* fill in parameter block and apdu for */
			/* invoke(UNBIND)                       */
	    i_inrqpb.d23_6hdinfo.d2_version = D23_V02;
	    i_inrqpb.d23_6assid = ipc_info->d21_i_assid;
	    i_inrqpb.d23_6invinfo.d23_invid = D23_UNUSED;
	    i_inrqpb.d23_6invinfo.d23_usrid = bind_info->d21_b_usr_id;
	    i_inrqpb.d23_6oprid = D23_UNBIND;
	    i_inrqpb.d23_6imdat = D23_LASTFG;
	    i_inunbind_ptr = (D23_inunbind *) ipc_info->d21_i_refms;
	    i_inrqpb.d23_6rfidt = (byte *) i_inunbind_ptr;
	    i_inunbind_ptr->d23_Chdr.d23_versno = D23_APDUV22;
	    i_inunbind_ptr->d23_Chdr.d23_dirid = bind_info->d21_b_dir_id;
	    i_inunbind_ptr->d23_Chdr.d23_bindid = 0;
	    i_inunbind_ptr->d23_Ceom.d23_eomtag = D23_EOMTAG;
	    i_inrqpb.d23_6lnidt = sizeof(D23_inunbind);

	    /* call ipc                             */

	    i_inrqpb.d23_6invinfo.d23_contid =
				   D23_SAC (D27_DAP_AC) |
				   D23_SAS (D27_DAP_AS) |
				   D23_SACM (D27_LOC_AC) |
				   D23_STS (D27_PRIV_TS);

	    /* disable some signals */
	    d21_disable_signals();

	    if (d21_ipcinvoke(&i_inrqpb) != D2_NOERROR)
	    {                       /* ipc error            */
		d21_enable_signals();

		if                  /* error is not TOO_BUSY */
				    /* and not D2_NO_RESPONSE */
		    (i_inrqpb.d23_6hdinfo.d2_errvalue != D2_TOO_BUSY &&
		     i_inrqpb.d23_6hdinfo.d2_errvalue != D2_NO_RESPONSE)
		{   parblk->d21_2pbhead->d2_retcode  = D2_PERM_ERR;
		    parblk->d21_2pbhead->d2_errclass = D2_SYS_ERR;
		    parblk->d21_2pbhead->d2_errvalue = D2_NOT_SPEC;
		}
	    }
	    else
	    {
	    /*
	       set IPC-state
	    */
	    sprintf(sspec_field, "%c%d", D23_RSTATE, D21_NO_DIR_BIND);
	    pstat_pb.d23_Passid = ipc_info->d21_i_assid;
	    pstat_pb.d23_Ppstate = sspec_field;
	    pstat_pb.d23_Phdinfo.d2_version = D23_V02;
            (void)d23_set_pstate(&pstat_pb);

	    /* restore old signal status */
	    d21_enable_signals();
       		    /*                                      */
		    /* wait for response                    */
		    /*                                      */
		if (d21_bh6_i_wait(parblk->d21_2pbhead,
				   ipc_info,&i_wtrspb) != 0)
		{       /* some error with ipc-wait     */
		    if (parblk->d21_2pbhead->d2_errvalue != D2_TOO_BUSY &&
			parblk->d21_2pbhead->d2_errvalue != D2_NO_RESPONSE)
		    {   parblk->d21_2pbhead->d2_retcode  = D2_PERM_ERR;
			parblk->d21_2pbhead->d2_errclass = D2_SYS_ERR;
			parblk->d21_2pbhead->d2_errvalue = D2_NOT_SPEC;
		    }
		}
	    }

			    /*                                      */
			    /* release ipc-assoc.                   */
			    /*                                      */
	d21_bh5_i_unbind(ipc_info->d21_i_assid);
    }
}

do_unlock:
    ;

END_d21_bind_tab_LOCK  

}	/* d21_bh2_exec_unbind() */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca8_cred_iput()                                */
/*                                                                    */
/* AUTHOR       :  J. Heigert,    Softlab GmbH.                       */
/* DATE         :  20.01.88                                           */
/*                                                                    */
/* DESCRIPTION  :  copy a D2_credent structure (in ptr format)        */
/*                 to a apdu-structure                                */
/*                                                                    */
/* INPUT-PARAMETER :  from: ptr to credentials                        */
/*                    to:   ptr to target location                    */
/*                                                                    */
/* OUTPUT-PARAMETER :  end: ptr to ptr to next location after the     */
/*                          generated apdu                            */
/*                                                                    */
/* RETURN-VALUE :  none                                               */
/*                                                                    */
/* exoff **************************************************************/

void d21_ca8_cred_iput(
  D2_credent *from,
  char *to)
{
register signed32	i;
register char	*t, *f;
D23_pw_info	*pw_ptr;

/* normal user */
				/* copy user name			*/
if (from != NULL)
  {
  d21_cab_dsname_iput(from->d2_c_name, to, (char **)&pw_ptr);
				/* copy user password			*/
  pw_ptr->d23_pwtag = D23_PWDTAG;
  if (from->d2_c_passwd == NULL)
    {
    pw_ptr->d23_pwlen = pw_ptr->d23_pwoff = 0 ;
    }
  else
    {
    pw_ptr->d23_pwlen = from->d2_c_passwd->d2_p_len;
    pw_ptr->d23_pwoff = D2_ALIGN(pw_ptr->d23_pwlen);
    f = (char *) from->d2_c_passwd->d2_p_pw;
    t = (char *) (pw_ptr + 1);
    for (i=0; i<from->d2_c_passwd->d2_p_len; i++)
	{ *t++ = *f++; }
    }
  } /* of : from != NULL */
}	/* d21_ca8_cred_iput() */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_len_inbind()                                */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 BIND REQUEST in DUA.                               */
/*                                                                    */
/* INPUT-PARAMETER :  auth_mech, sec_ctx_size                         */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/

static signed32 d21_ca_len_inbind(
  D2_a_value * addr_dsa,
  signed16 auth_mech,
  signed32 sec_ctx_size)
{
signed32 d23_len;

switch (auth_mech)
  {
  case D2_ANONYMOUS:
	d23_len = 0;
	break;
  case D2_SIMPLE:
	d23_len = D23_S_SIMCR + sec_ctx_size;
	break;
  case D2_DCE_AUTH:
	d23_len = D23_S_EXTCR + sec_ctx_size;
	break;
  case D2_STRONG:
	d23_len = D23_S_SSCR + sec_ctx_size;
	break;
  default:
	d23_len = 0;
	break;
  }

d23_len += D23_S_INB + D23_S_ATV +
		((addr_dsa == NULL) ? 0 : addr_dsa->d2_a_v_len) +
		D23_S_EOM;

return(D2_ALIGN(d23_len));
}

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca6_cred_alloc()                               */
/*                                                                    */
/* AUTHOR       :  J. Heigert,    Softlab GmbH.                       */
/* DATE         :  24.12.87                                           */
/*                                                                    */
/* SYNTAX       :  D2_credent *d21_ca6_cred_alloc(from)               */
/*                    D2_credent *from;                               */
/*                                                                    */
/* DESCRIPTION  :  allocate new heap space and copy credent-info      */
/*                 to this new location                               */
/*                                                                    */
/* INPUT-PARAMETER :  from:  ptr to credent-info                      */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  ptr to new credent-info                            */
/*                 NULL: alloc-error                                  */
/*                                                                    */
/* GLOBAL DATA (used) :                                               */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* COMMENTS :  depends on definition of D2_credent and D2_password;   */
/*             some provisions for special alignment requirements;    */
/*             Assumption: c_name and c_passwd are not NULL,          */
/*             they may have length 0, though.                        */
/*                                                                    */
/* exoff **************************************************************/

static D2_credent * d21_ca6_cred_alloc(
  D2_credent * from)
{
register signed32	i;
register char	*fff, *ttt;
D2_credent	*to;
signed32	uname_len;
signed32	passwd_len;

				/* allocate memory for credent-info	*/
uname_len = (from->d2_c_name == NULL ?  0 : (strlen((char *)from->d2_c_name) + 1));
passwd_len = (from->d2_c_passwd == NULL)?  0 : from->d2_c_passwd->d2_p_len;
to = D27_MALLOC(svc_c_sev_warning,
			sizeof(D2_credent) + sizeof(D2_password) +
			uname_len + passwd_len + D2_ALIGN(1));
if (to != NULL)
	{
				/* copy credent-info			*/
	to->d2_c_name = (D2_name_string) (to + 1);
	if (uname_len != 0)
		strcpy((char *)to->d2_c_name,(char *)from->d2_c_name);
	if (from->d2_c_passwd == NULL)
	  {
	  to->d2_c_passwd = NULL;
	  }
	else
	  {
	  to->d2_c_passwd=(D2_password *)(to->d2_c_name + D2_ALIGN(uname_len));
	  to->d2_c_passwd->d2_p_len = from->d2_c_passwd->d2_p_len;
	  to->d2_c_passwd->d2_p_pw = (Octet_string) (to->d2_c_passwd +1);
	  ttt = (char *) (to->d2_c_passwd +1);
	  fff = (char *)(from->d2_c_passwd->d2_p_pw);
	  for (i=0; i<from->d2_c_passwd->d2_p_len; i++)
		*ttt++ = *fff++;
	  }
	}	/* if */

return(to);
} 	/* d21_ca6_cred_alloc() */


/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :   d21_ca7_cred_cmp()                                */
/*                                                                    */
/* AUTHOR       :  J. Heigert,    Softlab GmbH.                       */
/* DATE         :  24.12.87                                           */
/*                                                                    */
/* SYNTAX       :  boolean d21_ca7_cred_cmp(a,b)                        */
/*                    D2_credent *a, *b;                              */
/*                                                                    */
/* DESCRIPTION  :  compare D2-credent structures for equality         */
/*                                                                    */
/* INPUT-PARAMETER :  ALL					      */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  D2_NOERROR: structures are equal                   */
/*                 D2_ERROR: structures are not equal                 */
/*                                                                    */
/* exoff **************************************************************/

static boolean d21_ca7_cred_cmp(
  signed16 bind_auth_mech,
  D2_credent * bind_cred,
  signed16 par_auth_mech,
  D2_credent * par_cred)
{
register signed32	i;
register char	*ap, *bp;

/* auth. mechanism must be same to share a bind table entry.    */
if (bind_auth_mech != par_auth_mech)
  {
  return(FALSE);
  }

/* STRONG mechanism always use same credentials.        */
if ((par_auth_mech == D2_STRONG) || (par_auth_mech == D2_ANONYMOUS))
  {
  return(TRUE);
  }

/* because master bind changes the DCE creds, it is assumed that all DCE  */
/* binds have different creds                                             */
if (par_auth_mech == D2_DCE_AUTH)
  {
  return(FALSE);
  }

/* handling SIMPLE credential case.     */

				/* compare user-name 			*/
if (d21_caa_dsname_cmp(bind_cred->d2_c_name,par_cred->d2_c_name))
	{
	return(FALSE);
	}

if ((bind_cred->d2_c_passwd == NULL) && (par_cred->d2_c_passwd == NULL))
	{
	return(TRUE);
	}

if (((bind_cred->d2_c_passwd == NULL) && (par_cred->d2_c_passwd != NULL)) ||
    ((bind_cred->d2_c_passwd != NULL) && (par_cred->d2_c_passwd == NULL)) ||
    (bind_cred->d2_c_passwd->d2_p_len != par_cred->d2_c_passwd->d2_p_len))
	{
	return(FALSE);
	}
				/* compare passwd			*/
ap = (char *)(bind_cred->d2_c_passwd->d2_p_pw);
bp = (char *)(par_cred->d2_c_passwd->d2_p_pw);
for (i=0; i<bind_cred->d2_c_passwd->d2_p_len; i++)
	{
	if ( *ap++ != *bp++ )
		return(FALSE);
	}
return(TRUE);
} 	/* d21_ca7_cred_cmp() */

static boolean d21_caa_dsname_cmp(
  D2_name_string n1,
  D2_name_string n2)
{
if ((n1 == NULL) && (n2 == NULL))
  {
  return(TRUE);
  }

if (strcmp((char *)n1,(char *)n2) == 0)
  {
  return(TRUE);
  }

return(FALSE);
}	/* d21_caa_dsa_name_cmp() */

/*
* exon ***************************************************************
*                                                                    *
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_att_cl()                                       *
*                                                                    *
* AUTHOR       :  H.Volpers,                                         *
* DATE         :  30.10.90                                           *
*                                                                    *
* SYNTAX       :  Short d21_att_cl(				     *
*                   D2pbhead * pbhead)                               *
*                                                                    *
* DESCRIPTION  :  this function attaches the client                  *
*                                                                    *
* INPUT-PARAMETER :                                                  *
*                                                                    *
* OUTPUT-PARAMETER :                                                 *
*                                                                    *
* RETURN-VALUE :  D2_NOERROR: no error				     *
*                 D2_ERROR: error				     *
*                                                                    *
* GLOBAL DATA (used) :                                               *
*                                                                    *
* GLOBAL DATA (modified) :                                           *
*                                                                    *
* COMMENTS :                                                         *
*                                                                    *
* exoff **************************************************************
*/

d2_ret_val d21_att_cl(
  D2_pbhead *pbhead)
{
D23_atclpb      iatclpb;         /* ipc_attach ctrl block                */
d2_ret_val ret_value = D2_NOERROR;
signed32 i;
const char function_name[] = "d21_att_cl";

BEGIN_attached_to_ipc_LOCK

if (!attached_to_ipc)
	{

BEGIN_d21_ipc_tab_LOCK

				/* initialize ipc-tab			*/
	for (i=0; i<D23_MXCLTPROC; i++)
		d21_ipc_tab[i].d21_i_used = FALSE;
	DUA_TRACE_IPC_TAB_INITIALIZED

END_d21_ipc_tab_LOCK 

	iatclpb.d23_9hdinfo.d2_version = D23_V02;
#ifdef THREADSAFE
	iatclpb.d23_9evmode = D23_FASYNC_EM;
#else
	iatclpb.d23_9evmode = D23_SYNC_EM;
#endif /* THREADSAFE */

	if (d23_attclt(&iatclpb) != 0)
		{			/* ipc-attach didnt work	*/
					/* log error code of ipcs       */
		pbhead->d2_retcode = D2_TEMP_ERR;
		pbhead->d2_errclass = D2_REM_ERR;
		pbhead->d2_errvalue = D2_NO_RESPONSE;
		ret_value = D2_ERROR;
		}
	else	{			/* ipc-attach ok		*/
		attached_to_ipc = TRUE;

		(void) atexit(d21_detach);

#ifdef THREADSAFE
		if (d21_create_event_dispatcher (pbhead) != D2_NOERROR)
		  {
		  DUA_TRACE_CREATE_EVENT_DISPATCHER_FAILED(pbhead->d2_errvalue)
		  ret_value = D2_ERROR;
		  }
		else
		  {
		  DUA_TRACE_CREATED_EVENT_DISPATCHER
		  }
#endif /* THREADSAFE */
		  
		} /* if-else d23_attclt failed */
	} /* if not attached to ipc */

END_attached_to_ipc_LOCK

return(ret_value);
}

/*
**********************************************************************
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_88_cds_bind()                                  *
*                                                                    *
* AUTHOR       :  Jochen Keutel                                      *
* DATE         :  25.05 92                                           *
*                                                                    *
* SYNTAX       :  Short d21_88_cds_bind(parblk)                      *
*                 D21_p1 *parblk;                                    *
*                                                                    *
* DESCRIPTION  :  bind to CDS indicated by :                         *
*                 d21_b_ipcinf = NULL                             *
*                                                                    *
* INPUT-PARAMETER :                                                  *
*                                                                    *
* OUTPUT-PARAMETER :                                                 *
*                                                                    *
* RETURN-VALUE : D2_NOERROR : no error                               *
*                D2_ERROR   : error                                  *
*                                                                    *
**********************************************************************
*/

d2_ret_val d21_88_cds_bind(
  D21_p1 *parblk)
{
const char function_name[] = "d21_88_cds_bind";
register d21_bind_info    *bt_ptr;
register signed16 new_bid;

/*
initialize "error-bindid"
*/
parblk->d21_1bind_id = -1;


BEGIN_d21_bind_tab_LOCK
				/* check if CDS-bind already            */
				/* exists                               */
for (new_bid=0; new_bid<D21_BIND_MAX; new_bid++)
	{
	bt_ptr = &d21_bind_tab[new_bid];
	if ((bt_ptr->d21_b_used > 0) && (bt_ptr->d21_b_ipcinf == NULL))
	  {
	  break;
	  }
	}

if (new_bid < D21_BIND_MAX)
  {			/* matching entry found                 */
  d21_bind_tab[new_bid].d21_b_used++;
  DUA_TRACE_USING_EXISTING_BIND_ID(new_bid)
  }
else
  {
	/* find free bind-tab entry */
  for(new_bid=0;
	new_bid<D21_BIND_MAX && d21_bind_tab[new_bid].d21_b_used > 0;new_bid++)
    ;
  if (new_bid < D21_BIND_MAX)
	  {
				/* bt_ptr points to free bind_tab entry	*/
	  bt_ptr = &d21_bind_tab[new_bid];
	  bt_ptr->d21_b_credent = NULL;
	  bt_ptr->d21_b_used = 1;
	  bt_ptr->d21_b_dir_id = parblk->d21_1dir_id;
	  bt_ptr->d21_b_name_dsa = NULL;
	  bt_ptr->d21_b_ipcinf = NULL; /* that indicates the CDS dummy bind */
	  }
	else
	  {		/* bind table overflow		*/
	  parblk->d21_1pbhead->d2_retcode = D2_TEMP_ERR;
	  parblk->d21_1pbhead->d2_errclass = D2_SYS_ERR;
	  parblk->d21_1pbhead->d2_errvalue = D21_BINDS_TOO_MANY;
          DUA_TRACE_BIND_TABLE_OVERFLOW
	  return (D2_ERROR);
	  }
  } /* if-else (CDS bind already there?) */

END_d21_bind_tab_LOCK

parblk->d21_1bind_id = new_bid;
parblk->d21_1ret_dsa = D27_MALLOC(svc_c_sev_warning,strlen("")+1);
*(parblk->d21_1ret_dsa) = D2_EOS;
parblk->d21_1pbhead->d2_retcode = D2_NO_ERR;
DUA_TRACE_USING_NEW_BIND_ID(new_bid)

return(D2_NOERROR);
}

/*
**********************************************************************
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_ipc_clear()                                    *
*                                                                    *
* AUTHOR       :  Jochen Keutel                                      *
* DATE         :  08.03.93                                           *
*                                                                    *
* SYNTAX       :  void d21_ipc_clear (				     *
*				short b_id,			     *
*				Bool new_ipc,			     *
*				d21_bind_info * b_ptr)               *
*                                                                    *
* DESCRIPTION  :  IPC unbind if new_ipc is true			     *
*                                                                    *
**********************************************************************
*/

void d21_ipc_clear(
  signed16 b_id,
  Bool * new_ipc,
  d21_bind_info * b_ptr)
{
const char function_name[] = "d21_ipc_clear";

if (*new_ipc)
  {
  d21_bh5_i_unbind(b_ptr->d21_b_ipcinf->d21_i_assid);
  *new_ipc = FALSE;
  }
else
  {

BEGIN_d21_bind_tab_LOCK

  if (d21_bind_tab[b_id].d21_b_ipc_used == TRUE)
    {
    d21_bind_tab[b_id].d21_b_ipc_used = FALSE;
    }

END_d21_bind_tab_LOCK

  }
} /* end of d21_ipc_clear */

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_detach					*/
/*									*/
/* Description	: This function removes the event dispatcher (if	*/
/*		  THREADSAFE is defined) and sends an IPC abort on all	*/
/*		  open associations.					*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : May 25, 1993						*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

void d21_detach(void)
{
D23_declpb decl_pb ;
D23_inrqpb inrq_pb ;
signed32 i;
d2_ret_val ret_value = D2_NOERROR;

#ifdef THREADSAFE
D2_pbhead pbhead;
#endif

#ifdef THREADSAFE
ret_value = d21_remove_event_dispatcher(&pbhead);
#endif /* THREADSAFE */

if (ret_value == D2_NOERROR)
  {
  inrq_pb.d23_6hdinfo.d2_version = D23_V02;
  for (i=0; i< D23_MXCLTPROC; i++)
    {
    if (d21_ipc_tab[i].d21_i_used == TRUE)
      {
      inrq_pb.d23_6assid = d21_ipc_tab[i].d21_i_assid;
      inrq_pb.d23_6oprid = D23_ABORT;
      (void)d23_invoke(&inrq_pb);
      }
    }
  decl_pb.d23_Ahdinfo.d2_version = D23_V02;
  (void)d23_detclt (&decl_pb);
  }
} /* end of d21_detach */

/* exon ***************************************************************/
/*                                                                    */
/* TYPE         :  C-FUNCTION                                         */
/*                                                                    */
/* NAME         :  d21_ca_calc_credlen()                              */
/*                                                                    */
/* AUTHOR       :  G. Loose                                           */
/* DATE         :  25.09.90                                           */
/*                                                                    */
/* DESCRIPTION  :  to calculate length needed to make APDU for        */
/*                 credent in DUA.                                    */
/*                                                                    */
/* INPUT-PARAMETER :  cred 					      */
/*                                                                    */
/* OUTPUT-PARAMETER :  none                                           */
/*                                                                    */
/* RETURN-VALUE :  calculated length                                  */
/*                                                                    */
/* COMMENTS :                                                         */
/*                                                                    */
/* exoff **************************************************************/

signed32 d21_ca_calc_credlen(
  D2_credent * cred)
{
signed32 d23_len = 0;

if (cred != NULL) 
	{
	d23_len += D23_S_NM;
	if (cred->d2_c_name != NULL)
	  {
	  d23_len += D2_ALIGN(strlen((char *) cred->d2_c_name) + 1);
	  }

	d23_len += D23_S_PW;
	if (cred->d2_c_passwd != NULL)
	  {
	  d23_len += D2_ALIGN(cred->d2_c_passwd->d2_p_len);
	  }

	} /* of cred != NULL */

return(D2_ALIGN(d23_len));
}

