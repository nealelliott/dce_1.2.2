/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppmzx.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:03  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:09  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:07  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:38  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:43  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:39  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:00  keutel
 * 	creation
 * 	[1994/03/21  13:19:24  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPMZX_H
#define _ROPPMZX_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppmzx_rcsid[] = { "@(#) $RCSfile: roppmzx.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:57 $" } ;
#endif

/*
roppmzz.h,
Tue Jan 28 08:12:06 1992
*/

/* ********************************************************************** */
/*                                                                        */
signed short int PASCAL asn1st_matchctags PROTO ((
				     unsigned long int hdl, /* STORHANDLE */
				     int tag1,
				     int tag2,
				     int tag3));
/*                                                                        */
/*                                                                        */
/*  Returnvalue : Error  < 0  if StoreError occures                       */
/*                0           if one of the input tags == actual byte     */
/*                RC041l_UnrecognizedPpdu   otherwise;                    */
/*                                                                        */
/* ********************************************************************** */

#define CP_type_tag(hdl)       asn1st_matchctags(hdl,49,0,0)    /* SET */  
#define CPA_PPDU_tag(hdl)      asn1st_matchctags(hdl,49,0,0)    /* SET */  
#define CPR_PPDU_tag(hdl)      asn1st_matchctags(hdl,49,48,0)              
							/* SET, SEQUENCE */
#define Abort_type_tag(hdl)    asn1st_matchctags(hdl,49,160,48)            
						   /* SET, [0], SEQUENCE */
#define User_data_tag(hdl)     asn1st_matchctags(hdl,96,97,0)
				      /* [APPLICATION 0], APPLICATION 1] */


typedef unsigned long int STOREHANDLE;
typedef void NEAR * asn1_pvoid;

/* CODING AND DECODING PROCEDURES */

int PASCAL CPA_PPDU_dec PROTO((STOREHANDLE,unsigned long,rT041B_stCpa FAR *,asn1_pvoid));
int PASCAL CPA_PPDU_cod PROTO((STOREHANDLE,int,long,rT041B_stCpa FAR *,asn1_pvoid));

int PASCAL CPR_PPDU_dec PROTO((STOREHANDLE,unsigned long,rT041C_stCpr FAR *,asn1_pvoid));
int PASCAL CPR_PPDU_cod PROTO((STOREHANDLE,int,long,rT041C_stCpr FAR *,asn1_pvoid));

int PASCAL Abort_type_dec PROTO((STOREHANDLE,unsigned long,rT041G_unPpdu FAR *,asn1_pvoid));
int PASCAL Abort_type_cod PROTO((STOREHANDLE,int,long,rT041G_unPpdu FAR *,asn1_pvoid));

int PASCAL ARU_PPDU_dec PROTO((STOREHANDLE,unsigned long,rT041D_stAru FAR *,asn1_pvoid));
int PASCAL ARU_PPDU_cod PROTO((STOREHANDLE,int,long,rT041D_stAru FAR *,asn1_pvoid));

int PASCAL ARP_PPDU_dec PROTO((STOREHANDLE,unsigned long,rT041E_stArp FAR *));
int PASCAL ARP_PPDU_cod PROTO((STOREHANDLE,int,long,rT041E_stArp FAR *));

int PASCAL CP_type_dec PROTO((STOREHANDLE,unsigned long,rT041A_stCp FAR *,asn1_pvoid));
int PASCAL CP_type_cod PROTO((STOREHANDLE,int,long,rT041A_stCp FAR *,asn1_pvoid));

int PASCAL User_data_dec PROTO((STOREHANDLE,unsigned long,rT0417_stUserData FAR *,asn1_pvoid));
int PASCAL User_data_cod PROTO((STOREHANDLE,int,long,rT0417_stUserData FAR *,asn1_pvoid));

#endif	/* _ROPPMZX_H */
