#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: rgy_acct.ksh,v $
# Revision 1.1.4.2  1996/03/11  02:48:07  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:09:01  marty]
#
# Revision 1.1.4.1  1995/12/11  22:45:46  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:21:46  root]
# 
# Revision 1.1.2.2  1994/10/06  20:45:06  agd
# 	expand copyright
# 	[1994/10/06  14:32:27  agd]
# 
# Revision 1.1.2.1  1994/09/19  18:21:30  bhaim
# 	multidelegate test
# 	[1994/09/19  17:52:23  bhaim]
# 
# $EndLog$
set -x

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

if [[ -z "${cell_admin_pw:-}" ]]; then
   get_cell_admin_pw
fi

if [[ -z "${KRB5CCNAME:-}" ]]; then
   dce_login cell_admin ${cell_admin_pw} -e $0 $*
   exit $?
fi

#user_list=${TUSER:-"deleg-user"}
user_list=${TUSER:-"odss-delegation-princ"}
principal_list="string-conv string-conv2 string-conv3 string-conv4 string-conv5 string-conv6"
principal_pw=${TPW:-"pignoses"}

this_host="`hostname`"

typeset -L ktvnos

function create_acct
{
   if [[ -z "${cell_admin_pw}" ]]; then
      get_cell_admin_pw
   fi

   {
      for principal in ${principal_list}; do
      {
         print "create principals and accounts for ${principal} using pw ${principal_pw}" 

         if [ -n "${KEYTAB_FILE}" ]; then
            print "create a keytab file named \"${KEYTAB_FILE}\""
         else
            print "add keys to the system keytab file for ${principal}"
         fi
      } > /dev/null

      print "
         domain principal
         add ${principal}
         domain account
         add ${principal} -g none -o none -pw ${principal_pw} \
         -mp ${cell_admin_pw} -mcl 8h
      "

      print "
         ktadd -p ${principal} ${KEYTAB_FILE:-} -pw ${principal_pw}
         ktl -p ${principal} ${KEYTAB_FILE:-}
      "

      done
      print "exit"

   } | rgy_edit

   {
      for users in ${user_list}; do
      {
         print "create principals and accounts for ${users} using pw ${principal_pw}"

         if [ -n "${KEYTAB_FILE}" ]; then
            print "create a keytab file named \"${KEYTAB_FILE}\""
         else
            print "add keys to the system keytab file for ${users}"
         fi
      } > /dev/null

      print "
         domain principal
         add ${users}
         domain account
         add ${users} -g none -o none -pw ${principal_pw} \
         -mp ${cell_admin_pw} -mcl 8h
      "

      print "
         ktadd -p ${users} ${KEYTAB_FILE:-} -pw ${principal_pw}
         ktl -p ${users} ${KEYTAB_FILE:-}
      "

      done
      print "exit"

   } | rgy_edit
} 

function delete_acct
{
set -x
   {
      for principal in ${principal_list}; do
      {
         if [ -n "${KEYTAB_FILE}" ]; then
            print "delete the keytab file named \"${KEYTAB_FILE}\""
         else
            print "delete the keys from the system keytab file"
         fi

         print "delete principal and account for ${principal}"

      } > /dev/null

      ktvnos=$( \
         print "ktl -p ${principal:-} ${KEYTAB_FILE:-}" \
         | rgy_edit \
         | awk '{if ($1 ~ /\/'"${principal:-}"'$/) {printf ("%d ", $2)} }' \
         )

      for key_no in ${ktvnos:-}; do
         print "ktd -p ${principal} -v ${key_no} ${KEYTAB_FILE:-}" > /dev/null
         print "ktd -p ${principal} -v ${key_no} ${KEYTAB_FILE:-}"
      done

      print "
         domain account
         delete -p ${principal} -g none -o none 
         domain principal
         delete ${principal}"

      done
      print "exit"

   } | rgy_edit

   {
      for users in ${user_list}; do
      {
         if [ -n "${KEYTAB_FILE}" ]; then
            print "delete the keytab file named \"${KEYTAB_FILE}\""
         else
            print "delete the keys from the system keytab file"
         fi

         print "delete principal and account for ${users}"

      } > /dev/null

      ktvnos=$( \
         print "ktl -p ${users:-} ${KEYTAB_FILE:-}" \
         | rgy_edit \
         | awk '{if ($1 ~ /\/'"${users:-}"'$/) {printf ("%d ", $2)} }' \
         )

      for key_no in ${ktvnos:-}; do
         print "ktd -p ${users} -v ${key_no} ${KEYTAB_FILE:-}" > /dev/null
         print "ktd -p ${users} -v ${key_no} ${KEYTAB_FILE:-}"
      done

      print "
         domain account
         delete -p ${users} -g none -o none 
         domain principal
         delete ${users}"

      done
      print "exit"

   } | rgy_edit

   ktvnos="${ktvnos:-} "$(
   for principal in ${principal_list}; do
      print "ktl -p ${principal:-} ${KEYTAB_FILE:-}" \
      | rgy_edit \
      | awk '{if ($1 ~ /\/'"${principal:-}"'$/) {printf ("%s ", $2)} }'
   done
   )

   if [[ -n "${KEYTAB_FILE}" && -z "${ktvnos}" ]]; then
      print "rm ${KEYTAB_FILE}" > /dev/null
      rm ${KEYTAB_FILE}
   fi
}

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
         print "usage: rgy_acct.ksh [create | delete]"
         exit 1
         ;;
   esac
done

{
   klist | grep 'Principal:.*cell_admin'
} > /dev/null 2>&1

if (( $? != 0 )); then
   print "you must dce_login as cell_admin before you run this script."
   exit 1
fi

if [ -n "${KEYTAB_FILE}" ]; then
   KEYTAB_FILE="-f ${KEYTAB_FILE}"
else
   if [ "`whoami`" != "root" ]; then
      print "you must be root to access the system keytab file"
      print "you are currently \"`whoami`\""
      exit 1
   fi
fi

case "${operation:-unknown}" in
   create)
      create_acct
      ;;
   delete)
      delete_acct
      ;;
   *)
      print "missing operation"
      print "usage: rgy_acct.ksh [create | delete]"
      kdestroy
      exit 1
      ;;
esac
kdestroy
