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
 * Revision 1.1.4.2  1996/02/17  23:17:56  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:57  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:16  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:39:30  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:45:54  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <struct_ct_union.h>
#include <test_common.h>

#define JUNK    0xFEF1F0F8

void _st_dup(h, isp, osp)
    handle_t    h;
    struct_t    *isp;
    struct_t    *osp;
{
    /*
     * Rather than verify the data here simply duplicate the input data
     * and return it as output data for the client to validate.
     */
    osp->fu.sw = isp->fu.sw;
    osp->c = isp->c;
    osp->mu.sw = isp->mu.sw;
    osp->d = isp->d;
    osp->lu.sw = isp->lu.sw;

    /* Start with junk in all or part of union fields. */
    osp->fu.tagged_union.l = JUNK;
    osp->mu.tagged_union.struc.l = JUNK;
    osp->lu.tagged_union.l = JUNK;

    switch (isp->fu.sw)
    {
    case 'l':   osp->fu.tagged_union.l = isp->fu.tagged_union.l; break;
    case 's':   osp->fu.tagged_union.s = isp->fu.tagged_union.s; break;
    case 'c':   osp->fu.tagged_union.c = isp->fu.tagged_union.c; break;
    }

    switch (isp->mu.sw)
    {
    case 0:     break;
    case 1:
        osp->mu.tagged_union.struc.l = isp->mu.tagged_union.struc.l;
        osp->mu.tagged_union.struc.fu.sw = isp->mu.tagged_union.struc.fu.sw;
        osp->mu.tagged_union.struc.s = isp->mu.tagged_union.struc.s;
        osp->mu.tagged_union.struc.lu.sw = isp->mu.tagged_union.struc.lu.sw;
        osp->mu.tagged_union.struc.d = isp->mu.tagged_union.struc.d;

        /* Start with junk in all or part of union fields. */
        osp->mu.tagged_union.struc.fu.tagged_union.l = JUNK;
        osp->mu.tagged_union.struc.lu.tagged_union.l = JUNK;

        switch (isp->mu.tagged_union.struc.fu.sw)
        {
        case 'l':
            osp->mu.tagged_union.struc.fu.tagged_union.l
                = isp->mu.tagged_union.struc.fu.tagged_union.l;
            break;
        case 's':
            osp->mu.tagged_union.struc.fu.tagged_union.s
                = isp->mu.tagged_union.struc.fu.tagged_union.s;
            break;
        case 'c':
            osp->mu.tagged_union.struc.fu.tagged_union.c
                = isp->mu.tagged_union.struc.fu.tagged_union.c;
            break;
        }

        switch (isp->mu.tagged_union.struc.lu.sw)
        {
        case 0:
            osp->mu.tagged_union.struc.lu.tagged_union.c
                = isp->mu.tagged_union.struc.lu.tagged_union.c;
            break;
        case 1:
            osp->mu.tagged_union.struc.lu.tagged_union.s
                = isp->mu.tagged_union.struc.lu.tagged_union.s;
            break;
        case 2:
            osp->mu.tagged_union.struc.lu.tagged_union.l
                = isp->mu.tagged_union.struc.lu.tagged_union.l;
            break;
        default:
            osp->mu.tagged_union.struc.lu.tagged_union.d
                = isp->mu.tagged_union.struc.lu.tagged_union.d;
            break;
        }
    }

    switch (isp->lu.sw)
    {
    case 0:     osp->lu.tagged_union.c = isp->lu.tagged_union.c; break;
    case 1:     osp->lu.tagged_union.s = isp->lu.tagged_union.s; break;
    case 2:     osp->lu.tagged_union.l = isp->lu.tagged_union.l; break;
    default:    osp->lu.tagged_union.d = isp->lu.tagged_union.d; break;
    }
}

globaldef struct_ct_union_v0_0_epv_t struct_ct_union_v0_0_m_epv = {
    _st_dup
};
