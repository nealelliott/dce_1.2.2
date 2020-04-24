# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: parray.tcl,v $
# Revision 1.1.6.2  1996/03/09  23:09:18  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:27:26  marty]
#
# Revision 1.1.6.1  1995/12/08  21:32:18  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  18:03:41  root]
# 
# Revision 1.1.4.1  1994/06/09  16:03:58  devsrc
# 	cr10892 - fix copyright
# 	[1994/06/09  15:49:13  devsrc]
# 
# Revision 1.1.2.1  1993/12/12  01:37:12  rousseau
# 	Initial 7.3 version.
# 	[1993/12/12  01:37:05  rousseau]
# 
# $EndLog$
# parray:
# Print the contents of a global array on stdout.
#
# $Header: /u0/rcs_trees/dce/rcs/admin/tcl7.3/library/parray.tcl,v 1.1.6.2 1996/03/09 23:09:18 marty Exp $ SPRITE (Berkeley)
#
# Copyright (c) 1991-1993 The Regents of the University of California.
# All rights reserved.
#
# Permission is hereby granted, without written agreement and without
# license or royalty fees, to use, copy, modify, and distribute this
# software and its documentation for any purpose, provided that the
# above copyright notice and the following two paragraphs appear in
# all copies of this software.
#
# IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
# DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
# OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
# CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
# ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
# PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
#

proc parray a {
    upvar 1 $a array
    if [catch {array size array}] {
	error "\"$a\" isn't an array"
    }
    set maxl 0
    foreach name [lsort [array names array]] {
	if {[string length $name] > $maxl} {
	    set maxl [string length $name]
	}
    }
    set maxl [expr {$maxl + [string length $a] + 2}]
    foreach name [lsort [array names array]] {
	set nameString [format %s(%s) $a $name]
	puts stdout [format "%-*s = %s" $maxl $nameString $array($name)]
    }
}
