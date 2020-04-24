/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stanydef.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:25  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:46  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:02:28  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:02:08  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:50  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:45:55  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:29:46  keutel
 * 	creation
 * 	[1994/03/21  13:15:30  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stanydef.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:36 $" } ;
#endif

/****************************************************************************\
*                                                                            *
*  * Copyright (c) 1991 by                                                   *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG                                  *
*  * All rights reserved                                                     *
*  *                                                                         *
*  * This software is furnished under licence and may be used only in        *
*  * accordance with the terms of that licence and with the inclusion of     *
*  * the above copyright notice. This software may not be provided or        *
*  * otherwise made available to, or used by, any other person. No title     *
*  * to or ownership of the software is hereby transferred.                  *
*                                                                            *
\****************************************************************************/

/*daton *********************************************************************/
/*                                                                          */
/* TYPE         : MODULE                                                    */
/*                                                                          */
/* NAME         : st_anydef.c                                               */
/*                                                                          */
/* AUTHOR       : Hubert Trieb                                              */
/* DATE         : 20.04.1993                                                */
/*                                                                          */
/* COMPONENT    : Encode/Decode functions for ANY DEFINED BY as used        */
/*                in RTS.                                                   */
/*                                                                          */
/* PRD#/VERS.   : ST-MAVROS                                                 */
/*                                                                          */
/* DESCRIPTION  : This modul provides functions which                       */
/*                are the Interface between functions created               */
/*                by MAVROS and the Encode/Decode Control functions         */
/*                These functions are called when within a pdu the type     */
/*                ANY DEFINED BY is used.                                   */
/*                                                                          */
/* SYSTEM DEPENDENCE :  SINIX V5.4                                          */
/*                                                                          */
/* HISTORY      :                                                           */
/*                                                                          */
/* Vers.Nr. | Date     |         Modification              |mark| CR# EM#   */
/* 1.00 K0  | 93-04-20 |  original                         | ht |           */
/*          |          |                                   |    |           */
/*datoff ********************************************************************/

/****************************************************************************/
/*                                                                          */
/*                     Module Identification                                */
/*                                                                          */
/****************************************************************************/

static char *sccsid = " @(#) stanydef.c 1.1 93/11/29  ST_MAVROS";

/****************************************************************************/
/*                                                                          */
/*                    S T D - I N C L U D E S                               */
/*                                                                          */
/****************************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************************************/
/*                                                                           */
/*        D E F I N E S,  T Y P E D E F S  &   P R O T O T Y P E S           */
/*                                                                           */
/*****************************************************************************/

#include <rogenrl.h>                             /* general defines for RTS */
#include <rostor0.h>                                    /* Store Prototypes */
#include <rouply0.h>
#include <roppm01.h>
#include <rodpat1.h>
#include <roppm05.h>

#include <asn1st.h>

/***************************************************************************/
/*                                                                         */
/*              P R O V I D E D   G L O B A L   F U N C T I O N S          */
/*                                                                         */
/***************************************************************************/



/***************************************************************************/
/*                                                                         */
/* int RTS_anydef_cod(hdl,poszm,pNode,stDefinedBy,AnyNumb,pContext)        */
/*                                                                         */
/* STOREHANDLE         hdl;          Handle to store containing ASN1 String*/
/* ulong *             pNode;        pointer to next node handle           */
/* rTro76_stDefinedBy *stDefinedBy;  Value of the ANY DEF BY selector      */
/* int                 AnyNumb;      number to identify the PDU            */
/* void *              pContext;     PPM context(pointer to)               */
/*                                                                         */
/*                                                                         */
/* Function for encoding an ANY DEFINED BY (used e.g. in ROSE)             */
/* This function is called by a MAVROS created function.                   */
/*                                                                         */
/***************************************************************************/

int PASCAL RTS_anydef_cod(
STOREHANDLE hdl,
int                       m,
int                       n,
unsigned long int  FAR *  pNode,
rTro76_stDefinedBy FAR *  stDefinedBy,
int                       sAnyNumb,
void NEAR *                pContext)
{
  int sterr;
  signed short int ed_control_err;

  rTro76_stDefinedBy FAR * pDefinedBy;
  unsigned long int FAR * pTransSynName;
  signed short int sPmNumb;
  unsigned long int ulNumb; /* length of encoded ANY data */

  sterr = ro0063_sterror(hdl);
  if (sterr) return sterr;

  pDefinedBy = 0;
  ed_control_err = ro0413_encodeAny(&pDefinedBy,
				    &pTransSynName,
				    &sPmNumb, pContext, * pNode);
			/* get control information for next E/D PM */
  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }
  ed_control_err = ro0247_encodingPm(sPmNumb, stDefinedBy,
				     (rTro01_pfvObjectId)pTransSynName,
				     hdl, pContext, pNode, &ulNumb,sAnyNumb);
				     /* switch to next encodeing Function  */
  /* Any Numb is included as a parameter . by kc on 21/5 */
  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }
  return(ro0063_sterror(hdl));
}


/***************************************************************************/
/*                                                                         */
/* int RTS_anydef_dec(hdl,poszm,pNode,stDefinedBy,AnyNumb,pContext)        */
/*                                                                         */
/* STOREHANDLE         hdl;          Handle to store containing ASN1 String*/
/* unsigned long       poszm;        End of ASN1 String in store           */
/* ulong *             pNode;        pointer to next node handle           */
/* rTro76_stDefinedBy *stDefinedBy;  Value of the ANY DEF BY selector      */
/* int                 AnyNumb;      number to identify the PDU            */
/* void *              pContext;     PPM context(pointer to)               */
/*                                                                         */
/*                                                                         */
/* Function for decoding an ANY DEFINED BY (used e.g. in ROSE)             */
/* This function is called by a MAVROS created function.                   */
/*                                                                         */
/***************************************************************************/

int PASCAL RTS_anydef_dec(
STOREHANDLE hdl,
unsigned long        poszm,
unsigned long int FAR  * pNode,
rTro76_stDefinedBy FAR * stDefinedBy,
int sAnyNumb,
void NEAR * pContext)
{
  int sterr;
  signed short int ed_control_err;

  rTro76_stDefinedBy  FAR * pDefinedBy;
  unsigned long int FAR * pTransSynName;
  signed short int sAbstrSynId;
  signed short int sPmNumb;
  unsigned long int ulNumb; /* maximum length endposition  encoded ANY data */
  long  lEncodingChoice = RCro0m_ChoiceAny;


  pDefinedBy = 0;
  /* the comment on ulNumb=0 is by kc on 21/5 */
  ulNumb = poszm - ro0076_stwhere(hdl);  /* maximum length of ANY data */
/*  ulNumb = 0;*/ /* real length of of ANY data not known */

  sterr = ro0063_sterror(hdl);
  if (sterr)
	return (sterr);

  ed_control_err = ro0418_decodeAny(&pDefinedBy, &pTransSynName, &sAbstrSynId,
				    &sPmNumb, (signed short ) sAnyNumb,
				    pContext);
			/* get control information for next E/D PM */
  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
	return ((int)ed_control_err);
  }

  ed_control_err = ro0248_decodingPm(sPmNumb, sAbstrSynId,
				     lEncodingChoice,pDefinedBy,
				     (rTro01_pfvObjectId)pTransSynName,
				     hdl, pContext, pNode, ulNumb,sAnyNumb);
						 /* switch to next E/D PM  */
  /* Any Numb is included as a parameter . by kc on 21/5 */

  if (ed_control_err) {
	ro0080_seterror(hdl,ed_control_err);
					    /* assign error to store error */
  }
  return ((int)ed_control_err);
}

/* EOF */
