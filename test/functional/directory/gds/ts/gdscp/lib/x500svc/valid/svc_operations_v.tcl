# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: svc_operations_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  01:34:17  marty
# 	Update copyright years
# 	[1996/03/10  19:36:21  marty]
#
# Revision 1.1.4.1  1995/12/11  15:28:06  root
# 	Submit
# 	[1995/12/11  14:37:00  root]
# 
# Revision 1.1.2.2  1994/06/10  20:23:34  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:26  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  14:35:39  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:47  marrek]
# 
# $EndLog$
# 
# $RCSfile: svc_operations_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 01:34:17 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : svc_operations_v.tcl                          	  #
# Description : This file contains all the valid test cases       #
#		for the "operations" operation on x500svc object  #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc svc_operations_01v {} {
    set test_case_name "svc_operations_01v"
    set command "x500svc operations"
    set exp_result "modify show help operations"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_operations_02v {} {
    set test_case_name "svc_operations_02v"
    set command "x500svc o"
    set exp_result "modify show help operations"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_operations_03v {} {
    set test_case_name "svc_operations_03v"
    set command "x500svc oper"
    set exp_result "show"
    run_testcase_and_extract $test_case_name $command $exp_result 2
}
