/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Get a message from somewhere or abort.
 */

/*
 * HISTORY
 * $Log: msgget.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:06  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:37:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:33  root]
 * 
 * Revision 1.1.4.1  1994/06/09  16:06:00  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:35  devsrc]
 * 
 * Revision 1.1.2.3  1993/11/04  18:53:43  rsalz
 * 	Use error_status_t, not unsigned32, for status codes
 * 	[1993/11/04  18:52:45  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:08:30  rsalz
 * 	Initial release
 * 	[1993/08/16  18:03:23  rsalz]
 * 
 * $EndLog$
 */

#include <msgimpl.h>
#define DCE_ASSERT
#include <dce/assert.h>


/*
**  Get a message; abort on error.
*/
unsigned char *
dce_msg_get(
    unsigned32			message
)
{
    error_status_t		status;
    unsigned char		*p;

    p = dce_msg_get_msg(message, &status);
    if (p == NULL)
	dce___assert(NULL, "Could not retrieve message", "?unknown?", 0);
    return p;
}

