/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: example.h,v $
 * Revision 1.1.10.2  1996/02/18  23:03:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:41  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:12:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:21  root]
 * 
 * Revision 1.1.8.2  1994/10/05  16:44:21  keutel
 * 	OT 12431
 * 	[1994/10/05  16:36:47  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:40:09  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:34:19  marrek]
 * 
 * Revision 1.1.6.2  1993/08/12  12:50:37  marrek
 * 	July 1993 code drop.
 * 	[1993/08/04  11:00:23  marrek]
 * 
 * Revision 1.1.4.2  1993/01/05  20:00:32  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:17:53  htf]
 * 
 * Revision 1.1.2.2  1992/06/11  20:00:06  melman
 * 	 gds drop
 * 	[1992/06/11  19:22:56  melman]
 * 
 * $EndLog$
 */
#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char example_rcsid[] = "@(#)$RCSfile: example.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:03:59 $";
#endif

/*
 * define some convenient exit codes
 */

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0


/*
 * declare an error handling function and
 * an error checking macro for DS
 */

void handle_ds_error(DS_status error);

#define CHECK_DS_CALL(function_call)		\
		error = (function_call);	\
		if (error != DS_SUCCESS)	\
			handle_ds_error(error);


/*
 * declare an error handling function and
 * an error checking macro for OM
 */

void handle_om_error(OM_return_code return_code);

#define CHECK_OM_CALL(function_call)		\
		return_code = (function_call);	\
		if (return_code != OM_SUCCESS)	\
			handle_om_error(return_code);

/*
 * the error handling code
 *
 * NOTE: any errors arising in these functions are ignored.
 */

void handle_ds_error(DS_status error)
{
	(void) fprintf(stderr, "DS error has occurred\n");

	(void) om_delete((OM_object) error);

	/* At this point, the error has been reported and storage cleaned
	 * up, so the handler could return to the main program now for it 
	 * to take recovery action.  But we choose the simple option ...
	 */

	exit(EXIT_FAILURE);
}


void handle_om_error(OM_return_code return_code)
{
	(void) fprintf(stderr, "OM error %d has occurred\n", return_code);

	/* At this point, the error has been reported and storage cleaned up,
	 * so the handler could return to the main program now for it to take
	 * recovery action.  But we choose the simple option ...
	 */

	exit(EXIT_FAILURE);
}

#endif /* _EXAMPLE_H */
