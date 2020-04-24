/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: casysinfo.c,v $
 * Revision 1.1.4.2  1996/02/18  19:23:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:10:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:18  root]
 * 
 * Revision 1.1.2.1  1994/08/03  18:58:55  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:05:28  mccann]
 * 
 * $EndLog$
 */
/*
 * Module: unix_casysinfo.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED. 
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */
 
#include <sys/types.h>
#include <sys/param.h>
#include <sys/table.h>
#include <mach.h>
 

/*
 * System-dependent function to return various configuration info.
 * Returns 0 if it failed to find those values, non-zero otherwise.
 */
int
CAsysinfo (
 memsize,    /* OUT: size of physical memory in bytes */
 sh_min,     /* OUT: minimum shared memory segment size */
 sh_max      /* OUT: maximum shared memory segment size */
)
    int *memsize, *sh_min, *sh_max;
{
    struct host_basic_info hi;
    int size;
 
    /*
     * Fetch physmem value from kernal
     */
     size = sizeof(hi) / sizeof(int);
     host_info(host_self(), HOST_BASIC_INFO, &hi, &size);
    /*
     * Physical memory size is in units of pages, but page size varies
     * with hardware so convert result to bytes
     */
     *memsize = hi.memory_size;
 
    /*
     * Now fetch sminfo structure from kernal
     */
     table(TBL_SHMINFO, SHMINFO_MAX, sh_max, 1, sizeof(int));
     table(TBL_SHMINFO, SHMINFO_MIN, sh_min, 1, sizeof(int));

     return (1);
}

char *
CA_mappoint (void)
{
    return(0);
}
