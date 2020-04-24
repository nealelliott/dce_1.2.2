#
#      SCCS:  @(#)common.mk	1.2 (92/09/30) 
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
# SCCS:   	@(#)common.mk	1.2 92/09/30
# NAME:		common.mk
# PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
# AUTHOR:		Andrew Dingwall, UniSoft Ltd.
# DATE CREATED:	June 1992
#
# DESCRIPTION:
#	common machine-independent definitions used in makefiles
#	this file is included in lower level makefiles
# 
# MODIFICATIONS:
# 
# ************************************************************************

# options for $(CC) -c
CFLAGS = $(CDEFS) $(LCDEFS) $(COPTS)

# locations of non-local files and libraries
LIB = ../../../lib/dtet
DINC = ../../../inc
BIN = ../../../bin
INC = ../inc
LLIB = ../llib
LIBDAPI = $(LIB)/libdapi.a

