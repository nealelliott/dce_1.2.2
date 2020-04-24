'\"	SCCS: @(#) TET SPECIFICATION		Rel 4.5 (06/23/92)
'\"
.nr H1 2
.nr Ej 1
.nr Cl 2
.HM A 1 1 1 1 1 1
.HU "Appendix C - Project Description"

.H 2 "Summary"
This Project Description details the joint 
working relationship between the Open Software Foundation (OSF), 
Unix International (UI) and X/Open in the area of verification 
technologies, the process and results of the 
co-operative effort. 
.P
Organisations such as OSF, UI and X/Open produce or guide industry 
specifications for software products. When computer vendors 
implement these specifications, they use tests to verify conformance of 
the software to the specifications. These tests have in the past included 
the System V Verification Suite (SVVS) from AT&T, BSD Validation 
from Perennial, PCTS from Mindcraft, NIST-PCTS from NIST and VSX 
from X/Open. 
.P
These suites all have different user interfaces and different calling 
conventions, so testing organisations need to learn multiple 
technologies for verification work. In addition, to extend any of these 
suites test writers need to learn incompatible calling conventions.
.P
.H 2 "Project Management"
.P
The Project is managed by the following representatives of each 
company:
.BL
.LI
John Morris from OSF
.LI
Shane McCarron from UI
.LI
James de Raeve from X/Open
.LE
.H 2 "Background - Organisational Needs"
OSF, UI and X/Open (hereafter "the Project") plan to co-operate in 
verification technologies to avoid further duplication of effort in the 
industry. To that end, the three organisations have agreed to define 
and produce a common verification interface and driver technology 
which will facilitate test development and execution. The following 
paragraphs describe the requirements each organisation is satisfying by 
participation in this project.
.P
.H 3 "UNIX International"
UI members have identified a requirement that conformance tests be 
developed and supplied independently of the operating system.
.P
UI recognises that the requirements met by products will always be 
greater than the scope of common specifications. Therefore, the scope 
for common test technology will always be less than the requirement 
for product testing.
.P
UI, by charter, does not develop software, and will therefore not itself 
develop test technology.
.P
.H 3 "Open Software Foundation"
OSF needs to distribute test suites for the verification of OSF 
technologies.
.P
OSF has developed validation suites for functional areas of the AES. To 
date, these include the User Environment and Operating System 
areas. The User Environment test cases are independent of any test 
framework. The Operating System test cases have been developed to 
operate with the current version of VSX - the verification suite from 
X/Open. Both sets of test cases will be migrated to the specified 
interfaces.
.P
.H 3 "X/Open Company Ltd."
X/Open has 3 clear goals in the field of test technology:
.AL 1
.LI
To maximise the quality and coverage of the X/Open test systems in 
order to best support the X/Open branding program;
.LI
To co-operate with UI, OSF and others in the development of 
conformance testing technology thus making the best use of resources;
.LI
To adopt a distribution/commercialisation policy for the X/Open 
Verification Suite that seeks to promote the market for open systems at 
the expense of revenue, yet ensure that the true costs of maintenance 
are recovered.
.LE
.H 2 "Project Description"
The Project began at a joint meeting between OSF, UI and X/Open in 
London on September 29, 1989. At that meeting, the three 
organisations set base expectations for their future verification needs. 
The group then met with NIST on October 24th to discuss 
co-operation between the three organisations and NIST. NIST is 
supportive of the efforts of the Project and will participate in future 
talks on verification co-operation. On January 24, 1990, the three 
organisations met in Washington to detail the work program and 
structure the project management.
.P
After the basic needs of the project were established, all three 
organisations agreed that it would be valuable to invite interested 
members of the test software development community to discuss their 
requirements for a test driver. This meeting occurred on January 24, 
1990 in Washington and represented government, development and 
conformance testing interests. The project management group believes 
that through careful study of these requirements, a reasonably 
comprehensive and effective test driver can be specified. The 
management group further believes that if this specification process is 
successful, the technology will evolve into a suitable framework for the 
test development community.
.P
.H 2 "Technology Descriptions"
The deliverables for the project include:
.BL
.LI
Requirements
.LI
Architectural Specification
.LI
Functional Specifications
.LI
Installation Procedure
.LI
Test Case Controller
.LI
Report Generator
.LI
API Library
.LI
Documentation
.BL
.LI
User's Manual
.LI
Programmer's Manual
.LI
Style Guide
.LI
Manual Pages
.LE
.LE
.I
[Requirement 1.4]
.R
.P
Users execute tests via the Test Case Controller (TCC). This 
single interface is capable of building and executing all tests provided by 
suites installed in the defined environment. While building and 
executing test cases, the TCC records messages and results in a journal. 
This journal may then be processed by a report generator to construct 
customised reports.
.P
In order to take maximum advantage of the facilities provided by 
the toolkit environment, test suites are written using the defined API.
The API Library 
contains routines that may be linked into executable test cases to 
interact with the TCC. These routines include interfaces to the journal 
file, entry and exit procedures for TCC startup and shutdown, and 
general-purpose routines for consistent test execution.
.P
The documentation includes a User's Manual for operation of the 
system, a Programmer's Manual to assist programmers in writing test 
cases and manual pages for the TCC as well as the library routines.
.P
The Project will provide driver extensibility and flexibility so that test 
developers can work within the structures of the driver and still 
produce useful, effective tests. The Project will not address the 
structure, content, or scope of Test Cases. A Test Case can be arbitrarily 
complex or simple and still function under the Driver. The structure 
of a Test Case can be based on assertions, functions, modules, or any 
other appropriate abstraction. It is up to the test developer to choose 
the most useful structure for a given test. This will often, but not 
always, be based on requirements of various conformance 
specifications. Because of the overriding concern for flexibility on the 
part of the test developer, the Driver will impose a minimum amount 
of structure on Test Cases.
.P 
.H 2 "Responsibilities"
OSF has agreed to develop the driver and tools based on the 
requirements document. This agreement is based on the assumption 
that existing technology will be adapted to the needs of the group rather 
than invented from scratch. 
.P
UI has agreed to produce the supporting documentation, including the 
User's Manual and Programmer's Manual.
.P
X/Open has agreed to move VSX to the new scaffold, and to 
develop the API library.
.P
.H 2 "Technology Ownership"
The technology, which is derived from many sources including OSF, 
Unix International and X/Open, will be copyrighted and available on a 
royalty free basis to the industry. OSF, UI and X/Open will make the 
technology available to third parties. There will be no sub-licence for 
the technology, and derivative works are encouraged. Derivative works 
must retain the copyright notice contained in the distributed source,
and the use of the product name selected by the project will only be 
granted to versions that conform to the specification agreed to by OSF, 
UI and X/Open.
.I
[Requirement 1.2]
.R
.P
.H 2 "Maintenance"
Maintenance of supplied technologies will be provided by the 
respective development organisations. All changes to the scaffold will 
be made in agreement with UI and X/Open. There will be no formal 
defect reporting system; all defect reports will be received through each 
company's normal reporting mechanisms. Versions of the technology 
will be delivered to UI, OSF and X/Open members for review before 
distribution as a released technology. 
