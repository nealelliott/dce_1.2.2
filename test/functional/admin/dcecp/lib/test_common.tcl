# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: test_common.tcl,v $
# Revision 1.1.6.1  1996/10/03  18:35:16  arvind
# 	Initial DCE1.2.2 branch.
# 	[1996/09/09  12:51 UTC  truitt  /main/truitt_dcecp_pk4/1]
#
# 	Update the Upgrade_Rootdir() function to verify each clearinghouse.
# 	[1996/03/11  02:08:36  marty  1.1.4.3]
#
# Revision 1.1.4.3  1996/03/11  02:08:36  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:30  marty]
# 
# Revision 1.1.4.2  1995/12/13  15:23:43  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:50:33  root]
# 
# Revision 1.1.2.3  1994/10/05  22:01:22  rousseau
# 	Updates for cds hcell tests
# 	[1994/10/05  22:00:22  rousseau]
# 
# Revision 1.1.2.2  1994/10/01  01:31:43  rousseau
# 	Add functions for dynamic string binding generation.
# 	[1994/10/01  01:30:41  rousseau]
# 
# Revision 1.1.2.1  1994/09/28  21:36:51  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:05:52  melman]
# 
# Revision 1.1.2.20  1994/09/27  19:42:45  rousseau
# 	Fix dcp_get_binding_info (CR 12268).
# 	[1994/09/27  19:42:39  rousseau]
# 
# Revision 1.1.2.19  1994/09/22  16:01:08  rousseau
# 	Added dcp_get_binfing_info (CR 12268).
# 	[1994/09/22  16:00:59  rousseau]
# 
# Revision 1.1.2.18  1994/09/20  22:11:54  pwang
# 	Added match_string.
# 	[1994/09/20  22:09:38  pwang]
# 
# Revision 1.1.2.17  1994/09/02  14:37:22  kevins
# 	OT 10617 changed -linkto to -to
# 	[1994/09/02  09:39:36  kevins]
# 
# Revision 1.1.2.16  1994/07/18  20:04:48  salamone
# 	Add polymorphic object support to ACL procedures.
# 	[1994/07/18  20:03:55  salamone]
# 
# Revision 1.1.2.15  1994/06/26  13:14:29  kevins
# 	OT 11079 Fixed auto_load/unknown under TET
# 	[1994/06/26  13:14:18  kevins]
# 
# Revision 1.1.2.14  1994/06/20  19:52:03  coren
# 	Added dcp_no_error function.
# 	[1994/06/16  17:08:51  coren]
# 
# Revision 1.1.2.13  1994/06/10  22:00:02  pwang
# 	Merged with changes from 1.1.2.12
# 	[1994/06/10  21:59:53  pwang]
# 
# 	Added dcp_search_element function
# 	[1994/06/10  21:57:27  pwang]
# 
# Revision 1.1.2.12  1994/06/10  20:01:28  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:51:57  devsrc]
# 
# Revision 1.1.2.11  1994/05/26  20:20:55  coren
# 	Modified compare_lists to condense multiple spaces.
# 	[1994/05/26  19:11:43  coren]
# 
# Revision 1.1.2.10  1994/05/19  14:10:47  coren
# 	Added setup procedures for "directory merge" and some routines for
# 	comparing the contents of directories.
# 	[1994/05/17  20:59:13  coren]
# 
# Revision 1.1.2.9  1994/05/11  13:48:00  kevins
# 	updated dcecp_multi_values function
# 	[1994/05/11  13:47:23  kevins]
# 
# Revision 1.1.2.8  1994/05/06  15:34:52  rousseau
# 	Added dcp_hide (again), and made print_verbose even more verbose (CR 10558).
# 	[1994/05/06  15:34:37  rousseau]
# 
# Revision 1.1.2.7  1994/05/02  19:26:08  salamone
# 	Added LOG FVT support
# 	[1994/04/29  22:34:32  salamone]
# 
# Revision 1.1.2.6  1994/04/21  20:59:32  rousseau
# 	Merged up to latest rev.
# 	[1994/04/21  20:59:15  rousseau]
# 
# Revision 1.1.2.5  1994/03/24  16:25:29  salamone
# 	Add ACL FVT support
# 	[1994/03/24  16:24:29  salamone]
# 
# Revision 1.1.2.4  1994/03/09  19:20:07  kevins
# 	Initial test drop.
# 	[1994/03/09  19:17:16  kevins]
# 
# 	fixed return statements
# 
# Revision 1.1.2.2  1994/02/25  22:17:26  rousseau
# 	Minor change to return FAIL for test fail.
# 	[1994/02/25  22:17:06  rousseau]
# 
# Revision 1.1.2.1  1994/02/10  23:15:12  rousseau
# 	Initial test drop.
# 	[1994/02/10  23:14:52  rousseau]
# 
# $EndLog$

#
# tcl test scaffold common routines
#

set auto_noexec 1

proc print_verbose {contents_of_test code answer passing_results} {
    tet_infoline ==== Contents of test case:
    tet_infoline $contents_of_test

    switch $code {
	0 {
	    tet_infoline Test completed successfully.
	    tet_infoline == Test output was:
	    tet_infoline $answer
            tet_infoline == Expected output was:
            tet_infoline $passing_results 
	}
	1 {
	    tet_infoline == Test generated error.
	    tet_infoline == Test output was:
	    tet_infoline $answer
	}
	2 {
	    tet_infoline == Test generated return exception.
	    tet_infoline == Test output was:
	    tet_infoline $answer
	}
	3 {
	    tet_infoline == Test generated break exception.
	}
	4 {
	    tet_infoline == Test generated continue exception.
	}
	* {
	    tet_infoline == Test generated exception $code.
	    tet_infoline == Test output was:
	    tet_infoline $answer
            tet_infoline == Expected output was:
            tet_infoline $passing_results 
	}
    }
}


proc test {contents_of_test comparison_function passing_results} {
    global VERBOSE

    set code [catch {uplevel $contents_of_test} answer]
    if {$code != 0} {
	if {$VERBOSE} {
            print_verbose $contents_of_test $code $answer $passing_results
	}
        tet_result FAIL
    } elseif {[$comparison_function $answer $passing_results] == 0} then { 
	if {$VERBOSE} {
	    print_verbose $contents_of_test $code $answer $passing_results
	}
	tet_infoline Test Passed.
	tet_result PASS
    } else {
	if {$VERBOSE} {
	    print_verbose $contents_of_test $code $answer $passing_results
	}
	tet_infoline Test Failed.
	tet_result FAIL
    }
}


#
# Null comparison
#
# Always returns 0.
#
# Purpose is to supply as a "comparison function" for a test when the
# only question of interest is whether "contents_of_test" returned
# without error.
#

proc dcp_no_error {s1 s2} {
  return 0
}


#
# Compare two strings
# 
# Returns 0 if strings are the same
# Returns 1 or -1 if string1 is less than or greater than string2 
#   respectively.
#

proc compare_strings {string1 string2} {
  return [string compare $string1 $string2]
}
    
#
# Match two strings
# 
# Returns 0 if strings are NOT matched 
# Returns 1 if strings does matchedi
#

proc match_strings {string1 string2} {
  return [string match $string1 $string2]
}
    

#
# Compare two lists
# 
# Returns 0 if strings are the same
# Returns 1 or -1 if list1 is less than or greater than list2 
#   respectively.
#
# This is the same as a string comapre except that all CR's are
# translated into spaces, and all sequences of spaces are reduced to a
# single space in the list before comparison.

proc compare_lists {list1 list2} {
    regsub -all "\n" $list1 " " string1
    regsub -all " +" $string1 " " string1
    regsub -all "\n" $list2 " " string2
    regsub -all " +" $string2 " " string2
    return [string compare $string1 $string2]
}

# Wrap all calls to trap for errors
#
# Does nothing if the command works OK.
# Logs the error and exits if the command returns an error.
#

proc dcp_wrap {cmd} {
    set rescode [catch {uplevel $cmd} result]
    if {$rescode != 0} {
        tet_infoline Command failed: $cmd
        tet_infoline Command returned: $result
        tet_infoline Test aborted.
        tet_result UNRESOLVED
    }
    return $rescode
}

# Wrap all calls to trap for errors
#
# Run a command, and hide any errors.
#

proc dcp_hide {cmd} {
    catch {uplevel $cmd} result
    return 0
}

#
# Extract an element from a list using a tag
#
# Returns the string found after the tag in the list or
# an empty string (and -1) if no match was found.

proc dcp_get_element {source tag} {
    foreach line $source {
	set i [string compare [lindex $line 0] $tag]
	if {$i == 0} {
	    return [lindex $line 1]
	}
    }
    return
}

#
# Search an element from a list
#
# Returns 0 if found in the list or -1 if no match 
# was found.

proc dcp_search_element {source sstring} {
    set ret [lsearch $source $sstring] 
    if { $ret != -1 } { return 0 }
    return -1 
}

#
# This function manipulates attribute lists. Currently it only 
# supports a search function. Upon finding the attribute value 
# it returns the {attrname value} list. 
#

proc dcp_attrlist {type msg search_case} {
    foreach el $msg {
        set i [ string compare [lindex $el 0 ] $search_case ] 
        if { $i == 0 } {
            if { [string compare $type search] == 0 } {
               return $el
            } elseif {[string compare $type getval] == 0 } {
	       return [lrange $el 1 end]
            }
        }
    }
    return -1
}

#
# find an attribute. Regardless of the value. If it exists return 
# 0 else return -1.
#
proc dcp_find_attr {attr_list value } {
    set i [dcp_attrlist search $attr_list $value]
    if { $i != -1 } {return 0}
    return -1
}


#
# Find a value in a pgo list. This can be either a simple or cell qualified 
# name. 
#
proc dcp_find_pgo {pgo_list value} {
    foreach el $pgo_list {
	catch {string match *$value $el} answer
        if { $answer == 1 }  {return 0}
    }
    return -1
}
	

#
# verify a range of attribute values passed in as args.
# list_value is a list of attributes to check.
# base_list is a list of attributes to check against
# if base_list value(s) are found in list_value 
# found is returned. Otherwise the mismatched attribute 
# entry from base_list is returned. 
#

proc dcp_multi_values {list_value base_list} {
   foreach el $base_list {
     set attr [lindex $el 0]
     set value [lrange $el 1 end ]
     if { [string match [dcp_attrlist getval $list_value $attr] $value] != 1 } {
	  return "$el not found"
     }
   }
   return found
}

#
# Verify that an entry(ies) does not exist in a list
#

proc dcp_missing { list_value base_list } {
    foreach el $base_list {
        foreach element $list_value {
           if { [string match [lindex $el 0] [lindex  $element 0]] == 1 } {
               return "$el found"
           }
        }
    }
    return {not found}
}

#
# verify that an entry(ies) exist in a list
# 

proc dcp_found { list_value base_list } {
    foreach el $base_list {
       if {[dcp_find_attr $list_value [lindex  $el 0]] != 0 } {
           return "$el not found"
       }
    }
    return {found}
}

#
# PROCEDURE: dcp_acl_wrap_test
#
# OVERVIEW:
# This procedure is used for acl operations: permissions and show.
# Also, this procedure is used for log operations: list and show.
#
# A comparison test cannot be done unless the expected output is
# hard-coded in the test plan.
#
# INPUTS:
# command:  Command that is to be tested.
#
# OUTPUTS:
# N/A
#
# RETURNS:
#  0    if test succeeds
#  FAIL if test fails
#
proc dcp_acl_wrap_test {command} {

    tet_infoline Command: $command
    set rc [catch {uplevel $command} test_result]
    tet_infoline Command Result: $test_result
    if {$rc == 0} {
        tet_infoline Test Completed Successfully.
        tet_infoline Test Passed.
 	tet_result PASS
    } else {
        tet_infoline Test Failed.
 	tet_result FAIL
    }
}

#
# PROCEDURE: dcp_acl_modify_test
#
# OVERVIEW:
# Verify that the exact ACL entry exists for each ACL object.
# Note that this procedure can be deleted once acl_modify_P 
# has been changed to use the "test" procedure instead.
#
# INPUTS:
# contents_of_test:  
#               Command to tested.
# acl_type:     Valid ACL types are:
#		{}:     ACL on the object itself
#		ic:     ACL for the initial container 
#		io:     ACL for the initial object
#		entry:  ACL on the namespace entry 
# entry type:   ACL entry type for the specified ACL entry.
# entry key:    ACL entry key for the specified ACL entry. Note that 
#               a NULL string {} should be specified if the ACL entry 
#               does not require a key.
# entry_perms:  ACL permissions set for the specified ACL entry.
#
# OUTPUTS:
#
# RETURNS:
#  PASS   if successful
#  FAIL   if fails
#
proc dcp_acl_modify_test {contents_of_test acl_type expected_entry_type expected_entry_key expected_entry_perms} {

    set rc [catch {uplevel $contents_of_test} error_msg]
    if {$rc != 0} {
        tet_infoline Test Failed on $contents_of_test
        tet_infoline Error: $error_msg
    } else {
        tet_infoline Test Completed Successfully on $contents_of_test
	set object_list [lindex $contents_of_test 2]
        set rc [dcp_acl_find_entry $object_list $acl_type $expected_entry_type $expected_entry_key $expected_entry_perms]
        tet_infoline Comparison Result: $rc
    }

    if {$rc == 0} {
        tet_infoline Test Passed.
	tet_result PASS
    } else {
	tet_result FAIL
    }

}

#
# PROCEDURE: dcp_acl_find_entry
#
# OVERVIEW:
# Verify that the exact ACL entry exists for each ACL object.
# Note that this procedure can be deleted once acl_modify_P 
# has been changed to use the "test" procedure instead.
#
# INPUTS:
# object_list:  Contains a list of ACL objects
# acl_type:     Valid ACL types are:
#		{}:     ACL on the object itself
#		ic:     ACL for the initial container 
#		io:     ACL for the initial object
#		entry:  ACL on the namespace entry 
# entry type:   ACL entry type for the specified ACL entry.
# entry key:    ACL entry key for the specified ACL entry. Note that 
#               a NULL string {} should be specified if the ACL entry 
#               does not require a key.
# entry_perms:  ACL permissions set for the specified ACL entry.
#
# OUTPUTS:
#
# RETURNS:
#  0   if successful
# -1   if fails
#
proc dcp_acl_find_entry {object_list acl_type entry_type entry_key entry_perms_list} {

    tet_infoline ACL object list: $object_list
    tet_infoline ACL type:   $acl_type
    set i 0

    foreach object $object_list {
        if {[llength $acl_type] == 0} {
	    set rc [catch {acl show $object} show_result]
        } else {
    	    set rc [catch {acl show $object -$acl_type} show_result]
        }
	tet_infoline Result from acl show on $object === $show_result
	if {$rc != 0} {
            tet_infoline Test Failed on acl show command for $object.
	    return -1
	}

	# If more than 1 ACL object is specified and the
	# command is "acl check", then we must compare 
	# the first $object with the corresponding first
	# $entry_perms to an entry from the "acl show"
	# command, the second $object with the corresponding 
	# second $entry_perms to an entry from the "acl show"
	# command, etc...
        if {[llength $entry_perms_list] > 1} {
	    set entry_perms [lindex $entry_perms_list $i]
	    incr i 
	} else {
	    set entry_perms $entry_perms_list
	}

        if {[llength $entry_key] == 0} {
            tet_infoline ACL entry to be compared: "$entry_type $entry_perms"
            if {[string first "{$entry_type $entry_perms}" "$show_result"] == -1} {
                tet_infoline Test Failed for $object.
		return -1
	    } 
	} else {
            tet_infoline ACL entry to be compared: "$entry_type $entry_key $entry_perms"
            if {[string first "{$entry_type $entry_key $entry_perms}" "$show_result"] == -1} {
                tet_infoline Test Failed for $object.
		return -1
	    } 
	}

    } 

    tet_infoline Test Comparison Completed Successfully.
    return 0

}

#
# PROCEDURE: dcp_acl_find_entry2
#
# OVERVIEW:
# Only called from the dcp_acl_find_multi_entries procedure.
# For each ACL object, this procedure verifies that the ACL entry
# exists.
#
# INPUTS:
# object_list:  Contains a list of ACL objects
# acl_type:     Valid ACL types are:
#		{}:     ACL on the object itself
#		ic:     ACL for the initial container 
#		io:     ACL for the initial object
#		entry:  ACL on the namespace entry 
# mgr_type:     Valid ACL manager types are:
#               {}:     none specified
#               ACL manager name documented for DCE.
# entry:        1 ACL entry to be compared to the ACL entries
#               obtained from the "acl show" command on each specified 
#               ACL object.
#
# OUTPUTS:
#
# RETURNS:
#   0      if ACL entry exists
#  -1      if ACL entry does not exist
#
proc dcp_acl_find_entry2 {object_list acl_type mgr_type entry} {

    tet_infoline ACL object list:  $object_list
    tet_infoline ACL type:         $acl_type
    tet_infoline ACL manager type: $mgr_type

    foreach object $object_list {
        if {[llength $acl_type] == 0} {
	    if {[llength $mgr_type] == 0} {
	        set rc [catch {acl show $object} show_result]
            } else {
	        set rc [catch {acl show $object -type $mgr_type} show_result]
	    }
        } else {
	    if {[llength $mgr_type] == 0} {
    	        set rc [catch {acl show $object -$acl_type} show_result]
            } else {
	        set rc [catch {acl show $object -$acl_type -type $mgr_type} show_result]
	    }
        }
	tet_infoline Result from acl show on $object === $show_result
	if {$rc != 0} {
            tet_infoline Test Failed on acl show command for $object.
	    return -1
	}

       tet_infoline ACL entry to be compared: "$entry"
       if {[string first "{$entry}" "$show_result"] == -1} {
	    return -1
	} 
    }
    return 0
}

#
# PROCEDURE: dcp_acl_find_multi_entries
#
# OVERVIEW:
# For ACL entries that exist, this procedure verifies one or more ACL
# entries that exist in one or more ACL objects. 
# However, for ACL entries that do not exist, this procedure verifies
# only 1 ACL entry that does not exist in only 1 ACL object.
#
# INPUTS:
# object_list:  Contains a list of ACL objects
# acl_type:     Valid ACL types are:
#		{}:     ACL on the object itself
#		ic:     ACL for the initial container 
#		io:     ACL for the initial object
#		entry:  ACL on the namespace entry 
# mgr_type:     Valid ACl manager types are:
#               {}:     none specified
#               ACL manager name documented for DCE.
# entry_list:   List of ACL entries to be compared to the ACL entries
#               obtained from the "acl show" command on the specified 
#               ACL object(s).
#               Note that ACL entries must be in the following format:
#               type [key ]perms
#
# OUTPUTS:
#
# RETURNS:
#  found       if ACL entry exists
#  not found   if ACL entry does not exist
#
proc dcp_acl_find_multi_entries {object_list acl_type mgr_type entry_list} {

    tet_infoline ACL object list:  $object_list
    tet_infoline ACL type:         $acl_type
    tet_infoline ACL manager type: $mgr_type
    tet_infoline ACL entry list:   $entry_list

    foreach entry $entry_list {
	set rc [dcp_acl_find_entry2 $object_list $acl_type $mgr_type $entry]
	if {$rc != 0} {
	    return {not found}
	}
    } 

    return {found}
}

#
# PROCEDURE: dcp_acl_find_cell
#
# OVERVIEW:
# This procedure verifies that the default cell name matches the
# default cell name found in one or more ACL objects. 
#
# INPUTS:
# object_list:  Contains a list of ACL objects
# acl_type:     Valid ACL types are:
#		{}:     ACL on the object itself
#		ic:     ACL for the initial container 
#		io:     ACL for the initial object
#		entry:  ACL on the namespace entry 
# new_cell_name:  
#               Default cell name to be compared
#
# OUTPUTS:
#
# RETURNS:
#  found       if default cell name matches
#  not found   if default cell name does not match
#
proc dcp_acl_find_cell {object_list acl_type new_cell_name} {

    tet_infoline ACL object list: $object_list
    tet_infoline ACL type:   $acl_type
    set i 0

    foreach object $object_list {
        if {[llength $acl_type] == 0} {
	    set rc [catch {acl show $object -cell} show_result]
        } else {
    	    set rc [catch {acl show $object -$acl_type -cell} show_result]
        }
	tet_infoline Result from acl show on $object === $show_result
	if {$rc != 0} {
            tet_infoline Test Failed on acl show command for $object.
	    return {not found}
	}

       tet_infoline Default cell name to be compared: "$new_cell_name"
       if {[string compare "$new_cell_name" "$show_result"] == -1} {
            tet_infoline Test Failed for $object.
	    return {not found}
	} 

    } 

    tet_infoline Test Comparison Completed Successfully.
    return {found}
}

#
# PROCEDURE" dcp_acl_change_entry
#
# OVERVIEW:
# Change and ACL entry on a specified object.
#
# INPUTS:
# object:      CDS name of object
# type:        type of ACL entry
# name:        identifier to whom permissions apply
# permissions: permissions to be set in ACl entry
#
# OUTPUTS:
#
# RETURNS:
#  error message   if failed
#  empty string    if successful
#
proc dcp_acl_change_entry {object type name permissions} {

    set aclentry [list $type $name $permissions]
    if [catch {acl modify $object -change $aclentry} message] {
      return -code error $message
   }
}

#
# PROCEDURE: dcp_log_find_multi_routings
#
# OVERVIEW:
# This procedure verifies one or more SVC routings in one or more objects. 
#
# INPUTS:
# object_list:  Contains a list of objects
# routing_type: Valid ACL types are:
#		{}:     Indicates production routing
#		debug:  Indicates debug routing
# routing_list: List of SVC routings to be compared to the SVC routings
#               obtained from the "log show" command on the specified 
#               object(s).
#               Note that all SVC routings must be in TCL syntax.
#
# OUTPUTS:
#
# RETURNS:
#  found       if routing matches
#  not found   if routing does not matches
#
proc dcp_log_find_multi_routings {object_list routing_type routing_list} {

    tet_infoline object list: $object_list
    tet_infoline routing type: $routing_type
    tet_infoline routing entry list:  $routing_list

    foreach routing $routing_list {
	set rc [dcp_log_find_routing $object_list $routing_type $routing]
	if {$rc != 0} {
	    return {not found}
	}
    } 

    return {found}
}

#
# PROCEDURE: dcp_log_find_routing
#
# OVERVIEW:
# Only called from the dcp_log_find_multi_routings procedure.
# For each object, this procedure verifies that the SVC routing
# exists.
#
# INPUTS:
# object_list:  Contains a list of objects
# routing_type: Valid ACL types are:
#		{}:     Indicates production routing
#		debug:  Indicates debug routing
# routing:      1 SVC routing to be compared to the SVC routings
#               obtained from the "log show" command on each specified 
#               object.
#
# OUTPUTS:
#
# RETURNS:
#   0      if SVC routing exists
#  -1      if SVC routing does not exist
#
proc dcp_log_find_routing {object_list routing_type routing} {

    tet_infoline object list:    $object_list
    tet_infoline routing type:   $routing_type

    foreach object $object_list {
        if {[llength $routing_type] == 0} {
	    set rc [catch {log show $object} show_result]
        } else {
    	    set rc [catch {log show $object -$routing_type} show_result]
        }
	tet_infoline Result from log show on $object === $show_result
	if {$rc != 0} {
            tet_infoline Test Failed on log show command for $object.
	    return -1
	}

       tet_infoline SVC routing to be compared: "$routing"
       if {[string first "{$routing}" "$show_result"] == -1} {
	    return -1
	} 
    }
    return 0
}


#
# PROCEDURE: dcp_dir_merge_setup
#
# OVERVIEW:
# Sets up for a test of "directory merge" by creating (if necessary)
# the source directory and its contents, and the target directory
#
# INPUTS:
#
# OUTPUTS:
# 
# RETURNS:
# Error message in case of failure, empty string otherwise

proc dcp_dir_merge_setup {} {

# find out if source dir exists; if not, create it and its contents.
# If other error in listing it, abort.

    if [catch {set srclist [directory list /.:/testmersrc -simplename]} message] {
       if {[compare_strings $message "Requested entry does not exist"] != 0} {
          return -code error $message
       }

       if [catch {dcp_dir_merge_create_src} message] {
          return -code error $message
       }
    } elseif {$srclist != {d1 d2 o1 l1}} {


# source dir exists; make sure its contents are as expected. If not,
# delete and recreate it.

       if [catch {directory delete /.:/testmersrc -tree} message] {
          return -code error $message
       }

       if [catch {dcp_dir_merge_create_src} message] {
          return -code error $message
       }
    }

# if target dir doesn't exist, create it; if it exists but is not
# empty, delete and recreate it.

    if [catch {set targlist [directory list /.:/testmertarg]} message] {
       if {[compare_strings $message "Requested entry does not exist"] != 0} {
          return -code error $message
       }
       if [catch {directory create /.:/testmertarg} message] {
          return -code error $message
       }
    } elseif {[llength $targlist] > 0} {
       if [catch {directory delete /.:/testmertarg -tree} message] {
          return -code error $message
       }
       if [catch {directory create /.:/testmertarg} message] {
          return -code error $message
       }
   }
   return 0
}

#
# dcp_dir_merge_create_src
#
# used by dcp_dir_merge_setup. Creates the source directory and some
# directories, objects, and links within it.

proc dcp_dir_merge_create_src {} {
   if [catch {directory create /.:/testmersrc} message] {
      return -code error $message
   }

   if [catch {directory create {/.:/testmersrc/d1 /.:/testmersrc/d2}} message] {
      return -code error $message
   }

   if [catch {object create {/.:/testmersrc/o1 /.:/testmersrc/d1/o11}} message] {
      return -code error $message
   }

   if [catch {link create /.:/testmersrc/l1 -to /.:/testmersrc/o1} message] {
      return -code error $message
   }
}


#
# PROCEDURE: dcp_dir_compare_dirs
#
# OVERVIEW:
# Compares two directories to see if they contain objects of the same
# names and types
#
# INPUTS:
# Names of the two directories.
#
# OUTPUTS:
#
# RETURNS:
#  0 if the directory contents match
# -1 if they do not match

proc dcp_dir_compare_dirs {dir1 dir2} {

    if {[directory list $dir1 -directories -simplename] != [directory list $dir2 -directories -simplename]} {
      return -1
    }

    if {[directory list $dir1 -objects -simplename] != [directory list $dir2 -objects -simplename]} {
      return -1
    }

    if {[directory list $dir1 -links -simplename] != [directory list $dir2 -links -simplename]} {
      return -1
    }

    return 0
}


#
# PROCEDURE: dcp_dir_compare_trees
#
# OVERVIEW:
# Compares two directory hierarchies to see if they and their
# subdirectories contain objects of the same names and types
#
# INPUTS:
# Names of the directories at the tops of the two trees.
#
# OUTPUTS:
#
# RETURNS:
#  0 if the tree contents match
# -1 if they do not match

proc dcp_dir_compare_trees {dir1 dir2} {

    if {[dcp_dir_compare_dirs $dir1 $dir2] != 0} {
       return -1
    }

    set dlist [directory list $dir1 -directories -simplename]
    foreach this_dir $dlist {
       if {[dcp_dir_compare_trees $dir1/$this_dir $dir2/$this_dir] != 0} {
          return -1
       }
    }

    return 0
}

#
# dcp_get_binding_info
#  Get binding information from a namespace entry
# 
proc dcp_get_binding_info {entry interface} {
    set x [rpcentry show $entry]

    set uuid [lindex [lindex $x [expr [llength $x] - 1]] 0]
    set binding [lindex [rpcentry import $entry -interface $interface] 0]
    set endpoint [lindex [resolve $binding -interface $interface \
                                           -object $uuid] 2]
    return "${uuid} ${binding} ${endpoint}"
}
    
#
# dcp_acl_binding_vars
#
# Set up variables needed for acl string binding tests
#
proc dcp_acl_binding_vars {} {
    global OBJECT_UUID PROTSEQ NETADDR ENDPOINT

    set bind_info [dcp_get_binding_info /.:/sec {47b33331-8000-0000-0d00-01dc6c000000 1.0}]

    set OBJECT_UUID [lindex $bind_info 0]
    set PROTSEQ [lindex $bind_info 1]
    set NETADDR [lindex $bind_info 2]
    set ENDPOINT [lindex $bind_info 3]
}

#
# dcp_log_binding_vars
#
# Set up variables needed for log string binding tests
#
proc dcp_log_binding_vars {} {
    global DCP_LOG_OBJECT_UUID DCP_LOG_PROTSEQ DCP_LOG_NETADDR 
    global DCP_LOG_ENDPOINT DCP_HOSTNAME_ONE

    set bind_info [dcp_get_binding_info /.:/hosts/$DCP_HOSTNAME_ONE/dts-entity {019ee420-682d-11c9-a607-08002b0dea7a 1.0}]

    set DCP_LOG_OBJECT_UUID [lindex $bind_info 0]
    set DCP_LOG_PROTSEQ [lindex $bind_info 1]
    set DCP_LOG_NETADDR [lindex $bind_info 2]
    set DCP_LOG_ENDPOINT [lindex $bind_info 3]
}

#
# Upgrade_Rootdir
#
# Procedure to upgrade the CDS_DirectoryVersion of the root directory to 4.0.
# Verify each existing clearinghouse, not just the primary clearinghouse.
#
proc Upgrade_Rootdir {} {

    # Verify each clearinghouse in the local cell.
    foreach clhouse [clearinghouse catalog] {
        clearinghouse verify $clhouse
    }

    # CDS_DirectoryVersion of root dir must be 4.0
    if {[string compare [dcp_get_element [directory show /.:] CDS_DirectoryVersion] 4.0] != 0} {
        if {[dcp_wrap {directory modify /.: -add {CDS_UpgradeTo 4.0}
                       directory synchronize /.:
                       foreach clhouse [clearinghouse catalog] {clearinghouse verify $clhouse}
                       directory synchronize /.:}] != 0} {return 1;}
    } else {
        # Already 4.0
        return 0;
    }

    # Check directory version again
    if {[string compare [dcp_get_element [directory show /.:] CDS_DirectoryVersion] 4.0] != 0} {return 1;}

    return 0;
}
