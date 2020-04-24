/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: exp_closetcl.c,v $
 * Revision 1.1.2.1  1996/08/09  21:09:29  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  21:08:49  arvind]
 *
 * $EndLog$
 */

/* exp_closetcl.c - close tcl files */

/* isolated in it's own file since it has hooks into Tcl and exp_clib user */
/* might like to avoid dragging it in */

#include <expect_cf.h>
#include <tclInt.h>

void (*exp_close_in_child)() = 0;

void
exp_close_tcl_files() {
	int i;

	/* So much for close-on-exec.  Tcl doesn't mark its files that way */
	/* everything has to be closed explicitly. */

	for (i=3; i<tclNumFiles;i++) close(i);
}
