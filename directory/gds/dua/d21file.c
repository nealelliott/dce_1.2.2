/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21file.c,v $
 * Revision 1.1.744.2  1996/02/18  18:16:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:36  marty]
 *
 * Revision 1.1.744.1  1995/12/08  15:53:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:59  root]
 * 
 * Revision 1.1.742.3  1994/06/21  14:46:41  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:39:06  marrek]
 * 
 * Revision 1.1.742.2  1994/05/10  15:53:54  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:32:01  marrek]
 * 
 * Revision 1.1.742.1  1994/02/22  18:05:54  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:32:32  marrek]
 * 
 * Revision 1.1.740.2  1993/10/14  17:27:31  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:25:43  keutel]
 * 
 * Revision 1.1.740.1  1993/10/13  17:32:04  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:41:02  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  06:15:32  marrek
 * 	bug in sc11: mflag was wrong.
 * 	[1993/08/03  09:21:22  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/30  10:54:52  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  10:15:31  marrek
 * 	Insert <pthread.h>
 * 	use const declaration
 * 	[1993/02/02  09:20:30  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:47:30  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:25:02  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  18:07:33  marrek
 * 	November 1992 code drop
 * 	signal handling for d23_waitrs
 * 	[1992/12/04  11:04:20  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  21:15:15  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  13:37:52  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:52:53  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:21:42  melman]
 * 
 * Revision 1.1.2.2  1992/06/02  03:23:13  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:37:22  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21file.c,v $ $Revision: 1.1.744.2 $ $Date: 1996/02/18 18:16:57 $";
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
* AUTHOR       :  Praveen Gupta                                      *
* DATE         :   21.11.90                                          *
*                                                                    *
* DESCRIPTION  :  subroutines for write-to-file option of search     *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <fcntl.h>

#include <d21dua.h>

/******** LOCAL **********/

static d2_ret_val d21_sc11_get_result_buffer( D2_pbhead * , D21_req_info * ,
					int , Bool , signed16 * , Bool * );
static signed16 d21_write ( int , char * , signed16 );
static signed16 d21_read ( int , char * , signed16 );

/*********************************************************************/

/*
write to file
*/

static signed16 d21_write(
  int fd,
  char * buf,
  signed16 nbyte)
{
    int   retval;

    while (TRUE)
    {
	if ((retval=write(fd, buf, nbyte)) != nbyte)
	{
	    if ((retval == -1) && (errno == EINTR))
		continue;
	    return (D2_ERROR);
	}
	else return (nbyte);
    }
}                                     /*  End  d21_write  */

static signed16 d21_read(
  int fd,
  char * buf,
  signed16 nbyte)
{
    int   retval;

    while (TRUE)
    {
	if ((retval=read(fd, buf, nbyte)) != nbyte)
	{
	    if ((retval == -1) && (errno == EINTR))
		continue;
	    return (D2_ERROR);
	}
	else return (nbyte);
    }
}                                     /*  End  d21_read  */


/*
 For write to file option, Sizelimit will be ignored. Hence, entries_found
 is not needed to be computed.
*/

d2_ret_val d21_sc10_write_output_to_file(
  D2_pbhead * pbhead,
  d21_bind_info * bt_ptr,  /* bind table entry               */
  char * req_ptr,       /* search_apdu                          */
  signed32 req_len,       /* length of apdu                       */
  D23_rf_info ** ref_ptr,
  signed16 * entries_found,
  signed16 opcode,
  signed16 * limit_pb,
  int file,          /* file descriptor */
  Bool cache)         /* search in DUAcache if TRUE        */
{
D21_req_info            req_info;
signed16                  apdu_len;
Bool                    last_buffer = FALSE;
D23_eom                 *eom_ptr;
D23_rserror             *err_pdu_ptr;
unsigned16                  tag;
char                 *oq_ptr;
off_t                    offset;
d21_ipc_info * ipc_ptr;
signed32 usr_id;
const char function_name[] = "d21_sc10";

*ref_ptr = NULL;

if (cache == TRUE)
{   /*
       build association to cache
    */
    if (d21_bh4_i_bind(D23_DCASID,&ipc_ptr,pbhead) != D2_NOERROR)
      {   
      return(D2_ERROR);
      }
}

/*
   set ipc-request block
*/
req_info.d21_r_opcode = opcode;
req_info.d21_r_bufapdu = req_ptr;
req_info.d21_r_lenapdu = req_len;
if (cache == TRUE)
  {
  req_info.d21_r_ipcinfo = ipc_ptr;
  usr_id = ipc_ptr->d21_i_assid;
  }
else
  {
  req_info.d21_r_ipcinfo = bt_ptr->d21_b_ipcinf;
  usr_id = bt_ptr->d21_b_usr_id;
  }

/*
   send request to dsa or cache
*/
if (d21_send_invoke(&req_info,usr_id,pbhead) != D2_NOERROR)
{   
    if (cache == TRUE)
    {   /* release association to cache */
	d21_bh5_i_unbind(ipc_ptr->d21_i_assid);
    }
    return (D2_ERROR);
}

/*
   get first part of result
*/
pbhead->d2_retcode =
pbhead->d2_errclass =
pbhead->d2_errvalue  = D2_NO_ERR;

if (d21_sc11_get_result_buffer(pbhead,&req_info,
			       file, FALSE,
			       &apdu_len, &last_buffer) != D2_NOERROR)
{   /* error */
    /* read all IPC buffers */
    d21_sc11_get_result_buffer(pbhead, &req_info,
				file, TRUE,
			       &apdu_len, &last_buffer);
    if (cache == TRUE)
    {   /* release association to cache */
	d21_bh5_i_unbind(ipc_ptr->d21_i_assid);
    }
    return (D2_ERROR);
}

/*
   error-referral or "not-found" ?
*/
if ( (pbhead->d2_retcode   == D2_CALL_ERR) &&
     (pbhead->d2_errclass  == D2_PAR_ERR)  &&
     (pbhead->d2_errvalue  == D2_REF_ERROR) )
{   /* error_referral or not-found-error  */
    err_pdu_ptr = (D23_rserror *) req_info.d21_r_bufapdu;
    *ref_ptr     = (D23_rf_info *) (err_pdu_ptr + 1);
    return(D2_NOERROR);
}
if ( (pbhead->d2_retcode   != D2_NO_ERR) ||
     (pbhead->d2_errclass  != D2_NO_ERR) ||
     (pbhead->d2_errvalue  != D2_NO_ERR) )
{   /* another error  */
    /* read all IPC buffers */
    d21_sc11_get_result_buffer(pbhead, &req_info,
			       file, TRUE,
			       &apdu_len, &last_buffer);
    if (cache == TRUE)
    {   /* release association to cache */
	d21_bh5_i_unbind(ipc_ptr->d21_i_assid);
    }
    return (D2_ERROR);
}

 /* Check if ei_info are received */
tag = ((D23_ds_name *) req_info.d21_r_bufapdu)->d23_dstag;
if (tag == D23_NAMTAG)
{
  tag = ((D23_en_info *) (req_info.d21_r_bufapdu + D23_S_NM
	 + ((D23_ds_name *)req_info.d21_r_bufapdu)->d23_dsoff))->d23_entag;
}
if (tag == D23_ENTTAG)
 {
    *entries_found = 1; /* This is used as boolean to indicate
			    that some entries are received  */
  }

/* read all IPC buffers */

if (d21_sc11_get_result_buffer(pbhead, &req_info,
			       file, TRUE,
			       &apdu_len, &last_buffer) != D2_NOERROR)
  { /* error */
  return(D2_ERROR);
  }
if (cache == TRUE)
{   /* release association to cache */
    d21_bh5_i_unbind(ipc_ptr->d21_i_assid);
}

eom_ptr = (D23_eom *)(req_info.d21_r_bufapdu + apdu_len - sizeof(D23_eom));
if (eom_ptr->d23_eomtag != D23_EOMTAG)
{ /* error */
      pbhead->d2_retcode   = D2_TEMP_ERR;
      pbhead->d2_errclass  = D2_INT_ERR;
      pbhead->d2_errvalue  = D21_RESULTAPDU_INVALID;
      return(D2_ERROR);
}

if (eom_ptr->d23_eom_oqoff == 0)
{
  DUA_TRACE_POQ_EMPTY
  return(D2_NOERROR);
}

oq_ptr = D27_MALLOC(svc_c_sev_warning,eom_ptr->d23_eom_oqoff);

if (oq_ptr == NULL)
{
  pbhead->d2_retcode = D2_TEMP_ERR;
  pbhead->d2_errclass = D2_SYS_ERR;
  pbhead->d2_errvalue = D21_ALLOC_ERROR;
  return (D2_ERROR);
}

offset = -eom_ptr->d23_eom_oqoff;
lseek(file, offset, SEEK_END);

if (d21_read(file,oq_ptr,
	eom_ptr->d23_eom_oqoff) != eom_ptr->d23_eom_oqoff )
    {
       pbhead->d2_retcode = D2_TEMP_ERR;
       pbhead->d2_errclass = D2_SYS_ERR;
       pbhead->d2_errvalue = D2_NOT_SPEC;
       return (D2_ERROR);
      }

if (((D23_part_oq *)oq_ptr)->d23_oqtag != D23_OQTAG)
	  {
	     pbhead->d2_retcode = D2_TEMP_ERR;
	     pbhead->d2_errclass = D2_SYS_ERR;
	     pbhead->d2_errvalue = D2_NOT_SPEC;
	     return (D2_ERROR);
	    }

*limit_pb = ((D23_part_oq *)oq_ptr)->d23_oq_limit;
if (eom_ptr->d23_eom_oqoff != D23_S_POQ + D23_S_CRES + D23_S_EOM)
 { *ref_ptr = (D23_rf_info *)(oq_ptr + D23_S_POQ);
   DUA_TRACE_REFERRALS_FOUND
   }
else
 { *ref_ptr = NULL;
   DUA_TRACE_POQ_EMPTY
 }

return(D2_NOERROR);
} /* end of d21_sc10   */


static d2_ret_val d21_sc11_get_result_buffer(
  D2_pbhead * pbhead,
  D21_req_info * res_pb,
  int fd,
  Bool read_all,
  signed16 * apdu_len,
  Bool * last_buffer)
{
D23_wtrspb              rst_pb;
D23_pstatpb		pstat_pb;
D23_rserror             *err_apdu;
d2_ret_val                   ret_value = D2_NOERROR;
d2_ret_val                     wr_error = D2_NOERROR;
char                    sspec_field[10];
signed32 res;
signed16 mflag;
const char function_name[] = "d21_sc11_get_result_buffer";

/*
set rst_pb
*/
rst_pb.d23_7hdinfo.d2_version = D23_V02;
rst_pb.d23_7assid  = res_pb->d21_r_ipcinfo->d21_i_assid;
rst_pb.d23_7wtime  = D23_INFIN;
if ( read_all == FALSE )
  {
  mflag = D23_LASTFG;
  }
else
  {
  mflag = D23_MRDATA;
  }

for ( ; *last_buffer == FALSE; )
{   /*
       get result (in case of errors or if "read_all" is TRUE,
		   all ipc blocks will be read)
    */
    while ((res = d21_ipcreceive(NULL,&rst_pb,mflag)) == D23_SUCES &&
		    rst_pb.d23_7rltid == D23_NOOPR) ;
    mflag = D23_MRDATA;
    if (res != D23_SUCES) {

	/* set errorcodes */
	pbhead->d2_retcode  = rst_pb.d23_7hdinfo.d2_retcode;
	pbhead->d2_errclass = rst_pb.d23_7hdinfo.d2_errclass;
	pbhead->d2_errvalue = rst_pb.d23_7hdinfo.d2_errvalue;
	return(D2_ERROR);
    }

    if (rst_pb.d23_7rltid == D23_ABORT ||
	(rst_pb.d23_7rltid >= D23_RREJECT &&
	 rst_pb.d23_7rltid <= D23_RREJECT + D23_MXRREJ)) {
	  pbhead->d2_retcode = D2_TEMP_ERR;
	  pbhead->d2_errclass = D2_REM_ERR;
	  pbhead->d2_errvalue = D2_NO_RESPONSE;

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
	  return (D2_ERROR);
    }

    /*
       error-pdu ?
    */
    if (rst_pb.d23_7rltid >= D23_RERROR &&
	rst_pb.d23_7rltid <= D23_RERROR + D23_MXRERR)
    {   /* error-apdu received */
	err_apdu = (D23_rserror *)rst_pb.d23_7rfrdt;
	pbhead->d2_retcode  = err_apdu->d23_Zretcod;
	pbhead->d2_errclass = err_apdu->d23_Zerrcls;
	pbhead->d2_errvalue = err_apdu->d23_Zerrval;
	DUA_TRACE_ERROR_PDU_RECEIVED
    }

    /*
       set address of data, length of apdu and last_buffer flag
    */
    res_pb->d21_r_bufapdu = (char *)rst_pb.d23_7rfrdt;
    *apdu_len = rst_pb.d23_7lnrdt;
    *last_buffer = (rst_pb.d23_7rmdat == D23_MRDATA) ? FALSE : TRUE;

    if (wr_error == D2_NOERROR)
	if (d21_write(fd, res_pb->d21_r_bufapdu,
		 *apdu_len) !=  *apdu_len)
		wr_error = D2_ERROR;

    if (ret_value == D2_NOERROR)
    {   /*
	   if not all blocks are to be read --> break
	   (an error was detected elsewhere in last IPC-block, then
	    IPC-buffers will be cleared)
	*/
	if (read_all == FALSE)
	    break;
    }
}

return(ret_value);

} /* end of d21_sc11 */


d2_ret_val d21_sc13_handle_file_ref(
  D21_lb_info * lb_info,
  signed16 * lb_nr,
  D2_pbhead * pbhead,
  char * req_ptr,
  signed32 req_len,
  time_t maxtime,
  D23_rf_info * ref_ptr,
  signed16 * entries_found,
  signed16 opcode,
  signed16 * limit_pb,
  d21_bind_info * bt_ptr,
  int file)
{
time_t            curr_time;
d2_ret_val           ret_value;
D2_name_string  rdsa_name;
signed16           rbind_id = D2_NO_BIND;
D21_p2          unbd_parblk;
D2_pbhead       u_pbhead;
d21_bind_info  r_b_tab;
signed32 i;
D2_pbhead       ref_pbhead;
				/* indicates whether search on a referral
				   DSA was at least one time successful or
				   not
				*/
Bool loop_detected = FALSE;
Bool            ref_dsa_err = TRUE;
D2_name_string         r_target ;
signed32           offset,old_offset;
signed32		old_req_len;
D23_av_value         * r_psap;
D2_a_value             psap;
char                * req_ptr1 = req_ptr;
char               * apdu_ptr;
Bool                cpy = FALSE;
Bool            err_referral;
Bool r_new_ipc = FALSE;

D23_rf_info    *r_ptr;
const char function_name[] = "d21_sc13";

ref_pbhead.d2_retcode = ref_pbhead.d2_errclass = ref_pbhead.d2_errvalue =
			D2_NO_ERR;

   /*
       handle referrals
    */
    /*
       set pbheads
    */
    u_pbhead.d2_version = pbhead->d2_version;
    err_referral = pbhead->d2_errvalue == D2_REF_ERROR ? TRUE : FALSE;
    pbhead->d2_retcode  = pbhead->d2_errclass =
			    pbhead->d2_errvalue = D2_NO_ERR;
    /*
       start handling of referrals
    */
    while(((D23_rf_info *)ref_ptr)->d23_rftag == D23_REFTAG)
    {
      r_target = (D2_name_string)(ref_ptr + 1);
      if (((D23_ds_name *)r_target)->d23_dslen == 0)
	 r_target = NULL;
      else
	 r_target += D23_S_NM;
      /*
       copy target object as base object in the next operation request ????
       */
       switch (opcode) {
	case D23_LIST: /* copy target object to inlist APDU */
			 offset =  D23_S_HDR + D23_S_CARG;
			 break;
	case (D23_SEARCH+D2_ADM_OPID):
	case D23_SEARCH: /* copy target object to insearch APDU */
			 offset =  D23_S_HDR + D23_S_CARG+D23_S_SCHARG;
			 break;
       }
      apdu_ptr =  req_ptr + offset;
      if (((D23_ds_name *)apdu_ptr)->d23_dslen == 0)
      apdu_ptr = NULL;
      else
      apdu_ptr += D23_S_NM;
      if (apdu_ptr != NULL && r_target != NULL && strcmp(apdu_ptr,
						(char *)r_target) != 0)
      {
       if (strlen(apdu_ptr) >= strlen((char *)r_target))
	  { strcpy(apdu_ptr,(char *)r_target);
	   ((D23_ds_name *)(apdu_ptr-D23_S_NM))->d23_dslen =
						strlen((char *)r_target)+1;
	   }
      else
	 {
	   old_req_len = req_len;
	   req_len += strlen((char *)r_target)-strlen(apdu_ptr);
	   req_len = D2_ALIGN(req_len) ;
	   old_offset = offset += D23_S_NM;
	   old_offset +=  ((D23_ds_name *)(apdu_ptr-D23_S_NM))->d23_dsoff;
	   cpy = TRUE;
	  }
      }
      else
      if (apdu_ptr == NULL && r_target != NULL)
       {
	    old_req_len = req_len;
	    req_len += strlen((char *)r_target); /* increment req_len */
	    req_len = D2_ALIGN(req_len) ;
	    old_offset = offset += D23_S_NM;
	    apdu_ptr =  req_ptr + offset;
	    cpy = TRUE;
       }
      else
       if (apdu_ptr != NULL && r_target == NULL)
	  {
	    *apdu_ptr = (char)0;
	    ((D23_ds_name *)(apdu_ptr-D23_S_NM))->d23_dslen = 0;
	   }
      if (cpy == TRUE)
       {
       if ((req_ptr1=D27_MALLOC(svc_c_sev_warning,req_len)) == NULL)
	{ 
	  pbhead->d2_retcode = D2_TEMP_ERR;
	  pbhead->d2_errclass = D2_SYS_ERR;
	  pbhead->d2_errvalue = D21_ALLOC_ERROR;
	  ret_value = D2_ERROR;
	  break;
	  }
       memcpy(req_ptr1, req_ptr, offset);
       apdu_ptr = req_ptr1+offset;
       strcpy(apdu_ptr,(char *)r_target);
       ((D23_ds_name *)(apdu_ptr-D23_S_NM))->d23_dslen =
						strlen((char *)r_target)+1;
       ((D23_ds_name *)(apdu_ptr-D23_S_NM))->d23_dsoff =
				  	D2_ALIGN(strlen((char *)r_target)+1);
       offset += ((D23_ds_name *)(apdu_ptr-D23_S_NM))->d23_dsoff;
       for (i=old_offset; i < old_req_len-1 ; i++,offset++)
		  *(req_ptr1+offset) = *(req_ptr+i);
       }
				    /*                                        */
				    /*  navigate access points to get one bind*/
				    /*                                        */
       apdu_ptr = (char *)(ref_ptr + 1);
       apdu_ptr += ((D23_ds_name *)apdu_ptr)->d23_dsoff + D23_S_NM;
       for (i=0; i < ref_ptr->d23_nm_acc; i++)
       {
	rdsa_name = (D2_name_string)(apdu_ptr + D23_S_ACC + D23_S_NM);
	r_psap    = (D23_av_value *)(rdsa_name +
		  ((D23_ds_name *)(rdsa_name-D23_S_NM))->d23_dsoff);
	psap.d2_a_v_len = r_psap->d23_avlen;
	psap.d2_a_rep   = D2_PSAP_STX;
	psap.d2_at_value.d2_oct_a_v = (Octet_string)(r_psap +1);
	apdu_ptr = ((char *) r_psap) + r_psap->d23_avoff + D23_S_ATV;

       /*
	Check for LoopBack
       */
       if (d21_cr3_check_lb(lb_info,lb_nr,r_target,
		(D2_op_progress *)&ref_ptr->d23_op_pr, rdsa_name)
			 == TRUE)
         {   DUA_TRACE_LOOP_DETECTED
	      if(err_referral == TRUE)
	      {
	        pbhead->d2_retcode = D2_CALL_ERR;
	        pbhead->d2_errclass = D2_PAR_ERR;
	        pbhead->d2_errvalue = D2_LOOP_DETECTED;
	        ret_value = D2_ERROR;
	      }
	      else
	      {
		loop_detected = TRUE;
	        ret_value = D2_NOERROR;
	      }
	      break;
         }

       /*
       Try bind to DSA if not sucessfull then read next access point(i.e. DSA)
       */
       if ((ret_value = d21_b03_ref_bind(pbhead, rdsa_name,
				    &psap,bt_ptr, &rbind_id)) == D2_NOERROR)
	{
	  break;
	}
       } /* of for loop for acc_points */

	if(ret_value == D2_NOERROR && loop_detected == FALSE)
	{   /* referral DSA could be bound                      */
	    /*
	       add referral to ref_tab
	    */

	     if ((ret_value = d21_cr1_add_lb(lb_info,lb_nr,r_target,
		(D2_op_progress *)&ref_ptr->d23_op_pr,rdsa_name)) != D2_NOERROR)
	      {   
		  pbhead->d2_retcode = D2_TEMP_ERR;
		  pbhead->d2_errclass = D2_SYS_ERR;
		  pbhead->d2_errvalue = D21_ALLOC_ERROR;
		  ret_value = D2_ERROR;
		  break;
	       }
       /*
	  copy operation progress
       */
	      ((D23_com_arg *)(req_ptr1+D23_S_HDR))->d23_cmopr
					    = ref_ptr->d23_op_pr;

	}
       for (i++; i < ref_ptr->d23_nm_acc; i++)
       {
	rdsa_name = (D2_name_string)(apdu_ptr + D23_S_ACC + D23_S_NM);
	r_psap    = (D23_av_value *)(rdsa_name +
		    ((D23_ds_name *)(rdsa_name-D23_S_NM))->d23_dsoff);
	apdu_ptr = ((char *) r_psap) + r_psap->d23_avoff + D23_S_ATV;
       }
       ref_ptr = (D23_rf_info *) apdu_ptr;
       if(ret_value != D2_NOERROR)
	{ /* No bind to any referral */
	  continue;
	}
	/* set block for unbind */
	unbd_parblk.d21_2pbhead = &u_pbhead;
	unbd_parblk.d21_2bind_id = rbind_id;

       /*
	  enough time to start handling of the referral ?
       */
       curr_time = time(0L);
       if ((maxtime != D2_T_UNLIMITED) && (curr_time >= maxtime))
       {   /* set time_limit_exceeded and return */
	   pbhead->d2_retcode = D2_TEMP_ERR;
	   pbhead->d2_errclass = D2_REM_ERR;
	   pbhead->d2_errvalue = D2_TIME_LIMIT_EXCEEDED;
	   /*
	      unbind
	    */
	   d21_bh2_exec_unbind(&unbd_parblk);
	   ret_value = D2_ERROR;
	   break;
       }
       /*
	 set time-limit in read-request and send read-request to remote dsa
       */
       if (d21_bh8_get_bind_info(rbind_id,&r_b_tab,&r_new_ipc,
				pbhead,TRUE) != D2_NOERROR)
         {
         return (D2_ERROR);
         }
       ((D23_header *)req_ptr1)->d23_bindid = 0;

       /* copy other elements of referral to common args ? */
       /* DAP Protocol dependent part */

       if (maxtime != D2_T_UNLIMITED)
       { ((D23_com_arg *)(req_ptr1+D23_S_HDR))->d23_cmctr.d23_sctlm
					    = maxtime - curr_time;
       }
	/*
	   execute search at referred dsa (set cache-flag to false)
	*/
	ret_value = d21_sc10_write_output_to_file(pbhead,&r_b_tab,
						  req_ptr1,req_len, &r_ptr,
						entries_found,opcode, limit_pb,
                                                file,FALSE);

	if (ret_value == D2_ERROR)
	{   /* --> continue with next referral */
	    ret_value = D2_NOERROR;
	    /*
	       store error values from referral-DSA
	    */
	    ref_pbhead.d2_retcode  = pbhead->d2_retcode;
	    ref_pbhead.d2_errclass = pbhead->d2_errclass;
	    ref_pbhead.d2_errvalue = pbhead->d2_errvalue;
	    /*
	       reset pbhead
	    */
	    pbhead->d2_retcode  =
	    pbhead->d2_errclass =
	    pbhead->d2_errvalue = D2_NO_ERR;
	}
	else
	{   ref_dsa_err = FALSE;
	    if (r_ptr == NULL)
	    {
	       /*
		  unbind
	       */
	       d21_bh2_exec_unbind(&unbd_parblk);
	       ret_value = D2_NOERROR;
	       break;
	     }
	    else if(pbhead->d2_errvalue == D2_NO_ERR)
	    {       /* result including referrals, reenter the function */
		if((ret_value = d21_sc13_handle_file_ref(lb_info,lb_nr,pbhead,
		  req_ptr, req_len,maxtime,r_ptr,entries_found,opcode,limit_pb,
		   bt_ptr,file)) != D2_ERROR)
		{
			/* hard error occurred                          */
		    break;
		}
	    }
	}
	/*
	   unbind
	*/
	d21_bh2_exec_unbind(&unbd_parblk);

       if (req_ptr != req_ptr1)
	{
	  free(req_ptr1);
	  req_ptr1 = req_ptr;
	}

    d21_ipc_clear(rbind_id,&r_new_ipc,&r_b_tab);
    }  /* while    */

    /*
       check whether referral-search was at least successfullfy performed
       one time or SEARCH already returned partial result
    */
    if (*entries_found == 0 && ref_dsa_err == TRUE &&
	pbhead->d2_retcode == D2_NO_ERR &&
	pbhead->d2_errclass == D2_NO_ERR &&
	pbhead->d2_errvalue == D2_NO_ERR)
    {   /*
	   exchange error values from referral-DSA
	*/
	pbhead->d2_retcode =  ref_pbhead.d2_retcode;
	pbhead->d2_errclass = ref_pbhead.d2_errclass;
	pbhead->d2_errvalue = ref_pbhead.d2_errvalue;
	ret_value = D2_ERROR;
    }
    else
    {   if (pbhead->d2_retcode != D2_NO_ERR &&
	    pbhead->d2_errclass != D2_NO_ERR &&
	    pbhead->d2_errvalue != D2_NO_ERR)
	{   ret_value = D2_ERROR;
	}
    }

if (rbind_id != D2_NO_BIND) d21_ipc_clear(rbind_id,&r_new_ipc,&r_b_tab);
return(ret_value);

} /* end of d21_sc13_handle_file_ref() */

