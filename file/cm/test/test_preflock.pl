#!/afs/tr/proj/tools/bin/perl

#
# Copyright (C) 1995, 1994 Transarc Corporation - All rights reserverd
# 

# Global variables
$procs = 1;
$reps = 10;
$prefs_prefix = "/tmp/prefs.";
$prefs_file = "/tmp/prefs.$$";
$flprefs_prefix = "/tmp/flprefs.";
$flprefs_file = "/tmp/flprefs.$$";

if(@ARGV > 3 || @ARGV < 1) {
    print "Usage: $0 file_in_dfs [<number_of_processes>] [<number_of_reps>]\n";
    exit 1;
}


$tfile = shift(@ARGV);
if (! -e $tfile) {
    print "$0: $tfile does not exist\n";
    exit 1;
}	   
if (@ARGV >= 1) {
    $procs = shift(@ARGV);
    if ($procs > 30) {
	print "$0: Too many processes]\n";
	exit 1;
    }
}
if (@ARGV == 1) {
    $reps = shift(@ARGV);
}

#
# Determine system information
#
$myname=`hostname`;
chop($myname);

print "Running $reps repetitions in $procs processes on $myname\n";
#
# Get the fileset and server into the system, dump the 
# file server and fldb server preferences.
#
system("ls -l $tfile > /dev/null");
system("cm getpref -path $prefs_file");
system("cm getpref -path $flprefs_file -fldb");

#
# Open the prefs file, read in and get values to vary the preferences
#
open(PFILE,"$prefs_file") || die "cannot open file $prefs_file\n";
$i = 0;
$max_pref = 0;
$min_pref = 100000;
while(<PFILE>) {
    next if $_ =~ /^\s*$/;
    next if $_ =~ /^#/;
    next if $_ =~ /^;/;
    ($s, $p) = split(/\s+/, $_, 2);
    chop($p);
    $server[$i] = $s;
    $prefs[$i] = $p;
    $i++;
}
close(PFILE);
$count_pref = $i;

#
# Open the fldb prefs file, read in and get values to vary the preferences
#
open(PFILE,"$flprefs_file") || die "cannot open file $flprefs_file\n";
$j = 0;
$max_flpref = 0;
$min_flpref = 100000;
while(<PFILE>) {
    next if $_ =~ /^\s*$/;
    next if $_ =~ /^#/;
    next if $_ =~ /^;/;
    ($s, $p) = split(/\s+/, $_, 2);
    chop($p);
    $flserver[$j] = $s;
    $flprefs[$j] = $p;
    $j++;
}
close(PFILE);
$count_flpref = $j;


# Each process will now create its own prefs file with different values.
# They will then proceed with adjusting preferences to their own values.
# When done, they will remove their own files.

for($i=0; $i<$procs; $i++) {
    $pid = fork();
    if ($pid == 0) {
	&creat_ranks();
	&creat_flranks();
	&set_ranks();
	exit(0);
    } else {
	$pids[$i] = $pid;
    }
}
#
# Now start the readers who will attempt to access the servers being
# adjusted.  In this process (parent), just waits for the children to die.
#
$reader_pid = fork();
if ($reader_pid == 0) {
    &read_files($prefs_file);
    exit(0);
} else {
    for ($i=0; $i<$procs; $i++) {
	waitpid($pids[$i], 0);
    }
}


#
# Remove the prefs file.  This signals the reader to die.  Then wait for it.
#
print "All pref setters complete\n";
system("rm -f $prefs_file");
system("rm -f $flprefs_file");

waitpid($reader_pid, 0);
print "All childred complete\n";


#
# Routine to creat a shuffled preferences file for each child.
# 
sub creat_ranks{
    $myprefs = "/tmp/prefs.$$";
    open(MYPREFS, ">$myprefs") || die "cannot open child prefs file $myprefs";
    $shuffle = $$ % $count_pref;
    for ($i=0; $i<$count_pref; $i++) {
	$index = $i + $shuffle;
	$index = $index % $count_pref;
	print (MYPREFS "$server[$i]      $prefs[$index]\n");
    }
    close(MYPREFS);
}

#
# Routine to creat a shuffled fldb preferences file for each child.
# 
sub creat_flranks{
    $myflprefs = "/tmp/flprefs.$$";
    open(MYFL, ">$myflprefs") || die "cannot open child prefs file $myflprefs";
    $shuffle = $$ % $count_flpref;
    for ($i=0; $i<$count_flpref; $i++) {
	$index = $i + $shuffle;
	$index = $index % $count_flpref;
	print (MYFL "$flserver[$i]      $flprefs[$index]\n");
    }
    close(MYFL);
}


#
# Routine to set preferences.
#
sub set_ranks{
    $mypref_file = "$prefs_prefix$$";
    $myflpref_file = "$flprefs_prefix$$";
    for($j=0; $j<$reps; $j++) {
	if (! -e $mypref_file) {
	    die "cm setp file gone!";
	} else {
	    system("cm setp -path $mypref_file");
	}
	if (! -e $myflpref_file) {
	    die "cm setp -fldb file gone!";
	} else {
	    system("cm setp -path $mypref_file -fldb");
	}
	sleep(int(rand(10)));
    }
    system("rm -f $mypref_file");
    system("rm -f $myflpref_file");

}

#
# Routine to read a file in DFS causing server access.
#
sub read_files{
    local($signal_file) = @_;
    while (-e $signal_file) {
	open(TFILE,"$tfile") || die "cannot open file $prefs_file\n";
	read(TFILE,$garbage,1,0);
	close(TFILE);
#	system("cat $tfile > /dev/null");
#	system("touch $tfile");
	system("cm flush -path $tfile");
	system("cm checkfilesets > /dev/null");
    }
}
