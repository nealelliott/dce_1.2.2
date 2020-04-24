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
 * Revision 1.1.4.2  1996/02/18  00:16:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:46:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:08  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:29:20  agd
 * 	expand copyright
 * 	[1994/10/06  14:27:49  agd]
 * 
 * Revision 1.1.2.1  1994/08/09  17:32:39  burati
 * 	DFS/EPAC/KRPC/dfsbind changes
 * 	[1994/08/09  17:01:52  burati]
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
    kmalloc(size, M_DYNAMIC, M_WAITOK | M_ALIGN);
}
	
char *mkfree( char *a )
{
    kfree( a, M_DYNAMIC );
}
