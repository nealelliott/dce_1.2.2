/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: error_message.c,v $
 * Revision 1.1.2.1  1996/06/05  21:23:09  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:13:12  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * util/et/error_message.c
 *
 * Copyright 1987 by the Student Information Processing Board
 * of the Massachusetts Institute of Technology
 *
 * For copyright info, see "mit-sipb-copyright.h".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error_table.h"
#include "mit-sipb-copyright.h"
#include "internal.h"

static const char copyright[] =
    "Copyright 1986, 1987, 1988 by the Student Information Processing Board\nand the department of Information Systems\nof the Massachusetts Institute of Technology";

static char buffer[25];

struct et_list * _et_list = (struct et_list *) NULL;

const char * INTERFACE error_message (code)
long code;
{
    int offset;
    long l_offset;
    struct et_list *et;
    long table_num;
    int started = 0;
    char *cp;

    l_offset = code & ((1<<ERRCODE_RANGE)-1);
    offset = (int) l_offset;
    table_num = code - l_offset;
    if (!table_num) {
#ifdef HAS_STRERROR
	return strerror (offset);
#else
        if (offset < sys_nerr)
	    return(sys_errlist[offset]);
	else
	    goto oops;
#endif
    }
    for (et = _et_list; et; et = et->next) {
	if (et->table->base == table_num) {
	    /* This is the right table */
	    if (et->table->n_msgs <= offset)
		goto oops;
	    return(et->table->msgs[offset]);
	}
    }
oops:
    strcpy (buffer, "Unknown code ");
    if (table_num) {
	strcat (buffer, error_table_name (table_num));
	strcat (buffer, " ");
    }
    for (cp = buffer; *cp; cp++)
	;
    if (offset >= 100) {
	*cp++ = '0' + offset / 100;
	offset %= 100;
	started++;
    }
    if (started || offset >= 10) {
	*cp++ = '0' + offset / 10;
	offset %= 10;
    }
    *cp++ = '0' + offset;
    *cp = '\0';
    return(buffer);
}
