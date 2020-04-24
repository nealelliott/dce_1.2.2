* @OSF_COPYRIGHT@
* COPYRIGHT NOTICE
* Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
* ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
* the full copyright text.
* 
* HISTORY
* $Log: cma_assem.asm,v $
* Revision 1.1.6.2  1996/03/09  23:30:24  marty
* 	Update OSF copyright year
* 	[1996/03/09  22:45:42  marty]
*
* Revision 1.1.6.1  1995/12/07  21:42:31  root
* 	Submit OSF/DCE 1.2.1
* 	[1995/12/07  20:58:00  root]
* 
* Revision 1.1.4.1  1994/06/09  13:33:16  devsrc
* 	 CR10892 - fix copyright in file
* 	[1994/06/09  13:24:38  devsrc]
* 
* Revision 1.1.2.2  1992/12/15  22:19:28  alan
* 	Insert copyright notices
* 	[1992/12/07  18:12:08  alan]
* 
* $EndLog$

    MODULE  cma_assem

*** Exported routines:
    entry.f cma__fetch_sp
    entry.f cma__do_call_on_stack
    entry.f cma__do_async_alert
    entry.p cma__init_thread_stack
    entry.p cma__force_dispatch
    entry.p cma__restore_regs
    entry.p cma__transfer_regs
    entry.p cma__thread_start0
    entry.p cma__test_and_set
    entry.p cma__unset
    entry.p caller_sb

*** Imported symbols:
    extern.p cma__async_delivery
    extern.p cma__bugcheck
    extern.p cma__thread_start1
    extern.f cma__get_async_info
    extern.p cma__yield_processor
    extern.p cma__undefer
    extern.d cma__g_kernel_critical
    extern.d cma__g_def_ctx_buf

*** Note: all routines are in data sect so don't have to be PIC, whew
    DATA

*
* void *cma__fetch_sp()
*
cma__fetch_sp equ *
    move.l sb,d0
    rts

*
* cma_t_boolean cma__test_and_set(address)
* unsigned long *address;   /* 4(sp) */
*
cma__test_and_set equ *
    move.l  4(sp),a0
    bset    #0,3(a0)
    bne     was_set
    clr.l   d0
    rts
was_set equ *
    move.l  #1,d0
    rts

*
* void cma__unset(address)
* unsigned long *address;   /* 4(sp) */
*
cma__unset equ *
    move.l  4(sp),a0
    bclr    #0,3(a0)
    rts

*
* void *cma__do_call_on_stack(stk, func, arg)
* void *stk;         /* 8(sb)  */
* void *(*func)();   /* 12(sb) */
* void *arg;         /* 16(sb) */
*
cma__do_call_on_stack equ *
    link    sb,#0
    move.l  8(sb),sp             * switch stacks
    move.l  16(sb),-(sp)         * push arg
    move.l  12(sb),a0            * get func
    jsr     (a0)                 * call func
    unlk    sb                   * restore old stack
    rts                          * d0 contains func's return value

*
* void cma__init_thread_stack(stk, tcb, ctx)
* void *stk;               /* 8(sb) */
* cma_t_int_tcb *tcb;      /* 12(sb) */
* cma__t_static_ctx *ctx;  /* 16(sb) */
*
* Initial stack:
*    top:    cma__thread_start0
*    top-4:  NULL
*    top-8:  tcb
*    top-12: cma__thread_start0
*    top-16: cma__thread_start1   * start addr after first cma__restore_regs
*    top-20: top-4		* fake dynamic link
*    top-24: a5
*    ...
*    top-60: d2
*
* ???more explanation...
*
cma__init_thread_stack PROCEDURE #0 NOXEP
* cma__init_thread_stack equ *
*    link    sb,#0
    and.l   #-4,8(sb)            * align stack on 4 byte boundary
    move.l  8(sb),a0             * get child's stack

    move.l  #cma__thread_start0,-(a0)   * push fake return addr (so debugger happy)
    move.l  #0,-(a0)             * push a NULL dynamic link (so debugger happy)
    move.l  a0,a1                * remember link addr for later

    move.l  12(sb),-(a0)         * push tcb (arg to cma__thread_startup)
    move.l  #cma__thread_start0,-(a0) * fake return addr???
    move.l  #cma__thread_start1,-(a0) * push addr thread will start at
    move.l  a1,-(a0)             * save fake dynamic link addr in ctx (so debugger happy)
    movem.l a2-a5/d2-d7,-(a0)    * do a ctx save (a6 was pushed in prev instr)
    move.l  16(sb),a1            * get child's static ctx
    move.l  a0,(a1)              * save child's sp in its ctx (1st field)
    RETURN cma__init_thread_stack
*    unlk    sb
*    rts

*
* Fake procedure to make debugger happy 
* Never executed, just need the address
* Will show up in trace-back as thread entry point ???doesn't work yet
*
* cma__thread_start0 equ *
cma__thread_start0 PROCEDURE NONSTANDARD NOXEP
    rts

*
* void cma__do_async_alert()
*
cma__do_async_alert equ *
    jsr     cma__undefer         * leaving critical region, check for deferred actions
    clr.l   cma__g_kernel_critical  * leave critical region
    jsr	    cma__async_delivery	 * Deliver the alert
*
* This point should never be reached, since the CMA exception package doesn't
* support continuable exceptions.
*
    jsr     cma__bugcheck	 * Handle the error

*
* void cma__force_dispatch()
*
cma__force_dispatch equ *
    sub     #6,sp                * make space for pc+ccr
    movem.l a0-a1/d0-d1,-(sp)    * push regs not preserved across calls
    jsr     cma__get_async_info  * d0 gets addr of saved pc+ccr 
    move.l  d0,a0                * move to a0 so can use as pointer
    move.w  4(a0),16(sp)         * put ccr on stack
    move.l  (a0),18(sp)          * put interrupted pc on stack
    jsr     cma__yield_processor * preempt current thread
    jsr     cma__undefer         * leaving critical region, check for deferred actions
    clr.l   cma__g_kernel_critical  * leave critical region
    movem.l (sp)+,a0-a1/d0-d1    * pop regs not preserved across calls
    rtr                          * restore ccr and return to interrupted instruction

*
* void cma__restore_regs(new_ctx)
* cma__t_static_ctx *new_ctx;  /* 4(sp) */
*
cma__restore_regs equ *
    move.l  4(sp),a0             * get new_ctx
    move.l  (a0),sp              * use new stack
    movem.l (sp)+,a2-a6/d2-d7    * pop regs preserved across calls
    rts

*
* void cma__transfer_regs(cur_ctx, new_ctx)
* cma__t_static_ctx *cur_ctx;  /* 4(sp) */
* cma__t_static_ctx *new_ctx;  /* 8(sp) */
*
cma__transfer_regs equ *
    move.l  4(sp),a0             * get cur_ctx
    move.l  8(sp),a1             * get new_ctx
    movem.l a2-a6/d2-d7,-(sp)    * push regs preserved across calls
    move.l  sp,(a0)              * save current sp
    move.l  (a1),sp              * use new stack
    movem.l (sp)+,a2-a6/d2-d7    * pop regs preserved across calls
    rts

caller_sb equ *
    move.l sb,d0
    rts

    END
*   DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.ASM 
*   *3    17-SEP-1991 15:05:45 BUTENHOF "Integrate CMA5 reverse drop" 
*   *2     6-DEC-1990 21:01:31 SCALES "Convert to stream format for ULTRIX build" 
*   *1     6-DEC-1990 20:57:37 SCALES "Apollo Domain/OS-specifi assembler routines" 
*   DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.ASM 
