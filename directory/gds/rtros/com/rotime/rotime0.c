/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rotime0.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:59  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:06  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:32:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:14  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:07:44  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:49  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:39  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:54:21  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:18  keutel
 * 	creation
 * 	[1994/03/21  13:24:05  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: rotime0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:59 $" } ;
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
/* NAME         : rotime0.c                                           */
/*                                                                    */
/* AUTHOR       : Natalia Serra                                       */
/* DATE         : 19-9-91                                             */
/*                                                                    */
/* COMPONENT    :                                                     */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 2.0A00 K00| 19-09-91| original                      |ns  |         */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                     Module Identification                          */
/**********************************************************************/

static char *sccsid =    "@(#) rotime0.c 1.1 93/11/29   RTS-V2.00";

/**********************************************************************/
/*              I N C L U D E S                                       */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include "rogenrl.h"	  /* General definitions		      */
#include "roerval.h"	  /* General definitions (RTS error values)   */
#include "rolgval.h"	  /* General definitions		      */
#include "routil0.h"	  /* General utilities			      */
#include "rodpat0.h"	  /* Dispatcher - PM interface		      */

#include "rotime0.h"	  /* Timer interface			      */
#include "rotime2.h"	  /* Timer calling interface		      */
#include "rotime3.h"	  /* Timer interface definitions	      */

#include "rotime10.h"

/**********************************************************************/
/*                                                                    */
/*              E X T E R N A L   D A T A                             */
/*                                                                    */
/**********************************************************************/


/**********************************************************************/
/*                                                                    */
/*              G L O B A L   D A T A                                 */
/*                                                                    */
/**********************************************************************/

static unsigned long int   hFirstElem = NULL;  /* mem hdl of the first elem */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1100_timer()                                      */
/*                                                                    */
/* AUTHOR       : Natalia Serra                                       */
/* DATE         : 19-09-91                                            */
/*                                                                    */
/* DESCRIPTION  : This function is similar to a PM module.            */
/*                It gets an event (ro0210_popInEvent() function)     */
/*                from the dispatcher and processes it.               */
/*                Events:                                             */
/*                - RC1101_SetTimerReq: It sets a new entry in the    */
/*                     list of calls to the timer.                    */
/*		  - RC1102_ResetTimerReq: It resets specific entries  */
/*		       of the list.				      */
/*                - RC1103_GetEventReq: It gives a new event to       */
/*                     the dispatcher (time expired).                 */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL ro1100_timer (
	unsigned long int   lContext)

{
   RM0001_STRG (fnctName, "ro1100_timer")

   signed	 int   iInEvent;     /* incoming event	      */
   unsigned long int   ulP1;         /* event parameter P1    */
   unsigned long int   ulP2;         /* event parameter P2    */
   unsigned long int   ulP3;         /* event parameter P3    */
   unsigned long int   ulP4;         /* event parameter P4    */
   unsigned long int   ulAlarmTime;  /* system time + delay   */
   unsigned long int   hACBAux;      /* ACB memory handle     */
   unsigned long int   hECBAux;      /* ECB memory handle     */
   unsigned long int   hNewElem;     /* new element's mem hdl */
   rT1100_ctrlTimer   *pNewElem;     /* mem ptr to a new elem */
   unsigned long int   hPrev;	     /* auxiliar mem handle   */
   unsigned long int   hCur;	     /* auxiliar mem handle   */
   rT1100_ctrlTimer   *pPrev;	     /* auxiliar pointer      */
   rT1100_ctrlTimer   *pCur;	     /* auxiliar pointer      */
   signed short int    retVal;	     /* return code	      */

   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   retVal = OK;

   /*--------------------*/
   /* Pop incoming event */
   /*--------------------*/

   iInEvent = (signed int)ro0210_popInEvent (&ulP1, &ulP2, &ulP3, &ulP4);

   /*-------------------------*/
   /* Write interface logging */
   /*-------------------------*/

   hACBAux = ro0214_getAcbEcb (&hECBAux);

   ro0043_pack10Log (RC0022_ACTIVITY,
		     RL1101_ActivityTimer,
		     6,
		     (signed short int) sizeof (hECBAux),
		     (char FAR	     *) &hECBAux,
		     (signed short int) sizeof (hACBAux),
		     (char FAR	     *) &hACBAux,
		     (signed short int) sizeof (iInEvent),
		     (char FAR	     *) &iInEvent,
		     (signed short int) sizeof (ulP1),
		     (char FAR	     *) &ulP1,
		     (signed short int) sizeof (ulP3),
		     (char FAR	     *) &ulP3,
		     (signed short int) sizeof (ulP4),
		     (char FAR	     *) &ulP4);

   switch (iInEvent) {

      case RC1101_SetTimerReq:         /* Set a new entry in the list */
            
	 /*-----------------------------------------------*/
	 /* Allocate memory for a new element in the list */
	 /*-----------------------------------------------*/

	 if ((hNewElem = ro0030_localalloc (sizeof(rT1100_ctrlTimer)))
             == NULL) {
	    retVal = -R_LALLCE;
            break;
         }
	 if ((pNewElem = (rT1100_ctrlTimer *)ro0031_locallock (hNewElem))
	     == NULL) {
	    ro0034_localfree (hNewElem);
	    retVal = -R_LLOCKE;
            break;
         }

	 /*--------------------------------*/
	 /* Look for the correct position  */
	 /* of the new element in the list */
	 /*--------------------------------*/

         ulAlarmTime = ro0040_time() + ulP1;

	 if (hFirstElem == NULL) {

            /*----------------------------------*/
            /* The list of timer calls is empty */
            /*----------------------------------*/

	    hFirstElem	    = hNewElem;
	    pNewElem->hNext = NULL;

	 } else {

            /*------------------------------------------------------*/
            /* The list is not empty: look for the correct position */
	    /*------------------------------------------------------*/

	    hPrev = NULL;
	    pPrev = (rT1100_ctrlTimer *)NULL;
	    hCur  = hFirstElem;

	    if ((pCur = (rT1100_ctrlTimer *)ro0031_locallock (hCur)) == NULL) {
	       retVal = -R_LLOCKE;
	       break;
	    }

	    do {
	       if (pCur->ulTimeDelay <= ulAlarmTime) {

		  if ((retVal = ro1110_movePtrsOnePos (
                           (unsigned long int FAR *)&hPrev,
                           (unsigned long int FAR *)&hCur,
                           (rT1100_ctrlTimer  FAR * FAR *)&pPrev,
		           (rT1100_ctrlTimer  FAR * FAR *)&pCur, FALSE)) < 0) {
		     RM0003_RETURN (fnctName, signed short int, retVal)
		  }

	       } else {
		  break;    /* position found */
	       }

	    } while (hCur != NULL);

	    /*-------------------------------------------*/
	    /* Insert the element between pPrev and pCur */
	    /*-------------------------------------------*/

	    pNewElem->hNext = hCur;

	    if (hPrev == NULL) {

	       /*------------------------------------------*/
	       /* The new element is the first in the list */
	       /*------------------------------------------*/

	       hFirstElem = hNewElem;

	    } else {

	       /*----------------------------------*/
	       /* The new element is not the first */
	       /*----------------------------------*/

	       pPrev->hNext = hNewElem;
	    }

	    if (hPrev != NULL) {
	       if (ro0032_localunlock (hPrev) != 0) {
		  retVal = -R_LULOKE;
		  break;
	       }
	       hPrev = NULL;
	    }

	    if (hCur != NULL) {
	       if (ro0032_localunlock (hCur) != 0) {
		  retVal = -R_LULOKE;
		  break;
	       }
	       hCur = NULL;
	    }
         }

	 /*--------------------------------------------------*/
	 /* Fill the new element with the parameter's values */
	 /*--------------------------------------------------*/

         pNewElem->ulTimeDelay = ulAlarmTime;
	 pNewElem->hACB        = hACBAux;
	 pNewElem->hECB        = hECBAux;
         pNewElem->ulModuleNum = ulP3;
	 pNewElem->ulTimerInvo = ulP4;

	 if (ro0032_localunlock (hNewElem) != 0) {
	    retVal = -R_LULOKE;
	    break;
	 }

	 lContext++;
	 ro0226_changeState (lContext);

         break;
                                
      case RC1102_ResetTimerReq:      /* Reset one entry of the list  */

	 if (hFirstElem == NULL) {

	    /*------------------*/
	    /* No timer entries */
	    /*------------------*/

	    break;
	 }

	 hPrev = NULL;
	 pPrev = (rT1100_ctrlTimer *)NULL;
	 hCur  = hFirstElem;

	 if ((pCur = (rT1100_ctrlTimer *)ro0031_locallock (hCur)) == NULL) {
	    retVal = -R_LLOCKE;
	    break;
	 }

	 do {

	    /*---------------------------------------*/
	    /* Look for the entry(ies) to be deleted */
	    /*---------------------------------------*/

	    if ((pCur->hACB == hACBAux) &&
		(pCur->ulModuleNum == ulP3) &&
		((ulP1 != 0) ||
		 ((ulP1 == 0) && (pCur->ulTimerInvo == ulP4)))) {

	       /*---------------------*/
	       /* When found, free it */
	       /*---------------------*/

	       if ((retVal = ro1110_movePtrsOnePos (
                            (unsigned long int  FAR *)&hPrev,
                            (unsigned long int  FAR *)&hCur,
                            (rT1100_ctrlTimer   FAR * FAR *)&pPrev,
		            (rT1100_ctrlTimer   FAR * FAR *)&pCur, TRUE)) < 0) {
		  RM0003_RETURN (fnctName, signed short int, retVal)
	       }

	       lContext--;
	       ro0226_changeState (lContext);

	       if (ulP1 == 0) {

		  /*-----------------------------------*/
		  /* Reset only the first timer call   */
		  /* identified by ulP4: stop the loop */
		  /*-----------------------------------*/

		  break;
	       }

	    } else {

	       /*------------------------------------------*/
	       /* Still not found: continue looking for it */
	       /*------------------------------------------*/

	       if ((retVal = ro1110_movePtrsOnePos (
                            (unsigned long int FAR  *)&hPrev,
                            (unsigned long int FAR  *)&hCur,
                            (rT1100_ctrlTimer  FAR * FAR *)&pPrev,
		            (rT1100_ctrlTimer  FAR * FAR *)&pCur, FALSE)) < 0) {
		  RM0003_RETURN (fnctName, signed short int, retVal)
	       }
	    }

	 } while (hCur != NULL);

	 if (hPrev != NULL) {
	    if (ro0032_localunlock (hPrev) != 0) {
	       retVal = -R_LULOKE;
	       break;
	    }
	    hPrev = NULL;
	 }

	 if (hCur != NULL) {
	    if (ro0032_localunlock (hCur) != 0) {
	       retVal = -R_LULOKE;
	       break;
	    }
	    hCur = NULL;
	 }

         break;
                                
      case RC1103_GetEventReq:

	 /*-------------------------------------------*/
	 /* It gives the indication to the dispatcher */
	 /* that one timer block has already expired  */
	 /*-------------------------------------------*/

	 if ((pCur = (rT1100_ctrlTimer *)ro0031_locallock (hFirstElem))
	     == NULL) {
	    retVal = -R_LLOCKE;
	    break;
	 }

	 ulP3 = pCur->ulModuleNum;
	 ulP4 = pCur->ulTimerInvo;

	 hCur = pCur->hNext;

	 if (ro0032_localunlock (hFirstElem) != 0) {
	    retVal = -R_LULOKE;
	    break;
	 }

	 if (ro0034_localfree (hFirstElem) != 0) {
	    retVal = -R_LFREEE;
	    break;
	 }

	 hFirstElem = hCur;

	 lContext--;
	 ro0226_changeState (lContext);

	 retVal = ro0211_pushOutEvent ((signed short int)ulP3, /* pm number */
				       RC1104_TimerInd,        /* pm event  */
				       NULL,
				       NULL,
				       NULL,
				       ulP4);
         break;

      default:
	 retVal = -R_TUNEVT;
         break;
   }

   RM0003_RETURN (fnctName, signed short int, retVal)
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1101_cleanUp()                                    */
/*                                                                    */
/* AUTHOR       : Natalia Serra                                       */
/* DATE         : 19-09-91                                            */
/*                                                                    */
/* DESCRIPTION	: This function is similar to all the cleanUp func-   */
/*		  tions but in the Timer PM module, nothing is done.  */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL ro1101_cleanUp (
	signed	 long int   pmEvent,
	unsigned long int   ulP1,
	unsigned long int   ulP2,
	unsigned long int   ulP3,
	unsigned long int   ulP4)

{
   RM0001_STRG (fnctName, "ro1101_cleanUp")
   RM0002_ENTRY (fnctName)
   RM0003_RETURN (fnctName, signed short int, OK)
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro1102_checkTimer()                                 */
/*                                                                    */
/* AUTHOR       : Natalia Serra                                       */
/* DATE         : 19-09-91                                            */
/*                                                                    */
/* DESCRIPTION  : This function has to check the list of timer calls. */
/*                Because it is a sorted list (increasing), it only   */
/*                has to check the first element of the list to know  */
/*                whether one set clock has expired or not yet.       */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL ro1102_checkTimer (
	unsigned long int FAR  *hACB,
	signed	 long int FAR  *pEvent)

{
   RM0001_STRG (fnctName, "ro1102_checkTimer")

   rT1100_ctrlTimer  *pFirstElem;
   signed short int   retVal;	     /* return code	      */

   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   /*------------------------------*/
   /* Lock the first timer element */
   /*------------------------------*/

   if (hFirstElem == NULL) {
      *hACB   = 0;
      *pEvent = 0;
      RM0003_RETURN (fnctName, signed short int, 2)
   }

   if ((pFirstElem = (rT1100_ctrlTimer *)ro0031_locallock (hFirstElem))
       == NULL) {
      RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)
   }

   /*-----------------------------------*/
   /* Check if the time has expired yet */
   /*-----------------------------------*/

   if (ro0040_time() >= pFirstElem->ulTimeDelay) {
      retVal  = 0;		     /* One timer bloc has expired    */
      *hACB   = pFirstElem->hACB;
      *pEvent = RC1103_GetEventReq;
   } else {
      retVal  = 2;		     /* No timer bloc has expired yet */
      *hACB   = 0;
      *pEvent = 0;
   }

   /*--------------------------------*/
   /* Unlock the first timer element */
   /*--------------------------------*/

   if (ro0032_localunlock (hFirstElem) != 0) {
      retVal = -R_LULOKE;
   }

   RM0003_RETURN (fnctName, signed short int, retVal)
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1103_clearTimerQueue()			      */
/*                                                                    */
/* AUTHOR       : Natalia Serra                                       */
/* DATE 	: 05-12-91					      */
/*                                                                    */
/* DESCRIPTION  : This function frees all the elements of the list    */
/*                of calls to the timer belonging to a specific       */
/*		  ECB (which is the parameter of the function).       */
/*                                                                    */
/*inoff ***************************************************************/

signed short int PASCAL ro1103_clearTimerQueue (
	unsigned long int   hECB)

{
   RM0001_STRG (fnctName, "ro1103_clearTimerQueue")

   unsigned long int   hPrev;	     /* auxiliar mem handle   */
   unsigned long int   hCur;	     /* auxiliar mem handle   */
   rT1100_ctrlTimer   *pPrev;	     /* auxiliar pointer      */
   rT1100_ctrlTimer   *pCur;	     /* auxiliar pointer      */
   signed short int    retVal;	     /* return code	      */

   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   retVal = OK;

   if (hFirstElem != NULL) {

      /*------------------------*/
      /* The queue is not empty */
      /*------------------------*/

      hPrev = NULL;
      pPrev = (rT1100_ctrlTimer *)NULL;
      hCur  = hFirstElem;

      /*------------------------*/
      /* Lock the first element */
      /*------------------------*/

      if ((pCur = (rT1100_ctrlTimer *)ro0031_locallock (hCur))
	  == NULL) {
	 RM0003_RETURN (fnctName, signed short int, -R_LLOCKE)
      }

      do {

	 /*------------------------------------*/
	 /* Test all the elements of the queue */
	 /*------------------------------------*/

	 if (pCur->hECB != hECB) {

	    /*--------------------------------*/
	    /* It does not have to be deleted */
	    /*--------------------------------*/

	    if ((retVal = ro1110_movePtrsOnePos (
                           (unsigned long int FAR *)&hPrev, 
                           (unsigned long int FAR *)&hCur, 
                           (rT1100_ctrlTimer  FAR * FAR *)&pPrev,
		           (rT1100_ctrlTimer  FAR * FAR *)&pCur, FALSE)) < 0) {
	       RM0003_RETURN (fnctName, signed short int, retVal)
	    }

	 } else {

	    /*-------------------------------*/
	    /* The element has to be deleted */
	    /*-------------------------------*/

	    if ((retVal = ro1110_movePtrsOnePos (
                             (unsigned  long int FAR *)&hPrev, 
                             (unsigned  long int FAR *)&hCur, 
                             (rT1100_ctrlTimer   FAR * FAR *)&pPrev,
		             (rT1100_ctrlTimer   FAR * FAR *)&pCur, TRUE)) < 0){
	       RM0003_RETURN (fnctName, signed short int, retVal)
	    }
	 }
      } while (hCur != NULL);

      if (hPrev != NULL) {
	 if (ro0032_localunlock (hPrev) != 0) {
	    retVal = -R_LULOKE;
	 }
	 hPrev = NULL;
      }
   }

   RM0003_RETURN (fnctName, signed short int, retVal)
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro1110_movePtrsOnePos()			      */
/*                                                                    */
/* AUTHOR       : Natalia Serra                                       */
/* DATE 	: 06-12-91					      */
/*                                                                    */
/* DESCRIPTION	: This function works with the list of calls to the   */
/*		  timer. The memory handles and the pointers to two   */
/*		  elements in the list are incoming and outgoing pa-  */
/*		  rameters: the function moves them one position,     */
/*		  locking and unlocking the corresponding memory.     */
/*		  There is a pointer to the current element (pCur,    */
/*		  the one which is being tested), and another one     */
/*		  to the previous element (pPrev), and their corres-  */
/*		  pondant memory handles.			      */
/*		  Depending on the last parameter (bDelete, incoming) */
/*		  the current element (pCur) will be deleted or not.  */
/*                                                                    */
/*inoff ***************************************************************/

signed short int ro1110_movePtrsOnePos (
	unsigned long int        FAR *phPrev,
	unsigned long int        FAR *phCur,
	rT1100_ctrlTimer   FAR * FAR *ppPrev,
	rT1100_ctrlTimer   FAR * FAR *ppCur,
	signed	      int   bDelete)

{
   RM0001_STRG (fnctName, "ro1110_movePtrsOnePos")

   signed short int    retVal;	     /* return code	      */

   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   RM0002_ENTRY (fnctName)

   retVal = OK;

   if (bDelete == FALSE) {

      /*---------------------------------------*/
      /* It does not have to be deleted:       */
      /* move the pointers, unlock the already */
      /* checked element and lock the next one */
      /*---------------------------------------*/

      if (*phPrev != NULL) {
	 if (ro0032_localunlock (*phPrev) != 0) {
	    ro0032_localunlock (*phCur);
	    *phCur = NULL;
	    RM0003_RETURN (fnctName, signed short int, -R_LULOKE)
	 }
      }

      *ppPrev = *ppCur;
      *phPrev = *phCur;
      if (*phCur != NULL)
	 *phCur  = (*ppCur)->hNext;

      if (*phCur != NULL) {
	 if ((*ppCur = (rT1100_ctrlTimer *)ro0031_locallock (*phCur))
	     == NULL) {
	    if (*phPrev != NULL)
	        ro0032_localunlock (*phPrev);
	    retVal = -R_LLOCKE;
	 }
      } else {
	 *ppCur = (rT1100_ctrlTimer *)NULL;
      }

   } else {	/* bDelete == TRUE */

      if (*phPrev == NULL) {

	 /*----------------------------------------------------*/
	 /* Change the value of the global and static variable */
	 /* hFistElem when the first element must be deleted   */
	 /*----------------------------------------------------*/

	 hFirstElem = (*ppCur)->hNext;

      } else {

	 (*ppPrev)->hNext = (*ppCur)->hNext;

      }

      /*-----------------------------------*/
      /* Unlock and free the found element */
      /*-----------------------------------*/

      if (ro0032_localunlock (*phCur) != 0) {
	 if (*phPrev != NULL)
	    ro0032_localunlock (*phPrev);
	 RM0003_RETURN (fnctName, signed short int, -R_LULOKE)
      }

      if (ro0034_localfree (*phCur) != 0) {
	 if (*phPrev != NULL)
	    ro0032_localunlock (*phPrev);
	 RM0003_RETURN (fnctName, signed short int, -R_LFREEE)
      }

      if (*phPrev == NULL) {
	 *phCur = hFirstElem;
      } else {
	 *phCur = (*ppPrev)->hNext;
      }

      if (*phCur != NULL) {
	 if ((*ppCur = (rT1100_ctrlTimer *)ro0031_locallock (*phCur))
	     == NULL) {
	    if (*phPrev != NULL)
	       ro0032_localunlock (*phPrev);
	    retVal = -R_LLOCKE;
	 }
      } else {
	 *ppCur = (rT1100_ctrlTimer *)NULL;
      }
   }

   RM0003_RETURN (fnctName, signed short int, retVal)
}


/********************       END OF ROTIME0.C       ********************/
