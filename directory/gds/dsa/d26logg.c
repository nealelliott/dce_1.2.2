/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26logg.c,v $
 * Revision 1.1.12.2  1996/02/18  19:45:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:47  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:45:42  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/mothra_work/1  1995/07/07  22:59 UTC  dtruong
 * 	Correct uninitialize variable
 * 	[1995/12/08  15:07:11  root]
 * 
 * Revision 1.1.10.5  1994/10/05  15:39:11  keutel
 * 	OT 12481: multiple routing
 * 	[1994/10/05  14:36:36  keutel]
 * 
 * Revision 1.1.10.4  1994/08/11  07:58:07  marrek
 * 	Fix alignment problem (ot11618).
 * 	[1994/08/10  14:54:36  marrek]
 * 
 * Revision 1.1.10.3  1994/05/10  15:52:23  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:22:31  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  15:09:27  keutel
 * 	March 1994 code drop
 * 	[1994/03/22  14:14:01  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  16:06:04  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:14:21  marrek]
 * 
 * Revision 1.1.8.1  1993/11/08  15:57:58  keutel
 * 	CR 9390
 * 	[1993/11/08  15:55:05  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  14:17:02  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:12:07  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:41:01  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:20:02  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:46:32  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:13:12  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:50:12  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:56:30  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26logg.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 19:45:31 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*								      */
/* TYPE         : MODULE                                              */
/*								      */
/* NAME         : d26logg.c                                           */
/*								      */
/* AUTHOR       : Volpers, D AP 11                                    */
/* DATE         : 15.02.89                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*								      */
/* DESCRIPTION  : This module contains the function for the logging   */
/*                in the DSA.                                         */
/*								      */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

/*****  Operating-System-Includes  *****/

#include <d26dsa.h>

#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

/*****  external Includes    *****/

/*****  internal Includes    *****/

#include <dce/d27util.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <d26schema.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

#define D26_UPDATE_MSG_LEN      8000

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 D E C L A R A T I O N S        DATA                */
/*								      */
/**********************************************************************/

/******** LOCAL  ********/

static char *file_format = "%s%s";  /* sprints format to build        */
				    /* filename of shdlog file        */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

/******** IMPORT ********/

signed32 d26_m13_open_shdlog(void);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d26_m12_logg_message                                  */
/*								      */
/* AUTHOR:      Volpers, D AP 11                                      */
/* DATE:        15.02.89                                              */
/*								      */
/* DESCRIPTION: This function make the logging for the DSA            */
/*								      */
/* RETURNVALUE:                                                       */
/*     signed32 D2_NO_ERR     no error occurred                       */
/*                                                                    */
/*              D26_ERROR     any error occurred                      */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_m12_logg_message(
    byte     *request,  /* IN    -  request message for mod. operation*/
    signed32 req_len,   /* IN    -  length of request message         */
    signed16 op_id,     /* IN    -  operation ID                      */
    boolean  dnl_upd,   /* IN    -  indicates update of DN-list attr. */
    boolean  add_mk)    /* IN    -  indicates whether master knowledge*/
			/*          attribute must be appended        */

{                               /*  d26_m12_logg_message              */

  static time_t timestamp = (time_t)0L;             /* last timestamp */

#ifdef DPX2200
  static struct flock    arg = { F_WRLCK, SEEK_SET, 0L, 0L, 0, 0 };
#else
  static struct flock    arg = { F_WRLCK, SEEK_SET, 0L, 0L, 0 };
#endif

	/* variables to append master-knowledge attribute information */
			/* length of master-knowledge attribute       */
  static signed32 mk_len = -1L;
			/* master-knowledge attribute information to  */
			/* append to the given message                */
  static byte     mk_val[D23_S_ATT + D2_ALIGN(sizeof(D2_MASTER_KNOWLEDGE))
		  + D23_S_AV + D23_S_ATV + D2_ALIGN(D2_DNL_MAX) + D23_S_EOM];

  signed32 return_value = D2_NO_ERR;
  int      file_no;
  time_t   new_timestamp;
  boolean  unlock_file = FALSE;

				    /* message blocks for DS_SEARCH   */
  byte     *sm_in;
  byte     *chaining_arguments;

  signed16 len;
  signed16 off;

  register signed32     chlen;           /* Chaining arg length       */
  register signed32     tot_msg_len = 0; /* total message length      */
				    /* message length to logg in APDU */
  register signed32     logg_msg_len =0;
#ifndef __hpux_13317CHFts
				    /* pointer to ipc message block   */
  register byte         *msg_ptr;
#else
				    /* pointer to ipc message block   */
  register byte         *msg_ptr = sm_in;

#endif
  register D23_at_type  *at_type;
  register D23_av_value *av_value;
#ifndef DAEMON
  signed32 object_cl[D26_OCL_VAL_COUNT + 1];
  signed16 sup_len;                 /* length of superior name        */
  byte     *sm_out;
  signed32 out_len = D26_UPDATE_MSG_LEN;
  signed32 remlen;
				    /* parameters for DS_SEARCH       */
				    /* exact  dereferenced object     */
  byte     exact_obj[D2_DNL_MAX + 1];
  D26_attr_info attr[D2_AT_MAX + 1];
  D26_acl  acl_sup[D2_ACLIDX_MAX];
  boolean  rec;
  char     *record;
  boolean  al_flag;
  signed16 ob_nr;
  register D23_ds_name  *ds_name;
  register D23_rq_info  *rq_info;
#endif

  DCE_SVC_LOG((GDS_S_ENTRY_SHUPD_LOG_MSG));

#ifndef DAEMON
/************************************/
/*  get mk index of superior object */
/************************************/
  if                            /* d26_sup_mk not given               */
    (op_id != D23_ADDOBJT && dnl_upd == FALSE)
  {                             /*  allocate memory for DS_SEARCH     */
				/*  invoke msg                        */
    if                          /*  no more memory available          */
      ((sm_out = (byte *)D27_MALLOC(svc_c_sev_warning,D26_UPDATE_MSG_LEN))
       == NULL)
    {                           /*  MA_LOG: error no memory available */
      return (D26_ERROR);
    }
    else if                     /*  no more memory available          */
      ((sm_in = (byte *)D27_MALLOC(svc_c_sev_warning,D26_UPDATE_MSG_LEN))
       == NULL)
    {
      free(sm_out);
      return (D26_ERROR);
    }
    else
    {
      msg_ptr = sm_in;
				/*  calculate length of superior      */
      sup_len = d26_u08_calc_len (d26_objname, 1);

				/* set parametrs for internal search  */
      ((D23_header *)msg_ptr)->d23_versno = D23_APDUV22;
      ((D23_header *)msg_ptr)->d23_dirid  = d26_dir_id;
      msg_ptr += D23_S_INSRCH;

      ((D23_com_arg *) msg_ptr )->d23_cmtag = D23_CMTAG;
      ((D23_com_arg *) msg_ptr )->d23_cmctr.d23_scopt =
	D2_DREFALIAS | D2_LOCALSCOPE;
      ((D23_com_arg *) msg_ptr )->d23_cmctr.d23_scpri = 0;
      ((D23_com_arg *) msg_ptr )->d23_cmctr.d23_sctlm = D2_T_UNLIMITED;
      ((D23_com_arg *) msg_ptr )->d23_cmctr.d23_scslm = 1;
      ((D23_com_arg *) msg_ptr )->d23_cmopr.d23_opnm_rs = D2_NOT_STARTED;
      ((D23_com_arg *) msg_ptr )->d23_cmopr.d23_op_nxtrdn = 0;
      msg_ptr += D23_S_CARG;

      ((D23_srcharg *) msg_ptr )->d23_Itag = D23_ITAG;
      ((D23_srcharg *) msg_ptr )->d23_Iali = FALSE;;
      ((D23_srcharg *) msg_ptr )->d23_Iobjsub = D2_BASE_OBJECT;
      msg_ptr += D23_S_SCHARG;

      ds_name = (D23_ds_name *)msg_ptr;
      ds_name->d23_dstag = D23_NAMTAG;
      ds_name->d23_dscomplete = TRUE;
      msg_ptr += sizeof(D23_ds_name);
      if                        /* superior == ROOT                   */
	(sup_len == 0)
      {                         /* superior object = "SCHEME"         */
	strcpy((char *)msg_ptr, D2_NAME_SCHEME);
	ds_name->d23_dslen = strlen((char *)msg_ptr) + 1;
	ds_name->d23_dsoff = D2_ALIGN(ds_name->d23_dslen);
      }
      else
      {                         /* copy name in SEARCH-message        */
	memcpy(msg_ptr,d26_objname,sup_len);
	msg_ptr[sup_len] = D2_EOS;
	ds_name->d23_dslen = sup_len + 1;
	ds_name->d23_dsoff = D2_ALIGN(ds_name->d23_dslen);
      }
				/* put requested info (NONE) to mess. */
      rq_info = (D23_rq_info *)(msg_ptr + ds_name->d23_dsoff);
      rq_info->d23_rqtag = D23_REQTAG;
      rq_info->d23_rqreq = D2_R_TYP;
      rq_info->d23_rqnbr = 0;
				/* put EOM to message                 */
      ((D23_eom *)(rq_info + 1))->d23_eomtag = D23_EOMTAG;
				/* call internal search               */
      if                        /* search was not successful          */
	((d26_u00_search(FALSE,D23_MODENTR,sm_in,&out_len,&sm_out,&remlen,
	 exact_obj,attr,object_cl,acl_sup,&d26_sup_mk,&rec,&record,&al_flag,
	 &ob_nr)) != D2_NO_ERR)
      {                         /* error log                          */
	return_value = D26_ERROR;
      }                         /* search was not successfull         */
      free(sm_in);
      free(sm_out);
      d26_i14_unlock();         /* unlock / close used files          */
    }
  }
#endif

  if                            /* no error occurred                  */
    (return_value == D2_NO_ERR)
  {                             /* get unique time stamp              */
    while                       /* time stamp not changed             */
	 ((new_timestamp = time((time_t *) 0L)) <= timestamp)
    {                           /* sleep 1 second                     */
      sleep (1);
    }

  /**************/
  /*  open file */
  /**************/
    if                          /* shadow logfile cannot be opened    */
      (d26_m13_open_shdlog () != D2_NO_ERR)
    {                           /* return error                       */
      return_value = D26_ERROR;
    }
    else
    {                           /* get filenumber                     */
      file_no = fileno(d26_fpshdlog);
      arg.l_type = F_WRLCK ;
      if                        /* file cannot be locked              */
	(fcntl(file_no,F_SETLKW,&arg) == (-1))
      {                         /* set return value                   */
	dce_svc_printf(GDS_S_FCNTL_ERR_MSG,F_SETLKW,errno);
	return_value = D26_ERROR;
      }
      else
      {                         /* write logg file                    */
	unlock_file = TRUE;
				/* create Chaining-Arguments          */
        if                      /*  no more memory available          */
          ((chaining_arguments = (byte *)D27_MALLOC(svc_c_sev_warning,
		D23_S_HDR + D23_S_CHARG + D23_S_TRACE + D2_DNL_MAX + 
		D2_DNL_MAX)) == NULL)
        {
          return (D26_ERROR);
        }
	msg_ptr = chaining_arguments;
	((D23_header *)msg_ptr)->d23_versno = D23_APDUV22;
	((D23_header *)msg_ptr)->d23_dirid  = d26_dir_id;
	msg_ptr += sizeof(D23_header);
	((D23_chain_arg *)msg_ptr)->d23_chtag    = D23_CHTAG;
	((D23_chain_arg *)msg_ptr)->d23_al_deref = FALSE;
	((D23_chain_arg *)msg_ptr)->d23_al_RDN   = 0;
	((D23_chain_arg *)msg_ptr)->d23_ret_cref = FALSE;
	((D23_chain_arg *)msg_ptr)->d23_ref_type = D2_SUBORDINATE;
	((D23_chain_arg *)msg_ptr)->d23_chtmlt   = D2_T_UNLIMITED;
	((D23_chain_arg *)msg_ptr)->d23_op_prog.d23_opnm_rs = D2_COMPLETED;
	((D23_chain_arg *)msg_ptr)->d23_op_prog.d23_op_nxtrdn = 0;
	((D23_chain_arg *)msg_ptr)->d23_max_tr   = 1;
	msg_ptr                                 += D23_S_CHARG;
	((D23_trace_info *)msg_ptr)->d23_trtag   = D23_TRTAG;
	((D23_trace_info *)msg_ptr)->d23_op_prog.d23_opnm_rs = D2_NOT_STARTED;
	((D23_trace_info *)msg_ptr)->d23_op_prog.d23_op_nxtrdn = 0;
	msg_ptr                                 += D23_S_TRACE;
	((D23_ds_name *)msg_ptr)->d23_dstag      = D23_NAMTAG;
	((D23_ds_name *)msg_ptr)->d23_dscomplete = TRUE;
	len                         = strlen((char *)d26_myname) + 1;
	off                         = D2_ALIGN(len);
	((D23_ds_name *)msg_ptr)->d23_dslen      = len;
	((D23_ds_name *)msg_ptr)->d23_dsoff      = off;
	msg_ptr                                 += sizeof(D23_ds_name);
	strcpy((char *)msg_ptr,(char *)d26_myname);
	msg_ptr                                 += off;
	((D23_ds_name *)msg_ptr)->d23_dstag      = D23_RQTAG;
	((D23_ds_name *)msg_ptr)->d23_dscomplete = TRUE;
	if                      /* requestor is not anonymous         */
	  (strcmp(d26_requestor.d26_req_name,D26_ANONYMOUS))
	{                       /* copy requestor name to message     */
	  len = strlen(d26_requestor.d26_req_name) + 1;
	  off = D2_ALIGN(len);
	  ((D23_ds_name *)msg_ptr)->d23_dslen = len;
	  ((D23_ds_name *)msg_ptr)->d23_dsoff = off;
	  msg_ptr+= sizeof(D23_ds_name);
	  strcpy((char *)msg_ptr,d26_requestor.d26_req_name);
	  msg_ptr+= off;
	}
	else
	{                       /* set requestor length 0             */
	  ((D23_ds_name *)msg_ptr)->d23_dslen = 0;
	  ((D23_ds_name *)msg_ptr)->d23_dsoff = 0;
	  msg_ptr+= sizeof(D23_ds_name);
	}
	tot_msg_len = chlen = msg_ptr - chaining_arguments;
	tot_msg_len += logg_msg_len = req_len - sizeof(D23_eom);
	if                      /* master-knowledge must be appended  */
	  (add_mk == TRUE)
	{                       /* check whether master-knowledge     */
				/* is created already                 */
	  if                    /* masterknowledge not yet created    */
	    (mk_len < 0L)
	  {                     /* create master-knowledge attribute  */
	    at_type = (D23_at_type *) mk_val;
	    at_type->d23_atttag = D23_ATTTAG;
	    at_type->d23_atlen = (signed16)(sizeof(D2_MASTER_KNOWLEDGE)-1);
	    at_type->d23_atoff = (signed16)(D2_ALIGN (at_type->d23_atlen));
	    msg_ptr = (byte *)(at_type + 1);
	    memcpy(msg_ptr,D2_MASTER_KNOWLEDGE,at_type->d23_atlen);
	    msg_ptr += at_type->d23_atoff;
	    ((D23_av_info *)msg_ptr)->d23_avtag = D23_AVTAG;
	    ((D23_av_info *)msg_ptr)->d23_avnum = 1;
	    ((D23_av_info *)msg_ptr)->d23_asyntx = D2_DISTNAME;
	    msg_ptr += sizeof(D23_av_info);
	    av_value = (D23_av_value *)msg_ptr;
	    av_value->d23_avlen = (signed16)(strlen((char *)d26_myname) + 1);
	    av_value->d23_avoff = D2_ALIGN(strlen((char *)d26_myname) + 1);
	    av_value->d23_avrep = D2_DISTNAME;
	    msg_ptr += sizeof(D23_av_value);
	    memcpy(msg_ptr,d26_myname,av_value->d23_avlen);
	    msg_ptr += av_value->d23_avoff;
	    ((D23_eom *)msg_ptr)->d23_eomtag = D23_EOMTAG;
	    msg_ptr += sizeof(D23_eom);
	    mk_len = msg_ptr - mk_val;
	  }
				/* reset total message length         */
	  tot_msg_len  += mk_len;
	}
	else
	{                       /* reset message lengths              */
	  tot_msg_len  += sizeof(D23_eom);
	  logg_msg_len += sizeof(D23_eom);
	}

				/* write file                         */
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_warning,d26_fpshdlog,
	  D2_WSHDLOG_FORM,d26_objname,d26_sup_mk,(long)new_timestamp,d27_pid,
	  op_id,tot_msg_len);
	D27_FWRITE(svc_c_sev_warning,chaining_arguments,(size_t)chlen,1,
	  d26_fpshdlog);
	D27_FWRITE(svc_c_sev_warning,request,(size_t)logg_msg_len,1,
	  d26_fpshdlog);
	if                      /*  master knowledge must be appended */
	  (add_mk == TRUE)
	{                       /*  do it                             */
	  D27_FWRITE(svc_c_sev_warning,mk_val,(size_t)mk_len,1,d26_fpshdlog);
	}
	D27_FFLUSH(svc_c_sev_warning,d26_fpshdlog);
	timestamp = new_timestamp;
	free(chaining_arguments);
      }
    }
  }

/****************/
/*  unlock file */
/****************/
  if                            /* file must be unlocked              */
    (unlock_file == TRUE)
  {                             /* do it                              */
    arg.l_type = F_UNLCK ;
    if                          /* file cannot be unlocked            */
      (fcntl(file_no,F_SETLKW,&arg) == (-1))
    {                           /* set error code                     */
      dce_svc_printf(GDS_S_FCNTL_ERR_MSG,F_SETLKW,errno);
      return_value = D26_ERROR;
    }
  }

/***************************/
/*  reset global variables */
/***************************/
  d26_sup_mk = D26_IDX_NOT_DEF; /* mk index of superior               */

  DCE_SVC_LOG((GDS_S_EXIT_SHUPD_LOG_MSG));
  return(return_value);

}                               /* d26_m12_logg_message               */

/*exon ******************************  **********************************/
/*                                                                      */
/* TYPE:        C-FUNCTION                                              */
/*                                                                      */
/* NAME  :      d26_m13_open_shdlog                                     */
/*                                                                      */
/* AUTHOR:      Schreck, D AP 11                                        */
/* DATE:        22.03.89                                                */
/*                                                                      */
/* DESCRIPTION: This function opens t  he D2_SHDLOG file (file for      */
/*              logging update operat  ions).                           */
/*                                                                      */
/* RETURNVALUE:                                                         */
/*     Int      D2_NO_ERR     no erro  r occurred                       */
/*                                                                      */
/*              D26_ERROR     any err  or occurred                      */
/*                                                                      */
/*exoff *****************************  **********************************/

signed32 d26_m13_open_shdlog(void)

{                               /*  d26_m13_open_shdlog               */

  signed32    return_value = D2_NO_ERR;
  char        filename[D27_LFILE_NAME];

  if (d26_fpshdlog == (FILE *)NULL)     /* file is closed             */
  {
			      /* give filname of logg-file            */
    sprintf(filename, file_format,d26_install, D2_SHDLOG);
    if                        /* open of logg file                    */
      ((d26_fpshdlog = D27_FOPEN(svc_c_sev_warning,filename,"a"))
       == (FILE *)NULL)
    {                         /* set error values                     */
	  return_value = D26_ERROR;
    }
  }

  return(return_value);

}                               /*  d26_m13_open_shdlog               */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d26_m14_close_shdlog                                  */
/*								      */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        22.03.89                                              */
/*								      */
/* DESCRIPTION: This function closes the D2_SHDLOG file (file for     */
/*              logging update operations).                           */
/*								      */
/* RETURNVALUE:                                                       */
/*     signed32 D2_NO_ERR     no error occurred                       */
/*                                                                    */
/*              D26_ERROR     any error occurred                      */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_m14_close_shdlog(void)

{                               /*  d26_m14_close_shdlog              */

  if                    /* shadow logfile is opened                   */
    (d26_fpshdlog != NULL)
  {                     /* close shadow logfile                       */
    if                  /* shadow logfile could be closed             */
      ((D27_FCLOSE(svc_c_sev_warning,d26_fpshdlog)) == 0)
    {                   /* set shadow log file pointer to NULL        */
      d26_fpshdlog = NULL;
      return(D2_NO_ERR);
    }
    else
    {                   /* return error                               */
      return(D26_ERROR);
    }
  }
  else
  {
    return(D2_NO_ERR);
  }

}                               /*  d26_m14_close_shdlog              */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d26_m21_logg_dnlist()                                 */
/*								      */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        21.11.89                                              */
/*								      */
/* DESCRIPTION: This function prepares the logging for the            */
/*              modification of the DN-List-Attribute.                */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*   D2_attr_mod    attr_mod      modification-type of DN-List attr.  */
/*   D2_name_string old_dn        old attribute value                 */
/*   D2_name_string new_dn        new attribute value                 */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                none                                                */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/*exoff ***************************************************************/

void d26_m21_logg_dnlist(D2_attr_mod attr_mod, D2_name_string old_dn,
    D2_name_string new_dn)

{                               /*  d26_m21_logg_dnlist               */

  char objname[D2_DNL_MAX + 1];     /* object name of changed object  */
  signed32 sup_mk;                  /* mk index of superior           */
  signed16 offset;                  /* message offset                 */

  byte mod_in[D26_UPDATE_MSG_LEN ]; /* message block  for DS_MODIFY   */
  signed32 length;
  D23_av_value *av_value;

  register byte *message;            /* message pointer                */

/*  save global variables */
  strcpy(objname,(char *)d26_objname);
  sup_mk = d26_sup_mk;

  strcpy((char *)d26_objname,(char *)d26_myname);
  d26_sup_mk = D26_OWNNAME;

  message = mod_in;
  ((D23_com_arg *) message)->d23_cmtag = D23_CMTAG;
  ((D23_com_arg *) message)->d23_cmctr.d23_scopt =
			       D2_DREFALIAS | D2_LOCALSCOPE;
  ((D23_com_arg *) message)->d23_cmctr.d23_scpri = D2_PRIO_MEDIUM;
  ((D23_com_arg *) message)->d23_cmctr.d23_sctlm = D2_T_UNLIMITED;
  ((D23_com_arg *) message)->d23_cmctr.d23_scslm = D2_S_UNLIMITED;
  ((D23_com_arg *) message)->d23_cmopr.d23_opnm_rs = D2_NOT_STARTED;
  ((D23_com_arg *) message)->d23_cmopr.d23_op_nxtrdn = 0;
  message += D23_S_CARG;
  ((D23_ds_name *)message)->d23_dstag = D23_NAMTAG;
  ((D23_ds_name *)message)->d23_dscomplete = TRUE;
  ((D23_ds_name *)message)->d23_dslen = strlen((char *)d26_objname) + 1;
  ((D23_ds_name *)message)->d23_dsoff = offset =
			  D2_ALIGN(((D23_ds_name *)message)->d23_dslen);
  message += sizeof(D23_ds_name);
  strcpy((char *)message,(char *)d26_objname);
  message += offset;
  ((D23_md_info *)message)->d23_mdtag = D23_MODTAG;
  ((D23_md_info *)message)->d23_mdmod = attr_mod;
  message += sizeof(D23_md_info);
  ((D23_at_type *)message)->d23_atttag = D23_ATTTAG;
  ((D23_at_type *)message)->d23_atlen = sizeof(D2_DNLIST)-1;
  ((D23_at_type *)message)->d23_atoff = D2_ALIGN(sizeof(D2_DNLIST) -1);
  message += sizeof(D23_at_type);
  memcpy(message, D2_DNLIST, sizeof(D2_DNLIST)-1);
  message += D2_ALIGN(sizeof(D2_DNLIST)-1);
  if                    /*  old value is given                        */
    (attr_mod == D2_REP_AV )
  {                     /*  write old value into message              */
    ((D23_av_info *)message)->d23_avtag = D23_AVTAG;
    ((D23_av_info *)message)->d23_avnum = 2;
    ((D23_av_info *)message)->d23_asyntx = D2_DISTNAME;
    message += sizeof(D23_av_info);
    av_value = (D23_av_value *)message;
    av_value->d23_avlen = (signed16)(strlen((char *)old_dn)+1);
    av_value->d23_avoff = D2_ALIGN(strlen((char *)old_dn)+1);
    av_value->d23_avrep = D2_DISTNAME;
    message += sizeof(D23_av_value);
    memcpy(message,old_dn,av_value->d23_avlen);
    message += av_value->d23_avoff;
    av_value = (D23_av_value *)message;
    av_value->d23_avlen = (signed16)(strlen((char *)new_dn)+1);
    av_value->d23_avoff = D2_ALIGN(strlen((char *)new_dn)+1);
    av_value->d23_avrep = D2_DISTNAME;
    message += sizeof(D23_av_value);
    memcpy(message,new_dn,av_value->d23_avlen);
    message += av_value->d23_avoff;
  }
  else if (attr_mod != D2_DELATT)
  {
    ((D23_av_info *)message)->d23_avtag = D23_AVTAG;
    ((D23_av_info *)message)->d23_avnum = 1;
    ((D23_av_info *)message)->d23_asyntx = D2_DISTNAME;
    message += sizeof(D23_av_info);
    av_value = (D23_av_value *)message;
    av_value->d23_avrep = D2_DISTNAME;
    if (attr_mod == D2_DEL_AV)
    {
      av_value->d23_avlen = (signed16)(strlen((char *)old_dn)+1);
      av_value->d23_avoff = D2_ALIGN(strlen((char *)old_dn)+1);
      message += sizeof(D23_av_value);
      memcpy(message,old_dn,av_value->d23_avlen);
      message += av_value->d23_avoff;
    }
    else
    {
      av_value->d23_avlen = (signed16)(strlen((char *)new_dn)+1);
      av_value->d23_avoff = D2_ALIGN(strlen((char *)new_dn)+1);
      message += sizeof(D23_av_value);
      memcpy(message,new_dn,av_value->d23_avlen);
      message += av_value->d23_avoff;
    }
  }
  ((D23_eom *)message)->d23_eomtag = D23_EOMTAG;
  message += sizeof(D23_eom);
  length = message - mod_in;

  d26_m12_logg_message(mod_in,length,D23_MODENTR,TRUE,FALSE);

  strcpy((char *)d26_objname,objname);
  d26_sup_mk = sup_mk;

}                                /*  d26_m21_logg_dnlist              */
