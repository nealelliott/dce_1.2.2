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
 * $Log: gncpmain.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:38  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:52  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:07:10  takano
 * 	First Submit
 * 	[1996/02/21  08:07:06  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <process.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <ncpext.h>
#include <nwconn.h>
#include <nwbindry.h>
#include <fcntl.h>
#include <io.h>
#include <stddef.h>
#include <ctype.h>
#include <errno.h>
#include <niterror.h>

#include "dfamdef.h"
#include "dfamext.h"
#include "dfamstr.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "err.h"
#include "gfiler.h"

/*EnglishComment**************************************************************/
/*  ID   = fnCommandListen                                                   */
/*  name = NCP Extension handling routine                                    */
/*  func = This routine is an NCP Extension handler that analyzes a request  */
/*         from a NetWare client.                                            */
/*         (1) Analyzes the request from a NetWare client.                   */
/*         (2) If the request is LGIN, PWDO, PWDN, LCHK:                     */
/*                Changes the thread group ID so that the context is the     */
/*                same as the main thread group.                             */
/*             If the request is other than LGIN, PWDO, PWDN, LCHK:          */
/*                Changes the thread group ID so that the context is the     */
/*                same as the thread group corresponding to the connection   */
/*                number.                                                    */
/*         (3) Asks processing corresponding to each request.                */
/*         (4) Takes the trace for the requested data and corresponding data.*/
/*                                                                           */
/*  i/o  = pClient     : NCPExtensionClient * : i  : client information      */
/*         pReqdat     : void *               : i  : receiving buffer        */
/*         uiReqlen    : LONG                 : i  : receiving buffer length */
/*         pReplydfata : void *               : i/o: sending buffer          */
/*         puiReplylen : LONG *               : i/o: sending buffer length   */
/*                                                                           */
/*  return = 0                                                               */
/*  common = none                                                            */
/*  note   = If already logged in to DCE, this routine continues even if     */
/*           there is no DCED user property (i.e., Gateway user registration */
/*           has been deleted).                                              */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


BYTE fnCommandListen(pClient, pReqdat, uiReqlen, pReplydata, puiReplylen)

struct NCPExtensionClient *pClient;
void *pReqdat;
LONG uiReqlen;
void *pReplydata;
LONG *puiReplylen;
{
    struct GTGroup_t *pTGroup;
    long uiObjID;
    int iRc;
    int iMyThreadGroupID;
    WORD usObjType;
    char aszCommand[12];
    char aszObjName[MAX_NWUSERNAME];
    char aszSegment[SEGMENTNUM];
    char aszLogComment[12];
    BYTE byFlags;
    BYTE byMore;
    BYTE byLoginTime[7];

    iMyThreadGroupID = GetThreadGroupID();

    iRc = SetThreadGroupID(iMain_tgid);
    if (iRc == EFAILURE) {

        memcpy( ((struct NFILRecvFrame *)pReplydata)->asComName, (unsigned char *)pReqdat,
                  COMLEN);
        *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));

        if ((memcmp((unsigned char *)pReqdat, NCPLOGIN, COMLEN) == 0)
          ||(memcmp((unsigned char *)pReqdat, NCPLOGOUT, COMLEN) == 0)
          ||(memcmp((unsigned char *)pReqdat, NCPOLDPASSWD, COMLEN) == 0)
          ||(memcmp((unsigned char *)pReqdat, NCPNEWPASSWD, COMLEN) == 0)
          ||(memcmp((unsigned char *)pReqdat, NCPLOGINCHK, COMLEN) == 0)) {
            ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_FAILURE;
        }
        else {
            ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)INTERE;
        }
        return 0;
    }

    GW_tsh_functrace(FUNC_FNCOMMANDLISTEN, GW_FUNC_START, 0);

    memcpy(aszCommand, (unsigned char *)pReqdat, COMLEN);
    aszCommand[COMLEN] = '\0';
    GW_tsh_trace(pMainTracep, COM_TR_SIZE, TSH_RECV, pClient->connection, uiReqlen,
                 aszCommand);





    GetConnectionInformation((WORD)(pClient->connection), aszObjName, &usObjType,  &uiObjID, byLoginTime);

    if (usObjType != OT_USER) {

        memcpy( ((struct NFILRecvFrame *)pReplydata)->asComName, (unsigned char *)pReqdat, COMLEN);

        *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));
        ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_NOT_OT_USER;




        sprintf(aszLogComment, "%hu", usObjType);
        GW_tsh_log(1100, aszLogComment);
        NCP_LogComment(pClient->connection, aszObjName);

        goto NCPend;
    }




    if (memcmp((unsigned char *)pReqdat, NCPLOGIN, COMLEN) == 0) {

        pTGroup = GetTGrp(pClient->connection);

        if (pTGroup == NULL) {

            iRc = ReadPropertyValue(aszObjName, OT_USER, DCE_USER_PROPERTY, 1, aszSegment, &byMore, &byFlags);

            switch (iRc) {
              case ESUCCESS :
                break;

              case ERR_NO_SUCH_PROPERTY :
              case ERR_NO_SUCH_SEGMENT  :
                memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPLOGIN, COMLEN);

                ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_NOT_DFAM_USER;
                *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));
                goto NCPend;
                break;

              case ERR_SERVER_OUT_OF_MEMORY :
                memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPLOGIN, COMLEN);

                ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
                *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));
                GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
                NCP_LogComment(pClient->connection, aszObjName);
                goto NCPend;
                break;

              case ERR_WILDCARD_NOT_ALLOWED :
                memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPLOGIN, COMLEN);

                ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_FAILURE;
                *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));


                GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);

                sprintf(aszLogComment, "%d", iRc);
                GW_tsh_log(1101, aszLogComment);
                NCP_LogComment(pClient->connection, aszObjName);

                goto NCPend;
                break;

              default :
                memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPLOGIN, COMLEN);

                ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_BINDERY_ACCESS_FAILURE;
                *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));


                GW_Message(DFAM_CON, 0, LOG_YES, KDDS11215, iRc);

                sprintf(aszLogComment, "%d", iRc);
                GW_tsh_log(1102, aszLogComment);
                NCP_LogComment(pClient->connection, aszObjName);

                goto NCPend;
                break;
            }
        }
        else {

            memcpy(((struct LGINRecvFrame *)pReplydata)->asComName, NCPLOGIN, COMLEN);
            ((struct LGINRecvFrame *)pReplydata)->ushRc = (unsigned short)LGIN_ALREADY_LOGIN;
            ((struct LGINRecvFrame *)pReplydata)->ushLen = (unsigned short)0;
            *puiReplylen = (LONG)(sizeof(struct LGINRecvFrame) - 1);

            goto NCPend;
        }

        if (bIsConnection_live(pAgent_sp) == FALSE) {

            memcpy(((struct LGINRecvFrame *)pReplydata)->asComName, NCPLOGIN, COMLEN);
            ((struct LGINRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_AGT_DOWN;
            ((struct LGINRecvFrame *)pReplydata)->ushLen = (unsigned short)0;
            *puiReplylen = (LONG)(sizeof(struct LGINRecvFrame) - 1);
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11217);
            NCP_LogComment(pClient->connection, aszObjName);

            goto NCPend;
        }

        if (pAgent_sp->bLogin_Stop == TRUE) {

            memcpy(((struct LGINRecvFrame *)pReplydata)->asComName, NCPLOGIN, COMLEN);
            ((struct LGINRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_BUSY;
            ((struct LGINRecvFrame *)pReplydata)->ushLen = (unsigned short)0;
            *puiReplylen = (LONG)(sizeof(struct LGINRecvFrame) - 1);

            goto NCPend;
        }


        NCP_DCELogin(pClient->connection, pReplydata, puiReplylen);
        sprintf(aszLogComment, "%p", pTGroup_sp);
        GW_tsh_log(1114, aszLogComment);

        sprintf(aszLogComment, "%d", iLoginUsersNum);
        GW_tsh_log(1115, aszLogComment);

    }

    else if (memcmp((unsigned char *)pReqdat, NCPLOGOUT, COMLEN) == 0) {

        pTGroup = GetTGrp(pClient->connection);

        if (pTGroup == NULL) {

            iRc = ReadPropertyValue(aszObjName, OT_USER, DCE_USER_PROPERTY, 1, aszSegment, &byMore, &byFlags);

            switch (iRc) {
              case ESUCCESS :

                memcpy( ((struct NFILRecvFrame *)pReplydata)->asComName, NCPLOGOUT, COMLEN);

                ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_NOT_DCE_LOGIN;
                *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));
                break;

              case ERR_NO_SUCH_PROPERTY :
              case ERR_NO_SUCH_SEGMENT  :
                memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPLOGOUT, COMLEN);

                ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_NOT_DFAM_USER;
                *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));
                break;

              case ERR_SERVER_OUT_OF_MEMORY :
                memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPLOGOUT, COMLEN);

                ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
                *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));
                GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
                NCP_LogComment(pClient->connection, aszObjName);
                break;

              case ERR_WILDCARD_NOT_ALLOWED :
                memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPLOGOUT, COMLEN);

                ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_FAILURE;
                *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));


                GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);

                sprintf(aszLogComment, "%d", iRc);
                GW_tsh_log(1103, aszLogComment);
                NCP_LogComment(pClient->connection, aszObjName);

                break;

              default :
                memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPLOGOUT, COMLEN);

                ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_BINDERY_ACCESS_FAILURE;
                *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));


                GW_Message(DFAM_CON, 0, LOG_YES, KDDS11215, iRc);

                sprintf(aszLogComment, "%d", iRc);
                GW_tsh_log(1104, aszLogComment);
                NCP_LogComment(pClient->connection, aszObjName);

                break;
            }

            goto NCPend;
        }

        if ((pTGroup->iLoginStatus == 1)
          ||(pTGroup->iLogoutStatus == 1)) {

            memcpy( ((struct LGOTRecvFrame *)pReplydata)->asComName, NCPLOGOUT, COMLEN);
            ((struct LGOTRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_BUSY;
            ((struct LGOTRecvFrame *)pReplydata)->ushLen = (unsigned short)0;
            *puiReplylen = (LONG)(sizeof(struct LGOTRecvFrame) - 1);

            goto NCPend;
        }

        if (bIsConnection_live(pAgent_sp) == FALSE) {
            pTGroup->iLogoutStatus = 1;
            ResumeThread(pTGroup->iThreadID);

            memcpy( ((struct LGOTRecvFrame *)pReplydata)->asComName, NCPLOGOUT, COMLEN);
            ((struct LGOTRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_AGT_DOWN;
            ((struct LGOTRecvFrame *)pReplydata)->ushLen = (unsigned short)0;
            *puiReplylen = (LONG)(sizeof(struct LGOTRecvFrame) - 1);
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11217);
            NCP_LogComment(pClient->connection, aszObjName);

            goto NCPend;
        }

        pTGroup->iLogoutStatus = 1;

        iRc = SetThreadGroupID(pTGroup->iTGrpID);
        if (iRc == EFAILURE) {
            memcpy( ((struct LGOTRecvFrame *)pReplydata)->asComName, NCPLOGOUT, COMLEN);
            ((struct LGOTRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_FAILURE;
            ((struct LGOTRecvFrame *)pReplydata)->ushLen = (unsigned short)0;
            *puiReplylen = (LONG)(sizeof(struct LGOTRecvFrame) - 1);
            pTGroup->iLogoutStatus = 0;

            goto NCPend;
        }




        NCP_DCELogout(pTGroup, pReplydata, puiReplylen);

        SetThreadGroupID(iMain_tgid);
    }

    else if (memcmp((unsigned char *)pReqdat, NCPOLDPASSWD, COMLEN) == 0) {

        iRc = ReadPropertyValue(aszObjName, OT_USER, DCE_USER_PROPERTY, 1, aszSegment, &byMore, &byFlags);


        switch (iRc) {
          case ESUCCESS :
            break;

          case ERR_NO_SUCH_PROPERTY :
          case ERR_NO_SUCH_SEGMENT :
            memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPOLDPASSWD, COMLEN);

            ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_NOT_DFAM_USER;
            *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));
            goto NCPend;
            break;

          case ERR_SERVER_OUT_OF_MEMORY :
            memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPOLDPASSWD, COMLEN);

            ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
            *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
            NCP_LogComment(pClient->connection, aszObjName);
            goto NCPend;
            break;

          case ERR_WILDCARD_NOT_ALLOWED :
            memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPOLDPASSWD, COMLEN);

            ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_FAILURE;
            *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));



            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);

            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1105, aszLogComment);
            NCP_LogComment(pClient->connection, aszObjName);

            goto NCPend;
            break;

          default :
            memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPOLDPASSWD, COMLEN);

            ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_BINDERY_ACCESS_FAILURE;
            *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));


            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11215, iRc);

            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1106, aszLogComment);
            NCP_LogComment(pClient->connection, aszObjName);

            goto NCPend;
            break;
        }





        NCP_DCEOldPasswd(pClient->connection, pReqdat, uiReqlen, pReplydata, puiReplylen);
    }

    else if (memcmp((unsigned char *)pReqdat, NCPNEWPASSWD, COMLEN) == 0) {

        iRc = ReadPropertyValue(aszObjName, OT_USER, DCE_USER_PROPERTY, 1, aszSegment, &byMore, &byFlags);


        switch (iRc) {
          case ESUCCESS :
            break;

          case ERR_NO_SUCH_PROPERTY :
          case ERR_NO_SUCH_SEGMENT  :
            memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPNEWPASSWD, COMLEN);

            ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_NOT_DFAM_USER;
            *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));
            goto NCPend;
            break;

          case ERR_SERVER_OUT_OF_MEMORY :
            memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPNEWPASSWD, COMLEN);

            ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_OUT_OF_MEMORY;
            *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));
            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
            NCP_LogComment(pClient->connection, aszObjName);
            goto NCPend;
            break;

          case ERR_WILDCARD_NOT_ALLOWED :
            memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPNEWPASSWD, COMLEN);

            ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_FAILURE;
            *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));



            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);

            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1107, aszLogComment);
            NCP_LogComment(pClient->connection, aszObjName);

            goto NCPend;
            break;

          default :
            memcpy(((struct NFILRecvFrame *)pReplydata)->asComName, NCPNEWPASSWD, COMLEN);

            ((struct NFILRecvFrame *)pReplydata)->ushRc = (unsigned short)NCP_BINDERY_ACCESS_FAILURE;
            *puiReplylen = (LONG)(sizeof(struct NFILRecvFrame));


            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11215, iRc);

            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1108, aszLogComment);
            NCP_LogComment(pClient->connection, aszObjName);

            goto NCPend;
            break;
        }





        NCP_DCENewPasswd(pClient->connection, pReqdat, uiReqlen, pReplydata, puiReplylen);
    }

    else if (memcmp((unsigned char *)pReqdat, NCPLOGINCHK, COMLEN) == 0) {




        NCP_DCELoginChk(pClient->connection, aszObjName, pReplydata, puiReplylen);


    }

    else if (memcmp((unsigned char *)pReqdat, NCPCHKVOL, COMLEN) == 0) {




  		fnChkvol(pReqdat, uiReqlen, pReplydata, puiReplylen);

    }

    else {

        pTGroup = GetTGrp(pClient->connection);
        if (pTGroup == NULL) {

            iRc = ReadPropertyValue(aszObjName, OT_USER, DCE_USER_PROPERTY, 1, aszSegment, &byMore, &byFlags);

            switch (iRc) {
              case ESUCCESS :
                memcpy( ((struct RetHeader *)pReplydata)->command_name, (unsigned char *)pReqdat, COMLEN);

                ((struct RetHeader *)pReplydata)->DFAM_errno = (unsigned short)NCP_NOT_DCE_LOGIN;
                ((struct RetHeader *)pReplydata)->ret_value = (unsigned short)FALSE;
                ((struct RetHeader *)pReplydata)->next_frame = (unsigned short)FALSE;
                *puiReplylen = (LONG)(sizeof(struct RetHeader));
                break;

              case ERR_NO_SUCH_PROPERTY :
              case ERR_NO_SUCH_SEGMENT  :
                memcpy( ((struct RetHeader *)pReplydata)->command_name, (unsigned char *)pReqdat, COMLEN);

                ((struct RetHeader *)pReplydata)->DFAM_errno = (unsigned short)NCP_NOT_DFAM_USER;
                ((struct RetHeader *)pReplydata)->ret_value = (unsigned short)FALSE;
                ((struct RetHeader *)pReplydata)->next_frame = (unsigned short)FALSE;
                *puiReplylen = (LONG)(sizeof(struct RetHeader));
                break;

              case ERR_SERVER_OUT_OF_MEMORY :
                memcpy( ((struct RetHeader *)pReplydata)->command_name, (unsigned char *)pReqdat, COMLEN);

                ((struct RetHeader *)pReplydata)->DFAM_errno = (unsigned short)NCP_OUT_OF_MEMORY;
                ((struct RetHeader *)pReplydata)->ret_value = (unsigned short)FALSE;
                ((struct RetHeader *)pReplydata)->next_frame = (unsigned short)FALSE;
                *puiReplylen = (LONG)(sizeof(struct RetHeader));

                GW_Message(DFAM_CON, 0, LOG_YES, KDDS11214);
                NCP_LogComment(pClient->connection, aszObjName);
                break;

              case ERR_WILDCARD_NOT_ALLOWED :
                memcpy( ((struct RetHeader *)pReplydata)->command_name, (unsigned char *)pReqdat, COMLEN);

                ((struct RetHeader *)pReplydata)->DFAM_errno = (unsigned short)INTERE;
                ((struct RetHeader *)pReplydata)->ret_value = (unsigned short)FALSE;
                ((struct RetHeader *)pReplydata)->next_frame = (unsigned short)FALSE;
                *puiReplylen = (LONG)(sizeof(struct RetHeader));



                GW_Message(DFAM_CON, 0, LOG_YES, KDDS11201, iRc);

                sprintf(aszLogComment, "%d", iRc);
                GW_tsh_log(1109, aszLogComment);
                NCP_LogComment(pClient->connection, aszObjName);

                break;

              default :
                memcpy( ((struct RetHeader *)pReplydata)->command_name, (unsigned char *)pReqdat, COMLEN);


                ((struct RetHeader *)pReplydata)->DFAM_errno = (unsigned short)NCP_BINDERY_ACCESS_FAILURE;
                ((struct RetHeader *)pReplydata)->ret_value = (unsigned short)FALSE;
                ((struct RetHeader *)pReplydata)->next_frame = (unsigned short)FALSE;
                *puiReplylen = (LONG)(sizeof(struct RetHeader));



                GW_Message(DFAM_CON, 0, LOG_YES, KDDS11215, iRc);

                sprintf(aszLogComment, "%d", iRc);
                GW_tsh_log(1110, aszLogComment);
                NCP_LogComment(pClient->connection, aszObjName);

                break;
            }
            goto NCPend;
        }

        if ((pTGroup->iLoginStatus == 1)
          ||(pTGroup->iLogoutStatus == 1)) {

            memcpy( ((struct RetHeader *)pReplydata)->command_name, (unsigned char *)pReqdat, COMLEN);

            ((struct RetHeader *)pReplydata)->DFAM_errno = (unsigned short)NCP_BUSY;
            ((struct RetHeader *)pReplydata)->ret_value = (unsigned short)FALSE;
            ((struct RetHeader *)pReplydata)->next_frame = (unsigned short)FALSE;
            *puiReplylen = (LONG)(sizeof(struct RetHeader));

            goto NCPend;
        }

        if (bIsConnection_live(pAgent_sp) == FALSE) {
            pTGroup->iLogoutStatus = 1;
            ResumeThread(pTGroup->iThreadID);

            memcpy( ((struct RetHeader *)pReplydata)->command_name, (unsigned char *)pReqdat, COMLEN);

            ((struct RetHeader *)pReplydata)->DFAM_errno = (unsigned short)NCP_AGT_DOWN;
            ((struct RetHeader *)pReplydata)->ret_value = (unsigned short)FALSE;
            ((struct RetHeader *)pReplydata)->next_frame = (unsigned short)FALSE;
            *puiReplylen = (LONG)(sizeof(struct RetHeader));

            GW_Message(DFAM_CON, 0, LOG_YES, KDDS11217);
            NCP_LogComment(pClient->connection, aszObjName);

            goto NCPend;
        }

        pTGroup->iExistCommand++;

        iRc = SetThreadGroupID(pTGroup->iTGrpID);
        if (iRc == EFAILURE) {
            pTGroup->iExistCommand--;

            memcpy( ((struct RetHeader *)pReplydata)->command_name, (unsigned char *)pReqdat, COMLEN);

            ((struct RetHeader *)pReplydata)->DFAM_errno = (unsigned short)INTERE;
            ((struct RetHeader *)pReplydata)->ret_value = (unsigned short)FALSE;
            ((struct RetHeader *)pReplydata)->next_frame = (unsigned short)FALSE;
            *puiReplylen = (LONG)(sizeof(struct RetHeader));

            goto NCPend;
        }




        fnExec_Filer(pTGroup, pClient->task, pReqdat, uiReqlen, pReplydata, puiReplylen);


        SetThreadGroupID(iMain_tgid);

        pTGroup->iExistCommand--;
    }


NCPend:

    sprintf(aszCommand + COMLEN, "%hu", ((struct NFILRecvFrame *)pReplydata)->ushRc);
    GW_tsh_trace(pMainTracep, COM_TR_SIZE, TSH_SEND, pClient->connection, *puiReplylen,
                 aszCommand);

    GW_tsh_functrace(FUNC_FNCOMMANDLISTEN, GW_FUNC_END, 0);



    SetThreadGroupID(iMyThreadGroupID);
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = fnConnDown                                                        */
/*  name = Call back routine when the connection is closed (Connection Event */
/*         Handler)                                                          */
/*  func = (1) If an even is CONNECTION_BEING_KILLED or CONNECTION_BEING_    */
/*             LOGGED_OUT:                                                   */
/*               Issues a DCE logout request.                                */
/*         (2) If an event is CONNECTION_BEING_RESTARTED or CONNECTION_      */
/*             BEING_FREED:                                                  */
/*               Does nothing.                                               */
/*                                                                           */
/*  i/o  = uiConnNum : LONG : i : connection number                          */
/*         uiEvent   : LONG : i : event                                      */
/*                                                                           */
/*  return = none                                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


void fnConnDown(uiConnNum, uiEvent)
LONG uiConnNum;
LONG uiEvent;
{
    struct GTGroup_t *pTGroup;
    long iObjID;
    int iMyThreadGroupID;
    int iRc;
    int iFlag;

    WORD ushObjType;
    char aszLogComment[12];
    char aszObjName[MAX_NWUSERNAME];
    BYTE byLoginTime[7];

    iMyThreadGroupID = GetThreadGroupID();

    pTGroup = GetTGrp(uiConnNum);
    if (pTGroup == NULL) {
        return;
    }

    iRc = SetThreadGroupID(iMain_tgid);
    if (iRc == EFAILURE) {
        return;
    }

    GW_tsh_functrace(FUNC_FNCONNDOWN, GW_FUNC_START, 0);




    switch(uiEvent) {
      case CONNECTION_BEING_RESTARTED:

      case CONNECTION_BEING_FREED:
        break;

      case CONNECTION_BEING_LOGGED_OUT:
      case CONNECTION_BEING_KILLED:

        if (uiEvent == CONNECTION_BEING_LOGGED_OUT) {
            GW_tsh_log(1111, "Logged_Out");
        }
        else {
            GW_tsh_log(1112, "Killed");
        }

        NCP_LogComment(uiConnNum, NULL);

        pTGroup = GetTGrp(uiConnNum);
        if (pTGroup == NULL) {
            goto ConnEnd;
        }

        if (pTGroup->iLoginStatus == 1) {

            iFlag = 0;

            while ((GetTGrp(uiConnNum) != NULL)
                 &&(pTGroup->iLoginStatus == 1)) {
                delay(SHORTWAITTIME);
                if (iFlag == 0) {
                    sprintf(aszLogComment, "%lu", uiConnNum);
                    GW_tsh_log(1113, "Wait");
                    NCP_LogComment(uiConnNum, NULL);
                    iFlag  = 1;
                }
            }

            if (GetTGrp(uiConnNum) == NULL) {
                break;
            }
        }

        if (pTGroup->iLogoutStatus == 0) {
            pTGroup->iLogoutStatus = 1;

            if (bIsConnection_live(pAgent_sp) == FALSE) {
                ResumeThread(pTGroup->iThreadID);
            }
            else {
                iRc = NCP_DCELogoutDown(pTGroup);
            }
        }
        else {
            break;
        }

        GetConnectionInformation(uiConnNum, aszObjName, &ushObjType, &iObjID, byLoginTime);

        if (ushObjType != OT_USER) {




            sprintf(aszLogComment, "%hu", ushObjType);
            GW_tsh_log(1116, aszLogComment);
            NCP_LogComment(uiConnNum, NULL);
            aszObjName[0] = '\0';
        }

        while (GetTGrp(uiConnNum) != NULL) {
            delay(SHORTWAITTIME);
        }

        GW_Message(DFAM_CON, 0, LOG_YES, KDDS11226, aszObjName);
        break;

      default :
        break;
    }

ConnEnd:

    GW_tsh_functrace(FUNC_FNCONNDOWN, GW_FUNC_END, 0);

    SetThreadGroupID(iMyThreadGroupID);
    return;
}


/*EnglishComment**************************************************************/
/*  ID   = NCP_LogComment                                                    */
/*  name = Common routine for logging taken by the Command Listen module     */
/*         and login control.                                                */
/*  func = (1) With using GW_tsh_log(), this routine takes logging of the    */
/*             error-causing user's connection number.                       */
/*         (2) With using GW_tsh_log(), this routine takes logging of the    */
/*             error-causing user's NetWare user name (first 7 bytes).       */
/*                                                                           */
/*  i/o  = uiConNum  : LONG   : i : connection number                        */
/*         pszNWUser : char * : i : NetWare user name                        */
/*                                                                           */
/*  return = none                                                            */
/*                                                                           */
/*  common = (1) Gets a NetWare user name if pszNWUser is NULL.              */
/*           (2) When logging is taken by the Command Listen module and      */
/*               login control, this routine must be called, in order to     */
/*               pinpoint the erroneous client, after GW_tsh_log() is        */
/*               called (to locate the error).                               */
/*           (3) Recommended to call this routine when GW_Message() is       */
/*               invoked.                                                    */
/*           (4) This routine does not take function trace (not necessary).  */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/


void NCP_LogComment(uiConNum, pszNWUser)
LONG uiConNum;
char *pszNWUser;
{
    long uiObjID;
    WORD usObjType;
    char aszLogComment[12];
    char aszObjName[MAX_NWUSERNAME];
    BYTE byLoginTime[7];

    if (pszNWUser != NULL) {
        strcpy(aszObjName, pszNWUser);
    }
    else {
        GetConnectionInformation((WORD)uiConNum, aszObjName, &usObjType,  &uiObjID, byLoginTime);

        if (usObjType != OT_USER) {
            aszObjName[0] = '\0';
        }
    }

    sprintf(aszLogComment, "%lu", uiConNum);
    GW_tsh_log(1999, aszLogComment);

    if (strlen(aszObjName) < 12) {
        strcpy(aszLogComment, aszObjName);
    }
    else {
        strncpy(aszLogComment, aszObjName, 11);
        aszLogComment[11] = '\0';
    }
    GW_tsh_log(1999, aszLogComment);

    return;
}
