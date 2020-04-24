/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdsp.h,v $
 * Revision 1.1.10.2  1996/02/18  23:38:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:58  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:36:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:27  root]
 * 
 * Revision 1.1.8.5  1994/08/10  08:25:53  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:16:29  marrek]
 * 
 * Revision 1.1.8.4  1994/05/10  16:04:12  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:52  marrek]
 * 
 * Revision 1.1.8.3  1994/03/23  15:49:12  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:10  keutel]
 * 
 * Revision 1.1.8.2  1994/02/24  15:03:35  marrek
 * 	Change rcs string format.
 * 	[1994/02/24  12:14:00  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:59:53  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:59:34  marrek]
 * 
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:32  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:03:23  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:11:33  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:21:01  marrek]
 * 
 * Revision 1.1.4.6  1993/02/02  09:00:30  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.5  1993/02/02  08:14:58  marrek
 * 	introduce dsP_thread_id()
 * 	[1993/02/01  14:14:49  marrek]
 * 
 * Revision 1.1.4.4  1992/12/30  23:30:50  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:20:48  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  17:37:05  marrek
 * 	November 1992 code drop
 * 	var args prototype for dsP_trace_msg
 * 	[1992/12/04  12:16:34  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:11:22  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  16:06:03  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:17:24  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:43:45  melman]
 * 
 * $EndLog$
 */
#ifndef _XDSP_H
#define _XDSP_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdsp_rcsid[] = "@(#)$RCSfile: xdsp.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:38:00 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xdsp.h                                              */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    : X/Open CAE Spec. API to Directory Services (XDS)    */
/*                X/Open CAE Spec. OSI-Abstract-Data Manip. API (XOM) */
/*                SNI XDS/XOM Design Specification November 1991      */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : declarations of functions and macros used           */
/*                internally by XDS                                   */ 
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 90-10-01| Original                       | VR |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


#include <xdsiapl.h>
#include <errno.h>
#include <assert.h>
#include <dsTrace.h> /* for DSP_ERROR_STRING_LEN */


extern int   errno;
extern char  *sys_errlist[];


#ifdef THREADSAFE
#	include <pthread.h>
#	include <d21iapl.h>		/* for d21*cancel_state() prototypes */
#endif



/* proprietary XDS definitions  */


/* this macro assumes the presence of OM workspace in the calling function */
#define CHECK_OMP_CALL(function, arg_list, status, err_code)                  \
    {                                                                         \
        OM_return_code rc;                                           \
                                                                              \
        if ((rc = function arg_list) != OM_SUCCESS) {                         \
            status =                                                          \
                dsP_handle_omP_error(rc, #function"()", err_code, workspace); \
            continue;                                                         \
        }                                                                     \
    }





/* this macro assumes the presence of OM workspace in the calling function */
#define CHECK_OMP_XALLOC(object, structr, type, status)                       \
    {                                                                         \
        if ((object = (structr OMP_XALLOC(workspace,                          \
                                                  (AL_##type),1))) == NULL) { \
            status =                                                          \
                dsP_handle_omP_error(                                         \
                    OM_MEMORY_INSUFFICIENT,                                   \
                    "OMP_XALLOC(" #object ")",                                \
                    DS_E_MISCELLANEOUS,                                       \
                    workspace);                                               \
            continue;                                                         \
        }                                                                     \
        memset(object, 0, sizeof(* object));                                  \
    }




#define CHECK_OMP_CREATE_ERROR(class, status, err_val, value)           \
    {                                                                   \
        xds_error   *x_error;                                           \
                                                                        \
        CHECK_OMP_XALLOC(                                               \
            x_error,                                                    \
            (xds_error *),                                              \
            XDS_ERROR,                                                  \
            status                                                      \
        )                                                               \
        x_error->val = value;                                           \
                                                                        \
        CHECK_OMP_CALL(                                                 \
            omP_create_handle,                                          \
            (class, workspace, x_error, & status),                  \
            status,                                                     \
            err_val                                                     \
        )                                                               \
        if (memcmp(class.elements, dsP_DS_C_LIBRARY_ERROR.elements,     \
                                   dsP_DS_C_LIBRARY_ERROR.length) == 0) \
			if ( 0 < value && value < DSP_ERROR_STRING_LEN) {			\
				TRACE_ERROR2("%s (%s) returned\n", "Library-Error", dsP_error_string[value - 1])  \
			} else {													\
				TRACE_ERROR2("%s (%d) returned\n", "unknown Library-Error", value)  \
			} /* endif */												\
        continue;                                                       \
    }



#ifdef THREADSAFE

#define INIT_ONCE                                                           \
    {                                                                        \
        if (pthread_once(&dsP_once, dsP_init_once) != 0) {                   \
            TRACE_ERROR2("%s: %s","cannot init-once", sys_errlist[errno]);  \
            exit(1);                                                         \
        }                                                                    \
    }

#define LOCK(mutex)                                                          \
    {                                                                        \
        if (pthread_mutex_lock(&mutex) != 0) {                               \
            TRACE_ERROR2("%s: %s","cannot lock " #mutex, sys_errlist[errno]);\
            exit(1);                                                         \
        }                                                                    \
    }

#define UNLOCK(mutex)                                                        \
    {                                                                        \
        if (pthread_mutex_unlock(&mutex) != 0) {                             \
            TRACE_ERROR2("%s: %s","cannot unlock " #mutex, sys_errlist[errno]);\
            exit(1);                                                         \
        }                                                                    \
    }

#else

#define INIT_ONCE           if (dsP_once) {      \
                                dsP_once = 0;    \
                                dsP_init_once(); \
                            }
#define LOCK(mutex)         ;
#define UNLOCK(mutex)       ;

#endif /* THREADSAFE */


#ifdef THREADSAFE
	#define ABANDON_PRELUDE_QUERY 										\
	{																	\
		int old_async_cancel_state;										\
		d21_store_old_cancel_state(pthread_setcancel(CANCEL_OFF));		\
		old_async_cancel_state = pthread_setasynccancel(CANCEL_OFF);	\
		TRY {

	#define ABANDON_PRELUDE_NONQUERY 									\
	{																	\
		int old_async_cancel_state;										\
		d21_store_old_cancel_state(pthread_setcancel(CANCEL_OFF));		\
		old_async_cancel_state = pthread_setasynccancel(CANCEL_OFF);

	#define ABANDON_INTERLUDE_QUERY										\
		} FINALLY {

	#define ABANDON_POSTLUDE_QUERY										\
		pthread_setcancel(d21_old_cancel_state());						\
		assert (pthread_setasynccancel(old_async_cancel_state) ==  CANCEL_OFF);\
	}ENDTRY																\
}

	#define ABANDON_POSTLUDE_NONQUERY									\
		pthread_setcancel(d21_old_cancel_state());						\
		assert (pthread_setasynccancel(old_async_cancel_state) ==  CANCEL_OFF);\
	}


#else /* THREADSAFE */

	#define ABANDON_PRELUDE_QUERY

	#define ABANDON_PRELUDE_NONQUERY

	#define ABANDON_INTERLUDE_QUERY

	#define ABANDON_POSTLUDE_QUERY

	#define ABANDON_POSTLUDE_NONQUERY

#endif /* THREADSAFE */										
	

#define D21_P1_NULL {NULL,0,NULL,NULL,NULL,NULL,0,0,NULL}
#define D21_P2_NULL {NULL,0}
#define D21_P3_NULL {NULL,NULL,0,NULL,NULL,NULL,NULL,NULL}
#define D21_P4_NULL {NULL,NULL,0,NULL,0,NULL,NULL,NULL,0,FALSE,NULL,NULL}
#define D21_P5_NULL {NULL,NULL,0,NULL,0,FALSE,NULL,NULL,0,NULL,NULL,NULL,0,FALSE,NULL,NULL}
#define D21_P6_NULL {NULL,NULL,0,NULL,NULL,NULL,FALSE,FALSE,NULL,NULL}

#define DSP_MAX_WKSPACES    16

typedef struct {
    OM_private_object       default_session;
    OM_private_object       default_context;
    OM_private_object       no_filter;
    OM_private_object       select_all_tv;
    OM_private_object       select_all_t;
    OM_private_object       select_none;
} dsP_wkspace_entry;


#define DSP_MAX_SESSIONS    32

typedef struct {
    OM_private_object       bound_session;
    OM_sint                 bind_id;
    OM_sint                 op_num;
} dsP_bind_entry;



#define SIZE32  sizeof(OM_uint32)* 8
#define SIZE16  sizeof(OM_uint16)* 8
#define SIZE8   8

#define XHASH(retcode, errcode, errval)                   \
    ((((OM_uint32)retcode) << (SIZE32 - SIZE8)) |    \
     (((OM_uint32)errcode) << (SIZE32 - SIZE16)) |    \
       (OM_uint16)errval)

typedef struct {
    OM_uint32		index;
    OM_uint16 		type;
    OM_uint16		value;
} dsP_error_entry;


/* global variables */

extern dsP_wkspace_entry        dsP_wkspace_table[];
extern dsP_bind_entry           dsP_bind_table[];
extern OM_uint32                dsP_def_auth_mech;

#ifdef THREADSAFE
extern pthread_once_t   dsP_once;
extern pthread_mutex_t  dsP_wksptab_mutex;
extern pthread_mutex_t  dsP_bindtab_mutex;
#else
extern OM_sint          dsP_once;
#endif


DS_status
dsP_handle_omP_error(
    const OM_return_code  rc,
    const char            *fn,
    const OM_sint             ec,
    const OM_workspace    workspace
);


OM_sint
dsP_verify_instance(
    OM_object               object,
    OM_object_identifier    class
);


OM_sint
dsP_validate_wkspace(
    OM_workspace   workspace,
    OM_sint        *index
);


OM_sint
dsP_validate_session(
    OM_private_object   session,
    OM_sint                 *bind_id,
    OM_sint                 *op_num
);


OM_sint
dsP_dec_op_num(
    OM_private_object   session
);


OM_sint
dsP_validate_context(
    OM_private_object   context
);


OM_sint
dsP_validate_name(
    OM_object           name,
    OM_private_object   *pri_name,
    DS_status           *status,
    OM_workspace        workspace
);


OM_sint
dsP_validate_selection(
    OM_object           selection,
    OM_private_object   *pri_selection,
    DS_status           *status,
    OM_workspace        workspace
);


OM_sint
dsP_xds2iapl_filter(
    xds_filter    *xds_filt,
    D2_Fi_filter **iapl_filt
);


OM_sint
dsP_count_filters(
    xds_filter    *xds_filt,
    OM_sint           *count
);


OM_sint
dsP_fill_filters(
    xds_filter    *x_f,
    D2_Fi_filter  *i_f,
    OM_sint            i_f_cur,
    OM_sint           *i_f_nxt
);


OM_sint
dsP_fill_filter_items(
    xds_fi_item  *x_fi,
    D2_Fi_filter *i_f,
    OM_sint          *i_f_nxt
);


OM_sint
dsP_free_iapl_filter(
    D2_Fi_filter  *iapl_filt
);


OM_sint
dsP_find_ete_entry(
    dsP_error_entry     *et,
    OM_sint             entry_num,
    OM_uint32 			ul,
    dsP_error_entry     **entry
);


OM_sint
dsP_iapl2xds_error(
    dsP_error_entry     *err_tab,
    OM_sint             entry_num,
    D2_pbhead           *pb,
    D2_error            *err,
    DS_status           *status,
    OM_workspace        workspace
);


OM_sint
dsP_get_psap_length(
    xds_psap psap_addr
);


void
dsP_init_once(
    void
);


#ifdef THREADSAFE
OM_sint
dsP_thread_id(
    void
);
#endif


/* declarations for tracing functions */

void
dsP_trace_init(
    void
);

OM_sint
dsP_display_object(
    const OM_sint         tid,
    const OM_object   arg,
    const char       *arg_str
);

OM_sint
dsP_display_feature(
    const DS_feature *f,
    const char       *arg
);

OM_sint
dsP_display_integer(
    const OM_sint    i,
    const char       *arg
);

OM_sint
dsP_display_boolean(
    const OM_sint    b,
    const char       *arg
);

void
dsP_trace_msg(
    const OM_sint   tid,
    const char 		*fmt,
    ...
);


#endif /* _XDSP_H */
