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
 * Revision 1.1.6.2  1996/02/17  23:16:16  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:44  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:27:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:49  root]
 * 
 * Revision 1.1.4.1  1993/10/14  17:52:38  ganni
 * 	OT 7426 - malloc the buffers to avoid the alignment problems.
 * 	[1993/10/14  17:51:52  ganni]
 * 
 * Revision 1.1.2.3  1993/01/13  21:49:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:39:31  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <pipetest2.h>     
#include <test_common.h>

typedef unsigned char byte;

#define MANAGER_BUFFER_SIZE 2000
#define MANAGER_ELEMENT_COUNT 999

extern char *filename;

void his_oper2(op_float,iop_struct,ip_float,correct_to_server)
    /* [out] */pipe_of_float *op_float;
    /* [in, out] */pipe_of_struct *iop_struct;
    /* [in] */pipe_of_float *ip_float;
    /* [out] */ndr_boolean *correct_to_server;
{
    unsigned long data_count;
    pt2_struct iop_checksums;
    pt2_struct *struct_array;
    float ip_checksum;
    float *float_array;
    int i,j;
    unsigned long *p_as_word;
    ndr_long_float wk_double;
    ndr_short_float wk_single;

#ifdef DEBUG
    printf("Entered his_oper2\n");
#endif
    *correct_to_server = true;
    iop_checksums.pt2_long = 0;
    iop_checksums.pt2_double = 0.0;
    struct_array = (pt2_struct *)malloc(MANAGER_BUFFER_SIZE);
    j = 1;
    while (true)
    {
        (*iop_struct->pull)(iop_struct->state, struct_array,
                            MANAGER_BUFFER_SIZE/sizeof(pt2_struct),
                            &data_count);
        if (data_count == 0)
        {
#ifdef DEBUG
            printf("Manager checksums for iop_struct: %d %.1f\n",
               iop_checksums.pt2_long, iop_checksums.pt2_double);
#endif
              if ( iop_checksums.pt2_long != 1498500 ) {
                    *correct_to_server = false;
                    printf("Error in Manager checksums for iop_struct: %d %.1f\n",
                       iop_checksums.pt2_long, iop_checksums.pt2_double);
                    }
              if ( iop_checksums.pt2_double != 1998000.0 ) {
                    *correct_to_server = false;
                    printf("Error in Manager checksums for iop_struct: %d %.1f\n",
                       iop_checksums.pt2_long, iop_checksums.pt2_double);
                    }
            break;
        }
        for (i=0; i<data_count; i++)
        {
            if (struct_array[i].pt2_long != j*3)
            {
                printf("j = %d, pt2_long = %d\n", j, struct_array[i].pt2_long);
            }
            wk_double = j*4.0;
            if (wk_double != struct_array[i].pt2_double)
            {
                printf("j = %d, wk_double = %.8f, pt2_double = %.8f\n", 
                       j, wk_double, struct_array[i].pt2_double);
                p_as_word = (unsigned long *)&struct_array[i].pt2_double;
                printf("%08x %08x should be ", *p_as_word, *(p_as_word+1));
                p_as_word = (unsigned long *)&wk_double;
                printf("%08x %08x\n", *p_as_word, *(p_as_word+1));
            }
            iop_checksums.pt2_long += struct_array[i].pt2_long;
            iop_checksums.pt2_double += struct_array[i].pt2_double;
            j++;
        }
    }

    ip_checksum = 0.0 ;
    float_array = (float *)malloc(MANAGER_BUFFER_SIZE);
    j=1;
    while (true)
    {
        (*ip_float->pull)(ip_float->state,float_array,
                            MANAGER_BUFFER_SIZE/sizeof(float),
                            &data_count);
        if (data_count == 0)
        {
#ifdef DEBUG
        printf("Manager checksum for ip_float: %.1f\n", ip_checksum); 
#endif
              if ( ip_checksum != 999000.0 ) {
                    *correct_to_server = false;
                    printf("Error in Manager checksum for ip_float: %.1f\n", ip_checksum); 
                    }
            break;
        }
        for (i=0; i<data_count; i++)
        {
            wk_single = j * 2.0;
            if (float_array[i] != wk_single)
            {
                printf("j = %d, float = %.8f\n", j, float_array[i]);
                p_as_word = (unsigned long *)&float_array[i];
                printf("%08x should be ",*p_as_word);
                p_as_word = (unsigned long *)&wk_single;
                printf("%08x\n",*p_as_word);
            }
            ip_checksum += float_array[i];
            j++;
        }
    }

#ifdef DEBUG
    printf("In pipes processed; No doing out pipe\n");
#endif
    data_count = MANAGER_BUFFER_SIZE/sizeof(float);
    j=1;
    while (true)
    {
        for (i=0; i<data_count; i++)
        {
            float_array[i] = (float)(4*j);
            j++;
            if (j > MANAGER_ELEMENT_COUNT) 
            {
                i++;
                break;
            }
        }
        (*op_float->push)(op_float->state, float_array,i);
         if (j > MANAGER_ELEMENT_COUNT) break;
    }
    (*op_float->push)(op_float->state,float_array,0);

    data_count = MANAGER_BUFFER_SIZE/sizeof(pt2_struct);
    j=1;
    while (true)
    {
        for (i=0; i<data_count; i++)
        {
            struct_array[i].pt2_long = 5*j;
            struct_array[i].pt2_double = (double)(6*j);
            j++;
            if (j > MANAGER_ELEMENT_COUNT)
            {
                i++;
                break;
            }
        }
        (*iop_struct->push)(iop_struct->state,struct_array,i);
         if (j > MANAGER_ELEMENT_COUNT) break;
    }
    (*iop_struct->push)(iop_struct->state,struct_array,0);

    free(float_array);
    free(struct_array);
}

globaldef pipetest2_v0_0_epv_t pipetest2_v0_0_m_epv = {his_oper2};
