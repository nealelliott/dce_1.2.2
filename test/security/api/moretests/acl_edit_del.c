/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl_edit_del.c,v $
 * Revision 1.1.4.2  1996/02/17  23:25:13  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:24  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:48:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:05  root]
 * 
 * Revision 1.1.2.3  1993/02/25  22:05:25  cuti
 * 	Change sec_rgy_site_open() to sec_rgy_site_open_update() for using site update operation.
 * 	[1993/02/25  18:56:08  cuti]
 * 
 * Revision 1.1.2.2  1992/12/31  17:28:12  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:53:37  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* 
*/ 
/* 
 * Copyright 1991 by Apollo Computer, Inc.
 * 
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Apollo Computer, Inc not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Apollo Computer, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 * 
 *
 *   
 *	Extracted	10/14/91 15:55:47
 *	from delta on	8/5/91 14:44:02
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/acl_edit_del.c, security.test, dce_osf";
#endif

/* Include files */

#include <stdio.h>
#include <tsh_common.h>
#include <dce/rgybase.h>

main (argc, argv)

/* Formal parameters */

   ndr_long_int      argc;
   unsigned_char_p_t argv[];

{

/* Local variables */

   sec_rgy_pname_t	node_name;
   sec_rgy_pname_t	site_name;	/* sec_rgy_site_open_update */
   sec_rgy_handle_t	context;	/* sec_rgy_site_open_update */
   sec_rgy_name_t	acl_obj_name;	/* sec_rgy_pgo_delete */
   error_status_t	tstatus;	/* test status */


/* Obtain the input arguments from the acl_edit_delete command line. */

   memcpy ((char *)acl_obj_name, (char *)argv[0], sizeof (sec_rgy_name_t));

/* Obtain the site name for rgyd. */

   gethostname (node_name, sec_rgy_pname_t_size);
   sprintf ((char *)site_name, "ip:%s", node_name);
   
/* Bind to a registry server. */

   sec_rgy_site_open_update (site_name,
                      &context,
                      &tstatus);

   if (tstatus != rpc_s_ok)
     exit (tstatus);

/* Delete the test person item from the registry. */

   sec_rgy_pgo_delete (context,
                       (sec_rgy_domain_t)sec_rgy_domain_person,
                       acl_obj_name,
                       &tstatus);

/* Release the context handle. */

   sec_rgy_site_close (context, &tstatus);

/* Exit this routine. */

   exit (tstatus);

} /* End acl_edit_delete */

