/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif05.c,v $
 * Revision 1.1.4.2  1996/03/11  13:21:58  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:31  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:09:47  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:51:57  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:11  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:01:27  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:45  keutel
 * 	creation
 * 	[1994/03/21  13:30:28  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roexif05.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:58 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1993 BY SIEMENS NIXDORF INFORMATIONS AG	      */
/*  * All rights reserved                                             */
/*  *                                                                 */
/*  * This software is furnished under licence and may be used only   */
/*  * in accordance with the terms of that licence and with the       */
/*  * inclusion of the above copyright notice.                        */
/*  * This software may not be provided or otherwise made available   */
/*  * to, or used by, any other person. No title to or ownership of   */
/*  * the software is hereby  transferred.                            */
/*                                                                    */
/* ****************************************************************** */

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME 	: roexif05.c					      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-03-25					      */
/*                                                                    */
/* COMPONENT    : ROS external interface                              */
/*                                                                    */
/* PRD#/VERS.	: ROS-V3.00					      */
/*                                                                    */
/* DESCRIPTION	: This modul contains the entry points to ROS-V3.0.   */
/*		  for IROS-3.00 ONLY FOR LOGGING                      */
/*                                                                    */
/* SYSTEM DEPENDENCE :	none					      */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/*  3.00 K0 | 93-03-25|  original		       | kc |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/





/**********************************************************************/
/*                                                                    */
/*		Module Identification				      */
/*                                                                    */
/**********************************************************************/

static char *sccsid = " ";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <ross.h>	 /* ROS external interface		     */
#include <roerval.h>	 /* ROS error value file		     */

#include <rogenrl.h>	 /* ROS general definitions		     */



/*********************************************************************/
/*                                                                   */
/*		M A C R O S					     */
/*                                                                   */
/*********************************************************************/

/* ****************** Macro which defines all single entry ********** */
/* ****************** functions                            ********** */
#ifdef SEPLOG

signed short int FAR PASCAL roexlog PROTO ((RO_excopb      FAR * pParlst));

#define RM1350_ENTRY(name, pblk, fnctnb)			     \
	         						     \
	int FAR PASCAL name (                                        \
              pblk FAR *ro_parblk)	                             \
	      { 						     \
	      ro_parblk->r_hdinfo.r_fnctnb = fnctnb;		     \
	      ro_parblk->r_hdinfo.r_cont   = FALSE;		     \
	      return (roexlog ((RO_excopb FAR *) ro_parblk));  \
	      }


/* ****************** ROS logging functions ************************* */
/* ******************                       ************************* */

RM1350_ENTRY (ro_logon, RO_lgonpb, RO_LOGON)
RM1350_ENTRY (ro_logof, RO_lgofpb, RO_LOGOF)
RM1350_ENTRY (ro_logwr, RO_lgwrpb, RO_LOGWR)



#endif

