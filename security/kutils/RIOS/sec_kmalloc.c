/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_kmalloc.c,v $
 * Revision 1.1.4.2  1996/02/18  00:16:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:46:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:09  root]
 * 
 * Revision 1.1.2.3  1994/10/06  20:29:32  agd
 * 	expand copyright
 * 	[1994/10/06  14:27:54  agd]
 * 
 * Revision 1.1.2.2  1994/08/11  14:38:45  rsarbo
 * 	use xmfree, not xfree
 * 	[1994/08/11  14:38:15  rsarbo]
 * 
 * Revision 1.1.2.1  1994/08/09  17:32:43  burati
 * 	DFS/EPAC/KRPC/dfsbind changes
 * 	[1994/08/09  17:01:27  burati]
 * 
 * $EndLog$
 */

/* sec_kmalloc.c
 * Internal kernel malloc support routines for kernel security modules.
 *
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

#include <sys/malloc.h>

/* Wrapper for kmalloc and kfree */
char *mkmalloc( unsigned int size )
{
    xmalloc( size, 0, kernel_heap );
}
	
char *mkfree( char *a )
{
    xmfree( a, kernel_heap );
}
