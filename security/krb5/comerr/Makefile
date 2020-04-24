# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.9.2  1996/03/09  23:29:32  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:44:53  marty]
#
# Revision 1.1.9.1  1995/12/08  17:39:20  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:56:29  root]
# 
# Revision 1.1.7.3  1992/12/29  13:55:24  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:50:20  zeliff]
# 
# Revision 1.1.7.2  1992/11/06  21:26:17  john
# 	Testing removal of error_table.h:error_table.h dependency.
# 	[1992/11/04  16:26:42  john]
# 
# 	Added dependency for .h to .y to use the command ture.
# 	[1992/11/04  14:27:22  john]
# 
# 	Moved the location of the NO_Y_H_RULE to before .include <osf.rules.mk>.
# 	[1992/11/03  15:46:12  john]
# 
# 	Added NO_Y_H_RULE to ignore the .y.h: default rule.
# 	[1992/11/03  15:31:15  john]
# 
# Revision 1.1.3.5  1992/06/08  12:55:58  john
# 	The LIBS change can only be used when building libcom_err.a.
# 	It will cause compile_et to fail.
# 	[1992/06/08  12:55:35  john]
# 
# Revision 1.1.3.4  1992/06/05  20:59:05  john
# 	Added -ldce to the LIBS line to resolve linker glue warnings when
# 	linking some file stuff.
# 	[1992/06/05  20:58:49  john]
# 
# Revision 1.1.3.3  1992/05/04  20:21:07  sekhar
# 	Fixed a minor typo : changed libcom_ero.EXPDIR to libcom_err.EXPDIR.
# 	This was exporting libcom_err.a to /usr/include instead of /usr/lib
# 	on RIOS.
# 	[1992/05/04  20:15:45  sekhar]
# 
# Revision 1.1.3.2  1992/04/30  21:19:11  john
# 	Added SETUP_PROGRAMS.
# 	[1992/04/10  14:43:20  john]
# 
# Revision 1.1  1992/01/19  14:53:50  devrcs
# 	Initial revision
# 
# $EndLog$
#

#   Makefile.ODE V=2 10/21/91 //littl/prgy/krb5/comerr
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /krb5/comerr Makefile
#

# Compiler flags for this directory
CFLAGS			= -DOSF_DCE 

# Library to build
LIBRARIES		= libcom_err.a

# Program to build
PROGRAMS		= compile_et

# Program to be built in the setup pass
SETUP_PROGRAMS		= compile_et

libcom_err.a_EXPDIR	= /usr/lib/
EXPDIR			= /usr/include/
compile_et_EXPDIR	= /usr/bin/

EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}

INCLUDES		= 	com_err.h     

libcom_err.a_OFILES	= 	com_err.o       \
				error_message.o \
				et_name.o       \
				init_et.o 

compile_et_OFILES	= 	compile_et.o \
				error_table.o

# Libraries to link the program with
LIBS			= -ll
libcom_err.a_LIBS	= -ll -ldce

# set this to turn off the default .y.h: rules
NO_Y_H_RULE=ON

.include <${RULES_MK}>

#error_table.h: error_table.y
#	@true

error_table.o: et_lex.lex.c ${INCLUDES}

