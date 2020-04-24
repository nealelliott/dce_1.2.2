/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: private_eval.h,v $
 * Revision 1.1.4.2  1996/02/18  23:05:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:44  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:49:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:25:38  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:16:58  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:43:26  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:15:33  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/04  21:53:53  mori_m
 * 	CR 9701: RPC runtime I18N extension functional test drop.
 * 	[1994/05/04  21:28:18  mori_m]
 * 
 * $EndLog$
 */

/*
 * ISO 10646:1992, UCS-2, Level 2
 * Universal Code Set (encoding) for DCE
 */
#define UCS2_L2		0x0010101

/*
 * When rpc_ns_mgmt_*** routines are extended in the future to, 
 * deal with attributes other than code sets, the new attribute
 * specifier needs to be added here.
 */
#define	RPC_EVAL_TYPE_CODESETS		0x0001
#define	RPC_CUSTOM_EVAL_TYPE_CODESETS	0x0002

/*
 *  Tagged union identifiers.
 *  This is used to distinguish tagged union data structure for
 *  i18n binding handle extension.
 */
#define	RPC_CS_EVAL_TAGS		0
#define RPC_CS_EVAL_METHOD		1


/*
 * Code sets interoperability connection models
 */
#define	RPC_EVAL_NO_CONVERSION		0x0001
#define	RPC_EVAL_RMIR_MODEL		0x0002
#define	RPC_EVAL_CMIR_MODEL		0x0003
#define	RPC_EVAL_SMIR_MODEL		0x0004
#define	RPC_EVAL_INTERMEDIATE_MODEL	0x0005
#define	RPC_EVAL_UNIVERSAL_MODEL	0x0006


/*
 * R P C _ C S _ C O D E S E T _ I 1 4 Y _ D A T A
 *
 * Argument to OSF code set evaluation routine.  This data will be passed
 * to the evaluation routine, and figure out the compatible client and server
 * code sets combination.  The evaluation routine will be called from a client
 * in OSF implementation, and it will not be called from a client stub.
 *
 * ns_name	: NSI entry name for a server
 * cleanup	: boolean flag indicating any clean-up action required.
 * method_p	: pointer to 'rpc_cs_method_eval_t' data.  See above.
 * status	: result of the code set evaluation.
 */
typedef struct codeset_i14y_data {
	unsigned_char_p_t	ns_name;
	void			*args;
	boolean32		cleanup;
	rpc_cs_method_eval_p_t	method_p;
	error_status_t		status;
} rpc_cs_codeset_i14y_data, *rpc_cs_codeset_i14y_data_p;
