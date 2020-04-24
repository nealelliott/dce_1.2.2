# include in package main itself	
package main;

#
# Load specifications
# For aim3, CONCURRENCY is not useful
# DURATION becomes number of iterations
%fsetReclone1Spec = ( MIN_FSETS,		    1,
        	  );

# Define required subroutines - fsetReclone1Parse, fsetReclone1Validate, fsetReclone1Run

sub fsetReclone1Parse {
    local($loadName, $loadNumber, $start, $end) =@_;

    if ( $loadName ne "fsetReclone1" ) {
	&Exit("Load name specified ($loadName) should be fsetReclone1",
	      __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
    }

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_DURATION, SINGLE, OPTIONAL );

    &GetLoadAttr($loadName, $loadNumber, $start, $end,
		 LS_FSETNAMES, MULTIPLE, REQUIRED);
    0;
}

# Also sets defaults 
# return zero if all ok
sub fsetReclone1Validate {
    local($loadName, $loadNumber)=@_;
    local($errorCount)=0;
    local($index);

    if ( $loadName ne "fsetReclone1" ) {
		&Exit("Load name specified ($loadName) should be fsetReclone1",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
    }
	
    # Check if required specifications have been specified.
    if ( !defined($fsetReclone1Spec{MIN_FSETS}) ) {
		&Log("Required minimum value for MIN_FSETS not specified for load $loadName, instance $loadNumber");
		$errorCount++ ;
    }

    # Handle required attributes
    if ( !defined($loadAttr{$loadName, $loadNumber, LS_FSETNAMES}) ) {
		&Log("Filesets for load $loadName instance $loadNumber not specified");
		$errorCount++;
    }

    # DURATION defaults to forever.
    if ( !defined($loadAttr{$loadName, $loadNumber, LS_DURATION}) ) {
		$loadAttr{$loadName, $loadNumber, LS_DURATION}=0; 
    }

    # Find out if specified attributes are valid.

    # Check min. number of dirs has been specified
    local(*fsets)=$loadAttr{$loadName, $loadNumber, LS_FSETNAMES};

    if ( @fsets < $fsetReclone1Spec{MIN_FSETS} ) {
        &Log("Load $loadName number $loadNumber - fails MIN_FSETS ($fsetReclone1Spec{MIN_FSETS}) spec");
		$errorCount++;
    }

	# XXX Check if filesets exist 
	
    $errorCount;
}

sub fsetReclone1Run {
	local($loadName, $loadNumber)=@_;
	local($index);
	local($threadRepCnt);
	local($iter);

	if ( $loadName ne "fsetReclone1" ) {
		&Exit("Load name specified ($loadName) should be fsetReclone1",
			  __FILE__, __LINE__, $EXIT_MD_SCRIPT_ERROR);
	}
	local(*fsets)=$loadAttr{$loadName, $loadNumber, LS_FSETNAMES};
	$threadRepCnt=$loadAttr{$loadName, $loadNumber, LS_DURATION};
	&Log(sprintf("RL: Load fsetReclone1, instance $loadNumber, filesets are %s",
				 &AryToStr(@fsets)));
	if ( $threadRepCnt ) {
		&Log("RL: Load fsetReclone1, instance $loadNumber, no. of iterations: $threadRepCnt");
	} else {
		&Log("RL: Load fsetReclone1, instance $loadNumber, no. of iterations: forever");
	}
	foreach (@fsets) {
		&FsetClone($_);
	}
	if ( $threadRepCnt ) {
		foreach $iter ( 0..$threadRepCnt-1 ) {
  			&Sleep(120);
			foreach $fset (@fsets) {
				&FsetReclone($fset);
			}
		}
	} else {
		while ( 1 ) {
  			&Sleep(120);
			foreach $fset (@fsets) {
				&FsetReclone($fset);
			}
		}
	}
	0;
}

1;

__END__

# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:

