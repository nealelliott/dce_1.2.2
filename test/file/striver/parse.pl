# include in package main itself	
package main;

# parse routines
sub InitParser {
	@sectionTypes=(AGGR, FSET, FSET_MNT_PT, FSET_DIR_MAP, LOADSUITE);
	&ReadConfigFile;
	&FindSectionDefnRanges;
}

sub ReadConfigFile { 
    local($configFile)=@_;
    local($line)=0;
    
    $configFile || ($configFile = $CONFIG_FILE);
    open(CFH, $configFile);
    while ( <CFH> ) {
	   chop;
       # ignore comments or blank lines
	   next if /^#/ || /^[ \t]*$/ ;
	   $configFileAry[$line]=$_;
	   $cfgline[$line]=$.;
	   $line++;
    }
}

sub cfgLine {
	local($aryLine)=@_;
	$cfgline[$aryLine];
}

# XXXX  - Revisit to see if cleanup can be done now that all GetAttr usage
# specifies rangeStart and rangeEnd
# Assumption: The defn. of an attribute should start with no leading spaces
# 
# attrType values - SINGLE, FLAG, MULTIPLE
# attrOpt values - OPTIONAL, REQUIRED.
# $rangeStart and $rangeEnd are optional
sub GetAttr { 
	local($attrName, $attrType, $attrOpt, *attr, $rangeStart, $rangeEnd)=@_;
	local($line, $val);
	
	$attrDefnFnd=0;

	$rangeStart || ( $rangeStart = 0);
	$rangeEnd || ( $rangeEnd = $#configFileAry) ;

	for ( $line=$rangeStart; $line <= $rangeEnd; $line++ ) {
		$_=$configFileAry[ $line ];
		split(' '); 
		if ( $_[0] eq $attrName ) {		
			#Found first line corresponding to the attribute being looked for
			$attrDefnFnd=1;
			last;
		}
	}

	if ( $attrDefnFnd ) {
		if ( $attrType eq FLAG ) {
			$attr=1;
		} else {
            # $line contains line number in configFileAry that contains start 
            # of attribute value definition. @_ contains the attribute name 
            # and attribute values.
			shift;
			$val=join(" ",@_);
			if ( $attrType eq SINGLE ) {
				$attr=$val;
			} else  {
				push(@attr,$val);

                # Process other lines containing attribute instances defns 
                # till another attribute definition is started
				for($line++; ($line <= $rangeEnd); $line++) {
					$_=$configFileAry[ $line ];
					last if (/^SECTION/ ||
							 /^[ \t]*LS_/); # Another section starts.
					split(' ');
					$val=join(" ",@_);
					push(@attr,$val);
				}
			}
		}
	} else {
		if ( $attrOpt eq REQUIRED ) {
			&Exit("REQUIRED attribute $attrName not specified",
				  __FILE__, __LINE__, $EXIT_CONFIG_FILE_ERROR);
		}
	}
}

# Routine to be used by the load package to fetch value of any attribute
# Updates the global 3D array loadAttr{ loadName, loadNum, attrName }.

sub GetLoadAttr {
	local($loadName, $loadNum, $sectionStart, $sectionEnd,
		  $attrName, $attrType, $attrOpt)=@_;
	local($attrVal);

	if ( $attrType eq "MULTIPLE" ) {
		if (!defined($loadMultAttrValStr)) {
			$loadMultAttrValStr="loadMultAttrVal00000";
		} else {
			$loadMultAttrValStr++;
		}
		
		local(*multAttrVal)=eval("\*$loadMultAttrValStr");

		# This is ok as long as user uses it correctly.
		# *a = $loadAttr{$loadName, $loadNum, $attrName};
		# foreach $attr ( @a ) { 
	    #    <blah> ....
        # }
		&GetAttr($attrName, $attrType, $attrOpt, *multAttrVal,
				 $sectionStart, $sectionEnd);
		$loadAttr{$loadName, $loadNum, $attrName}=*multAttrVal; 
	} else {
		&GetAttr($attrName, $attrType, $attrOpt, *attrVal,
				 $sectionStart, $sectionEnd);
		$loadAttr{$loadName, $loadNum, $attrName}=$attrVal; 
	}
}


sub FindSectionDefnRanges {
	local($line,$sectionStart,$sectionEnd,$sectionType);

	for ( $line = 0; $line < @configFileAry; $line++ ) {
		$_=$configFileAry[$line];
		if ( /^[ \t]*SECTION/ || $line == $#configFileAry ) {
			if ( defined($sectionStart) ) {
				if ( $line == $#configFileAry ) {
					$sectionEnd=$line;
				} else {
					$sectionEnd=$line-1;
				}
				if ( !defined($sectionDefnRanges{$sectionType}) ) {
					$sectionDefnRanges{$sectionType} =
						join(' ', $sectionStart, $sectionEnd);
				} else {
					$sectionDefnRanges{$sectionType} =
						join(' ', $sectionDefnRanges{$sectionType},
							 $sectionStart, $sectionEnd);
				}
				$sectionStart = 0;
			}
			if ( $line != $#configFileAry ) {
				split(' ');
				$sectionType = $_[1];
				if ( ! grep( $sectionType eq $_, @sectionTypes)) {
					&Exit(sprintf("Invalid section type ($sectionType) specified in config file $CONFIG_FILE at line %d", &cfgLine($line)),
						  __FILE__,__LINE__,$EXIT_CONFIG_FILE_ERROR);
				}
				$sectionStart = $line;
			} 
		} 
	}
}



# general routines

# Uses global @nextConfigLine;
sub FindSectionDefn {
	local($sectionType,*firstLine,*lastLine)=@_;
	local($line,$maxline);
	local($fndSection)=0;

	for ($line=$nextConfigLine{$sectionType}, $maxline=@configFileAry;
		 ( ! $fndSection) && ( $line < $maxline ) ;
		 $line++ ) {
		$_=$configFileAry[$line];
		split(' ');
		if ( ( $_[0] eq "SECTION" ) && ( $_[1] eq $sectionType ) ) {
			# Struck gold !
			$fndSection=1;
			$firstLine=$line;
			$lastLine=&FindSectionDefnEnd($line);
			$nextConfigLine{$sectionType} = $lastLine + 1;
		}
	}

	$fndSection;
}

sub FindSectionDefnEnd {
	local($firstLine)=@_;
	local($line, $maxline);

	for ( $line = $firstLine + 1 , $maxline = @configFileAry;
   		  $line < $maxline;
		  $line ++ ) {
		$_=$configFileAry[$line];
		if ( /^[ \t]*SECTION/ ) {
			last;
		}
	}

	$line - 1;
}	

1;

# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:
