#!/afs/tr/proj/tools/bin/perl
# 

#Globals
$PROGRAMNAME=$0;
if ($ENV{DEBUG}) { $DEBUG = 1 };
if ($ENV{VERBOSE}) { $VERBOSE = 1 };

require 'error.pl';
require 'fsutil.pl';
require 'util.pl';
require 'loads.pl';
require 'parse.pl';
require 'setup.pl';
require 'query.pl';

#more Globals
$DFSTABFILE="/opt/dcelocal/var/dfs/dfstab";

#other globals setup by CheckUsage -
# $CONFIG_FILE - name of configuration file
# $PHASE - functionality mode
# $HARDFAILURE - If validation of one load fails, exit. Default is to 
# run as many of the specified loads as possible
# @LOADLIST - If specified, serves to restrict the loads to be run.

&CheckUsage;

&ProcessLogDir;

# 1 - means log on console too
&SetLogFile("${LOGDIR}/main",1);

# Parse config file
&InitParser;

# ProcessSetup phase
&Log("SETUP PHASE STARTING");
&ParseAndProcessSetup;

# Process loads
if ("$PHASE" ne "setuponly") {
	&Log("LOAD PROCESSING PHASE STARTING");
	&ParseAndProcessLoads ;
}

&FinishUp;

#############################################################################
#
# Subs
#
#############################################################################

sub ProcessLogDir {
	local($date,$logId,$oldDate,$oldId,$id);

	if (!defined($LOGDIR)) {
		$LOGDIR="/tmp/stresstestlogs";
	}

	$logIdFile="${LOGDIR}/.logid";
	@ltime = localtime(time);
	$date = sprintf("%02d%02d", $ltime[4] + 1, $ltime[3]);

 	$id="aa";
	if ( ! -e $LOGDIR ) {
		&MakeDir($LOGDIR,0777);
        #create magic file
		if ( open(LFH, ">$logIdFile") ) {
			$logId = "$date.$id";
			print LFH "$logId\n";
			close(LFH);
		} else {
			&Exit("Cannot create needed files in dir $LOGDIR ($!)", 
				  __FILE__, __LINE__);
		}
	} else {
		if ( ! -d $LOGDIR ) {
			&Exit("Specified log directory, $LOGDIR, is not a directory", 
				  __FILE__, __LINE__);
		} else {
			if ( -e $logIdFile ) {
				open(LFH, "<$logIdFile") ||
					&Exit("Cannot read logid file $logIdFile ($!)",
						  __FILE__, __LINE__);
				chop($logId = <LFH>);
				($oldDate, $oldId) = split(/\./, $logId);
				$id = ++$oldId if $date eq $oldDate;
				close(LFH);
				$logId = "$date.$id";
				open(LFH,">$logIdFile") ||
					&Exit("Cannot write logid file $logIdFile ($!)",
						  __FILE__, __LINE__);
				print LFH "$logId\n";
				close(LFH);
			} else {
				open(LFH, ">$logIdFile") ||
					&Exit("Cannot create logid file $logIdFile ($!)", 
						  __FILE__, __LINE__);
				$logId = "$date.$id";
				print LFH "$logId\n";
				close(LFH);
			}
		}
	}
	$LOGDIR="${LOGDIR}/log.${logId}";
	&MakeDir($LOGDIR);
	# In case umask prevented 777 perm on dir change it.
	&Chmod($LOGDIR,0777);
	&Log("Detailed logs appear in log file ${LOGDIR}/main");

    # Also save filehandle to console.
    open (CONSOLE, ">&STDOUT");
}

# CheckUsage 
# Apart from checking usage of driver, it sets $CONFIG_FILE, $PHASE,
# $HARDFAILURE, @LOADLIST

sub CheckUsage {
	local($usageMsg)="USAGE: $PROGRAMNAME <config-file> [ -setuponly ] [ -loads <load1>,<load2>,... ] [ -hardfailure ] [ -logdir <log dir>] [ -consolelogonly ]";
	local($token);

	$PHASE="";
	@_=@ARGV;
    if ( @_ < 1 ) {
		&Exit("Wrong number of arguments \n $usageMsg" ,__FILE__,__LINE__,
			  $EXIT_USAGE_ERROR);
    }
	$CONFIG_FILE=shift;
	if ( ! -r $CONFIG_FILE ) {
		&Exit("Filesystem setup config file $CONFIG_FILE unreadable",
			  __FILE__,__LINE__,$EXIT_CONFIG_FILE_ERROR);
	} 
	while ($token = shift) {
		if ( $token eq "-setuponly") {
			$PHASE="setuponly";
		} elsif ( $token eq "-hardfailure" ) {
			$HARDFAILURE=1;
		} elsif ( $token eq "-loads" ) {
			while ($token = shift) {
				if ( ! /^-/ ) {
					if (/^[^a-zA-Z]/) {
						&Exit("Invalid load ($token) specified",
							  __FILE__, __LINE__, &EXIT_USAGE_ERROR);
					}
					push(@LOADLIST,$token);				
				} else {
					unshift(@_,$token);
					last;
				}
			}
			if ( ! @LOADLIST ) {
				&Exit("Loads not specified with -load switch",
					  __FILE__,__LINE__,$EXIT_USAGE_ERROR);
			}
		} elsif ( $token eq "-logdir" ) {
			$LOGDIR=shift;
			if ( !defined($LOGDIR) ) {
				&Exit("Log directory not specified with -logdir switch",
					  __FILE__, __LINE__, &EXIT_USAGE_ERROR);
			}
		} elsif ( $token eq "-consolelogonly" ) {
			$LOGONCONSOLEONLY=1;
		} else {
			&Exit("Unknown option ($token) specified \n $usageMsg",
				  __FILE__, __LINE__, $EXIT_USAGE_ERROR);
		}
	}
}

# Emacs Variable settings.
# Local Variables:
# tab-width:4
# End:

 
