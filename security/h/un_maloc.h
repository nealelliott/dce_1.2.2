/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: un_maloc.h,v $
 * Revision 1.1.6.2  1996/02/18  23:01:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:00  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:30:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:34  root]
 * 
 * Revision 1.1.4.1  1994/04/01  20:08:20  jd
 * 	This module was relying on _CMA_PROTO_ to pick up stdlib.h. CMA_PROTO
 * 	no longer exists so set it as a temporary work around.
 * 	[1994/03/28  20:07:57  jd]
 * 
 * Revision 1.1.2.2  1992/12/29  13:28:09  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:16:19  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  un_maloc.h V=6 11/26/91 //nbs/dds_tools/src
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
**      Prototypes for unix malloc functions
*/

#ifndef unix_malloc_h__included
#define unix_malloc_h__included

/*  */

#ifdef OSF_DCE
#   include <krb5/osf_dce.h>
#endif
#define _CMA_PROTO_ 1
#if defined(__STDC__) || defined(_CMA_PROTO_)
#   include <stdlib.h>
    /* we trust that if you have __STDC__, you have stdlib.h */

#else

#   ifdef ultrix
#       include <malloc.h>
#   else
char * malloc ();

void free ();

char *realloc ();

char *calloc ();

char *alloca (); 
#   endif   /* ultrix */

#endif /* !__STDC__ */

#endif /* unix_malloc_h__included */
