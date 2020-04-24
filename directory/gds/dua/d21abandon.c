/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21abandon.c,v $
 * Revision 1.1.4.2  1996/02/18  18:16:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:53:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:41  root]
 * 
 * Revision 1.1.2.1  1994/06/21  14:39:39  marrek
 * 	Bug fix for June 94 code submission.
 * 	[1994/06/15  10:31:48  marrek]
 * 
 * 	Fix for abandon.
 * 	[1994/06/13  12:24:46  marrek]
 * 
 * 	Created in June 94 code submission.
 * 	[1994/06/10  16:04:22  marrek]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21abandon.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:16:40 $";
#endif

/*----------------------------------------------------------------------*/
/*									*/
/* Copyright (c) 1994 Siemens Nixdorf Informationssysteme AG		*/
/* All Rights Reserved							*/
/*									*/
/* Project	: Abandon						*/
/*									*/
/* File Name	: d21abandon.c						*/
/*									*/
/* Description  : The module contains functions for handling		*/
/*		  ABANDON						*/
/*									*/
/* Date         : May 30, 1994						*/
/*									*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <d21dua.h>

/*----------------------------------------------------------------------*/
/*									*/
/* Function Name:	d21_abandon_handler				*/
/*									*/
/* Description	: This function handles ABANDON in threaded environment	*/
/*									*/
/* Return Value	: none							*/
/*									*/
/* Date         : May 30, 1994						*/
/* Author       : Jochen Keutel, SNI BA NM 123, Munich, Germany		*/
/*									*/
/*----------------------------------------------------------------------*/

#ifdef THREADSAFE
pthread_startroutine_t d21_abandon_handler (
  pthread_addr_t arg)
{
d21_ipc_info * ipc_inf;
D21_req_info ab_req_info;
char ab_req_field[D2_DNL_MAX];
char ab_res_field[D2_DNL_MAX];
D23_inabandon * in_abandon;
D23_eom * in_eom;
D2_pbhead pbhead;
D21_abandon_info abandon_info;
const char function_name[] = "d21_abandon_handler";

DUA_TRACE_IAPL_ENTRY

abandon_info.op_code = ((D21_abandon_info *)arg)->op_code;
abandon_info.serv_id = ((D21_abandon_info *)arg)->serv_id;
abandon_info.inv_id = ((D21_abandon_info *)arg)->inv_id;
abandon_info.usr_id = ((D21_abandon_info *)arg)->usr_id;

if (d21_bh4_i_bind(abandon_info.serv_id,
			&ipc_inf,&pbhead) == D2_NOERROR)
  {
  /* build ABANDON message */
  in_abandon = (D23_inabandon *) ab_req_field;
  in_abandon->d23_Rhdr.d23_versno = D23_APDUV22;
  in_abandon->d23_Rhdr.d23_filler = 0;
  in_abandon->d23_Rhdr.d23_dirid = 0;
  in_abandon->d23_Rhdr.d23_bindid = 0;

  in_abandon->d23_Rinv_id = abandon_info.inv_id;

  in_eom = (D23_eom *) (in_abandon + 1);
  in_eom->d23_eomtag = D23_EOMTAG;
  in_eom->d23_eomfil = 0;

  ab_req_info.d21_r_opcode = D23_ABANDON;
  ab_req_info.d21_r_bufapdu = ab_req_field;
  ab_req_info.d21_r_lenapdu = sizeof(D23_inabandon) + sizeof(D23_eom);;
  ab_req_info.d21_r_ipcinfo = ipc_inf;

  DUA_TRACE_SEND_ABANDON(abandon_info.inv_id)
  if (d21_send_invoke(&ab_req_info,abandon_info.usr_id,&pbhead) == D2_NOERROR)
    {
    ab_req_info.d21_r_bufapdu = ab_res_field;
    ab_req_info.d21_r_lenapdu = D2_DNL_MAX;

    (void)d21_recv_result(&ab_req_info,abandon_info.usr_id,&pbhead);
    } /* end of : d21_send_invoke was OK */

  d21_bh5_i_unbind(ipc_inf->d21_i_assid);
  } /* end of : d21_bh4_bind was OK */

if (ab_req_info.d21_r_opcode == D23_RESULT)
  {
  DUA_TRACE_SUCC_ABANDONED(abandon_info.inv_id)
  }
else
  {
  DUA_TRACE_NOT_ABANDONED(abandon_info.inv_id)
  }
DUA_TRACE_IAPL_EXIT(pbhead.d2_errvalue)

} /* end of d21_abandon_handler */
#endif
