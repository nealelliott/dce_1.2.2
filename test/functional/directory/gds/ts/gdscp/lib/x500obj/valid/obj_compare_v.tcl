# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_compare_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:18  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:30  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:19  root
# 	Submit
# 	[1995/12/11  14:36:43  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:36  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:01  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:48  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:01  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:08  marrek
# 	Updated.
# 	[1994/05/11  10:25:13  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:43  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:59  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_compare_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:18 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_compare_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the compare operation on the x500obj object   #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_compare_01v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_compare_01v"
    set command "x500obj compare /c=de/o=sni/ou=ap11/cn=naik -attribute tn=12345"
    set exp_result "TRUE"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
}

proc obj_compare_02v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni/ou=ap11"
    set test_case_name "obj_compare_02v"
    set command "x500obj compare cn=naik -attribute tn=12345"
    set exp_result "TRUE"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_compare_03v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni/ou=ap11/cn=naik"
    set test_case_name "obj_compare_03v"
    set command "x500obj co -attribute tn=12345"
    set exp_result "TRUE"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_compare_04v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_compare_04v"
    set command "x500obj comp /c=de/o=sni/ou=ap11/cn=naik -a tn=492224"
    set exp_result "FALSE"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
}

proc obj_compare_05v {} {
    bind_to_dsa
    create_dit_objects
    create_dit_mail_objects
    set test_case_name "obj_compare_05v"
    set command "compare /c=de/o=sni/ou=ap11/cn=mail2 -attribute {{mds={pt=0, ind={dn={/c=de/o=sni/ou=ap11/cn=ind1}, admd=admd, c=de,o=orgname,prmd=prmd}}}}"
    set exp_result "TRUE"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_mail_objects
    remove_dit_objects
}

proc obj_compare_06v {} {
    bind_to_dsa
    create_dit_objects
    create_dit_mail_objects
    set test_case_name "obj_compare_06v"
    set command "compare /c=de/o=sni/ou=ap11/cn=mail2 -attribute {{mds={pt=0, ind={dn={/c=de/o=sni/ou=ap11/cn=xyz}, admd=admd, c=de,o=orgname,prmd=prmd}}}}"
    set exp_result "FALSE"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_mail_objects
    remove_dit_objects
}

proc obj_compare_07v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_compare_07v"
    set command "compare /c=de/o=sni/ou=ap11/cn=naik,ou=ap11 -attribute ou=ap11"
    set exp_result "TRUE"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
}

proc obj_compare_08v {} {
    bind_to_dsa
    create_object_with_local_strings
    set test_case_name "obj_compare_08v"
    set command "x500obj compare /c=de/o=sni/ou=ap11/cn=müller -attribute sn=müller"
    set exp_result "TRUE"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_object_with_local_strings
}

proc obj_compare_09v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_t61 TRUE"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller -attribute ocl=orp sn=m\\\\xc8uller"
    set test_case_name "obj_compare_09v"
    set command "x500obj compare /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller -attribute sn=m\\\\xc8uller"
    set exp_result "TRUE"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller"
    run_cmd_and_log "set gdscp_t61 FALSE"
    remove_dit_objects
}

proc obj_compare_10v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_t61 TRUE"
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller -attribute ocl=orp sn=m\\\\xc8uller"
    run_cmd_and_log "set gdscp_t61 FALSE"
    set test_case_name "obj_compare_10v"
    set command "x500obj compare /c=de/o=sni/ou=ap11/cn=müller -attribute sn=müller"
    set exp_result "TRUE"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=müller"
    remove_dit_objects
}
