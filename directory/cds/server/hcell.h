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
 * $Log: hcell.h,v $
 * Revision 1.1.8.3  1996/02/18  23:34:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:59  marty]
 *
 * Revision 1.1.8.2  1995/12/08  15:31:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:26  root]
 * 
 * Revision 1.1.4.1  1994/10/26  19:22:41  zee
 * 	OT 12653: Add argument in function prototype for name_in_cell_test.
 * 	[1994/10/26  19:22:08  zee]
 * 
 * Revision 1.1.2.4  1994/08/03  19:04:19  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:25:43  mccann]
 * 
 * Revision 1.1.2.3  1994/06/09  18:43:33  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:06  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/06  16:04:21  zee
 * 	     HCell BL4 support.
 * 	[1994/05/05  21:20:06  zee]
 * 
 * Revision 1.1.2.1  1994/03/22  20:00:27  griffin
 * 	     HCell BL3 support: New module.
 * 	     [1994/03/22  18:07:54  griffin]
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1994. ALL RIGHTS RESERVED.
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
/*
 * hcell.h
 */

#ifndef HCELL_INCLUDED
#define HCELL_INCLUDED 1

typedef enum cdsNameRef {
    cdsLocalRef=1,                      /* Name is in current cell */
    cdsParentRef,                       /* Name is in parent cell  */
    cdsGDARef                           /* Name should go to GDA   */
  } cdsNameRef_t;

typedef enum AddAliasAs {
    AsNormal=1,                         /* As normal alias */
    AsPreferred,                        /* As preferred alias */
    AsChild,                            /* As child of current cell */
    AsInherit                           /* As inherited from parent cell */
  } AddAliasAs_t;

/*
 * Function prototypes - server_hcell_support.c
 */
dns_status_t
cell_relative_fullname (file_info_t *file_p,
                        FullName_t  *fname_in_p,
                        FullName_t  *relname_out_p);

int
cell_alias_test (file_info_t *file_p,
                 FullName_t  *TestName_p);

cdsNameRef_t
name_in_cell_test (file_info_t *file_p,
                   FullName_t  *TestName_p,
		   int          include_aliases);

int
match_fullnames (FullName_t *fname1_p,
                 FullName_t *fname2_p);

dns_status_t
transmogrify_cdsname (file_info_t *file_p,
                      FullName_t  *OldName_p,
                      FullName_t  *NewName_p);

dns_status_t
cell_full_to_simple (
    FullName_t   *cell_fullname,
    SimpleName_t *cell_simplename);

dns_status_t
cell_simple_to_full (
    SimpleName_t *cell_simplename,
    FullName_t   *cell_fullname);

void
transmogrify_ch (file_info_t *file_p);

int
get_preferred_member(DBSet_t *alias_set_p,
                     DBSetMember_t **preferred_member_pp);

/*
 * Function prototypes - server_hcell_maint.c
 */
dns_status_t
add_cell_alias (user_descriptor_t *user_p,
                uuid_t            *chid_p,
                AddAliasAs_t       how,
                FullName_t        *alias_p);

dns_status_t
remove_cell_alias (user_descriptor_t *user_p,
                   uuid_t            *chid_p,
                   FullName_t        *alias_p);

#endif  /* !HCELL_INCLUDED */
