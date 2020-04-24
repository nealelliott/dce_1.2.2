$ @OSF_COPYRIGHT@
$ COPYRIGHT NOTICE
$ Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
$ ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
$ the full copyright text.
$
$ (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
$ Burlington, MA, USA
$ All Rights Reserved
$ (c) Copyright 1990, 1991, 1992
$
$ 
$ HISTORY
$ $Log: dcethd.msg,v $
$ Revision 1.1.7.2  1996/03/09  23:31:04  marty
$ 	Update OSF copyright year
$ 	[1996/03/09  22:46:17  marty]
$
$ Revision 1.1.7.1  1995/12/07  22:08:23  root
$ 	Submit OSF/DCE 1.2.1
$ 	[1995/12/07  21:08:02  root]
$ 
$ Revision 1.1.5.1  1994/06/09  13:39:15  devsrc
$ 	 CR10892 - fix copyright in file
$ 	[1994/06/09  13:29:01  devsrc]
$ 
$ Revision 1.1.2.5  1992/12/15  22:33:14  alan
$ 	Insert copyright notices
$ 	[1992/12/07  19:44:40  alan]
$ 
$ Revision 1.1.2.4  1992/12/07  13:21:19  zeliff
$ 	Removed duplicate OSF copyright markers
$ 	[1992/12/06  00:45:21  zeliff]
$ 
$ Revision 1.1.2.3  1992/09/29  20:16:35  devsrc
$ 	Fixed merge hiccup
$ 	[1992/09/21  18:11:22  weisman]
$ 
$ 	[OT 5373]    SNI/SVR4 merge.
$ 	[1992/09/17  21:00:58  sekhar]
$ 
$ 	Replace current source file with equivalent from DEC BL10+
$ 	source base, as originally integrated in the
$ 	nosupport/threads_bl10 tree.  (See OT defect 5300.)
$ 	[1992/09/02  01:49:01  bolinger]
$ 
$ Revision 1.1.2.2  1992/09/04  20:37:51  bolinger
$ 	Move revision 1.1.4.2 of threads/dcethreads.msg to this file
$ 	(threads/dcethd.msg), so that the corresponding .cat file
$ 	can be found via dce_error_inq_text()'s transformation of
$ 	component code.
$ 	[1992/09/04  20:36:35  bolinger]
$ 
$ Revision 1.1.2.2  1992/05/09  00:17:56  bolinger
$ 	BL10 CMA sources from DEC.
$ 	[1992/05/09  00:17:43  bolinger]
$ 
$ Revision 1.1.2.2  1992/06/08  14:39:38  bolinger
$ 	Fix OT defect 3993:  change "$ set" (which is a comment)
$ 	to "$set" (which, curiously enough, is a command).  [Sic]
$ 	[1992/06/08  14:37:37  bolinger]
$ 
$ $EndLog$
$ ++
$ !
$ !  FACILITY:
$ !
$ !      DECthreads Message Catalog
$ !
$ !  FILENAME:
$ !
$ !      dcethreads.msg
$ !
$ !  ABSTRACT:
$ !
$ !	 DCE threads messages.
$ !
$ !  AUTHORS:
$ !
$ !	 Dave Butenhof
$ !
$ !  CREATION DATE:
$ !
$ !	 06 June 1991
$ !
$ !  MODIFICATION HISTORY:
$ !
$ !	001	Dave Butenhof	20 November 1991
$ !		Remove arguments from exceptions 01 and 02: the arguments
$ !		weren't used, and these are really placeholders anyway, since
$ !		those statuses aren't even used except on VMS.
$ !	002	Dave Butenhof	08 January 1992
$ !		Remove "Exception: " prefix from messages; instead,
$ !		exc_report will add it. That way it'll show on all exceptions
$ !		reported by DECthreads, not just DECthreads facility codes.
$ --
$set 1 
01 Address exception raised
02 Status exception raised
03 Uninitialized exception raised
05 Invalid memory address
06 Operation failed due to insufficient quota
07 Insufficient virtual memory for requested operation
08 Insufficent privilege for requested operation
09 Normal successful completion
10 Illegal instruction
11 Reserved addressing error
12 Privileged instruction error
13 Reserved operand error
14 Trace or breakpoint trap
15 IOT trap
16 EMT trap
17 Arithmetic error
18 Bad system call
19 Broken pipe
20 CPU-time limit exceeded
21 File-size limit exceeded
22 Integer overflow
23 Integer divide by zero
24 Floating point overflow
25 Floating point/decimal divide by zero
26 Floating point underflow
27 Decimal overflow
28 Subscript out of range
29 Illegal opcode
30 Illegal operand
31 Illegal addressing mode
32 Illegal trap
33 privileged opcode
34 Privileged register
35 Co-processor
36 Bad Stack
37 Process breakpoint
38 Process trace
39 Invalid floating point operation
40 Address not mapped to object
41 Invalid permissions
42 Invalid address alignment
43 Non-existent physical address
44 Object specific
48 Thread has been canceled
49 A thread assertion check has failed
50 Invalid parameter to operation
51 Internal error detected in thread library
52 Current thread has been requested to exit
53 Object referenced does not currently exist
54 Object referenced is already in use
55 Operation inappropriate for specified object
56 Wrong mutex specified in condition wait
57 Attempted stack overflow was detected
58 No space is currently available to create a new stack
59 The current stack was not allocated by thread library
60 Timed condition wait expired
61 Unimplemented feature
62 Thread library initialization is already in progress
63 No space currently available to process interrupt request
64 Unable to process condition signal from interrupt level
65 Improper nesting of alert scope
128 Unknown status code 0x%1$08X
