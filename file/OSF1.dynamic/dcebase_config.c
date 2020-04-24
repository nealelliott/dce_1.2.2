/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dcebase_config.c,v $
 * Revision 1.1.11.1  1996/10/02  16:56:43  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:17  damon]
 *
 * Revision 1.1.6.1  1994/06/09  13:49:33  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:02  annie]
 * 
 * Revision 1.1.4.2  1993/01/18  19:48:29  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:21:35  cjd]
 * 
 * Revision 1.1.2.3  1992/04/02  17:58:01  garyf
 * 	fix conditional
 * 	[1992/04/02  17:12:33  garyf]
 * 
 * 	Merging dfs5.11 to dfs6.3
 * 
 * Revision 1.1.2.2  1992/01/24  01:47:22  devsrc
 * 	Bring in dfs6.3 sources
 * 	[1992/01/22  19:28:36  devsrc]
 * 
 * Revision 1.1.2.2  91/11/20  17:38:23  garyf
 * 	Initial coding.
 * 	[91/11/20  17:35:25  garyf]
 * 
 * $EndLog$
*/
/*
 * Configure the 'dcebase' kernel extension.
 *
 * Right now, this involves init'ing any function
 * pointers stored in the kernel to point out into
 * this extension.  This includes the pointer to
 * AFS Syscall in the sysent table, and a miscellaneous
 * EPV used by UFS and VFS to call into xvnode.
 * 
 */
#include <dce_afs.h>
#include <sys/sysconfig.h>
#include <sys/systm.h>
#include <sys/errno.h>
#undef DCE_AFS_DYNAMIC /* do this to obtain SYS_afs_syscall */
#include <sys/syscall.h>

/* shared between CM and EPISODE */
long cold_shutdown = 0;			/* Shutdown related */

/* 
 * Declare all the system calls we'll need to hook up. 
 */
extern int
	afs_syscall(),    nosys();

/*
 * Structure of the system-entry table
 */
extern int nsysent;
	

/*
 * EPV used by UFS and VFS to call into xvnode layer.
 */
extern int afs_readop(), afs_writeop();
extern struct afsglueops_t {
	int	(*op_read)();
	int	(*op_write)();
} afsglueops;


dcebase_config (op, indata, indatalen, outdata, outdatalen)
  sysconfig_op_t    op;
  void              *indata;
  size_t            indatalen;
  void              *outdata;  
  size_t            outdatalen;
{

#ifdef AFS_OSF11_ENV
	struct sysent afscall_entry = {
	6 /* number of args */,
	0 /* parallel? */ ,
	nosys }
#endif /* AFS_OSF11_ENV */
	/*
	 * Install the afs syscall into the syscall table.
	 */

	if (SYS_afs_syscall >= nsysent)
		return (EINVAL);
	
#define afson (op == SYSCONFIG_CONFIGURE)

#ifdef AFS_OSF11_ENV
#define SystemCall(index, entry) syscall_add(index, name, entry, proc)

	SystemCall (SYS_afs_syscall,    "afs_syscall", afscall_entry, afson ? afs_syscall    : nosys);
#else
#define SystemCall(index,proc) sysent[(index)].sy_call = (proc)

	SystemCall (SYS_afs_syscall,    afson ? afs_syscall    : nosys);
#endif /* AFS_OSF11_ENV */

	/*
	 * Initialize the function ptrs used by UFS and VFS
	 * to call into the xvnode layer.
	 */
	afsglueops.op_read  = afson ? afs_readop : 0;
	afsglueops.op_write = afson ? afs_writeop : 0;
	
	return 0;

}
