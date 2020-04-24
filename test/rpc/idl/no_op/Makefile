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
# Revision 1.1.10.2  1996/03/11  02:15:11  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:53:49  marty]
#
# Revision 1.1.10.1  1995/12/11  19:25:26  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:13:51  root]
# 
# Revision 1.1.8.3  1994/05/24  17:57:58  tom
# 	Set IDL_GEN_AUX_FILES so idl will generate aux files.
# 	[1994/05/24  17:56:37  tom]
# 
# Revision 1.1.8.2  1994/03/03  22:13:26  bmw
# 	add no_op.h to DATAFILES
# 	[1994/03/03  22:13:10  bmw]
# 
# Revision 1.1.8.1  1993/12/28  22:10:31  pellis
# 	Updated for ODE 2.3 make
# 	[1993/12/28  21:53:58  pellis]
# 
# Revision 1.1.6.2  1993/01/07  19:59:18  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  13:35:10  bbelch]
# 
# Revision 1.1.2.2  1992/04/07  19:20:12  mckeen
# 	Fixed dependencies
# 	[1992/03/31  16:38:36  mckeen]
# 
# Revision 1.1  1992/01/19  03:32:49  devrcs
# 	Initial revision
# 
# $EndLog$
#

DATAFILES		= RUN_ARGS no_op.h


IDLBASE			= no_op
OTHER_CLIENT_FILES	= client_aux.o
OTHER_SERVER_FILES	= no_op_imported_saux.o

.include "../maketest.mk"

no_op_imported_saux.o : no_op_imported.h

# In order to get the newer idl to produce aux files, must
# set IDL_GEN_AUX_FILES environment variable.
#
no_op_imported.h : no_op_imported.idl
	env IDL_GEN_AUX_FILES=1 ${IDL} ${no_op_imported.idl:P} ${_IDLFLAGS_} ${IDL_CC} \
		-client none -server aux

${IDLBASE}.h : no_op_imported.h
