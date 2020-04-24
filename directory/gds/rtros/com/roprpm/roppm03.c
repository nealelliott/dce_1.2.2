/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm03.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:13:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:52  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:55  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:07  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:06  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:51:26  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:36  keutel
 * 	creation
 * 	[1994/03/21  13:20:38  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roppm03.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:45 $" } ;
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
/* NAME         : roppm03.c                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-06-29                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : PPM encoding and decoding functions                 */
/*                                                                    */
/*   This C module provides functions which                           */
/*                                                                    */
/*     - encode a PDV node into a PDV encoding                        */
/*     - decode a PDV encoding into a PDV node                        */
/*     - clear user data references within a pres. PDV node (P!?).    */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-03-25|  original                      |hm  |         */
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
/*   bs  bit string (i.e. unsigned long)                              */
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

static char *sccsid =    "@(#) roppm03.c 1.2 93/12/20   RTS-V2.00";


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
#include <rouply1.h>      /* PM service interface structures          */
#include <routil0.h>      /* Utility interface                        */
#include <rostor0.h>      /* Store interface                          */
#include <rodpat0.h>      /* Dispatcher interface                     */
#include <rodpat1.h>      /* Dispatcher en/de-coding interface        */
#include <rospm00.h>      /* SPM service interface                    */
#include <rospmz0.h>      /* SPM Inform function defs & structs       */
#include <roppm00.h>      /* PPM service interface                    */
#include <roppm01.h>      /* PPM protocol interface                   */
#include <roppm04.h>      /* PPM entity invoc. context (before 03.h!) */
#include <roppm05.h>      /* PPM                                      */
#include <roppm03.h>      /* PPM logging interface                    */
#include <roppm10.h>      /* PPM intern: macros                       */
#include <roppm11.h>      /* PPM intern: types and constants          */
#include <roppmz0.h>      /* PPM intern: MAVROS     definitions       */
#include <roppmzx.h>      /* PPM intern: MAVROS     definitions       */
#include <roppmzy.h>      /* PPM intern: SniPrivate definitions       */
#include <roppmzw.h>      /* PPM intern: SniPrivate prototyping       */
#include <roppm12.h>      /* PPM intern: Function prototypes          */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0430_encodPpm                                     */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0430_encodPpm (
	unsigned       int        uOutEvent,
	unsigned long  int        ulConnectResult,
	unsigned long  int FAR  * uloffset,
	rT0430_stContext   NEAR * pContext,
	unsigned long  int FAR  * pStore,
	unsigned long  int FAR  * phNode)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0430_encodPpm")
rT041G_unPpdu  FAR *pNode;           /*pointer to PDV node */
signed   long  int  lStoreBuffSize;  /*Store buffer size (after resize) */
signed   short int  rc;              /*return code of a called function */
signed   short int  rc1;             /*return code of a called function */
unsigned long  int  hAcb=0;          /* acb passed for inform fn        */
rT08z0_ulStoreOffsetLen info;
unsigned long  int  offset;
/*
unsigned long  int  hNode;
char           FAR *pOrigBuffer;
char           FAR *pSendBuffer;
char           FAR *pStoreAddress;
unsigned long  int  ulLength;
*/
unsigned long  int  PdvNode=0;
signed   short int  route=0;  
char          FAR * cBuf = NULL;  /* kc . To Move the store Ptr        */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*------------------------------------------------------------------*/
  /* Return if no PDV tree exists.                                    */
  /*------------------------------------------------------------------*/

  if (!*phNode)
    RM0003_RETURN (fnctName, signed short int, OK)

  /*------------------------------------------------------------------*/
  /* Return incase of SCONrspNeg & Redirect                           */
  /*------------------------------------------------------------------*/
   if ( (uOutEvent == RC0803_SCONNECTrsp) &&
      ((ulConnectResult & RCro85_Redirection ) == RCro85_Redirection) ){
          rc = ro0246_clearPdvTree (phNode); 
           RM0003_RETURN (fnctName, signed short int, OK)
   }


  /*------------------------------------------------------------------*/
  /* Create a Store.                                                  */
  /* (Not if X410-mode RTTR APDU; there the Store from the            */
  /*  RTPM PDV node is used.)                                         */
  /*------------------------------------------------------------------*/

  /* following condition would  be true for Sni Private mode too.     */
  /* hence  a new store would be created   :: vp                      */

  if (pContext->bNormalMode                   /*(if normal-mode or      */
  ||  uOutEvent != RC080c_SDATAreq            /* if not S-DATA request) */
  )
  {
    *pStore = ro0060_newstore (
      ST_USEMAXBUF,                         /* --> Max. buffer length.      */
      ST_CLOSED,                            /* --> Open mode: no file open. */
      &rc);                                 /* <-- Error value.             */
      /* max. buffer length event specific, ARP !! P!?) */

    if (rc)
      RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

      /* call spm inform function to get  store offset */
/*    info= uOutEvent; */
    hAcb = ro0214_getAcbEcb (NULL);
    rc = ro0231_getInform (hAcb, RC0214_Session, RC08z1_InformStoreOffset, 
                           &info);
      
    if (rc)                                                
      RM0003_RETURN (fnctName, signed short int, rc)

    *uloffset = offset = info;
    if (offset) {
        cBuf = ro0069_stputs ( *pStore , offset);
        if(cBuf)ro0043_memset (cBuf , 0 , offset); 
        else 
           RM0003_RETURN (fnctName, signed short int, -R_PSTERR)
                    /* fills the store with dummy data */
    }


    rc =  ro0066_storepos (*pStore , offset);
      
    if (rc)                                                
      RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

  } /* end create new  store & get offset from Spm */


  /*------------------------------------------------------------------*/
  /* Process special events for X410-mode.                            */
  /* Call the RTPM X410-mode encoding function.                       */
  /*------------------------------------------------------------------*/

  if (!pContext->bNormalMode                  /*(if X410-mode and */
  &&  (  uOutEvent == RC080c_SDATAreq         /* (if S-DATA request or */
      || uOutEvent == RC080e_STOKEN_PLEASEreq /*  if S-TOKEN-PLEASE request)) */
      )
  )
  {
    rc = ro0244_encodX410 (
      (signed short int)RC0218_Rtse,/* --> Rtse Protocol machine No.          */
       pStore,                   /* --> Store handle if S-TOKEN-PLEASE request*/
                                 /* <-- Store handle if S-DATA request.       */
       pContext,                 /* --> PPM context (pointer to).             */
      (unsigned long int FAR *)phNode);
                                 /* <-> Memory handle to PDV node (pointer to)*/
  }


  /*------------------------------------------------------------------*/
  /* Process a pres. PDV node (normal-mode or X410-mode).             */
  /* Call a MAVROS created function.                                  */
  /*------------------------------------------------------------------*/

  else
  {
    RM0421_GlobLock (*phNode, pNode, rT041G_unPpdu)

  /*------------------------------------------------------------------*/
  /* SNI Private mode encoding                                        */
  /*------------------------------------------------------------------*/

    if (pContext->bNormalMode == -1) /* for Sni Private mode  */
    {
      /* preserve original position of the buffer */

    /*  pOrigBuffer=pSendBuffer; */
 
      switch (uOutEvent) {

      case RC0801_SCONNECTreq:

        rc = CPSniP_type_cod (
          *pStore     ,             /* --> Buffer pointer where encoded byte */
                                    /*     string to be stored temporarily.  */
          (rT041A_stCp FAR *) pNode,/* --> PDV node (pointer to).            */
          pContext);                /* --> PPM context (pointer to).         */
        if (!rc){
          PdvNode = pNode->stCp.stUserData.unUserData.stFully.vPdvList[0]; 
          route = RC0216_Acse; 
        }
        break;

      case RC0803_SCONNECTrsp:

        if (ulConnectResult == RCro80_Accept)
        {
        rc = CPASniP_type_cod (
          *pStore,             /* --> Buffer pointer where encoded byte */
                                    /* byte string to be stored temporarily  */
          (rT041B_stCpa FAR *) pNode,/* --> PDV node (pointer to).           */
          pContext);                /* --> PPM context (pointer to).         */
        if(!rc){ 
          PdvNode = pNode->stCpa.stUserData.unUserData.stFully.vPdvList[0]; 
          route = RC0216_Acse;
        }
        break;
        }  /* SConnectRsp+ */ 
     
        else
        {
        rc = CPRSniP_type_cod (
          *pStore     ,          /* --> Buffer pointer where encoded byte   */
                                 /* byte string to be stored temporarily    */  
          (rT041C_stCpr FAR *) pNode,/* --> PDV node (pointer to).           */
          pContext);            /* --> PPM context (pointer to).           */
        if (!rc){ 
          PdvNode = pNode->stCpr.stUserData.unUserData.stFully.vPdvList[0]; 
          route = RC0216_Acse;
        }
        break;
        }  /* SConnectRsp- */ 

      case RC0809_SUABORTreq:

       if (pNode->stPdvHeader.lType == RC041D_Aru)
       {
        rc = ABUSniP_type_cod (
          *pStore,                 /* --> Store handle (encoded byte string). */
          (rT041D_stAru FAR *) pNode,/* --> PDV node (pointer to).           */
          pContext);               /* --> PPM context (pointer to).           */
        if(!rc){
          PdvNode = pNode->stAru.stUserData.unUserData.stFully.vPdvList[0]; 
          route = RC0216_Acse;
        }
        break;
       }

       else 
       {
        rc = ABPSniP_type_cod (
          *pStore,                 /* --> Store handle (encoded byte string). */
          (rT041E_stArp FAR *) pNode,/* --> PDV node (pointer to).           */
          pContext);               /* --> PPM context (pointer to).           */
        break;
       } 

      default:
        rc = UserSniP_type_cod (
          *pStore,                      /*-->Store handle(encoded byte string)*/
          (rT041F_stOther FAR *) pNode, /*--> PDV node sub-structure (ptr to) */
          pContext,                     /*--> PPM context (pointer to)        */
          (signed short int FAR *)&route);
        if(!rc)
          PdvNode = pNode->stOther.stUserData.unUserData.stFully.vPdvList[0]; 
        break;

      } /*to "switch (uOutEvent)" for Sni Private  Mode*/





     /* call despatcher to switch encoding */ 
     if ((route)  && (PdvNode) && (!rc) )
         rc = ro0242_encodSniP (route, *pStore, pContext,
                                (unsigned long int FAR *)&PdvNode); 
      if (( rc == OK) && (uOutEvent == RC0803_SCONNECTrsp) &&
          ( ulConnectResult != RCro80_Accept )) {
             rc = ro0067_stputc(*pStore , (unsigned char)0xff ); 
      }   
    } /* sni private mode encoding */
    else
    { /* normal or Sni Private mode */

  /*------------------------------------------------------------------*/
  /* Normal mode encoding                                             */
  /*------------------------------------------------------------------*/

     switch (uOutEvent)
     {
      case RC0801_SCONNECTreq:
        rc = CP_type_cod (
          *pStore,                      /* --> Store handle (encoded byte string). */
          -1,                           /* --> SET/SEQUENCE coding mode (MAVROS).  */
          0,                            /* --> SET/SEQUENCE coding mode (MAVROS).  */
          (rT041A_stCp FAR *) pNode,    /* --> PDV node (pointer to).              */
          pContext);                    /* --> PPM context (pointer to).           */
        break;

      case RC0803_SCONNECTrsp:
        if (ulConnectResult == RCro80_Accept)
          rc = CPA_PPDU_cod (
            *pStore,                    /* --> Store handle (encoded byte string). */
            -1,                         /* --> SET/SEQUENCE coding mode (MAVROS).  */
            0,                          /* --> SET/SEQUENCE coding mode (MAVROS).  */
            (rT041B_stCpa FAR *) pNode, /* --> PDV node (pointer to).              */
            pContext);                  /* --> PPM context (pointer to).           */

   /*     else if ((ulConnectResult & RCro85_Redirection ) == RCro85_Redirection)
                 break; */
         /* commented by kc on 7/7/93    
          rc = CP_type_cod (                                                    
          *pStore,                      
          -1,                          
          0,                          
          (rT041A_stCp FAR *) pNode, 
          pContext);                
        ****** end of comment */
                                                                              
        else
          rc = CPR_PPDU_cod (
            *pStore,                    /* --> Store handle (encoded byte string). */
            -1,                         /* --> SET/SEQUENCE coding mode (MAVROS).  */
            0,                          /* --> SET/SEQUENCE coding mode (MAVROS).  */
            (rT041C_stCpr FAR *) pNode, /* --> PDV node (pointer to).              */
            pContext);                  /* --> PPM context (pointer to).           */
        break;

      case RC0809_SUABORTreq:
        rc = Abort_type_cod (
          *pStore,                      /* --> Store handle (encoded byte string). */
          -1,                           /* --> SET/SEQUENCE coding mode (MAVROS).  */
          0,                            /* --> SET/SEQUENCE coding mode (MAVROS).  */
          (rT041G_unPpdu FAR *) pNode,  /* --> PDV node (pointer to).              */
          pContext);                    /* --> PPM context (pointer to).           */
        break;

      default:
        if (!pContext->bNormalMode)     /*SS-user data illegal if X410-mode */
          RM0003_RETURN (fnctName, signed short int, -R_PUDATN)
          /*Is this redundant check necessary (I think no) P!? */

        rc = User_data_cod (
          *pStore,                      /* --> Store handle (encoded byte string). */
          -1,                           /* --> SET/SEQUENCE coding mode (MAVROS).  */
          0,                            /* --> SET/SEQUENCE coding mode (MAVROS).  */
          &pNode->stOther.stUserData,   /* --> PDV node sub-structure (pointer to).*/
          pContext);                    /* --> PPM context (pointer to).           */
        break;
    } /*to "switch (uOutEvent)" */
   } /* end normal mode encoding */ 
    if (rc >= 0)                        /*if OK or provider abort */
      RM0422_GlobUnLock (*phNode)

  } /*to "if (!bNormalMode ...) else" i.e. Normal/Sni Private mode */


  /*------------------------------------------------------------------*/
  /* Process return values (for X410-mode too).                       */
  /*------------------------------------------------------------------*/

  if (rc < 0)
  {
    /*map return value if Store error */
    if (rc >= ST_MINERRVAL)
      rc = -R_PSTERR;
    RM0003_RETURN (fnctName, signed short int, rc)
  }

  if (rc == 0)
  {
    rc = ro0246_clearPdvTree (phNode);  /*sets *phNode to 0 if successful */
    if (rc)                             /*rc never > 0 */
      RM0003_RETURN (fnctName, signed short int, rc)

    lStoreBuffSize = ro0083_stbufresize (*pStore);
    if (lStoreBuffSize < 0)
      RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

    rc = ro0073_storeidle (*pStore);
    if (rc)                             /*rc never > 0 */
      RM0003_RETURN (fnctName, signed short int, -R_PSTERR)
    /* by kc on 7/7/93 */
/*    if ((ulConnectResult & RCro85_Redirection ) == RCro85_Redirection)
           rc = ro0075_deletestore (*pStore); */
  }

  /*------------------------------------------------------------------*/
  /* If rc > 0 (i.e. soft error): Mapping of return values which      */
  /* are set by MAVROS created functions.                             */
  /* (Note: deletestore() and clearPdvTree() will be called when      */
  /*  P-PABORT is issued.                                             */
  /*  This has the advantage that there is no danger of lost of       */
  /*  the original return value.)                                     */
  /*------------------------------------------------------------------*/

  else
  {
    if (rc == RCro6l_UnrecognizedPpduParam
    ||  rc == RCro6n_InvalidPpduParam
    )
      rc = RC040N_EncodingError;

    /*----------------------------------------------------------------*/
    /* The MAVROS created functions put their error code into the     */
    /* Store Control Block --> Necessary:                             */
    /* Reset the Store error code to avoid error at Store deletion    */
    /* (deletion will be done when P-PABORT will be issued).          */
    /* The return value (old MAVROS error code) is ignored. P!?       */
    /*----------------------------------------------------------------*/

    rc1 = ro0080_seterror (*pStore, OK);
  }

  RM0003_RETURN (fnctName, signed short int, rc)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0431_decodPpm                                     */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-25                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0431_decodPpm (
	unsigned       int        uInEvent,
	rT0430_stContext   NEAR * pContext,
	unsigned long  int FAR  * pStore,
	unsigned long  int FAR  * phNode,
	unsigned long  int        ulOffset)

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0431_decodPpm")
signed   long  int  lRtseApduType;   /*RTSE APDU type (X410-mode) */
signed   long  int  lStoreLength;    /*Store length (max. length of the byte string) */
signed   long  int  lStoreWhere;     /*actual Store position */
rT041G_unPpdu  FAR *pNode;           /*pointer to PDV node */
signed   short int  rc;              /*return code of a called function */
signed   short int  rc1;             /*return code of a called function */
/*
char                    FAR *pStoreAddress;
rT08z0_ulStoreOffsetLen      info;
*/
unsigned long  int  FAR *PdvNode = LNULL;
signed   short int  route=0;  
signed   short int  sAbsId=0;  
signed   short int  sPresConId=0;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*------------------------------------------------------------------*/
  /* Return if no Store exists.                                       */
  /* If no Store (i. e. encoded PPDU) exists ro0431_decodPpm returns  */
  /* immediately. If PS-user data (S-DATA indication) or PPCI (CP,    */
  /* CPA, CPR, ARU, ARP) are mandatory the return code will cause a   */
  /* provider abort. S-TOKEN-PLEASE indication: SS-user data are      */
  /* mandatory only in X410-mode (RTSE APCI).                         */
  /* (S-DATA indication: better hard error because SS-user data are   */
  /* mandatory P!? )                                                  */
  /*------------------------------------------------------------------*/

  if (!*pStore)
  {
    switch (uInEvent)
    {
      case RC080f_STOKEN_PLEASEind:
        /*SS-user data are only mandatory in X410-mode (RTSE APCI) */
        if (pContext->bNormalMode)
          break;
      case RC080d_SDATAind:
      case RC0802_SCONNECTind:
      case RC0439_IEvCPA:
      case RC043b_IEvCPR:
      case RC080a_SUABORTind:
        RM0003_RETURN (fnctName, signed short int, RC041l_UnrecognizedPpdu)
      default:
        break;
    } /*to "switch (uInEvent)" */

    RM0003_RETURN (fnctName, signed short int, OK)
  }


  /*------------------------------------------------------------------*/
  /* Activate the Store.                                              */
  /*------------------------------------------------------------------*/

  /* "hsStore = *pStore;"  because of efficiency P!? */
/*  info = uInEvent; */
/*
  rc = ro0231_getInform (ro0214_getAcbEcb (NULL), 
                         RC0214_Session, 
                         RC08z1_InformStoreOffset, &info);
  ulOffset = info; */

  rc = ro0074_storeactive (
    *pStore,              /* --> Store handle.                                   */
    ulOffset);            /* --> Store position from which the buffer is filled. */

  if (rc)
    RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

  /*------------------------------------------------------------------*/
  /* Compute the Store length.                                        */
  /* Used as input parameter of MAVROS functions and for looking if   */
  /* the whole Store was decoded.                                     */
  /*------------------------------------------------------------------*/
  rc =  ro0066_storepos (*pStore , ulOffset);

  lStoreLength = ro0062_storelength (*pStore);
  if (lStoreLength < 0)
    RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

/* kc : check for  the Sni PTS and set the bNormalMode on 26.5  */
  if (uInEvent == RC0802_SCONNECTind)
      rc  = ro04y0_CheckSniPTS( *pStore , pContext);

  /*------------------------------------------------------------------*/
  /* Process special events for X410-mode.                            */
  /* Call the RTPM X410-mode decoding function.                       */
  /*------------------------------------------------------------------*/

  if (!pContext->bNormalMode                 /*(if X410-mode and */
  &&  (  uInEvent == RC080d_SDATAind         /* (if S-DATA request or */
      || uInEvent == RC080f_STOKEN_PLEASEind /*  if S-TOKEN-PLEASE request)) */
      )
  )
  {
    rc = ro0245_decodX410 (
      RC0218_Rtse,           /* --> Rtse Protocol machine No.               */
      uInEvent,              /* --> Used to  determine RT APDU type in Cf Fn*/
      lRtseApduType,         /* --> Not used                                */
     *pStore,                /* --> Store handle (encoded byte string).     */
      pContext,              /* --> PPM context (pointer to).               */
      phNode);               /* <-> Memory handle to PDV node (pointer to). */
  }


  /*------------------------------------------------------------------*/
  /* Process a pres. PDV node (normal-mode or X410-mode).             */
  /*------------------------------------------------------------------*/

  else
  {
    /*----------------------------------------------------------------*/
    /* Alloc, lock and initialize a PDV node                          */
    /*----------------------------------------------------------------*/

    RM0420_GlobAlloc (*phNode, sizeof (rT041G_unPpdu))
      /*later: maybe PDU specific size !! (P!?) */
    RM0421_GlobLock (*phNode, pNode, rT041G_unPpdu)

    pNode->stPdvHeader.sPmNumb = RC0215_Presentation;
    /* Note: sAbstrSynId, lEncodingChoice are not relevant */

    /*----------------------------------------------------------------*/
    /* Call a MAVROS created function.                                */
    /* Before: Check the beginning tag of the PPDU (will be done once */
    /* more in the MAVROS created function except of CP PPDU and CPA  */
    /* PPDU; but checking here has the advantage that the appropriate */
    /* abort reason "Unrecognized PPDU" is set).                      */
    /*----------------------------------------------------------------*/
    if (pContext->bNormalMode == -1) /* for Sni Private mode  */
    {
     /* get store address to copy encoded string to store */            
    /*                                                                    
      if ((pStoreAddress = ro0069_stputs (*pStore, 
                                           sizeof (rT041G_unPpdu)))  == NULL) 
        RM0003_RETURN (fnctName, signed short int, -R_STOERR)           
       by kc on 16/5 */                                                                   
     switch (uInEvent)
     {
      case RC0802_SCONNECTind:
          rc = CPSniP_type_dec (
             *pStore,                  /*-->Store Address encoded byte string)*/
            (rT041A_stCp FAR *) pNode, /*-->PDV node (pointer to).            */
            pContext);                 /*-->PPM context (pointer to).         */
        if ( pNode->stCp.bUserData == TRUE) {
            PdvNode = (unsigned long int FAR *)
                         &pNode->stCp.stUserData.unUserData.stFully.vPdvList[0];
        }
        route = RC0216_Acse;                                            

        break;

      case RC0439_IEvCPA:
          rc = CPASniP_type_dec (
            *pStore,            /*-->Store Address encoded byte string)*/
            (rT041B_stCpa FAR *) pNode,/*-->PDV node (pointer to).            */
            pContext);                 /*-->PPM context (pointer to).         */

        if ( pNode->stCpa.bUserData == TRUE) {
            PdvNode = (unsigned long int FAR *)
                      &pNode->stCpa.stUserData.unUserData.stFully.vPdvList[0];  
        }
        route = RC0216_Acse;

        break;

      case RC043b_IEvCPR:
          rc = CPRSniP_type_dec (
            *pStore,            /*-->Store Address encoded byte string)*/
            (rT041C_stCpr FAR *) pNode,/*-->PDV node (pointer to).            */
            pContext);                 /*-->PPM context (pointer to).         */

        if ( pNode->stCpr.bUserData == TRUE) {
           PdvNode = (unsigned long int FAR *)
                     &pNode->stCpr.stUserData.unUserData.stFully.vPdvList[0]; 
        }
        route = RC0216_Acse;

        break;

      case RC080a_SUABORTind:
          rc = ABUSniP_type_dec (
            *pStore,            /*-->Store Address encoded byte string)*/
            (rT041D_stAru FAR *) pNode,/*-->PDV node (pointer to).            */
            pContext);                 /*-->PPM context (pointer to).         */

        if ( pNode->stAru.bUserData == TRUE) {
	   PdvNode = (unsigned long int FAR *)
	             &pNode->stAru.stUserData.unUserData.stFully.vPdvList[0];
        }
	route = RC0216_Acse;

        break;

      case RC043l_IEvARP:
          rc = ABPSniP_type_dec (
            *pStore,            /*-->Store Address encoded byte string)*/
            (rT041E_stArp FAR *) pNode,/*-->PDV node (pointer to).            */
            pContext);                 /*-->PPM context (pointer to).         */
        break;

      default:
          rc = UserSniP_type_dec (
            *pStore,            /*-->Store Address encoded byte string)*/
            (rT041F_stOther FAR *) pNode,/*-->PDV node (pointer to).          */
            pContext);                 /*-->PPM context (pointer to).         */
            route = ro0070_stgetc(*pStore);
            
            sPresConId = ro0070_stgetc(*pStore); /* to get the BER or Octet mask*/
            {
             rTro00_vObjectId  TransSynName;
             signed short int  sPmNumb;
             rT0410_stPdvList  stEncodingInfo;
           
             stEncodingInfo.lPresContId=(signed long int)sPresConId; 
             stEncodingInfo.oTransSynName [0] = 0;
             
             rc = ro0416_decodePdvList ( 
                   (rTro01_pfvObjectId       FAR *) &TransSynName,
                   (signed short int         FAR *) &sAbsId,
                   (signed short int         FAR *) &sPmNumb,
                   (rT0410_stPdvList         FAR *) &stEncodingInfo,
                   (rT0430_stContext             *) pContext );       

            } 

          PdvNode = (unsigned long int FAR *)
                    &pNode->stOther.stUserData.unUserData.stFully.vPdvList[0];
          if (!rc) { /* ** SKH on 19/6 */
              /* to get the route get the byte from store        */
              route = ro0077_stgetcp(*pStore);
              rc = ro04y1_routeSnixAutoNo ((signed short int FAR *)&route);
          }
        break;
     } /* end switch uInEvent */  
     if (!rc) { /* **SKH on 19/6 */
       if ( (route)  && ( PdvNode)  )
          rc = ro0243_decodSniP (route, *pStore, pContext,PdvNode,
                                 (signed short int FAR *)&sAbsId);
     }
    } /* Sni Private Mode decoding */
    else {
    switch (uInEvent)
    {
      case RC0802_SCONNECTind:
        pContext->lType          =            /*for use in E/D Control functions         */
        pNode->stPdvHeader.lType = RC041A_Cp;
        rc = CP_type_tag (*pStore);           /*check the beginning tag (macro)          */
        if (rc == 0)
          rc = CP_type_dec (
            *pStore,                          /* --> Store handle (encoded byte string). */
            (unsigned long int) lStoreLength, /* --> Max. length of encoded byte string. */
            (rT041A_stCp FAR *) pNode,        /* --> PDV node (pointer to).              */
            pContext);                        /* --> PPM context (pointer to).           */
        break;

      case RC0439_IEvCPA:
        pContext->lType          =            /*for use in E/D Control functions         */
        pNode->stPdvHeader.lType = RC041B_Cpa;
        rc = CPA_PPDU_tag (*pStore);          /*check the beginning tag (macro)          */
        if (rc == 0)
          rc = CPA_PPDU_dec (
            *pStore,                          /* --> Store handle (encoded byte string). */
            (unsigned long int) lStoreLength, /* --> Max. length of encoded byte string. */
            (rT041B_stCpa FAR *) pNode,       /* --> PDV node (pointer to).              */
            pContext);                        /* --> PPM context (pointer to).           */
        break;

      case RC043b_IEvCPR:
        pContext->lType          =            /*for use in E/D Control functions         */
        pNode->stPdvHeader.lType = RC041C_Cpr;
        rc = CPR_PPDU_tag (*pStore);          /*check the beginning tag (macro)          */
        if (rc == 0)
          rc = CPR_PPDU_dec (
            *pStore,                          /* --> Store handle (encoded byte string). */
            (unsigned long int) lStoreLength, /* --> Max. length of encoded byte string. */
            (rT041C_stCpr FAR *) pNode,       /* --> PDV node (pointer to).              */
            pContext);                        /* --> PPM context (pointer to).           */
        break;

      case RC080a_SUABORTind:
        pContext->lType = RC041D_Aru;         /*for use in E/D Control functions         */
                                              /*(important: set a value != RC041A_Cp)    */
        rc = Abort_type_tag (*pStore);        /*check the beginning tag (macro)          */
        if (rc == 0)
          rc = Abort_type_dec (
            *pStore,                          /* --> Store handle (encoded byte string). */
            (unsigned long int) lStoreLength, /* --> Max. length of encoded byte string. */
            (rT041G_unPpdu FAR *) pNode,      /* --> PDV node (pointer to).              */
            pContext);                        /* --> PPM context (pointer to).           */
        break;

      default:
        if (!pContext->bNormalMode)           /*SS-user data illegal if X410-mode */
          rc = RC041m_UnexpectedPpdu;         /*note: only given to PS-user (no ARP) */

        pContext->lType          =            /*for use in E/D Control functions         */
        pNode->stPdvHeader.lType = RC041F_Other;
        rc = User_data_tag (*pStore);         /*check the beginning tag (macro)          */
        if (rc == 0)
          rc = User_data_dec (
            *pStore,                          /* --> Store handle (encoded byte string). */
            (unsigned long int) lStoreLength, /* --> Max. length of encoded byte string. */
            &pNode->stOther.stUserData,       /* --> PDV node (pointer to).              */
            pContext);                        /* --> PPM context (pointer to).           */
        break;
    } /*to "switch (uInEvent)" */
   } /* normal or X410 mode  */ 

    if (rc >= 0)                            /*if OK or provider abort */
      RM0422_GlobUnLock (*phNode)

  } /*to "if (!bNormalMode ...) else" */


  /*------------------------------------------------------------------*/
  /* Process return values (for X410-mode too).                       */
  /*------------------------------------------------------------------*/

  if (rc < 0)
  {
    /*map return value if Store error */
    if (rc >= ST_MINERRVAL)
      rc = -R_PSTERR;
    RM0003_RETURN (fnctName, signed short int, rc)
  }

  if (rc == 0)
  {
    /*----------------------------------------------------------------*/
    /* Look if the whole Store is decoded. Force abort if not.        */
    /* Exceptions:                                                    */
    /* - Ignore CPC-type values.                                      */
    /* - Ignore length difference if X410-mode S-DATA indication      */
    /*   (because their are no tag and length fields and the deco-    */
    /*   ding function does not really "decode" the SS-user data).    */
    /*----------------------------------------------------------------*/

    lStoreWhere = ro0076_stwhere (*pStore);  /*get the current position */
    if (lStoreWhere < 0)
      RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

    if (lStoreWhere != lStoreLength       /*lStoreLength was computed above */
    &&  uInEvent    != RC0802_SCONNECTind /*ignore CPC-type values          */
    &&  (  uInEvent != RC080d_SDATAind    /*ignore length difference if     */
        || pContext->bNormalMode          /*X410-mode S-DATA indication     */
        )
    )
      RM0003_RETURN (fnctName, signed short int, RCro6l_UnrecognizedPpduParam)
      /*right value S!? */


    /*----------------------------------------------------------------*/
    /* Delete the Store.                                              */
    /*----------------------------------------------------------------*/

    rc = ro0075_deletestore (*pStore);
    if (rc)                             /*rc never > 0 */
      RM0003_RETURN (fnctName, signed short int, -R_PSTERR)

    /*avoid providerAbort() actions regarding *pStore */
    *pStore = 0;                      
  }

  /*---------------------------------------------------------------------*/
  /* If rc > 0 (i.e. soft error): No mapping of return values necessary. */
  /* (Note: deletestore() and clearPdvTree() will be called when         */
  /*  P-PABORT is issued.                                                */
  /*  This has the advantage that there is no danger of lost of          */
  /*  the original return value.)                                        */
  /*---------------------------------------------------------------------*/

  else

    /*----------------------------------------------------------------*/
    /* The MAVROS created functions put their error code into the     */
    /* Store Control Block --> Necessary:                             */
    /* Reset the Store error code to avoid error at Store deletion    */
    /* (deletion will be done when P-PABORT will be issued).          */
    /* The return value (old MAVROS error code) is ignored. P!?       */
    /*----------------------------------------------------------------*/

    rc1 = ro0080_seterror (*pStore, OK);

  RM0003_RETURN (fnctName, signed short int, rc)
}/*end*/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0432_clearUserData                                */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0432_clearUserData (
	rT0417_stUserData  FAR *pUserData)     

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/

RM0001_STRG (fnctName, "ro0432_clearUserData")
signed         int  iInx;            /*index in "for"-loop */
signed         int  iErrCount;       /*error counter */


/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

  RM0002_ENTRY (fnctName)

  /*------------------------------------------------------------------*/
  /* Init error counter.                                              */
  /* Note: The function continues in the case of an error.            */
  /*       (Is this a right behaviour P!?)                            */
  /*------------------------------------------------------------------*/

  iErrCount = 0;


  /*------------------------------------------------------------------*/
  /* Simple encoding.                                                 */
  /*------------------------------------------------------------------*/

  if (pUserData->lSimplyFullyChoice == RC0417_SimplyEncoded)
  {
    if (ro0246_clearPdvTree (&pUserData->unUserData.hSimply))
      iErrCount++;
  }


  /*------------------------------------------------------------------*/
  /* Full encoding.                                                   */
  /*------------------------------------------------------------------*/

  else
    for (
    iInx = 0;
    iInx < RCro0a_MaxPdvList;     /*better pUserData->stFully.ulPdvList P!? */
    iInx++)
      if (ro0246_clearPdvTree (&pUserData->unUserData.stFully.
                               vPdvList [iInx]))
        iErrCount++;


  /*------------------------------------------------------------------*/
  /* Evaluate the error counter.                                      */
  /*------------------------------------------------------------------*/

  if (iErrCount)
    RM0003_RETURN (fnctName, signed short int, -R_PCLPDV)
  else
    RM0003_RETURN (fnctName, signed short int, OK)
}/*end*/


/*EOF */

