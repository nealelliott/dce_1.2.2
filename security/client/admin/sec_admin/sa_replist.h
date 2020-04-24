/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sa_replist.h,v $
 * Revision 1.1.7.2  1996/03/11  13:27:31  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:22  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:02:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:54  root]
 * 
 * Revision 1.1.5.1  1994/06/17  18:41:27  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:12  devsrc]
 * 
 * Revision 1.1.2.2  1993/01/29  18:52:26  burati
 * 	Initial version
 * 	[1993/01/14  23:00:13  burati]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992, 1993
 * Unpublished work. All Rights Reserved.
 */

PUBLIC void sa_refresh_replist(
#ifdef __STDC__
    sec_rgy_handle_t	context,
    error_status_t	*stp
#endif
);

PUBLIC void sa_refresh_full_replist(
#ifdef __STDC__
    sec_rgy_handle_t	context,
    error_status_t	*stp
#endif
);

PUBLIC void sa_display_replist(
#ifdef __STDC__
    boolean32		luuid,
    boolean32		laddr,
    boolean32		lstate,
    error_status_t	*stp
#endif
);

PUBLIC void sa_display_full_replist(
#ifdef __STDC__
    boolean32		luuid,
    boolean32		laddr,
    boolean32		lstate,
    boolean32		lprop,
    error_status_t	*stp
#endif
);

PUBLIC void sa_bind_to_rep_name(
#ifdef __STDC__
    unsigned_char_p_t	rep_name,
    sec_rgy_handle_t	*ctxtP,
    boolean32		authenticated,
    error_status_t      *status
#endif
);

PUBLIC boolean32 sa_get_rep_id_from_name(
#ifdef __STDC__
    unsigned_char_p_t	rep_name,
    uuid_p_t            rep_id
#endif
);
