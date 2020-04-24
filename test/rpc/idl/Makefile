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
# Revision 1.1.12.1  1996/07/09  15:03:08  arvind
# 	Fix IDL build errors - change for HPUX only
# 	[1996/06/12  20:10 UTC  arvind  /main/DCE_1.2.2/arvind_work/2]
#
# 	FIx IDL errors during build - change it for HPUX only
# 	[1996/06/12  18:45 UTC  arvind  /main/DCE_1.2.2/arvind_work/1]
#
# 	FIx IDL errors during build
# 	[1996/06/12  18:38 UTC  arvind  /main/DCE_1.2.2/1]
#
# 	Make DCE_1.2.2 branch
# 	[1996/03/11  02:08:20  marty  1.1.10.2]
#
# Revision 1.1.10.2  1996/03/11  02:08:20  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:51:04  marty]
# 
# Revision 1.1.10.1  1995/12/11  19:54:49  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:27:44  root]
# 
# Revision 1.1.8.2  1994/07/14  21:29:38  tom
# 	Get rid of NCS 1.5 tests (OT 7435).
# 	[1994/07/14  21:24:59  tom]
# 
# Revision 1.1.8.1  1994/03/03  22:08:01  bmw
# 	no changes
# 	[1994/03/03  16:34:14  bmw]
# 
# Revision 1.1.6.2  1993/08/11  16:23:20  ganni
# 	tests for new IDL functionality
# 	[1993/08/11  16:12:55  ganni]
# 
# Revision 1.1.2.3  1993/01/07  19:00:37  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  13:24:25  bbelch]
# 
# Revision 1.1.2.2  1992/11/02  15:36:23  root
# 	Add ILIST for install pass
# 	[1992/10/28  20:35:00  root]
# 
# Revision 1.1  1992/01/19  03:34:20  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#
# test/rpc/idl/Makefile for idl testcases
#

# Sub-directories to visit
.if ${TARGET_OS} == "HPUX"
DATA_SUBDIRS	= refdels threads
.else
DATA_SUBDIRS    = context cover1 no_op pipefault pipes pipetest         \
                refdels threads
.endif

SUBDIRS		= ${SUCCESSFUL}

# Script to copy to OBJ tree for running testcases.
SCRIPTS		= run_tests

# Library to build and export, and its export location
LIBRARIES	= libidltest.a
EXPDIR		= /usr/lib/
CFLAGS		= ${${TARGET_MACHINE}_CFLAGS}
OFILES		= util.o

ILIST		= ${SCRIPTS}

# The following testcases compile and run successfully.
SUCCESSFUL	= \
	alias		ao_arrays	array		array2		\
	array_v1_string	array_v2_string	arrayp		arrayptr	\
	arraystr	arraystr2	arrayswith	arrayv1		\
	bigtest1	binop_alloc	binopwk		brown		\
	bug_3		comprefs	comprefs_ool	conftree	\
	context		context2	cover1		cover2		\
	cover_array	cover_array_ool	cover_contain_ref cover_ool	\
	cover_struct	cover_top_pointer cover_union	epv		\
	es_array	es_arrayb	es_arrayc			\
	factorial	faultstat	fpe		gregbug1	\
	h_ta		ichar1		ichar2		icharexp	\
	icharpkl	ignore		mtsaconf	mtsvary		\
	multiref	n_e_union					\
	no_op		nodethreads	ool		oolsmall	\
	oolthings	op_results	orecord		out_refs	\
	outrefs		packed_byte_array parray	pipe_of_array	\
	pipefault	pipes		pipetest	pipetest2	\
	pipetest2_cancel pipetest2_ool	pointarray	pointed		\
	primes		ptrarray					\
	refdels		refptrs		repas1				\
	repas2		represent	represent_ool	sp_union	\
	sparse		string0		string1		string_tree	\
	stringptr	stringts	stringy		struct_ct_union	\
	threads		tree		ue1		ue2		\
	unique1		unique2						\
	xmit		xmitas		xmitas_conf	xmitas_nested	\
	xmitas_repas	xmitas_repas_ool

.include <${RULES_MK}>
