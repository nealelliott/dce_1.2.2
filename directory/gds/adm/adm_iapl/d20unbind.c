/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20unbind.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:04  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:12:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:15  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:13  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:03:07  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:43  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:16:41  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:55:04  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:12:18  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:46:25  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:59:43  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:36:09  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:09:10  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20unbind.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:07 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20unbind.c                                         */
/*								      */
/* AUTHOR       : Schmid , D AP 11                                    */
/* DATE         : 10.05.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

#include <gds.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d21iapl.h>
#include <d20proto.h>

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20__unbind()                                         */
/*								      */
/* AUTHOR:      Schmid    (D AP 11)                                   */
/* DATE:        10.05.88                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20__unbind(bind_id)                       */
/*              signed16       bind_id;                               */
/*								      */
/* DESCRIPTION:                                                       */
/*              This function will execute the "unbind" operation.    */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              signed16       bind_id;                               */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*              D2_NOERROR:     no errors                             */
/*              D2_ERROR:       unbind failed                         */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 d2_ret_val d20__unbind(
   signed16 bind_id)

{                               /*  d20__unbind()                     */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

static  D2_pbhead   head = { D21V022 };
    D21_p2      parblk;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

parblk.d21_2pbhead  = &head;
parblk.d21_2bind_id = bind_id;

				/*  return(ret_value of "unbind"      */
return(d21_2_unbind(&parblk));

}                               /*  d20__unbind()                     */
