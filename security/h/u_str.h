/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: u_str.h,v $
 * Revision 1.2.4.2  1996/02/18  23:01:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:57  marty]
 *
 * Revision 1.2.4.1  1995/12/08  17:30:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:32  root]
 * 
 * Revision 1.2.2.2  1992/12/29  13:28:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:16:16  zeliff]
 * 
 * Revision 1.2  1992/01/19  22:13:45  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
*/

/*
 *  OSF DCE Version 1.0 
 */

/*  u_str.h V=5 07/18/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 * u_str.h: common string porcessing functions/macros
 */ 

#ifndef __u_str_h__included
#define __u_str_h__included

#include <un_strng.h>

#define u_strlen(parm) ( (char *) parm == 0 ? 0 : strlen((char *) parm) )
#define u_strcmp(s1, s2) strcmp((char *) (s1), (char *) (s2))
#define u_strncmp(s1, s2, len) strncmp((char *) (s1), (char *) (s2), (len))
#define u_strcpy(dest, src) strcpy((char *) (dest), ((char *) (src) == 0 ? "" : (char *) (src)))
#define u_strncpy(dest, src, len) strncpy((char *) (dest), (char *) (src), (len))
#define u_strcat(dest, src) strcat((char *) (dest), ((char *) (src) == 0 ? "" : (char *) (src)))
#define u_strncat(dest, src, len) strncat((char *) (dest), (char *) (src), (len))
#define u_strchr(str, chr) strchr((char *) (str), (chr))
#define u_free(parm) free((char *) (parm))
    
/* d y n s t r c p y   
 *
 * given a string  and a pointer, allocate dynamic
 * storage for a copy of the string and do the copy.
 * It's up to the caller to free the storage.
 */
void dynstrcpy (
#ifdef __STDC__
    char  **dest,
    char  source[]
#endif
);

/* d y n s t r n c p y   
 *
 * Not to be confused with strncpy, allocates storage for 
 * (len + 1) chars and copies len byes.  Unlike strncpy, the result
 * is always NULL terminated. Really useful for turning non-C
 * strings into C strings.
 */
void dynstrncpy (
#ifdef __STDC__
    char     **dest,
    char     source[],
    long int len
#endif
);

#define u_dynstrcpy(dest, src) dynstrcpy((char **)(dest), (char *)(src))
#define u_dynstrncpy(dest, src, len) dynstrncpy((char **)(dest), (char *)(src), (len))

#endif
