# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: misc_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:35:10  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:28  marty]
#
# Revision 1.1.4.1  1995/12/11  15:25:53  root
# 	Submit
# 	[1995/12/11  14:36:11  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:10  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:13  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:20:59  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:18  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:34:45  marrek
# 	Updated.
# 	[1994/05/11  10:24:11  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:33:21  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:14:16  marrek]
# 
# $EndLog$
# 
# $RCSfile: misc_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:10 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : misc_i.tcl                              	  #
# Description : This file contains all the invalid test cases     #
#		for the other miscellaneous gdscp operations      #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc misc_01i {} {
    set test_case_name "misc_01i"
    set command "exit -some_option"
    set exp_result "Unknown option \"-some_option\"."
    set exp_error_code "GDSCP -701"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc misc_02i {} {
    set test_case_name "misc_02i"
    set command "exit some_arg"
    set exp_result "Invalid argument \"some_arg\"."
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc misc_03i {} {
    set test_case_name "misc_03i"
    set command "exit 23f"
    set exp_result "Invalid argument \"23f\"."
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc misc_04i {} {
    set test_case_name "misc_04i"
    set command "exit some_arg 23"
    set exp_result "Too many arguments."
    set exp_error_code "GDSCP -707"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc misc_05i {} {
    set test_case_name "misc_05i"
    set command "quit -some_option"
    set exp_result "Unknown option \"-some_option\"."
    set exp_error_code "GDSCP -701"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc misc_06i {} {
    set test_case_name "misc_06i"
    set command "quit some_arg"
    set exp_result "Invalid argument \"some_arg\"."
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
