#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: do.ksh,v $
# Revision 1.1.8.2  1996/03/11  02:44:42  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:07:41  marty]
#
# Revision 1.1.8.1  1995/12/11  22:00:08  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:59:17  root]
# 
# Revision 1.1.6.2  1994/06/10  20:48:45  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:19:00  devsrc]
# 
# Revision 1.1.6.1  1994/06/03  16:22:55  annie
# 	fixed incorrect comment leader
# 	[1994/06/03  16:17:26  annie]
# 
# Revision 1.1.4.1  1993/10/01  21:04:56  gmd
# 	 	CR 8891 - Changed exit messages to contain either FAILED or PASSED
# 	 		and exit status to be either 1 or 0.
# 	 	[1993/10/01  21:03:00  gmd]
# 
# 	 	R 8891 Porting changes plus cleanup:
# 	 	        Removed UNIX user setup code
# 	 	        Added variables for HP-UX commands/syntax
# 	 	        Added more verification routines
# 	 	        Changed exitting code
# 	 	[1993/10/01  20:43:02  gmd]
# 
# Revision 1.1.2.3  1993/08/02  19:02:58  dassarma
# 	Took out some site dependencies. Made the script more general.
# 	[1993/08/02  18:51:37  dassarma]
# 
# Revision 1.1.2.2  1993/07/09  20:22:25  dassarma
# 	This is a sample shell script to exercise file reads and writes
# 	by a user logged in a DFS account.
# 	[1993/07/09  20:16:08  dassarma]
# 
# $EndLog$
# Name: do.ksh
#	This is a just a sample script to be used during a login session
#	by users with accounts on DFS which were set by the script dfs.read.write_all.
#
# ${1} - name of the logfile
# ${2} - username
# ${3} - number of hours to run


test_suite()
{
   create_directory_structure
   if (( $? != 0 ))
   then
      return 1
   fi

   tar_and_compress
   if (( $? != 0 ))
   then
      return 1
   fi

   translate_and_dd
   if (( $? != 0 ))
   then
      return 1
   fi

   delete_directories
   if (( $? != 0 ))
   then
      return 1
   fi

   return 0
}

create_directory_structure()
{
   cd $myhome
   sts=$?
   check_CR "$sts" "cd $myhome" $0
   if (( $sts != 0 ))
   then
      return 1
   fi 
   for i in $directories
   do
     mkdir $myhome/$i
     sts=$?
     check_CR "$sts" "mkdir $i" $0
     if (( $sts != 0 ))
     then 
	return 1
     fi 
   done
   return 0
}

tar_and_compress()
{
    cd $myhome
    sts=$?
    check_CR "$sts" "cd $myhome" $0
    if (( $sts != 0 ))
    then
       return 1
    fi

    (cd /opt/dcelocal; tar cf $myhome/dfs.test etc)
     check_CR "$?" "(cd /opt/dcelocal tar cf $myhome/dfs.test etc)" $0
     
     mv $myhome/dfs.test $myhome/utils
     sts=$?
     check_CR "$sts" "mv $myhome/dfs.test $myhome/utils" $0
     if (( $sts != 0 ))
     then
        return 1
     fi
     cd $myhome/utils
     compress dfs.test
     check_CR "$?" "compress dfs.test" $0
     uncompress dfs.test.Z
     check_CR "$?" "uncompress dfs.test" $0
     tar xf dfs.test
     check_CR "$?" "tar xf dfs.test" $0
     cd $myhome/utils/etc
     sts=$?
     check_CR "$sts" "cd $myhome/utils" $0
     if (( $sts != 0 ))
     then
        return 1
     fi

     for i in `ls`
     do
       mv $myhome/utils/etc/$i $myhome/utils/etc/$i.new
       check_CR "$?" "mv $i $i.new" $0
       ln -s $myhome/utils/etc/$i.new $myhome/utils/etc/$i
       check_CR "$?" "ln -s $i $i.new" $0
       rm -rf $myhome/utils/etc/$i.new $myhome/utils/etc/$i
       check_CR "$?" "rm $myhome/utils/etc/$i.new $myhome/utils/etc/$i" $0
     done
     cd $myhome
     sts=$?
     check_CR "$sts" "cd $myhome" $0
     if (( $sts != 0 ))
     then
	return 1
     fi
     return 0
}

translate_and_dd()
{
      cd $myhome/myfiles
      pwd
      cp /opt/dcelocal/etc/dce_config .
      dd if=$myhome/myfiles/dce_config of=$myhome/myfiles/dce_config.upper conv=ucase
      dd if=$myhome/myfiles/dce_config.upper of=$myhome/myfiles/dce_config.lower conv=lcase
      rm dce_config.upper 
      cat $myhome/myfiles/dce_config.lower | tr '[a-z]' '[A-Z]' > $myhome/myfiles/dce_config.upper
      rm $myhome/myfiles/dce_config.lower
      cat $myhome/myfiles/dce_config.upper | tr '[A-Z]' '[a-z]' > $myhome/myfiles/dce_config.lower
      rm dce_config dce_config.upper $myhome/myfiles/dce_config.lower
      cd $myhome
      return 0
}
delete_directories()
{
   cd $myhome
   sts=$?
   check_CR "$sts" "cd $myhome" $0   
   if (( $sts != 0 ))
   then
      return 1
   fi
   for i in $directories
   do
     rm -rf $i
     check_CR "$?" "rm -rf $i" $0
   done
   return 0
}
check_CR()
# ${1} - error status
# ${2} - command that was executed
# ${3} - name of the function were the command was executed
{
   CR=${1}
   text=${2}
   funcname=${3}
   tnd=`date`
   if (( ${1} != 0 ))
   then
      print "In ${3} command ${2} failed at $tnd"  >> $temp_file
      return 1
   fi
   return 0
}

###################################################################
# Main 
###################################################################
#set -x
myhome=`dirname $0`
cd $myhome
myhome=`pwd`
. .profile

parent=`dirname $myhome`
timeofday=`date`
host=`hostname`
username=${2}
temp_file=${username}.tmplog
common_log=${1}

directories="utils one big file system  Mail tests myfiles news phones "

print "User ${username} entering $0 script on host $host at $timeofday." > $temp_file
print "Home directory => $myhome" >> $temp_file

HOURS_TO_EXECUTE=${3}
SECONDS_TO_EXECUTE=`awk "BEGIN { print 3600 * $HOURS_TO_EXECUTE; exit; }"`
TIMER='eval echo `awk "BEGIN { print \`date +"%j"\`*86400 + \`date +"%H"\`*3600 + \`date +"%M"\`*60 + \`date +"%S"\`; exit}"`'

PASS=0
FAIL=1
PASS_FAIL=$PASS
DONE="FALSE"
ITERATION=0
START_TIME=`$TIMER`

while [ "$DONE" = "FALSE" ]
do
   test_suite
   if (( $? != 0 ))
   then
     DONE="TRUE"
     print "$username on $host: FAILED - check the logs. Exiting ..." 
     PASS_FAIL=$FAIL
     break 
   fi

   SECONDS_EXECUTED=$SECONDS
   ITERATION=`expr $ITERATION + 1`
   echo "User ${username} completed iteration $ITERATION on host $host at `date`" >> $temp_file
   print " " >> $temp_file
   if [ "$SECONDS_EXECUTED" -gt "$SECONDS_TO_EXECUTE" ]; then
      DONE="TRUE"
      print "User ${username} ran to completion and PASSED." >> $temp_file
   fi
done

timeofday=`date`
print "User ${username} exiting $0 script on host $host at $timeofday." >> $temp_file
#set +x

# Log to common log file
cat $temp_file >> ${common_log}
rm -f $temp_file
exit $PASS_FAIL
