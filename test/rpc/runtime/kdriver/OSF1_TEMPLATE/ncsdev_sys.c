/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ncsdev_sys.c,v $
 * Revision 1.2.6.2  1996/02/17  23:23:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:51  marty]
 *
 * Revision 1.2.6.1  1995/12/11  20:14:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:57  root]
 * 
 * Revision 1.2.4.3  1993/01/27  18:59:13  delgado
 * 	Modify to use the new krpc_helper interface
 * 	[1993/01/27  18:58:36  delgado]
 * 
 * Revision 1.2.4.2  1993/01/13  19:53:08  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:20:39  bbelch]
 * 
 * Revision 1.2.2.3  1992/05/28  17:51:43  garyf
 * 	fix compilation warning
 * 	[1992/05/28  17:44:07  garyf]
 * 
 * Revision 1.2.2.2  1992/05/01  15:51:17  rsalz
 * 	 29-oct-91 sommerfeld    Change ../v2test -> ../v2test_lib
 * 	 18-oct-91 sommerfeld    Change cm_helper calls -> krpc_helper.
 * 	 18-oct-91 sommerfeld    added module header.
 * 	[1992/05/01  00:57:48  rsalz]
 * 
 * Revision 1.2  1992/01/19  22:14:04  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
 */
/*
 *  OSF DCE Version 1.0 
 */
/*
 * NCS test jig device driver.
 *
 * This provides a mechanism for user code to get various
 * kernel NCS things to happen.
 */

#include <sys/types.h>
#include <sys/uio.h>
#include <dce/ker/krpc_helper.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <sys/conf.h>
#include <sys/param.h>

/*
 * Supported test jig operations (via ioctl):
 * The actual IOCs are defined in the test specific files (since we have
 * this funny cross dependency on their args types).  They are listed
 * here so that a) you know where to look for them and b) you have a
 * greater likelihood of not using one that someone else is using.
 */

#ifdef NCSDEV_V2TEST
#  include <../v2test_lib/v2test.h>     /* pick up v2test IOCs and arg types */
#endif

#ifdef NCSDEV_SOC
#  include <../soctest/soctest.h>       /* pick up soctest IOCs and arg types */
#endif

#ifdef NCSDEV_NCSTEST
#  include <../ncstest/ncstest.h>      /* pick up ncstest IOCs and arg types */
#endif

/* ---------------------------------------------------------------------- */

#ifdef NOTDEF

#define NIOCV2C           _IOWR('N', 0, v2c_args_t)     /* run v2client */
#define NIOCV2S           _IOWR('N', 1, v2s_args_t)     /* run v2server */
#define NIOCSOC           _IOWR('N', 2, soc_args_t)     /* perform rpc_socket functions */
#define NIOCMEM           _IOWR('N', 3, mem_args_t)     /* perform rpc_mem functions */
#define NIOCCMA           _IOWR('N', 4, cma_args_t)     /* perform cma_ functions */
#define NIOCARC           _IOWR('N', 5, arc_args_t)     /* run NIDL array client */
#define NIOCARS           _IOWR('N', 6, ars_args_t)     /* run NIDL array server */
#define NIOCPIC           _IOWR('N', 7, pic_args_t)     /* run NIDL pipe client */
#define NIOCPIS           _IOWR('N', 8, pis_args_t)     /* run NIDL pipe server */
#define NIOCNTC           _IOWR(NIOC, 9, ncstest_args_t)     /* run ncstest client */
#define NIOCNTS           _IOWR(NIOC, 10, ncstest_args_t)    /* run ncstest server */

#endif

#ifdef NCSDEV_DESTEST
#define NIOCDESVERIFY          _IO('N', 11)           /* run v2client */
#define NIOCDESSPEED           _IOW('N', 12, int)     /* run v2client */
#endif

#ifdef NCSDEV_CMHTEST
#define NIOCCMHT                _IO('N', 13)          /* run cm_helper test */
#endif

/* ---------------------------------------------------------------------- */

int ncsdev_open(dev, flag)
dev_t dev;
int flag;
{                           
   int error;
#ifdef DEBUG
    printf("(ncsdev_open) Entered\n");
#endif
    
    error = krpch_test_driver_open();

#ifdef DEBUG
    printf("(ncsdev_open) Returning\n");
#endif
    return (error);
}

int ncsdev_close(dev)
dev_t dev;
{
    krpch_close();
    return 0;
}

int ncsdev_write (dev, uio)
dev_t dev;
struct uio *uio;
{
    return krpch_write(uio);
}

int ncsdev_read (dev, uio)
dev_t dev;
struct uio *uio;
{
    return krpch_read(uio);
}

/*
 * The old krpc_HelperSelect used to be a no-op
 * therefore, we will not make any calls here.
 * There is now a real krpch_select which will
 * always return true for POLLWRITE and will
 * return true for POLLREAD when there is an 
 * upcall to service, but I am not certain that
 * this is what the users of kdriver are expecting.
 */
int ncsdev_select (dev, events, revents, scanning)
dev_t dev;
short *events, *revents;
int scanning;
{
    return (0);
}

int ncsdev_ioctl(dev, cmd, data, flag)
dev_t dev;
int cmd;
caddr_t data;
int flag;
{
    int error = 0;

#ifdef DEBUG
    printf("(ncsdev_ioctl) Entered, cmd=%x\n", cmd);
#endif

    switch (cmd)
    {
#ifdef NCSDEV_SOC
    case NIOCSOC:      /* socket tests */
        run_soc(data);
        break;
#endif

#ifdef NCSDEV_V2TEST
    case NIOCV2C:      /* run v2client */
        run_v2c(data);
        break;
    case NIOCV2S:      /* run v2server */
        run_v2s(data);
        break;
#endif /* NCSDEV_V2TEST */

#ifdef NCSDEV_NCSTEST
    case NIOCNTC:      /* run ncstest client */
        run_ntc(data);
        break;
    case NIOCNTS:      /* run ncstest server */
        run_nts(data);
        break;
#endif /* NCSDEV_NCSTEST */

#ifdef NCSDEV_DESTEST
    case NIOCDESVERIFY:
        run_des_verify();
        break;

    case NIOCDESSPEED:
        run_des_speed(data);
        break;
#endif

#ifdef NCSDEV_CMHTEST
    case NIOCCMHT:
        printf("cmh test\n");
        run_cmh_test();
        break;
#endif

     default:
        error = EINVAL;
    }


#ifdef DEBUG
    printf("(ncsdev_ioctl) Returning, error=%d\n", error);
#endif

    return (error);
}

/* ---------------------------------------------------------------------- */

#ifdef NCSDEV_SOC

int run_soc(args)
soc_args_t *args;
{
    soc_outargs_t outargs;

    soc_doit(&args->inargs, &outargs);
    args->outargs = outargs;
}

#endif

/* ---------------------------------------------------------------------- */

#ifdef NCSDEV_V2TEST

int run_v2c(args)
v2c_args_t *args;
{
    v2c_outargs_t outargs;

    v2c_doit(&args->in, &outargs);
    args->out = outargs;
}

int run_v2s(args)
v2s_args_t *args;
{
    v2s_outargs_t outargs;

    v2s_doit(&args->in, &outargs);
    args->out = outargs;
}

#endif /* NCSDEV_V2TEST */

/* ---------------------------------------------------------------------- */

#ifdef NCSDEV_NCSTEST

int run_ntc(args)
ncstest_args_t *args;
{
    ncstest_outargs_t outargs;

#ifdef DEBUG
    printf("(run_ntc) Entered\n");
#endif

    ncstest_c_doit(&args->inargs, &outargs);
    args->outargs = outargs;

#ifdef DEBUG
    printf("(run_ntc) Returning\n");
#endif
}

int run_nts(args)
ncstest_args_t *args;
{
    ncstest_outargs_t outargs;

#ifdef DEBUG
    printf("(run_nts) Entered\n");
#endif

    ncstest_s_doit(&args->inargs, &outargs);
    args->outargs = outargs;

#ifdef DEBUG
    printf("(run_nts) Returning\n");
#endif
}

#endif /* NCSDEV_NCSTEST */

/* ---------------------------------------------------------------------- */

#ifdef NCSDEV_DESTEST

int run_des_verify ()
{
    static char *des_argv[] = { "kernel_des_verify", 0 };
    des_verify_main(1, des_argv);
}

int run_des_speed (data)
     int *data;
{
    int count = *data;
    int i;

    printf("%d iterations\n", *data);
    for (i=0; i<count; i++) {
        des_test();
    }
}

#endif

/* ---------------------------------------------------------------------- */

#ifdef NCSDEV_CMHTEST

int run_cmh_test()
{
  krpc_helperTest();
}

#endif

/* ---------------------------------------------------------------------- */

#ifdef NCSDEV_DYNAMIC

#define NCSDEV_MAJOR 30

extern int nodev();
struct cdevsw ncs_cdevsw = {
    ncsdev_open,    ncsdev_close,   ncsdev_read,    ncsdev_write,
    ncsdev_ioctl,   nodev,          nodev,          0,
    ncsdev_select,  nodev
};

ncsdev_init()
{
    dev_t devno;

#ifdef DEBUG
    printf("(ncsdev_init) Entered\n");
#endif

    devno = makedev(NCSDEV_MAJOR, 0);

    cdevsw_del(devno);      /* Just in case we're there already */

    devno = cdevsw_add(devno, &ncs_cdevsw);

#ifdef DEBUG
    printf("(ncsdev_init) devno=%x\n", devno);
#endif

}

#endif

/* ---------------------------------------------------------------------- */

