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
# Revision 1.1.10.3  1996/03/09  23:09:23  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:27:30  marty]
#
# Revision 1.1.10.2  1995/12/08  21:33:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  18:04:59  root]
# 
# Revision 1.1.6.1  1994/10/21  21:38:38  mori_m
# 	CR: 11259 Tcl I18N Work.  Added +e flag for compiling wchar_t type.
# 	[1994/10/21  21:26:34  mori_m]
# 
# Revision 1.1.4.2  1994/06/09  16:05:17  devsrc
# 	cr10892 - fix copyright
# 	[1994/06/09  15:50:00  devsrc]
# 
# Revision 1.1.4.1  1994/03/07  14:27:25  rousseau
# 	Changed TCL_LIBRARY path (CR 9952).
# 	[1994/03/07  14:24:21  rousseau]
# 
# Revision 1.1.2.3  1993/11/09  14:30:34  rousseau
# 	Added switches for more compiler warnings.
# 	[1993/11/09  14:30:25  rousseau]
# 
# Revision 1.1.2.2  1993/10/17  23:42:06  rousseau
# 	Added -DTCL_LIBRARY
# 	[1993/10/17  23:41:15  rousseau]
# 
# Revision 1.1.2.1  1993/10/14  15:26:40  rousseau
# 	Initial Version
# 	[1993/10/14  14:26:55  rousseau]
# 
# $EndLog$
#

# HP800 specific config options.

MACHINE_CFLAGS	= +w1 +e \
		-DNO_GETWD=1 -DHAVE_UNISTD_H=1 -DNO_UNION_WAIT=1 \
		-DNEED_MATHERR=1 -DTCL_LIBRARY=\"/opt/dcelocal/tcl\"

HP800_LIBS	 = -lm
