#!/afs/tr/proj/tools/bin/perl

# /* Copyright (C) 1995 Transarc Corporation - All rights reserved. */
# $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/sautils/RCS/epistle.pl,v 1.2 1995/04/19 19:17:09 ota Exp $

# Epistle: A Lexical front end for epistat.

$epistat = ($ENV{'EPISTAT'} || 'epistat');

%opcodes = (get, 1, set, 2, getnext, 4);

$epistle = ($0 =~ /epistle/);		# look up single value
$epistle = 0 if (grep(/^-all/,@ARGV) > 0);

# else epidumpstats and report all available info

sub Usage
{
    $usage =
	($epistle
	 ? "Usage: $0 get|set|getnext key[.key[.key...]] [value/instance]\n"
	 : "Usage: $0");
    die ($usage);
}

while (<DATA>) {
    ($name, $code) = split;
    $name = "\L$name";
    push(@keys, $name);
    $code{$name} = $code; 
    $name{$code} = $name;
}

if ($epistle) {
    &Usage if ! ($opcode = $opcodes{shift});
    &Usage if ! ($key = shift);
    $value = shift;
    &Epistle;
} else {
    &EpiDumpStats;
}
exit 0;

sub Epistle
{
    $key = "\L$key";
    @list = grep(/^$key/, @keys);
    if (@list == 1) {
	($code = $code{$list[0]}) =~ tr/\./ /;
        exec "$epistat $opcode $code $value";
	die("Error executing epistat ($!)\n");
    }
    if (@list == 0) {
	print("No matches on $key. Possible keys are:\n");
	@list = @keys;
    } else {
	print("$key is not unique. Choose one of:\n");
    }
    for (@list) {print("	$_\n");}
    exit (@list != 1);
}

sub EpiDumpStats
{
    @stats = `$epistat -all`;
    if ($?) {
	print STDERR ("epistat failed:\n", @stats);
	die ("no epistat");
    }
    foreach (@stats) {
	@s = split;
	($instance) = ($s[2] =~ /\[(\d+)\]/);
	if ($s[$instance ? 3 : 2] ne "=") {
	    warn ("Unexpected input line: '$_'");
	    next;
	}

	$code = "$s[0].$s[1]";
	$name = $name{$code};
	if (!$name) {
#	    warn ("Unknown code: $code");
	    next;
	}
	($module, $field) = split('\.', $name);
	$value = $s[$instance ? 4 : 3];
	if ($instance) {
	    eval "\$$field[$instance] = $value";
	} else {
	    $value{$name} = $value;
	}
    }

    $dirCalls = $value{'dirbuf.calls'};
    $dirMisses = $value{'dirbuf.ios'};
    printf("Dirbufs: %d, %d refs, %d misses (%.3g%% misses)\n",
	   $value{'dirbuf.nbuffers'}, $dirCalls, $dirMisses,
	   ($dirCalls ? $dirMisses*100.0/$dirCalls : 0.0));
    $bcKb = $value{'vnops.bufferpoolsize'}/1024;
    print "Buffer pool contains $bcKb Kbytes\n";
    for ($bsize=1; $bsize<=64; $bsize<<=1) {
	next if (!$allocs[$bsize]);
	print "In ${bsize}K pool: $nbufs[$bsize] buffers\n";

	printf("  With %d refs: %d misses, lately missing %.3g%%\n",
	       $hits[$bsize]+$misses[$bsize], $misses[$bsize],
	       $missratio[$bsize]/256.0);
	printf("  Writes %d (%d log forces)",
	       $writes[$bsize], $forces[$bsize]);
	printf(", allocs %d", $allocs[$bsize]);
	printf(" (cost %.2g)", $alloccost[$bsize]/256.0)
	    if ($alloccost[$bsize]);
	printf(" (createblocks %d) (cleans %d)\n",
	       $createblocks[$bsize], $allocwrites[$bsize]);
	printf("  Avg age at reuse: %.1f, recycle: %.1f\n",
	       $meanreuseage[$bsize]/256.0, $meanrecycleage[$bsize]/256.0)
    }
}

# Any strings can be added here. The naming hierarchy need not correspond
# to the official MIB hierarchy. So, for example, log.ecthrottlecounter
# could also be defined as:
# thottle.log.ec 8.4
__END__
log.passno 8.1
log.activesize 8.2
log.lsn 8.3
log.ecthrottlecounter 8.4
log.lfthrottlecounter 8.5
log.fsynccalled 8.6
buffer.nbufs 9.1
buffer.createblocks 9.2
buffer.allocwrites 9.3 
buffer.meanrecycleage 9.4
buffer.meanreuseage 9.5
buffer.meanpoolsize 9.6
buffer.allocs 9.7
buffer.hits 9.8
buffer.misses 9.9
buffer.inuse 9.10
buffer.missratio 9.11
buffer.morespacedesire 9.12
buffer.nextmore 9.13
buffer.maxtrans 9.14
buffer.alloccost 9.15
buffer.writes 9.16
buffer.forces 9.17
vnops.bufferPoolSize 10.0
vnops.anodePoolSize 10.1
vnops.dirBufferPoolSize 10.2
vnops.vnodePoolSize 10.3
vnops.flockPoolSize 10.4
vnops.isExportable 10.5
vnops.allowSuserDirLinkOps 10.6
dirbuf.nbuffers 11.0
dirbuf.calls 11.1
dirbuf.ios 11.2
dirbuf.waiters 11.3
dirbuf.hashSize 11.4 
logtune.maxlogs 14.0
logtune.fullsize 14.1
logtune.maxtran 14.2
buffertune.debuglevel 15.0
buffertune.decayrate1 15.1
trace.episode.anode 16.0
trace.episode.async 16.1
trace.episode.logSleepCnt 16.2
trace.episode.logbuf 16.3
trace.episode.recover 16.4
trace.episode.vnops 16.5
trace.osi 16.6
