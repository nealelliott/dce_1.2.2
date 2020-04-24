#   
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#  
# All Rights Reserved
#    
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/10/03  20:29:50  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  19:24:55  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/09/09  21:27 UTC  arvind
# 	PKSS drop from DEC (DCE1.2.2)
# 	[1996/08/30  16:17 UTC  arvind  /main/arvind_pkss/1]
# 
# $EndLog$
#
# (c) Copyright 1996,
# Digital Equipment Corporation, Maynard, Massachusetts, USA
# All Rights Reserved
#
# DCE Security Services 
# pkc/h Makefile
#

BSAFE_INCDIR = /usr/local/rsadsi/include

# Files to export and their locations


EXPDIR = /usr/include/dce/

# Files to install and their locations

IDIR    = /share/include/dce/

#CFLAGS+= -E

#CFLAGS+= -DNOENCRYPT_PIPE -DDEBUG
#CFLAGS+= -DDEBUG

INCFLAGS = -I.			\
	   -I..			\
	   -I../../../psm/h	\
	   -I../../../h		\
	   -I../server		\
	   -I../stubs		\
	   -I./$(TARGET_MACHINE)	\
	   -I../../../../../../src/directory/cds/gloria/gdb

OBJECTS = pkss_pipe_format.o	\
	  pkss_pipe_client.o	\
	  pkss_pipe_server.o

pkss_pipe_format.o : pkss_pipe_format.cxx \
                     pkss_pipe_format.h 

pkss_pipe_client.o : pkss_pipe_client.cxx \
                     pkss_pipe_format.h \
                     pkss_pipe_messages.h

pkss_pipe_server.o : pkss_pipe_server.cxx \
                     pkss_pipe_format.h \
                     pkss_pipe_messages.h

test = pkss_pipe_test pkss_pipe_test_dummy

dcepksmsg.o : ../server/dcepksmsg.c

dcepkssvc.o : ../server/dcepkssvc.c

pkss_pipe_test : pkss_pipe_test.o \
                 pkss_pipe_client.o \
                 pkss_pipe_format.o \
                 dcepksmsg.o \
                 dcepkssvc.o
	xlC ${_LDFLAGS_} -o pkss_pipe_test \
                                pkss_pipe_test.o \
                                pkss_pipe_client.o \
                                pkss_pipe_format.o \
                                dcepksmsg.o \
                                dcepkssvc.o \
                                ../../../psm/sec_bsafe.o \
                                -ldce

pkss_pipe_test_dummy : pkss_pipe_test_dummy.o \
                       pkss_pipe_server.o \
                       pkss_pipe_format.o \
                       dcepksmsg.o \
                       dcepkssvc.o
	xlC ${_LDFLAGS_} -o pkss_pipe_test_dummy \
                                pkss_pipe_test_dummy.o \
                                pkss_pipe_server.o \
                                pkss_pipe_format.o\
                                dcepksmsg.o \
                                dcepkssvc.o \
                                ../../../psm/sec_bsafe.o \
                                -ldce

VPATH=../server:../../../directory/cds/library:./$(TARGET_MACHINE)


.include <${RULES_MK}>

