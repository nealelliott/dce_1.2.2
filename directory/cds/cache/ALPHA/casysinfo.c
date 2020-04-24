/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: casysinfo.c,v $
 * Revision 1.1.4.2  1996/02/18  19:23:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:49  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:10:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:17  root]
 * 
 * Revision 1.1.2.4  1994/08/03  18:58:54  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:05:01  mccann]
 * 
 * Revision 1.1.2.3  1994/06/09  16:06:55  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:28  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/12  21:10:23  peckham
 * 	Add CA_mappoint().
 * 	[1994/05/12  19:12:13  peckham]
 * 
 * Revision 1.1.2.1  1994/03/12  21:55:08  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  20:01:31  peckham]
 * 
 * $EndLog$
 */
/*
 * Module: casysinfo.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
 * MODIFICATION HISTORY:
 *
 * Gen 2, 8-Dec-1992, Alan Peckham
 *	Remove temporary osf workarounds.
 */

#include <pthread.h>
#include <stdio.h>

#include <unistd.h>
#include <nlist.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/param.h>

#include <dce/dns_record.h>

# define SYM_PHYSMEM	"_physmem"
# define SYM_SMINFO	"_shminfo"

static void 
getval (int   fd,
        long  pos,
        char  *dest,
        int   len)
{
    ZERO_bytes(dest, len);
    /*
     * Seek to the value of the symbol and read its current value
     */
    if (pos == 0)
	return;
    if (lseek (fd, pos, 0) == -1) {
	perror ("lseek");
	return;
    }
    if (read (fd, dest, len) < len)
	perror ("read");

    return;
}

/*
 * System-dependent function to return various configuration info.
 * Returns 0 if it failed to find those values, non-zero otherwise.
 */
int
CAsysinfo (int  *memsize,
           int  *sh_min,
           int  *sh_max)
{
    static struct nlist nl[] = {
	{ SYM_PHYSMEM },
	{ SYM_SMINFO },
	0,
    };
    int i, fd;
    long npages = -1;
    struct shminfo sminfo;

  /*
   * Read symbol table of system executable image to get offset
   * of SYM_PHYSMEM in core file
   * nlist(3) returns -1 if it can't read the image or the number
   * of unfound symbols
   */
   i = nlist ("/vmunix", nl);
   if (i < 0) {
     perror ("nlist");
     return (0);
   }
   else if (i != 0) {
     for (i = 0; i < sizeof(nl) / sizeof(struct nlist) - 1; i++)
	 if (nl[i].n_value == 0) {
	   fprintf(stderr,
		   "Cannot get address for \"%s\"\n", nl[i].n_name);
	   return (0);
	 }
   }
   else {
     fd = open ("/dev/kmem", 0, 0);
     if (fd < 0) {
       perror ("open(/dev/kmem)");
       return (0);
     }
   }
   
   
   if (i == 0 && fd >= 0) {
    /*
     * Fetch physmem value from kernal
     */
     getval (fd, (long)nl[0].n_value, (char *) &npages, sizeof (npages));
    /*
     * Physical memory size is in units of pages, but page size varies
     * with hardware so convert result to bytes
     */
     *memsize = npages * NBPG;

    /*
     * Now fetch sminfo structure from kernal
     */
     getval (fd, (long)nl[1].n_value, (char *) &sminfo, sizeof (sminfo));
     *sh_max = sminfo.shmmax;
     *sh_min = sminfo.shmmin;
     close (fd);					
     return (1);
   }
   return (0);
}

char *
CA_mappoint (void)
{
    return(0);
}
