/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppmzz.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:05  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:11  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:08  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:38  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:44  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:42  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:01  keutel
 * 	creation
 * 	[1994/03/21  13:19:27  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPMZZ_H
#define _ROPPMZZ_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppmzz_rcsid[] = { "@(#) $RCSfile: roppmzz.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:58 $" } ;
#endif

/*
roppmzz.h,
Tue Nov 23 10:32:53 1993
*/


 /* Includes removed with sed */ 

typedef rT0414_stPresContId     FAR * asn1_prT0414_stPresContId;
typedef rT0413_stPresContDefRes FAR * asn1_prT0413_stPresContDefRes;



/* TYPE MATCHING MACROS */
#define CPA_PPDU_match(matchz) ((matchz)[0] == 49)
#define CPR_PPDU_match(matchz) (((matchz)[0] == 32)\
||	Components_of_set_match(matchz)\
||	(((matchz)[0]|32) == 48))
#define Abort_type_match(matchz) (((matchz)[0] == 32)\
||	ARU_PPDU_match(matchz)\
||	(((matchz)[0]|32) == 48))
#define ARU_PPDU_match(matchz) (((matchz)[0] == 32)\
||	Components_of_set_match(matchz)\
||	(((matchz)[0]|32) == 160))
#define ARP_PPDU_match(matchz) ((matchz)[0] == 48)
#define Pres_cont_id_list_match(matchz) ((matchz)[0] == 48)
#define Pres_cont_id_list_seq_match(matchz) ((matchz)[0] == 48)
#define Result_list_match(matchz) ((matchz)[0] == 48)
#define Result_list_seq_match(matchz) ((matchz)[0] == 48)
#define User_data_match(matchz) (((matchz)[0] == 32)\
||	(((matchz)[0]|32) == 96)\
||	(((matchz)[0]|32) == 97))
#define CPA_PPDU_seq_match(matchz) ((matchz)[0] == 48)
#define CPR_PPDU_seq_match(matchz) ((matchz)[0] == 48)
#define ARU_PPDU_seq_match(matchz) ((matchz)[0] == 48)

/* CODING AND DECODING PROCEDURES */

int PASCAL CPA_PPDU_dec PROTO((STOREHANDLE,unsigned long,rT041B_stCpa FAR *,asn1_pvoid));
int PASCAL CPA_PPDU_cod PROTO((STOREHANDLE,int,long,rT041B_stCpa FAR *,asn1_pvoid));
int PASCAL CPA_PPDU_ccod PROTO((STOREHANDLE,rT041B_stCpa FAR *,asn1_pvoid));
int PASCAL CPR_PPDU_dec PROTO((STOREHANDLE,unsigned long,rT041C_stCpr FAR *,asn1_pvoid));
int PASCAL CPR_PPDU_cod PROTO((STOREHANDLE,int,long,rT041C_stCpr FAR *,asn1_pvoid));
int PASCAL Abort_type_dec PROTO((STOREHANDLE,unsigned long,rT041G_unPpdu FAR *,asn1_pvoid));
int PASCAL Abort_type_cod PROTO((STOREHANDLE,int,long,rT041G_unPpdu FAR *,asn1_pvoid));
int PASCAL ARU_PPDU_dec PROTO((STOREHANDLE,unsigned long,rT041D_stAru FAR *,asn1_pvoid));
int PASCAL ARU_PPDU_cod PROTO((STOREHANDLE,int,long,rT041D_stAru FAR *,asn1_pvoid));
int PASCAL ARP_PPDU_dec PROTO((STOREHANDLE,unsigned long,rT041E_stArp FAR *));
int PASCAL ARP_PPDU_cod PROTO((STOREHANDLE,int,long,rT041E_stArp FAR *));
int PASCAL ARP_PPDU_ccod PROTO((STOREHANDLE,rT041E_stArp FAR *));
int PASCAL ARP_PPDU_dseq PROTO((STOREHANDLE,unsigned long int,rT041E_stArp FAR *));

int PASCAL Pres_cont_id_list_dec PROTO((STOREHANDLE,unsigned long,ulong FAR *,asn1_prT0414_stPresContId));
int PASCAL Pres_cont_id_list_cod PROTO((STOREHANDLE,int,long,ulong FAR *,asn1_prT0414_stPresContId));
int PASCAL Pres_cont_id_list_seq_dec PROTO((STOREHANDLE,unsigned long,rT0414_stPresContId FAR *));
int PASCAL Pres_cont_id_list_seq_cod PROTO((STOREHANDLE,int,long,rT0414_stPresContId FAR *));
int PASCAL Pres_cont_id_list_seq_ccod PROTO((STOREHANDLE,rT0414_stPresContId FAR *));
int PASCAL Pres_cont_id_list_seq_dseq PROTO((STOREHANDLE,unsigned long int,rT0414_stPresContId FAR *));

int PASCAL Result_list_dec PROTO((STOREHANDLE,unsigned long,ulong FAR *,asn1_prT0413_stPresContDefRes));
int PASCAL Result_list_cod PROTO((STOREHANDLE,int,long,ulong FAR *,asn1_prT0413_stPresContDefRes));
int PASCAL Result_list_seq_dec PROTO((STOREHANDLE,unsigned long,rT0413_stPresContDefRes FAR *));
int PASCAL Result_list_seq_cod PROTO((STOREHANDLE,int,long,rT0413_stPresContDefRes FAR *));
int PASCAL Result_list_seq_ccod PROTO((STOREHANDLE,rT0413_stPresContDefRes FAR *));
int PASCAL Result_list_seq_dseq PROTO((STOREHANDLE,unsigned long int,rT0413_stPresContDefRes FAR *));

int PASCAL User_data_dec PROTO((STOREHANDLE,unsigned long,rT0417_stUserData FAR *,asn1_pvoid));
int PASCAL User_data_cod PROTO((STOREHANDLE,int,long,rT0417_stUserData FAR *,asn1_pvoid));
int PASCAL CPA_PPDU_seq_dec PROTO((STOREHANDLE,unsigned long,rT041B_stCpa FAR *,asn1_pvoid));
int PASCAL CPA_PPDU_seq_cod PROTO((STOREHANDLE,int,long,rT041B_stCpa FAR *,asn1_pvoid));
int PASCAL CPA_PPDU_seq_ccod PROTO((STOREHANDLE,rT041B_stCpa FAR *,asn1_pvoid));
int PASCAL CPA_PPDU_seq_dseq PROTO((STOREHANDLE,unsigned long int,rT041B_stCpa FAR *,asn1_pvoid));

int PASCAL CPR_PPDU_seq_dec PROTO((STOREHANDLE,unsigned long,rT041C_stCpr FAR *,asn1_pvoid));
int PASCAL CPR_PPDU_seq_cod PROTO((STOREHANDLE,int,long,rT041C_stCpr FAR *,asn1_pvoid));
int PASCAL CPR_PPDU_seq_ccod PROTO((STOREHANDLE,rT041C_stCpr FAR *,asn1_pvoid));
int PASCAL CPR_PPDU_seq_dseq PROTO((STOREHANDLE,unsigned long int,rT041C_stCpr FAR *,asn1_pvoid));

int PASCAL ARU_PPDU_seq_dec PROTO((STOREHANDLE,unsigned long,rT041D_stAru FAR *,asn1_pvoid));
int PASCAL ARU_PPDU_seq_cod PROTO((STOREHANDLE,int,long,rT041D_stAru FAR *,asn1_pvoid));
int PASCAL ARU_PPDU_seq_ccod PROTO((STOREHANDLE,rT041D_stAru FAR *,asn1_pvoid));
int PASCAL ARU_PPDU_seq_dseq PROTO((STOREHANDLE,unsigned long int,rT041D_stAru FAR *,asn1_pvoid));

#endif	/* _ROPPMZZ_H */
