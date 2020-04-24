#!/bin/ksh
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: dirwrite.sh,v $
# Revision 1.1.6.2  1996/03/11  02:44:40  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:07:40  marty]
#
# Revision 1.1.6.1  1995/12/11  22:00:06  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:59:16  root]
# 
# Revision 1.1.2.5  1993/03/09  21:29:09  gmd
# 	Updated symbolic link section to first create a symbolic link
# 	that points to something that doesn't exist, then something
# 	that does.
# 	[1993/03/09  21:26:03  gmd]
# 
# Revision 1.1.2.4  1993/02/12  21:38:54  gmd
# 	Cleaned up a bit.
# 	[1993/02/11  17:35:29  gmd]
# 
# Revision 1.1.2.3  1993/02/05  15:20:34  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:40:51  cjd]
# 
# Revision 1.1.4.2  1993/02/04  22:07:41  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.2.2  1993/01/03  23:31:49  gmd
# 	Original version.
# 	[1993/01/03  23:29:28  gmd]
# 
# $EndLog$
#
############################################################################
# TITLE: dirwrite.sh
# SYNTAX:
#	-p parent directory name (to create test directory in)
#	-t test directory name (to create) 
#	-n number of entries to create in test directory
#	-r (remove flag) OR -o output_file in which to list directory contents
# OBJECTIVE:
#	To test the directory commands.
# DESCRIPTION:
# 	- create a directory
#	- chmod the directory
#	- chgrp the directory
#	- chown the directory
#	- create links to the directory
#	- copy the directory
#	- move the directory
#	- fill the directory
#	(includes using cp, ln, touch, mkdir, mkfifo, rm, rmdir)
#	- if asked to, remove the directory (rm -rf)
# OUTPUT:
#	if directory not removed, output_file lists directory contents
#	If a command fails, exits with <command> FAILED
###############################################################

check_RC()
{
	RC="$1"
	COMMAND="$2"
	if [ "$RC" != "0" ]; then
		echo "$COMMAND FAILED"
		echo "$IDENT : FAILED"
		exit 1 
	fi
}
###############################################################

initialize_variables()
{
REMOVE="FALSE"
PARENT_DIR=`pwd`
HOST=`hostname`
TEST_DIR=${HOST}_$$_dir
TEST_DIR2=${HOST}_$$_dir2
NUM_ENTRIES_TO_CREATE=5000
OUTPUT_FILE=${PARENT_DIR}/${TEST_DIR}/CONTENTS

# MIN_ENTRIES = function of do_test - change if do_test changes!!!
MIN_ENTRIES=7			

OS=`uname -a | awk '{print $1}'`
if [ "$OS" = "OSF1" ]; then
  DF="df -k -t ufs"
  LARGE_FILE="/vmunix"
else
  DF="df"
  LARGE_FILE="/usr/lib/boot/unix"
fi
HARD_LINK=${HOST}_$$_hardlink
SYMBOLIC_LINK=${HOST}_$$_symlink
SUB_DIR=${HOST}_$$_subdir
ENTRIES_CREATED=0
ME=`whoami`
MY_GROUP=`id | awk -F\( '{print $3}' | awk -F\) '{print $1}'`
IDENT=${HOST}_$$_${SCRIPT}
}
###############################################################

show_usage()
{
cat <<-EOF
Usage:  $SCRIPT [options] 

Where [options] are:

 -r		remove test directory when done
 -h		Show detailed usage for the workstation
 -p <parent_dir>	Parent directory to test in
			(default = current dir)		 
 -t <test_dir>	Test directory name to create
			(default = host_pid_dir)
 -n <num_entries_to_create>	Number of test_dir entries to create
			(default = 5000)
 -l <large_file>	Full path to large file to place in test dir
 -o <output_file>	File containing listing of test_dir contents
			(default = test_dir/CONTENTS)

For example:
	$SCRIPT  -p /:/ctd -t test_dir -n 500 -l /vmunix -o /:/ctd/test_dir_ls

EOF

}
###############################################################

parse_command_line()
{
	set -- `getopt rhp:t:l:o:n: $COMMAND_LINE_PARAMETERS` \
		>> /dev/null 2>&1

	if [ "$?" != "0" ]; then
		echo "Invalid command line parameters"
		exit 1
	fi
	
	#----------------------------------------------------------------------
	# Set variables depending on the flag encountered and then shift that
	# variable and/or flag out.
	#-----------------------------------------P-----------------------------
	while :; do
		case $1 in
			-r)	REMOVE="TRUE"
				shift;;
			-h)	show_usage
				exit;;
			-p)	PARENT_DIR="$2"
				OUTPUT_FILE=${PARENT_DIR}/${TEST_DIR}/CONTENTS
				shift
				shift;;
			-t)	TEST_DIR="$2"
				OUTPUT_FILE=${PARENT_DIR}/${TEST_DIR}/CONTENTS
				shift
				shift;;
			-l)	LARGE_FILE="$2"
				shift
				shift;;
			-n)	NUM_ENTRIES_TO_CREATE="$2"
				if [ $NUM_ENTRIES_TO_CREATE -lt $MIN_ENTRIES ]; then
					echo "Number of entries must be >= $MIN_ENTRIES"
					exit 1
				fi
				shift
				shift;;
			-o)	OUTPUT_FILE="$2"
				shift
				shift;;
			--)	shift
				break;;
		esac
	done
}
###############################################################
announce_intentions()
{
echo "$IDENT creating $NUM_ENTRIES_TO_CREATE entries in ${PARENT_DIR}/${TEST_DIR}"
echo "Entries will include a copy of $LARGE_FILE"
if [ "$REMOVE" = "TRUE" ]; then
	echo "Removing directory immediately after completing (or when interrupted)"
else
	echo "Contents of directory (ls output) upon completion will be in $OUTPUT_FILE"
fi
}
###############################################################

fill_dir()
{
# large file (1)
cp ${LARGE_FILE} ${TEST_DIR}
check_RC "$?" "cp"
ENTRIES_CREATED=`expr $ENTRIES_CREATED + 1`

cd ${TEST_DIR}
check_RC "$?" "cd"

# empty file (2)
touch touch_file
check_RC "$?" "touch"
ENTRIES_CREATED=`expr $ENTRIES_CREATED + 1`

# directory (3)
mkdir ${SUB_DIR}
check_RC "$?" "mkdir"
rmdir ${SUB_DIR}
check_RC "$?" "rmdir"
mkdir ${SUB_DIR}
check_RC "$?" "mkdir"
ENTRIES_CREATED=`expr $ENTRIES_CREATED + 1`

# links (4 and 5)
ln touch_file ${HARD_LINK}
check_RC "$?" "ln"
rm ${HARD_LINK}
check_RC "$?" "rm"
ln touch_file ${HARD_LINK}
check_RC "$?" "ln"
ENTRIES_CREATED=`expr $ENTRIES_CREATED + 1`

# point to something that doesn't exist
ln -s ${TEST_DIR} ${SYMBOLIC_LINK}
check_RC "$?" "ln -s"
rm ${SYMBOLIC_LINK}
check_RC "$?" "rm"
# point to something that does
ln -s ${PARENT_DIR}/${TEST_DIR} ${SYMBOLIC_LINK}
check_RC "$?" "ln -s"
ENTRIES_CREATED=`expr $ENTRIES_CREATED + 1`

# special file (6)
mkfifo fifo_file
check_RC "$?" "mkfifo"
ENTRIES_CREATED=`expr $ENTRIES_CREATED + 1`

# regular files
while [ $ENTRIES_CREATED -lt $NUM_ENTRIES_TO_CREATE ]
do
  ENTRIES_CREATED=`expr $ENTRIES_CREATED + 1`
  echo "$ENTRIES_CREATED" >> $ENTRIES_CREATED
done

# if necessary, make room for the contents file
if [ "${OUTPUT_FILE}" = "${PARENT_DIR}/${TEST_DIR}/CONTENTS" ]; then
  rm $ENTRIES_CREATED
  check_RC "$?" "rm"
  ENTRIES_CREATED=`expr $ENTRIES_CREATED - 1`
fi
}
###############################################################

do_test()
{
cd ${PARENT_DIR}
check_RC "$?" "cd"

# processing:	- create a directory
mkdir ${TEST_DIR}
check_RC "$?" "mkdir"

#		- chmod the directory
chmod ogu+rwx ${TEST_DIR}
check_RC "$?" "chmod"

#		- chgrp the directory
chgrp ${MY_GROUP} ${TEST_DIR}
check_RC "$?" "chgrp"

#		- chown the directory
chown ${ME} ${TEST_DIR}
check_RC "$?" "chown"

#		- create a links to the directory
ln -s ${TEST_DIR} ${SYMBOLIC_LINK}
check_RC "$?" "ln -s"
rm ${SYMBOLIC_LINK}
check_RC "$?" "rm"

#		- copy the directory
cp -r ${TEST_DIR} ${TEST_DIR2}
check_RC "$?" "cp -r"
rmdir ${TEST_DIR2}
check_RC "$?" "rmdir"

#		- move the directory
mv ${TEST_DIR} ${TEST_DIR2}
check_RC "$?" "mv"
mv ${TEST_DIR2} ${TEST_DIR}
check_RC "$?" "mv"

#		- fill the directory
fill_dir

#		- move the sub directory around
mv ${SUB_DIR} ${PARENT_DIR}
check_RC "$?" "mv"
cd ${PARENT_DIR}
check_RC "$?" "cd"
mv ${SUB_DIR} ${TEST_DIR}
check_RC "$?" "mv"
}
###############################################################

cleanup()
{
if [ "$REMOVE" = "TRUE" ]; then
	cd ${PARENT_DIR}
	check_RC "$?" "cd"
	rm -rf ${TEST_DIR}
	check_RC "$?" "rm -rf"
else
	touch ${OUTPUT_FILE}
	check_RC "$?" "touch"
	if [ "${OUTPUT_FILE}" = "${PARENT_DIR}/${TEST_DIR}/CONTENTS" ]; then
		ENTRIES_CREATED=`expr $ENTRIES_CREATED + 1`
	fi
	ls ${PARENT_DIR}/${TEST_DIR} >> ${OUTPUT_FILE}
	check_RC "$?" "ls"
	LS_ENTRIES=`cat ${OUTPUT_FILE} | wc -l`
	if [ $LS_ENTRIES -ne $ENTRIES_CREATED ]; then
		echo "$IDENT AND LS DO NOT AGREE ON ENTRY COUNT"
		echo "Entries created = $ENTRIES_CREATED"
		echo "Ls entries = $LS_ENTRIES"
		echo "$IDENT : FAILED"
		exit 1
	fi
	if [ $LS_ENTRIES -ne $NUM_ENTRIES_TO_CREATE ]; then
		echo "Warning: Did not create $NUM_ENTRIES_TO_CREATE"
	fi
	echo "$IDENT : PASSED"
fi
}
###############################################################
#
# Main Processing
#
###############################################################
if [ "$START_TIME" = "" ]; then
  START_TIME=`date`
fi
trap   'echo
	echo "========================================"
	echo "Trap signal received... cleaning up and exiting"
	echo "========================================"
	cleanup
	END_TIME=`date`
	echo "Start time was $START_TIME"
	echo "End time is $END_TIME"
	exit 1' 1 2 3 15

SCRIPT=`basename $0 .sh` 
initialize_variables
COMMAND_LINE_PARAMETERS="$@"
if [ "$COMMAND_LINE_PARAMETERS" != "" ]; then
  parse_command_line
fi

announce_intentions
do_test
cleanup
###############################################################



