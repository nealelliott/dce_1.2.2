/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routilt.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:24  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:33:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:47  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:54  keutel
 * 	OT 11620
 * 	[1994/08/19  13:48:53  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:27  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:50:15  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:57  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:56:20  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:54  keutel
 * 	creation
 * 	[1994/03/21  13:25:53  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: routilt.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:24 $" } ;
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
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : routilt.c                                           */
/*                                                                    */
/* AUTHOR       : Shivkumar                                           */
/* DATE         : 93-04-28                                            */
/*                                                                    */
/* COMPONENT    : Virtual Tpdu Access Interface                       */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V03.00                                        */
/*                                                                    */
/* DESCRIPTION  : Implements the new interface that hides the heavily */
/*                pointer dependant TPDU interface defined in tpdu.h  */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)routilt.c 1.1 94/01/26               */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 03.00  K0 |93-04-28|  original                      |skh |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) routilt.c 1.1 93/11/29   RTS-V2.00";

/******************************************************************************/
/*                                                                            */
/*                              I N C L U D E S                               */
/*                                                                            */
/******************************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <roerval.h>
#include <routil0.h>
#include <rouply0.h>
#include <rouply1.h>
#include <rodpat0.h>
#include <tpdu.h>
#include <roexif2.h>
#include <rostor0.h>
#include <routilt.h>

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro00t1_tpOpen                                               */
/*                                                                            */
/* AUTHOR       : Shivkumar, Haran                                            */
/* DATE         : 92-02-11                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro00t1_tpOpen (
	rT00t1_stTpOpen    FAR *pTpOpen)      /* <-> : see routilt.h */

/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Calls the tpdu function for OPEN.                                      */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     pDataDescription     : File name to be opened/created.                 */
/*     usLenDataDescription : Length of file description in vDataDescription  */
/*     iDir                 : TPIN  => open file for reading                  */
/*                            TPOUT => open/create file for writing           */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/*     sOpenId              : Identifier of file opened or created.           */
/*                                                                            */
/*     lFileLength          : Length of file opened.                          */
/*                                                                            */
/*                            If file opened for read, it is the length of    */
/*                            the file. It will be -1, if file size is not    */
/*                            known at the time of OPEN. (Such a case occurs  */
/*                            when the file refers to I/O devices etc. Note   */
/*                            further, that in such cases TP_POSITION may     */
/*                            NOT be possible !!).                            */
/*                                                                            */
/*                            If file opened for write and the amount of space*/
/*                            available is not known (such as a Teleprinter)  */
/*                            then the file is assumed to be of an indefinite */
/*                            length), or otherwise if the space available is */
/*                            already known, the amount of space available is */
/*                            returned  (for e.g., if a write container such  */
/*                            as a disk is present, the disk capacity could be*/
/*                            the length returned, if all of the disk can be  */
/*                            made available.                                 */
/*                                                                            */
/*                            If file is opened for write and does exist, the */
/*                            length returned is the length returned on the   */
/*                            first open minus the length actually used.      */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*      OK                                                                    */
/*                                                                            */
/* GLOBAL DATA (read only) : none                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  : none                                             */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro00t1_tpOpen*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro00t1_tpOpen")
signed   short int   sRetCode = OK;
FARPROC              pTpduCallBack;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY  (pFnctName)

/*----------------------------------------------------------------------------*/
/* Phase 1: Assignment of pointers                                            */
/*----------------------------------------------------------------------------*/

pTpOpen->unTpdu.stOpen.tp_pbhead              = (TP_pbhead FAR *)
                                                 &pTpOpen->stPbHead;
pTpOpen->unTpdu.stOpen.tpopenid               = (TP_short  FAR *)
                                                 &pTpOpen->sOpenId;
pTpOpen->unTpdu.stOpen.tpdata_description     = (TP_char   FAR *)
                                                 pTpOpen->pDataDescription;
pTpOpen->unTpdu.stOpen.tplen_data_description = (TP_ushort FAR *)
                                                 &pTpOpen->usLenDataDescription;
pTpOpen->unTpdu.stOpen.tpdir                  = (Tpdir     FAR *)
                                                 &pTpOpen->iDir;
pTpOpen->unTpdu.stOpen.tpfilelength           = (TP_long   FAR *)
                                                 &pTpOpen->lFileLength;

/*----------------------------------------------------------------------------*/
/* Phase 2: Assignment of values if any and execution of call                 */
/*----------------------------------------------------------------------------*/

pTpOpen->stPbHead.tpversion = TPVERSION;

pTpduCallBack = ro0228_getTpduFct ();
if (pTpduCallBack == LNULL)
    RM0003_RETURN (pFnctName, signed short int, R_FTPERR)

if (!pTpOpen->bIgnore) {
    if (pTpduCallBack (RC0101_Open, (rT0100_unCallBack FAR *)&pTpOpen->unTpdu)
                                                                          != OK)
        sRetCode = R_FTPERR;
}

RM0003_RETURN (pFnctName, signed short int, sRetCode)

}/*end-ro00t1_tpOpen*/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro00t2_tpRead                                               */
/*                                                                            */
/* AUTHOR       : Shivkumar, Haran                                            */
/* DATE         : 92-02-11                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro00t2_tpRead (
	rT00t2_stTpRead    FAR *pTpRead)      /* <-> : see routilt.h */

/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Calls the tpdu function for READ.                                      */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     sOpenId               : Identifier of file to be read.                 */
/*     pInBufferAddress      : Buffer where data to be read into.             */
/*     lInRequiredDataLength : Length of data to be read in.                  */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/*     lReadDataLength      : Length of data actually read.                   */
/*     bInFurtherData       : Whether further data exists for reading ?       */
/*                            TP_YES : Data exists                            */
/*                            TP_NO  : Data does not exist                    */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*      OK                                                                    */
/*                                                                            */
/* GLOBAL DATA (read only) : none                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  : none                                             */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro00t2_tpRead*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro00t2_tpRead")
signed   short int   sRetCode = OK;
FARPROC              pTpduCallBack;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY  (pFnctName)

/*----------------------------------------------------------------------------*/
/* Phase 1: Assignment of pointers                                            */
/*----------------------------------------------------------------------------*/

pTpRead->unTpdu.stRead.tp_pbhead                = (TP_pbhead FAR *)
                                                &pTpRead->stPbHead;
pTpRead->unTpdu.stRead.tpopenid                 = (TP_short  FAR *)
                                                &pTpRead->sOpenId;
pTpRead->unTpdu.stRead.tpinbufferaddress        = (TP_char   FAR *)
                                                pTpRead->pInBufferAddress;
pTpRead->unTpdu.stRead.tpinrequired_data_length = (TP_long   FAR *)
                                                &pTpRead->lInRequiredDataLength;
pTpRead->unTpdu.stRead.tpread_data_length       = (TP_long   FAR *)
                                                &pTpRead->lReadDataLength;
pTpRead->unTpdu.stRead.tpinfurther_data         = (TP_bool   FAR *)
                                                &pTpRead->bInFurtherData;

/*----------------------------------------------------------------------------*/
/* Phase 2: Assignment of values if any and execution of call                 */
/*----------------------------------------------------------------------------*/

pTpRead->stPbHead.tpversion = TPVERSION;

pTpduCallBack = ro0228_getTpduFct ();
if (pTpduCallBack == LNULL)
    RM0003_RETURN (pFnctName, signed short int, R_FTPERR)

if (!pTpRead->bIgnore) {
    if (pTpduCallBack (RC0102_Read, (rT0100_unCallBack FAR *)&pTpRead->unTpdu)
                                                                          != OK)
        sRetCode = R_FTPERR;
}

RM0003_RETURN (pFnctName, signed short int, sRetCode)

}/*end-ro00t2_tpRead*/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro00t3_tpWrite                                              */
/*                                                                            */
/* AUTHOR       : Shivkumar, Haran                                            */
/* DATE         : 92-02-11                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro00t3_tpWrite (
	rT00t3_stTpWrite   FAR *pTpWrite)      /* <-> : see routilt.h */
/*                                                                            */
/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Calls the tpdu function for WRITE.                                     */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     sOpenId                : Identifier of file to be written to.          */
/*     pOutBufferAddress      : Buffer from which data to be written.         */
/*     lOutRequiredDataLength : Length of data to be written.                 */
/*     bSecure                : Store immediately ?                           */
/*                              TP_YES : Store immediately (always used by    */
/*                                                          RTS).             */
/*                              TP_NO  : (=> to store or not depends on       */
/*                                           TPDU implementation).            */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/*     lWrittenDataLength    : Length of data actually written.               */
/*     bOutFurtherDataSpace  : Whether further space exists for writing ?     */
/*                             TP_YES : Space exists for writing              */
/*                             TP_NO  : Space does not exist for writing      */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*      OK                                                                    */
/*                                                                            */
/* GLOBAL DATA (read only) : none                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  : none                                             */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro00t3_tpWrite*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro00t3_tpWrite")
signed   short int   sRetCode = OK;
FARPROC              pTpduCallBack;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY  (pFnctName)

/*----------------------------------------------------------------------------*/
/* Phase 1: Assignment of pointers                                            */
/*----------------------------------------------------------------------------*/

pTpWrite->unTpdu.stWrite.tp_pbhead                 = (TP_pbhead FAR *)
                                              &pTpWrite->stPbHead;
pTpWrite->unTpdu.stWrite.tpopenid                  = (TP_short  FAR *)
                                              &pTpWrite->sOpenId;
pTpWrite->unTpdu.stWrite.tpoutbufferaddress        = (TP_char   FAR *)
                                               pTpWrite->pOutBufferAddress;
pTpWrite->unTpdu.stWrite.tpoutrequired_data_length = (TP_long  FAR *)
                                              &pTpWrite->lOutRequiredDataLength;
pTpWrite->unTpdu.stWrite.tpwritten_data_length     = (TP_long   FAR *)
                                              &pTpWrite->lWrittenDataLength;
pTpWrite->unTpdu.stWrite.tpoutfurtherdataspace     = (TP_bool   FAR *)
                                              &pTpWrite->bOutFurtherDataSpace;

    /*------------------------------------------------------------------------*/
    /* WARNING: RTPM always uses TP_YES. Must be changed for other            */
    /*          implementations, if so required.                              */
    /*------------------------------------------------------------------------*/
pTpWrite->bSecure = TP_YES;
pTpWrite->unTpdu.stWrite.tpsecure        = (TP_bool   FAR *) &pTpWrite->bSecure;


/*----------------------------------------------------------------------------*/
/* Phase 2: Assignment of values if any and execution of call                 */
/* Note: The legaility of (lWrittenDataLength < lOutRequiredDataLength) is    */
/*       left to the user to resolve. No checking done here !!                */
/*----------------------------------------------------------------------------*/

pTpWrite->stPbHead.tpversion = TPVERSION;

pTpduCallBack = ro0228_getTpduFct ();
if (pTpduCallBack == LNULL)
    RM0003_RETURN (pFnctName, signed short int, R_FTPERR)

if (!pTpWrite->bIgnore) {
    if (pTpduCallBack (RC0103_Write, (rT0100_unCallBack FAR *)&pTpWrite->unTpdu)
                                                                          != OK)
        sRetCode = R_FTPERR;
}

RM0003_RETURN (pFnctName, signed short int, sRetCode)

}/*end-ro00t3_tpWrite*/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro00t4_tpPosition                                           */
/*                                                                            */
/* AUTHOR       : Shivkumar, Haran                                            */
/* DATE         : 92-02-11                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro00t4_tpPosition (
	rT00t4_stTpPosition FAR *pTpPosition)      /* <-> : see routilt.h */
/*                                                                            */
/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Calls the tpdu function for POSITIONing                                */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     sOpenId                : Identifier of file to be positioned into.     */
/*     lOffset                : Distance relative to the position after       */
/*                              TP_OPEN.                                      */
/*                                                                            */
/* OUT-PARAMETERS : none                                                      */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*      OK                                                                    */
/*                                                                            */
/* GLOBAL DATA (read only) : none                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  : none                                             */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro00t4_tpPosition*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro00t4_tpPosition")
signed   short int   sRetCode = OK;
FARPROC              pTpduCallBack;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY  (pFnctName)

/*----------------------------------------------------------------------------*/
/* Phase 1: Assignment of pointers                                            */
/*----------------------------------------------------------------------------*/

pTpPosition->unTpdu.stPosition.tp_pbhead = (TP_pbhead FAR *)
                                                         &pTpPosition->stPbHead;
pTpPosition->unTpdu.stPosition.tpopenid  = (TP_short  FAR *)
                                                         &pTpPosition->sOpenId;
pTpPosition->unTpdu.stPosition.tpoffset  = (TP_long   FAR *)
                                                         &pTpPosition->lOffset;


/*----------------------------------------------------------------------------*/
/* Phase 2: Assignment of values if any and execution of call                 */
/*----------------------------------------------------------------------------*/

pTpPosition->stPbHead.tpversion = TPVERSION;

pTpduCallBack = ro0228_getTpduFct ();
if (pTpduCallBack == LNULL)
    RM0003_RETURN (pFnctName, signed short int, R_FTPERR)

if (!pTpPosition->bIgnore) {
    if (pTpduCallBack (RC0104_Position,
                           (rT0100_unCallBack FAR *)&pTpPosition->unTpdu) != OK)
        sRetCode = R_FTPERR;
}

RM0003_RETURN (pFnctName, signed short int, sRetCode)

}/*end-ro00t4_tpPosition*/

/*EOF */

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro00t5_tpClose                                              */
/*                                                                            */
/* AUTHOR       : Shivkumar, Haran                                            */
/* DATE         : 92-02-11                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro00t5_tpClose (
	rT00t5_stTpClose   FAR *pTpClose)      /* <-> : see routilt.h */
/*                                                                            */
/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Calls the tpdu function for TP_CLOSE.                                  */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     sOpenId : Identifier of file which is to be closed.                    */
/*                                                                            */
/* OUT-PARAMETERS : none.                                                     */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*      OK                                                                    */
/*                                                                            */
/* GLOBAL DATA (read only) : none                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  : none                                             */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro00t5_tpClose*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro00t5_tpClose")
signed   short int   sRetCode = OK;
FARPROC              pTpduCallBack;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY  (pFnctName)

/*----------------------------------------------------------------------------*/
/* Phase 1: Assignment of pointers                                            */
/*----------------------------------------------------------------------------*/

pTpClose->unTpdu.stClose.tp_pbhead = (TP_pbhead FAR *) &pTpClose->stPbHead;
pTpClose->unTpdu.stClose.tpopenid  = (TP_short  FAR *) &pTpClose->sOpenId;

/*----------------------------------------------------------------------------*/
/* Phase 2: Assignment of values if any and execution of call                 */
/*----------------------------------------------------------------------------*/

pTpClose->stPbHead.tpversion = TPVERSION;

pTpduCallBack = ro0228_getTpduFct ();
if (pTpduCallBack == LNULL)
    RM0003_RETURN (pFnctName, signed short int, R_FTPERR)

if (!pTpClose->bIgnore) {
    if (pTpduCallBack (RC0105_Close, (rT0100_unCallBack FAR *)&pTpClose->unTpdu)
                                                                          != OK)
        sRetCode = R_FTPERR;
}

RM0003_RETURN (pFnctName, signed short int, sRetCode)

}/*end-ro00t5_tpClose*/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro00t6_tpDelete                                             */
/*                                                                            */
/* AUTHOR       : Shivkumar, Haran                                            */
/* DATE         : 92-02-11                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro00t6_tpDelete (
	rT00t6_stTpDelete  FAR *pTpDelete)      /* <-> : see routilt.h */
/*                                                                            */
/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Calls the tpdu function for TP_DELETE.                                 */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     sOpenId                : Identifier of file to be written to.          */
/*     lRequiredDataLength    : No. of bytes which are NOT deleted.           */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/*     lDataLength           : Actual file length.                            */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*      OK                                                                    */
/*                                                                            */
/* GLOBAL DATA (read only) : none                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  : none                                             */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro00t6_tpDelete*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro00t6_tpDelete")
signed   short int   sRetCode = OK;
FARPROC              pTpduCallBack;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY  (pFnctName)

/*----------------------------------------------------------------------------*/
/* Phase 1: Assignment of pointers                                            */
/*----------------------------------------------------------------------------*/

pTpDelete->unTpdu.stDelete.tp_pbhead              = (TP_pbhead FAR *)
                                                &pTpDelete->stPbHead;
pTpDelete->unTpdu.stDelete.tpopenid               = (TP_short  FAR *)
                                                &pTpDelete->sOpenId;
pTpDelete->unTpdu.stDelete.tprequired_data_length = (TP_long  FAR *)
                                                &pTpDelete->lRequiredDataLength;
pTpDelete->unTpdu.stDelete.tpdata_length          = (TP_long   FAR *)
                                                &pTpDelete->lDataLength;

/*----------------------------------------------------------------------------*/
/* Phase 2: Assignment of values if any and execution of call                 */
/*----------------------------------------------------------------------------*/

pTpDelete->stPbHead.tpversion = TPVERSION;

pTpduCallBack = ro0228_getTpduFct ();
if (pTpduCallBack == LNULL)
    RM0003_RETURN (pFnctName, signed short int, R_FTPERR)

if (!pTpDelete->bIgnore) {
    if (pTpduCallBack (RC0106_Delete,
                             (rT0100_unCallBack FAR *)&pTpDelete->unTpdu) != OK)
        sRetCode = R_FTPERR;
}

RM0003_RETURN (pFnctName, signed short int, sRetCode)

}/*end-ro00t6_tpDelete*/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro00t7_tpduToStore                                          */
/*                                                                            */
/* AUTHOR       : Shivkumar, Haran                                            */
/* DATE         : 92-02-11                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro00t7_tpduToStore (
	rT00t2_stTpRead    FAR *pTpRead,           /* <-> : see routilt.h */
	unsigned long  int      ulStoreHandle)     /*  -> : see rostor0.h */

/*                                                                            */
/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Copies data from TPDU file to STORE.                                   */
/*     The information to be copied is specified in pTpRead.                  */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     pTpRead                : Description of data to be read from TPDU.     */
/*     ulStoreHandle          : Store where the data is to be copied into.    */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/*     pTpRead                : Output parameters modified by tpRead ().      */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*      OK                                                                    */
/*     -R_FSTERR                                                              */
/*      Return values of tpRead() [+ve values, non-serious errors].           */
/*      [Note: +ve returns should cause release of allocated resources].      */
/*                                                                            */
/* GLOBAL DATA (read only) : none                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  : none                                             */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro00t7_tpduToStore*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro00t7_tpduToStore")
signed   short int       sRetCode              = OK;
signed   long  int       lInRequiredDataLength = LNULL;
signed   long  int       lReadDataLength       = LNULL;
signed   long  int       lBytesRemaining       = LNULL;
signed   long  int       lStoreBufferLength    = LNULL;
/* kc on 30-8-93 */
signed   long  int       lStoreIniLen          = LNULL;
/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY  (pFnctName)

lStoreBufferLength = ro0061_stbuflen (ulStoreHandle);
if (lStoreBufferLength < LNULL)
    RM0003_RETURN (pFnctName, signed short int, -R_FSTERR)

lBytesRemaining = lInRequiredDataLength = pTpRead->lInRequiredDataLength;
lReadDataLength = 0L;

/* kc on 30-8-93 */
/* lStoreIniLen = ro0062_storelength(ulStoreHandle);          K02 */
lStoreIniLen = ro0076_stwhere(ulStoreHandle);
if (lStoreIniLen < 0 )                                    
     RM0003_RETURN(pFnctName, signed short int, -R_FSTERR)
do {
    /*------------------------------------------------------------------------*/
    /* No of bytes to be written onto STORE must not exceed STORE buffer size.*/
    /*                                                                        */
    /* Note: If file length unknown, read in sizes of buffer length till end  */
    /*       of file.                                                         */
    /*       In each iteration of the loop then, if there is more data in     */
    /*       file, lBytesRemaining is each time initialized to                */
    /*       lStoreBufferLength at start of loop and becomes zero at end of   */
    /*       loop.                                                            */
    /*       In the last iteration (first = last is special case),            */
    /*       bInFurtherData = FALSE and lBytesRemaining has no meaning.       */
    /*------------------------------------------------------------------------*/
    if (lInRequiredDataLength == RC00t6_RttrSizeUnknown) {
        lBytesRemaining = pTpRead->lInRequiredDataLength = lStoreBufferLength;
    }
    else {
        pTpRead->lInRequiredDataLength = MIN (lBytesRemaining,
                                              lStoreBufferLength);
    }

    /*------------------------------------------------------------------------*/
    /* Reserve space in STORE for writing (stputs)                            */
    /* Note that the last time the stputs() is called, a certain amount of    */
    /* space is reserved, but the no. of bytes actually read may be less than */
    /* this size. This might result in the storeLength being greater than the */
    /* physical length of the actual data. The value that is supplied as      */
    /* storeLength in the calling function must be therefore the actual length*/
    /* and not the physical length.                                           */
    /*------------------------------------------------------------------------*/
    pTpRead->pInBufferAddress = ro0069_stputs (ulStoreHandle,
                                               pTpRead->lInRequiredDataLength);
    if (pTpRead->pInBufferAddress == LNULL)
        RM0003_RETURN (pFnctName, signed short int, -R_FSTERR)

    /*------------------------------------------------------------------------*/
    /* tpRead() fills up the space reserved via sputs() with data from TPDU.  */
    /* Note: tpRead returns positive return codes which is a non serious error*/
    /*------------------------------------------------------------------------*/
    sRetCode = ro00t2_tpRead (pTpRead);
    if (sRetCode != OK)
        RM0003_RETURN (pFnctName, signed short int, sRetCode)

    lReadDataLength += pTpRead->lReadDataLength;
    if (pTpRead->bInFurtherData){
        if (lInRequiredDataLength != RC00t6_RttrSizeUnknown )
          lBytesRemaining -= pTpRead->lInRequiredDataLength;
    }
    else {

        lBytesRemaining  = 0;
        /* the EOF is reached . Now resize the Store to the length of the read 
         data */                                                               
       lStoreIniLen += lReadDataLength ;                                       
         if (ro0084_stStoreResize(ulStoreHandle, lStoreIniLen) < 0)            
               RM0003_RETURN (pFnctName, signed short int, -R_FSTERR)          
    }                                                                          


} /* while ( (pTpRead->bInFurtherData));   */
     while (lBytesRemaining); 

pTpRead->lInRequiredDataLength = lInRequiredDataLength;
pTpRead->lReadDataLength       = lReadDataLength;

RM0003_RETURN (pFnctName, signed short int, sRetCode)

}/*end-ro00t7_tpduToStore*/

/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro00t8_storeToTpdu                                          */
/*                                                                            */
/* AUTHOR       : Shivkumar, Haran                                            */
/* DATE         : 92-02-11                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro00t8_storeToTpdu (
	rTro02_stStore     FAR *pStore,            /* <-> : see rouply0.h */
	rT00t3_stTpWrite   FAR *pTpWrite)          /* <-> : see routilt.h */
/*                                                                            */
/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Copies data from STORE to TPDU. The information to be copied is        */
/*     specified in the pStore (a handle, offset and a length).               */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     pTpWrite               : Description of position in TPDU where data    */
/*                              from STORE to be written into.                */
/*                                                                            */
/*     pStore                 : A handle, offset and length that specifies    */
/*                              the store, the start position of the data     */
/*                              in the store and the length of the data       */
/*                              from this start position that is to be copied */
/*                              into the TPDU.                                */
/*                                                                            */
/* OUT-PARAMETERS :                                                           */
/*                                                                            */
/*     pTpWrite               : Information about written data as returned    */
/*                              by tpWrite ().                                */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*      OK                                                                    */
/*     -R_FSTERR                                                              */
/*      Return values of tpWrite [+ve values, i.e. non serious errors]        */
/*      [Note: +ve returns should cause release of allocated resources].      */
/*                                                                            */
/* GLOBAL DATA (read only) : none                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  : none                                             */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr-ro00t8_storeToTpdu*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro00t8_storeToTpdu")
signed   short int       sRetCode               = OK;
signed   long  int       lOutRequiredDataLength = LNULL;
signed   long  int       lBytesRemaining        = LNULL;
signed   long  int       lStoreBufferLength     = LNULL;

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY  (pFnctName)

sRetCode = ro0066_storepos (pStore->hsStore, pStore->ulOffset);
if (sRetCode != OK)
    RM0003_RETURN (pFnctName, signed short int, -R_FSTERR)

lStoreBufferLength = ro0061_stbuflen (pStore->hsStore);

lBytesRemaining    = lOutRequiredDataLength = pTpWrite->lOutRequiredDataLength;

do {
    /*------------------------------------------------------------------------*/
    /* No. of bytes to be read from STORE should not exceed STORE buffer size.*/
    /*------------------------------------------------------------------------*/
    pTpWrite->lOutRequiredDataLength = MIN(lBytesRemaining, lStoreBufferLength);

    /*------------------------------------------------------------------------*/
    /* Get the read pointer in STORE buffer where data to be read is present. */
    /*------------------------------------------------------------------------*/
    pTpWrite->pOutBufferAddress = ro0072_stgets (pStore->hsStore,
                                              pTpWrite->lOutRequiredDataLength);
    if (pTpWrite->pOutBufferAddress == LNULL)
        RM0003_RETURN (pFnctName, signed short int, -R_FSTERR)

    /*------------------------------------------------------------------------*/
    /* tpWrite() writes data from STORE buffer got via sgets() onto TPDU.     */
    /* Note: tpWrite returns positive values which are non serious errors.    */
    /*------------------------------------------------------------------------*/
    sRetCode = ro00t3_tpWrite (pTpWrite);
    if (sRetCode != OK)
        RM0003_RETURN (pFnctName, signed short int, sRetCode)

    lBytesRemaining   -= pTpWrite->lOutRequiredDataLength;
} while (lBytesRemaining > 0);

pTpWrite->lOutRequiredDataLength = lOutRequiredDataLength;
pTpWrite->lWrittenDataLength     = lOutRequiredDataLength - lBytesRemaining;

RM0003_RETURN (pFnctName, signed short int, sRetCode)

}/*end-ro00t8_storeToTpdu*/
