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
 * $Log: dfaagtp.h,v $
 * Revision 1.1.2.2  1996/03/11  14:10:52  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:19  marty]
 *
 * Revision 1.1.2.1  1996/02/28  07:37:04  takano
 * 	First Submit
 * 	[1996/02/28  07:36:42  takano]
 * 
 * $EndLog$
 */


/*
 * dfaagtp.h
 *
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi,Ltd. All Rights Reserved. Derechos Reservados.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include <dce/nbase.h>		
#define     F_ON                (char)1
#define     F_ON1               (char)2
#define     F_ON2               (char)3
#define     F_OFF               (char)0
#define     F_NUL               (char)-1
#define     LOG_MD_OK           (char)0
#define     LOG_MD_NG           (char)1
#define     LOG_MD_OK_C         "SUCCESSFUL"
#define     LOG_MD_NG_C         "FAILED"
#define     LOG_EX_LOGIN        (char)11
#define     LOG_EX_CONF         (char)12
#define     LOG_EX_TREEF        (char)13
#define     LOG_EX_DIRC         (char)14
#define     LOG_EX_FILEC        (char)15
#define     LOG_EX_FILED        (char)16
#define     LOG_EX_DIRD         (char)17
#define     LOG_EX_LOGOUT       (char)18
#define     LOG_EX_LOGIN_C      "DCELOGIN"
#define     LOG_EX_CONF_C       "CONFIG"
#define     LOG_EX_TREEF_C      "TREEFILE"
#define     LOG_EX_DIRC_C       "DIRCREATE:"
#define     LOG_EX_FILEC_C      "FILCREATE:"
#define     LOG_EX_FILED_C      "FILDELETE:"
#define     LOG_EX_DIRD_C       "DIRDELETE:"
#define     LOG_EX_LOGOUT_C     "DCELOGOUT"
#define     DF_DIR              (char)1
#define     DF_FIL              (char)2
#define     DF_DIR_C            "DIR"
#define     DF_FIL_C            "FILE"
#define     BT_BINARY           (char)2
#define     BT_BINARY_C         "BINARY"
#define     BT_TEXT             (char)3
#define     BT_TEXT_C           "TEXT"
#define     MSG_T_LOGIN_ERROR       "KDDS30001-W DCE login failed.\n"
#define     MSG_T_LOGOUT_ERROR      "KDDS30002-W DCE logout failed.\n"
#define     MSG_T_CONF_READ_ERR     "KDDS30003-W Failed to read Configuration File.\n"
#define     MSG_T_TREEF_READ_ERR    "KDDS30004-W Failed to read Tree Generation File.\n"
#define     MSG_T_LOGF_WRITE_ERR    "KDDS30005-W Failed to write to Log File.\n"
#define     MSG_T_CONF_ERROR        "KDDS30011-W Error in Configuration File (line number: %d).\n"
#define     MSG_T_TREEF_ERROR       "KDDS30012-W Error in Tree Generation File (line number: %d).\n"
#define     MSG_T_LOGF_RECREATE     "KDDS30013-Q Log File already exists. Overwrite ? (Y/N) "
#define     MSG_T_END               "KDDS30014-E Cannot run further tests. Will take an immediate exit.\n"
#define     MSG_T_TREE_CREATE       "KDDS30021-I Started to build Test Tree.\n"
#define     MSG_T_DIR_CREATE        "KDDS30022-I Directory %s was made.\n"
#define     MSG_T_FILE_CREATE       "KDDS30023-I File %s was made.\n"
#define     MSG_T_TREE_CREATE_END   "KDDS30024-I Test Tree build completed.\n"
#define     MSG_T_DIR_CREATE_ERR    "KDDS30025-W Error in making Directory %s.\n"
#define     MSG_T_DIR_ACL_ERR       "KDDS30026-W Error in setting ACL to Directory %s.\n"
#define     MSG_T_DIR_OWNER_ERR     "KDDS30027-W Error in setting Owner to Directory %s.\n"
#define     MSG_T_FILE_CREATE_ERR   "KDDS30028-W Error in making File %s.\n"
#define     MSG_T_FILE_ACL_ERR      "KDDS30029-W Error in setting ACL to File %s.\n"
#define     MSG_T_FILE_OWNER_ERR    "KDDS30030-W Error in setting Owner to File %s.\n"
#define     MSG_T_FILE_TIME_ERR     "KDDS30031-W Error in setting Time Stamp to File %s.\n"
#define     MSG_T_FILE_WRITE_ERR    "KDDS30032-W Error in setting contents to File %s.\n"
#define     MSG_T_FILE_SIZE_ERR     "KDDS30033-W Failed to assign enough area to File %s.\n"
#define     MSG_T_DIR_TIME_ERR      "KDDS30034-W Error in setting Time Stamp to Directory %s.\n"
#define     MSG_T_DIR_OWNER_G_ERR   "KDDS30035-W Error in setting Owner Group to Directory %s.\n"
#define     MSG_T_FIL_OWNER_G_ERR   "KDDS30036-W Error in setting Owner Group to File %s.\n"
#define     MSG_T_MEM_OVER_TREE     "KDDS30037-W No enough memory to read Tree Generation File.\n"
#define     MSG_T_TREE_DELETE       "KDDS30041-I Started to delete Test Tree.\n"
#define     MSG_T_DIR_DELETE        "KDDS30042-I Directory %s was deleted.\n"
#define     MSG_T_FILE_DELETE       "KDDS30043-I File %s was deleted.\n"
#define     MSG_T_TREE_DELETE_END   "KDDS30044-I Test Tree Deletion completed.\n"
#define     MSG_T_DIR_DELETE_ERR    "KDDS30045-W Failed to delete Directory %s.\n"
#define     MSG_T_FILE_DELETE_ERR   "KDDS30046-W Failed to delete File %s.\n"
#define     MSG_T_ERROR_MEM_CODE    "            %s()->%s():%d\n"
#define     MSG_T_ERROR_MEM_CODE2   "            %s()->%s():%s\n"
#define     MSG_T_PROGRAM_ERR_MSG   "KDDS30099-E Program Error msg_code(%d)\n"
#define     MSG_T_PROGRAM_ERR       "KDDS30099-E Program Error (%s)\n"
#define     MSG_LOGIN_ERROR         0
#define     MSG_LOGOUT_ERROR        1
#define     MSG_CONF_READ_ERR       2
#define     MSG_TREEF_READ_ERR      3
#define     MSG_LOGF_WRITE_ERR      4
#define     MSG_CONF_ERROR          5
#define     MSG_TREEF_ERROR         6
#define     MSG_LOGF_RECREATE       7
#define     MSG_END                 8
#define     MSG_TREE_CREATE         9
#define     MSG_DIR_CREATE          10
#define     MSG_FILE_CREATE         11
#define     MSG_TREE_CREATE_END     12
#define     MSG_DIR_CREATE_ERR      13
#define     MSG_DIR_ACL_ERR         14
#define     MSG_DIR_OWNER_ERR       15
#define     MSG_FILE_CREATE_ERR     16
#define     MSG_FILE_ACL_ERR        17
#define     MSG_FILE_OWNER_ERR      18
#define     MSG_FILE_TIME_ERR       19
#define     MSG_FILE_WRITE_ERR      20
#define     MSG_FILE_SIZE_ERR       21
#define     MSG_DIR_TIME_ERR        22
#define     MSG_DIR_OWNER_G_ERR     23
#define     MSG_FIL_OWNER_G_ERR     24
#define     MSG_MEM_OVER_TREE       25
#define     MSG_TREE_DELETE         26
#define     MSG_DIR_DELETE          27
#define     MSG_FILE_DELETE         28
#define     MSG_TREE_DELETE_END     29
#define     MSG_DIR_DELETE_ERR      30
#define     MSG_FILE_DELETE_ERR     31
#define     MSG_ERROR_MEM_CODE      32
#define     MSG_ERROR_MEM_CODE2     33
#define     MSG_PROGRAM_ERR_MSG     34
#define     MSG_PROGRAM_ERR         35
#define     TOPDIR_P            0               
#define     LOGFILE_P           1               
#define     TREE_P              2               
#define     LOGINUSER_P         3               
#define     LOGINPASSWORD_P     4               
#define     DCEUSER0_P          5               
#define     DCEUSER1_P          6               
#define     DCEUSER2_P          7               
#define     DCEUSER3_P          8               
#define     DCEUSER4_P          9               
#define     DCEUSER5_P          10              
#define     DCEUSER6_P          11              
#define     DCEUSER7_P          12              
#define     DCEUSER8_P          13              
#define     DCEUSER9_P          14              
#define     DCEGROUP0_P         15              
#define     DCEGROUP1_P         16              
#define     DCEGROUP2_P         17              
#define     DCEGROUP3_P         18              
#define     DCEGROUP4_P         19              
#define     DCEGROUP5_P         20              
#define     DCEGROUP6_P         21              
#define     DCEGROUP7_P         22              
#define     DCEGROUP8_P         23              
#define     DCEGROUP9_P         24              
#define     CONFIG_LEN          1025
#define     CONFIG_CONT         25
#define     T_LEN_P             0               
#define     T_TYPE_P            1               
#define     T_USER0_P           2               
#define     T_USER1_P           3               
#define     T_USER2_P           4               
#define     T_USER3_P           5               
#define     T_USER4_P           6               
#define     T_USER5_P           7               
#define     T_USER6_P           8               
#define     T_USER7_P           9               
#define     T_USER8_P           10              
#define     T_USER9_P           11              
#define     T_GROUP0_P          12              
#define     T_GROUP1_P          13              
#define     T_GROUP2_P          14              
#define     T_GROUP3_P          15              
#define     T_GROUP4_P          16              
#define     T_GROUP5_P          17              
#define     T_GROUP6_P          18              
#define     T_GROUP7_P          19              
#define     T_GROUP8_P          20              
#define     T_GROUP9_P          21              
#define     T_OTHER_P           22              
#define     T_MASK_P            23              
#define     TREE_CLR            (struct tree_for *)0
#define     COMT_1              '#'
#define     COMT_2              "REM"
#define     EXIT                exit(logout_exec(err_flg))
#define     LIN_1               (char)1         
#define     LIN_2               (char)2         
#define     RIGHT_LIST          "rwxcidRWXCID"
#define     CONFIG_NAME         "dfaagtp.cfg"
#define     DEF_OTHER           ""                                              
#define     ALL_RIGHT           "rwxcid"
#define     DEF_MASK            "rwxcid"
#define     FILE_MIX            (long)0
#define     FILE_MAX            1073741823L
#define     DIR_LEVEL_MAX       512
#define MSG_MAX	        512                                                     
struct  utc_tm {                                
    char    week[3];                            
    char    aki1;                               
    char    mon[3];                             
    char    aki2;                               
    char    day[2];                             
    char    aki3;                               
    char    hour[2];                            
    char    koron1;                             
    char    min[2];                             
    char    koron2;                             
    char    sec[2];                             
    char    aki4;                               
    char    year[4];                            
    char    null[3];                            
};                                              
struct  tree_for {                              
    int     level;                              
    char    path[256];                          
    char    type;                               
    struct utc_tm time;                         
    long    len;                                
    char    len_f;                              
    char    bt_type;                            
    char    bt_type_f;                          
    char    user[10][7];                        
    char    user_f[10];                         
    char    group[10][7];                       
    char    group_f[10];                        
    int     owner_u;                            
    int     owner_g;                            
    char    other[7];                           
    char    other_f;                            
    char    mask[7];                            
    char    mask_f;                             
    char    g2_flg[24];                         
    struct  tree_for    *up;                    
    struct  tree_for    *down;                  
    struct  tree_for    *back;                  
    struct  tree_for    *next;                  
};

 

#define	FLAG_USER	0
#define	FLAG_GROUP	1
#define	FLAG_OTHER	2
#define	FLAG_MASK	3

struct acl_list {
	char *name;		
	char *t_list;		
	char flag;		
	struct acl_list *next;	
};

error_status_t	dce_login(char *principal, char *password);
error_status_t	dce_logout(void);
error_status_t	ug_chk(char *name, char flag);
error_status_t	acl_set(char *path, struct acl_list *acl_list);
error_status_t	acl_owner(char *path, char *name, char flag);
error_status_t	time_set(char *path, char *time);

