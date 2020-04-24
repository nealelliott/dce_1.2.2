/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: error_table.h,v $
 * Revision 1.1.4.2  1996/02/18  23:01:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:18  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:37  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:55:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:50:45  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:53:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  error_table.h V=2 10/23/91 //littl/prgy/krb5/comerr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * Copyright 1988 by the Student Information Processing Board of the
 * Massachusetts Institute of Technology.
 *
 * For copyright info, see mit-sipb-cr.h.
 */

#ifndef _ET_H
/* Are we using ANSI C? */
#ifndef __STDC__
#define const
#endif
extern int errno;
struct error_table {
    char const * const * msgs;
    long base;
    int n_msgs;
};
struct et_list {
    struct et_list *next;
    const struct error_table *table;
};
extern struct et_list * _et_list;

#define	ERRCODE_RANGE	8	/* # of bits to shift table number */
#define	BITS_PER_CHAR	6	/* # bits to shift per character in name */

extern const char *error_table_name();
#define _ET_H
#endif
