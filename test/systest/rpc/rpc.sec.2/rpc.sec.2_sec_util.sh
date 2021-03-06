#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: rpc.sec.2_sec_util.sh,v $
# Revision 1.1.84.2  1996/03/11  02:46:54  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:08:31  marty]
#
# Revision 1.1.84.1  1995/12/11  22:44:08  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1994/08/04  15:19 UTC  bissen
# 	merge from HPDCE01
# 
# 	HP revision /main/HPDCE01/1  1994/01/27  21:41  bissen
# 	merge kk and hpdce01
# 	[1995/12/11  22:20:47  root]
# 
# Revision 1.1.82.2  1993/10/27  14:53:29  cmckeen
# 	Added the -update option to the rgy_edit commands
# 	[1993/10/27  14:52:17  cmckeen]
# 
# Revision 1.1.82.1  1993/09/09  19:13:02  cmckeen
# 	 HP's TET'ized version
# 	[1993/09/09  19:11:59  cmckeen]
# 
# Revision 1.1.7.2  1993/08/16  14:40:58  eheller
# 	File tetified.
# 	[1993/07/23  17:47:01  eheller]
# 
# Revision 1.1.1.5  1993/05/04  22:22:19  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# Revision 1.1.2.5  1993/02/25  20:16:38  mhickey
# 	Fixed name of path to keytab file in error message.
# 	[1993/02/25  18:58:54  mhickey]
# 
# Revision 1.1.2.4  1993/02/05  15:23:09  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:44:28  cjd]
# 
# Revision 1.1.4.2  1993/02/04  22:13:48  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.2.3  1993/01/13  19:40:25  mhickey
# 	Changed the rpcsec2_sec_add_key  call to allow
# 	caller to specify whether or not to generate a
# 	random key.
# 	[1993/01/13  19:40:11  mhickey]
# 
# Revision 1.1.2.2  1992/12/03  19:53:15  mhickey
# 	Initial check submit for rpc.sec.2 system test.
# 
# 	Defect 5887.
# 	[1992/12/03  15:40:06  mhickey]
# 
# $EndLog$
# 
# rpc.sec.2_sec_util.sh
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
#    Contains utility functions for operations in the Security namespace.  See
#    the section below labelled "FUNCTION LIST" for a list of the functions 
#    contained in this file, what they do, and how they are used.
#
# CALLED BY
#    Called by rpc.sec.2_setup.sh via rpc.sec.2_sec_conf.sh in setting up 
#    the DCE Cell directory service for running the rpc.sec.2 system test.
#
# CALLS INTO
#    DCE user commands (rgy_edit).
#
# FUNCTION LIST
#    Current list of functions and their actions is :
#      rpcsec2_sec_add_princ(principal) - Adds a principal for the passed name.
#                                         Returns "okay" or "error" in variable
#                                         rpcsec2_sec_ap_out.
#
#      rpcsec2_sec_add_group(group)    - Add a group. Returns "okay" or "error"
#                                        in variable rpcsec2_sec_ag_out.
#
#      rpcsec2_sec_add_account(<client|server> name group password 
#                                                               "account_info")
#                     Add an account for a client or server named 'name', 
#                     adding the principal to group 'group', with password 
#                     'password' and account info as specified.  If server is 
#                     specified, then the account allows the principal to be 
#                     both a server and a client (to allow proxy operations,
#                     etc) principal.  If client is specified, the principal
#                     is only allowed to be a client principal.  Returns "okay"
#                     or "error" in the variable rpcsec2_sec_ag_out.
#
#      rpcsec2_sec_add_key(principal, password, keytab_dir, keytab_fn)
#                     Creates a keytab file for the specified principal with
#                     the name keytab_fn, in the directory keytab_dir.  This
#                     function generates 2 keys, the first is based on the 
#                     passed password, the second is a random password 
#                     generated by the system.  Returns "okay" or "error" in
#                     the variable rpcsec2_sec_ak_out.
#
#      rpcsec2_sec_gen_pwent principal 
#                     Generate part of a /etc/passwd entry for the principal. 
#                     Only generates the name:passwd:uid:gid part. It is up to
#                     the caller to fill in the GECOS info, shell, and home 
#                     directory if needed.  Returns the partial /etc/passwd 
#                     entry or "error" in the variable rpcsec2_sec_gp_out.
#
#      rpcsec2_sec_cnf_chkvars
#                     Check for values we need to be successful.  We check all
#                     variables and report all problems to releive the user of
#                     the burden of iterative error discovery.  Returns "okay"
#                     or "error" in rpcsec2_sec_cc_out.
#


#
# rpcsec2_sec_add_princ(principal) - Add a principal.  Returns "okay" or 
#                                    "error" in variable rpcsec2_sec_ap_out.
#
rpcsec2_sec_add_princ()
{
   #
   # initialize return value
   #
   rpcsec2_sec_ap_out="okay"
  
   #
   # try to add the principal
   #
   rgy_edit -update -p << EOF > /tmp/rs2_ap.creat.$$ 2>&1
add
$1

"$2"


quit
EOF

   #
   # Now test to see that the principal got added
   #
   rgy_edit -update -p -v $1 > /tmp/rs2_ap.chk.$$
   #
   # if the principal does not exist...
   #
   egrep "$1[ ]*[0-9]"  /tmp/rs2_ap.chk.$$ >/dev/null 2>&1
   if [ $? -ne 0 ]
   then
      #
      # report prob and set error return value
      #
      echo "Unable to create principal for $1. Error(s) was/were:"
      cat  /tmp/rs2_ap.creat.$$
      rpcsec2_sec_ap_out="error"
   fi
   
   #
   # remove the temp files
   #
   rm -f /tmp/rs2_ap.creat.$$
   rm -f /tmp/rs2_ap.chk.$$
}       

#
# rpcsec2_sec_add_group(group info) - Add a group with associated info.  
#                                     Returns "okay" or "error" in variable
#                                     rpcsec2_sec_ag_out.
#
rpcsec2_sec_add_group()
{
   #
   # initialize return value
   #
   rpcsec2_sec_ag_out="okay"
  
   #
   # try to add the group
   #
   rgy_edit -update -g << EOF > /tmp/rs2_ag.creat.$$ 2>&1
add
$1

"$2"


quit
EOF

   #
   # Now test to see that the group got added
   #
   rgy_edit -update -g -v $1 > /tmp/rs2_ag.chk.$$
   #
   # if the group does not exist
   #
   egrep "$1[ ]*[0-9]"  /tmp/rs2_ag.chk.$$ >/dev/null 2>&1
   if [ $? -ne 0 ]
   then
      #
      # report prob and set error return value
      #
      echo "Unable to create group $1. Error(s) was/were:"
      cat /tmp/rs2_ag.creat.$$
      rpcsec2_sec_ag_out="error"
   fi
   
   #
   # remove the temp files
   #
   rm -f /tmp/rs2_ag.creat.$$
   rm -f /tmp/rs2_ag.chk.$$
}       

#
# rpcsec2_sec_add_account(<client|server> name group password "account_info")
#
#     Add an account for a client or server named 'name', adding the principal
#     to group 'group', with password 'password' and accout info as specified.
#     Returns "okay" or "error" in variable rpcsec2_sec_aa_out.
#
rpcsec2_sec_add_account()
{
   #
   # Note: Called as :
   #      rpcsec2_add_account <client|server> name group password "account_info"
   #
   # initialize return value
   #
   rpcsec2_sec_aa_out="okay"
   #
   # set client or server and initial passwd
   #
   [ "$1" = "client" ] && disable="-dis s" 
   #
   # generate the date
   #
   date_str=`date +%y/%m/%d.%H:%M`
   
   #
   # Try to add the account
   #
   rgy_edit -update -a << EOF > /tmp/rs2_aa.creat.$$ 2>&1
a $2 -g $3 -o none -pw $4 -mp $PRIN_PASSWD -pv -av -m "$5" $disable -gsd $date_str

quit
EOF

   #
   # Now test to see that the account got added
   #
   rgy_edit -update -a -v $2> /tmp/rs2_aa.chk.$$
   #
   # if the account does not exist
   #
   egrep "$2 \[$3"  /tmp/rs2_aa.chk.$$ >/dev/null 2>&1
   if [ $? -ne 0 ]
   then
      #
      # report prob and set error return value
      #
      echo "Unable to create account for $2. Error was:"
      cat  /tmp/rs2_aa.creat.$$
      rpcsec2_sec_aa_out="error"
   fi
   
   #
   # remove the temp files
   #
   rm -f /tmp/rs2_aa.creat.$$
   rm -f /tmp/rs2_aa.chk.$$
}      

#
# rpcsec2_sec_add_key(principal, password, keytab_dir, keytab_fn)
#                      Creates a keytab file for the specified principal with
#                      the name keytab_fn, in the directory keytab_dir.  The
#                      key written to the file is generated by encrypting 
#                      password.  If the keytab_dir does not exists, an attempt
#                      is made to create it.  After the keytab file is created
#                      with the passed password, a random key is generated and
#                      written to both the keytab file and the security 
#                      registry.
#
rpcsec2_sec_add_key()
{
   #
   # set return value
   #
   rpcsec2_sec_ak_out="okay"
  
   #
   # attempt to create the keytab file
   #
   # if the directory does not exist...
   if [ ! -d $4 ]
   then
      #
      # call conf_makepath to get it made
      #
      rpcsec2_conf_makepath $4
      #
      # if the makepath failed...
      #
      if [ "$rpcsec2_conf_mp_out" = "error" ]
      then
         #
         # report and set error status
         #
         echo "Failed to create $4."
         rpcsec2_sec_ak_out="error"
      fi
   fi
   #
   # try to create the keytab, then a random key written to registry and keytab
   #
   if [ "$2" = "random" ]
   then
      rand_key="ktadd -p $1 -a -r -f $4/$5"
   else
      rand_key=""
   fi
   rgy_edit -update << EOF > /tmp/rs2_ak.creat.$$ 2>&1
ktadd -p $1 -pw $3 -f $4/$5
$rand_key
quit
EOF
   #
   # Now check to see if we succeeded.
   #
   # do a ktlist, and capture the output
   #
   rgy_edit -update << EOF > /tmp/rs2_ak.chk.$$ 2>&1
ktlist -f $4/$5
quit
EOF
   #
   # look for the principal and version in the keytab file
   # 
   grep "$1[ 	]*[12]" /tmp/rs2_ak.chk.$$ > /tmp/rs2_ak.chk2.$$ 2>&1
   #
   # if not found...
   #
   if [ $? -ne 0 ]
   then
      #
      # report and set error value
      #
      echo "Could not set up keytab file ($4/$5) for $1."
      echo "Error(s) was/were:"
      cat /tmp/rs2_ak.creat.$$
      rpcsec2_sec_ak_out="error"
   fi
   #
   # paranoid check to make sure that both ktadd ops were done
   #
   rs2_sak_nkeys=`wc -l /tmp/rs2_ak.chk2.$$ | awk '{print $1}'`
   #
   # if nkeys not = 2...
   # 
   if [ $rs2_sak_nkeys -ne 2 -a "$s" = "random" -o $rs2_sak_nkeys -ne 1 -a "$s" = "non-random" ]
   then 
      #
      # report and set error return
      #
      echo "Error on ktadd - keytab file ($4/$5) for $1."
      echo "Error(s) was/were:"
      cat /tmp/rs2_ak.creat.$$
      rpcsec2_sec_ak_out="error"
   fi
   #
   # cleanup temp files
   #
   rm -f /tmp/rs2_ak.creat.$$ /tmp/rs2_ak.chk*.$$
}


#
# rpcsec2_sec_gen_pwent principal 
# 
#    Generate part of a /etc/passwd entry for the principal.  Only generates 
#    the name:passwd:uid:gid part. It is up to the caller to fill in the 
#    GECOS info, shell, and home directory if needed.  Returns the partial
#    /etc/passwd entry or "error" in the variable rpcsec2_sec_gp_out.
#
rpcsec2_sec_gen_pwent()
{
   #
   # get the account info
   #
   rgy_edit -update -a -v $1 > /tmp/rs2_sgp.acc.$$ 2>&1
   #
   # if we failed...
   #
   if [ $? -ne 0 ]
   then 
      # 
      # report error...
      #
      echo "Couldn't get DCE account info for $1.  Error(s) was/were:"
      cat /tmp/rs2_sgp.acc.$$
      #
      # set error return value
      #
      rpcsec2_sec_gp_out="error"
   else
      #
      # strip the funky name off the front of the info, and strip the 
      # rest of the fields (GECOS,shell, home dir) off of the back
      #
      rpcsec2_sgp_pwids=`tail -1 /tmp/rs2_sgp.acc.$$ | \
                         awk -F':' '{print $2 ":" $3 ":" $4}'`
      #
      # construct the final name:passwd:uid:gid return value
      #
      rpcsec2_sec_gp_out=$1:$rpcsec2_sgp_pwids
   fi
}



#
# rpcsec2_sec_cnf_chkvars
#
#   Check for values we need to be successful.  We check all variables and 
#   report all problems to releive the user of the burden of iterative 
#   error discovery.  Return "okay" or "error" in rpcsec2_sec_cc_out.
#
rpcsec2_sec_cnf_chkvars()
{
   #
   # initialize return value
   #
   rpcsec2_sec_cc_out="okay"
   #
   # if no server principal
   #
   if [ ! -n "$RPCSEC2_SRV_PRINC_NAME" ]
   then
      #
      # echo and set error flag
      #
      echo "No value defined for RPCSEC2_SRV_PRINC_NAME..."
      rpcsec2_sec_cc_out="error"
   fi
   #
   # if no client principal
   #
   if [ ! -n "$RPCSEC2_CLI_PRINC_NAME" ]
   then
      #
      # echo and set error flag
      #
      echo "No value defined for RPCSEC2_CLI_PRINC_NAME..."
      rpcsec2_sec_cc_out="error"
   fi
   #
   # if no sever group
   #
   if [ ! -n "$RPCSEC2_SRV_GROUP_NAME" ]
   then
      #
      # echo and set error flag
      #
      echo "No value defined for RPCSEC2_SRV_GROUP_NAME..."
      rpcsec2_sec_cc_out="error"
   fi
   #
   # if no server keytab directory path
   #
   if [ ! -n "$RPCSEC2_SRV_KEYTAB_DIRPATH" ]
   then
      #
      # echo and set error flag
      #
      echo "No value defined for RPCSEC2_SRV_KEYTAB_DIRPATH..."
      rpcsec2_sec_cc_out="error"
   fi
   #
   # if no server keytab filename
   #
   if [ ! -n "$RPCSEC2_SRV_KEYTAB_FN" ]
   then
      #
      # echo and set error flag
      #
      echo "No value defined for RPCSEC2_SRV_KEYTAB_FN..."
      rpcsec2_sec_cc_out="error"
   fi
   #
   # if no client keytab directory
   #
   if [ ! -n "$RPCSEC2_CLI_KEYTAB_DIRPATH" ]
   then
      #
      # echo and set error flag
      #
      echo "No value defined for RPCSEC2_CLI_KEYTAB_DIRPATH..."
      rpcsec2_sec_cc_out="error"
   fi
   #
   # if no client keytab filename
   #
   if [ ! -n "$RPCSEC2_CLI_KEYTAB_FN" ]
   then
      #
      # echo and set error flag
      #
      echo "No value defined for RPCSEC2_CLI_KEYTAB_FN..."
      rpcsec2_sec_cc_out="error"
   fi
   #
   # if no server initial password
   #
   if [ ! -n "$RPCSEC2_SRV_INIT_PW" ]
   then
      #
      # echo and set error flag
      #
      echo "No value defined for RPCSEC2_SRV_INIT_PW..."
      rpcsec2_sec_cc_out="error"
   fi
   #
   # if no client initial password
   #
   if [ ! -n "$RPCSEC2_CLI_INIT_PW" ]
   then
      #
      # echo and set error flag
      #
      echo "No value defined for RPCSEC2_CLI_INIT_PW..."
      rpcsec2_sec_cc_out="error"
   fi
}

