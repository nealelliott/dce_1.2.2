#!/opt/dcelocal/bin/dcecp
#
# FILE: test_common.tcl
#
# USAGE:
#
# DESCRIPTION:
#      Define common procedures to test the I18N implementation 
#      in the DCE components.
#
# PREREQUISITS:
#      
#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: set_locale
#	This procedure sets the LANG and NLSPATH variables.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc set_locale {loc msgcat} {

   global OLD_LANG OLD_NLSPATH lang_was_set nlspath_was_set LANG NLSPATH
   global env

   set lang_was_set 0
   set nlspath_was_set 0

# Read current setup of NLSPATH and LANG and save them to restore this values at exit
# time.

    if { [catch {set OLD_LANG $env(LANG)} msg] } {
       tet_infoline  The variable LANG had not set before starting this test.
       set lang_was_set 0
   } else {
       tet_infoline  At the start of this test, LANG was set to $OLD_LANG.
       set lang_was_set 1
   }

   if { [catch {set OLD_NLSPATH $env(NLSPATH)} msg] } {
       tet_infoline The variable NLSPATH had not set before starting this test.
       set nlspath_was_set 0
   } else {
       tet_infoline At the start of this test, NLSPATH was set to $OLD_NLSPATH  
       set nlspath_was_set 1
   }

# Set the LANG and NLSPATH environment varaibles.

   set env(LANG)    $loc
   set env(NLSPATH) [concat $msgcat\/%N]
   
   tet_infoline  LANG set to $env(LANG)
   tet_infoline  NLSPATH set to $env(NLSPATH)
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: reset_locale
#	This procedure resets the locale to what is was set before the tests
#	started.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc reset_locale {} {
 
   global OLD_LANG OLD_NLSPATH lang_was_set nlspath_was_set
   global env

   set LANG $env(LANG)
   set NLSPATH $env(NLSPATH)

   if { $lang_was_set } {
      set env(LANG) $OLD_LANG
      tet_infoline LANG set back to $env(LANG).
   } else {
     unset env(LANG)
     tet_infoline unset LANG.
  }

   if { $nlspath_was_set } {
      set env(NLSPATH) $OLD_NLSPATH
      tet_infoline set back NLSPATH to $env(NLSPATH).
   } else {
      unset env(NLSPATH)
      tet_infoline unset NLSPATH
  }
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: run_cmd 
#	This procedure executes a command.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
proc run_cmd {cmd} {

   set sts  [catch {uplevel $cmd} answer]


   if {$sts == 0} {
       tet_result PASS
       return 0
   } else {
        tet_infoline === $answer
        tet_infoline == Command failed.
        tet_result FAIL
        return 1
   }
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: admin_login
#	This procedure logins the user as the DCE cell_admin principal.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc admin_login {} {

   global CELL_ADMIN CELL_PW

   set procname [lindex [info level 0] 0]

   if { [run_cmd {login $CELL_ADMIN -password $CELL_PW}] != 0 } {
      tet_infoline Test failed to login as cell_admin.
      tet_result FAIL
   }
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: create_directories
#	This procedure creates CDS directories. The name of the directories to 
#	create are read from a file.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
proc create_dirobj_entries {} {

   global TET_ROOT LOCALE_DATAFILE
   set fname [concat $TET_ROOT\/system/I18N/lib/$LOCALE_DATAFILE]
   
   if { [file exists $fname] == 1 } {
      set df [open $fname r]
   } else {
      tet_infoline Test Failed: I could not open locale datafile.
      tet_result FAIL
   }

   set count 0
   set dirs ""
   while { ([gets $df word] >=0) && ($count < 5) } {
      if { [llength df] > 0 } {
         lappend dirs $word
         incr count 
      }
   }

   set count 0
   set objs ""
   while { ([gets $df word] >=0) && ($count < 5) } {
      if { [llength df] > 0} {
         lappend objs $word
         incr count 
      }
   }

   set count 0
   set links ""
   while { ([gets $df word] >=0) && ($count < 5) } {
      if { [llength df] > 0 } { 
         lappend links $word
         incr count 
      }
   }
   tet_infoline ===
   tet_infoline Directory list:
   tet_infoline $dirs
   tet_infoline Object list:
   tet_infoline $objs
   tet_infoline Links list:
   tet_infoline $links

   tet_infoline ====
   tet_infoline Creating directories and objects.
   set target_links ""
   set objs_created ""
   set dirs_created ""
   foreach i $dirs {
      tet_infoline "directory create /.:/$i"
      run_cmd {directory create /.:/$i}
      tet_infoline "directory list /.:/$i"
      run_cmd {directory list /.:/$i} 
      lappend dirs_created /.:/$i
      foreach j $objs {
         tet_infoline "object create  /.:/$i/$j"
         run_cmd {object create  /.:/$i/$j}
         tet_infoline "object show /.:/$i/$j"
         run_cmd {object show /.:/$i/$j}
         lappend target_links /.:/$i/$j
         lappend objs_created /.:/$i/$j
      }
   }

   tet_infoline ===   
   tet_infoline Creating links.
   foreach k $links {
      set tolink [lindex $target_links 0]
      set target_links [lreplace $target_links 0 0]
      tet_infoline "link create /.:/$k -to $tolink"
      run_cmd {link create /.:/$k -to $tolink}
      tet_infoline "link show /.:/$k"
      run_cmd {link show /.:/$k}
   }
   tet_infoline ===
   tet_infoline  Deleting objects, directories, and links.
   foreach x $objs_created {
       tet_infoline "object show $x"
       run_cmd {object show $x}
       tet_infoline "object delete $x"
       run_cmd {object delete $x}
   }
   run_cmd {directory list /.: -links}
   run_cmd {directory list /.: -links -simplename}
#   run_cmd {directory list /.:}
#   run_cmd {directory list /.: -simplename}
   foreach y $dirs_created {
      run_cmd {directory delete $y}
   }
   foreach i $links {
       run_cmd {link delete /.:/$i}
   }
   close $df
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: acl_test
#	This procedure create a number of CDS objects and
#	then displays their ACLs.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
proc acl_test {} {

   global TET_ROOT LOCALE_DATAFILE
   set fname [concat $TET_ROOT\/system/I18N/lib/$LOCALE_DATAFILE]
   
   if { [file exists $fname] == 1 } {
      set df [open $fname r]
   } else {
       tet_infoline Test Failed: I could not open locale datafile.
       tet_result FAIL
   }

   set count 0
   set dirs ""
   while { ([gets $df word] >=0) && ($count < 5) } {
      if { [llength df] > 0 } {
         lappend dirs $word
         incr count 
      }
   }

   set count 0
   set objs ""
   while { ([gets $df word] >=0) && ($count < 5) } {
      if { [llength df] > 0} {
         lappend objs $word
         incr count 
      }
   }
   tet_infoline ===
   tet_infoline Directory list:
   tet_infoline $dirs
   tet_infoline Object list:

   tet_infoline === Creating directories and displaying acls
   foreach i $dirs {
      tet_infoline "directory create /.:/$i"
      run_cmd {directory create /.:/$i}
      run_cmd {acl check /.:/$i}
   }
   tet_infoline === Creating objects and displaying acls
   foreach i $objs {
      tet_infoline "object create /.:/$i"
      run_cmd {object create /.:/$i}
      tet_infoline "object show /.:/$i"
      run_cmd {object show /.:/$i} 
#      run_cmd {acl check /.:/$i}
   }
   tet_infoline === Delete directories
   foreach i $dirs {
      tet_infoline "directory delete /.:/$i"
      run_cmd {directory delete /.:/$i}
   }
   tet_infoline ==== Delete objects
   foreach i $objs {
      tet_infoline "object delete /.:/$i"
      run_cmd {object delete /.:/$i}
   }
   close $df
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: createCH_and_replicas
#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
proc createCH_and_replicas {} {

   global TET_ROOT LOCALE_DATAFILE
   set fname [concat $TET_ROOT\/system/I18N/lib/$LOCALE_DATAFILE]
   
   if { [file exists $fname] == 1 } {
      set df [open $fname r]
   } else {
       tet_infoline Test Failed: I could not open locale datafile.
       tet_result FAIL
   }

# Read first word and create a clearinghouse with that name.
   set ch_name [gets $df]
   tet_infoline "clearinghouse create /.:/$ch_name\_ch"
   run_cmd {clearinghouse create /.:/$ch_name\_ch}
   tet_infoline "object show /.:/$ch_name\_ch"
   run_cmd {object show /.:/$ch_name\_ch}
 
# Read next word and create a directory.
   set dir_name [gets $df]
   tet_infoline "directory create /.:/$dir_name"
   run_cmd {directory create /.:/$dir_name}

# Replicate the directory in the cleainghouse created.
   tet_infoline "directory create /.:/$dir_name -replica -clearinghouse /.:/$ch_name\_ch"
   run_cmd {directory create /.:/$dir_name -replica -clearinghouse /.:/$ch_name\_ch}
   tet_infoline "directory synchronize"
   run_cmd {directory synchronize /.:}

# Make sure the directory has been replicate by listing it

# Delete replica
  tet_infoline "directory delete /.:/$dir_name -replica -clearinghouse /.:/$ch_name\_ch"
  run_cmd {directory delete /.:/$dir_name -replica -clearinghouse /.:/$ch_name\_ch}
  tet_infoline "directory delete /.:/$dir_name"
  run_cmd {directory delete /.:/$dir_name}

# Delete clearinghouse
  tet_infoline "clearinghouse delete /.:/$ch_name\_ch"
  run_cmd {clearinghouse delete /.:/$ch_name\_ch}

# Delete directory
   close $df
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: create_rgy_accts
#	Create accounts for users. The accounts names to create are 
#	read from a file.
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc create_rgy_accts {} {
   global TET_ROOT LOCALE_DATAFILE CELL_PW
   set fname [concat $TET_ROOT\/system/I18N/lib/$LOCALE_DATAFILE]
   
   if { [file exists $fname] == 1 } {
      set df [open $fname r]
   } else {
       tet_infoline Test Failed: I could not open locale datafile.
       tet_result FAIL
   }

   set org [gets $df]
   set group [gets $df]
   set passwd [gets $df]
   tet_infoline Creating an organization: $org
   run_cmd {organization create $org}
   tet_infoline Creating a group: $group
   run_cmd {group create $group}

   while { [gets $df pname] >= 0 } {
	tet_infoline "principal create $pname"
	run_cmd {principal create $pname}
	run_cmd {group add $group -member $pname}
        run_cmd {organization add $org -member $pname}
	tet_infoline "account create $pname -group $group -org $org -password $passwd -mypwd $CELL_PW"
	run_cmd {account create $pname -group $group -org $org -password $passwd -mypwd $CELL_PW}
	tet_infoline "login $pname -password $passwd"
        run_cmd {login $pname -password $passwd}
	tet_infoline "logout"
	run_cmd {logout}
   }
   tet_infoline "Display registry after having created the accounts."
   catch {principal catalog} msg
   puts $msg
   tet_infoline "Accounts have been created."
}

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# PROCEDURE NAME: delete_rgy_accts
#
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

proc delete_rgy_accts {} {

   global TET_ROOT LOCALE_DATAFILE 
   set fname [concat $TET_ROOT\/system/I18N/lib/$LOCALE_DATAFILE]
   
   if { [file exists $fname] == 1 } {
      set df [open $fname r]
   } else {
       tet_infoline Test Failed: I could not open locale datafile.
       tet_result FAIL
   }

   set org [gets $df]
   set group [gets $df]
   set passwd [gets $df]

   while { [gets $df pname] >= 0 } {
	tet_infoline "principal delete $pname"
	run_cmd {principal delete $pname}
   }

   tet_infoline "organization delete $org"
   run_cmd {organization delete $org}
   tet_infoline "group delete $group"
   run_cmd {group delete $group}
   
   tet_infoline "Display regitry after having deleted the accounts."
   catch {principal catalog} msg
   puts ===================
   puts $msg
   tet_infoline "Accounts have been deleted."
}



