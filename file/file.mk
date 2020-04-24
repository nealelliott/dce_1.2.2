#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: file.mk,v $
# Revision 1.1.28.1  1996/10/02  17:45:40  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:36:47  damon]
#
# Revision 1.1.21.2  1994/06/09  14:06:44  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:21:59  annie]
# 
# Revision 1.1.21.1  1994/02/04  20:17:38  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:12:32  devsrc]
# 
# Revision 1.1.19.2  1994/01/20  18:43:01  annie
# 	added copyright header
# 	[1994/01/20  18:39:16  annie]
# 
# Revision 1.1.19.1  1993/12/07  17:23:21  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:14:12  jaffe]
# 
# Revision 1.1.15.2  1993/07/19  19:30:42  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:23:13  zeliff]
# 
# Revision 1.1.13.4  1993/07/16  19:32:49  kissel
# 	Remove the exporters dir since it is not ready for prime time.
# 	[1993/06/23  16:11:40  kissel]
# 
# 	*** empty log message ***
# 	[1993/06/21  14:28:06  kissel]
# 
# Revision 1.1.11.2  1993/06/04  18:24:38  kissel
# 	Initial HPUX RP version.
# 	[1993/06/03  21:37:01  kissel]
# 
# Revision 1.1.3.5  1993/01/15  18:49:28  toml
# 	Merge Tarc2.3 with 1.1.3.4: add file/exporters, and HP800 to kernel dirs conditional.
# 	[1993/01/15  18:39:36  toml]
# 
# Revision 1.1.7.4  1993/01/21  19:33:15  zeliff
# 	Embedding copyright notices
# 	[1993/01/19  19:46:49  zeliff]
# 
# 	Move conditional that turns off USE_SHARED_LIBRARIES for
# 	kernel directories from osf.dce.mk to this file.
# 	Defect 3332.
# 	[1992/05/11  15:52:58  mhickey]
# 
# Revision 1.1.1.11  1993/01/14  16:52:23  toml
# 	    Loading drop Dce1_0_2_Tarc2_3
# 
# Revision 1.1.3.4  1992/11/23  21:01:31  kinney
# 	Reconcile following (1.1.3.3) change with bd5 drop (tarc-55); the revision #s are messed up?
# 	[1992/11/23  21:00:36  kinney]
# 
# Revision 1.1.7.3  1992/11/18  19:43:04  jaffe
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
# 	    Remove leftover junk from $OFILES.
# 	    Rearrange things slightly to make use of a vendor-supplied definition
# 	    of MOUNT_AGFS.
# 	    Fix bogus linker warnings about shared libraries when compiling .o files.
# 	    Turns out that this is the magic file which turns off USE_SHARED_LIBRARIES
# 	    for the in-kernel directories and of course Jeff forgot to add agfs here,
# 	    like it's an obvious thing to do.
# 	    Can't file.mk set USE_SHARED_LIBRARIES to 0 whenever KERNEL_CC is defined
# 	    on the PMAX?
# 	    Anyone?
# 	[1992/11/17  21:28:50  jaffe]
# 
# Revision 1.1.7.2  1992/10/27  21:03:18  jaffe
# 	Transarc delta: comer-3013-zlc-file-handling 1.13
# 	  Selected comments:
# 	    In order to handle the ability to continue to read or write and open
# 	    file after that file has been unlinked, is addressed by open-for-delete
# 	    (OFD) tokens.  When a file is unlinked, the file exporter or glue code
# 	    tries to get this OFD token; but, the token is not granted until all
# 	    other open tokens have been returned or have expired.
# 	    This scheme has to be modified to work with fileset moves or a moved
# 	    zero-linkcount (deleted) file's storage will be recovered prematurely.
# 	    This delta contains the implementation for a zero-linkcount (ZLC) file
# 	    manager that sits in the core extension and manages ZLC files with its
# 	    own thread and as its own host object.
# 	    Fixed a problem with the tkc code where the cache entry reference
# 	    count was not being decremented (tkc_Release wasn't being called),
# 	    so vnodes were never being released (and storage not reclaimed).
# 	    1) Replaced a call to volreg_Lookup with one to VOL_VGET to fix a
# 	    deadlock problem.  The volreg_Lookup was unnecessary anyway.
# 	    2) Changed code to no longer try and reset SOURCE and TARGET flags
# 	    in volume structure.
# 	    3) Keep track of the number of items on the ZLC list.
# 	    4) Call the ZLC clean function when destroying a volume.
# 	    Was using the wrong fid when doing a VGET.
# 	    Was calling volreg_LookupExtended with an xcode = 1.  How foolish of
# 	    me!  xcode should be set to 2.
# 	    Fixed another fid problem and fixed a problem with returning bogus
# 	    tokens.
# 	    The fid passed from VOLOP_SETATTR did not contain a volume or
# 	    cell.  This is wrong....
# 	    In the event an asyncronous grant was granted for a file no longer in the
# 	    zlc list, an incorrect value was being returned to the token manager.
# 	    Since 1) the volume package makes calls to the zlc package, 2) stand-alone
# 	    episode includes the volume package, 3) stand-alone episode doesn't
# 	    require the functionality from this package, and 4) stand-alone episode
# 	    cannot link sdcelfs.ext because of references to zlc symbols, a stubs file
# 	    is created here.  The file is actually compiled from the libefs/<system>
# 	    Makefiles.
# 	    Need to hook zlc into build process.
# 	    link problems
# 	    Turn off shared libraries for ZLC too.
# 	[1992/10/27  14:20:56  jaffe]
# 
# Revision 1.1.2.6  1992/05/22  19:12:02  garyf
# 	more location of -I- for improved include handling
# 	[1992/05/22  03:08:52  garyf]
# 
# Revision 1.1.2.5  1992/05/11  18:04:55  mhickey
# 	Moved conditionals that switch off USE_SHARED_LIBRARIES for kernel
# 	pieces of the build from osf.dce.mk to the proper
# 	component .mk files.
# 	Defect 3332
# 	[1992/05/11  18:03:50  mhickey]
# 
# Revision 1.1.2.4  1992/05/05  19:00:33  rec
# 	Removed dependence on libkrb5.a by allowing libdce.a
# 	to export required symbol (krb5_cksumarray).
# 	[1992/05/05  18:54:33  rec]
# 
# Revision 1.1.2.3  1992/04/07  20:27:09  rec
# 	Have -lkrb5 on link line depend on RIOS platform since
# 	it is not needed with a shared libdce on OSF/1.
# 	[1992/04/07  19:58:25  rec]
# 
# Revision 1.1.2.2  1992/01/24  04:23:45  devsrc
# 	Fixed logs
# 	[1992/01/23  22:15:39  devsrc]
# 
# $EndLog$
#
#
# 10-22-91; mhickey: Modified for the new model for building libdce.a.  This
#                    means that:
#                       - This makefile allows 'make' to pick up the
#                         definitions within it if it is being read as part
#                         of the parsing of the libdce.a Makefile; ie: the
#                         variable LIBDCE_LIBRARY_MAKEFILE is defined.
#
#                       - The '.if' conditions that determine the settings of
#                         variables for directories where pieces of libdce.a 
#                         are built are conditional on the value of the 
#                         variable LIBDCE_CURR_INCDIR if the variable 
#                         LIBDCE_LIBRARY_MAKEFILE is set.
#       
#                       - ${${TARGET_MACHINE}_OFILES} settings for directories
#                         where pieces of libdce.a are built are changed to 
#                         ${${TARGET_MACHINE}_<libname>_OFILES}.
#
#
# 10-22-91; mhickey: Ported for PMAX/OSF1. This means that I added a few 
#                    PMAX_OFILES and PMAX_LIBS definitions, and a '.if PMAX'.
#

#
# This makefile needs to allow multiple accesses for the libdce build.  The
# multiple accesses should not be idempontent, since we want different settings
# on each access.
#
.if !defined(_FILE_MK) 
_FILE_MK_=

RIOS_CFLAGS_ALL = -Dunix -D_ALL_SOURCE

RIOS_CFLAGS	?= ${RIOS_CFLAGS_ALL}

CFLAGS ?= ${${TARGET_MACHINE}_CFLAGS}

_ansi_GENINC_= -I- ${x:L:!${GENPATH} -I.!}
_traditional_GENINC_= -I- ${x:L:!${GENPATH} -I.!}
_writable_strings_GENINC_= -I- ${x:L:!${GENPATH} -I.!}

.if ${TARGET_MACHINE}=="PMAX" || ${TARGET_MACHINE}=="HP800"
# Turn off shared libraries for all in-kernel DFS components
.if   (!empty(MAKESUB:Mfile/cm*)     || !empty(MAKESUB:Mfile/epidummy*) || \
       !empty(MAKESUB:Mfile/fshost*) || !empty(MAKESUB:Mfile/px*)       || \
       !empty(MAKESUB:Mfile/sysacl*) || !empty(MAKESUB:Mfile/tkc*)      || \
       !empty(MAKESUB:Mfile/tkset*)  || !empty(MAKESUB:Mfile/ufsops*)   || \
       !empty(MAKESUB:Mfile/volreg*) || !empty(MAKESUB:Mfile/xvnode*)   || \
       !empty(MAKESUB:Mfile/episode/libefs*) ||                            \
       !empty(MAKESUB:Mfile/zlc*) ||                                       \
       !empty(MAKESUB:Mfile/agfs*) ||                                      \
       !empty(MAKESUB:Mfile/libafs*) ||                                    \
       !empty(MAKESUB:Mfile/*.klib*)) && empty(MAKESUB:Mfile/pxd*)
USE_SHARED_LIBRARIES=0
.endif
.endif

.endif

