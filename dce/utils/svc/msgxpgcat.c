/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Interface to XPG message catalogs.
 */

/*
 * HISTORY
 * $Log: msgxpgcat.c,v $
 * Revision 1.1.10.3  1996/02/18  19:22:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:18  marty]
 *
 * Revision 1.1.10.2  1995/12/08  21:37:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:44  root]
 * 
 * Revision 1.1.6.1  1994/10/25  19:45:25  bowe
 * 	Handle untranslatable message=0 in dce_msg_get_cat_msg(). [CR 12721]
 * 	[1994/10/25  19:43:47  bowe]
 * 
 * Revision 1.1.4.7  1994/09/30  19:44:55  rsalz
 * 	Change name of ISV-style message catalogs.
 * 	[1994/09/30  19:42:50  rsalz]
 * 
 * 	Implement DCE RFC 24.2 (OT CR 11929).
 * 	[1994/09/27  04:45:56  rsalz]
 * 
 * Revision 1.1.4.6  1994/09/13  15:27:08  rsalz
 * 	Make dce_msg__inq_comp thread-safe (OT CR 12067).
 * 	[1994/09/13  05:00:40  rsalz]
 * 
 * Revision 1.1.4.5  1994/09/07  21:05:05  bowe
 * 	If dce_msg_g_noxpgcat set, return msg_s_not_found. [CR 12079]
 * 	[1994/09/07  21:03:42  bowe]
 * 
 * Revision 1.1.4.4  1994/07/27  12:28:00  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:34:34  rsalz]
 * 
 * Revision 1.1.4.3  1994/06/09  16:06:07  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:42  devsrc]
 * 
 * Revision 1.1.4.2  1994/05/26  18:51:41  bowe
 * 	Replaced in-line code to get component name with new func call,
 * 	dce_msg__inq_comp().  [CR 10483,10478]
 * 	[1994/05/26  18:16:02  bowe]
 * 
 * Revision 1.1.4.1  1994/03/17  21:17:12  rsalz
 * 	Make dce_msg_*() routines thread-safe (OT CR 9897)
 * 	[1994/03/09  21:19:44  rsalz]
 * 
 * Revision 1.1.2.4  1993/11/04  18:53:46  rsalz
 * 	Use error_status_t, not unsigned32, for status codes
 * 	[1993/11/04  18:52:49  rsalz]
 * 
 * Revision 1.1.2.3  1993/08/30  14:27:48  rsalz
 * 	Use NL_CAT_LOCALE (XPG/4) if it exists
 * 	[1993/08/30  14:27:40  rsalz]
 * 
 * 	Use NL_CAT_LOCALE (XPG4) if it exists.
 * 
 * Revision 1.1.2.2  1993/08/16  18:09:03  rsalz
 * 	Initial release
 * 	[1993/08/16  18:03:58  rsalz]
 * 
 * $EndLog$
 */

#include <msgimpl.h>

#if	!defined(NL_CAT_LOCALE)
#define NL_CAT_LOCALE	0
#endif	/* !defined(NL_CAT_LOCALE) */


/*
**  One-shot routine to get a message from a DCE message catalog.
**  Leave the open message catalog cached.
*/
unsigned char *
dce_msg_get_cat_msg(
    unsigned32			message,
    error_status_t		*status
)
{
    static dce_msg_cat_handle_t	h;
    unsigned char		*p;

    /* Map "OK" to a messages that says "Success".
     * (Message catalogs can not have a message numbered 0). */
    if (message == 0)
	message = msg_s_ok_text;

    if (dce_msg_g_noxpgcat) {
	*status = msg_s_not_found;
	return NULL;
    }

    /* If we have an old open catalog, try to use it. */
    if (h != NULL) {
	if (h->top_bits == (message & ~h->mask)) {
	    p = dce_msg_cat_get_msg(h, message, status);
	    return p;
	}
	dce_msg_cat_close(h, status);
	h = NULL;
    }
	    
    /* Open catalog. */
    if ((h = dce_msg_cat_open(message, status)) == NULL)
	p = NULL;
    else
	p = dce_msg_cat_get_msg(h, message, status);

    return p;
}


/*
**  Given a "typical" message code, return a handle to the open
**  message catalog.
*/
dce_msg_cat_handle_t
dce_msg_cat_open(
    unsigned32			typical_message,
    error_status_t		*status
)
{
    dce_msg_cat_handle_t	h;
    char			buff[SMBUF];
    dce_msg_inqbuf_t		comp_name;
    dce_msg_inqbuf_t		tech_name;
    unsigned32			mask;
    nl_catd			cd;

    /* Build the message catalog name. */
    if (IS_ISV_MSG(typical_message)) {
	(void)sprintf(buff, "dce%6.6lx" CATALOG_SUFFIX,
	    (long)(typical_message >> ISV_COMP_SHIFT));
	mask = ISV_CODE_MASK;
    }
    else {
	dce_msg__inq_comp(typical_message, comp_name),
	dce_msg__inq_tech(typical_message, tech_name),
	(void)sprintf(buff, "%s%s" CATALOG_SUFFIX, tech_name, comp_name);
	mask = CODE_MASK;
    }

    /* Try to open the catalog. */
    if ((cd = catopen(buff, NL_CAT_LOCALE)) == (nl_catd)-1) {
	switch (errno) {
	default:
	    *status = msg_s_no_cat_open;
	    break;
	case ENOENT:
	    *status = msg_s_no_catalog;
	    break;
	case EPERM:
	    *status = msg_s_no_cat_perm;
	    break;
	}
	return NULL;
    }

    if ((h = (dce_msg_cat_handle_t)malloc(1 * sizeof *h)) == NULL) {
	catclose(cd);
	*status = msg_s_no_memory;
	return NULL;
    }
    h->cd = cd;
    h->mask = mask;
    h->top_bits = typical_message & ~mask;
    return h;
}


/*
**  Get a message from an open DCE message catalog.
*/
unsigned char *
dce_msg_cat_get_msg(
    dce_msg_cat_handle_t	handle,
    unsigned32			message,
    error_status_t		*status
)
{
    static char			NIL[] = "?NOPE?";
    unsigned char		*p;

    p = (unsigned char *)catgets(handle->cd, 1, (int)(message & handle->mask),
				    NIL);
    if (strcmp((char *)p, NIL) == 0) {
	*status = msg_s_not_found;
	return NULL;
    }

    return p;
}


/*
**  Close an open DCE message catalog.
*/
void
dce_msg_cat_close(
    dce_msg_cat_handle_t	handle,
    error_status_t		*status
)
{
    catclose(handle->cd);
    free(handle);
    *status = msg_s_ok;
}
