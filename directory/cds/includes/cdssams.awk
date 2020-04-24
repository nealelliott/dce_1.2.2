#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: cdssams.awk,v $
# Revision 1.1.4.2  1996/03/09  23:11:55  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:13  marty]
#
# Revision 1.1.4.1  1995/12/08  15:15:00  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:42:32  root]
# 
# Revision 1.1.2.3  1994/06/09  18:38:42  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:11:13  devsrc]
# 
# Revision 1.1.2.2  1994/03/21  22:03:52  peckham
# 	The errcod variable was not being incremented for uninteresting codes.
# 	[1994/03/21  21:30:17  peckham]
# 
# Revision 1.1.2.1  1994/03/12  22:02:26  peckham
# 	DEC serviceability and i18n drop
# 	[1994/03/12  19:32:08  peckham]
# 
# $EndLog$
#
#
# COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
#
# THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
# ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
# INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
# COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
# OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
# TRANSFERRED.
#
# THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
# AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
# CORPORATION.
#
# DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
# SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
#
# Module: cdssams.awk
#
#
# Function:
#  Produce OSF idl file from cds.sams,
#  used for generating cdsmessage.h
#
BEGIN {
#
# Calculate the facility|component parts of the status code.
#
# facility_code = facility_code << facility_shift = facility_code * 2^28
# Because AWK on ULTRIX does not support the exponential operator (^),
# facility is calculated by manually.
#
        facility = 1 * 4096 * 65536
#
# Calculate the component code which is "cds" encoded in radix-40
# char1 = 2  = "c"
# char2 = 3  = "d"
# char3 = 18 = "s"
#
# Complete the encoding using the following formula:
#     component = (((c1*40)+c2)*40)+c3 = 3338
#
# Shift the component code left 12 bits i.e. component = component << 2^12
#
        component = 3338 * 4096
#
# Add the component and facility codes. The prefix should be
# facility | component | 000000000000
#
        prefix = component + facility
	errcod = 1
        hprefix = prefix / 4096
#
# Enable the following when we can handle it.
#
	  type_st = "dns_status_t"
#
	  if (type_st == "") type_st = "long"
	  print "/**** dnsmessage.idl ****/"
          print " "
          print "[local] interface cds"
          print "{"
	  if (type_st == "dns_status_t") {
	    print " "
	    print "const long _DNS_STATUS_T_ = 1;"
	    print "typedef unsigned long dns_status_t;"
	    print " "
	  }
#
# DCE_MESSAGE_FILE_VERSION is unique. Its value must always be the current
# version of the DCE message file. Currently that is 1.
#
          print "const long DCE_MESSAGE_FILE_VERSION = 1;"
	  print ""
        }
#
# Pick up the changes in next value to assign
#
/^set[ 	]+value/ {
	  errcod = $NF
	  next }
#
# Pick up code lines
#
/^code[ 	]+[^ 	]+[ 	]*=/ {
	  errcod = $4 }
/^code[ 	]+CDS_/ {
	  name = substr($2,4)
	  printf("const %s DNS%s = 0x%x%03x;\n",type_st,name,hprefix,errcod)
	  errcod++
	  next }
/^code[ 	]+/ {
	  errcod++
	  next }
#
# For the header file, figure where the OSF errors can go.
# Put in trailing header lines.
#
END	{
	  print ""
	  print "}"
	}
