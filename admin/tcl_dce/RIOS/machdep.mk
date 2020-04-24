# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/09  23:09:25  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:27:32  marty]
#
# Revision 1.1.4.1  1995/12/08  21:33:47  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  18:05:03  root]
# 
# Revision 1.1.2.4  1994/06/09  16:05:19  devsrc
# 	cr10892 - fix copyright
# 	[1994/06/09  15:50:03  devsrc]
# 
# Revision 1.1.2.3  1994/03/07  14:27:26  rousseau
# 	Changed TCL_LIBRARY path (CR 9952).
# 	[1994/03/07  14:24:29  rousseau]
# 
# Revision 1.1.2.2  1994/02/02  14:42:53  rousseau
# 	Add -lm to RIOS_LIBS
# 	[1994/02/02  14:42:30  rousseau]
# 
# Revision 1.1.2.1  1993/12/20  19:05:22  rousseau
# 	Initial version.
# 	[1993/12/20  19:05:18  rousseau]
# 
# $EndLog$

# RIOS specific config options.
# NOTE: This is blatently untested due to the lack of a build machine.

MACHINE_CFLAGS	= -DHAVE_UNISTD_H=1 -DNO_UNION_WAIT=1 -DNEED_MATHERR=1 \
                -DTCL_LIBRARY=\"/opt/dcelocal/tcl\"

RIOS_LIBS       = -lm
