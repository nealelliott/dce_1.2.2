/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bomb_syscall.c,v $
 * Revision 1.1.24.1  1996/10/02  17:03:40  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:43  damon]
 *
 * Revision 1.1.18.3  1994/07/13  22:19:13  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  17:55:05  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:53:49  mbs]
 * 
 * Revision 1.1.18.2  1994/06/09  13:51:50  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:26  annie]
 * 
 * Revision 1.1.18.1  1994/02/04  20:06:06  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:34  devsrc]
 * 
 * Revision 1.1.16.1  1993/12/07  17:12:50  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  20:07:19  jaffe]
 * 
 * $EndLog$
 */

/*
 * Copyright (C) 1993 Transarc Corporation - All rights reserved
 */

#ifndef KERNEL
#include <pthread.h>
#endif

#include <dcedfs/osi.h>
#include <dcedfs/syscall.h>

#include <bomb.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bomb/bomb_syscall.c,v 1.1.24.1 1996/10/02 17:03:40 damon Exp $")


#ifdef KERNEL

extern int
afscall_bomb(long opcode, long p1, long p2, long p3, long p4, long* retval)
{
    int		code = 0;
    bombPoint_t	bp;
    char	bpName[BOMB_MAX_NAME+1];
    unsigned	bpNameLen;

    /* Make sure we're running as root */
    if (!osi_suser(osi_getucred()))
        return EACCES;

    switch (opcode) {
      case BOMB_OP_SET:
	code = osi_copyinstr((char*)p1, bpName, sizeof bpName, &bpNameLen);
	if (code)
	    return code;
	
	code = osi_copyin((caddr_t)p2, (caddr_t)&bp, sizeof bp);
	if (code)
	    return code;

	code = bomb_Set(bpName, &bp);
	switch (code) {
	  case 0:
	    break;

	  case BOMB_E_MALFORMED_BOMB_POINT:
	    code = ENOEXEC;
	    break;

	  case BOMB_E_NAME_TOO_LONG:
	    code = E2BIG;
	    break;

	  case BOMB_E_BAD_EXPLOSION_TYPE:
	    code = EINVAL;
	    break;

	  case BOMB_E_BAD_TRIGGER_TYPE:
	    code = ENOENT;
	    break;

	  default:
	    code = EIO;
	    break;
	}

	break;

      case BOMB_OP_UNSET:
	code = osi_copyinstr((char*)p1, bpName, sizeof bpName, &bpNameLen);
	if (code)
	    return code;
	
	code = bomb_Unset(bpName);
	if (code == BOMB_E_BOMB_POINT_NOT_SET)
	    code = ENOENT;
	else if (code)
	    code = EIO;
	break;

      case BOMB_OP_TEST:
	if (code = BOMB_EXEC("afscall_bomb#1", 0))
	    return code;

	BOMB_IF("afscall_bomb#2") {
	    (void)printf("The bomb at afscall_bomb#2 has exploded\n");
	}

	BOMB_POINT("afscall_bomb#3");

	BOMB_RETURN("afscall_bomb#4");
	break;

      default:
	code = EINVAL;
	break;
    }

    return code;
}	/* afscall_bomb */

#else	/* KERNEL */

extern long
bomb_Syscall(long opcode, long p1, long p2, long p3, long p4)
{
    long code;

    code = afs_syscall(AFSCALL_BOMB, opcode, p1, p2, p3, p4);
    if (code == -1) {
	/* Convert 8 bit errnos to 32 bit codes if possible */
	code = errno;

	switch (opcode) {
	  case BOMB_OP_SET:
	    switch (errno) {
	      case ENOEXEC:
		code = BOMB_E_MALFORMED_BOMB_POINT;
		break;

	      case E2BIG:
		code = BOMB_E_NAME_TOO_LONG;
		break;

	      case EINVAL:
		code = BOMB_E_BAD_EXPLOSION_TYPE;
		break;

	      case ENOENT:
		code = BOMB_E_BAD_TRIGGER_TYPE;
		break;
	    }

	  case BOMB_OP_UNSET:
	    if (errno == ENOENT)
		code = BOMB_E_BOMB_POINT_NOT_SET;
	    break;
	}
    }

    return code;
}	/* bomb_Syscall */


extern long
bomb_KernelSet(char* bpName, bombPoint_t* bpP)
{
    return bomb_Syscall(BOMB_OP_SET, (long)bpName, (long)bpP, 0, 0);
}	/* bomb_KernelSet */


extern long
bomb_KernelSetDesc(char* bpDesc)
{
    bombPoint_t	bp;
    char	bpName[BOMB_MAX_NAME + 1];
    long	code;

    if (code = bomb_ParseDesc(bpDesc, bpName, &bp))
	return code;

    return bomb_KernelSet(bpName, &bp);
}	/* bomb_KerneSetDesc() */


extern long
bomb_KernelUnset(char* bpName)
{
    return bomb_Syscall(BOMB_OP_UNSET, (long)bpName, 0, 0, 0);
}	/* bomb_KernelUnset */

#endif	/* KERNEL */
