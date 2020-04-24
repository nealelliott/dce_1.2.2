/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: u_str.c,v $
 * Revision 1.1.6.1  1996/10/03  15:16:10  arvind
 * 	Silence compiler warnings.
 * 	[1996/09/16  23:33 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.4.2  1996/02/18  00:23:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:48  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  18:03:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:19  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:41:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:32:30  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:41:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  u_str.c V=5 11/11/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 * str_util.c: common string porcessing functions
 */ 

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)u_str.c	5 - 11/11/91";
#endif


#include <un_maloc.h>
#include <macros.h>
#include <u_str.h>


/* d y n u s t r c p y
 *
 * given a string  and a pointer, allocate dynamic
 * storage for a copy of the string and do the copy.
 * It's up to the caller to free the storage.
 */
PUBLIC void dynstrcpy
#ifndef __STDC__
    ( dest, source )
    char      **dest;
    char      source[];
#else
  (
    char      **dest,
    char      source[]
  )
#endif
{        
    *dest = NULL;

    if ((source != NULL) && (*source != 0)) {
	*dest = malloc(strlen(source) + 1);
	if (*dest != NULL) {
	    strcpy(*dest, source);
        }  
    }
}

PUBLIC void dynstrncpy
#ifndef __STDC__
    ( dest, source, len )
    char      **dest;
    char      source[];
    long int  len;
#else
(
    char      **dest,
    char      source[],
    long int  len
  )
#endif
{        
    *dest = NULL;

    if ((source != NULL) && (*source != 0)) {
	*dest = malloc(len + 1);
	if (*dest != NULL) {
	    strncpy(*dest, source, len);
            (*dest)[len] = '\0';
        }  
    }
}

