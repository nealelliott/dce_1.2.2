/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: kern_uniqaddr.c,v $
 * Revision 1.1.9.2  1996/02/17  23:59:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:18  marty]
 *
 * Revision 1.1.9.1  1995/12/08  00:13:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:27  root]
 * 
 * Revision 1.1.7.1  1994/01/21  22:31:00  cbrooks
 * 	Code Cleanup
 * 	[1994/01/21  20:19:13  cbrooks]
 * 
 * Revision 1.1.5.2  1993/06/24  20:09:27  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:38:10  hinman]
 * 
 * Revision 1.1.3.4  1993/01/03  22:34:48  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:50:03  bbelch]
 * 
 * Revision 1.1.3.3  1992/12/23  19:36:01  zeliff
 * 	Embedding copyright markers.
 * 	[1992/12/23  15:09:34  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:12:47  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:35:35  weisman]
 * 
 * $EndLog$
 */

#define _DCE_POSIX_H_

#include <commonp.h>
#include <dce/uuid.h>
#include <uuidp.h>

#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/route.h>
#include <netinet/in_var.h>
#include <netinet/ip_str.h>
#include <netinet/arp.h>

#include <sys/types.h>
#include <sys/param.h>
#include <sys/user.h>
#include <sys/systm.h>
#include <sys/errno.h>
#include <sys/vfs.h>
#include <sys/vnode.h>
#include <sys/file.h>
#include <sys/fstyp.h>
#include <sys/resource.h>
#include <sys/cred.h>
#include <sys/uio.h>
#include <sys/debug.h>

#define IEEE_802_ADDR "/etc/ieee_802_addr"
#define IEEE_SIZE (sizeof(uuid_address_t))

#ifdef SNI_SVR4
#define IFF_FLAGS	(IFF_UP)
#define utc_gettime        dcesys01
#define utc_adjtime        dcesys02
#define get_unique_addr(a,b)	dcesys03(a,b)
#else
#define IFF_FLAGS	(IFF_UP | IFF_RUNNING)
#endif

extern nclentry;

struct addrstat {
	uuid_address_p_t addr;
	unsigned32 *stat;
};

int
get_unique_addr (uap, rvp)
	struct addrstat *uap;
	rval_t *rvp;
{

    static char ieeebuf[IEEE_SIZE];
    static int  ieee_valid = 0;

    int error = 0;
    uuid_address_t eaddr;
    unsigned32 status; 

    uuid__get_os_address(&eaddr, &status);
    if (status == uuid_s_ok) {
	if (copyout((caddr_t)&eaddr, (caddr_t)uap->addr, IEEE_SIZE))
	    return EFAULT;
    }
    else {
	status = uuid_s_ok;
	if (ieee_valid == 0) {
	    if (error = read_ieee802(ieeebuf))
		status = uuid_s_no_address;
	    else 
		ieee_valid = 1;
	}
	if (copyout((caddr_t)ieeebuf, (caddr_t)uap->addr, IEEE_SIZE))
	   return EFAULT;
    }
    if (copyout((caddr_t)&status, (caddr_t)uap->stat, sizeof (status)))
            return EFAULT;
    return (error);
}

static int
read_ieee802(bufptr)
    char *bufptr;
{
    int i, val;
    int error = 0;
    struct vnode *vp;
    char ieeechars[IEEE_SIZE*2];

    if (error = vn_open(IEEE_802_ADDR, UIO_SYSSPACE, FREAD, 0, &vp, 0))	
	return (error);
	
    if (error = vn_rdwr(UIO_READ, vp, (caddr_t)ieeechars, sizeof(ieeechars), 0,
		UIO_SYSSPACE,  0, RLIM_INFINITY, u.u_cred, (int *)NULL))
	goto close;

    for (i = 0; i < IEEE_SIZE; i++) bufptr[i] = 0;
    for (i = 0; i < (IEEE_SIZE * 2); i++) {
	if (ieeechars[i] < '0' || ieeechars[i] > '9')  {
	    switch (ieeechars[i]) {
		case 'A': case 'B': case 'C': 
		case 'D': case 'E': case 'F':
		    val = ieeechars[i] - 'A' + 0xa;
		    break;
		case 'a': case 'b': case 'c': 
		case 'd': case 'e': case 'f':
		    val = ieeechars[i] - 'a' + 0xa;
		    break;
		default:
	    	    error = EBADF;
		    goto close;
	    }
	}
	else 
	    val = ieeechars[i] - '0';
	bufptr[i/2] += (i%2 ? val : val*16);
    }
close:
    VOP_CLOSE(vp, FREAD, 1, 0, u.u_cred); 
    VN_RELE(vp);
    return (error);
}

#ifndef KRPC_INCLUDED

PRIVATE void uuid__get_os_address (addr, status)
uuid_address_p_t	addr;
unsigned32		*status;
{
    int s;
    struct ip_provider *prov;
    struct app_pcb *app;
    queue_t *q;
    extern struct ip_provider provider[];
    extern struct ip_provider *lastprov;

    s = splstr();

    for (prov = provider; prov <= lastprov; prov++) {
	if (prov->qbot == NULL ||
	    ((prov->if_flags & IFF_FLAGS) != IFF_FLAGS) ||
	    (prov->if_flags & IFF_LOOPBACK))
	    continue;
	
	q = prov->qbot;
	ASSERT(q);
	q = q->q_next;
	ASSERT(q);
	app = ((struct app_pcb *)q->q_ptr);
	ASSERT(app);

	/* b_c_o_p_y((caddr_t)app->app_ac.ac_enaddr, (caddr_t)addr,
	    sizeof (uuid_address_t)); */

        memmove( (caddr_t)addr, (caddr_t)app->app_ac.ac_enaddr, 
		 sizeof(uuid_address_t));

	splx(s);

	*status = uuid_s_ok;
	return;
    }
    splx(s);

    *status = -1; 
}
#endif

/* The init routine for dce1 driver sets up these entries in the sysent
 * We will use this till clocal becomes available.
 */
#define UTC_GETTIME 70
#define UTC_ADJTIME 71
#define GET_UNIQUE_ADDR 73

extern struct sysent sysent[];
extern int utc_gettime(), utc_adjtime();

dce1init()
{
	if (nclentry > 4)
		return;
	sysent[UTC_GETTIME].sy_narg = 1;
	sysent[UTC_GETTIME].sy_flags = 0;
	sysent[UTC_GETTIME].sy_call = dcesys01;

	sysent[UTC_ADJTIME].sy_narg = 2;
	sysent[UTC_ADJTIME].sy_flags = 0;
	sysent[UTC_ADJTIME].sy_call = dcesys02;

	sysent[GET_UNIQUE_ADDR].sy_narg = 2;
	sysent[GET_UNIQUE_ADDR].sy_flags = 0;
	sysent[GET_UNIQUE_ADDR].sy_call = dcesys03;

}
