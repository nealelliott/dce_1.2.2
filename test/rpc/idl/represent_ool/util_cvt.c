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
 * Revision 1.1.4.2  1996/02/17  23:17:12  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:25  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:29:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:39  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:35:32  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:43:01  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <represent_ool.h>

void IN_MOCS_from_local( p_jis_kanji, p_p_in_mocs )
JIS_KANJI *p_jis_kanji;
IN_MOCS **p_p_in_mocs;
{
    IN_MOCS *p_in_mocs;

    p_in_mocs = (IN_MOCS *)malloc(sizeof(IN_MOCS));
    p_in_mocs->group = p_jis_kanji->high_byte;
    p_in_mocs->plane = 0;
    p_in_mocs->row = 0;
    p_in_mocs->column = p_jis_kanji->low_byte;
    *p_p_in_mocs = p_in_mocs;
}

void IN_MOCS_to_local( p_in_mocs, p_jis_kanji )
IN_MOCS *p_in_mocs;
JIS_KANJI *p_jis_kanji;
{
    p_jis_kanji->high_byte = p_in_mocs->group;
    p_jis_kanji->low_byte = p_in_mocs->column;
}

void IN_MOCS_free_inst( p_in_mocs )
IN_MOCS *p_in_mocs;
{
    free( p_in_mocs );
}

void IN_MOCS_free_local( p_jis_kanji )
JIS_KANJI *p_jis_kanji;
{
    printf ( "IN_MOCS_free_local of %c%c\n", 
                p_jis_kanji->high_byte, p_jis_kanji->low_byte );
}

void OUT_MOCS_from_local( p_ibm_kanji, p_p_out_mocs )
IBM_KANJI *p_ibm_kanji;
OUT_MOCS **p_p_out_mocs;
{
    OUT_MOCS *p_out_mocs;

    p_out_mocs = (OUT_MOCS *)malloc(sizeof(OUT_MOCS));
    p_out_mocs->group = 0;
    p_out_mocs->plane = p_ibm_kanji->high_byte;
    p_out_mocs->row = p_ibm_kanji->low_byte;
    p_out_mocs->column = 0;
    *p_p_out_mocs = p_out_mocs;
}

void OUT_MOCS_to_local( p_out_mocs, p_ibm_kanji )
OUT_MOCS *p_out_mocs;
IBM_KANJI *p_ibm_kanji;
{
    p_ibm_kanji->high_byte = p_out_mocs->plane;
    p_ibm_kanji->low_byte = p_out_mocs->row;
}

void OUT_MOCS_free_inst( p_out_mocs )
OUT_MOCS *p_out_mocs;
{
    free( p_out_mocs );
}

void OUT_MOCS_free_local( p_ibm_kanji )
IBM_KANJI *p_ibm_kanji;
{
    printf ( "OUT_MOCS_free_local of %c%c\n", 
                p_ibm_kanji->high_byte, p_ibm_kanji->low_byte );
}
