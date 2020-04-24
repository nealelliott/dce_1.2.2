# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: svc_help_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:44  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:48  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:53  root
# 	Submit
# 	[1995/12/11  14:36:55  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:51  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:23  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:23:28  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:15  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:16  marrek
# 	Updated.
# 	[1994/05/11  10:25:36  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:35:21  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:30  marrek]
# 
# $EndLog$
# 
# $RCSfile: svc_help_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:44 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : svc_help_i.tcl                                    #
# Description : This file contains all the invalid test cases     #
#		for the help operation on the x500svc object  	  #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc svc_help_01i {} {
    set test_case_name "svc_help_01i"
    set command "x500svc help some_operation"
    set exp_result "Unknown operation \"some_operation\"."
    set exp_error_code "GDSCP -705"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_help_02i {} {
    set test_case_name "svc_help_02i"
    set command "x500svc help -some_option"
    set exp_result "Unknown option \"-some_option\"."
    set exp_error_code "GDSCP -701"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_help_03i {} {
    set test_case_name "svc_help_03i"
    set command "x500svc help -verbose show"
    set exp_result "Too many arguments."
    set exp_error_code "GDSCP -707"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
