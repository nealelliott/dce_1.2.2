/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/*
 * HISTORY
 * $Log: msginqcma.c,v $
 * Revision 1.1.4.2  1996/02/18  19:22:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:39  root]
 * 
 * Revision 1.1.2.2  1994/09/08  13:16:45  bowe
 * 	#define NL_CAT_LOCALE if not already defined (affects non-osf1).
 * 	Added some proper error checking/return missed earlier.  [CR 12098]
 * 	[1994/09/08  13:16:33  bowe]
 * 
 * Revision 1.1.2.1  1994/09/07  21:05:02  bowe
 * 	Initial implementation of dce_cma_error_inq_text(). [CR 11885]
 * 	[1994/09/07  21:03:37  bowe]
 * 
 * $EndLog$
 */

/*
 *  A non-threaded version of dce_error_inq_text() - called from
 *  threads library code.
 */

#define NO_INCLUDE_PTHREAD
#include <msgimpl.h>
#include <dce/dcethdmsg.h>

#if !defined(NL_CAT_LOCALE)
#define NL_CAT_LOCALE   0
#endif	/* !defined(NL_CAT_LOCALE) */

#define TECH_MASK_DCE	0x10000000u
#define COMP_MASK_THD	(exc_s_exception & COMP_MASK)

/*
**  Copy the text of a status message into a buffer.  If status isn't NULL,
**  stuff it with -1 on failure, else 0.
*/
void
dce_cma_error_inq_text(
    error_status_t		status_to_convert,
    unsigned char		*error_text,
    int				*status
)
{
    unsigned char		*p;
    nl_catd			cd;
    char			buff[SMBUF];

    /* Default return status; pessimistically. */
    if (status)
	*status = -1;
    p = NULL;
    cd = (nl_catd)-1;

    /* Must be DCE and threads component.  If not, return error. */
    if ( ((status_to_convert & TECH_MASK) == TECH_MASK_DCE) &&
	 ((status_to_convert & COMP_MASK) == COMP_MASK_THD) ) {

	/* Try message catalog, then in-core table, then punt. */
	(void)strcpy(buff, "dcethd" CATALOG_SUFFIX);
	if ((cd = catopen(buff, NL_CAT_LOCALE)) != (nl_catd)-1) {
	    static char NIL[]="?NOPE?";
	    p = (unsigned char *)catgets(cd, 1,
			(int)(status_to_convert & CODE_MASK), NIL);
	    if (strcmp((char *)p, NIL) == 0)
		p = NULL;
	}
	if (p == NULL) {	/* No catalog entry - try in-core table */
	    dce_msg_table_t	*mp;
	    unsigned32		i;
	    for (mp = thd_msg_table,
			i = sizeof thd_msg_table / sizeof thd_msg_table[0];
			i >= 1; i--, mp++)
		if (mp->message == status_to_convert) {
		    p = (unsigned char *)mp->text;
		    break;
		}
	}
    }

    if (p == NULL) {
	(void)sprintf((char *)error_text,
	    "Internal CMA error: unknown message number 0x%8.8lx",
	    (long)status_to_convert);
    }
    else {
	(void)strcpy((char *)error_text, (char *)p);
	(void)strcat((char *)error_text, " (dce / thd)");
	if (status)
	    *status = 0;
    }

    if (cd != (nl_catd)-1)
	catclose(cd);

}

