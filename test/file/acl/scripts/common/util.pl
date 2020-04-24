package main;

#
# NOTE: If you change indentation of portions/entire file, ensure
# that all the heredoc ending delimiters STILL start in column 1.
#

#
# Globals 
#

# Globals for log processing
# $CONSOLELOGHANDLE
# $LOGONCONSOLE 
# $LOGONCONSOLEONLY

# Globals for local account/group processing
$LOCAL_PASSWD_FILE = "/etc/passwd";
$LOCAL_GROUP_FILE = "/etc/group";

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
    while (($k, $v) = each(%array)) {
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
    &Exit(sprintf("exec @args failed, errno %d", $!));
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

# Sys2(cmd, cmdOut);
# return codes
#    0 success
#    -2 killed by signal, no core file 
#    -3 killed by signal, core file
#    -4 command suspended
#    +ve value, exit status of command
#
# @cmdOut is a reference array parameter
sub Sys2 { 
    local($args, *cmdOut) = @_;    
    local($result, $retVal);
    local($file) = "/tmp/sys2.$$.out";
    
    &Log("execute \"$args\"");
    $DEBUG && return 0;

    open(SAVESTDOUT, ">&STDOUT");
    open(SAVESTDERR, ">&STDERR");
    open(OUTFILE, ">$file");
    open(STDOUT, ">&OUTFILE");
    open(STDERR, ">&OUTFILE");
    
    $result = system("$args");            

    close(OUTFILE);
    open(STDOUT, ">&SAVESTDOUT");
    open(STDERR, ">&SAVESTDERR");
    
    open(OUTFILE, "$file");
    @output = <OUTFILE>;
    close(OUTFILE);
    @cmdOut = @output;

    unlink($file) ||
	&ErrorExit("Unable to delete temporary internal $file", $!);

    &Log(sprintf("%s", join("", @output)));

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

# Accepts subCmds as a single string with newlines seperating 
# the commands
sub SysHeredoc { 
    local($args, $subCmdsStr) = @_;    
    local($result, $retVal);
    
    &Log("execute \"$args <<==EOC== \n$subCmdsStr\n<<==EOC==");
    $DEBUG && return 0;
    
    $result = system("$args << ==EOC==
$subCmdsStr
==EOC==
");            
    # Exit status handling
    if ($result & 0xFF) {
        if (($result & 0xFF) == 0x7F) {
            &Log("\"$args\" was suspended");
            $retVal = -4;
        } else {
            if ($result & 0x80) {
                &Log("\"@args\" produced core file");
                $retVal = -3;                        
            } else {
                $retVal = -2;                                        
            }
            &Log(sprintf("\"$args\" killed by signal %d", 
                         $result & 0x7F ));
        }
    } else {
        $result >>= 8;
        if ( $result ) {
            &Log(sprintf("\"$args\" exit with nonzero value %d",
                         $result));
            $retVal = $result;
        } else {
            &Log("  success of \"$args\" ") if $VERBOSE;
            $retVal = 0;
        }
    }
    $retVal;
}

# Accepts subCmds as a single string with newlines seperating 
# the commands
# @cmdOut reference array parameter version
sub SysHeredoc2 { 
    local($args, $subCmdsStr, *cmdOut) = @_;    
    local($result, $retVal);
    local($file) = "/tmp/sys2.$$.out";
    
    &Log("execute \"$args <<==EOC== \n$subCmdsStr\n==EOC==");
    $DEBUG && return 0;
    
    open(SAVESTDOUT, ">&STDOUT");
    open(SAVESTDERR, ">&STDERR");
    open(OUTFILE, ">$file");
    open(STDOUT, ">&OUTFILE");
    open(STDERR, ">&OUTFILE");

    $result = system("$args << ==EOC==
$subCmdsStr
==EOC==
");            

    close(OUTFILE);
    open(STDOUT, ">&SAVESTDOUT");
    open(STDERR, ">&SAVESTDERR");
    
    open(OUTFILE, "$file");
    @output = <OUTFILE>;
    close(OUTFILE);
    @cmdOut = @output;

    unlink($file) ||
	&ErrorExit("Unable to delete temporary internal $file", $!);

    &Log("Output of interactive command is:");
    &Log(sprintf("%s", join("", @output)));

    # Exit status handling
    if ($result & 0xFF) {
        if (($result & 0xFF) == 0x7F) {
            &Log("\"$args\" was suspended");
            $retVal = -4;
        } else {
            if ($result & 0x80) {
                &Log("\"@args\" produced core file");
                $retVal = -3;                        
            } else {
                $retVal = -2;                                        
            }
            &Log(sprintf("\"$args\" killed by signal %d", 
                         $result & 0x7F ));
        }
    } else {
        $result >>= 8;
        if ($result) {
            &Log(sprintf("\"$args\" exit with nonzero value %d",
                         $result));
            $retVal = $result;
        } else {
            &Log("  success of \"$args\" ") if $VERBOSE;
            $retVal = 0;
        }
    }
    $retVal;
}

# special myWarn function to allow logging
# $err argument optional. If present indicates errno.
# XXX Make $file, $line optional
sub Warn { 
    local($msg, $file, $line, $err) = @_;    
    local($warnMsg);
    if ($err) {
        $warnMsg = sprintf("WARNING in file %s, line %d, %s, errno %d",
			   $file, $line, $msg, $err);
    } else {
        $warnMsg = sprintf("WARNING in file %s, line %d, %s",
			   $file, $line, $msg);
    }
    warn("[ $warnMsg ]\n");
}

sub Error {                          
    local($msg) = @_;
    $error = 1;
    &Log("ERROR: $msg");
}

sub ErrorExit {
    local($msg, $exitVal) = @_;
    
    local($dieMsg) = sprintf("DIE in $msg");
    
    if (defined($exitVal)) {
	$! = $exitVal;
	$dieMsg .= sprintf(", exit code %d", $exitVal);
    } else {
	$! = 255;		# Force a non-zero exit status
    };
    &Log($dieMsg);
    die("** $dieMsg **\n");
    # Never Returns
}

# Sleep
sub Sleep {                        
    local($sec) = @_;
    &Log("Sleeping $sec seconds");
    $DEBUG && return;	
    &Log(sprintf("Actually slept %d seconds", sleep($sec)));
}

#XXX Is this useful - No
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
    local($logFile, $consoleHandle) = @_;

    open(STDOUT,">>$logFile") ||
	die(sprintf("$0 - Unable to set log file $logFile, errno %d", $!));
    open(STDERR,">&STDOUT");
    select(STDERR); $| = 1;
    select(STDOUT); $| = 1;
    
    if ($consoleHandle) {
	$CONSOLELOGHANDLE = $consoleHandle;
	$LOGONCONSOLE  = 1;
    } else {
	$LOGONCONSOLE = 0;
	undef($CONSOLELOGHANDLE);
    }
    
    # Initialize internal date for loggin
    $prevDate = "";
}

# Assumes STDOUT and STDERR have been redirected to the log file.
# Uses globals 
# $CONSOLELOGHANDLE
# $LOGONCONSOLE 
# $LOGONCONSOLEONLY

sub Log {                         
    local($msg) = @_;
    local(@ltime) = localtime(time);
    local($curDate) = "$ltime[4]/$ltime[3]/$ltime[5]";
    local($time) = sprintf("%02d:%02d:%02d", $ltime[2], $ltime[1], $ltime[0]);
    local($str) = "";
    
    if (!defined($prevDate) || ($curDate ne $prevDate)) {
	$str = "[ Date is $curDate ]\n";
	$prevDate = $curDate;
    }
    $str .= "[ $time | $msg ]\n";
    
    if ($LOGONCONSOLEONLY) {
	print $CONSOLELOGHANDLE "$str";
    } else {
	print "$str" ;
	print $CONSOLELOGHANDLE "$str" if $LOGONCONSOLE;
    }
}

sub LogEnv {
    &Log("Environment Variables are:");
    foreach (sort(keys(%ENV))) {
	&Log("$_ \t $ENV{$_}");
    }
}

############################################################################;
# File / Dir system utils
# Make a directory, if necessary. Aborts if it can't be made.
# Default perm = 0777;
sub MakeDir {
    local($path, $perm) = @_;
    local($curPath) = "";
    local(@comp);
    
    if (!$perm) {$perm = 0777};
    
    &Log(sprintf("MakeDir $path %#o", $perm));
    $DEBUG && return;
    
    if (-d $path) {
        chmod($perm, $path) ||
	    &Exit(sprintf("couldn't chmod $dir to %#o ($!)", $perm));
    } else {
	if (index($path, '/') == 0) {
	    $curPath = "/";
	    $path = substr($path, 1);
     	}
    	@comp = split('/',$path);
    	shift(@comp) if ($comp[0] eq ".");
    	while ($comp = shift(@comp)) {
            #print "Handle $comp \n";
     	    $curPath .= "$comp/";
    	    if (! -d $curPath) {
                #printf "mkdir $curPath \n";
		mkdir($curPath, $perm) ||
		    &Exit("couldn't make directory $dir ($!)");
		chmod($perm, $curPath) ||
		    &Exit("couldn't set directory $dir mode bits to $perm ($!)");
    	    }
    	}
    }
}

# Make a directory, if it does not exist. 
# Return EEXIST if it already exists
sub MakeDir2 {
    local($dir, $perm) = @_;
    local($code) = 0;

    &Log(sprintf("MakeDir2 $dir %#o", $perm));
    $DEBUG && return;
    
    mkdir($dir, $perm) || ($code = $!);
    $code;
}

sub Chmod {                     
    local($obj, $perm) = @_;
    
    &Log(sprintf("chmod $obj %#o", $perm));
    $DEBUG && return;
    chmod($perm, $obj) ||
        &Exit(sprintf("couldn't chmod $obj to %#o ($!)", $perm));
}

# XXX is this really used
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

sub getMode {
    local($fileObj) = @_;
    local(@stat, $mode);
    
    @stat = stat($fileObj);
    &Exit ("couldn't stat $fileObj ($!)") if (!defined(@stat));
    $mode = $stat[2];
    $mode &= 0777;
    &Log(sprintf("getMode: object $fileObj, unix mode %#o", $mode));
    $mode;
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

############################################################################
# Pathname utils
# XXX is this really used. - No.
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
#XXX Are these really used.

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
# Local account/group Utils

sub createLocalGroup {
    local($group, $groupId, @groupMembers) = @_;
    local($localEntry);
    local($code) = 0;
    local($logMsg);

    $logMsg = "Creating local group $group, with id $groupId";
    if (@groupMembers) {
	$logMsg .= sprintf(", with members %s", join(' ', @groupMembers));
    }
    &Log($logMsg);
    open(GROUPFILE, "$LOCAL_GROUP_FILE") || 
	&ErrorExit("Create local group, unable to read $LOCAL_GROUP_FILE", $!);
    while (<GROUPFILE>) {
	if (/^$group:/) {
	    &ErrorExit("Create local group $group failed as it already exists");
	    # Never returns
	}
    }
    close(GROUPFILE);
    
    open(GROUPFILE, ">>$LOCAL_GROUP_FILE") ||
	&ErrorExit("Create local group, unable to append to $LOCAL_GROUP_FILE", $!);
    $localEntry = join(":",($group, "", $groupId, join(",", @groupMembers)));
    print GROUPFILE "$localEntry \n";
    close(GROUPFILE);
    $code;
}

# Takes gid for primary group.
# XXX Does not take a list of secondary groups for the user yet.
sub createLocalUser {
    local($user, $userId, $pGroup, $pGid) = @_;
    local($localEntry);
    local($code) = 0;

    &Log("Creating local user $user, with id $userId, primary group $pGroup");

    open(USERFILE, "$LOCAL_PASSWD_FILE") ||
	&ErrorExit("Create local user, unable to read $LOCAL_PASSWD_FILE", $!);
    while (<USERFILE>) {
	if (/^$user:/) {
	    &ErrorExit("Create local user $user failed as it already exists");
	    # Never returns
	}
    }
    close(USERFILE);
    
    open(USERFILE, ">>$LOCAL_PASSWD_FILE") || 
	&ErrorExit("Create local user, unable to append to $LOCAL_PASSWD_FILE", $!);
    $localEntry = join(':',
		       ($user, "", $userId, $pGid, "ACL Test User", "/tmp",
			"/bin/sh"));
    print USERFILE "$localEntry \n";
    close(USERFILE);
    $code;
}

sub removeLocalGroups {
    local(@groups) = @_;
    local($tmpfile);
    local($group, %groups, $entry);

    $tmpfile = "/tmp/group$$";
    &Log(sprintf("Removing local groups %s", join(' ', @groups)));
    open(GROUPFILE, $LOCAL_GROUP_FILE) ||
	&ErrorExit("Unable to remove local groups, can't read $LOCAL_GROUP_FILE", $!);
    open(TMPFILE, ">$tmpfile") || 
	&ErrorExit("Unable to remove local groups, couldn't create temporary file $tmpfile", $!);	

    &Log("Temporary new group file is $tmpfile");
    while ($entry = <GROUPFILE>) {
	$skip = 0;
	foreach $group (@groups) {
	    if ($entry =~ /^$group:/) {
		$skip = 1;
		$groups{$group} = 1;
		last;
	    }
	}
	if (!$skip) {
	    print TMPFILE $entry;
	}
    }

    foreach $group (@groups) {
	if ($groups{$group} != 1) {
	    &Log("Local group $group does not exist");
	}
    }
    close(GROUPFILE);
    close(TMPFILE);

    &Log("Copy $tmpfile to $LOCAL_GROUP_FILE");
    &CopyFile($tmpfile, $LOCAL_GROUP_FILE);
    unlink($tmpfile) ||
	&Log("Unable to delete temporary group internal file $tmpfile ($!)");;
}

sub removeLocalUsers {
    local(@users) = @_;
    local($tmpfile);
    local($user, %users, $entry);

    $tmpfile = "/tmp/passwd$$";
    &Log(sprintf("Removing local users %s", join(' ', @users)));
    open(USERFILE, $LOCAL_PASSWD_FILE) ||
	&ErrorExit("Unable to remove local users, can't read $LOCAL_PASSWD_FILE", $!);
    open(TMPFILE, ">$tmpfile") || 
	&ErrorExit("Unable to remove local users, couldn't create temporary file $tmpfile", $!);	

    &Log("Temporary new passwd file is $tmpfile");
    while ($entry = <USERFILE>) {
	$skip = 0;
	foreach $user (@users) {
	    if ($entry =~ /^$user:/) {
		$skip = 1;
		$users{$user} = 1;
		last;
	    }
	}
	if (!$skip) {
	    print TMPFILE $entry;
	}
    }

    foreach $user (@users) {
	if ($users{$user} != 1) {
	    &Log("Local user $user does not exist");
	}
    }
    close(USERFILE);
    close(TMPFILE);

    &Log("Copy $tmpfile to $LOCAL_PASSWD_FILE");
    &CopyFile($tmpfile, $LOCAL_PASSWD_FILE);
    unlink($tmpfile) ||
	&Log("Unable to delete temporary internal passwd file $tmpfile ($!)");;
}

###########################################################################
# Misc utils
# no prefix - bytes
# K prefix - kbytes
# M prefix - Mbytes

# XXX is this used? - No.
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

