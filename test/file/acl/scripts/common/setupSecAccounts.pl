#!/afs/tr/proj/tools/bin/perl

# The following environment variables were used to communicate information to 
# this script through the dce_login:
#
#  TESTACL_GROUP_LIST:	list of groups to create
#  TESTACL_USER_LIST:	list of users to create (as user, group pairs)
#  TESTACL_USER_COUNT:  number of test users
#  TESTACL_GROUP_COUNT: number of test groups
#  TESTACL_GROUP_MEMBERS: list of members in each group
#
#  TESTACL_SEC_DATABASE_CMD:	what to do to the database - setup/cleanup
#
#  CELL_ADMIN_PASSWD:	password for cell administrator (current user)
#  COMMON_USER_PASSWD:	password to use for all new accounts
#  

#
# Deal with the environment interface. Perform sanity checks.
#
$TESTACL_SRCDIR = $ENV{TESTACL_SRCDIR};
require "$TESTACL_SRCDIR/scripts/common/util";
require "$TESTACL_SRCDIR/scripts/common/sysUtil";
require "$TESTACL_SRCDIR/scripts/common/aclUtil";
require "$TESTACL_SRCDIR/scripts/common/rgyUtil";
 
&unBufferFH(STDOUT);
&unBufferFH(STDERR);

$CELL_ADMIN_PASSWD = $ENV{CELL_ADMIN_PASSWD};
$COMMON_USER_PASSWD = $ENV{COMMON_USER_PASSWD};

$numGroups = $ENV{TESTACL_GROUP_COUNT};
@groups = split(' ', $ENV{TESTACL_GROUP_LIST});
@groupMembers = split(' ', $ENV{TESTACL_GROUP_MEMBERS});
if ($numGroups != @groups  || $numGroups != @groupMembers) {
    &ErrorExit("Mismatch between TESTACL_GROUP_COUNT and TESTACL_GROUP_LIST or TESTACL_GROUP_MEMBERS");
}
$numUsers = $ENV{TESTACL_USER_COUNT};
@usersWithPGroup = split(' ', $ENV{TESTACL_USER_LIST});
if ($numUsers != @usersWithPGroup/2) {
    &ErrorExit("Mismatch between TESTACL_USER_COUNT and TESTACL_USER_LIST");
}

#
# Massage the data into the form expected by the internal interfaces 
# of this module.
#

# Groups - Create an associative array with the group name as the key and the 
# group members as the value.
for ($i = 0; $i < $numGroups; $i++) {
    $group = shift(@groups);
    $members = shift(@groupMembers);
    $members =~ tr/,/ /;
    $groupList{$group} = $members;
}

# Users - Create an associative array with the user name as the key and the 
# user's primary group as the value
for ($i = 0; $i < $numUsers; $i++) {
    $user = shift(@usersWithPGroup);
    $pGroup = shift(@usersWithPGroup);
    $userList{$user} = $pGroup;
}

#
# The main section of the script that does the main work.
#

if ($ENV{TESTACL_SEC_DATABASE_CMD} eq "setup") {
    #Determine max id allowed in the security database
    &Log("Determine maximum id number allowed in DCE");
    $maxRgyId = &findMaxRgyId;

    #Determine uids and gids that can be used.
    &Log("Determine principal ids to be used");
    &findIds('u', $numUsers, $maxRgyId, *testaclUids);
    &Log(sprintf("Principals ids to be used are %s\n",
		 join(' ', @testaclUids)));

    &Log("Determine group ids to be used");
    &findIds('g', $numGroups, $maxRgyId, *testaclGids);
    &Log(sprintf("Group ids to be used are %s\n", join(' ', @testaclGids)));

    &displayRgyGroups();
    &displayRgyPrincipals();
    &displayRgyAccounts();

    # Create groups in registry and locally
    &Log("Create local and DCE groups");
    &setupGroups(join(' ', @testaclGids), %groupList);

    #Create users in registry and locally
    &Log("Create local and DCE users");
    &setupUsers(join(' ', @testaclUids), %userList);
} elsif ($ENV{TESTACL_SEC_DATABASE_CMD} eq "cleanup") {
    &cleanupUsers(keys(%userList));
    &cleanupGroups(keys(%groupList));
} else {
    &ErrorExit("$0: invalid mode $ENV{TESTACL_SEC_DATABASE_CMD} specified.",
	       1);
}

#
# End of main section
#

#
# SUBROUTINES
#

sub findMaxRgyId {
    local($maxId, $foundMaxId);
    local($code);
    local(@rgyOut);

    $foundMaxId = 0;
    $code = &rgyEditInteractive("", "prop \nn \nquit", *rgyOut);
    &ErrorExit("findMaxRgyId", $code) if $code;
    
    foreach (@rgyOut) {
	if (/Maximum possible UID:\s*(\d+)/) {
	    $maxId = $1;
	    $foundMaxId = 1;
	    last;
	}
    }
    
    &ErrorExit("findMaxRgyId unable to find max id", 1) if (!$foundMaxId);
    $maxId;
}

# XXX The algorithm used by this function can be much improved.
# Sets @ids passed by reference
sub findIds {
    local($idType, $idCount, $maxId, *ids) = @_;

    local($curMaxId) = $maxId;
    local($count) = 0;
    local($localId);
    local($idUnavailable);

    while ($count < $idCount) {
	$localId = &findLocalId($idType, $curMaxId);
	if ($localId == -1) {
	    &ErrorExit("Unable to find free type $idType local id");
	} else {
	    &Log("Local $idType id $localId free");
	}

	$idUnavailable = &checkRgyId($idType, $localId);
	if (!$idUnavailable) {
	    push(@ids, $localId);
	    $count++;
	}
	$curMaxId = $localId - 1;
    }
}

# Returns local id. If -1, unable to find id.
# 
sub findLocalId {
    local($idType, $maxLocalId) = @_;
    local($localId) ;
    local($localFile);
    local(@localIds);

    $localId = -1; # Initialize
    
    if ($idType eq 'u') {
	$localFile = $LOCAL_PASSWD_FILE;
    } elsif ($idType eq 'g') {
	$localFile = $LOCAL_GROUP_FILE;
    } else {
	&ErrorExit("findLocalId: internal error, unknown id type $idType");
    }
    &getUsedLocalIds($localFile, *localIds);
    $localId = &findUnusedId($maxLocalId, @localIds);
    $localId;
}

sub getUsedLocalIds {
    local($localFile, *localIds) = @_;
    local(@localEntry);
    local($prevId);
    local(@tmpIds);

    open(LOCALFILE, "<$localFile") ||
	&ErrorExit("ERROR: Unable to read $localFile", $!);
    while (<LOCALFILE>) {
	@localEntry = split(':',$_);
	push(@tmpIds, $localEntry[2]);
    }
    close(LOCALFILE);		
    @tmpIds = sort(@tmpIds);
    # Remove duplicates
    push(@localIds, $tmpIds[0]);
    $prevId = $tmpIds[0];
    foreach (@tmpIds) {
	next if ($prevId == $_) ;
	push(@localIds, $_);
	$prevId = $_;
    }
}

# Expects a ascending order sorted array of ids
sub findUnusedId {
    local($maxId, @usedIds) = @_;

    local($availableId);
    local($noMoreUsedIds) = 0;
    local($usedId) = $maxId + 1;
    local($prevUsedId);
    local($idIndex) = $#usedIds;

    while ($usedId >= $maxId) {
	$prevUsedId = $usedId;
	if ($idIndex < $[) {
	    $noMoreUsedIds = 1;
	    last;
	} else {
	    $usedId = $usedIds[$idIndex--];
	}
    }
    
    if ($noMoreUsedIds) {
	#ASSERT: $prevUsedId >= $maxId
	&ErrorExit("findUnusedId - internal error, prevUsed id $prevUsedId ! >= maxId $maxId") 
	    if ($prevUsedId < $maxId);
	if ($prevUsedId > $maxId) {
	    $availableId = $maxId;
	} else {
	    $availableId = $maxId - 1;
	}
    } else {
	# ASSERT: $usedId < $maxId
	&ErrorExit("findUnusedId - internal error, usedId $usedId ! < maxId $maxId")
	    if ($usedId >= $maxId);
	# Find first gap of 2. Starting search range is 
	# (MIN($prevUsedId, $maxId + 1), $used_id)
	if ($prevUsedId > $maxId + 1) {
	    $prevUsedId = $maxId + 1;
	}
	while ($prevUsedId - $usedId < 2) {
	    $prevUsedId = $usedId;
	    if ($idIndex < $[) {
		# no more ids, hence found available id
		last;
	    } else {
		$usedId = $usedIds[$idIndex--];
	    }
	}
	$availableId = $prevUsedId - 1;
    }
    
    $availableId;
}

sub checkRgyId {
    local($idType, $id) = @_;
    local($rgyEditOptions, @rgyOut, $code, $idUnAvailable);
    
    $idUnAvailable = 1;
    if ($idType eq 'u') {
	$rgyEditOptions = "-p ";
    } else {
	$rgyEditOptions = "-g ";
    }
    $rgyEditOptions .= "-v -un $id";
    &Log("Check if DCE id type $idType, id $id is free");
    $code = &rgyEdit($rgyEditOptions, *rgyOut);
    &ErrorExit("rgy_edit $rgyEditOptions failed", $code) if $code;
    foreach (@rgyOut) {
	if (/Entry not found/) {
	    $idUnAvailable = 0;
	    last;
	}
    }
    $idUnAvailable;
}

# XXX Does not add members to the group in the registry yet.
sub setupGroups {
    local($groupIdsStr, %groupList) = @_;
    local($group, $groupId, @groupIds, $groupMembersStr, @groupMembers);
    local($code);

    @groupIds = split(' ', $groupIdsStr);
    &ErrorExit("Number of group ids do not match number of groups")
	if (@groupIds != keys(%groupList));

    while (($group, $groupMembersStr) = each(%groupList)) {
	$groupId = shift(@groupIds);
	@groupMembers = split(' ', $groupMembersStr);
	$code = &createRgyGroup($group, $groupId);
	&ErrorExit("Could not create group $group, group id $groupId in registry") if $code;
	$code = &createLocalGroup($group, $groupId, @groupMembers);
	&ErrorExit("Could not create group $group, group id $groupId locally") 
	    if ($code);
    }
}

# XXX Does not take a list of secondary groups for the user yet.
sub setupUsers {
    local($userIdsStr, %userList) = @_;
    local($user, $userId, @userIds, $pGroup);
    local($code);
    local($pGid); # Determine via registry

    @userIds = split(' ', $userIdsStr);
    &ErrorExit("Number of user ids do not match number of users") 
	if (@userIds != keys(%userList));

    while (($user, $pGroup) = each(%userList)) {
	$userId = shift(@userIds);
	$code = &createRgyUser($user, $userId, $pGroup);
	&ErrorExit("Could not create user $user, userId $userId, primary group $pGroup in registry") if ($code);

	$code = &getRgyGroupId($pGroup, *pGid);
	&ErrorExit("Could not determine group id for group $pGroup from registry") if ($code);
	
	$code = &createLocalUser($user, $userId, $pGroup, $pGid);
	&ErrorExit("Could not create user $user, userId $userId, primary group $pGroup locally") if ($code);
    }
}

sub cleanupUsers {
    local(@users) = @_;

    &Log(sprintf("Removing local and DCE test users %s", join(' ', @users)));
    &removeRgyUsers(@users);
    &removeLocalUsers(@users);
}

sub cleanupGroups {
    local(@groups) = @_;

    &Log(sprintf("Removing local and DCE test groups %s", join(' ', @users)));
    &removeRgyGroups(@groups);
    &removeLocalGroups(@groups);
}

