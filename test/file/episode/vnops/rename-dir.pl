#!/afs/tr/fs/dev/perl

# /* Copyright (C) 1994 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/test/file/episode/vnops/RCS/rename-dir.pl,v 1.2 1994/08/19 22:49:55 blake Exp $

# rename-dir -- exercises the rename check path code.  It does this by
#     generating a number of uniquely named directories and repeatedly randomly
#     renaming one under another.
#
#     The basic loop selects two directories at random, locates the pathname of
#     each, then renames the first under the second.  The program uses two
#     techniques for determining the pathname of a directory.  The fastest
#     depends on a database of parents, by traversing this database the
#     pathname can be easily determined.  However, if multiple processes are
#     running these pathnames may not be correct and the rename will get an
#     ENOENT.  If this happens the program does an exhaustive search using
#     readdir (incidently updating its parent database).  This, of course, may
#     also fail to get accurate pathnames, but if this fails it chalks up
#     failure, and goes on.

$dir_root = 'rename-test';
$dir_perm = 0777;

$ENOENT = 2;
$EINVAL = 22;

@letters = 'a' .. 'z';
sub Radix26
{
    local ($r) = @_;
    if ($r < 26) {
	$letters[$r];
    } else {
	&Radix26(int($r/26)) . $letters[$r % 26];
    }
}

`rm -rf $dir_root`;
mkdir($dir_root, $dir_perm) || die "mkdir $dir_root: $!\n";
chdir($dir_root) || die "can't cd to $dir_root: $!\n";
$dot = ".";

$procs = ($ARGV[0] || 1);
$iter = ($ARGV[1] || 100);
$ndirs = ($ARGV[2] || 10);

@dirs = ();
for ($i = 0; $i < $ndirs; $i++) {
    push (@dirs, &Radix26($i));
    mkdir($dirs[$i], $dir_perm) || die "mkdir $dir_root/$i: $!\n";
    $parent{$dirs[$i]} = $dot;
}

$verbose = $procs < 2;
$lost = $win = $fastTry = 0;
%errno = ();

$p = 1;
for ($i = 1; $i < $procs; $i++) {
    $p = fork;
    last if ($p == 0);
}

for (1 .. $iter) {
    $a = $dirs[int(rand(@dirs))];
    $b = int(rand(1+@dirs));		# allow "." as a possible destination
    $b = (($b >= @dirs) ? '.' : $dirs[$b]);

    $pathA = &LastDir($a);
    $pathB = &LastDir($b);

    $goodTry = ($pathA && $pathB);
    if ($goodTry) {
	$newA = "$pathB/$a";
	$r = rename($pathA, $newA);
	$errno = 0 + $!;
	$goodTry = ($r != 0 || $errno != $ENOENT);
    }
    if ($goodTry) {
	# count it as a good try if pathnames were okay
	$fastTry++;
    } else {
	# else try finding the names by exhaustive search
	$pathA = &FindDir("", $dot,$a);
	if (!$pathA) {
	    $lost++;
	    next;
	}
	$pathB = &FindDir("", $dot,$b);
	if (!$pathB) {
	    $lost++;
	    next;
	}
	$newA = "$pathB/$a";
	$r = rename ($pathA, $newA);
	$errno = 0+$!;
    }

    if (!$r) {
	print "Rename $pathA to $newA failed with $errno\n" if ($verbose);
	$errno{$errno}++;
	print "Got $errno!\n" if ($errno != $ENOENT && $errno != $EINVAL);
    } else {
	$parent{$a} = $b;
	$win++;
    }
}

if (open (RESULTS, ">results.$$")) {
    print RESULTS   "Succeeded  lost-name  failed  fast-try\n";
    printf RESULTS ("      %3d        %3d     %3d       %3d\n",
		    $win, $lost+$errno{$ENOENT}, $errno{$EINVAL}, $fastTry);
    delete $errno{$ENOENT};
    delete $errno{$EINVAL};
    foreach $errno (keys(%errno)) {
	$!=$errno;
	$msg="$!";
	print RESULTS
	    "Got $errno{$errno} rename failures with errno=$errno ($msg)\n";
    }
    close RESULTS;
} else {
    warn ("Couldn't open file results.$$: $!");
}

if ($p) {
    # parent collects all the results
    $i = 0;
    $successes = $losses = $failures = 0;
    push (@pids, $pid) while (($pid = wait) > 0);
    push (@pids, $$);
    foreach $pid (@pids) {
	$i++;
	$prefix = sprintf ("%02d", $i);
	open (RESULTS, "results.$pid") || next;
	@r = <RESULTS>;
	close RESULTS;
	foreach $_ (@r) {
	    print "$prefix: $_";
	    next unless (($s, $l, $f) = /\s*(\d+)\s+(\d+)\s+(\d+)/);
	    $successes += $s;
	    $losses += $l;
	    $failures += $f;
	}
    }
    print "Total successful renames $successes with $losses lookup failures and $failures illegal renames\n";
    $total = ($successes + $losses + $failures);
    if ($total != $procs*$iter) {
	printf "Total was $total, expected %d\n", $procs*$iter;
    }
}

%parent = ();				# last seen parent of each dir.

sub LastDir
{
    local($p) = @_;
    local($loop) = 0;
    local(@comp) = ($p);
    until ($p eq '.') {
	($p = $parent{$p}) || return undef;
	($loop++ < $ndirs) || return undef;
	unshift (@comp, $p);
    }
    join ('/', @comp);
}

sub FindDir
{
    local ($path, $p, $d) = @_;
    local ($e, $found);

    if ($p eq '.') {
	$path = $p;
    } else {
	$path .= "/$p";
    }
    opendir(DIR, $path) || return undef;
    foreach $e (readdir(DIR)) {
	next if ($e eq "." || $e eq "..");
	$parent{$e} = $p;
	if ($e eq $d) {
	    $found = "$path/$e";
	    last;
	} elsif ($found = &FindDir($path, $e, $d)) {
	    last;
	}
    }
    closedir(DIR);
    $found;
}
