/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.12.2  1996/02/17  23:14:11  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:42  marty]
 *
 * Revision 1.1.12.1  1995/12/11  20:05:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:22  root]
 * 
 * Revision 1.1.9.1  1993/10/12  20:27:09  ganni
 * 	OT 9087 - flush the first half of the test for 486/osf
 * 	[1993/10/12  20:26:37  ganni]
 * 
 * Revision 1.1.7.2  1993/06/15  21:05:38  sommerfeld
 * 	The first half of this test is not portable and should be flushed.
 * 	We just flush it for HP-UX now.
 * 	[1993/06/14  20:55:46  sommerfeld]
 * 
 * Revision 1.1.3.3  1993/01/07  19:56:31  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:33:31  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:21:17  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:46:35  weisman]
 * 
 * Revision 1.1  1992/01/19  03:24:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <faultstat.h>

int client_aux(handle, passno)
    handle_t handle;
    int passno;
{
    handle_t bad_h;
    error_status_t cst, fst, fcst;

    fst = setup(handle);
          
#if (!defined(mips) && !defined(SNI_SINIX) && !defined(__hpux) && !defined(__OSF__)) || defined(PRT)
    /*
     * Don't do this code under MIPS RT since when RT attempts to dereference
     * bogus handle it kills the client on an unaligned access rather than
     * gracefully raising an exception.
     */
    /*
     * It's really a bad idea to use anything other than NULL, because
     * RPC runtime will try to dereference it to validate the handle. We
     * could allocate the storage for the bogus rpc_binding_rep_t and
     * initialize it to some bogus, but not causing segmentation
     * violation, values. But since handle_t is an opaque pointer, how
     * could we know its structure?
     */
    bad_h = (handle_t) 0xFFFFFFFF;

    fcst = fr_cr_op( bad_h );
    if (fcst != rpc_s_unbound_handle && fcst != rpc_s_invalid_binding)
        fprintf(stderr, "fr_cr_op incorrect comm_status 0x%lx\n", fcst);

    cst = fp_cr_op( bad_h, &fst );
    if (cst != rpc_s_unbound_handle && cst != rpc_s_invalid_binding)
        fprintf(stderr, "fp_cr_op incorrect comm_status 0x%lx\n", cst);
    if (fst != 0)
        fprintf(stderr, "fp_cr_op incorrect fault_status 0x%lx\n", fst);

    cst = fep_cr_op( bad_h, &fst );
    if (cst != rpc_s_unbound_handle && cst != rpc_s_invalid_binding)
        fprintf(stderr, "fep_cr_op incorrect comm_status 0x%lx\n", cst);
    if (fst != 0)
        fprintf(stderr, "fep_cr_op incorrect fault_status 0x%lx\n", fst);

    fst = fr_cp_op( bad_h, &cst );
    if (cst != rpc_s_unbound_handle && cst != rpc_s_invalid_binding)
        fprintf(stderr, "fr_cp_op incorrect comm_status 0x%lx\n", cst);
    if (fst != 0)
        fprintf(stderr, "fr_cp_op incorrect fault_status 0x%lx\n", fst);

    fdp_cp_op( bad_h, &cst, &fst );
    if (cst != rpc_s_unbound_handle && cst != rpc_s_invalid_binding)
        fprintf(stderr, "fdp_cp_op incorrect comm_status 0x%lx\n", cst);
    if (fst != 0)
        fprintf(stderr, "fdp_cp_op incorrect fault_status 0x%lx\n", fst);

    fsp_cp_op( bad_h, &fcst );
    if (fcst != rpc_s_unbound_handle && fcst != rpc_s_invalid_binding)
        fprintf(stderr, "fsp_cp_op incorrect comm_status 0x%lx\n", fcst);

    fep_cp_op( bad_h, &cst, &fst );
    if (cst != rpc_s_unbound_handle && cst != rpc_s_invalid_binding)
        fprintf(stderr, "fep_cp_op incorrect comm_status 0x%lx\n", cst);
    if (fst != 0)
        fprintf(stderr, "fep_cp_op incorrect fault_status 0x%lx\n", fst);

    fst = fr_cep_op( bad_h, &cst );
    if (cst != rpc_s_unbound_handle && cst != rpc_s_invalid_binding)
        fprintf(stderr, "fr_cep_op incorrect comm_status 0x%lx\n", cst);
    if (fst != 0)
        fprintf(stderr, "fr_cep_op incorrect fault_status 0x%lx\n", fst);

    fp_cep_op( bad_h, &fst, &cst );
    if (cst != rpc_s_unbound_handle && cst != rpc_s_invalid_binding)
        fprintf(stderr, "fp_cep_op incorrect comm_status 0x%lx\n", cst);
    if (fst != 0)
        fprintf(stderr, "fp_cep_op incorrect fault_status 0x%lx\n", fst);

    fsp_cep_op( bad_h, &fcst );
    if (fcst != rpc_s_unbound_handle && fcst != rpc_s_invalid_binding)
        fprintf(stderr, "fsp_cep_op incorrect comm_status 0x%lx\n", fcst);

    fdp_cep_op( bad_h, &cst, &fst );
    if (cst != rpc_s_unbound_handle && cst != rpc_s_invalid_binding)
        fprintf(stderr, "fdp_cep_op incorrect comm_status 0x%lx\n", cst);
    if (fst != 0)
        fprintf(stderr, "fdp_cep_op incorrect fault_status 0x%lx\n", fst);
#endif /* !defined(mips) || defined(PRT) */

    fcst = fr_cr_dz( handle );
    if (fcst != rpc_s_fault_fp_error && fcst != rpc_s_fault_int_div_by_zero)
        fprintf(stderr, "fr_cr_dz incorrect fault_status 0x%lx\n", fcst);

    cst = fp_cr_dz( handle, &fst );
    if (fst != rpc_s_fault_fp_error && fst != rpc_s_fault_int_div_by_zero)
        fprintf(stderr, "fp_cr_dz incorrect fault_status 0x%lx\n", fst);
    if (cst != 0)
        fprintf(stderr, "fp_cr_dz incorrect comm_status 0x%lx\n", cst);

    cst = fep_cr_dz( handle, &fst );
    if (fst != rpc_s_fault_fp_error && fst != rpc_s_fault_int_div_by_zero)
        fprintf(stderr, "fep_cr_dz incorrect fault_status 0x%lx\n", fst);
    if (cst != 0)
        fprintf(stderr, "fep_cr_dz incorrect comm_status 0x%lx\n", cst);

    fst = fr_cp_dz( handle, &cst );
    if (fst != rpc_s_fault_fp_error && fst != rpc_s_fault_int_div_by_zero)
        fprintf(stderr, "fr_cp_dz incorrect fault_status 0x%lx\n", fst);
    if (cst != 0)
        fprintf(stderr, "fr_cp_dz incorrect comm_status 0x%lx\n", cst);

    fdp_cp_dz( handle, &cst, &fst );
    if (fst != rpc_s_fault_fp_error && fst != rpc_s_fault_int_div_by_zero)
        fprintf(stderr, "fdp_cp_dz incorrect fault_status 0x%lx\n", fst);
    if (cst != 0)
        fprintf(stderr, "fdp_cp_dz incorrect comm_status 0x%lx\n", cst);

    fsp_cp_dz( handle, &fcst );
    if (fcst != rpc_s_fault_fp_error && fcst != rpc_s_fault_int_div_by_zero)
        fprintf(stderr, "fsp_cp_dz incorrect fault_status 0x%lx\n", fcst);

    fep_cp_dz( handle, &cst, &fst );
    if (fst != rpc_s_fault_fp_error && fst != rpc_s_fault_int_div_by_zero)
        fprintf(stderr, "fep_cp_dz incorrect fault_status 0x%lx\n", fst);
    if (cst != 0)
        fprintf(stderr, "fep_cp_dz incorrect comm_status 0x%lx\n", cst);

    fst = fr_cep_dz( handle, &cst );
    if (fst != rpc_s_fault_fp_error && fst != rpc_s_fault_int_div_by_zero)
        fprintf(stderr, "fr_cep_dz incorrect fault_status 0x%lx\n", fst);
    if (cst != 0)
        fprintf(stderr, "fr_cep_dz incorrect comm_status 0x%lx\n", cst);

    fp_cep_dz( handle, &fst, &cst );
    if (fst != rpc_s_fault_fp_error && fst != rpc_s_fault_int_div_by_zero)
        fprintf(stderr, "fp_cep_dz incorrect fault_status 0x%lx\n", fst);
    if (cst != 0)
        fprintf(stderr, "fp_cep_dz incorrect comm_status 0x%lx\n", cst);

    fsp_cep_dz( handle, &fcst );
    if (fcst != rpc_s_fault_fp_error && fcst != rpc_s_fault_int_div_by_zero)
        fprintf(stderr, "fsp_cep_dz incorrect fault_status 0x%lx\n", fcst);

    fdp_cep_dz( handle, &cst, &fst );
    if (fst != rpc_s_fault_fp_error && fst != rpc_s_fault_int_div_by_zero)
        fprintf(stderr, "fdp_cep_dz incorrect fault_status 0x%lx\n", fst);
    if (cst != 0)
        fprintf(stderr, "fdp_cep_dz incorrect comm_status 0x%lx\n", cst);

    return(0);
}
