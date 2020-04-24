# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.7.2  1996/03/11  13:25:38  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:13:47  marty]
#
# Revision 1.1.7.1  1995/12/08  17:53:16  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:27:36  root]
# 
# Revision 1.1.5.2  1992/12/30  17:17:23  zeliff
# 	Embedding copyright notice
# 	[1992/12/30  17:15:26  zeliff]
# 
# Revision 1.1.2.2  1992/01/22  23:03:59  melman
# 	Adding changes from defect_fixes archive.
# 	[1992/01/22  22:20:33  melman]
# 
# $EndLog$
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
	${LIBDIRS} -T512 -H512 -lkrb5 -lbsd -lc -ldce 2> shr.o.error
	ar crv libdcedfs.a shr.o
	rm -f *.o
