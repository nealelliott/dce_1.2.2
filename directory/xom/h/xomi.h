/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xomi.h,v $
 * Revision 1.1.12.1  1996/10/03  14:36:20  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:37 UTC  arvind  /main/arvind_pkss/1]
 *
 * Revision 1.1.10.2  1996/02/18  23:38:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:11  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  15:39:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:56  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:49:22  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:25  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:00:35  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:10:12  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:20:50  marrek
 * 	Remove duplicated history lines.
 * 	[1993/08/03  09:59:37  marrek]
 * 
 * 	July 1993 code drop.
 * 	[1993/07/27  14:24:51  marrek]
 * 
 * Revision 1.1.4.2  1992/12/30  21:17:56  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:06:23  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:20:51  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:47:37  melman]
 * 
 * $EndLog$
 */
#ifndef _XOMI_H
#define _XOMI_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xomi_rcsid[] = "@(#)$RCSfile: xomi.h,v $ $Revision: 1.1.12.1 $ $Date: 1996/10/03 14:36:20 $";
#endif
/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/
/* @(#)xomi.h	10.2.1.1 91/12/19 (SNI) */

/*	@(#)xomi.h	1.7 (BULL S.A) 1/29/92	*/

/**********************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xomi.h                                              */
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
/* DESCRIPTION  : xomi.h as defined in XOM                            */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)@(#)xomi.h	1.7 (Bull S.A) 1/29/92 */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       |    |         */
/*   1.0    | 90-10-01| Original                       |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/**********************************************************************/

/* BEGIN WORKSPACE INTERFACE */

#include <stddef.h>

#define XOM_VERSION2

/* TYPES AND MACROS */

/* Standard Internal Representation of an object */

typedef OM_descriptor OMP_object_header[2];
typedef OMP_object_header *OMP_object;

/* Standard Internal Representation of a workspace */

typedef OM_return_code
(*OMP_copy) (
	OM_private_object	original,
	OM_workspace		workspace,
	OM_private_object     	*copy
);

typedef OM_return_code
(*OMP_copy_value) (
	OM_private_object	source,
	OM_type			source_type,
	OM_value_position	source_value_position,
	OM_private_object	destination,
	OM_type			destination_type,
	OM_value_position	destination_value_position
);

typedef OM_return_code
(*OMP_create) (
	OM_object_identifier	theClass,
	OM_boolean		initialise,
	OM_workspace		workspace,
	OM_private_object     	*object
);

typedef OM_return_code
(*OMP_decode) (
	OM_private_object	encoding,
	OM_private_object     	*original
);

typedef OM_return_code
(*OMP_delete) (
	OM_object		subject 
);

typedef OM_return_code
(*OMP_encode) (
	OM_private_object	original,
	OM_object_identifier	rules,
	OM_private_object     	*encoding
);

typedef OM_return_code
(*OMP_get) (
	OM_private_object	original,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object      	*copy,
	OM_value_position     	*total_number
);

typedef OM_return_code
(*OMP_instance) (
	OM_object		subject,
	OM_object_identifier	theClass,
	OM_boolean	      	*instance
);

typedef OM_return_code
(*OMP_put) (
	OM_private_object	destination,
	OM_modification		modification,
	OM_object		source,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value
);

typedef OM_return_code
(*OMP_read) (
	OM_private_object	subject,
	OM_type			type,
	OM_value_position	value_position,
	OM_boolean		local_string,
	OM_string_length   	*string_offset,
	OM_string	      	*elements
);

typedef OM_return_code
(*OMP_remove) (
	OM_private_object	subject,
	OM_type			type,
	OM_value_position	initial_value,
	OM_value_position	limiting_value
);

typedef OM_return_code
(*OMP_write) (
	OM_private_object	subject,
	OM_type			type,
	OM_value_position	value_position,
	OM_syntax		syntax,
	OM_string_length   	*string_offset,
	OM_string		elements
);

typedef struct OMP_functions_body {
	OM_uint32	function_number;
	OMP_copy 	omp_copy;
	OMP_copy_value	omp_copy_value;
	OMP_create	omp_create;
	OMP_decode	omp_decode;
	OMP_delete	omp_delete;
	OMP_encode	omp_encode;
	OMP_get		omp_get;
	OMP_instance	omp_instance;
	OMP_put		omp_put;
	OMP_read 	omp_read;
	OMP_remove	omp_remove;
	OMP_write	omp_write;
} OMP_functions;

typedef struct OMP_workspace_body {
	struct OMP_functions_body     	*functions;
} *OMP_workspace;


/* Useful Macros */

#define OMP_EXTERNAL(internal)	\
        ((OM_object)((OM_descriptor *)(internal) + 1))


#define OMP_INTERNAL(external)	((OM_descriptor *)(external) - 1)


#define OMP_TYPE(desc)  (((OM_descriptor *)(desc))->type)


#define OMP_WORKSPACE(external) \
        ((OMP_workspace)(OMP_INTERNAL(external)->value.string.elements))


#define OMP_FUNCTIONS(external) (OMP_WORKSPACE(external)->functions)


/* DISPATCHER MACROS */

#ifdef XOM_VERSION2

#define om_copy(ORIGINAL,WORKSPACE,COPY)				\
	( (WORKSPACE) == NULL	?					\
	  OM_NO_SUCH_WORKSPACE	:					\
          (((OMP_workspace)(WORKSPACE))->functions->omp_copy(		\
                (ORIGINAL),(WORKSPACE),(COPY)))				\
	)
 
 
#define om_copy_value(SOURCE, SOURCE_TYPE, SOURCE_POSITION,		\
                      DEST,   DEST_TYPE,   DEST_POSITION)		\
	( (DEST) == NULL	?					\
	  OM_NO_SUCH_OBJECT	:					\
	  (((DEST)->syntax & OM_S_PRIVATE)   ?				\
	    (OMP_FUNCTIONS(DEST)->omp_copy_value(                      	\
                      (SOURCE), (SOURCE_TYPE), (SOURCE_POSITION),   	\
                      (DEST),   (DEST_TYPE),   (DEST_POSITION))) :	\
	    OM_NOT_PRIVATE						\
	  )								\
	)
 
 
#define om_create(CLASS,INITIALISE,WORKSPACE,OBJECT)                	\
	( (WORKSPACE) == NULL	?					\
	  OM_NO_SUCH_WORKSPACE	:					\
          (((OMP_workspace)(WORKSPACE))->functions->omp_create(		\
                  (CLASS),(INITIALISE),(WORKSPACE),(OBJECT)))		\
	)
 
 
#define om_decode(ENCODING,ORIGINAL)                                	\
	( (ENCODING) == NULL	?					\
	  OM_NO_SUCH_OBJECT	:					\
	  (((ENCODING)->syntax & OM_S_PRIVATE)   ?			\
            (OMP_FUNCTIONS(ENCODING)->omp_decode((ENCODING),(ORIGINAL))):  \
	    OM_NOT_PRIVATE						\
	  )								\
	)
 
 
#define om_delete(SUBJECT)			                    	\
	( (SUBJECT) == NULL	?					\
	  OM_NO_SUCH_OBJECT	:					\
	  (								\
	    ((SUBJECT)->syntax & OM_S_SERVICE_GENERATED)   ?		\
	    OMP_FUNCTIONS(SUBJECT)->omp_delete((SUBJECT)) :		\
	    OM_NOT_THE_SERVICES						\
	  )								\
	)
 
 
#define om_encode(ORIGINAL,RULES,ENCODING)                          	\
	( (ORIGINAL) == NULL	?					\
	  OM_NO_SUCH_OBJECT	:					\
	  (((ORIGINAL)->syntax & OM_S_PRIVATE)   ?			\
            (OMP_FUNCTIONS(ORIGINAL)->omp_encode(			\
				(ORIGINAL),(RULES),(ENCODING)))  :	\
	    OM_NOT_PRIVATE						\
	  )								\
	)
 
 
#define om_get(ORIGINAL,EXCLUSIONS,TYPES,LOCAL_STRINGS,			\
			INITIAL,LIMIT,COPY,TOTAL_NUMBER)		\
	( (ORIGINAL) == NULL	?					\
	  OM_NO_SUCH_OBJECT	:					\
	  (((ORIGINAL)->syntax & OM_S_PRIVATE)   ?			\
            (OMP_FUNCTIONS(ORIGINAL)->omp_get(				\
         	(ORIGINAL),(EXCLUSIONS),(TYPES),(LOCAL_STRINGS),	\
			(INITIAL),(LIMIT),(COPY),(TOTAL_NUMBER))):	\
	    OM_NOT_PRIVATE						\
	  )								\
	)
 
 
#define om_instance(SUBJECT,CLASS,INSTANCE)				\
	( (SUBJECT) == NULL	?					\
	  OM_NO_SUCH_OBJECT	:					\
    	  (((SUBJECT)->syntax & OM_S_SERVICE_GENERATED) 	?	\
    	   OMP_FUNCTIONS(SUBJECT)->omp_instance(			\
				(SUBJECT),(CLASS),(INSTANCE))   :	\
    	   OM_NOT_THE_SERVICES						\
	  )								\
	)
 
 
#define om_put(DESTINATION,MODIFICATION,SOURCE,TYPES,INITIAL,LIMIT)    	\
	( (DESTINATION) == NULL	?					\
	  OM_NO_SUCH_OBJECT	:					\
	  (((DESTINATION)->syntax & OM_S_PRIVATE)   ?			\
           (OMP_FUNCTIONS(DESTINATION)->omp_put(			\
               (DESTINATION),(MODIFICATION),(SOURCE),(TYPES),		\
		   (INITIAL),(LIMIT)))				 :	\
	    OM_NOT_PRIVATE						\
	  )								\
	)
 
 
#define om_read(SUBJECT,TYPE,VALUE_POS,LOCAL_STRING,			\
			STRING_OFFSET,ELEMENTS)				\
	( (SUBJECT) == NULL	?					\
	  OM_NO_SUCH_OBJECT	:					\
	  (((SUBJECT)->syntax & OM_S_PRIVATE)   ?			\
            (OMP_FUNCTIONS(SUBJECT)->omp_read(                          \
             (SUBJECT),(TYPE),(VALUE_POS),(LOCAL_STRING),           	\
		(STRING_OFFSET),(ELEMENTS)))			 :	\
	    OM_NOT_PRIVATE						\
	  )								\
	)
 
 
#define om_remove(SUBJECT,TYPE,INITIAL,LIMIT)                       	\
	( (SUBJECT) == NULL	?					\
	  OM_NO_SUCH_OBJECT	:					\
	  (((SUBJECT)->syntax & OM_S_PRIVATE)   ?			\
            (OMP_FUNCTIONS(SUBJECT)->omp_remove(                        \
                 (SUBJECT),(TYPE),(INITIAL),(LIMIT)))		 :	\
	    OM_NOT_PRIVATE						\
	  )								\
	)
 
 
#define om_write(SUBJECT,TYPE,VALUE_POS,SYNTAX,STRING_OFFSET,ELEMENTS) 	\
	( (SUBJECT) == NULL	?					\
	  OM_NO_SUCH_OBJECT	:					\
	  (((SUBJECT)->syntax & OM_S_PRIVATE)   ?			\
            (OMP_FUNCTIONS(SUBJECT)->omp_write(                         \
             	(SUBJECT),(TYPE),(VALUE_POS),                          	\
             	(SYNTAX),(STRING_OFFSET),(ELEMENTS)))		 :	\
	    OM_NOT_PRIVATE						\
	  )								\
	)
 
#else /* XOM_VERSION2 */

#define om_copy(ORIGINAL,WORKSPACE,COPY)				\
        (((OMP_workspace)(WORKSPACE))->functions->omp_copy(		\
                (ORIGINAL),(WORKSPACE),(COPY)))


#define om_copy_value(SOURCE, SOURCE_TYPE, SOURCE_POSITION,		\
                      DEST,   DEST_TYPE,   DEST_POSITION)		\
        (OMP_FUNCTIONS(DEST)->omp_copy_value(                       	\
                      (SOURCE), (SOURCE_TYPE), (SOURCE_POSITION),   	\
                      (DEST),   (DEST_TYPE),   (DEST_POSITION)))


#define om_create(CLASS,INITIALISE,WORKSPACE,OBJECT)                	\
        (((OMP_workspace)(WORKSPACE))->functions->omp_create(		\
                  (CLASS),(INITIALISE),(WORKSPACE),(OBJECT)))


#define om_decode(ENCODING,ORIGINAL)                                	\
        (OMP_FUNCTIONS(ENCODING)->omp_decode((ENCODING),(ORIGINAL)))


#define om_delete(SUBJECT)			                    	\
	(((SUBJECT)->syntax & OM_S_SERVICE_GENERATED) ?			\
	OMP_FUNCTIONS(SUBJECT)->omp_delete((SUBJECT)) :			\
	OM_NOT_THE_SERVICES)


#define om_encode(ORIGINAL,RULES,ENCODING)                          	\
        (OMP_FUNCTIONS(ORIGINAL)->omp_encode((ORIGINAL),(RULES),(ENCODING)))

 
#define om_get(ORIGINAL,EXCLUSIONS,TYPES,LOCAL_STRINGS,			\
			INITIAL,LIMIT,COPY,TOTAL_NUMBER)		\
        (OMP_FUNCTIONS(ORIGINAL)->omp_get(				\
         (ORIGINAL),(EXCLUSIONS),(TYPES),(LOCAL_STRINGS),		\
			(INITIAL),(LIMIT),(COPY),(TOTAL_NUMBER)))


#define om_instance(SUBJECT,CLASS,INSTANCE)				\
    (((SUBJECT)->syntax & OM_S_SERVICE_GENERATED) ?			\
    OMP_FUNCTIONS(SUBJECT)->omp_instance((SUBJECT),(CLASS),(INSTANCE)) :\
    OM_NOT_THE_SERVICES)


#define om_put(DESTINATION,MODIFICATION,SOURCE,TYPES,INITIAL,LIMIT)    	\
        (OMP_FUNCTIONS(DESTINATION)->omp_put(				\
               (DESTINATION),(MODIFICATION),(SOURCE),(TYPES),		\
		   (INITIAL),(LIMIT)))


#define om_read(SUBJECT,TYPE,VALUE_POS,LOCAL_STRING,			\
			STRING_OFFSET,ELEMENTS)				\
        (OMP_FUNCTIONS(SUBJECT)->omp_read(                              \
             (SUBJECT),(TYPE),(VALUE_POS),(LOCAL_STRING),           	\
		(STRING_OFFSET),(ELEMENTS)))
 

#define om_remove(SUBJECT,TYPE,INITIAL,LIMIT)                       	\
        (OMP_FUNCTIONS(SUBJECT)->omp_remove(                            \
                 (SUBJECT),(TYPE),(INITIAL),(LIMIT)))
 

#define om_write(SUBJECT,TYPE,VALUE_POS,SYNTAX,STRING_OFFSET,ELEMENTS) 	\
        (OMP_FUNCTIONS(SUBJECT)->omp_write(                             \
             (SUBJECT),(TYPE),(VALUE_POS),                          	\
             (SYNTAX),(STRING_OFFSET),(ELEMENTS)))

#endif /* XOM_VERSION2 */

/* END WORKSPACE INTERFACE */
#endif /* _XOMI_H */
