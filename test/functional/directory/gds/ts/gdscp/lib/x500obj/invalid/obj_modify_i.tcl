# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_modify_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:21  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:02  root
# 	Submit
# 	[1995/12/11  14:36:38  root]
# 
# Revision 1.1.2.5  1994/07/06  15:11:31  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:49  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:29:28  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:50:20  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:42  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:50  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:03  marrek
# 	Updated.
# 	[1994/05/11  10:25:00  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:28  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:43  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_modify_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:06 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_modify_i.tcl                              	  #
# Description : This file contains all the invalid test cases     #
#		for the modify operation on the x500obj object    #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_modify_01i {} {
    bind_to_dsa
    set test_case_name "obj_modify_01i"
    set command "x500obj modify"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_02i {} {
    bind_to_dsa
    set test_case_name "obj_modify_02i"
    set command "x500obj modify -types"
    set exp_result "Unknown option \"-types\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_03i {} {
    bind_to_dsa
    set test_case_name "obj_modify_03i"
    set command "x500obj modify -r"
    set exp_result "Ambiguous option \"-r\": -removeattr -rdn."
    set exp_error_code "GDSCP -532"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_04i {} {
    bind_to_dsa
    set test_case_name "obj_modify_04i"
    set command "x500obj modify /c=de/o=sni -types"
    set exp_result "Unknown option \"-types\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_05i {} {
    bind_to_dsa
    set test_case_name "obj_modify_05i"
    set command "x500obj modify /c=de/o=sni some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_06i {} {
    bind_to_dsa
    set test_case_name "obj_modify_06i"
    set command "x500obj modify /c=de/o=sni"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_07i {} {
    bind_to_dsa
    set test_case_name "obj_modify_07i"
    set command "x500obj modify /c=de/o=sni -addattr \"\{tn=1234\""
    set exp_result "unmatched open brace in list"
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_08i {} {
    bind_to_dsa
    set test_case_name "obj_modify_08i"
    set command "x500obj modify /c=de/o=sni -addattr {{tn=1234} {-rdn}}"
    set exp_result "Invalid argument \"{tn=1234} {-rdn}\"."
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_09i {} {
    bind_to_dsa
    set test_case_name "obj_modify_09i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -addattr tn=1234 -addattr sn=naik"
    set exp_result "Conflicting option \"-addattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_10i {} {
    bind_to_dsa
    set test_case_name "obj_modify_10i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -addattr tn=1234 -changeattr sn=naik"
    set exp_result "Conflicting option \"-changeattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_11i {} {
    bind_to_dsa
    set test_case_name "obj_modify_11i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -addattr"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_12i {} {
    bind_to_dsa
    set test_case_name "obj_modify_12i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -rdn cn=naik1 -addattr sn=naik"
    set exp_result "Conflicting option \"-addattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_13i {} {
    bind_to_dsa
    set test_case_name "obj_modify_13i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -rdn cn=naik1 -a sn=naik"
    set exp_result "Conflicting option \"-a\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_14i {} {
    bind_to_dsa
    set test_case_name "obj_modify_14i"
    set command "x500obj modify /c=de/o=sni -addattr c=de o=sni"
    set exp_result "Too many values."
    set exp_error_code "GDSCP -533"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_15i {} {
    bind_to_dsa
    set test_case_name "obj_modify_15i"
    set command "x500obj modify /c=de/o=sni/ou=/cn=naik -addattr tn=1234"
    set exp_result "Missing attribute value - \"/c=de/o=sni/ou=/cn=naik\" : Error position <16>."
    set exp_error_code "GDSCP -338"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_16i {} {
    bind_to_dsa
    set test_case_name "obj_modify_16i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -addattr \"tn=1234;\""
    set exp_result "Missing attribute value - \"tn=1234;\" : Error position <9>."
    set exp_error_code "GDSCP -338"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_17i {} {
    bind_to_dsa
    set test_case_name "obj_modify_17i"
    set command "x500obj modify /c=de/o=sni/ou=xx/cn=naik -addattr tn=1234"
    set exp_result "The specified name does not match the name of any object in the directory."
    set exp_error_code "GDSCP -34"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_18i {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute OCL=C"
    set test_case_name "obj_modify_18i"
    set command "x500obj modify /c=de -addattr c=de"
    set exp_result "An attempt is made to add an attribute or value that already exists.Violating attribute - Country-Name."
    set exp_error_code "GDSCP -5"
    run_testcase $test_case_name $command $exp_result $exp_error_code
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_modify_19i {} {
    bind_to_dsa
    set test_case_name "obj_modify_19i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -addattr tn=12345 -removeattr tn"
    set exp_result "Conflicting option \"-removeattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_20i {} {
    bind_to_dsa
    set test_case_name "obj_modify_20i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -changeattr tn=1234 tn=3456 -addattr sn=naik"
    set exp_result "Conflicting option \"-addattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_21i {} {
    bind_to_dsa
    set test_case_name "obj_modify_21i"
    set command "x500obj modify /c=de/o=sni/ou=xx/cn=naik -changeattr tn=1234899 tn=7895 some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_22i {} {
    bind_to_dsa
    set test_case_name "obj_modify_22i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -changeattr tn=1234 -changeattr sn=naik"
    set exp_result "Missing information for the new attribute values."
    set exp_error_code "GDSCP -506"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_23i {} {
    bind_to_dsa
    set test_case_name "obj_modify_23i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -changeattr tn=1234 tn=678 -changeattr sn=naik"
    set exp_result "Missing information for the new attribute values."
    set exp_error_code "GDSCP -506"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_24i {} {
    bind_to_dsa
    set test_case_name "obj_modify_24i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -changeattr tn=1234 tn=678 -removeattr sn=naik"
    set exp_result "Conflicting option \"-removeattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_25i {} {
    bind_to_dsa
    set test_case_name "obj_modify_25i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -changeattr"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_26i {} {
    bind_to_dsa
    set test_case_name "obj_modify_26i"
    set command "x500obj modify /c=de/o=sni/ou=ap11/cn=naik -changeattr -changeattr"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_27i {} {
    bind_to_dsa
    set test_case_name "obj_modify_27i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -changeattr \"tn=1234;\" tn=345"
    set exp_result "Missing attribute value - \"tn=1234;\" : Error position <9>."
    set exp_error_code "GDSCP -338"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_28i {} {
    bind_to_dsa
    set test_case_name "obj_modify_28i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -changeattr tn=345 \"tn=1234;\""
    set exp_result "Missing attribute value - \"tn=1234;\" : Error position <9>."
    set exp_error_code "GDSCP -338"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_29i {} {
    bind_to_dsa
    set test_case_name "obj_modify_29i"
    set command "modify /c=de/o=sni/ou=xx/cn=naik -changeattr tn=1234899 tn=7895"
    set exp_result "The specified name does not match the name of any object in the directory."
    set exp_error_code "GDSCP -34"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_30i {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute OCL=C"
    set test_case_name "obj_modify_30i"
    set command "modify /c=de -changeattr c=fr c=in"
    set exp_result "The specified attribute or value was not found in the directory entry.Violating attribute - Country-Name."
    set exp_error_code "GDSCP -33"
    run_testcase $test_case_name $command $exp_result $exp_error_code
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_modify_31i {} {
    bind_to_dsa
    set test_case_name "obj_modify_31i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -removeattr tn=1234 -addattr sn=naik"
    set exp_result "Conflicting option \"-addattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_32i {} {
    bind_to_dsa
    set test_case_name "obj_modify_32i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -removeattr tn=1234 -changeattr sn=naik"
    set exp_result "Conflicting option \"-changeattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_33i {} {
    bind_to_dsa
    set test_case_name "obj_modify_33i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -removeattr \"tn=1234;\""
    set exp_result "Missing attribute value - \"tn=1234;\" : Error position <9>."
    set exp_error_code "GDSCP -338"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_34i {} {
    bind_to_dsa
    set test_case_name "obj_modify_34i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -removeattr xyz"
    set exp_result "Abbreviation unknown - xyz."
    set exp_error_code "GDSCP -328"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_35i {} {
    bind_to_dsa
    set test_case_name "obj_modify_35i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -removeattr sn tn pa={someabbr=123}"
    set exp_result "Abbreviation unknown - \"pa={someabbr=123}\" : Error position <5>."
    set exp_error_code "GDSCP -328"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_36i {} {
    bind_to_dsa
    set test_case_name "obj_modify_36i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -removeattr sn -removeattr tn"
    set exp_result "Conflicting option \"-removeattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_37i {} {
    bind_to_dsa
    set test_case_name "obj_modify_37i"
    set command "modify /c=de/o=sni/ou=xx/cn=naik -removeattr tn=1234"
    set exp_result "The specified name does not match the name of any object in the directory."
    set exp_error_code "GDSCP -34"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_38i {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute OCL=C"
    set test_case_name "obj_modify_38i"
    set command "modify /c=de -removeattr c=de"
    set exp_result "The attribute or attribute value does not conform to the imposed constraints.Violating attribute - Country-Name."
    set exp_error_code "GDSCP -16"
    run_testcase $test_case_name $command $exp_result $exp_error_code
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_modify_39i {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute OCL=C"
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute OCL=ORG"
    set test_case_name "obj_modify_39i"
    set command "modify /c=de/o=sni -removeattr PA"
    set exp_result "The specified attribute or value was not found in the directory entry.Violating attribute - Postal-Address."
    set exp_error_code "GDSCP -33"
    run_testcase $test_case_name $command $exp_result $exp_error_code
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de/o=sni"
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_modify_40i {} {
    bind_to_dsa
    set test_case_name "obj_modify_40i"
    set command "modify /c=de/o=sni -rdn c=de o=sni"
    set exp_result "Too many values."
    set exp_error_code "GDSCP -533"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_41i {} {
    bind_to_dsa
    set test_case_name "obj_modify_41i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -rdn /c=de"
    set exp_result "Format error - \"/c=de\" : Error position <1>."
    set exp_error_code "GDSCP -333"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_42i {} {
    bind_to_dsa
    set test_case_name "obj_modify_42i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -rdn c=de -changeattr c=de"
    set exp_result "Conflicting option \"-changeattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_43i {} {
    bind_to_dsa
    set test_case_name "obj_modify_43i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -rdn c=de -removeattr c=de"
    set exp_result "Conflicting option \"-removeattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_44i {} {
    bind_to_dsa
    set test_case_name "obj_modify_44i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -rdn c=de -addattr c=de"
    set exp_result "Conflicting option \"-addattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_45i {} {
    bind_to_dsa
    set test_case_name "obj_modify_45i"
    set command "modify /c=de/o=sni/ou=ap11/xyz=naik -rdn /cn=naik"
    set exp_result "Abbreviation unknown - \"/c=de/o=sni/ou=ap11/xyz=naik\" : Error position <21>."
    set exp_error_code "GDSCP -328"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_46i {} {
    bind_to_dsa
    set test_case_name "obj_modify_46i"
    set command "modify /c=de/o=sni/ou=ap11/cn=naik -rdn cn=hello,sn"
    set exp_result "Missing equal operator - \"cn=hello,sn\" : Error position <12>."
    set exp_error_code "GDSCP -337"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_47i {} {
    bind_to_dsa
    set test_case_name "obj_modify_47i"
    set command "modify /c=de/o=sni/ou=xx/cn=naik -rdn cn=hello"
    set exp_result "The specified name does not match the name of any object in the directory."
    set exp_error_code "GDSCP -34"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_modify_48i {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute OCL=C"
    set test_case_name "obj_modify_48i"
    set command "modify /c=de -rdn OCL=ORG"
    set exp_result "The modification leaves the DIT structured incorrectly."
    set exp_error_code "GDSCP -31"
    run_testcase $test_case_name $command $exp_result $exp_error_code
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_modify_49i {} {
    bind_to_dsa
    run_cmd_and_log "set gdscp_t61 TRUE"
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute {{OCL=C}  \
	{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"
#            lat[desig. seq.  kanji][1.charac. 2.charac][des.seq.greek] [alpha beta gamma][des.seq.latin][ AE  OE   UE]lat

    set test_case_name "obj_modify_49i"
    set command "x500obj modify /C=DE -addattr \
         {{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xCF\\xD4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"
#              lat[desig. seq.  kanji][1.charac. invalid ][des.seq.greek] [alpha beta gamma][des.seq.latin][ AE  OE   UE]lat

    set exp_result "The attribute value does not conform to the attribute syntax of the attribute type.Violating attribute - Description."
    set exp_error_code "GDSCP -22"
    run_testcase $test_case_name $command $exp_result $exp_error_code
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
    run_cmd_and_log "set gdscp_t61 FALSE"
}

proc obj_modify_50i {} {
    bind_to_dsa
    run_cmd_and_log "set gdscp_t61 TRUE"
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute {{OCL=C}  \
	{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"
#            lat[desig. seq.  kanji][1.charac. 2.charac][des.seq.greek] [alpha beta gamma][des.seq.latin][ AE  OE   UE]lat

    set test_case_name "obj_modify_50i"
    set command "x500obj modify /C=DE -addattr \
         {{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xA4\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"
#              lat[desig. seq.  kanji][1.charac. 2.charac][des.seq.greek] [alpha invalid gamma][des.seq.latin][ AE  OE   UE]lat

    set exp_result "The attribute value does not conform to the attribute syntax of the attribute type.Violating attribute - Description."
    set exp_error_code "GDSCP -22"
    run_testcase $test_case_name $command $exp_result $exp_error_code
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
    run_cmd_and_log "set gdscp_t61 FALSE"
}

proc obj_modify_51i {} {
    bind_to_dsa
    run_cmd_and_log "set gdscp_t61 TRUE"
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute {{OCL=C}  \
	{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8O\\xC8UDEF}}"
#            lat[desig. seq.  kanji][1.charac. 2.charac][des.seq.greek] [alpha beta gamma][des.seq.latin][ AE  OE   UE]lat

    set test_case_name "obj_modify_51i"
    set command "x500obj modify /C=DE -addattr \
         {{DSC=ABC\\x1B\\x24\\x29\\x42\\xB0\\xA1\\xF4\\xA4\\x1B\\x2C\\x46\\xC1\\xC2\\xC3\\x1B\\x29\\x75\\xC8A\\xC8B\\xC8UDEF}}"
#              lat[desig. seq.  kanji][1.charac. 2.charac][des.seq.greek] [alpha beta gamma][des.seq.latin][ AE inval.UE]lat

    set exp_result "The attribute value does not conform to the attribute syntax of the attribute type.Violating attribute - Description."
    set exp_error_code "GDSCP -22"
    run_testcase $test_case_name $command $exp_result $exp_error_code
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
    run_cmd_and_log "set gdscp_t61 FALSE"
}
