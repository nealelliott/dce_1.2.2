#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: test.mk,v $
# Revision 1.2.25.1  1996/11/14  17:15:30  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  15:22:32  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/10/18  00:44 UTC  arvind
# 	Add CAPI .mk includes.
# 	[1996/10/17  19:20 UTC  arvind  /main/arvind_capi/2]
# 
# 	Cert API changes
# 	[1996/10/17  18:52 UTC  jrr  /main/arvind_capi/1]
# 
# Revision 1.2.23.2  1996/03/11  02:49:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:23  marty]
# 
# Revision 1.2.23.1  1995/12/11  22:52:16  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1994/10/10  13:29 UTC  mothra
# 	Merge Hpdce02_04 to HPDCE02 branch
# 	[1995/12/11  22:24:01  root]
# 
# Revision 1.2.21.5  1994/08/31  18:25:59  marty
# 	CR # 10472 - Adde variable FVTBASE to point to test/tet/functional.
# 	[1994/08/31  18:01:12  marty]
# 
# Revision 1.2.21.4  1994/07/27  13:02:24  keutel
# 	OT 11438: removed old gds (ros) stuff
# 	[1994/07/27  12:20:47  keutel]
# 
# Revision 1.2.21.3  1994/06/21  14:30:01  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:51:32  marrek]
# 
# 	HP revision /main/HPDCE01/1  1994/02/03  13:17  mgm
# 	merge hpdce01 and kk_final
# 
# Revision 1.1.8.3  1993/09/23  14:42:13  tatsu_s
# 	KK merged upto DCE1_0_3b03.
# 
# 	Removed TOSTAGE line from this file.  It is overriding
# 	the command line TOSTAGE variable.
# 	[1993/04/02  21:49:01  mgm]
# 
# Revision 1.1.9.3  1993/09/15  17:30:17  root
# 	    Loading drop DCE1_0_3b03
# 	Revision 1.1.8.2  1993/07/09  16:52:41  root
# 	Initial King Kong branch
# 	[1993/07/09  16:52:12  root]
# 
# Revision 1.2.17.5  1993/08/18  20:52:48  pellis
# 	Bug #8471 - Uncommented TOSTAGE so that tests get installed in dcetest.
# 	[1993/08/18  20:52:25  pellis]
# 
# Revision 1.2.17.4  1993/08/09  19:54:43  cmckeen
# 	Changed the IDIR line to set IDIR if IDIR is not already defined.
# 	[1993/08/09  19:29:19  cmckeen]
# 
# Revision 1.2.17.3  1993/08/03  19:42:38  tom
# 	Bug 3931 - move some standard stuff up here.
# 	[1993/08/03  16:58:51  tom]
# 
# Revision 1.2.17.2  1993/07/19  19:45:42  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:34:13  zeliff]
# 
# Revision 1.2.15.3  1993/07/16  22:51:25  kissel
# 	The TOSTAGE on the "build "TOSTAGE=/release/our_area"
# 	install_all" command line
# 	is NOT overriding this TOSTAGE variable -not sure why.This is not good.
# 	TOSTAGE should be able to be tailored to any customer's environment.
# 	[1993/06/30  16:42:44  mgm]
# 
# Revision 1.2.21.2  1994/04/21  15:54:19  tom
# 	Bug 10343 - Change dce1.0 to dce1.1.
# 	[1994/04/20  20:06:39  tom]
# 
# Revision 1.2.21.1  1993/12/28  22:11:44  pellis
# 	Updated for ODE 2.3 make
# 	[1993/12/28  21:55:06  pellis]
# 
# Revision 1.2.19.1  1993/10/15  21:16:11  ohara
# 	SVR4 porting changes - 8596
# 	[1993/10/15  16:36:04  ohara]
# 
# Revision 1.2.17.5  1993/08/18  20:52:48  pellis
# 	Bug #8471 - Uncommented TOSTAGE so that tests get installed in dcetest.
# 	[1993/08/18  20:52:25  pellis]
# 
# Revision 1.2.17.4  1993/08/09  19:54:43  cmckeen
# 	Changed the IDIR line to set IDIR if IDIR is not already defined.
# 	[1993/08/09  19:29:19  cmckeen]
# 
# Revision 1.2.17.3  1993/08/03  19:42:38  tom
# 	Bug 3931 - move some standard stuff up here.
# 	[1993/08/03  16:58:51  tom]
# 
# Revision 1.2.17.2  1993/07/19  19:45:42  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:34:13  zeliff]
# 
# Revision 1.2.15.3  1993/07/16  22:51:25  kissel
# 	The TOSTAGE on the "build "TOSTAGE=/release/our_area"
# 	install_all" command line
# 	is NOT overriding this TOSTAGE variable -not sure why.This is not good.
# 	TOSTAGE should be able to be tailored to any customer's environment.
# 	[1993/06/30  16:42:44  mgm]
# 
# Revision 1.2.7.4  1993/02/01  21:33:48  hinman
# 	[hinman] - SVR4 changes
# 	[1993/02/01  21:05:10  hinman]
# 
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  16:30:45  hinman]
# 
# 	[hinman] - Check in merged SNI version
# 	[1993/01/11  16:16:02  hinman]
# 
# Revision 1.2.7.3  1992/12/06  03:02:41  zeliff
# 	Embedding Copyrights
# 	[1992/12/06  03:00:42  zeliff]
# 
# Revision 1.2.7.2  1992/11/02  15:38:56  root
# 	Create install tree for dcetest. (which would have test and systest
# 	under it) The install tree for dcetest will live parallel to the
# 	'opt' tree.
# 	[1992/10/28  20:00:12  root]
# 
# Revision 1.2.2.7  1992/05/13  22:01:06  sekhar
# 	Link krb5 library when building security test case on RIOS.
# 	[1992/05/13  21:02:03  sekhar]
# 
# Revision 1.2.2.6  1992/05/01  15:52:06  rsalz
# 	  19-mar-92 nm    Break out rpc test into a separate file.
# 	[1992/05/01  00:58:35  rsalz]
# 
# Revision 1.2.2.4  1992/01/31  20:18:43  zeliff
# 	Added more PMAX_LIBS
# 	[1992/01/31  20:18:23  zeliff]
# 
# Revision 1.2.2.3  1992/01/31  20:07:38  zeliff
# 	Added PMAX_LIBS to include -lbsd for test/rpc/runtime/v2test_lib
# 	[1992/01/31  20:06:34  zeliff]
# 
# Revision 1.2.2.2  1992/01/22  22:19:57  mhickey
# 	Added CCTYPE = traditional as default for RIOS,
# 	prettied up format for readability.
# 	[1992/01/22  22:19:16  mhickey]
# 
# Revision 1.2  1992/01/19  22:14:42  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#  OSF DCE Version 1.0 


.if !defined(_TEST_MK_)
_TEST_MK_=

#
# The test and systest install trees live parallel to the opt tree; ie:
#       
#                         <sandbox_base>
#                             |
#                          install
#                             |
#                     +-------+-------+
#                     |               |
#                   pmax             rios
#                    |                
#       +------------+-----------+
#       |                        |
#      opt                    dcetest
#	|			 |
#     dce1.1                   dce1.1
#				 |
#                               test
#                                |
#                +---------------+-------+----+-----+------+-------+------+
#                |               |       |    |     |      |       |      |
#	      systest       directory  file  gds   rpc  security threads time
#                |
#   +---------+----+---+-----+--------+------+----+
#   |         |    |   |     |        |      |    |
# directory  file gds rpc security threads  time tools
#
#

#
# Reset TOSTAGE variable to get the test install tree in the right structure.
#

# TOSTAGE should not be defined in this makefile.  It affects all other
# directories below here.  Please use build line or rc_files.
# TOSTAGE =  ${SOURCEBASE}/../install/${target_machine}/dcetest/dce1.0
FVTBASE =  /test/tet/functional

#
# Set the IDIR variable to be the directory in which we are building.  This 
# affects the install build of the whole test tree.
#
IDIR ?= /${MAKESUB}

SVR4_CFLAGS  ?= -DSVR4 -Xa -DSNI

#
# install samples in /usr/examples/devkit_sample/ under the opt subtree
# rather than the dcetest subtree on SVR4 platforms.  It's much handier
# for packaging this way.
# 
.if ${MAKESUB}=="test/sample/" && ${TARGET_MACHINE}=="SVR4"
 TOSTAGE =  ${SOURCEBASE}/../install/${target_machine}/opt/dce1.1
 IDIR = /usr/examples/devkit_sample/
.endif

.if ${MAKESUB}=="test/directory/cds/"
  RIOS_LIBS	?= -lm -lbsd
.endif

.if ${MAKESUB}=="test/security/commands/rgy/"
  RIOS_CFLAGS	?= -Dunix 
  RIOS_LIBS	?= -lbsd -lkrb5
.endif

.if ${MAKESUB}=="test/security/api/testsh/"
  RIOS_CFLAGS	?= -Dunix -D_BSD 
  RIOS_YFLAGS	?= -l
  APOLLO_YFLAGS	?= -l
.endif

.if ${MAKESUB}=="test/security/api/moretests/"
  RIOS_CFLAGS	?= -Dunix -D_BSD
  RIOS_LIBS	?= -lbsd
.endif

.if ${MAKESUB}=="test/time/service/"
  RIOS_CFLAGS	?= -Dunix -D_BSD
  MIPS_CLFAGS	?= -g3
  VAX_CFLAGS	?= -g
  RIOS_LIBS	?= -lm
  MIPS_LIBS	?= -li
  VAX_LIBS	?= -li
.endif

.if ${MAKESUB}=="test/time/common/"
  RIOS_CFLAGS	?= -Dunix
  MIPS_CFLAGS	?= -g3
  VAX_CFLAGS	?= -g
  RIOS_LIBS	?= -lm
.endif

.if !empty(MAKESUB:Mtest/rpc*)
.include "${MAKETOP}/test/rpc/rpc.mk"
.endif

.if ${MAKESUB}=="test/functional/security/ts/capi/certs/"
.include "${MAKETOP}/test/security/api/capi/certs/certs_data.mk"
.endif

.if ${MAKESUB}=="test/functional/security/ts/capi/keys/"
.include "${MAKETOP}/test/security/api/capi/keys/keys_data.mk"
.endif

.if ${MAKESUB}=="test/functional/security/ts/capi/testcases/"
.include "${MAKETOP}/test/security/api/capi/testcases/test_data.mk"
.endif

.if ${TARGET_MACHINE} == "RIOS"
  CCTYPE                ?= traditional
.endif
.endif


CFLAGS		?= ${${TARGET_MACHINE}_CFLAGS}
LDFLAGS		?= ${${TARGET_MACHINE}_LDFLAGS}
YFLAGS		?= ${${TARGET_MACHINE}_YFLAGS}
LIBS		?= ${LIBS_DCE} ${${TARGET_MACHINE}_LIBS}




