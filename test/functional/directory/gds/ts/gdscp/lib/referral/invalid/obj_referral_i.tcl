# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: obj_referral_i.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:35:20  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:36  marty]
#
# Revision 1.1.4.1  1995/12/11  15:26:05  root
# 	Submit
# 	[1995/12/11  14:36:15  root]
# 
# Revision 1.1.2.7  1994/08/23  13:14:16  keutel
# 	again OT 11744
# 	[1994/08/23  10:27:22  keutel]
# 
# Revision 1.1.2.6  1994/08/18  14:42:52  keutel
# 	OT 11744: dce_printf changes
# 	[1994/08/18  13:35:18  keutel]
# 
# Revision 1.1.2.5  1994/07/06  15:11:13  marrek
# 	July 1994 code drop.
# 	[1994/07/06  07:23:16  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:29:21  marrek
# 	June 1994 code submission.
# 	[1994/06/16  07:49:58  marrek]
# 
# Revision 1.1.2.3  1994/06/10  20:21:29  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:59:22  devsrc]
# 
# Revision 1.1.2.2  1994/05/11  10:34:47  marrek
# 	Updated.
# 	[1994/05/11  10:24:14  marrek]
# 
# Revision 1.1.2.1  1994/05/10  14:33:31  marrek
# 	Updated from old test subdir.
# 	[1994/05/10  09:35:47  marrek]
# 
# 	Created in April 1994 submission.
# 	[1994/05/03  12:14:26  marrek]
# 
# $EndLog$
# 
# $RCSfile: obj_referral_i.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:35:20 $
# 

#-----------------------------------------------------------------#
#                                                                 #
# Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       #
# All Rights Reserved                                             #
#                                                                 #
# Project     : Tester for GDS Control Program (gdscp)            #
# File Name   : obj_referral_i.tcl                             	  #
# Description : This file contains all the test cases for referral#
#		handling.					  #
# Date        : February 1994                                     #
# Author      : B T Naik, SNI BU BA KP13, SISL India.             #
#                                                                 #
#-----------------------------------------------------------------#

# source in the dsa configuration file. If not present try in current directory
if {[catch {source ../../../test_config.tcl}] != 0} {
    source test_config.tcl
}

# source in the utilities file. If not present try in current directory
if {[catch {source ../../../test_utils.tcl}] != 0} {
    source test_utils.tcl
}

# split the dsa name into the respective objects
set object_list [split $dsa_name /]

# set the country object
set country_name [lindex $object_list 1]
append country_object "/" $country_name

# set the organization object
set org_name [lindex $object_list 2]
append org_object $country_object "/" $org_name

# set the organizational-unit object
set ou_name [lindex $object_list 3]
append ou_object $org_object "/" $ou_name

# set the common-name object
set cn_name [lindex $object_list 4]
append cn_object $ou_object "/" $cn_name

# open the expected output file
open_referral_exp_file

set message "TEST CASES FOR REFERRAL HANDLING"
puts stdout $message
write_to_referral_exp_file $message
set message "--------------------------------"
puts stdout $message
write_to_referral_exp_file $message
set message "Initializing ..."
puts stdout $message
write_to_referral_exp_file $message

run_referral_test_cmd "x500obj bind -dsa $dsa_name -dirid $directory_id"
run_referral_test_cmd "x500obj create $country_object -attribute ocl=c"
run_referral_test_cmd "x500obj create $org_object -attribute ocl=org"
run_referral_test_cmd "x500obj create $ou_object -attribute ocl=ou"
run_referral_test_cmd "x500obj create $cn_object -attribute ocl=app"
run_referral_test_cmd "x500obj create $dsa_name -attribute ocl=ape \"PSA={$psap_address}\""
run_referral_test_cmd "x500obj bind -dsa $dsa_name_add -dirid $directory_id_add"
run_referral_test_cmd "x500svc modify -dontusecopy  FALSE -localscope TRUE"
run_referral_test_cmd "x500obj create $country_object -attribute ocl=c \"mk={$dsa_name}\""
run_referral_test_cmd "x500obj create $org_object -attribute ocl=org \"mk={$dsa_name}\""
run_referral_test_cmd "x500obj create $ou_object -attribute ocl=ou \"mk={$dsa_name}\""
run_referral_test_cmd "x500obj create $cn_object -attribute ocl=app \"mk={$dsa_name}\""
run_referral_test_cmd "x500obj create $dsa_name -attribute ocl=ape \"PSA={$psap_address}\" \"mk={$dsa_name}\""
run_referral_test_cmd "x500svc modify -default"
run_referral_test_cmd "x500svc modify -preferadmfunctions TRUE"
run_referral_test_cmd "x500obj create $dsa_name_add -attribute ocl=ape \"PSA={$psap_address_add}\""
run_referral_test_cmd "x500svc modify -default"


set message "\nTEST CASES FOR HANDLING OF REFERRALS RETURNED AS ERRORS"
puts stdout $message
write_to_referral_exp_file $message
set message "-------------------------------------------------------"
puts stdout $message
write_to_referral_exp_file $message

run_referral_test_cmd "x500obj bind -dirid $directory_id -dsa $dsa_name"
run_referral_test_cmd "x500svc modify -automaticcontinuation FALSE -chainingprohibited TRUE"

set exp_result "ERROR: Referral returned - $dsa_name_add."
run_referral_test_cmd "x500obj show $dsa_name_add -pretty" $exp_result

set exp_result "ERROR: Referral returned - $dsa_name_add."
run_referral_test_cmd "x500obj search $dsa_name_add -baseobject -pretty" $exp_result

run_referral_test_cmd "x500svc modify -default"


set message "\nTEST CASES FOR HANDLING REFERRALS WITH NO RESULT"
puts stdout $message
write_to_referral_exp_file $message
set message "------------------------------------------------"
puts stdout $message
write_to_referral_exp_file $message

run_referral_test_cmd "x500obj bind -dirid $directory_id -dsa $dsa_name"
run_referral_test_cmd "x500svc modify -automaticcontinuation FALSE -chainingprohibited TRUE"

set exp_result "ERROR: Incomplete operation - Referral returned : $dsa_name_add.\n"
run_referral_test_cmd "x500obj search $cn_object -filter cn=ljlsfl -onelevel -pretty" $exp_result

run_referral_test_cmd "x500svc modify -default"


set message "\nTEST CASES FOR HANDLING REFERRALS WITH ONE OR MORE RESULTS"
puts stdout $message
write_to_referral_exp_file $message
set message "----------------------------------------------------------"
puts stdout $message
write_to_referral_exp_file $message

run_referral_test_cmd "x500obj bind -dirid $directory_id -dsa $dsa_name"
run_referral_test_cmd "x500svc modify -automaticcontinuation FALSE -chainingprohibited TRUE"

set exp_result1 " 1) $dsa_name"
set exp_result2 "Incomplete operation - Referral returned : $dsa_name_add."
run_referral_test_cmd "x500obj list $cn_object -pretty" $exp_result1 $exp_result2

set exp_result "$dsa_name {Incomplete operation - Referral returned : $dsa_name_add.\n}"
run_referral_test_cmd "x500obj list $cn_object" $exp_result

set exp_result1 " 1) $dsa_name"
set exp_result2 "Incomplete operation - Referral returned : $dsa_name_add."
run_referral_test_cmd "x500obj search $cn_object -onelevel -pretty" $exp_result1 $exp_result2

set exp_result "$dsa_name {Incomplete operation - Referral returned : $dsa_name_add.\n}"
run_referral_test_cmd "x500obj search $cn_object -onelevel" $exp_result

run_referral_test_cmd "x500svc modify -default"

set message "Cleaning up ..."
puts stdout $message
write_to_referral_exp_file $message

run_referral_test_cmd "x500obj bind -dsa $dsa_name_add -dirid $directory_id_add"
run_referral_test_cmd "x500svc modify -preferadmfunctions TRUE"
run_referral_test_cmd "x500obj delete $dsa_name_add"
run_referral_test_cmd "x500svc modify -default"
run_referral_test_cmd "x500svc modify -dontusecopy  FALSE -localscope TRUE"
run_referral_test_cmd "x500obj delete $dsa_name"
run_referral_test_cmd "x500obj delete $cn_object"
run_referral_test_cmd "x500obj delete $ou_object"
run_referral_test_cmd "x500obj delete $org_object"
run_referral_test_cmd "x500obj delete $country_object"
run_referral_test_cmd "x500svc modify -default"
run_referral_test_cmd "x500obj bind -dsa $dsa_name -dirid $directory_id"
run_referral_test_cmd "x500obj delete $dsa_name"
run_referral_test_cmd "x500obj delete $cn_object"
run_referral_test_cmd "x500obj delete $ou_object"
run_referral_test_cmd "x500obj delete $org_object"
run_referral_test_cmd "x500obj delete $country_object"

# close the expected output file
close_referral_exp_file
