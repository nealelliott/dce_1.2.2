/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routilt.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:26  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:13  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:37:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:36  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:12  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:54  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:48  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:23  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:04  keutel
 * 	creation
 * 	[1994/03/21  13:28:43  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUTILT_H
#define _ROUTILT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _routilt_rcsid[] = { "@(#) $RCSfile: routilt.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:26 $" } ;
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
/* NAME         : routilt.h                                           */
/*                                                                    */
/* AUTHOR       : Shivkumar                                           */
/* DATE         : 93-04-28                                            */
/*                                                                    */
/* COMPONENT    : ROS and RTS utility functions                       */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Defines a new interface that hides the heavily      */
/*                pointer dependant TPDU interface defined in tpdu.h  */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/*    R_G000_Windows : Compiler constant for MS-Windows               */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)routilt.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 03.00  K0 |93-04-28|  original                      |skh |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


#define RC00t6_RttrSizeUnknown           (-1L)

/*----------------------------------------------------------------------------*/
/* TPDU open.                                                                 */
/*----------------------------------------------------------------------------*/

typedef struct rS00t1_stTpOpen {
    rT0100_unCallBack    unTpdu;               /*     stOpen internally used. */
    TP_bool              bIgnore;              /*  -> Actually perform or not:*/
                                               /*     TRUE  : Ignore  call.   */
                                               /*     FALSE : Perform call.   */
    TP_pbhead            stPbHead;             /* <-> standard header         */
    TP_short             sOpenId;              /* <-  file identifier         */
    TP_char         FAR *pDataDescription;     /*  -> file name               */
    TP_ushort            usLenDataDescription; /*  -> length of the filename  */
    Tpdir                iDir;                 /*  -> transfer type:          */
                                               /*     TPIN : for read         */
                                               /*     TPOUT: for write        */
    TP_long              lFileLength;          /* <-  filelength              */
} rT00t1_stTpOpen;

/*----------------------------------------------------------------------------*/
/* TPDU read.                                                                 */
/*----------------------------------------------------------------------------*/

typedef struct rS00t2_stTpRead {
    rT0100_unCallBack    unTpdu;               /*     stRead internally used. */
    TP_bool              bIgnore;              /*  -> Actually perform or not:*/
                                               /*     TRUE  : Ignore  call.   */
                                               /*     FALSE : Perform call.   */
    TP_pbhead            stPbHead;             /* <-> standard header         */
    TP_short             sOpenId;              /*  -> file identifier         */
    TP_char         FAR *pInBufferAddress;     /*  ->  buffer                 */
    TP_long              lInRequiredDataLength;/*  ->  number of bytes        */
                                               /*      to be read             */
    TP_long              lReadDataLength;      /* <-  number of bytes read    */
    TP_bool              bInFurtherData;       /* <-  Does further data       */
                                               /*     exist in this file:     */
                                               /*     TP_YES or TP_NO         */
} rT00t2_stTpRead;

/*----------------------------------------------------------------------------*/
/* TPDU write.                                                                */
/*----------------------------------------------------------------------------*/

typedef struct  rS00t3_stTpWrite {
    rT0100_unCallBack    unTpdu;               /*     stWrite internally used.*/
    TP_bool              bIgnore;              /*  -> Actually perform or not:*/
                                               /*     TRUE  : Ignore  call.   */
                                               /*     FALSE : Perform call.   */
    TP_pbhead            stPbHead;             /* <-> standard header         */
    TP_short             sOpenId;              /*  -> file identfier          */
    TP_char         FAR *pOutBufferAddress;    /*  -> buffer address          */
    TP_long              lOutRequiredDataLength;/* -> number of bytes         */
                                               /*     to be written           */
    TP_long              lWrittenDataLength;   /* <-  number of bytes written */
    TP_bool              bOutFurtherDataSpace; /* <-  free space in the file? */
                                               /*     (TP_YES or TP_NO)       */
    TP_bool              bSecure;              /*  -> Store immediately       */
                                               /*     (TP_YES or TP_NO)       */
} rT00t3_stTpWrite;

/*----------------------------------------------------------------------------*/
/* TPDU position.                                                             */
/*----------------------------------------------------------------------------*/

typedef struct rS00t4_stTpPosition {
    rT0100_unCallBack    unTpdu;              /*    stPosition internally used*/
    TP_bool              bIgnore;             /*  -> Actually perform or not: */
                                              /*     TRUE  : Ignore  call.    */
                                              /*     FALSE : Perform call.    */
    TP_pbhead            stPbHead;            /* <-> standard header          */
    TP_short             sOpenId;             /*  -> file identifier          */
    TP_long              lOffset;             /*  -> distance relative to the */
                                              /*     position after TP_OPEN   */
} rT00t4_stTpPosition;

/*----------------------------------------------------------------------------*/
/* TPDU close.                                                                */
/*----------------------------------------------------------------------------*/

typedef struct rS00t5_stTpClose {
    rT0100_unCallBack    unTpdu;              /*     stClose internally used. */
    TP_bool              bIgnore;             /*  -> Actually perform or not: */
                                              /*     TRUE  : Ignore  call.    */
                                              /*     FALSE : Perform call.    */
    TP_pbhead            stPbHead;            /* <-> standard header          */
    TP_short             sOpenId;             /*  ->  file identifier         */
} rT00t5_stTpClose;

/*----------------------------------------------------------------------------*/
/* TPDU delete.                                                               */
/*----------------------------------------------------------------------------*/

typedef struct rS00t6_stTpDelete {
    rT0100_unCallBack    unTpdu;              /*     stDelete internally used.*/
    TP_bool              bIgnore;             /*  -> Actually perform or not: */
                                              /*     TRUE  : Ignore  call.    */
                                              /*     FALSE : Perform call.    */
    TP_pbhead            stPbHead;            /* <-> standard header          */
    TP_short             sOpenId;             /*  -> file identifier          */
    TP_long              lRequiredDataLength; /*  -> number of bytes          */
                                              /*     which are not            */
                                              /*     deleted.                 */
    TP_long              lDataLength;         /* <-  actual file length       */
} rT00t6_stTpDelete;

/*----------------------------------------------------------------------------*/
/* TPDU function prototypes.                                                  */
/*----------------------------------------------------------------------------*/

signed short int PASCAL ro00t1_tpOpen      PROTO ((rT00t1_stTpOpen     FAR *));
signed short int PASCAL ro00t2_tpRead      PROTO ((rT00t2_stTpRead     FAR *));
signed short int PASCAL ro00t3_tpWrite     PROTO ((rT00t3_stTpWrite    FAR *));
signed short int PASCAL ro00t4_tpPosition  PROTO ((rT00t4_stTpPosition FAR *));
signed short int PASCAL ro00t5_tpClose     PROTO ((rT00t5_stTpClose    FAR *));
signed short int PASCAL ro00t6_tpDelete    PROTO ((rT00t6_stTpDelete   FAR *));
signed short int PASCAL ro00t7_tpduToStore PROTO ((rT00t2_stTpRead     FAR *,
                                                   unsigned long int    ));
signed short int PASCAL ro00t8_storeToTpdu PROTO ((rTro02_stStore      FAR *,
                                                   rT00t3_stTpWrite    FAR *));

#endif	/* _ROUTILT_H */
