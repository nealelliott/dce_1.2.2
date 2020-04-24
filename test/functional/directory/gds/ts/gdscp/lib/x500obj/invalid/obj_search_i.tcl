# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_search_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:11  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:25  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:08  root
# 	Submit
# 	[1995/12/11  14:36:40  root]
# 
# Revision 1.1.2.5  1994/07/06  15:11:33  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:54  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:29:29  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:50:24  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:22:44  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:52  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:05  marrek
# 	Updated.
# 	[1994/05/11  10:25:06  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:33  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:50  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_search_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:11 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_search_i.tcl                              	  #
# Description : This file contains all the invalid test cases     #
#		for the search operation on the x500obj object    #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc obj_search_01i {} {
    bind_to_dsa
    set test_case_name "obj_search_01i"
    set command "x500obj search"
    set exp_result "Missing scope of search."
    set exp_error_code "GDSCP -512"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_02i {} {
    bind_to_dsa
    set test_case_name "obj_search_02i"
    set command "x500obj search -rdn"
    set exp_result "Unknown option \"-rdn\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_03i {} {
    bind_to_dsa
    set test_case_name "obj_search_03i"
    set command "x500obj search -a"
    set exp_result "Ambiguous option \"-a\": -allattr -attribute."
    set exp_error_code "GDSCP -532"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_04i {} {
    bind_to_dsa
    set test_case_name "obj_search_04i"
    set command "x500obj search /c=de some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_05i {} {
    bind_to_dsa
    set test_case_name "obj_search_05i"
    set command "x500obj search -filter"
    set exp_result "Missing filter value."
    set exp_error_code "GDSCP -511"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_06i {} {
    bind_to_dsa
    set test_case_name "obj_search_06i"
    set command "x500obj search -filter {{c=de && ( )}} -baseobject"
    set exp_result "Missing attribute information - \"c=de && ( )\" : Error position <11>."
    set exp_error_code "GDSCP -339"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_07i {} {
    bind_to_dsa
    set test_case_name "obj_search_07i"
    set command "x500obj search -filter {{c=de & o=sni}} -baseobject"
    set exp_result "Unknown keyword encountered - \"c=de & o=sni\" : Error position <6>."
    set exp_error_code "GDSCP -347"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_08i {} {
    bind_to_dsa
    set test_case_name "obj_search_08i"
    set command "x500obj search -filter {{c = de && cn ~ naik}} -baseobject"
    set exp_result "Missing operator - \"c = de && cn ~ naik\" : Error position <15>."
    set exp_error_code "GDSCP -348"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_09i {} {
    bind_to_dsa
    set test_case_name "obj_search_09i"
    set command "x500obj search -filter {{c=de cn=naik}} -baseobject"
    set exp_result "Missing compound operator - \"c=de cn=naik\" : Error position <6>."
    set exp_error_code "GDSCP -349"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_10i {} {
    bind_to_dsa
    set test_case_name "obj_search_10i"
    set command "x500obj search -attribute"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_11i {} {
    bind_to_dsa
    set test_case_name "obj_search_11i"
    set command "x500obj search -attribute -types"
    set exp_result "Missing attribute information."
    set exp_error_code "GDSCP -505"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_12i {} {
    bind_to_dsa
    set test_case_name "obj_search_12i"
    set command "x500obj search -attribute CN tn -types"
    set exp_result "Conflicting option \"-types\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_13i {} {
    bind_to_dsa
    set test_case_name "obj_search_13i"
    set command "x500obj search -attribute CN tn -t"
    set exp_result "Conflicting option \"-t\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_14i {} {
    bind_to_dsa
    set test_case_name "obj_search_14i"
    set command "x500obj search -types -attribute CN tn"
    set exp_result "Conflicting option \"-attribute\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_15i {} {
    bind_to_dsa
    set test_case_name "obj_search_15i"
    set command "x500obj search -attribute CN tn -allattr"
    set exp_result "Conflicting option \"-allattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_16i {} {
    bind_to_dsa
    set test_case_name "obj_search_16i"
    set command "x500obj search -attribute CN -baseobject -onelevel"
    set exp_result "Conflicting option \"-onelevel\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_17i {} {
    bind_to_dsa
    set test_case_name "obj_search_17i"
    set command "x500obj search -attribute CN -baseobject -o"
    set exp_result "Conflicting option \"-o\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_18i {} {
    bind_to_dsa
    set test_case_name "obj_search_18i"
    set command "x500obj search -attribute CN -attribute tn -baseobject"
    set exp_result "Conflicting option \"-attribute\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_19i {} {
    bind_to_dsa
    set test_case_name "obj_search_19i"
    set command "x500obj search -attribute CN -onelevel -baseobject"
    set exp_result "Conflicting option \"-baseobject\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_20i {} {
    bind_to_dsa
    set test_case_name "obj_search_20i"
    set command "x500obj search -attribute CN -subtree -baseobject"
    set exp_result "Conflicting option \"-baseobject\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_21i {} {
    bind_to_dsa
    set test_case_name "obj_search_21i"
    set command "x500obj search -attribute CN -subtree -b"
    set exp_result "Conflicting option \"-b\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_22i {} {
    bind_to_dsa
    set test_case_name "obj_search_22i"
    set command "x500obj search -filter CN=* -filter CN=naik"
    set exp_result "Conflicting option \"-filter\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_23i {} {
    bind_to_dsa
    set test_case_name "obj_search_23i"
    set command "x500obj search -filter \"   \""
    set exp_result "Missing filter value."
    set exp_error_code "GDSCP -511"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_24i {} {
    bind_to_dsa
    set test_case_name "obj_search_24i"
    set command "search -filter"
    set exp_result "Missing filter value."
    set exp_error_code "GDSCP -511"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_25i {} {
    bind_to_dsa
    set test_case_name "obj_search_25i"
    set command "search -filter -attribute CN"
    set exp_result "Missing filter value."
    set exp_error_code "GDSCP -511"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_26i {} {
    bind_to_dsa
    set test_case_name "obj_search_26i"
    set command "search -types -attribute CN"
    set exp_result "Conflicting option \"-attribute\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_27i {} {
    bind_to_dsa
    set test_case_name "obj_search_27i"
    set command "search -types -allattr"
    set exp_result "Conflicting option \"-allattr\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_28i {} {
    bind_to_dsa
    set test_case_name "obj_search_28i"
    set command "search -allattr -attribute CN tn"
    set exp_result "Conflicting option \"-attribute\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_29i {} {
    bind_to_dsa
    set test_case_name "obj_search_29i"
    set command "search -allattr -types"
    set exp_result "Conflicting option \"-types\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_30i {} {
    bind_to_dsa
    set test_case_name "obj_search_30i"
    set command "search {{    c=de/   o=  /ou=ap11}} -subtree"
    set exp_result "Missing attribute value - \"/    c=de/   o=  /ou=ap11\" : Error position <18>."
    set exp_error_code "GDSCP -338"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_31i {} {
    bind_to_dsa
    set test_case_name "obj_search_31i"
    set command "search \"\{c=de/o=sni/ou=ap11\" -subtree"
    set exp_result "unmatched open brace in list"
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_32i {} {
    bind_to_dsa
    set test_case_name "obj_search_32i"
    set command "search {{c=de/o=sni/ou=ap11} {-attribute}} -subtree"
    set exp_result "Invalid argument \"{c=de/o=sni/ou=ap11} {-attribute}\"."
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_33i {} {
    bind_to_dsa
    set test_case_name "obj_search_33i"
    set command "search -subtree -attribute xyz"
    set exp_result "Abbreviation unknown - xyz."
    set exp_error_code "GDSCP -328"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_34i {} {
    bind_to_dsa
    set test_case_name "obj_search_34i"
    set command "search -subtree -filter {{c = de o = sni}}"
    set exp_result "Missing compound operator - \"c = de o = sni\" : Error position <8>."
    set exp_error_code "GDSCP -349"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_35i {} {
    bind_to_dsa
    set test_case_name "obj_search_35i"
    set command "search /c=de/o=sni/ou=ap11 -subtree"
    set exp_result "The specified name does not match the name of any object in the directory."
    set exp_error_code "GDSCP -34"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc obj_search_36i {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute OCL=C"
    set test_case_name "obj_search_36i"
    set command "search /c=de -filter {{cn=n* && dsc=x*}} -subtree"
    set exp_result "Entry not found."
    set exp_error_code "GDSCP -527"
    run_testcase $test_case_name $command $exp_result $exp_error_code
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_search_37i {} {
    bind_to_dsa
    log_message "Initializing ..."
    run_cmd_and_log "x500obj create /c=de -attribute OCL=C"
    set test_case_name "obj_search_37i"
    set command "search /c=de -filter tn=878 -subtree"
    set exp_result "Entry not found."
    set exp_error_code "GDSCP -527"
    run_testcase $test_case_name $command $exp_result $exp_error_code
    log_message "Cleaning up ..."
    run_cmd_and_log "x500obj delete /c=de"
}

proc obj_search_38i {} {
    bind_to_dsa
    create_dit_objects
    run_cmd_and_log "x500svc modify -sizelimit 1"
    set test_case_name "obj_search_38i"
    set command "search / -subtree"
    set exp_result "/C=de/O=sni/OU=ap11/CN=dsa/CN=dsa15 {Insufficient size limit.}"
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc modify -default"
    remove_dit_objects
}
