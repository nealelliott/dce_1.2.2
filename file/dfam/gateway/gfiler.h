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
 * $Log: gfiler.h,v $
 * Revision 1.1.2.2  1996/03/11  14:08:05  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:26  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:37:18  takano
 * 	First Submit
 * 	[1996/02/21  07:37:10  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * GFILER.H                                                                  *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
#ifndef GFILER_H
#define GFILER_H

#include <time.h>

#define C_FILE      0
#define C_DIR       1

#define DEFAULT_USER      0
#define DEFAULT_GROUP     1
#define DEFAULT_BOTH      2

#define ADD         0
#define DEL         1

#define C_NONE      0
#define C_ALLBUT    1
#define C_ONLY      2

#define NETWARE     0
#define DFAM        1

#define CURRENT     "."

#define NW_PATHSIZE       304
#define DFS_PATHSIZE     1025

#define UNAMELEN            48

#define COMMLEN             4

#define SERVLEN             48

#define VOLLEN              17

#define TLENGTH             16

#define SUCCESS             0
#define LIB_ERR            -1

#define RETRY               1

#define T_DIR               1
#define T_FILE              2
#define ACL_DIR             3
#define ACL_FILE            4

#define MODE_ERR           -1
#define MAP_ERR            -2

#define VOL_ERR            -1
#define MEM_ERR            -2
#define TOOLONG_ERR        -3

#define FLAG      0xFFFF

#define DUOBJ				1
#define DUSER				2
#define DTHRESHLD			3
#define DGOBJ				4
#define DGRP				8
#define DOTHER				16
#define DMASK				32


struct dgrant_data{
    unsigned char command_name[COMMLEN];
    unsigned short right_mask;
    unsigned char t_list[TLENGTH];
    unsigned char path_name[NW_PATHSIZE];
    unsigned short ug_class;
    unsigned char name[UNAMELEN];
    unsigned char option[2];
};

struct drevoke_data{
    unsigned char command_name[COMMLEN];
    unsigned char t_list[TLENGTH];
    unsigned char path_name[NW_PATHSIZE];
    unsigned short ug_class;
    unsigned char name[UNAMELEN];
    unsigned char option[2];
};

struct dremove_data{
    unsigned char command_name[COMMLEN];
    unsigned char path_name[NW_PATHSIZE];
    unsigned short ug_class;
    unsigned char name[UNAMELEN];
    unsigned char option[2];
};

struct dtlist_data{
    unsigned char command_name[COMMLEN];
    unsigned char path_name[NW_PATHSIZE];
    unsigned short ug_class;
};

struct dright_data{
    unsigned char command_name[COMMLEN];
    unsigned char path_name[NW_PATHSIZE];
    unsigned char user_name[UNAMELEN];
};

struct dlistdir_data{
    unsigned char command_name[COMMLEN];
    unsigned char path_name[NW_PATHSIZE];
    unsigned char option[5];
};

struct dndir_data{
    unsigned char command_name[COMMLEN];
    unsigned char path_name[NW_PATHSIZE];
    unsigned char option[2];
};

struct chkvol_data{
    unsigned char command_name[COMMLEN];
    unsigned char vol_name[VOLLEN];
};

struct chgowner_data{
    unsigned char command_name[COMMLEN];
    unsigned char dir_name[NW_PATHSIZE];
    unsigned char own_name[UNAMELEN];
};

struct chgdate_data{
    unsigned char command_name[COMMLEN];
    unsigned char file_name[NW_PATHSIZE];
    time_t        chg_date;
};

struct viewdate_data{
    unsigned char command_name[COMMLEN];
    unsigned char dir_name[NW_PATHSIZE];
};

struct viewown_data{
    unsigned char command_name[COMMLEN];
    unsigned char dir_name[NW_PATHSIZE];
};

struct add_data{
    char command_name[COMMLEN];
};


struct RetHeader
{
    unsigned char     command_name[COMMLEN];
    unsigned short    DFAM_errno;
    unsigned short    ret_value;
    unsigned short    next_frame;
};


struct RetGrantData
{
    unsigned char     path_name[NW_PATHSIZE];
    unsigned short    dir_info;
    unsigned char     t_list[TLENGTH];
    unsigned short    next_data;
};


struct RetRevokeData
{
    unsigned char     path_name[NW_PATHSIZE];
    unsigned short    dir_info;
    unsigned char     t_list[TLENGTH];
    unsigned short    next_data;
};


struct RetRemoveData
{
    unsigned char     path_name[NW_PATHSIZE];
    unsigned short    dir_info;
    unsigned short    next_data;
};


struct RetTlistData
{
    unsigned char     path_name[NW_PATHSIZE];
    NWOBJ_TYPE        ug_info;
    unsigned char     user_name[UNAMELEN];
    unsigned char     t_info[TLENGTH];
    unsigned short    next_data;
};


struct RetRightsData
{
    unsigned char    rights_list[TLENGTH];
    unsigned short   dir_info;
};



struct RetListdirData
{
    unsigned char     path_name[NW_PATHSIZE];
    time_t            change_date;
    unsigned char     rights_list[TLENGTH];
    unsigned short    level;
    unsigned short    sub;
    unsigned short    next_data;
};


struct RetNdirData
{
    unsigned short    dir_info;
    unsigned char     path_name[NW_PATHSIZE];
    unsigned long     file_size;
    time_t            change_date;
    unsigned char     user_name[UNAMELEN];
    unsigned char     dfs_name[DFS_PATHSIZE+1];
    unsigned char     rights_list[TLENGTH];
    unsigned short    next_data;
};


struct RetChkvolData{
    unsigned short is_vol;
};




struct RetGrantDataKeep
{
    struct RetGrantDataKeep *pNext;
    struct RetGrantData      data;
};

struct RetRevokeDataKeep
{
    struct RetRevokeDataKeep *pNext;
    struct RetRevokeData      data;
};

struct RetRemoveDataKeep
{
    struct RetRemoveDataKeep *pNext;
    struct RetRemoveData      data;
};

struct RetTlistDataKeep
{
    struct RetTlistDataKeep *pNext;
    struct RetTlistData      data;
};

struct RetListdirDataKeep
{
    struct RetListdirDataKeep *pNext;
    struct RetListdirData      data;
};

struct RetNdirDataKeep
{
    struct RetNdirDataKeep *pNext;
    struct RetNdirData      data;
};

struct RetComData
{
    void *pNext;
};

struct TableKeep{
    unsigned char             command_name[COMMLEN];
    void                      *pData;
    int                       task_num;
	unsigned short            agterr;
    char                      *pcAclbuf;
    struct RetHeader          *plRet_st;
    struct RetGrantData       *plRet_gdata;
    struct RetRevokeData      *plRet_rvdata;
    struct RetRemoveData      *plRet_rmdata;
    struct RetTlistData       *plRet_tdata;
    struct RetRightsData      *plRet_rgdata;
    struct RetListdirData     *plRet_ldata;
    struct RetNdirData        *plRet_ndata;
    struct RetChkvolData      *plRet_ckvoldata;
    struct RetGrantDataKeep   **ppGDataKeep;
    struct RetRevokeDataKeep  **ppRvDataKeep;
    struct RetRemoveDataKeep  **ppRmDataKeep;
    struct RetTlistDataKeep   **ppTDataKeep;
    struct RetListdirDataKeep **ppLDataKeep;
    struct RetNdirDataKeep    **ppNDataKeep;
    struct TableKeep          *pNext;
};



struct RightsMapping
{
    int          mode;
    WORD         trustee_list;
    unsigned int ACL_list;
};

#endif
