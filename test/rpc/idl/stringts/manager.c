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
 * Revision 1.1.6.2  1996/02/17  23:17:41  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:47  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:30:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:05  root]
 * 
 * Revision 1.1.4.1  1994/02/01  22:00:29  cbrooks
 * 	OT 9858 - add dce.h to manager.c files
 * 	[1994/02/01  21:59:23  cbrooks]
 * 
 * Revision 1.1.2.2  1993/01/07  23:38:53  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:45:25  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <dce/dce.h>
#include <stdio.h>
#include <stringts.h>
#include <test_common.h>

void his_op_fixed(h, f_i, f_io, f_o)
    /* [in] */handle_t h;
    /* [in] */ndr_char f_i[81];
    /* [in, out] */ndr_char f_io[81];
    /* [out] */ndr_char f_o[81];
{
    strcpy(f_o, f_io);
    strcpy(f_io, f_i);
}

void his_op_conf
#ifdef _DCE_PROTO_
(
    /* [in] */handle_t h,
    /* [in] */ndr_small_int i_size,
    /* [in] */ndr_byte c_i[],
    /* [in] */ndr_short_int io_size,
    /* [in, out] */ndr_byte c_io[],
    /* [in] */ndr_long_int o_size,
    /* [out] */ndr_byte c_o[]
)
#else
( h, i_size, c_i, io_size, c_io, o_size, c_o )
    /* [in] */handle_t h;
    /* [in] */ndr_small_int i_size;
    /* [in] */ndr_byte c_i[];
    /* [in] */ndr_short_int io_size;
    /* [in, out] */ndr_byte c_io[];
    /* [in] */ndr_long_int o_size;
    /* [out] */ndr_byte c_o[];
#endif
{
    strcpy(c_o, c_io);
    strcpy(c_io, c_i);
}

void his_op_self( h, s_i, s_io )
    /* [in] */handle_t h;
    /* [in] */ISO_LATIN_1 s_i[];
    /* [in, out] */ISO_LATIN_1 s_io[];
{
    strcpy(s_io, s_i);
}

void his_op_multi( h, m_i, m_o, p8, tb )
    /* [in] */handle_t h;
    /* [in] */ISO_UCS m_i[];
    /* [out] */ISO_UCS m_o[14];
    /* [in, out] */private_8 *p8;
    /* [in, out] */two_byte *tb;
{
    int i;
    
    i=0;
    while (m_i[i].group != 0)
    {
        m_o[i].group = m_i[i].group + 13;
        m_o[i].plane = m_i[i].plane + 13;
        m_o[i].row = m_i[i].row + 13 - ('a' - 'A'); 
        m_o[i].column = m_i[i].column + 13 + ('a' - 'A');
        i++;
    }
    m_o[i].group = m_o[i].plane = m_o[i].row = m_o[i].column = 0;
    strcpy(p8->s_p, "Orion plunges prone");
    i=0;
    while (tb->p_16[i].row != 0)
    {
        tb->p_16[i].row = 'A' + ('Z' - tb->p_16[i].row);
        tb->p_16[i].column = 'a' + ('z' - tb->p_16[i].column);
        i++;
    }
}

globaldef stringts_v0_0_epv_t stringts_v0_0_m_epv = {
    his_op_fixed,
    his_op_conf,
    his_op_self,
    his_op_multi
};

