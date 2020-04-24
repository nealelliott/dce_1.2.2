# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: svc_show_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  01:34:18  marty
# 	Update copyright years
# 	[1996/03/10  19:36:22  marty]
#
# Revision 1.1.4.1  1995/12/11  15:28:08  root
# 	Submit
# 	[1995/12/11  14:37:01  root]
# 
# Revision 1.1.2.2  1994/06/10  20:23:50  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:27  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  14:35:42  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:49  marrek]
# 
# $EndLog$
# 
# $RCSfile: svc_show_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 01:34:18 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : svc_show_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the show operation on the x500svc object      #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc svc_show_01v {} {
    bind_to_dsa
    run_cmd_and_log "x500svc modify -default"
    set test_case_name "svc_show_01v"
    set command "x500svc show"
    set exp_result "INFINITE"
    run_testcase_and_extract $test_case_name $command $exp_result 14 2
}

proc svc_show_02v {} {
    bind_to_dsa
    run_cmd_and_log "x500svc modify -default"
    set test_case_name "svc_show_02v"
    set command "x500svc s"
    set exp_result "INFINITE"
    run_testcase_and_extract $test_case_name $command $exp_result 14 2
}
