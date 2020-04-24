/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: base.h,v $
 * Revision 1.1.4.2  1996/02/18  23:45:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:42  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:34:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:17:30  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:14:21  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:46:57  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:14:13  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:10:11  zeliff]
 * 
 * Revision 1.1  1992/01/19  02:59:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef base__included
#define base__included
#include <idl_base.h>
#include <nbase.h>
#include <timebase.h>
typedef ndr_$long_int status_$all_t;
#define proc1_$n_user_processes 56
#define name_$long_complen_max 255
#define name_$long_pnamlen_max 1023
#define name_$pnamlen_max 256
#define name_$complen_max 32
typedef ndr_$char name_$pname_t[256];
typedef ndr_$char name_$name_t[32];
typedef ndr_$char name_$long_name_t[256];
typedef ndr_$char name_$long_pname_t[1024];
#define ios_$max 127
#define ios_$stdin 0
#define ios_$stdout 1
#define ios_$stderr 2
#define ios_$errin 2
#define ios_$errout 2
#define stream_$stdin ios_$stdin
#define stream_$stdout ios_$stdout
#define stream_$stderr ios_$stderr
#define stream_$errin stream_$stderr
#define stream_$errout stream_$stderr
typedef ndr_$short_int ios_$id_t;
typedef struct ios_$seek_key_t ios_$seek_key_t;
struct ios_$seek_key_t {
ndr_$long_int rec_adr;
ndr_$long_int byte_adr;
};
typedef ndr_$short_int stream_$id_t;
typedef struct uid_$t uid_$t;
struct uid_$t {
ndr_$long_int high;
ndr_$long_int low;
};
#ifdef __STDC__
handle_t uid_$t_bind(uid_$t h);
void uid_$t_unbind(uid_$t uh, handle_t h);
#else
handle_t uid_$t_bind();
void uid_$t_unbind();
#endif
typedef struct xoid_$t xoid_$t;
struct xoid_$t {
ndr_$long_int rfu1;
ndr_$long_int rfu2;
uid_$t uid;
};
typedef struct ec2_$eventcount_t ec2_$eventcount_t;
struct ec2_$eventcount_t {
ndr_$long_int value;
pinteger awaiters;
};
typedef ec2_$eventcount_t *ec2_$ptr_t;
#endif
