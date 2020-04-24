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
 * Revision 1.1.4.2  1996/02/17  23:15:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:22  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:20  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:36:09  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:37:23  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <out_refs.h>
#include <test_common.h>

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    int i,j;
    error_status_t st;
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

    fixed_array_t	fixed_array_val[3];
    fixed_struct_t	fixed_struct_val[3];
    varying_struct_t	varying_struct_val[10];
    conf_struct_t	conformant_struct_val[10];
    var_conf_struct_t	varying_conformant_struct_val[10];

    /* init data values */
    for (i=0; i<3; i++)
    {
        boolean_val[i]      = 0xAA;
        byte_val[i]         = 0xAA;
        char_val[i]         = 0xAA;
        small_int_val[i]    = 0xAA;
        short_int_val[i]    = 0xAAAA;
        long_int_val[i]     = 0xAAAAAAAA;
        hyper_int_val[i].low = 0xAAAAAAAA;
        hyper_int_val[i].high= 0xAAAAAAAA;
        usmall_int_val[i]   = 0xAA;
        ushort_int_val[i]   = 0xAAAA;
        ulong_int_val[i]    = 0xAAAAAAAA;
        uhyper_int_val[i].low = 0xAAAAAAAA;
        uhyper_int_val[i].high= 0xAAAAAAAA;
        short_float_val[i]  = 99;
        long_float_val[i]   = 77;

        for (j=0; j<3; j++)
            fixed_array_val[i][j] = 0xAAAAAAAA;

        fixed_struct_val[i].size    = 0xAAAAAAAA;
        fixed_struct_val[i].arr[0]  = 0xAAAAAAAA;
    }

    for (i=0; i<10; i++)
    {
        varying_struct_val[i].size  = 0xAAAAAAAA;
        varying_struct_val[i].arr[0]= 0xAAAAAAAA;

        conformant_struct_val[i].size  = 0xAAAAAAAA;
        conformant_struct_val[i].arr[0]= 0xAAAAAAAA;

        varying_conformant_struct_val[i].size  = 0xAAAAAAAA;
        varying_conformant_struct_val[i].used  = 0xAAAAAAAA;
        varying_conformant_struct_val[i].arr[0]= 0xAAAAAAAA;
    }

    /* init struct values */
    fixed_struct_val[1].size = 0;
    varying_struct_val[1].size = 0;
    conformant_struct_val[1].size = 0;
    varying_conformant_struct_val[1].size = 0;
    varying_conformant_struct_val[1].used = 0;

    i_h = h;   
    boolean_op(&boolean_val[1]);
    if (boolean_val[0] != 0xAA)
    {
        printf("Error before boolean_val\n");
    }
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


    byte_op(&byte_val[1]);
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

    char_op(&char_val[1]);
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

    small_op(&small_int_val[1]);
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

    short_op(&short_int_val[1]);
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
    
    long_op(&long_int_val[1]);
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

    hyper_op(&hyper_int_val[1]);
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

    usmall_op(&usmall_int_val[1]);
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

    ushort_op(&ushort_int_val[1]);
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

    ulong_op(&ulong_int_val[1]);
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
    
    uhyper_op(&uhyper_int_val[1]);
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

    float_op(&short_float_val[1]);
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

    double_op(&long_float_val[1]);
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


/* 
** returning fixed array
*/
    fixed_array_t_op(&fixed_array_val[1]);
    /* Check the existing values */
    for (i = 0; i < fixed_array_size; i++)
	if (fixed_array_val[1][i] != i) printf("Error in fixed_array_val\n");


/* 
** returning fixed struct
*/
    fixed_struct_t_op(&fixed_struct_val[1]);
    /* check for correct returned size */
    if (fixed_struct_val[1].size != fixed_array_size) printf("Error in fixed_struct_val[1].size\n");
    /* Check the existing values */
    for (i = 0; i < fixed_array_size; i++)
	if (fixed_struct_val[1].arr[i] != i) printf("Error in fixed_struct_val[1].arr[]\n");


/* 
** returning varying struct
*/
    varying_struct_t_op(&varying_struct_val[1]);
    /* check for correct returned size */
    if (varying_struct_val[1].size != varying_array_size) printf("Error in varying_struct_val[1].size\n");
    /* Check the existing values */
    for (i = 0; i < varying_array_size; i++)
	if (varying_struct_val[1].arr[i] != i) printf("Error in varying_struct_val[1].arr[]\n");
    /* Make sure all of the space is allocated */
    for (i = 0; i < fixed_array_size; i++) varying_struct_val[1].arr[i] = i;


/* 
** returning conformant struct
*/
    conformant_struct_val[1].size = conformant_array_size;
    conformant_struct_t_op(&conformant_struct_val[1]);
    /* check for correct returned size */
    if (conformant_struct_val[1].size != conformant_array_size) printf("Error in conformant_struct_val[1].size\n");
    /* Check the existing values */
    for (i = 0; i < conformant_array_size; i++)
	if (conformant_struct_val[1].arr[i] != i) printf("Error in conformant_struct_val[1].arr[]\n");



/* 
** varying and conformant struct
*/
    varying_conformant_struct_val[1].size = conformant_array_size;
    varying_conformant_struct_val[1].used = conformant_array_size;
    varying_conformant_struct_t_op(&varying_conformant_struct_val[1]);
    /* check for correct returned size */
    if (varying_conformant_struct_val[1].size != conformant_array_size) printf("Error in varying_conformant_struct_val[1].size\n");
    /* check for correct returned used size */
    if (varying_conformant_struct_val[1].used != varying_array_size) printf("Error in varying_conformant_struct_val[1].used\n");
    /* Check the existing values */
    for (i = 0; i < varying_array_size; i++)
	if (varying_conformant_struct_val[1].arr[i] != i) 
          printf("Error in varying_conformant_struct_val[1].arr[]\n");
    /* Check the existing values */
    for (i = varying_array_size; i < conformant_array_size; i++)
	if (varying_conformant_struct_val[1].arr[i] != 0xAAAAAAAA) 
              printf("Error in varying_conformant_struct_val[1].arr[], unused area changed\n");

 
/*
** Test completed, this should be the only message on a successful test
*/
    printf("Test complete\n");
    return 0;
}
