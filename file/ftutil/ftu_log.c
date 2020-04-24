/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftu_log.c,v $
 * Revision 1.1.26.1  1996/10/02  17:48:43  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:35  damon]
 *
 * Revision 1.1.21.4  1994/08/17  15:40:33  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/16  20:30:37  ruby]
 * 
 * Revision 1.1.23.2  1994/06/29  11:37:59  devsrc
 * 	merged with bl-10
 * 
 * Revision 1.1.23.1  1994/04/28  17:35:54  mbs
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:19  mbs]
 * 
 * Revision 1.1.21.2  1994/06/09  14:09:16  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:55  annie]
 * 
 * Revision 1.1.21.1  1994/02/04  20:19:46  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:38  devsrc]
 * 
 * Revision 1.1.19.2  1994/01/20  18:43:13  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:28  annie]
 * 
 * Revision 1.1.19.1  1993/12/07  17:25:42  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:06:21  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1990, 1991, 1992 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <stdarg.h>
#include <time.h>

#include <dcedfs/stds.h>

#include <ftutil.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/ftu_log.c,v 1.1.26.1 1996/10/02 17:48:43 damon Exp $")

/*
 * The functionality of each of the following exported routines is described
 * in the header file.  The comments below only give implementation
 * specifics, where necessary.
 */

/*
 * Constants
 */
#define TIME_BUF_SIZE	256

/*
 * Static storage
 */
SHARED struct icl_set*	_ftu_iclSetP = 0;
PRIVATE FILE*		logStream = 0;

/******************************************************************************
 * Logging routines
 *****************************************************************************/

/*
 * ftu_CreateIclSet()
 */
EXPORT long
ftu_CreateIclSet(iclLogP)
    IN struct icl_log*	iclLogP;
{
    return icl_CreateSet("ftutil", iclLogP, 0, &_ftu_iclSetP);
}	/* ftu_CreateIclSet() */

/*
 * ftu_SetLogStream()
 */
EXPORT void
ftu_SetLogStream(stream)
    IN FILE*	stream;
{
    logStream = stream;
}	/* ftu_SetLogStream() */

/*
 * ftu_GetLogStream()
 */
EXPORT void
ftu_GetLogStream(streamP)
    OUT FILE**	streamP;
{
    *streamP = logStream;
}	/* ftu_GetLogStream() */

/*
 * ftu_LogPrintf()
 */
EXPORT void
ftu_LogPrintf(IN char* format, ...)
{
    va_list		args;
    char		buf[TIME_BUF_SIZE];
    time_t		clock;
    struct tm* 	tm;

    if (logStream != 0) {
	clock = osi_Time();
	tm = localtime(&clock);
	if (strftime(buf, TIME_BUF_SIZE, "%Y-%b-%d %T ", tm) != 0)
	    (void)fprintf(logStream, buf);
	else
	    (void)fprintf(logStream, "??-??-?? ??:??:?? ");

	va_start(args, format);
	(void)vfprintf(logStream, format, args);
	va_end(args);

	(void)putc('\n', logStream);
	(void)fflush(logStream);
    }
}	/* ftu_LogPrintf() */

/*
 * ftu_svc_LogPrintf()
 * An ftu_LogPrintf that takes a message id instead of a format string
 */
EXPORT void
ftu_svc_LogPrintf(const unsigned32		message_index,
		  ...)
{
    va_list		args;
    char		buf[TIME_BUF_SIZE];
    time_t		clock;
    struct tm* 	tm;
    unsigned char     *format_string;
    error_status_t    st;

    if (logStream) {
      clock = osi_Time();
      tm = localtime(&clock);
	
      if (strftime(buf, TIME_BUF_SIZE, "%Y-%b-%d %T ", tm) != 0)
	(void) fprintf(logStream, buf);
      else
	(void) fprintf(logStream, "??-??-?? ??:??:?? ");

      format_string = dce_msg_get_msg(message_index, &st);

      va_start(args, message_index);
      vfprintf(logStream, (char *) format_string, args);
      va_end(args);

      free(format_string);

      (void)putc('\n', logStream);
      (void)fflush(logStream);
    }
}	/* ftu_svc_LogPrintf() */
