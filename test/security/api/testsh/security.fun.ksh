#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: security.fun.ksh,v $
# Revision 1.1.2.2  1996/03/09  20:50:30  marty
# 	Add OSF copyright
# 	[1996/03/09  20:28:36  marty]
#
# Revision 1.1.2.1  1995/12/11  21:50:41  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:53:19  root]
# 
# $EndLog$
print "START DCE SECURITY tests: ${TET_PNAME}; DATE: `date`"
NLSPATH=/usr/lib/nls/%L/%N
export NLSPATH 
export CELLADMIN=cell_admin 
export PATH=${TET_ROOT}/test/security/api/control:${TET_ROOT}/test/security/api/testsh:${PATH}
export HOST=`hostname`
LOGDIR=${LOGDIR:-/tet/test/results}
LOGFILE=${LOGDIR}/security_log.$(date +%h%d_%H:%M)
touch ${LOGFILE}
echo "Log file name is " ${LOGFILE}
link=0

if [ -f /usr/bin/dce_login -a ! -f /usr/bin/dce_login_noexec ]
then
	ln -s /usr/bin/dce_login /usr/bin/dce_login_noexec >/dev/null
        link=1
fi 

export KRB5CCNAME=`dce_login_noexec cell_admin -dce-` 
cd $TET_ROOT/test/security/api/moretests
rgy_setup.sh  >>${LOGFILE} 2>&1
key_mgmt_setup.sh  >>${LOGFILE} 2>&1
cd ../control
print "DCE SECURITY Tests"
print $PWD

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/pgo_person.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/pgo_group.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/pgo_org.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/pgo_get_next.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/pgo_member.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/pgo_key_transfer.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/acct_add.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/acct_admin_repl.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/acct_del.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/acct_lookup.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/acct_passwd.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/acct_rename.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/acct_repl_all.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/acct_user_repl.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_setup_identity.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_purge_context.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_release_context.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_validate_identity.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_certify_identity.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_valid_and_cert_ident.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_set_context.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_get_current_context.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_refresh_identity.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_get_pwent.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_get_expiration.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_export_import_context.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_get_groups.tsh >>${LOGFILE} 2>&1
           
testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_newgroups.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_get_info.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/login_get_effective.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/rgy_set_resolve_mode.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/rgy_wait_until_consistent.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/rgy_set_cache_handler.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/rgy_reset_cursor.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/site_mgmt.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/site_bind.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/sec_acl.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/id_map.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/key_mgmt_set_key.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/key_mgmt_get_key.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/key_mgmt_get_next_key.tsh >>${LOGFILE} 2>&1
 
testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/key_mgmt_cursor.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/key_mgmt_get_nth_key.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/key_mgmt_gen_rand_key.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/key_mgmt_change_key.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/key_mgmt_garbage_collect.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/key_mgmt_free_key.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/key_mgmt_delete_key.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/key_mgmt_delete_key_type.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/key_mgmt_manage_key.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/auth_pol_get.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/auth_pol_get_eff.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/auth_pol_set.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/policy_get.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/policy_get_eff.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/policy_set.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/props_get.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/props_set.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/unix.tsh >>${LOGFILE} 2>&1

cd $TET_ROOT/test/security/api/moretests
/bin/sh $TET_ROOT/test/security/api/moretests/rgy_add.sh -d >>${LOGFILE} 2>&1 

/bin/sh $TET_ROOT/test/security/api/moretests/rgy_policy.sh -d >>${LOGFILE} 2>&1

/bin/sh $TET_ROOT/test/security/api/moretests/rgy_change.sh -d >>${LOGFILE} 2>&1

/bin/sh $TET_ROOT/test/security/api/moretests/rgy_delete.sh -d >>${LOGFILE} 2>&1

/bin/sh $TET_ROOT/test/security/api/moretests/rgy_person.sh -d >>${LOGFILE} 2>&1

/bin/sh $TET_ROOT/test/security/api/moretests/rgy_group.sh -d >>${LOGFILE} 2>&1

/bin/sh $TET_ROOT/test/security/api/moretests/rgy_org.sh -d >>${LOGFILE} 2>&1

/bin/sh $TET_ROOT/test/security/api/moretests/rgy_account.sh -d >>${LOGFILE} 2>&1

/bin/sh $TET_ROOT/test/security/api/moretests/rgy_member.sh -d >>${LOGFILE} 2>&1

/bin/sh $TET_ROOT/test/security/api/moretests/rgy_misc.sh -d >>${LOGFILE} 2>&1
  
po_setup.sh  >>${LOGFILE} 2>&1
cd ../control
testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/passwd_override.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/passwd_import.tsh >>${LOGFILE} 2>&1

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/sec_salvage_db.tsh >>${LOGFILE} 2>&1

cd $TET_ROOT/test/security/api/moretests  
acl_edit_setup.sh  >>${LOGFILE} 2>&1
cd ../testsh
if [ -f /usr/bin/dce_login -a ! -f /usr/bin/dce_login_noexec ]
then
           ln -s /usr/bin/dce_login /usr/bin/dce_login_noexec >/dev/null
           link=1
fi
export KRB5CCNAME=`dce_login_noexec flintstone yabadabado`

testsh -I $TET_ROOT/test/security/api/control -I $TET_ROOT/test/security/api/moretests -d63 $TET_ROOT/test/security/api/control/acl_edit.tsh  >>${LOGFILE} 2>&1

if ((link == 1))
then
           rm /usr/bin/dce_login_noexec
           link=0
fi
rm /tmp/key_mgmt_test

# kill the acl_server if already running. The acl_edit.sh also kill acl_server
# at the end of the acl_edit test.  So the killing the acl_server here allows
# acl_edit_setup.sh to be run multiple times without starting up a new instance
# of acl_server each time the setup is done.

ps -ef | grep acl_server | grep -v "grep acl_server" | awk '{ print $2 }' | xargs kill -9

grep FAILED $LOGFILE
if [[ $? = 0 ]]
then
        print "FAILED, SECURITY functional test: see $LOGFILE"
        exit 1
fi

grep "[1-99] failed" $LOGFILE
if [[ $? = 0 ]]
then
        print "FAILED, SECURITY functional test: see $LOGFILE \n"
        print "May have core dumped... grep \"[1-99] failed\" $LOGFILE"
        exit 1
else
        print "PASSED, SECURITY functional test: see $LOGFILE"
        exit 0
fi
