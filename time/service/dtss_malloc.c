/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_malloc.c,v $
 * Revision 1.1.41.2  1996/02/17  23:35:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:45  marty]
 *
 * Revision 1.1.41.1  1995/12/08  18:13:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:47  root]
 * 
 * Revision 1.1.39.6  1994/08/16  18:13:01  cbrooks
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:09  cbrooks]
 * 
 * Revision 1.1.39.5  1994/05/16  21:24:48  cbrooks
 * 	CR 10642
 * 	[1994/05/16  21:16:35  cbrooks]
 * 
 * Revision 1.1.39.4  1994/05/16  18:51:45  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:18  rhw]
 * 
 * Revision 1.1.39.3  1994/04/21  22:05:59  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:59:44  cbrooks]
 * 
 * Revision 1.1.39.2  1994/01/21  22:06:32  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:29  rsalz]
 * 
 * Revision 1.1.2.2  1992/12/30  16:38:30  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:20:53  htf]
 * 
 * Revision 1.1  1992/01/19  15:34:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1989, 1990 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 */
#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>
#include <dce/dce.h>
#include <dcedtssvc.h>
#include <dcedtsmsg.h>
#include <dcedtsmac.h>
#include <stdlib.h>


/*
 *++
 *  DTSSmalloc()
 *	Does a malloc and then checks the return. If the malloc failed
 *	this routine logs an insufficient memory event and exits.
 *
 *	This routine and the corresponding DTSSfree are places to put
 *	debugging code for debugging mem management problems.
 *--
 */
pointer_t 
DTSSmalloc(
    unsigned int	size
)
{
    VOID		*mp;

    if ((mp = malloc (size)) == NULL) {
	dce_svc_printf(DTS_SERVER_NO_MEMORY_MSG);
    }
    return (pointer_t)mp;
}


/*
 *++
 *  DTSSfree()
 *	This routine and the corresponding DTSSmalloc are places to put
 *	debugging code for debugging mem management problems.
 *--
 */

void
DTSSfree(
	pointer_t mp
)
{
    free(mp);
}
