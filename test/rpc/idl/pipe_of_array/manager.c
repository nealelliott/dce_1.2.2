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
 * Revision 1.1.4.2  1996/02/17  23:16:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:32  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:35  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:38:36  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:38:27  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <pipe_of_array.h>     

typedef unsigned char byte;

#define MANAGER_BUFFER_SIZE 2000
byte manager_buffer[MANAGER_BUFFER_SIZE];
#define MANAGER_ELEMENT_COUNT 100

void his_op0(h, p_in_pipe, p_manager_checksum, p_out_pipe )
    handle_t h;
    pipe_t *p_in_pipe;
    long *p_manager_checksum;
    pipe_t *p_out_pipe;
{
    unsigned long data_count;
    array_t *array_array;
    int j,k;
    unsigned int i;

    *p_manager_checksum = 0;
    array_array = (array_t *)manager_buffer;
    while (true)
    {
        (*p_in_pipe->pull)(p_in_pipe->state, array_array,
                            MANAGER_BUFFER_SIZE/sizeof(array_t),
                            &data_count);
        if (data_count == 0)
        {
#ifdef DEBUG
            printf("Manager *p_manager_checksum %d\n", *p_manager_checksum);
#endif
            break;
        }
        for (i=0; i<data_count; i++)
        {
            for(j=0; j<ARRAYSIZE; j++)
                *p_manager_checksum += array_array[i][j];
        }
    }

    array_array = (array_t *)manager_buffer;
    data_count = MANAGER_BUFFER_SIZE/sizeof(array_t);
    j=1;
    while (true)
    {
        for (i=0; i<data_count; i++)
        {
            for (k=0; k<ARRAYSIZE/2; k++)
                array_array[i][k] = j * 100 + k;
            for (k=ARRAYSIZE/2; k<ARRAYSIZE; k++)
                array_array[i][k] = j * 100 - k + ARRAYSIZE / 2;
            j++;
            if (j > MANAGER_ELEMENT_COUNT)
            {
                i++;
                break;
            }
        }
        (*p_out_pipe->push)(p_out_pipe->state,(array_t *)manager_buffer,i);
         if (j > MANAGER_ELEMENT_COUNT) break;
    }
    (*p_out_pipe->push)(p_out_pipe->state,(array_t *)manager_buffer,0);
}

void his_op1(h, p_in_pipe, p_manager_checksum, p_out_pipe )
    handle_t h;
    pipe2_t *p_in_pipe;
    long *p_manager_checksum;
    pipe2_t *p_out_pipe;
{
    unsigned long data_count;
    il_a_t *array_array;
    int i,j,k,l;

    *p_manager_checksum = 0;
    array_array = (il_a_t *)manager_buffer;
    while (true)
    {
        (*p_in_pipe->pull)(p_in_pipe->state, array_array,
                            MANAGER_BUFFER_SIZE/sizeof(array_t),
                            &data_count);
        if (data_count == 0)
        {
#ifdef DEBUG
            printf("Manager *p_manager_checksum %d\n", *p_manager_checksum);
#endif
            break;
        }
        for (i=0; i<data_count; i++)
        {
            for(j=0; j<ILASIZE; j++)
                for (k=0; k<OOLASIZE; k++)
                    *p_manager_checksum += array_array[i][j][k];
        }
    }

    array_array = (il_a_t *)manager_buffer;
    data_count = MANAGER_BUFFER_SIZE/sizeof(array_t);
    l=1;
    while (true)
    {
        for (i=0; i<data_count; i++)
        {
            for (j=0; j<ILASIZE/2; j++)
            {
                for (k=0; k<OOLASIZE; k++)
                    array_array[i][j][k] = l * 100 + j * OOLASIZE + k;
            }
            for (j=0; j<ILASIZE/2; j++)
            {
                for (k=0; k<OOLASIZE; k++)
                    array_array[i][j+ILASIZE/2][k] = l * 100 - j * OOLASIZE - k;
            }
            l++;
            if (l > MANAGER_ELEMENT_COUNT)
            {
                i++;
                break;
            }
        }
        (*p_out_pipe->push)(p_out_pipe->state, (il_a_t *)manager_buffer, i);
         if (l > MANAGER_ELEMENT_COUNT) break;
    }
    (*p_out_pipe->push)(p_out_pipe->state,(il_a_t *)manager_buffer,0);
}

globaldef pipe_of_array_v0_0_epv_t pipe_of_array_v0_0_m_epv = {his_op0,his_op1};
