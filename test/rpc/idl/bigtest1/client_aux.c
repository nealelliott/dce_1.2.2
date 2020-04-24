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
 * Revision 1.1.4.2  1996/02/17  23:12:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:49  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:57:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:40  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:08:04  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:27:47  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <bigtest1.h>
#include <test_common.h>

void make_confstruct(p_p_confstruct,array_size)
confstruct_t **p_p_confstruct;
int array_size;
{
    int i;
    ndr_long_int *p_array_elt;

    *p_p_confstruct = (confstruct_t *)malloc
              ( sizeof(confstruct_t) + (array_size-1) * sizeof(ndr_long_int) );
    (*p_p_confstruct)->s = array_size;
    p_array_elt = (*p_p_confstruct)->a;
    for (i=0; i<array_size; i++)
    {
        *p_array_elt = 10 * i;
        p_array_elt++;
    }
}

void make_ptr_unions(p_varstruct)
varstruct_t *p_varstruct;
{
    int i;
    
    for (i=p_varstruct->f; i<=p_varstruct->l; i++)
    {
        p_varstruct->x[i].discrim = ptr_k;
        make_confstruct(&(p_varstruct->x[i].ub.ptr),i);
    }
}

void make_ptr_varstruct(p_p_varstruct,i,j)
varstruct_t **p_p_varstruct;
int i,j;
{
    *p_p_varstruct = (varstruct_t *)malloc(sizeof(varstruct_t));
    (*p_p_varstruct)->f = i;
    (*p_p_varstruct)->l = 4 * i + j;
    make_ptr_unions(*p_p_varstruct);
}

void make_count_unions(p_varstruct)
varstruct_t *p_varstruct;
{
    int i;
    
    for (i=p_varstruct->f; i<=p_varstruct->l; i++)
    {
        p_varstruct->x[i].discrim = count_k;
        p_varstruct->x[i].ub.count = i;
    }
}

void make_count_varstruct(p_p_varstruct,i,j)
varstruct_t **p_p_varstruct;
int i,j;
{
    *p_p_varstruct = (varstruct_t *)malloc(sizeof(varstruct_t));
    (*p_p_varstruct)->f = i;
    (*p_p_varstruct)->l = 4 * i + j;
    make_count_unions(*p_p_varstruct);
}

void print_confstruct(p_confstruct)
confstruct_t *p_confstruct;
{
    int i;

    printf("confstruct_t");
    for (i=0; i<p_confstruct->s; i++)
    {
        printf(" %d",p_confstruct->a[i]);
    }
    printf("\n");
}

void print_x_union(p_x_union)
x_union_t *p_x_union;
{
    printf("    x_union ");
    switch (p_x_union->discrim) {
        case ptr_k:
            print_confstruct(p_x_union->ub.ptr);
            break;
        case count_k:
            printf("count %d\n",p_x_union->ub.count);
            break;
        case sum_k:
            printf("sum %d\n",p_x_union->ub.sum);
            break;
    }
}

void print_varstruct(p_varstruct)
varstruct_t *p_varstruct;
{
    int i;
    
    for (i=p_varstruct->f; i<=p_varstruct->l; i++)
    {
        print_x_union(&(p_varstruct->x[i]));
    }
}

void print_array(name,array)
char *name;
varstruct_t *array[5][5];
{
    int i,j;
    
    for (i=0; i<5; i++)
    {
        for (j=0; j<5; j++)
        {
            printf("%s[%d][%d]\n",name,i,j);
            print_varstruct(array[i][j]);
        }
    }
}

int client_aux(h,passes)
      handle_t h;
{
      error_status_t st;
      varstruct_t *ptrtosum[5][5];
      varstruct_t *counttoptr[5][5];
      int i,j;


    for (i=0; i<5; i++)
    {
        for (j=0; j<5; j++)
        {
            make_ptr_varstruct(&ptrtosum[i][j],i,j);
            make_count_varstruct(&counttoptr[i][j],i,j);
        }
    }
/*
    print_array("ptrtosum",ptrtosum);
    print_array("counttoptr",counttoptr); 
*/
    op0( h, ptrtosum, counttoptr );
    print_array("ptrtosum",ptrtosum);
    print_array("counttoptr",counttoptr);
    return 0;
}
