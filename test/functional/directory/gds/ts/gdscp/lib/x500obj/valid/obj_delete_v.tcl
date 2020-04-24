# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_delete_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:34  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:28  root
# 	Submit
# 	[1995/12/11  14:36:46  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:44  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:08  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:23:05  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:05  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:11  marrek
# 	Updated.
# 	[1994/05/11  10:25:21  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:53  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:08  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_delete_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:25 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_delete_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the delete operation on the x500obj object    #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_delete_01v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute ocl=orp sn=naik"
    set test_case_name "obj_delete_01v"
    set command "x500obj delete /c=de/o=sni/ou=ap11/cn=naik"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj search /c=de/o=sni/ou=ap11 -subtree"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_delete_02v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute ocl=orp sn=naik"
    set test_case_name "obj_delete_02v"
    set command "x500obj d /c=de/o=sni/ou=ap11/cn=naik"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj search /c=de/o=sni/ou=ap11 -subtree"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_delete_03v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=naik,ou=ap11 -attribute ocl=orp sn=naik"
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni/ou=ap11"
    set test_case_name "obj_delete_03v"
    set command "x500obj d cn=naik,ou=ap11"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj search /c=de/o=sni/ou=ap11 -subtree"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_delete_04v {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"
    set test_case_name "obj_delete_04v"
    set command "delete /c=de/o=sni"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj search /c=de -subtree"
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_delete_05v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=müller -attribute ocl=orp sn=müller"
    set test_case_name "obj_delete_05v"
    set command "x500obj delete /c=de/o=sni/ou=ap11/cn=müller"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj list /c=de/o=sni/ou=ap11"
    remove_dit_objects
}

proc obj_delete_06v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=müller -attribute ocl=orp sn=müller"
    run_cmd_and_log "set gdscp_t61 TRUE"
    set test_case_name "obj_delete_06v"
    set command "x500obj delete /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "set gdscp_t61 FALSE"
    run_cmd_and_log "x500obj list /c=de/o=sni/ou=ap11"
    remove_dit_objects
}
