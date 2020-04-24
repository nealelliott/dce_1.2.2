#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.11.1  1996/10/02  20:59:50  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:47:53  damon]
#
# Revision 1.1.6.1  1994/06/09  14:20:06  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:31:46  annie]
# 
# Revision 1.1.4.2  1993/01/21  15:20:22  cjd
# 	embedded copyright notice
# 	[1993/01/20  15:59:18  cjd]
# 
# Revision 1.1.2.2  1992/03/20  21:53:34  mhickey
# 	For reasons that escape my feeble brain, AIX will not
# 	allow you to un-archive a file that doesn't have write
# 	permission, despite the fact that it has your uid on it,
# 	you own the destination directory.  To accomodate this
# 	useful bit of functionality, we most chmod the copied
# 	file dfsloadx.exp to 644 before archiving so that it
# 	can be unarchived for the bulid of libdcedfs.
# 
# 	Defect 2068.
# 	[1992/03/20  21:40:55  mhickey]
# 
# Revision 1.1  1992/01/19  02:56:10  devrcs
# 	Initial revision
# 
# $EndLog$
#

VPATH			= ${TARGET_MACHINE}:../libafs/${TARGET_MACHINE}
OTHERS			= dfsloadobj dfsload.exp afs.exp
OFILES			+= lclcalls_aix.o dfsloadx.exp dfsload.o

dfsload.o_CFLAGS	= ${USERCFLAGS}
dfsloadobj_LDFLAGS	= -edfsloadobj -bexport:dfsload.exp -bI:afs.exp

ILIST			= dfsloadobj
IDIR			= /ext/

dfsloadobj: dfsload.exp afs.exp dfsloadobj.o
	-$(CP) ${dfsload.exp:P} .
	-$(CP) ${afs.exp:P} .
	${_LD_} -o $@ ${$@_LDFLAGS} $@.o
	${RM} -f dfsload.exp
	${RM} -f dfs.exp

# until there's a better way XXX
dfsloadx.exp: dfsload.exp
	-${CP} ${${.ALLSRC}:P} ${.TARGET}
	${CHMOD} 644 ${.TARGET}
