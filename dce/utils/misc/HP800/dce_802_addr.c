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
 * Revision 1.1.4.2  1996/02/18  19:21:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:43  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:36:56  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/18  14:15 UTC  tatsu_s
 * 	Merged from rpc/runtime/HP800/uuidsys.c.
 * 	  Streams support (DLPI).
 * 	[1995/12/08  18:08:45  root]
 * 
 * Revision 1.1.2.1  1994/06/22  17:51:37  bowe
 * 	initial checkin
 * 	[1994/06/22  16:48:17  bowe]
 * 
 * $EndLog$
 */

/*
 * dce_get_802_addr
 *
 * Return the IEEE 802 (ie, ethernet hardware) address.
 */

#include <sys/fcntl.h>
#include <netio.h>

#include <dce/dce.h>
#include <dce/dce_utils.h>

#ifndef HPDCE_RPC_DLPI_SUPPORT
#if __hpux1000p
#include <stream.h>
#include <sys/stropts.h>
#include <sys/dlpi.h>
#include <sys/dlpi_ext.h>
#endif
#endif

#define LAN_DEV0 "/dev/lan0"

void
dce_get_802_addr (
    dce_802_addr_t	*addr,
    error_status_t	*st
)
{
#ifndef HPDCE_RPC_DLPI_SUPPORT
#if __hpux1000p
    struct strbuf control;
    long buff[128]; 
    dl_hp_ppa_req_t *ppa_req;
    dl_hp_ppa_ack_t *ppa_ack;
    dl_hp_ppa_info_t *ppa_info;
    int fd, flags = 0, i;

    *st = utils_s_802_cant_read;

    /* 
     * /dev/dlpi is the cloneable DLPI dev file 
     */
    if ((fd = open("/dev/dlpi", O_RDWR)) == -1)
        return;

    control.maxlen = 4 * 128;
    control.buf = (char *) buff;

    control.len = sizeof(dl_hp_ppa_req_t);
    ppa_req = (dl_hp_ppa_req_t *)buff;
    ppa_req->dl_primitive = DL_HP_PPA_REQ;

    if (putmsg(fd, &control, 0, 0) == -1)
    {  
        close(fd);
        return;
    }

    if (getmsg(fd, &control, 0, &flags) == -1)
    {  
        close(fd);
        return;
    }

    close(fd);

    ppa_ack = (dl_hp_ppa_ack_t *)buff;
    if (ppa_ack->dl_primitive != DL_HP_PPA_ACK)
        return;

    ppa_info = (dl_hp_ppa_info_t *)((char *)buff + 
                ppa_ack->dl_offset);

    for (i = 0; i < 6; i++)
        addr->eaddr[i] = ppa_info->dl_phys_addr[i];
#else
    int		fd;
    struct fis	iocnt_block;
    int		i;
    char	net_buf[sizeof(LAN_DEV0)+1];
    char	*p;

    (void)sprintf(net_buf, "%s", LAN_DEV0);
    p = net_buf + strlen(net_buf) - 1;

    /* 
     * Get 802.3 address from card by opening the driver and interrogating it.
     */
    for (i = 0; i < 10; i++, (*p)++) {
        if ((fd = open (net_buf, O_RDONLY)) != -1) {
	    iocnt_block.reqtype = LOCAL_ADDRESS;
	    ioctl (fd, NETSTAT, &iocnt_block);
	    close (fd);

            if (iocnt_block.vtype == 6)
                break;
        }
    }

    if (fd == -1 || iocnt_block.vtype != 6) {
        *st = utils_s_802_cant_read;
        return;
    }

    for (i = 0; i < 6; i++)
       addr->eaddr[i] = iocnt_block.value.s[i];
#endif
    *st = error_status_ok;
#else
    /* Never #define HPDCE_RPC_DLPI_SUPPORT */
    *st = utils_s_802_cant_read;
#endif
}

