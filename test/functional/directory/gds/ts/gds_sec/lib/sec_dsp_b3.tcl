# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: sec_dsp_b3.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:34:46  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:08  marty]
#
# Revision 1.1.4.1  1995/12/11  15:25:25  root
# 	Submit
# 	[1995/12/11  14:36:00  root]
# 
# Revision 1.1.2.1  1994/07/06  15:10:53  marrek
# 	Created in July 1994 code drop.
# 	[1994/07/06  13:02:38  marrek]
# 
# $EndLog$
#
# $RCSfile: sec_dsp_b3.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:46 $
#
#######################################################################
#    FILE : sec_dsp_b3.tcl                                            #
#    TYPE : gdscp script                                              #
#    DESCRIPTION : used for DSP bind tests                            #
#######################################################################

set testid sec_dsp_b3
puts "TEST : $testid"

# (1) perform an anomymous bind - expected result SUCCESS
if {[catch {eval "bind -dirid 12 -dsa /C=re/O=sni/OU=buba/CN=dsa/CN=dsa2"} message] == 0 } {
	puts "EXPECTED : anonymous bind to DSA2 ok"
	} else {
	puts $message
	puts "UNEXPECTED : anonymous bind DSA2 failed"
	}

# set service control preferadmfunctions TRUE
if {[catch {eval "x500svc modify -preferadmfunctions TRUE"} message] == 0 } {
	puts "EXPECTED : preferadmfunctions set TRUE ok"
	} else {
	puts $message
	puts "UNEXPECTED : preferadmfunctions set TRUE failed"
	}

# delete SIMPLE value for the AM recurring attribute
if {[catch {eval "modify /c=re/O=sni/ou=buba/cn=dsa/cn=dsa2 -removeattr AM=2"} message] == 0 } { 
	puts "EXPECTED : SIMPLE AM removed from the dsa2 object"
	} else {
	puts $message
	puts "UNEXPECTED : SIMPLE AM not removed from the dsa2 object"
	}

# (1) perform an anomymous bind - expected result SUCCESS
if {[catch {eval "bind -dirid 11 -dsa /C=re/O=sni/OU=buba/CN=dsa/CN=dsa1"} message] == 0 } {
	puts "EXPECTED : anonymous bind ok"
	} else {
	puts $message
	puts "UNEXPECTED : anonymous bind failed"
	}

# set service control preferadmfunctions FALSE
if {[catch {eval "x500svc modify -preferadmfunctions FALSE"} message] == 0 } {
	puts "EXPECTED : preferadmfunctions set FALSE ok"
	} else {
	puts $message
	puts "UNEXPECTED : preferadmfunctions set FALSE failed"
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
	puts "UNEXPECTED: show protected attr succeeded"
	} else {
	puts $message
	puts "EXPECTED : show protected attr failed"
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

# (1) perform an anomymous bind - expected result SUCCESS
if {[catch {eval "bind -dirid 12 -dsa /C=re/O=sni/OU=buba/CN=dsa/CN=dsa2"} message] == 0 } {
	puts "EXPECTED : anonymous bind DSA2 ok"
	} else {
	puts $message
	puts "UNEXPECTED : anonymous bind DSA2 failed"
	}

# set service control chainingprohibited FALSE
if {[catch {eval "x500svc modify -default"} message] == 0 } {
	puts "EXPECTED : default service controls set ok."
	} else {
	puts $message
	puts "UNEXPECTED : default service controls set ok."
	}

# set service control preferadmfunctions TRUE
if {[catch {eval "x500svc modify -preferadmfunctions TRUE"} message] == 0 } {
	puts "EXPECTED : preferadmfunctions set TRUE ok"
	} else {
	puts $message
	puts "UNEXPECTED : preferadmfunctions set TRUE failed"
	}

# add SIMPLE value for the AM recurring attribute
if {[catch {eval "modify /c=re/O=sni/ou=buba/cn=dsa/cn=dsa2 -addattr AM=2"} message] == 0 } { 
	puts "EXPECTED : SIMPLE AM added in the dsa2 object"
	} else {
	puts $message
	puts "UNEXPECTED : SIMPLE AM not added in the dsa2 object"
	}

