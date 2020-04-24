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
 * $Log: gliobind.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:26  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:43  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:03:07  takano
 * 	First Submit
 * 	[1996/02/21  08:03:03  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nwbindry.h>
#include <errno.h>
#include <niterror.h>

#include "dfamdef.h"
#include "dfamext.h"
#include "dfamstr.h"
#include "dfamprt.h"


/*EnglishComment**************************************************************/
/*  ID   = GetDFAMUserName                                                   */
/*  name = Gateway user name processing routine                              */
/*  func = Gets a Gateway user name corresponding to the designated DCE      */
/*         user name.                                                        */
/*         (1) Scans all the NetWare users.                                  */
/*         (2) Checks if there is a DCE user property in the properties      */
/*             obtained in the above.                                        */
/*         (3) If exists, this routine reads a property value from the DCE   */
/*             user property.                                                */
/*         (4) Decrypts the property value.                                  */
/*         (5) Compares the DCE user name with the DCE user name obtained    */
/*             in (4).                                                       */
/*                                                                           */
/*  i/o  = pszDCEUserNam   : char * : inp : DCE user name                    */
/*         pszDFAMUserName : char * : out : Gateway User name                */
/*                                                                           */
/*  return =  0 : normal end                                                 */
/*           -1 : the designated DCE user name is not registered as a        */
/*                Gateway user.                                              */
/*           -2 : memory shortage                                            */
/*           -3 : failed to access bindery                                   */
/*           -4 : internal error                                             */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int GetDFAMUserName(pszDCEUserName, pszDFAMUserName)
char *pszDCEUserName;
char *pszDFAMUserName;
{
    long iId;
    int iRc;
    int iRtn;
    WORD ushType;
    char aszNWUserName[MAX_NWUSERNAME];
    char aszDCEUserName[MAX_DCEUSERNAME];
    char aszLogComment[12];
    char cProperty;
    char cFlag;
    char cSecurity;

    GW_tsh_functrace(FUNC_GETDFAMUSERNAME, GW_FUNC_START, 0);

    *pszDFAMUserName = '\0';
    iId = (long)-1;

    while (1) {

        iRc = ScanBinderyObject("*", OT_USER, &iId, aszNWUserName, &ushType, &cProperty, &cFlag,
                                &cSecurity);

        if (iRc == ESUCCESS) {

            iRtn = GetDCEUserName(aszNWUserName, aszDCEUserName);
            if (iRtn == 0) {

                if (strcmp(pszDCEUserName, aszDCEUserName) == 0) {
                    strcpy(pszDFAMUserName, aszNWUserName);
                    GW_tsh_functrace(FUNC_GETDFAMUSERNAME, GW_FUNC_END, 0);
                    return 0;
                }
            }
            else if ((iRtn == -2)
                   ||(iRtn == -1)) {
                continue;
            }
            else if (iRtn == -3) {
                GW_tsh_functrace(FUNC_GETDFAMUSERNAME, GW_FUNC_END, -2);
                return -2;
            }
            else if (iRtn == -4) {




                sprintf(aszLogComment, "%d", iRtn);
                GW_tsh_log(1600, aszLogComment);
                NCP_LogComment(0, aszNWUserName);

                GW_tsh_functrace(FUNC_GETDFAMUSERNAME, GW_FUNC_END, -3);
                return -3;
            }
            else {




                sprintf(aszLogComment, "%d", iRtn);
                GW_tsh_log(1601, aszLogComment);
                NCP_LogComment(0, aszNWUserName);

                GW_tsh_functrace(FUNC_GETDFAMUSERNAME, GW_FUNC_END, -4);
                return -4;
            }
        }

        else if (iRc == ERR_NO_SUCH_OBJECT) {
            GW_tsh_functrace(FUNC_GETDFAMUSERNAME, GW_FUNC_END, -1);
            return -1;
        }

        else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
            GW_tsh_functrace(FUNC_GETDFAMUSERNAME, GW_FUNC_END, -2);
            return -2;
        }

        else {




            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1602, aszLogComment);

            GW_tsh_functrace(FUNC_GETDFAMUSERNAME, GW_FUNC_END, -3);
            return -3;
        }
    }
}


/*EnglishComment**************************************************************/
/*  ID   = GetDCEUserName                                                    */
/*  name = Gateway user name processing routine                              */
/*  func = Gets a Gateway user name corresponding to the designated DCE      */
/*         user name.                                                        */
/*         (1) Checks if there is a DCE user property in the properties of   */
/*             the designated Gateway user.                                  */
/*         (2) If exists, this routine reads a property value from the DCE   */
/*             user property.                                                */
/*         (3) Decrypts the property value with using the NetWare user ID    */
/*             as a decryption key.                                          */
/*                                                                           */
/*  i/o  = pszDFAMUserName : char * : inp : Gateway user name                */
/*         pszDCEUserName  : char * : out : DCE user name                    */
/*                                                                           */
/*  return =  0 : normal end                                                 */
/*           -1 : not a NetWare user                                         */
/*           -2 : not a Gateway user                                         */
/*           -3 : memory shortage                                            */
/*           -4 : failed to access bindery                                   */
/*           -5 : internal error                                             */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int GetDCEUserName(pszDFAMUserName, pszDCEUserName)
char *pszDFAMUserName;
char *pszDCEUserName;
{
    BYTE *pszDCEUser;
    LONG auiKey[2];
    long iUserID;
    int iRc;
    int iSegmentNum;
    char aszLogComment[12];
    BYTE aszSegment[SEGMENTNUM];
    BYTE aszDec[SEGMENTNUM];
    BYTE byFlag;
    BYTE byMore;

    GW_tsh_functrace(FUNC_GETDCEUSERNAME, GW_FUNC_START, 0);

    iRc = GetBinderyObjectID(pszDFAMUserName, OT_USER, &iUserID);
    switch (iRc) {
      case ESUCCESS :
        break;

      case ERR_SERVER_OUT_OF_MEMORY :
        GW_tsh_functrace(FUNC_GETDCEUSERNAME, GW_FUNC_END, -3);
        return -3;
        break;

      case ERR_INVALID_NAME :
      case ERR_NO_SUCH_OBJECT :
      case ERR_WILDCARD_NOT_ALLOWED :
        GW_tsh_functrace(FUNC_GETDCEUSERNAME, GW_FUNC_END, -1);
        return -1;
        break;

      default :




        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1603, aszLogComment);
        NCP_LogComment(0, pszDFAMUserName);

        GW_tsh_functrace(FUNC_GETDCEUSERNAME, GW_FUNC_END, -4);

        return -4;
        break;
    }

    iSegmentNum = 1;
    pszDCEUser = pszDCEUserName;

    auiKey[0] = (LONG)iUserID;
    auiKey[1] = (LONG)iUserID;

    while (1) {
        memset(aszSegment, '\0', SEGMENTNUM);
        iRc = ReadPropertyValue(pszDFAMUserName, OT_USER, DCE_USER_PROPERTY, iSegmentNum,
                                aszSegment, &byMore, &byFlag);

        if (iRc == ESUCCESS) {

            fnGetDecryptData(auiKey, aszSegment, SEGMENTNUM, aszDec);

            if (byMore == 0) {
                strcpy(pszDCEUser, aszDec);
                break;
            }

            strncpy(pszDCEUser ,aszDec ,SEGMENTNUM);
            pszDCEUser += SEGMENTNUM;
            iSegmentNum++;
        }

        else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
            *pszDCEUserName = '\0';
            GW_tsh_functrace(FUNC_GETDCEUSERNAME, GW_FUNC_END, -3);
            return -3;
        }

        else if (iRc == ERR_NO_SUCH_OBJECT) {
            *pszDCEUserName = '\0';
            GW_tsh_functrace(FUNC_GETDCEUSERNAME, GW_FUNC_END, -1);
            return -1;
        }

        else if ((iRc == ERR_NO_SUCH_PROPERTY)
               ||(iRc == ERR_NO_SUCH_SEGMENT)) {
            *pszDCEUserName = '\0';
            GW_tsh_functrace(FUNC_GETDCEUSERNAME, GW_FUNC_END, -2);
            return -2;
        }

        else if (iRc == ERR_WILDCARD_NOT_ALLOWED) {




            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1604, aszLogComment);
            NCP_LogComment(0, pszDFAMUserName);

            *pszDCEUserName = '\0';

            GW_tsh_functrace(FUNC_GETDCEUSERNAME, GW_FUNC_END, -5);
            return -5;
        }

        else {




            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1605, aszLogComment);
            NCP_LogComment(0, pszDFAMUserName);

            *pszDCEUserName = '\0';

            GW_tsh_functrace(FUNC_GETDCEUSERNAME, GW_FUNC_END, -4);
            return -4;
        }
    }
    GW_tsh_functrace(FUNC_GETDCEUSERNAME, GW_FUNC_END, 0);
    return 0;
}


/*EnglishComment**************************************************************/
/*  ID   = GetDCEPassword                                                    */
/*  name = DCE password processing routine                                   */
/*  func = Gets a DCE password corresponding to the designated DCE user name.*/
/*         (1) Checks if there is a DCE password property and a DCE user     */
/*             property in the properties of the designated Gateway user.    */
/*         (2) If they exist, this routine reads a property value from the   */
/*             DCE password property.                                        */
/*         (3) Decrypts the property value with using the NetWare user ID    */
/*             as a decryption key.                                          */
/*                                                                           */
/*  i/o  = pszDFAMUserName : char * : inp : Gateway user name                */
/*         pszDCEPassword  : char * : out : DCE password                     */
/*                                                                           */
/*  return =  0 : normal end                                                 */
/*           -1 : not a NetWare user                                         */
/*           -2 : not a Gateway user                                         */
/*           -3 : memory shortage                                            */
/*           -4 : failed to access bindery                                   */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Dec. 1994 by Hitoshi "Kin-chan" Takahashi                       */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

int GetDCEPassword(pszDFAMUserName, pszDCEPassword)
char *pszDFAMUserName;
char *pszDCEPassword;
{
    BYTE *pszDCEPwd;
    LONG auiKey[2];
    long iUserID;
    int iRc;
    int iSegmentNum;
    char aszLogComment[12];
    BYTE aszSegment[SEGMENTNUM];
    BYTE aszDec[SEGMENTNUM];
    BYTE byFlag;
    BYTE byMore;

    GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_START, 0);

    *pszDCEPassword = '\0';

    iRc = GetBinderyObjectID(pszDFAMUserName, OT_USER, &iUserID);
    switch (iRc) {
      case ESUCCESS :
        break;

      case ERR_SERVER_OUT_OF_MEMORY :
        GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -3);
        return -3;
        break;

      case ERR_INVALID_NAME :
      case ERR_NO_SUCH_OBJECT :
      case ERR_WILDCARD_NOT_ALLOWED :
        GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -1);
        return -1;
        break;

      default :




        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1606, aszLogComment);
        NCP_LogComment(0, pszDFAMUserName);

        GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -4);
        return -4;
        break;
    }

    iSegmentNum = 1;
    pszDCEPwd = pszDCEPassword;

    auiKey[0] = (LONG)iUserID;
    auiKey[1] = (LONG)iUserID;

    iRc = ReadPropertyValue(pszDFAMUserName, OT_USER, DCE_PWD_PROPERTY, iSegmentNum,
                            aszSegment, &byMore, &byFlag);

    switch (iRc) {
      case ESUCCESS :
        break;

      case ERR_SERVER_OUT_OF_MEMORY :
        GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -3);
        return -3;
        break;

      case ERR_NO_SUCH_OBJECT :
        GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -1);
        return -1;
        break;

      case ERR_NO_SUCH_PROPERTY :
      case ERR_NO_SUCH_SEGMENT :
        GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -2);
        return -2;
        break;

      case ERR_WILDCARD_NOT_ALLOWED :




        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1607, aszLogComment);
        NCP_LogComment(0, pszDFAMUserName);

        GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -5);
        return -5;
        break;

      default :




        sprintf(aszLogComment, "%d", iRc);
        GW_tsh_log(1608, aszLogComment);
        NCP_LogComment(0, pszDFAMUserName);

        GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -4);
        return -4;
        break;
    }

    while (1) {
        memset(aszSegment, '\0', SEGMENTNUM);
        iRc = ReadPropertyValue(pszDFAMUserName, OT_USER, DCE_PWD_PROPERTY, iSegmentNum,
                                aszSegment, &byMore, &byFlag);

        if (iRc == ESUCCESS) {

            fnGetDecryptData(auiKey, aszSegment, SEGMENTNUM, aszDec);

            if (byMore == 0) {
                strcpy(pszDCEPwd, aszDec);
                break;
            }

            memcpy(pszDCEPwd ,aszDec ,SEGMENTNUM);
            pszDCEPwd += SEGMENTNUM;
            iSegmentNum++;
        }

        else if (iRc == ERR_SERVER_OUT_OF_MEMORY) {
            memset(pszDCEPassword, '\0', MAX_DCEPASSWORD);
            GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -3);
            return -3;
        }

        else if (iRc == ERR_NO_SUCH_OBJECT) {
            memset(pszDCEPassword, '\0', MAX_DCEPASSWORD);
            GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -1);
            return -1;
        }

        else if ((iRc == ERR_NO_SUCH_PROPERTY)
               ||(iRc == ERR_NO_SUCH_SEGMENT)) {
            memset(pszDCEPassword, '\0', MAX_DCEPASSWORD);
            GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -2);
            return -2;
        }

        else if (iRc == ERR_WILDCARD_NOT_ALLOWED) {




            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1609, aszLogComment);
            NCP_LogComment(0, pszDFAMUserName);

            memset(pszDCEPassword, '\0', MAX_DCEPASSWORD);

            GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -5);
            return -5;
        }

        else {




            sprintf(aszLogComment, "%d", iRc);
            GW_tsh_log(1610, aszLogComment);
            NCP_LogComment(0, pszDFAMUserName);

            memset(pszDCEPassword, '\0', MAX_DCEPASSWORD);

            GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, -4);
            return -4;
        }
    }
    GW_tsh_functrace(FUNC_GETDCEPASSWORD, GW_FUNC_END, 0);
    return 0;
}
