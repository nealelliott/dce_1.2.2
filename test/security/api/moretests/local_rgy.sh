#!/bin/sh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY 
# $Log: local_rgy.sh,v $
# Revision 1.1.6.2  1996/03/11  02:34:36  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:32  marty]
#
# Revision 1.1.6.1  1995/12/11  21:48:40  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/6  1995/02/09  17:20 UTC  mullan_s
# 	Login as root to initialie local rgy in testcase 2.
# 
# 	HP revision /main/HPDCE02/6  1995/02/09  17:19 UTC  mullan_s
# 
# 	HP revision /main/HPDCE02/mullan_mothra_lrgy_fix/1  1995/02/09  16:57 UTC  mullan_s
# 	For testcase 2, login as root to create local rgy. Also,
# 	create unique test logfiles for each testcase.
# 
# 	HP revision /main/HPDCE02/5  1995/02/02  20:18 UTC  mdf
# 	Fix .
# 
# 	HP revision /main/HPDCE02/mdf_MOTHRA_5/1  1995/02/02  20:16 UTC  mdf
# 	Fix so that the nigthly tests work.
# 
# 	HP revision /main/HPDCE02/4  1995/01/11  20:30 UTC  mdf
# 	comment
# 
# 	HP revision /main/mdf_MOTHRA_0/1  1994/12/01  15:52 UTC  mdf
# 	Added callout to cleanup routine.
# 
# 	HP revision /main/HPDCE02/3  1994/11/30  19:02 UTC  mgm
# 	Change from 4 to 7
# 
# 	HP revision /main/HPDCE02/2  1994/11/30  18:50 UTC  mgm
# 	This is way too long... change from 15 users to 4
# 
# 	HP revision /main/HPDCE02/1  1994/11/15  22:07 UTC  mullan_s
# 	Merge.
# 
# 	HP revision /main/HPDCE01/1  1994/01/31  18:09  mdf
# 	merged forward
# 	[1995/12/11  20:52:22  root]
# 
# 	Initial King Kong branch
# 	[1993/07/09  10:38:50  root]
# 
# 	Embedding copyright notice
# 	[1992/12/31  16:54:52  htf]
# 
# Revision 1.1.3.2  1993/05/17  15:16:14  cuti
# 	Add SERVER_MODE, so this test can be run automically if test and secd on the same host.
# 	[1993/05/17  15:14:48  cuti]
# 
# 	Add one blank output line for easy reading.
# 	[1992/06/16  16:26:06  cuti]
# 
# 	Modified the way secd is started to accomodate server/client model.
# 	[1992/06/16  16:17:33  cuti]
# 
# Revision 1.1.2.3  1992/05/28  20:12:40  cuti
# 	Converted to testsh format for testing.
# 	[1992/05/28  18:31:04  cuti]
# 
# Revision 1.1.2.2  1992/05/19  21:27:08  cuti
# 	 Add ./ to local executible files
# 	[1992/05/19  20:23:31  cuti]
# 
# 	 Created to test local registry.
# 	[1992/05/19  19:26:36  cuti]
# 
# Revision 1.1.1.8  1993/05/04  22:15:03  mgm
# 	    Loading drop DCE1_0_2FNL
# 
# $EndLog$

#set -x

check_status() 
{ 
#set -x

	if [ "$1" -ne "$2" ] 
	then
		STATUS=2 
		EXIT=1
		if [ $TRACE = "1" ]
		then
			echo "Trace Message:     \"$3\" failed, returnd : $1, expected: $2"
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
MODE=${SERVER_MODE-/server_remote}

#
# Setup local_rgy logfile for this run.
#

echo ""
echo "---------------------------------------------------------------"
echo "Routine Tested:    local registry"
echo "Tested for:        $CASEVAL $LBLSTR"
echo""

 
#
# Send a copy of the output to stderr if -s is specified on command line.
#

if [ $SFLAG = "1" ]
then
  echo "" 1>&2
  echo "---------------------------------------------------------------" 1>&2
  echo "Routine Tested:    local registry" 1>&2
  echo "Tested for:        $CASEVAL $LBLSTR" 1>&2
  echo"" 1>&2
fi

# You need to login as cell_admin to be able to add accounts.
# Setup 30 accounts.
#     
../moretests/local_rgy_setup.sh  > /tmp/local_rgy$CASEVAL.out

case "$CASEVAL" in

  "1")
     
    if [ $MODE = server_remote ]
    then
	echo "NOTE: This test will kill secd twice and prompt you to start secd on the"
	echo "server node twice MANUALLY."
	echo " "
    fi
	
    


# login as root to start local rgy, so you can view and change the property. 
		if [ $TRACE = "1" ]
		then
			echo " \"binlogin root -dce-\" to start local rgy's DB"
		fi
		../moretests/binlogin root -dce- 
             
# remove root and change max_entry to 10.
        
        echo "v" > /tmp/local_rgy.script
        echo "pr" >> /tmp/local_rgy.script
        echo "y" >> /tmp/local_rgy.script
        echo "25" >> /tmp/local_rgy.script
        echo "1s" >> /tmp/local_rgy.script # change life span to 1s.
        echo "pu" >> /tmp/local_rgy.script # purge all the extries.
        echo "v"  >> /tmp/local_rgy.script
        echo "pr" >> /tmp/local_rgy.script
        echo "y"  >> /tmp/local_rgy.script
        echo "10" >> /tmp/local_rgy.script # change max_entries to 10
        echo "1d" >> /tmp/local_rgy.script
        echo "pr" >> /tmp/local_rgy.script
        echo "n"  >> /tmp/local_rgy.script
        echo "q"  >> /tmp/local_rgy.script 
               
		if [ $TRACE = "1" ]
		then
       		echo " Run local rgy_edit"
		fi
       	rgy_edit -l < /tmp/local_rgy.script  >> /tmp/local_rgy$CASEVAL.out 
        
# login 4 users.
               
		if [ $TRACE = "1" ]
		then
			echo " Login 4 users"           
		fi
		../moretests/local_login.sh 2 >> /tmp/local_rgy$CASEVAL.out
                    
# kill secd 
		if [ $TRACE = "1" ]
		then
			echo " Kill secd"
		fi
		../moretests/kill_secd.sh >> /tmp/local_rgy$CASEVAL.out 
                   
# Verify local rgy is working.
		if [ $TRACE = "1" ]
		then
        	echo " Verify local rgy is working." 
		fi
	    ../moretests/binlogin dineen  -dce6- >> /tmp/local_rgy$CASEVAL.out
		check_status $? 1 "binlogin dineen  -dce6-"
        ../moretests/binlogin wyant  -dce7- >> /tmp/local_rgy$CASEVAL.out
		check_status $? 1 "binlogin wyant  -dce7-"
        ../moretests/binlogin pcl  -dce8- >> /tmp/local_rgy$CASEVAL.out
		check_status $? 1 "binlogin pcl  -dce8-"
        ../moretests/binlogin emartin  -dce9- >> /tmp/local_rgy$CASEVAL.out
		check_status $? 1 "binlogin emartin  -dce9-"
        ../moretests/binlogin nacey  -dce10- >> /tmp/local_rgy$CASEVAL.out
		check_status $? 1 "binlogin nacey  -dce10-"
#        ../moretests/binlogin dale  -dce11-  >> /tmp/local_rgy$CASEVAL.out
#		check_status $? 0 "binlogin dale  -dce11-"
#        ../moretests/binlogin markar -dce12- >> /tmp/local_rgy$CASEVAL.out
#		check_status $? 0 "binlogin markar  -dce12-"
#        ../moretests/binlogin greg  -dce13- >> /tmp/local_rgy$CASEVAL.out
#		check_status $? 0 "binlogin greg  -dce13-"
#        ../moretests/binlogin burati -dce14- >> /tmp/local_rgy$CASEVAL.out
#		check_status $? 0 "binlogin burati  -dce14-"
#        ../moretests/binlogin ahop  -dce15- >> /tmp/local_rgy$CASEVAL.out
#		check_status $? 0 "binlogin ahop  -dce15-"
#        ../moretests/binlogin sommerfeld  -dce16- >> /tmp/local_rgy$CASEVAL.out
#		check_status $? 0 "binlogin sommerfeld  -dce16-"
#        ../moretests/binlogin tbl  -dce17-  >> /tmp/local_rgy$CASEVAL.out
#		check_status $? 0 "binlogin tbl  -dce17-"
#        ../moretests/binlogin frisco  -dce18- >> /tmp/local_rgy$CASEVAL.out
#		check_status $? 0 "binlogin frisco  -dce18-"
        ../moretests/binlogin mk  -dce19- >> /tmp/local_rgy$CASEVAL.out
		check_status $? 0 "binlogin mk  -dce19-"
        ../moretests/binlogin cuti  -dce20-  >> /tmp/local_rgy$CASEVAL.out
		check_status $? 0 "binlogin cuti  -dce20-" 


# Start secd .
          
	if [ $MODE = "server_remote" ]
	then
		echo "Please start secd on the server node, then hit return to continue"
                echo "NOTE:  If secd is not running, the test will fail. "

		read answer

	else
            if [ $TRACE = "1" ]
            then
                       echo " Start secd"
            fi
            ST=1
            CNT=0

            while [ $ST -ne 0 -a $CNT -lt 3 ]
            do
               secd
               CNT=$CNT+1

# Make sure secd is running.

               ps -e | grep secd | grep -v "grep secd" >/dev/null
               ST=$?
               if [ $ST -ne 0 ]
               then
                   if [ $TRACE = "1" ]
                   then
                      echo " SECD is not running."
                   fi
               fi
            done

            if [ $ST = 0 ]
            then
                if [ $TRACE = "1" ]
                    then
                    echo " SECD is running"
                 fi
             else
                       EXIT=1
                       echo " SECD is still not running after three try.  exit."
                       exit 1
               fi

	fi
;; 


	"2")

# login as root to start local rgy, so you can view and change the property.
                if [ $TRACE = "1" ]
                then
                        echo " \"binlogin root -dce-\" to start local rgy's DB"
                fi
                ../moretests/binlogin root -dce-
 
# remove all the entries in local rgy, reset max_entries to 5.
        echo "v" > /tmp/local_rgy.script
        echo "pr" >> /tmp/local_rgy.script
        echo "y" >> /tmp/local_rgy.script
        echo "25" >> /tmp/local_rgy.script
        echo "1s" >> /tmp/local_rgy.script
        echo "pu" >> /tmp/local_rgy.script
        echo "v"  >> /tmp/local_rgy.script
        echo "pr" >> /tmp/local_rgy.script
        echo "y"  >> /tmp/local_rgy.script
        echo "5" >> /tmp/local_rgy.script
        echo "1d" >> /tmp/local_rgy.script
        echo "pr" >> /tmp/local_rgy.script
        echo "n"  >> /tmp/local_rgy.script
        echo "q"  >> /tmp/local_rgy.script 
          
		if [ $TRACE = "1" ]
		then
        	echo " Run local rgy_edit to reset max_entries to 5"
		fi
        rgy_edit -l < /tmp/local_rgy.script  >> /tmp/local_rgy$CASEVAL.out 
        

                                                                   
# login 10 users.
		if [ $TRACE = "1" ]
		then
        	echo " Login 10 users"
		fi
       	../moretests/local_login.sh 3 >> /tmp/local_rgy$CASEVAL.out      
        
#  kill secd
		if [ $TRACE = "1" ]
		then
			echo " Kill secd"
		fi
       	../moretests/kill_secd.sh  >> /tmp/local_rgy$CASEVAL.out
             
# Verify local rgy is working.
		if [ $TRACE = "1" ]
		then
       		echo " Verify local rgy is working." 
		fi
		../moretests/binlogin flintstone yabadabado >> /tmp/local_rgy$CASEVAL.out
       	check_status $? 1 "binlogin flintstone yabadabado"
        ../moretests/binlogin a tida >> /tmp/local_rgy$CASEVAL.out
       	check_status $? 1 "binlogin a tida"
        ../moretests/binlogin abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz \
  			abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz  >> /tmp/local_rgy$CASEVAL.out
       	check_status $? 1 "binlogin abcde...  ......"
        ../moretests/binlogin mishkin  -dce4-  >> /tmp/local_rgy$CASEVAL.out
       	check_status $? 1 "binlogin mishkin  -dce4"
        ../moretests/binlogin pato  -dce5- >> /tmp/local_rgy$CASEVAL.out
       	check_status $? 1 "binlogin pato  -dce5"
        ../moretests/binlogin dineen  -dce6- >> /tmp/local_rgy$CASEVAL.out
   	 	check_status $? 0 "binlogin dineen  -dce6"
        ../moretests/binlogin wyant  -dce7- >> /tmp/local_rgy$CASEVAL.out
   	 	check_status $? 0 "binlogin wyant  -dce7-"
        ../moretests/binlogin pcl  -dce8- >> /tmp/local_rgy$CASEVAL.out
   	 	check_status $? 0 "binlogin pcl  -dce8-"
        ../moretests/binlogin emartin  -dce9- >> /tmp/local_rgy$CASEVAL.out
   	 	check_status $? 0 "binlogin emartin  -dce9-"
        ../moretests/binlogin nacey  -dce10- >> /tmp/local_rgy$CASEVAL.out
   	 	check_status $? 0 "binlogin nacey  -dce10-"

#       	../moretests/local_login.sh 3      
#        
#             
#       	../moretests/binlogin pato  -dce5- 
#       	check_status $? 0 "binlogin pato  -dce5"
#        
#               
#        
#       	../moretests/binlogin dineen  -dce6-
#   	 	check_status $? 1 "binlogin dineen  -dce10-"
        
		if [ $TRACE = "1" ]
		then
        	echo " Run local rgy_edit"
		fi
        
        echo "v" > /tmp/local_rgy.script
        echo "q"  >> /tmp/local_rgy.script 
                                           
        
        rgy_edit -l < /tmp/local_rgy.script  >> /tmp/local_rgy$CASEVAL.out 
      
# Start secd .

        if [ $MODE = "server_remote" ]
        then
                echo "Please start secd on the server node, then hit return to continue"
                echo "NOTE:  If secd is not running, the test will fail. "

                read answer

        else
            if [ $TRACE = "1" ]
            then
                       echo " Start secd"
            fi
            ST=1
            CNT=0

            while [ $ST -ne 0 -a $CNT -lt 3 ]
            do
               secd
               CNT=$CNT+1

# Make sure secd is running.

               ps -e | grep secd | grep -v "grep secd" >/dev/null
               ST=$?
               if [ $ST -ne 0 ]
               then
                   if [ $TRACE = "1" ]
                   then
                      echo " SECD is not running."
                   fi
               fi
            done

            if [ $ST = 0 ]
            then
                if [ $TRACE = "1" ]
                    then
                    echo " SECD is running"
                 fi
             else
                       EXIT=1
                       echo " SECD is still not running after three try.  exit."
                       exit 1
               fi

        fi



	;;     


	esac

../moretests/local_rgy_cleanup.sh  >> /tmp/local_rgy$CASEVAL.out

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

    echo "Failure Message:   local registry test case failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   local registry test case failed with error $EXIT" 1>&2
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
