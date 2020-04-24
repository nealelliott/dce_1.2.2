
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.154.3  1994/07/28  17:38:00  mckeen
# 	Added gateway subdirectory
# 	cleaned up old history entries
# 	[1994/07/28  14:10:54  mckeen]
#
# Revision 1.1.154.2  1994/07/13  22:31:01  devsrc
# 	merged with bl-10
# 	[1994/06/29  12:14:22  devsrc]
# 
# 	Moving tests from file/episode to test/file (ravage, scavange
# 	and rcx renamed to "recovery")
# 	[1994/03/21  17:59:22  delgado]
# 
# Revision 1.1.154.1  1994/02/04  20:38:24  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:21:37  devsrc]
# 
# Revision 1.1.152.1  1993/12/07  17:39:52  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  18:28:10  jaffe]
# 
# Revision 1.1.6.12  1993/04/09  17:15:09  gmd
# 	Added fset, itl, and tsr to DATA_SUBDIRS now that these Makefiles
# 	contain DATAFILES.
# 	[1993/04/09  17:13:32  gmd]
# 
# Revision 1.1  1992/01/19  03:19:29  devrcs
# 	Initial revision
# 
# $EndLog$
#

#
#

SUBDIRS= util itl backup bak bos cm fs fset fts low naming scout ubik update \
	 os_calls salvage rep cache_mgr acl tsr zlc pioctl fx \
	 recovery ravage scavenge gateway episode \
	 ftcleanup bakserver mmap cmfxauth cmmhs delegation.system

EXPLIB_SUBDIRS = cache_mgr itl fset ubik
EXPINC_SUBDIRS = itl ubik

DATA_SUBDIRS= util itl backup bak bos cm fs fset fts low naming scout ubik \
	      update salvage rep os_calls cache_mgr acl tsr zlc pioctl fx \
	      recovery ravage scavenge \
	      ftcleanup bakserver delegation.system

DATA_FILES=check runall README copytests.sh

ILIST	= ${DATA_FILES}

.include <${RULES_MK}>
