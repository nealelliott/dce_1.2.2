/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/syscall.h>

#ifdef	AFS_HPUX_ENV
extern int dfs_is_present();
#endif

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/sys/RCS/lclcalls.c,v 4.26 1996/02/22 18:29:11 cfe Exp $")

/* setpag() -- do the syscall locally */
int
setpag(void)
{
#ifdef	AFS_HPUX_ENV
    if (!dfs_is_present())
	return -1;			/* emulate stub in sec_login_pag.c */
#endif	/* AFS_HPUX_ENV */
    return afs_syscall(AFSCALL_SETPAG, 0, 0, 0, 0, 0);
}

int
resetpag(void)
{
#ifdef	AFS_HPUX_ENV
    if (!dfs_is_present())
	return -1;			/* emulate stub in sec_login_pag.c */
#endif	/* AFS_HPUX_ENV */
    return afs_syscall(AFSCALL_RESETPAG, 0, 0, 0, 0, 0);
}

/* newtgt() -- do the syscall locally */
int
newtgt(u_long pag, u_long time)
{
    int code;
#ifdef	AFS_HPUX_ENV
    if (!dfs_is_present())
	return 0;			/* emulate stub in sec_login_pag.c */
#endif	/* AFS_HPUX_ENV */
    code = afs_syscall(AFSCALL_NEWTGT, pag, time, 0, 0, 0);
    return (code < 0 ? errno : code);
}

/* pioctl() -- do the syscall locally */
int
pioctl(char *path, int cmd, char *cmarg, int follow)
{
    return afs_syscall(AFSCALL_PIOCTL, (int)path, cmd, (int)cmarg, follow, 0);
}
