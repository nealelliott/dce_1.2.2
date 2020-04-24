/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26aban.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:52  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:42:33  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/mothra_work/1  1995/07/07  22:57 UTC  dtruong
 * 	Correct uninitialize variable
 * 	[1995/12/08  15:06:10  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:45:07  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:30:28  marrek]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:44  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:19:15  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:05:04  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:08:07  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  13:34:15  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:00:37  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:58:45  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:18:02  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:44:58  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:52:52  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26aban.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:45 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*								      */
/* TYPE         : MODULE                                              */
/*								      */
/* NAME         : d26aban.c     < DS_ABANDON >                        */
/*								      */
/* AUTHOR       : Kraemer                                             */
/* DATE         : 24.06.91                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES:                                               */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*   0.1    |13.03.89 |  Original                      | as |         */
/*								      */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

/*****  external Includes    *****/

/*****  internal Includes    *****/

#include <dce/d27util.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

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

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME  :      d26_9_abandon                                         */
/*								      */
/* AUTHOR:      Schreck D AP 11                                       */
/* DATE:        13.03.89                                              */
/*								      */
/* DESCRIPTION: This function generates the error message 'cannot     */
/*              abandon', if the abandon request was received.        */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*      String        input;        input message from IPC            */
/*      Int           input_len;    length of input message           */
/*      Int           *output_len;  length of memory allocated for    */
/*                                  result                            */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      String        *output;      result message to produce         */
/*      Int           *output_len;  length of memory for result       */
/*                                  message blocks                    */
/*								      */
/* RETURN VALUE :                                                     */
/*      Short       D2_NO_ERR               no error occured          */
/*                  D26_ERROR               error occured             */
/*								      */
/*exoff ***************************************************************/

signed16 d26_9_abandon(
    byte       *input,
    byte       **output,
    signed32   *output_len,
    unsigned16 cont_id,
    signed32   usr_id)

{                                   /*  d26_9_abandon                 */

  static char *function = "d26_9_abandon";
			/* parameter block for abandon                */
  static D23_abandonpb abandonpb = {{D23_V02}};

  byte     *message;    /* pointer to message to write                */
  signed32 invoke_id;   /* invoke ID of operation to be abandoned     */
  signed16 err_value;   /* error value of IPC abandon                 */
			/* return value                               */
  signed16 ret_value = D2_NO_ERR;

  DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

  message = *output;
  *output_len = 0;

				/*  get invoke ID from message        */
  abandonpb.d23_Ginvinfo.d23_invid  = ((D23_inabandon *)input)->d23_Rinv_id;
  abandonpb.d23_Ginvinfo.d23_usrid  = usr_id;
  abandonpb.d23_Ginvinfo.d23_contid = cont_id;

  if                            /* abandon returns error              */
    (d23_abandon(&abandonpb) < 0)
  {                             /* generate abandon error message     */
    ((D23_rserror *)message)->d23_Zretcod  = D2_CALL_ERR;
    ((D23_rserror *)message)->d23_Zerrcls  = D2_PAR_ERR;
    err_value = abandonpb.d23_Ghdinfo.d2_errvalue;
    ((D23_rserror *)message)->d23_Zerrval  = err_value == D23_NOSERV ?
      D2_NO_SUCH_OPERATION : D2_CANNOT_ABANDON;
    ((D23_rserror *)message)->d23_Zversion = D23_V1988;

    message         += sizeof(D23_rserror);
    *output_len     += sizeof(D23_rserror);

#ifdef __hpux_13317CHFts
    *(signed32 *)message = invoke_id;
#endif
    message         += sizeof(signed32);
    *output_len     += sizeof(signed32);
    ret_value        = D26_ERROR;
  }

  ((D23_eom *)message)->d23_eomtag = D23_EOMTAG;
  message         += sizeof(D23_eom);
  *output_len     += sizeof(D23_eom);

  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));

  return(ret_value);

}                               /*  d26_9_abandon                     */
