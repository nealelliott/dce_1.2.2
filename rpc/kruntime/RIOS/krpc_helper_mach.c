/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krpc_helper_mach.c,v $
 * Revision 1.1.4.2  1996/02/18  00:00:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:14:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:09  root]
 * 
 * Revision 1.1.2.4  1993/01/03  22:35:55  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:51:50  bbelch]
 * 
 * Revision 1.1.2.3  1992/12/23  19:38:22  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:52:33  zeliff]
 * 
 * Revision 1.1.2.2  1992/12/03  22:55:39  delgado
 * 	Changes for new krpc helper interface
 * 	[1992/12/03  22:55:05  delgado]
 * 
 * $EndLog$
 */

#include <commonp.h>
#include <com.h>

#include <krpc_helper.h>
#include <krpc_helper_data.h>
#include <sys/file.h>

#include <sys/device.h>
#include <sys/poll.h>
#include <sys/errno.h>

extern struct krpc_helper *pending;

krpchdd_open(dev, devflag, chan, ext)
dev_t dev;
ulong devflag;
chan_t chan;
int ext;
{
    return(krpch_open());
}


/* 
 * close the device - free all swap space held by this client.
 */
krpchdd_close(dev, chan)
dev_t dev;
chan_t;
{
    krpch_close();
    return (0);
}


krpchdd_ioctl(dev, cmd, data, flag, chan, ext)
dev_t   dev;
int     cmd;
caddr_t data;
int	flag;
int     chan, ext;
{
    return (krpch_ioctl(cmd, data));
}


krpchdd_read(dev, uio, chan, ext)
dev_t dev;
struct uio *uio;
chan_t chan;
int ext;
{
    return(krpch_read(uio));
}


krpchdd_write(dev, uio, chan, ext)
dev_t dev;
struct uio *uio;
chan_t chan;
int ext;
{
    return(krpch_write(uio));
}

int krpchdd_select(dev, events, reventsp, chan)
    dev_t dev;
    ushort events, *reventsp;
    int chan;
{
    if ((events & POLLIN) && !QUEUE_EMPTY(pending))
	*reventsp |= POLLIN;
    else
	*reventsp &= ~POLLIN;

    if (events & POLLOUT)
	*reventsp |= POLLOUT;

    return(0);
}


/*
 * Configure the pseudo device driver in the AIX environment.
 * It supports the CFG_INIT and CFG_TERM commands to initialize the pseudo
 * device in the device switch table and deleting it from the same table
 * when the device is unconfigured.
 */
dev_t krpchdd_dev;

krpchdd_config(devno, cmd)
dev_t devno;
int   cmd;
{
	struct devsw dsw;
	int rc = 0;
	extern nodev();

	switch (cmd) {
	case CFG_INIT:
		bzero (&dsw, sizeof (struct devsw));

		dsw.d_open = (int (*)()) krpchdd_open;
		dsw.d_close = (int (*)()) krpchdd_close;
		dsw.d_read = (int (*)()) krpchdd_read;
		dsw.d_write = (int (*)()) krpchdd_write;
		dsw.d_ioctl = (int (*)()) krpchdd_ioctl;
		dsw.d_select = (int (*)()) krpchdd_select;
		dsw.d_dump = nodev;
		dsw.d_print = nodev;
		dsw.d_mpx = nodev;
		dsw.d_revoke = nodev;
		dsw.d_strategy = nodev;

		if (devswadd(devno,&dsw) == -1)
			return(ENXIO);

		krpchdd_dev = devno;
		break;

	case CFG_TERM:
		(void) devswdel(devno);
		break;

	default:
		rc = EINVAL;
		break;
	}
	return (rc);
}
