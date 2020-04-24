# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
# Burlington, MA, USA
# All Rights Reserved
# (c) Copyright 1990, 1991, 1992
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.6.2  1996/03/09  23:30:54  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:46:09  marty]
#
# Revision 1.1.6.1  1995/12/07  21:56:10  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:03:21  root]
# 
# Revision 1.1.4.1  1994/06/10  20:52:44  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:49:42  devsrc]
# 
# 	[OT 5373]    SNI/SVR4 merge.
# 	[1992/09/17  20:40:02  sekhar]
# 
# Revision 1.1.2.2  1993/02/01  20:14:58  hinman
# 	[hinman] - SVR4 changes
# 	[1993/02/01  20:03:39  hinman]
# 
# 	[hinman] - Check in SNI revision 9.6.1.5
# 	[1993/01/08  14:09:41  hinman]
# 
# Revision 9.6.1.5  92/09/30  12:39:46  root
# 	Acceptance of OSF rev 1.1.3.2
# 
# Revision 9.12.1.2  92/09/30  10:31:35  hinman
# 	Auto checkin of OSF rev 1.1.3.2
# 
# $EndLog$
# 
#

#It is necessary to remove the #line statements from the file that comes 
#	from the compiler, which is why:
#	${SED} '/^\#/d'
#is invoked.
#	The command:
#	${SED} 's/% \([a-z]*\)/%\1/g'
#is used to work around a bug in the SVR4 preprocessor.  The bug in the 
#preprocessor is as follows:  
#The following lines:
#	#define STACK_ARG1 4
#	movl STACK_ARG1(%esp),%ebx
#are transformed into:
#	movl 4 ( % esp ) , % ebx
#which is not valid assembly language.
#The sed command transforms lines like:
#	movl 4 ( % esp ) , % ebx
#into:
#	 4 ( %esp ) , %ebp

cma_assem.o: cma_assem.s
	${RM} -f file1.s
	${_CC_} -E ${_CCFLAGS_}  ${_GENINC_} ${${TARGET_MACHINE}_AFLAGS} $> | ${SED} -e '/^\#/d' -e 's/% \([a-z]*\)/%\1/g' -e '//d' > file1.s
	${AS} -o cma_assem.o file1.s

