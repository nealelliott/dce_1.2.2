/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omEntry.c,v $
 * Revision 1.1.8.2  1996/02/18  18:25:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:54  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:40:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:04  root]
 * 
 * Revision 1.1.6.2  1994/07/06  15:09:28  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:26:07  marrek]
 * 
 * Revision 1.1.6.1  1994/02/22  18:11:52  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:07:51  marrek]
 * 
 * Revision 1.1.4.2  1993/08/10  07:23:21  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:26:37  marrek]
 * 
 * Revision 1.1.2.3  1992/12/30  21:18:21  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:07:09  bbelch]
 * 
 * Revision 1.1.2.2  1992/11/30  08:41:33  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:47:58  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omEntry.c,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 18:25:31 $";
#endif

/***************************************************************
*															   *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG	1991   *
*				 ALL RIGHTS	RESERVED						   *
*															   *
***************************************************************/

/**********************************************************************/
/*				 COPYRIGHT (C) BULL	S.A. 1990 1991 1992				  */
/*						  All Rights Reserved						  */
/**********************************************************************/
/*																	  */
/*	 * This	software is	furnished under	licence	and	may	be used	only  */
/*	 * in accordance with the terms	of that	licence	and	with the	  */
/*	 * inclusion of	the	above copyright	notice.						  */
/*	 * This	software may not be	provided or	otherwise made available  */
/*	 * to, or used by, any other person. No	title to or	ownership of  */
/*	 * the software	is hereby transferred.							  */
/*																	  */
/**********************************************************************/
/*	@(#)omEntry.c	1.0	(BULL S.A) 8/25/92	*/

/****************************************************************/
/*								*/
/*	This module	contains all XOM entry point functions	*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <stdio.h>
#include <xom.h>
#include <xomip.h>
#include <alloc.h>
#include <xomtrace.h>
#include <omEntry.h>

#ifdef THREADSAFE
static pthread_once_t	xom_init_once =	pthread_once_init;
static OMP_mutex	mutex_trace;
#endif /* THREADSAFE */
static OM_boolean mutex_trace_initialised = OM_FALSE;



/************************************************************************/
/*																		*/
/*		omP_encode() function											*/
/*																		*/
/************************************************************************/


OM_return_code
omP_encode (
	OM_private_object	original,
	OM_object_identifier	rules,
	OM_private_object		*encoding)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */

	if (!OMP_PRIVATE(original))
		return(OM_NOT_PRIVATE);

	/* check the validity of the workspace */
	if ( OMP_WORKSPACE(original)->status !=		WORKSPACE_INIT)	 {
		*encoding =		(OM_private_object)	NULL ;
		return(OM_NO_SUCH_WORKSPACE);
	}
	MUTEX_LOCK(OMP_WORKSPACE(original)->mutex_wk, "om_encode");
	ret	 = omP_xencode (	original, rules, encoding );
	TRC_ENCODE(*encoding);
	MUTEX_UNLOCK(OMP_WORKSPACE(original)->mutex_wk,		"om_encode");

	return(ret);
} /* end omP_encode	 */


/************************************************************************/
/*																		*/
/*		omP_decode() function											*/
/*																		*/
/************************************************************************/

OM_return_code
omP_decode (
	OM_private_object		encoding,
	OM_private_object		*original)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */

	if (!OMP_PRIVATE(encoding))
		return(OM_NOT_PRIVATE);

	/* Is workspace	initialised? */
	if ( OMP_WORKSPACE(encoding)->status !=		WORKSPACE_INIT)	 {
		return (OM_NO_SUCH_WORKSPACE);
	}
	MUTEX_LOCK(OMP_WORKSPACE(encoding)->mutex_wk, "om_decode");
	ret	 = omP_xdecode (	encoding, original );
	TRC_DECODE(*original);
	MUTEX_UNLOCK(OMP_WORKSPACE(encoding)->mutex_wk,		"om_decode");

	return(ret);
} /* end omP_decode	 */


/************************************************************************/
/*																		*/
/*		omP_copy() function												*/
/*																		*/
/************************************************************************/

OM_return_code
omP_copy(
	OM_private_object			original,
	OM_workspace				workspace,
	OM_private_object			*copy)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	/* Check the workspace in which	the	copy is	to be created.	*/
	if (((OMX_workspace) workspace)->status	 !=	WORKSPACE_INIT )
		return(OM_NO_SUCH_WORKSPACE);
	MUTEX_LOCK(((OMX_workspace)workspace)->mutex_wk, "om_copy");
	ret	 = omP_xcopy(original, workspace, copy);
	TRC_COPY(*copy);
	MUTEX_UNLOCK(((OMX_workspace)workspace)->mutex_wk, "om_copy");

	return(ret);
} /* end omP_copy */


/************************************************************************/
/*																		*/
/*		omP_copy_value() function										*/
/*																		*/
/************************************************************************/

OM_return_code
omP_copy_value(
	OM_private_object			source,
	OM_type						source_type,
	OM_value_position			source_value_position,
	OM_private_object			destination,
	OM_type						destination_type,
	OM_value_position			destination_value_position)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */

	if (!OMP_PRIVATE(destination))
		return(OM_NOT_PRIVATE);

	if (!OMP_PRIVATE(source))
		return(OM_NOT_PRIVATE);

	if (OMP_WORKSPACE(destination)->status != WORKSPACE_INIT )
		return(OM_NO_SUCH_WORKSPACE);
	MUTEX_LOCK(OMP_WORKSPACE(destination)->mutex_wk, "om_copy_value");
	ret	 = omP_xcopy_value(source, source_type,	source_value_position,
		 destination, destination_type,		destination_value_position);
	MUTEX_UNLOCK(OMP_WORKSPACE(destination)->mutex_wk, "om_copy_value");

	return(ret);
} /* end omP_copy_value	*/


/************************************************************************/
/*																		*/
/*		omP_create() function											*/
/*																		*/
/************************************************************************/

OM_return_code
omP_create(
	OM_object_identifier	class,
	OM_boolean				initialise,
	OM_workspace			workspace,
	OM_private_object		*object)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	/* Is workspace	initialised? */
	if (((OMX_workspace) workspace)->status	 !=	WORKSPACE_INIT)
		return (OM_NO_SUCH_WORKSPACE);
	MUTEX_LOCK(((OMX_workspace)workspace)->mutex_wk, "om_create");
	ret	 = omP_xcreate(class, initialise, workspace,	object);
	TRC_CREATE(*object);
	MUTEX_UNLOCK(((OMX_workspace)workspace)->mutex_wk, "om_create");

	return(ret);
} /* end omP_create	*/


/************************************************************************/
/*																		*/
/*		omP_create_handle()	function									*/
/*																		*/
/************************************************************************/

OM_return_code
omP_create_handle(
	OM_object_identifier	class,
	OM_workspace			workspace,
	void *priv_object,
	OM_private_object		*object)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	/* Is workspace	initialised? */
	if ( ((OMX_workspace )workspace	 ==	(OMX_workspace )0) ||  (((OMX_workspace	)workspace)->status	!=
		WORKSPACE_INIT)	)
		return (OM_NO_SUCH_WORKSPACE);
	MUTEX_LOCK(((OMX_workspace)workspace)->mutex_wk, "om_create_handle");
	ret	 = omP_xcreate_handle(class,	workspace, priv_object,	object);
	MUTEX_UNLOCK(((OMX_workspace)workspace)->mutex_wk, "om_create_handle");

	return(ret);
} /* end omP_create_handle */


/************************************************************************/
/*																		*/
/*		omP_delete() function											*/
/*																		*/
/************************************************************************/

OM_return_code
omP_delete(
	OM_object  subject)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	OMP_mutex		 * mutex_save;
	if (OMP_PRIVATE(subject)) {

		/* Check if	the	workspace status is	initialized	*/
		if (OMP_WORKSPACE(subject)->status != WORKSPACE_INIT)
			return(OM_NO_SUCH_WORKSPACE);
	}
	/* save	mutex before deleting the object */
	/* we can arrive with INVALID_WORKSPACE	to delete a	handle */
	if (mutex_save = OMP_WORKSPACE(subject)->mutex_wk) {
		MUTEX_LOCK(OMP_WORKSPACE(subject)->mutex_wk, "om_delete");
	}
	TRC_DELETE(subject);
	ret	 = omP_xdelete(subject);
	if (mutex_save)	 {
		MUTEX_UNLOCK(mutex_save, "om_delete");
	}

	return(ret);
} /* end omP_delete	*/


/************************************************************************/
/*																		*/
/*		omP_get() function												*/
/*																		*/
/************************************************************************/

OM_return_code
omP_get(
	OM_private_object		original,
	OM_exclusions			exclusions,
	OM_type_list			included_types,
	OM_boolean				local_strings,
	OM_value_position		initial_value,
	OM_value_position		limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */

	if (!OMP_PRIVATE(original))
		return(OM_NOT_PRIVATE);

	if (OMP_WORKSPACE(original)->status	 !=	WORKSPACE_INIT)
		return(OM_NO_SUCH_WORKSPACE);
	MUTEX_LOCK(OMP_WORKSPACE(original)->mutex_wk, "om_get");
	ret	 = omP_xget(original, exclusions, included_types, local_strings,
		 initial_value,		limiting_value,		copy, total_number);
	TRC_GET(*copy, *total_number);
	MUTEX_UNLOCK(OMP_WORKSPACE(original)->mutex_wk,		"om_get");

	return(ret);
} /* end omP_get */


/************************************************************************/
/*																		*/
/*		omP_instance() function											*/
/*																		*/
/************************************************************************/

OM_return_code
omP_instance(
	OM_object					subject,
	OM_object_identifier		class,
	OM_boolean					*instance)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	/*
	 * Check the subject argument.
	 */
	if ( subject ==		NULL )
		return(OM_POINTER_INVALID);

	if (!((OMP_SPUB(subject)) || (OMP_PRIVATE(subject))) )
		return(OM_NO_SUCH_OBJECT);

	MUTEX_LOCK(OMP_WORKSPACE(subject)->mutex_wk, "om_instance");
	ret	 = omP_xinstance(subject, class,	instance);
	MUTEX_UNLOCK(OMP_WORKSPACE(subject)->mutex_wk, "om_instance");

	return(ret);
} /* end omP_instance */


/************************************************************************/
/*																		*/
/*		omP_put() function												*/
/*																		*/
/************************************************************************/

OM_return_code
omP_put(
	OM_private_object		destination,
	OM_modification			modification,
	OM_object				source,
	OM_type_list			included_types,
	OM_value_position		initial_value,
	OM_value_position		limiting_value)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */

	if (!OMP_PRIVATE(destination))
		return(OM_NOT_PRIVATE);

	if (OMP_WORKSPACE(destination)->status != WORKSPACE_INIT )
		return(OM_NO_SUCH_WORKSPACE);
	MUTEX_LOCK(OMP_WORKSPACE(destination)->mutex_wk, "om_put");
	ret	 = omP_xput(destination,	modification, source, included_types,
		 initial_value,		limiting_value);
	MUTEX_UNLOCK(OMP_WORKSPACE(destination)->mutex_wk, "om_put");

	return(ret);
} /* end omP_put */


/************************************************************************/
/*																		*/
/*		omP_read() function												*/
/*																		*/
/************************************************************************/

OM_return_code
omP_read(
	OM_private_object			subject,
	OM_type						type,
	OM_value_position			value_position,
	OM_boolean					local_string,
	OM_string_length			*element_position,
	OM_string					*elements)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */

	if (!OMP_PRIVATE(subject))
		return(OM_NOT_PRIVATE);

	/* Check if	the	workspace status is	initialized	*/
	if (OMP_WORKSPACE(subject)->status != WORKSPACE_INIT)
		return(OM_NO_SUCH_WORKSPACE);

	MUTEX_LOCK(OMP_WORKSPACE(subject)->mutex_wk, "om_read");
	ret	 = omP_xread(subject, type,	value_position,	local_string, element_position,
		 elements);
	MUTEX_UNLOCK(OMP_WORKSPACE(subject)->mutex_wk, "om_read");

	return(ret);
} /* end omP_read */


/************************************************************************/
/*																		*/
/*		omP_remove() function											*/
/*																		*/
/************************************************************************/

OM_return_code
omP_remove(
	OM_private_object			subject,
	OM_type						type,
	OM_value_position			initial_value,
	OM_value_position			limiting_value)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */

	if (!OMP_PRIVATE(subject))
		return(OM_NOT_PRIVATE);

	/* Check if	the	workspace status is	initialized	*/
	if (OMP_WORKSPACE(subject)->status != WORKSPACE_INIT)
		return(OM_NO_SUCH_WORKSPACE);

	MUTEX_LOCK(OMP_WORKSPACE(subject)->mutex_wk, "om_remove");
	ret	 = omP_xremove(subject,	type, initial_value,	limiting_value);
	MUTEX_UNLOCK(OMP_WORKSPACE(subject)->mutex_wk, "om_remove");

	return(ret);
} /* end omP_remove	*/


/************************************************************************/
/*																		*/
/*		omP_write()	function											*/
/*																		*/
/************************************************************************/

OM_return_code
omP_write(
	OM_private_object			subject,
	OM_type						type,
	OM_value_position			value_position,
	OM_syntax					syntax,
	OM_string_length			*element_position,
	OM_string					elements)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */

	if (!OMP_PRIVATE(subject))
		return(OM_NOT_PRIVATE);

	/* Check if	the	workspace status is	initialized	*/
	if (OMP_WORKSPACE(subject)->status != WORKSPACE_INIT)
		return(OM_NO_SUCH_WORKSPACE);

	MUTEX_LOCK(OMP_WORKSPACE(subject)->mutex_wk, "om_write");
	ret	 = omP_xwrite(subject, type,	value_position,		syntax,	element_position,
		 elements);
	MUTEX_UNLOCK(OMP_WORKSPACE(subject)->mutex_wk, "om_write");

	return(ret);
} /* end omP_write */


/************************************************************************/
/*																		*/
/*		omP_version() function											*/
/*																		*/
/************************************************************************/

OM_return_code
omP_version(
	OM_workspace	workspace,
	OMP_feature	feature_list[])
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	/* Check if	the	workspace status is	initialized	*/
	if (((OMX_workspace) workspace)->status	 !=	WORKSPACE_INIT)
		return(OM_NO_SUCH_WORKSPACE);
	MUTEX_LOCK(((OMX_workspace)workspace)->mutex_wk, "om_version");
	ret	 = omP_xversion(workspace, feature_list);
	MUTEX_UNLOCK(((OMX_workspace)workspace)->mutex_wk, "om_version");

	return(ret);
} /* end omP_version */


/************************************************************************/
/*																		*/
/*		omP_shutdown() function											*/
/*																		*/
/************************************************************************/

void
omP_shutdown(
	OM_workspace	workspace)
{
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	/* Check if	the	workspace status is	initialized	*/
	if (((OMX_workspace) workspace)->status	 !=	WORKSPACE_INIT)
		return;
	MUTEX_LOCK(((OMX_workspace)workspace)->mutex_wk, "om_shutdown");
	omP_xshutdown(workspace);
	MUTEX_UNLOCK(((OMX_workspace)workspace)->mutex_wk, "om_shutdown");

	return;
} /* end omP_shutdown */


/************************************************************************/
/*																		*/
/*		omP_package_present() function									*/
/*																		*/
/************************************************************************/

OM_return_code
omP_package_present	(
	OM_workspace			workspace ,
	OM_object_identifier	package_oid	,
	OM_boolean				*package_present)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	if ( workspace == (OM_workspace) NULL )
		return(OM_POINTER_INVALID);
	if (((OMX_workspace)workspace)->mutex_wk ==		(OMP_mutex *  )	NULL)
		ret	 = omP_xpackage_present(	workspace ,		package_oid	,
			package_present);
	else
	{
		MUTEX_LOCK(((OMX_workspace)workspace)->mutex_wk, "om_package_present");
		ret	 = omP_xpackage_present(	workspace ,		package_oid	,
			package_present);
		MUTEX_UNLOCK(((OMX_workspace)workspace)->mutex_wk, "om_package_present");
	}
	return(ret);
} /* end omP_package_present */


/************************************************************************/
/*																		*/
/*		omP_add_package() function										*/
/*																		*/
/************************************************************************/

OM_return_code
omP_add_package	(
	OM_workspace			workspace ,
	OMP_package				package_feature)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	if ( workspace == (OM_workspace) NULL )
		return(OM_POINTER_INVALID);
	if (((OMX_workspace)workspace)->mutex_wk ==		(OMP_mutex *  )	NULL)
		ret	 = omP_xadd_package(	workspace ,		package_feature);
	else
	{
		MUTEX_LOCK(((OMX_workspace)workspace)->mutex_wk, "om_add_package");
		ret	 = omP_xadd_package(	workspace ,		package_feature);
		MUTEX_UNLOCK(((OMX_workspace)workspace)->mutex_wk, "om_add_package");
	}

	return(ret);
} /* end omP_add_package  */


/************************************************************************/
/*																		*/
/*		omP_delete_package() function									*/
/*																		*/
/************************************************************************/

OM_return_code
omP_delete_package (
	OM_workspace			workspace ,
	OM_object_identifier	package_oid)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	if ( workspace == (OM_workspace) NULL )
		return(OM_POINTER_INVALID);
	MUTEX_LOCK(((OMX_workspace)workspace)->mutex_wk, "om_delete_package");
	ret	 = omP_xdelete_package(	workspace ,	package_oid);
	MUTEX_UNLOCK(((OMX_workspace)workspace)->mutex_wk, "om_delete_package");

	return(ret);
} /* end omP_delete_package	 */


/************************************************************************/
/*																		*/
/*		omP_set_generic_workspace()	function							*/
/*																		*/
/************************************************************************/

OM_return_code
omP_set_generic_workspace (
	OM_workspace	workspace ,
	void *generic_workspace)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	if ( workspace == (OM_workspace) NULL )
		return(OM_POINTER_INVALID);
	MUTEX_LOCK(((OMX_workspace)workspace)->mutex_wk, "om_set_gen_wk");
	ret	 = omP_xset_generic_workspace( workspace	, generic_workspace);
	MUTEX_UNLOCK(((OMX_workspace)workspace)->mutex_wk, "om_set_gen_wk");

	return(ret);
} /* end omP_set_generic_workspace */


/************************************************************************/
/*																		*/
/*		omP_get_generic_workspace()	function							*/
/*																		*/
/************************************************************************/

OM_return_code
omP_get_generic_workspace (
	OM_workspace	workspace ,
	void **generic_workspace)
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	if ( workspace == (OM_workspace) NULL )	 {
		*generic_workspace = (void *) NULL ;
		return(OM_POINTER_INVALID);
	}
	MUTEX_LOCK(((OMX_workspace)workspace)->mutex_wk, "om_get_gen_wk");
	ret	 = omP_xget_generic_workspace( workspace	, generic_workspace);
	MUTEX_UNLOCK(((OMX_workspace)workspace)->mutex_wk, "om_get_gen_wk");

	return(ret);
} /* end omP_get_generic_workspace	*/


/************************************************************************/
/*																		*/
/*		omP_complete_shutdown()	function								*/
/*																		*/
/************************************************************************/

void
omP_complete_shutdown(
	OM_workspace	workspace)
{
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	OMP_mutex		 * mutex_save;
	/* Check if	the	workspace status is	initialized	*/
	if (((OMX_workspace) workspace)->status	 !=	WORKSPACE_INIT)
		return;
	MUTEX_LOCK(((OMX_workspace)workspace)->mutex_wk, "om_complete_shutdown");
	/* save	mutex before freeing the workspace structure */
	mutex_save = ((OMX_workspace)workspace)->mutex_wk;
	omP_xcomplete_shutdown(workspace);
	MUTEX_UNLOCK(mutex_save, "om_complete_shutdown");
#ifdef THREADSAFE
	if (pthread_mutex_destroy(mutex_save) <		0)
		omP_error("mutex_destroy: %s", strerror(errno));
	OM_FREE(mutex_save);

#endif /* THREADSAFE */

	return;
} /* end omP_complete_shutdown */


/************************************************************************/
/*																		*/
/*		omP_negotiate()	function										*/
/*																		*/
/************************************************************************/

OM_return_code
omP_negotiate (
	OM_workspace			workspace ,
	OMP_negotiate_feature	feature_list[])
{
	OM_return_code	ret;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	if ( workspace == (OM_workspace) NULL )
		return(OM_POINTER_INVALID);
	MUTEX_LOCK(((OMX_workspace)workspace)->mutex_wk, "om_negotiate");
	ret	 = omP_xnegotiate( workspace	, feature_list);
	MUTEX_UNLOCK(((OMX_workspace)workspace)->mutex_wk, "om_negotiate");

	return(ret);
} /* end omP_negotiate */


/************************************************************************/
/*																		*/
/*		xom_init_routine() routine										*/
/*																		*/
/************************************************************************/

#ifdef THREADSAFE
static void
xom_init_routine(void)
{
#if 0 /* CC 18.1.94 to avoid warnings */
	pthread_mutexattr_t	attr;
#endif /* 0 */

	/* Set the trace */
	mmgt_set_trace();
	omP_set_trace () ;

	if (pthread_mutex_init(&mutex_trace,pthread_mutexattr_default) < 0) {
		omP_error("pthread_mutex_init: %s",		strerror(errno));
	} else {
		mutex_trace_initialised = OM_TRUE;
	} /* endif */

#if	0 /* CC	18.6.93	*/
	if (pthread_mutexattr_create(&attr)	 < 0)
		omP_error("pthread_mutexattr_create: %s", strerror(errno));
	if (pthread_mutexattr_setkind_np(&attr,		MUTEX_FAST_NP) <  0)
		omP_error("pthread_mutexattr_setkind_np: %s", strerror(errno));

	if (pthread_mutex_init(&mutex_trace, attr) < 0)
		omP_error("pthread_mutex_init: %s",		strerror(errno));
	if (pthread_mutexattr_delete(&attr)	 < 0)
		omP_error("pthread_mutexattr_delete: %s", strerror(errno));
#endif /* 0	*/
} /* end xom_init_routine */


#endif /* THREADSAFE */

/************************************************************************/
/*																		*/
/*		omP_initialize() function										*/
/*																		*/
/************************************************************************/

OM_return_code
omP_initialize(
	struct OMP_switch_functions_body *sw_functions,
	OMP_package		*packages_table,
	OMP_package		*closure_packages_table,
	void *generic_workspace	,
	struct alloc_table *alloc_table,
	void *default_session,
	OM_workspace	*workspace_return)
{
	OM_return_code	ret;
#ifdef	THREADSAFE
	pthread_mutexattr_t	attr;
	OM_sint	state;	/* state of	thread asynchronous	cancelability */

	if ((state = pthread_setasynccancel(CANCEL_OFF)) < 0)
		omP_error("%s -	setasynccancel:	%s", "omP_initialize", strerror(errno));
	if (pthread_once(&xom_init_once, xom_init_routine) < 0)
		omP_error("pthread_once_t: %s",		strerror(errno));
	if (pthread_setasynccancel(state) <		0)
		omP_error("%s -	setasynccancel:	%s", "omP_initialize", strerror(errno));

#else /* THREADSAFE	*/
	/* Set the trace */
	mmgt_set_trace () ;
	omP_set_trace () ;
#endif /* THREADSAFE */
	ret	 = omP_xinitialize(	sw_functions, packages_table, closure_packages_table,
		 generic_workspace , alloc_table, default_session,
		workspace_return);
#ifdef THREADSAFE
    {
	    OMX_workspace	workspace;

	    if (ret	 ==	OM_SUCCESS)	{
		    if (pthread_mutexattr_create(&attr)	 < 0)
			    omP_error("pthread_mutexattr_create: %s", strerror(errno));
		    if (pthread_mutexattr_setkind_np(&attr,		MUTEX_RECURSIVE_NP)	 <
			    0)
			    omP_error("pthread_mutexattr_setkind_np: %s", strerror(errno));
		    workspace =		*((OMX_workspace * )workspace_return);
		    workspace->mutex_wk	 = (OMP_mutex *	)OM_MALLOC(sizeof(OMP_mutex));
		    if (pthread_mutex_init(workspace->mutex_wk,		attr) <	 0)
			    omP_error("pthread_mutex_init: %s",		strerror(errno));
		    if (pthread_mutexattr_delete(&attr)	 < 0)
			    omP_error("pthread_mutexattr_delete: %s", strerror(errno));
	    }
    }
#endif /* THREADSAFE */
	return(ret);
} /* end omP_initialize	*/


/************************************************************************/
/*																		*/
/*		memory management trace	functions (for internal	use	only)		*/
/*																		*/
/************************************************************************/

char	*
mmgt_calloc(
	char *file,
	OM_sint	line,
	OM_sint	n,
	OM_sint	size)
{
	char	*pt;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	MUTEX_LOCK(&mutex_trace, "mmgt_calloc");
	pt = (char *)mmgt_xcalloc( file, line, n, size);
	MUTEX_UNLOCK(&mutex_trace, "mmgt_calloc");
	return(pt);
} /* end mmgt_calloc */


void
mmgt_free (
	char *file,
	OM_sint	line,
	char *buffer)
{
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	if (!mutex_trace_initialised) {
		mmgt_xfree ( file, line, buffer);
	} else {
		MUTEX_LOCK(&mutex_trace,"mmgt_free");
		mmgt_xfree ( file, line, buffer);
		MUTEX_UNLOCK(&mutex_trace, "mmgt_free");
	}
} /* end mmgt_free	*/


char	*
mmgt_malloc(
	char *file,
	OM_sint	line,
	OM_sint	size)
{
	char	*pt;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	if (!mutex_trace_initialised) {
		pt = (char *)mmgt_xmalloc( file, line, size);
	} else {
		MUTEX_LOCK(&mutex_trace,"mmgt_malloc");
		pt = (char *)mmgt_xmalloc( file, line, size);
		MUTEX_UNLOCK(&mutex_trace, "mmgt_malloc");
	}
	return(pt);
} /* end mmgt_malloc */


char	*
mmgt_realloc (
	char *file,
	OM_sint	line,
	char *old,
	OM_sint	size)
{
	char	*pt;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */
	MUTEX_LOCK(&mutex_trace, "mmgt_realloc");
	pt = (char *)mmgt_xrealloc ( file, line, old, size);
	MUTEX_UNLOCK(&mutex_trace, "mmgt_realloc");
	return(pt);
} /* end mmgt_realloc  */


/************************************************************************/
/*																		*/
/*		omP_iw_encode()	function										*/
/*																		*/
/************************************************************************/


OM_return_code
omP_iw_encode (
	OM_private_object	original,
	OM_object_identifier	rules,
	OM_private_object		*encoding)
{
	return(OM_NO_SUCH_OBJECT);
} /* end omP_iw_encode */


/************************************************************************/
/*																		*/
/*		omP_iw_decode()	function										   */
/*																		*/
/************************************************************************/

OM_return_code
omP_iw_decode (
	OM_private_object		encoding,
	OM_private_object		*original)
{
	return(OM_NO_SUCH_OBJECT);
} /* end omP_iw_decode	*/


/************************************************************************/
/*																		*/
/*		omP_iw_copy() function											*/
/*																		*/
/************************************************************************/

OM_return_code
omP_iw_copy(
	OM_private_object			original,
	OM_workspace				workspace,
	OM_private_object			*copy)
{
	return(OM_NO_SUCH_OBJECT);
} /* end omP_iw_copy */


/************************************************************************/
/*																		*/
/*		omP_iw_copy_value()	function									*/
/*																		*/
/************************************************************************/

OM_return_code
omP_iw_copy_value(
	OM_private_object			source,
	OM_type						source_type,
	OM_value_position			source_value_position,
	OM_private_object			destination,
	OM_type						destination_type,
	OM_value_position			destination_value_position)
{
	return(OM_NO_SUCH_OBJECT);
} /* end omP_iw_copy_value */


/************************************************************************/
/*																		*/
/*		omP_iw_create()	function										*/
/*																		*/
/************************************************************************/

OM_return_code
omP_iw_create(
	OM_object_identifier	class,
	OM_boolean				initialise,
	OM_workspace			workspace,
	OM_private_object		*object)

{
	return (OM_NO_SUCH_WORKSPACE);
} /* end omP_iw_create */


/************************************************************************/
/*																		*/
/*		omP_iw_delete()	function										*/
/*																		*/
/************************************************************************/

OM_return_code
omP_iw_delete(
	OM_object  subject)
{
	return(OM_NO_SUCH_OBJECT);
} /* end omP_iw_delete */


/************************************************************************/
/*																		*/
/*		omP_iw_get() function											*/
/*																		*/
/************************************************************************/

OM_return_code
omP_iw_get(
	OM_private_object		original,
	OM_exclusions			exclusions,
	OM_type_list			included_types,
	OM_boolean				local_strings,
	OM_value_position		initial_value,
	OM_value_position		limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	return(OM_NO_SUCH_OBJECT);
} /* end omP_iw_get	*/


/************************************************************************/
/*																		*/
/*		omP_iw_instance() function										*/
/*																		*/
/************************************************************************/

OM_return_code
omP_iw_instance(
	OM_object					subject,
	OM_object_identifier		class,
	OM_boolean					*instance)
{
	return(OM_NO_SUCH_OBJECT);
} /* end omP_iw_instance */


/************************************************************************/
/*																		*/
/*		omP_iw_put() function											*/
/*																		*/
/************************************************************************/

OM_return_code
omP_iw_put(
	OM_private_object		destination,
	OM_modification			modification,
	OM_object				source,
	OM_type_list			included_types,
	OM_value_position		initial_value,
	OM_value_position		limiting_value)

{
	return(OM_NO_SUCH_OBJECT);
} /* end omP_iw_put	*/


/************************************************************************/
/*																		*/
/*		omP_iw_read() function											   */
/*																		*/
/************************************************************************/

OM_return_code
omP_iw_read(
	OM_private_object			subject,
	OM_type						type,
	OM_value_position			value_position,
	OM_boolean					local_string,
	OM_string_length			*element_position,
	OM_string					*elements)
{
	return(OM_NO_SUCH_OBJECT);
} /* end omP_iw_read */


/************************************************************************/
/*																		*/
/*		omP_iw_remove()	function										*/
/*																		*/
/************************************************************************/

OM_return_code
omP_iw_remove(
	OM_private_object			subject,
	OM_type						type,
	OM_value_position			initial_value,
	OM_value_position			limiting_value)
{
	return(OM_NO_SUCH_OBJECT);
} /* end omP_iw_remove */


/************************************************************************/
/*																		*/
/*		omP_iw_write() function											*/
/*																		*/
/************************************************************************/

OM_return_code
omP_iw_write(
	OM_private_object			subject,
	OM_type						type,
	OM_value_position			value_position,
	OM_syntax					syntax,
	OM_string_length			*element_position,
	OM_string					elements)
{
	return(OM_NO_SUCH_OBJECT);
} /* end omP_iw_write */


