# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: sec_dap_b4.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:34:41  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:03  marty]
#
# Revision 1.1.4.1  1995/12/11  15:25:17  root
# 	Submit
# 	[1995/12/11  14:35:57  root]
# 
# Revision 1.1.2.1  1994/07/06  15:10:45  marrek
# 	Created in July 1994 code drop.
# 	[1994/07/06  13:02:33  marrek]
# 
# $EndLog$
#
# $RCSfile: sec_dap_b4.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:41 $
#
#######################################################################
#    FILE : sec_dap_b4.tcl                                            #
#    TYPE : gdscp script                                              #
#    DESCRIPTION : test of remote DAP bind ,DCE + simple will succeed #
#######################################################################

set testid sec_dap_b4
puts "TEST : $testid"

# + perform an anomymous bind 
if {[catch {eval "bind -dirid 11 -dsa /c=re/O=sni/ou=buba/cn=dsa/cn=dsa2"} message] == 0 } {
	puts "EXPECTED : anonymous bind to DSA2 ok"
	} else {
	puts $message
	puts "UNEXPECTED : anonymous bind to DSA2 failed"
	}

# - try to read ACL protected Attribute
if {[catch {eval "show /c=re/O=sni/ou=ap2/cn=two -pretty -attribute UP"} message] == 0 } {
	puts "UNEXPECTED : show protected attr succeeded"
	} else {
	puts $message
	puts "EXPECTED : show protected attr failed"
	}

# + perform a simple bind 
if {[catch {eval "bind -dirid 11 -dsa /c=re/O=sni/ou=buba/cn=dsa/cn=dsa2 \
	-authentication SIMPLE\ -user /c=re/O=sni/ou=ap2/cn=two \
	-password secret2"} message] == 0 } {
	puts "EXPECTED : simple bind to DSA2 ok"
	} else {
	puts $message
	puts "UNEXPECTED : simple bind to DSA2 failed"
	}

# + try to read ACL protected Attribute
if {[catch {eval "show /c=re/O=sni/ou=ap2/cn=two -pretty -attribute UP"} message] == 0 } {
	puts "EXPECTED : show protected attr succeeded"
	} else {
	puts $message
	puts "UNEXPECTED : show protected attribute failed"
	}

# + try to bind using 
if {[catch {eval "bind -dirid 11 -dsa /c=re/O=sni/ou=buba/cn=dsa/cn=dsa2 -authentication DCE"} message] == 0 } {
	puts "EXPECTED : dce bind to DSA2 succeeded"
	} else {
	puts $message
	puts "UNEXPECTED : DCE bind to DSA2 failed"
	}

# + try to read ACL protected Attribute
if {[catch {eval "show /c=re/O=sni/ou=ap2/cn=two -pretty -attribute UP"} message] == 0 } {
	puts "EXPECTED : show protected attr succeeded"
	} else {
	puts $message
	puts "UNEXPECTED : show protected attribute failed"
	}

