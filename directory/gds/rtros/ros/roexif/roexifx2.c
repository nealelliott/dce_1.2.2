/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexifx2.c,v $
 * Revision 1.1.4.2  1996/03/11  13:22:05  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:44  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:09:59  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:06  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:19  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:02:11  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:39  keutel
 * 	creation
 * 	[1994/03/21  13:31:11  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roexifx2.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:05 $" } ;
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
/* NAME 	: roexifx2.c					      */
/*                                                                    */
/* AUTHOR       : Chandrasekar .K.                                    */
/* DATE 	: 93-02-23					      */
/*                                                                    */
/* COMPONENT    : ROS external interface                              */
/*                                                                    */
/* PRD#/VERS.	: ROS-V3.00					      */
/*                                                                    */
/* DESCRIPTION	: This modul contains the entry points to ROS-V3.0.   */
/*								      */
/*                                                                    */
/* SYSTEM DEPENDENCE :	SINIX 5.40         			      */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  | KZ | CR# FM# */
/*  3.00 K0 | 93-02-23|  original		       | kc |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*		Module Identification				      */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "";   /*  !!!                                  */

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <sys/types.h>   /* Needed for the getpid-call               */
#include <unistd.h>      /* Needed for the getpid call               */


#include <ross.h>	 /* ROS external interface IROS-3.00	     */
#include <rogenrl.h>     /* RTS general definitions                  */  
#include <roerval.h>     /*                                          */
#include <rolgval.h>     /*                                          */ 
#include <routil0.h>     /* Commen utilities                         */  
                                                                               
#include <roexif02.h>	 /* ROS external interface internal include  */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1320_taskId 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-02-23					      */
/*                                                                    */
/* SYNTAX       :                                                     */

signed long int FAR PASCAL ro1320_taskId (void)

/*                                                                    */
/* DESCRIPTION	: Makes the system call to get the task/process id.   */
/*                                                                    */
/* IN-PARAMETERS:             : none                                  */
/*                                                                    */
/* OUT-PARAMETERS             : none                                  */
/*                                                                    */
/* RETURN VALUES              :task/process id.                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  System  dependent.SINIX 5.40         */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

return ( getpid());

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1321_ssiMpi 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE 	: 93-02-23					      */
/*                                                                    */
/* SYNTAX       :                                                     */

FARPROC   	FAR PASCAL ro1321_ssiMpi (

			   FARPROC          fct,
                           signed long int  mHd)
/*                                                                    */
/* DESCRIPTION	: mHd is the Dummy parameters to keep the function    */
/*                compatibility with WINDOWS                          */
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
/* REMARKS                    :  System dependent SINIX 5.40          */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

return (fct);

}/*end*/
