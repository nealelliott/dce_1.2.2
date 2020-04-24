/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropm2.c,v $
 * Revision 1.1.4.2  1996/03/11  13:22:58  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:23:09  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:12:31  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:02  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:16  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:06:06  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:49  keutel
 * 	creation
 * 	[1994/03/21  13:34:57  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: roropm2.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:58 $" } ;
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
/* NAME         : roropm2.c                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-01                                            */
/*                                                                    */
/* COMPONENT    : Remote Opeartions service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Automata Functions for RO-PM Only(Table A.6.        */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropm2.c	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-04-01|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/* ****************************************************************** */
/*                  I n c l u d e  F i l e s                          */
/* ****************************************************************** */

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>      /* Machine dependent generation             */
#include <roerval.h>      /* Unrecoverable error codes                */
#include <rolgval.h>      /* Logging constants                        */
#include <rouply0.h>      /* Common type and value definitions        */
#include <rouply1.h>      /* PM service interface structures          */
#include <routil0.h>      /* Utility interface (common functions)     */
#include <routil1.h>      /* Utility interface (object id. functions) */
#include <rodpat0.h>      /* Dispatcher interface                     */

#include  <rortpm0.h>
#include <roacpm0.h>

#include <roppm00.h>


#include <roropm0.h>      /* Interface definitions                    */
#include <roropm1.h>      /* PDU data structures                      */
#include <roropm3.h>      /* Local Events & Data structures           */
#include <roropm4.h>      /* Automata Function proto TYpe             */
/** Not needed Baiju , GNU GCC 
#include <roropm5.h>      =* Static Automata Table                    *=
**/
#include <roropm6.h>      /* Internally used  function prototypes.    */


/* ****************** Logging function name strings ***************** */
/* ******************                               ***************** */
RM0001_STRG (rs0720, "ro0720_localError  ")
RM0001_STRG (rs0721, "ro0721_STA00xBindRe")
RM0001_STRG (rs0722, "ro0722_STA00xStartE")

RM0001_STRG (rs0723, "ro0723_STA01xBindRs")
RM0001_STRG (rs0724, "ro0724_STA01xUNBIND")
RM0001_STRG (rs0725, "ro0725_STAXXxABORTr")
RM0001_STRG (rs0726, "ro0726_STA10xAA_EST")
RM0001_STRG (rs0727, "ro0727_STA01xEndREL")
RM0001_STRG (rs0728, "ro0728_STAXXxAA_ABi")

RM0001_STRG (rs0729, "ro0729_STA02_UNBIND")
RM0001_STRG (rs072a, "ro072a_STA02xINVreq")
RM0001_STRG (rs072b, "ro072b_STA02xRESreq")
RM0001_STRG (rs072c, "ro072c_STA02xERRreq")
RM0001_STRG (rs072d, "ro072d_STA02xRJUreq")
RM0001_STRG (rs072e, "ro072e_STA02xAA_REL")
RM0001_STRG (rs072f, "ro072f_STA02xROINV ")
RM0001_STRG (rs072g, "ro072g_STA02xRORES ")
RM0001_STRG (rs072h, "ro072h_STA02xROERR ")
RM0001_STRG (rs072i, "ro072i_STA02xRORJu ")
RM0001_STRG (rs072j, "ro072j_STA02xRORJp ")
RM0001_STRG (rs072k, "ro072k_STA02xAPDUua")
/*exon ************************************************************************/
/*                                                                            */
/* TYPE         : C-FUNCTION                                                  */
/*                                                                            */
/* NAME         : ro0720_localError                                           */
/*                                                                            */
/* AUTHOR       : Chandrsekar.K.                                              */
/* DATE         : 93-04-01                                                    */
/*                                                                            */
/* SYNTAX       :                                                             */

signed short int PASCAL ro0720_localError (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *puInEvent,
signed   long  int FAR   *puOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4        )

/* DESCRIPTION :                                                              */
/*                                                                            */
/*     Returns a positive return code. When the running function detects an   */
/*     flow error.                                                            */
/*                                                                            */
/* IN-PARAMETERS :                                                            */
/*                                                                            */
/*     hP1    : Used. Calls cleanUp with this parameter.                      */
/*     hP2    : Currently unused.                                             */
/*     hP3    : Used. Calls cleanUp with this parameter.                      */
/*     hP4    : Currently unused.                                             */
/*                                                                            */
/* OUT-PARAMETERS : none.                                                     */
/*                                                                            */
/* RETURN VALUES :                                                            */
/*                                                                            */
/*      OK                                                                    */
/*                                                                            */
/* GLOBAL DATA (read only) : none                                             */
/*                                                                            */
/* GLOBAL DATA (modified)  :                                                  */
/*                                                                            */
/* REMARKS :  none                                                            */
/*                                                                            */
/*inoff ***********************************************************************/

{/*entr*/

/* *********************** l o c a l    v a r i a b l e s ******************* */
/* ***********************                                ******************* */

RM0001_STRG (pFnctName, "ro0720_localError")

/* **************************** S T A R T *********************************** */
/* ****************************           *********************************** */

RM0002_ENTRY (pFnctName)

/*----------------------------------------------------------------------------*/
/* An abort and clean up will be performed in the running function (because   */
/* of a positive return code).                                                */
/*----------------------------------------------------------------------------*/
pContext->sStateRopm    = RC0731_STAnoPm;
pContext->sStateTrPm    = RC0735_STAnoTr1;
*puOutEvent = NULL;

RM0003_RETURN (pFnctName, signed short int, R_FLSQFL)

}/*end*/


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0721_STA00xBindReq                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0721_STA00xBindReq (
rT0730_stContext   NEAR  *pContext,
signed   long int  FAR   *plInEvent,
signed   long int  FAR   *plOutEvent,
unsigned long  int FAR   *phP1,
unsigned long  int FAR   *phP2,
unsigned long  int FAR   *phP3,
unsigned long  int FAR   *phP4)





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
signed   short int      rc = OK;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0721)

 /* ------------ Check if the service parameters are Present --------- */
 if (phP3){
    if (*phP4 )  /* RTSE Present */
          pContext->sSubAutoNum  = RC073h_RopmPrTr;
    else 
          pContext->sSubAutoNum  = RC073i_RopmRtTr;

    pContext->sStateRopm   = RC0732_STA01;
    pContext->bAssocInit   = TRUE;
    pContext->bmappingInfo = (signed short int)*phP4 ;
    *plOutEvent            = RC073K_AA_StartESTAB;
 }
 else rc = R_NOBHPT;
RM0003_RETURN (rs0721, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0722_STA00xStartESTAB                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


signed short int PASCAL ro0722_STA00xStartESTAB (
rT0730_stContext   NEAR  *pContext,       
signed   long int  FAR   *plInEvent,
signed   long int  FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        



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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0722)
    pContext->sStateRopm   = RC0732_STA01;
    *plOutEvent            = RC0702_RO_BINDind;
    pContext->sSubAutoNum  = RC073j_End;
    rc = RC073k_PushOutEvent;
RM0003_RETURN (rs0722, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0723_STA01xBindRsp                                */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0723_STA01xBindRsp (
rT0730_stContext   NEAR  *pContext,       
signed   long int  FAR   *plInEvent,
signed   long int  FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        




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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0723)
 pContext->sSubAutoNum =  (pContext->bmappingInfo)
                        ? RC073h_RopmPrTr
                        : RC073i_RopmRtTr            ;
 if ( *phP4 & 0x000000f0UL  ){
    pContext->sStateRopm = RC0731_STAnoPm;
 }
 else 
    pContext->sStateRopm = RC0733_STA02;
 
    *plOutEvent            = RC073L_AA_ESTAB;
RM0003_RETURN (rs0723, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0724_STA01xUNBINDrsp                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0724_STA01xUNBINDrsp (
rT0730_stContext   NEAR  *pContext,       
signed   long int  FAR   *plInEvent,
signed   long int  FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        


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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0724)
 if (pContext->sReleaseFg ){
    pContext->sStateRopm =RC0731_STAnoPm ;
 pContext->sSubAutoNum =  (pContext->bmappingInfo)
                        ? RC073h_RopmPrTr
                        : RC073i_RopmRtTr            ;
    *plOutEvent          = RC073N_AA_EndREL;
 }
 else rc = R_ORELER;

RM0003_RETURN (rs0724, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0725_STAXXxABORTreq  			      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0725_STAXXxABORTreq (
rT0730_stContext   NEAR  *pContext,       
signed   long int  FAR   *plInEvent,
signed   long int  FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        


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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0725)
 pContext->sStateRopm = RC0731_STAnoPm;
 *plOutEvent          = RC073v_AA_ABreq;
 pContext->sSubAutoNum =  (pContext->bmappingInfo)
                        ? RC073h_RopmPrTr
                        : RC073i_RopmRtTr            ;
RM0003_RETURN (rs0725, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0726_STA01xAA_ESTAB                               */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0726_STA10xAA_ESTAB (
rT0730_stContext   NEAR  *pContext,       
signed   long int  FAR   *plInEvent,
signed   long int  FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0726)
 if ( *phP4 == RCro80_Accept )
   pContext->sStateRopm  = RC0733_STA02 ;
 else 
   pContext->sStateRopm  = RC0731_STAnoPm ;
 pContext->sSubAutoNum = RC073j_End;
 *plOutEvent           = RC0704_RO_BINDcnf;
 rc                    = RC073k_PushOutEvent;
RM0003_RETURN (rs0726, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0727_STA01xEndREL                                 */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0727_STA01xEndREL (
rT0730_stContext   NEAR  *pContext,       
signed   long int  FAR   *plInEvent,
signed   long int  FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0727)
 if (pContext->sReleaseFg){
     pContext->sStateRopm = RC0731_STAnoPm;
     pContext->sSubAutoNum= RC073j_End ;
     *plOutEvent          = RC0708_RO_UNBINDcnf;
     rc                   = RC073k_PushOutEvent;
 } 
 else rc = R_ORELER;

RM0003_RETURN (rs0727, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0728_STAXXxAA_ABind                               */ 
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL ro0728_STAXXxAA_ABind (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        





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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0728)
     pContext->sStateRopm = RC0731_STAnoPm;
     pContext->sSubAutoNum= RC073j_End ;
/*     *plOutEvent          = RC070a_RO_UABORTind; commented by kc on 5/5 */

     if ( (*plInEvent     == RC060a_AA_ABORTind) )
        *plOutEvent          = RC070a_RO_UABORTind;
     else 
        *plOutEvent          = RC070b_RO_PABORTind; 
     rc                   = RC073k_PushOutEvent;
RM0003_RETURN (rs0728, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro0729_STA02_UNBINDreq                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0729_STA02_UNBINDreq (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs0729)
 pContext->sStateRopm =RC0732_STA01;
 pContext->sReleaseFg = TRUE;
 pContext->sSubAutoNum =  (pContext->bmappingInfo)
                        ? RC073h_RopmPrTr
                        : RC073i_RopmRtTr            ;
 *plOutEvent           = RC073M_AA_REL;
 
RM0003_RETURN (rs0729, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro072a_STA02xINVreq                    	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro072a_STA02xINVreq (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int      rc = OK;
rT070b_stInvReq        *pInvRq = LNULL;

unsigned long  int      hPdv   = LNULL;
rT0715_unRoseApdus FAR *pPdv   = LNULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs072a)
    
  /* ------------------ Lock the P3 handle -------------------------- */
      pInvRq = (rT070b_stInvReq *) ro0031_locallock ( *phP3 );
      if (pInvRq == LNULL ) rc = R_LLOCKE;
      else {        /*------ Successful Lock -------- */
         hPdv = ro0020_globalalloc (sizeof(rT0715_unRoseApdus) );
         if (hPdv == LNULL ) rc = R_GALLCE;
         else {    /* ------- Successful Allocation ----------- */
             pPdv = (rT0715_unRoseApdus FAR * ) ro0021_globallock (hPdv);
             if (pPdv == LNULL ) rc = R_GLOCKE;

             else { /* -------- Sucessful Alloc & Lock ------------- */
                 if (pInvRq->ulOperationClass == RC070o_OpClass1 ){
                        pContext->bSyncFlag = TRUE;
                        pContext->lInvokeId = pInvRq->lInvokeID;
                 }
                 /* --------- Fill the Pdv Header ----------------- */
                 pPdv->stPdvHeader.lFraCount     = (long int)1;
                 pPdv->stPdvHeader.vPdvHandle[0] = hPdv;
                 pPdv->stPdvHeader.sPmNumb       = RC0219_Rose;
                 pPdv->stPdvHeader.sAbstrSynId   = pInvRq->sAbsId;
                 pPdv->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;
                 pPdv->stPdvHeader.lType = RC071P_ROIVapdu;
                 /* ------- PdvHeader is Over ------------------------ */
   
                 /* ---------- INV apdu ------------------------------ */
                 pPdv->stROIVapdu.lInvokeID = pInvRq->lInvokeID;
                 if (pInvRq->bLinkedID ){
                        pPdv->stROIVapdu.bLinkedID = TRUE;
                        pPdv->stROIVapdu.lLinkedID = pInvRq->lLinkedID;
                 }
                 else   /* ----- No Linked ID ------- */
                        pPdv->stROIVapdu.bLinkedID = FALSE;
                          
                 pPdv->stROIVapdu.stOperationValue = 
                                       pInvRq->stOperationValue;
          
                /* ----------- Argument in the P1 (OPTIONAL) -------- */
                 if (*phP1 == LNULL ) { /* No Arguments -- */ 
                     pPdv->stROIVapdu.bArgument =  FALSE;
                     pPdv->stROIVapdu.hArgument =  LNULL;
                 }                        
                 else {   /* -------- Argument Present ------------ */
                     pPdv->stROIVapdu.bArgument =  TRUE;
                     pPdv->stROIVapdu.hArgument =  *phP1;
                     
                 }
                 /* ----------- Pdv Node is Over ------------------ */
                 if ((ro0022_globalunlock(hPdv) ) != OK) {
                     rc = R_GULOKE;
                     if ((ro0024_globalfree(hPdv)) != OK) rc = R_GFREEE;
                 }
                 if ( (ro0032_localunlock(*phP3) ) != OK )
                         rc = R_LULOKE;
                 if ( (ro0034_localfree (*phP3) )!= OK )
                         rc = R_LFREEE;
                 /* ------- Assign the new P1 -------------------- */
                 
                 *phP1 = hPdv ;
                 *plOutEvent = RC073w_TRANSreq;
                  pContext->sSubAutoNum =  (pContext->bmappingInfo)
                                            ? RC073h_RopmPrTr
                                            : RC073i_RopmRtTr            ;
                                 
             } /* ----- Sucessful Alloc & Lock else ends ----------- */
         } /* ------- Successful Allocation else ends --------- */
      }/* ------- Successful Lock ends -------------- */

  
RM0003_RETURN (rs072a, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro072b_STA02xRESreq	                              */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro072b_STA02xRESreq (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int      rc = OK;
rT070d_stResReq        *pResRq = LNULL;

unsigned long  int      hPdv   = LNULL;
rT0715_unRoseApdus FAR *pPdv   = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs072b)
  /* ------------------ Lock the P3 handle -------------------------- */
      pResRq = (rT070d_stResReq *) ro0031_locallock ( *phP3 );
      if (pResRq == LNULL ) rc = R_LLOCKE;
      else {        /*------ Successful Lock -------- */
         hPdv = ro0020_globalalloc (sizeof(rT0715_unRoseApdus) );
         if (hPdv == LNULL ) rc = R_GALLCE;
         else {    /* ------- Successful Allocation ----------- */
             pPdv = (rT0715_unRoseApdus FAR * ) ro0021_globallock (hPdv);
             if (pPdv == LNULL ) rc = R_GLOCKE;

             else { /* -------- Sucessful Alloc & Lock ------------- */
                 /* --------- Fill the Pdv Header ----------------- */
                 pPdv->stPdvHeader.lFraCount     = (long int)1;
                 pPdv->stPdvHeader.vPdvHandle[0] = hPdv;
                 pPdv->stPdvHeader.sPmNumb       = RC0219_Rose;
                 pPdv->stPdvHeader.sAbstrSynId   = pResRq->sAbsId;
                 pPdv->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;
                 pPdv->stPdvHeader.lType = RC071Q_RORSapdu;
                 /* ------- PdvHeader is Over ------------------------ */
   
                 /* ---------- Result Apdu---------------------------- */
                 pPdv->stRORSapdu.lInvokeID = pResRq->lInvokeID;
                  
                 if (pResRq->bOperationValue ) {
                    pPdv->stRORSapdu.bOperationResult = TRUE;
                            
                    pPdv->stRORSapdu.stOperationResult.stOperationValue = 
                                       pResRq->stOperationValue;
                    pPdv->stRORSapdu.stOperationResult.hResult = 
                                       *phP1 ;
                 }
                 else  /* ------ No Result -------------------------- */
                    pPdv->stRORSapdu.bOperationResult = FALSE;
          
                 /* ----------- Pdv Node is Over ------------------ */
                 if ((ro0022_globalunlock(hPdv) ) != OK) {
                     rc = R_GULOKE;
                     if ((ro0024_globalfree(hPdv)) != OK) rc = R_GFREEE;
                 }
                 if ( (ro0032_localunlock(*phP3) ) != OK )
                         rc = R_LULOKE;
                 if ( (ro0034_localfree (*phP3) )!= OK )
                         rc = R_LFREEE;
                 /* ------- Assign the new P1 -------------------- */
                 
                 *phP1 = hPdv ;
                 *plOutEvent = RC073w_TRANSreq;
                  pContext->sSubAutoNum =  (pContext->bmappingInfo)
                                             ? RC073h_RopmPrTr
                                             : RC073i_RopmRtTr            ;
                                 
             } /* ----- Sucessful Alloc & Lock else ends ----------- */
         } /* ------- Successful Allocation else ends --------- */
      }/* ------- Successful Lock ends -------------- */

  
RM0003_RETURN (rs072b, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro072c_STA02xERRreq                                 */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro072c_STA02xERRreq (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        






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
signed   short int      rc = OK;
rT070f_stErrReq        *pErrRq = LNULL;

unsigned long  int      hPdv   = LNULL;
rT0715_unRoseApdus FAR *pPdv   = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs072c)

  /* ------------------ Lock the P3 handle -------------------------- */
      pErrRq = (rT070f_stErrReq *) ro0031_locallock ( *phP3 );
      if (pErrRq == LNULL ) rc = R_LLOCKE;
      else {        /*------ Successful Lock -------- */
         hPdv = ro0020_globalalloc (sizeof(rT0715_unRoseApdus) );
         if (hPdv == LNULL ) rc = R_GALLCE;
         else {    /* ------- Successful Allocation ----------- */
             pPdv = (rT0715_unRoseApdus FAR * ) ro0021_globallock (hPdv);
             if (pPdv == LNULL ) rc = R_GLOCKE;

             else { /* -------- Sucessful Alloc & Lock ------------- */
                 /* --------- Fill the Pdv Header ----------------- */
                 pPdv->stPdvHeader.lFraCount     = (long int)1;
                 pPdv->stPdvHeader.vPdvHandle[0] = hPdv;
                 pPdv->stPdvHeader.sPmNumb       = RC0219_Rose;
                 pPdv->stPdvHeader.sAbstrSynId   = pErrRq->sAbsId;
                 pPdv->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;
                 pPdv->stPdvHeader.lType = RC071R_ROERapdu;
                 /* ------- PdvHeader is Over ------------------------ */
   
                 /* ---------- Result Apdu---------------------------- */
                 pPdv->stROERapdu.lInvokeID = pErrRq->lInvokeID;
                  
                            
                 pPdv->stROERapdu.stErrorValue = 
                                       pErrRq->stErrorValue;
          
                /* ----------- Parameter in the P1 (OPTIONAL) -------- */
                 if (*phP1 == LNULL ) { /* No Parameter -- */ 
                     pPdv->stROERapdu.bParameter =  FALSE;
                     pPdv->stROERapdu.hParameter =  LNULL;
                 }                        
                 else {   /* -------- Parameter Present ------------ */
                     pPdv->stROERapdu.bParameter =  TRUE;
                     pPdv->stROERapdu.hParameter =  *phP1;
                     
                 }
                 /* ----------- Pdv Node is Over ------------------ */
                 if ((ro0022_globalunlock(hPdv) ) != OK) {
                     rc = R_GULOKE;
                     if ((ro0024_globalfree(hPdv)) != OK) rc = R_GFREEE;
                 }
                 if ( (ro0032_localunlock(*phP3) ) != OK )
                         rc = R_LULOKE;
                 if ( (ro0034_localfree (*phP3) )!= OK )
                         rc = R_LFREEE;
                 /* ------- Assign the new P1 -------------------- */
                 
                 *phP1 = hPdv ;
                 *plOutEvent = RC073w_TRANSreq;
                 pContext->sSubAutoNum =  (pContext->bmappingInfo)
                                          ? RC073h_RopmPrTr
                                           : RC073i_RopmRtTr            ;
                                 
             } /* ----- Sucessful Alloc & Lock else ends ----------- */
         } /* ------- Successful Allocation else ends --------- */
      }/* ------- Successful Lock ends -------------- */

RM0003_RETURN (rs072c, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro072d_STA02xRJUreq                                 */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro072d_STA02xRJUreq (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int       rc     = OK;
rT070h_stURejReq       * pRjURq = LNULL;
unsigned long  int       hPdv   = LNULL;
rT0715_unRoseApdus FAR * pPdv = LNULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs072d)
  /* ------------------ Lock the P3 handle -------------------------- */
      pRjURq = (rT070h_stURejReq *) ro0031_locallock ( *phP3 );
      if (pRjURq == LNULL ) rc = R_LLOCKE;
      else {        /*------ Successful Lock -------- */
         hPdv = ro0020_globalalloc (sizeof(rT0715_unRoseApdus) );
         if (hPdv == LNULL ) rc = R_GALLCE;
         else {    /* ------- Successful Allocation ----------- */
             pPdv = (rT0715_unRoseApdus FAR * ) ro0021_globallock (hPdv);
             if (pPdv == LNULL ) rc = R_GLOCKE;

             else { /* -------- Sucessful Alloc & Lock ------------- */
                 /* --------- Fill the Pdv Header ----------------- */
                 pPdv->stPdvHeader.lFraCount     = (long int)1;
                 pPdv->stPdvHeader.vPdvHandle[0] = hPdv;
                 pPdv->stPdvHeader.sPmNumb       = RC0219_Rose;
                 pPdv->stPdvHeader.sAbstrSynId   = pRjURq->sAbsId;
                 pPdv->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;
                 pPdv->stPdvHeader.lType = RC071S_RORJapdu;
                 /* ------- PdvHeader is Over ------------------------ */
   
                 /* ---------- Result Apdu---------------------------- */
                 pPdv->stRORJapdu.lInvokeIDChoiceNum = 
                                        RC071P_InvIdPrest ;

                 pPdv->stRORJapdu.lInvokeID = pRjURq->lInvokeID;
                 {
                   signed long int lProblem;

                   lProblem = (pRjURq->ulRejReason & 0x000000ff)  ;
                   pPdv->stRORJapdu.lProblemChoiceNum =
                         (pRjURq->ulRejReason & 0x00030000) >> 16 ;
                   switch(pPdv->stRORJapdu.lProblemChoiceNum ) {
                      case RC0713_InvokeProblem:
                           pPdv->stRORJapdu.lProblem = (lProblem <= 7)
                                                      ? lProblem 
                                                      : 0               ;
                           break;
                      case RC0714_ReturnResultProblem:
                           pPdv->stRORJapdu.lProblem = (lProblem <= 2)
                                                      ? lProblem 
                                                      : 0               ;
                           break;
                      case RC0715_ReturnErrorProblem:
                           pPdv->stRORJapdu.lProblem = (lProblem <= 4)
                                                      ? lProblem 
                                                      : 0               ;
                           break;
                      default:
                           pPdv->stRORJapdu.lProblemChoiceNum =
                                                RC0713_InvokeProblem; 
                           pPdv->stRORJapdu.lProblem = (lProblem <= 7)
                                                      ? lProblem 
                                                      : 0               ;
                           break;
                    }/* end of switch */
                  }/*end of block*/
                                      
                 /* ----------- Pdv Node is Over ------------------ */
                 if ((ro0022_globalunlock(hPdv) ) != OK) {
                     rc = R_GULOKE;
                     if ((ro0024_globalfree(hPdv)) != OK) rc = R_GFREEE;
                 }
                 if ( (ro0032_localunlock(*phP3) ) != OK )
                         rc = R_LULOKE;
                 if ( (ro0034_localfree (*phP3) )!= OK )
                         rc = R_LFREEE;
                 /* ------- Assign the new P1 -------------------- */
                 
                 *phP1 = hPdv ;
                 *plOutEvent = RC073w_TRANSreq;
                  pContext->sSubAutoNum =  (pContext->bmappingInfo)
                                           ? RC073h_RopmPrTr
                                           : RC073i_RopmRtTr            ;
                                 
             } /* ----- Sucessful Alloc & Lock else ends ----------- */
         } /* ------- Successful Allocation else ends --------- */
      }/* ------- Successful Lock ends -------------- */

RM0003_RETURN (rs072d, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro072e_STA02xAA_REL                    	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro072e_STA02xAA_REL (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int      rc = OK;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs072e)

     pContext->sStateRopm = RC0732_STA01;
     pContext->sSubAutoNum= RC073j_End;
     *plOutEvent          = RC0706_RO_UNBINDind;
     rc                   = RC073k_PushOutEvent;

RM0003_RETURN (rs072e, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro072f_STA02xROINV                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-03                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro072f_STA02xROINV (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int      rc = OK;

unsigned long  int      hInvInd = LNULL;
rT0715_unRoseApdus FAR *pPdv    = LNULL;
rT070c_stInvInd        *pInvInd = LNULL;
unsigned long  int      hTmp    = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs072f)
/* if (pContext->ulAssocClas == RC070l_AssocClass1){
      signed   short int        sAbsId;    
      signed   long  int        lInvokeId; 

      pPdv = (rT0715_unRoseApdus FAR *) ro0021_globallock ( *phP1 );
      if (pPdv == LNULL ) 
            RM0003_RETURN (rs072f, signed short int, -R_GLOCKE)
      sAbsId = pPdv->stPdvHeader.sAbstrSynId;
      lInvokeId= pPdv->stROIVapdu.lInvokeID;
      if ((ro0022_globalunlock(*phP1) ) != OK) 
            RM0003_RETURN (rs072f, signed short int, -R_GULOKE)
                                        
   * free the memeory *
   rc = ro0701_cleanUp (*plInEvent,*phP1,*phP2,*phP3,*phP4);                   
                                                                            
  if (rc == OK ) {                                                            
   *phP1 = *phP2 = *phP3 = *phP4 = LNULL;                                   
   *phP1 = ro0020_globalalloc ( sizeof (rT0715_unRoseApdus));               
    pPdv = (*phP1) ? (rT0715_unRoseApdus FAR * ) ro0021_globallock (*phP1)  
                   : LNULL;                                                 
    if (pPdv != NULL){                                                      
       pPdv->stPdvHeader.lFraCount     = (long int)1;              
       pPdv->stPdvHeader.vPdvHandle[0] = *phP1;                    
       pPdv->stPdvHeader.sPmNumb       = RC0219_Rose;              
       pPdv->stPdvHeader.sAbstrSynId   = sAbsId;                   
       pPdv->stPdvHeader.lEncodingChoice = RCro0m_ChoiceAny;       
       pPdv->stPdvHeader.lType = RC071S_RORJapdu;    
                
       pPdv->stRORJapdu.lInvokeIDChoiceNum =  RC071P_InvIdPrest;
       pPdv->stRORJapdu.lInvokeID = lInvokeId;                  
       pPdv->stRORJapdu.lProblemChoiceNum = RC0713_InvokeProblem; 
       pPdv->stRORJapdu.lProblem = RC071C_resourceLimitation;        
                                                                             
       *plOutEvent = RC073w_TRANSreq;                              
       pContext->sSubAutoNum =  (pContext->bmappingInfo )          
                                 ? RC073h_RopmPrTr
                                 : RC073i_RopmRtTr            ;
       if (ro0022_globalunlock (*phP1) ) rc = -R_GULOKE;           
     }              
   }
 }
 else {*/  /* Operation Allowed */
  /* ------------------ Lock the P1(User Data ) handle ---------------- */
      pPdv = (rT0715_unRoseApdus FAR *) ro0021_globallock ( *phP1 );
      if (pPdv == LNULL ) rc = R_GLOCKE;
      else {        /*------ Successful Lock -------- */
         hInvInd = ro0030_localalloc (sizeof(rT070c_stInvInd) );
         if (hInvInd == LNULL ) rc = R_LALLCE;
         else {    /* ------- Successful Allocation ----------- */
             pInvInd = (rT070c_stInvInd * ) ro0031_locallock (hInvInd);
             if (pInvInd == LNULL ) rc = R_LLOCKE;

             else { /* -------- Sucessful Alloc & Lock ------------- */
   
                 /* ---------- Invoke Ind ---------------------------- */
                 pInvInd->stOperationValue = pPdv->stROIVapdu.stOperationValue;
                 pInvInd->lInvokeID        = 
                 *phP4                     = pPdv->stROIVapdu.lInvokeID;
                 
                 if (pPdv->stROIVapdu.bLinkedID ){
                    pInvInd->bLinkedID        = TRUE;
                    pInvInd->lLinkedID        = pPdv->stROIVapdu.lLinkedID;
                 }
                 else 
                    pInvInd->bLinkedID        = FALSE;
                           
                if (pPdv->stROIVapdu.bArgument )  /* User data p1 */
                      hTmp = pPdv->stROIVapdu.hArgument;
                else 
                      hTmp = LNULL;
                                      
                 /* ----------- Pdv Node is Over ------------------ */
                 if ((ro0022_globalunlock(*phP1) ) == OK) {
                      if ((ro0024_globalfree(*phP1)) != OK)    
                          rc = R_GFREEE;
                 }
                 else  
                       rc = R_GULOKE;
                 if ( ro0032_localunlock(hInvInd) ) rc = -R_LULOKE;
                 
                /*  ------- Assign Output control prrameters ----- */
                 if (rc == OK){
                     *plOutEvent =  RC070d_RO_INVind;
                     pContext->sSubAutoNum =  RC073j_End ;
                     rc           = RC073k_PushOutEvent;
                     *phP1        = hTmp;
                     *phP3        = hInvInd;
                 }
             } /* end else of Successful Alloc & lock */
          } /*end else of Successful Alloc */
      } /* end of Successful Lock of p1 onlt */
/*   } */ /*end else of Allowed Operation */
                
RM0003_RETURN (rs072f, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro072g_STA02xRORES                                  */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro072g_STA02xRORES (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        
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
signed   short int      rc = OK;
unsigned long  int      hResInd = LNULL;
rT0715_unRoseApdus FAR *pPdv    = LNULL;
rT070e_stResInd        *pResInd = LNULL;
unsigned long  int      hTmp    = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs072g)
  /* ------------------ Lock the P1(User Data ) handle ---------------- */
      pPdv = (rT0715_unRoseApdus FAR *) ro0021_globallock ( *phP1 );
      if (pPdv == LNULL ) rc = R_GLOCKE;
      else {        /*------ Successful Lock -------- */
         hResInd = ro0030_localalloc (sizeof(rT070e_stResInd) );
         if (hResInd == LNULL ) rc = R_LALLCE;
         else {    /* ------- Successful Allocation ----------- */
             pResInd = (rT070e_stResInd * ) ro0031_locallock (hResInd);
             if (pResInd == LNULL ) rc = R_LLOCKE;

             else { /* -------- Sucessful Alloc & Lock ------------- */
   
                 /* ---------- Resoke Ind ---------------------------- */
                 if (pPdv->stRORSapdu.bOperationResult ) {
                    pResInd->stOperationValue = 
                           pPdv->stRORSapdu.stOperationResult.stOperationValue;
                    hTmp = pPdv->stRORSapdu.stOperationResult.hResult;
                    pResInd->bOperationValue = TRUE;
                 }
                 else {
                    pResInd->bOperationValue = FALSE;
                    hTmp = LNULL;
                 }

                     
                 pResInd->lInvokeID        = 
                 *phP4                     = pPdv->stRORSapdu.lInvokeID;
                 if ( ( pContext->bSyncFlag )  &&
                      (pContext->lInvokeId  == pResInd->lInvokeID) )
                            pContext->bSyncFlag = FALSE;
                                         
                 /* ----------- Pdv Node is Over ------------------ */
                 if ((ro0022_globalunlock(*phP1) ) == OK) {
                      if ((ro0024_globalfree(*phP1)) != OK)    
                           rc = R_GFREEE;
                 }else 
                      rc = R_GULOKE;
                 
                 
                 if ( ro0032_localunlock(hResInd) ) rc = -R_LULOKE;
                /*  ------- Assign Output control prrameters ----- */
                 if (rc ==OK){
                      *plOutEvent =  RC070f_RO_RESind;
                      pContext->sSubAutoNum =  RC073j_End ;
                      rc           = RC073k_PushOutEvent;
                      *phP1        = hTmp;
                      *phP3        = hResInd;
                 }
             } /* end else of Successful Alloc & lock */
          } /*end else of Successful Alloc */
      } /* end of Successful Lock of p1 onlt */
                

RM0003_RETURN (rs072g, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro072h_STA02xROERR              		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro072h_STA02xROERR (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int      rc = OK;
unsigned long  int      hErrInd = LNULL;
rT0715_unRoseApdus FAR *pPdv    = LNULL;
rT070g_stErrInd        *pErrInd = LNULL;
unsigned long  int      hTmp    = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs072h)
  /* ------------------ Lock the P1(User Data ) handle ---------------- */
      pPdv = (rT0715_unRoseApdus FAR *) ro0021_globallock ( *phP1 );
      if (pPdv == LNULL ) rc = R_GLOCKE;
      else {        /*------ Successful Lock -------- */
         hErrInd = ro0030_localalloc (sizeof(rT070g_stErrInd) );
         if (hErrInd == LNULL ) rc = R_LALLCE;
         else {    /* ------- Successful Allocation ----------- */
             pErrInd = (rT070g_stErrInd * ) ro0031_locallock (hErrInd);
             if (pErrInd == LNULL ) rc = R_LLOCKE;

             else { /* -------- Sucessful Alloc & Lock ------------- */
   
                 /* ---------- Error Ind ---------------------------- */
                 pErrInd->stErrorValue = pPdv->stROERapdu.stErrorValue;
                     
                 pErrInd->lInvokeID        = 
                 *phP4                     = pPdv->stROERapdu.lInvokeID;
                
                 if ( ( pContext->bSyncFlag )  &&
                      (pContext->lInvokeId  == pErrInd->lInvokeID) )
                            pContext->bSyncFlag = FALSE;
                 if (pPdv->stROERapdu.bParameter )
                          hTmp  = pPdv->stROERapdu.hParameter;
                 else hTmp  = LNULL;
                   
                                      
                 /* ----------- Pdv Node is Over ------------------ */
                 if ((ro0022_globalunlock(*phP1) ) == OK) {
                       if ((ro0024_globalfree(*phP1)) != OK)    
                                rc = R_GFREEE;
                 }else 
                    rc = R_GULOKE;
                 if ( ro0032_localunlock(hErrInd) ) rc = -R_LULOKE;
                 
                 
                /*  ------- Assign Output control prrameters ----- */
                 if (rc==OK){
                      *plOutEvent =  RC070h_RO_ERRind;
                      pContext->sSubAutoNum =  RC073j_End ;
                      rc           = RC073k_PushOutEvent;
                      *phP1        = hTmp;
                      *phP3        = hErrInd;
                 }
             } /* end else of Successful Alloc & lock */
          } /*end else of Successful Alloc */
      } /* end of Successful Lock of p1 onlt */
                
RM0003_RETURN (rs072h, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro072i_STA02xRORJp             		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro072i_STA02xRORJu (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int      rc = OK;
unsigned long  int      hRjUInd = LNULL;
rT0715_unRoseApdus FAR *pPdv    = LNULL;
rT070i_stURejInd        *pRjUInd = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs072i)
  /* ------------------ Lock the P1(User Data ) handle ---------------- */
      pPdv = (rT0715_unRoseApdus FAR  *) ro0021_globallock ( *phP1 );
      if (pPdv == LNULL ) rc = R_GLOCKE;
      else {        /*------ Successful Lock -------- */
         hRjUInd = ro0030_localalloc (sizeof(rT070i_stURejInd) );
         if (hRjUInd == LNULL ) rc = R_LALLCE;
         else {    /* ------- Successful Allocation ----------- */
             pRjUInd = (rT070i_stURejInd * ) ro0031_locallock (hRjUInd);
             if (pRjUInd == LNULL ) rc = R_LLOCKE;

             else { /* -------- Sucessful Alloc & Lock ------------- */
   
                 /* ---------- User Reject ind------------------------ */
                 if (pPdv->stRORJapdu.lInvokeIDChoiceNum 
                       == RC071P_InvIdPrest ) {
                     pRjUInd->lInvokeID = 
                     *phP4              = pPdv->stRORJapdu.lInvokeID;
                     if ( ( pContext->bSyncFlag )  &&
                          (pContext->lInvokeId  == pRjUInd->lInvokeID) )
                                pContext->bSyncFlag = FALSE;
                 }
                 pRjUInd->ulRejReason = 
                              ( pPdv->stRORJapdu.lProblem |
                               (pPdv->stRORJapdu.lProblemChoiceNum  << 16 ) |
                               RC0700_RoseEvent                            );

                                      
                 /* ----------- Pdv Node is Over ------------------ */
                 if ((ro0022_globalunlock(*phP1) ) == OK) {
                       if ((ro0024_globalfree(*phP1)) != OK)    
                                rc = R_GFREEE;
                  }else 
                      rc = R_GULOKE;
                 
                 if ( (ro0032_localunlock(hRjUInd) ) != OK )
                         rc = R_LULOKE;
                 
                /*  ------- Assign Output control prrameters ----- */
                if (rc == OK){
                     *plOutEvent =  RC070j_RO_RJUind;
                     pContext->sSubAutoNum =  RC073j_End ;
                     rc           = RC073k_PushOutEvent;
                     *phP3        = hRjUInd;
                     *phP1        = LNULL;
                }
             } /* end else of Successful Alloc & lock */
          } /*end else of Successful Alloc */
      } /* end of Successful Lock of p1 onlt */
                
RM0003_RETURN (rs072i, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro072j_STA02xRORJp             		      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro072j_STA02xRORJp (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int      rc = OK;
unsigned long  int      hRjPInd = LNULL;
rT0715_unRoseApdus FAR *pPdv    = LNULL;
rT070j_stPRejInd        *pRjPInd = LNULL;
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs072j)
  /* ------------------ Lock the P1(User Data ) handle ---------------- */
      pPdv = (rT0715_unRoseApdus FAR *) ro0021_globallock ( *phP1 );
      if (pPdv == LNULL ) rc = R_GLOCKE;
      else {        /*------ Successful Lock -------- */
         hRjPInd = ro0030_localalloc (sizeof(rT070j_stPRejInd) );
         if (hRjPInd == LNULL ) rc = R_LALLCE;
         else {    /* ------- Successful Allocation ----------- */
             pRjPInd = (rT070j_stPRejInd * ) ro0031_locallock (hRjPInd);
             if (pRjPInd == LNULL ) rc = R_LLOCKE;

             else { /* -------- Sucessful Alloc & Lock ------------- */
   
                 /* ---------- User Reject ind------------------------ */
                 if (pPdv->stRORJapdu.lInvokeIDChoiceNum 
                       == RC071P_InvIdPrest ) {
                     pRjPInd->lInvokeID = 
                     *phP4              = pPdv->stRORJapdu.lInvokeID;
                     pRjPInd->bInvokeID = TRUE ;
                     if ( ( pContext->bSyncFlag )  &&
                          (pContext->lInvokeId  == pRjPInd->lInvokeID) )
                                pContext->bSyncFlag = FALSE;
                 }
                 else 
                     pRjPInd->bInvokeID = FALSE ;

                 pRjPInd->lRejectReason = 
                              ( pPdv->stRORJapdu.lProblem |
                               (pPdv->stRORJapdu.lProblemChoiceNum  << 16 ) |
                               RC0700_RoseEvent                            );
                 pRjPInd->bRejectReason = TRUE;

                                      
                 /* ----------- Pdv Node is Over ------------------ */
                 if ((ro0022_globalunlock(*phP1) ) == OK) {
                       if ((ro0024_globalfree(*phP1)) != OK)    
                                   rc = R_GFREEE;
                 }else 
                      rc = R_GULOKE;
                 if ( ro0032_localunlock(hRjPInd) ) rc = -R_LULOKE;
                 
                 
                /*  ------- Assign Output control prrameters ----- */
                 if (rc== OK){
                    *plOutEvent =  RC070k_RO_RJPind;
                    pContext->sSubAutoNum =  RC073j_End ;
                    rc           = RC073k_PushOutEvent;
                    *phP3        = hRjPInd;
                    *phP1        = LNULL;
                 }                 
             } /* end else of Successful Alloc & lock */
          } /*end else of Successful Alloc */
      } /* end of Successful Lock of p1 onlt */
                
RM0003_RETURN (rs072j, signed short int, rc)                                    
}/*end*/                                                                        


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: ro072k_STA02xAPDUua                   	      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-24                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro072k_STA02xAPDUua (
rT0730_stContext   NEAR  *pContext,       
signed   long  int FAR   *plInEvent,
signed   long  int FAR   *plOutEvent,
unsigned long  int FAR   *phP1,           
unsigned long  int FAR   *phP2,          
unsigned long  int FAR   *phP3,         
unsigned long  int FAR   *phP4)        

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
signed   short int      rc        = OK;
rT0715_unRoseApdus FAR *pPdv      = LNULL;
signed   long  int      lType     = NULL;
signed   short int      sAbsId    = NULL;
signed   short int      bInvokeId = TRUE;
signed   long  int      lInvokeId = LNULL;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

RM0002_ENTRY (rs072k)
  /* ------------------ Lock the P1(User Data ) handle ---------------- */
      pPdv = (rT0715_unRoseApdus FAR *) ro0021_globallock ( *phP1 );
      if (pPdv == LNULL ) rc = R_GLOCKE;
      else {        /*------ Successful Lock -------- */
           lType = pPdv->stPdvHeader.lType;
           sAbsId= pPdv->stPdvHeader.sAbstrSynId;
           switch ( lType){
                 case RC071P_ROIVapdu:
                           bInvokeId = TRUE;
                           lInvokeId = *phP4 = pPdv->stRORJapdu.lInvokeID;
                           break;
                 case RC071Q_RORSapdu:
                           bInvokeId = TRUE;
                           lInvokeId = *phP4 = pPdv->stRORJapdu.lInvokeID;
                           break;
                 case RC071R_ROERapdu:
                           bInvokeId = TRUE;
                           lInvokeId = *phP4 = pPdv->stRORJapdu.lInvokeID;
                           break;
                 case RC071S_RORJapdu:
                      if (pPdv->stRORJapdu.lInvokeIDChoiceNum ==
                                           RC071P_InvIdPrest ) {
                           bInvokeId = TRUE;
                           lInvokeId = *phP4 = pPdv->stRORJapdu.lInvokeID;
                      }
                      else bInvokeId = FALSE; 
                      break;
           }
           if ((ro0022_globalunlock(*phP1) ) != OK) rc = R_GULOKE;
/*           if ((ro0024_globalfree(*phP1)) != OK)    rc = R_GFREEE; */
           if ( (lType == RC071S_RORJapdu ) || 
                ( ++(pContext->sRejCount ) > RC073f_MaxAPDUua ) ){  
                                 /*Reject Pdu  so Reject */
                 rc = ro0708_ProviderAbort ( pContext,
                                             *plInEvent,
                                             rc,
                                             *phP1, *phP2, *phP3, *phP4 );

           }
           else {
               
                 rc =ro0709_ProviderReject (pContext, 
                                           plInEvent,
                                           plOutEvent,
                                           phP1, phP2, phP3, phP4,
                                           sAbsId , bInvokeId, lInvokeId);
            }
             
      } /* end else of Successful lock */

RM0003_RETURN (rs072k, signed short int, rc)                                    
}/*end*/                                                                        


