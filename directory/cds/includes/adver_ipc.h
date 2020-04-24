/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: adver_ipc.h,v $
 * Revision 1.1.4.2  1996/02/18  23:33:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:14:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:03  root]
 * 
 * Revision 1.1.2.3  1994/08/03  19:01:53  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:55:23  mccann]
 * 
 * Revision 1.1.2.2  1994/06/09  18:38:27  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:51  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/19  17:06:02  mccann
 * 	New module for code cleanup
 * 	[1994/04/19  16:39:19  mccann]
 * 
 * $EndLog$
 */
#ifndef _ADVER_IPC_H
#define _ADVER_IPC_H
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 */
#include <sys/un.h>
#include <dce/dns_record.h>

/*************************/
/* E R R O R  C O D E S  */
/*************************/
#define ADVER_BADVERSION 1

/*****************************************************/
/* P R O T O C O L  V E R S I O N  C O N S T A N T S */
/*****************************************************/
/* solicit and advertise */
#define MINMAJOR 1
#define MINMINOR 0

/* ipc */
#define CurrentIPCversion 1

/*******************************************/
/* S O L I C I T  A N D  A D V E R T I S E */
/*******************************************/
typedef struct SAHeader_t {
    protocolVersion_t  version;
    MessageType_t      msgtype;
    } SAHeader_t;

typedef bytes_u SAHeader_u[sizeof(protocolVersion_u) + sizeof(MessageType_u)];

typedef struct SAMsg_t {
    SAHeader_t header;
    char       *restOfMsg;
    } SAMsg_t;

typedef struct AdMsg_t {
    ObjUID_t     ns_uid;
    SimpleName_t ns_nickname;
    char         ch_count;
    } AdMsg_t;

typedef enum MsgType_t {
    AdvReq_t,      /* advertiser request  */
    SolReq_t,      /* solicit request     */
    BChldReq_t,    /* bind child request  */
    BChldRes_t,    /* bind child response */
    GenRes_t       /* generic response    */
	} MsgType_t;

typedef struct BChldMsgHdr_t {
    char               version;   /* ipc protocol version number */
    enum MsgType_t     msgType;
    int                infoLen;   /* actual length of principal (req) or saddr (res) */
    }BChldMsgHdr_t;

#define DNS_MAX_PRINC (255)

typedef struct BChldReqMsg_t {
    BChldMsgHdr_t hdr;
    char          principal[DNS_MAX_PRINC];
    } BChldReqMsg_t;

typedef struct BChldResMsg_t {
    BChldMsgHdr_t      hdr;
    struct sockaddr_un saddr;
    } BChldResMsg_t;


#define MsgMax (1492 /* max ethernet msg */ - 1 /* version */ - 12 /* msgType, advType, msgLen */ )

typedef enum AdvType_t {
    ResetMsg,    /* throw out existing msg's */
    EthernetMsg, /* message for ethernet device only */
    an802Msg,    /* message for 802 */
    PingMsg,     /* multicast existing stash of messages */
    QuitMsg      /* throw out existing msg's and quit answering solicits */
    } AdvType_t;

typedef struct AdvertiseRequest_t {
    char            version;
    enum MsgType_t  msgType;
    enum AdvType_t  advType;
    int             msgLen;
    char            msg[MsgMax];
    } AdvertiseRequest_t;

#define AdReqHdrSize ((sizeof(AdvertiseRequest_t)) - (MsgMax))
/*
 * following used to malloc advertise request messages that contain no data
 */
#define ShortAdvertiseRequestLen  AdReqHdrSize


#endif

