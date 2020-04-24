/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pipe_mgr.c,v $
 * Revision 1.1.10.2  1996/03/09  20:51:14  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:04  marty]
 *
 * Revision 1.1.10.1  1995/12/13  21:55:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:13  root]
 * 
 * Revision 1.1.6.2  1993/07/09  12:48:42  root
 * 	Initial King Kong branch
 * 	[1993/07/09  12:48:16  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:29:07  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:55:36  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:02:56  mcfarland]
 * 
 * 	Reorganize util routines
 * 	[1992/04/15  16:00:03  rps]
 * 
 * Revision 1.1.1.2  1992/05/28  15:13:36  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:36:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <pipe.h>     
#include <util.h>

typedef unsigned char byte;

#define MANAGER_BUFFER_SIZE 2000
byte manager_buffer[MANAGER_BUFFER_SIZE];
#define MANAGER_ELEMENT_COUNT 999

extern char *filename, buf[128], header[512];

void my_oper2(h, op_float,iop_struct,ip_float,correct_to_server)
    /* [in] */ handle_t h;
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
    char *func = "my_oper2";

#ifdef DEBUG
    sprintf(buf, "(%s):  Entered his_oper2\n", func);
    xx_log(buf);
#endif
    *correct_to_server = true;
   iop_checksums.pt2_long = 0;
   iop_checksums.pt2_double = 0.0;
    struct_array = (pt2_struct *)manager_buffer;
    j = 1;
    while (true)
    {
        (*iop_struct->pull)(iop_struct->state, struct_array,
                            MANAGER_BUFFER_SIZE/sizeof(pt2_struct),
                            &data_count);
        if (data_count == 0)
        {
#ifdef DEBUG
            sprintf(buf, "(%s):  Manager checksums for iop_struct: %d %.1f\n",
               func, iop_checksums.pt2_long, iop_checksums.pt2_double);
	    xx_log(buf);
#endif
              if ( iop_checksums.pt2_long != 1498500 ) {
                    *correct_to_server = false;
                    sprintf(buf, "(%s):  Error in Manager checksums for iop_struct: %d %.1f\n",
                       func, iop_checksums.pt2_long, iop_checksums.pt2_double);
		    xx_error(buf);
                    }
              if ( iop_checksums.pt2_double != 1998000.0 ) {
                    *correct_to_server = false;
                    sprintf(buf, "(%s):  Error in Manager checksums for iop_struct: %d %.1f\n",
                       func, iop_checksums.pt2_long, iop_checksums.pt2_double);
		    xx_error(buf);
                    }
            break;
        }
        for (i=0; i<data_count; i++)
        {
            if (struct_array[i].pt2_long != j*3)
            {
                sprintf(buf, "(%s):  j = %d, pt2_long = %d\n", func, j, 
		   	struct_array[i].pt2_long);
		xx_log(buf);
            }
            wk_double = j*4.0;
            if (wk_double != struct_array[i].pt2_double)
            {
                sprintf(buf, "(%s):  j = %d, wk_double = %.8f, pt2_double = %.8f\n", 
                       func, j, wk_double, struct_array[i].pt2_double);
		xx_log(buf);
                p_as_word = (unsigned long *)&struct_array[i].pt2_double;
                sprintf(buf, "(%s):  %08x %08x should be ", func,*p_as_word, 
			*(p_as_word+1));
		xx_log(buf);
                p_as_word = (unsigned long *)&wk_double;
                sprintf(buf, "(%s):  %08x %08x\n", func, *p_as_word, 
			*(p_as_word+1));
		xx_log(buf);
            }
            iop_checksums.pt2_long += struct_array[i].pt2_long;
            iop_checksums.pt2_double += struct_array[i].pt2_double;
            j++;
        }
    }

    ip_checksum = 0.0 ;
    float_array = (float *)manager_buffer;
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
        (*op_float->push)(op_float->state,(float *)manager_buffer,i);
         if (j > MANAGER_ELEMENT_COUNT) break;
    }
    (*op_float->push)(op_float->state,(float *)manager_buffer,0);

    struct_array = (pt2_struct *)manager_buffer;
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
        (*iop_struct->push)(iop_struct->state,(pt2_struct *)manager_buffer,i);
         if (j > MANAGER_ELEMENT_COUNT) break;
    }
    (*iop_struct->push)(iop_struct->state,(pt2_struct *)manager_buffer,0);


}

globaldef pipetest2_v0_0_epv_t pipetest2_v0_0_m_epv = {my_oper2};
