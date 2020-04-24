;
; @OSF_COPYRIGHT@
; COPYRIGHT NOTICE
; Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
; ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
; the full copyright text.
;
; (C) Copyright 1992 Transarc Corporation - All Rights Reserved
;
; HISTORY
; $Log: process.s,v $
; Revision 1.1.9.1  1996/10/02  17:28:52  damon
; 	Newest DFS from Transarc
; 	[1996/10/01  18:34:41  damon]
;
; Revision 1.1.4.1  1994/06/09  14:03:47  annie
; 	fixed copyright in src/file
; 	[1994/06/08  21:38:41  annie]
; 
; Revision 1.1.2.4  1993/01/21  18:24:10  zeliff
; 	Embedding copyright notices
; 	[1993/01/19  14:14:22  zeliff]
; 
; Revision 1.1.2.3  1992/09/15  20:31:06  jaffe
; 	sync with Transarc, correct comment character.
; 	[1992/09/15  20:21:19  jaffe]
; 
; Revision 1.1.2.2  1992/08/31  20:21:25  jaffe
; 	Transarc delta: mason-add-hp800-osi-routines 1.15
; 	  Selected comments:
; 	    This delta is misnamed - these are really porting fixes.
; 	    assembly piece of lwp.
; 	    Need to checkpoint to fix a different bug.
; 	    Checkpoint the first system where test_anode works.  Only one change is not
; 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
; 	    is -lBSD.  This should be fixed more generally.
; 	    This is the HP version of the LWP assembly code.
; 	    More HP-UX changes.
; 	    More HP-UX related changes.
; 	    Fix compile problem on the RIOS.
; 	    Fix more compilation problems.
; 	    Checkpoint.
; 	    Checpoint.
; 	    In this snap we turned all malloc() calls into osi_Alloc calls.
; 	    Also turned free calls into osi_Free.
; 	    HP-UX EOF detection fix.
; 	    Another checkpoint.
; 	    This is a checkpoint of the first working HP-UX Episode system.
; 	    Needed to add the vnops routines specific to HP-UX.
; 	    Fix compilation problems.
; 	[1992/08/30  02:53:38  jaffe]
; 
; Revision 1.1.1.2  1992/08/30  02:53:38  jaffe
; 	Transarc delta: mason-add-hp800-osi-routines 1.15
; 	  Selected comments:
; 	    This delta is misnamed - these are really porting fixes.
; 	    assembly piece of lwp.
; 	    Need to checkpoint to fix a different bug.
; 	    Checkpoint the first system where test_anode works.  Only one change is not
; 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
; 	    is -lBSD.  This should be fixed more generally.
; 	    This is the HP version of the LWP assembly code.
; 	    More HP-UX changes.
; 	    More HP-UX related changes.
; 	    Fix compile problem on the RIOS.
; 	    Fix more compilation problems.
; 	    Checkpoint.
; 	    Checpoint.
; 	    In this snap we turned all malloc() calls into osi_Alloc calls.
; 	    Also turned free calls into osi_Free.
; 	    HP-UX EOF detection fix.
; 	    Another checkpoint.
; 	    This is a checkpoint of the first working HP-UX Episode system.
; 	    Needed to add the vnops routines specific to HP-UX.
; 	    Fix compilation problems.
; 
; 	$TALog: process.s,v $
; 	Revision 1.6  1994/11/01  21:32:02  cfe
; 	Bring over the changes that the OSF made in going from their DCE 1.0.3
; 	release to their DCE 1.1 release.
; 	[from r1.5 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
; 
; 	Revision 1.3  1992/06/06  23:56:28  mason
; 	Checkpoint the first system where test_anode works.  Only one change is not
; 	included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
; 	is -lBSD.  This should be fixed more generally.
; 
; 	This is the HP version of the LWP assembly code.
; 	[from revision 1.1 by delta mason-add-hp800-osi-routines, revision 1.3]
; 
; Revision 1.2  1992/06/06  23:47:17  mason
; 	Continue changes!  This delta checkpoints a working system except the
; 	Makefile in anode which uses -lbsd for all platforms.  It should use -lBSD.
; 	I imported lwp back from AFS-3.  I've done some cleanup, but the cleanup
; 	needs to be finished and these changes must be verified on AIX.
; 
; 	Minor changes.
; 	[from revision 1.1 by delta mason-add-hp800-osi-routines, revision 1.3]
; 
; $EndLog$
;
#include <m4_frame.h>

	.CODE

	;
	; savecontext(f, area1, newsp)
	;    int (*f)();
	;    struct savearea *area1;
	;    char *newsp;
	;
savecontext
	.PROC
	;
	; Callinfo sets up register saves using the ENTRY_GR
	; and ENTRY_FR parameters.  ENTRY_FR=21 is only valid
	; for PA 1.1.  (How to deal with this for 800?)
	;
	.CALLINFO CALLER,FRAME=0,SAVE_RP,ENTRY_GR=18,ENTRY_FR=21
	; The ENTER statement generates register saves and
	; procedure setup.
	.ENTER

	LDI   1,%r31				; Store a (char) 1 in
	ADDIL LR'PRE_Block-$global$,%r27	; global variable
	STB   %r31,RR'PRE_Block-$global$(0,%r1)	; PRE_Block.

	COPY  %r26,%r22		; Copy arg0 (f) to dyncall's input register.

	COMIB,= 0,%r24,L$0001	; Compare arg2 (newsp) to 0.  Execute the
				; next instruction regardless of value.
	STWS  %r30,0(0,%r25)	; Store the stack pointer in the first
				; element (0th offset) of arg1 (area1).
	COPY  %r24,%r30		; Move arg2 (newsp) into the stack ptr.

L$0001
	.CALL
	BL    $$dyncall,%r31	; Dynamic call using pointer in r22.
	COPY  %r31,%r2

	.CALL
	BL    abort,%r2		; Can't get here, so abort.
	NOP
	.LEAVE
	.PROCEND

	; returnto(area2)
	;    struct savearea *area2;
	;
returnto
	.PROC
	.CALLINFO CALLER,FRAME=0,SAVE_RP,ENTRY_GR=18,ENTRY_FR=21
	;  No ENTRY is used since this is a magic routine.
	ADDIL LR'PRE_Block-$global$,%r27	; PRE_Block = 0
	STB   %r0,RR'PRE_Block-$global$(0,%r1)

	LDWS  0(0,%r26),%r30	; Load the stack pointer from area2
	.LEAVE
	.PROCEND

	.EXPORT savecontext,ENTRY
	.EXPORT returnto,ENTRY
	.IMPORT $global$,DATA
	.IMPORT PRE_Block,DATA
	.IMPORT $$dyncall,MILLICODE
	.IMPORT abort,CODE

	.END
