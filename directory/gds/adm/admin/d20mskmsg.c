/* 
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */ 
/* 
 * HISTORY
 * $Log: d20mskmsg.c,v $
 * Revision 1.1.8.2  1996/02/18  19:41:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:03  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:17:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:09  root]
 * 
 * Revision 1.1.6.3  1994/05/10  15:49:47  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:12:45  marrek]
 * 
 * Revision 1.1.6.2  1994/03/23  15:05:35  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:10:42  keutel]
 * 
 * Revision 1.1.6.1  1994/02/22  15:58:52  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:31:40  marrek]
 * 
 * Revision 1.1.4.2  1993/08/10  10:20:38  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:32:56  marrek]
 * 
 * Revision 1.1.2.3  1992/12/31  18:09:14  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:05:03  bbelch]
 * 
 * Revision 1.1.2.2  1992/11/27  13:18:00  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:13:16  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20mskmsg.c,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 19:41:20 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20mskmsg.c                                          */
/*								      */
/* AUTHOR       : Mueller V., SNI AP 11                               */
/* DATE         : 03.09.92                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  : Mask Definition Module of msgomask and qomask       */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*  0.1     |03.09.92 |  original                      |    |         */
/*datoff **************************************************************/

/*****************************************************************************/
/*                                                                           */
/*                 M A S K   D E F I N I T I O N   M O D U L E               */
/*                                                                           */
/*           (used by the administration of the directory system V2)         */
/*                                                                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  Author: V. Mueller                                    Date: 03.9.92      */
/*****************************************************************************/

#include <gds.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <cget.h>

/* ************************************************************************* */
/* ------------------------ M A S K (QUIT) --------------------------------- */
/* ************************************************************************* */

static char d20_clearl[] = { "                                                                           " } ;
D20_omask d20_qomask[] = {

	(D20_CSAY | D20_NOERASE), NORMAL, 23,  5, d20_clearl,		NULL, 0, FALSE, FALSE,
	D20_CSAY,    INVERS, 23,  5, "ERROR:",				"%-6s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 23, 12, NULL,				"%-67s", 0, FALSE, FALSE,
	D20_CSAY,    NORMAL, 24, 12, "To continue press <CR> !",        "%-24s", 0, FALSE, FALSE,
	D20_FKTX,     FKTCR,  0,  0, NULL,				NULL, 0, FALSE, FALSE,
	D20_EOMSK,           NORMAL } ;

/* ************************************************************************* */
/* ------------------------ M A S K (MESSAGE) ------------------------------ */
/* ************************************************************************* */

D20_omask d20_msgomask[] = {
	(D20_CSAY | D20_NOERASE), NORMAL, 23,  5, d20_clearl,		NULL, 0, FALSE, FALSE,
	D20_CSAY,           NORMAL, 23, 12, NULL,			"%-67s", 0, FALSE, FALSE,
	D20_CSAY,           NORMAL, 24, 12, "To continue press <CR> !", "%-24s", 0, FALSE, FALSE,
	D20_FKTX,            FKTCR,  0,  0, NULL,			NULL, 0, FALSE, FALSE,
	D20_EOMSK,          NORMAL } ;

