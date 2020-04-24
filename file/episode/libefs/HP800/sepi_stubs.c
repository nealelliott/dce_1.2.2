/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sepi_stubs.c,v $
 * Revision 1.1.40.1  1996/10/02  17:24:10  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:31:52  damon]
 *
 * Revision 1.1.35.1  1994/06/09  14:00:23  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:39  annie]
 * 
 * Revision 1.1.2.5  1993/01/19  15:26:34  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:46:42  cjd]
 * 
 * Revision 1.1.2.4  1992/11/24  16:24:47  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:22:50  bolinger]
 * 
 * Revision 1.1.2.3  1992/09/25  16:20:22  jaffe
 * 	Transarc delta: jaffe-ot5416-cleanup-RCS-log-entries-epi 1.1
 * 	  Selected comments:
 * 	    Cleanup extra RCS log entries.  There should only be one per file, and
 * 	    it should be closed with an EndLog comment.
 * 	[1992/09/23  17:46:03  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  19:36:41  jaffe
 * 	Transarc delta: mason-add-hp800-osi-routines 1.15
 * 	  Selected comments:
 * 	    This delta is misnamed - these are really porting fixes.
 * 	    Need to checkpoint to fix a different bug.
 * 	    Checkpoint the first system where test_anode works.  Only one change is not
 * 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
 * 	    is -lBSD.  This should be fixed more generally.
 * 	    More HP-UX changes.
 * 	    More HP-UX related changes.
 * 	    Fix compile problem on the RIOS.
 * 	    Fix more compilation problems.
 * 	    Checkpoint.
 * 	    Added afs_syscall initialization code.
 * 	    Checpoint.
 * 	    More stubs.
 * 	    In this snap we turned all malloc() calls into osi_Alloc calls.
 * 	    Also turned free calls into osi_Free.
 * 	    HP-UX EOF detection fix.
 * 	    Another checkpoint.
 * 	    This is a checkpoint of the first working HP-UX Episode system.
 * 	    Stub routines for compiling a stand-alone episode system.
 * 	    Needed to add the vnops routines specific to HP-UX.
 * 	    Fix compilation problems.
 * 	[1992/08/30  02:23:19  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1992 Transarc Corporation - All Rights Reserved
 */

/*
 * This file declares stubs for use in a stand-alone Episode (no glue, no
 * volume ops, no ag ops, etc.)
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>

/*
 * volreg_* routines.
 */

volreg_Lookup() {panic("volreg_Lookup");}
volreg_Init() { return 0; }

/*
 * tkc_* routine(s)
 */
tkc_Init() { return 0; }

/*
 * xvfs_InitFromVFSOps - this is a no-op on the standalone system 'cuz nobody
 * calls the X ops.
 */
xvfs_InitFromVFSOps() { return 0; }

char *
strcat(s1, s2)
        register char *s1, *s2;
{
        register char *os1;

        os1 = s1;
        while (*s1++)
                ;
        --s1;
        while (*s1++ = *s2++)
                ;
        return (os1);
}

/*
 * This is REALLY STUPID.  I'll track down the places we do asserts later...
 */

#undef assert

assert(must_be_true)
  int must_be_true;
  {
      if (!must_be_true)
	  panic("assertion failed");
  }

/*
 * No pag routines on a stand-alone episode system
 */

osi_GetPagFromCred() { return -1; }

/* not implemented; should not be called */
osi_SetPagInCred() { panic("SetPagInCred"); }

/*
 * This is the entry point for stand-alone episode initialization; it is
 * called exactly once upon system intialization
 */

#ifndef AFS_SYSCALL
#define AFS_SYSCALL 49
#endif /* AFS_SYSCALL */

epic_link()
  {
      extern int afs_syscall();

      sysent_assign(AFS_SYSCALL, 6, afs_syscall);
  }

/*
 * other entry points we stub out - only afscall_episode is working...
 */
afscall_cm_setpag() { return EINVAL; }
afscall_aggr() { return EINVAL; } 
afscall_cm() { return EINVAL; }
afscall_fpkint() { return EINVAL; }
afscall_ktc() { return EINVAL; }
afscall_tkm_control()  { return EINVAL; }
afscall_exporter() { return EINVAL; }
afscall_cm_pioctl()  { return EINVAL; }
afscall_xcredkint()  { return EINVAL; }
afscall_vnode_ops()  { return EINVAL; }
afscall_volser() { return EINVAL; }
afscall_cm_getpag()  { return EINVAL; }
