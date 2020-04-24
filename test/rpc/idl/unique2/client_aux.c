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
 * Revision 1.1.4.2  1996/02/17  23:18:17  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:15  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:39  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:30:18  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:17:31  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <test_common.h>
#include <unique2.h>

static void insert();
static void free_all();
static int traverse();
static int left();
static int right();

idl_short_int fixed_non_null_array[NELEMENTS];
idl_short_int *conf_non_null_array;

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    this_way_t this_way;
    that_way_t that_way;
    char buf[100];
    tree_t *root, *new;
    int i, j, n, failures = 0;
    idl_long_int s, l, c;
   largenode *p_l, *new_l;
   smallnode *p_s, *new_s;
    idl_long_int tmp, length, size, checksum, check2;
    char *getenvres;


    this_way.p_boolean = (idl_boolean *)malloc(sizeof(idl_boolean));
    *this_way.p_boolean = true;
    this_way.p_byte = (idl_byte *)malloc(sizeof(idl_byte));
    *this_way.p_byte = 'A';
    this_way.p_char = (idl_char *)malloc(sizeof(idl_char));
    *this_way.p_char = 'Z';
    this_way.p_small = (idl_small_int *)malloc(sizeof(idl_small_int));
    *this_way.p_small = 0x11;
    this_way.p_short = (idl_short_int *)malloc(sizeof(idl_short_int));
    *this_way.p_short = 0x2222;
    this_way.p_long = (idl_long_int *)malloc(sizeof(idl_long_int));
    *this_way.p_long = 0x33333333;
    this_way.p_hyper = (ndr_hyper_int *)malloc(sizeof(ndr_hyper_int));
    (*this_way.p_hyper).high = 0x44444444;
    (*this_way.p_hyper).low = 0x55555555;
    this_way.p_usmall = (idl_usmall_int *)malloc(sizeof(idl_usmall_int));
    *this_way.p_usmall = 0x66;
    this_way.p_ushort = (idl_ushort_int *)malloc(sizeof(idl_ushort_int));
    *this_way.p_ushort = 0x7777;
    this_way.p_ulong = (idl_ulong_int *)malloc(sizeof(idl_ulong_int));
    *this_way.p_ulong = 0x88888888;
    this_way.p_uhyper = (ndr_uhyper_int *)malloc(sizeof(ndr_uhyper_int));
    (*this_way.p_uhyper).high = 0x99999999;
    (*this_way.p_uhyper).low = 0xAAAAAAAA;
    this_way.p_float = (idl_short_float *)malloc(sizeof(idl_short_float));
    *this_way.p_float = 1.2345;
    this_way.p_double = (idl_long_float *)malloc(sizeof(idl_long_float));
    *this_way.p_double = 0.9876;
    this_way.p_primary = (primary *)malloc(sizeof(primary));
    *this_way.p_primary = red;

    that_way.p_boolean = NULL;
    that_way.p_byte = NULL;
    that_way.p_char = NULL;
    that_way.p_small = NULL;
    that_way.p_short = NULL;
    that_way.p_long = NULL;
    that_way.p_hyper = NULL;
    that_way.p_usmall = NULL;
    that_way.p_ushort = NULL;
    that_way.p_ulong = NULL;
    that_way.p_uhyper = NULL;
    that_way.p_float = NULL;
    that_way.p_double = NULL;
    that_way.p_primary = NULL;

   new_oper ( h, &this_way, &that_way );

    if (*this_way.p_boolean != *that_way.p_boolean)
    {
        printf("Error in p_boolean\n");
        failures++;
    }
    if (*this_way.p_byte != *that_way.p_byte)
    {
        printf("Error in p_byte\n");
        failures++;
    }
    if (*this_way.p_char != *that_way.p_char)
    {
        printf("Error in p_char\n");
        failures++;
    }
    if (*this_way.p_small != *that_way.p_small)
    {
        printf("Error in p_small\n");
        failures++;
    }
    if (*this_way.p_short != *that_way.p_short)
    {
        printf("Error in p_short\n");
        failures++;
    }
    if (*this_way.p_long != *that_way.p_long)
    {
        printf("Error in p_long\n");
        failures++;
    }
    if ((*this_way.p_hyper).low != (*that_way.p_hyper).low)
    {
        printf("Error in p_hyper.low\n");
        failures++;
    }
    if ((*this_way.p_hyper).high != (*that_way.p_hyper).high)
    {
        printf("Error in p_hyper.high\n");
        failures++;
    }
    if (*this_way.p_usmall != *that_way.p_usmall)
    {
        printf("Error in p_usmall\n");
        failures++;
    }
    if (*this_way.p_ushort != *that_way.p_ushort)
    {
        printf("Error in p_ushort\n");
        failures++;
    }
    if (*this_way.p_ulong != *that_way.p_ulong)
    {
        printf("Error in p_ulong\n");
        failures++;
    }
    if ((*this_way.p_uhyper).low != (*that_way.p_uhyper).low)
    {
        printf("Error in p_uhyper.low\n");
        failures++;
    }
    if ((*this_way.p_uhyper).high != (*that_way.p_uhyper).high)
    {
        printf("Error in p_uhyper.high\n");
        failures++;
    }
    if (*this_way.p_float != *that_way.p_float)
    {
        printf("Error in p_float\n");
        failures++;
    }
    if (*this_way.p_double != *that_way.p_double)
    {
        printf("Error in p_double\n");
        failures++;
    }
    if (*this_way.p_primary != *that_way.p_primary)
    {
        printf("Error in p_primary\n");
        failures++;
    }

    root = (tree_t *)malloc(sizeof(tree_t));
    root->key = 0;
    root->left = root->right = NULL;

    n = random()%2000;

#ifdef DEBUG
    printf("Building a binary tree with %d nodes\n", n + 1);
#endif

    for (i = 0; i < n; i++)
    {
        new = (tree_t *)malloc(sizeof(tree_t));
        new->key = random(); new->key <<= 1;
        new->left = new->right = NULL;
        insert(root, new);
    }

    s = left(root);
    l = right(root);
    c = traverse(root);
#ifdef DEBUG
    printf("Smallest is 0x%lx, largest is 0x%lx, checksum is 0x%lx\n",
            s, l, c);
#endif

    printf("Calling op1\n");
    failures += op1(h, root, s, l, c);

    free_all(root);

    p_l = NULL;
    for (i=1; i<10; i++)
    {
        new_l = (largenode *)malloc(sizeof(largenode)+(i-1)*sizeof(int ));
        new_l->next = p_l;
        new_l->array_size = i;
        for (j=0; j<i; j++) new_l->array_data[j] = 100*i + j;
        p_l = new_l;
   }
    p_s = NULL;

   largetosmall ( h, p_l, &p_s );

    for( ; p_s != NULL; p_s = p_s->next)
    {
        printf( "%d array elements ", (p_s->array_max)+1 );
        for (j=0; j<=p_s->array_max; j++) printf(" %d", p_s->array_data[j] );
        printf( "\n" );
    }

    p_s = NULL;
    for (i=1; i<10; i++)
    {
        new_s = (smallnode *)malloc(sizeof(smallnode)+(i-1)*sizeof(int ));
        new_s->next = p_s;
        new_s->array_max = i - 1;
        for (j=0; j<i; j++) new_s->array_data[j] = 100*i + j;
        p_s = new_s;
   }
    p_l = NULL;

    smalltolarge ( h, p_s, &p_l );

    for( ; p_l != NULL; p_l = p_l->next)
    {
        printf( "%d array elements ", p_l->array_size);
        for (j=0; j<p_l->array_size; j++) printf(" %d", p_l->array_data[j] );
        printf( "\n" );
    }

    checksum = 0;
    for (j = 0; j < NELEMENTS; j++)
        checksum += fixed_non_null_array[j] = (random()&0x7fff);

    printf("Calling null_in_array()\n");
    if (!null_in_array (h, NULL)) {
        printf("!null_in_array (h, NULL)\n");
        failures++;
    }

    printf("Calling non_null_in_array()\n");
    if (!non_null_in_array (h, fixed_non_null_array, checksum)) {
        printf("!non_null_in_array (h, fixed_non_null_array)\n");
        failures++;
    }

    printf("Calling null_in_out_array()\n");
    if (!null_in_out_array (h, NULL)) {
        printf("!null_in_out_array (h, NULL)\n");
        failures++;
    }

    printf("Calling non_null_in_out_array()\n");
    if (!non_null_in_out_array (h, fixed_non_null_array, &checksum)) {
        printf("!non_null_in_out_array (h, fixed_non_null_array)\n");
        failures++;
    }
    else {
        check2 = 0;
        for (j = 0; j < NELEMENTS; j++) check2 += fixed_non_null_array[j];
 
        if (check2 != checksum) {
            printf("after non_null_in_out_array: check2 != checksum\n");
            failures++;
        }
    }

    length = random()%NELEMENTS;

    checksum = 0;
    for (j = 0; j < length; j++) 
          checksum += fixed_non_null_array[j] = (random()&0x7fff);

#ifdef DEBUG
    printf("Calling varying_in_out_array(length = %d)\n", length);
#endif
    if (!varying_in_out_array(h, fixed_non_null_array, &length, &checksum)) {
        printf("!varying_in_out()\n");
        failures++;
    }
    else {
        check2 = 0;
        for (j = 0; j < length; j++) check2 += fixed_non_null_array[j];
        if (check2 != checksum) {
            printf("after varying_in_out_array: check2 != checksum\n");
            failures++;
        }
    }

    size = random()%(2*NELEMENTS);
    conf_non_null_array = (idl_short_int *)malloc(size*sizeof(idl_short_int));

    checksum = 0;
    for (j = 0; j < size; j++) checksum += conf_non_null_array[j] = (random()&0x7fff);

#ifdef DEBUG
    printf("Calling conformant_in_out_array(size = %d)\n", size);
#endif
    if (!conformant_in_out_array(h, conf_non_null_array, size, &checksum)) {
        printf("!conformant_in_out()\n");
        failures++;
    }
    else {
        check2 = 0;
        for (j = 0; j < size; j++) check2 += conf_non_null_array[j];
        if (check2 != checksum) {
            printf("after conformant_in_out_array: check2 != checksum\n");
            failures++;
        }
    }

    free(conf_non_null_array);

    size = random()%(2*NELEMENTS);
    conf_non_null_array = (idl_short_int *)malloc(size*sizeof(idl_short_int));
    length = random()%size;

    checksum = 0;
    for (j = 0; j < length; j++) checksum += conf_non_null_array[j] = (random()&0x7fff);

#ifdef DEBUG
    printf("Calling conformant_varying_in_out_array(length = %d, size = %d)\n", 
           length, size);
#endif
    if (!conformant_varying_in_out_array(h, conf_non_null_array, &length, size, 
        &checksum)) {
        printf("!conformant_in_out_array()\n");
        failures++;
    }

    else {
        check2 = 0;
        for (j = 0; j < length; j++) check2 += conf_non_null_array[j];
        if (check2 != checksum) {
            printf("after conformant_varying_in_out_array: check2 != checksum\n");
            failures++;
        }
    }

    free(conf_non_null_array);

    return failures; 
}

static void free_all(tree)
    tree_t *tree;
{
    if (tree == NULL) return;
    free_all(tree->left);
    free_all(tree->right);
    free(tree);
}

static void insert(root, node)
    tree_t *root;
    tree_t *node;
{
    if (node->key <= root->key)
    {
        if (root->left) insert(root->left, node);
        else root->left = node;
    }
    else if (root->right) insert(root->right, node);
    else root->right = node;
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
    while(tree->left) tree = tree->left;
    return tree->key;
}

static int right(tree)
    tree_t *tree;
{
    while (tree->right) tree = tree->right;
    return tree->key;
}
