/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20dnget.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:42  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:11:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:49  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:56  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:10  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:18  marrek
 * 	February 1994 code drop
 * 	[1994/02/17  16:41:04  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:14:11  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:39:03  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:09:03  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:45:14  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:58:38  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:29:28  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:07:41  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20dnget.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:44 $";
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
/* NAME         : d20dnget.c                                          */
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
/*  FUNCTION:  d20_dnget ()                                                  */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function reads an entry information queue, given in a format     */
/*      used at the IAPL-interface of the directory system V2, and returns   */
/*      the distinguished names contained in this queue (one distinguished   */
/*      name per function call).                                             */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      mode        = Operation mode:                                        */
/*                       D20_BEGIN    = Start reading at the beginning of    */
/*                                      the entry information queue.         */
/*                       D20_CONTINUE = Continue reading of the entry in-    */
/*                                      formation queue from the actual      */
/*                                      position.                            */
/*                                                                           */
/*      vptr        = Pointer at which the reference to the found            */
/*                    distinguished name should be stored.                   */
/*                                                                           */
/*      info        = Entry information queue which should be read.          */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      vptr        = Pointer to the found distinguished name.               */
/*                    Note: Because the found distinguished names are used   */
/*                    by the administration on following directory calls,    */
/*                    every distinguished name is not only returned but      */
/*                    also copied into the common IAPL-interface parameter   */
/*                    block as object entry name.                            */
/*  RETURN-VALUE:                                                            */
/*      D2_NOERROR  = The function has been performed successfully.          */
/*      D2_ERROR    = An error is occurred -> entry information queue is     */
/*                    empty or the end of the entry information queue is     */
/*                    reached.                                               */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 4.3.88       */
/*exoff **********************************************************************/

#include <gds.h>
#include <stdlib.h>
#include <string.h>

#include <dce/d2dir.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2info.h>
#include <d20proto.h>

/* -------------------- declaration of global data ------------------------- */

extern Ds_v2_info      info ;       /* common IAPL-interface parameter block */

extern D2_ei_info     *d20_entptr ; /* pointer to actual entry information   */

/* ------------------------------------------------------------------------- */

 d2_ret_val d20_dnget (
   signed16        mode,
   D2_name_string *vptr)
{

	if (mode == D20_BEGIN)
		/* start reading at beginning of the entry information queue */
	   {    if (info.resentry == (D2_ei_info *) NULL)
			/* error -> entry information queue is empty */
			return (D2_ERROR) ;
		d20_entptr = info.resentry ;
	   }
	else
		/* go to next queue entry */
	   d20_entptr = d20_entptr->d2_ei_next ;

	if (d20_entptr == (D2_ei_info *) NULL)
		/* error -> end of entry information queue is reached */
		return (D2_ERROR) ;

	/* entry information available, return distinguished name */
	*vptr = d20_entptr->d2_ei_entry ;
	/* copy found distinguished name into common parameter block */
	strcpy ((char *)(info.entry + 1), (char *)d20_entptr->d2_ei_entry) ;

	return (D2_NOERROR) ;
}
