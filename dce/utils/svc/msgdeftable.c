/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993, 1994, 1996 Open Software Foundation, Inc.
 * Permission is hereby granted to use, copy, modify and freely distribute  
 * the software in this file and its documentation for any purpose without  
 * fee, provided that the above copyright notice appears in all copies and  
 * that both the copyright notice and this permission notice appear in  
 * supporting documentation.  Further, provided that the name of Open  
 * Software Foundation, Inc. ("OSF") not be used in advertising or  
 * publicity pertaining to distribution of the software without prior  
 * written permission from OSF.  OSF makes no representations about the  
 * suitability of this software for any purpose.  It is provided "as is"  
 * without express or implied warranty.
 * 
 * Adding to the in-core message tables.
 */

/*
 * HISTORY
 * $Log: msgdeftable.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:05  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:37:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:32  root]
 * 
 * Revision 1.1.4.3  1994/09/13  18:03:43  rrizzo
 * 	Added missing first line to copyright text.
 * 	[1994/09/13  17:58:45  rrizzo]
 * 
 * Revision 1.1.4.2  1994/08/30  20:41:58  marty
 * 	CR 11923 - Change copyright to a public domain copyright.
 * 	[1994/08/30  20:34:31  marty]
 * 
 * Revision 1.1.4.1  1994/06/09  16:05:58  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:33  devsrc]
 * 
 * Revision 1.1.2.3  1993/11/04  18:53:42  rsalz
 * 	Use error_status_t, not unsigned32, for status codes
 * 	[1993/11/04  18:52:44  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:08:26  rsalz
 * 	Initial release
 * 	[1993/08/16  18:03:18  rsalz]
 * 
 * $EndLog$
 */

#include <msgimpl.h>


/*
**  Add a program-specific message table to the in-core table list.
*/
void
dce_msg_define_msg_table(
    dce_msg_table_t		*table,
    unsigned32			size,
    error_status_t		*status
)
{
    dce_msg_table_list_t	*new;

    LOCK();
    /* Grow table. */
    new = (dce_msg_table_list_t *)
		malloc((dce_msg_g_table_size + 1) * sizeof *new);
    if (new == NULL) {
	*status = msg_s_no_memory;
	UNLOCK();
	return;
    }
    (void)memcpy(new, dce_msg_g_curr_table, dce_msg_g_table_size * sizeof *new);

    /* If old table was allocated, free it; update pointer. */
    if (dce_msg_g_curr_table != dce_msg_g_compiled_table)
	free(dce_msg_g_curr_table);
    dce_msg_g_curr_table = new;

    /* Fill in the new entry. */
    new = &dce_msg_g_curr_table[dce_msg_g_table_size++];
    new->smallest = new->biggest = table[0].message;
    for (new->table = table, new->size = size; size; size--, table++) {
	if (table->message < new->smallest)
	    new->smallest = table->message;
	if (table->message > new->biggest)
	    new->biggest = table->message;
    }
    *status = msg_s_ok;
    UNLOCK();
}
