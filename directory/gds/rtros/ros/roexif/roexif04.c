/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif04.c,v $
 * Revision 1.1.4.2  1996/03/11  13:21:57  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:43  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:29  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:09:45  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:51:56  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:10  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:01:24  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:44  keutel
 * 	creation
 * 	[1994/03/21  13:30:26  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roexif04.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:57 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1993                     */
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
/* NAME         : roexif04.c                                          */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-24                                            */
/*                                                                    */
/* COMPONENT    : Remote Opeartions Service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Logging functions (logon,logoff,logwrite)           */
/*                with the ENTRY points from USER                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roexif04.c	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-02-25|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/* ************** Include files ************************************* */
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <ross.h>          /* Data related to logging defined         */
#include <rogenrl.h>	   /* ROS general definitions		      */
#include <roerval.h>	 /* ROS error values			     */
#include <rolgval.h>       /* Logging Constants defined               */
#include <routil0.h>       /* Utility Function defs                   */
#include <roexif08.h>      /* Function Prototype for Logging Functions*/


RM0001_STRG (rs1340, "roexlog     ")
RM0001_STRG (rs1341, "ro1341_LogOn       ")
RM0001_STRG (rs1342, "ro1342_LogOff      ")
RM0001_STRG (rs1343, "ro1343_LogWrite    ")

signed short int FAR PASCAL roexlog PROTO ((RO_excopb      FAR * pParlst));

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: roexlog				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


	signed short int FAR PASCAL roexlog (
         
             RO_excopb      FAR * pParlst)


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
 signed short int rc;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs1340)

     /* ************* check for VERSION  **************************** */
     if (pParlst->r_hdinfo.r_versnb != RO_VERSION) {
	    pParlst->r_hdinfo.r_errval = R_NSUPVS;
	    pParlst->r_hdinfo.r_erclas = RO_SYSERR;
	    pParlst->r_hdinfo.r_retcod = RO_PRMERR;
            RM0003_RETURN (rs1340, signed short int, -R_NSUPVS) 
     }
     switch (pParlst->r_hdinfo.r_fnctnb)
     {
     
     /* ************* Execute logging functions ********************* */
     /* *************				********************* */

     case RO_LOGON : 
                    rc = ro1341_LogOn (& pParlst->r_lgonpb);
		    break;

     case RO_LOGOF : 
                    rc = ro1342_LogOff ();
		    break;

     case RO_LOGWR : 
                    rc = ro1343_LogWrite (&pParlst->r_lgwrpb);
		    break;
     default	  :
	    pParlst->r_hdinfo.r_errval = R_FCTCAL;
	    pParlst->r_hdinfo.r_erclas = RO_SYSERR;
	    pParlst->r_hdinfo.r_retcod = RO_PRMERR;
                    rc = -R_FCTCAL;
		    break;
     }

RM0003_RETURN (rs1340, signed short int, rc)                                    
}/*end*/                                                                        


/* ***** Loging functions are called from roexif01.c      *********** */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1341_LogOn    				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

              signed short int NEAR PASCAL ro1341_LogOn (
         
               RO_lgonpb       FAR * pLogOnPb)

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
 signed   short int  rc;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs1341)
  rc = OK;

  /* --------- Check FMODE ------------------------------------------ */

  if ( (pLogOnPb->r_fmode != RC0025_NewFile ) &&
       (pLogOnPb->r_fmode != RC0026_ExtendFile ) )  rc = -R_LOGERR;
  else {
       if (!pLogOnPb->r_fname) rc = -R_LOGERR;
       else 
        rc = ro0040_openLog ( (char FAR *)pLogOnPb->r_fname        ,
                              (unsigned short int)pLogOnPb->r_fmode,
                              (unsigned short int)pLogOnPb->r_rcnum,
                              (unsigned short int)pLogOnPb->r_option);
  }

RM0003_RETURN (rs1341, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1342_LogOff   				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

              signed short int NEAR PASCAL ro1342_LogOff(void )
         

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
 signed   short int  rc;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs1342)
  rc = OK;
  rc = ro0041_closeLog(); 
RM0003_RETURN (rs1342, signed short int, rc)                                    
}/*end*/                                                                        



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1343_LogWrite 				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

              signed short int NEAR PASCAL ro1343_LogWrite(
               RO_lgwrpb       FAR * pLogWrPb)

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
 signed   short int  rc;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs1343)
  rc = OK;
  if (pLogWrPb->r_option & RC0024_ProvOptions) rc = -R_LOGERR;
  else if (pLogWrPb->r_reclen > RO_MAXREC    ) rc = -R_LOGERR;
  else if (!pLogWrPb->r_recptr)                rc = -R_LOGERR;
  else rc = ro0042_writeLog( (unsigned short int) pLogWrPb->r_option,
                             (signed long int )   pLogWrPb->r_rectyp,
                             (unsigned long int )pLogWrPb->r_reclen ,
                             (char FAR *   )pLogWrPb->r_recptr      );



RM0003_RETURN (rs1343, signed short int, rc)                                    
}/*end*/                                                                        









