#!/bin/ksh 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: all_sec.sh,v $
# Revision 1.1.6.2  1996/03/11  02:30:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:01:33  marty]
#
# Revision 1.1.6.1  1995/12/11  20:16:22  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:49:32  root]
# 
# Revision 1.1.4.2  1994/08/09  21:22:21  bhaim
# 	moved acl_server to test tree
# 	[1994/08/09  21:20:42  bhaim]
# 
# Revision 1.1.4.1  1994/06/10  20:44:57  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:16:42  devsrc]
# 
# Revision 1.1.2.2  1993/01/22  21:50:54  sekhar
# 	[OT 6973]  Shell wrapper to execute most of the security functional tests.
# 	[1993/01/22  21:31:35  sekhar]
# 
# $EndLog$
# Useage:
#       sec_func.sh <cell_admin> <cell_admin_pw> <cell_name> <test_case> <test_case_dependent_args>

usagefile=/tmp/usage_$$

cat > $usagefile << !EOF

Useage: sec_func.sh <cell_admin_name> <cell_admin_pw> <cell_name> \ 
                    <test_case> <test_case_dependent_args>
     
        <test_case> -> | api      (tests security api)
                       | rgy_edit (tests rgy_edit commands)
                       | acl_edit (tests acl_edit commands)
                       | update   (tests updating of registry database)
                       | query    (tests reads from registry database)
                       | passwd_import
                       | passwd_override
                       | all      (runs all of the above)

        <test_case_dependent_args> ->
               acl_server (required by acl_edit and "all")

N.B.

1. Before running this script, start with a clean registry i.e.
   the registry only contains entries created by cell configuration.

2. passwd_import MUST be run first BEFORE any other functional tests
   are run since this test requires a clean registry.

3. The tests do not clean up the registry database. So some entries
   will be left in the registry database.

4. The acl_server used by acl_edit is built as part of the dce source 
   tree and is located in :
   
   <dce-root-dir>/obj/<machine>/test/security/svr_example/acl_server

!EOF

if [ $# -lt 4 ]
then
     cat $usagefile
     rm  $usagefile
     exit
fi

CELL_ADMIN=$1
CELL_PW=$2
CELL_NAME=$3
test_case=$4

if [ $test_case = "acl_edit" -a  $# -ne 5  -o  $test_case = "all" -a  $# -ne 5 ] 
then
    cat $usagefile
    rm  $usagefile
    exit
else
    ACL_SERVER=$5
fi
  
DCE_LOGIN=/opt/dcelocal/bin/dce_login

if [ `whoami` != root ]
then
    echo "You must be logged in as root on the local system for running this test"
    exit
fi

testdir=`pwd`
datesfile=`pwd`/dates.`date +%m_%d_%y`_$
# just in case the files exist remove them
rm -f $datesfile
touch $datesfile

#  passwd_import MUST be the first in the list so that it is executed
#  before any of the other tests.

if [ $test_case = all ]
then
    cat > /tmp/test_list << !EOF
passwd_import
api
rgy_edit
acl_edit
passwd_override
update
query
!EOF
else
    cat > /tmp/test_list << !EOF
$test_case
!EOF
fi

cat /tmp/test_list

for test_case in `cat /tmp/test_list`
do

    case $test_case in

    "api")  cat << !EOF

++++++++++++  Begin setup for API tests  ++++++++++++++++++++++++

!EOF

        set -x
        cd $testdir/api/moretests
        chmod +x *.sh
        chmod u+s binlogin
        chown root binlogin
        $DCE_LOGIN $CELL_ADMIN $CELL_PW -exec rgy_setup.sh
        $DCE_LOGIN $CELL_ADMIN $CELL_PW -exec key_mgmt_setup.sh
        CELLADMIN=$CELL_ADMIN
        export CELLADMIN
        set +x

        cat << !EOF

++++++++++++ End setup for API tests  ++++++++++++++++++++++++++++

!EOF

        cd $testdir/api/control

        cat << !EOF

%%%%%%%%%%%%%%% most_sec.tsh  %%%%%%%%%%%%%%%%%%

!EOF

        echo "most_unix.tsh  ......... began at " `date` >> $datesfile

        echo "$DCE_LOGIN $CELL_ADMIN $CELL_PW -exec ../testsh/testsh -I../moretests most_sec.tsh"
        $DCE_LOGIN $CELL_ADMIN $CELL_PW -exec ../testsh/testsh -I../moretests most_sec.tsh

        echo ""
        echo "most_unix.tsh  ......... ended at " `date` >> $datesfile
        echo "" >> $datesfile
        ;; 

    "acl_edit") 
        cat << !EOF

++++++++++++  Begin setup for acl_edit command test  ++++++++++++++

!EOF

        set -x
        cd $testdir/api/moretests
        if [ ! -f $testdir/api/moretests/acl_server ]
        then
            echo ".... acl_server not found in the current directory"
            echo ".... Copying acl_server from " $ACL_SERVER
            cp $ACL_SERVER .
            chmod +x acl_server
        fi
        $DCE_LOGIN $CELL_ADMIN $CELL_PW -exec acl_edit_setup.sh
        set +x
 
        cat << !EOF

++++++++++++  End setup for acl_edit command test  ++++++++++++++++

!EOF

        cd $testdir/api/control
        cat << !EOF

%%%%%%%%%%%%%%%%%%%% acl_edit.tsh %%%%%%%%%%%%%%%%%%%%%%%%

!EOF
        echo "acl_edit.tsh ........... began at" `date` >> $datesfile

        echo "$DCE_LOGIN flintstone yabadabado -exec ../testsh/testsh -I../moretests acl_edit.tsh"
        $DCE_LOGIN flintstone yabadabado -exec ../testsh/testsh -I../moretests acl_edit.tsh

        echo ""
        echo "acl_edit.tsh ........... ended at" `date` >> $datesfile
        echo "" >> $datesfile
        ;;

    "passwd_import")
        set -x
        cd $testdir/api/moretests
        chmod +x *.sh
        chmod u+s binlogin
        chown root binlogin
        CELLADMIN=$CELL_ADMIN
        export CELLADMIN
        set +x

        cd $testdir/api/control
        cat <<!EOF

%%%%%%%%%%%%%%%%% passwd_import.tsh %%%%%%%%%%%%%%%%%%%%%%

!EOF
        echo "passwd_import.tsh ...... began at" `date` >> $datesfile
        echo "$DCE_LOGIN $CELL_ADMIN $CELL_PW -exec ../testsh/testsh -I../moretests passwd_import.tsh"

        $DCE_LOGIN $CELL_ADMIN $CELL_PW -exec ../testsh/testsh -I../moretests passwd_import.tsh

        echo ""
        echo "passwd_import.tsh ...... ended at" `date` >> $datesfile
        echo "" >> $datesfile
        ;;

    "update")
        cd $testdir/commands/rgy
        pwd
        cat <<!EOF

%%%%%%%%%%%%%%%%%%%%%%% update  %%%%%%%%%%%%%%%%%%%%%%%%%%

!EOF
        echo "update ................. began at" `date` >> $datesfile
        echo "./update -a 50 $CELL_NAME -p $CELL_ADMIN -pw $CELL_PW"

        ./update -a 50 $CELL_NAME -p $CELL_ADMIN -pw $CELL_PW

        echo ""
        echo "update ................. ended at" `date` >> $datesfile
        echo "" >> $datesfile
        ;;

    "query")
        cd $testdir/commands/rgy
        cat <<!EOF

%%%%%%%%%%%%%%%%%%%%%%% query   %%%%%%%%%%%%%%%%%%%%%%%%%%

!EOF
        echo "query  ................. began at" `date` >> $datesfile
        echo "$DCE_LOGIN $CELL_ADMIN $CELL_PW -exec ./query $CELL_NAME"

        $DCE_LOGIN $CELL_ADMIN $CELL_PW -exec ./query $CELL_NAME

        echo ""
        echo "query  ................. ended at" `date` >> $datesfile
        echo "" >> $datesfile

        cd $testdir/commands/rgy

        cat <<!EOF

%%%%%%%  update - remove entries from registry %%%%%%%%%%%

!EOF
        echo "update ................. began at" `date` >> $datesfile
        echo "./update -r 50 $CELL_NAME -p $CELL_ADMIN -pw $CELL_PW"

        ./update -r 50 $CELL_NAME -p $CELL_ADMIN -pw $CELL_PW

        echo ""
        echo "update ................. ended at" `date` >> $datesfile
        echo "" >> $datesfile
        ;;

    "rgy_edit")
        cd $testdir/api/moretests
        cat << !EOF

%%%%%%%%%%%%%%%%%%%% rgy_edit.sh %%%%%%%%%%%%%%%%%%%%%%%%

!EOF
        echo "rgy_edit ............... began at" `date` >> $datesfile
        echo "$DCE_LOGIN $CELL_AMDIN $CELL_PW -exec all_rgy_edit.sh -d2"

        $DCE_LOGIN $CELL_ADMIN $CELL_PW -exec all_rgy_edit.sh -d2

        echo ""
        echo "rgy_edit ............... ended at" `date` >> $datesfile
        echo ""  >> $datesfile
        ;;

    "passwd_override")

        set -x
        cd $testdir/api/moretests
        chmod +x *.sh
        chmod u+s binlogin
        chown root binlogin
        set +x

        cd $testdir/api/control
        cat << !EOF

%%%%%%%%%%%%%%%%% passwd_override.tsh %%%%%%%%%%%%%%%%%%%%%

!EOF
        echo "passwd_override ........ began at" `date` >> $datesfile
        echo "$DCE_LOGIN $CELL_ADMIN $CELL_PW -exec ../testsh/testsh -I../moretests passwd_override.tsh"

        $DCE_LOGIN $CELL_ADMIN $CELL_PW -exec ../testsh/testsh -I../moretests passwd_override.tsh

        echo ""
        echo "passwd_override ........ ended at" `date` >> $datesfile
        echo "" >> $datesfile
    esac
done

cat << !EOF

Summary of timing information for tests
----------------------------------------

!EOF

cat $datesfile
rm -f $datesfile $usagefile
