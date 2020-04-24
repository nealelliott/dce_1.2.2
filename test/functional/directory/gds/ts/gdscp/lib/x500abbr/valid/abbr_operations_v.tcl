# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: abbr_operations_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:35:43  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:57  marty]
#
# Revision 1.1.4.1  1995/12/11  15:26:37  root
# 	Submit
# 	[1995/12/11  14:36:26  root]
# 
# Revision 1.1.2.3  1994/07/06  15:54:08  marrek
# 	Add version string in July 1994 code drop.
# 	[1994/07/06  15:42:28  marrek]
# 
# Revision 1.1.2.2  1994/06/10  20:21:57  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:37  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  14:34:01  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:09  marrek]
# 
# $EndLog$
#
# $RCSfile: abbr_operations_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:43 $
#

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : abbr_operations_v.tcl                          	  #
# Description : This file contains all the valid test cases       #
#		for the "operations" operation on x500abbr object #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc abbr_operations_01v {} {
    set test_case_name "abbr_operations_01v"
    set command "x500abbr operations"
    set exp_result "show help operations"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc abbr_operations_02v {} {
    set test_case_name "abbr_operations_02v"
    set command "x500abbr o"
    set exp_result "show help operations"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc abbr_operations_03v {} {
    set test_case_name "abbr_operations_03v"
    set command "x500abbr oper"
    set exp_result "show"
    run_testcase_and_extract $test_case_name $command $exp_result 1
}
