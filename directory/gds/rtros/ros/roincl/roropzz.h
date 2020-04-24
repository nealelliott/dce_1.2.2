/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropzz.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:27  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:06  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:19  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:36  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:29  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:38  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:10  keutel
 * 	creation
 * 	[1994/03/21  13:33:08  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROROPZZ_H
#define _ROROPZZ_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roropzz_rcsid[] = { "@(#) $RCSfile: roropzz.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:27 $" } ;
#endif

/*
roropzz.h,
Tue Nov 23 14:51:20 1993
*/

#include "rogenrl.h"
#include "rouply0.h"
#include "rostor0.h"
#include "roropm1.h"
#include "asn1st.h"
#define RTS_anydef_match(matchz) RTS_any_match(matchz)
/*
int PASCAL RTS_anydef_dec PROTO((
 STOREHANDLE              ,
 unsigned long            ,
 unsigned long FAR *      ,
 rTro76_stDefinedBy FAR * ,
 int                      ,
 void NEAR *                ));

int PASCAL RTS_anydef_cod PROTO((
 STOREHANDLE             ,
 int                     ,
 int                     ,
 unsigned long int FAR *             ,
 rTro76_stDefinedBy FAR *,
 int                     ,
 void NEAR  *              ));

*/


/* TYPE MATCHING MACROS */
#define ROSEapdus_match(matchz) (((matchz)[0] == 32)\
||	(((matchz)[0]|32) == 161)\
||	(((matchz)[0]|32) == 162)\
||	(((matchz)[0]|32) == 163)\
||	(((matchz)[0]|32) == 164))
#define ROIVapdu_match(matchz) ((matchz)[0] == 48)
#define RORSapdu_match(matchz) ((matchz)[0] == 48)
#define RORSapdu_stOperationResult_match(matchz) ((matchz)[0] == 48)
#define ROERapdu_match(matchz) ((matchz)[0] == 48)
#define RORJapdu_match(matchz) ((matchz)[0] == 48)
#define OpType_match(matchz) (((matchz)[0] == 32)\
||	((matchz)[0] == 2)\
||	(((matchz)[0]|32) == 38))
#define ErrType_match(matchz) (((matchz)[0] == 32)\
||	((matchz)[0] == 2)\
||	(((matchz)[0]|32) == 38))

/* CODING AND DECODING PROCEDURES */

int PASCAL ROSEapdus_dec PROTO((STOREHANDLE,unsigned long,rT0715_unRoseApdus FAR *,asn1_pvoid));
int PASCAL ROSEapdus_cod PROTO((STOREHANDLE,int,long,rT0715_unRoseApdus FAR *,asn1_pvoid));
int PASCAL ROIVapdu_dec PROTO((STOREHANDLE,unsigned long,rT0710_stROIVapdu FAR *,asn1_pvoid));
int PASCAL ROIVapdu_cod PROTO((STOREHANDLE,int,long,rT0710_stROIVapdu FAR *,asn1_pvoid));
int PASCAL ROIVapdu_ccod PROTO((STOREHANDLE,rT0710_stROIVapdu FAR *,asn1_pvoid));
int PASCAL ROIVapdu_dseq PROTO((STOREHANDLE,unsigned long int,rT0710_stROIVapdu FAR *,asn1_pvoid));

int PASCAL RORSapdu_dec PROTO((STOREHANDLE,unsigned long,rT0712_stRORSapdu FAR *,asn1_pvoid));
int PASCAL RORSapdu_cod PROTO((STOREHANDLE,int,long,rT0712_stRORSapdu FAR *,asn1_pvoid));
int PASCAL RORSapdu_ccod PROTO((STOREHANDLE,rT0712_stRORSapdu FAR *,asn1_pvoid));
int PASCAL RORSapdu_dseq PROTO((STOREHANDLE,unsigned long int,rT0712_stRORSapdu FAR *,asn1_pvoid));

int PASCAL RORSapdu_stOperationResult_dec PROTO((STOREHANDLE,unsigned long,rT0711_stOperationResult FAR *,asn1_pvoid));
int PASCAL RORSapdu_stOperationResult_cod PROTO((STOREHANDLE,int,long,rT0711_stOperationResult FAR *,asn1_pvoid));
int PASCAL RORSapdu_stOperationResult_ccod PROTO((STOREHANDLE,rT0711_stOperationResult FAR *,asn1_pvoid));
int PASCAL RORSapdu_stOperationResult_dseq PROTO((STOREHANDLE,unsigned long int,rT0711_stOperationResult FAR *,asn1_pvoid));

int PASCAL ROERapdu_dec PROTO((STOREHANDLE,unsigned long,rT0713_stROERapdu FAR *,asn1_pvoid));
int PASCAL ROERapdu_cod PROTO((STOREHANDLE,int,long,rT0713_stROERapdu FAR *,asn1_pvoid));
int PASCAL ROERapdu_ccod PROTO((STOREHANDLE,rT0713_stROERapdu FAR *,asn1_pvoid));
int PASCAL ROERapdu_dseq PROTO((STOREHANDLE,unsigned long int,rT0713_stROERapdu FAR *,asn1_pvoid));

int PASCAL RORJapdu_dec PROTO((STOREHANDLE,unsigned long,rT0714_stRORJapdu FAR *,asn1_pvoid));
int PASCAL RORJapdu_cod PROTO((STOREHANDLE,int,long,rT0714_stRORJapdu FAR *,asn1_pvoid));
int PASCAL RORJapdu_ccod PROTO((STOREHANDLE,rT0714_stRORJapdu FAR *,asn1_pvoid));
int PASCAL RORJapdu_dseq PROTO((STOREHANDLE,unsigned long int,rT0714_stRORJapdu FAR *,asn1_pvoid));

int PASCAL OpType_dec PROTO((STOREHANDLE,unsigned long,rTro76_stDefinedBy FAR *));
int PASCAL OpType_cod PROTO((STOREHANDLE,int,long,rTro76_stDefinedBy FAR *));
int PASCAL ErrType_dec PROTO((STOREHANDLE,unsigned long,rTro76_stDefinedBy FAR *));
int PASCAL ErrType_cod PROTO((STOREHANDLE,int,long,rTro76_stDefinedBy FAR *));

#endif	/* _ROROPZZ_H */
