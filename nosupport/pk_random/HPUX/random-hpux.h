/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
struct random_stats
{
    int poolbits;
    int deferredsize;
    int ent_stored;
    int entropy_in;
    int entropy_out;
    int input_samples;
    int waiters;
    int deferred_last;
    int spare1;
    int spare2;
    int spare3;
    int spare4;
};




#define RNDGETENTCNT	_IOR('r', 1, int)
#define RNDADDTOENTCNT  _IOW('r', 2, int)
#define RNDZAPENTCNT    _IO('r', 3)
#if 0
#define RNDGETPOOL	
#define RNDADDENTROPY
#endif
#define RNDCLEARPOOL	_IO('r', 6)
#define RNDGETSTATS	_IOR('r', 7, struct random_stats)
