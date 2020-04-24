/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xomip.h,v $
 * Revision 1.1.10.2  1996/02/18  23:38:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:13  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:39:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:57  root]
 * 
 * Revision 1.1.8.3  1994/07/06  15:09:22  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:49  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:49:23  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:31  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:00:37  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:10:27  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:21:08  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:25:06  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:00  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:06:29  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:39:31  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:46:07  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:21:00  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:47:43  melman]
 * 
 * $EndLog$
 */
#ifndef _XOMIP_H
#define _XOMIP_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xomip_rcsid[] = "@(#)$RCSfile: xomip.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:38:33 $";
#endif
/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/
/* @(#)xomip.h	10.2.1.1 91/12/19 (SNI) */

/**********************************************************************/
/*               COPYRIGHT (C) BULL S.A. 1990 1991 1992               */
/*                        All Rights Reserved                         */
/**********************************************************************/
/*                                                                    */
/*   * This software is furnished under licence and may be used only  */
/*   * in accordance with the terms of that licence and with the      */
/*   * inclusion of the above copyright notice.                       */
/*   * This software may not be provided or otherwise made available  */
/*   * to, or used by, any other person. No title to or ownership of  */
/*   * the software is hereby transferred.                            */
/*                                                                    */
/**********************************************************************/
/*	@(#)xomip.h	1.29 (Bull S.A) 7/1/92	*/

/**********************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xomip.h                                             */
/*                                                                    */
/* AUTHOR       : Bull S.A.                                           */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XOM                                                 */
/*                                                                    */
/* DOC.-REF.    : OSI-Abstract-Data Manipulation API (XOM) Nov. 1991  */
/*                (formerly "OSI Object Management API (XOM)")        */
/*                XDS/XOM Design Specification August 1990            */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : declarations of functions and macros used           */
/*                internally by XOM                                   */ 
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)@(#)xomip.h	1.19 (Bull S.A) 8/29/91 */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       |    |         */
/*   1.0    | 90-10-01| Original                       |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/**********************************************************************/


#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef THREADSAFE
#include <pthread.h>
#include <dce/cma_stdio.h>
#endif /* THREADSAFE */

#ifdef THREADSAFE
typedef	pthread_mutex_t	OMP_mutex;

/* state of asynchronous cancelability declaration	*/
/* used in MUTEX_LOCK and MUTEX_UNLOCK			*/
#define MUTEX_STATE						\
	int	state;

#define MUTEX_LOCK(MUTEX,STR)					\
{								\
	if ((state = pthread_setasynccancel(CANCEL_OFF)) < 0)	\
		omP_error("%s - setasynccancel: %s",STR, strerror(errno)); \
	if (pthread_mutex_lock(MUTEX) < 0)			\
		omP_error("%s - mutex_lock: %s",STR, strerror(errno)); \
}


#define MUTEX_UNLOCK(MUTEX,STR)					\
{								\
	if (pthread_mutex_unlock(MUTEX) < 0)			\
		omP_error("%s - mutex_unlock: %s",STR, strerror(errno)); \
	if (pthread_setasynccancel(state) < 0)			\
		omP_error("%s - setasynccancel: %s",STR, strerror(errno)); \
}

#else /* THREADSAFE */

typedef	void	OMP_mutex;

#define MUTEX_STATE
#define MUTEX_LOCK(MUTEX,STR)
#define MUTEX_UNLOCK(MUTEX,STR)

#endif /* THREADSAFE */

/*
 * Definition for NULL pointer.
 */
#ifndef	NULL
#define	NULL	((void *) 0)
#endif

/************************************************************************/
/*		Definition of an object type structure			*/
/************************************************************************/
typedef struct
{
	short	package;			/* package number	*/
	short	class;				/* class number		*/
}OMP_object_type;

/************************************************************************/
/*		Definition of  private object link structure		*/
/************************************************************************/
typedef struct
{
	struct private_object	*private_forw;	/* private objects link forward	*/
	struct private_object	*private_back;	/* private objects link backward	*/
} OMP_object_link;

/************************************************************************/
/*		Definition of a private object structure 		*/
/************************************************************************/
typedef struct private_object			/* private object description*/
{
	OM_descriptor		OMP_object_header[2];
	OMP_object_type		object_type;
	OMP_object_link		subobjects_list;
	void			*priv_object;
}OMP_private_object;



typedef void OMX_package ;		/* for future use */

typedef	struct {
	OM_object_identifier	feature;
	OM_boolean		activated;
	
}OMP_feature;

typedef	struct {
	OM_object_identifier	feature;
	OM_sint16		activated;
}OMP_package;

/* package status in the packages_table */
#define OMP_BASIC	( (OM_sint16) 0 )
#define OMP_DEFAULT	( (OM_sint16) 1 )
#define OMP_ADDED	( (OM_sint16) 2 )

/*
* The new structure is used by the omP_negotiate function
*/
#define OMP_ACTIVATED			( (OM_sint) 0 )
#define OMP_DEACTIVATED			( (OM_sint) 1 )
#define OMP_QUERY_STATE			( (OM_sint) 2 )
#define OMP_QUERY_SUPPORTED		( (OM_sint) 3 )
#define OMP_INVALID			( (OM_sint) 4 )

typedef struct {
	OM_object_identifier	feature;
	OM_sint			request ;
	OM_boolean		response ;
} OMP_negotiate_feature ;

/*---------------------------------------------------------------------------*/
/* AA | 15/03/93 | Add the OMX_STATIC macro that performs a static OM_EXPORT */
/*---------------------------------------------------------------------------*/
#define OMX_STATIC(class_name)  \
	static char OMP_D_##class_name[] = OMP_O_##class_name ;               \
	static OM_string class_name =                                         \
		{ OMP_LENGTH(class_name), OMP_D_##class_name } ;


/***************************************************************/
/* Define macros for xxx_switch function calls                 */
/***************************************************************/

typedef OM_return_code
(*OMP_create_switch) (
	OMP_object_type		class_sub_id,
	OM_private_object	destination,
	OM_boolean		initialise
);

typedef OM_return_code
(*OMP_cr_handle_switch) (
	OMP_object_type	class_sub_id
);

typedef OM_return_code
(*OMP_decode_switch) (
	OM_private_object 	encoding ,
	OM_private_object 	*original
);

typedef OM_return_code
(*OMP_delete_switch) (
	OM_object 		subject
);

typedef OM_return_code
(*OMP_encode_switch) (
	OM_private_object 	subject,
	OM_object_identifier	rules ,
	OM_private_object	*encoding
	
);

typedef	OM_return_code
(*OMP_get_switch) (
	OM_private_object	original,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object      	*copy,
	OM_value_position     	*total_number
);

typedef	OM_return_code
(*OMP_instance_switch) (
    OM_workspace		workspace,
    OM_object			subject,
    OM_object_identifier	class,
    OMP_object_type		class_type,
    OMP_object_type		subject_type,
    OM_boolean			*flag
);

typedef	OM_return_code
(*OMP_put_switch) (
	OMP_object_type		object_type,
	OM_private_object	destination,
	OM_modification		modification,
	OM_object		source,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value
);

typedef	OM_return_code
(*OMP_init_switch) (
    OM_workspace	workspace,
    void		*default_session
);

typedef	OM_return_code
(*OMP_delete_specific_value) (
    OM_workspace	workspace,
    OM_descriptor	*desc_ptr
);

typedef	OM_return_code
(*OMP_check_specific_syntax) (
    OM_syntax		syntax
);

typedef struct	OMP_switch_functions_body {
	OMP_create_switch	create;
	OMP_cr_handle_switch	cr_handle;
	OMP_decode_switch	decode;
	OMP_delete_switch	delete;
	OMP_encode_switch	encode;
	OMP_get_switch		get;
	OMP_instance_switch	instance;
	OMP_put_switch		put;
	OMP_init_switch		init;
	OMP_delete_specific_value	del_spec_value;
	OMP_check_specific_syntax	check_spec_syntax;
} OMP_switch_functions;


/************************************************************************/
/*		Definition of a workspace structure			*/
/************************************************************************/
#define NBMAXPACKAGES	20	/* maximum number of packages 		*/
#define NBMAX_CLOSURE_PACKAGES	2	/* maximum number of packages 	*/
                              	 	/* closures                     */

typedef struct OMX_workspace_body	/* workspace body structure	*/
{
					/* pointer to Jump table	*/
	struct OMP_functions_body *functions;	
					/* workspace status (init,shut)	*/
	OM_boolean status;
					/* mutex pointer		*/
	OMP_mutex	*mutex_wk;
					/* private objects list		*/
	OMP_private_object *om_private_object_list;
					/* pointer to switch table	*/
	struct OMP_switch_functions_body *sw_functions;	
					/* pointer to packages table	*/
	OMP_package *packages_table;	
					/* pointer to closure packages table*/
	OMP_package *closure_packages_table;	
					/* ptr to workspace generic structure*/
	void        *generic_workspace ;
					/* pointer to alloc_info table	*/
	struct alloc_table *alloc_table;	
					/* packages activated		*/
	OM_boolean feature[NBMAXPACKAGES];
					/* spub objects list		*/
	OM_descriptor *om_spub_object_list;
} *OMX_workspace;

extern OMX_workspace	OMX_invalid_workspace;

#undef OMP_WORKSPACE
#define OMP_WORKSPACE(external) \
        ((OMX_workspace)(OMP_INTERNAL(external)->value.string.elements))


#define	WORKSPACE_INIT	OM_TRUE		/* workspace initialized	*/
#define	WORKSPACE_SHUT	OM_FALSE	/* workspace shutdowned		*/

typedef struct {
	int	magic;
} OMP_subobject;

#define OMP_S_SUBOBJECT			0xdeafbeef

/*----------------------------------------------------------------------*/
/* AA : 24/02/93 | Add the OMP_S_INVALIDATED_SUBOBJECT constant to be   */
/*		   able to make the difference between a SPUB subobject */
/*		   and a SPUB object in the om_delete			*/
/*----------------------------------------------------------------------*/
#define OMP_S_INVALIDATED_SUBOBJECT	0xffffffff

/************************************************************************/
/*		Definition of macros					*/
/************************************************************************/

/*	set SUBOBJECT flag in the handle	*/
#define	OMP_SET_SUBOBJECT(external)	                                \
	((OMP_subobject *)OMP_INTERNAL(external))->magic = OMP_S_SUBOBJECT

/*	reset SUBOBJECT flag in the handle	*/
#define	OMP_RESET_SUBOBJECT(external)	                                \
	((OMP_subobject *)OMP_INTERNAL(external))->magic = OMP_S_INVALIDATED_SUBOBJECT


/*	to access bits that determine kind of object	*/
#define	OMP_KIND(external)						\
		(((OM_descriptor *)(external))->syntax & 		\
		(OM_S_SERVICE_GENERATED | OM_S_PRIVATE))

/*	to access syntax		*/
#define	OMP_SYNTAX(external)						\
	(((OM_descriptor *)(external))->syntax & OM_S_SYNTAX)

/*	to access package number in the object type	*/
#define OMP_PACKAGE(object_type) object_type.package

/*	to access class number in the object type	*/
#define OMP_CLASS(object_type)   object_type.class

/* 	to access object pointer in the object handle	*/
#define OMP_DATA(external)	\
		((OMP_private_object *)OMP_INTERNAL(external))->priv_object

/* 	to access addr object in the object handle	*/
#define OMP_DATA_ADDR(external)	\
		(&(OMP_DATA(external)))

/*	to access class identifier in the object handle	*/
#define	OMP_CLASS_ID_STR(external)	\
	(((OM_descriptor *)(external))->value.string)

/*	to access object type in the object handle	*/
#define OMP_OBJ_TYPE(external)	\
		((OMP_private_object *)OMP_INTERNAL(external))->object_type

/*	to check if a package is a closure package (pack_no < 0)	*/
#define OMP_CHECK_CLOSURE(pack_no)	(pack_no < 0)

/*	to check if an object handle is valid	*/
#define OMP_VALID_OBJECT(external)					\
	(OMP_INTERNAL(external)->value.string.elements != (void *)OMX_invalid_workspace	\
	 && OMP_INTERNAL(external)->value.string.elements != NULL)

/***************************************************************/
/* Define macros to test the different types of objects        */
/***************************************************************/

/* Client generated Public object */
#define	OMP_CPUB(external)	((OMP_KIND (external) &		\
				 (OM_S_PRIVATE | OM_S_SERVICE_GENERATED)) == 0)

/* Service generated Public object */
#define	OMP_SPUB(external) ((OMP_TYPE(external) != OM_PRIVATE_OBJECT)&&       \
			   ((OMP_KIND (external) & OM_S_SERVICE_GENERATED) == \
				 (OM_S_SERVICE_GENERATED)))

/* Private Object */
#define	OMP_PRIVATE(external)   ((OMP_TYPE(external) == OM_PRIVATE_OBJECT)&& \
				 ((OMP_KIND (external) &		     \
				 (OM_S_PRIVATE | OM_S_SERVICE_GENERATED)) == \
				 (OM_S_PRIVATE | OM_S_SERVICE_GENERATED)))

/* Public Object */
#define	OMP_PUBLIC(external)	(OMP_CPUB (external) || OMP_SPUB (external))

/* Private or Public subobject	*/
#define OMP_SUBOBJECT(external)						\
	(((OMP_subobject *)OMP_INTERNAL(external))->magic == OMP_S_SUBOBJECT)

/*  Reference a private subobject	*/
#define OMP_REFPRIVATE(descriptor) 					      \
			   ((OMP_KIND (descriptor) & OM_S_PRIVATE) == 	      \
				 (OM_S_PRIVATE))

/****************************************************************/
/* Modification remove values of an attribute			*/
/****************************************************************/
#define	OMP_REMOVE_VALUES	((OM_modification)0xFFFFFFFF)

/****************************************************************/
/* Dispatcher macros for switch functions calls 		*/
/****************************************************************/

#define OMP_SWITCH(workspace) (workspace->sw_functions)

#define OMP_ALLOC_TABLE(WORKSPACE) (((OMX_workspace)WORKSPACE)->alloc_table)

#define OMP_PACKAGES(workspace) (workspace->packages_table)

#define OMP_CLOSURE_PACKAGE(workspace) (workspace->closure_packages_table)

#define OMP_DEFAULT_PACKAGE(workspace) (workspace->default_package)

#define	omP_create_switch(WORKSPACE,CLASS_SUB_ID,DESTINATION,INITIALISE) \
	OMP_SWITCH(WORKSPACE)->create(CLASS_SUB_ID,DESTINATION,INITIALISE)

#define	omP_cr_handle_switch(WORKSPACE,CLASS_SUB_ID)			\
	OMP_SWITCH(WORKSPACE)->cr_handle(CLASS_SUB_ID)

#define	omP_decode_switch(WORKSPACE,ENCODING,ORIGINAL)			\
	OMP_SWITCH(WORKSPACE)->decode(ENCODING,ORIGINAL)

#define	omP_delete_switch(WORKSPACE,SUBJECT)				\
	OMP_SWITCH(WORKSPACE)->delete(SUBJECT)

#define	omP_encode_switch(WORKSPACE,ORIGINAL,RULES,ENCODING)		\
	OMP_SWITCH(WORKSPACE)->encode(ORIGINAL,RULES,ENCODING)

#define	omP_get_switch(WORKSPACE,ORIGINAL,EXCLUSIONS,			\
		INCLUDED_TYPES,LOCAL_STRINGS,				\
		INITIAL_VALUE,LIMITING_VALUE,				\
		COPY,TOTAL_NUMBER)					\
	OMP_SWITCH(WORKSPACE)->get(ORIGINAL,EXCLUSIONS,			\
		INCLUDED_TYPES,LOCAL_STRINGS,				\
		INITIAL_VALUE,LIMITING_VALUE,				\
		COPY,TOTAL_NUMBER)

#define	omP_instance_switch(WORKSPACE,SUBJECT,CLASS,CLASS_TYPE,SUBJECT_TYPE,FLAG)	\
	OMP_SWITCH(WORKSPACE)->instance(WORKSPACE,SUBJECT,CLASS,CLASS_TYPE,SUBJECT_TYPE,FLAG)

#define	omP_put_switch(WORKSPACE,OBJECT_TYPE,DESTINATION,		\
		MODIFICATION,SOURCE,INCLUDED_TYPES,			\
		INITIAL_VALUE,LIMITING_VALUE)				\
	OMP_SWITCH(WORKSPACE)->put(OBJECT_TYPE,DESTINATION,		\
		MODIFICATION,SOURCE,INCLUDED_TYPES,			\
		INITIAL_VALUE,LIMITING_VALUE)

#define	omP_init_switch(WORKSPACE,DEFAULT)				\
	OMP_SWITCH(WORKSPACE)->init(WORKSPACE,DEFAULT)

#define	omP_delete_specific_value(WORKSPACE,DESCRIPTOR)			\
	((OMP_SWITCH(WORKSPACE)->del_spec_value) ?			\
	OMP_SWITCH(WORKSPACE)->del_spec_value(WORKSPACE,DESCRIPTOR) :	\
	OM_SUCCESS)

#define	omP_check_specific_syntax(WORKSPACE,SYNTAX)			\
	((OMP_SWITCH(WORKSPACE)->check_spec_syntax) ?			\
	OMP_SWITCH(WORKSPACE)->check_spec_syntax(SYNTAX) :		\
	OM_SUCCESS)

/***************************************************************/
/* Define macros for allocation memory function calls          */
/***************************************************************/

#define ROUNDUP(WORKSPACE,LENGTH)					\
	(LENGTH)

#define OMP_XALLOC(WORKSPACE,TYPE,COUNT)				\
	OM_MALLOC(OMP_ALLOC_TABLE(WORKSPACE)[(int)TYPE].object_size * (COUNT))

#define OMP_XFREE(WORKSPACE,DATA,TYPE,COUNT)				\
	free(DATA)

#define OMP_XALLOC_STR(WORKSPACE,STRING,LENGTH)				\
	omP_xalloc_str(STRING,LENGTH)

#define OMP_XFREE_STR(WORKSPACE,STRING,LENGTH)				\
	OM_FREE(STRING)



/****************************************************************/
/* Prototypes for internal functions				*/
/****************************************************************/

OM_return_code
omP_copy(
    OM_private_object  original,
    OM_workspace       workspace,
    OM_private_object *copy
);

OM_return_code
omP_copy_value(
    OM_private_object  source,
    OM_type            source_type,
    OM_value_position  source_value_position,
    OM_private_object  destination,
    OM_type            destination_type,
    OM_value_position  destination_value_position
);

OM_return_code
omP_create(
    OM_object_identifier  class,
    OM_boolean            initialise,
    OM_workspace          workspace,
    OM_private_object    *object
);

OM_return_code
omP_decode(
    OM_private_object  encoding,
    OM_private_object *original
);

OM_return_code
omP_delete(
    OM_object  subject 
);

OM_return_code
omP_encode(
    OM_private_object     original,
    OM_object_identifier  rules,
    OM_private_object    *encoding
);

OM_return_code
omP_get(
    OM_private_object  original,
    OM_exclusions      exclusions,
    OM_type_list       included_types,
    OM_boolean         local_strings,
    OM_value_position  initial_value,
    OM_value_position  limiting_value,
    OM_public_object  *copy,
    OM_value_position *total_number
);

OM_return_code
omP_instance(
    OM_object             subject,
    OM_object_identifier  class,
    OM_boolean           *instance
);

OM_return_code
omP_put(
    OM_private_object  destination,
    OM_modification    modification,
    OM_object          source,
    OM_type_list       included_types,
    OM_value_position  initial_value,
    OM_value_position  limiting_value
);

OM_return_code
omP_read(
    OM_private_object  subject,
    OM_type            type,
    OM_value_position  value_position,
    OM_boolean         local_string,
    OM_string_length  *string_offset,
    OM_string         *elements
);

OM_return_code
omP_remove(
    OM_private_object  subject,
    OM_type            type,
    OM_value_position  initial_value,
    OM_value_position  limiting_value
);

OM_return_code
omP_write(
    OM_private_object  subject,
    OM_type            type,
    OM_value_position  value_position,
    OM_syntax          syntax,
    OM_string_length  *string_offset,
    OM_string          elements
);


OM_return_code
omP_create_handle(
	OM_object_identifier	class,
	OM_workspace		workspace,
	void			*priv_object,
	OM_private_object	*object
);

OM_return_code
omP_initialize(
	struct OMP_switch_functions_body *sw_functions,	/* switch functions */
	OMP_package	*packages_table,   /* packages table */
	OMP_package	*closure_packages_table,   /* Closure packages table */
	void		*generic_workspace,
	struct alloc_table *alloc_table,   /* memory managmt structures */
	void	 	*default_session,  /* initial values of the Session */
	OM_workspace	*workspace_return  /* workspace handle */
);

void
omP_shutdown(
	OM_workspace	workspace
);

void
omP_complete_shutdown(
	OM_workspace	workspace
);

OM_return_code
omP_version(
	OM_workspace	workspace,
	OMP_feature	feature_list[]
);

OM_return_code
omP_negotiate(
	OM_workspace		workspace,
	OMP_negotiate_feature	feature_list[]
);


void
omP_error(
    const char *fmt,
    ...
);



/* internal workspace functions */

OM_return_code
omP_get_generic_workspace(
    OM_workspace    workspace,
    void **generic_workspace
);

OM_return_code
omP_set_generic_workspace(
    OM_workspace    workspace,
    void *generic_workspace
);

OM_return_code
omP_package_present(
    OM_workspace          workspace,
    OM_object_identifier  package_oid,
    OM_boolean           *package_present
);

OM_return_code
omP_add_package(
    OM_workspace  workspace,
    OMP_package   package_feature
);

OM_return_code
omP_delete_package(
    OM_workspace  workspace,
    OM_object_identifier  package_oid
);

void
omP_free_workspace(
    OM_workspace    workspace
);




/* intermediate OMP_x functions */

OM_return_code
omP_xcopy(
    OM_private_object  original,
    OM_workspace       workspace,
    OM_private_object *copy
);

OM_return_code
omP_xcopy_value(
    OM_private_object  source,
    OM_type            source_type,
    OM_value_position  source_value_position,
    OM_private_object  destination,
    OM_type            destination_type,
    OM_value_position  destination_value_position
);

OM_return_code
omP_xcreate(
    OM_object_identifier  class,
    OM_boolean            initialise,
    OM_workspace          workspace,
    OM_private_object    *object
);

OM_return_code
omP_xdecode(
    OM_private_object  encoding,
    OM_private_object *original
);

OM_return_code
omP_xdelete(
    OM_object  subject 
);

OM_return_code
omP_xencode(
    OM_private_object     original,
    OM_object_identifier  rules,
    OM_private_object    *encoding
);

OM_return_code
omP_xget(
    OM_private_object  original,
    OM_exclusions      exclusions,
    OM_type_list       included_types,
    OM_boolean         local_strings,
    OM_value_position  initial_value,
    OM_value_position  limiting_value,
    OM_public_object  *copy,
    OM_value_position *total_number
);

OM_return_code
omP_xinstance(
    OM_object             subject,
    OM_object_identifier  class,
    OM_boolean           *instance
);

OM_return_code
omP_xput(
    OM_private_object  destination,
    OM_modification    modification,
    OM_object          source,
    OM_type_list       included_types,
    OM_value_position  initial_value,
    OM_value_position  limiting_value
);

OM_return_code
omP_xread(
    OM_private_object  subject,
    OM_type            type,
    OM_value_position  value_position,
    OM_boolean         local_string,
    OM_string_length  *string_offset,
    OM_string         *elements
);

OM_return_code
omP_xremove(
    OM_private_object  subject,
    OM_type            type,
    OM_value_position  initial_value,
    OM_value_position  limiting_value
);

OM_return_code
omP_xwrite(
    OM_private_object  subject,
    OM_type            type,
    OM_value_position  value_position,
    OM_syntax          syntax,
    OM_string_length  *string_offset,
    OM_string          elements
);


OM_return_code
omP_xcreate_handle(
	OM_object_identifier	class,
	OM_workspace		workspace,
	void			*priv_object,
	OM_private_object	*object
);

OM_return_code
omP_xinitialize(
	struct OMP_switch_functions_body *sw_functions,	/* switch functions */
	OMP_package	*packages_table,   /* packages table */
	OMP_package	*closure_packages_table,   /* Closure packages table */
	void		*generic_workspace,
	struct alloc_table *alloc_table,   /* memory managmt structures */
	void	 	*default_session,  /* initial values of the Session */
	OM_workspace	*workspace_return  /* workspace handle */
);

void
omP_xshutdown(
	OM_workspace	workspace
);

void
omP_xcomplete_shutdown(
	OM_workspace	workspace
);

OM_return_code
omP_xversion(
	OM_workspace	workspace,
	OMP_feature	feature_list[]
);

OM_return_code
omP_xnegotiate(
	OM_workspace		workspace,
	OMP_negotiate_feature	feature_list[]
);

OM_return_code
omP_xget_generic_workspace(
    OM_workspace    workspace,
    void **generic_workspace
);

OM_return_code
omP_xset_generic_workspace(
    OM_workspace    workspace,
    void *generic_workspace
);

OM_return_code
omP_xpackage_present(
    OM_workspace          workspace,
    OM_object_identifier  package_oid,
    OM_boolean           *package_present
);

OM_return_code
omP_xadd_package(
    OM_workspace  workspace,
    OMP_package   package_feature
);

OM_return_code
omP_xdelete_package(
    OM_workspace  workspace,
    OM_object_identifier  package_oid
);




void
omP_update_descriptor(
	OM_private_object original,
	OM_descriptor *desc0
);

OM_return_code
omP_get_object_type (
        OMX_workspace           workspace,
        OM_object_identifier    class,
        OMP_object_type *class_id
);

OM_boolean
omP_check_negotiated_package(
	OM_workspace workspace,
	int package_no
);

OM_return_code
omP_get_class_sub_id (
	OMX_workspace	workspace,
	OM_object_identifier	class,
	OMP_object_type	*class_sub_id
);

OMP_private_object *
omP_get_object_handle (
	OMX_workspace	workspace,
	OM_object_identifier	class,
	OMP_object_type		class_sub_id,
	void			*priv_object
);

OM_return_code
omP_check_value_syntax(
    OMX_workspace   workspace,
    OM_syntax syntax
);




/****************************************************************/
/* Prototypes for invalid workspace functions			*/
/****************************************************************/

OM_return_code
omP_iw_copy(
    OM_private_object  original,
    OM_workspace       workspace,
    OM_private_object *copy
);

OM_return_code
omP_iw_copy_value(
    OM_private_object  source,
    OM_type            source_type,
    OM_value_position  source_value_position,
    OM_private_object  destination,
    OM_type            destination_type,
    OM_value_position  destination_value_position
);

OM_return_code
omP_iw_create(
    OM_object_identifier  class,
    OM_boolean            initialise,
    OM_workspace          workspace,
    OM_private_object    *object
);

OM_return_code
omP_iw_decode(
    OM_private_object  encoding,
    OM_private_object *original
);

OM_return_code
omP_iw_delete(
    OM_object  subject 
);

OM_return_code
omP_iw_encode(
    OM_private_object     original,
    OM_object_identifier  rules,
    OM_private_object    *encoding
);

OM_return_code
omP_iw_get(
    OM_private_object  original,
    OM_exclusions      exclusions,
    OM_type_list       included_types,
    OM_boolean         local_strings,
    OM_value_position  initial_value,
    OM_value_position  limiting_value,
    OM_public_object  *copy,
    OM_value_position *total_number
);

OM_return_code
omP_iw_instance(
    OM_object             subject,
    OM_object_identifier  class,
    OM_boolean           *instance
);

OM_return_code
omP_iw_put(
    OM_private_object  destination,
    OM_modification    modification,
    OM_object          source,
    OM_type_list       included_types,
    OM_value_position  initial_value,
    OM_value_position  limiting_value
);

OM_return_code
omP_iw_read(
    OM_private_object  subject,
    OM_type            type,
    OM_value_position  value_position,
    OM_boolean         local_string,
    OM_string_length  *string_offset,
    OM_string         *elements
);

OM_return_code
omP_iw_remove(
    OM_private_object  subject,
    OM_type            type,
    OM_value_position  initial_value,
    OM_value_position  limiting_value
);

OM_return_code
omP_iw_write(
    OM_private_object  subject,
    OM_type            type,
    OM_value_position  value_position,
    OM_syntax          syntax,
    OM_string_length  *string_offset,
    OM_string          elements
);


/******************************************************************/
/*      XOM TRACE LEVELS                                          */
/******************************************************************/
#define OMP_TRACE_OFF              0x00000000
#define OMP_TRACE_ASSERT_ON        0x00000001
#define OMP_TRACE_CHECK_SYNTAX     0x00000002
#define OMP_TRACE_BED              0x00000004
#define OMP_TRACE_FATAL_ERROR      0x00000008
#define OMP_TRACE_OBJECT           0x00000010
#define OMP_DO_NOT_CHECK_SYNTAX    0x00000020
#define OMP_TRACE_ALL              ((0xffffffff)&(~OMP_TRACE_ASSERT_ON))

extern int     omP_trace_opt;
extern FILE    *omP_trace_file;

/******************************************************************/
/*      ASSERT DEFINITION                                         */
/******************************************************************/
#define	OMP_ASSERT(assertion) \
	if(omP_trace_opt & OMP_TRACE_ASSERT_ON) assert((assertion))


/******************************************************************/
/*      OMP_FATAL_FAILURE                                         */
/******************************************************************/
/*
* This MACRO is implementation defined
* It's used to generate a signal to the process or to the current
* thread.
*/
#define OMP_FATAL_FAILURE	kill(getpid() , SIGSEGV) 

/*defines for different types of modification */
#define	OMP_INSERT_AT_BEGINNING	((initial_value == 0) && (limiting_value == 0)) 
#define	OMP_INSERT_AT_END	((initial_value == OM_ALL_VALUES) &&\
				 (limiting_value == OM_ALL_VALUES)) 
#define	OMP_REPLACE_ALL	((initial_value == 0) && \
			 (limiting_value == OM_ALL_VALUES))
#define	OMP_REPLACE	(initial_value != limiting_value)
#define	OMP_INSERT	(initial_value == limiting_value)

#define	OMP_REPLACE_SINGLE_VALUE	((initial_value == 0) &&	\
					 ((limiting_value == 1) ||	\
					  (limiting_value == OM_ALL_VALUES)))	

#define	MAX_VALUES	((OM_sint16)0x7fff)
#define	MAX_LENGTH	((OM_value_length)0x7fffffff)

/* define min and max modification */
#define OMP_MODIF_MIN	OM_INSERT_AT_BEGINNING
#define OMP_MODIF_MAX	OM_REPLACE_CERTAIN_VALUES

#define	OMP_VALID_MOD(modif)	((modif >= OMP_MODIF_MIN) &&	\
				 (modif <= OMP_MODIF_MAX))

/***************************************************************/

#endif  /* _XOMIP_H     */
