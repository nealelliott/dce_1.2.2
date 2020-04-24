/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmx2k.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:04  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:32  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:46:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:56  root]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:22  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:21:21  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:48:15  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:45:53  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:16:24  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:24  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:03  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:50:16  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmx2k.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:04 $";
#endif

/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG  All rights reserved
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

/*****************************************************************************/
/*                                                                           */
/*        ICMX(L) CONNECTION MODULE FOR SYSTEM/STATION/SNA COUPLING          */
/*        ===========================================================        */
/*                                                                           */
/*     ICMX(L) is an ISO 8072 compatible programming interface, implemented  */
/*  in numerous "C" library functions. In addition to the individual service */
/*  primitives of ISO 8072 (T-CONNECTION, T-DISCONNECTION, T-DATA, and       */
/*  T-EXPEDITED-DATA) with the corresponding elements (.request, .indication,*/
/*  .response, and .confirmation), ICMX(L) also offers local primitive, which*/
/*  support and facilitate the concrete implementation of CMX applications   */
/*  a real environment (T-ATTACH, T-DETACH, T-REDIRECT, T-EVENT              */
/*  T-FLOWCONTROL, T-INFO und T-CALLBACK).				     */
/*     A CMX application (generally) does not need to know how the local end */
/*  system, in which it is running, is connected to the remote end system.   */
/*  CMX supports various connection types in a uniform way for the CMX       */
/*  application, i.e.:                                                       */
/*      T_COM   Usual system connection in networks using OSI-compatible     */
/*              transport system (including TRANSDATA)                       */
/*      T_TUI   Station coupling without using a transport system by means of*/
/*              data communications procedures and TUI (Terminal User        */
/*              in the TRANSDATA network                                     */
/*      T_SNA   Station coupling to a SNA Network using a central            */
/*              SNA nucleus                                                  */
/*      T_INET  System coupling using TCP/IP protocol in INTERNET            */
/*      T_XTI   System coupling using XTI                                    */
/*     This connection module enables a CMX user process to simultaneously   */
/*  access the ICMX(L) variant features for system and/or station coupling,  */
/*  and/or SNA coupling. The respective set of functions is selected with the*/
/*  first t_attach() call using the information transferred in the LOCAL     */
/*  NAME (parameter name).                                                   */
/*  With subsequent calls, the selection is made using information in the    */
/*  TRANSPORTADDRESS of the partner (t_conrq()) or an ID of the tref         */
/*  (other). In the case of t_event(), each reported coupling is requested.  */
/*     CMX deals with the following signals:                                 */
/*      SIGPIPE can occur when accessing TNS using FIFOs or                  */
/*              with T_INET coupling when accessing sockets. It is ignored   */
/*              while reporting the process with CMX.                        */
/*      SIGIO   are sent by T_COM/T_TUI/T_INET connections, if there is an   */
/*              event here. With careful programming of races, this can be   */
/*              used to multiplex I/O streams (SIGIO aborts read() to tty    */
/*              with EINTR).                                                 */
/*     File descriptors are used in all connections in order to access the   */
/*  system functions. This can be opened as required and closed after use.   */
/*									     */
/* For address information:                                                  */
/*  There are CMX applications that want to transfer name and address formats*/
/*  to ICMX(L), other than those defined in TNS. In order to do this, we     */
/*  accept then these formats at the interface, but subject them immediately */
/*  to a mapping function t_mapaddr() or t_mapname(). These functions are in */
/*  a separate module of the CMX library and NOOPS in the normal case. Each  */
/*  CMX application can define these functions in its area and thus use NOOPS*/
/*  provided in the CMX library at this positions.                           */
/*									     */
/*                            cmx2k.c   === ICMX(L)		             */
/*      +--------------------------------------------------------------+     */
/*      |     |             CMX(L) connection module                   |     */
/*      +--------------------------------------------------------------+     */
/*       |              |              |              |              |       */
/* +------------+ +------------+ +------------+ +------------+ +------------+*/
/* | CMX(L)-Lib | | CMX(L)-Lib | | CMX(L)-Lib | | CMX(L)-Lib | | CMX(L)-Lib |*/
/* |  for       | |  for       | |  for       | |  for       | |  for       |*/
/* | system     | | station    | | SNA        | | INET       | | XTI        |*/
/* | coupling   | | coupling   | | coupling   | | coupling   | | coupling   |*/
/* +------------+ +------------+ +------------+ +------------+ +------------+*/
/* cmx3.c         cmx.c          cmxsna.c       cmxin*c        cmxxti.c      */
/*									     */
/* Defines:                                                                  */
/* SIGIO        if not defined (XPG2 or SVID), or set to BSD SIGIO           */
/*									     */
/*									     */
/* Author: R. Horn V2.0                                  Date: 5.5.86        */
/* Ammendments:                                                              */
/* 21.07.87 V2.1 rit							     */
/* 25.08.88 V2.2 obg(UNA)						     */
/* 16.03.89 V2.5 rit 							     */
/* 31.05.90 V3.0 bee 							     */
/* 05.03.92 V3.1 kam 							     */
/*****************************************************************************/

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <errno.h>
#include <stdio.h>
#if defined(SO_AVAIL)
#include <dlfcn.h>
#endif
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#if defined(_AIX)
# include <sys/select.h>
#endif /* _AIX */
#include <sys/types.h>
#include <sys/time.h>
#define t_error(x) *meng_ma_ned
#if !defined(__hpux)
#include <sys/tiuser.h>
#endif
#include <ndsif.h>
#undef t_error /* now t_error is taken from CMX */
#define CMX2K
#undef T_ERROR
#undef T_MORE
#undef T_CHECK
#include "cmx.h"
#include "cmxextern.h"
#include "cmxaddr.h"
#include "cmxtrace.h"
#include "tnsx.h"
#include "cmx_macros.h"

#ifndef SIGIO
#define SIGIO 22
#endif

#ifndef	NSIG
#define	NSIG	32	/* it's defined in the header file only sometimes */
#endif

/*
static char	sccsid[] = "@(#)cmx2k.c	311.43 93/08/24 CMX (NC1)";
*/

/*****************************************************************************/
/*      D E C L A R A T I O N   G L O B A L   D A T A                        */
/*****************************************************************************/

/*
 * This module contains distributer functions and special help routine,
 * which are neutral on it:
 *      t_ncpy          Copy function for ANY byte strings
 *      t_fdset;        fd_set with 1 bit corresponding to CMX internal fd's
 *      _ds_error       Calls ts_error, expected by neabx.c
 *      t_callback      Calls an application routine instead of waiting select
 *      t_getccptyp     Decides which transport system is selected
 *      t_addcclist     Adds a cc list to the global cc list
 */

fd_set  t_fdset, t_wfds, t_rfds, t_xfds;                                        
int     cmx_fd = -1;		/* File descriptor for CMX automaton	*/
int     t_ncpy(register char *, register char *, register int);
int     t_pid = 0;		/* Process ID of CMX application process*/
int     _t_error = 0;		/* ICMX(L) error variable		*/
int     t_sfds = 0;                                                             
int     t_cmxsig = 0;		/* >0: CMX signalization with this signal-# */
int     t_sigio = FALSE;	/* TRUE: CMX handler active for SIGIO	*/
int     t_sigoth = FALSE;	/* TRUE: CMX handler active for SIGOTH	*/
#if defined(AIX) || defined(__OSF__)
struct sigaction t_prevsigio = {SIG_PTR SIG_DFL,0,0};  /* Store SIGIO handler */
struct sigaction t_prevsigoth = {SIG_PTR SIG_DFL,0,0}; /* Store old signal handler */
#else
struct sigaction t_prevsigio = {0,SIG_PTR SIG_DFL,0};  /* Store SIGIO handler */
struct sigaction t_prevsigoth = {0,SIG_PTR SIG_DFL,0}; /* Store old signal handler */
#endif  /* AIX || __OSF__ */

void	t_signal(int);			/* Signal handler		*/
	/* no call-back routine mounted	*/
int	(*t_cb_routine)(int,fd_set*,fd_set*,fd_set*,struct timeval*,void*) = 
	(int(*)(int,fd_set*,fd_set*,fd_set*,struct timeval*,void*))NULL;
void	*usrptr;			/* user pointer with_callback	*/
int	open_max;			/* global variable for open_max	*/
int	t_tidusize = 1024;		/* TIDU-size for TCP/IP and TLI/XTI */
/* NDS-Facilities */
	/* Function nds_map_addrinfo	*/
int	(*NDS_map_addrinfo)(int, Nds_mapinfo *) = 
		(int(*)(int, Nds_mapinfo *))NULL;
void	*NDS_handler = NULL;		/* Handler for dlopen/dlsym	*/
#if defined(__hpux)
#define NDS_LIBNAME "/usr/lib/libNDS.sl"
#else
#define NDS_LIBNAME "/usr/lib/libNDS.so"
#endif

/* Declaration internal functions  */
static int	t_getccptyp (unsigned short type);
/* this one will be exported		*/
void	t_addcclst(struct t_cclst *result_cclist);

/*
 * Declaration of external functions for the connections
 * t_1??xxxxx for system coupling
 * t_2??xxxxx for station coupling
 * t_3??xxxxx for SNA coupling
 * t_4??xxxxx for system coupling over TCP/IP protocol
 * t_5??xxxxx for system coupling over XTI
 */
int	t_100attach(struct t_myname *name, t_opta *opt),
	t_101detach(struct t_myname *name),
	t_102conrq(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt),
	t_103conin(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt),
	t_104conrs(int *tref, t_opt13 *opt),
	t_105concf(int *tref, t_opt1 *opt),
	t_106datarq(int *tref, char *datap, int *datal, int *chain),
	t_107datain(int *tref, char *datap, int *datal, int *chain),
	t_108datago(int *tref),
	t_109datastop(int *tref),
	t_110redrq(int *tref, int *pid, t_opt12 *opt),
	t_111redin(int *tref, int *pid, t_opt23 *opt),
	t_112xdatrq(int *tref, char *datap, int *datal),
	t_113xdatin(int *tref, char *datap, int *datal),
	t_114xdatgo(int *tref),
	t_115xdatstop(int *tref),
	t_116disrq(int *tref, t_opt2 *opt),
	t_117disin(int *tref, int *reason, t_opt2 *opt),
	t_118event(int *tref, int cmode, t_opte *opt),
	t_119error(void),
	t_120info(int *tref, t_opti *opt),
	t_121vdatarq(int *tref, struct t_data *vdata, int *vcnt, int *chain),
	t_122vdatain(int *tref, struct t_data *vdata, int *vcnt, int *chain);

int	t_200attach(struct t_myname *name, t_opta *opt),
	t_201detach(struct t_myname *name),
	t_202conrq(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt),
	t_203conin(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt),
	t_204conrs(int *tref, t_opt13 *opt),
	t_205concf(int *tref, t_opt1 *opt),
	t_206datarq(int *tref, char *datap, int *datal, int *chain),
	t_207datain(int *tref, char *datap, int *datal, int *chain),
	t_208datago(int *tref),
	t_209datastop(int *tref),
	t_210redrq(int *tref, int *pid, t_opt12 *opt),
	t_211redin(int *tref, int *pid, t_opt23 *opt),
	t_212xdatrq(int *tref, char *datap, int *datal),
	t_213xdatin(int *tref, char *datap, int *datal),
	t_214xdatgo(int *tref),
	t_215xdatstop(int *tref),
	t_216disrq(int *tref, t_opt2 *opt),
	t_217disin(int *tref, int *reason, t_opt2 *opt),
	t_218event(int *tref, int cmode, t_opte *opt),
	t_219error(void),
	t_220info(int *tref, t_opti *opt),
	t_221vdatarq(int *tref, struct t_data *vdata, int *vcnt, int *chain),
	t_222vdatain(int *tref, struct t_data *vdata, int *vcnt, int *chain);

int	t_300attach(struct t_myname *name, t_opta *opt),
	t_301detach(struct t_myname *name),
	t_302conrq(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt),
	t_303conin(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt),
	t_304conrs(int *tref, t_opt13 *opt),
	t_305concf(int *tref, t_opt1 *opt),
	t_306datarq(int *tref, char *datap, int *datal, int *chain),
	t_307datain(int *tref, char *datap, int *datal, int *chain),
	t_308datago(int *tref),
	t_309datastop(int *tref),
	t_310redrq(int *tref, int *pid, t_opt12 *opt),
	t_311redin(int *tref, int *pid, t_opt23 *opt),
	t_312xdatrq(int *tref, char *datap, int *datal),
	t_313xdatin(int *tref, char *datap, int *datal),
	t_314xdatgo(int *tref),
	t_315xdatstop(int *tref),
	t_316disrq(int *tref, t_opt2 *opt),
	t_317disin(int *tref, int *reason, t_opt2 *opt),
	t_318event(int *tref, int cmode, t_opte *opt),
	t_319error(void),
	t_320info(int *tref, t_opti *opt),
	t_321vdatarq(int *tref, struct t_data *vdata, int *vcnt, int *chain),
	t_322vdatain(int *tref, struct t_data *vdata, int *vcnt, int *chain);

int	t_400attach(struct t_myname *name, t_opta *opt),
	t_401detach(struct t_myname *name),
	t_402conrq(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt),
	t_403conin(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt),
	t_404conrs(int *tref, t_opt13 *opt),
	t_405concf(int *tref, t_opt1 *opt),
	t_406datarq(int *tref, char *datap, int *datal, int *chain),
	t_407datain(int *tref, char *datap, int *datal, int *chain),
	t_408datago(int *tref),
	t_409datastop(int *tref),
	t_410redrq(int *tref, int *pid, t_opt12 *opt),
	t_411redin(int *tref, int *pid, t_opt23 *opt),
	t_412xdatrq(int *tref, char *datap, int *datal),
	t_413xdatin(int *tref, char *datap, int *datal),
	t_414xdatgo(int *tref),
	t_415xdatstop(int *tref),
	t_416disrq(int *tref, t_opt2 *opt),
	t_417disin(int *tref, int *reason, t_opt2 *opt),
	t_418event(int *tref, int cmode, t_opte *opt),
	t_419error(void),
	t_420info(int *tref, t_opti *opt),
	t_421vdatarq(int *tref, struct t_data *vdata, int *vcnt, int *chain),
	t_422vdatain(int *tref, struct t_data *vdata, int *vcnt, int *chain);

int	t_500attach(struct t_myname *name, t_opta *opt),
	t_501detach(struct t_myname *name),
	t_502conrq(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt),
	t_503conin(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt),
	t_504conrs(int *tref, t_opt13 *opt),
	t_505concf(int *tref, t_opt1 *opt),
	t_506datarq(int *tref, char *datap, int *datal, int *chain),
	t_507datain(int *tref, char *datap, int *datal, int *chain),
	t_508datago(int *tref),
	t_509datastop(int *tref),
	t_510redrq(int *tref, int *pid, t_opt12 *opt),
	t_511redin(int *tref, int *pid, t_opt23 *opt),
	t_512xdatrq(int *tref, char *datap, int *datal),
	t_513xdatin(int *tref, char *datap, int *datal),
	t_514xdatgo(int *tref),
	t_515xdatstop(int *tref),
	t_516disrq(int *tref, t_opt2 *opt),
	t_517disin(int *tref, int *reason, t_opt2 *opt),
	t_518event(int *tref, int cmode, t_opte *opt),
	t_519error(void),
	t_520info(int *tref, t_opti *opt),
	t_521vdatarq(int *tref, struct t_data *vdata, int *vcnt, int *chain),
	t_522vdatain(int *tref, struct t_data *vdata, int *vcnt, int *chain);

/*****************************************************************************/
/*      D E C L A R A T I O N   I N T E R N A L   D A T A                    */
/*****************************************************************************/

/*
 * With respect to implementation, a control box in t_switch[] corresponds
 * to each connection, and contains
 * pointers to the corresponding CMX functions. Thus, all connections are
 * ALWAYS connected in the CMX application.
 */

/*
 * The tref is marked for coupling
 * existing connections. For this, we use the uppermost 4 bits, hoping
 * that the remaining 28 bits are sufficient for the tref.
 * (It is best to not to use bit 31, due to its sign extension.)
 * The bits must be managed in the underlying CMX components in the
 * nucleus.
 */

#define T_COMTREF	0
#define T_STATREF	(T_TUI << 28)
#define T_SNATREF	(T_SNA << 28)
#define T_INETREF	(T_INET << 28)
#define T_XTITREF	(T_XTI << 28)
#define T_TREFMASK	((T_TUI | T_SNA | T_INET | T_XTI) << 28)
#define T_MAPTREF(tref) \
	 (tref & T_TREFMASK) == T_COMTREF ? T_COM : \
	 (tref & T_TREFMASK) == T_STATREF ? T_TUI : \
	 (tref & T_TREFMASK) == T_INETREF ? T_INET : \
	 (tref & T_TREFMASK) == T_XTITREF ? T_XTI : \
	 (tref & T_TREFMASK) == T_SNATREF ? T_SNA : (-1)

#define T_NSW		5
#define T_NOSIIF	3

static int      t_attcnt;               /* Central counter of 't_attach' */
static int      t_evcnt=0;              /* Supervisor of t_event recursion */

static int      t_ignsigpipe = 0;       /* > 0: SIGPIPE is ignored */
#if defined(AIX) || defined(__OSF__)
static struct sigaction t_prsigpipe = {SIG_PTR SIG_IGN,0,0};
#else
static struct sigaction t_prsigpipe = {0,SIG_PTR SIG_IGN,0};
#endif  /* AIX || __OSF__ */

static struct timeval	t;

static int	t_wreq(void);

struct	t_cclst cc_result;            /* Result parameter for t_attach */

/*
 * Control structure for branching and coupling
 */
static struct t_switch {
	short   t_type;                 /* Type of connection*/
	short   t_refcnt;               /* Number of references*/
	int	(*t_attach)(struct t_myname *name, t_opta *opt);
	int	(*t_detach)(struct t_myname *name);
	int	(*t_conrq)(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt13 *opt);
	int	(*t_conin)(int *tref, union t_address *toaddr, union t_address *fromaddr, t_opt1 *opt);
	int	(*t_conrs)(int *tref, t_opt13 *opt);
	int	(*t_concf)(int *tref, t_opt1 *opt);
	int	(*t_datarq)(int *tref, char *datap, int *datal, int *chain);
	int	(*t_datain)(int *tref, char *datap, int *datal, int *chain);
	int	(*t_datago)(int *tref);
	int	(*t_datastop)(int *tref);
	int	(*t_redrq)(int *tref, int *pid, t_opt12 *opt);
	int	(*t_redin)(int *tref, int *pid, t_opt23 *opt);
	int	(*t_xdatrq)(int *tref, char *datap, int *datal);
	int	(*t_xdatin)(int *tref, char *datap, int *datal);
	int	(*t_xdatgo)(int *tref);
	int	(*t_xdatstop)(int *tref);
	int	(*t_disrq)(int *tref, t_opt2 *opt);
	int	(*t_disin)(int *tref, int *reason, t_opt2 *opt);
	int	(*t_event)(int *tref, int cmode, t_opte *opt);
	int	(*t_cmxerror)(void);
	int	(*t_info)(int *tref, t_opti *opt);
	int	(*t_vdatarq)(int *tref, struct t_data *vdata, int *vcnt, int *chain);
	int	(*t_vdatain)(int *tref, struct t_data *vdata, int *vcnt, int *chain);
} t_switch[] = {
	{ /*  System coupling */
		T_COM, 0,
		t_100attach, t_101detach, t_102conrq, t_103conin,
		t_104conrs, t_105concf, t_106datarq, t_107datain,
		t_108datago, t_109datastop, t_110redrq, t_111redin,
		t_112xdatrq, t_113xdatin, t_114xdatgo, t_115xdatstop,
		t_116disrq, t_117disin, t_118event, t_wreq, t_120info,
		t_121vdatarq, t_122vdatain },
	{ /* Station coupling */
		T_TUI, 0,
		t_200attach, t_201detach, t_202conrq, t_203conin,
		t_204conrs, t_205concf, t_206datarq, t_207datain,
		t_208datago, t_209datastop, t_210redrq, t_211redin,
		t_212xdatrq, t_213xdatin, t_214xdatgo, t_215xdatstop, 
		t_216disrq, t_217disin, t_218event, t_wreq, t_220info,
		t_221vdatarq, t_222vdatain },
	 { /* SNA coupling */
		T_SNA, 0,
		t_300attach, t_301detach, t_302conrq, t_303conin,
		t_304conrs, t_305concf, t_306datarq, t_307datain,
		t_308datago, t_309datastop, t_310redrq, t_311redin,
		t_312xdatrq, t_313xdatin, t_314xdatgo, t_315xdatstop, 
		t_316disrq, t_317disin, t_318event, t_wreq, t_320info,
		t_321vdatarq, t_322vdatain },
	 { /* System coupling over TCP/IP protocol */
		T_INET, 0,
		t_400attach, t_401detach, t_402conrq, t_403conin,
		t_404conrs, t_405concf, t_406datarq, t_407datain,
		t_408datago, t_409datastop, t_410redrq, t_411redin,
		t_412xdatrq, t_413xdatin, t_414xdatgo, t_415xdatstop, 
		t_416disrq, t_417disin, t_418event, t_wreq, t_420info,
		t_421vdatarq, t_422vdatain },
	 { /* System coupling over XTI */
		T_XTI, 0,
		t_500attach, t_501detach, t_502conrq, t_503conin,
		t_504conrs, t_505concf, t_506datarq, t_507datain,
		t_508datago, t_509datastop, t_510redrq, t_511redin,
		t_512xdatrq, t_513xdatin, t_514xdatgo, t_515xdatstop, 
		t_516disrq, t_517disin, t_518event, t_wreq, t_520info,
		t_521vdatarq, t_522vdatain }
};

/*---------------------------------------------------------------------------*
 * T-ATTACH
 *---------------------------------------------------------------------------*/

/*
 * T-ATTACH.request
 * Reports the CMX application for a number of connections.
 * Already successful, if one of the subordinate logons for the required
 * connections is successful.
 * NOTE: There is no definition stating for which of a number of connections
 * T_NOTFIRST is supplied. If all logons fail, only the error of the
 * last one is reported.
 */
t_attach(name, opt)
struct t_myname	*name;
t_opta	*opt;
{
	struct cx_myname *p ;
	char		 my_osiaddr[T_NOSIIF * T_TSAPSIZE] ;
	T_adrdesc 	 *p2 ;
	Nds_cmxselinfo	 osiselinfo ;
	union cx_unisel	 *p1;
	int		i, j, pid, sel_cnt, result, ok_notf = T_ERROR;
	int		t[T_NSW];
#if defined(SO_AVAIL)
	char	*dlerr;
#endif
static struct sigaction prev;
static struct sigaction cmxsig;
#if defined(AIX) || defined(__OSF__)
static struct sigaction sig_ign = {SIG_PTR SIG_IGN,0,0};
#else
static struct sigaction sig_ign = {0,SIG_PTR SIG_IGN,0};
#endif /* AIX || __OSF__ */

	/*
	 * Parameter and status check.
	 * After fork(), t_switch[] is installed again.
	 */
	_t_error = T_WPARAMETER;
	if ((pid = getpid()) != t_pid) {
		for (i = 0; i < T_NSW; i++)
			t_switch[i].t_refcnt = 0;
		t_attcnt = 0;
		t_pid = pid;

		if (t_sigio) {
			sigaction(SIGIO,&t_prevsigio, NULL);
			t_sigio = FALSE;
			t_prevsigio.sa_handler = SIG_DFL;
		}
		if (t_sigoth) {
			sigaction(SIGTERM,&t_prevsigoth, NULL);
			t_sigoth = FALSE;
			t_prevsigoth.sa_handler = SIG_DFL;
		}

		/*
		 * Would be helpful for reinitialization, but not possible,
		 * as cmx_fd remains open in the T_COM module over t_detach().
		FD_ZERO(&t_fdset);
		FD_ZERO(&t_rfds);
		FD_ZERO(&t_wfds);
		FD_ZERO(&t_xfds);
		 */
	}
	/*
	 * Mapping of LOCAL NAME in front of trace. We specify absolutely no
	 * prerequesites to guarantee maximum flexibility.
	 */
	name = (struct t_myname *) t_mapname ((char *)name);
	t_trc(T_FATTACH, 0, name, opt);
	p = (struct cx_myname *) ((struct t_myname *) name)->t_mn;

	/*
	 * cclst, ccbits, and sptypes are arranged by the connections,
	 * therefore delete here
	 */

	open_max = sysconf(_SC_OPEN_MAX);

	if (opt != NULL &&
	     ((opt->opta2.t_optnr == T_OPTA2) ||
	      (opt->opta5.t_optnr == T_OPTA5) ||
	      (opt->opta6.t_optnr == T_OPTA6))) {
		opt->opta2.t_ccbits = 0;
		opt->opta2.t_sptypes = 0;
		cc_result.cl_num = 0;
	}

	if (name->t_mnmode == T_MNMODE) {
		/* Name server is TNS */
		/*
		* With the connections, logon ONCE in name in compliance with TSELs.
		* Errors with sequence LAN2 after LAN1; therefore from the back.
	 	*/
		for (i = 0; i < T_NSW; t[i++] = 0)
			;
		sel_cnt = (((struct t_myname *) name)->t_mnlng - (CXN_PRESIZE +
			CXN_FIXSIZE))/sizeof (union cx_unisel);
		if (sel_cnt < 1 || sel_cnt > CXN_SELNUM) {
			_t_error = T_WPARAMETER;
			goto out;
		}
		p1 = &p->cxn_sel[sel_cnt];
		while (sel_cnt--) {
			p1--;
			j = t_getccptyp(p1->cxs.cx_type);
			if (t[j] == 0) {                /* only once per connection */
				t_trc(T_DATTACH, j);
	
				result = ((*t_switch[j].t_attach)(name, opt));
	
				if (result == T_ERROR) {
					t_trc(T_DRETERR, _t_error);
					continue;
				}
				t_trc(T_DRETOK, T_FATTACH, result);
				if (result != T_NOTPRIV) {
					t_switch[j].t_refcnt++; /* successfully attached */
					t[j]++;
					t_attcnt++;
				}
				if (ok_notf == T_ERROR)
					ok_notf = result;
				else
					ok_notf |= result;
			}
		}
	} else {
		/* Name server is DIRX */
		if ((((T_adrdesc *) name)->t_infotype != T_TSAPINFO) &&
		    (((T_adrdesc *) name)->t_infotype != T_NSAPINFO)) {
			_t_error = T_WPARAMETER;
			goto out;
		}
		if (NDS_handler == NULL) {
#if defined(SO_AVAIL)
			NDS_handler = dlopen(NDS_LIBNAME, RTLD_LAZY);
			if (NDS_handler == NULL) {
				/* NDS-ERROR */
				dlerr=dlerror();
				t_trc(T_DLERRO,0,dlerr);
				_t_error = T_W_NDS_ACCESS;
				goto out;
			}

			NDS_map_addrinfo = (int(*)(int, (Nds_mapinfo *)))
				dlsym(NDS_handler,"nds_map_addrinfo");
			if (NDS_map_addrinfo == (int(*)(int, (Nds_mapinfo *)))NULL) {
				/* NDS-ERROR */
				t_trc(T_DLERRS,0,dlerror());
				_t_error = T_W_NDS_ACCESS;
				goto out;
			}
#elif defined (NDS_LNK_ACCESS)
			NDS_map_addrinfo = nds_map_addrinfo ;
#else
			_t_error = T_W_NDS_ACCESS;
			goto out;
#endif
		}

		/* Logon with all connections corresponds to OSI-NSAP addresse(s) */
		osiselinfo.nds_osi1addr = (T_adrdesc *) name ;
		osiselinfo.nds_sel1addr = (T_adrdesc *) my_osiaddr ;
		osiselinfo.nds_osi2addr = (T_adrdesc *) NULL ;
		if (NDS_map_addrinfo (NDS_CMXSEL, (Nds_mapinfo *) &osiselinfo) < 0) {
			_t_error = osiselinfo.nds_errno ;
			goto out;
		}
		p2 = (T_adrdesc *) my_osiaddr ;
		for (i = 0; i < T_NSW; i++) {
			if ((osiselinfo.nds_iftypes & (1 << i)) == 0)
				continue ;
	
			t_trc(T_DATTACH, i);

			result = ((*t_switch[i].t_attach)((char *) p2, opt));

			if (result == T_ERROR)
				t_trc(T_DRETERR, _t_error);
			else {
				t_trc(T_DRETOK, T_FATTACH, result);
						/* erfolgreich attach'ed */
				t_switch[i].t_refcnt++;
				t_attcnt++;
				if (ok_notf == T_ERROR)
					ok_notf = result;
				else
					ok_notf |= result;
			}
			p2 = (T_adrdesc *) ((char *) p2 + p2->t_translng) ;
		}
	}

out:
	if (opt != NULL && (opt->opta6.t_optnr == T_OPTA6))
		opt->opta6.t_cclist = (struct t_cclst *)&cc_result;

	if (ok_notf == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else {
		t_trc(T_FRETATT, 0, ok_notf, opt);
		if (t_cmxsig == SIGIO) {
			if (!t_sigio) {
				t_trc(T_FSIGIO,0);
				cmxsig.sa_flags=0;
				cmxsig.sa_handler=t_signal;
				sigemptyset(&cmxsig.sa_mask);
				sigaction(t_cmxsig,&cmxsig,&t_prevsigio);
				t_sigio = TRUE;
				if (t_prevsigio.sa_handler == SIG_DFL) 
					;
				else {
					cmxsig.sa_flags=0;
					cmxsig.sa_handler=t_signal;
					sigemptyset(&cmxsig.sa_mask);
					sigaction(t_cmxsig,&t_prevsigio,NULL);
					t_sigio = FALSE;
				}
			}
		}
		else {
			if (t_cmxsig != 0) {
				if (!t_sigoth) {
					t_trc(T_FSIGTERM,0);
					cmxsig.sa_flags=0;
					cmxsig.sa_handler=t_signal;
					sigemptyset(&cmxsig.sa_mask);
					sigaction(t_cmxsig,&t_prevsigio,&t_prevsigoth);
					t_sigoth = TRUE;
					if (t_prevsigoth.sa_handler == SIG_DFL) 
						;
					else {
						cmxsig.sa_flags=0;
						cmxsig.sa_handler=t_signal;
						sigemptyset(&cmxsig.sa_mask);
						sigaction(t_cmxsig,&t_prevsigoth,NULL);
						t_sigoth = FALSE;
					}
				}
			}
		}
				

		if (t_ignsigpipe != 0
		&& (sigaction(SIGPIPE, &sig_ign, &prev)) == 0) {
			t_prsigpipe = prev;
			t_ignsigpipe++;
		}
	}
	return (ok_notf); }

/*
 * T-ATTACH.release
 * Logs off the CMX application with several connections.
 * Is already successful if one of the subordinate logons for the required
 * connections is successul.
 * NOTE: Silence is required; the name itself is as with the corresponding
 * application.
 */
t_detach(name)
struct t_myname *name;
{
	struct cx_myname *p;
	char		 my_osiaddr[T_NOSIIF * T_TSAPSIZE] ;
	T_adrdesc *p2 ;
	Nds_cmxselinfo	 osiselinfo ;
	union cx_unisel	*p1;
	int		i, j, sel_cnt, result = T_ERROR;
	int		t[T_NSW];

	/*
	 * Parameter and status check
	 * LOCAL NAME must be syntactically correct.
	 * Mapping of LOCAL NAME before trace. We specify absolutely no
	 * prerequisites, in order to guarantee maximum flexibility.
	 */
	name = (struct t_myname *) t_mapname ((char *)name);
	t_trc(T_FDETACH, 0, name);

	if (t_pid != getpid() || t_attcnt == 0) {
		_t_error = T_WSEQUENCE;
		goto out;
	}
	p = (struct cx_myname *) ((struct t_myname *) name)->t_mn;

	/*
	 * With the connections, log off ONCE in the name in compliance with
	 * the TSELs.
	 */
	result = T_ERROR;  
	if (name->t_mnmode == T_MNMODE) {
		/* Name server is TNS */
		for (i = 0; i < T_NSW; t[i++] = 0)
			;
		sel_cnt = (((struct t_myname *) name)->t_mnlng - (CXN_PRESIZE +
			CXN_FIXSIZE))/sizeof (union cx_unisel);
		if (sel_cnt < 1 || sel_cnt > CXN_SELNUM) {
			_t_error = T_WPARAMETER;
			goto out;
		}
		_t_error = T_WAPPLICATION;
		p1 = &p->cxn_sel[sel_cnt];
		while (sel_cnt--) {
			p1--;
			j = t_getccptyp(p1->cxs.cx_type);
			if (t[j]++ == 0 && t_switch[j].t_refcnt) {
				int	tmp;
	
				t_trc(T_DDETACH, j);

				tmp = (*t_switch[j].t_detach)(name);

				if (tmp == T_ERROR) {
					t_trc(T_DRETERR, _t_error);
					continue;
				}
				t_switch[j].t_refcnt--;	/* nicht attach'ed */
				t_attcnt--;
				result = tmp;
			}
		}
	} else {
		/* Name server is DIRX */
		if ((((T_adrdesc *) name)->t_infotype != T_TSAPINFO) &&
		    (((T_adrdesc *) name)->t_infotype != T_NSAPINFO)) {
			_t_error = T_WPARAMETER;
			goto out;
		}

		/* Log off with all connection corresponds to OSI-NSAP address(es) */
		osiselinfo.nds_osi1addr = (T_adrdesc *) name ;
		osiselinfo.nds_sel1addr = (T_adrdesc *) my_osiaddr ;
		osiselinfo.nds_osi2addr = (T_adrdesc *) NULL ;
		if (NDS_map_addrinfo (NDS_CMXSEL, (Nds_mapinfo *) &osiselinfo) < 0) {
			_t_error = osiselinfo.nds_errno ;
			goto out;
		}
		p2 = (T_adrdesc *) my_osiaddr ;
		for (i = 0; i < T_NSW; i++) {
			if ((osiselinfo.nds_iftypes & (1 << i)) == 0)
				continue ;
	
			if (t_switch[i].t_refcnt) {
				t_trc(T_DDETACH, i);

				result = ((*t_switch[i].t_detach)((char *) p2));

				if (result == T_ERROR)
					t_trc(T_DRETERR, _t_error);
				else {
					t_trc(T_DRETOK, T_FATTACH, result);
						/* nicht attach'ed */
					t_switch[i].t_refcnt--;
					t_attcnt--;
				}
			}
			p2 = (T_adrdesc *) ((char *) p2 + p2->t_translng) ;
		}
	}
out: 
	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else {
		if (t_attcnt == 0) {    /* all applications logged off */
			if (t_ignsigpipe) {
				sigaction(SIGPIPE,&t_prsigpipe,NULL);
				t_prsigpipe.sa_handler = SIG_IGN;
				t_ignsigpipe = 0;
			}
			if (t_cmxsig == SIGIO) {
				if (t_sigio) {
					sigaction(t_cmxsig,&t_prevsigio,NULL);
					t_sigio = FALSE;
					t_cmxsig = 0;
					t_prevsigio.sa_handler = SIG_DFL; 
				}
			}
			else {
				if (t_cmxsig != 0) {
					if (t_sigoth) {
						sigaction(t_cmxsig,&t_prevsigoth,NULL);
						t_sigoth = FALSE;
						t_cmxsig = 0;
						t_prevsigoth.sa_handler = SIG_DFL; 
					}
				}
			}
			t_pid = 0;
		}
		t_trc (T_FRETOK, T_FDETACH, result);
	}
	return (result);
}

/*---------------------------------------------------------------------------*
 * T-CONNECTION
 *---------------------------------------------------------------------------*/

/*
 * T-CONNECTION.request
 */
t_conrq(tref, toaddr, fromaddr, opt)
int	*tref;
union t_address	*toaddr, *fromaddr;
t_opt13	*opt;
{
	struct cx_addr	*p1 ;
	char		 my_osiaddr[T_TSAPSIZE] ;
	char		 pa_osiaddr[T_TSAPSIZE] ;
	Nds_cmxselinfo	 osiselinfo ;
	int	result = T_ERROR;
	int	i ;

	/* 
	 * For maximum flexibility when mapping names and addresses, there
	 * are no prerequisites for the contents. Map 'toaddr' first, as
	 * 'fromaddr' is used for this in the original.
	 */
	toaddr = t_mapaddr (toaddr, fromaddr);
	fromaddr = (union t_address *) t_mapname ((char *)fromaddr);
	t_trc(T_FCONRQ, 0, tref, toaddr, fromaddr, opt);

	if (((fromaddr->tmyname.t_mnmode == T_MNMODE) ||
	     (fromaddr->tmyname.t_mnmode == T_PAMODE)) &&
	    (toaddr->tpartaddr.t_pamode == T_PAMODE)      ) {
		p1 = (struct cx_addr *) toaddr->tpartaddr.t_pa;
		i = t_getccptyp(p1->cx_type);

		if (t_pid != getpid() || t_switch[i].t_refcnt == 0)
			_t_error = T_WSEQUENCE;
		else
			result = ((*t_switch[i].t_conrq) (tref, toaddr,
								fromaddr, opt));
	} else {
		if (((((T_adrdesc *) toaddr)->t_infotype != T_TSAPINFO) &&
		     (((T_adrdesc *) toaddr)->t_infotype != T_NSAPINFO)) &&
		    ((((T_adrdesc *) fromaddr)->t_infotype != T_TSAPINFO) &&
		     (((T_adrdesc *) fromaddr)->t_infotype != T_NSAPINFO))) {
			_t_error = T_WPARAMETER;
			goto out;
		}
		osiselinfo.nds_osi1addr = (T_adrdesc *) fromaddr ;
		osiselinfo.nds_osi2addr = (T_adrdesc *) toaddr ;
		osiselinfo.nds_sel1addr = (T_adrdesc*) my_osiaddr ;
		osiselinfo.nds_sel2addr = (T_adrdesc *) pa_osiaddr ;
		osiselinfo.nds_mprule = NDS_HPRIO ;
		
		if (NDS_map_addrinfo (NDS_CMXSEL, (Nds_mapinfo *) &osiselinfo) < 0) {
			_t_error = osiselinfo.nds_errno ;
			goto out;
		}
		for (i = 0; i < T_NSW; i++)
			if ((osiselinfo.nds_iftypes & (1 << i)) != 0)
				break ;

		if (t_pid != getpid() || t_switch[i].t_refcnt == 0)
			_t_error = T_WSEQUENCE;
		else
			result = ((*t_switch[i].t_conrq) (tref,
				 (union t_address *) pa_osiaddr,
				 (union t_address *) my_osiaddr, opt));
	}

out:
	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else
		t_trc(T_FRETOK, T_FCONRQ, tref);
	return (result);
}

/*
 * T-CONNECTION.indication
 */
t_conin (tref, toaddr, fromaddr, opt)
int	*tref;
union t_address	*toaddr, *fromaddr;
t_opt1	*opt;
{

	int	result = T_ERROR;
	int	temp = 0;
	int	i = T_MAPTREF(*tref);

	if (opt != NULL)
		temp = ((struct t_optc1 *) opt)->t_udatal << 16;
		
	t_trc(T_FCONIN, T_TRSTART, tref, NULL, NULL, opt, temp);


	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_conin)(tref, toaddr, fromaddr, opt));

	t_trc(T_FCONIN, 0, tref, toaddr, fromaddr, opt, temp);
	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	return (result);
}

/*
 * T_CONNECTION.response
 */
t_conrs (tref, opt)
int 	*tref;
t_opt13	*opt;
{
	int	result = T_ERROR;
	int	i = T_MAPTREF(*tref);

	t_trc(T_FCONRS, 0, tref, opt);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_conrs)(tref, opt));

	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else
		t_trc(T_FRETOK,T_FCONRS);
	return (result);
}

/*
 * T-CONNECTION.confirmation
 */
t_concf (tref, opt)
int 	*tref;
t_opt1	*opt;
{
	int	result = T_ERROR;
	int	temp = 0;
	int	i = T_MAPTREF(*tref);

	if (opt != NULL)
		temp = ((struct t_optc1 *) opt)->t_udatal << 16;
	t_trc(T_FCONCF, T_TRSTART, tref, opt, temp);

	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_concf)(tref, opt));

	t_trc(T_FCONCF, 0, tref, opt, temp);
	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else
		t_trc(T_FRETOK, T_FCONCF);
	return (result);
}

/*---------------------------------------------------------------------------*
 * T-REDIRECT
 *---------------------------------------------------------------------------*/

/*
 * T-REDIRECT.request
 */
t_redrq (tref, pid, opt)
int	*tref, *pid;
t_opt12	*opt;
{
	int	result = T_ERROR;
	int	i = T_MAPTREF(*tref);

	t_trc(T_FREDRQ, 0, tref, pid, opt);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_redrq)(tref, pid, opt)); 

	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else
		t_trc(T_FRETOK,T_FREDRQ);
	return (result);
}

/*
 * T-REDIRECT.indication
 */
t_redin (tref, pid, opt)
int	*tref, *pid;
t_opt23	*opt ;
{
	int	result = T_ERROR;
	int	temp = 0;
	int	i = T_MAPTREF(*tref);

	if (opt != NULL)
		temp = ((struct t_optc2 *) opt)->t_udatal << 16;
	t_trc(T_FREDIN, T_TRSTART, tref, pid, opt, temp);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_redin)(tref, pid, opt)); 

	t_trc(T_FREDIN, 0, tref, pid, opt, temp);
	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else
		t_trc(T_FRETOK, T_FREDIN);
	return (result);
}

/*---------------------------------------------------------------------------*
 * T-DISCONNECTION
 *---------------------------------------------------------------------------*/

/*
 * T-DISCONNECTION.request
 */
t_disrq (tref, opt)
int	*tref;
t_opt2	*opt;
{
	int	result = T_ERROR;
	int	i = T_MAPTREF(*tref);

	t_trc(T_FDISRQ, 0, tref, opt);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_disrq)(tref, opt));

	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else
		t_trc(T_FRETOK,T_FDISRQ);
	return (result);
}

/*
 * T-DISCONNECTION.indication
 */
t_disin (tref, reason, opt)
int	*tref, *reason;
t_opt2	*opt;
{
	int	result = T_ERROR;
	int	temp = 0;
	int	i = T_MAPTREF(*tref);

	if (opt != NULL)
		temp = ((struct t_optc2 *) opt)->t_udatal << 16;
	t_trc(T_FDISIN, T_TRSTART, tref, reason, opt, temp);

	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_disin)(tref, reason, opt));

	t_trc(T_FDISIN, 0, tref, reason, opt, temp);
	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else
		t_trc(T_FRETOK, T_FDISIN);
	return (result);
}

/*---------------------------------------------------------------------------*
 * T-DATA, T-FLOWCONTROL
 *---------------------------------------------------------------------------*/

/*
 * T-DATA.request
 */
t_datarq (tref, datap, datal, chain)
int	*tref, *datal, *chain;
char	*datap;
{
	int	result = T_ERROR;
	int	i = T_MAPTREF(*tref);

	t_trc(T_FDATARQ, 0, tref, datap, *datal, *chain);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_datarq)(tref, datap, datal, chain)); 

	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else if (result == T_DATASTOP || result == T_OK)
		t_trc(T_FRETOK, T_FDATARQ, result);
	return (result);
}

/*
 * T-DATA.request
 */
t_vdatarq(tref, vdata, vcnt, chain)
int	*tref, *vcnt, *chain;
struct t_data	*vdata;
{
	int	result = T_ERROR;
	int	i = T_MAPTREF(*tref);

	t_trc(T_FVDATARQ, 0, tref, vdata, *vcnt, *chain);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_vdatarq)(tref, vdata, vcnt, chain)); 

	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else if (result == T_DATASTOP || result == T_OK)
		t_trc(T_FRETOK, T_FVDATARQ, result);
	return (result);
}

/*
 * T-DATA.indication
 */
t_datain (tref, datap, datal, chain)
int	*tref, *datal, *chain;
char	*datap;
{
	int	result = T_ERROR;
	int	temp = 0;
	int	i = T_MAPTREF(*tref);

	temp = *datal << 16;
	t_trc(T_FDATAIN, T_TRSTART, tref, datap, temp, *chain);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_datain)(tref, datap, datal, chain)); 

	if (result == T_ERROR) {
		t_trc(T_FDATAIN, T_TRNFLSH, tref, datap, *datal, *chain);
		t_trc(T_FRETERR, _t_error);
	} else {
		temp |= *datal & 0xffff;
		t_trc(T_FDATAIN, T_TRFLSH, tref, datap, temp, *chain);
		if (result >= T_OK)
			t_trc(T_FRETOK, T_FDATAIN, result);
	}
	return (result);
}

t_vdatain (tref, vdata, vcnt, chain)
int	*tref, *vcnt, *chain;
struct t_data	*vdata;
{
	int	result = T_ERROR;
	int	j = T_MAPTREF(*tref);
	struct t_data t_data[T_VCNT];
	int	i;

	t_trc(T_FVDATAIN, T_TRSTART, tref, vdata, *vcnt, *chain);
	if (t_pid != getpid() || j < 0 || t_switch[j].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else {
		for (i = 0; i < *vcnt && i < T_VCNT; i++) {
			t_data[i].t_datap = vdata[i].t_datap;
			t_data[i].t_datal = vdata[i].t_datal << 16;
		}
		result = ((*t_switch[j].t_vdatain)(tref, vdata, vcnt, chain)); 
	}

	if (result == T_ERROR) {
		t_trc(T_FVDATAIN, T_TRNFLSH, tref, vdata, *vcnt, *chain);
		t_trc(T_FRETERR, _t_error);
	} else {
		for (i = 0; i < *vcnt; i++)
			t_data[i].t_datal |= vdata[i].t_datal & 0xffff;
		t_trc(T_FVDATAIN, T_TRFLSH, tref, t_data, *vcnt, *chain);
		if (result >= T_OK)
			t_trc(T_FRETOK, T_FVDATAIN, result);
	}
	return (result);
}

/*
 * T-FLOWCONTROL.release
 */
t_datago (tref)
int	*tref;
{
	int	result = T_ERROR;
	int	i = T_MAPTREF(*tref);

	t_trc(T_FDATAGO, 0, tref);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_datago)(tref)); 

	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else
		t_trc(T_FRETOK,T_FDATAGO);
	return (result);
}

/*
 * T-FLOWCONTROL.request
 */
t_datastop (tref)
int	*tref;
{
	int	result = T_ERROR;
	int	i = T_MAPTREF(*tref);

	t_trc(T_FDATASTOP, 0, tref);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_datastop)(tref)); 

	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else
		t_trc(T_FRETOK,T_FDATASTOP);
	return (result);
}

/*---------------------------------------------------------------------------*
 * T-EXPEDITED-DATA, T-EXPEDITED-FLOWCONTROL
 *---------------------------------------------------------------------------*/

/*
 * T-EXPEDITED-DATA.request
 */
t_xdatrq (tref, datap, datal)
int	*tref, *datal;
char	*datap;
{
	int	result = T_ERROR;
	int	i = T_MAPTREF(*tref);

	t_trc(T_FXDATRQ, 0, tref, datap, *datal);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_xdatrq)(tref, datap, datal));

	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else if (result == T_XDATSTOP || result == T_OK)
		t_trc(T_FRETOK, T_FXDATRQ, result);
	return (result);
}

/*
 * T-EXPEDITED-DATA.indication
 */
t_xdatin (tref, datap, datal)
int	*tref, *datal;
char	*datap;
{
	int	result = T_ERROR;
	int	temp = 0;
	int	i = T_MAPTREF(*tref);

	temp = *datal << 16;
	t_trc(T_FXDATIN, T_TRSTART, tref, datap, *datal);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_xdatin)(tref, datap, datal));

	if (result == T_ERROR) {
		t_trc(T_FXDATIN, T_TRNFLSH, tref, datap, *datal);
		t_trc(T_FRETERR, _t_error);
	} else {
		temp |= *datal & 0xffff;
		t_trc(T_FXDATIN, T_TRFLSH, tref, datap, temp);
		if (result >= T_OK)
			t_trc(T_FRETOK, T_FXDATIN, result);
	}
	return (result);
}

/*
 * T-FLOWCONTROL.release
 */
t_xdatgo (tref)
int	*tref;
{
	int	result = T_ERROR;
	int	i = T_MAPTREF(*tref);

	t_trc(T_FXDATGO, 0, tref);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_xdatgo)(tref));

	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else
		t_trc(T_FRETOK,T_FXDATGO);
	return (result);
}

/*
 * T-FLOWCONTROL.request
 */
t_xdatstop (tref)
int	*tref;
{
	int	result = T_ERROR;
	int	i = T_MAPTREF(*tref);

	t_trc(T_FXDATSTOP, 0, tref);
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0)
		_t_error = T_WSEQUENCE;
	else
		result = ((*t_switch[i].t_xdatstop)(tref));

	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else
		t_trc(T_FRETOK,T_FXDATSTOP);
	return (result);
}

/*---------------------------------------------------------------------------*
 * T-EVENT, T-INFO, T-ERROR
 *---------------------------------------------------------------------------*/

/*
 * T-EVENT
 *
 * All connections must be operated simultaneously.
 * The individual connections are requested in rotation (round robin),
 * to make it as easy as possible to operate all connections simultaneously.
 * The reference counter in t_switch[] ensures that a (illegal
 * message T_WSEQUENCE) connection is not called illegally, for which
 * there is no login.
 * The strategy is as follows:
 *      Requests in rotation of logged on connection with T_CHECK
 *      T_WAIT  (time monitored) wait with select() for a result
 *      new request in rotation of logged on connection with T_CHECK
 */
t_event (tref, cmode, opt)
int	*tref, cmode;
t_opte	*opt;
{
	static i = 0;                      /* for rotary requests */
	static  fd_set krfds,kwfds,kxfds;  /* local copy for callback */
	static  int cbret;                 /* return value of callback routine */
	int	result = T_ERROR, j;
	/* nfds = min(open_max,FD_SETSIZE) */
	int	nfds = ((open_max > FD_SETSIZE) ? FD_SETSIZE : open_max);
	struct timeval	*tim = &t;
	fd_set	wfds, rfds, xfds;
 
	t_trc(T_FEVENT, 0, tref, cmode, opt);
	if (t_pid != getpid() || t_attcnt == 0) {
		_t_error = T_WSEQUENCE;
		goto out;
	}
	if (cmode != T_CHECK && cmode != T_WAIT) {
		_t_error = T_WPARAMETER;
		goto out;
	}

	/*
	 * T_CHECK all connection to see whether there is an event.
	 * If all logged on connections return T_NOEVENT, we are finished
	 * with T_CHECK.
	 */
	result = T_NOEVENT;
	for (j = 0; j < T_NSW; j++) {	/* max. T_NSW Durchgaenge */
		i = ++i % T_NSW;
		if (t_switch[i].t_refcnt != 0) {
			t_trc(T_DEVENT, i, T_CHECK);

			result = ((*t_switch[i].t_event)(tref, T_CHECK, opt));

			if (result != T_NOEVENT)
				goto loopend;
		}
	}
loopend:
	if (t_evcnt == 1) {
		_t_error = T_CBRECURSIVE;
		goto out;
	}

	if ((cmode == T_CHECK) && (t_cb_routine == (int(*)(int,fd_set*,fd_set*,fd_set*,struct timeval*,void*))NULL))
		goto out;

	if (((result) || (cmode == T_CHECK)) &&
	     (t_cb_routine != (int(*)(int,fd_set*,fd_set*,fd_set*,struct timeval*,void*)) NULL)) {
		/* Only copies of our file descriptors are provided
		   for the callback routine. */
		rfds = t_rfds;
		wfds = t_wfds;
		xfds = t_xfds;
		memcpy ((char *)&krfds, (char *)&rfds, sizeof(fd_set));
		memcpy ((char *)&kwfds, (char *)&wfds, sizeof(fd_set));
		memcpy ((char *)&kxfds, (char *)&xfds, sizeof(fd_set));
		t_trc(T_FCBRES, 0, nfds, &krfds, &kwfds, &kxfds, tim, 0, 0);
		t_evcnt=1;
		t.tv_sec=0; /* only check */
		cbret=(t_cb_routine(nfds, &krfds, &kwfds, &kxfds, tim, usrptr));
		t_evcnt=0;
		t_trc(T_FCBRES, 1, nfds, &krfds, &kwfds, &kxfds, tim, cbret, errno);
		goto out;
	}


	if (result)
		goto out;
/* Branch-in for retries of LAN1 flow control */
retry:

	/*
	 * We must wait for an event here. This occurs with
	 * select(), by specifying the fildesc to be selected
	 * and a monitoring time.
	 *
	 * For T_WAIT, set timeout from t_event option in select argument.
	 * T_WAIT with timeout T_NO (no waits) complies with T_CHECK.
	 */
	if (opt != NULL) {
		if (opt->opte1.t_timeout == T_NOLIMIT)
			tim = NULL;             /* constant wait */
		else if (opt->opte1.t_timeout == T_NO) 
			goto out;               /* no wait */
		else
			t.tv_sec = opt->opte1.t_timeout; /* x sec wait */
	} else
		tim = NULL;                     /* constant wait */

	/*
	 * A WAITING select() now occurs. Timeout returns T_NOEVENT.
	 * After interruption by a signal, a CHECK is carried out again.
	 * All active fildescs are in GLOBAL t_[rwx]fds, the individual
	 * connections maintain these in compliance with the respective state.
	 * Copies are used, as select() uses them as a return value.
	 */
	rfds = t_rfds;
	wfds = t_wfds;
	xfds = t_xfds;

	if (t_cb_routine != (int(*)(int,fd_set*,fd_set*,fd_set*,struct timeval*,void*))NULL ) {
		/* Only copies of our file descriptors are provided for
		   the callback routine. */
		memcpy ((char *)&krfds, (char *)&rfds, sizeof(fd_set));
		memcpy ((char *)&kwfds, (char *)&wfds, sizeof(fd_set));
		memcpy ((char *)&kxfds, (char *)&xfds, sizeof(fd_set));
		t_trc(T_FCBRES, 0, nfds, &krfds, &kwfds, &kxfds, tim, 0, 0);
		t_evcnt=1;
		cbret=(t_cb_routine(nfds, &krfds, &kwfds, &kxfds, tim, usrptr));
		t_evcnt=0;
		t_trc(T_FCBRES, 1, nfds, &krfds, &kwfds, &kxfds, tim, cbret, errno);
		switch (cbret) {
			case T_NOEVENT: {
				/* These was no event */
				result = T_NOEVENT;
				goto out;
				/* NOTREACHED */
			}
			case T_TSEVENT: {
				/* There was no TS event */
				t_sfds = select(nfds, &rfds, &wfds, &xfds, 0);
				if (t_sfds <= 0) {
					/* callback routine returned a
					   TS_EVENT but there was none */
					result = T_NOEVENT;
					goto out;
				}
				break;
			}
			case T_USEREVENT: {
				/* There was no TS event */
				result = T_NOEVENT;
				goto out;
				/* NOTREACHED */
			}
		}
	}
	else {
		t_trc(T_FTCPSEL, 0, nfds, &rfds, &wfds, &xfds, tim, 0, 0);
		t_sfds = select(nfds, &rfds, &wfds, &xfds, tim);
		t_trc(T_FTCPSEL, 1, nfds, &rfds, &wfds, &xfds, tim, t_sfds, errno);
	}

	if (t_sfds == 0) {			/* Timeout */
		result = T_NOEVENT;
		goto out;
	}
	if (t_sfds < 0 && errno != EINTR) {     /* no interruption */
		_t_error = errno;
		result = T_ERROR;
		goto out;
	}

	/*
	 * Here, after (a possibly interrupted) wait, there are one or more,
	 * or no fildescs for selection. A check (in rotation) with T_CHECK,
	 * is only suitable for more specific assessment.
	 * (From an efficienncy point of view, with additional knowledge a
	 * selected fildesc can be DIRECTLY evaluated.)
	 */
	for (j = 0; j < T_NSW; j++) {	/* max. T_NSW passes */
		i = ++i % T_NSW;
		if (t_switch[i].t_refcnt != 0) {
			t_trc(T_DEVENT, i, T_CHECK);

			result = ((*t_switch[i].t_event)(tref, T_CHECK, opt));

			if (result != T_NOEVENT)
				goto out;
		}
	}
		
out:
	if (result == T_RETRY) {
		/* check for more events; timeout = T_NO works as T_CHECK */
		if ((cmode == T_WAIT) &&
		    ((opt == NULL) || (opt->opte1.t_timeout != T_NO)))
			goto retry;
		/* cmode == T_CHECK */
		result = T_NOEVENT;
		t_trc(T_FRETOK, T_FEVENT, tref, result);
		return(result);
	}
	else if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else if (result == T_NOEVENT || opt == NULL)
		t_trc(T_FRETOK, T_FEVENT, tref, result);
	else
		t_trc(T_FRETEVT, 0, tref, result, opt);

	return (result);
}

/*
 * T-ERROR
 * Central error request
 */
t_cmxerror (void)
{
	t_trc(T_FERROR, _t_error);
	return (_t_error);
}

/*
 * T-INFO
 * Informs on current value of relevant CMX parameter
 */
t_info (tref, opt)
int	*tref;
t_opti	*opt;
{
	int	result = T_ERROR;
	int	i = T_MAPTREF(*tref);

	if (!opt)
		return(T_WPARAMETER);

	if (opt->opti3.t_optnr == T_OPTI3) {
	        t_trc(T_FINFO3, T_TRSTART, tref, opt);
	}
	else
		t_trc(T_FINFO, T_TRSTART, tref, opt);
	
	if (t_pid != getpid() || i < 0 || t_switch[i].t_refcnt == 0) {
		_t_error = T_WSEQUENCE;
		goto out;
	}

	/*
	 * T_OPTI2 only permitted from BS2000 compatibility;
	 * Do not return information
	 */

	if (opt->opti2.t_optnr == T_OPTI2) {
	        t_trc(T_FINFO, 0, tref, opt);
		return (T_OK);
	}

	result = ((*t_switch[i].t_info)(tref, opt));
out:
	if (opt->opti3.t_optnr == T_OPTI3) {
		opt->opti3.t_adrsize = 0;
	        t_trc(T_FINFO3, 0, tref, opt);
	}
	else
		t_trc(T_FINFO, 0, tref, opt);

	if (result == T_ERROR)
		t_trc(T_FRETERR, _t_error);
	else
		t_trc(T_FRETOK, T_FINFO);
	return (result);
}

/*
 * T_NCPY
 * copy any data
 */
t_ncpy (register char *p1, register char *p2, register int lng)
{
	while (lng-- > 0)
		*p1++ = *p2++;
}

/*
 * T_SIGNAL
 * Check whether to set CMX signalization for SIGIO and SIGIO handler
 * CMX signalization is set in t_optinit.
 */
static int	sigcnt;

void t_signal(int sigg)
{

#if defined(AIX) || defined(__OSF__)
static struct sigaction this_routine = {SIG_PTR t_signal,0,0};
#else
static struct sigaction this_routine = {0,SIG_PTR t_signal,0};
#endif  /* AIX || __OSF__ */

	t_trc(T_FSIGTERM, ++sigcnt);
	sigaction (t_cmxsig,&this_routine,NULL);
}

/*
 * t_wreq - returns T_WREQUEST
 */
static 
t_wreq(void)
{
	_t_error = T_WREQUEST;
	return (T_ERROR);
} 
/*
 * t_callback presents to CMX a pointer to a call back routine in an
 * application program executing during t_event()
 */

int (*t_callback(int(*routine)(int,fd_set*,fd_set*,fd_set*,struct timeval*,void*)
	,void *usr, void *opt))(int,fd_set*,fd_set*,fd_set*,struct timeval*,void*)
{
	/* local buffer for old value */
static int (*old_routine)(int,fd_set*,fd_set*,fd_set*,struct timeval*,void*); 

	if (t_pid != getpid() || t_attcnt == 0) {
		_t_error = T_WSEQUENCE;
		return ((int (*)(int,fd_set*,fd_set*,fd_set*,struct timeval*,void*))T_ERROR);
	}

	if (opt) {
		_t_error = T_WPARAMETER;
		return ((int (*)(int,fd_set*,fd_set*,fd_set*,struct timeval*,void*))T_ERROR);
	}

  	t_trc(T_FCALLBACK, 0, routine, usr, opt); 
	old_routine = t_cb_routine;	
			/* old value for return local buffers */
	t_cb_routine = routine; /* Add new routine */
  	t_trc(T_FCALLBACK, 1, old_routine, usr, opt); 
	usrptr = usr;   /* save user-pointer */
	return (old_routine);   /* return old value */
}
 
/*
 * t_getccptyp : Determines the type of coupling
 * Selection of connection type from CCP type
 * in LOCAL NAME and in the TRANSPORTADDRESS (cx_type in t_myname or
 * t_partaddr) for indexing t_switch[] with t_attach, t_detach and
 * t_conrq.
 */
static int
t_getccptyp(unsigned short type)
{
	if (type == CX_RFC1006) return (T_INET);
	if (type == CX_LANINET) return (T_INET);
	if (type == CX_EMSNA) return (T_SNA);
#ifdef PCMX
	return (T_XTI);   /* must be T_XTI */
#else
	return (T_COM);
#endif /* PCMX */
}

void	t_addcclst(struct t_cclst *result_cclist)
{
	int i;

	if (cc_result.cl_num == CMX_ALLCCIDS)
		return;

	if (result_cclist->cl_num == CMX_ALLCCIDS) {
		cc_result.cl_num = CMX_ALLCCIDS;
		return;
	}

	/* add to global cc_result */
	for (i=0; i < (int)result_cclist->cl_num; i++) {
		cc_result.cl_id[cc_result.cl_num] = result_cclist->cl_id[i];
		cc_result.cl_num++;
	}

	
	return;
}
