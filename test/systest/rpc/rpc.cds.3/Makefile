#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.9.2  1996/03/11  02:45:24  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:07:56  marty]
#
# Revision 1.1.9.1  1995/12/11  22:01:06  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1994/08/29  15:00 UTC  bissen
# 
# 	HP revision /main/HPDCE02/2  1994/08/04  19:46 UTC  bissen
# 	merge from HPDCE01
# 	[1995/12/11  20:59:58  root]
# 
# Revision 1.1.7.1  1994/06/10  20:49:16  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:19:08  devsrc]
# 
# 	HP revision /main/HPDCE01/1  1994/02/01  19:23  giza
# 	merge kk & hpdce01
# 
# Revision 1.1.4.3  1993/08/27  21:02:18  giza
# 	fixed IDIRs for TET
# 	[1993/08/27  21:01:47  giza]
# 
# Revision 1.1.4.2  1993/08/27  17:46:05  giza
# 	Minor build changes.
# 	[1993/08/27  17:45:12  giza]
# 
# Revision 1.1.1.6  1993/05/04  22:21:14  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.2.3  1993/03/12  18:55:12  mhickey
# 	Fixing bsubmit mystery meat
# 	[1993/03/12  18:50:18  mhickey]
# 
# Revision 1.1.1.2  1993/01/29  16:52:33  mhickey
# 	initial checkin for  the rpc.cds.3 system test
# 
# $EndLog$
#

#
# Makefile - specify environment and actions for building the rpc.cds.3 system
#            test programs.
#
# ASSOCIATION
#    This file is part of the rpc.cds.3 DCE System Test.
# 
#    The rpc.cds.3 system test corresponds to test rpc.cds.3 in the DCE1.0.2
#    Core Component System Test plan.  For more information on the operation
#    and goals of the rpc.sec.2 system test, see the test plan, or the README
#    file in this directory.
#
#
# FUNCTION
#    Specifies the compile flags and object file lists for an ODE build of
#    the rpc.cds.3 system test.
#
# CALLED BY
#    make
#
# CALLS INTO
#    Nothing.
#
# PREREQUISITES
#    Existence of the files that make up the rpc.cds.3 system test
#


VPATH=.

##########################################
# Default mix of flags for make         ##
##########################################
#
RPCCDS3_ALL_OPTS=

#######################################################
# ifdefs to turn on classes of optional code.        ##
#######################################################
#
# Compile in all optional functionality
#
.ifdef RPCCDS3_ALL_OPTS
  #
  # Server status reports
  #
  RPCCDS3_SRV_REPORTING=
  #
  # Automatic refreshing of identity before ticket expires
  #
  RPCCDS3_AUTO_REFRESH=
  #
  # Identity change to client or server principle
  #
  RPCCDS3_DO_LOGIN=
.endif

#
# Compile in code to workaround defects not yet fixed.
#
.ifdef RPCCDS3_ALL_WKARNDS
  #
  # Empty since no workarounds are currently applicable
  #
.endif

#
# Compile in code that anticipates future enhancements
#
.ifdef RPCCDS3_ALL_EXTS
  #
  # Anticipates driver enhancement to handle client exit
  #
  RPCCDS3_DRVR_HNDLS_SIGCHLD=
.endif

#
# Compile in all debug code and keep debug symbols in binary
#
.ifdef RPCCDS3_ALL_DEBUGS
  #
  # Debug printout of identity maintainence activity
  #
  RPCCDS3_ID_REFR_DEBUG=
  #
  # Keep debug symbols in binaries
  #
  RPCCDS3_KEEP_SYMBOLS=
  #
  # build data structure dumpers into the server for debugging
  #
  RPCCDS3_ADD_DUMP_ROUTINES=
.endif


##########################################
# Programs that make up the test        ##
##########################################
#
PROGRAMS = rpc.cds.3_srv rpc.cds.3_cli

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
SCRIPTS = rpc.cds.3_cds_conf.sh  rpc.cds.3_runtest.sh  rpc.cds.3_setup.sh \
          rpc.cds.3_cds_util.sh  rpc.cds.3_sec_conf.sh rpc.cds.3_chkcli.awk \
          rpc.cds.3_conf_util.sh  rpc.cds.3_sec_util.sh rpc.cds.3.conf \
          rpc.cds.3_gen_summ.awk  rpc.cds.3_gen_rep.awk rpc.cds.3_sec_addkey.sh

#########################
# Calendar data files  ##
#########################
#
CALENDARS = rpc.cds.3_cal1.data rpc.cds.3_cal2.data rpc.cds.3_cal3.data

##########################
# List of installables  ##
##########################
#
rpc.cds.3_srv_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_cli_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_cds_conf.sh_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_runtest.sh_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_setup.sh_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_cds_util.sh_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_sec_conf.sh_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_chkcli.awk_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_conf_util.sh_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_sec_util.sh_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3.conf_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_gen_summ.awk_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_gen_rep.awk_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_sec_addkey.sh_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_cal1.data_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_cal2.data_IDIR = /tet/rpc/ts/rpc.cds.3/
rpc.cds.3_cal3.data_IDIR = /tet/rpc/ts/rpc.cds.3/

ILIST = ${PROGRAMS} ${SCRIPTS} ${CALENDARS}
IDIR = /test/tet/system/rpc/ts/rpc.cds.3/

#######################################################
# lists of OFILES that make up the various programs  ##
#######################################################
#
.ORDER: rpc_cds_3_cstub.o rpc.cds.3_cmain.o 
.ORDER: rpc_cds_3_sstub.o rpc.cds.3_smain.o 

#
# General utility files
#
RPC_CDS3_UTILS = \
    chk_stat.o  rdconf.o rdconf_err_str.o rpc.cds.3_err_str.o \
    dump_errs.o rpc.cds.3_parse_cal_spec.o \
    rpc.cds.3_set_cal_uuid.o maintain_id.o dump_key.o \
    sig_hndlr.o dump_uuid_vec.o

#
# Some routines that are useful for inspecting data in the debugger
# Note: these are requested specifically by the clients so are always built
# for the client binary.
#
RPC_CDS3_DEBUG_UTILS =  \
  dump_uuid.o  dump_binding.o

#
# Client specific utilities
#
RPC_CDS3_CLI_UTILS = ${RPC_CDS3_UTILS} rep_timer.o \
                     ${RPC_CDS3_DEBUG_UTILS}

#
# Server object files list
#
rpc.cds.3_srv_OFILES = rpc_cds_3_sstub.o rpc.cds.3_smain.o ${RPC_CDS3_UTILS}

#
# If server reporting is requested, then add the report timer object
#
.ifdef RPCCDS3_SRV_REPORTING
  rpc.cds.3_srv_OFILES += rep_timer.o
.endif 

#
# if we will be debugging the server and want to look at bindings and uuids
# add the dumper routines
#
.ifdef RPCCDS3_ADD_DUMP_ROUTINES
  rpc.cds.3_srv_OFILES += ${RPC_CDS3_DEBUG_UTILS}
.endif

#
# Client object files list
#
rpc.cds.3_cli_OFILES = rpc_cds_3_cstub.o rpc.cds.3_cmain.o \
                       ${RPC_CDS3_CLI_UTILS}


################################################################
# make parse-time conditionals to get the right compile time  ##
# switches set for various optional pieces of functionality.  ##
################################################################
#
CFLAGS +=  -DIDL_PROTOTYPES

#
# Compile in server report generation code
#
.ifdef RPCCDS3_SRV_REPORTING
  CFLAGS += -DRPCCDS3_SRV_REPORTING
.endif

#
# Compile in id maintainence activity printouts.  Note that this has 
# no effect if RPCCDS3_AUTO_REFRESH is not specified, because there will
# be no id refresh thread spawned in that case.
#
.ifdef RPCCDS3_ID_REFR_DEBUG
  CFLAGS += -DRPCCDS3_ID_REFR_DEBUG
.endif

#
# Compile in code to spawn a thread to automatically refresh the server ticket
# before it expires.
#
.ifdef RPCCDS3_AUTO_REFRESH
  CFLAGS += -DRPCCDS3_AUTO_REFRESH
.endif

#
# Compile in code to change identity to the client or server principle
#
.ifdef RPCCDS3_DO_LOGIN
  CFLAGS += -DRPCCDS3_DO_LOGIN
.endif

#
# Compile in code to initialize the comm status variable to rpc_s_ok
# before each call since the rpc runtime does not currently do it.  This
# means that once you get an error returned in the comm status variable
# you will always get it.
#
.ifdef RPCCDS3_COMM_STAT_WKARND
  CFLAGS += -DRPCCDS3_COMM_STAT_WKARND
.endif

#
# Tell the compiler/loader to keep debug symbols in the binary
#
.ifdef RPCCDS3_KEEP_SYMBOLS
  CFLAGS += -g
  rpc.cds.3_cli_LDFLAGS = -g
  rpc.cds.3_srv_LDFLAGS = -g
  rpc_cds_3_cstub.o_IDLFLAGS = -g
  rpc_cds_3_sstub.o_IDLFLAGS = -g
.endif


########################
# the idl files defs. ##
########################
#
IDLFILES = rpc_cds_3.idl
rpc_cds_3_ACF=

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
# Most of the routines depend on rpc.cds.3_defs.h for macro, return value,
# and data structure definitions.  Some of the routines also use the definition
# of the calendar data type defined in rpc_cds_3.h, which is included from 
# rpc.cds.3_defs.h.  Because of the way make does lookups for sources, it is 
# not valid to say rpc.cds.3_defs.h : rpc_cds_3.h.  Instead, each target that
# depends on rpc.cds.3_defs.h must also depend on rpc_cds_3.h
# 
rpc.cds.3_cmain.o rpc.cds.3_smain.o : rpc_cds_3.h rpc.cds.3_defs.h
chk_stat.o dump_errs.o : rpc_cds_3.h rpc.cds.3_defs.h
rpc.cds.3_parse_cal_spec.o rpc.cds.3_set_cal_uuid.o : rpc_cds_3.h \
                                                      rpc.cds.3_defs.h
maintain_id.o rep_timer.o : rpc_cds_3.h rpc.cds.3_defs.h
rdconf.o : rpc_cds_3.h rpc.cds.3_defs.h


