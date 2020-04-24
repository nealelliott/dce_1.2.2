/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY 
 * $Log: marshall.h,v $
 * Revision 1.1.132.2  1996/02/18  22:57:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:07  marty]
 *
 * Revision 1.1.132.1  1995/12/08  00:22:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:49  root]
 * 
 * Revision 1.1.130.2  1994/06/10  20:54:55  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:21  devsrc]
 * 
 * Revision 1.1.130.1  1994/01/21  22:39:51  cbrooks
 * 	changed struct tags for hyper ints
 * 	[1994/01/21  17:39:32  cbrooks]
 * 
 * 	platform dependent RPC marshalling macros
 * 	[1994/01/21  17:15:43  cbrooks]
 * 
 * $EndLog$
 */
/*
 * marshall.h 
 * platform dependent (OS + Architecture) file split out from stubbase.h 
 * for DCE 1.1 code cleanup 
 *
 * This file is always included as part of stubbase.h 
 */

#ifndef _MARSHALL_H 
#define _MARSHALL_H

#undef USE_DEFAULT_MACROS

#define rpc_marshall_boolean(mp, src)\
    *(ndr_boolean *)mp = src

#define rpc_unmarshall_boolean(mp, dst)\
    dst = *(ndr_boolean *)mp

#define rpc_convert_boolean(src_drep, dst_drep, mp, dst)\
    rpc_unmarshall_boolean(mp, dst)



#define rpc_marshall_byte(mp, src)\
    *(ndr_byte *)mp = src

#define rpc_unmarshall_byte(mp, dst)\
    dst = *(ndr_byte *)mp

#define rpc_convert_byte(src_drep, dst_drep, mp, dst)\
    rpc_unmarshall_byte(mp, dst)



#define rpc_marshall_char(mp, src)\
    *(ndr_char *)mp = src

#define rpc_unmarshall_char(mp, dst)\
    *((ndr_char *) &dst) = *(ndr_char *)mp

#define rpc_convert_char(src_drep, dst_drep, mp, dst)\
    if (src_drep.char_rep == dst_drep.char_rep)\
        rpc_unmarshall_char(mp, dst);\
    else if (dst_drep.char_rep == ndr_c_char_ascii)\
        *((ndr_char *) &dst) = (*ndr_g_ebcdic_to_ascii) [*(ndr_char *)mp];\
    else\
        *((ndr_char *) &dst) = (*ndr_g_ascii_to_ebcdic) [*(ndr_char *)mp]



#define rpc_marshall_enum(mp, src)\
    *(ndr_short_int *)mp = (ndr_short_int)src

#define rpc_unmarshall_enum(mp, dst)\
    dst = *(ndr_short_int *)mp

#define rpc_convert_enum(src_drep, dst_drep, mp, dst)\
    if (src_drep.int_rep == dst_drep.int_rep)\
        rpc_unmarshall_enum(mp, dst);\
    else {\
        ndr_short_int _sh;\
        ndr_byte *_d = (ndr_byte *) &_sh;\
        ndr_byte *_s = (ndr_byte *) mp;\
        _d[0]=_s[1]; _d[1]=_s[0];\
        dst = _sh;\
        }



#ifdef TWO_BYTE_ENUMS
#define rpc_marshall_v1_enum(mp, src)\
    rpc_marshall_ushort(mp, 0);\
    rpc_marshall_enum((mp+2), src)

#define rpc_unmarshall_v1_enum(mp, dst)\
    rpc_unmarshall_enum((mp+2), src)

#define rpc_convert_v1_enum(src_drep, dst_drep, mp, dst)\
    if (src_drep.int_rep == dst_drep.int_rep)\
        rpc_unmarshall_v1_enum(mp, dst);\
    else {\
        ndr_ushort_int _sh;\
        ndr_byte *_d = (ndr_byte *) &_sh;\
        ndr_byte *_s = (ndr_byte *) mp;\
        _d[0]=_s[1]; _d[1]=_s[0];\
        dst = _sh;\
        }
#else
#define rpc_marshall_v1_enum(mp, src)\
    *(ndr_ulong_int *)mp = (ndr_ulong_int)src

#define rpc_unmarshall_v1_enum(mp, dst)\
    dst = *(ndr_ulong_int *)mp

#define rpc_convert_v1_enum(src_drep, dst_drep, mp, dst)\
    if (src_drep.int_rep == dst_drep.int_rep)\
        rpc_unmarshall_v1_enum(mp, dst);\
    else {\
        ndr_ulong_int _l;\
        ndr_byte *_d = (ndr_byte *) &_l;\
        ndr_byte *_s = (ndr_byte *) mp;\
        _d[0]=_s[3]; _d[1]=_s[2]; _d[2]=_s[1]; _d[3]=_s[0];\
        dst = _l;\
        }
#endif /* TWO_BYTE_ENUMS */



#define rpc_marshall_small_int(mp, src)\
    *(ndr_small_int *)mp = src

#define rpc_unmarshall_small_int(mp, dst)\
    dst = *(ndr_small_int *)mp

#define rpc_convert_small_int(src_drep, dst_drep, mp, dst)\
    rpc_unmarshall_small_int(mp, dst)



#define rpc_marshall_usmall_int(mp, src)\
    *(ndr_usmall_int *)mp = src

#define rpc_unmarshall_usmall_int(mp, dst)\
    dst = *(ndr_usmall_int *)mp

#define rpc_convert_usmall_int(src_drep, dst_drep, mp, dst)\
    rpc_unmarshall_usmall_int(mp, dst)



#define rpc_marshall_short_int(mp, src)\
    *(ndr_short_int *)mp = src

#define rpc_unmarshall_short_int(mp, dst)\
    dst = *(ndr_short_int *)mp

#define rpc_convert_short_int(src_drep, dst_drep, mp, dst)\
    if (src_drep.int_rep == dst_drep.int_rep)\
        rpc_unmarshall_short_int(mp, dst);\
    else {\
        ndr_byte *_d = (ndr_byte *) &dst;\
        ndr_byte *_s = (ndr_byte *) mp;\
        _d[0]=_s[1]; _d[1]=_s[0];\
        }



#define rpc_marshall_ushort_int(mp, src)\
    *(ndr_ushort_int *)mp = (ndr_ushort_int)src

#define rpc_unmarshall_ushort_int(mp, dst)\
    *((ndr_ushort_int *)&dst) = *(ndr_ushort_int *)mp

#define rpc_convert_ushort_int(src_drep, dst_drep, mp, dst)\
    if (src_drep.int_rep == dst_drep.int_rep)\
        rpc_unmarshall_ushort_int(mp, dst);\
    else {\
        ndr_byte *_d = (ndr_byte *) &dst;\
        ndr_byte *_s = (ndr_byte *) mp;\
        _d[0]=_s[1]; _d[1]=_s[0];\
        }



#define rpc_marshall_long_int(mp, src)\
    *(ndr_long_int *)mp = src

#define rpc_unmarshall_long_int(mp, dst)\
    dst = *(ndr_long_int *)mp

#define rpc_convert_long_int(src_drep, dst_drep, mp, dst)\
    if (src_drep.int_rep == dst_drep.int_rep)\
        rpc_unmarshall_long_int(mp, dst);\
    else {\
        ndr_byte *_d = (ndr_byte *) &dst;\
        ndr_byte *_s = (ndr_byte *) mp;\
        _d[0]=_s[3]; _d[1]=_s[2]; _d[2]=_s[1]; _d[3]=_s[0];\
        }



#define rpc_marshall_ulong_int(mp, src)\
    *(ndr_ulong_int *)mp = (ndr_ulong_int)src

#define rpc_unmarshall_ulong_int(mp, dst)\
    *((ndr_ulong_int *)&dst) = *(ndr_ulong_int *)mp

#define rpc_convert_ulong_int(src_drep, dst_drep, mp, dst)\
    if (src_drep.int_rep == dst_drep.int_rep)\
        rpc_unmarshall_ulong_int(mp, dst);\
    else {\
        ndr_byte *_d = (ndr_byte *) &dst;\
        ndr_byte *_s = (ndr_byte *) mp;\
        _d[0]=_s[3]; _d[1]=_s[2]; _d[2]=_s[1]; _d[3]=_s[0];\
        }



#define rpc_marshall_hyper_int(mp, src) {\
    *(struct ndr_hyper_int_rep_s_t *)mp = *(struct ndr_hyper_int_rep_s_t *)&src;\
    }

#define rpc_unmarshall_hyper_int(mp, dst) {\
    *(struct ndr_hyper_int_rep_s_t *)&dst = *(struct ndr_hyper_int_rep_s_t *)mp;\
    }

#define rpc_convert_hyper_int(src_drep, dst_drep, mp, dst)\
    if (src_drep.int_rep == dst_drep.int_rep)\
        rpc_unmarshall_hyper_int(mp, dst)\
    else {\
        ndr_byte *_d = (ndr_byte *) &dst;\
        ndr_byte *_s = (ndr_byte *) mp;\
        _d[0]=_s[7]; _d[1]=_s[6]; _d[2]=_s[5]; _d[3]=_s[4];\
        _d[4]=_s[3]; _d[5]=_s[2]; _d[6]=_s[1]; _d[7]=_s[0];\
        }



#define rpc_marshall_uhyper_int(mp, src) {\
    *(struct ndr_uhyper_int_rep_s_t *)mp = *(struct ndr_uhyper_int_rep_s_t *)&src;\
    }

#define rpc_unmarshall_uhyper_int(mp, dst) {\
    *(struct ndr_uhyper_int_rep_s_t *)&dst = *(struct ndr_uhyper_int_rep_s_t *)mp;\
    }

#define rpc_convert_uhyper_int(src_drep, dst_drep, mp, dst)\
    if (src_drep.int_rep == dst_drep.int_rep)\
        rpc_unmarshall_uhyper_int(mp, dst)\
    else {\
        ndr_byte *_d = (ndr_byte *) &dst;\
        ndr_byte *_s = (ndr_byte *) mp;\
        _d[0]=_s[7]; _d[1]=_s[6]; _d[2]=_s[5]; _d[3]=_s[4];\
        _d[4]=_s[3]; _d[5]=_s[2]; _d[6]=_s[1]; _d[7]=_s[0];\
        }



#define rpc_marshall_short_float(mp, src) {\
    ndr_short_float tmp;\
    tmp = src;\
    *(ndr_short_float *)mp = tmp;\
    }

#define rpc_unmarshall_short_float(mp, dst)\
    dst = *(ndr_short_float *)mp

#define rpc_convert_short_float(src_drep, dst_drep, mp, dst)\
    if ((src_drep.float_rep == dst_drep.float_rep) &&\
        (src_drep.int_rep   == dst_drep.int_rep))\
        rpc_unmarshall_short_float(mp, dst);\
    else {\
        ndr_cvt_short_float (src_drep, dst_drep,\
            (short_float_p_t)mp,\
            (short_float_p_t)&dst);\
        }



#define rpc_marshall_long_float(mp, src)\
    *(ndr_long_float *)mp = src

#define rpc_unmarshall_long_float(mp, dst)\
    dst = *(ndr_long_float *)mp

#define rpc_convert_long_float(src_drep, dst_drep, mp, dst)\
    if ((src_drep.float_rep == dst_drep.float_rep) &&\
        (src_drep.int_rep   == dst_drep.int_rep))\
        rpc_unmarshall_long_float(mp, dst);\
    else\
        ndr_cvt_long_float (src_drep, dst_drep,\
            (long_float_p_t)mp,\
            (long_float_p_t)&dst)

#endif /* _MARSHALL_H */
