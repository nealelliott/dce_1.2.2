# ________________________________________________________________________
# Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
# All rights reserved.
# ________________________________________________________________________
#
# $Log: Makefile,v $
# Revision 1.2  1996/11/21  13:34:40  salamone
# Added basic & telcom subdirs
#
# Revision 1.1  96/10/03  19:47:25  19:47:25  bowe (John Bowe)
# Initial revision
# 
# $EndLog$
# ________________________________________________________________________

SUBDIRS = qif basic telcom

all:
	for i in $(SUBDIRS) ; do (cd $$i ; make all ) ; done
clean:
	for i in $(SUBDIRS) ; do (cd $$i ; make clean ) ; done

