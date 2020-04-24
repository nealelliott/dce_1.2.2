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
 * $Log: uadmvol.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:19  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:24  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:21:48  takano
 * 	First Submit
 * 	[1996/02/21  08:21:43  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <process.h>
#include <ctype.h>
#include <nwsnut.h>
#include <nwdir.h>
#include <string.h>
#include <errno.h>
#include <niterror.h>

#include "uadmmlh.h"
#include "uadmdef.h"
#include "uadmext.h"

char aszGDFAMVolume[_MAX_VOLUME];


/*EnglishComment**************************************************************/
/*  ID   = DFAMVolumeList                                                    */
/*  name = "Gateway Volumes Screen" processing routine                       */
/*  func = Makes "Gateway Volumes Screen."                                   */
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

void DFAMVolumeList(void)
{
    struct VolumeInfo *pVolumeP;
    LIST *pPtr;
    LIST *pPtr1;
    LONG uiCcode;
    int iRc;
    int iNWSEnd;


    pVInfoTop = (struct VolumeInfo *)NULL;

    NWSInitList(pHandle, free);
    iNWSEnd = 0;

    NWSStartWait(WAITVOLROW, WAITVOLCOL, pHandle);

    iRc = VolumeInit(FROM_VOLUME_INIT);
    switch (iRc) {
      case 1 :
      case 0 :
        break;

      case -1 :
        NWSEndWait(pHandle);
        NWSAlert(0, 0, pHandle, FILEACCESSERROR);
        NWSDestroyList(pHandle);
        return;
        break;

      case -2 :
        NWSEndWait(pHandle);
        NWSAlert(0, 0, pHandle, OUTOFMEMORY);
        NWSDestroyList(pHandle);
        return;
        break;

      case -3 :
        NWSEndWait(pHandle);
        NWSAlert(0, 0, pHandle, VOLUMEFILENOTUSE);
        NWSDestroyList(pHandle);
        return;
        break;

      case -4 :
        NWSEndWait(pHandle);
        iNWSEnd = 1;
        NWSAlert(0, 0, pHandle, VOLUMEFILEINVALID);
        break;

      case -5 :
        NWSEndWait(pHandle);
        NWSAlert(0, 0, pHandle, FILEACCESSERROR);
        NWSDestroyList(pHandle);
        return;
        break;

      case -6 :
        NWSEndWait(pHandle);
        iNWSEnd = 1;
        NWSAlert(0, 0, pHandle, VOLUMENOTFOUND);
        break;

      default :
        NWSEndWait(pHandle);
        sprintf(aszError, "%d", iRc);
        AlertErrorCode(ADMINERROR, aszError);
        NWSDestroyList(pHandle);
        return;
        break;
    }

    pVolumeP = pVInfoTop;
    while (pVolumeP != (struct VolumeInfo *)NULL) {

        pPtr = NWSAppendToList(pVolumeP->aszDFAMVolume, (void *)0, pHandle);
        if (pPtr == (LIST *)NULL) {
            NWSAlert(0, 0, pHandle, SCREENPREFAILED);
            goto lbVolEnd;
        }
        pVolumeP = pVolumeP->pNext;
    }

    if (iNWSEnd == 0) {
        NWSEndWait(pHandle);
    }

    pPtr1 = NULL;

    do {
        uiCcode = NWSList(DFAMVOLUMELIST, VOLLISTROW, VOLLISTCOL, VOLLISTHEIGHT, VOLLISTWIDTH,
                          M_ESCAPE | M_SELECT | M_DELETE | M_INSERT, &pPtr1, pHandle, NULL,
                          DFAMVolumeAction, NULL);

    } while (uiCcode > 1);

    NWSStartWait(WAITVOLROW, WAITVOLCOL, pHandle);
    iRc = writeVolumeFile();
    NWSEndWait(pHandle);

    if (iRc != 0) {
        NWSAlert(0, 0, pHandle, FILEACCESSERROR);
    }

lbVolEnd:
    delAllVolume();
    NWSDestroyList(pHandle);
    return;
}


/*EnglishComment**************************************************************/
/*  ID   = DFAMVolumeAction                                                  */
/*  name = "Gateway Volumes Screen" processing routine                       */
/*  func = (1) Goes back to "Menu Screen" if the Esc. key was pressed.       */
/*         (2) Displays "Delete Gateway Volume Screen" if the Del key was    */
/*             pressed.  If OK to delete, the Gateway volume is deleted from */
/*             the table and list.  If not OK to delete, goes back to        */
/*             "Gateway Volumes Screen."                                     */
/*         (3) Displays "Not Gateway Volumes Screen" if the Ins. key is      */
/*             pressed.                                                      */
/*                                                                           */
/*  i/o  = uiKey      : LONG    :: pressed key                               */
/*         pSelect    : LIST ** :: selected element                          */
/*         puiItemNum : LONG    :: item number (not used)                    */
/*         pPar       : void *  :: parameter (not used)                      */
/*                                                                           */
/*  return = -1 : Goes back to "Gateway Volume Screen."                      */
/*            0 : Goes back to "Menu Screen."                                */
/*            2 : Re-displays "Gateway Volumes Screen." (Gateway Volume      */
/*                List has been changed)                                     */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DFAMVolumeAction(uiKey, pSelect, pItemNum, pPar)
LONG uiKey;
LIST **pSelect;
LONG *pItemNum;
void *pPar;
{
    struct VolumeInfo *pVolumeP;
    LIST *pPtr;
    LONG uiCcode;
    int iRc;
    int iVolume;
    char aszVolName[_MAX_VOLUME];
    char aszMPoint[MAX_DFSMOUNTPOINT];

    pItemNum = pItemNum;
    pPar = pPar;

    if (uiKey == M_ESCAPE) {
        return 0;
    }

    if (uiKey == M_DELETE) {
        uiCcode = NWSConfirm(DELDFAMVOLUME, DELVOLUMEROW, DELVOLUMECOL, DELVOLUMEHEIGHT, NULL,
                             pHandle, NULL);

        if (uiCcode == (LONG)1) {
            iRc = delVolume((*pSelect)->text);
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

        NWSStartWait(WAITNOTVOLROW, WAITNOTVOLCOL, pHandle);

        for (iVolume = 0; iVolume < 64; iVolume++) {
            iRc = GetVolumeName(iVolume, aszVolName);
            if (iRc != 0) {

                NWSEndWait(pHandle);

                sprintf(aszError, "%d", iRc);
                AlertErrorCode(ADMINERROR, aszError);
                NWSDestroyList(pHandle);
                NWSPopList(pHandle);
                return -1;
            }

            if (*aszVolName == '\0') {
                continue;
            }

            if (strcmp(aszVolName,"SYS") == 0) {
                continue;
            }

            if (getVolume(aszVolName) != NULL) {
                continue;
            }

            pPtr = NWSAppendToList(aszVolName, (void *)0, pHandle);
            if (pPtr == (LIST *)NULL) {

                NWSEndWait(pHandle);

                NWSAlert(0, 0, pHandle, SCREENPREFAILED);
                NWSDestroyList(pHandle);
                NWSPopList(pHandle);
                return -1;
            }
        }

        NWSEndWait(pHandle);

        uiCcode = NWSList(NOTDFAMVOLUMELIST, NOTVOLUMELISTROW, NOTVOLUMELISTCOL,
                          NOTVOLUMELISTHEIGHT, NOTVOLUMELISTWIDTH, M_ESCAPE | M_SELECT, NULL,
                          pHandle, NULL, NOTDFAMVolumeAction, NULL);

        NWSDestroyList(pHandle);
        NWSPopList(pHandle);

        if (uiCcode == (LONG)2) {
            pPtr = NWSAppendToList(aszGDFAMVolume, (void *)0, pHandle);
            if (pPtr == (LIST *)NULL) {
                NWSAlert(0, 0, pHandle, SCREENPREFAILED);
                return -1;
            }
            *pSelect = pPtr;
            return 2;
        }
        return -1;
    }

    else {
        pVolumeP = getVolume((*pSelect)->text);
        if (pVolumeP == (struct VolumeInfo *)NULL) {
            AlertErrorCode(ADMINERROR, "0");
            return -1;
        }

        strcpy(aszGDFAMVolume, (*pSelect)->text);
        strcpy(aszMPoint, pVolumeP->aszDFSMountPoint);

        iRc = NWSEditString(MOUNTPOINTROW, MOUNTPOINTCOL, MOUNTPOINTHEIGHT, MOUNTPOINTWIDTH,
                            MOUNTPOINTNAME, NOTHING, aszMPoint, MAX_DFSMOUNTPOINT - 1,
                            EF_SET, pHandle, NULL, DFSMountPointAction, CHAR_OF_STRING);

        return -1;
    }
}


/*EnglishComment**************************************************************/
/*  ID   = NOTDFAMVolumeAction                                               */
/*  name = "Not Gateway Volumes Screen" processing routine                   */
/*  func = (1) Goes back to "Gateway Volumes Screen" if the Escape key was   */
/*             pressed.                                                      */
/*         (2) Displays "DFS Mount Point Screen" if the Enter key was        */
/*             pressed.                                                      */
/*                                                                           */
/*  i/o  = uiKey      : LONG    :: pressed key                               */
/*         pSelect    : LIST ** :: selected element                          */
/*         puiItemNum : LONG    :: item number (not used)                    */
/*         pPar       : void *  :: parameter (not used)                      */
/*                                                                           */
/*  return = -1 : Goes back to "Not Gateway Volumes Screen."                 */
/*            0 : Goes back to "Gateway Volumes Screen."                     */
/*            2 : Goes back to "Gateway Volumes Screen" (re-displays the     */
/*                list)                                                      */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int NOTDFAMVolumeAction(uiKey, pSelect, puiItemNum, pPar)
LONG uiKey;
LIST **pSelect;
LONG *puiItemNum;
void *pPar;
{
    int iRc;
    char aszMPoint[MAX_DFSMOUNTPOINT];

    puiItemNum = puiItemNum;
    pPar = pPar;

    if (uiKey == M_ESCAPE) {
        return 0;
    }





    strcpy(aszGDFAMVolume, (*pSelect)->text);

    aszMPoint[0] = '\0';

    iRc = NWSEditString(MOUNTPOINTROW, MOUNTPOINTCOL, MOUNTPOINTHEIGHT, MOUNTPOINTWIDTH,
                        MOUNTPOINTNAME, NOTHING, aszMPoint, MAX_DFSMOUNTPOINT - 1,
                        EF_SET, pHandle, NULL, DFSMountPointAction, CHAR_OF_STRING);

    if (iRc == 2) {
        return 2;
    }
    return -1;
}


/*EnglishComment**************************************************************/
/*  ID   = DFSMountPointAction                                               */
/*  name = "DFS Mount Point Screen" processing routine                       */
/*  func = (1) Goes back to "Not Gateway Volumes Screen" if the Escape key   */
/*             was pressed.                                                  */
/*         (2) Registers the NetWare volume name and DFS mount point with    */
/*             the Volume Table if the Enter key was pressed.                */
/*  i/o  = uiAction : LONG   :: inputed key                                  */
/*         pszBuf   : BYTE * :: entered data (DFS mount point)               */
/*         pPar     : void * :: other data (nor used)                        */
/*                                                                           */
/*  return = -2 : Goes back to "Not Gateway Volumes Screen."                 */
/*           -1 : Goes back to "DFS Mount Point Screen."                     */
/*            2 : Gateway volume has been registered (goes back to "Gateway  */
/*                Volume Screen" to to modify the Screen)                    */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DFSMountPointAction(uiAction, pszBuf, pPar)
LONG uiAction;
BYTE *pszBuf;
void *pPar;
{
    struct VolumeInfo *pVolumeP;
    int iRc;

    pPar = pPar;

    if ((uiAction & E_ESCAPE) != (LONG)0) {
        return -2;
    }

    iRc = DFSPathCheck(pszBuf);
    if (iRc == -1) {
        NWSAlert(0, 0, pHandle, DFSMOUNTPOINTERR);
        return -1;
    }

    pVolumeP = getVolume(aszGDFAMVolume);
    if (pVolumeP != (struct VolumeInfo *)NULL) {
        if ((uiAction & E_CHANGE) != (LONG)0) {
           strcpy(pVolumeP->aszDFSMountPoint, pszBuf);
        }
    }
    else {
        iRc = addVolume(aszGDFAMVolume, pszBuf);
        if (iRc != 0) {
            NWSAlert(0, 0, pHandle, OUTOFMEMORY);
            return -1;
        }
    }
    return 2;
}


/*EnglishComment**************************************************************/
/*  ID   = DFSPathCheck                                                      */
/*  name = DFS path name syntax check routine                                */
/*  func = (1) Checks the syntax of a DFS path name.  The error cases are    */
/*             as follows:                                                   */
/*                i) No DFS path name                                        */
/*               ii) /.../ is missing                                        */
/*              iii) / comes right after /.../                               */
/*               iv) No / after /.../                                        */
/*                v) No fs after /                                           */
/*                                                                           */
/*  i/o  = pszDFSPath : char  * : i : DFS path name                          */
/*                                                                           */
/*  return = -1 : Invalid DFS path name                                      */
/*            0 : normal end                                                 */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DFSPathCheck(pszDFSPath)
char *pszDFSPath;
{
    char *pszDFSp;
    char *pszDFSwp;

    if (*pszDFSPath == '\0') {
        return -1;
    }

    if (strlen(pszDFSPath) > MAX_DFSMOUNTPOINT - 1) {
        return -1;
    }

    if (strncmp(pszDFSPath, "/.../", 5) != 0) {
        return -1;
    }

    pszDFSp = pszDFSPath + 5;

    pszDFSwp = strchr(pszDFSp, '/');
    if (pszDFSwp == NULL) {
        return -1;
    }

    if (( (pszDFSwp - pszDFSp) < 1)
      ||( (pszDFSwp - pszDFSp) > 255)) {
        return -1;
    }

    pszDFSp = pszDFSwp + 1;

    if (strncmp(pszDFSp, "fs", 2) != 0) {
        return -1;
    }

    pszDFSp += 2;
    if ((*pszDFSp != '\0')
      &&(*pszDFSp != '/')) {
        return -1;
    }

    pszDFSp++;

    while (1) {

        pszDFSwp = strchr(pszDFSp, '/');
        if (pszDFSwp == NULL) {
            if (strlen(pszDFSp) > 255) {
                return -1;
            }
            break;
        }

        if (( (pszDFSwp - pszDFSp) < 1)
          ||( (pszDFSwp - pszDFSp) > 255)) {
            return -1;
        }

        pszDFSp = pszDFSwp + 1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = VolumeInit                                                        */
/*  name = Volume File initialization routine                                */
/*  func = (1) Gets Volume Information from Volume File.                     */
/*         (2) Checks the version of Volume File.                            */
/*         (3) Stores the information of Volume File to Volume Table.        */
/*                                                                           */
/*  i/o  = iMode : int : i : mode  FORM_VOLUEME_INIT (called from            */
/*                           uadmvol.c)                                      */
/*                                                                           */
/*  return = -6 : no volume                                                  */
/*           -5 : failed to access Option File                               */
/*           -4 : invalid data in Volume File (the data are ignored)         */
/*           -3 : invalid version of Volume File                             */
/*           -2 : failed to allocate memory                                  */
/*           -1 : failed to access Volume File                               */
/*            0 : Volume File successfully initialized                       */
/*            1 : no Volume File (new registration)                          */
/*                                                                           */
/*  common = -6 surpasses -4 in the return value.                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int VolumeInit(iMode)
int iMode;
{
    char *pszBufp;
    int iRc;
    int iVolNum;
    int bFlag;

    int bSkipFlag;

    int bVFlag;
    char aszVolBuf[_MAX_VOLUME + _MAX_SERVER + 2];
    char aszMPointBuf[MAX_DFSMOUNTPOINT + 1];


    if (iMode == FROM_VOLUME_INIT) {



    }

    pVolFp = fopen(VOLFILE,"r");
    if (pVolFp == NULL) {
        if (errno == ENOENT) {
            return 1;
        }
        return -1;
    }

    bSkipFlag = 0;

    iRc = CFGVerCheck(pVolFp, VOLFILEVER);
    if (iRc == 0) {
        ;
    }
    else if (iRc == -1) {
        fclose(pVolFp);
        pVolFp = NULL;
        return -3;
    }
    else {
        bSkipFlag = 1;
    }

    bVFlag = 0;

    while (1) {
        bFlag = 0;
        memset(aszVolBuf, '\0', sizeof(aszVolBuf));

        if (fgets(aszVolBuf, sizeof(aszVolBuf), pVolFp) == NULL) {

            if (ferror(pVolFp) != 0) {
                fclose(pVolFp);
                pVolFp = NULL;
                delAllVolume();
                return -1;
             }
             break;
        }

        pszBufp = strrchr(aszVolBuf,'\n');
        if (pszBufp != NULL) {
            *pszBufp = '\0';
        }
        else {
			if (aszVolBuf[0] == 0x1a)
				break;

            iRc = SkipReadCFGFile(pVolFp);

            switch (iRc) {
              case  0 :
                break;

              case -2 :
                bSkipFlag = 1;
                goto lbVolInitEnd;
                break;

              default :
                fclose(pVolFp);
                pVolFp = NULL;
                delAllVolume();
                return -1;
                break;
            }
        }

        if (bFlag == 0) {
            iRc = DeleteServerNameFromBackupDir(aszVolBuf);
            if (iRc != 0) {
                bFlag = 1;
            }
        }

        if (bFlag == 0) {
            iRc = DFAMVolumeNameCheck(aszVolBuf);
            if (iRc != 0) {
                bFlag = 1;
            }
        }


        memset(aszMPointBuf, '\0', sizeof(aszVolBuf));

        if (fgets(aszMPointBuf, sizeof(aszMPointBuf), pVolFp) == NULL) {

            if (ferror(pVolFp) != 0) {
                fclose(pVolFp);
                pVolFp = NULL;
                delAllVolume();
                return -1;
             }
             bSkipFlag = 1;
             goto lbVolInitEnd;
        }

        pszBufp = strrchr(aszMPointBuf,'\n');
        if (pszBufp != NULL) {
            *pszBufp = '\0';
        }
        else {

            if (strlen(aszMPointBuf) > MAX_DFSMOUNTPOINT - 1) {

                iRc = SkipReadCFGFile(pVolFp);

                switch (iRc) {
                  case  0 :
                    break;

                  case -2 :
                    bSkipFlag = 1;
                    goto lbVolInitEnd;
                    break;

                  default :
                    fclose(pVolFp);
                    pVolFp = NULL;
                    delAllVolume();
                    return -1;
                    break;
                }
                bFlag = 1;
            }
        }

        if (bFlag == 0) {
            iRc = DFSPathCheck(aszMPointBuf);
            if (iRc < 0) {
                bFlag = 1;
            }
        }



        if (bFlag == 0) {
            iRc = GetVolumeNumber(aszVolBuf, &iVolNum);
            if (iRc != ESUCCESS) {
                bFlag = 1;
                bVFlag = 1;
            }
        }

        if (bFlag != 0) {
            bSkipFlag = 1;
            continue;
        }

        iRc = addVolume(aszVolBuf, aszMPointBuf);
        if (iRc != 0) {
             fclose(pVolFp);
             pVolFp = NULL;
             return -2;
        }
    }

lbVolInitEnd:

    fclose(pVolFp);
    pVolFp = NULL;

    if (bSkipFlag != 0) {
        if (bVFlag != 0) {
            return -6;
        }
        return -4;
    }
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = addvolume                                                         */
/*  name = Volume Table processing routine                                   */
/*  func = (1) Searches the Volume Table for the designated Gateway Volume   */
/*             name, and gets the Volume Table.                              */
/*                                                                           */
/*  i/o  = pszVol    : char * : i : Gateway Volume name                      */
/*         pszMPoint : char * : i : DFS mount point                          */
/*                                                                           */
/*  return = -1 : failed to allocate memory for Volume Information           */
/*            0 : Volume Information was successfully registered             */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int addVolume(pszVol, pszMPoint)
char *pszVol;
char *pszMPoint;
{
    struct VolumeInfo *pVolumeP;
    struct VolumeInfo *pVolumeWp;

    if (pVInfoTop == (struct VolumeInfo *)NULL) {

        pVInfoTop = (struct VolumeInfo *)NWSAlloc(sizeof(struct VolumeInfo), pHandle);
        if (pVInfoTop == (struct VolumeInfo *)NULL) {
            return -1;
        }

        strcpy(pVInfoTop->aszDFAMVolume, pszVol);
        strcpy(pVInfoTop->aszDFSMountPoint, pszMPoint);
        pVInfoTop->pNext = (struct VolumeInfo *)NULL;
    }

    else {
        pVolumeP = pVInfoTop;

        while(pVolumeP->pNext != NULL) {
            pVolumeP = pVolumeP->pNext;
        }

        pVolumeP->pNext = (struct VolumeInfo *)NWSAlloc(sizeof(struct VolumeInfo), pHandle);
        if (pVolumeP->pNext == (struct VolumeInfo *)NULL) {
            return -1;
        }

        pVolumeWp = pVolumeP->pNext;
        strcpy(pVolumeWp->aszDFAMVolume, pszVol);
        strcpy(pVolumeWp->aszDFSMountPoint, pszMPoint);
        pVolumeWp->pNext = (struct VolumeInfo *)NULL;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = delVolume                                                         */
/*  name = Volume Table deletion routine                                     */
/*  func = (1) Searches the Volume Table for the designated Gateway Volume   */
/*             name, and frees the Volume Table.                             */
/*                                                                           */
/*  i/o  = pszDFAM : char * : i : Gateway Volume name to be deleted          */
/*                                                                           */
/*  return = -1 : the designated Gateway Volume does not exist in the        */
/*                Volume Table.                                              */
/*            0 : the Volume Information was successfully deleted.           */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int delVolume(pszDFAM)
char *pszDFAM;
{
    struct VolumeInfo *pVolumeP;
    struct VolumeInfo *pVolumePre;
    int iExist;

    pVolumeP = pVInfoTop;
    pVolumePre  = pVInfoTop;
    iExist = 0;

    do {
        if (strcmp(pszDFAM, pVolumeP->aszDFAMVolume) == 0) {
            iExist = 1;
            break;
        }
        pVolumePre = pVolumeP;
        pVolumeP = pVolumeP->pNext;
    } while (pVolumeP != (struct VolumeInfo *)NULL);

    if (iExist == 0) {
        return -1;
    }

    if (pVolumeP == pVInfoTop) {
        pVInfoTop = pVolumeP->pNext;
    }
    else {
        pVolumePre->pNext = pVolumeP->pNext;
    }

    NWSFree((struct VolumeInfo *)pVolumeP,pHandle);
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = getVolume                                                         */
/*  name = Gateway Volume searching routine                                  */
/*  func = (1) Searches for the designated Gateway Volume.                   */
/*                                                                           */
/*  i/o  = pszDFAM : char * : i : Gateway Volume to be searched              */
/*                                                                           */
/*  return = NULL : The table registering the designated Gateway Volume      */
/*                  does not exist.                                          */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

struct VolumeInfo *getVolume(pszDFAM)
char *pszDFAM;
{
    struct VolumeInfo *pVolumeP;

    if (pVInfoTop == (struct VolumeInfo *)NULL) {
        return pVInfoTop;
    }
    else {
        pVolumeP = pVInfoTop;
        do {

            if (strcmp(pVolumeP->aszDFAMVolume, pszDFAM) == 0) {
                break;
            }
            pVolumeP = pVolumeP->pNext;
        } while(pVolumeP != (struct VolumeInfo *)NULL);

        return pVolumeP;
    }
}


/*EnglishComment**************************************************************/
/*  ID   = delAllVolume                                                      */
/*  name = Volume Information deletion routine                               */
/*  func = (1) Frees the momory holding the Volume Information               */
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

void delAllVolume(void)
{
    struct VolumeInfo *pVolumeP;
    struct VolumeInfo *pVolumeNp;

    pVolumeP = pVInfoTop;

    while (pVolumeP != (struct VolumeInfo *)NULL) {

        pVolumeNp = pVolumeP->pNext;
        NWSFree((struct VolumeInfo *)pVolumeP, pHandle);

        pVolumeP = pVolumeNp;
    }
    pVInfoTop = (struct VolumeInfo *)NULL;
    return ;
}


/*EnglishComment**************************************************************/
/*  ID   = writeVolumeFile                                                   */
/*  name = Volume Information storing routine                                */
/*  func = Stores Volume Information to Volume File.                         */
/*         (1) Makes a temporary file.                                       */
/*         (2) Writes version information and Volume Information onto the    */
/*             temporary file.                                               */
/*         (3) Deletes an old Volume File.                                   */
/*         (4) Renames the temporary file to Volume File.                    */
/*                                                                           */
/*  i/o  = none                                                              */
/*  return = -2 : failed to rename DFAVOL.TMP file (No DFAVOL.CFG file)      */
/*           -1 : failed to access the Volume File                           */
/*            0 : normal end                                                 */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int writeVolumeFile(void)
{
    FILE *pVolFp;
    struct VolumeInfo *pVolumeP;
    int iRc;
    char aszBuf[7];

    pVolFp = fopen(TMPVOLFILE,"W");
    if (pVolFp != NULL) {

        strcpy(aszBuf,VOLFILEVER);
        strcat(aszBuf,"\n");
        iRc = fputs(aszBuf,pVolFp);

        if (iRc == EOF) {
            fclose(pVolFp);
            pVolFp = NULL;
            unlink(TMPVOLFILE);
            return -1;
        }

        if (pVInfoTop == NULL) {
            fclose(pVolFp);
            pVolFp = NULL;

            SetDefaultFileAttribute(VOLFILE);
            unlink(VOLFILE);

            iRc = rename(TMPVOLFILE,VOLFILE);
            if (iRc != 0) {
                return -2;
            }
            return 0;
        }

        pVolumeP = pVInfoTop;
        do {
            strcat(pVolumeP->aszDFAMVolume,":");
            strcat(pVolumeP->aszDFAMVolume,"\n");
            strcat(pVolumeP->aszDFSMountPoint,"\n");

            iRc = fputs(pVolumeP->aszDFAMVolume, pVolFp);
            if (iRc == EOF) {
                fclose(pVolFp);
                pVolFp = NULL;
                unlink(TMPVOLFILE);
                return -1;
            }

            iRc = fputs(pVolumeP->aszDFSMountPoint, pVolFp);
            if (iRc == EOF) {
                fclose(pVolFp);
                pVolFp = NULL;
                unlink(TMPVOLFILE);
                return -1;
            }
            pVolumeP = pVolumeP->pNext;

        } while(pVolumeP != (struct VolumeInfo *)NULL);

		iRc = fputc(0x1a,pVolFp);
        if (iRc == EOF) {
            fclose(pVolFp);
            pVolFp = NULL;
            unlink(TMPVOLFILE);
            return -1;
        }
    }

    else {
        return -1;
    }

    fclose(pVolFp);
    pVolFp = NULL;

    SetDefaultFileAttribute(VOLFILE);
    unlink(VOLFILE);

    iRc = rename(TMPVOLFILE,VOLFILE);
    if (iRc != 0) {
        return -2;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = DFAMVolumeNameCheck                                               */
/*  name = Gateway Volume syntax check routine                               */
/*  func = Checks the syntax of Gateway Volume.                              */
/*                                                                           */
/*  i/o  = pszDFAMVolume : char * : i : Volume name                          */
/*                                                                           */
/*  return = -1 : invalid Gateway Volume name                                */
/*            0 : Gateway Volume is syntactically correct                    */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int DFAMVolumeNameCheck(pszDFAMVolume)
char *pszDFAMVolume;
{
    struct VolumeInfo *pVolumeP;
    char *pszBuf;

    pszBuf = pszDFAMVolume;
    while (*pszBuf != '\0') {

        if (isprint(*pszBuf) == 0) {
            return -1;
        }

        if (islower(*pszBuf) != 0) {
            *pszBuf = (char)toupper(*pszBuf);
        }

        pszBuf++;
    }

    pszBuf--;
    if (*pszBuf == ':') {
        *pszBuf = '\0';
    }

    if (strcmp(pszDFAMVolume, "SYS") == 0) {
        return -1;
    }

    pVolumeP = getVolume(pszDFAMVolume);
    if (pVolumeP != (struct VolumeInfo *)NULL) {
        return -1;
    }

    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = GetEscapeDirName                                                  */
/*  name = Getting Backup Directory name routine                             */
/*  func = Gets the Backup Directory name from DFAMPT.CFG file.              */
/*                                                                           */
/*  i/o  = pszEscapeDir : char * : o : Backup Directory name                 */
/*                                                                           */
/*  return = -1 : failed to access Option File                               */
/*            0 : normal end                                                 */
/*                                                                           */
/*  common = does not check the version and syntax of Option File.           */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int GetEscapeDirName(pszEscapeDir)
char *pszEscapeDir;
{
    char *pszBufp;
    int iRc;
    int tmLen;
    char aszBuf[MAX_COPYBACKDIRNAME + 19];

    *pszEscapeDir = '\0';

    pOthrFp = fopen(OPTFILE,"r");
    if (pOthrFp == NULL) {
        if (errno == ENOENT) {
            return 0;
        }
        return -1;
    }

    iRc = CFGVerCheck(pOthrFp, OPTFILEVER);
    if (iRc != 0) {
        goto lbCheckEnd;
    }

    while (fgets(aszBuf, sizeof(aszBuf), pOthrFp) != NULL) {

        pszBufp = strrchr(aszBuf,'\n');
        if (pszBufp != NULL) {
            *pszBufp = '\0';
        }
        else {

            if (strlen(aszBuf) > MAX_COPYBACKDIRNAME + 18) {

                iRc = SkipReadCFGFile(pOthrFp);

                switch (iRc) {
                  case  0 :
                    break;

                  case -2 :
                    goto lbCheckEnd;
                    break;

                  default :
                    fclose(pOthrFp);
                    pOthrFp = NULL;
                    return -1;
                    break;
                }
                continue;
            }
        }

        if (strncmp(aszBuf,"COPYBACKDIRECTORY=",18) == 0) {

            iRc = DeleteServerNameFromBackupDir(aszBuf+18);
            if (iRc != 0) {
                goto lbCheckEnd;
            }

            pszBufp = strchr(aszBuf, ':');
            if (pszBufp != NULL) {
                *pszBufp = '\0';
            }
            else {
                goto lbCheckEnd;
            }

            tmLen = strlen(aszBuf) - 18;

            if ((tmLen < _MAX_VOLUME)
              &&(tmLen > 1)) {
                strcpy(pszEscapeDir, aszBuf + 18);
                strupr(pszEscapeDir);
            }
            goto lbCheckEnd;
        }
    }

    if (ferror(pOthrFp) != 0) {
        fclose(pOthrFp);
        pOthrFp = NULL;
        return -1;
    }

lbCheckEnd:
    fclose(pOthrFp);
    pOthrFp = NULL;
    return 0;
}
