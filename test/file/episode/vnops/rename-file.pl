#!/afs/tr/proj/tools/bin/perl

# /* Copyright (C) 1995 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/test/file/episode/vnops/RCS/rename-file.pl,v 1.1 1995/08/24 13:59:11 ota Exp $

# rename-file -- derrived from rename-dir.  It is intended to module the
#     behavior of the jobgen WRT Episode renames.
#
#     I had occasional luck reproducing db6835 with args 16 10000 4 2.

$dir_root = 'rename-test';
$dir_perm = 0777;

$ENOENT = 2;

&Usage if ($ARGV[@ARGV-1] =~ /^-h/i);

sub Usage
{
    print STDERR "Usage: $0 [<total procs> [<files to create> [<create procs> [<delete procs> [<# subdirs>]]]]]\n";
    print STDERR "    Test runs in ./$dir_root.\n";
    exit 1;
}

$procs = ($ARGV[0] || 8);
$files = ($ARGV[1] || 100);
$creators = ($ARGV[2] || $procs/4);
$deletors = ($ARGV[3] || $creators);

$renamors = $procs-($creators+$deletors);

$lcm = $creators*$deletors*$renamors;
foreach $f (2,3,5,7,11) {
    $newLCM = $lcm/$f;
    $c = grep (int($newLCM/$_)*$_ == $newLCM,
	       ($creators, $deletors, $renamors));
    if ($c == 3) {
	$lcm = $newLCM;
	redo;
    }
}
$nfiles = int(($files + ($lcm-1))/$lcm) * $lcm;

$ncreate = int($nfiles/$creators);
$ndelete = int($nfiles/$deletors);
$nrename = int($nfiles/$renamors);
print "Creating $nfiles with $creators procs (@ $ncreate), $renamors procs renaming (@ $nrename) and $deletors deleting (@ $ndelete).\n";

(($procs > 0) &&
 ($ncreate*$creators == $nfiles) && ($ndelete*$deletors == $nfiles) &&
 ($nrename*$renamors == $nfiles))
    || die ("Uneven workload: try setting tasks to lcm of # procs of each type");

`rm -rf $dir_root`;
mkdir($dir_root, $dir_perm) || die "mkdir $dir_root: $!\n";
chdir($dir_root) || die "can't cd to $dir_root: $!\n";

@pids = ();
for ($i = 0; $i < $procs; $i++) {
    $p = fork;
    last if ($p == 0);
    push (@pids, $p);
}

if ($p == 0) {
    $successes = $skips = $restarts = 0;
    @errors = ();
    # schedule each child for work
    if ($i < $creators) {
	&Create();
    } elsif ($i < $deletors+$creators) {
	&Delete();
    } else {
	&Rename();
    }
}

sub Create
{
    $type = "Create";
    for (1 .. $ncreate) {
	$f = "c$$.$_";
	open (F, ">$f")
	    || die ("Can't create $f: $!");
	close F;	
	$successes++;
    }
}

sub Rename
{
    $type = "Rename";
    for ($i=1; $successes<$nrename;) {
	$success = 0;
	opendir (D, ".") || warn("failed to open '.': $!");
	while ($f = readdir(D)) {
	    if ($f =~ /^c\d/) {
		$r = "r$$.$i";
		$success = rename($f, $r);
		if ($success) {
		    $successes++;
		    $i++;
		    last if ($successes >= $nrename);
		} else {
		    if ($! != $ENOENT) {
			$m = "Rename $f to $r failed with $!\n";
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

sub Delete
{
    $type = "Delete";
    while ($successes < $ndelete) {
	$success = 0;
	opendir (D, '.');
	while ($f = readdir(D)) {
	    if ($f =~ /^r\d/) {
		$success = unlink $f;
		if ($success) {
		    $successes++;
		    last if ($successes >= $ndelete);
		} else {
		    if ($! != $ENOENT) {
			$m = "Delete of $f failed with $!\n";
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
    open (RESULTS, ">results.$$") || die ("Couldn't open file results.$$: $!");
    printf RESULTS ("%s %4d %4d %4d\n", $type, $successes, $skips, $restarts);
    print RESULTS join('',@errors);
    close RESULTS;
} else {
    # parent waits for all children
    $base = time;
    while (1) {
	sleep 1;
	opendir (D, ".");
	@f = grep(/^[cr][1-9]/,readdir(D));
	closedir D;
	$time = time - $base;
	$count = @f;
	push (@times, sprintf("%-4d %5d\n", $time, $count));
	push (@counts, $count);
	shift @counts if (@count > 10);	# keep last 10 counts
	last				# finished because files are all gone
	    if ((@counts > 5) && ($count == 0));
	if (10 == grep ($count == $_, @counts)) {
	    # bail because we're stuck
	    print STDERR "killing children because something is stuck\n";
	    kill 'INT', @pids;
	    last;
	}
    }

    # dump plot directory size progess
    if (open (PLOT, ">plot.$$")) {
	print PLOT join('',@times);
	close PLOT;
    } else {
	warn "Couldn't write plot file: $!";
    }

    # collects all the results
    $i = 0;
    1 while (($pid = wait) > 0);
    print "Finished.\n";
    print "      Type  succ skip rstrt\n";
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
