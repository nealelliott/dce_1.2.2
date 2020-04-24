# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_cwo_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:35:56  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:11  marty]
#
# Revision 1.1.4.1  1995/12/11  15:26:52  root
# 	Submit
# 	[1995/12/11  14:36:33  root]
# 
# Revision 1.1.2.5  1994/07/06  15:11:27  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:36  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:29:25  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:50:11  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:21  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:45  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:34:58  marrek
# 	Updated.
# 	[1994/05/11  10:24:48  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:17  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:27  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_cwo_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:56 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_cwo_i.tcl                              	  #
# Description : This file contains all the invalid test cases     #
#		for the current working object variable 	  #
#		"gdscp_cwo"					  #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_cwo_01i {} {
    bind_to_dsa
    set test_case_name "obj_cwo_01i"
    set command "set gdscp_cwo c=de/o=/ou=ap11"
    set exp_result "can't set \"gdscp_cwo\": Missing attribute value - \"/c=de/o=/ou=ap11\" : Error position <9>."
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_cwo_02i {} {
    bind_to_dsa
    set test_case_name "obj_cwo_02i"
    set command "set gdscp_cwo \"  \""
    set exp_result "can't set \"gdscp_cwo\": Missing object name."
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_cwo_03i {} {
    bind_to_dsa
    set test_case_name "obj_cwo_03i"
    set command "set gdscp_cwo -attribute"
    set exp_result "can't set \"gdscp_cwo\": Missing object name."
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_cwo_04i {} {
    bind_to_dsa
    set test_case_name "obj_cwo_04i"
    set command "set gdscp_cwo /c=de/o=yy"
    set exp_result "can't set \"gdscp_cwo\": The specified name does not match the name of any object in the directory."
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_cwo_05i {} {
    bind_to_dsa
    set test_case_name "obj_cwo_05i"
    set command "unset gdscp_cwo"
    set exp_result "Error: Cannot unset gdscp_cwo."
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
