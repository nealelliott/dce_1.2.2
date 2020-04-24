/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: krpch_conf.c,v $
 * Revision 1.1.18.2  1996/02/18  00:00:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:35  marty]
 *
 * Revision 1.1.18.1  1995/12/08  00:14:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:57  root]
 * 
 * Revision 1.1.16.2  1994/06/10  20:54:16  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:02  devsrc]
 * 
 * Revision 1.1.16.1  1994/01/21  22:31:46  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:41:23  cbrooks]
 * 
 * Revision 1.1.14.2  1993/09/14  20:24:28  delgado
 * 	Fix compliation problem with krpch_cdeventry declaration.
 * 	[1993/09/14  20:20:02  delgado]
 * 
 * Revision 1.1.14.1  1993/09/01  14:38:17  delgado
 * 	DFS 486 port - add routines to support the new device configuration
 * 	semantics in OSF1.2
 * 	[1993/09/01  14:37:42  delgado]
 * 
 * $EndLog$
 */


#include <sys/types.h>
#include <sys/errno.h>
#include <sys/param.h>
#include <sys/conf.h>
#include <sys/sysconfig.h>


extern int krpchdd_open(), krpchdd_close(), krpchdd_read(), krpchdd_write(), krpchdd_ioctl(), krpchdd_select();

static struct cdevsw krpch_cdeventry = {
        krpchdd_open,   krpchdd_close,  krpchdd_read,   krpchdd_write,  
        krpchdd_ioctl,  nodev,          nodev,          0,
        krpchdd_select, nodev,           DEV_FUNNEL_NULL 
};

extern dev_t krpch_cdev;

int
krpch_configure(
	 sysconfig_op_t	op,
	 caddr_t	indata,
	 size_t		indatalen,
	 caddr_t	outdata,
	 size_t		outdatalen

	     )
{
	int		i, configured, error = 0;
	dev_t		dev;
	struct subsystem_info	info;

	strcpy(info.subsystem_name, "krpch");
	configured = ((!subsys_reg(SUBSYS_GET_INFO, &info)) &&
		      info.config_flag);
	if ((configured && op == SYSCONFIG_CONFIGURE) ||
	    (!configured &&  op != SYSCONFIG_CONFIGURE))
		return (EALREADY);

	switch (op) {
	case SYSCONFIG_CONFIGURE:
		if ((dev = cdevsw_add(krpch_cdev, &krpch_cdeventry)) == NODEV) {
			printf("krpch: found cdev 0x%x in use\n", krpch_cdev);
			error = ENODEV;
			break;
		}
		break;

	case SYSCONFIG_RECONFIGURE:
	case SYSCONFIG_QUERYSIZE:
	case SYSCONFIG_QUERY:
	case SYSCONFIG_UNCONFIGURE:
		error = EINVAL;
		break;
	default:
		error = EINVAL;
		break;
	}
	return (error);
}
