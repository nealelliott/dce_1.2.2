/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * (c) Copyright 1991, 1992
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: uuidgen.c,v $
 * Revision 1.1.7.2  1996/02/17  23:59:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:11  marty]
 *
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.2  1993/07/08  09:13:30  root
 * 	Initial King Kong branch
 * 	[1993/07/08  09:12:54  root]
 *
 * Revision 1.1.7.1  1995/12/07  22:38:34  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/30  22:40 UTC  sommerfeld
 * 	Now identical to /main/LATEST
 * 	[1995/01/24  22:05 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	Bug 12003 - Don't print status code, print error text.
 * 	[1994/09/01  19:39:09  tom]
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/21  14:50 UTC  tatsu_s
 * 	Merged mothra up to dce1_1_b18.
 * 	[1995/12/07  21:18:59  root]
 * 
 * Revision 1.1.5.4  1994/09/01  19:39:26  tom
 * 	Bug 12003 - Don't print status code, print error text.
 * 	[1994/09/01  19:39:09  tom]
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:33 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/21  22:48  lmm
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.7.3  1993/09/20  18:00:28  tatsu_s
 * 	KK merged upto DCE1_0_3b03.
 * 
 * 	Added #include <hpdce_version.h>.
 * 	[1993/05/26  14:13:56  tatsu_s]
 * 
 * Revision 1.1.5.3  1994/06/28  21:02:17  sommerfeld
 * 	OT 7713: additional uuidgen conversions
 * 	[1994/06/28  21:01:23  sommerfeld]
 * 
 * Revision 1.1.5.2  1994/04/15  14:39:49  rsalz
 * 	Use SAMS and getopt; general cleanup (OT CR 10342).
 * 	[1994/04/13  19:12:53  rsalz]
 * 
 * Revision 1.1.5.1  1994/01/21  22:30:42  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  21:51:25  cbrooks]
 * 
 * Revision 1.1.3.6  1993/01/12  15:38:25  markar
 * 	     OT CR 2153 fix: UUID strings must be lower case (per AES)
 * 	[1992/11/13  18:57:22  markar]
 * 
 * Revision 1.1.3.5  1993/01/07  16:58:32  tom
 * 	Bug 6412 - Add setlocale call for i18n.
 * 	[1993/01/05  23:22:46  tom]
 * 
 * Revision 1.1.3.4  1993/01/03  22:34:20  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:49:14  bbelch]
 * 
 * Revision 1.1.3.3  1992/12/23  19:34:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:14:50  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:12:36  devsrc
 * 	Fix merge hiccup.
 * 	[1992/09/21  18:38:14  weisman]
 * 
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:35:26  weisman]
 * 
 * Revision 1.1  1992/01/19  03:05:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
*/
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dce_error.h>
#include <dce/uuid.h>
#include <dceuidmsg.h>


/*
**  If error, print message and exit.
*/
static void
check_status
(
    error_status_t	st,
    char		*func
)
{
    char error_text[dce_c_error_string_len];
    int  dummy_st;

    if (st != error_status_ok) {
	dce_error_inq_text(st, error_text, &dummy_st);
	dce_fprintf(stderr, uuidgen_st_failure, func, error_text);
	exit(1);
    }
}

static void output_uuid (FILE *F, int format, uuid_p_t u)
{
    unsigned char	*buff;
    error_status_t 	st;
    
    uuid_to_string(u, &buff, &st);
    check_status(st, "uuid_to_string");
	
    switch (format) {
    case 'u':
	fprintf(F, "%s\n", buff);
	break;
    case 'i':
	fprintf(F, "[\n");
	fprintf(F, "    uuid(%s),\n", buff);
	fprintf(F, "    version(1.0)\n");
	fprintf(F, "]\n");
	fprintf(F, "interface INTERFACENAME\n");
	fprintf(F, "{\n");
	fprintf(F, "}\n");
	break;
    case 's':
	fprintf(F, "= { /* %s */\n", buff);
	fprintf(F, "    0x%8.8x, 0x%4.4x, 0x%4.4x, 0x%2.2x, 0x%2.2x,\n",
		u->time_low, u->time_mid, u->time_hi_and_version,
		u->clock_seq_hi_and_reserved, u->clock_seq_low);
	fprintf(F,
		"    { 0x%2.2x, 0x%2.2x, 0x%2.2x, 0x%2.2x, 0x%2.2x, 0x%2.2x }\n",
		u->node[0], u->node[1], u->node[2],
		u->node[3], u->node[4], u->node[5]);
	fprintf(F, "};\n");
    }
    rpc_string_free(&buff, &st);
}

int
main(
    int			ac,
    char		*av[]
)
{
    unsigned char	*buff;
    uuid_t		u;
    int			count;
    char		format;
    FILE		*F;
    int			i;
    error_status_t	st;

    /* Set defaults. */
    setlocale(LC_ALL, "");
    dce_msg_define_msg_table(uuidgen_msg_table,
	sizeof uuidgen_msg_table / sizeof uuidgen_msg_table[0],
	&st);
    format = 'u';
    count = 1;
    F = stdout;

    /* Parse JCL. */
    while ((i = getopt(ac, av, "hisvn:o:t:c:")) != EOF)
	switch (i) {
	default:
	    dce_printf(uuidgen_st_usage);
	    return 1;
	case 'h':
	    dce_printf(uuidgen_st_usage);
	    dce_printf(uuidgen_st_h_flag);
	    dce_printf(uuidgen_st_i_flag);
	    dce_printf(uuidgen_st_s_flag);
	    dce_printf(uuidgen_st_n_flag);
	    dce_printf(uuidgen_st_o_flag);
	    dce_printf(uuidgen_st_t_flag);
	    dce_printf(uuidgen_st_c_flag);
	    dce_printf(uuidgen_st_v_flag);
	    return 0;
	case 'i':
	case 's':
	    format = i;
	    break;
	case 'v':
	    dce_printf(uuidgen_st_version);
	    return 0;
	case 'n':
	    count = atoi(optarg);
	    break;
	case 'o':
	    if ((F = fopen(optarg, "w")) == NULL) {
		dce_fprintf(stderr, uuidgen_st_cantopen, optarg);
		return 1;
	    }
	    break;
	case 't':
	    uuid_from_string((idl_char *) optarg, &u, &st);
	    check_status(st, "uuid_from_string");
	    uuid_to_string(&u, &buff, &st);
	    check_status(st, "uuid_to_string");
	    fprintf(F, "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x\n",
		    u.time_low, u.time_mid, u.time_hi_and_version,
		    u.clock_seq_hi_and_reserved, u.clock_seq_low,
		    u.node[0], u.node[1], u.node[2],
		    u.node[3], u.node[4], u.node[5]);
	    return 0;
	case 'c':
	    uuid_from_string((idl_char *) optarg, &u, &st);
	    check_status(st, "uuid_from_string");
	    output_uuid(F, format, &u);
	    return 0;
	}
    ac -= optind;
    av += optind;
    if (ac) {
	dce_fprintf(stderr, uuidgen_st_usage, "uuidgen");
	return 1;
    }

    /* Do the work. */
    while (--count >= 0) {
	uuid_create(&u, &st);
	check_status(st, "uuid_create");

	output_uuid(F, format, &u);
    }
    return 0;
}

