/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: uuidsys.c,v $
 * Revision 1.1.53.2  1996/02/18  00:00:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:30  marty]
 *
 * Revision 1.1.53.1  1995/12/08  00:14:11  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/14  20:13 UTC  maunsell_c
 * 	merge Walnut Creek Port to HPDCE02 mainline
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/6  1995/08/22  17:48 UTC  maunsell_c
 * 	use krpc_xxxx calls
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:33 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 	[1995/12/07  23:55:47  root]
 * 
 * Revision 1.1.51.3  1994/06/10  20:54:15  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:01  devsrc]
 * 
 * Revision 1.1.51.2  1994/02/02  21:48:57  cbrooks
 * 	OT9855 code cleanup breaks KRPC
 * 	[1994/02/02  21:00:08  cbrooks]
 * 
 * Revision 1.1.51.1  1994/01/21  22:31:32  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:33:05  cbrooks]
 * 
 * Revision 1.1.5.2  1993/07/08  09:28:41  root
 * 	Initial King Kong branch
 * 	[1993/07/08  09:27:52  root]
 * 
 * Revision 1.1.3.3  1993/06/02  23:01:45  tmm
 * 	include <sys/socket.h>
 * 	[1993/06/01  17:55:16  tmm]
 * 
 * Revision 1.1.3.2  1993/03/29  13:19:45  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.2.2  1992/06/18  18:35:43  tmm
 * 	06/16/92  tmm  Created from COSL drop.
 * 	[1992/06/18  18:31:09  tmm]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**  NAME:
**
**      uuid_osc.c
**
**  FACILITY:
**
**      UUID
**
**  ABSTRACT:
**
**      UUID - OSF/1 Operating System Component (osc) dependent routines
**
*/

#include <commonp.h>            /* common definitions                   */
#include <dce/uuid.h>           /* uuid idl definitions (public)        */
#include <uuidp.h>              /* uuid idl definitions (private)       */

#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

#include <sys/kernel.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/user.h>       /* need the u area for a thread handle */
#include <sys/proc.h>       /* need for u.u_procp.c_cptr */

#include <dce/ker/krpc_osi_mach.h> /* need for proc/kthread iface macros */

/* device flags we require to be set */

#define IFF_FLAGS       (IFF_UP | IFF_RUNNING)

/*
 * U U I D _ _ G E T _ O S _ T I M E
 *
 * Get OS time - contains platform-specific code.
 *
 */

void uuid__get_os_time 
#ifdef _DCE_PROTO_
(
    uuid_time_t        *os_time
)
#else
(os_time)
uuid_time_t        *os_time;
#endif
{

    struct timeval      tp;

    MICROTIME(&tp);
    os_time->hi = tp.tv_sec;
    os_time->lo = tp.tv_usec*10;
}


/*
 * U U I D _ _ G E T _ O S _ P I D
 *
 * Get the process id
 */
unsigned32 uuid__get_os_pid (void)

{
    return ((unsigned32) krpc_osi_GetPid() );
}


/*
 * U U I D _ _ G E T _ O S _ A D D R E S S
 *
 * Get our ethernet hardware address - contains platform-specific code
 *
 */

void uuid__get_os_address 
#ifdef _DCE_PROTO_
(
    uuid_address_p_t        addr,
    unsigned32              *status
)
#else
(addr, status)
uuid_address_p_t        addr;
unsigned32              *status;
#endif
{
    struct ifnet *ifp;

    /*
     * Scan through the kernel's network interface configuration
     * database looking for any network interface that is UP and
     * isn't a LOOPBACK.
     * !!! probably needs some kind of lock...
     */
    for (ifp = ifnet; ifp; ifp = ifp->if_next)
    {
        if ((ifp->if_flags & IFF_FLAGS) != IFF_FLAGS || (ifp->if_flags & IFF_LOOPBACK))
            continue;
        bcopy(((struct arpcom *)ifp)->ac_enaddr, addr, sizeof (uuid_address_t));

        break;
    }

    *status = uuid_s_ok;
}
