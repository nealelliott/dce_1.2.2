/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: svcdumplog.c,v $
 * Revision 1.1.4.2  1996/02/18  19:22:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:38:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:58  root]
 * 
 * Revision 1.1.2.7  1994/09/13  15:27:09  rsalz
 * 	Make dce_msg__inq_comp thread-safe (OT CR 12067).
 * 	[1994/09/13  05:00:43  rsalz]
 * 
 * Revision 1.1.2.6  1994/08/31  14:24:16  bowe
 * 	Remove not-needed #includes [CR 11860]
 * 	[1994/08/31  14:15:24  bowe]
 * 
 * Revision 1.1.2.5  1994/07/27  12:28:01  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:34:53  rsalz]
 * 
 * Revision 1.1.2.4  1994/07/08  21:42:41  bowe
 * 	Print the file and line number with rest of prolog. [CR 10479]
 * 	[1994/07/08  21:41:37  bowe]
 * 
 * Revision 1.1.2.3  1994/07/08  17:11:50  bowe
 * 	Add call to dce_svc_init() [CR 11156]
 * 	[1994/07/08  16:35:01  bowe]
 * 
 * Revision 1.1.2.2  1994/06/27  19:13:34  bowe
 * 	When processign args, check for null pointer [CR 11055]
 * 	[1994/06/27  18:31:45  bowe]
 * 
 * Revision 1.1.2.1  1994/05/26  18:51:45  bowe
 * 	Initial checkin
 * 	[1994/05/26  18:09:18  bowe]
 * 
 * $EndLog$
 */

/*
 * Dump a binary serviceability log file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <ctype.h>
#include <dce/dce.h>
#include <pthread.h>
#include <dce/utc.h>
#include <dce/svclog.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>

int main(int, char**);

int
main(int ac, char *av[])
{
    int				n, i, ai, count, skip;
    error_status_t		st;
    dce_svc_log_handle_t	h;
    dce_svc_log_prolog_t	prolog;
    char			buf[UTC_MAX_STR_LEN];
    char			*p, *cp, *prog;
    svc_bin_arg_t		*dp;
    idl_byte			*bp;
    boolean			islong;
    char			fmt[60];	/* > any printf fmt string */
    char			comp_name[4];

    /* Parse JCL. */
    setlocale(LC_ALL, "");
    prog = av[0];
    skip = 0;
    while ((i = getopt(ac, av, "s:")) != EOF)
	switch (i) {
	default:
	    dce_fprintf(stderr, svc_s_dump_usage, prog);
	    exit(1);
	    /* NOTREACHED */
	case 's':	skip = atoi(optarg);	break;
	}
    ac -= optind;
    av += optind;
    if (ac != 1) {
	dce_fprintf(stderr, svc_s_dump_usage, prog);
	exit(1);
    }

    /* Open the log. */
    dce_svc_log_open(av[0], &h, &st);
    if (st != svc_s_ok) {
	(void)fprintf(stderr, "Error opening log file \"%s\": %s\n",
		av[0], dce_msg_get_msg(st, &st));
	exit(1);
    }

    dce_svc_init(0, &st);

    /* Loop over all entries. */
    for (count = 1; ; count++) {
	/* Get next entry. */
	dce_svc_log_get(h, &prolog, &st);
	if (st == svc_s_at_end)
	    break;
	if (st != svc_s_ok) {
	    (void)fprintf(stderr, "Error reading log: %s\n",
		dce_msg_get_msg(st, &st));
	    dce_svc_log_close(h, &st);
	    return 1;
	}
	if (skip && --skip)
	    continue;

	/* Print the prolog part. */
	(void)utc_asclocaltime(buf, sizeof buf, &prolog->t);
	dce_msg__inq_comp(prolog->message_index, comp_name);
	if (prolog->file) {
	    printf("%s %s %s %s %s %s %d 0x%8.8lx\n", buf, prolog->progname,
		dce_svc_inq_sev_name(prolog->attributes), comp_name,
		prolog->fac_name, prolog->file, prolog->line,
		(unsigned long)prolog->thread_code);
	}
	else {
	    printf("%s %s %s %s %s 0x%8.8lx\n", buf, prolog->progname,
		dce_svc_inq_sev_name(prolog->attributes), comp_name,
		prolog->fac_name, (unsigned long)prolog->thread_code);
	}

	/* Now, process the message and args */
	for (ai=0,p=prolog->message_text; p && *p; p++) {
	    if (*p != '%') {
		putchar(*p);
		continue;
	    }
	    p++;
	    if (*p == '%') {		/* 2 %'s in a row? */
		putchar('%');
		continue;
	    }
	    n = ai;
	    /* Check the XPG4 conversion, something like: %1$s.  XPG4 says the
	     * numbers are in range 1-9, but we'll accept more.
	     */
	    if (isdigit(*p) && (p[1] == '$' || p[2] == '$')) {
		n = atoi(p) - 1;
		if (n < 0) break;	/* something is wrong if < 0 ... */
		while(*p != '$' && *p) p++;
		p++;			/* past dollar sign */
	    }
	    /* Now, copy the conversion part of format string,
	     * without the "N$" part.
	     */
	    for (cp = fmt, *cp++ = '%', islong = FALSE; ; ) {
		/* Copy until we hit end of the conversion part. It ends with
		 * one of these characters.  (see ANSI printf manpage)
		 */
		*cp++ = *p;
		if (*p == 's' || *p == 'c' || *p == 'p' ||
		    *p == 'd' || *p == 'i' || *p == 'u' || *p == 'o' ||
		    *p == 'x' || *p == 'X' ||
		    *p == 'f' || *p == 'e' || *p == 'E' ||
		    *p == 'g' || *p == 'G' || *p == 'b' )
		    break;
		if (*p == 'l')
		    islong = TRUE;
		p++;
	    }
	    *cp = 0;
	    /* p now points to conversion type (the last character of it) */
	    dp = &prolog->args[n];
	    switch (*p) {
	    default:	break;	/* Do something here? */
	    case 's':	printf(fmt, dp->tagged_union.s);	break;
	    case 'c':	printf(fmt, dp->tagged_union.c);	break;
	    case 'p':	printf(fmt, dp->tagged_union.v);	break;
	    case 'd': case 'i':
	    case 'x': case 'X': case 'o':
		if (islong)
		    printf(fmt, dp->tagged_union.l);
		else
		    printf(fmt, dp->tagged_union.i);
		break;
	    case 'u':
		if (islong)
		    printf(fmt, dp->tagged_union.ul);
		else
		    printf(fmt, dp->tagged_union.ui);
		break;
	    case 'f': case 'e': case 'E': case 'g': case 'G':
		printf(fmt, dp->tagged_union.d);
		break;
	    case 'b':
		for (i=0,bp=dp->tagged_union.bytes.data;
			i<dp->tagged_union.bytes.size; bp++,i++)
		    printf("%s%02x", i?" ":"", (unsigned int)*bp);
		break;
	    }
	    ai++;
	    islong = FALSE;
	}
	putchar('\n');
    }

    dce_svc_log_close(h, &st);
    return 0;
}

