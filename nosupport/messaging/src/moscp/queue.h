/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * MODULE:
 * queue.h
 *
 * DESCRIPTION:
 * External declarations for manipulating queues.
 * ______________________________________________________________________
 *
 * $Log: queue.h,v $
 * Revision 1.6  1996/12/02  16:19:24  salamone
 * Remove queue rename
 *
 * Revision 1.5  96/11/29  13:08:17  13:08:17  salamone (Julie Salamone)
 * Fix queue enqueue
 * 
 * Revision 1.4  96/11/25  14:26:32  14:26:32  salamone (Julie Salamone)
 * Fix queue show
 * 
 * Revision 1.3  96/11/22  18:06:19  18:06:19  salamone (Julie Salamone)
 * Remove queue save & cancel
 * 
 * Revision 1.2  96/10/17  17:54:34  17:54:34  wwang (Weidong Wang)
 * added dequeue support
 * 
 * Revision 1.1  1996/10/07  16:40:43  wwang
 * Initial revision
 *
 * Revision 1.1  1996/10/01  17:07:12  wwang
 * Initial revision
 * 
 * $EndLog$
 * ______________________________________________________________________
 */
#ifndef _QUEUE_H
#define _QUEUE_H

#include <moscp.h>
#include <mosif.h>

/****************************************************************
 * prototypes
 ****************************************************************/
int dcp_que_create     (ClientData, Tcl_Interp *, int, char **); 
int dcp_que_catalog    (ClientData, Tcl_Interp *, int, char **); 
int dcp_que_delete     (ClientData, Tcl_Interp *, int, char **); 
int dcp_que_dequeue    (ClientData, Tcl_Interp *, int, char **); 
int dcp_que_enqueue    (ClientData, Tcl_Interp *, int, char **); 
int dcp_que_list       (ClientData, Tcl_Interp *, int, char **); 
int dcp_que_modify     (ClientData, Tcl_Interp *, int, char **); 
int dcp_que_move       (ClientData, Tcl_Interp *, int, char **); 
int dcp_que_remove     (ClientData, Tcl_Interp *, int, char **); 
int dcp_que_show       (ClientData, Tcl_Interp *, int, char **); 

#endif   /* _QUEUE_H */
