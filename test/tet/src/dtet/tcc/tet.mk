#
#      SCCS:  @(#)tet.mk	1.2 (92/09/30) 
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
# SCCS:   	@(#)tet.mk	1.2 92/09/30
# NAME:		tet.mk
# PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
# AUTHOR:		Andrew Dingwall, UniSoft Ltd.
# DATE CREATED:	June 1992
#
# DESCRIPTION:
#	aux makefile for tcc TET-derived files
# 
# MODIFICATIONS:
# 
# ************************************************************************

include ../defines.mk
LCDEFS = $(TET_CDEFS)
include ../common.mk

ALL = again.o bld_cln.o config.o dconfig.o dist.o error.o exec.o \
	generic.o group.o journal.o lock.o misc.o modes.o remote.o rescode.o \
	resfile.o save.o scenario.o spawn.o store.o syntax.o tcc.o \
	testcase.o util.o

all install: $(ALL)

CLEAN clean:
	rm -f $(ALL)

CLOBBER clobber: clean

FORCE FRC: clobber all


# dependencies

again.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

bld_cln.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

config.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

dconfig.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

dist.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

error.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

exec.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

generic.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

group.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

journal.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

lock.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

misc.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

modes.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

remote.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

rescode.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

resfile.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

save.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

scenario.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

spawn.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

store.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

syntax.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

tcc.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

testcase.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h

util.o: $(DINC)/dtet/tet_jrnl.h $(INC)/tcc_env.h $(INC)/tcc_mac.h \
	$(INC)/tcc_prot.h


