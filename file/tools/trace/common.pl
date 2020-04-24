package common;

sub DetermineSystemType {
    local(*sysType) = @_;
    local($retval) = 0;

    local($os) = `uname -s`;
    chop($os);
    if ($os =~ /^SunOS$/) {
	local($ver);
	$ver = `uname -r`;
	chop($ver);
	if ($ver >= 5.0) {
	    $sysType = SPARC_SOLARIS2;
	} else {
	    $sysType = "$os $ver";
	    $retval = 1;
	}
    } elsif ($os =~ /^AIX$/) {
	$sysType = RIOS;
    } else {
	$sysType = $os;
	$retval = 1;
    }
    $retval;
}

1;
