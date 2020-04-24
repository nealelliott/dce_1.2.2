/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexifd3.c,v $
 * Revision 1.1.4.2  1996/03/11  13:22:04  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:49  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:42  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:09:57  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:05  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:18  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:02:08  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:37  keutel
 * 	creation
 * 	[1994/03/21  13:31:07  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roexifd3.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:04 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1993 BY SIEMENS NIXDORF INFORMATIONS AG         */
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
/* NAME         : roexifd2.c                                          */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 93-07-23                                            */
/*                                                                    */
/* COMPONENT    : ROS external interface                              */
/*                                                                    */
/* PRD#/VERS.   : ROS-V3.00                                           */
/*                                                                    */
/* DESCRIPTION  : This modul contains the entry points to ROS-V3.0.   */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  MS-DOS WINDOWS 3.00                           */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/*  3.00 K00| 93-07-23|  original                      | jm |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "@(#)roexif2.c   3.00 93/07/23";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/

#ifdef R_G000_Windows
#define WINDOWS
#endif

#include <windows.h>


#undef ERROR
#define WIN_INCLUDED
#define RP0002_WinIncl /* for dos compilation -sss */


#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <ross.h>        /* ROS external interface                   */
#include <rogenrl.h>     /* RTS general definitions                  */  
#include <roexif02.h>    /* ROS external interface internal include  */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1320_taskId                                       */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed long int FAR PASCAL ro1320_taskId (void)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

return ((HANDLE) GetCurrentTask());

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1321_ssiMpi                                       */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 90-12-31                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

FARPROC         FAR PASCAL ro1321_ssiMpi (

			   FARPROC           fct,
			   signed long int   mHd)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

return (MakeProcInstance ((FARPROC) fct, (HANDLE) mHd));

}/*end*/
