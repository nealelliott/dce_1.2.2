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
 * Revision 1.1.6.2  1996/02/18  00:00:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:44  marty]
 *
 * Revision 1.1.6.1  1995/12/08  00:14:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:14  root]
 * 
 * Revision 1.1.4.3  1993/01/03  22:36:08  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:52:12  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  19:38:37  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:52:55  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/26  06:06:58  jim
 * 	Added include of sys/domain.h and sys/protosw.h for AIX 3.2
 * 	[1992/06/02  16:02:53  jim]
 * 
 * Revision 1.1  1992/01/19  03:15:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**  %a%copyright(,**  )
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
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**  22-apr-91 labossiere removed some extraneous includes
**  25-sep-90 nacey     Changed error_status_t -> unsigned32.
**  08-jun-90 nacey     Modeled after com/uuid_ultrix.c
**
**  %a%private_end
**
*/

#if defined(_AIX) && defined(_KERNEL)
#include <sys/domain.h>
#include <sys/protosw.h>
#endif

#include <commonp.h>            /* common definitions                   */
#include <dce/uuid.h>               /* uuid idl definitions (public)        */
#include <uuidp.h>              /* uuid idl definitions (private)       */

#include <net/if.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>

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

    int opri;

    opri = i_disable(INTTIMER);
    os_time->hi = tod.tv_sec;
    os_time->lo = tod.tv_nsec/100;
    i_enable(opri);

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

    CODING_ERROR(status);
    /*
     * Scan through the kernel's network interface configuration
     * database looking for any network interface that is UP and
     * isn't a LOOPBACK.
     */
    for (ifp = ifnet; ifp; ifp = ifp->if_next)
    {
        if ((ifp->if_flags & IFF_FLAGS) != IFF_FLAGS || (ifp->if_flags & IFF_LOOPBACK))
            continue;
        bcopy (ifp->if_addrlist->ifa_addr, addr, sizeof (uuid_address_t));
        *status = uuid_s_ok;
        return;
    }

}
