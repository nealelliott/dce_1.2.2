#
#      SCCS:  @(#)dtet.mk	1.3 (92/10/06) 
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
# SCCS:   	@(#)dtet.mk	1.3 92/10/06
# NAME:		dtet.mk
# PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
# AUTHOR:		Andrew Dingwall, UniSoft Ltd.
# DATE CREATED:	June 1992
#
# DESCRIPTION:
#	aux makefile for dtet tcm files
# 
# MODIFICATIONS:
#	Andrew Dingwall, UniSoft Ltd., October 1992
#	added rules for source files required to build dtcmchild.o
# 
# ************************************************************************

include ../defines.mk
LCDEFS = $(DTET_CDEFS)
include ../common.mk

# generic tcm .o files
OFILES_GN = child.o ctcmfuncs.o mtcmfuncs.o rtcmfuncs.o stcmfuncs.o \
	tcmfuncs.o fake.o

# INET-specific tcm .o files
OFILES_IN = tcm_in.o tcm_bs.o

OFILES = $(OFILES_GN) $(OFILES_IN)

all install: $(OFILES)

CLEAN clean:
	rm -f $(OFILES)

CLOBBER clobber: clean

FORCE FRC: clobber all


# dependencies

child.o: $(DINC)/dtet/tet_api.h $(INC)/dapi.h $(INC)/dtetlib.h \
	$(INC)/dtmsg.h $(INC)/error.h $(INC)/synreq.h tcmfuncs.h

ctcmfuncs.o: $(INC)/dtmsg.h $(INC)/error.h $(INC)/synreq.h tcmfuncs.h

fake.o: $(INC)/dtmsg.h $(INC)/error.h $(INC)/ptab.h $(INC)/server.h

lint_in.o: $(INC)/dtmsg.h $(INC)/ptab.h $(INC)/server_bs.h $(INC)/server_in.h \
	tcmfuncs.h

mtcmfuncs.o: $(INC)/dtmsg.h $(INC)/error.h $(INC)/synreq.h tcmfuncs.h

rtcmfuncs.o: $(INC)/dtmsg.h $(INC)/synreq.h tcmfuncs.h

stcmfuncs.o: $(INC)/dtmsg.h $(INC)/error.h $(INC)/synreq.h tcmfuncs.h

tcm_bs.o: $(INC)/avmsg.h $(INC)/dtetlib.h $(INC)/dtmac.h $(INC)/dtmsg.h \
	$(INC)/error.h $(INC)/ptab.h $(INC)/server_bs.h $(INC)/valmsg.h \
	tcmfuncs.h

tcm_in.o: $(INC)/dtetlib.h $(INC)/dtmac.h $(INC)/dtmsg.h $(INC)/error.h \
	$(INC)/inetlib_in.h $(INC)/ltoa.h $(INC)/ptab.h $(INC)/server.h \
	$(INC)/tptab_in.h $(INC)/tsinfo_in.h tcmfuncs.h

tcmfuncs.o: $(INC)/dapi.h $(INC)/dtetlib.h $(INC)/dtmac.h $(INC)/dtmsg.h \
	$(INC)/error.h $(INC)/ltoa.h $(INC)/ptab.h $(INC)/server.h \
	$(INC)/servlib.h $(INC)/synreq.h $(INC)/tslib.h tcmfuncs.h


