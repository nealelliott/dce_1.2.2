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
 * Serviceability print routines.
 */

/*
 * HISTORY
 * $Log: svcprintf.c,v $
 * Revision 1.1.10.3  1996/02/18  19:22:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:25  marty]
 *
 * Revision 1.1.10.2  1995/12/08  21:38:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:10:09  root]
 * 
 * Revision 1.1.6.1  1994/10/14  18:17:26  bowe
 * 	Added LOCK/UNKLOCK to dce_svc__debug(), dce_svc_bin_log() [CR 12608]
 * 	[1994/10/14  18:17:01  bowe]
 * 
 * Revision 1.1.4.21  1994/10/03  16:54:05  rsalz
 * 	Add GOESTO route type (OT CR 10474).
 * 	[1994/10/03  16:33:14  rsalz]
 * 
 * Revision 1.1.4.20  1994/09/13  18:11:02  rrizzo
 * 	Added missing first line to copyright text.
 * 	[1994/09/13  17:56:14  rrizzo]
 * 
 * Revision 1.1.4.19  1994/09/01  19:39:14  rsalz
 * 	Get "brief" test right (OT CR 12006).
 * 	[1994/09/01  19:37:08  rsalz]
 * 
 * Revision 1.1.4.18  1994/08/30  20:42:01  marty
 * 	CR 11923 - Change copyright to a public domain copyright.
 * 	[1994/08/30  20:34:33  marty]
 * 
 * Revision 1.1.4.17  1994/08/28  00:54:55  rsalz
 * 	Oops, SVC_BRIEF is only for debug messages.  (OT CR 11512).
 * 	[1994/08/28  00:53:53  rsalz]
 * 
 * Revision 1.1.4.16  1994/08/28  00:51:07  rsalz
 * 	Add "SVC_BRIEF" env var support (OT CR 11512).
 * 	[1994/08/28  00:45:34  rsalz]
 * 
 * Revision 1.1.4.15  1994/08/01  15:45:42  bowe
 * 	Merged with changes from 1.1.4.14
 * 	[1994/08/01  15:45:37  bowe]
 * 
 * 	Make functions cancel-safe [CR 11463]
 * 	[1994/08/01  15:44:59  bowe]
 * 
 * Revision 1.1.4.14  1994/08/01  15:30:48  rsalz
 * 	Cleanup typo's in internal dce_assert calls (OT CR 11502).
 * 	[1994/08/01  15:24:09  rsalz]
 * 
 * Revision 1.1.4.13  1994/07/27  12:28:03  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:35:08  rsalz]
 * 
 * Revision 1.1.4.12  1994/07/12  20:10:23  bowe
 * 	In dce_svc__binprint(), free what idl_es_encode_dyn_buffer() gave us.
 * 	Some minor cleanup.  [CR 11156]
 * 	[1994/07/12  20:09:29  bowe]
 * 
 * Revision 1.1.4.11  1994/07/08  21:42:43  bowe
 * 	Print the file and line number with rest of prolog. [CR 10479]
 * 	[1994/07/08  21:41:40  bowe]
 * 
 * Revision 1.1.4.10  1994/06/30  21:24:37  bowe
 * 	In dce_svc_bin_log(), set prolog.format=NULL [11055]
 * 	[1994/06/30  21:21:57  bowe]
 * 
 * Revision 1.1.4.9  1994/06/27  19:13:35  bowe
 * 	In dce_svc_bin_log(), set prolog.format (was NULL). [CR 11055]
 * 	In dce_svc_inq_sev_name(), print DEBUGn, when appropriate. [CR 11078]
 * 	[1994/06/27  18:34:22  bowe]
 * 
 * Revision 1.1.4.8  1994/06/10  19:42:08  tom
 * 	Change argument to dce_svc__fillin (OT 10922).
 * 	[1994/06/10  19:41:30  tom]
 * 
 * Revision 1.1.4.7  1994/06/09  16:06:31  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:01  devsrc]
 * 
 * Revision 1.1.4.6  1994/05/27  17:44:51  bowe
 * 	Added #include <dce/idl_es.h> [CR 10483,10478]
 * 	[1994/05/27  16:43:05  bowe]
 * 
 * Revision 1.1.4.5  1994/05/26  18:51:53  bowe
 * 	Use IDL encoding services to extract binary log data.  Affected
 * 	dce_svc__binprint().  Added new call, dce_svc_inq_sev_name().
 * 	[CR 10483,10478]
 * 	Add newline between prolog and message. [CR 10482]
 * 	[1994/05/26  18:46:46  bowe]
 * 
 * Revision 1.1.4.4  1994/04/29  15:31:28  rsalz
 * 	Add misssing printf controls to dce_svc__binprint (OT CR 10488)
 * 	[1994/04/29  15:08:38  rsalz]
 * 
 * Revision 1.1.4.3  1994/04/14  16:44:18  rsalz
 * 	Return value in dce_svc__lock_for_write (OT CR 10354)
 * 	[1994/04/14  16:41:52  rsalz]
 * 
 * Revision 1.1.4.2  1994/03/17  21:17:13  rsalz
 * 	Make dce_msg_*() routines thread-safe (OT CR 9897)
 * 	[1994/03/09  21:19:45  rsalz]
 * 
 * Revision 1.1.4.1  1994/03/09  20:43:22  rsalz
 * 	Add dce_pgm_{printf,fprintf,sprintf} (OT CR 9926).
 * 	[1994/03/09  20:14:45  rsalz]
 * 
 * 	Fix va_start call in dce_svc_bin_log (OT CR 10108).
 * 	[1994/03/09  19:42:27  rsalz]
 * 
 * Revision 1.1.2.7  1993/12/08  14:17:53  rsalz
 * 	Add dce_svc__basename() so only simple name of the source file
 * 	is printed in the log.
 * 	[1993/12/08  14:17:35  rsalz]
 * 
 * Revision 1.1.2.6  1993/11/30  17:09:56  rsalz
 * 	Count was off-by-one for rotating log files; fix from
 * 	<kazuko@austin.ibm.com>.
 * 	[1993/11/30  17:09:47  rsalz]
 * 
 * Revision 1.1.2.5  1993/11/04  18:40:45  rsalz
 * 	Make dce_svc_bin_log calling sequence be like dce_svc_printf's.
 * 	[1993/11/04  18:39:02  rsalz]
 * 
 * Revision 1.1.2.4  1993/10/26  18:43:11  rsalz
 * 	Fix submit error.
 * 	[1993/10/26  18:42:16  rsalz]
 * 
 * 	Change tag so NOTICE_VERBOSE is correct.
 * 
 * Revision 1.1.2.3  1993/09/14  18:57:14  rsalz
 * 	Fix filter dispatch; work around AIX compiler bug.
 * 	[1993/09/14  18:55:51  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:10:12  rsalz
 * 	Initial release
 * 	[1993/08/16  18:04:49  rsalz]
 * 
 * $EndLog$
 */

#include <svcimpl.h>
#include <dce/idl_es.h>


#define SET_PROGNAME(buff)	\
    if (dce_svc_g_progname[0])	\
	(void)strcpy(buff, dce_svc_g_progname);	\
    else	\
	(void)sprintf(buff, "PID#%ld", (long)getpid())	\

const char		*dce_svc_g_code_table[5] = {
	"FATAL",	"ERROR",	"WARNING",
	"NOTICE",	"NOTICE_VERBOSE",
};
unsigned32		dce_svc_g_attr_table[5] = {
    svc_c_sev_fatal,	 svc_c_sev_error,	svc_c_sev_warning,
    svc_c_sev_notice,	svc_c_sev_notice_verbose
};
dce_svc_routing_block_t	dce_svc_g_routes[5];
char			dce_svc_g_progname[dce_svc_c_progname_buffsize];


static int
dce_svc__lock_for_write(
    int			fd
)
{
    struct flock	arg;

    arg.l_type = F_WRLCK;
    arg.l_whence = SEEK_SET;
    arg.l_start = 0;
    arg.l_len = 0;
    fcntl(fd, F_SETLKW, &arg);
    return pthread_setcancel(CANCEL_OFF);
}


static void
dce_svc__unlock_for_write(
    int			fd,
    int			cancelmode
)
{
    struct flock	arg;

    arg.l_type = F_UNLCK;
    arg.l_whence = SEEK_SET;
    arg.l_start = 0;
    arg.l_len = 0;
    fcntl(fd, F_SETLKW, &arg);
    if (cancelmode == CANCEL_ON)
	(void)pthread_setcancel(CANCEL_ON);
}



static void
dce_svc__vfprintf(
    FILE		*F,
    dce_svc_prolog_t	prolog,
    boolean32		split_line,
    va_list		args
)
{
    char		buff[UTC_MAX_STR_LEN];
    int			cancelmode;
    char		sep;

    (void)utc_asclocaltime(buff, sizeof buff, &prolog->t);
    cancelmode = dce_svc__lock_for_write((int)fileno(F));
    sep = split_line ? '\n' : ' ';

    /* If not brief and not debug-globally brief, print the prolog. */
    if ((prolog->attributes & svc_c_action_brief) != 0
     || (dce_svc_g_brief != 0 && (prolog->attributes & svc__c_debugmask) != 0))
	;
    else {
	if ((prolog->attributes & svc__c_debugmask) != 0) {
#if	defined(DCE_SVC_WANT__FILE__)
	    fprintf(F, "%s %s DEBUG%d %s %s %s %d 0x%8.8lx%c",
#else
	    fprintf(F, "%s %s DEBUG%d %s %s 0x%8.8lx%c",
#endif	/* defined(DCE_SVC_WANT__FILE__) */
		buff, prolog->progname,
		prolog->attributes & svc__c_debugmask,
		prolog->handle->component_name,
		prolog->handle->table[prolog->table_index].sc_name,
#if	defined(DCE_SVC_WANT__FILE__)
		prolog->file, prolog->line,
#endif	/* defined(DCE_SVC_WANT__FILE__) */
		(unsigned long)dce_svc__mapthreadid(&prolog->thread_id), sep);
	}
	else {
#if	defined(DCE_SVC_WANT__FILE__)
	    fprintf(F, "%s %s %s %s %s %s %d 0x%8.8lx%c",
#else
	    fprintf(F, "%s %s %s %s %s 0x%8.8lx%c",
#endif	/* defined(DCE_SVC_WANT__FILE__) */
		buff, prolog->progname,
		dce_svc_g_code_table[ATTRIBUTEStoINDEX(prolog->attributes)],
		prolog->handle->component_name,
		prolog->handle->table[prolog->table_index].sc_name,
#if	defined(DCE_SVC_WANT__FILE__)
		prolog->file, prolog->line,
#endif	/* defined(DCE_SVC_WANT__FILE__) */
		(unsigned long)dce_svc__mapthreadid(&prolog->thread_id), sep);
	}
    }

    if (prolog->format == NULL)
	fprintf(F, "Unknown message 0x%d\n", prolog->message_index);
    else {
	vfprintf(F, prolog->format, args);
	putc('\n', F);
    }
    (void)fflush(F);
    dce_svc__unlock_for_write((int)fileno(F), cancelmode);
}


static void
dce_svc__binprint(
    int				fd,
    dce_svc_prolog_t		prolog,
    va_list			args
)
{
    static char			null_buf[SVC_BLOCK_SIZE];
    static svc_bin_data_t	bin_data_null;
    const char			*p;
    int				n;
    boolean			islong;
    int				cancelmode;
    idl_ulong_int		enc_size;
    idl_byte			*enc_p;
    idl_es_handle_t		enc_h;
    error_status_t		st;
    svc_bin_data_t		bin_data;
    svc_bin_arg_t		values[dce_svc_c_num_values];
    svc_bin_arg_t		*dp;
    unsigned32			a_count;
    error_status_t		st2;

    idl_es_encode_dyn_buffer(&enc_p, &enc_size, &enc_h, &st);
    if (st != error_status_ok) {
	dce_assert(prolog->handle, st == error_status_ok);
	return;		/* not much else we can do here */
    }
    bin_data			= bin_data_null;
    bin_data.version		= prolog->version;
    bin_data.t			= prolog->t;
    bin_data.attributes		= prolog->attributes;
    bin_data.message_index	= prolog->message_index;
    bin_data.thread_code	=
	    (unsigned32)dce_svc__mapthreadid(&prolog->thread_id);
    bin_data.argtypes		= (idl_char *)prolog->argtypes;
    bin_data.fac_name		=		/* subcomponent */
	    ((idl_char *)prolog->handle->table[prolog->table_index].sc_name);
    bin_data.format		= (idl_char *)prolog->format;
    bin_data.progname		= (idl_char *)prolog->progname;
    bin_data.file		= (idl_char *)prolog->file;
    bin_data.line		= prolog->line;
    bin_data.count		= 0;

    /* First see how many we need 'values' we need.  If more than what's on
     * stack, then use malloc.  (We prefer to use the stack, for efficiency...)
     */
    for (a_count = 0, p = prolog->argtypes; *p; p++)
	if (*p == '%')
	    a_count++;
    if (a_count > dce_svc_c_num_values)
	bin_data.args = malloc(sizeof (svc_bin_arg_t) * a_count);
    else
	bin_data.args = values;

    a_count = 0;
    for (islong=FALSE, dp=bin_data.args, p=prolog->argtypes; *p; p++) {
	switch (*p) {
	    /* Note the continue in the first cases, not break. */
	default:
	case '%':
	    continue;
	case 'l':
	    islong = TRUE;
	    continue;

	case 'd': case 'i': case 'o': case 'x': case 'X':
	    if (islong) {
		dp->bin_encoding = svc_bin_type_long;
		dp->tagged_union.l = va_arg(args, long);
	    }
	    else {
		dp->bin_encoding = svc_bin_type_int;
		dp->tagged_union.i = va_arg(args, int);
	    }
	    break;
	case 'u':
	    if (islong) {
		dp->bin_encoding = svc_bin_type_ulong;
		dp->tagged_union.ul = va_arg(args, long);
	    }
	    else {
		dp->bin_encoding = svc_bin_type_uint;
		dp->tagged_union.ui = va_arg(args, int);
	    }
	    break;
	case 'f': case 'e': case 'E': case 'g': case 'G':
	    dp->bin_encoding = svc_bin_type_double;
	    dp->tagged_union.d = va_arg(args, double);
	    break;
	case 'c':
	    dp->bin_encoding = svc_bin_type_char;
	    dp->tagged_union.c = (char)va_arg(args, int);
	    break;
	case 's':
	    dp->bin_encoding = svc_bin_type_string;
	    dp->tagged_union.s = (idl_char *)va_arg(args, char*);
	    break;
	case 'p':
	    dp->bin_encoding = svc_bin_type_ptr;
	    dp->tagged_union.v = SVC_VOID_TO_UNSIGNED(va_arg(args, void*));
	    break;
	case 'b':
	    /* This is a non-portable extension that intrudces into the
	     * space of printf formatters controlled by ANSI C.  Oh well. */
	    dp->bin_encoding = svc_bin_type_bytes;
	    dp->tagged_union.bytes.size = va_arg(args, int);
	    dp->tagged_union.bytes.data = (idl_char *)va_arg(args, char*);
	    break;
	}
	islong = FALSE;
	a_count++;
	dp++;
    }
    bin_data.count = a_count;

    /* Write it. */
    cancelmode = dce_svc__lock_for_write(fd);
    dce_svc_bin_data_encode(enc_h, &bin_data, &st);
    if (st != error_status_ok) {
	rpc_sm_client_free(enc_p, &st2);
	if (bin_data.args != values)		/* only if allocated */
	    free(bin_data.args);
	return;
    }
    (void)write(fd, (void *)enc_p, enc_size);

    /* Write a multiple of SVC_BLOCK_SIZE bytes. Figure out the "filler". */
    n = SVC_FILL_TO_BLOCK(enc_size);
    write(fd, null_buf, n);	/* null_buf is static, hence all nulls */
    idl_es_handle_free(&enc_h, &st);

    dce_svc__unlock_for_write(fd, cancelmode);

    rpc_sm_client_free(enc_p, &st);
    if (bin_data.args != values)
	free(bin_data.args);
}


/*
**  Set program name.
*/
void
dce_svc_set_progname(
    char			*progname,
    error_status_t		*st
)
{
    (void)strncpy(dce_svc_g_progname, progname, sizeof dce_svc_g_progname - 1);
    dce_svc_g_progname[sizeof dce_svc_g_progname - 1] = '\0';
    *st = svc_s_ok;
}


static const char *
dce_svc__filename(
    const char			*name
)
{
    const char			*p;

    if ((p = strrchr(name, '/')) == NULL)
	return name;
    return p + 1;
}


/*
**  Look at <dce/dce_svc.h>; it changes the name of this routine.
*/
void
dce_svc_printf(
    dce_svc_handle_t		handle,
#if	defined(DCE_SVC_WANT__FILE__)
    const char			*file,
    const int			line,
#endif	/* defined(DCE_SVC_WANT__FILE__) */
    const char			*argtypes,
    const unsigned32		table_index,
    const unsigned32		attributes,
    const unsigned32		message_index,
    ...
)
{
    struct dce_svc_prolog_s_t	prolog;
    va_list			args;
    error_status_t		st;
    dce_svc_routing_t		*r;
    dce_svc_routing_t		**rp;
    dce_svc_filter_proc_t	filter;
    int				i;
    int				cstate;		/* thread cancel state */
    int				goesto;

    /* Fill in prolog. */
    cstate = pthread_setcancel(CANCEL_OFF);
    prolog.format = (char *)dce_msg_get_msg(message_index, &st);
    LOCK();
    prolog.handle = handle;
    prolog.version = dce_svc_c_version;
    (void)utc_gettime(&prolog.t);
    prolog.argtypes = argtypes;
    prolog.table_index = table_index;
    prolog.attributes = attributes;
    prolog.message_index = message_index;
#if	defined(DCE_SVC_WANT__FILE__)
    prolog.file = dce_svc__filename(file);
    prolog.line = line;
#else
    prolog.file = "?";
    prolog.line = 0;
#endif	/* defined(DCE_SVC_WANT__FILE__) */
    prolog.thread_id = pthread_self();
    SET_PROGNAME(prolog.progname);

    /* If a filter is defined, give it the chance to reject. */
    if (handle->filter) {
	va_start(args, message_index);
	filter = (dce_svc_filter_proc_t)handle->filter;
	i = (*filter)(&prolog, args);
	va_end(args);
	if (i == FALSE)
	    goto Done;
    }

    /* If logging, do it. */
    if ((attributes & svc_c_route_nolog) == 0) {
	if ((attributes & svc__c_debugmask) != 0) {
	    /* It's a debug message; fill in debug values. */
	    if (!handle->setup && !dce_svc__debug_fillin(handle, &st))
		goto Done;
	    rp = handle->routes.routes;
	    i = handle->routes.nroutes;
	}
	else {
	    i = ATTRIBUTEStoINDEX(attributes);
	    if (dce_svc_g_routes[i].nroutes == 0)
		dce_svc__fillin(attributes, &st);
	    rp = dce_svc_g_routes[i].routes;
	    i = dce_svc_g_routes[i].nroutes;
	}

Start:
	while (--i >= 0) {
	    va_start(args, message_index);
	    r = *rp++;
	    if (r->maxcount && ++(r->count) > r->maxcount)
		dce_svc__rotate_route(r);
	    goesto = 0;
	    switch (r->route) {
	    default:
		dce_assert(handle, "Unknown route created" == NULL);
		/* FALLTHROUGH */
	    case route_discard:
	    case route_default:
		break;
	    case route_binfile:
		dce_svc__binprint(r->data.binfile.fd, &prolog, args);
		break;
	    case route_stdout:
		dce_svc__vfprintf(stdout, &prolog, TRUE, args);
		break;
	    case route_stderr:
		dce_svc__vfprintf(stderr, &prolog, TRUE, args);
		break;
	    case route_textfile:
		dce_svc__vfprintf(r->data.textfile.f, &prolog, FALSE, args);
		break;
	    case route_goesto:
		rp = dce_svc__goesto(r->data.goesto.where, &goesto);
		break;

	    }
	    va_end(args);
	    if (goesto) {
		i = goesto;
		goto Start;
	    }
	}
    }

    /* Handle special actions. */
    if (attributes & svc_c_route_stderr) {
	va_start(args, message_index);
	dce_svc__vfprintf(stderr, &prolog, TRUE, args);
	va_end(args);
    }
    if (attributes & svc_c_action_abort)
	abort();
    if (attributes & svc_c_action_exit_bad)
	exit(1);
    if (attributes & svc_c_action_exit_ok)
	exit(0);

Done:
    if (prolog.format)
	free((char *)prolog.format);
    UNLOCK();
    if (cstate == CANCEL_ON)
	cstate = pthread_setcancel(CANCEL_ON);
}


#if	defined(DCE_DEBUG)


/*
**  Debugging print with text string format spec.
*/
void
dce_svc__debug(
    dce_svc_handle_t		handle,
    const int			table_index,
    const unsigned32		debug_level,
    char			*format,
    ...
)
{
    dce_svc_subcomp_t		*tp;
    dce_svc_routing_t		*r;
    dce_svc_routing_t		**rp;
    struct dce_svc_prolog_s_t	prolog;
    error_status_t		st;
    va_list			args;
    int				i;
    int				cstate;		/* thread cancel state */
    int				goesto;

    cstate = pthread_setcancel(CANCEL_OFF);

    LOCK();
    /* Fill in routing. */
    if (!handle->setup && !dce_svc__debug_fillin(handle, &st)) {
	if (cstate == CANCEL_ON)
	    cstate = pthread_setcancel(CANCEL_ON);
	UNLOCK();
	return;
    }

    /* Ignoring this level? */
    tp = &handle->table[table_index];
    if ((tp->sc_level & svc__c_debugmask) < debug_level) {
	if (cstate == CANCEL_ON)
	    cstate = pthread_setcancel(CANCEL_ON);
	UNLOCK();
	return;
    }

    /* Fill in prolog. */
    prolog.handle = handle;
    prolog.format = format;
    prolog.version = dce_svc_c_version;
    (void)utc_gettime(&prolog.t);
    prolog.argtypes = "?";
    prolog.table_index = table_index;
    prolog.attributes = debug_level;
    prolog.message_index = 0;
    prolog.file = "?";
    prolog.line = 0;
    prolog.thread_id = pthread_self();
    SET_PROGNAME(prolog.progname);

    rp = handle->routes.routes;
    i = handle->routes.nroutes;
Start:
    while (--i >= 0) {
	va_start(args, format);
	r = *rp++;
	if (r->maxcount && ++(r->count) > r->maxcount)
	    dce_svc__rotate_route(r);
	goesto = 0;
	switch (r->route) {
	default:
	    dce_assert(handle, "Unknown debug route created" == NULL);
	    /* FALLTHROUGH */
	case route_discard:
	case route_default:
	    break;
	case route_binfile:
	    dce_svc__binprint(r->data.binfile.fd, &prolog, args);
	    break;
	case route_stdout:
	    dce_svc__vfprintf(stdout, &prolog, TRUE, args);
	    break;
	case route_stderr:
	    dce_svc__vfprintf(stderr, &prolog, TRUE, args);
	    break;
	case route_textfile:
	    dce_svc__vfprintf(r->data.textfile.f, &prolog, FALSE, args);
	    break;
	case route_goesto:
	    rp = dce_svc__goesto(r->data.goesto.where, &goesto);
	    break;
	}
	va_end(args);
	if (goesto) {
	    i = goesto;
	    goto Start;
	}
    }
    UNLOCK();
    if (cstate == CANCEL_ON)
	cstate = pthread_setcancel(CANCEL_ON);
}
#endif	/* defined(DCE_DEBUG) */


/*
**  Debugging print with message ID format spec.
**  Look at <dce/dce_svc.h>; it changes the name of this routine.
*/
void
dce_svc_bin_log(
    dce_svc_handle_t		handle,
#if	defined(DCE_SVC_WANT__FILE__)
    const char			*file,
    const int			line,
#endif	/* defined(DCE_SVC_WANT__FILE__) */
    const char			*argtypes,
    const int			table_index,
    const unsigned32		debug_level,
    const unsigned32		message_index,
    ...
)
{
    dce_svc_subcomp_t		*tp;
    dce_svc_routing_t		*r;
    dce_svc_routing_t		**rp;
    struct dce_svc_prolog_s_t	prolog;
    error_status_t		st;
    va_list			args;
    int				i;
    int				cstate;		/* thread cancel state */

    cstate = pthread_setcancel(CANCEL_OFF);

    LOCK();
    /* Fill in routing. */
    if (!handle->setup && !dce_svc__debug_fillin(handle, &st)) {
	if (cstate == CANCEL_ON)
	    cstate = pthread_setcancel(CANCEL_ON);
	UNLOCK();
	return;
    }

    /* Ignoring this level? */
    tp = &handle->table[table_index];
    if ((tp->sc_level & svc__c_debugmask) < debug_level) {
	if (cstate == CANCEL_ON)
	    cstate = pthread_setcancel(CANCEL_ON);
	UNLOCK();
	return;
    }

    /* Fill in prolog. */
    prolog.handle = handle;
    prolog.format = NULL;
    prolog.version = dce_svc_c_version;
    (void)utc_gettime(&prolog.t);
    prolog.argtypes = argtypes;
    prolog.table_index = table_index;
    prolog.attributes = debug_level;
    prolog.message_index = message_index;
#if	defined(DCE_SVC_WANT__FILE__)
    prolog.file = dce_svc__filename(file);
    prolog.line = line;
#else
    prolog.file = "?";
    prolog.line = 0;
#endif	/* defined(DCE_SVC_WANT__FILE__) */
    prolog.thread_id = pthread_self();
    SET_PROGNAME(prolog.progname);

    rp = handle->routes.routes;
    for (i = handle->routes.nroutes; --i >= 0; ) {
	va_start(args, message_index);
	r = *rp++;
	if (r->maxcount && ++(r->count) > r->maxcount)
	    dce_svc__rotate_route(r);
	switch (r->route) {
	default:
	    dce_assert(handle, "Unknown debug route created" == NULL);
	    /* FALLTHROUGH */
	case route_stdout:
	case route_stderr:
	case route_textfile:
	case route_discard:
	case route_default:
	case route_goesto:
	    break;
	case route_binfile:
	    dce_svc__binprint(r->data.binfile.fd, &prolog, args);
	    break;
	}
	va_end(args);
    }
    UNLOCK();
    if (cstate == CANCEL_ON)
	cstate = pthread_setcancel(CANCEL_ON);
}


/* Get the severity of a message, given the prolog. */
/* Note that we return a pointer to static storage so it's thread-safe. */

const char		*dce_svc_g_debug_table[9] = {
	"DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4", "DEBUG5",
	"DEBUG6", "DEBUG7", "DEBUG8", "DEBUG9",
};
#define ATTRIBUTEStoINDEX_DEBUG(i)    \
	(((i) & svc__c_debugmask) - 1)

const char *
dce_svc_inq_sev_name(
    unsigned32		attributes
)
{
    if ((attributes & svc__c_debugmask) != svc_c_debug_off) {
	return dce_svc_g_debug_table[ATTRIBUTEStoINDEX_DEBUG(attributes)];
    }
    return dce_svc_g_code_table[ATTRIBUTEStoINDEX(attributes)];
}

