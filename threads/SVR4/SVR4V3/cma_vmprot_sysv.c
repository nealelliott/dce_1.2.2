/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
vvvvvvvvvvvvvvvvvvvv
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
^^^^^^^^^^^^^^^^^^^^
 */
/*
 * HISTORY
 * $Log: cma_vmprot_sysv.c,v $
 * Revision 1.1.6.2  1996/02/17  23:33:25  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:38  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:56:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:20  root]
 * 
 * Revision 1.1.4.1  1994/06/10  20:52:43  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:49:41  devsrc]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:39:55  sekhar]
 * 
 * Revision 1.1.2.2  1993/02/01  20:14:50  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  20:03:22  hinman]
 * 
 * 	[hinman] - Check in SNI revision 9.6.1.4
 * 	[1993/01/08  14:09:11  hinman]
 * 
 * Revision 9.6.1.4  92/10/01  14:08:34  root
 * 	Acceptance of OSF rev 1.1.3.2
 * 
 * Revision 9.12.1.2  92/09/30  10:31:27  hinman
 * 	Auto checkin of OSF rev 1.1.3.2
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
 * ALL RIGHTS RESERVED
*/
/*
 *  Copyright (c) 1989, 1990 by
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
 *  001	    Paul Curtin	    5 July 1990
 *	    Added 1 to guard page size calculation.
 *  002	    Paul Curtin	    6 July 1990
 *	    Added check on returned size in set_writable
 *
 */

/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <cma_defs.h>
#include <cma_vmprot.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stddef.h>

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
    ptrdiff_t		length;
    cma_t_integer	protection;

    length = ((cma_t_integer) high_address - (cma_t_integer) low_address) + 1;
    protection = 0;

    status =  mprotect ( low_address, length, protection );

#if _CMA_UNIX_TYPE == _CMA__SVR4
	if ( status != 0 )
#else
    if ((status == -1) || (status != length))
#endif /* _CMA_UNIX_TYPE == _CMA__SVR4 */
	cma__bugcheck();  /* RAISE AN ERROR */

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
    ptrdiff_t		length;
    cma_t_integer	protection;


    length = ((cma_t_integer) high_address - (cma_t_integer) low_address) + 1;
    protection = 0x7;

    status =  mprotect ( low_address, length, protection );

#if _CMA_UNIX_TYPE == _CMA__SVR4
	if ( status != 0 )
#else
    if ((status == -1) || (status != length))
#endif /* _CMA_UNIX_TYPE == _CMA__SVR4 */
	cma__bugcheck();		/* RAISE AN ERROR */

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_VMPROT_VMS.C */
/*  *3     9-FEB-1990 11:13:26 BUTENHOF "CMA.h is now lowercase" */
/*  *2    23-JAN-1990 14:10:17 BUTENHOF "Make function prototypes conditional" */
/*  *1    24-OCT-1989 11:15:39 BUTENHOF "VM protection for guard pages" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_VMPROT_VMS.C */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_VMPROT_ULTRIX.C */
/*  *4     6-JUL-1990 11:18:19 CURTIN "fixed up cma_vmprot_ultrix.c" */
/*  *3     5-JUL-1990 10:36:24 CURTIN "adjusted stack guard size calculation by 1" */
/*  *2     3-JUL-1990 07:39:31 BUTENHOF "Change copyright header" */
/*  *1    27-JUN-1990 18:22:05 CURTIN "mprotect protection for guard pages" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_VMPROT_ULTRIX.C */
