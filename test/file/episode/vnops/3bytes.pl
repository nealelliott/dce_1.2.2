#!/afs/tr/fs/dev/perl

# /* Copyright (C) 1994 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/test/file/episode/vnops/RCS/3bytes.pl,v 1.1 1994/09/07 16:00:31 blake Exp $

# 3bytes.pl -- is used to check for db5831 (aka "backward in time").  The basic
#     idea is to move a fileset while growing a large number of files from
#     empty to three bytes in length.
#
#     The setup for the script assumes that the current machine (named by
#     $HOST) is a dfs file exporter.  It creates a fileset and locally mounts
#     it.  The local path is used to modify the fileset and check that the move
#     was successful.
#
#     After setting up the initial conditions, the script forks itself.  The
#     child grows files to three bytes, creating them if necessary.  Meanwhile
#     the parent runs "fts move".  When the move fails the child is told to
#     stop and report how many files it wrote.  The parent then checks the
#     fileset for consistency aborting if it finds any.

if ($ARGV[0] eq '-setup') {
    shift;
    $doSetup = 1;
}
($aggr1, $aggr2) = @ARGV[0..1];
$host = $ENV{HOST};			# local file server host name
$ft = "three-byte.$host.test";
$mp = "/tmp/3byte";

$doSetup = 1 if (!(-d $mp));
`df|egrep -s '$ft'`;
$doSetup = 1 if ($? != 0);

$|=1;

&Setup if ($doSetup);

sub Setup
{
    mkdir ($mp, 0700) || (-d $mp)
	|| die ("Couldn't create mount point dir $mp: $!");
    @fts = `fts create $ft $host $aggr1 2>&1`;
    if ($? != 0) {
	die ("Couldn't create $ft:\n" . &Prefix("fts", @fts));
    }
    `epimount $ft $mp`;
    ($? == 0) || die "Failed to mount $ft on $mp";
}

sub Loop
{
    unlink "$mp/highwater";
    $pid = fork;
    if (!$pid) {
	print "...Starting 3 byte writes in pid $$\n";
	&WriteThrees($mp);
	exit (0);
    }

    print "Moving $ft from $aggr1 to $aggr2\n";
    @fts = `fts move $ft $host $aggr1 $host $aggr2 2>&1`;
    if ($? != 0) {
	die ("Couldn't move $ft from $aggr1 to $aggr2:\n" . &Prefix("fts", @fts));
    }
    ($aggr1, $aggr2) = ($aggr2, $aggr1); # swap aggregates

    kill 'INT',$pid;
    `sync`;
    (wait == $pid) || warn ("awaited funny pid");

    open (FILE, "$mp/highwater") || die ("Couldn't open $mp/highwater: $!");
    $_ = <FILE>;
    ($nFiles) = /^(\d+)$/;
    $nFiles || die ("Bad count in highwater file: '$_'");
    close FILE;
    print "Child wrote $nFiles 3-byte files.\n";
    &ClearThrees($mp, $nFiles);
}

print "This is pid $$\n";
while (++$iter) {
    print "Starting iteration #$iter.\n";
    &Loop;
}


sub FinishThrees
{
    $done = 1;
}

sub WriteThrees
{
    local ($mp) = @_;			# working directory for files
    local ($i, $file);

    $done = 0;
    $SIG{'INT'} = 'FinishThrees';

    while (!$done) {
	$i++;
	$file = "$mp/3byte.$i";
	open (FILE, ">$file") || die ("Couldn't create $file: $!");
	print FILE "abc";
	close FILE;
	print "...Wrote $i 3-byte files.\n" if ($i % 100 == 0);
    }
    open (FILE, ">$mp/highwater") || die ("Couldn't create $mp/highwater: $!");
    print FILE "$i\n";
    close FILE;
}

sub ClearThrees
{
    local ($mp, $n) = @_;		# working directory for files and count
    local ($i, @stat, $file);

    for ($i=1; $i<=$n; $i++) {
	$file = "$mp/3byte.$i";
	(@stat = stat($file)) || die ("File $file is missing: $!\n");
	($stat[7] == 3) || die ("Size of $file is $stat[7]");
	# truncate
	open (FILE, ">$file");
	close FILE;
    }
    print "Truncated $n 3-byte files";
    # now flush all the extra
    while (1) {
	$file = "$mp/3byte.$i";
	(-e $file) || last;
	(-z $file) || die ("Non-empty file $file found after trunc phase.");
	unlink $file;
	$i++;
    }
    print " and removed ".($i-$n-1)." others" if ($i > $n);
    print ".\n";
}

sub Prefix
{
    local ($pre, @output) = @_;
    $pre = " $pre: ";
    $pre . join ($pre, @output);
}
