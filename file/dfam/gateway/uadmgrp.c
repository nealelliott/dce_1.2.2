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
 * $Log: uadmgrp.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:12  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:19  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:19:45  takano
 * 	First Submit
 * 	[1996/02/21  08:19:41  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <io.h>
#include <conio.h>
#include <advanced.h>
#include <process.h>
#include <ctype.h>
#include <time.h>
#include <nwsnut.h>
#include <string.h>
#include <malloc.h>
#include <nwlocale.h>
#include <nwbindry.h>
#include <errno.h>
#include <niterror.h>
#include <conio.h>

#include "uadmmlh.h"
#include "uadmdef.h"
#include "uadmext.h"

int iLongGroupNum;
char aszGDFAMGroup[MAX_DFAMGROUPNAME];


/*EnglishComment**************************************************************/
/*  ID   = DFAMGroupList                                                     */
/*  name = "Gateway Groups Screen" displaying routine                        */
/*  func =                                                                   */
/*                                                                           */
/*  i/o  = none                                                              */
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

void DFAMGroupList(void)
{
    LIST *pPtr;
    LIST *pPtr1;
    LONG uiCcode;
    long iId;
    WORD ushType;
    int iRc;
    int iRtn;
    int iWidth;
    BYTE aszSegment[SEGMENTNUM];
    char aszDFAMGroup[MAX_DFAMGROUPNAME];
    char cProperty;
    char cFlag;
    char cSecurity;
    BYTE byFlags;
    BYTE byMore;

    NWSInitList(pHandle, free);

    NWSStartWait(WAITGROUPROW, WAITGROUPCOL, pHandle);

    iId = (long)-1;
    iLongGroupNum = 0;

    while (1) {
        iRc = ScanBinderyObject("*", OT_GROUP, &iId, aszDFAMGroup, &ushType, &cProperty, &cFlag,
                                &cSecurity);

        if (iRc == ESUCCESS) {
            iRtn = ReadPropertyValue(aszDFAMGroup, OT_GROUP, DCE_GROUP_PROPERTY, 1, aszSegment,
                                     &byMore, &byFlags);

            if (iRtn == ESUCCESS) {
                pPtr = NWSAppendToList(aszDFAMGroup, (void *)0, pHandle);
                if (pPtr == (LIST *)NULL) {
                    NWSEndWait(pHandle);
                    NWSAlert(0, 0, pHandle, SCREENPREFAILED);
                    goto lbGroupEnd;
                }

                if (strlen(aszDFAMGroup) > MIN_GROUPLISTWIDTH) {
                    iLongGroupNum++;
                }
            }

            else if ((iRtn == ERR_NO_SUCH_PROPERTY)
                   ||(iRtn == ERR_NO_SUCH_SEGMENT)
                   ||(iRtn == ERR_NO_SUCH_OBJECT)) {
                ;
            }

            else if (iRtn == ERR_SERVER_OUT_OF_MEMORY) {
                NWSEndWait(pHandle);
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                goto lbGroupEnd;
            }

            else {
                NWSEndWait(pHandle);
                sprintf(aszError, "%d", iRtn);
                AlertErrorCode(BINDERYERROR, aszError);
                goto lbGroupEnd;
            }
        }

        else if (iRc == ERR_NO_SUCH_OBJECT) {
            break;
        }

        else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
            NWSEndWait(pHandle);
            NWSAlert(0, 0, pHandle, OUTOFMEMORY);
            goto lbGroupEnd;
        }

        else {
            NWSEndWait(pHandle);
            sprintf(aszError, "%d", iRc);
            AlertErrorCode(BINDERYERROR, aszError);
            goto lbGroupEnd;
        }
    }

    pPtr1 = NULL;
    NWSEndWait(pHandle);

    do {

        iWidth = MIN_GROUPLISTWIDTH;
        if (iLongGroupNum > 0) {
            iWidth = MAX_GROUPLISTWIDTH;
        }

        uiCcode = NWSList(DFAMGROUPLIST, USERLISTROW, USERLISTCOL, USERLISTHEIGHT, iWidth,
                          M_ESCAPE | M_SELECT | M_DELETE | M_INSERT, &pPtr1, pHandle, NULL,
                          DFAMGroupAction, NULL);

    } while (uiCcode > 1);

lbGroupEnd:
    NWSDestroyList(pHandle);
    return;

}


/*EnglishComment**************************************************************/
/*  ID   = DFAMGroupAction                                                   */
/*  name = "Gateway Groups Screen" processing routine                        */
/*  func = (1) Goes back to "Menu Screen" if the Esc. key was pressed.       */
/*         (2) Displays "Delete Gateway Group Screen" if the Del. key was    */
/*             was pressed.  Deletes the property of the Gateway group from  */
/*             the bindery if the group is allowed to delete.                */
/*         (3) Displays "Not Gateway Groups Screen" if the Ins. key was      */
/*             pressed.                                                      */
/*         (4) Displays "DCE Group Name Screen" if the Enter key was         */
/*             pressed.                                                      */
/*                                                                           */
/*  i/o  = uikey    : LONG    :: pressed key                                 */
/*         pSelect  : LIST ** :: selected element                            */
/*         pItemNum : LONG    :: item number (not used)                      */
/*         pPar     : void *  :: parameter (not used)                        */
/*                                                                           */
/*  return = -1 : Goes back to "Gateway Groups Screen"                       */
/*            0 : Goes back to "Menu Screen"                                 */
/*            2 : Re-displays "Gateway Groups Screen (the Gateway Group      */
/*                List has been changed).                                    */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DFAMGroupAction(uiKey, pSelect, pItemNum, pPar)
LONG uiKey;
LIST **pSelect;
LONG *pItemNum;
void *pPar;
{
    LIST *pPtr;
    LONG uiCcode;
    long iId;
    int iRc;
    int iRtn;
    int iWidth;
    int iErrno;
    WORD ushType;
    BYTE aszSegment[SEGMENTNUM];
    char aszDFAMGroup[MAX_DFAMGROUPNAME];
    char aszDCEGroup[MAX_DCEGROUPNAME];
    char cProperty;
    char cFlag;
    char cSecurity;
    BYTE byMore;
    BYTE byFlags;

    pItemNum = pItemNum;
    pPar = pPar;

    if (uiKey == M_ESCAPE) {
        return 0;
    }

    if (uiKey == M_DELETE) {
        uiCcode = NWSConfirm(DELDFAMGROUP, DELGROUPROW, DELGROUPCOL, DELGROUPHEIGHT, NULL,
                             pHandle, NULL);

        if (uiCcode == (LONG)1) {

            iRc = DeleteProperty((*pSelect)->text, OT_GROUP, DCE_GROUP_PROPERTY);
            if (iRc == ESUCCESS) {
                ;
            }

            else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                return -1;
            }

            else {
                sprintf(aszError, "%d", iRc);
                AlertErrorCode(BINDERYERROR, aszError);
                return -1;
            }

            if (strlen((*pSelect)->text) > MIN_GROUPLISTWIDTH) {
                iLongGroupNum--;
            }

            pPtr = NWSDeleteFromList(*pSelect, pHandle);
            *pSelect = pPtr;
            return 2;
        }
        return -1;
    }

    if (uiKey == M_INSERT) {

        uiCcode = NWSPushList(pHandle);
        if (uiCcode == (LONG)0) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            return -1;
        }

        NWSInitList(pHandle, free);

        NWSStartWait(WAITNOTGROUPROW, WAITNOTGROUPCOL, pHandle);
        iId = (long)-1;
        iWidth = MIN_NOTGROUPLISTWIDTH;

        while (1) {
            iRc = ScanBinderyObject("*", OT_GROUP, &iId, aszDFAMGroup, &ushType, &cProperty,
                                    &cFlag, &cSecurity);

            if (iRc == ESUCCESS) {
                iRtn = ReadPropertyValue(aszDFAMGroup, OT_GROUP, DCE_GROUP_PROPERTY, 1,
                                         aszSegment, &byMore, &byFlags);

                if (iRtn == ERR_NO_SUCH_PROPERTY) {
                    pPtr = NWSAppendToList(aszDFAMGroup, (void *)0, pHandle);
                    if (pPtr == (LIST *)NULL) {
                        NWSEndWait(pHandle);
                        NWSAlert(0, 0, pHandle, SCREENPREFAILED);
                        NWSDestroyList(pHandle);
                        NWSPopList(pHandle);
                        return -1;
                    }

                    if (strlen(aszDFAMGroup) > MIN_NOTGROUPLISTWIDTH) {
                        iWidth = MAX_NOTGROUPLISTWIDTH;
                    }
                }

                else if ((iRtn == ESUCCESS)
                      ||(iRtn == ERR_NO_SUCH_SEGMENT)
                      ||(iRtn == ERR_NO_SUCH_OBJECT)) {
                    ;
                }

                else if (iRtn == ERR_SERVER_OUT_OF_MEMORY) {
                    NWSEndWait(pHandle);
                    NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                    NWSDestroyList(pHandle);
                    NWSPopList(pHandle);
                    return -1;
                }

                else {
                    NWSEndWait(pHandle);
                    sprintf(aszError, "%d", iRtn);
                    AlertErrorCode(BINDERYERROR, aszError);
                    NWSDestroyList(pHandle);
                    NWSPopList(pHandle);
                    return -1;
                }
            }

            else if (iRc == ERR_NO_SUCH_OBJECT) {
                break;
            }

            else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
                NWSEndWait(pHandle);
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                NWSDestroyList(pHandle);
                NWSPopList(pHandle);
                return -1;
            }

            else {
                NWSEndWait(pHandle);
                sprintf(aszError, "%d", iRc);
                AlertErrorCode(BINDERYERROR, aszError);
                NWSDestroyList(pHandle);
                NWSPopList(pHandle);
                return -1;
            }
        }

        NWSEndWait(pHandle);

        uiCcode = NWSList(NOTGROUPLIST, NOTGROUPLISTROW, NOTGROUPLISTCOL, NOTGROUPLISTHEIGHT,
                          iWidth, M_ESCAPE | M_SELECT, NULL, pHandle, NULL,
                          NotDFAMGroupAction, NULL);

        NWSDestroyList(pHandle);
        NWSPopList(pHandle);

        if (uiCcode == (LONG)2) {
            pPtr = NWSAppendToList(aszGDFAMGroup, (void *)0, pHandle);
            if (pPtr == (LIST *)NULL) {
                NWSAlert(0, 0, pHandle, SCREENPREFAILED);
                return -1;
            }
            if (strlen(aszGDFAMGroup) > MIN_GROUPLISTWIDTH) {
                iLongGroupNum++;
            }

            *pSelect = pPtr;
            return 2;
        }
        return -1;
    }

    else {

        iRc = GetDCEGroupName((*pSelect)->text, aszDCEGroup, &iErrno);
        switch (iRc) {
          case  0 :
            break;

          case -3 :
            NWSAlert(0, 0, pHandle, OUTOFMEMORY);
            return -1;
            break;

          case -4 :
            sprintf(aszError, "%d", iErrno);
            AlertErrorCode(BINDERYERROR, aszError);
            return -1;
            break;

          default :
            sprintf(aszError, "%d", iRc);
            AlertErrorCode(ADMINERROR, aszError);
            return -1;
            break;
        }

        strcpy(aszGDFAMGroup, (*pSelect)->text);

        iRc = NWSEditString(DCEGROUPROW, DCEGROUPCOL, DCEGROUPHEIGHT, DCEGROUPWIDTH,
                            DCEGROUPNAME, NOTHING, aszDCEGroup, MAX_DCEGROUPNAME - 1,
                            EF_SET, pHandle, NULL, DCEGroupNameAction, CHAR_OF_STRING);

        return -1;
    }
}


/*EnglishComment**************************************************************/
/*  ID   = NotDFAMGroupAction                                                */
/*  name = "Not Gateway Groups Screen" processing routine                    */
/*  func = (1) Goes back to "Gateway Groups Screen" if the Esc. key was      */
/*             pressed.                                                      */
/*         (2) Displays "DCE Group Name Screen" if the Enter key was         */
/*             pressed.                                                      */
/*                                                                           */
/*  i/o  = uikey    : LONG    :: pressed key                                 */
/*         pSelect  : LIST ** :: selected element                            */
/*         pItemNum : LONG    :: item number (not used)                      */
/*         pPar     : void *  :: parameter (not used)                        */
/*                                                                           */
/*  return = -1 : Goes back to "Not Gateway Groups Screen"                   */
/*            0 : Goes back to "Gateway Groups Screen"                       */
/*            2 : Gateway Group has been registered (the Gateway Group Name  */
/*                is added to "Gateway Groups Screen").                      */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int NotDFAMGroupAction(uiKey, pSelect, puiItemNum, pPar)
LONG uiKey;
LIST **pSelect;
LONG *puiItemNum;
void *pPar;
{
    int iRc;
    char aszDCEGroup[MAX_DCEGROUPNAME];

    puiItemNum = puiItemNum;
    pPar = pPar;

    if (uiKey == M_ESCAPE) {
        return 0;
    }

    strcpy(aszGDFAMGroup,(*pSelect)->text);
    aszDCEGroup[0] = '\0';

    iRc = NWSEditString(DCEGROUPROW, DCEGROUPCOL, DCEGROUPHEIGHT, DCEGROUPWIDTH, DCEGROUPNAME,
                        NOTHING, aszDCEGroup, MAX_DCEGROUPNAME - 1, EF_SET, pHandle,
                        NULL, DCEGroupNameAction, CHAR_OF_STRING);

    if (iRc == 2) {
        return 2;
    }
    return -1;
}


/*EnglishComment**************************************************************/
/*  ID   = DCEGroupNameAction                                                */
/*  name = "DCE Group Name Screen" processing routine                        */
/*  func = (1) Goes back to "Gateway Groups Screen" or "Not Gateway Groups   */
/*             Screen" if the Escape key was pressed.                        */
/*         (2) Registeres the DCE Group name with the Gateway Group Property */
/*             if the Enter was pressed.                                     */
/*                                                                           */
/*  i/o  = uiAction : LONG   :: inputed key                                  */
/*         pszBuf   : BYTE * :: inputed data (DCE group name)                */
/*         pPar     : void * :: other data (not used)                        */
/*                                                                           */
/*  return =  2 : "Gateway Groups Screen" should be modified due to the      */
/*                registration of the Gateway group name.                    */
/*            0 : Goes back to the previous screen ("Gateway Groups Screen"  */
/*                or "Not Gateway Groups Screen").                           */
/*           -1 : Goes back to "DCE Group Name Screen."                      */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DCEGroupNameAction(uiAction, pszBuf, pPar)
LONG uiAction;
BYTE *pszBuf;
void *pPar;
{
    char *pszDCEGroup;
    int iRc;
    int iRtn;
    int iSegmentNum;
    int iCnt;
    BYTE aszSegment[SEGMENTNUM];
    BYTE byMore;

    pPar = pPar;

    if ((uiAction & E_ESCAPE) != (LONG)0) {
        return 0;
    }

    if (pszBuf[0] == '\0') {
        NWSAlert(0, 0, pHandle, DCEGROUPNAMEFAILURE);
        return -1;
    }


    iRc = CreateProperty(aszGDFAMGroup, OT_GROUP, DCE_GROUP_PROPERTY, BF_STATIC | BF_ITEM,
                         BS_SUPER_WRITE | BS_LOGGED_READ);

    if ((iRc == ESUCCESS)
      ||(iRc == ERR_PROPERTY_ALREADY_EXISTS)) {

        iSegmentNum = 1;
        byMore = 0xFF;
        pszDCEGroup = pszBuf;

        while (1) {

            memset(aszSegment, '\0', SEGMENTNUM);

            for (iCnt = 0; iCnt <= SEGMENTNUM - 1; iCnt++) {
                aszSegment[iCnt] = *pszDCEGroup;
                if (*pszDCEGroup == '\0') {
                    break;
                }
                pszDCEGroup++;
            }

            if (aszSegment[SEGMENTNUM - 1] == '\0') {
                byMore = 0x00;
            }

            iRtn = WritePropertyValue(aszGDFAMGroup, OT_GROUP, DCE_GROUP_PROPERTY, iSegmentNum,
                                      aszSegment, byMore);

            if (iRtn == ESUCCESS) {

                iSegmentNum++;
                if (byMore == 0x00) {
                    break;
                }
            }

            else if (iRtn == ERR_SERVER_OUT_OF_MEMORY) {
                NWSAlert(0, 0, pHandle, OUTOFMEMORY);
                return -1;
            }

            else {
                sprintf(aszError, "%d", iRtn);
                AlertErrorCode(BINDERYERROR, aszError);
                return -1;
            }
        }
    }

    else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
        NWSAlert(0, 0, pHandle, OUTOFMEMORY);
        return -1;
    }

    else {
        sprintf(aszError, "%d", iRc);
        AlertErrorCode(BINDERYERROR, aszError);
        return -1;
    }

    return 2;
}


/*EnglishComment**************************************************************/
/*  ID   = GetDCEGroupName                                                   */
/*  name = Getting the DCE Group Name corresponding with the designated      */
/*         Gateway Group Name.                                               */
/*  func = (1) Checks if the DCE Group Property exists in the designated     */
/*             Gateway Group Property.                                       */
/*         (2) Reads the Property Value from the DCE User Property if the    */
/*             DCE Group Property exists.                                    */
/*                                                                           */
/*  i/o  = pszDFAMGroupName : char * : i : Gateway Group Name                */
/*         pszDCEGroupName  : char * : o : DCE Group Name                    */
/*         iErrno           : int *  : o : detailed error code               */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -2: Not a Gateway Group                                         */
/*           -3: insufficient memory                                         */
/*           -4: bindery access error                                        */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/
int GetDCEGroupName(pszDFAMGroupName, pszDCEGroupName, iErrno)
char *pszDFAMGroupName;
char *pszDCEGroupName;
int *iErrno;
{
    BYTE *pszDCEGroup;
    int iRc;
    int iSegmentNum;
    BYTE aszSegment[SEGMENTNUM];
    BYTE byFlag;
    BYTE byMore;

    iSegmentNum = 1;
    pszDCEGroup = pszDCEGroupName;

    *iErrno = 0;

    while (1) {
        memset(aszSegment, '\0', SEGMENTNUM);
        iRc = ReadPropertyValue(pszDFAMGroupName, OT_GROUP, DCE_GROUP_PROPERTY, iSegmentNum,
                                aszSegment, &byMore, &byFlag);

        if (iRc == ESUCCESS) {

            if (byMore == 0) {
                strcpy(pszDCEGroup, aszSegment);
                break;
            }

            strncpy(pszDCEGroup ,aszSegment ,SEGMENTNUM);
            pszDCEGroup += SEGMENTNUM;
            iSegmentNum++;

        }

        else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
            *pszDCEGroupName = '\0';
            *iErrno = iRc;
            return -3;
        }

        else if(iRc == ERR_NO_SUCH_PROPERTY) {
            *pszDCEGroupName = '\0';
            *iErrno = iRc;
            return -2;
        }

        else {
            *pszDCEGroupName = '\0';
            *iErrno = iRc;
            return -4;
        }
    }
    return 0;
}
