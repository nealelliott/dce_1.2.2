package main;

# Module containing communication and related primitives between
# parent and child.

# IMPORTANT
# Caller must also have included typeLen.pl module
# Caller must also have included typeLen.pl module

# Setup signal handler to detect broken pipe
# Note this is not the only way to detect brokend pipes. SIGPIPE can
# be set up to be ignored and the following pipe code modified to
# check for EPIPE errno.
$sigpipe = 0;
$SIG{'PIPE'} = 'sigpipeHandle';

# Error return values
$PIPE_SUCCESS = 0;
$PIPE_ERROR_NO_MATE = 1;
$PIPE_ERROR_MISC = 2;

sub sigpipeHandle {
    local($sig) = @_;		
    &Log("ERROR: process $$ Caught SIGPIPE");
    $sigpipe = 1;
}

sub pipeWriteInt {
    local($pipeh, $n) = @_;
    local($code, $sendBuf);

    $sendBuf = pack("i", $n);
    
    $sigpipe = 0;
    $code = syswrite($pipeh, $sendBuf, $LEN_INT);
    if ($sigpipe == 0) {
	if (!defined($code)) {
	    &Log("ERROR ($!) writing integer to pipe $pipeh");
	    $code = $PIPE_ERROR_MISC;
	} elsif ($code == $LEN_INT) {
	    $code = $PIPE_SUCCESS;
	} else {
	    &Log("ERROR (wrote $code of $LEN_INT bytes) writing integer to pipe $pipeh, ");
	    $code = $PIPE_ERROR_MISC;
	}
    } else {
	# Caught SIGPIPE
	&Log("ERROR (Broken Pipe) writing integer to pipe $pipeh");
	$code = $PIPE_ERROR_NO_MATE;
    }
    $code;
}

sub pipeReadInt {
    local($pipeh, *n) = @_;
    local($code, $recvBuf, @a);

    $recvBuf = "";
    $code = sysread($pipeh, $recvBuf, $LEN_INT);
    if ($code == $LEN_INT) {
	@a = unpack("i", $recvBuf);
	$n = $a[0];
	$code = $PIPE_SUCCESS;
    } else {
	if (!defined($code)) {	
	    &Log("ERROR ($!) reading integer from pipe $pipeh");
	    $code = $PIPE_ERROR_MISC;
	} elsif ($code == 0) {
	    #No pipe writers/EOF
	    &Log("ERROR (no open write end) reading integer from pipe $pipeh");
	    $code = $PIPE_ERROR_NO_MATE;
	} else {
	    &Log("ERROR (read $code of $LEN_INT bytes) reading integer form pipe $pipeh, ");
	    $code = $PIPE_ERROR_MISC;
	}
    }
    $code;
}

sub pipeWriteStr {
    local($pipeh, $str) = @_;
    local($code, $sendBuf, $len);

    $sendBuf = pack("a*", $str);
    $len = length($sendBuf);

    $code = &pipeWriteInt($pipeh, $len);
    if ($code == $PIPE_SUCCESS) {
	$sigpipe = 0;
	$code = syswrite($pipeh, $sendBuf, $len);
	if ($sigpipe == 0) {
	    if (!defined($code)) {
		&Log("ERROR ($!) writing string to pipe $pipeh");
		$code = $PIPE_ERROR_MISC;
	    } elsif ($code == $len) {
		$code = $PIPE_SUCCESS;
	    } else {
		&Log("ERROR (wrote $code of $len bytes) writing string to pipe $pipeh");
		$code = $PIPE_ERROR_MISC;
 	    }
	} else {
	    # Caught SIGPIPE
	    &Log("ERROR (Broken Pipe) writing string to pipe $pipeh");
	    $code = $PIPE_ERROR_NO_MATE;
	}
    } 
    $code;
}

sub pipeReadStr {
    local($pipeh, *str) = @_;
    local($code, $recvBuf, @a, $len);

    $recvBuf = "";
    $code = &pipeReadInt($pipeh, *len);
    if ($code == $PIPE_SUCCESS) {
	$code = sysread($pipeh, $recvBuf, $len);
	if ($code == $len) {
	    @a = unpack("a*", $recvBuf);
	    $str = $a[0];
	    $code = $PIPE_SUCCESS;
	} else {
	    if (!defined($code)) {	
		&Log("ERROR ($!) reading string from pipe $pipeh");
		$code = $PIPE_ERROR_MISC;
	    } elsif ($code == 0) {
		&Log("ERROR (no open write end) reading string from pipe $pipeh");
		$code = $PIPE_ERROR_NO_MATE;
	    } else {
		&Log("ERROR reading string from pipe $pipeh, string length $len, read $code bytes");
		$code = $PIPE_ERROR_MISC;
	    }
	}
    }
    $code;
}

1;
