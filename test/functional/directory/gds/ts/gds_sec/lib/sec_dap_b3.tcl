# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: sec_dap_b3.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:34:39  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:40:01  marty]
#
# Revision 1.1.4.1  1995/12/11  15:25:15  root
# 	Submit
# 	[1995/12/11  14:35:56  root]
# 
# Revision 1.1.2.1  1994/07/06  15:10:43  marrek
# 	Created in July 1994 code drop.
# 	[1994/07/06  13:02:32  marrek]
# 
# $EndLog$
#
# $RCSfile: sec_dap_b3.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:39 $
#
#######################################################################
#    FILE : sec_dap_b3.tcl                                            #
#    TYPE : gdscp script                                              #
#    DESCRIPTION : test of local DAP bind , DCE will succeed 	      #
#		   Simple will fail.	      			      #
#######################################################################

set testid sec_dap_b3
puts "TEST : $testid"

# + perform an anomymous bind 
if {[catch {eval "bind -dirid 11 -dsa /c=re/O=sni/ou=buba/cn=dsa/cn=dsa1"} message] == 0 } {
	puts "EXPECTED : anonymous bind ok"
	} else {
	puts $message
	puts "UNEXPECTED : anonymous bind failed"
	}

# - try to read ACL protected Attribute
if {[catch {eval "show /c=re/O=sni/ou=buba/cn=one -pretty -attribute UP"} message] == 0 } {
	puts "UNEXPECTED : show protected attr succeeded" 
	} else {
	puts $message
	puts "EXPECTED : show protected attr failed"
	}

# + perform a simple bind 
if {[catch {eval "bind -dirid 11 -dsa /c=re/O=sni/ou=buba/cn=dsa/cn=dsa1 \
	-authentication SIMPLE\ -user /c=re/O=sni/ou=buba/cn=one \
	-password secret1"} message] == 0 } {
	puts "UNEXPECTED : simple bind ok"
	} else {
	puts $message
	puts "EXPECTED : simple bind failed" 
	}

# + try to bind using DCE auth
if {[catch {eval "bind -dirid 11 -dsa /c=re/O=sni/ou=buba/cn=dsa/cn=dsa1 \
	-authentication DCE"} message] == 0 } {
	puts "EXPECTED : dce bind succeeded"
	} else {
	puts $message
	puts "UNEXPECTED : DCE bind failed"
	}

# + try to read ACL protected Attribute
if {[catch {eval "show /c=re/O=sni/ou=buba/cn=one -pretty -attribute UP"} message] == 0 } {
	puts "EXPECTED : show protected attr succeeded" 
	} else {
	puts $message
	puts "UNEXPECTED : show protected attribute failed"
	}

