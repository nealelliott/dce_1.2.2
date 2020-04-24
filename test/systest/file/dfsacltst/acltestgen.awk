# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: acltestgen.awk,v $
# Revision 1.1.6.2  1996/03/11  02:44:29  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:07:34  marty]
#
# Revision 1.1.6.1  1995/12/11  21:59:52  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:59:04  root]
# 
# Revision 1.1.4.2  1994/06/10  20:48:38  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:18:47  devsrc]
# 
# Revision 1.1.4.1  1994/06/03  16:22:47  annie
# 	fixed incorrect comment leaders
# 
# 	 Revision 1.1.2.2  1993/02/27  00:28:31  ramb
# 	 	Revised version
# 	 	[1993/02/26  23:51:43  ramb]
# 
# 	 	Initial version
# 	 	[1993/02/22  15:06:25  ramb]
# 	[1994/06/03  15:44:21  annie]
# 
# $EndLog$
# This awk file will be invoked by the acltest generator program. It will
# pass the following variables to the awk file.
#	TESTNAME	/* name of the test */
#	CELL_LOCAL	/* name of the variable local cell */
#	CELL_FOREIGN1	/* name of the first foreign cell */
#	CELL_FOREIGN2	/* name of the second foreign cell */
# The awk file will be invoked as given below:
#awk -f acltestgen.awk TESTNAME=$TESTNAME CELL_LOCAL=$CELL_LOCAL CELL_FOREIGN1=$CELL_FOREIGN1 CELL_FOREIGN2=$CELL_FOREIGN2 $INPUT_FILENAME.aclset
#
#
BEGIN	{
	FS=":"
	FALSE=0
	TRUE=1
	USER_OBJ_SEEN		= FALSE
	USER_SEEN		= FALSE
	FOREIGN_USER_SEEN	= FALSE
	GROUP_OBJ_SEEN		= FALSE
	GROUP_SEEN		= FALSE
	FOREIGN_GROUP_SEEN	= FALSE
	OTHER_OBJ_SEEN		= FALSE
	FOREIGN_OTHER_SEEN	= FALSE
	ANY_OTHER_SEEN		= FALSE
	NULL_PERMISSION_SET          = "------"
	OTHER_OBJ_PERMISSION_SET     = "------"
	FOREIGN_OTHER_PERMISSION_SET = "------"
	ANY_OTHER_PERMISSION_SET     = "------"
	outputfilename1 = TESTNAME ".aclsetup"
	outputfilename2 = TESTNAME ".acltest"
	objectfilename  = TESTNAME ".object"
}

$0 ~ /^[ 	]*#.*/	{
	next
}

$0 !~ /^[ 	]*#.*/	{
	if ( $0 == "" ) {
		next
	}
#
#
#	Object Record
#	-------------
#	   Format is 'object:' followed by the fully qualified DFS objectname.
#	   There can only be one record of this type.
#	   (e.g.) object:/.../something.cell/fs/somefilename
#
#	Acl Records
#	-----------
#	   All acl records start with 'acl' followed by 'aclentrytype' followed
#	   by 'permissionset'.  Fields are delimited by ':'
#	   The format is:
#		acl:aclentrytype:permission_set
#	   where:
#		acl is a keyword
#
#		aclentrytype can be one of:
#			'user_obj', 'user', 'foreign_user',
#			'group_obj', 'group', 'foreign_group' ,
#			'other_obj', 'foreign_other' and 'any_other'
#
#		permission_set is a position_dependent combination of:
#			r for read	and	- for noread
#			w for write	and	- for nowrite
#			x for execute	and	- for noexecute
#			c for control	and 	- for nocontrol
#			i for insert	and 	- for noinsert
#			d for delete	and 	- for nodelete
#
#
#The output files are:
#	objectname.aclsetup and
#	objectname.acltest
#The format of 'objectname.acltest' is:
#	aclentrytype:cell:groupname:principalname:DFSoperation:expected_outcome
#	where
#		aclentrytype is same as in the input file
#		cell is the value of the keywords 'local', 'foreign1' or 'foreign2'
#		DFSoperation is one of the above permission set letters.
#		expected_outcome is either 'granted' or 'denied'
#
	if ($1 == "object") {
		fullname = substr($0,8)
		printf("%s\n",fullname) >> objectfilename
		next
	}
	if ($1 == "acl") {
		aclentrytype = $2
		permission_set = $3

		if ( aclentrytype == "user_obj" ) {
			USER_OBJ_SEEN=TRUE
			aclentrytype="user_obj"
			cellname="local"
			groupname="user_obj_group"
			principalname="uo_user"
			write_acltests()
			write_setup()
			next
		}

		if ( aclentrytype == "user" ) {
			USER_SEEN=TRUE
			aclentrytype="user"
			cellname="local"
			groupname="user_group"
			principalname="u_user"
			write_acltests()
			write_setup()
			next
		}

		if ( aclentrytype == "foreign_user" ) {
			FOREIGN_USER_SEEN=TRUE
			aclentrytype="foreign_user"
			cellname="foreign1"
			groupname="user_group"
			principalname="u_user"
			write_acltests()
			write_setup()
			next
		}
	
		if ( aclentrytype == "group_obj" ) {
			GROUP_OBJ_SEEN=TRUE
			aclentrytype="group_obj"
			cellname="local"
			groupname="user_obj_group"
			principalname="uo_dummy"
			write_acltests()
			write_setup()
			next
		}
	
		if ( aclentrytype == "group" ) {
			GROUP_SEEN=TRUE
			aclentrytype="group"
			cellname="local"
			groupname="group"
			principalname="g_user"
			write_acltests()
			principalname="g_dummy"
			write_acltests()
			write_setup()
			next
		}
	
		if ( aclentrytype == "foreign_group" ) {
			FOREIGN_GROUP_SEEN=TRUE
			aclentrytype="foreign_group"
			cellname="foreign1"
			groupname="group"
			principalname="g_user"
			write_acltests()
			principalname="g_dummy"
			write_acltests()
			write_setup()
			next
		}
	
		if ( aclentrytype == "other_obj" ) {
			OTHER_OBJ_SEEN=TRUE
			OTHER_OBJ_PERMISSION_SET=permission_set
			aclentrytype="other_obj"
			cellname="local"
			groupname="other_group"
			principalname="o_user"
			if ( OTHER_SEEN == FALSE) {
				write_acltests()
			}
			write_setup()
			next
		}
	
		if ( aclentrytype == "foreign_other" ) {
			FOREIGN_OTHER_SEEN=TRUE
			FOREIGN_OTHER_PERMISSION_SET=permission_set
			aclentrytype="foreign_other"
			cellname="foreign1"
			groupname="other_group"
			principalname="o_user"
			write_acltests()
			write_setup()
			next
		}
	
		if ( aclentrytype == "any_other" ) {
			ANY_OTHER_SEEN=TRUE
			ANY_OTHER_PERMISSION_SET=permission_set
			aclentrytype="any_other"
			cellname="local"
			groupname="other_group"
			principalname="o_user"
			if ( OTHER_OBJ_SEEN == FALSE ) {
				write_acltests()
			}
			write_setup()
			next
		}
	}
	printf("Invalid record (%s) in input file...\n",$0)
	next
}

END	{
	if ( (OTHER_OBJ_SEEN == TRUE) && (FOREIGN_OTHER_SEEN == TRUE) && 
             (ANY_OTHER_SEEN == TRUE) ){
		aclentrytype="generated"
		cellname="local"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=OTHER_OBJ_PERMISSION_SET
		write_acltests()

		aclentrytype="generated"
		cellname="foreign1"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=FOREIGN_OTHER_PERMISSION_SET
		write_acltests()

		aclentrytype="generated"
		cellname="foreign2"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=ANY_OTHER_PERMISSION_SET
		write_acltests()
	}

	if ( (OTHER_OBJ_SEEN == TRUE) && (FOREIGN_OTHER_SEEN == TRUE) && 
             (ANY_OTHER_SEEN == FALSE) ){
		aclentrytype="generated"
		cellname="local"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=OTHER_OBJ_PERMISSION_SET
		write_acltests()

		aclentrytype="generated"
		cellname="foreign1"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=FOREIGN_OTHER_PERMISSION_SET
		write_acltests()

		aclentrytype="generated"
		cellname="foreign2"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=NULL_PERMISSION_SET
		write_acltests()
	}

	if ( (OTHER_OBJ_SEEN == TRUE) && (FOREIGN_OTHER_SEEN == FALSE) && 
             (ANY_OTHER_SEEN == TRUE) ){
		aclentrytype="generated"
		cellname="local"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=OTHER_OBJ_PERMISSION_SET
		write_acltests()

		aclentrytype="generated"
		cellname="foreign1"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=ANY_OTHER_PERMISSION_SET
		write_acltests()
	}

	if ( (OTHER_OBJ_SEEN == TRUE) && (FOREIGN_OTHER_SEEN == FALSE) && 
             (ANY_OTHER_SEEN == FALSE) ){
		aclentrytype="generated"
		cellname="local"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=OTHER_OBJ_PERMISSION_SET
		write_acltests()

		aclentrytype="generated"
		cellname="foreign1"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=NULL_PERMISSION_SET
		write_acltests()
	}

	if ( (OTHER_OBJ_SEEN == FALSE) && (FOREIGN_OTHER_SEEN == TRUE) && 
             (ANY_OTHER_SEEN == TRUE) ){
		aclentrytype="generated"
		cellname="local"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=ANY_OTHER_PERMISSION_SET
		write_acltests()

		aclentrytype="generated"
		cellname="foreign1"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=FOREIGN_OTHER_PERMISSION_SET
		write_acltests()
	}

	if ( (OTHER_OBJ_SEEN == FALSE) && (FOREIGN_OTHER_SEEN == TRUE) && 
             (ANY_OTHER_SEEN == FALSE) ){
		aclentrytype="generated"
		cellname="local"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=NULL_PERMISSION_SET
		write_acltests()

		aclentrytype="generated"
		cellname="foreign1"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=FOREIGN_OTHER_PERMISSION_SET
		write_acltests()

		aclentrytype="generated"
		cellname="foreign2"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=NULL_PERMISSION_SET
		write_acltests()
	}

	if ( (OTHER_OBJ_SEEN == FALSE) && (FOREIGN_OTHER_SEEN == FALSE) && 
             (ANY_OTHER_SEEN == TRUE ) ){
		aclentrytype="generated"
		cellname="local"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=ANY_OTHER_PERMISSION_SET
		write_acltests()

		aclentrytype="generated"
		cellname="foreign1"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=ANY_OTHER_PERMISSION_SET
		write_acltests()
	}

	if ( (OTHER_OBJ_SEEN == FALSE) && (FOREIGN_OTHER_SEEN == FALSE) && 
             (ANY_OTHER_SEEN == FALSE) ){
		aclentrytype="generated"
		cellname="local"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=NULL_PERMISSION_SET
		write_acltests()

		aclentrytype="generated"
		cellname="foreign1"
		groupname="other_group"
		principalname="o_dummy"
		permission_set=NULL__PERMISSION_SET
		write_acltests()
	}
}

function write_acltests() {
	read_permission    = substr(permission_set,1,1)
	write_permission   = substr(permission_set,2,1)
	execute_permission = substr(permission_set,3,1)
	control_permission = substr(permission_set,4,1)
	insert_permission  = substr(permission_set,5,1)
	delete_permission  = substr(permission_set,6,1)

	dfsoperation="read"
	if (read_permission == "r" || read_permission == "R") {
		outcome="granted"
	} else {
		outcome="denied"
	}
	write_acltest()
	dfsoperation="write"
	if (write_permission == "w" || write_permission == "W") {
		outcome="granted"
	} else {
		outcome="denied"
	}
	write_acltest()
	dfsoperation="execute"
	if (execute_permission == "x" || execute_permission == "X") {
		outcome="granted"
	} else {
		outcome="denied"
	}
	write_acltest()
	dfsoperation="control"
	if (control_permission == "c" || control_permission == "C") {
		outcome="granted"
	} else {
		outcome="denied"
	}
	write_acltest()
	dfsoperation="insert"
	if (insert_permission == "i" || insert_permission == "I") {
		outcome="granted"
	} else {
		outcome="denied"
	}
	write_acltest()
	dfsoperation="delete"
	if (delete_permission == "d" || delete_permission == "D") {
		outcome="granted"
	} else {
		outcome="denied"
	}
	write_acltest()
}

function write_acltest() {
	printf("%s:%s:%s:%s:%s:%s\n",aclentrytype,cellname,groupname,principalname,dfsoperation,outcome) >> outputfilename2
}

function write_setup() {
	if (cellname == "local") {
		real_cellname=CELL_LOCAL
	}
	if (cellname == "foreign1") {
		real_cellname=CELL_FOREIGN1
	}
	if (cellname == "foreign2") {
		real_cellname=CELL_FOREIGN2
	}

	if (aclentrytype == "user_obj" || aclentrytype == "group_obj" || aclentrytype == "other_obj" || aclentrytype == "any_other" ) {
		printf("%s:%s\n",aclentrytype,permission_set) >> outputfilename1
	}

	if (aclentrytype == "user" ) {
		printf("%s:%s:%s\n",aclentrytype,principalname,permission_set) >> outputfilename1
	}

	if (aclentrytype == "foreign_user" ) {
		printf("%s:/.../%s/%s:%s\n",aclentrytype,real_cellname,principalname,permission_set) >> outputfilename1
	}

	if (aclentrytype == "group" ) {
		printf("%s:%s:%s\n",aclentrytype,groupname,permission_set) >> outputfilename1
	}

	if (aclentrytype == "foreign_group" ) {
		printf("%s:/.../%s/%s:%s\n",aclentrytype,real_cellname,groupname,permission_set) >> outputfilename1
	}

	if (aclentrytype == "foreign_other" ) {
		printf("%s:/.../%s:%s\n",aclentrytype,real_cellname,permission_set) >> outputfilename1
	}
}

