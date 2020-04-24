/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21remh.c,v $
 * Revision 1.1.10.2  1996/02/18  18:17:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:40:21  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:54:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:31  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:54:08  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:32:48  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:14:37  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:28:59  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:43:01  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:35:29  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:34:48  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:59:08  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:48:40  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:26:04  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  21:36:27  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:42:03  marrek]
 * 
 * Revision 1.1.2.2  1992/06/02  03:33:34  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:38:15  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21remh.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:17:32 $";
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
* NAME         :  d21remh.c                                          *
*                                                                    *
* AUTHOR       :  J.Forster                                          *
* DATE         :   3.02.88                                           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <stdio.h>

#include <d21dua.h>

/********************************************************/

d2_ret_val d21_rm_02_rem_from_dsa(
  signed16			  opmode,
  D21_p8                  *parblk,
  d21_bind_info    *bnd_tbl,
  char                    *req_buf,
  signed32                  req_len,
  char                    **res_buf,
  signed32                  *res_len )
{
D21_req_info    req_info;
d2_ret_val           ret_value;
D23_inremobjt   *inv_apdu ;

    ret_value = D2_NOERROR;

	/* set bind-id in apdu */

    inv_apdu = (D23_inremobjt *) req_buf ;
    inv_apdu->d23_Phdr.d23_bindid = 0;

    /*
    Set the Parameter-Block for Request
    */
    req_info.d21_r_opcode = D23_REMOBJT;
    if ( opmode != D21_STD_UPDATE )         
      {                                    
      req_info.d21_r_opcode += D2_ADM_OPID;
      }                                    
    req_info.d21_r_bufapdu = req_buf;
    req_info.d21_r_lenapdu = req_len;
    req_info.d21_r_ipcinfo = bnd_tbl->d21_b_ipcinf;
    /*
    Send the Request to DSA
    */
    if (d21_send_invoke (&req_info,bnd_tbl->d21_b_usr_id,
					parblk->d21_8pbhead) != D2_NOERROR)
    {
    return (D2_ERROR);
    }
    /*
    Receive Result from DSA in res_buf
    */
    req_info.d21_r_bufapdu = *res_buf;
    req_info.d21_r_lenapdu = *res_len;

    ret_value = d21_recv_result (&req_info, bnd_tbl->d21_b_usr_id,
							parblk->d21_8pbhead);
    *res_buf = req_info.d21_r_bufapdu;
    *res_len = req_info.d21_r_lenapdu;

    /*
    set ret_value of function
    */
    if ((ret_value == D2_NOERROR) && (req_info.d21_r_opcode == D23_RERROR))
	{
	ret_value = D2_ERROR;
	}
    return (ret_value);
}
