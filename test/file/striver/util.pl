# include in package main itself	
package main;

############################################################################
# System utils

sub DestroyProcesses {
	local(@pids) = @_;
	local($pid);

	kill(SIGKILL, @pids);
	foreach $pid (@pids) {
		waitpid($pid, 0);
	}
}

# special Exit function to allow logging
# $file, $line, $exitval optional
sub Exit {  
    local($msg, $file, $line, $exitval) = @_;    
    local($dieMsg);
	if ($file && $line) {
		$dieMsg = sprintf("DIE in file %s, line %d, %s", $file, $line, $msg);
	} else {
		$dieMsg = sprintf("DIE in $msg");
	}
	if ($exitVal) {$!=$exitVal};
	&Log($dieMsg);
    die("** $dieMsg **\n");
}

sub PrintAssocArray { 
    local(%array) = @_;
    while (($k,$v) = each(%array)) {
		print (" Key $k - Value $v \n");
    }
}

# Same as exec, except it logs the exec.
sub Exec { 
	local(@args) = @_;
	&Log("exec @args");
	$DEBUG && return 0;	
	exec(@args);
	# return 1
	&Exit("exec @args failed");
}

# Sys(cmd)
# return codes
#    0 success
#    -2 killed by signal, no core file 
#    -3 killed by signal, core file
#    -4 command suspended
#    +ve value, exit status of command
#

sub Sys { 
    local(@args) = @_;    
    local($result, $retVal);
    
	&Log("execute \"@args\"");
	$DEBUG && return 0;

	$result = system("@args");            
    # Exit status handling
    if ($result & 0xFF) {
        if (($result & 0xFF) == 0x7F) {
            &Log("\"@args\" was suspended");
            $retVal = -4;
        } else {
            if ($result & 0x80) {
                &Log("\"@args\" produced core file");
                $retVal = -3;                        
            } else {
                $retVal = -2;                                        
            }
            &Log(sprintf("\"@args\" killed by signal %d", 
                         $result & 0x7F ));
        }
    } else {
        $result >>= 8;
        if ( $result ) {
            &Log(sprintf("\"@args\" exit with nonzero value %d",
                         $result));
            $retVal = $result;
        } else {
            &Log("  success of \"@args\" ") if $VERBOSE;
            $retVal = 0;
        }
    }
    $retVal;
}


# special myWarn function to allow logging
# $err argument optional. If present indicates errno.
sub Warn { 
    local($msg, $file, $line, $err) = @_;    
    local($dieMsg);
    if ($err) {
        $warnMsg = sprintf("WARNING in file %s, line %d, %s, errno %d",
               $file, $line, $msg, $err);
    } else {
        $warnMsg = sprintf("WARNING in file %s, line %d, %s",
               $file, $line, $msg);
    }
    warn("[ $dieMsg ]\n");
}

sub Error {                          
    local($msg) = @_;
	$error = 1;
    &Log("ERROR: $msg");
}

# Sleep
sub Sleep {                        
	local($sec) = @_;
	&Log("Sleeping $sec seconds");
    $DEBUG && return;	
	&Log(sprintf("Actually slept %d seconds", sleep($sec)));
}

sub FinishUp {    
    if ($error) {
        &Log("Completed, with errors");
        $exitval = 1;
    } else {
       &Log("Completed");
#       &closeLog;
       $exitval = 0;
    }
}

sub SetLogFile {
	local($logFile, $consoleLog) = @_;
	open(STDOUT,">>$logFile");
	open(STDERR,">&STDOUT");
	select(STDERR); $| = 1;
	select(STDOUT); $| = 1;
	
	$logOnConsole = $consoleLog;
}

# Assumes STDOUT and STDERR have been redirected to the log file.
sub Log {                         
    local($msg) = @_;
	local(@ltime) = localtime(time);
	local($date) = "$ltime[4]/$ltime[3]/$ltime[5]";
	local($time) = sprintf("%02d:%02d:%02d", $ltime[2], $ltime[1], $ltime[0]);
	local($str) = "[ $date, $time | $msg ]\n";
	if ($LOGONCONSOLEONLY) {
		print CONSOLE "$str" 
	} else {
		print "$str" ;
		print CONSOLE "$str" if $logOnConsole;
	}
}

sub LogEnv {
	&Log("Environment Variables are:");
	foreach (sort(keys(%ENV))) {
		&Log("$_ \t $ENV{$_}");
	}
}

###########################################################################;
#
# File / Dir system utils
#

# Create a file.
sub CreateFile {
    local($file, $fileh) = @_;
    local($code);
								# 
	&Log("CreateFile $file");
    $code = open($fileh, ">${file}");
    if (!defined($code)) {
		&Exit("Unable to create file $file ($!)");
    }
}

# Make a directory, if necessary. Aborts if it can't be made.
# Default perm = 0777;
sub MakeDir {
    local($path, $perm) = @_;
    local($curPath) = "";
    local(@comp);

    if (! $perm) {$perm = 0777};

    &Log(sprintf("MakeDir $path %o", $perm));
    $DEBUG && return;

    if (-d $path) {
        chmod($perm, $path) ||
           &Exit(sprintf("couldn't chmod $dir to %o ($!)", $perm));
    } else {
	    if (index($path, '/') == 0) {
	        $curPath = "/";
	        $path = substr($path, 1);
     	}
    	@comp = split('/',$path);
    	shift(@comp) if ($comp[0] =~ /\./);
    	while ($comp = shift(@comp)) {
            #print "Handle $comp \n";
     	    $curPath .= "$comp/";
    	    if (! -d $curPath) {
                #printf "mkdir $curPath \n";
    		    mkdir($curPath, $perm) ||
                &Exit("couldn't make directory $dir ($!)");
    	    }
    	}
    }
}

sub Chmod {                     
	local($obj, $perm) = @_;

    &Log(sprintf("chmod $obj %o", $perm));
    $DEBUG && return;
    chmod($perm, $obj) ||
        &Exit(sprintf("couldn't chmod $obj to %o ($!)", $perm));
}

sub Rename {
    local($cur, $new) = @_;
    &Log("rename $cur $new");
    $DEBUG && return;

	rename($cur, $new) || &Exit("couldn't rename $cur to $new ($!)");

}

sub Chdir {
    local($dir) = @_;

    &Log("chdir $dir");
    $DEBUG && return;
    chdir($dir) || &Exit("couldn't change dir to $dir ($!)");
}

sub CopyFile {
    local($src, $copy) = @_;
    open(SRC, "$src");
    open(COPY, ">$copy");
    while (<SRC>) {
       print COPY $_;
    }
    close(SRC);
    close(COPY);
}

sub Trun {
    local($file, $fileh, $len) = @_;
    local($code);

    &Log("trun $file $len");
    $code = truncate($file, $len);
    &Exit("trun $file $len failed ($!)") if (!$code);
}

sub Write {
    local($file, $fileh, $offset) = @_;
    local($code);

    &Log("write $file $offset");
    $! = 0;
    $code = seek($fileh, $offset, 0);
    &Exit("seek $file to $offset failed ($!)") if (!$code);
    $code = syswrite($fileh, "a", 1);
    &Exit("write $file at $offset failed ($!)") if (!defined($code));
}

############################################################################
# Pathname utils
sub Tail {                           
    local($string) = @_;
    local($index, $length, $tail);
    $index = rindex($string, "/");
    $length = length($string);
    $tail = substr($string,$index+1);
}

# No . or .. or consecutive / in path
sub ParentDir {
	local($path) = @_;
	local($parentDir);
	
	if ( $path eq "/" ) {
		$parentDir = "/";
	} else {
		# Remove trailing "/" if any
		if (substr($path,-1,1) eq "/") {
			$path=substr($path, 0, length($path) - 1);
		}
		$index = rindex($path, "/");		
		$parentDir = substr($path, 0, $index);
	}
	$parentDir;
}

############################################################################
# Number utils
sub IsEven {
	local($number) = @_;

	!($number & 0x1);
}

sub Max {
    local($a, $b) = @_;
    local($max);
    $max = $a;
    if ($max < $b) {$max = $b};
    $max;
}

sub Min {
    local($a, $b) = @_;
    local($min);
    $min = $a;
    if ($min > $b) { $min = $b };
    $min;
}

sub RoundUp {
    local($a)=@_;
    local($aInt)=int($a);
    if ( $aInt != $a ) {
       $aInt++;
    }
    $aInt;
}

############################################################################
# Array utils
# ary is the assoc array to be created.
# key field ranges from 1 onwards to $#@fields + 1.
sub ReadAssocArray {
	local($dataFile, $keyField, *ary) = @_;
	local(@fields);

	open(DH, "<$dataFile");
	while ( <DH> ) {
		# ignore leading spaces

		@fields=split(' ');
		$ary{ $fields[$keyField - 1] } =
		    (splice(@fields, $keyField - 1, 1), join(' ',@fields));
	}
	close(DH);
}

sub WriteAssocArray {
	local($dataFile, $keyField, %ary) = @_;
	local($key,$val);
	local(@fields,$record);

	if ( $DEBUG ) { &Log("Wrote file $dataFile"); return };
	$result=1;
	open(DH, ">$dataFile") ;
	$save=$";    #";
	$"="\t";     #";
	while ( ($key, $val) = each(%ary) ) {
		@fields = split(' ',$val);
		$record = "";
		$record .= "@fields[0..$keyField-2]"."\t" if ($keyField != 1);
		$record .= "$key";
		$record .= "\t" . "@fields[$keyField-1..$#fields]" if ($keyField <=
															   @fields);
		
		print(DH "$record\n");
	}
	$"=$save;   #";
	close(DH);
}

sub WriteArrayToFile {
    local($file,$delimiter,@ary)=@_;
    
    open(FILE, ">$file") ||
       &Exit("WriteArrayToFile - unable to create \"$file\" ($!)");
	foreach ( @ary ) {
		print FILE "${_}${delimiter}\n";
	}
	close (FILE);
}

sub AryToStr {
    local(@ary) = @_;
    local($str);
    foreach ( @ary ) {
	$str .= "$_\t";
    }
    $str;
}

###########################################################################
# Misc utils
# no prefix - bytes
# K prefix - kbytes
# M prefix - Mbytes
sub SizeInKB {
    local($size)=@_;
    local($ksize);
    
    $_=$size;
    if ( /M$/ ) {
	    $ksize = $` * 1024;
    } elsif ( /K$/ ) {
    	$ksize = $`;
    } else { 
        # in bytes 
	    $ksize = int($size/1024);
    }
    $ksize;
}

1;

# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:
