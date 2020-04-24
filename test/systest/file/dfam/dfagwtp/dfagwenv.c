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
 * $Log: dfagwenv.c,v $
 * Revision 1.1.2.2  1996/03/11  14:11:18  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:29  marty]
 *
 * Revision 1.1.2.1  1996/02/29  09:58:21  takano
 * 	Submit DFAM-client test
 * 	[1996/02/29  09:55:52  takano]
 * 
 * Revision 1.1.2.1  1996/02/22  10:30:03  takano
 * 	First Submit
 * 	[1996/02/22  10:29:58  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nwbindry.h>
#include <errno.h>
#include <niterror.h>
#include <nwenvrn.h>
#include <nwdir.h>

#include "dfagwtp.h"

extern struct ConfigInfo *pConInfo;
extern char *pszReadBuf;
extern FILE *pVolFp;


/*EnglishComment**************************************************************/
/*  ID   = CheckGWEnv                                                        */
/*                                                                           */
/*  name = Checking the Gateway environment                                  */
/*  func = (1) Checks the registration of the Gateway user.                  */
/*         (2) Checks the registration of the Gateway group.                 */
/*         (3) Checks the registration of the Gateway volume.                */
/*                                                                           */
/*  i/o    = none                                                            */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int CheckGWEnv(void)
{
    int iRc;

    FVTMessage(LOG_NO, KDDS32000);

    iRc = CheckGWUser();
    if (iRc != 0) {
        FVTMessage(LOG_NO, KDDS32002);
        return -1;
    }

    iRc = CheckGWGroup();
    if (iRc != 0) {
        FVTMessage(LOG_NO, KDDS32002);
        return -1;
    }

    iRc = CheckGWVolume();
    if (iRc != 0) {
        FVTMessage(LOG_NO, KDDS32002);
        return -1;
    }

    FVTMessage(LOG_NO, KDDS32001);
    PrintRegisteredInfo();

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = CheckGWUser                                                       */
/*                                                                           */
/*  name = Checking the registration of the Gateway user                     */
/*  func = (1) Checks the registration of the Gateway user.                  */
/*                                                                           */
/*  i/o    = none                                                            */
/*  return =  0: normal end                                                  */
/*           -1: the user is not registered, or an error occurred            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int CheckGWUser(void)
{
    int iRc;
    int iLoop;

    iRc = PutStartLog();
    if (iRc != 0) {
        return -1;
    }

    for (iLoop = 0; iLoop < MAX_NWUSERNUM; iLoop++) {
        if (*(pConInfo->pszNWUser[iLoop]) != '\0') {
            iRc = CheckGWUserProperty(pConInfo->pszNWUser[iLoop]);
            if (iRc != 0) {
                return -1;
            }
        }
    }

    iRc = PutEndLog(LOG_USER, LOG_SUCCESS);
    if (iRc != 0) {
        return -1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = CheckGWUserProperty                                               */
/*                                                                           */
/*  name = Checking the property of a Gateway user                           */
/*  func = (1) Checks if the designated Gateway user has a DCE user          */
/*             property, and DCE password property (does not check their     */
/*             contents).                                                    */
/*                                                                           */
/*  i/o    = pszGWUser : char * :inp: NetWare user name                      */
/*  return =  0: normal end                                                  */
/*           -1: not a Gateway user                                          */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int CheckGWUserProperty(char *pszGWUser)
{
    int iRc;
    int iErrnoSave;
    int iNWErrnoSave;
    BYTE aszSegment[SEGMENTNUM];
    BYTE byFlag;
    BYTE byMore;

    iRc = ReadPropertyValue(pszGWUser, OT_USER, DCE_USER_PROPERTY, 1,
                            aszSegment, &byMore, &byFlag);

    iErrnoSave = errno;
    iNWErrnoSave = NetWareErrno;

    switch (iRc) {
      case ESUCCESS :
        break;

      case ERR_SERVER_OUT_OF_MEMORY :
        PutEndLog(LOG_USER,LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32201);
        return -1;

      case ERR_NO_SUCH_OBJECT :
      case ERR_WILDCARD_NOT_ALLOWED :
        PutEndLog(LOG_USER,LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32005, pszGWUser);
        return -1;

      case  ERR_NO_SUCH_PROPERTY :
      case  ERR_NO_SUCH_SEGMENT  :
        PutEndLog(LOG_USER,LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32006, pszGWUser);
        return -1;

      default :
        PutEndLog(LOG_USER,LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32017, FUNC_READPROPERTY, iRc, iErrnoSave, iNWErrnoSave, pszGWUser, DCE_USER_PROPERTY);

        return -1;
    }

    iRc = ReadPropertyValue(pszGWUser, OT_USER, DCE_PWD_PROPERTY, 1,
                            aszSegment, &byMore, &byFlag);

    iErrnoSave = errno;
    iNWErrnoSave = NetWareErrno;

    switch (iRc) {
      case ESUCCESS :
        break;

      case ERR_SERVER_OUT_OF_MEMORY :
        PutEndLog(LOG_USER,LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32201);
        return -1;

      case ERR_NO_SUCH_OBJECT :
      case ERR_WILDCARD_NOT_ALLOWED :
        PutEndLog(LOG_USER,LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32005, pszGWUser);
        return -1;

      case  ERR_NO_SUCH_PROPERTY :
      case  ERR_NO_SUCH_SEGMENT  :
        PutEndLog(LOG_USER,LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32006, pszGWUser);
        return -1;

      default :
        PutEndLog(LOG_USER,LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32017, FUNC_READPROPERTY, iRc, iErrnoSave, iNWErrnoSave, pszGWUser, DCE_PWD_PROPERTY);

        return -1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = CheckGWGroup                                                      */
/*                                                                           */
/*  name = Checking the registration of the Gateway group                    */
/*  func = (1) Checks the registration of the Gateway group.                 */
/*                                                                           */
/*  i/o    =  none                                                           */
/*  return =  0: normal end                                                  */
/*           -1: the group is not registered, or an error occurred           */
/*                                                                           */
/*  common = none                                                            */
/*  note   = This function checks other_obj, and mask_obj, too.              */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int CheckGWGroup(void)
{
    int iRc;
    int iLoop;

    iRc = PutStartLog();
    if (iRc != 0) {
        return -1;
    }

    for (iLoop = 0; iLoop < MAX_GROUPNUM; iLoop++) {
        if (*(pConInfo->pszNWGroup[iLoop]) != '\0') {
            iRc = CheckGWGroupProperty(pConInfo->pszNWGroup[iLoop], pConInfo->pszDCEGroup[iLoop]);

            if (iRc != 0) {
                return -1;
            }
        }
    }

    iRc = CheckGWGroupProperty(pConInfo->aszOtherobj, DFA_OTHER);
    if (iRc != 0) {
        return -1;
    }

    iRc = CheckGWGroupProperty(pConInfo->aszMaskobj, DFA_MASK);
    if (iRc != 0) {
        return -1;
    }

    iRc = PutEndLog(LOG_GROUP, LOG_SUCCESS);
    if (iRc != 0) {
        return -1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = CheckGWGroupProperty                                              */
/*                                                                           */
/*  name = Checking the property of a Gateway group                          */
/*  func = (1) Checks if the designated Gateway group has a DCE group        */
/*             property.                                                     */
/*         (2) Checks if the value of the DCE group property is same as      */
/*             the designated DCE group name.                                */
/*                                                                           */
/*  i/o    = pszGWGroup :char * :inp: NetWare group name                     */
/*           pszDCEGroup:char * :inp: DCE group name                         */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: not a Gateway group                                         */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int CheckGWGroupProperty(char *pszGWGroup, char *pszDCEGroup)
{
    char *pszGroup;
    int iRc;
    int iSegmentNum;
    int iErrnoSave;
    int iNWErrnoSave;
    char aszProperty[MAX_DCEGROUPLEN + 1];
    BYTE aszSegment[SEGMENTNUM];
    BYTE byFlag;
    BYTE byMore;

    iSegmentNum = 1;
    pszGroup = aszProperty;

    while (1) {
        memset(aszSegment, '\0', SEGMENTNUM);
        iRc = ReadPropertyValue(pszGWGroup, OT_GROUP, DCE_GROUP_PROPERTY, iSegmentNum,
                                aszSegment, &byMore, &byFlag);

        iErrnoSave = errno;
        iNWErrnoSave = NetWareErrno;

        if (iRc == ESUCCESS) {

            if (byMore == 0) {
                strcpy(pszGroup, aszSegment);
                break;
            }

            strncpy(pszGroup ,aszSegment ,SEGMENTNUM);
            pszGroup += SEGMENTNUM;
            iSegmentNum++;
        }

        else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
            PutEndLog(LOG_GROUP,LOG_FAILED);
            FVTMessage(LOG_YES, KDDS32201);
            return -1;
        }

        else if ((iRc == ERR_NO_SUCH_OBJECT)
               ||(iRc == ERR_WILDCARD_NOT_ALLOWED)) {
            PutEndLog(LOG_GROUP,LOG_FAILED);
            FVTMessage(LOG_YES, KDDS32007, pszGWGroup);
            return -1;
        }

        else if ((iRc == ERR_NO_SUCH_PROPERTY)
               ||(iRc == ERR_NO_SUCH_SEGMENT)) {

            PutEndLog(LOG_GROUP,LOG_FAILED);
            FVTMessage(LOG_YES, KDDS32008, pszGWGroup);
            return -1;
        }

        else {
            PutEndLog(LOG_GROUP,LOG_FAILED);
            FVTMessage(LOG_YES, KDDS32017, FUNC_READPROPERTY, iRc, iErrnoSave,
                       iNWErrnoSave, pszGWGroup, DCE_GROUP_PROPERTY);
            return -1;
        }
    }

    if (strcmp(aszProperty, pszDCEGroup) != 0) {
        PutEndLog(LOG_GROUP,LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32009, pszGWGroup, pszDCEGroup);
        return -1;
    }

    return 0;
}


/*EnglishComment***************************************************************/
/*  ID   = CheckGWVolume                                                     */
/*                                                                           */
/*  name = Checking the registration of the Gateway volume                   */
/*  func = (1) Checks if the volume defined in the configuration file is     */
/*             a NetWare volume (i.e., mounted or not).                      */
/*         (2) Checks if the volume defined in the configuration file and    */
/*             the DFS mount point are registered (or, mapped) with the      */
/*             volume file (DFAVOL.CFG).                                     */
/*                                                                           */
/*  i/o    = none                                                            */
/*  return =  0: normal end                                                  */
/*           -1: the volume is not registered, or an error occurred          */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int CheckGWVolume(void)
{
    char *pszBuf;
    char *pszWork;
    int iRc;
    int iErrnoSave;
    int iNWErrnoSave;
    int iResult;
    int tmLen;
    int bFlag;
    int iVolNum;

    iResult = 0;

    iRc = PutStartLog();
    if (iRc != 0) {
        return -1;
    }

    iRc = GetVolumeNumber(pConInfo->aszGWVolume, &iVolNum);
    if (iRc != ESUCCESS) {
        PutEndLog(LOG_VOLUME,LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32012, pConInfo->aszGWVolume);
        return -1;
    }

    pVolFp = fopen(VOLFILE,"r");

    iErrnoSave = errno;
    iNWErrnoSave = NetWareErrno;

    if (pVolFp == NULL) {
        PutEndLog(LOG_VOLUME,LOG_FAILED);
        if (iErrnoSave == ENOENT) {
            FVTMessage(LOG_YES, KDDS32013, pConInfo->aszGWVolume);
        }
        else {
            FVTMessage(LOG_YES, KDDS32200S, VOLFILE, FUNC_FOPEN, LOG_NULL, iErrnoSave, iNWErrnoSave);

        }
        return -1;
    }

    pszBuf = fgets(pszReadBuf, READBUFLEN, pVolFp);

    iErrnoSave = errno;
    iNWErrnoSave = NetWareErrno;

    if (pszBuf == NULL) {

        PutEndLog(LOG_VOLUME,LOG_FAILED);

        if (ferror(pVolFp) != 0) {

            FVTMessage(LOG_YES, KDDS32200S, VOLFILE, FUNC_FGETS, LOG_NULL, iErrnoSave, iNWErrnoSave);

        }
        else {
            FVTMessage(LOG_YES, KDDS32018);
        }

        iResult = -1;
        goto lbCheckGWVolumeEnd;
    }

    pszWork = strrchr(pszReadBuf, '\n');
    if (pszWork != NULL) {
        *pszWork = '\0';
    }
    else {
        PutEndLog(LOG_VOLUME,LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32018);
        iResult = -1;
        goto lbCheckGWVolumeEnd;
    }

    if (strcmp(pszReadBuf, VOLFILEVER) != 0) {
        PutEndLog(LOG_VOLUME,LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32018);
        iResult = -1;
        goto lbCheckGWVolumeEnd;
    }

    while (1) {

        bFlag = 0;
        pszBuf = fgets(pszReadBuf, READBUFLEN, pVolFp);

        iErrnoSave = errno;
        iNWErrnoSave = NetWareErrno;

        if (pszBuf == NULL) {
            if (ferror(pVolFp) != 0) {

                PutEndLog(LOG_VOLUME,LOG_FAILED);

                FVTMessage(LOG_YES, KDDS32200S, VOLFILE, FUNC_FGETS, LOG_NULL, iErrnoSave, iNWErrnoSave);

                iResult = -1;
                goto lbCheckGWVolumeEnd;
            }
            else {
                break;
            }
        }

        pszWork = strrchr(pszReadBuf, '\n');
        if (pszWork != NULL) {
            *pszWork = '\0';
        }
        else {
            PutEndLog(LOG_VOLUME,LOG_FAILED);
            FVTMessage(LOG_YES, KDDS32018);
            iResult = -1;
            goto lbCheckGWVolumeEnd;
        }

        iRc = DeleteServerNameFromVolume(pszReadBuf);
        if (iRc == -1) {
            bFlag = 1;
        }
        else if (iRc == -2) {
            PutEndLog(LOG_VOLUME,LOG_FAILED);
            FVTMessage(LOG_YES, KDDS32018);
            iResult = -1;
            goto lbCheckGWVolumeEnd;
        }

        if (bFlag == 0) {
            tmLen = strlen(pszReadBuf);
            if ((tmLen > _MAX_VOLUME - 1)
              ||(tmLen < 2)) {
                PutEndLog(LOG_VOLUME,LOG_FAILED);
                FVTMessage(LOG_YES, KDDS32018);
                iResult = -1;
                goto lbCheckGWVolumeEnd;
            }
        }

        if (bFlag == 0) {
            if (stricmp(pszReadBuf, pConInfo->aszGWVolume) != 0) {

                bFlag = 1;
            }
        }

        pszBuf = fgets(pszReadBuf, READBUFLEN, pVolFp);

        iErrnoSave = errno;
        iNWErrnoSave = NetWareErrno;

        if (pszBuf == NULL) {
            if (ferror(pVolFp) != 0) {

                PutEndLog(LOG_VOLUME,LOG_FAILED);
                FVTMessage(LOG_YES, KDDS32200S, VOLFILE, FUNC_FGETS, LOG_NULL, iErrnoSave, iNWErrnoSave);

                iResult = -1;
                goto lbCheckGWVolumeEnd;
            }
            else {
                PutEndLog(LOG_VOLUME,LOG_FAILED);
                FVTMessage(LOG_YES, KDDS32018);
                iResult = -1;
                goto lbCheckGWVolumeEnd;
            }
        }

        tmLen = strlen(pszReadBuf);
        if (pszReadBuf[tmLen-1] == '\n') {
            pszReadBuf[tmLen-1] = '\0';
            tmLen--;
        }
        else {
            PutEndLog(LOG_VOLUME,LOG_FAILED);
            FVTMessage(LOG_YES, KDDS32018);
            iResult = -1;
            goto lbCheckGWVolumeEnd;
        }

        if (pszReadBuf[tmLen - 1] == '/') {
            pszReadBuf[tmLen -1 ] = '\0';
            tmLen--;
        }

        if ((tmLen > MAX_TOPDIRLEN)
          ||(tmLen < 1)) {
            PutEndLog(LOG_VOLUME,LOG_FAILED);
            FVTMessage(LOG_YES, KDDS32018);
            iResult = -1;
            goto lbCheckGWVolumeEnd;
        }

        if (bFlag != 0) {
            continue;
        }

        if (iResult > 0) {
            PutEndLog(LOG_VOLUME,LOG_FAILED);
            FVTMessage(LOG_YES, KDDS32018);
            iResult = -1;
            goto lbCheckGWVolumeEnd;
        }

        if (strcmp(pszReadBuf, pConInfo->aszTopdir) == 0) {
            iResult = 1;
        }
        else {
            iResult = 2;
        }
    }

    switch (iResult) {
      case 1 :
        iRc = PutEndLog(LOG_VOLUME, LOG_SUCCESS);
        if (iRc != 0) {
            iResult = -1;
            goto lbCheckGWVolumeEnd;
        }
        iResult = 0;
        break;

      case 2 :
        iRc = PutEndLog(LOG_VOLUME, LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32014, pConInfo->aszGWVolume, pConInfo->aszTopdir);
        iResult = -1;
        break;

      default :
        iRc = PutEndLog(LOG_VOLUME, LOG_FAILED);
        FVTMessage(LOG_YES, KDDS32013, pConInfo->aszGWVolume);
        iResult = -1;
        break;
    }

lbCheckGWVolumeEnd:

    fclose(pVolFp);
    pVolFp = NULL;
    return iResult;
}


/*EnglishComment**************************************************************/
/*  ID   = DeleteServerNameFromVolume                                        */
/*                                                                           */
/*  name = Removing a server name and colon from the volume name             */
/*  func = (1) Removes a server name if the server name is designated in     */
/*             the volume name.                                              */
/*         (2) Removes a colon if the volume name has a colon on the end.    */
/*                                                                           */
/*  i/o    = pszVol : char * :i/o: volume name                               */
/*  return =  0: normal end                                                  */
/*           -1: the server name defined in the volume name is not a         */
/*               current server                                              */
/*           -2: No volume name designated.                                  */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int DeleteServerNameFromVolume(char *pszVol)
{
    FILE_SERV_INFO sbuf;
    char *pszBuf;
    int tmLen;

    tmLen = strlen(pszVol) - 1;
    if (pszVol[tmLen] == ':') {
        pszVol[tmLen] = '\0';
    }

    if (*pszVol == '\0') {
        return -2;
    }

    pszBuf = strchr(pszVol, '\\');
    if (pszBuf == NULL) {
        pszBuf = strchr(pszVol, '/');
        if (pszBuf == NULL) {
            return 0;
        }
    }

    *pszBuf = '\0';
    tmLen = strlen(pszVol);

    GetServerInformation(128, &sbuf);

    if (stricmp(sbuf.serverName, pszVol) != 0) {
        return -1;
    }

    strcpy(pszVol, pszVol + tmLen + 1);

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = PrintRegisteredInfo                                               */
/*                                                                           */
/*  name = Displaying the checked information                                */
/*  func = (1) Displays all the examined information (i.e., user, group,     */
/*             and volume information).                                      */
/*                                                                           */
/*  i/o    = none                                                            */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date =  August 28, 1995  by Hitoshi "Kin-chan" Takahashi                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
void PrintRegisteredInfo(void)
{
    int iLoop;
    int iFlag;

    iFlag = 0;

    for (iLoop = 0; iLoop < MAX_NWUSERNUM; iLoop++) {
        if (*(pConInfo->pszNWUser[iLoop]) != '\0') {
            if (iFlag == 0) {
                iFlag = 1;
                FVTMessage(LOG_NO, K32000, pConInfo->pszNWUser[iLoop]);
            }
            else {
                FVTMessage(LOG_NO, K32001, pConInfo->pszNWUser[iLoop]);
            }
        }
    }

    iFlag = 0;

    for (iLoop = 0; iLoop < MAX_GROUPNUM; iLoop++) {
        if (*(pConInfo->pszNWGroup[iLoop]) != '\0') {
            if (iFlag == 0) {
                iFlag = 1;
                FVTMessage(LOG_NO, K32002, pConInfo->pszNWGroup[iLoop]);
            }
            else {
                FVTMessage(LOG_NO, K32001, pConInfo->pszNWGroup[iLoop]);
            }
        }
    }

    FVTMessage(LOG_NO, K32003, pConInfo->aszOtherobj, pConInfo->aszMaskobj, pConInfo->aszGWVolume);

    return;
}
