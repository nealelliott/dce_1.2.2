/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dbfree.c,v $
 * Revision 1.1.4.2  1996/02/18  19:20:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:07:23  root]
 * 
 * Revision 1.1.2.1  1994/01/24  22:01:38  bowe
 * 	Initial checkin
 * 	[1994/01/24  21:59:29  bowe]
 * 
 * Revision 1.1.2.1  1993/12/02  20:46:07  bowe
 * 	Initial revision.
 * 	[1993/12/02  20:44:47  bowe]
 * 
 * $EndLog$
 */

/* ______________________________________________________________________ 
 *
 *  Database / backing store entry free routine (call it after fetch).
 *  External interfaces:
 *	dce_db_free()
 */

#include <stdio.h>

#include <dce/uuid.h>
#include <dbimpl.h>

void
dce_db_free(
    dce_db_handle_t	handle,
    void		*data,
    error_status_t	*st
)
{
    /*
	Perhaps this is not needed at all.  We (the library) don't
	have access to what was allocated, so it will probably be up
	to the dce_db_* caller to free their own memory that we allocated.
    */
}

