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
 * Revision 1.1.8.2  1996/02/18  19:23:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:52  marty]
 *
 * Revision 1.1.8.1  1995/12/08  00:10:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:22  root]
 * 
 * Revision 1.1.6.3  1994/08/03  18:59:00  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:06:50  mccann]
 * 
 * Revision 1.1.6.2  1994/05/12  21:10:34  peckham
 * 	Add CA_mappoint().
 * 	[1994/05/12  19:12:37  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:55:18  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:46:11  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:35:57  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:18:42  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/14  20:49:37  mfox
 * 	Fixes to allow CDS to compile cleanly with RIOS ANSI C compiler
 * 	[1992/04/14  19:38:30  mfox]
 * 
 * Revision 1.1  1992/01/19  15:23:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: casysinfo.c
 * Version: X02.28
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990. ALL RIGHTS RESERVED.
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
#include <stdio.h>
#include <cache.h>
#include <limits.h>
#include <sys/seg.h>
#include <sys/cfgodm.h>

#include <memory.h>
#include <nlist.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/param.h>

#include <pthread.h>
#include <errno.h>

/*
 * Determine initial cache size in bytes based on system envirnment
 */
int
CAsysinfo (
 memsize,    /* OUT: size of physical memory in bytes */
 sh_min,     /* OUT: minimum shared memory segment size */
 sh_max      /* OUT: maximum shared memory segment size */
)
    int *memsize, *sh_min, *sh_max;
{
  int i, fd, nbytes;
  struct shminfo sminfo;
  struct CuAt *classp, retp;
  char *criteria = "name = 'sys0' AND attribute = 'realmem'";


  /* 
   * Initialize for a call to the ODM database
   */

  odm_initialize();

  classp = (struct CuAt *)odm_get_obj(CuAt_CLASS,criteria,&retp,ODM_FIRST);
  if(classp != (struct CuAt *) -1){

       nbytes = atoi(classp->value);
       /*
        * Physical memory size is in units of 1K, so convert result to bytes
        */
       nbytes = (nbytes * 1024);

       /*
	* Since shminfo is not exported by the kernel and results
	* of reading /dev/kmem are unpredictable for unexported 
	* symbols I am limiting nbytes to be not greater
	* than SEGSIZE (largest mapped file).  This should not
	* ever happen.
	*/
       *sh_min = 4096;
       *sh_max = SEGSIZE;
       *memsize = nbytes;
       return(TRUE);
     }
  return(FALSE);
}

char *
CA_mappoint (void)
{
    return(0);
}
