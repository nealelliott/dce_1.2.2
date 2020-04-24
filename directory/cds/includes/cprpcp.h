/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cprpcp.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:10  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:15:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:42  root]
 * 
 * Revision 1.1.4.5  1994/08/03  19:02:07  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:55:47  mccann]
 * 
 * Revision 1.1.4.4  1994/06/09  18:38:47  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:17  devsrc]
 * 
 * Revision 1.1.4.3  1994/04/07  10:22:18  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:18:02  kevins]
 * 
 * Revision 1.1.4.2  1994/03/12  22:02:34  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:12:54  peckham]
 * 
 * Revision 1.1.4.1  1994/02/02  20:37:08  rousseau
 * 	Change include of cprpc.h to dce/cprpc.h for CR 9865.
 * 	[1994/02/02  20:36:52  rousseau]
 * 
 * Revision 1.1.2.2  1992/12/30  13:28:57  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:46:40  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CPRPCP_H
#define _CPRPCP_H
/*
 * Module: cprpcp.h
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
*/
#include <stdio.h>
#include <dce/cprpc.h>

/*
 *****************************************************************************
 * miscellaneous defines
 */
#define CPRPC_MAX_CALLS   5

/*
 *****************************************************************************
 * EXTERNAL    
 *      Applied to variables that are external to a module.
 * GLOBAL
 *      Applied to defining instance of a variable.
 * PUBLIC
 *      Applied to (global) functions that are part of the defined API.
 * PRIVATE
 *      Applied to (global) functions that are NOT part of the defined API.
 * INTERNAL
 *      Applied to functions and variables that are private to a module.
 */

#ifndef EXTERNAL
#  define EXTERNAL      extern
#endif

#ifndef GLOBAL
#  define GLOBAL
#endif

#ifndef PUBLIC
#  define PUBLIC
#endif

#ifndef PRIVATE
#  define PRIVATE
#endif

#ifndef INTERNAL
#  define INTERNAL        static
#endif

/*
 *****************************************************************************
 * global pointer routines
 */
PUBLIC struct op_blk  *get_op_blk(void);
PUBLIC struct err_blk *get_err_blk_ptr(void);
PUBLIC struct msgblk  *get_msgblk_ptr(void);
PUBLIC struct envblk  *get_envblk_ptr(void);
PUBLIC struct entblk  *entitylist_pointer(void);
PUBLIC struct attrblk *get_attrblk_ptr(void);

/*
 *****************************************************************************
 * structure for emit_server_mumble() and emit_clerk_mumble() functions
 */
typedef struct _cprpc_emit_parms
  {
    cprpc_attr_list_t *attrp; /* ptr to attribute list */
    clh_info_list_t   *clhp;  /* ptr to clh information list */
    ns_info_list_t    *nsp;   /* ptr to ns information list */
    struct msgblk     *msgbp; /* ptr to message control block */
    struct op_blk     *outbp; /* ptr to output block */
    char              *keyp;  /* ptr to message key */
  } cprpc_emit_parms_t, *cprpc_emit_parms_p_t;

/*
 *****************************************************************************
 * structure for passing parameters to cp command functions
 */
typedef struct _cprpc_cmd_parm_blk
  {
      signed32 cmd;             /* cmd function code */
      signed32 caller_id;       /* caller's id (may be user id) */
      char *nodename;           /* desired/target host nodename */
      struct entblk  *entbp;    /* ptr to entity block */
      struct attrblk *atrbp;    /* ptr to attribute block */
      struct envblk  *envbp;    /* ptr to environment block */
      struct msgblk  *msgbp;    /* ptr to message block */
      struct op_blk  *outbp;    /* ptr to output flie list block */
  } cprpc_cmd_parm_blk_t, *cprpc_cmd_parm_blk_p_t;

/*
 *****************************************************************************
 * Lastly, include our utility definitions and prototypes include file.
 */
#include <dce/cprpc_util.h>

#endif  /* #ifndef _CPRPCP_H */
