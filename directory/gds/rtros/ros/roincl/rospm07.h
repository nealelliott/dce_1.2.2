/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm07.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:34  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:11  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:41:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:29  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:51  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:36  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:43  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:56  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:17  keutel
 * 	creation
 * 	[1994/03/21  13:33:22  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROSPM07_H
#define _ROSPM07_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rospm07_rcsid[] = { "@(#) $RCSfile: rospm07.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:34 $" } ;
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
/* NAME         : rospm07.h                                           */
/*							              */	
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-07                                            */
/*                                                                    */
/* COMPONENT    : RTROS session protocol machine                      */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.00                                         */
/*                                                                    */
/* DESCRIPTION	: Includes local to protocol machine.      	      */
/*                Defines context block used in Session PM.           */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#) rospm07.h 1.2 93/12/20 RTROS-V3.00              */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-04-07|  original                      |ma  |	      */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

typedef struct rS08a0_stRedirect {
/*
    signed short  int              sCallingAddrsLen;  
    unsigned char                  vCallingAddrs[RC0830_AddressLen+1] ; 
       
    unsigned short int             usFunctionalUnits; 
    unsigned short int             usPopt;        * Protocol Options  * 
    unsigned short int             usProtvers;    * Version Number    * 
*/
    /* New  parameters as per new design on 7/7/93 */
    unsigned long int              hsStore;
    unsigned long int              ulOffset;
	}rT08a0_stRedirect;

typedef struct rS0870_stSpmContext {

    unsigned short int             usState;
    unsigned long  int             ulConectresult;

    unsigned long  int             ulTidusize;

    signed short  int              sCalledAddrsLen;  
    unsigned char                  vCalledAddrs[RC0830_AddressLen+1] ; 

    unsigned long int              hAddrs; 
    unsigned long int              hCalledAddress;  /* by kc on 08-12-93 */

    unsigned short int             usFunctionalUnits; 
    unsigned short int             usPopt;        /* Protocol Options  */ 
    unsigned short int             usProtvers;    /* Version Number    */ 

    unsigned short int             usUabort;      /*                   */
    unsigned short int             usAreuse;

    /* 
      usVtca = false  : the SPM received the T-CONNECT request
                      ( transport connection initiator )
      usVtca = true   : the SPM received the T-CONNECT indication
                      ( transport connection acceptor )
    */
    unsigned short int             usVtca;

    /*
      usVcoll = true  : a collision of FINISH SPDUs has been dected
      usVcoll = false : there has not been a collision of FINISH SPDUs. 
    */
    unsigned short int             usVcoll;

    /* 
       If activity management functional unit is selected  
       usVact = true  : an activity is in progress 
       usVact = false : an activity is not in progress 
    */
    unsigned short int             usVact;       

    /*
      usVtrr = true   : the transport connection can be reused by
                      the SPM for another session connection 
      usVtrr = false  : the transport connection cannot be reused by
                      the SPM for another session connection 
    */
    unsigned short int             usVtrr;

    /* 
       TEXP = true   : use of transport expedited service is 
                       selected for use on this session connection. 
       TEXP = false  : use of transport expedited service is 
                       selected for use on this session connection. 
    */
    unsigned short int             usTexp;
    
    /*
       usVdnr = true  : a DISCONNECT SPDU has been received in RC08da_STA09
                      ( following a collision of FINISH SPDUs ) 
       usVdnr = false : no DISCONNECT SPDU has been received. 
    */
    unsigned short int             usVdnr;

    unsigned long int              hUserData;
    unsigned long int              ulOffset;
 
    /* unsigned short int          usredirection;  */
    rT08a0_stRedirect              stRedir;

    unsigned short int             usUserConnExists;

    signed   short int             bAbortPend;  /* S-U-ABORT-req between  */
                                                /* TCONreq and T-CONcnf ! */

} rT0870_stSpmContext;  /*   CONTEXT_STRUCT ; */

typedef union rU0871_unConn {
     rTro5e_stConnReqSpm             stReq;
     rTro5f_stConnIndSpm             stInd;
     rTro5g_stConnRspCnfSpm          stRsp;
     rTro5g_stConnRspCnfSpm          stCnf;
}rT0871_unConn;  

#endif	/* _ROSPM07_H */
