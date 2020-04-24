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
 * Revision 1.1.8.2  1996/02/18  19:23:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:51  marty]
 *
 * Revision 1.1.8.1  1995/12/08  00:10:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:19  root]
 * 
 * Revision 1.1.6.3  1994/08/03  18:58:57  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:05:56  mccann]
 * 
 * Revision 1.1.6.2  1994/05/12  21:10:25  peckham
 * 	Move apollo conditional from casys.c to here.
 * 	Add CA_mappoint().
 * 	[1994/05/12  19:12:18  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:55:09  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:45:44  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:35:38  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:18:19  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/11  22:14:32  grober
 * 	Initial Revision
 * 	[1992/06/11  22:14:05  grober]
 * 
 * $EndLog$
 */
/*
 * Module: unix_casysinfo.c
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
 */
#include <stdio.h>

#include <unistd.h>
#include <nlist.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/param.h>
#if defined(__osf__) || defined(__OSF__)
/*
 * Temporary hack to workaround bug where __KERNAL must be defined to
 * pick this up out of shm.h. QARed on 17 July.
 */
struct  shminfo {
        int     shmmax,         /* max shared memory segment size */
                shmmin,         /* min shared memory segment size */
                shmmni,         /* number of shared memory identifiers */
                shmseg;         /* max attached shared memory segments per process */
};
#endif

#include <pthread.h>
#include <dce/dns_record.h>

#ifdef __hpux
#    ifdef __hp9000s300
#        define SYM_PHYSMEM     "_physmem"
#        define SYM_SMINFO      "_shminfo"
#    else
#        define SYM_PHYSMEM     "physmem"
#        define SYM_SMINFO      "shminfo"
#    endif /* __hp9000s300 */
#else /* __hpux */
#ifndef mips
#define SYM_PHYSMEM     "_physmem"
#define SYM_SMINFO      "_sminfo"
#else
#define SYM_PHYSMEM     "physmem"
#ifdef __osf__
#define SYM_SMINFO      "shminfo"
#else
#define SYM_SMINFO      "sminfo"
#endif
#endif
#endif

#if !defined(apollo)

int nlist(char *,struct nlist nl[]);

static void
getval (
    int fd,
    long pos,
    char *dest,
    int len)
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

#endif /* !defined(apollo) */

/*
 * System-dependent function to return various configuration info.
 * Returns 0 if it failed to find those values, non-zero otherwise.
 */
int
CAsysinfo (
 int *memsize,    /* OUT: size of physical memory in bytes */
 int *sh_min,     /* OUT: minimum shared memory segment size */
 int *sh_max      /* OUT: maximum shared memory segment size */
)
{
#if !defined(apollo)

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
   i = nlist ("/hp-ux", nl);
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

#endif /* !defined(apollo) */

   return (0);
}

char *
CA_mappoint (void)
{
    return(0);
}
