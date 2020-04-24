/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_svg_prt.c,v $
 * Revision 1.1.17.4  1996/10/03  15:13:13  arvind
 * 	CHFts19728: support salvage of krbtgt private key values
 * 	[1996/08/27  16:02 UTC  aha  /main/DCE_1.2.2/aha_dbpk1_1/1]
 *
 * 	CHFts19728: support salvaging of krbtgt private key
 * 	[1996/08/27  15:33 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 *
 * 	Modifications to allow salvaging a krbtgt private key.
 * 	[1996/07/18  17:34 UTC  aha  /main/DCE_1.2.2/3]
 *
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/15  19:43 UTC  aha  /main/DCE_1.2.2/aha_pk6/2]
 *
 * 	sec_salvage_db -print failure if > 10 attributes
 * 	[1996/06/14  20:03 UTC  pdd  /main/DCE_1.2.2/pdd_dce_1_2/1]
 *
 * 	sec_salvage_db -print failure if > 10 attributes
 * 	[1996/05/24  23:18 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/11/07  23:28 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
 *
 * 	Remove code to take address of literal string in get_prop_str
 * 	HP 	[1995/07/11  18:19 UTC  jrr  /main/HPDCE02/6]
 *
 * 	Merge scalability work for sec_salvage_db.
 * 	HP 	[1995/07/06  18:05 UTC  jrr  /main/HPDCE02/hanfei_mothra_bl4/3]
 *
 * 	Fix memory corruption problems.
 * 	HP 	[1995/06/23  13:19 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/2]
 *
 * 	build acl separately from pgo to save memory
 * 	HP 	[1995/05/26  13:32 UTC  hanfei  /main/HPDCE02/5]
 *
 * 	merge.
 * 	HP 	[1995/05/25  17:06 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/1]
 *
 * 	modify to use RSDB_PGO and RSDB_DIR macros
 * 	HP 	[1995/05/15  21:11 UTC  greg  /main/HPDCE02/4]
 *
 * 	merge.
 * 	HP 	[1995/01/17  17:52 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bugfix/2]
 *
 * 	add prints for replist acls.
 * 	HP 	[1994/12/06  17:19 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bugfix/1]
 *
 * 	bug fix for OT 12759 : Null pointer checks.
 * 	HP 	[1994/10/27  15:29 UTC  hanfei  /main/HPDCE02/1]
 *
 * 	merge
 * 	HP 	[1994/10/25  17:26 UTC  hanfei  /main/hanfei_svg_bl1/4]
 *
 * 	Enhancement work for attribute schema and attributes.
 * 	HP 	[1994/10/12  18:29 UTC  hanfei  /main/hanfei_svg_bl1/3]
 *
 * 	coding work for printing attr_schema and attributes.
 * 	HP 	[1994/09/16  17:02 UTC  hanfei  /main/hanfei_svg_bl1/2]
 *
 * 	add codes to put DCE 1.1 stuff in info.prt for DCE 1.0.3.
 * 	HP 	[1994/09/14  19:22 UTC  hanfei  /main/hanfei_svg_bl1/1]
 * 	[1996/10/03  13:55:42  arvind]
 *
 * Revision 1.1.17.3  1996/08/09  12:11:55  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/15  19:43 UTC  aha  /main/DCE_1.2.2/aha_pk6/2]
 * 
 * 	Changes for Public Key Login; support Registry storage of KDC private key
 * 	[1996/06/18  20:56 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
 * 
 * 	sec_salvage_db -print failure if > 10 attributes
 * 	[1996/06/14  20:03 UTC  pdd  /main/DCE_1.2.2/pdd_dce_1_2/1]
 * 
 * 	sec_salvage_db -print failure if > 10 attributes
 * 	[1996/05/24  23:18 UTC  sommerfeld  /main/DCE_1.2.2/1]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/11/07  23:28 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
 * 
 * 	Remove code to take address of literal string in get_prop_str
 * 	HP 	[1995/07/11  18:19 UTC  jrr  /main/HPDCE02/6]
 * 
 * 	Merge scalability work for sec_salvage_db.
 * 	HP 	[1995/07/06  18:05 UTC  jrr  /main/HPDCE02/hanfei_mothra_bl4/3]
 * 
 * 	Fix memory corruption problems.
 * 	HP 	[1995/06/23  13:19 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/2]
 * 
 * 	build acl separately from pgo to save memory
 * 	HP 	[1995/05/26  13:32 UTC  hanfei  /main/HPDCE02/5]
 * 
 * 	merge.
 * 	HP 	[1995/05/25  17:06 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/1]
 * 
 * 	modify to use RSDB_PGO and RSDB_DIR macros
 * 	HP 	[1995/05/15  21:11 UTC  greg  /main/HPDCE02/4]
 * 
 * 	merge.
 * 	HP 	[1995/01/17  17:52 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bugfix/2]
 * 
 * 	add prints for replist acls.
 * 	HP 	[1994/12/06  17:19 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bugfix/1]
 * 
 * 	bug fix for OT 12759 : Null pointer checks.
 * 	HP 	[1994/10/27  15:29 UTC  hanfei  /main/HPDCE02/1]
 * 
 * 	merge
 * 	HP 	[1994/10/25  17:26 UTC  hanfei  /main/hanfei_svg_bl1/4]
 * 
 * 	Enhancement work for attribute schema and attributes.
 * 	HP 	[1994/10/12  18:29 UTC  hanfei  /main/hanfei_svg_bl1/3]
 * 
 * 	coding work for printing attr_schema and attributes.
 * 	HP 	[1994/09/16  17:02 UTC  hanfei  /main/hanfei_svg_bl1/2]
 * 
 * 	add codes to put DCE 1.1 stuff in info.prt for DCE 1.0.3.
 * 	HP 	[1994/09/14  19:22 UTC  hanfei  /main/hanfei_svg_bl1/1]
 * 	[1996/08/08  20:44:43  arvind]
 * 
 * Revision 1.1.17.2  1996/07/08  18:39:04  arvind
 * 	sec_salvage_db -print failure if > 10 attributes
 * 	[1996/06/14  20:03 UTC  pdd  /main/DCE_1.2.2/pdd_dce_1_2/1]
 * 
 * 	sec_salvage_db -print failure if > 10 attributes
 * 	[1996/05/24  23:18 UTC  sommerfeld  /main/DCE_1.2.2/1]
 * 
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:48 UTC  hanfei  /main/DCE_1.2/2]
 * 
 * 	multi-cell group work: add codes to print foreign principal group members.
 * 	[1996/03/14  18:44 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 * 
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:51  marty]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/11/07  23:28 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
 * 
 * 	Remove code to take address of literal string in get_prop_str
 * 	HP 	[1995/07/11  18:19 UTC  jrr  /main/HPDCE02/6]
 * 
 * 	Merge scalability work for sec_salvage_db.
 * 	HP 	[1995/07/06  18:05 UTC  jrr  /main/HPDCE02/hanfei_mothra_bl4/3]
 * 
 * 	Fix memory corruption problems.
 * 	HP 	[1995/06/23  13:19 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/2]
 * 
 * 	build acl separately from pgo to save memory
 * 	HP 	[1995/05/26  13:32 UTC  hanfei  /main/HPDCE02/5]
 * 
 * 	merge.
 * 	HP 	[1995/05/25  17:06 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/1]
 * 
 * 	modify to use RSDB_PGO and RSDB_DIR macros
 * 	HP 	[1995/05/15  21:11 UTC  greg  /main/HPDCE02/4]
 * 
 * 	merge.
 * 	HP 	[1995/01/17  17:52 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bugfix/2]
 * 
 * 	add prints for replist acls.
 * 	HP 	[1994/12/06  17:19 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bugfix/1]
 * 
 * 	bug fix for OT 12759 : Null pointer checks.
 * 	HP 	[1994/10/27  15:29 UTC  hanfei  /main/HPDCE02/1]
 * 
 * 	merge
 * 	HP 	[1994/10/25  17:26 UTC  hanfei  /main/hanfei_svg_bl1/4]
 * 
 * 	Enhancement work for attribute schema and attributes.
 * 	HP 	[1994/10/12  18:29 UTC  hanfei  /main/hanfei_svg_bl1/3]
 * 
 * 	coding work for printing attr_schema and attributes.
 * 	HP 	[1994/09/16  17:02 UTC  hanfei  /main/hanfei_svg_bl1/2]
 * 
 * 	add codes to put DCE 1.1 stuff in info.prt for DCE 1.0.3.
 * 	HP 	[1994/09/14  19:22 UTC  hanfei  /main/hanfei_svg_bl1/1]
 * 
 * 	codes for backward conversion from DCE 1.1 to DCE 1.0.3.
 * 	[1995/12/08  17:22:39  root]
 * 
 * Revision 1.1.17.1  1996/05/10  13:25:54  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:48 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/14  18:44 UTC  hanfei
 * 	multi-cell group work: add codes to print foreign principal group members.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:19 UTC  psn
 * 	Remove code to take address of literal string in get_prop_str
 * 	[1995/11/07  23:28 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
 * 
 * 	Merge scalability work for sec_salvage_db.
 * 	[1995/07/11  18:19 UTC  jrr  /main/HPDCE02/6]
 * 
 * 	Fix memory corruption problems.
 * 	[1995/07/06  18:05 UTC  jrr  /main/HPDCE02/hanfei_mothra_bl4/3]
 * 
 * 	build acl separately from pgo to save memory
 * 	[1995/06/23  13:19 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/2]
 * 
 * 	merge.
 * 	[1995/05/26  13:32 UTC  hanfei  /main/HPDCE02/5]
 * 
 * 	modify to use RSDB_PGO and RSDB_DIR macros
 * 	[1995/05/25  17:06 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/1]
 * 
 * 	Remove #define of  rs_c_master_info_file_version_dce103.
 * 	It is now defined in rs_pvt.h.
 * 	[1995/04/22  20:35 UTC  greg  /main/HPDCE02/SEC_migrate_merge/2]
 * 
 * 	Create SEC_migrate_merge mainline by merging SEC_migrate and HPDCE02.
 * 	[1995/04/20  20:48 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	merge more acl migration fixes.
 * 	[1995/04/20  17:12 UTC  greg  /main/HPDCE02/SEC_migrate/5]
 * 
 * 	Remove call to sec_acl_mgr_configure from print_init().  It's called
 * 	earlier on now.
 * 	[1995/04/19  20:36 UTC  greg  /main/HPDCE02/SEC_migrate/greg_sec_migrate_acls/1]
 * 
 * 	merge
 * 	[1995/04/07  15:13 UTC  hanfei  /main/HPDCE02/SEC_migrate/4]
 * 
 * 	add print and reconstruct for xattrschema acls
 * 	[1995/04/07  15:04 UTC  hanfei  /main/HPDCE02/SEC_migrate/hanfei_migrate_bl2/2]
 * 
 * 	don't print acl bit sec_acl_perm_svc if for dce1.0.3
 * 	[1995/04/05  15:40 UTC  hanfei  /main/HPDCE02/SEC_migrate/hanfei_migrate_bl2/1]
 * 
 * 	merge
 * 	[1995/02/22  18:01 UTC  hanfei  /main/HPDCE02/SEC_migrate/2]
 * 
 * 	work for new data types added in for dce1.1 migration work.
 * 	[1995/02/22  17:50 UTC  hanfei  /main/HPDCE02/hanfei_comp_bl2/1]
 * 
 * 	work for new data fields defined by migration codes
 * 	[1995/02/08  15:54 UTC  hanfei  /main/HPDCE02/SEC_migrate/1]
 * 
 * 	merge.
 * 	[1995/01/17  18:27 UTC  hanfei  /main/HPDCE02/3]
 * 
 * 	add prints for replist acls.
 * 	[1995/01/17  17:52 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bugfix/2]
 * 
 * 	bug fix for OT 12759 : Null pointer checks.
 * 	[1994/12/06  17:19 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bugfix/1]
 * 
 * 	merge
 * 	[1994/10/27  15:29 UTC  hanfei  /main/HPDCE02/1]
 * 
 * 	Enhancement work for attribute schema and attributes.
 * 	[1994/10/25  17:26 UTC  hanfei  /main/hanfei_svg_bl1/4]
 * 
 * 	coding work for printing attr_schema and attributes.
 * 	[1994/10/12  18:29 UTC  hanfei  /main/hanfei_svg_bl1/3]
 * 
 * 	add codes to put DCE 1.1 stuff in info.prt for DCE 1.0.3.
 * 	[1994/09/16  17:02 UTC  hanfei  /main/hanfei_svg_bl1/2]
 * 
 * 	codes for backward conversion from DCE 1.1 to DCE 1.0.3.
 * 
 * 	 *
 * 	[1994/09/14  19:22 UTC  hanfei  /main/hanfei_svg_bl1/1]
 * 
 * Revision 1.1.11.1  1994/10/26  15:17:34  hanfei
 * 	ERA enhancement work.
 * 	[1994/10/25  18:29:34  hanfei]
 * 
 * Revision 1.1.8.4  1994/09/20  19:18:45  hanfei
 * 	codes for backward conversion from DCE 1.1 to DCE 1.0.3
 * 	[1994/09/19  14:20:15  hanfei]
 * 
 * Revision 1.1.8.3  1994/06/17  18:42:58  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:20:08  devsrc]
 * 
 * Revision 1.1.8.2  1994/03/31  19:57:00  hanfei
 * 	work for ERA entries in policy.
 * 	[1994/03/31  19:49:41  hanfei]
 * 
 * Revision 1.1.8.1  1994/02/04  21:23:31  hanfei
 * 	fixed various memory leaks.
 * 	won't sort pgo and members as default behavior, -sort to turn on
 * 	fixed a bug : print_members won't print correctly if > 80 members
 * 	[1994/02/04  21:18:33  hanfei]
 * 
 * Revision 1.1.4.4  1993/03/30  22:10:21  hanfei
 * 	authentication keys's default pepper values are printed/reconstructed.
 * 	[1993/03/30  22:08:06  hanfei]
 * 
 * Revision 1.1.4.3  1993/03/23  23:02:29  hanfei
 * 	add codes for salvager reconstruct
 * 	[1993/03/23  22:48:41  hanfei]
 * 
 * Revision 1.1.4.2  1993/03/05  18:32:55  frisco
 * 	fix for RIOS port
 * 	[1993/03/03  20:01:24  frisco]
 * 
 * 	initial salvager functionality
 * 	[1993/03/02  21:23:57  frisco]
 * 
 * Revision 1.1.1.2  1992/11/25  21:11:35  frisco
 * 	Initial Revision
 * 
 * $EndLog$
 */

/*  
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
*/

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: sec_svg_prt.c,v 1.1.17.4 1996/10/03 15:13:13 arvind Exp $";
#endif

/* 
**      Rgy Salvager Tool Print Functions
*/
#include <sec_svg_util.h>
#include <sys/file.h>
#include <macros.h>
#include <sec_attr_tools.h>
#include <stdio.h>
#include <rsdb.h>  
#include <rsdb_create.h>  
#include <rsdb_util.h>  
#include <rsdb_acct.h>  
#include <rsdb_acl.h>  
#include <rsdb_file.h>  
#include <rsdb_auth.h>  
#include <rsdb_policy.h>  
#include <dce/sec_daclmgr.h>
#include <un_io.h>
#include <rs_pvt.h>
#include <rsdb_attr_pvt.h>
#include <sec_attr_base.h>
#include <rs_rep_pvt.h>
#include <rs_attr_base.h>

#include <sec_svg.h>
#include <sec_svg_prt.h>
#include <sec_svg_tokens.h>

extern boolean32 sort;
extern boolean32 sec_svg_dce103;

#define FILE_CREATION_PROTECTION S_IRUSR|S_IWUSR
#define DIR_CREATION_PROTECTION S_IRUSR|S_IWUSR|S_IXUSR
#define DCE103_RGY_FILE_VERSION  2
#define DCE103_RS_C_STATE_FILE_VERSION 6

PRIVATE sec_acl_mgr_handle_t acl_mgr_handle;
PRIVATE FILE                 *fp_dce103=NULL;

PRIVATE void print_attr_binding
  (
    FILE                             *fp,
    sec_attr_bind_info_t             *binding_p
  );

PUBLIC void print_init
#ifndef __STDC__
    (status)
    error_status_t *status;
#else
  (
    error_status_t *status
  )
#endif
{
    mkdir((char *) prt_path, DIR_CREATION_PROTECTION);
    rsdb_properties_get_info(&properties, status);

}


PUBLIC void print_open_info_file () 
{
    sec_rgy_pname_t prt_file;

    if ( ( sec_svg_dce103 == TRUE ) && ( fp_dce103 == NULL ) ) {
        strcpy((char *) prt_file, (char *) prt_path);
        strcat((char *) prt_file, PRT_FILE_INFO);
        fp_dce103 = fopen((char *) prt_file, "w");
        chmod((char *)prt_file, FILE_CREATION_PROTECTION);
    }
}

 
PUBLIC void print_close_info_file ()
{
        fclose(fp_dce103);
	fp_dce103 = NULL;
}


PRIVATE void init_node
#ifndef __STDC__
(node_ptr)
prt_list_item_t *node_ptr;
#else
  (
    prt_list_item_t *node_ptr
  )
#endif
{
    node_ptr->ptr = NULL;
    bzero(node_ptr->name, sizeof(sec_rgy_name_t));
    node_ptr->name_len = 0;
    node_ptr->internal_id = 0;
}

PRIVATE void insert_node
#ifndef __STDC__
    (head_ptr, new_ptr)
    prt_list_item_t *head_ptr;
    prt_list_item_t *new_ptr;
#else
  (
    prt_list_item_t *head_ptr,
    prt_list_item_t *new_ptr
  )
#endif
{
    boolean32 insert_here;
    prt_list_item_t *cur_ptr;
    prt_list_item_t *last_ptr;

    cur_ptr = head_ptr;
    insert_here = FALSE;
    while (cur_ptr != NULL) {
        if (strcmp((char *)new_ptr->name, (char *)cur_ptr->name) > 0) {
            last_ptr = cur_ptr;
            cur_ptr = cur_ptr->ptr;
            if (cur_ptr == NULL)
                insert_here = TRUE;
        }
        else
            insert_here = TRUE;
        if (insert_here) {
            /* insert node here */
            new_ptr->ptr = cur_ptr;
            last_ptr->ptr = new_ptr;
            break;
        }
    }
}

PRIVATE void insert_rep_node
#ifndef __STDC__
    (head_ptr, new_ptr)
    prt_rep_list_item_t *head_ptr;
    prt_rep_list_item_t *new_ptr;
#else
  (
    prt_rep_list_item_t *head_ptr,
    prt_rep_list_item_t *new_ptr
  )
#endif
{
    boolean32 insert_here;
    prt_rep_list_item_t *cur_ptr;
    prt_rep_list_item_t *last_ptr;

    cur_ptr = head_ptr;
    insert_here = FALSE;
    while (cur_ptr != NULL) {
        if (strcmp((char *)new_ptr->rep_info->rep_name, 
                   (char *)cur_ptr->rep_info->rep_name) > 0) {
            last_ptr = cur_ptr;
            cur_ptr = cur_ptr->ptr;
            if (cur_ptr == NULL)
                insert_here = TRUE;
        }
        else
            insert_here = TRUE;
        if (insert_here) {
            /* insert node here */
            new_ptr->ptr = cur_ptr;
            last_ptr->ptr = new_ptr;
            break;
        }
    }
}

PRIVATE void print_delim
#ifndef __STDC__
    (fp)
    FILE *fp;
#else
  (
    FILE *fp
  )
#endif
{
    int i;

    for (i=0;i<80;i++)
         fprintf(fp, "=");
    fprintf(fp, "\n");
}

PRIVATE void print_key
#ifndef __STDC__
    (fp, string, key, key_len)
    FILE *fp;
    char *string;
    char *key;
    long key_len;
#else
  (
    FILE *fp,
    char *string,
    char *key,
    long key_len
  )
#endif
{
    unsigned char prt_string[RSDB_ACCT_STRING_BUF_SIZE];
    int i;

    memcpy(prt_string, key, key_len);
    fprintf(fp, "%s = ", string);
    for (i = 0; i < key_len; i++)
        fprintf(fp, "%02x", prt_string[i]);
    fprintf(fp, "\n");
}

PRIVATE void print_tower
#ifndef __STDC__
    (fp, tower, tower_len)
    FILE *fp;
    idl_byte tower[];
    int tower_len;
#else
  (
    FILE *fp,
    idl_byte tower[],
    int tower_len
  )
#endif
{
    int length;
    int offset = 0;
    int i;

    fprintf(fp, "%s = %d\n", prt_Tower_Length, tower_len);
    fprintf(fp, "%s = ", prt_Tower);

    while (offset < tower_len) {
        if (offset > 0)
            fprintf(fp, "%s", tower_lead);
        if (tower_len - offset >= TOWER_MOD)
            length = TOWER_MOD;
        else
            length = tower_len - offset;
        for (i = 0; i < length; i++)
            fprintf(fp, "%02x", tower[offset + i]);
        fprintf(fp, "\n");
        offset = offset + length;
    }
}

PRIVATE void print_attr_value
  (
    FILE *fp,
    sec_attr_t       *attr_p
  )
{
   int	i,n, j;
   unsigned_char_t 		*uuid_string;
   error_status_t               status;

   switch(SA_ATTR_ENCODING(attr_p)) {
   case sec_attr_enc_any:
        fprintf(fp, "%s = %s\n", prt_Attr_Value_Type, prt_Attr_Value_Any);
	break;
   case sec_attr_enc_void:
        fprintf(fp, "%s = %s\n", prt_Attr_Value_Type, prt_Attr_Value_Void);
    	break;
   case sec_attr_enc_integer:
        fprintf(fp, "%s = %s\n", prt_Attr_Value_Type, prt_Attr_Value_Integer);
    	fprintf(fp, "%s = %d\n", prt_Attr_Value, (SA_ATTR_INTEGER(attr_p)));
   	break;
   case sec_attr_enc_printstring:
        fprintf(fp, "%s = %s\n", prt_Attr_Value_Type, prt_Attr_Value_Printstring);
    	fprintf(fp, "%s = %s\n", prt_Attr_Value, (SA_ATTR_PRINTSTRING_P(attr_p)));
    	break;
    case sec_attr_enc_printstring_array:
        fprintf(fp, "%s = %s\n", prt_Attr_Value_Type, prt_Attr_Value_Printstring_Array);
    	if(SA_ATTR_STR_ARRAY_P(attr_p)) {
	    fprintf(fp, "%s = %d\n", prt_Num_Attr_Printstring_Array_Entries,
		SA_ATTR_STR_ARRAY_NUM(attr_p));
    	    for(i = 0; i < SA_ATTR_STR_ARRAY_NUM(attr_p); i++) {
    		fprintf(fp, "%s = %s\n", prt_Attr_Value,
    		    (SA_ATTR_STR_ARRAY_ELT_P(attr_p, i)));
    	    }
    	}
	else
	    fprintf(fp, "%s = %d\n", prt_Num_Attr_Printstring_Array_Entries,
		SA_ATTR_STR_ARRAY_NUM(attr_p));
    	break;
    case sec_attr_enc_bytes:
        fprintf(fp, "%s = %s\n", prt_Attr_Value_Type, prt_Attr_Value_Encoding_Byte);
    	fprintf(fp, "%s = ", prt_Attr_Value);
    	for(i = 0; i < SA_ATTR_BYTES_LEN(attr_p); i++) {
    	    fprintf(fp, "%c", (SA_ATTR_BYTES_DATA(attr_p, i)));	
    	}
	fprintf(fp, "\n");
    	break;
    case sec_attr_enc_confidential_bytes:
        fprintf(fp, "%s = %s\n", prt_Attr_Value_Type, prt_Attr_Value_Confidential_Byte);
    	fprintf(fp, "%s = ", prt_Attr_Value);
    	for(i = 0; i < SA_ATTR_BYTES_LEN(attr_p); i++) {
    	    fprintf(fp, "%c", (SA_ATTR_BYTES_DATA(attr_p, i)));	
    	}
	fprintf(fp, "\n");
    	break;
    case sec_attr_enc_i18n_data:
    	if(SA_ATTR_IDATA_P(attr_p)) {
            fprintf(fp, "%s = %s\n", prt_Attr_Value_Type, prt_Attr_Value_I18n_Data);
   	    fprintf(fp, "%s = %d\n", prt_Attr_I18n_Codeset, (SA_ATTR_IDATA_CODESET(attr_p)));
    	    fprintf(fp, "%s = ", prt_Attr_Value);
    	    for(i = 0; i < SA_ATTR_IDATA_LEN(attr_p); i++) {
    	        fprintf(fp, "%c ", (SA_ATTR_IDATA_DATA(attr_p, i)));	
    	    }
	    fprintf(fp, "\n");
    	}
    	break;
    case sec_attr_enc_uuid:
        fprintf(fp, "%s = %s\n", prt_Attr_Value_Type, prt_Attr_Value_UUID);
    	uuid_to_string(&SA_ATTR_UUID(attr_p), &uuid_string, &status);
	if ( BAD_STATUS (&status) ) {
	    fprintf(stderr, "Cannot convert uuid to string\n");
	    return;
	}
    	fprintf(fp, "%s = %s\n", prt_Attr_Value, uuid_string);
    	break;
    case sec_attr_enc_attr_set:
    	if(SA_ATTR_SET_P(attr_p))  {
            fprintf(fp, "%s = %s\n", prt_Attr_Value_Type, prt_Attr_Value_Set);
	    fprintf(fp, "%s = %d\n", prt_Num_Attr_Set, SA_ATTR_SET_NUM(attr_p));
    	    for(i = 0; i < SA_ATTR_SET_NUM(attr_p); i++) {
    		uuid_to_string(&SA_ATTR_SET_MEMBERS(attr_p,i), &uuid_string, 
			&status);
    		fprintf(fp, "%s = %s\n", prt_Attr_Value, uuid_string);
    	    }
	}
    	break;
    case sec_attr_enc_binding:
    	if(SA_ATTR_BND_INFO_P(attr_p)) {
            fprintf(fp, "%s = %s\n", prt_Attr_Value_Type, prt_Attr_Value_Binding);
	    print_attr_binding(fp, SA_ATTR_BND_INFO_P(attr_p));
	}
    	break;
    default:
	fprintf(stderr, "Cannot print this attr value\n");
    	break;
    } /* end enc_type switch */

}

PRIVATE void print_attr_schema_aclmgr
  (
    FILE 			    *fp,
    char                            *schema_name,
    sec_attr_acl_mgr_info_set_t	    *aclmgr_p
  )
{
    error_status_t         lst;
    int                    i;
    rsdb_acl_mgr_type_t    mgr_type;
    uuid_t                 mgr_type_chain;
    sec_acl_printstring_t  mgr_pstring;
    boolean32              tokenize;
    unsigned32             num_pstrings;
    unsigned32             size_used;
    sec_acl_printstring_t  printstrings[MAX_ACL_PRINTSTRINGS];
    char                   new_string[BUFSIZE];
    char                   perm_string[BUFSIZE];

    new_string[0] = '\0';
    perm_string[0] = '\0';

    fprintf(fp, "%s = %d\n", prt_Num_Attr_Acl_Mgr_Entries, 
		aclmgr_p->num_acl_mgrs);

    for (i = 0; i < aclmgr_p->num_acl_mgrs; i++) {
	sec_acl_mgr_get_printstring(acl_mgr_handle, 
	    	&(aclmgr_p->mgr_info[i]->acl_mgr_type),
              	MAX_ACL_PRINTSTRINGS, &mgr_type_chain, &mgr_pstring, 
		&tokenize, &num_pstrings, &size_used, printstrings, 
		&lst);
	rsdb_acl_get_mgr_type_from_uuid(&aclmgr_p->mgr_info[i]->acl_mgr_type,
		&mgr_type, &lst);
	if ( BAD_STATUS(&lst ) ) {
	    error_st(lst, "Cannot get acl mgr type from uuid");
	}

	if ( ! rsdb_aclmgr_type_to_str(mgr_type, new_string) ) {
	    fprintf(stderr, "Error: cannot convert acl mgr type to string\n");
	}
    	fprintf(fp, "%s = %s\n", prt_Attr_Acl_Mgr_Type, new_string);

	rsdb_attr_aclmgr_to_str(aclmgr_p->mgr_info[i]->query_permset, 
	    perm_string, printstrings, num_pstrings);
    	fprintf(fp, "%s = %s\n", prt_Attr_Acl_Mgr_Query_Entry, perm_string);
        
	rsdb_attr_aclmgr_to_str(aclmgr_p->mgr_info[i]->update_permset, 
	    perm_string, printstrings, num_pstrings);
    	fprintf(fp, "%s = %s\n", prt_Attr_Acl_Mgr_Update_Entry, perm_string);

	rsdb_attr_aclmgr_to_str(aclmgr_p->mgr_info[i]->test_permset, 
	    perm_string, printstrings, num_pstrings);
    	fprintf(fp, "%s = %s\n", prt_Attr_Acl_Mgr_Test_Entry, perm_string);

	rsdb_attr_aclmgr_to_str(aclmgr_p->mgr_info[i]->delete_permset, 
	    perm_string, printstrings, num_pstrings);
    	fprintf(fp, "%s = %s\n", prt_Attr_Acl_Mgr_Delete_Entry, perm_string);

    }

}

PRIVATE void print_acl
#ifndef __STDC__
    (cell_label, entry_label, fp, acl_id)
    char *cell_label;
    char *entry_label;
    FILE *fp;
    rsdb_pvt_id_t acl_id;
#else
  (
    char *cell_label,
    char *entry_label,
    FILE *fp,
    rsdb_pvt_id_t acl_id
  )
#endif
{
    char label[BUFSIZE];
    rsdb_acl_t *acl;
    error_status_t status;
    unsigned_char_p_t uuid_p;
    boolean32 is_foreign = false;
    int i;
    uuid_t              mgr_type_chain;
    sec_acl_printstring_t mgr_pstring;
    boolean32           tokenize;
    unsigned32          total_pstrings;
    unsigned32          size_used;
    sec_acl_printstring_t printstrings[MAX_ACL_PRINTSTRINGS];
    prt_list_item_t *entry_list_ptr;
    prt_list_item_t *old_ptr;
    prt_list_item_t *new_ptr;

    entry_list_ptr = NEW(prt_list_item_t);
    init_node(entry_list_ptr);

    rsdb_acl_get_by_id(acl_id, &acl, &status);

    strcpy((char *) label, cell_label);
    if (uuid_equal(&(acl->default_realm.uuid), &properties.realm_uuid, &status)) {
        strcat((char *) label, prt_Name);
        fprintf(fp, "%s = %s\n", label, properties.realm);
    }
    else {        /* foreign cell */
        strcat((char *) label, prt_UUID);
        uuid_to_string(&(acl->default_realm.uuid), &uuid_p, &status);
        fprintf(fp, "%s = %s\n", label, uuid_p);
        if (uuid_p != NULL)
            rpc_string_free(&uuid_p, &status);
        is_foreign = true;
    }

    sec_acl_mgr_get_printstring(acl_mgr_handle, &(acl->sec_acl_manager_type), 
              MAX_ACL_PRINTSTRINGS, &mgr_type_chain, &mgr_pstring, &tokenize, 
              &total_pstrings, &size_used, printstrings, &status);
    fprintf(fp, "%s = %d\n", prt_Num_Acl_Entries, acl->num_entries);

    for (i = 0; i < acl->num_entries; i++) {
        new_ptr = NEW(prt_list_item_t);
	if ( sec_svg_dce103 == TRUE ) {
	/* don't print servicibility bit for backward compatibility */
	    if  ( acl->entries[i].perms & sec_acl_perm_svc ) {
		    acl->entries[i].perms -= sec_acl_perm_svc;
	    }
	}
        rsdb_acl_entry_to_str(is_foreign, acl->entries[i], 
                  (char *) new_ptr->name, printstrings, total_pstrings);
        insert_node(entry_list_ptr, new_ptr);
    }

    /* print each acl entry */
    new_ptr = entry_list_ptr->ptr;
    while (new_ptr != NULL) {
        old_ptr = new_ptr;
        fprintf(fp, "%s = %s\n", entry_label, new_ptr->name);
        new_ptr = new_ptr->ptr;
	if ( old_ptr != NULL )
	    free(old_ptr);
    }
    if (entry_list_ptr != NULL )
        free(entry_list_ptr);

    free(acl);

}

PRIVATE void print_passwd_policy
#ifndef __STDC__
    (fp, name)
    FILE *fp;
    sec_rgy_name_t name;
#else
  (
    FILE *fp,
    sec_rgy_name_t name
  )
#endif
{
    sec_rgy_plcy_t org_policy;
    char time_string[TIME_STRING_LEN];
    char lifetime_string[LIFETIME_STRING_LEN];
    error_status_t status;

    rsdb_policy_get_info(name, &org_policy, &status);
    if (GOOD_STATUS(&status)) {
        period_to_dt_string(org_policy.acct_lifespan, (char *) lifetime_string);
        fprintf(fp, "%s = %s\n", prt_Acct_Lifetime, lifetime_string);
        fprintf(fp, "%s = %d\n", prt_Passwd_Min_Len, org_policy.passwd_min_len);
        period_to_dt_string(org_policy.passwd_lifetime, (char *) lifetime_string);
        fprintf(fp, "%s = %s\n", prt_Passwd_Lifetime, lifetime_string);
        sec_to_dt_string(org_policy.passwd_exp_date, (char *) time_string);
        fprintf(fp, "%s = %s\n", prt_Passwd_Expire_Date, time_string);

        if (IS_SET(org_policy.passwd_flags, sec_rgy_plcy_pwd_no_spaces))
            fprintf(fp, "%s = %s\n", prt_Passwd_All_Spaces_Ok, prt_false);
        else
            fprintf(fp, "%s = %s\n", prt_Passwd_All_Spaces_Ok, prt_true);
        if (IS_SET(org_policy.passwd_flags, sec_rgy_plcy_pwd_non_alpha))
            fprintf(fp, "%s = %s\n", prt_Passwd_All_Alphanumeric_Ok, prt_false);
        else
            fprintf(fp, "%s = %s\n", prt_Passwd_All_Alphanumeric_Ok, prt_true);
    }

}

PRIVATE void print_creator_changer
#ifndef __STDC__
    (fp, admin_item, status)
    FILE *fp;
    sec_rgy_acct_admin_t *admin_item;
    error_status_t *status;
#else
  (
    FILE *fp,
    sec_rgy_acct_admin_t *admin_item,
    error_status_t *status
  )
#endif
{
    sec_rgy_name_t name;
    long name_len;
    rsdb_credentials_t creds;
    char time_string[TIME_STRING_LEN];
    unsigned_char_p_t uuid_p;

    if (uuid_equal(&admin_item->creator.cell, 
        &properties.realm_uuid, status)) {
                  /* person in this cell */
        rsdb_get_credentials_by_uid(rsdb_person_db, &admin_item->creator.principal,
                              &creds);
        rsdb_util_path_construct(sec_rgy_domain_person, creds.id, name, &name_len,
                              status);
        fprintf(fp, "%s = %s\n", prt_Acct_Creator_Name, name);
    }
    else {        /* foreign principal */
        uuid_to_string(&(admin_item->creator.cell), &uuid_p, status);
        fprintf(fp, "%s = %s\n", prt_Acct_Creator_Cell_UUID, uuid_p);
        if (uuid_p != NULL)
            rpc_string_free(&uuid_p, status);

        uuid_to_string(&(admin_item->creator.principal), &uuid_p, status);
        fprintf(fp, "%s = %s\n", prt_Acct_Creator_Person_UUID, uuid_p);
        if (uuid_p != NULL)
            rpc_string_free(&uuid_p, status);
    }
    sec_to_dt_string(admin_item->creation_date, time_string);
    fprintf(fp, "%s = %s\n", prt_Acct_Creation_Time, time_string);

    if (uuid_equal(&admin_item->last_changer.cell, 
        &properties.realm_uuid, status)) {
                  /* person in this cell */
        rsdb_get_credentials_by_uid(rsdb_person_db, 
                              &admin_item->last_changer.principal, &creds);
        rsdb_util_path_construct(sec_rgy_domain_person, creds.id, name, 
                              &name_len, status);
        fprintf(fp, "%s = %s\n", prt_Acct_Changer_Name, name);
    }
    else {        /* foreign principal */
        uuid_to_string(&(admin_item->last_changer.cell), &uuid_p, status);
        fprintf(fp, "%s = %s\n", prt_Acct_Changer_Cell_UUID, uuid_p);
        if (uuid_p != NULL)
            rpc_string_free(&uuid_p, status);

        uuid_to_string(&(admin_item->last_changer.principal), &uuid_p, status);
        fprintf(fp, "%s = %s\n", prt_Acct_Changer_Person_UUID, uuid_p);
        if (uuid_p != NULL)
            rpc_string_free(&uuid_p, status);
    }
    sec_to_dt_string(admin_item->change_date, time_string);
    fprintf(fp, "%s = %s\n", prt_Acct_Change_Time, time_string);

}

PRIVATE void print_foreign_member
  (
    FILE *fp,
    prt_list_item_t	*ptr,
    error_status_t      *stp
  )
{
    unsigned_char_p_t	uuid_str_p;

    fprintf(fp, "%s = /%s\n", prt_Foreign_Member_Name, ptr->name);
    /* print the  cell and principal uuids */
    uuid_to_string(&ptr->cell_uuid, &uuid_str_p, stp);
    fprintf(fp, "%s = %s\n", prt_Cell_UUID, uuid_str_p);
    if (uuid_str_p != NULL) {
        rpc_string_free(&uuid_str_p, stp);
    }
    uuid_to_string(&ptr->princ_uuid, &uuid_str_p, stp);
    fprintf(fp, "%s = %s\n", prt_Princ_UUID, uuid_str_p);
    if (uuid_str_p != NULL) {
        rpc_string_free(&uuid_str_p, stp);
    }
}

PRIVATE void print_members
#ifndef __STDC__
    (fp, domain, data_domain, list_id, num_members)
    FILE *fp;
    sec_rgy_domain_t domain;
    sec_rgy_domain_t data_domain;
    rsdb_pvt_id_t list_id;
    long num_members;
#else
  (
    FILE *fp,
    sec_rgy_domain_t domain,
    sec_rgy_domain_t data_domain,
    rsdb_pvt_id_t list_id,
    long num_members
  )
#endif
{
    rsdb_max_membership_list_t member_list;
    int current_count;
    int i;
    error_status_t status;
    prt_list_item_t *mem_list_ptr;
    prt_list_item_t *old_ptr;
    prt_list_item_t *new_ptr;
    sec_rgy_domain_t name_domain;

    mem_list_ptr = NEW(prt_list_item_t);
    init_node(mem_list_ptr);

    if (domain == sec_rgy_domain_person)
        name_domain = sec_rgy_domain_group;
    else 
	if ( domain == sec_rgy_domain_foreign_person ) {
	    name_domain = domain;
	}
	else {
            name_domain = sec_rgy_domain_person;
	}

    current_count = 0;
    while (current_count < num_members) {
        rsdb_util_get_member_list(data_domain, list_id, &member_list);
        for (i = 0; i < member_list.total_count; i++) {
            if (!member_list.member[i].deleted) {
                new_ptr = NEW(prt_list_item_t);
                rsdb_util_path_construct(name_domain, 
                    (rsdb_pvt_id_t)member_list.member[i].id, 
                    new_ptr->name, &new_ptr->name_len, &status);
		if ( domain == sec_rgy_domain_foreign_person ) {
		/* get cell and princ uuids for this foreign principal */
		    rsdb_get_credentials_uuids_by_pvt_id(
		    		domain_info[domain].db, 
				(rsdb_pvt_id_t)member_list.member[i].id,
				&new_ptr->cell_uuid, &new_ptr->princ_uuid);
		}

	    	if ( sort != TRUE ) {
		    if ( domain == sec_rgy_domain_foreign_person ) {
			print_foreign_member(fp, new_ptr, &status);
		    } else {
         	    	fprintf(fp, "%s = %s\n", prt_Member_Name, new_ptr->name);
		    }
		    free(new_ptr);
	    	}
	    	else {
                    insert_node(mem_list_ptr, new_ptr);
		}
            }
        }
        current_count = current_count + member_list.count;
	list_id = member_list.next;
    }

    if ( sort == TRUE ) {
    /* print each member */
	new_ptr = mem_list_ptr->ptr;
   	while (new_ptr != NULL) {
            old_ptr = new_ptr;
	    if ( domain == sec_rgy_domain_foreign_person ) {
	        print_foreign_member(fp, new_ptr, &status);
	    } else {
                fprintf(fp, "%s = %s\n", prt_Member_Name, new_ptr->name);
	    }
            new_ptr = new_ptr->ptr;
	    if ( old_ptr != NULL )
	        free(old_ptr);
	}
    }
    if (mem_list_ptr != NULL )
        free (mem_list_ptr);

}

PRIVATE void print_authkey
#ifndef __STDC__
    (fp, acct_item, key_item, current, status)
    FILE *fp;
    rsdb_acct_item_t *acct_item;
    rsdb_authkey_item_t *key_item;
    boolean32 current;
    error_status_t *status;
#else
  (
    FILE *fp,
    rsdb_acct_item_t *acct_item,
    rsdb_authkey_item_t *key_item,
    boolean32 current,
    error_status_t *status
  )
#endif
{
    char *pepper_in;
    short pepper_len_in;
    boolean32 default_pepper;
    char prt_string[RSDB_ACCT_STRING_BUF_SIZE];
    char time_string[TIME_STRING_LEN];
    krb5_keyblock               dkey;
    krb5_encrypted_keyblock     ekey;
    krb5_data                   pepper;
    sec_rgy_name_t              pname;
    long                        name_len;

    if (current) {
        pepper_in = ACCT_PEPPER(acct_item);
        pepper_len_in = acct_item->key_pepper_len;
        default_pepper = acct_item->default_pepper;
    }
    else {
        pepper_in = key_item->buf;
        pepper_len_in = key_item->pepper_len;
        default_pepper = key_item->default_pepper;
    }

    if (!default_pepper) {
        if (pepper_in != NULL) {
	    strncpy(prt_string, pepper_in, pepper_len_in);
	    prt_string[pepper_len_in] = '\0';
            fprintf(fp, "%s = %s\n", prt_Auth_Key_Pepper, prt_string);
        }
    }
    else {
	pepper.data = NULL;
	pepper.length = 0;
	rsdb_util_path_construct(sec_rgy_domain_person, acct_item->person_id,  
				 pname, &name_len, status);
	rsdb_acct_gen_default_pepper(pname, &pepper, status );
	if(BAD_STATUS(status)) 
	    return;
	strncpy(prt_string, pepper.data, pepper.length);
        prt_string[pepper.length] = '\0';
        fprintf(fp, "%s = %s\n", prt_Auth_Key_Pepper, prt_string);
	if (pepper.data)
	    free((char *)pepper.data);
    }
    
    /* decrypt the key so master_key effect is out */
    if (current) {
        ekey.contents = (krb5_octet *)ACCT_KEY(acct_item);
        ekey.length = acct_item->key_len;
    }
    else {
        ekey.contents = (krb5_octet *)&(key_item->buf[key_item->pepper_len]);
        ekey.length = key_item->key_len;
    }
    ekey.keytype = RSDB_AUTH_DEFAULT_KEY_TYPE;
    rs_mkey_decrypt_with_master(acct_item->mkey_version, &ekey, &dkey, status);

    fprintf(fp, "%s = %d\n", prt_Auth_Key_Len, dkey.length);
    print_key(fp, prt_Auth_Key, (char *)dkey.contents, dkey.length);
    if (dkey.contents) {
    	memset((char *)dkey.contents, 0, dkey.length);
        free((char *)dkey.contents);
    }

    if (current) {
        fprintf(fp, "%s = %s\n", prt_Auth_Key_Expire_Time, prt_none);
    }
    else {
        sec_to_dt_string(key_item->key_expire_time, time_string);
        fprintf(fp, "%s = %s\n", prt_Auth_Key_Expire_Time, 
                                 time_string);
    }
    
}

PRIVATE void print_authkeys
#ifndef __STDC__
    (fp, acct_item, acct_id, status)
    FILE *fp;
    rsdb_acct_item_t *acct_item;
    rsdb_pvt_id_t acct_id;
    error_status_t *status;
#else
  (
    FILE *fp,
    rsdb_acct_item_t *acct_item,
    rsdb_pvt_id_t acct_id,
    error_status_t *status
  )
#endif
{
    int i;
    sec_passwd_version_t next_vers;
    rsdb_authkey_item_t next_key;

    /* Count authkeys */
    i = 1;
    if (PK_AUTHKEY_EXISTS(acct_item))
        i++;
    if (acct_item->next_key_version != sec_passwd_c_version_none) {
        next_vers = (sec_passwd_version_t)acct_item->next_key_version;
        while (next_vers != sec_passwd_c_version_none) {
	    i++;
            rsdb_auth_get_by_authkey_key(next_vers, RSDB_AUTH_DEFAULT_KEY_TYPE,
                  acct_id, &next_key, status);
            next_vers = next_key.next_version;
        }

    }      
    fprintf(fp, "%s = %d\n", prt_Num_Auth_Keys, i);
    
    /* print current auth key */
    fprintf(fp, "%s = %d\n", prt_Auth_Key_Version, acct_item->cur_key_version);
    print_authkey(fp, acct_item, NULL, TRUE, status);

    /* print previous default auth keys (if any) */
    if (acct_item->next_key_version != sec_passwd_c_version_none) {
        next_vers = (sec_passwd_version_t)acct_item->next_key_version;

        while (next_vers != sec_passwd_c_version_none) {
            fprintf(fp, "%s = %d\n", prt_Auth_Key_Version, next_vers);
            fprintf(fp, "%s = %s\n", prt_Auth_Key_Type, prt_des);
            rsdb_auth_get_by_authkey_key(next_vers, RSDB_AUTH_DEFAULT_KEY_TYPE,
                  acct_id, &next_key, status);
            
            print_authkey(fp, acct_item, &next_key, FALSE, status);
            next_vers = next_key.next_version;
        }
    }

    /* Print non-default authkey (if any) */
    if (PK_AUTHKEY_EXISTS(acct_item)) {
        fprintf(fp, "%s = %d\n", prt_Auth_Key_Version, 1);
        fprintf(fp, "%s = %s\n", prt_Auth_Key_Type, prt_keytype_encrypt_rsa_priv);
        rsdb_auth_get_by_authkey_key(1, KEYTYPE_ENCRYPT_RSA_PRIV,
              acct_id, &next_key, status);
        print_authkey(fp, acct_item, &next_key, FALSE, status);
    }        
}

PRIVATE void print_acct
#ifndef __STDC__
    (fp, id)
    FILE *fp;
    rsdb_pvt_id_t id;
#else
  (
    FILE *fp,
    rsdb_pvt_id_t id
  )
#endif
{
    rsdb_acct_item_t acct_item;
    sec_rgy_name_t name;
    long name_len;
    char prt_string[RSDB_ACCT_STRING_BUF_SIZE];
    error_status_t status;
    char time_string[TIME_STRING_LEN];

    if (rsdb_get_acct_by_id(id, &acct_item) && id != -1) {

        rsdb_util_path_construct(sec_rgy_domain_group, acct_item.group_id,  
            name, &name_len, &status);
        fprintf(fp, "%s = %s\n", prt_Acct_Group_Name, name);
        
        rsdb_util_path_construct(sec_rgy_domain_org, acct_item.org_id,  
            name, &name_len, &status);
        fprintf(fp, "%s = %s\n", prt_Acct_Org_Name, name);
        
        print_creator_changer(fp, &acct_item.admin_part, &status);

        sec_to_dt_string(acct_item.admin_part.expiration_date, time_string);
        fprintf(fp, "%s = %s\n", prt_Acct_Expire_Time, time_string);
        sec_to_dt_string(acct_item.admin_part.good_since_date, time_string);
        fprintf(fp, "%s = %s\n", prt_Acct_Good_Since_Time, time_string);
    
             /* acct admin flags */
        if (IS_SET(acct_item.admin_part.flags, sec_rgy_acct_admin_valid))
            fprintf(fp, "%s = %s\n", prt_Acct_Valid_For_Login_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Acct_Valid_For_Login_Flag, prt_false);
    
        if (IS_SET(acct_item.admin_part.flags, sec_rgy_acct_admin_server))
            fprintf(fp, "%s = %s\n", prt_Acct_Valid_As_Server_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Acct_Valid_As_Server_Flag, prt_false);
    
        if (IS_SET(acct_item.admin_part.flags, sec_rgy_acct_admin_client))
            fprintf(fp, "%s = %s\n", prt_Acct_Valid_As_Client_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Acct_Valid_As_Client_Flag, prt_false);
    
             /* acct admin auth flags */
        if (IS_SET(acct_item.admin_part.authentication_flags, 
                   sec_rgy_acct_auth_post_dated))
            fprintf(fp, "%s = %s\n", prt_Acct_Post_Dated_Cert_Ok_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Acct_Post_Dated_Cert_Ok_Flag, prt_false);
    
        if (IS_SET(acct_item.admin_part.authentication_flags, 
                   sec_rgy_acct_auth_forwardable))
            fprintf(fp, "%s = %s\n", prt_Acct_Forwardable_Cert_Ok_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Acct_Forwardable_Cert_Ok_Flag, prt_false);
    
        if (IS_SET(acct_item.admin_part.authentication_flags, sec_rgy_acct_auth_tgt))
            fprintf(fp, "%s = %s\n", prt_Acct_TGT_Auth_Cert_Ok_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Acct_TGT_Auth_Cert_Ok_Flag, prt_false);
    
        if (IS_SET(acct_item.admin_part.authentication_flags, 
                   sec_rgy_acct_auth_renewable))
            fprintf(fp, "%s = %s\n", prt_Acct_Renewable_Cert_Ok_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Acct_Renewable_Cert_Ok_Flag, prt_false);
    
        if (IS_SET(acct_item.admin_part.authentication_flags, 
                   sec_rgy_acct_auth_proxiable))
            fprintf(fp, "%s = %s\n", prt_Acct_Proxiable_Cert_Ok_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Acct_Proxiable_Cert_Ok_Flag, prt_false);
    
        if (IS_SET(acct_item.admin_part.authentication_flags, 
                   sec_rgy_acct_auth_dup_skey))
            fprintf(fp, "%s = %s\n", prt_Acct_Dup_Session_Key_Ok_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Acct_Dup_Session_Key_Ok_Flag, prt_false);
    
             /* Unix Info */
        fprintf(fp, "%s = %s\n", prt_Unix_Key, acct_item.user_part.passwd);
	fprintf(fp, "%s = \n", prt_Plaintext_Passwd);
        strncpy(prt_string, acct_item.string_buf, acct_item.user_part.homedir_len);
        prt_string[acct_item.user_part.homedir_len] = '\0';
        fprintf(fp, "%s = %s\n", prt_Home_Dir, prt_string);
        strncpy(prt_string, ACCT_SHELL(&acct_item), acct_item.user_part.shell_len);
        prt_string[acct_item.user_part.shell_len] = '\0';
        fprintf(fp, "%s = %s\n", prt_Shell, prt_string);
        strncpy(prt_string, ACCT_GECOS(&acct_item), acct_item.user_part.gecos_len);
        prt_string[acct_item.user_part.gecos_len] = '\0';
        fprintf(fp, "%s = %s\n", prt_Gecos, prt_string);
    
        if (IS_SET(acct_item.user_part.flags, sec_rgy_acct_user_passwd_valid))
            fprintf(fp, "%s = %s\n", prt_Passwd_Valid_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Passwd_Valid_Flag, prt_false);
        sec_to_dt_string(acct_item.user_part.passwd_dtm, time_string);
        fprintf(fp, "%s = %s\n", prt_Passwd_Change_Time, time_string);

        if (acct_item.auth_plcy.max_ticket_lifetime == 
                RSDB_DEF_AUTH_POL_MAX_TKT_LIFE)
            fprintf(fp, "%s = %s\n", prt_Max_Certificate_Lifetime, 
                    prt_default_policy);
        else
            fprintf(fp, "%s = %d\n", prt_Max_Certificate_Lifetime, 
                    acct_item.auth_plcy.max_ticket_lifetime);

        if (acct_item.auth_plcy.max_ticket_lifetime == 
                RSDB_DEF_AUTH_POL_MAX_RENEW_LIFE)
            fprintf(fp, "%s = %s\n", prt_Max_Renewable_Lifetime, 
                    prt_default_policy);
        else
            fprintf(fp, "%s = %d\n", prt_Max_Renewable_Lifetime, 
                    acct_item.auth_plcy.max_renewable_lifetime);
    
        fprintf(fp, "%s = %d\n", prt_Master_Key_Version, acct_item.mkey_version);

        print_authkeys(fp, &acct_item, id, &status);

    }

}

PRIVATE void print_dir
#ifndef __STDC__
    (domain, list_ptr, rec_num, fp)
    sec_rgy_domain_t domain;
    prt_list_item_t *list_ptr;
    int rec_num;
    FILE *fp;
#else
  (
    sec_rgy_domain_t domain,
    prt_list_item_t *list_ptr,
    int rec_num,
    FILE *fp
  )
#endif
{
    rsdb_named_item_t item;
    unsigned_char_p_t uuid_p;
    error_status_t status;

    rsdb_util_get_item_by_id(domain, list_ptr->internal_id, &item);
    uuid_to_string(&((RSDB_DIR(&item)).uid), &uuid_p, &status);
    fprintf(fp, "%s = %d\n", prt_Record_Number, rec_num);
    fprintf(fp, "%s = %s\n", prt_Object_Type, prt_DIR);
    fprintf(fp, "%s = %s\n", prt_Name, list_ptr->name);

       /* take out UUID for now */
    /* fprintf(fp, "%s = %s\n", prt_UUID, uuid_p); */

    if (uuid_p != NULL)
        rpc_string_free(&uuid_p, &status);
    print_acl(prt_Obj_Acl_Def_Cell_, prt_Obj_Acl_Entry, fp, 
	      (RSDB_DIR(&item)).acl);
    print_acl(prt_Init_Obj_Acl_Def_Cell_, prt_Init_Obj_Acl_Entry, fp, 
              (RSDB_DIR(&item)).def_obj_acl);
    print_acl(prt_Init_Cont_Acl_Def_Cell_, prt_Init_Cont_Acl_Entry, fp, 
              (RSDB_DIR(&item)).def_dir_acl);
    print_delim(fp);

}

/*  print_pgo prints entries for a single pgo to princ.prt file  */

PRIVATE void print_pgo
#ifndef __STDC__
    (domain, list_ptr, rec_num, fp)
    sec_rgy_domain_t domain;
    prt_list_item_t *list_ptr;
    int rec_num;
    FILE *fp;
#else
  (
    sec_rgy_domain_t domain,
    prt_list_item_t *list_ptr,
    int rec_num,
    FILE *fp
  )
#endif
{
    rsdb_named_item_t         	item;
    rsdb_credentials_t 	       	creds;
    sec_rgy_name_t 	      	fullname;
    unsigned_char_p_t 	       	uuid_p;
    error_status_t 	       	status;
    db_handle 		       	db_name;
    rsdb_pvt_id_t               attr_list_id;
    rsdb_max_attr_list_t        attr_list;
    rsdb_util_rgy_obj_info_t    rgy_obj_info;
    rs_attr_cursor_t            cursor;
    uuid_t                      attr_uuid;
    unsigned32                  attr_apace_avail;
    unsigned32                  num_attr_returned;
    unsigned32                  num_attr_left;
    rsdb_attr_sch_list_t        *attr_sch_list = NULL;
    sec_attr_t                  attr_val;
    unsigned_char_t 		*uuid_string;
    int                         i, j;
    rsdb_attr_schema_entry_t    *sch_entry_p;

    if (domain == sec_rgy_domain_person)
        db_name = rsdb_person_db;
    else if (domain == sec_rgy_domain_group)
        db_name = rsdb_group_db;
    else
        db_name = rsdb_org_db;

    rsdb_util_get_item_by_id(domain, list_ptr->internal_id, &item);
    rsdb_get_credentials_by_unix_id(db_name, (RSDB_PGO(&item)).unix_id, &creds);
    uuid_to_string(&(creds.uid), &uuid_p, &status);
    fprintf(fp, "%s = %d\n", prt_Record_Number, rec_num);
    if (domain == sec_rgy_domain_person)
        fprintf(fp, "%s = %s\n", prt_Object_Type, prt_PRINC);
    else if (domain == sec_rgy_domain_group)
        fprintf(fp, "%s = %s\n", prt_Object_Type, prt_GROUP);
    else
        fprintf(fp, "%s = %s\n", prt_Object_Type, prt_ORG);

    fprintf(fp, "%s = %s/%s\n", prt_Name, root_name_keys[domain].name,
	    list_ptr->name);
    fprintf(fp, "%s = %s\n", prt_UUID, uuid_p);
    if (uuid_p != NULL)
        rpc_string_free(&uuid_p, &status);
    fprintf(fp, "%s = %d\n", prt_Unix_ID, (RSDB_PGO(&item)).unix_id);

         /* flags */
    if (IS_SET((RSDB_PGO(&item)).flags, sec_rgy_pgo_is_an_alias))
        fprintf(fp, "%s = %s\n", prt_Is_Alias_Flag, prt_true);
    else
        fprintf(fp, "%s = %s\n", prt_Is_Alias_Flag, prt_false);
    if (IS_SET((RSDB_PGO(&item)).flags, sec_rgy_pgo_is_required))
        fprintf(fp, "%s = %s\n", prt_Is_Required_Flag, prt_true);
    else
        fprintf(fp, "%s = %s\n", prt_Is_Required_Flag, prt_false);

    if (domain == sec_rgy_domain_group) {
        if (IS_SET((RSDB_PGO(&item)).flags, sec_rgy_pgo_projlist_ok))
            fprintf(fp, "%s = %s\n", prt_Projlist_Ok_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Projlist_Ok_Flag, prt_false);
    }

    if (domain == sec_rgy_domain_person) {
         /* quota */
        if ((RSDB_PGO(&item)).quota == sec_rgy_quota_unlimited)
            fprintf(fp, "%s = %s\n", prt_Quota, prt_unlimited);
        else
            fprintf(fp, "%s = %d\n", prt_Quota, (RSDB_PGO(&item)).quota);
    }

    if ( sec_svg_dce103 != TRUE ) {
	/* if for backward compatibility with dce1.0.3, don't do this part */
    	fprintf(fp, "%s = %d\n", prt_Num_Attr_List_Entries, (RSDB_PGO(&item)).num_attrs);

    	if ( (RSDB_PGO(&item)).num_attrs > 0 ) {
	    attr_list_id = (RSDB_PGO(&item)).attr_list_id;
	    while (attr_list_id != END_OF_LIST) {
		rsdb_attr_list_get_by_id(attr_list_id, &attr_list);

		/* for each attr type associated with this pgo ... */
		for ( i=0; i<attr_list.total_count; i++) {
		    if(!attr_list.attr[i].inst_id.deleted) {
			rsdb_attr_sch_get_by_id(attr_list.attr[i].type_id, 
						&sch_entry_p, &status);
			if ( BAD_STATUS(&status) ) {
			    error_st(status, "Cannot retrieve attr schema via id");
			    return;
			} 
			rsdb_attr_get_by_id_and_export(attr_list.attr[i].inst_id.id,
						       malloc, &attr_val, &status);
			uuid_to_string(&(sch_entry_p->attr_id), &uuid_string, &status);
			if ( BAD_STATUS(&status) ) {
			    error_st(status, "Cannot convert uuid to string");
			}
			fprintf(fp, "%s = %s\n", prt_Attr_UUID, uuid_string);
			free(uuid_string);
			print_attr_value(fp, &attr_val);
			sec_attr_util_inst_free_ptrs(&attr_val);
			free(sch_entry_p);
		    }
		}
		attr_list_id = attr_list.next;
    	    }
	}
    }

    strncpy((char *)fullname, (char *)(RSDB_PGO(&item)).fullname, 
            (RSDB_PGO(&item)).fullname_len);
    fullname[(RSDB_PGO(&item)).fullname_len] = '\0';
    fprintf(fp, "%s = %s\n", prt_Fullname, fullname);

    print_members(fp, domain, domain, (RSDB_PGO(&item)).membership, (RSDB_PGO(&item)).num_members);
   
    if ( (RSDB_PGO(&item)).foreign_membership != END_OF_LIST ) {
	print_members(fp, sec_rgy_domain_foreign_person, domain,
		(RSDB_PGO(&item)).foreign_membership, 
		(RSDB_PGO(&item)).num_foreign_members);
    }

    if (domain == sec_rgy_domain_person)
        print_acct(fp, (RSDB_PGO(&item)).info);

    print_acl(prt_Obj_Acl_Def_Cell_, prt_Obj_Acl_Entry, 
	      fp, creds.acl_id);

    if (domain == sec_rgy_domain_org)
        print_passwd_policy(fp, list_ptr->name);

    print_delim(fp);

}

PUBLIC void print_pgos
#ifndef __STDC__
    (fp, domain, status)
    FILE *fp;
    sec_rgy_domain_t domain;
    error_status_t *status;
#else
  (
    FILE *fp,
    sec_rgy_domain_t domain,
    error_status_t *status
  )
#endif
{
    int i;
    sec_rgy_cursor_t cursor;
    rsdb_named_item_t item;
    prt_list_item_t *dir_list_ptr;
    prt_list_item_t *pgo_list_ptr;
    prt_list_item_t *old_ptr;
    prt_list_item_t *new_ptr;
    rsdb_name_key_t name_key;
    long            name_key_len = sizeof(name_key);
    boolean32       do_top_level=true;

    CLEAR_STATUS(status);

    i = 0;
    cursor.valid = false;

    /* initialize lists */
    dir_list_ptr = NEW(prt_list_item_t);
    init_node(dir_list_ptr);
    pgo_list_ptr = NEW(prt_list_item_t);
    init_node(pgo_list_ptr);

    /*  get all pgos  */
    while (*status != sec_rgy_no_more_entries) {
         /*  malloc next pgo node into new_ptr */
         new_ptr = NEW(prt_list_item_t);
	 if ( do_top_level == true ) {
	 /* get top-level domain item */
	     rsdb_util_get_key_of_id(domain, START_OF_LIST,
				     (Pointer)&name_key, &name_key_len);
	     if ( rsdb_util_get_item(domain, (Pointer)&name_key,
				     name_key_len, &item)) {
		 new_ptr->internal_id = item.body.threads.id;
		 strcpy((char *)new_ptr->name, (char *)name_key.name);
		 new_ptr->name_len = name_key_len;
	     }
	     else
	         SET_STATUS(status, sec_rgy_bad_domain);
	     do_top_level = false;
	 }
	 else {
	     rsdb_util_get_next_by_id(domain, &cursor,
				      &item, &new_ptr->internal_id, status);
	     if (GOOD_STATUS(status)) {
	         /* get name and put into list entry */
	         rsdb_util_path_construct(domain, new_ptr->internal_id,  
			       new_ptr->name, &new_ptr->name_len, status);
	     }
	 }
	 if (BAD_STATUS(status)) {
	 /* return error message */
	     free(new_ptr);
	     break;
	 }
	 else {
	     if ( sort != TRUE ) { /* print out now */
		 i++;
	         if ( RSDB_IS_DIR(&item) ) 
         	     print_dir(domain, new_ptr, i, fp);
	         if ( RSDB_IS_PGO(&item) ) {
		     if ( RSDB_IS_DIR(&item) && ( sec_svg_dce103 == TRUE ) )
		     /* 
	              * Backward conversion to DCE 1.0.3, for aggregate pgodir
		      * type, print DIR and put PGO in an information file
                      * so user may choose if desire.
                      */
         	         print_pgo(domain, new_ptr, i, fp_dce103);
		     else
         	         print_pgo(domain, new_ptr, i, fp);
		 }
             	 free(new_ptr);
	     }
	     else {
	         if ( RSDB_IS_DIR(&item) )
	             insert_node(dir_list_ptr, new_ptr);
	         if ( RSDB_IS_PGO(&item) )
		     if ( RSDB_IS_DIR(&item) && ( sec_svg_dce103 == TRUE ) )
         	         print_pgo(domain, new_ptr, i, fp_dce103);
		     else
	                 insert_node(pgo_list_ptr, new_ptr);
	     }
	 }
    }

    if ( *status == sec_rgy_no_more_entries )
        CLEAR_STATUS(status);
 
    if ( sort == TRUE ) {
    /* print each dir record */
        i = 0;
        new_ptr = dir_list_ptr->ptr;
        while (new_ptr != NULL) {
             i++;
	     old_ptr = new_ptr;
             print_dir(domain, new_ptr, i, fp);
             new_ptr = new_ptr->ptr;
	     if ( old_ptr != NULL )
	         free(old_ptr);
        }

        /* print each pgo record */
        new_ptr = pgo_list_ptr->ptr;
        while (new_ptr != NULL) {
             i++;
	     old_ptr = new_ptr;
             print_pgo(domain, new_ptr, i, fp);
             new_ptr = new_ptr->ptr;
	     if ( old_ptr != NULL )
	         free(old_ptr);
	}
    }

    free(pgo_list_ptr);
    free(dir_list_ptr);
}

PUBLIC void print_pgo_domain
#ifndef __STDC__
    (domain, status)
    sec_rgy_domain_t domain;
    error_status_t *status;
#else
  (
    sec_rgy_domain_t domain,
    error_status_t *status
  )
#endif
{
    FILE *fopen(), *fp;
    sec_rgy_pname_t prt_file;

    strcpy((char *) prt_file, (char *) prt_path);
    if (domain == sec_rgy_domain_person)
        strcat((char *) prt_file, PRT_FILE_PRINC);
    else if (domain == sec_rgy_domain_group)
        strcat((char *) prt_file, PRT_FILE_GROUP);
    else
        strcat((char *) prt_file, PRT_FILE_ORG);

    /* open .prt file */
    fp = fopen((char *) prt_file, "w");
    if (fp) {
        print_pgos(fp, domain, status);
        fclose(fp);
        chmod((char *)prt_file, FILE_CREATION_PROTECTION);
    }
    else
        *status = sec_rgy_dir_not_found;
  
}


PRIVATE void print_binding_protect_level
  (
    FILE                             *fp,
    unsigned32                       protect_level
  )
{
    switch(protect_level) {
	case rpc_c_protect_level_default:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Protect_Level,
		prt_rpc_c_protect_level_default);
	    break;
	case rpc_c_protect_level_none:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Protect_Level,
		prt_rpc_c_protect_level_none);
	    break;
	case rpc_c_protect_level_connect:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Protect_Level,
		prt_rpc_c_protect_level_connect);
	    break;
	case rpc_c_protect_level_call:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Protect_Level,
		prt_rpc_c_protect_level_call);
	    break;
	case rpc_c_protect_level_pkt:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Protect_Level,
		prt_rpc_c_protect_level_pkt);
	    break;
	case rpc_c_protect_level_pkt_integ:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Protect_Level,
		prt_rpc_c_protect_level_pkt_integ);
	    break;
	case rpc_c_protect_level_pkt_privacy:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Protect_Level,
		prt_rpc_c_protect_level_pkt_privacy);
	    break;
	default:
            fprintf(stderr, "Invalid binding protect level - %d\n", protect_level);
    }
}


PRIVATE void print_binding_authn
  (
    FILE                             *fp,
    unsigned32                       authn
  )
{
    switch(authn) {
	case rpc_c_authn_none:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Authn,
		prt_rpc_c_authn_none);
	    break;
	case rpc_c_authn_dce_secret:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Authn,
		prt_rpc_c_authn_dce_secret);
	    break;
	case rpc_c_authn_dce_public:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Authn,
		prt_rpc_c_authn_dce_public);
	    break;
	case rpc_c_authn_default:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Authn,
		prt_rpc_c_authn_default);
	    break;
	default:
            fprintf(stderr, "Invalid binding authn - %d\n", authn);
    }
}


PRIVATE void print_binding_authz
  (
    FILE                             *fp,
    unsigned32                       authz
  )
{
    switch(authz) {
	case rpc_c_authz_none:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Authz,
		prt_rpc_c_authz_none);
	    break;
	case rpc_c_authz_name:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Authz,
		prt_rpc_c_authz_name);
	    break;
	case rpc_c_authz_dce:
            fprintf(fp, "%s = %s\n", prt_Attr_Auth_Authz,
		prt_rpc_c_authz_dce);
	    break;
	default:
            fprintf(stderr, "Invalid binding authz - %d\n", authz);
    }
}


PRIVATE void print_attr_schema_binding
  (
    FILE                             *fp,
    sec_attr_binding_t               *binding
  )
{
    int            i;
    switch (binding->bind_type) {
	case sec_attr_bind_type_string:
            fprintf(fp, "%s = %s\n", prt_Attr_Binding_String,
		binding->tagged_union.string_binding);
	    break;
	case sec_attr_bind_type_twrs:
    	    fprintf(fp, "%s = %d\n", prt_Num_Towers, binding->tagged_union.twr_set->count);
    	    for (i = 0; i < binding->tagged_union.twr_set->count; i++) {
        	print_tower(fp, binding->tagged_union.twr_set->towers[i]->tower_octet_string,
                    binding->tagged_union.twr_set->towers[i]->tower_length);
	    }
	    break;
	case sec_attr_bind_type_svrname:
	    fprintf(fp, "%s = %d\n", prt_Attr_Binding_Svr_Name_Syntax,
		binding->tagged_union.svrname->name_syntax);
            fprintf(fp, "%s = %s\n", prt_Attr_Binding_Svr_Name,
		binding->tagged_union.svrname->name);
	    break;
	default:
            fprintf(stderr, "Invalid binding type\n");
    }
}


PRIVATE void print_attr_binding
  (
    FILE                             *fp,
    sec_attr_bind_info_t             *binding_p
  )
{
    int i;

    /* print trig_auth_info_type */
    switch (binding_p->auth_info.info_type) {
	case sec_attr_bind_auth_none:
            fprintf(fp, "%s = %s\n", prt_Attr_Bind_Auth_Type, 
		prt_bind_auth_none);
	    break;
	case sec_attr_bind_auth_dce:
            fprintf(fp, "%s = %s\n", prt_Attr_Bind_Auth_Type, 
		prt_bind_auth_dce);
	    /* print trig_svr_princ_name */
    	    fprintf(fp, "%s = %s\n", prt_Attr_Svr_Princ_Name,
			binding_p->auth_info.tagged_union.dce_info.svr_princ_name);
    
    	    /* print trig_protect_level */
    	    print_binding_protect_level(fp, 
		binding_p->auth_info.tagged_union.dce_info.protect_level);
    	    /* print trig_authn_svc */
    	    print_binding_authn(fp, 
		binding_p->auth_info.tagged_union.dce_info.authn_svc);
    	    /* print trig_authz_svc */
            print_binding_authz(fp, 
		binding_p->auth_info.tagged_union.dce_info.authz_svc);
	    break;
	default:
            fprintf(stderr, "Invalid binding auth type\n");
    }

    fprintf(fp, "%s = %d\n", prt_Attr_Num_Binding, 
		binding_p->num_bindings);
    /* print trig binding */
    for ( i = 0; i < binding_p->num_bindings; i++ ) {
	print_attr_schema_binding(fp, &binding_p->bindings[i]);
    }

}


PRIVATE void print_attr_schema_trig_types
  (
    FILE                         *fp,
    sec_attr_trig_type_flags_t   trig_type
  )
{
    switch (trig_type) {
	case sec_attr_trig_type_none:
            fprintf(fp, "%s = %s\n", prt_Attr_Trig_Type, prt_none);
	    break;
	case sec_attr_trig_type_query:
            fprintf(fp, "%s = %s\n", prt_Attr_Trig_Type, prt_query);
	    break;
	case sec_attr_trig_type_update:
            fprintf(fp, "%s = %s\n", prt_Attr_Trig_Type, prt_update);
	    break;
	default:
            fprintf(stderr, "Invalid attr trig type\n");
    }
}


PRIVATE void print_attr_schema_intercell_act
  (
    FILE                         *fp,
    sec_attr_intercell_action_t  act
  )
{
    switch (act) {
	case sec_attr_intercell_act_accept:
            fprintf(fp, "%s = %s\n", prt_Attr_Intercell_Action, 
			prt_accept);
	    break;
	case sec_attr_intercell_act_reject:
            fprintf(fp, "%s = %s\n", prt_Attr_Intercell_Action, 
			prt_reject);
	    break;
	case sec_attr_intercell_act_evaluate:
            fprintf(fp, "%s = %s\n", prt_Attr_Intercell_Action, 
			prt_evaluate);
	    break;
	default:
            fprintf(stderr, "Invalid attr intercell action\n");
    }
}


PRIVATE void print_attr_schema_entry_flags
  (
    FILE                         *fp,
    sec_attr_sch_entry_flags_t   flags
  )
{
    if ( flags == sec_attr_sch_entry_none ) {
            fprintf(fp, "%s = %s\n", prt_Attr_Schema_Entry_Flag, 
		prt_attr_sch_entry_none);
	return;
    }
    if ( flags & sec_attr_sch_entry_unique )
            fprintf(fp, "%s = %s\n", prt_Attr_Schema_Entry_Flag, 
		prt_attr_sch_entry_unique);
    if ( flags & sec_attr_sch_entry_multi_inst )
            fprintf(fp, "%s = %s\n", prt_Attr_Schema_Entry_Flag, 
		prt_attr_sch_entry_multi_inst);
    if ( flags & sec_attr_sch_entry_reserved )
            fprintf(fp, "%s = %s\n", prt_Attr_Schema_Entry_Flag, 
		prt_attr_sch_entry_reserved);
    if ( flags & sec_attr_sch_entry_use_defaults )
            fprintf(fp, "%s = %s\n", prt_Attr_Schema_Entry_Flag, 
		prt_attr_sch_entry_use_defaults);
    
}


PRIVATE void print_attr_encoding
  (
    FILE                     *fp,
    sec_attr_encoding_t      attr_encoding
  )
{
    sec_rgy_name_t           attr_encoding_name;

    switch (attr_encoding) {
	case sec_attr_enc_any:
            fprintf(fp, "%s = %s\n", prt_Attr_Encoding, prt_attr_enc_any);
	    break;
        case sec_attr_enc_void:
            fprintf(fp, "%s = %s\n", prt_Attr_Encoding, prt_attr_enc_void);
	    break;
        case sec_attr_enc_integer:
            fprintf(fp, "%s = %s\n", prt_Attr_Encoding, prt_attr_enc_integer);
	    break;
        case sec_attr_enc_printstring:
            fprintf(fp, "%s = %s\n", prt_Attr_Encoding, prt_attr_enc_printstring);
	    break;
        case sec_attr_enc_printstring_array:
            fprintf(fp, "%s = %s\n", prt_Attr_Encoding, prt_attr_enc_printstring_array);
	    break;
        case sec_attr_enc_bytes:
            fprintf(fp, "%s = %s\n", prt_Attr_Encoding, prt_attr_enc_bytes);
	    break;
        case sec_attr_enc_confidential_bytes:
            fprintf(fp, "%s = %s\n", prt_Attr_Encoding, prt_attr_enc_confidential_bytes);
	    break;
        case sec_attr_enc_i18n_data:
            fprintf(fp, "%s = %s\n", prt_Attr_Encoding, prt_attr_enc_i18n_data);
	    break;
        case sec_attr_enc_uuid:
            fprintf(fp, "%s = %s\n", prt_Attr_Encoding, prt_attr_enc_uuid);
	    break;
        case sec_attr_enc_attr_set:
            fprintf(fp, "%s = %s\n", prt_Attr_Encoding, prt_attr_enc_attr_set);
	    break;
        case sec_attr_enc_binding:
            fprintf(fp, "%s = %s\n", prt_Attr_Encoding, prt_attr_enc_binding);
	    break;
        case sec_attr_enc_trig_binding:
            fprintf(fp, "%s = %s\n", prt_Attr_Encoding, prt_attr_enc_trig_binding);
	    break;
	default:
	    fprintf(stderr, "Invalid attr schema encoding type\n");

    }

}


PRIVATE unsigned32 print_attr_schema_entry
  (
    FILE                         *fp,
    char                         *schema_name,
    sec_attr_schema_entry_t      *sch_entry_p,
    error_status_t *status
  )
{
    static    unsigned32  rec_num=0;
    sec_attr_acl_mgr_info_t   attr_acl_mgr_info;
    unsigned_char_p_t     uuid_p;

    fprintf(fp, "%s = %d\n", prt_Record_Number, ++rec_num);
    fprintf(fp, "%s = %s\n", prt_Name, sch_entry_p->attr_name);

    uuid_to_string(&sch_entry_p->attr_id, &uuid_p, status);
    fprintf(fp, "%s = %s\n", prt_UUID, uuid_p);
    if (uuid_p != NULL)
        rpc_string_free(&uuid_p, status);

    print_attr_encoding(fp, sch_entry_p->attr_encoding);

    if ( sch_entry_p->acl_mgr_set != NULL ) {
    	print_attr_schema_aclmgr(fp, schema_name, sch_entry_p->acl_mgr_set);
    }

    print_attr_schema_entry_flags(fp, sch_entry_p->schema_entry_flags);
    print_attr_schema_intercell_act(fp, sch_entry_p->intercell_action);

    print_attr_schema_trig_types(fp, sch_entry_p->trig_types);
    if ( sch_entry_p->trig_binding != NULL ) {
    	print_attr_binding(fp, sch_entry_p->trig_binding);
    }

    /* print scope and comment */
    if ( sch_entry_p->scope == NULL )
	fprintf(fp, "%s =\n", prt_Attr_Schema_Scope);
    else
	fprintf(fp, "%s = %s\n", prt_Attr_Schema_Scope, sch_entry_p->scope);

    if ( sch_entry_p->comment == NULL )
        fprintf(fp, "%s =\n", prt_Attr_Schema_Comment); 
    else
        fprintf(fp, "%s = %s\n", prt_Attr_Schema_Comment, sch_entry_p->comment);
    
    print_delim(fp);
    return rec_num;
}


PUBLIC void print_attr_schema
  (
    error_status_t *status
  )
{
    sec_attr_schema_entry_t     sch_entry;
    unsigned32                  num_entries=0;
    rs_attr_cursor_t            cursor;
    unsigned32			rec_num;
    char                        *schema_name=SCHEMA_NAME;
    FILE *fopen(), *fp;
    sec_rgy_pname_t prt_file;

    if ( sec_svg_dce103 == TRUE ) {
    /* don't do attribute stuff for DCE 1.0.3 */
	return;
    }

    strcpy((char *) prt_file, (char *) prt_path);
    strcat((char *) prt_file, PRT_FILE_ATTR_SCHEMA);

    /* open .prt file */
    fp = fopen((char *) prt_file, "w");
    if (!fp) {
        *status = sec_rgy_dir_not_found;
	return;
    }
  
    CLEAR_STATUS(status);

    rsdb_attr_sch_cursor_init(schema_name, &num_entries, &cursor, status);
    if ( BAD_STATUS(status))
	return;

    do {
	rsdb_attr_sch_get_next(schema_name, NULL, &cursor, &sch_entry, 
		&num_entries, status);
	if ( GOOD_STATUS(status) )
	    rec_num = print_attr_schema_entry(fp, schema_name, &sch_entry, status);
	else {
	    break;
        }
	sec_attr_util_sch_ent_free_ptrs(&sch_entry);
    } while ( *status != sec_attr_no_more_entries );

    fprintf(fp, "%s = %d\n", prt_Record_Number, ++rec_num);
    fprintf(fp, "%s = %s\n", prt_Xattrschema_Name, SEC_ATTR_SCHEMA_OBJ_NAME);
    print_acl(prt_Obj_Acl_Def_Cell_, prt_Obj_Acl_Entry, fp, 
                   rsdb_get_attr_schema_acl_id());
    print_delim(fp);

    fclose(fp);
    chmod((char *)prt_file, FILE_CREATION_PROTECTION);
    if STATUS_EQUAL(status, sec_attr_no_more_entries)
    	CLEAR_STATUS(status);
    
}


PUBLIC void print_mkey
#ifndef __STDC__
    (status)
    error_status_t *status;
#else
  (
    error_status_t *status
  )
#endif
{
    FILE *fopen(), *fp;
    sec_rgy_pname_t prt_file;
    rsdb_auth_key_t mkey;

    strcpy((char *) prt_file, (char *) prt_base_path);
    strcat((char *) prt_file, PRT_FILE_MKEY);

    /* open mkey.prt file */
    fp = fopen((char *) prt_file, "w");
  
    if (fp) {
        rsdb_mkey_read(".mkey", &mkey, status);
        if (GOOD_STATUS(status)) {
            fprintf(fp, "%s = %d\n", prt_Master_Key_Version, 
                    mkey.key_version);
            if (mkey.keytype == KEYTYPE_DES)
                fprintf(fp, "%s = %s\n", prt_Master_Key_Keytype, prt_des);
            fprintf(fp, "%s = %d\n", prt_Master_Key_Length, mkey.length);
            print_key(fp, prt_Master_Key, (char *)mkey.contents, mkey.length);
        }
        fclose(fp);
        chmod((char *)prt_file, FILE_CREATION_PROTECTION);
    }
    else
        *status = sec_rgy_dir_not_found;

}

PUBLIC void print_policies
#ifndef __STDC__
    (status)
    error_status_t *status;
#else
  (
    error_status_t *status
  )
#endif
{
    FILE *fopen(), *fp;
    sec_rgy_pname_t prt_file;
    char time_string[TIME_STRING_LEN];
    char lifetime_string[LIFETIME_STRING_LEN];
    sec_rgy_name_t name;
    unsigned_char_p_t uuid_p;
    sec_rgy_login_name_t login_name;
    sec_rgy_plcy_auth_t auth_policy;
    sec_timeval_sec_t princ_dtm, group_dtm, org_dtm;
    rsdb_encrypt_key_t mkey;
    long file_version;
    signed32     num_attrs;

    strcpy((char *) prt_file, (char *) prt_path);
    strcat((char *) prt_file, PRT_FILE_POLICY);

    /* open policy.prt file */
    fp = fopen((char *) prt_file, "w");
  
    if (fp) {
        rsdb_get_rgy_info_file_version(&file_version);
	if ( sec_svg_dce103 == TRUE )
	    /* for backward conversion, hard-coded the policy file version */
            fprintf(fp, "%s = %d\n", prt_Rgy_Policy_File_Version, DCE103_RGY_FILE_VERSION);
	else
            fprintf(fp, "%s = %d\n", prt_Rgy_Policy_File_Version, file_version);
        fprintf(fp, "%s = %d\n", prt_Prop_Read_Version, properties.read_version);
        fprintf(fp, "%s = %d\n", prt_Prop_Write_Version, properties.write_version);
        period_to_dt_string(properties.minimum_ticket_lifetime, 
                  (char *) lifetime_string);
        fprintf(fp, "%s = %s\n", prt_Min_Certificate_Lifetime, lifetime_string);
        period_to_dt_string(properties.default_certificate_lifetime, 
                  (char *) lifetime_string);
        fprintf(fp, "%s = %s\n", prt_Default_Certificate_Lifetime, lifetime_string);
        fprintf(fp, "%s = %d\n", prt_Low_Unix_ID_Principal, 
                properties.low_unix_id_person);
        fprintf(fp, "%s = %d\n", prt_Low_Unix_ID_Group, properties.low_unix_id_group);
        fprintf(fp, "%s = %d\n", prt_Low_Unix_ID_Org, properties.low_unix_id_org);
        fprintf(fp, "%s = %d\n", prt_Max_Unix_ID, properties.max_unix_id);
        if (IS_SET(properties.flags, sec_rgy_prop_readonly))
            fprintf(fp, "%s = %s\n", prt_Rgy_Readonly_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Rgy_Readonly_Flag, prt_false);
        if (IS_SET(properties.flags, sec_rgy_prop_auth_cert_unbound))
            fprintf(fp, "%s = %s\n", prt_Auth_Certificate_Unbound_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Auth_Certificate_Unbound_Flag, prt_false);
        if (IS_SET(properties.flags, sec_rgy_prop_shadow_passwd))
            fprintf(fp, "%s = %s\n", prt_Shadow_Passwd_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Shadow_Passwd_Flag, prt_false);
        if (IS_SET(properties.flags, sec_rgy_prop_embedded_unix_id))
            fprintf(fp, "%s = %s\n", prt_Embedded_Unix_ID_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Embedded_Unix_ID_Flag, prt_false);
        fprintf(fp, "%s = %s\n", prt_Realm_Name, properties.realm);
        uuid_to_string(&(properties.realm_uuid), &uuid_p, status);
        fprintf(fp, "%s = %s\n", prt_Realm_UUID, uuid_p);
        if (uuid_p != NULL)
            rpc_string_free(&uuid_p, status);
        fprintf(fp, "%s = %d\n", prt_Unauthenticated_Quota, 
                properties.unauthenticated_quota);
        sprintf((char *)name, "");
        print_passwd_policy(fp, name);    /* prints default policy */
    
        login_name.pname[0] = '\0';   
        login_name.gname[0] = '\0';   
        login_name.oname[0] = '\0';   
             /* get registry auth policy */
        rsdb_auth_policy_get_info(&login_name, &auth_policy, status);
        period_to_dt_string(auth_policy.max_ticket_lifetime, 
                  (char *) lifetime_string);
        fprintf(fp, "%s = %s\n", prt_Max_Certificate_Lifetime, lifetime_string);
        period_to_dt_string(auth_policy.max_renewable_lifetime, 
                  (char *) lifetime_string);
        fprintf(fp, "%s = %s\n", prt_Max_Renewable_Lifetime, lifetime_string);
    
        rsdb_get_cache_state(&princ_dtm, &group_dtm, &org_dtm);
        sec_to_dt_string(princ_dtm, (char *)time_string);
        fprintf(fp, "%s = %s\n", prt_Princ_Cache_State, time_string);
        sec_to_dt_string(group_dtm, (char *)time_string);
        fprintf(fp, "%s = %s\n", prt_Group_Cache_State, time_string);
        sec_to_dt_string(org_dtm, (char *)time_string);
        fprintf(fp, "%s = %s\n", prt_Org_Cache_State, time_string);

	/* if printing for backward-converting to DCE 1.0.3, we don't
	 * want to print these entries.  If this is for DCE1.1 (or post-),
	 * since it is just informational, we print them in comments 
	 * ( line started with # )
	 */
	if ( sec_svg_dce103 == FALSE ) {
	    /* for policy_num_attrs, replist_num_attrs and attr_schema_num_attrs*/
	    rsdb_policy_get_num_attrs(RSDB_POLICY_OBJ_ID, &num_attrs);        
            fprintf(fp, "%s = %d\n", prt_Policy_Num_Attrs, num_attrs);
	    rsdb_policy_get_num_attrs(RSDB_REPLIST_OBJ_ID, &num_attrs);        
            fprintf(fp, "%s = %d\n", prt_Replist_Num_Attrs, num_attrs);
	    rsdb_policy_get_num_attrs(RSDB_ATTR_SCHEMA_OBJ_ID, &num_attrs);        
            fprintf(fp, "%s = %d\n", prt_Attr_Schema_Num_Attrs, num_attrs);
        }

        rsdb_get_my_server_name(name);
        fprintf(fp, "%s = %s\n", prt_My_Name, name);
    
        rsdb_get_enc_master_key(&mkey, status);
        fprintf(fp, "%s = %d\n", prt_Master_Key_Version, mkey.mkey_version);
        if (mkey.keytype == KEYTYPE_DES)
            fprintf(fp, "%s = %s\n", prt_Master_Key_Keytype, prt_des);
        fprintf(fp, "%s = %d\n", prt_Master_Key_Length, mkey.length);
    
        print_key(fp, prt_Master_Key, (char *)mkey.contents, mkey.length);
    
        print_acl(prt_Obj_Acl_Def_Cell_, prt_Obj_Acl_Entry, fp, 
                  rsdb_get_rgy_acl_id());
        fclose(fp);
        chmod((char *)prt_file, FILE_CREATION_PROTECTION);
    }
    else
        *status = sec_rgy_dir_not_found;

}

PUBLIC void print_rgy_state
#ifndef __STDC__
    (status)
    error_status_t *status;
#else
  (
    error_status_t *status
  )
#endif
{
    FILE *fopen(), *fp;
    sec_rgy_pname_t prt_file;
    boolean32 master_known;
    uuid_t master_id;
    rs_update_seqno_t master_seqno;
    unsigned_char_p_t uuid_p;
    unsigned32 file_version;
    unsigned32 global_sw_rev=0;;

    strcpy((char *) prt_file, (char *) prt_path);
    strcat((char *) prt_file, PRT_FILE_RGY_STATE);

    /* open rgy_state.prt file */
    fp = fopen((char *) prt_file, "w");
  
    if (fp) {
        rs_master_info_get(&master_known, &master_id, &master_seqno,
                           status);
        rs_master_info_get_file_version(&file_version, status);
	if ( sec_svg_dce103 == TRUE ) 
	/* for backward conversion, hard-coded the state file version */
            fprintf(fp, "%s = %d\n", prt_Rgy_State_File_Version, DCE103_RS_C_STATE_FILE_VERSION);
	else
            fprintf(fp, "%s = %d\n", prt_Rgy_State_File_Version, rs_state.version);
        if (rs_state.rep_state == rs_c_state_unknown_to_master)
            fprintf(fp, "%s = %s\n", prt_Replica_State, prt_unknown_to_master);
        else if (rs_state.rep_state == rs_c_state_uninitialized)
            fprintf(fp, "%s = %s\n", prt_Replica_State, prt_uninitialized);
        else if (rs_state.rep_state == rs_c_state_initializing)
            fprintf(fp, "%s = %s\n", prt_Replica_State, prt_initializing);
        else if (rs_state.rep_state == rs_c_state_in_service)
            fprintf(fp, "%s = %s\n", prt_Replica_State, prt_in_service);
        else if (rs_state.rep_state == rs_c_state_renaming) {
            fprintf(fp, "%s = %s\n", prt_Replica_State, prt_renaming);
               /* need to add next_rep_state value */
            if (rs_state.next_rep_state == rs_c_state_uninitialized)
                fprintf(fp, "%s = %s\n", prt_Replica_Next_State, 
                        prt_uninitialized);
            else if (rs_state.next_rep_state == rs_c_state_in_service)
                fprintf(fp, "%s = %s\n", prt_Replica_Next_State, 
                        prt_in_service);
        }
        else if (rs_state.rep_state == rs_c_state_copying_dbase)
            fprintf(fp, "%s = %s\n", prt_Replica_State, prt_copying_database);
        else if (rs_state.rep_state == rs_c_state_in_maintenance)
            fprintf(fp, "%s = %s\n", prt_Replica_State, prt_in_maintenance);
        else if (rs_state.rep_state == rs_c_state_mkey_changing)
            fprintf(fp, "%s = %s\n", prt_Replica_State, prt_master_key_changing);
        else if (rs_state.rep_state == rs_c_state_becoming_master)
            fprintf(fp, "%s = %s\n", prt_Replica_State, prt_becoming_master);
        else if (rs_state.rep_state == rs_c_state_closed)
            fprintf(fp, "%s = %s\n", prt_Replica_State, prt_closed);
        else if (rs_state.rep_state == rs_c_state_deleted)
            fprintf(fp, "%s = %s\n", prt_Replica_State, prt_deleted);

        uuid_to_string(&(rs_state.cell_sec_id), &uuid_p, status);
        fprintf(fp, "%s = %s\n", prt_Cell_UUID, uuid_p);
        if (uuid_p != NULL)
            rpc_string_free(&uuid_p, status);

        uuid_to_string(&(rs_state.rep_id), &uuid_p, status);
        fprintf(fp, "%s = %s\n", prt_Server_UUID, uuid_p);
        if (uuid_p != NULL)
            rpc_string_free(&uuid_p, status);

        uuid_to_string(&(rs_state.init_id), &uuid_p, status);
        fprintf(fp, "%s = %s\n", prt_Initialization_UUID, uuid_p);
        if (uuid_p != NULL)
            rpc_string_free(&uuid_p, status);

	if ( sec_svg_dce103 == TRUE ) {
	    /* 
	     * This field is taken out since dce1.1, printing only
	     * for backward-compatibility.
	     */
            fprintf(fp, "%s = %d\n", prt_Software_Version, 0);
	}

	if ( sec_svg_dce103 == TRUE ) {
            fprintf(fp, "%s = %d\n", prt_Master_File_Version, 
		rs_c_master_info_file_version_dce103);
	}
	else {
            fprintf(fp, "%s = %d\n", prt_Master_File_Version, file_version);
	}

        if (master_known)
            fprintf(fp, "%s = %s\n", prt_Master_Known_Flag, prt_true);
        else
            fprintf(fp, "%s = %s\n", prt_Master_Known_Flag, prt_false);

        uuid_to_string(&(master_id), &uuid_p, status);
        fprintf(fp, "%s = %s\n", prt_Master_UUID, uuid_p);
        if (uuid_p != NULL)
            rpc_string_free(&uuid_p, status);

        fprintf(fp, "%s = %d.%d\n", prt_Master_Seqno, master_seqno.high,
                master_seqno.low);

	if ( sec_svg_dce103 != TRUE ) {
	    rs_master_info_get_sw_rev(&global_sw_rev, status);
	    if ( BAD_STATUS(status) ) {
	    	return;
	    }
            fprintf(fp, "%s = %d\n", prt_Master_Global_Software_Rev, 
	   		global_sw_rev);
	}

        fclose(fp);
        chmod((char *)prt_file, FILE_CREATION_PROTECTION);
    }
    else
        *status = sec_rgy_dir_not_found;

}

PRIVATE char *get_prop_str
  (
    unsigned32 prop_type
  )
{
    if (prop_type == rs_c_replica_prop_init)
        return ((char *)prt_init);
    else if (prop_type == rs_c_replica_prop_initing)
        return ((char *)prt_initing);
    else if (prop_type == rs_c_replica_prop_update)
        return ((char *)prt_update);
    else if (prop_type == rs_c_replica_prop_delete)
        return ((char *)prt_delete);
    else if (prop_type == rs_c_replica_prop_unknown) 
	return ((char *)prt_unknown);
    else if (prop_type == rs_c_replica_prop_bad_sw_rev)
	return ((char *)prt_bad_sw_rev);
}

PRIVATE void print_replica_prop_types
  (
    FILE 	*fp,
    unsigned32  prop_type,
    unsigned32  prev_prop_type
  )
{
    if ( sec_svg_dce103 == TRUE ) { 
	/* 
	 * prev_prop_type is new to dce1.1, so pick up a right one
	 * for dce1.0.3
	 */
        if ( prop_type == rs_c_replica_prop_unknown ) {
            fprintf(fp, "%s = %s\n", prt_Propagation_Type, prt_init);
    	}
    	else if ( prop_type == rs_c_replica_prop_bad_sw_rev ) {
	    if (prev_prop_type == rs_c_replica_prop_unknown )
	    	fprintf(fp, "%s = %s\n", prt_Propagation_Type, prt_init);
	    else
	    	fprintf(fp, "%s = %s\n", prt_Propagation_Type, 
			get_prop_str(prev_prop_type));
	}
	else {
            fprintf(fp, "%s = %s\n", prt_Propagation_Type, get_prop_str(prop_type));
	}
    }
    else {
        fprintf(fp, "%s = %s\n", prt_Propagation_Type, get_prop_str(prop_type));
        fprintf(fp, "%s = %s\n", prt_Prev_Propagation_Type, 
			get_prop_str(prev_prop_type));
    }

}

PUBLIC void print_replica
#ifndef __STDC__
    (fp, rec_num, rep_mgt, status)
    FILE *fp;
    int rec_num;
    rs_replica_mgt_item_t *rep_mgt;
    error_status_t *status;
#else
  (
    FILE *fp,
    int rec_num,
    rs_replica_mgt_item_t *rep_mgt,
    error_status_t *status
  )
#endif
{
    unsigned_char_p_t uuid_p;
    int i;
    
    fprintf(fp, "%s = %d\n", prt_Record_Number, rec_num);

    uuid_to_string(&(rep_mgt->rep_id), &uuid_p, status);
    fprintf(fp, "%s = %s\n", prt_Replica_UUID, uuid_p);
    if (uuid_p != NULL)
        rpc_string_free(&uuid_p, status);

    fprintf(fp, "%s = %s\n", prt_Replica_Name, rep_mgt->rep_name);

    fprintf(fp, "%s = %d\n", prt_Num_Towers, rep_mgt->rep_twrs->num_towers);
    for (i = 0; i < rep_mgt->rep_twrs->num_towers; i++) {
        print_tower(fp, rep_mgt->rep_twrs->towers[i]->tower_octet_string,
                    rep_mgt->rep_twrs->towers[i]->tower_length);
    }

    print_replica_prop_types(fp, rep_mgt->prop_type, rep_mgt->prev_prop_type);

    uuid_to_string(&(rep_mgt->init_id), &uuid_p, status);
    fprintf(fp, "%s = %s\n", prt_Initialization_UUID, uuid_p);
    if (uuid_p != NULL)
        rpc_string_free(&uuid_p, status);

}

PUBLIC void print_replicas
#ifndef __STDC__
    (status)
    error_status_t *status;
#else
  (
    error_status_t *status
  )
#endif
{
    FILE *fopen(), *fp;
    sec_rgy_pname_t prt_file;
    uuid_t rep_cursor;
    unsigned32 buff_len;
    boolean32 last = FALSE;
    int i = 0;
    prt_rep_list_item_t *list_ptr;
    prt_rep_list_item_t *old_ptr;
    prt_rep_list_item_t *new_ptr;

    CLEAR_STATUS(status);

    /* initialize lists */
    list_ptr = NEW(prt_rep_list_item_t);
    list_ptr->rep_info = 
              (rs_replica_mgt_item_t *) malloc(sizeof(rs_replica_mgt_item_t));
    list_ptr->rep_info->rep_name = (unsigned char *) malloc(1);
    list_ptr->rep_info->rep_name[0] = '\0';
    list_ptr->ptr = NULL;

    strcpy((char *) prt_file, (char *) prt_path);
    strcat((char *) prt_file, PRT_FILE_REPLICAS);

    /* open replicas.prt file */
    fp = fopen((char *) prt_file, "w");
  
    uuid_create_nil(&rep_cursor, status);
    if (fp) {
        while (!last) {
            new_ptr = NEW(prt_rep_list_item_t);
            rsdb_replica_lookup(&rep_cursor, &new_ptr->rep_info, status);
            if (uuid_is_nil(&rep_cursor, status))
                last = TRUE;
            insert_rep_node(list_ptr, new_ptr);
        }
        i = 0;
        new_ptr = list_ptr->ptr;
        while (new_ptr != NULL) {
            i++;
	    old_ptr = new_ptr;
            print_replica(fp, i, new_ptr->rep_info, status);
            rsdb_replica_item_free(&new_ptr->rep_info, status);
            new_ptr = new_ptr->ptr;
	    if ( old_ptr != NULL )
	        free(old_ptr);
    	    print_delim(fp);
        }

	if ( sec_svg_dce103 != TRUE ) {
	    /* 
	     * bug fix, since we support back-compatibility, a DCE 1.0.3
	     * salvager won't be able to parse this.
	     */
            fprintf(fp, "%s = %d\n", prt_Record_Number, ++i);
            fprintf(fp, "%s = %s\n", prt_Replist_Name, SEC_REPLIST_OBJ_NAME);
            print_acl(prt_Obj_Acl_Def_Cell_, prt_Obj_Acl_Entry, fp, 
                  rsdb_get_replist_acl_id());
	}

	if ( list_ptr->rep_info->rep_name != NULL )
           free(list_ptr->rep_info->rep_name);
	if ( list_ptr->rep_info != NULL )
           free(list_ptr->rep_info);
	if ( list_ptr != NULL )
	   free(list_ptr);
        fclose(fp);
        chmod((char *)prt_file, FILE_CREATION_PROTECTION);
    }
    else
        *status = sec_rgy_dir_not_found;

}

