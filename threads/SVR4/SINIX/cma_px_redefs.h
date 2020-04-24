/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: cma_px_redefs.h,v $
 * Revision 1.1.4.2  1996/02/18  23:08:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:47  marty]
 *
 * Revision 1.1.4.1  1995/12/07  21:54:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:02:50  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:52:40  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:49:39  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/14  21:31:41  ohara
 * 	new file for OSF
 * 	[1994/04/14  20:25:16  ohara]
 * 
 * 	created file -- added wrapper for sigprocmask() (see CR# 690)
 * 
 * $EndLog$
 */
#ifndef R_PX_REDEFS
#define R_PX_REDEFS

#if 0

/* The definition of sigprocmask is now in libc (libc_r)
 * see CR# 690 for details 
 */

#undef sigprocmask
#ifdef __STDC__ 
#pragma weak sigprocmask = _sigprocmask
#define cma_sigprocmask _sigprocmask
#else /* no __STDC__ */
#define cma_sigprocmask sigprocmask
#endif /* __STDC__ */

#endif /* 0 */

#endif /*  R_PX_REDEFS */
