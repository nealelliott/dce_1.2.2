/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: editline.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:02  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:08  root]
 * 
 * Revision 1.1.2.1  1994/01/25  22:53:20  bowe
 * 	Initial checkin - moved here from ./dce/utils
 * 	[1994/01/25  22:38:53  bowe]
 * 
 * Revision 1.1.2.3  1993/12/10  00:04:10  melman
 * 	Made editline.h a private editlinep.h and added a public editline.h
 * 	[1993/12/10  00:03:53  melman]
 * 
 * $EndLog$
 */

#ifndef _EDITLINE_H
#define _EDITLINE_H

char *readline(char *);
void add_history(char *);

#endif

