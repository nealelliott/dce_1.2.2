# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: svc_operations_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:48  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:52  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:58  root
# 	Submit
# 	[1995/12/11  14:36:57  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:53  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:30  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:23:30  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:17  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:18  marrek
# 	Updated.
# 	[1994/05/11  10:25:43  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:35:27  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:38  marrek]
# 
# $EndLog$
# 
# $RCSfile: svc_operations_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:48 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : svc_operations_i.tcl                              #
# Description : This file contains all the invalid test cases     #
#		for the "operations" operation on x500svc object  #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc svc_operations_01i {} {
    set test_case_name "svc_operations_01i"
    set command "x500svc operations some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -702"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_operations_02i {} {
    set test_case_name "svc_operations_02i"
    set command "x500svc operations -some_option"
    set exp_result "Unknown option \"-some_option\"."
    set exp_error_code "GDSCP -701"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
