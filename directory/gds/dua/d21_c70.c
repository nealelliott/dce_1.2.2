/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_c70.c,v $
 * Revision 1.1.742.2  1996/02/18  18:16:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:07  marty]
 *
 * Revision 1.1.742.1  1995/12/08  15:53:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:38  root]
 * 
 * Revision 1.1.740.4  1994/06/21  14:46:34  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:38:32  marrek]
 * 
 * Revision 1.1.740.3  1994/05/10  15:53:44  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:31:10  marrek]
 * 
 * Revision 1.1.740.2  1994/03/23  15:14:23  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:27:55  keutel]
 * 
 * Revision 1.1.740.1  1994/02/22  17:38:25  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:30:31  marrek]
 * 
 * Revision 1.1.738.2  1993/10/14  17:27:28  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:25:20  keutel]
 * 
 * Revision 1.1.738.1  1993/10/13  17:31:58  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:40:41  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  06:03:35  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:51:45  marrek]
 * 
 * Revision 1.1.4.5  1992/12/31  19:46:28  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:24:20  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/04  18:06:39  marrek
 * 	November 1992 code drop
 * 	one waitrs forgotten
 * 	[1992/12/04  10:58:00  marrek]
 * 
 * Revision 1.1.4.3  1992/11/30  14:08:31  marrek
 * 	November 1992 code drop
 * 	Change type of buf_rest and buf_used to int
 * 	[1992/11/30  14:07:31  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  21:08:48  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:35:10  marrek]
 * 
 * Revision 1.1.2.4  1992/07/06  18:45:33  melman
 * 	Porting change from res_pb to rst_pb in d21_recv_result()
 * 	[1992/07/06  18:43:04  melman]
 * 
 * Revision 1.1.2.3  1992/07/01  16:52:38  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:21:23  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:14:09  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:36:27  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_c70.c,v $ $Revision: 1.1.742.2 $ $Date: 1996/02/18 18:16:34 $";
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
* NAME         :  d21_c70.c                                          *
*                                                                    *
* AUTHOR       :  Albert Biber                                       *
* DATE         :  22.01.88                                           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <stdio.h>
#include <stdlib.h>

#include <d21dua.h>

/*
* exon ***************************************************************
*                                                                    *
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_send_invoke                                    *
*                                                                    *
* AUTHOR       :  Albert Biber,  Softlab GmbH.                       *
* DATE         :  21.01.88                                           *
*                                                                    *
*                                                                    *
* RETURN-VALUE :   D2_NOERROR : successful			     *
*                  D2_ERROR:   error				     *
*                                                                    *
* exoff **************************************************************
*/

signed32 d21_send_invoke(
  D21_req_info * req_pb,
  signed32 usr_id,
  D2_pbhead * err_pb)
{
    signed16      	  apdu_len, blck_len;
    D23_inrqpb            inv_pb;
    char                * data_ref;
    signed32 result;
const char function_name[] = "d21_send_invoke";

  if (req_pb->d21_r_ipcinfo->d21_i_aborted == TRUE)
  { err_pb->d2_retcode =  D2_TEMP_ERR;
    err_pb->d2_errclass = D2_REM_ERR;
    err_pb->d2_errvalue = D2_NO_RESPONSE;
    return (D2_ERROR);
  }

#ifdef THREADSAFE
  (void) pthread_setcancel(d21_old_cancel_state());

  TRY
    {
    pthread_testcancel();
    }
  CATCH (pthread_cancel_e)
    {
    DUA_TRACE_CANCEL_EXCEPTION_DETECTED(req_pb->d21_r_opcode)
    if (req_pb->d21_r_opcode == D23_SEARCH ||
	req_pb->d21_r_opcode == D23_LIST ||
	req_pb->d21_r_opcode == D23_READ ||
	req_pb->d21_r_opcode == D23_COMPARE )
      {
      result = D21_INTERRUPTED;
      }
    else
      {
      result = D2_NOERROR;
      }
    }
  ENDTRY

  if (result == D21_INTERRUPTED)
    {
    return(result);
    }

/* set general cancallation to OFF for sending the invoke */

(void) pthread_setcancel(CANCEL_OFF);

#endif /* THREADSAFE */

/* disable some signals */
d21_disable_signals();

  /*
  Set Parameter for Invoke
  */
  inv_pb.d23_6assid = req_pb->d21_r_ipcinfo->d21_i_assid;
  inv_pb.d23_6invinfo.d23_invid = D23_UNUSED;
  inv_pb.d23_6invinfo.d23_usrid = usr_id;
  inv_pb.d23_6rfidt = req_pb->d21_r_ipcinfo->d21_i_refms;
  inv_pb.d23_6hdinfo.d2_version = D23_V02;
  inv_pb.d23_6oprid = req_pb->d21_r_opcode;
  blck_len = req_pb->d21_r_ipcinfo->d21_i_lenms;

  data_ref = req_pb->d21_r_bufapdu ;

  /*
  Send the Invoke-Request 
  */
  apdu_len = req_pb->d21_r_lenapdu;
  DCE_SVC_LOG((GDS_S_APDU_REQUEST_MSG,req_pb->d21_r_opcode,
			req_pb->d21_r_lenapdu,req_pb->d21_r_bufapdu));

  while (apdu_len > blck_len)
  {
    /*
    Copy Request Data-Block
    */
    memcpy((char *)(req_pb->d21_r_ipcinfo->d21_i_refms),data_ref,blck_len);
    inv_pb.d23_6lnidt = blck_len;
    inv_pb.d23_6imdat = D23_MRDATA;

    /*
    Transfer Request Data-Block
    */
    inv_pb.d23_6invinfo.d23_contid =
			 D23_SAC (D27_DAP_AC) |
			 D23_SAS (D27_DAP_AS) |
			 D23_SACM (D27_LOC_AC) |
			 D23_STS (D27_PRIV_TS);

    if  (d21_ipcinvoke(&inv_pb) != D2_NOERROR)
    {
      err_pb->d2_retcode =  D2_PERM_ERR;
      err_pb->d2_errclass = D2_SYS_ERR;
      err_pb->d2_errvalue = D2_NOT_SPEC;
      d21_enable_signals();
      return (D2_ERROR);
    }
    apdu_len -= blck_len;

    data_ref += blck_len ;

  }
  /*
  Copy the last Request Data-Block
  */
  memcpy((char *)(req_pb->d21_r_ipcinfo->d21_i_refms),data_ref,apdu_len);
  inv_pb.d23_6lnidt = apdu_len;
  inv_pb.d23_6imdat = D23_LASTFG;

  /*
  Transfer the last Request Data-Block
  */
  inv_pb.d23_6invinfo.d23_contid =
		       D23_SAC (D27_DAP_AC) |
		       D23_SAS (D27_DAP_AS) |
		       D23_SACM (D27_LOC_AC) |
		       D23_STS (D27_PRIV_TS);

  if (d21_ipcinvoke(&inv_pb) != D2_NOERROR)
  {
    err_pb->d2_retcode =  D2_PERM_ERR;
    err_pb->d2_errclass = D2_SYS_ERR;
    err_pb->d2_errvalue = D2_NOT_SPEC;
    d21_enable_signals();
    return (D2_ERROR);
  }

  d21_enable_signals();
  req_pb->d21_r_ipcinfo->d21_ipcev_pblk.d23_Binvinfo.d23_invid =
			inv_pb.d23_6invinfo.d23_invid;
  return (D2_NOERROR);

}                  /*  End d21_send_invoke  */



/*
* exon ***************************************************************
*                                                                    *
* TYPE         :  C-FUNCTION                                         *
*                                                                    *
* NAME         :  d21_recv_result                                    *
*                                                                    *
* AUTHOR       :  Albert Biber,  Softlab GmbH.                       *
* DATE         :  21.01.88                                           *
*                                                                    *
*                                                                    *
* RETURN-VALUE :   D2_NOERROR : successful			     *
*                  D2_ERROR:   error				     *
* exoff **************************************************************
*/

signed32 d21_recv_result(
  D21_req_info * res_pb,
  signed32 usr_id,
  D2_pbhead * err_pb )
{
    signed16      	  buf_rest;
    D23_wtrspb            rst_pb ;
    D23_pstatpb		  pstat_pb;
    D23_rserror          *err_apdu;
    char		 *new_base;
    signed32                buf_used;
    char                  sspec_field[10] ;
    Bool 		  alloc_error = FALSE;
    signed16 mflag;
    signed32 ret_value = D2_NOERROR;
    signed32 interrupt_ret_value = D2_NOERROR;
    D21_abandon_info abandon_info;

  /*
  Set Parameter for Wait-Result 
  */
  rst_pb.d23_7hdinfo.d2_version = D23_V02;
  rst_pb.d23_7assid = res_pb->d21_r_ipcinfo->d21_i_assid;
  rst_pb.d23_7wtime = D23_INFIN;

  abandon_info.op_code = res_pb->d21_r_opcode;
  abandon_info.serv_id = res_pb->d21_r_ipcinfo->d21_i_servid;
  abandon_info.inv_id =
		res_pb->d21_r_ipcinfo->d21_ipcev_pblk.d23_Binvinfo.d23_invid;
  abandon_info.usr_id = usr_id;

  /*
  Get the Result  
  */
  buf_rest = res_pb->d21_r_lenapdu;
  buf_used = buf_rest;
  mflag = D23_LASTFG;

  do 
  {
    /*
    Receive Result Data-Block
    */
    while ((ret_value = d21_ipcreceive(&abandon_info,&rst_pb,mflag))
			!= D2_ERROR && rst_pb.d23_7rltid == D23_NOOPR) ;
    mflag = D23_MRDATA;
    if (ret_value == D21_INTERRUPTED)
      {
      interrupt_ret_value = D21_INTERRUPTED;
      }
    if (ret_value == D2_ERROR) {

      err_pb->d2_retcode = rst_pb.d23_7hdinfo.d2_retcode;
      err_pb->d2_errclass = rst_pb.d23_7hdinfo.d2_errclass;
      err_pb->d2_errvalue = rst_pb.d23_7hdinfo.d2_errvalue;
      break;

    } else if (rst_pb.d23_7rltid == D23_ABORT ||
		(rst_pb.d23_7rltid >= D23_RREJECT && 
                 rst_pb.d23_7rltid <= D23_RREJECT + D23_MXRREJ)) {
      err_pb->d2_retcode = D2_TEMP_ERR;
      err_pb->d2_errclass = D2_REM_ERR;
      err_pb->d2_errvalue = D2_NO_RESPONSE;

      if (rst_pb.d23_7rltid == D23_ABORT)
      {     res_pb->d21_r_ipcinfo->d21_i_aborted = TRUE;
	    /*
	       set IPC-state
	    */
	    sprintf(sspec_field, "%c%d", D23_RSTATE, D21_NO_DIR_BIND);
	    pstat_pb.d23_Passid = res_pb->d21_r_ipcinfo->d21_i_assid;
	    pstat_pb.d23_Ppstate = sspec_field;
	    pstat_pb.d23_Phdinfo.d2_version = D23_V02;
            (void)d23_set_pstate(&pstat_pb);

	    /*
		release IPC connection
	    */
	    d21_bh5_i_unbind(res_pb->d21_r_ipcinfo->d21_i_assid);
      }
      ret_value = D2_ERROR;
      break;
   }
	
    /*
    Check if enough memory for result
    */
    if (buf_rest < rst_pb.d23_7lnrdt)
    {
      /*
      Allocate new memory-segment
      */
      buf_used += res_pb->d21_r_ipcinfo->d21_i_lenms;
      buf_rest += res_pb->d21_r_ipcinfo->d21_i_lenms;
      new_base = D27_REALLOC(svc_c_sev_warning,res_pb->d21_r_bufapdu,buf_used);
      if (new_base == NULL)
      {
        err_pb->d2_retcode = D2_TEMP_ERR;
        err_pb->d2_errclass = D2_SYS_ERR;
        err_pb->d2_errvalue = D21_ALLOC_ERROR;
	alloc_error = TRUE;
	break;
      }
      res_pb->d21_r_bufapdu = new_base;
    }
    /*
    Copy Result Data-Block
    */
    memcpy(res_pb->d21_r_bufapdu + (buf_used - buf_rest),
               (char *)rst_pb.d23_7rfrdt,rst_pb.d23_7lnrdt);
    buf_rest -= rst_pb.d23_7lnrdt;
  } while (rst_pb.d23_7rmdat == D23_MRDATA);
  
if (alloc_error == TRUE)
  {
  while (rst_pb.d23_7rmdat == D23_MRDATA)
    {
    while ((ret_value = d21_ipcreceive(&abandon_info,&rst_pb,mflag))
			!= D2_ERROR && rst_pb.d23_7rltid == D23_NOOPR) ;
    if (ret_value == D2_ERROR) 
      {
      break;
      }
    }
  ret_value = D2_ERROR;
  }


  res_pb->d21_r_lenapdu = buf_used - buf_rest;
  if ( rst_pb.d23_7rltid >= D23_RERROR &&
       rst_pb.d23_7rltid <= D23_RERROR + D23_MXRERR )
  {
    err_apdu = (D23_rserror *) res_pb->d21_r_bufapdu;
    err_pb->d2_retcode = err_apdu->d23_Zretcod;
    err_pb->d2_errclass = err_apdu->d23_Zerrcls;
    err_pb->d2_errvalue = err_apdu->d23_Zerrval;
    DCE_SVC_LOG((GDS_S_APDU_ERROR_MSG,
			res_pb->d21_r_lenapdu,res_pb->d21_r_bufapdu));
    if (res_pb->d21_r_opcode ==D23_BIND) res_pb->d21_r_opcode =D23_RREJECT;
    else res_pb->d21_r_opcode = D23_RERROR;
  }
  else
    {
    DCE_SVC_LOG((GDS_S_APDU_RESULT_MSG,res_pb->d21_r_opcode,
			res_pb->d21_r_lenapdu,res_pb->d21_r_bufapdu));
    res_pb->d21_r_opcode = D23_RESULT;
    }

if (interrupt_ret_value == D21_INTERRUPTED)
  {
  ret_value = D21_INTERRUPTED;
  }

return(ret_value);
}                  /*  End d21_recv_result  */

signed32 d21_recv_bind_res(
  D21_req_info * res_pb,
  D2_pbhead * err_pb,
  unsigned16 * contid)
{
    signed16      	  buf_rest;
    D23_wtrspb            rst_pb;
    D23_pstatpb		  pstat_pb;
    D23_rserror          *err_apdu;
    char		 *new_base;
    signed16                buf_used;
    char                  sspec_field[10] ;
    signed16			res;
    signed16 mflag;
    D21_abandon_info abandon_info;

  /*
  Set Parameter for Wait-Result 
  */
  rst_pb.d23_7hdinfo.d2_version = D23_V02;
  rst_pb.d23_7assid = res_pb->d21_r_ipcinfo->d21_i_assid;
  rst_pb.d23_7wtime = D23_INFIN;

  abandon_info.op_code = res_pb->d21_r_opcode;
  abandon_info.serv_id = res_pb->d21_r_ipcinfo->d21_i_servid;
  abandon_info.inv_id =
		res_pb->d21_r_ipcinfo->d21_ipcev_pblk.d23_Binvinfo.d23_invid;
  abandon_info.usr_id = res_pb->d21_r_ipcinfo->d21_i_assid;
  /*
  Get the Result  
  */
  buf_rest = res_pb->d21_r_lenapdu;
  buf_used = buf_rest;
  mflag = D23_LASTFG;
  do 
  {
    /*
    Receive Result Data-Block
    */
    rst_pb.d23_7assid = res_pb->d21_r_ipcinfo->d21_i_assid;
    while ((res = d21_ipcreceive(&abandon_info,&rst_pb,mflag)) == D23_SUCES &&
		    rst_pb.d23_7rltid == D23_NOOPR) ;
    mflag = D23_MRDATA;
    if (res != D23_SUCES) {

      err_pb->d2_retcode = rst_pb.d23_7hdinfo.d2_retcode;
      err_pb->d2_errclass = rst_pb.d23_7hdinfo.d2_errclass;
      err_pb->d2_errvalue = rst_pb.d23_7hdinfo.d2_errvalue;
      return (D2_ERROR);

    } else if (rst_pb.d23_7rltid == D23_ABORT)
	{
        err_pb->d2_retcode = D2_TEMP_ERR;
        err_pb->d2_errclass = D2_REM_ERR;
        err_pb->d2_errvalue = D2_NO_RESPONSE;

        res_pb->d21_r_ipcinfo->d21_i_aborted = TRUE;
	    /*
	       set IPC-state
	    */
        sprintf(sspec_field, "%c%d", D23_RSTATE, D21_NO_DIR_BIND);
	pstat_pb.d23_Passid = res_pb->d21_r_ipcinfo->d21_i_assid;
	pstat_pb.d23_Ppstate = sspec_field;
	pstat_pb.d23_Phdinfo.d2_version = D23_V02;
        (void)d23_set_pstate(&pstat_pb);

	    /*
		release IPC connection
	    */
	d21_bh5_i_unbind(res_pb->d21_r_ipcinfo->d21_i_assid);
        return (D2_ERROR);
        }
	
    /*
    Check if enough memory for result
    */
    if (buf_rest < rst_pb.d23_7lnrdt)
    {
      /*
      Allocate new memory-segment
      */
      buf_used += res_pb->d21_r_ipcinfo->d21_i_lenms;
      buf_rest += res_pb->d21_r_ipcinfo->d21_i_lenms;
      new_base = D27_REALLOC(svc_c_sev_warning,res_pb->d21_r_bufapdu,buf_used);
      if (new_base == NULL)
      {
        err_pb->d2_retcode = D2_TEMP_ERR;
        err_pb->d2_errclass = D2_SYS_ERR;
        err_pb->d2_errvalue = D21_ALLOC_ERROR;
        return (D2_ERROR);
      }
      res_pb->d21_r_bufapdu = new_base;
    }
    /*
    Copy Result Data-Block
    */
    memcpy(res_pb->d21_r_bufapdu + (buf_used - buf_rest),
           (char *)rst_pb.d23_7rfrdt,rst_pb.d23_7lnrdt);
    buf_rest -= rst_pb.d23_7lnrdt;
  } while (rst_pb.d23_7rmdat == D23_MRDATA);
  
  res_pb->d21_r_lenapdu = buf_used - buf_rest;
  if ( rst_pb.d23_7rltid == D23_RREJECT)
    {
    err_apdu = (D23_rserror *) res_pb->d21_r_bufapdu;
    err_pb->d2_retcode = err_apdu->d23_Zretcod;
    err_pb->d2_errclass = err_apdu->d23_Zerrcls;
    err_pb->d2_errvalue = err_apdu->d23_Zerrval;
    res_pb->d21_r_opcode =D23_RREJECT;
    DCE_SVC_LOG((GDS_S_APDU_ERROR_MSG,
			res_pb->d21_r_lenapdu,res_pb->d21_r_bufapdu));
    }
  else
    {
    DCE_SVC_LOG((GDS_S_APDU_RESULT_MSG,res_pb->d21_r_opcode,
			res_pb->d21_r_lenapdu,res_pb->d21_r_bufapdu));
    res_pb->d21_r_opcode = D23_RESULT;
    }

  *contid = rst_pb.d23_7invinfo.d23_contid;
  return (D2_NOERROR);

} /* end of d21_recv_bind_res */
