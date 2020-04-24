/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spstep5.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:56  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:04  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:31:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:08  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:07:04  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:45  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:36  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:54:07  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:12  keutel
 * 	creation
 * 	[1994/03/21  13:23:50  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spstep5.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:56 $" } ;
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
/* NAME         : spstep5.c                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 03-11-93                                            */
/*                                                                    */
/* COMPONENT    : STEP for the Abort Reasons of different PMs.        */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Abort reasons of different PMs.                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)spstep5.c	1.2 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |03-11-93|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/* ******************************************************************* */
/*           I n c l u d e  F i l e s                                  */
/* ******************************************************************* */

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rouply0.h>
#include <rouply1.h>
#include <routil0.h>
#include <spstep0.h>
 
#include <routil6.h>      /* for login data definitions */

#include <rotpm00.h>
#include <rospm00.h>
#include <roppm00.h>
#include <roacpm0.h>
#include <roropm0.h>
#include <rortpm0.h>
 
 
/*************************************************************************/
/*                     Local Constants and Macros                        */
/*************************************************************************/
/*************************************************************************/
void sp1291_TpmReason PROTO ((
unsigned long int       ulVal
));

void sp1292_SpmReason PROTO ((
unsigned long int       ulVal
));

void sp1293_PpmReason PROTO ((
unsigned long int       ulVal
));

void sp1294_AcpmReason PROTO ((
unsigned long int       ulVal
));

void sp1295_RopmReason PROTO ((
unsigned long int       ulVal
));

/*************************************************************************/
/***                                                                   ***/
/***                        Global Functions                           ***/
/***                                                                   ***/
/*************************************************************************/
/*************************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1290_writeAbortReason                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-08-30                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

void sp1290_writeAbortReason(
	unsigned long int          ulVal)

/*                                                                    */
/* DESCRIPTION  :  This function evaluates the Abort reasons.         */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
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
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

/* ****************************************************************** */


 sp1257_wrUnLong(RC1201_OFFSET1,"Abort Reason   : ",
                 (unsigned long int FAR *)&ulVal,
                 RC1208_HEX_NL);

switch(ulVal & 0x0000ff00){
   case RC0900_TpmEvent:
         sp1264_write(RC1202_OFFSET2,"%s %s\n","Protocol Machine :","TPM",NULL,
                       NULL);
         sp1291_TpmReason(ulVal);
         break;
   case RC0800_SpmEvent:
         sp1264_write(RC1202_OFFSET2,"%s %s\n","Protocol Machine :","SPM",NULL,
                       NULL);
         sp1292_SpmReason(ulVal);
         break;
   case RC0400_PpmEvent:
         sp1264_write(RC1202_OFFSET2,"%s %s\n","Protocol Machine :","PPM",NULL,
                       NULL);
         sp1293_PpmReason(ulVal);
         break;
   case RC0600_AcpmEvent:
         sp1264_write(RC1202_OFFSET2,"%s %s\n","Protocol Machine :","ACPM",NULL,
                       NULL);
         sp1294_AcpmReason(ulVal);
         break;
   case RC0700_RoseEvent:
         sp1264_write(RC1202_OFFSET2,"%s %s\n","Protocol Machine :","ROPM",NULL,
                       NULL);
         sp1295_RopmReason(ulVal);
         break;
   default:
         sp1264_write(RC1202_OFFSET2,"%s %s\n","Protocol Machine :",
                      "NO-PM",NULL, NULL);
         break;
 }

}/*end*/                                                                        



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1291_TpmReason                                    */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1291_TpmReason(
	unsigned long int       ulVal)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
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
unsigned long int   ulLoc;
unsigned long int   ulReason;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  ulLoc = ulVal & 0x000000ff;
  ulReason = ((ulVal & 0xffff0000) >> 16 );
  switch(ulLoc ){
    case 0:
      sp1257_wrUnLong(RC1202_OFFSET2,"Location  : ",
                   (unsigned long int FAR *)&ulLoc,RC1207_HEX);
      sp1264_write(RC1200_OFFSET0,"%s \n"," -CMX Abort ",
                      NULL,NULL, NULL);
      sp1257_wrUnLong(RC1202_OFFSET2,"Reason    : ",
                   (unsigned long int FAR *)&ulReason,RC1207_HEX);
      sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -CMX Reason(see CMX INTERFACE LOGGING) ",
                      NULL,NULL, NULL);
      break;
    case 1:
      sp1257_wrUnLong(RC1202_OFFSET2,"Location  : ",
                   (unsigned long int FAR *)&ulLoc,RC1207_HEX);
      sp1264_write(RC1200_OFFSET0,"%s \n"," -Local Abort ",
                      NULL ,NULL, NULL);
      sp1257_wrUnLong(RC1202_OFFSET2,"Reason    : ",
                   (unsigned long int FAR *)&ulReason,RC1207_HEX);
      switch(ulVal){
          case RC0302_OtherError:
              sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -OtherError       ",
                      NULL,NULL, NULL);
              break;
          case RC0303_CMXError:
              sp1264_write(RC1200_OFFSET0,"%s \n",
                      " - CMX Error       ",
                      NULL,NULL, NULL);
              break;
          case RC0304_SysError:
              sp1264_write(RC1200_OFFSET0,"%s \n",
                      " - SYS Error       ",
                      NULL,NULL, NULL);
              break;
          default:
              sp1264_write(RC1200_OFFSET0,"%s \n",
                      " - UNKNOWN         ",
                      NULL,NULL, NULL);
              break;
             
      }
               
      break;
  }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1292_SpmReason                                    */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1292_SpmReason(
	unsigned long int       ulVal)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
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
unsigned long int   ulLoc;
unsigned long int   ulReason;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  ulLoc = ulVal & 0x000000ff;
  ulReason = ((ulVal & 0xffff0000) >> 16 );
  switch(ulLoc ){
    case 0:
      sp1257_wrUnLong(RC1202_OFFSET2,"Location  : ",
                   (unsigned long int FAR *)&ulLoc,RC1207_HEX);
      sp1264_write(RC1200_OFFSET0,"%s \n"," -Remote Abort ",
                      NULL,NULL, NULL);
      sp1257_wrUnLong(RC1202_OFFSET2,"Reason    : ",
                   (unsigned long int FAR *)&ulReason,RC1207_HEX);
      switch (ulVal){
          case RC0311_TransportDisconnect:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -Transport Disconnect  ", NULL,NULL, NULL);
             break;
          case RC0312_ProtocolError:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -ProtocolError ", NULL,NULL, NULL);
             break;
          case RC0314_UndefinedReason:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -UndefinedReason  ", NULL,NULL, NULL);
             break;
          case RC0313_RestrictionInPICS:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -RestrictionInPICS   ", NULL,NULL, NULL);
             break;
          default:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -OSS Reason(see OSS LOGGING) ", NULL,NULL, NULL);
             break;
      }
      break;
    case 1:
      sp1257_wrUnLong(RC1202_OFFSET2,"Location  : ",
                   (unsigned long int FAR *)&ulLoc,RC1207_HEX);
      sp1264_write(RC1200_OFFSET0,"%s \n"," -Local Abort ",
                      NULL ,NULL, NULL);
      sp1257_wrUnLong(RC1202_OFFSET2,"Reason    : ",
                   (unsigned long int FAR *)&ulReason,RC1207_HEX);
      switch(ulVal){
          case RC0322_ConrqRetryReturn:
              sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -ConrqRetryReturn       ", NULL,NULL, NULL);
              break;
          case RC0321_UdataTooLong:
              sp1264_write(RC1200_OFFSET0,"%s \n",
                      " - UdataTooLong       ", NULL,NULL, NULL);
              break;
          case RC0323_CmxError:
              sp1264_write(RC1200_OFFSET0,"%s \n",
                      " - CmxError       ", NULL,NULL, NULL);
              break;
          case RC0324_SysOssError:
              sp1264_write(RC1200_OFFSET0,"%s \n",
                      " - SysOssError       ", NULL,NULL, NULL);
              break;
          case RC0325_OssError:
              sp1264_write(RC1200_OFFSET0,"%s \n",
                      " - OssError       ", NULL,NULL, NULL);
              break;
          default:
              sp1264_write(RC1200_OFFSET0,"%s \n",
                      " - UNKNOWN       ", NULL,NULL, NULL);
              break;
           
      }
               
      break;
  }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1293_PpmReason                                    */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 03-11-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1293_PpmReason(
	unsigned long int       ulVal)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
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

unsigned long int   ulLoc;
unsigned long int   ulReason;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  ulLoc = ulVal & 0x000000ff;
  ulReason = ((ulVal & 0xffff0000) >> 16 );
  sp1257_wrUnLong(RC1202_OFFSET2,"Location  : ",
                   (unsigned long int FAR *)&ulLoc,RC1207_HEX);
  switch(ulLoc ){
    case 0:
      sp1264_write(RC1200_OFFSET0,"%s \n"," -Abort by Remote Provider ",
                      NULL,NULL, NULL);
      break;
    case 1:
      sp1264_write(RC1200_OFFSET0,"%s \n"," -Abort by Local Provider ",
                      NULL ,NULL, NULL);
      break;
  }
  sp1257_wrUnLong(RC1202_OFFSET2,"Reason    : ",
                   (unsigned long int FAR *)&ulReason,RC1207_HEX);
  ulReason = ( ulVal & 0x00ff0000);
  switch (ulReason){
     case 0x00000000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -Reason Not Specified  ", NULL,NULL, NULL);
          break;
     case 0x00010000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -Unrecognized PPDU ", NULL,NULL, NULL);
          break;
     case 0x00020000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -Unexpected PPDU  ", NULL,NULL, NULL);
          break;
     case 0x00030000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -Unexpected session service primitive   ", 
                       NULL,NULL, NULL);
          break;
     case 0x00040000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -Unrecognized PPDU parameter ", NULL,NULL, NULL);
          break;
     case 0x00050000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -Unexpected PPDU parameter ", NULL,NULL, NULL);
          break;
     case 0x00510000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -Session user-data too long  ", NULL,NULL, NULL);
          break;
     case 0x00530000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -System call error within OSS  ", NULL,NULL, NULL);
          break;
     case 0x00540000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -CMX error  ", NULL,NULL, NULL);
          break;
     case 0x00550000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -OSS error  ", NULL,NULL, NULL);
          break;
     case 0x00600000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -Wrong presentation-context-definition-list", NULL,NULL, NULL);
          break;
     case 0x00610000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " - Wrong default abstract syntax  ", NULL,NULL, NULL);
          break;
     case 0x00620000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -Wrong default transfer syntax  ", NULL,NULL, NULL);
          break;
     case 0x00630000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -Wrong presentation-context-definition-result-list",
                      NULL,NULL, NULL);
          break;
     case 0x00640000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -Wrong abstract syntax identifier",
                      NULL,NULL, NULL);
          break;
     case 0x00650000:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " - Encoding error",
                      NULL,NULL, NULL);
          break;
     default:
          sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -No-Reasson found ", NULL,NULL, NULL);
          break;
      }
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1294_AcpmReason                                    */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 29-10-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1294_AcpmReason(
	unsigned long int       ulVal)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
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
unsigned long int ulLoc;
unsigned long int ulReason;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  ulLoc = (ulVal & 0x000000ff);
  ulReason = (ulVal & 0xffff0000) >> 16;
  sp1257_wrUnLong(RC1202_OFFSET2,"Location  : ",
                   (unsigned long int FAR *)&ulLoc,RC1207_HEX);
  switch(ulVal & 0x000000ff ){
    case 0:
      sp1264_write(RC1200_OFFSET0,"%s \n"," - UserAbort ",
                      NULL,NULL, NULL);
      break;
    case 1:
      sp1264_write(RC1200_OFFSET0,"%s \n"," - ProviderAbort ",
                      NULL,NULL, NULL);
      break;
  }
  sp1257_wrUnLong(RC1202_OFFSET2,"Reason    : ",
                   (unsigned long int FAR *)&ulReason,RC1207_HEX);
  switch (ulVal){
          case RC0610_AA_UserABORT:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -RC0610_AA_UserABORT  ", NULL,NULL, NULL);
             break;
          case RC0611_AA_ProvABORT:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -RC0611_AA_ProvABORT ", NULL,NULL, NULL);
             break;
          case RC0612_AAREapduError:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -RC0612_AAREapduError  ", NULL,NULL, NULL);
             break;
          case RC0613_RLRQapduError:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -RC0613_RLRQapduError   ", NULL,NULL, NULL);
             break;
          case RC0614_RLREapduError:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -RC0614_RLREapduError   ", NULL,NULL, NULL);
             break;
          case RC0615_ABRTapduError:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -RC0615_ABRTapduError   ", NULL,NULL, NULL);
             break;
          case RC0616_PROTsequError:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -RC0616_PROTsequError   ", NULL,NULL, NULL);
             break;
          default:
             sp1264_write(RC1200_OFFSET0,"%s \n",
                      " -No Reason found ", NULL,NULL, NULL);
             break;
      }
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp1295_RopmReason                                    */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 03-11-93                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
void sp1295_RopmReason(
	unsigned long int       ulVal)
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   Void                                */
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
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
unsigned long int   ulLoc;
unsigned long int   ulReason;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */
  ulLoc = ulVal & 0x000000ff;
  ulReason = ((ulVal & 0xffff0000) >> 16 );
  sp1257_wrUnLong(RC1202_OFFSET2,"Location  : ",
                   (unsigned long int FAR *)&ulLoc,RC1207_HEX);
  sp1264_write(RC1200_OFFSET0,"%s \n"," -No Info ",
                      NULL,NULL, NULL);

  sp1257_wrUnLong(RC1202_OFFSET2,"Reason    : ",
                   (unsigned long int FAR *)&ulReason,RC1207_HEX);
  sp1264_write(RC1200_OFFSET0,"%s \n"," -No Info ",
                      NULL,NULL, NULL);

}/*end*/


