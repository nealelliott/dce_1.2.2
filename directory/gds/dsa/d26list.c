/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26list.c,v $
 * Revision 1.1.10.2  1996/02/18  19:45:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:45  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:45:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:09  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:52:22  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:22:22  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:06:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:14:06  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  14:15:36  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:11:44  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:40:57  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:19:57  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:45:57  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:12:50  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:49:58  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:56:14  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26list.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:45:29 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : d26list.c                                           */
/*                                                                    */
/* AUTHOR       : W. Schmid , D AP 11                                 */
/* DATE         : 09.02.88                                            */
/*                                                                    */
/* COMPONENT    : D2                                                  */
/*                                                                    */
/* DOC.-NR.     : D2-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*                                                                    */
/* DESCRIPTION  : The module contains the "list" function of DSA.     */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*      1.0 | 09.02.88| Birth                          | WS |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <stdlib.h>

/*****  external Includes    *****/

#include <dce/d27util.h>

/*****  internal Includes    *****/

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

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

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_4_list            <list>                        */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 09.02.88                                            */
/*                                                                    */
/* DESCRIPTION  : This function will return all object beneath the    */
/*                the given superior object.                          */
/*                This function is mapped on DS_SEARCH.               */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR:      signed16        no error occured              */
/*      D26_ERROR:                      any error has occurred        */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_4_list(
    byte       *input,  /* IN    -  request message                   */
			/* IN    -  length of request message         */
    signed32   input_len,
    byte       **output,/* OUT   -  result message                    */
			/* OUT   -  length of result message          */
    signed32   *output_len,
    D23_rsrqpb *rsrqpb) /* IN    -  parameters for d23_result         */

{                                   /*  d26_4_list                    */

  static char *function = "d26_4_list";

  signed32 meslen;                  /* message length                 */
  signed32 remlen;                  /* remaining message length       */
				    /* exact  dereferenced object     */
  byte     exact_obj[D2_DNL_MAX + 1];
				    /* object classes of base object  */
  signed32 ob_cl[D26_OCL_VAL_COUNT + 1];
				    /* indices into DN list for ACL   */
  D26_acl  acl[D2_ACLIDX_MAX];
				    /* index into DN list for master  */
  signed32 mk;                      /* knowledge                      */
  boolean  recurring;               /* recurring attribute indicator  */
  signed16 ob_nr;                   /* object number of base object   */
  char     *record;                 /* record containing base object  */
  signed16 ret_value;               /* return value                   */
  boolean  al_flag;                 /* indicates, whether alias was   */
				    /* found                          */

  byte     *s_i_apdu;               /* search invoke APDU             */
  byte     *scan;                   /* actual pointer to incoming     */
				    /* message                        */
  byte     *message;                /* actual pointer to message      */
  byte     *dap_message;            /* pointer to DAP-part of READ    */
				    /* message                        */
  signed16 result_type;             /* result type returned by req.   */
				    /* decomp. algo.	      	      */

  DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

  if                           /*  error during initialization of DSA */
    (d26_u09_init_dsa(0) != D2_NO_ERR)
  {                             /*  set return_value and error_codes  */
    d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,D2_REM_ERR,
      D2_UNWILLING,output_len);
    ret_value = D26_ERROR;
  }
  else if                           /*  error from allocating memory  */
	 ((s_i_apdu = (byte *)D27_MALLOC(svc_c_sev_warning,
	  input_len + sizeof(D23_srcharg) + sizeof(D23_rq_info))) == NULL)
  {                              /*  set return_value and error_codes */
    d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,D2_REM_ERR,
      D2_UNWILLING,output_len);
    ret_value = D26_ERROR;
  }
  else
  {                           /*  build parameter block for DS_SEARCH*/
    scan    = input;
    message = s_i_apdu;
				    /*  copy header                   */
    *((D23_header *)message) = *((D23_header *)scan);
    scan    += sizeof(D23_header);
    message += sizeof(D23_header);
    d26_a02_analyse_chaining_args(&scan);

    dap_message = scan;
    input_len  -= scan - input;
    d27_501_copy_common_arguments(&scan,&message);
				    /*  create search arguments       */
    ((D23_srcharg *)message)->d23_Itag    = D23_ITAG;
    ((D23_srcharg *)message)->d23_Iali    = FALSE;
    ((D23_srcharg *)message)->d23_Iobjsub = D2_ONE_LEVEL;
    message += sizeof(D23_srcharg);
				    /*  copy distinguished name       */
    d27_502_copy_distinguished_name(&scan,&message);
				    /*  generate empty request-info   */
    ((D23_rq_info *)message)->d23_rqtag = D23_REQTAG;
    ((D23_rq_info *)message)->d23_rqreq = D2_R_TYP;
    ((D23_rq_info *)message)->d23_rqnbr = 0;
    message += sizeof(D23_rq_info);

				    /* generate end of message        */
    ((D23_eom *)message)->d23_eomtag = D23_EOMTAG;

				    /*  call internal search          */
    ret_value = d26_u00_search(FALSE,D23_LIST,s_i_apdu,output_len,output,
      &remlen,exact_obj,d26_attr,ob_cl,acl,&mk,&recurring,&record,&al_flag,
      &ob_nr);
    switch(ret_value)
    {
      case D2_NO_ERR:

	if          /*  chaining preferred,                           */
		    /*  limits are not exceeded and  continuation     */
		    /*  refererence present                           */
	  (!(d26_serv_contr->d23_scopt & D2_NOCHAINING) &&
	   (d26_serv_contr->d23_scopt & D2_PREF_CHAINING) &&
	   (d26_chaining_policy == D26_MULTI_HOP || d26_chain_arg == NULL) &&
	   (d26_serv_contr->d23_scslm == D2_S_UNLIMITED ||
	   d26_entry_count <= d26_serv_contr->d23_scslm) &&
	   d26_dbop_flag != D26_DB_NOOP && d26_time_ex == FALSE &&
	   d26_ref_present == TRUE)
	{           /*  handle request decomposition.                 */
	  result_type = D26_RD_RESULT;
	  if        /*  error from request decomposition              */
	    (d26_d21_req_decomp(dap_message,input_len,output,output_len,
	     &remlen,D23_LIST,&result_type,rsrqpb) == D26_ERROR)
	  {         /*  return error                                  */
	    d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,
	      D2_REM_ERR,D2_UNWILLING,&meslen);
	    remlen    = *output_len - meslen;
	    ret_value = D26_ERROR;
	  }
	}
	d26_u41_free_referrals();
	break;
      case D2_REF_ERROR:
	if              /*  chaining required                         */
	  (!(d26_serv_contr->d23_scopt & D2_NOCHAINING) &&
	   d26_serv_contr->d23_scopt & D2_PREF_CHAINING &&
	   d26_dbop_flag != D26_DB_NOOP && d26_time_ex == FALSE &&
	   (d26_chaining_policy == D26_MULTI_HOP || d26_chain_arg == NULL))
	{               /*  do the chaining                           */
	  ret_value = d26_d01_chain_request(dap_message,input_len,output,
	    output_len,&remlen,D23_LIST,rsrqpb);
	}
	if              /*  still no result present                   */
	  (ret_value == D2_REF_ERROR || ret_value == D26_ERROR)
	{               /*  write the error referrals                 */
	  if            /*  hard error from writing referrals         */
	    (d26_u63_write_error_referral(output,output_len,&remlen,
	     d26_referral) != D2_NO_ERR)
	  {             /*  set  error apdu                           */
	    d27_010_set_error_apdu((D23_rserror *)*output,D2_TEMP_ERR,
	      D2_REM_ERR,D2_UNWILLING,&meslen);
	    remlen -= meslen;
	  }
	  ret_value = D26_ERROR;
	}
	else if         /*  other DSP error occurred                  */
	     (ret_value == D26_DSP_ERR)
	{               /*  reset the return value                    */
	  ret_value = D26_ERROR;
	}
	d26_u41_free_referrals();
	break;
      case D2_ABANDONED:
	d26_u41_free_referrals();
	d26_u84_send_abandoned(D23_LIST,output,&remlen,output_len,rsrqpb);
	ret_value = D26_ERROR;
	break;
      default:
	ret_value = D26_ERROR;
	break;
    }
    *output_len -= remlen;
    alarm(0);

    if                  /*  DAP-result  must be returned              */
      (ret_value == D2_NO_ERR && d26_chain_arg == NULL)
    {                   /*  update cache if necessary                 */
      d26_h01_cache_update(*output,D23_LIST,d26_dir_id,d26_serv_contr,
	d26_base_object);
    }
    free(s_i_apdu);
  }

  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));

				    /*  return ret_value              */
  return(ret_value);

}                                   /*  d26_4_list                    */
