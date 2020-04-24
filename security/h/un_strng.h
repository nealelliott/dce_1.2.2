/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: un_strng.h,v $
 * Revision 1.1.5.2  1996/02/18  23:01:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:07  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:30:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:37  root]
 * 
 * Revision 1.1.3.3  1992/12/29  13:28:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:47:36  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:17:52  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:21:40  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:42:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  un_strng.h V=3 04/30/91 //nbs/dds_tools/src
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
**      Prototypes for unix string functions
** 
*/

#ifndef unix_string_h__included
#define unix_string_h__included

#if defined(SYS5) || __STDC__ || defined(__STDC_ZERO_OK__) || defined(SNI_SVR4)
#   include <string.h>

#ifdef SNI_SVR4_POSIX
#define index(c, i)             strchr((c), (i))
#define rindex(c, i)            strrchr((c), (i))
#endif /* SNI_SVR4_POSIX */

#else
#   include <strings.h>
#endif

#endif /* unix_string_h__included */
