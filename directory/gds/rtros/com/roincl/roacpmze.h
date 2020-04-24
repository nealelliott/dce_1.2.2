/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roacpmze.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:18  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:38  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:25  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:13  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:24  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:30  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:47  keutel
 * 	creation
 * 	[1994/03/21  13:18:49  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROACPMZE_H
#define _ROACPMZE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roacpmze_rcsid[] = { "@(#) $RCSfile: roacpmze.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:37 $" } ;
#endif

/*
roacpmze.h,
Tue Nov 23 10:40:39 1993
*/


 /* Includes removed with sed */ 

/* TYPE MATCHING MACROS */
#define AARQ_apdu_match(matchz) ((matchz)[0] == 96)
#define AP_title_match(matchz) (((matchz)[0] == 32)\
||	Name_match(matchz)\
||	(((matchz)[0]|32) == 38))
#define AE_qualifier_match(matchz) (((matchz)[0] == 32)\
||	(((matchz)[0]|32) == 49)\
||	((matchz)[0] == 2))
#define Name_match(matchz) (((matchz)[0] == 32)\
||	((matchz)[0] == 48))
#define RelDisName_match(matchz) ((matchz)[0] == 49)
#define AttrValAss_match(matchz) ((matchz)[0] == 48)

/* CODING AND DECODING PROCEDURES */

int PASCAL AARQ_apdu_dec PROTO((STOREHANDLE,unsigned long,rT0641_aarqApdu FAR *,asn1_pvoid));
int PASCAL AARQ_apdu_cod PROTO((STOREHANDLE,int,long,rT0641_aarqApdu FAR *,asn1_pvoid));
int PASCAL AARQ_apdu_ccod PROTO((STOREHANDLE,rT0641_aarqApdu FAR *,asn1_pvoid));
int PASCAL AARQ_apdu_dseq PROTO((STOREHANDLE,unsigned long int,rT0641_aarqApdu FAR *,asn1_pvoid));

int PASCAL AP_title_dec PROTO((STOREHANDLE,unsigned long,long FAR *,rT0635_uApTitle FAR *));
int PASCAL AP_title_cod PROTO((STOREHANDLE,int,long,long FAR *,rT0635_uApTitle FAR *));
int PASCAL AE_qualifier_dec PROTO((STOREHANDLE,unsigned long,long FAR *,rT0637_uAeQualifier FAR *));
int PASCAL AE_qualifier_cod PROTO((STOREHANDLE,int,long,long FAR *,rT0637_uAeQualifier FAR *));
int PASCAL Name_dec PROTO((STOREHANDLE,unsigned long,rT0634_stName FAR *));
int PASCAL Name_cod PROTO((STOREHANDLE,int,long,rT0634_stName FAR *));
int PASCAL RelDisName_dec PROTO((STOREHANDLE,unsigned long,rT0631_stRDN FAR *));
int PASCAL RelDisName_cod PROTO((STOREHANDLE,int,long,rT0631_stRDN FAR *));
int PASCAL AttrValAss_dec PROTO((STOREHANDLE,unsigned long,rT0630_stAttriValueAssertion FAR *));
int PASCAL AttrValAss_cod PROTO((STOREHANDLE,int,long,rT0630_stAttriValueAssertion FAR *));
int PASCAL AttrValAss_ccod PROTO((STOREHANDLE,rT0630_stAttriValueAssertion FAR *));
int PASCAL AttrValAss_dseq PROTO((STOREHANDLE,unsigned long int,rT0630_stAttriValueAssertion FAR *));

#endif	/* _ROACPMZE_H */
