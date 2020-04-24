/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm01.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:13:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:45  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:40  keutel
 * 	OT 11620
 * 	[1994/08/19  13:47:43  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:52  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:02  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:03  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:50:59  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:31  keutel
 * 	creation
 * 	[1994/03/21  13:20:15  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roppm01.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:39 $" } ;
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
/* TYPE         : C-MODULE                                            */
/*                                                                    */
/* NAME         : roppm01.c                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-06-29                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : PPM E/D Control functions                           */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-03-24|  original                      |hm  |         */
/* 2.0A00 K01|92-04-24|  simpleEncode: R_PNIMPL->abort,|hm  |         */
/*           |        |  invert objcpy rc > 0          |    |         */
/* 2.0A00 K02|92-06-29|  SCCS-Id changed               |hm  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/**********************************************************************/
/*                                                                    */
/* Note:                                                              */
/* The use of the Hungarian Notation to mark the C data types         */
/* is as follows:                                                     */
/*                                                                    */
/*   b   boolean (i.e. in the most cases signed int)                  */
/*   c   signed char                                                  */
/*   h   memory handle (i.e. unsigned long)                           */
/*   hs  Store handle (i.e. unsigned long)                            */
/*   i   signed int                                                   */
/*   l   signed long                                                  */
/*   o   object identifier                                            */
/*   p   pointer to any data type                                     */
/*   s   signed short                                                 */
/*   st  structure                                                    */
/*   u   unsigned int                                                 */
/*   uc  unsigned char                                                */
/*   ul  unsigned long                                                */
/*   un  union                                                        */
/*   us  unsigned short                                               */
/*   v   vector of any data type                                      */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/* Note:                                                              */
/* In order to mark code parts which are not clear or which are still */
/* missing, the use of some strings is as follows:                    */
/*                                                                    */
/*   S!?  open question (regarding ISO standard)                      */
/*   P!?  open question (regarding programming style)                 */
/*   !!   has still to be done                                        */
/*   !!!! has still to be done (very important)                       */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*              Module Identification                                 */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) roppm01.c 1.1 93/11/29   RTS-V2.00";


/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>      /* Machine dependent generation             */
#include <roerval.h>      /* Unrecoverable error codes                */
#include <rouply0.h>      /* Common type and value definitions        */
#include <routil0.h>      /* Utility interface (common functions)     */
#include <routil1.h>      /* Utility interface (object id. functions) */
#include <rodpat0.h>      /* Dispatcher interface                     */
#include <roppm00.h>      /* PPM service interface                    */
#include <roppm01.h>      /* PPM protocol interface                   */
#include <roppm04.h>      /* PPM entity invoc. context (before 03.h!) */
#include <roppm03.h>      /* PPM logging interface                    */
#include <roppm05.h>      /* PPM E/D Control function prototyping     */
#include <roppmz0.h>      /* PPM Inform function defs. protos         */
#include <roppm10.h>      /* PPM intern: macros                       */
#include <roppm12.h>      /* PPM intern: function prototyping         */


/**********************************************************************/
/*                                                                    */
/*              S T A T I C   D A T A                                 */
/*                                                                    */
/**********************************************************************/

/*ASN.1 Basic Encoding Rules */
static unsigned long int rv0410_vBerObjId [] = {3, 2, 1, 1};
/*better full ObjId length (if copied/compared in full length) P!? */
/*static rTro00_vObjectId rv0410_vBerObjId = {3, 2, 1, 1}; */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0410_encodeSimply                                 */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-04-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0410_encodeSimply (
rTro01_pfvObjectId             FAR *pTransSynName,
signed   short int       FAR *pPmNumb,
void                               *pContext,
unsigned long  int                   hNode)

/*                                                                    */
/* DESCRIPTION  : E/D Control function (simply-enc. pres. user-data)  */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0410_encodeSimply")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*!! */

  RM0003_RETURN (fnctName, signed short int, RC040N_EncodingError)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0411_encodePdvList                                */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-04-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0411_encodePdvList (
	rT0410_stPdvList   FAR       *pEncodingInfo,
	rTro01_pfvObjectId         FAR *pTransSynName,
	signed   short int FAR       *pPmNumb,
	void                               *pCxt,
	unsigned long  int            hNode)

/*                                                                    */
/* DESCRIPTION  : E/D Control function (PDV-list)                     */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0411_encodePdvList")
rT0441_stEntContext    *pEntContext;    /*pointer to the pres. entity inv. context */
signed         int      iAbsId;         /*abstract syntax identifier */
signed         int      iTransId;       /*transfer syntax identifier */
rTro03_stPdvHeader FAR *pNode;          /*pointer to PDV header */
signed   short int      rc;             /*return code of a called function */

rT0430_stContext       *pContext = (rT0430_stContext *)pCxt;


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  pEntContext = pContext->pEntContext;    /*local variable necessary P!? */

  /*------------------------------------------------------------------------*/
  /* Get information from PDV node                                          */
  /*------------------------------------------------------------------------*/

  RM0421_GlobLock (hNode, pNode, rTro03_stPdvHeader)

  *pPmNumb = pNode->sPmNumb;
  iAbsId   = (signed int) pNode->sAbstrSynId;

  if (pNode->lEncodingChoice != RCro0m_ChoiceAny)
    RM0003_RETURN (fnctName, signed short int, -R_PNIMPL)
  else
    pEncodingInfo->lEncodingChoice = pNode->lEncodingChoice;

  RM0422_GlobUnLock (hNode)


  /*------------------------------------------------------------------------*/
  /* Check if the given AbsId belongs to the proposed or negotiated DCS.    */
  /* - Check of pContext->vTransSynId because the protocol allows arbitrary */
  /*   values for the elements of pContext->vPresContId (including the      */
  /*   value RC043D_NoContId.)                                              */
  /* - For CP PPDU: pContext->vTransSynId was created temporarily during    */
  /*   P-CONNECTreq.                                                        */
  /* Note: Is this still correct when the PPM will be changed S!?           */
  /*       (e.g. implementation of default context)                         */
  /*------------------------------------------------------------------------*/

  if (pContext->vTransSynId [iAbsId] == RC043O_NoTransId)
    RM0003_RETURN (fnctName, signed short int, RC040M_WrongAbstrSynIdPdv)

  pEncodingInfo->lPresContId = pContext->vPresContId [iAbsId];


  /*------------------------------------------------------------------------*/
  /* Compute the transfer syntax object identifier (OPTIONAL).              */
  /* - ISO 8823 8.4.2.7: Shall be present if CP PPDU and if more than one   */
  /*   transfer syntaxes were proposed.                                     */
  /* - pContext->vTransSynId was created temporarily during P-CONNECTreq.   */
  /*------------------------------------------------------------------------*/

  if (pContext->lType                                 == RC041A_Cp
  &&  pEntContext->vAbstrSyn [iAbsId].vTransSynId [0]  > 1
  )
  {
    iTransId = pContext->vTransSynId [iAbsId];

    rc = ro0002_objcpy (
      pEncodingInfo->oTransSynName,               /* <-- Destination. */
      pEntContext->vTransSynName [iTransId]);     /* --> Source.      */
    if (rc > 0)                                   /* because R_LENULL */
      rc = -rc;
    if (rc)                                       /* rc now never > 0 */
      RM0003_RETURN (fnctName, signed short int, rc)
  }
  else
    pEncodingInfo->oTransSynName [0] = 0;  /*OPTIONAL */


  /*------------------------------------------------------------------------*/
  /* Compute the transfer syntax object identifier (input parameter of the  */
  /* next encoding function)                                                */
  /* - For CP PPDU: pContext->vTransSynId was created temporarily during    */
  /*   P-CONNECTreq.                                                        */
  /*------------------------------------------------------------------------*/

  iTransId = pContext->vTransSynId [iAbsId];    /*check if defined value P!? */

  pContext->pTransSynName =               /*for ANY */
  *pTransSynName          =
                (rTro01_pfvObjectId)pEntContext->vTransSynName [iTransId];

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0412_encodeExternal                               */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-04-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0412_encodeExternal (
	rT0411_stExternal  FAR       *pEncodingInfo,
	rTro01_pfvObjectId         FAR *pTransSynName,
	signed   short int FAR       *pPmNumb,
	void                         *pCxt,
	unsigned long  int            hNode)

/*                                                                    */
/* DESCRIPTION  : E/D Control function (EXTERNAL)                     */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0412_encodeExternal")
rT0441_stEntContext    *pEntContext;    /*pointer to the pres. entity inv. context */
signed         int      iAbsId;         /*abstract syntax identifier */
signed         int      iTransId;       /*transfer syntax identifier */
rTro03_stPdvHeader FAR *pNode;          /*pointer to PDV header */
signed   short int      rc;             /*return code of a called function */

rT0430_stContext       *pContext = (rT0430_stContext *)pCxt;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  pEntContext = pContext->pEntContext;    /*local variable necessary P!? */

  /*------------------------------------------------------------------------*/
  /* Get information from PDV node                                          */
  /*------------------------------------------------------------------------*/

  RM0421_GlobLock (hNode, pNode, rTro03_stPdvHeader)

  *pPmNumb = pNode->sPmNumb;
  iAbsId   = (signed int) pNode->sAbstrSynId;
/* commented by kc on 25/5 to allow Octet & Bit strings    
  if (pNode->lEncodingChoice != RCro0m_ChoiceAny)
    RM0003_RETURN (fnctName, signed short int, -R_PNIMPL)
  else      */

    pEncodingInfo->lEncodingChoice = pNode->lEncodingChoice;

  RM0422_GlobUnLock (hNode)


  /*------------------------------------------------------------------------*/
  /* Check if the given AbsId belongs to the proposed or negotiated DCS.    */
  /* - Check of pContext->vTransSynId because the protocol allows arbitrary */
  /*   values for the elements of pContext->vPresContId (including the      */
  /*   value RC043D_NoContId.)                                              */
  /* - For CP PPDU: pContext->vTransSynId was created temporarily during    */
  /*   P-CONNECTreq.                                                        */
  /* Note: Is this still correct when the PPM will be changed S!?           */
  /*       (e.g. implementation of default context)                         */
  /*------------------------------------------------------------------------*/

  if (pContext->vTransSynId [iAbsId] == RC043O_NoTransId)
    RM0003_RETURN (fnctName, signed short int, RC040M_WrongAbstrSynIdPdv)

  pEncodingInfo->bIndirectRef   = TRUE; /*ISO 8824 34.5 not supported */
  pEncodingInfo->lIndirectRef   = pContext->vPresContId [iAbsId];

  pEncodingInfo->bObjectDescrip = FALSE;


  /*------------------------------------------------------------------------*/
  /* Compute the transfer syntax object identifier (OPTIONAL).              */
  /* - ISO 8824 34.6: Shall be present if "pres. layer negotiation is in    */
  /*   use but incomplete" --> our interpretation: in CP PPDU (S!?).        */
  /* - Note: Different from PDV-list, the standard states no dependence on  */
  /*   the number of proposed transfer syntaxes.                            */
  /* - pContext->vTransSynId was created temporarily during P-CONNECTreq.   */
  /*------------------------------------------------------------------------*/

  if (pContext->lType == RC041A_Cp)
  {
    iTransId = pContext->vTransSynId [iAbsId];

    rc = ro0002_objcpy (
      pEncodingInfo->oDirectRef,                  /* <-- Destination. */
      pEntContext->vTransSynName [iTransId]);     /* --> Source.      */
    if (rc > 0)                                   /* because R_LENULL */
      rc = -rc;
    if (rc)                                       /* rc now never > 0 */
      RM0003_RETURN (fnctName, signed short int, rc)
  }
  else
    pEncodingInfo->oDirectRef [0] = 0;  /*OPTIONAL */


  /*------------------------------------------------------------------------*/
  /* Compute the transfer syntax object identifier (input parameter of the  */
  /* next encoding function)                                                */
  /* - For CP PPDU: pContext->vTransSynId was created temporarily during    */
  /*   P-CONNECTreq.                                                        */
  /*------------------------------------------------------------------------*/

  iTransId = pContext->vTransSynId [iAbsId];    /*check if defined value P!? */

  pContext->pTransSynName =               /*for ANY */
  *pTransSynName          =
                 (rTro01_pfvObjectId)pEntContext->vTransSynName [iTransId];

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0413_encodeAny                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0413_encodeAny (
	rTro76_stDefinedBy FAR * FAR *pDefinedBy,
	rTro01_pfvObjectId         FAR *pTransSynName,
	signed   short int       FAR *pPmNumb,
	void                        *pCxt,
	unsigned long  int            hNode)

/*                                                                    */
/* DESCRIPTION  : E/D Control function (ANY, ANY DEFINED BY)          */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0413_encodeAny")
rTro03_stPdvHeader FAR *pNode;           /*pointer to PDV header */

rT0430_stContext       *pContext = (rT0430_stContext *)pCxt;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*------------------------------------------------------------------------*/
  /* Store the DEFINED BY info if it is input parameter                     */
  /*------------------------------------------------------------------------*/

  if (pDefinedBy)
    pContext->pDefinedBy = (rTro76_stDefinedBy FAR *)*pDefinedBy;


  /*------------------------------------------------------------------------*/
  /* Get information from PDV node                                          */
  /*------------------------------------------------------------------------*/

  RM0421_GlobLock (hNode, pNode, rTro03_stPdvHeader)

  *pPmNumb = pNode->sPmNumb;
  /*AbstrSynId - check P!? (pContext->sAbstrSynId) */

  RM0422_GlobUnLock (hNode)


  /*------------------------------------------------------------------------*/
  /* Set other output parameters                                            */
  /*------------------------------------------------------------------------*/

  if (pContext->bNormalMode)            /*if normal-mode */
  {
    *pDefinedBy    = (rTro76_stDefinedBy FAR *)pContext->pDefinedBy;
    *pTransSynName = (rTro01_pfvObjectId      )pContext->pTransSynName;
  }
  else
  {
    *pDefinedBy    = (rTro76_stDefinedBy FAR *)LNULL;
    *pTransSynName = (rTro01_pfvObjectId      )rv0410_vBerObjId;
                                                     /*redundant for X410 S!? */
  }

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0415_decodeSimply                                 */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0415_decodeSimply (
	rTro01_pfvObjectId         FAR *pTransSynName,
	signed   short int       FAR *pAbstrSynId,
	signed   short int       FAR *pPmNumb,
	void                         *pCxt)

/*                                                                    */
/* DESCRIPTION  : E/D Control function (simply-enc. pres. user-data)  */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0415_decodeSimply")


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*!! */

  RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
  /*right value S!? */
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0416_decodePdvList                                */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0416_decodePdvList (
	rTro01_pfvObjectId         FAR *pTransSynName,
	signed   short int       FAR *pAbstrSynId,
	signed   short int       FAR *pPmNumb,
	rT0410_stPdvList         FAR *pEncodingInfo,
	void                         *pCxt)

/*                                                                    */
/* DESCRIPTION  : E/D Control function (PDV-list)                     */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0416_decodePdvList")
rT0441_stEntContext    *pEntContext;    /*pointer to the pres. entity inv. context */
signed         int      iAbsId;         /*abstract syntax identifier */
signed         int      iTransId;       /*transfer syntax identifier */

rT0430_stContext       *pContext = (rT0430_stContext *)pCxt;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  pEntContext = pContext->pEntContext;    /*local variable necessary P!? */

  /*------------------------------------------------------------------------*/
  /* Check the incoming PDU                                                 */
  /*------------------------------------------------------------------------*/

  if ((   pContext->sDcsNumb         <= 1                /*(if "fully" not o.k. or */
      &&  pContext->lType            != RC041A_Cp        /* but interworking S!?   */
      )
 /* ||  pEncodingInfo->lEncodingChoice != RCro0m_ChoiceAny */
   /* commented by kc on 28/6 to allow Octet sring & Bitstring    */
 /* if "enc. choice" not o.k.) */
  )
    RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
    /*right value S!? more than one S!? */


  /*------------------------------------------------------------------------*/
  /* Search abstract syntax identifier                                      */
  /*------------------------------------------------------------------------*/

  for (iAbsId = 0; iAbsId < RCro01_MaxAbstrSyn; iAbsId++)
    if (pContext->vPresContId [iAbsId] == pEncodingInfo->lPresContId)
      break;

  /*------------------------------------------------------------------------*/
  /* Check if the found AbsId belongs to the proposed or negotiated DCS.    */
  /* (Check of pContext->vTransSynId because the protocol allows arbitrary  */
  /* values for the elements of pContext->vPresContId, including the value  */
  /* RC043D_NoContId.)                                                      */
  /*------------------------------------------------------------------------*/

  if (iAbsId                         == RCro01_MaxAbstrSyn /*means "Not found" */
  ||  pContext->vTransSynId [iAbsId] == RC043O_NoTransId
  )
    RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
    /*right value S!? */


  /* Compute input parameter of the next decoding function. */

  pContext->sAbstrSynId =               /*for ANY */
  *pAbstrSynId          = (signed short int) iAbsId;
  *pPmNumb              = pEntContext->vAbstrSyn [iAbsId].vPmNumb [0];


  /*------------------------------------------------------------------------*/
  /* Check the transfer syntax object identifier (OPTIONAL).                */
  /* - ISO 8823 8.4.2.7: Shall be present if CP PPDU and if more than one   */
  /*   transfer syntaxes were proposed.                                     */
  /* - Implementation rule: Interworking is more important than conformance.*/
  /*   --> If CP PPDU there is no check of the presence dependent on the    */
  /*       number of proposed transfer syntaxes.                            */
  /*       Notes: - Mr. Temme says: Conformance is a prerequisite of        */
  /*                interworking tests S!?                                  */
  /*              - The number of proposed transfer syntaxes is stored      */
  /*                in pContext->vTransSynNumbInReceivedCp [] (not used     */
  /*                in the current version).                                */
  /* - Check of the object identifier value is done here and will be done   */
  /*   in the next decoding function (advantages of checking here:          */
  /*   1. native matter of the PPM                                          */
  /*   2. avoid error in the decoding function                              */
  /*   3. give pointer to the pres. entity invoc. context and not pointer   */
  /*      to pEncodingInfo to the decoding function)                        */
  /*                                                                        */
  /* Compute transfer syntax object identifier (input parameter of the      */
  /* next decoding function).                                               */
  /*------------------------------------------------------------------------*/

  if (pContext->lType                  == RC041A_Cp
  &&  pEncodingInfo->oTransSynName [0] != 0          /*OPTIONAL */
  )
  {
    /* Check if the transfer syntax is supported or not.  */

    iTransId = (signed int) ro0425_getTransId (
      pEntContext,                   /* --> Pres. entity invocation context (pointer to).   */
      iAbsId,                        /* --> Abstract syntax identifier.                     */
      pEncodingInfo->oTransSynName); /* --> Transfer syntax object identifier (pointer to). */

    if (iTransId == RC043O_NoTransId)
      RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
      /*right value S!? JM: CPR ! */
  }
  else
  {
    /* Check the presence of the transfer syntax parameter                   */
    /* (the check fails never if CP PPDU without transfer syntax parameter). */

    if (pEncodingInfo->oTransSynName [0] != 0)          /*OPTIONAL */
      RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
      /*right value S!? */


    /* Get the negotiated transfer syntax. */

    iTransId = pContext->vTransSynId [iAbsId];
    /*check if defined value P!? */
  }

  /* Compute input parameter of the next decoding function. */

  pContext->pTransSynName =               /*for ANY */
  *pTransSynName          =
              (rTro01_pfvObjectId     )pEntContext->vTransSynName [iTransId];

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0417_decodeExternal                               */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0417_decodeExternal (
	rTro01_pfvObjectId         FAR *pTransSynName,
	signed   short int       FAR *pAbstrSynId,
	signed   short int       FAR *pPmNumb,
	rT0411_stExternal        FAR *pEncodingInfo,
	void                         *pCxt)

/*                                                                    */
/* DESCRIPTION  : E/D Control function (EXTERNAL)                     */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0417_decodeExternal")
rT0441_stEntContext    *pEntContext;    /*pointer to the pres. entity inv. context */
signed         int      iAbsId;         /*abstract syntax identifier */
signed         int      iTransId;       /*transfer syntax identifier */

rT0430_stContext       *pContext = (rT0430_stContext *)pCxt;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  pEntContext = pContext->pEntContext;    /*local variable necessary P!? */

  /*------------------------------------------------------------------------*/
  /* Check the incoming PDU                                                 */
  /*------------------------------------------------------------------------*/

  if (pEncodingInfo->bIndirectRef    == FALSE            /*ISO 8824 34.5 not supported */
 /* ||  pEncodingInfo->lEncodingChoice != RCro0m_ChoiceAny */
   /* commented by kc on 24/5 to allow Octet sring & Bitstring    */
                       /*if "enc. choice" not o.k.   */
  )                                                      /* not checked in ROS V2.5 S!? */
    RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
    /*right value S!? more than one S!? */


  /*------------------------------------------------------------------------*/
  /* Search abstract syntax identifier                                      */
  /*------------------------------------------------------------------------*/

  for (iAbsId = 0; iAbsId < RCro01_MaxAbstrSyn; iAbsId++)
    if (pContext->vPresContId [iAbsId] == pEncodingInfo->lIndirectRef)
      break;

  /*------------------------------------------------------------------------*/
  /* Check if the found AbsId belongs to the proposed or negotiated DCS.    */
  /* (Check of pContext->vTransSynId because the protocol allows arbitrary  */
  /* values for the elements of pContext->vPresContId, including the value  */
  /* RC043D_NoContId.)                                                      */
  /*------------------------------------------------------------------------*/

  if (iAbsId                         == RCro01_MaxAbstrSyn /*means "Not found" */
  ||  pContext->vTransSynId [iAbsId] == RC043O_NoTransId
  )
    RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
    /*right value S!? */


  /* Compute input parameter of the next decoding function. */

  pContext->sAbstrSynId =               /*for ANY */
  *pAbstrSynId          = (signed short int) iAbsId;
  *pPmNumb              = pEntContext->vAbstrSyn [iAbsId].vPmNumb [0];


  /*------------------------------------------------------------------------*/
  /* Check the transfer syntax object identifier (OPTIONAL).                */
  /* - ISO 8824 34.6: Shall be present if "pres. layer negotiation is in    */
  /*   use but incomplete" --> our interpretation: in CP PPDU (!$).         */
  /*   (There are further inprecise things too, e.g. last sentence: "shall  */
  /*   be identified by comment"; nothing about the number of proposed      */
  /*   transfer syntaxes).                                                  */
  /* - Implementation rule: Interworking is more important than conformance.*/
  /*   --> If CP PPDU there is no check of the presence (!$).               */
  /* - Check of the object identifier value is done here and will be done   */
  /*   in the next decoding function (advantages of checking here:          */
  /*   1. native matter of the PPM                                          */
  /*   2. avoid error in the decoding function                              */
  /*   3. give pointer to the pres. entity invoc. context and not pointer   */
  /*      to pEncodingInfo to the decoding function)                        */
  /*                                                                        */
  /* Compute transfer syntax object identifier (input parameter of the      */
  /* next decoding function).                                               */
  /*------------------------------------------------------------------------*/

/*  if (pContext->lType               == RC041A_Cp
  &&  pEncodingInfo->oDirectRef [0] != 0              *OPTIONAL *
  ) kc on 18-8-94 */
  if (pContext->lType  == RC041A_Cp)
  {
    if (pEncodingInfo->oDirectRef[0] != 0 )
    {

    /* Check if the transfer syntax is supported or not.  */

    iTransId = (signed int) ro0425_getTransId (
      pEntContext,                /* --> Pres. entity invocation context (pointer to).   */
      iAbsId,                     /* --> Abstract syntax identifier.                     */
      pEncodingInfo->oDirectRef); /* --> Transfer syntax object identifier (pointer to). */

    if (iTransId == RC043O_NoTransId)
      RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
      /*right value S!? JM: CPR ! */
   }
   else
   {
    /* If the CP PDU with only Indirect Ref. & negotiated for more than one */
    /* TS then select the BER if present in the TS list for the AS or       */
    /* select the TS which is negotiated. So the order of selecton is       */
    /*  1) Direct  Ref.                                                     */
    /*  2) BER                                                              */
    /*  3) TS negotiated.                                                   */
    /* by kc on 18-08-94                                                    */
   if ( pContext->vTransSynNumbInReceivedCp [iAbsId] > 1 )
   {   /* only more than one TS  */
     iTransId = (signed int) ro0425_getTransId (
       pEntContext,                /* --> Pres. entity invocation context (pointer to).   */
       iAbsId,                     /* --> Abstract syntax identifier.                     */
       rv0410_vBerObjId);         /* --> Transfer syntax object identifier (pointer to). */

     if (iTransId == RC043O_NoTransId)
        iTransId = pContext->vTransSynId[iAbsId];
    
    } else iTransId = pContext->vTransSynId [iAbsId]; /* only one TS */
   } /* end of else of Indir ref */ 
  } /* end of CP PDU check */  
  else
   {
    /* Check the presence of the transfer syntax parameter                   */
    /* (the check fails never if CP PPDU without transfer syntax parameter). */

    if (pEncodingInfo->oDirectRef [0] != 0)          /*OPTIONAL */
      RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
      /*right value S!? */


    /* Get the negotiated transfer syntax. */

    iTransId = pContext->vTransSynId [iAbsId];
    /*check if defined value S!? */
  }

  /* Compute input parameter of the next decoding function. */

  pContext->pTransSynName =               /*for ANY */
  *pTransSynName          =
              (rTro01_pfvObjectId      )pEntContext->vTransSynName [iTransId];

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0418_decodeAny                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0418_decodeAny (
	rTro76_stDefinedBy FAR * FAR *pDefinedBy,
	rTro01_pfvObjectId         FAR *pTransSynName,
	signed   short int       FAR *pAbstrSynId,
	signed   short int       FAR *pPmNumb,
	signed   short             sAnyNumb,
	void                         *pCxt)

/*                                                                    */
/* DESCRIPTION  : E/D Control function (ANY, ANY DEFINED BY)          */
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0418_decodeAny")
rT0441_stEntContext    *pEntContext;     /*pointer to the pres. entity inv. context */

rT0430_stContext       *pContext = (rT0430_stContext *)pCxt;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  pEntContext = pContext->pEntContext;  /*local variable necessary P!? */

  /*------------------------------------------------------------------------*/
  /* Store the DEFINED BY info if it is input parameter                     */
  /*------------------------------------------------------------------------*/

  if (pDefinedBy)
    pContext->pDefinedBy = (rTro76_stDefinedBy FAR *)*pDefinedBy;

  /*------------------------------------------------------------------------*/
  /* Set output parameters                                                  */
  /*------------------------------------------------------------------------*/

  if (pContext->bNormalMode)            /*if normal-mode */
  {
    *pDefinedBy    = (rTro76_stDefinedBy FAR *)pContext->pDefinedBy;
    *pTransSynName = (unsigned  long int FAR *)pContext->pTransSynName;
    *pAbstrSynId   = pContext->sAbstrSynId;


    /* Check the range of the "ANY number". */

    if (sAnyNumb < 0
    ||  sAnyNumb > RC0440_MaxDecodeControl
    )
      RM0003_RETURN (fnctName, signed short int, -R_PANYNU)


    *pPmNumb = pEntContext->vAbstrSyn [*pAbstrSynId].
               vPmNumb [sAnyNumb];

    /* Check the computed PM number. */

    if (*pPmNumb == RC0441_NoPmNumb)
      RM0003_RETURN (fnctName, signed short int, RCro6m_UnexpectedPpduParam)
      /*right value S!? */
  }
  else
  {
    *pDefinedBy    = (rTro76_stDefinedBy FAR *)0;
    *pTransSynName = (rTro01_pfvObjectId      )rv0410_vBerObjId;
                                                     /*redundant for X410 S!? */
    *pAbstrSynId   = -1;               /*#define P!? */
    *pPmNumb       = RC0220_RtseUser;
  }

  RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/

             
/*EOF */
