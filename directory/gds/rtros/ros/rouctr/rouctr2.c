/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rouctr2.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:36  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:12  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:31  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:41  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:47  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:09:25  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:26  keutel
 * 	creation
 * 	[1994/03/21  13:38:05  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rouctr2.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:36 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1991 by                                                  *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG                                 *
*  * All rights reserved                                                    *
*  *                                                                        *
*  * This software is furnished under licence and may be used only in       *
*  * accordance with the terms of that licence and with the inclusion of    *
*  * the above copyright notice. This software may not be provided or       *
*  * otherwise made available to, or used by, any other person. No title    *
*  * to or ownership of the software is hereby transferred.                 *
*                                                                           *
\***************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME 	: rouctr2.c					      */
/*                                                                    */
/* AUTHOR	: Vinita                           	              */
/* DATE 	: 93-03-20					      */
/*                                                                    */
/* COMPONENT	: RTS association control function		      */
/*                                                                    */
/* PRD#/VERS.	: RTS-V3.00					      */
/*                                                                    */
/* DESCRIPTION	:						      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/* 2.00 K0  | 92-01-25|  original		       |vp  |	      */
/*	    |	      | 			       |    |	      */
/*datoff **************************************************************/

/**********************************************************************/
/*                     Module Identification                          */
/**********************************************************************/

static char *sccsid =    "@(#) rouctr2.c 1.2 93/12/20   RTS-V3.00";

/*********************************************************************/
/*              I N C L U D E S                                      */
/*********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>	 /* RTS general definitions		     */
#include <roerval.h>	 /* RTS error values			     */
#include <rolgval.h>	 /* RTS logging type definitions	     */

#include <rouply0.h>	 /* RTS common upper layer definitons	     */
#include <rouply1.h>	 /* RTS common upper layer definitons	     */

#include <routil0.h>	 /* RTS utilities interface		     */
#include <routil1.h>	 /* RTS utilities interface for upper layer  */

#include <rodpat0.h>	 /* RTS dispatcher pm interface 	     */

#include <rostor0.h>	 /* RTS utility store interface 	     */
#include <tpdu.h>	 /* RTS tpdu intterface         	     */
#include <roexif2.h>	 /* RTS tpdu intterface         	     */

#include <asn1st.h>      /* RTS apma calling  interface.             */

#include <routilt.h>	 /* RTS apma calling  interface.	     */

#include <rouctr0.h>     /* RTS apma service  interface              */
#include <rouctr2.h>	 /* RTS apma calling  interface.	     */
#include <rouctr3.h>	 /* RTS apma logging  interface.	     */
#include <rouctr5.h>	 /* RTS apma logging  interface.	     */


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME 	: ro1520_CreateEvData					      */
/*                                                                            */
/* AUTHOR	: Vinita        					      */
/* DATE         : 93-03-20                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro1520_CreateEvData (

unsigned long int         hBuf,
unsigned long  int   FAR *hEvData)

/* DESCRIPTION :                                                              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr*/

RM0001_STRG (fnctName, "ro1520_CreateEvData")
/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed	 short int	 rc=0;
rT1501_stBuffer         *phBuf;      
rT1504_stEvData         *phEvData;
/* kc on 9-9-93 */
signed   long  int        lSize = sizeof(rT1504_stEvData);

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (fnctName)

   /* *************** lock the buffer handle ***************************** */
   /* kc on 14-9-93 */
   if  ((*hEvData = ro0030_localalloc ( lSize )) == NULL )                    
    RM0003_RETURN (fnctName, signed short int, - R_LALLCE)                    
                                                                              
   if ( (phEvData = (rT1504_stEvData *) ro0031_locallock (*hEvData)) == NULL) 
    RM0003_RETURN (fnctName, signed short int, - R_LLOCKE)                    
 
if (!hBuf) 
{
     phEvData->bSpecFlg = TRUE;
     phEvData->ulBufLen = NULL;
     phEvData->iMemType = RC1500_Memory;
     phEvData->ulFragCount = NULL;
 /*    ro0227_setSpecFlg(); */
 /*     rc = -R_UBUFER; */
}
else 
{  /* locallock is changed to global and checks are added by kc on19/5 */
     if ( (phBuf = (rT1501_stBuffer *) ro0031_locallock (hBuf)) == NULL) 
           rc = - R_LLOCKE;
     else {   /* by kc on 19.5 */

   /* ********** Covert Buffer content in EvData Format ****************** */
   if (phBuf->iMemType == RC1501_File ) {
         lSize +=  phBuf->ulLength;
         if (ro0032_localunlock (*hEvData))
           RM0003_RETURN (fnctName, signed short int, -R_LULOKE)
         if ((*hEvData =  
           ro0033_localrealloc ( *hEvData, sizeof(rT1504_stEvData), lSize)) == 
             NULL )
           RM0003_RETURN (fnctName, signed short int, - R_LALLCE)
         if ( (phEvData = (rT1504_stEvData *) ro0031_locallock (*hEvData)) == 
             NULL) 
            RM0003_RETURN (fnctName, signed short int, - R_LLOCKE)                     
   }

        phEvData->iMemType = phBuf->iMemType;                                
                                                                         
        if (phBuf->iMemType == RC1500_Memory)                              
            phEvData->unBufferRef.pBufRef = phBuf->unBufferRef.pBufRef;    
        else if (phBuf->iMemType == RC1501_File) {
               phEvData->unBufferRef.pTpduName =
                 (char FAR *)((char FAR *) phEvData + sizeof(rT1504_stEvData));
               ro0041_memcpy ( phEvData->unBufferRef.pTpduName,                 
                                 phBuf->unBufferRef.pTpduName,               
                                 phBuf->ulLength);
             }                                                         
        else                                                             
            phEvData->unBufferRef.ulhandle = phBuf->unBufferRef.ulhandle;  
                                                                         
        phEvData->ulBufLen = phBuf->ulLength;
         
        if      (ro0032_localunlock (hBuf))     rc = -R_LULOKE;
        else if (ro0034_localfree   (hBuf))     rc = -R_LFREEE;
     } /* endof else by kc on 19/5 */
     if (rc) RM0003_RETURN (fnctName, signed short int, rc)
}
 
   /* ********** Unlock EvData .. Not Freed  ***************************** */

if (ro0032_localunlock (*hEvData))
    RM0003_RETURN (fnctName, signed short int, -R_LULOKE)             

RM0003_RETURN (fnctName, signed short int, rc) 
                                                                         
}/*end*/


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME 	: ro1521_CreateIvkEle					      */
/*                                                                            */
/* AUTHOR	: Vinita        					      */
/* DATE         : 93-03-20                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro1521_CreateIvkEle (

unsigned long int         ulIvkId,
unsigned long int         hBuf,
rT1530_stAsoContext  FAR *phCxt)

/* DESCRIPTION :                                                              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr*/

RM0001_STRG (fnctName, "ro1521_CreateIvkEle")
/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed	 short int	 rc=0;
rT1501_stBuffer         *phBuf;      
unsigned long  int       hInvokeData;
rT1531_stInvokeData     *phInvokeData;
unsigned long  int       hEvData;
rT1504_stEvData         *phEvData;
/* kc on 13-9 */                                                 
signed         long  int        lSize = sizeof(rT1504_stEvData); 


/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (fnctName)

   /* ********** alloc-lock to get pointer to Invoke Data **************** */

    if ( (hInvokeData = ro0030_localalloc (                                     
                     (unsigned long int) sizeof(rT1531_stInvokeData))) == NULL )
        RM0003_RETURN (fnctName, signed short int, -R_LALLCE)
                                                                              
    if ( (phInvokeData = (rT1531_stInvokeData *) 
                                       ro0031_locallock (hInvokeData)) == NULL )
        RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)
    if ( (phBuf = (rT1501_stBuffer *) ro0031_locallock (hBuf)) == NULL)       
            RM0003_RETURN (fnctName, signed short int, - R_LLOCKE)            
    if (phBuf->iMemType == RC1501_File)                                       
      lSize +=  phBuf->ulLength;                                              
    if ( (hEvData = ro0030_localalloc (lSize))==NULL)        
       RM0003_RETURN (fnctName, signed short int, - R_LALLCE)     

                                                                              
    if ( (phEvData = (rT1504_stEvData *) ro0031_locallock (hEvData)) == NULL) 
        RM0003_RETURN (fnctName, signed short int, - R_LLOCKE)             

   /* ********** If Buf provided lock & get pointer to Buf *************** */
   /* ********** If Buf provided lock & get pointer to Buf *************** */
   /* ********** Copy Information provided in Buf to Context ************* */
              
    
    phEvData->iMemType = phBuf->iMemType ;                                

    if (phBuf->iMemType == RC1500_Memory)                              
        phEvData->unBufferRef.pBufRef = phBuf->unBufferRef.pBufRef; 

    else if (phBuf->iMemType == RC1501_File) {
          phEvData->unBufferRef.pTpduName =                                  
               (char FAR *)((char FAR *) phEvData + sizeof(rT1504_stEvData));
          ro0041_memcpy ( phEvData->unBufferRef.pTpduName,                   
                          phBuf->unBufferRef.pTpduName,                      
                          phBuf->ulLength);                                  
    }                                                                                                 

    else                                                             
        phEvData->unBufferRef.ulhandle = phBuf->unBufferRef.ulhandle;  
                     
    phEvData->ulBufLen = phBuf->ulLength;
                                                                        
   /* ********** Set InvokeId as received in input parameter ************* */
   /* ********** Initiate hNext to NULL ********************************** */

    phInvokeData->ulInvokeId = ulIvkId ; 
    phInvokeData->hEvData = hEvData ; 
    phInvokeData->hNext = 0 ;                                                    
   /* ********** Prepend InvokeData handle in apma Context *************** */
   /* ********** for future use ,i.e. for result/error ind *************** */
  
      /* ********** If Operation handle already exists  *************** */
      /* ********** put it as next handle in new element ************** */

    if (phCxt->hOperHandle)
         phInvokeData->hNext = phCxt->hOperHandle;

     /* ********** Common Action : Copy this handle in *************** */
     /* ********** Cotext block.(hNext=0 for first element ) ********* */

    phCxt->hOperHandle = hInvokeData;                                       

   /* ********** Unlock InvokeData  .. Not Freed  ********************* */

    if      (ro0032_localunlock (hEvData)     ) rc = -R_LULOKE;
    else if (ro0032_localunlock (hInvokeData) ) rc = -R_LULOKE;
    else if (ro0032_localunlock (hBuf)        ) rc = -R_LULOKE;
    else if (ro0034_localfree   (hBuf)        ) rc = -R_LFREEE;

RM0003_RETURN (fnctName, signed short int, rc) 
                                                                         
}/*end*/


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME 	: ro1522_UserDataCp                                           */
/*                                                                            */
/* AUTHOR	: Vinita        					      */
/* DATE         : 93-03-20                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro1522_UserDataCp(

unsigned  long  int       hUserData,
unsigned  long  int       hEvData)

/* DESCRIPTION :                                                              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr*/

RM0001_STRG (fnctName, "ro1522_UserDataCp")
/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed	 short int	 rc = OK;
rTro04_stUserData   FAR *phUserData;
rTro04_stUserData   FAR *phExtData;
rT1504_stEvData     FAR *phEvData;
unsigned long int        count=0;
unsigned long int        i=0;
unsigned long int        DataLength=0;
unsigned long int        Numb=0;
char                FAR *pBufRef1;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */
RM0002_ENTRY (fnctName)

if ( !hUserData )                                              
       RM0003_RETURN (fnctName, signed short int, rc)          
else {                                                         
    if (!hEvData) {                                            
         ro0227_setSpecFlg();                                  
            RM0003_RETURN (fnctName, signed short int, rc);    
    }                                                          

if ( (phEvData = (rT1504_stEvData *)ro0031_locallock (hEvData)) == NULL)
     RM0003_RETURN (fnctName, signed short int, - R_LLOCKE)                 
   
   /* *************** lock the User Data  handle ************************* */
   /* *************** Get store handle of Data   ************************* */

if ( (phUserData = (rTro04_stUserData FAR *)
                                         ro0021_globallock (hUserData)) == NULL)
     RM0003_RETURN (fnctName, signed short int, -R_GLOCKE)

   /* *************** Check the condition that   ************************* */
   /* *************** sufficient buffer provided ************************* */

DataLength = phUserData->stStore.ulNumb ;
if ((phEvData->iMemType == RC1500_Memory) || 
    (phEvData->iMemType == RC1502_Handle)) 
{   
     DataLength = phUserData->stStore.ulNumb ;
     if (DataLength > phEvData->ulBufLen) goto specialFlag;

     if (phEvData->iMemType == RC1502_Handle)
     {  
         if ((pBufRef1 = (char FAR *)ro0021_globallock
                                      (phEvData->unBufferRef.ulhandle)) == NULL)
             RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)
     }
     else 
         pBufRef1 = phEvData->unBufferRef.pBufRef;

     rc = ro1523_EventDataCp (pBufRef1, (rT1504_stEvData FAR *)phEvData,
                              phUserData, count);
     if (rc)
         RM0003_RETURN (fnctName, signed short int, rc) 

     pBufRef1 += phUserData->stStore.ulNumb;
     Numb      = phUserData->stPdvHeader.lFraCount-1;
     /*by kc x-x-x 10-11-93 */
     phEvData->ulFragCount = phUserData->stPdvHeader.lFraCount; 

     if (Numb)
     {  
         for (i=0 ; i<Numb ; i++ )
         {
             if ( (phExtData = (rTro04_stUserData FAR *) ro0021_globallock
                               (phUserData->stPdvHeader.vPdvHandle[i])) == NULL)
                 RM0003_RETURN (fnctName, signed short int, -R_GLOCKE)
             DataLength += phExtData->stStore.ulNumb;
             if (DataLength > phEvData->ulBufLen)goto specialFlag;
             count++;
             rc = ro1523_EventDataCp (pBufRef1, (rT1504_stEvData FAR *)phEvData,
                                      phExtData, count);
             if (rc)
                 RM0003_RETURN (fnctName, signed short int, rc) 
             pBufRef1 += phExtData->stStore.ulNumb;
             if (ro0022_globalunlock (phUserData->stPdvHeader.vPdvHandle[i]))
                 RM0003_RETURN (fnctName, signed short int, -R_GULOKE )

         } /* end of for loop i = 0 to Numb */
     } /* end if Numb */

     if (phEvData->iMemType == RC1502_Handle) {
         if (ro0022_globalunlock (phEvData->unBufferRef.ulhandle))
             RM0003_RETURN (fnctName, signed short int, -R_GULOKE )
     }

} /* end if Memtype = Memory */
else
{
    rT00t1_stTpOpen   stTpOpen;
    rT00t3_stTpWrite  stTpWrite;
    rT00t5_stTpClose  stTpClose;
  /* ** by kc on 25-11-93 for the windows movable mem area *** */
    phEvData->unBufferRef.pTpduName = (char FAR *) ((char FAR *) phEvData +
                                          sizeof( rT1504_stEvData));
    pBufRef1 = phEvData->unBufferRef.pTpduName;

    stTpOpen.bIgnore              = FALSE;
    stTpOpen.pDataDescription     = pBufRef1;
    stTpOpen.usLenDataDescription = (unsigned short int)phEvData->ulBufLen;
    stTpOpen.iDir                 = TPOUT;
                                                                
    rc = ro00t1_tpOpen ((rT00t1_stTpOpen FAR *)&stTpOpen);
    if (rc)  goto specialFlag;
    stTpWrite.bIgnore = FALSE;            
    stTpWrite.sOpenId = stTpOpen.sOpenId;

    rc = ro1524_EventTpduCp ((rT00t3_stTpWrite FAR *)&stTpWrite, phUserData,
                                                                phEvData,count);
    if (rc) goto specialFlag;
    Numb = phUserData->stPdvHeader.lFraCount;
    phEvData->ulFragCount = phUserData->stPdvHeader.lFraCount;
    Numb = phUserData->stPdvHeader.lFraCount-1; /* x-x-x 10-11 kc */
    /* kc 10-11-93 x-x-x +1 is removed */
    if (Numb )
    {  
         for (i=0 ; i<Numb ; i++ )
         {
             if ( (phExtData = (rTro04_stUserData FAR *) ro0021_globallock
                               (phUserData->stPdvHeader.vPdvHandle[i])) == NULL)
                 RM0003_RETURN (fnctName, signed short int, -R_GLOCKE)
             count++;
             rc = ro1524_EventTpduCp ((rT00t3_stTpWrite FAR *)&stTpWrite,
                                                    phExtData, phEvData, count);
             if (rc){                                       
                 if (ro0022_globalunlock (phUserData->stPdvHeader.vPdvHandle[i]))
                       RM0003_RETURN (fnctName, signed short int, -R_GULOKE)
                 goto specialFlag;
             }
         if (ro0022_globalunlock (phUserData->stPdvHeader.vPdvHandle[i]))
             RM0003_RETURN (fnctName, signed short int, -R_GULOKE)
         }
         /* if (ro0022_globalunlock (phUserData->stPdvHeader.vPdvHandle[i]))
             RM0003_RETURN (fnctName, signed short int, -R_GULOKE)
        */
    }/* end if Numb */

    stTpClose.bIgnore = FALSE;
    stTpClose.sOpenId = stTpOpen.sOpenId;
  
    rc = ro00t5_tpClose ((rT00t5_stTpClose FAR *)&stTpClose);                  
    if (rc)
       goto specialFlag;
} /* end if  mem type is Tpdu */

if (!rc) {
    if (ro0075_deletestore (phUserData->stStore.hsStore))
         RM0003_RETURN (fnctName, signed int, -R_USTERR)
    if      (ro0032_localunlock  (hEvData)   != OK) rc = -R_LULOKE;
    else if (ro0022_globalunlock (hUserData) != OK) rc = -R_GULOKE; 
    else if (ro0024_globalfree   (hUserData) != OK) rc = -R_GFREEE;
   }
} /* kc on 14-9-93 for else part */
RM0003_RETURN (fnctName, signed short int, rc) 
specialFlag:
   /* if tpdu error then come here */ 
   rc = OK;
   phEvData->bSpecFlg = TRUE;
   phEvData->vFragArray[0].ulLength = DataLength;
   phEvData->vFragArray[0].iAbstrSynId =     
          phUserData->stPdvHeader.sAbstrSynId;
   
   ro0227_setSpecFlg();
   if (ro0075_deletestore (phUserData->stStore.hsStore))
     RM0003_RETURN (fnctName, signed int, -R_USTERR)
   if (ro0032_localunlock  (hEvData)   != OK) rc = -R_LULOKE;
   else if (ro0022_globalunlock (hUserData) != OK) rc = -R_LULOKE;
   if (ro0024_globalfree   (hUserData) != OK) rc = -R_GFREEE;
   RM0003_RETURN (fnctName, signed short int, rc )
                                                                         
}/*end*/


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME 	: ro1523_EventDataCp                                          */
/*                                                                            */
/* AUTHOR	: Vinita        					      */
/* DATE         : 93-03-20                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro1523_EventDataCp (
char                 FAR *pBufRef1,
rT1504_stEvData      FAR *phEvData,
rTro04_stUserData    FAR *phUserData, 
unsigned long int         count)

/* DESCRIPTION :                                                              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr*/

RM0001_STRG (fnctName, "ro1523_EventDataCp")
/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed	 short int	 rc=0;
signed long  int         lStoreBufferLength=0;
signed long  int         lOutRequiredDataLength=0;
signed long  int         lBytesRemaining=0;
char                FAR *pOutBufferAddress;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (fnctName)

   /* ********** Position store at offset given ************************** */

      rc = ro0066_storepos(phUserData->stStore.hsStore,
                           phUserData->stStore.ulOffset);
      if (rc )                                        
        RM0003_RETURN (fnctName, signed short int, -R_USTERR) 
                                                         
      lStoreBufferLength = ro0061_stbuflen (phUserData->stStore.hsStore); 

      lBytesRemaining = lOutRequiredDataLength = phUserData->stStore.ulNumb;
 
      do {                                                                      
          /************ No. of bytes to be read from STORE should ********/
          /************ not exceed STORE buffer size. ********************/
    
          lOutRequiredDataLength = MIN(lBytesRemaining, lStoreBufferLength);
     
          /* ********** Get the read pointer in STORE buffer *********** */
          /* ********** where data to be read is present. ************** */

          pOutBufferAddress = ro0072_stgets (phUserData->stStore.hsStore,   
                                             lOutRequiredDataLength);

          if (pOutBufferAddress == (char FAR *)LNULL)                 
            RM0003_RETURN (fnctName, signed short int, -R_USTERR)           
   
          ro0041_memcpy (pBufRef1, pOutBufferAddress, lOutRequiredDataLength);
          lBytesRemaining   -= lOutRequiredDataLength;   
          pBufRef1          += lOutRequiredDataLength;

        } while (lBytesRemaining > 0);                               

       phEvData->vFragArray[count].ulLength = phUserData->stStore.ulNumb;

       phEvData->vFragArray[count].iAbstrSynId = 
                 phUserData->stPdvHeader.sAbstrSynId;

RM0003_RETURN (fnctName, signed short int, rc) 
                                                                         
}/*end*/


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME 	: ro1524_EventTpduCp                                          */
/*                                                                            */
/* AUTHOR	: Vinita        					      */
/* DATE         : 93-03-23                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro1524_EventTpduCp(

rT00t3_stTpWrite     FAR *phTpWrite, 
rTro04_stUserData    FAR *phUserData, 
rT1504_stEvData      FAR *phEvData,
unsigned long int         count)

/* DESCRIPTION :                                                              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr*/

RM0001_STRG (fnctName, "ro1524_EventTpduCp")
/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed	 short int	 rc=0;
/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (fnctName)

     phTpWrite->lOutRequiredDataLength = phUserData->stStore.ulNumb; 
 
     rc = ro00t8_storeToTpdu (&phUserData->stStore,phTpWrite); 


     phEvData->vFragArray[count].ulLength = phUserData->stStore.ulNumb;

     phEvData->vFragArray[count].iAbstrSynId = 
                 phUserData->stPdvHeader.sAbstrSynId;


RM0003_RETURN (fnctName, signed short int, rc) 
                                                                         
}/*end*/


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME 	: ro1525_SearchIvkEle                                         */
/*                                                                            */
/* AUTHOR	: Vinita        					      */
/* DATE         : 93-03-25                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro1525_SearchIvkEle(

unsigned  long  int       ulInvokeId,
rT1530_stAsoContext  FAR *phCxt,    
unsigned  long  int  FAR *hEvData)

/* DESCRIPTION :                                                              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */

{/*entr*/

RM0001_STRG (fnctName, "ro1525_SearchIvkEle")
/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed	 short int	 rc=0;
rT1531_stInvokeData     *phInvokeData;
unsigned long  int       hCur;
unsigned long  int       hPrev;
rT1531_stInvokeData FAR *phPrev;
rT1531_stInvokeData FAR *phCur;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (fnctName)
                                                                               
   /* *************** lock the Association handle ************************ */  
   /* *************** stored in apma Context      ************************ */  
    
    if (phCxt->hOperHandle == NULL)                    
        RM0003_RETURN (fnctName, signed short int, - R_UBUFER)
                                                           
    if ( (phInvokeData = (rT1531_stInvokeData *)
                                 ro0031_locallock (phCxt->hOperHandle)) == NULL)
        RM0003_RETURN (fnctName, signed short int, - R_LLOCKE)
                                                                               
   /* *************** search for evdata handle meant for this  *********** */ 
   /* *************** particular invoke id & get a pointer for it ******** */    
    hCur   = phCxt->hOperHandle;
    phCur  = (rT1531_stInvokeData FAR *) phInvokeData;
    phPrev = (rT1531_stInvokeData FAR *) NULL;
    hPrev  = NULL;

    do
    {
      if (ulInvokeId != phCur->ulInvokeId)
      { 
         rc = ro1526_MovePtrsOnePos ((unsigned long int         FAR *)&hPrev,
                                     (unsigned long int         FAR *)&hCur,
                                     (rT1531_stInvokeData FAR * FAR *)&phPrev,
                                     (rT1531_stInvokeData FAR * FAR *)&phCur);
         if (rc) break; 
      }
      else
         break;   /* right element is found */
    }while (phCur->hNext > 0);

      /* ** necessary operation for preparing event data ************* */
    if (!rc)
    {
      *hEvData = phCur->hEvData;

      /* ****** unlock & free the used handle and complete the ********* */ 
      /* ****** link again by joining                          ********* */ 

      if (hPrev != NULL)
      {
         phPrev->hNext = phCur->hNext ;
      }
      if ( hCur == phCxt->hOperHandle ) 
          phCxt->hOperHandle = phCur->hNext;  /* kc on 17-11-93 */
    } /* end if no rc */

    if (hPrev != NULL)
    {
      if (ro0032_localunlock (hPrev) != 0)                    
          RM0003_RETURN (fnctName, signed short int, -R_LULOKE)  
    }
    if (hCur != NULL) {
      if (ro0032_localunlock (hCur) != 0)
          RM0003_RETURN (fnctName, signed short int, -R_LULOKE)  
      if (ro0034_localfree (hCur) != 0)                      
          RM0003_RETURN (fnctName, signed short int, -R_LFREEE)  
      /* Added by kc on 17/5/93 for error of lock error of this handle when 
         UNBINDrsp has arrived */
      /* start */
      /*  if ( hCur == phCxt->hOperHandle ) 
          phCxt->hOperHandle = NULL; */
      /* end */
 
    } /* if non-zero hCur */
RM0003_RETURN (fnctName, signed short int, rc) 
                                                                         
}/*end*/


/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME 	: ro1526_MovePtrsOnePos                                       */
/*                                                                            */
/* AUTHOR	: Vinita        					      */
/* DATE         : 93-03-25                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro1526_MovePtrsOnePos(

unsigned long int   FAR        * phPrev,
unsigned long int   FAR        * phCur, 
rT1531_stInvokeData FAR  * FAR * ppPrev,
rT1531_stInvokeData FAR  * FAR * ppCur)

/* DESCRIPTION :                                                              */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/* GLOBAL DATA (read only) : None                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr*/

RM0001_STRG (fnctName, "ro1526_MovePtrsOnePos")
/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

signed	 short int	 rc=0;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (fnctName)
      if (*phPrev != NULL) {                                         
         if (ro0032_localunlock (*phPrev))
            RM0003_RETURN (fnctName, signed short int, -R_LULOKE)    
      }                                                              

      *ppPrev = *ppCur;                                              
      *phPrev = *phCur;                                              
      *phCur  = (*ppCur)->hNext;                                  
      if (*phCur == NULL)
	  RM0003_RETURN (fnctName, signed short int, -R_UBUFER)                                                                    
      if ((*ppCur = (rT1531_stInvokeData *)ro0031_locallock (*phCur)) == NULL)
          rc = -R_LLOCKE;

RM0003_RETURN (fnctName, signed short int, rc) 
                                                                         
}/*end*/

/* EOF */
