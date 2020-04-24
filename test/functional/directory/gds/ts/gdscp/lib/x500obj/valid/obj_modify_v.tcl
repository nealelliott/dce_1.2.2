# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_modify_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:37  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:38  root
# 	Submit
# 	[1995/12/11  14:36:48  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:45  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:14  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:23:07  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:08  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:13  marrek
# 	Updated.
# 	[1994/05/11  10:25:28  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:35:01  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:14  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_modify_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:31 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_modify_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the modify operation on the x500obj object    #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_modify_01v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni/ou=ap11/cn=naik"
    set test_case_name "obj_modify_01v"
    set command "x500obj modify -removeattr \"tn=369072;576268\""
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=naik"
    remove_dit_objects
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_modify_02v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_modify_02v"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -removeattr tn"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=naik"
    remove_dit_objects
}

proc obj_modify_03v {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "set gdscp_cwo /c=de/o=sni/ou=ap11"
    run_cmd_and_log "x500obj modify cn=naik -removeattr tn"
    set test_case_name "obj_modify_03v"
    set command "x500obj modify cn=naik -addattr \"tn=12345;369072;576268;9999\""
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=naik"
    remove_dit_objects
    run_cmd_and_log "set gdscp_cwo /"
}

proc obj_modify_04v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_modify_04v"
    set command "x500obj m /c=de/o=sni/ou=ap11/cn=naik -a dsc=xx"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=naik"
    remove_dit_objects
}

proc obj_modify_05v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_modify_05v"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -rem tn=12345 up"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=naik"
    remove_dit_objects
}

proc obj_modify_06v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_modify_06v"
    set command "x500obj mod /c=de/o=sni/ou=ap11/cn=mueller -changeattr dsc=Engineer dsc=Manager"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=mueller"
    remove_dit_objects
}

proc obj_modify_07v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_modify_07v"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -changeattr tn=12345 tn=56789 -changeattr sn=naik sn=bhavesh"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=naik"
    remove_dit_objects
}

proc obj_modify_08v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_modify_08v"
    set command "modify /c=de/o=sni/ou=ap11/cn=mueller -changeattr {dsc=Engineer dsc=Manager}"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=mueller"
    remove_dit_objects
}

proc obj_modify_09v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_modify_09v"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -c tn=12345 tn=56789"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=naik"
    remove_dit_objects
}

proc obj_modify_10v {} {
    bind_to_dsa
    create_dit_objects
    create_dit_mail_objects
    set test_case_name "obj_modify_10v"
    set command "modify /c=de/o=sni/ou=ap11/cn=mail1 -rdn cn=mail10"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=mail10"
    run_cmd_and_log "x500obj modify /c=de/o=sni/ou=ap11/cn=mail10 -rdn cn=mail1"
    remove_dit_mail_objects
    remove_dit_objects
}

proc obj_modify_11v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_modify_11v"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik,ou=ap11 -rd cn=naik1,ou=kp13"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=naik1,ou=kp13"
    run_cmd_and_log "x500obj modify /c=de/o=sni/ou=ap11/cn=naik1,ou=kp13 -rdn cn=naik,ou=ap11"
    remove_dit_objects
}

proc obj_modify_12v {} {
    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_modify_12v"
    run_cmd_and_log "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -a dsc=xx"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -add dsc=yy"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=naik"
    remove_dit_objects
}

proc obj_modify_13v {} {
    bind_to_dsa
    create_object_with_local_strings
    set test_case_name "obj_modify_13v"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=müller -addattr cn=näk"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=müller"
    remove_object_with_local_strings
}

proc obj_modify_14v {} {
    bind_to_dsa
    create_object_with_local_strings
    run_cmd_and_log "x500obj modify /c=de/o=sni/ou=ap11/cn=müller -addattr cn=näk"
    set test_case_name "obj_modify_14v"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=müller -changeattr cn=näk cn=äöü"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=müller"
    remove_object_with_local_strings
}

proc obj_modify_15v {} {
    bind_to_dsa
    create_object_with_local_strings
    run_cmd_and_log "x500obj modify /c=de/o=sni/ou=ap11/cn=müller -addattr cn=näk"
    set test_case_name "obj_modify_15v"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=müller -removeattr cn=näk"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=müller"
    remove_object_with_local_strings
}

proc obj_modify_16v {} {
    bind_to_dsa
    create_object_with_local_strings
    set test_case_name "obj_modify_16v"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=müller -rdn cn=näk"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=näk"
    run_cmd_and_log "x500obj modify /c=de/o=sni/ou=ap11/cn=näk -rdn cn=müller"
    remove_object_with_local_strings
}

proc obj_modify_17v {} {
    bind_to_dsa
    create_object_with_local_strings
    run_cmd_and_log "set gdscp_t61 TRUE"
    set test_case_name "obj_modify_17v"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller -addattr cn=n\\\\xc8uk"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "set gdscp_t61 FALSE"
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=müller"
    remove_object_with_local_strings
}

proc obj_modify_18v {} {
    bind_to_dsa
    create_object_with_local_strings
    run_cmd_and_log "x500obj modify /c=de/o=sni/ou=ap11/cn=müller -addattr cn=nük"
    run_cmd_and_log "set gdscp_t61 TRUE"
    set test_case_name "obj_modify_18v"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller -changeattr cn=n\\\\xc8uk cn=\\\\xc8o\\\\xc8u"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "set gdscp_t61 FALSE"
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=müller"
    remove_object_with_local_strings
}

proc obj_modify_19v {} {
    bind_to_dsa
    create_object_with_local_strings
    run_cmd_and_log "x500obj modify /c=de/o=sni/ou=ap11/cn=müller -addattr cn=nük"
    run_cmd_and_log "set gdscp_t61 TRUE"
    set test_case_name "obj_modify_19v"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller -removeattr cn=n\\\\xc8uk"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "set gdscp_t61 FALSE"
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=müller"
    remove_object_with_local_strings
}

proc obj_modify_20v {} {
    bind_to_dsa
    create_object_with_local_strings
    run_cmd_and_log "set gdscp_t61 TRUE"
    set test_case_name "obj_modify_20v"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=m\\\\xc8uller -rdn cn=n\\\\xc8uk"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "set gdscp_t61 FALSE"
    run_cmd_and_log "x500obj show /c=de/o=sni/ou=ap11/cn=nük"
    run_cmd_and_log "x500obj modify /c=de/o=sni/ou=ap11/cn=nük -rdn cn=müller"
    remove_object_with_local_strings
}
