# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_show_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:13  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:27  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:11  root
# 	Submit
# 	[1995/12/11  14:36:41  root]
# 
# Revision 1.1.2.5  1994/07/06  15:11:34  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:57  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:29:30  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:50:27  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:46  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:53  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:06  marrek
# 	Updated.
# 	[1994/05/11  10:25:08  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:35  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:53  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_show_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:13 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_show_i.tcl                              	  #
# Description : This file contains all the invalid test cases     #
#		for the show operation on the x500obj object      #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_show_01i {} {
    bind_to_dsa
    set test_case_name "obj_show_01i"
    set command "x500obj show /c=de some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_02i {} {
    bind_to_dsa
    set test_case_name "obj_show_02i"
    set command "x500obj show -baseobject"
    set exp_result "Unknown option \"-baseobject\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_03i {} {
    bind_to_dsa
    set test_case_name "obj_show_03i"
    set command "x500obj show {{/c=de/  o = sni, /ou=ap11}}"
    set exp_result "Missing Attribute Value Assertion - \"/c=de/  o = sni, /ou=ap11\" : Error position <18>."
    set exp_error_code "GDSCP -331"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_04i {} {
    bind_to_dsa
    set test_case_name "obj_show_04i"
    set command "x500obj show /c=de/o=sni/ou=ap11/cn=naik -attribute cn tn -types"
    set exp_result "Conflicting option \"-types\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_05i {} {
    bind_to_dsa
    set test_case_name "obj_show_05i"
    set command "x500obj show /c=de/o=sni/ou=ap11/cn=naik -types -attribute cn tn"
    set exp_result "Conflicting option \"-attribute\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_06i {} {
    bind_to_dsa
    set test_case_name "obj_show_06i"
    set command "x500obj show /c=de/o=sni/ou=ap11/cn=naik -attribute"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_07i {} {
    bind_to_dsa
    set test_case_name "obj_show_07i"
    set command "x500obj show /c=de/o=sni/ou=ap11/cn=naik -attribute sn -attribute tn"
    set exp_result "Conflicting option \"-attribute\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_08i {} {
    bind_to_dsa
    set test_case_name "obj_show_08i"
    set command "x500obj show /c=de/o=sni/ou=ap11/cn=naik -attribute -types"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_09i {} {
    bind_to_dsa
    set test_case_name "obj_show_09i"
    set command "show /c=de/o=sni/ou=ap11/cn=naik -attribute xyz"
    set exp_result "Abbreviation unknown - xyz."
    set exp_error_code "GDSCP -328"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_10i {} {
    bind_to_dsa
    set test_case_name "obj_show_10i"
    set command "show /c=de/o=sni/ou=ap11/cn=naik -types -attribute cn"
    set exp_result "Conflicting option \"-attribute\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_11i {} {
    bind_to_dsa
    set test_case_name "obj_show_11i"
    set command "show /c=de/o=sni/ou=ap11/cn=naik -t -a cn"
    set exp_result "Conflicting option \"-a\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_12i {} {
    bind_to_dsa
    set test_case_name "obj_show_12i"
    set command "show \"\{/c=de/o=sni/ou=ap11/cn=naik\""
    set exp_result "unmatched open brace in list"
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_13i {} {
    bind_to_dsa
    set test_case_name "obj_show_13i"
    set command "show {{/c=de/o=sni/ou=ap11/cn=naik} {-attribute}}"
    set exp_result "Invalid argument \"{/c=de/o=sni/ou=ap11/cn=naik} {-attribute}\"."
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_14i {} {
    bind_to_dsa
    set test_case_name "obj_show_14i"
    set command "show -types -attribute cn"
    set exp_result "Conflicting option \"-attribute\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_15i {} {
    bind_to_dsa
    set test_case_name "obj_show_15i"
    set command "show /c=de/o=sni/ou=ap11/cn=naik -filter"
    set exp_result "Unknown option \"-filter\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_16i {} {
    bind_to_dsa
    set test_case_name "obj_show_16i"
    set command "show /c=de/o=sni/ou=ap11/cn=naik some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_17i {} {
    bind_to_dsa
    set test_case_name "obj_show_17i"
    set command "show {{/c=de/o=sni/ou=ap11/cn='not present'}}"
    set exp_result "The specified name does not match the name of any object in the directory."
    set exp_error_code "GDSCP -34"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_show_18i {} {
    bind_to_dsa
    set test_case_name "obj_show_18i"
    set command "show /"
    set exp_result "The specified name does not match the name of any object in the directory."
    set exp_error_code "GDSCP -34"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
