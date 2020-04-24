/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_sequence.c,v $
 * Revision 1.1.6.2  1996/02/17  23:34:15  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:24  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:05:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:06:54  root]
 * 
 * Revision 1.1.4.2  1994/06/09  13:38:43  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:28:29  devsrc]
 * 
 * Revision 1.1.4.1  1994/04/01  20:17:10  jd
 * 	Fist drop of code cleanup
 * 	[1994/03/28  19:18:37  jd]
 * 
 * Revision 1.1.2.2  1992/12/15  22:30:21  alan
 * 	Insert copyright notices
 * 	[1992/12/07  19:41:08  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 *	Concert Multithread Architecture (CMA) services
 *
 *  ABSTRACT:
 *
 *	Routine to generate unique sequence numbers for CMA objects
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	24 July 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Dave Butenhof	25 August 1989
 *		Add initialization code for convenience.
 *	002	Dave Butenhof	31 August 1989
 *		Initialize sequence to 1, not 0.
 *	003	Dave Butenhof	11 October 1989
 *		Convert to use internal mutex operations.
 */

/*
 *  INCLUDE FILES
 */

#include <cma.h>
#include <cma_defs.h>
#include <cma_sequence.h>
#include <cma_mutex.h>

/*
 *  LOCAL DATA
 */

/*
 *  LOCAL FUNCTIONS
 */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Assign a unique sequence number for some type of object
 *
 *  FORMAL PARAMETERS:
 *
 *	control		Sequence control structure (cma_t_sequence)
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
 *	The sequence number of the new object
 *
 *  SIDE EFFECTS:
 *
 *	Increments number in sequence control structure
 */
extern cma_t_natural
cma__assign_sequence(
	cma__t_sequence	*control)	/* Sequence control block */
    {
    cma_t_natural	seq;		/* Local copy of next sequence */


    cma__int_lock (control->mutex);
    seq = control->seq++;
    cma__int_unlock (control->mutex);
    return seq;
    }

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Initialize a sequence generator control structure
 *
 *  FORMAL PARAMETERS:
 *
 *	control		Sequence control structure (cma_t_sequence)
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
cma__init_sequence(
	cma__t_sequence	*control)	/* Sequence control block */
    {
    control->mutex = cma__get_mutex (&cma__g_def_attr);
    control->seq = 1;
    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_SEQUENCE.C */
/*  *2    10-JUN-1991 18:23:23 SCALES "Add sccs headers for Ultrix" */
/*  *1    12-DEC-1990 21:52:39 BUTENHOF "Atomic sequence numbers" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_SEQUENCE.C */
