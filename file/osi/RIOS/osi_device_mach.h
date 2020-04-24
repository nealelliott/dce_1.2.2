/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *      Copyright (C) 1991, 1990 Transarc Corporation
 *      All rights reserved.
 */

#ifndef TRANSARC_OSI_DEVICE_MACH_H
#define	TRANSARC_OSI_DEVICE_MACH_H

/* 
 * avoid user space assert conflict w/ assert.h and syspest.h
 */
#if !defined(_KERNEL) && defined(assert)
#undef assert
#endif

#include <sys/device.h>
#include <sys/sysmacros.h>
#include <sys/buf.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/user.h>

#ifdef _KERNEL
#define osi_inc_ru_oublock(n)	(u.u_ru.ru_oublock += (n))
#define osi_inc_ru_inblock(n)	(u.u_ru.ru_inblock += (n))
#endif

#ifdef _KERNEL
#define osi_strategy(bp)	(devstrat(bp), 0)
#else
extern int us_strategy(struct buf *bp);
#define osi_strategy(bp)	us_strategy(bp)
#endif /* _KERNEL */

/* 
 * Would you believe that at this point, we might have an assert defined
 * from syspest.h, and so we need to undef it again!
 *
 * XXX It might be better to use #error if assert is defined initially,
 * since undefining assert was probably not what the user hoped to achieve
 * by including this file.  What a mess.
 */
#if !defined(_KERNEL) && defined(assert)
#undef assert
#endif

#define osi_major(d)	major(d)
#define	osi_minor(d)	minor(d)
#define osi_makedev(maj, min)	makedev((maj), (min))

#endif /* TRANSARC_OSI_DEVICE_MACH_H */
