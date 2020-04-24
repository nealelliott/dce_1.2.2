/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dce_priv_proc.c,v $
 * Revision 1.1.4.2  1996/02/18  19:21:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:51  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:00  root]
 * 
 * Revision 1.1.2.2  1994/06/09  16:05:44  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:21  devsrc]
 * 
 * Revision 1.1.2.1  1994/01/25  22:53:14  bowe
 * 	Initial checkin - moved here from ./dce/utils
 * 	[1994/01/25  22:38:49  bowe]
 * 
 * Revision 1.1.2.5  1993/10/05  12:02:39  melman
 * 	Removed dce/ from include dce_priv_proc.h
 * 	[1993/10/05  12:02:26  melman]
 * 
 * Revision 1.1.2.4  1993/09/20  18:52:54  melman
 * 	Really added dce_priv_proc.h prototypes
 * 	[1993/09/20  18:52:42  melman]
 * 
 * Revision 1.1.2.2  1993/08/26  12:38:00  melman
 * 	Renamed sec_priv_* to dce_priv_*
 * 	[1993/08/26  12:37:19  melman]
 * 
 * Revision 1.1.2.2  1993/07/14  19:09:18  melman
 * 	Initial version
 * 	[1993/07/14  18:49:39  melman]
 * 
 * $EndLog$
 */
/*
 * From RFC 17.0:
 *
 * Presently, DCE relies upon a uid of 0 to determine appropriate (root)
 * privilege (e.g., src/security/server/rs/rs_main.c).  The POSIX
 * standard does not prescribe that uid 0 is the root or superuser.  In fact,
 * uid 0 need not exist for POSIX compliance.  Hence, the mechanism DCE
 * employs to check for appropriate privilege, namely setuid(0) and
 * subsequent checks for geteuid() == 0, may not suffice for some
 * compliant POSIX systems.  In fact, these will fail where a uid of 0 is not
 * allowed/defined.
 * 
 * This module is the PMAX implementation of these routines.  It will
 * reside in a machine specific directory (should be OS specific when 
 * we get those) and will require porting to other platforms.  
 */

#include <dce/dce.h>
#include <unistd.h>
#include <dce_priv_proc.h>

/* 
 * Obtains appropriate (root) privileges for a server 
 * Returns 0 on success, -1 on failure and sets errno.
 */
int dce_priv_proc_enable(void)
{
    return setuid((uid_t)0);
}

/*  Disable appropriate (root) privileges for a server */
int dce_priv_proc_disable(void)
{
    return setuid(getuid());
}

/* Returns TRUE if privileged, FALSE if not */
boolean dce_priv_proc_is_appropriate(void)
{
    return (geteuid() == 0) ? TRUE : FALSE;
}

