/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26mutil.c,v $
 * Revision 1.1.12.2  1996/02/18  19:45:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:58  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:46:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:24  root]
 * 
 * Revision 1.1.10.7  1994/09/30  12:09:33  keutel
 * 	OT 12360
 * 	[1994/09/30  09:59:01  keutel]
 * 
 * Revision 1.1.10.6  1994/08/24  14:51:53  marrek
 * 	Add include of svcremote.h
 * 	[1994/08/24  14:51:40  marrek]
 * 
 * Revision 1.1.10.5  1994/08/24  12:14:54  keutel
 * 	OT 11802
 * 	[1994/08/23  16:01:39  keutel]
 * 
 * Revision 1.1.10.4  1994/06/21  14:45:39  marrek
 * 	June 1994 code submission.
 * 	[1994/06/20  13:04:53  marrek]
 * 
 * Revision 1.1.10.3  1994/05/10  15:52:28  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:22:49  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:09:31  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:21:32  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  16:06:15  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:15:36  marrek]
 * 
 * Revision 1.1.8.1  1993/10/29  15:11:09  keutel
 * 	CR 9261
 * 	[1993/10/29  14:06:11  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  14:23:45  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:13:33  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:41:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:20:17  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:47:30  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:13:52  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:50:48  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:56:58  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26mutil.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 19:45:39 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : d26mutil.c         [mutil]                          */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 27.12.88                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the utilities used by         */
/*                  the DSA main function.                            */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 27.12.88| birth                          | ek |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

/*****  internal Includes    *****/

#include <d26dsa.h>
#include <d26schema.h>

/*****  Operating-System-Includes  *****/

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

/*****  external Includes    *****/

#include <dce/dcesvcmsg.h>
#include <dce/dce_svc.h>
#include <dce/svcremote.h>
#include <dce/d27util.h>
#include <d23ipc.h>
#define DSA
#include <d2asn1.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_m02_free                                          */
/*                                                                    */
/* AUTHOR:      Kraemer D AP 11                                       */
/* DATE:        21.01.88                                              */
/*                                                                    */
/* DESCRIPTION: The function frees all memory allocated by main before*/
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_m02_free(void)

{                                     /*  d26_m02_free                */

if                      /*  memory for SRT allocated                  */
  (d26_srt != NULL)
{                       /*  free srt memory                           */
  free((void *)d26_srt);
}                       /*                                            */
if                      /*  memory for naming attribute structures    */
			/*  allocated                                 */
  (d26_nam_at != NULL)
{                       /*  free naming attribute structures memory   */
  free((void *)d26_nam_at);
}                       /*                                            */
if                      /*  memory for naming positions allocated     */
  (d26_name_pos != NULL)
{                       /*  free naming positions memory              */
  free((void *)d26_name_pos);
}                       /*                                            */
if                      /*  memory for object class arcs allocated    */
  (d26_ocl_arc != NULL)
{                       /*  free object class arcs memory             */
  free((void *)d26_ocl_arc);
}                       /*                                            */

if                      /*  memory for OCT allocated                  */
  (d26_oct != NULL)
{                       /*  free oct memory                           */
  free((void *)d26_oct);
}                       /*                                            */
if                      /*  memory for OCT indices allocated          */
  (d26_oct_idx != NULL)
{                       /*  free OCT indices memory                   */
  free((void *)d26_oct_idx);
}                       /*                                            */

if                      /*  memory for AT allocated                   */
  (d26_at != NULL)
{                       /*  free AT memory                            */
  free((void *)d26_at);
}                       /*                                            */
if                      /*  memory for field positions allocated      */
  (d26_at_pos != NULL)
{                       /*  free field positions memory               */
  free((void *)d26_at_pos);
}                       /*                                            */
if                      /*  memory for object identifiers allocated   */
  (d26_obid_str != NULL)
{                       /*  free object identifiers memory            */
  free((void *)d26_obid_str);
}                       /*                                            */
if                      /*  memory attribute informations allocated   */
  (d26_attr != NULL)
{                       /*  free attribute infomations memory         */
  free((void *)d26_attr);
}                       /*                                            */
if                      /*  memory for attribute usage indicators     */
			/*  allocated                                 */
  (d26_ate_used != NULL)
{                       /*  free attribute usage indicators           */
  free((void *)d26_ate_used);
}                       /*                                            */

d26_mi2_free();

if                      /*  memory for access point list allocated    */
  (d26_accp_list != NULL)
{                       /*  free master knowledge list                */
  free((void *)d26_accp_list);
}                       /*                                            */

}                                   /*  d26_m02_free                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_m06_hdl_sigdistcmd                                */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        23.03.89                                              */
/*                                                                    */
/* DESCRIPTION: This function is called when a signal for distributed */
/*              commands arrives. It resets the signal handling       */
/*              routine for distributed command signal to IGNORE.     */
/*              The distributed command is read and handled later.    */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      sig         int         signal that caused the call           */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_m06_hdl_sigdistcmd(int sig)

{                               /*  d26_m06_hdl_sigdistcmd            */

			/* structure for sigaction call               */
  static struct sigaction siginfo = {0};

  sigemptyset(&siginfo.sa_mask);
  siginfo.sa_handler = SIG_IGN;
  D27_SIGACTION(svc_c_sev_warning,SIGUSR1,&siginfo,(struct sigaction *)NULL);

  return;

}                               /*  d26_m06_hdl_sigdistcmd            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_m08_set_logging                                   */
/*                                                                    */
/* AUTHOR:      E. Kraemer, BU BA NM 12                               */
/* DATE:        22.08.94                                              */
/*                                                                    */
/* DESCRIPTION: The function generates the command line parameters    */
/*              for a program to call by execv.                       */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_m08_set_logging(
    int  *argc,         /* INOUT - count of parameters                */
    char *argv[])       /* INOUT - array of call parameters           */

{                                   /* d26_m08_set_logging            */

  dce_svc_stringarray_t svc_table;  /* contains actual setting of     */
				    /* logging                        */
  int     i;                        /* loop variable                  */
  error_status_t st;                /* error status for serviceability*/
				    /* parameter for exception logging*/
  static char elog_par[5][30 + D27_LFILE_NAME];
				    /* parameter for debug logging    */
  static char dlog_par[5][30 + D27_LFILE_NAME];

  dce_svc_routings(&svc_table,FALSE,&st);
  if                    /* error occurred                             */
    (st != svc_s_ok)
  {                     /* issue error message                        */
    dce_svc_printf(GDS_S_ROUTINGS_ERR_MSG,st);
  }
  else
  {                     /* set routing parameters                     */
    for                 /* all routing specification strings          */
       (i = 0; i < svc_table.tab_size && i < 5; i++)
    {                   /* generate serviceability parameter          */
      sprintf(elog_par[i],"-w%s",svc_table.table[i]);
      argv[(*argc)++] = elog_par[i];
    }
  }
  if                    /* memory was allocated                       */
    (svc_table.table != NULL)
  {                     /* free it                                    */
    rpc_ss_free(svc_table.table);
  }

  dce_svc_routings(&svc_table,TRUE,&st);
  if                    /* error occurred                             */
    (st != svc_s_ok)
  {                     /* issue error message                        */
    dce_svc_printf(GDS_S_ROUTINGS_ERR_MSG,st);
  }
  else
  {                     /* set debug routing parameters               */
    for                 /* all debug routing specification strings    */
       (i = 0; i < svc_table.tab_size && i < 5; i++)
    {                   /* generate serviceability parameter          */
      sprintf(dlog_par[i],"-v%s",svc_table.table[i]);
      argv[(*argc)++] = dlog_par[i];
    }
  }
  if                    /* memory was allocated                       */
    (svc_table.table != NULL)
  {                     /* free it                                    */
    rpc_ss_free(svc_table.table);
  }

}                               /*  d26_m08_set_logging               */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_m09_execv                                         */
/*                                                                    */
/* AUTHOR:      E. Kraemer, D AP 11                                   */
/* DATE:        16.09.91                                              */
/*                                                                    */
/* DESCRIPTION: The function modifies the parameterlist of the DSA    */
/*              and forks and execvs then.                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_m09_execv(
    int  argc,          /* IN   -  count of parameters                */
    char *argv[])       /* IN   -  array of call parameters           */

{                                 /*  d26_m09_execv                   */

  int     fork_result;              /* result from fork-call          */
  int     stat_loc;                 /* location for status of child   */

  DCE_SVC_LOG((GDS_S_DSAFORKS_MSG));

  if                    /* fork returns in father process             */
    ((fork_result = D27_FORK(svc_c_sev_warning)) > 0)
  {                     /* wait for termination and return            */
    D27_WAIT(svc_c_sev_warning,&stat_loc);
  }
  else if               /* fork returns in child process              */
	 (fork_result == 0)
  {                     /* prepare parameters                         */
    d26_m08_set_logging(&argc,argv);
    argv[argc++] = D26_ADDON_PAR;
    argv[argc++] = (String)NULL;
    D27_EXECV(svc_c_sev_warning,argv[0],argv);
  }

}                               /*  d26_m09_execv                     */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_m11_distcmd                                       */
/*                                                                    */
/* AUTHOR:      E. Kraemer, BUBA KP1                                  */
/* DATE:        13.07.93                                              */
/*                                                                    */
/* DESCRIPTION: The function reads                                    */
/*              the distributed command, checks which command has to  */
/*              be performed and sets some global variables handled   */
/*              later on in the DSA main module.                      */
/*                                                                    */
/*exoff ***************************************************************/

void d26_m11_distcmd(int sig)

{                               /*  d26_m11_distcmd                   */

             		        /* parameters of distributed command  */
  static D23_dreadpb     rdc_pb = {{D23_V02}};
  static D23_dconfpb     cdc_pb = {{D23_V02}};
  boolean   confirm = TRUE;     /* confirmation flag                  */

  error_status_t st;            /* error status for serviceability    */

  while                 /* distributed can be read                    */
       (d23_read_dist_cmd(&rdc_pb) != D2_ERROR)
  {                     /*  handle the command                        */
    switch(rdc_pb.d23_Kcmd)
    {
      case D23_CLLFILE: /*  D2_SHDLOG file should be closed           */
	if              /*  shadow logfile cannot be closed           */
	  (d26_m14_close_shdlog() != D2_NO_ERR)
	{               /*  reset the confirmation flag               */
	  confirm = FALSE;
	}
	else
	{               /* switch off database modifications          */
			/* delta-update-daemon works with SHDLOG file */
	  d26_dbop_flag = D26_DB_RO;
	}
	break;
      case D23_OPLFILE: /*  D2_SHDLOG file can be opened              */
		   /* modifications are allowed to perform again      */
		   /* delta update daemon finished work               */
	d26_dbop_flag = D26_DB_RW;
	break;
      case D23_ENADBOP: /* database operations should be enabled      */
	d26_dbop_flag = D26_DB_RW;
	break;
      case D23_RESTART: /* DSA should be restarted after restoring of */
			/* data                                       */
	d26_restart = TRUE;
	confirm = FALSE;
	break;
      case D23_CLDBFILES:
			/* close directory database files and disable */
			/* database operations                        */
	d26_dbop_flag = D26_DB_NOOP;
	d26_i14_unlock();
	d26_mi4_close_files();
	d26_i06_close_dnlistfile();
	break;
      case D23_DISDBOP: /* database modifications should be disabled  */
	d26_dbop_flag = D26_DB_RO;
	break;
      case D23_DCABANDON:
			/* abandon request                            */
	d26_abandoned = TRUE;
	break;
      case D23_DISLOG:  /* no logging                                 */
      case D23_ENALOG:  /* logging required                           */
			/* retarget messages for logging              */
	d27_003_reset_routing((char *)rdc_pb.d23_Kparref,&st);
	break;
      case D23_SVCLOGLEVEL:
			/* reset debug levels for logging             */
	dce_svc_debug_set_levels(rdc_pb.d23_Kparref,&st);
	break;
      case D23_DISSHLOG:/* disable shadow logging (all shadowing jobs */
			/* are removed)				      */
	d26_m14_close_shdlog();
        d26_ulogging = FALSE;
	break;
      case D23_ENASHLOG:/* enable shadow logging (a shadowing job     */
			/* was created for this DSA)		      */
        d26_ulogging = TRUE;
	break;
      default:          /* unknown command                            */
	DCE_SVC_LOG((GDS_S_ILL_DIST_CMD_MSG,rdc_pb.d23_Kcmd));
	confirm = FALSE;
	break;
    }

    if                  /*  command must be confirmed                 */
      (confirm == TRUE)
    {                   /*  confirm it                                */
      cdc_pb.d23_Lkey = rdc_pb.d23_Kkey;
      d23_conf_dist_cmd(&cdc_pb);
    }
    else
    {                   /*  break the loop for restart                */
      break;
    }
  }

  return;

}                               /*  d26_m11_distcmd                   */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_m01_ret_result                                    */
/*                                                                    */
/* AUTHOR:      Volpers D AP 11                                       */
/* DATE:        20.01.88                                              */
/*                                                                    */
/* DESCRIPTION: The function splits the result or error message into  */
/*              blocks and sends it via IPC to the client.            */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR     signed16    no error occurred                   */
/*      D26_ERROR                 error occurred                      */
/*                                                                    */
/*inoff ***************************************************************/

signed16 d26_m01_ret_result(
    signed16 op_id,     /* IN    -  operation ID                      */
    signed32 err_id,    /* IN    -  error ID                          */
    byte *result,       /* IN    -  result or error message to send   */
    signed32 result_len,/* IN    -  length of result or error message */
    D23_rsrqpb *res_par,/* IN    -  parameters for result             */
    signed16 res_type)  /* IN    -  indicates result or error type of */
			/*          message to be sent                */

{                                     /*  d26_m01_ret_result          */

  static char *function = "d26_m01_ret_result";
			/* parameter block for disable abandon        */
  static D23_disabandonpb disabandonpb = {{D23_V02}};
  static D23_remcrdpb     ruc_pb = {{D23_V02}};

  asn1_field asn1_result;           /* encoded result                 */
  unsigned16 req_syntax;            /* transfer syntax of request     */
				    /* memory for error apdu          */
  byte err_apdu[sizeof(D23_rserror) + sizeof(D23_eom)];
				    /* return value from coding       */
  signed32 coding_err;              /* function                       */
  signed16 ret_value = D2_NO_ERR;   /* return value                   */
  byte *free_ptr = NULL;            /* pointer to be freed            */

  signed16 block_count = 0;         /* count of result blocks         */

  req_syntax = D23_GTS(res_par->d23_3invinfo.d23_contid);

  if                    /*  request is not abandoned                  */
    (d26_abandoned == FALSE)
  {                     /*  check and disable abandon                 */
    disabandonpb.d23_Hassid = res_par->d23_3assid;
    if                  /* request cannot be disabandoned             */
      (d23_disable_abandon(&disabandonpb) == D23_ERROR)
    {                   /* check the reason                           */
      switch(disabandonpb.d23_Hhdinfo.d2_errvalue)
      {
	case D23_TOOLATE:
	  d26_abandoned = TRUE;
	  d27_010_set_error_apdu((D23_rserror *)result,D2_TEMP_ERR,
	    D2_REM_ERR,D2_ABANDONED,&result_len);
	  err_id   = 0;
	  res_type = D23_RERROR;
	  break;
	default:
	  d27_010_set_error_apdu((D23_rserror *)result,D2_TEMP_ERR,
	    D2_REM_ERR,D2_UNWILLING,&result_len);
	  err_id    = 0;
	  res_type  = D23_RERROR;
	  ret_value = D26_ERROR;
	  break;
      }
    }
  }
  if                    /*  result must be returned                   */
    (res_type == D23_RESULT)
  {                     /*  check transfer syntax                     */
    DCE_SVC_LOG((GDS_S_APDU_RESULT_MSG,op_id,result_len,result));
    if                  /*  result must be encoded                    */
      (req_syntax == D27_ASN1_TS || req_syntax == D27_ASN1_ADM_TS)
    {                   /*  encode result                             */
      if                /*  error from encoding result                */
	(d2a000_apdu_asn1(op_id + D2_RES_OPID,
	 (signed16)D23_GAS(res_par->d23_3invinfo.d23_contid),
	 result,result_len,&asn1_result,err_apdu) == D2_ERROR)
      {                 /*  write error message                       */
	D27_022_ENTER_TIME_TRACE(GDS_S_ENCODING_ERR,NULL,0,0);
	res_type = D23_RERROR;
	result_len = sizeof(D23_rserror) + sizeof(D23_eom);
	result   = err_apdu;
      }
      else
      {                 /*  reset length and message pointer          */
	D27_022_ENTER_TIME_TRACE(GDS_S_ENCODING_OK,NULL,0,0);
	result_len = asn1_result.l;
	if              /*  result pointer is changed                 */
	  (result != asn1_result.v)
	{               /*  reset the result pointer                  */
	  result   = free_ptr = asn1_result.v;
	}
      }
    }
  }
  if                    /*  error must be returned                    */
    (res_type == D23_RERROR)
  {                     /*  check transfer syntax                     */
    DCE_SVC_LOG((GDS_S_APDU_ERROR_MSG,result_len,result));
    if                  /*  error must be encoded                     */
      ((req_syntax == D27_ASN1_TS || req_syntax == D27_ASN1_ADM_TS) &&
       op_id != D23_UNBIND)
    {                   /*  log APDU message                          */
      if                /*  bind error must be encoded                */
	(op_id == D23_BIND)
      {                 /*  encode bind error                         */
	coding_err = d2a026_apdu_bind_error_asn(result,result_len,
						&asn1_result);
      }
      else
      {                 /*  encode other error                        */
	coding_err = d2a027_apdu_error_asn(result,result_len,&err_id,
	  &asn1_result,(signed16)D23_GAS(res_par->d23_3invinfo.d23_contid));
      }
      if                /*  error from encoding                       */
	(coding_err == D2_ERROR)
      {                 /*  reject the request                        */
	D27_022_ENTER_TIME_TRACE(GDS_S_ENCODING_ERR,NULL,0,0);
	res_type   = D23_RREJECT;
	result_len = 0;
	err_id     = D23R_RESOURCE_LIMIT;
      }
      else
      {                 /*  reset length and message pointer          */
	D27_022_ENTER_TIME_TRACE(GDS_S_ENCODING_OK,NULL,0,0);
	result_len = asn1_result.l;
	result     = free_ptr = asn1_result.v;
      }
    }
  }

  if                    /*  request must be rejected                  */
    (res_type == D23_RREJECT)
  {                     /*  set result type                           */
    res_par->d23_3rltid = D23_RREJECT + (signed16)err_id;
  }
  else if               /*  result or abort must be returned          */
	 (res_type == D23_RESULT || res_type == D23_ABORT)
  {                     /*  set result type                           */
    res_par->d23_3rltid = res_type;
  }
  else if               /*  bind error must be returned               */
	 (op_id == D23_BIND)
  {                     /*  set result type 'reject'                  */
    res_par->d23_3rltid = D23_RREJECT;
  }
  else
  {                     /*  set result type error                     */
    res_par->d23_3rltid = D23_RERROR + (signed16)err_id;
  }

  do
  {                     /*  copy data to IPC-block                    */
    if                  /*  more data than one block present          */
      (result_len > res_par->d23_3lnrdt)
    {                   /*  set length and MORE-DATA-indicator        */
      res_par->d23_3rmdat = D23_MRDATA;
    }
    else
    {                   /*  set length and LAST FRAGMENT indicator    */
      res_par->d23_3lnrdt = result_len;
      res_par->d23_3rmdat = D23_LASTFG;
    }
    memcpy(res_par->d23_3rfrdt,result,res_par->d23_3lnrdt);
    block_count++;
    switch(d23_result(res_par))
    {
      case D23_ERROR:   /*  log and check return value                */
	if              /*  client crashed                            */
	  (res_par->d23_3hdinfo.d2_errvalue == D23_ABOERR)
	{               /*  remove requestor from shared memory       */
	  ruc_pb.d23_Ncontid = res_par->d23_3invinfo.d23_contid;
	  ruc_pb.d23_Nusrid = res_par->d23_3invinfo.d23_usrid;
	  d23_rem_usr_cred(&ruc_pb);
	  result_len = 0;
	}
	else
	{               /*   reset return value                       */
	  ret_value = D26_ERROR;
	}
	result_len = 0;
	break;
      default:
	result     += res_par->d23_3lnrdt;
	result_len -= res_par->d23_3lnrdt;
	break;
    }
  }
  while                 /*  still data to send                        */
       (result_len > 0);

  if                    /*  memory for ASN1-message was allocated     */
    (free_ptr != NULL)
  {                     /*  free that memory                          */
    free(free_ptr);
  }

  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_SENT_BLOCKS,function,block_count,0);

			/*  return                                    */
  return(ret_value);

}                                   /*  d26_m01_ret_result            */

