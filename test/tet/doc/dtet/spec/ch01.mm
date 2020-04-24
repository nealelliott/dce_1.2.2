'''	SCCS: @(#) DTET SPECIFICATION		Rel 1.2 (9/15/92)
.nr H1 0
.H 1 Introduction
.P
The Distributed Test Environment Toolkit (DTET) Project started in October 1991
as an extension to the TET Project. The DTET Project drew requirements from a
number of sources who wished to use the TET in a distributed testing environment
and found that the current definition did not meet their requirements.
.P
The primary goal of the project, as reflected in the requirements, was to
define a specification, backwards compatible with the TET definition, which
guaranteed the portability and the interoperability of the TET across a wide
variety of systems. This would allow for the development of well structured
tests operating across a network of (potentially different) computing platforms
each of which satisfied the system requirements. The definition was aimed at
being transport independent with the potential for implementation providers
to address specific transports without altering the application programmer
interface.
.P
