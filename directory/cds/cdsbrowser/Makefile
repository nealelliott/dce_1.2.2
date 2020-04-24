# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.12.2  1996/03/09  23:11:29  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:52  marty]
#
# Revision 1.2.12.1  1995/12/08  15:02:45  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:36:44  root]
# 
# Revision 1.2.9.2  1993/06/10  19:14:08  sommerfeld
# 	HP-UX cleanup: flush explicit include of /usr/include/X11
# 	[1993/06/08  18:00:15  sommerfeld]
# 
# Revision 1.2.5.2  1992/12/30  12:37:51  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  22:21:29  zeliff]
# 
# Revision 1.2.2.3  1992/05/29  20:34:25  marty
# 	Undo last submission.
# 	[1992/05/29  20:33:56  marty]
# 
# Revision 1.2.2.2  1992/05/29  15:42:55  marty
# 	Add -I to Motif header files.  At OSF we'll provide the header files
# 	in a separate backing tree referenced through the MOTIF_INFLAGS variable.
# 	Customers better have these header files sitting in /usr/include.
# 	[1992/05/29  15:42:35  marty]
# 
# Revision 1.2  1992/01/19  22:12:54  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
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
#
# Makefile for building the cdsbrowser
#

# programs to build
PROGRAMS	= cdsbrowser

# install directory
IDIR		= /bin/

# list of things to install
ILIST		= cdsbrowser

# list of libraries used to bind programs
LIBFLAGS	= ${${TARGET_MACHINE}_LIBFLAGS}

#LIBS		= -lMrm ${${TARGET_MACHINE}_XM_LIBS} \
#		  -lXm -lXt -lX11 ${${TARGET_MACHINE}_LIBS}
cdsbrowser_LIBS		= ${${TARGET_MACHINE}_LIBS}

# compiler flags
CFLAGS		= ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS        = -I../includes ${${TARGET_MACHINE}_INCFLAGS}

# List of objects to build cdsbrowser
cdsbrowser_OFILES = \
	cds_browser.o \
	cds_browsersource.o \
	cds.o \
	cds_display.o \
	cds_display_outline.o \
	cds_selections.o \
	cds_structure.o \
	pane.o

.include <${RULES_MK}>

