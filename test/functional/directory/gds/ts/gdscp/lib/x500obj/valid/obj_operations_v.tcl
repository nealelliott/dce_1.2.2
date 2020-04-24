# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_operations_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:33  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:39  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:41  root
# 	Submit
# 	[1995/12/11  14:36:49  root]
# 
# Revision 1.1.2.2  1994/06/10  20:23:08  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:09  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  14:35:04  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:16  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_operations_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:33 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_operations_v.tcl                          	  #
# Description : This file contains all the valid test cases       #
#		for the "operations" operation on x500obj object  #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_operations_01v {} {
    set test_case_name "obj_operations_01v"
    set command "x500obj operations"
    set exp_result "bind compare create delete list modify search show help operations"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_operations_02v {} {
    set test_case_name "obj_operations_02v"
    set command "x500obj o"
    set exp_result "bind compare create delete list modify search show help operations"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_operations_03v {} {
    set test_case_name "obj_operations_03v"
    set command "x500obj oper"
    set exp_result "delete"
    run_testcase_and_extract $test_case_name $command $exp_result 4
}
