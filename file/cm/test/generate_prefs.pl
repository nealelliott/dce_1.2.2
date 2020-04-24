#!/afs/tr/proj/tools/bin/perl 

# HISTORY
# $Log$
# $EndLog$
#
# Copyright (C) 1994 Transarc Corporation - All rights reserverd
# 

# Global variables
$hosts="/etc/hosts";
$outfile="prefs_database.txt";

if($#ARGV > 1) {
    print "Usage: $0 <outFileName>\n";
    exit 1;
}

if ($#ARGV == 1) {
    $outfile = shift(@ARGV);
}

#
# Determine system information
#
$systype=`uname`;
$myname=`hostname`;
chop($myname);

($name, $aliases, $addrtype, $length, @addrs) = gethostbyname($myname);
(@myaddr) = unpack('C4', $addrs[0]);
$me = sprintf("%d.%d.%d.%d",$myaddr[0],$myaddr[1],$myaddr[2],$myaddr[3]);
@netlines = `netstat -in`;
foreach $line (@netlines) {
    chop($line);
    ($interface, $mtu, $net, $address, $rest) = split(/[ \t\n]+/, $line, 5);
    if ($address eq $me) {
	$myinterface = $interface;
    }
}

$ifconfig = `ifconfig $myinterface | grep netmask`;
($a,$b,$c,$d,$subnetmask,$e,$broadcast) = split(/[ \t\n]+/, $ifconfig, 7);
#
# On some machines ifconfig gives a hex subnet mask without the leading 0x
# detect this and correct the format of the subnet mask.
#
if ($subnetmask !~ 0x) {
    $subnetmask = sprintf("0x%s",$subnetmask);
}
$subnetmask = hex($subnetmask);
$mysubnetmask[0] = ($subnetmask&0xff000000) >> 24;
$mysubnetmask[1] = ($subnetmask&0x00ff0000) >> 16;
$mysubnetmask[2] = ($subnetmask&0x0000ff00) >> 8;
$mysubnetmask[3] = $subnetmask&0x000000ff;
#
# Now compute my network mask based on my address top byte
#
if ( $myaddr[0] < 128 ) {	# Class A network?
    @mynetmask = (255,0,0,0);
} elsif ( $myaddr[0] < 192) {	# Class B network?
    @mynetmask = (255,255,0,0);
} elsif ( $myaddr[0] < 224) {	# Class C network?
    @mynetmask = (255,255,255,0);
} else {			# Other
    @mynetmask = (0,0,0,0);
}
#
# Compute my network and my subnetwork from the masks.
#
@mynetwork = ($myaddr[0] & $mynetmask[0],
	      $myaddr[1] & $mynetmask[1],
	      $myaddr[2] & $mynetmask[2],
	      $myaddr[3] & $mynetmask[3],
	      );
@mysubnet = ($myaddr[0] & $mysubnetmask[0],
	     $myaddr[1] & $mysubnetmask[1],
	     $myaddr[2] & $mysubnetmask[2],
	     $myaddr[3] & $mysubnetmask[3],
	     );


print("I am $name at $myaddr[0].$myaddr[1].$myaddr[2].$myaddr[3] ");
print("on $myinterface \n");
#print("netmask    $mynetmask[0].$mynetmask[1].$mynetmask[2].$mynetmask[3] ");
#print("network    $mynetwork[0].$mynetwork[1].$mynetwork[2].$mynetwork[3]\n");
#print("subnetmask $mysubnetmask[0].$mysubnetmask[1].$mysubnetmask[2].$mysubnetmask[3] ");
#print("subnet     $mysubnet[0].$mysubnet[1].$mysubnet[2].$mysubnet[3]\n");


open(INFILE,"$hosts") || die "cannot open file $hosts\n";
open(OUTFILE,">$outfile") || die "cannot open file $outfile\n";
while(<INFILE>) {
    next if $_ =~ /^\s*$/;
    next if $_ =~ /^#/;
    next if $_ =~ /^;/;
    ($a, $n, @r) = split(/\s+/, $_, 3);
    ($name, $aliases, $addrtype, $length, @addrs) = gethostbyname($n);
    (@addr) = unpack('C4', $addrs[0]);
    $thisrank = &compute_rank(@addr);
    $thistype = int(rand(3)+1);
    print(OUTFILE "$name $thisrank $thistype\n");
}
close(OUTFILE);
close(INFILE);

#
# Routine to compute the rank for the current host entry being examined
# uses variables $mynetwork $mynetmask, $mysubnet, $mysubnetmask, $myhexaddr 
# takes parameter address to test.
#
sub compute_rank{
    local(@addr) = @_;
    if ($myaddr[0] == $addr[0] &&
	$myaddr[1] == $addr[1] &&
	$myaddr[2] == $addr[2] &&
	$myaddr[3] == $addr[3] ) {
	return(5000);
    } elsif (($addr[0] & $mysubnetmask[0]) == $mysubnet[0] &&
	     ($addr[1] & $mysubnetmask[1]) == $mysubnet[1] &&
	     ($addr[2] & $mysubnetmask[2]) == $mysubnet[2] &&
	     ($addr[3] & $mysubnetmask[3]) == $mysubnet[3] ) {
	return(20000);
    } elsif (($addr[0] & $mynetmask[0]) == $mynetwork[0] &&
	     ($addr[1] & $mynetmask[1]) == $mynetwork[1] &&
	     ($addr[2] & $mynetmask[2]) == $mynetwork[2] &&
	     ($addr[3] & $mynetmask[3]) == $mynetwork[3] ) {
	return(30000);
    } else {
	return(40000);
    }
}
