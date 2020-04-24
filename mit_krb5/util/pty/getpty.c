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
 * $Log: getpty.c,v $
 * Revision 1.1.2.1  1996/06/05  21:25:45  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:14:59  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: getpty.c,v $
 * Revision 1.1.2.1  1996/06/05  21:25:45  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:14:59  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  20:14:59  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 *
 * Revision /main/DCE_1.2.2/1  1996/05/30  21:19 UTC  mullan_s
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 
 * Revision /main/mullan_dce1.2.2_krb5_drop/1  1996/05/29  20:45 UTC  mullan_s
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 
 * Revision /main/mullan_dce_k5_bsd44_rutils/1  1996/05/16  19:49 UTC  mullan_s
 * 	Put angle brackets around pty-int.h - DCE will look in
 * 	export tree for include file instead of current directory.
 * 
 * $EndLog$
 */
/*
 * pty_getpty: open a PTY master.
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
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#include <com_err.h>
#include "libpty.h"
#ifdef OSF_DCE
#include <pty-int.h>
#else
#include "pty-int.h"
#endif

long pty_getpty (fd, slave, slavelength)
    int slavelength;
    int *fd; char *slave;
{
    char c;
    char *p;
    int i,ptynum;
    struct stat stb;
char slavebuf[1024];


#ifdef HAVE_OPENPTY
    int slavefd;

    if(openpty(fd, &slavefd, slave, (struct termios *) 0,
         (struct winsize *) 0)) return 1;
close(slavefd);
    return 0;
#else

    *fd = open("/dev/ptmx", O_RDWR|O_NDELAY);	/* Solaris, IRIX */
    if (*fd < 0) *fd = open("/dev/ptc", O_RDWR|O_NDELAY); /* AIX */
    if (*fd < 0) *fd = open("/dev/pty", O_RDWR|O_NDELAY); /* sysvimp */

    if (*fd >= 0) {

#ifdef HAVE_GRANTPT
	if (grantpt(*fd) || unlockpt(*fd)) return PTY_GETPTY_STREAMS;
#endif
    
#ifdef HAVE_PTSNAME
	p = ptsname(*fd);
#else
#ifdef	HAVE_TTYNAME
	p = ttyname(*fd);
#else
	/* XXX If we don't have either what do we do */
#endif
#endif
	if (p) {
	    if ( strlen(p) > slavelength)
		{
		    close (*fd);
		    *fd = -1;
		    return PTY_GETPTY_SLAVE_TOOLONG;
		}
	    
	    strcpy(slave, p);
	    return 0;
	}

	if (fstat(*fd, &stb) < 0) {
	    close(*fd);
	    return PTY_GETPTY_FSTAT;
	}
	ptynum = (int)(stb.st_rdev&0xFF);
    sprintf(slavebuf, "/dev/ttyp%x", ptynum);
    if ( strlen(slavebuf) > slavelength) {
	close(*fd);
	*fd = -1;
	return PTY_GETPTY_SLAVE_TOOLONG;
    }
    strncpy ( slave, slavebuf, slavelength);
	return 0;

    } else {
    
	for (c = 'p'; c <= 's'; c++) {
	    sprintf(slavebuf,"/dev/ptyXX");
	    slavebuf[strlen("/dev/pty")] = c;
	    slavebuf[strlen("/dev/ptyp")] = '0';
	    if (stat(slavebuf, &stb) < 0)
		break;
	    for (i = 0; i < 16; i++) {
		slavebuf[sizeof("/dev/ptyp") - 1] = "0123456789abcdef"[i];
		*fd = open(slavebuf, O_RDWR);
		if (*fd < 0) continue;

		/* got pty */
		slavebuf[strlen("/dev/")] = 't';
		if ( strlen(slavebuf) > slavelength ) {
		    close ( *fd);
		    *fd = -1;
return PTY_GETPTY_SLAVE_TOOLONG;
		}
		strncpy ( slave, slavebuf, slavelength);
		
		return 0;
	    }
	}
	return PTY_GETPTY_NOPTY;
    }
#endif /* HAVE_OPENPTY */
}

