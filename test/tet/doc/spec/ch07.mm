'\"	SCCS: @(#) TET SPECIFICATION		Rel 4.5 (06/23/92)
'\"
.nr H1 6
.nr Cl 3
.nr Ej 1
.H 1 "Data File Formats"
This section defines the format of Toolkit portable data files. These
data files (provided by the Toolkit, test suite authors, and Toolkit users)
are portable to all implementations of the Toolkit. Note that these
formats may be extended by conforming implementations.
.SK
.H 2 "Input Files"
All Toolkit input files use a similar set of construction rules:
.BL
.LI
Data elements start in column 1 and are terminated with an
.I end-of-line
character.
.LI
Comments start with a
.Cw #
in column 1, and later terminated with an
.I end-of-line
character.
.LI
Blank lines (lines consisting only of 
.I blanks
and the
.I end-of-line
character) are ignored.
.LE
.Sk
.H 3 "Configuration Variables Files"
There is a single configuration file per executing test
scenario per execution mode. This configuration file is found, by
default, in the root directory of the appropriate test suite. By
default this file is called 
.Fi tetbuild.cfg , 
.Fi tetexec.cfg ,
or
.Fi tetclean.cfg 
for each of build, execute, and clean modes
respectively. The names and locations of these files can be changed
through options to the TCC. See the Test Case
Controller Interface Specification (\(sc5.1) for further details.
.P
In a configuration variable file, variables are defined with lines
of the form 
.Ar name=value .
The
.Ar name
may consist of any sequence of alphanumeric characters and
the underscore character which does not start with a numeric 
character. The
.Ar value
may consist of any sequence of characters.
.P
Note that names starting with the sequence "TET_" are reserved for
implementations of this specification.
.P
Example:
.P
.in +.5i
.Cw "# Example configuration variables settings."
.br
.Cw "TET_BUILD_TOOL=make -f"
.br
.Cw "TET_BUILD_FILE=Makefile"
.in -.5i
.SK
.H 3 "Scenario Files"
Scenario files assign user visible names to one of more invocable
components. The default scenario file is called
.Fi tet_scen
and is
located in the root directory of each test suite. This file must
at least define a scenario named
.BR all ,
which should cause execution of every invocable component in the
suite. Test cases and "include files" are referenced in scenarios via
pathnames.  In build and clean modes, and in execute mode
when there is no alternate execution directory, test case pathnames are
relative to the test suite root directory, and may only reference files
within the test suite hierarchy. In execute mode when there is an
alternate execution directory, test case pathnames are relative to that
directory and may only reference files within the alternate execution
directory hierarchy.  Include file pathnames are always relative to
the test suite root directory.
.P
The scenario file is made up of multiple data elements. Each data 
element
consists of a name and one or more instructions.
Scenario file data elements may continue beyond the end of a line.
Continuation of a data element is indicated by beginning subsequent
lines beyond column 1.  Note that comments (indicated by a
.Cw #
in column 1) do not indicate the end of a data element.
.P
The name is built of alphanumeric characters, and may be up to
31 characters in length.
.P
A scenario file data element may contain any of the following
instructions:
.AL
.LI
Invocable component execution. These specify the test cases and 
invocable
components to be executed. Multiple invocable components within a
single test case may be specified in a single instruction. Invocable
component names consist of a leading slash, the pathname of a test
case (see above), and optional comma-separated
invocable component numbers within braces ({,}) (e.g.
.Cw /tests/readtests/test.exe{1,2,3} ).
.LI
Journal message output. This instruction causes a message to be 
placed
into the journal with the type 
.Cw "scenario information" .
These lines
could be used by a specific report treatment filter to establish 
check points
in a report. 
Journal messages
must be surrounded by quotation marks (") and may not contain
quotation marks (e.g. 
.\" The "" is an escaped form of " within a macro argument
.Cw """This is a Journal Message""").
.LI
Invocable component inclusion. Instructions of this type reference a 
set of invocable
components in another file. The included invocable components are
executed synchronously. This facility can be used to reduce the
complexity of scenario files. Include directives consist of the
colon-delimited term
.B include
and the pathname to include (e.g. 
.Cw ":include:/tests/readtests/testlist" ).
.LE
.P
Files referenced by include directives consist of entries, each of which
is either a journal message (see above) or the pathname of a test case
with optional invocable components (e.g. 
.Cw "/tests/readtests/test1{1,3}" ).
.P
Example:
.in +.5i
.Cw "# Example scenario file"
.br
.Cw "readtest /tests/readtests/test1{1,3}"
.br
.Cw "writetest /tests/writetests/test1{1,2,3}"
.br
.Cw "all /tests/readtests/test1"
.br
.Cw "    ""Read tests completed"""
.br
.Cw "    /tests/writetests/test1"
.br
.Cw "    ""Write tests completed"""
.in -.5i
.H 4 "Future Directions"
The following scenario file features are proposed future additions:
.AL
.LI
Invocable component iteration. These indicate that a set of
invocable components (defined in the referenced file) are to be
executed synchronously, but repeatedly. Iteration directives consist
of colon-delimited terms
.B repeat,#
and the pathname to include, where # is the number of times to 
repeat
the included ICs (e.g. 
.Cw ":repeat,10:/tests/readtests/testlist" ).
.LI
Invocable component simultaneous execution. These indicate that
a set of invocable components (defined in the referenced file) are to
be executed asynchronously. They consist of a colon-delimited term
.B group[,#]
and the pathname of ICs to group, where # is an optional repeat 
count
(see iteration, above) (e.g. 
.Cw ":group,10:/tests/readtests/testlist" ).
.LE
.SK
.H 3 "Results Code Definitions"
The Toolkit and test suites both supply result code definitions. 
Test suite supplied result code definitions
are stored in files called
.Fi tet_code
in the test suite
root directories.
.I "[Requirement 5.2]"
These files are made up of lines which have
single elements. These elements contain up to three blank-separated
fields, defined as follows:
.AL 1
.LI
The result code. This is a non-negative decimal integer between 0 
and
127, inclusive.
Result codes from 0 to 31 (inclusive) are reserved for use by the TET
implementation.  The remainder are available for specification by 
the
test suite author.
.LI
The name of this result. This is a double quote-delimited field which
can contain any sequence of alphanumeric characters and blanks.  
.LI
The action to take when this result is encountered. This is an
indication of what the TCC and TCM should do when the result is 
returned by a
test purpose. Legal values are 
.Cw Continue 
and 
.Cw Abort . 
The default
value for this field is 
.Cw Continue .
.LE
.P
The following is an example result codes definition file. It
contains those result codes provided by TET.
.in +.5i
.ft 7
.S -1
.TS
l l l.
0	"PASS"	Continue
1	"FAIL"	Continue
2	"UNRESOLVED"	Continue
3	"NOTINUSE"	Continue
4	"UNSUPPORTED"	Continue
5	"UNTESTED"	Continue
6	"UNINITIATED"	Continue
7	"NORESULT"	Continue
.TE
.S +1
.R
.in -.5i
.R
.SK
.H 2 "Output Files"
.H 3 "Journal and Result Files"
Result files are written by test cases, buildtools and cleantools when
run with output capture mode disabled. These
result files are then transferred into the journal file by the TCC.  
The
format of lines in these files is identical.  
The total length of a journal line must not
exceed 512 bytes.
Each journal line is made up of a message type, the parameters for
that message, and a message area (the format of which is 
unconstrained).  Each message may have zero or 
more 
parameters associated
with it.  These parameters (strings or integers represented by no 
more than
ten decimal digits) are blank-separated and are contained between 
vertical bars.
Possible parameters include:
.AL 1
.LI
The TCC activity number (Activity). This number is incremented once 
for
each activity performed by the TCC (build, execute and clean-up are
each considered individual activities).
.LI
The invocable component number (ICnumber).
.LI
The test purpose number (TPnumber).  This number uniquely 
identifies
the test purpose within a test case.
.LI
The test purpose count (TPcount).  This is the number of test 
purposes 
that make up an invocable component.
.LI
The message context (Context). This field represents the process 
that generated
the journal line. Its make up is implementation defined. 
Implementations are
required to ensure that there are no repeated process identifiers
during the life of a test purpose.
.LI
The message block number (Block).  This number is set to one at the 
start of
each test purpose or new context, and is incremented once when the
test purpose requests it.  This number, along with the process identifier
and message number (below) is used by the TCC to order the data in an
execution result file prior to transferring that file into the
journal.
.LI
The message sequence number (Sequence). This number is set to one 
at the start of
each block, and is incremented once for each message written 
to
the result file.
.LI
The current time (Time).  Times are given using the notation 
HH:MM:SS with a
24 hour clock.
.LI
The current date (Date).  Dates are given using the notation 
YYYYMMDD, e.g. 19910610 for 10th June 1991.
.LI
A test case name (TestCase).  This is the test case name as given in
the scenario file.
.LI
A pathname (Pathname).  The full pathname of a file.
.LI
A mode (Mode).  Legal modes are Build (0), Execute (1), and Clean-up 
(2).
.LI
A completion status (Status).  This is the value returned to the TCC 
by the
operating system after execution of another process.  Values less 
than
zero are reserved by TET.
.LI
An invocable component count (ICcount).  This is the number of
invocable components executed in each test case (expected or 
actual
as specified).
.LE
.P
In the list below each message type is listed, followed by a line
indicating the number associated with that message and the required
parameters, followed by a textual description of the message and its
uses.
.VL .5i
.LI "Test Case Controller Start"
.P
.Cw "0|Version Time Date|who"
.P
This message is generated by the TCC at the beginning of each
TCC execution. The parameters include the version of the TCC used,
the time at which TCC execution started, and the current date.
The message area contains information about who ran the test.
.LI "Test Case Start"
.P
.Cw "10|Activity TestCase Time|IC list"
.P
A test case start message is generated by the TCC for each test case
executed in a scenario. The parameters are the sequence number of 
this
TCC activity, the pathname of the test case, and the time the test 
was
started.  If the corresponding scenario line contains a list of invocable
components to be executed, then that IC list is included in the text area
of the message.
.LI "Test Case Manager Start"
.P
.Cw "15|Activity Version ICcount|"
.P
The TCM writes this message during its initialisation.
The parameters include the sequence number of this TCC activity, 
the
version of the TCM being used, and the number of invocable 
components
to be executed.
The number of invocable components should be used by report 
treatment
filters which audit test results to ensure that all ICs were correctly
executed.
.LI "Configuration Start"
.P
.Cw "20|Pathname Mode|"
.P
Configuration start messages are placed in the journal by the TCC at 
test
session startup.  Parameters are the pathname of the
configuration file being referenced, and the mode to which this
configuration information applies.
.LI "Configuration Variable Setting"
.P
.Cw "30||variable=value"
.P
A configuration variable setting line is written to the journal by the
TCC for each variable set for the current mode.  The message area
is of the form "variable=value".
.LI "Configuration End"
.P
.Cw "40||"
.P
A configuration end message is placed in the journal by the TCC to 
indicate
the end of configuration options for a particular mode.
.LI "Test Case Controller Message"
.P
.Cw "50||"
.P
The TCC generates messages when it encounters execution problems.  
The message area gives an indication of the nature of the problem.
.LI "Scenario Information"
.P
.Cw "70||text"
.P
The TCC generates a scenario information line when it encounters a
journal message in the scenario file being processed.
The text of the message is included in the message area.
.LI "Test Case End"
.P
.Cw "80|Activity Status Time|"
.P
A test case end message is generated by the TCC after each test 
case completes
execution. The
parameters are the TCC activity number, 
the time the execution completed and an indication of
the TC's completion status.
.LI "Operator Abort"
.P
.Cw "90|Time|"
.P
The TCC generates an operator abort messages when its execution is 
interrupted by the user.
.LI "Captured"
.P
.Cw "100|Activity|text"
.P
The TCC generates a captured message for each line of output 
captured
when the output capture mode is enabled.  The parameter is the
TCC's activity counter.
.LI "Build Start"
.P
.Cw "110|Activity TestCase Time|"
.P
A build start message is generated by the TCC prior to buildtool
execution. The parameters are the TCC activity counter, the
pathname of the test case being built
and the time the build started.
.LI "Build End"
.P
.Cw "130|Activity Status Time|"
.P
A build end message is generated by the TCC upon completion of 
buildtool
execution.
It contains the TCC activity number, an indication of the completion 
status of buildtool and
the time of that completion.
.LI "Test Purpose Start"
.P
.Cw "200|Activity TPnumber Time|"
.P
The TCM generates a test purpose start message for each test 
purpose executed.
The message contains the TCC activity number, the test purpose 
number, 
and the time execution started.
.LI "Test Purpose Result"
.P
.Cw "220|Activity TPnumber Result Time|"
.P
The TCM generates a result message for each test purpose executed.
This result is usually supplied by the test purpose, although it is
supplied by the TCM in the event a test purpose does not specify a
result. Legal results are specified in 
Result Code Definitions (\(sc7.1.3).  Parameters for this message 
are
the TCC activity number, the test purpose number, the result code, 
and
the time execution completed.
.LI "Clean Start"
.P
.Cw "300|Activity TestCase Time|"
.P
The TCC writes a clean start message to the journal before invoking
cleantool for a given test case.
.LI "Clean End"
.P
.Cw "320|Activity Status Time|"
.P
After a clean operation on a test case is complete, the TCC records
the TCC activity number, the completion status, and the time at which
the operation was completed.
.LI "Invocable Component Start"
.P
.Cw "400|Activity ICnumber TPcount Time|"
.P
The TCM generates an invocable component start message for each 
invocable
component that is executed
during a test case run. This message contains the TCC activity
counter, the invocable component number, number of test purposes 
to
be executed, and the time invocable component execution started.
.LI "Invocable Component End"
.P
.Cw "410|Activity ICnumber TPcount Time|"
.P
The TCM generates an invocable component end message for each 
invocable
component that is completed.
The message contains the TCC activity counter, the invocable 
component
number, the number of test purposes actually executed
and the time of the invocable component completion.
.LI "Test Case Manager Message"
.P
.Cw "510|Activity|text"
.P
If the TCM/API encounters a problem, it will be written to the journal as
a TCM message.  The parameters indicate the TCC activity counter.
The message
area contains a brief description of the problem.
.LI "Test Case Information"
.P
.Cw "520|Activity TPnumber Context Block Sequence|text"
.P
When a test case outputs information to the execution results file,
it is recorded as test case information.  The message specified by the
test case is in the text area of this line.
.LI "Test Case Controller End"
.P
.Cw "900|Time|"
.P
When the TCC completes sequencing of the scenario, it generates a 
TCC
end message.  This message indicates the time execution completed.
.LE
