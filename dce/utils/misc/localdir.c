/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: localdir.c,v $
 * Revision 1.1.4.2  1996/02/18  19:22:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:15  root]
 * 
 * Revision 1.1.2.3  1994/06/09  16:05:49  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:23  devsrc]
 * 
 * Revision 1.1.2.2  1994/01/27  20:05:27  bowe
 * 	Declare as 'char *dcelocal_path', not 'char dcelocal_path[]'.
 * 	Same for dceshared_path.
 * 	[1994/01/27  19:30:42  bowe]
 * 
 * Revision 1.1.2.1  1994/01/25  22:53:25  bowe
 * 	Initial checkin - moved here from ./dce/utils
 * 	[1994/01/25  22:38:56  bowe]
 * 
 * Revision 1.1.2.3  1993/07/14  19:31:09  melman
 * 	Changed localdir to dcelocal_path and added dceshared_path
 * 	[1993/07/14  19:30:43  melman]
 * 
 * Revision 1.1.2.2  1993/07/14  19:09:13  melman
 * 	Initial version
 * 	[1993/07/14  18:49:36  melman]
 * 
 * $EndLog$
 */

const char *dcelocal_path="/opt/dcelocal";
const char *dceshared_path="/opt/dcelocal";

