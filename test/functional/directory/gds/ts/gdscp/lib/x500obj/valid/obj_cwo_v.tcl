# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_cwo_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:22  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:33  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:25  root
# 	Submit
# 	[1995/12/11  14:36:45  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:43  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:06  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:23:04  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:04  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:10  marrek
# 	Updated.
# 	[1994/05/11  10:25:19  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:50  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:05  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_cwo_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:22 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_cwo_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the manipulation of the "gdscp_cwo" variable  #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_cwo_01v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_cwo_01v"
    set command "set gdscp_cwo /c=de/o=sni/ou=ap11/cn=naik"
    set exp_result "/c=de/o=sni/ou=ap11/cn=naik"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "set gdscp_cwo /"
    remove_dit_objects
}

proc obj_cwo_02v {} {
    bind_to_dsa
    set test_case_name "obj_cwo_02v"
    set command "set gdscp_cwo /"
    set exp_result "/"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_cwo_03v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni"
    set test_case_name "obj_cwo_03v"
    set command "set gdscp_cwo ou=ap11/cn=naik"
    set exp_result "/c=de/o=sni/ou=ap11/cn=naik"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_cwo_04v {} {
    bind_to_dsa
    set test_case_name "obj_cwo_04v"
    set command "set gdscp_cwo \"  /   \""
    set exp_result "/"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_cwo_05v {} {
    bind_to_dsa
    create_object_with_local_strings
    set test_case_name "obj_cwo_05v"
    set command "set gdscp_cwo /c=de/o=sni/ou=ap11/cn=müller"
    set exp_result "/c=de/o=sni/ou=ap11/cn=müller"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_object_with_local_strings
}
