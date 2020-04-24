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
 * $Log: gds_sec_util.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:43  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:02:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:26  root]
 * 
 * Revision 1.1.2.4  1994/09/23  12:16:41  marrek
 * 	Removed additional trace facility.
 * 	[1994/09/23  12:16:21  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:25  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:30  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:56:10  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:46:29  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:39:09  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:18:41  keutel
 * 	creation
 * 	[1994/03/21  15:53:10  keutel]
 * 
 * $EndLog$
 */
/*--------------------------------------------------------------------*/
/*                                                                    */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG          */
/* All Rights Reserved                                                */
/*                                                                    */
/* Project      : GDS SEC                                             */
/*                                                                    */
/* File Name    : gds_sec_util.h                                      */
/*                                                                    */
/* Description  : The header contains macro definitions used within   */
/*                the security layer                                  */
/*                                                                    */
/* Date         : December 21, 1993                                   */
/*                                                                    */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

#ifndef _GDS_SEC_UTIL_H
#define _GDS_SEC_UTIL_H

#if !defined lint && defined GDS_RCS_ID
static char gds_sec_util_rcsid[] = "@(#)$RCSfile: gds_sec_util.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:15 $";
#endif


/* macro to check the return value of sec functions */
#define GDS_SEC_CHECK_RETURN(fu,x,st) \
	{fprintf(stderr," %s %d %s value %d\n", __FILE__, __LINE__ ,(fu), (x));\
	 fprintf(stderr," retcode <%d>\t",(st)->d2_retcode);\
	 fprintf(stderr," errclass <%d>\t",(st)->d2_errclass);\
	 fprintf(stderr," errvalue <%d>\n",(st)->d2_errvalue);\
	 fprintf(stderr," logstring <%s>\n",(st)->logstring);\
	 fflush(stderr);\
	 }

#define GDS_SEC_TRACE0(trace_level,p1) \
{ \
char buffer[1024]; \
sprintf(buffer,"%s",p1);\
switch(trace_level){ \
	case 1:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug1,GDS_S_SEC_TRACE,buffer)); \
			break;\
	case 2:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug2,GDS_S_SEC_TRACE,buffer)); \
			break;\
	case 3:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,GDS_S_SEC_TRACE,buffer)); \
			break;\
	case 4:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug4,GDS_S_SEC_TRACE,buffer)); \
			break;\
	default:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug5,GDS_S_SEC_TRACE,buffer)); \
			break;\
	} \
}

#define GDS_SEC_TRACE1(trace_level,formatstr,p1) \
{ \
char buffer[1024]; \
sprintf(buffer,formatstr,p1);\
switch(trace_level){ \
	case 1:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug1,GDS_S_SEC_TRACE,buffer)); \
			break;\
	case 2:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug2,GDS_S_SEC_TRACE,buffer)); \
			break;\
	case 3:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,GDS_S_SEC_TRACE,buffer)); \
			break;\
	case 4:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug4,GDS_S_SEC_TRACE,buffer)); \
			break;\
	default:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug5,GDS_S_SEC_TRACE,buffer)); \
			break;\
	} \
} 

#define GDS_SEC_TRACE2(trace_level,formatstr,p1,p2) \
{ \
char buffer[1024];\
sprintf(buffer,formatstr,p1,p2);\
switch(trace_level){ \
	case 1:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug1,GDS_S_SEC_TRACE,buffer)); \
			break;\
	case 2:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug2,GDS_S_SEC_TRACE,buffer)); \
			break;\
	case 3:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,GDS_S_SEC_TRACE,buffer)); \
			break;\
	case 4:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug4,GDS_S_SEC_TRACE,buffer)); \
			break;\
	default:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug5,GDS_S_SEC_TRACE,buffer)); \
			break;\
	} \
} 

#define GDS_SEC_TRACE3(trace_level,formatstr,p1,p2,p3) \
{ \
char buffer[1024];\
sprintf(buffer,formatstr,p1,p2,p3);\
switch(trace_level){ \
	case 1:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug1,GDS_S_SEC_TRACE,buffer)); \
			break;\
	case 2:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug2,GDS_S_SEC_TRACE,buffer)); \
			break;\
	case 3:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug3,GDS_S_SEC_TRACE,buffer)); \
			break;\
	case 4:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug4,GDS_S_SEC_TRACE,buffer)); \
			break;\
	default:DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%s"),GDS_S_GENERAL,svc_c_debug5,GDS_S_SEC_TRACE,buffer)); \
			break;\
	} \
} 


#define GDS_SEC_TRACE_TOKEN(trace_level,str,tp) 

#define GDS_SEC_TRACE_NAME(trace_level,str,nm) 


#define GDS_SEC_SET_STATUS(st ,rc, ec, ev) \
{ \
	(st)->d2_retcode = (rc);\
	(st)->d2_errclass =(ec);\
	(st)->d2_errvalue =(ev);\
} 

/* macro to allocate memory including error handling */
#define GDS_SEC_MALLOC(M_pointer,M_type,M_size,st) \
	{M_pointer = (M_type) malloc((M_size)) ;\
	if ( M_pointer == (M_type) 0 ) \
		{ \
		(st)->d2_errvalue=D2_S_ALLOC_ERROR; \
		(st)->d2_retcode=D2_TEMP_ERR;\
		(st)->d2_errclass=D2_SYS_ERR;\
		sprintf((st)->logstring,"%d bytes malloc error in %s : %d\n",M_size,__FILE__,__LINE__); \
		DCE_SVC_LOG((GDS_S_NOMEMORY_MSG,M_size)); \
		return (D2_ERROR); \
		} \
	 }

/* prototypes for test functions */

void gds_sec__printtoken(
		/* IN */ D2_str  *t); 


#endif /* ifndef _GDS_SEC_UTIL_H */
