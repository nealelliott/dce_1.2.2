/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_stubs.c,v $
 * Revision 1.1.12.1  1996/10/02  17:24:14  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:31:53  damon]
 *
 * Revision 1.1.7.2  1994/06/09  14:00:26  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:41  annie]
 * 
 * Revision 1.1.7.1  1994/02/04  20:13:14  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:19  devsrc]
 * 
 * Revision 1.1.5.1  1994/01/28  20:43:39  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:14  annie]
 * 
 * 	expand OSF copyright
 * 
 * Revision 1.1.3.2  1993/08/13  16:48:36  kinney
 * 	<<<Empty log message>>>
 * 	[1993/08/11  20:15:28  kinney]
 * 
 * 	Initial revision.
 * 	[1993/04/12  18:31:00  toml]
 * 
 * $EndLog$
*/
#include <dcedfs/param.h>
#include <errno.h>

px_configure() {
	return(EINVAL);
}

cm_configure() {
	return(EINVAL);
}

