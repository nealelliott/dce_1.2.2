# include in package main itself	
package main;

##########################################################################
#
#  Setup phase subs
#
##########################################################################

sub ParseAndProcessSetup {
	&InitSetupParser;
	&ParseAggrs;
	&ParseFsets;
	&ParseFsetMntPts;
	&ParseFsetDirMaps;

	&Log("Setup processing aggregates");
	&ProcessAggrs;
	&Log("Setup processing filesets");
	&ProcessFsets;
	&Log("Setup processing filesets mount points");
	&ProcessFsetMntPts;
	&Log("Setup processing filesets directory mappings");
	&ProcessFsetDirMaps;
}


# Read config file
# Set defaults if necessary
# Validate 
# Config aggregates
# Config filesets
# Mount filesets
# Create test dirs

############################################################################
#
# Setup routines 
#
###########################################################################

sub InitSetupParser {
	@setupSectionTypes=(AGGR, FSET, FSET_MNT_PT, FSET_DIR_MAP);
	foreach (@setupSectionTypes) {
		$nextConfigLine{$_} = 0;
	}
}

# Section AGGR parsing

sub ParseAggrs{
	local($index, $sectionStart, $sectionEnd, $rangeList, @rangeAry);

	$rangeList = $sectionDefnRanges{AGGR};
	@rangeAry = split(' ', $rangeList);
	for ($index = 0; $index < @rangeAry; $index += 2) {
		$sectionStart = $rangeAry[$index];
		$sectionEnd = $rangeAry[$index+1];
		&InitAggrAttrs;
		&ParseAggrAttrs($sectionStart, $sectionEnd);		
		&CanonalizeAggrAttrs;
		&FillAggrAttrDefaults;
		&StoreAggrAttrs;
	}
}

sub InitAggrAttrs {
	undef($NOINIT);
	undef($AGGRTYPE, $CELLNAME, $MACHINE, $DEVNAME, $DEVSIZE,
		  $AGGRNAME, $AGGRID);
	undef($BLKSIZE, $FRAGSIZE);
	undef($FSETNAME);
}

sub CanonalizeAggrAttrs {
	if (index($DEVNAME,"/") != 0) {
		$DEVNAME = "/dev/".$DEVNAME;
	}
	if ($CELLNAME eq "localCell") {
		open(FH, "/opt/dcelocal/dce_cf.db") || 
			&Exit("Cannot read /opt/dcelocal/dce_cf.db", __FILE__, __LINE__);
		while (<FH>) {
		if (m#^cellname[ \t]*/\.\.\./#) {
				$CELLNAME = $';  #';
				last;
			}
		}
		close(FH);
	}
}

sub FillAggrAttrDefaults {

	# AggrName default
	$NOINIT || ($NOINIT = 0);

	$MACHINE || ($MACHINE = localHost);
	$CELLNAME || ($CELLNAME = localCell);
	$DEVSIZE || ($DEVSIZE = 0);
	$AGGRNAME || ($AGGRNAME = &Tail($DEVNAME));
	if ($AGGRTYPE eq "LFS") {
		$BLKSIZE || ($BLKSIZE=8192);
		$FRAGSIZE || ($FRAGSIZE=1024);
	} else {
		$FSETNAME || ($FSETNAME = "UfsFset_$AGGRID");
	}
}

# Aggr attr arrays:
# %aggrType, key aggrname
# %aggrGenAttr, key aggrname
# %aggrLFSAttr, key aggrnam

sub StoreAggrAttrs {
	local(@aggrAttr);

	@aggrAttr=($AGGRTYPE, $NOINIT, $CELLNAME, $MACHINE, $DEVNAME,
			   $AGGRID);
	$aggrGenAttr{$AGGRNAME} = join(' ', @aggrAttr);
	if ($AGGRTYPE eq "LFS") {
		@aggrAttr = ($BLKSIZE, $FRAGSIZE);
		$aggrLFSAttr{$AGGRNAME} = join(' ', @aggrAttr);
	} else {
		# only 1 specific attribute so far.
		$aggrUFSAttr{$AGGRNAME} = $FSETNAME;
	}
};

sub ParseAggrAttrs {
	local($firstLine, $lastLine) = @_;

	&GetAttr(NOINIT, FLAG, OPTIONAL, *NOINIT, $firstLine, $lastLine);
	&GetAttr(AGGRTYPE, SINGLE, REQUIRED, *AGGRTYPE, $firstLine, $lastLine);
	&GetAttr(CELLNAME, SINGLE, REQUIRED, *CELLNAME, $firstLine, $lastLine);
	&GetAttr(AGGRID, SINGLE, REQUIRED, *AGGRID, $firstLine, $lastLine);
 	&GetAttr(DEVNAME, SINGLE, REQUIRED, *DEVNAME, $firstLine, $lastLine);
# XXXX
	&GetAttr(MACHINE, SINGLE, OPTIONAL, *MACHINE, $firstLine, $lastLine);
	&GetAttr(DEVSIZE, SINGLE, OPTIONAL, *DEVSIZE, $firstLine, $lastLine);
	&GetAttr(AGGRNAME, SINGLE, OPTIONAL, *AGGRNAME, $firstLine, $lastLine);


	if ($AGGRTYPE eq "LFS") {
    	# aggregate type specific attrs parsing
		&GetAttr(BLKSIZE, *BLKSIZE, OPTIONAL, $firstLine, $lastLine);
		&GetAttr(FRAGSIZE, *FRAGSIZE, OPTIONAL, $firstLine, $lastLine);	
	} else {
		# UFS aggregate
		&GetAttr(FSETNAME, *FSETNAME, OPTIONAL, $firstLine, $lastLine);
	}
}	

# Section FSET parsing

sub ParseFsets{
	local($index, $sectionStart, $sectionEnd, $rangeList, @rangeAry);

	$rangeList = $sectionDefnRanges{FSET};
	@rangeAry = split(' ', $rangeList);
	for ($index = 0; $index < @rangeAry; $index += 2 ) {
		$sectionStart = $rangeAry[$index];
		$sectionEnd = $rangeAry[$index+1];
		&InitFsetAttrs;
		&ParseFsetAttrs($sectionStart, $sectionEnd);
		&CanonalizeFsetAttrs;
		&FillFsetAttrDefaults;
		&ValidateFsetAttrs;
		&StoreFsetAttrs;
	}
}

sub InitFsetAttrs {
	undef($NOINIT, $DISALLOWOVERALLOC);
	undef($MACHINE, $FSETNAME, $FSETQUOTA);
}

sub CanonalizeFsetAttrs {
	if ($FSETQUOTA) {
		$FSETQUOTA = &SizeInKB($FSETQUOTA);
	}
}

sub FillFsetAttrDefaults {
	$NOINIT || ($NOINIT = 0);
	$DISALLOWOVERALLOC || ($DISALLOWOVERALLOC = 0);
	$FSETQUOTA || ($FSETQUOTA = 5*1024); # in Kilo bytes i.e. 5M
}

sub	ValidateFsetAttrs {
	local(@aggr);

	# aggregate for the fileset
	grep($AGGRNAME eq $_, keys(%aggrGenAttr)) ||
		&Exit("Unknown aggregate ($AGGRNAME) specified for fileset $FSETNAME",
			  __FILE__, __LINE__, $EXIT_CONFIG_FILE_ERROR);

}

sub StoreFsetAttrs {
	local(@fsetAttr);

	@fsetAttr = ($NOINIT, $DISALLOWOVERALLOC, $AGGRNAME, $FSETQUOTA);
	$fsetAttr{$FSETNAME} = join(' ', @fsetAttr);
}

sub ParseFsetAttrs {
	local($firstLine, $lastLine) = @_;

	&GetAttr(NOINIT, FLAG, OPTIONAL, *NOINIT, $firstLine, $lastLine);
	&GetAttr(DISALLOWOVERALLOC, OPTIONAL, FLAG,*NOINIT, $firstLine, $lastLine);
	&GetAttr(AGGRNAME, SINGLE, REQUIRED, *AGGRNAME, $firstLine, $lastLine);
	&GetAttr(FSETNAME, SINGLE, REQUIRED, *FSETNAME, $firstLine, $lastLine);
	&GetAttr(FSETQUOTA, SINGLE, OPTIONAL, *FSETQUOTA, $firstLine, $lastLine);
}

# Section FSET_MNT_PT

sub ParseFsetMntPts{
	local($index, $sectionStart, $sectionEnd, $rangeList, @rangeAry);

	$rangeList = $sectionDefnRanges{FSET_MNT_PT};
	@rangeAry = split(' ', $rangeList);
	for ($index = 0; $index < @rangeAry; $index += 2) {
		$sectionStart = $rangeAry[$index];
		$sectionEnd = $rangeAry[$index+1];
		&InitFsetMntPtAttrs;
		&ParseFsetMntPtAttrs($sectionStart, $sectionEnd);
		&CanonalizeFsetMntPtAttrs;
		&FillFsetMntPtAttrDefaults;
		&ValidateFsetMntPtAttrs;
		&StoreFsetMntPtAttrs;
	}
}

sub InitFsetMntPtAttrs {
	undef($FSETNAME, $FSETMNTDIR);
}

sub CanonalizeFsetMntPtAttrs {
# do nothing for now
}

sub FillFsetMntPtAttrDefaults {
# do nothing for now
}

sub	ValidateFsetMntPtAttrs {
	local(@aggr);

	# aggregate for the fileset
	grep($FSETNAME eq $_, keys(%fsetAttr)) ||
		&Exit("Unknown fileset ($FSETNAME) specified",
			  __FILE__, __LINE__, $EXIT_CONFIG_FILE_ERROR);

}

sub StoreFsetMntPtAttrs {
	local($list) = $fsetMntPts{$FSETNAME};

	if (! $list) {
		$fsetMntPts{$FSETNAME} = $FSETMNTDIR;
	} else {
		$fsetMntPts{$FSETNAME} = join(' ', $list, $FSETMNTDIR);
	}
}

sub ParseFsetMntPtAttrs {
	local($firstLine, $lastLine) = @_;

	&GetAttr(FSETNAME, SINGLE, REQUIRED, *FSETNAME, $firstLine, $lastLine);
	&GetAttr(FSETMNTDIR, SINGLE, REQUIRED, *FSETMNTDIR, $firstLine, $lastLine);
}

#SECTION FSET_DIR_MAP

sub ParseFsetDirMaps {
	local($index, $sectionStart, $sectionEnd, $rangeList, @rangeAry);

	$rangeList = $sectionDefnRanges{FSET_DIR_MAP};
	@rangeAry = split(' ', $rangeList);
	for ($index = 0; $index < @rangeAry; $index += 2) {
		$sectionStart = $rangeAry[$index];
		$sectionEnd = $rangeAry[$index+1];
		&InitFsetDirMapAttrs;
		&ParseFsetDirMapAttrs($sectionStart, $sectionEnd);
		&CanonalizeFsetDirMapAttrs;
		&FillFsetDirMapAttrDefaults;
		&ValidateFsetDirMapAttrs;
		&StoreFsetDirMapAttrs;
	}
	&RevisitFsetDirMapDefaults;
}

sub InitFsetDirMapAttrs {
	undef($FSETNAME, $DIRNAME, $DIRQUOTA);
}

sub CanonalizeFsetDirMapAttrs {
	local($dirNameLength) =  length($DIRNAME);

	# Remove trailing / from dir name if there is one
	if (rindex($DIRNAME,"/") == $dirNameLength - 1 ) {
		$DIRNAME= substr($DIRNAME, 0, $dirNameLength - 1);
	}
	# Handle K, or M suffixes in DIRQUOTA
	$DIRQUOTA = &SizeInKB($DIRQUOTA);
}

sub FillFsetDirMapAttrDefaults {
	# special marker
	$DIRQUOTA || ( $DIRQUOTA = -1 );
}

sub	ValidateFsetDirMapAttrs {
	local(@aggr);

	# aggregate for the fileset
	grep($FSETNAME eq $_, keys(%fsetAttr)) ||
		&Exit("Unknown fileset ($FSETNAME) specified",
			  __FILE__,__LINE__,$EXIT_CONFIG_FILE_ERROR);

}

# Need to think of another array name each time a new fset is encountered
sub StoreFsetDirMapAttrs {
	if (!defined($DIRQUOTA)) {
		&Exit("Internal error, value of variable DIRQUOTA not defined",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}

	if (!defined($fsetDirMaps{$FSETNAME})) {
        # global string variable to be used only by this procedure.
		# It is used to generate a new array name each time a new fileset
		# is encountered. The array will hold attributes of the
		# directory e.g. name, quota etc.
		# New names are required for each fileset as we store only array
		# pointers.
		if (!defined($fsetDirMapAttrStr)) {
			$fsetDirMapAttrStr = "fsetDirMap0000";
		} else {
			$fsetDirMapAttrStr++;
		}
		local(*dirMapAttr) = eval("\*$fsetDirMapAttrStr");
		@dirMapAttr = ($DIRNAME, $DIRQUOTA);
		$fsetDirMaps{$FSETNAME} = *dirMapAttr;
	} else {
		local(*dirMapAttrAry) = $fsetDirMaps{$FSETNAME};
		push(@dirMapAttrAry, $DIRNAME, $DIRQUOTA);
	}
}

sub ParseFsetDirMapAttrs {
	local($firstLine, $lastLine) = @_;

	&GetAttr(FSETNAME, SINGLE, REQUIRED, *FSETNAME, $firstLine, $lastLine);
	&GetAttr(DIRNAME, SINGLE, REQUIRED, *DIRNAME, $firstLine, $lastLine);
	&GetAttr(DIRQUOTA, SINGLE, OPTIONAL, *DIRQUOTA, $firstLine, $lastLine);
}

# All dirs for which quotas were not specified were marked with
# a quota of -1. Split unallocated space if fileset among these dirs
# equally.
sub RevisitFsetDirMapDefaults {
	local($fsetName, @fsetNames);
	local($fsetQuota, $defaultDirQuota);
	local($fsetQuotaAlloc) = 0;
	local($dirsWithoutQuota);
	local($index);

	@fsetNames = keys(%fsetAttr);
	foreach $fsetName (@fsetNames) {
		next if ! defined $fsetDirMaps{$fsetName};
		local(*fsetDirs) = $fsetDirMaps{$fsetName};
		if (!&IsEven(@fsetDirs)) {
			&Exit("Internal Script error",
				  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
		}
		$dirsWithoutQuota = @fsetDirs / 2;	
		for ($index = 1; $index < @fsetDirs; $index += 2) {
			if ($fsetDirs[$index] != -1) {
				$dirsWithoutQuota--;
				$fsetQuotaAlloc += $fsetDirs[$index];
			}
		}
		$fsetQuota = &FsetToQuota($fsetName);
		if ($dirsWithoutQuota) {
			$defaultDirQuota =
				($fsetQuota - $fsetQuotaAlloc) / $dirsWithoutQuota;
			for ($index = 1; $index < @fsetDirs; $index += 2 ) {
				next if ($fsetDirs[$index] != -1);
				$fsetDirs[$index]=$defaultDirQuota;
			}
		}
	}
}


#---------------------------------------------------------------------------

# Process functions
#	ignore MACHINE 
sub ProcessAggrs {
	local($aggrName, $aggrAttrs);
	local($aggrType, $aggrNoInit, $cellName, $machine, $devName, $aggrId);
	local($blkSize, $fragSize);
	local($fsetName);
	local($dfstabModified) = 0;
	local($dfstabRead) = 0;
	local($exportAggr) = 0;
	local(%dfstab);

	while (($aggrName, $aggrAttrs) = each(%aggrGenAttr)) {
		($aggrType, $aggrNoInit, $cellName, $machine, $devName, $aggrId) =
			split(' ', $aggrAttrs);
		if ($aggrType eq "LFS") {
			($blkSize, $fragSize) = split(' ', $aggrLFSAttr{$aggrName});
		} else {
			# UFS aggr
			$fsetName = $aggrUFSAttr{$aggrName};
		}

		if (!$aggrNoInit) {
			if ($aggrType eq "LFS") {
				&CreateLFSAggr($devName, $blkSize, $fragSize);
				if ($cellName eq noExport) {
					&AttachSALFSAggr($devName,$aggrName,$aggrId);
				} else {
					&ParseDfstab(*dfstab);
					&PrepareToAttachDFSLFSAggr($devName, $aggrName,
											   $aggrId, *dfstab,
											   *dfstabModified);
					$exportAggr++;
				}
			} else {
				if ($cellName eq noExport) {
					# $aggrName has to be mount point.
					&MountSAUFSAggr($devName, $aggrName);
				} else {
					&ParseDfstab(*dfstab);
					&PrepareToAttachDFSUFSAggr($devName, $aggrName,
											   $aggrId, $fsetName,
											   *dfstab, *dfstabModified);
					$exportAggr++;
				}
			}
		} else {
			if (!&IsAttachedAggr($aggrName)) {
				if ($aggrType eq "LFS") {
					if (!&RecoverAggr($aggrName)) {
						&SalvageAggr($aggrName);
					}
					if ($cellName eq noExport) {
						&AttachSALFSAggr($devName, $aggrName, $aggrId);
					} else {
						&ParseDfstab(*dfstab);
						&PrepareToAttachDFSLFSAggr($devName, $aggrName,
												   $aggrId, *dfstab,
												   *dfstabModified);
						$exportAggr++;
					}
				} else {
					if ($cellName eq noExport) {
                        # $aggrName has to be mount point.
						&MountSAUFSAggr($devName, $aggrName);
					} else {
						&ParseDfstab(*dfstab);
						&PrepareToAttachDFSUFSAggr($devName, $aggrName,
												   $aggrId, $fsetName,
												   *dfstab,
												   *dfstabModified);
						$exportAggr++;
					}
				}
			}
			# nothing else to do for now
		}
	}
	if ($dfstabModified) {
		&WriteDfstab($DFSTABFILE, 2, %dfstab);
	}
	if ($exportAggr) {
		&AttachDfsAggr;
	}
}

# Accesses global $dfstabRead, $DFSTABFILE
sub ParseDfstab {
	local(*dfstab)=@_;
	if (!$dfstabRead) {
		if (-e $DFSTABFILE) {
			&CopyFile($DFSTABFILE, $DFSTABFILE.BAK.$$);
		}
		&ReadDfstab($DFSTABFILE, 2, *dfstab);
		$dfstabRead++;
	}
}

sub ProcessFsets {
	local($fsetName, $fsetAttrs);
	local($fsetNoInit, $fsetAllocPolicy, $aggrName, $fsetQuota);

	while (($fsetName, $fsetAttrs) = each(%fsetAttr) ) {
		($fsetNoInit, $fsetAllocPolicy, $aggrName, $fsetQuota) =
			split(' ', $fsetAttrs);
		if (!$fsetNoInit) {
			&CreateLFSFset($aggrName, $fsetName, $fsetQuota);
		} else {
			if (!&IsValidLFSFset($aggrName,$fsetName)) {
				&Exit("Non-existent fileset $FSETNAME specified",
					  __FILE__, __LINE, $EXIT_SETUP_ERROR);
			}
		}
	}
}

sub ProcessFsetMntPts {
#	ignore FSETRANGE
	local($fsetName, $mntPts, @mntPts);
	local($mntPtParentDir);

	while (($fsetName, $mntPts) = each(%fsetMntPts)) {
		@mntPts = split(' ',$mntPts);
		foreach (@mntPts) {
			$mntPtParentDir = &ParentDir($_);
			if (&IsDfsDir($mntPtParentDir)) {
				&DfsMountFset($fsetName, $_, $EXIT_SETUP_ERROR);
			} else {
				if  (! -d $_) {
					&MakeDir($_);
				}
				&LocalMountFset($fsetName, $_, $EXIT_SETUP_ERROR);
			}
		}
	}
}

# Also sets up a mapping between dir names and their quotas
sub ProcessFsetDirMaps {
	local(@fsetMntDirs, $fsetMntDir);
	local($index);
	local($fsetName, @fsetNames);

	@fsetNames = keys(%fsetDirMaps);
	# Get 1st mnt pnt
	foreach $fsetName (@fsetNames) {
		local(*dirs) = $fsetDirMaps{$fsetName};
		@fsetMntDirs = split(' ', $fsetMntPts{$fsetName});
		$fsetMntDir = shift(@fsetMntDirs);
        # No trailing / 
		if (rindex($fsetMntDir,"/") == length($fsetMntDir) - 1) {
			&Exit("Internal script error",
				  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
		}
		for ($index = 0; $index < @dirs; $index += 2) {
			$dir = $fsetMntDir . "/" . $dirs[$index];
			&MakeDir($dir);

			# Set quotas for all possible mount points of fileset
			$dirQuota{$dir} = $dirs[$index+1];
			shift(@fsetMntDirs);
			foreach (@fsetMntDir) {
				if (rindex($_,"/") == length($_) - 1) {
					&Exit("Internal script error ($_)",
						  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
				}
				$dir = $_ . "/" . $dirs[$index];
				$dirQuota{$dir} = $dirs[$index+1];
			}
		}
	}
}

1;

# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:
