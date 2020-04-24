/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:17:49  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:53  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:12  root]
 * 
 * Revision 1.1.2.1  1994/05/03  18:51:00  tom
 * 	Initial checkin.
 * 	[1994/05/03  18:49:55  tom]
 * 
 * $EndLog$
 */
#include "test_common.h"

#include "strint.h"


extern void init16(PRIVATE_CHAR_16 a[], idl_char *str);

int cpy16(PRIVATE_CHAR_16 *dst, PRIVATE_CHAR_16 *src)
{
    while (*src != 0)
        *dst++ = *src++;
    *dst = 0;
    return 0;
}

int cpy32(PRIVATE_CHAR_32 *dst, PRIVATE_CHAR_32 *src)
{
    while (*src != 0)
        *dst++ = *src++;
    *dst = 0;
    return 0;
}

void his_op_fixed(h, f_i, f_io, f_o)
      /* [in] */ handle_t h;
      /* [in] */ PRIVATE_CHAR_16 f_i[81];
      /* [in, out] */ PRIVATE_CHAR_16 f_io[81];
      /* [out] */ PRIVATE_CHAR_16 f_o[81];
{
    cpy16(f_o, f_io);
    cpy16(f_io, f_i);
}

void his_op_conf
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in] */ idl_small_int i_size,
      /* [in] */ PRIVATE_CHAR_32 c_i[],
      /* [in] */ idl_short_int io_size,
      /* [in, out] */ PRIVATE_CHAR_32 c_io[],
      /* [in] */ idl_long_int o_size,
      /* [out] */ PRIVATE_CHAR_32 c_o[]
)
#else
( h, i_size, c_i, io_size, c_io, o_size, c_o )
      /* [in] */ handle_t h;
      /* [in] */ idl_small_int i_size;
      /* [in] */ PRIVATE_CHAR_32 c_i[];
      /* [in] */ idl_short_int io_size;
      /* [in, out] */ PRIVATE_CHAR_32 c_io[];
      /* [in] */ idl_long_int o_size;
      /* [out] */ PRIVATE_CHAR_32 c_o[];
#endif
{
    cpy32(c_o, c_io);
    cpy32(c_io, c_i);
}

void his_op_self( h, s_i, s_io )
      /* [in] */ handle_t h;
      /* [in] */ PRIVATE_CHAR_16 s_i[];
      /* [in, out] */ PRIVATE_CHAR_16 s_io[];
{
    cpy16(s_io, s_i);
}

void his_op_multi( h, m_i, m_o, p16, tb )
      /* [in] */ handle_t h;
      /* [in] */ PRIVATE_CHAR_32 m_i[];
      /* [out] */ PRIVATE_CHAR_32 m_o[14];
      /* [in, out] */ private_16 *p16;
      /* [in, out] */ two_byte *tb;
{
    int i;
    
    i=0;
    while (m_i[i] != 0)
    {
        m_o[i] = m_i[i] + 0x0d0d0d0d;   /* +13 each byte */
        i++;
    }
    m_o[i] = 0;
    init16(p16->s_p, (idl_char*)"Orion plunges prone");
    i=0;
    while (tb->p_32[i] != 0)
    {
        tb->p_32[i] -= 0x20202020;  /* -0x20 each byte */
        i++;
    }
}

void his_op_sp( h, p_sp)
      /* [in] */ handle_t h;
      /* [in, out] */ stringptrs_t *p_sp;
{
    PRIVATE_CHAR_16 *alphawork;
    int i;

    alphawork = (PRIVATE_CHAR_16 *)
                rpc_ss_allocate(27 * sizeof(PRIVATE_CHAR_16));
    for(i=0; i<26; i++)
    {
        alphawork[i] = p_sp->astring[i] + 0x4141 - 0x6161;
    }
    alphawork[26] = 0;
    p_sp->astring = NULL;
    p_sp->bstring = alphawork;
}

void his_op_fas( h, ilsa, imsa)
      /* [in] */ handle_t h;
      /* [in] */ ils_t ilsa[5];
      /* [out] */ ims_t imsa[5];
{
    int i,j;
    idl_usmall_int b0,b1,b2,b3;

    for (i=0; i<5; i++)
    {
        j = -1;
        do {
            j++;
            b0 = ilsa[i][j] % 256;
            b1 = ilsa[i][j] / 256;
            b2 = b0 + 2;
            b3 = b1 + 2;
            imsa[i][j] = (b3<<24) + (b2<<16) + (b1<<8) + b0;
        } while (ilsa[i][j] != 0);
        imsa[i][j] = 0;
    }
}

void his_op_fac( h,s, imsa, iusa)
      /* [in] */ handle_t h;
      /* [in] */ idl_long_int s;
      /* [in] */ ims_t imsa[];
      /* [out] */ ius_t iusa[];
{
    int i,j;

    for (i=0; i<s; i++)
    {
        j = -1;
        do {
            j++;
            iusa[i][j] = imsa[i][j] + 0x20202020;   /* +0x20 each byte */
        } while (imsa[i][j] != 0);
        iusa[i][j] = 0;
    }
}

void his_op_selfconf( h, scs )
      /* [in] */ handle_t h;
      /* [in, out] */ PRIVATE_CHAR_16 scs[];
{
    int i;

    for (i=0; i<(strlen(scs)/2); i++)
        scs[i] -= 0x6161 - 0x4141;
}

/*globaldef*/ strint_v0_0_epv_t strint_v0_0_m_epv = {
    his_op_fixed,
    his_op_conf,
    his_op_self,
    his_op_multi,
    his_op_sp,
    his_op_fas,
    his_op_fac,
    his_op_selfconf
};

