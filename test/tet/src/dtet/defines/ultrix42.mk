#
#      SCCS:  @(#)ultrix42.mk	1.2 (92/09/30) 
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
# SCCS:   	@(#)ultrix42.mk	1.2 92/09/30
# NAME:		ultrix42.mk
# PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
# AUTHOR:		Andrew Dingwall, UniSoft Ltd.
# DATE CREATED:	June 1992
#
# DESCRIPTION:
#	common machine-dependent definitions used in makefiles
#	this file is included in lower level makefiles
#
#	this one for ULTRIX 4.2
#
# MODIFICATIONS:
# 
# ************************************************************************

# tccd can be started:
#	from /etc/inittab (SYSV systems)
#	from /etc/inetd (BSD4.3 style)
#	from /etc/rc (BSD4.2 style)
#	interactively by a user
#
# inittab systems should include -DINITTAB in DTET_CDEFS below
# inetd systems should include -DINETD in DTET_CDEFS below

# TCCD should be either in.tccd (INETD defined) or tccd
TCCD = tccd

# parallel build indicator (mainly for DYNIX)
P =

# make utilities
MAKE = make
SHELL = /bin/sh

# TET and DTET defines:
#	TET_CDEFS are used in the tcc apilib makefiles
#	DTET_CDEFS are used in all the other makefiles
TET_CDEFS = -D_POSIX_SOURCE -D__POSIX -DNSIG=32
DTET_CDEFS =

# sgs component definitions and flags
# CDEFS may be passed to lint and cc, COPTS to cc only
CC = cc
CDEFS = -I$(INC) -I$(DINC)
COPTS = -O -Olimit 680
LDFLAGS =
MCS = @:
AR = ar
RANLIB = ranlib
# ultrix lorder/tsort seem not to work properly, so ...
LORDER = echo
TSORT = cat

# system libraries for inclusion at the end of cc command line
SYSLIBS =

# lint libraries for inclusion at the end of lint command line
LINTLIBS =

# rules to make the lint libraries
.SUFFIXES: .ln

.c.ln:
	lint $(CDEFS) -c $<

