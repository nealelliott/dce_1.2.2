/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rgy_clt.h,v $
 * Revision 1.1.65.2  1996/02/18  23:08:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:21  marty]
 *
 * Revision 1.1.65.1  1995/12/11  22:46:51  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/05  15:39 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/31  14:47  bissen
 * 	merge kk and hpdce01
 * 	[1995/12/11  22:22:12  root]
 * 
 * Revision 1.1.5.2  1993/07/09  14:29:54  root
 * 	Initial King Kong branch
 * 	[1993/07/09  14:29:28  root]
 * 
 * Revision 1.1.62.1  1993/09/09  19:56:16  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/09  19:51:04  cmckeen]
 * 
 * Revision 1.1.5.2  1993/07/02  16:42:24  eheller
 * 	GAMMERA version which runs under TET.
 * 	[1993/07/02  16:17:42  eheller]
 * 
 * Revision 1.1.3.2  1993/05/04  20:42:27  sekhar
 * 	Defined constant defintions (to replace hardcode numbers
 * 	/strings in code).
 * 	[1993/05/04  18:59:59  sekhar]
 * 
 * 	Added GETTIMEOFDAY define
 * 	[1993/03/25  18:45:22  eperkins]
 * 
 * 	Replaced delay with item and corrected the datatype of val_cert_flag
 * 	[1993/03/11  00:08:16  eperkins]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/05  14:46:46  cjd]
 * 
 * 	Initial version 1.0.2 system test sec.rgy.7
 * 	[1993/01/04  18:41:48  eperkins]
 * 
 * Revision 1.1.1.4  1993/04/21  20:28:17  root
 * 	    Loading drop DCE1_0_2B23
 * 
 * Revision 1.1.3.2  1993/02/04  22:17:44  cjd
 * 	Embedded copyright notice
 * 
 * $EndLog$
 */

/*
 *  N.B. In DCE 1.1 release TRUE and FALSE will be defined
 *  in dce.h (as noted in RFC17 and RFC34). When dce.h 
 *  becomes available these definitions maybe removed and dce.h
 *  included.
 */

#ifndef TRUE
#define     TRUE    1
#define     FALSE   0
#endif

/*
 * MAX_NS_NAME is the maximum size of name space
 * entry 
 */

#define MAX_NS_NAME    1024

#define CELLADMIN            "cell_admin"
#define CELLADMIN_PW         "-dce-"

/* 
 *  NS_ENTRY_PREFIX is used to construct the name space 
 *  entry where the secrgy test server exports its binding
 *  information.
 */

#define NS_ENTRY_PREFIX      "/subsys"

#define GETTIMEOFDAY(t) \
{ \
    struct timezone tz; \
 \
    gettimeofday(t, &tz); \
}


typedef struct call_args {
	int	server_num;
	unsigned_char_t *client;
	unsigned_char_t *password;
	rpc_binding_handle_t binding;
	int	duration;
	int	delay;
	int	val_cert_flag;
	idl_char *msg;
	int	status;
} call_args_t;







