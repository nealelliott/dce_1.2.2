; 
; 	@HP_COPYRIGHT@
; 
; @OSF_COPYRIGHT@
;
; COPYRIGHT NOTICE
; Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
; ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
; the full copyright text.
;
; HISTORY
; $Log: cma_assem.s,v $
; Revision 1.1.11.1  1996/05/10  13:26:25  arvind
; 	Drop 1 of DCE 1.2.2 to OSF
;
; 	HP revision /main/DCE_1.2/1  1996/03/08  21:49 UTC  bissen
; 	merge in long PIC changes from pare_vista_mothra branch
; 	[1995/01/11  18:45 UTC  mk  /main/HPDCE02/3]
;
; 	revert references to cma__g_serialize, cma__g_def_ctx_buf, and
; 	cma__g_abort_signal to regular PIC
; 	[1994/11/30  20:14 UTC  mk  /main/HPDCE02/pare_vista_mothra/4]
;
; 	change references to cma_singlestack_stackaddr back to long PIC
; 	(this version is identical to .../pare_vista_mothra/1)
; 	[1994/11/30  16:32 UTC  mk  /main/HPDCE02/pare_vista_mothra/3]
;
; 	revert references to cma_singlestack_stackaddr to regular PIC
; 	[1994/11/29  15:33 UTC  mk  /main/HPDCE02/pare_vista_mothra/2]
;
; 	convert to long PIC
; 	[1994/11/18  18:24 UTC  mk  /main/HPDCE02/pare_vista_mothra/1]
;
; 	Merge singlestack code.
; 	[1994/10/27  21:06 UTC  chris_p  /main/HPDCE02/2]
;
; 	Changes for growing and shrinking stacks.
; 	[1994/10/20  20:43 UTC  chris_p  /main/HPDCE02/proven_threads_940803/4]
;
; 	Change symbol names with "carevue" to "singlestack"
; 	[1994/10/17  20:30 UTC  chris_p  /main/HPDCE02/proven_threads_940803/3]
;
; 	Actually do stack growth
; 	[1994/10/06  21:54 UTC  chris_p  /main/HPDCE02/proven_threads_940803/2]
;
; 	Project carevue.
; 	Threads now use only one stack.
; 	Stacks are copied at context switch time.
; 	[1994/09/06  22:51 UTC  chris_p  /main/HPDCE02/proven_threads_940803/1]
;
; 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
; 	[1994/07/06  16:47 UTC  jss  /main/HPDCE02/1]
;
; 	merge kk and hpdce01
; 	[1994/01/24  18:28  truitt  /main/HPDCE01/1]
;
; 	add commas where there are more than one
; 	argument on a .CALLINFO line.  The new
; 	(10.0) assembler barfs if there are only
; 	spaces.
; 	[1993/10/06  15:15:15  rmm]
; 	;
;
; 	Merge kk and dce1_0_3.
; 	[1993/09/20  14:00:22  truitt]
;
; 	Loading drop DCE1_0_3b03
; 	[1993/09/15  17:41:31  root  1.1.11.3]
;
; 	Add unwind descriptor in cma__thread_start0,
; 	and save 0 previous stack pointer in stack,
; 	so that unwind will stop at thread_start0.
; 	[1993/09/07  21:40:49  hopkins]
;
; 	Flush commented-out instructions from change of 10/3/92
; 	Mark exported entry points as ,ENTRY so that they are visible outside
; 	of the shared library.
; 	[1993/06/11  18:19:02  sommerfeld]
;
; 	Initial King Kong branch
; 	[1993/07/08  17:27:06  root]
;
; 	Gamera merge
; 	[1993/03/26  23:53:24  hopkins]
;
; 	Loading drop DCE1_0_2B18
; 	[1993/03/11  22:24:13  osfrcs  1.1.1.8]
;
; 	DCE 1.0.1 Release
; 	[1992/09/23  20:36:34  hopkins]
;
; 	CR10892 - fix copyright in file
; 	[1994/06/09  13:25:21  devsrc]
; 	;
;
; 	Flush commented-out instructions from change of 10/3/92
; 	Mark exported entry points as ,ENTRY so that they are visible outside
; 	of the shared library.
; 	[1993/06/11  18:19:02  sommerfeld]
;
; 	Insert copyright notices
; 	[1992/12/07  15:59:25  alan]
;
; 	Work around HP-UX 8.05 assembler bug.
; 	Fix cma__do_call_on_stack to behave properly (use $$dyncall millicode
; 	instead of assuming arg1 points to the first instruction of the routine
; 	to call)
; 	[1992/09/30  21:36:31  sommerfeld]
;
; 	Initial revision
; 	[1992/01/19  18:34:34  devrcs  1.1]
;
; 	Loading drop Dce1_0_1fnl
; 	[1992/09/21  18:48:47  osfrcs  1.1.1.7]
;
; 	b19 merge.
; 	[1992/06/18  18:55:54  rose_l]
;
; Revision 1.1.7.1  1994/06/09  13:34:21  devsrc
; 	 CR10892 - fix copyright in file
; 	[1994/06/09  13:25:21  devsrc]
; 
; Revision 1.1.5.2  1993/06/11  18:23:47  sommerfeld
; 	Flush commented-out instructions from change of 10/3/92
; 	Mark exported entry points as ,ENTRY so that they are visible outside
; 	of the shared library.
; 	[1993/06/11  18:19:02  sommerfeld]
; 
; Revision 1.1.2.3  1992/12/15  22:21:00  alan
; 	Insert copyright notices
; 	[1992/12/07  15:59:25  alan]
; 
; Revision 1.1.2.2  1992/10/03  19:45:51  sommerfeld
; 	Work around HP-UX 8.05 assembler bug.
; 	Fix cma__do_call_on_stack to behave properly (use $$dyncall millicode
; 	instead of assuming arg1 points to the first instruction of the routine
; 	to call)
; 	[1992/09/30  21:36:31  sommerfeld]
; 
; Revision 1.1  1992/01/19  18:34:34  devrcs
; 	Initial revision
; 
; $EndLog$
;
;
;  version 1
;   MODULE  cma_assem_hp9000s800

#include <m4_frame.h>

#define frame_size	48

temp	.REG	%r1
temp1	.REG	%r1
temp2	.REG	%r28


;
; Callee Saves Register Context Save Definitions.
;
; # words 40 (160 bytes).
#define callee_save_sz	160

#define r3save		-callee_save_sz
#define r4save		r3save + 4
#define r5save		r4save + 4
#define r6save		r5save + 4
#define r7save		r6save + 4
#define r8save		r7save + 4
#define r9save		r8save + 4
#define r10save		r9save + 4
#define r11save		r10save + 4
#define r12save		r11save + 4
#define r13save		r12save + 4
#define r14save		r13save + 4
#define r15save		r14save + 4
#define r16save		r15save + 4
#define r17save		r16save + 4
#define r18save		r17save + 4

; PA-RISC 1.1 visible FP regs
#define fr21save	r18save + 4
#define fr20save	fr21save + 8
#define fr19save	fr20save + 8
#define fr18save	fr19save + 8
#define fr17save	fr18save + 8
#define fr16save	fr17save + 8

#define fr15save	fr16save + 8
#define fr14save	fr15save + 8
#define fr13save	fr14save + 8
#define fr12save	fr13save + 8
#define fr0save		fr12save + 8

#define sr3save		fr0save + 8

;
; Caller Saves Register Context Save Definitions.
;
; Total of 41 words (164 bytes)
#define caller_save_sz	164

#define r1save		caller_save_sz
#define r2save		r1save + 4
#define r19save		r2save + 4
#define r20save		r19save + 4
#define r21save		r20save + 4
#define r22save		r21save + 4
#define r23save		r22save + 4
#define r24save		r23save + 4
#define r25save		r24save + 4
#define r26save		r25save + 4
#define r28save		r26save + 4
#define r29save		r28save + 4
#define r31save		r29save + 4

#define cr11save	r31save + 4

#define fr11save	cr11save + 4
#define fr10save	fr11save + 4
#define fr9save		fr10save + 4
#define fr8save		fr9save + 4
#define fr7save		fr8save + 8
#define fr6save		fr7save + 8
#define fr5save		fr6save + 8
#define fr4save		fr5save + 8

#define sr2save		fr4save + 8
#define sr1save		sr2save + 4
#define sr0save		sr1save + 4

;;; Global data
	.SUBSPA	$DATA$
	.ALIGN	16
; Used to serialize access to kernel semaphores.  The hppa instruction,
; ldcws, atomically tests and clears this word before it tests the 
; kernel semaphore.  If this word is found to be busy (ie, a zero), 
; then the kernel semaphore is also logically busy.
;
; Note that this word must be aligned on a 16 byte boundary because of
; the use of the ldcws instruction.

cma__g_serialize	.WORD	1	; If == 0 , threads must serialize
	.EXPORT	cma__g_serialize,DATA

#ifdef PROF
cnt			.WORD	0
#endif

;;; Imported data
	.DATA
	.IMPORT cma__g_kernel_critical
	.IMPORT cma__g_def_ctx_buf
	.IMPORT cma__g_abort_signal
	.IMPORT _SYSTEM_ID

	.CODE

;;; Exported routines
	.EXPORT cma__fetch_sp,ENTRY,RTNVAL=GR
	.EXPORT cma__do_call_on_stack,ENTRY
	.EXPORT cma__init_thread_stack,ENTRY
	.EXPORT cma__restore_regs,ENTRY
	.EXPORT cma__serialize,ENTRY
	.EXPORT cma__transfer_regs,ENTRY
	.EXPORT cma__thread_start0,ENTRY
	.EXPORT cma__transfer_main_abort,ENTRY

;;; Imported routines
	.IMPORT cma__thread_start1
	.IMPORT cma__abort_process
	.IMPORT exit
	.IMPORT $$dyncall

#ifdef	PROF
	.IMPORT _mcount
#endif

; void *cma__fetch_sp ()
;
; Returns a pointer to the caller's top of stack.
;
cma__fetch_sp
	.PROC
	.CALLINFO NO_CALLS
	.ENTRY
	bv	%r0(%rp)
	.EXIT
	copy	%sp, %ret0
	.PROCEND

;
; void *cma__do_call_on_stack (stk, func, arg)
;    void                 *stk;               /* arg 0  */
;    void                 *(*func)();         /* arg 1 */
;    void                 *arg;               /* arg 2 */
;
stk	.REG	%arg0
func	.REG	%arg1
arg	.REG	%arg2
old_sp	.REG	%arg3

cma__do_call_on_stack
	.PROC
; Note: does not make calls on the current stack (uses stk).
	.CALLINFO NO_CALLS
	.ENTER
	copy	%sp, old_sp		; get current stack pointer

; Switch to the new stack and create/initialize a stack frame.
	copy	stk, %sp		; switch stacks
	ldo	frame_size(%sp), %sp	; allocate a stack frame on new stack
	stw	%rp, fm_crp(%sp)	; save return link
	stw	old_sp, fm_psp(%sp)	; save old value of sp
	ldw	fm_sl(old_sp), temp	; put caller's static link addr 
	stw	temp, fm_sl(%sp)	;    into callee's marker

	ldo	frame_size(%sp), %sp	; allocate a stack frame for callee
	copy	arg, %arg0	 	; pass target function's arg
	copy	func, %r22
	bl	$$dyncall,%r31		; invoke function correctly.
	copy	%r31, %rp

	ldo	-frame_size(%sp), %sp	; deallocate callee's stack frame

; Restore the caller's stack and pc.
        ldw	fm_crp(%sp), %rp	; Recover return link.
	ldw 	fm_psp(%sp), %sp	; Switch back to the caller's stack.
		
	.LEAVE                        	; %ret0 contains func's return value
	.PROCEND

;
; void cma__init_thread_stack (
;    void                 *chld_stk,
;    cma_t_int_tcb        *tcb,              
;    cma__t_static_ctx    *ctx)              
; 
; Initial stack
;    cma__thread_start0 stack frame
;	tcb is stored in arg0 save
;    cma__transfer_regs stack frame
;	general registers, space registers, and floating point registers
;	context is initialzed (usually to zero).
;
chd_stk	.REG	%arg0
tcb	.REG	%arg1
contxt	.REG	%arg2

cma__init_thread_stack
	.PROC
	.CALLINFO NO_CALLS

	.ENTER

	ldi	0xf, temp1		; adjust stack to an 8 byte boundary
	andcm	chd_stk, temp1, chd_stk

; Place marker onto stack with cma_start_thread0 as the target procedure.
; This marker belongs to the "caller" of cma_start_thread).
	ldo 	frame_size(chd_stk), chd_stk	; allocate a stack frame on TOS
	stw	%r0,	fm_crp(chd_stk)		; NULL rp
	stw	%r0,	fm_sl(chd_stk)		; NULL dynamic link
	stw	%r0,	fm_psp(chd_stk)		; NULL previous stack pointer

	stw	tcb,	va_arg0(chd_stk)	; Pass tcb to cma__thread_start0

; Place marker onto stack with cma__transfer_regs as the target procedure.
; This marker belongs to cma_start_thread0.
	ldo 	frame_size(chd_stk), chd_stk 	; create  stack frame
#ifdef PIC
	bl	.+8, temp2
	depi	0, 31, 2, temp2
LLL	addil	L%cma__thread_start0-LLL, temp2
	ldo	R%cma__thread_start0-LLL(%r1), temp2
#else
	ldil	L%cma__thread_start0, temp1	; rp = cma__thread_start0
	ldo	R%cma__thread_start0(temp1), temp2
#endif
	stw	temp2, fm_crp(chd_stk)
	stw	%r0,   fm_sl(chd_stk)		; NULL dynamic link
	stw	%r0,   fm_psp(chd_stk)		; NULL previous stack pointer

; Initialize child's "callee saves" general registers
	ldo 	callee_save_sz(chd_stk), chd_stk ; create  stack frame
	stw	%r0, r3save(chd_stk)
	stw	%r0, r4save(chd_stk)
	stw	%r0, r5save(chd_stk)
	stw	%r0, r6save(chd_stk)
	stw	%r0, r7save(chd_stk)
	stw	%r0, r8save(chd_stk)
	stw	%r0, r9save(chd_stk)
	stw	%r0, r10save(chd_stk)
	stw	%r0, r11save(chd_stk)
	stw	%r0, r12save(chd_stk)
	stw	%r0, r13save(chd_stk)
	stw	%r0, r14save(chd_stk)
	stw	%r0, r15save(chd_stk)
	stw	%r0, r16save(chd_stk)
	stw	%r0, r17save(chd_stk)
	stw	%r0, r18save(chd_stk)

#ifdef PIC
	addil	LT'_SYSTEM_ID,r19
	ldw	RT'_SYSTEM_ID(r1),r21
	ldw	(r21),r21
#else
	ldil	L%_SYSTEM_ID,r21
	ldw	R%_SYSTEM_ID(r21),r21
#endif
	ldi	0x20B,r31

; Initialize child's floating point registers
	fcpy	 %fr0, %fr4
	ldo	 fr12save(chd_stk), temp2
	fstds,ma %fr4, -8(temp2)
	fstds,ma %fr4, -8(temp2)
	fstds,ma %fr4, -8(temp2)

; If this is PA 1.1, extra floating point registers to initialize
	comb,<= r21,r31,init_not_pa89
	fstds,ma %fr4, -8(temp2)
	fstds,ma %fr4, -8(temp2)
	fstds,ma %fr4, -8(temp2)
	fstds,ma %fr4, -8(temp2)
	fstds,ma %fr4, -8(temp2)
	fstds,ma %fr4, -8(temp2)
	fstds	 %fr4,  0(temp2)
init_not_pa89
; What should the initial value be for %fr0
; For now we'll save its existing value.....
	ldo	 fr0save(chd_stk), temp2
	fstds    %fr0, 0(temp2)

; Initialize child's "callee saves" space registers.
	stw	%r0, sr3save(chd_stk)

; Save the child's stack pointer in its context save area.
    	stw	chd_stk, (contxt)	; (1st field)

	.LEAVE
	.PROCEND

;
; Atomically tests and clears the kernel serializer lock.
;
; int cma__serialize ()		; returns true if obtained serializer lock
;

cma__serialize
	.PROC
#ifdef	PROF
	.CALLINFO CALLER, SAVE_RP, FRAME=0, NO_UNWIND
	.ENTER
	copy	%rp, %arg0
	ldil	L'cma__serialize, %r1
	ldo	R'cma__serialize(%r1), %arg1
	addil	L'cnt-$global$, %r27
	.CALL
	bl	_mcount, %rp
	ldo	R'cnt-$global$ (%r1), %arg2
#else
	.CALLINFO NO_CALLS, NO_UNWIND
	.ENTER
#endif

#ifdef PIC
	ldw	T'cma__g_serialize(%r19), temp
#else
	addil	L'cma__g_serialize-$global$, %r27	;get &cma__g_serialize
	ldo	R'cma__g_serialize-$global$ (%r1), temp
#endif

	ldcws 	0(temp), temp      	;test and clear cma__g_serialize

	copy 	%r0, %ret0		;did we lock it?
	comb,=,n temp, 0, leave_serialize
	ldi	1, ret0			;yes

leave_serialize
	.LEAVE
	.PROCEND

;
;
;
; void cma__thread_start0 (
;   void *		tcb)
;
; Note: This routine is "exited" into rather than called.  Its stack frame
;       has already been created.

cma__thread_start0
	.PROC
;	.CALLINFO NO_CALLS, NO_UNWIND
; Need to create unwind descriptor and
; save PSP on stack ...
	.CALLINFO NO_CALLS, SAVE_SP
  
; Save PSP ...
	stw	%r0,-4(0,%sp)
; Just call the C routine, cma__thread_start1
	bl 	cma__thread_start1, %rp	; cma__thread_start1 (tcb)
	ldw	va_arg0-frame_size(%sp), %arg0	

; Cma__thread_start1 should never return, but if it does then we quit.
	bl	exit, %rp		; exit (1);
	ldi	1, %arg0
	
	.PROCEND

;
; void cma__restore_regs (
;    cma__t_static_ctx    *new_ctx)           /* 4(%sp) */
;
; Restore the thread whose context is "new_ctx".
;

cma__restore_regs
	.PROC
	.CALLINFO FRAME=callee_save_sz, NO_CALLS

new_ctx	.REG	%arg0

	ldw	(new_ctx), %sp		; switch to target stack

; Restore "callee saves" space registers.
	ldw	sr3save(%sp), %r1
	mtsp	%r1, %sr3

#ifdef PIC
	addil   LT'_SYSTEM_ID,r19
	ldw     RT'_SYSTEM_ID(r1),r21
	ldw     (r21),r21
#else
	ldil    L%_SYSTEM_ID,r21
	ldw     R%_SYSTEM_ID(r21),r21
#endif
	ldi	0x20B,r31

; Restore "callee saves" floating point registers.
	ldo	 fr12save(%sp), temp2
	fldds,ma -8(temp2), %fr12
	fldds,ma -8(temp2), %fr13
	fldds,ma -8(temp2), %fr14

; If this is PA1.1, restore extra fp regs
	comb,<= r21,r31,restore_not_pa89
	fldds,ma -8(temp2), %fr15
	fldds,ma -8(temp2), %fr16
	fldds,ma -8(temp2), %fr17
	fldds,ma -8(temp2), %fr18
	fldds,ma -8(temp2), %fr19
	fldds,ma -8(temp2), %fr20
	fldds	  0(temp2), %fr21
restore_not_pa89
	ldo      fr0save(%sp), temp2
	fldds	 0(temp2), %fr0

; Restore "callee saves" general registers.
	ldw	fm_crp-callee_save_sz(%sp),  %r2 ; %rp
	ldw	r3save(%sp),  %r3
	ldw	r4save(%sp),  %r4
	ldw	r5save(%sp),  %r5
	ldw	r6save(%sp),  %r6
	ldw	r7save(%sp),  %r7
	ldw	r8save(%sp),  %r8
	ldw	r9save(%sp),  %r9
	ldw	r10save(%sp), %r10
	ldw	r11save(%sp), %r11
	ldw	r12save(%sp), %r12
	ldw	r13save(%sp), %r13
	ldw	r14save(%sp), %r14
	ldw	r15save(%sp), %r15
	ldw	r16save(%sp), %r16
	ldw	r17save(%sp), %r17
	ldw	r18save(%sp), %r18

; Return to the target thread.
	bv	%r0(%rp)
	ldo	-callee_save_sz(%sp), %sp
    .PROCEND

;
; void cma__transfer_regs (
;    cma__t_static_ctx    *cur_ctx,
;    cma__t_static_ctx    *trg_ctx)	/* "target_context" of new thread */
;
; Save the current threads's register context onto the stack and cur_ctx,
; then launch the new thread by restoring the new thread's "new_ctx."
;
 
cma__transfer_regs
	.PROC
	.CALLINFO FRAME=callee_save_sz, NO_CALLS
; For some reason using .ENTER on 8.05 allocates a frame marker -
; even though we specified NO_CALLS - whereas previously it did not.
; Since this messes up our offsets, we'll allocate the user stack 
; frame manually.
;	.ENTER

cur_ctx	.REG	%arg0
trg_ctx .REG	%arg1

; allocate space for the saved registers
	ldo	callee_save_sz(%sp), %sp

; Save "callee saves" general registers
	stw	%r2,  fm_crp-callee_save_sz(%sp) ; %rp
	stw	%r3,  r3save(%sp)
	stw	%r4,  r4save(%sp)
	stw	%r5,  r5save(%sp)
	stw	%r6,  r6save(%sp)
	stw	%r7,  r7save(%sp)
	stw	%r8,  r8save(%sp)
	stw	%r9,  r9save(%sp)
	stw	%r10, r10save(%sp)
	stw	%r11, r11save(%sp)
	stw	%r12, r12save(%sp)
	stw	%r13, r13save(%sp)
	stw	%r14, r14save(%sp)
	stw	%r15, r15save(%sp)
	stw	%r16, r16save(%sp)
	stw	%r17, r17save(%sp)
	stw	%r18, r18save(%sp)

#ifdef PIC
	addil	LT'_SYSTEM_ID,r19
	ldw	RT'_SYSTEM_ID(r1),r21
	ldw	(r21),r21
#else
	ldil	L%_SYSTEM_ID,r21
	ldw	R%_SYSTEM_ID(r21),r21
#endif
	ldi	0x20B,r31

; Save "callee saves" floating point registers.
	ldo	 fr0save(%sp), temp2
	fstds,ma %fr0,  -8(temp2)
	fstds,ma %fr12, -8(temp2)
	fstds,ma %fr13, -8(temp2)
	fstds,ma %fr14, -8(temp2)

; If this is PA 1.1, extra floating point registers to save
	comb,<= r21,r31,xfer_not_pa89
	fstds,ma %fr15, -8(temp2)
	fstds,ma %fr16, -8(temp2)
	fstds,ma %fr17, -8(temp2)
	fstds,ma %fr18, -8(temp2)
	fstds,ma %fr19, -8(temp2)
	fstds,ma %fr20, -8(temp2)
	fstds	 %fr21,  0(temp2)
xfer_not_pa89

; Save "callee saves" space registers.
	mfsp	%sr3, temp
	stw	temp, sr3save(%sp)

	stw	%sp, (cur_ctx)		; save stack pointer 

; Get new context and launch the new thread.
;	ldil	L%cma__restore_regs, temp
;	ldo	R%cma__restore_regs(temp), temp
;	bv 	%r0(temp)
	b	cma__restore_regs
	copy	trg_ctx, cur_ctx
    .PROCEND

;
;
;
; void cma__transfer_main_abort()
;
; Note: This routine does not return.  It transfers to the main thread
;	stack and then calls cma__abort_process

cma__transfer_main_abort
	.PROC
	.CALLINFO NO_CALLS, NO_UNWIND

;
; switch to main thread stack
;
#ifdef PIC
	ldw	T'cma__g_def_ctx_buf(%r19), temp
#else
	addil	L'cma__g_def_ctx_buf-$global$, %r27
	ldo	R'cma__g_def_ctx_buf-$global$ (%r1), temp
#endif
	ldw	0(temp), temp
	ldw	0(temp), %sp

	ldo	48(%sp), %sp		; allocate a stack frame & arg area
					; - probably don't need to do this
					;   since we're terminating, but
					;   it's cleaner
;
; call cma__abort_process with cma__g_abort_signal as the only argument
;
#ifdef PIC
	ldw	T'cma__g_abort_signal(%r19), temp
#else
	addil	L'cma__g_abort_signal-$global$, %r27
	ldo	R'cma__g_abort_signal-$global$ (%r1), temp
#endif

	bl	cma__abort_process, %rp
	ldw	0(temp), %arg0

; cma__abort_process should never return, but if it does then we quit.
	bl	exit, %rp		; exit (1);
	ldi	1, %arg0

	.PROCEND
;   DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.S 
;   *3    17-SEP-1991 14:53:00 BUTENHOF "Integrate CMA5 reverse drop" 
;   *2    12-MAR-1991 19:00:38 SCALES "Updates from HP for CD4" 
;   *1     6-DEC-1990 22:38:50 SCALES "HP-specific assembler code" 
;   DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.S 
