# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
# All Rights Reserved
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.18.2  1996/03/09  23:11:58  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:15  marty]
#
# Revision 1.2.18.1  1995/12/08  15:19:48  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:44:22  root]
# 
# Revision 1.2.15.13  1994/09/06  17:37:04  proulx
# 	fix timestamps
# 	[1994/09/06  17:29:48  proulx]
# 
# 	fix timestamps
# 
# 	fix timestamps
# 
# Revision 1.2.15.12  1994/08/16  18:27:55  jd
# 	drop from janet
# 	[1994/08/13  16:06:27  jd]
# 
# Revision 1.2.15.11  1994/07/25  15:16:42  proulx
# 	Provide routine for correcting namespace entry
# 	[1994/07/25  13:52:46  proulx]
# 
# Revision 1.2.15.10  1994/06/30  19:16:07  mccann
# 	sams cleanup drop2
# 	[1994/06/30  18:57:37  mccann]
# 
# Revision 1.2.15.8  1994/05/20  22:03:19  sommerfeld
# 	remove last vestiges of rdaclif references.
# 	[1994/05/20  21:33:54  sommerfeld]
# 
# 	fix sams invocations.
# 	[1994/05/17  19:23:39  sommerfeld]
# 
# Revision 1.2.15.7  1994/04/29  15:51:13  peckham
# 	Change unix_mbx.o to cds_mbx.o
# 	Change dnsbind.o to cdsbind.o
# 	Add deb_ascii.o
# 	Eliminate dnstables.o
# 	Move private objects to libcdsutil.a
# 	[1994/04/29  13:58:34  peckham]
# 
# Revision 1.2.15.6  1994/04/04  20:06:26  peckham
# 	Remove sams -f flag
# 	[1994/04/04  17:58:00  peckham]
# 
# Revision 1.2.15.5  1994/03/15  22:08:40  jd
# 	Had to change the angle brackets in the .include line to double
# 	quotes. We shouldn't have to do this but...
# 	[1994/03/15  15:31:21  jd]
# 
# Revision 1.2.15.4  1994/03/14  21:38:29  jd
# 	SAMSFLAG didn't work. Added the option -f to sams invocation.
# 	[1994/03/14  21:32:40  jd]
# 
# Revision 1.2.15.3  1994/03/14  21:22:12  jd
# 	Add SAMSFLAGS=-f
# 	[1994/03/14  21:21:39  jd]
# 
# Revision 1.2.15.2  1994/03/12  22:05:08  peckham
# 	DEC serviceability and i18n drop
# 	[1994/03/12  14:22:39  peckham]
# 
# Revision 1.2.15.1  1993/12/08  03:54:03  mario
# 	Changes for using ode2.3 make
# 	[1993/12/08  03:53:40  mario]
# 
# Revision 1.2.13.1  1993/10/15  21:19:02  ohara
# 	expand the VPATH to use context, needed for ODE2.3 on SVR4
# 	[1993/10/15  16:12:51  ohara]
# 
# Revision 1.2.11.2  1993/05/24  20:41:36  cjd
# 	Submitting 102-dme port to 103i
# 	[1993/05/24  20:12:31  cjd]
# 
# Revision 1.2.9.2  1993/05/12  03:09:58  jd
# 	Initial 486 port.
# 	[1993/05/12  03:09:43  jd]
# 
# Revision 1.2.5.3  1992/12/30  13:52:31  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  23:04:47  zeliff]
# 
# Revision 1.2.5.2  1992/09/29  18:54:33  devsrc
# 	Converted back to the original way of doing business.  We can add the
# 	security option later.
# 	[1992/09/22  18:32:38  john]
# 
# 	SNI/SVR4 merge.  OT 5373
# 	[1992/09/11  15:13:47  weisman]
# 
# Revision 1.2.2.4  1992/05/11  17:50:02  mhickey
# 	Removed obsolete define of NEED_PROJECT_MAKE_DEFS
# 	and include of osf.dce.mk.  Fixes defect 2031.
# 	[1992/04/22  15:21:11  mhickey]
# 
# Revision 1.2.2.3  1992/03/27  18:24:09  zeliff
# 	Add rule for libcds.a.o.
# 	[1992/03/27  18:23:51  zeliff]
# 
# Revision 1.2.2.2  1992/03/22  22:13:59  weisman
# 		Big merge with post-1.0 development at Digital.
# 	[1992/03/22  20:29:44  weisman]
# 
# Revision 1.2  1992/01/19  22:12:57  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# 
#  OSF DCE Version 1.0 
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
# Makefile for local naming library
#

#
###########################################################
# define the following "globally" for the libdce.so build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#
# List of objects
libcds_OFILES	= \
	attribute_lib.o \
	library_svc.o \
	cdsbind.o \
	cdsopqtosimple.o \
	cdsstrings.o \
	cdsstrtosimple.o \
	cds_uuid_lib.o \
	deb_ascii.o \
	dcecdsmsg.o \
	dcecdssvc.o \
	dns_record.o \
	dns_record_lib.o \
	dnsaddattrvalue.o \
	dnsaddmbr.o \
	dnsaddreplica.o \
	dnsallowch.o \
	dnsattrval.o \
	dnscancel.o \
	dnsclearhandle.o \
	dnscmpcts.o \
	dnscmpfull.o \
	dnscmpsimple.o \
	dnscombine.o \
	dnscountsimple.o \
	dnscreatechild.o \
	dnscreatects.o \
	dnscreatedir.o \
	dnscreategrp.o \
	dnscreateobj.o \
	dnscreateslink.o \
	dnscvtcdsfull.o \
	dnscvtcdssimple.o \
	dnscvtobjid.o \
	dnscvtts.o \
	dnsdelchild.o \
	dnsdeldir.o \
	dnsdelgrp.o \
	dnsdelobj.o \
	dnsdelslink.o \
	dnsdisallowch.o \
	dnsdoupdate.o \
	dnsenumattr.o \
	dnsenumchild.o \
	dnsenumgrp.o \
	dnsenumobj.o \
	dnsenumslink.o \
	dnsexpandstrfull.o \
	dnsfreehandle.o \
	dnsgather.o \
	dnsgetattrent.o \
	dnsgethandle.o \
	dnsgetinfo.o \
	dnsgetmsg.o \
	dnsgetopqfulllen.o \
	dnsgetopqsimplelen.o \
	dnshandleattrcon.o \
	dnshandleattrmbr.o \
	dnshandleattrspc.o \
	dnshandlereplcon.o \
	dnshandlesetnam.o \
	dnslinkreplica.o \
	dnsmaxsize.o \
	dnsmodifyreplica.o \
	dnsnewepoch.o \
	dnsnormal.o \
	dnsread.o \
	dnsreadattrset.o \
	dnsreadattrvalue.o \
	dnsreadmbr.o \
	dnsreadreplica.o \
	dnsresolvename.o \
	dnsrmattr.o \
	dnsrmattrvalue.o \
	dnsrmmbr.o \
	dnsrmreplica.o \
	dnsscatter.o \
	dnssend.o \
	dnsskulkdir.o \
	dnsstatus.o \
	dnsstatusclerk.o \
	dnsstatusunix.o \
	dnstestattrvalue.o \
	dnstestmbr.o \
	dnsvalidate.o \
	dnswrite.o \
	formatts.o \
	tower_lib.o \
	validate_tower.o \
	dpeacl.o \
	dpeaclstore.o \
	dpeaclconvert.o \
	dpeaclaccess.o \
	prm_sec.o \
	unix_time.o \
	unix_qarith.o 

libcdsutil.a_OFILES = \
	cds_mbx.o \
	cds_rpcserver.o \
	coredefault.o \
	dnscfg.o \
	dnsdaemon.o \
	dnsmalloc.o \
	dnsuser.o \
	rpc_cvt.o \
	sec_init.o \
	string_lib.o \
	threads_lib.o \
	utl_string.o \
	utl_table_mgr.o \
	x500_name_utils.o \
	cds_old_sstub.o

libcds.a_OFILES = ${libcds_OFILES}

#
# The rest of the Makefile is "private" for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#
# The following definitions are for shared libraries:
#   OBJECTS must be defined to pull in the common rules for compilation
#   LIBS    must be defined to prevent -ldce from appearing on compile lines
#
OBJECTS	=
LIBS	=

# dirs to search for source
VPATH           = ${TARGET_OS}:${TARGET_MACHINE}:../stubs:${MAKETOP}/../export/${target_machine}/usr/include/dce:${MAKETOP}/../export/${context}/usr/include/dce

#
# Don't want to build any '.a' here if we are using shared libraries
#
.if ${USE_SHARED_LIBRARIES} == 0
  # libraries to build
  LIBRARIES	= libcds.a libcdsutil.a
  # things to install, utils is private
  ILIST = libcds.a
.else
  LIBRARIES	= libcdsutil.a
.endif

EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}

EXPSHLIB_TARGETS        = ${libcds_OFILES}

# install dir for libs & export dir for libs
EXPDIR		= /usr/lib/
IDIR		= /usr/lib/

# compiler flags
CFLAGS	        = ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS        = -I../stubs -I../includes

LDFLAGS		= ${${TARGET_MACHINE}_LDFLAGS}

IDLFILES        = cds_old.idl

.include <${RULES_MK}>

#
# build sams(1)
#
dcecdssvc.c: ../includes/cds.sams
	${SAMS} -o S $>
dcecdsmsg.c: ../includes/cds.sams
	${SAMS} -o t $>
.endif
# 'endif' for libdce library makefile




