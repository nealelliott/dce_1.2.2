/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: name.c,v $
 * Revision 1.1.73.1  1996/10/02  17:27:19  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:33:55  damon]
 *
 * Revision 1.1.68.3  1994/07/13  22:22:26  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:43:49  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:57:13  mbs]
 * 
 * Revision 1.1.68.2  1994/06/09  14:02:59  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:38:01  annie]
 * 
 * Revision 1.1.68.1  1994/02/04  20:14:54  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:11  devsrc]
 * 
 * Revision 1.1.66.1  1993/12/07  17:20:33  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  14:55:28  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/19  15:32:58  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:54:57  cjd]
 * 
 * Revision 1.1.5.3  1992/11/24  16:56:08  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:25:27  bolinger]
 * 
 * Revision 1.1.5.2  1992/11/18  19:32:26  jaffe
 * 	Transarc delta: cburnett-ot5890-rename-salvage.h-to-salveger.h 1.1
 * 	  Selected comments:
 * 	    Rename salvage/salvage.p.h to salvage/salvager.p.h to avoid having
 * 	    two salvage.h include files.  There is also a salvage.h in the anode dir.
 * 	    salvage.p.h to salvager.p.h
 * 	[1992/11/17  21:16:18  jaffe]
 * 
 * Revision 1.1.3.5  1992/06/23  13:55:32  jdp
 * 	Transarc delta: ota-ot3897-missing-rcsids 1.2
 * 	  Selected comments:
 * 	    Apparently some RCSID macros had been removed from some salvage files.
 * 	    Add RCSID.
 * 	    Add some more RCSID's
 * 	[1992/06/22  22:10:10  jdp]
 * 
 * Revision 1.1.3.4  1992/05/05  18:31:25  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  22:48:20  jdp]
 * 
 * Revision 1.1.3.3  1992/01/24  03:00:44  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:10:13  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:43:56  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:29:48  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:13:45  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/* Copyright (C) 1991, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/salvage/name.c,v 1.1.73.1 1996/10/02 17:27:19 damon Exp $")

#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/file.h>
#include <dcedfs/dir_buffer.h>
#include <dcedfs/dir.h>
#include <dcedfs/episode/salvage.h>

#include <name.h>
#include <salvager.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

SHARED struct epis_path * epis_AllocNameElem( elem, path )
  IN char * elem;
  IN struct epis_path * path;
{
    struct epis_path * newPath;

    assert( (elem != 0) );
    newPath = (struct epis_path *) osi_Alloc( sizeof(*newPath) );
    newPath->name = (char *) osi_Alloc( strlen(elem)+1 );
    strcpy( newPath->name, elem );
    newPath->prev = path;
    if (!path)
	/* This is the root pathname, so pathname and name are the same. */
	newPath->pathname = newPath->name;
    else 
	newPath->pathname = 0;
    newPath->refCount = 0;

    return newPath;
}

PRIVATE int epis_lostPathnames = 0;

SHARED void epis_FreeNameElem(path)
  IN struct epis_path * path;
{
    assert( path != 0 );
    if (path->name) {
	(void)osi_Free( (caddr_t)path->name, strlen(path->name)+1 );
    }
    if (path->pathname) {
	if (path->refCount == 0) {
	    if (path->name != path->pathname) /* special case root */
		osi_Free ((opaque)path->pathname, strlen(path->pathname)+1);
	} else
	    epis_lostPathnames++;
    } else {
	afsl_MBZ (path->refCount);
    }
    (void)osi_Free( (caddr_t)path, sizeof(*path) );
}

#if 0
/*
** CatPrevPath.
** Use a recursive routine instead of a stack
** to invert the order of the path names.
*/
PRIVATE char * CatPrevPath( entirePath, path )
  IN char * entirePath;
  IN struct epis_path * path;
{
    if (path->prev) {
	CatPrevPath( entirePath, path->prev );
    }
    if (path->name) {
	strcat( entirePath, path->name );
    }
    return( entirePath );
}
#endif

/*
** epis_MakeNamePath
** Turn the latest path name into a real path name.
*/
SHARED char * epis_MakeNamePath (path)
  IN struct epis_path * path;
{
    char *entirePath;
    char *parentPath;
    long totalLen = 0;

    afsl_Assert (path);

    /* If it already exists, just bump reference count and return it. */
    if (path->pathname) {
	path->refCount++;
	return path->pathname;
    } else {
	afsl_MBZ (path->refCount);
    }

    parentPath = epis_MakeNamePath (path->prev);
    totalLen = strlen (parentPath) + strlen (path->name) + 1;

    entirePath = (char *) osi_Alloc( totalLen );
    strcpy (entirePath, parentPath);
    strcat (entirePath, path->name);

    path->pathname = entirePath;
    path->refCount = 1;
    return entirePath;
}

SHARED void epis_ReleaseNamePath (path)
  IN struct epis_path * path;
{
    path->refCount--;
    afsl_PAssert (path->refCount >= 0,
		  ("epis_ReleaseNamePath: Reference count is negative"));
}  

/*
 * epis_DecorateNames -- This is the place we name all the entries on
 *     epiz_salvageState where we scan for anodeIx/volIx, and substitute the
 *     path names we find. */

SHARED long epis_DecorateNames (anodeIx, volIx, path)
  IN u_long anodeIx;
  IN u_long volIx;
  IN struct epis_path * path;
{
    long code;
    struct epiz_anode * a;

    a = epiz_FindAnode (anodeIx, volIx);
    if (a) {
	if (epiz_DecorateAnode( a, epis_MakeNamePath( path ) ))
	    /* release the pathname if not used. */
	    epis_ReleaseNamePath (path);
    }
    return 0;
}
