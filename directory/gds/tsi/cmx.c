/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmx.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:01  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:31  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:46:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:53  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:21  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:21:10  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:16:22  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:23  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:02  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:50:07  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmx.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:01 $";
#endif

/*
 * COPYRIGHT (C) 1987 BY SIEMENS AG All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1993
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/****************************************************************************/
/*                                                                          */
/* ICMX(L) - INTERFACE FOR STATION COUPLING OF SINIX-PC's AS OF CMX V2.0    */
/* ======================================================================== */
/*                                                                          */
/*   Comments:                                                              */
/*                                                                          */
/*   The station coupling for STA 2 is not supported for CMX Version 3.0    */
/*   for MX300I. A dummy module is therefore provided here. T_ERROR is      */
/*   returned with _t_error T_WREQUEST with all library calls.              */
/*                                                                          */
/*   Author: R. Horn                                        Date:  1.7.85   */
/*   Ammendments:                                                           */
/*   11.04.89 V2.5 rit                                                      */
/*   Dummy module:                                                          */
/*   15.02.90 V3.0 bee							    */
/****************************************************************************/

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include "cmx.h"

/*
static char sccsid[] = "@(#)cmx.c	311.3 93/08/24 CMX (NC1)";
*/

/*
 * Dummy module.
 * All functions return T_ERROR with T_WREQUEST.
 */

extern int _t_error;
/*
 * Prototyping
 */
int	t_200attach(struct t_myname *name, t_opta *opt);
int	t_201detach(struct t_myname *name);
int	t_202conrq(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt);
int	t_203conin(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt);
int	t_204conrs(int *tref, t_opt13 *opt);
int	t_205concf(int *tref, t_opt1 *opt);
int	t_206datarq(int *tref, char *datap, int *datal, int *chain);
int	t_207datain(int *tref, char *datap, int *datal, int *chain);
int	t_208datago(int *tref);
int	t_209datastop(int *tref);
int	t_210redrq(int *tref, int *pid, t_opt12 *opt);
int	t_211redin(int *tref, int *pid, t_opt23 *opt);
int	t_212xdatrq(int *tref, char *datap, int *datal);
int	t_213xdatin(int *tref, char *datap, int *datal);
int	t_214xdatgo(int *tref);
int	t_215xdatstop(int *tref);
int	t_216disrq(int *tref, t_opt2 *opt);
int	t_217disin(int *tref, int *reason, t_opt2 *opt);
int	t_218event(int *tref, int cmode, t_opte *opt);
int	t_219error(void);
int	t_220info(int *tref, t_opti *opt);
int	t_221vdatarq(int *tref, struct t_data *vdata, int *vcnt, int *chain);
int	t_222vdatain(int *tref, struct t_data *vdata, int *vcnt, int *chain);


int	t_200attach(struct t_myname *name, t_opta *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_201detach(struct t_myname *name)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_202conrq(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_203conin(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_204conrs(int *tref, t_opt13 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_205concf(int *tref, t_opt1 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_206datarq(int *tref, char *datap, int *datal, int *chain)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_207datain(int *tref, char *datap, int *datal, int *chain)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_208datago(int *tref)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_209datastop(int *tref)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_210redrq(int *tref, int *pid, t_opt12 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_211redin(int *tref, int *pid, t_opt23 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_212xdatrq(int *tref, char *datap, int *datal)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_213xdatin(int *tref, char *datap, int *datal)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_214xdatgo(int *tref)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_215xdatstop(int *tref)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_216disrq(int *tref, t_opt2 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_217disin(int *tref, int *reason, t_opt2 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_218event(int *tref, int cmode, t_opte *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_219error(void)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_220info(int *tref, t_opti *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_221vdatarq(int *tref, struct t_data *vdata, int *vcnt, int *chain)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_222vdatain(int *tref, struct t_data *vdata, int *vcnt, int *chain)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}
