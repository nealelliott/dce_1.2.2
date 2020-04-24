/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roacpmzz.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:25  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:42  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:29  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:17  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:27  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:39  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:35  keutel
 * 	creation
 * 	[1994/03/21  13:18:56  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROACPMZZ_H
#define _ROACPMZZ_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roacpmzz_rcsid[] = { "@(#) $RCSfile: roacpmzz.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:40 $" } ;
#endif

/*
roacpmzz.h,
Tue Nov 23 10:40:20 1993
*/


 /* Includes removed with sed */ 

/* TYPE MATCHING MACROS */
#define ACSE_apdu_match(matchz) (((matchz)[0] == 32)\
||	AARQ_apdu_match(matchz)\
||	(((matchz)[0]|32) == 97)\
||	(((matchz)[0]|32) == 98)\
||	(((matchz)[0]|32) == 99)\
||	(((matchz)[0]|32) == 100))
#define AARE_apdu_match(matchz) ((matchz)[0] == 97)
#define RLRQ_apdu_match(matchz) ((matchz)[0] == 98)
#define RLRE_apdu_match(matchz) ((matchz)[0] == 99)
#define ABRT_apdu_match(matchz) ((matchz)[0] == 100)
#define Association_info_match(matchz) ((matchz)[0] == 48)
#define Ass_source_diag_match(matchz) (((matchz)[0] == 32)\
||	((matchz)[0] == 161)\
||	((matchz)[0] == 162))

/* CODING AND DECODING PROCEDURES */

int PASCAL ACSE_apdu_dec PROTO((STOREHANDLE,unsigned long,rT0646_acseApdus FAR *,asn1_pvoid));
int PASCAL ACSE_apdu_cod PROTO((STOREHANDLE,int,long,rT0646_acseApdus FAR *,asn1_pvoid));
int PASCAL AARE_apdu_dec PROTO((STOREHANDLE,unsigned long,rT0642_aareApdu FAR *,asn1_pvoid));
int PASCAL AARE_apdu_cod PROTO((STOREHANDLE,int,long,rT0642_aareApdu FAR *,asn1_pvoid));
int PASCAL AARE_apdu_ccod PROTO((STOREHANDLE,rT0642_aareApdu FAR *,asn1_pvoid));
int PASCAL AARE_apdu_dseq PROTO((STOREHANDLE,unsigned long int,rT0642_aareApdu FAR *,asn1_pvoid));

int PASCAL RLRQ_apdu_dec PROTO((STOREHANDLE,unsigned long,rT0643_rlrqApdu FAR *,asn1_pvoid));
int PASCAL RLRQ_apdu_cod PROTO((STOREHANDLE,int,long,rT0643_rlrqApdu FAR *,asn1_pvoid));
int PASCAL RLRQ_apdu_ccod PROTO((STOREHANDLE,rT0643_rlrqApdu FAR *,asn1_pvoid));
int PASCAL RLRQ_apdu_dseq PROTO((STOREHANDLE,unsigned long int,rT0643_rlrqApdu FAR *,asn1_pvoid));

int PASCAL RLRE_apdu_dec PROTO((STOREHANDLE,unsigned long,rT0644_rlreApdu FAR *,asn1_pvoid));
int PASCAL RLRE_apdu_cod PROTO((STOREHANDLE,int,long,rT0644_rlreApdu FAR *,asn1_pvoid));
int PASCAL RLRE_apdu_ccod PROTO((STOREHANDLE,rT0644_rlreApdu FAR *,asn1_pvoid));
int PASCAL RLRE_apdu_dseq PROTO((STOREHANDLE,unsigned long int,rT0644_rlreApdu FAR *,asn1_pvoid));

int PASCAL ABRT_apdu_dec PROTO((STOREHANDLE,unsigned long,rT0645_abrtApdu FAR *,asn1_pvoid));
int PASCAL ABRT_apdu_cod PROTO((STOREHANDLE,int,long,rT0645_abrtApdu FAR *,asn1_pvoid));
int PASCAL ABRT_apdu_ccod PROTO((STOREHANDLE,rT0645_abrtApdu FAR *,asn1_pvoid));
int PASCAL ABRT_apdu_dseq PROTO((STOREHANDLE,unsigned long int,rT0645_abrtApdu FAR *,asn1_pvoid));

int PASCAL Association_info_dec PROTO((STOREHANDLE,unsigned long,ulong FAR *,asn1_pulong,asn1_pvoid));
int PASCAL Association_info_cod PROTO((STOREHANDLE,int,long,ulong FAR *,asn1_pulong,asn1_pvoid));
int PASCAL Ass_source_diag_dec PROTO((STOREHANDLE,unsigned long,ulong FAR *,rT063b_uAssociSourceDiag FAR *));
int PASCAL Ass_source_diag_cod PROTO((STOREHANDLE,int,long,ulong FAR *,rT063b_uAssociSourceDiag FAR *));

#endif	/* _ROACPMZZ_H */
