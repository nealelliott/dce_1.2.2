/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20updutil.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:35  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:19:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:34  root]
 * 
 * Revision 1.1.8.5  1994/08/16  08:16:00  marrek
 * 	Fix for delta update (ot 11617).
 * 	[1994/08/12  11:59:44  marrek]
 * 
 * Revision 1.1.8.4  1994/06/21  14:43:50  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  15:49:12  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:59  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:13:54  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:49  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:00  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:29  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:35:33  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:43:40  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:37:46  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:17  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:35  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:58:06  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:11:21  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  19:59:41  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:43:07  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20updutil.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:51 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* NAME         : d20updutil.c                                        */
/*								      */
/* AUTHOR       : Heinrichmeyer, D AP 11                              */
/* DATE         : 31.03.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <dce/asn1.h>
#include <d23ipc.h>
#include <d23apdu.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <ros.h>
#include <d2asn1.h>
#include <d20proto.h>
#include <gds_sec.h>

/******** IMPORT ********/

extern char d26_install[];

/******** LOCAL  ********/
				  /* parameter block for d23_bind   */
static  D23_bindpb  ipc_bind_pb = {{D23_V02, 0, 0, 0}};

				  /* parameter block for d23_unbind */
static  D23_ubndpb  ipc_unbind_pb = {{D23_V02, 0, 0, 0}};

				  /* parameter block for d23_invoke */
static   D23_inrqpb  ipc_invoke_pb = {{D23_V02, 0, 0, 0}};

				  /* parameter block for d23_waitrs */
static   D23_wtrspb  ipc_waitrs_pb = {{D23_V02, 0, 0, 0}, D23_INFIN};

static   signed16    cont_id = D23_SAC(D27_DSP_AC) |
			     D23_SACM(D27_LOC_AC) |
			     D23_STS(D27_PRIV_TS);

static Bool auth_mech[D2_MAX_AUTH_MECHS];

/* LOCAL FUNCTIONS */

static signed32 d20d051_invexe( Bool , D2_pbhead * , char * , signed16 );
static signed32 d20d052_make_bind( D2_pbhead * , signed16 , D2_name_string );
static signed32 d20d053_get_psap_auth_mech( D2_pbhead * , signed16 ,
	D2_name_string , char **, char **, Bool * );
static signed32 d20d054_gen_bind_message( D2_pbhead * , signed16 , char * ,
	char *, D2_name_string , Bool , signed16 * , unsigned32 * , 
	signed16 *, char ** , signed32 * );

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20d050_update_dsa()                                  */
/*								      */
/* AUTHOR:      Heinrichmeyer, D AP 11                                */
/* DATE:        31.03.89                                              */
/*								      */
/* DESCRIPTION: This function sends the given update message to the   */
/*              remote DSA using the S-STUB.                          */
/*								      */
/*              Before sending the message it checks whether a DS_BIND*/
/*              to the remote DSA exists. When no bind exist the      */
/*              bind APDU is read from the local DSA and the remote   */
/*              DS_BIND is executed via S-STUB.                       */
/*								      */
/*              When the parameter last_operation == TRUE the         */
/*              DS_UNBIND to remote DSA, the IPC-unbind and the       */
/*              IPC detach client are done only.                      */
/*								      */
/*              When errors occur the parameter pbhead is supplied.   */
/*              (Errors occurred in the other components are given in */
/*              pbhead too).                                          */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*       D2_pbhead      *pbhead          INPUT-OUTPUT PARAMETER       */
/*                                       -> ptr to error information  */
/*                                       <- error codes for error     */
/*                                          trace when function fails */
/*                                                                    */
/*       Bool           last_operation   indicator whether DS_UNBIND  */
/*                                       should be done only          */
/*                                       TRUE: make DS_UNBIND         */
/*                                                all the other       */
/*                                                parameters except   */
/*                                                pbhead are ignored  */
/*                                       FALSE: send update msg to    */
/*                                                 remote DSA         */
/*                                                                    */
/*       signed16       dir_id           directory id                 */
/*                                                                    */
/*       D2_name_string dsa_name         name of remote DSA           */
/*                                                                    */
/*       signed32       operation_id     update operation which should*/
/*                                       be executed by the remote DSA*/
/*                                                                    */
/*       char           *msg_ptr         update invoke APDU containing*/
/*                                       data for update function     */
/*                                                                    */
/*       signed32       msg_len          length of msg_ptr            */
/*                                                                    */
/* OUTPUT-PARAMETERS:  none                                           */
/*								      */
/* RETURNVALUE:							      */
/*     d2_ret_val       D2_NOERROR:     no errors                     */
/*								      */
/*              D2_UPD_ERR:     last_operation == D2_TRUE:            */
/*                                DS_UNBIND in remote DSA fails       */
/*                              last_operation == D2_FALSE:           */
/*                                update operation in remote DSA fails*/
/*                                continue with next update operation */
/*                                in the same remote DSA              */
/*								      */
/*              D2_NEXT_DSA     Bind to remote DSA cannot be executed */
/*                              (e.g. bind message is not given by    */
/*                               local DSA or remote DSA is not       */
/*                               available)                           */
/*                              continue with next DSA                */
/*								      */
/*              D2_FATAL_ERR    fatal error occurred (e.g. IPC returns*/
/*                              error)                                */
/*                              work cannot be continued              */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*       LOCAL D23_bindpb  ipc_bind_pb    par. block for d23_bind     */
/*       LOCAL D23_ubndpb  ipc_unbind_pb  par. block for d23_unbind   */
/*       LOCAL D23_inrqpb  ipc_invoke_pb  par. block for d23_invoke   */
/*								      */
/*exoff ***************************************************************/

signed32 d20d050_update_dsa(
  D2_pbhead * pbhead,
  Bool last_operation,
  signed16 dir_id,
  D2_name_string dsa_name,
  signed32 operation_id,
  char * msg_ptr,
  signed32 msg_len)
{
static Bool        bind_exists = FALSE;
static signed16    bind_id;
static signed16    dirid;
static Bool        loaded = FALSE;
static char function[] = "d20d050_update_dsa";

signed32    return_value = D2_NOERROR;
asn1_field  asn1_cod;
char        err_apdu[sizeof(D23_rserror) + sizeof(D23_eom)];

register D23_inunbind  *inunbind;

register char    *data_ref;            /* pointer to actual position   */
				       /* of complete DSA message      */
register signed32  apdu_len;           /* total message length         */

register D23_rsbind    *rsbind;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

d27_002_set_error_code (pbhead, D2_NO_ERR, D2_NOT_SPEC, D2_NOT_SPEC);

/***************************/
/* last operation received */
/***************************/
if
  (last_operation == TRUE)
{   
  if
    (bind_exists == TRUE)
  {   
    bind_exists = FALSE;

    /***************************/
    /* DS_UNBIND to remote DSA */
    /***************************/
    /* build invoke message */
    inunbind = (D23_inunbind *) (ipc_bind_pb.d23_4refms);

    inunbind->d23_Chdr.d23_versno = D23_APDUV22;
    inunbind->d23_Chdr.d23_dirid  = dirid;
    inunbind->d23_Chdr.d23_bindid = bind_id;
    inunbind->d23_Ceom.d23_eomtag = D23_EOMTAG;

    /* send message */
    ipc_invoke_pb.d23_6assid = ipc_bind_pb.d23_4assid;
    ipc_invoke_pb.d23_6invinfo.d23_invid = D23_UNUSED;
    ipc_invoke_pb.d23_6invinfo.d23_usrid = ipc_invoke_pb.d23_6assid;
    ipc_invoke_pb.d23_6oprid = D23_UNBIND;
    cont_id = ipc_invoke_pb.d23_6invinfo.d23_contid =
					  D23_SAC(D27_DSP_AC) |
					  D23_SACM(D27_LOC_AC) |
					  D23_STS(D27_PRIV_TS) |
					  D23_SAS(D27_DSP_AS);

    return_value = d20d051_invexe (FALSE, pbhead, (char *)inunbind, 
                                   sizeof (D23_inunbind));
    /*************************/
    /* ipc unbind for S-STUB */
    /*************************/
    ipc_unbind_pb.d23_5assid = ipc_bind_pb.d23_4assid;
    if ((d23_unbind (&ipc_unbind_pb)) == D23_ERROR)
    {   
      d27_002_set_error_code (pbhead,
			      ipc_unbind_pb.d23_5hdinfo.d2_retcode,
			      ipc_unbind_pb.d23_5hdinfo.d2_errclass,
			      ipc_unbind_pb.d23_5hdinfo.d2_errvalue);
      return_value = D2_FATAL_ERR;
    } 

  }   /* bind_exists == TRUE */
  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_value));
  return (return_value);
}   /* last_operation == TRUE */

/******************************************/
/* if first call: make bind to remote DSA */
/******************************************/
/* first call */

if (bind_exists == FALSE)
  {   
  /******************************************/
  /* make DS_BIND to target dsa             */
  /******************************************/

  if ((return_value = d20d052_make_bind(pbhead, dir_id, dsa_name)) !=
								D2_NOERROR)
    {
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_value));
    return(return_value);
    }

  /* get bind_id */
  rsbind = (D23_rsbind *) ipc_waitrs_pb.d23_7rfrdt;
  bind_id = rsbind->d23_Bhdr.d23_bindid;

  dirid = dir_id;

  bind_exists = TRUE;

  }  /* first call */

/***********************************/
/* make delta update by remote DSA */
/***********************************/
if ((D23_GTS(cont_id) == D27_ASN1_TS) || (D23_GTS(cont_id) == D27_ASN1_ADM_TS))
  {               /* ANS.1 encoding necessary */

  if (loaded == FALSE)
    {   
	if (d27_307_init_att (d26_install) == D2_ERROR)
	{   
	  DCE_SVC_LOG((GDS_S_SUP_DEBUG_INFO_MSG, 
		"d050: ASN.1 attribute table can't be loaded"));
	  d27_002_set_error_code (pbhead, D2_TEMP_ERR, D2_INT_ERR,
				    D2_NO_RESPONSE);
	  /* mac - do a directory unbind 
	       call d20d050_update_dsa to do this in the future */
    	  /* build invoke message */
    	  inunbind = (D23_inunbind *) (ipc_bind_pb.d23_4refms);

    	  inunbind->d23_Chdr.d23_versno = D23_APDUV22;
    	  inunbind->d23_Chdr.d23_dirid  = dirid;
    	  inunbind->d23_Chdr.d23_bindid = bind_id;
    	  inunbind->d23_Ceom.d23_eomtag = D23_EOMTAG;

          /* send message */
          ipc_invoke_pb.d23_6assid = ipc_bind_pb.d23_4assid;
	  ipc_invoke_pb.d23_6invinfo.d23_invid = D23_UNUSED;
	  ipc_invoke_pb.d23_6invinfo.d23_usrid = ipc_invoke_pb.d23_6assid;
          ipc_invoke_pb.d23_6oprid = D23_UNBIND;
          cont_id = ipc_invoke_pb.d23_6invinfo.d23_contid =
						  D23_SAC(D27_DSP_AC) |
					          D23_SACM(D27_LOC_AC) |
					          D23_STS(D27_PRIV_TS) |
					          D23_SAS(D27_DSP_AS);

          return_value = d20d051_invexe (FALSE, pbhead, (char *)inunbind, 
                                   sizeof (D23_inunbind));
          if (return_value != D2_NOERROR)
            {   
	         DCE_SVC_LOG((GDS_S_SUP_DEBUG_INFO_MSG, 
	         	"d050: DS_UNBIND(rem DSA) fails"));
            } 
	    /* mac - end */

	  ipc_unbind_pb.d23_5assid = ipc_bind_pb.d23_4assid;
	  d23_unbind (&ipc_unbind_pb);
	  bind_exists = FALSE;
  	  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NEXT_DSA));
	  return (D2_NEXT_DSA);
	}
else
{
	    loaded = TRUE;
	} 
    } 

    DCE_SVC_LOG((GDS_S_APDU_REQUEST_MSG,operation_id,msg_len,msg_ptr));
    if (d2a000_apdu_asn1(operation_id, D27_DSP_AS, (byte *)msg_ptr, msg_len,
			 &asn1_cod, (byte *)err_apdu) != D2_NOERROR)
    {   
	d27_002_set_error_code (pbhead, D2_TEMP_ERR, D2_INT_ERR,
				D2_NO_RESPONSE);

	/* mac - do a directory unbind 
	   call d20d050_update_dsa to do this in the future */
    	/* build invoke message */
    	inunbind = (D23_inunbind *) (ipc_bind_pb.d23_4refms);

    	inunbind->d23_Chdr.d23_versno = D23_APDUV22;
    	inunbind->d23_Chdr.d23_dirid  = dirid;
    	inunbind->d23_Chdr.d23_bindid = bind_id;
    	inunbind->d23_Ceom.d23_eomtag = D23_EOMTAG;

        /* send message */
        ipc_invoke_pb.d23_6assid = ipc_bind_pb.d23_4assid;
	ipc_invoke_pb.d23_6invinfo.d23_invid = D23_UNUSED;
	ipc_invoke_pb.d23_6invinfo.d23_usrid = ipc_invoke_pb.d23_6assid;
        ipc_invoke_pb.d23_6oprid = D23_UNBIND;
        cont_id = ipc_invoke_pb.d23_6invinfo.d23_contid =
					      D23_SAC(D27_DSP_AC) |
					      D23_SACM(D27_LOC_AC) |
					      D23_STS(D27_PRIV_TS) |
					      D23_SAS(D27_DSP_AS);

        return_value = d20d051_invexe (FALSE, pbhead, (char *)inunbind, 
                                   sizeof (D23_inunbind));
        if (return_value != D2_NOERROR)
        {   
	  DCE_SVC_LOG((GDS_S_SUP_DEBUG_INFO_MSG, 
	    "d050: DS_UNBIND(rem DSA) fails"));
        } 
	/* mac - end */

	ipc_unbind_pb.d23_5assid = ipc_bind_pb.d23_4assid;
	d23_unbind (&ipc_unbind_pb);
	bind_exists = FALSE;
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NEXT_DSA));
	return (D2_NEXT_DSA);
    } 
    apdu_len = asn1_cod.l;
    data_ref = (char *)asn1_cod.v;
    ipc_invoke_pb.d23_6invinfo.d23_contid =
				D23_SAC(D27_DSP_AC) |
				D23_SACM(D27_LOC_AC) |
				D23_STS(D27_ASN1_TS) |
				D23_SAS(D27_DSP_AS);
}
else
  {
    apdu_len = msg_len;             /* total length of DSA message */
    data_ref = msg_ptr;             /* ptr to begin of DSA message */
    ipc_invoke_pb.d23_6invinfo.d23_contid =
				D23_SAC(D27_DSP_AC) |
				D23_SACM(D27_LOC_AC) |
				D23_STS(D27_PRIV_TS) |
				D23_SAS(D27_DSP_AS);
  } 

/* set fix values of send invoke parameter */
ipc_invoke_pb.d23_6assid = ipc_bind_pb.d23_4assid;
ipc_invoke_pb.d23_6invinfo.d23_invid = D23_UNUSED;
ipc_invoke_pb.d23_6invinfo.d23_usrid = ipc_invoke_pb.d23_6assid;
ipc_invoke_pb.d23_6oprid = operation_id;

/* send invoke and wait for result */
return_value = d20d051_invexe (FALSE, pbhead, data_ref, apdu_len);

/* release memory, if invoke was ASN.1 encoded */
if ((D23_GTS(cont_id) == D27_ASN1_TS) || (D23_GTS(cont_id) == D27_ASN1_ADM_TS))
{   
  free(asn1_cod.v);
} 

if (return_value != D2_NOERROR)
{   
     /*
     d27_009_trace_dn ("d050: delta upd(rem DSA) fails", "");
     */
     if (return_value == D2_FATAL_ERR)
     {   
	  bind_exists = FALSE;
     } 
} 

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_value));
return (return_value);

} /* end of d20d050_update_dsa */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20d051_invexe ()                                     */
/*								      */
/* AUTHOR:      Heinrichmeyer, D AP 11                                */
/* DATE:        31.03.89                                              */
/*								      */
/* DESCRIPTION: This function sends message blocks using d23_invoke() */
/*              and waits for result (d23_waitrs() ).                 */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*       Bool             do_unbind      indicator whether d23_unbind */
/*                                       should be done               */
/*                                       when server returns error    */
/*                                       APDU                         */
/*                                       TRUE: call d23_unbind        */
/*                                       FALSE: the ipc functions     */
/*                                                 are not called     */
/*                                                                    */
/*       D2_pbhead        *head          INPUT-OUTPUT PARAMETER       */
/*                                       -> ptr to error information  */
/*                                       <- error codes for error     */
/*                                          trace when function fails */
/*                                                                    */
/*       char             *mesg          the message to be sent       */
/*                                       NULL unless a delta update   */
/*                                       is to be done, in which case */
/*                                       the message may be large and */
/*                                       may need to be split         */
/*                                                                    */
/*       signed32         mesg_len       the message length           */
/*                                                                    */
/* OUTPUT-PARAMETERS: none                                            */
/*								      */
/* RETURNVALUE:							      */
/*    signed32  D2_NOERROR:     no errors                             */
/*              D2_NEXT_DSA:    server function return error APDU     */
/*                              and do_unbind == TRUE                 */
/*              D2_UPD_ERR:     server function return error APDU     */
/*                              and do_unbind == FALSE                */
/*              D2_FATAL_ERR:   IPC-function returns error            */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*       LOCAL D23_inrqpb  ipc_invoke_pb  par. block for d23_invoke   */
/*       LOCAL D23_wtrspb  ipc_waitrs_pb  par. block for d23_waitrs   */
/*       LOCAL D23_ubndpb  ipc_unbind_pb  par. block for d23_unbind   */
/*								      */
/*exoff ***************************************************************/

static signed32 d20d051_invexe(
  Bool do_unbind,
  D2_pbhead * head,
  char * mesg,
  signed16 mesg_len)
{
 static char function[] = "d20d051_invexe";
 asn1_field asn1_buf;
 char       err_apdu[sizeof(D23_rserror) + sizeof(D23_eom)];
 char      *apdu;
 signed32 apdu_length;
 char      *data_ref;
 signed32   data_len;

register char     *p_msg;
register signed16  blck_len;             /* length of ipc message block  */

blck_len = ipc_bind_pb.d23_4lenms;     /* length of IPC message block */

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

for ( ; ; )
{                       
  p_msg = (char *)ipc_bind_pb.d23_4refms;
  ipc_invoke_pb.d23_6rfidt = ipc_bind_pb.d23_4refms;
  memcpy (p_msg, mesg, mesg_len);
  ipc_invoke_pb.d23_6lnidt = mesg_len;

  if (mesg_len > blck_len)
  {   
      data_len = mesg_len;	       /* save ptr to mesg & mesg len */
      data_ref = mesg;
      ipc_invoke_pb.d23_6lnidt = blck_len;
      ipc_invoke_pb.d23_6imdat = D23_MRDATA;
      
      /* build message blocks and send blocks */
      while (data_len > blck_len)  /*  mesg must be split into several blocks */
        {                            /*  copy block                           */
        memcpy (p_msg, data_ref, blck_len);
			           /*  send invoke                            */
        if                         /* d23_invoke fails                        */
  	    ((d23_invoke (&ipc_invoke_pb)) == D23_ERROR)
        {                          /* handle ipc error                        */
            d27_002_set_error_code (head,
			            ipc_invoke_pb.d23_6hdinfo.d2_retcode,
			            ipc_invoke_pb.d23_6hdinfo.d2_errclass,
			            ipc_invoke_pb.d23_6hdinfo.d2_errvalue);
            ipc_unbind_pb.d23_5assid = ipc_bind_pb.d23_4assid;
            d23_unbind (&ipc_unbind_pb);
  	    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
            return (D2_FATAL_ERR);
        }                          /* d23_invoke fails                        */
   			           /*  go to next block                       */
        data_len -= blck_len;
        data_ref += blck_len;
      }                            /*  while (apdu_len > blck_len             */

      /* send last message block */
      memcpy (p_msg, data_ref, data_len); 
  			           /*  send invoke                            */
      ipc_invoke_pb.d23_6lnidt = data_len;

  } 

  ipc_invoke_pb.d23_6imdat = D23_LASTFG;
			/*  send invoke message                      */
  if                    /*  d23_invoke fails                         */
     ((d23_invoke (&ipc_invoke_pb)) == D23_ERROR)
  {                     /*  handle ipc error                         */
    d27_002_set_error_code (head,
			    ipc_invoke_pb.d23_6hdinfo.d2_retcode,
			    ipc_invoke_pb.d23_6hdinfo.d2_errclass,
			    ipc_invoke_pb.d23_6hdinfo.d2_errvalue);
    ipc_unbind_pb.d23_5assid = ipc_bind_pb.d23_4assid;
    d23_unbind (&ipc_unbind_pb);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
    return (D2_FATAL_ERR);
  } 

			/*  wait for result                         */
  ipc_waitrs_pb.d23_7assid = ipc_bind_pb.d23_4assid;
  if                    /*  d23_waitrs fails                         */
     ((d23_waitrs (&ipc_waitrs_pb)) == D23_ERROR)
    {                     /*  handle ipc error                         */
    d27_002_set_error_code (head,
			   ipc_waitrs_pb.d23_7hdinfo.d2_retcode,
			   ipc_waitrs_pb.d23_7hdinfo.d2_errclass,
			   ipc_waitrs_pb.d23_7hdinfo.d2_errvalue);
    ipc_unbind_pb.d23_5assid = ipc_bind_pb.d23_4assid;
    d23_unbind (&ipc_unbind_pb);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
    return (D2_FATAL_ERR);
    } 

  if (ipc_waitrs_pb.d23_7rltid == D23_ABORT) {
        d27_002_set_error_code (head, D2_TEMP_ERR, D2_INT_ERR, D2_NO_RESPONSE);
	ipc_unbind_pb.d23_5assid = ipc_bind_pb.d23_4assid;
	d23_unbind (&ipc_unbind_pb);
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NEXT_DSA));
	return (D2_NEXT_DSA);
  }

  if    (ipc_waitrs_pb.d23_7rltid >= D23_RREJECT &&
	 ipc_waitrs_pb.d23_7rltid <= D23_RREJECT + D23_MXRREJ) {
        d27_002_set_error_code (head, D2_TEMP_ERR, D2_INT_ERR, D2_NO_RESPONSE);
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_UPD_ERR));
	return (D2_UPD_ERR);
  }

  p_msg = (char *)ipc_waitrs_pb.d23_7rfrdt;

  /* set context ID */
  cont_id = ipc_waitrs_pb.d23_7invinfo.d23_contid;

  if ((D23_GTS(cont_id)==D27_ASN1_TS) || (D23_GTS(cont_id)==D27_ASN1_ADM_TS))
  {   
    if (ipc_waitrs_pb.d23_7rltid >= D23_RERROR &&
	ipc_waitrs_pb.d23_7rltid <= D23_RERROR + D23_MXRERR)
    {                   /* decode ASN.1 error                */
      asn1_buf.l = ipc_waitrs_pb.d23_7lnrdt;
      asn1_buf.v = (asn1)p_msg;
      if (d2a047_asn_error_apdu((byte **)&apdu, &apdu_length,
				ipc_waitrs_pb.d23_7rltid - D23_RERROR,
				D27_DSP_AS,
				&asn1_buf, (byte *)err_apdu) != D2_NOERROR)
      {   
	d27_002_set_error_code (head, D2_TEMP_ERR, D2_INT_ERR,
				D2_NO_RESPONSE);
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_UPD_ERR));
	return (D2_UPD_ERR);
      } 
      p_msg = apdu;
    }
else
{                   /* result */
      DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NOERROR));
      return(D2_NOERROR);
    } 
  }
else
{
    if (ipc_waitrs_pb.d23_7rltid == D23_RESULT)
    {   
      DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NOERROR));
      return(D2_NOERROR);
    } 
    if (ipc_waitrs_pb.d23_7rltid < D23_RERROR ||
	ipc_waitrs_pb.d23_7rltid > D23_RERROR + D23_MXRERR)
    {   
			/* wrong result type                 */
      d27_002_set_error_code (head, D2_PERM_ERR, D2_SYS_ERR, D2_NOT_SPEC);
      if (do_unbind == TRUE)
      {   
	ipc_unbind_pb.d23_5assid = ipc_bind_pb.d23_4assid;
	d23_unbind (&ipc_unbind_pb);
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NEXT_DSA));
	return (D2_NEXT_DSA);
      }
else
{
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_UPD_ERR));
	return (D2_UPD_ERR);
      } 
    } 
  } 

  if                  /* server is busy                    */
     ( ((D23_rserror *) p_msg)->d23_Zerrval == D2_TOO_BUSY)
  {                   /* wait some time and try again      */
    sleep(5);
    continue;
  } 
		    /* copy error code                    */
  d27_002_set_error_code (head,
			  ((D23_rserror *) p_msg)->d23_Zretcod,
			  ((D23_rserror *) p_msg)->d23_Zerrcls,
			  ((D23_rserror *) p_msg)->d23_Zerrval);
  if ((D23_GTS(cont_id)==D27_ASN1_TS) || (D23_GTS(cont_id)==D27_ASN1_ADM_TS))
  {   
    free(apdu);
  } 
  if (do_unbind == TRUE)
  {   
    ipc_unbind_pb.d23_5assid = ipc_bind_pb.d23_4assid;
    d23_unbind (&ipc_unbind_pb);
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NEXT_DSA));
    return (D2_NEXT_DSA);
  }
else
{
    DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_UPD_ERR));
    return (D2_UPD_ERR);
  } 

  break;
}  /* end of infinite for loop */

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NOERROR));
return (D2_NOERROR);
} /* end of d20d051_invexe */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20d052_make_bind ()                         	      */
/*								      */
/* AUTHOR:      Sanjay jain, D AP 11                                  */
/* DATE:        01.12.93                                              */
/*								      */
/* DESCRIPTION: This function makes bind to the target DSA. It	      */
/*			- gets the psap and auth_meth of the target   */
/*			  DSA					      */
/*			- generates the bind message		      */
/*			- sends the bind message and waits for the    */
/*			  bind result				      */
/*			- analyses the bind result		      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed16		dir_id;			      */
/*		D2_name_string		dsa_name;		      */
/*                                                                    */
/* OUTPUT-PARAMETERS: none                                            */
/*		D2_pbhead		*pbhead;		      */
/*								      */
/* RETURNVALUE:							      */
/*		D2_NOERROR	bind succeeds			      */
/*		D2_FATAL_ERR	bind fails			      */
/*		return value from incexe() function 	bind fails    */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

static signed32 d20d052_make_bind(
  D2_pbhead * pbhead,
  signed16 dir_id,
  D2_name_string dsa_name)
{
static char	function[] = "d20d052_make_bind";
unsigned16    	association_id;
Bool		auth_mech_known;
char		*psap_av_value;
char		*pr_name_av_value;
signed16	desired_auth_mech;
unsigned32	sec_ctx_id;
D2_str		return_sec_token;
D2_str		*ret_sec_tok_ptr;
D2_sec_status	status;
signed32	return_value;
char		*p_msg;	/* pointer to analyse the bind result.	*/
asn1_field	asn1_result;
char       	err_apdu[sizeof(D23_rserror) + sizeof(D23_eom)];
byte		*bind_res;
signed32	bind_res_len;
signed16	dsa_init_status;

Bool		bind_ok;
Bool		bind_retry;
signed32	no_trials;

/* vars for bind msg apdu.	*/
char		*request_ptr;
signed32	request_len;

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

/* ipc_bind to the local DSA */

ipc_bind_pb.d23_4svrid = D23_DS0SID + dir_id;
if ((d23_bind (&ipc_bind_pb)) == D23_ERROR)
	{
	d27_002_set_error_code (pbhead,
			     ipc_bind_pb.d23_4hdinfo.d2_retcode,
			     ipc_bind_pb.d23_4hdinfo.d2_errclass,
			     ipc_bind_pb.d23_4hdinfo.d2_errvalue);
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
	return (D2_FATAL_ERR);
	}

/* save association id.	*/

association_id = ipc_bind_pb.d23_4assid;

/* get psap address and auth_mechs of the target DSA.	*/
/* psap_av_value points to the IPC memory and authentication mechs are set */
/* in the global array.							   */

if (d20d053_get_psap_auth_mech (pbhead, dir_id, dsa_name, &psap_av_value, 
				&pr_name_av_value, &auth_mech_known) 
				!= D2_NOERROR)
	{
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
	return (D2_FATAL_ERR);
	}

/* ipc_unbind from the local DSA.	*/
ipc_unbind_pb.d23_5assid = association_id;
d23_unbind (&ipc_unbind_pb);

/* ipc_bind to S-STUB */

ipc_bind_pb.d23_4svrid = D23_SSTSID + dir_id;

if ((d23_bind (&ipc_bind_pb)) == D23_ERROR)
	{
	d27_002_set_error_code (pbhead,
			     ipc_bind_pb.d23_4hdinfo.d2_retcode,
			     ipc_bind_pb.d23_4hdinfo.d2_errclass,
			     ipc_bind_pb.d23_4hdinfo.d2_errvalue);

	/* ipc_unbind from the local DSA.	*/
	ipc_unbind_pb.d23_5assid = association_id;
	d23_unbind (&ipc_unbind_pb);
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
	return (D2_FATAL_ERR);
	}

/* if supported authentication mechanisms for the target DSA is known then */
/* bind is tried only once otherwise first DCE bind is tried and if that   */
/* fails then ANONYMOUS bind is tried.					   */

return_value = D2_NOERROR;
bind_ok = FALSE;
bind_retry = FALSE;
no_trials = 1;
desired_auth_mech = D2_DCE_AUTH; /* used by gen_bind_message function if */
				 /* auth_mech is not known.		 */

/* this loop is executed at most 2 times. */
for (;;)
	{
	/* generate the bind message for the target DSA.	*/

	if (d20d054_gen_bind_message (pbhead, dir_id, psap_av_value, 
		pr_name_av_value, dsa_name, auth_mech_known, 
		&desired_auth_mech, &sec_ctx_id, &dsa_init_status, 
		&request_ptr, &request_len) != D2_NOERROR)
		{
		return_value = D2_FATAL_ERR;

		/* go to place where security exit function calls are made */
		ret_sec_tok_ptr = NULL;
		goto SEC_EXIT;	
		}

	/* send the bind request and wait for the bind result from the 	*/
	/* target DSA. 							*/

        ipc_invoke_pb.d23_6invinfo.d23_contid =
				D23_SAC(D27_DSP_AC) |
				D23_SACM(D27_LOC_AC) |
				D23_STS(D27_ASN1_TS) |
				D23_SAS(D27_DSP_AS);
	ipc_invoke_pb.d23_6oprid = D23_BIND;

	bind_res = NULL;

	/* send invoke and wait for result */
	if ((return_value = d20d051_invexe (TRUE, pbhead, request_ptr, 
					    request_len)) != D2_NOERROR)
		{
		if ((return_value == D2_FATAL_ERR) || 
		    (return_value == D2_UPD_ERR) ||
		    (return_value == D2_NEXT_DSA && pbhead->d2_errvalue != 
						    D2_INAPPR_AUTH))
			bind_retry = FALSE;
		else
			{
			/* case when the target DSA returns D2_INAPPR_AUTH */
			/* error code.					   */

			return_value = D2_NOERROR;

			if (auth_mech_known == FALSE && no_trials == 1)
				{
				bind_retry = TRUE; 

				/* make ipc bind to S-Stub again.	*/
				ipc_bind_pb.d23_4svrid = D23_SSTSID + dir_id;

				if ((d23_bind (&ipc_bind_pb)) == D23_ERROR)
					{
					d27_002_set_error_code (pbhead,
					ipc_bind_pb.d23_4hdinfo.d2_retcode,
					ipc_bind_pb.d23_4hdinfo.d2_errclass,
					ipc_bind_pb.d23_4hdinfo.d2_errvalue);

					return_value = D2_FATAL_ERR;
					}
				}
			else
				bind_retry = FALSE;
			}

		ret_sec_tok_ptr = NULL;  
			
		}
	else
		{
		/* handle the bind result.	*/

		ret_sec_tok_ptr = &return_sec_token;

		/* cont_id is set in d051_invexe() function.	*/
		if ((D23_GTS(cont_id)==D27_ASN1_TS) || 
		    (D23_GTS(cont_id)==D27_ASN1_ADM_TS))
			{
			/* asn.1 decode the bind result. */

			/* D2A_BIND_RES is the operation ID used in asn1 */
			/* encoding/decoding function. It is taken from  */
			/* d26_m31_map_oprid() in d26mutil.c 		 */

			asn1_result.v = (Octet_string)ipc_waitrs_pb.d23_7rfrdt;
			asn1_result.l = ipc_waitrs_pb.d23_7lnrdt;

			if (d2a001_asn1_apdu(D23_BIND + D2_RES_OPID, D27_DSP_AS, 
			    &bind_res, &bind_res_len, &asn1_result,
						(byte *)err_apdu) != D2_NOERROR)
				{
				d27_002_set_error_code (pbhead, D2_PERM_ERR, 
						D2_REM_ERR, D2_NOT_SPEC);

				return_value = D2_FATAL_ERR;
				}
			else
				p_msg = (char *)bind_res;
			}
		else
			p_msg = (char *)ipc_waitrs_pb.d23_7rfrdt;

		if (return_value != D2_NOERROR)
			{
			/* asn1 decoding failed.	*/
			ret_sec_tok_ptr = NULL;  
			}
		else
			{
			/* analyse the bind result.			*/
			/* and extract the return security token.	*/

			switch ((int)((D23_rsbind *)p_msg)->d23_Bcrkind)
				{
				case D23_NO_CRED:
					return_sec_token.d2_value = NULL;
					return_sec_token.d2_size = 0;
					break;

				case D23_SIMPLE_CRED:
					p_msg = p_msg + sizeof (D23_rsbind);
					return_sec_token.d2_value = (unsigned 
					char *)(p_msg + sizeof(D23_simcr_info));

					/*size is immaterial since D23_ds_name*/
				        /* and D23_pw_info structures follow. */
					return_sec_token.d2_size = 0;
					break;
			
				case D23_STRONG_CRED:
					if ((D23_GTS(cont_id)==D27_ASN1_TS) || 
					    (D23_GTS(cont_id)==D27_ASN1_ADM_TS))
						{
						/* bind result is asn1 encoded*/
						/* security token             */
						return_sec_token.d2_value = 
						(unsigned char *)ipc_waitrs_pb.
								d23_7rfrdt;
						return_sec_token.d2_size =
						ipc_waitrs_pb.d23_7lnrdt;
						}
					else
						{
						p_msg = p_msg + 
							sizeof (D23_rsbind);
						return_sec_token.d2_value = 
						(unsigned char *)(p_msg + 
							sizeof(D23_strcr_info));
						return_sec_token.d2_size =
						((D23_strcr_info *)p_msg)->
								d23_strlen;
						}
					break;

				case D23_EXTERNAL_CRED:
					p_msg = p_msg + sizeof (D23_rsbind);
					return_sec_token.d2_value = 
					(unsigned char *)(p_msg + 
							sizeof(D23_extcr_info));
					return_sec_token.d2_size =
					((D23_extcr_info *)p_msg)->d23_extlen;
					break;

				default:
					break;
				}
			}
		}

	/* free space allocated for the DSP bind message.	*/
	free (request_ptr);

	/* space allocated by the asn1_to_apdu function is freed here.	*/
	if (bind_res != NULL)
		free(bind_res);

SEC_EXIT:

	/*  following operations are performed even if the bind fails */
	/* or no bind has been performed.			      */

	/* confirm the security context if init_sec_ctx succeeded.	*/
	if (dsa_init_status != D2_NOERROR)
		{
		if (dsa_conf_sec_ctx(desired_auth_mech, sec_ctx_id, 
			ret_sec_tok_ptr,&status) == D2_ERROR)
			{
			d27_002_set_error_code (pbhead, status.d2_retcode, 
					status.d2_errclass, status.d2_errvalue);

			return_value = D2_FATAL_ERR;
			}
		else
			if (ret_sec_tok_ptr != NULL)
				{
				/* bind result was received case */
				bind_ok = TRUE;
				}
		}

	/* delete the credentials acquired in the gen_bind_message function. */
	if (dsa_del_cred(desired_auth_mech, &status) == D2_ERROR)
		{
		d27_002_set_error_code (pbhead, status.d2_retcode, 
					status.d2_errclass, status.d2_errvalue);

		return_value = D2_FATAL_ERR;
		}

	if (return_value != D2_NOERROR || bind_ok == TRUE)
		break;

	if (bind_retry == TRUE)
		{
		no_trials = 2;
		continue;
		}
	else
		break;
	}

/* if bind not successful, make ipc unbind.	*/
if (return_value != D2_NOERROR)
	{
	ipc_unbind_pb.d23_5assid = ipc_bind_pb.d23_4assid;
	d23_unbind (&ipc_unbind_pb);
	}

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,return_value));
return(return_value);
}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20d053_get_psap_auth_mech ()                         */
/*								      */
/* AUTHOR:      Sanjay jain, D AP 11                                  */
/* DATE:        01.12.93                                              */
/*								      */
/* DESCRIPTION: This function gets the PSAP address and AUTH_MECHS    */
/* supported by the target DSA from the LOCAL DSA.		      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed16	dir_id;				      */
/*		D2_name_string	dsa_name;			      */
/*                                                                    */
/* OUTPUT-PARAMETERS: none                                            */
/*		D2_pbhead	*pbhead;			      */
/*		char		**psap_av_value;		      */
/*		Bool		*auth_mech_known;		      */
/*								      */
/* RETURNVALUE:							      */
/*		D2_FATAL_ERR					      */
/*		D2_NOERROR					      */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*		auth_mech[]					      */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

static signed32 d20d053_get_psap_auth_mech(
  D2_pbhead * pbhead,
  signed16 dir_id,
  D2_name_string dsa_name,
  char ** psap_av_value,
  char ** pr_name_av_value,
  Bool * auth_mech_known)
{
static char	function[] = "d20d053_get_psap_auth_mech";
char      	*p_msg;
Bool		psap_found; 
signed32	request_len;
char      	*request_ptr;

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

/* build read invoke message */

/* calculate read message length.	*/
request_len = sizeof (D23_inread) + sizeof (D23_com_arg) + sizeof (D23_ds_name) 
	      + D2_ALIGN(strlen((char *)dsa_name) + 1) + sizeof (D23_rq_info) 
	      + sizeof (D23_at_type)
	      + D2_ALIGN(sizeof (D2_PSAP_ADDRESS) - 1)
	      + sizeof (D23_at_type)
	      + D2_ALIGN(sizeof (D2_AUTH_MECH) - 1)
	      + sizeof (D23_at_type)
	      + D2_ALIGN(sizeof (D2_PR_NM) - 1)
	      + sizeof (D23_eom);

/* allocate space for the request.	*/
if ((p_msg = request_ptr = (char *)D27_MALLOC (svc_c_debug1, request_len)) 
	== NULL)
	{
	d27_002_set_error_code (pbhead, D2_TEMP_ERR, D2_SYS_ERR, D2_NOT_SPEC);

  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
	return (D2_FATAL_ERR);
	}
 
((D23_header *) p_msg)->d23_versno = D23_APDUV22;
((D23_header *) p_msg)->d23_dirid  = dir_id;

p_msg = p_msg + sizeof (D23_inread);

/* put common arguments	*/
((D23_com_arg *) p_msg)->d23_cmtag = D23_CMTAG;
((D23_com_arg *) p_msg)->d23_cmali = 0;

((D23_com_arg *) p_msg)->d23_cmctr.d23_scopt = D2_LOCALSCOPE | D2_NOREFERRAL | 
				D2_USEDSA | D2_DREFALIAS | D2_DONT_STORE;

((D23_com_arg *) p_msg)->d23_cmctr.d23_scpri = D2_PRIO_LOW;
((D23_com_arg *) p_msg)->d23_cmctr.d23_sctlm = D2_T_UNLIMITED;
((D23_com_arg *) p_msg)->d23_cmctr.d23_scslm = 1;
((D23_com_arg *) p_msg)->d23_cmctr.d23_scref = D2_WORLD_SCOPE;

((D23_com_arg *) p_msg)->d23_cmopr.d23_opnm_rs = D2_NOT_STARTED;
((D23_com_arg *) p_msg)->d23_cmopr.d23_op_nxtrdn = 0;

p_msg = p_msg + sizeof (D23_com_arg);

/* put dn to message */
p_msg = d27_201_fill_msg_dn (dsa_name, (D23_ds_name *) p_msg);

/* put required info.	*/
((D23_rq_info *) p_msg)->d23_rqtag = D23_REQTAG;
((D23_rq_info *) p_msg)->d23_rqreq = D2_R_VAL_TYP;
((D23_rq_info *) p_msg)->d23_rqnbr = 3;

p_msg = p_msg + sizeof (D23_rq_info);

((D23_at_type *) p_msg)->d23_atttag = D23_ATTTAG;
((D23_at_type *) p_msg)->d23_atlen = sizeof (D2_PSAP_ADDRESS) - 1;
((D23_at_type *)p_msg)->d23_atoff = D2_ALIGN(((D23_at_type *)p_msg)->d23_atlen);

memcpy(p_msg + sizeof (D23_at_type), D2_PSAP_ADDRESS, 
						sizeof (D2_PSAP_ADDRESS) - 1);

p_msg = p_msg + sizeof (D23_at_type) + ((D23_at_type *)p_msg)->d23_atoff;


((D23_at_type *) p_msg)->d23_atttag = D23_ATTTAG;
((D23_at_type *) p_msg)->d23_atlen = sizeof (D2_AUTH_MECH) - 1;
((D23_at_type *)p_msg)->d23_atoff = D2_ALIGN(((D23_at_type *)p_msg)->d23_atlen);

memcpy(p_msg + sizeof (D23_at_type), D2_AUTH_MECH, sizeof (D2_AUTH_MECH) - 1);

p_msg = p_msg + sizeof (D23_at_type) + ((D23_at_type *)p_msg)->d23_atoff;

((D23_at_type *) p_msg)->d23_atttag = D23_ATTTAG;
((D23_at_type *) p_msg)->d23_atlen = sizeof (D2_PR_NM) - 1;
((D23_at_type *)p_msg)->d23_atoff = D2_ALIGN(((D23_at_type *)p_msg)->d23_atlen);

memcpy(p_msg + sizeof (D23_at_type), D2_PR_NM, sizeof (D2_PR_NM) - 1);

p_msg = p_msg + sizeof (D23_at_type) + ((D23_at_type *)p_msg)->d23_atoff;

/* put end of message	*/
((D23_eom *) p_msg)->d23_eomtag = D23_EOMTAG;

p_msg = p_msg + sizeof (D23_eom);

/* send message */
ipc_invoke_pb.d23_6assid = ipc_bind_pb.d23_4assid;
cont_id = ipc_invoke_pb.d23_6invinfo.d23_contid = D23_SAC(D27_DAP_AC) |
						D23_SACM(D27_LOC_AC) |
						D23_STS(D27_PRIV_TS) |
						D23_SAS(D27_DSP_AS);
ipc_invoke_pb.d23_6oprid = D23_DREAD;

/* send invoke and wait for result */
if (d20d051_invexe (TRUE, pbhead, request_ptr, p_msg - request_ptr)
								!= D2_NOERROR)
	{
	free(request_ptr);
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
	return (D2_FATAL_ERR);
	}

/* extract PSAP address and the auth. mech attributes from the result */
/* result of local DSA */
p_msg = (char *)ipc_waitrs_pb.d23_7rfrdt;

/* skip entry info.	*/
p_msg = p_msg + sizeof (D23_en_info);

/* skip entry name info */
p_msg = p_msg + sizeof (D23_ds_name) + ((D23_ds_name *)p_msg)->d23_dsoff;

/* look for PSAP , PR_NAME and AUTH_MECH in the result.	*/
psap_found = FALSE;
*auth_mech_known = FALSE;
pr_name_av_value = NULL;

while (((D23_at_type *)p_msg)->d23_atttag == D23_ATTTAG)
	{
	/* PSAP address found */
	if ((((D23_at_type *)p_msg)->d23_atlen == sizeof (D2_PSAP_ADDRESS) - 1) 
		&& memcmp(p_msg + sizeof (D23_at_type), D2_PSAP_ADDRESS, 
					sizeof (D2_PSAP_ADDRESS) - 1) == 0)
		{
		*psap_av_value = p_msg + sizeof (D23_at_type) + 
		((D23_at_type *)p_msg)->d23_atoff + sizeof (D23_av_info);

		/* NOTE: PSAP is a single valued attribute.	*/
		p_msg = *psap_av_value + sizeof (D23_av_value) + 
				((D23_av_value *)*psap_av_value)->d23_avoff;
		
		psap_found = TRUE;
		continue;
		}

	/* PR_NAME found */
	if ((((D23_at_type *)p_msg)->d23_atlen == sizeof (D2_PR_NAME) - 1) 
		&& memcmp(p_msg + sizeof (D23_at_type), D2_PR_NAME, 
					sizeof (D2_PR_NAME) - 1) == 0)
		{
		*pr_name_av_value = p_msg + sizeof (D23_at_type) + 
		((D23_at_type *)p_msg)->d23_atoff + sizeof (D23_av_info);

		/* NOTE: PR_NAME is a single valued attribute.	*/
		p_msg = *pr_name_av_value + sizeof (D23_av_value) + 
				((D23_av_value *)*pr_name_av_value)->d23_avoff;
		continue;
		}

	/* AUTH_MECH attribute found.	*/
	if ((((D23_at_type *)p_msg)->d23_atlen == sizeof (D2_AUTH_MECH) - 1) 
		&& memcmp(p_msg + sizeof (D23_at_type), D2_AUTH_MECH, 
					sizeof (D2_AUTH_MECH) - 1) == 0)
		{
		p_msg = p_msg + sizeof (D23_at_type) + 
		((D23_at_type *)p_msg)->d23_atoff + sizeof (D23_av_info);

		while ((unsigned16)(((D23_av_value *)p_msg)->d23_avlen) <
								D23_MINTAG)
			{
			auth_mech[*(signed32 *)(p_msg + sizeof (D23_av_value))] 
			  = TRUE;
			p_msg = p_msg + sizeof (D23_av_value) + 
					((D23_av_value *)p_msg)->d23_avoff;
			*auth_mech_known = TRUE;
			}

		continue;
		}
	}

if (psap_found == FALSE)
	{
	d27_002_set_error_code (pbhead, D2_PERM_ERR, D2_INT_ERR, D2_NOT_SPEC);

	free(request_ptr);
        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
	return (D2_FATAL_ERR);
	}

free(request_ptr);
DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NOERROR));
return(D2_NOERROR);
}

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME:      	signed32   d20d054_gen_bind_message ()		      */
/*								      */
/* AUTHOR:      Sanjay jain, D AP 11                                  */
/* DATE:        01.12.93                                              */
/*								      */
/* DESCRIPTION: This function generates the bind message for the      */
/*	target DSA. It analyses the authentication methods supported  */
/* 	by the target DSA.					      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed16		dir_id;			      */
/*		char			*psap_av_value;		      */
/*		D2_name_string		target_dsa_name;	      */
/*		Bool			auth_mech_known;	      */
/*		signed16		*desired_auth_mech;	      */
/*                                                                    */
/* OUTPUT-PARAMETERS: none                                            */
/*		D2_pbhead		*pbhead;		      */
/*		signed16		*desired_auth_mech;	      */
/*		signed32		*sec_ctx_id;		      */
/*		char			**request_space;	      */
/*		signed32		*request_len;		      */
/*								      */
/* RETURNVALUE:							      */
/*		D2_FATAL_ERR					      */
/*		D2_NOERROR					      */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

static	signed32 d20d054_gen_bind_message(
  D2_pbhead * pbhead,
  signed16 dir_id,
  char * psap_av_value,
  char * pr_name_av_value,
  D2_name_string target_dsa_name,
  Bool auth_mech_known,
  signed16 * desired_auth_mech,
  unsigned32 * sec_ctx_id,
  signed16 * dsa_init_status,
  char ** request_space,
  signed32 * request_len)
{
static char 	function[] = "d20d054_gen_bind_message";
char		*p_msg;
D2_str		sec_token;
D2_sec_status	status;
char		local_dsa_name[D2_DNL_MAX + 1];
char		filename[D27_LFILE_NAME];
asn1_field      asn1_cod;
char            err_apdu[sizeof(D23_rserror) + sizeof(D23_eom)];	
signed32	max_req_len;
char		*tmp_ptr;
FILE		*fp;
char		*bind_msg_pos;
D2_name_string  nm_ptr;
signed16	cred_kind;

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

/* get local DSA name.	*/

/* generate filename containing DSA name.	*/
sprintf (filename, "%s%s%s%s%d%s", dcelocal_path, D27_SERVER_DIR, D27_DSA_DIR, 
                                  D27_SUBDSA_DIR, dir_id, D27_DSANAME);

if ((fp = D27_FOPEN(svc_c_debug1,filename,"r")) == (FILE *) NULL)
	{
	d27_002_set_error_code (pbhead, D2_PERM_ERR, D2_INT_ERR, D2_NOT_SPEC);
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
	return(D2_FATAL_ERR);
	}

if (D27_FGETS(svc_c_debug1,local_dsa_name, D2_DNL_MAX, fp) == NULL)
	{
	d27_002_set_error_code (pbhead, D2_PERM_ERR, D2_INT_ERR, D2_NOT_SPEC);
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
	return(D2_FATAL_ERR);
	}

/* find out authentication mechanism to be used.	*/
if (auth_mech_known == TRUE)
	{
	if (auth_mech[D2_DCE_AUTH] == TRUE)
		*desired_auth_mech = D2_DCE_AUTH;
	else
		if (auth_mech[D2_STRONG] == TRUE)
			*desired_auth_mech = D2_STRONG;
		else
			if (auth_mech[D2_SIMPLE] == TRUE)
				*desired_auth_mech = D2_SIMPLE;
			else
				*desired_auth_mech = D2_ANONYMOUS;
	}
else
	{
	/* desired_auth_mech is passed by the calling function.	*/
	}

/* acquire credentials.	*/

if (dsa_acquire_cred(*desired_auth_mech,(D2_name_string)local_dsa_name,
							&status) == D2_ERROR)
	{
	d27_002_set_error_code (pbhead, status.d2_retcode, status.d2_errclass,
			     status.d2_errvalue);
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
	return(D2_FATAL_ERR);
	}

/* detrmine name which has to be given to dsa_init_sec_ctx */
if (*desired_auth_mech == D2_DCE_AUTH)
{
  if (pr_name_av_value != NULL)
  {
    nm_ptr = (D2_name_string) (pr_name_av_value + D23_S_ATV +
	     ((D23_av_value *)pr_name_av_value)->d23_avoff);
  }
  else
  {
    nm_ptr = (D2_name_string) NULL;
  }
}
else
{
  nm_ptr = target_dsa_name;
}

if ((*dsa_init_status = dsa_init_sec_ctx(*desired_auth_mech, nm_ptr, NULL,
		     sec_ctx_id, &sec_token, &status)) == D2_ERROR)
	{
	d27_002_set_error_code (pbhead, status.d2_retcode, status.d2_errclass,
			     status.d2_errvalue);
  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
	return(D2_FATAL_ERR);
	}

/* calculate the maximum space required for the bind apdu.	*/
max_req_len = sizeof (D23_av_value)
	      + ((D23_av_value *)psap_av_value)->d23_avoff + sizeof (D23_inbind)
	      + sizeof (D23_simcr_info) + sizeof (D23_strcr_info)
	      + sizeof (D23_extcr_info) + D2_ALIGN(sec_token.d2_size);

/* allocate memory for the bind request apdu.	*/
if ((*request_space = p_msg = (char *)D27_MALLOC(svc_c_debug1, max_req_len)) 
	== NULL)
	{

	/* set errro code.	*/

  	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
	return(D2_FATAL_ERR);
	}

/* generate bind apdu for DSP bind.	*/

/* put PSAP address.	*/
memcpy(p_msg, psap_av_value, sizeof (D23_av_value) + 
		((D23_av_value *)psap_av_value)->d23_avoff);

p_msg = p_msg + sizeof (D23_av_value) + 
		((D23_av_value *)psap_av_value)->d23_avoff;

/* save bind message position.	*/
bind_msg_pos = p_msg;

((D23_inbind *)p_msg)->d23_Aversno = D23_APDUV22;
((D23_inbind *)p_msg)->d23_Adirid = dir_id;

switch ((int)*desired_auth_mech)
	{
	case D2_ANONYMOUS:
		((D23_inbind *)p_msg)->d23_Acrkind = D23_NO_CRED;
		break;
	case D2_SIMPLE:
		((D23_inbind *)p_msg)->d23_Acrkind = D23_SIMPLE_CRED;
		break;
	case D2_STRONG:
		((D23_inbind *)p_msg)->d23_Acrkind = D23_STRONG_CRED;
		break;
	case D2_DCE_AUTH:
		((D23_inbind *)p_msg)->d23_Acrkind = D23_EXTERNAL_CRED;
		break;
	default:
		break;
	}

cred_kind = ((D23_inbind *)p_msg)->d23_Acrkind;
p_msg = p_msg + sizeof (D23_inbind);

switch ((int) cred_kind)
	{
	case D23_EXTERNAL_CRED:
		((D23_extcr_info *)p_msg)->d23_exttag = D23_EXTTAG;
		((D23_extcr_info *)p_msg)->d23_extkind = D23_DCE_AUTH;
		((D23_extcr_info *)p_msg)->d23_extlen = sec_token.d2_size;
		((D23_extcr_info *)p_msg)->d23_extoff = 
						D2_ALIGN(sec_token.d2_size);
		p_msg = p_msg + sizeof (D23_extcr_info);
		break;
	case D23_STRONG_CRED:
		/* security token is asn.1 encoded bind message.	*/
		memcpy(bind_msg_pos, sec_token.d2_value, sec_token.d2_size);
		p_msg = bind_msg_pos + sec_token.d2_size;
		break;
	case D23_SIMPLE_CRED:
		((D23_simcr_info *)p_msg)->d23_scrtag = D23_SIMTAG;
		p_msg = p_msg + sizeof (D23_simcr_info);
		break;
	case D23_NO_CRED:
		break;
	}

if (*desired_auth_mech != D2_STRONG)
	{
	/* generate complete bind message and asn.1 encode.	*/

	/* put token to message */

	/* case of anonymous bind.	*/
	if (sec_token.d2_value != NULL)
		{
		memcpy (p_msg, sec_token.d2_value, sec_token.d2_size);
		p_msg = p_msg + D2_ALIGN(sec_token.d2_size);
		}
 
	/* put EOM 	*/
	((D23_eom *) p_msg)->d23_eomtag = D23_EOMTAG;		

	p_msg = p_msg + sizeof (D23_eom);

        DCE_SVC_LOG((GDS_S_APDU_REQUEST_MSG,D23_BIND,p_msg - bind_msg_pos,
		bind_msg_pos));
	/* bind message is always asn1 encoded.	*/
	if (d2a000_apdu_asn1(D23_BIND, D27_DSP_AS, (byte *)bind_msg_pos, 
	     p_msg - bind_msg_pos, &asn1_cod, (byte *)err_apdu) != D2_NOERROR)
		{
		d27_002_set_error_code (pbhead, D2_PERM_ERR, D2_REM_ERR, 
								D2_NOT_SPEC);
		free (*request_space);
  	        DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
		return(D2_FATAL_ERR);
		}

	/* asn1 bind message fits in the allocated space.	*/
	if (max_req_len >= (asn1_cod.l + (bind_msg_pos - *request_space)))
		{
		memcpy(bind_msg_pos,  asn1_cod.v, asn1_cod.l);
		*request_len = asn1_cod.l + (bind_msg_pos - *request_space);
		}
	else
		{
		tmp_ptr = *request_space;

		/* allocate new space for complete message.	*/
		if ((*request_space = (char *)D27_MALLOC(svc_c_debug1,
			asn1_cod.l + (bind_msg_pos - tmp_ptr))) == NULL) 
			{
			/*
			d27_009_trace_dn (
		"d054: asn1 message generation of bind message fails","");
			*/

			d27_002_set_error_code (pbhead, D2_TEMP_ERR, D2_SYS_ERR,
			     D2_NOT_SPEC);

			free (tmp_ptr);
  	    	DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_FATAL_ERR));
			return(D2_FATAL_ERR);
			}

		/* copy PSAP info in the new space.	*/
		memcpy (*request_space, tmp_ptr, bind_msg_pos - tmp_ptr);
	
		/* new bind message position.	*/
		bind_msg_pos = *request_space + (bind_msg_pos - tmp_ptr);

		/* copy the actual bind message.	*/
		memcpy (bind_msg_pos, asn1_cod.v, asn1_cod.l);

		*request_len = asn1_cod.l + (bind_msg_pos - *request_space);

		/* free old space.	*/
		free(tmp_ptr);
		}
	
	free(asn1_cod.v);
	}
else
	*request_len = p_msg - *request_space;

DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,D2_NOERROR));
return(D2_NOERROR);
}
