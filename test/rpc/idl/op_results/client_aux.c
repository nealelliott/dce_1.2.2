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
 * Revision 1.1.4.2  1996/02/17  23:15:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:18  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:12  root]
 * 
 * Revision 1.1.2.3  1993/01/13  21:49:20  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:36:36  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <test_common.h>
#include <op_results.h>

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    int i;

    /* base type operation results */
    error_status_t error_status_val[3];
    context_handle_t context_handle_val[3];
    ndr_boolean boolean_val[3];
    ndr_byte byte_val[3];
    ndr_char char_val[3];
    ndr_small_int small_int_val[3];
    ndr_short_int short_int_val[3];
    ndr_long_int long_int_val[3];
    ndr_hyper_int hyper_int_val[3];
    ndr_usmall_int usmall_int_val[3];
    ndr_ushort_int ushort_int_val[3];
    ndr_ulong_int ulong_int_val[3];
    ndr_uhyper_int uhyper_int_val[3];
    ndr_short_float short_float_val[3];
    ndr_long_float long_float_val[3];
    color_t enum_val[3];

    /* pointer to base type operation results */
    ndr_boolean * p_boolean_val[3];
    ndr_byte * p_byte_val[3];
    ndr_char * p_char_val[3];
    ndr_small_int * p_small_int_val[3];
    ndr_short_int * p_short_int_val[3];
    ndr_long_int * p_long_int_val[3];
    ndr_hyper_int * p_hyper_int_val[3];
    ndr_usmall_int * p_usmall_int_val[3];
    ndr_ushort_int * p_ushort_int_val[3];
    ndr_ulong_int * p_ulong_int_val[3];
    ndr_uhyper_int * p_uhyper_int_val[3];
    ndr_short_float * p_short_float_val[3];
    ndr_long_float * p_long_float_val[3];
    color_t * p_enum_val[3];

    /* pointer operation results */
    fixed_array_t	*fixed_array_val;
    conformant_array_t	*conformant_array_val;
    fixed_struct_t	*fixed_struct_val;
    varying_struct_t	*varying_struct_val;
    conf_struct_t	*conformant_struct_val;
    var_conf_struct_t	*varying_conformant_struct_val;

/*    
 * Initializtion of variables
 */
    error_status_val[0] = 0xAAAAAAAA;
    error_status_val[1] = 0xAAAAAAAA;
    error_status_val[2] = 0xAAAAAAAA;
    context_handle_val[0] = (context_handle_t)0xAAAAAAAA;
    context_handle_val[1] = (context_handle_t)0xAAAAAAAA;
    context_handle_val[2] = (context_handle_t)0xAAAAAAAA;
    boolean_val[0]	    = 0xAA;
    boolean_val[1]	    = 0xAA;
    boolean_val[2]	    = 0xAA;
    byte_val[0]	    = 0xAA;
    byte_val[1]	    = 0xAA;
    byte_val[2]	    = 0xAA;
    char_val[0]	    = 0xAA;
    char_val[1]	    = 0xAA;
    char_val[2]	    = 0xAA;
    small_int_val[0]  = 0xAA;
    small_int_val[1]  = 0xAA;
    small_int_val[2]  = 0xAA;
    short_int_val[0]  = 0xAAAA;
    short_int_val[1]  = 0xAAAA;
    short_int_val[2]  = 0xAAAA;
    long_int_val[0]    = 0xAAAAAAAA;
    long_int_val[1]    = 0xAAAAAAAA;
    long_int_val[2]    = 0xAAAAAAAA;
    hyper_int_val[0].high  = 0xAAAAAAAA;
    hyper_int_val[0].low  = 0xAAAAAAAA;
    hyper_int_val[1].high  = 0xAAAAAAAA;
    hyper_int_val[1].low  = 0xAAAAAAAA;
    hyper_int_val[2].high  = 0xAAAAAAAA;
    hyper_int_val[2].low  = 0xAAAAAAAA;
    usmall_int_val[0]	= 0xAA;
    usmall_int_val[1]	= 0xAA;
    usmall_int_val[2]	= 0xAA;
    ushort_int_val[0]	= 0xAAAA;
    ushort_int_val[1]	= 0xAAAA;
    ushort_int_val[2]	= 0xAAAA;
    ulong_int_val[0]	= 0xAAAAAAAA;
    ulong_int_val[1]	= 0xAAAAAAAA;
    ulong_int_val[2]	= 0xAAAAAAAA;
    uhyper_int_val[0].high	= 0xAAAAAAAA;
    uhyper_int_val[0].low	= 0xAAAAAAAA;
    uhyper_int_val[1].high	= 0xAAAAAAAA;
    uhyper_int_val[1].low	= 0xAAAAAAAA;
    uhyper_int_val[2].high	= 0xAAAAAAAA;
    uhyper_int_val[2].low	= 0xAAAAAAAA;
    short_float_val[0]	= 99;
    short_float_val[1]	= 99;
    short_float_val[2]	= 99;
    long_float_val[0]	= 77;
    long_float_val[1]	= 77;
    long_float_val[2]	= 77;
    enum_val[0] = RED;
    enum_val[1] = RED;
    enum_val[2] = RED;

    p_boolean_val[0] = &boolean_val[0];
    p_boolean_val[2] = &boolean_val[2];
    p_byte_val[0] = &byte_val[0];
    p_byte_val[2] = &byte_val[2];
    p_char_val[0] = &char_val[0];
    p_char_val[2] = &char_val[2];
    p_small_int_val[0] = &small_int_val[0];
    p_small_int_val[2] = &small_int_val[2];
    p_short_int_val[0] = &short_int_val[0];
    p_short_int_val[2] = &short_int_val[2];
    p_long_int_val[0] = &long_int_val[0];
    p_long_int_val[2] = &long_int_val[2];
    p_hyper_int_val[0] = &hyper_int_val[0];
    p_hyper_int_val[2] = &hyper_int_val[2];
    p_usmall_int_val[0] = &usmall_int_val[0];
    p_usmall_int_val[2] = &usmall_int_val[2];
    p_ushort_int_val[0] = &ushort_int_val[0];
    p_ushort_int_val[2] = &ushort_int_val[2];
    p_ulong_int_val[0] = &ulong_int_val[0];
    p_ulong_int_val[2] = &ulong_int_val[2];
    p_uhyper_int_val[0] = &uhyper_int_val[0];
    p_uhyper_int_val[2] = &uhyper_int_val[2];
    p_short_float_val[0] = &short_float_val[0];
    p_short_float_val[2] = &short_float_val[2];
    p_long_float_val[0] = &long_float_val[0];
    p_long_float_val[2] = &long_float_val[2];
    p_enum_val[0] = &enum_val[0];
    p_enum_val[2] = &enum_val[2];


    /*	
     * Set the implicit_handle global to the handle obtained above
     */
    i_h = h;


    /*	
     * Now the real tests for returned scalars...
     */
    error_status_val[1] = comm_status_t_op();
    if (error_status_val[1] != error_status_ok)
    {
        printf("Error in error_status_val\n");
    }
    if (error_status_val[0] != 0xAAAAAAAA)
    {
        printf("Error before error_status_val\n");
    }
    if (error_status_val[2] != 0xAAAAAAAA)
    {
        printf("Error after error_status_val\n");
    }

    context_handle_val[1] = context_handle_op();
    if (context_handle_val[1] == (context_handle_t)NULL
        ||  context_handle_val[1] == (context_handle_t)0xAAAAAAAA)
    {
        printf("Error in context_handle_val\n");
    }
    if (context_handle_val[0] != (context_handle_t)0xAAAAAAAA)
    {
        printf("Error before context_handle_val\n");
    }
    if (context_handle_val[2] != (context_handle_t)0xAAAAAAAA)
    {
        printf("Error after context_handle_val\n");
    }

    boolean_val[1] = boolean_op();
    if (boolean_val[1] != boolean_return_val)
    {
        printf("Error in boolean_val\n");
    }
    if (boolean_val[0] != 0xAA)
    {
        printf("Error before boolean_val\n");
    }
    if (boolean_val[2] != 0xAA)
    {
        printf("Error after boolean_val\n");
    }


    byte_val[1] = byte_op();
    if (byte_val[1] != byte_return_val)
    {
        printf("Error in byte_val\n");
    }
    if (byte_val[0] != 0xaa)
    {
        printf("Error before byte_val\n");
    }
    if (byte_val[2] != 0xaa)
    {
        printf("Error after byte_val\n");
    }

    char_val[1] = char_op();
    if (char_val[1] != char_return_val)
    {
        printf("Error in char_val\n");
    }
    if (char_val[0] != 0xaa)
    {
        printf("Error before char_val\n");
    }
    if (char_val[2] != 0xaa)
    {
        printf("Error after char_val\n");
    }

    small_int_val[1] = small_op();
    if (small_int_val[1] != small_return_val )
    {
        printf("Error in small_int_val\n");
    }
    if (small_int_val[0] != (ndr_small_int)0xaa)
    {
        printf("Error before small_int_val\n");
    }
    if (small_int_val[2] !=  (ndr_small_int)0xaa)
    {
        printf("Error after small_int_val\n");
    }

    short_int_val[1] = short_op();
    if (short_int_val[1] != short_return_val )
    {
        printf("Error in short_int_val\n");
    }
    if (short_int_val[0] != (ndr_short_int)0xaaaa)
    {
        printf("Error before short_int_val\n");
    }
    if (short_int_val[2] != (ndr_short_int)0xaaaa)
    {
        printf("Error after short_int_val\n");
    }
    
    long_int_val[1] = long_op(context_handle_val[1]);
    if (long_int_val[1] != long_return_val )
    {
        printf("Error in long_int_val\n");
    }
    if (long_int_val[0] != (ndr_long_int)0xaaaaaaaa)
    {
        printf("Error before long_int_val\n");
    }
    if (long_int_val[2] != (ndr_long_int)0xaaaaaaaa)
    {
        printf("Error after long_int_val\n");
    }

    hyper_int_val[1]= hyper_op();
    if ((hyper_int_val[1]).low != hyper_low_return_val)
    {
        printf("Error in hyper_int_val.low\n");
    }
    if ((hyper_int_val[1]).high != hyper_high_return_val)
    {
        printf("Error in hyper_int_val.high\n");
    }
    if ((hyper_int_val[0]).low != 0xaaaaaaaa)
    {
        printf("Error before hyper_int_val.low\n");
    }
    if ((hyper_int_val[0]).high != 0xaaaaaaaa)
    {
        printf("Error before hyper_int_val.high\n");
    }
    if ((hyper_int_val[2]).low != 0xaaaaaaaa)
    {
        printf("Error after hyper_int_val.low\n");
    }
    if ((hyper_int_val[2]).high != 0xaaaaaaaa)
    {
        printf("Error after hyper_int_val.high\n");
    }

    usmall_int_val[1] = usmall_op();
    if (usmall_int_val[1] != usmall_return_val )
    {
        printf("Error in usmall_int_val\n");
    }
    if (usmall_int_val[0] != 0xaa)
    {
        printf("Error before usmall_int_val\n");
    }
    if (usmall_int_val[2] != 0xaa)
    {
        printf("Error after usmall_int_val\n");
    }

    ushort_int_val[1] = ushort_op();
    if (ushort_int_val[1] != ushort_return_val )
    {
        printf("Error in ushort_int_val\n");
    }
    if (ushort_int_val[0] != 0xaaaa )
    {
        printf("Error before ushort_int_val\n");
    }
    if (ushort_int_val[2] != 0xaaaa )
    {
        printf("Error after ushort_int_val\n");
    }

    ulong_int_val[1] = ulong_op();
    if (ulong_int_val[1] != ulong_return_val )
    {
        printf("Error in ulong_int_val\n");
    }
    if (ulong_int_val[0] != 0xaaaaaaaa )
    {
        printf("Error before ulong_int_val\n");
    }
    if (ulong_int_val[2] != 0xaaaaaaaa)
    {
        printf("Error after ulong_int_val\n");
    }
    
    uhyper_int_val[1]= uhyper_op();
    if ((uhyper_int_val[1]).low != uhyper_low_return_val)
    {
        printf("Error in uhyper_int_val[1].low\n");
    }
    if ((uhyper_int_val[1]).high != uhyper_high_return_val)
    {
        printf("Error in uhyper_int_val[1].high\n");
    }
    if ((uhyper_int_val[0]).low != 0xaaaaaaaa)
    {
        printf("Error before uhyper_int_val[0].low\n");
    }
    if ((uhyper_int_val[0]).high != 0xaaaaaaaa)
    {
        printf("Error before uhyper_int_val[0].high\n");
    }
    if ((uhyper_int_val[2]).low != 0xaaaaaaaa)
    {
        printf("Error after uhyper_int_val[2].low\n");
    }
    if ((uhyper_int_val[2]).high != 0xaaaaaaaa)
    {
        printf("Error after uhyper_int_val[2].high\n");
    }

    short_float_val[1] = float_op();
    if (short_float_val[1] != float_return_val )
    {
        printf("Error in short_float_val\n");
    }
    if (short_float_val[0] != 99)
    {
        printf("Error before short_float_val\n");
    }
    if (short_float_val[2] != 99)
    {
        printf("Error after short_float_val\n");
    }

    long_float_val[1] = double_op();
    if (long_float_val[1] != double_return_val )
    {
        printf("Error in long_float_val[1]\n");
    }
    if (long_float_val[0] != 77)
    {
        printf("Error before long_float_val[0]\n");
    }
    if (long_float_val[2] != 77)
    {
        printf("Error after long_float_val[2]\n");
    }

    enum_val[1] = enum_op();
    if (enum_val[1] != enum_return_val)
    {
        printf("Error in enum_val[1]\n");
    }
    if (enum_val[0] != RED)
    {
        printf("Error in enum_val[0]\n");
    }
    if (enum_val[2] != RED)
    {
        printf("Error in enum_val[2]\n");
    }


    /*	
     * Now the real tests for returned pointers to scalars...
     */
    p_boolean_val[1] = p_boolean_op();
    if (*p_boolean_val[1] != boolean_return_val)
    {
        printf("Error in p_boolean_val\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_boolean_val[1]);
    if (*p_boolean_val[0] != 0xAA)
    {
        printf("Error before p_boolean_val\n");
    }
    if (*p_boolean_val[2] != 0xAA)
    {
        printf("Error after p_boolean_val\n");
    }


    p_byte_val[1] = p_byte_op();
    if (*p_byte_val[1] != byte_return_val)
    {
        printf("Error in p_byte_val\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_byte_val[1]);
    if (*p_byte_val[0] != 0xaa)
    {
        printf("Error before p_byte_val\n");
    }
    if (*p_byte_val[2] != 0xaa)
    {
        printf("Error after p_byte_val\n");
    }

    p_char_val[1] = p_char_op();
    if (*p_char_val[1] != char_return_val)
    {
        printf("Error in p_char_val\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_char_val[1]);
    if (*p_char_val[0] != 0xaa)
    {
        printf("Error before p_char_val\n");
    }
    if (*p_char_val[2] != 0xaa)
    {
        printf("Error after p_char_val\n");
    }

    p_small_int_val[1] = p_small_op();
    if (*p_small_int_val[1] != small_return_val )
    {
        printf("Error in p_small_int_val\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_small_int_val[1]);
    if (*p_small_int_val[0] != (ndr_small_int)0xaa)
    {
        printf("Error before p_small_int_val\n");
    }
    if (*p_small_int_val[2] !=  (ndr_small_int)0xaa)
    {
        printf("Error after p_small_int_val\n");
    }

    p_short_int_val[1] = p_short_op();
    if (*p_short_int_val[1] != short_return_val )
    {
        printf("Error in p_short_int_val\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_short_int_val[1]);
    if (*p_short_int_val[0] != (ndr_short_int)0xaaaa)
    {
        printf("Error before p_short_int_val\n");
    }
    if (*p_short_int_val[2] != (ndr_short_int)0xaaaa)
    {
        printf("Error after p_short_int_val\n");
    }
    
    p_long_int_val[1] = p_long_op();
    if (*p_long_int_val[1] != long_return_val )
    {
        printf("Error in p_long_int_val\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_long_int_val[1]);
    if (*p_long_int_val[0] != (ndr_long_int)0xaaaaaaaa)
    {
        printf("Error before p_long_int_val\n");
    }
    if (*p_long_int_val[2] != (ndr_long_int)0xaaaaaaaa)
    {
        printf("Error after p_long_int_val\n");
    }

    p_hyper_int_val[1]= p_hyper_op();
    if ((*p_hyper_int_val[1]).low != hyper_low_return_val)
    {
        printf("Error in p_hyper_int_val.low\n");
    }
    if ((*p_hyper_int_val[1]).high != hyper_high_return_val)
    {
        printf("Error in p_hyper_int_val.high\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_hyper_int_val[1]);
    if ((*p_hyper_int_val[0]).low != 0xaaaaaaaa)
    {
        printf("Error before p_hyper_int_val.low\n");
    }
    if ((*p_hyper_int_val[0]).high != 0xaaaaaaaa)
    {
        printf("Error before p_hyper_int_val.high\n");
    }
    if ((*p_hyper_int_val[2]).low != 0xaaaaaaaa)
    {
        printf("Error after p_hyper_int_val.low\n");
    }
    if ((*p_hyper_int_val[2]).high != 0xaaaaaaaa)
    {
        printf("Error after p_hyper_int_val.high\n");
    }

    p_usmall_int_val[1] = p_usmall_op();
    if (*p_usmall_int_val[1] != usmall_return_val )
    {
        printf("Error in p_usmall_int_val\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_usmall_int_val[1]);
    if (*p_usmall_int_val[0] != 0xaa)
    {
        printf("Error before p_usmall_int_val\n");
    }
    if (*p_usmall_int_val[2] != 0xaa)
    {
        printf("Error after p_usmall_int_val\n");
    }

    p_ushort_int_val[1] = p_ushort_op();
    if (*p_ushort_int_val[1] != ushort_return_val )
    {
        printf("Error in p_ushort_int_val\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_ushort_int_val[1]);
    if (*p_ushort_int_val[0] != 0xaaaa )
    {
        printf("Error before p_ushort_int_val\n");
    }
    if (*p_ushort_int_val[2] != 0xaaaa )
    {
        printf("Error after p_ushort_int_val\n");
    }

    p_ulong_int_val[1] = p_ulong_op();
    if (*p_ulong_int_val[1] != ulong_return_val )
    {
        printf("Error in p_ulong_int_val\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_ulong_int_val[1]);
    if (*p_ulong_int_val[0] != 0xaaaaaaaa )
    {
        printf("Error before p_ulong_int_val\n");
    }
    if (*p_ulong_int_val[2] != 0xaaaaaaaa)
    {
        printf("Error after p_ulong_int_val\n");
    }
    
    p_uhyper_int_val[1]= p_uhyper_op();
    if ((*p_uhyper_int_val[1]).low != uhyper_low_return_val)
    {
        printf("Error in p_uhyper_int_val[1].low\n");
    }
    if ((*p_uhyper_int_val[1]).high != uhyper_high_return_val)
    {
        printf("Error in p_uhyper_int_val[1].high\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_uhyper_int_val[1]);
    if ((*p_uhyper_int_val[0]).low != 0xaaaaaaaa)
    {
        printf("Error before p_uhyper_int_val[0].low\n");
    }
    if ((*p_uhyper_int_val[0]).high != 0xaaaaaaaa)
    {
        printf("Error before p_uhyper_int_val[0].high\n");
    }
    if ((*p_uhyper_int_val[2]).low != 0xaaaaaaaa)
    {
        printf("Error after p_uhyper_int_val[2].low\n");
    }
    if ((*p_uhyper_int_val[2]).high != 0xaaaaaaaa)
    {
        printf("Error after p_uhyper_int_val[2].high\n");
    }

    p_short_float_val[1] = p_float_op();
    if (*p_short_float_val[1] != float_return_val )
    {
        printf("Error in p_short_float_val\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_short_float_val[1]);
    if (*p_short_float_val[0] != 99)
    {
        printf("Error before p_short_float_val\n");
    }
    if (*p_short_float_val[2] != 99)
    {
        printf("Error after p_short_float_val\n");
    }

    p_long_float_val[1] = p_double_op();
    if (*p_long_float_val[1] != double_return_val )
    {
        printf("Error in p_long_float_val[1]\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_long_float_val[1]);
    if (*p_long_float_val[0] != 77)
    {
        printf("Error before p_long_float_val[0]\n");
    }
    if (*p_long_float_val[2] != 77)
    {
        printf("Error after p_long_float_val[2]\n");
    }

    p_enum_val[1] = p_enum_op();
    if (*p_enum_val[1] != enum_return_val)
    {
        printf("Error in p_enum_val[1]\n");
    }
    /* Call free to make sure we haven't overwritten unowned memory */
    free(p_enum_val[1]);
    if (*p_enum_val[0] != RED)
    {
        printf("Error in p_enum_val[0]\n");
    }
    if (*p_enum_val[2] != RED)
    {
        printf("Error in p_enum_val[2]\n");
    }


/* 
** Operation returning pointer to fixed array
*/
    fixed_array_val = fixed_array_t_op();
    /* Check the existing values */
    for (i = 0; i < fixed_array_size; i++)
	if ((*fixed_array_val)[i] != i) printf("Error in fixed_array_val\n");
    /* Call free to make sure we haven't overwritten unowned memory */
    free(fixed_array_val);

/* 
** Operation returning pointer to conformant array
    conformant_array_val = conformant_array_t_op();
    for (i = 0; i < conformant_array_size; i++)
	if ((*conformant_array_val)[i] != i) printf("Error in conformant_array_val\n");
    free(conformant_array_val);
*/


/* 
** Operation returning pointer to fixed struct
*/
    fixed_struct_val = fixed_struct_t_op();
    /* check for correct returned size */
    if (fixed_struct_val->size != fixed_array_size) printf("Error in fixed_struct_val->size\n");
    /* Check the existing values */
    for (i = 0; i < fixed_array_size; i++)
	if ((*fixed_struct_val).arr[i] != i) printf("Error in fixed_struct_val->arr[]\n");
    /* Call free to make sure we haven't overwritten unowned memory */
    free(fixed_struct_val);


/* 
** Operation returning pointer to varying struct
*/
    varying_struct_val = varying_struct_t_op();
    /* check for correct returned size */
    if (varying_struct_val->size != varying_array_size) printf("Error in varying_struct_val->size\n");
    /* Check the existing values */
    for (i = 0; i < varying_array_size; i++)
	if ((*varying_struct_val).arr[i] != i) printf("Error in varying_struct_val->arr[]\n");
    /* Make sure all of the space is allocated */
    for (i = 0; i < fixed_array_size; i++) (*varying_struct_val).arr[i] = i;
    /* Call free to make sure we haven't overwritten unowned memory */
    free(varying_struct_val);


/* 
** Operation returning pointer to conformant struct
*/
    conformant_struct_val = conformant_struct_t_op();
    /* check for correct returned size */
    if (conformant_struct_val->size != conformant_array_size) printf("Error in conformant_struct_val->size\n");
    /* Check the existing values */
    for (i = 0; i < conformant_array_size; i++)
	if ((*conformant_struct_val).arr[i] != i) printf("Error in conformant_struct_val->arr[]\n");
    /* Call free to make sure we haven't overwritten unowned memory */
    free(conformant_struct_val);



/* 
** Operation returning pointer to varying and conformant struct
*/
    varying_conformant_struct_val = varying_conformant_struct_t_op();
    /* check for correct returned size */
    if (varying_conformant_struct_val->size != conformant_array_size) 
          printf("Error in varying_conformant_struct_val->size\n");
    if (varying_conformant_struct_val->used != varying_array_size) 
       printf("Error in varying_conformant_struct_val->used\n");
    /* Check the existing values */
    for (i = 0; i < varying_array_size; i++)
	if ((*varying_conformant_struct_val).arr[i] != i) printf("Error in varying_conformant_struct_val->arr[]\n");
    /* Make sure all of the space is allocated */
    for (i = 0; i < conformant_array_size; i++) 
        (*varying_conformant_struct_val).arr[i] = i;
    /* Call free to make sure we haven't overwritten unowned memory */
    free(varying_conformant_struct_val);

 
/*
** Test completed, this should be the only message on a successful test
*/
    printf("Test complete\n");

    return 0;
}
