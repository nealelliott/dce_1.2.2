/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPWkspace.c,v $
 * Revision 1.1.35.2  1996/02/18  18:25:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:35  marty]
 *
 * Revision 1.1.35.1  1995/12/08  15:33:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:43  root]
 * 
 * Revision 1.1.33.4  1994/07/06  15:09:11  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:05  marrek]
 * 
 * Revision 1.1.33.3  1994/05/10  16:04:01  marrek
 * 	Bug fixes and I18N for April 1994 submission.
 * 	[1994/05/06  09:59:44  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:22  marrek]
 * 
 * Revision 1.1.33.2  1994/03/23  15:48:47  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:36:53  keutel]
 * 
 * Revision 1.1.33.1  1994/02/22  18:10:21  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:21:39  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:00:55  marrek]
 * 
 * Revision 1.1.31.1  1993/10/14  19:19:34  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:17:44  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:45:36  keutel]
 * 
 * Revision 1.1.2.2  1993/08/10  07:06:29  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:16:38  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:17:10  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:05:31  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:35:57  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:44:20  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:19:25  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:46:34  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsPWkspace.c,v $ $Revision: 1.1.35.2 $ $Date: 1996/02/18 18:25:00 $";
#endif

/***************************************************************
*															   *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG	1991   *
*				 ALL RIGHTS	RESERVED						   *
*															   *
***************************************************************/

/* ****************************************************************** */
/*																	  */
/*	 * COPYRIGHT   (C)	1990 BY	BULL S.A					  */
/*	 * All rights reserved											  */
/*	 *																  */
/*	 * This	software is	furnished under	licence	and	may	be used	only  */
/*	 * in accordance with the terms	of that	licence	and	with the	  */
/*	 * inclusion of	the	above copyright	notice.						  */
/*	 * This	software may not be	provided or	otherwise made available  */
/*	 * to, or used by, any other person. No	title to or	ownership of  */
/*	 * the software	is hereby transferred.							  */
/*																	  */
/* ****************************************************************** */
/*	@(#)dsPWkspace.c	1.13 (BULL S.A)	7/1/92	*/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <xom.h>
#include <alloc.h>
#include <d2dir.h>
#include <xdsiapl.h>
#include <xds.h>
#include <xdsgds.h>
#include <xomip.h>
#include <xdsbdcp.h>
#include <xdsdme.h>
#include <xdssap.h>
#include <xmhdef.h>
#define	XDSINIT
#include <xdsalloc.h>
#include <xdsimport.h>
#include <dsPPsap.h> /* for dsP_get_psap_length() */
#include <dsPWkspace.h>
#include <d27conv.h>

/********************************************************/
/*			static prototypes							*/
/********************************************************/
static void
dsP_free_default_xds_session(void);

#ifdef THREADSAFE
static void
xds_init_routine(void);
#endif /* THREADSAFE */

extern OMP_package	om_package;

/* pointer to the XDS default session object	*/
/* given by	ds_initialise			*/
/* used	at creation	time of	a session object	*/

OMP_mutex	mutex_session;
xds_session	*default_xds_session = 0;
#ifdef THREADSAFE
static pthread_once_t	xds_init_once =		pthread_once_init;
#endif /* THREADSAFE */

/********************************************************/
/*							*/
/*	dsP_free_default_xds_session()			*/
/*		free the memory	allocated for the	*/
/*		initial	values of the XDS session object*/
/*							*/
/********************************************************/
static void
dsP_free_default_xds_session(void)
{
	if (default_xds_session) {
		if (default_xds_session->password.d2_p_pw)
			OM_FREE(default_xds_session->password.d2_p_pw);
		if (default_xds_session->requestor)
			OM_FREE(default_xds_session->requestor);
		if (default_xds_session->dsa_address)
			OM_FREE(default_xds_session->dsa_address);
		if (default_xds_session->dsa_name)
			OM_FREE(default_xds_session->dsa_name);
		if (default_xds_session->auth_info.d2_value)
			OM_FREE(default_xds_session->auth_info.d2_value);
		OM_FREE(default_xds_session);
		default_xds_session	 = 0;
	}
} /* end dsP_free_default_xds_session */


/************************************************************************/
/*																		*/
/*		xom_init_routine() routine										*/
/*																		*/
/************************************************************************/

#ifdef THREADSAFE
static void
xds_init_routine(void)
{
	if (pthread_mutex_init(&mutex_session, pthread_mutexattr_default) < 0)
		omP_error("mutex_init: %s",		strerror(errno));
#if	0 /* CC	18.6.93	OS/2 problem with fast mutex */
	pthread_mutexattr_t	attr;

	if (pthread_mutexattr_create(&attr)	 < 0)
		omP_error("pthread_mutexattr_create: %s", strerror(errno));
	if (pthread_mutexattr_setkind_np(&attr,		MUTEX_FAST_NP) <  0)
		omP_error("pthread_mutexattr_setkind_np: %s", strerror(errno));
	if (pthread_mutex_init(&mutex_session, attr)	 < 0)
		omP_error("pthread_mutex_init: %s",		strerror(errno));
	if (pthread_mutexattr_delete(&attr)	 < 0)
		omP_error("pthread_mutexattr_delete: %s", strerror(errno));
#endif /* 0	*/

	/* set up thread-specific data */
	{ extern pthread_key_t d21_old_cancel_key; /* declared in dua/d21common.c */

		pthread_keycreate(&d21_old_cancel_key,NULL);
	}

	{
		signed32 result;
		if ((result = d27_init_conv_tables()) != D2_NOERROR) {
			omP_error("d27_init_conv_tables:  %d", result);
		} /* endif */
	}

} /* end xds_init_routine */


#endif /* THREADSAFE */

/********************************************************/
/*							*/
/*	dsP_init_switch	called by omP_initialise()		*/
/*		initialize the pointer to the		*/
/*		initial	values of the session object	*/
/*							*/
/********************************************************/
OM_return_code
dsP_init_switch(
	const OMX_workspace workspace,
	xds_session *default_session)
{
	OM_return_code	ret	 = OM_SUCCESS;
	OM_boolean	while_flag = OM_TRUE;

#ifdef THREADSAFE
	OM_sint	state;	/* state of	thread asynchronous	cancelability */
	/* call	the	pthread_once() to create the mutex to protect the
	 * default_xds_session pointer
	 */
	if ((state = pthread_setasynccancel(CANCEL_OFF)) < 0)
		omP_error("pthread_setasynccancel: %s",		strerror(errno));
	if (pthread_once(&xds_init_once, xds_init_routine) < 0)
		omP_error("pthread_once_t: %s",		strerror(errno));
	if (pthread_setasynccancel(state) <		0)
		omP_error("pthread_setasynccancel: %s",		strerror(errno));
#endif /* THREADSAFE */
	/* lock	the	mutex on default_xds_session */
	MUTEX_LOCK(&mutex_session, "init_session");
	if (default_xds_session)
		dsP_free_default_xds_session();
	while (	while_flag && ret == OM_SUCCESS	) {
		if (!(default_xds_session =		(void *)OM_MALLOC(sizeof(xds_session)))) {
			ret	 = OM_MEMORY_INSUFFICIENT;
			break;
		}
		memset(default_xds_session,		0, sizeof(xds_session));
		if (default_session->dsa_name) {
			if (!( default_xds_session->dsa_name =	(void *)OM_MALLOC( strlen((char
				*)default_session->dsa_name)
				 + 1) )) {
				ret	 = OM_MEMORY_INSUFFICIENT;
				break;
			}
			memcpy(default_xds_session->dsa_name, (char
			*)(default_session->dsa_name), strlen((char
			*)default_session->dsa_name) + 1);
		}

		if (default_session->dsa_address) {
			if (!( default_xds_session->dsa_address	 =	(void
				*)OM_MALLOC( dsP_get_psap_length(default_session->dsa_address))))	 {
				ret	 = OM_MEMORY_INSUFFICIENT;
				break;
			}
			memcpy(default_xds_session->dsa_address, (char *)(default_session->dsa_address),
				 dsP_get_psap_length(default_session->dsa_address));
		}

		if (default_session->requestor)	 {
			if (!( default_xds_session->requestor =	 (void *)OM_MALLOC(	strlen((char
				*)default_session->requestor)
				 + 1) )) {
				ret	 = OM_MEMORY_INSUFFICIENT;
				break;
			}
			memcpy(default_xds_session->requestor, (char *)(default_session->requestor),
				 strlen((char
			*)default_session->requestor) +		1);
		}

		if (default_session->password.d2_p_pw) {
			if (!( default_xds_session->password.d2_p_pw =
                       (void *)OM_MALLOC(default_session->password.d2_p_len))) {
				ret	 = OM_MEMORY_INSUFFICIENT;
				break;
			}
			memcpy(default_xds_session->password.d2_p_pw,
                   (char *)(default_session->password.d2_p_pw),
                   default_session->password.d2_p_len);
			default_xds_session->password.d2_p_len =
                                             default_session->password.d2_p_len;
		}

		default_xds_session->dir_id	 = default_session->dir_id;

		default_xds_session->auth_mech = default_session->auth_mech;

		if (default_session->auth_info.d2_value) {
			if (!( default_xds_session->auth_info.d2_value =
                       (void *)OM_MALLOC(default_session->auth_info.d2_size))) {
				ret	 = OM_MEMORY_INSUFFICIENT;
				break;
			}
			memcpy(default_xds_session->auth_info.d2_value,
                   (char *)(default_session->auth_info.d2_value),
                   default_session->auth_info.d2_size);
			default_xds_session->auth_info.d2_size =
                                             default_session->auth_info.d2_size;
		}


		while_flag = OM_FALSE;
	}
	if (ret	 !=	OM_SUCCESS)
		dsP_free_default_xds_session();

	/* unlock the mutex	on default_xds_session */
	MUTEX_UNLOCK(&mutex_session,"init_session");
	return(ret);
} /* end dsP_init_switch */


#include <xdspackage.h>
