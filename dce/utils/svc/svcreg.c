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
 * Register a component with serviceability.
 */

/*
 * HISTORY
 * $Log: svcreg.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:29  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:38:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:10:14  root]
 * 
 * Revision 1.1.4.6  1994/09/13  18:11:40  rrizzo
 * 	Added missing first line to copyright text.
 * 	[1994/09/13  17:55:33  rrizzo]
 * 
 * Revision 1.1.4.5  1994/08/30  20:42:02  marty
 * 	CR 11923 - Change copyright to a public domain copyright.
 * 	[1994/08/30  20:34:34  marty]
 * 
 * Revision 1.1.4.4  1994/07/27  12:28:05  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:35:17  rsalz]
 * 
 * Revision 1.1.4.3  1994/07/22  18:06:20  rsalz
 * 	Init all fields in dce_svc_register (OT CR 11397).
 * 	[1994/07/22  18:04:00  rsalz]
 * 
 * Revision 1.1.4.2  1994/07/08  17:11:52  bowe
 * 	Add call to dce_svc_init() [CR 11156]
 * 	[1994/07/08  16:34:26  bowe]
 * 
 * Revision 1.1.4.1  1994/06/09  16:06:33  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:03  devsrc]
 * 
 * Revision 1.1.2.3  1993/11/23  20:14:28  rsalz
 * 	Make lock in dce_svc_register() finer grain,
 * 	thanks to <sekhar@apollo.hp.com>.
 * 	[1993/11/23  20:14:16  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:10:22  rsalz
 * 	Initial release
 * 	[1993/08/16  18:04:56  rsalz]
 * 
 * $EndLog$
 */

#include <svcimpl.h>


/*
**  Mapping of sub-component tables to serviceability handles.
*/
dce_svc_handle_t	*dce_svc_g_map;
int			dce_svc_g_map_size;


/*
**  Add a handle/table map.  Return FALSE and fill in *status on error.
*/
boolean
dce_svc__add_to_table(
    dce_svc_handle_t		h,
    error_status_t		*status
)
{
    if (h->intable)
	return TRUE;
    if (dce_svc_g_map_size == 0) {
	dce_svc_g_map = (dce_svc_handle_t *)malloc(1 * sizeof *dce_svc_g_map);
    }
    else
	dce_svc_g_map = (dce_svc_handle_t *)realloc(dce_svc_g_map,
			    (dce_svc_g_map_size + 1) * sizeof *dce_svc_g_map);
    if (dce_svc_g_map == NULL) {
	*status = svc_s_no_memory;
	return FALSE;
    }
    dce_svc_g_map[dce_svc_g_map_size++] = h;
    h->intable = TRUE;
    return TRUE;
}


/*
**  Find the handle that has this table.
*/
dce_svc_handle_t
dce_svc__table_to_handle(
    dce_svc_subcomp_t		*table
)
{
    dce_svc_handle_t		*hp;
    int				i;

    for (i = dce_svc_g_map_size, hp = dce_svc_g_map; --i >= 0; hp++)
	if (hp[0]->table == table)
	    return hp[0];
    return NULL;
}


dce_svc_handle_t
dce_svc__component_to_handle(
    unsigned char		*comp
)
{
    dce_svc_handle_t		*hp;
    int				i;

    for (i = dce_svc_g_map_size, hp = dce_svc_g_map; --i >= 0; hp++)
	if (strcmp(hp[0]->component_name, (char *)comp) == 0)
	    return hp[0];
    return NULL;
}


dce_svc_handle_t
dce_svc_register(
    dce_svc_subcomp_t		*table,
    const idl_char		*component_name,
    error_status_t		*status
)
{
    static struct dce_svc_handle_s_t	nilhandle;
    dce_svc_handle_t		h;

    /* Get space. */
    if ((h = (dce_svc_handle_t)malloc(1 * sizeof *h)) == NULL) {
	*status = svc_s_no_memory;
	return NULL;
    }

    /* Fill in fields. */
    *h = nilhandle;
    h->allocated = TRUE;
    h->table = table;
    h->component_name = dce_strdup((char *)component_name);

    /* Add to table. */
    LOCK();
    if (!dce_svc__add_to_table(h, status)) {
	free(h->component_name);
	free(h);
	UNLOCK();
	return NULL;
    }

    UNLOCK();
    *status = svc_s_ok;
    return h;
}


/*
**  Close all routes and free whatever we can.
*/
void
dce_svc_unregister(
    dce_svc_handle_t		h,
    error_status_t		*status
)
{
    dce_svc_handle_t		*hp;
    int				i;

    LOCK();
    dce_svc__close_routes(&h->routes);

    /* Remove from table. */
    if (h->intable)
	for (i = dce_svc_g_map_size, hp = dce_svc_g_map; --i >= 0; hp++)
	    if (hp[0] == h) {
		/* Found element, move everyone else down. */
		while (--i >= 0)
		    hp[0] = hp[1];
		dce_svc_g_map_size--;
		dce_svc_g_map = (dce_svc_handle_t *)realloc(dce_svc_g_map,
				    dce_svc_g_map_size * sizeof *dce_svc_g_map);
		break;
	    }

    /* We can't free translated descriptions since we don't store
     * an "allocated" flag for them.  Oh well. */

    if (h->allocated) {
	free(h->component_name);
	free(h);
    }

    *status = svc_s_ok;
    UNLOCK();
}


/*
**  Shut down all serviceability routes.
*/
void
dce_svc__shutdown(void)
{
    int			i;
    dce_svc_handle_t	*hp;

    for (i = 0; i < sizeof dce_svc_g_code_table / sizeof dce_svc_g_code_table[0]; i++)
	dce_svc__close_routes(&dce_svc_g_routes[i]);

    for (i = dce_svc_g_map_size, hp = dce_svc_g_map; --i >= 0; hp++)
	dce_svc__close_routes(&hp[0]->routes);
}


/*
**  Translate the messages if not already done so.
*/
void
dce_svc__translate(
    dce_svc_handle_t		h,
    error_status_t		*status
)
{
    dce_svc_subcomp_t		*tp;
    dce_msg_cat_handle_t	c;
    unsigned char		*p;
    unsigned32			typical;

    if (h->translated)
	return;
    h->translated = TRUE;

    /* Get the message catalog. */
    for (typical = 0, tp = h->table; tp->sc_name; tp++)
	if ((typical = tp->sc_descr_msgid) != 0)
	    break;
    if (typical == 0)
	return;
    c = dce_msg_cat_open(typical, status);
    if (c == NULL)
	return;

    for (tp = h->table; tp->sc_name; tp++)
	if (tp->sc_descr_msgid != 0) {
	    p = dce_msg_cat_get_msg(c, tp->sc_descr_msgid, status);
	    if (p != NULL)
		tp->sc_descr = (idl_char *)dce_strdup((char *)p);
	}
    dce_msg_cat_close(c, status);
}


/*
**  Initialize svc stuff for this thread.
**  Right now, this is only needed for code that does binary logfile
**  manipulation outside of a server-side RPC operation.
*/
void
dce_svc_init(
    unsigned32		flags,
    error_status_t	*status
)
{
    *status = svc_s_ok;
    rpc_sm_enable_allocate(status);
}

