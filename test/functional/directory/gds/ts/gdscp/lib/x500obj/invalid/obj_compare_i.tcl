# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_compare_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:35:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:07  marty]
#
# Revision 1.1.4.1  1995/12/11  15:26:48  root
# 	Submit
# 	[1995/12/11  14:36:31  root]
# 
# Revision 1.1.2.5  1994/07/06  15:11:25  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:32  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:29:23  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:50:06  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:20  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:43  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:34:56  marrek
# 	Updated.
# 	[1994/05/11  10:24:43  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:12  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:23  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_compare_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:53 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_compare_i.tcl                              	  #
# Description : This file contains all the invalid test cases     #
#		for the compare operation on the x500obj object   #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_compare_01i {} {
    bind_to_dsa
    set test_case_name "obj_compare_01i"
    set command "x500obj compare"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_02i {} {
    bind_to_dsa
    set test_case_name "obj_compare_02i"
    set command "x500obj compare -dsa"
    set exp_result "Unknown option \"-dsa\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_03i {} {
    bind_to_dsa
    set test_case_name "obj_compare_03i"
    set command "x500obj compare /c=de/o=sni -types"
    set exp_result "Unknown option \"-types\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_04i {} {
    bind_to_dsa
    set test_case_name "obj_compare_04i"
    set command "x500obj compare /c=de/o=sni/ou=ap11"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_05i {} {
    bind_to_dsa
    set test_case_name "obj_compare_05i"
    set command "x500obj compare /c=de/o=sni/ou=ap11 some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_06i {} {
    bind_to_dsa
    set test_case_name "obj_compare_06i"
    set command "x500obj compare -attribute c=de -attribute o=sni"
    set exp_result "Conflicting option \"-attribute\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_07i {} {
    bind_to_dsa
    set test_case_name "obj_compare_07i"
    set command "x500obj compare -attribute c=de -a o=sni"
    set exp_result "Conflicting option \"-a\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_08i {} {
    bind_to_dsa
    set test_case_name "obj_compare_08i"
    set command "compare /c=de/o=sni/ou=ap11 -attribute"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_09i {} {
    bind_to_dsa
    set test_case_name "obj_compare_09i"
    set command "compare /c=de/o=sni/ou=ap11 -attribute \"  \""
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_10i {} {
    bind_to_dsa
    set test_case_name "obj_compare_10i"
    set command "compare /c=de/o=sni/ou=ap11 -attribute c=de o=sni"
    set exp_result "Too many values."
    set exp_error_code "GDSCP -533"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_11i {} {
    bind_to_dsa
    set test_case_name "obj_compare_11i"
    set command "compare /c=de/o=sni/ou=ap11 -attribute \"\{cn=nai\""
    set exp_result "unmatched open brace in list"
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_12i {} {
    bind_to_dsa
    set test_case_name "obj_compare_12i"
    set command "compare /c=de/o=sni/ou=ap11 -attribute {{cn=naik} {-attri}}"
    set exp_result "Invalid argument \"{cn=naik} {-attri}\"."
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_13i {} {
    bind_to_dsa
    set test_case_name "obj_compare_13i"
    set command "compare /c=de/o=sni/ou=ap11/cn=naik -attribute \"tn=12345;67689\""
    set exp_result "Too many attribute values."
    set exp_error_code "GDSCP -223"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_14i {} {
    bind_to_dsa
    set test_case_name "obj_compare_14i"
    set command "compare /c=de/o=sni/ou=ap11/cn=some_name -attribute tn=12345"
    set exp_result "The specified name does not match the name of any object in the directory."
    set exp_error_code "GDSCP -34"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_15i {} {
    bind_to_dsa
    set test_case_name "obj_compare_15i"
    set command "compare /c=de/o=sni/ou=/cn=naik -attribute tn=12345"
    set exp_result "Missing attribute value - \"/c=de/o=sni/ou=/cn=naik\" : Error position <16>."
    set exp_error_code "GDSCP -338"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_16i {} {
    bind_to_dsa
    set test_case_name "obj_compare_16i"
    set command "compare /c=de/o=sni/ou=ap11/cn=naik -attribute tn=12345 -attribute sn=naik"
    set exp_result "Conflicting option \"-attribute\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_compare_17i {} {
    bind_to_dsa
    set test_case_name "obj_compare_17i"
    set command "compare /c=de/o=sni/ou=ap11/cn=naik -attribute \"tn=12345;\""
    set exp_result "Missing attribute value - \"tn=12345;\" : Error position <10>."
    set exp_error_code "GDSCP -338"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
