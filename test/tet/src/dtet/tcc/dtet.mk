#
#      SCCS:  @(#)dtet.mk	1.2 (92/09/30) 
#
#	UniSoft Ltd., London, England
#
# (C) Copyright 1992 X/Open Company Limited
#
# All rights reserved.  No part of this source code may be reproduced,
# stored in a retrieval system, or transmitted, in any form or by any
# means, electronic, mechanical, photocopying, recording or otherwise,
# except as stated in the end-user licence agreement, without the prior
# permission of the copyright owners.
#
# X/Open and the 'X' symbol are trademarks of X/Open Company Limited in
# the UK and other countries.
#
#
# ************************************************************************
#
# SCCS:   	@(#)dtet.mk	1.2 92/09/30
# NAME:		dtet.mk
# PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
# AUTHOR:		Andrew Dingwall, UniSoft Ltd.
# DATE CREATED:	June 1992
#
# DESCRIPTION:
#	aux makefile for dtet tcc files
# 
# MODIFICATIONS:
# 
# ************************************************************************

include ../defines.mk
LCDEFS = $(DTET_CDEFS)
include ../common.mk

# generic tcc .o files
OFILES_GN = d_fake.o d_tccfuncs.o 

# INET-specific tcc .o files
OFILES_IN = d_tcc_in.o d_tcc_bs.o d_fake_in.o

OFILES = $(OFILES_GN) $(OFILES_IN)

all: $(OFILES)

CLEAN clean:
	rm -f $(OFILES)

CLOBBER clobber: clean
	rm -f $(TARGETS)

FORCE FRC: clobber all


# dependencies

d_fake.o: $(INC)/dtmsg.h $(INC)/error.h $(INC)/ptab.h $(INC)/server.h \
	$(INC)/tcc_mac.h tccfuncs.h

d_fake_in.o: $(INC)/error.h $(INC)/server_in.h $(INC)/tcc_mac.h

d_tcc_bs.o: $(INC)/avmsg.h $(INC)/dtetlib.h $(INC)/dtmac.h $(INC)/dtmsg.h \
	$(INC)/error.h $(INC)/ptab.h $(INC)/server_bs.h $(INC)/servlib.h \
	$(INC)/tcc_mac.h $(INC)/valmsg.h tccfuncs.h

d_tcc_in.o: $(INC)/bstring.h $(INC)/dtetlib.h $(INC)/dtmac.h $(INC)/dtmsg.h \
	$(INC)/error.h $(INC)/inetlib_in.h $(INC)/ltoa.h $(INC)/ptab.h \
	$(INC)/server.h $(INC)/server_in.h $(INC)/tptab_in.h \
	$(INC)/tsinfo_in.h $(INC)/tslib.h tccfuncs.h

d_tccfuncs.o: $(INC)/dtetlib.h $(INC)/dtmac.h $(INC)/dtmsg.h $(INC)/error.h \
	$(INC)/ptab.h $(INC)/server.h $(INC)/tcc_mac.h $(INC)/tslib.h \
	tccfuncs.h


