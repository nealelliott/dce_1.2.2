#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.13.2  1996/03/11  02:46:22  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:18  marty]
#
# Revision 1.1.13.1  1995/12/11  22:43:38  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1994/08/29  15:09 UTC  bissen
# 	Need libc for getclock
# 
# 	HP revision /main/HPDCE02/2  1994/08/03  20:41 UTC  bissen
# 	merge from HPDCE01
# 
# 	HP revision /main/HPDCE01/1  1994/01/27  19:08  bissen
# 	merge kk and hpdce01
# 	[1995/12/11  22:20:27  root]
# 
# Revision 1.1.4.3  1993/08/27  21:05:21  giza
# 	fixed IDIRs for TET
# 	[1993/08/27  21:04:57  giza]
# 
# Revision 1.1.4.2  1993/08/25  20:01:06  bissen
# 	Made changes for TET
# 	[1993/08/25  20:00:18  bissen]
# 
# Revision 1.1.8.2  1993/08/16  14:02:51  eheller
# 	File tetified.
# 	[1993/07/23  17:40:50  eheller]
# 
# Revision 1.1.2.9  1993/03/12  04:06:51  mhickey
# 	Final checkin for comment fixes before final submit
# 	[1993/03/12  03:59:55  mhickey]
# 
# 	Fixed comments and added default build flags.
# 	[1993/03/10  21:29:18  mhickey]
# 
# Revision 1.1.2.8  1993/02/25  20:15:53  mhickey
# 	Removed makefile stuff for workaround for 7250, since it
# 	is no longer needed.
# 	[1993/02/25  18:47:26  mhickey]
# 
# 	Reorg-ed and cleaned up.
# 	[1993/02/19  21:24:51  mhickey]
# 
# Revision 1.1.2.6  1993/01/25  21:52:12  mhickey
# 	Added rpc.sec.2_signal_hndlr.o to the list of .o's for
# 	rpc.sec.2_srv
# 	[1993/01/25  21:46:56  mhickey]
# 
# 	Added rpc.sec.2_maintain_id to the utils list, and
# 	created a new list for client only utils (rpc.sec.2_rep_timer.c
# 	and rpc.sec.2_sig_hndlr.c).
# 	[1993/01/19  18:41:30  mhickey]
# 
# Revision 1.1.2.4  1993/01/13  18:36:21  mhickey
# 	Added rpc.sec.2_sec_addkey.sh to the list of scripts.
# 
# 	Added rpc.sec.2_dump_key.o to the utils list.
# 	[1993/01/13  18:34:31  mhickey]
# 
# Revision 1.1.4.3  1993/01/07  19:20:06  mhickey
# 	added rpc.sec.2_dump_key.o to list for ro rpc.sec.2_cmain.c
# 
# Revision 1.1.4.2  1992/12/10  16:17:49  mhickey
# 	Added rpc.sec.2_gen_summ.awk and rpc.sec.2_clichk.awk to the
# 	ILIST
# 
# Revision 1.1.2.2  1992/12/07  19:25:58  mhickey
# 	Initial check in of rpc.sec.2 Makefile.
# 
# 	Defect 5887
# 	[1992/12/07  19:25:36  mhickey]
# 
# $EndLog$
#

#
# Makefile - specify environment and actions for building the rpc.sec.2 system
#            test programs.
#
# ASSOCIATION
#    This file is part of the rpc.sec.2 DCE System Test.
# 
#    The rpc.sec.2 system test corresponds to test rpc.sec.2 in the DCE1.0.2
#    Core Component System Test plan.  For more information on the operation
#    and goals of the rpc.sec.2 system test, see the test plan, or the README
#    file in this directory.
#
#
# FUNCTION
#    Specifies the compile flags and object file lists for an ODE build of
#    the rpc.sec.2 system test.
#
# CALLED BY
#    make
#
# CALLS INTO
#    Nothing.
#
# PREREQUISITES
#    Existence of the files that make up the rpc.sec.2 system test
#

#
# We use some generic tool type modules from the rpc.cds.3 test, so
# point make at it as well
#
VPATH = .:../rpc.cds.3
INCFLAGS = -I../rpc.cds.3

##########################################
# Default mix of flags for make         ##
##########################################
#
RPCSEC2_ALL_OPTS=


#######################################################
# ifdefs to turn on classes of optional code.        ##
#######################################################
#
# Compile in all optional functionality
#
.ifdef RPCSEC2_ALL_OPTS
  #
  # Server status reports
  #
  RPCSEC2_SRV_REPORTING=
.endif

#
# Compile in code to workaround defects not yet fixed.
#
.ifdef RPCSEC2_ALL_WKARNDS
#
# Currently no workarounds
#
.endif

#
# Compile in code that anticipates future enhancements
#
.ifdef RPCSEC2_ALL_EXTS
  #
  # Anticipates driver enhancement to handle client exit
  #
  RPCSEC2_DRVR_HNDLS_SIGCHLD=
.endif

#
# Compile in all debug code and keep debug symbols in binary
#
.ifdef RPCSEC2_ALL_DEBUGS
  #
  # Debug printout of identity maintainence activity
  #
  MAINTAIN_ID_REFR_DEBUG=
  #
  # Keep debug symbols in binaries
  #
  RPCSEC2_KEEP_SYMBOLS=
  #
  # build data structure dumpers into the server for debugging
  #
  RPCSEC2_ADD_DUMP_ROUTINES=

.endif

##########################################
# Programs that make up the test        ##
##########################################
#
PROGRAMS = rpc.sec.2_srv rpc.sec.2_cli


############################################
# Libraries needed for external routines  ##
############################################
#
LIBS = -ldce -lc_r -lc

###########################################################################
# Scripts that setup for and run the test.  Note that rpc.sec.2.conf is  ##
# not REALLY a script, but for the purposes of exporting, etc, it can    ##
# be considered one.                                                     ##
###########################################################################
#
SCRIPTS = rpc.sec.2_cds_conf.sh  rpc.sec.2_runtest.sh  rpc.sec.2_setup.sh \
          rpc.sec.2_cds_util.sh  rpc.sec.2_sec_conf.sh rpc.sec.2_chkcli.awk\
          rpc.sec.2_conf_util.sh rpc.sec.2_sec_util.sh rpc.sec.2.conf \
          rpc.sec.2_gen_summ.awk rpc.sec.2_gen_rep.awk \
          rpc.sec.2_sec_addkey.sh rpc.sec.2_rgy_edit.cli.sh \
	  rpc.sec.2_rgy_edit.srv.sh dcerpsec

#
##########################
# List of installables  ##
##########################
#
dcerpsec_IDIR			= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2.conf_IDIR		= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_cds_conf.sh_IDIR	= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_cds_util.sh_IDIR	= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_chkcli.awk_IDIR	= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_conf_util.sh_IDIR	= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_gen_rep.awk_IDIR	= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_gen_summ.awk_IDIR	= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_rgy_edit.cli.sh_IDIR	= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_rgy_edit.srv.sh_IDIR	= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_runtest.sh_IDIR	= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_sec_addkey.sh_IDIR	= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_sec_conf.sh_IDIR	= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_sec_util.sh_IDIR	= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_setup.sh_IDIR		= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_srv_IDIR		= /tet/rpc/ts/rpc.sec.2/
rpc.sec.2_cli_IDIR              = /tet/rpc/ts/rpc.sec.2/

ILIST = ${PROGRAMS} ${SCRIPTS}

#######################################################
# lists of OFILES that make up the various programs  ##
#######################################################
#
.ORDER: rpc_sec_2_cstub.o rpc.sec.2_cmain.o 
.ORDER: rpc_sec_2_sstub.o rpc.sec.2_smain.o 

#
# General utility files
#
RPC_SEC2_UTILS = \
  chk_stat.o  rdconf.o rdconf_err_str.o rpc.sec.2_err_str.o \
  dump_errs.o dump_key.o maintain_id.o \
  sig_hndlr.o 


#
# Client specific utilities
#
RPC_SEC2_CLI_UTILS = ${RPC_SEC2_UTILS} rep_timer.o

#
# Some routines that are useful for inspecting data in the debugger
# Note: these are requested specifically by the clients so are always built
# for the client binary.
#
RPC_SEC2_DEBUG_UTILS =  \
  dump_uuid.o  dump_binding.o


#
# Server object files list
#
rpc.sec.2_srv_OFILES = rpc_sec_2_sstub.o rpc.sec.2_smain.o ${RPC_SEC2_UTILS}


#
# If server reporting is requested, then add the report timer object
#
.ifdef RPCSEC2_SRV_REPORTING
  rpc.sec.2_srv_OFILES += rep_timer.o
.endif 

#
# if we will be debugging the server and want to look at bindings and uuids
# add the dumper routines
#
.ifdef RPCSEC2_ADD_DUMP_ROUTINES
  rpc.sec.2_srv_OFILES += ${RPC_SEC2_DEBUG_UTILS}
.endif

#
# Client object files list
#
rpc.sec.2_cli_OFILES = rpc_sec_2_cstub.o rpc.sec.2_cmain.o \
                       ${RPC_SEC2_CLI_UTILS}

#
# if we will be debugging the server and want to look at bindings and uuids
# add the dumper routines
#
.ifdef RPCSEC2_ADD_DUMP_ROUTINES
  rpc.sec.2_cli_OFILES += ${RPC_SEC2_DEBUG_UTILS}
.endif


################################################################
# make parse-time conditionals to get the right compile time  ##
# switches set for various optional pieces of functionality.  ##
################################################################
#
CFLAGS +=  -DIDL_PROTOTYPES

#
# Compile in server report generation code
#
.ifdef RPCSEC2_SRV_REPORTING
  CFLAGS += -DRPCSEC2_SRV_REPORTING
.endif

#
# Compile in id maintainence activity printouts. 
#
.ifdef MAINTAIN_ID_REFR_DEBUG
  CFLAGS += -DMAINTAIN_ID_REFR_DEBUG
.endif

#
# Tell the compiler/loader to keep debug symbols in the binary
#
.ifdef RPCSEC2_KEEP_SYMBOLS
  CFLAGS += -g
  rpc.sec.2_cli_LDFLAGS = -g
  rpc.sec.2_srv_LDFLAGS = -g
  rpc_sec_2_cstub.o_IDLFLAGS = -g
  rpc_sec_2_sstub.o_IDLFLAGS = -g
.endif


########################
# the idl files defs. ##
########################
#
IDLFILES = rpc_sec_2.idl
rpc_sec_2_ACF=

###############################################################
# Need the -keep c_source flag since we have no .idl.o: rule ##
###############################################################
#
IDLFLAGS = -keep c_source

#################################
# include the common makefiles ##
#################################
#
.include <${RULES_MK}>

########################
# local dependencies  ##
########################
#
rpc_sec_2_sstub.o rpc_sec_2_cstub.o: rpc_sec_2.idl
rpc.sec.2_cmain.o rpc.sec.2_smain.o: rpc_sec_2.h rpc.sec.2_defs.h


