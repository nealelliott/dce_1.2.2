/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm02.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:18  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:42  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:13:59  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:22  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:32  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:07:55  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:45:58  keutel
 * 	creation
 * 	[1994/03/21  13:36:28  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rospm02.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:18 $" } ;
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
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : rospm02.c                                           */
/*                                                                    */
/* AUTHOR       : Milind M.Aphale                                     */
/* DATE         : 93-03-17                                            */
/*                                                                    */
/* COMPONENT    : RTROS Convergence protocol machine                  */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : provides the decode functions of the SPDU           */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-03-17|  original                      |ma  |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "@(#) rospm02.c 1.1 93/11/29 RTROS-V3.00";

/*********************************************************************/
/*                                                                   */
/*              I N C L U D E S                                      */
/*                                                                   */
/*********************************************************************/


#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>    /* Include of ROS global defines              */
#include <rouply0.h>    /* Common generation constants,types & values */
#include <rouply1.h>    /* Common generation constants,types & values */
#include <routil0.h>    /* Common utility functions prototypings      */

#include <rospm01.h>    /* Defines for coding functions               */
#include <rospm03.h>    /* Defines for convergence protocol m/c       */  
#include <rospm07.h>    /* Prototyping of convergence protocol m/c    */    
#include <roendeco.h>   /* Macros for encoding/decoding of Byte strings*/
#include <rospm05.h>    /* Prototyping of coding of functions         */ 
#include <rospm06.h>    /* Prototyping of convergence protocol m/c    */    

/*********************************************************************/
/*                                                                   */
/*              G L O B A L   V A R I A B L E S                      */
/*                                                                   */
/*********************************************************************/

/*********************************************************************/
/*                                                                   */
/*              G L O B A L   F U N C T I O N S                      */
/*                                                                   */
/*********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro082A_WhichSpdu                                    */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-17                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */

signed short int PASCAL ro082A_WhichSpdu (
char   spdu)

/* DESCRIPTION  :      decodes the incoming SPDU                      */
/*                                                                    */
/* IN-PARAMETERS      :                                               */
/*                                                                    */
/*    spdu   =    This byte indicates the type of SPDU                */
/*                                                                    */
/* FORMAL OUT-PARAMETERS : none                                       */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      return the value assigned to corresponding SPDU.              */
/*      ( see rospm01.h for further reference )                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/
{/*entr-ro082A_WhichSpdu*/

signed short int retvalue;
 switch ( spdu )
 { case  1   : retvalue = RC08em_ADT;
	       break;
   case  9   : retvalue = RC08ef_AFN;
	       break;
   case  10  : retvalue = RC08ej_ADN;
	       break;
   case  12  : retvalue = RC08eb_ARF;
	       break;
   case  13  : retvalue = RC08e7_ACN;
	       break;
   case  14  : retvalue = RC08ea_AAC;
	       break;
/*******
   case  15  : retvalue = RC08e8_ACDO;
	       break;
   case  16  : retvalue = RC08e9_AOA;
	       break;
*******/
   case  25  : retvalue = RC08ei_AAB;
	       break;
   case  26  : retvalue = RC08eg_AAA;
	       break;
   default   : retvalue = RC08eo_BadEvent;
	       break;
 }
 return ( retvalue );

}/*end-ro082A_WhichSpdu*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro082B_InitStruc                                    */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-05-10                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */

void PASCAL ro082B_InitStruc( 
void FAR *vPtr,
unsigned long int ulLength)

/*                                                                    */
/* DESCRIPTION    :  Fills a memory area with a NULL                  */
/*                                                                    */
/* IN-PARAMETERS  : vPtr      - Start of the Area.                    */
/*                  ulLength  - Number of bytes to fill with NULL.    */
/*                                                                    */
/* FORMAL OUT-PARAMETERS : none                                       */
/*                                                                    */
/* RETURN VALUES  : none                                              */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/
{/*entr-ro082B_InitStruc*/

  ro0043_memset((char FAR *)vPtr , (int)0 , (unsigned long int)ulLength); 

}/*end-ro082B_InitStruc*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0821_DecodeAB                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-17                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short  int ro0821_DecodeAB(
rT0811_stAB FAR *stAB)

/* DESCRIPTION  :   Decodes the incoming Abort  SPDU                  */
/*                                                                    */
/* IN-PARAMETERS   :                                                  */
/*                                                                    */
/*      stAB    =   coading structure for ABORT SPDU                  */
/*                                                                    */
/* FORMAL OUT-PARAMETERS :                                            */
/*                                                                    */
/*      stAB    =   coading structure for ABORT SPDU                  */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr-ro0821_DecodeAB*/

unsigned char pgi;
signed short int totlen ,len,j;
unsigned char FAR * pointer;
unsigned short int usDecodLen;  
  stAB->usFunctionalUnits = RC08ca_FUValue; 
  pointer = (unsigned char FAR *)stAB->pDataptr+1;

/****************** Decode AB Total Blength ******************************/

 if (*pointer < 255)
   totlen = *pointer++;
 else
 {
   pointer++;
   DECODE(usDecodLen,pointer,short);
   totlen = usDecodLen;
 }
 
 stAB->lDatalen = 0;
 
 while (totlen > 0)
 {
   pgi = *pointer++;totlen--;
   switch (pgi)
   {
   case RC08ba_PITransportDisconnect :        /* Transport Disconnect  */    

                 if (*pointer++ != 1)    
                   return(-1);
                 stAB->usTdiscAreuse     = ((*pointer & 1) != 1 );
                 stAB->usTdiscUserAbort = ((*pointer & 2) == 2 );
                 stAB->usTdiscProtAbort = ((*pointer & 4) == 4 );
                 stAB->usTdiscNoReason  = ((*pointer & 8) == 8 );
                 pointer +=  1; totlen  -=  2;
                 break;

   case RC08bl_PIEnclosureItem:               /*  Enclosure Item       */    

                 if (*pointer++ != 1)   
                     return(-1);
                 stAB->ulEnclosure = (*pointer & 3);
                 pointer +=  1; totlen  -=  2;
                 break;

   case RC08bt_PIReflectParamVal:            /* Reflect Parameter Values  */

                  len = *pointer++; totlen--;
                  stAB->sReflParamLen = len;   
                  for (j=0;j<len;)
                  stAB->ucReflParam[j++]=*pointer++;
                  totlen  -=  len;
                  break;
 
   case RC08a5_PGIUserData:                    /*   User Data              */

                if (*pointer < 255)      
                {
                  stAB->lDatalen = *pointer++;totlen--;
                }
                else
                { 
                  pointer++;totlen -= 3;
                  DECODE(usDecodLen,pointer,short);
                  stAB->lDatalen = usDecodLen;
                }
                stAB->pDataptr = (char FAR *)pointer;
                pointer +=  stAB->lDatalen ;
                totlen = (signed short int)(stAB->lDatalen - totlen); 
                break;
   default:     return(-1);
   }
 }
return(0); 
}/*end-ro0821_DecodeAB*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0822_DecodeAC                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-17                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short  int ro0822_DecodeAC(
rT0812_stAC FAR *stAC)

/* DESCRIPTION  :   Decodes incomming Accept SPDU                     */
/*                                                                    */
/* IN-PARAMETERS      :                                               */
/*                                                                    */
/*      stAC    =   coading structure for Accept SPDU                 */
/*                                                                    */
/* FORMAL OUT-PARAMETERS  :                                           */
/*                                                                    */
/*      stAC    =   coading structure for Accept SPDU                 */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):                                       */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :                                       */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr-ro0822_DecodeAC*/

  unsigned char pgi,pi;
  signed short int totlen,len,pgilen;
  unsigned char FAR * pointer;
  unsigned short int usDecodLen;  
  stAC->usFunctionalUnits = RC08ca_FUValue;  
  pointer = (unsigned char FAR *)stAC->pDataptr+1;

/****************** Decode AC Total Blength ******************************/

 if (*pointer < 255)
   totlen = *pointer++;
 else
 {
   pointer++;
   DECODE(usDecodLen,pointer,short);
   totlen = usDecodLen;
 }
 
 stAC->lDatalen = 0;
 
 while (totlen > 0)
 {
   pgi = *pointer++;totlen--;
   switch (pgi)
   {
   case RC08a0_PGIConnectionIdentifier:       /*  Connection Identifier  */
     pgilen = *pointer++;totlen--;
     while (pgilen > 0)
     {
       pi = *pointer++;totlen--;pgilen--;
       switch (pi)
       {
       case RC08b6_PICallingSSUserRef:  /* Calling user reference */
                 len = *pointer;
                 pointer +=  len+1; totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       case RC08b7_PICommonRef:         /*  Common Reference       */ 
                 len = *pointer;    
                 pointer +=  len+1; totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       case RC08b8_PIAddRefInfo:      /* Additional Reference    */
                 len = *pointer; 
                 pointer +=  len+1; totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       default:  return(-1);
                 /* break; */
       }
     }
     break;

   case RC08a1_PGIConnectSAcceptItem:       /*   Connect/Accept Item     */
     pgilen = *pointer++;totlen--;
     while (pgilen > 0)
     {
       pi = *pointer++;
       totlen--; pgilen--;
       switch (pi)
       {
       case RC08bb_PIProtocolOptions:        /* Protocol Options         */
                if (*pointer++ != 1)
                   return(-1);                 
                 stAC->usPopt = (*pointer & 1);
                 pointer +=  1; totlen  -=  2; pgilen  -=  2;
                 break;

       case RC08bi_PITSDUMaxsize:        /*  TSDU Maximum Size         */ 
                if (*pointer++ != 4) 
                   return(-1);
                 DECODE(usDecodLen,pointer,short);
                 stAC->ulTSDUmaxto   = usDecodLen;

                 DECODE(usDecodLen,pointer,short);
                 stAC->ulTSDUmaxfrom = usDecodLen;
                 totlen  -=  5; pgilen  -= 5;
                 break;

       case RC08bj_PISessionVersionNo:       /*  version Number           */
                if (*pointer++ != 1)    
                   return(-1);
                 stAC->usVersion = *pointer;
                 pointer += 1 ; totlen  -= 2  ; pgilen  -= 2 ;
                 break;

       case RC08bk_PIInitialSerialNo:       /*  Initial serial Number    */
                 len = *pointer;       
                 pointer +=  len+1; totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       case RC08bm_PITokenSettingItem:       /* Token Setting Item        */ 
                 len = *pointer;        
                 pointer +=  len+1; totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       case RC08bE_PISecondInitialSerialNo:  /* Second Initial Serial No  */   
                 len = *pointer;        
                 pointer +=  len+1; totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       default:  return(-1);
                 /* break; */
       }
     }
     break;

   case RC08b9_PITokenItem:                    /*  Token Item             */  
                 len = *pointer;     
                 pointer +=  len+1; totlen  -=  len+1;                     
                 break;

   case RC08bh_PISessionUserRequirements:     /* Session User Requirements */ 
                if (*pointer++ != 2)     
                   return(-1);
                 DECODE(usDecodLen,pointer,short) ;
                 stAC->usFunctionalUnits = usDecodLen;
                 totlen  -=  3;
                 break;

   case RC08bl_PIEnclosureItem:                 /* Enclosure Item           */
               if (*pointer++ != 1)       
                 return(-1);
                stAC->ulEnclosure = (*pointer & 3);
                pointer +=  1; totlen  -=  2;
                break;

   case RC08bB_PICallingSessionSelector:        /* Calling Session Selector  */
                len = *pointer++; totlen--;  
 		stAC->sCallingLen = len;
                ro0041_memcpy ((char FAR *)stAC->vCalling,
                               (char FAR *)pointer,
                               (unsigned long int)len );
                pointer +=  len; totlen  -=  len;
                break;

   case RC08bD_PIRespondingSessionSelector:    /* Responding session Selector */
                len = *pointer++;  totlen--;  
                stAC->sRespondingLen = len;
                ro0041_memcpy ((char FAR *)stAC->ucResponding, 
                               (char FAR *)pointer, 
                               (unsigned long int)len );
                pointer +=  len; totlen  -=  len;
                break;

   case RC08a5_PGIUserData:                   /* User Data                   */
                if (*pointer < 255)         
                {
                  stAC->lDatalen = *pointer++;totlen--;
                }
                else
                { 
                  pointer++;totlen -= 3;
                  DECODE(usDecodLen,pointer,short);
                  stAC->lDatalen = usDecodLen;
                }
                stAC->pDataptr = (char FAR *)pointer;
                pointer +=  stAC->lDatalen ;
                totlen = (signed short int)(stAC->lDatalen - totlen); 
                break;

   default:     return(-1);
   }
 }
return(0); 
}/*end-ro0822_DecodeAC*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0824_DecodeCN                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-17                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short  int ro0824_DecodeCN(
rT0814_stCN FAR *stCN)

/* DESCRIPTION  :   Decodes the incoming Connect SPDU                 */
/*                                                                    */
/* IN-PARAMETERS         :                                            */
/*                                                                    */
/*      stCN    =   coading structure for Connect  SPDU               */
/*                                                                    */
/* FORMAL OUT-PARAMETERS  :                                           */
/*                                                                    */
/*      stCN    =   coading structure for Connect  SPDU               */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/
{/*entr-ro0824_DecodeCN*/

  unsigned char pgi,pi;
  signed short int totlen ,len,pgilen;
  unsigned  char FAR * pointer;
  unsigned short int usDecodLen; 
  pointer = (unsigned char FAR *)stCN->pDataptr+1;

/****************** Decode CN Total Blength ******************************/

 if (*pointer < 255)
   totlen = *pointer++;
 else
 {
   pointer++;
   DECODE(usDecodLen,pointer,short);
   totlen = usDecodLen;
 }
 
 stCN->lDatalen = 0;
 
 while (totlen > 0)
 {
   pgi = *pointer++;totlen--;
   switch (pgi)
   {
   case RC08a0_PGIConnectionIdentifier :          /*  Connection Identifier  */

     pgilen = *pointer++;totlen--;
     while (pgilen > 0)
     {
       pi = *pointer++;totlen--;pgilen--;
       switch (pi)
       {
       case RC08b6_PICallingSSUserRef :    /* Calling User Reference */
                 len = *pointer;                                         
                 pointer +=  len+1; totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       case RC08b7_PICommonRef :            /* common Reference      */ 
                 len = *pointer;        
                 pointer +=  len+1; totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       case RC08b8_PIAddRefInfo :         /* Additional Reference   */
                 len = *pointer;                                         
                 pointer +=  len+1; totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       default:  return(-1);
                 /* break; */
       }
     }
     break;
 
   case RC08a1_PGIConnectSAcceptItem :           /*   Connect/Accept Item     */

     pgilen = *pointer++;totlen--;
     while (pgilen > 0)
     {
       pi = *pointer++;
       totlen--; pgilen--;
       switch (pi)
       {
       case RC08bb_PIProtocolOptions:            /* Protocol Options          */
                if (*pointer++ != 1)
                   return(-1);                 
                 stCN->usPopt = (*pointer & 1);
                 pointer +=  1; totlen  -=  2; pgilen  -=  2;
                 break;

       case RC08bi_PITSDUMaxsize :              /* TSDU Maximum Size          */
                if (*pointer++ != 4)  
                   return(-1);
                 DECODE(usDecodLen,pointer,short);
                 stCN->ulTSDUmaxto   = usDecodLen;
 
                 DECODE(usDecodLen,pointer,short);
                 stCN->ulTSDUmaxfrom = usDecodLen;
                 totlen  -=  5; pgilen  -= 5;
                 break;
       
       case RC08bj_PISessionVersionNo :       /*  Version Number            */ 
                 if (*pointer++ != 1)    
                   return(-1);
                 stCN->usVersion = *pointer;
                 pointer += 1 ; totlen  -= 2  ; pgilen  -= 2 ;
                 break;

       case RC08bk_PIInitialSerialNo :        /*  Initial Serial Number    */
                 len = *pointer;       
                 pointer +=  len+1; totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       case RC08bm_PITokenSettingItem :       /*  Token Setting Item       */
                 len = *pointer;        
                 pointer +=  len+1; totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       case RC08bE_PISecondInitialSerialNo : /* Second Initial serial No. */
                 len = *pointer;         
                 pointer +=  len+1; totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       default:  return(-1);
                 /* break; */
       }
     }
     break;

   case RC08bh_PISessionUserRequirements :    /* Session User Requirements   */
                 if (*pointer++ != 2)   
                   return(-1);
                 DECODE(usDecodLen,pointer,short);
                 stCN->usFunctionalUnits = usDecodLen;
                 totlen  -=  3;
                 break;

   case RC08bB_PICallingSessionSelector :       /* Calling Session Selector   */
                 len = *pointer++; totlen--;
                 ro0041_memcpy ( (char FAR *)stCN->vCalling, 
			         (char FAR *)pointer, 
                                 (unsigned long int)len );
                 stCN->sCallingLen  = len;
                 pointer +=  len; totlen  -=  len;
                 break;

   case RC08bC_PICalledSessionSelector :      /* Called Session selector     */
                len = *pointer++;  totlen--; 
                ro0041_memcpy ((char FAR *)stCN->vCalled,
                               (char FAR *)pointer, 
                               (unsigned long int)len );
                stCN->sCalledLen = len; 
                pointer +=  len; totlen  -=  len;
                break;

   case RC08a5_PGIUserData :                 /*  User Data                   */  
                if (*pointer < 255)        
                {
                  stCN->lDatalen = *pointer++;totlen--;
                }
                else
                { 
                  pointer++;totlen -= 3;
                  DECODE(usDecodLen,pointer,short);
                  stCN->lDatalen = usDecodLen;
                }
                stCN->pDataptr = (char FAR *)pointer;
                pointer +=  stCN->lDatalen ;
                totlen = (signed short int)(stCN->lDatalen - totlen); 
                break;

   case RC08bI_PIDataOverflow :           /* Data Overflow                  */
                if (*pointer++ != 1) 
                 return(-1);
                stCN->usOverflow = (*pointer);
                pointer +=  1; totlen  -=  2;
                break;

   case RC08a6_PGIExtendedUserData :       /* Extended User Data             */
               if (*pointer < 255) 
                {
                  stCN->lDatalen = *pointer++;totlen--;
                }
                else
                { 
                  pointer++;totlen -= 3;
                  DECODE(usDecodLen,pointer,short);
                  stCN->lDatalen = usDecodLen;

                }
                   stCN->pDataptr = (char FAR *)pointer;
                   pointer += stCN->lDatalen;
                   totlen = (signed short int)(stCN->lDatalen - totlen); 
                   break;

   default:     return(-1);
   }
 }

return(0); 
}/*end-ro0824_DecodeCN*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0825_DecodeDN                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-17                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short  int ro0825_DecodeDN(
rT0815_stDN  FAR *stDN)

/* DESCRIPTION  :   Decodes the incoming Disconnect SPDU              */
/*                                                                    */
/* IN-PARAMETERS            :                                         */
/*                                                                    */
/*      stDN    =   coading structure for Disconnect SPDU             */
/*                                                                    */
/* FORMAL OUT-PARAMETERS    :                                         */
/*                                                                    */
/*      stDN    =   coading structure for Disconnect SPDU             */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/
{/*entr-ro0825_DecodeDN*/

  unsigned char pgi;
  signed short int totlen;
  unsigned  char FAR * pointer;
  unsigned short int usDecodLen; 
  stDN->usFunctionalUnits  = RC08ca_FUValue;
  pointer = (unsigned char FAR *)stDN->pDataptr+1;

/****************** Decode DN Total Blength ******************************/

 if (*pointer < 255)
   totlen = *pointer++;
 else
 {
   pointer++;
   DECODE(usDecodLen,pointer,short);
   totlen = usDecodLen;
 }
 
 stDN->lDatalen = 0;
 
 while (totlen > 0)
 {
   pgi = *pointer++;totlen--;
   switch (pgi)
   {
   case RC08bl_PIEnclosureItem:                 /* Enclosure Item     */ 
               if (*pointer++ != 1)       
                 return(-1);
                stDN->ulEnclosure = (*pointer & 3);
                pointer +=  1; totlen  -=  2;
                break;

   case RC08a5_PGIUserData:                      /* User Data         */
                if (*pointer < 255)      
                {
                  stDN->lDatalen = *pointer++;totlen--;
                }
                else
                { 
                  pointer++;totlen -= 3;
                  DECODE(usDecodLen,pointer,short);
                  stDN->lDatalen = usDecodLen;
                }
                stDN->pDataptr = (char FAR *)pointer;
                pointer +=  stDN->lDatalen ;
                totlen = (signed short int)(stDN->lDatalen - totlen); 
                break;

   default:     return(-1);
   }
 }
return(0); 
}/*end-ro0825_DecodeDN*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0826_DecodeDT                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-17                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */

signed short int ro0826_DecodeDT(
rT0816_stDT FAR *stDT)

/*                                                                    */
/* DESCRIPTION  :   Decodes the incoming DATA TRANSFER SPDU           */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*      stDT    =   coading structure for Data Transfer SPDU          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
/*                                                                    */
/*      stDT    =   coading structure for Data Transfer SPDU          */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/
{/*entr-ro0826_DecodeDT*/

  signed short ret = 0;
  char FAR * pointer;
   
 stDT->usFunctionalUnits = RC08ca_FUValue;
 pointer = stDT->pDataptr;

/****************** Decode DT Total Blength ***************************/

 if (( *pointer++ == 1) && (*pointer++ == 0) && (*pointer++ == 1))
 {
   if (*pointer == 3)
   {
     pointer++;
     if ((*pointer++ == 25) && (*pointer++ == 1))      /* Enclosure Item */
     {
        stDT->ulEnclosure = (*pointer++ & 3);
        stDT->pDataptr = pointer;
        stDT->lDatalen -= 7;
     }   
     else
      return(-1);
   }
   else
   {
     if (*pointer == 0)
     {
       stDT->pDataptr = ++pointer;
       stDT->lDatalen -= 4;
     }
     else
       return(-1);
   }
 }
 else
   return(-1);

 return(ret);
}/*end-ro0826_DecodeDT*/ 



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0827_DecodeFN                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-17                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short  int ro0827_DecodeFN(
rT0817_stFN FAR *stFN)

/* DESCRIPTION  : Decodes the incoming Finish SPDU                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*      stFN    =   coading structure for Finish SPDU                 */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
/*                                                                    */
/*      stFN    =   coading structure for Finish SPDU                 */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/
{/*entr-ro0827_DecodeFN*/

  unsigned char pgi;
  signed short int totlen;
  unsigned  char FAR * pointer;
  unsigned short int usDecodLen;
 
  stFN->usFunctionalUnits = RC08ca_FUValue; 
  pointer = (unsigned char FAR *)stFN->pDataptr+1;

/****************** Decode FN Total Blength ******************************/

 if (*pointer < 255)
   totlen = *pointer++;
 else
 {
   pointer++;
   DECODE(usDecodLen,pointer,short);
   totlen = usDecodLen;
 }
 
 stFN->lDatalen = 0;
 
 while (totlen > 0)
 {
   pgi = *pointer++;totlen--;
   switch (pgi)
   {
   case RC08ba_PITransportDisconnect:         /*  Transport Disconnect      */
                if (*pointer++ != 1)   
                   return(-1);
                 stFN->usTdiscAreuse = ((*pointer & 1) != 1 );
                 stFN->usTdiscUserAbort = ((*pointer & 2) == 2 );
                 stFN->usTdiscProtAbort = ((*pointer & 4) == 4 );
                 stFN->usTdiscNoReason = ((*pointer & 8) == 8 );
                 pointer +=  1; totlen  -=  2;
                 break;

   case RC08bl_PIEnclosureItem:               /* Enclosure Item              */
                  if (*pointer++ != 1)    
                     return(-1);
                   stFN->ulEnclosure = (*pointer & 3);
                   pointer +=  1; totlen  -=  2;
                   break;

   case RC08a5_PGIUserData:                   /* User Data                   */
                if (*pointer < 255)     
                {
                  stFN->lDatalen = *pointer++;totlen--;
                }
                else
                { 
                  pointer++;totlen -= 3;
                  DECODE(usDecodLen,pointer,short);
                  stFN->lDatalen = usDecodLen;
                }
                stFN->pDataptr = (char FAR *)pointer;
                pointer +=  stFN->lDatalen ;
                totlen = (signed short int)(stFN->lDatalen - totlen); 
                break;

   default:     return(-1);
   }
 }
return(0); 
}/*end-ro0827_DecodeFN*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0829_DecodeRF                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-17                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short  int ro0829_DecodeRF(
rT0819_stRF FAR *stRF)

/* DESCRIPTION  :  Decodes the incoming Refuse SPDU                   */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/*      stRF    =   coading structure for Refuse SPDU                 */
/*                                                                    */
/* FORMAL OUT-PARAMETERS      :                                       */
/*                                                                    */
/*      stRF    =   coading structure for Refuse SPDU                 */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/
{/*entr-ro0829_DecodeRF*/

  unsigned char pgi,pi;
  signed short int totlen,len,pgilen;
  unsigned  char FAR * pointer;
  unsigned short int usDecodLen;
 
  /* init_pstruc(); */
  
  pointer = (unsigned char FAR *)stRF->pDataptr+1;

/****************** Decode RF Total Blength ******************************/

 if (*pointer < 255)
   totlen = *pointer++;
 else
 {
   pointer++;
   DECODE(usDecodLen,pointer,short);
   totlen = usDecodLen;
 }
 
 stRF->lDatalen = 0;
 
 while (totlen > 0)
 {
   pgi = *pointer++;totlen--;
   switch (pgi)
   {
   case RC08a0_PGIConnectionIdentifier:      /*  Connection Identifier  */
     pgilen = *pointer++;totlen--;
     while (pgilen > 0)
     {
       pi = *pointer++;totlen--;pgilen--;
       switch (pi)
       {
       case RC08b0_PICalledSSUserRef : /* Called user Reference   */
                 len = *pointer;   
                 pointer +=  len+1;
                 totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       case  RC08b7_PICommonRef :       /* Common Reference        */
                 len = *pointer;      
                 pointer +=  len+1;
                 totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       case  RC08b8_PIAddRefInfo :    /* Additional Reference     */
                 len = *pointer; 
                 pointer +=  len+1;
                 totlen  -=  len+1; pgilen  -=  len+1;
                 break;

       default:  return(-1);
                 /* break; */
       }
     }
     break;
 
   case RC08ba_PITransportDisconnect :        /* Transport Disconnect      */
                if (*pointer++ != 1)  
                   return(-1);
                 stRF->usTdiscAreuse = ((*pointer & 1) != 1 );
                 pointer +=  1; totlen  -=  2;
                 break;

   case RC08bh_PISessionUserRequirements :   /* session User Requirements */
                if (*pointer++ != 2)    
                   return(-1);
                 DECODE(usDecodLen,pointer,short) ;
                 stRF->usFunctionalUnits = usDecodLen;
                 totlen  -=  3;
                 break;

   case RC08bj_PISessionVersionNo :          /* Version Number           */
                if (*pointer++ != 1)  
                  return(-1);
                stRF->usVersion = *pointer;
                pointer += 1 ; totlen  -= 2 ;
                break;

   case RC08bl_PIEnclosureItem :                   /* Enclosure Item        */ 
               if (*pointer++ != 1)       
                 return(-1);
                stRF->ulEnclosure = (*pointer & 3);
                pointer +=  1; totlen  -=  2;
                break;

   case RC08bA_PIReasonCode :                       /*  Reason               */
                if (*pointer < 255)    
                {
                    stRF->lDatalen = *pointer++;totlen--;
                }
                else
                { 
                  pointer++;totlen -= 3;
                  DECODE(usDecodLen,pointer,short);
                  stRF->lDatalen = usDecodLen;
                }
                stRF->usReason = *pointer; 
                stRF->lDatalen--;
                pointer++;totlen--;
                if (stRF->lDatalen == 0)
                 {   pointer = NULL;
                     return(0);
                 }
                stRF->pDataptr = (char FAR *) pointer;
/*     by kc on 24/6 *****
                pointer +=  stRF->lDatalen+1;
                totlen = (signed short int)(stRF->lDatalen + 1 - totlen); 
 ******** end *********** */
                pointer +=  stRF->lDatalen;
                totlen = (signed short int)(stRF->lDatalen  - totlen); 

                break;
   default:     return(-1);
   }
 }
return(0); 
}/*end-ro0829_DecodeRF*/
/*EOF*/
