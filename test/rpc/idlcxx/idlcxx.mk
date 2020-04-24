# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: idlcxx.mk,v $
# Revision 1.1.4.1  1996/09/09  20:58:59  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  19:48:14  bfc]
#
# Revision 1.1.2.3  1996/03/09  20:48:47  marty
# 	Add OSF copyright
# 	[1996/03/09  20:27:20  marty]
# 
# Revision 1.1.2.2  1996/02/23  19:13:23  root
# 	OT 13362, fix for IBM C++ suffix
# 	[1996/02/23  19:11:43  root]
# 
# Revision 1.1.2.1  1995/12/11  19:34:16  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:18:01  root]
# 
# $EndLog$

.if defined(IDLFILES)
${IDLFILES:.idl=.h}: \
		$${.TARGET:.h=.idl} \
		$${$${.TARGET:.h=}_ACF:D$${.TARGET:.h=.acf}}
	-${RM} ${_RMFLAGS_} ${.TARGET}
	${IDL} -keep none -header ${.TARGET} ${_IDLFLAGS_} ${${.TARGET:.h=.idl}:P}

${IDLFILES:.idl=_cstub.C}: \
		$${.TARGET:_cstub.C=.idl} \
		$${.TARGET:_cstub.C=.h} \
		$${$${.TARGET:_cstub.C=}_ACF:D$${.TARGET:_cstub.C=.acf}}
	-${RM} ${_RMFLAGS_} ${.TARGET:_cstub.C=_cstub.C}
	${IDL} ${_IDLFLAGS_} -server none -keep both -header /tmp/${.TARGET:_cstub.C=.h} ${${.TARGET:_cstub.C=.idl}:P}
	-${RM} ${_RMFLAGS_} /tmp/${.TARGET:_cstub.C=.h}

${IDLFILES:.idl=_sstub.C}: \
		$${.TARGET:_sstub.C=.idl} \
		$${.TARGET:_sstub.C=.h} \
		$${$${.TARGET:_sstub.C=}_ACF:D$${.TARGET:_sstub.C=.acf}}
	-${RM} ${_RMFLAGS_} ${.TARGET:_sstub.C=_sstub.C}
	${IDL} ${_IDLFLAGS_} -client none -keep both -header /tmp/${.TARGET:_sstub.C=.h} ${${.TARGET:_sstub.C=.idl}:P}
	-${RM} ${_RMFLAGS_} /tmp/${.TARGET:_sstub.C=.h}

${IDLFILES:.idl=_caux.C}: 
	rm -f ${.TARGET}
	echo "static int idl_aux_stub = 0;" >${.TARGET}

${IDLFILES:.idl=_saux.C}:
	rm -f ${.TARGET}
	echo "static int idl_aux_stub = 0;" >${.TARGET}

.if ${TARGET_MACHINE} == "RIOS"
# With TET builds
CFLAGS			+= -DTET -DIDL_CHAR_IS_CHAR
IDLFLAGS	+=	-lang cxx -v -cc_opt "-DTET -DIDL_CHAR_IS_CHAR -DAIX32"
RIOS_LIBS	+=	-lidlcxx -ldce -ltetapi -ltettcm -ltetapi
# No TET builds
# CFLAGS			+= -DIDL_CHAR_IS_CHAR
# IDLFLAGS	+=	-lang cxx -v -cc_opt "-DIDL_CHAR_IS_CHAR -DAIX32"
# RIOS_LIBS	+=	-lidlcxx -ldce 

.elif ${TARGET_MACHINE} == "ALPHA"
ALPHA_LIBS              := -lidlcxx -ldce -lpthreads -lc_r -lmach -lm
CFLAGS		+=	-DTET -DPTHREAD_USE_D4 -D_REENTRANT -threads -O0 -DIDL_CHAR_IS_CHAR
IDLFLAGS	+=	-lang cxx -v -cc_opt "-threads -DIDL_CHAR_IS_CHAR"
ALPHA_LIBS		:= -lidlcxx -ldce -ltetapi -ltettcm -ltetapi -threads -lc -lmach -lm
.endif

LIBDIRS			+= ${SHLIBDIRS}
INCDIRS		+= ${BACKED_SOURCEDIR:S/\:/ /g:S/^/-I/g:S/$/\/test\/rpc\/idlcxx/g}
.endif
