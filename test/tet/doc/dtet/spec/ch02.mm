'''	SCCS: @(#) DTET SPECIFICATION		Rel 1.2 (9/15/92)
.nr H1 1
.H 1 Scope
.P
This specification makes the distinction between a master system and one or
more slave systems which take part in the execution of the DTET. The user 
executes an enhanced version of the test case controller (TCC) on the master
system which controls the execution of the DTET. When the appropriate scenario
directive is given, the master TCC (MTCC) interacts with a test case controlling
interface on each of the relevant slave systems (STCCs).
.P
An important distinction in the DTET is whether or not a test case is 
distributed. A test that is not distributed executes independently on the 
master system, makes no use of the DTET application programming interface 
(API), and involves no synchronisation. All test cases that conform to the
TET specification are in this category.
.P
A scenario file directive can be given that specifies a list of system
identifiers that apply to a test case. The DTET will, where appropriate,
concurrently build, execute and clean the test case on the systems specified.
The MTCC ensures that all the systems involved are synchronised at the
beginning and end of each mode of operation.
.P
The distributed features of the DTET are further enhanced by the inclusion of
the master system in the list of system identifiers. The test case manager
(TCM) that is linked to the test case executing on the master system is
responsible for controlling synchronisation within the test case. The master
test case manager (MTCM) initiates synchronisation between the concurrently
executing test cases at a test purpose level. The inclusion of the MTCM also
allows the user to issue specific synchronisation requests from within a
test purpose.
.P
