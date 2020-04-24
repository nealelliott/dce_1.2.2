// EnglishComment
/*****************************************************************************/
/* cli_filr.h :                                                              */
/*                                                                           */
/*  date = March 7, 1995 by Kousuke Takano and Kentarou Chikura              */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nwcaldef.h>
#include <nwdpath.h>
#include <nwcalls.h>
#include "gfiler.h"
#include "err.h"
#include "prot.h"

void	msg_lst(int, char*);
void	trst_chr(char*, char*);
int		parm_pru(int, char**, char***);

extern  unsigned    short   user_group;
extern  unsigned    short   flg_progrm;

#define     DGRANT_MSG      "Usage : DGRANT [ ONLY | ALL BUT ] right-list [ FOR path ] TO [ USER | GROUP ] name\n"      
#define     DLISTDIR_MSG    "Usage : DLISTDIR [ path ] [ option ]\n"                                                    
#define     DNDIR_MSG       "Usage : DNDIR [ path ] [option]\n"                                                         
#define     DREMOVE_MSG     "Usage : DREMOVE [ USER | GROUP ] name [ FROM path ]\n"                                     
#define     DREVOKE_MSG     "Usage : DREVOKE right-list [ FOR path ] FROM [ USER | GROUP ] name\n"                      
#define     DRIGHT_MSG      "Usage : DRIGHT [ path ]\n"                                                                 
#define     DTLIST_MSG      "Usage : DTLIST [ path [ USERS | GROUPS ] ]\n"                                              

#define     DGRANT_ID       "GRNT"
#define     DLISTDIR_ID     "LSTD"
#define     DNDIR_ID        "NDIR"
#define     DREMOVE_ID      "RMOV"
#define     DREVOKE_ID      "RVOK"
#define     DRIGHT_ID       "RGHT"
#define     DTLIST_ID       "TLST"

#define     NCP_NAME    "DFA_Command_Listen"

#define     DATNUM              10
#define     DNDIR_DATNUM        2


#define     DEFINE_USER_H       "User"                          
#define     DEFINE_GROUP_H      "Group"                         
#define     DEFINE_FILE_H       "File"                          
#define     DEFINE_FILE_H2      "files"                         
#define     DEFINE_FILE_H3      "file"                          
#define     DEFINE_DLEC_H       "Directory"                     
#define     DEFINE_DLEC_H2      "directories"                   
#define     DEFINE_DLEC_H3      "directory"                     
#define     DEFINE_USER_T       "user trustee:\n"               
#define     DEFINE_GROUP_T      "group trustee:\n"              
#define     NOT_DCE_USER        "non DCE  User"                 
#define     NOT_GW_USER         "non G/W  User"                 

#define     DNDIR_TIME          "%m-%d-%y %I:%M%p"
#define     DNDIR_TIME_00       "%m-%d-%y 00:%M%p"
#define     DLISTDIR_TIME       "%m-%d-%y   %I:%M%p"
#define     DLISTDIR_TIME_00    "%m-%d-%y   00:%M%p"

#define     MSG_PARM_ERR        "KDDS23007-W  Invalid option\n"                                              
#define     MSG_MEMORY_OVER     "KDDS23017-W  Client out of memory.\n"                                       
#define     MSG_NON_PASSNAME    "KDDS23000-W  No such path or file\n"                                        
#define     MSG_NON_PASS        "KDDS23001-W  No such path\n"                                                
#define     MSG_NON_DIR         "KDDS23002-W  No such subdirectory\n"                                        
#define     MSG_NON_USERNAME    "KDDS23003-W  No such user\n"                                                
#define     MSG_NON_GROUPNAME   "KDDS23004-W  No such group\n"                                               
#define     MSG_LOCALE_DRV      "KDDS23006-W  Local drive is selected: Should be a Network drive.\n"         
#define     MSG_ERR_RIGHT       "KDDS23008-W  Invalid right list\n"                                          
#define     MSG_NON_RIGHT       "KDDS23009-W  No access right to set rights\n"                               
#define     MSG_PASS_MAX        "KDDS23012-W  Converted path name is too long.\n"                            
#define     MSG_NON_DFAM_USER   "KDDS23014-W  Not a Gateway user.\n"                                         
#define     MSG_NON_DLOGIN      "KDDS23015-W  Not logged in to DCE\n"                                        
#define     MSG_ENOTLFS         "KDDS23016-W  A UFS path entered: The selected operation is good for LFS\n"  
#define     MSG_NOT_DEL         "KDDS23018-I  There are unremovable users or groups.\n"                      
#define     MSG_NON_DFAM_VOL    "KDDS23020-W  Not a Gateway Volume\n"                                        
#define     MSG_NON_DFAM_GROUP  "KDDS23022-W  Not a Gateway group\n"                                         
#define     MSG_NODCEENT        "KDDS23024-W  The selected name is not registered with DCE User Group.\n"    
#define     MSG_NOTRLST         "KDDS23026-W  The selected name is not Trustee.\n"                           
#define     MSG_CONN_ERR        "KDDS23110-E  Connection error\n"                                            
#define     MSG_SYSTEM_ERR      "KDDS23120-E  System error\n"                                                
#define     MSG_PROGRM_ERR1     "KDDS23100-E  Internal error (%d)\n"                                         
#define     MSG_PROGRM_ERR2     "KDDS23140-E  An unexpected error occurred to Gateway or Agent (%d)\n"       

#define     MSG_DGRANT_OK       "KDDS23200-I %-12s    Trustee right is [%s]\n"                               

#define     MSG_DLISTDIR_READY  "KDDS23700-I Subdirectory of %s\n"                                           
#define     MSG_DLISTDIR_OK     "KDDS23701-I Number of subdirectories: %d\n"                                 

#define     MSG_DNDIR_OK        "KDDS23800-I %s bytes (%4ld files)\n"                                        

#define     MSG_REMOVE_T_DEL    "KDDS23400-I %s %s has been deleted from the trustees for %s.\n"             
#define     MSG_REMOVE_F_CNT    "KDDS23402-I Trustee %s has been deleted from %ld files.\n"                  
#define     MSG_REMOVE_D_CNT    "KDDS23404-I Trustee %s has been deleted from %ld directories.\n"            

#define     MSG_REVOKE_T_LST    "KDDS23300-I Trustee right is [%s].\n"                                       
#define     MSG_REVOKE_F_CNT    "KDDS23302-I Trustee right for %ld files has been changed to %s.\n"          
#define     MSG_REVOKE_D_CNT    "KDDS23304-I Trustee right for %ld directories has been changed to %s.\n"    

#define     MSG_DRIGHT_T_LST    "KDDS23600-I Effective right for %s is [%s].\n"                              
#define     MSG_DRIGHT_READ_D   "KDDS23604-I Has right to read files.                       (R)*\n"          
#define     MSG_DRIGHT_READ_F   "KDDS23606-I Has right to read files.                       (R) \n"          
#define     MSG_DRIGHT_WRITE_D  "KDDS23608-I Has right to write to files.                   (W)*\n"          
#define     MSG_DRIGHT_WRITE_F  "KDDS23610-I Has right to write to files.                   (W) \n"          
#define     MSG_DRIGHT_ADD_D    "KDDS23612-I Has right to create files and directories.     (C) \n"          
#define     MSG_DRIGHT_ADD_F    "KDDS23614-I Has right to create files and directories.     (C)*\n"          
#define     MSG_DRIGHT_DEL_D    "KDDS23616-I Has right to delete directories.               (E) \n"          
#define     MSG_DRIGHT_DEL_F    "KDDS23618-I Has right to delete files.                     (E)*\n"          
#define     MSG_DRIGHT_CHANGE_D "KDDS23620-I Has right to modify directories.               (M)*\n"          
#define     MSG_DRIGHT_CHANGE_F "KDDS23622-I Has right to modify files.                     (M)*\n"          
#define     MSG_DRIGHT_FIND_D   "KDDS23624-I Has right to search files.                     (F) \n"          
#define     MSG_DRIGHT_FIND_F   "KDDS23625-I Has right to search files.                     (F)*\n"          
#define     MSG_DRIGHT_ACC      "KDDS23626-I Has right to modify access control list.       (A) \n"          
#define     MSG_DRIGHT_CH_D     "KDDS23628-I Has right to modify files and directories.     (Q)*\n"          
#define     MSG_DRIGHT_CH_F     "KDDS23630-I Has right to modify files.                     (Q)*\n"          
#define     MSG_DRIGHT_D_NON    "KDDS23632-I Not effective in directories (denoted by *)\n"                  
#define     MSG_DRIGHT_F_NON    "KDDS23632-I Not effective in files (denoted by *)\n"                        
#define     MSG_DRIGHT_ALL_R    "KDDS23634-I Has all the rights to %s.\n"                                    

#define     MSG_NON_USER_TR     "KDDS23500-I No user trustee\n"                                              
#define     MSG_NON_GRP_TR      "KDDS23502-I No group trustee\n"                                             
#define     MSG_DTLIST_OK       "  %-48s [%s]\n"

#define     FLG_DGRANT          1
#define     FLG_DREVOKE         2
#define     FLG_DREMOVE         3
#define     FLG_DLISTDIR        4
#define     FLG_DNDIR           5
#define     FLG_DTLIST          6
#define     FLG_DRIGHT          7

// EnglishComment
                        /*  0----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8 */
#define DNDIR_FILE_DFS_T1   "MS-DOS Name        DFS Name            Size        Last Update      Owner      \n"
#define DNDIR_FILE_DFS_T2   "------------ --------------------- ------------- ---------------- -------------\n"
                        /*   xxxxxxxxxxxx xxxxxxxxxxxxxxxxxxxxx n,nnn,nnn,nnn mm-dd-yy hh:mmxx xxxxxxxxxxxxx  */

                        /*  0----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8 */
#define DNDIR_FILE_NODFS_T1 "MS-DOS Name      Size        Last Update      Owner      \n"
#define DNDIR_FILE_NODFS_T2 "------------ ------------- ---------------- -------------\n"
                        /*   xxxxxxxxxxxx n,nnn,nnn,nnn mm-dd-yy hh:mmxx xxxxxxxxxxxxx                        */

                        /*  0----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8 */
#define DNDIR_DIR_DFS_T1    "MS-DOS Name         DFS Name           Rights        Owner       Last Update   \n"
#define DNDIR_DIR_DFS_T2    "------------ ----------------------- ----------- ------------- ----------------\n"
                        /*   xxxxxxxxxxxx xxxxxxxxxxxxxxxxxxxxxxx [srwcemfaq] xxxxxxxxxxxxx mm-dd-yy hh:mmxx  */

                        /*  0----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8 */
#define DNDIR_DIR_NODFS_T1  "MS-DOS Name    Rights        Owner       Last Update   \n"
#define DNDIR_DIR_NODFS_T2  "------------ ----------- ------------- ----------------\n"
                        /*   xxxxxxxxxxxx [srwcemfaq] xxxxxxxxxxxxx mm-dd-yy hh:mmxx                          */

                        /*  0----+----1----+----2   */
#define DLISTDIR_TIME_T1    "Date        Time    "
#define DLISTDIR_TIME_T2    "--------------------"
                        /*   mm-dd-yy   hh:mmxx     */

                        /*   ----+----3---          */
#define DLISTDIR_RIGHT_T1   " Rights      "
#define DLISTDIR_RIGHT_T2   "-------------"
                        /*   [srwcemfaq]            */

                        /*   -+----4----+----5----+----6----+----7----+----8 */
#define DLISTDIR_DIR_T1     "  Directory"
#define DLISTDIR_DIR_T2     "----------------------------------"
                        /*   ->xxxxxxxx                             */
                        /*   ->  xxxxxxxx                           */
// EnglishComment
