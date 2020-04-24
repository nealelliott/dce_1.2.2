/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rodpat4.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:27  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:44  root]
 * 
 * Revision 1.1.2.5  1994/08/24  08:35:21  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:34:02  marrek]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:33  keutel
 * 	OT 11620
 * 	[1994/08/19  13:47:08  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:51  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:18  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:28  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:42  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:36  keutel
 * 	creation
 * 	[1994/03/21  13:18:59  keutel]
 * 
 * $EndLog$
 */
#ifndef _RODPAT4_H
#define _RODPAT4_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rodpat4_rcsid[] = { "@(#) $RCSfile: rodpat4.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:41 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONS AG	      */
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
/* NAME 	: rodpat4.h					      */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE 	: 91-10-31					      */
/*                                                                    */
/* COMPONENT	: RTS dispatcher				      */
/*								      */
/* PRD#/VERS.	: RTS-V02.00					      */
/*                                                                    */
/* DESCRIPTION	: Rts dispatcher internal definitions.		      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rodpat4.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 02.00 K0 | 91-10-30|  original		       |jm  |	      */
/* 03.00 K03| 94-06-16|  A0264044: Busy Flag: FILE.D   |skh	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************** Common constants ****************************** */
/* ******************		       ****************************** */

#define RC0250_StackSize			   8
#define RC0251_StoreSize			   4
#define RC0252_MaxRef1u2			  64
#define RC0253_MaxRef3				   4
#define RC0254_MaxLayerCxt			   4

/* ****************** Acb state flag values ************************* */
/* ******************			    ************************* */

#define RC0255_Idle			  0x00
#define RC0256_InActive 		  0x01
#define RC0257_ExActive 		  0x02
#define RC0258_SpecFlag 		  0x04
#define RC0259_EndPend			  0x08

/* ****************** Rdb record types ****************************** */
/* ******************		       ****************************** */

#define RC0265_EcbRecord		     1
#define RC0266_PmRecord 		     2

/* ****************** Protocol machine massage control block ******** */
/* ******************					     ******** */

typedef struct rS0250_mcb		   {
	       unsigned long  int toAcb    ;
	       signed	short int autoNo   ;
	       signed	long  int event    ;
	       unsigned long  int params[4];
} rT0250_mcb;

/* ****************** Association control block ********************* */
/* ******************				********************* */

typedef struct rS0251_acb				      {
	       unsigned long  int stateFlg		      ;
	       signed	short int evtCount		      ;
	       signed	short int stkCount		      ;
	       signed	short int cxtCount		      ;
	       unsigned long  int toEcb 		      ;
	       signed	long  int loifId		      ;
	       signed	short int configNo		      ;
	       unsigned long  int pmcxt   [RC0200_MaxAutomata];
	       signed	short int pmRdbNo [RC0200_MaxAutomata];
	       unsigned long  int lAcbRef1		      ;
	       char		  cAcbRef1[RC0252_MaxRef1u2  ];
	       unsigned long  int lAcbRef2		      ;
	       char		  cAcbRef2[RC0252_MaxRef1u2  ];
	       unsigned long  int lAcbRef3		      ;
	       char		  cAcbRef3[RC0253_MaxRef3    ];
} rT0251_acb;

/* ****************** Application entity invocation control ********* */
/* ****************** block				    ********* */

typedef struct rS0252_ecb				   {
	       unsigned long  int stateFlg		   ;
	       signed	short int configNo		   ;
	       signed	short int recInPm		   ;
	       signed	long  int recInEvt		   ;
	       unsigned long  int myAddres		   ;
	       unsigned long  int eiCxt[RC0254_MaxLayerCxt];
	       signed	short int ecbRdbNo		   ;
       /* for Tpm flow control by kc                           */
       /* FALSE = No flow control (default)                    */
       /* TRUE  = Flow control                                 */
               signed   short int bTpmFlowCtrl             ; 
} rT0252_ecb;

/* ****************** Provided buffer control block ***************** */
/* ******************				    ***************** */

typedef struct rS0253_pcb		  {
	       unsigned long  int stateFlg;
	       unsigned long  int toEcb   ;
} rT0253_pcb;

/* ****************** System control block ************************** */
/* ******************			   ************************** */

typedef struct rS0254_scb		  {
	       unsigned long  int stateFlg;

	       signed	long  int appliHd ;
	       signed	short int appliNo ;

	       signed	long  int assocHd ;
	       signed	short int assocNo ;

	       signed	short int prBufNo ;
	       unsigned long  int prBufHd ;

	       signed	short int uEvntQNo;
	       unsigned long  int uEvntQhd;
	       signed	short int uEvnHead;
	       signed	short int uEvnTail;

	       signed	long  int systemId;
	       FARPROC		  tpduFct ;
	       unsigned long  int hRdbFile;
	       signed	short int activScb;
                              /* ** skh: K03: A0264044: Busy Flag: FILE.D */
               unsigned long  int ulBusyFlag;
} rT0254_scb;

/* ****************** Control block union defintion ***************** */
/* ******************				    ***************** */

typedef union  rU0255_cb		 {
	       unsigned long int stateFlg;
	       rT0251_acb	  acbType;
	       rT0252_ecb	  ecbType;
	       rT0253_pcb	  pcbType;
	       rT0254_scb	  scbType;
} rT0255_cb;

#endif	/* _RODPAT4_H */
