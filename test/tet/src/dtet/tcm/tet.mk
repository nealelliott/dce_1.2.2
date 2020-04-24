#
#      SCCS:  @(#)tet.mk	1.4 (92/11/10) 
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
# SCCS:   	@(#)tet.mk	1.4 92/11/10
# NAME:		tet.mk
# PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
# AUTHOR:		Andrew Dingwall, UniSoft Ltd.
# DATE CREATED:	June 1992
#
# DESCRIPTION:
#	aux makefile for tcm TET-derived files
# 
# MODIFICATIONS:
# 
# ************************************************************************

include ../defines.mk
LCDEFS = $(TET_CDEFS)
include ../common.mk

OFILES = dtcm.o ictp.o mtcmdist.o stcmdist.o

all install: $(OFILES)

CLEAN clean:
	rm -f $(OFILES)

CLOBBER clobber: clean

FORCE FRC: clobber all


# dependencies

dtcm.o: $(DINC)/dtet/tet_api.h $(INC)/dtmsg.h $(INC)/error.h $(INC)/synreq.h \
	tcmfuncs.h

ictp.o: $(DINC)/dtet/tet_api.h $(INC)/dtmsg.h $(INC)/dtetlib.h \
	$(INC)/synreq.h tcmfuncs.h

mtcmdist.o: $(INC)/dtmac.h $(INC)/dtmsg.h $(INC)/error.h $(INC)/synreq.h \
	$(INC)/dtetlib.h $(INC)/servlib.h $(DINC)/dtet/tet_api.h \
	$(DINC)/dtet/tet_jrnl.h tcmfuncs.h

stcmdist.o: tcmfuncs.h


