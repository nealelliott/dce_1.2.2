# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.12.2  1996/03/09  23:11:46  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:04  marty]
#
# Revision 1.1.12.1  1995/12/08  15:13:53  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:41:50  root]
# 
# Revision 1.1.10.12  1994/08/16  18:27:49  jd
# 	drop from janet
# 	[1994/08/13  16:05:40  jd]
# 
# Revision 1.1.10.11  1994/08/03  19:01:52  mccann
# 	include file cleanup
# 	[1994/07/19  17:55:19  mccann]
# 
# Revision 1.1.10.10  1994/07/13  15:07:01  marty
# 	CR # 10017 - Set permissions on header files to 444 during install pass
# 	[1994/07/12  15:53:21  marty]
# 
# Revision 1.1.10.9  1994/06/30  19:15:20  mccann
# 	sams cleanup drop2
# 	[1994/06/30  18:36:25  mccann]
# 
# 	sams cleanup drop 2
# 
# Revision 1.1.10.8  1994/05/13  12:56:18  jd
# 	Removed reference to dnstables.h which got defuncted.
# 	[1994/05/13  12:54:21  jd]
# 
# Revision 1.1.10.7  1994/04/29  15:50:03  peckham
# 	Add deb_ascii.h to EXP_INCLUDES
# 	[1994/04/29  13:56:58  peckham]
# 
# Revision 1.1.10.6  1994/04/14  14:39:34  peckham
# 	Split decnet_types into machine dependent cds_basic_types.h
# 	Add ${TARGET_MACHINE} to VPATH.
# 	[1994/04/11  19:50:33  peckham]
# 
# Revision 1.1.10.5  1994/04/07  10:22:16  kevins
# 	Merged with changes from 1.1.10.4
# 	[1994/04/07  10:21:47  kevins]
# 
# 	CR 10304 Export some header files for cross component dependency issues
# 	[1994/04/07  10:18:00  kevins]
# 
# Revision 1.1.10.4  1994/04/04  20:06:18  peckham
# 	Remove sams -f flag
# 	[1994/04/04  17:57:55  peckham]
# 
# Revision 1.1.10.3  1994/03/14  20:26:08  jd
# 	Added SAMSFLAG=-f until we adjust cds.sam.
# 	[1994/03/14  20:23:17  jd]
# 
# Revision 1.1.10.2  1994/03/12  22:02:01  peckham
# 	DEC serviceability and i18n drop
# 	[1994/03/12  14:11:03  peckham]
# 
# Revision 1.1.10.1  1994/02/02  20:37:06  rousseau
# 	Export cprpc.h for CR 9865.
# 	[1994/02/02  20:36:11  rousseau]
# 
# Revision 1.1.8.1  1993/10/07  20:50:36  pwang
# 	Removed existing file in obj tree before overwrite them
# 	[OT#9053]
# 
# Revision 1.1.5.3  1992/12/30  13:27:51  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  22:45:02  zeliff]
# 
# Revision 1.1.5.2  1992/12/23  17:37:29  john
# 	Changed hardcoded gencat to the variable ${GENCAT}.
# 	[1992/12/23  17:20:16  john]
# 
# Revision 1.1.3.3  1992/04/07  19:07:03  mckeen
# 	Merged with mainline
# 	[1992/03/31  16:35:58  mckeen]
# 
# Revision 1.1.3.2  1992/03/22  22:00:00  weisman
# 		Big merge with post-1.0 development at Digital.
# 	[1992/03/22  20:25:09  weisman]
# 
# Revision 1.1  1992/01/19  15:17:16  devrcs
# 	Initial revision
# 
# $EndLog$
#
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
#
#  Makefile for local naming include files
#

VPATH = ${TARGET_MACHINE}

SAMSFILES	= cds.sams
SAMIDL_INCLUDES	= dcecdsmac.h dcecdssvc.h dcecdsmsg.h
SAMS_TARGETS	= ${SAMIDL_INCLUDES} dcecds.cat


# Directories to export and install into
EXPDIR              = /usr/include/dce/
dcecds.cat_EXPDIR   = /usr/lib/msg/
cds_attributes_EXPDIR = /etc/
cds_globalnames_EXPDIR = /etc/
IDIR                = /share/include/dce/
dcecds.cat_IDIR     = /nls/msg/en_US.ASCII/
cds_attributes_IDIR = /etc/
cds_globalnames_IDIR = /etc/

IDLFILES	    = dnsmessage.idl

IMODE		    = 444
ILIST               = ${IDLFILES} ${SAMS_TARGETS} \
		      dnsclerk.h cdsclerk.h \
		      dnsmessage.h  \
		      cds_attributes cds_globalnames

EXPINC_TARGETS	    = ${ILIST:S/^/export_/g} ${EXP_INCLUDES:S/^/export_/g} \
		       cds_attr_values.c dns_status_map.c

INCLUDES            = dnsclerk.h dnsmessage.h dnsmessage.idl dcecds.cat \
                      cds_attr_values.c \
                      cds_attributes cds_globalnames gda_common.h cdsclerk.h

EXP_INCLUDES         = attributes.h cprpcp.h rpc_cvt.h dnsmanpar.h  \
		       dns_record.h cprpc_util.h \
		       dnsclerk_cds.h cds_basic_types.h \
		       dns_config.h sys_time.h qarith.h \
		       server_ncl_defs.h deb_ascii.h gda_common.h \
		       x500_name_utils.h


#DATAFILES           = ${INCLUDES} cdsval.awk dns_status_map.awk

.include <${RULES_MK}>

# Rules for building and dnsmessage.idl

dnsmessage.idl : cds.sams cdssams.awk
	egrep '^(set|code)' ${cds.sams:P} | $(AWK) -f ${cdssams.awk:P} >$@

# Rules for building cds_attr_values.c

cds_attr_values.c : cdsval.awk cdsclerk.h
	$(AWK) -f ${cdsval.awk:P} ${cdsclerk.h:P} >$@

# Rules for building dns_status_map.c

dns_status_map.c : dns_status_map.awk dnsmessage.h
	$(ECHO) "/* Generated by ${dns_status_map.awk:P} */" >$@
	$(ECHO) '' >>$@
	$(AWK) -f ${dns_status_map.awk:P} ${dnsmessage.h:P} >>$@

# Rules for building ?ds_attributes

cds_attributes : cds_attributes.txt
	${RM} ${_RMFLAGS_} $@
	$(CP) ${cds_attributes.txt:P} $@

cds_globalnames : cds_globalnames.txt
	${RM} ${_RMFLAGS_} $@
	$(CP) ${cds_globalnames.txt:P} $@

dnsclerk.h : dnsclerk_cds.h
	${RM} ${_RMFLAGS_} $@
	$(CP) ${dnsclerk_cds.h:P} $@


# GARBAGE = dcecds.msf dnsmessage.idl


