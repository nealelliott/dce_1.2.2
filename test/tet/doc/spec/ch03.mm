'\"	SCCS: @(#) TET SPECIFICATION		Rel 4.5 (06/23/92)
'\"
.nr H1 2
.H 1 "Architecture"
The architecture of the Test Environment Toolkit (TET) provides
simplicity and portability,
which will lead to transportability of suites. The structure of TET is
intuitive, and the required tools have consistent structure and
interfaces.
.I "[Requirement 1.3]"
The architecture can be described in terms of four objects:
.BL
.LI
The Terminology, which loosely defines the elements of the TET
conceptual model.
.LI
The Conceptual Model, which shows how elements in the TET
structure link together.
.LI
The System Requirements, which define minimum system facilities necessary
to support an implementation of the TET functionality.
.LI
The Test Suite structural requirements, which show how a Test Suite
must be constructed to function within a conforming TET
implementation.
.LE
.P
The remainder of this section describes these four objects.
.H 2 "Terminology"
The following terms are used throughout this document.
.VL .5i
.LI "Application Programming Interface"
.br
An application programming interface (API) is the set
of software interfaces between an application and the
system. In the case of TET, the API provides a Test Case Manager (TCM), which
provides an interface between the test case and the test case controller,
and an API library, which provides facilities for use by test cases.
.LI
Buildfile
.br
A buildfile is a set of test suite defined instructions to the
buildtool for use in constructing executable versions of test cases.
.LI
Buildtool
.br
The test case controller uses a test suite defined buildtool to control
the construction of
executable versions of test cases. This buildtool uses a
test suite defined buildfile as instructions for building each test
case. In order for configuration variables to be available to the buildtool 
it must use the TET API.
.LI
Build Fail Tool
.br
A test suite defined utility which is executed by the test case controller
when the buildtool indicates that a test case build has failed.
.LI
Cleanfile
.br
A cleanfile is a set of test suite defined instructions to the
cleantool for use in removing executable versions of test cases and
associated files.
.LI
Cleantool
.br
The test case controller uses a test suite defined cleantool to
control the removal of
executable versions of test cases and associated files. This cleantool uses a
test suite defined cleanfile as instructions for cleaning up each test
case. In order for configuration variables to be available to the cleantool
it must use the TET API.
.LI "Communication Variables"
.br
The TCC uses communication variables to provide information to the
buildtool, cleantool, and test cases during execution.  On some
implementations communication variables may share namespace with
information outside of TET.  In these cases implementors are
responsible for ensuring that users and test suite authors are
informed of the potential for collision.
.LI "Configuration Variables"
.br
The TCC and API use configuration variables to change operation
behaviour. Configuration variables are set via configuration variable
files (\(sc7.1.1) and via the TCC user interface.  Configuration
variables should exist within a namespace private to an instance of
TET.  TET-defined configuration variable names all begin with
the sequence "TET".  Test suite authors are cautioned to use obvious
and consistent naming conventions to avoid potential conflicts with
other objects in the namespace.
.LI
Execfile
.br
An execfile is a set of test suite defined instructions
for use in executing test cases under control of an exectool.
.LI
Exectool
.br
A tool used for executing test cases under special control, e.g. a
debugger.  Normally no exectool is specified, which means test cases
are executed directly.
.LI "Execution Results"
.br
A file into which test cases, via the API, place information
regarding their execution. This information is later incorporated
into the journal by the TCC.
.LI "Invocable Component"
.br
An invocable component is the smallest unit that the
TCC can execute individually. Invocable
components are made up of one or more test purposes.
.LI "Journal"
.br
A journal is the file into which test results and tracking data are
deposited by the TCC.
.LI "Result Code"
.br
A result code is the determination made by a test case about a test
purpose. TET supports the result codes defined by IEEE Std
1003.3-1991, as well as additional, test suite defined results.
Results are recorded via the API, and can be analysed by the report
generator and appropriate treatment filters.
.LI "Test Case"
.br
A test case is the software which conducts tests.
The scope of the term "test" is broad. It
may range from a single test purpose for a single function
being tested, all the way to a complete suite of
conformance tests for a specification.
Test cases are made up of one or more invocable components.
.LI "Test Case Controller"
.br
A test case controller (TCC) is the tool that provides structure
and control for test cases.
The TCC builds test cases when invoked in build mode, removes them in
clean mode, and executes the invocable components within test cases when
in execute mode.
The tool handles such functions as sequencing of test cases,
parameter passing, and journal file constructs.
.LI "Test Case Manager"
.br
The test case manager is a component of the API. This component interfaces
between the test case controller and the test suite defined parts of
the test case.  It provides support for the automatic sequencing of test
purposes and invocable components, as well as insulation from the testing
environment.
.LI "Test Purpose"
.br
A test purpose is the software which tests that statement which
is the smallest level of granularity of a test
specification. A test purpose always leads to a single
result. In the case of a POSIX.3 conforming test suite, for example,
test purposes would correspond to assertions.
.LI "Test Scenario"
.br
A test scenario is a sequence of one or more invocable components
associated with a single user-exposed name. There may be multiple
test scenarios per scenario file.
Test scenarios are used by the TCC to translate user
requests into actions.
.I "[Requirement 3.1]"
.LI "Test Suite"
.br
A test suite is a set of test cases and other required and optional
files. A single test case is a minimal
example of a test suite.
.LE
.H 2 "Conceptual Model"
The TET architecture consists of a number of
functions and data files.
The functions do not necessarily relate to specific executables in the
implementation.
Neither do the data files necessarily
relate to specific files. These are considered to be implementation
details which are beyond the scope of the architecture description. This
description is intended to be platform-independent within the resource
requirements detailed in section 3.4.
.I "[Requirements 1.1 & 1.6]"
.P
The conceptual model chosen consists of six functions and ten data files 
that together describe TET.
.SP
.DS CB
.ce
\s+2\fBTET Conceptual Model\fP\s0
.SP 
.PS
	scale=1.25
	boxwid = 1.5i; boxht = 0.75i
	ellipsewid = 1.5i; ellipseht = 0.75i
	movewid = 0.0i
.\"
.\" Start with the Buildfile ellipse at the top, and slightly
.\" right of centre then work down that 'column'
.\"
	move right 4.0i
BLDF:	ellipse "Buildfile"
BLDTL:	box "\s+2\fBBuildtool\fP\s0" with .c at BLDF.c + (0.0i, -1.0i)
CLNTL:	box "\s+2\fBCleantool\fP\s0" with .c at BLDF.c + (0.0i, -2.0i)
.\"	ellipsewid = 1.0i; ellipseht = 0.5i
CLNF:	ellipse "Cleanfile" with .c at BLDF.c + (0.0i, -3.0i)
EXECRS:	ellipse "Execution" "Results" with .c at BLDF.c + (0.0i, -5.0i)
RESCOD:	ellipse "Result" "Codes" with .c at BLDF.c + (0.0i, -6.0i)
	ellipsewid = 1.5i; ellipseht = 0.75i
.\"
.\" Configuration files and the scenario file ellipses are on the left,
.\" clustered around the TCC box
.\"
BCONF:	ellipse "Build" "Configuration" with .c at BLDF.c + (-2.0i, -1.5i)
CCONF:	ellipse "Clean" "Configuration" with .c at BLDF.c + (-3.5i, -2.5i)
ECONF:	ellipse "Execution" "Configuration" with .c at BLDF.c + (-4.0i, -4.0i)
SCEN:	ellipse "Scenario" with .c at BLDF.c + (-3.5i, -5.0i)
JNL:	ellipse "Journal" with .c at BLDF.c + (-2.0i, -6.0)
TCC:	box "\s+2\fBTest Case\fP\s0" "\s+2\fBController\fP\s0" with .c at BLDF.c + (-2.0i, -4.0i)
.\"
.\" Report-related boxes are positioned relative to the Journal ellipse
.\"
RPTGEN:	box "\s+2\fBReport\fP\s0" "\s+2\fBGenerator\fP\s0" with .c at JNL.c + (0.0i, -1.0i)
TRTMNT:	box "\s+2\fBTreatment\fP\s0" "\s+2\fBFilter\fP\s0"with .c at RPTGEN.c + (2.0i, 0.0i)
FMTRPT:	ellipse "Formatted" "Report" with .c at TRTMNT.c + (2.0i, 0.0i)
.\"
.\" Test Case boxes structure is out on the extreme right.
.\" Simulate 3 boxes 'stacked' at a slight offset by having two of
.\" them invisible and drawing lines along part of their edges.
.\"
TCASES:	box "\s+2\fBTest Case\fP\s0" with .c at TCC.c + (4.2i, 0.0i)
TC1:	box invis with .c at TCASES.c + (0.25i, -0.25i)
TC2:	box invis with .c at TC1.c + (0.25i, -0.25i)
	line from TC1.ne left 0.25i
	line from TC1.ne to TC1.se
	line from TC1.se to TC1.sw
	line from TC1.sw up 0.25i
	line from TC2.ne left 0.25i
	line from TC2.ne to TC2.se
	line from TC2.se to TC2.sw
	line from TC2.sw up 0.25i
.\"
.\" Then draw the connecting arrows between the boxes and ellipses
.\"
	arrow from BLDF.s to BLDTL.n
	line <-> from BLDTL.w to 1/2 of the way between TCC.n and TCC.ne
	line <-> from CLNTL.w to TCC.ne
	arrow from CLNF.n to CLNTL.s
	arrow from BCONF.s to TCC.n
	arrow from CCONF.se to TCC.nw
	arrow from ECONF.e to TCC.w
	arrow from SCEN.ne to TCC.sw
	arrow from TCC.s to JNL.n
	arrow from EXECRS.w to TCC.se
	arrow from RESCOD.nw to 1/2 of the way between TCC.s and TCC.se
	arrow from TCC.e to TCASES.w
	arrow dashed from BLDTL.e to TCASES.n "\f2Produces\f1" 
	arrow dashed from CLNTL.e to TCASES.nw "\f2Removes\f1" 
	arrow from TCASES.sw to EXECRS.e
	arrow from JNL.s to RPTGEN.n
	arrow from RPTGEN.e to TRTMNT.w
	arrow from TRTMNT.e to FMTRPT.w
.\"
.\" Finally reset box dimensions to their defaults
.\"
	boxwid = 0.75i; boxht = 0.5i
	movwid = 0.5i
.PE
.DE
.P
Each of the functions and data files indicated above is described in
detail in the following sections.
.H 2 "Structural Description"
The following diagram shows how the various components of TET
and test suites relate to one another:
.SP
.DS CB
.ce
\s+2\fBTET Directory Structure\fP\s0
.SP 
.S 10
.PS
scale=1.0
boxht=0.2
movewid=0.2
down
L1: [
	right
	A: box invis "\f7TET_ROOT\fR"
]
move
L2: [
	right
	A: box invis "\f7bin\fR"
	move
	B: box invis "\f7src\fR"
	move right 0.525
	C: box invis "\f7inc\fR"
	move
	E: box invis "\f7lib\fR"
	D: [
		down
		A: box invis "\fItest suite\fR\|.\|.\|."
		move down 0.125
		B: box wid 0.375 ht 0.375 invis "\s-2continued in\s0" "\s-2section 3.5\s0"
		line from A.s to B.n
		line from A.s to B.nw
		line from A.s to B.ne
	] with .nw at last box.ne + (0.525, 0)
]
move down 0.125
L4: [
	right
	move right 0.475
	A: box invis "\f7posix_c\fR"
	move right 0.675
	B: box invis "\f7posix_c\fR"
	move
	C: box invis "\f7tet_jrnl.h\fR"
	move
	D: box invis "\f7posix_c\fR"
	move
	box invis
]
move
L5: [
	right
	A: box invis "\f7api\fR"
	move
	B: box invis "\f7tcc\fR"
	move
	C: box invis "\f7tet_api.h\fR"
	move
	D: box invis "\f7tcm.o\fR"
	move
	E: box invis "\f7tcmchild.o\fR"
	move
	F: box invis "\f7libapi.a\fR"
]

line from L1.A.s to L2.A.n
line from L1.A.s to L2.B.n
line from L1.A.s to L2.C.n
line from L1.A.s to L2.D.n
line from L1.A.s to L2.E.n

line from L2.B.s to L4.A.n
line from L2.C.s to L4.B.n
line from L2.C.s to L4.C.n
line from L2.E.s to L4.D.n

line from L4.A.s to L5.A.n
line from L4.A.s to L5.B.n
line from L4.B.s to L5.C.n
line from L4.D.s to L5.D.n
line from L4.D.s to L5.E.n
line from L4.D.s to L5.F.n
.PE
.S
.DE
.H 2 "System Requirements"
In order for a system to support the toolkit architecture, the
following facilities must be available.
.VL .5i
.LI "Execution of Discrete Components"
.br
Systems must allow applications to execute other applications. These
applications must have a hierarchical relationship such that an
application which launches another can have some control over the
other's execution. Further, there must be a mechanism for
uniquely identifying each application concurrently executing in the
system.
.LI "Files and Directories of Files"
.br
TET software requires a system with the ability to read and
write data resources ("files"). These files must be
structured in such a way that directories of files can be separated
from other directories. These directories must also be
combinable into other directories, with the memberships of the various
groupings maintained. Files within each directory are
independently accessible.
.LI "Resource Protection Mechanism"
.br
Support to ensure that a specified resource is protected from access
by other processes executing concurrently.
.LI "Atomic File Writes"
.br
The system must permit atomic writes of at least 512 bytes to files.
.LI "Options/Operands"
.br
The system must provide a method of providing applications with
user-specified parameters. The order of user-specified parameters must be
maintained.
.LI "Date/Time Facility"
.br
The system must provide a way for an application to determine the date
and time of day.  While there is no absolute requirement on granularity of
such a timer, one second granularity is advisable.
.LI "Completion Status"
.br
Upon termination of an application, the system must provide a way of
delivering a status of that termination to the application which controls
the terminated application.  Further, the system must have a concept
of "normal" completion.  TET relies upon "normal" and "abnormal"
execution completion in some modes (\(sc4.1).
.LE
.H 3 "Future Directions"
Proposed future additions to TET will require systems to provide the
following facilities.
.VL .5i
.LI "Internationalisation"
.br
A future revision of the specification will require that TET
implementations support the concept of internationalisation. This
includes the use of Message Libraries, Locale-dependent behaviour, and
wide-character based string manipulation.  Systems will need to
provide the relevant facilities.
.I
[Requirement 2.3]
.R
.LE
.H 2 "Test Suite Structure"
The Toolkit attempts to impose minimum structural requirements on test
suites. However, in order to allow the maximum flexibility 
of those suites, some specific files and utilities must be present.  A rough 
representation of this structure is:
.SP
.ce
\s+2\fBTest Suite Directory Structure\fP\s0
.SP 
.DS CB
.S 10
.PS
scale=1.0
boxht=0.2
movewid=0.2
down
L1: [
	right
	A: box invis "\fItest suite\fR"
]
move
L2: [
	right
	B: box invis "\f7doc\fR"
	move
	A: box invis "\f7bin\fR"
	move
	C: box invis "\f7results\fR"
]
move
L3: [
	right
	A: box invis "\f7install\fR"
	move
	B: box invis "\fIbuild tool\fR"
	move
	C: box invis "\fIclean tool\fR"
	move
	D: box invis "\fIbuild fail tool\fR"
	move
	E: box invis "\fItreatment filter\fR\|.\|.\|."
]

line from L1.A.s to L2.A.n
line from L1.A.s to L2.B.n
line from L1.A.s to L2.C.n

line from L2.A.s to L3.A.n
line from L2.A.s to L3.B.n
line from L2.A.s to L3.C.n
line from L2.A.s to L3.D.n
line from L2.A.s to L3.E.n

.PE
.S
.DE
.H 3 "Required Files"
.VL .5i
.LI "Execution Results Files"
.br
As tests are executed, their results are accumulated in execution
results files.  The TCC processes those files upon completion of test
case execution.  Execution results must be placed in a file called
.Fi tet_xres
in the same directory as test case execution took place. The format of
this file is defined in Journal Files (\(sc7.2.1).
.LI "Result Code Definitions"
.br
The result code definitions file contains the test suite specific
definitions for possible test purpose results. This file is used by
the API and test case controller when interpreting test purpose
results. The format of this file is defined in Result Code Definitions 
(\(sc7.1.3).
.LI "Configuration Variable Settings"
.br
TET has several configuration variables which allow test suites to
customise the way in which the test case controller operates. 
Test suites may define additional configuration variables as
needed. Files containing the test suite settings for these
variables are used by the API and test case controller when building,
executing and cleaning up test cases. The format of these files is
defined in Configuration Variable Files (\(sc7.1.1).
.LI "Test Scenario Definitions"
.br
Each test suite must provide a test Scenario definition file. This
file (by default named
.Fi tet_scen )
contains sequences of invocable components and a related, single
name that can be used by the TCC and Toolkit users
when executing the test suite. As a minimum a scenario named
.B all
must be provided. This scenario should execute all invocable components in 
all test cases in the test suite. The format of a scenario file is
defined in Scenario Files (\(sc7.1.2).
.LI "Tools Directory"
.br
Each test suite must have a directory in its top level called
.Fi bin .
This directory contains tools that are used by the test suite and
the TCC.
.LI "Results Directory"
.br
Each test suite has within it a directory called
.Fi results .
The TCC creates a unique subdirectory in this directory on each run,
into which it places the journal file and a hierarchy of files requested
to be saved by the user.
.LI "Documentation Directory"
.br
Each test suite is required to provide a directory in its top level
called
.Fi doc .
This directory contains appropriate documentation for the test suite.
.LE
.H 3 "Required Utilities"
Since TET cannot anticipate the internal structure of test suites,
it uses test-suite-supplied utilities to perform some tasks. In
particular, TET requires that the test suite provide utilities to
assist in installing the test suite.
.H 3 "Optional Utilities"
Test Suites may provide some additional tools for TET users.
.VL .5i
.LI "Build Tool"
.br
This utility is executed by the TCC to build each test case.
The build tool is invoked in the source directory of the test case.
If the communication parameter TET_EXECUTE is set, it is the responsibility
of the build tool to install the relevant files under the specified
alternate execution directory.
.LI "Clean Tool"
.br
This utility is executed by the TCC to clean each test case.
The clean tool is invoked in the source directory of the test case.
If the communication parameter TET_EXECUTE is set, it is the responsibility
of the clean tool to remove the relevant files from under the specified
alternate execution directory.
.LI "Build Fail Tool"
.br
This utility is optionally activated by the TCC when the buildtool
indicates that a test case build failed. The build fail tool is
invoked in the directory where the build took place
with the name of the test case as an argument. One possible
use for such a utility would be to provide a skeleton test case which
merely indicated that the test case was not built successfully when
executed.
.LI "Treatment Filters"
.br
Test suite authors may wish to provide treatment filters that produce
reports in specific formats. The format of the journal file is
defined in Journal Files (\(sc7.2.1).
.LE
.H 2 "Test Case Structure"
Test cases are constructed by grouping together routines which test
specific system features.
These routines take advantage of support functions provided
by the TET API, and are called by the TET TCM.  This relationship is
best illustrated through the following picture:
.PS
	boxwid = 1.2i; boxht = .5i
	movewid = 0.0i
	move right 1.5i
TCM:	box "\s+2\fBTCM\fP\s0"
API:	box "\s+2\fBAPI\fP\s0" with .n at TCM.s
CASE:	box height 1i "\s+2\fBTest Purpose\fP\s0" with .nw at TCM.ne + (1.0i, 0.0i)
	arrow from TCM.e to CASE.w + (0.0i, 0.25i) "\f2Calls\f1"
	arrow from CASE.w + (0.0i, -0.25i) to API.e "\f2Calls\f1"
.PE
.P
This diagram shows that the TCM calls test purposes, which in turn
make calls into the API.  The API performs functions like fetching
configuration variable settings and writing messages into the journal.
By concentrating this functionality in the API, and by allowing the
TCM to control test sequencing, TET offers the test suite author
strong support for developing test suites.
.P
Test purposes within a test case may be grouped together into
invocable components.  In doing so, a test suite author can ensure
that a set of test purposes is always executed together and in the
correct sequence.  There is no limit to the number of test purposes
that can be in an invocable component, and there is no limit to the
number of invocable components that can be within a single test case.
.P
The advantage of this structure is that it allows the test suite
author a great deal of flexibility, while still presenting a consistent
interface for the test suite user.
.P
Details about the functionality provided by the TCM and API can be
found in the Application Programming Interface functional
specification (\(sc4.3).  Specific information about constructing test
cases in various programming languages and environments is available
in Application Programming Interfaces (\(sc6).
