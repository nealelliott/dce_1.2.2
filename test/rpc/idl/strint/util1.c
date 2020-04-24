/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util1.c,v $
 * Revision 1.1.4.2  1996/02/17  23:17:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:54  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:13  root]
 * 
 * Revision 1.1.2.1  1994/05/03  18:51:05  tom
 * 	Initial checkin.
 * 	[1994/05/03  18:50:02  tom]
 * 
 * $EndLog$
 */

#include "strint.h"

/*
 * Initialize a string of 'unsigned short' base type with data from a
 * character string.
 */
void init16(PRIVATE_CHAR_16 a[], idl_char *str)
{
    int i;
    idl_usmall_int b0,b1;
    for (i = 0; i < 81; i++)
    {
        if (str[i] == '\0')
            break;
        b0 = str[i];
        b1 = str[i]+1;
        a[i] = (b1<<(1*8)) + b0;
    }
    a[i] = 0;
}
