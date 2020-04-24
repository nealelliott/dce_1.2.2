/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dce_802_addr.c,v $
 * Revision 1.1.4.2  1996/02/18  19:21:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:36:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:08:48  root]
 * 
 * Revision 1.1.2.1  1994/06/22  17:51:38  bowe
 * 	initial checkin
 * 	[1994/06/22  16:48:18  bowe]
 * 
 * $EndLog$
 */

/*
 * dce_get_802_addr
 *
 * Return the IEEE 802 (ie, ethernet hardware) address.
 */

#include <fcntl.h>
#include <sys/mode.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/utsname.h>        /* uname system call definitions        */
#include <sys/devinfo.h>        /* device info structure declaration    */

#include <dce/dce.h>
#include <dce/dce_utils.h>

#define DNAME	"/dev"

void
dce_get_802_addr (
    dce_802_addr_t	*addr,
    error_status_t	*st
)
{
    DIR			*dir_p;		/* pointer to a directory structure */

    struct dirent	*direntry;	/* pointer to a directory entry     */

    char		*d_dev = "/dev/"; /* path to special files         */

    char		dev[32];	/* array containing full path name  */
					/* of a token ring or ethernet      */
					/* special file		            */
	    
    int			fd;		/* file descriptor		    */

    int			rc;		/* return code			    */

    int			i;		/* loop counter			    */

    struct devinfo	devchar;	/* device information stored in     */
					/* in this variable                 */

    struct xutsname	uname_buf;	/* buffer which holds the output of */
					/* the uname system call. Used to   */
					/* get the machine id.              */

    unsigned long	machine_id;	/* variable used to store the       */
					/* machine id returned by uname     */

    /**************************************************************************
    * Start of code                                                           *
    **************************************************************************/

    *st = error_status_ok;	/* be optimistic */

    /* 
     * Open the /dev directory
     */

    dir_p = opendir(DNAME);

    while (1) {
	/* do until we either find a valid hardware address or run */
        /* out of directory entries in /dev                        */

	direntry = readdir(dir_p);	/* read the next directory entry */

	if (direntry == NULL)
	    break;		/* no more directory entries to read */

	strcpy(dev, d_dev);
#if 0
	i = 0;
	while ( (dev[i] = d_dev[i]) != '\0')
	    i++;	/*  initialize the name of the device to "/dev" */
#endif

	if ( strncmp(direntry->d_name, "tok", 3) == 0 ) {
	    /* found a valid token ring special device file */
	    strcat(dev, direntry->d_name);
	    if (fd = open(dev, O_RDONLY, 0)) {
		/* open was successful, get hw address */
		if ( (rc = ioctl(fd, IOCINFO, &devchar)) != -1 ) {
		    for (i=0; i<6; i++)
			addr->eaddr[i] = devchar.un.token.haddr[i];
		    close(fd);
		    return;
		}
		close(fd);
	    }
	}
	else if ( strncmp(direntry -> d_name, "ent", 3) == 0 )	{
	    /* found a valid ethernet special device file */
	    strcat(dev, direntry->d_name);
	    if (fd = open(dev, O_RDONLY, 0)) {
		/* open was successful, get hw address */
		if ( (rc = ioctl(fd, IOCINFO, &devchar)) != -1 ) {
		    for (i=0; i<6; i++)
			addr->eaddr[i] = devchar.un.ethernet.haddr[i];
		    close(fd);
		    return;
		}
		close(fd);
	    }
	}
    } /* end while searching for address of an ethernet or token ring adapter */


    /*
     * If we get here we didn't find the address of an ethernet or token
     * ring adapter.
     * Call uname to get the unique machine id of the system
     */

    rc = unamex(&uname_buf);
    if (rc == -1) /* an error occured */ {
	*st = utils_s_802_uname_failure;
	return;
    }

    machine_id = uname_buf.nid;             /* node id stored in uname_buf */
		
    addr->eaddr[0] = 0x00;
    addr->eaddr[1] = 0x00;	
    addr->eaddr[2] = machine_id >> 24;
    addr->eaddr[3] = machine_id >> 16;
    addr->eaddr[4] = machine_id >> 8;
    addr->eaddr[5] = machine_id;
}

