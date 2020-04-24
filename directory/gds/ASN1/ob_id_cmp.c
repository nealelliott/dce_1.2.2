/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ob_id_cmp.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:48  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:07:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:19  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:56  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:10  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:27:14  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:54:57  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:43:47  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  17:01:30  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:36:34  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:52:58  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:19:19  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:19:32  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ob_id_cmp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:56 $";
#endif

 
/* ****************************************************************** */
/*                                                                    */
/*   COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONSSYSTEME AG   */
/*                           AND INRIA                                */
/*   All rights reserved                                              */
/*                                                                    */
/*   This software is furnished under licence and may be used only    */
/*   in accordance with the terms of that licence and with the        */
/*   inclusion of the above copyright notice.                         */
/*   This software may not be provided or otherwise made available    */
/*   to, or used by, any other person. No title to or ownership of    */
/*   the software is hereby  transferred.                             */
/*                                                                    */
/* ****************************************************************** */

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <string.h>
#include <dce/asn1.h>

/*--------------------------------------------------------------------------
  function_name : asn1_ob_id_cmp
  description   : returns 0 if the two asn1 object identifiers are identical;
--------------------------------------------------------------------------*/

int asn1_ob_id_cmp (id1, id2)
asn1_object_id *id1, *id2;
{		
	if ((id1->nb == 0) && (id2->nb == 0))
		return (0);
	if ((id1->contents == 0) && (id2->contents == 0))
		return (0);
	if (id1->contents && id2->contents)
		return  (  (!(id1->nb == id2->nb)) ||
	        	   (memcmp ((char *)id1->contents, 
				   (char *)id2->contents, 
			           id1->nb)));
	return (-1);
}
