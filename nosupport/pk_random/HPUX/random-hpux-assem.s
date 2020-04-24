;;;
;;; @OSF_COPYRIGHT@
;;; COPYRIGHT NOTICE
;;; Copyright (c) 1996 Open Software Foundation, Inc.
;;; ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
;;; the full copyright text.
;;; All Rights Reserved
;;;
;;;  Interrupt service "hook" routine we patch in place of eirr table.
;;;
;;;  Uses:	t1,t2,t3,t4 (r22-r19) (software scratch regs)
;;;             r1 (architectural scratch reg)
;;;  Called with:
;;;	arg0:	 interrupt bit number (0..31)
;;;	arg1:	 save state pointer.
;;;	arg2:	 interrupt routine argument
;;;	rp:	 return address.
;;; 
;;;  This routine tail-calls the "real" handler when done.
;;;  TODO:	 ??shave a cycle by putting the STW in the delay slot 
;;;		 of the BE..
	.code	
	.export randomhook,code
	.import randomhookbase,data
	.import $global$,data

#include "m4_reg.h"

rhvec	.equ	0	
rhbase	.equ	4
rhend	.equ	8		

randomhook
	MFCTL	%cr16, t1			 ; cycle counter
        ADDIL   LR'randomhookbase-$global$,%dp,%r1	 ; common base..
        LDW     RR'randomhookbase-$global$+rhvec(%r1),t4  ; "real" vec ptr
        LDW     RR'randomhookbase-$global$+rhbase(%r1),t2   ; current ptr
        LDW     RR'randomhookbase-$global$+rhend(%r1),t3    ; end ptr
	LDWX,S	%arg0(t4), t4			 ; "real" interrupt vector
        COMCLR,>>=      t2,t3,%r0		 ; room in buffer??
        STWS,MA  t1,4(t2)			 ; [>>=] yup, save it..
	BE	0(t4)				 ; and tail-call "real" handler
        STW     t2,RR'randomhookbase-$global$+rhbase(%r1)  ; [DELAY] and the ptr, too.
