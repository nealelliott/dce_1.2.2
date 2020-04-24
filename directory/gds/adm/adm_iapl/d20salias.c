/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20salias.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:55  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:12:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:06  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:03:06  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:02:45  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:35  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:15:52  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:48:27  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:11:00  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:46:04  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:59:19  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:35:04  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:08:38  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20salias.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:57 $";
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
/* NAME         : d20salias.c                                         */
/*								      */
/* AUTHOR       : Schmid , D AP 11                                    */
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
/*  FUNCTION:  d20_ins_alias(), d20_res_alias()                              */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*      The function fill the entry name to the alias name and put           */
/*      the aliased object name into the entry name                          */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      aliased objectname (only for ins_alias)                              */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      info structure for add alias                                         */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    None                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: W.Schmid                                      DATE: 4.5.88       */
/*exoff **********************************************************************/

#include <gds.h>

#include <dce/d2dir.h>
#include <d2info.h>
#include <d2dump.h>
#include <d20proto.h>

/* -------------------- declaration of global data ------------------------- */

extern Ds_v2_info	info ;	/* common IAPL-interface parameter block */

/* ------------------------------------------------------------------------- */

static D2_name_string   entry_sav;
static D2_name_string   alias_sav;


 void d20_ins_alias (
   char *aliased_obj)

{
  entry_sav = info.entry;
  alias_sav = info.alias;

  info.alias = info.entry;
  info.entry = (D2_name_string)aliased_obj;
}

 void d20_res_alias (void)

{
  info.entry = entry_sav  ;
  info.alias = alias_sav  ;

}

