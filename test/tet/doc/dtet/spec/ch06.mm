'''	SCCS: @(#) DTET SPECIFICATION		Rel 1.2 (9/15/92)
.nr H1 5
.H 1 "Application Programming Interfaces"
.P
The DTET does not provide a Shell Language interface for distributed test
suites. The standard TET Shell Language interface can still be used to
generate test cases which execute under the MTCC, but no support is provided
for tests executing partly on a master and partly on slave systems.
.P
The C Language interface is enhanced by the addition of a series of new
interfaces usable in the DTET. There are also some additions to the
semantics of the current interfaces which are only effective in a DTET
environment. The syntax of the current interfaces is unchanged.
.P
.H 2 "C Language Binding"
.P
Applications written to this language binding attach themselves to it
through the following files:
.VL 6n 0n 1
.LI \s-1\f7TET_ROOT/lib/dtet/libdapi.a\fP\s0
.br
contains the DTET support routines for test purposes in both the MTCM and the
STCM.
.sp 0.5v
.LI \s-1\f7TET_ROOT/lib/dtet/mtcm.o\fP\s0
.br
contains the routine \fImain\fP() and associated support routines for the
sequencing and control of invocable components and test purposes in the MTCM.
.sp 0.5v
.LI \s-1\f7TET_ROOT/lib/dtet/stcm.o\fP\s0
.br
contains the routine \fImain\fP() and associated support routines for the
sequencing and control of test purposes in the STCM.
.sp 0.5v
.LI \s-1\f7TET_ROOT/lib/dtet/tcmrem.o\fP\s0
.br
contains a \fImain\fP() routine which can be used by test suites when building
processes which test purposes will launch using the \fItet_remexec\fP()
interface.
.LE
.P
.H 3 "Test Case Structure and Management Functions"
.P
The MTCM and STCM adhere to the same structure as that provided for the TCM
in TET.
.P
.H 3 "Insulating from the Test Environment"
.P
Each process in a distributed test case is insulated from its own environment
in the same manner that a local test case is insulated in the TET environment.
The values of the configuration variables TET_SIG_IGN and TET_SIG_LEAVE may
differ between different systems that are executing parts of a test case 
concurrently.
.P
.H 3 "Making Journal Entries"
.P
The DTET makes available to each of the processes involved in a distributed
test case the same functions as those specified for the TET. The DTET versions
of these functions include an additional system designation of the form
\fInnn\fP at the start of the context. This allows the source of each 
information line to be identified by a reporting procedure without changes
being needed to the journal format or to the re-ordering algorithm.
.P
.H 3 "Cancelling Test Purposes"
.P
The DTET makes available to each of the processes involved in a distributed
test case the same functions as those specified for the TET.
.P
.H 3 "Manipulating Configuration Variables"
.P
The DTET makes available to each of the processes involved in a distributed
test case the same functions as those specified for the TET. Each process
retrieves the requested data from its own set of configuration variables.
.P
.H 3 "Generating and Executing Processes"
.P
The DTET makes available to each of the processes involved in a distributed
test case the same functions as those specified for the TET. 
.P
In the DTET environment the additional problem exists of allowing both the
process which calls the \s-1\f7tet_fork()\fP\s0 function
and the generated process to synchronise with other processes which
form part of the test case. 
The test writer is responsible for limiting synchronisation to either 
the \s-1\f7parentproc\fP\s0 or \s-1\f7childproc\fP\s0.
.P
.H 3 "Executed Process Functions"
.P
The DTET allows for the existence of both locally and remotely executed
processes. The syntax for the \fItet_main\fP() associated with these two
types of process is identical, though the underlying behaviour is considerably
different. The behaviour visible to the test suite author is as described in
the TET, except that the remote execution version is obtained from
\s-1\f7tcmrem.o\fP\s0 rather than \s-1\f7tcmchild.o\fP\s0. 
.P
.SK
.P
.H 3 "Test Purpose Synchronisation" 
.P
These functions allow different parts of a test purpose to synchronise to an
agreed point in the executing code.
.P
.H 4 Synopsis
.P
.DS
.S -1
.ft 7
	int tet_sync (int syncptno, int *syncnames, int waittime);
.ft 1
.S +1
.DE
.P
.H 4 Description
.P
The function \fItet_sync\fP() causes the calling process to synchronise with
those STCMs designated in the NULL terminated array pointed to by 
\s-1\f7syncnames\fP\s0. All synchronisation is moderated through the
MTCM and the MTCM is not included in the \s-1\f7syncnames\fP\s0 list.
If \s-1\f7syncnames\fP\s0 is a null pointer, then the synchronisation request
must be called from an STCM and synchronisation is made only with the MTCM.
When \fItet_sync\fP() is called from the MTCM, \s-1\f7syncnames\fP\s0
cannot be a null pointer and a list of STCM designations must be given.
.P
The \s-1\f7syncptno\fP\s0 variable is used to ensure that all of the processes
have reached an agreed point in the executing code. If the \s-1\f7syncptno\fP\s0
is set to zero the next synchronisation point in each process is assumed, rather
than a designated synchronisation point.
.P
The \s-1\f7waittime\fP\s0 argument acts as a mechanism which specifies the
period of time to wait for synchronisation to occur. 
When synchronisation is requested from more than one system, each system
will be expected to register its response. In this case \s-1\f7waittime\fP\s0 
specifies the maximum interval that can occur between each response.
If \s-1\f7waittime\fP\s0 is zero, \fItet_sync()\fP returns immediately without
waiting for other systems to synchronise which have not already done so.
If \s-1\f7waittime\fP\s0 is negative, \fItet_sync()\fP waits indefinately 
for all the systems to synchronise (or terminate).
.P
The call to \fItet_sync\fP() returns zero when all of the requested processes
have synchronised to the requested synchronisation point. If an error occurs
in any of the processes with which synchronisation has been requested, then
-1 is returned and the MTCM places and entry in the \s-1\f7tet_xres\fP\s0
file detailing the system which failed to honour the synchronisation
request. 
.P
If a \fItet_sync()\fP call fails, an entry is made in the journal file
which includes sufficient information to determine which process
failed to synchronise correctly.
.SK
.P
.H 3 "Remote System Designations"
.P
These functions allow a DTET test purpose to retrieve information about 
system designations.
.P
.H 4 Synopsis
.P
.DS
.S -1
.ft 7
	int tet_remgetlist (int **sysnames);

	int tet_remgetsys (void);
.ft 1
.S +1
.DE
.P
.H 4 Description
.P
The \fItet_remgetlist\fP() function returns the number of slave systems
which are currently available. If there is at least one slave system available,
a pointer to an array containing the names of the available slave systems is
returned indirectly through \s-1\f7*sysnames\fP\s0.
.P
The \fItet_remgetsys\fP() function returns the system designation of the
slave system on which the calling process is executing. The designation of
the master system will always be zero.
.SK
.P
.H 3 "Remote Process Control"
.P
These functions allow a DTET test purpose executing on the MTCM to generate
a remote process on a slave.
.P
.H 4 Synopsis
.P
.DS
.S -1
.ft 7
	int tet_remexec (int sysname, char *file, char *argv[]);

	int tet_remwait (int remoteid, int waittime, int *stat_loc);

	int tet_remkill (int remoteid);
.ft 1
.S +1
.DE
.P
.H 4 Description
.P
The \fItet_remexec\fP() function may be called from any process under the
control of the MTCM. The test writer should ensure that the calling process 
has synchronisation authority assigned to it. 
The calling process will wait until the requested process has been started 
and has synchronised with it. 
.P
[ Note: This restricts \fItet_remexec\fP() to generating one process on each
separate system. 
This is because there is no provision for discriminating between processes 
of the same type on the same system.]
.P
Upon successful synchronisation the call to \fItet_remexec\fP() returns the
\s-1\f7remoteid\fP\s0 as a value greater than zero. 
If the call to \fItet_remexec\fP() fails, a value of -1 is returned.
.P
The \s-1\f7sysname\fP\s0 argument is mapped to the \fInnn\fP designation of
a remote systems and the corresponding STCC is requested to initiate the
process specified by \s-1\f7file\fP\s0. The location of \s-1\f7file\fP\s0
is relative to the remote system's TET_EXECUTE directory if set, otherwise, it 
is relative to TET_ROOT.
.P
The \fItet_remexec\fP() function passes the argument data as specified by 
\s-1\f7argv\fP\s0 to the process specified by \s-1\f7file\fP\s0. The usage
of \fItet_remexec\fP() is similar to the ISO 9945-1 \fIexecv\fP() function.
.P
The call to \fItet_remexec\fP() returns -1 and sets \fIerrno\fP to EINVAL
if \s-1\f7sysname\fP\s0 does not refer to a known slave system.
.P
The call to \fItet_remexec\fP() returns -1 and sets \fIerrno\fP to ENOEXEC
if \s-1\f7file\fP\s0 cannot be executed on the slave system.
.P
[ Note: The environment is not passed as there is not expected to be any
correlation between the environment information on the remote machine to
that of the calling process. Any data that is need by the remote process
must be passed as an argument. ]
.P
The \fItet_remwait\fP() function waits for the termination of a remote
process initiated by \fItet_remexec\fP(). The \s-1\f7remoteid\fP\s0 argument
is the remote execution identifier returned from the call to 
\fItet_remexec\fP().
.P
The call to \fItet_remwait\fP() provides the exit status of the remote process
in the integer pointed to by \s-1\f7stat_loc\fP\s0 and returns zero if the
call has completed successfully. If the call to \fItet_remwait\fP() fails to
complete after \s-1\f7waittime\fP\s0 or fails for any other reason a value
of -1 is returned. When \s-1\f7waittime\fP\s0 is set to zero, the call to
\fItet_remwait\fP() will return immediately, either with an error if the
requested process has not yet terminated or with the exit status if the
requested process has already terminated.
.P
The call to \fItet_remwait\fP() returns -1 and sets \fIerrno\fP to EINVAL
if \s-1\f7remoteid\fP\s0 does not refer to a process initiated from a call
to \fItet_remexec\fP().
.P
The call to \fItet_remwait\fP() returns -1 and sets \fIerrno\fP to ECHILD
if \s-1\f7remoteid\fP\s0 refers to a process which is already the subject
of a successful call to \fItet_remwait\fP().
.P
The call to \fItet_remwait\fP() returns -1 and sets \fIerrno\fP to EAGAIN
if \s-1\f7timeout\fP\s0 elapses.
.P
The \fItet_remkill\fP() function causes the STCC which controls the remote
process designated by \s-1\f7remoteid\fP\s0 to terminate the process. The
\fItet_remkill\fP() call returns immediately without awaiting confirmation
that the process has terminated. (This information can be obtained from a
subsequent call to \fItet_remwait\fP() if required).
.P
The call to \fItet_remkill\fP() returns -1 and sets \fIerrno\fP to EINVAL
if \s-1\f7remoteid\fP\s0 does not refer to a process initiated from a call
to \fItet_remexec\fP().
.SK
.P
.H 2 "Shell Language Bindings"
.P
The Shell Language Binding is as described in the TET document and no additional
support functions are defined for the DTET.
.P
