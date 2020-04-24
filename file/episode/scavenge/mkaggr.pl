#!/afs/tr/fs/dev/perl -w
#
# Copyright (C) 1993 Transarc Corporation - All rights reserved
#
# Generate test aggregate for the salt salvager test.
#
# Usage: mkaggr <aggregate> <mount_point>
#
# You must run as superuser in order for these operations to succeed.
# Additionally, your path should be set up so that the various commands
# (efts, newaggr, epimount, epiunmount, dd, mknod) will be found.
# 
die "usage: mkaggr <aggregate> <mount_point>\n" unless ($#ARGV == 1);

$aggr = shift(@ARGV);
$mountpt = shift(@ARGV);

$backfs = "abelard";
$cowfs = "heloise";

$K = 1024;
$M = $K * $K;

$fs = $K;
$bs = 8 * $K;

die "aggregate $aggr doesn't exist" unless (-e $aggr);
die "aggregate $aggr is not writable" unless (-w $aggr);
die "mount point $mountpt doesn't exist" unless (-d $mountpt);
#
# First fill the space with zeroes.
#
system "dd if=/dev/zero of=$aggr bs=8192" if (-r '/dev/zero');

#
# Now make a fresh aggregate.
#
&docmd("newaggr -aggregate $aggr -blocksize $bs -fragsize $fs -overwrite",
    "Can't newaggr $aggr: $!\n");

#
# Attach it, create the first fileset, and then detach.
#
&docmd("efts attach $aggr", "Can't attach $aggr: $!\n");
&docmd("efts createft -aggregate $aggr -name $cowfs",
    "Can't create fileset $cowfs: $!\n");
&docmd("efts detach $aggr", "Can't detach $aggr: $!\n");

#
# Mount the first fileset and initialize it.
#
&docmd("epimount $aggr $cowfs $mountpt",
    "Can't epimount fileset $cowfs on $mountpt: $!\n");

&create_dir("$mountpt/files");
&create_dir("$mountpt/devices");
&create_dir("$mountpt/bigdir");

#
# Populate the files directory, which contains the ordinary files
# that we will play with.
#
&create_file("$mountpt/files/zero", 0);
&create_file("$mountpt/files/inline", 25);
&create_file("$mountpt/files/frags", 509);
&create_file("$mountpt/files/frags.2", $fs + 1);
&create_file("$mountpt/files/dblocks", 6 * $bs);
&create_file("$mountpt/files/iblocks", 10 * $bs);
&create_sparse_file("$mountpt/files/sparse", ($K - 1) * $M, 20);

#
# Populate the devices directory, which contains the special files
# that we will play with.
#
&docmd("mknod $mountpt/devices/black_hole c 2 2",
    "Can't create char device $mountpt/devices/black_hole: $!\n");
&docmd("mknod $mountpt/devices/pied_piper p",
    "Can't create fifo $mountpt/devices/pied_piper: $!\n");
symlink('black_hole', "$mountpt/devices/piltdown") ||
die "Can't create symlink $mountpt/devices/piltdown: $!\n";

#
# Populate the bigdir directory, which we use for directory testing.
# It contains a fairly large number of empty files whose names are chosen
# to guarantee that all hash chains contain at least two entries.
#
for $i (0 .. 5) {
    for $l (ord('a') .. ord('z')) {
	$path = sprintf("$mountpt/bigdir/%c%d", $l, $i);
	&create_file($path, 0);
	$path = sprintf("$mountpt/bigdir/%%%c%d", $l - 1, $i);
	&create_file($path, 0);
    }
}

#
# Now unmount the fileset and clone the second copy.
#
&docmd("epiunmount $mountpt", "Can't unmount $mountpt: $!\n");
&docmd("efts attach $aggr", "Can't attach $aggr: $!\n");
&docmd("efts clone -aggregate $aggr -srcname $cowfs -destname $backfs",
    "Can't create clone $backfs of $cowfs on $aggr: $!\n");
&docmd("efts detach $aggr", "Can't detach $aggr: $!\n");

#
# That's all:
#
print "Test aggregate $aggr successfully created.\n";
exit(0);

sub docmd {
    local($cmd, $errmsg) = @_;
    local($xstatus) = system $cmd;
    die $errmsg unless (($xstatus >> 8) == 0);
}

sub create_dir {
    local($dname) = @_;
    mkdir($dname, 0666) || die "Can't create $dname: $!\n";
}

sub create_file {
    local($path, $len) = @_;
    open(file, ">$path") || die "Can't create $path: $!\n";
    if ($len > 0) {
	local($buf) = 'x' . $len;
	syswrite(file, $buf, $len) || die "Can't write to $path: $!\n";
    }
    close(file) || die "Can't close $path: $!\n";
}

sub create_sparse_file {
    local($path, $offset, $len) = @_;
    open(file, ">$path") || die "Can't create $path: $!\n";
    seek(file, $offset, 0) || die("Can't seek on $path: $!");
    if ($len > 0) {
	local($buf) = 'x' . $len;
	syswrite(file, $buf, $len) || die "Can't write to $path: $!\n";
    }
    close(file) || die "Can't close $path: $!\n";
}
