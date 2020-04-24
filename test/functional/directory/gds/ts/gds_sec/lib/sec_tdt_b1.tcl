# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: sec_tdt_b1.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:34:47  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:11  marty]
#
# Revision 1.1.4.1  1995/12/11  15:25:27  root
# 	Submit
# 	[1995/12/11  14:36:01  root]
# 
# Revision 1.1.2.1  1994/07/06  15:10:55  marrek
# 	Created in July 1994 code drop.
# 	[1994/07/06  13:02:40  marrek]
# 
# $EndLog$
#
# $RCSfile: sec_tdt_b1.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:47 $
#
#######################################################################
#    FILE : sec_tdt_b1.tcl                                            #
#    TYPE : gdscp script                                              #
#    DESCRIPTION : used for TDT  tests                                #
#######################################################################

puts "TEST : sec_tdt_b1"

# DCE bind to DSA1 - expected result SUCCESS
if {[catch {eval "bind -dirid 11 -dsa /C=re/O=sni/OU=buba/CN=dsa/CN=dsa1 -authentication DCE"} message] == 0 } {
	puts "EXPECTED : DCE bind succeeded" 
	} else {
	puts $message
	puts "UNEXPECTED : DCE bind failed"
	}

# set service control preferchain TRUE
if {[catch {eval "x500svc modify -preferchain TRUE"} message] == 0 } {
	puts "EXPECTED : preferchain set TRUE ok"
	} else {
	puts $message
	puts "UNEXPECTED : preferchain set TRUE failed"
	}

# set service control chainingprohibited FALSE
if {[catch {eval "x500svc modify -chainingprohibited FALSE"} message] == 0 } {
	puts "EXPECTED : chainingprohibited set FALSE ok"
	} else {
	puts $message
	puts "UNEXPECTED : chainingprohibited set FALSE failed"
	}

# read UP of C=re/O=sni/OU=ap2/CN=two 
if {[catch {eval "show /C=re/O=sni/OU=ap2/CN=two -pretty -attribute UP"} message] == 0 } {
	puts "EXPECTED: show protected attr succeeded" 
	} else {
	puts $message
	puts "UNEXPECTED : show protected attr failed"
	}

# modify UP of C=re/O=sni/OU=ap2/CN=two 
if {[catch {eval "modify /C=re/O=sni/OU=ap2/CN=two -changeattr UP=secret2 UP=secret"} message] == 0 } {
	puts "EXPECTED: Modify protected attr succeeded" 
	} else {
	puts $message
	puts "UNEXPECTED : Modify protected attr failed"
	}

# read UP of C=re/O=sni/OU=ap2/CN=two 
if {[catch {eval "show /C=re/O=sni/OU=ap2/CN=two -pretty -attribute UP"} message] == 0 } {
	puts "EXPECTED: show protected attr succeeded" 
	} else {
	puts $message
	puts "UNEXPECTED : show protected attr failed"
	}

# modify UP of C=re/O=sni/OU=ap2/CN=two 
if {[catch {eval "modify /C=re/O=sni/OU=ap2/CN=two -changeattr UP=secret UP=secret2"} message] == 0 } {
	puts "EXPECTED: Modify protected attr succeeded" 
	} else {
	puts $message
	puts "UNEXPECTED : Modify protected attr failed"
	}
