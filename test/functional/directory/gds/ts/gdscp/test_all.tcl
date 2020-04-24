# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: test_all.tcl,v $
# Revision 1.1.4.2  1996/03/11  01:34:19  marty
# 	Update copyright years
# 	[1996/03/10  19:36:24  marty]
#
# Revision 1.1.4.1  1995/12/11  15:28:10  root
# 	Submit
# 	[1995/12/11  14:37:02  root]
# 
# Revision 1.1.2.3  1994/07/06  15:11:56  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:34  marrek]
# 
# Revision 1.1.2.2  1994/06/10  20:23:52  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:28  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  14:35:45  marrek
# 	Updated from old test subdir.
# 	[1994/05/10  09:35:34  marrek]
# 
# 	Created in April 1994 submission.
# 	[1994/05/03  12:16:51  marrek]
# 
# $EndLog$
# 
# $RCSfile: test_all.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 01:34:19 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : test_all.tcl                                	  #
# Description : This file is used to run all the test cases	  #
#		for all the operations of all the gdscp objects.  #
#		(with the exception of test cases for referral    #
#		handling and test cases for structured output)    #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#


# source in the dsa configuration script
source test_config.tcl

# source in the test utilities script
source test_utils.tcl

# invoke the procedure to source all the test scripts
source_all_test_scripts  ./lib

# open the log file
open_log_file

# run the valid general test cases for the x500obj object
foreach test_case [lsort [info commands obj_general_*v]] {
    $test_case
    log_print_line
}

# run the invalid general test cases for the x500obj object
foreach test_case [lsort [info commands obj_general_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the bind operation on the x500obj object
foreach test_case [lsort [info commands obj_bind_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the create operation on the x500obj object
foreach test_case [lsort [info commands obj_create_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the manipulation of "gdscp_cwo" variable
foreach test_case [lsort [info commands obj_cwo_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the compare operation on the x500obj object
foreach test_case [lsort [info commands obj_compare_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the delete operation on the x500obj object
foreach test_case [lsort [info commands obj_delete_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the help operation on the x500obj object
foreach test_case [lsort [info commands obj_help_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the list operation on the x500obj object
foreach test_case [lsort [info commands obj_list_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the modify operation on the x500obj object
foreach test_case [lsort [info commands obj_modify_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the operations operation on the x500obj object
foreach test_case [lsort [info commands obj_operations_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the search operation on the x500obj object
foreach test_case [lsort [info commands obj_search_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the show operation on the x500obj object
foreach test_case [lsort [info commands obj_show_*i]] {
    $test_case
    log_print_line
}

# run the valid test cases for the bind operation on the x500obj object
foreach test_case [lsort [info commands obj_bind_*v]] {
    $test_case
    log_print_line
}

# run the valid test cases for the compare operation on the x500obj object
foreach test_case [lsort [info commands obj_compare_*v]] {
    $test_case
    log_print_line
}

# run the valid test cases for the create operation on the x500obj object
foreach test_case [lsort [info commands obj_create_*v]] {
    $test_case
    log_print_line
}

# run the valid test cases for the manipulation of "gdscp_cwo" variable
foreach test_case [lsort [info commands obj_cwo_*v]] {
    $test_case
    log_print_line
}

# run the valid test cases for the delete operation on the x500obj object
foreach test_case [lsort [info commands obj_delete_*v]] {
    $test_case
    log_print_line
}

# run the valid test cases for the list operation on the x500obj object
foreach test_case [lsort [info commands obj_list_*v]] {
    $test_case
    log_print_line
}

# run the valid test cases for the modify operation on the x500obj object
foreach test_case [lsort [info commands obj_modify_*v]] {
    $test_case
    log_print_line
}

# run the valid test cases for the operations operation on the x500obj object
foreach test_case [lsort [info commands obj_operations_*v]] {
    $test_case
    log_print_line
}

# run the valid test cases for the search operation on the x500obj object
foreach test_case [lsort [info commands obj_search_*v]] {
    $test_case
    log_print_line
}

# run the valid test cases for the show operation on the x500obj object
foreach test_case [lsort [info commands obj_show_*v]] {
    $test_case
    log_print_line
}

# run the invalid general test cases for the x500svc object
foreach test_case [lsort [info commands svc_general_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the help operation on the x500svc object
foreach test_case [lsort [info commands svc_help_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the modify operation on the x500svc object
foreach test_case [lsort [info commands svc_modify_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the operations operation on the x500svc object
foreach test_case [lsort [info commands svc_operations_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the show operation on the x500svc object
foreach test_case [lsort [info commands svc_show_*i]] {
    $test_case
    log_print_line
}

# run the valid test cases for the modify operation on the x500svc object
foreach test_case [lsort [info commands svc_modify_*v]] {
    $test_case
    log_print_line
}

# run the valid test cases for the operations operation on the x500svc object
foreach test_case [lsort [info commands svc_operations_*v]] {
    $test_case
    log_print_line
}

# run the valid test cases for the show operation on the x500svc object
foreach test_case [lsort [info commands svc_show_*v]] {
    $test_case
    log_print_line
}

# run the general invalid test cases for the x500abbr object
foreach test_case [lsort [info commands abbr_general_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the help operation on the x500abbr object
foreach test_case [lsort [info commands abbr_help_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the operations operation on the x500abbr object
foreach test_case [lsort [info commands abbr_operations_*i]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the show operation on the x500abbr object
foreach test_case [lsort [info commands abbr_show_*i]] {
    $test_case
    log_print_line
}

# run the valid test cases for the operations operation on the x500abbr object
foreach test_case [lsort [info commands abbr_operations_*v]] {
    $test_case
    log_print_line
}

# run the valid test cases for the show operation on the x500abbr object
foreach test_case [lsort [info commands abbr_show_*v]] {
    $test_case
    log_print_line
}

# run the invalid test cases for the miscellaneous commands of gdscp
foreach test_case [lsort [info commands misc_*i]] {
    $test_case
    log_print_line
}

# run the valid test cases for the miscellaneous commands of gdscp
foreach test_case [lsort [info commands misc_*v]] {
    $test_case
    log_print_line
}

# close the log file
close_log_file
