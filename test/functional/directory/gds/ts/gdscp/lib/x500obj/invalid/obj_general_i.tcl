# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_general_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:00  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:17  marty]
#
# Revision 1.1.4.1  1995/12/11  15:26:55  root
# 	Submit
# 	[1995/12/11  14:36:35  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:28  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:40  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:23  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:47  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:34:59  marrek
# 	Updated.
# 	[1994/05/11  10:24:52  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:22  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:33  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_general_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:00 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_general_i.tcl                              	  #
# Description : This file contains all the general test cases     #
#		for the invalid operations on the x500obj object  #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_general_01i {} {
    set test_case_name "obj_general_01i"
    set command "some_command"
    set exp_result "invalid command name \"some_command\""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_02i {} {
    set test_case_name "obj_general_02i"
    set command "compare"
    set exp_result "Perform bind operation first."
    set exp_error_code "GDSCP -521"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_03i {} {
    set test_case_name "obj_general_03i"
    set command "create"
    set exp_result "Perform bind operation first."
    set exp_error_code "GDSCP -521"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_04i {} {
    set test_case_name "obj_general_04i"
    set command "list"
    set exp_result "Perform bind operation first."
    set exp_error_code "GDSCP -521"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_05i {} {
    set test_case_name "obj_general_05i"
    set command "modify"
    set exp_result "Perform bind operation first."
    set exp_error_code "GDSCP -521"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_06i {} {
    set test_case_name "obj_general_06i"
    set command "set gdscp_cwo /c=de"
    set exp_result "can't set \"gdscp_cwo\": Perform bind operation first."
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_07i {} {
    set test_case_name "obj_general_07i"
    set command "show"
    set exp_result "Perform bind operation first."
    set exp_error_code "GDSCP -521"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_08i {} {
    set test_case_name "obj_general_08i"
    set command "delete"
    set exp_result "Perform bind operation first."
    set exp_error_code "GDSCP -521"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_09i {} {
    set test_case_name "obj_general_09i"
    set command "search"
    set exp_result "Perform bind operation first."
    set exp_error_code "GDSCP -521"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_10i {} {
    set test_case_name "obj_general_10i"
    set command "x500svc modify"
    set exp_result "Perform bind operation first."
    set exp_error_code "GDSCP -521"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_11i {} {
    set test_case_name "obj_general_11i"
    set command "x500obj"
    set exp_result "Missing operation."
    set exp_error_code "GDSCP -706"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_12i {} {
    set test_case_name "obj_general_12i"
    set command "x500obj \"\""
    set exp_result "Unknown operation \"\"."
    set exp_error_code "GDSCP -705"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_13i {} {
    set test_case_name "obj_general_13i"
    set command "x500obj -attribute"
    set exp_result "Missing operation."
    set exp_error_code "GDSCP -706"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_14i {} {
    set test_case_name "obj_general_14i"
    set command "x500obj c"
    set exp_result "Ambiguous operation \"c\": compare create."
    set exp_error_code "GDSCP -708"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_15i {} {
    set test_case_name "obj_general_15i"
    set command "x500obj s"
    set exp_result "Ambiguous operation \"s\": search show."
    set exp_error_code "GDSCP -708"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_16i {} {
    set test_case_name "obj_general_16i"
    set command "x500obj some_operation"
    set exp_result "Unknown operation \"some_operation\"."
    set exp_error_code "GDSCP -705"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_17i {} {
    set test_case_name "obj_general_17i"
    set command "x500obj quit"
    set exp_result "Unknown operation \"quit\"."
    set exp_error_code "GDSCP -705"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_general_18i {} {
    set test_case_name "obj_general_18i"
    set command "הצü"
    set exp_result "invalid command name \"הצü\""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
