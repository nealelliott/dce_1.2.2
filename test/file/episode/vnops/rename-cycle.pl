#!/afs/tr/proj/tools/bin/perl

# /* Copyright (C) 1995 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/test/file/episode/vnops/RCS/rename-cycle.pl,v 1.1 1995/08/24 13:59:10 ota Exp $

# rename-cycle -- derrived from rename-file.  This tests creates files in a
#     group of subdirs and deletes them from a (possibly) different group.
#     Meanwhile rename processes move the files from one subdir to the next
#     until they are all taken out by the delete process(es).
#
#     This program is works pretty well in the following configuration but
#     hasn't been thoroughly tested in other arrangements, such as more than
#     two dirs, or zero deleters.

# The following settings caused some failures due to db6835 on a 2-way MP
# running on a locally mounted Episode fileset.
#     sb1# perl ~/junk/rename-cycle.pl 10002 2 3
#     Creating 10002 with 3 procs (@ 3334), 2 procs renaming (@ 100020) and 1 deleting (@ 10002).
#     Rename c762.2152 to r764.4262 failed with I/O error
#     Rename r765.2974 to r764.9641 failed with I/O error
#     Rename r765.3073 to r764.9739 failed with I/O error
#     Finished.
#          Type  succ skip rstrt
#     01: Create 3334    0    0
#     02: Create 3334    0    0
#     03: Create 3334    0    0
#     04: Delete 10002  502  200
#     05: Rename 10914    3    7
#     05: Rename c762.2152 to r764.4262 failed with I/O error
#     05: Rename r765.2974 to r764.9641 failed with I/O error
#     05: Rename r765.3073 to r764.9739 failed with I/O error
#     06: Rename 4246 4509  131
#     70.27u 708.26s 12:58.53 100.0%

$dir_root = 'rename-test';
$dir_perm = 0777;

$ENOENT = 2;

&Usage if ($ARGV[@ARGV-1] =~ /^-h/i);

sub Usage
{
    print STDERR "Usage: $0 [<# files> [<renamers> [<creaters> [<deleters> [<sub dirs>]]]]]\n";
    print STDERR "    Test runs in ./$dir_root.\n";
    exit 1;
}

$nfiles = ($ARGV[0] || 100);
$renamors = ($ARGV[1] || 2);
$creators = ($ARGV[2] || 1);
$deletors = ($ARGV[3] || 1);
$nsubdirs = ($ARGV[4] || 2);
$procs = $creators+$deletors+$renamors;

$verbose = 0;

$ncreate = int($nfiles/$creators);
$ndelete = ($deletors ? int($nfiles/$deletors) : 0);
$nrename = 10*$nfiles;
print "Creating $nfiles with $creators procs (@ $ncreate), $renamors procs renaming (@ $nrename) and $deletors deleting (@ $ndelete).\n";

(($ncreate*$creators == $nfiles) && ($ndelete*$deletors == $nfiles))
    || die ("Uneven workload: try setting tasks to lcm of # procs of each type");

`rm -rf $dir_root`;
mkdir($dir_root, $dir_perm) || die "mkdir $dir_root: $!\n";
chdir($dir_root) || die "can't cd to $dir_root: $!\n";

if ($nsubdirs > 1) {
    @subdirs = grep (($_ = "d$_", mkdir($_, 0777) || die ("mkdir $_: $!\n")),
		     (1..$nsubdirs));
} else {
    @subdirs = (".");
}

print "Forking\n" if ($verbose);
@pids = ();
for ($i = 0; $i < $procs; $i++) {
    $p = fork;
    last if ($p == 0);
    push (@pids, $p);
}

if ($p == 0) {
    # for each child...
    $successes = $skips = $restarts = 0;
    @errors = ();
    # schedule some work
    if ($i < $creators) {
	&Create($subdirs[$i % @subdirs]);
    } elsif ($i < $deletors+$creators) {
	&Delete($subdirs[($nsubdirs-1-$i-$creators) % @subdirs]);
    } else {
	&Rename($subdirs[$i % @subdirs], $subdirs[($i+1) % @subdirs]);
    }
}

sub PrintResults
{
    open (RESULTS, ">results.$$") || die ("Couldn't open file results.$$: $!");
    printf RESULTS ("%s %4d %4d %4d\n", $type, $successes, $skips, $restarts);
    print RESULTS join('',@errors);
    close RESULTS;
}

sub Create
{
    local ($d) = @_;
    $type = "Create";
    print "$$ doing $type in $d\n" if ($verbose);
    for (1 .. $ncreate) {
	$f = "$d/c$$.$_";
	open (F, ">$f")
	    || die ("Can't create $f: $!");
	close F;	
	$successes++;
    }
}

sub Cleanup
{
    local ($sig) = @_;
    &PrintResults();
    exit 0;
}

sub Rename
{
    local ($src, $dst) = @_;
    $type = "Rename";
    print "$$ doing $type from $src to $dst\n" if ($verbose);
    $SIG{'INT'} = 'Cleanup';
    for ($i=1; $successes<$nrename;) {
	$success = 0;
	opendir (D, $src) || warn("failed to open '$src': $!");
	while ($f = readdir(D)) {
	    if ($f =~ /^[rc]\d/) {
		$r = "r$$.$i";
		$success = rename("$src/$f", "$dst/$r");
		if ($success) {
		    $successes++;
		    $i++;
		} else {
		    if ($! != $ENOENT) {
			$m = "Rename $f to $r failed with $!\n";
			print $m;
			push (@errors, $m);
			`dfstrace setset -inactive`;
		    }
		    $skips++;
		}
	    }
	}
	closedir D;
	if ($success) {
	    $restarts++;
	    sleep 1;
	    # try again
	}
    }
}

sub Delete
{
    local ($d) = @_;
    $type = "Delete";
    print "$$ doing $type in $d\n" if ($verbose);
    while ($successes < $ndelete) {
	$success = 0;
	opendir (D, $d);
	while ($f = readdir(D)) {
	    if ($f =~ /^r\d/) {
		$success = unlink "$d/$f";
		if ($success) {
		    $successes++;
		} else {
		    if ($! != $ENOENT) {
			$m = "Delete of $d/$f failed with $!\n";
			print $m;
			push (@errors, $m);
		    }
		    $skips++;
		}
	    }
	}
	closedir D;
	if (!$success) {
	    $restarts++;
	    sleep 1;
	    # try again
	}
    }
}

if (!$p) {
    &PrintResults();
} else {
    # parent waits for all children
    foreach ($creators..$creators+$deletors-1) {
	$p = waitpid ($pids[$_],0);
	$p == $pids[$_] || die ("illegal deletor $pids[$_]: $!");
    }
    kill 'INT',@pids;			# kill renaming procs
    1 while (($pid = wait) > 0);
    print "Finished.\n";
    print "     Type  succ skip rstrt\n";
    $i = 0;
    foreach $pid (@pids) {
	$i++;
	$prefix = sprintf ("%02d", $i);
	$r = "results.$pid";
	if (open (RESULTS, $r)) {
	    @r = <RESULTS>;
	    close RESULTS;
	    unlink $r;
	} else {
	    print "$prefix: PROCESS FAILED\n";
	    next;
	}
	foreach $_ (@r) {
	    print "$prefix: $_";
	}
    }
}
