/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil62.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:18  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:33:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:39  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:15  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:50:08  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:53  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:55:48  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:44  keutel
 * 	creation
 * 	[1994/03/21  13:25:26  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: routil62.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:18 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1992                     */
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
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : routil62.c                                          */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* COMPONENT    : Remote Opeartions service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Logging Functions for Open/write/close              */
/*                Uses the Local Open /Close / Write functions        */
/*                                                                    */
/* EVALUATED CONDITIONS :  RC0008_LOG_V1,RC0008_LOG_V2,RC0008_LOG_V3  */        
/*                                                                    */        
/* SYSTEM DEPENDENCIES : None                                         */
/*                       Compiler Constant switched                   */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)routil62.c	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-04-19|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                   Module Dependencies                              */
/**********************************************************************/
					  /* select logging version 2 */ 
#ifndef RC0061_ThroOSS

									 
		      /***** use the RTS-V2.0 logging ****************/  
/* ****************************************************************** */
/*           I N C L U D E     F I L E S                              */
/* ****************************************************************** */
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <gds.h>
#include <string.h>


#include <rogenrl.h>         /* ROS general defintions & Macros       */
#include <roerval.h>         /* Return error codes                    */
#include <rouply0.h>      

#include <routil0.h>         /* defs for File function calls          */

#include <routil6.h>         /* Data & function definitions for logging*/
#include <roloif9.h>         /* loif logging function prtos           */
#include <rolgval.h>




RM0001_STRG (rs0060, "ro0060_OpenLog          ")
RM0001_STRG (rs0061, "ro0061_CloseLog         ")


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0060_OpenLog                                      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int FAR PASCAL ro0060_OpenLog (
	signed         char       FAR  * fileName,     
	unsigned short int               fileMode,     
	unsigned short int               options,     
	unsigned long  int        FAR  * phFileHandle,
	void                FAR * FAR  * lpOpenBuff)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
signed   short int      sRetCode = OK;
char                    vFMode[4];

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0060)

switch (fileMode){
     case RC0025_NewFile: /* New file */
	   ro0050_strcpy ((char FAR *)vFMode, (char FAR *)"w");
	   break;

     case RC0026_ExtendFile: /*Old file open in Append */
	   ro0050_strcpy ((char FAR *)vFMode, (char FAR *)"a");
	   break;

     default:  /* New file */
	   ro0050_strcpy ((char FAR *)vFMode, (char FAR *)"w");
	   break;
}

sRetCode = ro00ff_Shfopen ((char FAR *)fileName,     (char FAR *)vFMode,
                           phFileHandle, lpOpenBuff);

RM0003_RETURN (rs0060, signed short int, sRetCode)
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0061_CloseLog                                     */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int FAR PASCAL ro0061_CloseLog(
	unsigned long  int       hFileHandle,
	void               FAR * lpOpenBuff,
	signed   short int FAR * pbCloseAll)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0061)

rc = ro00fj_Shfclose (hFileHandle, lpOpenBuff, pbCloseAll);

RM0003_RETURN (rs0061, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0062_WriteLog                                     */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-19                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int FAR PASCAL ro0062_WriteLog (
	unsigned long int             hFileHandle,
	void                    FAR * lpOpenBuff,
	rT0060_LogRecHead       FAR * pHeader,
	char                    FAR * pRecord) 

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

    /* call DCE-serviceability function instead of RTROS-logging function */
    DCE_SVC_LOG ((DCE_SVC (gds_svc_handle, "%b%b"), GDS_S_ROS,
       ((pHeader->lRecordType & RL1300_ROEXIF) == RL1300_ROEXIF) ? svc_c_debug1:
       ((pHeader->lRecordType & RL2000_CMX) == RL2000_CMX) ? svc_c_debug2 :
	svc_c_debug3, GDS_S_RTROS_LOG, sizeof (rT0060_LogRecHead), pHeader,
						   pHeader->lLength, pRecord)) ;
/*
    rc = ro00fi_Shfwrite ((char FAR *)pHeader,
                          (unsigned long int)sizeof(rT0060_LogRecHead),
                          (unsigned long int)1,
                          hFileHandle, lpOpenBuff);
if ((!rc) && pRecord )
    rc = ro00fi_Shfwrite (pRecord,
                          (unsigned long int)sizeof(char),
                          (unsigned long int)pHeader->lLength,
                          hFileHandle, lpOpenBuff);
*/

return (rc);

}/*end*/                                                                        

/* ****************** END of MODULE ************************************* */    
										
#endif  /* RC0061_ThroOSS */                                                     
										

/*EOF*/
