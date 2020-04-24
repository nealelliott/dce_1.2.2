# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: sec_dsp_b2.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:34:44  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:06  marty]
#
# Revision 1.1.4.1  1995/12/11  15:25:23  root
# 	Submit
# 	[1995/12/11  14:35:58  root]
# 
# Revision 1.1.2.1  1994/07/06  15:10:50  marrek
# 	Created in July 1994 code drop.
# 	[1994/07/06  13:02:36  marrek]
# 
# $EndLog$
#
# $RCSfile: sec_dsp_b2.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:44 $
#
#######################################################################
#    FILE : sec_dsp_b1.tcl                                            #
#    TYPE : gdscp script                                              #
#    DESCRIPTION : used for DSP bind tests                            #
#######################################################################

set testid sec_dsp_b2
puts "TEST : $testid"

# (1) perform an anomymous bind - expected result SUCCESS
if {[catch {eval "bind -dirid 11 -dsa /C=re/O=sni/OU=buba/CN=dsa/CN=dsa1"} message] == 0 } {
	puts "EXPECTED : anonymous bind ok"
	} else {
	puts $message
	puts "UNEXPECTED : anonymous bind failed"
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

# (2) read ACL protected attribute from DSA2 - expected result FAILURE
if {[catch {eval "show /C=re/O=sni/OU=ap2/CN=two -pretty -attribute UP"} message] == 0 } {
	puts "UNEXPECTED : show protected attr succeeded"
	} else {
	puts $message
	puts "EXPECTED : show protected attr failed"
	}

# (3) perform simple bind (DN=ONE) to DSA1 - expected result SUCCESS
if {[catch {eval "bind -dirid 11 -dsa /C=re/O=sni/OU=buba/CN=dsa/CN=dsa1 \
	-authentication SIMPLE\ -user /C=re/O=sni/OU=buba/CN=one \
	-password secret1"} ] == 0 } {
	puts "EXPECTED : simple bind ok"
	} else {
	puts "UNEXPECTED : simple bind failed"
	}

# (4) read ACL protected attribute from DSA2 - expected result SUCCESS
if {[catch {eval "show /C=re/O=sni/OU=ap2/CN=two -pretty -attribute UP"} message] == 0 } {
	puts "EXPECTED: show protected attr succeeded"
	} else {
	puts $message
	puts "UNEXPECTED : show protected attr failed"
	}

# (5) perform DCE bind (DN=ONE) to DSA1 - expected result SUCCESS
if {[catch {eval "bind -dirid 11 -dsa /C=re/O=sni/OU=buba/CN=dsa/CN=dsa1 -authentication DCE"} message] == 0 } {
	puts "EXPECTED : dce bind succeeded"
	} else {
	puts $message
	puts "UNEXPECTED : DCE bind failed"
	}

#(6) read ACL protected attribute from DSA2 - expected result SUCCESS
if {[catch {eval "show /C=re/O=sni/OU=ap2/CN=two -pretty -attribute TN"} message] == 0 } {
	puts "EXPECTED: show protected attr succeeded"
	} else {
	puts $message
	puts "UNEXPECTED : show protected attr failed"
	}
