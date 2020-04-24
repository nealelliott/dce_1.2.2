#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.9.1  1996/10/02  17:54:08  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:42:00  damon]
#
# Revision 1.1.4.1  1994/06/09  14:13:00  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:26:17  annie]
# 
# Revision 1.1.2.2  1993/01/19  16:07:38  cjd
# 	embedded copyright notice
# 	[1993/01/19  15:11:02  cjd]
# 
# Revision 1.1  1992/01/19  02:59:16  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# libdcedfs Makefile
#

SCRIPTS	= makesyms 

libdcedfs.a: ${LIBS} makesyms
	rm -f *.o libdcedfs.syms
	for x in ${LIBS}; \
	do \
		./makesyms $$x >> libdcedfs.syms; \
		ar -t $$x > .xxx; \
		ar -xv $$x; \
		pre=`basename $$x|sed -e "s/^lib//" -e "s/$\\.a//"` ; \
		for y in `cat .xxx`; \
		do \
			mv $$y $${pre}_$$y; \
		done; \
		rm .xxx; \
	done;	
	${_LD_} -o shr.o *.o *.exp -bloadmap:shr.o.map -bM:sre \
	-bE:libdcedfs.syms -bI:/lib/syscalls.exp \
	${LIBDIRS} -T512 -H512 -lbsd -lc -ldce 2> shr.o.error
	ar crv libdcedfs.a shr.o
	rm -f *.o
