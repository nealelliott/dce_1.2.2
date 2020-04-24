package main;

# Module containing basic utilities for ACL interaction

#
# Globals
#

# Cmd name globals
$ACL_EDIT = "acl_edit";

# Access type globals
$PERMNONE = 0x0;
$PERMEXEC = 0x1;  # chosen to be compatible with UNIX mode bit
$PERMWRITE = 0x2; # chosen to be compatible with UNIX mode bit
$PERMREAD = 0x4;  # chosen to be compatible with UNIX mode bit
$PERMCONTROL = 0x8;
$PERMINSERT = 0x10;
$PERMDELETE = 0x20;

# An ACL is represented as a associative array. The various keys are
# owner_realm, user_obj, group_obj, ...., user:rajesh ....

# Uses $ACL_EDIT and $ACL_EDIT_FLAGS globals
sub aclEdit {
    local($flags, $object, $args, *output) = @_;
    local($failure) = 0;

    $failure = &Sys2("$ACL_EDIT $object $flags $args", *output);
    $failure;
}

# $subCmdsStr - commands are seperated by a newline char
sub aclEditInteractive {
    local($flags, $object, $args, $subCmdsStr, *output) = @_;
    local($failure) = 0;

    $failure = &SysHeredoc2("$ACL_EDIT $object $flags $args", $subCmdsStr,
			   *output);
    $failure;
}

# $which = { o | io | ic }
# Gets assoc ACL in %acl reference parameter
# Deals with foreign ACL entries and also with restrictions imposed by mask_obj
# $effectiveAccess specifies if the resulting acl should have the 
# actual perms in each entry, or whether effectives ones. Effectives perms
# are actual perms masked by mask_obj.y
sub getAcl {
    local($object, $which, $effectiveAccess, *acl) = @_;
    local($code, $cmdArg, @localAcl);
    local($entryType, $entryPermStr);
    local(@entries);

    if ($TESTACL_DEBUG) {
	&Log("getAcl started on object $object $which acl"); 
    }

    if ($which ne "o") {
	$cmdArg = "-$which -l";
    } else {
	$cmdArg = "-l";
    }
    $code = &aclEdit("", $object, $cmdArg, *localAcl);
    &ErrorExit("getAcl $object, type $which", $code) if $code;
    # Parse ACL
    foreach (@localAcl) {
	if (/acl_edit $cmdArg/ || /SEC_ACL for/ || /^\s*$/) {
	    next;
	} elsif (/Default cell = \/...\/(.*)$/) {
	    $acl{'owner_realm'} = $1;
	} elsif (/^(mask_obj|user_obj|group_obj|other_obj|any_other|user|group|any_other|foreign_user|foreign_group|foreign_other)/) {
	    $entryType = ""; 
	    $entryPermStr = "";
	    if (/^(mask_obj|user_obj|other_obj):((\w|-)+)\s*$/) {
		# mask_obj, user_obj, other_obj entries
		$entryType = $1;
		$entryPermStr = $2;
	    } else {
		# entries masked_by mask_obj
		if ($effectiveAccess && /#effective:/) {
		    @entries = split(':', $_);
		    #entry permissions restricted by mask_obj
		    if (/^(group_obj|any_other)/) {
			$entryType = $entries[0];
			$entryPermStr = $entries[2];
		    } elsif (/^(user|group|foreign_other)/) {
			$entryType = "$entries[0]:$entries[1]";
			$entryPermStr = $entries[3];
		    } elsif (/^(foreign_user|foreign_group)/) {
			$entryType = "$entries[0]:$entries[1]:$entries[2]";
			$entryPermStr = $entries[4];
		    }
		} else {
		    if (/#effective:/) {
			$_ =~ s/#effective:.*$//;
		    }
		    @entries = split(':', $_);
		    #entry permissions not restricted by mask_obj
		    if (/^(group_obj|any_other)/) {
			$entryType = $entries[0];
			$entryPermStr = $entries[1];
		    } elsif (/^(user|group|foreign_other)/) {
			$entryType = "$entries[0]:$entries[1]";
			$entryPermStr = $entries[2];
		    } elsif (/^(foreign_user|foreign_group)/)  {
			$entryType = "$entries[0]:$entries[1]:$entries[2]";
			$entryPermStr = $entries[3];
		    }
		}
	    }
	    if ($TESTACL_DEBUG) {
		&Log("getAcl parsed entry $entryType - $entryPermStr on object $object $which acl"); 
	    }
	    $acl{$entryType} = &aclPermStrToAclPermSet($entryPermStr);
	} else {
	    next;
	}
    }
    if ($TESTACL_DEBUG) {
	&Log("getAcl completed on object $object $which acl"); 
    }
    $code;
}

# XXX Not used
# Sets a ACL according to the passed in assoc ACL, with -n acl_edit option.
sub setAcl {
    local($object, $which, %acl) = @_;
    local($code, $cmdArg, $aclEntries, @dummy);
    local($k, $v);
    
    if ($which ne "o") {
	$cmdArg = "-$which -m";
    } else {
	$cmdArg = "-m";
    }
    while (($k, $v) = each(%acl)) {
	# XXX - is this really ok to ignore owner_realm here. What
	# if it is being changed.
	if ($k eq 'owner_realm') {
	    next;
	}
	$aclEntries .= "$k:$v";
    }
    $cmdArg .= $aclEntries;
    $code = &aclEdit("-n", $object, $cmdArg, *dummy);
    &ErrorExit("setAcl $object, type $which", $code) if $code;
    $code
}

sub aclPermStrToAclPermSet {
    local($aclPermStr) = @_;
    local($aclPermSet) = $PERMNONE;

    if ($aclPermStr =~ /r/) {
	$aclPermSet |= $PERMREAD;
    }
    if ($aclPermStr =~ /w/) {
	$aclPermSet |= $PERMWRITE;
    } 
    if ($aclPermStr =~ /x/) {
	$aclPermSet |= $PERMEXEC;
    }
    if ($aclPermStr =~ /c/) {
	$aclPermSet |= $PERMCONTROL;
    }
    if ($aclPermStr =~ /i/) {
	$aclPermSet |= $PERMINSERT;
    }
    if ($aclPermStr =~ /d/) {
	$aclPermSet |= $PERMDELETE;
    }
    $aclPermSet;
}

sub aclPermSetToAclPermStr {
    local($aclPermSet) = @_;
    local($aclPermStr);
    
    if ($aclPermSet & $PERMREAD) {
	$aclPermStr .= 'r';
    }
    if ($aclPermSet & $PERMWRITE) {
	$aclPermStr .= 'w';
    }
    if ($aclPermSet & $PERMEXEC) {
	$aclPermStr .= 'x';
    }
    if ($aclPermSet & $PERMCONTROL) {
	$aclPermStr .= 'c';
    }
    if ($aclPermSet & $PERMINSERT) {
	$aclPermStr .= 'i';
    }
    if ($aclPermSet & $PERMDELETE) {
	$aclPermStr .= 'd';
    }
    $aclPermStr;
}

sub getAssocAclEntryPermStr {
    local($entry, %acl) = @_;
    local($permStr);

    if (defined($acl{$entry})) {
	$permStr = $acl{$entry};
    } else {
	$permStr = "";
    }
    $permStr;
}    
	

# XXX Not used
sub getArrayAclEntryPerm {
    local($entryType, @acl) = @_;
    local($perm);

    foreach (@acl) {
	if (/$entryType:\(.*\)/) {
	    $perm = $1;
	    last;
	}
    }
    $perm;
}

# XXX Not used
sub getArrayAclOwnerRealm {
    local(@acl) = @_;
    local($realm);

    foreach (@acl) {
	if (/Default Cell:\(.*\)/) {
	    $realm = $1;
	    last;		# 
	}
    }
    $realm;
}

# Checks access in the kernel for current uid
# Return value indicates whether there was an internal error.
#  1 - internal error, ignore grant 
#  0 - no internal error, 
#      grant = 1 => success
#      grant = 0 => denied

sub kernelAclAccess {
    local($objectType, $object, $access, $insertFile,
	  $deleteFile, *grant) = @_;

    local($code, $granted);

     if ($objectType eq "FILE") {
	$code = &fileKernelAclAccess($object, $access, *grant);
    } else {
	$code = &dirKernelAclAccess($object, $access, 
				    $insertFile, $deleteFile, *grant);
    }

    $code;
}    

# XXX Only handles r,w,x currently. "c" to be added.
sub fileKernelAclAccess {
    local($file, $access, *grant) = @_;
    local($code, $granted, $accessProg, $match);		

    $match = 0;
    # Assume success;
    $granted = 1;

    if ($access =~ /r/) {
	$match++;
	$code = open(TA_FH, "$file");
	if (!defined($code)) {
	    &Log("Open of $file for read to test r access failed ($!)");
	    $granted = 0;
	} else {
	    # r allowed
	    close(TA_FH);
	    $code = 0;
	}
    } 

    if (!$code) {
	if ($access =~ /w/) {
	    $match++;
	    $code = open(TA_FH, ">>$file");
	    if (!defined($code)) {
		&Log("Open of $file for append to test w access failed ($!)");
		$granted = 0;
	    } else {
		# w allowed
		close(TA_FH);
		$code = 0;
	    }
	} 
    }	

    if (!$code) {
	if ($access =~ /x/) {
	    $match++;
	    # This relies on a C program as Perl does not have the access 
	    # call and I could not use a wrapper around syscall as AIX does 
	    # not use syscall to trap to kernel. Instead on AIX the loader 
	    # sets up traps to kernel space for special symbols specified 
	    # to it via an import list. 
	    
	    # The environment variable TESTACL_SRCDIR should be defined for 
	    # this to work.
	    if (!defined($ENV{TESTACL_SRCDIR})) {
		&Log("ERROR: Env var TESTACL_SRCDIR needed to execute access program not specified");
		$code = 1;
	    } else {
		$accessProg = "$ENV{TESTACL_SRCDIR}" . "/utils/access";
		if ($ENV{TESTACL_DEBUG}) {
		    $code = system("dbx $accessProg");
		} else {
		    # exit value 0 for grant, and the errno for denied.
		    $code = &Sys("$accessProg $file x");
		}
		if ($code == 0) {
		    # x allowed
		} else {
		    &Log("access($file, X_OK) failed with errno $code");
		    $granted = 0;
		    $code = 0;
		}
	    }
	} 
    }

    if (!$match) {
	&Log("ERROR: Invalid access ($access) specified on file $file");
	$code = 1;
	$granted = 0;
    }

    $grant = $granted;
    $code;
}

# XXX Only handles r,w,x currently. "c" to be added.
#	 opendir	openfile	chdir
# r  x 	 P		P		P
# r  !x  P		F		F
# !r x	 F		P		P
# !r !x	 F		F		F		
#
# opendir -> r
# openfile -> x
# chdir -> x
#
sub dirKernelAclAccess {
    local($dir, $access, $insertFile, $deleteFile, *grant) = @_;
    local($code, $granted, $curDir, $match, $file, $error);

    $match = 0;
    #Assume success
    $granted = 1;

    # r, x, wi, wd.
    if ($access =~ /r/) {    
	$match++;
	$code = &openDirectory($dir, *error);
	if (!$code) {
	    &Log("Open of dir $dir to test access $access failed ($!)");
	    $granted = 0;
	} else {
	    # r allowed
	    $code = 0;
	}
    }

    if (!$code) {
	if ($access =~ /x/) {
	    $match++;
	    $curDir = `pwd`;
	    chop($curDir);
	    &Log("Changing dir to $dir to test x access");
	    $code = chdir($dir);
	    if (!$code) {
		&Log("Chdir to dir $dir to test x access ($!)");
		$granted = 0;
	    } else {
		# x allowed
		chdir($curDir);
		$code = 0;
	    }
	}
    }
    
    if (!$code) {
	if (($access =~ /w/) && ($access =~ /i/)) {
	    $match++;
	    $file = $dir . "/" . $insertFile;
	    $code = &createFile($file, *error);
	    if ($code == 0) {
		&Log("Create of $insertFile in $dir to test wi access, failed ($error)"); 
		# failure
		$granted = 0;
	    } else {
		# wi allowed
		$code = &unlinkObject($file, *error);
		if (!$code) {
		    &Log("WARNING: Unable to delete temporary file $file. Will try to clean this file up later");
		}
		$code = 0;
	    }
	}
    }
    
    if (!$code) {
	if (($access =~ /w/) && ($access =~ /d/)) {
	    $match++;
	    $file = $dir . "/" . $deleteFile;
	    # Need x access to delete file. So check for its existence
	    $code = &unlinkObject($file, *error);
	    if ($code == 0) {
		# failure
		&Log("Delete of $deleteFile in $dir to test wd access failed ($error)"); 
		$granted = 0;
	    } else {
		# wd allowed
		$code = 0;
	    }
	}
    }
    
    if (!$match) {
	&log("ERROR: Invalid access ($access) specified on dir $dir\n");
	$code = 1;
	$granted = 0;
    }

    $grant = $granted;
    $code;
}


# Checks access for current uid using test_access subcommand of acl_edit
# This is only a USER SPACE check NOT representative of the kernel 
# space access check.
sub testAclAccess {
    local($object, $access) = @_;
    local(@aclOut);
    local($code);
    local($result);

    $result = 1; # Assume failure
    $code = &aclEditInteractive("-n", "$object", "", "t $access \nabort", *aclOut);
    &ErrorExit("testAclAccess $object for $access", $code) if $code;
    
    foreach (@aclOut) {
	if (/GRANTED/) {
	    $result = 0;
	    last;
	}
    }

    $result;
}    

# XXX
# Phase 2: need to add realms here - requestorRealm, storingRealm, owningRealm
# $requestedAccess is in bitset format 
sub checkCalcAclAccess {
    local($object, $requestor, $requestorPgroup, $requestorSGroupsStr,
	  $requestedAccess, $owner, $ownerGroup, %acl) = @_;
    local($deny) = 1;
    local($grantedAccess); # in a bit set format;
    
    $grantedAccess = &calcAclAccess($requestor, $requestorPgroup,
				    $requestorSGroupsStr, $owner, $ownerGroup,
				    %acl);

    # Need write perm also for insert and delete
    if (-d $object) {
	if ($requestedAccess & $PERMINSERT ||
	    $requestedAccess & $PERMDELETE) {
	    $requestedAccess |= $PERMWRITE;
	}
    }
    if (($requestedAccess & $grantedAccess) == $requestedAccess) {
	$deny = 0;
    }
    $deny;
}

# XXX
# Phase 2: need to add realms here - requestorRealm, storingRealm, owningRealm
# XXX
# Check with POSIX access check algorithm
# user and group as names and not ids.
sub calcAclAccess {
    local($requestor, $requestorPGroup, $requestorSGroupsStr,
	  $owner, $ownerGroup, %acl) = @_;
    local(%userEntries, $groupEntry, %groupEntries);
    local($entry, $perm);
    local($reqGrp, @reqGrps);
    local($checkMaskObj) = 0;
    local($groupMatch) = 0;

    local($access) = $PERMNONE;

    # Check out user_obj entry
    if ($requestor eq $owner) {
	$access = $acl{'user_obj'};
    } else {
	# Figure out group and user ACL entries 
	while (($entry, $perm) = each(%acl)) {
	    if ($entry =~ /^user:(\w*)$/) {
		$userEntries{$1} = $perm;
	    } elsif ($entry =~ /^group:(\w*)$/) {
		$groupEntries{$1} = $perm;
	    }
	}
	
	# Check out the user entries
	if (defined($userEntries{$requestor})) {
	    $access = $userEntries{$requestor};
	    $checkMaskObj = 1;
	} else {
	    # Collate requestor groups
	    @reqGrps = split(' ', $requestorSGroupsStr);
	    unshift(@reqGrps, $requestorPGroup);
	    
	    # Check out the group entries
	    # POSIX and the DCE AES specifications specify that 
	    # even on matching group_obj, all other matching groups
	    # need to be considered.
	    foreach $reqGrp (@reqGrps) {
		if ($reqGrp eq $ownerGroup) {
		    $access |= $acl{'group_obj'};
		    $groupMatch = 1;
		}
		if (defined($groupEntries{$reqGrp})) {
		    $access |= $groupEntries{$reqGrp};
		    $groupMatch = 1;
		}
	    }

	    if ($groupMatch) {
		$checkMaskObj = 1;
	    } else {
		# Check out the other_obj entry
		$access = $acl{'other_obj'};
	    }
	}
	if ($checkMaskObj && defined($acl{'mask_obj'})) {
	    $access &= $acl{'mask_obj'};
	}
    }
    $access;
}

sub getUnixPermFromAclPerm {
    local($aclPerm) = @_;
    local($unixPerm);

    $unixPerm = $aclPerm & ($PERMEXEC | $PERMWRITE | $PERMREAD);
    $unixPerm;
}


sub getModeFromAcl {
    local(%acl) = @_;
    local($uo, $go, $oo, $mo);
    local($mode) = 0;

    $uo = &getUnixPermFromAclPerm($acl{"user_obj"});
    $mode = ($uo << 6);

    if (defined($acl{"mask_obj"})) {    
	$mo = &getUnixPermFromAclPerm($acl{"mask_obj"});
	$mode |= ($mo << 3);
    } else {
	$go = &getUnixPermFromAclPerm($acl{"group_obj"});
	$mode |= ($go << 3);
    }

    $oo = &getUnixPermFromAclPerm($acl{"other_obj"});
    $mode |= $oo;

    $mode;
}

# The 2 associative array arguments are by reference since in perl you 
# cannot pass more than one associative array by value. The first
# array eats up all the contents of the succeeding arrays.
# But things are ok if they are by passed via reference.
sub sameAcls {
    local(*acl1, *acl2) = @_;
    local($k1, $v1);
    local(%acl2tmp);
    local($same);

    $same = 1;
    %acl2tmp = %acl2;
    while (($k1, $v1) = each(%acl1)) {
	if ($acl2tmp{$k1} ne $v1) {
	    $same = 0;
	    last;
	}
	delete $acl2tmp{$k1};
    }
    if (%acl2tmp) {
	# The 2nd ACL has entries not in the first.
	$same = 0;
    }
    $same;
}

sub listAcl {
    local(%acl) = @_;
    local($tag, $perm, $permstr);

    foreach $tag (sort keys %acl) {
	$perm = $acl{$tag};
	$permStr = &aclPermSetToAclPermStr($perm);
	&Log("$tag:$permstr");
    }
}

# XXX Add UNIX mode bit conversion routines

# XXX Add UNIX mode bit and ACL perm bit conversion, comparision routines.

1;
