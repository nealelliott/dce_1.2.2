# ______________________________________________________________________
# Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
# All rights reserved.
# ______________________________________________________________________
#
# $Log: Makefile,v $
# Revision 1.3  1996/12/04  15:18:41  bowe
# Add sams-generated stuff to make clean.
#
# Revision 1.2  1996/12/04  15:16:11  bowe
# Add dependency for dcetclmsg.h (so that sams will generate it).
#
# Revision 1.1  1996/11/22  18:14:23  salamone
# Initial version
#
# $EndLog$
# ________________________________________________________________________

CC	= c89
SAMS	= sams

# Something like this with gcc
# make CC=gcc CFLAGS="-g -O -fno-builtin -Wall -I. -I../idl"

# These are platform/compiler dependent
# HPUX needs -lc_r (for re-entrant libc)
EXTRALIB	= -lc_r
MACHINE_CFLAGS  = -D_HPUX_SOURCE \
		  -DNO_GETWD=1 -DHAVE_UNISTD_H=1 -DNO_UNION_WAIT=1 \
		  -DNEED_MATHERR=1 -DTCL_LIBRARY=\"/opt/dcelocal/tcl\"
CFLAGS	= -g ${MACHINE_CFLAGS} -I. 
LDFLAGS	= -g -z -Wl,-Bimmediate,-Bnonfatal,-a,default,+b,/lib:/usr/lib

BINDIR	= /opt/dcelocal/bin
LIBDIR	= /opt/dcelocal/lib
INCDIR	= /opt/dcelocal/share/include/dce

LIBS	= libtcl.a

SAMSSRC = dcemossvc.c dcemosmsg.c
SAMSGEN = dcetcl.cat dcetcl.msg
#SAMSOBJ = $(SAMSSRC:.c=.o)
SAMSHDR = dcemcpsvc.h dcemcpmac.h dcemcpmsg.h
GENHDR	= $(SAMSHDR) 

SRC	= panic.c \
	  regexp.c \
	  tclAsync.c \
	  tclBasic.c \
	  tclCkalloc.c \
	  tclCmdAH.c \
	  tclCmdIL.c \
	  tclCmdMZ.c \
	  tclEnv.c \
	  tclExpr.c \
	  tclGet.c \
	  tclGlob.c \
	  tclHash.c \
	  tclHistory.c \
	  tclLink.c \
	  tclMain.c \
	  tclMtherr.c \
	  tclParse.c \
	  tclProc.c \
	  tclUnixAZ.c \
	  tclUnixStr.c \
	  tclUnixUtil.c \
	  tclUtil.c \
	  tclVar.c \
	  tclWCmdAH.c \
	  tclWCmdIL.c \
	  tclWCmdMZ.c \
	  tclWParse.c \
	  tclWUtil.c \
	  dcetclmsg.c
LIBOBJ	= $(SAMSOBJ) $(SRC:.c=.o)

# ----------------------------------------------------------------

all: $(LIBS)

install:
	cp -f $(LIBS) $(LIBDIR)
	cp -f $(GENHDR) $(INCDIR)

libtcl.a: $(LIBOBJ) dcetcl.cat
	ar rv $@ $(LIBOBJ)

regexp.o: dcetclmsg.h

# ----------------------------------------------------------------
# sams-generated stuff
dcetclmsg.o:    dcetclmsg.c
dcetclmsg.h:    tcl.sams
	${SAMS} -oh tcl.sams
dcetclmsg.c:    tcl.sams
	${SAMS} -ot tcl.sams
dcetcl.msg:     tcl.sams
	${SAMS} -om tcl.sams
dcetcl.cat:     dcetcl.msg
	gencat dcetcl.cat dcetcl.msg

# ----------------------------------------------------------------
clean:
	rm -f *.o $(LIBS) $(LIBOBJ) $(SAMSHDR) $(SAMSSRC) $(SAMSGEN) core

