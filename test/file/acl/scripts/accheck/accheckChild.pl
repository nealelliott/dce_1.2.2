#!/afs/tr/proj/tools/bin/perl

sub sigHandle {
    local($sig) = @_;
    &Log("Child $TESTACL_DRIVER_INDEX caught SIG$sig -- exiting");
    exit(0);
}

$TESTACL_SRCDIR = $ENV{TESTACL_SRCDIR};

require "$TESTACL_SRCDIR/scripts/common/util";
require "$TESTACL_SRCDIR/scripts/common/sysUtil";
require "$TESTACL_SRCDIR/scripts/common/aclUtil";
require "$TESTACL_SRCDIR/scripts/common/rgyUtil";
require "$TESTACL_SRCDIR/scripts/common/typeLen";
require "$TESTACL_SRCDIR/scripts/common/comm";

$TESTACL_DRIVER_INDEX = $ENV{TESTACL_DRIVER_INDEX};
$SIG{'INT'} = 'sigHandle';

&unBufferFH(STDOUT);
&unBufferFH(STDERR);

$TESTACL_TEST_FILES = $ENV{TESTACL_TEST_FILES};
$TESTACL_TEST_DIRS = $ENV{TESTACL_TEST_DIRS};
$TESTACL_TEST_FILES_OWNER = $ENV{TESTACL_TEST_FILES_OWNER};
$TESTACL_TEST_FILES_OWNINGGROUP = $ENV{TESTACL_TEST_FILES_OWNINGGROUP};

@files = split(' ', $TESTACL_TEST_FILES);
@dirs = split(' ', $TESTACL_TEST_DIRS);

$TESTACL_DRIVER_USER = $ENV{TESTACL_DRIVER_USER};
$TESTACL_DRIVER_PGROUP = $ENV{TESTACL_DRIVER_GROUP};
# for now no secondary groups
$TESTACL_DRIVER_SGROUPS = "";

# XXX Is this needed
$TESTACL_DRIVER_CNT = $ENV{TESTACL_DRIVER_CNT};

$ALLPERMS = $ENV{ALLPERMS};
$INSERTFILEPREFIX = "insertFile" . "$TESTACL_DRIVER_INDEX";
$DELETEFILE = "deleteFile" . "$TESTACL_DRIVER_INDEX";

#
#  This is the "main" section
#

&Log("CHECKPOINT: Child $TESTACL_DRIVER_INDEX, pid $$ acquired desired identity");
&Log("DCE identity: principal $TESTACL_DRIVER_USER, primary group $TESTACL_DRIVER_PGROUP");

if (!$ENV{TESTACL_DEBUG}) {
    $TESTACL_DRIVER_READFD = $ENV{TESTACL_DRIVER_READFD};
    $TESTACL_DRIVER_WRITEFD = $ENV{TESTACL_DRIVER_WRITEFD};

    &Log("READ FD is $TESTACL_DRIVER_READFD");
    &Log("WRITE FD is $TESTACL_DRIVER_WRITEFD");

    open(RFH, "<&$TESTACL_DRIVER_READFD") ||
	&ErrorExit("Child $TESTACL_DRIVER_INDEX: Unable to open RFH ($!)");
    open(WFH, ">&$TESTACL_DRIVER_WRITEFD") ||
	&ErrorExit("Child $TESTACL_DRIVER_INDEX: Unable to open WFH ($!)");
    &unBufferFH(WFH);

    # Forever loop
    for ($i = 0; 1; $i++) {
	$INSERTFILE = "$INSERTFILEPREFIX" . "$i";
	$code = 0;
	$code = &pipeReadStr(RFH, *label);
	&ErrorExit("Child $TESTACL_DRIVER_INDEX, unable to read from parent. Exiting")
	    if ($code);
	&Log("CHECKPOINT: Iteration $i: Starting checks for case $label");
	foreach $object (@files, @dirs) {
	    $code = &checkEntryAllPerms($object,
					$TESTACL_TEST_FILES_OWNER,
					$TESTACL_TEST_FILES_OWNINGGROUP,
					$TESTACL_DRIVER_USER, 
					$TESTACL_DRIVER_PGROUP,
					$TESTACL_DRIVER_SGROUPS,
					"$object; $label");
	    last if ($code);
	} #object
	if (!$code) {
	    &Log("CHECKPOINT: Completed checks for case $label");
	    $code = &pipeWriteInt(WFH, 0);
	    &ErrorExit("Child $TESTACL_DRIVER_INDEX, unable to write to parent. Exiting")
		if ($code);
	} else {
	    &Log("FAILURE of checks for case $label, on $object");
	    $code = &pipeWriteInt(WFH, 1);
	    &ErrorExit("Child $TESTACL_DRIVER_INDEX, unable to write to parent. Exiting")
		if ($code);
	}
    }
} else {
    print "Child $$ here\n";
    $object = $files[0];
    $label = "DEBUG";
    $code = &checkEntryAllPerms($object,
				$TESTACL_TEST_FILES_OWNER,
				$TESTACL_TEST_FILES_OWNINGGROUP,
				$TESTACL_DRIVER_USER, 
				$TESTACL_DRIVER_PGROUP,
				$TESTACL_DRIVER_SGROUPS,
				"$object; $label");
#    while (1) {
#	&Log("Child $TESTACL_DRIVER_INDEX here");
#	if ($TESTACL_DRIVER_INDEX) {
#	    &Sleep(200);
#	} else {
#	    &Sleep(10);
#	}
#    }
    print "Child $$ exiting\n";
    exit 0;
} 

    
0;

#
# End of main section
#

#
# Subroutines
#

# checkEntryAllPerms parameters
#  $1: object
#  $1: object owner
#  $2: object owning group
#  $3: requestor
#  $4: requestor primary group
#  $5: requestor secondary groups
#  $6: entry type being tested
#  $7: label for checkpoint
sub checkEntryAllPerms {
    local($object, $objectOwner, $objectOwningGroup,
	  $requestor, $requestorPGroup, $requestorSGroupsStr,
	  $label) = @_;
    local($code, $permstr, $systemGrant, $systemDeny, $testDeny, $objectType);
    local(%acl);

    &Log("Object being tested against is $object");
    
    # XXX "c" perms functionality to be added.
    if (-d $object) {
	$objectType = "DIR";
	@probePerms = ("r", "x", "wi", "wd");
    } else {
	$objectType = "FILE";
	@probePerms = ("r", "w", "x");
    }

    # Get effective acl entry perms.
    &Log("Fetch object ACL");
    $code = &getAcl($object, "o", 1, *acl);
    if ($code) {
	&Log("ERROR: $label: unable to get object acl for $object", $code);
	return $code;
    }

    foreach $permstr (@probePerms) {
    #$permstr = $probePerms[0];
	$permset = &aclPermStrToAclPermSet($permstr);

	&Log("CHECKPOINT: $label; probe perms: $permstr");

	# check for auth	
	
	# XXX Replace this by a system call based access check
	# which would need object type to save time.
	# testAclAccess assumes that requestor is already dce_login
	# at this point here
#	$systemDeny = &testAclAccess($object, $permstr);

	# Kernel access check
	# DELETEFILE is set in the beginning of this script to allow testing
	# of delete permission in the directory and INSERTFILE for insert 
	# perm.
	$code = &kernelAclAccess($objectType, $object, $permstr, 
				       $INSERTFILE, $DELETEFILE,
				       *systemGrant);
	return $code if ($code);
	$systemDeny = !$systemGrant;
	$testDeny = &checkCalcAclAccess($object, $requestor, $requestorPGroup,
					$requestorSGroupsStr, $permset,
					$objectOwner, $objectOwningGroup,
					%acl);
	if ($systemDeny != $testDeny) {
	    local($sstr, $tstr);
	    if ($systemDeny) {
		$sstr = "no";
	    } else {
		$sstr = "yes";
	    }
	    if ($testDeny) {
		$tstr = "no";
	    } else {
		$tstr = "yes";
	    }			# 
	    &Log("ERROR: $label: access $permstr tested for $requestor on $object, Granted access $sstr, Calculated access $tstr");
	    $code = 1;
	    return $code;
	}

	# XXX for unauth access execute test with 
	# (uid, gid, cellid) triplet  of (-2, -2, -2)
    } # PERMI
}


