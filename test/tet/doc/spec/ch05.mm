'\"	SCCS: @(#) TET SPECIFICATION		Rel 4.5 (06/23/92)
'\"
.nr H1 4
.nr Cl 2
.ds HP +4 +3 +2 +1 +0 +0 +0
.ds HF 3 3 3 3 3 1 1
.H 1 "User Interface Specifications"
This section defines the "command line" interfaces for tools 
provided in implementations of TET for various architectures.
This includes the user interface to the TCC, the
Report Generator, the Test Suite Installer and test cases written
using the TET API.
The functionality represented by these interfaces is defined in 
Functional
Specifications (\(sc4).
.H 2 "Test Case Controller"
The Test Case Controller provides support for the building, 
execution,
and clean-up of Test Scenarios.
.I "[Requirement 1.5]"
This interface is appropriate for systems which support a command
shell as defined in the X/Open Portability Guide Issue 3 Volume 1 
\(em
.Cw sh .
.H 3 Synopsis
.Sy "tcc"
.Op b
.Op e
.Op c
.O2 a Alt_exec_dir
.O2 f Clean_cfg_file
.O2 g Build_cfg_file
.O2 i Int_results_dir
.O2 j Journal_file
.O2 s Scenario_file
.O2 t Timeout
.O2 v Variable=value
.O2 x Exec_config_file
.br
.Bo
.Ar Test_suite
.Bo
.Ar Scenario
.Bc
.Bc
.Se
.P
.Sy "tcc"
.Op b
.Op e
.Op c
.Co
.A2 m Pattern
.Pi
.A2 r Pattern
.Cc
.O2 a Alt_exec_dir
.O2 f Clean_cfg_file
.O2 g Build_cfg_file
.O2 i Int_results_dir
.O2 j Journal_file
.O2 s Scenario_file
.O2 t Timeout
.O2 v Variable=value
.O2 x Exec_config_file
.Ar Old_journal_file
.Bo
.Ar Test_suite
.Bo
.Ar Scenario
.Bc
.Bc
.Se
.H 3 Description
The TCC has three modes of operation, which may be executed singly 
or
together. Each of these modes accepts options
as modifiers to that mode's behaviour. The mode (with optionally
modified behaviour) is then applied to the test cases and invocable
components referenced by the
.Ar Scenario . 
Unless otherwise specified, journal information and saved 
intermediate
result files are
placed within an
implementation-defined directory under the
.Fi results
directory of the test suite selected. The TCC writes the name of the
journal to standard output on startup.
.H 3 Options
.Cm tcc
supports the following modes.
.VL 6n
.L1 b
Enable build mode.
.L1 e
Enable execute mode.
.L1 c
Enable clean mode.
.LE
.P
These modes accept the following options.
.VL 6n
.L2 m Pattern
.br
Resume from a specified point in a previous run.  The argument
.Ar Pattern
specifies a comma-separated list of result code names and the letters
\fBb\fR, \fBe\fR and \fBc\fR.  Upon encountering an execution result
corresponding to one of the specified result code names, or where
\fBb\fR, \fBe\fR or \fBc\fR is specified, any build, execute or clean
failure respectively, in the
.Ar Old_journal_file ,
the associated invocable component and all subsequent invocable
components in the scenario are built, executed and/or cleaned up, as
appropriate.  E.g.:
.SP
.DS I
.ft 7
.ps -2
tcc -b -m b                 # resume building from IC which
                            # failed to build.
    
tcc -e -m FAIL,UNRESOLVED   # resume execution from IC which
                            # gave FAIL or UNRESOLVED result
                            # on execution.

tcc -bec -m b,e             # resume building, executing and
                            # cleaning from IC which failed
                            # to build or execute, or did not
                            # give a PASS result on execution.
.ps +2
.ft R
.DE
.L2 r Pattern
.br
Re-run specified invocable components from a previous run.  The argument
.Ar Pattern
specifies a comma-separated list of result code names and the letters
\fBb\fR, \fBe\fR and \fBc\fR.  Upon encountering an execution result
corresponding to one of the specified result code names, or where
\fBb\fR, \fBe\fR or \fBc\fR is specified, any build, execute or clean
failure respectively, in the
.Ar Old_journal_file ,
the associated invocable component is built, executed and/or cleaned
up, as appropriate.  E.g.:
.SP
.DS I
.ft 7
.ps -2
tcc -b -r b                 # re-build all ICs which failed
                            # to build.
    
tcc -e -r FAIL,UNRESOLVED   # re-execute all ICs which gave
                            # FAIL or UNRESOLVED result on
                            # execution.

tcc -bec -r b,e             # re-build, execute and clean 
                            # all ICs which failed to build
                            # or execute, or did not give a
                            # PASS result on execution.
.ps +2
.ft R
.DE
.L2 a Alt_exec_dir
.br
The argument
.Ar Alt_exec_dir
specifies the pathname of the root of a directory hierarchy in which
executable test cases should be placed (by the build tool 
in build mode), executed 
from
(in execute mode), and cleaned from (by the clean tool
in clean mode). If unspecified,
the default behaviour is to execute test cases from the test suite 
hierarchy.
.L2 f Clean_cfg_file
.br
The argument
.Ar Clean_cfg_file
specifies the pathname of the file of configuration variables to use
in clean mode. If unspecified this value defaults to
.Fi tetclean.cfg
in the root directory of the test suite selected.
.L2 g Build_cfg_file
.br
The argument
.Ar Build_cfg_file
specifies the pathname of the file of configuration variables to use
in build mode. If unspecified this value defaults to
.Fi tetbuild.cfg
in the root directory of the test suite selected.
.L2 i Int_results_dir
.br
The argument
.Ar Int_results_dir
specifies the pathname of the root of a directory hierarchy to store
intermediate result files (matching the configuration variable
TET_SAVE_FILES). If unspecified the intermediate results are saved
within a uniquely named directory hierarchy under the
.Fi results
directory of the test suite.
.L2 j Journal_file
.br
The argument
.Ar Journal_file
specifies the pathname of the file in which journal information
is to be recorded.  If unspecified, the journal information is stored
in a file named
.Fi journal
in the root of the intermediate results hierarchy.
.L2 s Scenario_file
.br
The argument
.Ar Scenario_file
specifies the pathname of a scenario file. If unspecified this value
defaults to
.Fi tet_scen
in the root directory of the test suite selected.
.L2 t Timeout
.br
The argument
.Ar Timeout
specifies the number of seconds to allow a single build, execute, or
clean of a test case to continue. If unspecified, the TCC imposes no time
limit.
.L2 v Variable=value
.br
The argument
.Ar Variable=value
is used to set the configuration variable
.Ar Variable
to
.Ar value .
Note that multiple variables may be manipulated by repeating this 
option with
different variables names.
.L2 x Exec_config_file
.br
The argument
.Ar Exec_config_file
specifies the pathname of the file of configuration variables to use
in execute mode. If unspecified this value defaults to
.Fi tetexec.cfg
in the alternate execution directory, or failing that,
.Fi tetexec.cfg
in the root directory of the test suite selected.
.LE
.H 3 Operands
.VL 6n
.La Old_journal_file
.br
This operand is only used with the resume and re-run options.  It
indicates the pathname of the journal file that is to be searched for
test failures meeting the criteria specified by the user.
.La Test_suite
.br
This operand indicates the suite to which the scenario must be
applied. The suite name is used by the TCC to determine the
suite root directory name.
If none is given on the command line, a default test suite may be deduced
by the TCC according to the following rules:
.AL
.LI
If the current directory lies under the TET_ROOT directory hierarchy,
the test suite is the component of the current directory's pathname
which lies immediately below the TET_ROOT;
.br
e.g. if the TET_ROOT is
.Cw /usr/TET
and the current directory is
.Cw /usr/TET/suite1/results/0017bec ,
then the default test suite is
.Cw suite1 .
.LI
If the current directory lies outside of the TET_ROOT directory hierarchy,
then no default may be deduced.
.LE
.La Scenario
.br
This operand indicates the Test Scenario that is to be processed
by the Test Case Controller. If none is given on the command line,
the default scenario is
.Cw all .
.LE
.H 2 "API conforming test case"
Test cases written to the TET API are usable as stand-alone
applications, although some set up of Communication and 
Configuration
variables may be required.  See sections 4.1.3 and 4.1.4 for more
details.
.H 3 "Synopsis"
.Sy test_case
.Ar [iclist]\ ...\(el
.Se
.H 3 "Description"
The 
.Cm test_case
is provided by a test suite author, and may reside anywhere in the
test suite directory tree.  Invocable components within the test case
are selected by number.
.H 3 "Options"
None.
.H 3 "Operands"
.VL 6n
.La iclist
.br
A list of invocable components to be executed.  This is either the
special keyword "all", or a comma-separated list of single invocable
component numbers or ranges of numbers separated by a dash (\-).
If no
.Ar iclist
is indicated, "all" is assumed.
If more than one
.Ar iclist
is supplied, they are processed in the order given.  If "all" occurs
as a second or subsequent operand, the behaviour is unspecified.
.LE
