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
# $Log: osf.obj.mk,v $
# Revision 1.1.27.1  1996/10/03  14:36:58  arvind
# 	Merge from sommerfeld_pk_kdc to aha_pk9
# 	[1996/08/13  21:33 UTC  aha  /main/aha_pk9/1]
#
# 	include hack to let you "lint" via GCC.
#
# 	this is something of a *large* hammer at the moment since there's a
# 	lot of code in the tree which isn't even *close* to passing gcc -Wall -Werror..
# 	[1996/06/20  03:37 UTC  sommerfeld  /main/sommerfeld_pk_kdc/1]
#
# Revision 1.1.25.2  1996/03/09  23:25:23  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:00  marty]
# 
# Revision 1.1.25.1  1995/12/07  22:10:24  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/jrr_1.2_mothra/1  1995/11/21  21:10 UTC  jrr
# 	Merge second XIDL drop for DCE 1.2.1
# 	[1995/11/17  17:00 UTC  dat  /main/dat_xidl2/1]
# 
# Revision 1.1.23.1  1994/07/26  20:52:01  annie
# 	added VISTA - 11163
# 	[1994/07/26  20:51:30  annie]
# 
# Revision 1.1.19.4  1993/09/22  20:25:57  zeliff
# 	Merging up dce1.0.3i drop changes and adding .s.o translation rule
# 	[1993/09/22  20:25:18  zeliff]
# 
# Revision 1.1.20.3  93/09/22  16:18:25  zeliff
# 	Moving to dce1.1i
# 
# Revision 1.1.20.2  1993/09/07  13:52:39  zeliff
# 	Adding .s.o rule
# 
# Revision 1.1.17.2  1993/06/10  19:19:06  sommerfeld
# 	Flush garbage typo from start of file
# 	[1993/06/10  18:42:32  sommerfeld]
# 
# 	Add rules to figure out whether we're building
# 	objects for a library or not.  If so, add +z
# 	to _CC_ to build PIC code.  (unfortunately,
# 	the rule doesn't discriminate between archive
# 	and shared libs, 'cause we have to use the
# 	same .o for both.
# 	[1992/07/29  19:54:14  hopkins]
# 
# 	Add NO_RECOMPILE option.
# 	Add .C.o rule for C++.
# 	[1993/04/05  16:33:12  sommerfe]
# 
# 	Add NO_RECOMPILE option.
# 	[1993/04/05  16:30:19  sommerfe]
# 
# Revision 1.1.5.5  1993/02/03  17:51:26  john
# 	Removed the ${MV} of the object files since none are created.
# 	[1993/02/03  16:12:58  john]
# 
# 	Added rules for .y.pp and .l.pp.  When generating preprocessor files the
# 	compiler will use the _PP_CCFLAGS_ variable instead of _CCFLAGS_ since
# 	_CCFLAGS_ does not get the target specific compiler options.
# 	[1993/02/03  15:49:19  john]
# 
# Revision 1.1.5.4  1993/01/29  20:23:13  john
# 	Fixed the typo.  Changed _RMFLAGS to _RMFLAGS_.
# 	[1993/01/29  19:42:05  john]
# 
# Revision 1.1.5.3  1992/12/31  16:47:35  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:51:36  zeliff]
# 
# Revision 1.1.5.2  1992/11/06  21:25:37  john
# 	Added NO_Y_H_RULE to eliminate the .y.h: rule when set in a Makefile.
# 	[1992/11/03  15:26:09  john]
# 
# 	Added Test for .y.o: rule
# 	[1992/10/30  19:15:49  john]
# 
# 	Added the CCMDFLAGS flag to the CC command.
# 	[1992/10/29  02:53:07  john]
# 
# 	Added CCMDFLAGS to the CC build rules.
# 	[1992/10/20  01:22:29  john]
# 
# Revision 1.1.2.3  1992/05/11  21:05:57  mhickey
# 	Replaced -f with _RMFLAGS_ in common makefiles.
# 	defect 2142.
# 	[1992/05/11  21:04:57  mhickey]
# 
# Revision 1.1.2.2  1992/04/16  14:32:39  marty
# 	Fix copyright marker and hardwired copyright notices
# 	[1992/04/16  14:29:58  marty]
# 
# Revision 1.1  1992/01/19  15:31:14  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#  The following History revisions does not exist under rcs control;
#  however the comments may be helpful is describing how this file
#  has changed over time.
#
#
# Revision 1.4.3.2  91/08/13  11:57:10  mhickey
# 	Added code to the .c.o rule to determine whether there is a backed up .h
# 	file generated from a .idl file during the export pass that needs to be
# 	restored to it's original name.  This is a fix for IBM defect 2135.
# 	The problem is that during the export pass, idl generates .h files in
# 	the obj tree that are exported.  When the other passes are run, some
# 	of these .h files are regenerated as a side effect of generating
# 	'C' files from the idl files.  These newly generated files may differ
# 	from the export versions; eg: if a .ACF file is defined for the
# 	resulting 'C' file.  If the export pass is run again, the NEWLY
# 	GENERATED .h is exported, since it is newer than the export tree
# 	version.  This is undesirable/dangerous.  The fix, implemented
# 	here and in osf.std.mk, is to move any existing .h file to a new
# 	name before doing the idl that generates the 'C' file, then move it
# 	back after compiling the file.
# 	[91/08/13  11:52:44  mhickey]
# 
# Revision 1.3  91/07/10  15:01:54  devrcs
# 	Adding dce.50
# 

.if !defined(_OSF_OBJ_MK_)
_OSF_OBJ_MK_=

#
# definitions for compilation
#
_ALL_OFILES_=${PROGRAMS:D${PROGRAMS:@.PROG.@${${.PROG.}_OFILES:U${.PROG.}.o}@}:U${OFILES}}
_OFILES_=${${.TARGET}_OFILES:U${OFILES:U${.TARGET}.o}}

#
#  Default double suffix compilation rules
#

.if ${TARGET_OS} == "HPUX"

.if ( defined(LIBRARIES) || defined(SHARED_LIBRARIES) || defined(EXPSHLIB_TARGETS) ) && !defined(NO_ZFLAG)

.if defined(PROGRAMS)

_POFILES_LIST_	:= ${PROGRAMS:S/^/{/g:S/$/_OFILES}/g:S/^/\$/g}
_NEED_ZFLAG_	= is_prog_ofile=false ;					\
		  for prog_ofile in ${_POFILES_LIST_} Not_An_Ofile ; do	\
			if [ "$$prog_ofile" = "${.TARGET}" ] ; then	\
				is_prog_ofile=true ;			\
				break ;					\
			fi ;						\
		  done ;						\
		  [ "$$is_prog_ofile" = "false" ] && echo "+z" ;	\
		  exit 0
			
_CC_OL_ += ${_NEED_ZFLAG_:!${_NEED_ZFLAG_}!}

.else

_CC_OL_ += +z

.endif	# defined(PROGRAMS)
.endif	# defined(LIBRARIES) || defined(SHARED_LIBRARIES)
.endif	# TARGET_OS == "HPUX"

.if !defined(NO_RECOMPILE)

.C.o:
	${_CC_} -c ${_CCPLUSPLUSFLAGS_} ${.IMPSRC}

.cxx.o:
	${_cplusplus_CC_} -c ${_CCPLUSPLUSFLAGS_} ${.IMPSRC}

.c.o:
.if defined(GCC_LINT)
	${GCC_LINT} -o /tmp/trash.o -c ${CCMDFLAGS} ${_CCFLAGS_} ${.IMPSRC}
.endif
	${_CC_} -c ${CCMDFLAGS} ${_CCFLAGS_} ${.IMPSRC}

.s.o:
	${AS} ${ASFLAGS} -o ${.TARGET} ${.IMPSRC}

.y.o:
	${YACC} ${_YFLAGS_} ${.IMPSRC}
	${_CC_} -c ${_CCFLAGS_} y.tab.c
	-${RM} ${_RMFLAGS_} y.tab.c
#
# rename the yacc map file if using vista
#
.if defined(VISTA)
	${MV} -f y.tab.M ${.TARGET:.o=.M}
.endif
	${MV} -f y.tab.o ${.TARGET}

.y.c:
	${YACC} ${_YFLAGS_} ${.IMPSRC}
	${MV} -f y.tab.c ${.TARGET}
	${RM} ${_RMFLAGS_} y.tab.h

.if	!defined(NO_Y_H_RULE)
.y.h:
	${YACC} -d ${_YFLAGS_} ${.IMPSRC}
	${MV} -f y.tab.h ${.TARGET}
	${RM} ${_RMFLAGS_} y.tab.c
.endif

.l.o:
	${LEX} ${_LFLAGS_} ${.IMPSRC}
	${_CC_} -c ${_CCFLAGS_} lex.yy.c
	-${RM} ${_RMFLAGS_} lex.yy.c
#
# rename the lex map file if using vista
#
.if defined(VISTA)
	${MV} -f lex.yy.M ${.TARGET:.o=.M}
.endif
	${MV} -f lex.yy.o ${.TARGET}

.l.c:
	${LEX} ${_LFLAGS_} ${.IMPSRC}
	${MV} -f lex.yy.c ${.TARGET}

# these are the rules for generating preprocessor output.
.c.pp:
	${_CC_} -E ${_PP_CCFLAGS_} ${.IMPSRC} > ${.TARGET}

.y.pp:
	${YACC} ${_YFLAGS_} ${.IMPSRC}
	${_CC_} -E ${_PP_CCFLAGS_} y.tab.c > ${.TARGET}
	-${RM} ${_RMFLAGS_} y.tab.c

.l.pp:
	${LEX} ${_LFLAGS_} ${.IMPSRC}
	${_CC_} -E ${_PP_CCFLAGS_} lex.yy.c > ${.TARGET}
	-${RM} ${_RMFLAGS_} lex.yy.c

.if defined(OFILES) || defined(PROGRAMS)
${_ALL_OFILES_}: ${HFILES}
.endif

.endif	# defined(NO_RECOMPILE)

.endif	# !defined(_OSF_OBJ_MK_)
