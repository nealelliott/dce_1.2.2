.\"	SCCS:  @(#)rel_note.mm	1.4 (92/12/15)
.\"
.\"	UniSoft Ltd., London, England
.\"
.\"	(C) Copyright 1992 X/Open Company Limited
.\"
.\" All rights reserved.  No part of this document may be reproduced,
.\" stored in a retrieval system, or transmitted, in any form or by any
.\" means, electronic, mechanical, photocopying, recording or otherwise,
.\" except as stated in the end-user licence agreement, without the prior
.\" permission of the copyright owners.
.\"
.\" X/Open and the 'X' symbol are trademarks of X/Open Company Limited in
.\" the UK and other countries.
.\"
.\" **********************************************************************
.\"
.\" Format with: tbl | troff -mm
.\"
.\" the release date:
.ds dT "(December\ 1992)
.\" some useful strings
.ds rE "Release 1.0b
.ds tR \fItet_root\fP
.\" macros
.de Es	\" example start
.SP 0.5v
.DS I
.if t .ft C
.if n .ft R
.nr T1 \\w'\0'u*8
.ta \\n(T1u +\\n(T1u +\\n(T1u +\\n(T1u +\\n(T1u +\\n(T1u +\\n(T1u +\\n(T1u
..
.de Ee	\" example end
.ft R
.DE
..
.\" courier font macros
.\" as much like .B, .RB etc. in mm as possible
.\" uses bold instead of courier in nroff
.\"
.de C	\" courier font
.nr fS \\n(.f
.if t .ft C
.if n .ft B
.if \\n(.$ \{\
\&\\$1
.ft \\n(fS \}
..
.de CR	\" courier roman ...
.if t .aF C R "\\$1" "\\$2" "\\$3" "\\$4" "\\$5" "\\$6"
.if n .aF B R "\\$1" "\\$2" "\\$3" "\\$4" "\\$5" "\\$6"
..
.de RC	\" roman courier ...
.if t .aF R C "\\$1" "\\$2" "\\$3" "\\$4" "\\$5" "\\$6"
.if n .aF R B "\\$1" "\\$2" "\\$3" "\\$4" "\\$5" "\\$6"
..
.de CI	\" courier italic ...
.if t .aF C I "\\$1" "\\$2" "\\$3" "\\$4" "\\$5" "\\$6"
.if n .aF B I "\\$1" "\\$2" "\\$3" "\\$4" "\\$5" "\\$6"
..
.de IC	\" italic courier ...
.if t .aF I C "\\$1" "\\$2" "\\$3" "\\$4" "\\$5" "\\$6"
.if n .aF I B "\\$1" "\\$2" "\\$3" "\\$4" "\\$5" "\\$6"
..
.de aF	\" alternate fonts subroutine for .CR et al
.nr fS \\n(.f
.ft \\$1
.nr f1 \\n(.f
.ft \\$2
.nr f2 \\n(.f
.a2 \\n(f1 \\n(f2 \& "\\$3" "\\$4" "\\$5" "\\$6" "\\$7" "\\$8"
.ft \\n(fS
..
.de a2	\" .aF subroutine
.ds sE
.if t .if \\$1=2 .if !\\$5 .ds sE \^
.ie \\$1>9 .ds fT \\f(\\$1
.el .ds fT \\f\\$1
.ie \\$4 \\$3
.el .a2 \\$2 \\$1 "\\$3\\*(fT\\$4\\*(sE" "\\$5" "\\$6" "\\$7" "\\$8" "\\$9"
..
.\" initialisation
.rm )k
.PH ""
.PF ""
.ds HP 14 12 12
.ds HF 3 3 3
.nr Hs 3
.nr Hb 7
.nr Ej 1
.\" cover sheet
.S 20
.SP 1.5i
.ce 99
.B
Release Notes for the
Distributed Test Environment Toolkit
.R
.SP 2v
.S 12
Implementation in `C' for POSIX.1 Systems with TCP/IP networking
.br
by X/Open 
.SP 1v
\*(rE\0\0\*(dT
.ce 0
.\" start of document
.SK
.S 10
The information contained within this document is subject to change
without notice.
.SP 2
Copyright \(co 1992 X/Open Company Limited
.br
All rights reserved.
.P
No part of this document or accompanying source code
may be reproduced,
stored in a retrieval system, or transmitted, in any form or by any
means, electronic, mechanical, photocopying, recording or otherwise,
except as stated in the end-user licence agreement, without the prior
permission of the copyright owners.
.P
X/Open and the `X' symbol are trademarks of X/Open Company Limited in
the UK and other countries.
.SP
.PH "'DTET Release Notes''\*(rE\0\0\*(dT"
.SK
.nr P 1
.nr % 1
.af P 1
.PF "''- \\\\nP -''"
.S 11
.H 1 Introduction
.H 2 "Preface"
These release notes accompany \*(rE of the
.I "Distributed Test Environment Toolkit"
implementation, by X/Open, in `C' for POSIX.1 systems with TCP/IP networking.
They describe the prerequisites for running this toolkit, what is supplied
in the toolkit and how to build and run the toolkit.
The release notes also
list the operating system platforms on which the
Distributed Test Environment Toolkit (DTET) has
been built and run.
.P
The DTET is the first of a set of extensions to the TET by X/Open.
.P
The DTET is owned by X/Open and
its shareholders.
.P
This release of the DTET is based on
Release\ 1.10 of the Test Environment Toolkit (TET) implementation in `C' for
POSIX.1 systems by OSF, UI and X/Open,
and is released as an extension to TET Release\ 1.10.
.P
This release is made to sites participating in the alpha review of VSX with
XNFS, and to DTET test sites.
It is intended that there will be subsequent DTET releases to a wider audience
who will be able to licence the product from X/Open.
.H 2 "Audience"
This document is written for software engineers
who will be porting
their validation test suites into the Distributed Test Environment Toolkit.
A knowledge of UNIX system and TCP/IP network administration is assumed in those
sections describing DTET system installation and configuration.
.H 2 "Related Documents"
For additional information about the Distributed Test Environment Toolkit,
refer to the following documents:
.BL
.LI 
.I
Test Environment Toolkit: Architectural, Functional, and
Interface Specification, Version 4.5
.R
.LI 
.I
Distributed Test Environment Toolkit: Architectural, Functional, and
Interface Specification, Version 1.2
.R
.LE
.P
Note that the DTET specification is in the form of an extension to the TET
specification.
.H 2 "Typographic and Keying Conventions"
This document uses the following typographic conventions: 
.C
.nr x \w'Typewriter\ font'u+2n
.R
.VL \nxu
.C
.LI "Typewriter\ font"
.R
Names of programs, functions and file names that you must use literally
appear in this font.
.I
.LI Italics
.R
Italic words or characters in formats and command descriptions 
represent values that you must supply.
.LE
.SP 
.H 2 "Problem Reporting"
If you encounter problems when building or executing the DTET,
take a copy of the error reporting template contained in the file
.C \*(tR/doc/dtet/err.template
in the distribution, fill in the details of the problem, and send it by
electronic mail to:
.Es
dtet_support@xopen.co.uk
.Ee
You should include sufficient information in your report
to enable someone who is
unfamiliar with your system to be of assistance in solving the problem.
.H 1 "Introducing the Distributed Test Environment Toolkit"
The purpose of the Distributed Test Environment Toolkit (DTET)
is to provide a
uniform framework, or test scaffold, into which distributed
test suites can be
incorporated.
By providing such a scaffold, test suites from
different vendors can share a common interface, allowing for, among
other things, ease of portability.  
.P
In this context, and throughout these notes, the term
.I "distributed test"
means a test which consists of several parts; each test part executes
concurrently on a different system and contributes to the overall test result.
Typically, distributed tests are used to verify some kind of interaction
between two or more systems.
.P
The DTET \*(rE is backwards compatible with, and fully supports the
functionality of, version 1.10 of the TET.
.P
This implementation of the DTET consists of:
.BL
.LI
a test case controller
providing support for the building, execution,
and clean-up of test scenarios
.RC ( dtcc )
.LI
server processes to undertake synchronisation and execution results processing
.RC ( tetsyncd
and
.CR tetxresd ),
and remote system processing
.RC ( tccd )
.LI
a `C' Application Programming Interface library
.RC ( libdapi.a )
and partially linked object
modules for inclusion in the various parts of distributed test cases
.RC ( mtcm.o ,
.CR stcm.o ,
.C dtcmchild.o
and
.CR tcmrem.o )
.LE
.P
The DTET Application Programming Interface (API) consists of a test case
manager and an API library which are used in distributed
test cases to handle
interfacing between the test case parts and the rest of the DTET system.
.P
As well as executing distributed test cases that have been built using
the DTET API,
the DTET Test Case Controller is also able to execute
non-distributed test cases, either sequentially or in parallel,
that have been built using the TET API.
.P
This release does not include documentation such as a Programmers' Guide
or Users' Guide.
.P
A reference manual page for
.C dtcc
is supplied in the directory
.CR \*(tR/doc/dtet .
Refer to the chapters entitled ``Application Programming Interfaces'' in
both the TET and DTET specifications
for information on the TET and DTET Application Programming Interfaces.
.H 1 "Loading the Distribution"
This
document assumes that your DTET root directory is \*(tR.
When
you specify your DTET path name, replace \*(tR with your
equivalent path name.
.P
The layout of the DTET distribution has been designed to allow the distribution
to be installed with the same \*(tR value as for the TET version\ 1.10
distribution.
However, if you intend to install the DTET distribution on your system
in the same place as
TET version\ 1.10, you should first inspect the DTET distribution and
satisfy yourself that no files will be
overwritten when you load the DTET on to your system.
.P
You should load the DTET distribution on to every system that is to
participate in the running of distributed tests.
.br
.ne 10v
.P
After you load the DTET, you should have the following
subdirectories on your system:
.SP 1v
.na
.TS
lf(C) lw(3.75i).
\*(tR/doc/dtet	Directory containing DTET documentation source files.
.sp 0.55v
\*(tR/inc/dtet	Directory containing header files for use by DTET test suites.
.sp 0.55v
\*(tR/src/dtet	Source directory for the DTET tools and API.
.sp 0.55v
.ne 2v
\*(tR/src/dtet/apilib	T{
Directory containing source files for TET-derived API functions.
T}
.ne 2v
.sp 0.55v
\*(tR/src/dtet/dapilib	T{
Directory containing source files for DTET-specific API functions.
T}
.ne 2v
.sp 0.55v
\*(tR/src/dtet/defines	T{
Directory containing example system-specific makefile definition files.
T}
.ne 2v
.sp 0.55v
\*(tR/src/dtet/dtetlib	T{
Directory containing source files for low-level DTET library functions.
T}
.ne 2v
.sp 0.55v
\*(tR/src/dtet/inc	Directory containing header files for the DTET `C' sources.
.ne 2v
.sp 0.55v
\*(tR/src/dtet/inetlib	T{
Directory containing source files for INET transport-specific
library functions.
T}
.sp 0.55v
\*(tR/src/dtet/llib	Directory containing lint library source files.
.ne 2v
.sp 0.55v
\*(tR/src/dtet/servlib	T{
Directory containing source files for the generic server and server access
library functions.
T}
.ne 2v
.sp 0.55v
\*(tR/src/dtet/syncd	T{
Directory containing source files for the DTET Synchronisation daemon
.CR tetsyncd .
T}
.ne 2v
.sp 0.55v
\*(tR/src/dtet/tcc	T{
Directory containing source files for the DTET Master Test Case Controller
.CR dtcc .
T}
.ne 2v
.sp 0.55v
\*(tR/src/dtet/tccd	T{
Directory containing source files for the DTET Test Case Controller daemon
.CR tccd .
T}
.ne 2v
.sp 0.55v
\*(tR/src/dtet/tcm	T{
Directory containing source files for the DTET Test Case Manager components.
T}
.ne 2v
.sp 0.55v
\*(tR/src/dtet/xresd	T{
Directory containing source files for the DTET Execution Results daemon
.CR tetxresd .
T}
.ne 6v
.sp 0.55v
\*(tR/bin	Directory that will contain executable tools for the DTET.
.ne 2v
.sp 0.55v
\*(tR/lib/dtet	T{
Directory that will contain library files for use when building DTET tools
and test suites.
T}
.\" .ne 2v
.\" .sp 0.55v
.\" \*(tR/demo/dtet	T{
.\" Directory containing the files that can generate a simple demonstration
.\" of a DTET test suite.
.\" T}
.TE
.SA
.SP
.H 1 "Building the Distributed Test Environment Toolkit"
This chapter describes how to set up your system and build the test
environment.
Prior to building the test environment, you should check
that the following requirements are met.
The test environment will not build without them.
.P
You should build and install the DTET on every system that is to
participate in the running of distributed tests.
.H 2 "System Requirements"
The DTET requires that your system's
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
.I "X/Open Portability Guide Issue 3, Volume 1: XSI Commands and Utilities"
.FE
(for the
.CR shell ,
.CR find ,
.C cp
and
.C rm
commands)
.LE
.P
In addition, your system should provide the set of network-related
system calls and/or library routines, sometimes known as
.IR "Berkeley sockets" ,\*F
.FS
As implemented in BSD4.3 and SunOS.
.FE
that allow access to TCP/IP network facilities.\*F
.FS
It is possible that future implementations of the DTET may support
other network transports and efforts have been made to design the
internal transport-specific interfaces with this in mind.
.FE
.P
As of this writing, the DTET has only been
installed and tested on UNIX\*F
.FS
UNIX is a registered trademark of UNIX System Laboratories, Inc. in the U.S.
and other countries.
.FE
type of systems.  Note that this
does not preclude its installation on other types of operating
systems.
.P
The DTET provides a series of makefiles that can be used to build the toolkit
in a UNIX type of environment.  If you are not running a UNIX type of 
operating system, you will have to develop your own equivalent method of 
building the DTET.
.H 2 "Installed Platforms"
This release of the DTET has been
installed and tested on the following operating system platforms (UNIX
type of operating systems).
.BL
.LI
AIX\*F
.FS
AIX is a trademark of International Business Machines Corporation.
.FE
version 3.2
.LI
HP-UX\*F
.FS
HP-UX is a registered trademark of Hewlett Packard Corporation.
.FE
version 7.0.
.LI
SunOS\*F
.FS
SunOS is a trademark of SUN Microsystems, Inc.
.FE
release 4.1
.LI
UNIX System V release 4.0\*F
.FS
There is a suspected problem in some versions of the SVR4 socket
emulation library
.C libsocket
that causes 
.C fcntl()
to fail under certain circumstances with
.CR EINVAL .
If your system exhibits this behaviour, you may need to make
modification to the socket emulation library in order to run the DTET.
.FE
.LE
.SP
.H 2 "Building the Environment"
.R
.H 3 "The \f(CBdefines.mk\fP File"
Once the DTET distribution is loaded on to your system, you can build the
Distributed Test Environment Toolkit
with the makefiles supplied.
However, there is a makefile definition file that needs to be supplied
before the DTET can be built; the name of this file is 
.CR \*(tR/src/dtet/defines.mk .
This file contains system-specific definitions and is
.IR include d
in the makefiles that are used to build the DTET.
.br
.ne 6v
.P
Several example makefile definition
files are included with the distribution and may be found in the directory
.C \*(tR/src/dtet/defines
as follows:
.SP 0.5v
.DS I
.TS
lf(C) l.
aix32.mk	for use with AIX version 3.2
.sp 0.55v
hpux7.mk	for use with HP-UX version 7.0
.sp 0.55v
sunos41.mk	for use with SunOS release 4.1
.sp 0.55v
svr4.mk	for use with UNIX System V release 4.0
.sp 0.55v
ultrix42.mk	for use with ULTRIX\*F version 4.2
.TE
.DE
.FS
ULTRIX is a registered trademark of Digital Equipment Corporation.
.FE
.SP
If you are building the DTET on one of these systems and are satisfied that the
definitions provided are correct for your system, it should be sufficient
simply to make a link to the relevant
.C \&.mk
file.
However, if you are building the DTET on some other system, or wish to change
any of the example definitions, you should copy one of the supplied
.C \&.mk
files in the
.C defines
directory to
.C \*(tR/src/dtet/defines.mk
and edit it as required.\*F
.FS
If you build the DTET on a system other than one of those for which a
.C \&.mk
is supplied and you would like your
.C defines.mk
file to be included in future DTET distributions, please send a copy of the
file by electronic mail to the DTET support mailbox mentioned elsewhere in
these release notes.
.FE
.P
In addition, a template that can be used to construct your own
makefile definitions is provided in the file
.C \*(tR/src/dtet/defines/template.mk
for use in cases where it is necessary to create the
.C defines.mk
file from scratch.
.H 3 "Definitions Related to the TET-derived Source"
The portion of the DTET `C' source derived from the TET is written to
POSIX.1 using common `C' language,
although prototypes are provided in the appropriate
header files for use with ANSI `C' compilers.
One extension to POSIX.1 is required; namely the symbol
.C NSIG
which defines the number of signals supported by the UNIX implementation.
Many UNIX systems already define
.C NSIG
in
.C <signal.h>
as an extension to POSIX.1.
In this case,
.C NSIG
can be made available by compiling with the appropriate feature test
macro defined in addition to
.CR _POSIX_SOURCE .
.P
The set of compiler definitions (but not other compiler
options) that should be used
when compiling the TET-derived source should be assigned to the
variable
.C TET_CDEFS
in
.CR defines.mk .
.P
.C dtcc
catches all the signals it can, from 1 up to and including
.CR "(NSIG\ \-\ 1)" .
On some systems
this can cause problems with non-standard signals; for example,
.C SIGCLD
if it is supported separately from
.CR SIGCHLD .
Also, with the default signal handling,
.C dtcc
cannot be suspended and restarted using job control signals.
Two additional compiler definitions may be added to
.C TET_CDEFS
to alter the signal handling.
The
.C TET_SIG_IGNORE
definition specifies a list of signal numbers to ignore, and the
.C TET_SIG_LEAVE
definition specifies a list of signal numbers to leave alone.
If you wish to use job control with
.CR dtcc ,
you should include the job control signals in
.C TET_SIG_LEAVE .
.H 3 "Definitions Related to the DTET-specific Source"
The portion of the DTET `C' source that deals with certain DTET-specific
functions (including transport-specific functions) uses features that
are not specified in POSIX.1, although they are available on many
UNIX systems on which TCP/IP networking is implemented.
As with the TET-derived source,
the DTET-specific source is written using the common `C' language,
and prototypes are provided in the appropriate
header files for use with ANSI `C' compilers.
.P
The set of compiler definitions (but not other options) that should be used
when compiling the DTET-specific source should be assigned to the
variable
.C DTET_CDEFS
in
.CR defines.mk .
.P
This DTET implementation uses a client/server architecture and one of the
servers \(em the TCC daemon \(em
is required to run on each system which may run distributed tests.
.ne 6v
This daemon may be started in one of several ways as follows:
.BL
.LI
interactively from the shell command line
.LI
at system boot time from an entry in the system initialisation file
.C /etc/rc
.LI
at system boot time from an entry in the
.C init
configuration file
.C /etc/inittab
.LI
on demand, from an entry in the
.C inetd
configuration file
.C /etc/inetd.conf
.LE
.P
The way in which the TCC daemon is to be
started depends on the type of system on which is is to be run, and on local
system administration policy.
The DTET-specific source may be compiled for any of the above options; the
required start method is specified by extra compiler definitions as follows:
.BL
.LI
interactive start and
.C /etc/rc
start \(em no extra definitions are required to build the
.B rc
version of
.C tccd
.LI
.C /etc/inittab
start \(em define
.C INITTAB
to build the
.B inittab
version of
.C tccd
.LI
.C /etc/inetd.conf
start \(em define
.C INETD
to build the
.B inetd
version of
.C tccd
.LE
.P
If one of these compiler definitions is required, it should be added to the
.C DTET_CDEFS
assignment in
.CR defines.mk .
.P
The name by which the TCC
daemon is to be known should be assigned to the
.C TCCD
variable in
.CR defines.mk .
.C TCCD
should be set to
.C in.tccd
on systems where the TCC daemon
is to be started by
.C inetd
(in accordance with established conventions); otherwise,
.C TCCD
should be set to
.CR tccd .
.H 3 "Other Definitions"
Compiler definitions and header file directories 
that are to be specified when compiling both the
TET-derived and DTET-specific source should be assigned to the
.C CDEFS
variable in
.CR defines.mk .
.P
Other compiler options should be assigned to the
.C COPTS
variable.
.P
Loader options should be assigned to the
.C LDFLAGS
variable.
.P
System library names and
.C \-l
options to be added to the end of the compiler link-edit command line
should be assigned to the
.C SYSLIBS
variable.
.P
If your system has working
.C lorder
and
.C tsort
utilities, set the
.C LORDER
variable to
.C lorder
and
.C TSORT
to
.CR tsort ;
otherwise, set
.C LORDER
to
.C echo
and
.C TSORT
to
.CR cat .
.P
If your system has the
.C mcs
utility, set the
.C MCS
variable to
.CR mcs ;
otherwise, set
.C MCS
to
.C \&@\&:
(to cause
.C make
silently to do nothing).
.P
If your system requires that
.C ranlib
be used to process archive libraries, set the
.C RANLIB
variable to
.CR ranlib ;
otherwise, set
.C RANLIB
to
.CR \&@\&: .
.H 2 "Building the Tools and `C' API"
.AL
.LI
To start, if you are not already there, change directory to
.CR \*(tR/src/dtet ,
thus:
.Es
cd \*(tR/src/dtet
.Ee
.LI
Type
.Es
make all
.Ee
to build the DTET tools and API, or
.Es
make install
.Ee
to build the tools and API and install them in their destination directories
under \*(tR.
.LE
.SP 1v
.H 1 "Configuring Your System to Run the DTET"
.H 2 "Password File Entry"
When
.C tccd
runs, it attempts to change
its user and group IDs to those specified for the user
.C tet
in the system password file.\*F
.FS
More precisely, to the user and group ID, and home directory that are returned
by a
.C getpwnam()
call.
.FE
In addition,
.C tccd
changes directory to the home directory specified for the user
.CR tet .
(A different user name may be specified using the
.C \-u
command-line option if required.)
.P
You should create a home directory for the user
.C tet
and
add a suitable entry to the system password file.
The user and group IDs allocated to the user
.C tet
need not imply any special privilege
(i.e., they should have value 100 or greater).
.H 2 "Systems and Hosts File Entries"
Remote systems are known to the DTET by numeric system names.
This DTET implementation maps these values to host names by means of a
.C systems
file in directory specified by the value of
.C TET_ROOT
on each system when
.C dtcc
is run.
You should create a
.C systems
file on the master system, and on each slave system that may run a test case
that calls
.CR tet_remexec() .
You should ensure that there is no conflict between
.C systems
file entries on different systems.
.P
An example
.C systems
file is supplied in the \*(tR directory in the DTET distribution.
.P
You should check the
.B hosts
database on each system that may run parts of a distributed test
to ensure that entries exist for all the
host names specified in the
.C systems
files.\*F
.FS
It is probably a bad thing to have the host name
.C localhost
in a
.C systems
file since this name means different things on different systems.
.FE
.H 2 "Services File Entry"
The TCC daemon
.C tccd
listens for requests on the well-known Internet TCP port number
specified for the
.B tcc
service in
.C /etc/services
or its equivalent.\*F
.FS
More precisely, to the port number that is returned
by a
.C getservbyname()
call.
.FE
This port number must be the same on all systems that are to participate in a
set of distributed tests.
The
.B tcc
port number should be that of a non-privileged port (i.e., 1024 or greater).
.br
.ne 6v
.P
For example, to define the well-known
port as TCP port 1234, add the following line to each
.C /etc/services
file:
.Es
tcc	1234/tcp
.Ee
.H 2 "System Equivalence File"
When a client DTET system logs on to
.CR tccd ,
the server looks up the client system's host name in the file
.C systems.equiv
in the home directory specified for the
.C tet
user name,
and rejects logins that appear to come from systems that do not have
an entry in that file.
You should create a
.C systems.equiv
file in the
.C tet
home directory on each system that is to run
.CR tccd .\*F
.FS
If
.C tccd
runs using a different user ID because it is started with the
.C \-u
command-line option, the
.C systems.equiv
file should be created in the home directory specified for that user.
.FE
Each equivalence
file should contain an entry for every system to which you wish to
offer DTET services.
.P
An example
.C systems.equiv
file is supplied in the \*(tR directory in the DTET distribution.
.H 2 "UNIX Configuration Parameters"
The number of processes generated by
.C dtcc
depends on the complexity of the scenario directive being processed.
If the combination of
.C parallel
and
.C remote
directives cause
.C dtcc
to generate more than the maximum number of processes allowed by your system
for an individual
user,
you will need to increase your system's per-user process limit.
.P
DTET processes use sockets to communicate with one another,
and each socket uses a file descriptor and may use certain other UNIX system
resources as well.
If the complexity of a test scenario causes
DTET processes to attempt to open more files and sockets than
allowed by your system, you will have to increase your system's per-user and/or
per-system open file limit.\*F
.FS
On small systems, it may also be necessary to increase your system's number of
.I mbufs
(for BSD-style kernel network implementations) or stream buffers (for
.SM STREAMS -based
network implementations).
.FE
.R
.H 1 "Starting and Terminating \f(CBtccd\fP"
As mentioned previously, the TCC daemon
.C tccd
must be started on each system that may run the slave parts of a
distributed test suite.
.P
By default,
.C tccd
places its diagnostic output in a log file named
.CR /tmp/tccdlog ;
however,
this name may be changed if required by means of the
.C \-l
command line option.
If
.C tccd
starts successfully, it prints a START message to its log file;
otherwise it prints a diagnostic (to its log file if possible) and exits.
If the log file cannot be opened,
.C tccd
attempts to send diagnostic output to
.CR /dev/console .
.P
The master
.C tccd
daemon can be terminated by sending it a
.C SIGTERM
signal.
.P
Because
.C tccd
is responsible for executing all processes related to the
.IR build ,
.I execute
and
.I clean
phases of slave testcase parts, the only environment variables made
available to these processes are those in force when
.C tccd
is started.
Therefore, when starting
.CR tccd ,
you should ensure that the values of environment variables
(in particular, the value of
.CR PATH )
enable your slave testcase parts to build, execute and clean-up correctly.
.R
.H 2 "Starting \f(CBtccd\fP from the Shell Command Line"
If possible, you should be logged in as
.C root
or as the user
.CR tet .
To start the
.B rc
version of
.C tccd
from the shell, type
.Es
tccd
.Ee
.P
To start the
.B inittab
version of
.C tccd
from a shell that does not support job control, type
.Es
tccd &
.Ee
.P
To start the
.B inittab
version of
.C tccd
from a shell that supports job control, type
.Es
sh -c "tccd &"
.Ee
.P
The
.B inetd
version of the TCC daemon
.RC ( in.tccd )
cannot be started from the shell command line.
.R
.H 2 "Starting \f(CBtccd\fP from \f(CB/etc/rc\fP"
If you decide that
.C tccd
should be started at system startup time from
.C /etc/rc
or its equivalent,
you should include the following lines at a suitable place
in that file:
.Es
if test -x \*(tR/bin/tccd
then
	\*(tR/bin/tccd && echo tccd started
fi
.Ee
.P
Once this is done,
.C tccd
will be started each time your system enters multi-user mode.
.P
Only the
.B rc
version of
.C tccd
should be started in this way.
.R
.H 2 "Starting \f(CBtccd\fP from \f(CB/etc/inittab\fP"
If your system supports multiple
.B init
run levels and you decide that
.C tccd
should be run at system startup time from
.C /etc/inittab
or its equivalent,
you should include a suitable line in that file.
The following example entry might be used to start
.C tccd
when the system is brought up in multi-user mode at run level\ 3:
.Es
tet1:3:respawn:\*(tR/bin/tccd
.Ee
.P
Once this is done,
.C tccd
will be started each time the system enters the specified run level.
It will be necessary to log in as
.C root
and type
.Es
init q
.Ee
in order to instruct
.C init
to re-read
.C /etc/inittab
and take account of the new entry.
.P
Only the
.B inittab
version of
.C tccd
should be started in this way.
If some problem prevents
.C tccd
from starting successfully, you should expect to see some diagnostic output from
.C init
appear on the system console.
.br
.ne 6v
.R
.H 2 "Starting \f(CBin.tccd\fP from \f(CB/etc/inetd.conf\fP"
If your system runs the
.C inetd
super-server and you decide that the TCC daemon
should be run on demand by
.CR inetd ,
you should include the following line in
.C /etc/inetd.conf
or its equivalent:\*F
.Es
tcc  stream  tcp  nowait  tet  \*(tR/bin/in.tccd  in.tccd
.Ee
.FS
There is a feature in some versions of
.C inetd
(notably those derived from the
.C inetd
distributed with BSD4.3) that causes an 
.C inetd.conf
entry for an external service not to work properly
if it appears after the entries
for the internal services.
If you suspect that
.C inetd
on your system exhibits this feature, make sure that the
.C inetd.conf
entry for
.C in.tccd
appears
.I before
entries defining services implemented internally by
.CR inetd .
.FE
.P
It will be necessary to log in as
.C root
and type
.Es
kill -1 \fIinetd_pid\fP
.Ee
(where
.I inetd_pid
is
.CR inetd 's
process ID)
in order to instruct
.C inetd
to re-read
.C /etc/inetd.conf
and take account of the new entry.
.P
Only the
.B inetd
version of the TCC daemon
.CB ( in.tccd )
should be started this way.
.P
Note that
.C inetd
only attempts to start
.C in.tccd
when a process connects to the well-known
.B tcc
port; hence no START message is printed to the log file.
If some problem prevents
.C in.tccd
from starting successfully on demand, you may need to examine
.C /etc/syslog.conf
(or its equivalent) to see where
.C inetd
diagnostic output is directed.\*F
.FS
If you have difficulties in getting
.C in.tccd
to start, it may be helpful to try and run one of the other versions of
.C tccd
in order to isolate the cause of the problem.
.FE
.H 1 "Problems Fixed in this Release"
The following problems reported since the last DTET release have been
fixed in this release:
.AL 1
.LI
Comments in the scenario file are no longer reported as an error by
.CR dtcc .
.LI
When executing a distributed test,
journal file output from a slave test part now goes only to the combined
execution results file on the master system, and not to the
.C tet_xres
file on the slave system.
.LI
Support added for systems that return
.C ENOTEMPTY
when
.C rmdir()
attempts to remove a directory that is not empty.
.LI
Long lines sent to the combined execution results
file are now truncated correctly.
.LI
A trailing newline in a string passed to
.C tet_infoline()
is now removed.
.LI
Added auto-sync to DTET TCM before executing user-supplied
startup and cleanup functions.
.LI
If
.CR TET_EXEC_IN_PLACE=False ,
the temporary execution directory is now removed correctly on slave
systems.
.LI
The result code string now appears on a test result line in the combined
execution results file.
.LI
Each DTET TCM message is now tagged with the system name so that its
origin can be determined.
.LI
Support added for API-conforming DTET build and clean tools.
.LI
An auto-sync at test purpose end in one test part now cancels a
.C tet_sync()
call in progress in another test part when that
call names the system whose test part has reached test purpose end.
.LE
.H 1 "Known Problems with this Release"
The following problems exist in this 
release, and are expected to be rectified in the future:
.H 2 "Problems Inherited from the TET"
.AL
.LI
When using either the
.I rerun
or
.I resume
options of
.C dtcc
an error occurs
if a test case has more than fifty invocable components.
An interim solution is to change the definition of ICS_MAX in
.CR tcc/again.c .
A value should be used that is greater than the maximum number 
of invocable components that will be used in any test case.
.LI
.C dtcc
makes use of the command
.CR "cp\ \-r" .
If this use of
.C cp
is not supported on 
your system you should install a script that correctly 
interprets, and replicates, the recursive copy option of
.CR cp .
In order to facilitate this, the strings defining the
.C cp
command and its
.C \-r
argument are
.CR #define d
in the related source file as
.C TET_CP_CMD
and
.CR TET_CP_ARG ,
respectively.
If you want to change the default values of these strings, you may add
suitable definitions to
.C TET_CDEFS
in your version of the
.C defines.mk
file.
For example, if the name of the script is
.CR mycp ,
you should append the string
.Es
-DTET_CP_CMD=mycp
.Ee
to the definition of
.C TET_CDEFS
in your version of the
.C defines.mk
file.
.LE
.H 2 "Problems Specific to the DTET"
.AL 1
.LI
The DTET API needs to be able to transmit
.C errno
values between systems in such a way that the
.I meaning
of the value is preserved even though the value itself is different on
different systems.
At present, the way that this is done can cause problems when building the
DTET if one
.C errno
value is defined in terms of another.
On systems where this is the case, the problem is usually indicated when
the C compiler emits a diagnostic of the form
.I "duplicate case in switch"
when attempting to compile certain source files.
.P
The source files known to be affected by this problem are as follows:
.Es
\*(tR/src/dtetlib/errname.c
\*(tR/src/dtetlib/maperr.c
.Ee
Each of these files contains instructions and an example of how to apply a
suitable workaround if so required.
.LI
During system testing, the
.C \-y
and
.C \-n
command-line options of
.C dtcc
were found not to work correctly.
.LE
.H 1 "Notes on the Specifications"
The aim of this chapter is to clarify some points about the functionality of
the DTET that may not immediately be clear from reading the TET and DTET
specifications.
.br
.ne 6v
.H 2 "TET Specification Notes"
.H 3 "Reserved Result Code Names"
The
.I rerun
and
.I resume
options of the
.C dtcc
make use of the strings
.CR b ,
.C e
and
.CR c .
Setting a result code name equal to one of these strings may cause
unexpected results when the
.I rerun
or
.I resume
options are used.
.R
.H 3 "The \f(CBTET_OUTPUT_CAPTURE\fP Variable"
This variable may be set independently for each mode of the
.CR dtcc .
When set to
.C False
(the default),
.C dtcc
expects the appropriate mode of
operation to generate results in a file called
.CR tet_xres .
The format of the results contained in this file are dictated by the sections
entitled ``Journal and Results Files'' in both the TET and DTET
specifications.
.P
When set to
.CR True ,
.C dtcc
captures the
.I "standard output"
and
.I "standard error"
from the appropriate mode of operation and makes no attempt to open the 
.C tet_xres
file. 
.H 3 "The Alternate Execution Directory"
An alternate execution directory can be set using the
.C \-a
option of
.CR dtcc .
(See also the section entitled ``TET_EXECUTE'' later in these notes.)
Test cases are specified in
a scenario in terms of a relative directory path name ending with the test case
target name.
For each test case, the 
.I build
and
.I clean
modes of
.C dtcc
are initiated in the appropriate test
case directory which is relative to the test suite root directory.
The execute mode of
.C dtcc
is initiated in a directory that is either relative to 
the alternate execution directory (if it is set), or else the test suite root 
directory.
.R
.H 3 "The \f(CBTET_EXEC_IN_PLACE\fP Variable"
When set to
.C False
(the default) this variable instructs
.C dtcc
recursively to
copy the directory in which execution would have taken place (see 
the above section) to a new temporary directory (see the section
entitled ``TET_TMP_DIR'' in these notes).
The execute mode of
.C dtcc
then takes place in the temporary 
directory; following this, the temporary directory is recursively removed.
.br
.ne 6v
.H 2 "DTET Specification Notes"
.H 3 "Synchronisation Authority"
The DTET specification introduces the notion of synchronisation authority in
its description of the
.C tet_sync()
and
.C tet_remexec()
API functions, but does not currently define a mechanism by which such
authority may be passed between processes that run on a particular system.
This makes it difficult for the API to keep track of which process
on a particular system is
considered to have synchronisation authority assigned to it.
It is anticipated that this issue will be clarified as the DTET specification
evolves.
.P
For now, it remains the test suite writer's responsibility to ensure that
test processes that run on the same system do not attempt to make overlapping
.C tet_sync()
calls; otherwise (in this implementation, at least) chaos will ensue.
This is because synchronisation is requested in terms of a system name, and
does not discriminate between individual processes that may run on a
particular system.
.H 3 "Relationship Between TET and DTET Test Cases"
In this discussion, a TET test case is defined as
one that runs on a single system; this is referred to as a
.IR "non-distributed test case" .
A DTET test case is defined as a single test case
that has both a master part
and one or more slave parts and runs concurrently on several systems;
this is referred to as a
.IR "distributed test case" .
.P
The DTET test case controller
.C dtcc
is capable of executing both distributed and non-distributed test cases.
A distributed test case must be linked with the DTET API,
and a non-distributed test case must be linked with the TET API.
.P
The following modes of operation,
combinations of test case and system types,
scenario directives, and APIs are possible:
.de Li
.B
.LI "\\$1"
.R
.sp 0.1v
..
.VL 5
.Li "Distributed test case running on both the master system and one or \
more slave systems"
In this mode,
.C dtcc
executes all the test parts concurrently on all the participating
systems defined in the test scenario.
.P
The scenario names all the systems that are to participate in the
test, thus:
.Es
:remote,000,001,\fR.\|.\|.\fP:
/tcdir/TC.one
:endremote:
.Ee
.P
Note that the remote directive must include the master system
(system 000) when several system names appear.
.P
The part of the distributed
test that executes on the master system should be linked with
.C mtcm.o
and
.C libdapi.a.
.P
The parts of the distributed test that execute on
slave systems should each be linked with
.C stcm.o
and
.CR libdapi.a .
.P
A process that is to be started by a call to
.C tet_exec()
on any system should be linked with
.C dtcmchild.o
and
.CR libdapi.a .
.P
A process that is to be started on any system by a call to
.C tet_remexec()
should be linked with
.C tcmrem.o
and
.CR libdapi.a .
.Li  "Non-distributed test case running on the master system"
In this mode,
.C dtcc
operates in the same way as does the TET
.CR tcc .
This mode is provided for backward compatibility with the TET.
.P
The format of the test scenario
is the same as it would be if the test case was executed by
.C tcc
and does not specify a system, thus:
.Es
/tcdir/TC.one
.Ee
.P
The test case should be linked with
.C tcm.o
and
.CR libapi.a .
.P
A process that is to be started by
.C tet_exec()
should be linked with
.C tcmchild.o
and
.CR libapi.a .\*F
.FS
.CR tcm.o ,
.C tcmchild.o
and
.C libapi.a
are built from the TET distribution.
.FE
.Li "Non-distributed test case running on a single remote system"
In this mode,
.C dtcc
operates in the same way as does the TET
.CR tcc ,
except that the test case executes on the remote system defined in the test
scenario.
.P
The scenario names a single remote system, thus:
.Es
:remote,001:
/tcdir/TC.one
:endremote:
.Ee
.P
Note that the
.C remote
directive may not specify the master system (system 000) when only one
system name appears.
.P
The test case should be linked with
.C tcm.o
and
.CR libapi.a .
.P
A process that is to be started by
.C tet_exec()
should be linked with
.C tcmchild.o
and
.CR libapi.a .
.LE
.SP
.H 3 "DTET Build and Clean Tools"
The DTET Test Case Controller
.C dtcc
supports the use of both API-conforming and non API-conforming build and
clean tools.
However, when
.C dtcc
executes the
.I build
phase of a scenario that specifies a distributed test case, the
tools specified by
.C TET_BUILD_TOOL
on the participating systems must either be all API-conforming or
be all non API-conforming.
It is not possible to mix API-conforming and non API-conforming build
tools on different systems when building a distributed test.
.P
The same considerations apply to the types of clean tool specified by
.C TET_CLEAN_TOOL
on the participating systems when cleaning up a distributed test case.
.H 1 "Extensions to the Specifications"
The following features are implemented in addition to those
required by the TET and DTET specifications.
.H 2 "Environment Variables"
.R
.H 3 "\f(CBTET_ROOT\fP"
You can override the value of
.C TET_ROOT
compiled into
.C dtcc
by setting
.C TET_ROOT
in your shell environment before executing
.CR dtcc .
For example:
.BL
.LI
For the C shell, type the following line at the shell prompt:
.Es
setenv TET_ROOT \fIyour\-TET_ROOT\-path\fP
.Ee
.P
.LI
Or, for the Bourne shell, type the following line at the shell prompt:
.Es
TET_ROOT=\fIyour\-TET_ROOT\-path\fP
export TET_ROOT
.Ee
.P
By doing this, you can then refer to
.I your\-TET_ROOT\-path
as
.C $TET_ROOT
in the shell.
.LE
.SP
.R
.H 3 "\f(CBTET_EXECUTE\fP"
The environment variable
.C TET_EXECUTE
can be set in a similar fashion to that for
.CR TET_ROOT ,
and allows you to specify a default value for the
alternate execution directory that is used by
.CR dtcc .
Note, however, that a command line supplied value
for the alternate execution directory has a higher precedence than that
supplied by the
.C TET_EXECUTE
environment variable.
Also, note that only the location of the alternate execution directory 
on the master system can be specified in this way.
.R
.H 3 "\f(CBTET_TMP_DIR\fP"
During the execution of
.CR dtcc ,
temporary files are usually created in
unique sub_directories under a directory called
.CR tet_tmp_dir ,
which is located under the \*(tR directory.
Whilst every effort has been made to ensure
that each sub-directory is removed when
.C dtcc
terminates, it is possible 
that an abnormal termination will mean that a sub-directory remains.
Such directories should be removed manually in order to conserve disk space.
.P
You may specify an alternative temporary directory to be used by
.C dtcc
on the master system
by setting the environment variable
.CR TET_TMP_DIR .
This variable
should be set in a similar fashion to that for the
.C TET_ROOT
variable discussed above.
Any
performance problems due to the use of networked file systems
can be overcome by setting
this variable to a directory on a local file system.
.P
If you wish to specify
.C TET_TMPDIR
for a remote system, you may do this by
setting the appropriate
.CI TET_REM nnn _TET_TMP_DIR
variable in the
.C tetdist.cfg
configuration file.
.R
.H 2 "Extra \f(CBdtcc\fP Options"
The following
.C dtcc
command line options are provided in addition to those
in the specification:
.VL 7n
.C
.LI \-l
.R
may be used to supply a one-test scenario line for execution without the
need to create a scenario file.
If a scenario line needs to contain white space (e.g., because it
contains a
.C remote
directive, the test scenario should be presented as a single argument by
enclosing it in single or double quotes.
.C
.LI \-p
.R
enables progress reporting to standard output.
.if t .LI "\fC\-y\fR\ and\ \fC\-n\fR"
.if n .LI "\fB\-y\fR\ and\ \fB\-n\fR"
.br
may be used to select parts of a scenario for processing.  Test case
execution lines are processed only if they match the specified search
string
.RC ( \-y )
or don't match it
.RC ( \-n ).
.LE
.P
See the manual page in
.C \*(tR/doc/dtet/dtcc.1
for more details.
.H 2 "API Functions"
The following functions are implemented in addition to those
required by the current DTET specification:
.SP
.Es
void tet_exit(int \fIstatus\fP)
.Ee
.P
This function should be used instead of
.C exit()
by processes (linked with
.CR dtcmchild.o )
that are started by a call to
.CR tet_exec() ,
and
by processes (linked with
.CR tcmrem.o )
that are started by a call to
.CR tet_remexec() .
This function logs off all servers, then calls
.C exit()
with the specified
.I status
as argument.
.C tet_exit()
should only be called from the process that is started by
.C tet_exec()
or
.C tet_remexec()
and not by any of its children.
.SP
.Es
void tet_logoff()
.Ee
.P
This function may be called by processes (linked with
.CR dtcmchild.o )
that are started by a call to
.CR tet_exec() ,
and processes (linked with
.CR tcmrem.o )
that are started by a call to
.CR tet_remexec() ,
which do not need to make any further DTET API calls and are not able to call
.C tet_exit()
at process termination time (e.g., if one of the flavours of
.C exec() 
is about to be called without a prior
.CR fork() ).
.C tet_logoff()
should only be called once from a process that is started by
.C tet_exec()
or
.CR tet_remexec() .
The results are undefined
if a process or any of its descendents makes any DTET API calls after
.C tet_logoff()
is called.
.H 2 "File Name Changes"
It has been necessary to change the names and/or locations of some of the
DTET files
so as to allow the TET and DTET distributions to co-exist below a
single \*(tR directory.
.P
The name of the DTET Test Case Controller has been changed from
.C tcc
to
.CR dtcc .
.P
The DTET versions of the header files
.C tet_api.h
and
.C tet_jrnl.h
are located in the directory
.CR \*(tR/inc/dtet .
.H 1 "Network Security Considerations"
The TCC daemon
.C tccd
offers services to any network entity that satisfies its logon criteria.
Apart from the rudimentary host name checking performed against entries in the
.C systems.equiv
file (described previously), the security implications of running
.C tccd
on networked systems have not really been addressed.
Therefore, users are strongly advised against running the DTET on systems that
can be accessed from an external network unless
they are satisfied that adequate measures are in place
to prevent unauthorised access to networks that serve those systems.
.br
.TC
