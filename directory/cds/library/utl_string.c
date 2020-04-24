/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: utl_string.c,v $
 * Revision 1.1.6.2  1996/02/18  19:35:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:23  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:27:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:45  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:18:02  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:05:13  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:37  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:52  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:56:01  peckham
 * 	Remove unnecessary conditionals.
 * 	[1994/04/29  14:35:55  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:09:20  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:36:28  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:17:57  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:18:10  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module utl_string.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
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
 *
 *
 * Module Description
 *
 * This module provides additional string functions:
 *
 *     dns_strdup - duplicates a string into an allocated area
 *     strelement - extracts an element from a list of elements
 *
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <pthread.h>
#include <string.h>
#include <utl_string.h>

/*
 * ROUTINE - dns_strdup
 *
 * Replicates the input string into a dynamically allocated area and
 * returns the address of this new string (or NULL if there was insufficient
 * memory to malloc).
 */

char *
dns_strdup (char *in_string)
{
    char *copy_of;

    copy_of = (char *) malloc(strlen(in_string)+1);
    if (copy_of != NULL) {
        strcpy (copy_of, in_string);
    }
    return (copy_of);
}


/*
 * ROUTINE - strelement
 *
 * A C emulation of the F$Element function in DCL: extracts one element
 * from a string of elements.
 *
 * 
 */

char *
strelement (char  *result_string,
             int   element,
             char  *delimiter,
             char  *element_list)
{
    int x;                /* work variable */
    char *result_in;      /* remembers where s pointed to when we were called */

    result_in = result_string;             /* See? */

    /*
     * The outer for loop is counting the elements we find, and the
     * inner while loop is skipping over the characters in each element.
     * The element is delimited by the delimiter character or end of string.
     */

    for (x = 0; x < element; x++) {
        while (!((*element_list == '\0') || (*element_list == *delimiter))) {
            element_list++;
        }
        if (*element_list == '\0') {
            break;
        }
        element_list++;
    }

    /*
     * If we ended up at the end of the input string, then the caller
     * asked for an element beyond the list supplied.  When this happens
     * we return the delimiter.
     */
    if (*element_list == '\0') {
        return (strcpy(result_string, delimiter));
    }

    /*
     * element_list is positioned at the beginning of the element we want to
     * extract.  We copy the element to s (the output string) until
     * we find a delimiter or end of string.
     */
    while (!((*element_list == '\0') || (*element_list == *delimiter))) {
            *result_string++ = *element_list++;
    }

    /* Add a null terminator to the output string. */
    *result_string = '\0';

    /* Return the output string */
    return (result_in);

}

