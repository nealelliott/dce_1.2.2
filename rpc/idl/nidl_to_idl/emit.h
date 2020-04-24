/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: emit.h,v $
 * Revision 1.1.4.2  1996/02/18  23:45:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:44  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:35:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:17:45  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:14:33  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:47:18  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:14:42  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:10:30  zeliff]
 * 
 * Revision 1.1  1992/01/19  02:59:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * emit.h
 */

#ifdef __STDC__
    extern void emit(char *idl_suffix, char *acf_suffix, binding_t *ast);
    extern void emit_parameters(parameter_t *parameter, NAMETABLE_id_t op_name);
#else
    extern void emit();
    extern void emit_parameters();
#endif

extern FILE *F, *AF;
