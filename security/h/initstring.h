/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: initstring.h,v $
 * Revision 1.1.4.2  1996/02/18  22:58:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:53  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:24:34  root
 * 	Submit
 * 	[1995/12/11  15:14:18  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:06:35  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:20  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  initstring.h V=1 06/04/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
**  String Constant generation
*/

#ifndef _iniststring_h_
#define _iniststring_h_

#define STRING_CONCAT(buf, prefix, str, suffix) \
                (buf ? buf : sec_util_string_init(&buf, prefix, str, suffix))

char * sec_util_string_init (
#ifdef __STDC__
    char    **buf,
    char    *prefix,
    char    *str,
    char    *suffix
#endif
);

#endif /* _iniststring_h_ */
