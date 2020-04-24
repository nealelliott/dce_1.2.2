/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roloif22.c,v $
 * Revision 1.1.4.2  1996/03/11  13:22:48  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:52  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:14  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:51  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:03  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:04:53  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:36  keutel
 * 	creation
 * 	[1994/03/21  13:33:51  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roloif22.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:48 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1991                     */
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
/* TYPE         : C-MODULE                                            */
/*                                                                    */
/* NAME 	: roloif2.c					      */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE 	: 93-03-09					      */
/*                                                                    */
/* COMPONENT	: RTROS lower interface				      */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION	: This module contains functions which manage	      */
/*		  the PSAP address structure.			      */
/*                                                                    */
/* SYSTEM DEPENDENCE :						      */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 2.0A00 K00| 25-05-92| original: functions got from  |ns  |	      */
/*	     |	       | "roloif0.c"		       |    |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/
 
/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/
 
static char *sccsid =    "@(#) roloif22.c 1.1 93/11/29   RTS-V2.00";

/**********************************************************************/
/*              I N C L U D E S                                       */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <cmx.h>          /* interface to the SINIX transport system  */

#include <rogenrl.h>      /* General definitions                      */
#include <roerval.h>      /* General definitions (RTS error values)   */
#include <routil0.h>      /* General utilities                        */
#include <rouply0.h>      /* Common upper layer definitions.          */
#include <rodpat0.h>      /* Dispatcher - PM interface                */

#include <roloif2.h>      /* LOIF calling interface                   */
#include <roloif32.h>      /* Private definitions for LOIF-modules     */
#include <roloif42.h>
 
/*********************************************************************/
/*              G L O B A L  V A R I A B L E S                       */
/*********************************************************************/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0306_GetAddress				      */
/*                                                                    */
/* AUTHOR       : Shanbhag, sanjay.                                   */
/* DATE 	: 93-03-15					      */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    Gets the address part for the Layer out of the PSAP address     */
/*    structure whose handle is given.				      */
/*    Returns the length of the address part.			      */
/*    From the PSAP address structure (hAddr parameter) takes out     */
/*    the PSelector or the SSelector when the presentation or the     */
/*    Session layers are chosen respectively.When the Transport       */
/*    layer is chosen the function returns the Transport address  or  */
/*    selector which is in the form of structure t_partadr or         */
/*    structure t_myname.(Since the first three elements of the       */
/*    structures t_adrdesc,t_myname,t_partaddr are of the same type   */
/*    it is possible to access the first three elements of  any of    */
/*    these structures with a pointer to any one the structures       */
/*    mentioned above.Normally a pointer to structure t_adrdesc is    */
/*    used because this structure is binary compatible across various */
/*    directories. Typecasting is done before passing to CMX funct-   */
/*    ions.)                                                          */
/*    When the length of the buffer provided is less than the         */
/*    selector length the function returns an error.                  */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*								      */
/*    hAddr	 : Handle of PSAP address structure		      */
/*    usLayer	 : Layer number of the protocol machine 	      */
/*    usLen	 : Length of buffer				      */
/*                                                                    */
/* OUT-PARAMETERS:						      */
/*                                                                    */
/*    pAddr	 : Buffer to store address part 		      */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*		>= 0 : Successful and address length		      */
/*		<  0 : Error:					      */
/*			  -R_NOTIMP				      */
/*			  -R_LLOCKE				      */
/*			  -R_LULOKE				      */
/*			  -R_NOENME				      */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS		  :					      */
/*                                                                    */
/*inoff ***************************************************************/
 
signed short int PASCAL ro0306_GetAddress (

unsigned long int    hAddr,
unsigned short int   usLayer,
unsigned short int   usLen,
char FAR	    *pAddr)

{
   RM0001_STRG (fnctName, "ro0306_GetAddress")

   struct t_adrdesc    *pAdrDesc;
   char 	       *pAllAddr;
   unsigned short int	usTmpLen;
 
   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   usTmpLen = 0;

   /*----------------------------*/
   /* Lock the address structure */
   /*----------------------------*/

   if (!hAddr) {

      /*----------------------------------*/
      /* If the address is a NULL handle, */
      /* return OK but address length = 0 */
      /*----------------------------------*/

       RM0003_RETURN (fnctName, signed short int, 0)
   }

   if ((pAllAddr = ro0031_locallock (hAddr)) == NULL) {
      RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)
   }

   pAdrDesc = (struct t_adrdesc *)pAllAddr;
   if (usLayer == RCro11_Pselector) {

      if (pAdrDesc->t_infotype == T_PSAPINFO) { /* id. for PSEL component */

         usTmpLen = (unsigned short int)pAdrDesc->t_infolng;
         if (usLen < usTmpLen) {

	 /*----------------------------*/
	 /* The buffer used to get the */
	 /* address is not big enougth */
	 /*----------------------------*/

             RM0003_RETURN (fnctName, signed short int, -R_NOENME)
          }

      /*------------------------------------*/
      /* Copy the PSelector into the buffer */
      /*------------------------------------*/

            ro0041_memcpy ((char FAR *)pAddr,
		     (char FAR *)(pAllAddr + sizeof(struct t_adrdesc)),
		     (unsigned long int)usTmpLen);
      }
   /* else ==> there is no PSEL in this address ==> return 0 */

   } else if (usLayer == RCro13_Sselector) {
      if (pAdrDesc->t_infotype == T_PSAPINFO) { /* id. for PSEL component */
         pAllAddr += pAdrDesc->t_nextoff;
         pAdrDesc = (struct t_adrdesc *)pAllAddr;
      }
      if (pAdrDesc->t_infotype == T_SSAPINFO) { /* id. for SSEL component */
      
         usTmpLen = (unsigned short int)pAdrDesc->t_infolng;
         if (usLen < usTmpLen) {

	 /*----------------------------*/
	 /* The buffer used to get the */
	 /* address is not big enougth */
	 /*----------------------------*/

             RM0003_RETURN (fnctName, signed short int, -R_NOENME)
          }

      /*------------------------------------*/
      /* Copy the SSelector into the buffer */
      /*------------------------------------*/

          ro0041_memcpy ((char FAR *)pAddr,
		     (char FAR *)(pAllAddr + sizeof(struct t_adrdesc)),
		     (unsigned long int)usTmpLen);
      }
   /* else ==> there is no SSEL in this address ==> return 0 */
   } else if (usLayer == RCro14_TransportAddress) {

      if (pAdrDesc->t_infotype == T_PSAPINFO) { /* id. for PSEL component */
         pAllAddr += pAdrDesc->t_nextoff;
         pAdrDesc = (struct t_adrdesc *)pAllAddr;
      }
      if (pAdrDesc->t_infotype == T_SSAPINFO) { /* id. for SSEL component */
         pAllAddr += pAdrDesc->t_nextoff;
         pAdrDesc = (struct t_adrdesc *)pAllAddr;
      }

      usTmpLen = (unsigned short int)pAdrDesc->t_translng;
      if (usLen < usTmpLen) {

	 /*----------------------------*/
	 /* The buffer used to get the */
	 /* address is not big enougth */
	 /*----------------------------*/

          RM0003_RETURN (fnctName, signed short int, -R_NOENME)
      }

      /* pMyname = (struct t_myname *)pAdrDesc;

      switch (pMyname->t_mnmode) {

         case T_MNMODE :
               
            usTmpLen = pMyname->t_mnlng;
            if (usLen < usTmpLen) {

               RM0003_RETURN (fnctName, signed short int, -R_NOENME)
            }
            break;

         case T_PAMODE :
               
            pPartAddr = (struct t_partaddr *)pAdrDesc;
            usTmpLen = pPartAddr->t_palng;
            if (usLen < usTmpLen) {

               RM0003_RETURN (fnctName, signed short int, -R_NOENME)
            }
            break;
       } */   

      /*-----------------------------------------------------------------*/
      /* Copy the TSelector (struct t_myname/t_partaddr) into the buffer */
      /*-----------------------------------------------------------------*/

       ro0041_memcpy ((char FAR *)pAddr,
		     (char FAR *)pAllAddr,
		     (unsigned long int)usTmpLen);

   } else {      /* if usLayer is invalid */

      RM0003_RETURN (fnctName, signed short int, -R_NOTIMP)
   }

   if (ro0032_localunlock (hAddr) != 0) {
      RM0003_RETURN (fnctName, signed short int, -R_LULOKE)
   }

   RM0003_RETURN (fnctName, signed short int, usTmpLen)
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0307_PutAddress				      */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE 	: 93-03-15					      */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    Puts the given selector of the Layer into the address structure */
/*    whose handle is given. If the given handle is NULL, a new       */
/*    buffer will not be allocated and a 'not implemented' error will */
/*    be returned.( A buffer containing at least a transport selector */
/*    is to be supplied and only Presentation and Session selectors   */
/*    can be added.)                                                  */
/*    When the given handle points to an existing address then  a     */
/*    new buffer with the length of the existing address plus the     */
/*    length of the address to be copied is allocated, then the       */
/*    header plus the new selector is copied into buffer and the      */
/*    existing address is appended to this selector.  The old buffer  */
/*    is then deleted.                                                */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*								      */
/*    phAddr	 : Pointer to handle of address buffer		      */
/*    usLayer	 : Layer number of the protocol machine 	      */
/*    usLen	 : Length of the address part			      */
/*    pAddr	 : Buffer containing the address part		      */
/*                                                                    */
/* OUT-PARAMETERS: None 					      */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*		>= 0 : Successful and address length		      */
/*		<  0 : Error:					      */
/*			  -R_NOTIMP				      */
/*			  -R_LALLCE				      */
/*			  -R_LLOCKE				      */
/*			  -R_LULOKE				      */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS		  :					      */
/*                                                                    */
/*inoff ***************************************************************/
 
signed short int PASCAL ro0307_PutAddress (

unsigned long  int FAR	*phAddr,
unsigned short int	 usLayer,
unsigned short int	 usLen,
char FAR		*pAddr)

{
   RM0001_STRG (fnctName, "ro0307_PutAddress")

   struct t_adrdesc    *pAdrDesc;
   char 	       *pExAddr;
   unsigned long  int	hNewAddr;
   char 	       *pNewAddr;
   unsigned short int	usExLen;
   unsigned short int	usTotLen;
 
   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)


   if (*phAddr == NULL) {

      /*----------------------------------------*/
      /* The address should always contain a    */
      /* Tansport selector.                     */
      /*----------------------------------------*/

      RM0003_RETURN (fnctName, signed short int, -R_NOTIMP)

   } 
      /*---------------------------*/
      /* Lock the existing address */
      /*---------------------------*/

   if ((pExAddr = ro0031_locallock (*phAddr)) == NULL)
      RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)

      pAdrDesc = (struct t_adrdesc *)pExAddr;
      usExLen    = pAdrDesc->t_translng;

   /*----------------------------------------*/
   /* Get the total length: the existing one */
   /* + the one which has to be added	     */
   /*----------------------------------------*/

   usTotLen   = usExLen + sizeof(struct t_adrdesc) + usLen;

   /*-------------------------------------*/
   /* Allocate memory for the new address */
   /*-------------------------------------*/

   if ((hNewAddr = ro0030_localalloc (usTotLen)) == 0) {
      RM0003_RETURN (fnctName, signed short int, -R_LALLCE)
   }

   if ((pNewAddr = ro0031_locallock (hNewAddr)) == NULL) {
      RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)
   }

   pAdrDesc = (struct t_adrdesc *)pNewAddr;
   if (usLayer == RCro11_Pselector) {

   /*-----------------------------------------------------*/
   /* Fill in the descriptive structure for the PSelector */
   /*-----------------------------------------------------*/

   pAdrDesc->t_infotype = T_PSAPINFO;
   pAdrDesc->t_infovers = T_PSELVERS;
   pAdrDesc->t_translng = usTotLen;
   pAdrDesc->t_infolng	= usLen;
   pAdrDesc->t_nextoff	= sizeof(struct t_adrdesc) + usLen;

   }
   if (usLayer == RCro13_Sselector) {

   /*-----------------------------------------------------*/
   /* Fill in the descriptive structure for the SSelector */
   /*-----------------------------------------------------*/

   pAdrDesc->t_infotype = T_SSAPINFO;
   pAdrDesc->t_infovers = T_SSELVERS;
   pAdrDesc->t_translng = usTotLen;
   pAdrDesc->t_infolng	= usLen;
   pAdrDesc->t_nextoff	= sizeof(struct t_adrdesc) + usLen;

   }
   /*----------------------------------------------------*/
   /* Copy the PSelector after the descriptive structure */
   /*----------------------------------------------------*/

   ro0041_memcpy ((char FAR *)(pNewAddr + sizeof(struct t_adrdesc)),
		  (char FAR *)pAddr,
		  (unsigned long int)usLen);


      /*--------------------------------------------------*/
      /* Copy the rest of the address after the PSelector */
      /*--------------------------------------------------*/

      ro0041_memcpy ((char FAR *)(pNewAddr + pAdrDesc->t_nextoff),
		     (char FAR *)pExAddr,
		     (unsigned long int)usExLen);

      if (ro0032_localunlock (*phAddr) != 0) {
	 RM0003_RETURN (fnctName, signed short int, -R_LULOKE)
      }

      if (ro0034_localfree (*phAddr) != 0) {
	 RM0003_RETURN (fnctName, signed short int, -R_LFREEE)
      }

   *phAddr = hNewAddr;

   if (ro0032_localunlock (*phAddr) != 0) {
      RM0003_RETURN (fnctName, signed short int, -R_LULOKE)
   }

   RM0003_RETURN (fnctName, signed short int, usTotLen)
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0308_getAddrLen				      */
/*                                                                    */
/* AUTHOR       : Shanbhag, sanjay.                                   */
/* DATE 	: 93-03-15					      */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    Gets the length of an address.				      */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*								      */
/*    hAddr	 : Address handle				      */
/*                                                                    */
/* OUT-PARAMETERS:						      */
/*                                                                    */
/*    pLen	 : Pointer to where the length is stored	      */
/*                                                                    */
/* RETURN VALUES:                                                     */
/*                0 : Successful                                      */
/*		< 0 : Error					      */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS		  :					      */
/*                                                                    */
/*inoff ***************************************************************/
 
signed short int PASCAL ro0308_getAddrLen (

unsigned long int FAR  *pLen,
unsigned long int	hAddr)

{
   RM0001_STRG (fnctName, "ro0308_getAddrLen")

   struct t_adrdesc  *pAdrDesc;
   char 	     *pAddr;
 
   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   *pLen = 0;

   if (!hAddr) {

      /*----------------------------------*/
      /* If the address is a NULL handle, */
      /* return OK but address length = 0 */
      /*----------------------------------*/

      RM0003_RETURN (fnctName, signed short int, 0)
   }
   /*----------------------------*/
   /* Lock the address structure */
   /*----------------------------*/

   if ((pAddr = ro0031_locallock (hAddr)) == NULL) {
      RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)
   }

   /*------------------------------*/
   /* Get the address length value */
   /*------------------------------*/

   pAdrDesc = (struct t_adrdesc *)pAddr;
   *pLen    = (unsigned long int)pAdrDesc->t_translng;

   /*------------------------------*/
   /* Unlock the address structure */
   /*------------------------------*/

   if (ro0032_localunlock (hAddr) != 0) {
      RM0003_RETURN (fnctName, signed short int, -R_LULOKE)
   }
 
   RM0003_RETURN (fnctName, signed short int, OK)
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0354_getAddressWithoutPandSsel	              */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE 	: 93-03-09					      */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*    It prepares a TSAP address (incoming parameter, P1) to be used  */
/*    by the CMX (SINIX, V3.0) product. 			      */
/*    The function gives the pointer to the Transport address without */
/*    the PSelector & SSelector.				      */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*								      */
/*    pAllAddr	 : Pointer to the PSAP address   		      */
/*                                                                    */
/* OUT-PARAMETERS:						      */
/*                                                                    */
/*    pAddr	 : Pointer to the address without PSEL & SSEL	      */
/*                                                                    */
/* RETURN VALUES: Void						      */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (modified) :                                           */
/*                                                                    */
/* REMARKS		  :					      */
/*                                                                    */
/*inoff ***************************************************************/

void PASCAL ro0354_getAddressWithoutPandSsel (

char   *pAllAddr,	   /* pointer to the PSAP address             */
char  **pAddr)		   /* pt. to the address without PSEL  & SSEL */

{
   RM0001_STRG (fnctName, "ro0354_getAddressWithoutPandsel")

   struct t_adrdesc  *pAdrDesc;
 
   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   pAdrDesc = (struct t_adrdesc *)pAllAddr;

   if (pAdrDesc->t_infotype == T_PSAPINFO) {	   /* id. for PSEL component */
      pAllAddr += pAdrDesc->t_nextoff;
      pAdrDesc = (struct t_adrdesc *)pAllAddr;
   } 
   if (pAdrDesc->t_infotype == T_SSAPINFO) {      /* id. for SSEL component */
      pAllAddr += pAdrDesc->t_nextoff;
   }
   *pAddr = pAllAddr;

   RM0004_RETURN_VOID (fnctName)
}


/********************	    END OF ROLOIF2.C	   ********************/
