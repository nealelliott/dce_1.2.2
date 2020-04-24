# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: svc_modify_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  01:34:16  marty
# 	Update copyright years
# 	[1996/03/10  19:36:19  marty]
#
# Revision 1.1.4.1  1995/12/11  15:28:04  root
# 	Submit
# 	[1995/12/11  14:36:59  root]
# 
# Revision 1.1.2.2  1994/06/10  20:23:33  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:25  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  14:35:36  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:45  marrek]
# 
# $EndLog$
# 
# $RCSfile: svc_modify_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 01:34:16 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : svc_modify_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the modify operation on the x500svc object    #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc svc_modify_01v {} {
    bind_to_dsa
    set test_case_name "svc_modify_01v"
    set command "x500svc modify -default"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
}

proc svc_modify_02v {} {
    bind_to_dsa
    set test_case_name "svc_modify_02v"
    set command "x500svc modify -timelimit INFINITE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_03v {} {
    bind_to_dsa
    set test_case_name "svc_modify_03v"
    set command "x500svc modify -sizelimit INFINITE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_04v {} {
    bind_to_dsa
    set test_case_name "svc_modify_04v"
    set command "x500svc m -cacheclass NONE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_05v {} {
    bind_to_dsa
    set test_case_name "svc_modify_05v"
    set command "x500svc mod -scopeofreferral UNLIMITED"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_06v {} {
    bind_to_dsa
    set test_case_name "svc_modify_06v"
    set command "x500svc modify -timelimit 100"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_07v {} {
    bind_to_dsa
    run_cmd_and_log "x500svc modify -timelimit 100"
    set test_case_name "svc_modify_07v"
    set command "x500svc modify -timelimit INFINITE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_08v {} {
    bind_to_dsa
    set test_case_name "svc_modify_08v"
    set command "x500svc modify -sizelimit 100"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_09v {} {
    bind_to_dsa
    run_cmd_and_log "x500svc modify -sizelimit 100"
    set test_case_name "svc_modify_09v"
    set command "x500svc modify -sizelimit INFINITE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_10v {} {
    bind_to_dsa
    set test_case_name "svc_modify_10v"
    set command "x500svc modify -cacheclass PRIVILEGE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_11v {} {
    bind_to_dsa
    set test_case_name "svc_modify_11v"
    set command "x500svc modify -cacheclass NONE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_12v {} {
    bind_to_dsa
    run_cmd_and_log "x500svc modify -cacheclass PRIVILEGE"
    set test_case_name "svc_modify_12v"
    set command "x500svc m -ca NONE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_13v {} {
    bind_to_dsa
    set test_case_name "svc_modify_13v"
    set command "x500svc modify -scopeofreferral COUNTRY"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_14v {} {
    bind_to_dsa
    set test_case_name "svc_modify_14v"
    set command "x500svc modify -scopeofreferral {UNLIMITED}"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_15v {} {
    bind_to_dsa
    set test_case_name "svc_modify_15v"
    set command "x500svc modify -automaticcontinuation FALSE -cacheclass PRIVILEGE -chainingprohibited FALSE -dontdereferencealias TRUE -dontusecopy FALSE -duacache TRUE -duafirst TRUE -localscope TRUE -preferadmfunctions TRUE -preferchain TRUE -priority LOW -scopeofreferral DMD -timelimit 100 -sizelimit 500 -usedsa FALSE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_16v {} {
    bind_to_dsa
    set test_case_name "svc_modify_16v"
    set command "x500svc modify -a TRUE -cacheclass NORMAL -ch TRUE -dontderef FALSE -dontuse TRUE -duac FALSE -duaf FALSE -l FALSE -preferadm FALSE -preferch FALSE -prio MEDIUM -sc UNLIMITED -t INFINITE -siz INFINITE -u TRUE"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}

proc svc_modify_17v {} {
    bind_to_dsa
    set test_case_name "svc_modify_17v"
    set command "x500svc modify -def"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
}

proc svc_modify_18v {} {
    bind_to_dsa
    set test_case_name "svc_modify_18v"
    set command "x500svc modify -cacheclass RESIDENT -priority HIGH"
    set exp_result ""
    set exp_error_code ""
    run_testcase $test_case_name $command $exp_result $exp_error_code
    run_cmd_and_log "x500svc show"
    run_cmd_and_log "x500svc modify -default"
}
