# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: abbr_operations_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:35:37  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:52  marty]
#
# Revision 1.1.4.1  1995/12/11  15:26:29  root
# 	Submit
# 	[1995/12/11  14:36:24  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:19  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:21  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:21:54  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:35  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:34:52  marrek
# 	Updated.
# 	[1994/05/11  10:24:33  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:33:54  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:04  marrek]
# 
# $EndLog$
# 
# $RCSfile: abbr_operations_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:37 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : abbr_operations_i.tcl                             #
# Description : This file contains all the invalid test cases     #
#		for the "operations" operation on x500abbr object #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc abbr_operations_01i {} {
    set test_case_name "abbr_operations_01i"
    set command "x500abbr operations some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -702"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc abbr_operations_02i {} {
    set test_case_name "abbr_operations_02i"
    set command "x500abbr operations -some_option"
    set exp_result "Unknown option \"-some_option\"."
    set exp_error_code "GDSCP -701"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
