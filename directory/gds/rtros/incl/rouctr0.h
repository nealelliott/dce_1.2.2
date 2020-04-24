/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rouctr0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:17  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:03  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:36:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:19  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:05  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:46  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:40  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:47  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:48  keutel
 * 	creation
 * 	[1994/03/21  13:28:13  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUCTR0_H
#define _ROUCTR0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rouctr0_rcsid[] = { "@(#) $RCSfile: rouctr0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:17 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1989 BY SIEMENS AG                              */
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
/* NAME 	: rouctr0.h					      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-02-13					      */
/*                                                                    */
/* COMPONENT	: ROS remote operation upper control protocol         */
/*		  machine					      */
/*								      */
/* PRD#/VERS.	: RORTS-V3.00					      */
/*                                                                    */
/* DESCRIPTION	: Protocol machine eventing interface		      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rouctr0.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 3.00 K0  | 90-12-31|  original		       |jm  |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/


/* ****************** Types of IROS user data buffer **************** */
/* ******************                                **************** */

/* The following constants are contained in the union  of 
   bufref  to identify the type of buffer.
*/

#define RC1500_Memory       0
#define RC1501_File         1
#define RC1502_Handle       2

#define RC1504_SnData       3
#define RC1504_RcvData      4

#define RC1501_RO_BUFFERind -1


/* ****************** Reference to IROS user data buffer ************ */
/* ******************                                    ************ */

typedef union  rU1500_unBufferRef {
        unsigned long  int      ulhandle;
        char           FAR     *pBufRef;   
        char           FAR     *pTpduName;   
} rT1500_unBufferRef;

/*
  - ulhandle    : memory handle (global memory)
  - pBufRef     : FAR pointer to the memory area ,length defined in
		  structure
  - pTpduName   : virtual TPDU file name,length defined in the
		  structure
*/

typedef struct rS1501_stBuffer {               
         signed        int     iMemType;
         rT1500_unBufferRef    unBufferRef;
         unsigned long int     ulLength; 
}rT1501_stBuffer;                    

/*
  - iMemType    : identifies whether handle/pointer/tpdu file
  - unBufferRef : see union above
  - ulLength    : length of buffer 
*/

typedef struct rS1502_stSnData {
        rTro03_stPdvHeader     stPdvHeader; 	
	signed        int       iMemType;
        rT1500_unBufferRef      unBufferRef;
	unsigned long int  	ulBufLen;
	unsigned long int  	vOffset[RCro0b_MaxExternal];
	unsigned long int  	vLength[RCro0b_MaxExternal];
	unsigned long int  	vAbsId[RCro0b_MaxExternal];
        signed long int         lCount;
        signed short int        sOpenId;
}rT1502_stSnData;

/*
  - stPdvHeader : see rouply0.h , new Pdv header which would take care
	          of sequence of externals
  - iMemType    : identifies whether handle/pointer/tpdu file
  - unBufRef    : see union above
  - ulBufLen    : Total length of buffer
  - ulOffset    : Identifies starting point of a fragment
  - ulLength    : length of the fragment

*/


typedef struct rS1503_stFragArray {
         signed        int    iAbstrSynId;    
	 unsigned long int    ulLength;
}rT1503_stFragArray;                  

/*
  - iAbstrSynId : Abstract Syntax Id of an External
  - ulLength    : length of fragment
*/


typedef struct rS1504_stEvData {                                               
	 signed          int     iMemType; 
         rT1500_unBufferRef      unBufferRef;
	 unsigned long  int      ulBufLen;
         signed         int      bSpecFlg;
         unsigned long           ulFragCount;
         rT1503_stFragArray      vFragArray[RCro0b_MaxExternal];
}rT1504_stEvData;                                                           

/* 
  - iMemType    : identifies whether handle/pointer/tpdu file
  - bSpecFlag   : set when no buffers found
  - unBufferRef : see union above
  - ulBufLen    : Total length of buffer
  - ulFragCount : no. of fragments (Optional ,when value is '0' it
                  means no Fragments in the Array
  - vFragArray  : see above
*/

typedef union  rU1505_unData {
        rTro03_stPdvHeader     stPdvHeader; 	
        rT1502_stSnData        stSnData;
        rTro04_stUserData      stUserData; 	
} rT1505_unData;

/*
  - stPdvHeader : See rouply0.h                   
  - stSnData    : See above                                        
  - stUserData  : See rouply0.h                               
*/


/* ****************** Event-Parameter-matrix ************************ *
 * ******************			     ************************ *

|-------------------|------|--------------|---------------|----------|-------|
|event		    |IN/OUT|p1		  |p2		  |p3	     | p4    |
|-------------------|------|--------------|---------------|----------|-------|
|RC0701_RO_BINDreq  |IN+OUT|1502_SnData|ND|1501_Buffer|nB |rT0701    |bMappIf|
|RC0702_RO_BINDind  |IN+OUT|1504_EvData|ND|NULL/Pid|rT0702    |NULL   |
|RC0703_RO_BINDrsp  |IN+OUT|1502_SnData|ND|NULL 	  |rT0703    |BindRes|
|RC0704_RO_BINDcnf  |IN+OUT|1504_EvData|ND|NULL           |rT0704    |BindRes|
|-------------------|------|--------------|---------------|----------|-------|
|RC0705_RO_UNBINDreq|IN+OUT|1502_SnData|ND|UnbindReason   |1501Buf|nB|NULL   |
|RC0706_RO_UNBINDind|IN+OUT|1504_EvData|ND|UnbindReason   |NULL      |NULL   |
|RC0707_RO_UNBINDrsp|IN+OUT|1502_SnData|NNDUnbindReason   |NULL      |UnBnRes|
|RC0708_RO_UNBINDcnf|IN+OUT|1504_EvData|ND|UnbindReason   |NULL      |UnBnRes|
|-------------------|------|--------------|---------------|----------|-------|
|RC0709_RO_UABORTreq|IN+OUT|1502_SnData|ND|NULL 	  |NULL      |AbortRe|
|RC070a_RO_UABORTind|IN+OUT|1504_EvData|ND|adAboInfo	  |NULL      |AbortRe|
|-------------------|------|--------------|---------------|----------|-------|
|RC070b_RO_P_ABORTin|IN+OUT|1504_EvData|ND|adAboInfo	  |NULL      |AbortRe|
|-------------------|------|--------------|---------------|----------|-------|
|RC070c_RO_INVreq   |IN+OUT|1502_SnData|NND1501_Buffer|nB |rT070b    |InvkId |
|RC070d_RO_INVind   |IN+OUT|1504_EvData|ND|NULL           |rT070c    |NULL   |
|-------------------|------|--------------|---------------|----------|-------|
|RC070e_RO_RESreq   |IN+OUT|1502_SnDataIND|NULL           |rT070d    |NULL   |
|RC070f_RO_RESind   |IN+OUT|1504_EvData|ND|NULL           |rT070e    |InvkId |
|-------------------|------|--------------|---------------|----------|-------|
|RC070g_RO_ERRreq   |IN+OUT|1502_SnDataIND|NULL           |rT070f    |NULL   |
|RC070h_RO_ERRind   |IN+OUT|1504_EvData|ND|NULL           |rT070g    |InvkId |
|-------------------|------|--------------|---------------|----------|-------|
|RC070i_RO_RJUreq   |IN+OUT|NULL          |NULL 	  |rT070h    |NULL   |
|RC070j_RO_RJUind   |IN+OUT|NULL          |NULL           |rT070i    |InvkId |
|-------------------|------|--------------|---------------|----------|-------|
|RC070k_RO_RJpind   |IN+OUT|1504_EvData|ND|NULL           |NULL      |InvkId |
|-------------------|------|--------------|---------------|----------|-------|
|RC1501_RO_BUFFERind|OUT   |1504_EvData   |NULL           |NULL      |NULL   |
|-------------------|------|--------------|---------------|----------|-------|
|all other values   |IN+OUT|NOTDEFINED	  |NOTDEFINED	  |NOTDEFINED|NOTDEF.|
|-------------------|------|------------------------------|----------|-------|

IN	   : It is an incomming event (received by the Upper Control).
OUT	   : It is an outgoing event  (sent by the Upper Control).
NOTDEFINED : This parameter contains a not defined value.
NULL	   : Parameter not used.

1502_SnData: Global heap memory handle of pdv-tree
	     root node 

1504_EvData: Global heap memory handle of local_event_data
	     which is being passed to External interface of ROS.

1501_Buffer: Global heap memory handle of an
	     event buffer			   

bMappingInfo :Information about the protocal stack.i.e. whether RTSE or NOT. 
              This BOOLEAN will have TRUE - RTSE present                     
                                     FALSE- RTSE not present                 

UnbindReason :Close Reason supplied by the Ros User in the case where the    
              events are from RO-User (UNBINDreq & UNBINDres)                
              If the events are from lower PM (ACSE) then Acse reason will be
              present.                                                       

AbortReason  :This reason will be given by the provider of the service.      
                                                                             
AddAbortInfo :Addditional information  for abort if exists.                  

BindReason : Association reason
 
rT070x	   : Local heap memory handle to direct mapped service parameter 

 * ****************** End of event parameter matrix ***************** *
 * ******************				    ***************** */

#endif	/* _ROUCTR0_H */
