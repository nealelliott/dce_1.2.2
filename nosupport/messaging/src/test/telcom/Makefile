# ______________________________________________________________________
# Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
# All rights reserved.
# ______________________________________________________________________
#
# $Log: Makefile,v $
# Revision 1.2  1996/11/25  15:52:26  bowe
# cd into idl (and make there) before making rest
#
# Revision 1.1  1996/11/20  17:13:03  salamone
# Initial version
#
# $EndLog$
# ________________________________________________________________________
IDLDIR  = idl
IDL     = idl -I$(IDLDIR)

CC      = c89
CFLAGS  = -g -D_HPUX_SOURCE -I. -Iidl -I../../idl -I../include
LDFLAGS = -g -z -Wl,-Bimmediate,-Bnonfatal,-a,default,+b,/lib:/usr/lib
LIB     = -L../../api -lmos -ldce -lc_r

STUBSRC = telcom_convert_cstub.c
STUBHDR = charge_base.h mwi_base.h telcom_convert.h
STUBOBJ = $(STUBSRC:.c=.o)
IDLSRC  = telcom_convert_cstub.acf

SRC1    = telcom_deqall_mq.c
OBJ1    = $(STUBOBJ) $(SRC1:.c=.o)
SRC2    = telcom_multiq_mq.c
OBJ2    = $(STUBOBJ) $(SRC2:.c=.o)
SRC3    = telcom_oneq_mq.c
OBJ3    = $(STUBOBJ) $(SRC3:.c=.o)
PROGS   = tmq_deqall tmq_multiq tmq_oneq 

# ----------------------------------------------------------------

all: idl-files $(PROGS)

idl-files:
	(cd idl ; make all)

tmq_deqall: $(OBJ1) 
	$(CC) $(LDFLAGS) -o $@ $(OBJ1) $(LIB)

tmq_multiq: $(OBJ2)
	$(CC) $(LDFLAGS) -o $@ $(OBJ2) $(LIB)

tmq_oneq: $(OBJ3) 
	$(CC) $(LDFLAGS) -o $@ $(OBJ3) $(LIB)

telcom_deqall_mq.c: ../../idl/dcemosmsg.h ../../idl/dcemosmac.h ../../idl/dcemossvc.h
telcom_multiq_mq.c: ../../idl/dcemosmsg.h ../../idl/dcemosmac.h ../../idl/dcemossvc.h
telcom_oneq_mq.c: ../../idl/dcemosmsg.h ../../idl/dcemosmac.h ../../idl/dcemossvc.h

# ----------------------------------------------------------------
# idl-generated stuff
# idl makes all this less convenient than it should be
HSTUB_FLAGS   = -header -client none -server none -no_mepv
SSTUB_FLAGS   = -keep c_source -client none -no_mepv
CSTUB_FLAGS   = -no_mepv -server none -keep c_source
TMP           = /tmp

mwi_base.h: $(IDLDIR)/mwi_base.idl
	$(IDL) -I$(IDLDIR) $(HSTUB_FLAGS) $?
charge_base.h: $(IDLDIR)/charge_base.idl
	$(IDL) -I$(IDLDIR) $(HSTUB_FLAGS) $?
telcom_convert.h: $(IDLDIR)/telcom_convert.idl
	$(IDL) -I$(IDLDIR) $(HSTUB_FLAGS) $?

telcom_convert_cstub.c: $(IDLDIR)/telcom_convert.idl
	$(IDL) -I$(IDLDIR) $(CSTUB_FLAGS) $?
# ----------------------------------------------------------------
clean:
	rm -f *.o $(PROGS) $(STUBSRC) $(STUBOBJ) $(STUBHDR) core
	(cd idl ; make clean)

