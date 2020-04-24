# ________________________________________________________________________
# Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
# All rights reserved.
# ________________________________________________________________________
#
# $Log: Makefile,v $
# Revision 1.1  1996/11/20  17:01:18  salamone
# Initial version
#
# $EndLog$
# ________________________________________________________________________
IDL	= idl
IDLSRC	= mwi_base.idl charge_base.idl telcom_convert.idl 

# generated files:
IDLGEN	= $(IDLSRC:.idl=.h)

all: $(IDLGEN) 
install:
clean:
	rm -f core *.o $(IDLGEN) 

# idl-generated
mwi_base.h: mwi_base.idl
	$(IDL) -keep none $?
charge_base.h: charge_base.idl
	$(IDL) -keep none $?
telcom_convert.h: telcom_convert.idl 
	$(IDL) -no_mepv -keep none $?
