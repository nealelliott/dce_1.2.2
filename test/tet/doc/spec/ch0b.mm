'\"	SCCS: @(#) TET SPECIFICATION		Rel 4.5 (06/23/92)
'\"
.nr H1 1
.nr Ej 1
.nr Cl 2
.HM A 1 1 1 1 1 1
.HU "Appendix B - Requirements"
'\" 
'\" currently unsatisfied requirements:
'\"
'\"	2.5 - documentation extraction
'\"	3.4 - Network testing
'\"	4.6 - test synchronisation
'\"	4.10 - API documenting statically used resources
'\"
.B Participants
.P
The Project asked for requirements input from a number of
organisations. These organisations represented Government,
Development, and Conformance Testing interests. Through their
contributions, the Project believes that it has received the broad
input necessary to make the work valid throughout the testing
community.
.P
.B Approach
.P
In the following pages, the requirements submitted 
have been categorised and,
where possible, combined into single instances (many
requirements were repeated by a number of the participants).
The requirements have been analysed and prioritised
by the Project. In making this
summary, it is hoped that the essence of the original
requirements has been maintained
.P
The requirements are further categorised by their relative importance.
The categories are:
.VL .5i
.LI "Parenthood"
.P
The requirement is a base assumption of any testing support software,
and is therefore automatic.
.LI "Essential"
.P
The requirement must be in any Toolkit implementation in order to
satisfy the goals of the Project.
.LI "Nice"
.P
The requirement is useful, but is not a base requirement. If
provided, this is a "value-added" extra. Requirements categorised as
"Nice" which are not in the initial implementation are expected to
be provided in a future version.
.LI "Future"
.P
The requirement is one which the Project agrees with, and may include
in future implementations.
.LI "Never"
.P
The requirement is not useful to a broad enough audience to be
included in a Toolkit implementation. However, the requirement does
represent functionality that some test writers may find useful in
specific situations.
.LE
.P
A notation such as "Essential+" means that the requirement is
"essential", but that it has aspects that are "nice".
.SK
.H 2 "Overall Requirements"
.H 3 Portability
.I "Essential"
.P
The Toolkit specification must be portable across a wide
variety of systems. In particular, it must be portable to
systems that are based upon UN*X. The minimum
requirements of the Toolkit must be specified well enough to implement on
a new system.
.P
The Toolkit must conform to widely accepted industry standards whenever those
are available.
.P
.H 3 "Availability of Implementation and Specifications"
.I "Essential"
.P
The implementation and specification of this work must be
freely available, and must explicitly permit the
distribution of derivative works without royalty or
restrictions. Derivative works that are inconsistent
with the Toolkit specifications must indicate that they
have been modified, and clearly state that they are not
compatible with the Toolkit specification. 
.H 3 "Intuitive Structure"
.I "Parenthood"
.P
The specifications must provide for intuitive, well
documented, and consistent interfaces. This means, for
instance, that the look and feel of test execution via
the user interface should appear to be the same,
regardless of the structure of the underlying Test Cases.
.H 3 "Documentation"
.I "Essential"
.P
User and Programmer Manuals must be
provided. In addition, there must be a Style Guide that
outlines recommended test development practices.
.H 3 "User Interfaces"
.I "Essential"
.P
A user interface to the Toolkit must be provided. In
environments that support "command line" style interfaces, such
an interface to the Toolkit must be provided.
.H 3 "Development Environment"
.I "Parenthood"
.P
In addition to the testing environment, sufficient
structure must be provided such that Test Cases can be
developed under the Toolkit. This includes the requirement
that the Toolkit does not preclude the use of a source code
control mechanism (e.g. RCS and/or SCCS).
.SK
.H 2 "Generic Tool Requirements"
.H 3 "Installation of Test Suites"
.I "Essential"
.P
The installation tool must allow the Test Suite to have
maximum control of its environment and configuration
during installation. This means that the Installation
Tool must impose the minimum necessary structure on Test
Suites. 
.H 3 "Configuration of Test Suites"
.I "Essential"
.P
User configuration of the Test Case Controller and Test
Suites should allow for the specification of system
specific details (e.g. additional libraries required for
compilation, different compiler names, alternate header
hierarchies)
.P
Run time configuration should be changeable. Further,
configuration should be able to be set per user, per
test-suite, or globally.
.P
Configuration information should be accessible to
the user.
.H 3 "Internationalisation"
.I "Essential"
.P
A Toolkit implementation must allow international
environments. This includes the selection of user locale
based on industry standard mechanisms.
.H 3 "Binary Testing"
.I "Essential"
.P
The Toolkit must be capable of executing Test Cases
that are distributable in executable form in appropriate
environments.
.H 3 "Documentation Extraction"
.I "Nice +"
.P
The Tools must be able to extract the purpose and
strategy for a given Invocable Component. This information might be
used for user education, or by report generators.
.H 3 "Result Codes"
.I "Essential"
.P
The Toolkit must be able to extend its base set of result
codes with a Test Suite specific set.
.SK
.H 3 "Report Generation"
.I "Essential"
.P
The Toolkit must provide report generation facilities.
.SK
.H 2 "Test Case Controller Requirements"
.H 3 "Selective Execution"
.I "Essential"
.P
The Test Case Controller must allow a user to select
which Invocable Components are to be run.
.P
The names of Invocable Components must be made available
to the Toolkit by Test Suites. It is likely that Test Suites
will come pre-configured with popular sequences of
Invocable Components.
.H 3 "Test Interruption"
.I "Essential"
.P
It must be possible to interrupt execution of a Test Case,
and optionally to halt execution of the Test Scenario. An
"added value" extra would be to support interrupts
based on time interval or iteration count.
.H 3 "Restart of Interrupted Tests"
.I "Nice"
.P
An interrupted Test Scenario must be able to be
re-started at the same or next Invocable Component in the
Test Scenario. Such control must be available at the
time of interruption, or at some time in the future. Test
Scenarios must be interruptible by the operator.
.H 3 "Network Testing"
.I "Essential"
.P
The Test Case Controller must not preclude the execution
of Invocable Components on connected, capable systems.
.P
.I "Future"
.P
The Test Case Controller should provide facilities that
can take advantage of parallelism and multiple,
simultaneous executions on connected, capable systems.
Such facilities must be selectable by the user and/or Test Suite.
.H 3 "Test Case Build/Run"
.I "Essential"
.P
The Test Case Controller must allow the user to request
that Test Cases be built before execution. A user option
must be provided so that that Test Cases are built, run, and cleaned up
before another Test Case is started.
.P
The Test Case Controller must be configurable in such a
way that Test Case source files are separated from the
actual tests being run. It must be possible to keep a
clean copy of the Test Cases at all times.
.H 3 "Selection Criteria"
.I "Essential"
.P
The criteria for selecting which tests are to be executed
by the Test Case Controller must be readily definable by Toolkit users or
by the system.
.H 3 "Isolation"
.I "Essential"
.P
The Test Case Controller must not prevent individual Test
Cases from being executed in parallel. Each Test Case
execution should be protected from like executions.
.P
The Test Case Controller must provide cleanup of Test Case space, so that
Test Cases do not corrupt each other, or induce inter-test
dependencies.
.H 3 "Black-Box Testing"
.I "Parenthood"
.P
The Test Cases must communicate with the Test Case Controller only through
the API as defined in the Toolkit specifications. The Test Case
Controller 
cannot have any knowledge of the internals of an
individual Test Case.
.H 3 "Audit Trails"
.I "Essential"
.P
In addition to providing Test Cases with Journal facilities, the
Test Case Controller must mark in the Journal the start
and finish of each Test Case execution.
.H 3 "Sequencing"
.I "Essential"
.P
The Test Case Controller must ensure that Invocable
Components are processed in the sequence defined in the
Test Scenario.
.H 3 "Interactive Testing"
.I "Essential"
.P
The Test Case Controller must not use the system in such
a way that executing Test Cases are prevented from
interacting with the operator.
.SK
.H 2 "API Requirements"
.H 3 "Stand-Alone Tests"
.I "Essential"
.P
The API must allow a test to be built in such a way that
it can be executed without the Test Case Controller. Compilation of
stand-alone tests should be possible with a minimum of
difficulty.
.H 3 "Test Structure Support"
.I "Parenthood"
.P
The API must allow test developers to write tests in a
well structured manner.
.H 3 "Dynamic Configuration"
.I "Essential"
.P
The API must allow dynamic configuration of run time
parameters. In this way, stand-alone executables can test 
in different ways without re-compilation.
.H 3 "Language Independence"
.I "Essential"
.P
The specification of the API must be such that it can be
extended to other languages.
.H 3 "Exception Handling"
.I "Essential"
.P
The API must provide exception handling mechanisms to
deal with unusual system events that occur while Test
Cases are executing.
.H 3 "Test Synchronisation"
.I "Future"
.P
The API should provide facilities that will assist in
synchronising multiple processes associated with a single
Test Case.
.H 3 "Test Progress Tracking"
.I "Nice"
.P
The API must provide a mechanism that allows users to view
the progress of Invocable Component execution.
.H 3 "Result Code Journal Information"
.I "Essential"
.P
The API must provide functionality that will record messages and
result codes in a journal. These messages must be delivered to the
journal synchronously.
.H 3 "Result Code Enforcement"
.I "Essential"
.P
The API must provide a mechanism that allows Test Case authors to
insert support for Result Code checking and enforcement.
.H 3 "Resource Requirements"
.I "Essential"
.P
The API must document statically-used resources. The API must also
provide a run time method to determine the dynamically allocated
resources.
.SK
.H 2 "Journal Requirements"
.H 3 "Comprehensive Reporting"
.I "Essential"
.P
The journal-recording mechanism must provide information which
includes, at the very least, Test Case start and end, Test
Case journal entries (with test results), and the
Invocable Component names used within a Test Case.
.H 3 "Result Codes"
.I "Essential"
.P
The specification must allow for Test Suite-defined
result codes to be added on to those specified in the
base set (IEEE Std 1003.3-1991 result codes). In particular, the
reporting tools, API, and Test Case Controller must process
result codes outside the scope of the base set.
.P
Ensure that result codes provided in the base specification are broad
enough to satisfy common test requirements.
.H 3 "Journal File Format"
.I "Essential"
.P
The format must be well defined, so that third parties
can develop tools that manipulate the data.
.SK
.H 2 "Style Guide Requirements"
.H 3 "Test Case Structure"
Test Cases must be built
in such a way that the granularity of the execution
selections will be appropriate for the specification being
tested.
.H 3 "Dynamic Configuration"
Test developers should be encouraged to use parameters that
allow for dynamic re-configuration of their tests in a
stand-alone execution environment.
.H 3 "System Resource Requirements"
Test Developers should be warned against having their tests
require too much system resource for a given test. Developers must 
ensure that the Test Case requirements
do not exceed the resources available in likely test
environments.
.H 3 "Evolution"
The Style Guide must clearly document how the
specification of the Test Case Controller and Tools will evolve. In
particular, it must show how extensions may be migrated
into the base technology.
.H 3 "Portability of Tests"
Test Developers should be encouraged to develop tests
that are maximally portable.
.H 3 "Result Codes"
Test Developers must use the predefined result codes whenever possible.
.H 3 "Required System Resources"
Test Suites must document their system resource requirements.
.H 3 "Required Interfaces as Macros"
Interfaces that are defined as macros in an
implementation must be detected, and the installation
process should adjust the Test Scenarios, Invocable
Component name list, and Test Cases accordingly.
.H 3 "File System Use"
Test Cases must only operate on test files in the working
directory provided by the Test Case Controller.
.P
Global Test Suite files should not be written to, only
read from. 
