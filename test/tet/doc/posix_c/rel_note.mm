.\" Format with: tbl | troff -mm
.de )k
..
.PH ""
.PF ""
.ds HP 14 12 12
.ds HF 3 3 3
.nr Hs 3
.nr Hb 7
.S 20
.SP 1.5i
.ce 99
.B "Test Environment Toolkit Release Notes"
.SP 1
.S
.S 12
.SP 2
Implementation for POSIX.1 Systems in `C' and XPG3 Shell
.br
by OSF, UNIX International and X/Open 
.SP
Release 1.10  (06/23/92)
.SP 4.5i
.ce 0
.SK
.SP 15
.P
.S 10
The information contained within this document is subject to change
without notice.
.P
.S 8 -1
OSF, UI, and X/Open MAKE NO WARRANTY OF ANY KIND WITH REGARD TO THIS MATERIAL,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO
EVENT SHALL OSF, UI or X/Open BE LIABLE FOR ANY SPECIAL, INDIRECT OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
.S 10 +1
.P
OSF, UI and X/Open shall not be liable for errors contained herein or
for incidental
consequential damages in connection with the furnishing, performance,
or use of this material.
.P
Copyright\(co 1990, Open Software Foundation, Inc. (OSF)
.br
Copyright\(co 1990, UNIX International (UI)
.br
Copyright\(co 1990, X/Open Company Limited (X/Open)
.P
Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted, provided
that the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation, and that the name of OSF, UI or X/Open not be used in
advertising or publicity pertaining to distribution of the software
without specific, written prior permission.  OSF, UI and X/Open make
no representations about the suitability of this software for any purpose.
It is provided "as is" without express or implied warranty.
.SP
Open Software Foundation, OSF and OSF/1
are trademarks of the Open Software Foundation, Inc.
.SP .5
X/Open is a trademark of the X/Open Company, Ltd. in the U.K. and
other countries.
.SP .5
UI is a trademark of UNIX International.
.SP .5
UNIX is a registered trademark of UNIX System Laboratories, Inc. in
the U.S. and other countries.
.SP
.\"************NEW TEXT - MANDATORY***********************************
.DS 2
.S 8
FOR U.S. GOVERNMENT CUSTOMERS REGARDING THIS DOCUMENTATION
AND THE ASSOCIATED SOFTWARE
.S
.DE
.DS 3
Notice: Notwithstanding any other lease or licence agreement that may
pertain to, or accompany the delivery of, this computer software, the rights
of the Government regarding its use, reproduction, and disclosure are as
set forth in the FAR Section 52.227-19 "Computer Software - Restricted
Rights."
.DE
.P
Unpublished - All rights reserved under the Copyright Laws of the United
States.
.P
This notice shall be marked on any reproduction of this data, in whole or in
part.
.\"*****************END NEW TEXT*********************************
.PH "'TET Release Notes''Release 1.10'"
.SK
.nr P 1
.nr % 1
.PF "''- \\\\nP -''"
.P
.S 11
.HU "Preface"
.P
These release notes briefly describe Release 1.10 of the
\fITest Environment Toolkit\fR implementation for POSIX.1 systems in `C'
and XPG3 shell by OSF, UI and X/Open.
They describe the prerequisites for running this toolkit, what is supplied
in the toolkit, and how to build the toolkit.  The release notes also
list the operating system platforms on which the Test Environment Toolkit has
been built and run.
.HU "Audience"
This document is written for software engineers who will be porting
their validation test suites into the Test Environment Toolkit.
.HU "Document Usage"
This document is organised into the following chapters.
.BL
.LI
Chapter 1 gives introductory information about the Test
Environment Toolkit.
.LI
Chapter 2 describes how to build the Test Environment Toolkit.
.LI
Chapter 3 describes known problems with this release.
.LI
Chapter 4 describes the changes made since the previous release.
.LI
Chapter 5 contains comments on specific areas of the specification.
.LI
Chapter 6 describes extensions to the specification.
.LE
.HU "Related Documents"
For additional information about the Test Environment Toolkit,
refer to the following document:
.BL
.LI 
\fITest Environment Toolkit: Architectural, Functional, and
Interface Specification, Version 4.5\fR
.LE
.HU "Typographic and Keying Conventions"
.P
This document uses the following typographic conventions: 
.VL \w@\f(CWTypewriter\ font\ \ \ fR@u
.LI "\fBBold\fR"
In formats and command descriptions, 
.B "bold"
words or characters represent commands or keywords 
that you must use literally, including pathnames.  
.\".P
.\"In text, 
.\".B "bold"
.\"words indicate the first use of a new 
.\"term.
.P
In examples, information that you enter appears in
.B "bold".
.LI "\fIItalics\fR"
.ft I
Italic words or characters in formats and command descriptions 
represent values that you must supply.
.ft R
.LI "\f(CWTypewriter font\fR"
.ft CW
Information that the system displays appears in 
this typeface.  Examples also appear in 
this typeface.
.ft R
.LE
.SP 
.HU "Problem Reporting"
.P
If you have problems with the TET
send electronic mail to:
.P
\fBtet_support@xopen.co.uk\fR
.P
Include the following information in your mailing:
.BL
.LI
Your name, company or organisation, and email address
.LI
Hardware you are running on
.LI
Operating system version TET is installed on
.LI
TET version number
.LI
Description of encountered problem, including how the problem occurred
and how it can be reproduced
.LI
A copy of the TET configuration file(s) for the affected modes
.LI
A copy of the TET journal file
.LI
A suggested fix for the problem, if possible
.LE
.P
A template to use for reporting problems by email is provided in the file
.BR doc/posix_c/err.template .
.SK
.H 1 "Introducing the Test Environment Toolkit"
.P
The purpose of the Test Environment Toolkit (TET) is to provide a
uniform framework, or test scaffold, into which test suites can be
incorporated.  By providing such a scaffold, test suites from
different vendors can share a common interface, allowing for, among
other things, ease of portability.  
.P
This implementation of the Test Environment Toolkit currently consists of:
.BL
.LI
A test case controller providing support for the building, execution,
and clean-up of test scenarios (\fBtcc\fR);
.LI
a `C' Application Programming Interface; and
.LI
a shell Application Programming Interface.
.LE
.P
Each Application Programming Interface (API) consists of a test case
manager and an API library, which are used in test cases to handle
interfacing between the test case and the test case controller.
.P
This release does not include the Programmers' Guide or Users' Guide.
These documents are currently being worked on by UNIX International,
and will be distributed separately when they are ready.
.P
A reference manual page is supplied for the \fBtcc\fR
in the directory \fItet_root\fB/doc/posix_c\fR.
Refer to Chapter 6 of the TET specification
for information on the Application Programming Interfaces.
.P
In addition to the TET, you will also receive a copy of a simple
demonstration TET test suite.
.SK
.H 1 "Building the Test Environment Toolkit
.P
This chapter describes how to set up your system and build the test
environment.
Prior to building the test environment, you should check
that the following requirements are met.  The test environment
will not build without them.
.P
.H 2 "Test Environment Toolkit Requirements"
The Test Environment Toolkit requires that your system's
operating system be compliant with the following:
.BL
.LI
POSIX.1\*F
.FS
IEEE Std 1003.1-1990, \fIPortable Operating System Interface for
Computer Environments\fR
.FE
with either standard `C' or common `C' support.
.LI
XPG3, Vol. 1\*F
.FS
\fIX/Open Portability Guide Issue 3, Volume 1: XSI Commands and Utilities\fR
.FE
(for the \fBshell, find, cp, rm,\fR and \fBmake\fR commands)
.LE
.P
As of this writing, the Test Environment Toolkit has only been
installed and tested on UNIX\*F
.FS
UNIX is a registered trademark of UNIX System Laboratories, Inc. in the U.S.
and other countries.
.FE
type of systems.  Note that this
does not preclude its installation on other types of operating
systems.
.P
The TET provides a series of makefiles that can be used to build the toolkit
in a UNIX type of environment.  If you are not running a UNIX type of 
operating system, you will have to develop your own equivalent method of 
building TET.
.P
.H 2 "Installed Platforms"
This release of the TET has been
installed and tested on the following operating system platforms (UNIX
type of operating systems).
.BL
.LI
HP-UX\*F
.FS
HP-UX is a registered trademark of Hewlett Packard Corporation.
.FE
version 7.0.
.LI
ULTRIX\*F
.FS
ULTRIX, DEC, and DIGITAL are a registered trademarks of Digital
Equipment Corporation.
.FE
version 4.2.
.LI
UNIX System V release 4.0
.LI
SunOS\*F
.FS
SunOS is a trademark of SUN Microsystems, Inc.
.FE
release 4.1
.LE
.P
.H 2 "What is in the Test Environment Toolkit"
This
document assumes that your TET root directory is \fItet_root\fR.  When
you specify your TET pathname, replace \fItet_root\fR with your
equivalent pathname.  After you load the TET, you should have the following
subdirectories on your system:
.P
.VL 25
.LI "\fItet_root\fB/inc/posix_c\fR"
POSIX `C' header files for use by test suites, currently only \fBtet_api.h\fR.
.LI "\fItet_root\fB/src/posix_c\fR"
Source directory for the TET tools and API, for POSIX `C'.
.LI "\fItet_root\fB/src/posix_c/tcc\fR"
Source directory for the \fBtcc\fR.
.LI "\fItet_root\fB/src/posix_c/api\fR"
Source directory for the TET Application Programming Interface for
POSIX `C'.
.LI "\fItet_root\fB/src/posix_c/inc\fR"
Header files, for the TET POSIX `C' sources, including the file
\fBtet_jrnl.h\fR.
.LI "\fItet_root\fB/src/xpg3sh/api\fR"
Source directory for the TET Application Programming Interface for
XPG3 shell.
.LI "\fItet_root\fB/bin\fR"
Executable tools for the TET, including the \fBtcc\fR.
.LI "\fItet_root\fB/lib/posix_c\fR"
POSIX `C' library files for use by test suites, including \fBlibapi.a\fR,
\fBtcm.o\fR, and \fBtcmchild.o\fR.
.LI "\fItet_root\fB/demo\fR"
Directory containing the files that can generate a simple demonstration of a
TET test suite.
.LE
.P
.H 2 "Building the Environment"
After the TET is loaded, you can build the
Test Environment Toolkit, including the \fBtcc\fR and the application
programming interfaces, with the makefiles supplied.  However, there is a
makefile that needs to be edited before you can build the TET.
.BL
.LI
\fItet_root\fB/src/posix_c/makefile\fR
.LE
.P
This makefile calls other makefiles in
\fItet_root\fB/src/posix_c/tcc\fR and
\fItet_root\fB/src/posix_c/api\fR.
Edit the
\fItet_root\fB/src/posix_c/makefile\fR
and follow the instructions in the comments.
.P
The TET `C' source is written to POSIX.1 using common `C' language,
although prototypes are provided in the appropriate
header files for use with ANSI `C' compilers.  One extension to POSIX.1
is required: the symbol NSIG.  Many UNIX systems already define NSIG in
\fB<signal.h>\fR as an extension to POSIX.1.  In this case, NSIG
can be made available by compiling with the appropriate feature test
macro defined in addition to _POSIX_SOURCE.
.P
The \fBtcc\fR catches all signals it can from 1 up to (NSIG-1).  On some systems
this can cause problems with non-standard signals, for example SIGCLD
if it is supported separately from SIGCHLD.  Also, with the default signal
handling the \fBtcc\fR cannot be suspended and restarted using job control
signals.  Two variables can be set in \fBmakefile\fR to alter the signal
handling.  One specifies a list of signal numbers to ignore, the other a
list of signal numbers to leave alone.  If you wish to use job control
with the \fBtcc\fR, you should include the job control signals in SIG_LEAVE.
.H 2 "Building the Tools and `C' API"
.P
.AL
.LI
To start, if you are not already there, change directory to
\fItet_root\fB/src/posix_c\fR.
.P
.ft CW
\fBcd \fItet_root\fB/src/posix_c\fR
.ft R
.P
.LI
Type \fBmake\fR.  This will build the TET tool; \fBtcc\fR,
as well as the `C' API.
.LE
.P
When you use \fBmake\fR,
an executable file for each TET tool component is placed in
the directory 
\fItet_root\fR\fB/bin\fR.  The TET API files are generated and placed
into the appropriate directories:
.SP
.DS
.fi
.TS
center, tab(;), box;
lB | lB | lBw(3i)
l | l | l.
Name;Path;Description
_
\fBtcm.o\fR;\fItet_root\fB/lib/posix_c\fR;T{
Test Case Manager \fBmain()\fR routine.  For processes to be executed
by the \fBtcc\fR
T}
_
\fBtcmchild.o\fR;\fItet_root\fB/lib/posix_c\fR;T{
Executed processes \fBmain()\fR routine.  For processes to be executed
by \fBtet_exec()\fR
T}
_
\fBlibapi.a\fR;\fItet_root\fB/lib/posix_c\fR;Library of API interfaces
.TE
.DE
.P
In addition, the following files are also available for use with the
Application Programming Interface.
.SP
.DS
.fi
.TS
center, tab(;), box;
lB | lB | lBw(3i)
l | l | l.
Name;Path;Description
_
\fBtet_api.h\fR;\fItet_root\fB/inc/posix_c\fR;T{
Header containing useful symbol definitions and
declarations/prototypes for all API interfaces
T}
_
\fBllib-ltcm.c\fR;\fItet_root\fB/src/posix_c/api\fR;Lint libraries defining
\fBllib-ltcmc.c\fR;;T{
the API interfaces.  Use the one which corresponds
to the object file in which \fBmain()\fR is defined (\fBtcm.o\fR or
\fBtcmchild.o\fR).  They can be used directly or turned into libraries
for use with \fBlint -l\fR by doing a \fBmake LINTLIB\fR and
installing the \fB.ln\fR files on the system.
T}
.TE
.DE
.P
.H 2 "Building the Shell API"
.P
.AL
.LI
To start, if you are not already there, change directory to
\fItet_root\fB/src/xpg3sh/api\fR.
.P
.ft CW
\fBcd \fItet_root\fB/src/xpg3sh/api\fR
.ft R
.LI
Edit the \fBmakefile\fR and change the lists of signal numbers to
the correct values for your system.
.LI
Type \fBmake\fR.  This will install the shell API files \fBtcm.sh\fR and
\fBtetapi.sh\fR in
the directory \fItet_root\fR\fB/lib/xpg3sh\fR.
.LE
.H 2 "Running the Test Environment Toolkit Demonstration"
This section describes how to run the simple demonstration TET test suite in
\fItet_root\fB/demo\fR.  The given invocation will build, execute, and clean 
the test suite.
.P
.AL
.LI
To start, if you are not already there, change directory to
\fItet_root\fR.
.P
\fBcd \fItet_root\fR
.P
.LI
Make sure that \fItet_root\fB/bin\fR is in your PATH, and type the following 
command invocation:
.AL
.DS I
\fBtcc -bec demo\fR
.DE
.P
.LE
As the demonstration executes, it displays the following messages:
.SP
.DS
.ft CW
.S 9
Journal file is: tet_root/demo/results/0001bec/journal
We have not set TET_OUTPUT_CAPTURE=True so all normal stdin/stdout/stderr
files are available.
If we had set output capture, the results logged by the API would not
be in the journal.
But these lines would.
.S
.ft R
.DE
You can then look in \fItet_root\fB/demo/results/0001bec/journal\fR to see
the results of the demonstration.
.P
.LE
.SK
.H 1 "Known Problems With This Release"
.P
The following problems were discovered too late to be rectified in this 
release. 
.AL
.LI
When using either the rerun or resume options of the \fBtcc\fR an error occurs
if a test case has more than fifty invocable components. An interim solution, 
is to enlarge the value on given on line 60 of 
\fBagain.c\fR. A value should be used that is greater than the maximum number 
of invocable components that will be used in any test case.
.LI
The \fBtcc\fR makes use of the command \fBcp -r\fR. If this use of \fBcp\fR is not supported on 
your system you should install a script in your PATH that correctly 
interprets, and replicates the recursive option of \fBcp\fR.
.LE
.SK
.H 1 "Changes Since the Previous Release"
.H 2 "`C' API"
.BL
.LI
No changes have been made to the `C' API.
.LE
.H 2 "Shell API"
.BL
.LI
No changes have been made to the shell API.
.LE
.H 2 "\fBtcc\fR"
.BL
.LI
The tcc will no longer generate a badly formatted message when it receives a
NULL text value.
.LI
A pointer problem in a results file reordering function has been resolved.
.LI
Minor modifications have been made to an assignment and to some function 
declarations to remove problems encountered with ANSI-C compilers.
.LI
The results code handling functions have been reviewed and modified. The tcc
now responds correctly to any result code with an action of "Abort". It is
also possible now to change the name of reserved result codes.
.LI
The optional BUILD_FAIL_TOOL is now tested for whenever a build produces a 
non-zero result. The previous action was only to consider the BUILD_FAIL_TOOL
whenever a build gave a specific result of one.
.LI
A bug in the scenario parser meant it was not permitting trailing white space. 
Additionally the use of more than one directive per line was generating a
fatal error. Both bugs have now been fixed.
.LI
If the test suite root directory is specified as a full pathname this no longer
creates a problem when the tcc calculates the default name for configuration
files.
.LI
Linker problems due to multiple declarations of variables have now been fixed.
.LE
.H 2 "\fBtet_ins\fR"
.BL
.LI
Removed.
.LE
.H 2 "\fBrpt\fR"
.BL
.LI
Removed.
.LE
.H 2 "Specification"
.P
This release is made with version 4.5 of the TET specification. The following
differences exist between versions 4.4 and 4.5 of the specification.
.BL
.LI
All references to the \fBrpt\fR and \fBtet_ins\fR tools have been removed.
.LE
.SK
.H 1 "Notes on the Specification"
.P
The aim of this chapter is to clarify some points about the functionality of
the TET that may not be immediately clear from reading the specification.
.P
.H 2 "Reserved Result Code Names"
.P
The rerun and resume options of the \fBtcc\fR make use of the strings: 'b', 'e'
and 'c'. Setting a result code name equal to one of these strings may cause
unexpected results when the rerun or resume options are used.
.P
.H 2 "The TET_OUTPUT_CAPTURE variable"
.P
This variable may be set independently for each mode of the \fBtcc\fR. When
set to 'False' (the default) the \fBtcc\fR expects the appropriate mode of
operation to generate results in a file called \fBtet_xres\fR. The format
of the results contained in this file are dictated by section 7.2.1 of the 
specification.
.P
When set to 'True', the \fBtcc\fR captures the \fIstderr\fR and \fIstdout\fR
from the appropriate mode of operation, and makes no attempt to open the 
\fBtet_xres\fR file. 
.P
.H 2 "The Alternate Execution Directory"
.P
An alternate execution directory can be set using the \fB-a\fR option of
the tcc (See also section 6.1.2 of these notes). Test cases are specified in
a scenario in terms of a relative directory pathname ending with the test case
target name. For each test case, the 
build and clean modes of the \fBtcc\fR are initiated in the appropriate test
case directory which is relative to the test suite root directory. The execute 
mode of the \fBtcc\fR is initiated in a directory that is either relative to 
the alternate execution directory (if it is set), or else the test suite root 
directory.
.P
.H 2 "The TET_EXEC_IN_PLACE variable"
.P
When set to 'False' (the default) this variable instructs the \fBtcc\fR to 
recursively copy the directory in which execution would have taken place (see 
the above section) to a new temporary directory (See section 6.1.3 of these
notes). The execute mode of the \fBtcc\fR then takes place in the temporary 
directory, after which, the temporary directory is recursively removed.
.SK
.H 1 "Extensions to the Specification"
.P
The following features are implemented in addition to those
required by the TET specification.
.H 2 "Environment Variables"
.P
.H 3 "TET_ROOT"
.P
You can override the value of TET_ROOT compiled into \fBtcc\fR
by setting TET_ROOT in your shell environment before executing \fBtcc\fR.
For example
.BL
.LI
For the C shell, type a line like the
following at the shell prompt:
.P
\f(CWsetenv TET_ROOT <\fIyour TET_ROOT path\f(CW>
.ft R
.P
.LI
Or, for the Bourne shell, type the following line at the shell prompt:
.P
\f(CWTET_ROOT=<\fIyour TET_ROOT path\f(CW>;export TET_ROOT
.ft R
.P
By doing this, you can then refer to \fI<your TET_ROOT path>\fR as
$TET_ROOT in the shell.
.LE
.P
.H 3 "TET_EXECUTE"
.P
The environment variable TET_EXECUTE can be set in a similar fashion to that
of the TET_ROOT, and allows you to specify a default value for the
alternate execution directory that is used by the \fBtcc\fR. 
Note however that a command line supplied value
for the alternate execution directory has a higher precedence than that
supplied by the TET_EXECUTE environment variable.
.P
.H 3 "TET_TMP_DIR"
.P
During the execution of the \fBtcc\fR temporary files are usually created in
unique sub_directories under a directory called \fBtet_tmp_dir\fR, which is
located in the \fItet_root\fR.  Whilst every effort has been made to ensure
that each sub-directory is removed as the \fBtcc\fR finishes, it is possible 
that an abnormal termination will mean that a sub-directory remains.  Such
directories should be removed to conserve disk space.
.P
The user may specify an alternative temporary directory for use by
\fBtcc\fR by setting the environment variable TET_TMP_DIR.  This variable
should be set in a similar fashion to the TET_ROOT variable discussed above.
Performance problems due to network file systems can be helped by setting
this variable to a directory on a local file system.
.P
.H 2 "Extra Options"
.P
The following \fBtcc\fR command line options are provided in addition to those
in the specification:
.VL 7n
.LI \fB-l\fR
may be used to supply a one-off scenario line for execution without the
need to create a scenario file.
.LI \fB-p\fR
enables progress reporting to standard output.
.LI "\fB-y -n\fR"
may be used to select parts of a scenario for processing.  Test case
execution lines are processed only if they match the specified search
string (\fB-y\fR) or don't match it (\fB-n\fR).
.LE
.P
See the manual page in \fItet_root\fB/doc/posix_c/tcc.1\fR for more details.
