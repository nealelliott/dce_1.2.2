#!/bin/ksh
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: acltestgen.ksh,v $
# Revision 1.1.6.2  1996/03/11  02:44:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:07:35  marty]
#
# Revision 1.1.6.1  1995/12/11  21:59:53  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:59:05  root]
# 
# Revision 1.1.4.2  1994/06/10  20:48:39  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:18:48  devsrc]
# 
# Revision 1.1.4.1  1994/06/03  16:22:48  annie
# 	fixed incorrect comment leaders
# 
# 	 Revision 1.1.2.2  1993/02/27  00:28:37  ramb
# 	 	Revised version
# 	 	[1993/02/26  23:51:48  ramb]
# 
# 	 	Initial version
# 	 	[1993/02/22  15:06:13  ramb]
# 	[1994/06/03  15:44:22  annie]
# 
# $EndLog$
#	ACLTEST Generation Phase
#	------------------------
#	Given a subset of acls on a given object (the input data), we need
#	to generate a test that would test the access granted/denied for the
#	object by DFS.	This can be done automatically.	 The program to do
#	this is the ACLTEST_GENERATOR.	The input for this program is
#	a file whose contents identify the object and the set of acls for
#	that object. (objectname_xxxxx.aclset, above).
#	The program generates the following outputs:
#
#		objectname_xxxxx.aclsetup
#		objectname_xxxxx.acltest
#
#	The aclsetup file consists of UNIX commands to create the object and
#	acledit commands to set the acls on the object.
#
#	The acltest file consists of a series of lines each of which identifies
#	a specific DFS operation to be performed by a specific principal in a 
#	specific group in a specific cell on a specific object. It also 
#	specifies the expected result of the operation.	 Each line in the
#	file consists of several fields delimited by ':'.
#
#	aclentrytype:cell:groupname:principalname:DFSoperation:expected_outcome
#	where:
#		cellname is either 'local', or 'foreign1' or 'foreign2'.
#		groupname is one of:
#			user_obj_group
#			user_group
#			group_group
#			other_group
#		principal_name is one of:
#			user_obj_user
#			user_obj_dummy
#			user_user
#			user_dummy
#			group_user
#			group_dummy
#			other_user
#			other_dummy
#		DFSoperation is one of:
#			read
#			write
#			execute
#			control
#			insert
#			delete
#		expected_outcome is one of:
#			granted
#			denied
#
#
#Processing Logic:
#-----------------
#Set all global flags to 'unset' value.
#write to the 'objectname_xxxxx.aclsetup' output file the UNIX commands to
#create the object.
#For each entry in the input file,
#do
#  write the acledit commands to set the acl to the 'aclsetup' file.
#  write out to the 'acltest' file an entry for each permission bit. 
#  Each entry would specify the entrytype,cellname,groupname,username, 
#  operation and the expected outcome.
#  Also set global flags indicating the entrytypes processed.
#  Capture the permission set for entry types 'other_obj', 'foreign_other' and
#  'any_other'.
#  If the entry type is either 'group', or 'foreign_group' then generate
#  additional entries to check access with another member of the specified
#  group.
#enddo
#
#Depending on the global flags set, write a number of entries to the 'acltest'
#output file to ensure the denial or grant of access to the file.
#
#
#ACLTEST Generator Detailed Processing Logic:
#--------------------------------------------


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

get_input_file()
{
	if [ "$1" = "" ]
	then
		echo "Input filename should be provided as first parameter!"
		echo "Aborting"
		exit 1
	fi

	if [ ! \(  -f $1 -a -r $1 \) ]
	then
		echo "Invalid filename ($1) !!!"
		echo "Aborting"
		exit 1
	fi

	INPUT_FILENAME=${1%.aclset}
	TESTNAME=${INPUT_FILENAME##*/}  #take just the filename without path & .aclset
	HOSTNAME=`hostname | cut -f1 -d. 2>/dev/null`
}

get_input_file $1
rm -f $INPUT_FILENAME.acltest $INPUT_FILENAME.setup $INPUT_FILENAME.object
awk -f acltestgen.awk TESTNAME=$TESTNAME CELL_LOCAL=$CELL_LOCAL CELL_FOREIGN1=$CELL_FOREIGN1 CELL_FOREIGN2=$CELL_FOREIGN2 $INPUT_FILENAME.aclset

