/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_login_np.h,v $
 * Revision 1.1.6.2  1996/03/11  13:27:54  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:42  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:05:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:31:31  root]
 * 
 * Revision 1.1.4.3  1992/12/29  13:01:45  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:39:44  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/04  21:29:32  burati
 * 	CR5978 Changes to separate OS dependent code into machine dep dirs
 * 	[1992/12/04  21:20:21  burati]
 * 
 * Revision 1.1.2.2  1992/06/17  18:34:20  burati
 * 	CR3482 Fix passing of pointer to num_groups
 * 	[1992/06/17  16:14:42  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  sec_login_np.h V=1 10/08/91 //littl/prgy/src/client/sec_login
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *     Non-portable sec_login_routines
 */

#ifndef sec_login_np__included
#define sec_login_np__included

/*
 * This signature may need to change depending on the specifics
 * of the "struct passwd" type on the target system
 * Memory is allocated for "groups" and must be freed
 * by the caller.  
 */
PUBLIC  void  sec_login_pvt_get_local_login_info (
#ifdef __STDC__
    char            *name,      
    char            pw_passwd[],
    int             *pw_uid,
    int             *pw_gid,
    char            pw_gecos[],
    char            pw_dir[],
    char            pw_shell[],
    unsigned16      *num_groups,
    signed32        **groups,
    error_status_t  *stp
#endif
);

PUBLIC void sec_login_np_lc_to_pwent(
#ifdef __STDC__
    sec_login_context_t *lcP,
    sec_login_passwd_t  *pwdP,
    error_status_t      *stP
#endif /* __STDC__ */
);

#endif /* sec_login_np__included */
