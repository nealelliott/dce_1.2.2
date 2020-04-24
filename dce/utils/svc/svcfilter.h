/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Serviceability filtering header file.  Requires <stdarg.h>
 */

/*
 * HISTORY
 * $Log: svcfilter.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:12  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:38:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:10:02  root]
 * 
 * Revision 1.1.4.1  1994/06/09  16:06:27  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:57  devsrc]
 * 
 * Revision 1.1.2.2  1993/08/16  18:09:53  rsalz
 * 	Initial release
 * 	[1993/08/16  18:04:36  rsalz]
 * 
 * $EndLog$
 */

#if	!defined(_DCE_SVCFILTER_H)
#define _DCE_SVCFILTER_H


typedef struct dce_svc_prolog_s_t {
    dce_svc_handle_t		handle;
    int				version;
    utc_t			t;
    const char			*argtypes;
    unsigned32			table_index;
    unsigned32			attributes;
    unsigned32			message_index;
    char			*format;
    const char			*file;
    char			progname[dce_svc_c_progname_buffsize];
    int				line;
    pthread_t			thread_id;
} *dce_svc_prolog_t;


typedef boolean (*dce_svc_filter_proc_t)(
    const dce_svc_prolog_t	/* prolog */,
    va_list			/* args */
);


typedef void (*dce_svc_filterctl_proc_t)(
    int				/* arg_size */,
    unsigned char*		/* arg */,
    error_status_t*		status
);


/*
**  Attach a filter and a filter-control to a component.
*/
extern void dce_svc_define_filter(
    dce_svc_handle_t		/* handle */,
    dce_svc_filter_proc_t	/* filter_function */,
    dce_svc_filterctl_proc_t	/* filter_ctl_function */,
    error_status_t*		/* status */
);

#endif	/* !defined(_DCE_SVCFILTER_H) */
