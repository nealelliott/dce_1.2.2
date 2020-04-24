/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * stub.h -- 
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: stub.h,v $
 * Revision 1.1.116.1  1996/10/17  18:18:11  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:58  damon]
 *
 * Revision 1.1.111.1  1994/02/04  20:40:19  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:22:24  devsrc]
 * 
 * Revision 1.1.109.1  1993/12/07  17:41:38  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:42:10  jaffe]
 * 
 * Revision 1.1.9.2  1993/07/19  19:40:16  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:30:50  zeliff]
 * 
 * Revision 1.1.7.3  1993/07/16  21:30:30  kissel
 * 	*** empty log message ***
 * 	[1993/06/21  14:52:16  kissel]
 * 
 * Revision 1.1.5.2  1993/06/04  20:52:14  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:40:10  kissel]
 * 
 * Revision 1.1.3.2  1993/03/29  19:12:13  kissel
 * 	Porting changes for HPUX: the wlr_localdirent is the same as for OSF1.  There
 * 	is a related change in spoke.c.
 * 	[1993/03/29  18:20:46  kissel]
 * 
 * Revision 1.1.2.3  1993/01/11  18:25:35  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:57:42  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  16:11:25  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  17:59:36  jaffe]
 * 
 * $EndLog$
 */
/* define structures for marshalling */

#ifndef _STUB_H_ENV_
#define _STUB_H_ENV_1

#include <sys/types.h>
#include <sys/stat.h>

#define WLR_STRINGSIZE		1024
#define WLR_BLOCKSIZE		8192

/* opcodes for marshalling/unmarshalling */
#define WLR_OP_CHMOD		1
#define WLR_OP_STAT		2
#define WLR_OP_OPEN		3
#define WLR_OP_READ		4
#define WLR_OP_WRITE		5
#define WLR_OP_FTRUNC		6
#define WLR_OP_FSTAT		7
#define WLR_OP_CLOSE		8
#define WLR_OP_TRUNC		10
#define WLR_OP_RENAME		11
#define WLR_OP_UNLINK		12
#define WLR_OP_LINK		13
#define WLR_OP_SYMLINK		14
#define WLR_OP_MKDIR		15
#define WLR_OP_RMDIR		16
#define WLR_OP_UTIMES		17
#define WLR_OP_READDIR		18
#define WLR_OP_READLINK		19

/* This section defines common data passed between the hub and the wheel
 * in running tests.  Things that describe the core "rpc" protocol should
 * be defined in com.h; only applications-level stuff goes here.
 */

/* description of dirent read from the kernel */
#if defined(OSF1) || defined(__hpux)
struct wlr_localdirent {
    long inode;
    short recLength;
    short nameLength;
    char name[1];
};
#else	/* defined(OSF1) || defined(__hpux) */
#if sun
struct wlr_localdirent {
    long inode;
    long offset;
    short recLength;
    char name[1];
};
#else /* sun */
struct wlr_localdirent {
    long offset;
    long inode;
    short recLength;
    short nameLength;
    char name[1];
};
#endif /* sun */
#endif /* defined(OSF1) || defined(__hpux) */

/* basic info we pass over for a directory entry.  Generally matches
 * the Unix equivalent, but it is hopelessly unportable to use struct
 * defined by the kernel.
 */
struct wlr_netdirent {
    long offset;
    long inode;
    short recLength;
    short nameLength;
    char name[1];
};

#define WLR_ERROR_EXISTS	1001	/* already exists */
#define WLR_ERROR_BIGRECORD	1002	/* big record found in a dir */
#define WLR_ERROR_DUPENTRIES	1003	/* duplicate entries in a dir */
#define WLR_ERROR_BADOP		1004	/* bad opcode */

/* prototypes for stub.c, except internal XDR functions */

extern wlr_XDRStat _TAKES((struct wlr_call *cp, struct stat *statp,
			   int marshall));
extern wlr_XDRString _TAKES((struct wlr_call *cp, char *stringp, int marshall));
extern wlr_XDRBlock _TAKES((struct wlr_call *cp, char *stringp, int marshall));
extern wlr_XDRLong _TAKES((struct wlr_call *cp, long *lp, int marshall));
extern wlr_XDRShort _TAKES((struct wlr_call *cp, short *sp, int marshall));
extern wlr_DoChmod _TAKES((struct wlr_client *cp, char *pathp, long mode));
extern wlr_DoStat _TAKES((struct wlr_client *cp, char *p, struct stat *stp));
extern wlr_DoOpen _TAKES((struct wlr_client *cp, char *p, long flags,
		  long mode, long *fdp));
extern wlr_DoRead _TAKES((struct wlr_client *cp, long fd, long offset, long len,
		  char *blockp, long *lenp));
extern wlr_DoWrite _TAKES((struct wlr_client *callp, long fd,
			   long offset, long len,
			   char *blockp, long *lenp));
extern wlr_DoFtrunc _TAKES((struct wlr_client *cp, long fd, long len));
extern wlr_DoFstat _TAKES((struct wlr_client *cp, long fd, struct stat *stp));
extern wlr_DoClose _TAKES((struct wlr_client *cp, long fd));
extern wlr_DoTrunc _TAKES((struct wlr_client *cp, char *p, long len));
extern wlr_DoRename _TAKES((struct wlr_client *cp, char *n1, char *n2));
extern wlr_DoUnlink _TAKES((struct wlr_client *cp, char *p));
extern wrl_DoLink _TAKES((struct wlr_client *cp, char *n1, char *n2));
extern wrl_DoSymlink _TAKES((struct wlr_client *cp, char *n1, char *n2));
extern wlr_DoMakedir _TAKES((struct wlr_client *cp, char *p, long mode));
extern wlr_DoRmdir _TAKES((struct wlr_client *cp, char *p));
extern wlr_DoUtimes _TAKES((struct wlr_client *cp, char *p, long mtimeSec,
		    long mtimeUsec, long atimeSec, long atimeUsec));
extern wlr_DoReaddir _TAKES((struct wlr_client *cp,
		     long fd, long offset, long len,
		     char *blockp, long *lenp));
extern wlr_DoReadlink _TAKES((struct wlr_client *cp, char *p,
			      char *block, long *lenp));

/* other XDR functions */
extern wlr_XDRChmodIP _TAKES((struct wlr_call *cp, char *pathp, long *modep,
		      int marshall));
extern wlr_XDROpenIP _TAKES((struct wlr_call *cp, char *pathp,
			     long *flagsp, long *modep, int marshall));
extern wlr_XDRReadIP _TAKES((struct wlr_call *cp, long *fdp,
			     long *offsetp, long *lenp, int marshall));
extern wlr_XDRReadOP _TAKES((struct wlr_call *cp, char *blockp,
			     long *lenp, int marshall));
extern wlr_XDRWriteIP _TAKES((struct wlr_call *cp, long *fdp,
			      long *offsetp, long *lenp, char *blockp,
			      int marshall));
extern wlr_XDRFtruncIP _TAKES((struct wlr_call *cp, long *fdp,
			       long *lenp, int marshall));
extern wlr_XDRTruncIP _TAKES((struct wlr_call *cp, char *namep,
			      long *lenp, int marshall));
extern wlr_XDRRenameIP _TAKES((struct wlr_call *cp, char *srcp,
			       char *destp, int marshall));
extern wlr_XDRLinkIP _TAKES((struct wlr_call *cp, char *sp,
			     char *tp, int marshall));
extern wlr_XDRSymlinkIP _TAKES((struct wlr_call *cp, char *sp,
				char *tp, int marshall));
extern wlr_XDRMakedirIP _TAKES((struct wlr_call *cp, char *pathp,
				long *modep, int marshall));
extern wlr_XDRUtimesIP _TAKES((struct wlr_call *cp, char *pathp,
			       long *mts, long *mtu, long *ats,
			       long *atu, int marshall));
extern wlr_XDRReaddirIP _TAKES((struct wlr_call *cp, long *fdp, long *offsetp,
				long *lenp, int marshall));
extern wlr_XDRReaddirOP _TAKES((struct wlr_call *cp, char *blockp,
				long *lenp, int marshall));
extern wlr_XDRReadlinkOP _TAKES((struct wlr_call *cp, char *stringp,
				 long *lenp, int marshall));
#endif /* _STUB_H_ENV_ */
