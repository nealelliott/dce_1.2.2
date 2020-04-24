/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:59  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:50:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:19  root]
 * 
 * Revision 1.1.2.4  1992/12/31  17:35:03  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:59:25  htf]
 * 
 * Revision 1.1.2.3  1992/11/10  19:26:39  cuti
 * 	Conform to DCE1.1 coding standard: change __STDC__ to _DCE_PROTO_
 * 	[1992/11/10  18:20:40  cuti]
 * 
 * Revision 1.1.2.2  1992/11/10  16:09:37  cuti
 * 	Change _PROTOTYPE to __STDC__
 * 	[1992/11/10  14:47:18  cuti]
 * 
 * Revision 1.1  1992/01/19  03:41:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * tsh.h - header for tsh
 * 
 * Copyright 1989 by Apollo Computer, Inc.
 * Copyright 1990 by Locus Computing Corp.
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
 * @(#)tsh.h	1.6
 *  
 *	Extracted	1/7/91 10:19:07 
 * 	from delta on	1/3/91 14:16:51
 *
 */
#include <stdio.h>
          
#if defined(__STDC__)           /* other conditionals can be tested */
#define _DCE_PROTO_
#endif                          /* defined(__STDC__) */

#if defined(_DCE_PROTO_)
#define _DCE_PROTOTYPE_(arg) arg 
#else                           /* defined(_DCE_PROTO_) */
#define _DCE_PROTOTYPE_(arg) ()
#endif                          /* defined(_DCE_PROTO_) */

extern void	log _DCE_PROTOTYPE_((char *format, ...));
extern void	vlog _DCE_PROTOTYPE_((char *format, ...));

typedef struct searchlist {
	struct searchlist *	searchnext;
	char *				searchdir;
}	searchlist_t, *searchlist_p_t;

#define		MASKDEFAULT	1
#define		DOTTSHRC	".tshrc"
#define		ETCTSHRC	"/etc/tshrc"
