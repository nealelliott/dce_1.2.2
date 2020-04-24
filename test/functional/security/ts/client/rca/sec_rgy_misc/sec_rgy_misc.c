/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1995, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
** Copyright (c) Hewlett-Packard Company 1995
** Unpublished work. All Rights Reserved.
*/
/*
 * HISTORY
 * $Log: sec_rgy_misc.c,v $
 * Revision 1.1.4.1  1996/07/09  15:01:07  arvind
 * 	Fix some API arguments. Some extra cleanup.
 * 	[1996/06/14  20:55 UTC  jrr  /main/jrr_122_1/1]
 *
 * Revision 1.1.2.2  1996/02/17  23:11:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:23  marty]
 * 
 * Revision 1.1.2.1  1995/12/11  19:53:38  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/27  17:50 UTC  bartf
 * 	add sec_rgy_misc test
 * 
 * 	HP revision /main/bartf_sec_90/3  1995/06/27  17:42 UTC  bartf
 * 	add po
 * 
 * 	HP revision /main/bartf_sec_90/2  1995/06/19  21:37 UTC  bartf
 * 	add passwd_override test
 * 
 * 	HP revision /main/bartf_sec_90/1  1995/06/15  16:51 UTC  bartf
 * 	initial test for sec_rgy_pgo_get_by_eff_unix_num with no passwd_override
 * 	[1995/12/11  19:27:20  root]
 * 
 * $EndLog$
 */


#include <tet_api.h>
#include <ts-common.h>
#include <dce/pgo.h>
#include <dce/uuid.h>


static void tp1();
static void sec_rgy_misc_startup(void);
static void sec_rgy_misc_cleanup(void);

/* 
 * Initialize TCM data structures 
 */

void (*tet_startup)() = sec_rgy_misc_startup;
void (*tet_cleanup)() = sec_rgy_misc_cleanup;

struct tet_testlist tet_testlist[] = {
    {tp1, 1},	/* sec_rgy_pgo_get_by_eff_unix_num() */
    {NULL, 0}
};


/*
 * Test Case Wide Declarations 
 */

static char                     msg[1024];		/* buffer for info lines */
static sec_rgy_handle_t         read_context;
static sec_rgy_handle_t         write_context;
static sec_rgy_cursor_t         cursor;
static sec_rgy_name_t           tester = "tester";

static void
sec_rgy_misc_startup(void)
{
/* save the passwd_override file if there is one and use ours */
    system("mv /opt/dcelocal/etc/passwd_override  /opt/dcelocal/etc/passwd_override.save");
    system("cp ./passwd_override /opt/dcelocal/etc/passwd_override");

    SEC_RGY_SITE_OPEN_READ(&read_context);
    SEC_RGY_SITE_OPEN_WRITE(&write_context);

}

static void
sec_rgy_misc_cleanup(void)
{
    error_status_t st;

    sec_rgy_pgo_delete (write_context, (sec_rgy_domain_t)sec_rgy_domain_person,
                       tester, &st );

    sec_rgy_site_close(read_context, &st);
    sec_rgy_site_close(write_context, &st);
 
/* move the orig passwd_override back */
    system("mv /opt/dcelocal/etc/passwd_override.save /opt/dcelocal/etc/passwd_override");

}

static void
tp1() /* test sec_rgy_pgo_get_by_eff_unix_num() */
{
    error_status_t	st, status;    
    sec_rgy_name_t	name, scope;
    sec_rgy_pgo_item_t  itemInfo1, itemInfo2, itemInfo3, itemInfo4;
    boolean32		overridden;

    mtet_infoline("tp1() {No passwd_override}: test sec_rgy_pgo_get_by_eff_unix_num()\n");

    strcpy(scope, "");

/* for non-override user */
    sec_rgy_cursor_reset(&cursor);

    sec_rgy_pgo_get_next ( read_context,
                           (sec_rgy_domain_t) sec_rgy_domain_person,
                            scope,
                            &cursor,
                            &itemInfo1,
                            name,
                            &st );
    CHECK_RESULT(true, "SEC_RGY_MISC: tp1: sec_rgy_pgo_get_next(): ",
                 &st, error_status_ok);

    sec_rgy_pgo_get_by_eff_unix_num( read_context, /* context */
				     sec_rgy_domain_person, /* name_domain */
				     scope,
				     itemInfo1.unix_num, /* unix_id */
				     false, /* allow aliases */
  				     &cursor,
                                     &itemInfo2,
				     name,
				     &overridden,
				     &st );

    if (!uuid_equal (&itemInfo1.id, &itemInfo2.id, &status) ||
         itemInfo1.unix_num != itemInfo2.unix_num ||
         itemInfo1.quota != itemInfo2.quota ||
         itemInfo1.flags != itemInfo2.flags ||
         strcmp (itemInfo1.fullname, itemInfo2.fullname) || 
         overridden != false ) {
       	tet_result(TET_FAIL);
        return;
    } else {
    	CHECK_RESULT(true, "SEC_RGY_MISC: tp1 {No passwd_override}: sec_rgy_pgo_get_by_eff_unix_num(): ",
                 &st, error_status_ok);
    }
/* ----------------------------------------------------------------------*/

/* For override user 
   need to open an update site (open_write)
   login as cell_admin
   add one principal to rgy( tester using default data)
   add this principal to /opt/dcelocal/etc/passwd_override (tester:*:1234:12:::)

   sec_rgy_pgo_get_by_eff_unix_num (1234), then compare item.fullname , tester
   and override need to be true.
 */
    mtet_infoline("tp1() {passwd_override}: test sec_rgy_pgo_get_by_eff_unix_num()\n");

/* Add a test person item to the registry. */

    bzero ((char *)&itemInfo3, sizeof (itemInfo3));

    uuid_create_nil (&itemInfo3.id, &status);

    itemInfo3.unix_num = 1234; 
    itemInfo3.quota = 1;
    itemInfo3.flags = sec_rgy_pgo_flags_none;

    memcpy ((char *)itemInfo3.fullname,
	   (char *)"Security Tester\0",
	   sizeof (itemInfo3.fullname));

    sec_rgy_cursor_reset( &cursor );

    sec_rgy_pgo_add( write_context,
		     sec_rgy_domain_person,
		     tester,
		     &itemInfo3,
		     &st );		     
    CHECK_RESULT(true, "SEC_RGY_MISC: tp1: sec_rgy_pgo_add(): ",
                 &st, error_status_ok);    

    sec_rgy_pgo_get_by_eff_unix_num( read_context, /* context */
				     sec_rgy_domain_person, /* name_domain */
				     scope,
				     itemInfo3.unix_num, /* unix_id */
				     false, /* allow aliases */
  				     &cursor,
                                     &itemInfo4,
				     name,
				     &overridden,
				     &st );

    CHECK_RESULT(true, "SEC_RGY_MISC: tp1 { passwd_override}: sec_rgy_pgo_get_by_eff_unix_num(): ",
                 &st, error_status_ok);

    if ( strcmp ( "Security Tester", itemInfo4.fullname ) ) {
        mtet_infoline("SEC_RGY_MISC: tp1 { passwd_override}: fullname compare failed.\n");
        mtet_infoline("%s\n", itemInfo4.fullname );
       	tet_result(TET_FAIL);
        return;
    }
    if ( overridden != true ) {
        mtet_infoline("SEC_RGY_MISC: tp1 { passwd_override}: overridden should be true.\n");
        mtet_infoline("%d\n", overridden );
       	tet_result(TET_FAIL);
        return;
    } 
    

    tet_result(TET_PASS);
}

