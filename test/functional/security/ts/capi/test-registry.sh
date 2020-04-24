#!/bin/sh
#   
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#  
# All Rights Reserved
#    
# HISTORY
# $Log: test-registry.sh,v $
# Revision 1.1.2.1  1996/11/14  16:12:37  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  15:27:36  arvind]
#
# $EndLog$
#  
#
# JC - Aug 20, 1996
# Script to invoke the PKSS tests             
#

echo "**********************************************************"
echo "Invoking test-registry..."
echo "**********************************************************"
echo "Doing dce_login_noexec..."
rm -f dce_login_noexec
ln -s `which dce_login` dce_login_noexec
KRB5CCNAME=`dce_login_noexec -c cell_admin -dce-`
export KRB5CCNAME
klist
#
# ignore any errors here.
#
echo "============================================================"
echo "** IGNORE Any Errors while deleteing principal foo
echo "** This is just a cleanup"
echo "============================================================"
echo "Deleting principal foo.."
rgy_edit < delete_foo
echo "============================================================"
echo "** Creating principal test_capi **, This better work! 
echo "============================================================"
rgy_edit < create_foo
echo "============================================================"
echo "Creating Authn/Cipher ERA schema and attaching it to test_capi's xattrs..."
echo "If the ERA schema already exists, you can ignore the error here as well.."
echo "We don't delete the schema since it may affect existing principals"
echo "other than the test principal test_capi which use this schema"
echo "============================================================"
dcecp < create_era
echo "============================================================"
echo "** STARTING Registry Policy Test **, "
echo "   Any errors now will result in failure"
echo "============================================================"
#
# running first with certified credentials
#
REGISTRY_TEST_ARGUMENTS=direct
export REGISTRY_TEST_ARGUMENTS
echo "Trying <test_registry_tet direct> ......"
./test_registry_tet
echo "RETURNED from <test_registry_tet direct>"
kdestroy
KRB5CCNAME=
export KRB5CCNAME
#
# running without certified credentials
#
KRB5CCNAME=`dce_login_noexec  cell_admin -dce-`
export KRB5CCNAME
klist
REGISTRY_TEST_ARGUMENTS=untrusted
export REGISTRY_TEST_ARGUMENTS
echo "Trying <test_registry_tet untrusted> ......."
./test_registry_tet
echo "RETURNED from <test_registry_tet untrusted>"
echo "CLEANING up after run of Registry Policy Test"
echo "Deleting principal foo.."
rgy_edit < delete_foo
kdestroy
KRB5CCNAME=
export KRB5CCNAME
echo "END of Registry Policy Test"
#EOF
