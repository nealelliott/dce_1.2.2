#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: dtscp.ksh,v $
# Revision 1.1.9.2  1996/03/11  02:50:28  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:11:00  marty]
#
# Revision 1.1.9.1  1995/12/11  23:05:51  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/09/18  17:59 UTC  pare
# 	Merge changes back to HPDCE02
# 
# 	HP revision /main/HPDCE02/pare_MOTHRA/1  1995/09/18  15:12 UTC  pare
# 	Add sleep 10 after dtscp enable -- cell timing issue
# 
# 	HP revision /main/HPDCE02/2  1994/10/10  14:25 UTC  pare
# 	Merge bl21 changes.
# 	[1995/12/11  22:42:59  root]
# 
# Revision 1.1.7.3  1994/10/03  19:36:45  tom
# 	Fix expected errors and add a few sleeps before show state.
# 	[1994/10/03  18:28:40  tom]
# 
# 	HP revision /main/HPDCE02/1  1994/06/30  18:37 UTC  pare
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# Revision 1.1.7.2  1994/06/10  20:51:25  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:20:11  devsrc]
# 
# Revision 1.1.7.1  1994/04/21  22:04:45  cbrooks
# 	CR 3231 - remote interface
# 	[1994/04/21  20:10:39  cbrooks]
# 
# 	HP revision /main/HPDCE01/1  1994/02/01  14:45  pare
# 	merge kk and hpdce01
# 
# Revision 1.1.4.4  1993/09/30  20:25:58  pare
# 	Removed "print" statement which was causing mismatch between
# 	expected and actual output.
# 	[1993/09/30  20:25:20  pare]
# 
# Revision 1.1.4.3  1993/07/22  17:16:04  pare
# 	Remove "too many servers detected" functionality
# 	[1993/07/22  17:15:22  pare]
# 
# Revision 1.1.4.2  1993/07/09  17:21:19  root
# 	Initial King Kong branch
# 	[1993/07/09  17:20:48  root]
# 
# Revision 1.1.2.3  1993/04/29  16:44:57  truitt
# 	Add some sleep time between an enable directive and the
# 	subsequent show state command, in order to let the state
# 	reach 'on'.
# 	[1993/04/29  16:44:28  truitt]
# 
# Revision 1.1.2.2  1993/04/15  18:57:00  truitt
# 	Initial Revision.
# 	[1993/04/15  18:55:52  truitt]
# 
# $EndLog$
#
# This ksh script will execute a series of dtscp commands and
# place the resulting information onto the screen if desired,
# or into a file if the output is redirected.
#
# This test should be called by the test_dtscp.ksh script, and not
# called by hand.

blech()
{
   print $@
   $@
}

#
# test the dtscp help commands
#
test_help()
{
   print "*** testing dtscp help ***"
   print
   blech ${dtscp_cmd} help
   blech ${dtscp_cmd} HEL
   print
   blech ${dtscp_cmd} help advertise
   blech ${dtscp_cmd} HEL ADV
   print
   blech ${dtscp_cmd} help change
   blech ${dtscp_cmd} HEL CHA
   print
   blech ${dtscp_cmd} help create
   blech ${dtscp_cmd} HEL CRE
   print
   blech ${dtscp_cmd} help delete
   blech ${dtscp_cmd} HEL DEL
   print
   blech ${dtscp_cmd} help disable
   blech ${dtscp_cmd} HEL DIS
   print
   blech ${dtscp_cmd} help enable
   blech ${dtscp_cmd} HEL ENA
   print
   blech ${dtscp_cmd} help set
   blech ${dtscp_cmd} HEL SET
   print
   blech ${dtscp_cmd} help show
   blech ${dtscp_cmd} HEL SHO
   print
   blech ${dtscp_cmd} help synchronize
   blech ${dtscp_cmd} HEL SYN
   print
   blech ${dtscp_cmd} help unadvertise
   blech ${dtscp_cmd} HEL UNA
   print
   blech ${dtscp_cmd} help update
   blech ${dtscp_cmd} HEL UPD
   print
}

#
# test the dtscp advertise command
#
test_advertise()
{
   print "*** testing dtscp advertise ***"
   print
   blech ${dtscp_cmd} show type
   blech ${dtscp_cmd} advertise
   blech ${dtscp_cmd} show type
   print "*** the next command should fail with message 'Type already registered' ***"
   blech ${dtscp_cmd} ADV
   blech ${dtscp_cmd} unadvertise
   blech ${dtscp_cmd} show type
   print
}

#
# test the dtscp change command
#
test_change()
{
   print "*** testing dtscp change ***"
   print
   blech ${dtscp_cmd} show epoch number
   blech ${dtscp_cmd} change epoch 7
   blech ${dtscp_cmd} show epoch number
   blech ${dtscp_cmd} CHA EPO 0
   blech ${dtscp_cmd} show epoch number
   print "*** the next command should fail with message 'Epoch out of range' ***"
   blech ${dtscp_cmd} change epoch 777
   blech ${dtscp_cmd} show epoch number
   print
}

#
# test the dtscp create command
#
test_create()
{
   print "*** testing dtscp create ***"
   print "*** the next 8 commands should fail with message 'Time service already created' ***"
   print
   blech ${dtscp_cmd} create
   blech ${dtscp_cmd} CRE
   blech ${dtscp_cmd} create type
   blech ${dtscp_cmd} CRE TYP
   blech ${dtscp_cmd} create type clerk
   blech ${dtscp_cmd} CRE TYP CLE
   blech ${dtscp_cmd} create type server
   blech ${dtscp_cmd} CRE TYP SER
   print
}

#
# test the dtscp delete command
#
test_delete()
{
   print "*** testing dtscp delete ***"
   print "*** the next 2 commands should fail with message 'Time service in wrong state' ***"
   print
   blech ${dtscp_cmd} delete
   blech ${dtscp_cmd} DEL
   print
}

#
# test the dtscp disable command
#
test_disable()
{
   print "*** testing dtscp disable ***"
   print
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show disable directives completed
   blech ${dtscp_cmd} disable
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show disable directives completed
   blech ${dtscp_cmd} enable
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} DIS
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show disable directives completed
   print "*** the next command should fail with message 'Time service in wrong state' ***"
   blech ${dtscp_cmd} disable
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} enable
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   sleep $SLEEP_TIME
   print
}

#
# test the dtscp enable command
#
test_enable()
{
   print "*** testing dtscp enable ***"
   print
   blech ${dtscp_cmd} disable
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} enable
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show enable directives completed
   blech ${dtscp_cmd} disable
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} ENA
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show enable directives completed
   print "*** the next command should fail with message 'Time service in wrong state' ***"
   blech ${dtscp_cmd} enable
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show enable directives completed
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} disable
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} enable set clock
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show enable directives completed
   blech ${dtscp_cmd} disable
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} ENA SET CLO
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show enable directives completed
   print "*** the next command should fail with message 'Time service in wrong state' ***"
   blech ${dtscp_cmd} enable set clock
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show enable directives completed
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} disable
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} enable set clock false
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show enable directives completed
   blech ${dtscp_cmd} disable
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} ENA SET CLO FAL
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show enable directives completed
   print "*** the next command should fail with message 'Time service in wrong state' ***"
   blech ${dtscp_cmd} enable set clock false
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show enable directives completed
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} disable
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} enable set clock true
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show enable directives completed
   blech ${dtscp_cmd} disable
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} ENA SET CLO TRU
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show enable directives completed
   print "*** the next command should fail with message 'Time service in wrong state' ***"
   blech ${dtscp_cmd} enable set clock true
   sleep $SLEEP_TIME
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} show enable directives completed
   sleep $SLEEP_TIME
   print
}

#
# test the dtscp exit command
#
test_exit()
{
   print "*** testing dtscp exit ***"
   print
   blech ${dtscp_cmd} exit
   blech ${dtscp_cmd} EXI
   print
}

#
# test the dtscp quit command
#
test_quit()
{
   print "*** testing dtscp quit ***"
   print
   blech ${dtscp_cmd} quit
   blech ${dtscp_cmd} QUI
   print
}

#
# test the dtscp set commands
#
test_set()
{
   print "*** testing dtscp set ***"
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show check interval
      blech ${dtscp_cmd} set check interval 0-00:30:00.00I0.00
      blech ${dtscp_cmd} show check interval
      blech ${dtscp_cmd} SET CHE INT 0-02:00:00.00I0.00
      blech ${dtscp_cmd} show check interval
      blech ${dtscp_cmd} set check interval
      blech ${dtscp_cmd} show check interval
      print
      blech ${dtscp_cmd} set courier role backup courier
      blech ${dtscp_cmd} show courier role
      blech ${dtscp_cmd} set courier role courier
      blech ${dtscp_cmd} show courier role
      blech ${dtscp_cmd} SET COU ROLE noncourier
      blech ${dtscp_cmd} show courier role
      blech ${dtscp_cmd} set courier role
      blech ${dtscp_cmd} show courier role
      print
   fi
   blech ${dtscp_cmd} show error tolerance
   blech ${dtscp_cmd} set error tolerance 0-00:30:00.00I0.00
   blech ${dtscp_cmd} show error tolerance
   blech ${dtscp_cmd} SET ERR TOL 0-00:20:00.00I0.00
   blech ${dtscp_cmd} show error tolerance
   blech ${dtscp_cmd} set error tolerance
   blech ${dtscp_cmd} show error tolerance
   print
   blech ${dtscp_cmd} show global set timeout
   blech ${dtscp_cmd} set global set timeout 0-00:00:30.000I0.00
   blech ${dtscp_cmd} show global set timeout
   blech ${dtscp_cmd} SET GLO SET TIM 0-00:00:20.000I0.00
   blech ${dtscp_cmd} show global set timeout
   blech ${dtscp_cmd} set global set timeout
   blech ${dtscp_cmd} show global set timeout
   print
   blech ${dtscp_cmd} show local set timeout
   blech ${dtscp_cmd} set local set timeout 0-00:00:30.000I0.00
   blech ${dtscp_cmd} show local set timeout
   blech ${dtscp_cmd} SET LOC SET TIM 0-00:00:20.000I0.00
   blech ${dtscp_cmd} show local set timeout
   blech ${dtscp_cmd} set local set timeout
   blech ${dtscp_cmd} show local set timeout
   print
   blech ${dtscp_cmd} show maximum inaccuracy
   blech ${dtscp_cmd} set maximum inaccuracy 0-00:30:00.00I0.00
   blech ${dtscp_cmd} show maximum inaccuracy
   blech ${dtscp_cmd} SET MAX INA 0-00:00:30.00I0.00
   blech ${dtscp_cmd} show maximum inaccuracy
   blech ${dtscp_cmd} set maximum inaccuracy
   blech ${dtscp_cmd} show maximum inaccuracy
   print
   blech ${dtscp_cmd} show query attempts
   blech ${dtscp_cmd} set query attempts 4
   blech ${dtscp_cmd} show query attempts
   blech ${dtscp_cmd} SET QUE ATT 2
   blech ${dtscp_cmd} show query attempts
   blech ${dtscp_cmd} set query attempts
   blech ${dtscp_cmd} show query attempts
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show server entry name
      print "*** (KNOWN BUG) the next command will fail with message 'No permission for name service operation' ***"
      blech ${dtscp_cmd} set server entry name yourmomma
      blech ${dtscp_cmd} show server entry name
      print "*** the next command will fail with message 'Entry not found' ***"
      blech ${dtscp_cmd} SET SER ENT NAM hosts/yourmomma/dts-entity
      blech ${dtscp_cmd} show server entry name
      print "*** (KNOWN BUG) the next command will fail with message 'No permission for name service operation' ***"
      blech ${dtscp_cmd} set server entry name
      blech ${dtscp_cmd} show server entry name
      blech ${dtscp_cmd} set server entry name hosts/$HOSTNAME/dts-entity
      blech ${dtscp_cmd} show server entry name
      print
      blech ${dtscp_cmd} show server group name
      blech ${dtscp_cmd} set server group name subsys/dce/dts-gumby
      blech ${dtscp_cmd} show server group name
      blech ${dtscp_cmd} SET SER GRO NAM subsys/dce/dts-pokey
      blech ${dtscp_cmd} show server group name
      blech ${dtscp_cmd} set server group name
      blech ${dtscp_cmd} show server group name
      print
      blech ${dtscp_cmd} show server principal name
      print "*** (KNOWN BUG) the next command will fail with message 'Attempt to modify a read-only attribute' ***"
      blech ${dtscp_cmd} set server principal name hosts/$HOSTNAME/gumby
      blech ${dtscp_cmd} show server principal name
      print "*** (KNOWN BUG) the next command will fail with message 'Attempt to modify a read-only attribute' ***"
      blech ${dtscp_cmd} SET SER PRI NAM hosts/$HOSTNAME/pokey
      blech ${dtscp_cmd} show server principal name
      print "*** (KNOWN BUG) the next command will fail with message 'Attempt to modify a read-only attribute' ***"
      blech ${dtscp_cmd} set server principal name
      blech ${dtscp_cmd} show server principal name
      print
   fi
   blech ${dtscp_cmd} show servers required
   blech ${dtscp_cmd} set servers required 4
   blech ${dtscp_cmd} show servers required
   blech ${dtscp_cmd} SET SER REQ 2
   blech ${dtscp_cmd} show servers required
   blech ${dtscp_cmd} set servers required
   blech ${dtscp_cmd} show servers required
   print
   blech ${dtscp_cmd} show synchronization hold down
   blech ${dtscp_cmd} set synchronization hold down 0-00:15:00.00I0.00
   blech ${dtscp_cmd} show synchronization hold down
   blech ${dtscp_cmd} SET SYN HOL DOW 0-00:10:00.00I0.00
   blech ${dtscp_cmd} show synchronization hold down
   blech ${dtscp_cmd} set synchronization hold down
   blech ${dtscp_cmd} show synchronization hold down
   print
   blech ${dtscp_cmd} show arithmetic trace
   blech ${dtscp_cmd} set arithmetic trace true
   blech ${dtscp_cmd} show arithmetic trace
   blech ${dtscp_cmd} SET ARI TRA false
   blech ${dtscp_cmd} show arithmetic trace
   blech ${dtscp_cmd} set arithmetic trace
   blech ${dtscp_cmd} show arithmetic trace
   print
   blech ${dtscp_cmd} show communication trace
   blech ${dtscp_cmd} set communication trace true
   blech ${dtscp_cmd} show communication trace
   blech ${dtscp_cmd} SET COM TRA false
   blech ${dtscp_cmd} show communication trace
   blech ${dtscp_cmd} set communication trace
   blech ${dtscp_cmd} show communication trace
   print
   blech ${dtscp_cmd} show service trace
   blech ${dtscp_cmd} set service trace true
   blech ${dtscp_cmd} show service trace
   print "*** (KNOWN BUG) the next command will fail with the message 'Illegal dts command' ***"
   blech ${dtscp_cmd} SET SER TRA false
   blech ${dtscp_cmd} show service trace
   blech ${dtscp_cmd} set service trace
   blech ${dtscp_cmd} show service trace
   print
   blech ${dtscp_cmd} show synchronization trace
   blech ${dtscp_cmd} set synchronization trace true
   blech ${dtscp_cmd} show synchronization trace
   blech ${dtscp_cmd} SET SYN TRA false
   blech ${dtscp_cmd} show synchronization trace
   blech ${dtscp_cmd} set synchronization trace
   blech ${dtscp_cmd} show synchronization trace
   print
}

#
# test the dtscp show commands (characteristics)
#
test_show_characteristics()
{
   print "*** testing dtscp show (characteristics) ***"
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show acting courier role
      blech ${dtscp_cmd} SHO ACT COU ROL
      print
   fi
   blech ${dtscp_cmd} show automatic tdf change
   blech ${dtscp_cmd} SHO AUT TDF CHA
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show check interval
      blech ${dtscp_cmd} SHO CHE INT
      print
   fi
   blech ${dtscp_cmd} show clock adjustment rate
   blech ${dtscp_cmd} SHO CLO ADJ RAT
   print
   blech ${dtscp_cmd} show clock resolution
   blech ${dtscp_cmd} SHO CLO RES
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show courier role
      blech ${dtscp_cmd} SHO COU ROL
      print
   fi
   blech ${dtscp_cmd} show dts version
   blech ${dtscp_cmd} SHO DTS VER
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show epoch number
      blech ${dtscp_cmd} SHO EPO NUM
      print
   fi
   blech ${dtscp_cmd} show error tolerance
   blech ${dtscp_cmd} SHO ERR TOL
   print
   blech ${dtscp_cmd} show global set timeout
   blech ${dtscp_cmd} SHO GLO SET TIM
   print
   blech ${dtscp_cmd} show local set timeout
   blech ${dtscp_cmd} SHO LOC SET TIM
   print
   blech ${dtscp_cmd} show local time differential factor
   blech ${dtscp_cmd} SHO LOC TIM DIF FAC
   print
   blech ${dtscp_cmd} show maximum clock drift rate
   blech ${dtscp_cmd} SHO MAX CLO DRI RAT
   print
   blech ${dtscp_cmd} show maximum inaccuracy
   blech ${dtscp_cmd} SHO MAX INA
   print
   blech ${dtscp_cmd} show next tdf change
   blech ${dtscp_cmd} SHO NEX TDF CHA
   print
   blech ${dtscp_cmd} show query attempts
   blech ${dtscp_cmd} SHO QUE ATT
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show server entry name
      blech ${dtscp_cmd} SHO SER ENT NAM
      print
      blech ${dtscp_cmd} show server group name
      blech ${dtscp_cmd} SHO SER GRO NAM
      print
      blech ${dtscp_cmd} show server principal name
      blech ${dtscp_cmd} SHO SER PRI NAM
      print
   fi
   blech ${dtscp_cmd} show servers required
   blech ${dtscp_cmd} SHO SER REQ
   print
   blech ${dtscp_cmd} show synchronization hold down
   blech ${dtscp_cmd} SHO SYN HOL DOW
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show time provider present
      blech ${dtscp_cmd} SHO TIM PRO PRE
      print
   fi
   blech ${dtscp_cmd} show time representation version
   blech ${dtscp_cmd} SHO TIM REP VER
   print
   blech ${dtscp_cmd} show type
   blech ${dtscp_cmd} SHO TYP
   print
   blech ${dtscp_cmd} show arithmetic trace
   blech ${dtscp_cmd} SHO ARI TRA
   print
   blech ${dtscp_cmd} show communication trace
   blech ${dtscp_cmd} SHO COM TRA
   print
   blech ${dtscp_cmd} show service trace
   print "*** (KNOWN BUG) the next command will fail with the message 'Illegal dts command' ***"
   blech ${dtscp_cmd} SHO SER TRA
   print
   blech ${dtscp_cmd} show synchronization trace
   blech ${dtscp_cmd} SHO SYN TRA
   print
}

#
# test the dtscp show commands (counters)
#
test_show_counters()
{
   print "*** testing dtscp show (counters) ***"
   print
   blech ${dtscp_cmd} show clock settings
   blech ${dtscp_cmd} SHO CLO SET
   print
   #blech ${dtscp_cmd} show creation time
   #blech ${dtscp_cmd} SHO CRE TIM
   #print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show different epochs detected
      blech ${dtscp_cmd} SHO DIF EPO DET
      print
   fi
   blech ${dtscp_cmd} show disable directives completed
   blech ${dtscp_cmd} SHO DIS DIR COM
   print
   blech ${dtscp_cmd} show enable directives completed
   blech ${dtscp_cmd} SHO ENA DIR COM
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show epoch changes completed
      blech ${dtscp_cmd} SHO EPO CHA COM
      print
   fi
   blech ${dtscp_cmd} show insufficient resources detected
   blech ${dtscp_cmd} SHO INS RES DET
   print
   blech ${dtscp_cmd} show local times not intersecting
   blech ${dtscp_cmd} SHO LOC TIM NOT INT
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show no global servers detected
      blech ${dtscp_cmd} SHO NO GLO SER DET
      print
      blech ${dtscp_cmd} show server times not intersecting
      blech ${dtscp_cmd} SHO SER TIM NOT INT
      print
      blech ${dtscp_cmd} show servers not responding
      blech ${dtscp_cmd} SHO SER NOT RES
      print
      blech ${dtscp_cmd} show synchronizations completed
      blech ${dtscp_cmd} SHO SYN COM
      print
   fi
   blech ${dtscp_cmd} show system errors detected
   blech ${dtscp_cmd} SHO SYS ERR DET
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show time provider failures detected
      blech ${dtscp_cmd} SHO TIM PRO FAI DET
      print
   fi
   blech ${dtscp_cmd} show time representation mismatches detected
   blech ${dtscp_cmd} SHO TIM REP MIS DET
   print
   #blech ${dtscp_cmd} show too few servers detected
   #blech ${dtscp_cmd} SHO TOO FEW SER DET
   #print
   blech ${dtscp_cmd} show time provider timeouts detected
   blech ${dtscp_cmd} SHO TIM PRO TIM DET
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show updates initiated
      blech ${dtscp_cmd} SHO UPD INI
      print
      blech ${dtscp_cmd} show local server not in group
      blech ${dtscp_cmd} SHO LOC SER NOT IN GRO
      print
   fi
   blech ${dtscp_cmd} show servers not in group
   blech ${dtscp_cmd} SHO SER NOT IN GRO
   #blech dtscp show too many servers detected
   #blech dtscp SHO TOO MAN SER DET
   print
}

#
# test the dtscp show commands (status)
#
test_show_status()
{
   print "*** testing dtscp show (status) ***"
   print
   #blech ${dtscp_cmd} show last synchronization
   #blech ${dtscp_cmd} SHO LAS SYN
   #print
   blech ${dtscp_cmd} show state
   blech ${dtscp_cmd} SHO STA
   print
   #blech ${dtscp_cmd} show uid
   #blech ${dtscp_cmd} SHO UID
   #print
}

#
# test the dtscp show commands (other)
#
test_show_other()
{
   print "*** testing dtscp show (other) ***"
   print
   #blech ${dtscp_cmd} show current time
   #blech ${dtscp_cmd} SHO CUR TIM
   #print
   blech ${dtscp_cmd} show global servers
   blech ${dtscp_cmd} SHO GLO SER
   print
   if [[ $1 = "server" ]]; then
      blech ${dtscp_cmd} show local servers
      blech ${dtscp_cmd} SHO LOC SER
      print
   fi
   blech ${dtscp_cmd} show all
   blech ${dtscp_cmd} SHO ALL
   print
   blech ${dtscp_cmd} show all characteristics
   blech ${dtscp_cmd} SHO ALL CHA
   print
   #blech ${dtscp_cmd} show all counters
   #blech ${dtscp_cmd} SHO ALL COU
   #print
   #blech ${dtscp_cmd} show all status
   #blech ${dtscp_cmd} SHO ALL STA
   #print
}

#
# test the dtscp synchronize command
#
test_synchronize()
{
   print "*** testing dtscp synchronize ***"
   print
   blech ${dtscp_cmd} show synchronizations completed
   blech ${dtscp_cmd} synchronize
   blech ${dtscp_cmd} show synchronizations completed
   blech ${dtscp_cmd} SYN
   blech ${dtscp_cmd} show synchronizations completed
   blech ${dtscp_cmd} synchronize set
   blech ${dtscp_cmd} show synchronizations completed
   blech ${dtscp_cmd} SYN SET
   blech ${dtscp_cmd} show synchronizations completed
   blech ${dtscp_cmd} synchronize set clock
   blech ${dtscp_cmd} show synchronizations completed
   blech ${dtscp_cmd} SYN SET CLO
   blech ${dtscp_cmd} show synchronizations completed
   blech ${dtscp_cmd} synchronize set clock false
   blech ${dtscp_cmd} show synchronizations completed
   blech ${dtscp_cmd} SYN SET CLO FAL
   blech ${dtscp_cmd} show synchronizations completed
   blech ${dtscp_cmd} synchronize set clock true
   blech ${dtscp_cmd} show synchronizations completed
   blech ${dtscp_cmd} SYN SET CLO TRU
   blech ${dtscp_cmd} show synchronizations completed
}

#
# test the dtscp unadvertise command
#
test_unadvertise()
{
   print "*** testing dtscp unadvertise ***"
   print
   blech ${dtscp_cmd} advertise
   blech ${dtscp_cmd} show type
   blech ${dtscp_cmd} unadvertise
   blech ${dtscp_cmd} show type
   blech ${dtscp_cmd} advertise
   blech ${dtscp_cmd} show type
   blech ${dtscp_cmd} UNA
   blech ${dtscp_cmd} show type
   print
}

#
# test an Illegal dtscp command
#
test_illegal()
{
   print "*** testing an Illegal dtscp command***"
   print
   print "*** the next command should fail with message 'Illegal dts command' ***"
   blech ${dtscp_cmd} biteme
   print
}

#
# main
#

HOSTNAME=`hostname`
dtscp_cmd="dtscp"
SLEEP_TIME=10

case $# in 
  2) HOSTNAME=$2 
     dtscp_cmd="dtscp -h $HOSTNAME "
     echo "host name is $HOSTNAME, dtscp command is $dtscp_cmd" >/dev/tty
     ;;
esac
export HOSTNAME 

test_help
if [[ $1 = "server" ]]; then
   test_advertise
   test_change
fi
test_create 
test_delete
test_disable
test_enable
test_exit
test_quit
test_set $1
test_show_characteristics $1
test_show_counters $1
test_show_status
test_show_other $1
if [[ $1 = "server" ]]; then
   test_synchronize
   test_unadvertise
fi
test_illegal
