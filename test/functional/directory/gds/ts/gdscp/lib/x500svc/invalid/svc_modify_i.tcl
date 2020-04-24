# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: svc_modify_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:36:46  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:41:51  marty]
#
# Revision 1.1.4.1  1995/12/11  15:27:55  root
# 	Submit
# 	[1995/12/11  14:36:55  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:52  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:28  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:23:29  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:16  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:17  marrek
# 	Updated.
# 	[1994/05/11  10:25:41  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:35:24  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:36  marrek]
# 
# $EndLog$
# 
# $RCSfile: svc_modify_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:36:46 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : svc_modify_i.tcl                              	  #
# Description : This file contains all the invalid test cases     #
#		for the modify operation on the x500svc object    #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc svc_modify_01i {} {
    bind_to_dsa
    set test_case_name "svc_modify_01i"
    set command "x500svc modify"
    set exp_result "Missing arguments."
    set exp_error_code "GDSCP -515"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_02i {} {
    bind_to_dsa
    set test_case_name "svc_modify_02i"
    set command "x500svc modify -attribute"
    set exp_result "Unknown option \"-attribute\"."
    set exp_error_code "GDSCP -502"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_03i {} {
    bind_to_dsa
    set test_case_name "svc_modify_03i"
    set command "x500svc modify some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_04i {} {
    bind_to_dsa
    set test_case_name "svc_modify_04i"
    set command "x500svc modify -c"
    set exp_result "Ambiguous option \"-c\": -cacheclass -chainingprohibited."
    set exp_error_code "GDSCP -532"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_05i {} {
    bind_to_dsa
    set test_case_name "svc_modify_05i"
    set command "x500svc modify -d"
    set exp_result "Ambiguous option \"-d\": -default -dontdereferencealias -dontusecopy -duacache -duafirst."
    set exp_error_code "GDSCP -532"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_06i {} {
    bind_to_dsa
    set test_case_name "svc_modify_06i"
    set command "x500svc modify -do"
    set exp_result "Ambiguous option \"-do\": -dontdereferencealias -dontusecopy."
    set exp_error_code "GDSCP -532"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_07i {} {
    bind_to_dsa
    set test_case_name "svc_modify_07i"
    set command "x500svc modify -p"
    set exp_result "Ambiguous option \"-p\": -preferadmfunctions -preferchain -priority."
    set exp_error_code "GDSCP -532"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_08i {} {
    bind_to_dsa
    set test_case_name "svc_modify_08i"
    set command "x500svc modify -s"
    set exp_result "Ambiguous option \"-s\": -scopeofreferral -sizelimit."
    set exp_error_code "GDSCP -532"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_09i {} {
    bind_to_dsa
    set test_case_name "svc_modify_09i"
    set command "x500svc modify -automaticcontinuation"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_10i {} {
    bind_to_dsa
    set test_case_name "svc_modify_10i"
    set command "x500svc modify -automaticcontinuation -localscope"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_11i {} {
    bind_to_dsa
    set test_case_name "svc_modify_11i"
    set command "x500svc modify -automaticcontinuation some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_12i {} {
    bind_to_dsa
    set test_case_name "svc_modify_12i"
    set command "x500svc modify -automaticcontinuation TRUE -duacache TRUE -automaticcontinuation"
    set exp_result "Conflicting option \"-automaticcontinuation\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_13i {} {
    bind_to_dsa
    set test_case_name "svc_modify_13i"
    set command "x500svc modify -cacheclass"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_14i {} {
    bind_to_dsa
    set test_case_name "svc_modify_14i"
    set command "x500svc modify -cacheclass -localscope"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_15i {} {
    bind_to_dsa
    set test_case_name "svc_modify_15i"
    set command "x500svc modify -cacheclass some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_16i {} {
    bind_to_dsa
    set test_case_name "svc_modify_16i"
    set command "x500svc modify -cacheclass NORMAL -duacache TRUE -cacheclass"
    set exp_result "Conflicting option \"-cacheclass\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_17i {} {
    bind_to_dsa
    set test_case_name "svc_modify_17i"
    set command "x500svc modify -chainingprohibited"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_18i {} {
    bind_to_dsa
    set test_case_name "svc_modify_18i"
    set command "x500svc modify -chainingprohibited -localscope"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_19i {} {
    bind_to_dsa
    set test_case_name "svc_modify_19i"
    set command "x500svc modify -chainingprohibited some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_20i {} {
    bind_to_dsa
    set test_case_name "svc_modify_20i"
    set command "x500svc modify -chainingprohibited TRUE -duacache TRUE -chainingprohibited"
    set exp_result "Conflicting option \"-chainingprohibited\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_21i {} {
    bind_to_dsa
    set test_case_name "svc_modify_21i"
    set command "x500svc modify -default -duacache"
    set exp_result "Too many arguments."
    set exp_error_code "GDSCP -526"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_22i {} {
    bind_to_dsa
    set test_case_name "svc_modify_22i"
    set command "x500svc modify -chainingprohibited TRUE -default"
    set exp_result "Too many arguments."
    set exp_error_code "GDSCP -526"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_23i {} {
    bind_to_dsa
    set test_case_name "svc_modify_23i"
    set command "x500svc modify -dontdereferencealias"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_24i {} {
    bind_to_dsa
    set test_case_name "svc_modify_24i"
    set command "x500svc modify -dontdereferencealias -localscope"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_25i {} {
    bind_to_dsa
    set test_case_name "svc_modify_25i"
    set command "x500svc modify -dontdereferencealias some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_26i {} {
    bind_to_dsa
    set test_case_name "svc_modify_26i"
    set command "x500svc modify -dontdereferencealias TRUE -duacache TRUE -dontdereferencealias"
    set exp_result "Conflicting option \"-dontdereferencealias\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_27i {} {
    bind_to_dsa
    set test_case_name "svc_modify_27i"
    set command "x500svc modify -dontusecopy"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_28i {} {
    bind_to_dsa
    set test_case_name "svc_modify_28i"
    set command "x500svc modify -dontusecopy -localscope"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_29i {} {
    bind_to_dsa
    set test_case_name "svc_modify_29i"
    set command "x500svc modify -dontusecopy some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_30i {} {
    bind_to_dsa
    set test_case_name "svc_modify_30i"
    set command "x500svc modify -dontusecopy TRUE -duacache TRUE -dontusecopy"
    set exp_result "Conflicting option \"-dontusecopy\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_31i {} {
    bind_to_dsa
    set test_case_name "svc_modify_31i"
    set command "x500svc modify -dontusecopy \"\{TRUE\""
    set exp_result "unmatched open brace in list"
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_32i {} {
    bind_to_dsa
    set test_case_name "svc_modify_32i"
    set command "x500svc modify -dontusecopy {{TRUE} -duacache}"
    set exp_result "Invalid argument \"{TRUE} -duacache\"."
    set exp_error_code "GDSCP -703"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_33i {} {
    bind_to_dsa
    set test_case_name "svc_modify_33i"
    set command "x500svc modify -duacache"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_34i {} {
    bind_to_dsa
    set test_case_name "svc_modify_34i"
    set command "x500svc modify -duacache -localscope"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_35i {} {
    bind_to_dsa
    set test_case_name "svc_modify_35i"
    set command "x500svc modify -duacache some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_36i {} {
    bind_to_dsa
    set test_case_name "svc_modify_36i"
    set command "x500svc modify -duacache TRUE -duacache TRUE -duacache"
    set exp_result "Conflicting option \"-duacache\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_37i {} {
    bind_to_dsa
    set test_case_name "svc_modify_37i"
    set command "x500svc modify -duafirst"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_38i {} {
    bind_to_dsa
    set test_case_name "svc_modify_38i"
    set command "x500svc modify -duafirst -localscope"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_39i {} {
    bind_to_dsa
    set test_case_name "svc_modify_39i"
    set command "x500svc modify -duafirst some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_40i {} {
    bind_to_dsa
    set test_case_name "svc_modify_40i"
    set command "x500svc modify -duafirst TRUE -duacache TRUE -duafirst"
    set exp_result "Conflicting option \"-duafirst\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_41i {} {
    bind_to_dsa
    set test_case_name "svc_modify_41i"
    set command "x500svc modify -localscope"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_42i {} {
    bind_to_dsa
    set test_case_name "svc_modify_42i"
    set command "x500svc modify -localscope -cacheclass"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_43i {} {
    bind_to_dsa
    set test_case_name "svc_modify_43i"
    set command "x500svc modify -localscope some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_44i {} {
    bind_to_dsa
    set test_case_name "svc_modify_44i"
    set command "x500svc modify -localscope TRUE -duacache TRUE -localscope"
    set exp_result "Conflicting option \"-localscope\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_45i {} {
    bind_to_dsa
    set test_case_name "svc_modify_45i"
    set command "x500svc modify -preferadmfunctions"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_46i {} {
    bind_to_dsa
    set test_case_name "svc_modify_46i"
    set command "x500svc modify -preferadmfunctions -cacheclass"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_47i {} {
    bind_to_dsa
    set test_case_name "svc_modify_47i"
    set command "x500svc modify -preferadmfunctions some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_48i {} {
    bind_to_dsa
    set test_case_name "svc_modify_48i"
    set command "x500svc modify -preferadmfunctions TRUE -duacache TRUE -preferadmfunctions"
    set exp_result "Conflicting option \"-preferadmfunctions\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_49i {} {
    bind_to_dsa
    set test_case_name "svc_modify_49i"
    set command "x500svc modify -preferchain"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_50i {} {
    bind_to_dsa
    set test_case_name "svc_modify_50i"
    set command "x500svc modify -preferchain -cacheclass"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_51i {} {
    bind_to_dsa
    set test_case_name "svc_modify_51i"
    set command "x500svc modify -preferchain some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_52i {} {
    bind_to_dsa
    set test_case_name "svc_modify_52i"
    set command "x500svc modify -preferchain TRUE -duacache TRUE -preferchain"
    set exp_result "Conflicting option \"-preferchain\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_53i {} {
    bind_to_dsa
    set test_case_name "svc_modify_53i"
    set command "x500svc modify -priority"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_54i {} {
    bind_to_dsa
    set test_case_name "svc_modify_54i"
    set command "x500svc modify -priority -cacheclass"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_55i {} {
    bind_to_dsa
    set test_case_name "svc_modify_55i"
    set command "x500svc modify -priority some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_56i {} {
    bind_to_dsa
    set test_case_name "svc_modify_56i"
    set command "x500svc modify -priority LOW -duacache TRUE -priority"
    set exp_result "Conflicting option \"-priority\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_57i {} {
    bind_to_dsa
    set test_case_name "svc_modify_57i"
    set command "x500svc modify -scopeofreferral"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_58i {} {
    bind_to_dsa
    set test_case_name "svc_modify_58i"
    set command "x500svc modify -scopeofreferral -cacheclass"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_59i {} {
    bind_to_dsa
    set test_case_name "svc_modify_59i"
    set command "x500svc modify -scopeofreferral some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_60i {} {
    bind_to_dsa
    set test_case_name "svc_modify_60i"
    set command "x500svc modify -scopeofreferral DMD -duacache TRUE -scopeofreferral"
    set exp_result "Conflicting option \"-scopeofreferral\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_61i {} {
    bind_to_dsa
    set test_case_name "svc_modify_61i"
    set command "x500svc modify -sizelimit"
    set exp_result "Missing size limit."
    set exp_error_code "GDSCP -514"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_62i {} {
    bind_to_dsa
    set test_case_name "svc_modify_62i"
    set command "x500svc modify -sizelimit -cacheclass"
    set exp_result "Missing size limit."
    set exp_error_code "GDSCP -514"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_63i {} {
    bind_to_dsa
    set test_case_name "svc_modify_63i"
    set command "x500svc modify -sizelimit some_value"
    set exp_result "Invalid size limit \"some_value\"."
    set exp_error_code "GDSCP -518"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_64i {} {
    bind_to_dsa
    set test_case_name "svc_modify_64i"
    set command "x500svc modify -sizelimit 0"
    set exp_result "Invalid size limit \"0\"."
    set exp_error_code "GDSCP -518"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_65i {} {
    bind_to_dsa
    set test_case_name "svc_modify_65i"
    set command "x500svc modify -sizelimit 10 -duacache TRUE -sizelimit"
    set exp_result "Conflicting option \"-sizelimit\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_66i {} {
    bind_to_dsa
    set test_case_name "svc_modify_66i"
    set command "x500svc modify -timelimit"
    set exp_result "Missing time limit."
    set exp_error_code "GDSCP -513"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_67i {} {
    bind_to_dsa
    set test_case_name "svc_modify_67i"
    set command "x500svc modify -timelimit -cacheclass"
    set exp_result "Missing time limit."
    set exp_error_code "GDSCP -513"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_68i {} {
    bind_to_dsa
    set test_case_name "svc_modify_68i"
    set command "x500svc modify -timelimit some_value"
    set exp_result "Invalid time limit \"some_value\"."
    set exp_error_code "GDSCP -517"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_69i {} {
    bind_to_dsa
    set test_case_name "svc_modify_69i"
    set command "x500svc modify -timelimit 0"
    set exp_result "Invalid time limit \"0\"."
    set exp_error_code "GDSCP -517"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_70i {} {
    bind_to_dsa
    set test_case_name "svc_modify_70i"
    set command "x500svc modify -timelimit 10 -duacache TRUE -timelimit"
    set exp_result "Conflicting option \"-timelimit\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_71i {} {
    bind_to_dsa
    set test_case_name "svc_modify_71i"
    set command "x500svc modify -usedsa"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_72i {} {
    bind_to_dsa
    set test_case_name "svc_modify_72i"
    set command "x500svc modify -usedsa -cacheclass"
    set exp_result "Missing value."
    set exp_error_code "GDSCP -522"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_73i {} {
    bind_to_dsa
    set test_case_name "svc_modify_73i"
    set command "x500svc modify -usedsa some_arg"
    set exp_result "Unknown argument \"some_arg\"."
    set exp_error_code "GDSCP -519"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}

proc svc_modify_74i {} {
    bind_to_dsa
    set test_case_name "svc_modify_74i"
    set command "x500svc modify -usedsa TRUE -duacache TRUE -usedsa"
    set exp_result "Conflicting option \"-usedsa\"."
    set exp_error_code "GDSCP -501"
    run_testcase $test_case_name $command $exp_result $exp_error_code
}
