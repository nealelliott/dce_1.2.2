# ______________________________________________________________________
# Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
# All rights reserved.
# ______________________________________________________________________
#
# $Log: Makefile,v $
# Revision 1.5  1996/11/13  17:19:51  bowe
# Add provisions for mos_ack.*
#
# Revision 1.4  1996/10/15  01:48:22  bowe
# Remove unused cruft/comments.  Added revision log section.
#
# Revision 1.3	1996/10/14  17:30:43  salamone
# Added MOS_API source files
#
# Revision 1.2  1996/10/03  19:19:35  bowe
# Added qdebug.c
#
# Revision 1.1  1996/09/12  20:00:27  bowe
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
LIBDIR	= /opt/dcelocal/lib
INCDIR	= /opt/dcelocal/share/include/dce

LIBS	= libmos.a

STUBSRC	= mos_q_cstub.c mos_mgmt_cstub.c mos_convert_cstub.c mos_ack_cstub.c
STUBHDR	= mosttypes.h mos_base.h mos_mgmt.h mos_q.h mos_convert.h mos_ack.h
STUBOBJ	= $(STUBSRC:.c=.o)
IDLSRC	= mos_mgmt.acf mos_q.acf mos_convert.acf mos_ack.acf

SAMSSRC = dcemossvc.c dcemosmsg.c
SAMSOBJ = $(SAMSSRC:.c=.o)
SAMSHDR = dcemossvc.h dcemosmac.h dcemosmsg.h

GENHDR	= $(SAMSHDR) $(STUBHDR)

SRC	= qdebug.c mos_api.c mos_rsrc.c mos_talk.c mos_msg_attr.c \
          mos_que_attr.c mos_que_mgmt.c mos_msg_selfilter.c mos_util.c
LIBOBJ	= $(SAMSOBJ) $(STUBOBJ) $(SRC:.c=.o)

# ----------------------------------------------------------------

all: $(LIBS)

install:
	cp -f $(LIBS) $(LIBDIR)
	cp -f $(GENHDR) $(INCDIR)

libmos.a: $(LIBOBJ)
	ar rv $@ $(LIBOBJ)

# ----------------------------------------------------------------
# idl-generated stuff
# idl makes all this less convenient than it should be.
HSTUB_FLAGS	= -header -client none -server none -no_mepv
SSTUB_FLAGS	= -keep c_source -client none -no_mepv
CSTUB_FLAGS	= -no_mepv -server none -keep c_source
TMP		= /tmp/

mostypes.h: $(IDLDIR)/mostypes.idl
	$(IDL) -I$(IDLDIR) $(HSTUB_FLAGS) $?
mos_base.h: $(IDLDIR)/mos_base.idl
	$(IDL) -I$(IDLDIR) $(HSTUB_FLAGS) $?
mos_q.h: $(IDLDIR)/mos_q.idl
	$(IDL) -I$(IDLDIR) $(HSTUB_FLAGS) $?
mos_mgmt.h: $(IDLDIR)/mos_mgmt.idl
	$(IDL) -I$(IDLDIR) $(HSTUB_FLAGS) $?
mos_convert.h: $(IDLDIR)/mos_convert.idl
	$(IDL) -I$(IDLDIR) $(HSTUB_FLAGS) $?
mos_ack.h: $(IDLDIR)/mos_ack.idl
	$(IDL) -I$(IDLDIR) $(HSTUB_FLAGS) $?

mos_q_cstub.c: $(IDLDIR)/mos_q.idl
	$(IDL) $(CSTUB_FLAGS) -header $(TMP)mos_q.h $?
	rm -f $(TMP)mos_q.h
mos_mgmt_cstub.c: $(IDLDIR)/mos_mgmt.idl
	$(IDL) $(CSTUB_FLAGS) -header $(TMP)mos_mgmt.h $?
	rm -f $(TMP)mos_mgmt.h
mos_convert_cstub.c: $(IDLDIR)/mos_convert.idl
	$(IDL) -I. $(CSTUB_FLAGS) $?
	rm -f $(TMP)mos_convert.h
mos_ack_cstub.c: $(IDLDIR)/mos_ack.idl
	$(IDL) -I. $(CSTUB_FLAGS) $?
	rm -f $(TMP)mos_ack.h

# ----------------------------------------------------------------
# sams-generated stuff
dcemossvc.o: dcemosmsg.h dcemosmac.h
dcemossvc.c: $(IDLDIR)/mos.sams
	$(SAMS) -oS $?
dcemossvc.h: $(IDLDIR)/mos.sams
	$(SAMS) -os $?
dcemosmsg.c: $(IDLDIR)/mos.sams
	$(SAMS) -ot $?
dcemosmsg.h: $(IDLDIR)/mos.sams
	$(SAMS) -oh $?
dcemosmac.h: $(IDLDIR)/mos.sams
	$(SAMS) -ou $?

# ----------------------------------------------------------------
clean:
	rm -f *.o $(LIBS) $(LIBOBJ) $(SAMSHDR) $(SAMSSRC) $(STUBSRC) $(STUBHDR) core

