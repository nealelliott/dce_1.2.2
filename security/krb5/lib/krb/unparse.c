/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: unparse.c,v $
 * Revision 1.1.6.2  1996/02/18  00:14:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:18  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:02:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:16  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:59:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:16:07  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  unparse.c V=4 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/unparse.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * krb5_unparse_name() routine
 *
 * Rewritten by Theodore Ts'o to propoerly unparse principal names
 * which have the component or realm separator as part of one of their
 * components.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_unparse_c[] =
"$Id: unparse.c,v 1.1.6.2 1996/02/18 00:14:52 marty Exp $";
#endif	/* !lint & !SABER */

#include <stdio.h>
#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

/*
 * converts the multi-part principal format used in the protocols to a
 * single-string representation of the name. 
 *  
 * The name returned is in allocated storage and should be freed by
 * the caller when finished.
 *
 * Conventions: / is used to separate components; @ is used to
 * separate the realm from the rest of the name.  If '/', '@', or '\0'
 * appear in any the component, they will be representing using
 * backslash encoding.  ("\/", "\@", or '\0', respectively)
 *
 * returns error
 *	KRB_PARSE_MALFORMED	principal is invalid (does not contain
 *				at least 2 components)
 * also returns system errors
 *	ENOMEM			unable to allocate memory for string
 */

#define REALM_SEP	'@'
#define	COMPONENT_SEP	'/'

krb5_error_code
krb5_unparse_name_ext(principal, name, size)
krb5_const_principal principal;
register char **name;
int	*size;
{
	register char *cp, *q;
	register int i,j;
	register int totalsize = 0;
	int	length;

	if (!principal[0] || !principal[1])
		return KRB5_PARSE_MALFORMED;
	for (i = 0; principal[i]; i++) {
		cp = principal[i]->data;
		length = principal[i]->length;
		for (j=0; j < length; j++,cp++)
			if (*cp == REALM_SEP || *cp == COMPONENT_SEP ||
			    *cp == '\0' || *cp == '\\' || *cp == '\t')
				totalsize += 2;
			else
				totalsize++;
		totalsize++;	/* This is for the separator */
	}

	/*
	 *  we need only n-1 seps for n components, but we need an
	 * extra byte for the NULL at the end
	 */
	if (*name) {
		if (*size < (totalsize)) {
			*size = totalsize;
			*name = realloc(*name, totalsize);
		}
	} else {
		*name = malloc(totalsize);	/* room for null */
		if (size)
			*size = totalsize;
	}
	
	if (!*name)
		return ENOMEM;

	q = *name;
	
	for (i = 1; principal[i]; i++) {
		cp = principal[i]->data;
		length = principal[i]->length;
		for (j=0; j < length; j++,cp++) {
			switch (*cp) {
			case COMPONENT_SEP:
			case REALM_SEP:
			case '\t':
			case '\\':
				*q++ = '\\';
				*q++ = *cp;
				break;
			case '\0':
				*q++ = '\\';
				*q++ = '0';
				break;
			default:
				*q++ = *cp;
			}
		}
		*q++ = COMPONENT_SEP;
	}

	q--;			/* Back up last component separator */
	*q++ = REALM_SEP;
	
	cp = principal[0]->data;
	length = principal[0]->length;
	for (j=0; j < length; j++,cp++) {
		switch (*cp) {
		case COMPONENT_SEP:
		case REALM_SEP:
		case '\t':
		case '\\':
			*q++ = '\\';
			*q++ = *cp;
			break;
		case '\0':
			*q++ = '\\';
			*q++ = '0';
			break;
		default:
			*q++ = *cp;
		}
	}
	*q++ = '\0';
	
    return 0;
}

krb5_error_code
krb5_unparse_name(principal, name)
krb5_const_principal principal;
register char **name;
{
	*name = NULL;
	return(krb5_unparse_name_ext(principal, name, NULL));
}


