# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.13.2  1996/03/09  23:11:25  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:48  marty]
#
# Revision 1.2.13.1  1995/12/08  00:10:22  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:53:20  root]
# 
# Revision 1.2.10.2  1994/08/11  20:35:09  peckham
# 	Add casrv.o module.
# 	[1994/08/11  04:45:09  peckham]
# 
# Revision 1.2.10.1  1994/03/12  21:55:12  peckham
# 	DEC serviceability and i18n drop
# 	[1994/03/12  13:45:56  peckham]
# 
# Revision 1.2.8.2  1993/05/24  20:40:00  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:11:44  cjd]
# 
# Revision 1.2.6.2  1993/05/12  02:47:17  jd
# 	Initial 486 port.
# 	[1993/05/12  02:46:54  jd]
# 
# Revision 1.2.4.2  1992/12/30  12:35:47  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  22:18:30  zeliff]
# 
# Revision 1.2  1992/01/19  22:12:52  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# 
# 
#  OSF DCE Version 1.0 
#

# COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991. ALL RIGHTS RESERVED.
#
# THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
# ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
# INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
# COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
# OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
# TRANSFERRED.
#
# THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
# AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
# CORPORATION.
#
# DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
# SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
#
# Makefile to build cdscache library and tools
#

# dirs to search for source
VPATH		= ${TARGET_OS}:${TARGET_MACHINE}

# libraries and programs to build and install directory
LIBRARIES	= libcdscache.a
PROGRAMS	= cadump catraverse
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
EXPDIR		= /usr/lib/

# list of libraries to bind the programs
#
# Since we may be building using shared libraries (which means ${LIBS} gets
# tacked on to the compile lines; a bad thing when we are compiling the '.o''s
# for libcdscache!!! :-) ), we explicitly define the libs for the PROGRAMS
#
cadump_LIBS     = -lcdscache ${${TARGET_MACHINE}_LIBS}
catraverse_LIBS = -lcdscache ${${TARGET_MACHINE}_LIBS}
LIBS		= ${${TARGET_MACHINE}_LIBS}

# compiler flags
CFLAGS 		= ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS	= -I../includes -I../stubs

LDFLAGS		= -L. ${${TARGET_MACHINE}_LDFLAGS}

# list of objects to build tools
cadump_OFILES     = cadumpmain.o
catraverse_OFILES = catraverse.o

# list of objects for the libraries
libcdscache.a_OFILES = \
       caaccess.o caattributes.o cache.o cacle.o cadata.o cadir.o \
       cafreelist.o cagroup.o cahash.o cainsertatt.o calink.o caload.o \
       calock.o camiscl.o caname.o caremoveatt.o casrv.o casys.o \
       catime.o causer.o cadump.o casysinfo.o

.include <${RULES_MK}>
