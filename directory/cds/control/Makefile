# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#  COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991. ALL RIGHTS RESERVED.
#
#  THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
#  ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
#  INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
#  COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
#  OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
#  TRANSFERRED.
#
#  THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
#  AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
#  CORPORATION.
#
#  DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
#  SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.15.2  1996/03/09  23:11:40  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:59  marty]
#
# Revision 1.2.15.1  1995/12/08  15:06:09  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:39:08  root]
# 
# Revision 1.2.13.15  1994/06/23  18:29:27  mccann
# 	sams cleanup drop1
# 	[1994/06/22  19:33:03  mccann]
# 
# Revision 1.2.13.14  1994/06/09  16:08:14  devsrc
# 	cr10892 - fix copyright
# 	[1994/06/09  15:53:00  devsrc]
# 
# Revision 1.2.13.13  1994/05/20  22:03:12  sommerfeld
# 	Generate one file at a time with PARSER_AID
# 	Fix build of cds.i to use ${_GENINC_} instead of backquoted genpath
# 	invocation.
# 	[1994/05/20  21:30:55  sommerfeld]
# 
# 	Generate one file at a time with PARSER_AID
# 	Fix build of cds.i to use ${_GENINC_} instead of backquoted genpath
# 	invocation.
# 
# Revision 1.2.13.12  1994/04/19  17:05:21  mccann
# 	More code cleanup
# 	[1994/04/19  16:25:17  mccann]
# 
# Revision 1.2.13.11  1994/03/24  18:49:22  jd
# 	parser_aid -> $(PARSER_AID).
# 	[1994/03/24  18:39:55  jd]
# 
# Revision 1.2.13.10  1994/03/23  19:43:07  jd
# 	replace the call to prs with $(PRS). $(PRS) get made into $DEFTOOLBASE/prs
# 	by the common makefiles. prs sometimes get found in /bin or /usr/bin and
# 	it not the correct prs.
# 	[1994/03/23  19:37:31  jd]
# 
# Revision 1.2.13.9  1994/03/23  14:27:39  jd
# 	Put SETUP_PROGRAM=prs back.
# 	[1994/03/23  14:21:43  jd]
# 
# Revision 1.2.13.8  1994/03/23  13:36:54  jd
# 	Removed extra set of log entries.
# 	[1994/03/23  13:31:55  jd]
# 
# Revision 1.2.13.7  1994/03/23  13:01:23  jd
# 	Change cdsprs back to prs again. It got dropped in the H-Cell
# 	submission.
# 	[1994/03/23  12:38:11  jd]
# 
# Revision 1.2.13.6  1994/03/22  19:59:52  griffin
# 	HCell BL3 support
# 	[1994/03/22  17:43:17  griffin]
# 
# Revision 1.2.13.5  1994/03/15  22:02:04  jd
# 	Put cdsprs back to prs.
# 	[1994/03/15  21:55:18  jd]
# 
# Revision 1.2.13.4  1994/03/15  15:42:01  jd
# 	Remove redundant rule for cds.i.
# 	[1994/03/15  14:31:25  jd]
# 
# Revision 1.2.13.3  1994/03/12  21:57:22  peckham
# 	DEC serviceability and i18n drop
# 	[1994/03/12  13:55:06  peckham]
# 
# Revision 1.2.13.2  1993/12/07  17:54:30  mario
# 	Changed for ode2.3 - < and > becomes this except for RULES_MK
# 	[1993/12/07  17:25:19  mario]
# 
# Revision 1.2.9.3  1993/07/20  20:23:17  jd
# 	Moved paser_aid_LIBS definition for hp800 up to cds.mk. It broke the
# 	osf1 build on the 486.
# 	[1993/07/20  20:19:32  jd]
# 
# Revision 1.2.9.2  1993/07/19  19:27:41  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:21:08  zeliff]
# 
# Revision 1.2.7.3  1993/07/16  19:00:12  kissel
# 	Initial submission for HP RP.
# 	[1993/06/21  21:14:42  kissel]
# 
# Revision 1.1.5.2  1993/03/18  19:17:55  smythe
# 	added -Wl,-a,archive to build parser_aid_LIBS static
# 	necessary to link proper backing build libraries
# 	[1993/03/18  19:17:16  smythe]
# 
# Revision 1.2.5.4  1992/12/30  12:43:43  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  22:29:59  zeliff]
# 
# Revision 1.2.5.3  1992/11/17  17:20:41  m_sawyer
# 	Remove defunct files [clr_cds.o control_c.o control_z.o tty_mode.o]
# 	[1992/11/17  16:08:11  m_sawyer]
# 
# Revision 1.2.5.2  1992/09/29  19:09:16  devsrc
# 	SNI/SVR4 merge.  OT 5373
# 	[1992/09/11  15:10:27  weisman]
# 
# Revision 1.2.2.5  1992/05/06  16:43:07  m_sawyer
# 	eliminated several defunct modules from the build and added a new one
# 	[1992/05/06  16:42:42  m_sawyer]
# 
# Revision 1.2.2.4  1992/04/30  21:02:42  john
# 	Added SETUP_PROGRAMS.
# 	[1992/04/10  14:32:53  john]
# 
# Revision 1.2.2.3  1992/04/29  17:00:25  m_sawyer
# 	remove unnecessary modules from cdscp build
# 	[1992/04/29  17:00:03  m_sawyer]
# 
# Revision 1.2.2.2  1992/03/22  21:36:55  weisman
# 		Big merge with post-1.0 development at Digital.
# 	[1992/03/22  20:11:26  weisman]
# 
# Revision 1.2  1992/01/19  22:12:55  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
# Makefile for building control program
#

VPATH		= ../stubs:../includes

# programs to build
PROGRAMS	= parser_aid prs cdscp
SETUP_PROGRAMS	= prs

# install directory
IDIR		= /bin/
cdscp.bpt_IDIR	= /etc/
ILIST		= cdscp cdscp.bpt


# list of libraries used to bind programs
cdscp_LIBS	= -lcdscache ${${TARGET_MACHINE}_LIBS}
parser_aid_LIBS	=
prs_LIBS	=


# compiler flags
CFLAGS		= ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS	= -I../stubs -I../includes

LIBFLAGS	= -L../cache

# other objects to build
OBJECTS		= cdscp.bpt

# List of objects to build cdscp

IDLFILES	= cds_clerkserver.idl cprpc_clerk.idl cprpc_server.idl \
		  cds_types.idl cprpc_hcell.idl
cprpc_clerk_ACF =
cprpc_server_ACF =
cds_types_ACF =
cprpc_hcell_ACF =

cdscp_files = \
	mgmt_main.o \
	accum_cmd.o \
	argv.o \
	attrgroup_actions.o \
	attributes_list.o \
	attrname_actions_cds.o \
	baddest.o \
	cds_clerkserver_cstub.o \
	cds_types_caux.o \
	cdscp_help.o \
	cmdblk_init.o \
	confidence.o \
	cp_cell.o \
	cprpc_clerk.o \
	cprpc_clerk_cstub.o \
	cprpc_emit_clerk.o \
	cprpc_emit_server.o \
	cprpc_hcell_cstub.o \
	cprpc_server.o \
	cprpc_server_cstub.o \
	cprpc_util.o \
	default_ent.o \
	dnscp_args.o \
	dnscp_credir.o \
	dnscp_crekid.o \
	dnscp_creobj.o \
	dnscp_cresl.o \
	dnscp_delkid.o \
	dnscp_delobj.o \
	dnscp_delsl.o \
	dnscp_replica.o \
	dnsinitflagstat.o \
	dumpclerkcachecmd.o \
	emit_cds_values.o \
	emit_group_line.o \
	emit_member_lines.o \
	emit_myname_line.o \
	emit_one_line.o \
	emit_ref_hdr.o \
	entities_actions_cds.o \
	entities_list.o \
	err_list.o \
	err_report.o \
	exec_docmd.o \
	exec_mgmt_cmd_cds.o \
	filespec_actions.o \
	fmt_uid_for_print.o \
	fopen2.o \
	format.o \
	format_chstate_for_print.o \
	format_classversion_for_print.o \
	format_confidence_for_print.o \
	format_convergence_for_print.o \
	format_fullname_for_print.o \
	format_inch_for_print.o \
	format_reptyp_for_print.o \
	format_simplename_for_print.o \
	format_tower.o \
	format_utcabs_for_print.o \
	get_command.o \
	load_parse_file.o \
	logging.o \
	msgfil.o \
	msgfnd.o \
	names_actions_cds.o \
	op_list.o \
	operator_actions.o \
	parse_command.o \
	parsing_support.o \
	preferred_clh.o \
	pretend_not_seen.o \
	process_commands.o \
	prscmd.o \
	prsrea.o \
	recognize_filespec.o \
	replica_actions.o \
	report_parse_error.o \
	savemajorminor.o \
	spaces.o \
	startup_file.o \
	time_actions.o \
	visit_cds.o \
	with.o

cdscp_OFILES = ${cdscp_files}

# List of objects to build prs
prs_OFILES = prs.o

# List of objects to build parser_aid
parser_aid_OFILES = parser_aid_2.o hash.o

.include <${RULES_MK}>

cdscpmsg.h : dcecdsmsg.h
	grep -v ' CDS_' $> >$@

${cdscp_files} ${pip_OFILES} : cdscp.h cdscpmsg.h

cds.i : cds.cdt
	$(CP) ${cds.cdt:P} cds.c
	chmod 644 cds.c
	$(_CC_) -E ${INCDIRS} ${CFLAGS} ${_GENINC_} cds.c >cds.i
	$(RM) cds.c

# Rule for creating cdscp.bpt
cdscp.bpt : prs cds.i
	$(PRS) cds.i
	$(MV) cds.bin cdscp.bpt

mgmt_com_act_decl.h : parser_aid cds.i
	$(PARSER_AID) mgmt_com_act_decl.h /dev/null cds.i

mgmt_com_act_init.h : parser_aid  cds.i
	$(PARSER_AID) /dev/null mgmt_com_act_init.h cds.i

parse_command.o : parse_command.c mgmt_com_act_decl.h mgmt_com_act_init.h

mgmt_main.o : mgmt_main.c mgmt_com_act_decl.h mgmt_com_act_init.h

GARBAGE = cds.i mgmt_com_act_decl.h mgmt_com_act_init.h \
          cds.cdt cprpc_clerk_cstub.c cprpc_server_cstub.c \
	  cds_clerkserver_cstub.c cds_types_caux.c cprpc_hcell_cstub.c
