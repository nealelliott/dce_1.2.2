# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_list_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:29  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:36  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:34  root
# 	Submit
# 	[1995/12/11  14:36:48  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:44  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:10  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:23:06  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:07  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:12  marrek
# 	Updated.
# 	[1994/05/11  10:25:25  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:57  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:11  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_list_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:29 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_list_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the list operation on the x500obj object      #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_list_01v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_list_01v"
    set command "x500obj list /"
    set exp_result "/C=de"
    run_testcase_and_extract $test_case_name $command $exp_result 2
    remove_dit_objects
}

proc obj_list_02v {} {
    bind_to_dsa
    create_dit_objects
    create_dit_mail_objects
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni/ou=ap11"
    set test_case_name "obj_list_02v"
    set command "x500obj l"
    set exp_result "/C=de/O=sni/OU=ap11/CN=naik,OU=ap11"
    run_testcase_and_extract $test_case_name $command $exp_result 11
    remove_dit_mail_objects
    remove_dit_objects
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_list_03v {} {
    bind_to_dsa
    create_dit_objects
    create_dit_mail_objects
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni"
    set test_case_name "obj_list_03v"
    set command "x500obj list ou=ap11"
    set exp_result "/C=de/O=sni/OU=ap11/CN=Andrews"
    run_testcase_and_extract $test_case_name $command $exp_result 10
    remove_dit_mail_objects
    remove_dit_objects
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_list_04v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_list_04v"
    set command "list /c=de"
    set exp_result "/C=de/O=sni"
    run_testcase_and_extract $test_case_name $command $exp_result 1
    remove_dit_objects
}

proc obj_list_05v {} {
    bind_to_dsa
    create_object_with_local_strings
    set test_case_name "obj_list_05v"
    set command "x500obj list /c=de/o=sni/ou=ap11"
    set exp_result "/C=de/O=sni/OU=ap11/CN=müller"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_object_with_local_strings
}

proc obj_list_06v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "set gdscp_t61 TRUE"
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller -attribute ocl=orp sn=m\\\\xc8uller"
    run_cmd_and_log "set gdscp_t61 FALSE"
    set test_case_name "obj_list_06v"
    set command "x500obj list /c=de/o=sni/ou=ap11"
    set exp_result "/C=de/O=sni/OU=ap11/CN=müller"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=müller"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}
