# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: abbr_show_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:35:45  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:59  marty]
#
# Revision 1.1.4.1  1995/12/11  15:26:39  root
# 	Submit
# 	[1995/12/11  14:36:27  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:22  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:25  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:21:58  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:38  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:34:54  marrek
# 	Updated.
# 	[1994/05/11  10:24:36  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:34:03  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:15:11  marrek]
# 
# $EndLog$
# 
# $RCSfile: abbr_show_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:45 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : abbr_show_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the show operation on the x500abbr object     #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc abbr_show_01v {} {
    bind_to_dsa
    set test_case_name "abbr_show_01v"
    set command "x500abbr show"
    run_testcase_and_extract_abbr $test_case_name $command 1 23 2
}

proc abbr_show_02v {} {
    bind_to_dsa
    set test_case_name "abbr_show_02v"
    set command "x500abbr s"
    run_testcase_and_extract_abbr $test_case_name $command 2 64 3
}

proc abbr_show_03v {} {
    bind_to_dsa
    set test_case_name "abbr_show_03v"
    set command "x500abbr sh"
    run_testcase_and_extract_abbr $test_case_name $command 3 18 3 2
}
