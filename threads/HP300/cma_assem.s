 # @OSF_COPYRIGHT@
 # COPYRIGHT NOTICE
 # Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 # ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 # the full copyright text.
 # HISTORY
 # $Log: cma_assem.s,v $
 # Revision 1.1.6.2  1996/03/09  23:30:29  marty
 # 	Update OSF copyright year
 # 	[1996/03/09  22:45:46  marty]
 #
 # Revision 1.1.6.1  1995/12/07  21:45:01  root
 # 	Submit OSF/DCE 1.2.1
 # 	[1995/12/07  20:58:53  root]
 # 
 # Revision 1.1.4.1  1994/06/09  13:33:56  devsrc
 # 	 CR10892 - fix copyright in file
 # 	[1994/06/09  13:25:08  devsrc]
 # 
 # Revision 1.1.2.2  1992/12/15  22:19:57  alan
 # 	Insert copyright notices
 # 	[1992/12/07  15:58:38  alan]
 # 
 # Revision 1.1  1992/01/19  18:34:11  devrcs
 # 	Initial revision
 # 
 # $EndLog$
 #
 #
    version 1
 #   MODULE  cma_assem_hp9000s300

 ### Exported routines:
    global	_cma__fetch_sp
    global	_cma__do_call_on_stack
    global	_cma__test_and_set
    global	_cma__init_thread_stack
    global      _cma__do_async_alert
    global	_cma__force_dispatch
    global	_cma__restore_regs
    global	_cma__transfer_regs
    global	_cma__thread_start0
    global	_cma__unset
    global	_cma__transfer_main_abort

 ### Imported symbols:
    global	_exit
    global      _cma__async_delivery
    global      _cma__bugcheck
    global      _cma__get_async_info
    global	_cma__thread_start1
    global      _cma__undefer
    global	_cma__yield_processor
    global	_cma__abort_process
    global	_cma__g_kernel_critical
    global	_cma__g_def_ctx_buf
    global	_cma__g_abort_signal

 #
 # void *cma__fetch_sp ()
 #
_cma__fetch_sp:
   move.l %fp,%d0
   rts

 #
 # void *cma__do_call_on_stack (stk, func, arg)
 #    void                 *stk;               /* 8(%fp)  */
 #    void                 *(*func)();         /* 12(%fp) */
 #    void                 *arg;               /* 16(%fp) */
 #
_cma__do_call_on_stack:
    link.l  %fp,&0			#save current value of sp
    move.l  8(%fp),%sp             	# switch stacks
    move.l  16(%fp),-(%sp)         	# push arg
    move.l  12(%fp),%a0            	# get func
    jsr     (%a0)                	# call func
    unlk    %fp                  	# restore old stack
    rts                          	# %d0 contains func's return value

#
# int cma__test_and_set(address)
#    int 		   *address;           /* 4(%sp) */
#
_cma__test_and_set:
    move.l  4(%sp),%a0
    bset    &0,3(%a0)             # set least sig bit in 32 bit word
    bne     not_zero
    clr.l   %d0
    rts
not_zero:
    moveq.l &1,%d0
    rts

#
# void cma__unset(address)
#    int 		   *address;           /* 4(%sp) */
#
_cma__unset:
    move.l  4(%sp),%a0
    clr.l   (%a0)
    rts

 #
 # void cma__init_thread_stack(stk, tcb, ctx)
 #    void                 *stk;               /* 8(%fp) */
 #    cma_t_int_tcb        *tcb;               /* 12(%fp) */
 #    cma__t_static_ctx    *ctx;               /* 16(%fp) */
 # 
 # Initial stack:
 #    top:    cma__thread_start0
 #    top-4:  NULL
 #    top-8:  tcb
 #    top-12: cma__thread_start0
 #    top-16: cma__thread_start1   	# start addr after first cma__restore_regs
 #    top-20: top-4			# fake dynamic link
 #    top-24: a5
 #    ...
 #    top-60: d2
 #
 # ???more explanation...
 #
_cma__init_thread_stack:
    link.l  %fp,&0
ifdef(`PIC',`
    move.l  %a2,-(%sp)			# save register a2
    move.l  &DLT,%a2			# PC-relative offset to Data Link Table
    lea.l   -6(%pc,%a2.l),%a2		# addr of DLT
')
    and.l   &-4,8(%fp)            	# align stack on 4 byte boundary
    move.l  8(%fp),%a0             	# get child's stack

ifdef(`PIC',`
    move.l  _cma__thread_start0(%a2),-(%a0)
',`
    move.l  &_cma__thread_start0,-(%a0)	# push fake return addr (so debugger happy)
')
    move.l  &0,-(%a0)             	# push a NULL dynamic link (so debugger happy)
    move.l  %a0,%a1                	# remember link addr for later

    move.l  12(%fp),-(%a0)         	# push tcb (arg to cma__thread_startup)
ifdef(`PIC',`
    move.l  _cma__thread_start0(%a2),-(%a0)
    move.l  _cma__thread_start1(%a2),-(%a0)
',`
    move.l  &_cma__thread_start0,-(%a0)	# fake return addr???
    move.l  &_cma__thread_start1,-(%a0)	# push addr thread will start at
')
    move.l  %a1,-(%a0)             	# save fake dynamic link addr in ctx 
					# (so debugger happy)
    movem.l %a2-%a5/%d2-%d7,-(%a0) 	# do a ctx save (a6 was pushed in prev instr)

    # save current floating point register values as initial values
    # for new thread
ifdef(`PIC',`
    move.l  flag_68881(%a2),%a1
    tst.w   (%a1)
',`
    tst.w   flag_68881			# 68881 present?
')
    beq.w   Dno_68881
    fmov.l  %fpcr,-(%a0)		# save control reg
    fmovm.x %fp2-%fp7,-(%a0)		# save data regs
Dno_68881:
ifdef(`PIC',`
    move.l  flag_fpa(%a2),%a1
    tst.w   (%a1)
    beq.w   Dno_fpa
    move.l  fpa_loc(%a2),%a1 
',`
    tst.w   flag_fpa			# FPA present?
    beq.w   Dno_fpa
    lea.l   fpa_loc,%a1			# get base address of FPA
')
    fpareg  %a1
    sub.l   &108,%a0			# make space for FPA regs
    fpmov.l %fpacr,104(%a0)		# save control and
    fpmov.d %fpa15,96(%a0)		#       data registers
    fpmov.d %fpa14,88(%a0)
    fpmov.d %fpa13,80(%a0)
    fpmov.d %fpa12,72(%a0)
    fpmov.d %fpa11,64(%a0)
    fpmov.d %fpa10,56(%a0)
    fpmov.d %fpa9,48(%a0)
    fpmov.d %fpa8,40(%a0)
    fpmov.d %fpa7,32(%a0)
    fpmov.d %fpa6,24(%a0)
    fpmov.d %fpa5,16(%a0)
    fpmov.d %fpa4,8(%a0)
    fpmov.d %fpa3,0(%a0)
Dno_fpa:

    move.l  16(%fp),%a1            	# get child's static ctx
    move.l  %a0,(%a1)             	# save child's %sp in its ctx (1st field)
ifdef(`PIC',`
    move.l  (%sp)+,%a2
')
    unlk    %fp
    rts

 #
 # Fake procedure to make debugger happy 
 # Never executed, just need the address
 # Will show up in trace-back as thread entry point ???doesn't work yet
 #
_cma__thread_start0:
    rts

#
# void cma__do_async_alert()
#
_cma__do_async_alert:
ifdef(`PIC',`
    bsr.l   _cma__undefer
',`
    jsr     _cma__undefer      	       # leaving critical region,
      	       	     	      	       # check for deferred actions
')

ifdef(`PIC',`
    move.l  &DLT,%a1
    lea.l   -6(%pc,%a1.l),%a1
    move.l  _cma__g_kernel_critical(%a1),%a0
    clr.l   (%a0)
',`
    clr.l   _cma__g_kernel_critical    # leave critical region
')

ifdef(`PIC',`
    bsr.l   _cma__async_delivery
',`
    jsr	    _cma__async_delivery       # Deliver the alert
')

 #
 # This point should never be reached, since the CMA exception package doesn't
 # support continuable exceptions.
 #
ifdef(`PIC',`
    bsr.l   _cma__bugcheck
',`
    jsr     _cma__bugcheck     	       # Handle the error
')

 #
 # void cma__force_dispatch ()
 #
_cma__force_dispatch:
    sub     &6,%sp                	# make space for pc+ccr
    movem.l %a0-%a1/%d0-%d1,-(%sp)    	# push regs not preserved across calls
    lea.l   16(%sp),%a1
    move.l  %a1,%d0
ifdef(`PIC',`
    move.l  &DLT,%a1
    lea.l   -6(%pc,%a1.l),%a1
    move.l  flag_68881(%a1),%a0
    tst.w   (%a0)
',`
    tst.w   flag_68881			# 68881 present?
')
    beq.w   Eno_68881
    fmov.l  %fpsr,-(%sp)		# save fp status reg
    fmovm.x %fp0-%fp1,-(%sp)		# save fp scratch data regs
Eno_68881:
ifdef(`PIC',`
    move.l  flag_fpa(%a1),%a0
    tst.w   (%a0)
    beq.w   Eno_fpa
    move.l  fpa_loc(%a1),%a0 
',`
    tst.w   flag_fpa			# FPA present?
    beq.w   Eno_fpa
    lea.l   fpa_loc,%a0			# get base address of FPA
')
    fpareg  %a0
    sub.l   &28,%sp			# make space for FPA regs
    fpmov.l %fpasr,24(%sp)		# save status and
    fpmov.d %fpa2,16(%sp)		#       scratch registers
    fpmov.d %fpa1,8(%sp)
    fpmov.d %fpa0,0(%sp)
Eno_fpa:
    move.l  %d0,-(%sp)
ifdef(`PIC',`
    bsr.l   _cma__get_async_info
',`
    jsr     _cma__get_async_info  	# %d0 gets addr of saved pc+ccr 
')
    move.l  (%sp)+,%a1
    move.l  %d0,%a0                	# move to %a0 so can use as pointer
    move.w  4(%a0),0(%a1)         	# put ccr on stack
    move.l  (%a0),2(%a1)          	# put interrupted pc on stack
ifdef(`PIC',`
    bsr.l   _cma__yield_processor
    bsr.l   _cma__undefer
',`
    jsr     _cma__yield_processor 	# preempt current thread
    jsr     _cma__undefer         	# leaving critical region, check for 
')
					# deferred actions
ifdef(`PIC',`
    move.l  &DLT,%a1
    lea.l   -6(%pc,%a1.l),%a1
    move.l   _cma__g_kernel_critical(%a1),%a0
    clr.l   (%a0)
    move.l  flag_fpa(%a1),%a0
    tst.w   (%a0)
    beq.w   Fno_fpa
    move.l  fpa_loc(%a1),%a0
',`
    clr.l   _cma__g_kernel_critical  	# leave critical region
    tst.w   flag_fpa			# FPA present?
    beq.w   Fno_fpa
    lea.l   fpa_loc,%a0			# get base address of FPA
')
    fpareg  %a0
    fpmov.d 0(%sp),%fpa0		# restore scratch and
    fpmov.d 8(%sp),%fpa1		#	status registers
    fpmov.d 16(%sp),%fpa2
    fpmov.l 24(%sp),%fpasr
    add.l   &28,%sp			# make space for FPA regs
Fno_fpa:
ifdef(`PIC',`
    move.l  flag_68881(%a1),%a0
    tst.w   (%a0)
',`
    tst.w   flag_68881			# 68881 present?
')
    beq.w   Fno_68881
    fmovm.x (%sp)+,%fp0-%fp1		# pop fp scratch data regs
    fmov.l  (%sp)+,%fpsr		# pop fp status reg
Fno_68881:
    movem.l (%sp)+,%a0-%a1/%d0-%d1    	# pop regs not preserved across calls
    rtr                          	# restore ccr and return to interrupted 
					# instruction

 #
 # void cma__restore_regs (new_ctx)
 #    cma__t_static_ctx    *new_ctx;           /* 4(%sp) */
 #
_cma__restore_regs:
    move.l  4(%sp),%a0             	# get new_ctx
    move.l  (%a0),%sp              	# use new stack
ifdef(`PIC',`
    move.l  &DLT,%a1
    lea.l   -6(%pc,%a1.l),%a1
    move.l  flag_fpa(%a1),%a0
    tst.w   (%a0)
    beq.w   Ano_fpa
    move.l  fpa_loc(%a1),%a2
',`
    tst.w   flag_fpa			# FPA present?
    beq.w   Ano_fpa
    lea.l   fpa_loc,%a2			# get FPA location
')
    fpareg  %a2
    fpmov.d 0(%sp),%fpa3		# restore data regs
    fpmov.d 8(%sp),%fpa4
    fpmov.d 16(%sp),%fpa5
    fpmov.d 24(%sp),%fpa6
    fpmov.d 32(%sp),%fpa7
    fpmov.d 40(%sp),%fpa8
    fpmov.d 48(%sp),%fpa9
    fpmov.d 56(%sp),%fpa10
    fpmov.d 64(%sp),%fpa11
    fpmov.d 72(%sp),%fpa12
    fpmov.d 80(%sp),%fpa13
    fpmov.d 88(%sp),%fpa14
    fpmov.d 96(%sp),%fpa15
    fpmov.l 104(%sp),%fpacr		# restore control reg
    add.l   &108,%sp
Ano_fpa:
ifdef(`PIC',`
    move.l  flag_68881(%a1),%a0
    tst.w   (%a0)
',`
    tst.w   flag_68881
')
    beq.w   Ano_68881
    fmovm.x (%sp)+,%fp2-%fp7
    fmov.l  (%sp)+,%fpcr
Ano_68881:
    movem.l (%sp)+,%a2-%a6/%d2-%d7    	# pop regs preserved across calls
    rts

 #
 # void cma__transfer_regs(cur_ctx, new_ctx)
 #    cma__t_static_ctx    *cur_ctx;           /* 4(%sp) */
 #    cma__t_static_ctx    *new_ctx;           /* 8(%sp) */
 # 
_cma__transfer_regs:
    move.l  4(%sp),%a0             	# get cur_ctx
    move.l  8(%sp),%a1             	# get new_ctx
    movem.l %a2-%a6/%d2-%d7,-(%sp)    	# push regs preserved across calls
ifdef(`PIC',`
    move.l  &DLT,%a2
    lea.l   -6(%pc,%a2.l),%a2
    move.l  flag_68881(%a2),%a3
    tst.w   (%a3)
',`
    tst.w   flag_68881			# 68881 present?
')
    beq.w   Bno_68881
    fmov.l  %fpcr,-(%sp)		# save control reg
    fmovm.x %fp2-%fp7,-(%sp)		# save data regs
Bno_68881:
ifdef(`PIC',`
    move.l  flag_fpa(%a2),%a3
    tst.w   (%a3)
    beq.w   Bno_fpa
    move.l  fpa_loc(%a2),%a4
',`
    tst.w   flag_fpa			# FPA present?
    beq.w   Bno_fpa
    lea.l   fpa_loc,%a4			# get base address of FPA
')
    fpareg  %a4
    sub.l   &108,%sp			# make space for FPA regs
    fpmov.l %fpacr,104(%sp)		# save control and
    fpmov.d %fpa15,96(%sp)		#       data registers
    fpmov.d %fpa14,88(%sp)
    fpmov.d %fpa13,80(%sp)
    fpmov.d %fpa12,72(%sp)
    fpmov.d %fpa11,64(%sp)
    fpmov.d %fpa10,56(%sp)
    fpmov.d %fpa9,48(%sp)
    fpmov.d %fpa8,40(%sp)
    fpmov.d %fpa7,32(%sp)
    fpmov.d %fpa6,24(%sp)
    fpmov.d %fpa5,16(%sp)
    fpmov.d %fpa4,8(%sp)
    fpmov.d %fpa3,0(%sp)
Bno_fpa:
    move.l  %sp,(%a0)              	# save current %sp
    move.l  (%a1),%sp              	# use new stack
ifdef(`PIC',`
    move.l  flag_fpa(%a2),%a3
    tst.w   (%a3)
    beq.w   Cno_fpa
',`
    tst.w   flag_fpa			# FPA present?
    beq.w   Cno_fpa
')
    fpareg  %a4
    fpmov.d 0(%sp),%fpa3		# restore data regs
    fpmov.d 8(%sp),%fpa4
    fpmov.d 16(%sp),%fpa5
    fpmov.d 24(%sp),%fpa6
    fpmov.d 32(%sp),%fpa7
    fpmov.d 40(%sp),%fpa8
    fpmov.d 48(%sp),%fpa9
    fpmov.d 56(%sp),%fpa10
    fpmov.d 64(%sp),%fpa11
    fpmov.d 72(%sp),%fpa12
    fpmov.d 80(%sp),%fpa13
    fpmov.d 88(%sp),%fpa14
    fpmov.d 96(%sp),%fpa15
    fpmov.l 104(%sp),%fpacr		# restore control reg
    add.l   &108,%sp
Cno_fpa:
ifdef(`PIC',`
    move.l  flag_68881(%a2),%a3
    tst.w   (%a3)
',`
    tst.w   flag_68881			# 68881 present?
')
    beq.w   Cno_68881
    fmovm.x (%sp)+,%fp2-%fp7
    fmov.l  (%sp)+,%fpcr
Cno_68881:
    movem.l (%sp)+,%a2-%a6/%d2-%d7    	# pop regs preserved across calls
    rts

 #
 # cma__transfer_main_abort()
 # 
_cma__transfer_main_abort:

 # 
 # switch to main thread's stack
 #
ifdef(`PIC',`
    move.l  &DLT,%a1				# addr of Data Link Table
    lea.l   -6(%pc,%a1.l),%a1
    move.l  _cma__g_def_ctx_buf(%a1),%a0
    move.l   (%a0),%a0				# a0 <- addr of main thread ctx
',`
    move.l  _cma__g_def_ctx_buf,%a0
')
    move.l  (%a0),%sp              		# main thread's stack

 #
 # call cma__abort_process with cma__g_abort_signal as the only argument
 #
ifdef(`PIC',`
    move.l  _cma__g_abort_signal(%a1),%a0
    move.l  (%a0),-(%sp)
    bsr.l   _cma__abort_process
',`
    move.l  _cma__g_abort_signal,-(%sp)
    jsr     _cma__abort_process
')

 #
 # should never return, but if we do then exit
 #
    move.l  &1,-(%sp)
ifdef(`PIC',`
    bsr.l   _exit
',`
    jsr     _exit
')
    rts

 #  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.S 
 #  *2    17-SEP-1991 14:43:38 BUTENHOF "Integrate CMA5 reverse drop" 
 #  *1     7-DEC-1990 16:22:02 SCALES "HP-specific assembler code" 
 #  DEC/CMS REPLACEMENT HISTORY, Element CMA_ASSEM.S 
