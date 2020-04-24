/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20subset.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:02  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:12:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:13  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:12  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:03:04  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:41  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:16:33  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:54:01  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:12:09  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:46:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:59:40  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:35:57  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:09:06  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20subset.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:05 $";
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
/* NAME         : d20subset.c                                         */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 10.05.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d20_set_subset ()                                             */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function inserts a given subset value (used on directory search  */
/*      operations) into the common parameter block used at the IAPL-inter-  */
/*      face of the directory system V2.                                     */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      subset      = Subset value                                           */
/*                       D2_BASE_OBJECT   = single object search.            */
/*                       D2_ONE_LEVEL     = one level search.                */
/*                       D2_WHOLE_SUBTREE = whole subtree search.            */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    None                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                         DATE: 11.3.87     */
/*exoff **********************************************************************/

#include <gds.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2info.h>
#include <d20proto.h>

/* -------------------- declaration of global data ------------------------- */

extern Ds_v2_info	info ;	/* common IAPL-interface parameter block */

/* ------------------------------------------------------------------------- */

 void d20_set_subset (
   D2_subset subset)
{
	/* fill in given subset value into common parameter block */
	info.srchop = subset ;
}
