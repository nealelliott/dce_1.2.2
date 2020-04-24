/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20tnsx.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:12  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:18:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:15  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:53  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:13:19  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:37  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:11:19  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:59:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:32:55  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:26:45  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:34:15  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:09:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:05:23  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  18:01:20  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	[1992/12/04  10:35:43  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  13:19:33  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  14:14:28  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  22:02:06  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:17:22  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20tnsx.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:29 $";
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
/* NAME         : d20tnsx.c                                           */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 15.11.86                                            */
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

#include <gds.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include <dce/cmx_addr.h>
#include <tnsx.h>
#include <d20proto.h>

/*exon ***********************************************************************/
/*                                                                           */
/*                         FUNCTION hex_asc ()                               */
/*                         -------------------                               */
/*                                                                           */
/*   Funktion zur Umwandlung eines Binaerstrings in einen Hexziffernstring   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   Autor: R. Horn                                     Date : 23.1.86       */
/*exoff **********************************************************************/
 
 signed32 hex_asc (
   char *out,
   char *in,
   signed16 isize)
{
        register signed16 i ;
        register char c ;
 
        for (i = 1; i <= isize; i++)
                if (i % 2 != 0)
			*out++ = ((c = (*in >> 4) & 0x0F) <= 9) ?
				   c + '\060' : c + '\067' ;
                else
			*out++ = ((c = *in++ & 0x0F) <= 9) ?
				   c + '\060' : c + '\067' ;
        *out = '\0' ;
}
 
 
 
 
 
/*exon ***********************************************************************/
/*                                                                           */
/*                         FUNCTION ascform ()                               */
/*                         -------------------                               */
/*                                                                           */
/*   Funktion zur Feststellung des ASCII-Formats eines Strings               */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   Autor: R. Horn                                     Date : 23.1.86       */
/*exoff **********************************************************************/
 
 
 d2_ret_val asc_form (
   char *in1,
   signed16 isize)
{
        register signed16 i ;
	register unsigned char *in = (unsigned char *) in1;
 
        for (i = 0; i < isize; i++, in++)
/*
                if (*in < 0x20 || *in > 0x7F)
*/
		if (!isprint(*in))
                        return (0) ;

        return (1) ;
}
 
