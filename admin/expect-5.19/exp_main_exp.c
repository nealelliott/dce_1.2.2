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
 * $Log: exp_main_exp.c,v $
 * Revision 1.1.2.1  1996/08/09  19:10:35  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:55:19  arvind]
 *
 * $EndLog$
 */

/* main.c - main() and some logging routines for expect

Written by: Don Libes, NIST, 2/6/90

Design and implementation of this program was paid for by U.S. tax
dollars.  Therefore it is public domain.  However, the author and NIST
would appreciate credit if this program or parts of it are used.
*/

#include <expect_cf.h>
#include <stdio.h>
#include <tcl.h>
#include <expect_tcl.h>

void
main(argc, argv)
int argc;
char *argv[];
{
	int rc = 0;
	Tcl_Interp *interp = Tcl_CreateInterp();

	if (Tcl_Init(interp) == TCL_ERROR) {
		fprintf(stderr,"Tcl_Init failed: %s\n",interp->result);
		exit(1);
	}

	if (Exp_Init(interp) == TCL_ERROR) {
		fprintf(stderr,"Exp_Init failed: %s\n",interp->result);
		exit(1);
	}

	exp_parse_argv(interp,argc,argv);

	/* become interactive if requested or "nothing to do" */
	if (exp_interactive)
		(void) exp_interpreter(interp);
	else if (exp_cmdfile)
		rc = exp_interpret_cmdfile(interp,exp_cmdfile);
	else if (exp_cmdfilename)
		rc = exp_interpret_cmdfilename(interp,exp_cmdfilename);

	/* assert(exp_cmdlinecmds != 0) */

	exp_exit(interp,rc);
	/*NOTREACHED*/
}

