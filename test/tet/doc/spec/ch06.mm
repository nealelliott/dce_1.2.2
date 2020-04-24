'\"	SCCS: @(#) TET SPECIFICATION		Rel 4.5 (06/23/92)
'\"
.nr H1 5
.nr Cl 2
.ds HP +4 +3 +2 +1 +0 +0 +0
.ds HF 3 3 3 3 3 1 1
.H 1 "Application Programming Interfaces"
The section defines the interfaces that an application (test case) 
would
use to control the execution of invocable components and test 
purposes and
to output information and results to the execution results file
(referred to as the API - Applications Programming Interface).
It is expected that several different language bindings will be made
to the Toolkit functionality. The following sections are normative
references to language bindings for:
.BL
.LI
The C Language on ISO 9945-1 Systems
.LI
The Shell Language (X/Open Portability Guide Issue 3 Volume 1 \(em
.Cw sh )
.LE
.nr Ej 3
.H 2 "C Language Binding"
The synopses here are described in accordance with the
International C Standard ISO 9899. An ISO 9899 or Common 
Usage C (as defined in ISO 9945-1) conforming
compiler is required to develop Test Cases using these interfaces.
The interfaces in this section support all the functionality required
in \(sc4.3 of this document. In environments in which this binding is
available, communication variables are placed in the Environment 
(as
defined in ISO 9945-1).
.P
Applications written to this language binding attach themselves to 
it through the following files:
.VL 4n
.LI
.Cw TET_ROOT/lib/posix_c/libapi.a
.br
contains the support routines for test purposes.
.LI
.Cw TET_ROOT/lib/posix_c/tcm.o
.br
contains the routine
.Fn main
and associated support routines for the sequencing and control of 
invocable
components and test purposes.
.LI
.Cw TET_ROOT/lib/posix_c/tcmchild.o
.br
contains a
.Fn main
routine which can be used by test suites when building processes 
which
test purposes will launch using the
.Fn tet_exec
interface.
.LI
.Cw TET_ROOT/inc/posix_c/tet_api.h
.br
provides a definition of
.Cw "struct tet_testlist" ,
values for use as arguments to
.Fn tet_result
(i.e.
.Cw TET_PASS ,
.Cw TET_FAIL ,
.Cw TET_UNRESOLVED ,
.Cw TET_NOTINUSE ,
.Cw TET_UNSUPPORTED ,
.Cw TET_UNTESTED ,
.Cw TET_UNINITIATED
and
.Cw TET_NORESULT )
and declarations/prototypes
for all the 'C' API interfaces.
.LE
.P
Each of these files should be accessed by test suites
via their 
.B buildtool
in a way which is appropriate given the available C\ language
translation system.  Test suite authors are advised to allow easy
specification of alternate pathnames for these files (possible 
through
TET configuration variables), thus improving the customisability of
their suites.
.P
Note that test cases built to this API do not require the TCC to
execute.  With careful setup of the communication variables 
(\(sc4.1.4)
test cases can be invoked from the command line (\(sc5.4).
.I "[Requirement 4.1]"
.H 3 "Test Case Structure and Management Functions"
These functions support functionality described in Initialising and
Cleaning up Test Cases and Selecting Invocable Components and Test
Purposes (\(sc4.3.1 and \(sc4.3.2).
.H 4 "Synopsis"
.Sy "struct tet_testlist {"
.Cw "void\ (*testfunc)\ (void); int\ icref;\ };"
.Se
.Sy "struct tet_testlist tet_testlist[];"
.Se
.Sy "void (*tet_startup) (void);"
.Se
.Sy "void (*tet_cleanup) (void);"
.Se
.Sy "int tet_thistest;"
.Se
.Sy "int tet_nosigreset;"
.Se
.Sy "char *tet_pname;"
.Se
.H 4 "Description"
The
.Cw tet_testlist
array declares, in the
.Cw testfunc
element of the
structure, the pointer to the function that is associated with each 
test purpose
and, in the
.Cw icref
element of the structure, the relationship of the
test purpose to an invocable component. The
.Cw tet_testlist
array is
terminated by a structure with the
.Cw testfunc
element set to NULL. No other
element of the array will use the value NULL for this element.
.P
For each requested invocable component, the Test Case Manager 
(TCM)
scans the
.Cw tet_testlist
array and executes, in order, each test
purpose that is associated with that invocable component. When
.B all
invocable
components are requested, the TCM executes all ICs for which 
entries
are defined in the 
.Cw tet_testlist
array, in ascending order.
In both cases the
TCM will calculate the number of test purposes that are to be 
executed
for each requested invocable component.
.P
The TCM will not perform any error checking on the contents of the
.Cw tet_testlist
array. It is the test author's responsibility to ensure that
the array is correctly specified.
.P
The function pointers
.Cw tet_startup
and
.Cw tet_cleanup
are set
to the functions to be used for Test Case specific start-up and clean 
up
procedures respectively. The start-up procedure is executed before 
the first requested invocable component and the clean-up procedure is
executed on completion of the last requested invocable component.
Note that if either of these pointers is set to 
.Cw NULL ,
the TCM will not attempt to call the respective function.
.P
The TCM will be provided as the
.Fn main
routine to the test case
program and will contain an external declaration to the
.Cw tet_startup
and
.Cw tet_cleanup
function pointers and to the
.Cw tet_testlist
array.
.P
The
.Cw tet_thistest
variable contains the sequence number (starting at 1) of the element 
in the
.Cw tet_testlist
array that is associated with the currently executing
test purpose. During execution of the start-up and clean-up functions, 
.Cw tet_thistest
is set to zero.
.P
The
.Cw tet_nosigreset
variable controls whether the TCM reinstates
signal handlers for unexpected signals before each test purpose.
The default value of zero means that signal handlers will be
reinstated before each test purpose, to ensure that unexpected signals
do not go unnoticed if an earlier test purpose installed a local
handler but did not restore the original handler.  If tet_nosigreset
is set to a non-zero value in the start-up function called via
tet_startup, then signal handlers will be left in place between test
purposes.  In test cases where stray signals constitute a test
failure, it is recommended that tet_nosigreset is left with its
default value of zero.  This is because even if test purposes
contain code to restore the signal handling, this code will not be
executed if an unexpected signal arrives and the TCM skips to the
start of the next test purpose.
.P
The
.Cw tet_pname
variable contains the process name as given on the test case command line.
.H 3 "Insulating from the Test Environment"
The following Configuration Variables are used by the C\ Language 
TCM to help
determine which events should be handled for the Test Case, and 
which
should be passed through. They are used by the TCM to support
functionality specified in Insulating from the Test Environment 
(\(sc4.3.3).
.VL .5i
.LI TET_SIG_IGN
.br
This variable defines (by comma-separated
number) the set of signals that are to be
ignored during test purpose execution. Any signal that is not set to 
be ignored
or to be left (see TET_SIG_LEAVE below) with its current 
disposition, will be
caught when raised and the result of the test purpose will be set to 
.Cw UNRESOLVED
because of the receipt of an unexpected signal. A test purpose may 
undertake its
own signal handling as required for the execution of that test 
purpose; the
disposition of signals will be reset after the test purpose has 
completed, unless the global variable
.Cw tet_nosigreset
is non-zero.
The TCM needs to know how many signals the implementation 
supports
in order to set up catching functions for these signals.
.LI TET_SIG_LEAVE
.br
This variable defines (by number) the set of signals that are to be 
left
unchanged during test execution. In most cases this will mean that 
the signal
takes its default action. However, the user can change the 
disposition of the
signal (to ignore) before executing the TCC if this signal is to 
remain
ignored during the execution of the test purposes.
.LE
.P
The implementation should not allow "standard" signals to be set to 
be ignored
or left unchanged,
as this may pervert test results.
.H 3 "Making Journal Entries"
These functions support functionality described in Making Journal
Entries (\(sc4.3.4).
.H 4 "Synopsis"
.Sy "void tet_setcontext(void);"
.Se
.Sy "void tet_setblock(void);"
.Se
.Sy "void tet_infoline(char *data);"
.Se
.Sy "void tet_result(int result);"
.Se
.H 4 "Description"
The
.Fn tet_setcontext
function sets the current context to the value of
the current process ID. 
A call to
.Fn tet_setcontext
should be
made by any application which executes a
.Fn fork
to create a new process
and which wishes to write entries from both processes. The call to
.Fn tet_setcontext
must be made from the child process, not from the
parent.
.P
The
.Fn tet_setblock
function increments the current block ID. The value
of the current block ID is reset to one at the start of every test 
purpose
or after a call to
.Fn tet_setcontext
which altered the current context.
The sequence ID of the next entry,
a number which is automatically incremented as each entry is output 
to the
execution results file,
is set to one at the start of each new block.
.P
The
.Fn tet_infoline
function outputs an information line to the execution
results file. 
The sequence number is
incremented by one after the line is output. If the current context 
and the
current block ID have not been set, the call to
.Fn tet_infoline
causes the
current context to be set to the value of the calling process ID and 
the current
block ID to be set to one.
.P
The
.Fn tet_result
function sets the result to
.Cw result .
This result is output to the execution results file by the TCM upon 
test purpose completion.  This ensures that all informational 
messages are written out before the test purpose result, and that 
there is one (and only one) result generated per test purpose.
.H 3 "Cancelling Test Purposes"
These functions support functionality described in Cancelling Test
Purposes (\(sc4.3.5).
.H 4 "Synopsis"
.Sy "void tet_delete(int test_no, char *reason);"
.Se
.Sy "char *tet_reason(int test_no);"
.Se
.H 4 "Description"
The function
.Fn tet_delete
marks the test purpose specified by
.Cw test_no
as cancelled and will output
.Cw reason
as the reason for
cancellation on the information line which is output whenever the 
test driver
attempts to execute this test purpose. The argument
.Cw test_no
is the sequence number (starting at 1) of the corresponding element 
in
the
.Cw tet_testlist[]
array.
If the requested
.Cw test_no
does not exist no action
is taken. If the requested
.Cw test_no
is already marked as cancelled the
reason is changed to
.Cw reason
and the test purpose remains marked as
cancelled. If the
.Cw reason
is set to
.Cw "(char\ *)\ NULL"
then the requested
.Cw test_no
is marked as active, this allows previously cancelled test
purposes to be re-activated.
If
.Cw reason
is not NULL it must point to static data, as the calling function will
have terminated when the reason string is accessed by the TCM.
This function cannot be called from a child process.
.P
The function
.Fn tet_reason
returns a pointer to a string which contains
the reason why the test purpose specified by
.Cw test_no
has been cancelled.
If this test purpose is not marked as cancelled, or does not exist,
a value of
.Cw "(char\ *)\ NULL"
is returned.
.H 3 "Manipulating Configuration Variables"
The functions in this section support functionality specified in
Manipulating Configuration Variables (\(sc4.3.6).
.H 4 "Synopsis"
.Sy "char *tet_getvar(char *name);"
.Se
.H 4 "Description"
The function
.Fn tet_getvar
retrieves the setting of the configuration variable
.Cw name 
and returns a pointer to that setting.  Implementations of this
function are assumed to be re-entrant.
.P
Note that if a variable is defined but has no value assigned,
.Fn tet_getvar
returns a pointer to an empty string.  If a requested variable is 
undefined,
.Fn tet_getvar
returns a NULL pointer.
.H 3 "Generating and Executing Processes"
These functions support the functionality described in Generating 
and
Executing Processes (\(sc4.3.7).
.H 4 "Synopsis"
.Sy "int tet_fork("
.Cw "void\ (*childproc)(void), void\ (*parentproc)(void),"
.Cw "int\ waittime, int\ validresults);"
.Se
.Sy "int tet_exec("
.Cw "char\ *file, char\ *argv[], char\ *envp[]);"
.Se
.Sy "extern pid_t tet_child;"
.Se
.H 4 "Description"
The
.Fn tet_fork
function will create a new process which is a copy of the
calling process and will modify the signal disposition in the newly 
created
process such that any signals that were being caught in the parent 
process
are now set to their default values in the child process. The child 
process
will then commence execution of the routine
.Fn childproc
and upon
completion of this routine will terminate with an exit code that 
indicates
to the parent the correctness of execution of this routine. If the 
routine
.Fn parentproc
is not set to NULL, this routine will be executed by the
parent process before the exit code of the child process is waited
for. On
completion of the
.Fn parentproc
routine, if any, the exit code returned
by the child process will be examined by masking off the bits which
are set in
.Cw validresults .  
If the result is zero, 
.Fn tet_fork
assumes that
this was a legal (or expected) termination code.  If not, it assumes
that the child process completed with an unexpected result and an
error has occurred.
This unexpected result is reported to the execution results file.
The
.Fn tet_fork
interface will return -1 if the result of the child
process was invalid or the valid result code if the result of the child
process was one of the valid results. When
.Fn tet_fork
returns -1 it reports the nature of the error using 
.Fn tet_infoline
and sets the test result code to
.Cw UNRESOLVED
by calling
.Fn tet_result .
If
.Cw waittime
is not set to zero,
the parent process will ensure that the child process does not 
continue to
execute for more than
.Cw waittime
seconds after the completion of the
routine
.Cw parentproc .
.P
The
.Fn tet_exec
function may be called from a
.Fn childproc
routine of
a child process generated by a call to
.Fn tet_fork .
The
.Fn tet_exec
function will pass the argument data as specified by
.Cw argv
and the
environment data specified by
.Cw envp
to the process specified by
.Cw file .
The usage of the
.Fn tet_exec
is equivalent to that of the ISO 9945-1
.Fn execve
function, except that the API can add arguments and environment
data that are to be interpreted by the driver of the executed
.Cw file .
If 
.Fn tet_exec
is called without first calling 
.Fn tet_fork ,
the results are undefined. The
.Fn tet_fork
function makes calls to 
.Fn tet_setcontext
in the child and
.Fn tet_setblock
in the parent to distinguish output from the child and from the 
parent
before, during, and after execution of the 
.Cw parentproc
routine.
.P
The global variable
.Cw tet_child
is provided for use in the
.Cw parentproc
routine called from 
.Fn tet_fork .
It is set to the process ID of the child.
.H 3 "Executed Process Functions"
These functions are to be used by processes executed through the
.Fn tet_exec
function.  They support functionality described in Generating and
Executing Processes (\(sc4.3.7).
.H 4 "Synopsis"
.Sy "int tet_main (int argc, char *argv[]);"
.Se
.Sy "int tet_thistest;"
.Se
.Sy "char *tet_pname;"
.Se
.H 4 "Description"
The function
.Fn tet_main ,
supplied by the test suite developer, is called by the
.Fn main
function of the TET-supplied child process controller
.Fi tcmchild.o .
Prior to calling 
.Fn tet_main ,
.Fi tcmchild.o
will set the
.Cw tet_thistest
variable to the number associated with the test purpose in the 
original test
case.
This value should not be changed by the executed process.
Upon completion of the
.Fn tet_main
routine the return value from this function will be returned to the 
process which called the
.Fn tet_fork
function. The value returned from
.Fn tet_main
should, in most cases, match one of the valid result values
specified on the call to the
.Fn tet_fork
function. The current context is preserved from the calling process
and the current block is incremented by one before
.Fn tet_main
is called.
.P
The
.Cw tet_pname
variable contains the process name as given in argv[0].
.H 2 "Shell Language Bindings"
The interfaces in this section support all the functionality required
in \(sc4.3 of this document. Implementations supporting this 
language
binding must support the command shell
.Cw sh
as defined in the X/Open Portability Guide Issue 3 Volume 1.
In such implementations, the communication variables are placed in 
the
Environment.
.P
Support for this language binding is provided through two shell
language source files.  The file
.Cw TET_ROOT/lib/xpg3sh/tcm.sh
contains 
the support routines for the sequencing and control of invocable
components and test purposes (the shell TCM).  The file 
.Cw TET_ROOT/lib/xpg3sh/tetapi.sh
contains the support routines for test purposes (the shell API).  
These files
must be "sourced" into an executing shell language test purpose 
using
the 
.B dot
.Cw "( . )"
shell built-in command.  Sourcing the shell TCM also automatically
sources the shell API.
.P
Note that test cases built to this API do not require the TCC to
execute.  With careful setup of the communication variables 
\(sc4.1.4)
the test case can be invoked from the command line (\(sc5.4).
.I "[Requirement 4.1]"
.H 3 "Test Case Structure and Management Functions"
These functions support functionality described in Initialising and
Cleaning up Test Cases and Selecting Invocable Components and Test
Purposes (\(sc4.3.1 and \(sc4.3.2).
.H 4 "Synopsis"
.Sy "iclist={blank-separated list of invocable components}"
.Se
.Sy "{icname}={blank-separated list of test purposes}"
.Se
.Sy "tet_startup={start-up function}"
.Se
.Sy "tet_cleanup={clean-up function}"
.Se
.Sy "tet_thistest"
.Se
.H 4 "Description"
The start-up routine, clean-up routine and each of the test purposes 
are to be
implemented by the test author as either shell functions or as 
separate
executable shell scripts. These shell functions or scripts will be 
called
by the shell TCM according to the requested set of invocable 
components.
The
.Cw iclist
is provided by the test suite author, and contains a blank-separated 
list of invocable component names.  These invocable component 
names are formed by
prefixing each invocable component
number with the letters
.BR ic .
.P
When an invocable component is requested by the TCC, the shell TCM
executes each name in the associated list of test purposes. Each of 
the test
purposes will be executed in a sub-shell with the appropriate signal 
handling
being applied to the sub-shell. 
.P
The TCM will not perform any explicit error checking on the contents 
of
a list of test purposes. It is the responsibility of the test author to 
ensure
that the names reference shell functions or executable shell scripts.
.P
The shell variables
.Cw tet_startup
and
.Cw tet_cleanup
are set to refer to
the shell function or script to be used for Test Case specific start 
up
and clean-up
procedures, respectively. The start-up procedure is executed before 
the first requested invocable component and the clean-up procedure is
executed on completion of the last requested invocable component.
If no start-up or clean-up is required, the 
.Cw tet_startup
and
.Cw tet_cleanup
variables may be left unset or set to an empty string.
.P
The TCM and API are provided as shell scripts which must be sourced by 
the
test suite author immediately after the shell 
variables 
for
.Cw tet_startup ,
.Cw tet_cleanup ,
.Cw iclist ,
each of the
.Cw icname s,
and any shell functions
have been defined. (The shell
script is sourced by use of the
.B dot
.Cw "( . )"
shell built-in command).
Note that if a test purpose is written as a separate shell script, that
script must source the shell API in order to have access to those
support routines.
.P
The
.Cw tet_thistest
shell variable contains the name of the test purpose, as specified in
the
.Cw icname
variable.
.H 3 "Insulating from the Test Environment"
The following Configuration Variables are used by the Shell TCM to 
help
determine which events should be handled for the Test Case, and 
which
should be passed through. They are used by the TCM to support
functionality specified in Insulating from the Test Environment 
\(sc4.3.3).
.VL .5i
.LI TET_SIG_IGN
.br
This variable defines (by comma-separated
number) the set of signals that are to be
ignored during test purpose execution. Any signal that is not set to 
be ignored
or to be left (see TET_SIG_LEAVE below) with its current 
disposition, will be
caught when raised and the result of the test purpose will be set to 
.Cw UNRESOLVED
because of the receipt of an unexpected signal. A test purpose may 
undertake its
own signal handling as required for the execution of that test 
purpose, the
disposition of signals will be reset after the test purpose has 
completed.
The API needs to know how many signals the implementation 
supports
in order to set up trap statements for these signals.
.LI TET_SIG_LEAVE
.br
This variable defines (by number) the set of signals that are to be 
left
unchanged during test execution. In most cases this will mean that 
the signal
takes its default action. However, the user can change the 
disposition of the
signal (to ignore) before executing the TCC if this signal is to 
remain
ignored during the execution of the test purposes.
.LE
.P
The implementation should not allow "standard" signals to be set to 
be ignored
or left unchanged,
as this may pervert test results.
.H 3 "Making Journal Entries"
The functions in this section implement functionality detailed in 
Making
Journal Entries (\(sc4.3.4).
.H 4 "Synopsis"
.Sy "tet_setcontext"
.Se
.Sy "tet_setblock"
.Se
.Sy "tet_infoline data ..."
.Se
.Sy "tet_result result"
.Se
.H 4 "Description"
The
.Cw tet_setcontext
shell function sets the current context to the value of
the process ID of the calling shell. The current context is stored in 
the
environment variable TET_CONTEXT which is marked for export. This 
allows the
context to be passed to subsequent processes by using this 
environment variable.
The
.Cw tet_setcontext
command should be executed by any application which executes
a background subshell and which wishes to write entries from both 
processes.
The
.Cw tet_setcontext
command must be executed from the child process, not from
the parent.  The parent should call
.Cw tet_setblock
as appropriate to distinguish
its output before, during and after execution of the child.
.P
The
.Cw tet_setblock
shell function increments the current block ID. The value
of the current block ID is reset to one at the start of every test 
purpose
and after a call to
.Cw tet_setcontext
which altered the current context.
The sequence ID of the next entry
is set to one at the start of each new block.
The current block ID is
stored in the variable TET_BLOCK which is marked for export.
.P
The
.Cw tet_infoline
shell function outputs an information line to
the execution
results file. The sequence 
number is
incremented by one after the line is output. If the current context 
and the
current block ID have not been set, the call to
.Cw tet_infoline
causes the
current context to be set using the value of the calling process ID 
and
the current block ID to be set to one.
Note that 
.Cw tet_infoline
does not process backslash escapes like the shell
.Cw echo
built-in command. If more than one argument is passed to
.Cw tet_infoline ,
then they will be output separated by a space character.
.P
The
.Cw tet_result
shell function outputs a test result to the
execution results
file. This result is output to the execution results file by the TCM 
upon 
test purpose completion.  This ensures that all informational 
messages are written out before the test purpose result, and that 
there is one (and only one) result generated per test purpose.
.H 3 "Cancelling Test Purposes"
These functions support functionality described in Cancelling Test
Purposes (\(sc4.3.5).
.H 4 "Synopsis"
.Sy "tet_delete test_name reason"
.Se
.Sy "tet_reason test_name"
.Se
.H 4 "Description"
The shell function
.Cw tet_delete
marks the test purpose specified by
.Cw test_name
as cancelled and will output
.Cw reason
as the reason for
cancellation on the information line which is output whenever the 
test driver
attempts to execute this test purpose. The argument
.Cw test_name
matches
the name which is used to call this test purpose. If the requested
.Cw test_name
does not match the name of a test purpose no action
is taken. If the requested
.Cw test_name
is already marked as cancelled the
reason is changed to
.Cw reason
and the test purpose remains marked as
cancelled. If
.Cw reason
is an empty string then the requested
.Cw test_name
is marked as active, this allows previously cancelled test purposes 
to be
re-activated.
.P
The shell function
.Cw tet_reason
outputs, to the standard output device,
the reason why the test purpose specified by
.Cw test_name
has been cancelled
and exits with an exit code of 0.
If this test purpose is not marked as cancelled, or does not match the 
name
of a test purpose, there is no output, to the standard output device, 
and the
function exits with an exit code of 1.
.H 3 "Manipulating Configuration Variables"
There is no explicit Shell interface to support this functionality. 
The API will ensure that
the configuration information is available to the test purposes as 
shell
variables marked 
.B readonly .
Each of these shell variables can
be accessed using the normal shell mechanisms.
.H 3 "Generation and Execution of Processes"
There is no explicit Shell interface. The API ensures that the
configuration information and the
.Cw tet_thistest
shell variable are
available as shell variables. The ability to use parentheses to 
generate
a subshell environment allows these variables to be inherited when 
a subshell
is generated. The only facilities that are not provided in the shell 
are the
ability to timeout a subshell process and the examination of the exit 
code
from the subshell. The shell provides facilities to accomplish these 
tasks
in a relatively straightforward manner and this is considered to be 
an issue
for the application programmer rather than for the API.
.H 3 "Executed Process Support"
Shell scripts which are executed by a test case written to this API
should source (using the 
.B dot
.Cw "( . )"
shell built-in command)
the shell API to include the necessary support routines.
Note that this will not provide TCM functions (like
signal handling and test purpose sequencing). Executed processes
which need this type of support should be test cases in their own
right.
