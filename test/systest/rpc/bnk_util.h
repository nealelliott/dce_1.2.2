/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bnk_util.h,v $
 * Revision 1.1.9.2  1996/02/18  23:07:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:27  marty]
 *
 * Revision 1.1.9.1  1995/12/11  22:00:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:59:50  root]
 * 
 * Revision 1.1.5.2  1993/02/05  15:21:32  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:41:53  cjd]
 * 
 * Revision 1.1.3.2  1993/02/04  22:09:05  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1.2.2  1992/04/15  16:54:18  rps
 * 	Remove common util.h routines
 * 	[1992/04/15  15:57:31  rps]
 * 
 * Revision 1.1  1992/01/19  14:37:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   	FILE_NAME: bnk_util.h
 *
 *   	COMPONENT_NAME: dce.systest
 *
 *   	FUNCTIONS: none
 *
 *   	ORIGINS: 2 27
 *
 *   	(C) COPYRIGHT International Business Machines Corp. 1990
 *   	All Rights Reserved
 *
 *	Copyright  1987 by Apollo Computer Inc., Chelmsford, Massachusetts
 *
 *   	FOR_TEST(s): dcerpbnk
 *
 *   	USAGE: n/a
 *
 *   	DESCRIPTION:
 *
 *		This file contains defines used by the function check_status
 *              and prototypes of functions in bnk_util.c.  The defines allow
 *              check_status() to process status codes and handle message
 *              displaying and types of error handling to be done in a flex-
 *              able manner.
 *
 *		The defines for the four type of account objects as well as 
 *		their object uuids and type uuids are included.  The uuid
 *		strings where obtained by executing uuid_gen and saving the
 *		output.
 *
 *   	HISTORY:
 *      	5/01/91         Modified original util.h	Gerald Cantor
 *	       10/19/91		Incorperated util.c and 
 *				bnk_util.c together, thsu
 *				facilitating a change from 
 *				util.h to bnk_util.h.		Gerald Cantor
 *	       10/20/91		ANSIfied functions and updated
 *				comments and prologs.		Gerald Cantor
 *	       12/17/91		Updated prologs, added 
 *				print_exception().		Gerald Cantor
 */
#define CK	0
#define SV	1
#define CD	2
#define IRA	3
#define CK_UUID	 	"cdc79674-f5ad-11c9-912a-02608c2ea88e"
#define SV_UUID	 	"fa1ecda0-229e-11ca-8698-02608c2c688e"
#define CD_UUID  	"971a9d20-f5ad-11c9-bceb-02608c2ea88e"
#define IRA_UUID 	"d83f9aac-f5ad-11c9-aa6d-02608c2ea88e"
#define RES_UUID 	"d9803f8e-f5ab-11c9-851b-02608c2ea88e"
#define UNRES_UUID 	"805f8846-22a0-11ca-bc8c-02608c2c688e"
#define ACCOUNT_EXISTS		100
#define ACCT_NOT_FOUND		200
#define BAD_DEPOSIT_AMOUNT	300
#define UNMATURE_ACCOUNT	400
#define INSUFFICIENT_FUNDS	500
#define EMPTY_BANK		600
#define FAIL			700

void BankNameToID(char *, uuid_t *);
void get_uuids(uuid_t *, uuid_t *, uuid_t *, uuid_t *, uuid_t *, uuid_t *);
void set_objects(uuid_t *, uuid_t *, uuid_t *, uuid_t *, uuid_t *, uuid_t *);
void build_object_vector(uuid_vector_p_t, uuid_p_t);
char *SAlloc(char *);
uuid_t *get_handle_obj(handle_t);
