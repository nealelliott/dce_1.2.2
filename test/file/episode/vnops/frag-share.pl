#!/afs/tr/fs/dev/perl

# /* Copyright (C) 1994 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/test/file/episode/vnops/RCS/frag-share.pl,v 1.2 1994/08/17 17:43:34 rajesh Exp $

# This test exposes a data consistency bug where VM writes of fragmented files
# to the disk will be over-written by a concurrent restore of a different
# fileset also containing fragmented files if both fileset are on the same
# aggregate. [DB 5787]

# The test finds a small aggregate on the local host.  The aggregate may
# optionally be specified explicitly in the first command line parameter.  The
# second parameter, if present, gives the base name of the filesets created by
# the test.

# The test uses epimount to locally access the data.  It also uses dfsexport to
# detach and re-attach the aggregate.  This will fail if there are other local
# mounts or active users of the aggregate.  The aggregate should have at least
# 1M free and have a 8K/1K organization.

$host = $ENV{HOST};			# local file server host name
$aggr;					# chosen aggr to use for tests
$mpRoot = "/tmp";			# where to put local mount points

%cleanup = ();

sub Cleanup
{
    local ($signal) = @_;
    print "Caught $signal signal\n" if ($signal);
    close (TEST);
    close (FILE);
    close (FTS);
    foreach $action (values(%cleanup)) {
	print "$action\n" if ($signal);
	eval "$action";
    }
    exit (0);
}

@filesets = ();

sub CleanFilesets
{
    local (@ftsOutput, $ftsMsg);

    &CleanLocalMounts;
    foreach $f (@filesets) {
	@ftsOutput = `fts delete $f $host $aggr 2>&1`;
	if ($?) {
	    $ftsMsg = &Prefix(FTS, @ftsOutput);
	    ($ftsMsg =~ /FLDB: no such entry/)
		|| warn ("Couldn't delete $f:\n$ftsMsg");
	}
    }
}

@localMounts = ();

sub CleanLocalMounts
{
    foreach $mp (@localMounts) {
	&Unmount ($mp);
    }
}

sub Die
{
    local ($msg) = @_;
    print STDERR "Bailing out because: $msg\n";
    &Cleanup(DIE);
    exit (1);
}

%ftHost = ();
%ftAggr = ();
%ftId = ();
%ftMP = ();
%mpFT = ();

sub CreateFT
{
    local ($name) = @_;
    local (@output, $id);

    $cleanup{"Filesets"} = "&CleanFilesets";
    push (@filesets, $name);
    open (FTS, "fts create $name $host $aggr|")
	|| &Die ("Can't start fts create $name: $!");
    @output = <FTS>;
    close (FTS)	|| &Die ("Can't finish fts create $name: $! doing:\n" .
			 &Prefix (FTS, @output));

    $ftHost{$name} = $host;
    $ftAggr{$name} = $aggr;
    foreach (@output) {
	($id) = (/readWrite\s*ID ([\d,]+)\s+valid/);
	$ftId{$name} = $id if ($id);
    }
    $ftId{$name} || &Die ("fts create $name output unrecognized:\n" .
			  &Prefix (FTS, @output));
}

sub Mount
{
    local ($ft) = @_;
    local ($mp) = "$mpRoot/$ft";

    $cleanup{"LocalMounts"} = "&CleanLocalMounts";
    push (@localMounts, $mp);
    
    ((-d $mp) || mkdir ($mp, 0700))
	|| &Die ("Couldn't create mp dir $mp: $!");
    $rc = system ("epimount", $ft, $mp);
    ($rc == 0) || &Die ("Couldn't mount $ft at $mp");
    $mpFT{$mp} = $ft;
    $ftMP{$ft} = $mp;
}

# Unmounts a fileset by mount point.  Returns zero for failure and nonzero for
# success.

sub Unmount
{
    local ($mp) = @_;
    local ($ft) = $mpFT{$mp};
    local (@output);

    if ($ft) {
	delete $ftMP{$ft};
	delete $mpFT{$mp};
	@output = `umount $mp 2>&1`;
	if ($? && (&Prefix (UnM, @output) =~ /busy/i)) {
	    system ("sync");
	    sleep 1;
	    @output = `umount $mp 2>&1`;
	}
	if ($?) {
	    warn ("Failed to unmount $mp:\n" . &Prefix (UnM, @output));
	    return 0;
	} else {
	    return rmdir $mp;
	}
    }
    return 0;
}

sub GetAggr
{
    $aggr = $ARGV[0];
    if ($aggr) {
	print "Using aggr $aggr on host $host\n";
    } else {
	print "Checking for good aggregate to use on $host.\n";
	open (AGGRINFO, "fts aggrinfo $host|");
	@smallest = ();
	while (<AGGRINFO>) {
	    ($aggr, $free, $size) =
		(/^LFS aggregate (\S+) .*:\s*(\d+)\s*K free out of total\s*(\d+)/);
	    if ($aggr && ($free > 1200)) {
		if (!@smallest || ($size < $smallest[1])) {
		    @smallest = ($aggr, $size, $free);
		}
	    }
	}
	close (AGGRINFO);
	($aggr = $smallest[0])
	    || die ("Couldn't find acceptable aggregate on $host");
	printf("Using %.3gM $aggr with %dK free\n",
	       $smallest[1]/1024.0, $smallest[2]);
    }
}

sub MakeSync
{
    local (*FILE) = @_;
    $F_SETFL = 3;
    $F_SETFL = 4;
    $O_SYNC = 0x10;
#    $flags = fcntl (FILE, $F_GETFL, 0);
#    print "$flags\n";
    fcntl (FILE, $F_SETFL, $O_SYNC) || die ("fcntl fails: $!");
}

sub GetBlockFrag
{
    local ($dir) = @_;
    local ($bsize, $fsize);
    local ($file, $buffer);
    local ($size, $oldSize);

    $file = "$dir/block-frag.file";
    $buffer = "x" x 1024;
    unlink ($file);
    open (TEST, ">$file") || &Die ("Couldn't open test file: $!");
    unlink ($file);
    &MakeSync(*TEST);
    $size = 0;
    do {
	syswrite (TEST, $buffer, 1024) || die ("Couldn't write test file: $!");
	@info = stat(TEST);
	$oldSize = $size;
	$size = $info[12]/2;		# convert 512 to units of Kilobytes
	($size > 0) || die ("Write didn't change size");
	$fsize || ($fsize = $size);
    } until (($size > $oldSize+$fsize) || ($size > 64));
    close TEST;
    $bsize = $size - $oldSize;
    ($bsize, $fsize);
}

$SIG{'INT'} = 'Cleanup';

$host = $ENV{HOST};

$FTbase = ($ARGV[1] || "frag-share.$host");

$freeBand = 1024;			# Run in last 1M of free space

$_ = `/opt/dcelocal/bin/epistat 1 10 0`;
if ($?) {
    warn ("Can't check buffer pool size");
} else {
    $bufferPoolSize = (split)[1]/1024;
    print "Episode buffer pool size is ${bufferPoolSize}K.\n";
    ($bufferPoolSize >= 1000)
	|| warn ("Buffer pool is too small.  Try 1Mb.");
}

&GetAggr();

# First make sure we can detach this sucker...
@dfsexportOutput = `dfsexport -detach $aggr 2>&1`;
($? == 0) || &Die ("Failed trying to detach $aggr:\n" .
		   &Prefix (DFS, @dfsexportOutput));
$rc = system ("dfsexport $aggr");
($rc == 0) || &Die ("Couldn't re-attach $aggr");

$fillFT = "$FTbase.fill";
$restoreFT = "$FTbase.restore";
$writeFT = "$FTbase.write";

&CreateFT ($fillFT);
system ("fts setquota -fileset $fillFT -size 999999")
    && &Die ("Couldn't set quota on $fillFT to 100M");

$fillMP = &Mount ($fillFT);

# Get block and frag size to make sure they are reasonable

($bsize, $fsize) = &GetBlockFrag ($fillMP);
print "Block/Frag organization is ${bsize}K/${fsize}K\n";
$volFragSize = 1;
$vmFragSize = 5;
print("Writing $volFragSize-frag files via volume restore and " .
       "$vmFragSize-frag files via VM path.\n");
(($bsize == 8) && ($fsize == 1))
    || warn ("Expecting to run on 8K/1K file system");

print "Filling $aggr with data...\n";
&Write ("$fillMP/FREE", $freeBand)
    || &Die ("Couldn't make $fillMP/FREE: $!");

$i = 0;
while (1) {
    &Write ("$fillMP/f$i", 128)
	|| last;
    $i++;
}
printf "Created $i files of totalling %.3gM.\n", $i*0.125;

# Now free up desired amount of space for the real test.

unlink ("$fillMP/FREE") || &Die ("Couldn't delete $fillMP/FREE file: $!");
&Unmount ($fillMP) || &Die ("Couldn't unmount $fillMP");

&CreateFT ($restoreFT);
$restoreId = $ftId{$restoreFT};
$mp = &Mount ($restoreFT);

# Put $volFragSize-fragment files into hald of it
for ($i=0; $i<($freeBand/2/$volFragSize); $i++) {
    &Write ("$mp/f$i", $volFragSize)
	|| &Die ("Failed to initialize restore fileset: $!");
}
&Unmount ($mp);

$scratchDump = "/tmp/frag-share.dumpfile";
$cleanup{"DumpFile"} = "unlink ('$scratchDump')";
$rc = system ("fts dump -fileset $restoreFT -time 0 -file $scratchDump");
($rc == 0) || &Die ("Couldn't dump $restoreFT into $scratchDump");

&CreateFT ($writeFT);
$mp = &Mount ($writeFT);

local ($file, $sig, @files, @sigs);	# files successfully written
local (@ftsOutput, $ftsExit);		# output from fts restore
local ($i, $f, $rc);

$| = 1;					# no buffering
$restoreWait = 20;			# this may be too long on some machines
$iteration = 0;
while (1) {
    print "Iter ";
    # Free space held by restore fileset.
    @ftsOutput = `fts delete $restoreFT $host $aggr 2>&1`;
    if ($?) {
	$ftsMsg = &Prefix(FTS, @ftsOutput);
	if ($ftsMsg =~ /FLDB: no such entry/) {
	    $restoreWait += 5;
	    print "\nRestore barely started so wait $restoreWait secs next time.\n ";
	} else {
	    &Die ("Couldn't delete $restoreFT:\n$ftsMsg");
	}
    }
    # Precreate and truncate all the files so that the volume table (for the VM
    # path, at least) doesn't consume blocks.  Make enough files to possibly
    # use all the free space.
    @files = ();
    for ($i=0; $i<$freeBand/$vmFragSize; $i++) {
	$file = "$mp/f$i";
	open (FILE, ">$file") || &Die ("Couldn't create $file: $!");
	close (FILE);
	$files[$i] = $file;
    }
    # Initialize all free space to well known pattern.
    &Fill ("$mp/FILL");

    $iteration++;
    print "#$iteration ";

    open (FTS, "fts restore -ftname $restoreFT -server $host -aggr $aggr -ftid $restoreId -file $scratchDump 2>&1 |")
	|| &Die ("Couldn't start restore: $!");
    sleep $restoreWait;				# let the restore get started

    # Put $vmFragSize-fragment files into write fileset.

    print "wrote ";
    @sigs = ();
    for ($i=0; $i<@files; $i++) {
	$file = $files[$i];
	$sig = &Write ($file, $vmFragSize, SYNC);
	$sig || last;
	# remember sig of each successfully written file
	$sigs[$i] = $sig;
    }
    $vmUsed = $vmFragSize*@sigs;
    print "${vmUsed}K ";
    if ($vmUsed < $freeBand/10) {
	$restoreWait /= 2;
	print "\nVM didn't start wait only $restoreWait secs for restore.\n ";
    }

    # wait for restore to finish
    @ftsOutput = <FTS>;
    close FTS;
    $ftsExit = $?;
    $ftsMsg = &Prefix(FTS, @ftsOutput);
    (!$ftsExit || ($ftsMsg =~ /No space left on device/) ||
     ($ftsMsg =~ /not enough space/))
	|| &Die ("fts restore got funny error:\n$ftsMsg");
    printf("while restore %s ",
	   ($ftsExit ? "got ENOSPC" : "succeeded"));

    # If VM path got more than half the space, wait longer for the restore to
    # get started before writing files.
    if ($vmUsed > $freeBand/2) {
	$restoreWait++;
	print "\nWill wait $restoreWait seconds for restore to start.\n ";
    }

    # Clear all caches
    print "clearing VM ";
    &Unmount ($mp);
    @dfsexportOutput = `dfsexport -detach $aggr 2>&1`;
    ($? == 0) || &Die ("Failed trying to detach $aggr:\n" .
		       &Prefix (DFS, @dfsexportOutput));
    $rc = system ("dfsexport $aggr");
    ($rc == 0) || &Die ("Couldn't attach $aggr");
    $mp = &Mount($writeFT);

    # Check for correctness
    print "check ";
    for ($i=0; $i<@files; $i++) {
	$f = $files[$i];
	$sig = $sigs[$i];
	last if (!$sig);
	&Read ($f, $vmFragSize, $sig) ||
	    # don't clean up!
	    die ("Read of $f failed, expected sig of $sig");
    }
    print "OK\n";
}

&cleanup();
exit (0);

# Fills a file with a random pattern up to a given size.  The return value is
# zero if any failure occurs, and a signature for the random patten upon
# success.  Optional third parameter, if true, tells write to sync on every
# write system call.

sub Write
{
    local ($file, $size, $sync) = @_;
    local ($signature) = int(rand(2000000000));
    local ($sig) = pack ("L", $signature);
    local ($buffer) = $sig x (256*$bsize);
    local ($i, $l, $n, $r);

    $bsize || die ("Must set bsize first");
    open (FILE, ">$file") || return 0;
    &MakeSync(*FILE) if ($sync);
    $l = $size;
    while ($l > 0) {
	$i = (($l < $bsize) ? $l : $bsize);
	$! = 0;
	$n = syswrite (FILE, $buffer, $i*1024);
	last if (($n != $i*1024) || $!);
	$l -= $i;
    }
    $r = $!;
    close (FILE);
    if ($r || $!) {
	unlink ($file);
	return 0;
    }
    $signature;
}

sub Read
{
    local ($file, $size, $sig) = @_;
    local ($nbytes, $bufs, $offset, $buffer);

    open (FILE, $file) || &Die ("Couldn't open $file: $!");
    $offset = 0;
    $bufs = 0;
check:
    while ($bufs < $size) {
	$nbytes = sysread (FILE, $buffer, 1024);
	if ($nbytes != 1024) {
	    print "Short read on $file: $!\n";
	    last check;
	}
	foreach $word (unpack("L256",$buffer)) {
	    if ($word != $sig) {
		print "Error in $file at $offset got $word expected $sig\n";
		last check;
	    }
	    $offset += 4;
	}
	$bufs++;
    }
    close FILE;
    $offset == 1024*$size;
}

sub Fill
{
    local ($file) = @_;
    local ($buffer) = "Z" x (1024*$bsize);
    local ($size, $n);

    open (FILE, ">$file") || return 0;
    $size = 0;
    while (1) {
	$! = 0;
	$n = syswrite (FILE, $buffer, length($buffer));
	last if (($n != $bsize*1024) || $!);
	$size += $n;
    }
    close (FILE);
    `sync`;
    unlink ($file);
}

sub Prefix
{
    local ($pre, @output) = @_;
    $pre = " $pre: ";
    $pre . join ($pre, @output);
}
