/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_crtlx.c,v $
 * Revision 1.1.10.1  1996/10/03  15:17:45  arvind
 * 	Add "bomb point" in malloc for use in code coverage testing.
 * 	[1996/09/17  00:32 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.8.2  1996/02/17  23:33:45  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:57  marty]
 * 
 * Revision 1.1.8.1  1995/12/07  22:00:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:05:02  root]
 * 
 * Revision 1.1.4.2  1994/06/09  13:37:35  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:33  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:15:46  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:16:26  jd]
 * 
 * Revision 1.1.2.2  1992/12/15  22:26:46  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:36:59  alan]
 * 
 * Revision 1.1  1992/01/19  14:40:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991 by
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
 *	Concert Multithread Architecture (CMA) Library services
 *
 *  ABSTRACT:
 *
 *	C rtl wrapper routines.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	25 March 1990
 *
 *  MODIFICATION HISTORY:
 *	001	Paul Curtin	29 August 1990
 *		Removed from CMA (cma_vm.c) and placed in CMALIB.
 *	002	Paul Curtin	11 October 1990
 *		Added a call to cma_init() in all wrapper routines.
 *	003	Paul Curtin	24 January 1990
 *		Changed wrapper routine defs to match `man' pages. (return
 * 		values)
 *	004	Dave Butenhof	19 February 1991
 *		Call "free" rather than "cfree" to implement cma_lib_cfree
 *		function. Most platforms don't appear to even document
 *		"cfree", and all appear to allow use of "free" instead.
 *		Alternate allocation packages such as libmalloc.a don't
 *		support cfree at all.
 *	005	Dave Butenhof	25 February 1991
 *		Remove include of cma_library.h
 *	006	Dave Butenhof	30 April 1991
 *		Switch to internal lock calls.
 *	007	Dave Butenhof	17 September 1991
 *		Simplify Makefile by just not compiling the code if
 *		_CMA_UNIPROCESSOR_ isn't set.
 *	008	Dave Butenhof	04 October 1991
 *		Change conditional compilation from _CMA_UNIPROCESSOR_ to
 *		_CMA_REENTRANT_CLIB_.
 */

/*
 *  INCLUDE FILES
 */

#define CMA_CRTLX_IMPLEMENTATION

#include <cma.h>

#if !_CMA_REENTRANT_CLIB_		/* Don't compile unless needed! */

#include <cma_mutex.h>
#if _CMA_OS_ == _CMA__VMS
#include <stdlib.h>                     /* Define UNIX VM prototypes */
#else
#include <sys/types.h>
#endif


/*
 *  LOCAL DATA
 */

static malloc_bomb_after = -1;

#define MAYBE_BOMB() {if (malloc_bomb_after > 0) {if (!(--malloc_bomb_after)) return cma_lib_malloc_die();}}

/*
 *  GLOBAL DATA
 */

/*
 * LOCAL FUNCTIONS
 */

cma_lib_malloc_bomb(int blow_in) 
{
    malloc_bomb_after = blow_in;
}

char *cma_lib_malloc_die(void) 
{
    return NULL;
}


/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Allocate "array" virtual memory for client
 *
 *  FORMAL PARAMETERS:
 *
 *	number	The number of items to be allocated
 * 
 *	size	The size (in bytes) to be allocated
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Address of the memory allocated (or NULL)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
STDLIB_MEM_PTR_T *
cma_lib_calloc(
	STDLIB_MEM_SIZE_T	number,	/* Number of items */
	STDLIB_MEM_SIZE_T	size)	/* Size of VM to allocate, bytes */
    {
    STDLIB_MEM_PTR_T	*block;

    cma_init ();    
    cma__int_lock (cma__g_global_lock);
    MAYBE_BOMB();
    block = calloc (number, size);
    cma__int_unlock (cma__g_global_lock);
    return block;
    }



/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Free virtual memory for client.
 *
 *  FORMAL PARAMETERS:
 *
 *	vm	The address of the memory to be freed
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
void
cma_lib_cfree(STDLIB_MEM_PTR_T	*vm)	/* Address of VM to free */
    {
    cma_init ();    
    cma__int_lock (cma__g_global_lock);

    /*
     * Most platforms support use of "free" to deallocate storage allocated
     * with "calloc", and some do not support the "cfree" function (for
     * example, the libmalloc.a allocation package on BSD systems).
     * Therefore, recognizing the risk that this won't work on some platform,
     * we've chosen to use "free".  On a hypothetical platform which actually
     * requires use of "cfree" for "calloc"d memory, the client will need to
     * write a custom wrapper function using the global mutex (this isn't
     * difficult).
     */
    free (vm);
    cma__int_unlock (cma__g_global_lock);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Free virtual memory for client.
 *
 *  FORMAL PARAMETERS:
 *
 *	vm	The address of the memory to be freed
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
void
cma_lib_free(STDLIB_MEM_PTR_T	*vm)	/* Address of VM to free */
    {
    cma_init ();    
    cma__int_lock (cma__g_global_lock);
    free (vm);
    cma__int_unlock (cma__g_global_lock);
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Allocate virtual memory for client
 *
 *  FORMAL PARAMETERS:
 *
 *	size	The size (in bytes) to be allocated
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Address of the memory allocated (or NULL)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
STDLIB_MEM_PTR_T *
cma_lib_malloc(STDLIB_MEM_SIZE_T	size)	/* Size of VM to allocate, bytes */
    {
    STDLIB_MEM_PTR_T	*block;

    cma_init ();    
    cma__int_lock (cma__g_global_lock);
    MAYBE_BOMB();
    block = malloc (size);
    cma__int_unlock (cma__g_global_lock);
    return block;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Re-allocate virtual memory for client
 *
 *  FORMAL PARAMETERS:
 *
 *	ptr	Pointer to an allocated area.
 * 
 *	size	The size (in bytes) to be allocated
 *
 *  IMPLICIT INPUTS:
 *
 *	none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	Address of the memory allocated (or NULL)
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
STDLIB_MEM_PTR_T *
cma_lib_realloc(
	STDLIB_MEM_PTR_T	*ptr,	/* Pointer to memory */
	STDLIB_MEM_SIZE_T	size)	/* Size of VM to allocate, bytes */
    {
    STDLIB_MEM_PTR_T	*block;

    cma_init ();    
    cma__int_lock (cma__g_global_lock);
    MAYBE_BOMB();    
    block = realloc (ptr, size);
    cma__int_unlock (cma__g_global_lock);
    return block;
    }

#endif					/* if !_CMA_REENTRANT_CLIB_ */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMALIB_CRTLX.C */
/*  *4    25-JAN-1991 10:19:33 CURTIN "Change wrapper routine definitions" */
/*  *3    11-OCT-1990 10:38:49 CURTIN "Added a call to cma_init() in all wrapper routines" */
/*  *2    17-SEP-1990 18:04:52 SCALES "Convert to stream-lf format" */
/*  *1    17-SEP-1990 14:50:09 CURTIN "Original checkin" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMALIB_CRTLX.C */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_CRTLX.C */
/*  *9    14-OCT-1991 13:37:56 BUTENHOF "Use new config symbols" */
/*  *8    17-SEP-1991 13:21:07 BUTENHOF "Simplify Makefile by conditionalizing" */
/*  *7    10-JUN-1991 18:18:27 SCALES "Add sccs headers for Ultrix" */
/*  *6     2-MAY-1991 13:57:40 BUTENHOF "Use internal lock functions" */
/*  *5    25-FEB-1991 19:04:55 BUTENHOF "Remove include of cma_library.h" */
/*  *4    19-FEB-1991 17:55:46 BUTENHOF "Don't use cfree (which isn't universally supported)" */
/*  *3    28-JAN-1991 10:47:04 CURTIN "Keeping up w/cmalib_crtlx.c" */
/*  *2    15-JAN-1991 21:58:07 BUTENHOF "Remove dependency on cma_library.h" */
/*  *1     2-JAN-1991 15:32:11 BUTENHOF "HACK: interlocked malloc/free until CMALIB" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_CRTLX.C */
