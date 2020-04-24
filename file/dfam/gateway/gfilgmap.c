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
 * $Log: gfilgmap.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:08  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:29  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:57:20  takano
 * 	First Submit
 * 	[1996/02/21  07:57:14  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nwbindry.h>
#include <nwsnut.h>
#include <errno.h>
#include <niterror.h>

#include "dfamdef.h"
#include "dfamext.h"
#include "dfamstr.h"
#include "dfamprt.h"
#include "err.h"
#include "gfiler.h"


/*EnglishComment**************************************************************/
/*  ID   = GW_DFAMGroup_Map                                                  */
/*                                                                           */
/*  name = Searching bindery for the DFAM group name corresponding to the    */
/*         designated DCE group name.                                        */
/*  func = (1) Scans all the NetWare groups.                                 */
/*         (2) Checks if there is a DCE group property in the property of    */
/*             the NetWare group (which is obtained by the above step).      */
/*         (3) If there is, reads a property value from the DCE group        */
/*             property.                                                     */
/*         (4) Decrypts the property value.                                  */
/*         (5) Compares the DCE group name obtained in (4) with the          */
/*             designated DCE group name.                                    */
/*                                                                           */
/*  i/o  = pszDCEGroupName :char *:inp: DCE Group name                       */
/*         pszDFAMGroupName:char *:out: DFAM Group name                      */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: the designated DCE group name is not registered as          */
/*               a DCE group                                                 */
/*           -2: insufficient memory                                         */
/*           -3: failed to access bindery                                    */
/*           -4: internal error (senl-contradiction)                         */
/*                                                                           */
/* common = none                                                             */
/*          :                                                                */
/* note   = none                                                             */
/*  date = April 17, 1995 by Jun Nakagawa                                    */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


int GW_DFAMGroup_Map(char *pszDCEGroupName,char *pszDFAMGroupName)
{
    int iRc;
    int iRtn;
    long iId;
    WORD ushType;
    char aszNWGroupName[MAX_NWUSERNAME];
    char aszDCEGroupName[MAX_DCEUSERNAME];
    char cProperty;
    char cFlag;
    char cSecurity;

    *pszDFAMGroupName = '\0';
    iId = (long)-1;

    while (1) {

        iRc = ScanBinderyObject("*", OT_GROUP, &iId, aszNWGroupName, &ushType, &cProperty, &cFlag,
                                &cSecurity);

        if (iRc == ESUCCESS) {

            iRtn = GW_DCEGroup_Map(aszNWGroupName, aszDCEGroupName);
            if (iRtn == 0) {

                if (strcmp(pszDCEGroupName, aszDCEGroupName) == 0) {
                    strcpy(pszDFAMGroupName, aszNWGroupName);
                    return 0;
                }
            }
            else if ((iRtn == -2)
                   ||(iRtn == -1)) {
                continue;
            }
            else if (iRtn == -3) {
                return -2;
            }
            else if (iRtn == -4) {
                return -3;
            }
            else {
                return -4;
            }
        }

        else if (iRc == ERR_NO_SUCH_OBJECT) {
            return -1;
        }

        else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
            return -2;
        }

        else {
            return -3;
        }
    }
}


/*EnglishComment**************************************************************/
/*  ID   = GW_DCEGroup_Map                                                   */
/*                                                                           */
/*  name = Searching bindery for the DCE group name corresponding to the     */
/*         designated DFAM group name.                                       */
/*  func = (1) Checks if there is a DCE group property in the property of    */
/*             the DFAM group.                                               */
/*         (2) If there is, reads a property value from the DCE group        */
/*             property.                                                     */
/*                                                                           */
/*  i/o  = pszDFAMGroupName:char *:inp: DFAM Group name                      */
/*         pszDCEGroupName :char *:out: DCE Group name                       */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: not a NetWare group                                         */
/*           -2: not a DFAM group                                            */
/*           -3: insufficient memory                                         */
/*           -4: failed to access bindery                                    */
/*           -5: internal error (senl-contradiction)                         */
/*                                                                           */
/* common = none                                                             */
/*          :                                                                */
/* note   = none                                                             */
/*  date = April 17, 1995 by Jun Nakagawa                                    */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/

int GW_DCEGroup_Map(char *pszDFAMGroupName,char *pszDCEGroupName)
{
    BYTE *pszDCEGroup;
    long iGroupID;
    int iRc;
    int iSegmentNum;
    BYTE aszSegment[SEGMENTNUM];
    BYTE byFlags;
    BYTE byMore;

    *pszDCEGroupName = '\0';

    iRc = GetBinderyObjectID(pszDFAMGroupName, OT_GROUP, &iGroupID);
    switch (iRc) {
      case ESUCCESS :
        break;

      case ERR_SERVER_OUT_OF_MEMORY :
        return -3;
        break;

      case ERR_INVALID_NAME :
      case ERR_NO_SUCH_OBJECT :
      case ERR_WILDCARD_NOT_ALLOWED :
        return -1;
        break;

      default :
        return -4;
        break;
    }

    iSegmentNum = 1;
    pszDCEGroup = pszDCEGroupName;

    while (1) {
        memset(aszSegment, '\0', SEGMENTNUM);
        iRc = ReadPropertyValue(pszDFAMGroupName, OT_GROUP, DCE_GROUP_PROPERTY, iSegmentNum,
                                aszSegment, &byMore, &byFlags);

        if (iRc == ESUCCESS) {

            if (byMore == 0) {
				strcat(pszDCEGroupName,aszSegment);
				break;
			}

            memcpy(pszDCEGroup ,aszSegment ,SEGMENTNUM);
            pszDCEGroup += SEGMENTNUM;
            *pszDCEGroup = '\0';
            iSegmentNum++;
        }

        else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
            *pszDCEGroupName = '\0';
            return -3;
        }

        else if (iRc == ERR_NO_SUCH_OBJECT) {
            *pszDCEGroupName = '\0';
            return -1;
        }

        else if((iRc == ERR_NO_SUCH_PROPERTY)
               ||(iRc == ERR_NO_SUCH_SEGMENT)) {
            *pszDCEGroupName = '\0';
            return -2;
        }

        else if (iRc == ERR_WILDCARD_NOT_ALLOWED){
            *pszDCEGroupName = '\0';
            return -5;
        }

        else {
            *pszDCEGroupName = '\0';
            return -4;
        }
    }
    return 0;
}
