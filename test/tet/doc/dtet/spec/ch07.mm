'''	SCCS: @(#) DTET SPECIFICATION		Rel 1.2 (9/15/92)
.nr H1 6
.H 1 "Data File Formats"
.P
.H 2 "Input Files"
.P
.H 3 "Configuration Variables Files"
.P
The DTET allows for a separate configuration file to be provided on each of the
systems that are used in the test suite execution. The rules for the precedence
of configuration data are as described in section 4.1.1.
.P
On slave systems the configuration files for build, execute and clean modes are
always assumed to be located in the test suite root directory.
.P
An extra configuration file is consulted by the MTCC. This file is called
\s-1\f7tetdist.cfg\fP\s0 and is always located in the test suite root directory
of the master system. The configuration variables in this file specify the
TET_ROOT, TET_TSROOT and TET_EXECUTE directories for slave systems. Any
variables that are needed by the MTCC to connect to a slave system should also
appear in this file (See also sections 3.3 and 4.1.3).
.P
.SK
.H 3 "Scenario Files"
.P
The list of instructions that a scenario file data element may contain is
extended to include the following:
.P
.AL
.LI
Remote system designation.  The format of the \fBremote\fP
directive is \s-1\f7:remote,mmm,nnn,...:[filename]\fP\s0. The comma
separated list of remote system designations \fImmm,nnn,...\fP defines the
remote systems on which the test is executed. 
When the \s-1\f7filename\fP\s0 is omitted, the \fBremote\fP directive 
applies to all subsequent test definitions in the scenario file until an 
\fBendremote\fP directive is encountered. 
.P
The special designation \fI000\fP is used to refer to the master system. The
master system must always be included for any test case which expects to
automatically synchronise at the test purpose level. The master system may be
omitted when a series of separate tests are being executed across a set of
slave systems and synchronisation is only required at the end of this set
of test cases.
.LI
Test case repetition.  The \fBrepeat\fP directive is used to define 
test cases which are to be repeated.
The format of the \fBrepeat\fP directive is \s-1\f7:repeat,n:[filename]\fP\s0, 
where \fIn\fP is the number of times the test cases are to be repeated.
When the \s-1\f7filename\fP\s0 is omitted, the \fBrepeat\fP directive 
applies to all subsequent test definitions in the scenario file until an 
\fBendrepeat\fP directive is encountered. 
.LI
Concurrent test case processing.  The \fBparallel\fP directive is used to 
define test cases which are to be run in parallel. The format of the 
\fBparallel\fP directive is \s-1\f7:parallel:[filename]\fP\s0.
When the \s-1\f7filename\fP\s0 is omitted, the \fBparallel\fP directive 
applies to all subsequent test definitions in the scenario file until an 
\fBendparallel\fP directive is encountered. 
.LE
.P
When used the appropriate \fBend\fR directive must occur in the same file as
the matching \fBparallel\fP, \fBremote\fP or \fBrepeat\fP.
The validity of nesting these directives is also carefully defined.
The following table describes the legal combinations of these three directives.
.TS
center box tab(:);
cw(1.0i) | cw(1.0i) s s
cw(1.0i) | cw(1.0i) cw(1.0i) cw(1.0i).
\fIInner\fR:\fIOuter Directive\fR
\fIDirective\fR:\fBrepeat\fR:\fBparallel\fR:\fBremote\fR
_
\fBrepeat\fR:Valid:Error:Error
\fBparallel\fR:Valid:Error:Error
\fBremote\fR:Valid:Valid:Error
.TE
.P
.SP
.DS
Example 1
.SP
\s-1\f7    :parallel:
    /tests/block/tc
    /tests/rec/tc
    /tests/byte/tc
    :endparallel:\fP\s0
.DE
.P
This defines a set of three tests that are to be processed concurrently.
This could also be described using:
.P
.DS
\s-1\f7    :parallel:/tests/par_file\fP\s0
.DE
.P
where \s-1\f7tests/par_file\fP\s0 is the name of a file containing the 
details of the three test cases.
.P
.SP
.DS
Example 2
.SP
\s-1\f7    :remote,001,002:/tests/tc_file\fP\s0
.DE
.P
This specifies that the tests listed in the file \s-1\f7/tests/tc_file\fP\s0 
will be processed on the two remote designations \fI001\fP and \fI002\fP.
.P
.SP
.DS
Example 3
.SP
\s-1\f7    :remote,001,002:
    /tests/block/tc
    /tests/rec/tc
    /tests/byte/tc
    :endremote:\fP\s0
.DE
.P
Each test case in the list will be sequentially processed. The processing of
each test case will take place concurrently on the two systems specified.
.P
.SP
.DS
Example 4
.SP
\s-1\f7    :parallel:
    :remote,000,001:
    /tests/rec/tc
    :endremote:
    :remote,001,002:
    /tests/rec/tc
    :endremote:
    :endparallel:\fP\s0
.DE
.P
This defines the concurrent processing of these two separate tests with
each being processed on the designated remote machines. Note, however, the test
case \s-1\f7tests/rec/tc\fP\s0 is to be run in parallel with itself on system 
\fI001\fP.
For a build, execute and clean, say, of this scenario, the test case 
\s-1\f7/test/rec/tc\fP\s0 will only be built and cleaned once on system 
\fI001\fP. The implementation should also support the concurrent execution 
of the same test case. If TET_SAVE_FILES were set, the implementation
should separate those saved files resulting from each instance of the same test
case being executed concurrently.
.P
.H 4 "Future Directions"
.P
The functionality described in the TET specification is implemented in the DTET
using the \fBrepeat\fR and \fBparallel\fR directives.
.P
.SK
.H 3 "Results Code Definitions"
.P
These remain unchanged from those described for the TET. 
.P
Only the results code file on the master system is consulted.
.P
.SK
.H 3 "System Definitions"
.P
A single file is used to define the mapping of system identifier to host name 
in the DTET.
This file is located in the TET_ROOT directory on each system.
.P
In the systems definition file, variables are defined with lines of the form
\fIsysid  host\fR. The \fIhost\fR descriptor should be a value that can be 
used by the implementation to establish a connection with the appropriate 
system.
.P
Example:
.P
.DS
\s-1\f7    # Example system file for INET implementation on host 'ozone'

    000   ozone
    001   neon
    002   argon
    003   ozone\fP\s0
.DE
.P
.SK
.H 2 "Output Files"
.P
.H 3 "Journal and Results Files"
.P
For scenarios which do not use any of the new directives, namely, 
\fBparallel\fP, \fBremote\fP and \fBrepeat\fP, the journal files will appear 
in the same format as for the TET. However, because of the increased 
complexity of the journal files from these directives, more information must 
be provided in the journal file in order so that the \fBresume\fP and 
\fBrerun\fP options of the MTCC work correctly.
.P
In order to provide this functionality the following additional message types
are provided.
.P
.VL 6n 0n 1
.LI "Configuration Start (For Remote Systems)"
.P
\s-1\f720|remote_nnn Mode|text\fP\s0
.P
Where \s-1\f7remote_nnn\fP\s0 describes the remote system ID instead of the 
pathname of the configuration file.
.P
.LI "Parallel Start"
.P
\s-1\f7600||text\fP\s0
.P
The scenario file \fBparallel\fR directive start marker.
.P
.LI "Parallel End"
.P
\s-1\f7620||text\fP\s0
.P
The scenario file \fBparallel\fR directive end marker.
.P
.LI "Repeat Start"
.P
\s-1\f7700|n|text\fP\s0
.P
The scenario file \fBrepeat\fR directive start marker.
Where \s-1\f7n\fP\s0 is the number of times the test cases within the 
repeat directive are to be repeated.
.P
.LI "Repeat End"
.P
\s-1\f7720||text\fP\s0
.P
The scenario file \fBrepeat\fR directive end marker.
.P
.LI "Remote Start"
.P
\s-1\f7800|nnn,mmm,...|text\fP\s0
.P
The scenario file \fBremote\fR directive start marker.
Where nnn, mmm, etc. are the remote designations specified by the scenario file
\fBremote\fR directive.
.P
.LI "Remote End"
.P
\s-1\f7820||text\fP\s0
.P
The scenario file \fBremote\fR directive end marker.
.P
.LE
