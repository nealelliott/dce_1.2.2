/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: void_assoc.c,v $
 * Revision 1.1.2.1  1996/06/05  21:28:01  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:16:34  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * ptyint_void_association(): Void association with controlling terminal
 *
 * Copyright 1995 by the Massachusetts Institute of Technology.
 *
 * 
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby
 * granted, provided that the above copyright notice appear in all
 * copies and that both that copyright notice and this permission
 * notice appear in supporting documentation, and that the name of
 * M.I.T. not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability
 * of this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 * 
 */

#include <com_err.h>
#include "libpty.h"
#include "pty-int.h"

long ptyint_void_association()
{
            int con_fd;
#ifdef HAVE_SETSID
    (void) setsid();
#endif

        /* Void tty association first */
#ifdef TIOCNOTTY
        if ((con_fd = open("/dev/tty", O_RDWR)) >= 0) {
          ioctl(con_fd, TIOCNOTTY, 0);
          close(con_fd);
	}
#endif
	    return 0;
}
