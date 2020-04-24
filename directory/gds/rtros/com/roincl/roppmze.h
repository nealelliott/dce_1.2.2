/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppmze.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:06  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:05  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:35  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:42  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:31  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:57  keutel
 * 	creation
 * 	[1994/03/21  13:19:20  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPMZE_H
#define _ROPPMZE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppmze_rcsid[] = { "@(#) $RCSfile: roppmze.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:55 $" } ;
#endif

/*
roppmze.h,
Tue Nov 23 10:33:03 1993
*/


 /* Includes removed with sed */ 

typedef rT0412_stPresContDef    FAR * asn1_prT0412_stPresContDef;
typedef unsigned long int       FAR * asn1_prTro00_vObjectId; 



/* TYPE MATCHING MACROS */
#define CP_type_match(matchz) ((matchz)[0] == 49)
#define Context_list_match(matchz) ((matchz)[0] == 48)
#define Context_list_seq_match(matchz) ((matchz)[0] == 48)
#define Default_context_name_match(matchz) ((matchz)[0] == 48)
#define Mode_selector_match(matchz) ((matchz)[0] == 49)
#define Fully_encoded_data_match(matchz) ((matchz)[0] == 48)
#define CP_type_seq_match(matchz) ((matchz)[0] == 48)

/* CODING AND DECODING PROCEDURES */

int PASCAL CP_type_dec PROTO((STOREHANDLE,unsigned long,rT041A_stCp FAR *,asn1_pvoid));
int PASCAL CP_type_cod PROTO((STOREHANDLE,int,long,rT041A_stCp FAR *,asn1_pvoid));
int PASCAL CP_type_ccod PROTO((STOREHANDLE,rT041A_stCp FAR *,asn1_pvoid));
int PASCAL Context_list_dec PROTO((STOREHANDLE,unsigned long,ulong FAR *,asn1_prT0412_stPresContDef));
int PASCAL Context_list_cod PROTO((STOREHANDLE,int,long,ulong FAR *,asn1_prT0412_stPresContDef));
int PASCAL Context_list_seq_dec PROTO((STOREHANDLE,unsigned long,rT0412_stPresContDef FAR *));
int PASCAL Context_list_seq_cod PROTO((STOREHANDLE,int,long,rT0412_stPresContDef FAR *));
int PASCAL Context_list_seq_ccod PROTO((STOREHANDLE,rT0412_stPresContDef FAR *));
int PASCAL Context_list_seq_dseq PROTO((STOREHANDLE,unsigned long int,rT0412_stPresContDef FAR *));

int PASCAL Default_context_name_dec PROTO((STOREHANDLE,unsigned long,asn1_prTro00_vObjectId,asn1_prTro00_vObjectId));
int PASCAL Default_context_name_cod PROTO((STOREHANDLE,int,long,asn1_prTro00_vObjectId,asn1_prTro00_vObjectId));
int PASCAL Default_context_name_ccod PROTO((STOREHANDLE,asn1_prTro00_vObjectId,asn1_prTro00_vObjectId));
int PASCAL Default_context_name_dseq PROTO((STOREHANDLE,unsigned long int,asn1_prTro00_vObjectId,asn1_prTro00_vObjectId));

int PASCAL Mode_selector_dec PROTO((STOREHANDLE,unsigned long,long FAR *));
int PASCAL Mode_selector_cod PROTO((STOREHANDLE,int,long,long FAR *));
int PASCAL Mode_selector_ccod PROTO((STOREHANDLE,long FAR *));
int PASCAL Fully_encoded_data_dec PROTO((STOREHANDLE,unsigned long,rT0415_stFully FAR *,asn1_pvoid));
int PASCAL Fully_encoded_data_cod PROTO((STOREHANDLE,int,long,rT0415_stFully FAR *,asn1_pvoid));
int PASCAL CP_type_seq_dec PROTO((STOREHANDLE,unsigned long,rT041A_stCp FAR *,asn1_pvoid));
int PASCAL CP_type_seq_cod PROTO((STOREHANDLE,int,long,rT041A_stCp FAR *,asn1_pvoid));
int PASCAL CP_type_seq_ccod PROTO((STOREHANDLE,rT041A_stCp FAR *,asn1_pvoid));
int PASCAL CP_type_seq_dseq PROTO((STOREHANDLE,unsigned long int,rT041A_stCp FAR *,asn1_pvoid));

#endif	/* _ROPPMZE_H */
