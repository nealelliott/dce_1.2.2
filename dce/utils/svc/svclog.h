/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Declarations for reading serviceability logs.
 */

/*
 * HISTORY
 * $Log: svclog.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:15  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:38:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:10:07  root]
 * 
 * Revision 1.1.4.4  1994/09/06  21:27:37  bowe
 * 	Remove unused functions: dce_svc_log_get_arg(), et al. [CR 12049]
 * 	[1994/09/06  21:08:56  bowe]
 * 
 * Revision 1.1.4.3  1994/06/09  16:06:30  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:00  devsrc]
 * 
 * Revision 1.1.4.2  1994/05/26  18:51:52  bowe
 * 	Added #include for svcbin.h.  Adjusted dce_svc_log_prolog_t
 * 	for using ES for list of args.  [CR 10483,10478]
 * 	[1994/05/26  18:24:43  bowe]
 * 
 * Revision 1.1.4.1  1994/04/26  19:21:33  rsalz
 * 	Move some field from prolog into log handle (OT CR 10461)
 * 	[1994/04/26  17:02:29  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:10:07  rsalz
 * 	Initial release
 * 	[1993/08/16  18:04:45  rsalz]
 * 
 * $EndLog$
 */

#if	!defined(_DCE_SVCLOG_H)
#define _DCE_SVCLOG_H

#include <dce/svcbin.h>

typedef struct dce_svc_log_handle_s_t *dce_svc_log_handle_t;


typedef struct dce_svc_log_prolog_s_t {
    int				version;
    utc_t			t;
    unsigned32			attributes;
    unsigned32			message_index;
    pthread_t			thread_id;
    unsigned32			thread_code;
    char			*argtypes;
    char			*fac_name;
    char			*message_text;
    char			*progname;
    char			*file;
    int				line;
    int				n_args;
    svc_bin_arg_t		*args;
} *dce_svc_log_prolog_t;


extern void dce_svc_log_open(
    char*			/* name */,
    dce_svc_log_handle_t*	/* handle */,	
    error_status_t*		/* status */
);


extern void dce_svc_log_get(
    dce_svc_log_handle_t	/* handle */,	
    dce_svc_log_prolog_t*	/* prolog */,
    error_status_t*		/* status */
);


extern void dce_svc_log_rewind(
    dce_svc_log_handle_t	/* handle */,	
    error_status_t*		/* status */
);


extern void dce_svc_log_close(
    dce_svc_log_handle_t	/* handle */,	
    error_status_t*		/* status */
);


extern void dce_svc_log_free_entry(
    dce_svc_log_handle_t	/* handle */,	
    error_status_t*		/* status */
);

#endif	/* !defined(_DCE_SVCLOG_H) */
