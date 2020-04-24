# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/09  23:09:27  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:27:32  marty]
#
# Revision 1.1.4.1  1995/12/08  21:33:49  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  18:05:04  root]
# 
# Revision 1.1.2.3  1994/06/09  16:05:20  devsrc
# 	cr10892 - fix copyright
# 	[1994/06/09  15:50:04  devsrc]
# 
# Revision 1.1.2.2  1994/02/18  19:41:22  ohara
# 	change the default location of init file
# 	[1994/02/18  15:49:54  ohara]
# 
# Revision 1.1.2.1  1993/12/20  19:05:07  rousseau
# 	Initial version.
# 	[1993/12/20  19:05:02  rousseau]
# 
# $EndLog$

# SVR4 specific config options.
# NOTE: This is blatently untested due to the lack of a build machine.

MACHINE_CFLAGS	= -DHAVE_UNISTD_H=1 -DNO_UNION_WAIT=1 -DNEED_MATHERR=1 \
                -DTCL_LIBRARY=\"/opt/dcelocal/tcl\"

RIOS_LIBS       = 
