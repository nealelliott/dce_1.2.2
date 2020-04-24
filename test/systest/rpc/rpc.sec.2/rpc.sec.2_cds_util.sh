# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: rpc.sec.2_cds_util.sh,v $
# Revision 1.1.67.2  1996/03/11  02:46:28  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:20  marty]
#
# Revision 1.1.67.1  1995/12/11  22:43:43  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:20:32  root]
# 
# Revision 1.1.64.1  1993/09/09  19:12:44  cmckeen
# 	 HP's TET'ized version
# 	[1993/09/09  19:11:42  cmckeen]
# 
# Revision 1.1.5.2  1993/08/16  14:22:10  eheller
# 	File tetified.
# 	[1993/07/23  17:42:04  eheller]
# 
# Revision 1.1.1.5  1993/05/04  22:22:02  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.2.3  1993/02/05  15:22:13  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:42:59  cjd]
# 
# Revision 1.1.3.2  1993/02/04  22:11:40  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.2.2  1992/12/03  19:52:44  mhickey
# 	Initial check submit for rpc.sec.2 system test.
# 
# 	Defect 5887.
# 	[1992/12/03  15:39:35  mhickey]
# 
# $EndLog$
# 
# rpc.sec.2_cds_util.sh
#
# ASSOCIATION
#    This file is part of the rpc.sec.2 DCE System Test.
# 
#    The rpc.sec.2 system test corresponds to test rpc.sec.2 in the DCE1.0.2
#    Core Component System Test plan.  For more information on the operation
#    and goals of the rpc.sec.2 system test, see the test plan, or the README
#    file in this directory.
#
#
# FUNCTION
#    Contains utility functions for operations in the CDS namespace.  See the
#    section below labelled "FUNCTION LIST" for a list of the functions 
#    contained in this file, what they do, and how they are used.
#
# CALLED BY
#    Called by rpc.sec.2_setup.sh via rpc.sec.2_cds_conf.sh in setting up 
#    the DCE Cell directory service for running the rpc.sec.2 system test.
#
# CALLS INTO
#    DCE user commands (cdscp).
#
# FUNCTION LIST
#    Current list of functions and their actions is :
#      rpcsec2_cds_makepath(path) 
#                    if path is a full path; ie: starts with /... or /.: ;
#                    will create all non-existent directories in the path.
#                    Output is a value of "okay" or "error" in the variable 
#                    tstrpc_cds_mp_out.
#
#      rpcsec2_cds_setacl(cds_entry acl_edit_switches sec_acl_entry)
#                    Sets the directory acl (or the initial object acl if 
#                    acl_edit_switches is set to -io) on cds_entry to 
#                    sec_acl_entry, then verifies that the sec_acl_entry is in
#                    the acl for the directory.  Returns "okay" or "error"
#                    in the variable rpcsec2_cds_sa_out.
#
#      rpcsec2_cds_cnf_chkvars
#                    Check for values we need to be successful.  We check all
#                    variables and report all problems to relieve the user of 
#                    the burden of iterative error discovery.  Returns "okay" 
#                    or "error" in rpcsec2_cds_cc_out.
#



#
# rpcsec2_cds_makepath(path)
#
#   Create the non-existent directories in the specified path. path must start
#   at the cell root.  Note that we use cdscp show dir to look for existing 
#   directories instead of cdscp list dir due to a bug in cdscp (as of 1.0.2b3)
#   whereby cdscp returns 0 if a non-existent directory is specified to the 
#   list subcommand.  This may change in the future.
#
rpcsec2_cds_makepath()
{
   rpcsec2_cds_mp_out="okay"

   #
   # check the input
   #
   rs2_cmp_rt=`echo $1 | awk -F'/' '{ print $2 }'`
   #
   # if the root of the path is not /.../ or /.:/...
   #
   if [ "$rs2_cmp_rt" != "..." -a "$rs2_cmp_rt" != ".:" ]
   then
      #
      # print a message and set the return value to error
      #
      echo "Path to be created must start at root."
      rpcsec2_cds_mp_out="error"
   else
      #
      # split the components up and strip off the root component
      #
      rs2_cmp_dl=`echo $1 | sed -e "s/^\/$rs2_cmp_rt\///"  -e 's/\// /g'`
      #
      # make a copy of the list.  This is the not found list.
      #
      rs2_cmp_nfl=$rs2_cmp_dl
      #
      # start the found list with the root dir
      # 
      rs2_cmp_fdl="/$rs2_cmp_rt"
      # 
      # loop through the directory path until we find a non-existent directory
      #
      for currdir in $rs2_cmp_dl
      do
         #
         # check for existence of the directory path so far
         #
         cdscp show dir $rs2_cmp_fdl/$currdir >/dev/null 2>&1
         
         #
         # if we couldn't find it...
         #
         if [ $? -ne 0 ] 
         then 
            #
            # break out of the loop
            #
            break
         #
         # else if we did find it
         #
         else
            #
            # pull the current directory off of the not found list
            #
            rs2_cmp_nfl=`echo $rs2_cmp_nfl | sed -e "s/$currdir //" -e 's/^ //'`
            #
            # and add it to the found path
            #
            rs2_cmp_fdl="$rs2_cmp_fdl/$currdir"
         fi
      done
      #
      # if the found list does not equal what we were passed
      #
      if [ "$1" != "$rs2_cmp_fdl" ]
      then
         # 
         # loop through the not found components
         #
         for currdir in $rs2_cmp_nfl
         do
            #
            # try to create the dir
            # 
            cdscp create dir $rs2_cmp_fdl/$currdir > /tmp/rs2_cdsmp.cr$$ 2>&1
            #
            # if we did not succeed
            #
            if [ $? -ne 0 ]
            then
               #
               # print error and set return value to error
               #
               echo "     Could not create $rs2_cmp_fdl/$currdir. Error(s):"
               cat /tmp/rs2_cdsmp.cr$$
               rm -f /tmp/rs2_cdsmp.cr$$
               rpcsec2_cds_mp_out="error"
               break
            #
            # else if we succeeded
            #
            else
               #
               # add the curr dir to the found path
               #
               rs2_cmp_fdl="$rs2_cmp_fdl/$currdir"
            fi
         done
      fi
   fi
   rm -f /tmp/rs2_cdsmp.cr$$
}      

#
# rpcsec2_cds_setacl(cds_entry acl_edit_switches sec_acl_entry)
#
#    Sets the directory acl (or the initial object acl if acl_edit_switches
#    is set to -io) on cds_entry to sec_acl_entry, then verifies that the
#    sec_acl_entry is in the acl for the directory.  Returns "okay" or "error"
#    in rpcsec2_cds_sa_out.  Note that it an error to call this routine with
#    all_edit_switches set to -io if the cds_entry arguement is nto a cds 
#    directory.  Note also that we do not (gracefully) handle setting acl's
#    on cds ENTRY OBJECTS as opposed to the directories and objects referred to
#    by the entry object; ie: setting the acl controlling who can modify the 
#    attributes of a directory object as opposed to who can add, delete, etc
#    in the directory referred to by the directory object.  This can be kludged
#    if necessary, by passing a quoted first arguement; eg: '-e /.:/xxx'.
#
rpcsec2_cds_setacl()
{
   #
   # set return value
   #
   rpcsec2_cds_sa_out="okay"
   #
   # set default list of possible permissions
   #
   rs2_cs_plist="r w d t c i a" 
   #
   # if we were passed 3 args
   #
   if [ $# -eq 3 ]
   then
      #
      # we have acl_edit switches.  Set internal variables 
      #
      rs2_cs_aclsw=$2
      rs2_cs_aclas=$3
      #
      # if acl_edit_switches is -ic or -io...
      #
      echo $2 > /tmp/rs2_cs.pchk.$$
      grep -e '-i[oc]' /tmp/rs2_cs.pchk.$$ > /dev/null 2>&1
      if [ $? -eq 0 ]
      then
         #
         # acl_switches is -io or -ic.  If the cds_entry is not a directory(or
         # does not exist)...
         #
         cdscp show dir $1 > /tmp/rs2_cs.dchk.$$ 2>&1
         if [ $? -ne 0 ]
         then 
            #
            # print and set error value
            #
            echo "Can't find CDS directory $1(directory implied by $2 switch)."
            echo "Error(s) was/were:"
            cat /tmp/rs2_cs.dchk.$$
            rpcsec2_cds_sa_out="error"
         fi
      fi
   else 
      #
      # no acl_edit_switches, so just set switch arg to ""
      #
      rs2_cs_aclsw=""
      rs2_cs_aclas=$2
   fi

   #
   # order the acl string we were passed like acl_edit will
   #
   # save the original bits to a file
   #
   echo $rs2_cs_aclas | awk -F':' '{print $3;}' > /tmp/rs2_cs.ochk.$$
   #
   # save the type and name
   #
   rs2_cs_aclas_base=`echo $rs2_cs_aclas | awk -F':' '{print $1 ":" $2 ":";}`
   #
   # init the bit string we will use
   #
   rs2_cs_aclstr=
   #
   # loop through the possible permission bits, in the order acl_edit wants
   #
   for perm in $rs2_cs_plist
   do
      #
      # look for this permission bit
      #
      grep $perm /tmp/rs2_cs.ochk.$$ >/dev/null 2>&1
      #
      # if perm in list...
      #
      if [ $? -eq 0 ]
      then
         #
         # set the bit in the string we will pass
         #
         rs2_cs_aclstr=${rs2_cs_aclstr}${perm}
      else
         #
         # bit not in string, so add a '-' like acl_edit will
         #
         rs2_cs_aclstr=${rs2_cs_aclstr}-
      fi
   done
   #
   # construct the string we will pass
   #
   rs2_cs_aclstr=${rs2_cs_aclas_base}${rs2_cs_aclstr}
   #
   # try to set the acl
   #  
   acl_edit $1 $rs2_cs_aclsw -m $rs2_cs_aclstr > /tmp/rs2_cs.set.$$ 2>&1
   #
   # now verify the acl. print the acl for the entry to a file.
   #
   acl_edit $1 $rs2_cs_aclsw -l > /tmp/rs2_cs.chk.$$ 2>&1
   #
   # if we don't find the acl_entry we were passed in the file...
   #
   grep $rs2_cs_aclstr  /tmp/rs2_cs.chk.$$ >/dev/null 2>&1
   if [ $? -ne 0 ]
   then
      #
      # acl entry not found.  report error and set error return.
      #
      echo "Couldn't set ACL on $1.  Error(s) was/were:"
      cat /tmp/rs2_cs.set.$$
      rpcsec2_cds_sa_out="error"
   fi
 
   #
   # remove temp files
   #
   rm -rf /tmp/rs2_cs.*.$$
}


#
# rpcsec2_cds_cnf_chkvars
#
#   Check for values we need to be successful.  We check all variables and 
#   report all problems to releive the user of the burden of iterative 
#   error discovery.  Return "okay" or "error" in rpcsec2_cds_cc_out.
#
rpcsec2_cds_cnf_chkvars()
{
   #
   # initialize return value
   #
   rpcsec2_cds_cc_out="okay"
   #
   # if no server CDS entry directory name
   #
   if [ ! -n "$RPCSEC2_SRV_CDS_NAME" ]
   then
      #
      # echo and set error flag
      #
      echo "No value defined for RPCSEC2_SRV_CDS_NAME..."
      rpcsec2_cds_cc_out="error"
   fi
   #
   # if no server group name
   #
   if [ ! -n "$RPCSEC2_SRV_GROUP_NAME" ]
   then
      #
      # echo and set error flag
      #
      echo "No value defined for RPCSEC2_SRV_GROUP_NAME..."
      rpcsec2_cds_cc_out="error"
   fi
   #
   # if no server CDS directory ACL entry
   #
   if [ ! -n "$RPCSEC2_SRV_CDS_DIR_ACL" ]
   then
      #
      # echo and set error flag
      #
      echo "No value defined for RPCSEC2_SRV_CDS_DIR_ACL..."
      rpcsec2_cds_cc_out="error"
   fi
}
