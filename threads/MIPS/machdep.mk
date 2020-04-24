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
# Revision 1.1.6.2  1996/03/09  23:30:34  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:52  marty]
#
# Revision 1.1.6.1  1995/12/07  21:49:15  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:00:19  root]
# 
# Revision 1.1.4.1  1994/06/09  13:35:12  devsrc
# 	 CR10892 - fix copyright in file
# 	[1994/06/09  13:25:55  devsrc]
# 
# Revision 1.1.2.2  1992/12/15  22:22:40  alan
# 	Insert copyright notices
# 	[1992/12/07  16:01:15  alan]
# 
# Revision 1.1  1992/01/19  14:38:36  devrcs
# 	Initial revision
# 
# $EndLog$
#
#       src/threads/MIPS/machdep.mk, , dce_osf, dce.75d     (ULTRIX/OSF)    11/19/91
#
# 

cma_assem.o :	cma_assem.s
	as ${AFLAGS} -o cma_assem.o `genpath -I${TARGET_MACHINE} -ICOMMON` $>
#  DEC/CMS REPLACEMENT HISTORY, Element MACHDEP.MK
#  *14   14-OCT-1991 13:34:21 BUTENHOF "Fix conditionals for assem build"
#  *13   24-SEP-1991 16:25:01 BUTENHOF "IBM reverse drop"
#  *12    3-SEP-1991 10:29:00 BUTENHOF "Try to make it work with new ODE"
#  *11   17-JUN-1991 10:51:08 BUTENHOF "Fix for new ODE 2.0.2"
#  *10   10-JUN-1991 18:09:30 SCALES "Add sccs headers for Ultrix"
#  *9     2-MAY-1991 14:01:51 BUTENHOF "Improve for Tin BL3"
#  *8     3-APR-1991 15:59:45 BUTENHOF "Modify to support generic OSF/1 build"
#  *7     2-JAN-1991 22:49:30 BUTENHOF "Fiddle about"
#  *6    28-DEC-1990 01:14:18 BUTENHOF "Convert to stream format for ULTRIX build"
#  *5    28-DEC-1990 01:12:22 BUTENHOF ""
#  *4    18-DEC-1990 22:50:46 BUTENHOF "Convert to stream format for ULTRIX build"
#  *3    18-DEC-1990 22:39:30 BUTENHOF "Make changes"
#  *2    18-DEC-1990 15:11:52 BUTENHOF "Convert to stream format for ULTRIX build"
#  *1    18-DEC-1990 01:24:13 BUTENHOF "MIPS platform-specific Makefile"
#  DEC/CMS REPLACEMENT HISTORY, Element MACHDEP.MK
