/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
#ifndef RANDOM_POOL_H
#define RANDOM_POOL_H 1

#define POOLWORDS 128    /* Power of 2 - note that this is 32-bit words */
#define POOLBITS (POOLWORDS*32)

#ifndef DEFERREDSIZE
#define DEFERREDSIZE 1024
#endif

typedef unsigned int unsigned32; /* !!! */
typedef unsigned char unsigned8; /* !!! */

typedef struct rand_pool {
    int add_ptr;
    int entropy_estimate;
    int entropy_in;
    int entropy_out;    
    int input_samples;
    int waiters;
    unsigned int input_rotate;
    unsigned32 pool[POOLWORDS];
} rand_pool_t, *rand_pool_p_t;

typedef struct rand_src 
{
    unsigned32  last_val;
    int		last_delta;
    int		last_delta2;    
    int 	count;
} rand_src_t, *rand_src_p_t;

typedef struct rand_deferred_pool
{
    unsigned32 *ptr;
    unsigned32 *end;
    unsigned32 *start;
    unsigned32 *last;    
} rand_pool_deferred_t, *rand_pool_deferred_p_t;

extern void rand_pool_add_entropy(rand_pool_p_t r, unsigned32 input,
				  unsigned32 nbits);
extern int rand_pool_add_timer(rand_pool_p_t r,
			       rand_src_p_t rs,
			       unsigned32 input);
extern void rand_pool_extract_block(rand_pool_p_t r, unsigned32 *block);
extern int rand_pool_extract_entropy(rand_pool_p_t r, char *buf, int nbytes);
extern void rand_pool_init(rand_pool_p_t r);
extern void rand_src_init(rand_src_p_t r);

extern void rand_pool_deferred_add(unsigned32 input);
extern void rand_pool_deferred_process(void);
extern void rand_pool_deferred_init(unsigned32 *buf, unsigned32 *end);

#endif
