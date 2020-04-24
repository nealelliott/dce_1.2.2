'\"	SCCS: @(#) TET SPECIFICATION		Rel 4.5 (06/23/92)
'\"
.nr H1 1
.H 1 Scope
This specification describes a test controller, a test suite installation
tool, a test report generator, and programmatic interfaces to be used in
the development of test cases.  The Project has tried to
allow for future extensibility and flexibility of the Toolkit so
that test developers can work within the structures of the
driver and still produce useful, effective tests.
.P
What the project has not addressed is the formulation, content,
or scope of test cases. A test case can be arbitrarily complex
or simple and still function under the Toolkit. The structure
of a test case can be based on assertions, functions, modules,
or any other appropriate abstraction. The test suite
developer must choose the most useful structure for a given
test suite. This choice will often be based on requirements of various
conformance specifications. Because of the
overriding concern for providing flexibility for the test
developer, the Toolkit imposes a minimum amount of
structure on test cases.
.I "[Requirement 3.8]"
