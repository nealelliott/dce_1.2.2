/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: cma_int.c,v $
 * Revision 1.1.37.2  1996/02/17  23:33:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:39  marty]
 *
 * Revision 1.1.37.1  1995/12/07  21:56:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:24  root]
 * 
 * Revision 1.1.35.2  1994/06/09  13:36:56  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:02  devsrc]
 * 
 * Revision 1.1.35.1  1994/01/04  19:41:42  ohara
 * 	generate an error if this files gets compiled
 * 	[1994/01/04  18:06:03  ohara]
 * 
 * 	ported to PYRMIPS
 * 
 * Revision 1.1.2.2  1993/06/24  20:51:15  hinman
 * 	[hinman] - Save merged-in SNI version (these files have been freed of nasty code)
 * 	[1993/06/17  14:27:08  hinman]
 * 
 * 	created to support some stdio functions internal to CMA
 * 
 * Revision 1.1.2.3  93/04/15  17:59:35  maher
 * 	ported to PYRMIPS
 * 
 * Revision 1.1.2.2  93/04/15  10:03:29  maher
 * 	initial submission
 * 
 * $EndLog$
 */
#ifndef lint
static char	*sccsid = "%W%	(DEC OSF/1)	%G%";
#endif 
/*
 *  Copyright (c) 1989, 1992 by
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
 *  FILENAME:
 *
 *	CMA_INT.C
 *
 *  ABSTRACT:
 *
 *	This module implements some stdio functions needed by CMA. 
 *
 *  AUTHORS:
 *
 *	Maher Samman
 *
 *  CREATION DATE:
 *
 *	9 April 1993
 *
 *  MODIFICATION HISTORY:
 *
 */


/*
 *  INCLUDE FILES
 */

#ifdef SNI_SVR4_TAINTED
   !!!Error - Missing tainted code
#endif /* SNI_SVR4_TAINTED */

/*
 * LOCAL MACROS
 */

/*
 * LOCAL DATA
 */


#ifdef SNI_SVR4_TAINTED
#endif /* SNI_SVR4_TAINTED */

