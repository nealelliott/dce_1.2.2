'''	SCCS: @(#) DTET SPECIFICATION		Rel 1.2 (9/15/92)
.nr H1 3
.H 1 "Functional Specification"
.P
.H 2 "Test Case Controller"
.P
.H 3 "Modes of Operation for Remote Test Cases"
.P
The MTCC follows the start-up phase described for the TCC.  In addition to its
own local configuration data, the MTCC must also sequentially enter all the 
configuration data associated with each system identifier referenced 
anywhere within the scenario. (See also Section 4.1.3). The implementation
must also ensure that the test suite and associated results directory exists
for each of the remote systems.
.P
.H 4 "Build Mode"
.P
The MTCC undertakes the building of a distributed test case in two phases. 
The first
phase is an optional prebuild phase which is only executed on the master system
and only if the master system was included in the the remote directive.
This prebuild phase can be used, for example, to transfer files from the master
to each of the slave systems, assuming that a suitable file transfer mechanism
is available. With regard to journal entries and resource protection the 
prebuild phase is considered to be part of the build phase.
The MTCC executes prebuild tool in the source directory of the test case on 
the master system. 
If output capture mode is enabled, then the arguments passed to
prebuild tool are TET_PREBUILD_FILE and the name of the test case, otherwise
no arguments are passed.
.P
The second phase is the concurrent building of the different parts of the test
case on each of the systems specified by the \fBremote\fR directive.
.P
The MTCC waits until the build has finished on all the systems specified and
then sequentially transfers the results of the build into the journal file.
.P
If any part of the build has failed, the MTCC will be informed
by the appropriate STCC and the whole build will be considered to have failed.
.P
.H 5 "Future Directions"
.P
It may be desirable to provide an the option for the prebuild tool to 
undertake the whole build on the master system and to transfer the 
appropriate files to each of the slave systems. 
This option may be useful when testing homogeneous systems
across a transport that provides a file transfer mechanism.
.P
There is also a request to allow partial build failures. 
This could be useful if a test accesses a variable number of systems 
according to a parameter specification.
.P
.P
.H 4 "Execute Mode"
.P
The execute mode of the DTET follows the model specified for the TET with the
following additions.
.AL
.LI
After the test case start message is delivered to the journal, the MTCC
initiates the concurrent execution of the test case on all the systems 
specified.
.LI
Each of the MTCC and the STCCs act according to the TET_EXEC_IN_PLACE 
configuration variable setting in the same manner as the TCC in TET.
.LI
The MTCC and the STCCs execute the appropriate parts of the test case as
described under the Application Programming Interfaces.
.LI
If TET_TRANSFER_SAVE_FILES is true, a file transfer mechanism
associated with the current transport is used to transfer each of the files
matching
TET_SAVE_FILES to the saved files directory hierarchy on the master system.
The saved files directory hierarchy is enhanced by the inclusion of
subdirectories for each of the remote systems. These subdirectories are
named REMOTE\fInnn\fP, where \fInnn\fP designates the remote system in the same 
manner as in the scenario file and the configuration variables. This includes
the case that the designation \fI000\fP is explicitly used to refer to the
master system.
.LI
If TET_TRANSFER_SAVE_FILES is not provided or is a null string, the STCC and 
MTCC treat the TET_SAVE_FILES configuration variable in the same manner as 
in the TET.
.LI
The MTCC waits until all the branches of the distributed test case have
completed before proceeding to the next stage in processing the scenario file.
.LE
.P
.H 4 "Clean Mode"
.P
The clean mode of the DTET follows the model specified for the TET.
The master system initiates the concurrent cleaning of the different 
parts of the test case on each of the systems specified by the \fBremote\fR 
directive.
.P
The MTCC waits until the clean has finished on all the systems specified and
then sequentially transfers the results of the clean into the journal file.
.P
If any part of the clean has failed, the MTCC will be informed
by the appropriate STCC and the whole clean will be considered to have failed.
.P
.H 5 "Future Directions"
.P
It may be desirable to permit the whole of the clean operation to be 
executed from the master system.
This option would require that the clean file be able to contain
directives that execute remotely on the slave system and/or transfer files
to these remote systems.
.P
.H 3 "Major Options"
.P
The resume and re-run options will operate in the same manner as for the TET
except for test cases running within \fBparallel\fP or \fBrepeat\fP directives.
.P
.H 4 "Resume Option"
.P
For a failed test case within a \fBparallel\fP directive, the resume option 
will cause
the MTCC to start from the beginning of the appropriate parallel group, 
ie. as though the scenario started at the \fBparallel\fR directive. 
.P
For a test case within a \fBrepeat\fP directive, the resume option will cause 
the DTCC to start processing the scenario file from the specific test case
within the \fBrepeat\fR directive.
Consequently, the matched test case and all the subsequent test cases within
the directive will be repeated the number of times specified by the 
\fBrepeat\fP directive. Those above the matched test case will be repeated 
once less.
.P
.H 4 "Rerun Option"
.P
For a failed test case within a \fBparallel\fP directive, the whole of the 
parallel group will be rerun.
.P
For a test case within a \fBrepeat\fP directive, a rerun will cause the MTCC 
to repeat only that specific test case the number of times specified by 
the \fBrepeat\fR directive.
.P
.H 3 "Configuration Variables"
.P
When a scenario contains a \fBremote\fR directive the variables contained in 
the master configuration files for build, execute and clean 
are transferred to each slave system and will form the default values for those
systems.
.P
Any variable on the slave system with the same name, that is defined for the
same mode, overrides a default variable from the master system.
.P
A special case exists for variables on the master system prefixed with
TET_REMnnn, where nnn refers to a particular slave system. The variable
overwrites any variable on the slave system specified which has the same name
after the "TET_REMnnn" string has been removed.
.P
As with TET, variables supplied on the master TCC command line, apply to 
all modes.
These variables have the highest precedence. A variable definition given on
the command line will override any definition from a configuration file. A
variable specified on the command line that is specific to a given system
will have higher precedence than a generic variable that is also specified
on the command line. The order in which variables appear on the command line 
is significant. Those appearing last have highest precedence. 
.P
The following additional variables are defined for the DTET:
.TS
center, box, tab(%);
l | l.
Variable%Default Value
_
TET_PREBUILD_FILE%Undefined
TET_PREBUILD_TOOL%Undefined
TET_TRANSFER_SAVE_FILES%False
.TE
.P
.VL 6n
.LI TET_PREBUILD_FILE
.br
This variable names the file of instructions for the prebuild tool.
.LI TET_PREBUILD_TOOL
.br
This variable names the prebuild tool.
.LI TET_REM\fInnn\fP_
.br
This prefix is used to make a variable specific to the system designated
by \fInnn\fR.  This provides the potential for a single point of control 
for configuration data. The master system is always designated as \fI000\fR.
.LI TET_TRANSFER_SAVE_FILES
.br
The setting of this variable determines whether the saved files designated
by the slave system's value for the variable TET_SAVE_FILES are to be
transferred to the master system.
.LE
.P
.H 3 "Communication Variables"
.P
The following additional variables are used by the DTET.
.P
.VL 6n
.LI TET_DIST
.br
The pathname of the distribution configuration file. This file contains the
TET_EXECUTE, TET_ROOT, and TET_TSROOT variables for the master system and all
the remote systems contained in the scenario. The file will also contain
any variables that the MTCC used to establish a connection with the remote 
systems.
.LI TET_COM_VAR
.br
The pathname of the file that contains those variables specified on the MTCC
command line.
.LI TET_TIARGS
.br
A string of transport independent arguments to be used by the distributed
features of the master or slave TCM.
.LE
.P
.H 3 "Result Codes"
.P
The MTCC uses result codes as described for the TET. Slave systems inherit
their result codes from the  master system.
.P
.H 3 "Journal Entries"
.P
The DTET's usage of the journal file is similar to that described for the TET.
The one additional complication that arises is that test purpose information
lines can be generated on any of the slaves as well as on the master. The
mechanism used by the TET for re-ordering lines attempts to retain blocks of
information in the time sequence of their generation.
The TET's ordering algorithm is based on the use of context and block 
identifiers. The DTET needs to add a further
system identifier to this pair in order that the possibility of duplication is
avoided. Thus, in the DTET, the triplet: system, context and block, provide the
set of identifiers that are remembered for re-ordering purposes. In the DTET
the first three characters of the context are always used to designate the
system and will match the system designation given to that system. The master
system, therefore, will always be designated \fI000\fP.
.P
.H 3 "Exception Conditions"
.P
.H 4 "Initialisation Errors"
.P
The errors specified for the TCC will apply to the MTCC.
.P
The MTCC, at the commencement of processing, will log on to each of the
STCCs and a failure to complete this task successfully will cause the MTCC
to abort after informing any STCC that has already established connection.
.P
.H 4 "Failure to Build, Execute or Clean"
.P
The DTET drives each of these modes from the master system and any failure
on the master system will be treated as specified for the TET.
.P
Each of the slave systems will inform the master system of any error during
the build, execute or clean modes. The master system will consider the failure
on any slave system in the same manner as for the master itself.
This requires that the STCC indicate to the MTCC the reason for the failure.
.P
.H 4 "Test Session Interruption"
.P
This functionality is implemented on the master system in the same manner as
described in the TET. However, the ability to shutdown processes executing
on the slave systems depends on the state of the processes on those slaves,
and may not be possible until the next synchronisation point, or until a 
timeout occurs.
There is no method of interrupting a slave system and relaying this information
back to the master system and from there to each of the other slaves.
.P
.H 4 "Data File Corruption"
.P
The DTET will act in the same manner as the TET.
.P
.H 4 Timeouts
.P
The timeout can only be specified for the master system. 
This timeout is also applied to each of the slave systems.
.P
.H 4 "Lock Files"
.P
In the DTET implementation, each of the master and slaves employs a locking
system as described for the TET.
.P
.H 3 "Miscellaneous TCC Issues"
.P
.H 4 "Messaging"
.P
See previous comments, in section 3.4.1, regarding the problems relating 
to the support of common locales in the DTET.
.P
.H 4 "Support for Test Cases not using the defined API"
.P
There are no extensions proposed to the mechanism described in the TET.
In this case the test case executing on the master is in complete control
of commencing execution of parts of test cases on other systems. The manner
in which this is done is outside the control of the DTET as is the collection
of test purpose information into the results files. The DTET will consider
this to be the execution of a test case under the TET without any reference
to remote systems.
.P
.H 4 "Support for Interactive Testing"
.P
The DTET implementation is not expected to support interactive distributed
test cases.
.P
.H 4 "Combining TCC Modes"
.P
The DTET supports the same combinations as the TET.
.P
.H 2 "Application Programming Interface"
.P
In addition to the support provided through the TET API, developers gain
support in:
.BL
.LI
Synchronising a series of processes to a specified point in the test case.
.LE
.P
The general flow of control for test cases built using the API is as follows:
.AL
.LI
The MTCM and the STCMs arrange for access to their respective configuration
information.
.LI
The MTCM outputs a TCM start entry to the combined execution results file.
.LI
Each TCM builds a list of test purposes to be executed from the list of
requested invocable components.
.LI
The MTCM and the STCMs arrange for the processing of asynchronous events.
.LI
The MTCM and the STCMs synchronise before executing their respective start-up
procedures (if any).
.LI
The MTCM and each STCM commence parallel execution of each invocable
component in turn. The MTCM outputs an invocable component start entry to
the combined execution results file as each invocable component is started.
.LI
At the commencement of each test purpose the MTCM and the STCMs synchronise.
During this synchronisation process the MTCM and the STCMs ensure that they
are executing a common test purpose. The MTCM outputs a test purpose start
message to the combined execution results file.
.P
During the test purpose any test case information lines from the TCMs are 
entered into the combined execution results file.
.P
The MTCM and one or more of the STCMs may synchronise at various points
during the execution of a test purpose.
.LI
If an event occurs which interrupts the processing of any part of a test
purpose, the interrupted process immediately proceeds to an end of test
purpose synchronisation point and informs the MTCM that an unexpected event
has occurred. The MTCM informs the other STCMs of this and then outputs a test
result of \s-1\f7UNRESOLVED\fP\s0. If the event is a termination message 
from the MTCC, then
the API reports receipt of the message to the execution results file, executes
the specified clean-up procedure (if any); otherwise it continues by processing
the next test purpose.
.LI
If a test purpose has been marked as cancelled, the MTCM and STCMs will be 
informed during the automatic sync at the start of the test purpose.
Cancellation, the MTCM proceeds to the next test purpose; otherwise the MTCM
synchronises with each STCM as if the test purpose were to be executed and
the MTCM outputs a test result of \s-1\f7UNINITIATED\fP\s0 to the combined 
execution results file and the STCMs and MTCM will then proceed to
process the next test purpose.
.LI
At the end of a test purpose, the MTCM and STCMs synchronise.
The individual results of the STCMs and the MTCM are then arbitrated and 
a final test result is output to the combined execution results file.
.LI
Once all of the test purposes in an invocable component have been executed,
the MTCM outputs an Invocable Component End message and the TCMs move on to 
the next invocable component.
.LI
After all test purposes for all requested invocable components have been
executed, the API executes the specified clean-up procedure (if any) and
exits normally.
.LI
If for any reason, a particular TCM fails to execute the requested set of 
invocable components or the specified start-up or clean-up procedure, the 
relevant API will exit abnormally.
.P
The remaining components of the distributed test case will fail to 
syncronise at the next sync and will all terminate.
.LE
.P
.H 3 "Initialising and Cleaning up Test Cases"
.P
Each part of a test case on the master or one of the slaves may have its own
start-up and clean-up routines.
.P
.H 3 "Selecting Invocable Components and Test Purposes"
.P
The MTCM controls the relationship between invocable components and test
purposes. Each STCM is only interested in executing the appropriate part of
a test purpose in parallel to the MTCM and other STCMs. Information regarding
the test purposes to be executed is transferred from the MTCM to each STCM at
the first synchronisation point. At each test purpose start synchronisation
point, the MTCM and the STCMs verify that they are executing the same test
purpose.
.P
.H 3 "Insulating from the Test Environment"
.P
Each of the STCMs and the MTCM is insulated from events that can occur in
its environment. The methods used and the types of event that can occur in
a particular environment are specific to that environment and there is no
requirement that the master and slave systems are consistent environments.
.P
.H 3 "Making Journal Entries"
.P
In the DTET environment the same interfaces are used to generate journal
entries as in the TET environment. The journal entries contain additional
information about the remote system identification, though this is provided
transparently from the API.
.P
.H 4 "Entries from the API"
.P
All distributed TCMs make entries in a \s-1\f7tet_xres\fP\s0 file in the same 
fashion as the TET. 
In the case of a \fBremote\fR directive which uses the master system, a 
combined execution results file (called \s-1\f7tet_combined\s0\fR) is also 
generated.
This file is located in the execution directory of the master test case and
is  called \s-1\f7tet_combined\s0\fR).
When a distributed test case uses the MTCM the \s-1\f7tet_combined\fP\s0 file
is incorporated into the journal file instead of the \s-1\f7tet_xres\fP\s0
files from each of the remote systems.
.P
.H 4 "Entries from Test Purposes"
.P
In the DTET environment, the STCMs and MTCM behave in the same manner as the
TCM in the TET environment.
The results supplied by the various parts of a distributed test purpose 
are arbitrated, and in the case that the final outcome is a result for which
the action is "Abort", the TCMs exit at the next synchronisation point.
.P
.H 3 "Cancelling Test Purposes"
.P
The DTET allows any of the STCMs or the MTCM to request the cancellation of a 
test purpose.
When a cancelled test purpose is about to start each of the STCMs
and the MTCM are informed via
the appropriate test purpose start synchronisation that this test
purpose has been cancelled. 
The STCMs and MTCM, on receipt of this message, proceed to
the next test purpose start, instead of executing the cancelled test purpose.
.P
.H 3 "Manipulating Configuration Variables"
.P
The DTET provides the same facilities as the TET. Each of the STCMs and the
MTCM has access to its own set of configuration variables via this common
interface.
.P
.H 3 "Generating and Executing Processes"
.P
The DTET allows either one of the STCMs or the MTCM to generate and execute
processes in the manner described in the TET. These processes are generated
and executed on the same system as the calling process, this mechanism does
not allow for the execution of processes on other systems.
.P
There is an additional problem in that there could be a requirement for a
child process to synchronise with a process on another system. This is
not considered to be desirable and only the processes
initiated by the MTCC or the STCC are allowed to request synchronisation.
.P
.H 3 "Remote Process Execution"
.P
The DTET provides an additional mechanism for the execution of a process on
a system other than that of the calling process.
The remote execution can only be targeted at a remote system
that is already known to the MTCC (i.e. the appropriate TET_REM\fInnn\fP_
variables have been specified and the MTCC has already established connection
with the STCC).
It is the test suite author's responsibility to decide whether the test is 
written as a series of separate parts for execution on separate systems, or 
whether the remote execution call is to be used.
.P
Once the STCC has initiated the requested remote process, the calling process
and the requested process will synchronise before any further processing takes
place. The two processes may then negotiate subsequent synchronisation using the
normal DTET synchronisation calls. It should be noted that the initial
synchronisation takes place between the calling MTCM and the newly created
remote process as part of its initialisation; the MTCC and the STCC do not
perform this synchronisation. This ensures that the newly created remote is
able to communicate with the calling MTCM.
.P
