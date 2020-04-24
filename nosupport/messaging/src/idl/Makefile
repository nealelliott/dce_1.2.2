# ________________________________________________________________________
# Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
# All rights reserved.
# ________________________________________________________________________
#
# $Log: Makefile,v $
# Revision 1.4  1996/11/13  17:21:52  bowe
# Provisions for mos_ack.*
#
# Revision 1.3  1996/10/01  13:39:41  bowe
# Added dcemossvc.h, dcemosmac.h to SAMSGEN.
#
# Revision 1.2  1996/09/12  19:58:33  bowe
# Added rcslog cookies.
#
# $EndLog$
# ________________________________________________________________________

IDL	= idl
SAMS	= sams
GENCAT	= gencat

IDLSRC	= mostypes.idl mos_base.idl mos_q.idl mos_mgmt.idl mos_convert.idl \
	  mos_ack.idl
SAMSSRC	= mos.sams
SRC	= $(IDLSRC) $(SAMSSRC)

# generated files:
IDLGEN	= $(IDLSRC:.idl=.h)
SAMSGEN	= dcemosmsg.h dcemos.msg dcemos.cat dcemossvc.h dcemosmac.h

all: $(IDLGEN) $(SAMSGEN)
install:
clean:
	rm -f core *.o $(IDLGEN) $(SAMSGEN)

# idl-generated
mostypes.h: mostypes.idl
	$(IDL) -keep none $?
mos_base.h: mos_base.idl
	$(IDL) -keep none $?
mos_q.h: mos_q.idl mos_base.h
	$(IDL) -keep none mos_q.idl
mos_mgmt.h: mos_mgmt.idl mos_base.h
	$(IDL) -keep none mos_mgmt.idl
mos_convert.h: mos_convert.idl mos_base.h
	$(IDL) -no_mepv -keep none mos_convert.idl
mos_ack.h: mos_ack.idl mos_base.h
	$(IDL) -no_mepv -keep none mos_ack.idl

# sams-generated
dcemossvc.h: mos.sams
	$(SAMS) -os $?
dcemosmsg.h: mos.sams
	$(SAMS) -oh $?
dcemosmac.h: mos.sams
	$(SAMS) -ou $?
dcemos.msg: mos.sams
	$(SAMS) -om $?
dcemos.cat: mos.sams
	GENCAT=$(GENCAT) $(SAMS) -oc $?

