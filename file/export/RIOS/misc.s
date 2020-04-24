#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# HISTORY
# $Log: misc.s,v $
# Revision 1.1.10.1  1996/10/02  17:45:36  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:36:44  damon]
#
# Revision 1.1.5.1  1994/06/09  14:06:37  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:21:57  annie]
# 
# Revision 1.1.3.2  1993/01/21  19:33:00  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  19:46:39  zeliff]
# 
# Revision 1.1  1992/01/19  18:33:37  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $Header: /u0/rcs_trees/dce/rcs/file/export/RIOS/misc.s,v 1.1.10.1 1996/10/02 17:45:36 damon Exp $
# $Locker:  $
#
# misc.s -	miscellaneous assembler routines for the rs/6000
#

#
# get_toc -	return the current TOC
#
 	.csect .get_toc[PR]
	 .globl .get_toc[PR]
	mr	3,2		# get TOC
	br
        .align 2
	.tbtag 0x0,0xc,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0

 	.toc
	.csect  get_toc[DS]
	.globl  get_toc[DS]
	.long   .get_toc[PR]
	.long   TOC[t0]

#
# get_ret_addr -	return the caller's return address
#
	.csect .get_ret_addr[PR]
	.globl .get_ret_addr[PR]
	l	3, 0(1)		# caller's saved stack pointer
	l	3, 8(3)		# caller's saved link register
	br
	.align 2
	.tbtag 0x0,0xc,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
 	.toc
        .csect  get_ret_addr[DS]
        .globl  get_ret_addr[DS]
	.long   .get_ret_addr[PR]
        .long   TOC[t0]
