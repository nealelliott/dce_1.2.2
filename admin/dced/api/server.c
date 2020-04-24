/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: server.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:27:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:59:24  root]
 * 
 * Revision 1.1.2.5  1994/09/20  16:59:08  bowe
 * 	Fix bad loop variable usage in dce_server_inq_attr() [CR 11791,11240]
 * 	[1994/09/20  16:58:44  bowe]
 * 
 * Revision 1.1.2.4  1994/08/20  18:23:22  bowe
 * 	Fix botched indirection in dce_server_inq_attr() [CR 11791]
 * 	[1994/08/20  18:23:08  bowe]
 * 
 * Revision 1.1.2.3  1994/08/20  18:05:07  bowe
 * 	Add dce_server_inq_attr() [CR 11791]
 * 	[1994/08/20  18:01:36  bowe]
 * 
 * Revision 1.1.2.2  1994/06/09  16:02:59  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:37  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/12  19:30:22  rsalz
 * 	Add some dce_server functions.
 * 	Add and use dced_authorized
 * 	More srvr implementation; export their interfaces
 * 	[1994/05/12  19:26:21  rsalz]
 * 
 * $EndLog$
 */

#include <dcedimpl.h>

void
dce_server_inq_uuids(
	/* [out] */	uuid_t			*conf_entry_uuid,
	/* [out] */	uuid_t			*exec_entry_uuid,
	/* [out] */	error_status_t		*st
)
{
    char		*p;
    char		*q;

    if ((p = getenv(dced_uuid_env_name)) == NULL
     || (q = strchr(p, ' ')) == NULL) {
	*st = dced_s_not_started_by_dced;
	return;
    }

    *st = error_status_ok;
    if (conf_entry_uuid) {
	*q = '\0';
	uuid_from_string((idl_char *)p, conf_entry_uuid, st);
	*q = ' ';
	if (*st != error_status_ok)
	    return;
    }
    if (exec_entry_uuid)
	uuid_from_string((idl_char *)q + 1, exec_entry_uuid, st);
}

/* ______________________________________________________________________ */
/* Pass through the list of attributes in a server_t, finding match by UUID.
 * value pointer may be null. If so, you can tell if an attribute with
 * the given UUID is set.
 */

void
dce_server_inq_attr(
	/* in */	uuid_t			*attr_id,
	/* out */	sec_attr_t		*value,
	/* out */	error_status_t		*st
)
{
    server_t		*server;
    unsigned32		i;
    sec_attr_t		*ap;

    *st = error_status_ok;
    dce_server_inq_server(&server, st);
    if (*st != error_status_ok)
	return;
    for (i=server->attributes.count, ap=server->attributes.list; i; i--,ap++) {
	if (uuid_equal(&ap->attr_id, attr_id, st)) {
	    if (value)
		*value = *ap;
	    return;
	}
    }
    *st = dced_s_server_attr_not_found;
}

/* ______________________________________________________________________ */
