# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.9.1  1996/10/02  17:48:11  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:38:15  damon]
#
# Revision 1.1.4.1  1994/06/09  14:08:55  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:23:36  annie]
# 
# Revision 1.1.2.3  1993/01/21  19:37:45  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  19:51:46  zeliff]
# 
# Revision 1.1.2.2  1992/11/18  20:34:14  jaffe
# 	Transarc delta: jdp-ot5415-mount-aggregate-on-attach 1.4
# 	  Selected comments:
# 	    This delta contains the bulk of the changes necessary to have aggregates
# 	    mounted at attach time.  The purpose of this feature is to provide a
# 	    means by which exported filesets (with no local mount points) will be
# 	    sync'd periodically.  To accomplish this, we have created a new VFS type
# 	    (agfs).  Whenever an aggregate is exported, a local mount of the aggregate
# 	    is also done; then, when the sync daemon does its thing, the aggregate's sync
# 	    vfsop will be called.  The agfs sync then calls the new sync aggrop, which
# 	    in the case of Episode, calls efs_sync().  The UFS sync aggrop doen't do
# 	    anything, since we know the UFS filesystem is mounted, and so, its sync vfsop
# 	    will get called anyway.
# 	    This delta must be bundled with the corresponding deltas in the Episode and
# 	    test configurations.
# 	    This is the makefile for the OSF1-specific ftutil code.
# 	    Remove leftover junk from $OFILES.
# 	    Rearrange things slightly to make use of a vendor-supplied definition
# 	    of MOUNT_AGFS.
# 	    Fix typo so that we add ftutil_osfmount.o to the library instead of
# 	    ftutil_osfmount.c.
# 	    Fix bogus linker warnings about shared libraries when compiling .o files.
# 	[1992/11/17  21:48:52  jaffe]
# 
# $EndLog$

VPATH			= ${TARGET_MACHINE}:${TARGET_OS}

libftutil.a_OFILES	+= ftutil_osfmount.o
