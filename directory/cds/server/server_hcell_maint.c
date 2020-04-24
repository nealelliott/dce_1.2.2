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
 * $Log: server_hcell_maint.c,v $
 * Revision 1.1.8.3  1996/02/18  19:36:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:14  marty]
 *
 * Revision 1.1.8.2  1995/12/08  15:32:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:46  root]
 * 
 * Revision 1.1.4.1  1994/10/17  19:18:31  zee
 * 	OT 12594: Remove cyclical name check for all add aliases except for AsChild.
 * 	[1994/10/17  18:52:14  zee]
 * 
 * Revision 1.1.2.7  1994/09/30  13:57:53  zee
 * 	OT 12345:
 * 	   - When adding a preferred alias, check that the name already exists
 * 	     as some alias.
 * 	   - When adding a child alias, ignore incoming alias and use the
 * 	     existing preferred alias.
 * 	   - Schedule skulk in remove_cell_alias.
 * 	[1994/09/29  23:28:11  zee]
 * 
 * Revision 1.1.2.6  1994/08/26  21:55:33  zee
 * 	Change minimum directory version needed from 3.1 to 4.0.
 * 	[1994/08/26  21:49:29  zee]
 * 
 * Revision 1.1.2.5  1994/08/03  19:04:34  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:27:50  mccann]
 * 
 * Revision 1.1.2.4  1994/06/09  18:43:46  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:20  devsrc]
 * 
 * Revision 1.1.2.3  1994/06/03  20:31:24  zee
 * 	     Bugfix for OT 10841: preferred alias got added as normal alias.
 * 	[1994/06/02  20:21:39  zee]
 * 
 * Revision 1.1.2.2  1994/05/12  21:12:26  peckham
 * 	     Eliminate use of dnstables.
 * 	[1994/05/12  19:18:38  peckham]
 * 
 * Revision 1.1.2.1  1994/05/06  16:04:26  zee
 * 	Initial version.
 * 	[1994/05/06  16:03:43  zee]
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
 * Module Description:
 *      Routine for adding/removing CDS hierarchical cells implementation.
 *
 */
#include <server.h>
#include <find.h>
#include <security.h>
#include <hcell.h>
#include <names.h>
#include <sets.h>
#include <uid.h>

/*
 * Local prototypes
 */
static dns_status_t 
check_child_name (FullName_t *);

static dns_status_t
hcell_create_child (
    db_stream_t *,
    user_descriptor_t *,
    FullName_t *);

static dns_status_t 
check_cyclical_name (
    FullName_t *,
    FullName_t *);

/*
 *++
 *   add_cell_alias
 *
 *  Description:
 *      Routine to modify the CDS_CellAliases attribute of the root directory.
 *      The rules are:
 *
 *          When adding an alias:
 *            A1. A new preferred alias supersedes a non-preferred alias
 *                of the same name.
 *            A2. A new preferred alias demotes the existing preferred
 *                alias to non-preferred.
 *            A3. A non-preferred alias CANNOT supersede a preferred
 *                alias of the same name (error).
 *          When bootstrapping (adding alias for the first time):
 *            A4. If the current cell name is not in the alias list, add
 *                it as the preferred alias if no other preferred alias
 *                is asserted, else add it as a non-preferred alias.
 *
 *      The algorithm to modify the CDS_CellAliases attribute is to
 *      remove the entire attribute, and then add each alias value (and
 *      any necessary modifications of preferred-ness).
 *
 *      Note: Adding an alias AsChild implies AsPreferred as well
 *            (e.g. it becomes the preferred alias).
 *
 *      LATE BREAKING NEWS: 9/29/94
 *            OT 12345: When adding AsPreferred, require the name
 *                      already be an existing alias.
 *                      Also, the behavior of AsChild has been modified.
 *                      Now, when adding AsChild, it will use the current
 *                      preferred name.  In order to make this change
 *                      in the fewest edits possible, when adding AsChild,
 *                      the incoming alias_p argument is now ignored, and
 *                      alias_p is set to use the existing preferred alias.
 *                      This should be cleaned up in the next release.
 *
 *  Formal Parameters:
 *      user_p         - ptr to user_descriptor  (IN)
 *      chid_p         - ptr to clearinghouse uuid  (IN)
 *      how            - how one wants to add this alias  (IN)
 *      alias_p        - cell alias to add/remove  (IN)
 *
 *  Returns:
 *      CDS_ALIASTOOLONG
 *      CDS_ALIASTOOSHORT
 *      CDS_LOWDIRVERSION
 *      CDS_ENTRYEXISTS
 *      CDS_ALIASCYCLE
 *      CDS_NOPREFERRED_EXISTS
 *
 *  Side Effects:
 *
 *--
 */
dns_status_t
add_cell_alias (
    user_descriptor_t *user_p,
    uuid_t            *chid_p,
    AddAliasAs_t       how,
    FullName_t        *alias_p)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_ObjUID(chidBuf)
    DEB_ASCII_BUF_FullName(aliasBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    byte_t  work_buffer[sizeof(byte_t) + sizeof(FullName_t)];
    byte_t *work_p = work_buffer;
    byte_t           linked;		 /* Used in clerk read attr calls */
    FullName_t       parent_name;	 /* Will hold parent name */
    file_info_t     *file_p;
    Update_t        *update_p;           /* Update work buffer */
    tlog_dsc_t       tlog;		 /* Transaction log update buf */
    Timestamp_u      ts;                 /* Current timestamp */
    FullName_t      *cur_cellname_p;	 /* Used in cyclical name testing */
    VersionNumber_t *dir_version_p;
    DBSet_t         *alias_set_p, *set_p;
    int              alias_set_length;
    DBSetMember_t   *bootstrap_p, *member_p;
    int              index, pos, alias_set_num, locstat, total_bufsize;
    int              num_names;		 /* number of simple names in alias */
    int              num_upptr_mem;	 /* number of parent replicas */
    Set_t           *par_rep_set_p;	 /* parent replica set ptr */
    int              alias_tlog_cnt = 0;
    int              upptr_tlog_cnt = 0;
    int              bufsize_alias = 0;
    int              bufsize_upptr = 0;
    int              childptr_created = BL_false;
    int              preferred_cell = BL_false;
    int              bootstrap_flag = 0;  /* 0 = don't bootstrap */
                                          /* 1 = bootstrap, not preferred */
                                          /* 2 = bootstrap preferred */
    db_stream_t     *stream_p = NULL;
    byte_t          *parent_replicas_p = NULL;/* Alloc replica set of parent */
    byte_t          *tlog_p = NULL;       /* Allocated tlog updates */
    dns_status_t   status = CDS_SUCCESS;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >add_cell_alias(%s, %s, %d, %s)\n",
	deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ObjUID(chidBuf,chid_p), how,
        deb_ascii_FullName(aliasBuf, alias_p)));

    status = db_open_stream(chid_p, &stream_p, OP_ModifyAttribute);
    if (status != CDS_SUCCESS) {
        goto exit_add_cell_alias;
    }
    file_p = stream_p->db_file_p;

    /* Check that this is the master replica of the root directory */
    if (!file_p->file_root_dir_p) {
        dce_svc_printf(CDS_S_SERVER_ALIASNOTROOT_MSG);
        status = CDS_NAMESERVERBUG;
        goto exit_add_cell_alias;
    }
    if (file_p->file_root_dir_p->dir_replica_type != RT_master) {
        dce_svc_printf(CDS_S_SERVER_ALIASNOTMASTER_MSG);
        status = CDS_NAMESERVERBUG;
        goto exit_add_cell_alias;
    }

    COPY_FullName(file_p->file_root_dir_p->dir_name, &stream_p->db_key);
    status = db_dir_read_for_update(stream_p);
    if (status != CDS_SUCCESS) {
        goto exit_add_cell_alias;
    }

    /*
     *  Security check (bypass if user_descriptor is null (used by diag))
     */
    if (user_p != NULL) {
	status = security_check(user_p, stream_p, Super_check, ET_directory,
				NULL);
	if (status != CDS_SUCCESS) {
	    goto exit_add_cell_alias;
	}
    }

    /*
     *  Check if we already have a cell aliases attribute
     */
    if (find_attribute(stream_p->db_dir.data_p,
                       dns_AttributeName(DNS_CELLALIASES),
                       &alias_set_p, &index) != CDS_SUCCESS) {
        alias_set_p = NULL;
        alias_set_length = 0;
        alias_set_num = 0;
    }
    else {
        alias_set_length = LEN_DBSet(alias_set_p);
        alias_set_num = NUM_DBSet(alias_set_p);
    }

    /*
     * OT 12345: If AsChild, ignore incoming alias_p and reset it to
     * use the current preferred alias.
     */
    if (how == AsChild) {
	DBSetMember_t *pref_member_p;
	int tmpstat;

	if (!alias_set_p) {
	    status = CDS_NOPREFERRED_EXISTS;
	    goto exit_add_cell_alias;
	}
	else {
	    tmpstat = get_preferred_member(alias_set_p, &pref_member_p);
	    if (tmpstat == CDS_SUCCESS) {
		unsigned char *tmp_p;

		tmp_p = pref_member_p->dm_data;
		tmp_p++;			/* Skip over preferred flag */
		alias_p = (FullName_t *) tmp_p;
	    }
	    else {
		/* No preferred member, so return error */
		status = CDS_NOPREFERRED_EXISTS;
		goto exit_add_cell_alias;
	    }
	}
    }

    /*
     *  First, check that the depth of the new alias (number of simple names)
     *  is:
     *      > 1 for normal alias (AsAlias)
     *      > 2 (e.g. /.../existingcell/newname) for AsChild
     */
    num_names = names_count(alias_p);
    if (how == AsChild) {
	if (num_names < 3) {
	    status = CDS_ALIASTOOSHORT;
	    goto exit_add_cell_alias;
	}
    }
    else {
	if (num_names < 2) {
	    status = CDS_ALIASTOOSHORT;
	    goto exit_add_cell_alias;
	}
    }
    
    /*
     *  Check the length of the alias (restriction that alias < 255 bytes,
     *  this so we don't have to change the advertisement protocol as
     *  the alias now becomes the namespace name (simple name) in the
     *  advertisement protocol.)
     */
    if (LEN_FullName(alias_p) > SIMPLENAMEMAX) {
	status = CDS_ALIASTOOLONG;
	goto exit_add_cell_alias;
    }

    /*
     *  Check that directory version >= 4.0
     */
    if (find_attribute(stream_p->db_dir.data_p,
		       dns_AttributeName(DNS_DIRECTORYVERSION),
		       &set_p, &index) != CDS_SUCCESS) {
	evl_AttributeMissing(stream_p->db_file_p->file_ch_p,
		       &stream_p->db_dir_hash_p->dir_name,
		       dns_AttributeName(DNS_DIRECTORYVERSION));
	status = CDS_MISSING_DIRECTORYVERSION;
	goto exit_add_cell_alias;
    }
    member_p = find_last_value(set_p);
    if (!member_p) {
	status = CDS_MISSING_DIRECTORYVERSION;
	goto exit_add_cell_alias;
    }
    dir_version_p = (VersionNumber_t *)member_p->dm_data;
    if (EXT8(dir_version_p->vn_major) < 4) {
	status = CDS_LOWDIRVERSION;
	goto exit_add_cell_alias;
    }

    /*
     * OT 12345 fix: Only allow AsPreferred if name already exists
     * in the alias set.
     */
    if (how == AsPreferred) {
	int test_status;

	test_status = cell_alias_test (file_p, alias_p);
	if (test_status != 0)
	    if (test_status != 1) {
		status = CDS_NOT_AN_ALIAS;
		goto exit_add_cell_alias;
	    }
    }

    if (how == AsChild || how == AsPreferred || how == AsInherit) {
	/*
	 *  If adding an alias AS CHILD, check that the name is not already
	 *  being used in the parent directory.  Also, set preferred flag if
	 *  appropriate.
	 */
	if (how == AsChild) {
	    status = check_child_name(alias_p);
	    if (status != CDS_UNKNOWNENTRY) {
		if (status == CDS_SUCCESS)  {
		    status = CDS_ENTRYEXISTS;
		}
		goto exit_add_cell_alias;
	    }
	    /*
	     *  Get the parent's CDS_REPLICAS for use later in updating the
	     *  current cell's parent cell pointer.
	     */
	    parent_replicas_p = dns_malloc(READ_BUF_SIZE);
	    if (!parent_replicas_p) {
		status = CDS_NONSRESOURCES;
		goto exit_add_cell_alias;
	    }
	    names_stripright(alias_p, &parent_name);
	    status = clerk_read_attribute_conf(&parent_name,
					       ET_directory, 
					       dns_AttributeName(DNS_REPLICAS),
					       parent_replicas_p,
					       NULL, &linked, cdsConfHi);
	    if (status != CDS_SUCCESS) {
		goto exit_add_cell_alias;
	    }
	}  /* end how == AsChild */

	preferred_cell = BL_true;
    }

    /*
     *  OT 12594: Check for cyclical names if AsChild
     */
    if (how == AsChild) {
	if (!alias_set_p) {
	    cur_cellname_p = (FullName_t *) file_p->file_root_dir_p->dir_name;
	}
	else {
	    DBSetMember_t *pref_member_p;
	    int tmpstat;

	    tmpstat = get_preferred_member(alias_set_p, &pref_member_p);
	    if (tmpstat == CDS_SUCCESS) {
		unsigned char *tmp_p;

		tmp_p = pref_member_p->dm_data;
		tmp_p++;		/* Skip over preferred flag */
		cur_cellname_p = (FullName_t *) tmp_p;
	    }
	    else if (tmpstat == 0) {
		cur_cellname_p = (FullName_t *) file_p->file_root_dir_p->dir_name;
	    }
	    else {
		goto exit_add_cell_alias;
	    }
	}
	status = check_cyclical_name(alias_p, cur_cellname_p); 
	if (status != CDS_SUCCESS) {
	    goto exit_add_cell_alias;
	}
    }  /* end if how == AsChild for cyclical name check */

    /*
     *  Bootstrap check - Rule A4., part 1 -
     *  Check if we need to add the cell name to the alias set.
     */
    if (!EQ_FullName(alias_p,
                     (FullName_t *) file_p->file_root_dir_p->dir_name)) {
        locstat = cell_alias_test(file_p,
                                  (FullName_t *) file_p->file_root_dir_p->dir_name);
        if ((locstat == -1) || (locstat == 2)) {  /* not in alias list */
            bootstrap_flag = 1;
            if (preferred_cell == BL_false) {
                if (alias_set_p == (DBSet_t *) NULL) {
                    bootstrap_flag = 2;
                }
                else {
                    locstat = get_preferred_member(alias_set_p, &bootstrap_p);
                    if (locstat == 0) {
                        bootstrap_flag = 2;
                    }
                }
            }
        }
        else if (locstat == 0) {        /* is alias, but not preferred */
            if (preferred_cell == BL_false) {
                locstat = get_preferred_member(alias_set_p, &bootstrap_p);
                if (locstat == 0) {
                    bootstrap_flag = 2;
                }
            }
        }
    }
    else {      /* Adding cell name as alias with no existing preferred alias,
                 * so make this the preferred alias.
                 */
	if (alias_set_p == (DBSet_t *) NULL) {
	    /*
	     * Adding the cell name as the first alias, so don't bootstrap,
	     * and make this the preferred alias.
	     */
	    bootstrap_flag = 0;
	    preferred_cell = BL_true;
	}
	else {
	    locstat = get_preferred_member(alias_set_p, &bootstrap_p);
	    if (locstat == 0) {
		preferred_cell = BL_true;
	    }
	}
    }

    /*
     *  Allocate a buffer large enough to hold all of the potential updates.
     *  For the cell aliases, this includes:
     *    - Fudge factor
     *    - The Update for the delete of the attribute
     *    - The possible update if the cell name is not in the alias set
     *    - Update overhead for each alias (which includes incoming)
     *    - Attribute Specifier for CDS_CellAliases for each alias
     *    - Value of alias for each alias
     */
    bufsize_alias = (50 + sizeof(Update_overhead) * (alias_set_num + 3)) +
              (sizeof(dns_Attribute(DNS_CELLALIASES)) * (alias_set_num + 2)) +
              alias_set_length + sizeof(byte_t) * 2 + LEN_FullName(alias_p) *2;
    if (how == AsChild) {
	/*
	 *  Need to calculate additional space for up pointers attribute update
	 */
	par_rep_set_p = (Set_t *)((AttributeContents_t *)parent_replicas_p)->ac_set;
	num_upptr_mem = NUM_Set((Set_t *)par_rep_set_p);
	bufsize_upptr = num_upptr_mem * sizeof(Update_t);
    }
    total_bufsize = bufsize_alias + bufsize_upptr;
    tlog_p = (byte_t *) dns_malloc(total_bufsize);
    if (!tlog_p) {
        status = CDS_NONSRESOURCES;
        goto exit_add_cell_alias;
    }

    /*
     *  Initialize transaction log descriptor
     */
    ZERO_bytes(&tlog, sizeof(tlog_dsc_t));
    tlog.buf_p = tlog_p;

    /* Initialize update_p to point to beginning of tlog buffer */
    update_p = (Update_t *) tlog_p;

    /* Build the Delete CDS_CellAliases attribute update */
    tlog.len += update_dns_build(update_p,
				 UD_absent,
				 NULL,
				 dns_Attribute(DNS_CELLALIASES),
				 BL_false,
				 0,
				 NULL);
    alias_tlog_cnt++;
    update_p = (Update_t *)SKIP_Update(update_p);

    /*
     *  Loop through each member of the current alias set.
     *  Note that we will always be adding the incoming alias (unless error),
     *  so if the incoming alias and a member have the same name, the
     *  incoming alias gets added and we don't add the mem_fname_p.
     */
    for (pos = 0; pos < alias_set_num; pos++) {
        DBSetMember_t *member_p;
        FullName_t    *mem_fname_p;     /* Ptr to current member fullname */
        unsigned char *mem_preferred_p; /* Is current member preferred?   */
        char           include_member, member_preferred;

        include_member = BL_false;
        member_preferred = BL_false;
        member_p = (DBSetMember_t *) INDEX_DBSet(alias_set_p, pos);
        if (!EXT8(member_p->dm_valid) ||
            !EXT8(member_p->dm_value_present)) continue;

        mem_preferred_p = member_p->dm_data;
        mem_fname_p = (FullName_t *) (mem_preferred_p + 1);

	if (EQ_FullName(alias_p, mem_fname_p)) {
	    if ((*mem_preferred_p == BL_true) &&
		(preferred_cell == BL_false)) {
		/* Rule A3. */
		status = CDS_PREFERRED_EXISTS;
		break;
	    }
	    else {
		/* Rule A1. */
		include_member = BL_false;
	    }
	}
	else {   /* not equal, so always include this member */
	    include_member = BL_true;
	    if (preferred_cell == BL_true) {
		/* Rule A2. */
		member_preferred = BL_false;
	    }
	    else {
		member_preferred = *mem_preferred_p;
	    }
	}

        /* Build update to include this member and set preferred flag */
        if (include_member) {
            ts_new((Timestamp_t *)ts);  /* Ignore errors, use null */
            work_p = work_buffer;
            PUT8(work_p, member_preferred);
            COPY_FullName(mem_fname_p, (FullName_t *)work_p);
            tlog.len += update_dns_build(update_p,
					 UD_present,
					 (Timestamp_t *)ts,
					 dns_Attribute(DNS_CELLALIASES),
					 BL_true,
					 LEN_FullName(mem_fname_p)+sizeof(byte_t),
					 work_buffer);
            alias_tlog_cnt++;
            update_p = (Update_t *)SKIP_Update(update_p);
        }
    } /* end for */

    if (status != CDS_SUCCESS) {
        goto exit_add_cell_alias;
    }

    /*
     *  If we get here okay, then we still need to add the update for the
     *  incoming alias.
     */

    ts_new((Timestamp_t *)ts);  /* Ignore errors, use null */
    work_p = work_buffer;
    PUT8(work_p, preferred_cell);
    COPY_FullName(alias_p, (FullName_t *)work_p);
    tlog.len += update_dns_build(update_p,
				 UD_present,
				 (Timestamp_t *)ts,
				 dns_Attribute(DNS_CELLALIASES),
				 BL_true,
				 LEN_FullName(alias_p)+sizeof(byte_t),
				 work_buffer);
    alias_tlog_cnt++;
    update_p = (Update_t *)SKIP_Update(update_p);

    /*
     *  Bootstrap check - Rule A4., part 2 -
     *  See if we need to add the cell name as an alias.
     */
    if (bootstrap_flag) {
        FullName_t *root_dir_p = (FullName_t *) file_p->file_root_dir_p->dir_name;

        ts_new((Timestamp_t *)ts);      /* Ignore errors, use null */
        work_p = work_buffer;
        if (bootstrap_flag == 2) {      /* Set preferred flag */
            PUT8(work_p, BL_true);
        }
        else {
            PUT8(work_p, BL_false);
        }
        COPY_FullName(root_dir_p, (FullName_t *)work_p);
        tlog.len += update_dns_build(update_p,
				     UD_present,
				     (Timestamp_t *)ts,
				     dns_Attribute(DNS_CELLALIASES),
				     BL_true,
				     LEN_FullName(root_dir_p)+sizeof(byte_t),
				     work_buffer);
        alias_tlog_cnt++;
	update_p = (Update_t *)SKIP_Update(update_p);
    }

    /* Check that we didn't overrun our allocated buffer */
    if (tlog.len > total_bufsize) {
        dce_svc_printf(CDS_S_SERVER_ALIASOVERFLOW_MSG);
        status = CDS_NAMESERVERBUG;
        goto exit_add_cell_alias;
    }

    if (how == AsChild) {
	/*
	 *  Create the child pointer in the parent directory
	 */
	status = hcell_create_child(stream_p, user_p, alias_p);
	if (status != CDS_SUCCESS) {
	    dce_svc_printf(CDS_S_SERVER_ALIASCHILD_MSG, (long)status);
	    goto exit_add_cell_alias;
	}
	childptr_created = BL_true;
    }

    /*
     *  If this cell is already a child cell, replace the old child pointer
     *  in the old parent directory with a softlink in its place.
     */
    if (how != AsInherit) {
	int            num_mem;
	DBSet_t       *parentcell_p;
	byte_t         exist_child_cell = BL_false;

	/*
	 *  Walk through PARENTCELLPOINTER and check for valid entries
	 *  to see if this cell is a valid child cell.
	 */
	parentcell_p = file_p->file_root_dir_p->dir_cellparent_set_p;
	if (parentcell_p) {
	    num_mem = NUM_DBSet(parentcell_p);
	    for (pos = 0; pos < num_mem; pos++) {
		DBSetMember_t *member_p;

		member_p = (DBSetMember_t *) INDEX_DBSet(parentcell_p, pos);
		if (EXT8(member_p->dm_valid) &&
		    EXT8(member_p->dm_value_present)) {
		    exist_child_cell = BL_true;
		}
	    }
	}

	if ((exist_child_cell == BL_true) && (alias_set_p != NULL)) {
	    DBSetMember_t *preferred_mem_p;
	    unsigned char *tmp_p;
	    FullName_t    *oldcellname_p;
	    Timestamp_t    slink_cts;

	    status = get_preferred_member(alias_set_p, &preferred_mem_p);
	    if (status != CDS_SUCCESS) {
		goto exit_add_cell_alias;
	    }
	    tmp_p = preferred_mem_p->dm_data;
	    oldcellname_p = (FullName_t *) (tmp_p + 1);
	    status = clerk_delete_child(oldcellname_p);
	    if (status != CDS_SUCCESS) {
		dce_svc_printf(CDS_S_SERVER_ALIASDELCHILD_MSG, (long)status);
	    }

	    /* create softlink in its place */
	    status = clerk_create_slink(oldcellname_p, alias_p, NULL, &slink_cts);
	    if (status != CDS_SUCCESS) {
		dce_svc_printf(CDS_S_SERVER_ALIASCREATESL_MSG, (long)status);
	    }
	}  /* end if (exist_child_cell == BL_true) */
    }  /* end if (how != AsInherit) */

    /*
     * COMMIT PHASE:
     *    - Update cell aliases
     *    - Update cell up pointers
     *    - Initiate skulk of root directory
     */
    /*
     *  Update the cell up pointers - only for AsChild
     */
    if (how == AsChild) {

	/* Build the Delete CDS_PARENTCELLPOINTERS attribute update */
	tlog.len += update_dns_build(update_p,
				     UD_absent,
				     NULL,
				     dns_Attribute(DNS_PARENTCELLPOINTERS),
				     BL_false,
				     0,
				     NULL);
	upptr_tlog_cnt++;
	update_p = (Update_t *)SKIP_Update(update_p);

	/*
	 *  Loop through the parent replica set and add the update
	 *  to the CDS_PARENTCELLPOINTERS of the current cell.
	 */
	for (pos = 0; pos < num_upptr_mem; pos++) {
	    MemberValue_t *member_p;
	    int            mem_length;

	    member_p = (MemberValue_t *) INDEX_Set(par_rep_set_p, pos);
	    mem_length = LEN_MemberValue(member_p);
	    ts_new((Timestamp_t *)ts);      /* Ignore errors, use null */
	    tlog.len += update_dns_build(update_p,
					 UD_present,
					 (Timestamp_t *)ts,
					 dns_Attribute(DNS_PARENTCELLPOINTERS),
					 BL_true,
					 mem_length,
					 (byte_t *)member_p->mv_value);
	    upptr_tlog_cnt++;
	    update_p = (Update_t *)SKIP_Update(update_p);
	} /* end for */

	/* Check that we didn't overrun our allocated buffer */
	if (tlog.len > total_bufsize) {
	    dce_svc_printf(CDS_S_SERVER_ALIASOVERFLOW_MSG);
	    status = CDS_NAMESERVERBUG;
	    goto exit_add_cell_alias;
	}

    } /* end updating parent cell pointers */

    /*
     *  Insert new values into their respective attribute
     */
    /* Initialize update_p to point to beginning of tlog buffer */
    update_p = (Update_t *) tlog_p;
    tlog.cnt = alias_tlog_cnt + upptr_tlog_cnt;

    /* Loop through the updates and apply each change to the attribute */
    for (pos = 0; pos < alias_tlog_cnt; pos++) {
        (void)set_insert(&stream_p->db_dir, update_p,
                         dns_Attribute(DNS_CELLALIASES), TRUE);
        update_p = (Update_t *)SKIP_Update(update_p);
    }

    if (how == AsChild) {
	/* Loop through the updates and apply each change to the attribute */
	for (pos = 0; pos < upptr_tlog_cnt; pos++) {
	    (void)set_insert(&stream_p->db_dir, update_p,
			     dns_Attribute(DNS_PARENTCELLPOINTERS), TRUE);
	    update_p = (Update_t *)SKIP_Update(update_p);
	}
    }

    /* Update cell parent cell up pointer and the cell aliases */
    db_dir_update(stream_p, &tlog);

    /*
     *  Initiate skulk of the root directory
     */
    if (how != AsInherit) {
	status = skulk_sched(stream_p);
	if (status != CDS_SUCCESS) {
	    DCE_SVC_DEBUG((
			   cds__svc_handle,
			   cds_svc_server,
			   svc_c_debug2,
			   "add_cell_alias(%s) - ignored skulk failure(%s)",
			   deb_ascii_user_descriptor(userBuf,user_p),
			   deb_ascii_dns_status(statusBuf,status)));
	    status = CDS_SUCCESS;
	}
    }


exit_add_cell_alias:
    if ((status != CDS_SUCCESS) && (childptr_created == BL_true)) {
	/*
	 *  Error occurred, so back out!  Try three times.
	 */
	for (pos = 0; pos < 3; pos++) {
	    dns_status_t local;

	    local = clerk_delete_child(alias_p);
	    if (local == CDS_SUCCESS) {
		break;
	    }
	    dce_svc_printf(CDS_S_SERVER_ALIASBACKOUTCHILD_MSG, (long)local);
	}
    }
    if (stream_p)  db_close_stream(stream_p);
    if (parent_replicas_p)  dns_free(parent_replicas_p);
    if (tlog_p)  dns_free(tlog_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <add_cell_alias(%s), returns %s\n",
	deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_dns_status(statusBuf,status)));

    return status;

}  /* end add_cell_alias */

/*
 *++
 *   remove_cell_alias
 *
 *  Description:
 *      Routine to remove an alias from the CDS_CellAliases attribute of the
 *       root directory.
 *      The rules are:
 *
 *          When removing an alias:
 *            R1. The alias to be removed cannot be the current preferred
 *                alias.
 *            R2. The alias to be removed cannot be the cellname.
 *            R3. The timestamp of the preferred alias must be < AllUpTo.
 *            R4. If the current cell name is not in the alias list, add
 *                it as the preferred alias if no other preferred alias
 *                is asserted, else add it as a non-preferred alias.
 *
 *      The algorithm to modify the CDS_CellAliases attribute is to
 *      remove the entire attribute, and then add each alias value.
 *
 *  Formal Parameters:
 *      user_p         - ptr to user_descriptor  (IN)
 *      chid_p         - ptr to clearinghouse uuid  (IN)
 *      alias_p        - cell alias to add/remove  (IN)
 *
 *  Returns:
 *
 *  Side Effects:
 *
 *--
 */
dns_status_t
remove_cell_alias (
    user_descriptor_t *user_p,
    uuid_t            *chid_p,
    FullName_t        *alias_p)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_ObjUID(chidBuf)
    DEB_ASCII_BUF_FullName(aliasBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    byte_t  work_buffer[sizeof(byte_t) + sizeof(FullName_t)];
    byte_t *work_p = work_buffer;
    file_info_t   *file_p;
    Update_t      *update_p;            /* Update work buffer */
    tlog_dsc_t     tlog;                /* Transaction log update buf */
    Timestamp_u    ts;                  /* Current timestamp */
    DBSet_t       *alias_set_p;
    int            alias_set_length;
    DBSetMember_t *bootstrap_p;
    int            index, pos, alias_set_num, bufsize, locstat;
    int            preferred_cell = BL_false;
    int            bootstrap_flag = 0;  /* 0 = don't bootstrap */
                                        /* 1 = bootstrap, not preferred */
                                        /* 2 = bootstrap preferred */
    db_stream_t   *stream_p = NULL;
    byte_t        *tlog_buf_p = NULL;   /* Allocated tlog updates */
    dns_status_t   status = CDS_SUCCESS;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >remove_cell_alias(%s, %s, %s)\n",
	deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ObjUID(chidBuf,chid_p),
        deb_ascii_FullName(aliasBuf, alias_p)));

    status = db_open_stream(chid_p, &stream_p, OP_ModifyAttribute);
    if (status != CDS_SUCCESS) {
        goto exit_remove_cell_alias;
    }
    file_p = stream_p->db_file_p;

    /* Check that this is the master replica of the root directory */
    if (!file_p->file_root_dir_p) {
        dce_svc_printf(CDS_S_SERVER_ALIASNOTROOT_MSG);
        status = CDS_NAMESERVERBUG;
        goto exit_remove_cell_alias;
    }
    if (file_p->file_root_dir_p->dir_replica_type != RT_master) {
        dce_svc_printf(CDS_S_SERVER_ALIASNOTMASTER_MSG);
        status = CDS_NAMESERVERBUG;
        goto exit_remove_cell_alias;
    }

    COPY_FullName(file_p->file_root_dir_p->dir_name, &stream_p->db_key);
    status = db_dir_read_for_update(stream_p);
    if (status != CDS_SUCCESS) {
        goto exit_remove_cell_alias;
    }

    /*
     * Security check (bypass if user_descriptor is null (used by diag))
     */
    if (user_p != NULL) {
	status = security_check(user_p, stream_p, Super_check, ET_directory,
				NULL);
	if (status != CDS_SUCCESS) {
	    goto exit_remove_cell_alias;
	}
    }

    if (find_attribute(stream_p->db_dir.data_p,
                       dns_AttributeName(DNS_CELLALIASES),
                       &alias_set_p, &index) != CDS_SUCCESS) {
        alias_set_p = NULL;
        alias_set_length = 0;
        alias_set_num = 0;
    }
    else {
        alias_set_length = LEN_DBSet(alias_set_p);
        alias_set_num = NUM_DBSet(alias_set_p);
    }

    /*
     *  Bootstrap check - Rule R4., part 1 -
     *  Check if we need to add the cell name to the alias set.
     */
    if (!EQ_FullName(alias_p,
                     (FullName_t *) file_p->file_root_dir_p->dir_name)) {
        locstat = cell_alias_test(file_p,
                                  (FullName_t *) file_p->file_root_dir_p->dir_name);
        if ((locstat == -1) || (locstat == 2)) {  /* not in alias list */
            bootstrap_flag = 1;
        }
    }
    else {
	/*
	 *  Rule R2., cannot remove cellname.
	 */
	status = CDS_CANNOT_REM_CELLNAME;
        goto exit_remove_cell_alias;
    }

    /*
     *  Allocate a buffer large enough to hold all of the potential updates.
     *  This includes:
     *    - Fudge factor
     *    - The Update for the delete of the attribute
     *    - The possible update if the cell name is not in the alias set
     *    - Update overhead for each alias (which includes incoming)
     *    - Attribute Specifier for CDS_CellAliases for each alias
     *    - Value of alias for each alias
     */
    bufsize = (50 + sizeof(Update_overhead) * (alias_set_num + 3)) +
              (sizeof(dns_Attribute(DNS_CELLALIASES)) * (alias_set_num + 2)) +
              alias_set_length + sizeof(byte_t) * 2 + LEN_FullName(alias_p) *2;
    tlog_buf_p = (byte_t *) dns_malloc(bufsize);
    if (!tlog_buf_p) {
        status = CDS_NONSRESOURCES;
        goto exit_remove_cell_alias;
    }

    /*
     *  Initialize transaction log descriptor
     */
    ZERO_bytes(&tlog, sizeof(tlog_dsc_t));
    tlog.buf_p = tlog_buf_p;

    /* Initialize update_p to point to beginning of tlog buffer */
    update_p = (Update_t *) tlog_buf_p;

    /* Build the Delete CDS_CellAliases attribute update */
    tlog.len += update_dns_build(update_p,
                                 UD_absent,
                                 NULL,
                                 dns_Attribute(DNS_CELLALIASES),
                                 BL_false,
                                 0,
                                 NULL);
    tlog.cnt++;
    update_p = (Update_t *)SKIP_Update(update_p);

    /*
     *  Loop through each member of the current alias set.
     *  Note that we will always be adding the incoming alias (unless error),
     *  so if the incoming alias and a member have the same name, the
     *  incoming alias gets added and we don't add the mem_fname_p.
     */
    for (pos = 0; pos < alias_set_num; pos++) {
        DBSetMember_t *member_p;
        FullName_t    *mem_fname_p;     /* Ptr to current member fullname */
        unsigned char *mem_preferred_p; /* Is current member preferred?   */
        char           include_member, member_preferred;

        include_member = BL_false;
        member_preferred = BL_false;
        member_p = (DBSetMember_t *) INDEX_DBSet(alias_set_p, pos);
        if (!EXT8(member_p->dm_valid) ||
            !EXT8(member_p->dm_value_present)) continue;

        mem_preferred_p = member_p->dm_data;
        mem_fname_p = (FullName_t *) (mem_preferred_p + 1);

	/* Remove cell alias */

	if (EQ_FullName(alias_p, mem_fname_p)) {
	    if (*mem_preferred_p == BL_true) {
		/* Rule R1. */
		status = CDS_CANNOT_REM_PREFERRED;
		break;
	    }
	    else {
		DBSet_t *set_p;
		DBSetMember_t *preferredvalue_p, *alluptovalue_p;
		
		preferredvalue_p = alluptovalue_p = (DBSetMember_t *)NULL;
		/* Check for Rule R3. */
		/*
		 *  Check that the TS of the preferred alias < AllUpTo,
		 *  or else it's some error.
		 */
		status = get_preferred_member(alias_set_p,
					      &preferredvalue_p);
		if ((status == 0) || (status == CDS_MULT_PREFERRED)) {
		    break;
		}
		if (find_attribute(stream_p->db_dir.data_p,
				   dns_AttributeName(DNS_ALLUPTO),
				   &set_p, &index) != CDS_SUCCESS) {

		  evl_AttributeMissing(stream_p->db_file_p->file_ch_p,
				       &stream_p->db_dir_hash_p->dir_name,
				       dns_AttributeName(DNS_ALLUPTO));
		  status = CDS_MISSING_ALLUPTO;
		  break;
		}
		alluptovalue_p = (DBSetMember_t *)DATA_DBSet(set_p);
		if ((preferredvalue_p > (DBSetMember_t *)NULL) &&
		    (alluptovalue_p > (DBSetMember_t *)NULL)) {
		    if (dnsCmpCTS((dns_cts *)preferredvalue_p->dm_ts,
				  (dns_cts *)alluptovalue_p->dm_data) > 0) {
			status = CDS_REM_NOT_SAFE;
			break;
		    }
		}
		else {
		    dce_svc_printf(CDS_S_SERVER_ALIASBADPTR_MSG);
		    status = CDS_NAMESERVERBUG;
		    break;
		}
		
		/*
		 *  Check that we're not removing the cell name
		 */
		if (EQ_FullName(alias_p,
				(FullName_t *) file_p->file_root_dir_p->dir_name)) {
		    status = CDS_CANNOT_REM_PREFERRED;
		    break;
		}
		include_member = BL_false;
	    }
	}
	else {
	    include_member = BL_true;
	    member_preferred = *mem_preferred_p;
	}
	
        /* Build update to remove this member and set preferred flag */
        if (include_member) {
            ts_new((Timestamp_t *)ts);  /* Ignore errors, use null */
            work_p = work_buffer;
            PUT8(work_p, member_preferred);
            COPY_FullName(mem_fname_p, (FullName_t *)work_p);
            tlog.len += update_dns_build(update_p,
                                         UD_present,
                                         (Timestamp_t *)ts,
                                         dns_Attribute(DNS_CELLALIASES),
                                         BL_true,
                                         LEN_FullName(mem_fname_p)+sizeof(byte_t),
                                         work_buffer);
            tlog.cnt++;
            update_p = (Update_t *)SKIP_Update(update_p);
        }
    } /* end for */

    if (status != CDS_SUCCESS) {
        goto exit_remove_cell_alias;
    }

    /*
     *  If we get here okay, then we still need to add the update for the
     *  remove cell alias.
     */

    ts_new((Timestamp_t *)ts);  /* Ignore errors, use null */
    work_p = work_buffer;
    PUT8(work_p, preferred_cell);
    COPY_FullName(alias_p, (FullName_t *)work_p);
    tlog.len += update_dns_build(update_p,
                                 UD_absent,
                                 (Timestamp_t *)ts,
                                 dns_Attribute(DNS_CELLALIASES),
                                 BL_true,
                                 LEN_FullName(alias_p)+sizeof(byte_t),
                                 work_buffer);
    tlog.cnt++;
    update_p = (Update_t *)SKIP_Update(update_p);

    /*
     *  Bootstrap check - Rule R4., part 2 -
     *  See if we need to add the cell name as an alias.
     */
    if (bootstrap_flag) {
        FullName_t *root_dir_p = (FullName_t *) file_p->file_root_dir_p->dir_name;

        ts_new((Timestamp_t *)ts);      /* Ignore errors, use null */
        work_p = work_buffer;
        if (bootstrap_flag == 2) {      /* Set preferred flag */
            PUT8(work_p, BL_true);
        }
        else {
            PUT8(work_p, BL_false);
        }
        COPY_FullName(root_dir_p, (FullName_t *)work_p);
        tlog.len += update_dns_build(update_p,
                                     UD_present,
                                     (Timestamp_t *)ts,
                                     dns_Attribute(DNS_CELLALIASES),
                                     BL_true,
                                     LEN_FullName(root_dir_p)+sizeof(byte_t),
                                     work_buffer);
        tlog.cnt++;
    }

    /* Check that we didn't overrun our allocated buffer */
    if (tlog.len > bufsize) {
        dce_svc_printf(CDS_S_SERVER_ALIASOVERFLOW_MSG);
        status = CDS_NAMESERVERBUG;
        goto exit_remove_cell_alias;

    }

    /* Initialize update_p to point to beginning of tlog buffer */
    update_p = (Update_t *) tlog_buf_p;

    /* Loop through the updates and apply each change to the attribute */
    for (pos = 0; pos < tlog.cnt; pos++) {
        (void)set_insert(&stream_p->db_dir, update_p,
                         dns_Attribute(DNS_CELLALIASES), TRUE);
        update_p = (Update_t *)SKIP_Update(update_p);
    }

    /* Update the whole kit and kaboodle */
    db_dir_update(stream_p, &tlog);

    /*
     *  Initiate skulk of the root directory
     */
    status = skulk_sched(stream_p);
    if (status != CDS_SUCCESS) {
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug2,
		       "remove_cell_alias(%s) - ignored skulk failure(%s)",
		       deb_ascii_user_descriptor(userBuf,user_p),
		       deb_ascii_dns_status(statusBuf,status)));
	status = CDS_SUCCESS;
    }

exit_remove_cell_alias:
    if (stream_p)  db_close_stream(stream_p);
    if (tlog_buf_p)  dns_free(tlog_buf_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <remove_cell_alias(%s), returns %s\n",
	deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_dns_status(statusBuf,status)));

    return status;

}  /* end remove_cell_alias */

/*
 *++
 *   check_child_name
 *
 *  Description:
 *      Checks that the entry name is not already being used in the
 *      parent directory by checking for a child, object, or softlink
 *      of the same name.
 *
 *  Formal Parameters:
 *      name_p - entry name to check for  (IN)
 *
 *  Returns:
 *      status from read attribute call
 *      CDS_NONSRESOURCES - can't allocate read attribute buffer
 *
 *--
 */
static dns_status_t 
check_child_name (FullName_t *name_p)
{
    DEB_ASCII_BUF_FullName(nameBuf)
    dns_status_t       status;
    byte_t             linked;
    byte_t            *outbuf_p;

    outbuf_p = (byte_t *)dns_malloc(READ_BUF_SIZE);
    if (!outbuf_p) {
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug8,
		       " <check_child_name(%s) -> CDS_NONSRESOURCES",
		       deb_ascii_FullName(nameBuf, name_p)));

	return(CDS_NONSRESOURCES);
    }
    status = clerk_read_attribute_conf(name_p,
				       ET_childPointer, 
				       dns_AttributeName(DNS_CTS),
				       outbuf_p,
				       NULL, &linked, cdsConfHi);
    if (status == CDS_SUCCESS) {
	goto exit_check_child_name;
    }

    status = clerk_read_attribute_conf(name_p,
				       ET_object, 
				       dns_AttributeName(DNS_CTS),
				       outbuf_p,
				       NULL, &linked, cdsConfHi);
    if (status == CDS_SUCCESS) {
	goto exit_check_child_name;
    }

    status = clerk_read_attribute_conf(name_p,
				       ET_softlink, 
				       dns_AttributeName(DNS_CTS),
				       outbuf_p,
				       NULL, &linked, cdsConfHi);

exit_check_child_name:

    dns_free(outbuf_p);
    return status;
}  /* end check_child_name */


/*
 *++
 *   hcell_create_child
 *
 *  Description:
 *      Create a child pointer in the parent directory.
 *
 *  Formal Parameters:
 *      stream_p - open stream to clearinghouse  (IN)
 *      child_p  - the child pointer to create  (IN)
 *      user_p   - who to create the child pointer on behalf of  (IN)
 *
 *  Returns:
 *      status from clerk_create_child
 *
 *--
 */
static dns_status_t 
hcell_create_child (db_stream_t       *stream_p,
		    user_descriptor_t *user_p,
		    FullName_t        *child_p)
{
    dns_status_t status;
    ReplicaPointer_u 	replica;		/* replica data storage  */
    MemberValue_t      *member_p;
    ObjUID_t 		dirid;			/* id for directory  */
    ObjUID_t 	        parent_uid;	        /* id for parent  */
    nsgbl_ch_t 	       *ch_p;			/* clearinghouse information */
    sec_id_foreign_t    onbehalfof, *on_p = &onbehalfof;
    Set_t              *const rpset_p = (Set_t *)replica;

    ch_p = stream_p->db_file_p->file_ch_p;
    
    /* 
     * Build the key and create the directory.
     * The case of the directory name is kept in the parent pointer.
     */
    INS8(rpset_p->st_flag, ST_present);
    INS16(rpset_p->st_offsetLength, sizeof(word_u));
    member_p = (MemberValue_t *)DATA_Set(rpset_p);
    INS16(OFFSET_Set(rpset_p, 0), DIFF_bytes(member_p, rpset_p));
    INS16(member_p->mv_length,
	  replica_build((ReplicaPointer_t *)member_p->mv_value,
			&ch_p->ch_uid, NULL, RT_master, &ch_p->ch_name));
    INS8(member_p->mv_flag, MV_present);
    status = ts_new((Timestamp_t *)member_p->mv_timestamp);
    INS8(member_p->mv_valuetype, VT_ReplicaPointer);
    INS16(rpset_p->st_valueLength, LEN_MemberValue(member_p));

#ifdef DCE_SEC
    if (user_p->ud_is_pac) {
	onbehalfof.id = user_p->ud_pac.principal;
	onbehalfof.realm = user_p->ud_pac.realm;
    } 
    else {
	on_p = (sec_id_foreign_t *)NULL;
    }
#else /* !DCE_SEC */
    
    on_p = (sec_id_foreign_t *)NULL;
    
#endif /* !DCE_SEC */

    /* Generate CTS */
    (void)id_new(&dirid);

    status = clerk_create_child(child_p,
				(byte_t *)on_p, &dirid, rpset_p, 
				&parent_uid);

    return status;

}  /* end hcell_create_child */

/*
 *++
 *   check_cyclical_name
 *
 *  Description:
 *      Check that a name will not cause a hierarchical cycle.
 *
 *  Formal Parameters:
 *      new_name_p     - the proposed name to check  (IN)
 *      current_cell_p - the current preferred alias  (IN)
 *
 *  Returns:
 *      CDS_NOTPREFERREDHIERARCHY - the proposed parent is not it's
 *                                  preferred alias
 *      CDS_ALIASCYCLE
 *      CDS_SUCCESS
 *
 *--
 */
static dns_status_t 
check_cyclical_name (FullName_t *new_name_p,
		     FullName_t *current_cell_p)
{
    FullName_t    new_parent;
    FullName_t    new_parent_alias;
    FullName_t   *compare_p;
    int           current_cell_cnt, match_cnt;
    dns_status_t  status;

    names_stripright(new_name_p, &new_parent);
    current_cell_cnt = names_count(&new_parent);
    if (current_cell_cnt == 1) {
	/* Then we're only left with /... so don't check */
	return CDS_SUCCESS;
    }
    status = clerk_get_preferred_alias(&new_parent, &new_parent_alias);
    if (status == CDS_SUCCESS) {
	if (NE_FullName(&new_parent, &new_parent_alias)) {
	    return CDS_NOTPREFERREDHIERARCHY;
	}
    }
    else if (status == CDS_NOTFND) {	/* No preferred alias found */
	COPY_FullName(&new_parent, &new_parent_alias);
    }
    else {
	return status;
    }

    current_cell_cnt = names_count(current_cell_p);
    match_cnt = match_fullnames(current_cell_p, &new_parent_alias);
    if (current_cell_cnt == match_cnt) {
	return CDS_ALIASCYCLE;
    }

    return CDS_SUCCESS;

}  /* end check_cyclical_name */
