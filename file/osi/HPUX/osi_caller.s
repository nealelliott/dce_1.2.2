;
; @OSF_COPYRIGHT@
; COPYRIGHT NOTICE
; Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
; ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
; src directory for the full copyright text.
;
;
; HISTORY
; $Log: osi_caller.s,v $
; Revision 1.1.8.1  1996/10/02  17:57:21  damon
; 	Newest DFS from Transarc
; 	[1996/10/01  18:43:23  damon]
;
; Revision 1.1.3.1  1994/07/13  22:04:31  devsrc
; 	Created this function to return the program counter of
; 	the caller of the caller.
; 	[1994/04/04  15:31:33  mbs]
; 
; $EndLog$
;
	.CODE

osi_caller
        .PROC
        .CALLINFO CALLER,FRAME=0
        .ENTRY
	; It's hard (impossible?) to get the total stack space 
	; of our caller.  However this caller is always
	; osi_Alloc or osi_Malloc, and both use a 64 byte stack
	; frame.  If this ever changes, this function will
	; break.  The address we're looking for is located in the
	; frame marker area of the caller of osi_Malloc or
	; osi_Alloc.  This is located a total of 84 bytes into
	; the stack.
	LDW -84(0,%r30), %r28
        BV      %r0(%r2)
	NOP
        .EXIT
        .PROCEND

	.EXPORT osi_caller,ENTRY

	.END
