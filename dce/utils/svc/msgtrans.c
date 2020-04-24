/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Translate an array of messages.
 */

/*
 * HISTORY
 * $Log: msgtrans.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:16  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:37:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:43  root]
 * 
 * Revision 1.1.4.3  1994/07/27  12:27:59  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:34:25  rsalz]
 * 
 * Revision 1.1.4.2  1994/06/09  16:06:06  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:41  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/17  21:17:11  rsalz
 * 	Make dce_msg_*() routines thread-safe (OT CR 9897)
 * 	[1994/03/09  21:19:43  rsalz]
 * 
 * Revision 1.1.2.3  1993/11/04  18:53:45  rsalz
 * 	Use error_status_t, not unsigned32, for status codes
 * 	[1993/11/04  18:52:48  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:08:58  rsalz
 * 	Initial release
 * 	[1993/08/16  18:03:52  rsalz]
 * 
 * $EndLog$
 */

#include <msgimpl.h>


/*
**  Get a message; abort on error.
*/
void
dce_msg_translate_table(
    dce_msg_table_t		*table,
    unsigned32			size,
    error_status_t		*status
)
{
    dce_msg_table_t		*tp;
    char			**new;
    char			**np;
    unsigned32			i;
    char			*p;

    /* Get space for new strings.. */
    if ((new = (char **)malloc(size * sizeof *new)) == NULL) {
	*status = msg_s_no_memory;
	return;
    }

    LOCK();

    /* Translate each message. */
    for (np = new, tp = table, i = 0; i < size; i++, tp++, np++) {
	p = (char *)dce_msg_get_cat_msg(table[i].message, status);
	if (p != NULL) {
	    *np = dce_strdup(p);
	    continue;
	}

	/* Failed; undo everything. */
	UNLOCK();
	while (np > new)
	    free(*--np);
	free(new);
	return;
    }

    UNLOCK();

    /* All messages translated; flip pointers, free space, and return. */
    for (np = new, tp = table, i = 0; i < size; i++, tp++, np++)
	tp->text = *np;
    free(new);
    *status = msg_s_ok;
}
