package main;

#
# Globals
#

# Cmd globals
$RGY_EDIT = "rgy_edit";
$DCE_LOGIN = "dce_login";


# Module containing basic utilities for Security Database interaction
# Uses $RGY_EDIT
sub rgyEdit {
    local($args, *output) = @_;
    local($failure) = 0;
    
    $failure = &Sys2("$RGY_EDIT $args", *output);
    $failure;
}

sub rgyEditInteractive {
    local($args, $subCmdsStr, *output) = @_;
    local($failure) = 0;
    
    $failure = &SysHeredoc2("$RGY_EDIT $args", $subCmdsStr, *output);
    
    $failure;
}

# XXX Does not add members to the group in the registry yet.
sub createRgyGroup {
    local($group, $groupId) = @_;
    local($code, @rgyOut);

    &Log("Creating DCE group $group, with id $groupId");
    $code = &rgyEditInteractive("-g", "add $group $groupId \n quit", *rgyOut);
    &ErrorExit("Could not create group $group, group id $groupId in registry",
	       $code) if $code;
    $code;
}

# XXX Does not take a list of secondary groups for the user yet.
sub createRgyUser {
    local($user, $userId, $pGroup) = @_;
    local($code, @rgyOut, $subCmdStr);
    
    &Log("Creating DCE user $user, with id $userId, primary group $pGroup");
    $subCmdStr = "domain principal \nadd $user $userId \n";
    $subCmdStr .= "domain account \n";
    $subCmdStr .=     
	"add $user -g $pGroup -o none -mp $CELL_ADMIN_PASSWD -pw $COMMON_USER_PASSWD";
    $subCmdStr .= "\nquit";
    
    $code = &rgyEditInteractive("", $subCmdStr, *rgyOut);
    &ErrorExit("Could not create user $user, user id $userId, primary group $pGroup in registry", $code) if $code;
    $code;
}

# returns zero on success, error value on failure
sub getRgyGroupId {
    local($group, *gid) = @_;
    local($rgyGroup, $rgyGid);
    local($code, @rgyOut);

    $code = &rgyEdit("-g $group -v", *rgyOut);
    &ErrorExit("ERROR: unable to determine id for registry group $group",
	       $code) if ($code);
    $code = 1;
    foreach (@rgyOut) {
	next if (/^Current site is:/);
	($rgyGroup, $rgyGid)  = split(' ', $_);
	if ($rgyGroup eq $group) {
	    $gid = $rgyGid;
	    $code = 0;
	    last;
	}
    }
    $code;
}

# XXX
# We can do better in getRgyPrincipals wrt regexp search which is really
# overkill. But punt for now and get back to it later.
# Arg is reference to an ordinary array
# Get principal names
sub getRgyPrincipals {
    local(*principals) = @_;
    local($code);
    local(@rgyOut);
    local($principal);
    
    $code = &rgyEdit("-p -v", *rgyOut);
    &ErrorExit("ERROR: unable to determine registry principals", $code)
	if ($code);
    foreach (@rgyOut) {
	next if (/^Current site is:/ || /^\s*$/);
	if (/^(\w+(\w|\/|-|\.)*)\s*(-?\d+)\s*$/) {
	    $principal = (split(' ', $_))[0];
	    push(@principals, $principal);
	}
    }
    $code;
}

# XXX
# We can do better in getRgyGroups wrt regexp search which is really
# overkill. But punt for now and get back to it later.
# Arg is reference to an ordinary array
# Get group names
sub getRgyGroups {
    local(*groups) = @_;
    local($code);
    local(@rgyOut);
    local($group);
    
    $code = &rgyEdit("-g -v", *rgyOut);
    &ErrorExit("ERROR: unable to determine registry groups", $code)
	if ($code);
    foreach (@rgyOut) {
	next if (/^Current site is:/ || /^\s*$/);
	if (/^(\w+(\w|\/|-|\.)*)\s*(-?\d+)\s*$/) {
	    $group = (split(' ', $_))[0];
	    push(@groups, $group);
	}
    }
    $code;
}

sub displayRgyGroups {
    local(@rgyOut);

    &Log("Displaying existing DCE groups");
    $code = &rgyEdit("-g -v", *rgyOut);
    &ErrorExit("ERROR: unable to list DCE groups", $code)
	if ($code);
}

sub displayRgyPrincipals {
    local(@rgyOut);

    &Log("Displaying existing DCE principals");
    $code = &rgyEdit("-p -v", *rgyOut);
    &ErrorExit("ERROR: unable to list DCE principals", $code)
	if ($code);
}

sub displayRgyAccounts {
    local(@rgyOut);

    &Log("Displaying existing DCE accounts");
    $code = &rgyEdit("-a -v", *rgyOut);
    &ErrorExit("ERROR: unable to list DCE accounts", $code)
	if ($code);
}

# Removing a principal also removes the account
sub removeRgyUsers {
    local(@users) = @_;
    local($user);
    local($code, @rgyOut);

    &Log(sprintf("Deleting DCE principals %s", join(' ', @users)));

    foreach $user (@users) {
	$code = &rgyEditInteractive("-p", "delete $user\n quit", *rgyOut);
	($code == 0)
	    || grep (/Entry not found/,@rgyOut) 
	    || &ErrorExit("Could not delete user $user from registry", $code);
    }
}

sub removeRgyGroups {
    local(@groups) = @_;
    local($group);
    local($code, @rgyOut);

    &Log(sprintf("Deleting DCE groups %s", join(' ', @groups)));

    foreach $group (@groups) {
	$code = &rgyEditInteractive("-g", "delete $group\nquit", *rgyOut);
	($code == 0)
	    || grep (/Entry not found/,@rgyOut) 
	    || &ErrorExit("Could not delete group $group from registry",
			  $code);
    }
}

1;
