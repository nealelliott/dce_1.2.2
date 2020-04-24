# include in package main itself	
package main;

#---------------------------------------------------------------------------
# Predicates
sub IsAttachedAggr {
	local($aggr) = @_;
	local($aggrDev, $fnd, @attachedAggrList);

	$fnd = 0;
	$aggrDev = &GetAggrDev($aggr); 
	open(PH, "efts lsaggr |");
	@attachedAggrList = <PH>;
	foreach (@attachedAggrList) {
		$dev = ( split )[0];
		if ($dev eq $aggrDev) {
			$fnd = 1;
			last;
		}
	}
	close(PH);
	$fnd;
}

sub IsValidLFSFset {
	local($aggr, $fset) = @_;
	local($fnd, @fsetList, $existFset);

	$fnd=0;
	open(PH, "efts lsft $aggr |");
	@fsetList = <PH> ;
	foreach (@fsetList) {
		$existFset = ( split )[0];
		if ($existFset eq $fset) {
			$fnd = 1;
			last;
		}
	}
	close(PH);
	$fnd;
}

sub IsDfsDir {
	local($dir) = @_;
	local($originalDir, $canonicalDir);
	local($result) = 0;

	chop($originalDir = `pwd`);
	chdir($dir);
	chop($canonicalDir = `pwd`);
	chdir($originalDir);
	if ($canonicalDir =~ m#^/\.\.\.#) {
	    $result = 1;
	}
	$result;
}


#--------------------------------------------------------------------------

# Aggr process functions
sub CreateLFSAggr {
	local($devName, $blkSize, $fragSize) = @_;

	&Sys("newaggr $devName $blkSize $fragSize -overwrite") && 
		&Exit("Could not initialize aggregate $devName",
			  __FILE__, __LINE__, $EXIT_SETUP_ERROR);
}

sub AttachSALFSAggr {
	local($devName, $aggrName, $aggrId) = @_;
	
	&Sys("efts attach -device $devName -name $aggrName -id $aggrId") &&
		&Exit("Could not attach aggregate $aggrName",
			  __FILE__, __LINE__, $EXIT_SETUP_ERROR);
}

sub PrepareToAttachDFSLFSAggr {
	local($aggrDev, $aggrName, $aggrId, *dfstab, *dfstabModified) = @_;
	local($dfstabAggrDev, $dfstabAggrType, $dfstabAggrId);

	if (!$dfstab{$aggrName} ||
		((($dfstabAggrDev, $dfstabAggrType, $dfstabAggrId) =
		  split(' ',$dfstab{$aggrName})),
		 ($dfstabAggrDev ne $aggrDev
		  || $dfstabAggrType ne "lfs"
		  || $dfstabAggrId != $aggrId))) {
		$dfstab{$aggrName} = join(' ', $aggrDev, "lfs", $aggrId);
		$dfstabModified++;
	}
}

sub PrepareToAttachDFSUFSAggr {
	local($aggrDev, $aggrFsetMntPt, $aggrId, $aggrFsetName, *dfstab,
		  *dfstabModified) = @_;
	local($fsetId);
	local($dfstabAggrDev, $dfstabAggrType, $dfstabAggrId);
	local($machine);

	chop($machine = `hostname`);
	# Need to add support for existing fldb entries.	

	if (!($fsetId = &CreateFldbEntry($aggrFsetName, $machine, $aggrId))) {
		&Exit("Could not create fldb entry for UFS aggregate $aggrDev",
			  __FILE__, __LINE__, $EXIT_SETUP_ERROR);
	}

	$dfstab{$aggrFsetMntPt} =join(' ', $aggrDev, "ufs", $aggrId,
								  "$fsetId");
	$dfstabModified++;
}

# return fsetid string
sub CreateFldbEntry {
	local($fsetName, $server, $aggrId) = @_;
	local(@fsetEntry, $fsetId);

	&Sys("fts crfldbentry -ftname $fsetName -server $server -aggrid $aggrId")
		&& &Exit("Could not create FLDB entry for UFS aggregate $aggrName",
				 __FILE__, __LINE__,$EXIT_SETUP_ERROR);

	open(PH,"fts lsfldb $fsetName |");
	@fsetEntry = <PH>;
	foreach (@fsetEntry) {
		if (/readWrite/) {
			split(' '); 		# ignore leading whitespace;
			$fsetId=$_[2];
			last;
		}
	}
	$fsetId;
}

sub AttachDfsAggr {
	local($aggrName) = @_;
	if (!$aggrName) {
		&Sys("dfsexport -all") &&
			&Exit("Could not export DFS aggregates",
				  __FILE__, __LINE__, $EXIT_SETUP_ERROR);
	} else {
		&Sys("dfsexport $aggrName") &&
			&Exit("Could not export DFS aggregate $aggrName",
				  __FILE__, __LINE__, $EXIT_SETUP_ERROR);
	}
}

sub RecoverAggr {
	local($aggr) = @_;
	local($res) = 0;

	$res = &Sys("salvage -aggregate $AGGRNAME -recover -verify");
}

sub SalvageAggr {
	local($aggr) = @_;

	&Sys("salvage -aggregate $AGGRNAME -salvage") &&
		&Exit("Could not salvage aggregate $aggrName",
			  __FILE__, __LINE__, $EXIT_SETUP_ERROR);
}

# Fset process functions.

sub CreateLFSFset {
	local($aggrName, $fsetName, $fsetQuota)=@_;
	local($machine);

	chop($machine=`hostname`);
	if (&IsDfsExportedAggr($aggrName)) {
		&Sys("fts create $fsetName -server $machine -aggregate $aggrName") &&
			&Exit("Could not create DFS LFS fileset $fsetName",
				  __FILE__, __LINE__, $EXIT_SETUP_ERROR);			
		if ($fsetQuota) {
			&Sys("fts setquota -fileset $fsetName -size $fsetQuota") && 
				&Exit("Could not set quota for DFS LFS fileset $fsetName",
					  __FILE__, __LINE__, $EXIT_SETUP_ERROR);			
		}
	} else {
		if ($fsetQuota) {
			&Sys("efts create -aggregate $aggrName -name $fsetName -quota $fsetQuota") && 
				&Exit("Could not create SA LFS fileset $fsetName",
				  __FILE__, __LINE__, $EXIT_SETUP_ERROR);			
		} else {
			&Sys("efts create -aggregate $aggrName -name $fsetName") &&
				&Exit("Could not create SA LFS fileset $fsetName",
					  __FILE__, __LINE__, $EXIT_SETUP_ERROR);			
		}
	}
}

sub DeleteLFSFset {
	local($aggrName, $fsetName) = @_;
	local($machine);
	
	chop($machine = `hostname`);
	if (&IsDfsExportedAggr($aggrName)) {
		&Sys("fts delete $fsetName -server $machine -aggregate $aggrName") &&
			&Exit("Could not delete DFS LFS fileset $fsetName",
				  __FILE__, __LINE__, $EXIT_LOAD_FAIL);			
	} else {
		&Sys("efts delete -aggregate $aggrName -name $fsetName") && 
			&Exit("Could not delete SA LFS fileset $fsetName",
				  __FILE__, __LINE__, $EXIT_LOAD_FAIL);			
	}
}

sub LocalMountFset {
	local($fsetName, $mntDir, $error) = @_;

	&Sys("epimount $fsetName $mntDir") &&
		&Exit("Could not locally mount fileset $fsetName",
			  __FILE__, __LINE__, $error);
}

sub LocalMountFset2 {
	local($fsetName, $mntDir, $error) = @_;

	&Sys("epimount $fsetName $mntDir") &&
		&Exit("Could not locally mount fileset $fsetName",
			  __FILE__, __LINE__, $error);
}

sub LocalUnmountFset {
	local($fsetMntPt, $error) = @_;

	&Sys("epiunmount $fsetMntPt") &&
		&Exit("Could not locally unmount fileset at local dir $fsetMntPt",
			  __FILE__, __LINE__, $error);			  
}

sub DfsMountFset {
	local($fsetName, $mntDir) = @_;
	local($device, $aggr);

    &Sys("fts crmount -dir $mntDir -fileset $fsetName") &&
		&Exit("Could not mount fileset $fsetName in DFS",
			  __FILE__, __LINE__, $error);
}

sub DfsUnmountFset {
	local($fsetMntPt, $error) = @_;

	&Sys("fts delmount $fsetMntPt") &&
		&Exit("Could not unmount fileset at DFS dir $fsetMntPt",
			  __FILE__, __LINE__, $error);			  
}

# $cloneFsetName is optional
sub FsetClone {
	local($srcFsetName, $cloneFsetName) = @_;
	local($aggrName);
	local($cloneStr) = "";	
	
	$aggrName = &FsetToAggrName($srcFsetName);
	if (&IsDfsExportedAggr($aggrName)) {
		$cloneFsetName &&
			&Exit("Cannot specify clone fileset name for \"fts clone\"",
				  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
		&Sys("fts clone -fileset $srcFsetName") &&
			&Exit("Could not fts clone fileset $fsetName",
				  __FILE__, __LINE__, $?);
	} else {
		if ($cloneFsetName) {
			$cloneStr = "-destname $cloneFsetName";
		}
		&Sys("efts clone -aggregate $aggrName -srcname $srcFsetName $cloneStr")
			&& &Exit("Could not efts clone fileset $fsetName ($?)",
					 __FILE__, __LINE__, $?);
	}
}

sub FsetReclone {
	local($fsetName) = @_;
	local($aggrName);
	$aggrName = &FsetToAggrName($fsetName);
	if (&IsDfsExportedAggr($aggrName)) {
		# A second fts clone is a reclone operation actually
		&Sys("fts clone -fileset $fsetName") &&
			&Exit("Could not fts reclone (via fts clone) fileset $fsetName",
				  __FILE__, __LINE__, $?);
	} else {
		&Sys("efts reclone -aggregate $aggrName -name $fsetName") && 
			&Exit("Could not efts clone fileset $fsetName ($?)",
				  __FILE__, __LINE__, $?);
	}
}

sub FsetReclone2 {
	local($aggrName, $fsetName) = @_;
	if (&IsDfsExportedAggr($aggrName)) {
		# A second fts clone is a reclone operation actually
		&Sys("fts clone -fileset $fsetName") &&
			&Exit("Could not fts reclone (via fts clone) fileset $fsetName",
				  __FILE__, __LINE__, $?);
	} else {
		&Sys("efts reclone -aggregate $aggrName -name $fsetName") && 
			&Exit("Could not efts clone fileset $fsetName ($?)",
				  __FILE__, __LINE__, $?);
	}
}

# Uses global arrays @lineContents, @comments
sub ReadDfstab {
	local($dataFile, $keyField, *ary) = @_;
	local(@fields);
	local($line) = 0;

	open(DH, "<$dataFile");
	while (<DH>) {
		chop($_);
		if (/^#/ || /^[ \t]*#/) {
			push(@comments, $_);
			$lineContents[$line++] = "_C";
		} elsif (/^[ \t]*$/) {
			$lineContents[$line++] = "_B";
		} else {
            # ignore leading spaces
			@fields = split(' ');
			$key = $fields[$keyField - 1];
			$ary{$key} =
				(splice(@fields, $keyField - 1, 1), join(' ', @fields));
			$lineContents[$line++] = $key;
		}
	}
	close(DH);
}

sub WriteDfstab {
	local($dataFile, $keyField, %ary) = @_;
	local($key, $val, $index, $loc);
	local(@fields, $record);

	if ($DEBUG) { &Log("Wrote file $dataFile"); return };
	$result = 1;
	open(DH, ">$dataFile") ;
	$save=$";    #";
	$"="\t";     #";
	
	for ($index = $[; $index <= $#lineContents; $index++) {
		$loc = $lineContents[$index];
		if ($loc eq "_B") {
			$record = "";
		} elsif ($loc eq "_C") {
			$record = shift(@comments); 
		} else {
			$val = $ary{$loc};
			@fields = split(' ',$val);
			$record = "";
			$record .= "@fields[0..$keyField-2]"."\t" if ($keyField != 1);
			$record .= "$loc";
			$record .= "\t" . "@fields[$keyField-1..$#fields]"
				if ($keyField <= @fields);
		}		
		print(DH "$record\n");
	}
	$" = $save;   #";
	close(DH);
}

#---------------------------------------------------------------------------
# database access functions

sub IsDfsExportedAggr {
	local($aggr) = @_;
    local($type);
    $type = (split(' ', $aggrGenAttr{$aggr}))[2];
	($type ne noExport);
}

sub IsLFSAggr {
	local($aggr) = @_;
	local($type);

    $type = (split(' ', $aggrGenAttr{$aggr}))[0];
	$type eq LFS;
}

sub AggrToDeviceName {
	local($aggr) = @_;
	local($dev);

	$dev = (split(' ', $aggrGenAttr{$aggr}))[4];
}

sub FsetToAggrName {
	local($fset) = @_;
	local($aggr);

    $aggr = (split(' ', $fsetAttr{$fset}))[2];
}

sub FsetToQuota {
	local($fset) = @_;
	local($quota);

    $quota = (split(' ',$fsetAttr{$fset}))[3];
}

sub DirToQuota {
	local($dir) = @_;
	local($quota) = $dirQuota{$dir};
	$quota;
}

1;

# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:
