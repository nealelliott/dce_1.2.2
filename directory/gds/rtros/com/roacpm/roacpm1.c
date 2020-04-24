/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roacpm1.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:07:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:26  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:47:25  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:39  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:38  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:44:44  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:22  keutel
 * 	creation
 * 	[1994/03/21  13:14:27  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roacpm1.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:15 $" } ;
#endif

/* *************************************************************************/
/*                                                                         */
/*  * COPYRIGHT  (C)                                                       */
/*  * Siemens Nixdorf Informationssysteme AG 1992                          */
/*  * All rights reserved                                                  */
/*  *                                                                      */
/*  * This software is furnished under licence and may be used only        */
/*  * in accordance with the terms of that licence and with the            */
/*  * inclusion of the above copyright notice.                             */
/*  * This software may not be provided or otherwise made available        */
/*  * to, or used by, any other person. No title to or ownership of        */
/*  * the software is hereby  transferred.                                 */
/*                                                                         */
/* *************************************************************************/

/*daton ********************************************************************/
/*                                                                         */
/* TYPE         : MODULE                                                   */
/*                                                                         */
/* NAME         : roacpm1.c                                                */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-01-22                                                 */
/*                                                                         */
/* COMPONENT    : ACSE help functions                                      */
/*                                                                         */
/* PRD#/VERS.   : RTS-V2.00                                                */
/*                                                                         */
/* DESCRIPTION  : This modul contains functions which helps for the        */
/*                sending and receiving of Application Title and           */
/*                Application Context Name                                 */
/*                                                                         */
/* SYSTEM DEPENDENCE :  none                                               */
/*                                                                         */
/* HISTORY      :                                                          */
/*                                                                         */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# EM#      */
/* 2.00 K0  | 92-01-22|  original                      |mr  |              */
/*                                                                         */
/*datoff *******************************************************************/

/* mr?: 1. die fct fur aptitle muss noch ausgefullt werden
	2. copyAplTitle muss fur called und calling geschrieben werden
	3. copyInvocId muss noch geandert werden auf lock aplCxt
	4. die includes uberprufen ob einige nicht notwendig sind
	5. FAR und NEAR einfuegen
*/

/***************************************************************************/
/*                     Module Identification                               */
/***************************************************************************/

static char *sccsid =    "@(#) roacpm1.c 1.1 93/11/29   RTS-V2.00";


/***************************************************************************/
/*              I N C L U D E S                                            */
/***************************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>     /* general definitions                            */
#include <roerval.h>     /* error values                                   */

#include <rouply0.h>     /* upper layer common eventing def.               */
#include <rouply1.h>     /* upper layer common eventing def.               */

#include <routil0.h>     /* utilities interface                            */
#include <routil1.h>     /* utilities interface                            */

#include <rodpat0.h>     /* dispatcher eventing function interf.           */

#include <rouctr4.h>     /*     application entity manager                 */

#include <roacpm0.h>     /* ac-pm eventing interface                       */
#include <roacpm1.h>     /* ac-pm protocol interface                       */

#include <roacpm01.h>    /*     ac-pm prototypes for help functions        */


/***************************************************************************/
/*                                                                         */
/*              L O C A L  V A R I A B L E S                               */
/*                                                                         */
/***************************************************************************/

/* ****************** Logging function name strings ***************** */
/* ******************                               ***************** */

RM0001_STRG  (rs0650, "ro0650_copyAplCxtName    ")
RM0001_STRG  (rs0651, "ro0651_cmpAndRetrieveAplCxtName")

RM0001_STRG  (rs0652, "ro0652_prpCpyTtlQul      ")
RM0001_STRG  (rs0653, "ro0653_retrieveApTitle   ")

RM0001_STRG  (rs0654, "ro0654_copyInvocId       ")
RM0001_STRG  (rs0655, "ro0655_cmpInvocId   ")

RM0001_STRG  (rs0656, "ro0656_copyAeTitle       ")
RM0001_STRG  (rs0657, "ro0657_cmpAeTitle        ")

RM0001_STRG  (rs0658, "ro0658_CheckAplCxtIDs    ")
RM0001_STRG  (rs0659, "ro0659_retriveRDN    ")
RM0001_STRG  (rs065a, "ro065a_retriveAVA    ")

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0650_copyAplCxtName                                    */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-01-92                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int FAR PASCAL ro0650_copyAplCxtName ( 
       signed short int        sAplId,
       rTro01_pfvObjectId      vApliCxtName)


/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      This function copys the application context name from the local    */
/*      application entity invocation into the protocol control            */
/*      information of ACSE-PM (a-pci).                                    */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                    signed short int  sAplId;                            */
/* OUT-PARAMETERS :                                                        */
/*                    rTro00_vObjectId  vApliCxtName;                      */
/* RETURN VALUES  :                                                        */
/*                                                                         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int         rc;

unsigned long  int         hApl;
rT0501_AEInvocation  NEAR *pApl;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY  (rs0650)
rc = 0;

rc = ro0215_getEiCxt ( RC0233_ApplicationEiCxt,    /* refers the Apl. Context */
		       &hApl             );   /* returns the handle */
if (! rc) {
    pApl = (rT0501_AEInvocation NEAR *) ro0031_locallock ( hApl);
    if (! pApl)                                              rc = -R_LLOCKE;
    else { if ( (sAplId < 0                      )||
		(sAplId > RC0570_MaxApplCtxtInfos)||
		(sAplId > pApl->sNumbOfApplCtxts )  )        rc = -R_AAPLID;
	   else {
	     rc = ro0002_objcpy(
                           &(vApliCxtName[0]),
			   &(pApl->vApplCtxtInfos[sAplId].oApplCtxtName[0]));

	     if ( rc > 0) rc = -R_AAPLID;
	   }
           if (!rc)
	       if (ro0032_localunlock ( hApl)) rc = -R_LULOKE;
    }
} else if ( rc>0)                                            rc = -R_AAPLCX;

RM0003_RETURN (rs0650, signed short int, rc )
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0651_cmpAndRetrieveAplCxtName                          */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-01-92                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int FAR PASCAL ro0651_cmpAndRetrieveAplCxtName (
       signed short int    FAR * sAplId      ,
       rTro01_pfvObjectId        vApliCxtName)

/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      This function retrieves the application context name from the      */
/*      protocol control information of ACSE-PM (a-pci) into the local     */
/*      application entity invocation.                                     */
/*                                                                         */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                    rTro00_vObjectId  vApliCxtName;                      */
/* OUT-PARAMETERS :                                                        */
/*                    signed short int FAR *sAplId;                        */
/* RETURN VALUES  :                                                        */
/*                    < 0: error                                           */
/*                    = 0: OK                                              */
/*                    = 1: no Aplication context name founded              */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed short int		rc;
unsigned long  int	      hApl;
rT0501_AEInvocation NEAR     *pApl;
unsigned long int   NEAR  *pAplObj;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY  (rs0651)

rc = ro0215_getEiCxt ( RC0233_ApplicationEiCxt, &hApl);

if (! rc) {
       pApl = (rT0501_AEInvocation NEAR *) ro0031_locallock ( hApl);
       if (pApl) {
	      /* -------- search for the apl cxt name ------------------------ */
	      rc = 1;

	      for (*sAplId = 0; *sAplId < pApl->sNumbOfApplCtxts; (*sAplId)++) {
		  pAplObj = pApl->vApplCtxtInfos[*sAplId].oApplCtxtName;

		  if (! ro0003_objcmp(vApliCxtName, pAplObj)) { rc = 0; break; }
		  }

	      if (ro0032_localunlock (hApl)) rc =  -R_LULOKE;
       } else {
	      rc = -R_LLOCKE;
       }
} else {
       if (rc > 0)  rc = -R_AAPLCX;
}

RM0003_RETURN (rs0651, signed short int, rc )
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0652_prpCpyTtlQul                                      */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-01-92                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int FAR PASCAL ro0652_prpCpyTtlQul ( 
       signed short int         mode     ,
       unsigned long int   FAR *phSource ,
       signed long int     FAR *pbApTitle,
       signed long int     FAR *plApTitle,
       rT0635_uApTitle     FAR *puApTitle,
       signed long int     FAR *pbAeQuali,
       signed long int     FAR *plAeQuali,
       rT0637_uAeQualifier FAR *puAeQuali)

/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      This function prepares the application title for copying.          */
/*      The mode parameter decides which application title it is.          */
/*       called     = RC0690_Called                                        */
/*       calling    = RC0691_Calling                                       */
/*       responding = RC0692_Respond                                       */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                  signed short int      mode                             */
/*                  unsigned long int    *phSource; if called and          */
/*                                                        responding       */
/*                  unsigned long int    *NULL      ; if calling           */
/* OUT-PARAMETERS :                                                        */
/*                  signed long int      *pbApTitle                        */
/*                  signed long int      *plApTitle                        */
/*                  rT0635_uApTitle      *puApTitle                        */
/*                  signed long int      *pbAeQuali                        */
/*                  signed long int      *plAeQuali                        */
/*                  rT0637_uAeQualifier  *puAeQuali                        */
/*                                                                         */
/* RETURN VALUES  :                                                        */
/*                                                                         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int        rc;
unsigned long  int        hAeInv;
rT0501_AEInvocation NEAR *pAeInv;
rTro72_unAeTitle    NEAR *pAeTitle;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY  (rs0652)
rc = 0;

/* -------------- decide called, calling or responding ------------------- */
switch (mode) {
case RC0690_Called :
case RC0692_Respond:

  if (! *phSource) {
     /* --------no AeTitle because of no handle -------------------- */
     *pbApTitle = FALSE;
     *pbAeQuali = FALSE;
  } else {
     *pbApTitle = TRUE;
     /* --------- lock the AeTitle ----------------------------- */
     pAeTitle = (rTro72_unAeTitle NEAR *)
		 ro0031_locallock ( *phSource);
     if (! pAeTitle) {  RM0003_RETURN (rs0650, signed short int, -R_LLOCKE)
     }
     /* -------- copy into the pdu presentation -------------------- */
     rc = ro0656_copyAeTitle ( pAeTitle ,   /* IN  */
			       pbApTitle,   /* OUT */
			       plApTitle,
			       puApTitle,
			       pbAeQuali,
			       plAeQuali,
			       puAeQuali );
     /* -------- unlock the locks ---------------------------------- */
     if (!rc) {
         if      (ro0032_localunlock ( *phSource)) rc = -R_LULOKE;
         else if (ro0034_localfree   ( *phSource)) rc = -R_LFREEE;
     }
  }/* end of if (! *phSource ) */
  break;

case RC0691_Calling:
  /* --------- get the own AEInvocation ---------------------------------- */
  rc = ro0215_getEiCxt (RC0233_ApplicationEiCxt,  /* refers the Apl. Context */
			&hAeInv            );/* returns the handle      */
  if (! rc) {
      /* --------- lock the AEInvocation --------------------------------- */
      pAeInv = (rT0501_AEInvocation NEAR *) ro0031_locallock ( hAeInv);
      if (! pAeInv)                                        rc = -R_LLOCKE;
      else {
	if (! pAeInv->stOwnAeTitle.hHanOfSub[0]) {
	   /* --------no AeTitle because of no handle -------------------- */
	   *pbApTitle = FALSE;
	   *pbAeQuali = FALSE;
	} else {
	   *pbApTitle = TRUE;
	   /* --------- lock the Own AeTitle ----------------------------- */
	   pAeTitle = (rTro72_unAeTitle NEAR *)
		       ro0031_locallock (pAeInv->stOwnAeTitle.hHanOfSub[0]);
	   if (! pAeTitle)
	        RM0003_RETURN (rs0650, signed short int, -R_LLOCKE)

	   /* -------- copy into the pdu presentation -------------------- */
	   rc = ro0656_copyAeTitle ( pAeTitle ,   /* IN  */
				     pbApTitle,   /* OUT */
				     plApTitle,
				     puApTitle,
				     pbAeQuali,
				     plAeQuali,
				     puAeQuali );
	   /* -------- unlock the locks ---------------------------------- */
           if (!rc)
	       if (ro0032_localunlock (pAeInv->stOwnAeTitle.hHanOfSub[0]))
		   rc = -R_LULOKE;
	}/* end of if (! pAeInv->stOwnAeTitle.hHanOfSub[0]) */
        if (!rc)
	    if (ro0032_localunlock (hAeInv)) rc = -R_LULOKE;
      }
  } else if ( rc>0)                                        rc = -R_AAPLCX;
  break;

default            :  break;
}
RM0003_RETURN (rs0652, signed short int, rc )
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0653_retrieveApTitle                                   */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-01-92                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int FAR PASCAL ro0653_retrieveApTitle( 
       unsigned long int   FAR *phAplTitle,
       signed long int          bApTitle  ,
       signed long int          lApTitle  ,
       rT0635_uApTitle     FAR *puApTitle ,
       signed long int          bAeQuali  ,
       signed long int          lAeQuali  ,
       rT0637_uAeQualifier FAR *puAeQuali )

/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      This function retrieves the application title from the             */
/*      protocol control information of ACSE-PM (a-pci)                    */
/*      into the local service parameter.                                  */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                  signed long int        bApTitle;                       */
/*                  signed long int        lApTitle;                       */
/*                  rT0635_uApTitle       *puApTitle;                      */
/*                  signed long int        bAeQuali;                       */
/*                  signed long int        lAeQuali;                       */
/*                  rT0637_uAeQualifier   *puAeQuali;                      */
/*                                                                         */
/* OUT-PARAMETERS :                                                        */
/*                  unsigned long int    *phAplTitle;                      */
/* RETURN VALUES  :                                                        */
/*                                                                         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int       rc;
unsigned short int       i;
unsigned long  int       hTarget;
rTro72_unAeTitle   NEAR *pTarget;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY  (rs0653)
rc      = 0;
hTarget = 0;

if (! bApTitle && (! bAeQuali)) {
   /* --------no ApTitle, no AeQualifier furthermore no handle --------- */
   *phAplTitle = NULL;
} else {

   /* mr? Theoretically it is possible that ApTitle and AeQuali are
	  from different types like form1 and form2.
	  In this case the intern presentation isn't able to handle this
	  because of only one form identifier (lAeTitleForm).
	  Actual there are a PDU-Error is given.                         */

   if ( bApTitle && bAeQuali) {
       if ( lApTitle != lAeQuali)
			     RM0003_RETURN (rs0653, signed short int, 1)
   }

   /* ---------- RCro77_ObjId ------------------------------------------ */
   if ( (lApTitle == RCro77_ObjId) || (lAeQuali == RCro77_ObjId)) {

      /* ------- allock the target ------------------------------------- */
      hTarget = ro0030_localalloc ( sizeof ( rTro71_stAeTitleForm2));
      pTarget = (hTarget) ? (rTro72_unAeTitle NEAR *)
				  ro0031_locallock (hTarget)
			  : 0;
      rc      = (pTarget) ? 0 : -R_LMEMRY;
      if (rc)
          RM0003_RETURN (rs0653, signed short int, rc)

      /* ---- copy from pdu to intern presentation --------------------- */
      /* --------------------------------------------------------------- */

      pTarget->lAeTitleForm = RCro77_ObjId;


      if ( bApTitle) {
	  /* ------ Application Title -----------------------------------*/

	  rc = ro0002_objcpy (pTarget->stAeTitleForm2.oApTitleObjId,
			      puApTitle->vApTitleForm2              );

	  /* rc>0 means that not defined, no objid */
	  if ( rc>0) rc=0;
      }

      if ( bAeQuali) {
	 /*-------- Ae Qualifier --------------------------------------- */

	  pTarget->stAeTitleForm2.bAeQualInt = TRUE;
	  pTarget->stAeTitleForm2.lAeQualInt = puAeQuali->lAeQualifierForm2;
      } else {
	  pTarget->stAeTitleForm2.bAeQualInt = FALSE;
      }
      /* ------- unlock of target -------------------------------------- */
      if ( ro0032_localunlock ( hTarget)) {
		       RM0003_RETURN (rs0653, signed short int, -R_LULOKE)
      }
      *phAplTitle= hTarget;
   }

   /* ---------- RCro78_RelDisName ------------------------------------- */
   if ( (lApTitle==RCro78_RelDisName) || (lAeQuali==RCro78_RelDisName)) {

      /* -mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr */

      /* mr? only for actual implementation the Rel. Dis. Name isn't
	    implemented. Up to here Rel.Dis.Name is used as NoAeTitle    */

      /*   *phAplTitle = NULL; */
     
      /* -mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr-mr */
      /* Implemented by kc on 13-10-93                                   */
      /* ------- allock the target ------------------------------------- */
      hTarget = ro0030_localalloc ( sizeof ( rTro70_stAeTitleForm1));
      pTarget = (hTarget) ? (rTro72_unAeTitle NEAR *)
				  ro0031_locallock (hTarget)
			  : 0;
      rc      = (pTarget) ? 0 : -R_LMEMRY;
      if (rc)
          RM0003_RETURN (rs0653, signed short int, rc)
      /* ---- copy from pdu to intern presentation --------------------- */
      /* --------------------------------------------------------------- */

      pTarget->lAeTitleForm = RCro78_RelDisName;


      if ( bApTitle) {
	  /* ------ Application Title -----------------------------------*/
         pTarget->stAeTitleForm1.lApTitle =  
           puApTitle->stApTitleForm1.uName.stRDNSequence.lRDNSequence;
         for (i=0;
              i<puApTitle->stApTitleForm1.uName.stRDNSequence.lRDNSequence; 
              i++){
            rc = ro0659_retriveRDN(
                  &(pTarget->stAeTitleForm1.vApTitle[i]),
             &(puApTitle->stApTitleForm1.uName.stRDNSequence.stRDNSequence[i]));
            if(rc)break;
          }
      }
      if ( bAeQuali) {
	 /*-------- Ae Qualifier --------------------------------------- */
        pTarget->stAeTitleForm1.lAeQuali = puAeQuali->stAeQualifierForm1.lRDN;
        for(i=0;i<puAeQuali->stAeQualifierForm1.lRDN;i++){
          rc=ro065a_retriveAVA(
                   &(pTarget->stAeTitleForm1.vAeQuali[i]),
                   &(puAeQuali->stAeQualifierForm1.stRDN[i]));
          if(rc)break;
        }   
      } 
      /* ------- unlock of target -------------------------------------- */
      if ( ro0032_localunlock ( hTarget)) {
		       RM0003_RETURN (rs0653, signed short int, -R_LULOKE)
      }
      *phAplTitle= hTarget;

     /* end of the Implementation by kc on 13-10-93 */
   }

}/* end of if (! bApTitle && (! bAeQuali))  */

RM0003_RETURN (rs0653, signed short int, rc )
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0654_copyInvocId                                       */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 22-01-92                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int FAR PASCAL ro0654_copyInvocId (
	signed long int   FAR   *bApInvocId,
	signed long int   FAR   *lApInvocId,
	signed long int   FAR   *bAeInvocId,
	signed long int   FAR   *lAeInvocId )

/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      This function copies the calling application invocation id. into   */
/*      the protocol control information of ACSE-PM (a-pci).               */
/*                                                                         */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                                                                         */
/* OUT-PARAMETERS :                                                        */
/*                  signed long int         *bApInvocId;                   */
/*                  signed long int         *lApInvocId;                   */
/*                  signed long int         *bAeInvocId;                   */
/*                  signed long int         *lAeInvocId;                   */
/* RETURN VALUES  :                                                        */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int         rc;
unsigned long  int        hAeInv;
rT0501_AEInvocation NEAR *pAeInv;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY  (rs0654)
rc = 0;

  /* --------- get the own AEInvocation ---------------------------------- */
  rc = ro0215_getEiCxt (RC0233_ApplicationEiCxt,  /* refers the Apl. Context */
			&hAeInv            );/* returns the handle      */
  if (! rc) {
      /* --------- lock the AEInvocation --------------------------------- */
      pAeInv = (rT0501_AEInvocation NEAR *) ro0031_locallock ( hAeInv);
      if (! pAeInv)                                        rc = -R_LLOCKE;
      else {
	 *bApInvocId = pAeInv->stOwnInvoc.bApInvocId;
	 *lApInvocId = pAeInv->stOwnInvoc.lApInvocId;
	 *bAeInvocId = pAeInv->stOwnInvoc.bAeInvocId;
	 *lAeInvocId = pAeInv->stOwnInvoc.lAeInvocId;

	 if (pAeInv)
             if (ro0032_localunlock (hAeInv)) rc = -R_LULOKE;
      }
  }
RM0003_RETURN (rs0654, signed short int, rc )
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0655_cmpInvocId                                        */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 22-01-92                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int FAR PASCAL ro0655_cmpInvocId (
	signed   long int         bApInvocId,
	signed   long int         lApInvocId,
	signed   long int         bAeInvocId,
	signed   long int         lAeInvocId,
	unsigned long int FAR *  pulReason  )
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      This function checks the called  application invocation id. with   */
/*      the applicaton entity invocation.                                  */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                  signed long int         bApInvocId;                    */
/*                  signed long int         lApInvocId;                    */
/*                  signed long int         bAeInvocId;                    */
/*                  signed long int         lAeInvocId;                    */
/*                  unsigned long int FAR * pulReason ;                    */
/*                                                                         */
/* OUT-PARAMETERS :                                                        */
/*                                                                         */
/* RETURN VALUES  :                                                        */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int         rc;
unsigned long  int        hAeInv;
rT0501_AEInvocation NEAR *pAeInv;

rTro74_stInvoc            stInvoc2;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY  (rs0655)
rc = 0;

  /* --------- get the own AEInvocation ---------------------------------- */
  rc = ro0215_getEiCxt (RC0233_ApplicationEiCxt,  /* refers the Apl. Context */
			&hAeInv            );/* returns the handle      */
  if (! rc) {
      /* --------- lock the AEInvocation --------------------------------- */
      pAeInv = (rT0501_AEInvocation *) ro0031_locallock ( hAeInv);
      if (! pAeInv)                                        rc = -R_LLOCKE;
      else {
	 stInvoc2.bApInvocId = bApInvocId;
	 stInvoc2.lApInvocId = lApInvocId;
	 stInvoc2.bAeInvocId = bAeInvocId;
	 stInvoc2.lAeInvocId = lAeInvocId;

	 rc = ro0004_cmpApplInvocation (
			      (rTro74_stInvoc FAR *) &(pAeInv->stOwnInvoc),
			      (rTro74_stInvoc FAR *) &stInvoc2          );
	 if (rc > 0) {
	    switch (rc) {
	    case  1: *pulReason = RC062d_UnrecCldApInvocId; rc = 0; break;
	    case  2: *pulReason = RC062f_UnrecCldAeInvocId; rc = 0; break;
	    default: *pulReason = RC0626_NoReason         ; rc = 0; break;
	    }
	 }
         if (!rc && pAeInv)
             if (ro0032_localunlock (hAeInv)) rc = -R_LULOKE;
      }
  }
RM0003_RETURN (rs0655, signed short int, rc )
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0656_copyAeTitle                                       */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-04-27                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int FAR PASCAL ro0656_copyAeTitle (
                    rTro72_unAeTitle    NEAR *pSource  ,   /* IN  */
		    signed long int     FAR  *pbApTitle,   /* OUT */
		    signed long int     FAR  *plApTitle,
		    rT0635_uApTitle     FAR  *puApTitle,
		    signed long int     FAR  *pbAeQuali,
		    signed long int     FAR  *plAeQuali,
		    rT0637_uAeQualifier FAR  *puAeQuali )


/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                 Copies the application title from the intern            */
/*                 presentation into the pdu presentation.                 */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                  rTro72_unAeTitle NEAR *pSource                         */
/*                                                                         */
/* OUT-PARAMETERS :                                                        */
/*                  signed long int      *plApTitle                        */
/*                  rT0635_uApTitle      *puApTitle                        */
/*                  signed long int      *pbAeQuali                        */
/*                  signed long int      *plAeQuali                        */
/*                  rT0637_uAeQualifier  *puAeQuali                        */
/*                                                                         */
/* RETURN VALUES  :                                                        */
/*                                                                         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed short int rc,i;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */
RM0002_ENTRY  (rs0656)
rc = 0;

if ((pSource->lAeTitleForm & 0x00ff)== RCro77_ObjId) {

   /* ------ RCro77_ObjId ------------------------------------------------ */

   rc = ro0002_objcpy (puApTitle->vApTitleForm2              ,
		       pSource->stAeTitleForm2.oApTitleObjId  );
   if ( rc<0)               RM0003_RETURN (rs0656, signed short int, rc )

   /* rc>0 means that not defined, no objid */
   if ( rc>0) {
       rc=0;
       *pbApTitle = FALSE;
   } else {
       *pbApTitle = TRUE;
       *plApTitle = RCro77_ObjId;
   }

   if ( pSource->stAeTitleForm2.bAeQualInt) {
       *pbAeQuali                    = TRUE;
       *plAeQuali                    = RCro77_ObjId;
	puAeQuali->lAeQualifierForm2 = pSource->stAeTitleForm2.lAeQualInt;
   } else {
       *pbAeQuali                    = FALSE;
   }
} else if((pSource->lAeTitleForm & 0x00ff) == RCro78_RelDisName) {
  /* ---------- RCro78_RelDisName ---------------------------------------- */

  /* mr? */
  /* implemented by kc on 13-08-93 */
  *pbApTitle = TRUE;
  *plApTitle = RCro78_RelDisName;
  puApTitle->stApTitleForm1.lName = 1; /* at present noother fot CHOICE */ 
  puApTitle->stApTitleForm1.uName.stRDNSequence.lRDNSequence =
    pSource->stAeTitleForm1.lApTitle;
  for(i=0;
      i<pSource->stAeTitleForm1.lApTitle;
      i++){
    rc= ro065b_CpyRDN(
       &(puApTitle->stApTitleForm1.uName.stRDNSequence.stRDNSequence[i]),
       &(pSource->stAeTitleForm1.vApTitle[i]));
   if(rc) break;

  }  
  *pbAeQuali = TRUE;
  *plAeQuali = RCro78_RelDisName;
  puAeQuali->stAeQualifierForm1.lRDN = (unsigned long int)
         pSource->stAeTitleForm1.lAeQuali;
  for (i=0;
       i<pSource->stAeTitleForm1.lAeQuali;
       i++){
     rc=ro065c_CpyAVA(&(puAeQuali->stAeQualifierForm1.stRDN[i]),
                      &(pSource->stAeTitleForm1.vAeQuali[i]));
     if (rc) break;
  }  
} else {
  *pbApTitle = FALSE;
  *pbAeQuali = FALSE;
}
  
RM0003_RETURN (rs0656, signed short int, rc )
}/*end*/

/*exon *********************************************************************/
/*                                                                         */
/* TYPE         : C-FUNKTION                                               */
/*                                                                         */
/* NAME         : ro0657_cmpAeTitle                                        */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-04-28                                                 */
/*                                                                         */
/* SYNTAX       :                                                          */

signed short int FAR PASCAL ro0657_cmpAeTitle (
		   unsigned long int        hAeTitle1,
		   unsigned long int  FAR * pulReason  )

/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/*      This function compares the application title from the local        */
/*      service parameter with the application title from the              */
/*      AEInvocation. A reason is given back in pulReason.                 */
/*                                                                         */
/*                                                                         */
/* IN-PARAMETERS:                                                          */
/*                   hAeTitle1                                             */
/*                                                                         */
/* OUT-PARAMETERS :                                                        */
/*                   pulReason                                             */
/*                        RC0626_NoReason                                  */
/*                        RC062c_UnrecCldApTitle                           */
/*                        RC062e_UnrecCldAeQual                            */
/*                        RC0626_NoReason                                  */
/*                                                                         */
/* RETURN VALUES  :                                                        */
/*                                                                         */
/*                                                                         */
/* GLOBAL DATA   ( read only ):  none                                      */
/*                                                                         */
/* GLOBAL DATA   ( modified ) :  none                                      */
/*                                                                         */
/* REMARKS                    :  none                                      */
/*                                                                         */
/*inoff ********************************************************************/

/* ****************** l o c a l    v a r i a b l e s ********************* */
/* ******************                                ********************* */
{/*entr*/

signed   short int        rc;
unsigned long  int        hAeInv;
rT0501_AEInvocation NEAR *pAeInv;

/* ****************** S T A R T ****************************************** */
/* ******************           ****************************************** */

RM0002_ENTRY  (rs0657)
rc = 0;

  /* --------- get the own AEInvocation ---------------------------------- */
  rc = ro0215_getEiCxt (RC0233_ApplicationEiCxt,  /* refers the Apl. Context */
			&hAeInv            );/* returns the handle      */
  if (! rc) {
      /* --------- lock the AEInvocation --------------------------------- */
      pAeInv = (rT0501_AEInvocation NEAR *) ro0031_locallock ( hAeInv);
      if (! pAeInv)                                        rc = -R_LLOCKE;
      else {
	/* --------- compare the AeTitle --------------------------------- */
	rc = ro0006_cmpAETitle ( pAeInv->stOwnAeTitle.hHanOfSub[0],
				 hAeTitle1                         );
	if (rc > 0) {  /* different AETitles*/
	   switch ( rc) {
	   case  1:   *pulReason = RC0626_NoReason       ; rc = 0; break;
	   case  2:   *pulReason = RC062c_UnrecCldApTitle; rc = 0; break;
	   case  3:   *pulReason = RC062e_UnrecCldAeQual ; rc = 0; break;
	   default:   *pulReason = RC0626_NoReason       ; rc = 0; break;
	   }
	}
	/* ------- unlock the AEInvocation ------------------------------- */
	if (!rc)
            if (ro0032_localunlock (hAeInv)) rc = -R_LULOKE;
      }
  } else if ( rc>0)                                        rc = -R_AAPLCX;
RM0003_RETURN (rs0657, signed short int, rc )
}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0658_CheckAplCxtIDs                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed  short int NEAR PASCAL ro0658_CheckAplCxtIDs(
         signed   short  int       sSourID,
         signed   short  int       sTarID,
         signed   short  int     * pbResult)

/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

signed   short  int       rc = OK;

unsigned long  int         hApl;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0658)
 
rc = ro0215_getEiCxt ( RC0233_ApplicationEiCxt,    /* refers the Apl. Context */
		       &hApl             );   /* returns the handle */
if (rc == OK)
   rc = ro000c_AplCxtCmp(sSourID,sTarID,hApl,pbResult);

RM0003_RETURN (rs0658, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0659_retriveRDN                                   */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-10-13                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed  short int NEAR PASCAL ro0659_retriveRDN(
         rTro69_stRDN  FAR  *      pLocRDN,
         rT0631_stRDN  FAR  *      pPduRDN)

/*                                                                    */
/* DESCRIPTION  : copies the RDN from ACPM-PDU to local (roulpy0.h)   */
/*  structure.                                                        */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

signed   short  int       rc = OK;
unsigned short  int       i  = NULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0659)

 pLocRDN->lRDN = (signed long int)pPduRDN->lRDN;
 for (i=0;i<pPduRDN->lRDN;i++){
    rc = ro065a_retriveAVA(
                           &(pLocRDN->vRDN[i]),
                           &(pPduRDN->stRDN[i]));
    if (rc) break;
 } 

RM0003_RETURN (rs0659, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro065a_retriveAVA                                   */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-10-13                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed  short int NEAR PASCAL ro065a_retriveAVA(
         rTro68_stAVA                  FAR  *pLocAVA,
         rT0630_stAttriValueAssertion  FAR  *pPduAVA)

/*                                                                    */
/* DESCRIPTION  : copies the AVA from ACPM-PDU to local (roulpy0.h)   */
/*  structure.                                                        */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

signed   short  int       rc = OK;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs065a)

 rc = ro0002_objcpy (pLocAVA->vAVA_Type,
			      pPduAVA->vAttributeType);

 /* rc>0 means that not defined, no objid */
 if ( rc>0) rc=0;
 pLocAVA->lAVA_Value = (signed long int)pPduAVA->lAttributeType; 
 ro0041_memcpy(pLocAVA->vAVA_Value,pPduAVA->vAttributeValue,
                pPduAVA->lAttributeType);


RM0003_RETURN (rs065a, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro065b_CpyRDN                                       */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-10-13                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed  short int NEAR PASCAL ro065b_CpyRDN    (
         rT0631_stRDN  FAR  *      pPduRDN,
         rTro69_stRDN  FAR  *      pLocRDN)

/*                                                                    */
/* DESCRIPTION  : copies the RDN from local(rouply0.h) to ACPM-PDU    */
/*  structure.                                                        */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

signed   short  int       rc = OK;
signed   short  int       i  = NULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs065b)

 pPduRDN->lRDN = (unsigned long int)pLocRDN->lRDN;
 for (i=0;i<pLocRDN->lRDN;i++){
    rc = ro065c_CpyAVA(&(pPduRDN->stRDN[i]),
                           &(pLocRDN->vRDN[i]));
    if (rc) break;
 } 

RM0003_RETURN (rs065b, signed short int, rc)                                    
}/*end*/                                                                        

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro065c_CpyAVA                                       */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-10-13                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed  short int NEAR PASCAL ro065c_CpyAVA    (
         rT0630_stAttriValueAssertion  FAR  *pPduAVA,
         rTro68_stAVA                  FAR  *pLocAVA)

/*                                                                    */
/* DESCRIPTION  : copies the AVA from  local (roulpy0.h)  to ACPM-PDU */
/*  structure.                                                        */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :   >= 0  Success                       */
/*                                <  0  Error                         */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

signed   short  int       rc = OK;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs065c)

 rc = ro0002_objcpy (pPduAVA->vAttributeType,
			      pLocAVA->vAVA_Type);

 /* rc>0 means that not defined, no objid */
 if ( rc>0) rc=0;
 pPduAVA->lAttributeType = (unsigned long int)pLocAVA->lAVA_Value; 
 ro0041_memcpy( pPduAVA->vAttributeValue,
                pLocAVA->vAVA_Value,
                pLocAVA->lAVA_Value);


RM0003_RETURN (rs065c, signed short int, rc)                                    
}/*end*/                                                                        

/* EOF */
