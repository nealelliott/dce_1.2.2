/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xomtrace.h,v $
 * Revision 1.1.6.2  1996/02/18  23:38:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:16  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:39:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:58  root]
 * 
 * Revision 1.1.4.1  1994/02/22  19:00:39  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:10:38  marrek]
 * 
 * Revision 1.1.2.2  1993/08/10  07:21:23  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:25:19  marrek]
 * 
 * $EndLog$
 */
#ifndef _XOMTRACE_H
#define _XOMTRACE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xomtrace_rcsid[] = "@(#)$RCSfile: xomtrace.h,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 23:38:34 $";
#endif
/**********************************************************************/
/*                  COPYRIGHT (C) BULL S.A. 1992                      */
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


/**********************************************************************/
/*  File name:  xom_trace.h                                           */
/*  Project:    CM-API                                                */
/*  Author:     Alain ALBRECHT                                        */
/**********************************************************************/
/*             M O D I F I C A T I O N   P R O T O C O L              */
/**********************************************************************/
/*  date         !          description                      ! Author */
/*_______________!___________________________________________!________*/
/* 02/03/93      ! initial version                           !        */
/*               !                                           !        */
/*               !                                           !        */
/**********************************************************************/
/*                                                                    */
/*                 f i l e   d e s c r i p t i o n                    */
/*                 ===============================                    */
/*                                                                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*	Object Manipulation control				      */
/**********************************************************************/


OM_return_code
omP_set_trace(
    void
);

void
omP_trace_create(                                                               
    FILE *trc_file,
    char *file,
    int line,
    OM_private_object   object
);

void
omP_trace_encode(
    FILE *trc_file,
    char *file,
    int line,
    OM_private_object   object
);

void
omP_trace_decode(
    FILE *trc_file,
    char *file,
    int line,
    OM_private_object   object
);

void
omP_trace_copy(
    FILE *trc_file,
    char *file,
    int line,
    OM_private_object   object
);

void 
omP_trace_get(
    FILE *trc_file,
    char *file,
    int line,
    OM_private_object   object,
    int att_nb
);

void 
omP_trace_delete(
    FILE *trc_file,
    char *file,
    int line,
    OM_object   object
);


#ifdef OM_TRACE

#define TRC_CREATE(object)\
		if (omP_trace_opt & OMP_TRACE_OBJECT)	\
		  omP_trace_create( 	\
				omP_trace_file,	\
				__FILE__,	\
				__LINE__,	\
				(object))
			
#define TRC_GET(object, desc_nb)\
		if (omP_trace_opt & OMP_TRACE_OBJECT)	\
		  omP_trace_get( 	\
				omP_trace_file,	\
				__FILE__,	\
				__LINE__,	\
				(object),	\
				(desc_nb))
			
#define TRC_DELETE(object)\
		if (omP_trace_opt & OMP_TRACE_OBJECT)	\
		  omP_trace_delete( 	\
				omP_trace_file,	\
				__FILE__,	\
				__LINE__,	\
				(object))
			
#define TRC_ENCODE(object)\
		if (omP_trace_opt & OMP_TRACE_OBJECT)	\
		  omP_trace_encode( 	\
				omP_trace_file,	\
				__FILE__,	\
				__LINE__,	\
				(object))
			
#define TRC_DECODE(object)\
		if (omP_trace_opt & OMP_TRACE_OBJECT)	\
		  omP_trace_decode( 	\
				omP_trace_file,	\
				__FILE__,	\
				__LINE__,	\
				(object))
			
#define TRC_COPY(object)\
		if (omP_trace_opt & OMP_TRACE_OBJECT)	\
		  omP_trace_copy( 	\
				omP_trace_file,	\
				__FILE__,	\
				__LINE__,	\
				(object))
			
/**********************************************************************/
/*	No Object Manipulation control				      */
/**********************************************************************/
#else

#define TRC_CREATE(object)
#define TRC_GET(object, desc_nb)
#define TRC_DELETE(object)
#define TRC_ENCODE(object)
#define TRC_DECODE(object)
#define TRC_COPY(object)

#endif		/*	OM_TRACE	*/


/**********************************************************************/
/*      OM TRACE LEVELS                                             */
/**********************************************************************/
#define OM_TRACE_NONE            0x00000000
#define OM_TRACE_ON              0x00000001
#define OM_TRACE_ALL             0xffffffff

#endif	/* _XOM_TRACE_H */

/***************/
/* END OF FILE */
/***************/
