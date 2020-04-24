/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: myexc_handling.h,v $
 * Revision 1.1.2.2  1996/03/09  20:49:23  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:27:47  marty]
 *
 * Revision 1.1.2.1  1995/12/11  19:34:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:18:25  root]
 * 
 * $EndLog$
 */
#define ENUM_EXC_STATE_T	
#define ENUM_EXC_KIND_T		
#define STRUCT_EXC_EXCEPTION_T	
/*
 *  Copyright (c) 1993, 1995 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and  copied
 *  only  in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any  other
 *  copies  thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is  hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without  notice
 *  and  should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	DECthreads exception services for WindowsNT
 *
 *  FILENAME:
 *
 * 	EXC_HANDLING.H
 *
 *  ABSTRACT:
 *
 *	Header file for exception handling in C
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *	Brian Keane
 *	Jerry Harrow
 *
 *  CREATION DATE:
 *
 *	9 November 1993
 *
 *  LAST MODIFIED DATE:
 *	 5 September 1995
 */




#ifndef EXC_HANDLING
# define EXC_HANDLING	1

#ifdef __cplusplus
    extern "C" {
#endif

/*
 *  INCLUDE FILES
 */

#ifndef CMA_CONFIG
# include <dce/cma_config.h>
#endif

#include <windows.h>

/*
 *  CONSTANTS AND MACROS
 */

# define exc_facility_c	0xC1E80000L

# define _CMA_STATUS_(val, sev) \
	((exc_int_t)(exc_facility_c | (val)))

#ifndef _CMA_STATUS_
# define _CMA_DCE_PREFIX_	0x177db000
# define exc_facility_c		_CMA_DCE_PREFIX_
# define _CMA_STATUS_(val, sev) \
	((exc_int_t)(_CMA_DCE_PREFIX_ | (val)))
#endif




/*
 * EXC facility messages
 */

#define exc_s_exception         _CMA_STATUS_(1, 4)
#define exc_s_exccop            _CMA_STATUS_(2, 4)
#define exc_s_uninitexc         _CMA_STATUS_(3, 4)
#define exc_s_unkstatus		_CMA_STATUS_(128, 4)
#define exc_s_exccoplos		_CMA_STATUS_(129, 4)

/*
 * These should be set to match with underlying system exception codes on
 * platforms where that is appropriate.
 */

# define exc_s_illaddr		STATUS_ACCESS_VIOLATION
# define exc_s_exquota		STATUS_TIMEOUT
# define exc_s_insfmem		STATUS_NO_MEMORY
# define exc_s_nopriv		exc_s_uninitexc
# define exc_s_illinstr		STATUS_ILLEGAL_INSTRUCTION
# define exc_s_resaddr		STATUS_DATATYPE_MISALIGNMENT
# define exc_s_privinst		STATUS_PRIVILEGED_INSTRUCTION
# define exc_s_resoper		STATUS_IN_PAGE_ERROR
# define exc_s_SIGTRAP		STATUS_BREAKPOINT
# define exc_s_SIGIOT		exc_s_uninitexc
# define exc_s_SIGEMT		exc_s_uninitexc   
# define exc_s_aritherr		STATUS_FLOAT_INVALID_OPERATION
# define exc_s_SIGSYS		exc_s_uninitexc
# define exc_s_SIGPIPE		exc_s_uninitexc
# define exc_s_excpu		exc_s_uninitexc
# define exc_s_exfilsiz		exc_s_uninitexc
# define exc_s_intovf		STATUS_INTEGER_OVERFLOW
# define exc_s_intdiv		STATUS_INTEGER_DIVIDE_BY_ZERO
# define exc_s_fltovf		STATUS_FLOAT_OVERFLOW
# define exc_s_fltdiv		STATUS_FLOAT_DIVIDE_BY_ZERO
# define exc_s_fltund		STATUS_FLOAT_UNDERFLOW
# define exc_s_decovf		exc_s_uninitexc
# define exc_s_subrng		STATUS_ARRAY_BOUNDS_EXCEEDED

/*
 * Define alias names
 */

#define exc_s_accvio		exc_s_illaddr
#define exc_s_SIGILL		exc_s_illinstr
#define exc_s_SIGFPE		exc_s_aritherr
#define exc_s_SIGBUS		exc_s_illaddr
#define exc_s_SIGSEGV		exc_s_illaddr
#define exc_s_SIGXCPU		exc_s_excpu
#define exc_s_SIGXFSZ		exc_s_exfilsiz

/*
 * DECthreads facility (CMA) messages
 */

#define cma_s_alerted           _CMA_STATUS_(48, 4)
#define cma_s_assertion         _CMA_STATUS_(49, 4)
#define cma_s_badparam          _CMA_STATUS_(50, 4)
#define cma_s_bugcheck          _CMA_STATUS_(51, 4)
#define cma_s_exit_thread       _CMA_STATUS_(52, 4)
#define cma_s_existence         _CMA_STATUS_(53, 4)
#define cma_s_in_use            _CMA_STATUS_(54, 4)
#define cma_s_use_error         _CMA_STATUS_(55, 4)
#define cma_s_wrongmutex	_CMA_STATUS_(56, 4)
#define cma_s_stackovf          _CMA_STATUS_(57, 4)
#define cma_s_nostackmem        _CMA_STATUS_(58, 4)
#define cma_s_notcmastack       _CMA_STATUS_(59, 4)
#define cma_s_timed_out         _CMA_STATUS_(60, 4)
#define cma_s_unimp             _CMA_STATUS_(61, 4)
#define cma_s_inialrpro         _CMA_STATUS_(62, 4)
#define cma_s_defer_q_full      _CMA_STATUS_(63, 4)
#define cma_s_signal_q_full	_CMA_STATUS_(64, 4)
#define cma_s_alert_nesting	_CMA_STATUS_(65, 4)

/*
 * Synonyms for convenience
 */

#define cma_s_normal		exc_s_normal

/*
 * TYPEDEFS
 */

typedef char *exc_address_t;

/*
 * Constants used to represent whether a try block is currently 
 * active or if it is in the process of handling and exceptions (success).
 */

#ifndef ENUM_EXC_STATE_T
#define ENUM_EXC_STATE_T
typedef enum { 
    exc_try,
    exc_success } exc_state_t;
#endif
/*
 * Constants for the kind of an exception object.
 *
 * There are *currently* only two kinds.  In the address-kind, the identity
 * of an exception object is its address; in the value-kind, the
 * identity of an exception object is an integer, typically, 
 * a system-defined-status-value. These coded kinds also
 * serve as sentinels to help detect uninitialized exceptions.
 */

#ifndef ENUM_EXC_KIND_T
#define ENUM_EXC_KIND_T
typedef enum EXC_KIND_T {
    exc_kind_none_c	= 0,
    exc_kind_address_c	= 0x02130455,  
    exc_kind_status_c	= 0x02130456
    }			exc_kind_t;
#endif

/*
 * Internal contents of an exception object.
 */

typedef long int exc_int_t;

#ifndef STRUCT_EXC_EXCEPTION_T
#define STRUCT_EXC_EXCEPTION_T
typedef struct EXC_EXCEPTION_T	{
    struct EXC_EXCEPTION_T 	*ptr;
    DWORD			value;
    exc_kind_t			kind;
    } EXCEPTION;
#endif

/*
 * Define the exception variables
 *
 * NOTE: it does not make sense to turn all DECthreads status codes into
 * exceptions as some are never raised.  Those are:
 * 
 *	cma_s_normal	-- never signalled
 *	cma_s_exception	-- internal to the implementation of exceptions
 * 	cma_s_exccop	-- internal to the implementation of exceptions
 *	cma_s_timed_out -- returned as status value from timed condition wait
 */

#if defined (__STDC__) || (_CMA_COMPILER_ == _CMA__DECCPLUS) || (_CMA_COMPILER_ == _CMA__MSC)
# define _CMA_EXCNAME(name)	exc_e_##name
# define _CMA_CMANAME(name)	cma_e_##name
#else
# define _CMA_EXCNAME(name)	exc_e_/**/name
# define _CMA_CMANAME(name)	cma_e_/**/name
#endif

#if !defined (_EXC_NO_EXCEPTIONS_) && !defined (_CMA_SUPPRESS_EXTERNALS_)
_CMA_IMPORT_ EXCEPTION
    _CMA_EXCNAME (uninitexc),
    _CMA_EXCNAME (illaddr),
    _CMA_EXCNAME (exquota),
    _CMA_EXCNAME (insfmem),
    _CMA_EXCNAME (nopriv),
    _CMA_EXCNAME (illinstr),
    _CMA_EXCNAME (resaddr),
    _CMA_EXCNAME (privinst),
    _CMA_EXCNAME (resoper),
    _CMA_EXCNAME (SIGTRAP),
    _CMA_EXCNAME (SIGIOT),
    _CMA_EXCNAME (SIGEMT),
    _CMA_EXCNAME (aritherr),
    _CMA_EXCNAME (SIGSYS),
    _CMA_EXCNAME (SIGPIPE),
    _CMA_EXCNAME (excpu),
    _CMA_EXCNAME (exfilsiz),
    _CMA_EXCNAME (intovf),
    _CMA_EXCNAME (intdiv),
    _CMA_EXCNAME (fltovf),
    _CMA_EXCNAME (fltdiv),
    _CMA_EXCNAME (fltund),
    _CMA_EXCNAME (decovf),
    _CMA_EXCNAME (subrng),
    _CMA_CMANAME (alerted),
    _CMA_CMANAME (assertion),
    _CMA_CMANAME (badparam),
    _CMA_CMANAME (bugcheck),
    _CMA_CMANAME (exit_thread),
    _CMA_CMANAME (existence),
    _CMA_CMANAME (in_use),
    _CMA_CMANAME (use_error),
    _CMA_CMANAME (wrongmutex),
    _CMA_CMANAME (stackovf),
    _CMA_CMANAME (nostackmem),
    _CMA_CMANAME (notcmastack),
    _CMA_CMANAME (unimp),
    _CMA_CMANAME (inialrpro),
    _CMA_CMANAME (defer_q_full),
    _CMA_CMANAME (signal_q_full),
    _CMA_CMANAME (alert_nesting);

/*
 * Define aliased exceptions
 */

# define exc_e_accvio		_CMA_EXCNAME (illaddr)
# define exc_e_SIGILL		_CMA_EXCNAME (illinstr)
# define exc_e_SIGFPE		_CMA_EXCNAME (aritherr)
# define exc_e_SIGBUS		_CMA_EXCNAME (illaddr)
# define exc_e_SIGSEGV		_CMA_EXCNAME (illaddr)
# define exc_e_SIGXCPU		_CMA_EXCNAME (excpu)
# define exc_e_SIGXFSZ		_CMA_EXCNAME (exfilsiz)

/*
 * The following are pthread exception names.
 */

# define exc_uninitexc_e	_CMA_EXCNAME (uninitexc)
# define exc_illaddr_e		_CMA_EXCNAME (illaddr)
# define exc_exquota_e		_CMA_EXCNAME (exquota)
# define exc_insfmem_e		_CMA_EXCNAME (insfmem)
# define exc_nopriv_e		_CMA_EXCNAME (nopriv)
# define exc_illinstr_e		_CMA_EXCNAME (illinstr)
# define exc_resaddr_e		_CMA_EXCNAME (resaddr)
# define exc_privinst_e		_CMA_EXCNAME (privinst)
# define exc_resoper_e		_CMA_EXCNAME (resoper)
# define exc_SIGTRAP_e		_CMA_EXCNAME (SIGTRAP)
# define exc_SIGIOT_e		_CMA_EXCNAME (SIGIOT)
# define exc_SIGEMT_e		_CMA_EXCNAME (SIGEMT)
# define exc_aritherr_e		_CMA_EXCNAME (aritherr)
# define exc_SIGSYS_e		_CMA_EXCNAME (SIGSYS)
# define exc_SIGPIPE_e		_CMA_EXCNAME (SIGPIPE)
# define exc_excpu_e		_CMA_EXCNAME (excpu)
# define exc_exfilsiz_e		_CMA_EXCNAME (exfilsiz)
# define exc_intovf_e		_CMA_EXCNAME (intovf)
# define exc_intdiv_e		_CMA_EXCNAME (intdiv)
# define exc_fltovf_e		_CMA_EXCNAME (fltovf)
# define exc_fltdiv_e		_CMA_EXCNAME (fltdiv)
# define exc_fltund_e		_CMA_EXCNAME (fltund)
# define exc_decovf_e		_CMA_EXCNAME (decovf)
# define exc_subrng_e		_CMA_EXCNAME (subrng)

# define pthread_cancel_e	_CMA_CMANAME (alerted)
# define pthread_assertion_e	_CMA_CMANAME (assertion)
# define pthread_badparam_e	_CMA_CMANAME (badparam)
# define pthread_bugcheck_e	_CMA_CMANAME (bugcheck)
# define pthread_exit_thread_e	_CMA_CMANAME (exit_thread)
# define pthread_existence_e	_CMA_CMANAME (existence)
# define pthread_in_use_e	_CMA_CMANAME (in_use)
# define pthread_use_error_e	_CMA_CMANAME (use_error)
# define pthread_wrongmutex_e	_CMA_CMANAME (wrongmutex)
# define pthread_stackovf_e	_CMA_CMANAME (stackovf)
# define pthread_nostackmem_e	_CMA_CMANAME (nostackmem)
# define pthread_notstack_e	_CMA_CMANAME (notcmastack)
# define pthread_unimp_e	_CMA_CMANAME (unimp)
# define pthread_inialrpro_e	_CMA_CMANAME (inialrpro)
# define pthread_defer_q_full_e	_CMA_CMANAME (defer_q_full)
# define pthread_signal_q_full_e _CMA_CMANAME (signal_q_full)

# define exc_accvio_e		_CMA_EXCNAME (accvio)
# define exc_SIGILL_e		_CMA_EXCNAME (SIGILL)
# define exc_SIGFPE_e		_CMA_EXCNAME (SIGFPE)
# define exc_SIGBUS_e		_CMA_EXCNAME (SIGBUS)
# define exc_SIGSEGV_e		_CMA_EXCNAME (SIGSEGV)
# define exc_SIGXCPU_e		_CMA_EXCNAME (SIGXCPU)
# define exc_SIGXFSZ_e		_CMA_EXCNAME (SIGXFSZ)
#endif

/*
 * Synonyms for convenience or unmatched statuses
 */

#define EXC_S_UNINITEXC 	exc_s_uninitexc
#define exc_s_normal		1
#define cma_s_normal		exc_s_normal

#define EXCEPTION_INIT(_e_) ( \
	(_e_).ptr = &(_e_), \
	(_e_).kind = exc_kind_address_c, \
	(_e_).value = (DWORD)&(_e_))

#define exc_set_status(_e_,_s_) ( \
	(_e_)->kind = exc_kind_status_c, \
	(_e_)->value = (_s_))

#define exc_get_status(_e_,_s_) ( \
	(_e_)->ptr->kind == exc_kind_status_c ? \
		(*(_s_) = (_e_)->ptr->value, 0) : \
		-1)

#define exc_matches(_e1_,_e2_) \
	((_e1_)->ptr->kind == (_e2_)->ptr->kind \
	&& (_e1_)->ptr->value == (_e2_)->ptr->value)

extern void exc_report _CMA_PROTOTYPE_ ((
	EXCEPTION *exc));

extern void exc_get_message _CMA_PROTOTYPE_ ((	/* Get exception text */
	EXCEPTION	*exc,
	int		buflen,
	char		*buffer));

#define RAISE(_exc_) \
    if ((_exc_).ptr->kind == (DWORD)exc_kind_address_c) \
	RaiseException((DWORD)exc_s_exception, \
		0, \
		1, \
		(const unsigned long *)(_exc_).ptr->value); \
    else if ((_exc_).ptr->kind == (DWORD)exc_kind_status_c) \
	RaiseException((DWORD)(_exc_).ptr->value, 0, 0, 0); \
    else RaiseException((DWORD)exc_s_uninitexc, 0, 0, 0)

# define TRY \
    { \
    _CMA_VOLATILE_ DWORD __exc_val__ = 0; \
    _CMA_VOLATILE_ DWORD __exc_arg__; \
    _CMA_VOLATILE_ exc_state_t __exc_state__ = exc_try; \
    struct _EXCEPTION_POINTERS *__exc_info__; \
    EXCEPTION __exc_struct__ = {&__exc_struct__, 0, exc_kind_status_c}; \
    do { \
    _try { \
	if (__exc_val__ == 0) {

# define CATCH(_x_) \
    } \
    if ((__exc_val__ != 0) && (exc_matches((&__exc_struct__), (&(_x_))))) { \
    EXCEPTION *THIS_CATCH = &__exc_struct__; \
    __exc_val__ = 0;

# define CATCH_ALL \
    } \
    if (__exc_val__ != 0) { \
    EXCEPTION *THIS_CATCH = &__exc_struct__; \
    __exc_val__ = 0;

# define FINALLY \
    } { \
    EXCEPTION *THIS_CATCH = &__exc_struct__; \
    __exc_state__ = exc_success;

# define ENDTRY \
    } if (__exc_val__ != 0) { \
    	if (__exc_struct__.kind == (DWORD)exc_kind_address_c) \
		RaiseException((DWORD)exc_s_exception, \
			0, \
			1, \
			(const unsigned long *)__exc_struct__.value); \
    	else \
		RaiseException((DWORD)__exc_val__, 0, 0, 0); \
      } \
    } _except (1 \
		&& (__exc_state__ == exc_try) \
		&& ((__exc_info__ = GetExceptionInformation ()) != NULL) \
		&& ((__exc_val__ = __exc_info__->ExceptionRecord->ExceptionCode) != 0) \
		&& ((__exc_arg__ = ((__exc_info__->ExceptionRecord->NumberParameters > 0) ? \
			*(DWORD *)__exc_info__->ExceptionRecord->ExceptionInformation : 0)) != -1) \
		) { \
    __exc_state__ = exc_success; \
    if (__exc_val__ != exc_s_exception) \
	__exc_struct__.value = __exc_val__; \
    else { \
	__exc_struct__.kind = exc_kind_address_c; \
	__exc_struct__.value = (DWORD)__exc_arg__; \
	} \
    } \
    } while (__exc_val__ != 0); \
    }

#define RERAISE         exc_raise(THIS_CATCH)


#define exc_raise(_exc_) \
	if (((_exc_)->ptr->kind != exc_kind_address_c) \
		&& ((_exc_)->ptr->kind != exc_kind_status_c)) \
			  RAISE(exc_e_uninitexc); \
	else RAISE(*_exc_)

#define exc_raise_status(_status_)  RaiseException((DWORD)(_status_), \
		0, \
		0, \
		0)

#ifdef __cplusplus
    }
#endif

#endif
