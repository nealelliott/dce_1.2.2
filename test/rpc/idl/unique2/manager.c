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
 * Revision 1.1.4.2  1996/02/17  23:18:19  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:16  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:40  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:30:23  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:17:35  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <test_common.h>
#include <unique2.h>

void his_new_oper (h, p_this_way, p_that_way)
    /* [in] */handle_t h;
    /* [in] */this_way_t *p_this_way;
    /* [out] */that_way_t *p_that_way;
{ 
    p_that_way->p_boolean = (idl_boolean *)rpc_ss_allocate(sizeof(idl_boolean));
    *p_that_way->p_boolean = *p_this_way->p_boolean;
    p_that_way->p_byte = (idl_byte *)rpc_ss_allocate(sizeof(idl_byte));
    *p_that_way->p_byte = *p_this_way->p_byte;
    p_that_way->p_char = (idl_char *)rpc_ss_allocate(sizeof(idl_char));
    *p_that_way->p_char = *p_this_way->p_char;
    p_that_way->p_small = (idl_small_int *)rpc_ss_allocate(sizeof(idl_small_int));
    *p_that_way->p_small = *p_this_way->p_small;
    p_that_way->p_short = (idl_short_int *)rpc_ss_allocate(sizeof(idl_short_int));
    *p_that_way->p_short = *p_this_way->p_short;
    p_that_way->p_long = (idl_long_int *)rpc_ss_allocate(sizeof(idl_long_int));
    *p_that_way->p_long = *p_this_way->p_long;
    p_that_way->p_hyper = (ndr_hyper_int *)rpc_ss_allocate(sizeof(ndr_hyper_int));
    (*p_that_way->p_hyper).high = (*p_this_way->p_hyper).high;
    (*p_that_way->p_hyper).low = (*p_this_way->p_hyper).low;
    p_that_way->p_usmall = (idl_usmall_int *)rpc_ss_allocate(sizeof(idl_usmall_int));
    *p_that_way->p_usmall = *p_this_way->p_usmall;
    p_that_way->p_ushort = (idl_ushort_int *)rpc_ss_allocate(sizeof(idl_ushort_int));
    *p_that_way->p_ushort = *p_this_way->p_ushort;
    p_that_way->p_ulong = (idl_ulong_int *)rpc_ss_allocate(sizeof(idl_ulong_int));
    *p_that_way->p_ulong = *p_this_way->p_ulong;
    p_that_way->p_uhyper = (ndr_uhyper_int *)rpc_ss_allocate(sizeof(ndr_uhyper_int));
    (*p_that_way->p_uhyper).high = (*p_this_way->p_uhyper).high;
    (*p_that_way->p_uhyper).low = (*p_this_way->p_uhyper).low;
    p_that_way->p_float = (idl_short_float *)rpc_ss_allocate(sizeof(idl_short_float));
    *p_that_way->p_float = *p_this_way->p_float;
    p_that_way->p_double = (idl_long_float *)rpc_ss_allocate(sizeof(idl_long_float));
    *p_that_way->p_double = *p_this_way->p_double;
    p_that_way->p_primary = (primary *)rpc_ss_allocate(sizeof(primary));
    *p_that_way->p_primary = *p_this_way->p_primary;
}

static int traverse();
static int left();
static int right();

static idl_small_int _op1
#ifdef NIDL_PROTOTYPES
(
    handle_t h,
    tree_t *tree,
    idl_long_int smallest,
    idl_long_int largest,
    idl_long_int checksum
)
#else
(h, tree, smallest, largest, checksum)
    handle_t h;
    tree_t *tree;
    idl_long_int smallest;
    idl_long_int largest;
    idl_long_int checksum;
#endif
{
    idl_small_int failures = 0;
    idl_long_int s, l, c;

    s = left(tree);
    l = right(tree);
    c = traverse(tree);

#ifdef DEBUG
    printf("In op1: smallest is 0x%lx, largest is 0x%lx, checksum is 0x%lx\n",
           s, l, c);
#endif

    if (s != smallest) failures++;
    if (l != largest) failures++;
    if (c != checksum) failures++;

    if (failures) printf("FAILURE: 0x%lx 0x%lx 0x%lx\n", smallest, largest, 
                         checksum);

    return failures;
}

static int traverse(tree)
    tree_t *tree;
{
    if (!tree) return 0;
    else return traverse(tree->left) + tree->key + traverse(tree->right);
}

static int left(tree)
    tree_t *tree;
{
    while (tree->left) tree = tree->left;
    return tree->key;
}

static int right(tree)
    tree_t *tree;
{
    while (tree->right) tree = tree->right;
    return tree->key;
}

void s_largetosmall
(h, p_l, p_s)
    /* [in] */handle_t h;
    /* [in] */largenode *p_l;
    /* [out] */smallnode **p_s;
{
    smallnode *new_s, *old_s;
    int j;

    old_s = NULL;
    for( ; p_l != NULL; p_l = p_l->next)
    {
#ifdef DEBUG
        printf( "%d array elements ", p_l->array_size );
#endif
        new_s = (smallnode *)rpc_ss_allocate(sizeof(smallnode)+
                                    (p_l->array_size-1)*sizeof(int ));
        new_s->next = old_s;
        new_s->array_max = p_l->array_size - 1;
        for (j=0; j<p_l->array_size; j++)
        {
#ifdef DEBUG
            printf(" %d", p_l->array_data[j] );
#endif
            new_s->array_data[j] = p_l->array_data[j];
        }
#ifdef DEBUG
        printf( "\n" );
#endif
        old_s = new_s;
    }
    *p_s = old_s;
}

void s_smalltolarge
( h, p_s, p_l )
    /* [in] */handle_t h;
    /* [in] */smallnode *p_s;
    /* [out] */largenode **p_l;
{
    largenode *new_l, *old_l;
    int j;

    old_l = NULL;
    for( ; p_s != NULL; p_s = p_s->next)
    {
#ifdef DEBUG
        printf( "%d array elements ", p_s->array_max+1 );
#endif
        new_l = (largenode *)rpc_ss_allocate(sizeof(largenode)+
                                    (p_s->array_max)*sizeof(int ));
        new_l->next = old_l;
        new_l->array_size = p_s->array_max+1;
        for (j=0; j<p_s->array_max+1; j++)
        {
#ifdef DEBUG
            printf(" %d", p_s->array_data[j] );
#endif
            new_l->array_data[j] = p_s->array_data[j];
        }
#ifdef DEBUG
        printf( "\n" );
#endif
        old_l = new_l;
    }
    *p_l = old_l;
}

static idl_boolean _null_in_array (h, a)
handle_t h;
idl_short_int a[NELEMENTS];
{
    idl_boolean answer;

    answer = (a == NULL);
    return answer;
}

static idl_boolean _non_null_in_array (h, a, checksum)
handle_t h;
idl_short_int a[NELEMENTS];
idl_long_int checksum;
{
    idl_boolean answer;
    idl_long_int check2 = 0;
    int i;

    if (!(answer = (a != NULL))) 
       ;
    else
    {
        for (i = 0; i < NELEMENTS; i++) check2 += a[i];
        if (answer = (check2 == checksum)) ;
        else printf("doesn't match\n");
    }

    return answer;
}

static idl_boolean _null_in_out_array (h, a)
handle_t h;
idl_short_int a[NELEMENTS];
{
    idl_boolean answer;

    answer = (a == NULL);
    return answer;
}

static idl_boolean _non_null_in_out_array  (h, a, checksum)
handle_t h;
idl_short_int a[NELEMENTS];
idl_long_int *checksum;
{
    idl_boolean answer;
    idl_long_int check2 = 0;
    int i;

    if (!(answer = (a != NULL))) 
        ;
    else
    {
        for (i = 0; i < NELEMENTS; i++) check2 += a[i];
        if (answer = (check2 == *checksum)) ;
        else printf("doesn't match\n");
       
        *checksum = 0;
        for (i = 0; i < NELEMENTS; i++) *checksum += a[i] = (random()&0x7fff);
    }

    return answer;
}

static idl_boolean _varying_in_out_array(h, a, length, checksum)
handle_t h;
idl_short_int a[NELEMENTS];
idl_long_int *length;
idl_long_int *checksum;
{
    idl_boolean answer;
    idl_long_int check2 = 0;
    int i;

    if (!(answer = (a != NULL))) 
        ;
    else
    {
        for (i = 0; i < *length; i++) check2 += a[i];
        if (answer = (check2 == *checksum)) ;
        else printf("doesn't match\n");
       
        *length = random()%NELEMENTS;
        *checksum = 0;
        for (i = 0; i < *length; i++) *checksum += a[i] = (random()&0x7fff);
    }

    return answer;
}

static idl_boolean _conformant_in_out_array(h, a, size, checksum)
handle_t h;
idl_short_int a[];
idl_long_int size;
idl_long_int *checksum;
{
    idl_boolean answer;
    idl_long_int check2 = 0;
    int i;

    if (!(answer = (a != NULL))) 
        ;
    else
    {
        for (i = 0; i < size; i++) check2 += a[i];
        if (answer = (check2 == *checksum)) ;
        else printf("doesn't match\n");
       
        *checksum = 0;
        for (i = 0; i < size; i++) *checksum += a[i] = (random()&0x7fff);
    }

    return answer;
}

static idl_boolean _conformant_varying_in_out_array(h, a, length, size, checksum)
handle_t h;
idl_short_int a[];
idl_long_int *length;
idl_long_int size;
idl_long_int *checksum;
{
    idl_boolean answer;
    idl_long_int check2 = 0;
    int i;

    if (!(answer = (a != NULL))) 
        ;
    else
    {
        for (i = 0; i < *length; i++) check2 += a[i];
        if (answer = (check2 == *checksum)) ;
        else printf("doesn't match\n");
       
        *length = random()%size;
        *checksum = 0;
        for (i = 0; i < *length; i++) *checksum += a[i] = (random()&0x7fff);
    }

    return answer;
}

/*globaldef*/ unique2_v0_0_epv_t unique2_v0_0_m_epv = {
    his_new_oper,
    _op1,
    s_largetosmall,
    s_smalltolarge,
    _null_in_array,
    _non_null_in_array,
    _null_in_out_array,
    _non_null_in_out_array,
    _varying_in_out_array,
    _conformant_in_out_array,
    _conformant_varying_in_out_array
};
