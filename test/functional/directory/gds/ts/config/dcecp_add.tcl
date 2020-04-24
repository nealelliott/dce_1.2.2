# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: dcecp_add.tcl,v $
# Revision 1.1.4.2  1996/03/11  02:34:11  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:39:37  marty]
#
# Revision 1.1.4.1  1995/12/11  15:24:06  root
# 	Submit
# 	[1995/12/11  14:35:32  root]
# 
# Revision 1.1.2.2  1994/07/13  16:13:55  keutel
# 	changed "-change" to "-add" (CR 11267)
# 	[1994/07/13  15:54:22  keutel]
# 
# Revision 1.1.2.1  1994/07/06  15:10:01  marrek
# 	Created in July 1994 code drop.
# 	[1994/07/06  13:02:12  marrek]
# 
# $EndLog$
#
# $RCSfile: dcecp_add.tcl,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 02:34:11 $
#

#######################################################################
#    FILE : dcecp_add.tcl                                             #
#    TYPE : dcecp script                                              #
#    DESCRIPTION : used to setup the DCE  registry for gds_sec tests  #
#######################################################################


# add the principals 

puts "creating principals "

principal create { gds_zero  gds_invalid  gds_cl1 gds_cl2 gds_cl3 gds-dsa1  gds-dsa2  }

# use modify to get the ERA in the rgy

puts "assigning values for ERA X500_DN "

principal modify gds_invalid -add {X500_DN {/C=re/O=sni/CN=invalid}}
principal modify gds_cl1 -add {X500_DN {/C=re/O=sni/OU=buba/CN=one}}
principal modify gds-dsa1 -add {X500_DN {/C=re/O=sni/OU=buba/CN=dsa/CN=dsa1}}
principal modify gds_cl2 -add {X500_DN {/C=re/O=sni/OU=ap2/CN=two}}
principal modify gds_cl3 -add {X500_DN {/C=re/O=sni/OU=ap2/CN=three}}
principal modify gds-dsa2 -add {X500_DN {/C=re/O=sni/OU=buba/CN=dsa/CN=dsa2}}

# creating new group and organization

puts "creating new group gds_sec_grp"
group create gds_sec_grp 

puts "creating new organization gds_sec_org"
organization create gds_sec_org 


# adding members to group gds_sec_grp  and org gds_sec_org

puts "adding members to group and org"

group add gds_sec_grp -member { gds_zero gds_invalid gds_cl1 gds_cl2 gds_cl3 gds-dsa1 gds-dsa2 }
organization add gds_sec_org -member { gds_zero gds_invalid gds_cl1 gds_cl2 gds_cl3 gds-dsa1 gds-dsa2 }



# creating accounts 

puts "creating accounts"

account create gds_zero -group {gds_sec_grp} -organization {gds_sec_org} -password {gds_zero} -mypwd {-dce-}     
account create gds_invalid -group {gds_sec_grp} -organization {gds_sec_org} -password {gds_invalid} -mypwd {-dce-}     
account create gds_cl1 -group {gds_sec_grp} -organization {gds_sec_org} -password {gds_cl1} -mypwd {-dce-}     
account create gds_cl2 -group {gds_sec_grp} -organization {gds_sec_org} -password {gds_cl2} -mypwd {-dce-}     
account create gds_cl3 -group {gds_sec_grp} -organization {gds_sec_org} -password {gds_cl3} -mypwd {-dce-}     
account create gds-dsa1 -group {gds_sec_grp} -organization {gds_sec_org} -password {gds-dsa1} -mypwd {-dce-}     
account create gds-dsa2 -group {gds_sec_grp} -organization {gds_sec_org} -password {gds-dsa2} -mypwd {-dce-}     


# keytab entries

