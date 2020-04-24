#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.15.1  1996/05/10  13:30:36  arvind
# 	Drop 1 of DCE 1.2.2 to OSF
#
# 	HP revision /main/DCE_1.2/2  1996/03/08  21:52 UTC  bissen
# 	merge back ux_release changse
# 	[1995/07/14  19:55 UTC  rmm  /main/HPDCE02/6]
#
# 	change the 1000 cond to work for both Mothra and WC
# 	[1995/06/29  17:33 UTC  rmm  /main/HPDCE02/rmm_wc_work/3]
#
# 	change name of p_is_running_P for WC
# 	[1995/06/29  14:58 UTC  rmm  /main/HPDCE02/rmm_wc_work/2]
#
# 	Change rule for getting p_is_running_P so new WC ar is happy
# 	[1995/06/23  15:52 UTC  rmm  /main/HPDCE02/rmm_wc_work/1]
#
# 	all commands (mv in this case) need to start with a tab
# 	[1995/06/14  21:38 UTC  rmm  /main/HPDCE02/5]
#
# 	can't use -f option with the 9.x ar
# 	[1995/06/14  21:37 UTC  rmm  /main/HPDCE02/rmm_mothra10/5]
#
# 	-f not allowed for 9.x in
# 	[1995/06/14  10:26 UTC  root  /main/HPDCE02/4]
#
# 	Merge back AR changes for purify'd libcma
# 	[1995/06/13  20:31 UTC  rmm  /main/HPDCE02/3]
#
# 	ack.  mv to p_is_running_P, not p_is_running_P.o
# 	[1995/04/10  17:22 UTC  rmm  /main/HPDCE02/rmm_mothra10/4]
#
# 	Oops.  -F should really be -f
# 	[1995/04/10  16:23 UTC  rmm  /main/HPDCE02/rmm_mothra10/3]
#
# 	conditionalize ar command line for 10.0
# 	[1995/04/10  15:40 UTC  rmm  /main/HPDCE02/rmm_mothra10/2]
#
# 	make sure we get the external tree version of ar
# 	[1995/04/07  15:41 UTC  rmm  /main/HPDCE02/rmm_mothra10/1]
#
# 	move purify stub to threads build
# 	[1995/03/28  23:07 UTC  rps  /main/HPDCE02/2]
#
# 	move purify stub to threads build
# 	[1995/03/28  00:19 UTC  rps  /main/HPDCE02/rps_mothra/1]
#
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 	[1994/07/06  16:49 UTC  jss  /main/HPDCE02/1]
#
# 	Merged tatsu_s_ccase_fix_b1 to HPDCE01.
# 	[1994/02/01  22:31  tatsu_s  /main/HPDCE01/2]
#
# 	Disabled the variable evaluations on ':' command.
# 	[1994/02/01  21:20  tatsu_s  /main/HPDCE01/tatsu_s_ccase_fix_b1/2]
#
# 	Made EVs not being echoed.
# 	[1994/02/01  17:19  tatsu_s  /main/HPDCE01/tatsu_s_ccase_fix_b1/1]
#
# 	Submitted tatsu_s_ccase_fix_b0 to HPDCE01.
# 	Added the capture of the critical environments in the translation rules.
# 	[1994/01/24  18:41  tatsu_s  /main/HPDCE01/1]
#
# 	Add missing rm -f.
# 	[1993/12/02  19:05:52  mgm]
# 	#
#
# 	HP revision /main/DCE_1.2/1  1996/01/03  18:48 UTC  psn
# 	Add creation rules for hpdce_platform.h
# 	[1995/11/16  21:36 UTC  jrr  /main/jrr_1.2_mothra/1]
#
# Revision 1.1.11.1  1994/06/09  13:35:01  devsrc
# 	 CR10892 - fix copyright in file
# 	[1994/06/09  13:25:50  devsrc]
# 
# Revision 1.1.9.2  1993/06/10  19:28:12  sommerfeld
# 	Remove out HP800_CFLAGS and make
# 	-DPIC dependent on value of NO_SHARED_LIBRARIESS.
# 	This works short term, but should be
# 	reworked the "right" way at some point.
# 	[1993/04/23  22:09:41  hopkins]
# 
# Revision 1.1.3.2  1992/12/15  22:21:59  alan
# 	Insert copyright notices
# 	[1992/12/07  16:00:17  alan]
# 
# Revision 1.1  1992/01/19  14:40:16  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# define machine dependent compiler flags

.if !defined(NO_SHARED_LIBRARIES)
HP800_ASMFLAG=-DPIC
.endif

HP800_INCLUDES  = hpdce_platform.h

cma_assem.o :	cma_assem.s
	-@: # CCOPTS='${CCOPTS}'
	-@: # ST_CPP_COMPAT='${ST_CPP_COMPAT}' ST_CPP_ANSI='${ST_CPP_ANSI}' ST_CPP_BBA='${ST_CPP_BBA}'
	-@: # ST_ASSEM='${ST_ASSEM}' ST_PRE='${ST_PRE}'
	-@: # INCLUDIR='${INCLUDIR}'
	-@: # LC_CTYPE='${LC_CTYPE:UC:S/^POSIX$/C/}' LC_ALL='${LC_ALL:UC:S/^POSIX$/C/}' LANG='${LANG:UC:S/^POSIX$/C/}'
	${RM} ${_RMFLAGS_} ${.TARGET}
	${_CC_} -c ${HP800_ASMFLAG} ${ASMFLAG} ${.ALLSRC}

hpdce_platform.h: hpdce_platform.template
	sed "s;<<os_revision>>;${ux_release};" ${hpdce_platform.template:P} > $@


