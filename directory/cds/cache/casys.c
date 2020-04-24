/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: casys.c,v $
 * Revision 1.1.12.2  1996/02/18  19:24:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:21  marty]
 *
 * Revision 1.1.12.1  1995/12/08  00:10:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:43  root]
 * 
 * Revision 1.1.9.7  1994/08/11  20:35:12  peckham
 * 	Call cached server file routines.
 * 	[1994/08/11  04:48:01  peckham]
 * 
 * Revision 1.1.9.6  1994/06/23  18:28:34  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  18:05:48  mccann]
 * 
 * Revision 1.1.9.5  1994/06/09  16:07:17  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:52  devsrc]
 * 
 * Revision 1.1.9.4  1994/05/12  21:10:40  peckham
 * 	Remove direct NBPG reference.
 * 	Move apollo conditional to HP subdirectory.
 * 	Use CA_mappoint() to determine cache mapping address.
 * 	The vax mappoint has been moved to PMAX temporarily.
 * 	[1994/05/12  19:12:57  peckham]
 * 
 * Revision 1.1.9.3  1994/04/29  15:49:36  peckham
 * 	Use dcelocal_path in file names.
 * 	Eliminate use of dthread_delay() by creating CA_delay()
 * 	which uses pthread_delay_np().
 * 	[1994/04/29  13:56:01  peckham]
 * 
 * Revision 1.1.9.2  1994/04/14  14:38:08  peckham
 * 	Remove VMS conditionals.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  18:04:53  peckham]
 * 
 * Revision 1.1.9.1  1994/03/12  21:56:01  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:48:49  peckham]
 * 
 * Revision 1.1.7.4  1992/12/30  12:37:31  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:20:56  zeliff]
 * 
 * Revision 1.1.7.3  1992/12/17  13:54:17  peckham
 * 	    Gen 60  Alan Peckham            25-Nov-1992
 * 		OT#5995: Null shmid file not handled properly.
 * 		Change to blow the file away if the shmid is empty
 * 		or has invalid data.
 * 		Also took this opportunity to merge DEC code (see below).
 * 	[1992/12/17  13:53:44  peckham]
 * 
 * Revision 1.1.7.2  1992/09/29  18:28:37  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:09:23  weisman]
 * 
 * Revision 1.1.2.4  1992/06/26  06:33:12  jim
 * 	Added include of <msgblk.h>.
 * 	[1992/06/26  05:00:34  jim]
 * 
 * Revision 1.1.2.3  1992/05/12  13:04:04  keegan
 * 	Fix advertiser to deal with  old state left in cdscache.shmid file.
 * 	[1992/05/11  19:24:58  keegan]
 * 
 * Revision 1.1.2.2  1992/03/22  21:15:51  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:06:17  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: casys.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
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
#include <cache.h>
#include <dns_config.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <dns_cfg.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <dns_strings.h>

CA_PRIVATE int ca_file_sequence = 0;
bytes_u CacheName[] = CDS_CACHE_NAME;

/*
 * Local prototypes
 */
CA_PRIVATE int
CA_SysSize (
    int);

CA_PRIVATE int
CA_CheckShmem (
    int		 );

CA_PRIVATE int
CA_ReadCurrentVersion (void);

CA_PRIVATE void
CA_delay (void);

/*
 * Check the version number and name of the cache we are going to map
 */
int 
CheckCacheVersion (struct CacheFixed *cache_p)
{
  if ((EXT8(cache_p->ca_version.vr_major) != MAJOR) ||   
      (EXT8(cache_p->ca_version.vr_minor) != MINOR) ||
      NE_bytes(cache_p->ca_id, CacheName, CacheNameSize)) 
    return(0);
  
  return(1);
}

  
/*
 * Determine initial cache size in bytes.
 * FOR U*X:
 * If a size was specified (size != -1), use it, but clamp it between
 * the minimum and maximum shared memory sized configured in the system.
 * If a size wasn't spec'd, calculate one based on .5% of physical memory.
 */
CA_PRIVATE int 
CA_SysSize (int size)
{
  int sh_min = 160*1024;
  int sh_max = 160*1024;
  int nbytes = 160*1024;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " >CA_SysSize(%d)",
      size));

  /* Get system info: amount of physical memory, minimum and
   * maximum shared memory sizes.
   */
  if (!CAsysinfo (&nbytes, &sh_min, &sh_max)) {
      nbytes = 160*1024;  /* Return a default value and hope it works */
      sh_min = sh_max = nbytes;
  } else if (size != -1) {
      nbytes = size;  /* use specified size */
  } else {
      nbytes /= 200;  /* calculate .5% of memory size */

      /* at least 1/2 meg */
      if (nbytes < (500*1024)) 
	nbytes=(500*1024);
      }

/* Clamp size to within configured boundaries.
*/
  if (nbytes > sh_max)
      nbytes = sh_max;
  if (nbytes < sh_min)
      nbytes = sh_min;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " <CA_SysSize(%d) return(%d)",
      size,
      nbytes));

  return (nbytes);
}

/*
 * Detach shared memory, zero CAfixed_p
 */
void 
CASysUnmap (void)
{
  /* Make sure pointer is valid before proceeding */
  if (CAfixed_p != (struct CacheFixed *)0) {
    (void)shmdt((caddr_t)CAfixed_p);	/* Detach region */

    CAfixed_p = NULL;
  }
}

/*
 * Detach and delete shared memory region, zero CAfixed_p
 */
void 
CASysDelete (int id)
{
  /* Dump cache */
  CASysWrite();
  
  /* Detach shared memory */
  CASysUnmap();		
  
  /* Delete shared memory */
  if (0 <= shmctl(id, IPC_RMID, 0)) {
    char file[_DNS_MAXPATHLEN+1];

    (void)sprintf(file, CDS_ETC_DIR, dcelocal_path, DNS_SHMID_FILE);
    (void)unlink(file);
  }
}

/* 
 * Map shared memory region and return address in CAfixed_p
 */
int 
CASysMap (int id)
{
  extern char *CA_mappoint(void);
  char *const mappoint = CA_mappoint();
  struct CacheFixed *const ca_p =
			(struct CacheFixed *)shmat(id, mappoint, SHM_RND);

  if (ca_p == (struct CacheFixed *)-1)
    return(CDS_NOTFND);

  CAfixed_p = ca_p;
  return(CDS_SUCCESS);
}

int 
CASysFindMem (void)
{
  char file[_DNS_MAXPATHLEN+1];
  register int loop = 2;
  int CacheId = -1;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " >CASysFindMem()"));

  (void)sprintf(file, CDS_ETC_DIR, dcelocal_path, DNS_SHMID_FILE);
  /*
   * Keep trying to find the file until we win.
   */
  while (TRUE) {
    char inbuf[30];
    struct shmid_ds shmds;
    int id, ch_time = 0;
    register int fd;

    if ((fd = open(file, O_RDONLY, 0)) < 0)
      if (--loop) {
	CA_delay();
	continue;
      } else
	break;

    id = read(fd, inbuf, sizeof(inbuf));
    (void)close(fd);
    /*
     * Maybe not created fully yet...
     */
    if (id < 0)
      if (--loop) {
	CA_delay();
	continue;
      } else
	break;

    inbuf[id] = '\0';
    /*
     * Maybe not written fully yet...
     *
     * may be 1 or 2 lines, depending on old version or new
     */
    if (sscanf (inbuf, "%d\n%d", &id, &ch_time) < 1)
      if (--loop) {
	CA_delay();
	continue;
      } else
	break;

    /*
     * If we can't access the memory data, assume gone.
     */
    if (shmctl(id, IPC_STAT, &shmds) < 0)
      if (--loop) {
	CA_delay();
	continue;
      } else
	break;

    /* Compare the memory ctime with the ctime in the file.
     * If they're unequal, then system was probably rebooted and
     * someone else got assigned our old memory id.  If they're equal,
     * the advertiser probably died ungracefully, before killing off
     * the clerk processes.
     */
    if (ch_time != shmds.shm_ctime)
      if (--loop) {
	CA_delay();
	continue;
      } else
	break;

    /* See if the creator (advertiser) is still around  */
    if (kill(shmds.shm_cpid, 0) < 0)
      if (--loop) {
	CA_delay();
	continue;
      } else
	break;

    /* This is it */
    CacheId = id;
    break;
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " <CASysFindMem() return(%d)",
      CacheId));

  return(CacheId);
}

CA_PRIVATE int 
CA_CheckShmem (int   CacheId)
{
  struct shmid_ds shmds;
  char file[_DNS_MAXPATHLEN+1];
  char inbuf[30], outbuf[30];
  int id, ch_time = 0;
  register int loop = 2;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " >CA_CheckShmem(%d)",
      CacheId));

  if (shmctl(CacheId, IPC_STAT, &shmds) < 0) {
    return (FALSE);
  }
  (void)sprintf(file, CDS_ETC_DIR, dcelocal_path, DNS_SHMID_FILE);
  /* File will contain memory id and last changed time of memory seg */
  (void)sprintf(outbuf, "%d\n%d\n", CacheId, shmds.shm_ctime);
/* Check for orphaned regions */
  /*
   * Keep trying to create the file until we win or someone else does.
   */
  while (TRUE) 
  {
    register int fd;

    if (0 <= (fd = open(file, O_CREAT|O_WRONLY|O_EXCL, 0644))) 
    {
      if (0 <= (id = write(fd, outbuf, strlen(outbuf)))) 
      {
	(void)close(fd);

  	DCE_SVC_DEBUG((
      	    cds__svc_handle,
      	    cds_svc_cache,
      	    svc_c_debug3,
      	    " <CA_CheckShmem(%d) return(TRUE)",
      	    CacheId));

	return(TRUE);
      }

      dce_svc_printf(CDS_S_CACHE_WRITESHMID_MSG, file, (long)errno);
      (void)close(fd);
      return(FALSE);
    } 
    else if (errno != EEXIST) 
    {
      dce_svc_printf(CDS_S_CACHE_CREATESHMID_MSG, file, (long)errno);
      return(FALSE);
    }

    /*
     * Someone could be writing the file right now...
     */
    if ((fd = open(file, O_RDONLY, 0)) < 0)
      if (--loop) 
      {
	CA_delay();
	continue;
      } 
      else 
      {
	dce_svc_printf(CDS_S_CACHE_OPENSHMID_MSG, file, (long)errno);
	return(FALSE);
      }
    id = read(fd, inbuf, sizeof(inbuf));
    (void)close(fd);
    /*
     * Maybe not created fully yet...
     */
    if (id < 0)
    {
      if (--loop) 
      {
	CA_delay();
	continue;
      } 
      else 
      {
	dce_svc_printf(CDS_S_CACHE_READSHMID_MSG, file, (long)errno);
	return(FALSE);
      }
    }
    /* make sure null-terminated */
    inbuf[id] = '\0';
    /*
     * Maybe not written fully yet...
     */
    if (sscanf (inbuf, "%d\n%d", &id, &ch_time) < 1) { /* may be 1 or 2 lines,
    * depending on old version or new  */
        if (--loop) 
	{
	    CA_delay();
	    continue;
	} 
	else 
	{
	    dce_svc_printf(CDS_S_CACHE_BADHEADERID_MSG, file, inbuf);

	    /* cause the file to be deleted */
	    id = -1;
	  }
      }
    /*
     * If we can't access the memory data, assume gone.
     */
    if ((0 <= id) && (0 <= shmctl(id, IPC_STAT, &shmds))) 
    {
      /*
       * See if anyone is still using the memory.
       */
      if (0 < shmds.shm_nattch) 
      {
	dce_svc_printf(CDS_S_CACHE_INUSE_MSG, id, shmds.shm_nattch);

        /* Compare the memory ctime with the ctime in the file.
	 * If they're unequal, then system was probably rebooted and
	 * someone else got assigned our old memory id.  If they're equal,
	 * the advertiser probably died ungracefully, before killing off the 
	 * clerk processes.
	 */
	if (ch_time == shmds.shm_ctime) 
	{
	/* See if the creator (advertiser) is still around  */
	  if (kill (shmds.shm_cpid, 0) >= 0) 
	  {
	    /*
	     * Creator process still active
	     */
	    dce_svc_printf(CDS_S_CACHE_CREATORMAPPED_MSG, id, shmds.shm_cpid);
	  }
	  else 
	  {
	    /*
	     * Creator process died, clerk process may still be around.
	     */
	    dce_svc_printf(CDS_S_CACHE_USERMAPPED_MSG, id);
	  }
	  /*
	   * Clean out the locks.
	   */
	  if (CASysMap(id) == CDS_SUCCESS) 
	  {
	    CACleanupLocks();
	    CASysUnmap();
	  }
	  (void)shmctl(id, IPC_RMID, 0);
	  dce_svc_printf(CDS_S_CACHE_RMID_MSG, id);
	}
	else 
	{  
	  if (ch_time > 0) 
	  { /* could be 0 if old shmid file format */
	    /* Someone else has it. Create new one */
	    dce_svc_printf(CDS_S_CACHE_REUSEID_MSG, id);
	  }
	}
      }
    }
    (void)unlink(file);
    dce_svc_printf(CDS_S_CACHE_SHMIDREMOVE_MSG, file);
  }
}

/*
 * Create shared memory region and map it. Set ca_id and ca_size on cache.
 */
int 
CASysCreateMem (int size)
{
  key_t key;
  int CacheId;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " >CASysCreateMem(%d)",
      size));

  /* Create shared memory region */  
  for (key = INIT_ID; ;++key ) {
    CacheId = shmget(key, size, IPC_EXCL | IPC_CREAT | 0700);
    if (CacheId != -1) break;
    if (errno != EEXIST) { 	/* If error is not 'key inuse' return */

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_cache,
          svc_c_debug3,
          " <CASysCreateMem(%d) return(-1)",
          size));

        return(-1);		/* Create a new one */
    }
  }

  if (!CA_CheckShmem(CacheId)) {

      (void)shmctl(CacheId, IPC_RMID, 0);

      DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_cache,
         svc_c_debug3,
         " <CASysCreateMem(%d) return(-1)",
         size));

      return(-1);
  }

  /* Shared memory has been created, Now map and load it */
  if (CASysMap(CacheId) != CDS_SUCCESS) {

      CASysDelete(CacheId);

      DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug3,
        " <CASysCreateMem(%d) return(-1)",
        size));

      return(-1);
  }

  /* Initialize header to nulls */
  ZERO_bytes(CAfixed_p, sizeof(struct CacheFixed));
  CAfixed_p->ca_size = size;
  CAfixed_p->ca_semid = -1;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " <CASysCreateMem(%d) return(%d)",
      size,
      CacheId));

  return(CacheId);
  
}


/*
 * Read the Current Version from disk.  
 */
CA_PRIVATE int 
CA_ReadCurrentVersion (void)
{
    char filename[_DNS_MAXPATHLEN+1];
    char version[DNS_VERSION_SIZE+1];
    int versionfile;
    int status;

    (void)sprintf(filename, CDS_CACHE_VERSION, dcelocal_path);

    versionfile = open(filename, O_RDONLY, 0);
    if (versionfile < 0) 
    {
	return(-1);
    }
  
    status = read(versionfile, version, DNS_VERSION_SIZE);
    if (status <= 0) 
    {
	dce_svc_printf(CDS_S_CACHE_READVERSION_MSG, filename, (long)errno);
	(void)close(versionfile);
	return(-1);
    }
    else if (status != DNS_VERSION_SIZE) 
    {
	dce_svc_printf(CDS_S_CACHE_BADVERSION_MSG, filename);
	(void)close(versionfile);
	return(-1);
    }
    
    /* convert from ascii */
    version[DNS_VERSION_SIZE] = 0;
    return atoi(version);
}

/*
 * CASysFindFile
 *
 * Find and open the last saved cache file
 */
int 
CASysFindFile (void)
{
  char name[_DNS_MAXPATHLEN+1];
  int sequence;

  /* Read version file */
  if ((sequence = CA_ReadCurrentVersion()) < 0) return(-1);

  /* Build cache file name */
  (void)sprintf(name, CDS_CACHE_FILE, dcelocal_path, sequence);
  
  /* Open cache file */
  return open(name, O_RDONLY, 0); 
}

/* 
 * Read Cache from disk.  If it doesn't exist return an error.
 * The cache file is left open on success, so it can be written later.
 * Only the advertiser makes this call, so locking of cache not needed
 * to access cache.
 * If size passed in arg differs from size of existing cache, 
 * delete old cache and return error.
 */
int 
CASysRead (int size)
{
  struct CacheFixed cache;
  char ca_file_name[_DNS_MAXPATHLEN+1];
  char ca_version_file[_DNS_MAXPATHLEN+1];
  char *buf_p;
  int save_lock;
  int CacheId;
  int cachefile;
  int u_status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " >CASysRead(%d)",
      size));

  /* Read version file */
  if ((ca_file_sequence = CA_ReadCurrentVersion()) < 0) return(-1);

  /* Build cache file names */
  (void)sprintf(ca_file_name, CDS_CACHE_FILE, dcelocal_path, ca_file_sequence);
  (void)sprintf(ca_version_file, CDS_CACHE_VERSION, dcelocal_path);
  
  /* Open cache file, on error return new cache created */
  cachefile = open(ca_file_name, O_RDONLY, 0); 
  if (cachefile < 0) 
  {
    dce_svc_printf(CDS_S_CACHE_OPENCACHE_MSG, ca_file_name, (long)errno);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug3,
        " <CASysRead(%d) return(-1)",
        size));

    return(-1);
  }
  
  /* File was found, read fixed portion to get cache size */
  u_status = read(cachefile, (char *)&cache, sizeof(struct CacheFixed));  
  if (u_status <= 0) {
    dce_svc_printf(CDS_S_CACHE_READCACHE_MSG, ca_file_name, (long)errno);

    (void)close(cachefile);
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug3,
        " <CASysRead(%d) return(-1)",
        size));

    return(-1);
    }
  else if (u_status < sizeof(struct CacheFixed)) 
  {
    dce_svc_printf(CDS_S_CACHE_BADCACHEHEADER_MSG, ca_file_name);

    (void)close(cachefile);
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug3,
        " <CASysRead(%d) return(-1)",
        size));

    return(-1);
    }

  /*
   * Make sure internal cache version is current.
   * If not, delete both cache file and cache version file that pointed to it.
   */
  if (!CheckCacheVersion(&cache)) 
  {
    (void)close(cachefile);
    dce_svc_printf(CDS_S_CACHE_OLDVERSION_MSG);

    (void)CAdelete(ca_file_name);
    (void)CAdelete(ca_version_file);
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug3,
        " <CASysRead(%d) return(-1)",
        size));

    return(-1);
  };

/* Determine cache size specification */
  size = CA_SysSize (size);

 if (size != cache.ca_size) 
 { 
    /* delete this cache */
    dce_svc_printf(CDS_S_CACHE_CACHESIZE_MSG, size);

    (void)CAdelete(ca_file_name);
    (void)CAdelete(ca_version_file);
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug3,
        " <CASysRead(%d) return(-1)",
        size));

    return (-1);
  }

  /* Create and map shared memory */
  CacheId = CASysCreateMem(cache.ca_size);
  if (CacheId == -1) {
    (void)close(cachefile);
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug3,
        " <CASysRead(%d) return(-1)",
        size));

    return(-1);
  }
  
  /* Move fixed portion into shared memory */
  save_lock = CAfixed_p->ca_semid;
  *CAfixed_p = cache;
  CAfixed_p->ca_semid = save_lock;
  
  /* Read the rest of cache into memory */
  size = CAfixed_p->ca_size - sizeof(struct CacheFixed);
  buf_p = (char *)(CAfixed_p + 1);
  while (size > 0) {
    if (size > _CDS_PAGE_SIZE_) 
      u_status = read(cachefile, buf_p, _CDS_PAGE_SIZE_);
    else 
      u_status = read(cachefile, buf_p, size);
    
    /* If what we were expecting wasn't there, return an error */  
    if (u_status <= 0) {
      dce_svc_printf(CDS_S_CACHE_READCACHE_MSG, ca_file_name, (long)errno);

      (void)close(cachefile);
      CASysDelete(CacheId);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_cache,
          svc_c_debug3,
          " <CASysRead(%d) return(-1)",
          size));

      return(-1);
      }
    
    size -= u_status;
    buf_p += u_status;
  }

  (void)close(cachefile);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " <CASysRead(%d) return(%d)",
      size,
      CacheId));

  return(CacheId);
}

/*
 * CAUserWrite - Cache flush for use by random applications
 *               (e.g. DECNET_LOC_REGISTER)
 *
 * On VMS, CAfixed_p must already be pointing at a user-mode-accessible
 * area that contains the cache data.  This is to avoid having to switch
 * to kernel mode.
 */

int 
CAUserWrite (void)
{
    /* Read and set the current version of the cache */

    if ((ca_file_sequence = CA_ReadCurrentVersion()) < 0) return(-1);

    /* Lock the cache and write it to disk */

    if (CAfixed_p) 
    {

        CAWriteLockCache();
        CASysWrite();
        CAWriteUnlockCache();
    }
    return(0);
}


/*
 * Write cache to disk.  Ignore errors.
 * Cache should be write locked to prevent others from making any changes.
 * This routine should only be called by the process that
 * created shared memory.  It assumes some global variables are set.
 */
void 
CASysWrite (void)
{
  int status;
  int size;
  char *buf_p;
  int cachefile,versionfile;
  char name[100];
  char version[DNS_VERSION_SIZE];


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " >CASysWrite()"));

  /* Create cache file, using sequence number to make file unique */
  (void)sprintf(name, CDS_CACHE_FILE, dcelocal_path, ++ca_file_sequence);

  /* Get size of cache, to create big enough file on VMS */
  size = CAfixed_p->ca_size;

  /* 
   * Create new cachefile, on error cache not saved.
   * Use o_sync instead of o_fsync for OSF
   */
   
  cachefile = open(name, O_RDWR | O_CREAT | O_TRUNC | O_SYNC, 0600); 

  if (cachefile < 0) 
  {
    dce_svc_printf(CDS_S_CACHE_CREATECACHE_MSG, name, (long)errno);
    return;
    }
  
  /* Write entire cache to disk */
  buf_p = (char *)CAfixed_p;

  while (size > 0) 
  {
    /*
     *  Write out file all at once, checking to make sure it was indeed all
     *	written.  If not, continue to write until done.
     */
    status = write(cachefile, buf_p, size);

    /*
     * If didn't all make it, return an error.
     */
    if (status <= 0) 
    {
      dce_svc_printf(CDS_S_CACHE_WRITECACHE_MSG, name, status, (long)errno);
      break;
      }    
    size -= status;
    buf_p += status;
  }
  
  (void)close(cachefile);
  
  /* If didn't write entire cache, delete this file */
  if (size > 0) 
  {
    (void)CAdelete(name);
    /* decrement the sequence since we didn't successfully write */
    ca_file_sequence--;
    }

  else 
  {
    char ca_version_file[_DNS_MAXPATHLEN+1];

    (void)sprintf(ca_version_file, CDS_CACHE_VERSION, dcelocal_path);
    /*
     * Update version number 
     * Use o_sync instead of o_fsync for OSF
     */
    versionfile = open(ca_version_file, O_RDWR | O_TRUNC | O_CREAT | O_SYNC,  
		       0600);
    if (versionfile < 0) 
    {
      dce_svc_printf(CDS_S_CACHE_CREATEVERSION_MSG, ca_version_file, (long)errno);

      (void)CAdelete(name);
    }
    else 
    {
      (void)sprintf(version, DNS_VERSION_FMT, ca_file_sequence);
      status = write(versionfile, version, DNS_VERSION_SIZE);
      (void)close(versionfile);
      
      /* If error, delete files */
      if (status < DNS_VERSION_SIZE) 
      {
	(void)CAdelete(name);
        (void)CAdelete(ca_version_file);
      } 
      else 
      {
	/* Delete previous version of cache file */
	(void)sprintf(name, CDS_CACHE_FILE, dcelocal_path, ca_file_sequence-1);
	(void)CAdelete(name);
      }
    }
  }

  CA_write_servers();

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug3,
      " <CASysWrite()"));
}

/*
 * Create a new cache in memory.
 * Returns shared memory id.
 */
int 
CASysCreate (int size)
{
  
  size = CA_SysSize(size);
  
  /* Create and map memory */
  return(CASysCreateMem(size));
  
}

CA_PRIVATE void
CA_delay (void)
{
    struct timespec timer;

    timer.tv_sec = 1;
    timer.tv_nsec = 0;
    (void)pthread_delay_np(&timer);
}
