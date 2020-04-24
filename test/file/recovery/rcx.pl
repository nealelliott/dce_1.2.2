#! /afs/tr/fs/dev/perl
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: rcx.pl,v $
# Revision 1.1.8.1  1996/10/17  18:34:06  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:56:56  damon]
#
# Revision 1.1.3.1  1994/07/13  22:31:52  devsrc
# 	moving files from src/file/episode to test/file
# 	[1994/03/22  20:40:24  delgado]
# 
# 	Moved '#!' line to top of file.
# 	[1992/05/04  19:14:08  jdp]
# 
# 	delta bob-add-rcx 1.1
# 	[1992/05/04  18:45:18  jdp]
# 
# Revision 1.1.15.1  1994/02/04  20:14:38  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:11:03  devsrc]
# 
# Revision 1.1.13.2  1993/12/07  20:32:17  jaffe
# 	Fixed Bad Comment leaders
# 	[1993/12/07  20:31:10  jaffe]
# 
# Revision 1.1.13.1  1993/12/07  17:20:16  jaffe
# 	New File from Transarc 1.0.3a
# 
# Revision 1.1.1.2  1993/12/02  20:34:24  jaffe
# 	New File from Transarc 1.0.3a
# 
# Revision 1.1.4.6  1993/01/19  15:31:42  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:53:14  cjd]
# 
# Revision 1.1.4.5  1992/12/09  20:04:40  jaffe
# 	Transarc delta: rajesh-ot6080-rcx-compiles-chgrp-incorrectly 1.1
# 	  Selected comments:
# 	    Correctly compile chgrp.
# 	    See above.
# 	[1992/12/03  22:15:00  jaffe]
# 
# Revision 1.1.4.4  1992/11/18  19:19:35  jaffe
# 	Transarc delta: rajesh-ot5841-clean-up-rcx 1.11
# 	  Selected comments:
# 	    The RCX script langauage interface in terms of naming and
# 	    functionality available is not consistent. Need to introduce
# 	    consistency here.
# 	    Also, among the existing RCX scripts, some are essentially duplicates
# 	    of others. Hence remove duplicate scripts and reorganize existing
# 	    scripts.w
# 	    Also change the scripts to use the new interface if necessary.
# 	    In this version of delta remove duplicate scripts.
# 	    First step in reorganising scripts according to complexity of scripts.
# 	    Final step (for now) in reorganising scripts according to complexity
# 	    of scripts.
# 	    Introduce consistency into the existing script language commands
# 	    in terms of naming and semantics. A checkpoint.
# 	    Introduce chmodfiles, rmfiles, rmdirs.
# 	    Modify mkdir to handle multiple directories too.
# 	    Completed and tested the implementation begun in last revision 1.4
# 	    for chmodfiles, rmfiles, rmdirs, mvfiles and mkdir.
# 	    Also developed some input rcx scripts for basic recovery tests, that
# 	    were actually developed for testing the new script language interface.
# 	    See comment for this revision of the delta.
# 	    Add support for chown, chgrp, symlink, link, truncate, writes. Develop
# 	    additional input simple test scripts. Rename the earlier created "basic"
# 	    test scripts into "simple" scripts.
# 	    Support for new syscalls. Modify names of some "...files" commands that
# 	    apply to directories also, to "...paths". Also change in order of arguments
# 	    to various commands.
# 	    Lots and lots of fixes.
# 	    -Correct setting of fragsize and blocksize.
# 	    -Log updates to system checksum on recovery.
# 	    -Correct subroutine hash to calculate signatures for symlinks too.
# 	    -Include only major device number of rdev in signatures for special
# 	    device files.
# 	    -Include file size in signatures for files but not for directories.
# 	    -Create infrastructure for maintaining a sibling relationship for files
# 	    that are hardlinks. The routines for this are link_create(), link_enter(),
# 	    link_delete(), link_fetch(), link_display().
# 	    -Update rcx_create(), rcx_rm() and rcx_link() to use the above mentioned
# 	    infrastructure to maintain hard link sibling relationships.
# 	    -Pass correct owner id to emit() in rcx_chown().
# 	    -Correct rcx_symlink to call hash() and emit() without the $testdir prefix
# 	    for pathnames.
# 	    -Correct pathname used in the stat() call in rcx_truncate().
# 	    -Put double quotes around the last argument in calls to aux_write() and
# 	    aux_truncate().
# 	    -and some other small fixes
# 	    Changes made while testing on OSF1. Include some generic changes too.
# 	    Rcx is no longer an independent standalone program in run/compile
# 	    mode. It need s some setup previously done in this program but which
# 	    is now done by runrcx. The setup is that of the test directories with
# 	    magic files.
# 	    The signature format for files and dir were swapped mistakenly. Corrected
# 	    this so that the size field is present only in non-dir signatures.
# 	    Also delete perl command causes segmentation fault and dumps core. So
# 	    a workaround is to use undef instead.
# 	    Some additional scripts.
# 	    Cannot mount filesets from the copy aggregate while the source aggregate is
# 	    still frozen as it can lead to a deadlock over the global buffer lock.
# 	    This version is tested and ready for export.
# 	    No change really.
# 	    Includes work that should have been done under delta
# 	    rajesh-ot5837-extend-syscalls-exercised-by-rcx
# 	[1992/11/17  20:33:16  jaffe]
# 
# Revision 1.1.4.3  1992/10/28  21:43:11  jaffe
# 	Fixed RCS id
# 	[1992/10/28  21:13:20  jaffe]
# 
# Revision 1.1.4.2  1992/10/27  20:52:41  jaffe
# 	Transarc delta: rajesh-ot5391-errors-in-runrcx 1.6
# 	  Selected comments:
# 	    Fix bugs in runrcx script.
# 	    Further updates
# 	    The signature included device major number which is incorrect as
# 	    a aggregate can be copied onto an aggregate on another disk where
# 	    recovery can be run. Hence since device number can change, should not
# 	    include it in filesystem object's signature.
# 	    Also, for efficiency (not really necessary) break out in rcx -r mode
# 	    break out of while loop when system has been found to be in consistent
# 	    state.
# 	    A checkpoint.
# 	    Running version
# 	    rcx_chmod now emits the mode in octal.
# 	    Make a portion of code more readable.
# 	    Minor cleanup to all_tests and stress_tests
# 	[1992/10/27  13:01:06  jaffe]
# 
# $EndLog$
#
# rcx -- Recovery ExcerCisor.
# $Header: /u0/rcs_trees/dce/rcs/test/file/recovery/rcx.pl,v 1.1.8.1 1996/10/17 18:34:06 damon Exp $
# Copyright (C) 1992 Transarc Corporation - All rights reserved

#
# Rcx -- Episode recovery excercisor/excorcisor.
#
$USAGE = "Usage: rcx [-x] [-c|-r] script directory blocksize fragsize";
#
# With no options: run script.rc indefinitely.
#
# Options:
#    -r Check recovery based on hash-coded script (use script.rech)
#    -c Compile script.rs to script.rc and script.rh
#    -x Echo lines as executed.

###########################################################################################################
#
#										I M P O R T A N T
#
# rcx is no longer an independent standalone tool in run/compile mode. It needs some setup previously done 
# in this program but which is now done by runrcx. The setup is that of the test directories with magic files
# - RA 11/06/92 
#
###########################################################################################################

# Keep LSTATERROR in sync with one in runrcx.pl
$LSTATERROR = 2;

# Unbuffer stdout: so that echo output can be interleaved with
# supervising script (e.g. runrcx).
select(STDOUT); $| = 1;

# Some global parameters
$MAGIC_FILE = ".recovery_test_dir";
while ($_ = shift) {
    $compile = 1, next if /^-c/;
    $recover = 1, next if /^-r/;
    $echo = 1, next if /^-x/;
    unshift(ARGV, $_), last;
}
($script, $testdir, $blocksize, $fragsize) = (shift, shift, shift, shift);
die("$USAGE\n") if @ARGV || !$script || ($compile && $recover) || !$testdir;
while ($script =~ s/\.r[sch]$//) {};
$exec = "$script.rc"; # .rc for whole system state
$sigs = "$script.rh"; # .rh for each file/dir state
$script = "$script." . ($compile ? "rs" : "rc");
$run = !$recover && !$compile;

if ($run || $compile) {
    open(SCRIPT, $script) || &x("unable to open $script ($!)");
}

if ($compile) {
    open(SIG, ">$sigs") || &x("unable to create $sigs ($!)");
    open(EXEC, ">$exec") || &x("unable to create $exec ($!)");
}

if ($recover) {
    open(EXEC, $exec) || &x("unable to open $exec ($!)");
    &loadhash;
}

#
# With use of createTestdirs in runrcx, rcx should always find the test dir with the magic file
# in each fileset in all modes - compile, run and recover. If it does not, its an error.
#
&x("Directory $testdir not found") if ! -d $testdir;
&checkdir($testdir);


if ($recover) {
    sub quit {
        local($exitcode) = @_;
        print("Directory $testdir appears to ");
        print("be in an inconsistent state.\n") if $exitcode != 0;
        print("have been recovered correctly.\n") if $exitcode == 0;
        exit($exitcode);
    }
    &scandir($testdir, *checksum);
    &quit(1) if $badsignatures;

    local($ok, $hexsum, $hashcode, $operation);
    $hexsum = sprintf("%08x", $checksum);
    while (<EXEC>) {
        ($hashcode, @operation) = split;
        if ($hashcode eq $hexsum) {
            print("Checksum $hexsum matches line $. of $exec: @operation\n");
            $ok = 1;
            # Found system in consistent state. Bail out.  - 9/17/92 RA
            last;
        }
    }
    if (!$ok) {
        printf("Checksum $hexsum not found in $exec.\n");
        &quit(1);
    }
    &quit(0);
}

# Read in script.
while (<SCRIPT>) {
    split;
    next if ! @_ || /^\s*#/;
    shift(@_) if !$compile;
    push(@commands, "rcx_@_");
}

# Run the script. 
local($i, $command);
do {
    for ($i = 0; $i <= $#commands; $i++) {
        $commandline = $i + 1;
        $_ = $commands[$i];
        ($command, @_) = split;
        &$command;
    }
} while $run;
    
if ($compile) {
# Undo any files generated by the script, so the checksum will be
# the same at the end as at the beginning.
    print("Removing files...\n") if $echo;
    &scandir($testdir, *finish);
    &dumphash;
}

# Die
sub x {
    die("rcx: @_\n");
}

sub checkdir {
    local($dir) = @_;
    &x("File $MAGIC_FILE is missing from directory $dir")
        if ! -f "$dir/$MAGIC_FILE";
}       

# Scan directory and do the supplied operation on each element,
# passing the relative pathname of the element to the function.
sub scandir {
    local($dir, *func) = @_;
    local(@names, $path, $entry, @paths, @entries, @dirs);
    $path = $dir;
    do {
        if (! -l $path && -d _) {
            push(@dirs, $path);
            opendir(DIR, $path) || &x("can't read directory $path ($!)");
            while (defined($entry = readdir(DIR))) {
                next if $entry eq '.' || $entry eq '..';
                push(@entries, $entry);
                push(@paths, $path? "$path/$entry" : $entry);
            }
        } else {
            &func($path, $entry);
        }
        $path = pop(@paths);
        $entry = pop(@entries);
    } while $path;
    for $dir (reverse @dirs) {
        &func($dir);
    }
}

# Compute checksum for the supplied file or directory and xor it
# with the global checksum. This is called by dirscan in $recover mode.
sub checksum {
    local($path) = @_;
	local($objchecksum,$oldchecksum);
    $path =~ s/$testdir\/?//o;
    return if $path eq $MAGIC_FILE;
    return if $entry eq '.';   # Ignore all . entries.
    return if $entry eq '..'; # Ignore ALL .. entries
	$oldchecksum=$checksum;
	$objchecksum = &signature($path);
    $checksum ^= $objchecksum;
	printf(	"**System checksum updated [ from %08x to %08x ] by using \n    $path [ path checksum %08x ] \n", $oldchecksum,$checksum,$objchecksum);
}

# Remove file or directory. This routine is called to cleanup a 
# target tree. Does not remove the root of the tree itself.
sub remove {
    local($path, $entry) = @_;
    return if unlink($path);
    return if $entry eq '.';
    return if $entry eq '..';
    return if ! $path;
    &x("unable to remove $path") if -e $path && ! -d _;
    rmdir($path) || &x("unable to remove $path ($!)");
}

# Finish off script by removing everything we've created and
# adding an entry for each of these operations. Only called in
# compile mode.
sub finish {
    local($path, $entry) = @_;
    return if $entry eq $MAGIC_FILE;
    return if $entry eq '.';
    return if $entry eq '..';
    local($relpath) = $path;
    $relpath =~ s/$testdir\/?//o;
    return if ! $relpath;
#   for $component (split(/\//, $path)) {
#       if (!$prefix) {
#           $prefix = $component;
#       } else {
#           $prefix = join('/', ($prefix, $component));
#       }
#       return if ($preserve{$prefix});
#   }
    if ((! -l $path) && -d $path) {
        &rcx_rmdir($relpath);
    } else {
        &rcx_rm($relpath);
    }
}

# Compute signature functions for supplied path names
# Compute global checksum for tree.
sub hash {
    local(@paths) = @_;
	local($tmp);	
    for $path(@paths) {
        if (defined($signature{$path})) {
            $checksum ^= $signature{$path};
        }
        if (! -e "$testdir/$path" && ! -l "$testdir/$path" ) {
            undef($signature{$path});
        } else {
            $checksum ^= ($signature{$path} = &signature($path));
        }
    }
}

# Get a single signature for a pathname.
# This involves encrypting most of the status information and the relative
# pathname.
sub signature {
    local($path) = @_;
    local($sigstring);
    local($dev, $ino, $mode, $nlink, $uid, $gid, $rdev, $size);
    local($atime, $mtime, $ctime, $blksize, $blocks);
    local(@statbuf);
    local($entry);
    local($error) = 0;

    $! = 0;
    if ($path) {
       @statbuf = lstat("${testdir}/${path}");
    } else {
       @statbuf = lstat("${testdir}");   
    }
    if (@statbuf == 0) {
      printf("rcx: subroutine signature lstat on \"%s\" FAILED with \($!\) \n",
              "${testdir}/${path}");
      &x2("lstat failed", $LSTATERROR);
    } else {
        ($dev, $ino, $mode, $nlink, $uid, $gid, $rdev, $size, $atime, $mtime, $ctime, $blksize, $blocks) = @statbuf;
        if ( !defined($dev) || !defined($rdev) || !defined($mode) || 
             !defined($ino) || !defined($nlink) || !defined($uid) ||
             !defined($gid) || !defined($size) || !defined($atime) ||
             !defined($mtime) || !defined($ctime) || !defined($blksize) ||
             !defined($blocks) ) {
           printf("rcx: subroutine signature lstat on \"%s\" FAILED with null fields)",
                 "${testdir}/${path}");
           printf("lstat on path %s data \n", "$testdir/$path");
           printf(" dev = %d.%d, mode=%o, links=$nlink, uid=$uid, gid=$gid, 
                    rdev=%d.%d, size=$size, blksize=$blksize, blocks=$blocks,
                    atime=$atime, mtime=$mtime, ctime=$ctime\n",
                    $dev >> 8, $dev & 0xff, $mode, $rdev >> 8, $rdev & 0xff);
           &x2("lstat failed", $LSTATERROR);
        }
    }

    $rdev = 0 if !-b _ && !-c _; #rdev only valid for special files.

    # Removed minor device, since it changes per volume.
    # Removed device number as aggregrate device number (major and
    # minor) could be different for the new aggregate than for the
    # original aggregate

    $dev=0  ;
    local(@args) = ($dev >> 16, $mode, $rdev >> 16);

	# 
	# Signatures for directories do not include the size field,
	# as directory compaction never occurs.
	#
	# At a future date, however the size field can be used to check
	# the directory state itself, but should not be used in the 
	# system checksum. i.e. Treat the size field having a fixed dummy value
    # when finding the random number that will be xored with the system
 	# state checksum. RA
	#

	if ( (! -l "$testdir/$path") && ( -d "$testdir/$path") ) {
		$sigstring =
			sprintf("$path,%d,%o,$nlink,$uid,$gid,%d,$blksize", @args);
	} else {
		$sigstring =
			sprintf("$path,%d,%o,$nlink,$uid,$gid,%d,$size,$blksize", @args);
	}
    local($result, $ok) = &ranhash($sigstring);
    if (!$ok || $echo) {
        local($name) = $testdir;
        $name .= "/$path" if $path;
        printf("$name is in an unrecognized state:") if !$ok;
        printf("$name is OK:") if $ok;
        printf("
    sigstring=$sigstring
    dev=%d.%d, mode=%o, links=$nlink, uid=$uid, gid=$gid, rdev=%d.%d,
    size=$size, blksize=$blksize\n", 
            $dev >> 8, $dev & 0xff, $mode, $rdev >> 8, $rdev & 0xff);
        $badsignatures++ if !$ok;
    }
    printf("Returning signature %08x for $sigstring\n", $result) if $echo;
    $result;    
}

# Random hash function. Uses hashcode dictionary for old strings
# (which may be loaded from the script.rc file), otherwise
# random number for next hash code.
# **Inexplicably, on the rs6000, we only seem to get 16 bits of precision,
# **so we call rand twice for 32 full bits.
sub ranhash {
    local($string) = @_;
    local($hash, $ok);
    if (defined($hashcodes{$string})) {
        $hash = $hashcodes{$string};
        $ok = 1;
    } else {
        if ($compile) {
            $hash = (((rand() * 0xffff) | 0) << 16)
                | ((rand() * 0xffff) & 0xffff);
            $hashcodes{$string} = $hash;
            $ok = 1;
        }
        else {
            # Error here if no hashcode found generate error.
            printf("sigstring %s not found in SIG table \n",$string);
            $ok = 0;
        }
    } 
    ($hash, $ok);
}

# Load hash codes for hash code routine.
sub loadhash {
    local($sig, $hash);
    open(SIGS, $sigs) || &x("unable to open $sigs ($!)");
    while (<SIGS>) {
        ($hash, $sig) = split;
        $hashcodes{$sig} = hex($hash);
    }
    close SIGS;
}

# Dump hash codes from hash code routine.
sub dumphash {
    local($sig, $hash);
    while (($sig, $hash) = each %hashcodes) {
        printf SIG ("%08x $sig\n", $hash);
    }
    close(SIG);
}

# Get the parent directory of the pathname supplied.
sub parent {
    local($path) = @_;
    return ($path =~ s/\/[^\/]*$//) ? $path : undef;
}

# Emit code to foo.rech file.
sub emit {
    printf EXEC ("%08x @_\n", $checksum);
}

#sub rcx_preserve {
#   local(@path) = @_;
#   for $path(@path) {
#       $preserve{$path} = 1;
#   }
#}

sub rcx_mkdir {
	local(@path)=@_;
    local($perm,$path);

	# perm if specified is the 1st arg.
    if ( &numeric($path[0])) {
        $perm=shift(@path);
    } else {
        print "mkdir: mode not specified. Assuming 0777 \n" if $echo;
        $perm = "0777";
    }
    $perm = oct($perm);
    foreach $path (@path) {
        printf("mkdir 0%o $testdir/$path \n", $perm) if $echo;
        mkdir("$testdir/$path",$perm) ||
            &x("unable to make directory $testdir/$path ($!)");
        if ($compile) {
            &hash($path, &parent($path));
            &emit(sprintf("mkdir %o $path", $perm));
        }
    }
}

sub rcx_mkdirs {
    local($tmp,$count, $mode, $path );
    $tmp=$_[1];
    if ( &numeric($tmp)) {
        ($count,$mode,$path) = @_;
    } else {
        ($count, $path) = @_;
        $mode = "0777"  ;
    }
    print("mkdirs $count $mode $testdir/$path \n") if $echo;
    for ($ext = "0001"; $count--; $ext++) {
        &rcx_mkdir($mode,"$path.$ext");
    }
}

sub rcx_create {
    local(@path) = @_;
    for $path (@path) {
        print("create $testdir/$path\n") if $echo;
        open(FILE, ">$testdir/$path") ||
            &x("unable to create $testdir/$path ($!)");
        close(FILE);
        if ($compile) {

			# construct entry in link hash table
			&link_create("$path");

            &hash($path, &parent($path));
            &emit("create $path");
        }
    }
}

sub rcx_createfiles {
    local($count, $path) = @_;
    print("createfiles $count $testdir/$path \n") if $echo;
    for ($ext = "0001"; $count--; $ext++) {
        &rcx_create("$path.$ext");
    }
}

sub rcx_chmod {
    local($mode, @path) = @_;
    $mode = oct($mode);
    for $path (@path) {
        print("chmod $mode $testdir/$path\n") if $echo;
        chmod($mode, "$testdir/$path") ||
            &x("unable to chmod $mode $testdir/$path ($!)");
        if ($compile) {
            &hash($path);
            &emit(sprintf("chmod %o $path",$mode));
        }
    }
}

sub rcx_chmodpaths {
    local($count,$mode,$path)=@_;
    local($ext);
    print("chmodpaths $count $mode $testdir/$path \n") if $echo;    
    for ($ext = "0001"; $count--; $ext++) {
        &rcx_chmod($mode,"$path.$ext");
    }
}   

sub rcx_rm {
    local(@path) = @_;
    local($len,$force); 
    # force flag if specified is the 1st arg into the subroutine
    if ( &numeric($path[0]) ) {
        $force=shift(@path);
    }
    for $path (@path) { 
        $len=(stat($path))[7];                      
        if ( $len && !$force ) {
            # first reduce size of file to zero by truncate
            &rcx_truncate(0,$path);         
        }
        printf("unlink %s $testdir/$path\n", ($force ? "(force)" : "" )) if $echo;
        unlink("$testdir/$path") || &x("unable to remove $testdir/$path ($!)");
        if ($compile) {

			# Delete entry from link hash table
			&link_delete("$path");

            &hash($path, &parent($path));
            &emit("rm $path");
        }
    }
}

sub rcx_rmfiles {
    local($count, $force, $path) = @_;
    # $forceflag is optional. 
    if ( ! &numeric($force) ) {
        $path=$force;
# undef $force so that later checks do not decide that a flag was specified
        undef($force);
    }
    print("rmfiles $count %s $testdir/$path \n", ($force ? "(force)" : "" )) if $echo;
    for ($ext = "0001"; $count--; $ext++) {
        if (defined($force)) {
            &rcx_rm($force, "$path.$ext");      
        } else {
            &rcx_rm("$path.$ext");
        }
    }
}

sub rcx_chown {
    local($owner, @path) = @_;
    if ( ! &numeric($owner) ) {
        $owner=(getpwnam($owner))[2];
    }
    for $path (@path) {
        print("chown $owner $testdir/$path\n") if $echo;
        chown($owner, -1, "$testdir/$path") ||
            &x("unable to chown $owner $testdir/$path ($!)");
        if ($compile) {
            &hash($path);
            &emit(sprintf("chown %d $path",$owner));
        }
    }
}

sub rcx_chownpaths {
    local($count,$owner,$path)=@_;
    local($ext);
    if ( ! &numeric($owner) ) {
        $owner=(getpwnam($owner))[2];
    }
    print("chownpaths $count $owner $testdir/$path \n") if $echo;   
    for ($ext = "0001"; $count--; $ext++) {
        &rcx_chown($owner,"$path.$ext");
    }
}   

sub rcx_chgrp {
    local($group, @path) = @_;
    if ( ! &numeric($group) ) {
        $group=(getgrnam($group))[2];
    }
    for $path (@path) {
        print("chgrp $group $testdir/$path\n") if $echo;
        chown(-1, $group, "$testdir/$path") ||
            &x("unable to chgrp $group $testdir/$path ($!)");
        if ($compile) {
            &hash($path);
            &emit(sprintf("chgrp %d $path",$group));
        }
    }
}

sub rcx_chgrppaths {
    local($count,$group,$path)=@_;
    local($ext);
    print("chgrppaths $count $group $testdir/$path \n") if $echo;   
    for ($ext = "0001"; $count--; $ext++) {
        &rcx_chgrp($group,"$path.$ext");
    }
}   


sub rcx_rmdir {
    local(@path) = @_;
    for $path (@path) {
        printf("rmdir $path\n") if $echo;
        rmdir("$testdir/$path") || &x("unable to remove directory $testdir/$path ($!)");
        if ($compile) {
            &hash($path, &parent($path));
            &emit("rmdir $path");
        }   
    }
}

sub rcx_rmdirs {
    local($count, $path) = @_;
    print("rmdirs $count $testdir/$path \n") if $echo;
    for ($ext = "0001"; $count--; $ext++) {
        &rcx_rmdir("$path.$ext");
    }
}

sub rcx_mv {
    local($path1, $path2) = @_;
    print("mv $path1 $path2\n") if $echo;
    rename("$testdir/$path1", "$testdir/$path2") ||
        &x("Unable to rename $testdir/$path1$ext to $testdir/$path2$ext ($!)");
    if ($compile) {
        &hash("$path1", &parent("$path1"), "$path2", &parent("$path2"));
        &emit("mv $path1 $path2");
    }
}

sub rcx_mvpaths {
    local($count, $path1, $path2) = @_;
    local($ext);
    print("mvpaths $count $testdir/$path1 $testdir/$path2 \n") if $echo;
    for ($ext = "0001"; $count--; $ext++) {
        &rcx_mv("$path1.$ext","$path2.$ext");
    }
}

sub rcx_symlink {
    local($target, @source) = @_;
    local($source);
    for $source (@source) {
        print("symlink $target $testdir/$source \n") if $echo;
		symlink("$target","$testdir/$source") ||
            &x("unable to create symlink from $testdir/$source to a relative pathname $target ($!)");
        if ($compile) {
            &hash($source,&parent($source));
            &emit(sprintf("symlink $target $source"));
        }
    }
}   

sub rcx_symlinkpaths {
    local($count, $target, $source) = @_;
    local($ext);
    print("symlinkpaths $count $target $testdir/$source \n") if $echo;
    for ($ext = "0001"; $count--; $ext++) {
        &rcx_symlink("$target","$source.$ext");
    }
}

sub rcx_link {
    local($target, @source) = @_;
    local($source,$sibling,@sibling);
    for $source (@source) {
        print("link $testdir/$target $testdir/$source \n") if $echo;
        link("$testdir/$target","$testdir/$source") ||
            &x("unable toc reate hard link from $testdir/$source to $testdir/$target ($!)");
		if ($compile) {
			# Recompute signature for each of the files corresponding to 
			# this hard link - RA.
			&link_enter($target,$source);
			@sibling=&link_fetch($target);
			print "siblings are @sibling \n" if $debug;		
			for $sibling (@sibling) {
				&hash($sibling,&parent($sibling));
			}
			# Record system state only after the affect of 
			# the new signatures of all files corresponding to this
			# hard link has been taken into account for the system
  			# state checksum
            &emit(sprintf("link $target $source"));
        }
    }
}   

sub rcx_linkpaths {
    local($count, $target, $source) = @_;
    local($ext);
    print("linkpaths $count $testdir/$target $testdir/$source \n") if $echo;
    for ($ext = "0001"; $count--; $ext++) {
        &rcx_link("$target","$source.$ext");
    }
}

sub rcx_truncate {
    local($endlength, @path) = @_;
    local($curlength,$length,$force);
    # force flag if specified is the 2nd arg into the subroutine
    if ( &numeric($path[0]) ) {
        $force=shift(@path);
    }
    for $path (@path) {
		printf("truncate $endlength $testdir/$path (%s) \n", ($force ? "force" : "$fragsize bytes at a time")) if $echo;
		if ( $force ) {
            #
            # 1 = straight truncate to specified length
            #
            &aux_truncate($endlength, $path);                   
        } else {
            #
            # 0 = a frag at a time until specified length
            # frag size is contained in $fragsize global
            #
            $curlength=(stat("$testdir/$path"))[7];                
            if ( $curlength  <= $endlength ) {
                #
                # truncate upwards - does not work on all systems   
                #
                # Curlength may not be fragment size aligned
                # find least multiple of fragsize > curlength
                #
                $length = $curlength + ($fragsize - ($curlength % $fragsize));
                while ( $length <= $endlength) {
                    &aux_truncate($length,$path);                   
                } continue {
                    $length += $fragsize; 
                }
                # Endlength may not be fragment size aligned
                if ( $length > $endlength ) {
                    &aux_truncate($endlength,$path);                    
                }
            } else  {
                #
                # Curlength may not be fragment size aligned
                # find highest multiple of fragsize <= curlength
                #
                $length =  $curlength - ( $curlength % $fragsize ) ;
                if ( $length == $curlength ) {
                    $length -= $fragsize;
                }
                while ( $length >= $endlength) {
                    &aux_truncate($length,$path);                   
                } continue {
                    $length -= $fragsize; 
                }
                # Endlength may not be fragment size aligned
                if ( $length < $endlength ) {
                    &aux_truncate($endlength,$path);                    
                }
            }
        }
    }
}

sub aux_truncate {
    local($length,$path)=@_;
#   local($fh);
    printf("  truncate $length $testdir/$path\n") if $echo;
    truncate("$testdir/$path", $length) ||
        &x("unable to truncate $length $testdir/$path ($!)");
    if ($compile) {
        &hash($path);
            &emit(sprintf("truncate %d $path",$length));
    }
}

# truncatepaths cnt len forceflag path
# forceflag = optional numeric
#    0 = a frag/block at a time until specified length [Default]
#    1 = straight truncate to specified length
    
sub rcx_truncatepaths {
    local($count,$length,$force,$path)=@_;
    local($ext);
    # $forceflag is optional. 
    if ( ! &numeric($force) ) {
        $path=$force;
# undef $force so that later checks do not decide that a flag was specified
        undef($force);
    }
    printf("truncatepaths $count $length %s $testdir/$path \n",
            ( $force ? "(force)" : "" ) ) if $echo;
    for ($ext = "0001"; $count--; $ext++) {
        if (defined($force)) {
            &rcx_truncate($length,$force,"$path.$ext");
        } else {
            &rcx_truncate($length,"$path.$ext");
        }
    }
}   

# write char numbytes offset whence [forceflag] file1 file2...
sub rcx_write {
    local($char, $length, $offset, $whence, @path) = @_;
	local($count,$force,$file); 
	$file=YO;
    # force flag if specified is the 5th arg into the subroutine
    if ( &numeric($path[0]) ) {
        $force=shift(@path);
    }

    for $path (@path) {
        printf("write $length $offset $whence (%s) $testdir/$path \n",
				($force ? "force" : "1 byte at a time") )if $echo;
		
		open($file,"+<$testdir/$path");
		seek($file,$offset,$whence);

		if ( $force ) {
            #
            # 1 = straight write to specified length
            #
			&aux_write($char,$length,$file,"$testdir/$path");
       	} else {
			for ($count=0; $count < $length; $count++) {
				&aux_write($char,1,$file,"$testdir/$path");
			}			
		}
		close($file);					
	}
}

sub aux_write {
    local($char, $datalen, $filehandle,$fileabspath)=@_;
	local($data,$writeoffset,$writelen);

	printf("  write $char $datalen $fileabspath \n") if $echo;

	$data = $char x $datalen;
	$writeoffset=0;
	while ($datalen) {
		$writelen=syswrite($filehandle,$data,$datalen,$writeoffset);
		if ( !defined($writelen)) {
	        &x("unable to write $datalen $fileabspath ($!)");
		}
		printf("    wrote $char $writelen $fileabspath \n") if $echo;
	    if ($compile) {
	        &hash($path);
            &emit(sprintf("write $char %d $fileabspath",$writelen));
	    }

		$datalen -=$writelen;
		$writeoffset += $writelen;
	}
}

# writefiles cnt char numbytes offset whence [forceflag] file
sub rcx_writefiles {
    local($count, $char, $length, $offset, $whence, $force, $path) = @_; 
    local($ext);

    # force flag if specified is the 6th arg into the subroutine
    if ( ! &numeric($force)) {
        $path=$force;
		undef($force);
    }
    printf("truncatepaths $count $length %s $testdir/$path \n") if $echo;

    printf("writepaths $count $char $length $offset $whence %s $testdir/$path\n",( $force ? "(force)" : "" )) if $echo;  
    for ($ext = "0001"; $count--; $ext++) {
		if (defined($force)) {
	    	&rcx_write($char,$length,$offset, $whence, $force, "$path.$ext");
		} else {
	    	&rcx_write($char, $length, $offset, $whence, "$path.$ext");		
		}
    }
}   

sub numeric {
    local($word)=@_;
    return ( $word =~ /^\d*$/ );
}

#
# Code to manage hard links to ensure that signatures for all files 
# corresponding to a hard link are recalculated whenever any operation
# is done to any of these files.  -- RA 11/5/92
#

$link_debug=1;

sub link_create {
	local($a)=@_;
	print "link_create $a \n" if $link_debug;
	if ( $hash{$a} ) {
		warn " hash{$a} already exists with value $hash{$a}. Ignoring current request ";
	} else {
		$hash{$a}=$a;	
	}
}

sub link_delete {
	local($a) = @_;
	local($i,@newsibling,$newsiblinglist,@sibling,$sibling);
	print "link_delete $a \n" if $link_debug;
	@sibling=&link_fetch($a);
	for ( $i = 0; $i <= $#sibling; $i++ ) {
		if ( $sibling[$i] ne $a ) {
			push(@newsibling, $sibling[$i]);
		}
	}
	$newsiblinglist=join(':',@newsibling);
	foreach $sibling ( @sibling ) {
		$hash{$sibling}=$newsiblinglist;
	}
	undef($hash{$a});
}

sub link_enter { 
	local($a,$b)=@_;
	local($newlist, $sibling, @sibling);
	print "link_enter $a $b \n" if $link_debug ;
	if (! $hash{$a} ) {
		warn " hash{$a} does not exist. Creating hash{$a} with $a:$b ";
		$hash{$a}=join(':',$a,$b);
		$hash{$b}=$hash{$a};
	} else {
		$newlist=join(':',$hash{$a},$b);		
		@sibling=&link_fetch($a);		
		foreach $sibling (@sibling) {
			$hash{$sibling}=$newlist;
		}
		$hash{$b}=$hash{$a};
	}
}

# return array
sub link_fetch {
	local($key)=@_;
	local(@val);
	$val=$hash{$key};
	@val=split(':',$val);
	print "link_fetch($key) = [ @val ] \n" if $link_debug ;
	@val;
}

sub link_display {
	local($key,$val,@val);
	print "****links hash table is ***** \n"; 
	while ( ($key,$val) = each(%hash) ) {
		@val=split(':',$val);
		print " key $key -> ( @val ) \n";		
	}
}

# Some routine similar to ones in runrcx.pl
sub x2 {
    local($exitMsg, $exitVal) = @_;
    local($msg);
    $msg = "$0 script died: $exitMsg ";
    if ($exitVal) {
       $msg .= "(with error $exitVal)";
    }
    if ($exitVal) {
       $! = $exitVal;
    }
	die("rcx: $msg\n");
}

# Emacs settings for this file follow.
# Local Variables:
# tab-width:4
# End:
