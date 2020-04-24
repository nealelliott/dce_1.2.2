#
# @OSF_FREE_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1993, 1992, 1991, 1990, 1996 Open Software Foundation, Inc.
# Permission is hereby granted to use, copy, modify and freely distribute
# the software in this file and its documentation for any purpose without
# fee, provided that the above copyright notice appears in all copies and
# that both the copyright notice and this permission notice appear in
# supporting documentation.  Further, provided that the name of Open
# Software Foundation, Inc. ("OSF") not be used in advertising or
# publicity pertaining to distribution of the software without prior
# written permission from OSF.  OSF makes no representations about the
# suitability of this software for any purpose.  It is provided "as is"
# without express or implied warranty.
#
#
# HISTORY
# $Log: osf.dce.passes.mk,v $
# Revision 1.2.25.4  1996/10/15  20:45:02  arvind
# 	OSF DCE 1.2.2 Drop 4.5
# 	[1996/10/15  20:08:58  arvind]
#
# Revision 1.2.25.3  1996/10/04  16:28:47  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/04  16:28:23  arvind]
# 
# Revision /main/DCE_1.2.2/3  1996/10/02  18:08 UTC  sommerfeld
# 	Fix DATAFILES, samidl_all handling:
# 	DATAFILES triggered run of FIRST pass, not FIRSTA pass.
# 	`build samidl_all' did only an export_all, not a build_all, of
# 	samidl..
# 	[1996/10/02  18:06 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
# 
# Revision /main/DCE_1.2.2/2  1996/08/23  14:42 UTC  aha
# 	Merge from sommerfeld_pk_kdc to aha_pk9
# 	[1996/08/13  21:32 UTC  aha  /main/DCE_1.2.2/aha_pk9/1]
# 
# 	Magic hacks for pass minimization.
# 	You are not expected to understand this.
# 	Heck, I don't any more, and I wrote it last week.
# 	[1995/09/27  17:56 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
# 
# Revision /main/DCE_1.2.2/1  1996/05/24  23:11 UTC  sommerfeld
# 	Merge from Hpdce02_02
# 	[1994/09/21  12:46 UTC  mothra  /main/HPDCE02/5]
# 
# 	CR 11111 - Added DEFAULT_SUBDIRS handling.
# 	[1994/08/29  23:13:37  marty]
# 	#
# 
# 	another kludge for clearcase: export all .idl files in AUTOGEN pass so
# 	that they're all there by the time we try to compile any of them in
# 	the FIRST pass..
# 	[1994/08/04  23:36 UTC  sommerfeld  /main/dce1.1_clearmakefix/1]
# 
# 	added VISTA - 11163
# 	[1994/07/26  20:51:28  annie]
# 
# 	Fix merge botch.
# 	[1994/07/15  15:13 UTC  sommerfeld  /main/HPDCE02/2]
# 
# 	Add additional "rm -f"'s on cpdata pass (ClearCase only)
# 	[1993/11/15  16:17:16  sommerfeld]
# 	#
# 
# 	Clean up for ClearCase
# 	[1993/11/10  20:08:05  sommerfeld]
# 
# 	Loading drop DCE1_0_3b03
# 	[1993/09/15  15:01:29  root  1.1.2.3]
# 
# Revision 1.2.23.2  1996/03/09  23:25:17  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:40:55  marty]
# 
# Revision 1.2.23.1  1995/12/07  22:09:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:08:30  root]
# 
# Revision 1.2.19.3  1994/08/30  15:01:44  marty
# 	CR 11111 - Added DEFAULT_SUBDIRS handling.
# 	[1994/08/29  23:13:37  marty]
# 
# Revision 1.2.19.2  1994/07/26  20:51:59  annie
# 	added VISTA - 11163
# 	[1994/07/26  20:51:28  annie]
# 
# Revision 1.2.19.1  1994/01/24  22:01:51  bowe
# 	Change slashes in _SUBDIR_TAGS to "+++", then, later, change "+++"
# 	back to slashes when .TARGETS us referenced.
# 	[1994/01/24  21:53:14  bowe]
# 
# Revision 1.2.16.7  1993/07/16  16:29:54  cjd
# 	Added ${SAMS_TARGETS} to _EXPORT_SAMIDL_TARGETS_ so a
# 	'build samidl_all' will work.
# 	[1993/07/16  16:29:35  cjd]
# 
# Revision 1.2.16.6  1993/07/16  15:49:27  cjd
# 	Added SAMIDL_SUBDIRS to _ALL_EXP_SUBDIRS_ so that the CLOBBER
# 	pass picks up the sams files.
# 	[1993/07/16  15:49:03  cjd]
# 
# Revision 1.2.16.5  1993/07/15  18:57:40  cjd
# 	Replaced _SAMIDL_SAMIDL_TARGETS_ with SAMS_TARGETS
# 	[1993/07/15  18:57:23  cjd]
# 
# Revision 1.2.16.4  1993/07/15  18:39:49  cjd
# 	Added _SAMIDL_SAMIDL_TARGETS to BUILD_SAMIDL_TARGETS
# 	[1993/07/15  18:39:12  cjd]
# 
# Revision 1.2.16.3  1993/07/14  18:42:50  cjd
# 	 - Changed the name of the pass "SECOND" to "AUTOGEN"
# 	 - Changed EXPORT rules to reflect new structure.
# 	 - Added dependencies to export rules to support building
# 	   samidl_all
# 	[1993/07/14  18:42:23  cjd]
# 
# Revision 1.2.11.8  1993/01/28  15:03:35  john
# 	Fixed the CHKDATA pass so that IDLFILES and DATAFILES can be in a Makefile.
# 	[1993/01/12  21:27:29  john]
# 
# Revision 1.2.11.7  1993/01/08  19:29:16  john
# 	Added rm -f before coping files in the CHKDATA pass.
# 	[1993/01/08  19:01:42  john]
# 
# Revision 1.2.11.6  1993/01/05  21:11:53  zeliff
# 	A couple of changes:
# 	  1.  For CHKDATA_TARGETS, added '-f' flag to copy instruction.
# 		This is needed for incremental builds.
# 	  2.  Changed references to 'mv' to '${MV}' and 'cp' to '${CP}'
# 	[1993/01/05  21:09:10  zeliff]
# 
# Revision 1.2.11.5  1992/12/31  16:47:07  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:51:01  zeliff]
# 
# Revision 1.2.11.4  1992/12/10  17:20:04  marty
# 	OT# 5572.  Add general benchmarking of builds.
# 	[1992/12/10  17:16:58  marty]
# 
# Revision 1.2.11.3  1992/11/06  21:25:20  john
# 	Modified RMTARGET to only remove PROGRAMS, LIBRARIES, and SHARED_LIBRARIES.
# 	[1992/10/29  02:51:51  john]
# 
# Revision 1.2.11.2  1992/09/29  20:02:26  devsrc
# 	Add SUBMAKE_FLAGS to allow -n to be passed to submakes
# 	[1992/09/11  15:19:04  rec]
# 
# Revision 1.2.3.8  1992/05/12  20:20:44  mhickey
# 	  o Added CLOBBER pass.  Made CLOBBER_SUBDIRS = unique list of
# 	    all passes subdirs so that clobber can visit all subdirs to
# 	    clobber libraries, etc. (defect 191).  Added the following to
# 	    the list of targets for clobber:
# 	       things generated from IDLFILES (defect 3387),
# 	       SHARED_LIBRARIES (3029),
# 	       INCLUDES (defect 3037),
# 	       EXPSHLIB_TARGETS (defect 3412)
# 
# 	  o Added a CLOBBER_EXP pass.  Made CLOBBER_EXPSUBDIRS = unique list of
# 	    all subdirs visited by EXPORT passes.  Added SHARED_LIBRARY targets
# 	    to list of CLOBBER_EXP targets. (defect 175).
# 
# 	  o Made clean_all run off of the CLOBBER pass so that it will visit
# 	    all subdirs to remove non-target items (defect 3416).
# 	    Added the following to the clean targets:
# 	       things generated from IDLFILES (defect 3387),
# 	       SHARED LIBRARIES (defect 3040),
# 	       EXPSHLIB_TARGETS (defect 3416)
# 
# 	  o Made rmtargets_all run off of the CLOBBER pass so that it will visit
# 	    all subdirs to remove non-target items.
# 	    Added the following to the clean targets:
# 	       things generated from IDLFILES,
# 	       SHARED LIBRARIES
# 
# 	    defect 3418
# 
# 	  o Removed the second pass since it is not used, and is just a
# 	    cpu sucker.
# 	[1992/05/12  20:11:31  mhickey]
# 
# Revision 1.2.3.7  1992/05/05  14:45:21  john
# 	Re-added Ron's changes for pass three from EXPLIB EXPSHLIB to
# 	EXPSHLIB EXPLIB.  See comment for revision 1.2.3.3 by rec.
# 	[1992/05/05  14:43:00  john]
# 
# Revision 1.2.3.6  1992/04/30  22:03:16  john
# 	Merged into baseline and tested.
# 	[1992/04/30  21:54:27  john]
# 
# Revision 1.2.3.5  1992/04/30  14:28:35  mckeen
# 	Added indirect pass for action with more then one pass to properly
# 	order building in the top level of the source tree.
# 	[1992/04/30  14:05:05  mckeen]
# 
# Revision 1.2.3.4  1992/04/16  14:42:10  marty
# 	Fix copyright marker and hardcoded copyright notices
# 	[1992/04/16  14:39:22  marty]
# 
# Revision 1.2.3.3  1992/04/13  19:45:11  rec
# 	Changed the order of libraries built in the THIRD pass back
# 	to EXPSHLIB EXPLIB.  The EXPLIB pass depends on having a
# 	libdce which is done in the EXPSHLIB for shared libraries.
# 	Otherwise references to libdce.so are made before it is
# 	linked.  This means the wrong libdce gets picked up and
# 	may cause Linker/Glue code errors.
# 	[1992/04/13  19:29:09  rec]
# 
# Revision 1.2.3.2  1992/04/07  19:17:06  mckeen
# 	Add .ORDER directive to order DATAFILES patch operations
# 	[1992/03/31  16:37:01  mckeen]
# 
# 	Changed order of EXPLIB and EXPSHLIB in Third Pass
# 	Build IDLDATAFILES during EXPINC pass
# 	Added line to prevent make from recursivly spawning
# 	[1992/03/13  15:40:40  mckeen]
# 
# Revision 1.2  1992/01/19  22:13:20  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#------------------------ osf.dce.passes.mk --------------------------
#

.if !defined(_OSF_DCE_PASSES_MK_)
_OSF_DCE_PASSES_MK_=

#
# Start of PASSES
#

#
#  These list the "tags" associated with each pass
#
_PASS_SETUP_TAGS_=SETUP
_PASS_AUTOGEN_TAGS_=SAMIDL
_PASS_FIRST_TAGS_=EXPINC
.if !defined(ODE_UNDER_CLEARCASE)
_PASS_FIRSTA_TAGS_=MVDATA
_PASS_FIRSTB_TAGS_=CHKDATA
.else
_PASS_FIRSTA_TAGS_=CPDATA
_PASS_FIRSTB_TAGS_=
.endif

.if defined(NO_SHARED_LIBRARIES)
_PASS_THIRD_TAGS_=EXPLIB
.else
_PASS_THIRD_TAGS_=EXPSHLIB EXPLIB
.endif
_PASS_BASIC_TAGS_=STANDARD
_PASS_CLOBBER_TAGS_=CLOBBER
_PASS_CLOBBER_EXP_TAGS_=CLOBBER_EXP
_PASS_INDIRECT_TAGS_=INDIRECT
#
#  If DEFAULT_SUBDIRS is defined in the Makefile, then, try to use it
#  as the default value if any of the variables SETUP_SUBDIRS, EXPINC_SUBDIRS,
#  SAMIDL_SUBDIRS, DATA_SUBDIRS, EXPBIN_SUBDIRS, EXPLIB_SUBDIRS, 
#  EXPSHLIB_SUBDIRS, and SUBDIRS  have not already been defined.
#
.if defined(DEFAULT_SUBDIRS)
SETUP_SUBDIRS    ?= ${DEFAULT_SUBDIRS}
EXPINC_SUBDIRS   ?= ${DEFAULT_SUBDIRS}
SAMIDL_SUBDIRS   ?= ${DEFAULT_SUBDIRS}
DATA_SUBDIRS     ?= ${DEFAULT_SUBDIRS}
EXPBIN_SUBDIRS   ?= ${DEFAULT_SUBDIRS}
EXPLIB_SUBDIRS   ?= ${DEFAULT_SUBDIRS}
EXPSHLIB_SUBDIRS ?= ${DEFAULT_SUBDIRS}
SUBDIRS          ?= ${DEFAULT_SUBDIRS}
.endif

#
#  These list the variables used to define subdirectories to recurse into
#
_SETUP_SUBDIRS_ = ${SETUP_SUBDIRS}
_EXPINC_SUBDIRS_= ${EXPINC_SUBDIRS}
_SAMIDL_SUBDIRS_= ${SAMIDL_SUBDIRS}
_MVDATA_SUBDIRS_= ${DATA_SUBDIRS}
_CHKDATA_SUBDIRS_=${DATA_SUBDIRS}
_CPDATA_SUBDIRS_=${DATA_SUBDIRS}
_EXPBIN_SUBDIRS_=${EXPBIN_SUBDIRS}
_EXPLIB_SUBDIRS_=${EXPLIB_SUBDIRS}
_EXPSHLIB_SUBDIRS_=${EXPSHLIB_SUBDIRS}
_STANDARD_SUBDIRS_=${SUBDIRS}

#
# Set up the list of all export pass subdirs to make the CLOBBER_*SUBDIRS 
# conditionals cleaner.  We basically want _ALL_EXP_SUBDIRS_ to get set if
# there is a value for any of {EXPINC,EXPLIB,EXPSHLIB}_SUBDIRS, but we need
# to do the if defined check to avoid the "Malformed conditional" message that
# make spits out if you try to do the empty test on an undefined variable.
#
.if ( (defined(EXPINC_SUBDIRS) && !empty(EXPINC_SUBDIRS)) || \
      (defined(EXPSHLIB_SUBDIRS) && !empty(EXPSHLIB_SUBDIRS)) || \
      (defined(SAMIDL_SUBDIRS) && !empty(SAMIDL_SUBDIRS)) || \
      (defined(EXPLIB_SUBDIRS) && !empty(EXPLIB_SUBDIRS)) )
_ALL_EXP_SUBDIRS_= ${EXPINC_SUBDIRS} ${EXPSHLIB_SUBDIRS} \
                   ${EXPLIB_SUBDIRS} ${SAMIDL_SUBDIRS}
.endif

#
# Make _CLOBBER_EXP_SUBDIRS_ a list, without duplicates, of all the subdirs 
# visited by all of the EXPORT passes.  This is done via the parse_ext 
# UNIQUELIST function.  Since there is a performance penalty associated with 
# the use of the parse_ext makefile (it execs a shell to do it's work), we 
# check to see a) if we are make'ing a clobber_exp target, and b) if there are
# any EXPORT subdirs before invoking the makefile.  This saves a lot of build
# time.
#
.if !empty(.TARGETS:Mclobber_exp*) && \
                           (defined(_ALL_EXP_SUBDIRS_) && !empty(_ALL_EXP_SUBDIRS_))
   _CLOBBER_EXP_SUBDIRS_ = ${_ALL_EXP_SUBDIRS_}
   parse_ext_args = action_UNIQUELIST list__CLOBBER_EXP_SUBDIRS_
.include <osf.parse_ext.mk>
.endif


#
# Make _CLOBBER_SUBDIRS_ a list, without duplicates, of all the subdirs visited
# by the EXPORT passes and the BASIC pass.  This is done via the parse_ext 
# UNIQUELIST function.  Since there is a performance penalty associated with the
# use of the parse_ext makefile (it execs a shell to do it's work), we check to 
# see a) if we are make'ing a clobber target, and b) if there are any EXPORT or 
# BASIC subdirs before invoking the makefile.  This saves a lot of build time.
#
.if (!empty(.TARGETS:Nclobber_exp*:Mclobber_*) || \
    !empty(.TARGETS:Nclobber_exp*:Mclean_*) || \
    !empty(.TARGETS:Nclobber_exp*:Mrmtarget_*)) && \
    (  (defined(_ALL_EXP_SUBDIRS_) && !empty(_ALL_EXP_SUBDIRS_)) || \
       (defined(SUBDIRS) && !empty(SUBDIRS))   )
   _CLOBBER_SUBDIRS_ = ${_ALL_EXP_SUBDIRS_} ${SUBDIRS}
   parse_ext_args = action_UNIQUELIST list__CLOBBER_SUBDIRS_
.include <osf.parse_ext.mk>
.endif


#
#  For each ACTION define the action for recursion, the passes for the
#  action, the targets for the complete action, and the targets for each
#  pass of the action
#
.if defined(MAKEFILE_PASS)
_BUILD_PASSES_=${MAKEFILE_PASS}
.else
_W=${AUTOGEN:L:@.PASS.@${_SUBDIR_PASSES_} ${_TARGET_TAGS_}@}
_AUTOGEN_NEEDED_=${_BUILD_SAMIDL_TARGETS_} ${_W}
_X=${FIRST:L:@.PASS.@${_SUBDIR_PASSES_}@}
_FIRST_NEEDED_=${_BUILD_EXPINC_TARGETS_} ${_X}
_Y=${THIRD:L:@.PASS.@${_SUBDIR_PASSES_}@} ${_EXPLIB_TARGET_TAGS_}
_Z=${THIRD:L:@.PASS.@${_SUBDIR_PASSES_}@} ${_EXPSHLIB_TARGET_TAGS_}
_THIRD_NEEDED_=${_BUILD_EXPLIB_TARGETS_} ${_EXPORT_EXPSHLIB_TARGETS_} ${_Y} ${_Z}
_BASIC_NEEDED_=${_BUILD_STANDARD_TARGETS_} ${BASIC:L:@.PASS.@${_SUBDIR_PASSES_}@}

.if !defined(ODE_UNDER_CLEARCASE)
_BUILD_PASS_LIST_=${AUTOGEN_NEEDED} ${FIRST_NEEDED} FIRSTA FIRSTB ${THIRD_NEEDED} ${BASIC_NEEDED}
.else
_FIRSTA_NEEDED_=${FIRSTA:L:@.PASS.@${_SUBDIR_PASSES_}@} ${_BUILD_CPDATA_TARGETS_}

_BUILD_PASS_LIST_=${AUTOGEN_NEEDED} ${FIRST_NEEDED} ${FIRSTA_NEEDED} ${THIRD_NEEDED} ${BASIC_NEEDED}
.endif
_BUILD_PASSES_=INDIRECT
.endif

_BUILD_ACTION_=build
_build_action_=BUILD
_BUILD_INDIRECT_TARGETS_=_INDIRECT/BUILD
_BUILD_SAMIDL_TARGETS_=\
	${_EXPORT_SAMIDL_TARGETS_}  ${SAMS_TARGETS}
_BUILD_EXPINC_TARGETS_=\
	${_EXPORT_EXPINC_TARGETS_}
_BUILD_MVDATA_TARGETS_=\
	${_MVDATA_MVDATA_TARGETS_}
_BUILD_CHKDATA_TARGETS_=\
	${_CHKDATA_CHKDATA_TARGETS_}
_BUILD_CPDATA_TARGETS_=\
	${_CPDATA_CPDATA_TARGETS_}
_BUILD_EXPBIN_TARGETS_=\
	${_EXPORT_EXPBIN_TARGETS_}
_BUILD_EXPSHLIB_TARGETS_=\
	${_EXPORT_EXPSHLIB_TARGETS_}
_BUILD_EXPLIB_TARGETS_=\
	${_EXPORT_EXPLIB_TARGETS_}
_BUILD_STANDARD_TARGETS_=\
	${_COMP_STANDARD_TARGETS_}

.if defined(MAKEFILE_PASS)
_COMP_PASSES_=${MAKEFILE_PASS}
.else
_COMP_PASSES_=BASIC
.endif

_COMP_ACTION_=comp
_comp_action_=COMP

_COMP_STANDARD_TARGETS_=\
	${LIBRARIES} \
	${PROGRAMS} \
	${OBJECTS} \
	${SCRIPTS} \
	${DATAFILES} \
	${OTHERS} \
	${MANPAGES:=.0} \
	${DOCUMENTS:S/$/.ps/g} \
	${DOCUMENTS:S/$/.out/g}

.if defined(MAKEFILE_PASS)
_CLEAN_PASSES_=${MAKEFILE_PASS}
.else
_CLEAN_PASSES_=CLOBBER
.endif

_CLEAN_TARGETS_=${_clean_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_CLEAN_ACTION_=clean
_clean_action_=CLEAN

_CLEAN_CLOBBER_TARGETS_=\
	${PROGRAMS:S/^/clean_/g} \
	${LIBRARIES:S/^/clean_/g} \
	${SHARED_LIBRARIES:S/^/clean_/g} \
	${IDLFILES:S/.idl/_cstub/g:S/^/clean_/g} \
	${IDLFILES:S/.idl/_sstub/g:S/^/clean_/g} \
	${IDLFILES:S/.idl/_caux/g:S/^/clean_/g} \
	${IDLFILES:S/.idl/_saux/g:S/^/clean_/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/msg.h/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/svc.h/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/mac.h/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/msg.c/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/svc.c/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/.msg/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/.cat/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/msg.man/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/msg.sgm/g} \
	${OBJECTS:S/^/clean_/g} \
	${SCRIPTS:S/^/clean_/g} \
	${DATAFILES:S/^/clean_/g} \
	${OTHERS:S/^/clean_/g} \
	${MANPAGES:S/^/clean_/g:S/$/.0/g} \
	${DOCUMENTS:S/^/clean_/g:S/$/.ps/g} \
	${DOCUMENTS:S/^/clean_/g:S/$/.out/g}

.if defined(MAKEFILE_PASS)
_RMTARGET_PASSES_=${MAKEFILE_PASS}
.else
_RMTARGET_PASSES_=CLOBBER
.endif

_RMTARGET_TARGETS_=${_rmtarget_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_RMTARGET_ACTION_=rmtarget
_rmtarget_action_=RMTARGET

_RMTARGET_CLOBBER_TARGETS_=\
	${PROGRAMS:S/^/rmtarget_/g} \
	${LIBRARIES:S/^/rmtarget_/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/msg.h/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/svc.h/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/msg.c/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/svc.c/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/.msg/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/.cat/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/mac.h/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/msg.man/g} \
	${SAMSFILES:S/^/clean_dce/g:S/.sams//g:S/$/msg.sgm/g} \
	${SHARED_LIBRARIES:S/^/rmtarget_/g}

.if defined(MAKEFILE_PASS)
_CLOBBER_PASSES_=${MAKEFILE_PASS}
.else
_CLOBBER_PASSES_=CLOBBER
.endif

_CLOBBER_TARGETS_=${_clobber_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_CLOBBER_ACTION_=clobber
_clobber_action_=CLOBBER

_CLOBBER_CLOBBER_TARGETS_=\
	${INCLUDES:S/^/clobber_/g} \
	${IDLFILES:S/.idl/.h/g:S/^/clobber_/g} \
	${IDLFILES:S/.idl/_cstub.c/g:S/^/clobber_/g} \
	${IDLFILES:S/.idl/_sstub.c/g:S/^/clobber_/g} \
	${IDLFILES:S/.idl/_caux.c/g:S/^/clobber_/g} \
	${IDLFILES:S/.idl/_saux.c/g:S/^/clobber_/g} \
        ${SAMSFILES:S/^/clobber_dce/g:S/.sams//g:S/$/msg.h/g} \
        ${SAMSFILES:S/^/clobber_dce/g:S/.sams//g:S/$/svc.h/g} \
        ${SAMSFILES:S/^/clobber_dce/g:S/.sams//g:S/$/mac.h/g} \
        ${SAMSFILES:S/^/clobber_dce/g:S/.sams//g:S/$/msg.c/g} \
        ${SAMSFILES:S/^/clobber_dce/g:S/.sams//g:S/$/svc.c/g} \
        ${SAMSFILES:S/^/clobber_dce/g:S/.sams//g:S/$/.msg/g} \
        ${SAMSFILES:S/^/clobber_dce/g:S/.sams//g:S/$/.cat/g} \
        ${SAMSFILES:S/^/clobber_dce/g:S/.sams//g:S/$/msg.man/g} \
        ${SAMSFILES:S/^/clobber_dce/g:S/.sams//g:S/$/msg.sgm/g} \
	${PROGRAMS:S/^/clobber_/g} \
	${LIBRARIES:S/^/clobber_/g} \
	${SHARED_LIBRARIES:S/^/clobber_/g} \
	${EXPSHLIB_TARGETS:S/^/clobber_/g} \
	${OBJECTS:S/^/clobber_/g} \
	${SCRIPTS:S/^/clobber_/g} \
	${DATAFILES:S/^/clobber_/g} \
	${OTHERS:S/^/clobber_/g} \
	${MANPAGES:S/^/clobber_/g:S/$/.0/g} \
	${DOCUMENTS:S/^/clobber_/g:S/$/.ps/g} \
	${DOCUMENTS:S/^/clobber_/g:S/$/.out/g}

.if defined(MAKEFILE_PASS)
_SETUP_PASSES_=${MAKEFILE_PASS}
.else
_SETUP_PASSES_=SETUP
.endif
_SETUP_TARGETS_=${_setup_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_SETUP_ACTION_=setup
_setup_action_=SETUP

_SETUP_SETUP_TARGETS_=\
	${SETUP_PROGRAMS:S/^/setup_/g} \
	${SETUP_SCRIPTS:S/^/setup_/g} \
	${SETUP_OTHERS:S/^/setup_/g} \
	${_EXPORT_SETUP_TARGETS_}

# SAMIDL pass
.if defined(MAKEFILE_PASS)
_SAMIDL_PASSES_=${MAKEFILE_PASS}
.else
_SAMIDL_PASSES_=AUTOGEN
.endif
_SAMIDL_TARGETS_=${_samidl_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_SAMIDL_ACTION_=samidl
_samidl_action_=SAMIDL

_SAMIDL_SAMIDL_TARGETS_=\
	${SAMS_TARGETS:S/^/samidl_/g} \
	${SAMS_SCRIPTS:S/^/samidl_/g} \
	${SAMS_OTHERS:S/^/samidl_/g} \
	${_EXPORT_SAMIDL_TARGETS_}

.if defined(MAKEFILE_PASS)
_LINT_PASSES_=${MAKEFILE_PASS}
.else
_LINT_PASSES_=BASIC
.endif

_LINT_TARGETS_=${_lint_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_LINT_ACTION_=lint
_lint_action_=LINT

_LINT_STANDARD_TARGETS_=\
	${PROGRAMS:S/^/lint_/g} \
	${LIBRARIES:S/^/lint_/g} \
	${OBJECTS:S/^/lint_/g}

#
#  statmet passes
#  MAKEFILE_PASS is set in the rule section in osf.dme.passes.mk
#  PROGRAMS, LIBRARIES, LOCAL_LIBRARIES, OBJECTS and OTHERS are
#    component makefile variables
#
.if defined(MAKEFILE_PASS)
_STATMET_PASSES_=${MAKEFILE_PASS}
.else
_STATMET_PASSES_=BASIC
.endif
_STATMET_TARGETS_=${_statmet_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_STATMET_ACTION_=statmet
_statmet_action_=STATMET
_STATMET_STANDARD_TARGETS_=${PROGRAMS:S/^/statmet_/g} \
                        ${LIBRARIES:S/^/statmet_/g} \
                        ${LOCAL_LIBRARIES:S/^/statmet_/g} \
                        ${OBJECTS:S/^/statmet_/g}

.if defined(MAKEFILE_PASS)
_TAGS_PASSES_=${MAKEFILE_PASS}
.else
_TAGS_PASSES_=BASIC
.endif

_TAGS_TARGETS_=${_tags_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_TAGS_ACTION_=tags
_tags_action_=TAGS

_TAGS_STANDARD_TARGETS_=\
	${PROGRAMS:S/^/tags_/g} \
	${LIBRARIES:S/^/tags_/g} \
	${OBJECTS:S/^/tags_/g}

.if defined(MAKEFILE_PASS)
_EXPORT_PASSES_=${MAKEFILE_PASS}
.else
_EXPORT_PASS_LIST_=AUTOGEN FIRST THIRD
_EXPORT_PASSES_=INDIRECT
.endif

_EXPORT_TARGETS_=${_export_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_EXPORT_ACTION_=export
_export_action_=EXPORT
_EXPORT_INDIRECT_TARGETS_= _INDIRECT/EXPORT

.if defined(ODE_UNDER_CLEARCASE) && defined(EXPORT_USING_SYMLINKS)
# clearcase kludge; instead of having all -> export_foo.h -> foo.h,
# have all->export_foo.h and all->foo.h
# this may need to be extended for EXPLIB, EXPBIN, EXPSHLIB; we'll see.

.if !defined(EXPSAMIDL_TARGETS)
EXPSAMIDL_TARGETS=${SAMIDL_INCLUDES:S/^/export_/g} ${INCLUDES:M*.idl:S/^/export_/g} ${SAMS_TARGETS}
.endif
.if !defined(EXPINC_TARGETS)
EXPINC_TARGETS=${INCLUDES:S/^/export_/g}
.endif
.if !defined(EXPSHLIB_TARGETS)
EXPSHLIB_TARGETS=${SHARED_LIBRARIES:S/^/export_/g}
.endif
.if !defined(EXPLIB_TARGETS)
EXPLIB_TARGETS=${LIBRARIES:S/^/export_/g}
.endif
.endif

.if defined(EXPINC_TARGETS)
_EXPORT_EXPINC_TARGETS_=\
	${EXPINC_TARGETS} ${IDLDATAFILES}
.else
_EXPORT_EXPINC_TARGETS_=\
	${INCLUDES:S/^/export_/g} ${IDLDATAFILES}
.endif
_EXPORT_EXPBIN_TARGETS_=\
	${EXPBIN_TARGETS:S/^/export_/g}
.if defined(EXPSHLIB_TARGETS)
_EXPORT_EXPSHLIB_TARGETS_=\
	${EXPSHLIB_TARGETS}
.else
_EXPORT_EXPSHLIB_TARGETS_=\
	${SHARED_LIBRARIES:S/^/export_/g}
.endif
.if defined(EXPLIB_TARGETS)
_EXPORT_EXPLIB_TARGETS_=\
	${EXPLIB_TARGETS}
.else
_EXPORT_EXPLIB_TARGETS_=\
	${LIBRARIES:S/^/export_/g}
.endif

_EXPORT_SETUP_TARGETS_=\
	${SETUP_INCLUDES:S/^/export_/g}

.if defined(EXPSAMIDL_TARGETS)
_EXPORT_SAMIDL_TARGETS_=\
        ${EXPSAMIDL_TARGETS}
.else
_EXPORT_SAMIDL_TARGETS_=\
	${SAMIDL_INCLUDES:S/^/export_/g} ${SAMS_TARGETS}
.endif

.if defined(MAKEFILE_PASS)
_CLOBBER_EXP_PASSES_=${MAKEFILE_PASS}
.else
_CLOBBER_EXP_PASSES_= CLOBBER_EXP
.endif

_CLOBBER_EXP_TARGETS_=${_clobber_exp_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_CLOBBER_EXP_ACTION_=clobber_exp
_clobber_exp_action_=CLOBBER_EXP

_CLOBBER_EXP_CLOBBER_EXP_TARGETS_=\
${_EXPORT_EXPINC_TARGETS_:S/^export/clobber_exp/g} \
${EXPBIN_TARGETS:S/^/clobber_exp_/g}
.if defined(EXPLIB_TARGETS)
_CLOBBER_EXP_CLOBBER_EXP_TARGETS_ += ${EXPLIB_TARGETS:Mexport_*:S/^export_/clobber_exp_/g}
.else
_CLOBBER_EXP_CLOBBER_EXP_TARGETS_ += ${LIBRARIES:S/^/clobber_exp_/g}
.endif
.if defined(EXPSHLIB_TARGETS)
_CLOBBER_EXP_CLOBBER_EXP_TARGETS_ += ${EXPSHLIB_TARGETS:Mexport_*:S/^export_/clobber_exp_/g}
.else
_CLOBBER_EXP_CLOBBER_EXP_TARGETS_ += ${SHARED_LIBRARIES:S/^/clobber_exp_/g}
.endif

.if defined(ODE_UNDER_CLEARCASE)
.if defined(MAKEFILE_PASS)
_CPDATA_PASSES_=${MAKEFILE_PASS}
.else
_CPDATA_PASSES_=FIRSTA
.endif
_CPDATA_TARGETS_=${_cpdata_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_CPDATA_ACTION_=cpdata
_cpdata_action_=CPDATA

.if defined(DATAFILES)
_CPDATA_CPDATA_TARGETS_=${DATAFILES}
.endif

.else
.if defined(MAKEFILE_PASS)
_MVDATA_PASSES_=${MAKEFILE_PASS}
.else
_MVDATA_PASSES_=FIRSTA
.endif

_MVDATA_TARGETS_=${_mvdata_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_MVDATA_ACTION_=mvdata
_mvdata_action_=MVDATA

.if defined(DATAFILES)
_MVDATA_MVDATA_TARGETS_=${DATAFILES:S/$/.local/g}
.endif

.if defined(MAKEFILE_PASS)
_CHKDATA_PASSES_=${MAKEFILE_PASS}
.else
_CHKDATA_PASSES_=FIRSTB
.endif

_CHKDATA_TARGETS_=${_chkdata_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_CHKDATA_ACTION_=chkdata
_chkdata_action_=CHKDATA

.if defined(DATAFILES) || defined(IDLDATAFILES)
.if defined(IDLDATAFILES)
 RESTORE_TARGETS= ${DATAFILES:S/$/.restore/g} ${IDLDATAFILES}
.else
 RESTORE_TARGETS= ${DATAFILES:S/$/.restore/g} 
.endif
CHKDATA_TARGETS= ${DATAFILES:S/$/.datachk/g}
_CHKDATA_CHKDATA_TARGETS_=${CHKDATA_TARGETS}
.endif

#
# if we are not recursing in place; ie: __DIDDATA__ defined; then add the 
# restore targets to the list for CHKDATA
#
.if !defined(__DIDDATA__) 
_CHKDATA_CHKDATA_TARGETS_ += ${RESTORE_TARGETS}
.ORDER: ${_CHKDATA_CHKDATA_TARGETS_}
.endif
.endif

.if defined(MAKEFILE_PASS)
_INSTALL_PASSES_=${MAKEFILE_PASS}
.else
_INSTALL_PASSES_=BASIC
.endif

_INSTALL_TARGETS_=${_install_action_:@.ACTION.@${_TARGET_ACTIONS_}@}
_INSTALL_ACTION_=install
_install_action_=INSTALL

_INSTALL_STANDARD_TARGETS_=\
	${ILIST:S/^/install_/g}

#
#  Magic begins here...
#
#  This sequence of indirect macros basically performs the following
#  expansion inline to generate the correct dependents for each action
#
#  foreach pass (${_${action}_PASSES_})
#     foreach tag (${_PASS_${pass}_TAGS_})
#        foreach subdir (${_${tag}_SUBDIRS_})
#           _SUBDIR/${action}/${pass}/${tag}/${subdir}
#        end
#     foreach tag (${_PASS_${pass}_TAGS_})
#        ${_${action}_${tag}_TARGETS_}
#     end
#  end
#

_ALL_ACTIONS_=BUILD COMP CLEAN RMTARGET CLOBBER CLOBBER_EXP LINT STATMET TAGS EXPORT MVDATA CHKDATA INSTALL SETUP SAMIDL

_SUBDIR_TAGS_=${_${.TAG.}_SUBDIRS_:S;/;+++;g:S;^;_SUBDIR/${.ACTION.}/${.PASS.}/${.TAG.}/;g}
_SUBDIR_PASSES_=${_PASS_${.PASS.}_TAGS_:@.TAG.@${_SUBDIR_TAGS_}@}
_TARGET_TAGS_=${_PASS_${.PASS.}_TAGS_:@.TAG.@${_${.ACTION.}_${.TAG.}_TARGETS_}@}
_PASS_ACTIONS_=${_${.ACTION.}_PASSES_:@.PASS.@${_SUBDIR_PASSES_} ${_TARGET_TAGS_}@}
_TARGET_ACTIONS_=${_${.ACTION.}_PASSES_:@.PASS.@${_TARGET_TAGS_}@}
_SUBDIR_ACTIONS_=${_${.ACTION.}_PASSES_:@.PASS.@${_SUBDIR_PASSES_}@}

_all_targets_=${_${.TARGET:S;_all$;;}_action_:@.ACTION.@${_PASS_ACTIONS_}@}

#
#  subdir recursion rule
#
#  This expands into targets matching the following pattern
#
#  _SUBDIR/<action>/<pass>/<tag>/<subdir>
#

# Makes depend on submakes so that submakes don't run in parallel
.MAKE: ${_ALL_ACTIONS_:@.ACTION.@${_SUBDIR_ACTIONS_}@}

# 
# If SUBMAKE is not defined, then define it before invoking make.  If SUBMAKE
# is set, then just pass it.  This has the effect of allowing any invocation
# of make to figure out if a parent make invoked it or if it is the initial
# invocation of make.  Make needs to know this in order to make the rules and
# targets that handle DATAFILES work when make is initially invoked in the
# directory in which DATAFILES is defined.
#
.if !defined(_SUBMAKE_)
_SUBMAKE_DEF_=_SUBMAKE_=_IS_SUBMAKE_
.endif

${_ALL_ACTIONS_:@.ACTION.@${_SUBDIR_ACTIONS_}@}:
.if defined(BENCHMARK_BUILD)
	@echo "[ ${MAKEDIR:/=}/${.TARGET:T:S;+++;/;g} at `date +'%H:%M (%p) %A'` ]"
.else
	@echo "[ ${MAKEDIR:/=}/${.TARGET:T:S;+++;/;g} ]"
.endif
	makepath ${.TARGET:T:S;+++;/;g}/. && cd ${.TARGET:T:S;+++;/;g} && \
	exec ${MAKE} MAKEFILE_PASS=${.TARGET:H:H:T} \
	     ${_SUBMAKE_DEF_} ${SUBMAKE_FLAGS} \
	      ${_${.TARGET:H:H:H:T}_ACTION_}_all

#
# For each action that has more the one pass, exec a make for each pass
# assigning MAKEFILE_PASS to the appropriate value.  This solves the problem
# of the pmake trying to build items in the next pass before all the pieces
# of the previous passes are done.
#
# Currently only BUILD and EXPORT actions span more then on pass.
#
_INDIRECT_ACTIONS_=BUILD EXPORT

_INDIRECT_PASSES_=${_${.ACTION.}_PASS_LIST_:@.PASS.@_INDIRECT/${.ACTION.}/${.PASS.}@}

${_INDIRECT_ACTIONS_:S,^,_INDIRECT/,g}: $${.TARGET:T:@.ACTION.@$${_INDIRECT_PASSES_}@}


.if !empty(_AUTOGEN_NEEDED_:M*)
AUTOGEN_NEEDED=AUTOGEN
.endif

.if !empty(_FIRST_NEEDED_:M*)
FIRST_NEEDED=FIRST
.endif

.if !empty(_FIRSTA_NEEDED_:M*)
FIRSTA_NEEDED=FIRSTA
.endif

.if !empty(_THIRD_NEEDED_:M*)
THIRD_NEEDED=THIRD
.endif

.if !empty(_BASIC_NEEDED_:M*)
BASIC_NEEDED=BASIC
.endif

#
# Run only one make at a time
#
.MAKE: ${_INDIRECT_ACTIONS_:@.ACTION.@${_INDIRECT_PASSES_}@}

${_INDIRECT_ACTIONS_:@.ACTION.@${_INDIRECT_PASSES_}@}:
	exec ${MAKE} MAKEFILE_PASS=${.TARGET:T} \
	     ${_SUBMAKE_DEF_} \
	      ${_${.TARGET:H:T}_ACTION_}_all

#
#  Special tag passes
#
.if !defined(EXPORTBASE)

export_all_SAMIDL: ${ALWAYS}
	@echo "You must define EXPORTBASE to do an ${.TARGET}"

export_all_EXPINC: ${ALWAYS}
	@echo "You must define EXPORTBASE to do an ${.TARGET}"

export_all_EXPLIB: ${ALWAYS}
	@echo "You must define EXPORTBASE to do an ${.TARGET}"

.elif defined(MAKEFILE_PASS)

_W=${AUTOGEN:L:@.PASS.@${_SUBDIR_PASSES_} ${_TARGET_TAGS_}@}
_SAMIDL_TARGET_TAGS=${_${.ACTION.}_SAMIDL_TARGETS_}
_X=${FIRST:L:@.PASS.@${_SUBDIR_PASSES_} ${_TARGET_TAGS_}@}
_EXPLIB_TARGET_TAGS_=${_${.ACTION.}_EXPLIB_TARGETS_}
_Y=${THIRD:L:@.PASS.@${_SUBDIR_PASSES_}@} ${_EXPLIB_TARGET_TAGS_}
_EXPSHLIB_TARGET_TAGS_=${_${.ACTION.}_EXPSHLIB_TARGETS_}
_Z=${THIRD:L:@.PASS.@${_SUBDIR_PASSES_}@} ${_EXPSHLIB_TARGET_TAGS_}
samidl_all export_all_SAMIDL  : ${EXPORT:L:@.ACTION.@${_W}@}
export_all_EXPINC  : ${EXPORT:L:@.ACTION.@${_X}@}
export_all_EXPLIB  : ${EXPORT:L:@.ACTION.@${_Y}@}
export_all_EXPSHLIB: ${EXPORT:L:@.ACTION.@${_Z}@}

.else

samidl_all :  _INDIRECT/BUILD/AUTOGEN
export_all_SAMIDL  : _INDIRECT/EXPORT/AUTOGEN
export_all_EXPINC  : _INDIRECT/EXPORT/FIRST
export_all_EXPLIB  : _INDIRECT/EXPORT/THIRD
export_all_EXPSHLIB: _INDIRECT/EXPORT/THIRD

.endif

#
# End of PASSES
#

.if defined(ODE_UNDER_CLEARCASE)

cpdata_all: $${_all_targets_}

.if !empty(_CPDATA_TARGETS_)

${_CPDATA_TARGETS_}:
	${RM} ${_RMFLAGS_} ${.TARGET}
	${CP} ${${.TARGET}:Q} ${.TARGET}
.endif
.else
#
# rules to handle DATAFILES
mvdata_all: $${_all_targets_}

.if !empty(_MVDATA_TARGETS_) 

#
# Move the files specified by the DATAFILES veriable that exist in the
# current directory [in the object tree] to <file>.datachk.
#
${_MVDATA_TARGETS_}:
	@if [ -f ${.TARGET:S/.local//} ];  then \
		${MV} ${.TARGET:S/.local//} ${.TARGET:S/.local/.datachk/} > /dev/null 2>&1; \
	fi
.endif

chkdata_all: $${_all_targets_}

.if !empty(_CHKDATA_TARGETS_)

.if ( !defined(_SUBMAKE_) && !defined(__DIDDATA__) )
${CHKDATA_TARGETS}: _DONT_EXIST

_DONT_EXIST:
	@exec ${MAKE} MAKEFILE_PASS=FIRSTB __DIDDATA__=DIDDATA build_all
.else
${CHKDATA_TARGETS}: $${.TARGET:S/.datachk//}
	${RM} -f ${.TARGET}
	${CP} $> ${.TARGET}
.endif
.endif

${RESTORE_TARGETS}:
	${MV} ${.TARGET:S/.restore/.datachk/} ${.TARGET:S/.restore//}
.endif
.endif


print_passes_needed:
#	@echo AN: "x${_AUTOGEN_NEEDED_:M*}x"
#	@echo FN: "x${_FIRST_NEEDED_:M*}x"
#	@echo FAN: "x${_FIRSTA_NEEDED_:M*}x"
#	@echo TN: "x${_THIRD_NEEDED_:M*}x"
#	@echo BN: "x${_BASIC_NEEDED_:M*}x"
	@echo Passes needed: ${_BUILD_PASS_LIST_}

