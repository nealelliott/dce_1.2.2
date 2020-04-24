#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: gdscp_test.sh,v $
# Revision 1.1.4.2  1996/03/11  02:35:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:21  marty]
#
# Revision 1.1.4.1  1995/12/11  15:25:42  root
# 	Submit
# 	[1995/12/11  14:36:07  root]
# 
# Revision 1.1.2.3  1994/06/21  14:29:20  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:49:56  marrek]
# 
# Revision 1.1.2.2  1994/06/10  20:20:54  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:14  devsrc]
# 
# Revision 1.1.2.1  1994/05/10  14:33:12  marrek
# 	Updated from old test subdir.
# 	[1994/05/10  09:35:28  marrek]
# 
# 	Add shell interpreter in first line.
# 	[1994/05/09  09:13:09  marrek]
# 
# 	Created in April 1994 submission.
# 	[1994/05/03  12:14:08  marrek]
# 
# $EndLog$
# 
# 
# $RCSfile: gdscp_test.sh,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:02 $
# 
#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : gdscp_test                               	  #
# Description : This script is the front end to the gdscp tester  #
#		It processes the user supplied arguments and      #
#		accordingly invokes gdscp with the appropriate    #
#		parameters.					  #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

# Function to print the usage message
usage()
{
    echo "Usage: gdscp_test [test_file_name] [-c <test_case_name>]"
    exit 1
}


# Function to run all the test cases
test_all()
{
    # check if invoked in the proper directory. Should be in the
    # installation directory which has a subdirectory "lib/structured"
    if [ ! -d "./lib/structured" ]
    then
	echo "gdscp_test: Cannot run test cases from this directory"
	exit
    fi

    # The XOM object information file
    XOI_SCHEMA_FILE=./xoischema.def; export XOI_SCHEMA_FILE

    # name of file containing test cases for structured output
    test_file=./lib/structured/valid/structured_v.tcl

    # The file name containing the expected result output
    exp_file=./lib/structured/valid/structured_v.exp

    # The file name where the result output is directed
    out_file=$GCT_LOG_DIR/structured_v.out

    # The log file name
    log_file=$GCT_LOG_DIR/gdscp_test.log

    # check if the required files exist
    if [ ! -f test_all.tcl ]
    then
	echo "gdscp_test: File test_all.tcl not found"
	exit
    fi

    if [ ! -f $test_file ]
    then
	echo "gdscp_test: File $test_file not found"
	exit
    fi

    # invoke gdscp with the file name option to run all the test cases
    gdscp test_all.tcl

    echo "Test cases for structured output        ... \c"

    # Now run the test cases for the structured output and redirect stdout
    gdscp $test_file > $out_file

    # Update the tester log file
    cat $out_file >> $log_file


    # Now check the output result with the expected result
    if diff $out_file $exp_file > /dev/null
    then
        echo "passed"

	# remove the created file
	rm -f $out_file
    else
        echo "failed"
        echo "Test cases for structured output failed" >> $log_file
        echo "Received result different:" >> $log_file
        echo "Output of \"diff $out_file $exp_file\":" >> $log_file
        diff $out_file $exp_file >> $log_file
    fi


    # name of file containing test cases for referral handling
    test_file=./lib/referral/invalid/obj_referral_i.tcl

    # The file name containing the expected result output
    exp_file=$GCT_LOG_DIR/obj_referral_i.exp

    # The file name where the result output is directed
    out_file=$GCT_LOG_DIR/obj_referral_i.out

    echo "Test cases for referral handling        ... \c"

    # Now run the test cases for referral handling and redirect stdout
    gdscp $test_file > $out_file

    # Update the tester log file
    cat $out_file >> $log_file

    # Now check the output result with the expected result
    if diff $out_file $exp_file > /dev/null
    then
        echo "passed"

	# remove the created files
	rm -f $out_file
	rm -f $exp_file
    else
        echo "failed"
        echo "Test cases for referral handling failed" >> $log_file
        echo "Received result different:" >> $log_file
        echo "Output of \"diff $out_file $exp_file\":" >> $log_file
        diff $out_file $exp_file >> $log_file
    fi

    exit
}


# Function to run all the test cases in a file
# This function is invoked with the file name as the argument
test_file()
{
    # check if invoked in the proper directory. Should be in the
    # directory where the script file is stored
    if [ ! -f ../../../test_utils.tcl ]
    then
	echo "gdscp_test: Cannot run test cases from this directory"
	exit
    fi

    # The XOM object information file
    XOI_SCHEMA_FILE=../../../xoischema.def; export XOI_SCHEMA_FILE

    # check if the file exists
    if [ ! -f $1 ]
    then
	echo "gdscp_test: File $1 does not exist"
	exit
    fi

    # check if test cases for referral handling is to be checked
    if [ "$1" = "obj_referral_i.tcl" -o "$1" = "structured_v.tcl" ]
    then
	# name of the file where logging is done
	log_file=$GCT_LOG_DIR/gdscp_test.log

	if [ "$1" = "obj_referral_i.tcl" ]
	then
	    echo "Test cases for referral handling        ... \c"
	    exp_file=$GCT_LOG_DIR/obj_referral_i.exp;
	    out_file=$GCT_LOG_DIR/obj_referral_i.out;
	else
	    echo "Test cases for structured output        ... \c"
	    exp_file=structured_v.exp;
	    out_file=$GCT_LOG_DIR/structured_v.out;
	fi

	# Now run test cases and redirect stdout
	gdscp $1 > $out_file

	# Update the tester log file
	cat $out_file > $log_file

	# Now check the output result with the expected result
	if diff $out_file $exp_file > /dev/null
	then
	    echo "passed"

	    # remove the created files
	    rm -f $out_file

	    if [ "$1" = "obj_referral_i.tcl" ]
	    then
		rm -f $exp_file
	    fi
	else
	    echo "failed"
	    echo "Test cases in $1 failed" >> $log_file
	    echo "Received result different:" >> $log_file
	    echo "Output of \"diff $out_file $exp_file\":" >> $log_file
	    diff $out_file $exp_file >> $log_file
	fi
    else
	# invoke gdscp with the "-c" switch
	# source in the test utilities and invoke the TCL procedure
	# which runs the test scripts in the file
	gdscp -c "source ../../../test_utils.tcl; run_test_cases_in_file $1"
    fi

    exit
}


# Function to run a single test case
# The input argument to this function is the test case name
test_single()
{
    
    # check if invoked in the proper directory.
    if [ ! -f ../../../test_utils.tcl ]
    then
	echo "gdscp_test: Cannot run test cases from this directory"
	exit
    fi

    # The XOM object information file
    XOI_SCHEMA_FILE=../../../xoischema.def; export XOI_SCHEMA_FILE

    # now invoke gdscp with the "-c" option
    gdscp -c "source ../../../test_utils.tcl; source ../../../test_config.tcl;  \
		open_log_file; source_all_test_scripts ../../../lib; $1;	  \
		close_log_file"

    exit
}


# Execution of script starts here
# Check if the environment variable for the logfile directory is defined
if [[ -n $GCT_LOG_DIR ]]
then
    # check if the directory exists
    if [ ! -d $GCT_LOG_DIR ]
    then
	echo "$0: Directory $GCT_LOG_DIR does not exist"
	exit
    fi
else
    echo "$0: Environmental variable \"GCT_LOG_DIR\" not defined"
    exit
fi


# Process the supplied arguments
case $# in
    0)
	# No arguments specified. So run all the test cases
	test_all;;
    1)
	# Maybe the file option to the tester
	case $1 in
	    -c)
		echo "$0: Missing test case name"
		usage;;
	    -*)
		echo "$0: Unknown switch $1"
		usage;;
	     *)
		# run the test cases in the file 
		test_file $1;;
	esac;;
    2)
	# May be the switch option
	case $1 in
	     -c)
		#run the test case
		test_single $2;;
	     -*)
		echo "$0: Unknown switch $1"
		usage;;
	      *)
		echo "$0: Unknown argument $1"
		usage;;
	esac;;
    *)
	echo "$0: Too many arguments"
	usage;;
esac
