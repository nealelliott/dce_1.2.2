/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: krpc_osi_mach.h,v $
 * Revision 1.1.2.3  1996/02/18  23:46:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:08  marty]
 *
 * Revision 1.1.2.2  1996/02/07  17:48:10  parul
 * 	DCE 1.2.1 final drop from HP
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:52 UTC  psn
 * 	walnut creek merge new file krpc_osi_mach.h to HPDCE02 mainline
 * 	[1995/09/14  19:14 UTC  maunsell_c  /main/HPDCE02/1]
 * 
 * 	use p param for kt_wchan macro
 * 	[1995/09/12  13:04 UTC  maunsell_c  /main/HPDCE02/maunsell_WC_port_fixes/4]
 * 
 * 	fix krpc_osi_set_cdir missing )
 * 	[1995/08/22  18:26 UTC  maunsell_c  /main/HPDCE02/maunsell_WC_port_fixes/3]
 * 
 * 	fix GetUID .
 * 	[1995/08/22  17:48 UTC  maunsell_c  /main/HPDCE02/maunsell_WC_port_fixes/2]
 * 
 * 	add to build
 * 	[1995/08/22  15:59 UTC  maunsell_c  /main/HPDCE02/maunsell_WC_port_fixes/1]
 * 
 * $EndLog$
 */

/*
 * Platform dependent header file for krpc module
 */

#ifndef _KRPC_OSI_MACH_H
#define _KRPC_OSI_MACH_H

#if __hpux1010p && defined(_KERNEL)
#include <sys/proc_iface.h>
#include <sys/kthread_iface.h>
#else
#include <sys/proc.h>
#endif

#define krpc_osi_curproc()           (u.u_procp)
#define krpc_osi_GetUID(c)           (c->cr_uid)

#if __hpux1010p && defined(_KERNEL)

#define krpc_osi_GetPid()            (p_pid(krpc_osi_curproc()))
#define krpc_osi_getucred()          kt_cred(u.u_kthreadp)
#define krpc_osi_set_ttyp(c)         (set_p_ttyp(u.u_procp, c))
#define krpc_osi_set_ttyd(c)         (set_p_ttyd(u.u_procp, c))
#define krpc_osi_get_highestfd()     (p_highestfd(u.u_procp))
#define krpc_osi_set_highestfd(c)    (set_p_highestfd(u.u_procp, c))

#define krpc_osi_get_sel()           (kt_sel(u.u_kthreadp))
#define krpc_osi_set_sel()           (set_kt_sel(u.u_kthreadp))
#define krpc_osi_clear_sel()         (clear_kt_sel(u.u_kthreadp))

#define krpc_osi_get_rdir()          (p_rdir(u.u_procp))
#define krpc_osi_set_rdir(c)         (set_p_rdir(u.u_procp, c))

#define krpc_osi_get_cdir()          (p_cdir(u.u_procp))
#define krpc_osi_set_cdir(c)         (set_p_cdir(u.u_procp, c))

#define krpc_osi_get_wchan(p)        (kt_wchan(p))

#else

#define krpc_osi_GetPid()            (krpc_osi_curproc()->p_pid)
#define krpc_osi_getucred()          u.u_cred
#define krpc_osi_set_ttyp(c)         (u.u_procp->p_ttyp = c)
#define krpc_osi_set_ttyd(c)         (u.u_procp->p_ttyd = c)

#define krpc_osi_get_highestfd()     (u.u_highestfd)
#define krpc_osi_set_highestfd(c)    (u.u_highestfd = c)

#define krpc_osi_get_sel()           (u.u_procp->p_flag & SSEL)
#define krpc_osi_set_sel()           (u.u_procp->p_flag |= SSEL)
#define krpc_osi_clear_sel()         (u.u_procp->p_flag &= ~SSEL)

#define krpc_osi_get_rdir()          (u.u_rdir)
#define krpc_osi_set_rdir(c)         (u.u_rdir = c)

#define krpc_osi_get_cdir()          (u.u_cdir)
#define krpc_osi_set_cdir(c)         (u.u_cdir = c)

#define krpc_osi_get_wchan(p)        (p->p_wchan)

#endif

#endif /* _KRPC_OSI_MACH_H*/
