/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl_edit_add.c,v $
 * Revision 1.1.4.2  1996/02/17  23:25:12  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:23  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:48:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:04  root]
 * 
 * Revision 1.1.2.3  1993/02/25  22:05:15  cuti
 * 	Change sec_rgy_site_open() to sec_rgy_site_open_update() for using site update operation.
 * 	[1993/02/25  16:30:12  cuti]
 * 
 * Revision 1.1.2.2  1992/12/31  17:28:08  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:53:33  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:23  devrcs
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
 *	from delta on	8/5/91 14:43:56
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/acl_edit_add.c, security.test, dce_osf";
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
   sec_rgy_name_t	acl_obj_name;	/* sec_rgy_pgo_add */
   sec_rgy_pgo_item_t	pgo_item;	/* sec_rgy_pgo_add */
   error_status_t	tstatus;	/* test status */


/* Obtain the input arguments from the acl_edit_add command line. */

   strcpy ((char *)acl_obj_name, (char *)argv[0]);

/* Obtain the site name for rgyd. */

   gethostname (node_name, sec_rgy_pname_t_size);
   sprintf ((char *)site_name, "ip:%s", node_name);
   
/* Bind to a registry server. */

   sec_rgy_site_open_update (site_name,
                      &context,
                      &tstatus);

   if (tstatus != rpc_s_ok)
     exit (tstatus);

/* Add a test person item to the registry. */

   bzero ((char *)&pgo_item, sizeof (pgo_item));

   pgo_item.id;
   pgo_item.unix_num = 5002; 
   pgo_item.quota = 1;
   pgo_item.flags = sec_rgy_pgo_flags_none;

   memcpy ((char *)pgo_item.fullname,
	   (char *)"marco's test person item\0",
	   sizeof (pgo_item.fullname));

   sec_rgy_pgo_add (context,
                    (sec_rgy_domain_t)sec_rgy_domain_person,
		    acl_obj_name,
		    &pgo_item,
                    &tstatus);

/* Release the context handle. */

   sec_rgy_site_close (context, &tstatus);

/* Exit this routine. */

   exit (tstatus);

} /* End acl_edit_add */

