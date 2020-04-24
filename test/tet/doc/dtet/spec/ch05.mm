'''	SCCS: @(#) DTET SPECIFICATION		Rel 1.2 (9/15/92)
.nr H1 4
.H 1 "User Interface Specifications"
.P
.H 2 "Test Case Controller"
.P
This remains unchanged by the DTET and in the case of the DTET describes the
interface to the MTCC.
.P
.H 2 "API conforming test case"
.P
The DTET does not readily support the capability for individual test cases
to be executed without the invocation of the appropriate test case controllers.
This does not mean that this is impossible, but it is considered that the amount
of effort involved in setting up the environment on each of the slave systems
will generally outweigh the advantages of testing in this way. 
.P
.H 2 "Slave Test Case Controller"
.P
The Slave Test Case Controller forms the test case interface on a slave system.
It is responsible for translating requests from the MTCC into actions on the
slave system.
.P
The STCC transfers information regarding the progress of the requested actions
to the corresponding MTCC.
.P
The combination of MTCC and STCC must provide the functionality
necessary to build, execute and clean a test case in a transparent fashion
on a slave system.
.P
The \s-1\f7-a\fP\s0, \s-1\f7-f\fP\s0, \s-1\f7-g\fP\s0, \s-1\f7-i\fP\s0 and 
\s-1\f7-x\fP\s0 options of the MTCC do not affect the operation of the STCC.
.P
