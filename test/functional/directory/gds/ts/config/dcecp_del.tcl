# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: dcecp_del.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:34:13  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:39:40  marty]
#
# Revision 1.1.4.1  1995/12/11  15:24:08  root
# 	Submit
# 	[1995/12/11  14:35:33  root]
# 
# Revision 1.1.2.1  1994/07/06  15:10:04  marrek
# 	Created in July 1994 code drop.
# 	[1994/07/06  13:02:13  marrek]
# 
# $EndLog$
#
# $RCSfile: dcecp_del.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:13 $
#

#######################################################################
#    FILE : dcecp_del.tcl                                             #
#    TYPE : dcecp script                                              #
#    DESCRIPTION : used to remove all rgy objects necessary for       #
#                  gds_sec tests                                      #
#######################################################################


# remove all keytab entries

# delete group and org ,and so ther accounts
puts "deleting group gds_sec_grp  and organization gds_sec_org"
group delete gds_sec_grp 
organization  delete gds_sec_org 

# delete principals
puts "deleting principals "

principal delete { gds_zero  gds_invalid  gds_cl1 gds_cl2 gds_cl3 gds-dsa1  gds-dsa2  }

