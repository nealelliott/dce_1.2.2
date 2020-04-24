/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dcd_st_utils.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:13  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:54:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:08  root]
 * 
 * Revision 1.1.2.5  1994/07/21  21:26:53  fadden
 * 	No changes
 * 	[1994/07/21  20:46:42  fadden]
 * 
 * Revision 1.1.2.4  1994/06/28  21:53:56  fadden
 * 	Modify name of an arg in error ck macros to compile on RIOS
 * 	[1994/06/28  21:22:38  fadden]
 * 
 * Revision 1.1.2.3  1994/06/24  19:53:56  fadden
 * 	Merged with changes from 1.1.2.2
 * 	[1994/06/24  19:53:10  fadden]
 * 
 * 	Drop use of tst_result. Improve error reporting.
 * 	[1994/06/24  18:21:36  fadden]
 * 
 * Revision 1.1.2.2  1994/06/17  18:46:03  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:36:27  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/26  22:44:05  fadden
 * 	Move use of TET down one level
 * 	[1994/05/26  20:09:53  fadden]
 * 
 * 	Freeze working dtet version
 * 	[1994/05/13  15:42:15  fadden]
 * 
 * 	Initial Version
 * 	[1994/04/25  20:03:28  fadden]
 * 
 * $EndLog$
 */
/* 
 * NAME
 * 
 *   dcd_st_utils.h 
 *
 * COMPONENT
 *   
 *   DCE dced system test
 *   
 * ABSTRACT
 *
 *   This module provides an interface to the utility routines used in the 
 *   dced system tests.
 *
 */

#ifndef _DCD_ST_UTILS_H
#define _DCD_ST_UTILS_H

#include <dce/dce.h>
#include <dce/dce_error.h>

/*------------------------ Macros -------------------------------------------*/

#define MAX_DBG_LEV	12
#define DBG_NONE    0
#define DBG_FATAL   1
#define DBG_WARN    2
#define DBG_NOTE    3
#define DBG_LEV1    4
#define DBG_LEV2    5
#define DBG_LEV3    6
#define DBG_LEV4    7
#define DBG_LEV5    8
#define DBG_LEV6    9
#define DBG_LEV7    10
#define DBG_LEV8    11
#define DBG_DEFAULT 0
#define DBG_UNSET   -1

#define TET_PASS 0
#define TET_FAIL 1


/* Action codes for error checking */
#define CHK_CONT	0
#define CHK_RET		1

#define TST_CK_STATUS(st,level,string,x)  \
if ((st) != error_status_ok) \
{ \
    TSTDBG((level,"%s returned: %d\n",string,st));  \
    if (x==CHK_RET) return; \
}

#define TST_CK_DCE_STATUS(st,level,string,x)  \
if ((st) != error_status_ok) \
{ dce_error_string_t text; int ret; \
  dce_error_inq_text(st,text,&ret); \
  TSTDBG((level,"%s ",string));  \
  TSTDBG((level,"%s \n",text)); \
  if (ret != 0) TSTDBG((DBG_WARN,"dce_error_inq_text returned %d\n",ret));\
  if (x==CHK_RET) return; \
}

/*
 * These debug macros use tet_infoline to print to the TET journal
 * file (or to stdout when linked using the stub routine). 
 * The newline character will be added automatically.
 *
 * Usage:
 *  TSTDBG((DBG_NOTE, "This is a note message from %s", argv[0]));
 *
 * set new debug level:
 *  TSTDBG_SET_LEVEL(DBG_LEV4);
 *
 * get current debug level:
 *  TSTDBG_GET_LEVEL(&level);
 */


#  define TSTDBG(args)        tst_log_msg args
#  define TSTDBG_SET_LEVEL(x) tst_set_dbglevel(x)
#  define TSTDBG_GET_LEVEL(x) tst_get_dbglevel(x)


/*----------------------------- Prototypes ----------------------------------*/

void
server_setup(
    unsigned_char_t		*servername,    /* in */
    unsigned_char_t		*groupname,     /* in */
    rpc_if_handle_t		ifspec,         /* in */
    uuid_t                      *obj_uuid,      /* in */
    unsigned_char_t		*annotation,    /* in */
    boolean32			(*authzn)(),    /* in */
    rpc_binding_vector_t	**bind_vec,     /* out */
    error_status_t		*st             /* out */
);

void
server_takedown(
    uuid_t			*obj_uuid,	/* in */
    rpc_binding_vector_t	**bind_vec,	/* in */
    unsigned_char_t		*servername,	/* in */
    rpc_if_handle_t		ifspec,		/* in */
    error_status_t		*st		/* out */
);

void
create_new_bh_from_old(
    rpc_binding_handle_t     old_bh,            /* in  */
    uuid_t                   *uuid,             /* in  */
    rpc_binding_handle_t     *new_bh,           /* in, out */
    error_status_t           *st		/* out */    
);

void
dcd_st_alloc_uuid_vec(
    int                  count,             /* in  */
    uuid_vector_t        **ublock,          /* in, out */
    error_status_t       *st                /* out */
);

void
dcd_st_load_uuid_vec(
  uuid_vector_t *ublock,  /* in, out */
  error_status_t *st      /* out */
);

void
dcd_st_free_uuid_vec(
    uuid_vector_t        *ublock,          /* in, out */
    error_status_t       *st                /* out */
);


extern void 
tst_log_msg(
    int 		level, 
    char 		*fmt, 
    ...
);

extern void 
tst_set_dbglevel(
    int 		level
);

extern void 
tst_get_dbglevel(
    int 		*level
);

extern void 
tst_init_dbg(
    int 		level
);

#endif /* _DCD_ST_UTILS_H */
