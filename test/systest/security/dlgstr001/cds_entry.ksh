#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: cds_entry.ksh,v $
# Revision 1.1.4.2  1996/03/11  02:48:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:09:00  marty]
#
# Revision 1.1.4.1  1995/12/11  22:45:21  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:21:31  root]
# 
# Revision 1.1.2.2  1994/10/06  20:39:41  agd
# 	expand copyright
# 	[1994/10/06  14:31:09  agd]
# 
# Revision 1.1.2.1  1994/09/19  18:21:14  bhaim
# 	multidelegate test
# 	[1994/09/19  17:52:19  bhaim]
# 
# $EndLog$

: ${cell_admin_pw:=-dce-}
export cell_admin_pw

function get_cell_admin_pw
{
   trap "stty echo; exit" 1 2 15
   print "enter the password for cell_admin, <ctrl-C> to exit"
   stty -echo
   print -n "passwd: "
   read cell_admin_pw
   print "\nenter it again to confirm"
   print -n "passwd: "
   read cell_admin_pw1
   stty echo
   if [ "${cell_admin_pw}" != "${cell_admin_pw1}" ]; then
      print "they do not match -- bye!"
      exit 1
   fi
   trap 1 2 15
}

function create_entry
{
   for principal in ${principal_list}; do
      print "add permissions for \"user:${principal}:${permissions}\""
      permission_string="${permission_string} user:${principal}:${permissions}"
   done
   print "acl_edit -e ${cell_profile}"
   print "\t-m ${permission_string}"
   acl_edit -e ${cell_profile} -m ${permission_string}
   sleep 5
}

function delete_entry
{
   for principal in ${principal_list}; do
      print "delete permissions for \"user:${principal}:${permissions}\""
      permission_string="${permission_string} user:${principal}:${permissions}"
   done
   print "acl_edit -e ${cell_profile}"
   print "\t-d ${permission_string}"
   acl_edit -e ${cell_profile} -d ${permission_string}
   cdscp delete object /.:/subsys/HP/sample-apps/string-convert
   cdscp delete object /.:/subsys/HP/sample-apps/string-convert2
   cdscp delete object /.:/subsys/HP/sample-apps/string-convert3
   cdscp delete object /.:/subsys/HP/sample-apps/string-convert4
   cdscp delete object /.:/subsys/HP/sample-apps/string-convert5
   cdscp delete object /.:/subsys/HP/sample-apps/string-convert6
   cdscp show object ${cell_profile} 1>/dev/null 2>&1  # bogus command to update cache or else we fail
   cdscp set cdscp confidence high
   rpccp remove element ${cell_profile} -a string_conv
   rpccp remove element ${cell_profile} -a string_conv2
   rpccp remove element ${cell_profile} -a string_conv3
   rpccp remove element ${cell_profile} -a string_conv4
   rpccp remove element ${cell_profile} -a string_conv5
   rpccp remove element ${cell_profile} -a string_conv6
   cdscp set cdscp confidence medium
}

if [[ -z "${cell_admin_pw:-}" ]]; then
   get_cell_admin_pw
fi

if [[ -z "${KRB5CCNAME:-}" ]]; then
   dce_login cell_admin ${cell_admin_pw} -e $0 $*
   exit $?
fi

this_host="$(hostname)"

principal_list="string-conv string-conv2 string-conv3 string-conv4 string-conv5 string-conv6"

: ${cell_profile:=/.:/cell-profile}
permissions="rwt"
typeset permission_string=""

while (( $# > 0 )); do
   case $1 in
      create)
         operation="create"
         shift
         ;;
      delete)
         operation="delete"
         shift
         ;;
      *)
         print "bad operation $1"
         print "usage: cds_entry.ksh [create | delete]"
         exit 1
         ;;
   esac
done

{
   klist | grep 'Principal:.*cell_admin'
} > /dev/null 2>&1

if (( $? != 0 )); then
   print "you must be dce_login as cell_admin before you run this script."
   exit 1
fi

case "${operation:-unknown}" in
   create)
      create_entry
      ;;
   delete)
      delete_entry
      ;;
   *)
      print "missing operation"
      print "usage: cds_entry.ksh [create | delete]"
      kdestroy
      exit 1
      ;;
esac
kdestroy
