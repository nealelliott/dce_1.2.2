# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: test_utils.tcl,v $
# Revision 1.1.4.2  1996/03/11  01:34:22  marty
# 	Update copyright years
# 	[1996/03/10  19:36:29  marty]
#
# Revision 1.1.4.1  1995/12/11  15:28:16  root
# 	Submit
# 	[1995/12/11  14:37:05  root]
# 
# Revision 1.1.2.4  1994/07/06  15:11:59  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:24:45  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:23:55  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:00:31  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:35:21  marrek
# 	Updated.
# 	[1994/05/11  10:25:53  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:35:55  marrek
# 	Updated from old test subdir.
# 	[1994/05/10  09:35:41  marrek]
# 
# 	Created in April 1994 submission.
# 	[1994/05/03  12:17:01  marrek]
# 
# $EndLog$
# 
# $RCSfile: test_utils.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 01:34:22 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : test_utils.tcl                               	  #
# Description : This file contains all the utility TCL procedures #
#		used by the gdscp test program "gdscp_test"       #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : source_all_test_scripts                       #
# Description     : This procedure is used to source all the test #
#		    scripts containing the test cases. However,   #
#		    the scripts related to testing for referral   #
#		    handling and the structured output are not    #
#		    sourced.				          #
# Input Arguments : basedir - The installation directory relative #
#		    to which all the script files are stored	  #
# Return value    : Same as that of TCL "source" command	  # 
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc source_all_test_scripts {basedir} {

    # source all scripts for the invalid test cases of miscellaneous commands 
    source $basedir/misc/invalid/misc_i.tcl

    # source all scripts for the valid test cases of miscellaneous commands 
    source $basedir/misc/valid/misc_v.tcl

    # source all scripts for invalid test cases of x500abbr operations
    source $basedir/x500abbr/invalid/abbr_general_i.tcl
    source $basedir/x500abbr/invalid/abbr_help_i.tcl
    source $basedir/x500abbr/invalid/abbr_operations_i.tcl
    source $basedir/x500abbr/invalid/abbr_show_i.tcl

    # source all scripts for valid test cases of x500abbr operations
    source $basedir/x500abbr/valid/abbr_operations_v.tcl
    source $basedir/x500abbr/valid/abbr_show_v.tcl

    # source all scripts for invalid test cases of x500obj operations
    source $basedir/x500obj/invalid/obj_bind_i.tcl
    source $basedir/x500obj/invalid/obj_compare_i.tcl
    source $basedir/x500obj/invalid/obj_create_i.tcl
    source $basedir/x500obj/invalid/obj_cwo_i.tcl
    source $basedir/x500obj/invalid/obj_delete_i.tcl
    source $basedir/x500obj/invalid/obj_general_i.tcl
    source $basedir/x500obj/invalid/obj_help_i.tcl
    source $basedir/x500obj/invalid/obj_list_i.tcl
    source $basedir/x500obj/invalid/obj_modify_i.tcl
    source $basedir/x500obj/invalid/obj_operations_i.tcl
    source $basedir/x500obj/invalid/obj_search_i.tcl
    source $basedir/x500obj/invalid/obj_show_i.tcl

    # source all scripts for valid test cases of x500obj operations
    source $basedir/x500obj/valid/obj_bind_v.tcl
    source $basedir/x500obj/valid/obj_compare_v.tcl
    source $basedir/x500obj/valid/obj_create_v.tcl
    source $basedir/x500obj/valid/obj_cwo_v.tcl
    source $basedir/x500obj/valid/obj_delete_v.tcl
    source $basedir/x500obj/valid/obj_general_v.tcl
    source $basedir/x500obj/valid/obj_list_v.tcl
    source $basedir/x500obj/valid/obj_modify_v.tcl
    source $basedir/x500obj/valid/obj_operations_v.tcl
    source $basedir/x500obj/valid/obj_search_v.tcl
    source $basedir/x500obj/valid/obj_show_v.tcl

    # source all scripts for invalid test cases of x500svc operations
    source $basedir/x500svc/invalid/svc_general_i.tcl
    source $basedir/x500svc/invalid/svc_help_i.tcl
    source $basedir/x500svc/invalid/svc_modify_i.tcl
    source $basedir/x500svc/invalid/svc_operations_i.tcl
    source $basedir/x500svc/invalid/svc_show_i.tcl

    # source all scripts for valid test cases of x500svc operations
    source $basedir/x500svc/valid/svc_modify_v.tcl
    source $basedir/x500svc/valid/svc_operations_v.tcl
    source $basedir/x500svc/valid/svc_show_v.tcl
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : open_log_file                       	  #
# Description     : This procedure is used to open the log file   #
#		    where all the test logging is done.           #
#		    The directory where the log file is to be     #
#		    opened is obtained from an environment	  #
#		    variable GCT_LOG.				  #
# Input Arguments : NONE					  #
# Return value    : The file handle or NULL or a message string   #
#		    in case of errors. If the environment variable#
#		    is undefined, NULL is returned.		  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc open_log_file {} {

    global logfile_id env;

    set logfile_id ""

    # check if the environment variable is defined
    if { [catch {set env(GCT_LOG_DIR)} dir_name] == 0 } {

	# open the log file and catch the return status
	set status [catch {open $dir_name/gdscp_test.log w} result];

	if {$status == 0} {
	    # set the global variable
	    set logfile_id $result
	} else {
	    error "$result"
	}
    }
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : close_log_file                       	  #
# Description     : This procedure is used to close the log file  #
# Input Arguments : NONE					  #
# Return value    : Same as that of the TCL close command	  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc close_log_file {} {

    global logfile_id;

    # close the log file if it has been opened
    if { $logfile_id != "" } {
	close $logfile_id
    }
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : log_message                       	          #
# Description     : This procedure is used to write messages into #
#		    the log file  				  #
# Input Arguments : The messages to be printed		  	  #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc log_message {args} {

    global logfile_id;

    # write the message if the log file has been opened
    if { $logfile_id != "" } {
	foreach message $args {
	    puts -nonewline $logfile_id $message
	}
	puts $logfile_id ""
	flush $logfile_id
    }
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : run_test_cases_in_file                     	  #
# Description     : This procedure is used to run all the test    #
#		    cases which are in the specified file.        #
# Input Arguments : test_file - file containing test procedures   #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc run_test_cases_in_file {test_file} {

    global directory_id dsa_name psap_address errorCode

    # source in the dsa configuration file
    source ../../../test_config.tcl

    # open the log file
    open_log_file
    
    # fetch the list of commands
    set old_command_list [info commands]

    # source in the test file
    source $test_file

    # now fetch the new list of commands
    set new_command_list [lsort [info commands]]

    # for every command in the new list, scan the old list. If the
    # command is not in the old list, run it.
    foreach test_case $new_command_list {
	if { [lsearch -exact $old_command_list $test_case] == -1} {
	    $test_case
	    log_print_line
	}
    }

    # close the log file
    close_log_file
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : log_print_line                     	  	  #
# Description     : This procedure is used to print the line      #
#		    in the log file which separates the test cases#
# Input Arguments : NONE					  #
# Return value    : Same as the return value for TCL puts command #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc log_print_line {} {
    log_message "-----------------------------------------------------------------------------"
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : run_cmd_and_log                  	  	  #
# Description     : This procedure is used to run the specified   #
#		    command and log the command into the log file.#
#		    Errors, if any, are also logged into the file #
# Input Arguments : command - The command to be run		  #
# Return value    : Same as the return value for TCL puts command #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc run_cmd_and_log {command} {

    global gdscp_cwo gdscp_t61;

    # log the command in the log file
    log_message $command

    # evaluate the command and trap the error return, if any
    set status [catch {eval $command} result]

    # if an error has occurred, log the error message to the log file
    if {$status == 1} {
	log_message "ERROR: " $result
    } else {
	if {$result != ""} {
	    log_message $result
	}
    }
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : bind_to-dsa                 	  	  #
# Description     : This procedure is used to performa a bind     #
#		    operation to the specified DSA.		  #
#		    The directory details like the identifier     #
#		    and the dsa name are obtained from the        #
#		    test_config.tcl file.			  #
# Input Arguments : NONE					  #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc bind_to_dsa {} {

    global directory_id dsa_name;

    # run the bind command and log messages in the log file
    run_cmd_and_log "x500obj bind -dirid $directory_id -dsa $dsa_name"
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : create_dit_objects                 	  	  #
# Description     : This procedure creates the objects in the     #
#		    DIT.					  #
# Input Arguments : NONE					  #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc create_dit_objects {} {

    # print message in log file
    log_message "Initializing ..."

    # create the country object
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"

    # create the organization object
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"

    # create the organizational-unit object
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"

    # create the common-name objects
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=naik -attribute \
		{ {ocl=orp} {sn=naik} {tn=12345;369072;576268} {up=naik} }"

    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=naik,ou=ap11	\
		 -attribute { {ocl=orp} {sn=naik} {tn=99999} }"

    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=muller -attribute \
		{ {ocl=orp} {sn=John} {dsc=Software Professional} }"

    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=miller -attribute \
		{ {ocl=orp} {sn=Peter} {dsc=Engineer} }"

    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=mueller -attribute \
		{ {ocl=orp} {sn=Henry} {dsc=Engineer} }"

    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=meller -attribute \
		{ {ocl=orp} {sn=John} }"

    # create the dsa name object
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=dsa -attribute \
		{ {ocl=app} }"

    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=dsa/cn=dsa15   \
		-attribute { {ocl=dsa} {PSA={NA='TCP/IP!internet=127.0.0.1+port=25015',TS=Server}} }"
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : create_dit_mail_objects           	  	  #
# Description     : This procedure creates the mail objects in    #
#		    the DIT.					  #
# Input Arguments : NONE					  #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc create_dit_mail_objects {} {

    # create the mail objects
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=mail1 -attribute \
	{ {ocl=mdl} {mds={pt=0, ind={dn={/c=de/o=sni}, admd=admd, cn='common name', c=de,dt1=dt1,dt2=dt2,dt3=dt3,dt4=dt4,dv1=dv1,dv2=dv2,dv3=dv3,dv4=dv4,gen=gen,gn=given,ini=init,o=orgname,ou1=ou1,ou2=ou2,ou3=ou3,ou4=ou4,prmd=prmd,sn=surname}}} {moa={admd=admd,c=de,prmd=prmd}} }" 


    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=mail2 -attribute \
	{ {ocl=mdl} {mds={pt=0, ind={dn={/c=de/o=sni/ou=ap11/cn=ind1}, admd=admd, c=de,o=orgname,prmd=prmd}};{pt=0,ind={dn={/c=de/o=sni/ou=ap11/cn=Submit},admd=admd,c=de,o=orgname,prmd=prmd}}} {moa={admd=admd,c=de,o=orgname,ou1=ou1,ou2=ou2,prmd=prmd}} {mdm={dn={/c=de/o=sni/ou=ap11},admd=admd,c=de,prmd=prmd}} }"

    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=Brown -attribute \
	{ {ocl=mdl} {dsc=Software Consultant} {mds={pt=0, ind={dn={/c=de/o=sni}, c=de, admd=dbp, prmd=sni, o=Siemens Nixdorf, ou1=Munich, ou2=P1, ou3=P4, ou4=ap113, cn=Alfred Brown, gn=Alfred, ini=P., sn=Brown, gen=396, dt1=MS MAIL, dv1=Brown}}} {moa={admd=admd,c=de,prmd=prmd}} }" 

    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=Andrews -attribute \
	{ {ocl=mdl} {dsc=Project Leader} {mds={pt=0, ind={dn={/c=de/o=sni}, c=de, admd=dbp, prmd=sni, o=Siemens Nixdorf, ou1=Munich, ou2=P1, ou3=P4, ou4=ap113, cn=Peter Andrews, gn=Peter, ini=P., sn=Andrews, gen=396, dt1=MS MAIL, dv1=Peter}}} {moa={admd=admd,c=de,prmd=prmd}} }" 
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : remove_dit_objects                 	  	  #
# Description     : This procedure removes the objects from the   #
#		    DIT.					  #
# Input Arguments : NONE					  #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc remove_dit_objects {} {

    # print message in log file
    log_message "Cleaning up ..."

    # bind to the dsa
    bind_to_dsa

    # remove the dsa object
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=dsa/cn=dsa15"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=dsa"

    # remove the common name objects
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=meller"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=mueller"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=miller"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=muller"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=naik,ou=ap11"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=naik"

    # remove the organizational-unit object
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"

    # remove the organization object
    run_cmd_and_log "x500obj delete /c=de/o=sni"

    # remove the country object
    run_cmd_and_log "x500obj delete /c=de"
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : remove_dit_mail_objects           	  	  #
# Description     : This procedure removes the mail objects from  #
#		    the DIT.					  #
# Input Arguments : NONE					  #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc remove_dit_mail_objects {} {

    # bind to the dsa
    bind_to_dsa

    # print message in log file
    log_message "Cleaning up ..."

    # remove the mail objects
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=mail1"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=mail2"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=Brown"
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=Andrews"
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : run_testcase                  	  	  #
# Description     : This procedure is used to run the specified   #
#		    test case.  The details of the test case and  #
#		    the return values, if any, are logged in the  #
#		    log file.					  #
# Input Arguments : test_case_name - The name of the test case    #
#		    command        - The command to be executed   #
#		    exp_result     - The expected result	  #
#		    exp_error_code - Expected error code in case  #
#				     of errors.			  #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc run_testcase {test_case_name command exp_result exp_error_code} {

    global errorCode gdscp_cwo gdscp_library gdscp_t61;

    # reset the errorCode variable
    set errorCode "NONE"

    # log the test case name in the log file
    log_message "Test case name: " $test_case_name

    # log the command in the log file
    log_message "Test case: " $command

    set message_out "Test case $test_case_name"

    # display test case name on stdout
    puts -nonewline stdout $message_out

    # display filler spaces
    for {set i [string length $message_out]} {$i < 40} {set i [expr $i + 1]} {
	puts -nonewline stdout " "
    }

    puts -nonewline stdout "... "

    # evaluate the command and trap the error return, if any
    set status [catch {eval $command} result]

    # log the received result to the log file, if present
    if {$result != ""} {
	log_message "Received string: " $result
    }

    # log the received error code to the log file, if present
    if {$status != 0 && $errorCode != "NONE"} {
	log_message "Received error code: " $errorCode
    }

    # if the test case was not successful
    if {$result != $exp_result || 			\
	($exp_error_code != "" && $exp_error_code != $errorCode)} {
	puts stdout "failed"
	log_message "Test case $test_case_name failed"

	if {$result != $exp_result} {
	    log_message "Expected string: " $exp_result
	}

	if {$exp_error_code != "" && $exp_error_code != $errorCode} {
	    log_message "Expected error code: " $exp_error_code
	}
    } else {
	puts stdout "passed"
	log_message "Test case $test_case_name passed"
    }
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : run_testcase_and_extract          	  	  #
# Description     : This procedure is used to run the specified   #
#		    test case and extract the specified items from#
#		    the returned TCL list.  The variable argument #
#		    list to this procedure specify the items to   #
#		    be extracted.				  #
#		    For eg: If the variable argument list is      #
#		    specified as "3", the command is executed, and#
#		    the 3rd item (index 2) is extracted from the  #
#		    returned list.				  #
#		    If the variable argument list is specified as #
#		    "3 1", the command is executed, and the	  #
#		    3rd sub-list (index 2) is extracted from the  #
#		    returned list.  Also from the 3rd sub-list,   #
#		    the 1st item (index 0) is extracted.	  #
#		    The details of the test case, the extracted   #
#		    items and the return values, if any, are      #
#		    logged in the log file.			  #
# Input Arguments : test_case_name - The name of the test case    #
#		    command        - The command to be executed   #
#		    exp_result     - The expected result. This	  #
#				     is the value of the last     #
#				     extracted item.		  #
#		    args	   - The variable argument list   #
#				     specifying the items to be   #
#				     extracted.			  #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc run_testcase_and_extract {test_case_name command exp_result args} {

    # log the test case name in the log file
    log_message "Test case name: " $test_case_name

    # log the command in the log file
    log_message "Test case: " $command

    # log the item list to be extracted in the log file
    log_message "Item to be extracted: " $args

    set message_out "Test case $test_case_name"

    # display test case name on stdout
    puts -nonewline stdout $message_out

    # display filler spaces
    for {set i [string length $message_out]} {$i < 40} {set i [expr $i + 1]} {
	puts -nonewline stdout " "
    }

    puts -nonewline stdout "... "

    # evaluate the command and trap the error return, if any
    set status [catch {eval $command} result]

    # if an error has occured
    if {$status != 0} {
	puts stdout "failed"
	log_message "Test case $test_case_name failed"
	log_message "Received error: " $result
	log_message "Expected string: " $exp_result
    } else {
	# log the result in the log file
	log_message "Received string: " $result

	set new_list $result

	# now extract the items from the returned list
	foreach i $args {
	    append item_path $i

	    # convert the item number to absolute index
	    set item_no [expr $i - 1]

	    # extract the required item
	    set sub_list [lindex $new_list $item_no]

	    # log the extracted item in the log file
	    log_message "Extracted item $item_path: " $sub_list

	    set new_list $sub_list
	    append item_path " "
	}

	if { $new_list != $exp_result} {
	    puts stdout "failed"
	    log_message "Test case $test_case_name failed"
	    log_message "Expected extracted item $args : " $exp_result
        } else {
	    puts stdout "passed"
	    log_message "Test case $test_case_name passed"
	}
    }
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : run_testcase_and_extract_abbr      	  	  #
# Description     : This procedure is used to run the specified   #
#		    test case for the x500abbr object and extract #
#		    the specified items from the returned TCL list#
#		    The variable argument list to this procedure  #
#		    specify the items to be extracted.		  #
#		    For eg: If the variable argument list is      #
#		    specified as "3", the command is executed, and#
#		    the 3rd item (index 2) is extracted from the  #
#		    returned list.				  #
#		    If the variable argument list is specified as #
#		    "3 1", the command is executed, and the	  #
#		    3rd sub-list (index 2) is extracted from the  #
#		    returned list.  Also from the 3rd sub-list,   #
#		    the 1st item (index 0) is extracted.	  #
#		    The details of the test case, the extracted   #
#		    items and the return values, if any, are      #
#		    logged in the log file.			  #
# Input Arguments : test_case_name - The name of the test case    #
#		    command        - The command to be executed   #
#		    args	   - The variable argument list   #
#				     specifying the items to be   #
#				     extracted.			  #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc run_testcase_and_extract_abbr {test_case_name command args} {

    # log the test case name in the log file
    log_message "Test case name: " $test_case_name

    # log the command in the log file
    log_message "Test case: " $command

    # log the item list to be extracted in the log file
    log_message "Item to be extracted: " $args

    set message_out "Test case $test_case_name"

    # display test case name on stdout
    puts -nonewline stdout $message_out

    # display filler spaces
    for {set i [string length $message_out]} {$i < 40} {set i [expr $i + 1]} {
	puts -nonewline stdout " "
    }

    puts -nonewline stdout "... "

    # evaluate the command and trap the error return, if any
    set status [catch {eval $command} result]

    # if an error has occured
    if {$status != 0} {
	puts stdout "failed"
	log_message "Test case $test_case_name failed"
	log_message "Received error: " $result
    } else {
	set new_list $result
	set path_count 0

	# now extract the items from the returned list
	foreach i $args {
	    set path_count [expr $path_count + 1]

	    append item_path $i

	    # convert the item number to absolute index
	    set item_no [expr $i - 1]

	    # extract the required item
	    set sub_list [lindex $new_list $item_no]

	    # log the extracted item in the log file only if it is not
	    # the first extracted item in the list. This is because
	    # the first extracted item is a very long string.
	    if {$path_count != 1} {
		log_message "Extracted item $item_path: " $sub_list
	    }

	    set new_list $sub_list
	    append item_path " "
	}

	if { $new_list == ""} {
	    puts stdout "failed"
	    log_message "Test case $test_case_name failed"
	    log_message "Could not extract item"
        } else {
	    puts stdout "passed"
	    log_message "Test case $test_case_name passed"
	}
    }
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : run_testcase_and_display      	  	  #
# Description     : This procedure is used to run the specified   #
#		    test case and display the details of the      #
#		    test case and the received result, if any, on #
#		    stdout.  This procedure is mainly used for    #
#		    the test cases with the "-pretty" switch,     #
#		    test cases for the "help" operation, and      #
#		    test cases for referral handling.             #
# Input Arguments : command        - The command to be executed   #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc run_test_case_and_display {command} {

    global gdscp_cwo directory_id dsa_name gdscp_t61

    # Display the command on stdout
    puts stdout $command

    # Evaluate the command and trap the error return, if any
    set status [catch {eval $command} result]

    # If an error has occurred
    if {$status == 1} {
	puts -nonewline stdout "ERROR: "
    }

    # If result string is present, display it
    if {$result != ""} {
	puts stdout $result
    }

    puts stdout "-----------------------------------------------------------------------------"
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : open_referral_exp_file                     	  #
# Description     : This procedure is used to open the expected   #
#		    output file for referral handling.            #
#		    The directory where the file is to be opened  #
#		    is obtained from an environment variable	  #
#		    GCT_LOG.				  	  #
# Input Arguments : NONE					  #
# Return value    : The file handle or NULL or a message string   #
#		    in case of errors. If the environment variable#
#		    is undefined, NULL is returned.		  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc open_referral_exp_file {} {

    global referral_exp_file_id env;

    set referral_exp_file_id ""

    # check if the environment variable is defined
    if { [catch {set env(GCT_LOG_DIR)} dir_name] == 0 } {

	# open the log file and catch the return status
	set status [catch {open $dir_name/obj_referral_i.exp w} result];

	if {$status == 0} {
	    # set the global variable
	    set referral_exp_file_id $result
	} else {
	    error "$result"
	}
    }
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : close_referral_exp_file                       #
# Description     : This procedure is used to close the expected  #
#		    output file for referral handling test cases  #
# Input Arguments : NONE					  #
# Return value    : Same as that of the TCL close command	  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc close_referral_exp_file {} {

    global referral_exp_file_id;

    # close the file if it has been opened
    if { $referral_exp_file_id != "" } {
	close $referral_exp_file_id
    }
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : write_to_referral_exp_file        	          #
# Description     : This procedure is used to write messages into #
#		    the expected output file for referral handling#
#		    test cases.					  #
# Input Arguments : The messages to be written		  	  #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc write_to_referral_exp_file {args} {

    global referral_exp_file_id;

    # write the message if the log file has been opened
    if { $referral_exp_file_id != "" } {
	foreach message $args {
	    puts -nonewline $referral_exp_file_id $message
	}
	puts $referral_exp_file_id ""
	flush $referral_exp_file_id
    }
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : run_referral_test_cmd              	  	  #
# Description     : This procedure is used to run the specified   #
#		    command for testing referral handling.	  #
#		    Additionally the command and the expected     #
#		    output is written into the expected output    #
#		    file.  					  #
# Input Arguments : command - The command to be run		  #
#		    args    - Variable args list of expected      #
#			      result.				  #
# Return value    : Same as the return value for TCL puts command #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc run_referral_test_cmd {command args} {

    # write the command in the expected output file
    write_to_referral_exp_file $command

    foreach i $args {
    write_to_referral_exp_file $i
    }

    write_to_referral_exp_file "-----------------------------------------------------------------------------"

    # Display the command on stdout
    puts stdout $command

    # evaluate the command and trap the error return, if any
    set status [catch {eval $command} result]

    # if an error has occurred, display it
    if {$status == 1} {
	puts -nonewline stdout "ERROR: "
    }

    # If result string is present, display it.
    if {$result != ""} {
	puts stdout $result
    }
    puts stdout "-----------------------------------------------------------------------------"
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : create_object_with_local_strings    	  #
# Description     : This procedure creates the tree in the DIT    #
#		    /c=de/o=sni/ou=ap11/cn=müller. Here the       #
#		    common name contains the german umlaut char.  #
# Input Arguments : NONE					  #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc create_object_with_local_strings {} {

    # print message in log file
    log_message "Initializing ..."

    # create the country object
    run_cmd_and_log "x500obj create /c=de -attribute ocl=c"

    # create the organization object
    run_cmd_and_log "x500obj create /c=de/o=sni -attribute ocl=org"

    # create the organizational-unit object
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11 -attribute ocl=ou"

    # create the common-name object
    run_cmd_and_log "x500obj create /c=de/o=sni/ou=ap11/cn=müller -attribute \
		{ {ocl=orp} {sn=müller} }"
}


#-----------------------------------------------------------------#
#                                                                 #
# Procedure Name  : remove_object_with_local_strings    	  #
# Description     : This procedure deletes the tree in the DIT    #
#		    /c=de/o=sni/ou=ap11/cn=müller. Here the       #
#		    common name contains the german umlaut char.  #
# Input Arguments : NONE					  #
# Return value    : NONE					  #
# Author          : B T Naik, SNI BU BA KP13, SISL India.         #
#                                                                 #
#-----------------------------------------------------------------#
proc remove_object_with_local_strings {} {

    # print message in log file
    log_message "Cleaning up ..."

    # delete the common-name object
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11/cn=müller"

    # delete the organizational-unit object
    run_cmd_and_log "x500obj delete /c=de/o=sni/ou=ap11"

    # delete the organization object
    run_cmd_and_log "x500obj delete /c=de/o=sni"

    # delete the country object
    run_cmd_and_log "x500obj delete /c=de"
}
