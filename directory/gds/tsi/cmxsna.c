/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxsna.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:27  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:50  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:48:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:33  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:37  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:22:23  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:34  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:47  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:32  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:27  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxsna.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:27 $";
#endif

/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1990
 *               All Rights Reserved
 *
 * This is a dummy module until the real module is available.
 * All the functions return T_OK or T_NOEVENT
 */
#ifdef THREADSAFE
# include <pthread.h>
#endif

#include "cmx.h"
/*
static char sccsid[] = "@(#)cmxsna.c	311.3 93/08/24 CMX (NC1)";
*/

extern int _t_error;

/*
 * Prototyping
 */
int	t_300attach(struct t_myname *name, t_opta *opt);
int	t_301detach(struct t_myname *name);
int	t_302conrq(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt);
int	t_303conin(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt);
int	t_304conrs(int *tref, t_opt13 *opt);
int	t_305concf(int *tref, t_opt1 *opt);
int	t_306datarq(int *tref, char *datap, int *datal, int *chain);
int	t_307datain(int *tref, char *datap, int *datal, int *chain);
int	t_308datago(int *tref);
int	t_309datastop(int *tref);
int	t_310redrq(int *tref, int *pid, t_opt12 *opt);
int	t_311redin(int *tref, int *pid, t_opt23 *opt);
int	t_312xdatrq(int *tref, char *datap, int *datal);
int	t_313xdatin(int *tref, char *datap, int *datal);
int	t_314xdatgo(int *tref);
int	t_315xdatstop(int *tref);
int	t_316disrq(int *tref, t_opt2 *opt);
int	t_317disin(int *tref, int *reason, t_opt2 *opt);
int	t_318event(int *tref, int cmode, t_opte *opt);
int	t_319error(void);
int	t_320info(int *tref, t_opti *opt);
int	t_321vdatarq(int *tref, struct t_data *vdata, int *vcnt, int *chain);
int	t_322vdatain(int *tref, struct t_data *vdata, int *vcnt, int *chain);


int	t_300attach(struct t_myname *name, t_opta *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_301detach(struct t_myname *name)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_302conrq(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_303conin(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_304conrs(int *tref, t_opt13 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_305concf(int *tref, t_opt1 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_306datarq(int *tref, char *datap, int *datal, int *chain)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_307datain(int *tref, char *datap, int *datal, int *chain)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_308datago(int *tref)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_309datastop(int *tref)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_310redrq(int *tref, int *pid, t_opt12 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_311redin(int *tref, int *pid, t_opt23 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_312xdatrq(int *tref, char *datap, int *datal)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_313xdatin(int *tref, char *datap, int *datal)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_314xdatgo(int *tref)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_315xdatstop(int *tref)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_316disrq(int *tref, t_opt2 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_317disin(int *tref, int *reason, t_opt2 *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_318event(int *tref, int cmode, t_opte *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_319error(void)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_320info(int *tref, t_opti *opt)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}
int	t_321vdatarq(int *tref, struct t_data *vdata, int *vcnt, int *chain)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}

int	t_322vdatain(int *tref, struct t_data *vdata, int *vcnt, int *chain)
{
	_t_error = T_WREQUEST ;
	return (T_ERROR) ;
}
