/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26dsa.c,v $
 * Revision 1.1.12.2  1996/02/18  19:45:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:13  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:43:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:34  root]
 * 
 * Revision 1.1.10.9  1994/10/07  16:47:50  keutel
 * 	OT 12548: problems with semicolon in SVC string
 * 	[1994/10/07  16:10:08  keutel]
 * 
 * Revision 1.1.10.8  1994/10/05  15:38:58  keutel
 * 	OT 12481: multiple routing
 * 	[1994/10/05  14:35:40  keutel]
 * 
 * Revision 1.1.10.7  1994/08/24  14:51:51  marrek
 * 	Add include of svcremote.h
 * 	[1994/08/24  14:51:26  marrek]
 * 
 * Revision 1.1.10.6  1994/08/24  12:14:51  keutel
 * 	OT 11802
 * 	[1994/08/23  16:01:31  keutel]
 * 
 * Revision 1.1.10.5  1994/07/06  15:06:53  marrek
 * 	July 1994 code drop.
 * 	[1994/07/05  09:10:46  marrek]
 * 
 * Revision 1.1.10.4  1994/06/21  14:45:17  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  12:59:19  marrek]
 * 
 * Revision 1.1.10.3  1994/05/10  15:52:03  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/04  14:24:31  marrek]
 * 
 * 	Bug fixes in April 1994 submission.
 * 	[1994/05/03  16:40:08  marrek]
 * 
 * 	Bug fixes.
 * 	[1994/04/29  09:29:03  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:20:37  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:09:14  keutel
 * 	March 1994 code drop
 * 	[1994/03/22  14:13:52  keutel]
 * 
 * 	March 1994 code drop
 * 	[1994/03/21  16:20:25  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  16:05:29  marrek
 * 	Update.
 * 	[1994/02/09  16:04:45  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:10:49  marrek]
 * 
 * Revision 1.1.8.2  1993/10/29  16:30:16  keutel
 * 	CR 9261
 * 	[1993/10/29  16:30:00  keutel]
 * 
 * Revision 1.1.8.1  1993/10/29  15:11:06  keutel
 * 	CR 9261
 * 	[1993/10/29  14:33:39  keutel]
 * 
 * 	CR 9261
 * 	[1993/10/29  14:04:05  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  13:51:57  marrek
 * 	Activation of SIGUSR1 before waitinvoke.
 * 	[1993/08/10  06:23:31  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/30  13:05:28  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:39:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:18:59  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/11  16:04:56  marrek
 * 	Fix shadow update problem by replacing
 * 	request by dap_request in d26_m12_logg_message()
 * 	[1992/12/10  14:45:10  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  19:39:44  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:07:19  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:52:23  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:21:03  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:47:18  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:54:16  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26dsa.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 19:45:04 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : d26dsa.c    [dsa]                                   */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 17.12.87                                            */
/*                                                                    */
/* COMPONENT    : D2                                                  */
/*                                                                    */
/* DOC.-NR.     : D2-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*                                                                    */
/* DESCRIPTION  : The module contains the main function of DSA.       */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*      1.0 | 17.12.87| Birth                          | ek |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>
#include <dce/dcesvcmsg.h>
#include <dce/svcremote.h>
#include <gds_sec.h>

/*****  Operating-System-Includes  *****/

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>

/*****  external Includes    *****/

#include <dce/d27util.h>
#define DSA
#include <d2asn1.h>

/*****  internal Includes    *****/

#include <d26schema.h>
#include <d26isam.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D26_DSA_PROC  "/gdsdsa"     /* base name of dsa process       */

#define D26_UMASK     0000          /* bit mask for files to create   */
#define D26_DIR       "/dir"
#define D26_NOATTACH    -2          /* indicates that no attach was   */
				    /* performed                      */
#define D26_CLATTACH    -1          /* indicates that only attach     */
				    /* client was performed           */
#define D26_NO_ARG      20          /* max. number of arguments       */

#define D26_SIMP_PLUS_DCE	25  /* support both sec. mechs	      */
#define D26_SIMP_PLUS_STRONG	26  /* support both sec. mechs	      */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

			/* indicates whether DSA has to be shut down  */
			/* on signal arriving                         */
static signed16   d26_shutdown = D26_NOATTACH;
			/* indicates wether DSA is add on DSA         */
static boolean    d26_add_on = FALSE;
			/* dynamical process unload factor            */
static int        d26_unloadf;

			/* parameters for get server info             */
static D23_gtsvpb d26_gtsvpb = {{D23_V02,0,0,0}};

			/* parameters for detach server               */
static D23_desvpb d26_desvpb = {{D23_V02,0,0,0}};

			/* parameters for detach client               */
static D23_declpb d26_declpb = {{D23_V02,0,0,0}};

			/* memory for one IPC-message block to save   */
			/* one invoke message block for search        */
static byte       *d26_save_inv_data = NULL;

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** IMPORT ********/

extern int      _check_key(char *product, int version);

/******** LOCAL  ********/

static byte *d26_m03_collect_blocks(D23_wtinpb *wtinpb, D23_rsrqpb *rsrqpb,
    signed32 *request_len);
static signed16 d26_m04_shutdown(void);
static void     d26_m05_hdl_term_signal(int sig);
static signed16 d26_m07_work(D23_atsvpb *atsvpb, int argc, char *argv[]);
static signed16 d26_m18_restart(boolean sm_broken, unsigned16 cont_id,
    signed32 usr_id);
static void     d26_m19_set_shdlogging(int argc, char *argv[]);

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_m03_collect_blocks()                              */
/*                                                                    */
/* AUTHOR:      Kraemer D AP 11                                       */
/* DATE:        21.01.88                                              */
/*                                                                    */
/* DESCRIPTION: The function collects all blocks for a request from   */
/*              IPC                                                   */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      != D2_NULL  String          pointer to collected input data   */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d26_m03_collect_blocks(
    D23_wtinpb *wtinpb, /* IN    -  parameters for wait invoke        */
    D23_rsrqpb *rsrqpb, /* IN    -  parameters for result request     */
			/* OUT   -  total length of request message   */
    signed32 *request_len)

{                                   /*  d26_m03_collect_blocks        */

				    /* function name for logs         */
  static char *function    = "d26_m03_collect_blocks";
  static byte *save_blocks = NULL;  /* pointer to concatenated message*/
				    /* blocks                         */
  static D23_remcrdpb ruc_pb = {{D23_V02}};

  signed16 op_id;               /* operation ID                       */
  signed32 error_len;           /* length of error message            */
				/* memory for error apdu              */
  byte     err_apdu[sizeof(D23_rserror) + sizeof(D23_eom)];

  signed16 block_count = 1;     /* count of invoke blocks             */

  register boolean ill_message; /* indicates incomplete messages      */

  if                                /*  memory was allocated earlier  */
    (save_blocks != NULL)
  {                                 /*  free memory                   */
    free(save_blocks);
    save_blocks = NULL;
  }

  if                                /*  blocks are following          */
    (wtinpb->d23_2imdat == D23_MRDATA)
  {                                 /*  collect blocks                */
    for                             /*  all incoming message blocks   */
       (*request_len = 0, ill_message = FALSE; ; )
    {                               /*  check whether message still ok*/
      if                            /*  message ok                    */
	(ill_message == FALSE)
      {                             /*  check IPC-event               */
	if                          /*  connection aborted            */
	  (wtinpb->d23_2oprid == D23_ABORT)
	{                           /*  remove user credentials       */
	  ruc_pb.d23_Ncontid = wtinpb->d23_2invinfo.d23_contid;
	  ruc_pb.d23_Nusrid = wtinpb->d23_2invinfo.d23_usrid;
	  d23_rem_usr_cred(&ruc_pb);
	  return(NULL);
	}
	else if                     /*  error from reallocating memory*/
	       ((save_blocks = (byte *)D27_REALLOC(svc_c_sev_warning,
		save_blocks,*request_len + wtinpb->d23_2lnidt)) == NULL)
	{                           /* set illegal message flag       */
	  ill_message = TRUE;
	}
	else
	{                           /*  copy message                  */
	  memcpy(save_blocks + *request_len,wtinpb->d23_2rfidt,
	    wtinpb->d23_2lnidt);
	  *request_len += wtinpb->d23_2lnidt;
	}
      }
      if                            /*  still more blocks following   */
	(wtinpb->d23_2imdat == D23_MRDATA)
      {                             /*  get next block                */
	block_count++;
	if                          /*  error from wait invoke        */
	  (d23_waitin(wtinpb) == D23_ERROR)
	{                           /*  check  error                  */
	  ruc_pb.d23_Ncontid = wtinpb->d23_2invinfo.d23_contid;
	  ruc_pb.d23_Nusrid = wtinpb->d23_2invinfo.d23_usrid;
	  d23_rem_usr_cred(&ruc_pb);
	  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_RECEIVED_BLOCKS,function,
	    block_count,0);
	  return(NULL);
	}
	/* D23_NOOPR cannot occur at this place, as SIGUSR1 is ignored*/
	/* and SIGTERM shuts down the DSA                             */
      }
      else if                       /*  message is illegal            */
	     (ill_message == TRUE)
      {                             /*  send error message            */
	d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,D2_REM_ERR,
				D2_UNWILLING,&error_len);
	op_id = wtinpb->d23_2oprid > D2_ADM_OPID ?
		wtinpb->d23_2oprid - D2_ADM_OPID : wtinpb->d23_2oprid;
	D27_022_ENTER_TIME_TRACE(GDS_S_FCT_RECEIVED_BLOCKS,function,
	  block_count,0);
	d26_m01_ret_result(op_id,0,err_apdu,error_len,rsrqpb,D23_RERROR);
	return(NULL);
      }
      else
      {                             /*  return collected message      */
	D27_022_ENTER_TIME_TRACE(GDS_S_FCT_RECEIVED_BLOCKS,function,
	  block_count,0);
	return(save_blocks);
      }
    }
  }
  else
  {                                 /*  check operation               */
    D27_022_ENTER_TIME_TRACE(GDS_S_FCT_RECEIVED_BLOCKS,function,block_count,
      0);
    *request_len = (signed32)wtinpb->d23_2lnidt;
    op_id = wtinpb->d23_2oprid > D2_ADM_OPID ?
	    wtinpb->d23_2oprid - D2_ADM_OPID : wtinpb->d23_2oprid;
    switch(op_id)
    {
      case D23_SEARCH   :
      case D23_BIND     :
      case D23_UNBIND   :
      case D23_DREAD    :
      case D23_READ     :
      case D23_LIST     :
      case D23_COMPARE  :
	return(wtinpb->d23_2rfidt);
      default           :
	if                  /* logging not required                   */
	  (d26_ulogging == FALSE)
	{
	  return(wtinpb->d23_2rfidt);
	}
	else
	{                   /* save invoke message                    */
	  memcpy(d26_save_inv_data,wtinpb->d23_2rfidt,wtinpb->d23_2lnidt);
	  wtinpb->d23_2rfidt = d26_save_inv_data;
	  return(d26_save_inv_data);
	}
    }
  }

}                                   /*  d26_m03_collect_blocks        */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_m04_shutdown()                                    */
/*                                                                    */
/* AUTHOR:      Kraemer D AP 11                                       */
/* DATE:        26.02.88                                              */
/*                                                                    */
/* DESCRIPTION: The function does all cleanup before leaving main     */
/*                                                                    */
/* RETURN VALUE :       None                                          */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_m04_shutdown(void)

{                                   /*  d26_m04_shutdown              */

  signed16 ret_value = D2_NO_ERR;   /* return value                   */
  signed32 auth_mech;
  D2_sec_status sec_status;

  if                    /*  error from closing C-isam-files           */
    (d26_mi4_close_files() == D26_ERROR)
  {                     /*  set return value                          */
    ret_value = -1;
  }
  else if               /*  error from closing DN list file           */
	 (d26_i06_close_dnlistfile() == D26_ERROR)
  {                     /*  set return value                          */
    ret_value = -1;
  }

/* release security credentials for all supported methods	      */
  for                   /* all authentication mechanisms              */
     (auth_mech = 0; auth_mech < D2_MAX_AUTH_MECHS; auth_mech++)
  {
    if (d26_auth_mech[auth_mech])
    {
      if (dsa_del_cred(auth_mech,&sec_status) == D2_ERROR)
      {
	DCE_SVC_LOG((GDS_S_DEL_CRED_ERR_MSG,auth_mech,sec_status.logstring));
	return (D26_ERROR);
      }
    }
  }

			/*  free allocated memory                     */
  d26_m02_free();

  if                    /*  memory for saved invoked data are allocted*/
    (d26_save_inv_data != NULL)
  {                     /*  free allocated memory                     */
    free(d26_save_inv_data);
    d26_save_inv_data = NULL;
  }

			/*  return ret_value                          */
  return(ret_value);

}                                   /*  d26_m04_shutdown              */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_m05_hdl_term_signal                               */
/*                                                                    */
/* AUTHOR:      Kraemer D AP 11                                       */
/* DATE:        28.03.88                                              */
/*                                                                    */
/* DESCRIPTION: The function handles incoming SIGTERM signals.        */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      sig     Int     received signal                               */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_m05_hdl_term_signal(int sig)

{                                   /*  d26_m05_hdl_term_signal       */

  int  exit_value = D2_NO_ERR;      /* exit value for shell           */
  time_t idle_time;                 /* time that DSA is permitted to  */
				    /* idle                           */

if                      /*  alarm signal caused the break             */
  (sig == SIGALRM)
{                       /*  check count of DSP bind table entries     */
  if                    /*  still idle DSP bind table entries         */
    (d26_bind_count > 1)
  {                     /*  compress bind table                       */
    d26_d34_comp_bind_table();
  }
  if                    /*  still idle DSP bind table entries         */
    (d26_bind_count > 1)
  {                     /*  rewind alarm clock                        */
    if                  /*  DSA is added on                           */
      (d26_add_on == TRUE)
    {                   /*  calculate idle time                       */
      idle_time = d26_dspunbind_delay > D26_IDLE_TIME ? d26_dspunbind_delay :
						      D26_IDLE_TIME;
    }
    else                /*  set idle time                             */
    {
      idle_time = d26_dspunbind_delay;
    }
    alarm(idle_time);
    return;
  }
  if                    /*  DSA is added on                           */
    (d26_add_on == TRUE)
  {                     /*  check unload factor                       */
    d26_gtsvpb.d23_Dunloadf = (signed16)d26_unloadf;
    if                  /*  server information can be read            */
      (d23_getsvrinfo(&d26_gtsvpb) != D23_ERROR)
    {                   /*  check number of idling DSA processes      */
      if                /*  too much idling DSA-processes             */
	((100 * (d26_gtsvpb.d23_Dnototpr - d26_gtsvpb.d23_Dnobsypr)) /
	 d26_gtsvpb.d23_Dnototpr > d26_gtsvpb.d23_Dunloadf)
      {                 /*  set the shutdown flag                     */
	d26_shutdown = TRUE;
      }
      else              /*  wind alarm clock again and return         */
      {
	alarm(D26_IDLE_TIME);
	return;
      }
    }
    else                /*  set the shutdown flag                     */
    {
      d26_shutdown = TRUE;
    }
  }
  else                  /*  return immediately                        */
  {
    return;
  }
}

switch(d26_shutdown)
{
  case  FALSE:
    d26_shutdown = TRUE;
    break;
  case  TRUE:
    if                              /*  error from detach server      */
      (d23_detsvr(&d26_desvpb) == D23_ERROR)
    {   
      exit_value = 1;
    } 
  case D26_CLATTACH:
    if                              /*  error from detach client      */
      (d23_detclt(&d26_declpb) == D23_ERROR)
    {                               /*  set exit value                */
      exit_value = 1;
    }
  case  D26_NOATTACH:
    if                              /*  error from shutdown           */
      (d26_m04_shutdown())
    {   
      exit_value = 1;
    } 
    dce_svc_printf(GDS_S_EXITDSA_MSG,d26_dir_id);
    exit(exit_value);
}   

return;

}                                   /*  d26_m05_hdl_term_signal       */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :       d26_m07_work           [work]                        */
/*                                                                    */
/* AUTHOR:      E. Kraemer, D AP 11                                   */
/*                                                                    */
/* DATE:         8. 7.91                                              */
/*                                                                    */
/* DESCRIPTION: The function waits for IPC-invoke. After invoke it    */
/*              calls the appropriate service-primitive, delivers     */
/*              results or errors to the caller and waits for the next*/
/*              invoke.                                               */
/*                                                                    */
/* RETURN-VALUE:                                                      */
/*      D2_NO_ERR       signed16        No error occurred             */
/*      D26_SYS_ERR                     system error occurred         */
/*      D26_IPC_ERR                     IPC  error occurred           */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_m07_work(
    D23_atsvpb *atsvpb, /* IN    -  parameter for attach server       */
    int argc,           /* IN    -  count of command line parameters  */
    char *argv[])       /* IN    -  list of command line parameters   */

{                               /*  d26_m07_work                      */

			/* parameters for wait for invoke             */
  static D23_wtinpb wtinpb = {{D23_V02,0,0,0},D23_INFIN};

			/* parameters for result                      */
  static D23_rsrqpb rsrqpb = {{D23_V02,0,0,0}};

			/* structure for sigaction call               */
  static struct sigaction siginfo = {0};

  static D23_remcrdpb ruc_pb = {{D23_V02}};
  static D23_readcrdpb	reuc_pb = {{D23_V02}};

			/* ipc error flag                             */
  signed16  ipcerr = FALSE;

			/* return value                               */
  signed16  ret_value = D2_NO_ERR;

  signed32  rc;         /* return value of read user credentials      */

  byte      *ipc_request;   /* string containing the collected request*/
  signed32  ipc_req_len;    /* length of collected IPC-request        */
  asn1_field asn1_request;  /* structure contains encoded request     */
  signed16  op_id;          /* operation ID of request                */
  byte      *request;       /* string containing decoded request      */
  signed32  request_len;    /* length of decoded request              */
  byte      *dap_request;   /* string containing DAP-request          */
  byte      *result;        /* string containing decoded result       */
  signed32  result_len;     /* length of decoded result               */
  byte      err_apdu[sizeof(D23_rserror) + sizeof(D23_eom)];

  signed16  name_len;       /* length of requestors name              */
  boolean   add_mk;         /* indicator whether master-knowledge     */
			    /* attribute must be added by d26_m12_    */
			    /* logg_message                           */
  time_t    idle_time;      /* time that DSA is permitted to idle     */

  d26_gtsvpb.d23_Dsvrid = atsvpb->d23_0svrid;

  sigemptyset(&siginfo.sa_mask);

  for                     /*  ever                                    */
     ( ; ; d26_shutdown = TRUE)
  {                       /*  handle distributed commands             */
    if                    /*  still idle DSP bind table entries       */
      (d26_bind_count > 1)
    {                     /*  compress bind table                     */
      d26_d34_comp_bind_table();
    }
    d27_021_reset_time_trace();
    d26_m11_distcmd(SIGUSR1);
    if                    /*  DSA has to be restarted                 */
      (d26_restart == TRUE)
    {                     /*  break;                                  */
      return(D2_NO_ERR);
    }
    siginfo.sa_handler = d26_m06_hdl_sigdistcmd;
    if                    /* signal routine cannot be activated       */
      (D27_SIGACTION(svc_c_sev_fatal,SIGUSR1,&siginfo,
       (struct sigaction *)NULL) < 0)
    {                     /* break                                    */
      return(D26_SYS_ERR);
    }

    idle_time = d26_bind_count > 1 ? d26_dspunbind_delay : 0;
    idle_time = d26_add_on == TRUE && idle_time < D26_IDLE_TIME ?
      D26_IDLE_TIME : idle_time;
    if                    /* DSA has to do delayed work               */
      (idle_time > 0)
    {                     /* rewind alarm clock                       */
      siginfo.sa_handler = d26_m05_hdl_term_signal;
      if                  /* signal routine cannot be activated       */
	(D27_SIGACTION(svc_c_sev_fatal,SIGALRM,&siginfo,
	 (struct sigaction *)NULL) < 0)
      {                   /* break                                    */
	return(D26_SYS_ERR);
      }
      alarm(idle_time);
    }

			  /*  wait for invoke message                 */
    if                    /*  error from wait for invoke              */
      (d23_waitin(&wtinpb) == D23_ERROR)
    {                     /*  set exit value , break loop             */
      alarm(0);
      return(D26_IPC_ERR);
    }
    alarm(0);

			  /* disable distributed command signal       */
    siginfo.sa_handler = SIG_IGN;
    if                    /* signal routines cannot be deactivated    */
      (D27_SIGACTION(svc_c_sev_fatal,SIGUSR1,&siginfo,
       (struct sigaction *)NULL) < 0)
    {                     /* return error                             */
      return(D26_SYS_ERR);
    }

    if                    /*  signal arrived                          */
      (wtinpb.d23_2oprid == D23_NOOPR)
    {                     /*  continue loop                           */
#ifdef THREADSAFE
      pthread_yield();
#endif
      continue;
    }
    else if               /*  association is aborted                  */
	   (wtinpb.d23_2oprid == D23_ABORT)
    {                     /*  remove user credentials                 */
      ruc_pb.d23_Ncontid = wtinpb.d23_2invinfo.d23_contid;
      ruc_pb.d23_Nusrid = wtinpb.d23_2invinfo.d23_usrid;
      d23_rem_usr_cred(&ruc_pb);
      continue;
    }

			  /* disable alarm signal                     */
    siginfo.sa_handler = SIG_IGN;
    if                    /* signal routines cannot be deactivated    */
      (D27_SIGACTION(svc_c_sev_fatal,SIGALRM,&siginfo,
       (struct sigaction *)NULL) < 0)
    {                     /* return error                             */
      return(D26_SYS_ERR);
    }

    d26_shutdown        = FALSE;
    rsrqpb.d23_3assid   = wtinpb.d23_2assid;
    rsrqpb.d23_3invinfo = wtinpb.d23_2invinfo;
    rsrqpb.d23_3rfrdt   = wtinpb.d23_2rfidt;
    rsrqpb.d23_3lnrdt   = atsvpb->d23_0mslen;
    if                    /*  error from collecting blocks            */
      ((ipc_request = d26_m03_collect_blocks(&wtinpb,&rsrqpb,&ipc_req_len))
       == NULL)
    {                     /*  skip request                            */
      continue;
    }
    else
    {                     /*  perform service request                 */
      op_id = wtinpb.d23_2oprid > D2_ADM_OPID ?
	      wtinpb.d23_2oprid - D2_ADM_OPID : wtinpb.d23_2oprid;
      DCE_SVC_LOG((GDS_S_ENTRY_OPERATION_MSG,op_id));

      if                  /*  operation ID is not OK                  */
	(op_id <= 0 || D23_DREAD < op_id)
      {                   /*  reject the request                      */
	d27_023_dump_time_trace();
	if                /*  error from rejecting the request        */
	  (d26_m01_ret_result(op_id,D23R_UNREC_OPER,NULL,0,&rsrqpb,D23_RREJECT)
	   != D2_NO_ERR)
	{                 /*  break                                   */
	  return(D26_IPC_ERR);
	}                 /*                                          */
      }
      else if             /*  database operation can't be performed   */
			  /*  accept DS_UNBIND or DS_ABANDON only     */
	     (d26_dbop_flag == D26_DB_NOOP &&
	      op_id != D23_UNBIND && op_id != D23_ABANDON)
      {                   /*  return error message                    */
	d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
	  D2_INT_ERR,op_id == D23_BIND ? D2_NO_RESPONSE : D2_TOO_BUSY,
	  &result_len);
	d27_023_dump_time_trace();
	if                /*  error from return result                */
	  (d26_m01_ret_result(op_id,0,err_apdu,result_len,&rsrqpb,D23_RERROR)
	   != D2_NO_ERR)
	{                 /*  break                                   */
	  return(D26_IPC_ERR);
	}
	else
	{                 /*  wait for next invoke                    */
	  continue;
	}
      }

      if                  /*  unload factor is positive               */
	(d26_unloadf > 0 && d26_dbop_flag != D26_DB_NOOP)
      {                   /*  read server information                 */
	d26_gtsvpb.d23_Dunloadf = 0;
	if                /*  server information could be read        */
	  (d23_getsvrinfo(&d26_gtsvpb) != D23_ERROR)
	{                 /*  check number of busy DSA-processes      */
	  if              /*  all DSA-processes are busy              */
	    (d26_gtsvpb.d23_Dnototpr == d26_gtsvpb.d23_Dnobsypr)
	  {               /*  fork a new DSA-process out              */
	    d26_m19_set_shdlogging(argc,argv);
	    d26_m09_execv(argc,argv);
	    D27_022_ENTER_TIME_TRACE(GDS_S_FORKED_OK,NULL,0,0);
	  }
	}
      }

      if                  /*  operation is not bind or abandon and DAP*/
			  /*  message came in or unbind came in       */
	(op_id != D23_BIND && op_id != D23_DREAD && !(op_id == D23_ABANDON &&
	 D23_GAC(wtinpb.d23_2invinfo.d23_contid) == D27_DAP_AC)
	 || op_id == D23_UNBIND)
      {                   /*  take requestor from shared memory       */
	reuc_pb.d23_Ocontid = wtinpb.d23_2invinfo.d23_contid;
	reuc_pb.d23_Ousrid = wtinpb.d23_2invinfo.d23_usrid;
	reuc_pb.d23_Ocredref = (byte *) &d26_requestor;

	rc = d23_read_usr_cred(&reuc_pb);
	D27_022_ENTER_TIME_TRACE(GDS_S_READ_USRCRED,NULL,0,0);
	if                /*  requestor is missing                    */
	  (rc == D23_ERROR)
	{                 /*  check whether it is a critical case     */
	  if              /*  DAP is processed                        */
	    (D23_GAC(wtinpb.d23_2invinfo.d23_contid) == D27_DAP_AC)
	  {               /*  return error message                    */
	    d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
	      D2_REM_ERR,D2_NO_RESPONSE,&result_len);
	    d27_023_dump_time_trace();
	    if            /*  error from return result                */
	      (d26_m01_ret_result(op_id,0,err_apdu,result_len,&rsrqpb,
	       D23_RERROR) != D2_NO_ERR)
	    {             /*  break                                   */
	      return(D26_IPC_ERR);
	    }
	    else
	    {             /*  get next request                        */
	      continue;
	    }
	  }
	  else
	  {               /*  set schema modification flag            */
	    d26_requestor.d26_sm_flag = D26_NO_SM_PENDING;
	  }
	}
	else
	{                 /*  norm requestor name                     */
	  name_len = strlen(d26_requestor.d26_req_name) + 1;
	  if              /* requestor is not anonymous               */
	    (strcmp(d26_requestor.d26_req_name,D26_ANONYMOUS) != 0)
	  {               /* norm it                                  */
	    d27_007_norm_name(d26_requestor.d26_req_name,name_len,
	      d26_requestor.d26_normed_req_name);
	    d27_012_remove_blanks(d26_requestor.d26_normed_req_name,name_len,
	      d26_requestor.d26_normed_req_name);
	  }
	  else
	  {               /* set other components fixed               */
	    strcpy(d26_requestor.d26_normed_req_name,D26_ANONYMOUS);
	  }
	  strcpy(d26_requestor.d26_acl_name,
	    d26_requestor.d26_normed_req_name);

	  if              /* DSP operation is processed               */
	    (D23_GAC(wtinpb.d23_2invinfo.d23_contid) == D27_DSP_AC)
	  {
	/* check wether operation is allowed for the DSA -            */
	/* if not, the ACL has to be checked against ANONYMOUS        */

	    if (d26_requestor.d26_trust_level == TDT_NOT_TRUSTED ||
		(d26_requestor.d26_trust_level == TDT_TRUSTED_FOR_READ &&
		 (op_id == D23_ADDOBJT || op_id == D23_REMOBJT ||
		  op_id == D23_MODENTR || op_id == D23_REPLRDN)) ||
		(d26_requestor.d26_trust_level == TDT_TRUSTED_FOR_MOD &&
		 (op_id == D23_READ || op_id == D23_COMPARE ||
		  op_id == D23_LIST || op_id == D23_SEARCH)))
	    {
	      strcpy(d26_requestor.d26_acl_name, D26_NOT_TRUSTED);
	    }
	  }
	}
      }

      if                  /*  request is not in local format          */
	(op_id == D23_BIND &&
	 D23_GACM(wtinpb.d23_2invinfo.d23_contid) == D27_REM_AC ||
	 op_id != D23_UNBIND &&
	 (D23_GTS(wtinpb.d23_2invinfo.d23_contid) == D27_ASN1_TS ||
	  D23_GTS(wtinpb.d23_2invinfo.d23_contid) == D27_ASN1_ADM_TS))
      {                   /*  decode the request                      */
	asn1_request.l = ipc_req_len;
	asn1_request.v = ipc_request;
	if                /*  decoding error occurred                 */
	  (d2a001_asn1_apdu(op_id,D23_GAS(wtinpb.d23_2invinfo.d23_contid),
	   &request,&request_len,&asn1_request,err_apdu) == D2_ERROR)
	{                 /*  check the operation ID                  */
	  D27_022_ENTER_TIME_TRACE(GDS_S_DECODING_ERR,NULL,0,0);
	  d27_023_dump_time_trace();
	  if              /*  error is in Bind message decoding       */
	    (op_id == D23_BIND)
	  {               /*  return the error                        */
	    if            /*  error from returning the result         */
	      (d26_m01_ret_result(op_id,0,NULL,0,&rsrqpb,D23_ABORT)
	       != D2_NO_ERR)
	    {             /*  break                                   */
	      return(D26_IPC_ERR);
	    }
	  }
	  else if         /*  error from rejecting the result         */
		 (d26_m01_ret_result(op_id,D23R_MISTYPE_ARG,NULL,0,&rsrqpb,
		  D23_RREJECT) != D2_NO_ERR)
	  {               /*  break                                   */
	    return(D26_IPC_ERR);
	  }
	  continue;
	}
	else
	{                 /* enter time trace                         */
	  D27_022_ENTER_TIME_TRACE(GDS_S_DECODING_OK,NULL,0,0);
	}
      }
      else
      {                   /*  request is in local format already      */
	request     = ipc_request;
	request_len = ipc_req_len;
      }

      DCE_SVC_LOG((GDS_S_APDU_REQUEST_MSG,op_id,request_len,request));
      if                  /*  error from allocating result memory     */
	((result = (byte *)D27_MALLOC(svc_c_sev_warning,D26_BLOCK_LEN))
	 == NULL)
      {                   /*  return error message                    */
	d27_010_set_error_apdu((D23_rserror *)err_apdu,D2_TEMP_ERR,
	  D2_REM_ERR,D2_UNWILLING,&result_len);
	if                /*  error from return result                */
	  (d26_m01_ret_result(op_id,0,err_apdu,result_len,&rsrqpb,D23_RERROR)
	   != D2_NO_ERR)
	{                 /*  break                                   */
	  return(D26_IPC_ERR);
	}
	else
	{                 /*  get next request                        */
	  continue;
	}
      }
      else
      {                   /*  reset result length                     */
	result_len = D26_BLOCK_LEN;
      }

			  /* initialize count of DB accesses          */
      d26_dbacc_count = 0L;
      d26_abandoned   = FALSE;
      switch(op_id)
      {
	case D23_BIND:
			  /*  perform service request DS_BIND         */
	  ret_value = d26_1_bind(request,result,&result_len,
	    wtinpb.d23_2invinfo.d23_contid,wtinpb.d23_2invinfo.d23_usrid);
	  break;
	case D23_READ:
			  /*  perform service request DS_READ         */
	  ret_value = d26_3_read(wtinpb.d23_2oprid,request,request_len,&result,
	    &result_len,&rsrqpb);
	  break;
	case D23_LIST:
			  /*  perform service request DS_LIST         */
	  ret_value = d26_4_list(request,request_len,&result,&result_len,
	    &rsrqpb);
	  break;
	case D23_SEARCH:
			  /*  perform service request DS_SEARCH       */
	  ret_value = d26_5_search(wtinpb.d23_2oprid,request,request_len,
	    &result,&result_len,&rsrqpb);
	  break;
	case D23_COMPARE:
			  /*  perform service request DS_COMPARE      */
	  ret_value = d26_6_compare(request,request_len,&result,&result_len,
	    &rsrqpb);
	  break;
	case D23_ADDOBJT:
			  /*  perform service request DS_ADD_OBJECT   */
	  ret_value = d26_7_add_object(wtinpb.d23_2oprid,
	    (D23_inaddobjt *)request,request_len,&dap_request,&result,
	    &result_len,&add_mk);
	  break;
	case D23_MODENTR:
			  /*  perform service request DS_MODIFY_ENTRY */
	  ret_value = d26_C_modify_entry(wtinpb.d23_2oprid,
	    (D23_inmodentr *)request,request_len,&dap_request,&result,
	    &result_len,wtinpb.d23_2invinfo.d23_contid,
	    wtinpb.d23_2invinfo.d23_usrid);
	  d26_requestor.d26_sm_flag =
	    d26_requestor.d26_sm_flag == D26_SM_PENDING &&
	    ret_value != D2_NO_ERR ?
	      D26_SM_BROKEN : d26_requestor.d26_sm_flag;
	  add_mk = FALSE;
	  break;
	case D23_REMOBJT:
			  /*  perform service request DS_REMOVE_OBJECT*/
	  ret_value = d26_8_remove_object(wtinpb.d23_2oprid,(byte *)request,
	    request_len,&dap_request,&result,&result_len);
	  add_mk = FALSE;
	  break;
	case D23_REPLRDN:
			  /*  perform service request DS_REPLACE_RDN  */
	  ret_value = d26_B_replace_rdn(wtinpb.d23_2oprid,
	    (D23_in_replrdn *)request,request_len,&dap_request,&result,
	    &result_len);
	  add_mk = FALSE;
	  break;
	case D23_UNBIND:
			  /*  perform service request DS_UNBIND       */
	  ret_value = d26_2_unbind(request,result,&result_len,
	    wtinpb.d23_2invinfo.d23_contid,wtinpb.d23_2invinfo.d23_usrid);
	  break;
	case D23_ABANDON:
			  /*  perform service request DS_ABANDON      */
	  ret_value = d26_9_abandon(request,&result,&result_len,
	    wtinpb.d23_2invinfo.d23_contid,wtinpb.d23_2invinfo.d23_usrid);
	  break;
	case D23_DREAD :
			  /*  perform daemon request to get bind msg. */
	  ret_value = d26_m15_get_bind_info(request,request_len, &result,
	    &result_len);
	  break;
      }

      D27_022_ENTER_TIME_TRACE(GDS_S_REQUEST_PERFORMED,NULL,0,0);

      if                  /*  logging required                        */
	((op_id == D23_ADDOBJT || op_id == D23_MODENTR ||
	  op_id == D23_REMOBJT || op_id == D23_REPLRDN) &&
	 d26_ulogging == TRUE && d26_ma_info == TRUE &&
	 ret_value == D2_NO_ERR)
      {                   /*  call logging function                   */
	d26_m12_logg_message(dap_request,
	  request_len - (dap_request - request),op_id + D2_ADM_OPID,FALSE,
	  add_mk);
	D27_022_ENTER_TIME_TRACE(GDS_S_SHADOW_UPDATE_MSG_WRITTEN,NULL,0,0);
      }

      if                  /*  request was decoded                     */
	(request != ipc_request)
      {                   /*  free it                                 */
	free(request);
      }

      if                  /*  schema modification has terminated      */
	(d26_requestor.d26_sm_flag == D26_SM_BROKEN ||
	 d26_requestor.d26_sm_flag == D26_SM_COMPLETE)
      {                   /*  restart DSA processes                   */
	if                /*  DSAs cannot restart                     */
	  ((ret_value =
	   (d26_m18_restart(d26_requestor.d26_sm_flag == D26_SM_BROKEN ?
	    TRUE : FALSE,wtinpb.d23_2invinfo.d23_contid,
	    wtinpb.d23_2invinfo.d23_usrid)) == D26_ERROR))
	{                 /*  set error message                       */
	  d27_010_set_error_apdu((D23_rserror *)result,D2_TEMP_ERR,D2_INT_ERR,
	    D2_UNWILLING,&result_len);
	}
	D27_022_ENTER_TIME_TRACE(GDS_S_DSAS_RESTARTED,NULL,0,0);
      }

      if                  /* operation was not abandoned              */
	(d26_abandoned == FALSE)
      {                   /* return the result                        */
	if                /* error from returning result              */
	  (d26_m01_ret_result(op_id,0,result,result_len,&rsrqpb,
	   ret_value == D2_NO_ERR ? D23_RESULT :
	   (ret_value == D23_RREJECT ? D23_RREJECT: D23_RERROR))
	   != D2_NO_ERR)
	{                 /* set ipc error                            */
	  ipcerr = TRUE;
	}
      }

      free(result);
    }

    DCE_SVC_LOG((GDS_S_EXIT_OPERATION_MSG,op_id));
    d27_023_dump_time_trace();

    if                    /*  ipc error set                           */
      (ipcerr == TRUE)
    {                     /*  return error                            */
      return(D26_IPC_ERR);
    }
    else if               /*  DSA must be restarted (because of schema*/
			  /*  modification) or must shut down         */
	   (d26_restart == TRUE || d26_shutdown == TRUE)
    {                     /*  return without error                    */
      return(D2_NO_ERR);
    }
  }

}                               /*  d26_m07_work                      */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_m18_restart                                       */
/*                                                                    */
/* AUTHOR:      Kraemer D AP 11                                       */
/* DATE:        07.11.91                                              */
/*                                                                    */
/* DESCRIPTION: The function sends a distributed command to the other */
/*              processes to close the database. If the schema        */
/*              modification was broken, the old schema object is     */
/*              restored. Then a distributed command is sent to the   */
/*              other processes to restart.                           */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR       Short       no error occurred                 */
/*      D26_ERROR                   error occurred                    */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_m18_restart(
    boolean sm_broken,  /* IN    -  indicates whether a schema        */
			/*          modification was broken           */
    unsigned16 cont_id, /* IN    -  context ID                        */
    signed32 usr_id)    /* IN    -  user ID                           */

{                                    /*  d26_m18_restart              */

  signed32 rc;              /* return value of d23_dwrite_cmd         */
			    /* return value of function               */
  signed16 ret_code = D2_NO_ERR;
  signed16 i;               /* loop variable                          */

  char     command[20 + 3 * D27_LFILE_NAME + 5 * (30 + D27_LFILE_NAME)
		   + (200 + D27_LFILE_NAME)];
  char     *cmd_ptr;        /* pointer to command line                */
			    /* table of routing specifications        */
  dce_svc_stringarray_t table;
				    /* error status for serviceability*/
  error_status_t st;                /* functions                      */

			    /* parblocks for ipc functions	      */
  D23_addcrdpb	auc_pb = {{D23_V02}};
  D23_dwritepb  wdc_pb = {{D23_V02}};

  if                /*  schema modification is broken                 */
    (sm_broken == TRUE)
  {                 /*  clean chdb directory                          */
    if              /*  error                                         */
      (d26_mi7_clean_chdb() != D2_NO_ERR)
    {               /*  return error                                  */
      ret_code = D26_ERROR;
    }
  }

  d26_requestor.d26_sm_flag = D26_NO_SM_PENDING;

  auc_pb.d23_Mcontid = cont_id;
  auc_pb.d23_Musrid = usr_id;
  auc_pb.d23_Mcredsize = D26_REQ_LEN;
  auc_pb.d23_Mcredref = (byte *) &d26_requestor;

  if                /*  user credentials cannot be overwritten        */
    (d23_add_usr_cred(&auc_pb) != D2_NOERROR)
  {                 /*  set ipc error                                 */
    ret_code = D26_ERROR;
  }

	       /* initialize parameter block for d23_write_dist_cmd() */
  wdc_pb.d23_Jsv_prid = D23_DS0SID + d26_dir_id;
  wdc_pb.d23_Jtmout = D23_DMAXTOUT;
  wdc_pb.d23_Jparsize = 0;
  wdc_pb.d23_Jparref = NULL;

  if (ret_code != D26_ERROR)
  {
    d26_mi4_close_files();
    d26_i06_close_dnlistfile();

			  /*  disable modify op. in all DSAs          */
			  /*  write distributed command to other DSAs */
    wdc_pb.d23_Jcmd = D23_DISDBOP;
    if ((rc = d23_write_dist_cmd(&wdc_pb)) != D23_SUCES)
    {
      if                  /* check wether this DSA is allone          */
	(d23_getsvrinfo(&d26_gtsvpb) != D23_ERROR)
      {                   /*  check number of busy DSA-processes      */
	if (d26_gtsvpb.d23_Dnototpr == 1)
	{
	  rc = D23_SUCES;
	}
      }
    }

    if
      (rc == D23_SUCES)
    {               /*  check success of gdschdb                      */
      cmd_ptr = command;
      sprintf(cmd_ptr,"%s %s %s ",d26_chdb_name,d26_install,d26_logdir);
      cmd_ptr += strlen(cmd_ptr);
      dce_svc_routings(&table,FALSE,&st);
      if            /* routing specification available                */
	(st == svc_s_ok)
      {             /* append specifications to command line          */
	for         /* all routing specifications                     */
	   (i = 0; i < table.tab_size; i++)
	{           /* copy string to the command line                */
	  sprintf(cmd_ptr,"-w\"%s\" ",table.table[i]);
	  cmd_ptr += strlen(cmd_ptr);
	}
      }
      rpc_ss_free(table.table);
      dce_svc_routings(&table,TRUE,&st);
      if            /* routing specification available                */
	(st == svc_s_ok)
      {             /* append specifications to command line          */
	for         /* all routing specifications                     */
	   (i = 0; i < table.tab_size; i++)
	{           /* copy string to the command line                */
	  sprintf(cmd_ptr,"-v\"%s\" ",table.table[i]);
	  cmd_ptr += strlen(cmd_ptr);
	}
      }
      rpc_ss_free(table.table);
      if ((rc = D27_SYSTEM(svc_c_sev_warning,command)) != 0)
      {                     /* set return code                        */
	ret_code = D26_ERROR;
      }
      else
      {
	wdc_pb.d23_Jcmd = D23_CLDBFILES;
	if ((rc = d23_write_dist_cmd(&wdc_pb)) != D23_SUCES)
	{
	  if                /* check wether this DSA is allone          */
	    (d23_getsvrinfo(&d26_gtsvpb) != D23_ERROR)
	  {                 /*  check number of busy DSA-processes      */
	    if (d26_gtsvpb.d23_Dnototpr == 1)
	    {
	      rc = D23_SUCES;
	    }
	  }
	}

	if (rc == D23_SUCES)
	{
	  sprintf(command,"ls %s%s | xargs -i cp %s%s/{} %s",
			  d26_install, D26_CHDB_DIR,
			  d26_install, D26_CHDB_DIR,
			  d26_install);
	  if ((rc = D27_SYSTEM(svc_c_sev_warning,command)) != 0)
	  {                 /* set return code                        */
	    ret_code = D26_ERROR;
	  }
	  else
	  {
	    wdc_pb.d23_Jcmd = D23_RESTART;
	    d23_write_dist_cmd(&wdc_pb);
			    /* processing continues even after errors */
			    /* some processes may have come up already*/
	    d26_mi7_clean_chdb();
	  }
	}
	else
	{
	  DCE_SVC_LOG((GDS_S_SCHMODERR_MSG));
	  ret_code = D26_ERROR;
	}
      }
    }
    else
    {
      DCE_SVC_LOG((GDS_S_SCHMODERR_MSG));
      ret_code = D26_ERROR;
    }
  }

  if (ret_code == D26_ERROR)
  {
    d26_mi7_clean_chdb();
    d26_restart = FALSE;
		    /*  enable modify op. in all DSAs                 */
		    /*  write distributed command to other DSAs       */
    wdc_pb.d23_Jcmd = D23_ENADBOP;
    d23_write_dist_cmd(&wdc_pb);
  }
  else
  {
		    /*  set flag that this DSA must be restarted too  */
    d26_restart = TRUE;
  }

  return(ret_code);

}                                   /*  d26_m18_restart               */

/************************************************************************/
/*								      	*/
/* Function Name : d26_m19_set_shdlogging                            	*/
/*								      	*/
/* Description	 : This function sets the shadow update logging para-   */
/*		   according the current value of d26_ulogging.		*/
/*								      	*/
/* Return Value  : none							*/
/*								      	*/
/* Author        : Loose, SNI BU BA NM123                               */
/* Date          : 30.05.94						*/
/*								      	*/
/************************************************************************/
static void d26_m19_set_shdlogging(int argc, char *argv[])
{

int i;

for (i = 1; i < argc; i++)
{
  if (argv[i][0] == '-' && argv[i][1] == 'l')
  {
    if (argv[i][2] == 'e' && d26_ulogging == FALSE)
    { /* disable shadow update logging */
      argv[i][2] = 'd';
      break;
    }
    else if (argv[i][2] == 'd' && d26_ulogging == TRUE)
    { /* enable shadow update logging */
      argv[i][2] = 'e';
      break;
    }
  }
}

}                                   /*  d26_m19_set_shdlogging        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :       main(argc,argv)        [main]                        */
/*                                                                    */
/* AUTHOR:      E. Kraemer, D AP 11                                   */
/*                                                                    */
/* DATE:        17.12.87                                              */
/*                                                                    */
/* DESCRIPTION: The function sets up signal handling and global flags */
/*              from the parameters. It checks the key and forks the  */
/*              DSA if required. It sets up the session logging, calls*/
/*              the initializing function and attaches the DSA to IPC */
/*              as a server. Then it goes to work.                    */
/*              If work is broken for some reason, it detaches the DSA*/
/*              from IPC and shuts DSA down. If required it restarts  */
/*              the DSA automatically.                                */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Int             argc            Number of parameters          */
/*      String          argv[1]         -d<nn>: Directory ID          */
/*      String          argv[2]         -c: no cache update           */
/*      String          argv[4]         -l: no logging of update      */
/*                                          message                   */
/*      String          argv[5]         -o: Add on DSA                */
/*      String          argv[6]         -r: DSA was restarted         */
/*      String          argv[7]         -s<mm>: unload factor         */
/*      String          argv[8]         -f: DSA should not fork       */
/*      String          argv[9]         -a: ASN1-logging required     */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      None                                                          */
/*                                                                    */
/* EXITVALUE:                                                         */
/*      D2_NO_ERR       Short           No error occurred             */
/*          1                           Error occurred                */
/*exoff ***************************************************************/

main(int argc, char *argv[])

{                               /*  main                              */

			/* parameters for attach server               */
  static D23_atsvpb atsvpb = {{D23_V02,0,0,0},0,D23_SYNC_EM};

			    /* parameters for attach client           */
  static D23_atclpb atclpb = {{D23_V02,0,0,0},D23_SYNC_EM};

			/* exit-value                                 */
  int    exit_value = D2_NO_ERR;

  int    fork_result;               /* result from fork-call          */

  boolean  conf_restart = FALSE;
  boolean  minus_l_given = FALSE;
  char     minus_le_param[] = "-le";
  int      new_argc;
  char     *new_argv[D26_NO_ARG + 2];

				    /* structure for sigaction call   */
  static struct sigaction siginfo = {0};

				    /* parblocks for ipc	      */
  static D23_dreadpb     rdc_pb = {{D23_V02}};
  static D23_dconfpb     cdc_pb = {{D23_V02}};
  static D23_regprocpb   rp_pb  = {{D23_V02}};

  boolean  do_fork   = TRUE;        /* indicates whether DSA must fork*/
  boolean  auth_mech_set = FALSE;   /*auth. mech set in parameter list*/

#ifndef NOT_CHECK_KEY
  static char *ck_err[] = {
			     "CHECK_KEY ERROR: ",
			     "No permission rights to run DIRECT.X",
			     "Format of SAFE-data file is wrong",
			     "Request to OPEN SAFE-DATA has failed",
			     "Request to READ SAFE-DATA has failed",
			     "Connection request to silsd demon has failed"
			  };

  int      x_ck_err, x_l, x_sv;
#endif

				    /* error status for serviceability*/
  error_status_t st;                /* functions                      */

				    /* DSA process name for execv()   */
  char    dsa_proc_name[D27_LFILE_NAME];
				    /* error message before           */
				    /* serviceability                 */
  char    err_message[D27_LFILE_NAME];

  register signed16 i;    /* loop variable                              */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

  setlocale(LC_ALL, "");

  sprintf(dsa_proc_name,"%s%s%s",dcelocal_path,D27_BIN_DIR,D26_DSA_PROC);
  new_argv[0] = dsa_proc_name;

for                     /*  all parameters                            */
   (i = new_argc = 1; i < argc; i++)
{                       /*  check single parameter                    */
  if                    /*  not label parameter                       */
    (argv[i][0] != '-')
  {                     /*  take next parameter                       */
    continue;
  }
  else if               /*  too much parameters found                 */
	 (new_argc >= D26_NO_ARG)
  {                     /*  set exit_value                            */
    fprintf(stderr,"%s: Too much parameters\n",dsa_proc_name);
    exit(D27_INV_PAR);
  }
  else
  {                     /*  check label                               */
    switch(argv[i][1])
    {
      case    'c' :   d26_upd_cache = FALSE;
		      new_argv[new_argc++] = argv[i];
		      break;
      case    'd' :   d26_dir_id    = atoi(argv[i] + 2);
		      new_argv[new_argc++] = argv[i];
		      break;
      case    'f' :   do_fork       = FALSE;
		      break;
      case    'l' :   switch (argv[i][2])
		      {
			case 'd': d26_ulogging  = FALSE;
				  break;
			case 'e': d26_ulogging  = TRUE;
				  break;
			default:  d26_ulogging  = TRUE;
				  argv[i][2] = 'e';
				  break;
      		      } 
		      new_argv[new_argc++] = argv[i];
		      minus_l_given = TRUE;
		      break;
      case    'r' :   conf_restart  = TRUE;
		      do_fork       = FALSE;
		      break;
      case    's' :   d26_unloadf   = atoi(argv[i] + 2);
		      new_argv[new_argc++] = argv[i];
		      break;
      case    'o' :   d26_add_on    = TRUE;
		      break;
      case    'P' :
      case    'v' :
      case    'w' :   /* routing specification is handled later       */
		      break;
      case    'A' :   switch (atoi(argv[i] + 2))
		      {
			case D2_ANONYMOUS:
				break;
			case D2_SIMPLE:
				d26_auth_mech[D2_SIMPLE] = TRUE;
				break;
			case D2_DCE_AUTH:
				d26_auth_mech[D2_DCE_AUTH] = TRUE;
				break;
			case D2_STRONG:
				d26_auth_mech[D2_STRONG] = TRUE;
				break;
			case D26_SIMP_PLUS_DCE:
				d26_auth_mech[D2_SIMPLE] = TRUE;
				d26_auth_mech[D2_DCE_AUTH] = TRUE;
				break;
			case D26_SIMP_PLUS_STRONG:
				d26_auth_mech[D2_STRONG] = TRUE;
				d26_auth_mech[D2_DCE_AUTH] = TRUE;
				break;
			default:			
				fprintf(stderr,
				  "%s: Invalid authentication mechanism.\n",
				  dsa_proc_name);
				exit(D27_INV_AUTH_MECH);
				break;
		      }
		      d26_auth_mech[D2_ANONYMOUS] = TRUE;
		      auth_mech_set = TRUE;
		      new_argv[new_argc++] = argv[i];
		      break;
      default     :   break;
    }   
  }
}

sprintf(err_message,"dsa, dir_id: %d",d26_dir_id);

umask((mode_t)D26_UMASK);

if                      /*  authentication mechainsm not given	      */
  (!auth_mech_set)
{                       /*  set exit_value                            */
  fprintf(stderr,"%s: No authentication meachansim given\n",err_message);
  exit(D27_NO_AUTH_MECH);
}

if                      /*  shadow update logging parameter	      */
  (!minus_l_given)
{                       /*  append parameter to parameter-list        */
  d26_ulogging = TRUE;
  if (new_argc + 1 < D26_NO_ARG)
  {
    new_argv[new_argc++] = minus_le_param;
  }
  else
  {                     /*  set exit_value                            */
    fprintf(stderr,"%s: Too much parameters\n",err_message);
    exit(D27_INV_PAR);
  }
}

if                      /*  directory ID is not ok                    */
  (!D2_CHECK_MINMAX(D2MINDIR_ID,D2MAXDIR_ID,d26_dir_id))
{                       /*  set exit_value                            */
  fprintf(stderr,"%s: Wrong directory ID\n",err_message);
  exit(D27_INV_DIR_ID);
}
else
{                       /*  write name of installation directory      */
#ifndef NOT_CHECK_KEY
  if                    /*  error from check key                      */
    ((x_l = _check_key("DIR-X-L",DIR_VERSION)) != 0 &&
     (x_sv = _check_key("DIR-X-SV",DIR_VERSION)) != 0)
  {   
    x_ck_err = x_l > 0 ? x_l : x_sv;
    exit(D27_CHK_KEY_ERR + x_ck_err);
  }
#endif
  sprintf(d26_install,"%s%s%s%s%d",dcelocal_path,D27_SERVER_DIR,D27_DSA_DIR,
    D26_DIR,d26_dir_id);
  sprintf(d26_logdir,"%s%s%s%s%s%d",dcelocal_path,D27_SERVER_DIR,D27_DCE_ADM,
    D27_DSA_DIR,D26_DIR,d26_dir_id);
  sprintf(d26_chdb_name,"%s%s%s",dcelocal_path,D27_BIN_DIR,d26_chdb_prog);
  if                    /* working directory cannot be changed        */
    (chdir(d26_logdir) < 0)
  {                     /* give error note and exit                   */
    fprintf(stderr,"Unable to chdir to %s, errno: %d\n",d26_logdir,errno);
    exit(D27_CHDIR_ERR);
  }
  if                    /*  DSA has to fork                           */
    (do_fork == TRUE)
  {                     /*  create DSA-process as child               */
    if                  /*  error from forking process                */
      ((fork_result = fork()) == -1)
    {                   /*  log error                                 */
      fprintf(stderr,"error from forking, errno: %d\n",errno);
      exit(D27_FORK_ERR);
    }
    else if             /*  father process                            */
	   (fork_result > 0)
    {                   /*  terminate                                 */
      exit(0) ;
    }
  }

  d27_011_close_unused_files(FALSE);
			/*  set a new process group                   */
  setpgrp();
  d27_pid = getpid();
  d27_001_svc_init(argc,argv,&st);
  if (st != svc_s_ok)
  {
    fprintf(stderr,"Unable to initialize serviceability, status: 0x%lx\n",
	    st);
    exit(D26_INIT_SVC_ERR);
  }
  dce_svc_printf(GDS_S_ENTRYDSA_MSG,d26_dir_id);
  sigemptyset(&siginfo.sa_mask) ;
  for                   /*  all possible signals                      */
     (i = 1; i <= NSIG; i++)
  {                     /*  initialize signal handle function         */
    switch(i)
    {
      case SIGUSR1:
	siginfo.sa_handler = SIG_IGN;
	if                /* signal routine cannot be activated       */
	  (D27_SIGACTION(svc_c_sev_fatal,i,&siginfo,(struct sigaction *)NULL)
	   < 0)
	{                 /* error exit                               */
	  exit(D26_SYS_ERR);
	}
	break;
      case SIGTERM:
	siginfo.sa_handler = d26_m05_hdl_term_signal;
	if                /* signal routine cannot be activated       */
	  (D27_SIGACTION(svc_c_sev_fatal,SIGTERM,&siginfo,
	   (struct sigaction *)NULL) < 0)
	{                 /* error exit                               */
	  exit(D26_SYS_ERR);
	}
	break ;
      case SIGKILL:
      case SIGSTOP:
      case SIGCHLD:
      case SIGVTALRM:
	break;
      default:
#ifdef D2_CATCH_SIG
	siginfo.sa_handler = d27_000_log_signal;
	if                /* signal routine cannot be activated       */
	  (D27_SIGACTION(svc_c_sev_fatal,i,&siginfo,
	   (struct sigaction *)NULL) < 0)
	{                 /* error exit                               */
	  exit(D26_SYS_ERR);
	}
#endif
	break ;
    }
  }

  atsvpb.d23_0svrid = D23_DS0SID + d26_dir_id;
  if                    /*  DSA is not attached                       */
    (conf_restart == FALSE)
  {                     /*  registrate at IPC                         */
    rp_pb.d23_Iptype = (signed32)atsvpb.d23_0svrid;
    rp_pb.d23_Idirid = (signed32)d26_dir_id;
    rp_pb.d23_Iotype = TRUE;
    if                  /*  DSA cannot registrate                     */
      (d23_reg_process(&rp_pb) == D23_ERROR)
    {                   /*  error exit                                */
      exit(D26_IPC_ERR);
    }                   /*                                            */
  }                     /*                                            */
  if                    /*  error from initialization of DSA          */
    (d26_u09_init_dsa(d26_dir_id))
  {                     /*  set exit_value                            */
    exit_value = D26_INIT_DSA;
  }                     /*                                            */
  if                    /*  DSA is not attached                       */
    (conf_restart == FALSE)
  {                     /*  registrate at IPC                         */
    rp_pb.d23_Iptype = (signed32)atsvpb.d23_0svrid;
    rp_pb.d23_Idirid = (signed32)d26_dir_id;
    rp_pb.d23_Iotype = FALSE;
    if                  /*  error from unregister DSA process         */
      (d23_reg_process(&rp_pb) == D23_ERROR)
    {                   /*  set exit value                            */
      exit_value = D26_IPC_ERR;
    }                   /*                                            */
  }                     /*                                            */
  if                    /*  no error occurred                         */
    (exit_value == D2_NO_ERR)
  {                     /*  attach DSA as client                      */
    if                  /*  error from ipc-client attach              */
      (d23_attclt(&atclpb) == D23_ERROR)
    {                   /*  set exit value                            */
      exit_value = D26_IPC_ERR;
    }
    else
    {                   /*  attach DSA as server                      */
      d26_shutdown = D26_CLATTACH;
      if                /*  error from ipcs attach                    */
	(d23_attsvr(&atsvpb) == D23_ERROR)
      {                 /*  set exit_value and error_codes            */
	exit_value = D26_IPC_ERR;
      }
      else
      {                 /*  check restart flag                        */
	d26_shutdown = TRUE;
	if              /*  DSA is restarted                          */
	  (conf_restart == TRUE)
	{               /*  read command again and confirm command    */
	  if            /*  command can be read                       */
	    (d23_read_dist_cmd(&rdc_pb) == D2_NOERROR)
	  {             /*  confirm command                           */
            cdc_pb.d23_Lkey = rdc_pb.d23_Kkey;
	    d23_conf_dist_cmd(&cdc_pb);
	  }
	}
	if              /*  allocating memory for saved invoke data   */
			/*  fails                                     */
	  ((d26_save_inv_data = (byte *)D27_MALLOC(svc_c_sev_fatal,
	   (unsigned)atsvpb.d23_0mslen)) == NULL)
	{               /*  set exit value                            */
	  exit_value = D26_IPC_ERR;
	}
	else
	{               /*  go to work                                */
	  dce_svc_printf(GDS_S_INITDSA_MSG,d26_dir_id);
	  exit_value = d26_m07_work(&atsvpb,new_argc,new_argv);
	  d27_021_reset_time_trace();
	}
	if              /*  DSA has to be restarted                   */
	  (d26_restart == TRUE && exit_value == D2_NO_ERR)
	{               /*  detach DSA as client                      */
	  d23_detclt(&d26_declpb);
	  d26_m04_shutdown();
	  d26_m08_set_logging(&new_argc,new_argv);
	  DCE_SVC_LOG((GDS_S_RESTART_DSA_MSG));

	  d26_m19_set_shdlogging(new_argc,new_argv);
	  new_argv[new_argc++] = "-r";
	  if            /*  DSA was added on                          */
	    (d26_add_on == TRUE)
	  {             /*  set add on parameter                      */
	    new_argv[new_argc++] = D26_ADDON_PAR;
	  }             /*                                            */
	  new_argv[new_argc++] = (String)NULL;
	  D27_EXECV(svc_c_sev_fatal,new_argv[0],new_argv);
	  exit_value = D26_EXECV_ERR;
	}               /*  DSA restarted                             */
			/*  detach server                             */
	if              /*  error from ipcs detach                    */
	  (d23_detsvr(&d26_desvpb) == D23_ERROR)
	{               /*  set exit_value                            */
	  exit_value = D26_IPC_ERR;
	}
      }                 /*  server-attach was ok                      */
      if                /*  error from detach client                  */
	(d23_detclt(&d26_declpb) == D23_ERROR)
      {                 /*  set exit value                            */
	exit_value = D26_IPC_ERR;
      }
    }                   /*  client-attach was ok                      */
  }
  else
  {                     /*  read and confirm distributed command      */
    if                  /*  command can be read                       */
      (d23_read_dist_cmd(&rdc_pb) == D2_NOERROR)
    {                   /*  confirm command                           */
      cdc_pb.d23_Lkey = rdc_pb.d23_Kkey;
      d23_conf_dist_cmd(&cdc_pb);
    }
    if                  /*  DSA was restarted, but didn't come up     */
      (conf_restart == TRUE)
    {                   /*  detach as server                          */
      if                /*  error from ipcs detach                    */
	(d23_detsvr(&d26_desvpb) == D23_ERROR)
      {                 /*  set exit_value                            */
	exit_value = D26_IPC_ERR;
      }
    }
  }

  if                    /*  error from shutdown DSA                   */
    (d26_m04_shutdown())
  {                     /*  set exit_value                            */
    exit_value = D26_SHUT_ERR;
  }
  dce_svc_printf(GDS_S_EXITDSA_MSG,d26_dir_id);
}

			/*  exit with exit_value                      */
exit(exit_value);

}                               /*  main                              */
