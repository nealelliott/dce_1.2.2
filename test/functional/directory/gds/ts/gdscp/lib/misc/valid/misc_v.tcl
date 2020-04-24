# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: misc_v.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:35:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:31  marty]
#
# Revision 1.1.4.1  1995/12/11  15:25:58  root
# 	Submit
# 	[1995/12/11  14:36:12  root]
# 
# Revision 1.1.2.3  1994/07/06  15:54:04  marrek
# 	Add version string in July 1994 code drop.
# 	[1994/07/06  15:42:22  marrek]
# 
# Revision 1.1.2.2  1994/06/10  20:21:26  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:19  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  14:33:24  marrek
# 	Created in April 1994 submission.
# 	[1994/05/03  12:14:21  marrek]
# 
# $EndLog$
#
# $RCSfile: misc_v.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:14 $
#

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : misc_v.tcl                              	  #
# Description : This file contains all the valid test cases       #
#		for the other miscellaneous gdscp operations      #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

proc misc_01v {} {
    set test_case_name "misc_01v"
    set command "exit 12"

    puts stdout "Test case misc_01v                      ... passed"

    log_message "Test case name: " $test_case_name
    log_message "Test case: " $command
    log_message "Test case $test_case_name passed"
    log_print_line

    set status [catch {eval $command} result]

    # should not reach here since it is an exit test case. So must be an error
    puts stdout "Test case misc_01v                      ... failed"
    log_message "Received error message: " $result
    log_message "Test case $test_case_name failed"
}
