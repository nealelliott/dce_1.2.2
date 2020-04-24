/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * TODO:
 * split pools for urandom vs random
 * implement select
 * implement rest of ioctl interface of Ted's driver.
 * need preemption point on read to avoid hosing system..
 * make MP-friendly?
 * use sleep_one/wait_one to avoid thundering herd??
 * back-port to 9.x S700 and S800 (ugh)
 *
 * revisit timeout processing for higher data-rates? (probably not).
 */

/*
 * random-hpux.c -- A strong random number generator.
 *
 * Portions derived from:
 * random.c -- A strong random number generator
 *
 * Version 0.97, last modified 24-Apr-96
 * Copyright Theodore Ts'o, 1994, 1995, 1996.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, and the entire permission notice in its entirety,
 *    including the disclaimer of warranties.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior
 *    written permission.
 * 
 */

#ifdef _KERNEL_BUILD
#include "../h/systm.h"
#include "../h/file.h"
#include "../h/user.h"
#include "../h/proc.h"
#include "../h/uio.h"
#include "../h/conf.h"
#include "../h/ioctl.h"
#include "../machine/int.h"
#else
#include <sys/systm.h>
#include <sys/file.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/uio.h>
#include <sys/conf.h>
#include <sys/conf.h>
#include <sys/ioctl.h>
#include <machine/int.h>
#endif

#include "random-pool.h"
#include "random-hpux.h"

static random_inited = 0;
static random_hooked = 0;
#define DBUF_SIZE 1024
static unsigned32 dbuf[DBUF_SIZE];

void random_hook_intr(void);

void random_oops_intr (int bit_num, struct save_state *ssp, int int_arg)
{
    printf("random_oops(%d, %08x, %08x)\n", bit_num, ssp, int_arg);
    panic("random_oops");
}

extern void (*random_eirr_switch[32])() = {
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
    random_oops_intr,
};

struct {
    void (**random_eirr_switch)();
    rand_pool_deferred_t dp;
} randomhookbase = {
    random_eirr_switch
};

#define dpool (randomhookbase.dp)


static rand_src_t gsrc;
rand_pool_t random_gpool;

#define gpool random_gpool

#include "random-dpool.c"

int random_open(dev_t dev) 
{
    if (!random_hooked)
	random_hook_intr();
    
    return 0;
}

#if 0
int random_close(dev_t dev)
{
}
#endif

#define RANDOM_WRITE_CHUNK 1024

#define RANDPOOL(dev) (&gpool)

#define RANDOM_NONBLOCKING(dev) (dev&1)

int random_read (dev_t dev, struct uio *uio)
{
    unsigned32 tmp[4];
    register rand_pool_p_t r = RANDPOOL(dev);

    while (uio->uio_resid > 0) {
	int nbytes = MIN(sizeof(tmp), uio->uio_resid);
	int error;
	
	if (!RANDOM_NONBLOCKING(dev)) {
	    while (r->entropy_estimate < 16*8) {
		r->waiters++;
		error = sleep(r, PCATCH|(PZERO+1));
		r->waiters--;
		if (error)
		    return EINTR; /* our caller will DTRT */
	    }
	}

	rand_pool_extract_block (r, tmp);

	error = uiomove(tmp, nbytes, UIO_READ, uio);
	if (error)
	    return error;
    }
    return 0;
}

int random_write (dev_t dev, struct uio *uio)
{
    char inbuf[RANDOM_WRITE_CHUNK];
    register rand_pool_p_t r = RANDPOOL(dev);
    
    while (uio->uio_resid > 0) {
	int nbytes = MIN(sizeof(inbuf), uio->uio_resid);
	int error, i;
	
	error = uiomove (inbuf, nbytes, UIO_WRITE, uio);
	if (error)
	    return error;

	for (i=0; i<nbytes; i++) {
	    rand_pool_add_entropy (r, inbuf[i], 0);
	}
    }
    return 0;
}

int random_ioctl (dev_t dev, int cmd, caddr_t data, int flag)
{
    int *iptr = (int *)data;
    struct random_stats *rp = (struct random_stats *)data;
    int ent_count;
    
    switch(cmd)
    {
    case RNDGETENTCNT:
	if (!suser())
	    return EPERM;
	*iptr = gpool.entropy_estimate;
	return 0;

    case RNDADDTOENTCNT:
	if (!suser())
	    return EPERM;
	ent_count = *iptr + gpool.entropy_estimate;
	if (ent_count < 0)
	    ent_count = 0;
	if (ent_count > POOLBITS)
	    ent_count = POOLBITS;
	gpool.entropy_estimate = ent_count;
	
	return 0;
    case RNDZAPENTCNT:
	if (!suser())
	    return EPERM;
	gpool.entropy_estimate = 0;
	return 0;

    case RNDGETSTATS:
	if (!suser())
	    return EPERM;
	rp->poolbits = POOLBITS;
	rp->deferredsize = sizeof(dbuf);
	rp->ent_stored = gpool.entropy_estimate;
	rp->entropy_in = gpool.entropy_in;
	rp->entropy_out = gpool.entropy_out;	
	rp->input_samples = gpool.input_samples;
	rp->waiters = gpool.waiters;
	rp->deferred_last = (dpool.last - dpool.start);
	return 0;
#if 0
    case RNDGETPOOL:
	break;
	
    case RNDADDENTROPY:
	break;
	
    case RNDCLEARPOOL:
#endif
    default:
	return EINVAL;
    }
}

#if 0
int random_select (dev_t fp, int which)
{
    switch(which) {
    case FREAD:
	break;
    case FWRITE:
	break;
    }
}
#endif

/* Declare the CDIO driver-specific fields */
/* Flags DRV_CHAR/DRV_BLOCK/DRV_PSEUDO/DRV_SCAN/DRV_MP_SAFE/DRV_SAFE_CONF */

static drv_info_t	random_info = {
    "random",	/* char *name	for device type (XXX random? XXX)*/
    "pseudo",	/* char *name	for device class */
    DRV_PSEUDO|DRV_CHAR,	/* ubit32 flags	pseudo? block? char? scan? */
    -1,		/* int b_major	maj dev# if block type */
    -1,		/* DYNAMIC; int c_major	maj dev# if char type */
    NULL,		/* struct cdio *cdio	drivers set to NULL */
    NULL,		/* void *gio_private	drivers set to NULL */
    NULL		/* void *cdio_private	drivers set to NULL */
};

/* Declare the driver entry points */
static drv_ops_t	random_ops = {
    random_open,	/* open <function pointer> */
    NULL, 		/* close */
    NULL,		/* strategy */
    NULL,		/* dump */
    NULL,		/* psize */
    NULL,		/* mount */
    random_read,	/* read */
    random_write,	/* write */
    random_ioctl,	/* ioctl */
    NULL,		/* select */
    NULL,		/* option_1 */
    NULL,		/* reserved1 */
    NULL,		/* reserved2 */
    NULL,		/* reserved3 */
    NULL,		/* reserved4 [<drv>_link] */
    0			/* flag */
};


extern void randomhook();

extern void int_illegal();
extern void nm_itmr_sync();
extern void ignore_int();
extern void sw_service();
extern void int_reserved();
extern void take_a_trap();
extern void clock_int();
extern void sampler();
extern void driver_handler();
extern void parallel_int();
extern void netisr();

extern void (*nothooked_fns[])() = {
    int_illegal,
    nm_itmr_sync,
    ignore_int,
    sw_service,
    int_reserved,
    take_a_trap,
    /* observed pseudo-devices */
    clock_int,
    sampler,
    driver_handler,
    parallel_int,
    netisr,
    NULL
};

/*
 * !!! HP-UX configures pseudo-devices before allocating interrupts for
 * real devices.
 *
 * This routine needs to run after the hardware has been configured
 * "for real", so we defer this until the first open() of /dev/random.
 *
 * This is unfortunate (we don't get to collect entropy during system boot)
 * but we're going to have to live with it for now.  A  workaround is 
 * echo foo >/dev/random early in startup..
 */

void random_hook_intr() 
{
    extern struct eirrswitch eirr_switch[];
    int i, j;
    void (*fn)();
    void (*nhfn)();
    
    for (i=3; i<19; i++) {
	fn = eirr_switch[i].int_action;
	for (j=0; nhfn = nothooked_fns[j]; j++) {
	    if (fn == nhfn)
		goto skip;
	}
	printf("random: replace eirr %d\n", i);
	if (fn == randomhook)
	    panic("random: double hook");
	random_eirr_switch[i] = fn;
	eirr_switch[i].int_action = randomhook;
      skip:
	if (i==3)
	    i=7;
	continue;
    }
    random_hooked = 1;
}

/* is the timeout handler really the right place/priority/etc to run this?? */
int random_deferred(caddr_t arg)
{
    rand_pool_deferred_process();
    if ((gpool.waiters) && (gpool.entropy_estimate >= 16*8))
	wakeup(&gpool);
    timeout (random_deferred, NULL, 1*HZ);    
}

int random_link(void) 
{
    if (random_inited)
	return 0;

    random_pool_deferred_init(dbuf, &dbuf[DBUF_SIZE]);
    timeout (random_deferred, NULL, 1*HZ);
    
    if (install_driver(&random_info, &random_ops) != 0) {
	return -1;
    } else {
	random_inited++;
	return 0;
    }
}
