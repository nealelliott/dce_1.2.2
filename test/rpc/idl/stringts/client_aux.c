/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.4.2  1996/02/17  23:17:40  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:45  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:04  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:38:49  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:45:20  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <stringts.h>
#include <test_common.h>

#define I_SIZE 31
#define IO_SIZE 36
#define O_SIZE 41
static ndr_char R_f_1[81] = "Clunton and Clunbury,";
static ndr_char R_f_2[81] = "Clungunford and Clun";
static ndr_char f_i[81];
static ndr_char f_io[81];
static ndr_char f_o[81];
static ndr_byte R_c_1[I_SIZE] = "Are the quietest places";
static ndr_byte R_c_2[IO_SIZE] = "Under the sun.";
static ndr_byte c_i[I_SIZE];
static ndr_byte c_io[IO_SIZE];
static ndr_byte c_o[O_SIZE];
static ISO_LATIN_1 R_s_1[] = "Morituri te salutamus";
static ISO_LATIN_1 R_s_2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static ISO_LATIN_1 s_i[] = "Morituri te salutamus";
static ISO_LATIN_1 s_io[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static ISO_UCS m_i[14];
static ISO_UCS m_o[14];

int client_aux(h)
   handle_t h;
{
	error_status_t st;
    int i;
    private_8 *p_p8;
    two_byte *p_tb;

    strcpy( f_i, R_f_1 );
    strcpy( f_io, R_f_2 );
    op_fixed( h, f_i, f_io, f_o );
    if ( strcmp(f_io, R_f_1) != 0 ) return(1);
    if ( strcmp(f_o, R_f_2) != 0 ) return(1);

    strcpy( c_i, R_c_1 );
    strcpy( c_io, R_c_2 );
    op_conf( h, I_SIZE, c_i, IO_SIZE, c_io, O_SIZE, c_o );
    if ( strcmp(c_io, R_c_1) != 0 ) return(1);
    if ( strcmp(c_o, R_c_2) != 0 ) return(1);
                        
    strcpy( s_i, R_s_1 );
    strcpy( s_io, R_s_2 );
    op_self( h, s_i, s_io );
    if ( strcmp(s_io, R_s_1) != 0 ) return(1);

    for(i=0; i<13; i++)
    {
        m_i[i].group = 'A' + i;
        m_i[i].plane = 'a' + i;
        m_i[i].row = 'a' + i;
        m_i[i].column = 'A' + i;
    }
    m_i[13].group = m_i[13].plane = m_i[13].row = m_i[13].column = 0;
    p_p8 = (private_8 *)malloc(sizeof(private_8) + 26 * sizeof(PRIVATE_CHAR_8));
    strcpy(p_p8->f_p_8, "The rainy Pleiads wester");
    strcpy(p_p8->s_p, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    p_tb = (two_byte *)malloc(sizeof(two_byte) + 26 * sizeof(PRIVATE_CHAR_16));
    for(i=0; i<26; i++)
    {
        p_tb->m_l[i].row = 'A' + i;
        p_tb->m_l[i].column = 'a' + i;
        p_tb->p_16[i].row = 'A' + i;
        p_tb->p_16[i].column = 'a' + i;
    }
    p_tb->m_l[26].row = p_tb->m_l[26].column 
       = p_tb->p_16[26].row = p_tb->p_16[26].column = 0;
    op_multi( h, m_i, m_o, p_p8, p_tb);
    if ( strcmp( (char *)m_o, 
               "NnNnOoOoPpPpQqQqRrRrSsSsTtTtUuUuVvVvWwWwXxXxYyYyZzZz") != 0 )
                                                                      return(1);
    if ( strcmp(p_p8->f_p_8, "The rainy Pleiads wester") != 0 ) return(1);
    if ( strcmp(p_p8->s_p , "Orion plunges prone") != 0 ) return(1);
    if ( strcmp((char *)p_tb->m_l,
               "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz") != 0 )
                                                                      return(1);
    if ( strcmp((char *)p_tb->p_16,
               "ZzYyXxWwVvUuTtSsRrQqPpOoNnMmLlKkJjIiHhGgFfEeDdCcBbAa") != 0 )
                                                                      return(1);

    return(0);
}
