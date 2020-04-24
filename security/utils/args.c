/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: args.c,v $
 * Revision 1.1.8.2  1996/02/18  00:22:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:06  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:01:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:50  root]
 * 
 * Revision 1.1.6.3  1993/10/22  19:49:41  desai
 * 	CR 9239 fix. Backed-out current changes associated with \ not escaping in
 * 	command line problem.
 * 	[1993/10/22  19:48:25  desai]
 * 
 * Revision 1.1.4.3  1992/12/29  16:40:19  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:31:39  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/12  19:46:01  burati
 * 	CR2828 increase command line length to accommodate 1024 char names
 * 	[1992/10/09  21:07:10  burati]
 * 
 * Revision 1.1.2.2  1992/05/08  17:26:14  burati
 * 	Fix buffer length
 * 	[1992/05/07  23:28:39  burati]
 * 
 * Revision 1.1  1992/01/19  14:41:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  args.c V=2 04/26/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      command line argument parsing routines - public header file
** 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)args.c	2 - 04/26/91";
#endif

#include <dce/nbase.h>

#include <stdio.h>
#include <ctype.h>
#include <un_strng.h>
#include <macros.h>

#include <args.h>
#include <commands.h>

static char *buf = NULL;
static char buf_start[CMD_LINE_LENGTH];
static char **buf_ptr = NULL;
static char *buf_ptr_start[CMD_LINE_LENGTH];


PRIVATE start_str(
#ifdef __STDC__
    void
#endif
)
{
	buf = buf_start;
	buf_ptr = buf_ptr_start;
}	

PRIVATE savestr
#ifndef __STDC__
    (str)
	char *str;
#else
  (
	char *str
  )
#endif
{
	int alloc_len = strlen(str) + 1;

	(void) strcpy(buf, str);
	*buf_ptr++ = buf;
	buf += alloc_len;
}

PRIVATE char *dostring
#ifndef __STDC__
    (buf)
   char *buf;
#else
  (
   char *buf
  )
#endif
{
	char *start = buf++;

	while (*buf != '"') {
		if (*buf == '\\') buf++;
		if (*buf == '\0') break;
		buf++;
	}
	/* get rid of opening quote - by sliding string over */
	if (*buf == '"') {
		*buf = '\0';
		strcpy(start, start+1);
		strcpy(buf-1, buf+1);
		buf -= 2;
	} else {
		strcpy(start, start+1);
	}
		
	return buf;
}

/* 
 * args_get 
 * takes a string as input (buf) and yields a count of arguments and
 * an "argv" buffer of pointers to each argument.
 */

PUBLIC void args_get
#ifndef __STDC__
    (buf, argc, argv)
   char *buf;
   int *argc;
   char ***argv;
#else
  (
   char *buf,
   int *argc,
   char ***argv
  )
#endif
{
	char *start;
	char t;

	start_str();	
	*argc = 0;
	*argv = buf_ptr;
	while (*buf != '\0') {
		while (isspace(*buf)) buf++;
		if (*buf == '\0')
			break;
		start = buf;
		while (!isspace(*buf)) {
			if (*buf == '"')
				buf = dostring(buf);
			if (*buf == '\\')
				buf++;
			if (*buf == '\0')
				break;
			buf++;
		}
		t = *buf;
		*buf = '\0';
                savestr(start);
		(*argc)++;
		if (t == '\0')
			break;
		*buf++ = t;
	}
	*buf_ptr = NULL;
}


/* 
 * match_command 
 * takes a key and string as input and returns whether the
 * string matches the key where at least min_len characters
 * of the key are required to be specified.
 */


PUBLIC boolean32 match_command
#ifndef __STDC__
    (key,str,min_len)
   char *key;
   char *str;
   int min_len;
#else
  (
   char *key,
   char *str,
   int min_len
  )
#endif
{
    int i = 0;

    if (*key) while (*key == *str) {
        i++;
        key++;
        str++;
        if (*str == '\0' || *key == '\0')
            break;
    }
    if (*str == '\0' && i >= min_len)
        return true;
    return false;
}
