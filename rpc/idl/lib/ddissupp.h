/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: ddissupp.h,v $
 * Revision 1.1.2.2  1996/03/11  14:09:20  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:26  marty]
 *
 * Revision 1.1.2.1  1995/12/13  17:24:45  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:06 UTC  dat
 * 	Add new files for merge of second XIDL drop for DCE 1.2.1
 * 	[1995/12/13  17:23:52  root]
 * 
 * Revision 1.1.2.1  1995/11/01  14:22:16  bfc
 * 	idl cleanup
 * 	[1995/11/01  14:21:08  bfc]
 * 
 * $EndLog$
 */
/*
**  @OSF_COPYRIGHT@
**
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME:
**
**      ddissupp.h
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      Header file for common DDIS support.
**
**  VERSION: DCE 1.0
**
*/

#ifndef DDISSUPP_H
#define DDISSUPP_H

#ifdef VMS
/*
 * Constants and data for dynamic activation of DDIS image on VMS.
 */
#define IDL_DDIS_CLOSE_STREAM		0
#define IDL_DDIS_OPEN_STREAM		1
#define IDL_DDIS_CREATE_STREAM		2
#define IDL_DDIS_PUT			3
#define IDL_DDIS_GET_TAG		4
#define	IDL_DDIS_GET_VALUE		5
#define IDL_DDIS_PUT_VALUE		6

static $DESCRIPTOR(ddis_shareable_desc, "CDA$ACCESS");
static $DESCRIPTOR(ddis_close_stream_desc, "ddis$close_stream");
static $DESCRIPTOR(ddis_open_stream_desc, "ddis$open_stream");
static $DESCRIPTOR(ddis_create_stream_desc, "ddis$create_stream");
static $DESCRIPTOR(ddis_put_desc, "ddis$put");
static $DESCRIPTOR(ddis_get_tag_desc, "ddis$get_tag");
static $DESCRIPTOR(ddis_get_value_desc, "ddis$get_value");
static $DESCRIPTOR(ddis_put_value_desc, "ddis$put_value");

globalref idl_long_int (*ddis_g_entries[])();
#endif /* VMS */

/*
 * Data and macro to support conditional space stripping of MIA text types.
 */
extern idl_boolean IDL_space_strip_ev_xlated;
extern idl_boolean IDL_space_stripping;
#define IDL_MIA_SPACE_STRIPPING \
    ( (IDL_space_strip_ev_xlated) ? IDL_space_stripping : \
        (IDL_space_strip_ev_xlated = idl_true, \
         IDL_space_stripping = (getenv("ACMS_NO_SPACE_STRIPPING") == NULL)) )

#endif
