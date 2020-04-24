# include in package main itself	
package main;

##########################################################################
#
#  Process load phase subs
#
##########################################################################
# Find load name from config file 
# require load.pl
# Call subroutine &loadParse to parse config file
#    Will create a database - an 3D array
#    D1 = loadsuite name
#    D2 = loadsuite instance # <starts at 1 onwards , 1,2...>
#    D3 = loadsuite attribute name
#    Value of array will be attribute value.
# Call subroutine &loadValidate to validate configuration and pass in 
#    the generated associative array.
#    Validation will require reading in another specification file
#    that specifies min values.
#    
# Call subrouting &loadRun to run load and pass in the generated associative
#    array.

sub ParseAndProcessLoads {
	local($index, $sectionStart, $sectionEnd, $rangeList, @rangeAry);
	local($load);
	local($loadPkgFile, @loadFuncs);
	
	$rangeList=$sectionDefnRanges{LOADSUITE};
	@rangeAry=split(' ',$rangeList);
	&Log("Validating All Loads");
	for ($index=0; $index < @rangeAry; $index += 2 ) {
		$sectionStart = $rangeAry[$index];
		$sectionEnd = $rangeAry[$index+1];
		split(' ',$configFileAry[$sectionStart]);
		$load=$_[2];
		if ( !defined($loadCnt{$load}) ) {
			$loadCnt{$load}=0;
		} else {
			$loadCnt{$load}++;
		};
		if (!$load) {
			&Exit("Load Name not specified at line $sectionStart of $CONFIG_FILE", __FILE__, __LINE__, $EXIT_CONFIG_FILE_ERROR);
		}
		if ( $loadCnt{$load} == 0 ) {
			# Encountered load the first time
			$loadPkgFile=$load . ".pl";
			require $loadPkgFile;
		
			# Check if required functions were defined
			@loadFuncs=( "${load}Parse", "${load}Validate", "${load}Run" );
			foreach (@loadFuncs) {
				if (!defined(&$_)) {
					&Exit("Required subroutine $_ not defined in $loadPkgFile",
						  __FILE__, __LINE__, $EXIT_LOAD_VALIDATION_ERROR);
				}
			}
		}
		
        # parse by calling subroutine ${load}Parse in load package.
		$res = eval("\&${load}Parse($load,$loadCnt{$load}, $sectionStart, $sectionEnd)");
		if ( !defined($res) ) {
			die($@);
		} elsif ( $res ) {
			&Exit(sprintf("Parsing failed for load $load ($CONFIG_FILE lines %d- %d)", &cfgLine($sectionStart), &cfgLine($sectionEnd)), __FILE__, __LINE__, $EXIT_LOAD_PARSE_ERROR);
		}		

		# validate 
		$res = eval("\&${load}Validate($load, $loadCnt{$load})");
		if ( !defined($res) ) { 
			die($@) ;
		} elsif ( $res ) {
			&Exit(sprintf("Validation failed for load $load ($CONFIG_FILE lines %d - %d)", &cfgLine($sectionStart), &cfgLine($sectionEnd)), __FILE__, __LINE__, $EXIT_LOAD_VALIDATION_ERROR);
		}
	}
	
	# Run loads 
	&Log("Running Loadsuites");
	&RunLoads;
}

sub RunLoads {
	local($load,$cnt);
	local($index, $pid, $loadLogFile);
	
	# $cnt starts from zero. therefore <= in for statement
	while ( ($load, $cnt) = each(%loadCnt) ) {
		&Log("Running $load loadsuite");
		for ($index=0; $index <= $cnt; $index++ ) {
			$loadLogFile="${LOGDIR}/${load}.i${index}";
 			if ( $pid = fork() ) {
				&Log("Forked load $load, instance $index (pid = $pid), log in $loadLogFile");
				$load{$pid}=join(' ', $load, $index);
			} else {
	            &SetLogFile($loadLogFile,0);
	            &Log("RL: Load $load, instance $index starting");
				sleep(10);
				$res=eval("\&${load}Run($load,$index)");				
				if ($@) {
					&Exit("Load $load instance $index failed ($@)",
						  __FILE__, __LINE__, $EXIT_LS_FAIL);
				} else {
					if ($res) {
						&Exit("Load $load instance $index failed",
							  __FILE__, __LINE__, $EXIT_LS_FAIL);
					} else {
						&Log("RL: Load $load instance $index passed");
						exit($EXIT_SUCCESS);
					}
				}
			}
		}
	}

	&Log("Waiting for all loads to complete");
	while ( ($dead = wait) != -1 ) {
		if ( !defined($load{$dead}) ) {
			&Log("Internal error - completed load process (pid $dead) not in driver's process table, in file __FILE__ at line __LINE__");
		} else {
			($load,$index)=split(' ', $load{$dead});
			if ( $? ) {
				&Log("Load $load, instance $index (pid $dead) failed, exit status $?");
                # remove $dead from @pids
				undef($load{$dead});
			} else {
				&Log("Load $load, instance $index (pid $dead) passed");
			}
		}
	}
# XXX some problems here.
	&Log("All loads completed");	
}

1;

# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:
