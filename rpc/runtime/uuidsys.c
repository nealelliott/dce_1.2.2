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
 * Revision 1.1.4.2  1996/02/18  00:05:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:22:31  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:35 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/18  14:00 UTC  tatsu_s
 * 	Merged from HP800/uuidsys.c.
 * 	  Standalone uuid generation.
 * 	[1995/12/08  00:00:43  root]
 * 
 * Revision 1.1.2.3  1994/06/29  20:55:33  ohara
 * 	gettimeofday only take one arg on SVR4
 * 	[1994/06/27  19:55:10  ohara]
 * 
 * Revision 1.1.2.2  1994/06/24  20:29:18  tom
 * 	Bug 11076:
 * 	 Put uuid__get_os_address back, but as a wrapper for dce_get_802_addr.
 * 	 KRPC has their own version of uuidsys.c (and uuid__get_os_address()).
 * 	[1994/06/24  20:28:53  tom]
 * 
 * Revision 1.1.2.1  1994/06/23  17:40:26  tom
 * 	Initial checkin.
 * 	Used to contain uuid__get_os_address, but now we use
 * 	libdce supplied dce_get_802_addr function, which is machine
 * 	sepcific.  Now, this file is the same for all reference platforms.
 * 	[1994/06/22  20:40:42  tom]
 * 
 * $EndLog$
 */
/*
**  NAME:
**
**      uuidsys.c
**
**  FACILITY:
**
**      UUID
**
**  ABSTRACT:
**
**      UUID - Unix dependant (therefore system dependant) routines
**
**
*/

#include <dce/dce.h>
#include <commonp.h>            /* common definitions                   */
#include <dce/uuid.h>           /* uuid idl definitions (public)        */
#include <uuidp.h>              /* uuid idl definitions (private)       */
#include <dce/dce_utils.h>      /* defines  dce_get_802_addr()          */


/*
 *  Define constant designation difference in Unix and DTSS base times:
 *  DTSS UTC base time is October 15, 1582.
 *  Unix base time is January 1, 1970.
 */
#define uuid_c_os_base_time_diff_lo     0x13814000
#define uuid_c_os_base_time_diff_hi     0x01B21DD2

/*
 * U U I D _ _ G E T _ O S _ T I M E
 *
 * Get OS time - contains platform-specific code.
 */
PRIVATE void uuid__get_os_time (uuid_time_t * uuid_time)
{

    struct timeval      tp;
    unsigned64_t        utc,
                        usecs,
                        os_basetime_diff;

    /*
     * Get current time
     */
#ifdef SNI_SVR4
    if (gettimeofday (&tp ))
#else
    if (gettimeofday (&tp, (struct timezone *) 0))
#endif
    {
        perror ("uuid__get_os_time");
        exit (-1);
    }

    /*
     * Multiply the number of seconds by the number clunks 
     */
    uuid__uemul ((long) tp.tv_sec, UUID_C_100NS_PER_SEC, &utc);

    /*
     * Multiply the number of microseconds by the number clunks 
     * and add to the seconds
     */
    uuid__uemul ((long) tp.tv_usec, UUID_C_100NS_PER_USEC, &usecs);
    UADD_UVLW_2_UVLW (&usecs, &utc, &utc);

    /*
     * Offset between DTSS formatted times and Unix formatted times.
     */
    os_basetime_diff.lo = uuid_c_os_base_time_diff_lo;
    os_basetime_diff.hi = uuid_c_os_base_time_diff_hi;
    UADD_UVLW_2_UVLW (&utc, &os_basetime_diff, uuid_time);

}


/* 
 * U U I D _ _ G E T _ O S _ P I D
 *
 * Get the process id
 */
PRIVATE unsigned32 uuid__get_os_pid ( void )
{
    return ((unsigned32) getpid());
}


/*
 * U U I D _ _ G E T _ O S _ A D D R E S S
 *
 * Wrapper for dce_get_802_addr()
 * 
 * Kruntime has kernel specific version of this.
 */
PRIVATE void uuid__get_os_address
(
    uuid_address_p_t        addr,
    unsigned32              *status
)
{
    /*
     * Cheat and cast the uuid_address_p_t to dce_802_addr_t
     * since they are the same
     */

    dce_get_802_addr((dce_802_addr_t *)addr, status);

    if (*status == error_status_ok)
	*status = uuid_s_ok;
}


