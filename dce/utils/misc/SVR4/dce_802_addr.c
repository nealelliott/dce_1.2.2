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
 * Revision 1.1.4.2  1996/02/18  19:21:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:46  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:08:50  root]
 * 
 * Revision 1.1.2.1  1994/06/22  17:51:39  bowe
 * 	initial checkin
 * 	[1994/06/22  16:48:20  bowe]
 * 
 * $EndLog$
 */

/*
 * dce_get_802_addr
 *
 * Return the IEEE 802 (ie, ethernet hardware) address.
 */

#include <fcntl.h>
#include <dce/dce.h>
#include <dce/dce_utils.h>

void
dce_get_802_addr (
    dce_802_addr_t	*addr,
    error_status_t	*st
)
{
    int		fd;
    char	buf[13];
    int		e[6];
    int		i;

    *st = error_status_ok;

#ifdef RPC_UUID_KERNEL
    /* This resides in rpc/runtime/SVR4/get_unique_addr.c */
    /* It may not work, since the structure may no longer match */
    if (get_unique_addr(addr, st) == 0 && *st == error_status_ok) 
	return;
#endif

    fd = open (IEEE_802_FILE, O_RDONLY);
    if (fd < 0) {
	*st = utils_s_802_cant_read;
	return;
    }

    if (read (fd, buf, 12) < 12) {
	*st = utils_s_802_cant_read;
	return;
    }
    close(fd);

    buf[12] = 0;

    if (sscanf (buf, "%2x%2x%2x%2x%2x%2x", 
		&e[0], &e[1], &e[2], &e[3], &e[4], &e[5]) != 6) {
	*st = utils_s_802_addr_format;
	return;
    }

    for (i = 0; i < 6; i++)
       addr->eaddr[i] = e[i];
}

