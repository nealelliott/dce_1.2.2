#
#      SCCS:  @(#)aix32.mk	1.1	92/11/17
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
# SCCS:   	@(#)aix32.mk	1.1 92/11/17
# NAME:		aix32.mk
# PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
# AUTHOR:		Andrew Dingwall, UniSoft Ltd.
# DATE CREATED:	November 1992
#
# DESCRIPTION:
#	common machine-dependent definitions used in makefiles
#	this file is included in lower level makefiles
#
#	this one for aix3.2 on the RS6000
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

# TCCD specifies the name by which tccd is to be known; this should be in.tccd
# if you define INETD, otherwise it should be tccd
TCCD = in.tccd

# parallel build indicator (mainly for DYNIX)
# leave this blank for most systems
P =

# make utilities - these don't usually change
MAKE = make
SHELL = /bin/sh

# TET and DTET defines; these are added to CDEFS in individual makefiles
#	TET_CDEFS are used to compile TET-derived code
#	these should include -D_POSIX_SOURCE and a value for NSIG (the number
#	of signals on your system)
#	also, you may want to define TET_SIG_IGNORE and TET_SIG_LEAVE here
#	for example:
#		TET_CDEFS = -D_POSIX_SOURCE -DNSIG=32
#	DTET_CDEFS are used to compile DTET and networking code
#
TET_CDEFS = -D_POSIX_SOURCE -DNSIG=64
DTET_CDEFS = -DINETD

# sgs component definitions and flags
# CC - the name of the C compiler
CC = cc
# CDEFS may be passed to lint and cc, COPTS to cc only
# CDEFS and COPTS are used to construct CFLAGS for use in make's built-in rules
CDEFS = -I$(INC) -I$(DINC)
COPTS = -O
# LDFLAGS - loader flags used by make's built-in rules
LDFLAGS =
# if your system's a.out format includes a .comment section that can be
# compressed by using mcs -c, set MCS to mcs; otherwise set MCS to @:
MCS = @:
# AR is the name of the archive library maintainer
AR = ar
# LORDER and TSORT are the names for lorder and tsort, used to order an archive
# library; if they don't exist on your system or don't work, set LORDER to echo
# and TSORT to cat
LORDER = lorder
TSORT = tsort
# if your system needs ranlib run after an archive library is updated,
# set RANLIB to ranlib; otherwise set RANLIB to @:
RANLIB = @:

# system libraries for inclusion at the end of cc command line
SYSLIBS =

# lint libraries for inclusion at the end of lint command line
LINTLIBS =

# rules to make the lint libraries
.SUFFIXES: .ln

.c.ln:
	lint $(CDEFS) -c -wk $<

# if you have a BSD-style lint and you want lint libraries, you will probably
# have to make them by hand


