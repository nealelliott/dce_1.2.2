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
 * $Log: open_slave.c,v $
 * Revision 1.1.2.1  1996/06/05  21:26:52  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:15:47  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * pty_open_slave: open slave side of terminal, clearing for use.
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


long pty_open_slave ( slave, fd)
    const char *slave;
    int *fd;
{
    int vfd;
    long retval;
#ifdef POSIX_SIGNALS
    struct sigaction sa;
    /* Initialize "sa" structure. */
    (void) sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
#endif


    /* First, chmod and chown the slave*/
    /*
       * If we have vhangup then we really need pty_open_ctty to make sure
       * Our controlling terminal is the pty we're opening.  However, if we
       * are using revoke or nothing then we just need  a file descriiptor
       * for the pty.  Considering some OSes in this category break on
       * the second call to open_ctty (currently OSF but others may),
       * we simply use a descriptor if we can.
       */
#ifdef VHANG_FIRST
    if (( retval = pty_open_ctty ( slave, &vfd )) != 0 )
      return retval;
        if (vfd < 0)
	return PTY_OPEN_SLAVE_OPENFAIL;

#endif
    

        if (chmod(slave, 0))
	    return PTY_OPEN_SLAVE_CHMODFAIL;
	if ( chown(slave, 0, 0 ) == -1 ) 
	  return PTY_OPEN_SLAVE_CHOWNFAIL;

#ifdef VHANG_FIRST
    ptyint_vhangup();
    (void) close(vfd);
#endif
    
    if ( (retval = ptyint_void_association()) != 0)
      return retval;
    
#ifdef HAVE_REVOKE
    if (revoke (slave) < 0 ) {
	return PTY_OPEN_SLAVE_REVOKEFAIL;
    }
#endif /*HAVE_REVOKE*/

/* Open the pty for real. */
    if  (( retval = pty_open_ctty ( slave, fd))  != 0 ) {
	return PTY_OPEN_SLAVE_OPENFAIL;
    }
    return pty_initialize_slave (*fd);
}


