/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm01.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:16  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:53  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:39  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:13:05  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:20  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:30  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:07:44  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:45:55  keutel
 * 	creation
 * 	[1994/03/21  13:36:19  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rospm01.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:16 $" } ;
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
/* NAME         : rospm01.c                                           */
/*                                                                    */
/* AUTHOR       : Milind M.Aphale.                                    */
/* DATE         : 93-03-15                                            */
/*                                                                    */
/* COMPONENT    : RTROS Convergence protocol machine                  */
/*                                                                    */
/* PRD#/VERS.   : RTROS V-3.00                                        */
/*                                                                    */
/* DESCRIPTION  : provides the encode functions of the SPDU           */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-03-15|  original                      |ma  |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid = "@(#) rospm01.c 1.1 93/11/29 RTROS-V3.00";

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
#include <routil0.h>    /* Common utility routines                    */  

#include <roendeco.h>   /* for encoding  kc on 6-8-93                 */     
#include <rospm01.h>    /* Defines for coding functions               */
#include <rospm07.h>    /* Prototyping of convergence protocol m/c    */    
#include <rospm05.h>    /* Prototyping of coding of functions         */ 
#include <rospm06.h>    /* Prototyping of convergence protocol m/c    */    

/*********************************************************************/
/*                                                                   */
/*              G L O B A L   V A R I A B L E S                      */
/*                                                                   */
/*********************************************************************/

static  char value[RC0831_MaxParamLen];

/*********************************************************************/
/*         LOCAL FUNCTION PROTOS                                     */
/*********************************************************************/
void ro081A_Short2Char PROTO (( 
unsigned short int usVal,
char *pVal));
/*********************************************************************/
/*                                                                   */
/*              G L O B A L   F U N C T I O N S                      */
/*                                                                   */
/*********************************************************************/


typedef  struct tochar {
       unsigned char lbyte;
       unsigned char hbyte;
       }toCHAR;

typedef union short2char {
        unsigned short int usbyte;
        toCHAR              stChar;
        } SHORT2CHAR;
           
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro081A_Short2Char                                   */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-06-14                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
void ro081A_Short2Char ( 
unsigned short int usVal,
char *pVal)
{
/* kc on 6-8-93 
SHORT2CHAR s2c;
   
     s2c.usbyte = usVal;
     *(pVal)    = s2c.stChar.hbyte;
     pVal++;
     *(pVal)    = s2c.stChar.lbyte;
  */
  ENCODE(usVal,pVal,short);
}
 


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0810_EncodeAA                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int ro0810_EncodeAA(
rT0810_stAA FAR *stAA  )

/* DESCRIPTION     : Encodes the ABORT ACCEPT SPDU                    */
/*                                                                    */
/* IN-PARAMETERS   :                                                  */
/*                                                                    */
/*        stAA   =   Coading structure for ABORT ACCEPT SPDU          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS  :                                           */
/*                                                                    */
/*        stAA   =   Coading structure for ABORT ACCEPT SPDU          */
/*                                                                    */
/* RETURN VALUES   :                                                  */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*                                                                    */
/* GLOBAL DATA   ( read only ): none                                  */
/*                                                                    */
/* GLOBAL DATA   ( modified ) : none                                  */
/*                                                                    */
/* REMARKS                    : none                                  */
/*                                                                    */
/*inoff ***************************************************************/
{/*entr-ro0810_EncodeAA*/

 stAA->pDataptr[0] = RC0899_SIAbortAcceptSPDU;
 stAA->pDataptr[1] = 0;
 stAA->lDatalen = 2;
 return(0);

}/*end-ro0810_EncodeAA*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0811_EncodeAB                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int ro0811_EncodeAB(
rT0811_stAB FAR *stAB)

/* DESCRIPTION     : Encodes the ABORT SPDU                           */
/*                                                                    */
/* IN-PARAMETERS   :                                                  */
/*                                                                    */
/*       stAB    =   Coading structure  for ABORT SPDU                */
/*                                                                    */
/* FORMAL OUT-PARAMETERS  :                                           */
/*                                                                    */
/*       stAB    =   Coading structure  for ABORT SPDU                */
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
{/*entr-ro0811_EncodeAB*/
unsigned short int i, code;
unsigned char ucBytelen = 255;
i = 0;

/******************** Encode AB transport Disconnect *****************/

 /*****
  if ((stAB->ulEnclosure == RC0876_OneSPDU) || (stAB->ulEnclosure == RC0877_StartSPDU)) 
  { 
 ********/
   value[i++] = RC08ba_PITransportDisconnect;
   value[i++] = 1;                        /*  length field */ 
   if ( stAB->usTdiscAreuse == RC0870_Reuse)
      code = 0;
   else
      code = 1;
   if ( stAB->usTdiscUserAbort == RC0856_UserAbort)
      code += 2;
   if ( stAB->usTdiscNoReason == RC0855_NoReason)
      code += 4;
   value[i++] = (char)code;                     /* PV field     */

 
/********
******************* Encode AB Enclosure Item ******************
PROBLEM
Enclosure item banned while sending 
     value[i++] = RC08bl_PIEnclosureItem;    
     value[i++] = 1;
     value[i++] = stAB->ulEnclosure;

}
********/


/********************* Encode AB User Bdata ************************/

 if ( stAB->usTdiscUserAbort == RC0856_UserAbort)
 {
     value[i++] = (char)RC08a5_PGIUserData;   /* User Bdata PGI Code       */
     if (stAB->lDatalen < (long int)ucBytelen )
         value[i++] = (char)stAB->lDatalen;
     else
       { value[i++] = ucBytelen;       /* Code Blength of SUSER DATA */
         value[i++] = (char)(stAB->lDatalen >> 8);
                                       /* i DIV 256 (high order bits)*/
         value[i++] = (char)(stAB->lDatalen & 255); 
                                       /* i MOD 256 (low order bits) */
       }
     stAB->pDataptr -= i;
     stAB->lDatalen += i;
     ro0041_memcpy((char FAR *)stAB->pDataptr,
                   (char FAR *)&value[0],
                   (unsigned long int)i );
 }
 else
 {
     value[i++] = RC08bt_PIReflectParamVal;       /* Reflect param   */

     value[i++] = (char)stAB->sReflParamLen; 
     ro0041_memcpy((char FAR *)&value[i],
                   (char FAR *)&stAB->ucReflParam[0], 
                   (unsigned long int)stAB->sReflParamLen);

     i += stAB->sReflParamLen; 

     stAB->pDataptr -= i;
     stAB->lDatalen += i;
     ro0041_memcpy((char FAR *)stAB->pDataptr, 
                   (char FAR *)&value[0], 
                   (unsigned long int)i );
 }

/****************** Encode SI Code for AB SPDU *********************/

 value[0] = RC0898_SIAbortSPDU;           /* SI Code for AB SPDU       */
 if (stAB->lDatalen < 255)
   { value[1] = (char)stAB->lDatalen;
     i = 2;
   }
 else
    { value[1] = ucBytelen;               /* Code Blength of sudata      */
      value[2] = (char)(stAB->lDatalen >> 8); /* i DIV 256 (high order bits) */
      value[3] = (char)(stAB->lDatalen & 255);/* i MOD 256 (low order bits)  */
      i = 4;
    }
  stAB->pDataptr -= i;         
  stAB->lDatalen += i;
  ro0041_memcpy((char FAR *)stAB->pDataptr, 
                (char FAR *)&value[0], 
                (unsigned long int)i );


return(0);
}/*end-ro0811_EncodeAB*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0812_EncodeAC                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short  int ro0812_EncodeAC(
rT0812_stAC FAR *stAC)

/* DESCRIPTION    : Encodes the ACCEPT SPDU                           */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*       stAC  =    Coading structure for ACCEPT SPDU                 */
/*                                                                    */
/* FORMAL OUT-PARAMETERS :                                            */
/*                                                                    */
/*       stAC  =    Coading structure for ACCEPT SPDU                 */
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
{/*entr-ro0812_EncodeAC*/

unsigned short int i,len,pos;
unsigned char ucBytelen = 255;
i = 0;

/**********
 if ((stAC->ulEnclosure == RC0876_OneSPDU) || (stAC->ulEnclosure == RC0877_StartSPDU))
 {
**********/

/****************** Encode AC Connect / S_ACCEPT Item **************/

   value[i++] = RC08a1_PGIConnectSAcceptItem;  /* PGI Code Connect/S_ACCEPT Item */
   pos = i++;

   value[i++] = RC08bb_PIProtocolOptions;      /* PI Code Protocol Options */
   value[i++] = 1;
   value[i++] = (char)stAC->usPopt; 

   if ( stAC->ulTSDUmaxto > 0 )
   {
     value[i++] = RC08bi_PITSDUMaxsize;        /* TSDU Maximum Size */
     value[i++] = 4;
     value[i++] = (char)(stAC->ulTSDUmaxto >> 8);
     value[i++] = (char)(stAC->ulTSDUmaxto & 255);
     value[i++] = (char)(stAC->ulTSDUmaxfrom >> 8);
     value[i++] = (char)(stAC->ulTSDUmaxfrom & 255);
   }
   
   value[i++] = RC08bj_PISessionVersionNo;   /* PI Code Version Number */
   value[i++] = 1;
   value[i++] = (char)stAC->usVersion;
   
   value[pos] = (char)(i - pos - 1);       /* Blength Connect/S_ACCEPT Item */
   
  /****************** Encode AC Session User Requirements *****************/

   /* PI Code Session User Requirements - Functional Units */
   value[i++] = RC08bh_PISessionUserRequirements; 
   value[i++] = 2;             
   ro081A_Short2Char(stAC->usFunctionalUnits,&value[i]);
   i = i+2;


/*************
 ***************** Encode AC Enclosure Item *********************
Enclosure item banned while sending.
PROBLEM 
   value[i++] = RC08bl_PIEnclosureItem;   
   value[i++] = 1;
   value[i++] = (char)stAC->ulEnclosure;
**************/

 /****************** Encode AC Calling/Responding Session Selector *********/

   len = stAC->sCallingLen; 
   if ( len > 0)
   {
     value[i++] = RC08bB_PICallingSessionSelector; /* PI Code Calling S-Selector */
     value[i++] = (char)len;
     ro0041_memcpy((char FAR *)&value[i], 
                   (char FAR *)stAC->vCalling, 
                   (unsigned long int)len );
     i = i + len;
   }
 
   len = stAC->sRespondingLen;
   if ( len > 0)
   {
      value[i++] = RC08bC_PICalledSessionSelector;  /* PI Code Respond S-Selector */
      value[i++] = (char)len;
      ro0041_memcpy((char FAR *)&value[i], 
                    (char FAR *)stAC->ucResponding, 
                    (unsigned long int)len );
      i = i + len ;
   }
/******
 }for-> if ((stAC->encl sure == RC0876_OneSPDU) || (stAC->ulEnclosure == RC0877_StartSPDU))
*****/

/****************** Encode AC User Bdata ***************************/
 
 if ( stAC->lDatalen != 0 )
 {
   value[i++] = (char)RC08a5_PGIUserData;           /* PGI Code User Bdata */
   if ( stAC->lDatalen < (long int)ucBytelen )
      value[i++] = (char)stAC->lDatalen;
    else
     { value[i++] = ucBytelen;
       value[i++] = (char)(stAC->lDatalen >> 8);
       value[i++] = (char)(stAC->lDatalen & 255);
     }
  }
  stAC->pDataptr -= i;
  stAC->lDatalen += i;
  ro0041_memcpy((char FAR *)stAC->pDataptr, 
                (char FAR *)&value[0], 
                (unsigned long int)i );

/****************** Encode SI Code for AC SPDU *********************/

 value[0] = RC0896_SIAcceptSPDU;      /* SI Code for AC SPDU */
 if (stAC->lDatalen < 255)
   { value[1] = (char)stAC->lDatalen;
     len = 2;
   }
 else
    { value[1] = ucBytelen;   /* Code Blength of sudata */
      value[2] = (char)(stAC->lDatalen >> 8);  /* i DIV 256 (high order bits) */
      value[3] = (char)(stAC->lDatalen & 255); /* i MOD 256 (low order bits) */
      len = 4;
    }
 stAC->pDataptr -= len;
 stAC->lDatalen += len;
 ro0041_memcpy((char FAR *)stAC->pDataptr ,
               (char FAR *)&value[0], 
               (unsigned long int)len);

 return(0);
}/*end-ro0812_EncodeAC*/                     



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0814_EncodeCN                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short  int ro0814_EncodeCN(
rT0814_stCN FAR *stCN)

/* DESCRIPTION    : Encodes the CONNECT SPDU                          */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*       stCN =   Coading strecture for CONNECT SPDU                  */
/*                                                                    */
/* FORMAL OUT-PARAMETERS :                                            */
/*                                                                    */
/*       stCN =   Coading strecture for CONNECT SPDU                  */
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
{/*entr-ro0814_EncodeCN*/
unsigned short int i,len,pos;
unsigned char ucBytelen = 255;

i = 0;

/****************** Encode CN Connect / S_ACCEPT Item ********************/

 value[i++] = RC08a1_PGIConnectSAcceptItem; /* PGI Code Connect/S_ACCEPT Item */
 pos = i++;

 value[i++] = RC08bb_PIProtocolOptions;       /* PI Code Protocol Options */
 value[i++] = 1;
 value[i++] = (char)stCN->usPopt;

 if ( stCN->ulTSDUmaxto > 0 )
 { 
   value[i++] = RC08bi_PITSDUMaxsize;         /* TSDU Maximum Size */
   value[i++] = 4;
   value[i++] = (char)(stCN->ulTSDUmaxto >> 8);
   value[i++] = (char)(stCN->ulTSDUmaxto & 255);
   value[i++] = (char)(stCN->ulTSDUmaxfrom >> 8);
   value[i++] = (char)(stCN->ulTSDUmaxfrom & 255);
 }

   value[i++] = RC08bj_PISessionVersionNo;   /* PI Code Version Number */
   value[i++] = 1;
   value[i++] = (char)stCN->usVersion;

   value[pos] = i - pos - 1;   /* Blength Connect/S_ACCEPT Item */

/****************** Encode CN Session User Rerquirements *****************/

 value[i++] = RC08bh_PISessionUserRequirements; 
                                   /* PI Code Session User Requirements */
 value[i++] = 2;                   /* length field for Functional Units */
 ro081A_Short2Char(stCN->usFunctionalUnits,&value[i]);
 i = i+2;                                              
/****************** Encode CN Calling/Called Session Selector *********/

 if ( stCN->sCallingLen > 0)
   { value[i++] = RC08bB_PICallingSessionSelector; /* PI Code Calling S-Selector */
     value[i++] = (char)stCN->sCallingLen;
     ro0041_memcpy((char FAR *)&value[i], 
                   (char FAR *)stCN->vCalling, 
                   (unsigned long int)stCN->sCallingLen );
     i = i + stCN->sCallingLen;
   }
 
 if ( stCN->sCalledLen > 0)
   { value[i++] = RC08bC_PICalledSessionSelector; /* PI Code Called S-Selector */
     value[i++] = (char)stCN->sCalledLen;
     ro0041_memcpy((char FAR *)&value[i], 
                   (char FAR *)stCN->vCalled, 
                   (unsigned long int)stCN->sCalledLen );
     i = i + stCN->sCalledLen;
   }

/****************** Encode CN Overflow ********************************/

 if (stCN->lDatalen != 0)
 {
  /*******
   Data Overflow in sending banned 
   RTROS V-3.00 Implimentation restrictions ..... 

  if ( stCN->usOverflow == RC087c_Overflow )
     { value[i++] = RC08bI_PIDataOverflow;    PI Code Overflow  
       value[i++] = 1;
       value[i++] = 1;
     }
   **********************/

/****************** Encode CN Extended User Bdata ***********************/

   if ((stCN->usVersion & RCro51_ProtVers2) ==  RCro51_ProtVers2)
   {                      
     value[i++] = (char)RC08a6_PGIExtendedUserData;
                                      /* PGI Code Extended User Bdata */
     if ( stCN->lDatalen < 255 )
        value[i++] = (char)stCN->lDatalen;
      else {
          value[i++] = ucBytelen;
          if ( stCN->lDatalen < RC0835_MaxConnectLen)
            ;
          else
            stCN->lDatalen = RC0835_MaxConnectLen;
          value[i++] = (char)(stCN->lDatalen >> 8);
          value[i++] = (char)(stCN->lDatalen & 255);
      }
    }
    else
    {
      value[i++] = (char)RC08a5_PGIUserData;       /* PGI Code User Bdata */
      if ( stCN->lDatalen > (long int)ucBytelen )
       return(-1); 
      value[i++] = (char)stCN->lDatalen;
    }
 }
 stCN->pDataptr -= i;
 stCN->lDatalen += i;

 ro0041_memcpy((char FAR *)stCN->pDataptr, 
               (char FAR *)&value[0], 
               (unsigned long int)i );

/****************** Encode SI Code for CN SPDU *********************/

 value[0] = RC0895_SIConnectSPDU;      /* SI Code for CN SPDU */
 if (stCN->lDatalen < 255)
   { value[1] = (char)stCN->lDatalen;
     len = 2;
   }
 else
   { value[1] = ucBytelen;              /* Code Blength of pDataptr        */
     value[2] = (char)(stCN->lDatalen >> 8);/* dlen DIV 256 (high order bits) */
     value[3] = (char)(stCN->lDatalen & 255);/* dlen MOD 256 (low order bits) */
     len = 4;
   }
 stCN->pDataptr -= len;
 stCN->lDatalen += len;
 ro0041_memcpy((char FAR *)stCN->pDataptr , 
               (char FAR *)&value[0] , 
               (unsigned long int)len );
 
 return(0);
}/*end-ro0814_EncodeCN*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0815_EncodeDN                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int ro0815_EncodeDN(
rT0815_stDN FAR *stDN)

/* DESCRIPTION    : Encodes the DISCONNECT SPDU                       */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*       stDN  =    Coading structure for DISCONNECT SPDU             */
/*                                                                    */
/* FORMAL OUT-PARAMETERS :                                            */
/*                                                                    */
/*       stDN  =    Coading structure for DISCONNECT SPDU             */
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
{/*entr-ro0815_EncodeDN*/

unsigned short int i,l;
unsigned char ucBytelen = 255;

i = 0;

/**********************

 ******************* Encode DN Enclosure Item ***********************
 
    value[i++] = RC08bl_PIEnclosureItem;    Enclosure Item PI Code 
    value[i++] = 1;
    value[i++] = stDN->ulEnclosure;
************/

/****************** Encode DT User Bdata ***************************/
 
 value[i++] = (char)RC08a5_PGIUserData;    /* User Bdata PGI Code */

 if (stDN->lDatalen < (long int)ucBytelen)
    value[i++] = (char)stDN->lDatalen;
 else
    { value[i++] = ucBytelen;            /* Code Blength of pDataptr     */
      value[i++] = (char)(stDN->lDatalen >> 8);/* i DIV 256 (high order bits) */
      value[i++] = (char)(stDN->lDatalen & 255);/* i MOD 256 (low order bits) */
    }

 stDN->pDataptr = stDN->pDataptr - i;
 stDN->lDatalen = i + stDN->lDatalen;
 ro0041_memcpy((char FAR *)stDN->pDataptr,
               (char FAR *)&value[0],
               (unsigned long int)i );

/****************** Encode SI Code for DN SPDU *********************/

 value[0] = RC0893_SIDisconnectSPDU;      /* SI Code for DN SPDU */
 if (stDN->lDatalen < (long int)ucBytelen)
   { value[1] = (char)stDN->lDatalen;
     l = 2;
   }
 else
    { value[1] = ucBytelen;    /* Code Blength of Bdata */
      value[2] = (char)(stDN->lDatalen >> 8); /* i DIV 256 (high order bits) */
      value[3] = (char)(stDN->lDatalen & 255); /* i MOD 256 (low order bits) */
      l = 4;
    }
 stDN->pDataptr = stDN->pDataptr - l;
 stDN->lDatalen = l + stDN->lDatalen;
 ro0041_memcpy((char FAR *)stDN->pDataptr,
               (char FAR *)&value[0], 
               (unsigned long int)l );

 return(0);
}/*end-ro0815_EncodeDN*/



/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0816_EncodeDT                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int ro0816_EncodeDT(
rT0816_stDT FAR *stDT)

/* DESCRIPTION    : Encodes the DATA TRANSFER SPDU                    */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      stDT  =     Coading structure for DATA TRANSFER SPDU          */
/*                                                                    */
/* FORMAL OUT-PARAMETERS :                                            */
/*                                                                    */
/*      stDT  =     Coading structure for DATA TRANSFER SPDU          */
/*                                                                    */
/* RETURN VALUES :                                                    */
/*                                                                    */
/*      = 0  SUCES     successfully                                   */
/*      < 0 (SUCES)    error                                          */
/*                                                                    */
/* GLOBAL DATA   ( read only ): none                                  */
/*                                                                    */
/* GLOBAL DATA   ( modified ) : none                                  */
/*                                                                    */
/* REMARKS                    : none                                  */
/*                                                                    */
/*inoff ***************************************************************/
{/*entr-encode_DT*/

unsigned short int i;
i = 0;

/****************** Encode SI Code for DT SPDU *********************/

  value[i++] = 1;        /* SI Code for GT SPDU */
  value[i++] = 0;
  value[i++] = RC0890_SIDataTransferSPDU;   /* SI Code for DT SPDU */

/********************
Enclosure Item banned while sending 
 
  if (stDT->ulEnclosure != 3)
  {
    value[i++] = 3;
    value[i++] = RC08bl_PIEnclosureItem;    
    value[i++] = 1;   
    value[i++] = stDT->ulEnclosure;    
  }
  else
******************/

 value[i++] = 0;   

/****************** Encode DT User Bdata ***************************/

 stDT->pDataptr -= i;
 stDT->lDatalen += i;
 ro0041_memcpy((char FAR *)stDT->pDataptr, 
               (char FAR *)&value[0], 
               (unsigned long int)i );

 return(0);
}/*end-ro0816_EncodeDT*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0817_EncodeFN                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int ro0817_EncodeFN(
rT0817_stFN FAR *stFN)

/* DESCRIPTION    : Encodes the FINISH SPDU                           */
/*                                                                    */
/* IN-PARAMETERS  : 					              */
/*                                                                    */
/*      stFN    =   coading structure for FINISH SPDU                 */
/*                                                                    */
/* FORMAL OUT-PARAMETERS :                                            */
/*                                                                    */
/*      stFN    =   coading structure for FINISH SPDU                 */
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
{/*entr-ro0817_EncodeFN*/

unsigned short int i;
unsigned char ucBytelen = 255;

i = 0;

/****************** Encode FN Transport Disconnect  **************/
/*****************
Enclosure item banned while sending !!

  if ((pstruc->ulEnclosure == RC0876_OneSPDU) || (pstruc->ulEnclosure == RC0877_StartSPDU))
  {
******************/
 
  value[i++] = RC08ba_PITransportDisconnect;  /* PI Code Transport Disconnect */
    value[i++] = 1;
    if ( stFN->usTdiscAreuse == RC0870_Reuse )
         value[i++] = 0;  /* Transport Connection kept     */
    else value[i++] = 1;  /* Transport Connection released */

/**********************
  } for =>if ((pstruc->ulEnclosure == RC0876_OneSPDU) || (pstruc->ulEnclosure == RC0877_StartSPDU))
 ******************** Encode FN Enclosure Item ************************

     value[i++] = RC08bl_PIEnclosureItem;    
     value[i++] = 1;
     value[i++] = stFN->ulEnclosure;
*****************/


/****************** Encode FN User Bdata ***************************/

   value[i++] = (char)RC08a5_PGIUserData;        /* PGI Code User Bdata */
   if ( stFN->lDatalen < (long int)ucBytelen )
      value[i++] = (char)stFN->lDatalen;
   else
    { value[i++] = ucBytelen;
      value[i++] = (char)(stFN->lDatalen >> 8);
      value[i++] = (char)(stFN->lDatalen & 255);
    }

 stFN->pDataptr -= i;
 stFN->lDatalen += i;
 ro0041_memcpy((char FAR *)stFN->pDataptr, 
               (char FAR *)&value[0],
               (unsigned long int)i );

/****************** Encode SI Code for FN SPDU *********************/

 value[0] = RC0892_SIFinishSPDU;        /* SI Code for FN SPDU */
 if (stFN->lDatalen < (long int)ucBytelen)
   { value[1] = (char)stFN->lDatalen;
     i = 2;
   }
 else
    { value[1] = ucBytelen;    /* Code Blength of tudata */
      value[2] = (char)(stFN->lDatalen >> 8);  /* i DIV 256 (high order bits) */
      value[3] = (char)(stFN->lDatalen & 255); /* i MOD 256 (low order bits)  */
      i = 4;
    }
 stFN->pDataptr -= i;
 stFN->lDatalen += i;
 ro0041_memcpy((char FAR *)stFN->pDataptr,
               (char FAR *)&value[0], 
               (unsigned long int)i );

return(0);
}/*end-ro0817_EncodeFN*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : ro0819_EncodeRF                                     */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-03-15                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int ro0819_EncodeRF(
rT0819_stRF FAR *stRF)

/* DESCRIPTION    : Encodes the REFUSE SPDU                           */
/*                                                                    */
/* IN-PARAMETERS  :                                                   */
/*                                                                    */
/*      stRF  =     Coading structure for REFUSE SPDU                 */
/*                                                                    */
/* FORMAL OUT-PARAMETERS :                                            */
/*                                                                    */
/*      stRF  =     Coading structure for REFUSE SPDU                 */
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
{/*entr-ro0819_EncodeRF*/
unsigned short int i;
unsigned char ucBytelen = 255;

i = 0;

/****************** Encode RF Transport Disconnect  **************/
/*********   
 if ((stRF->ulEnclosure == RC0876_OneSPDU) || (stRF->ulEnclosure == RC0877_StartSPDU))
 {
*********/
 
   value[i++] = RC08ba_PITransportDisconnect; /* PI Code Transport Disconnect */
   value[i++] = 1;
   value[i++] = (char)stRF->usTdiscAreuse;

/****************** Encode RF Sesion User Reqiurments **************/

   if (stRF->usReason == RC08c0_CalledSSUserRej)
   {
     value[i++] = RC08bh_PISessionUserRequirements;
     value[i++] = 2;
     ro081A_Short2Char(stRF->usFunctionalUnits,&value[i]);
     i = i+2;                                             
   }

    value[i++] = RC08bj_PISessionVersionNo;     /* PI Code Version Number */
    value[i++] = 1;
    value[i++] = (char)stRF->usVersion;

/***********************************************
 }
 for => if ((stRF->ulEnclosure == RC0876_OneSPDU) || (stRF->ulEnclosure == RC0877_StartSPDU))
    ***************** Encode RF Enclosure Item ****************

    value[i++] = 25; 
    value[i++] = 1; 
    value[i++] = (char)stRF->ulEnclosure;
*************************************************/
 
 /****************** Encode RF Reason and User Bdata  **************/

  value[i++] = RC08bA_PIReasonCode;             /* PI Code Reason  */

  if ( stRF->usReason == RC08c0_CalledSSUserRej )
  {
    if ( stRF->lDatalen < (long int)ucBytelen )
      value[i++] = (char)stRF->lDatalen + 1; 
                  /* for addition of 1 byte as a reason  */
    else
    {
      value[i++] = ucBytelen;
      value[i++] = (char)((stRF->lDatalen + 2 ) >> 8);
      value[i++] = (char)((stRF->lDatalen + 2 ) & 255); 


    }
      value[i++] = (char)stRF->usReason; 
  }
  else
  {
    value[i++] = 1;
    value[i++] = (char)133;     /* rejection by SPM : reason not specified  */
                          /*                     128+5                */   
  }
    stRF->pDataptr -= i;
    stRF->lDatalen += i;
    ro0041_memcpy((char FAR *)stRF->pDataptr,
                  (char FAR *)&value[0],
                  (unsigned long int)i);
 
/****************** Encode SI Code for RF SPDU *********************/

 value[0] = RC0894_SIRefuseSPDU;                 /* SI Code for RF SPDU */
 if (stRF->lDatalen  < (long int)ucBytelen)
   { value[1] = (char)stRF->lDatalen ;
     i = 2;
   }
 else
    { value[1] = ucBytelen;                 /* Code Blength of Entudata    */
      value[2] = (char)(stRF->lDatalen +1 >> 8);
                                  /* i DIV 256 (high order bits) */
      value[3] = (char)(stRF->lDatalen +1& 255);
                                 /* i MOD 256 (low order bits)  */
      /* lDatalen +1  by kc on  2/7 instead of  only lDatalen  */
      i = 4;
    }
 
 stRF->pDataptr -= i;
 stRF->lDatalen += i;
 ro0041_memcpy((char FAR *)stRF->pDataptr,
               (char FAR *)&value[0], 
               (unsigned long int)i );
 
 return(0);
}/*end-ro0819_EncodeRF*/
/*EOF*/
