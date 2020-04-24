# ________________________________________________________________________
# Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
# All rights reserved.
# ________________________________________________________________________
#
# $Log: Makefile,v $
# Revision 1.3  1996/12/04  15:59:17  bowe
# Add tcl_dce, moscp to list of subdirs
#
# Revision 1.2  1996/10/03  19:47:46  bowe
# Added "test" to list of dirs.
#
# Revision 1.1  1996/09/12  20:00:27  bowe
# Initial revision
#
# $EndLog$
# ________________________________________________________________________

SUBDIRS = idl api qmgr tcl_dce moscp test

all:
	for i in $(SUBDIRS) ; do (cd $$i ; make all ) ; done
clean:
	for i in $(SUBDIRS) ; do (cd $$i ; make clean ) ; done

