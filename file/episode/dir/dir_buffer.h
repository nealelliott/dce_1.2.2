/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dir_buffer.h,v $
 * Revision 1.1.70.1  1996/10/02  17:20:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:39  damon]
 *
 * Revision 1.1.64.2  1994/06/09  14:00:06  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:24  annie]
 * 
 * Revision 1.1.64.1  1994/02/04  20:12:51  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:03  devsrc]
 * 
 * Revision 1.1.62.1  1993/12/07  17:18:29  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:54:23  jaffe]
 * 
 * Revision 1.1.5.5  1993/01/19  15:25:52  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:45:48  cjd]
 * 
 * Revision 1.1.5.4  1993/01/13  16:40:54  shl
 * 	Transarc delta: bwl-ot6468-delete-ancient-notdefs 1.1
 * 	  Selected comments:
 * 	    Delete ancient notdef'ed code.
 * 	    Delete dirb_Shutdown.
 * 	[1993/01/12  20:06:47  shl]
 * 
 * Revision 1.1.5.3  1992/11/24  16:24:14  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:22:07  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/31  19:21:18  jaffe
 * 	Transarc delta: jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	  Selected comments:
 * 	    This revision of this delta contains changes made to episode/async by the OSF
 * 	    between dce1.0 and dce1.0.1b9.  Subsequent revisions will contain merges
 * 	    for other episode subdirectories.
 * 	    This revision of this delta contains changes made to episode/dir by the OSF
 * 	    Merged in changes made by the OSF between dce1.0 and dce1.0.1b9.
 * 	    This revision of this delta contains changes made to episode/lobguf by the OSF
 * 	    This revision of this delta contains changes made to episode/salvage by the OSF
 * 	    This revision of this delta contains changes made to episode/sautils by the OSF
 * 	    This revision of this delta contains changes made to episode/tools by the OSF
 * 	    This revision of this delta contains changes made to episode/utils by the OSF
 * 	    This revision of this delta contains changes made to episode/vnops by the OSF
 * 	    between dce1.0 and dce1.0.1b9.
 * 	    This revision fixes some problems in the logbuf layer that occurred after
 * 	    the merge.
 * 	    This delta adds anode/epi_errs.msf.
 * 	    This delta fixes problems in the logbuf layer.
 * 	    This revision fixes some problems in the vnode layer.
 * 	    Fix more vnode layer problems.
 * 	    Add files that were introduced after DCE 1.0 to the .klib Makefiles.
 * 	    This delta fixes various problems that caused undefined references
 * 	    while building the AIX kernel extension.
 * 	    This revision fixes random problems encountered under OSF/1.
 * 	    This revision fixes some problems encountered when compiling test_vnodeops
 * 	    on OSF/1.
 * 	    This revision includes the OSF's changes to epimount.
 * 	Transarc delta: jdp-ot4179-lfs-encapsulate-initialization 1.3
 * 	  Selected comments:
 * 	    This delta simplifies Episode user-space initialization.  There are two parts
 * 	    to this change.  The first simplifies the process of calling the *_Init
 * 	    routines.  Previously, you typically had to call dirb_Init(), N anode init
 * 	    routines, and logbuf_Init().  With this change, you need only specify the
 * 	    initialization parameters to the necessary layers in one structure and call
 * 	    the top-most init routine.  Since each initialization routine is now
 * 	    responsible for calling the lower layers' initialization routines, only
 * 	    one call is necessary.  The second change made in this delta was the
 * 	    encapsulation of the user-space device initialization code in one routine,
 * 	    us_InitDevice().  This routine is declared in tools/us.h and defined in
 * 	    tools/us_io.c.
 * 	    Changed signature of dirb_Init().
 * 	    Fix a couple of remaining compilation problems.
 * 	    Fix compilation problems on OSF/1.
 * 	[1992/08/30  02:16:53  jaffe]
 * 
 * Revision 1.1.3.5  1992/05/13  21:58:22  jdp
 * 	Transarc delta: bwl-dir-create-2633 1.1
 * 	  Files modified:
 * 	    dir: dir_buffer.c, dir_buffer.h, dir_main.c, dir_salvage.c
 * 	    dir: dtest.c, test_dir.c; salvage: walk.c
 * 	    vnops: efs_dir.c, efs_vnodeops.c, quota.test
 * 	  Selected comments:
 * 	    Fix bugs in error recovery during directory insertion (create, mkdir, etc.);
 * 	    as a by-product, reorganized directory insertion so that for multi-page
 * 	    directories, only one search through the directory's pages is done.
 * 	    Add dirb_AnnulPage.
 * 	[1992/05/13  21:01:41  jdp]
 * 
 * Revision 1.1.3.4  1992/05/05  19:04:19  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  21:21:57  jdp]
 * 
 * Revision 1.1.3.3  1992/01/24  03:00:11  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:09:22  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:42:24  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:28:32  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:12:54  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/*
 * Copyright (C) 1990 Transarc Corporation
 * All rights reserved.
 */

/*                          DEcorum File System
                         Directory Buffer Package                          */

#ifndef DIR_BUFFER_H
#define DIR_BUFFER_H

#include <dcedfs/episode/aggregate.h>	/* For epig_params */
#include <sys/types.h>

/* Directory Handle Interface */

struct dh_ops {
    int (*dh_read)();			/* read disk object */
    int (*dh_write)();			/* write disk object */
    u_int32 (*dh_len)();		/* length of disk object */
    int (*dh_hash)();			/* value to be hashed */
    int (*dh_eq)();			/* compare handles */
    void (*dh_zero)();			/* blank out obsolete handle */
    void (*dh_copy)();			/* copy handle */
};

#define DHOP_READ(dir,addr,data,len) (*dir->dh_ops->dh_read)(dir,addr,data,len)
#define DHOP_WRITE(dir,tranId,addr,data,len) (*dir->dh_ops->dh_write) \
						(dir,tranId,addr,data,len)
#define DHOP_LENGTH(dir) (*dir->dh_ops->dh_len)(dir)
#define DHOP_EQ(xdir,buff) (buff->dir.dh_ops->dh_eq == xdir->dh_ops->dh_eq \
				&& (*xdir->dh_ops->dh_eq)(&buff->dir,xdir))
#define DHOP_ZEROB(buff) (*buff->dir.dh_ops->dh_zero)(&buff->dir)
#define DHOP_COPY(xdir,buff) (*xdir->dh_ops->dh_copy) (&buff->dir,xdir)
#define DHOP_HASH(dir,page) (*dir->dh_ops->dh_hash)(dir,page)
#define DHOP_HASHB(buff,page) (*buff->dir.dh_ops->dh_hash)(&buff->dir,page)

struct Dir_Handle {
    struct dh_ops *dh_ops;		/* operators */
    int32 dh_data[6];			/* private data */
};

struct dirb_params {
    int abuffers;
    long spare[7];
    struct epig_params epigParams;
};
#define DIRB_EPIG_PARAMS(x) ((x).epigParams)
#define DIRB_ELBB_PARAMS(x) ((x).epigParams.elbbParams)

/* Exported functions */

caddr_t dirb_Read(struct Dir_Handle *dir, int page);
caddr_t dirb_Read0(struct Dir_Handle *dir, int page, u_int *reallength);
caddr_t dirb_New(struct Dir_Handle *dir, int page);
void dirb_Release(caddr_t dp);
int dirb_Init(struct dirb_params *dirbParams);
void dirb_Stat(int *abuffers, int *acalls, int *aios);
void dirb_Annul(struct Dir_Handle *dir);
void dirb_AnnulPage(struct Dir_Handle *dir, int page);

#endif /* DIR_BUFFER_H */
