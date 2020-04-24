# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: attr_eval.tcl,v $
# Revision 1.1.6.2  1996/03/09  23:06:24  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:25:17  marty]
#
# Revision 1.1.6.1  1995/12/08  21:23:34  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:57:32  root]
# 
# Revision 1.1.4.9  1994/06/09  16:02:31  devsrc
# 	cr10892 - fix copyright
# 	[1994/06/09  15:48:07  devsrc]
# 
# Revision 1.1.4.8  1994/05/10  10:00:19  kevins
# 	OT 9691 Add registry show/modify commands
# 	[1994/05/09  19:17:51  kevins]
# 
# Revision 1.1.4.7  1994/04/19  16:27:06  kevins
# 	CR 10381 added forwardabletkt and pwdvalid as account attributes
# 	[1994/04/19  10:50:30  kevins]
# 
# Revision 1.1.4.6  1994/04/08  10:40:19  kevins
# 	OT 9691 Implemented account commands
# 	[1994/04/08  10:39:33  kevins]
# 
# Revision 1.1.4.5  1994/03/22  16:14:21  kevins
# 	CR 10198 added orgid info
# 	[1994/03/22  16:14:00  kevins]
# 
# Revision 1.1.4.4  1994/02/03  18:42:46  kevins
# 	added info for cds attributes
# 	[1994/02/03  18:28:30  kevins]
# 
# Revision 1.1.4.3  1994/02/03  17:26:48  kevins
# 	added support for group and org objects
# 	[1994/02/03  17:18:08  kevins]
# 
# Revision 1.1.4.2  1994/01/05  16:30:35  kevins
# 	Attribute information is now read from cds_attributes file
# 	[1994/01/05  15:54:06  kevins]
# 
# Revision 1.1.4.1  1993/12/29  15:59:01  kevins
# 	Attribute name and oid information is now read from the
# 	/opt/dcelocal/etc/cds_attributes file.
# 	[1993/12/29  15:43:53  kevins]
# 
# Revision 1.1.2.2  1993/11/05  19:52:57  kevins
# 	added some attribute variables
# 	[1993/11/05  19:43:43  kevins]
# 
# Revision 1.1.2.1  1993/10/22  15:00:30  kevins
# 	updated to new source dir structure
# 	[1993/10/22  15:00:14  kevins]
# 
# Revision 1.1.2.4  1993/10/13  19:01:48  kevins
# 	Added attribute variable values.
# 	[1993/10/13  18:57:06  kevins]
# 
# Revision 1.1.2.3  1993/08/23  19:09:23  kevins
# 	Make attr_name command more robust.
# 	[1993/08/23  19:08:21  kevins]
# 
# Revision 1.1.2.2  1993/08/05  19:49:49  kevins
# 	Initial submission for dsh
# 	[1993/08/05  19:49:21  kevins]
# 
# $EndLog$


proc attr_name {name} {
  set attr [info globals $name*]
  if {[llength $attr] == 0} {
    error "unknown attribute \"$name\""
    return $attr
  } elseif {[llength $attr] == 1} {
    return $attr
  } else {
    foreach entry $attr {
     if {[string compare $entry $name] == 0} {
	return $entry
     }
    }
    error "Ambiguous attribute name \"$name\" could be: $attr"
  } 
}

set low 1
set medium 2
set high 3

set expiration 1
set extension 0

