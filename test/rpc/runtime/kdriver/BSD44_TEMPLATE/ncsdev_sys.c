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
 * Revision 1.1.6.2  1996/02/17  23:23:18  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:50  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:14:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:52  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:52:48  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:20:17  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:51:07  rsalz
 * 	 29-oct-91 sommerfeld    Change ../v2test -> ../v2test_lib
 * 	 18-oct-91 sommerfeld    Change cm_helper calls -> krpc_helper.
 * 	 18-oct-91 sommerfeld    added module header.
 * 	[1992/05/01  00:57:40  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:42:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * NCS test jig device driver.
 *
 * This provides a mechanism for user code to get various
 * kernel NCS things to happen.
 */

#include <sys/types.h>
#include <commonp.h>
#include <com.h>
#include <dce/ker/krpc_helper.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

/*
 * Supported test jig operations (via ioctl):
 * The actual IOCs are defined in the test specific files (since we have
 * this funny cross dependency on their args types).  They are listed
 * here so that a) you know where to look for them and b) you have a
 * greater likelyhood of not using one that someone else is using.
 */

#ifdef KNCK_CONFIG
#include "ncs_vtwotest.h"
#if NNCS_VTWOTEST > 0
#define NCSDEV_V2TEST
#endif

#include "ncs_ncstest.h"
#if NNCS_NCSTEST > 0
#define NCSDEV_NCSTEST
#endif

#include "des_test.h"
#if NDES_TEST > 0
#define NCSDEV_DESTEST
#endif

#include "cmh_test.h"
#if NCMH_TEST > 0
#define NCSDEV_CMHTEST
#endif
#else
#define NCSDEV_V2TEST
#define NCSDEV_NCSTEST
#define NCSDEV_DESTEST
#define NCSDEV_CMHTEST
#endif

#ifdef NCSDEV_V2TEST
#  include <../v2test_lib/v2test.h>       /* pick up v2test IOCs and arg types */
#endif

#ifdef NCSDEV_SOC
#  include <../soctest/soctest.h>      /* pick up soctest IOCs and arg types */
#endif

#ifdef NCSDEV_NCSTEST
#  include <../ncstest/ncstest.h>      /* pick up ncstest IOCs and arg types */
#endif

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

int ncsopen(dev, flag)
dev_t dev;
int flag;
{
  krpc_helper_init();
  return (0);
}

int ncsclose(dev)
dev_t dev;
{
  krpc_CancelHelper();
  return 0;
}

int ncswrite (dev, uio)
dev_t dev;
struct uio *uio;
{
  return krpc_helperSendResponse(uio);
}

int ncsread (dev, uio)
dev_t dev;
struct uio *uio;
{
  return krpc_helperGetRequest(uio);
}

int ncsselect (dev, events, revents, scanning)
dev_t dev;
short *events, *revents;
int scanning;
{
  return krpc_helperSelect(events, revents, scanning);
}

int ncsioctl(dev, cmd, data, flag)
dev_t dev;
int cmd;
caddr_t data;
int flag;
{
    int error = 0;

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
    return (error);
}

#ifdef NCSDEV_SOC
int run_soc(args)
soc_args_t *args;
{
    soc_outargs_t outargs;

    soc_doit(&args->inargs, &outargs);
    args->outargs = outargs;
}
#endif

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

#ifdef NCSDEV_NCSTEST
int run_ntc(args)
ncstest_args_t *args;
{
    ncstest_outargs_t outargs;

    ncstest_c_doit(&args->inargs, &outargs);
    args->outargs = outargs;
}

int run_nts(args)
ncstest_args_t *args;
{
    ncstest_outargs_t outargs;

    ncstest_s_doit(&args->inargs, &outargs);
    args->outargs = outargs;
}
#endif /* NCSDEV_NCSTEST */

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

#ifdef NCSDEV_CMHTEST

int run_cmh_test()
{
  krpc_helperTest();
}

#endif
