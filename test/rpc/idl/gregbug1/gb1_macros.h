/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gb1_macros.h,v $
 * Revision 1.1.6.2  1996/02/18  23:05:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:56  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:06:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:40  root]
 * 
 * Revision 1.1.4.2  1993/01/07  19:57:33  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:34:08  bbelch]
 * 
 * Revision 1.1.2.2  1992/07/09  16:29:33  rsalz
 * 	Put #ifndef wrapper around TRUE (avoid [AIX3.2] namespace pollution).
 * 	[1992/07/09  13:39:56  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:24:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*  macros.h
 * 
 * 
 *   --------------------------------------------------------------------------
 *  |   THE FOLLOWING PROGRAMS ARE THE SOLE PROPERTY OF APOLLO COMPUTER INC.   |
 *  |         AND CONTAIN ITS PROPRIETARY AND CONFIDENTIAL INFORMATION.        |
 *   --------------------------------------------------------------------------
 *
 *        ** OSF DCE Confidential - passwd etc DCE BL 1 **
 * VersionID = @(#)macros.h	prgy/ncs_2.0/3 - 09/26/90
 */

/*
** Useful macros
*/
#ifndef macros__included
#define macros__included

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#define TRUE 1
typedef unsigned long lboolean;
#endif

#define FAILURE 0
#define SUCCESS 1

#define PRIVATE static
#define INTERNAL static
#define PUBLIC

#define STATUS_OK(stp)            ((stp)==NULL || (*stp) == error_status_ok)
#define GOOD_STATUS(stp)          ((stp)==NULL || (*stp) == error_status_ok)
#define BAD_STATUS(stp)           ((*stp) != error_status_ok)
#define SET_STATUS(stp, val)      (*stp) = val
#define CLEAR_STATUS(stp)         (*stp) = error_status_ok
#define STATUS_EQUAL(stp, st_val) ((*stp) == (st_val))
#define STATUS(stp)               (*stp) 
#define STATUS_V(st)              (st)
#define COPY_STATUS(stp1, stp2)   (*stp2) = (*stp1)

#define FLAG_SET(v,f)       (((v) & (f)) == (f))
#define SET(v,f)            ((v) |= (f))
#define UNSET(v,f)          ((v) &= ~(f))

#define EQ_NIL(a)           { error_status_t lst; (uuid_is_nil(&(a), &lst)) }
#define BCOPY(a,b)          (bcopy(&(a), &(b), sizeof(a)))
#define EQ_STR(a, b, len)   (strncmp((a), (b), (len)) == 0)
#define EQ_OBJ(a, b, len)   (bcmp(&(a), &(b), (len)) == 0)

#define NEW(type)           (type *)malloc(sizeof(type))
#define DISPOSE(p)          free((char *) p)
#define MIN(a, b)           (a) > (b) ? (b) : (a)
#define MAX(a, b)           (a) > (b) ? (a) : (b)
#define DIFF(a, b)          (a) > (b) ? (a-b) : (b-a)    

#define NULL_STRING(s)      ((s) == NULL || *(s) == '\0')

#endif /* macros__included */
