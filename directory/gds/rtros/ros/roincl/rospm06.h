/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm06.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:33  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:41:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:26  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:49  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:35  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:42  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:53  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:16  keutel
 * 	creation
 * 	[1994/03/21  13:33:20  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROSPM06_H
#define _ROSPM06_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rospm06_rcsid[] = { "@(#) $RCSfile: rospm06.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:33 $" } ;
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
/* NAME         : rospm06.h                                           */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-06                                            */
/*                                                                    */
/* COMPONENT    : RTROS convergence protocol machine                  */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : This modul provides the function prototypes of the  */
/*                convergence protocol machine                        */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      :  sccsid  =  @(#) rospm06.h 1.2 93/12/20 RTROS-V3.00             */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-04-01|  original                      |wdm |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*********************************************************************/
/*                                                                   */
/*              F U N C T I O N  P R O T O T Y P E S                 */
/*                                                                   */
/*********************************************************************/

/* ----------------------------------------------- */
/*              File - rospm00.c                   */
/* ----------------------------------------------- */

void ro080a_InitContext PROTO ((
	rT0870_stSpmContext *     pContext
	));

signed short int ro080b_TransportData PROTO ((
	unsigned long int        /* ulparam1  */
	));

signed long int FAR PASCAL ro080a_StoreOffset PROTO ((
        signed long int,       /*  levent         */
        unsigned long int,     /*  hStore         */
        unsigned long int      /*  ulOffset       */
        ));

/* ----------------------------------------------- */
/*              File :- rospm02.c                  */
/* ----------------------------------------------- */

signed short int PASCAL ro082A_WhichSpdu PROTO ((
	char                    /* spdu       */
	));

void PASCAL ro082B_InitStruc PROTO (( 
	void FAR * ,            /* vPtr       */
	unsigned long int       /* ulLength   */
	));                 

/* ----------------------------------------------- */
/*              File - rospm03.c                   */
/* ----------------------------------------------- */

signed short int NEAR PASCAL ro0830_AbortLogicalFalse PROTO ((
	rT0870_stSpmContext * , /*  pContext      */
	unsigned long int ,     /*  hP1           */
	unsigned long int ,     /*  hP2           */
	unsigned long int ,     /*  hP3           */
	unsigned long int       /*  hP4           */
	));

signed short int NEAR PASCAL ro0831_InvalidSpdu PROTO ((
	rT0870_stSpmContext * ,   /* pContext      */
	unsigned long int ,       /* hUserData     */
	unsigned long int ,       /* ulOffset      */
	unsigned long int ,       /* hP3           */
	unsigned long int         /* hP4           */
	));

signed short int NEAR PASCAL ro0832_LocalMatter PROTO ((
	rT0870_stSpmContext * ,   /* pContext      */
	unsigned long int ,       /* hP1           */
	unsigned long int ,       /* hP2           */
	unsigned long int ,       /* hP3           */
	unsigned long int         /* hP4           */
	));

signed short int NEAR PASCAL ro0833_NothingHappens PROTO ((
	rT0870_stSpmContext * ,   /* pContext       */
	unsigned long int ,       /* hP1            */
	unsigned long int ,       /* hP2            */
	unsigned long int ,       /* hP3            */
	unsigned long int         /* hP4            */
	));

signed short int NEAR PASCAL ro0834_NoProtocolEvent PROTO ((
	rT0870_stSpmContext * ,   /* pContext       */
	unsigned long int ,       /* hP1            */
	unsigned long int ,       /* hP2            */
	unsigned long int ,       /* hP3            */
	unsigned long int         /* hP4            */
	));

signed short int NEAR PASCAL ro0835_Abort1 PROTO ((
	rT0870_stSpmContext * ,   /* pContext      */
	unsigned long int ,       /* hP1           */
	unsigned long int ,       /* hP2           */
	unsigned long int ,       /* hP3           */
	unsigned long int         /* hP4           */
	));

signed short int NEAR PASCAL ro0836_Abort2 PROTO ((
	rT0870_stSpmContext * ,   /* pContext        */
	unsigned long int ,       /* hP1             */
	unsigned long int ,       /* hP2             */
	unsigned long int ,       /* hP3             */
	unsigned long int         /* hP4             */
	));

signed short int NEAR PASCAL ro0837_Abort4 PROTO ((
	rT0870_stSpmContext * ,   /* pContext        */
	unsigned long int ,       /* hP1             */  
	unsigned long int ,       /* hP2             */
	unsigned long int ,       /* hP3             */
	unsigned long int         /* hP4             */
	));

signed short int NEAR PASCAL ro0838_Abort5 PROTO ((       
	rT0870_stSpmContext * ,  /* pContext        */
	unsigned long int ,      /* hP1             */  
	unsigned long int ,      /* hP2             */
	unsigned long int ,      /* hP3             */
	unsigned long int        /* hP4             */
	));

signed short int NEAR PASCAL ro0839_TDISCONind PROTO ((
	rT0870_stSpmContext * , /*  pContext       */
	unsigned long int ,     /*  hP1            */
	unsigned long int ,     /*  ulAddAboInfo   */ 
	unsigned long int ,     /*  hP3            */
	unsigned long int       /*  ulAboReason    */
	));

signed short int NEAR PASCAL ro083a_SUABORTreq PROTO ((
	rT0870_stSpmContext * ,   /* pContext       */
	unsigned long int ,       /* hUserData      */
	unsigned long int ,       /* ulOffset       */
	unsigned long int ,       /* hP3            */
	unsigned long int         /* hP4            */
        ));

signed short int NEAR PASCAL ro083b_Ab PROTO ((
	rT0870_stSpmContext * ,   /* pContext       */
	unsigned long int ,       /* hP1            */
	unsigned long int ,       /* hP2            */
	unsigned long int ,       /* hP3            */
	unsigned long int         /* hP4            */
	));


/* ----------------------------------------------- */
/*              File :- rospm04.c                  */
/* ----------------------------------------------- */

signed short int NEAR PASCAL ro0840_TCONTINUE PROTO ((
	rT0870_stSpmContext * ,   /*  pContext      */
	unsigned long int ,       /*  ulP1          */  
	unsigned long int ,       /*  ulP2          */
	unsigned long int ,       /*  ulP3          */
	unsigned long int         /*  ulP4          */
	));

signed short int NEAR PASCAL ro0841_STA01xSCONreq PROTO ((
	rT0870_stSpmContext * ,  /*  pContext      */
	unsigned long int ,      /*  hUserData     */
	unsigned long int ,      /*  ulOffset      */
	unsigned long int ,      /*  hConnReqSpm   */
	unsigned long int        /*  ulP4          */
	));

signed short int NEAR PASCAL ro0842_STA01xTCONind PROTO ((
	rT0870_stSpmContext * ,  /* pContext      */ 
	unsigned long int ,      /* hP1           */
	unsigned long int ,      /* hP2           */
	unsigned long int ,      /* hCallingAddr  */
	unsigned long int        /* hP4           */
	));

signed short int NEAR PASCAL ro0843_STA01AxAA PROTO ((
	rT0870_stSpmContext * ,  /* pContext       */
	unsigned long int ,      /* hP1            */
	unsigned long int ,      /* hP2            */
	unsigned long int ,      /* hP3            */
	unsigned long int        /* hP4            */
	));

signed short int NEAR PASCAL ro0844_STA01BxTCONcnf PROTO ((
	rT0870_stSpmContext * ,  /* pContext       */
	unsigned long int ,      /* hUserData      */
	unsigned long int ,      /* ulOffset       */
	unsigned long int ,      /* hRespondAddr   */
	unsigned long int        /* hP4            */
	));

signed short int NEAR PASCAL ro0845_STA01CxCN PROTO ((
	rT0870_stSpmContext * ,  /* pContext      */
	unsigned long int ,      /* hUserData     */
	unsigned long int ,      /* ulOffset      */
	unsigned long int ,      /* hP3           */
	unsigned long int        /* hP4           */
	));

signed short int NEAR PASCAL ro0846_STA01CxSCONreq PROTO ((
	rT0870_stSpmContext * ,  /*  pContext     */
	unsigned long int ,      /*  hUserData    */
	unsigned long int ,      /*  ulOffset     */ 
	unsigned long int ,      /*  hRespondAddr */
	unsigned long int        /*  hP4          */
	));

/******
 Not implemented in RTROS V-3.00 K00

signed short int NEAR PASCAL STA01DxCDO (void);       
*********/

/* ----------------------------------------------- */
/*              File :- rospm05.c                  */
/* ----------------------------------------------- */

signed short int NEAR PASCAL ro0850_STA02xRF PROTO ((
	rT0870_stSpmContext * ,  /* pContext      */ 
	unsigned long int ,      /* hUserData     */
	unsigned long int ,      /* ulOffset      */
	unsigned long int ,      /* hP3           */ 
	unsigned long int        /* hP4           */
	));

signed short int NEAR PASCAL ro0851_STA02AxAC PROTO ((
	rT0870_stSpmContext * ,  /* pContext      */
	unsigned long int ,      /* hUserData     */
	unsigned long int ,      /* ulOffset      */ 
	unsigned long int ,      /* hP3           */
	unsigned long int        /* hP4           */
	));

/******
 Not implemented in RTROS V-3.00 K00

signed short int NEAR PASCAL sta02b_s_con_req (void);    
signed short int NEAR PASCAL sta02b_oa (void);            
********/

/* ----------------------------------------------- */
/*              File :- rospm06.c                  */
/* ----------------------------------------------- */

signed short int NEAR PASCAL ro0860_STA03xFN PROTO ((
	rT0870_stSpmContext * ,  /* pContext      */
        unsigned long int ,      /* hUserData     */
	unsigned long int ,      /* ulOffset      */
	unsigned long int ,      /* ulP3          */
	unsigned long int        /* ulP4          */
	));

signed short int NEAR PASCAL ro0861_STA03xDT PROTO (( 
	rT0870_stSpmContext * ,  /* pContext      */
        unsigned long int ,      /* hUserData     */
	unsigned long int ,      /* ulOffset      */
	unsigned long int ,      /* hP3           */
	unsigned long int        /* hP4           */
	));

signed short int NEAR PASCAL ro0862_STA03xDN PROTO (( 
	rT0870_stSpmContext * ,  /* pContext      */
	unsigned long int ,      /* hUserData     */
	unsigned long int ,      /* ulOffset      */ 
	unsigned long int ,      /* ulP3          */
	unsigned long int        /* ulP4          */
	));

signed short int NEAR PASCAL ro0863_STA08xSCONrspPos PROTO ((
	rT0870_stSpmContext * ,    /*  pContext      */
	unsigned long int ,        /*  hUserData     */
	unsigned long int ,        /*  ulOffset      */
	unsigned long int ,        /*  hConnRspSpm   */
	unsigned long int          /*  ulConResult   */
	));

signed short int NEAR PASCAL ro0864_STA08xSCONrspNeg PROTO ((
	rT0870_stSpmContext * ,    /*  pContext      */
	unsigned long int ,        /*  hUserData     */
	unsigned long int ,        /*  ulOffset      */
	unsigned long int ,        /*  hConnRspAcpmorPID */
	unsigned long int          /*  ulConResult   */
	));

/* ----------------------------------------------- */
/*              File :- rospm07.c                  */
/* ----------------------------------------------- */

signed short int NEAR PASCAL ro0870_STA09xSRELreq PROTO ((
	rT0870_stSpmContext * ,    /*  pContext     */
	unsigned long int ,        /*  hUserData    */
	unsigned long int ,        /*  ulOffset     */
	unsigned long int ,        /*  hP3          */
	unsigned long int          /*  hP4          */
	));

signed short int NEAR PASCAL ro0871_STA09xSRELrspPos PROTO ((  
	rT0870_stSpmContext * ,    /*  pContext     */
	unsigned long int ,        /*  hUserData    */
	unsigned long int ,        /*  ulOffset     */
	unsigned long int ,        /*  hP3          */
	unsigned long int          /*  hP4          */
	));

signed short int NEAR PASCAL ro0872_STA09xDN PROTO ((
	rT0870_stSpmContext * ,    /*  pContext     */
	unsigned long int ,        /*  hUserData    */
	unsigned long int ,        /*  ulOffset     */
	unsigned long int ,        /*  hP3          */
	unsigned long int          /*  hP4          */
	));

signed short int NEAR PASCAL ro0873_STA09xSDATAreq PROTO (( 
	rT0870_stSpmContext * ,    /*  pContext     */
	unsigned long int ,        /*  hUserData    */
	unsigned long int ,        /*  ulOffset     */
	unsigned long int ,        /*  hP3          */
	unsigned long int          /*  hP4          */
	));

signed short int NEAR PASCAL ro0874_STA713xSRELreq PROTO ((
	rT0870_stSpmContext * ,    /*  pContext     */
	unsigned long int ,        /*  hUserData    */
	unsigned long int ,        /*  ulOffset     */
	unsigned long int ,        /*  hP3          */
	unsigned long int          /*  hP4          */
	));

signed short int NEAR PASCAL ro0875_STA713xFN PROTO ((
	rT0870_stSpmContext * ,    /*  pContext     */
	unsigned long int ,        /*  hUserData    */
	unsigned long int ,        /*  ulOffset     */
	unsigned long int ,        /*  hP3          */
	unsigned long int          /*  hP4          */
	));

signed short int NEAR PASCAL ro0876_STA713xSDATAreq  PROTO (( 
	rT0870_stSpmContext * ,   /*  pContext      */ 
	unsigned long int ,       /*  hUserData     */
	unsigned long int ,       /*  ulOffset      */
	unsigned long int ,       /*  hP3           */
	unsigned long int         /*  hP4           */
	));

signed short int NEAR PASCAL ro0877_STA713xDT PROTO ((
	rT0870_stSpmContext * ,   /*  pContext      */ 
	unsigned long int ,       /*  hUserData     */
	unsigned long int ,       /*  ulOffset      */
	unsigned long int ,       /*  hP3           */
	unsigned long int         /*  hP4           */
	));

signed short int NEAR PASCAL ro0878_PendAbort PROTO ((
        rT0870_stSpmContext *pContext,
        unsigned long int hP1,
        unsigned long int hP2,
        unsigned long int hP3,
        unsigned long int hP4));

/* ------------------------------------------------------------------ */

#endif	/* _ROSPM06_H */
