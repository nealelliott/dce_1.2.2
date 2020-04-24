/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rss_errors.h,v $
 * Revision 1.1.4.2  1996/02/18  23:00:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:28:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:02  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:26:54  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:39  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rss_errors.h V=7 04/29/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server Surrogate - Error Routines
 * 
 */

#ifndef rss_errors__included
#define rss_errors__included

#include <dce/nbase.h>

#define fatal(FATAL_msg) \
   { \
   char buf[1024]; \
   sprintf(buf, "%s\n at line %d of file %s", FATAL_msg, __LINE__, __FILE__); \
   rs_util_fatal(buf); \
   } 

void rs_util_fatal ( 
#ifdef __STDC__
    char  *string
#endif
);

#define fatal_st(FATAL_ST_st,FATAL_ST_msg) \
   { \
   char buf[1024]; \
   sprintf(buf, "%s\n at line %d of file %s", FATAL_ST_msg, __LINE__, __FILE__); \
   rs_util_fatal_st(FATAL_ST_st, buf); \
   }

void rs_util_fatal_st ( 
#ifdef __STDC__
    error_status_t  status,
    char            *string
#endif
);

void error (
#ifdef __STDC__
    char  *string
#endif
);


void error_st ( 
#ifdef __STDC__
    error_status_t  status,
    char            *string
#endif
);


void warning ( 
#ifdef __STDC__
    char *string
#endif
);

void warning_st ( 
#ifdef __STDC__
    error_status_t  status,
    char            *string
#endif
);

void print_st ( 
#ifdef __STDC__
    error_status_t  status
#endif
);

void print_rgy_st ( 
#ifdef __STDC__
    error_status_t  status
#endif
);

#endif  /* rss_errors__included */
