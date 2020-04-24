/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dce_exc_context.s,v $
 * Revision 1.1.4.2  1996/03/09  23:26:09  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:41:46  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:14:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:49  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:35:10  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:50:41  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:37:02  zeliff
 * 	Embedding copyright notice.
 * 	[1992/12/23  15:20:40  zeliff]
 * 
 * Revision 1.1  1992/01/19  22:13:31  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/* 
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/*
 *  OSF DCE Version 1.0 
 */

/* 
 *	File: dce_exp_context.s
 *
 *	setjmp and longjmp primitives required by the kernel RPC
 *	for implementing the DCE Exceptions (CMA Exceptions) package.
 *
 */

/*
 * Mach Operating System
 * Copyright (c) 1989 Encore Computer Corporation
 */

#include <mmax_xpc.h>
#include <mmax_apc.h>
#include <mmax_dpc.h>
#include <mmax_idebug.h>
#include <cpus.h>
#include <mmax/mlmacros.h>

/*
 * dce_exc_setjmp (address) - char	*address
 *	Stores information at address to enable a "return" to the current
 *	user PC.
 */

ENTRY(dce_exc_setjmp)
				# r0 has the address of the data storage area.

	movd    r3,0(r0)	#"Push" general registers.
	movd    r4,4(r0)
	movd    r5,8(r0)
	movd    r6,12(r0)
	movd    r7,16(r0)
	sprd    fp,20(r0)	#"Push" FP
	sprd    sp,24(r0)	#"Push" user SP
	sprw    upsr,30(r0)	#"Push" user PSR (condition codes)
	movd    0(sp),32(r0)	#"Push" PC
	movqd   $0,r0		#Return 0 to 'parent'.
	ret     $0


/*
 # dce_exc_longjmp (address, val) - char	*address
 #			   int	val
 #	Reads the information from address and simulates a return to the
 #	stored PC.  Uses rett to restore the PSR on return.  Returns val
 #	if non-zero, else returns 1.
 */

ENTRY(dce_exc_longjmp)
				# R0 has the addr of the stored return values.
				# R1 has the return value

	movd    0(r0),r3	#Restore the general registers.
	movd    4(r0),r4
	movd    8(r0),r5
	movd    12(r0),r6
	movd    16(r0),r7
	lprd    fp,20(r0)	# Restore FP
	lprw    upsr,30(r0)	# Restore user PSR (condition codes)
	movd    32(r0),r2	# Restore PC
	lprd    sp,24(r0)	# Restore user SP
	movd	tos, r0		# (skip over where return address would go)
	movd	r1,r0		# Setup return value
	cmpqd	$0,r0		# Guarantee that r0 != 0
	bne	longret
	movqd	$1,r0
longret:
	jump	0(r2)		# Return to 'child' (r0 = (val) ? val : 1).


