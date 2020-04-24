/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdstrace.h,v $
 * Revision 1.1.10.2  1996/02/18  23:38:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:08  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:37:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:30  root]
 * 
 * Revision 1.1.8.2  1994/02/24  15:03:37  marrek
 * 	Change rcs string format.
 * 	[1994/02/24  12:14:07  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:59:59  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:43  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:03:51  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:11:56  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:21:22  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  09:00:37  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  08:15:05  marrek
 * 	New HEADER and new argument in dsP_trace_msg()
 * 	[1993/02/01  14:15:57  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  23:30:53  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:20:53  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:11:34  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  16:06:14  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:17:32  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:43:52  melman]
 * 
 * $EndLog$
 */
#ifndef _XDSTRACE_H
#define _XDSTRACE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdstrace_rcsid[] = "@(#)$RCSfile: xdstrace.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:38:04 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xdstrace.h                                          */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    :                                                     */ 
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : declaration of tracing functions and macros         */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 90-10-01| Original                       | VR |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


#ifndef XDSTRACE_HEADER
#define XDSTRACE_HEADER

#include <xom.h>
#include <xds.h>


#ifdef TRACE

#define DEFAULT_TRACEFILE    "log_xds"      /* process id will be appended */

#define HEADERT "=========================================================================="
#define HEADER "--------------------------------------------------------------------------"
#define OPEN   "{"
#define CLOSE  "}"
#define TAB    2
#define MAXTOKLEN    32


/* exported TRACE functions */

void dsP_trace_msg (const OM_sint tid, const char *fmt, ...);
int dsP_display_object (const OM_sint tid, const OM_object arg, const char *arg_str);
int dsP_display_feature (const DS_feature *f, const char *arg);
int dsP_display_integer (const OM_sint i, const char *arg);
int dsP_display_boolean (const OM_sint b, const char *arg);


/* must be the first statement in a function */

#define TRACE_ENTER(f_name)             \
        static const char *fn = f_name; \
        dsP_trace_msg(2,HEADER"\n");    \
        dsP_trace_msg(1,"-> %s\n", fn); \
        dsP_trace_msg(1,"\n");

#define TRACE_EXIT                      \
        dsP_trace_msg(1,"\n");          \
        dsP_trace_msg(1,"<- %s\n", fn); \
        dsP_trace_msg(1,"\n");

#define TRACE_IARGS              \
        dsP_trace_msg(3,"   INPUT ARGUMENTS:\n");

#define TRACE_OARGS             \
        dsP_trace_msg(3,"\n");  \
        dsP_trace_msg(3,"\n");  \
        dsP_trace_msg(3,"   OUTPUT ARGUMENTS:\n");

#define TRACE_OERR             \
        dsP_trace_msg(3,"\n"); \
        dsP_trace_msg(3,"\n"); \
        dsP_trace_msg(3,"   ERROR RETURNED:\n");

#define TRACE_OBJECT(tid, object) \
        dsP_display_object(tid, object, #object);

#define TRACE_FEATURE(feature) \
        dsP_display_feature(feature, #feature);

#define TRACE_INTEGER(integer) \
        dsP_display_integer(integer, #integer);

#define TRACE_BOOLEAN(boolean) \
        dsP_display_boolean(boolean, #boolean);

#define TRACE_NOTE(mesg)        \
        dsP_trace_msg (1,"   NOTICE: %s\n", mesg);

#define TRACE_NOTE2(format, arg1, arg2)        \
        dsP_trace_msg (1,"   NOTICE: " format, arg1, arg2);

#define TRACE_NOTE3(format, arg1, octet_str, len)      \
{                                                      \
    char *p;                                  \
    char *q;                                  \
    int i;                                    \
                                                       \
    if ((p = q = (char *)malloc(len*2+1)) != NULL) {   \
        for (i=0; i < len; i++) {                      \
            sprintf(p, "%-2.2x", (unsigned char)octet_str[i]);     \
            p+=strlen(p);                              \
        }                                              \
        TRACE_NOTE2(format, arg1, q);                  \
        free(q);                                       \
    }                                                  \
}

#define TRACE_WARN(mesg)        \
        dsP_trace_msg (1,"   WARNING: %s\n", mesg);

#define TRACE_WARN2(format, arg1, arg2)        \
        dsP_trace_msg (1,"   WARNING: " format, arg1, arg2);

#define TRACE_ERROR(mesg)       \
        dsP_trace_msg (1,"   ERROR: %s\n", mesg);

#define TRACE_ERROR2(format, arg1, arg2)       \
        dsP_trace_msg (1,"   ERROR: " format, arg1, arg2);

#define TRACE_PANIC(mesg)       \
        dsP_trace_msg (1,"   PANIC: %s\n", mesg);

#define TRACE_PANIC2(format, arg1, arg2)       \
        dsP_trace_msg (1,"   PANIC: " format, arg1, arg2);


#define TRACE_SERVICE_CONTROLS(s_controls)       \
{ \
    OM_sint32 sc = s_controls; \
 \
    if (sc) { \
 \
        dsP_trace_msg(1,"   NOTICE: %s%s%s%s%s%s\n", \
                      (sc & D2_NOCHAINING) ? "ChainingProhib " : "", \
                      (sc & D2_DREFALIAS) ? "DontDerefAlias " : "", \
                      (sc & D2_NOCACHE) ? "DontUseCopy " : "", \
                      (sc & D2_LOCALSCOPE) ? "LocalScope " : "", \
                      (sc & D2_PREF_CHAINING) ? "PreferChaining " : "", \
                      (sc & D2_NOREFERRAL) ? "AutoContinuation " : "" \
                     ); \
 \
        dsP_trace_msg(1,"   NOTICE: %s%s%s%s%s%s%s%s\n", \
                      (sc & D2_DUAFIRST) ? "DUAFirst " : "", \
                      (sc & D2_DONT_STORE) ? "DontStore " : "", \
                      (sc & D2_NORMAL_CLASS) ? "NormalClass " : "", \
                      (sc & D2_PRIVILEGED_CLASS) ? "PrivClass " : "", \
                      (sc & D2_RESIDENT_CLASS) ? "ResClass " : "", \
                      (sc & D2_DUACACHE) ? "DUACache " : "", \
                      (sc & D2_USEDSA) ? "UseDSA " : "", \
                      (sc & D2_PREF_ADM_FUN) ? "PreferAdmFuncs " : "" \
                     ); \
    } \
    else \
        printf("   WARNING: no service-controls selected"); \
}


#else /* TRACE */


#define TRACE_ENTER(f_name)                    ;
#define TRACE_EXIT                             ;
#define TRACE_IARGS                            ;
#define TRACE_OARGS                            ;
#define TRACE_OERR                             ;
#define TRACE_OBJECT(object)                   ;
#define TRACE_FEATURE(feature)                 ;
#define TRACE_INTEGER(integer)                 ;
#define TRACE_BOOLEAN(boolean)                 ;
#define TRACE_NOTE(mesg)                       ;
#define TRACE_NOTE2(format,arg1,arg2)          ;
#define TRACE_NOTE3(format,arg1,octet_str,len) ;
#define TRACE_WARN(mesg)                       ;
#define TRACE_WARN2(format,arg1,arg2)          ;
#define TRACE_ERROR(mesg)                      ;
#define TRACE_ERROR2(format,arg1,arg2)         ;
#define TRACE_PANIC(mesg)                      ;
#define TRACE_PANIC2(format,arg1,arg2)         ;
#define TRACE_SERVICE_CONTROLS(s_controls)     ;


#endif /* TRACE */

#endif  /* XDSTRACE_HEADER */

#endif /* _XDSTRACE_H */
