/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Retrieving messages from the in-core message tables.
 */

/*
 * HISTORY
 * $Log: msggetdef.c,v $
 * Revision 1.1.10.3  1996/02/18  19:22:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:08  marty]
 *
 * Revision 1.1.10.2  1995/12/08  21:37:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:35  root]
 * 
 * Revision 1.1.6.1  1994/10/25  19:45:23  bowe
 * 	Handle untranslatable message=0 in dce_msg_get_default_msg(). [CR 12721]
 * 	[1994/10/25  19:43:44  bowe]
 * 
 * Revision 1.1.4.2  1994/06/09  16:06:02  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:36  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/17  21:17:07  rsalz
 * 	Make dce_msg_*() routines thread-safe (OT CR 9897)
 * 	[1994/03/09  21:19:41  rsalz]
 * 
 * Revision 1.1.2.3  1993/11/04  18:53:44  rsalz
 * 	Use error_status_t, not unsigned32, for status codes
 * 	[1993/11/04  18:52:46  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:08:35  rsalz
 * 	Initial release
 * 	[1993/08/16  18:03:28  rsalz]
 * 
 * $EndLog$
 */

#include <msgimpl.h>


/*
**  Get message from in-core tables.  Return static pointer.
*/
unsigned char *
dce_msg_get_default_msg(
    unsigned32			message,
    error_status_t		*status
)
{
    dce_msg_table_list_t	*tp;
    dce_msg_table_t		*mp;
    int				i;
    unsigned32			j;

    /* Map "OK" to a messages that says "Success".
     * (Message catalogs can not have a message numbered 0). */
    if (message == 0)
	message = msg_s_ok_text;

    /* Loop through all tables, looking only at ones in range. */
    for (tp = dce_msg_g_curr_table, i = dce_msg_g_table_size; --i >= 0; tp++) {
	if (message < tp->smallest || message > tp->biggest)
	    continue;

	for (mp = tp->table, j = tp->size; j >= 1; j--, mp++)
	    if (mp->message == message) {
		*status = msg_s_ok;
		return (unsigned char *)mp->text;
	    }
    }
    *status = msg_s_no_default;
    return NULL;
}
