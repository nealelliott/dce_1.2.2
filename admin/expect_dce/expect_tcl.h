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
 * $Log: expect_tcl.h,v $
 * Revision 1.1.2.1  1996/08/09  19:19:02  arvind
 * 	Port to tcl_dce
 * 	[1996/06/18  13:26 UTC  psn  /main/psn_krb5/1]
 *
 * $EndLog$
 */

/* expect_tcl.h - include file for using the expect library, libexpect.a
with Tcl (and optionally Tk)

Written by: Don Libes, libes@cme.nist.gov, NIST, 12/3/90

Design and implementation of this program was paid for by U.S. tax
dollars.  Therefore it is public domain.  However, the author and NIST
would appreciate credit if this program or parts of it are used.

*/

#ifndef _EXPECT_TCL_H
#define _EXPECT_TCL_H

#include <expect_comm.h>

EXTERN int exp_cmdlinecmds;
EXTERN int exp_interactive;
EXTERN FILE *exp_cmdfile;
EXTERN char *exp_cmdfilename;
EXTERN int exp_getpid;	/* pid of Expect itself */
EXTERN int exp_buffer_command_input;

EXTERN int exp_tcl_debugger_available;

EXTERN Tcl_Interp *exp_interp;

EXTERN int	Exp_Init EXP_PROTO((Tcl_Interp *));	/* for Tcl_AppInit apps */
EXTERN void	exp_parse_argv EXP_PROTO((Tcl_Interp *,int argc,char **argv));
EXTERN int	exp_interpreter EXP_PROTO((Tcl_Interp *));
EXTERN int	exp_interpret_cmdfile EXP_PROTO((Tcl_Interp *,FILE *));
EXTERN int	exp_interpret_cmdfilename EXP_PROTO((Tcl_Interp *,char *));
EXTERN void	exp_interpret_rcfiles EXP_PROTO((Tcl_Interp *,int my_rc,int sys_rc));

EXTERN char *	exp_cook EXP_PROTO((char *s,int *len));

EXTERN void	exp_close_on_exec _ANSI_ARGS_((int));

			/* app-specific exit handler */
EXTERN void	(*exp_app_exit)EXP_PROTO((Tcl_Interp *));
EXTERN void	exp_exit EXP_PROTO((Tcl_Interp *,int status));
EXTERN void	exp_exit_handlers EXP_PROTO((Tcl_Interp *));

#if 0
#if TCL_MAJOR_VERSION < 7 || (TCL_MAJOR_VERSION == 7 && TCL_MINOR_VERSION < 4)
EXTERN void	exp_error EXP_PROTO(VARARGS);
#else
#  if TCL_MAJOR_VERSION == 7 && TCL_MINOR_VERSION == 4
EXTERN void	exp_error EXP_PROTO(VARARGS(Tcl_Interp *));
#  else
EXTERN void	exp_error EXP_PROTO(VARARGS(Tcl_Interp *,interp));
#  endif
#endif
#endif /* 0 */

#endif /* _EXPECT_TCL_H */
