#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: cdsval.awk,v $
# Revision 1.1.8.2  1996/03/09  23:11:56  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:14  marty]
#
# Revision 1.1.8.1  1995/12/08  15:15:03  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:42:34  root]
# 
# Revision 1.1.6.2  1994/06/09  18:38:43  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:11:14  devsrc]
# 
# Revision 1.1.6.1  1994/03/12  22:02:27  peckham
# 	DEC serviceability and i18n drop
# 	[1994/03/12  14:12:30  peckham]
# 
# Revision 1.1.4.2  1992/12/30  13:28:42  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  22:46:20  zeliff]
# 
# Revision 1.1.2.3  1992/04/24  19:18:05  mfox
# 	The last rule reused temporary array s[] in a dangerous way.
# 	[1992/04/22  13:17:35  mfox]
# 
# Revision 1.1.2.2  1992/03/22  22:01:14  weisman
# 		Big merge with post-1.0 development at Digital.
# 	[1992/03/22  20:25:32  weisman]
# 
# Revision 1.1  1992/01/19  19:37:36  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
# Module: cdsval.awk
#
#
# Function:
#
#  Produce an include file from dnsclerk.h, defining an array containing
#  the DNS Value Type string definitions. It is used by dnsCvtObjID.c to
#  return the Value Type byte values.
#
# Command:
#
#  awk -f cdsval.awk cdsclerk.h > cds_attr_values.c
#
# MODIFICATIONS:
#
# Gen 3, 31-Jan-1992, Alan peckham
#	The original version recognized the start of the pertinent data
#	but did not find the end, resulting in a bigger table with other
#	garbage in it.
#
#	Rewrite for generic enum recoginition.
#	Also recognize value assignment.
#	Fill in holes with "?".
#

BEGIN	{ tabs = "		" }
#
# Strip out blank lines.
#
/^[ 	]*$/ { next }
#
# Strip out all other control lines.
#
/^[ 	]*\./ { next }
#
# Find enums.
#
/^[ 	]*typedef[ 	]*enum/ {
		enum_found = 1
		count = 0
		nextval = 0
		next
		}
#
# Toss line if we haven't found enum yet.
#
(enum_found == 0) { next }
#
# If this enum is what we have been waiting for,
# then process the lines and get out.
#
/[ 	]*}[ 	]*cds_syntax_t/ { 
		for ( i = 1; i <= count; i++) {
		    val = value[i]
		    if (maxval < val) maxval = val
		    name[val] = symbol[i]
		}
		print "/*"
		print " * This file is for exclusive use of dnscvtobjid.c"
		print " */"
		print "static char *cds_val[] = {"
		for ( i = 0; i <= maxval; i++) {
		    if (name[i] == "") name[i] = "?"
		    print tabs "\"" name[i] "\","
		}
		print tabs "\"\" };"
		exit
		}
#
# Termination of unwanted enum - throw away.
#
/[ 	]*}.*;/ { enum_found = 0; next }
#
# Accumulate lines in enum for later processing.
# The file better be formatted correctly!
#
/[ 	]*cds_.*=/ { split($3,s,",")
		nextval = s[1] }

/[ 	]*cds_/ { split($1,s,",")
		split(s[1],t,"_")
		count++
		symbol[count] = t[2]
		value[count] = nextval++
		next }
