/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm01.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:28  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:41:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:20  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:38  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:31  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:38  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:42  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:12  keutel
 * 	creation
 * 	[1994/03/21  13:33:11  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROSPM01_H
#define _ROSPM01_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rospm01_rcsid[] = { "@(#) $RCSfile: rospm01.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:28 $" } ;
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
/* NEW NAME     : rospm01.h                                           */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-01                                            */
/*                                                                    */
/* COMPONENT    : RTROS convergence protocol machine                  */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Defines for encoding/decoding functions             */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#) rospm01.h 1.1 93/11/29 RTROS-V3.00              */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-04-01|  original                      |ma  |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*********************************************************************/
/*                                                                   */
/*              D E F I N E S                                        */
/*                                                                   */
/*********************************************************************/

#define RC0830_AddressLen                 16
#define RC0836_MaxReflParamLen             9
#define RC0831_MaxParamLen                67
#define RC0835_MaxConnectLen           10240

#define RC0850_CalledReject                0
#define RC0851_SPMReject                   1

#define RC0871_NotReuse                    1
#define RC0870_Reuse                       0

#define RC0855_NoReason                    1
#define RC0856_UserAbort                   1
#define RC0857_NoUserAbort                 0
#define RC0858_ProtAbort                   1
#define RC0859_NoProtAbort                 0

#define RC087d_NoOverflow                  2
#define RC087c_Overflow                    1

#define RC0875_NoExtendSPDU                0          /* true   */
#define RC0874_ExtendSPDU                  1          /* false  */

/* --------------------------------------------------
   Enclosure Item ( 1 octate )
   bit 1 = 1 : biginning  of SSDU
   bit 2 = 0 : not end of SSDU 
  
   Bits 3-8 are reserved.
 ----------------------------------------------------- */

#define RC0876_OneSPDU      3           /* 0x0000 0011 */
#define RC0877_StartSPDU    1           /* 0x0000 0001 */
#define RC0878_MoreSPDU     0           /* 0x0000 0000 */
#define RC0879_EndSPDU      2           /* 0x0000 0010 */


/* --------------------------------------------------- */
/*  Constants used In Encoading / Decoding Routines    */
/* --------------------------------------------------- */

#define   RC0890_SIDataTransferSPDU           1
#define   RC0891_SINotFinishedSPDU            8
#define   RC0892_SIFinishSPDU                 9
#define   RC0893_SIDisconnectSPDU            10

#define   RC0894_SIRefuseSPDU                12
#define   RC0895_SIConnectSPDU               13
#define   RC0896_SIAcceptSPDU                14
#define   Rc0897_SIOverflowAcceptSPDU        16

#define   RC0898_SIAbortSPDU                 25
#define   RC0899_SIAbortAcceptSPDU           26

/* --------------------------------------------------------- */

#define	  RC08a0_PGIConnectionIdentifier      1
#define   RC08a1_PGIConnectSAcceptItem        5

#define   RC08a5_PGIUserData                193
#define   RC08a6_PGIExtendedUserData        194

/* --------------------------------------------------------- */

#define   RC08b0_PICalledSSUserRef            9

#define	  RC08b6_PICallingSSUserRef	     10
#define   RC08b7_PICommonRef                 11
#define	  RC08b8_PIAddRefInfo	             12
#define   RC08b9_PITokenItem                 16
#define   RC08ba_PITransportDisconnect       17
#define   RC08bb_PIProtocolOptions	     19

#define   RC08bh_PISessionUserRequirements   20
#define   RC08bi_PITSDUMaxsize	       	     21
#define   RC08bj_PISessionVersionNo          22
#define   RC08bk_PIInitialSerialNo           23
#define   RC08bl_PIEnclosureItem             25
#define   RC08bm_PITokenSettingItem          26

#define   RC08bt_PIReflectParamVal           49

#define   RC08bA_PIReasonCode                50
#define   RC08bB_PICallingSessionSelector    51
#define   RC08bC_PICalledSessionSelector     52
#define   RC08bD_PIRespondingSessionSelector 52
#define   RC08bE_PISecondInitialSerialNo     55

#define   RC08bI_PIDataOverflow              60

/* --------------------------------------------------------- */
/*          Constants related to Refuse SPDU                 */
/* --------------------------------------------------------- */

#define   RC08c0_CalledSSUserRej              2

#define   RC08c5_SessionSelectorUnknown        0x0081
#define   Rc08c6_RefuseNoReason                0x0085

/* --------------------------------------------------------- */
#define    RC08ca_FUValue                        0x349

/* ************************
 moved to rotmper.h 

#define R_SUNKEV  29052
#define R_SLSQFL  29152
#define R_SINFER  29252 
***************************/


/* ---------------------------------------------- */
/*            Struct for Header                   */
/* ---------------------------------------------- */
typedef struct rS081a_stHeader {
    char  FAR                     *pDataptr;
    long  int                      lDatalen;

    unsigned short int             usFunctionalUnits; 
    unsigned long int              ulEnclosure;
	} rT081a_stHeader;

/* ------------------------------------------------- */ 
/*         Struct for Abort Accept SPDU              */
/* ------------------------------------------------- */
typedef struct rS0810_stAA {
    char  FAR                     *pDataptr;
    long  int                      lDatalen;

    unsigned short int             usFunctionalUnits; 
	} rT0810_stAA;

/* ------------------------------------------------- */ 
/*           Struct for Abort SPDU                   */
/* ------------------------------------------------- */
typedef struct rS0811_stAB {
    char  FAR                      *pDataptr;
    long  int                       lDatalen;

    unsigned short int             usFunctionalUnits; 
    unsigned long int              ulEnclosure;

    unsigned short int             usTdiscAreuse;
    unsigned short int             usTdiscUserAbort;
    unsigned short int             usTdiscProtAbort;
    unsigned short int             usTdiscNoReason;

    signed short int               sReflParamLen;
    unsigned char                  ucReflParam[RC0836_MaxReflParamLen];        
	} rT0811_stAB;


/* ------------------------------------------------- */
/*           Struct for Accept SPDU                  */
/* ------------------------------------------------- */
typedef struct rS0812_stAC {
    char  FAR                     *pDataptr;
    long  int   	          lDatalen;

    unsigned short int            usFunctionalUnits; 
    unsigned long int             ulEnclosure;

    unsigned short int            usVersion;
    unsigned short int            usPopt;
    unsigned short int            usOverflow;

    unsigned long int             ulTSDUmaxto;
    unsigned long int             ulTSDUmaxfrom;

    signed short int              sRespondingLen;
    unsigned char                 ucResponding[RC0830_AddressLen+1];

    signed short int              sCallingLen;
    unsigned char                 vCalling[RC0830_AddressLen+1];
	} rT0812_stAC;

/* ---------------------------------------------- */
/*   Struct for Connect Data Overflow SPDU        */
/* ---------------------------------------------- */
typedef struct rS0813_stCDO {
    char  FAR                      *pDataptr;
    long  int                       lDatalen;

    unsigned short int             usFunctionalUnits; 
	} rT0813_stCDO;

/* ---------------------------------------------- */
/*           Struct for Connect SPDU              */
/* ---------------------------------------------- */
typedef struct rS0814_stCN {
    char  FAR                     *pDataptr;
    long  int                      lDatalen;

    unsigned short int             usFunctionalUnits; 
    unsigned long int              ulEnclosure;

    unsigned short int             usVersion;
    unsigned short int             usPopt;
    unsigned short int             usOverflow;

    unsigned long int              ulTSDUmaxto;
    unsigned long int              ulTSDUmaxfrom;

    signed short int               sCalledLen;
    unsigned char                  vCalled[RC0830_AddressLen+1];
    signed short int               sCallingLen;
    unsigned char                  vCalling[RC0830_AddressLen+1];
	} rT0814_stCN;


/* ------------------------------------------------- */ 
/*          Struct for Disconnect SPDU               */
/* ------------------------------------------------- */
typedef struct rS0815_stDN {
    char  FAR                      *pDataptr;
    long  int                       lDatalen;

    unsigned short int              usFunctionalUnits; 
    unsigned long int              ulEnclosure;
	} rT0815_stDN;

/* ------------------------------------------------- */ 
/*           Struct for Data Transfer SPDU           */
/* ------------------------------------------------- */
typedef struct rS0816_stDT {
    char  FAR                      *pDataptr;
    long  int                       lDatalen;

    unsigned short int              usFunctionalUnits; 
    unsigned long int              ulEnclosure;
	} rT0816_stDT;

/* ----------------------------------------------- */
/*           Struct for Finish SPDU                */
/* ----------------------------------------------- */
typedef struct rS0817_stFN {
    char  FAR                      *pDataptr;
    long  int  		           lDatalen;

    unsigned short int             usFunctionalUnits; 
    unsigned long int              ulEnclosure;

    unsigned short int             usTdiscAreuse;
    unsigned short int             usTdiscUserAbort;
    unsigned short int             usTdiscProtAbort;
    unsigned short int             usTdiscNoReason;
	} rT0817_stFN;

/* ---------------------------------------------- */
/*     Struct for Overflow Accept SPDU            */
/* ---------------------------------------------- */
typedef struct rS0818_stOA {
    char  FAR                      *pDataptr;
    long  int   	          lDatalen;

    unsigned short int             usFunctionalUnits; 

    unsigned short int             usVersion;

    unsigned long int              ulTSDUmaxto;
    unsigned long int              ulTSDUmaxfrom;
	} rT0818_stOA;

/* ---------------------------------------------- */ 
/*           Struct for Refuse SPDU               */
/* ---------------------------------------------- */
typedef struct rS0819_stRF {
    char  FAR                      *pDataptr;
    long  int  		           lDatalen;

    unsigned short int             usFunctionalUnits; 
    unsigned long int              ulEnclosure;

    unsigned short int             usVersion;

    unsigned short int             usTdiscAreuse;
    unsigned short int             usTdiscUserAbort;

    unsigned short int             usReason;
	} rT0819_stRF;

/* ------------------------------------------------- */ 
/*           Struct for Other SPDUs                  */
/* ------------------------------------------------- */
typedef struct rS081b_stOther {
    char  FAR                      *pDataptr;
    long  int   	           lDatalen;

    unsigned short int             usFunctionalUnits; 
    unsigned long int              ulEnclosure;
	} rT081b_stOther;


/* ============================================== */

typedef union rU081A_unPpdu {
	rT081a_stHeader      stHeader;
	rT0810_stAA          stAA;         /* Abort Accept SPDU       */
	rT0811_stAB    	     stAB;         /* Abort  SPDU             */
	rT0812_stAC          stAC;         /* Accept SPDU             */
	rT0813_stCDO         stCDO;        /* Conn Data Overflow SPDU */
	rT0814_stCN          stCN;         /* Connect SPDU            */
	rT0815_stDN          stDN;         /* Disconnect SPDU         */         
	rT0816_stDT          stDT;         /* Data Transfer SPDU      */
	rT0817_stFN          stFN;         /* Finish SPDU             */
	rT0818_stOA          stOA;         /* Overflow Accept SPDU    */  
	rT0819_stRF          stRF;         /* refuse SPDU             */
	rT081b_stOther       stOther;      /* Other SPDUs             */
   }rT081A_unPpdu;

#endif	/* _ROSPM01_H */
