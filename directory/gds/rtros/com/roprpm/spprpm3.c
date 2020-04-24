/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spprpm3.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:46  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:08:56  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:31:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:17:53  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:06:50  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:34  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:28  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:53:27  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:33:56  keutel
 * 	creation
 * 	[1994/03/21  13:22:34  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spprpm3.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:46 $" } ;
#endif

/**********************************************************************/
/*                                                                    */
/*    COPYRIGHT  (C)						      */
/*    Siemens Nixdorf Informationssysteme AG 1993		      */
/*    All rights reserved					      */
/*								      */
/*    This software is furnished under licence and may be used only   */
/*    in accordance with the terms of that licence and with the       */
/*    inclusion of the above copyright notice.			      */
/*    This software may not be provided or otherwise made available   */
/*    to, or used by, any other person. No title to or ownership of   */
/*    the software is hereby transferred.			      */
/*                                                                    */
/**********************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME 	: spprpm3.c					      */
/*                                                                    */
/* AUTHOR	: Chandrasekar.K                                      */
/* DATE 	: 24-11-93					      */
/*                                                                    */
/* COMPONENT    : RTROS Evaluation Program                            */
/* PRD#/VERS.   : RTROS V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : Dummy functions                                     */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 3.00   K00| 24-11-93| original		       | kc |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spprpm3.c 1.1 93/12/20   RTS-V2.00";

/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rolgval.h>      /* General RTS definitons                   */    
#include <roerval.h>      /* General RTS definitons                   */    
#include <rouply0.h>
#include <rodpat0.h>
#include <rodpat1.h>
#include <roppm01.h>
#include <roppm04.h>
#include <roppm03.h>
#include <roppm05.h>
/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0411_encodePdvList                                */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-25                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0411_encodePdvList (
	rT0410_stPdvList   FAR       *pEncodingInfo,
	unsigned long  int FAR * FAR * pTransSynName,
	signed   short int FAR       *pPmNumb,      
	void             *pContext,     
	unsigned long  int            hNode)                      

/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/
 return(0);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0410_encodeSimply                                 */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-25                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0410_encodeSimply (
	unsigned long  int FAR * FAR *pTransSynName,
	signed   short int       FAR *pPmNumb,      
	void             *pContext,     
	unsigned long  int            hNode)        
                                                                   
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/
 return(0);
}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : Rro0247_encodingPm                                  */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-25                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL ro0247_encodingPm (
	signed   short int        pmNo,     
	rTro76_stDefinedBy FAR  * defBy,     
	rTro01_pfvObjectId        tSyNa,     
	unsigned long  int        store,     
	void               NEAR * pcxt,     
	unsigned long  int FAR  * pdvN,     
	unsigned long  int FAR  * len,     
	signed         int        iAnyNumb)   
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/
 return(0);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0244:encodX410                                    */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-25                                            */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL  ro0244_encodX410 (
	signed   short int        sPmNo,   
	unsigned long  int FAR  * phsStore,
	void               NEAR * pContext,
	unsigned long  int FAR  * phNode)    
                                                                               
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/
 return(0);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0215_getEiCxt                                     */
/*                                                                    */
/* AUTHOR       :                                                     */
/* DATE         :                                                     */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL  ro0215_getEiCxt  (
	signed   short int          lay,
	unsigned long  int FAR   *eiCxt)

                                                                               
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/
 return(0);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0413_encodeAny                                    */
/*                                                                    */
/* AUTHOR       :                                                     */
/* DATE         :                                                     */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL  ro0413_encodeAny ( 
	rTro76_stDefinedBy FAR * FAR *pDefinedBy,   
	unsigned long  int FAR * FAR *pTransSynName,
	signed   short int       FAR *pPmNumb,      
	void             *pContext,     
	unsigned long  int            hNode)         
                                                                               
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/
 return(0);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : ro0412_encodeExternal                               */
/*                                                                    */
/* AUTHOR       :                                                     */
/* DATE         :                                                     */
/*                                                                    */
/* SYNTAX       :                                                     */
signed short int PASCAL  ro0412_encodeExternal(
	rT0411_stExternal  FAR       *pEncodingInfo,
	unsigned long  int FAR * FAR *pTransSynName,
	signed   short int FAR       *pPmNumb,      
	void             *pContext,     
	unsigned long  int            hNode)        
                                                                               
/*                                                                    */
/*inoff ***************************************************************/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */
{/*entr*/
 return(0);
}

