#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.6.2  1996/03/09  23:09:21  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:27:29  marty]
#
# Revision 1.1.6.1  1995/12/08  21:33:42  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  18:04:58  root]
# 
# Revision 1.1.4.2  1994/06/09  16:05:16  devsrc
# 	cr10892 - fix copyright
# 	[1994/06/09  15:49:59  devsrc]
# 
# Revision 1.1.4.1  1994/03/07  14:27:24  rousseau
# 	Changed TCL_LIBRARY path (CR 9952).
# 	[1994/03/07  14:24:10  rousseau]
# 
# Revision 1.1.2.3  1993/11/09  14:28:36  rousseau
# 	Added switches for more compiler warnings.
# 	[1993/11/09  14:28:30  rousseau]
# 
# Revision 1.1.2.2  1993/10/17  23:41:48  rousseau
# 	Added -DTCL_LIBRARY
# 	[1993/10/17  23:41:34  rousseau]
# 
# Revision 1.1.2.1  1993/10/14  14:34:08  rousseau
# 	Initial Version
# 	[1993/10/14  14:25:49  rousseau]
# 
# $EndLog$
#

# AT386 specific config options.

MACHINE_CFLAGS	= -pedantic -Wchar-subscripts -Wcomment -Wformat \
                -Wid-clash-32 -Wimplicit -Wmissing-prototypes \
		-Wswitch -Wreturn-type -Wstrict-prototypes \
		-Wtraditional -Wunused \
		-DHAVE_UNISTD_H=1 -DNO_UNION_WAIT=1 -DNEED_MATHERR=1 \
		-DTCL_LIBRARY=\"/opt/dcelocal/tcl\"

AT386_LIBS	 = -lm

