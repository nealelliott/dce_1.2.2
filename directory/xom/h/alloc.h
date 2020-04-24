/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: alloc.h,v $
 * Revision 1.1.11.2  1996/02/18  23:38:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:25  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:38:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:40  root]
 * 
 * Revision 1.1.9.2  1994/07/06  15:09:21  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:42  marrek]
 * 
 * Revision 1.1.9.1  1994/02/22  19:00:15  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:09:53  marrek]
 * 
 * Revision 1.1.7.2  1993/08/10  07:18:56  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:23:19  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:17:15  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:05:39  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:36:08  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:44:31  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:19:42  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:46:47  melman]
 * 
 * $EndLog$
 */
/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/
/* @(#)alloc.h	10.2.1.1 91/12/19 (SNI) */

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
/*	@(#)alloc.h	1.17 (Bull S.A) 6/26/92	*/

/*****************************************************************
 ** alloc.h -- Object Manager Allocation Package definitions.   **
 **								**
 ** Author : J.Zrihen, DASD/SSD.				**
 *****************************************************************/

#ifndef _ALLOC_H
#define _ALLOC_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char alloc_rcsid[] = "@(#)$RCSfile: alloc.h,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 23:38:12 $";
#endif

#include <stdlib.h>

/**********************************************************************/
/*	Memory ManaGemenT control				      */
/**********************************************************************/

extern char *mmgt_realloc(char *file, OM_sint line, char *old, OM_sint size);
extern char *mmgt_calloc(char *file, OM_sint line, OM_sint n, OM_sint size);
extern char *mmgt_malloc(char *file, OM_sint line, OM_sint size);
extern void  mmgt_free(char *file, OM_sint line, char *buffer);

/* for internal use */
extern char *mmgt_xrealloc(char *file, OM_sint line, char *old, OM_sint size);
extern char *mmgt_xcalloc(char *file, OM_sint line, OM_sint n, OM_sint size);
extern char *mmgt_xmalloc(char *file, OM_sint line, OM_sint size);
extern void  mmgt_xfree(char *file, OM_sint line, char *buffer);

extern OM_return_code mmgt_set_trace(void);

#ifdef MMGT

#define OM_REALLOC(old, size)\
		mmgt_realloc(__FILE__, __LINE__, (old), (size))

#define OM_CALLOC(nb, size)\
		mmgt_calloc(__FILE__, __LINE__, (nb), (size))

#define OM_MALLOC(size)\
		mmgt_malloc(__FILE__, __LINE__, (size))

#define OM_FREE(buffer)\
		mmgt_free(__FILE__, __LINE__, (char *) (buffer)) \
		buffer = NULL;


/**********************************************************************/
/*	No Memory ManaGemenT control				      */
/**********************************************************************/
#else

#define OM_REALLOC(old, size)\
		(char *) realloc((old), (size))

#define OM_CALLOC(nb, size)\
		(char *) calloc((nb), (size))

#define OM_MALLOC(size)\
		(char *) malloc((size))

#define OM_FREE(buffer)\
{			\
	if (buffer != NULL ) { \
		free((char *) (buffer)); \
		buffer = NULL; \
	} \
}
#endif		/*	MMGT	*/


/**********************************************************************/
/*      MMGT TRACE LEVELS                                             */
/**********************************************************************/
#define MMGT_TRACE_NONE            0x00000000
#define MMGT_TRACE_ON              0x00000001
#define MMGT_TRACE_ALL             0xffffffff


/*
 * Allocation classes, for structures declared.
 * Warning : alloc_type and alloc_info are order dependent.
 */
enum 	alloc_type
{
	AL_GEN_BEGIN=0,
	AL_STRING=AL_GEN_BEGIN,
	AL_DESCRIPTOR,			/* OM_descriptor  	*/
	AL_HANDLE,			/* Private object 	*/
	AL_GEN_END			/* End generic structures 	*/
};

struct	alloc_table 
{
    short		object_size;
};

#ifndef	NULL
#define	NULL	((void *) 0)
#endif

#ifdef	OMINIT
/*
 * Allocation information structures.
 * Warning : alloc_type and alloc_info initialization are order dependent.
 */
struct	alloc_table
 xom_alloc_table[] =
{
    {sizeof(char)},
    {sizeof(OM_descriptor)},
    {sizeof(OMP_private_object)}
};

/***********************************************************************/

#else
extern	struct alloc_table	xom_alloc_table[];
#endif	/* OMINIT */


/***********************************************************************/
/*     Prototypes for memory allocation functions calls                */
/***********************************************************************/

char *
omP_xalloc_str(
    char		*string,
    OM_sint			length
);

#endif	/* _ALLOC_H */

