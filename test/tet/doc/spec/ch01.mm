'\"	SCCS: @(#) TET SPECIFICATION		Rel 4.5 (06/23/92)
'\"
.nr H1 0
.H 1 "Introduction"
The Test Environment Toolkit (TET) Project started in September of 
1989. At that
time, X/Open, the Open Software Foundation, and UNIX
International entered into an announced agreement to produce a
specification for a test environment.  These organisations
also agreed to develop and make freely
available an implementation written to that specification. 
Additionally,
the three organisations committed to 
producing tests suites for execution within that environment.
.P
In the process of developing a specification, the project 
invited interested members
of the test software development community to discuss their
requirements for a test driver.  These requirements, which can be 
seen
in Appendix B, are cited throughout this specification using the
notation
.I "[Requirement n.n]" .
It was the belief of the Project 
that, through careful study of these
requirements, a reasonably comprehensive and effective test
driver could be specified. Having achieved this, the Project 
now expects that a substantial
portion of the test development community will begin using this 
environment 
for the development of conformance testing software.
.P
The primary goal of this project, as is reflected in the requirements,
was to define a specification which would guarantee portability of 
the
TET architecture across a wide variety of systems.  While it is
essential that TET operate in UNIX\(rg
.FS \(rg
UNIX is a registered trademark of UNIX System Laboratories in the 
United States and other countries.
.FE
like environments, every caution
has been taken to ensure that TET is implementable on any platform
which can satisfy the system requirements (\(sc3.4).  
.P
A ramification of working toward this goal is that much of this 
specification 
is not tied very tightly to any implementation of TET.  While this is
intentional, a short guide to the document may make it easier to
understand.
.P
Section 3 provides an
architectural overview of TET.  This includes the system
requirements and test suite requirements.  It also defines 
terminology
that is used throughout the document.  Section 4 is a system-independent
functional specification.  This describes the
functionality provided by TET in such a way that it can be bound to in
a number of different languages on any system satisfying the
requirements in section 3.
Sections 5 and 6 are where all of the TET language bindings appear.
These are bindings for specific TET functions in specific
environments.  The bindings provided in this specification pertain to POSIX.1
systems with either ANSI\ C or common\ C support, and XPG3 shell.  Section 7 
defines the formats
for TET portable input and output data files.  
