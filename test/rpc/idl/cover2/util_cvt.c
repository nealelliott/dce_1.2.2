/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_cvt.c,v $
 * Revision 1.1.6.2  1996/02/17  23:13:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:17  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:01:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:49  root]
 * 
 * Revision 1.1.4.2  1993/01/07  19:51:19  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:31:01  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/26  19:04:58  harrow
 * 	Remove test for [represent_as] on a conformant struct as it is
 * 	unsupported.
 * 	[1992/05/26  18:57:25  harrow]
 * 
 * Revision 1.1  1992/01/19  03:31:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <cover2.h>
#if CONF_REP_AS
void pastring_from_local( p_c_string, p_p_pastring )
c_string *p_c_string;
pastring **p_p_pastring;
{
    pastring *p_pastring;
    int length;

    length = strlen(p_c_string);
    p_pastring = (pastring *)malloc( sizeof(pastring) + length - 1 );
    p_pastring->char_count = length;
    memcpy( p_pastring->chars, p_c_string, length );
    *p_p_pastring = p_pastring;
}

void pastring_to_local( p_pastring, p_c_string )
pastring *p_pastring;
c_string *p_c_string;
{
    memcpy( p_c_string, p_pastring->chars, p_pastring->char_count );
    (*p_c_string)[p_pastring->char_count] = '\0';
}

void pastring_free_inst( p_pastring )
pastring *p_pastring;
{
    free( p_pastring );
}

void pastring_free_local( p_c_string )
c_string *p_c_string;
{
#ifdef DEBUG
    if (false) cma_debug();
    printf ( "pastring_free_local of %s\n", p_c_string );
#endif
}
#endif
