/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsSpoke_mgr.c,v $
 * Revision 1.1.6.1  1996/10/17  18:28:30  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:18  damon]
 *
 * $EndLog$
 */
/*
 * ID: $Id: dfsSpoke_mgr.c,v 1.1.6.1 1996/10/17 18:28:30 damon Exp $
 *
 * COMPONENT_NAME: Distributed ITL Development Infrastructure
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1993
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */

#include <dirent.h>
#include <dce/rpc.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/defs.h>
#include <utils/itl/dfsSpoke_defs.h>

PRIVATE unsigned32 dfsSpokeMgr_Stat(h, f, out_stat)
  IN handle_t h;
  IN unsigned char * f;
  INOUT dfsSpoke_stat_t * out_stat;
{
    struct stat 	stat_buf;
    int 		err;
    
    err = stat((char *)f, (struct stat *)&stat_buf);
    if (err == -1)
	return errno;	/* map these to some common form, you slime */

    CONVERT_LOCAL_STAT_TO_SPOKE_STAT(&stat_buf, out_stat);

    return 0;
}

PRIVATE unsigned32 dfsSpokeMgr_Readdir(h, f, out_dir)
  IN handle_t h;
  IN unsigned char * f;
  INOUT dfsSpoke_dirent_array_t * out_dir;
{
    DIR * dirp;
    struct dirent * entp;
    int n;
    int i;
    
    dirp = opendir((char *)f);
    if (!dirp)
	return errno;

    /* do this the easy way: */

    n = 0;
    do {
#ifndef	SUNOS5
	if ((entp = readdir(dirp)) && entp->d_namlen) n++;
#else
	if (entp = readdir(dirp)) n++;
#endif
    } while (entp);

    out_dir->entry = rpc_ss_allocate(sizeof(dfsSpoke_dirent_t) * n);
    out_dir->n_entries = n;

    rewinddir(dirp);

    for (i = 0; i < n ; i++) {
	entp = readdir(dirp);
#ifndef	SUNOS5
	if (entp && entp->d_namlen)
#else
	if (entp)
#endif
	    dfsSpoke_NormalizeLocalDirent(entp, 
					  (dfsSpoke_dirent_t *) &(out_dir->entry[i]));
    }
    
    closedir(dirp);
    
    return 0;
}

dfsSpoke_v1_0_epv_t dfsSpoke_v1_0_epv = {
    dfsSpokeMgr_Stat,
    dfsSpokeMgr_Readdir
};

SHARED boolean_t dfsSpokeMgr_Init()
{
    unsigned32		status;
    boolean_t retVal = FALSE;
    
    rpc_server_register_if(dfsSpoke_v1_0_s_ifspec, NULL,
			   (rpc_mgr_epv_t) &dfsSpoke_v1_0_epv, &status);

    if (status)
    {
	DCE_ERROR(status, "dfsSpoke register_if FAILED");
	retVal = TRUE;
    }

    return(retVal);
}
