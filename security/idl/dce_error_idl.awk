#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# HISTORY
# $Log: dce_error_idl.awk,v $
# Revision 1.1.4.2  1996/03/09  23:27:51  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:43:18  marty]
#
# Revision 1.1.4.1  1995/12/08  17:31:17  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:54:53  root]
# 
# Revision 1.1.2.2  1992/12/29  13:29:07  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:16:35  zeliff]
# 
# Revision 1.1  1992/01/19  19:37:39  devrcs
# 	Initial revision
# 
# $EndLog$
#   dce_error_idl.awk V=2 08/28/91 //littl/prgy/src/idl
##
##  Copyright (c) 1991 by
##      Hewlett-Packard Company, Palo Alto, Ca. & 
##      Digital Equipment Corporation, Maynard, Mass.
##
##   Copyright (c) Hewlett-Packard Company 1991
##   Unpublished work. All Rights Reserved.
##  
##
##  NAME:
##
##      dce_error_idl.awk
##
##  FACILITY:
##
##      Remote Procedure Call
##
##  ABSTRACT:
##
##  Awk script to produce a .idl from from a symbolic message source
##  file (SMSF).  An SMSF is like an XPG message source file (MSF) except
##  that symbolic message identifiers are used instead of numbers.  This
##  script emits "const" statements associating each symbol with its
##  "error status code", which is the logical "or" of the message number
##  and the appropriate DCE prefix.  (The prefix can be used to locate
##  the message catalog.)
##
##  Note that message numbers start at zero.  Message number zero is
##  special in that XPG message numbers must start at 1.  However, the
##  symbolic constant for message number zero can be used in masked tests
##  of status code (e.g., "(status & 0xfffff000) == symbol").
##
##    
##

#
# First print out the heading lines for the idl file.
#
BEGIN { 
        print "/**** secsts.idl ****/"
        print "/**** THIS FILE HAS BEEN AUTOMATICALLY GENERATED; DO NOT EDIT IT ****/"
        print " "
        print "interface secsts"
        print "{"
        print " "

#
# Facility code is "1" ("DCE")
#
        facility_code = 1

#
# Calculate the component code ("sec") encoded in radix-40
#
#   char1 = 18 = "s"
#   char2 = 4  = "e"
#   char3 = 2  = "c"
#
# Complete the encoding using the following formula:
#
#   component_code = (((((c1*40)+c2)*40)+c3))
#
        component_code = 28962

#        
# Calculate the facility and component parts of the status code. 
#
#   facility = facility_code << facility_shift 
#            = facility_code * 2^24
#            = facility_code * 0x1000000
#            = facility_code * 268435456
#
#   component = component_code << component_shift
#             = component_code * 2^12
#             = component_code * 4096
#
# prefix is the composition of the component and facility.
#
        facility  = facility_code * 268435456
        component = component_code * 4096
        prefix    = component + facility

        status_num = 0
}


#
# Strip out any blank lines
#
/^[  ]*$/ {
        next
}


#
# Skip any comments in the sec.msg file
# 
/^[#]/ {
        next
}


#
# Process the rest of the lines of the sec.msg file.  Note we print out
# the status code in this strange way to get around a problem with Apollo
# awk.
#
/^[A-Za-z]/ {
        tmp = prefix / 4096
        printf("const long %s = 0x%x%03x;\n", $1, tmp, status_num)
        status_num++
        next
}


# write out last line to the *.idl file
END { 
        print " "
        print "}"
}
