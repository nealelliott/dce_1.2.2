#!/bin/sh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: salvager.sh,v $
# Revision 1.1.8.2  1996/03/11  02:35:37  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:04  marty]
#
# Revision 1.1.8.1  1995/12/11  21:50:02  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:52:56  root]
# 
# Revision 1.1.6.1  1994/03/31  19:57:05  hanfei
# 	add -sort option since sort is not longer a default behavior.
# 	[1994/03/31  19:53:43  hanfei]
# 
# Revision 1.1.4.2  1993/08/05  21:15:20  cuti
# 	Nothing changed
# 	[1993/08/05  19:34:42  cuti]
# 
# 	Change all Unix_Key (include CIPHER) to XXX and Auth_Key_Pepper to XXX.
# 	[1993/08/05  18:53:39  cuti]
# 
# Revision 1.1.2.5  1993/04/01  14:38:16  cuti
# 	Change diff -r to diff.
# 	[1993/04/01  14:37:22  cuti]
# 
# Revision 1.1.2.4  1993/03/31  22:34:28  cuti
# 	Add mkdir if dbpath or prtpaht has not exists yet.
# 	[1993/03/31  22:33:42  cuti]
# 
# Revision 1.1.2.3  1993/03/31  22:09:30  cuti
# 	Change dircmp to "diff -r",
# 	[1993/03/31  22:08:17  cuti]
# 
# Revision 1.1.2.2  1993/03/31  21:10:58  cuti
# 	Initial creation
# 	[1993/03/31  20:37:31  cuti]
# 
# $EndLog$
#
#
#
#
# Copyright 1991 by Hewlett-Packard Co.
#
# sec_salvage_db test.
#     NB: You need to be a local root account to run this test, since
#	  sec_salvage_db is a privileged command.  


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
#set -x

EXIT=0
STATUS=0
PI_TMP=${TMPDIR-/tmp}
# You can change the definition of DIFF to dircmp or whatever is suitable.
DIFF=dircmp

# setup sec_salvage_db logfile for this run.
#

echo ""
echo "---------------------------------------------------------------"
echo "Command Tested:    sec_salvage_db"
echo "Tested for:        $CASEVAL $LBLSTR"
echo""


#
# Send a copy of the output to stderr if -s is specified on command line.
#

if [ $SFLAG = "1" ]
then
  echo "" 1>&2
  echo "---------------------------------------------------------------" 1>&2
  echo "Command Tested:    sec_salvage_db" 1>&2
  echo "Tested for:        $CASEVAL $LBLSTR" 1>&2
  echo"" 1>&2
fi

#
# Select sec_salvage_db test.
#

case "$CASEVAL" in
#
# Test the sec_salvage_db -print and -reconstruct  command line option.
#

  "1")
    # Remove old data.
    if [ -d $PI_TMP/salvager_print_test ]
    then
        rm -rf $PI_TMP/salvager_print_test/*
    else
	mkdir $PI_TMP/salvager_print_test
    fi

    if [ -d $PI_TMP/salvager_rec_test ]
    then
        rm -rf $PI_TMP/salvager_rec_test/*
    else
	mkdir $PI_TMP/salvager_rec_test
    fi

    rm -f $PI_TMP/salvager_print.out

    if [ $TRACE = "1" ]
    then
        echo "Trace Message: sec_create_db -alt_dir $PI_TMP/salvager_print_test -my salvager_test"
        echo " "
    fi

    echo "sec_create_db -alt_dir $PI_TMP/salvager_print_test -my salvager_test" > $PI_TMP/salvager.out

    sec_create_db -alt_dir $PI_TMP/salvager_print_test -my salvager_test 2>> $PI_TMP/salvager.out

   if [ $TRACE = "1" ]
    then
        echo "Trace Message: sec_salvage_db -print -sort -dbpath $PI_TMP/salvager_print_test -prtpath $PI_TMP/salvager_print_test -verbose" 
        echo " "
    fi

    echo "sec_salvage_db -print -sort -dbpath $PI_TMP/salvager_print_test -prtpath $P I_TMP/salvager_print_test -verbose" >> $PI_TMP/salvager.out

    sec_salvage_db -print -sort -dbpath $PI_TMP/salvager_print_test -prtpath $PI_TMP/salvager_print_test -verbose 2>> $PI_TMP/salvager.out

   if [ $TRACE = "1" ]
    then
        echo "Trace Message: Concatnate all the print files together, remove variation and compare with defecto print file"
	echo " "
    fi

    # Concatnate all the print files together.

    echo "************************** .mkey.prt *****************************\n " > $PI_TMP/salvager_print.tmp
    cat $PI_TMP/salvager_print_test/.mkey.prt >> $PI_TMP/salvager_print.tmp

    dir=$PWD
    cd $PI_TMP/salvager_print_test/rgy_print
    ls -1 *prt |
while read REPLY
    do
        echo "\n******************************" $REPLY " *********************\n "  >> $PI_TMP/salvager_print.tmp
        cat $REPLY >> $PI_TMP/salvager_print.tmp
    done
	cd $dir

    # Remove Cell_Name, UUID, Master_Key, Auth_Key, Time, krbtgt,  Unix_Key (not CIPHER), 
    # hosts, foreign name, Tower, Auth_Key_Pepper, Realm_name

	sed -e "s/Cell_Name = \/...\/.*/Cell_Name = \/...\/XXX/g; 
	s/UUID = .*/UUID = XXX/g;   
	s/Master_Key = .*/Master_Key = XXX/g; s/Auth_Key = .*/Auth_Key = XXX/g; 
	s/Time = [/.:0-9]*$/Time = XXX/g; s/krbtgt\/.*/krbtgt\/XXX/g; 
        s/Date = [/.:0-9]*$/Time = XXX/g; s/krbtgt\/.*/krbtgt\/XXX/g;
	s/Unix_Key = .*/Unix_Key = XXX/g;
	s/foreign_group:\/...\/[^/ ]*\/[^: ]*:/foreign_group:\/...\/XXX\/XXX:/g;
	s/foreign_other:\/...\/[^: ]*:/foreign_other:\/...\/XXX:/g;
	s/foreign_user:\/...\/[^/ ]*\/[^: ]*:/foreign_user:\/...\/XXX\/XXX:/g;
	/Tower.*/d; /        [0-9a-z]*/d;
	s/Auth_Key_Pepper = .*/Auth_Key_Pepper = XXX/g;
	s/Realm_Name = \/...\/.*/Realm_Name = \/...\/XXX/g;
	s/hosts\/.*\/self$/hosts\/XXX\/self/g; s/hosts\/[^/]*$/hosts\/XXX/g" $PI_TMP/salvager_print.tmp > $PI_TMP/salvager_print.out

    # Remove comments from sample print output.
    sed -e "/^#.*/d"  ../moretests/salvager_sample.prt > $PI_TMP/salvager_sample.prt
    echo "diff $PI_TMP/salvager_print.out $PI_TMP/salvager_sample.prt" >> $PI_TMP/salvager.out
    diff $PI_TMP/salvager_print.out $PI_TMP/salvager_sample.prt >> $PI_TMP/salvager.out

        if [ $? -ne 0 ]
            then
            STATUS=2
            EXIT=1
            echo "(sec_salvage_db -print) has different output from sample -print output"
            echo "Exit the test, can't contine to do -reconstruct." 


            if [ $TRACE = "1" ]
                then
                echo "Trace Message:    (Sec_salvage_db -print) produce different print files from the sample one."
		echo " "
            fi
        else
            if [ $TRACE = "1" ]
                then
                echo "Trace Message: (Sec_salvage_db -print) produce the same ourput as the sample one."
		echo " "
            fi
        fi

    # Do -reconstruct

    if [ $STATUS = 0 ]
    then
	echo >> $PI_TMP/salvager.out
        if [ $TRACE = "1" ]
        then
            echo "Trace Message: sec_salvage_db -reconstruct -dbpath  $PI_TMP/salvager_rec_test  -prtpath $PI_TMP/salvager_print_test -verbose"
    	echo " "
        fi

        echo "sec_salvage_db -reconstruct -dbpath  $PI_TMP/salvager_rec_test  -prtpath $PI_TMP/salvager_print_test -verbose" >> $PI_TMP/salvager.out

        sec_salvage_db -reconstruct -dbpath  $PI_TMP/salvager_rec_test  -prtpath $PI_TMP/salvager_print_test -verbose 2>> $PI_TMP/salvager.out

        if [ $TRACE = "1" ]
        then
            echo "Trace Message: sec_salvage_db -print -sort -dbpath $PI_TMP/salvager_rec_test -prtpath $PI_TMP/salvager_re c_test -verbose"
	    echo " "
        fi

        echo "sec_salvage_db -print -sort -dbpath $PI_TMP/salvager_rec_test -prtpath $PI_TMP/salvager_rec_test -verbose" >> $PI_TMP/salvager.out

        sec_salvage_db -print -sort -dbpath $PI_TMP/salvager_rec_test -prtpath $PI_TMP/salvager_rec_test -verbose 2>> $PI_TMP/salvager.out

        # compare first print output with reconstruct's print output.

        diff $PI_TMP/salvager_print_test/.mkey.prt $PI_TMP/salvager_rec_test/.mkey.prt>> $PI_TMP/salvager.out
 
        DIFF_RES=$?
 
        $DIFF $PI_TMP/salvager_print_test/rgy_print $PI_TMP/salvager_rec_test/rgy_print >> $PI_TMP/salvager.out

        if [ $DIFF_RES -ne 0 -o $? -ne 0 ]
        then
            STATUS=2
            EXIT=1
            echo "(sec_salvage_db -print) has different result from (sec_salvage_db -reconstruct and sec_salvage_db -print)"

            if [ $TRACE = "1" ]
   	    then
                echo "Trace Message:    Sec_salvage_db -reconstruct produce different database from the original."
	        echo " "
	    fi
        else
            if [ $TRACE = "1" ]
    	    then
                echo "Trace Message: Sec_salvage_db -reconstruct produce same database as the original."
	        echo " "
	    fi
        fi
    fi
    ;; # End case 1

    "2")
    
    # Check the error status.

    sec_salvage_db -print -sort -dbpath /tmp 2>> $PI_TMP/salvager.out
    ER1=$?
    sec_salvage_db -reconstruct -prtpath /tmp 2>> $PI_TMP/salvager.out
    ER2=$?
    sec_salvage_db -print -sort /tmp 2>> $PI_TMP/salvager.out
    ER3=$?

    if [ ER1 = 0 -o ER2 = 0 -o ER3 = 0 ]
    then
	STATUS=2
	EXIT=1
	echo "Error status didn't catched.  See $PI_TMP/salvager.out"
    else
	if [ $TRACE = "1" ]
        then
            echo "Trace Message: Error status catched."
	else
	    if [ $SFLAG = "1" ]
	    then
		echo "Trace Message: Error status catched." 1>&2
	    fi
	fi
    fi

    ;; # End case 2.

    esac

#
# Report sec_salvage_db test results.
#

echo " "

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

    echo "Failure Message:   sec_salvage_db test case failed with error $EXIT"
    if [ $SFLAG = "1" ]
    then
      echo "Failure Message:   sec_salvage_db test case failed with error $EXIT
" 1>&2
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
# End script salvager.sh

