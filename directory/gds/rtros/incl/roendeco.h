/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roendeco.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:38  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:34:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:13  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:55  keutel
 * 	OT 11620
 * 	[1994/08/19  13:48:56  keutel]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:35  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:11  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:10  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:05  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:16  keutel
 * 	creation
 * 	[1994/03/21  13:26:29  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROENDECO_H
#define _ROENDECO_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roendeco_rcsid[] = { "@(#) $RCSfile: roendeco.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:38 $" } ;
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
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : roendeco.h                                          */
/*                                                                    */
/* AUTHOR       : KChandrasekar                                       */
/* DATE         : 93-08-16                                            */
/*                                                                    */
/* COMPONENT    : RTROS protocol machines below the application layer */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.00                                         */  
/*                                                                    */
/* DESCRIPTION  : Macros for En-/Decoding of Byte Strings             */
/*                                                                    */
/* SYSTEM DEPENDENCE :  processor type                                */
/*                                                                    */
/*  Compiler constant:  R_G032_NoIntelLike_Processor                  */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#) roendeco.h 1.1 93/11/29 RTROS-V3.00              */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-04-02|  original                      |kc  |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


/**********************************************************************/
/*                                                                    */
/*              F U N C T I O N   P R O T O T Y P I N G               */
/*                                                                    */
/**********************************************************************/

/* function which changes the sequence of the bytes of an integer variable */
/* (see routil2.c)                                                         */


unsigned short int  BytesToUnsignedshort PROTO((
unsigned        char FAR * cpPointer ,                           
signed short  int        sLen ));

signed short int  UnsignedIntToBytes PROTO ((
unsigned        char FAR * cpPointer,                              
signed short  int        sLen,                                   
signed short  int        sVal));                                   

unsigned short int  Pts_BytesToUnsignedint PROTO ((
unsigned        char FAR * cpPointer,                            
signed short  int        sLen));

signed short int  Pts_UnsignedIntToBytes PROTO ((
unsigned        char FAR * cpPointer,                                      
signed short  int        sLen,                                           
unsigned short  int        sVal));                                           

/**********************************************************************/
/*                                                                    */
/*              M A C R O S                                           */
/*                                                                    */
/**********************************************************************/

/* ENCODE/DECODE:                                              */
/*   most significant byte is the first one in the byte stream */
/* RM0012_ENCODEi/RM0013_DECODEi:                              */
/*  least significant byte is the first one in the byte stream */
/*  (like Intel Convention)                                    */

#define ENCODE(v,p,s) {                                                        \
		     UnsignedIntToBytes((unsigned     char  FAR *)p,                                     \
                                        (signed short int)sizeof(s),           \
                                        (signed short int)(v));                \
		     (p) += sizeof(s);                                         \
		     }

#define DECODE(v,p,s) {                                                        \
                     (v)=(s)BytesToUnsignedshort(p,                            \
                                                 (signed short int)sizeof(s)); \
		     (p) += sizeof(s);                                         \
                      }

#define RM0012_ENCODEi(v,t,s) {                                                \
		     Pts_UnsignedIntToBytes((unsigned char FAR *)pBuffActual , \
					    (signed short int)sizeof(s),       \
                                            (unsigned short int)(v));          \
                     pBuffActual += sizeof(s);                                 \
			      }

#define RM0013_DECODEi(v,t,s) {                                                \
		     (v)=(t)Pts_BytesToUnsignedint((unsigned char FAR *)pBuffActual,                \
						 (signed short int)sizeof(s)); \
		     pBuffActual += sizeof(s);                                 \
			      }

#endif	/* _ROENDECO_H */
