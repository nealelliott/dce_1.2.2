#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: passwd_override.sh,v $
# Revision 1.1.8.2  1996/03/11  02:35:02  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:48  marty]
#
# Revision 1.1.8.1  1995/12/11  21:49:21  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:35  root]
# 
# Revision 1.1.6.1  1994/09/21  19:57:51  max
# 	CR 11498 fix.  Fixed shell syntax problems in passwd_override.sh.
# 	These fixes uncovered tests failure and lost sync between
# 	passwd_override.sh and passwd_override.  All uncovered problems were
# 	also fixed.
# 	[1994/09/21  19:57:26  max]
# 
# Revision 1.1.4.2  1992/12/31  17:30:48  htf
# 	Embedding copyright notice
# 	[1992/12/31  16:56:13  htf]
# 
# Revision 1.1.2.4  1992/05/22  20:27:03  cuti
# 	 Check po_setup's status and update group passwd_override test.
# 	[1992/05/22  20:06:11  cuti]
# 
# Revision 1.1.2.3  1992/05/22  14:09:35  cuti
# 	 Add print_unix_info flag to binlogin statements.
# 	[1992/05/22  02:41:05  cuti]
# 
# Revision 1.1.2.2  1992/05/21  21:29:45  cuti
# 	 passwd_override test script.
# 
# 	 passwd-override test shell script.
# 	[1992/05/21  21:09:39  cuti]
# 
# $EndLog$

#set -x       



check_status() 
{ 
#set -x

	if [ "$1" != "$2" ] 
	then
		STATUS=2 
		EXIT=1
		if [ $TRACE = "1" ]
		then
			echo "Trace Message:     \"$3\" failed"
		fi
	fi 	

	if [ $4 = 0 -a $1 = 0 ] 
	then
		shift 4
		auth_src=`cat /tmp/override.out1 | grep auth_src | sed -e "s/[^:]*: //"`


		if [ "$auth_src" != "$1" ]
		then
			STATUS=2 
			EXIT=1
			if [ $TRACE = "1" ]
			then
				echo "Trace Message:     returned data (auth_src: $auth_src) not correct, expected: $1. "
			fi 
		fi
		shift

		pw_uid=`cat /tmp/override.out1 | grep pw_uid | sed -e "s/[^:]*: //"`
		if [ $pw_uid != "$1" ]
		then
			STATUS=2 
			EXIT=1
			if [ $TRACE = "1" ]
			then
				echo "Trace Message:     returned data (pw_uid: $pw_uid) not correct, expected: $1. "
			fi 
		fi
		shift

		pw_gid=`cat /tmp/override.out1 | grep pw_gid | sed -e "s/[^:]*: //"`
		if [ "$pw_gid" != "$1" ]
		then
			STATUS=2 
			EXIT=1
			if [ $TRACE = "1" ]
			then
				echo "Trace Message:     returned data (pw_gid: $pw_gid) not correct, expected: $1. "
			fi 
		fi
		shift

		pw_gecos=`cat /tmp/override.out1 | grep pw_gecos | sed -e "s/[^:]*: //"`
		if [ "$pw_gecos" != "$1" ]
		then
			STATUS=2 
			EXIT=1
			if [ $TRACE = "1" ]
			then
				echo "Trace Message:     returned data (pw_gecos: $pw_gecos) not correct, expected: $1. "
			fi 
		fi
		shift

		pw_dir=`cat /tmp/override.out1 | grep pw_dir | sed -e "s/[^:]*: //"`
		if [ "$pw_dir" != "$1" ]
		then
			STATUS=2 
			EXIT=1
			if [ $TRACE = "1" ]
			then
				echo "Trace Message:     returned data (pw_dir: $pw_dir) not correct, expected: $1. "
			fi 
		fi
		shift

		pw_shell=`cat /tmp/override.out1 | grep pw_shell | sed -e "s/[^:]*: //"`
		if [ "$pw_shell" != "$1" ]
		then
			STATUS=2 
			EXIT=1
			if [ $TRACE = "1" ]
			then
				echo "Trace Message:     returned data (pw_shell: $pw_shell) not correct, expected: $1. "

			fi 
		fi
	fi

}






SFLAG="0"
FAIL="1"
SUCCESS="0"
TRACE="0"
  

for LCV in "$@"
do
  if [ "$LCV" = "-d" ]
  then
    DFLAG="-d"
    continue
  fi
  if [ "$DFLAG" = "-d" ]
  then
    case $LCV in
      "1") FAIL="1" ;;
      "2") SUCCESS="1" ;;
      "3") SUCCESS="1"; FAIL="1" ;;
      "32") TRACE="1" ;;
      "33") TRACE="1"; FAIL="1" ;;
      "34") TRACE="1"; SUCCESS="1" ;;
      "63") TRACE="1"; SUCCESS="1"; FAIL="1" ;;
    esac
    DFLAG="0"
    continue
  fi
  case "$LCV" in
    -d*) DMASK=`echo $LCV | sed 's/^-d//'`
	 case $DMASK in
	   "1") FAIL="1" ;;
	   "2") SUCCESS="1" ;;
	   "3") SUCCESS="1"; FAIL="1" ;;
	   "32") TRACE="1" ;;
	   "33") TRACE="1"; FAIL="1" ;;
	   "34") TRACE="1"; SUCCESS="1" ;;
	   "63") TRACE="1"; SUCCESS="1"; FAIL="1" ;;
	 esac  ;;
    -s) SFLAG="1" ;;
    *[1-9]*) CASEVAL=$LCV ;;
    *[" "]*) LBLSTR=$LCV ;;
  esac
done


#
# Initialize program variables.
#

EXIT=0
create=0  

# Create accounts. 
                   
	if [ $create = 0 ] 
	then
		../moretests/po_setup.sh > /tmp/po_setup.out

        grep "binding is not authenticated" /tmp/po_setup.out > /dev/null
		if [ $? = 0 ]
		then
			echo "Please do login to get the authentication:\"binding is not authenticated\". "
			exit 1 
		else
			create=1
		fi  
	fi


  
#
# Setup passwd_override header for this run.
#
      

echo ""
echo "---------------------------------------------------------------"
echo "Routine Tested:    passwd_override test"
echo "Tested for:        $CASEVAL $LBLSTR"
echo""
     
#
# Send a copy of the output to stderr if -s is specified on command line.
#

if [ $SFLAG = "1" ]
then
  echo "" 1>&2
  echo "---------------------------------------------------------------" 1>&2
  echo "Routine Tested:    acl_edit command line" 1>&2
  echo "Tested for:        $CASEVAL $LBLSTR" 1>&2
  echo"" 1>&2
fi



#
# Select passwd_override test.
#

case "$CASEVAL" in

#
# setup: Test if the account can login without override file.
#

	"1" )   
	
		echo "*** Setup test without override file" > /tmp/override.out


# Remove passwd_override file
 
		mv  /opt/dcelocal/etc/passwd_override  /opt/dcelocal/etc/passwd_override.bak
   
# Do login without override file.

		../moretests/binlogin zxcvb -dce- 1 >> /tmp/override.out
	   	check_status $?  0 "binlogin zxcvb -dce-" 1

		../moretests/binlogin asdf -dce- 1 >> /tmp/override.out
	   	check_status $? 0 "binlogin asdf -dce-" 1
	
		../moretests/binlogin test1 -dce- 1 >> /tmp/override.out
	   	check_status $? 0 "binlogin test1 -dce-" 1

		../moretests/binlogin test2 -dce- 1 >> /tmp/override.out
	   	check_status $? 0 "binlogin test2 -dce-" 1

		../moretests/binlogin test3 -dce- 1 >> /tmp/override.out
	   	check_status $? 0 "binlogin test3 -dce-" 1

		../moretests/binlogin test4 -dce- 1 >> /tmp/override.out
 	   	check_status $? 0 "binlogin test4 -dce-" 1

		../moretests/binlogin poiuy -dce- 1  >> /tmp/override.out
	   	check_status $? 0 "binlogin poiuy -dce-" 1

		../moretests/binlogin asasdfhgjl -dce- 1 >> /tmp/override.out
	   	check_status $? 0 "binlogin asasdfhgjl -dce-" 1
 
		../moretests/binlogin test5 -dce- 1 >> /tmp/override.out
	   	check_status $? 0 "binlogin test5 -dce-" 1

		../moretests/binlogin root -dce- 1 >> /tmp/override.out
	   	check_status $? 0 "binlogin root -dce-"  1


	;;   



	"2" ) 
#
# Install passwd_override file to  /opt/dcelocal/etc/passwd_override
#pname:passwd:uid:gid:gecos:home_dir:shell
#
#zxcvb:*:14:12:::                           # p cant' login
#asdf:OMIT:::/tmp:/usr/games:/bin/csh       # p cant' login
#:OMIT:18::::                               # group (tcb) cant' login, test1,test2, test3
#
#::::/tmp:/usr/games:/bin/csh               # g can't login
#poiuy::home:::/usr:/bin/ksh                # wrong syntex
#asasdfhgjl:alshglahl                       # sec_clientd ignore this entry, so orignal passwd works.
#test4:ZUQSMwH60M1Ho:2345:1111:override passwd:/usr/games:/bin/ksh  # p change passwd to -dce0-
#test5:::::/user/test5:/bin/ksh             # overriden home_dir and shell field.
#root::::root privelege:/games:/bin/ksh     # overriden gecos, home_dir and shell field.
#

        echo " "  >> /tmp/override.out
		echo "*** test with override file "  >> /tmp/override.out

  
		sed -e "/[\/ ]\*.*/d" -e "/^2/d" ../moretests/passwd_override | sed -e "1d" -e "s/^1//"  > /opt/dcelocal/etc/passwd_override
	                  
		../moretests/binlogin zxcvb -dce- 1 >> /tmp/override.out
	   	check_status $? 1 "binlogin zxcvb -dce-" 1

		../moretests/binlogin asdf -dce- 1 >> /tmp/override.out
	   	check_status $? 1 "binlogin asdf -dce-" 1
	
		../moretests/binlogin test4 -dce- 1 >> /tmp/override.out
 	   	check_status $? 1 "binlogin test4 -dce-" 1

		../moretests/binlogin test4 -dce0- 1  | tee /tmp/override.out1   >> /tmp/override.out
 	   	check_status $? 0 "binlogin test4 -dce-" 0 2 1053 1111 "override passwd" /usr/games /bin/ksh

		../moretests/binlogin test1 -dce-  1 >> /tmp/override.out
	   	check_status $? 1 "binlogin test1 -dce-" 1

		../moretests/binlogin test2 -dce- 1 >> /tmp/override.out
	   	check_status $? 1 "binlogin test2 -dce-" 1

		../moretests/binlogin test3 -dce- 1 >> /tmp/override.out
	   	check_status $? 1 "binlogin test3 -dce-"  1

		../moretests/binlogin poiuy -dce- 1  | tee /tmp/override.out1 >> /tmp/override.out
	   	check_status $? 0 "binlogin poiuy -dce-" 0 0 1057 12 "" / ""

		../moretests/binlogin asasdfhgjl -dce- 1 >> /tmp/override.out
	   	check_status $? 0 "binlogin asasdfhgjl -dce-" 1

		../moretests/binlogin test5 -dce-  1 | tee /tmp/override.out1 >> /tmp/override.out
	   	check_status $? 0 "binlogin test5 -dce-" 0 0 1054 12 " " /user/test5 /bin/ksh

		../moretests/binlogin root -dce- 1 | tee /tmp/override.out1 >> /tmp/override.out
	   	check_status $? 0 "binlogin root -dce-" 0 0 0 0 "root privilege" /games /bin/ksh
	;;	
     

#
# Modify passwd_override fiel to see if sec_clientd reread passwd_override file
#

	"3" )

        echo " "  >> /tmp/override.out
		echo "*** test with override file updated " >> /tmp/override.out

		sed -e "/[\/ ]\*.*/d" -e "/^1/d" ../moretests/passwd_override | sed -e "1d" -e "s/^2//"  > /opt/dcelocal/etc/passwd_override

		../moretests/binlogin zxcvb -dce- 1 >> /tmp/override.out
	   	check_status $? 1 "binlogin zxcvb -dce-" 1

		../moretests/binlogin asdf -dce- 1 >> /tmp/override.out
	   	check_status $? 1 "binlogin asdf -dce-" 1
	
		../moretests/binlogin test4 -dce- 1  >> /tmp/override.out
 	   	check_status $? 1 "binlogin test4 -dce-" 1

		../moretests/binlogin test4 -dce0- 1 | tee /tmp/override.out1   >> /tmp/override.out
 	   	check_status $? 0 "binlogin test4 -dce0-" 0 2 1053 1111 "overr passwd" /usr/games /bin/ksh

		../moretests/binlogin test1 -dce- 1 >> /tmp/override.out
	   	check_status $? 1 "binlogin test1 -dce-" 1

		../moretests/binlogin test2 -dce- 1 >> /tmp/override.out
	   	check_status $? 1 "binlogin test2 -dce-" 1

		../moretests/binlogin test3 -dce- 1 >> /tmp/override.out
	   	check_status $? 1 "binlogin test3 -dce-" 1

		../moretests/binlogin poiuy -dce- 1 | tee /tmp/override.out1  >> /tmp/override.out
	   	check_status $? 0 "binlogin poiuy -dce-" 0 0 1057 12 "" / ""

		../moretests/binlogin asasdfhgjl -dce- 1 >> /tmp/override.out
	   	check_status $? 0 "binlogin asasdfhgjl -dce-" 1

		../moretests/binlogin test5 -dce- 1 | tee /tmp/override.out1 >> /tmp/override.out
	   	check_status $? 0 "binlogin test5 -dce-" 0 0 1054 12 "" /u/test5 /bin/csh

		../moretests/binlogin root -dce- 1 | tee /tmp/override.out1 >> /tmp/override.out
	   	check_status $? 0 "binlogin root -dce-" 0  0 0 0 "root priv" /games /bin/csh
	;;	
  
	
esac   



	
#
# Report acl_edit test results.
#

if [ $FAIL = "1" ]
then

  case $STATUS in

    "1")

    echo "Failure Message:   Setup failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   Setup failed with error $EXIT" 1>&2
    fi

    ;; # End case 1

    "2")

    echo "Failure Message:   passwd_override test case failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   passwd_override test case failed with error $EXIT" 1>&2
    fi

    ;; # End case 2

    "3")

    echo "Failure Message:   Cleanup failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   Cleanup failed with error $EXIT" 1>&2
    fi

    ;; # End case 3

  esac # End case $STATUS in

fi # End if [ $FAIL = "1" ]

if [ $SUCCESS = "1" -a $EXIT -eq 0 ]
then
  echo "Test Status:       Test SUCCEEDED"
  if [ $SFLAG = "1" ]
  then
    echo "Test Status:       Test SUCCEEDED" 1>&2
  fi
fi

if [ $FAIL = "1" -a $EXIT -gt 0 ]
then
  echo "Test Status:       Test FAILED"
  if [ $SFLAG = "1" ]
  then
    echo "Test Status:       Test FAILED" 1>&2
  fi
fi

#
# Exit this shell and return error code.
#
exit $EXIT

#
# End script acl_edit.sh
#

