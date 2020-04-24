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
# Revision 1.1.7.2  1996/10/03  18:38:10  arvind
# 	Merge test changes/fixes back to mainline
# 	[1996/08/05  17:58 UTC  gaz  /main/DCE_1.2.2/1]
#
# 	change "DATAFILES" to "SCRIPTS" so ODE sets execute permissions
# 	[1996/08/02  14:20 UTC  gaz  /main/gaz_dce1.2/2]
#
# 	add dependency on ${IDLFILES} to dtapi_utils.o
# 	[1996/08/01  20:30 UTC  gaz  /main/gaz_dce1.2/1]
#
# 	Merge in all changes to dced tests for new ACLs.
# 	[1996/01/15  22:55 UTC  jss  /main/HPDCE02/jss_dced_acl/1]
#
# 	Added setup.sh and cleanup.sh for dced tests.
# 	[1995/04/06  18:14 UTC  mothra  /main/HPDCE02/1]
#
# 	add INCLUDES
# 	[1994/09/30  21:15:42  baum  1.1.2.2]
#
# Revision 1.1.7.1  1996/08/09  12:14:26  arvind
# 	Merge test changes/fixes back to mainline
# 	[1996/08/02  14:20 UTC  gaz  /main/gaz_dce1.2/2]
# 
# 	add dependency on ${IDLFILES} to dtapi_utils.o
# 	[1996/08/01  20:30 UTC  gaz  /main/gaz_dce1.2/1]
# 
# 	Merge in all changes to dced tests for new ACLs.
# 	[1996/01/15  22:55 UTC  jss  /main/HPDCE02/jss_dced_acl/1]
# 
# 	Added setup.sh and cleanup.sh for dced tests.
# 	[1995/04/06  18:14 UTC  mothra  /main/HPDCE02/1]
# 
# 	add INCLUDES
# 	[1994/09/30  21:15:42  baum  1.1.2.2]
# 
# Revision 1.1.5.2  1996/03/11  02:29:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:37:19  marty]
# 
# Revision 1.1.5.1  1995/12/11  15:17:51  root
# 	Submit
# 	[1995/12/11  14:32:58  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:18:09  root]
# 
# Revision 1.1.2.2  1994/09/30  21:15:42  baum
# 	New secval tests plus the ability to run without TET (OT 11975)
# 	[1994/09/30  21:14:42  baum]
# 
# Revision 1.1.2.1  1994/09/28  21:47:31  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:15:14  melman]
# 
# Revision 1.1.2.5  1994/09/28  17:36:00  baum
# 	Add debug falg for more output
# 	[1994/09/28  17:35:44  baum]
# 
# Revision 1.1.2.4  1994/07/20  19:57:35  ganni
# 	introduce the tests for srvrconf and srvrexec services
# 	[1994/07/20  19:33:14  ganni]
# 
# Revision 1.1.2.3  1994/06/17  18:44:41  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:32:06  devsrc]
# 
# Revision 1.1.2.2  1994/06/03  14:05:32  ganni
# 	create the principal needed for the tests through scripts
# 	[1994/06/03  14:03:14  ganni]
# 
# Revision 1.1.2.1  1994/05/18  21:21:06  ganni
# 	Initial Version
# 	[1994/05/18  21:11:09  ganni]
# 
# $EndLog$
#
#
#


# Compiler flags for this directory
.ifdef _NO_TET
CFLAGS = ${${TARGET_MACHINE}_CFLAGS} -DDEBUG -D_NO_TET
STANDARD_TETLIBS= 
NON_TET_MAIN=main.o
.else
CFLAGS = ${${TARGET_MACHINE}_CFLAGS} -DDEBUG
STANDARD_TETLIBS= -ltettcm -ltetapi
NON_TET_MAIN=
.endif

INCFLAGS =

# Programs to build
PROGRAMS = 	\
		secval_start     \
		secval_stop      \
		secval_validate  \
		secval_status  \
		rkeytab_create   \
		rkeytab_delete   \
		rkeytab_add_key  \
		rkeytab_change_key \
		rkeytab_inq_key \
		rkeytab_remove_key \
		hostdata_create \
		hostdata_read \
		hostdata_write \
		hostdata_delete \
		binding_create \
		binding_set_auth \
		binding_free \
		binding_fm_rpc \
		list_release \
		list_get \
		inq_name \
		inq_id \
		entry_add \
		entry_get_next \
		entry_remove \
		object_read \
		object_release \
		object_read_all \
		server_create \
		server_mod_attr \
		server_delete \
		dced_test_server \
		server_start \
		server_enable_if \
		server_disable_if \
		server_stop

SCRIPTS	= secval_setup.sh key_setup.sh hacl_setup.sh hacl_reset.sh \
		  setup.sh cleanup.sh

dced_test_server_OFILES      = dtapi_1_srvr_sstub.o dced_test_server.o

server_start_OFILES	= dtapi_1_srvr_cstub.o server_start.o dtapi_srvr_utils.o dtapi_utils.o ${NON_TET_MAIN}
server_stop_OFILES	= dtapi_1_srvr_cstub.o server_stop.o dtapi_srvr_utils.o dtapi_utils.o ${NON_TET_MAIN}
server_enable_if_OFILES	= dtapi_1_srvr_cstub.o server_enable_if.o dtapi_srvr_utils.o dtapi_utils.o ${NON_TET_MAIN}
server_disable_if_OFILES= dtapi_1_srvr_cstub.o server_disable_if.o dtapi_srvr_utils.o dtapi_utils.o ${NON_TET_MAIN}
server_create_OFILES	= server_create.o dtapi_utils.o ${NON_TET_MAIN}
server_delete_OFILES	= server_delete.o dtapi_utils.o ${NON_TET_MAIN}
server_mod_attr_OFILES	= server_mod_attr.o dtapi_utils.o ${NON_TET_MAIN}

binding_create_OFILES	= binding_create.o dtapi_utils.o ${NON_TET_MAIN}
binding_set_auth_OFILES	= binding_set_auth.o dtapi_utils.o ${NON_TET_MAIN}
binding_fm_rpc_OFILES	= binding_fm_rpc.o dtapi_utils.o ${NON_TET_MAIN}
binding_free_OFILES	= binding_free.o dtapi_utils.o ${NON_TET_MAIN}
entry_get_next_OFILES   = entry_get_next.o dtapi_utils.o ${NON_TET_MAIN}
list_release_OFILES	= list_release.o dtapi_utils.o ${NON_TET_MAIN}
list_get_OFILES		= list_get.o dtapi_utils.o ${NON_TET_MAIN}
inq_name_OFILES		= inq_name.o dtapi_utils.o ${NON_TET_MAIN}
inq_id_OFILES		= inq_id.o dtapi_utils.o ${NON_TET_MAIN}
entry_add_OFILES	= entry_add.o dtapi_utils.o ${NON_TET_MAIN}
entry_remove_OFILES	= entry_remove.o dtapi_utils.o ${NON_TET_MAIN}
object_read_OFILES	= object_read.o dtapi_utils.o ${NON_TET_MAIN}
object_read_all_OFILES	= object_read_all.o dtapi_utils.o ${NON_TET_MAIN}
object_release_OFILES	= object_release.o dtapi_utils.o ${NON_TET_MAIN}

secval_start_OFILES 	= secval_start.o dtapi_utils.o ${NON_TET_MAIN}
secval_stop_OFILES 	= secval_stop.o dtapi_utils.o ${NON_TET_MAIN}
secval_validate_OFILES 	= secval_validate.o dtapi_utils.o ${NON_TET_MAIN}
secval_status_OFILES 	= secval_status.o dtapi_utils.o ${NON_TET_MAIN}

rkeytab_add_key_OFILES 	= rkeytab_add_key.o dtapi_utils.o ${NON_TET_MAIN}
rkeytab_inq_key_OFILES 	= rkeytab_inq_key.o dtapi_utils.o ${NON_TET_MAIN}
rkeytab_remove_key_OFILES 	= rkeytab_remove_key.o dtapi_utils.o ${NON_TET_MAIN}
rkeytab_change_key_OFILES 	= rkeytab_change_key.o dtapi_utils.o ${NON_TET_MAIN}
rkeytab_create_OFILES 	= rkeytab_create.o dtapi_utils.o ${NON_TET_MAIN}
rkeytab_delete_OFILES 	= rkeytab_delete.o dtapi_utils.o ${NON_TET_MAIN}

hostdata_create_OFILES 	= hostdata_create.o dtapi_utils.o ${NON_TET_MAIN}
hostdata_read_OFILES 	= hostdata_read.o dtapi_utils.o ${NON_TET_MAIN}
hostdata_write_OFILES 	= hostdata_write.o dtapi_utils.o ${NON_TET_MAIN}
hostdata_delete_OFILES 	= hostdata_delete.o dtapi_utils.o ${NON_TET_MAIN}

# Libraries the programs need to link with
LIBS = -ldce ${STANDARD_TETLIBS}   ${${TARGET_MACHINE}_LIBS}
IDIR            = /test/tet/functional/admin/dced/ts/api/
ILIST		= ${PROGRAMS} ${SCRIPTS}
IDLFLAGS        = -keep c_source
IDLFILES        = dtapi_1_srvr.idl
INCLUDES	= dtapi_1_srvr.h

.include <${RULES_MK}>
dtapi_utils.o: ${IDLFILES:.idl=.h}
${dced_test_server_OFILES} : ${IDLFILES:.idl=.h}
${server_start_OFILES} ${server_stop_OFILES} : ${IDLFILES}
${server_enable_if_OFILES} ${server_disable_if_OFILES} : ${IDLFILES}

