/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_aud_evts.h,v $
 * Revision 1.1.12.1  1996/05/10  13:18:10  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:49 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/20  16:05 UTC  hanfei
 * 	change REPADMIN_SetSwRev from 0x0140 to 0x013A and
 * 	redefine SEC_AUD_NUM_OF_EVTS to be the size + 1.
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/15  21:06 UTC  kline_s
 * 	change REPADMIN_SetSwRev from 0x0140 to 0x014A
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/15  21:04 UTC  kline_s
 * 	change REPADMIN_SetSwRev from 0x0140 to 0x014A
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:36 UTC  psn
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:35 UTC  greg  /main/SEC_migrate_merge/1]
 *
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:44 UTC  greg  /main/SEC_migrate/1]
 *
 * 	add sec_aud_evts.hREPADMIN_Vers event.
 * 	[1995/01/21  21:22 UTC  greg  /main/greg_migration2/1]
 *
 * Revision 1.1.6.1  1994/10/25  20:23:34  kline_s
 * 	UHYP CR 12343: Enable Delegation audit points - Fix incorrect
 * 	SEC_AUD_NUM_OF_EVTS to reflect total number of current events
 * 	and insert comments indicating that no event number 311 is
 * 	currently defined.
 * 	[1994/10/25  20:22:42  kline_s]
 * 
 * Revision 1.1.2.4  1994/08/24  21:06:59  cuti
 * 	Add delegation audit points.
 * 	[1994/08/24  20:01:09  cuti]
 * 
 * Revision 1.1.2.3  1994/08/12  17:37:37  mdf
 * 	Hewlett Packard Security Drop
 * 	[1994/08/12  14:15:21  mdf]
 * 
 * Revision 1.1.2.2  1994/06/02  21:40:43  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:32  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.2.1  1994/04/01  20:48:34  kline_s
 * 	[OT 10264] First audit code point drop for security.
 * 
 * Revision 1.1.1.2  1994/04/01  20:47:58  kline_s
 * 	[OT 10264] First audit code point drop for security.
 * 
 * $EndLog$
 */

/* secd audit points:
 *
 * Event numbers 0x00000100 to 0x000001FF are assigned to the
 * DCE Security server.  For each new audit point added, 
 * SEC_AUD_NUM_OF_EVTS must be correspondingly increased.
 *
 * Comments to the left of each entry identify the decimal value
 * of the entry and the current count of message entries.
 * This latter count should avoid any future confusion as to what
 * the value of SEC_AUD_NUM_OF_EVTS should be set to.
 *
 * CAUTION! These #defines are in HEX notation !
 */
 
#define SEC_AUD_FIRST_EVT_NUMBER	0x0100  /* 256   1 */

/* Authentication (krb5rpc) */
#define AS_Request			0x0101  /* 257   2 */
#define TGS_TicketReq			0x0102  /* 258   3 */
#define TGS_RenewReq			0x0103  /* 259   4 */
#define TGS_ValidateReq			0x0104  /* 260   5 */

/* DACL Management (rdaclif) */
#define ACL_Lookup			0x0105  /* 261   6 */
#define ACL_Replace			0x0106  /* 262   7 */
#define ACL_GetAccess			0x0107  /* 263   8 */
#define ACL_TestAccess			0x0108  /* 264   9 */
#define ACL_TestOnBehalf		0x0109  /* 265  10 */
#define ACL_GetMgrTypes			0x010A  /* 266  11 */
#define ACL_GetReferral			0x010B  /* 267  12 */

/* Privilege Server (rpriv) */
#define PRIV_GetPtgt			0x010C  /* 268  13 */

/* Registry Server Account (rs_acct) */
#define ACCT_Add			0x010D  /* 269  14 */
#define ACCT_Delete			0x010E  /* 270  15 */
#define ACCT_Rename			0x010F  /* 271  16 */
#define ACCT_Lookup			0x0110  /* 272  17 */
#define ACCT_Replace			0x0111  /* 273  18 */
#define ACCT_GetProjlist		0x0112  /* 274  19 */

/* Registry Misc. (rs_misc) */
#define LOGIN_GetInfo			0x0113  /* 275  20 */

/* Registry PGO (rs_pgo) */
#define PGO_Add				0x0114  /* 276  21 */
#define PGO_Delete			0x0115  /* 277  22 */
#define PGO_Replace			0x0116  /* 278  23 */
#define PGO_Rename			0x0117	/* 279  24 */
#define PGO_Get				0x0118  /* 280  25 */
#define PGO_KeyTransfer			0x0119  /* 281  26 */
#define PGO_AddMember			0x011A  /* 282  27 */
#define PGO_DeleteMember		0x011B  /* 283  28 */
#define PGO_IsMember			0x011C  /* 284  29 */
#define PGO_GetMembers			0x011D  /* 285  30 */

/* Registry Policy (rs_policy) */
#define PROP_GetInfo			0x011E  /* 286  31 */
#define PROP_SetInfo			0x011F  /* 287  32 */
#define POLICY_GetInfo			0x0120  /* 288  33 */
#define POLICY_SetInfo			0x0121  /* 289  34 */
#define AUTHPOLICY_GetInfo		0x0122  /* 290  35 */
#define AUTHPOLICY_SetInfo		0x0123  /* 291  36 */

/* Registry Admin (rs_rep_admin) */
#define REPADMIN_Stop			0x0124  /* 292  37 */
#define REPADMIN_Maint			0x0125  /* 293  38 */
#define REPADMIN_Mkey			0x0126  /* 294  39 */
#define REPADMIN_Destroy		0x0127  /* 295  40 */
#define REPADMIN_Init			0x0128  /* 296  41 */

/* Identifier Mapping (secidmap) */
#define SECID_ParseName			0x0129  /* 297  42 */
#define SECID_GenName			0x012A  /* 298  43 */

/* Extended Registry Attributes (rs_attr) */
#define ERA_Update			0x012B  /* 299  44 */
#define ERA_Delete			0x012C  /* 300  45 */
#define ERA_TestUpdate			0x012D  /* 301  46 */
#define ERA_LookupById			0x012E  /* 302  47 */
#define ERA_LookupNoExpand		0x012F  /* 303  48 */
#define ERA_LookupByName		0x0130  /* 304  49 */

/* Extended Registry Schema Attributes (rs_attr_schema) */
#define ERA_SchemaCreate		0x0131  /* 305  50 */
#define ERA_SchemaDelete		0x0132  /* 306  51 */
#define ERA_SchemaUpdate		0x0133  /* 307  52 */
#define ERA_SchemaLookupId		0x0134  /* 308  53 */
#define ERA_SchemaLookupName		0x0135  /* 309  54 */

/* Delegation (rpriv_v1_1) */
#define PRIV_GetEptgt                   0x0136  /* 310  55 */
/* no event number 311 defined - can be used for an audit
/* event defined later.                         /* 311  56 */

#define PRIV_BecomeDelegate             0x0138  /* 312  57 */
#define PRIV_BecomeImpersonator         0x0139  /* 313  58 */

#define REPADMIN_SetSwRev               0x013A  /* 314  59 */

/* Developer notes:
 * Audit subtracts SEC_AUD_FIRST_EVT_NUMBER (256) from SEC_AUD_NUM_OF_EVTS
 * to determine the evt_table size.  While not used, SEC_AUD_FIRST_EVT_NUMBER
 * is the first entry in audit's evt_table.  Hence, to avoid potential
 * table overflow, we always add one to the count to account for the
 * unused entry and reflect the true maximum table size required.
 */

#define SEC_AUD_NUM_OF_EVTS		59     /* 314 - 256 + 1 */
