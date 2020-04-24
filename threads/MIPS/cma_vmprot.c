/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_vmprot.c,v $
 * Revision 1.1.6.2  1996/02/17  23:33:00  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:18  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:49:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:00:17  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:35:10  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:54  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:22:33  alan
 * 	Insert copyright notices
 * 	[1992/12/07  16:01:10  alan]
 * 
 * Revision 1.1  1992/01/19  14:38:34  devrcs
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
 *	CMA services
 *
 *  ABSTRACT:
 *
 *	System-dependent services for changing memory protection: Ultrix
 *
 *  AUTHORS:
 *
 *	Paul Curtin
 *
 *  CREATION DATE:
 *
 *	21 June 1990
 *
 *  MODIFICATION HISTORY:
 *	001	Paul Curtin	05 July 1990
 *		Added 1 to guard page size calculation.
 *	002	Paul Curtin	06 July 1990
 *		Added check on returned size in set_writable
 *	003	Dave Butenhof	03 April 1991
 *		Add support for OSF/1 (mprotect() returns 0 on success, not
 *		size of altered region).
 *	004	Dave Butenhof	08 October 1991
 *		Add typecasts to make MIPS C happy.
 *	005	Dave Butenhof	28 October 1991
 *		Add more casts.
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_vmprot.h>
#include <sys/mman.h>
#include <sys/types.h>

/*
 * LOCAL DATA
 */

/*
 * LOCAL FUNCTIONS
 */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Make a region of memory inaccessible (mark a range of pages as "no
 * 	access")
 *
 *	The mprotect system call changes the protection of portions
 *	of an application program's data memory.  Protection is per-
 *	formed on page cluster boundaries.  The default protection
 *	for data memory on process invocation is user READ/WRITE.
 *	The addr argument is the beginning address of the data
 *	block, and must fall on a page cluster boundary.
 *
 *	Chunk, stack, yellow zone, and guard page sizes must be rounded so
 *	that this granularity is acceptable (this is why the architecture
 *	defines the stack and guard sizes to be minimums).
 *
 *  FORMAL PARAMETERS:
 *
 *      low_address     Lowest address to modify
 *
 *      high_address    Highest address to modify
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
extern void
cma__set_noaccess
#ifdef _CMA_PROTO_
	(
        cma_t_address   low_address,    /* Lowest address to change */
        cma_t_address   high_address)   /* Highest address to change */
#else	/* no prototypes */
	(low_address, high_address)
        cma_t_address   low_address;    /* Lowest address to change */
        cma_t_address   high_address;   /* Highest address to change */
#endif	/* prototype */
    {
    cma_t_integer	status;
    cma_t_integer	length;
    cma_t_integer	protection;

    length = (cma_t_integer)
	((unsigned long)high_address - (unsigned long)low_address + 1);
    protection = 0;

    status = mprotect (low_address, length, protection);

    if (status == -1)
	cma__bugcheck ("set_noaccess:1");

#if _CMA_OSIMPL_ != _CMA__OS_OSF
    if (status != length)
	cma__bugcheck ("set_noaccess:2");
#endif
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Make a region of memory accessible (mark a range of pages as "user
 * 	write")
 *
 *	The mprotect system call changes the protection of portions
 *	of an application program's data memory.  Protection is per-
 *	formed on page cluster boundaries.  The default protection
 *	for data memory on process invocation is user READ/WRITE.
 *	The addr argument is the beginning address of the data
 *	block, and must fall on a page cluster boundary.
 *
 *	Chunk, stack, yellow zone, and guard page sizes must be rounded so
 *	that this granularity is acceptable (this is why the architecture
 *	defines the stack and guard sizes to be minimums).
 *
 *  FORMAL PARAMETERS:
 *
 *	begin_address	Beginning address of area to modify
 *
 *	length		Length of area to be modified
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
extern void
cma__set_writable
#ifdef _CMA_PROTO_
	(
        cma_t_address   low_address,    /* Lowest address to change */
        cma_t_address   high_address)   /* Highest address to change */
#else	/* no prototypes */
	(low_address, high_address)
        cma_t_address   low_address;    /* Lowest address to change */
        cma_t_address   high_address;   /* Highest address to change */
#endif	/* prototype */
    {
    cma_t_integer	status;
    cma_t_integer	length;
    cma_t_integer	protection;


    length = (cma_t_integer)
	((unsigned long)high_address - (unsigned long)low_address + 1);
    protection = 0x7;

    status = mprotect (low_address, length, protection);

    if (status == -1)
	cma__bugcheck ("set_writable:1");

#if _CMA_OSIMPL_ != _CMA__OS_OSF
    if (status != length)
	cma__bugcheck ("set_writable:2");
#endif
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_VMPROT.C */
/*  *7    29-OCT-1991 12:13:19 BUTENHOF "Add some casts" */
/*  *6    14-OCT-1991 13:34:16 BUTENHOF "Fix compilation errors" */
/*  *5    10-JUN-1991 18:09:22 SCALES "Add sccs headers for Ultrix" */
/*  *4     2-MAY-1991 14:01:44 BUTENHOF "Adjust OSF conditionals for Tin BL3" */
/*  *3    12-APR-1991 23:33:58 BUTENHOF "Modify to support OSF/1" */
/*  *2    12-DEC-1990 20:33:42 BUTENHOF "Fix assem include, and clean up CMS history" */
/*  *1    12-DEC-1990 19:07:24 BUTENHOF "ULTRIX vmprot code" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_VMPROT.C */
