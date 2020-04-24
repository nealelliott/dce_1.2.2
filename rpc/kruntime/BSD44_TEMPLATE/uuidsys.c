/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: uuidsys.c,v $
 * Revision 1.1.8.2  1996/02/17  23:59:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:22  marty]
 *
 * Revision 1.1.8.1  1995/12/08  00:13:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:36  root]
 * 
 * Revision 1.1.6.1  1994/01/21  22:31:14  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:38:26  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  22:35:04  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:50:31  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  19:36:45  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:47:32  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  17:55:01  rsalz
 * 	"Changed as part of rpc6 drop."
 * 	[1992/05/01  17:50:05  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:15:46  devrcs
 * 	Initial revision
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

#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

#include <sys/kernel.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/user.h>       /* need the u area for a thread handle */
#include <sys/proc.h>       /* need for u.u_procp.c_cptr */

/* device flags we require to be set */
#define IFF_FLAGS       (IFF_UP | IFF_RUNNING)

/*
 * U U I D _ _ G E T _ O S _ T I M E
 *
 * Get OS time - contains platform-specific code.
 *
 */
PRIVATE void uuid__get_os_time (os_time)
uuid_time_t        *os_time;
{

    struct timeval      tp;

    microtime(&tp);
    os_time->hi = tp.tv_sec;
    os_time->lo = tp.tv_usec*10;
}


/*
 * U U I D _ _ G E T _ O S _ P I D
 *
 * Get the process id
 */
PRIVATE unsigned32 uuid__get_os_pid ()
{
    return ((unsigned32) u.u_procp->p_pid );
}


/*
 * U U I D _ _ G E T _ O S _ A D D R E S S
 *
 * Get our ethernet hardware address - contains platform-specific code
 *
 */
PRIVATE void uuid__get_os_address (addr, status)
uuid_address_p_t        addr;
unsigned32              *status;
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
        /*b_copy (((struct arpcom *)ifp)->ac_enaddr, addr, sizeof (uuid_address_t));*/
        memmove(( addr, (struct arpcom *)ifp)->ac_enaddr, sizeof (uuid_address_t));*/
        break;
    }

    *status = uuid_s_ok;
}
