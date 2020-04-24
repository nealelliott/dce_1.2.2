/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm10.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:46  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:58  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:59  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:29  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:36  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:13  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:47  keutel
 * 	creation
 * 	[1994/03/21  13:19:08  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPM10_H
#define _ROPPM10_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppm10_rcsid[] = { "@(#) $RCSfile: roppm10.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:49 $" } ;
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
/* NAME         : roppm10.h                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-01-29                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : PPM intern header                                   */
/*                Macros                                              */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roppm10.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-01-28|  adaptation of ROS V2.5A00 K05 |hm  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/***************************************************************************/

/* MEMORY */

/*Note: RM0003_RETURN useable because of common function return type "signed short int". */

/*Set handle to 0 after succesfull free: correct in all cases !? */
/*it needs very little code more than necessary (e.g. local variables) */

#define RM0420_GlobAlloc(h,s)                                              \
    {                                                                      \
      if ( ! ( (h) = ro0020_globalalloc((unsigned long int) (s)) ) )       \
      {                                                                    \
        RM0003_RETURN (fnctName, signed short int, - R_GALLCE)             \
      }                                                                    \
    }
                                         
#define RM0421_GlobLock(h,p,t)                                             \
    {                                                                      \
      if ( ! ( (p) = (t FAR *) ro0021_globallock((h)) ) )                  \
      {                                                                    \
        RM0003_RETURN (fnctName, signed short int, - R_GLOCKE)             \
      }                                                                    \
    }

#define RM0422_GlobUnLock(h)                                               \
    {                                                                      \
      if ( ro0022_globalunlock((h)) )                                      \
      {                                                                    \
        RM0003_RETURN (fnctName, signed short int, - R_GULOKE)             \
      }                                                                    \
    }

#define RM0423_GlobReAlloc(h,s)                                            \
    {                                                                      \
      if ( ! ( (h) = ro0023_globalrealloc((h),(unsigned long int) (s)) ) ) \
      {                                                                    \
        RM0003_RETURN (fnctName, signed short int, - R_GRALLE)             \
      }                                                                    \
    }

#define RM0424_GlobFree(h)                                                 \
    {                                                                      \
      if ( ro0024_globalfree((h)) )                                        \
      {                                                                    \
        RM0003_RETURN (fnctName, signed short int, - R_GFREEE)             \
      }                                                                    \
      else                                                                 \
        (h) = 0;                                                           \
    }



#define RM0430_LocAlloc(h,s)                                               \
    {                                                                      \
      if ( ! ( (h) = ro0030_localalloc((unsigned long int) (s)) ) )        \
      {                                                                    \
        RM0003_RETURN (fnctName, signed short int, - R_LALLCE)             \
      }                                                                    \
    }

#define RM0431_LocLock(h,p,t)                                              \
    {                                                                      \
      if ( ! ( (p) = (t NEAR *) ro0031_locallock((h)) ) )                  \
      {                                                                    \
        RM0003_RETURN (fnctName, signed short int, - R_LLOCKE)             \
      }                                                                    \
    }
                                                                           
#define RM0432_LocUnLock(h)                                                \
    {                                                                      \
      if ( ro0032_localunlock((h)) )                                       \
      {                                                                    \
        RM0003_RETURN (fnctName, signed short int, - R_LULOKE)             \
      }                                                                    \
    }                                                                     

#define RM0433_LocReAlloc(h,s) /*not in use */                             \
    {                                                                      \
      if ( ! ( (h) = ro0033_localrealloc((h),(unsigned long int) (s)) ) )  \
      {                                                                    \
        RM0003_RETURN (fnctName, signed short int, - R_LRALLE)             \
      }                                                                    \
    }                                                                     

#define RM0434_LocFree(h)                                                  \
    {                                                                      \
      if ( ro0034_localfree((h)) )                                         \
      {                                                                    \
        RM0003_RETURN (fnctName, signed short int, - R_LFREEE)             \
      }                                                                    \
      else                                                                 \
        (h) = 0;                                                           \
    }

/***************************************************************************/

/* Formal parameter list of PA functions */

/* !? #define RM0400_ParList (                    */
#define RM0400_ParList(n) signed short int n (                             \
pContext,                                                                  \
pEntContext,                                                               \
pConn,                                                                     \
pNode,                                                                     \
pX410RtpmNode,                                                             \
pbUserData,                                                                \
pUserData,                                                                 \
/*pbEventId,*/                                                             \
pOutEvent,                                                                 \
phNode,                                                                    \
pP2,                                                                       \
pP3,                                                                       \
pP4)                                                                       \
rT0430_stContext        *pContext;     /* <-> PM context (pointer to) */                      \
rT0441_stEntContext     *pEntContext;  /* --> Pres. entity inv. context (pointer to) */       \
rT0431_unConn      FAR  *pConn;        /* <-> Parameter P3 structure (pointer to) */          \
rT041G_unPpdu      FAR  *pNode;        /* <-> Pres. PDV node (pointer to) */                  \
unsigned long  int FAR  *pX410RtpmNode;/* <-- PS-user data handle (X410-mode) (pointer to) */ \
signed   long  int FAR  *pbUserData;   /* <-- PS-user data OPTIONAL flag (normal-mode) */     \
                                       /*     (pointer to)                             */     \
rT0417_stUserData  FAR  *pUserData;    /* <-- PS-user data struct (normal-mode)        */     \
                                       /*     (pointer to)                             */     \
/*signed       int FAR  *pbEventId;*/  /* <-- TRUE: provider abort with event identifier */   \
                                       /*     (pointer to)                               */   \
unsigned       int FAR  *pOutEvent;    /* <-- Outgoing event (pointer to) */                  \
unsigned long  int FAR  *phNode;       /* <-> PDV root node, only used if */                  \
                                       /*     provider-generated CPR (pointer to) */          \
unsigned long  int FAR  *pP2;          /* <-> event parameter P2 (pointer to) */              \
unsigned long  int FAR  *pP3;          /* <-> event parameter P2 (pointer to) */              \
unsigned long  int FAR  *pP4;          /* <-> event parameter P4 (pointer to) */ 
#endif	/* _ROPPM10_H */
