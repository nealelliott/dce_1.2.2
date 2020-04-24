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
 * $Log: server_hcell_support.c,v $
 * Revision 1.1.8.3  1996/02/18  19:36:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:17  marty]
 *
 * Revision 1.1.8.2  1995/12/08  15:32:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:49  root]
 * 
 * Revision 1.1.4.1  1994/10/26  19:22:43  zee
 * 	OT 12653: Add flag argument to name_in_cell_test.
 * 	[1994/10/26  19:22:18  zee]
 * 
 * Revision 1.1.2.6  1994/08/03  19:04:36  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:27:58  mccann]
 * 
 * Revision 1.1.2.5  1994/06/09  18:43:48  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:22  devsrc]
 * 
 * Revision 1.1.2.4  1994/05/06  16:04:28  zee
 * 	     Moved and renamed modify_cell_alias to server_hcell_maint.c.
 * 	     Made routines global as they get called from server_hcell_maint.c.
 * 	     Fixed miscalculation in name_in_cell_test.
 * 	[1994/05/05  21:20:58  zee]
 * 
 * Revision 1.1.2.3  1994/04/14  14:51:31  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:22:00  peckham]
 * 
 * Revision 1.1.2.2  1994/03/23  19:48:46  jd
 * 	Replaced NULL with 0 in 4 places. RIOS NULL is void pointer which can't be
 * 	compared with int 0.
 * 	[1994/03/23  19:06:51  jd]
 * 
 * Revision 1.1.2.1  1994/03/22  20:00:33  griffin
 * 	HCell BL3 Support: New Module!  Does all the MCA and HCell gruntwork.
 * 	[1994/03/22  18:13:46  griffin]
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
 *      Support routines for CDS hierarchical cells implementation.
 *
 */

#include <server.h>
#include <find.h>
#include <security.h>
#include <hcell.h>
#include <names.h>
#include <sets.h>
#include <uid.h>



/* -------------------------------------------------------------------------- */


/*
 *++
 *    cell_relative_fullname
 *
 *  Description:
 *      Routine that takes a CDS global name and translates it to a
 *      cell-relative name.
 *
 *  Formal Parameters:
 *      file_p        - ptr to clearinghouse file (IN)
 *      fname_in_p    - ptr to global name (IN)
 *      relname_out_p - ptr to buffer to receive the cell-relative name (OUT)
 *
 *  Implicit Inputs:
 *      none.
 *
 *  Implicit Outputs:
 *      none.
 *
 *  Returns:
 *      CDS_SUCCESS
 *
 *  Side Effects:
 *      none.
 *--
 */
dns_status_t
cell_relative_fullname (file_info_t *file_p,
                        FullName_t  *fname_in_p,
                        FullName_t  *relname_out_p)
{
    int           index, num_members, status;
    DBSet_t      *set_p;
    FullName_t   *alias_p;
    int           num_of_names;
    FullName_t   *longest_alias_p = NULL;
    int match_cnt = 0;
    int longest_match_cnt = 0;
    DEB_ASCII_BUF_FullName(nameBuf1)
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >cell_relative_fullname(%s), fullname %s\n",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_FullName(nameBuf1,fname_in_p)));

    /* Get the cell aliases for this cell if the root dir exists */
    if (file_p->file_root_dir_p == (dir_hash_entry_t *) NULL) {
        /*
         * Root directory does not exist, so return a cell-relative
         * name with the last simple name.  We do this by getting the
         * number of simple names of the incoming name and then
         * subtract one.
         */

        longest_match_cnt = names_count(fname_in_p) - 1;
        if (longest_match_cnt < 0) {
            longest_match_cnt = 0;
        }
        longest_alias_p = fname_in_p;
    }
    else {
        set_p = file_p->file_root_dir_p->dir_cellalias_set_p;
        if (set_p != (DBSet_t *) NULL) {
            /* Loop through cell aliases to find the longest matching name */
            num_members = NUM_DBSet(set_p);
            for (index = 0; index < num_members; index++) {
                DBSetMember_t *member_p;

                member_p = (DBSetMember_t *) INDEX_DBSet(set_p, index);

                if (!EXT8(member_p->dm_valid) ||
                    !EXT8(member_p->dm_value_present)) continue;

                /* Skip over preferred byte */
                alias_p = (FullName_t *)SKIP_byte(member_p->dm_data);

                match_cnt = match_fullnames(fname_in_p, alias_p);
                num_of_names = names_count((FullName_t *) alias_p);
                if (match_cnt == num_of_names) {
                    /*
                     * This cell alias is a subset of fname_in_p (names-wise)
                     */
                    if (match_cnt > longest_match_cnt) {
                        longest_alias_p = alias_p;
                        longest_match_cnt = match_cnt;
                    }
                }
            }  /* end for */
        }  /* end if (set_p != NULL) */


        /*
         *  Always use the cellname as an alias candidate (root directory name).
         */

        alias_p = (FullName_t *) file_p->file_root_dir_p->dir_name;

        match_cnt = match_fullnames(fname_in_p, alias_p);
        num_of_names = names_count((FullName_t *) alias_p);
        if (match_cnt == num_of_names) {
            if (match_cnt > longest_match_cnt) {
                longest_alias_p = alias_p;
                longest_match_cnt = match_cnt;
            }
        }

        if (longest_match_cnt == 0) {
            longest_alias_p = NULL;
        }

    } /* end else (file_p->file_root_dir_p != NULL) */

    if (longest_alias_p == NULL) {
        status = CDS_NOTINCELL;
    }
    else {     /* Should have the longest matching cell alias */
               /* longest_alias_p and longest_match_cnt should be set */

        SimpleName_t *in_sname_p, *out_sname_p;
        unsigned int  new_fname_length = 0;
        byte_u        cell_root_sname[sizeof(byte_u)*4] =
                                {SN_cds, 0x02, '.', ':'};

        /* Copy the fullname root info (ignore fn_length for now) */
        COPY_bytes(fname_in_p->fn_root, relname_out_p->fn_root,
                   sizeof(ObjUID_u));

        /* Then reduce the longest alias to "/.:" in simplename terms */
        COPY_SimpleName(&cell_root_sname, relname_out_p->fn_name);
        new_fname_length = LEN_SimpleName(&cell_root_sname);
        out_sname_p = (SimpleName_t *)
                      SKIP_SimpleName((SimpleName_t *) relname_out_p->fn_name);

        /*
         * Then append the rest of the simplenames from the original string
         * by skipping past the matched simplenames and then copying
         * the rest of the simplenames.
         */
        in_sname_p = (SimpleName_t *) fname_in_p->fn_name;
        for (index = 0; index < longest_match_cnt; index++)
            in_sname_p = (SimpleName_t *) SKIP_SimpleName(in_sname_p);

        while (EXT8(in_sname_p->sn_flag) != SN_null) {
            COPY_SimpleName(in_sname_p, out_sname_p);
            new_fname_length = new_fname_length + LEN_SimpleName(in_sname_p);
            in_sname_p = (SimpleName_t *) SKIP_SimpleName(in_sname_p);
            out_sname_p = (SimpleName_t *) SKIP_SimpleName(out_sname_p);
        }
        /* Copy ending null simplename */
        COPY_SimpleName(NullSimpleName, out_sname_p);
        new_fname_length = new_fname_length + sizeof(NullSimpleName);

        /* Update the output name length */
        INS16(relname_out_p->fn_length, new_fname_length);
        status = CDS_SUCCESS;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <cell_relative_fullname(%s), returns (%s)\n",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dns_status(statusBuf,status)));

    return status;
} /* end cell_relative_fullname */



/* -------------------------------------------------------------------------- */


/*
 *++
 *    cell_alias_test
 *
 *  Description:
 *      Routine to test if the given CDS name is a cell alias of the given
 *      cell.
 *
 *  Formal Parameters:
 *      file_p      - pointer to clearinghouse file (IN)
 *      TestName_p  - name to test for. (IN)
 *
 *  Implicit Inputs:
 *      none.
 *
 *  Implicit Outputs:
 *      none.
 *
 *  Returns:
 *      -1 if TestName_p is not a cell alias.
 *       0 if TestName_p is a cell alias, but not the preferred cell alias.
 *       1 if TestName_p is the preferred cell alias.
 *       2 if TestName_p is not in the alias list, but matches the cell name.
 *
 *  Side Effects:
 *      none.
 *--
 */
int
cell_alias_test (file_info_t *file_p,
                 FullName_t *TestName_p)
{
    int           num_members, index, status;
    int           test = -1;
    FullName_t   *alias_p;
    DBSet_t      *set_p;
    DEB_ASCII_BUF_FullName(nameBuf1)
    DEB_ASCII_BUF_file_info(fileBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >cell_alias_test(%s), testname %s\n",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_FullName(nameBuf1,TestName_p)));

    /* Get the cell aliases for this cell */
    set_p = file_p->file_root_dir_p->dir_cellalias_set_p;
    if (set_p != (DBSet_t *) NULL) {
        /* Loop through the aliases in the set to see if there is a match */
        num_members = NUM_DBSet(set_p);
        for (index = 0; index < num_members; index++) {
            DBSetMember_t *member_p;
            byte_t        *nameptr_p;

            member_p = (DBSetMember_t *) INDEX_DBSet(set_p, index);
            if (!EXT8(member_p->dm_valid) ||
                !EXT8(member_p->dm_value_present)) continue;

            /* Skip over preferred byte */
            nameptr_p = SKIP_byte(member_p->dm_data);

            /* Compare full names */
            status = cdsCmpFull((cds_full_name_t *) TestName_p,
                                (cds_full_name_t *) nameptr_p);
            if (status == 0) {
                test = 0;
                if (EXT8(member_p->dm_data) == BL_true)
                    test = 1;
                break;
            }
        }  /* end for */
    }  /* end if */

    if (test < 0) {
        /*
         *  If we can't get the cell alias from the directory entry,
         *  use the cellname (root directory name).
         */
        alias_p = (FullName_t *) file_p->file_root_dir_p->dir_name;

        /* Compare full names */
        status = cdsCmpFull((cds_full_name_t *) TestName_p,
                            (cds_full_name_t *) alias_p);
        if (status == 0)
            test = 2;      /* Treat cellname as preferred alias */
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <cell_alias_test(%s), returns %d\n",
        deb_ascii_file_info(fileBuf,file_p), test));

    return(test);
} /* end cell_alias_test */



/* -------------------------------------------------------------------------- */

/*
 *++
 *    name_in_name_test
 *
 *  Description:
 *      Routine to test if a given CDS name is in the given Cell name.
 *
 *  Formal Parameters:
 *      TestName_p - name to test for. (IN)
 *      CellName_p - name to test for inclusion in. (IN)
 *
 *  Implicit Inputs:
 *      none.
 *
 *  Implicit Outputs:
 *      none.
 *
 *  Returns:
 *      cdsLocalRef if TestName_p is in this cell.
 *      cdsParentRef if TestName_p is in the parent cell.
 *      cdsGDARef if TestName_p is above the parent cell.
 *
 *  Side Effects:
 *
 *--
 */
static cdsNameRef_t
name_in_name_test (FullName_t   *TestName_p,
		   FullName_t   *CellName_p)
{
    int   smp_match_cnt;	/* Count of matching number of simple names */
    int   num_of_names = 0;     /* Number of simple names in the cell name */

    num_of_names = names_count(CellName_p);

    /* Get number of leading simplenames which match the cell name */
    smp_match_cnt = match_fullnames(TestName_p, CellName_p);

    /*
     * All names start with /... so the first name always matches,
     * so parent references have to have 2 or more matches.
     */

    if (smp_match_cnt >= num_of_names) {  /* Local cell reference */
	return cdsLocalRef;
    }
    else if (smp_match_cnt > 1) {         /* Parent cell reference */
	return cdsParentRef;
    }
    else {                                /* GDA reference */
	return cdsGDARef;
    }
}

/* -------------------------------------------------------------------------- */

/*
 *++
 *    name_in_cell_test
 *
 *  Description:
 *      Routine to test if a given CDS name is in a given cell.
 *
 *  Formal Parameters:
 *      file_p     - pointer to clearinghouse file (IN)
 *      TestName_p - name to test for. (IN)
 *      include_aliases - whether to use the alias list for inclusion. (IN)
 *
 *  Implicit Inputs:
 *      none.
 *
 *  Implicit Outputs:
 *      none.
 *
 *  Returns:
 *      cdsLocalRef if TestName_p is in this cell.
 *      cdsParentRef if TestName_p is in the parent cell.
 *      cdsGDARef if TestName_p is above the parent cell.
 *
 *  Side Effects:
 *
 *--
 */
cdsNameRef_t
name_in_cell_test (file_info_t  *file_p,
                   FullName_t   *TestName_p,
		   int           include_aliases)
{
    DBSet_t       *set_p;
    FullName_t    *CellName_p;
    int            index, num_members;
    cdsNameRef_t   saved_ref = cdsGDARef;

    /* Get Cell Aliases */
    set_p = file_p->file_root_dir_p->dir_cellalias_set_p;
    if (set_p == (DBSet_t *) NULL) {
        /*
         * If we can't get the cell aliases from the directory entry,
         * use the cellname (root directory name), and the
	 * include_aliases flag is moot.
         */
        CellName_p = (FullName_t *) file_p->file_root_dir_p->dir_name;

	saved_ref = name_in_name_test(TestName_p, CellName_p);
	return saved_ref;

    }  /* end if */

    else {
	/*
	 * Loop through the cell alias list
	 */
        num_members = NUM_DBSet(set_p);
        for (index = 0; index < num_members; index++) {
            DBSetMember_t *member_p;
            byte_t        *nameptr_p;
	    cdsNameRef_t   tmp_ref = cdsGDARef;

            member_p = (DBSetMember_t *) INDEX_DBSet(set_p, index);
            if (!EXT8(member_p->dm_valid) ||
                !EXT8(member_p->dm_value_present)) continue;

            /* Skip over preferred byte */
            nameptr_p = SKIP_byte(member_p->dm_data);

	    if (!include_aliases) {
		/* Check preferred flag */
		if (EXT8(member_p->dm_data) == BL_true) {
		    CellName_p = (FullName_t *) nameptr_p;
		    saved_ref = name_in_name_test(TestName_p, CellName_p);
		    return saved_ref;
		}
		else
		    continue;
	    }  /* end if !include_aliases */

	    CellName_p = (FullName_t *) nameptr_p;
	    tmp_ref = name_in_name_test(TestName_p, CellName_p);
	    if (tmp_ref < saved_ref) {
		saved_ref = tmp_ref;
	    }

        }  /* end for */

	return saved_ref;
    }  /* end else */

} /* end name_in_cell_test */

/* -------------------------------------------------------------------------- */

/*
 *++
 *   transmogrify_cdsname
 *
 *  Description:
 *      Routine to translate a given CDS name to the equivalent name
 *      as it exists in the current (preferred) cell.
 *
 *  Formal Parameters:
 *      file_p    - pointer to clearinghouse file (IN)
 *      OldName_p - name to test for. (IN)
 *      NewName_p - the new (preferred) cell name. (IN/OUT)
 *
 *  Implicit Inputs:
 *      none.
 *
 *  Implicit Outputs:
 *
 *  Returns:
 *    CDS_SUCCESS if the OldName_p is a cell alias
 *    CDS_NOT_AN_ALIAS if the OldName_p is not a cell alias
 *
 *  Side Effects:
 *
 *--
 */
dns_status_t
transmogrify_cdsname (file_info_t  *file_p,
                      FullName_t   *OldName_p,
                      FullName_t   *NewName_p)
{
    int           index, num_members, status;
    DBSet_t      *set_p;
    FullName_t   *preferred_alias_p = NULL;
    FullName_t   *longest_alias_p = NULL;
    int           match_cnt = 0;
    int           longest_match_cnt = 0;
    DEB_ASCII_BUF_FullName(nameBuf1)
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >transmogrify_cdsname(%s), testname %s\n",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_FullName(nameBuf1,OldName_p)));

    /* Get the cell aliases for this cell */
    set_p = file_p->file_root_dir_p->dir_cellalias_set_p;
    if (set_p == NULL) {
        /*
         *  If we can't get the cell alias from the directory entry,
         *  there's nothing to translate from, so return error status.
         */
        return CDS_NOT_AN_ALIAS;
    }

    else {
        /*
         * Loop through all of the cell aliases to find the one with
         * the longest matching (not preferred) name with OldName_p.
         */
        num_members = NUM_DBSet(set_p);
        for (index = 0; index < num_members; index++) {
            DBSetMember_t *member_p;
            byte_t        *nameptr_p;
            int            num_of_names;

            member_p = (DBSetMember_t *) INDEX_DBSet(set_p, index);
            if (!EXT8(member_p->dm_valid) ||
                !EXT8(member_p->dm_value_present)) continue;

            /* Skip over preferred byte */
            nameptr_p = SKIP_byte(member_p->dm_data);

            /* Check preferred flag */
            if (EXT8(member_p->dm_data) == BL_true) {
                preferred_alias_p = (FullName_t *) nameptr_p;
                continue;
            }
            match_cnt = match_fullnames(OldName_p, (FullName_t *) nameptr_p);
            num_of_names = names_count((FullName_t *) nameptr_p);
            if (match_cnt == num_of_names) {
                /*
                 * This cell alias is a subset of OldName_p (names-wise)
                 */
                if (match_cnt > longest_match_cnt) {
                    longest_alias_p = (FullName_t *) nameptr_p;
                    longest_match_cnt = match_cnt;
                }
            }
        }  /* end for */
    }  /* end else */

    if (longest_alias_p == NULL)
        status = CDS_NOT_AN_ALIAS;

    else {     /* Should have the longest matching cell alias */
        SimpleName_t *in_sname_p, *out_sname_p;
        int           new_fname_length = 0;

        /* Copy the preferred alias fullname to the new name */
        if (preferred_alias_p == NULL) {
            /* If no preferred name, use cellname */
            preferred_alias_p = (FullName_t *) file_p->file_root_dir_p->dir_name;
        }
        COPY_FullName(preferred_alias_p, NewName_p);
        new_fname_length = EXT16(preferred_alias_p->fn_length) -
                             sizeof(NullSimpleName);

        /*
         * Then set an output pointer to point to the null simplename
         * that is currently at the end of NewName_p since we'll start
         * appending simplenames there.
         */
        out_sname_p = (SimpleName_t *) ((byte_t *)SKIP_FullName(NewName_p) -
                                        sizeof(NullSimpleName));

        /*
         * Then append the rest of the simplenames from the original string
         * by skipping past the matched simplenames and then copying
         * the rest of the simplenames.
         */
        in_sname_p = (SimpleName_t *) OldName_p->fn_name;
        for (index = 0; index < longest_match_cnt; index++)
            in_sname_p = (SimpleName_t *) SKIP_SimpleName(in_sname_p);

        while (EXT8(in_sname_p->sn_flag) != SN_null) {
            COPY_SimpleName(in_sname_p, out_sname_p);
            new_fname_length = new_fname_length + LEN_SimpleName(in_sname_p);
            in_sname_p = (SimpleName_t *) SKIP_SimpleName(in_sname_p);
            out_sname_p = (SimpleName_t *) SKIP_SimpleName(out_sname_p);
        }
        /* Copy ending null simplename and update fullname length */
        COPY_SimpleName(NullSimpleName, out_sname_p);
        new_fname_length = new_fname_length + sizeof(NullSimpleName);
        INS16(NewName_p->fn_length, new_fname_length);

        status = CDS_SUCCESS;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <transmogrify_cdsname(%s), returns %s\n",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dns_status(statusBuf,status)));

    return status;
} /* end transmogrify_cdsname */



/* -------------------------------------------------------------------------- */


/*
 *++
 *   match_fullnames
 *
 *  Description:
 *      Routine to match two fullnames to see how many consecutive simple
 *      names they have in common starting from the beginning of the
 *      fullname.
 *
 *  Formal Parameters:
 *      fname1_p - name of first fullname
 *      fname2_p - name of second fullname
 *
 *  Implicit Inputs:
 *      none.
 *
 *  Implicit Outputs:
 *
 *  Returns:
 *      number of matching simplenames
 *
 *  Side Effects:
 *
 *--
 */
int
match_fullnames(FullName_t *fname1_p,
                FullName_t *fname2_p)
{
    SimpleName_t *sname1_p, *sname2_p;
    int sname_match = 0;

    sname1_p = (SimpleName_t *) fname1_p->fn_name;
    sname2_p = (SimpleName_t *) fname2_p->fn_name;

    while ((EXT8(sname1_p->sn_flag) != SN_null) ||
           (EXT8(sname2_p->sn_flag) != SN_null)) {

        if (cdsCmpSimple((cds_simple_name_t *) sname1_p,
                         (cds_simple_name_t *) sname2_p) != 0)
            break;
        sname_match++;
        sname1_p = (SimpleName_t *) SKIP_SimpleName(sname1_p);
        sname2_p = (SimpleName_t *) SKIP_SimpleName(sname2_p);
    } /* end while */

    return sname_match;

} /* end match_fullnames */


/* -------------------------------------------------------------------------- */


/*
 *++
 *   get_preferred_member
 *
 *  Description:
 *      Routine to get the address of the preferred cell alias member from a
 *      cell alias set.
 *
 *  Formal Parameters:
 *      alias_set_p         - pointer to cell alias set.  (IN)
 *      preferred_member_pp - pointer to hold pointer to preferred
 *                            DBSetMember_t.  (IN/OUT)
 *
 *  Returns:
 *      - CDS_SUCCESS if one preferred alias found.
 *      - NULL if no preferred alias found.
 *      - CDS_MULT_PREFERRED if more than one alias found.
 *
 *  Side Effects:
 *      If CDS_MULT_PREFERRED, preferred_member_pp will point to the first
 *      preferred alias in the list.
 *
 *--
 */
int
get_preferred_member (DBSet_t *alias_set_p,
                      DBSetMember_t **preferred_member_pp)
{
    int pos;
    DBSetMember_t *preferred_alias_p = (DBSetMember_t *) NULL;

    for (pos = 0; pos < NUM_DBSet(alias_set_p); pos++) {
        DBSetMember_t *member_p;
        unsigned char *mem_preferred_p;     /* Ptr to preferred flag */

        member_p = (DBSetMember_t *) INDEX_DBSet(alias_set_p, pos);
        if (!EXT8(member_p->dm_valid) ||
            !EXT8(member_p->dm_value_present)) continue;

        mem_preferred_p = member_p->dm_data;
        if (*mem_preferred_p == BL_true) {
            if (!preferred_alias_p) {
                preferred_alias_p = member_p;
            }
            else {
                *preferred_member_pp = preferred_alias_p;
                dce_svc_printf(CDS_S_SERVER_ALIASMULTIPRFD_MSG);
                return CDS_MULT_PREFERRED;
            }
        }
    } /* end for */

    if (!preferred_alias_p) {
        *preferred_member_pp = (DBSetMember_t *) NULL;
        dce_svc_printf(CDS_S_SERVER_ALIASNOPRFD_MSG);
        return 0;
    }
    *preferred_member_pp = preferred_alias_p;
    return CDS_SUCCESS;

}  /* end get_preferred_member */



/* -------------------------------------------------------------------------- */

/*
 *  cds_full_to_simple
 *  cds_simple_to_full
 *
 *  These routine supports the various cell name hacks (which push cell
 *  names into simplenames (for advertisements).
 */

dns_status_t
cell_full_to_simple (
    FullName_t   *cell_fullname,
    SimpleName_t *cell_simplename)
{
    unsigned_char_t    temp_name[CDS_STR_FNAME_MAX+1];
    int                output_len = sizeof(temp_name);
    dns_status_t       status;


    /*
     *  Convert the full name passed in into an external name, specifying
     *  the suppress flag to be FALSE so that we get the namespace nickname
     *  in the converted string.
     *  Note: The output length includes the terminating null.
     */

    status = dnsCvtCDSFullToStr((cds_full_name_t *)cell_fullname,
                                temp_name, &output_len);
    if (status != DNS_SUCCESS) {
        return (status);
    }

    INS8(cell_simplename->sn_flag, SN_cds);
    INS8(cell_simplename->sn_length, output_len-1);

    COPY_bytes(temp_name, cell_simplename->sn_name, output_len-1);

    return (DNS_SUCCESS);

}

/* -------------------------------------------------------------------------- */


dns_status_t
cell_simple_to_full (
    SimpleName_t *cell_simplename,
    FullName_t   *cell_fullname)
{
    unsigned_char_t    temp_name[CDS_STR_FNAME_MAX+1];
    int                output_len = sizeof(temp_name);
    dns_status_t       status;


    /*
     *  Convert the simplename passed in to a full name (the simplename
     *  has a fullname inside it).
     */

    status = dnsCvtCDSSimpleToStr((cds_simple_name_t *)cell_simplename,
                                  temp_name, &output_len);
    if (status != DNS_SUCCESS) {
        return (status);
    }

    output_len = sizeof(*cell_fullname);
    status = dnsCvtCDSFullToOpq(temp_name, (cds_full_name_t *)cell_fullname,
                                &output_len);

    if (status != DNS_SUCCESS) {
        return (status);
    }
    return (DNS_SUCCESS);

}


/* -------------------------------------------------------------------------- */

/*
 * transmogrify_ch
 *
 * This routine performs all of the actions necessary to transform the
 * root level directory and clearinghouse to a new cell name.
 *
 * This routine is call by background() after all directory skulking has
 * been completed, and prior to checkpoint evaluation.
 *
 */

void
transmogrify_ch (file_info_t *file_p)
{

    dns_status_t      status;                   /* Current status  */
    int               failure_point = 0;        /* Tracks routine failures */
    nsgbl_ch_t       *ch_p = file_p->file_ch_p; /* Clearinghouse information */
    db_stream_t      *stream_p;                 /* Clearinghouse stream */
    int               index;                    /* Working index  */
    DBSet_t          *set_p;                    /* Work set */
    DBSetMember_t    *av_p;                     /* Work attr value */
    FullName_t       *old_name_p;               /* Points to old name */
    FullName_t        new_name;                 /* Holds tmog'd name */
    byte_t           *tlog_buf_p;               /* Alloc'd tlog updates */
    tlog_dsc_t        tlog;                     /* Transaction log update  */
    Update_t         *update_p;                 /* Update work buffer */
    Timestamp_u       ts;                       /* Work timestamp for updates */
    FullName_t        cell_full_name;           /* Holds translated cell */
    unsigned_char_t   string_cell_name[CDS_STR_FNAME_MAX+1]; /* Ditto */
    int               output_len;               /* Work length */
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >transmogrify_ch(%s)\n",
        deb_ascii_file_info(fileBuf,file_p)));

    /*
     * We're going to need some update buffer room.   Allocate the space,
     * and initialize the transaction log descriptor.
     * If we don't have enough memory, then we quietly return.  If things
     * are this tight, transmogrification is the last thing on our minds.
     */

    tlog_buf_p = (byte_t *)dns_malloc(sizeof(Update_u) * 2);
    if (!tlog_buf_p) {
        return;
    }

    /* Set up update buffer */
    tlog.cnt = 0;
    tlog.len = 0;
    tlog.buf_p = tlog_buf_p;
    update_p = (Update_t *)tlog_buf_p;

    /*
     * Perform the steps and leave at the first sign of trouble.  The
     * "do" here provides an easy escape hatch for the C language.
     * We track where we drop out for instrumentation and memory deallocation
     * purposes.
     */

    do {

        /*
         * Access the clearinghouse pseudo-directory information.
         */

        status = db_open_stream((ObjUID_t *)&ch_p->ch_uid,
                                &stream_p,
                                OP_ModifyAttribute);

        /* If this fails, reschedule request */
        if (status != DNS_SUCCESS) {
            failure_point = 1;
            break;
        }

        /* Get pseudo directory so we can locate list of directories */
        status = db_pseudo_read_for_update(stream_p);
        if (status != DNS_SUCCESS) {
            failure_point = 2;
            break;
        }

        /*
         * Locate the clearinghouse name (should be there).
         */

        status = find_attribute(stream_p->db_dir.data_p,
                    dns_AttributeName(DNS_CHNAME), &set_p, &index);
        if (status != DNS_SUCCESS) {
            failure_point = 3;
            break;
        }

        av_p = find_last_value(set_p);
        if (!EXT8(av_p->dm_valid) || !EXT8(av_p->dm_value_present))  {
            failure_point = 4;
            break;
        }


        /*
         * Try to change the name.   If we can't (or don't want to)
         * then move on to the cell name.   If we do, then create
         * an update packet to replace the clearinghouse name and
         * update the pseudo-directory appropriately.
         *
         * In addition to the pdir update, we record the new name
         * in the pdir hash entry and the CH management structure.
         */


        old_name_p = (FullName_t *)av_p->dm_data;
        status = transmogrify_cdsname(file_p,
                                      old_name_p,
                                      (FullName_t *) &new_name);

        if (status == DNS_SUCCESS) {
            status = ts_new((Timestamp_t *)ts);
            if (status != DNS_SUCCESS) {
                failure_point = 5;
                break;
            }
            tlog.len += update_dns_build(update_p,
                                         UD_present,
                                         (Timestamp_t *)ts,
                                         dns_Attribute(DNS_CHNAME),
                                         BL_true,
                                         LEN_FullName(&new_name),
                                         (byte_t *)&new_name);
            (void)set_replace_value(&stream_p->db_dir, update_p,
                  dns_Attribute(DNS_CHNAME));
            tlog.cnt++;
            update_p = (Update_t *)SKIP_Update(update_p);
            SERVER_LOCK(stream_p->db_dir_hash_p->dir_mutex) {
                COPY_FullName(&new_name, stream_p->db_dir_hash_p->dir_name);
            } SERVER_END_LOCK(stream_p->db_dir_hash_p->dir_mutex);
            SERVER_LOCK(file_p->file_ch_p->ch_mutex) {
                COPY_FullName(&new_name, &file_p->file_ch_p->ch_name);
            } SERVER_END_LOCK(file_p->file_ch_p->ch_mutex);
        }


        /*
         * Repeat for the cell name.   We have to jump through some
         * hoops because the cell name in the pdir is stored in VT_byte,
         * not a structured name.
         *
         * As with the clearinghouse name we record the new cell name
         * in the root directory hash entry and the clearinghouse management
         * data structure.    We have to do the former because the root
         * directory has no parentpointer the change cannot be directly
         * propagated from a master replica.  So we "discover" it here.
         */

        status = find_attribute(stream_p->db_dir.data_p,
                    dns_AttributeName(DNS_NSCELLNAME), &set_p, &index);
        if (status != DNS_SUCCESS) {
            failure_point = 6;
            break;
        }

        av_p = find_last_value(set_p);
        if (!EXT8(av_p->dm_valid) || !EXT8(av_p->dm_value_present))  {
            failure_point = 7;
            break;
        }

        COPY_bytes(av_p->dm_data, string_cell_name, av_p->dm_length);
        string_cell_name[av_p->dm_length] = 0;
        output_len = sizeof(cell_full_name);
        status = dnsCvtCDSFullToOpq(string_cell_name,
                                    (cds_full_name_t *)&cell_full_name,
                                    &output_len);

        if (status != DNS_SUCCESS) {
            failure_point = 8;
            break;
        }
        status = transmogrify_cdsname(file_p,
                                      &cell_full_name,
                                      (FullName_t *) &new_name);

        if (status == DNS_SUCCESS) {
            output_len = sizeof(string_cell_name);
            status = dnsCvtCDSFullToStr((cds_full_name_t *)&new_name,
                                        string_cell_name, &output_len);
            if (status != DNS_SUCCESS) {
                failure_point = 9;
                break;
            }
            status = ts_new((Timestamp_t *)ts);
            if (status != DNS_SUCCESS) {
                failure_point = 10;
                break;
            }
            tlog.len += update_dns_build(update_p,
                                         UD_present,
                                         (Timestamp_t *)ts,
                                         dns_Attribute(DNS_NSCELLNAME),
                                         BL_true,
                                         output_len-1,
                                         (byte_t *)&string_cell_name);
            (void)set_replace_value(&stream_p->db_dir, update_p,
                  dns_Attribute(DNS_NSCELLNAME));
            tlog.cnt++;

            /*
             * Reflect this change in the root directory's hash entry
             * as well.
             */

            SERVER_LOCK(file_p->file_mutex) {
                if (file_p->file_root_dir_p) {
                    COPY_FullName(&new_name, file_p->file_root_dir_p->dir_name);
                }
            } SERVER_END_LOCK(file_p->file_mutex);
            SERVER_LOCK(file_p->file_ch_p->ch_mutex) {
                COPY_FullName(&new_name, &file_p->file_ch_p->ch_cellname);
            } SERVER_END_LOCK(file_p->file_ch_p->ch_mutex);

        }

        if (tlog.cnt > 0) {
            status = db_pseudo_update(stream_p, &tlog);
        }

    } while (0);    /* Not a loop, just a convenient way to avoid work */

    /*
     * Clean up and go home.
     */

    if (failure_point != 1) {
        (void)db_close_stream(stream_p);
    }

    dns_free(tlog_buf_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <transmogrify_ch(%s), status: %s, fp: %d\n",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dns_status(statusBuf,status), failure_point));

}
