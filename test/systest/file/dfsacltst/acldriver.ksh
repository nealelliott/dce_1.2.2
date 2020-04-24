#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: acldriver.ksh,v $
# Revision 1.1.6.2  1996/03/11  02:44:24  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:07:32  marty]
#
# Revision 1.1.6.1  1995/12/11  21:59:49  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:59:02  root]
# 
# Revision 1.1.4.2  1994/06/10  20:48:36  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:18:44  devsrc]
# 
# Revision 1.1.4.1  1994/06/03  16:22:44  annie
# 	fixed incorrect comment leaders
# 
# 	 Revision 1.1.2.2  1993/02/27  00:28:17  ramb
# 	 	Revised version
# 	 	[1993/02/26  23:51:30  ramb]
# 
# 	 	Initial version
# 	 	[1993/02/22  15:06:37  ramb]
# 	[1994/06/03  15:44:20  annie]
# 
# $EndLog$
# 
#	Program: acldriver.ksh
#
#	Date:	 01/15/93
#
#	Description: 
#		This script is used to test the acls on a given object.
#		It is used after 'acltest_generator' and uses the output
#		of 'acltest_generator' as its input.
#
#	Author:	Ram Balaraman
#
#	Input:
#		objectname_xxxxx.aclsetup
#		objectname_xxxxx.acltest
#
#
# make sure all the input parameters are given and that they are valid
# should have the following environment variables defined
#	CELL_LOCAL
#	CELL_FOREIGN1
#	CELL_FOREIGN2
#	CELL_SYSTEMS_LOCAL
#	CELL_SYSTEMS_FOREIGN1
#	CELL_SYSTEMS_FOREIGN2
#
progname=`basename $0`
number_of_params=$#
input_acl_filename=$1
current_directory=`pwd`

#---------------------------------------------
# get_string()
#
# This routine reads an input string.
#---------------------------------------------
get_string()
{
        read string
	export string
}

abort()
{
	cd $current_dir
	echo "Deleting directory $LOGDIR"
	#rm -rf $LOGDIR
	echo "Aborting...."
	exit 1
}

initialize()
{
	#get hostname and set HOSTNAME to it
	#make a unique directory and cd to it
	#
	input_acl_filename=$1
	process_id=$2
	if [ "$1" = "" -o "$2" = "" ]
	then
		echo "FAILED:You should invoke this test with 2 parameters:"
		exit 1
	fi
	x=${input_acl_filename%.acltest}
	TESTNAME=${x##*/}	#take just the filename without path & .acltest
	HOSTNAME=`hostname | cut -f1 -d. 2>/dev/null`
	LOGDIR=/tmp/$2/$TESTNAME
	echo "acldriver: ... Creating directory $LOGDIR"
	mkdir -p $LOGDIR
	cp $input_acl_filename $LOGDIR
	cp acldriver.awk $LOGDIR
	cp dfsacl.functions $LOGDIR
	cp dfsacl_registry_setup.main $LOGDIR
	cp dfsacl_registry_setup.exec $LOGDIR
	cp $TESTNAME.object $LOGDIR
	OBJECTNAME=`head -1 $TESTNAME.object`
	chmod +x $LOGDIR/*
	cd $LOGDIR
}


setup_registry()
{
	#ensure that all three cells are setup with the principals and groups
	cd .
}

setup_object()
{
	#assume cell_admin identity and create the object.
	#set the acl's on the object
	#reassume original identity
	#make sure everything went okay on the cell_admin identity before returning
	cd .
}

generate_test_for_cell()
{
	#take the input 'acltest' file sort it by principal within a cell
	#have a awk script turn each file into a shell file.
	sort -t: +1 +3 $input_acl_filename > $input_acl_filename.sorted
	awk -f acldriver.awk DRIVER_SYSTEM=$HOSTNAME CELL_LOCAL=$CELL_LOCAL CELL_FOREIGN1=$CELL_FOREIGN1 CELL_FOREIGN2=$CELL_FOREIGN2 TESTNAME=$TESTNAME OBJECTNAME=$OBJECTNAME ACLFILENAME=$input_acl_filename $input_acl_filename.sorted
	rm acldriver.awk
	if [ $? -ne 0 ]
	then
		abort
	fi
}

generate_test_for_systems()
{
#from the generated files for each cell make a number of files for each system
for system in $CELL_SYSTEMS_LOCAL
do
	if [ -a local.*.$TESTNAME ] 
	then
		for file in local.*.$TESTNAME
		do
			sed -e "s/^MYCELLNAME=/MYCELLNAME=$CELL_LOCAL/" -e "s/^MYHOSTNAME=/MYHOSTNAME=$system/" $file > $file.$system
		done
	fi
done
rm local.*.$TESTNAME > /dev/null 2> /dev/null

#
for system in $CELL_SYSTEMS_FOREIGN1
do
	if [ -a foreign1.*.$TESTNAME ] 
	then
		for file in foreign1.*.$TESTNAME
		do
			sed -e "s/^MYCELLNAME=/MYCELLNAME=$CELL_FOREIGN1/" -e "s/^MYHOSTNAME=/MYHOSTNAME=$system/" $file > $file.$system
		done
	fi
done
rm foreign1.*.$TESTNAME > /dev/null 2> /dev/null
#
for system in $CELL_SYSTEMS_FOREIGN2
do
	if [ -a foreign2.*.$TESTNAME ] 
	then
		for file in foreign2.*.$TESTNAME
		do
			sed -e "s/^MYCELLNAME=/MYCELLNAME=$CELL_FOREIGN2/" -e "s/^MYHOSTNAME=/MYHOSTNAME=$system/" $file > $file.$system 
		done
	fi
done
rm foreign2.*.$TESTNAME > /dev/null 2> /dev/null
#
}

run_tests()
{
	#we now have all the shell scripts that should be run under 
	#different principals from different systems.  We should now
	#copy the scripts to the systems and then remotely execute
	#them. Once all the scripts have been executed we need to go
	#and collect all the log files in these systems and then
	#store them all together in one place.  We can then examine
	#them for any 'FATAL', 'ERROR' or 'FAILED' messages. If we
	#do find any then we need to declare the script as failed.
	#Concatenate the acl input file and the log file and store
	#them in different directories.
	#delete all remote stuff
	
	#get a listing of all the files that start with $TESTNAME. 
	rm $input_acl_filename.sorted
	ls -1 *.$TESTNAME.* | awk -F. '{print $(NF)}' | sort | uniq > systems.list
	systems_list=`awk '{ printf("%s ",$0) }' systems.list`
	echo "acldriver: ... Systems list = $systems_list"
	make_remotescript
	execute_remotescript
	#collect_results
}

make_remotescript()
{
	cat > remotescript <<==EOF==
#set -x
echo "\\tremotescript: ... Starting"
for system in $systems_list
do
	echo "\\tremotescript: ... Copying files to [\$system]"
	remote_dir=/tmp/$HOSTNAME/$process_id/$TESTNAME
	rsh \$system mkdir -p \$remote_dir
	rsh \$system chmod 777 \$remote_dir
	rcp -p *.\$system \$system:\$remote_dir
	rcp -p dfsacl.functions \$system:\$remote_dir
	rcp -p dfsacl_registry_setup.main \$system:\$remote_dir
	rcp -p dfsacl_registry_setup.exec \$system:\$remote_dir
	rsh \$system chmod 777 \$remote_dir/\\*
	echo "\\tremotescript: ... Setting up registry & passwd files on [\$system]"
	rsh \$system \$remote_dir/dfsacl_registry_setup.main
	rsh \$system rm \$remote_dir/dfsacl_registry_setup.main
	rsh \$system rm \$remote_dir/dfsacl_registry_setup.exec
	dcelogin_status=\`rsh \$system ls \$remote_dir/dfsacl_registry_setup.exec.dcelogin\`
	if [ "\$dcelogin_status" = "" ]
	then
		echo "dce_login to [\$system] as [cell_admin] failed!"
		exit 1
	fi

#  if this system is where this test is being run from then execute script
#  to create the object
#
	if [ "\$system" = "$HOSTNAME" ]
	then
		$TESTNAME.setup $OBJECTNAME
	fi

	ls -1 *.\$system | cut -d. -f2 | sort | uniq > principals.list
	principals_list=\`awk '{ printf("%s ",\$0) }' principals.list\`
	echo "\\tremotescript: ... principals_list = \$principals_list"
	for principal in \$principals_list
	do
		echo "\\tremotescript: ... Executing test on [\$system] as [\$principal] principal"
		for x in *\$principal.*main.*\$system
		do
			echo "\\tremotescript: ... Testname is [\$x]"
			rsh  \$system -l \$principal \$remote_dir/\$x \$remote_dir/\$x &
			wait
			echo "\\tremotescript: ... Retrieving results of test"
			rcp \$system:\$remote_dir/\\*.log \$x.log
			rsh \$system rm -f \$remote_dir/\\*.log
			cat \$x.log >> all.log
			rm \$x.log
		done
		#touch processids.\$principal.\$system.\$!
	done
done
rm systems.list principals.list
#wait
==EOF==
	chmod +x remotescript
}

execute_remotescript()
{
	./remotescript $process_id &
	pid=$!
	echo "acldriver: ... remotescript started under process id $pid"
#	sleep 300    #wait for 30 minutes before going and looking for results
#	sleep 1800    #wait for 30 minutes before going and looking for results
#	kill -9 $pid > /dev/null 2> /dev/null  #kill the last process that we started if it is there
	echo "acldriver: ... waiting for remotescript (process $pid) to finish"
	wait
}


#
#	MAIN PROGRAM starts here
#
#
echo "acldriver: ... Starting"
initialize $1 $2
echo "acldriver: ... setting up registry"
setup_registry
echo "acldriver: ... setting up object"
setup_object
echo "acldriver: ... generating test for cell"
generate_test_for_cell
echo "acldriver: ... generating test for systems"
generate_test_for_systems
echo "acldriver: ... starting execution of all tests"
run_tests
cd $current_dir
echo "acldriver: ... cleaning up"
#rm -rf $LOGDIR
exit

