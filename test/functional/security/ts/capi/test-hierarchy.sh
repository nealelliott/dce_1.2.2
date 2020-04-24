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
# $Log: test-hierarchy.sh,v $
# Revision 1.1.2.1  1996/11/14  16:12:23  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/14  15:24:34  arvind]
#
# $EndLog$
#  
#
# JC - Aug 20, 1996
# Script to invoke the PKSS tests             
#
echo "******************************************************"
echo "Invoking test-hierarchy (Hierarchical Policy Test) ..."
echo "******************************************************"
echo "Doing dce_login_noexec..."
rm -f dce_login_noexec
ln -s `which dce_login` dce_login_noexec
KRB5CCNAME=`dce_login_noexec -c cell_admin -dce-`
export KRB5CCNAME
klist
#
# running first with certified credentials
#
HIERARCHICAL_TEST_ARGUMENTS="pc1 pc2 pc3 pc4 pc5 pc6 pc7 xc1 xc2 xc3 xc7"
export HIERARCHICAL_TEST_ARGUMENTS
echo " ===  Starting a run of test with arguments $HIERARCHICAL_TEST_ARGUMENTS"
./trycase_a_tet
echo "Returned from trycase_a_tet"
echo "******************************************************"
echo "END of Hierarchical Policy Test"
echo "******************************************************"
kdestroy
KRB5CCNAME=
export KRB5CCNAME
#EOF
