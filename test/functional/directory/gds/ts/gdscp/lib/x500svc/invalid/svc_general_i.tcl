# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: svc_general_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:46  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:51  root
# 	Submit
# 	[1995/12/11  14:36:54  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:50  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:21  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:23:27  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:14  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:15  marrek
# 	Updated.
# 	[1994/05/11  10:25:34  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:35:19  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:29  marrek]
# 
# $EndLog$
# 
# $RCSfile: svc_general_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:42 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : svc_general_i.tcl                              	  #
# Description : This file contains all the general test cases     #
#		for the invalid operations on the x500svc object  #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc svc_general_01i {} {
    set test_case_name "svc_general_01i"
    set command "x500svc"
    set exp_result "Missing operation."
    set exp_error_code "GDSCP -706"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_general_02i {} {
    set test_case_name "svc_general_02i"
    set command "x500svc \"\""
    set exp_result "Unknown operation \"\"."
    set exp_error_code "GDSCP -705"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_general_03i {} {
    set test_case_name "svc_general_03i"
    set command "x500svc -pretty"
    set exp_result "Missing operation."
    set exp_error_code "GDSCP -706"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_general_04i {} {
    set test_case_name "svc_general_04i"
    set command "x500svc search"
    set exp_result "Unknown operation \"search\"."
    set exp_error_code "GDSCP -705"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_general_05i {} {
    set test_case_name "svc_general_05i"
    set command "x500svc quit"
    set exp_result "Unknown operation \"quit\"."
    set exp_error_code "GDSCP -705"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
