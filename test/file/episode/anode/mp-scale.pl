#!/afs/tr/fs/dev/perl

# /* Copyright (C) 1995, 1994 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/test/file/episode/anode/RCS/mp-scale.pl,v 1.3 1995/02/07 19:18:41 ota Exp $

shift if ($clean = ($ARGV[0] eq '-clean'));
shift if ($delete = ($ARGV[0] eq '-delete'));

$nfiles = 200;
(@ARGV <= 1) || die("usage: $0 [<#procs>]");
$nprocs = ($ARGV[0] || 4);

$dir = "mp-scale";
$cwd = "/tmp";
chdir ($cwd);
#$cwd = `pwd`;
#chop $cwd;

sub MakeMp
{
    local ($i) = @_;
    local ($d) = "$dir/mp.$i";
    return if (-d $d);
    mkdir ($d, 0777) || die ("Couldn't make dir $d for mountpoint: $!");
    system ("/opt/dcelocal/bin/epimount ota.mp-scale.$i $d")
	&& die ("epimount failed");
}

if (! -d $dir) {
    print "Creating $cwd/$dir\n";
    mkdir ($dir, 0777);
}

for $i (1..$nprocs) {
    &MakeMp($i);
}

sub CChild {
    local ($i) = @_;
    $base = "$dir/mp.$i/base.$$";
    mkdir ($base, 0777) || die ("In child $i couldn't mkdir $base: $!");
    chdir ($base);
    foreach $i (1..$nfiles) {
	open (FILE, ">f$i") || die ("Couldn't create file $cwd/$base/f$i: $!");
	close (FILE) || die ("Couldn't create file $cwd/$base/f$i: $!");
    }
}

sub DChild {
    local ($i) = @_;
    local (@base) = <$dir/mp.$i/base.*>;
    local ($base, @files);

    foreach $base (@base) {
	chdir ("$cwd/$base");
	opendir (DIR, ".");
	@files = readdir DIR;
	closedir DIR;
	$ndeleted = unlink @files;
	($ndeleted == @files-2)
	    || die ("Couldn't unlink files in $cwd/$base: $!");
	chdir ("..");
	rmdir ("$cwd/$base")
	    || die ("Couldn't cleanup by removing dir $cwd/$base: $!");
    }
}

sub Create
{
    print "Forking $nprocs children\n";
    for $i (1..$nprocs) {
	$p = fork;
	if ($p == 0) {
	    &CChild($i);
	    exit (0);
	}
    }
    1 while (wait > 0);
}

sub Delete
{
    print "Forking $nprocs children\n";
    for $i (1..$nprocs) {
	$p = fork;
	if ($p == 0) {
	    &DChild($i);
	    exit (0);
	}
    }
    1 while (wait > 0);
}

if ($clean || $delete) {
    @files = <$dir/mp.1/base.*/*>;
    if ($delete && (@files != $nfiles)) {
	print "Cleaning first...\n  ";
	&Delete;
	print "  creating\n  ";
	&Create;
	print "  done creating\n";
    }
    @start = (time, times);
    &Delete;
} else {
    @files = <$dir/mp.1/base.*>;
    if (@files > 0) {
	print "Deleting first\n  ";
	&Delete;
	print "  done deleting\n";
    }
    @start = (time, times);
    &Create;
}

@stop = (time, times);
$elapse = $stop[0] - $start[0];
$cpu = 0;
grep ($cpu += $_, @stop[1..4]);
grep ($cpu -= $_, @start[1..4]);
$cpu = sprintf("%4g", $cpu);
$n = $nprocs*$nfiles;
print((($clean||$delete)?"Deleted":"Created") .
      " $n dirs and $n files (in $nprocs procs) in $elapse sec and $cpu cpu.\n");

exit (0);
