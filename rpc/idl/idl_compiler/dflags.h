/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: dflags.h,v $
 * Revision 1.1.9.2  1996/03/09  20:44:52  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:34  marty]
 *
 * Revision 1.1.9.1  1995/12/07  22:22:46  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:03 UTC  dat
 * 	New file for second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:13:21  root]
 * 
 * Revision 1.1.2.1  1995/10/31  21:15:29  bfc
 * 	may 5 version from nightly
 * 	[1995/10/31  18:49:02  bfc]
 * 
 * $EndLog$
 */
/*
**  NAME:
**
**      dflags.h
**
**  FACILITY:
**
**      IDL Compiler Backend
**
**  ABSTRACT:
**
**  Header for dflags.c
**
**  VERSION: DCE 1.0
**
*/

#ifndef DFLAGS_H
#define DFLAGS_H

boolean BE_flag_sync
(
#ifdef PROTO
    AST_parameter_n_t *params,
    boolean sync_next
#endif
);

void BE_flag_advance_mp
(
#ifdef PROTO
    AST_parameter_n_t *params,
    BE_side_t side,
    BE_direction_t direction,
    boolean force
#endif
);

int BE_set_alignment
(
#ifdef PROTO
    AST_parameter_n_t *param,
    int initial_alignment
#endif
);

int BE_flag_alignment
(
#ifdef PROTO
    AST_parameter_n_t *params,
    int alignment
#endif
);

int BE_flag_check_buffer
(
#ifdef PROTO
    AST_parameter_n_t *params,
    int count,
    long * current_alignment
#endif
);

boolean BE_any_pointers
(
#ifdef PROTO
    AST_parameter_n_t *params
#endif
);

boolean BE_any_ref_pointers
(
#ifdef PROTO
    AST_parameter_n_t *params
#endif
);

void BE_set_operation_flags
(
#ifdef PROTO
    AST_operation_n_t *oper
#endif
);

#endif
