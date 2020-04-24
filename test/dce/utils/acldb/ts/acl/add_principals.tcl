#!/usr/bin/dcecp
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: add_principals.tcl,v $
# Revision 1.1.5.2  1996/03/11  01:43:22  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:14:31  marty]
#
# Revision 1.1.5.1  1995/12/08  22:06:59  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:42:43  root]
# 
# Revision 1.1.3.4  1994/10/02  21:27:38  bowe
# 	Use "#!/usr/bin/dcecp". [CR 12363]
# 	[1994/10/02  21:27:20  bowe]
# 
# Revision 1.1.3.3  1994/06/17  18:45:34  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:34:52  devsrc]
# 
# Revision 1.1.3.2  1994/06/01  17:54:56  ganni
# 	Removed CELL_ADMIN
# 	[1994/06/01  17:54:40  ganni]
# 
# Revision 1.1.2.3  94/06/01  13:38:12  baum
# 	Removed CELL_ADMIN
# 
# Revision 1.1.2.2  1994/05/25  15:48:25  baum
# 	Added passwords
# 
# Revision 1.1.3.1  1994/05/19  14:34:50  baum
# 	Resync tests with ACL & DB API changes
# 	[1994/05/19  14:34:14  baum]
# 
# 	initail checkin
# 
# $EndLog$

##
## Create keytabAdd the principals needed by FVT's
## 

set TEST_PRINC1 [lindex $argv 0]
set TEST_PRINC2 [lindex $argv 1]
set CELL_ADMIN_PWD [lindex $argv 2]

proc principal_setup {} {

  global TEST_PRINC1 TEST_PRINC2 CELL_ADMIN_PWD
 	
	
  # Add 2 principals, $TEST_PRINC1 & TEST_PRINC2

  set ret [catch "organization create osf" msg]
  set ret [catch "group create osf" msg]

  set ret [catch "principal create $TEST_PRINC1" msg]
  set ret [catch "group add osf -member $TEST_PRINC1" msg]
  set ret [catch "organization add osf -member $TEST_PRINC1" msg]
  set ret [catch "account delete $TEST_PRINC1" msg]
  set ret [catch "account create $TEST_PRINC1 -attr {{organization osf} {password $TEST_PRINC1} {group osf} {mypwd $CELL_ADMIN_PWD}}" msg]
  if { $ret != 0 } {
    puts stdout "Could not add TEST_PRINC1"
    puts stdout $msg
  }
  
  set ret [catch "principal create $TEST_PRINC2" msg]
  set ret [catch "group add osf -member $TEST_PRINC2" msg]
  set ret [catch "organization add osf -member $TEST_PRINC2" msg]
  set ret [catch "account delete $TEST_PRINC2" msg]
  set ret [catch "account create $TEST_PRINC2 -attr {{organization osf} {password $TEST_PRINC2} {group osf} {mypwd $CELL_ADMIN_PWD}}" msg]
  if { $ret != 0 } {
    puts stdout "Could not add TEST_PRINC2"
    puts stdout $msg
  }

}

puts stdout "Adding TEST_PRINC1 and TEST_PRINC2 principals and accounts"
puts stdout $TEST_PRINC1
puts stdout $TEST_PRINC2

principal_setup
