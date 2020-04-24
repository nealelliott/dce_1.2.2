/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: initstring.c,v $
 * Revision 1.1.4.2  1996/02/18  00:22:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:02:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:55  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:40:34  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:31:54  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:41:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  initstring.c V=2 06/04/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
**  String Constant generation
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)initstring.c	2 - 06/04/91";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <initstring.h>

char * sec_util_string_init
#ifndef __STDC__
    ( buf, prefix, str, suffix ) 
    char    **buf;
    char    *prefix;
    char    *str;
    char    *suffix;
#else
  (
    char    **buf,
    char    *prefix,
    char    *str,
    char    *suffix
  )
#endif
{
    int len1 = 0;
    int len2 = 0;
    int len3 = 0;
    int len;
    char *p;
    char *q;

    if (prefix) {
        len1 = strlen(prefix);
    }

    if (str) {
        len2 = strlen(str);
    }

    if (suffix) {
        len3 = strlen(suffix);
    }

    len = len1 + len2 + len3;

    p = malloc(len + 1);
    if (p) {
        q = p;
        strcpy(q, prefix);
        q += len1;
        strcpy(q, str);
        q += len2;
        strcpy(q, suffix);
    }

    *buf = p;
    return p;
} 
