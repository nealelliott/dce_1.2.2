/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropm0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:58  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:51  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:51  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:33  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:28  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:01  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:27  keutel
 * 	creation
 * 	[1994/03/21  13:27:39  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROROPM0_H
#define _ROROPM0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roropm0_rcsid[] = { "@(#) $RCSfile: roropm0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:58 $" } ;
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
/* NAME         : roropm0.h                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-04                                            */
/*                                                                    */
/* COMPONENT    : Remote Operations Service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Protocol machine service interface                  */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropm0.h	1.2 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-02-04|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/
/* ****************** ROSE defines ********************************** */        
/* ******************              ********************************** */        
                                                                                
/* ******** Definition of the ros protocol machine "basic"number ***** */
                                                                                
#define RC0700_RoseEvent        0x00000800L
                                                                                
               /* Definition of the ros protocol machine service      */        
               /* events:                                             */        
                                                                                
               /* The rose bind service                               */        
                                                                                
#define RC0701_RO_BINDreq      (RC0700_RoseEvent |0x00)
#define RC0702_RO_BINDind      (RC0700_RoseEvent |0x01)
#define RC0703_RO_BINDrsp      (RC0700_RoseEvent |0x02)
#define RC0704_RO_BINDcnf      (RC0700_RoseEvent |0x03)

               /* The rose unbind service                             */        
                                                                                
#define RC0705_RO_UNBINDreq    (RC0700_RoseEvent |0x04)
#define RC0706_RO_UNBINDind    (RC0700_RoseEvent |0x05)
#define RC0707_RO_UNBINDrsp    (RC0700_RoseEvent |0x06)
#define RC0708_RO_UNBINDcnf    (RC0700_RoseEvent |0x07)
                                                                                
               /* The rose abort service                              */        
                                                                                
#define RC0709_RO_UABORTreq     (RC0700_RoseEvent |0x08)
#define RC070a_RO_UABORTind     (RC0700_RoseEvent |0x09)
                                                      
		/* The rose provider Abort                          */
#define RC070b_RO_PABORTind	(RC0700_RoseEvent |0x0a)


               /* The rose invoke service                             */        
                                                                                
#define RC070c_RO_INVreq       (RC0700_RoseEvent |0x0b)
#define RC070d_RO_INVind       (RC0700_RoseEvent |0x0c)
                                                                                
               /* The rose result service                             */        
                                                                               
#define RC070e_RO_RESreq	(RC0700_RoseEvent |0x0d)
#define RC070f_RO_RESind       	(RC0700_RoseEvent |0x0e)
                                                                                
               /* The rose error service                              */        
                                                                                
#define RC070g_RO_ERRreq       (RC0700_RoseEvent |0x0f)
#define RC070h_RO_ERRind       (RC0700_RoseEvent |0x10)
                                                                                
               /* The rose user reject service                        */        
                                                                                
#define RC070i_RO_RJUreq       (RC0700_RoseEvent |0x11)
#define RC070j_RO_RJUind       (RC0700_RoseEvent |0x12)
                                                                                
               /* The rose provider reject service                    */        
                                                                                
#define RC070k_RO_RJPind       (RC0700_RoseEvent |0x13)


                                                                                
                                                                               
                                                                                 
/* ****************************************************************** */
/* ************ Data structures ************************************* */

/* **********************ASSOCIATION CLASS definitions *************** */
/* Used in the ulAssocClass */

#define RC070l_AssocClass1	1
#define RC070m_AssocClass2	2
#define RC070n_AssocClass3	3

/* ******************************************************************** */

/* ************ Abort Indication Reasons **************************** */
#define RC070A_UnBindNotIniti   0x010801
#define RC070B_RejectCountAbo   0x010802
#define RC070C_ReleaseNotAcpt   0x010803



/* *************** BINDreq ****************************************** */
/* User data has to be stored in the Store and the rTro04_stUserData 
   and passed in P1                                                  */

/* The RTSE parameters are used only the RTSE is included in the stack */

typedef struct rS0700_stBind{
	unsigned long int  ulAssocClass;     /*Association class 9072-1 */
	unsigned long int  ulCheckPointSize; /*Rtse parameter(see rortpm0.h*/
        unsigned long int  ulWindowSize;     /*Rtse parameter(see rortpm0.h */
}rT0700_stBind;


typedef struct rS0701_stBindReqRopm{
        rTro4a_stReqAll     stAll;
        rTro6a_stReqPpm     stPpm;
        rTro7a_stAcpm       stAcpm;
        rT0700_stBind       stRopm;
}rT0701_stBindReqRopm;
/* This structure is passed to the ROPM during a RO-BIND request event:
                                                                                
   - stAll           : Substructure: common               (see rouply0.h).
   - stPpm           : Substructure: presentation         (see rouply0.h).      
   - stAcpm          : Substructure: ACSE                 (see rouply1.h).      
   - stRopm          : Substructure: ROSE                 (see above ).

*/                                                                              
                                                                                
/* ******************** Bindind **************************************** */

typedef struct rS0702_stBindIndRopm{
	rTro4b_stIndAll     stAll;
        rTro6b_stIndPpm     stPpm;
        rTro7b_stIndAcpm    stAcpm;
}rT0702_stBindIndRopm;
/* This structure is passed to the ROPM during a RO-BIND indication event:
                                                                                
   - stAll           : Substructure: common               (see rouply0.h).
   - stPpm           : Substructure: presentation         (see rouply0.h).      
   - stAcpm          : Substructure: ACSE                 (see rouply0.h).      
	No Ropm structure 
*/                                                                              
                                                                                
 
/* *********************** BINDres ************************************* */


typedef struct rS0703_stBindRspRopm{
	rTro4c_stRspCnfAll  stAll;
        rTro6c_stRspPpm     stPpm;
        rTro7a_stAcpm       stAcpm;
        rT0700_stBind       stRopm;
}rT0703_stBindRspRopm;

/* This structure is passed to the ROPM during a RO-BIND request event:
                                                                                
   - stAll           : Substructure: common               (see rouply0.h).
   - stPpm           : Substructure: presentation         (see rouply0.h).      
   - stAcpm          : Substructure: ACSE                 (see rouply0.h).      
   - stRopm          : Substructure: ROSE                 (see above ).

*/                                                                              
                                                                                
/* ************************** BINDcnf ********************************* */

typedef struct rS0704_stBindCnfRopm{
	rTro4c_stRspCnfAll  stAll;
        rTro6d_stCnfPpm     stPpm;
        rTro7a_stAcpm       stAcpm;
}rT0704_stBindCnfRopm;

/* This structure is passed to the ROPM during a RO-BIND confirm event:
                                                                                
   - stAll           : Substructure: common               (see rouply0.h).
   - stPpm           : Substructure: presentation         (see rouply0.h).      
   - stAcpm          : Substructure: ACSE                 (see rouply0.h).      

*/                                                                              
                                                                                
/* ********************** OPERATIONS  ************************** */
/* ********************** INVOCATION *************************** */

/* ********************** OPERATION CLASS ***************************** */
/* **** As define by the ISO 9072-1 *********************************** */

#define RC070o_OpClass1		1  /* Synchronised operation           */
#define RC070p_OpClass2		2  /* Async. operation                 */
#define RC070q_OpClass3		3
#define RC070r_OpClass4		4
#define RC070s_OpClass5		5



/* ******************************************************************** */



	
typedef struct rS070b_stInvReq{
	rTro76_stDefinedBy       stOperationValue;
	unsigned long int        ulOperationClass; /*MANDATORY in ISO 9072-1
                                                     one of the 5           */

	signed   long int        lInvokeID;
	signed   long int        bLinkedID;
	signed   long int        lLinkedID;
	signed   long int        lPriority;   
                                            /* If  NULL - either priority not
                                              present (in which case default 
                                              value is 0) or priority present
                                              and has value 0.
                                              (see 9066-1 9.4.1.1)       */
        signed  short int        sAbsId; 
}rT070b_stInvReq;

typedef  struct rS070c_stInvInd{
	rTro76_stDefinedBy       stOperationValue;
	signed   long int        lInvokeID;
	signed   long int        bLinkedID;
	signed   long int        lLinkedID;
}rT070c_stInvInd;

/* ******************** RESULT ******************************** */
/* ***** User data will have the Result defined by the RO-Notatin
	OPERATION Macro. ******************************************  */

typedef struct rS070d_stResReq{
	signed   long int        bOperationValue; /* Operation Value is present
                                                    only if the result para.
                                                    is present i.e.UserDate */

	rTro76_stDefinedBy       stOperationValue;
	signed   long int        lInvokeID;	/* Operation Invoke Id */
	signed   long int        lPriority;
        signed  short int        sAbsId; 

}rT070d_stResReq;

/* ----------- Result in P1 ----------- */

typedef struct rS070e_stResInd{
	signed   long int        bOperationValue; /* Operation Value is present
                                                    only if the result para.
                                                    is present i.e.UserDate */
	rTro76_stDefinedBy       stOperationValue;
	signed   long int        lInvokeID;

}rT070e_stResInd;
/* --------- Result in P1 ----------- */

/* ********************* ERROR ******************************** */
/* ***** User data will have the lists of errors defined by the RO-Notatin
	ERROR Macro. ******************************************  */
typedef struct rS070f_stErrReq{
	rTro76_stDefinedBy        stErrorValue;
	signed   long int         lInvokeID;
	signed   long int         lPriority;
        signed  short int        sAbsId; 
}rT070f_stErrReq;
 
/* --------- Error parameter in P1 --------- */

typedef struct rS070g_stErrInd{
	rTro76_stDefinedBy        stErrorValue;
	signed long int           lInvokeID;
}rT070g_stErrInd;
/* --------- Error parameter in P1 --------- */

/* ************************ U REJECT ****************************** */
                                                                               
typedef struct rS070h_stURejReq{
	unsigned   long int ulRejReason;
	signed     long int lInvokeID;
	signed     long int lPriority;
        signed  short int        sAbsId; 
}rT070h_stURejReq;

typedef struct rS070i_stURejInd{
	unsigned   long int ulRejReason;
	signed     long int lInvokeID;
}rT070i_stURejInd;

/* *********************** P-REJECT ******************************* */
/* *** In  the Provider Reject the Reject Parameters are sent in the Ind. 
	in the User data part                   ******************** */

typedef struct rS070j_stPRejInd{
	signed   long int bRejectReason;
	signed   long int lRejectReason;
        signed   long int bInvokeID;
	signed   long int lInvokeID;

}rT070j_stPRejInd;
	
/* *********** Mask values used for event to get the incomming ************ */
/* ********** PM  ********************************************************* */

#define RC070t_SubAutoMask  0x00ff00

/* ********************** Event-Parameter-Matrix *************************** */
/* **********************                        *************************** */
/*                                                                                
 ---------------------------------------------------------------------------- 
|event          |IN/OUT|p1            |p2          |p3         |p4           |
|----------------------------------------------------------------------------|
|RO_BINDreq     |IN    |pdv|noPdv     |NULL        |rT0701     |bMappingInfo |
|RO_BINDind     |rUser |pdv|noPdv     |NULL/Pid    |rT0702     |NULL         |
|RO_BINDrsp     |IN    |pdv|noPdv     |NULL        |rT0703     |BindResult   |
|RO_BINDcnf     |rUser |pdv|noPdv     |NULL        |rT0704     |BindResult   |
|----------------------------------------------------------------------------|
|RO_UNBINDreq   |IN    |pdv|noPdv     |UnbindReason|NULL       |NULL         |
|RO_UNBINDind   |rUser |pdv|noPdv     |UnbindReason|NULL       |NULL         |
|RO_UNBINDrsp   |IN    |pdv|noPdv     |UnbindReason|NULL       |UnBindResult |
|RO_UNBINDcnf   |rUser |pdv|noPdv     |UnbindReason|NULL       |UnbindResult |
|----------------------------------------------------------------------------|
|RO_ABORTreq    |IN    |pdv|noPdv     |NULL        |NULL       |AbortReason  |
|RO_ABORTind    |rUser |pdv|noPdv     |AddAbortInfo|NULL       |AbortReason  |
|----------------------------------------------------------------------------|
|RO_P_ABORTind  |rUser |pdv|noPdv     |AddAbortInfo|NULL       |AbortReason  |
|----------------------------------------------------------------------------|
|RO_INVreq      |IN    |pdv|noPdv    1|NULL        |rT070b     |NULL         |
|RO_INVind      |rUser |pdv|noPdv     |NULL        |rT070c     |NULL         |
|----------------------------------------------------------------------------|
|RO_RESreq      |IN    |pdv|noPdv    2|NULL        |rT070d     |NULL         |
|RO_RESind      |rUser |pdv|noPdv     |NULL        |rT070e     |NULL         |
|----------------------------------------------------------------------------|
|RO_ERRreq      |IN    |pdv|noPdv    3|NULL        |rT070f     |NULL         |
|RO_ERRind      |rUser |pdv|noPdv     |NULL        |rT070g     |NULL         |
|----------------------------------------------------------------------------|
|RO_RJUreq      |IN    |noPdv         |NULL        |rT070h     |NULL         |
|RO_RJUind      |rUser |noPdv         |NULL        |rT070i     |NULL         |
|----------------------------------------------------------------------------|
|RO_RJPind      |rUser |pdv|noPdv    4|NULL        |rT070j     |NULL         |
|               |      |              |            |           |             |
|----------------------------------------------------------------------------|
|All other      |IN   7|NOTDEFINED    |NOTDEFINED  |NOTDEFINED |NOTDEFINED   |
|values         |OUT   |NOTDONE       |NOTDONE     |NOTDONE    |NOTDONE      |
 ----------------------------------------------------------------------------

IN           : It is an incoming event (received by the RTPM).
OUT          : It is an outgoing event (sent by the RTPM).
NOTDEFINED   : This parameter contains a not defined value.
NOTDONE      : Such an event is not issued by the RTPM.
NULL         : Parameter not used.
pdv          : Global heap memory handle of PDV-tree root node.
                                                                 (see rouply0.h)
noPdv        : No PDV description of PM user data for this event (see rouply0.h)

bMappingInfo :Information about the protocal stack.i.e. whether RTSE or NOT.
              This BOOLEAN will have TRUE - RTSE present
                                     FALSE- RTSE not present
 
UnbindReason :Close Reason supplied by the Ros User in the case where the
              events are from RO-User (UNBINDreq & UNBINDres)
              If the events are from lower PM (ACSE) then Acse reason will be
              present.
AbortReason  :This reason will be given by the provider of the service.

AddAbortInfo :Addditional information  for abort if exists.

 
 p1:  Traditional & Suggested use: PM user data.
 p2:  Traditionally: Used for conveying the receive buffer description.
      Suggested: Use as non-memory-handle parameter.
 p3:  Traditionally: Used for conveying local memory handle.
      Suggested: Use as local memory handle for constructed types where possible
 p4:  Traditionally: Used for passing values.
      Suggested: Retain traditional use.
* ********************** End of Event-Parameter-Matrix  ********************* *
* **********************                                ********************* */

#endif	/* _ROROPM0_H */
