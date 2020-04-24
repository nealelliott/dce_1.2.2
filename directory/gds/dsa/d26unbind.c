/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26unbind.c,v $
 * Revision 1.1.10.2  1996/02/18  19:46:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:49  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:49:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:15  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:52:56  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:25:58  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:39:59  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:21:14  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  15:37:06  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:21:22  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  19:43:00  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:21:34  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:53:15  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:58:38  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26unbind.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:46:21 $";
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
/* NAME         : d26unbind.c   < DS_UNBIND >                         */
/*								      */
/* AUTHOR       : Schreck                                             */
/* DATE         : 13.03.89                                            */
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
/* NAME  :      d26_2_unbind                                          */
/*								      */
/* AUTHOR:      Schreck D AP 11                                       */
/* DATE:        13.03.89                                              */
/*								      */
/* DESCRIPTION: The function removes the user credentials for the     */
/*              requestor and writes a result message                 */
/*								      */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR   signed16        no error occured                  */
/*								      */
/*exoff ***************************************************************/

signed16 d26_2_unbind(
    byte       *a_in,   /* IN    -  invoke message                    */
    byte       *a_out,  /* OUT   -  result or error message           */
    signed32   *length, /* OUT   -  length of result or error message */
    unsigned16 cont_id, /* IN    -  context ID                        */
    signed32   usr_id)  /* IN    -  user ID                           */

{                                   /*  d26_2_unbind                  */

  static char *function = "d26_2_unbind";
  static D23_remcrdpb ruc_pb = {{D23_V02}};

  signed16 ret_value = D2_NO_ERR;

				    /*  trace function entry          */
  DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

				   /*  remove requestor from shared   */
				   /*  memory                         */
  ruc_pb.d23_Ncontid = cont_id;
  ruc_pb.d23_Nusrid = usr_id;
  d23_rem_usr_cred(&ruc_pb);

			    /*  write result message                  */
  ((D23_eom *)a_out)->d23_eomtag = D23_EOMTAG;
  *length = sizeof(D23_eom);

  DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function,ret_value));
  return(ret_value);

}                           /*  d26_2_unbind                          */

