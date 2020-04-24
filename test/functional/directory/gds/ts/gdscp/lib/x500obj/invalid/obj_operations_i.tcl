# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_operations_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:08  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:23  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:05  root
# 	Submit
# 	[1995/12/11  14:36:39  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:32  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:51  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:43  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:51  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:04  marrek
# 	Updated.
# 	[1994/05/11  10:25:02  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:31  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:46  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_operations_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:08 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_operations_i.tcl                              #
# Description : This file contains all the invalid test cases     #
#		for the "operations" operation on x500obj object  #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_operations_01i {} {
    set test_case_name "obj_operations_01i"
    set command "x500obj operations some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -702"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_operations_02i {} {
    set test_case_name "obj_operations_02i"
    set command "x500obj operations -some_option"
    set exp_result "Unknown option \"-some_option\"."
    set exp_error_code "GDSCP -701"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
