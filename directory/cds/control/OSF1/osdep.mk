# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: osdep.mk,v $
# Revision 1.1.9.2  1996/03/09  23:11:41  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:00  marty]
#
# Revision 1.1.9.1  1995/12/08  15:06:12  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:39:09  root]
# 
# Revision 1.1.7.1  1994/03/12  21:57:23  peckham
# 	DEC serviceability and i18n drop
# 	[1994/03/12  13:55:12  peckham]
# 
# Revision 1.1.5.2  1992/12/30  12:43:47  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  22:30:04  zeliff]
# 
# Revision 1.1.3.3  1992/04/14  13:49:20  mhickey
# 	Search the backing tree(s) for libdce.so if not found in
# 	sandbox.  Defect 2598.
# 	[1992/04/07  20:55:41  mhickey]
# 
# Revision 1.1  1992/01/19  22:12:56  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
# 
#  OSF DCE Version 1.0 
# 
# if we are building with shared libraries on an OSF/1 system and DEBUG is 
# defined when parser_aid is built, then the parser_aid executable has been 
# built linked to the deb_* routines in the cds objects in libdce.so.  If this 
# is the case, then we must load the shared library (in the export area) before
# we can run parser_aid to create the mgmt_com_* include files.  This makefile
# takes care of this.
#

#
# Since we want to look in the backing tree(s) for libdce.so as well, we 
# use make's substution mechanism to generate a list of the directories to look
# at, and then loop through, stopping on the first one containing a libdce.so.
#

.if ${USE_SHARED_LIBRARIES} == 1
mgmt_com_act_init.h mgmt_com_act_decl.h : parser_aid.bpt parser_aid cdscp.bpt
	for i in ${SHLIBDIRS:S/^-L//g}; do \
	   if [ -f $$i/libdce.so ]; then \
	     break; \
	   fi \
	done; \
	inlib $$i/libdce.so; \
	./parser_aid mgmt_com_act_decl.h mgmt_com_act_init.h cds.i; \
	rmlib $$i/libdce.so

.endif

