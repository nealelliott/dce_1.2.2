#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/08/09  16:55:03  arvind
# 	DCE 1.2.2 Intermediate drop (post bl3/pre bl4)
# 	[1996/08/09  14:09:07  arvind]
#
# $EndLog$
#

# Tcl include files.  (If you haven't installed Tcl yet, read the README file).
# This must point to the directory that contains ALL of Tcl's include
# files, not just the public ones.
TCLHDIR = ../../tcl

# TCL library.  Very little actually comes out of it, but it is handy.
TCLLIB = ../../tcl/libtcl.a
# if installed, you can use:
# TCLLIB = -ltcl

CC = gcc
CPLUSPLUS = g++
CPLUSPLUSLIBDIR = -L/depot/gnu/arch/lib
CPLUSPLUSLIB = -lg++

CFLAGS = -g -I.. -I$(TCLHDIR)
LIBEXPECT = ../libexpect.a

LIBS = $(LIBEXPECT) $(TCLLIB) -lm

SCRIPTS = su2 noidle script.exp bonfield.exp

all: chesslib chesslib2 chesslib++

# this can be compiled with either cc or gcc
chesslib: chesslib.o $(LIBEXPECT)
	$(CC) -g -o chesslib chesslib.o $(LIBS)

# this can be compiled with either cc or gcc
chesslib2: chesslib2.o $(LIBEXPECT)
	$(CC) -g -o chesslib2 chesslib2.o $(LIBS)

# this is compiled with c++
chesslib++: chesslib++.o $(LIBEXPECT)
	$(CPLUSPLUS) -g -o chesslib++ chesslib++.o $(LIBS) \
		$(CPLUSPLUSLIBDIR) $(CPLUSPLUSLIB)

chesslib++.o: chesslib++.c
	$(CPLUSPLUS) -c $(CFLAGS) chesslib++.c

printvars: printvars.o
	$(CC) -o printvars printvars.o $(LIBS)

ftplib: ftplib.o
	$(CC) -g -o ftplib ftplib.o $(LIBS)

match_max: match_max.o
	$(CC) -g -o match_max match_max.o $(LIBS)

jaj1: jaj1.o
	$(CC) -g -o jaj1 jaj1.o $(LIBS)

jaj2: jaj2.o
	$(CC) -g -o jaj2 jaj2.o $(LIBS)

# wrap up password-generation examples
passgen:
	shar passgen.README tkpasswd mkpasswd mkpasswd.man > /tmp/passgen

cleanup:
	rm -f expect devtty exho dumb test.raw test.results test.tmp

# copy some contributed scripts over to public-accessible directory
SCRIPTDIR = ~ftp/pub/expect/scripts
ftp:
	rcp README.scripts durer:$(SCRIPTDIR)/README
	rcp $(SCRIPTS) durer:$(SCRIPTDIR)
	rsh durer ls -l $(SCRIPTDIR)
