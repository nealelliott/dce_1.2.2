/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Get a message from somewhere.
 */

/*
 * HISTORY
 * $Log: msggetmsg.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:10  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:37:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:36  root]
 * 
 * Revision 1.1.4.9  1994/09/30  19:44:52  rsalz
 * 	Implement DCE RFC 24.2 (OT CR 11929).
 * 	[1994/09/27  04:45:27  rsalz]
 * 
 * Revision 1.1.4.8  1994/09/13  15:27:07  rsalz
 * 	Make dce_msg__inq_comp thread-safe (OT CR 12067).
 * 	[1994/09/13  05:00:36  rsalz]
 * 
 * Revision 1.1.4.7  1994/08/11  16:32:25  bowe
 * 	Back out setting return status in dce_error_inq_text().  It should
 * 	indeed be 0 or -1.  [CR 11588]
 * 	[1994/08/11  16:32:01  bowe]
 * 
 * Revision 1.1.4.6  1994/08/11  13:21:19  bowe
 * 	In dce_error_inq_text(), if status to convert is 0, use a "dummy"
 * 	message that says "Success ...".  [CR 11588]
 * 	[1994/08/11  13:20:50  bowe]
 * 
 * Revision 1.1.4.5  1994/07/27  12:27:57  rsalz
 * 	Silence compiler warnings (OT CR 11455).
 * 	[1994/07/27  03:33:57  rsalz]
 * 
 * Revision 1.1.4.4  1994/06/09  16:06:03  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:37  devsrc]
 * 
 * Revision 1.1.4.3  1994/05/26  18:51:40  bowe
 * 	Added new func dce_msg__inq_comp() to get component name.
 * 	Replaced in-line code to use it.  [CR 10483,10478]
 * 	[1994/05/26  18:15:10  bowe]
 * 
 * Revision 1.1.4.2  1994/05/18  19:22:10  rsalz
 * 	Fix dce_error_inq_text output (OT CR 10637).
 * 	[1994/05/13  19:17:51  rsalz]
 * 
 * Revision 1.1.4.1  1994/03/17  21:17:09  rsalz
 * 	Make dce_msg_*() routines thread-safe (OT CR 9897)
 * 	[1994/03/09  21:19:42  rsalz]
 * 
 * Revision 1.1.2.3  1993/11/04  18:53:45  rsalz
 * 	Use error_status_t, not unsigned32, for status codes
 * 	[1993/11/04  18:52:47  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:08:39  rsalz
 * 	Initial release
 * 	[1993/08/16  18:03:32  rsalz]
 * 
 * $EndLog$
 */

#include <msgimpl.h>


/*
**  Get message from catalog, or from in-core tables.  Return pointer to
**  allocated space that must be free'd.
*/
unsigned char *
dce_msg_get_msg(
    unsigned32			message,
    error_status_t		*status
)
{
    unsigned char		*p;
    unsigned char		*new;
    char			buff[128];

    LOCK();

    /* Get the message. */
    if ((p = dce_msg_get_cat_msg(message, status)) == NULL
     && (p = dce_msg_get_default_msg(message, status)) == NULL) {
	(void)sprintf(buff, "Unknown message number 0x%8.8lx", (long)message);
	p = (unsigned char *)buff;
	/* *status has already been filled in. */
    }
    else
	*status = msg_s_ok;

    /* Found it; return a copy. */
    if ((new = (unsigned char *)malloc(strlen((char *)p) + 1)) == NULL) {
	UNLOCK();
	*status = msg_s_no_memory;
	return NULL;
    }
    (void)strcpy((char *)new, (char *)p);
    UNLOCK();
    return new;
}


static void
dce__error_inq_isv(
    error_status_t		status_to_convert,
    unsigned char		*error_text,
    int				*status
)
{
    dce_msg_cat_handle_t	h;
    error_status_t		st;
    unsigned char		*p;

    if ((h = dce_msg_cat_open(status_to_convert, &st)) != NULL) {
	if ((p = dce_msg_cat_get_msg(h, status_to_convert, &st)) != NULL) {
	    /* Get base text. */
	    (void)strcpy((char *)error_text, (char *)p);

	    /* Get first message -- the ID. */
	    status_to_convert = (status_to_convert & ~ISV_CODE_MASK)
			      | ISV_ID_CODE;
	    if ((p = dce_msg_cat_get_msg(h, status_to_convert, &st)) != NULL) {

		/* Glue the two together. */
		sprintf((char *)&error_text[strlen((char *)error_text)],
		    " (%s)", (char *)p);
		dce_msg_cat_close(h, &st);
		if (status)
		    *status = 0;
		return;
	    }
	}
	dce_msg_cat_close(h, &st);
    }

    /* Failed. */
    sprintf((char *)error_text, "status 0x%8.8lx", (long)status_to_convert);
}


/*
**  Copy the text of a status message into a buffer.  If status isn't NULL,
**  stuff it with -1 on failure, else 0.
*/
void
dce_error_inq_text(
    error_status_t		status_to_convert,
    unsigned char		*error_text,
    int				*status
)
{
    unsigned char		*p;
    error_status_t		st;
    dce_msg_inqbuf_t		comp_name;
    dce_msg_inqbuf_t		tech_name;

    /* Default return status; pessimistically. */
    if (status)
	*status = -1;
    
    /* Map "OK" to a messages that says "Success".  (Message catalogs
     * can not have a message numbered 0). */
    if (status_to_convert == 0)
	status_to_convert = msg_s_ok_text;

    if (IS_ISV_MSG(status_to_convert)) {
	dce__error_inq_isv(status_to_convert, error_text, status);
	return;
    }

    dce_msg__inq_tech(status_to_convert, tech_name);
    dce_msg__inq_comp(status_to_convert, comp_name);

    if ((p = dce_msg_get_msg(status_to_convert, &st)) == NULL
     || st != msg_s_ok) {
	if (st == msg_s_bad_id)
	    (void)sprintf((char *)error_text,
		    "status 0x%8.8lx (unknown facility)",
		    (long)status_to_convert);
	else
	    (void)sprintf((char *)error_text, "status 0x%8.8lx (%s / %s)",
		    (long)status_to_convert, tech_name, comp_name);
	if (p)
	    free(p);
	return;
    }
    (void)sprintf((char *)error_text, "%s (%s / %s)",
	    p, tech_name, comp_name);
    free(p);

    if (status)
	*status = 0;
}


/*
**  Get the DCE component from a Message ID.
*/
void
dce_msg__inq_comp(
    unsigned32          code,
    dce_msg_inqbuf_t	comp_name
)
{
    static char		alphabet[] = RAD40ALPHABET;

    code = (code & COMP_MASK) >> COMP_SHIFT;
    comp_name[3] = '\0';
    comp_name[2] = alphabet[code % 40];
    code /= 40;
    comp_name[1] = alphabet[code % 40];
    comp_name[0] = alphabet[code / 40];
}


/*
**  Get the DCE technology from a Message ID.
*/
void
dce_msg__inq_tech(
    unsigned32		code,
    dce_msg_inqbuf_t	tech_name
)
{
    code &= TECH_MASK;
    if (code == TECH_MASK_DCE)
    	strcpy(tech_name, "dce");
    else if (code == TECH_MASK_DFS)
    	strcpy(tech_name, "dfs");
    else
    	strcpy(tech_name, "???");
}
