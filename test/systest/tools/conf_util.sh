#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: conf_util.sh,v $
# Revision 1.1.4.2  1996/03/11  02:49:20  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:09:58  marty]
#
# Revision 1.1.4.1  1995/12/11  22:50:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:23:32  root]
# 
# Revision 1.1.2.2  1993/07/02  16:40:45  mhickey
# 	Initial checkin of conf_util.  This contains shell
# 	utilities to determine the cell name, make a
# 	directory path, and check the users dce creds.
# 	[1993/07/02  16:40:08  mhickey]
# 
# $EndLog$
# 
# conf_util.sh
# 
# ASSOCIATION
#    This is a file of shell tools for use in system testing
#
# FUNCTION
#    Contains routines used by setup scripts.  See the section below
#    labelled "FUNCTION LIST" for a list of the functions contained in this
#    file, what they do, and how they are used.
#
# CALLED BY
#    Called by setup scripts while setting up the DCE cell for 
#    running system tests.
#
# CALLS INTO
#    DCE user commands (klist).
#
# FUNCTION LIST
#    Current list of functions and their actions is :
#      conf_get_cell()            - returns the full cell name
#
#      conf_check_auth(principal) - Checks to see that the principal is 
#                                   dce_login'ed as 'principal'
#
#      conf_makepath(path)        - Attempts to create any non-existent parts
#                                   of <path>.  Note that path must be 
#                                   absolute; ie: absolute UNIX type path, or
#                                   DCE global path.
#
# conf_get_cell()
# 
#   Returns the full cell name
#
conf_get_cell()
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
      conf_gc_out="error"
   else
      #
      # else no error, so fetch the cell name 
      #
      conf_gc_out=`awk '$1 == "CELL" {print $2}' /tmp/tcgc.$$`
   fi 
   
   #
   # remove the tmp file
   #
   rm -f /tmp/tcgc.$$
}

#
# conf_check_auth(principal)
#
#   Checks to see that the principal is dce_login'ed as 'principal'
#
conf_check_auth() 
{

   conf_ca_out="okay"

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
      conf_ca_out="error"
   else
      #
      # Still need to check if we have valid tickets
      #
      cca_gn=`awk '$1 == "Group:" { print $3; }' /tmp/tcca.$$`
      #
      # if the group is unknown, then we need to renew tickets
      #
      if [ "$cca_gn" = "<group name unknown>" ] 
      then
         #
         # Set return value to indicate error
         # 
         conf_ca_out="error"         
      fi
      #
      # at this point we will check the principal name regardless of whether
      # the tickets are expired.  We do this so that they can dce_login as 
      # the correct user if necessary before re-starting
      #
      cca_pn=`awk '$1 == "Principal:" { print $3; }'  /tmp/tcca.$$`
      #
      # compare the principal we were passed with the identity. If 
      # they are different...
      #
      if [ "$cca_pn" != "$1" ]
      then
         #
         # then print a message and set the error value
         #
         echo "     Incorrect principal name ($cca_pn)."
         echo "     Please dce_login as $1 and try again."
         conf_ca_out="error"
      #
      # if the names match, we still may need to tell the user to kinit.
      # see if the return value is set to error at this point.  If it is...
      #
      elif [ "$conf_ca_out" = "error" ]
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
# conf_makepath(path)
#
#   Create the non-existent directories in the specified path. path must start
#   at the  root
#
conf_makepath()
{
   conf_mp_out="okay"

   #
   # check the input
   #
   cnmp_rt=`echo $1 | awk '{ print substr($0,1,1) }'`
   #
   # if the root of the path is not /
   #
   if [ "$cnmp_rt" != "/" ]
   then
      #
      # print a message and set the return value to error
      #
      echo "Path to be created must start at root."
      conf_mp_out="error"
   else
      #
      # split the components up and strip off the root component
      #
      cnmp_dl=`echo $1 | sed -e "s/^\///"  -e 's/\// /g'`
      #
      # make a copy of the list.  This is the not found list.
      #
      cnmp_nfl=$cnmp_dl
      #
      # start the found list with the root dir
      # 
      cnmp_fdl="/"
      # 
      # loop through the directory path until we find a non-existent directory
      #
      for currdir in $cnmp_dl
      do
         #
         # check for existence of the directory path so far
         #
         [ "$cnmp_fdl" = "/" ] && cnmp_fdl=""
         if [ ! -d $cnmp_fdl/$currdir ]
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
            cnmp_nfl=`echo $cnmp_nfl | sed -e "s/$currdir //" -e 's/^ //'`
            #
            # and add it to the found path
            #
            cnmp_fdl="$cnmp_fdl/$currdir"
         fi
      done
      #
      # if the found list does not equal what we were passed
      #
      if [ "$1" != "$cnmp_fdl" ]
      then
         # 
         # loop through the not found components
         #
         for currdir in $cnmp_nfl
         do
            #
            # try to create the dir
            # 
            mkdir $cnmp_fdl/$currdir > /dev/null 2>&1
            #
            # if we did not succeed
            #
            if [ ! -d $cnmp_fdl/$currdir ]
            then
               #
               # print error and set return value to error
               #
               echo "Could not create $cnmp_fdl/$currdir."
               conf_mp_out="error"
               break
            #
            # else if we succeeded
            #
            else
               #
               # add the curr dir to the found path
               #
               cnmp_fdl="$cnmp_fdl/$currdir"
            fi
         done
      fi
   fi
}      

