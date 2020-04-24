# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_delete_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:35:58  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:12  marty]
#
# Revision 1.1.4.1  1995/12/11  15:26:54  root
# 	Submit
# 	[1995/12/11  14:36:34  root]
# 
# Revision 1.1.2.5  1994/07/06  15:11:28  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:38  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:29:26  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:50:13  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:22  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:46  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:34:59  marrek
# 	Updated.
# 	[1994/05/11  10:24:50  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:19  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:30  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_delete_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:58 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_delete_i.tcl                              	  #
# Description : This file contains all the invalid test cases     #
#		for the delete operation on the x500obj object    #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_delete_01i {} {
    bind_to_dsa
    set test_case_name "obj_delete_01i"
    set command "x500obj delete"
    set exp_result "Missing object name."
    set exp_error_code "GDSCP -504"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_delete_02i {} {
    bind_to_dsa
    set test_case_name "obj_delete_02i"
    set command "x500obj delete /c=de -attribute"
    set exp_result "Unknown option \"-attribute\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_delete_03i {} {
    bind_to_dsa
    set test_case_name "obj_delete_03i"
    set command "x500obj delete /c=de /c=de/o=sni"
    set exp_result "Too many values."
    set exp_error_code "GDSCP -533"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_delete_04i {} {
    bind_to_dsa
    set test_case_name "obj_delete_04i"
    set command "x500obj delete -attribute"
    set exp_result "Missing object name."
    set exp_error_code "GDSCP -504"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_delete_05i {} {
    bind_to_dsa
    set test_case_name "obj_delete_05i"
    set command "delete /c=de/o=sni,/ou=ap11"
    set exp_result "Missing Attribute Value Assertion - \"/c=de/o=sni,/ou=ap11\" : Error position <13>."
    set exp_error_code "GDSCP -331"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_delete_06i {} {
    bind_to_dsa
    set test_case_name "obj_delete_06i"
    set command "delete \"\{/c=de/o=sni/ou=ap11\""
    set exp_result "unmatched open brace in list"
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_delete_07i {} {
    bind_to_dsa
    set test_case_name "obj_delete_07i"
    set command "delete {{/c=de/o=sni/ou=ap11} {-attribute}}"
    set exp_result "Invalid argument \"{/c=de/o=sni/ou=ap11} {-attribute}\"."
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_delete_08i {} {
    bind_to_dsa
    set test_case_name "obj_delete_08i"
    set command "delete /c=de/o=xx/ou=yy"
    set exp_result "The specified name does not match the name of any object in the directory."
    set exp_error_code "GDSCP -34"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
