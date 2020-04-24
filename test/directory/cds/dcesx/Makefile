# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.8.2  1996/03/11  01:45:03  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:15:48  marty]
#
# Revision 1.1.8.1  1995/12/08  22:09:46  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:44:15  root]
# 
# Revision 1.1.6.1  1993/09/24  22:14:00  pwang
# 	Removed IDIR to have dcesx installed in the corresponding position
# 	of install tree [OT#8689]
# 	[1993/09/24  22:13:20  pwang]
# 
# Revision 1.1.4.4  1993/04/07  11:54:19  jd
# 	Install dcesx in test/directory/cds/dcesx.
# 	[1993/04/05  18:19:38  jd]
# 
# Revision 1.1.4.3  1993/03/29  20:32:20  jd
# 	rios needed LIBS defined.
# 	[1993/03/08  14:27:55  jd]
# 
# 	Don't build makefile.dcesx.
# 	[1993/03/08  13:34:07  jd]
# 
# Revision 1.1.4.2  1993/01/12  18:29:13  bbelch
# 	Embedding copyright notice
# 	[1993/01/07  01:13:04  bbelch]
# 
# Revision 1.1.2.2  1992/06/08  19:57:56  weisman
# 		Original submission.
# 	[1992/06/08  19:23:50  weisman]
# 
# $EndLog$
#
# COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991. ALL RIGHTS RESERVED.
#
# THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
# ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
# INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
# COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
# OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
# TRANSFERRED.
#
# THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
# AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
# CORPORATION.
#
# DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
# SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
#
# Makefile for DCE system exerciser
#
# 27-aug-91     ouellette       original
# 07-feb-92     sawyer          fixup for build in cds sandbox
#


# dirs to search for source
VPATH		= ${TARGET_MACHINE}

# programs to build
PROGRAMS        = dcesx

# list of things to install
ILIST           =  \
                   dcesx0.idl \
                   dcesx1.idl \
                   dcesx2.idl \
                   dcesx3.idl \
                   dcesx4.idl \
                   dcesx5.idl \
                   dcesx6.idl \
                   dcesx7.idl \
                   dcesx8.idl \
                   dcesx9.idl \
                   dcesx0.acf \
                   dcesx1.acf \
                   dcesx2.acf \
                   dcesx3.acf \
                   dcesx4.acf \
                   dcesx5.acf \
                   dcesx6.acf \
                   dcesx7.acf \
                   dcesx8.acf \
                   dcesx9.acf \
                   dcesx.c \
		   dcesx



# libs for binding
#LIBS            = ${${TARGET_MACHINE}_LIBS} 
#LIBS            = ${${TARGET_MACHINE}_LIB_DCE} -ldce_conf ${${TARGET_MACHINE}_LIB_COM}
RIOS_LIBS= -lm -lbsd
LIBS     = -ldce ${${TARGET_MACHINE}_LIBS}

# flags for compiling
CFLAGS          = ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS        =

LDFLAGS		= ${${TARGET_MACHINE}_LDFLAGS}

#LIBFLAGS        =

# lists of objects for programs

IDLFLAGS =      -keep c_source -cepv -no_mepv
IDLFILES =      dcesx0.idl \
                dcesx1.idl \
                dcesx2.idl \
                dcesx3.idl \
                dcesx4.idl \
                dcesx5.idl \
                dcesx6.idl \
                dcesx7.idl \
                dcesx8.idl \
                dcesx9.idl

dcesx_OFILES =  dcesx0_cstub.o dcesx0_sstub.o   \
                dcesx1_cstub.o dcesx1_sstub.o   \
                dcesx2_cstub.o dcesx2_sstub.o   \
                dcesx3_cstub.o dcesx3_sstub.o   \
                dcesx4_cstub.o dcesx4_sstub.o   \
                dcesx5_cstub.o dcesx5_sstub.o   \
                dcesx6_cstub.o dcesx6_sstub.o   \
                dcesx7_cstub.o dcesx7_sstub.o   \
                dcesx8_cstub.o dcesx8_sstub.o   \
                dcesx9_cstub.o dcesx9_sstub.o   \
                dcesx.o

.include <${RULES_MK}>

dcesx.o :       dcesx0.h \
                dcesx1.h \
                dcesx2.h \
                dcesx3.h \
                dcesx4.h \
                dcesx5.h \
                dcesx6.h \
                dcesx7.h \
                dcesx8.h \
                dcesx9.h
