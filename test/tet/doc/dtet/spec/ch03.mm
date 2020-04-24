'''	SCCS: @(#) DTET SPECIFICATION		Rel 1.2 (9/15/92)
.nr H1 2
.H 1 Architecture
.P
.H 2 Terminology
.P
.H 3 Abbreviations
.P
.VL 8n
.LI DTC
A Distributed Test Case
.LI MTCC
The Master Test Case Controller
.LI MTCM
The Master Test Case Manager
.LI STCC
The Slave Test Case Controller
.LI STCM
The Slave Test Case Manager
.LI TCC
The original TET Test Case Controller
.LI TCM
The original TET Test Case Manager
.LE
.H 3 Definitions
.VL 6n
.LI "Configuration Variables"
.br
Two types of configuration variables are used by the DTET. Those that are used
to inform the MTCC of specific details that it needs to interact properly with
each of the slave systems, and, those that used by the MTCC and API to change
the operational behaviour of the DTET in the same fashion as the TET.
.LI "Distributed Test Case"
.br
A test case that utilises the distributed features of the DTET.
.LI "Execution Results"
.br
Test cases, via the API, place their results into an execution results file.
Distributed test cases not involving the master system have their execution 
results transferred at the end of the test case to the master system, where they
are sequentially entered into the journal file. Distributed test cases that use
the master system and the MTCM have a combined execution results file. Entries
are transferred from each of the local execution results files to the combined
execution results file at each synchronisation point.
.LI "Master System"
.br
The system from which the building, execution and cleaning of the tests
is controlled. This system contains the test scenario for a particular
DTET invocation and transmits information to each of the slave systems
in order that they undertake the necessary tasks as specified in the
scenario file. Each DTET invocation has exactly one master system.
.LI "MTCC"
.br
The TCC run by the user on the master system is referred to as the MTCC.
When invoked in build mode, the
MTCC sends requests, to each of the remote systems involved, 
to build the relevant parts of the distributed test case. If
appropriate, the MTCC will also build those parts of the test case which 
reside on the master system.  The MTCC performs
the corresponding functions in execute mode and clean mode.
In the absence of a remote directive in the scenario file, the MTCC
behaves in a manner identical to the TCC.
.LI "MTCM"
.br
The MTCM is a component of the DTET API.
This component interfaces between the MTCC and the test suite specific
part of the test case which is executing on the master system.
.P
The MTCM is responsible for controlling synchronisation between the master
part of a test case and each of the slave parts of the test case at a sub
test case level.
.LI "Prebuild file"
.br
A prebuild file is a set of test suite defined instructions to the
prebuild tool for use in preparing for the building of executable
versions of test cases.
.LI "Prebuild tool"
.br
The MTCC uses a test suite defined prebuild tool to undertake the
preparation for the distributed build. The prebuild phase is only
undertaken on the master system, whereas the build phase is undertaken
on the master system and each of the slave systems concurrently.
The prebuild phase is optional and allows actions such as the transfer
of source files from the master to the slave systems in preparation
for the local build phase. (This assumes that a file transfer mechanism
is available, which is not required by the DTET specification).
.LI "Slave System"
.br
The system on which a remote part of the build, execution and 
cleaning of the test case is undertaken. The remote part may be a stand alone
test case or part of a distributed test case. A DTET invocation
may have any number of slave systems according to the parameters
directives in the scenario file. The slave system may be the same
as the master system.
.LI "STCC"
.br
The STCC is the test case controlling interface on the slave system. It receives
requests from the MTCC to undertake the local part of the build,
clean or execute. The actions of the STCC will be initiated on the receipt
of a request from the MTCC.
The STCC is responsible for transmitting completion status information
to the MTCC as each of the requested elements is completed on the
slave system.
.LI "STCM"
.br
The STCM is a component of the DTET API.
This component interfaces between the STCC and the test suite specific
part of the test case which is executing on the slave system. The
STCM participates in synchronisation between the slave and the master 
system.
.LI Synchronisation
.br
The process of ensuring that each part of the distributed test case
residing on the master and slave systems has reached an agreed point
in its execution. Certain synchronisation points are negotiated
automatically by the STCM and MTCM (e.g. at test purpose start) while
others are test case specific and are negotiated by the master with
one or more of the slaves. Synchronisation is in the form of a full
handshake.
.LE
.P
.H 2 "Conceptual Model"
.P
The following two, complete, diagrams describe the files and untilities that
interact in the processing of a distributed test case. When a \fBremote\fR
directive is specified that does not include the master system, the \fIMaster
Test Case\fR and its associated connections become redundant. For cases where a
\fBremote\fR directive is not being processed the conceptual model used for
the TET applies.
.P
.SK
.DS CB
.SP 2
.S +2
.ce 1
\fBDTET Conceptual Model - Master System\fR
.S -2
.SP 3
.PS
# ********************  profile definition   *********************
# profile drive: pic code
# 18 May 1987 by T. Monroe
paa = 1.75	# width of profile
pbb = paa/5	# height of profile
pcc = pbb/10	# offset of "trim"
pff = pcc*2	# width of top of foot

define pfoot
     @
	line right pff
	line down pff*0.75 left pff/6
	line left 2*pff/3 
	line up pff*0.75 left pff/6
     @

define profile
     @
.ps 11
	PA: box wid paa ht pbb
.ps 10
	PB: box wid paa-(2*pcc) ht pbb-pcc with .n at PA.n
	PC: box ht 2*pcc wid 6*pcc with .sw at PB.sw + 6*pcc,2*pcc
	PD: box ht 2*pcc wid 2*pcc with .e at PC.w - pcc/2,0
	PE: box ht 2*pcc wid 2*pcc with .se at PB.se - 4*pcc,-2*pcc
	# the feet
	[ pfoot ] with .nw at PA.sw + 3*pcc,0
	[ pfoot ] with .ne at PA.se - 3*pcc,0
	[ pfoot ] with .nw at PA.s  + 3*pcc,0
.ps 12
     @
# ********************  profile definition   *********************

	scale=1.25
	boxwid = 1.5i; boxht = 0.75i
	ellipsewid = 1.5i; ellipseht = 0.75i
	movewid = 0.0i
.\"
.\" Start with the Build file ellipse at the top, and slightly
.\" right of centre then work down that 'column'
.\"
	move right 4.0i
BLDF:	ellipse "Build file"
BLDTL:	box "\s+2\fBBuild tool\fP\s0" with .c at BLDF.c + (0.0i, -1.0i)
PBLDF:	ellipse "Prebuild file" with .c at BLDF.c + (-3.9i, 0.0i)
PBLDTL:	box "\s+2\fBPrebuild tool\fP\s0" with .c at BLDF.c + (-3.9i, -1.0i)
CLNTL:	box "\s+2\fBClean tool\fP\s0" with .c at BLDF.c + (0.0i, -2.0i)
.\"	ellipsewid = 1.0i; ellipseht = 0.5i
CLNF:	ellipse "Clean file" with .c at BLDF.c + (0.0i, -3.0i)
EXECRS:	ellipse "Execution" "Results" with .c at BLDF.c + (0.0i, -4.0i)
RESCOD:	ellipse "Result" "Codes" with .c at BLDF.c + (-4.0i, -6.0i)
	ellipsewid = 1.5i; ellipseht = 0.75i
	move to BLDF.c + (0.0i, -6.0i)
REMOTE:	[ profile ]
	move to BLDF.c + (2.2i, -6.0i)
	"\s+2Slave\s0" "\s+2System\s0" 
.\"
.\" Configuration files and the scenario file ellipses are on the left,
.\" clustered around the TCC box
.\"
BCONF:	ellipse "Build" "Configuration" with .c at BLDF.c + (-2.0i, -1.5i)
CCONF:	ellipse "Clean" "Configuration" with .c at BLDF.c + (-4.0i, -2.5i)
ECONF:	ellipse "Execution" "Configuration" with .c at BLDF.c + (-4.0i, -4.0i)
SCEN:	ellipse "Scenario" with .c at BLDF.c + (-4.0i, -5.0i)
JNL:	ellipse "Journal" with .c at BLDF.c + (-2.0i, -6.0)
	boxwid = 1.5i; boxht = 1.0i
TCC:	box "\s+2\fBMaster\fP\s0" "\s+2\fBTest Case\fP\s0" "\s+2\fBController\fP\s0" with .c at BLDF.c + (-2.0i, -4.0i)
	boxwid = 1.5i; boxht = 0.75i
.\"
.\" Report-related boxes are positioned relative to the Journal ellipse
.\"
RPTGEN:	box "\s+2\fBReport\fP\s0" "\s+2\fBGenerator\fP\s0" with .c at JNL.c + (0.0i, -1.0i)
TRTMNT:	box "\s+2\fBTreatment\fP\s0" "\s+2\fBFilter\fP\s0"with .c at RPTGEN.c + (2.0i, 0.0i)
FMTRPT:	ellipse "Formated" "Report" with .c at TRTMNT.c + (2.0i, 0.0i)
.\"
.\" Test Case boxes structure is out on the extreme right.
.\" Simulate 3 boxes 'stacked' at a slight offset by having two of
.\" them invisible and drawing lines along part of their edges.
.\"
	boxwid = 1.5i; boxht = 1.0i
TCASES:	box "\s+2\fBMaster\fP\s0" "\s+2\fBTest Case\fP\s0" with .c at TCC.c + (4.2i, 0.0i)
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
	boxwid = 1.5i; boxht = 0.75i
.\"
.\" Then draw the connecting arrows between the boxes and ellipses
.\"
	arrow from BLDF.s to BLDTL.n
	arrow from PBLDF.s to PBLDTL.n
	line <-> from BLDTL.w to 1/2 of the way between TCC.n and TCC.ne
	line <-> from PBLDTL.e to 1/2 of the way between TCC.nw and TCC.n
	line <-> from CLNTL.w to TCC.ne
	arrow from CLNF.n to CLNTL.s
	arrow from BCONF.s to TCC.n
	arrow from CCONF.se to TCC.nw
	arrow from ECONF.e to TCC.w
	arrow from SCEN.ne to TCC.sw
	arrow from TCC.s to JNL.n
	arrow from EXECRS.w to TCC.e
	arrow from RESCOD.ne to 1/2 of the way between TCC.s and TCC.sw
	arrow from TCC.se to TCASES.sw
	arrow dashed from BLDTL.e to TCASES.n "\f2Produces\f1" 
	arrow dashed from CLNTL.e to TCASES.nw "\f2Removes\f1" 
	arrow from TCASES.w to EXECRS.e
	arrow from JNL.s to RPTGEN.n
	arrow from RPTGEN.e to TRTMNT.w
	arrow from TRTMNT.e to FMTRPT.w
	arrow dashed from 1/2 of the way between TCC.s and TCC.se to REMOTE.nw "\f2Communicates\f1" "\f2With\f1"
	arrow dashed from TC2.sw to REMOTE.n
.\"
.\" Finally reset box dimensions to their defaults
.\"
	boxwid = 0.75i; boxht = 0.5i
	movwid = 0.5i
.PE
.DE
.SK
.DS CB
.SP 3
.S +2
.ce 1
\fBDTET Conceptual Model - Slave System\fR
.S -2
.SP 3
.PS
# ********************  profile definition   *********************
# profile drive: pic code
# 18 May 1987 by T. Monroe
paa = 1.75	# width of profile
pbb = paa/5	# height of profile
pcc = pbb/10	# offset of "trim"
pff = pcc*2	# width of top of foot

define pfoot
     @
	line right pff
	line down pff*0.75 left pff/6
	line left 2*pff/3 
	line up pff*0.75 left pff/6
     @

define profile
     @
.ps 11
	PA: box wid paa ht pbb
.ps 10
	PB: box wid paa-(2*pcc) ht pbb-pcc with .n at PA.n
	PC: box ht 2*pcc wid 6*pcc with .sw at PB.sw + 6*pcc,2*pcc
	PD: box ht 2*pcc wid 2*pcc with .e at PC.w - pcc/2,0
	PE: box ht 2*pcc wid 2*pcc with .se at PB.se - 4*pcc,-2*pcc
	# the feet
	[ pfoot ] with .nw at PA.sw + 3*pcc,0
	[ pfoot ] with .ne at PA.se - 3*pcc,0
	[ pfoot ] with .nw at PA.s  + 3*pcc,0
.ps 12
     @
# ********************  profile definition   *********************

	scale=1.25
	boxwid = 1.5i; boxht = 0.75i
	ellipsewid = 1.75i; ellipseht = 0.8i
	movewid = 0.0i
.\"
.\" Start with the Build file ellipse at the top, and slightly
.\" right of centre then work down that 'column'
.\"
	move right 4.0i
BLDF:	ellipse "Build file"
BLDTL:	box "\s+2\fBBuild tool\fP\s0" with .c at BLDF.c + (0.0i, -1.0i)
CLNTL:	box "\s+2\fBClean tool\fP\s0" with .c at BLDF.c + (0.0i, -2.0i)
.\"	ellipsewid = 1.0i; ellipseht = 0.5i
CLNF:	ellipse "Clean file" with .c at BLDF.c + (0.0i, -3.0i)
EXECRS:	ellipse "Execution" "Results" with .c at BLDF.c + (0.0i, -4.0i)
	ellipsewid = 1.75i; ellipseht = 0.8i
	move to BLDF.c + (0.0i, -6.0i)
REMOTE:	[ profile ]
	move to BLDF.c + (2.2i, -6.0i)
	"\s+2Master\s0" "\s+2System\s0" 
.\"
.\" Configuration files and the scenario file ellipses are on the left,
.\" clustered around the TCC box
.\"
BCONF:	ellipse "Local Build" "Configuration" with .c at BLDF.c + (-2.0i, -1.5i)
CCONF:	ellipse "Local Clean" "Configuration" with .c at BLDF.c + (-4.0i, -2.5i)
ECONF:	ellipse "Local Execution" "Configuration" with .c at BLDF.c + (-4.0i, -4.0i)
	boxwid = 1.5i; boxht = 1.0i
TCC:	box "\s+2\fBSlave\fP\s0" "\s+2\fBTest Case\fP\s0" "\s+2\fBController\fP\s0" with .c at BLDF.c + (-2.0i, -4.0i)
	boxwid = 1.5i; boxht = 0.75i
.\"
.\" Test Case boxes structure is out on the extreme right.
.\" Simulate 3 boxes 'stacked' at a slight offset by having two of
.\" them invisible and drawing lines along part of their edges.
.\"
	boxwid = 1.5i; boxht = 1.0i
TCASES:	box "\s+2\fBSlave\fP\s0" "\s+2\fBTest Case\fP\s0" with .c at TCC.c + (4.2i, 0.0i)
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
	boxwid = 1.5i; boxht = 0.75i
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
	arrow from EXECRS.w to TCC.e
	arrow from TCC.se to TCASES.sw
	arrow dashed from BLDTL.e to TCASES.n "\f2Produces\f1" 
	arrow dashed from CLNTL.e to TCASES.nw "\f2Removes\f1" 
	arrow from TCASES.w to EXECRS.e
	line dashed <-> from 1/2 of the way between TCC.s and TCC.se to REMOTE.nw "\f2Communicates\f1" "\f2With\f1"
	line dashed <-> from TC2.sw to REMOTE.n
	arrow dashed from EXECRS.s to 1/2 of the way between REMOTE.nw and REMOTE.n
.\"
.\" Finally reset box dimensions to their defaults
.\"
	boxwid = 0.75i; boxht = 0.5i
	movwid = 0.5i
.PE
.DE
.SK
.S +2
.ce 1
\fBDTET Conceptual Model - Master - Slave Interaction\fR
.S -2
.SP 2
If a scenario contains any \fBremote\fP directives, the MTCC should initiate
the following phase for each slave system designated by a \fBremote\fP
directive anywhere in that scenario.
.TS
allbox center tab(:);
cfB s
c   c
l l.
Start-Up Phase
MTCC:STCC
T{
Builds list of remote system IDs
T}:T{
Awaits information from the MTCC
T}
T{
Sends list of remote systems to each STCC
T}:T{
Receives remote system list
T}
T{
Sends configuration data for each mode to each STCC
T}:T{
Receives configuration data and combines it with local configuration data
T}
T{
Receives combined configuration data and writes it to the journal file
T}:T{
Sends combined configuration data to MTCC
T}
.TE
.SP 1
The following phases describe the master-slave interaction for test cases within
a \fBremote\fP directive.
.P
.TS
allbox center tab(:);
cfB s
c   c
l l.
\fBBuild Phase\fP
MTCC:STCC
T{
Executes a prebuild tool (if specified)
T}:T{
Awaits information from the MTCC
T}
T{
Sends build requirements to each STCC
T}:T{
Receives build requirements from MTCC
T}
T{
Syncs with STCC
T}:T{
Syncs with MTCC
T}
T{
Builds master part of DTC \fIif\fP the master system was specified within the \fBremote\fP directive.
T}:T{
Builds slave part of DTC
T}
T{
Syncs with STCC
T}:T{
Syncs with MTCC
T}
T{
Collects build output files from each STCC and consolidates them into the
journal file
T}:T{
Sends build output files to MTCC
T}
T{
Syncs with STCC
T}:T{
Syncs with MTCC
T}
.TE
.SK
.TS
allbox center tab(:);
cfB s
c   c
l l.
\fBExecute Phase\fP
MTCC:STCC
T{
Sends execute requirements to each STCC
T}:T{
Receives execute requirements from MTCC
T}
T{
Syncs with STCC
T}:T{
Syncs with MTCC
T}
T{
Executes master part of DTC \fIif\fP the master system was specified within the \fBremote\fP directive.
T}:T{
Executes slave part of DTC
T}
T{
Syncs with STCC at the start of each TP
T}:T{
Syncs with MTCC
T}
T{
Syncs with STCC at the end of each TP
T}:T{
Syncs with MTCC
T}
T{
Collects results at the end of each TP, from each STCC, and writes them to the journal file
T}:T{
Sends build output to MTCC
T}
T{
Syncs with STCC
T}:T{
Syncs with MTCC
T}
.TE
.SP 3
.TS
allbox center tab(:);
cfB s
c   c
l l.
\fBClean Phase\fP
MTCC:STCC
T{
Sends clean requirements to each STCC
T}:T{
Receives clean requirements from MTCC
T}
T{
Syncs with STCC
T}:T{
Syncs with MTCC
T}
T{
Cleans master part of DTC \fIif\fP the master system was specified within the \fBremote\fP directive.
T}:T{
Cleans slave part of DTC
T}
T{
Syncs with STCC
T}:T{
Syncs with MTCC
T}
T{
Collects results from east STCC and writes them to the journal file.
T}:T{
Sends build results to MTCC.
T}
T{
Syncs with STCC
T}:T{
Syncs with MTCC
T}
.TE
.SK
.P
.H 2 "Structural Description"
.P
The basic directory structure for DTET is the same as that for TET, though
the structure will need to be replicated on each of the slave systems. The DTET
will either need to be separately installed and built on each system, or in
the case of homogeneous systems, built on the master system and the bin, lib,
include and test suite directories under TET_ROOT transferred to each slave system. 
.P
For each remote system specified in the scenario, the TET_ROOT and test suite 
root directories must be known locally for that system. If the user wishes to 
specify an alternate execution directory on the remote system, this information
must also be communicated to the remote system.
In order that the master system can communicate these variables, the
configuration identifiers TET_REM\fInnn\fR_TET_ROOT, TET_REM\fInnn\fR_TET_TSROOT
and TET_REM\fInnn\fR_TET_EXECUTE are reserved for this purpose. 
.P
.H 2 "System Requirements"
.P
.VL 6n
.LI "Atomic File Writes"
.br
There is no requirement on the transport mechanism to transmit more than a
single byte as an atomic unit. The implementation over a particular transport
mechanism is required to ensure the integrity of data in units of at least
512 bytes.
.LI "Date/Time Facility"
.br
There is no requirement that there be synchronisation of date and time between
the master and slave systems.
.LI "Transport Mechanism"
.br
The master system and each of the slave systems must support a common transport
mechanism which allows the dispatch and receipt of data between the master and
the slave system. The endpoints used by the master and slave must ensure that
data dispatched from one system is clearly routed to the other system. (In terms
of asynchronous connection, this requires that two lines are established between
the master system and each slave system).
.LE
.P
.H 3 "Future Directions"
.P
.VL 6n
.LI Internationalisation
.br
The support for internationalisation requires that the master system and each
of the slave systems use a common character set, locale definition and message
cataloguing system.
.LE
.P
.H 2 "Test Suite Structure"
.P
The test suite directory structure is replicated to each of the slave systems.
.P
.H 3 "Required Files"
.P
.VL 6n 0n 1
.LI "Configuration Variable Setting"
.br
An extra configuration file is used by the DTET to supply information to the 
MTCC that is specific to the distributed aspects of the DTET. The format of
this file is described in section 7.1.2.
.LI "Execution Results Files"
.br
Execution results files are produced in the same fashion as for the TET. For
distributed test cases not using a master system component the 
\s-1\f7tet_xres\fP\s0 file on each of the slave systems is processed upon 
completion of test case execution. Distributed test cases using the master
system generate a single combined execution results file. At each 
synchronisation point results from all the test cases on the relevant systems
is transferred into the combined \s-1\f7tet_xres\fP\s0 file. At the end of 
tests case execution the MTCC processes the combined file.
.LI "Result Code Definitions"
.br
Only the result code definitions file on the master system is consulted. The
result codes are made available to test cases executing on a slave system in a
transparent fashion.
.LI "System Identifier Definitions"
.br
The DTET uses information contained in the system definitions file to translate
non-specific numeric system identifiers into specific system names. The format
of this file is described in section 7.1.4.
.LE
.nr H3 +1
.P
.H 3 "Optional Utilities"
.P
.VL 6n 0n 1
.LI "Prebuild Tool"
.br
This utility is used by the MTCC to undertake the
preparation for the distributed build. The prebuild phase is only
undertaken on the master system, whereas the build phase is undertaken
on the master system and each of the slave systems concurrently.
The prebuild phase is optional and allows actions such as the transfer
of source files from the master to the slave systems in preparation
for the local build phase. (This assumes that a file transfer mechanism
is available).
.LE
.P
.H 2 "Test Case Structure"
.P
The test case structure for the DTET is further complicated by the
interaction between those parts of the test purpose which execute on a slave
system and those parts that execute on the master system. The following points
 should be noted:
.AL
.LI
The master test purpose and the slave test purposes do not interoperate. 
Any interoperation between these two entities that is
not made through the API is expected to be for the purpose of performing
the test. Indeed the API may use a totally different transport mechanism
from that used in the test, a technique which minimises the reliance on
the transport mechanism under test.
.LI
One slave system is not allowed to directly interact with another slave
system. All interactions are through the master system. The DTET API
allows one slave to request the master to synchronise with itself and a
series of other slave systems. 
.LI
When the MTCM is not included no synchronisation occurs between the test case
elements that form a distributed test case.
.LE
.P
.H 2 "Synchronisation"
.P
The MTCC synchronises all the elements of a distributed test case at the
beginning and end of each build, execute and clean.
.P
Specifying the master system, \fI000\fP as one of the designations of a remote 
directive allows for automatic synchronisation to occur between master and
slave components of a test case.  In addition the DTET API allows the master
or slave components of a test purpose to request synchronisation before
proceeding with the remainder of the test purpose. It is the responsibility
of the test case author to ensure that requested synchronisations are matched
between the various components of a test case.
The DTET TCM allows the synchronisation request to designate a specific synchronisation point
in the test with which to synchronise. This allows intermediate synchronisation points
to be omitted if desired.
.P
The user defined synchronisation points and the automatic
synchronisation points are also differentiated in order that synchronisation at the
end of a TP is not confused with a user synchronisation request. The synchronisation point at
the end of a TP is also used as an opportunity for test deletion information
to be transmitted between processes.
.P
At each synchronisation point the information accumulated by each slave system
since the last synchronisation point is transferred to the master system and
consolidated into a single \s-1\f7tet_xres\fP\s0 file.
At the end of each TC, the consolidated \s-1\f7tet_xres\fP\s0 file
is added to the journal file. 
.P
