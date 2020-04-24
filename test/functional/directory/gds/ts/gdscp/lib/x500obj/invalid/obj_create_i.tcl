# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_create_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:35:55  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:09  marty]
#
# Revision 1.1.4.1  1995/12/11  15:26:50  root
# 	Submit
# 	[1995/12/11  14:36:32  root]
# 
# Revision 1.1.2.5  1994/07/06  15:11:26  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:35  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:29:24  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:50:09  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:21  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:44  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:34:57  marrek
# 	Updated.
# 	[1994/05/11  10:24:46  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:14  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:26  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_create_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:55 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_create_i.tcl                              	  #
# Description : This file contains all the invalid test cases     #
#		for the create operation on the x500obj object    #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_create_01i {} {
    bind_to_dsa
    set test_case_name "obj_create_01i"
    set command "x500obj create"
    set exp_result "Missing object name."
    set exp_error_code "GDSCP -504"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_02i {} {
    bind_to_dsa
    set test_case_name "obj_create_02i"
    set command "x500obj create /c=de c=fr"
    set exp_result "Unknown argument \"c=fr\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_03i {} {
    bind_to_dsa
    set test_case_name "obj_create_03i"
    set command "x500obj create -attribute cn=naik"
    set exp_result "Missing object name."
    set exp_error_code "GDSCP -504"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_04i {} {
    bind_to_dsa
    set test_case_name "obj_create_04i"
    set command "x500obj create /c=de -attribute ffsf=naik"
    set exp_result "Abbreviation unknown - \"ffsf=naik\" : Error position <1>."
    set exp_error_code "GDSCP -328"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_05i {} {
    bind_to_dsa
    set test_case_name "obj_create_05i"
    set command "x500obj create /c=de//ou=ap11/cn=naik"
    set exp_result "Missing Attribute Value Assertion - \"/c=de//ou=ap11/cn=naik\" : Error position <7>."
    set exp_error_code "GDSCP -331"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_06i {} {
    bind_to_dsa
    set test_case_name "obj_create_06i"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute sn=naik -attribute tn=1234"
    set exp_result "Conflicting option \"-attribute\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_07i {} {
    bind_to_dsa
    set test_case_name "obj_create_07i"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute {{cn=\\x2}}"
    set exp_result "Format error - \"cn=\\x2\" : Error position <5>."
    set exp_error_code "GDSCP -333"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_08i {} {
    bind_to_dsa
    set test_case_name "obj_create_08i"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute cn='1234"
    set exp_result "Mismatched quotes - \"cn='1234\" : Error position <9>."
    set exp_error_code "GDSCP -336"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_09i {} {
    bind_to_dsa
    set test_case_name "obj_create_09i"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute cn"
    set exp_result "Missing equal operator - \"cn\" : Error position <3>."
    set exp_error_code "GDSCP -337"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_10i {} {
    bind_to_dsa
    set test_case_name "obj_create_10i"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute cn="
    set exp_result "Missing attribute value - \"cn=\" : Error position <4>."
    set exp_error_code "GDSCP -338"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_11i {} {
    bind_to_dsa
    set test_case_name "obj_create_11i"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute TXN=TN=12345"
    set exp_result "Missing start of class block operator - \"TXN=TN=12345\" : Error position <5>."
    set exp_error_code "GDSCP -340"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_12i {} {
    bind_to_dsa
    set test_case_name "obj_create_12i"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute TXN="
    set exp_result "Missing class value - \"TXN=\" : Error position <5>."
    set exp_error_code "GDSCP -342"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_13i {} {
    bind_to_dsa
    set test_case_name "obj_create_13i"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute TXN=\{TN=12345"
    set exp_result "Mismatched brackets - \"TXN=\{TN=12345\" : Error position <14>."
    set exp_error_code "GDSCP -344"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_14i {} {
    bind_to_dsa
    set test_case_name "obj_create_14i"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute TXN={TN={12345}}"
    set exp_result "Unexpected operator - \"TXN={TN={12345}}\" : Error position <9>."
    set exp_error_code "GDSCP -345"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_15i {} {
    bind_to_dsa
    set test_case_name "obj_create_15i"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute sn=naik -a tn=1234"
    set exp_result "Conflicting option \"-a\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_16i {} {
    bind_to_dsa
    set test_case_name "obj_create_16i"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_17i {} {
    bind_to_dsa
    set test_case_name "obj_create_17i"
    set command "x500obj create /c=de/o=sni,=ap11/cn=naik"
    set exp_result "Missing abbreviation - \"/c=de/o=sni,=ap11/cn=naik\" : Error position <13>."
    set exp_error_code "GDSCP -332"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_18i {} {
    bind_to_dsa
    set test_case_name "obj_create_18i"
    set command "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute cn=naik sn"
    set exp_result "Missing equal operator - \"sn\" : Error position <3>."
    set exp_error_code "GDSCP -337"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_19i {} {
    bind_to_dsa
    set test_case_name "obj_create_19i"
    set command "create /c=de/o=sni/ou=ap11/cn=naik -attribute {{TXN={TN=12345;67890,CC=111;222  ; ,AB=90;333;444;}}}"
    set exp_result "Missing component value - \"TXN={TN=12345;67890,CC=111;222  ; ,AB=90;333;444;}\" : Error position <35>."
    set exp_error_code "GDSCP -343"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_20i {} {
    bind_to_dsa
    set test_case_name "obj_create_20i"
    set command "create /c=de -rdn"
    set exp_result "Unknown option \"-rdn\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_21i {} {
    bind_to_dsa
    set test_case_name "obj_create_21i"
    set command "create /c=de -attribute \"\{cn=naik\""
    set exp_result "unmatched open brace in list"
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_22i {} {
    bind_to_dsa
    set test_case_name "obj_create_22i"
    set command "create /c=de -attribute {{ocl=c} {-attribute}}"
    set exp_result "Invalid argument \"{ocl=c} {-attribute}\"."
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_23i {} {
    bind_to_dsa
    set test_case_name "obj_create_23i"
    set command "create /c=de some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_create_24i {} {
    bind_to_dsa
    set test_case_name "obj_create_24i"
    set command "create /c=de/o=xxx/ou=yyy"
    set exp_result "The specified name does not match the name of any object in the directory."
    set exp_error_code "GDSCP -34"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
