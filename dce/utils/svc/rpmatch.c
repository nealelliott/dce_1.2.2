/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * DCE wrapper around XPG/4 rpmatch.  To be replaced by vendors.
 */

/*
 * HISTORY
 * $Log: rpmatch.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:19  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:37:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:48  root]
 * 
 * Revision 1.1.4.2  1994/07/27  12:28:01  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:34:43  rsalz]
 * 
 * Revision 1.1.4.1  1994/06/09  16:06:09  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:45  devsrc]
 * 
 * Revision 1.1.2.2  1993/08/16  18:09:08  rsalz
 * 	Initial release
 * 	[1993/08/16  18:04:02  rsalz]
 * 
 * $EndLog$
 */
#include <dce/dce.h>
#include <dce/dce_utils.h>

int
dce_rpmatch(char *s)
{
    switch (*s) {
    case 'y':
    case 'Y':
	return 1;
    case 'n':
    case 'N':
	return 0;
    }
    return -1;
}
