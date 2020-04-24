/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kmimpl.h,v $
 * Revision 1.1.2.2  1996/02/18  23:32:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:24  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:48:58  root]
 * 
 * Revision 1.1.2.4  1994/08/24  19:27:03  bowe
 * 	rkeytab__get_keytab_filename() prototype. [CR 11791]
 * 	[1994/08/24  19:25:21  bowe]
 * 
 * Revision 1.1.2.3  1994/05/10  21:08:49  bowe
 * 	Add symbols for size of lists to allocate
 * 	[1994/05/10  21:06:58  bowe]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:06  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:56:01  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:54  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:23  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#ifndef _KMIMPL_H
#define _KMIMPL_H

#include <dce/dce.h>
#include <dce/sec_login.h>
#include <dce/keymgmt.h>

/* For allocating chunks of pointers at a time, reducing need for realloc() */
#define dced_km_c_list_size_init	20
#define dced_km_c_list_size_incr	20

/* prototypes */
extern void rkeytab__get_keytab_filename(uuid_t *, dced_string_t *, error_status_t *);


#endif
