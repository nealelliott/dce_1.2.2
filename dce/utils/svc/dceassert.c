/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * The dce_assert() macro; like ANSI C's assert().
 */

/*
 * HISTORY
 * $Log: dceassert.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:02  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:37:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:28  root]
 * 
 * Revision 1.1.4.3  1994/08/01  15:30:47  rsalz
 * 	Cleanup typo's in internal dce_assert calls (OT CR 11502).
 * 	[1994/08/01  15:13:16  rsalz]
 * 
 * Revision 1.1.4.2  1994/07/27  12:27:55  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:33:38  rsalz]
 * 
 * Revision 1.1.4.1  1994/06/09  16:05:54  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:30  devsrc]
 * 
 * Revision 1.1.2.2  1993/08/16  18:08:07  rsalz
 * 	Initial release
 * 	[1993/08/16  18:02:54  rsalz]
 * 
 * $EndLog$
 */

#include <svcimpl.h>
#if	!defined(DCE_ASSERT)
#define DCE_ASSERT
#endif	/* !defined(DCE_ASSERT) */
#include <dce/assert.h>


static dce_svc_subcomp_t dce_svc_table[] = {
    { (idl_char *)"assert", (idl_char *)"DCE assertion failures",
	0, svc_c_debug_off }
};


void
dce___assert(
    dce_svc_handle_t		handle,
    const char			*text,
    const char			*file,
    int				line
)
{
    static dce_svc_handle_t	myhandle;
    error_status_t		st;

    /* Build a fake handle if needed. */
    if (handle == NULL) {
	if (myhandle == NULL)
	    myhandle = dce_svc_register(dce_svc_table, (idl_char*)"dce", &st);
	handle = myhandle;
    }

    dce_svc_printf(DCE_SVC(handle, "ssd"),
	0,
	svc_c_sev_fatal | svc_c_action_abort,
	svc_s_assertion_failed,
	text, file, line);
}
