/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20frame.c,v $
 * Revision 1.1.4.2  1996/02/18  19:40:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:21:18  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:16:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:51:38  root]
 * 
 * Revision 1.1.2.2  1994/06/09  18:44:33  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:15  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:14:12  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:49:09  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20frame.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 19:40:16 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20frame.c                                          */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 14.12.89                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  : Mask Definition Module                              */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*  0.1     |14.12.89 |  original                      |    |         */
/*datoff **************************************************************/

#include <gds.h>
#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2shm.h>
#include <d2adm.h>
#include <d2msk.h>
#include <cget.h>


/* ************************************************************************* */
/* ------------------------- F R A M E   M A S K --------------------------- */
/* ************************************************************************* */

#ifdef WX200
static char d20_f1ofield[] =
{ "\x01b[12mZDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD?\x01b[10m" };
static char d20_f2ofield[] =
{ "\x01b[12mCDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD4\x01b[10m" };
static char d20_f3ofield[] =
{ "\x01b[12m\x040DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDY\x01b[10m" };
static char d20_f4ofield[] = { "\x01b[12m3\x01b[10m" };
#else
static char d20_f1ofield[] =
{ "+----------------------------------------------------------------------------+" } ;
static char d20_f2ofield[] =
{ "+----------------------------------------------------------------------------+" } ;
static char d20_f3ofield[] =
{ "+----------------------------------------------------------------------------+" } ;
static char d20_f4ofield[] = { "|" } ;
#endif

D20_omask d20_frameomask[] = {
	D20_CSAY,  NORMAL,  1,  2, d20_f1ofield, NULL,
	D20_CSAY,  NORMAL,  2,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  2, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  3,  2, d20_f2ofield, NULL,
	D20_CSAY,  NORMAL,  4,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  4, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  5,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  5, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  6,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  6, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  7,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  7, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  8,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  8, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  9,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL,  9, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 10,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 10, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 11,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 11, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 12,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 12, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 13,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 13, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 14,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 14, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 15,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 15, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 16,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 16, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 17,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 17, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 18,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 18, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 19,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 19, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 20,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 20, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 21,  2, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 21, 79, d20_f4ofield, NULL,
	D20_CSAY,  NORMAL, 22,  2, d20_f3ofield, NULL,
	D20_EOMSK, NORMAL } ;

