# ______________________________________________________________________
# Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
# All rights reserved.
# ______________________________________________________________________
# $Log: Makefile,v $
# Revision 1.5  1996/10/15  01:56:29  bowe
# Added revision log section.
#
# Revision 1.4  1996/10/03 19:19:53  bowe
# Removed debug.o
#
# Revision 1.3  1996/10/03  18:44:20  bowe
# Rework to get sams-generated files from libmos.
#
# Revision 1.2  1996/10/01 14:04:59  bowe
# Neaten up idl-generate section.
#
# Revision 1.1  1996/09/27 17:57:38  bowe
# Initial revision
#
# $EndLog$
# ________________________________________________________________________

IDLDIR	= ../idl
IDL	= idl -I$(IDLDIR)
SAMS	= sams

# Something like this with gcc
# make CC=gcc CFLAGS="-g -O -fno-builtin -Wall -I. -I../idl"

# These are platform/compiler dependent
# HPUX needs -lc_r (for re-entrant libc)
EXTRALIB	= -lc_r
CC	= c89
CFLAGS	= -g -D_HPUX_SOURCE -I. -I$(IDLDIR)
LDFLAGS	= -g -z -Wl,-Bimmediate,-Bnonfatal,-a,default,+b,/lib:/usr/lib

BINDIR	= /opt/dcelocal/bin

LIB	= -L../api -lmos -ldce $(EXTRALIB)

STUBSRC	= mos_q_sstub.c mos_mgmt_sstub.c
STUBOBJ	= $(STUBSRC:.c=.o)
IDLSRC	= mos_mgmt.acf mos_q.acf

GENHDR	= $(STUBHDR)

SRC	= main.c acl.c ops_q.c ops_m.c q_utils.c utils.c svc.c lock.c
INCLUDE = mosdefs.h lock.h
QMGROBJ	= $(STUBOBJ) $(SRC:.c=.o)
PROGS	= qmgr

# ----------------------------------------------------------------

all: $(PROGS)

install:
	cp -f $(PROGS) $(BINDIR)

qmgr: $(QMGROBJ)
	$(CC) $(LDFLAGS) -o $@ $(QMGROBJ) $(LIB)

# ----------------------------------------------------------------
# idl-generated stuff
# idl makes all this less convenient than it should be.
HSTUB_FLAGS	= -header -client none -server none -no_mepv
SSTUB_FLAGS	= -keep c_source -client none -header none -no_mepv
CSTUB_FLAGS	= -no_mepv -server none -keep c_source
TMP		= /tmp/

mos_q_sstub.c: $(IDLDIR)/mos_q.idl
	$(IDL) $(SSTUB_FLAGS) $?
mos_mgmt_sstub.c: $(IDLDIR)/mos_mgmt.idl
	$(IDL) $(SSTUB_FLAGS) $?

# ----------------------------------------------------------------
clean:
	rm -f *.o $(PROGS) $(STUBSRC) $(STUBHDR) core


# dependencies
main.o: main.c mosdefs.h lock.h $(GENHDR)
utils.o: utils.c mosdefs.h lock.h $(GENHDR)
acl.o: acl.c mosdefs.h lock.h $(GENHDR)
q_util.o: q_utils.c mosdefs.h lock.h $(GENHDR)
ops_q.o: ops_q.c lock.h $(GENHDR)
ops_m.o: ops_m.c lock.h $(GENHDR)
lock.o: lock.c lock.h mosdefs.h

