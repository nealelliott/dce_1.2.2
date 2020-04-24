/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d25cup.c,v $
 * Revision 1.1.10.2  1996/02/18  18:23:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:46:09  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:15:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:20:42  root]
 * 
 * Revision 1.1.8.4  1994/09/06  12:26:25  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:24:24  keutel]
 * 
 * Revision 1.1.8.3  1994/06/21  14:48:12  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  11:39:50  marrek]
 * 
 * Revision 1.1.8.2  1994/05/10  16:03:24  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:12:21  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  17:55:10  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:51:16  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  08:23:13  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:18:35  marrek]
 * 
 * Revision 1.1.4.4  1993/01/28  14:58:55  marrek
 * 	Wrong buffer is written in cache update for compare.
 * 	[1993/01/28  12:15:34  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  22:03:38  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:47:41  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  18:27:50  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  15:08:12  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:08:46  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:30:11  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d25cup.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:23:12 $";
#endif

/* ****************************************************************** */
/*                                                                    */
/*   * COPYRIGHT   (C)  1989 BY SIEMENS AG                            */
/*   * All rights reserved                                            */
/*   *                                                                */
/*   * This software is furnished under licence and may be used only  */
/*   * in accordance with the terms of that licence and with the      */
/*   * inclusion of the above copyright notice.                       */
/*   * This software may not be provided or otherwise made available  */
/*   * to, or used by, any other person. No title to or ownership of  */
/*   * the software is hereby transferred.                            */
/*                                                                    */
/* ****************************************************************** */

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : MODUL                                               */
/*                                                                    */
/* NAME         : d25cup.c    [d25cup]                                */
/*                                                                    */
/* AUTOR        : E. Kraemer, D AP 11                                 */
/* DATUM        : 10.07.91                                            */
/*                                                                    */
/* KOMPONENTE   : CACHE update                                        */
/*                                                                    */
/* DOK.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* BESCHREIBUNG : the module contains the functions, which are used   */
/*                by client stub to update the cache                  */
/*                                                                    */
/* SYSTEMABHAENGIGKEIT:                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORIE     :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 10.07.91| birth                          | ek |         */
/*                                                                    */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <gds.h>

/*****  Operating-System-Includes  *****/

#include <stdlib.h>

/*****  external Includes    *****/

#include <ros.h>

#include <dce/d27util.h>
#include <d23ipc.h>
#include <d25stub.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D25_CUP_JOBS_NO 10      /* number of entries in the cache     */
				/* update job table                   */
#define D25_CUP_BLOCK_LEN 4096  /* Block length for cache update      */
				/* messages                           */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

typedef struct
	    {
		signed32        ipc_ass_id; /* assigned IPC-ass.      */
		signed32        job_number; /* number of job          */
		signed16        dir_id;     /* directory ID           */
		R_int           op_id;      /* operation ID           */
		boolean         terminate;  /* indicates termination  */
		D23_serv_ctrl   serv_ctrl;  /* service controls       */
		D2_name_string  base_object;/* base object of oprtn.  */
		char *          result;     /* result of remote oprtn.*/
		char *          work;       /* actual pointer to work */
	    } D25_cup_job;

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

				    /* indicates an aborted cache     */
				    /* connection                     */
static   boolean d25_cache_aborted = FALSE;

				    /* maximum cache update job number*/
static   signed32 max_job_number = 1L;

				/* cache update job table             */
static   D25_cup_job d25_cup_job[D25_CUP_JOBS_NO] =
	    {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};

			/* pointer to attribute type structure in     */
			/* client message                             */
static   byte *      d25_cl_type;

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

static D25_cup_job  *d25_102_get_cup_job(signed32 ass_id);
static D25_cup_job  *d25_103_active_job(void);
static signed16     d25_122_find_tag(R_int op_id, D25_cup_job *cup_job);
static signed16     d25_123_add_comp_en(D25_cup_job *cup_job,
			byte **cmessage, signed32 *clen);
static signed16     d25_124_add_en(D25_cup_job *cup_job,
			byte **cmessage, signed32 *clen);
static byte         *d25_125_write_add_head(D25_cup_job *cup_job,
			byte **cmessage, byte *message, signed32 *memlen,
			signed32 *remlen);
static byte         *d25_126_check_buffer(signed16 len, signed32 *remlen,
			signed32 *memlen, byte **cmessage, byte *message);
static byte         *d25_128_write_add_dn(D25_cup_job *cup_job,
			byte **cmessage, byte *message, signed32 *memlen,
			signed32 *remlen, boolean *ignore);
static byte         *d25_129_write_add_type(D25_cup_job *cup_job,
			byte **cmessage, byte *message, signed32 *memlen,
			signed32 *remlen, boolean *ignore);
static byte         *d25_130_write_add_avinfo(D25_cup_job *cup_job,
			byte **cmessage, byte *message, signed32 *memlen,
			signed32 *remlen, boolean ignore);
static byte         *d25_131_write_add_avvalue(D25_cup_job *cup_job,
			byte **cmessage, byte *message, signed32 *memlen,
			signed32 *remlen, boolean ignore);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_101_save_invoke_data                            */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 10.07.91                                            */
/*                                                                    */
/* DESCRIPTION  : The Function scans the IPC-request of a retrieval   */
/*                function and writes the parameters, that are needed */
/*                for a following cache update operation into the     */
/*                CUP-job table.                                      */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D25_ACONT                       to continue execution at next */
/*                                      entry                         */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d25_101_save_invoke_data(
			/* INOUT  - Parameter block used for all stub */
    D25_actionpb *action_pb)        /* operations                     */
{                /*  d25_101_save_invoke_data                         */
/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* AUTO  ...  *******/

  R_int      op_id;         /* operation identifier                   */
  byte       *cmessage;     /* pointer to cache update message        */
  byte       *dmessage;     /* pointer to dummy message               */
  signed16   dir_id;        /* directory ID                           */
  D23_serv_ctrl *serv_cntrl;/* common arguments                       */
  D2_name_string base_object;   /* base object of operation           */
  signed16   offset;        /* offset to next APDU                    */
  D25_cup_job *cup_job;     /* pointer to cache update job            */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

  op_id = action_pb->d25_aipcref->d25_ioprid;
  op_id = op_id > D2_ADM_OPID ? op_id - D2_ADM_OPID : op_id;

  if            /*  cache connection is aborted or update function is */
		/*  calling                                           */
    (d25_cache_aborted == TRUE ||
     op_id != D23_READ && op_id != D23_COMPARE && op_id != D23_LIST &&
     op_id != D23_SEARCH)
  {             /*  return immediately                                */
    DCE_SVC_LOG((GDS_S_NOCUP_UPDATEFUNC_MSG));
    return(D25_ACONT);
  }
  else
  {             /*  scan request                                      */
    cmessage   = action_pb->d25_aipcref->d25_iudat.d25_datref;
    dir_id     = ((D23_header *)cmessage)->d23_dirid;
    cmessage  += sizeof(D23_header);
    serv_cntrl = &((D23_com_arg *)cmessage)->d23_cmctr;

    if          /*  no cache update required                          */
      ((serv_cntrl->d23_scopt & D2_DONT_STORE) != 0)
    {           /*  return immediately                                */
      DCE_SVC_LOG((GDS_S_NOCUP_REQUIRED_MSG));
      return(D25_ACONT);
    }
    else
    {           /*  proceed scanning the request                      */
      dmessage = cmessage;
      d27_501_copy_common_arguments(&cmessage,&dmessage);
      if        /*  search arguments are following                    */
	(((D23_srcharg *)cmessage)->d23_Itag == D23_ITAG)
      {         /*  just skip them                                    */
	cmessage += sizeof(D23_srcharg);
      }         /*                                                    */
      offset      = ((D23_ds_name *)cmessage)->d23_dsoff;
      cmessage   += sizeof(D23_ds_name);
      base_object = (D2_name_string)cmessage;
      cmessage   += offset;

      if        /*  READ or SEARCH are performed                      */
	(op_id == D23_READ || op_id == D23_SEARCH)
      {         /*  check requested attributes                        */
	if      /*  all attribute values are requested                */
	  (((D23_rq_info *)cmessage)->d23_rqnbr == D2_ALL_ATT_REQ &&
	   ((D23_rq_info *)cmessage)->d23_rqreq == D2_R_VAL_TYP)
	{       /*  return immediately                                */
	  DCE_SVC_LOG((GDS_S_NOCUP_ALL_ATTR_MSG));
	  return(D25_ACONT);
	}       /*                                                    */
      }         /*                                                    */

      if        /*  no free cache update job available                */
	((cup_job = d25_102_get_cup_job(D2_NOT_SPEC))
	 == (D25_cup_job *)NULL)
      {         /*  return immediately                                */
	dce_svc_printf(GDS_S_CUPJOB_QUEUE_FULL_MSG);
	return(D25_ACONT);
      }
      else if   /*  no more memory for base object                    */
	     ((cup_job->base_object =
	      (D2_name_string)D27_MALLOC(svc_c_sev_warning,D2_DNL_MAX + 1))
	      == NULL)
      {         /*  return immediately                                */
	return(D25_ACONT);
      }
      else
      {         /*  fill cache update job                             */
	DCE_SVC_LOG((GDS_S_CUPREQ_ENTERED_MSG,
	  action_pb->d25_aipcref->d25_iassid,strlen((char *)base_object) + 1,
	  base_object));
	cup_job->ipc_ass_id = action_pb->d25_aipcref->d25_iassid;
	cup_job->job_number = max_job_number++;
	cup_job->dir_id     = dir_id;
	cup_job->op_id      = op_id;
	cup_job->terminate  = FALSE;
	cup_job->serv_ctrl  = *serv_cntrl;
	strcpy((char *)cup_job->base_object,(char *)base_object);
      }
    }
  }

  return(D25_ACONT);

}               /*  d25_101_save_invoke_data                          */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_102_get_cup_job                                 */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 11.07.91                                            */
/*                                                                    */
/* DESCRIPTION  : The Function searches the cache update job table    */
/*                for the entry containing the CUP-job assigned to the*/
/*                association ID and returns a pointer to it.         */
/*                It returns the NULL-pointer when the entry cannot   */
/*                be found. To search for a free entry, pass          */
/*                D2_NOT_SPEC as association ID.                      */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D25_cup_job *   pointer to free cache update job table entry  */
/*                                                                    */
/*inoff ***************************************************************/

static D25_cup_job *d25_102_get_cup_job(
    signed32 ass_id)    /* IN    -  association ID                    */
{                /*  d25_102_get_cup_job                              */
/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

  register signed16    i;           /* loop variable                  */
  register D25_cup_job *cup_job;    /* pointer to job table           */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

  for               /*  all cache update job table entries            */
     (i = 0, cup_job = d25_cup_job; i < D25_CUP_JOBS_NO; i++, cup_job++)
  {                 /*  check whether entry is used                   */
    if              /*  entry is free                                 */
      (cup_job->ipc_ass_id == ass_id && cup_job->result == NULL)
    {               /*  return that pointer                           */
      return(cup_job);
    }               /*                                                */
  }                 /*                                                */

  return(NULL);

}               /*  d25_102_get_cup_job                               */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_103_active_job                                  */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 11.07.91                                            */
/*                                                                    */
/* DESCRIPTION  : The Function searches the cache update job table    */
/*                for the active CUP-job with the smallest job number.*/
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D25_cup_job *   pointer to active job table entry             */
/*                                                                    */
/*inoff ***************************************************************/

static D25_cup_job *d25_103_active_job(void)

{                /*  d25_103_active_job                               */

  signed32 min_job_number = 0L;     /* smallest jobnumber             */
				    /* CUP-job to return              */
  D25_cup_job *ret_job    = (D25_cup_job *)NULL;

/* FASTAUTO  ...    */

  register signed16    i;           /* loop variable                  */
  register D25_cup_job *cup_job;    /* pointer to job table           */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

  for               /*  all cache update job table entries            */
     (i = 0, cup_job = d25_cup_job; i < D25_CUP_JOBS_NO; i++, cup_job++)
  {                 /*  check whether entry is used                   */
    if              /*  job is active                                 */
      (cup_job->result != NULL)
    {               /*  check minimum job number                      */
      if            /*  minimum job number is still 0 or greater than */
		    /*  job number                                    */
	(min_job_number == 0L || cup_job->job_number < min_job_number)
      {             /*  reset it                                      */
	min_job_number = cup_job->job_number;
	ret_job        = cup_job;
      }             /*                                                */
    }               /*                                                */
  }                 /*                                                */

  return(ret_job);

}               /*  d25_103_active_job                                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_111_remove_cup_job                              */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 10.07.91                                            */
/*                                                                    */
/* DESCRIPTION  : The Function removes the cache update job, that is  */
/*                assigned to an IPC-association. If it recognizes    */
/*                the cache being aborted, it removes all the cache   */
/*                update jobs from the table and sets the global      */
/*                indicator variable.                                 */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D25_ACONT                       to continue execution at next */
/*                                      entry                         */
/*exoff ***************************************************************/

signed32 d25_111_remove_cup_job(
			/* INOUT  - Parameter block used for all stub */
    D25_actionpb *action_pb)        /* operations                     */
{                /*  d25_111_remove_cup_job                           */

  D25_cup_job *cup_job;     /* pointer to cache update job            */

/* FASTAUTO  ...    */

  register signed16 i;      /* loop variable                          */

  if            /*  cache is aborted                                  */
    (action_pb->d25_astate == D25_ICINVOKE_PENDING &&
     action_pb->d25_aevent == D25_IABORT_EV)
  {             /*  remove all cache update jobs                      */
    for         /*  all cache update jobs                             */
       (i = 0, cup_job = d25_cup_job; i < D25_CUP_JOBS_NO; i++, cup_job++)
    {           /*  reinitialize the association ID                   */
      cup_job->ipc_ass_id = D2_NOT_SPEC;
      cup_job->job_number = 0L;
      free(cup_job->base_object);
      if        /*  result was entered already                        */
	(cup_job->result != NULL)
      {         /*  free the result                                   */
	free(cup_job->result);
	cup_job->result = NULL;
      }         /*                                                    */
    }           /*                                                    */
    DCE_SVC_LOG((GDS_S_ALL_CUPREQ_REMOVED_MSG));
  }
  else if       /*  cache update job exists                           */
	 ((cup_job = d25_102_get_cup_job(action_pb->d25_aipcref->d25_iassid))
	  != NULL)
  {             /*  remove the single cache update job                */
    DCE_SVC_LOG((GDS_S_CUPREQ_REMOVED_MSG,action_pb->d25_aipcref->d25_iassid,
      strlen((char *)cup_job->base_object) + 1,cup_job->base_object));
    cup_job->ipc_ass_id = D2_NOT_SPEC;
    cup_job->job_number = 0L;
    free(cup_job->base_object);
    if          /*  result was entered already                        */
      (cup_job->result != NULL)
    {           /*  free the result                                   */
      free(cup_job->result);
      cup_job->result = NULL;
    }           /*                                                    */
  }             /*                                                    */

  return(D25_ACONT);

}               /*  d25_111_remove_cup_job                            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_121_save_result_data                            */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/*                                                                    */
/* DATE         : 11.07.91                                            */
/*                                                                    */
/* DESCRIPTION  : The Function checks, whether the result is a result */
/*                of a retrieval function or not. If not, it returns  */
/*                immediately. Otherwise it writes the result into the*/
/*                cache update job table.                             */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      signed32    indicates following state to be entered           */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d25_121_save_result_data(
			/* INOUT  - Parameter block used for all stub */
    D25_actionpb *action_pb)        /* operations                     */
{                /*  d25_121_save_result_data                         */
  byte        *add_message; /* pointer to cache update message        */
  signed32    add_memlen;   /* length of cache update message         */
  D25_cup_job *cup_job;     /* pointer to cache update job            */
  boolean     cup_active;   /* indicates an active cache update job   */

  signed32    ret_value;    /* return value                           */

  register    signed16 i;   /* loop variable                          */
  register    D25_ipcentry *ipcptr; /* pointer to IPC table entry     */

  cup_active = d25_103_active_job() == (D25_cup_job *)NULL ? FALSE : TRUE;
  if            /*  cache update job does not exist                   */
    ((cup_job = (D25_cup_job *)d25_102_get_cup_job(
     action_pb->d25_aipcref->d25_iassid)) == (D25_cup_job *)NULL)
  {             /*  return immediately                                */
    return(D25_ANSUC);
  }
  else if       /*  not enough memory to store result in CUP-table    */
	 ((cup_job->result = (char *)D27_MALLOC(svc_c_sev_warning,
	  (unsigned)action_pb->d25_aipcref->d25_iudat.d25_datlen)) == NULL)
  {             /*  remove job and return                             */
    cup_job->ipc_ass_id = D2_NOT_SPEC;
    cup_job->job_number = 0L;
    free(cup_job->base_object);
    return(D25_ANSUC);
  }
  else
  {             /*  copy result into CUP-table                        */
    memcpy(cup_job->result,action_pb->d25_aipcref->d25_iudat.d25_datref,
	   action_pb->d25_aipcref->d25_iudat.d25_datlen);
    if          /*  result not enough decoded                         */
      (d25_122_find_tag(cup_job->op_id,cup_job) != D2_NO_ERR)
    {           /*  remove job and return                             */
      cup_job->ipc_ass_id = D2_NOT_SPEC;
      cup_job->job_number = 0L;
      free(cup_job->base_object);
      free(cup_job->result);
      cup_job->result = (char *)NULL;
      DCE_SVC_LOG((GDS_S_INCOMP_DEC_OBJ_FOUND_MSG));
      return(D25_ANSUC);
    }
    else if     /*  cache update is already active                    */
	   (cup_active == TRUE)
    {           /*  return                                            */
      return(D25_ANSUC);
    }
    else if     /*  not enough memory for request message             */
	   ((add_message = (byte *)D27_MALLOC(svc_c_sev_warning,
	     D25_CUP_BLOCK_LEN)) == NULL)
    {           /*  remove job and return                             */
      cup_job->ipc_ass_id = D2_NOT_SPEC;
      cup_job->job_number = 0L;
      free(cup_job->base_object);
      free(cup_job->result);
      cup_job->result = (char *)NULL;
      return(D25_ANSUC);
    }
    else
    {                   /*  set memory length                         */
      add_memlen = D25_CUP_BLOCK_LEN;
      switch(cup_job->op_id)        /*  check calling function        */
      {
	case D23_COMPARE:
	  ret_value = d25_123_add_comp_en(cup_job,&add_message,&add_memlen);
	  break;
	default:
	  ret_value = d25_124_add_en(cup_job,&add_message,&add_memlen);
	  break;
      }
      if        /*  First add message could not be created            */
	(ret_value != D2_NO_ERR)
      {         /*  remove job and return                             */
	cup_job->ipc_ass_id = D2_NOT_SPEC;
	cup_job->job_number = 0L;
	free(cup_job->base_object);
	free(cup_job->result);
	cup_job->result = NULL;
	free(add_message);
	return(D25_ANSUC);
      }
      else
      {         /*  get internally used resources for cache update    */
		/*  search empty IPC-table entry                      */
	for     /*  all IPC-table entries                             */
	   (i = 0, ipcptr = action_pb->d25_asinforef->d25_itabref;
	    i < action_pb->d25_asinforef->d25_imaxentries; i++, ipcptr++)
	{       /*  check whether entry is free                       */
	  if    /*  entry is free                                     */
	    (D25_GIOS(ipcptr->d25_istate) == D25_IRIDLE)
	  {     /*  break loop                                        */
	    break;
	  }     /*                                                    */
	}       /*                                                    */
	if      /*  IPC-table overflow                                */
	  (i >= action_pb->d25_asinforef->d25_imaxentries)
	{       /*  log error and return                              */
	  dce_svc_printf(GDS_S_IOVERFLOW_ERR_MSG,
	    action_pb->d25_asinforef->d25_imaxentries);
	  cup_job->ipc_ass_id = D2_NOT_SPEC;
	  cup_job->job_number = 0L;
	  free(cup_job->base_object);
	  free(cup_job->result);
	  cup_job->result = NULL;
	  free(add_message);
	  return(D25_ANSUC);
	}
	else
	{        /*  occupy IPC-table entry                            */
	  action_pb->d25_lpar.d25_alocptr = (void *)action_pb->d25_aipcref;
	  action_pb->d25_aipcref          = ipcptr;
	  ipcptr->d25_istate              = D25_ICINVOKE_PENDING;
	  ipcptr->d25_iassid              = D25_UNUSED;
	  ipcptr->d25_irosref             = (D25_rosentry *) NULL ;
	  ipcptr->d25_ioprid              = D23_ADDOBJT;
	  ipcptr->d25_iudat.d25_datref    = add_message;
	  ipcptr->d25_iudat.d25_datlen    = add_memlen;
	  ipcptr->d25_iudat.d25_datoff    = 0;
	  action_pb->d25_asinforef->d25_iactentries++;
	  DCE_SVC_LOG((GDS_S_OCC_IPCENTRY_MSG,i,ipcptr->d25_iassid));
	}
      }
    }
  }

  return(D25_ACONT);

}               /*  d25_121_save_result_data                          */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_122_find_tag                                    */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 06.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function skips all information from the         */
/*                beginning of message, which is useless for cache    */
/*                and sets the work pointer. The base object is over- */
/*                written if it is returned in the result.            */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR   signed16            job may be performed          */
/*      D2_ERROR                        job may not be performed      */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d25_122_find_tag(
    R_int op_id,        /* IN    -  operation ID                      */
			/* INOUT -  pointer to cache update job table */
    D25_cup_job *cup_job)           /* entry                          */
{                /*  d25_122_find_tag                                 */
			/* actual message pointer                     */
  char *   message = cup_job->result;
  signed16 offset;      /* size to move message pointer               */
  Bool     complete;    /* indicates whether DN is completely decoded */

  switch(op_id)
  {
    case D23_COMPARE:
      message += sizeof(D23_rscompinfo);
      if(((D23_ds_name *)message)->d23_dstag == D23_NAMTAG)
      {
	if(((D23_ds_name *)message)->d23_dscomplete == FALSE)
	{
	  return(D2_ERROR);
	}
	else
	{
	  offset = ((D23_ds_name *)message)->d23_dsoff;
	  message += sizeof(D23_ds_name);
	  strcpy((char *)cup_job->base_object,message);
	  message += offset;
	}
      }
      break;
    case D23_LIST:
      if(((D23_ds_name *)message)->d23_dstag == D23_BONTAG)
      {
	if(((D23_ds_name *)message)->d23_dscomplete == FALSE)
	{
	  return(D2_ERROR);
	}
	else
	{
	  offset = ((D23_ds_name *)message)->d23_dsoff;
	  message += sizeof(D23_ds_name);
	  strcpy((char *)cup_job->base_object,message);
	  message += offset;
	}
      }
      break;
    case D23_SEARCH:
    case D23_SEARCH + D2_ADM_OPID:
      if(((D23_ds_name *)message)->d23_dstag == D23_BONTAG)
      {
	complete = ((D23_ds_name *)message)->d23_dscomplete;
	offset   = ((D23_ds_name *)message)->d23_dsoff;
	message += sizeof(D23_ds_name) + offset;
	if(complete == FALSE)
	{
	  offset   = ((D23_av_value *)message)->d23_avoff;
	  message += sizeof(D23_av_value) + offset;
	}
      }
      cup_job->base_object[0] = D2_EOS;
      break;
    case D23_READ:
    case D23_READ + D2_ADM_OPID:
      cup_job->base_object[0] = D2_EOS;
      break;
  }

		/*  set work pointer                                  */
  cup_job->work = message;

  return(D2_NO_ERR);

}               /*  d25_122_find_tag                                  */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_123_add_comp_en                                 */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function generates the add object message for   */
/*                the cache update of a compare result.               */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR                       message is generated          */
/*      D25_ERROR                       error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d25_123_add_comp_en(
			/* INOUT -  pointer to cache update job       */
    D25_cup_job *cup_job,
			/* INOUT -  message for add entry             */
    byte        **cmessage,
    signed32    *clen)  /* INOUT -  length of generated message       */
{                /*  d25_123_add_comp_en                              */
  byte     *message;     /* actual message pointer                    */
  signed32 remlen;       /* remaining message length                  */
  signed16 len;          /* length of part in current block           */
  signed16 offset;       /* offset to next apdu structure             */

  remlen  = *clen;
  message = *cmessage;

  if            /* error from writing header for add message          */
    ((message = d25_125_write_add_head(cup_job,cmessage,message,clen,
     &remlen)) == NULL)
  {             /* return error                                       */
    return(D25_ERROR);
  }
  else
  {             /* write base object into add message                 */
    len    = strlen((char *)cup_job->base_object) + 1;
    offset = D2_ALIGN(len);
    if                  /*  error from check buffer                   */
      ((message = d25_126_check_buffer(sizeof(D23_ds_name) + offset +
       sizeof(D23_eom),&remlen,clen,cmessage,message)) == NULL)
    {                   /*  return error                              */
      return(D25_ERROR);
    }
    else
    {                   /*  write distinguished name header           */
      ((D23_ds_name *)message)->d23_dstag = D23_NAMTAG;
      ((D23_ds_name *)message)->d23_dscomplete = TRUE;
      ((D23_ds_name *)message)->d23_dslen = len;
      ((D23_ds_name *)message)->d23_dsoff = offset;
    }
    remlen  -= sizeof(D23_ds_name);
    message += sizeof(D23_ds_name);
    memcpy(message,cup_job->base_object,len);
    remlen  -= offset;
    message += offset;
    ((D23_eom *)message)->d23_eomtag = D23_EOMTAG;
    remlen  -= sizeof(D23_eom);
    message += sizeof(D23_eom);
    cup_job->terminate = TRUE;
    *clen   -= remlen;
  }

  return(D2_NO_ERR);

}               /*  d25_123_add_comp_en                               */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_124_add_en                                      */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function generates the add object message for   */
/*                the cache update of a read, list or search result.  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR                       message is generated          */
/*      D25_ERROR                       error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d25_124_add_en(
			/* IN    -  pointer to cache update job       */
    D25_cup_job *cup_job,                             /**/
			/* INOUT -  message for add entry             */
    byte        **cmessage,
    signed32    *clen)  /* INOUT -  length of generated message       */
{                /*  d25_124_add_en                                   */
  unsigned16 tag;         /* scanned message tag                      */

  boolean    ignore;      /* indicates whether attributes must be     */
			  /* ignored                                  */

  byte       *message;    /* actual message pointer                   */
  byte       *cl_ei;      /* pointer to entry information in client   */
			  /* message                                  */
  signed32   remlen;      /* remaining message length                 */

  remlen  = *clen;
  message = *cmessage;

  if            /* error from writing header for add message          */
    ((message = d25_125_write_add_head(cup_job,cmessage,message,clen,
     &remlen)) == NULL)
  {             /* return error                                       */
    return(D25_ERROR);
  }
  else
  {             /* skip first structure                               */
    d25_cl_type = NULL;
    cl_ei = message;
    for(tag = *(unsigned16 *)cup_job->work;
	(tag != D23_ENTTAG && tag != D23_SOITAG || cl_ei == message) &&
	 tag != D23_OQTAG && tag != D23_CRSTAG;
	 tag = *(unsigned16 *)cup_job->work)
    {           /*  check tag                                         */
      switch(tag)
      {
	case D23_ENTTAG:
	  cup_job->work += sizeof(D23_en_info);
	  ignore = FALSE;
	  break;
	case D23_SOITAG:
	  cup_job->work += sizeof(D23_subord_info);
	  ignore = FALSE;
	  break;
	case D23_NAMTAG:
	  if((message = d25_128_write_add_dn(cup_job,cmessage,message,clen,
	     &remlen,&ignore)) == NULL)
	  {
	    return(D25_ERROR);
	  }
	  break;
	case D23_ATTTAG:
	  if(d25_cl_type != NULL)
	  {
	    message = d25_cl_type;
	  }
	  if((message = d25_129_write_add_type(cup_job,cmessage,message,clen,
	     &remlen,&ignore)) == NULL)
	  {
	    return(D25_ERROR);
	  }
	  break;
	case D23_AVTAG:
	  d25_cl_type = NULL;
	  if((message = d25_130_write_add_avinfo(cup_job,cmessage,message,
	     clen,&remlen,ignore)) == NULL)
	  {
	    return(D25_ERROR);
	  }
	  break;
	default:
	  if((message = d25_131_write_add_avvalue(cup_job,cmessage,message,
	     clen,&remlen,ignore)) == NULL)
	  {
	    return(D25_ERROR);
	  }
	  break;
      }
    }
    if                  /* no message was written                     */
      (cl_ei == message)
    {                   /* return error                               */
      return(D25_ERROR);
    }
    else if             /* error from check buffer                    */
	   ((message = d25_126_check_buffer(sizeof(D23_eom),&remlen,clen,
	    cmessage,message)) == NULL)
    {                   /* return error                               */
      return(D25_ERROR);
    }
    else
    {                   /*  write end of message tag                  */
      ((D23_eom *)message)->d23_eomtag = D23_EOMTAG;
      remlen  -= sizeof(D23_eom);
      message += sizeof(D23_eom);
    }
  }

  *clen -= remlen;

  return(D2_NO_ERR);

}               /*  d25_124_add_en                                    */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_125_write_add_head                              */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function writes the header of the add object    */
/*                message for cache update.                           */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      != NULL     char *          pointer to byte after message     */
/*                                  written.                          */
/*         NULL                     error occurred                    */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d25_125_write_add_head(
			/* IN    -  pointer to cache update job       */
    D25_cup_job *cup_job,                             /**/
			/* INOUT -  message for add entry header      */
    byte        **cmessage,
			/* IN    -  actual message pointer            */
    byte        *message,
			/* INOUT -  overall length of add entry       */
    signed32    *memlen,            /* message                        */
			/* INOUT -  remaining length of add entry     */
    signed32    *remlen)            /* message                        */

{                /* d25_125_write_add_head                            */
  signed16 len;                 /* length of data to write            */

  len = sizeof(D23_inaddobjt);

  if                    /* error from check buffer                    */
    ((message = d25_126_check_buffer(len,remlen,memlen,cmessage,message))
     == NULL)
  {                     /* return error                               */
    return(NULL);
  }
  else
  {                     /* write add object header                    */
    ((D23_inaddobjt *)message)->d23_Mhdr.d23_versno = D23_APDUV22;
    ((D23_inaddobjt *)message)->d23_Mhdr.d23_dirid  = cup_job->dir_id;
    *remlen -= len;
    message += len;

			/* write common arguments                     */
    len = sizeof(D23_com_arg);
    if                  /* error from check buffer                    */
      ((message = d25_126_check_buffer(len,remlen,memlen,cmessage,message))
       == NULL)
    {                     /* return error                             */
      return(NULL);
    }
    else
    {                     /* write common arguments                   */
      ((D23_com_arg *)message)->d23_cmtag = D23_CMTAG;
      ((D23_com_arg *)message)->d23_cmali = D2_NOT_DEFINED;
      ((D23_com_arg *)message)->d23_cmctr = cup_job->serv_ctrl;
      ((D23_com_arg *)message)->d23_cmopr.d23_opnm_rs   = D2_NOT_STARTED;
      ((D23_com_arg *)message)->d23_cmopr.d23_op_nxtrdn = 0;
      *remlen -= len;
      message += len;
    }
  }

		/* return no error                                    */
  return(message);

}               /* d25_125_write_add_head                             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d25_126_check_buffer                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        09.05.88                                              */
/*                                                                    */
/* DESCRIPTION: This function checks, whether the memory for cmessage */
/*              is large enough, to contain len more bytes. If not,   */
/*              the memory is reallocated. A pointer for to write the */
/*              message will be returned.                             */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      != D2_NULL  String          pointer to where the message may  */
/*                                  be written                        */
/*         D2_NULL                  in case of errors                 */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d25_126_check_buffer(
    signed16 len,       /* IN    - count of bytes to write            */
    signed32 *remlen,   /* INOUT - remaining length of message        */
    signed32 *memlen,   /* INOUT - overall length of message          */
    byte     **cmessage,/* INOUT - cache update message               */
    byte     *message)  /* IN    - actual message pointer             */
{                               /* d25_126_check_buffer               */

  byte *new_message;    /* message pointer returned from realloc      */

  if                    /* block length too small                     */
    (D25_CUP_BLOCK_LEN < len)
  {                     /* return error                               */
    dce_svc_printf(GDS_S_LARGE_CUPMSG_ELEMENT_MSG,len);
    return(NULL);
  }
  else if               /* message block is full                      */
	 (*remlen < (signed16)len)
  {                     /* reallocate it                              */
    if                  /* error from reallocating                    */
      ((new_message = (byte *)D27_REALLOC(svc_c_sev_warning,*cmessage,
       *memlen + D25_CUP_BLOCK_LEN)) == NULL)
    {                   /* log error                                  */
      return(NULL);
    }
    else
    {                   /*  reset message and length                  */
      *cmessage    = new_message;
      *remlen     += D25_CUP_BLOCK_LEN;
      *memlen     += D25_CUP_BLOCK_LEN;
      return(*cmessage + *memlen - *remlen);
    }
  }
  else
  {                     /*  return old message                        */
    return(message);
  }

}                       /* d25_126_check_buffer                       */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_128_write_add_dn                                */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads a distinguished name from the    */
/*                result message and writes it into the message for   */
/*                add object to cache.                                */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      != D2_NULL  String              pointer to byte after message */
/*                                      written.                      */
/*         D2_NULL                      error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d25_128_write_add_dn(
			/* IN    -  pointer to cache update job       */
    D25_cup_job *cup_job,                             /**/
			/* INOUT -  message for add entry header      */
    byte        **cmessage,
			/* IN    -  actual message pointer            */
    byte        *message,
			/* INOUT -  overall length of add entry       */
    signed32    *memlen,            /* message                        */
			/* INOUT -  remaining length of add entry     */
    signed32    *remlen,            /* message                        */
			/* IN    -  indicates that DN must be ignored */
    boolean     *ignore)                               /**/
{                /*  d25_128_write_add_dn                             */
  signed16 len;                 /* length of data to write            */
  signed16 s_offset;            /* offset to next data part in server */
				/* message                            */
  Bool     complete;            /* indicates whether DN is decoded    */
				/* completely                         */
  signed16 offset;              /* offset to next data part           */
  char     cat_object[D2_DNL_MAX + 1];  /* pointer to DN to write     */
  D2_name_string object;        /* pointer to DN to write             */

  s_offset = ((D23_ds_name *)cup_job->work)->d23_dsoff;
  complete = ((D23_ds_name *)cup_job->work)->d23_dscomplete;
  if                    /* DN note decoded completely                 */
    (complete == FALSE)
  {                     /* reset ignore flag                          */
    *ignore = TRUE;
  }

  if                    /* object must be written                     */
    (*ignore == FALSE)
  {                     /* check buffer for name structure            */
    if                  /* error from check buffer                    */
      ((message = d25_126_check_buffer(sizeof(D23_ds_name),remlen,memlen,
       cmessage,message)) == NULL)
    {                   /* return error                               */
      return(NULL);
    }
    else
    {                   /* write distinguished name header            */
      if                /* list has called                            */
	(cup_job->base_object[0] != D2_EOS)
      {                 /* concatenate names                          */
	sprintf(cat_object,"%s%c%s",cup_job->base_object,D2_RDN_SEP,
	  cup_job->work + sizeof(D23_ds_name));
	((D23_ds_name *)message)->d23_dstag = D23_NAMTAG;
	((D23_ds_name *)message)->d23_dscomplete = TRUE;
	((D23_ds_name *)message)->d23_dslen = len =
	  (signed16)(strlen((char *)cup_job->base_object) + 1);
	((D23_ds_name *)message)->d23_dsoff = offset = D2_ALIGN(len);
	object = (D2_name_string)cat_object;
      }
      else
      {                 /* copy name structure                        */
	*((D23_ds_name *)message) = *((D23_ds_name *)cup_job->work);
	object = (D2_name_string)((D23_ds_name *)cup_job->work + 1);
	len    = ((D23_ds_name *)cup_job->work)->d23_dslen;
	offset = s_offset;
      }
      *remlen       -= sizeof(D23_ds_name);
      message       += sizeof(D23_ds_name);
      cup_job->work += sizeof(D23_ds_name);

      if                /* error from check buffer                    */
	((message = d25_126_check_buffer(offset,remlen,memlen,cmessage,
	 message)) == NULL)
      {                 /* return error                               */
	return(NULL);
      }
      else
      {                 /*  write distinguished name into message     */
	memcpy(message,object,len);
	*remlen       -= offset;
	message       += offset;
	cup_job->work += s_offset;
      }
    }
  }
  else
  {                     /* reset server message pointer               */
    cup_job->work += sizeof(D23_ds_name) + s_offset;
    s_offset = ((D23_av_value *)cup_job->work)->d23_avoff;
    cup_job->work += sizeof(D23_av_value) + s_offset;
  }

			/* return no error                            */
  return(message);

}               /* d25_128_write_add_dn                               */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_129_write_add_type                             */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads an attribute type from the       */
/*                server message and writes it for add object into    */
/*                message for cache.                                  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*                String    != D2_NULL          pointer to byte after */
/*                                              message written.      */
/*                             D2_NULL          error occurred        */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d25_129_write_add_type(
			/* IN    -  pointer to cache update job       */
    D25_cup_job *cup_job,                             /**/
			/* INOUT -  message for add entry header      */
    byte        **cmessage,
			/* IN    -  actual message pointer            */
    byte        *message,
			/* INOUT -  overall length of add entry       */
    signed32    *memlen,            /* message                        */
			/* INOUT -  remaining length of add entry     */
    signed32    *remlen,            /* message                        */
			/* IN    -  indicates that DN must be ignored */
    boolean     *ignore)
{                       /* d25_129_write_add_type                     */
  signed16 len;         /* length of data to write                    */
  signed16 offset;      /* offset to next data part                   */
  D2_a_type comp_type;  /* attribute type to compare with pseudo-     */
			/* attribute                                  */

  comp_type.d2_typ_len = ((D23_at_type *)cup_job->work)->d23_atlen;
  comp_type.d2_type    = (Ob_id_string)(cup_job->work + sizeof(D23_at_type));

  if                    /* pseudo attribute is coming in              */
    (!D2_ATTCMP(&comp_type,D2_CACHE_ATTR))
  {                     /* reset ignore flag                          */
    *ignore = TRUE;
  }

  len = sizeof(D23_at_type) + ((D23_at_type *)cup_job->work)->d23_atoff;
  if                    /* attribute type has to be written           */
    (*ignore == FALSE)
  {                     /* check buffer                               */
    if                  /* error from check buffer                    */
      ((message = d25_126_check_buffer(len,remlen,memlen,cmessage,message))
       == NULL)
    {                   /* return error                               */
      return(NULL);
    }
    else
    {                   /* write attribute type and set pointer to    */
			/* type in client message                     */
      d25_cl_type = message;
      *((D23_at_type *)message) = *((D23_at_type *)cup_job->work);
      offset = ((D23_at_type *)cup_job->work)->d23_atoff;
      cup_job->work += sizeof(D23_at_type);
      message       += sizeof(D23_at_type);
      *remlen       -= sizeof(D23_at_type);

      memcpy(message,cup_job->work,comp_type.d2_typ_len);
      cup_job->work += offset;
      message       += offset;
      *remlen       -= offset;
    }
  }
  else
  {                     /* skip server message                        */
    cup_job->work += len;
  }

			/* return no error                            */
  return(message);

}                       /* d25_129_write_add_type                     */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_130_write_add_avinfo                              */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 09.05.88                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads an attribute value information   */
/*                from the result message                             */
/*                and writes it for add object into message for cache */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      != D2_NULL  String              pointer to byte after message */
/*                                      written.                      */
/*         D2_NULL                      error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d25_130_write_add_avinfo(
			/* IN    -  pointer to cache update job       */
    D25_cup_job *cup_job,                             /**/
			/* INOUT -  message for add entry header      */
    byte        **cmessage,
			/* IN    -  actual message pointer            */
    byte        *message,
			/* INOUT -  overall length of add entry       */
    signed32    *memlen,            /* message                        */
			/* INOUT -  remaining length of add entry     */
    signed32    *remlen,            /* message                        */
			/* IN    -  indicates that DN must be ignored */
    boolean     ignore)
{                       /* d25_130_write_add_avinfo                   */
  signed16 len;         /* length of data to write                    */

  len = sizeof(D23_av_info);
  if                    /* attribute information has to be written    */
    (ignore == FALSE)
  {                     /* check buffer                               */
    if                  /* error from check buffer                    */
      ((message = d25_126_check_buffer(len,remlen,memlen,cmessage,message))
       == NULL)
    {                   /* return error                               */
      return(NULL);
    }
    else
    {                   /* write attribute information                */
      *(D23_av_info *)message = *(D23_av_info *)cup_job->work;
      *remlen -= len;
      message += len;
    }
  }

  cup_job->work += len;

			/* return no error                            */
  return(message);

}                       /* d25_130_write_add_avinfo                   */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_131_write_add_avvalue                           */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 28.11.90                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads an attribute value               */
/*                from the result message                             */
/*                and writes it for add object into message for cache */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      != NULL     char *          pointer to byte after message     */
/*                                  written.                          */
/*         NULL                     error occurred                    */
/*                                                                    */
/*inoff ***************************************************************/

static byte *d25_131_write_add_avvalue(
			/* IN    -  pointer to cache update job       */
    D25_cup_job *cup_job,                             /**/
			/* INOUT -  message for add entry header      */
    byte        **cmessage,
			/* IN    -  actual message pointer            */
    byte        *message,
			/* INOUT -  overall length of add entry       */
    signed32    *memlen,            /* message                        */
			/* INOUT -  remaining length of add entry     */
    signed32    *remlen,            /* message                        */
			/* IN    -  indicates that DN must be ignored */
    boolean     ignore)
{                       /* d25_131_write_add_avvalue                  */
  signed16   offset;    /* offset to next data part                   */
  unsigned16 avrep;     /* representation of attribute value          */

  offset = ((D23_av_value *)cup_job->work)->d23_avoff;
  avrep  = ((D23_av_value *)cup_job->work)->d23_avrep;

  if                    /* attribute information has to be written    */
    (ignore == FALSE && offset <= D2_ATL_MAX)
  {                     /* check buffer                               */
    if                  /* error from check buffer                    */
      ((message = d25_126_check_buffer(sizeof(D23_av_value) + offset,remlen,
       memlen,cmessage,message)) == NULL)
    {                   /* return error                               */
      return(NULL);
    }
    else
    {                   /* write attribute value structure            */
      *(D23_av_value *)message = *(D23_av_value *)cup_job->work;
      *remlen       -= sizeof(D23_av_value);
      message       += sizeof(D23_av_value);
      cup_job->work += sizeof(D23_av_value);
      memcpy(message,cup_job->work,offset);
      *remlen       -= offset;
      message       += offset;
      cup_job->work += offset;
    }
  }
  else
  {                     /* skip server message                        */
    cup_job->work += sizeof(D23_av_value) + offset;
  }

			/* return no error                            */
  return(message);

}                       /* d25_131_write_add_avvalue                  */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_141_restore_env                                 */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/*                                                                    */
/* DATE         : 11.07.91                                            */
/*                                                                    */
/* DESCRIPTION  : The Function restores the IPC-table reference       */
/*                in the action parameter block.                      */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      signed32    indicates following state to be entered           */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d25_141_restore_env(
			/* INOUT  - Parameter block used for all stub */
    D25_actionpb *action_pb)        /* operations                     */
{                       /* d25_141_restore_env                        */

  action_pb->d25_aipcref = (D25_ipcentry *)action_pb->d25_lpar.d25_alocptr;

  return(D25_ANSUC);

}                       /* d25_141_restore_env                        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d25_151_continue_update                             */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/*                                                                    */
/* DATE         : 11.07.91                                            */
/*                                                                    */
/* DESCRIPTION  : The Function continues the cache update, which was  */
/*                started before. It generates an add message and     */
/*                sets the work pointer in the cache update job.      */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      signed32    indicates following state to be entered           */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d25_151_continue_update(
			/* INOUT  - Parameter block used for all stub */
    D25_actionpb *action_pb)        /* operations                     */
{                       /* d25_151_continue_update                    */

  byte     *add_message;/* pointer to cache update message            */
  signed32 add_memlen;  /* length of cache update message             */
  D25_cup_job *cup_job; /* pointer to cache update job                */

  signed32 ret_value;   /* return value                               */

			/* check the cache update event               */
  switch(action_pb->d25_aevent)
  {
    case D25_IREJECT_EV:/* cache has rejected                         */
      DCE_SVC_LOG((GDS_S_CUPREQ_REJECT_MSG));
      break;
    case D25_IERROR_EV: /* cache has returned error                   */
      DCE_SVC_LOG((GDS_S_CUPREQ_ERROR_MSG));
      break;
  }

  while                 /* still cache update job is active           */
       ((cup_job = d25_103_active_job()) != (D25_cup_job *)NULL)
  {                     /* check whether job may terminate            */
    if                  /* job may terminate                          */
      (cup_job->terminate == TRUE)
    {                   /* reset CUP-job                              */
      cup_job->ipc_ass_id = D2_NOT_SPEC;
      cup_job->job_number = 0L;
      free(cup_job->base_object);
      free(cup_job->result);
      cup_job->result = NULL;
    }
    else if             /* not enough memory for request message      */
	   ((add_message = (byte *)D27_MALLOC(svc_c_sev_warning,
	    D25_CUP_BLOCK_LEN)) == NULL)
    {                   /* log error and remove job                   */
      cup_job->ipc_ass_id = D2_NOT_SPEC;
      cup_job->job_number = 0L;
      free(cup_job->base_object);
      free(cup_job->result);
      cup_job->result = NULL;
    }
    else
    {                   /* check calling function                     */
      add_memlen = D25_CUP_BLOCK_LEN;
      switch(cup_job->op_id)
      {
	case D23_COMPARE:
	  ret_value = d25_123_add_comp_en(cup_job,&add_message,&add_memlen);
	  break;
	default:
	  ret_value = d25_124_add_en(cup_job,&add_message,&add_memlen);
	  break;
      }
      if                /* Add message could not be created           */
	(ret_value != D2_NO_ERR)
      {                 /* remove job and return                      */
	cup_job->ipc_ass_id = D2_NOT_SPEC;
	cup_job->job_number = 0L;
	free(cup_job->base_object);
	free(cup_job->result);
	cup_job->result = NULL;
	free(add_message);
      }
      else
      {                 /* assign created message to IPC-table entry  */
	action_pb->d25_aipcref->d25_iudat.d25_datref = add_message;
	action_pb->d25_aipcref->d25_iudat.d25_datlen = add_memlen;
	action_pb->d25_aipcref->d25_iudat.d25_datoff = 0;
	return(D25_ACONT);
      }
    }
  }

return(D25_ANSUC);

}                       /* d25_151_continue_update                    */
