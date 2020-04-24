/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:23  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:15:20  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/29  19:16 UTC  tatsu_s
 * 	Merged Mothra upto DCE 1.1 bata.
 * 	[1995/12/11  19:48:15  root]
 * 
 * Revision 1.1.4.3  1993/09/20  18:02:20  tatsu_s
 * 	Fix DSDe411738 in project "hpux.commands"
 * 	Added perf_{f}printf wrappers for {f}printf.
 * 	(Temporary fix for printf() mutex lock problem in libc_r.)
 * 	[1993/07/28  14:12:05  tatsu_s]
 * 
 * Revision 1.1.4.2  1993/01/13  19:57:45  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:25:43  bbelch]
 * 
 * Revision 1.1.2.3  1992/06/11  12:29:35  mishkin
 * 	Add Zot type and a Zot-type object.  Clear up some other comments.
 * 	[1992/06/10  19:19:54  mishkin]
 * 
 * Revision 1.1.2.2  1992/05/01  15:41:53  rsalz
 * 	 06-aug-91 hsiao	Include <ctype.h> to resolve isdigit() for VMS.
 * 	 22-jul-91 mishkin   Add NilObj.
 * 	[1992/05/01  01:22:56  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:41:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      util.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Utility routines to support the performance and system exerciser
**  client and server.
**
**
*/

#include <dce/dce_error.h>
#include <perf_c.h>

#if defined(__hpux) && !defined(HPDCE_PRINTF_FIX)
#include <stdarg.h>
#endif /* HPDCE_PRINTF_FIX */

#ifdef VMS
#include <ctype.h>
#endif

/*
 * Define the Foo and Bar type UIDS.
 */
uuid_old_t FooType =
{
    0x3c4d28ff,
    0xe000,
    0x0000,
    0x0d,
    {0x00, 0x01, 0x34, 0x22, 0x00, 0x00, 0x00} 
};

uuid_old_t BarType =
{
    0x3c4d2909,
    0x1000,
    0x0000,
    0x0d,
    {0x00, 0x01, 0x34, 0x22, 0x00, 0x00, 0x00}
};

uuid_t ZotType =
{ 
    0x007dd408,
    0x05b0,
    0x1a36,
    0xac,
    0x7e,
    {0x02, 0x60, 0x8c, 0x2f, 0xec, 0xd0}
};

/*
 * Define a couple of objects of type Foo.
 */
uuid_old_t FooObj1 =
{
    0x3c4d2911,
    0x4000,
    0x0000,
    0x0d,
    {0x00, 0x01, 0x34, 0x22, 0x00, 0x00, 0x00}
};

uuid_old_t FooObj2 =
{
    0x3c4d2da7,
    0x1000,
    0x0000,
    0x0d,
    {0x00, 0x01, 0x34, 0x22, 0x00, 0x00, 0x00}   
};

/*
 * Define a couple of objects of type Bar.
 */
uuid_old_t BarObj1 =
{
    0x3c4d2dae,
    0x4000,
    0x0000,
    0x0d,
    {0x00, 0x01, 0x34, 0x22, 0x00, 0x00, 0x00}   
};

uuid_old_t BarObj2 =
{
    0x3c4d2919,
    0x7000,
    0x0000,
    0x0d,
    {0x00, 0x01, 0x34, 0x22, 0x00, 0x00, 0x00}   
};

/*
 * Define an object of type Zot.
 */
uuid_t ZotObj =
{
    0x0053e49a,
    0x05d3,
    0x1a36,
    0x8a,
    0xd8,
    {0x02, 0x60, 0x8c, 0x2f, 0xec, 0xd0}
};

/*
 * Define an object of type "nil".  (Not to be confused with the "nil" object.)
 */
uuid_t NilTypeObj =
{ /* 1c5b2910-33ab-11ca-b092-08001e01d6d5 */
    0x1c5b2910,
    0x33ab,
    0x11ca,
    0xb0,
    0x92,
    {0x08, 0x00, 0x1e, 0x01, 0xd6, 0xd5}
};

/*
 * Define the "nil" object.  (Not to be confused with an object whose type
 * simply happens to be "nil".)
 */
uuid_t NilObj = {0};


    



char *authn_level_names[] =
{
    "default",
    "none",
    "connect",
    "call",
    "pkt",
    "pkt_integrity",
    "pkt_privacy",
    NULL
};

char *authn_names[] =
{
    "none",
    "dce_private",
    "dce_public",
    "dce_dummy",
    NULL
};

char *authz_names[] =
{
    "none",
    "name",
    "dce",
    NULL
};

/*
 * Return error text.
 */

char *error_text (st)

unsigned32      st;

{
    static dce_error_string_t error_string;
    int inq_st;

    dce_error_inq_text (st, error_string, &inq_st);
    return ((char *) error_string);
}

/*
 * Dump storage usage info.
 */
dump_stg_info()

{
#ifdef ETEXT_EDATA
    extern char etext, edata, end;

    printf ("\netext = %08x, edata = %08x, end = %08x\n\n",
        &etext, &edata, &end);
#endif
}


/*
 * Lookup a name in a table.
 */

int lookup_name(table, s)
char *table[];
char *s;
{
    int i;

    if (isdigit(s[0]))
    {
        return (atoi(s));
    }

    for (i = 0; table[i] != NULL; i++)
    {
        if (strcmp(table[i], s) == 0)
        {
            return (i);
        }
    }

    usage(-1);
}

#if defined(__hpux) && !defined(HPDCE_PRINTF_FIX)

/*
 * perf_printf(): temporary fix for printf() mutex lock problem in libc_r.
 */
int perf_printf(const char *format, ...)
{
    va_list args;
    int oc;
    int rc;

    va_start(args, format);
    oc = pthread_setcancel(CANCEL_OFF);
    rc =vprintf(format, args);
    pthread_setcancel(oc);
    va_end(args);
    pthread_testcancel();
    return (rc);
}

int perf_fprintf(FILE *stream, const char *format, ...)
{
    va_list args;
    int oc;
    int rc;

    va_start(args, format);
    oc = pthread_setcancel(CANCEL_OFF);
    rc = vfprintf(stream, format, args);
    pthread_setcancel(oc);
    va_end(args);
    pthread_testcancel();
    return (rc);
}
#endif /* HPDCE_PRINTF_FIX */
