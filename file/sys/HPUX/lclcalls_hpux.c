/* Copyright (c) 1994 Transarc Corp. - All rights reserved */

#include <dcedfs/param.h>
#include <signal.h>
#include <errno.h>
#include <dcedfs/syscall.h>
#include <dce/exc_handling.h>

int dfs_is_present();

/*
 * Main afs syscall entry point.
 */
afs_syscall(asyscall, parm1, parm2, parm3, parm4, parm5)
    int asyscall, parm1, parm2, parm3, parm4, parm5;
{
    volatile int errcode = -1;

    if (dfs_is_present()) {
	errno = 0;
	errcode = syscall(AFS_SYSCALL, asyscall,
			  parm1, parm2, parm3, parm4, parm5);
    } else
	errno = ENOSYS;

    return errcode;
}

/* dfs_is_present() -- make sure that DFS has kernel support */

/* LOGIC IS:
 *
 * if (DFS_SUPPORT is zero) then
 *	return 0 (forever)
 * else
 * 	if (KLOAD is in the kernel) then
 *		if (DFS is in the kernel) then
 *			return 1 (forever)
 *		else
 *			return 0 (this time)
 *		endif
 * 	else
 *		return 0 (forever)
 *	endif
 * endif
 *
 * "DFS_SUPPORT" is an external variable defined
 * in this module.  It is never referenced
 * outside this module, and it is initialized to
 * a non-zero value.  If kload is going to exist
 * in the kernel for other than DFS reasons, then
 * a system administrator can use ADB to set the
 * value to zero (in libdce.sl) to avoid the penalty
 * of the continual attempt to probe for DFS kernel
 * support that this combination would imply.
 * This is *gross* but it provides a hammer for those
 * who would use it.
 *
 * "forever" means that the code doesn't have
 * to try to probe again since the state is
 * unchangable (can't load DFS if no KLOAD and
 * can't unload DFS once loaded - at least for
 * UX9).
 */

#define	PROBE_UNKNOWN	0
#define	PROBE_NO_KLOAD	1
#define	PROBE_KLOAD	2
#define	PROBE_DFS	3

extern int DFS_SUPPORT = 1;
extern int DFS_FORCE = 0;

static int probe_for_kload();
static int probe_for_dfs();

int
dfs_is_present()
{
    static int probe_state = PROBE_UNKNOWN;

    if (DFS_FORCE)
	return(1);
    if (DFS_SUPPORT) {
	switch (probe_state) {
	  case PROBE_NO_KLOAD:
		/* no kernel support for KLOAD so none for DFS */
		/* so can't leave this state */
		break;
	  case PROBE_UNKNOWN:
		if (probe_for_kload()) {
		    probe_state = PROBE_KLOAD;
		}
		else {
		    probe_state = PROBE_NO_KLOAD;
		    break;
		}
		/* fall thru since found kload */
	  case PROBE_KLOAD:
		if (probe_for_dfs()) {
		    probe_state = PROBE_DFS;
		}
		else {
		    /* still not there, but no state change since */
		    /* must try again next time */
		}
		break;
	  case PROBE_DFS:
		/* found DFS and since can't unload... */
		/* then can't leave this state */
		break;
	  default:
		/* very bad to get here */
		abort();
	}
    }
    
    return(DFS_SUPPORT && (probe_state == PROBE_DFS));
}

/* probe_for_kload() - check for KLOAD support in the kernel
 * 
 * Rather than creating a new sub-function for kload, we'll
 * use the DEALLOC subcode but pass it a bad parameter.
 * This should cause it to return something other than ENOSYS
 * (and/or SIGSYS).
 *
 * ALERT: rather than include kload.h (which is hidden from
 * ODE export) I'll define DEALLOC here.  I'll also make sure
 * that a comment is added to kload.h to make sure that
 * DEALLOC isn't moved.
 */
#define KLOAD_DEALLOC   3

static int
probe_for_kload()
{
    long badpos = -1L;	/* this just can't exist, expect ENXIO */
    int errcode = -1;
    int prev_errno = errno;

    errno = 0;
    TRY
	errcode = syscall(AFS_SYSCALL, AFSCALL_KLOAD, KLOAD_DEALLOC,
			  badpos, 0, 0, 0);
    CATCH (exc_SIGSYS_e)
       errno = ENOSYS;
    ENDTRY
    if ((errcode < 0) && (errno == ENOSYS)) {
	errno = prev_errno;
	return 0;
    }
    else {
	errno = prev_errno;
	return 1;
    }
}

/* probe_for_dfs() - check for DFS support in the kernel
 *
 * Rather than creating a new sub-function for DFS, we'll
 * use the GETPAG subcode.  This should be harmless and
 * quick if it works.
 */
static int
probe_for_dfs()
{
    long pag;	/* if no PAG then ENOENT, otherwise PAG */
    int  errcode = -1;
    int prev_errno = errno;

    errno = 0;
    TRY
	errcode = syscall(AFS_SYSCALL, AFSCALL_GETPAG, &pag, 0, 0, 0, 0);
    CATCH (exc_SIGSYS_e)
       errno = ENOSYS;
    ENDTRY
    if ((errcode < 0) && (errno == ENOSYS)) {
	errno = prev_errno;
	return 0;
    }
    else {
	errno = prev_errno;
	return 1;
    }
}
