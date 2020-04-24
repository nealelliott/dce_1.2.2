/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ek_glob.h,v $
 * Revision 1.1.8.1  1996/11/13  17:58:36  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/10/28  15:09 UTC  mullan_s  /main/mullan_dce12_tp_preauth/1]
 *
 * 	Fix duplicate enum name ("none").
 * 	[1996/03/11  13:26:56  marty  1.1.6.2]
 *
 * Revision 1.1.6.2  1996/03/11  13:26:56  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:52  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:59:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:12  root]
 * 
 * Revision 1.1.4.1  1994/07/15  14:58:23  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:14:04  mdf]
 * 
 * Revision 1.1.2.2  1992/12/29  12:40:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:21  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:46:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  ek_glob.h V=14 11/22/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit kernel - PRIVATE globals
** 
*/

#ifndef ek_glob_h_included
#define ek_glob_h_included

#include <dce/nbase.h>            

#include <macros.h>

#define ek_never_expire 0       /* a clockh_t value that the kernel interprets as
                                 * a null expiration date 
				 */
#define ek_live_forever 0       /* infinite lifespan     */
#define ek_clock_one_minute 60  /* seconds in one minute */
#define ek_clock_one_hour 3600  /* seconds in one hour   */
#define ek_clock_one_day  86400 /* seconds in one day    */
#define ek_clock_one_week (7 * ek_clock_one_day)
#define ek_clock_one_year (366 * ek_clock_one_day)


    /* used internally to distinguish between registry and org policy calls */
typedef enum {ek_policy_org, ek_policy_rgy} ek_policy_domain_t; 

    /* used internally to distinguish operations */
typedef enum {ek_op_query, ek_op_add, ek_op_change, ek_op_delete} ek_op_type_t;

    /* represents possible password policies contained in pwd_val_type ERA */
typedef enum {
    no_policy, 
    user_select, 
    user_can_select,
    generation_required
} ek_pwd_val_type_t;


   
/* The form module leaves it up to the user to interpret  a field's
 * attributes.  The interpretation is consistent across all rgy_edit kernel
 * modules.  The following definitions establish that consistency.  
 */
   
/* bit masks for operation field attributes */

#define attribute_readonly_mask   0x1  /* field not writable by interface */
#define attribute_readwrite_mask  0x2  /* ok for interface to write */
#define attribute_required_mask   0x4  /* can't perform operation unless the
					* field has been written
					*/
#define attribute_unused_mask     0x8  /* field not used by this operation */  

#define FIELD_READONLY(field_attributes) ((field_attributes) & \
					  attribute_readonly_mask)

#define FIELD_READWRITE(field_attributes) ((field_attributes) & \
					   attribute_readwrite_mask)

#define FIELD_UNUSED(field_attributes) (((field_attributes) & \
					 attribute_unused_mask))

#define FIELD_REQUIRED(field_attributes) ((field_attributes) & \
					  attribute_required_mask)

#define CHECK_FIELD(field_offset, num_fields) (((field_offset) >= 0) && \
					       ((field_offset) < (num_fields)))

error_status_t Dummy_st;     /* status placeholder for form_write calls */

#endif
