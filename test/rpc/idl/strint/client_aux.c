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
 * Revision 1.1.4.2  1996/02/17  23:17:47  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:51  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:11  root]
 * 
 * Revision 1.1.2.1  1994/05/03  18:50:58  tom
 * 	Initial checkin.
 * 	[1994/05/03  18:49:52  tom]
 * 
 * $EndLog$
 */

#include "test_common.h"

#include "strint.h"


#define I_SIZE 31
#define IO_SIZE 36
#define O_SIZE 41

extern void init16(PRIVATE_CHAR_16 a[], idl_char *str);

/*
 * Compares a string of 'unsigned short' base type with data from a
 * character string.
 */
int cmp16(PRIVATE_CHAR_16 a[], idl_char *str)
{
    int i;
    idl_usmall_int b0,b1;
    for (i = 0; i < 81; i++)
    {
        if (str[i] == '\0')
            break;
        if ( (a[i]%256) != str[i] )
            return 1;
        if ( (a[i]/256) != (str[i]+1) )
            return 1;
    }
    if (a[i] != 0)
        return 1;
    return 0;
}

/*
 * Initialize a string of 'unsigned long' base type with data from a
 * character string.
 */
void init32(PRIVATE_CHAR_32 a[], idl_char *str)
{
    int i;
    idl_usmall_int b0,b1,b2,b3;
    for (i = 0; i < 81; i++)
    {
        if (str[i] == '\0')
            break;
        b0 = str[i];
        b1 = str[i]+1;
        b2 = str[i]+2;
        b3 = str[i]+3;
        a[i] = (b3<<(3*8)) + (b2<<(2*8)) + (b1<<(1*8)) + b0;
    }
    a[i] = 0;
}

/*
 * Compares a string of 'unsigned short' base type with data from a
 * character string.
 */
int cmp32(PRIVATE_CHAR_32 a[], idl_char *str)
{
    int i;
    idl_usmall_int b0,b1,b2,b3;
    for (i = 0; i < 81; i++)
    {
        if (str[i] == '\0')
            break;
        if ( (a[i]%256) != str[i] )
            return 1;
        if ( ((a[i]>>8)%256) != (str[i]+1) )
            return 1;
        if ( ((a[i]>>16)%256) != (str[i]+2) )
            return 1;
        if ( (a[i]>>24) != (str[i]+3) )
            return 1;
    }
    if (a[i] != 0)
        return 1;
    return 0;
}

static idl_char R_f_1[81] = "Clunton and Clunbury,";
static idl_char R_f_2[81] = "Clungunford and Clun";
static PRIVATE_CHAR_16 f_i[81];
static PRIVATE_CHAR_16 f_io[81];
static PRIVATE_CHAR_16 f_o[81];
static idl_byte R_c_1[I_SIZE] = "Are the quietest places";
static idl_byte R_c_2[IO_SIZE] = "Under the sun.";
static PRIVATE_CHAR_32 c_i[I_SIZE];
static PRIVATE_CHAR_32 c_io[IO_SIZE];
static PRIVATE_CHAR_32 c_o[O_SIZE];
static ISO_LATIN_1 R_s_1[] = "Morituri te salutamus";
static ISO_LATIN_1 R_s_2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static PRIVATE_CHAR_16 s_i[81];
static PRIVATE_CHAR_16 s_io[81];
static PRIVATE_CHAR_32 m_i[14];
static PRIVATE_CHAR_32 m_o[14];
static idl_char scs_in[] = "abcdefghijklmnopqrstuvwxyz";

int client_aux(h,passes)
    int passes;   handle_t h;
{
    error_status_t st;
    int i,j;
    private_16 *p_p16;
    two_byte *p_tb;
    stringptrs_t sp;
    ils_t ilsa[5];
    ims_t imsa[5];
    ius_t iusa[5];
    PRIVATE_CHAR_16 scs[27];


    init16( f_i, R_f_1 );
    init16( f_io, R_f_2 );
    op_fixed( h, f_i, f_io, f_o );
    if ( cmp16(f_io, R_f_1) != 0 )
    {
        printf("op_fixed - strcmp(f_io, R_f_1) failed\n");
        return(1);
    }
    if ( cmp16(f_o, R_f_2) != 0 )
    {
        printf("op_fixed - strcmp(f_o, R_f_2) failed\n");
        return(1);
    }

    init32( c_i, R_c_1 );
    init32( c_io, R_c_2 );
    op_conf( h, I_SIZE, c_i, IO_SIZE, c_io, O_SIZE, c_o );
    if ( cmp32(c_io, R_c_1) != 0 )
    {
        printf("op_conf - strcmp(c_io, R_c_1) failed\n");
        return(1);
    }
    if ( cmp32(c_o, R_c_2) != 0 )
    {
        printf("op_conf - strcmp(c_o, R_c_2) failed\n");
        return(1);
    }
                        
    init16( s_i, R_s_1 );
    init16( s_io, R_s_2 );
    op_self( h, s_i, s_io );
    if ( cmp16(s_io, R_s_1) != 0 )
    {
        printf("op_self - strcmp(s_io, R_s_1) failed\n");
        return(1);
    }

    init32(m_i, (idl_char*)"abcdefghijklm");
    p_p16 = (private_16 *)malloc(sizeof(private_16)
                                 + 26 * sizeof(PRIVATE_CHAR_16));
    init16(p_p16->f_p_16, (idl_char*)"The rainy Pleiads wester");
    init16(p_p16->s_p, (idl_char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    p_tb = (two_byte *)malloc(sizeof(two_byte) + 26 * sizeof(PRIVATE_CHAR_32));
    init32(p_tb->m_l, (idl_char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    init32(p_tb->p_32, (idl_char*)"abcdefghijklmnopqrstuvwxyz");
    op_multi( h, m_i, m_o, p_p16, p_tb);
    if (cmp32(m_o, (idl_char*)"nopqrstuvwxyz") != 0)
    {
        printf("op_multi - strcmp(m_o, ....) failed\n");
        return(1);
    }
    if (cmp16(p_p16->f_p_16, (idl_char*)"The rainy Pleiads wester") != 0 )
    {
        printf("op_multi - strcmp(p_p16->f_p_16, ...) failed\n");
        return(1);
    }
    if (cmp16(p_p16->s_p, (idl_char*)"Orion plunges prone") != 0)
    {
        printf("op_multi - strcmp(p_p16->s_p, ...) failed\n");
        return(1);
    }
    if (cmp32(p_tb->m_l, (idl_char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ") != 0)
    {
        printf("op_multi - strcmp(p_tb->m_l, ...) failed\n");
        return(1);
    }
    if (cmp32(p_tb->p_32, (idl_char*)"ABCDEFGHIJKLMNOPQRSTUVWXYZ") != 0)
    {
        printf("op_multi - strcmp(p_tb->p_32, ...) failed\n");
        return(1);
    }

    init16(s_i, (idl_char *)"abcdefghijklmnopqrstuvwxyz");
    sp.astring = s_i;
    sp.bstring = NULL;
    op_sp(h, &sp);
    if (sp.astring != NULL)
    {
        printf("op_sp - sp.a_string incorrect\n");
        return(1);
    }
    if (cmp16(sp.bstring, R_s_2) != 0)
    {
        printf("op_sp - sp.b_string incorrect\n");
        return(1);
    }

    for (i=0; i<5; i++)
        init16(ilsa[i], &R_s_2[i]);
    op_fas(h, ilsa, imsa);
    for (i=0; i<5; i++)
    {
        if (cmp32(imsa[i], &R_s_2[i]) != 0)
        {
            printf("op_fas - error at imsa[%d]\n", i);
            return(1);
        }
    }

    op_fac(h, 5, imsa, iusa);
    for (i=0; i<5; i++)
    {
        if (cmp32(iusa[i], &scs_in[i]) != 0)
        {
            printf("op_fas - error at iusa[%d]\n", i);
            return(1);
        }
    }

    init16(scs, scs_in);
    op_selfconf(h, scs);
    if (cmp16(scs, R_s_2) != 0)
    {
        printf("op_selfconf - scs = %s\n", scs);
        return(1);
    }

    return(0);
}
