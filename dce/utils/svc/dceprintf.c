/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993, 1994, 1996 Open Software Foundation, Inc.
 * Permission is hereby granted to use, copy, modify and freely distribute 
 * the software in this file and its documentation for any purpose without 
 * fee, provided that the above copyright notice appears in all copies and 
 * that both the copyright notice and this permission notice appear in 
 * supporting documentation.  Further, provided that the name of Open 
 * Software Foundation, Inc. ("OSF") not be used in advertising or 
 * publicity pertaining to distribution of the software without prior 
 * written permission from OSF.  OSF makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty.
 * 
 * Formatted output routines.
 */

/*
 * HISTORY
 * $Log: dceprintf.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:03  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:37:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:29  root]
 * 
 * Revision 1.1.4.8  1994/10/03  20:08:41  bowe
 * 	Realloc result of *_sprintf() to correct size. [CR 12445]
 * 	[1994/10/03  20:08:11  bowe]
 * 
 * Revision 1.1.4.7  1994/09/13  18:00:38  rrizzo
 * 	Added missing first line to copyright text.
 * 	[1994/09/13  17:59:16  rrizzo]
 * 
 * Revision 1.1.4.6  1994/08/30  20:41:57  marty
 * 	CR 11923 - Change copyright to a public domain copyright.
 * 	[1994/08/30  20:34:30  marty]
 * 
 * Revision 1.1.4.5  1994/08/01  15:40:13  bowe
 * 	Make functions cancel-safe [CR 11463]
 * 	[1994/08/01  15:34:22  bowe]
 * 
 * Revision 1.1.4.4  1994/07/27  12:27:56  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:33:47  rsalz]
 * 
 * Revision 1.1.4.3  1994/07/26  17:04:18  bowe
 * 	In dce_[f]printf, do not add \n to end of string.
 * 	In dce_pgm_[f]printf, however, DO add a \n.  [CR 10363]
 * 	[1994/07/26  17:04:04  bowe]
 * 
 * Revision 1.1.4.2  1994/06/09  16:05:56  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:31  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/09  20:43:20  rsalz
 * 	Add dce_pgm_{printf,fprintf,sprintf} (OT CR 9926).
 * 	[1994/03/09  20:14:43  rsalz]
 * 
 * Revision 1.1.2.3  1993/10/07  17:07:21  bowe
 * 	Changed to print newline after message is printed, in dce__vfprintf(),
 * 	which is called by dce_printf() and dce_fprintf().
 * 	[1993/10/07  17:01:26  bowe]
 * 
 * Revision 1.1.2.2  1993/08/16  18:08:11  rsalz
 * 	Initial release
 * 	[1993/08/16  18:02:59  rsalz]
 * 
 * $EndLog$
 */

#include <svcimpl.h>


static int
dce__vfprintf(
    FILE			*stream,
    const unsigned32		message_index,
    va_list			args
)
{
    unsigned char		*format;
    error_status_t		st;
    int				result;

    if ((format = dce_msg_get_msg(message_index, &st)) == NULL)
	return EOF;
    result = vfprintf(stream, (char *)format, args);
    free(format);
    return result;
}


int
dce_printf(
    const unsigned32		message_index,
    ...
)
{
    va_list			args;
    int				i;
    int				cstate;		/* thread cancel state */

    cstate = pthread_setcancel(CANCEL_OFF);
    va_start(args, message_index);
    i = dce__vfprintf(stdout, message_index, args);
    va_end(args);
    if (cstate == CANCEL_ON)
	cstate = pthread_setcancel(CANCEL_ON);
    return i;
}


int
dce_pgm_printf(
    const unsigned32		message_index,
    ...
)
{
    va_list			args;
    int				i;
    int				cstate;		/* thread cancel state */

    cstate = pthread_setcancel(CANCEL_OFF);
    va_start(args, message_index);
    printf("%s:  ", dce_svc_g_progname);
    i = dce__vfprintf(stdout, message_index, args);
    va_end(args);
    fputc('\n', stdout);
    if (cstate == CANCEL_ON)
	cstate = pthread_setcancel(CANCEL_ON);
    return i;
}


int
dce_fprintf(
    FILE			*stream,
    const unsigned32		message_index,
    ...
)
{
    va_list			args;
    int				i;
    int				cstate;		/* thread cancel state */

    cstate = pthread_setcancel(CANCEL_OFF);

    va_start(args, message_index);
    i = dce__vfprintf(stream, message_index, args);
    va_end(args);
    if (cstate == CANCEL_ON)
	cstate = pthread_setcancel(CANCEL_ON);
    return i;
}


int
dce_pgm_fprintf(
    FILE			*stream,
    const unsigned32		message_index,
    ...
)
{
    va_list			args;
    int				i;
    int				cstate;		/* thread cancel state */

    cstate = pthread_setcancel(CANCEL_OFF);

    va_start(args, message_index);
    fprintf(stream, "%s:  ", dce_svc_g_progname);
    i = dce__vfprintf(stream, message_index, args);
    va_end(args);
    fputc('\n', stream);
    if (cstate == CANCEL_ON)
	cstate = pthread_setcancel(CANCEL_ON);
    return i;
}


unsigned char *
dce_sprintf(
    const unsigned32	message_index,
    ...
)
{
    va_list		args;
    error_status_t	st;
    unsigned char	*result;
    unsigned char	*format;
    int				cstate;		/* thread cancel state */

    cstate = pthread_setcancel(CANCEL_OFF);
    /* Get message, allocate memory. */
    if ((format = dce_msg_get_msg(message_index, &st)) == NULL) {
	if (cstate == CANCEL_ON)
	    cstate = pthread_setcancel(CANCEL_ON);
	return NULL;
    }
    if ((result = malloc(SVCSPRINTFBUF)) == NULL) {
	if (cstate == CANCEL_ON)
	    cstate = pthread_setcancel(CANCEL_ON);
	free(format);
	return NULL;
    }

    /* Format the message. */
    va_start(args, message_index);
    (void)vsprintf((char *)result, (char *)format, args);
    va_end(args);

    /* Clean up and returns. */
    free(format);
    if (cstate == CANCEL_ON)
	cstate = pthread_setcancel(CANCEL_ON);
    return realloc(result, strlen((char *)result) + 1);
}


unsigned char *
dce_pgm_sprintf(
    const unsigned32	message_index,
    ...
)
{
    va_list		args;
    error_status_t	st;
    unsigned char	*result;
    unsigned char	*format;
    int				cstate;		/* thread cancel state */

    cstate = pthread_setcancel(CANCEL_OFF);
    /* Get message, allocate memory. */
    if ((format = dce_msg_get_msg(message_index, &st)) == NULL) {
	if (cstate == CANCEL_ON)
	    cstate = pthread_setcancel(CANCEL_ON);
	return NULL;
    }
    if ((result = malloc(SVCSPRINTFBUF)) == NULL) {
	if (cstate == CANCEL_ON)
	    cstate = pthread_setcancel(CANCEL_ON);
	free(format);
	return NULL;
    }
    sprintf((char *)result, "%s:  ", dce_svc_g_progname);

    /* Format the message. */
    va_start(args, message_index);
    (void)vsprintf((char *)result + strlen((char *)result),
	    (char *)format, args);
    va_end(args);

    /* Clean up and returns. */
    free(format);
    if (cstate == CANCEL_ON)
	cstate = pthread_setcancel(CANCEL_ON);
    return realloc(result, strlen((char *)result) + 1);
}
