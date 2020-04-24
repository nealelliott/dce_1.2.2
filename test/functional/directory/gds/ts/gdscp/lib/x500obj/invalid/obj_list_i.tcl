# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_list_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:04  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:19  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:00  root
# 	Submit
# 	[1995/12/11  14:36:37  root]
# 
# Revision 1.1.2.5  1994/07/06  15:11:30  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:44  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:29:27  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:50:15  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:25  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:49  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:01  marrek
# 	Updated.
# 	[1994/05/11  10:24:56  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:26  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:38  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_list_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:04 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_list_i.tcl                              	  #
# Description : This file contains all the invalid test cases     #
#		for the list operation on the x500obj object      #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_list_01i {} {
    bind_to_dsa
    set test_case_name "obj_list_01i"
    set command "x500obj list -attribute"
    set exp_result "Unknown option \"-attribute\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_list_02i {} {
    bind_to_dsa
    set test_case_name "obj_list_02i"
    set command "x500obj list /c=de -attribute"
    set exp_result "Unknown option \"-attribute\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_list_03i {} {
    bind_to_dsa
    set test_case_name "obj_list_03i"
    set command "x500obj list /c=de /c=de/o=sni"
    set exp_result "Too many values."
    set exp_error_code "GDSCP -533"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_list_04i {} {
    bind_to_dsa
    set test_case_name "obj_list_04i"
    set command "x500obj list /c=de/o=xx/ou=ap11"
    set exp_result "The specified name does not match the name of any object in the directory."
    set exp_error_code "GDSCP -34"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_list_05i {} {
    bind_to_dsa
    set test_case_name "obj_list_05i"
    set command "list /c=de/o=/ou=ap11"
    set exp_result "Missing attribute value - \"/c=de/o=/ou=ap11\" : Error position <9>."
    set exp_error_code "GDSCP -338"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_list_06i {} {
    bind_to_dsa
    set test_case_name "obj_list_06i"
    set command "list \"\{/c=de/o=sni/ou=ap11\""
    set exp_result "unmatched open brace in list"
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_list_07i {} {
    bind_to_dsa
    set test_case_name "obj_list_07i"
    set command "list {{/c=de/o=sni/ou=ap11} {-attribute}}"
    set exp_result "Invalid argument \"{/c=de/o=sni/ou=ap11} {-attribute}\"."
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_list_08i {} {
    bind_to_dsa
    set test_case_name "obj_list_08i"
    set command "list /cn=schema"
    set exp_result "Subordinates not found."
    set exp_error_code "GDSCP -528"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_list_09i {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "x500svc modify -sizelimit 1"
    set test_case_name "obj_list_09i"
    set command "list /c=de/o=sni/ou=ap11"
    set exp_result "/C=de/O=sni/OU=ap11/CN=naik {Insufficient size limit.}"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc modify -default"
    remove_dit_objects
}
