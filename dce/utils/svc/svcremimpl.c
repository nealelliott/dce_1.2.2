/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Low-level serviceability code to implement remote operations.
 */

/*
 * HISTORY
 * $Log: svcremimpl.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:30  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:38:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:10:16  root]
 * 
 * Revision 1.1.4.4  1994/09/29  20:24:04  bowe
 * 	Return all production routes in dce_svc_routings() [OT 11542]
 * 	[1994/09/29  20:23:31  bowe]
 * 
 * Revision 1.1.4.3  1994/07/27  12:28:05  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:35:26  rsalz]
 * 
 * Revision 1.1.4.2  1994/06/09  16:06:34  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:04  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/19  20:01:27  rsalz
 * 	Initialze output parameters for dce_svc_table (OT CR 10366).
 * 	[1994/04/18  01:37:29  rsalz]
 * 
 * Revision 1.1.2.3  1993/09/02  17:42:58  rsalz
 * 	Change dce_svc_routings to take a flag indicating if it
 * 	should return standard routings or debug settings.
 * 	Turn dce_svc_g_memory into an array of structures that keep
 * 	the text and an "is a debug message" indicator.
 * 	[1993/09/02  17:42:41  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:10:27  rsalz
 * 	Initial release
 * 	[1993/08/16  18:04:59  rsalz]
 * 
 * $EndLog$
 */

#include <svcimpl.h>
#include <dce/svcremote.h>


void
dce_svc_components(
    dce_svc_stringarray_t	*table,
    error_status_t		*status
)
{
    int				i;
    dce_svc_handle_t		*hp;
    dce_svc_string_t		*cp;

    *status = svc_s_ok;
    LOCK();
    if ((table->tab_size = dce_svc_g_map_size) == 0) {
	table->table = NULL;
	UNLOCK();
	return;
    }

    table->table = cp = rpc_ss_allocate(table->tab_size * sizeof *cp);
    for (hp = dce_svc_g_map, i = 0; i < dce_svc_g_map_size; i++)
	*cp++ = (dce_svc_string_t)(*hp++)->component_name;
    UNLOCK();
}


void
dce_svc_table(
    dce_svc_string_t		component,
    dce_svc_subcomparray_t	*table,
    error_status_t		*status
)
{
    dce_svc_handle_t	h;
    int			i;
    dce_svc_subcomp_t	*tp;

    if ((h = dce_svc__component_to_handle(component)) == NULL) {
	*status = svc_s_unknown_component;
	table->tab_size = 0;
	table->table = NULL;
	return;
    }

    for (i = 0, tp = h->table; tp->sc_name; tp++, i++)
	continue;

    *status = svc_s_ok;
    table->tab_size = i;
    table->table = h->table;
}


void
dce_svc_filter(
    dce_svc_string_t		component,
    idl_long_int		arg_size,
    idl_byte			*argument,
    error_status_t		*status
)
{
    dce_svc_handle_t		h;
    dce_svc_filterctl_proc_t	fp;

    if ((h = dce_svc__component_to_handle(component)) == NULL) {
	*status = svc_s_unknown_component;
	return;
    }
    if ((fp = (dce_svc_filterctl_proc_t)h->filterctl) == NULL) {
	*status = svc_s_no_filter;
	return;
    }

    *status = svc_s_ok;
    (*fp)(arg_size, argument, status);
}

#define sizeof_code_table \
	(sizeof dce_svc_g_code_table / sizeof dce_svc_g_code_table[0])

void
dce_svc_routings(
    dce_svc_stringarray_t	*table,
    boolean32			debug_values,
    error_status_t		*status
)
{
    dce_svc_string_t		*cp;
    dce_svc_memory_t		*mp;
    int				i;
    int				ti, len;
    const char			**tp;
    char			*p;
    char			buf[SMBUF];

    LOCK();
    *status = svc_s_ok;

    table->table = NULL;
    if (debug_values) {		/* debug routes */
	table->tab_size = 0;
	for (mp = dce_svc_g_memory, i = dce_svc_g_memory_size; --i >= 0; mp++)
	    if (mp->is_debug) table->tab_size++;
	if (table->tab_size == 0) {
	    /* no debug entries to return */
	    UNLOCK();
	    return;
	}
	table->table = cp = rpc_sm_allocate(table->tab_size * sizeof *cp,
		status);
	if (*status != error_status_ok) {
	    UNLOCK();
	    return;
	}
	for (mp = dce_svc_g_memory, i = dce_svc_g_memory_size; --i >= 0; mp++)
	    if (mp->is_debug) *cp++ = (dce_svc_string_t)mp->text;
    }
    else {			/* production routes */
	table->tab_size = sizeof_code_table;
	table->table = cp = rpc_sm_allocate(table->tab_size * sizeof *cp,
		status);
	if (*status != error_status_ok) {
	    UNLOCK();
	    return;
	}
	for (ti=sizeof_code_table, tp=dce_svc_g_code_table; --ti >= 0;
		tp++, cp++) {
	    len = strlen(*tp);
	    *cp = NULL;
	    /* check in-memory table */
	    for (mp=dce_svc_g_memory, i=dce_svc_g_memory_size; --i >= 0; mp++)
		if ( !mp->is_debug && !strncmp(mp->text, *tp, len) ) {
		    *cp = (dce_svc_string_t)mp->text;
		    break;
		}
	    if (*cp) continue;

	    /* try environment var */
	    (void)sprintf(buf, SVC_ROUTE_ENVVAR, *tp);
	    for (p = buf; *p; p++)
		if (islower(*p))
		    *p = toupper(*p);
	    if ((p = getenv(buf)) == NULL) {
		/* not set - now try the routing file */
		if ((p = dce_svc__linefor(*tp)) == NULL) {
		    /* not set - settle for the default */
		    if (!strcmp(*tp, "NOTICE") ||
				!strcmp(*tp, "NOTICE_VERBOSE") )
			p = "DISCARD:";
		    else
			p = "STDERR:";
		}
	    }

	    /* allocate storage for the string, then write it */
	    *cp = rpc_sm_allocate(strlen(*tp) + 1 + strlen(p) + 1, status);
	    sprintf((char *)*cp, "%s:%s", *tp, p);
	}
    }

    UNLOCK();
}
