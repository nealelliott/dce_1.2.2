#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: rpc.cds.3_conf_util.sh,v $
# Revision 1.1.6.2  1996/03/11  02:45:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:03  marty]
#
# Revision 1.1.6.1  1995/12/11  22:42:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:19:57  root]
# 
# Revision 1.1.4.1  1994/06/10  20:50:01  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:19:31  devsrc]
# 
# Revision 1.1.2.2  1993/03/12  04:27:22  mhickey
# 	Initial check in for the rpc.cds.3 system test.
# 	[1993/01/29  19:15:14  mhickey]
# 
# 	Initial check submit for rpc.sec.2 system test.
# 
# 	Defect 5887.
# 	[1992/12/03  15:39:47  mhickey]
# 
# $EndLog$
# 
# rpcsec2_conf_util.sh
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
#    Contains routines used by rpcsec2 setup scripts.  See the section below
#    labelled "FUNCTION LIST" for a list of the functions contained in this
#    file, what they do, and how they are used.
#
# CALLED BY
#    Called by rpc.sec.2_setup.sh via while setting up the DCE cell for 
#    running the rpc.sec.2 system test.
#
# CALLS INTO
#    DCE user commands (klist).
#
# FUNCTION LIST
#    Current list of functions and their actions is :
#      rpcsec2_conf_get_cell() - returns the full cell name
#
#      rpcsec2_conf_check_auth(principal) 
#                                Checks to see that the principal is 
#                                dce_login'ed as 'principal'
#
#
# rpcsec2_conf_get_cell()
# 
#   Returns the full cell name
#
rpcsec2_conf_get_cell()
{
   #
   # use cdscp to generate something that has the cell name in it
   #
   cdscp show cell /.: > /tmp/tcgc.$$
   #
   # check the status... if there is an error
   if [ $? -ne 0 ]
   then
      #
      # then print an error and return an error value
      #
      echo "Can't get cell name"
      rpcsec2_conf_gc_out="error"
   else
      #
      # else no error, so fetch the cell name 
      #
      rpcsec2_conf_gc_out=`awk '$1 == "CELL" {print $2}' /tmp/tcgc.$$`
   fi 
   
   #
   # remove the tmp file
   #
   rm -f /tmp/tcgc.$$
}

#
# rpcsec2_conf_check_auth(principal)
#
#   Checks to see that the principal is dce_login'ed as 'principal'
#
rpcsec2_conf_check_auth() 
{

   rpcsec2_conf_ca_out="okay"

   #
   # see if we can get auth info
   #
   klist > /tmp/tcca.$$
   #
   # check results... if we failed...
   #
   if [ $? -ne 0 ]
   then
      #
      # then print an error and return an error
      #
      echo "     Unauthenticated...dce_login as $1 and try again."
      rpcsec2_conf_ca_out="error"
   else
      #
      # Still need to check if we have valid tickets
      #
      rs2_cca_gn=`awk '$1 == "Group:" { print $3; }' /tmp/tcca.$$`
      #
      # if the group is unknown, then we need to renew tickets
      #
      if [ "$rs2_cca_gn" = "<group name unknown>" ] 
      then
         #
         # Set return value to indicate error
         # 
         rpcsec2_conf_ca_out="error"         
      fi
      #
      # at this point we will check the principal name regardless of whether
      # the tickets are expired.  We do this so that they can dce_login as 
      # the correct user if necessary before re-starting
      #
      rs2_cca_pn=`awk '$1 == "Principal:" { print $3; }'  /tmp/tcca.$$`
      #
      # compare the principal we were passed with the identity. If 
      # they are different...
      #
      if [ "$rs2_cca_pn" != "$1" ]
      then
         #
         # then print a message and set the error value
         #
         echo "     Incorrect principal name ($rs2_cca_pn)."
         echo "     Please dce_login as $1 and try again."
         rpcsec2_conf_ca_out="error"
      #
      # if the names match, we still may need to tell the user to kinit.
      # see if the return value is set to error at this point.  If it is...
      #
      elif [ "$rpcsec2_conf_ca_out" = "error" ]
      then
         #
         # print the appropriate message
         #
         echo "     Tickets expired... Please run kinit and try again."
      fi
   fi
   
   #
   # Remove tmp file
   #
   rm -f /tmp/tcca.$$
}

# 
# rpcsec2_conf_makepath(path)
#
#   Create the non-existent directories in the specified path. path must start
#   at the  root
#
rpcsec2_conf_makepath()
{
   rpcsec2_conf_mp_out="okay"

   #
   # check the input
   #
   rs2_cnmp_rt=`echo $1 | awk '{ print substr($0,1,1) }'`
   #
   # if the root of the path is not /
   #
   if [ "$rs2_cnmp_rt" != "/" ]
   then
      #
      # print a message and set the return value to error
      #
      echo "Path to be created must start at root."
      rpcsec2_conf_mp_out="error"
   else
      #
      # split the components up and strip off the root component
      #
      rs2_cnmp_dl=`echo $1 | sed -e "s/^\///"  -e 's/\// /g'`
      #
      # make a copy of the list.  This is the not found list.
      #
      rs2_cnmp_nfl=$rs2_cnmp_dl
      #
      # start the found list with the root dir
      # 
      rs2_cnmp_fdl="/"
      # 
      # loop through the directory path until we find a non-existent directory
      #
      for currdir in $rs2_cnmp_dl
      do
         #
         # check for existence of the directory path so far
         #
         [ "$rs2_cnmp_fdl" = "/" ] && rs2_cnmp_fdl=""
         if [ ! -d $rs2_cnmp_fdl/$currdir ]
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
            rs2_cnmp_nfl=`echo $rs2_cnmp_nfl | sed -e "s/$currdir //" -e 's/^ //'`
            #
            # and add it to the found path
            #
            rs2_cnmp_fdl="$rs2_cnmp_fdl/$currdir"
         fi
      done
      #
      # if the found list does not equal what we were passed
      #
      if [ "$1" != "$rs2_cnmp_fdl" ]
      then
         # 
         # loop through the not found components
         #
         for currdir in $rs2_cnmp_nfl
         do
            #
            # try to create the dir
            # 
            mkdir $rs2_cnmp_fdl/$currdir > /dev/null 2>&1
            #
            # if we did not succeed
            #
            if [ ! -d $rs2_cnmp_fdl/$currdir ]
            then
               #
               # print error and set return value to error
               #
               echo "Could not create $rs2_cnmp_fdl/$currdir."
               rpcsec2_conf_mp_out="error"
               break
            #
            # else if we succeeded
            #
            else
               #
               # add the curr dir to the found path
               #
               rs2_cnmp_fdl="$rs2_cnmp_fdl/$currdir"
            fi
         done
      fi
   fi
}      

