/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26svcis.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:00:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:51  root]
 * 
 * Revision 1.1.2.2  1994/06/09  18:46:14  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:18  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:13  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:11:55  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:53:12  marrek]
 * 
 * $EndLog$
 */
/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* File name    : d26svcis.h                                          */
/*                                                                    */
/* Description  : This Include-File contains "typedefs" and "defines" */
/*                used for the serviceability in GDS components using */
/*                C-ISAM                                              */
/*                                                                    */
/* Date         : 11/18/1993                                          */
/*                                                                    */
/* Author       : E. Kraemer, BU BA NM12                              */
/*                                                                    */
/**********************************************************************/

#ifndef _D26SVCIS_H
#define _D26SVCIS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d26svcis_rcsid[] = "@(#)$RCSfile: d26svcis.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:48 $";
#endif

#include <isam.h>

			    /* macros for the call of C-ISAM functions*/
#define D26_ISBUILD(attributes,filename,reclen,keydesc,mode)  \
	d26_isbuild(__FILE__,__LINE__,attributes,filename,reclen,keydesc, \
	    mode)

#define D26_ISADDINDEX(attributes,isfd,keydesc)  \
	d26_isaddindex(__FILE__,__LINE__,attributes,isfd,keydesc)

#define D26_ISOPEN(attributes,filename,mode)  \
	d26_isopen(__FILE__,__LINE__,attributes,filename,mode)

#define D26_ISCLOSE(attributes,isfd) \
	d26_isclose(__FILE__,__LINE__,attributes,isfd)

#define D26_ISUNIQUEID(attributes,isfd,uniqueid) \
	d26_isuniqueid(__FILE__,__LINE__,attributes,isfd,uniqueid)

#define D26_ISSTART(attributes,isfd,keydesc,len,record,mode) \
	d26_isstart(__FILE__,__LINE__,attributes,isfd,keydesc,len,record, \
	    mode)

#define D26_ISREAD(attributes,isfd,record,mode) \
	d26_isread(__FILE__,__LINE__,attributes,isfd,record,mode)

#define D26_ISWRITE(attributes,isfd,record) \
	d26_iswrite(__FILE__,__LINE__,attributes,isfd,record)

#define D26_ISREWRITE(attributes,isfd,record) \
	d26_isrewrite(__FILE__,__LINE__,attributes,isfd,record)

#define D26_ISREWCURR(attributes,isfd,record) \
	d26_isrewcurr(__FILE__,__LINE__,attributes,isfd,record)

#define D26_ISREWREC(attributes,isfd,rec_nr,record) \
	d26_isrewrec(__FILE__,__LINE__,attributes,isfd,rec_nr,record)

#define D26_ISDELCURR(attributes,isfd) \
	d26_isdelcurr(__FILE__,__LINE__,attributes,isfd)

#define D26_ISLOCK(attributes,isfd) \
	d26_islock(__FILE__,__LINE__,attributes,isfd)

#define D26_ISUNLOCK(attributes,isfd) \
	d26_isunlock(__FILE__,__LINE__,attributes,isfd)

#define D26_ISRELEASE(attributes,isfd) \
	d26_isrelease(__FILE__,__LINE__,attributes,isfd)


			    /* declarations of C-ISAM call replacement*/
extern int  d26_isbuild(char *file, int line, unsigned32 attributes,
		const char *filename, int reclen, struct keydesc *keydesc,
		int mode);

extern int  d26_isaddindex(char *file, int line, unsigned32 attributes,
		int isfd, struct keydesc *keydesc);

extern int  d26_isopen(char *file, int line, unsigned32 attributes,
		const char *filename, int mode);

extern int  d26_isclose(char *file, int line, unsigned32 attributes,
		int isfd);

extern int  d26_isuniqueid(char *file, int line, unsigned32 attributes,
		int isfd, long *uniqueid);

extern int  d26_isstart(char *file, int line, const unsigned32 attributes,
		int isfd, struct keydesc *keydesc, int len,
		const char *record, int mode);

extern int  d26_isread(char *file, int line, unsigned32 attributes,
		int isfd, char *record, int mode);

extern int  d26_iswrite(char *file, int line, unsigned32 attributes,
		int isfd, const char *record);

extern int  d26_isrewrite(char *file, int line, unsigned32 attributes,
		int isfd, const char *record);

extern int  d26_isrewcurr(char *file, int line, unsigned32 attributes,
		int isfd, const char *record);

extern int  d26_isrewrec(char *file, int line, unsigned32 attributes,
		int isfd, long rec_nr, const char *record);

extern int  d26_isdelcurr(char *file, int line, unsigned32 attributes,
		int isfd);

extern int  d26_islock(char *file, int line, unsigned32 attributes,
		int isfd);

extern int  d26_isunlock(char *file, int line, unsigned32 attributes,
		int isfd);

extern int  d26_isrelease(char *file, int line, unsigned32 attributes,
		int isfd);
#endif
