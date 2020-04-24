#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: acl_delete_P.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:09:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:46  marty]
#
# Revision 1.1.4.2  1995/12/13  15:23:55  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:50:47  root]
# 
# Revision 1.1.2.3  1994/10/01  01:32:57  rousseau
# 	Dynamically generate string bindings.
# 	[1994/10/01  01:28:35  rousseau]
# 
# Revision 1.1.2.2  1994/09/29  17:55:42  salamone
# 	CR12359 - Return "UNSUPPORTED" for initial container ACLs not supporting
# 		  user_obj and group_obj ACL entry types.
# 	[1994/09/29  17:24:51  salamone]
# 
# Revision 1.1.2.1  1994/09/28  21:37:08  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:05  melman]
# 
# Revision 1.1.2.9  1994/09/26  21:04:11  salamone
# 	CR9938 - Change ACL mgr names and return UNINITIATED for those tests that aren't configured.
# 	[1994/09/26  21:02:13  salamone]
# 
# Revision 1.1.2.8  1994/08/25  15:33:03  salamone
# 	CR11635 - Handle 1 string binding specification for an ACL object.
# 	[1994/08/25  15:29:11  salamone]
# 
# Revision 1.1.2.7  1994/07/26  18:58:46  salamone
# 	CR10045 - Provide string binding support for ACL objects.
# 	[1994/07/26  18:56:19  salamone]
# 
# Revision 1.1.2.6  1994/07/18  20:04:54  salamone
# 	Add tests for polymorphic object support
# 	[1994/07/18  19:59:36  salamone]
# 
# Revision 1.1.2.5  1994/06/10  20:01:38  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:09  devsrc]
# 
# Revision 1.1.2.4  1994/05/11  19:34:05  rousseau
# 	Changed to new install paths.
# 	[1994/05/11  19:14:56  rousseau]
# 
# Revision 1.1.2.3  1994/05/06  15:00:57  melman
# 	updating to new API path
# 	[1994/05/06  15:00:21  melman]
# 
# Revision 1.1.2.2  1994/04/12  14:21:40  salamone
# 	Added "group add" and "organization add" commands for "account create"
# 	to do its job in the "acl delete" FVT suite.
# 	[1994/04/12  13:53:17  salamone]
# 
# Revision 1.1.2.1  1994/03/24  16:51:29  salamone
# 	Initial version.
# 	[1994/03/24  16:48:47  salamone]
# 
# $EndLog$

if {([catch {set TET_ROOT $env(TET_ROOT)}]) || 
    ([string length $env(TET_ROOT)] == 0)} {
    puts stderr "$argv0: TET_ROOT is not defined."
    exit
}

## DCECP test case setup
source $TET_ROOT/functional/admin/dcecp/lib/test_common.tcl

proc acl_delete_startup {} {
    global DCP_CLIENT DCP_CLIENT_PW
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Load variables for string binding tests
    dcp_acl_binding_vars
}

proc acl_delete_cleanup {} {
    if {[dcp_wrap {logout}] != 0} {return;}
}

## Test purposes
proc acl_delete_015_P {} {

    # Identify the test
    tet_infoline acl_delete_015_P: Delete all ACL entries except the user_obj entry if it 
    tet_infoline acl_delete_015_P: exists for 1 ACL object, which is not enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {group add none -member $name}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $name}] != 0} {return;}
    if {[dcp_wrap {account create $name -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $name -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.
    test { acl delete /.:/sec/principal/$name
	   acl show /.:/sec/principal/$name
        } compare_strings {{user_obj rc--f--ug}}

    # Cleanup steps
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_delete_016_P {} {

    # Identify the test
    tet_infoline acl_delete_016_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_016_P: exists for 1 ACL object, which is enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {group add none -member $name}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $name}] != 0} {return;}
    if {[dcp_wrap {account create $name -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $name -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.
    test { acl delete "/.:/sec/principal/$name"
	   acl show /.:/sec/principal/$name
        } compare_strings {{user_obj rc--f--ug}}

    # Cleanup steps
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_delete_017_P {} {

    # Identify the test
    tet_infoline acl_delete_017_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_017_P: exists for more than 1 ACL object, which are not 
    tet_infoline acl_delete_017_P: individually enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {group add none -member "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {organization add none -member "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {account create "$prin0 $prin1" -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod "/.:/sec/principal/$prin0 /.:/sec/principal/$prin1" -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $prin0 -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {login $prin1 -p $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.

    # Invoke "acl delete" as $DCP_CLIENT and logout
    test { acl delete "/.:/sec/principal/$prin0 /.:/sec/principal/$prin1"
	   logout
	   acl show /.:/sec/principal/$prin1
        } compare_strings {{user_obj rc--f--ug}}

    # Invoke "acl modify" as $prin1 and logout
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin1 -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    test { acl show /.:/sec/principal/$prin0
        } compare_strings {{user_obj rc--f--ug}}

    # Invoke "acl modify" as $prin0 and logout
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin0 -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1"}] != 0} {return;}
}

proc acl_delete_018_P {} {

    # Identify the test
    tet_infoline acl_delete_018_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_018_P: exists for more than 1 ACL object, which are individually 
    tet_infoline acl_delete_018_P: enclosed in braces.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {group add none -member "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {organization add none -member "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {account create "$prin0 $prin1" -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod "/.:/sec/principal/$prin0 /.:/sec/principal/$prin1" -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $prin0 -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {login $prin1 -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.

    # Invoke "acl delete" as $DCP_CLIENT and logout
    test { acl delete [list "/.:/sec/principal/$prin0" "/.:/sec/principal/$prin1"]
	   logout
	   acl show /.:/sec/principal/$prin1
        } compare_strings {{user_obj rc--f--ug}}

    # Invoke "acl modify" as $prin1 and logout
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin1 -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    test { acl show /.:/sec/principal/$prin0
        } compare_strings {{user_obj rc--f--ug}}

    # Invoke "acl modify" as $prin0 and logout
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin0 -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1"}] != 0} {return;}
}

proc acl_delete_019_P {} {

    # Identify the test
    tet_infoline acl_delete_019_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_019_P: exists for 1 ACL object.
    tet_infoline acl_delete_019_P: This test operates on the namespace entry ACL of the 
    tet_infoline acl_delete_019_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {group add none -member $name}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $name}] != 0} {return;}
    if {[dcp_wrap {account create $name -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -entry -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $name -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.
    test { acl delete /.:/sec/principal/$name -entry
	   acl show /.:/sec/principal/$name -entry
        } compare_strings {{user_obj rc--f--ug}}

    # Cleanup steps
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -entry -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_delete_020_P {} {

    # Identify the test
    tet_infoline acl_delete_020_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_020_P: exists for more than 1 ACL object.
    tet_infoline acl_delete_020_P: This test ensures that the namespace entry ACL for all the 
    tet_infoline acl_delete_020_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {group add none -member "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {organization add none -member "$prin0 $prin1"}] != 0} {return;}
    if {[dcp_wrap {account create "$prin0 $prin1" -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod "/.:/sec/principal/$prin0 /.:/sec/principal/$prin1" -entry -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $prin0 -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {login $prin1 -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.

    # Invoke "acl delete" as $DCP_CLIENT and logout
    test { acl delete "/.:/sec/principal/$prin0 /.:/sec/principal/$prin1" -entry
	   logout
	   acl show /.:/sec/principal/$prin1 -entry
        } compare_strings {{user_obj rc--f--ug}}

    # Invoke "acl modify" as $prin1 and logout
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin1 -entry -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    test { acl show /.:/sec/principal/$prin0 -entry
        } compare_strings {{user_obj rc--f--ug}}

    # Invoke "acl modify" as $prin0 and logout
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin0 -entry -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0 $prin1"}] != 0} {return;}
}

proc acl_delete_021_P {} {

    # Identify the test
    tet_infoline acl_delete_021_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_021_P: exists for 1 ACL object.
    tet_infoline acl_delete_021_P: This test operates on initial container ACL of the 
    tet_infoline acl_delete_021_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    tet_result UNSUPPORTED;return
    if {[dcp_wrap {return "No initial container ACL to support a user_obj entry only."}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
        # Command is [acl delete $name -ic]

        # Expected results are:
        # Successful completion.

    # Cleanup steps
}

proc acl_delete_022_P {} {

    # Identify the test
    tet_infoline acl_delete_022_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_022_P: exists for more than 1 ACL object.
    tet_infoline acl_delete_022_P: This test ensures that the initial container ACL for all the 
    tet_infoline acl_delete_022_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    tet_result UNSUPPORTED;return
    if {[dcp_wrap {return "No initial container ACL to support a user_obj entry only."}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}

    # Run the test
        # Command is [acl delete "$name $name" -ic]

        # Expected results are:
        # Successful completion.

    # Cleanup steps
}

proc acl_delete_023_P {} {

    # Identify the test
    tet_infoline acl_delete_023_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_023_P: exists for 1 ACL object.
    tet_infoline acl_delete_023_P: This test operates on initial object ACL of the 
    tet_infoline acl_delete_023_P: specified object.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name/$name}] != 0} {return;}
    if {[dcp_wrap {group add none -member $name/$name}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $name/$name}] != 0} {return;}
    if {[dcp_wrap {account create $name/$name -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -io -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -a user:$name/$name:rcidDn}] != 0} {return;}
    if {[dcp_wrap {login $name/$name -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.
    test { acl delete /.:/sec/principal/$name -io
	   acl show /.:/sec/principal/$name -io
        } compare_strings {{user_obj rc--f--ug}}

    # Cleanup steps
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -io -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {principal delete $name/$name}] != 0} {return;}
}

proc acl_delete_024_P {} {

    # Identify the test
    tet_infoline acl_delete_024_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_024_P: exists for more than 1 ACL object.
    tet_infoline acl_delete_024_P: This test ensures that the initial object ACL for all the 
    tet_infoline acl_delete_024_P: specified objects are operated on.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create "$prin0/$prin0 $prin1/$prin1"}] != 0} {return;}
    if {[dcp_wrap {group add none -member "$prin0/$prin0 $prin1/$prin1"}] != 0} {return;}
    if {[dcp_wrap {organization add none -member "$prin0/$prin0 $prin1/$prin1"}] != 0} {return;}
    if {[dcp_wrap {account create "$prin0/$prin0 $prin1/$prin1" -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod "/.:/sec/principal/$prin0 /.:/sec/principal/$prin1" -io -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin0 -a user:$prin0/$prin0:rcidDn}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin1 -a user:$prin1/$prin1:rcidDn}] != 0} {return;}
    if {[dcp_wrap {login $prin0/$prin0 -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {login $prin1/$prin1 -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {login $DCP_CLIENT -password $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.

    # Invoke "acl delete" as $DCP_CLIENT and logout
    test { acl delete "/.:/sec/principal/$prin0 /.:/sec/principal/$prin1" -io
	   logout
	   acl show /.:/sec/principal/$prin1 -io
        } compare_strings {{user_obj rc--f--ug}}

    # Invoke "acl modify" as $prin1 and logout
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin1 -io -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    test { acl show /.:/sec/principal/$prin0 -io
        } compare_strings {{user_obj rc--f--ug}}

    # Invoke "acl modify" as $prin0 and logout
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin0 -io -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}


    # Cleanup steps
    if {[dcp_wrap {principal delete "$prin0/$prin0 $prin1/$prin1"}] != 0} {return;}
}

proc acl_delete_025_P {} {

    # Identify the test
    tet_infoline acl_delete_025_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_025_P: exists for 1 ACL object specified as a string binding 
    tet_infoline acl_delete_025_P: with an object UUID in string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {group add none -member $name}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $name}] != 0} {return;}
    if {[dcp_wrap {account create $name -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $name -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.
    test { acl delete "$OBJECT_UUID@$PROTSEQ:$NETADDR principal/$name"
	   acl show /.:/sec/principal/$name
        } compare_strings {{user_obj rc--f--ug}}

    # Cleanup steps
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_delete_026_P {} {

    # Identify the test
    tet_infoline acl_delete_026_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_026_P: exists for 1 ACL object specified as a string binding 
    tet_infoline acl_delete_026_P: with an object UUID in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW OBJECT_UUID PROTSEQ NETADDR}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {group add none -member $name}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $name}] != 0} {return;}
    if {[dcp_wrap {account create $name -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $name -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.
    test { acl delete "$OBJECT_UUID $PROTSEQ $NETADDR principal/$name"
	   acl show /.:/sec/principal/$name
        } compare_strings {{user_obj rc--f--ug}}

    # Cleanup steps
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_delete_027_P {} {

    # Identify the test
    tet_infoline acl_delete_027_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_027_P: exists for 1 ACL object specified as a string binding 
    tet_infoline acl_delete_027_P: with an endpoint in string syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW OBJECT_UUID PROTSEQ NETADDR ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {group add none -member $name}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $name}] != 0} {return;}
    if {[dcp_wrap {account create $name -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $name -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.
    test { acl delete "$OBJECT_UUID@$PROTSEQ:$NETADDR\[$ENDPOINT\] principal/$name"
	   acl show /.:/sec/principal/$name
        } compare_strings {{user_obj rc--f--ug}}

    # Cleanup steps
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_delete_028_P {} {

    # Identify the test
    tet_infoline acl_delete_028_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_028_P: exists for 1 ACL object specified as a string binding 
    tet_infoline acl_delete_028_P: with an endpoint in TCL syntax.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW OBJECT_UUID PROTSEQ NETADDR ENDPOINT}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {principal create $name}] != 0} {return;}
    if {[dcp_wrap {group add none -member $name}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $name}] != 0} {return;}
    if {[dcp_wrap {account create $name -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $name -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.
    test { acl delete "$OBJECT_UUID $PROTSEQ $NETADDR $ENDPOINT principal/$name"
	   acl show /.:/sec/principal/$name
        } compare_strings {{user_obj rc--f--ug}}

    # Cleanup steps
    if {[dcp_wrap {acl mod /.:/sec/principal/$name -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}
    if {[dcp_wrap {principal delete $name}] != 0} {return;}
}

proc acl_delete_029_P {} {

    # Identify the test
    tet_infoline acl_delete_029_P: Delete all ACL entries except the user_obj entry if it
    tet_infoline acl_delete_029_P: exists for 1 polymorphic object specifying the first
    tet_infoline acl_delete_029_P: ACL manager. Note that the user_obj entry is invalid
    tet_infoline acl_delete_029_P: for the second ACL manager, secdirectory.

    # Setup steps
    if {[dcp_wrap {global DCP_CLIENT DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {set name [lindex [info level 0] 0]}] != 0} {return;}
    if {[dcp_wrap {append prin0 $name 0}] != 0} {return;}
    if {[dcp_wrap {append prin1 $name 1}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0}] != 0} {return;}
    if {[dcp_wrap {principal create $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {group add none -member $prin0}] != 0} {return;}
    if {[dcp_wrap {organization add none -member $prin0}] != 0} {return;}
    if {[dcp_wrap {account create $prin0 -group none -organization none -mypwd $DCP_CLIENT_PW -password $DCP_CLIENT_PW}] != 0} {return;}
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin0 -type "principal" -ch user_obj:rcfug}] != 0} {return;}
    if {[dcp_wrap {login $prin0 -p $DCP_CLIENT_PW}] != 0} {return;}

    # Run the test
    # This proves that there is only 1 ACL entry left: user_obj.

    test { acl delete /.:/sec/principal/$prin0 -type "principal"
	   acl show /.:/sec/principal/$prin0 -type "principal"
        } compare_strings {{user_obj rc--f--ug}}

    # Invoke "acl modify" as $prin0 and logout
    if {[dcp_wrap {acl mod /.:/sec/principal/$prin0 -type "principal" -a user:$DCP_CLIENT:rcDnfmaug}] != 0} {return;}
    if {[dcp_wrap {logout}] != 0} {return;}

    # Cleanup steps
    if {[dcp_wrap {principal delete $prin0/$prin1}] != 0} {return;}
    if {[dcp_wrap {principal delete $prin0}] != 0} {return;}

}


set iclist "ic1 ic2 ic3 ic4 ic5 ic6 ic7 ic8 ic9 ic10 ic11 ic12 ic13 ic14 ic15 "
set ic1 "acl_delete_015_P"
set ic2 "acl_delete_016_P"
set ic3 "acl_delete_017_P"
set ic4 "acl_delete_018_P"
set ic5 "acl_delete_019_P"
set ic6 "acl_delete_020_P"
set ic7 "acl_delete_021_P"
set ic8 "acl_delete_022_P"
set ic9 "acl_delete_023_P"
set ic10 "acl_delete_024_P"
set ic11 "acl_delete_025_P"
set ic12 "acl_delete_026_P"
set ic13 "acl_delete_027_P"
set ic14 "acl_delete_028_P"
set ic15 "acl_delete_029_P"
set tet_startup acl_delete_startup
set tet_cleanup acl_delete_cleanup

# Source the TET main script into our script. The TET main
# script is what will do the actual function calling.
source $TET_ROOT/lib/tcl/tcm.tcl
