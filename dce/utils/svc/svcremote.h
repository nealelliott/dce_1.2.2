/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Declarations of remote-implementations routines not already declared.
 */

/*
 * HISTORY
 * $Log: svcremote.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:17  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:38:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:10:17  root]
 * 
 * Revision 1.1.4.2  1994/06/09  16:06:35  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:05  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/10  19:37:43  rsalz
 * 	Add debug_values parameter to dce_svc_routings (OT CR 9690).
 * 	Should have been done when service.idl was changed at:
 * 	    Revision 1.1.2.3  1993/09/02  17:42:57  rsalz
 * 	[1994/01/06  20:30:15  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:10:32  rsalz
 * 	Initial release
 * 	[1993/08/16  18:05:04  rsalz]
 * 
 * $EndLog$
 */

#if	!defined(_DCE_SVCREMOTE_H)
#define _DCE_SVCREMOTE_H

extern void dce_svc_components(
    dce_svc_stringarray_t*	/* table */,
    error_status_t*		/* status */
);

extern void dce_svc_table(
    dce_svc_string_t		/* component */,
    dce_svc_subcomparray_t*	/* table */,
    error_status_t*		/* status */
);

extern void dce_svc_filter(
    dce_svc_string_t		/* component */,
    idl_long_int		/* arg_size */,
    idl_byte*			/* argument */,
    error_status_t*		/* status */
);

extern void dce_svc_routings(
    dce_svc_stringarray_t*	/* table */,
    boolean32			/* debug_values */,
    error_status_t*		/* status */
);
#endif	/* !defined(_DCE_SVCREMOTE_H) */
