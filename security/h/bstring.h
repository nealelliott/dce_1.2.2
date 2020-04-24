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
 * Revision 1.1.6.2  1996/02/18  22:58:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:48  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:24:28  root
 * 	Submit
 * 	[1995/12/11  15:14:13  root]
 * 
 * Revision 1.1.3.3  1992/12/29  13:06:10  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:08  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:17:28  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:21:11  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  bstring.h V=4 11/22/91 //nbs/dds_tools/src
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      Prototypes for byte string manipulation functions
** 
** Copyright Hewlett-Packard Company 1989, 1990
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

#ifndef bstring_h__included
#define bstring_h__included

#include <Pointer.h>

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

#ifndef _AIX
int memcmp ( 
#ifdef __STDC__
    char    *s1, 
    char    *s2,
    int     n
#endif
);
#endif  /* _AIX */

#endif /* __STDC__ */

#endif /* SNI_SVR4_POSIX */

#endif /* bstring_h__included */
