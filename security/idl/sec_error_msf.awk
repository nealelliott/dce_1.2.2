#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# HISTORY
# $Log: sec_error_msf.awk,v $
# Revision 1.1.4.2  1996/03/09  23:29:13  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:44:37  marty]
#
# Revision 1.1.4.1  1995/12/08  17:37:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:56:07  root]
# 
# Revision 1.1.2.2  1992/12/29  13:54:10  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:22:18  zeliff]
# 
# Revision 1.1  1992/01/19  19:37:39  devrcs
# 	Initial revision
# 
# $EndLog$
#   sec_error_msf.awk V=1 08/28/91 //littl/prgy/src/idl
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
##      dce_error_msf.awk
##
##  FACILITY:
##
##      Remote Procedure Call
##
##  ABSTRACT:
##
##  Awk script to produce an X/Open message source file (MSF) from a
##  symbolic message source file (SMSF).  An SMSF is like an MSF except
##  that symbolic message identifiers are used instead of numbers.
##
##  Note that message numbers start at zero.  Message number zero is
##  special in that XPG message numbers must start at 1.  Thus, not
##  MSF output is generated for the first message in the SMSF.
##
##    
##

#
# The first line in rpcsts.msf should define the set of messages.  For
# this version, the value should be 0.
#
BEGIN { 
        print "$  *** secsts.msf ***"
        print "$  **** THIS FILE HAS BEEN AUTOMATICALLY GENERATED; DO NOT EDIT IT ****"
        print "$  "
        print "$set 1"

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
# For each status code (after the 0'th), print out the status_num, space,
# and the accompanying text
#
/^[A-Za-z]/ {
        if (status_num > 0) {
            printf("%d\t", status_num)
            printf("%s", $2)
            for (i = 3; i <= NF; i++)
            {   
                printf(" %s", $i) 
            }   
            printf("\n")
        }
        status_num++
        next
}
