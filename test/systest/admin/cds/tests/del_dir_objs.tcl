#!/opt/dcelocal/bin/dcecp
#
# Usage: del_dir_objs.tcl <testname>
#	Where testname is either DCPSAN002 or DCPSAN003

proc delete_enties {} {

   foreach i $objs {
   object delete $i
   }

   foreach i $dirs {
   directory delete $i
   }

}

set test_name [lindex $argv 0]

if {$test_name == "DCPSAN002"} {
   set dirs "/.:/br_dir_1 /.:/br_dir_2 /.:/br_dir_3" 
} elseif {$test_name == "DCPSAN003"}  {
   set dirs "/.:/dirtest01 /.:/dirtest02 /.:/dirtest03"
} else {
   set dirs ""
}
set objs [dir list $dirs -objects]
delete_enties
