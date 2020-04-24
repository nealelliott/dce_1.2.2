# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_bind_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:16  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:29  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:16  root
# 	Submit
# 	[1995/12/11  14:36:42  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:36  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:59  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:48  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:00  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:08  marrek
# 	Updated.
# 	[1994/05/11  10:25:11  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:41  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:57  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_bind_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:16 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_bind_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the bind operation on the x500obj object      #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_bind_01v {} {
    set test_case_name "obj_bind_01v"
    set command "x500obj bind"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_02v {} {
    set test_case_name "obj_bind_02v"
    set command "x500obj b -cache"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_03v {} {
    set test_case_name "obj_bind_03v"
    set command "x500obj bind -c"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_04v {} {
    global directory_id;

    set test_case_name "obj_bind_04v"
    set command "x500obj bi -cache -cache -dirid $directory_id"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_bind_05v {} {
    global directory_id dsa_name;

    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_bind_05v"
    set command "x500obj bind -dsa $dsa_name -dirid $directory_id -user /c=de/o=sni/ou=ap11/cn=naik -password naik -authentication SIMPLE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
}

proc obj_bind_06v {} {
    global directory_id dsa_name;

    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_bind_06v"
    set command "x500obj bind -ds $dsa_name -di $directory_id -u /c=de/o=sni/ou=ap11/cn=naik -pa naik -a SIMPLE "
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
}

proc obj_bind_07v {} {
    global psap_address;

    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_bind_07v"
    set command "bind -psap $psap_address -user /c=de/o=sni/ou=ap11/cn=naik -password naik -authentication SIMPLE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
}

proc obj_bind_08v {} {
    global psap_address;

    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_bind_08v"
    set command "bind -ps $psap_address -u /c=de/o=sni/ou=ap11/cn=naik -pa naik -authentication SIMPLE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
}

proc obj_bind_09v {} {
    global dsa_name directory_id;

    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_bind_09v"
    set command "bind -dsa $dsa_name -dirid $directory_id -user /c=de/o=sni/ou=ap11/cn=naik -password {{   \\x6e\\x61\\x69\\x6b    }} -authentication SIMPLE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
}

proc obj_bind_10v {} {
    global dsa_name directory_id;

    bind_to_dsa
    create_dit_objects
    set test_case_name "obj_bind_10v"
    set command "bind -dsa $dsa_name -dirid $directory_id -user /c=de/o=sni/ou=ap11/cn=naik -password {{   \\n\\a\\i\\k    }} -authentication SIMPLE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    remove_dit_objects
}

#proc obj_bind_11v {} {
#    global dsa_name directory_id;
#
#    set test_case_name "obj_bind_11v"
#    set command "bind -dsa $dsa_name -dirid $directory_id -authentication DCE"
#    set exp_result ""
#    set exp_error_code ""
#    run_testcase $test_case_name $command $exp_result $exp_error_code
#}
