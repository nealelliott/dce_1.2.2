/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util.h,v $
 * Revision 1.1.4.2  1996/02/18  23:46:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:58  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:37:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:18:44  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:34:10  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:48:55  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:34:13  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:14:33  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:00:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * util.h
 */

#ifdef __STDC__
extern char *get_name(NAMETABLE_id_t name);
extern char *get_string(STRTAB_str_t string);
extern char *get_family(unsigned long family);
extern void spell_typed_name(type_t *typeb, NAMETABLE_id_t name);
extern void pointerize_routine (routine_t *rp);
extern void print_comments (comment_t *c);
#else
extern char *get_name();
extern char *get_string();
extern char *get_family();
extern void spell_typed_name();
extern void pointerize_routine();
extern void print_comments();
#endif

extern boolean no_warnings;

#define warn(mesg)    if (!no_warnings) fprintf(stderr, "*** Warning: %s\n", mesg)
#define Is_subrange_kind(type) ((type)->kind == small_subrange_k || (type)->kind == short_subrange_k || \
                                (type)->kind == long_subrange_k)
#define Has_type_attrs(type) \
    ((type)->is_handle || (type)->kind == user_marshalled_k || \
    (type)->kind == fixed_array_k || (type)->kind == open_array_k || \
    (type)->kind == fixed_string_zero_k)

#define Has_routine_attrs(routine) ((routine)->idempotent || (routine)->maybe || (routine)->secure || \
                                    (routine)->broadcast)
#define Assert(cond) if (!cond) \
                        fprintf(stderr, "Assertion %s failed at line %d of %s\n", "cond", __LINE__, __FILE__)

#define SMALL_BUFFER    256
#define LARGE_BUFFER    1024
#define HYPER_BUFFER    4096
#define OUTPUT_COMMENTS(x) {print_comments(x); x = NULL;}
