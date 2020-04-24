/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dce_priv_proc.h,v $
 * Revision 1.1.4.2  1996/02/18  23:32:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:02  root]
 * 
 * Revision 1.1.2.2  1994/06/09  16:05:45  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:22  devsrc]
 * 
 * Revision 1.1.2.1  1994/01/25  22:53:16  bowe
 * 	Initial checkin - moved here from ./dce/utils
 * 	[1994/01/25  22:38:50  bowe]
 * 
 * Revision 1.1.2.1  1993/09/20  17:50:09  melman
 * 	Added dce_priv_proc.h prototypes
 * 	[1993/09/20  17:48:28  melman]
 * 
 * $EndLog$
 */

extern int dce_priv_proc_enable(void);
extern int dce_priv_proc_disable(void);
extern boolean dce_priv_proc_is_appropriate(void);

