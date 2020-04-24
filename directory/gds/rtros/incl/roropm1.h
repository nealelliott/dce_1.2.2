/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropm1.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:59  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:51  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:54  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:52  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:34  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:29  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:05  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:29  keutel
 * 	creation
 * 	[1994/03/21  13:27:41  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROROPM1_H
#define _ROROPM1_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roropm1_rcsid[] = { "@(#) $RCSfile: roropm1.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:59 $" } ;
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
/* NAME         : roropm1.h                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-05                                            */
/*                                                                    */
/* COMPONENT    : Remote Operations Service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Protocol machine apdu definitions                   */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropm1.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-02-05|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/* ****************** ROIV Apdu ************************************* */

typedef struct rS0710_stROIVapdu{
	rTro03_stPdvHeader  stPdvHeader;	/* PDV Header         */
	signed long int     lInvokeID;		/* INTEGER type ID    */
                                                /* same as service para*/
	signed long int     bLinkedID;		/* boolean since OPTIONAL */
	signed long int     lLinkedID;		/* same type as IncokeID  */
                                                /* see 9071-1             */

        rTro76_stDefinedBy  stOperationValue;   /* operation value as in  */
						/* OPERATION MACRO        */

	signed long int     bArgument;		/* Arguments OPTIONAL     */
	unsigned long int   hArgument;	        /*Handle to rTro04_stUserData*/
						/*passed in p1(User PDVdescr)*/
}rT0710_stROIVapdu;




/* ******************* RORES Apdu ************************************* */
typedef struct rS0711_stOperationResult{
        rTro76_stDefinedBy  stOperationValue;   /* operation value union */
	unsigned long int   hResult;            /* Handle to rTro04 User data*/
}rT0711_stOperationResult;

typedef struct rS0712_stRORSapdu{
	rTro03_stPdvHeader  stPdvHeader;	/* PDV Header         */
	signed long int     lInvokeID;		/* INTEGER type ID    */
	signed long int     bOperationResult;   /* OPTIONAL for the result */
                                                /*parameter struct rT0711  */
	rT0711_stOperationResult  stOperationResult;/* Operation Result       */
}rT0712_stRORSapdu;
	
/* ******************* ROERR Apdu ************************************ */

typedef struct rS0713_stROERapdu{
	rTro03_stPdvHeader  stPdvHeader;	/* PDV Header         */
	signed long int     lInvokeID;		/* INTEGER type ID    */
	rTro76_stDefinedBy  stErrorValue;	/* Erroe    value union */
	signed long int     bParameter;	        /* OPTIONAL erroe parameters*/
	unsigned long int   hParameter;	        /* Handle for rTro04 User   */
						/*for Error parameters      */
} rT0713_stROERapdu;

/* ******************* RO-REJECT Apdu ********************************* */

/* ***Values of Problem ID choice Num in stRORJapdu ******************* */

#define RC0712_GeneralProblem        0	  /*general for RO Provider Reject */
#define RC0713_InvokeProblem         1    /*Invoke releted                 */
#define RC0714_ReturnResultProblem   2    /* Result related problems       */
#define RC0715_ReturnErrorProblem    3    /* Error related problems        */


/* ******************** General Problems ****************************** */

#define RC0716_unrecognisedAPDU      0   /*unrecognised Apdu i.e. apdu other*/
					 /* than these Four */
#define RC0717_mistypedAPDU          1    /* mis typed error             */
#define RC0718_badlyStructuredAPDU   2    /* bad  structure ( violation)  */

/* ********************* Invoke Problem *******************************  */

#define RC0719_duplicateInvokation      0  /* duplicate invoke id           */
#define RC071A_unrecognisedOperation    1  /* unrecognised operation ie. not
					      accepted by the User           */
#define RC071B_mistypedArgument         2  /* argument is !matched           */
#define RC071C_resourceLimitation       3  /* No resource present           */
#define RC071D_initiatorReleasing       4  /* initator released before the   */
					   /*completion of  the operation     */
#define RC071E_unrecognisedLinkedID     5  /* unrecognised Linked ID          */
#define RC071F_linkedResponseUnexpected 6  /* unexpected link response        */
#define RC071G_unexpectedChildOperation 7  /* unexpected Child Operation      */

/* ************************ Return Result Problem ********************** */
#define RC071H_unrecognisedInvocation   0  /*unreconised invokation of Op   */
#define RC071I_resultResponseUnexpected 1  /*Result responce unexpected ie. */
					   /*not accepted by both           */
#define RC071J_mistypedResult           2  /*Result not matched             */

/* ************************** Return Error Problem ************************* */
#define RC071K_unrecognisedInvocation   0  /*unrecognised invokation         */
#define RC071L_ErrorResponseUnexpected  1  /*response to error unexpected    */
#define RC071M_UnrecognisedError        2  /* error not defined              */
#define RC071N_UnexpectedError          3  /* error not accecpted between the*/
					   /* Users                          */
#define RC071O_mistypedPrarameter       4  /*parameter of the error not      */ 
                                           /* matching*/
/* ********************** InvokeId Choice Num ************************* */
#define RC071P_InvIdPrest               2  /* invoke id present Int     */
#define RC071Q_NoInvId                  5  /* Noinvoke id NUll          */



/* ***************************RORJ Apdu ******************************** */

			
typedef struct rS0714_stRORJapdu{
	rTro03_stPdvHeader  stPdvHeader;	/* PDV Header         */
	signed long int     lInvokeIDChoiceNum;	/* to represent the CHOICE */
						/* of InvokeID and NULL (asn)*/
	signed long int     lInvokeID;		/* invoke-ID */
	signed long int     lProblemChoiceNum;	/* holdsCHOICE Id of problem*/
	signed long int     lProblem;            /*problem defined as above */
}rT0714_stRORJapdu;

/* ************************ ROSE Apdus ******************************** */

#define RC071P_ROIVapdu	0	/* RO-Invocation Apdu */
#define RC071Q_RORSapdu 1       /* RO-Result Apdu     */
#define RC071R_ROERapdu 2       /* RO-Erroe Apdu      */
#define RC071S_RORJapdu 3       /* RO-Reject Apdu     */

typedef union rU0715_unRoseApdus{
	rTro03_stPdvHeader  stPdvHeader;	/* PDV Header         */
	rT0710_stROIVapdu   stROIVapdu;
	rT0712_stRORSapdu   stRORSapdu;
	rT0713_stROERapdu   stROERapdu;
	rT0714_stRORJapdu   stRORJapdu;
}rT0715_unRoseApdus;

#endif	/* _ROROPM1_H */
