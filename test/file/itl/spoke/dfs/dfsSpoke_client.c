/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsSpoke_client.c,v $
 * Revision 1.1.6.1  1996/10/17  18:28:25  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:15  damon]
 *
 * $EndLog$
 */
/*
 * ID: $Id: dfsSpoke_client.c,v 1.1.6.1 1996/10/17 18:28:25 damon Exp $
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
#include <string.h>
#include <dirent.h>
#include <dce/rpc.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/dfsSpoke_defs.h>
#include <utils/itl/dfsSpoke.h>

EXPORT unsigned32 dfsSpokeClient_Stat(h, f, stat_buf)
  IN handle_t h;
  IN char *f;
  INOUT struct stat	*stat_buf;
{
    dfsSpoke_stat_t	remote_stat;
    error_status_t	err;

    err = dfsSpoke_RemoteStat(h, (idl_char *)f, &remote_stat);
    (void) memset((char *) stat_buf, 0, sizeof(*stat_buf)); 
    CONVERT_SPOKE_STAT_TO_LOCAL_STAT(&remote_stat, stat_buf);

    return err;
}

EXPORT unsigned32 dfsSpokeClient_Readdir(h, f, n_entries, dir_entries)
  IN handle_t h;
  IN char * f;
  INOUT int * n_entries;
  OUT struct dirent ** dir_entries;
{
    int			i, err;
    dfsSpoke_dirent_array_t entry;

    entry.n_entries = 0;
    entry.entry = NULL;
    
    err = dfsSpoke_RemoteReaddir(h, (idl_char *)f, &entry);

    *n_entries = entry.n_entries;
    if (entry.n_entries)
	ITL_ALLOC(*dir_entries, struct dirent *,
		  (sizeof(struct dirent) * (*n_entries)));
    else
	*dir_entries = (struct dirent *) 0;

    for (i = 0;  i < *n_entries; i++) {
	dfsSpoke_LocalizeNormalDirent(&(entry.entry[i]),
				      &((*dir_entries)[i]));
    }

    free(entry.entry);
    return err;
}

