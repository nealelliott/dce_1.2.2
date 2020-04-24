/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bstring.h,v $
 * Revision 1.1.5.2  1996/02/18  23:02:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:27  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:40:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:49  root]
 * 
 * Revision 1.1.3.3  1992/12/29  13:57:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:51:26  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:18:43  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:32:58  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:49:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  bstring.h V=2 02/06/91 //littl/prgy/krb5/include
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* bstring.h, /us/lib/rgy/src, pato, 08/22/89
**      Prototypes for byte string manipulation functions
** 
** Copyright Hewlett-Packard Company 1989, 1990
**  Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
**  VersionID = @(#)bstring.h	2 - 02/06/91
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

#ifndef bstring_h__included
#define bstring_h__included

#include "Pointer.h"

#ifdef SNI_SVR4_POSIX
#include <string.h>

#define bcopy(src,dst,len)	memmove((dst),(src),(len))
#define bzero(dst,len)		(void) memset((dst),0,(len))
#define bcmp(b1,b2,len)		memcmp((b1),(b2),(len))
#define index(c, i)		strchr((c), (i))
#define rindex(c, i)		strrchr((c), (i))

#else
#ifdef __STDC__
#   include <string.h>
#   if defined(BSD) || defined(apollo)
#       define bcopy(src,dst,len) memcpy((dst),(src),(int) (len))
#       define bzero(dst,len) (void) memset((dst), 0, (len))
#       define bcmp(b1,b2,len) memcmp((b1),(b2),(len))
#   endif
#else
#   ifdef SYS5
#       define bcopy(src,dst,len) memcpy((dst),(src),(int) (len))

Pointer memcpy (
#ifdef __STDC__
    Pointer dst,
    Pointer src,
    int     len
#endif
);

#   else

void bcopy (
#ifdef __STDC__
    Pointer src,
    Pointer dst,
    int     len
#endif
);

#   endif /* SYS5 */

int bcmp (
#ifdef __STDC__
    Pointer b1,
    Pointer b2,
    int     len
#endif
);

void bzero (
#ifdef __STDC__
    Pointer b,
    int     len
#endif
);

int ffs (
#ifdef __STDC__
    int     i
#endif
);

int memcmp ( 
#ifdef __STDC__
    char    *s1, 
    char    *s2,
    int     n
#endif
);

#endif /* __STDC__ */

#endif /* SNI_SVR4_POSIX */

#endif /* bstring_h__included */
