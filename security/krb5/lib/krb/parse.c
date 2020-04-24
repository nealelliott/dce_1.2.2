/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: parse.c,v $
 * Revision 1.1.6.2  1996/02/18  00:14:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:55  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:00:35  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/1  1994/05/20  21:07 UTC  mob
 * 	Vista workaround - add extra () to prevent operator precedence error
 * 
 * 	HP revision /main/mob_rodan2/1  1994/05/20  21:06 UTC  mob
 * 	Vista workaround - add extra () to prevent operator precedence error
 * 	[1995/12/08  17:00:02  root]
 * 
 * Revision 1.1.2.3  1993/02/03  22:26:58  sommerfeld
 * 	[OT6641] avoid dying if presented with two REALM-SEP's.
 * 	[1993/01/29  18:45:17  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/12/29  15:57:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:15:08  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  parse.c V=3 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/parse.c,v $
 * $Author: marty $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * krb5_parse_name() routine.
 *
 * Rewritten by Theodore Ts'o to properly handle arbitrary quoted
 * characters in the principal name.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_parse_c [] =
"$Id: parse.c,v 1.1.6.2 1996/02/18 00:14:22 marty Exp $";
#endif	/* !lint & !SABER */


#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>

/*
 * converts a single-string representation of the name to the
 * multi-part principal format used in the protocols.
 *
 * principal will point to allocated storage which should be freed by 
 * the caller (using krb5_free_principal) after use.
 * 
 * Conventions:  / is used to separate components.  If @ is present in the
 * string, then the rest of the string after it represents the realm name.
 * Otherwise the local realm name is used.
 * 
 * returns system errors XXX
 */

#define REALM_SEP	'@'
#define	COMPONENT_SEP	'/'
#define QUOTECHAR	'\\'
#define	MAXRLMSZ	256		/* XXX! */

#define FCOMPNUM	2


/*
 * May the fleas of a thousand camels infest the ISO, they who think
 * that arbitrarily large multi-component names are a Good Thing.....
 */
krb5_error_code
krb5_parse_name(name, nprincipal)
	const char	*name;
	krb5_principal	*nprincipal;
{
	register const char	*cp;
	register char	*q;
	register i,c,size;
	int		components = 0;
	const char	*parsed_realm = NULL;
	int		fcompsize[FCOMPNUM];
	static char	*default_realm = NULL;
        /*
         * default realm can be a static pointer even in a multi-threaded
         * environment if we assume an atomic write to the pointer - the
         * worst that will happen is that we will waste a little memory
         * when multiple threads all determine the default realm hasn't 
         * been set yet and go off and ask for the value.
         */
	krb5_data	**principal;
	krb5_error_code retval;
	
	/*
	 * Pass 1.  Find out how many components there are to the name,
	 * and get string sizes for the first FCOMPNUM components.
	 */
	size = 0;
	for (i=1,cp = name; (c = *cp); cp++) {
		if (c == QUOTECHAR) {
			cp++;
			if (!(c = *cp))
				/*
				 * QUOTECHAR can't be at the last
				 * character of the name!
				 */
				return(KRB5_PARSE_MALFORMED);
			size++;
			continue;
		} else if (c == COMPONENT_SEP) {
			if (parsed_realm) {
                            /*
                             * Treat a multi-component realm as a
                             * single component
                             */        
                            size++;
			} else {
                            if (i < FCOMPNUM) {
				fcompsize[i] = size;
                            }
                            size = 0;
                            i++;
                        }
		} else if (c == REALM_SEP) {
			if (!*(cp+1)) 
				/*
				 * Null Realm names are not allowed!
				 */
				return(KRB5_PARSE_MALFORMED);
			if (parsed_realm != NULL) 
			{
			    /* Two realm names are not allowed! */
			    return KRB5_PARSE_MALFORMED;
			}
			parsed_realm = cp+1;
			if (i < FCOMPNUM) {
				fcompsize[i] = size;
			}
			size = 0;
		} else
			size++;
	}
	if (parsed_realm)
		fcompsize[0] = size;
	else if (i < FCOMPNUM) 
		fcompsize[i] = size;
	components = i;
	/*
	 * Now, we allocate the principal structure and all of its
	 * component pieces
	 */
	principal = (krb5_principal)
		malloc(sizeof(krb5_data *) * (components+2));
	if (!principal) {
		return(ENOMEM);
	}
	for (i = 0; i <= components; i++) {
		if (!(principal[i] =
		      (krb5_data *) malloc(sizeof(krb5_data)))) {
			for (i--; i >= 0; i--) 
				xfree(principal[i]);
			xfree(principal);
			return (ENOMEM);
		}
	}
	principal[components+1] = NULL;
	/*
	 * If a realm was not found, then we need to find the defualt
	 * realm....
         *
         * Obtaining the default realm should really be protected under
         * a mutex lock (or pthread_once) in a multi-threaded environment
         * but as observed above, if we assume atomic writes to the pointer
         * we should not run into problems.
	 */
	if (!parsed_realm) {
		if (!default_realm &&
		    (retval = krb5_get_default_realm(&default_realm)))
			return(retval);
		principal[0]->length = fcompsize[0] = strlen(default_realm);
	}
	/*
	 * Pass 2.  Happens only if there were more than FCOMPNUM
	 * component; if this happens, someone should be shot
	 * immediately.  Nevertheless, we will attempt to handle said
	 * case..... <martyred sigh>
	 */
	if (components >= FCOMPNUM) {
		size = 0;
		parsed_realm = NULL;
		for (i=1,cp = name; (c = *cp); cp++) {
			if (c == QUOTECHAR) {
				cp++;
				size++;
			} else if (c == COMPONENT_SEP) {
                                if (parsed_realm) {
                                    size++;
                                } else {
                                    principal[i]->length = size;
                                    size = 0;
                                    i++;
                                }
			} else if (c == REALM_SEP) {
				principal[i]->length = size;
				size = 0;
				parsed_realm = cp+1;
			} else
				size++;
		}
		if (parsed_realm)
			principal[0]->length = size;
		else
			principal[i]->length = size;
		if (i != components) {
			fprintf(stderr,
				"Programming error in krb5_parse_name!");
			exit(1);
		}
	} else {
		/*
		 * If there were fewer than FCOMPSIZE components (the
		 * usual case), then just copy the sizes to the
		 * principal structure
		 */
		for (i=0; i <= components; i++)
			principal[i]->length = fcompsize[i];
	}
	/*	
	 * Now, we need to allocate the space for the strings themselves.....
	 */
	for (i=0; i <= components; i++) {
		if (!(principal[i]->data = malloc(principal[i]->length + 1))) {
			for (i--; i >= 0; i--)
				xfree(principal[i]->data);
			for (i=0; i <= components; i++)
				xfree(principal[i]);
			xfree(principal);
			return(ENOMEM);
		}
	}
	
	/*
	 * Pass 3.  Now we go through the string a *third* time, this
	 * time filling in the krb5_principal structure which we just
	 * allocated.
	 */
	parsed_realm = NULL;
	q = principal[1]->data;
	for (i=1,cp = name; (c = *cp); cp++) {
		if (c == QUOTECHAR) {
			cp++;
			switch (c = *cp) {
			case 'n':
				*q++ = '\n';
				break;
			case 't':
				*q++ = '\t';
				break;
			case 'b':
				*q++ = '\b';
				break;
			case '0':
				*q++ = '\0';
				break;
			default:
				*q++ = c;
			}
		} else if (c == COMPONENT_SEP) {  
                        if (parsed_realm) {
                            *q++ = c;
                        } else {
			    i++;
			    *q++ = '\0';
			    q = principal[i]->data;
                        }
                } else if(c == REALM_SEP) {
			parsed_realm = cp+1;  
                        i++;
			*q++ = '\0';
			q = principal[0]->data;    
		} else
			*q++ = c;
	}
	*q++ = '\0';
	if (!parsed_realm)
		strcpy(principal[0]->data, default_realm);
	/*
	 * Alright, we're done.  Now stuff a pointer to this monstrosity
	 * into the return variable, and let's get out of here.
	 */
	*nprincipal = principal;
	return(0);
}


