'\"	SCCS: @(#) TET SPECIFICATION		Rel 4.5 (06/23/92)
'\"
.nr H1 3
.H 1 "Functional Specifications"
The functional specification provides system-independent 
descriptions
of the technology required by the Toolkit's architecture. The
descriptions include sufficient detail to implement a 
conforming environment. The functionality described in this section 
must 
be fully supported by any TET implementation.  Interface
specifications which support this functionality in specific
environments are provided in Sections 5 and 6.
.H 2 "Test Case Controller"
This section specifies the functionality of the Test Case Controller 
(TCC). 
The TCC determines user and suite specific configuration options 
and
facilitates user control of test sessions. This control includes the 
building and execution of test cases.
.P
In addition to the control of test sessions, the TCC also includes
functionality to support internal mechanisms essential to the 
operation
of the Toolkit environment. These include managing interaction with
the test case, the processing of results and the removal of temporary files.
.P
Note that while the TET architecture is designed to support multiple
test suites simultaneously, the TCC only permits execution of test
cases from a single test suite per TCC run.
.H 3 "Modes of Operation"
The TCC provides these three modes of operation:
.BL
.LI
Build - translate source test cases into executables
.LI
Execute - load and execute test cases 
.LI
Clean - remove files created by build mode
.LE
.P
The TCC operates in any combination of build, execute or clean modes.
The behaviour of these modes may be modified by user direction
or by configuration
options.
.P
Regardless of the mode selected, the TCC performs the following
actions during start-up:
.AL 1
.LI
If a specific journal is named by the user, verifies that it does not
exist.
.LI
Verifies that the test suite selected by the user is
available.
.LI
Identifies the selected scenario file, and ensures
that the scenario selected by the user is present.
.LI
Identifies configuration variable settings, and integrates
custom user settings for those configuration variables.
.LI
Ensures that the results directory exists.
.LI
Tells the user the name of the journal file being used.
.LI
Writes a start-up message into the journal.
.LI
Delivers details of option
and configuration variable settings for all modes enabled to the 
journal file.
.LE
.P
After start-up the TCC processes the selected scenario, performing
operations on each test case in the order they are encountered.
.I "[Requirement 3.10]"
The following sections describe TCC actions in each mode when
test cases are encountered.
.H 4 "Build Mode"
When the test suite is provided in source form, the TCC is 
responsible
for building executable versions of test cases. There is no
requirement that test suites be provided in source form. Therefore 
use
of build functionality is at the discretion of the test suite provider
and user (i.e., binary distribution of tests is supported, assuming 
the binary is suitable for the system under test).
.I "[Requirement 2.4]"
.P
For each test case referenced in the specified test scenario, the TCC
does the following:
.AL 1
.LI
Delivers a build start message to the journal.
.LI
Obtains exclusive locks in the source and execution directories
of the test case.
.LI
Executes buildtool in the source directory of the test case.
If output capture mode is enabled, then the arguments passed to buildtool
are TET_BUILD_FILE and the name of the test case, otherwise
no arguments are passed.
.LI
Transfers buildtool output and error messages to the journal file,
if output capture mode is enabled.  If output capture mode is disabled,
re-orders and copies the contents of the results file to the journal.
.LI
Removes the lock set in step 2.
.LI
Delivers a build end message to the journal.
.LE
.P
If the user has
specified an alternate execution directory, TCC provides that 
information 
to the buildtool via a communication variable (\(sc4.1.4).
Buildtool and buildfile authors must ensure that appropriate files 
are
transferred to the alternate execution directory during the build
operation.
.P
In build mode the user is able to:
.BL
.LI
Set the scenario file name.
.LI
Set the build configuration file name.
.LI
Set the root directory for the hierarchy of test case executables.
.LI
Set the name of the journal file.
.LI
Set the timeout limit (in seconds) per buildtool execution.
.LI
Select the re-run or resume options (see section 4.1.2).
.LI
Set configuration variable values.
.LE
.H 4 "Execute Mode"
In this mode, the TCC executes test cases in
the specified scenario.
For each test case referenced, the TCC does the following:
.AL 1
.LI
Delivers a test case start message to the journal.
.I "[Requirement 3.9]"
.LI
Obtains a shared or exclusive lock in the execution directory
of the test case.  If TET_EXEC_IN_PLACE is false, the lock is
shared, otherwise exclusive.
.LI
If TET_EXEC_IN_PLACE is false, creates a temporary directory in which
executions can be safely performed.
.I "[Requirement 3.7]"
.LI
If TET_EXEC_IN_PLACE is false,
recursively copies the files in the test case execution directory to the
temporary directory.
.LI
Removes the lock set in step 2, if TET_EXEC_IN_PLACE is false.
.LI
If TET_EXEC_TOOL is not defined,
executes the test case in the test case execution directory 
if TET_EXEC_IN_PLACE is true, otherwise in the temporary directory,
with arguments of the appropriate invocable components.
If TET_EXEC_TOOL is defined,
executes the exectool in the test case execution directory 
if TET_EXEC_IN_PLACE is true, otherwise in the temporary directory,
with arguments of TET_EXEC_FILE, the test case name and the appropriate
invocable components.
Behaviour of
test cases written to the TET API is detailed in Application Programming
Interfaces (\(sc4.3).
.LI
Recursively transfers files matching TET_SAVE_FILES to the
corresponding place in the saved files directory hierarchy.
.LI
If output capture mode is disabled, re-orders and copies
the contents of the execution results file to the journal.
If any test purpose has not generated a result, the TCC adds a result 
message of
.Cw NORESULT .
If output capture mode is enabled, the TCC adds appropriate lines to the
journal based on test case output and exit status (\(sc4.1.3).
.LI
Removes the lock set in step 2, if TET_EXEC_IN_PLACE is true.
.LI
Removes the temporary directory, if TET_EXEC_IN_PLACE is false,
.LI
Delivers a test case end message to the journal.
.LE
.P
In execute mode the user may:
.BL
.LI
Set the scenario file name.
.LI
Set the execution configuration file name.
.LI
Set the alternate execution directory.
.LI
Set the root directory for the saved files hierarchy.
.LI
Set the name of the journal file.
.LI
Set the timeout limit (in seconds) per test case execution.
.LI
Set the list of file names which are to be copied to the saved files
hierarchy after each test case has completed.
.LI
Select the re-run or resume options (see section 4.1.2).
.LI
Set configuration variable values.
.LE
.H 4 "Clean Mode"
Users may request that the TCC perform operations on test 
scenarios
to remove unwanted files following test sessions. Clean mode will
never affect the results of previous test runs.
.P
For each test case referenced, the TCC does the following:
.AL 1
.LI
Delivers a clean start message to the journal.
.LI
Obtains exclusive locks in the source and execution directories
of the test case.
.LI
Executes cleantool in the source directory of the test case.
If output capture mode is enabled, then the arguments passed to
cleantool are TET_CLEAN_FILE and the name of the test case, otherwise
no arguments are passed.
.LI
Transfers cleantool output and error messages to the journal file,
if output capture mode is enabled.  If output capture mode is disabled,
re-orders and copies the contents of the results file to the journal.
.LI
Removes the lock set in step 2.
.LI
Delivers a clean end message to the journal.
.LE
.P
In clean mode the user may:
.BL
.LI
Set the scenario file name.
.LI
Set the clean configuration file name.
.LI
Set the root directory for the hierarchy of test case executables.
.LI
Set the timeout limit (in seconds) per clean.
.LI
Select the re-run or resume options (see section 4.1.2).
.LI
Set configuration variable values.
.LE
.H 3 "Major Options"
.H 4 "Resume Option"
.P
When invoked with the resume option, the TCC analyses the old journal
file, searching for user-specified result codes and build and clean
status codes.  When one of the specified result codes or status
codes is found in the journal file, the TCC builds, executes and/or
cleans that and all subsequent invocable components in the scenario.
.P
Note that this option requires the existence of the scenario file which
was used to generate the old journal file.
.I "[Requirement 3.3]"
.P
The user specifies the search criteria as a list of result code names
and other code letters which the TCC processes as follows:
.VL 20n
.LI "\fBList element\fR"
\fBJournal lines matched\fR (see 7.2.1)
.LI \fIresult_code_name\fR
Test Purpose Result lines giving execution results (not build or clean
results) with the equivalent result code number.
.LI b
Build End lines in which the completion status indicates a failed build;
and, Test Purpose Result lines giving build results with any non-zero
result code number.
.LI e
Test Case End lines in which the completion status indicates the test
case did not run to completion; and, Test Purpose Result lines giving
execution results with any non-zero result code number.
.LI c
Clean End lines in which the completion status indicates a failed clean;
and, Test Purpose Result lines giving clean results with any non-zero
result code number.
.LE
.H 4 "Re-run Option"
This option is similar to the resume option, with the exception that
.B only
invocable components which meet the user-specified search criteria
will be processed.
.I "[Requirement 3.6]"
Where the search finds test cases which did not run to completion, all
the invocable components of that test case that are listed in the scenario
file and for which there is no IC End line in the journal are processed.
.H 3 "Configuration Variables"
Configuration variables allow the modification of
defaults for the TCC and test suites. The mechanism for supporting
this functionality is a configuration file per test suite per mode.
.I "[Requirement 2.2]"
.P
For each mode the configuration information is obtained from a 
configuration file.  This file is located by the TCC using the
following algorithm:
.I "[Requirement 4.3]"
.AL 1
.LI
If the user named a specific configuration variable file at invocation
time, that file is used;
.LI
otherwise, if the user-named alternate execution directory has a 
configuration variable file in it, that file is used;
.LI
otherwise, if there is a configuration variable file in the test suite
root directory, that file is used.
.LE
.P
If the user set specific variables at TCC invocation time, those settings
override the ones in the configuration variable file.
.P
Configuration data is made available to the test case via the API.
The mechanism for providing this information is implementation 
defined. 
.P
Test suites may add their own configuration variables, although 
variable names
beginning with the sequence "TET_" are reserved for future 
extensions
of the Toolkit.  There are no default values for test suite defined
variables.  Test suite authors must ensure that in the absence of
necessary configuration information their tests behave appropriately.
.P
.ne 14v
The following configuration variables are currently defined:
.P
.TS
box center;
l | l.
Variable	Default Value
_
TET_BUILD_FAIL_TOOL	Undefined
TET_BUILD_FILE	Undefined
TET_BUILD_TOOL	Undefined
TET_CLEAN_FILE	Undefined
TET_CLEAN_TOOL	Undefined
TET_EXEC_FILE	Undefined
TET_EXEC_IN_PLACE	False
TET_EXEC_TOOL	Undefined
TET_OUTPUT_CAPTURE	False
TET_RESCODES_FILE	tet_code
TET_SAVE_FILES	Undefined
.TE
.P
Note that a default of "Undefined" means that the variable is not
provided unless it is in a test suite or a user-provided configuration
variable file.
.P
.VL .5i
.LI "TET_BUILD_FAIL_TOOL"
.br
This variable names a utility that will be executed if the buildtool
indicates that a test case build failed.
.LI "TET_BUILD_FILE"
.br
This variable names the file of instructions for the buildtool.
.LI "TET_BUILD_TOOL"
.br
This variables names the buildtool.
.LI "TET_CLEAN_FILE"
.br
This variable names the file of instructions for the cleantool.
.LI "TET_CLEAN_TOOL"
.br
This variable names the cleantool.
.LI "TET_EXEC_FILE"
.br
This variable names the file of instructions for the exectool.
.LI "TET_EXEC_IN_PLACE"
.br
This variable applies only to execute mode, and is used to control
where the test cases are executed.
When this variable is true, the TCC executes each test case `in place'
in the directory containing the test case executable; when it is false,
the TCC first copies the files to a temporary directory before executing them.
.LI "TET_EXEC_TOOL"
.br
This variable names the exectool.
Normally this is left undefined, in which case the test case itself is
executed.  If TET_EXEC_TOOL is defined, the named program is executed with
TET_EXEC_FILE and the test case name as additional arguments preceding any
test case arguments.
.LI "TET_OUTPUT_CAPTURE"
.br
Test cases not written using the TET API may 
write diagnostic
and error messages using system output or diagnostic facilities.
When this variable is true, the TCC pretends that the test case is an
executable that has within it a single invocable component numbered 
0
with a single test purpose.  The TCC generates test case, invocable
component, and test purpose start messages and then executes the 
test
case.  The output from the test case is recorded as test purpose
messages (with a block number of 0).  When the test case 
terminates,
the TCC supplies a test purpose result message using the completion
status of the process.  It then generates invocable component and 
test
case end messages.
The default for TCC operation is false.  Note that when this mode is
enabled, the result file processing normally done by the TCC is disabled.
.P
The setting of this variable also affects build and clean modes (\(sc4.1.1).
.LI "TET_RESCODES_FILE"
.br
This variable indicates the name of the file which contains test
suite specific result code definitions. The default for this variable
is "tet_code".
.LI "TET_SAVE_FILES"
.br
This is a comma-separated list of filenames.  If files matching one 
of these names are found in the execution directory hierarchy after 
test case execution, they are transferred to the corresponding place
in the saved files hierarchy.
.LE
.SK
.H 3 "Communication Variables"
The TCC must be able to communicate with the other tools it 
executes
(buildtool, cleantool, exectool, and the test cases). TCC does this
via communication variables. These variables may
co-exist with other resource spaces on a system. To prevent overlap 
with 
other names in the same space, implementations should reserve 
names 
beginning with "TET_". The communication variables currently defined
include:
.P
.VL .5i
.LI "TET_CONFIG"
.br
The pathname of the current configuration variable file.
.LI "TET_CODE"
.br
The pathname of the current result codes definition file.
.LI "TET_ACTIVITY"
.br
The number of activities performed thus far by the TCC.  Activities
include executions of buildtool, cleantool, exectool, and test cases.
.LI "TET_EXECUTE"
.br
The pathname of the root of the execution directory hierarchy.
.LI "TET_ROOT"
.br
The pathname of the TET root directory.
.LE
.H 3 "Result Codes"
A mapping mechanism for the TCC is provided to interpret results
from test suites. This mapping is contained in a results file. Upon
termination of a test case, the TCC determines the action required
for each result code and writes an entry in the journal. The API 
library 
ensures that the test purposes only generate TET or test suite
specified results.
.P
TET provides a default set of result codes
(\(sc7.1.3).  Additional result codes may be defined on a per test 
suite
basis.
.I "[Requirement 2.6]"
.H 3 "Journal Entries"
The TCC manages updates to the journal file. Upon completion of a
test case execution, build or clean the results are
transferred into the journal file. During this transfer, the TCC
ensures that each executed test purpose generated one (and only one)
result.
.P
Because a test purpose may have generated other processes that may
have written into the execution results file, the TCC must ensure 
that
the information in the execution results file is ordered correctly
when it is transferred into the journal.  This ordering is performed
using the following algorithm:
.AL 1
.LI
While the message type is not test purpose start, lines are copied 
from 
the result file to the journal.
.LI
Once a test purpose start message is found, it is transferred to the 
journal.  Then all lines between that and the test purpose result
message (or end of file) are ordered by:
.AL a
.LI
Looking at the first untransferred line in the range.  If the line is
not a test purpose information line, it is transferred and step (a) is
repeated.  If it is a test purpose information line, it is transferred
and the context and block identifiers are remembered.
.LI
The TCC then looks through the range for additional test purpose
information lines with the same context and block identifiers.  
Those
are transferred after sorting them in ascending order by sequence
number.
.LI
When the end of the range is encountered, the TCC returns to step 
(a).
This process is repeated until all lines in the range are transferred.
.LE
.LI
The test purpose result line is transferred to the journal and the 
TCC
starts again from step 1.  These steps are repeated until the
execution result file is exhausted.  If there is a missing test
purpose result, the TCC supplies a result of
.Cw NORESULT
for that test purpose.
.I "[Requirement 3.2]"
.LE
.H 4 "Future Directions"
A proposed future addition is for the TCC to be capable of supporting the
execution of several
test cases simultaneously.  In this situation, each test case places its
results into a separate execution results file.  The TCC captures and
transfers the execution results in the order in which the test cases 
were 
specified in the test scenario.
.I "[Requirement 5.1]"
.SK
.H 3 "Exception Conditions"
.H 4 "Initialisation Errors"
If the TCC does not locate a user specified or required input file 
upon 
test session start-up,
the TCC will abort and inform the user of the error. 
.H 4 "Failure to Build, Execute or Clean"
If any specified tools (e.g. buildtool, cleantool, exectool) are not available,
the TCC will generate an end message for that tool with a status of
"-1" and discontinue processing of the scenario.
.P
If, for a specified test case, the buildtool returns an abnormal 
termination, then the TCC will execute the optional build fail tool.
.P
If in execute mode the TCC cannot find a test case or the test case 
is
not executable, the TCC will indicate a test case completion status 
of
"-1" and continue processing the scenario.
.H 4 "Test Session Interruption"
This function is requested by using the normal system interrupt
facilities. Upon receipt of the system
interrupt, the TCC will order the shutdown of the currently 
executing
test case.  Test cases written to the TET API will record a TCM
message in the execution results file indicating that a termination
request was received, and terminate.
Once the test case execution has stopped, the execution results 
file is captured and transferred to the journal.  
.H 4 "Data File Corruption"
Upon detection of an invalid data file format, the TCC will abort
the test session and issue a diagnostic containing the pathname of
the file to the journal and to the user.
.H 4 "Timeouts"
The TCC user may specify a timeout period.  Build, execution or 
clean-up
operations which take longer than this timeout (on a per test case
basis) will be interrupted by the TCC.  In such a case, the TCC
retrieves any execution results and transfers them to the journal.  It
then places a message in the journal indicating a timeout, and sets
the completion status of the test case to "-2" (\(sc7.2.1).
.H 4 "Lock Files"
The TCC employs a locking system which prevents concurrently
executing TCCs from interfering with each other's processing of test
cases.  In the following description, "execution directory" means
the execution directory under the alternate execution directory
hierarchy if one is in use, otherwise
the source directory.
.P
In build and clean modes the TCC obtains an exclusive lock in both
the source and execution directories as follows:
.BL
.LI
A file "tet_lock" is created in the source directory, using an
atomic operation which will fail if a file or directory of
that name already exists.  If the file cannot be created the
locking operation fails.
.LI
If an alternate execution directory hierarchy is in use, a file "tet_lock"
is created in the execution directory in the same manner.  If the file
cannot be created, the lock is removed from the source directory
and the locking operation fails.
.LE
.P
When the build or clean has completed the lock files are removed,
execution directory first.
.P
In execute mode with TET_EXEC_IN_PLACE true, the TCC obtains an
exclusive lock in the execution directory using the same method
as for build mode.
.P
In execute mode with TET_EXEC_IN_PLACE false, the TCC obtains a
shared lock in the execution directory as follows:
.BL
.LI
A directory "tet_lock" is created in the execution directory,
with read and write permission for all users, using an atomic
operation which will fail if a file or directory of that name
already exists.  If the directory cannot be created because a
plain file exists, the locking operation fails.  If the
directory cannot be created because a directory already
exists, the failure is ignored.
.LI
A unique file is created in the directory "tet_lock".  If the
file cannot be created because "tet_lock" either does not
exist or is a plain file, then the locking attempt is re-started.
.LE
.P
When the execute has completed the lock is removed as follows:
.BL
.LI
The file created when the lock was obtained is removed from
the "tet_lock" directory.
.LI
The "tet_lock" directory is removed, using an operation which
will fail if the directory is not empty.  Failure of this
operation is ignored.
.LE
.P
When a locking operation fails, if the user has specified a timeout
period, then the TCC periodically re-tries the operation until the
timeout period has elapsed.  If the user has not specified a timeout
period, or the timeout period elapses, then the TCC places a message
in the journal indicating a locking failure, and sets the completion
status of the test case, buildtool, cleantool or exectool to "-3" (\(sc7.2.1).
.SK
.H 3 "Miscellaneous TCC Issues"
.H 4 "Messaging" 
Toolkit implementations provide messages to users in their native
language. This "messaging" is done in implementation defined ways,
and may not be available for all languages in all implementations.
Test suite authors should be aware of the languages and support for
messaging provided in particular implementations, and provide
equivalent functionality.
.H 4 "Support for Test Cases not using the defined API"
It is highly desirable to support test cases
written to the other test environment toolkits. Unfortunately, 
it is difficult to establish a mechanism which would support the 
many types 
of test cases which currently exist. Therefore,
a simple mechanism is provided to map exit status codes to result 
codes and capture
test case diagnostic output. This is done by enabling output
capture mode (\(sc4.1.3).
.H 4 "Support for Interactive Testing"
In some environments it may be appropriate to perform tests which
require interaction with users. In such environments, the TCC must not
prohibit such interactive testing using normal system-provided
methods.
.I "[Requirement 3.11]"
Note that in many environments interactive testing will not be
possible if the TET_OUTPUT_CAPTURE configuration variable is set 
to
true.
.H 4 "Combining TCC Modes"
The TCC supports combining its various operational modes.
If build, execute and clean modes are combined, each test case
in the scenario will be individually built, executed and/or cleaned
as required.
If a build fails and either there is no build fail tool in use or the
build fail tool terminates with abnormal completion status, then the
execution and cleaning of that test case are skipped.
.I "[Requirement 3.5]"
.H 2 "Application Programming Interface"
The API supplies functionality to developers of test cases. This
section describes that functionality in a manner independent of
programming language and implementation.
.I "[Requirement 4.4]"
Through
the API, developers gain support in:
.BL
.LI
Initialising and cleaning up test cases
.LI
Selecting invocable components and test purposes
.LI
Insulating from the test environment
.I "[Requirement 4.5]"
.LI
Making journal entries
.LI
Cancelling test purposes
.LI
Manipulating configuration variables
.LI
Generating and executing processes
.LE
.P
The general flow of control for test cases built using the API is as
follows:
.AL
.LI
The Test Case Manager (TCM) arranges for access to the 
configuration 
information. (See "Manipulating Configuration Variables" below).
.LI
The TCM outputs a TCM start entry to the execution results file.
.LI
The TCM builds a list of test purposes to be executed
from the list of requested invocable components.
.LI
The TCM arranges for processing of asynchronous events.
.LI
The TCM executes the specified start-up procedure (if any).
.LI
The TCM commences execution of each requested invocable 
component in turn
and outputs an invocable component start entry to the execution 
results file
as each invocable component is started. 
.LI
The TCM commences execution of each test purpose within an 
invocable
component. As each test purpose is started, a test purpose start
entry is output to the execution results file.
.P
The TCM ensures that the handling mechanism for asynchronous events is
still in place before each test purpose.  This is to ensure that
asynchronous events do not go unnoticed because the handling
mechanism has been changed by an earlier test purpose and not restored.
If desired, the API may provide a facility for the test case author to
disable this behaviour by setting a global flag in
the start-up procedure.  In this case the API programmer
documentation must make clear the dangers of setting this flag.
.LI
If an event occurs which interrupts the processing of a test purpose,
the TCM reports the receipt of the event to the execution results
file as an information line. The TCM then outputs a test result of
.Cw UNRESOLVED
to the execution results file.  If the event is a termination
message from the TCC,
then the API reports receipt of the message to the execution results
file, executes the specified clean-up procedure (if any) and exits
abnormally; otherwise, it continues by processing the next test purpose.
.LI
If a test purpose has been marked as cancelled
(\(sc4.3.5) the TCM does not attempt to execute this test purpose.
Instead, it
outputs an information line detailing the reason for cancellation to 
the
execution results file. The TCM then outputs a test result of
.Cw UNINITIATED
to the execution results file and continues by 
processing
the next test purpose.
.LI
Once all of the test purposes in an invocable component have been executed,
the TCM outputs an Invocable Component End message and moves on to the next
requested invocable component (See step 6).
.LI
After all test purposes for all requested invocable components have 
been 
executed, the API executes the specified clean-up procedure
(if any) and exits normally.
.LI
If, for any reason, the test case fails to execute the requested set of 
invocable
components or the specified start-up or clean-up procedure, the API will 
exit abnormally.
.LE
.H 3 "Initialising and Cleaning up Test Cases"
Upon start-up, test cases written using the Toolkit API may have a 
test
case specific start-up routine executed. Test cases may also have an
optional clean-up routine executed just before test case termination.
.H 3 "Selecting Invocable Components and Test Purposes"
Test Case Manager (TCM), controls sequencing of invocable components
and test purposes.  In order to accomplish
this, it needs information about the relationship of invocable
components to test purposes. Each invocable component can contain 
more than
one test purpose, but each test purpose can be referenced only by a 
single 
invocable component. The special invocable component 
.B all
is a reference
to all test purposes. Other invocable components are requested by 
the
TCC during test case start-up.
The manner in which the relationship
between invocable components and test purposes is established 
depends on the
language binding being used.
.I "[Requirement 4.2]"
.H 3 "Insulating from the Test Environment"
The TCM provides services which automatically process 
asynchronous
events in the system. The event mechanisms and names are
implementation defined. Event selection is done via Configuration
Variables.
.I "[Requirement 4.5]"
.P
.H 3 "Making Journal Entries"
The Toolkit API journal facility support functions provide a 
mechanism for 
outputting data 
to the execution results file. The Toolkit API ensures that each entry 
in 
this file is 
written atomically, and that there is sequencing information applied 
to 
the entry (to allow for a subsequent process to re-order data that is 
produced 
from two or more concurrently executing processes started by a 
single test purpose).
.P
In order to allow for the correct sequencing of information two 
attributes
are defined: the current context and the current block. The current 
block
is a subdivision of the current context and provides a means of 
ensuring
contiguity, after re-sequencing, of a block of data that needed to 
traverse 
several entries. The need to traverse several entries may be caused 
by the
limitations on the atomicity imposed by the implementation, or may be 
purely a 
matter of convenience for the test suite author.
.P
The current context is initialised during test case start-up and can be
changed only after a new process is generated.  This
allows the author to choose whether a number of concurrently executing
test purposes should have the same context or different ones.
.P
The current block is initialised to one at the commencement of
each test purpose. The test author
can increment the current block at any point during the output of 
entries
in order to distinguish one block of data from another. Each 
individual
entry within a block will be sequenced starting at one. Use of the
journal support facilities allows data from concurrently 
executing
test purposes to be ordered correctly by the TCC.
.H 4 "Entries from the API"
As mentioned above, the TCM handles the sequencing of test purposes 
as
a part of the process of executing invocable components. 
The sequencing mechanism will output invocable component start 
and end 
information, and
test purpose start information, to the execution results file. The test 
author is
responsible for outputting test information and test results to the 
execution 
results file.
.P
All of the data for an entry must be transferred atomically to the
execution results file.
It is the responsibility of the test suite author to remain within the
limitations imposed by the implementation for a single atomic write 
operation.
TET requires that systems permit atomic writes of at least 512 
bytes
(\(sc3.4).
.P
If a test purpose executed another process that is built to the 
Toolkit API, 
and that executable is expected to generate
journal messages, the test purpose must use the Toolkit API to
communicate the current message context to the executed process.
.H 4 "Entries from Test Purposes"
The API provides functionality for delivering informational 
messages and
results from test purposes to the execution results file. These
messages are in addition to those specified above, which are 
provided automatically by the TCM.
It is expected that test cases will use this mechanism to deliver 
special
messages to the journal or for additional reporting sequences
that can be analysed by test suite specific report treatment filters.
Test purposes also deliver results to the execution results file.
.I "[Requirement 4.8]"
These results are checked by the API to
ensure that they have been defined by the Toolkit or by the test 
suite. 
.I "[Requirement 4.9]"
In the event of an invalid result, the TCM delivers a message to the
execution results file and sets the result to
.Cw NORESULT .
The result actions are also checked by the API.  If a test purpose
specifies a result for which the action is "Abort", then the TCM will
immediately stop processing test purposes and exit.
.P
Note that if a test purpose neglects to generate a result via the API,
the TCM will supply a result of
.Cw NORESULT
for that test purpose.
.H 3 "Cancelling Test Purposes"
In certain cases, particularly where there are a number of 
inter-related test
purposes, the test author may wish to mark a test purpose as 
cancelled because 
of the failure of another test or because some hardware support is 
not
available. The TCM
determines the status of each test purpose prior to execution.
If the TCM identifies a cancelled test purpose, then it will 
output a test case defined reason for the cancellation as well as a
result of
.Cw UNINITIATED .
The text of 
the reason is provided via the API when the test suite cancels the 
test
purpose. The test suite author can use the API to determine
which test purposes are currently cancelled and the reason for 
cancellation.
.H 3 "Manipulating Configuration Variables"
The test case controller gathers Toolkit and test suite specific
configuration variable settings and provides them to test purposes 
via
the API.
.H 3 "Generating and Executing Processes"
The basic requirements for an implementation of the Toolkit state 
that
the system allows applications to execute other applications and 
that these
applications must have a hierarchical relationship with one another. 
The
API provides facilities for a test purpose
to generate a copy of the test case as a leaf node in this hierarchy of 
processes. It also allows for any node
within the hierarchy of processes to replace itself with
another application process. The implementation may require that 
these two
operations are not distinct.
.P
The TCM ensures that any generated process only performs activities
associated with the current test purpose.
The API also ensures that on completion, the generated
process reports to the generating process the results of its 
execution.
.P
The API also ensures that when a new application replaces an 
existing application within the hierarchy of processes,
this new application can use all of the
facilities within this API description in the same manner as the 
application which was replaced could use them. In particular the TCM 
ensures that the new application has access to the configuration information 
via the API interfaces. It further ensures that any special handling
mechanisms established by the generating process are disabled in 
the new process. A specified routine in the generated process is then
executed, and the results conveyed to the generating process via the
completion status.
The generating process will
optionally execute a routine before analysing the completion status 
of the generated process.
The generating process may specify a
timeout for the generated process.
.P
Explicit API support may be given to generated processes, ensuring that
the above actions occur before, during, and after the execution of
test purpose specified routines.
.H 2 "Test Suite Installer"
The installation procedure for a test suite can be very complex or
very simple.
.P
It is essential that thorough installation documentation be provided
by the test suite author. In particular, test suite specific
configuration variables or other special information should be
provided.
