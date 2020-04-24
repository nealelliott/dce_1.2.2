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
 * $Log: exp_event.h,v $
 * Revision 1.1.2.1  1996/08/09  19:09:15  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:54:11  arvind]
 *
 * $EndLog$
 */

/* exp_event.h - event definitions */

int exp_get_next_event _ANSI_ARGS_((Tcl_Interp *,int *, int, int *, int, int));
int exp_get_next_event_info _ANSI_ARGS_((Tcl_Interp *, int, int));
int exp_dsleep _ANSI_ARGS_((Tcl_Interp *, double));
void exp_init_event _ANSI_ARGS_((void));

extern void (*exp_event_exit) _ANSI_ARGS_((Tcl_Interp *));

void exp_event_disarm _ANSI_ARGS_((int));

void exp_arm_background_filehandler _ANSI_ARGS_((int));
void exp_disarm_background_filehandler _ANSI_ARGS_((int));
void exp_disarm_background_filehandler_force _ANSI_ARGS_((int));
void exp_unblock_background_filehandler _ANSI_ARGS_((int));
void exp_block_background_filehandler _ANSI_ARGS_((int));

void exp_background_filehandler _ANSI_ARGS_((ClientData,int));

