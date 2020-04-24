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
 * $Log: dfaagtp.c,v $
 * Revision 1.1.2.2  1996/03/11  14:10:47  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:17  marty]
 *
 * Revision 1.1.2.1  1996/02/28  07:12:15  takano
 * 	First Submit
 * 	[1996/02/28  07:11:24  takano]
 * 
 * $EndLog$
 */

#include "dfaagtp.h"

static  struct  CONFIG_KEYWORD  {               
    char    key[16];                            
    char    flg;                                
} con_key[] = {                                 
  "TOPDIR=",        F_OFF,                              "LOGFILE=",       F_OFF, 
  "TREE=",          F_OFF,                              "LOGINUSER=",     F_OFF, 
  "LOGINPASSWORD=", F_OFF,                              "DCEUSER0=",      F_OFF, 
  "DCEUSER1=",      F_OFF,                              "DCEUSER2=",      F_OFF, 
  "DCEUSER3=",      F_OFF,                              "DCEUSER4=",      F_OFF, 
  "DCEUSER5=",      F_OFF,                              "DCEUSER6=",      F_OFF, 
  "DCEUSER7=",      F_OFF,                              "DCEUSER8=",      F_OFF, 
  "DCEUSER9=",      F_OFF,                              "DCEGROUP0=",     F_OFF, 
  "DCEGROUP1=",     F_OFF,                              "DCEGROUP2=",     F_OFF, 
  "DCEGROUP3=",     F_OFF,                              "DCEGROUP4=",     F_OFF, 
  "DCEGROUP5=",     F_OFF,                              "DCEGROUP6=",     F_OFF, 
  "DCEGROUP7=",     F_OFF,                              "DCEGROUP8=",     F_OFF, 
  "DCEGROUP9=",     F_OFF,                              "\xff",           F_OFF};

static  struct  CONFIG_DEF  {                   
    char    def[50];                            
} con_def[] = {                                 
    "/.../sales.hitachi.co.jp/fs/dfatree",  
    "/.../sales.hitachi.co.jp/fs/dfaagtp/dfaagtp.log",  
    "/.../sales.hitachi.co.jp/fs/dfaagtp/dfaagtp.tre",
    "dfalogin",                                     "dfalogin",   
    "dfauser0",                                     "dfauser1",   
    "dfauser2",                                     "dfauser3",   
    "dfauser4",                                     "dfauser5",   
    "dfauser6",                                     "dfauser7",   
    "dfauser8",                                     "dfauser9",   
    "dfagroup0",                                    "dfagroup1",  
    "dfagroup2",                                    "dfagroup3",  
    "dfagroup4",                                    "dfagroup5",  
    "dfagroup6",                                    "dfagroup7",  
    "dfagroup8",                                    "dfagroup9",  
    "\xff" };     

static  struct  TREE_KEYWORD  {                 
    char    key[11];                            
} dir_key[] = {                                 
    "LEN=",                                         "TYPE=",      
    "DCEUSER0=",                                    "DCEUSER1=",  
    "DCEUSER2=",                                    "DCEUSER3=",  
    "DCEUSER4=",                                    "DCEUSER5=",  
    "DCEUSER6=",                                    "DCEUSER7=",  
    "DCEUSER8=",                                    "DCEUSER9=",  
    "DCEGROUP0=",                                   "DCEGROUP1=", 
    "DCEGROUP2=",                                   "DCEGROUP3=", 
    "DCEGROUP4=",                                   "DCEGROUP5=", 
    "DCEGROUP6=",                                   "DCEGROUP7=", 
    "DCEGROUP8=",                                   "DCEGROUP9=", 
    "OTHER=",                                       "MASK=",      
    "\xff" };     

static  struct  MSG_TBL {                       
    char    text[100];                          
    char    e_flg;                              
} msg_tbl[] = {                                 
    MSG_T_LOGIN_ERROR,      F_ON1,              
    MSG_T_LOGOUT_ERROR,     F_ON1,              
    MSG_T_CONF_READ_ERR,    F_ON1,              
    MSG_T_TREEF_READ_ERR,   F_ON1,              
    MSG_T_LOGF_WRITE_ERR,   F_ON1,              
    MSG_T_CONF_ERROR,       F_OFF,              
    MSG_T_TREEF_ERROR,      F_OFF,              
    MSG_T_LOGF_RECREATE,    F_OFF,              
    MSG_T_END,              F_OFF,              
    MSG_T_TREE_CREATE,      F_OFF,              
    MSG_T_DIR_CREATE,       F_OFF,              
    MSG_T_FILE_CREATE,      F_OFF,              
    MSG_T_TREE_CREATE_END,  F_OFF,              
    MSG_T_DIR_CREATE_ERR,   F_ON1,              
    MSG_T_DIR_ACL_ERR,      F_ON1,              
    MSG_T_DIR_OWNER_ERR,    F_ON1,              
    MSG_T_FILE_CREATE_ERR,  F_ON1,              
    MSG_T_FILE_ACL_ERR,     F_ON1,              
    MSG_T_FILE_OWNER_ERR,   F_ON1,              
    MSG_T_FILE_TIME_ERR,    F_ON1,              
    MSG_T_FILE_WRITE_ERR,   F_ON1,              
    MSG_T_FILE_SIZE_ERR,    F_ON1,              
    MSG_T_DIR_TIME_ERR,     F_ON1,              
    MSG_T_DIR_OWNER_G_ERR,  F_ON1,              
    MSG_T_FIL_OWNER_G_ERR,  F_ON1,              
    MSG_T_MEM_OVER_TREE,    F_OFF,              
    MSG_T_TREE_DELETE,      F_OFF,              
    MSG_T_DIR_DELETE,       F_OFF,              
    MSG_T_FILE_DELETE,      F_OFF,              
    MSG_T_TREE_DELETE_END,  F_OFF,              
    MSG_T_DIR_DELETE_ERR,   F_ON1,              
    MSG_T_FILE_DELETE_ERR,  F_ON1,              
    MSG_T_ERROR_MEM_CODE,   F_NUL,              
    MSG_T_ERROR_MEM_CODE2,  F_NUL,              
    MSG_T_PROGRAM_ERR_MSG,  F_ON1,              
    MSG_T_PROGRAM_ERR,      F_ON1,              
    "\xff", 0, F_NUL };                         

static  char    week[][4]   =   {   "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT", "???" };

static  char    mon[][4]    =   {   "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };

static  struct  tree_for    *tree_top;          
static  struct  tree_for    *tree_w;            
static  struct  tree_for    *tree_b;            
static  char    tree_flg;                       
#define     T_TOP    tree_top[0]                
#define     T_W      tree_w[0]                  
#define     T_B      tree_b[0]                  

static  char   config[CONFIG_CONT][CONFIG_LEN]; 
                                                
                                                
                                                
                                                
                                                
                                                
static  time_t  timer;                          
static  char    st_time[25];                    
static  char    ed_time[25];                    
static  char    log_flg;                        
static  char    err_mcall[50];                  
static  char    err_call[50];                   
static  int     err_code;                       
static  char    err_code2[50];                  
static  char    t_msg[MSG_MAX];                                                 
static  char    err_flg;                        
static  char    login_flg;                      
static  char    lin[2049];                      

static  struct  acl_list    acl_w[23];          
#define     ACL_CLR     (struct acl_list *)0    

    FILE    *fp;                                    
    FILE    *fp_con;                                
    FILE    *fp_tree;                               
    FILE    *w_fp;                                  

/*****************************************************************************/
/*  ID   = chk_count()                                                       */
/*                                                                           */
/*  name = Numeral check                                                     */
/*  func = Checks if the characters in the string are numeric.               */
/*                                                                           */
/*  i/o  = buf:char * :INP: target character string                          */
/*         len:int    :IN : size of the target character string              */
/*                                                                           */
/*  return =  0: all numeral                                                 */
/*           -1: not all numeral                                             */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = August 17, 1995  by Kentarou Chikura                              */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
int     chk_count( char *buf, int len )
{
    int i;                                          

    for( i=0 ; i < len ; i++ ) {                    
        if(!isdigit( (int)buf[i] ) )                
            return( -1 );                           
    }                                               
    return( 0 );                                    
}

/*****************************************************************************/
/*  ID   = msg_text()                                                        */
/*                                                                           */
/*  name = Message generation                                                */
/*  func = Puts an item whose length is variable to a message, and makes     */
/*         the message ready to display.                                     */
/*                                                                           */
/*  i/o  = code:int    :INP: message code                                    */
/*         msg :char * :OUT: message text                                    */
/*         parm:void * :INP: variable-length item                            */
/*                                                                           */
/*  return =  0: successfully completed                                      */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = August 19, 1995  by Kentarou Chikura                              */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
int msg_text( int code, char *msg, void *parm )
{
    char    *s_x;                                   
    int     *d_x;                                   
    char    msg_add[100];                           
    int     i, j;                                   

    switch ( code ) {                               
                                                    
        case MSG_CONF_ERROR      :                  
        case MSG_TREEF_ERROR     :                  
            d_x = (int *)parm;                      
            sprintf( msg, msg_tbl[code].text, *d_x );
            break;                                  
                                                    
        case MSG_DIR_CREATE      :                  
        case MSG_FILE_CREATE     :                  
        case MSG_DIR_CREATE_ERR  :                  
        case MSG_DIR_ACL_ERR     :                  
        case MSG_DIR_OWNER_ERR   :                  
        case MSG_FILE_CREATE_ERR :                  
        case MSG_FILE_ACL_ERR    :                  
        case MSG_FILE_OWNER_ERR  :                  
        case MSG_FILE_TIME_ERR   :                  
        case MSG_FILE_WRITE_ERR  :                  
        case MSG_FILE_SIZE_ERR   :                  
        case MSG_DIR_TIME_ERR    :                  
        case MSG_DIR_DELETE      :                  
        case MSG_FILE_DELETE     :                  
        case MSG_DIR_DELETE_ERR  :                  
        case MSG_FILE_DELETE_ERR :                  
        case MSG_DIR_OWNER_G_ERR :                  
        case MSG_FIL_OWNER_G_ERR :                  
            s_x = (char *)parm;                     
            sprintf( msg, msg_tbl[code].text, s_x ); 
            break;                                  
                                                    
        case MSG_LOGIN_ERROR     :                  
        case MSG_LOGOUT_ERROR    :                  
        case MSG_CONF_READ_ERR   :                  
        case MSG_TREEF_READ_ERR  :                  
        case MSG_LOGF_WRITE_ERR  :                  
        case MSG_LOGF_RECREATE   :                  
        case MSG_END             :                  
        case MSG_TREE_CREATE     :                  
        case MSG_TREE_CREATE_END :                  
        case MSG_TREE_DELETE     :                  
        case MSG_TREE_DELETE_END :                  
        case MSG_PROGRAM_ERR     :                  
        case MSG_MEM_OVER_TREE   :                  
            strcpy( msg, msg_tbl[code].text );      
            break;                                  
        default:                                    
            sprintf( msg, MSG_T_PROGRAM_ERR_MSG, code );
            printf("%s", msg );                     
            exit(-1);                               
            break;                                  
    }                                               
    if( msg_tbl[code].e_flg == F_ON1 ) {            
        sprintf( msg_add, MSG_T_ERROR_MEM_CODE,     
                err_mcall, err_call, err_code );    
        strcat( msg, msg_add );                     
    }                                               
    return( 0 );                                    
}

/*****************************************************************************/
/*  ID   = logfile_put()                                                     */
/*                                                                           */
/*  name = Writing to the logging file                                       */
/*  func = (1) Opens the logging file.                                       */
/*         (2) Generates the logging file information, and writes it onto    */
/*             the logging file.                                             */
/*         (3) Writes a message onto the logging file.                       */
/*                                                                           */
/*  i/o  = mode:char   :INP: normal/abnormal indicator                       */
/*         exec:char   :INP: the function to be executed                     */
/*         msg :char * :INP: the 1st line of the message                     */
/*         file:char * :INP: file/directory name processed                   */
/*                                                                           */
/*  return =  0: successfully completed                                      */
/*           -1: an access error when attempted to write on the logging file */
/*            1: the process terminated                                      */
/*                                                                           */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = August 19, 1995  by Kentarou Chikura                              */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
int logfile_put( char mode, char exec, char *x_msg, char *filen )
{
    int     rc;                                     
    unsigned char   key, inkey;                     
    char    msg[MSG_MAX];                           
    int     i;                                      

    memset( lin, 0x00, 2048 );
    if( con_key[LOGFILE_P].flg == F_OFF ) {         
        return(0);                                  
    }                                               
    if( log_flg == F_OFF ) {                        
        if( ( access( config[LOGFILE_P], F_OK ) ) == 0 ) { 
            key = ' ';                              
            while( key != 'Y' && key != 'N' ) {     
                msg_text( MSG_LOGF_RECREATE, msg, 0x00 ); 
                printf( "%s", msg );                
                i=0;                                
                while ( ( inkey = (unsigned char)getc(stdin) )
                         != (unsigned char)'\n' ) { 
                    inkey = (unsigned char)         
                            toupper( (int)inkey );  
                    key = inkey;                    
                    i++;                            
                }                                   
                if( i != 1 )                        
                    key = 0x00;                     
            }                                       
            if( key == 'Y' ) {                      
                if( (fp = fopen(config[LOGFILE_P],"w") )   
                        == NULL ) {                 
                    strcpy( err_call, "fopen" );    
                    err_code = errno;               
                    msg_text(MSG_LOGF_WRITE_ERR,x_msg,0x00); 
                    return( -1 );                   
                }                                   
            } else {                                
                return( 1 );                        
            }                                       
        } else if( errno == ENOENT ) {              
            if( (fp = fopen(config[LOGFILE_P],"w") )   
                      == NULL ) {                   
                strcpy( err_call, "fopen" );        
                err_code = errno;                   
                msg_text(MSG_LOGF_WRITE_ERR,x_msg,0x00); 
                return( -1 );                       
            }                                       
        } else {                                    
            strcpy( err_call, "access" );           
            err_code = errno;                       
            msg_text(MSG_LOGF_WRITE_ERR,x_msg,0x00); 
            return( -1 );                           
        }                                           
    } else {                                        
        if( (fp = fopen( config[LOGFILE_P], "a" ) )    
                 == NULL ) {                        
            strcpy( err_call, "fopen" );            
            err_code = errno;                       
            msg_text( MSG_LOGF_WRITE_ERR,x_msg, 0x00 ); 
            return( -1 );                           
        }                                           
    }                                               
    strcpy( lin, st_time );                         
    strcat( lin, " " );                             
    time( &timer );                                 
    memset( ed_time, 0x00, 25 );                    
    memcpy( ed_time, ctime(&timer), 24 );           
    strcat( lin, ed_time );                         
    strcat( lin, " " );                             
    if( mode == LOG_MD_OK ) {                       
        strcat( lin, LOG_MD_OK_C );                 
    } else {                                        
        strcat( lin, LOG_MD_NG_C );                 
    }                                               
    strcat( lin, " " );                             
    switch( exec ) {                                
        case LOG_EX_LOGIN :                         
            strcat( lin, LOG_EX_LOGIN_C );          
            break;                                  
        case LOG_EX_CONF  :                         
            strcat( lin, LOG_EX_CONF_C );           
            break;                                  
        case LOG_EX_TREEF :                         
            strcat( lin, LOG_EX_TREEF_C );          
            break;                                  
        case LOG_EX_DIRC  :                         
            strcat( lin, LOG_EX_DIRC_C );           
            strcat( lin, filen );                   
            break;                                  
        case LOG_EX_FILEC :                         
            strcat( lin, LOG_EX_FILEC_C );          
            strcat( lin, filen );                   
            break;                                  
        case LOG_EX_FILED :                         
            strcat( lin, LOG_EX_FILED_C );          
            strcat( lin, filen );                   
            break;                                  
        case LOG_EX_DIRD  :                         
            strcat( lin, LOG_EX_DIRD_C );           
            strcat( lin, filen );                   
            break;                                  
        case LOG_EX_LOGOUT :                        
            strcat( lin, LOG_EX_LOGOUT_C );         
            break;                                  
        default:                                    
            break;                                  
    }                                               
    strcat( lin, "\n" );                            
    if ( fputs( lin, fp ) == EOF ) {                
        strcpy( err_call, "fputs" );                
        err_code = errno;                           
        msg_text( MSG_LOGF_WRITE_ERR,x_msg, 0x00 ); 
        fclose( fp );                               
        fp = NULL;                                  
        return( -1 );                               
    }                                               
    if( mode == LOG_MD_NG ) {                       
        if ( fputs( x_msg, fp ) == EOF ) {          
            strcpy( err_call, "fputs" );            
            err_code = errno;                       
            msg_text( MSG_LOGF_WRITE_ERR,x_msg, 0x00 );   
            fclose( fp );                           
            fp = NULL;                              
            return( -1 );                           
        }                                           
    }                                               
    if( fclose( fp ) == EOF ) {                     
        strcpy( err_call, "fclose" );               
        err_code = errno;                           
        msg_text(MSG_LOGF_WRITE_ERR,x_msg,T_W.path); 
        fp = NULL;                                  
        return( -1 );                               
    }                                               
    fp = NULL;                                      
    log_flg = F_ON;                                 
    return( 0 );                                    
}

/*****************************************************************************/
/*  ID   = logout_exec()                                                     */
/*                                                                           */
/*  name = DCE logout                                                        */
/*  func = Logs out from DCE, and checks errors.                             */
/*                                                                           */
/*  i/o  = err_flg:char :INP: indicator for after-the-error/not              */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: abnormal end                                                */
/*                                                                           */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = August 17, 1995  by Kentarou Chikura                              */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
int logout_exec( char err_flg )                     
{
    error_status_t  api_rc;                         
    int rc;                                         

    time( &timer );                                 
    memset( st_time, 0x00, 25 );                    
    memcpy( st_time, ctime(&timer), 24 );           
    if( login_flg == F_ON ) {                       
        api_rc = dce_logout();                      
        if( api_rc != error_status_ok ) {           
            strcpy( err_call, "dce_logout" );       
            err_code = api_rc;                      
            msg_text( MSG_LOGOUT_ERROR, t_msg, 0x00 ); 
            printf("%s", t_msg );                   
            if( err_flg == F_OFF ) {                
                rc = logfile_put( LOG_MD_NG,        
                            LOG_EX_LOGOUT, t_msg, 0x00 ); 
            }                                       
            msg_text( MSG_END, t_msg, 0x00 );       
            printf("%s", t_msg );                   
            err_flg = F_ON;                         
        } else {                                    
            if( err_flg == F_OFF ) {                
                rc = logfile_put( LOG_MD_OK, LOG_EX_LOGOUT, 
                                    t_msg, 0x00 );  
                if (rc != 0) {                      
                    err_flg = F_ON;                 
                }                                   
            }                                       
        }                                           
    }                                               
    if( err_flg == F_ON ) {                         
        return( -1 );                               
    } else {                                        
        return( 0 );                                
    }                                               
}

/*****************************************************************************/
/*  ID   = tree_free()                                                       */
/*                                                                           */
/*  name = Releasing the tree generation file table                          */
/*  func = Frees the area for the tree generation file table.                */
/*         If successfully released, modifies the time stamp and owner of    */
/*         the file/directory on the tree.                                   */
/*                                                                           */
/*  i/o    = none                                                            */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 17, 1995  by Kentarou Chikura                            */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
void    tree_free( void )
{
    char    log_ex_flg;                             
    int     j;                                      
    error_status_t  api_rc;                         
    int     rc;                                     

    if( tree_flg == F_ON )                          
        free( tree_w );                             
    tree_w = tree_top;                              
    if( tree_top == TREE_CLR )                      
        return;                                     
    if( err_flg == F_OFF ) {                        
        if( chdir( config[TOPDIR_P] ) == -1 ) {     
            strcpy( err_call, "chdir" );            
            err_code = errno;                       
            msg_text( MSG_DIR_CREATE_ERR, t_msg,    
                        config[TOPDIR_P] );         
            printf("%s", t_msg );                   
            rc = logfile_put( LOG_MD_NG, LOG_EX_DIRC,
                              t_msg, config[TOPDIR_P] ); 
            err_flg = F_ON;                         
        }                                           
    }                                               
    while ( tree_w != TREE_CLR ) {                  
        if( T_W.down == TREE_CLR ) {                
            tree_b = tree_w;                        
            if( T_W.up != TREE_CLR ) {              
                if( T_W.next == TREE_CLR ) {        
                    (T_W.up)->down = TREE_CLR;      
                    tree_w = T_W.up;                
                } else {                            
                    (T_W.up)->down = T_W.next;      
                    (T_W.next)->back = TREE_CLR;    
                    tree_w = T_W.next;              
                }                                   
            } else {                                
                if( T_W.next == TREE_CLR ) {        
                    tree_w = TREE_CLR;              
                } else {                            
                    (T_W.next)->back = TREE_CLR;    
                    tree_w = T_W.next;              
                }                                   
            }                                       
            if( err_flg == F_OFF ) {                
                if( T_B.type == DF_DIR ) {          
                    log_ex_flg = LOG_EX_DIRC;       
                } else {                            
                    log_ex_flg = LOG_EX_FILEC;      
                }                                   
                if( T_B.time.week[0] != (char)0x00 ) {
                    api_rc = time_set( T_B.path, (char *)&(T_B.time) ); 
                    if( api_rc != error_status_ok ) {
                        strcpy( err_call, "time_set" );
                        err_code = api_rc;
                        if( T_B.type == DF_DIR )
                            msg_text( MSG_DIR_TIME_ERR, t_msg, T_B.path );
                        else
                            msg_text( MSG_FILE_TIME_ERR, t_msg, T_B.path );
                        printf("%s", t_msg );
                        rc = logfile_put( LOG_MD_NG,
                                 log_ex_flg,  t_msg, T_B.path ); 
                        err_flg = F_ON;
                    }                                       
                }                                           
                if( err_flg == F_OFF && T_B.owner_u != -1 ) {
                    memset( (void*)&acl_w, 0x00,    
                            sizeof(struct acl_list)*23 ); 
                    j = 0;                          
                    acl_w[j].next = ACL_CLR;        
                    acl_w[j].name = config[LOGINUSER_P]; 
                    acl_w[j].t_list = T_B.user[T_B.owner_u]; 
                    acl_w[j].flag = FLAG_USER;      
                    api_rc = acl_set( T_B.path, &acl_w[0] ); 
                    if( api_rc != error_status_ok ){ 
                        strcpy( err_call, "acl_set" ); 
                        err_code = api_rc;          
                        if( T_B.type == DF_DIR )    
                            msg_text( MSG_DIR_ACL_ERR,  
                                 t_msg, T_B.path ); 
                        else                        
                            msg_text( MSG_FILE_ACL_ERR, 
                                 t_msg, T_B.path ); 
                        printf("%s", t_msg );       
                        rc = logfile_put( LOG_MD_NG,    
                                 log_ex_flg,  t_msg, T_B.path ); 
                        err_flg = F_ON;             
                    } else {                        
                        api_rc = acl_owner( T_B.path, 
                                    config[T_B.owner_u+DCEUSER0_P], 
                                    FLAG_USER );    
                        if( api_rc != error_status_ok ){ 
                            strcpy( err_call, "acl_owner" ); 
                            err_code = api_rc;      
                            if( T_B.type == DF_DIR ) 
                                msg_text( MSG_DIR_OWNER_ERR,    
                                     t_msg, T_B.path ); 
                            else                    
                                msg_text( MSG_FILE_OWNER_ERR,   
                                     t_msg, T_B.path ); 
                            printf("%s", t_msg );       
                            rc = logfile_put( LOG_MD_NG,    
                                     log_ex_flg,  t_msg, T_B.path ); 
                            err_flg = F_ON;         
                        }                           
                    }                               
                }                                   
                if( err_flg == F_OFF && tree_w != TREE_CLR ) {
                    if( T_W.level < T_B.level ) {   
                        if( chdir( ".." ) == -1 ) { 
                            strcpy( err_call, "chdir" ); 
                            err_code = errno;       
                            if( T_W.type == DF_DIR ) 
                                msg_text( MSG_DIR_CREATE_ERR,   
                                         t_msg, T_W.path ); 
                            else                    
                                msg_text( MSG_FILE_CREATE_ERR,  
                                         t_msg, T_W.path ); 
                            printf("%s", t_msg );   
                            rc = logfile_put( LOG_MD_NG, 
                                     log_ex_flg,  t_msg, T_W.path ); 
                            err_flg = F_ON;         
                        }                           
                    }                               
                }                                   
            }                                       
            free( tree_b );                         
        } else {                                    
            if( err_flg == F_OFF ) {                
                if( chdir( T_W.path ) == -1 ) {     
                    strcpy( err_call, "chdir" );    
                    err_code = errno;               
                    if( T_W.type == DF_DIR )        
                        msg_text( MSG_DIR_CREATE_ERR, t_msg, T_W.path ); 
                    else                            
                        msg_text( MSG_FILE_CREATE_ERR, t_msg, T_W.path ); 
                    printf("%s", t_msg );           
                    rc = logfile_put( LOG_MD_NG,    
                             log_ex_flg,  t_msg, T_W.path ); 
                    err_flg = F_ON;                 
                }                                   
            }                                       
            tree_w = T_W.down;                      
        }                                           
    }                                               
    tree_top = tree_b = TREE_CLR;                   
}

/*****************************************************************************/
/*  ID   = err_exec()                                                        */
/*                                                                           */
/*  name = Error handling                                                    */
/*  func = Handles outputting to the logging file, and logging out.          */
/*                                                                           */
/*  i/o    = log_ex_flg:char  :INP: the status of the logging file           */
/*           df_name   :char *:INP: file/directory name for the logging file */
/*                                  (LOG_EX_DIRC LOG_EX_FILEC)               */
/*  return = none (exit() terminates the process if an error occurred)       */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 17, 1995  by Kentarou Chikura                            */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
void    err_exec( char log_ex_flg, char *df_name )
{
    int rc;                                         

    if( fp_tree != NULL )                           
        fclose(fp_tree);                            
    if( fp_con != NULL )                            
        fclose(fp_con);                             
    err_flg = F_ON;                                 
    tree_free();                                    
    printf("%s", t_msg );                           
    if( log_ex_flg == LOG_EX_DIRC ||                
        log_ex_flg == LOG_EX_FILEC )                
        rc = logfile_put( LOG_MD_NG, log_ex_flg,    
                          t_msg, df_name );         
    else                                            
        rc = logfile_put( LOG_MD_NG, log_ex_flg,    
                          t_msg, 0x00 );            
    msg_text( MSG_END, t_msg, 0x00 );               
    printf("%s", t_msg );                           
    EXIT;                                           
}

/*****************************************************************************/
/*  ID   = conf_get()                                                        */
/*                                                                           */
/*  name = Inputting the configuration file                                  */
/*  func = Inputs the configuration file, and gets the defined content of    */
/*         each item.                                                        */
/*                                                                           */
/*  i/o    = none                                                            */
/*  return = none (exit() terminates the process if an error occurred)       */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 17, 1995  by Kentarou Chikura                            */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
void    conf_get( void )
{
    char    *ps_pot;                                
    char    *pe_pot;                                
    int     chk_p;                                  
    int     i, j;                                   
    int     rc;                                     
    error_status_t  api_rc;                         
    char    def_u_flg, def_g_flg;                   

    strcpy( err_mcall, "conf_get" );                
    time( &timer );                                 
    memset( st_time, 0x00, 25 );                    
    memcpy( st_time, ctime(&timer), 24 );           
    if ( ( fp_con = fopen( CONFIG_NAME, "r" ) )     
             == NULL ) {                            
        strcpy( err_call, "fopen" );                
        err_code = errno;                           
        msg_text( MSG_CONF_READ_ERR, t_msg, 0x00 ); 
        printf("%s", t_msg );                       
        msg_text( MSG_END, t_msg, 0x00 );           
        printf("%s", t_msg );                       
        err_flg = F_ON;                             
        EXIT;                                       
    } else {                                        
        i = 1;                                      
        while( fgets( lin, 1100, fp_con ) != NULL ) { 
            if( lin[0] != COMT_1 &&                 
                strncasecmp( lin, COMT_2, strlen( COMT_2 ) ) != 0 ) { 
                if( (ps_pot = strchr( lin, '=' ) ) == NULL ) { 
                    msg_text( MSG_CONF_ERROR, t_msg,
                                (void *)&i );       
                    fclose( fp_con );               
                    err_exec( LOG_EX_CONF, 0x00 );  
                }                                   
                for( j=0, chk_p = -1 ;              
                     con_key[j].key[0] != (char)0xff ; 
                     j++ ) {                        
                    if( strncasecmp( lin, con_key[j].key,   
                            strlen(con_key[j].key) ) == 0 ) {   
                        chk_p = j;                  
                    }                               
                }                                   
                switch( chk_p ) {                   
                    case TOPDIR_P        :          
                    case LOGFILE_P       :          
                    case TREE_P          :          
                    case LOGINUSER_P     :          
                    case LOGINPASSWORD_P :          
                    case DCEUSER0_P      :          
                    case DCEUSER1_P      :          
                    case DCEUSER2_P      :          
                    case DCEUSER3_P      :          
                    case DCEUSER4_P      :          
                    case DCEUSER5_P      :          
                    case DCEUSER6_P      :          
                    case DCEUSER7_P      :          
                    case DCEUSER8_P      :          
                    case DCEUSER9_P      :          
                    case DCEGROUP0_P     :          
                    case DCEGROUP1_P     :          
                    case DCEGROUP2_P     :          
                    case DCEGROUP3_P     :          
                    case DCEGROUP4_P     :          
                    case DCEGROUP5_P     :          
                    case DCEGROUP6_P     :          
                    case DCEGROUP7_P     :          
                    case DCEGROUP8_P     :          
                    case DCEGROUP9_P     :          
                        if( con_key[chk_p].flg == F_ON ) { 
                            msg_text( MSG_CONF_ERROR, t_msg,
                                        (void *)&i );   
                            fclose( fp_con );       
                            err_exec( LOG_EX_CONF, 0x00 ); 
                        } else {                    
                            if( ( pe_pot =          
                                  strchr( ++ps_pot, '\n' ) )    
                                != NULL )           
                                pe_pot[0] = (char)0x00; 
                            if( strlen( ps_pot ) > 1024 ) { 
                                msg_text( MSG_CONF_ERROR,
                                         t_msg, (void *)&i );   
                                fclose( fp_con );   
                                err_exec( LOG_EX_CONF, 0x00 ); 
                            } else {                
                                strcpy( config[chk_p], ps_pot ); 
                                con_key[chk_p].flg = F_ON; 
                            }                       
                        }                           
                        if( chk_p>=DCEUSER0_P &&    
                            chk_p<=DCEUSER9_P ) {   
                            api_rc = ug_chk( config[chk_p], 
                                      FLAG_USER );  
                            if( api_rc != error_status_ok ) {   
                                msg_text( MSG_CONF_ERROR,
                                         t_msg, (void *)&i );   
                                fclose( fp_con );   
                                err_exec( LOG_EX_CONF, 0x00 ); 
                            }                       
                        }                           
                        if( chk_p>=DCEGROUP0_P &&   
                            chk_p<=DCEGROUP9_P ) {  
                            api_rc = ug_chk( config[chk_p], 
                                      FLAG_GROUP ); 
                            if( api_rc != error_status_ok ) {   
                                msg_text( MSG_CONF_ERROR,
                                         t_msg, (void *)&i );   
                                fclose( fp_con );   
                                err_exec( LOG_EX_CONF, 0x00 ); 
                            }                       
                        }                           
                        break;                      
                    default:                        
                        msg_text( MSG_CONF_ERROR,   
                                 t_msg, (void *)&i );   
                        fclose( fp_con );           
                        err_exec( LOG_EX_CONF, 0x00 ); 
                        break;                      
                }                                   
            }                                       
            i++;                                    
        }                                           
        if ( feof( fp_con ) != 0 ) {                
            def_u_flg = def_g_flg = F_OFF;          
            j = i;                                  
            for( i=0 ; con_key[i].key[0] != (char)0xff ; i++ ) { 
                if( DCEUSER0_P<=i && i<=DCEUSER9_P ) { 
                    if( con_key[i].flg == F_ON )    
                        def_u_flg = F_ON;           
                } else if( DCEGROUP0_P<=i && i<=DCEGROUP9_P ) { 
                    if( con_key[i].flg == F_ON )    
                        def_g_flg = F_ON;           
                } else {                            
                    if( con_key[i].flg == F_OFF ) { 
                        strcpy( config[i], con_def[i].def ); 
                        con_key[i].flg = F_ON;      
                    }                               
                }                                   
            }                                       
            if( def_u_flg == F_OFF ||               
                def_g_flg == F_OFF ) {              
                msg_text( MSG_CONF_ERROR,           
                         t_msg, (void *)&j );       
                fclose( fp_con );                   
                err_exec( LOG_EX_CONF, 0x00 );      
            }                                       
        } else {                                    
            strcpy( err_call, "fgets" );            
            err_code = errno;                       
            msg_text( MSG_CONF_READ_ERR, t_msg, 0x00 ); 
            fclose( fp_con );                       
            err_exec( LOG_EX_CONF, 0x00 );          
        }                                           
    }                                               
    if( fclose( fp_con ) == EOF ) {                 
        strcpy( err_call, "fclose" );               
        err_code = errno;                           
        msg_text( MSG_CONF_READ_ERR, t_msg, 0x00 ); 
        err_exec( LOG_EX_CONF, 0x00 );              
    }                                               
    rc = logfile_put( LOG_MD_OK, LOG_EX_CONF,       
                  t_msg, 0x00 );                    
    if( rc != 0 ) {                                 
        printf("%s", t_msg );                       
        msg_text( MSG_END, t_msg, 0x00 );           
        printf("%s", t_msg );                       
        EXIT;                                       
    }                                               
}

/*****************************************************************************/
/*  ID   = tree_err()                                                        */
/*                                                                           */
/*  name = Error handling for the tree generation file                       */
/*  func = When a syntax error is detected in the tree generation file,      */
/*         displays a message, and terminates.                               */
/*                                                                           */
/*  i/o    = line:int  :INP: the line where an error occurred                */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 17, 1995  by Kentarou Chikura                            */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
void    tree_err( int line )
{
    msg_text( MSG_TREEF_ERROR, t_msg, (void *)&line );
    err_exec( LOG_EX_TREEF, 0x00 );                 
}

/*****************************************************************************/
/*  ID   = chk_right()                                                       */
/*                                                                           */
/*  name = Validation check for the trustee characters                       */
/*  func = Checks  the validity of the designated character string.          */
/*                                                                           */
/*  i/o    = chk_out:char *:OUT: area for storing the check-completed-       */
/*                               character-string                            */
/*           chk_in :char *:INP: area for storing the target character-      */
/*                               string to be checked                        */
/*           line   :int   :INP: the line where an error occurred            */
/*                                                                           */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 17, 1995  by Kentarou Chikura                            */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
void    chk_right( char *chk_out, char *chk_in, int line )
{
    int     l, i;                                   

    l = strlen( chk_in );                           
    if( l > 8 )                                     
        tree_err( line );                           
    if( chk_in[0] != '[' || chk_in[l-1] != ']' )    
        tree_err( line );                           
    for( i=1 ; chk_in[i] != ']' ; i++ ) {           
        if( ( strchr( RIGHT_LIST, (int)chk_in[i] ) ) 
                == NULL )                           
            tree_err( line );                       
        else                                        
            chk_out[i-1] = chk_in[i];               
    }                                               
}

/*****************************************************************************/
/*  ID   = tree_get()                                                        */
/*                                                                           */
/*  name = Inputting the tree generation file                                */
/*  func = Reads the tree generation file, and expands it on a table.        */
/*                                                                           */
/*  i/o    = none                                                            */
/*  return = none (exit() terminates the process if an error occurred)       */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 17, 1995  by Kentarou Chikura                            */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
void     tree_get( void )
{
    int     i, j;                                   
    int     rc;                                     
    char    lin_flg;                                
    int     lin_cnt;                                
    int     v_len;                                  
    char    snt_flg;                                
    char    *chk_p;                                 
    char    *chk_e;                                 
    struct  tm  time_tbl;                           
    time_t  mktm;                                   
    char    w_path[1025];                           

    strcpy( err_mcall, "tree_get" );                
    time( &timer );                                 
    memset( st_time, 0x00, 25 );                    
    memcpy( st_time, ctime(&timer), 24 );           
    if ( ( fp_tree = fopen( config[TREE_P], "r" ) ) 
             == NULL ) {                            
        strcpy( err_call, "fopen" );                
        err_code = errno;                           
        msg_text( MSG_TREEF_READ_ERR, t_msg, 0x00 );
        err_exec( LOG_EX_TREEF, 0x00 );             
    }                                               
    lin_cnt = 0;                                    
    lin_flg = LIN_1;                                
    while( fgets( lin, 2048, fp_tree ) != NULL ) {  
        lin_cnt++;                                  
        if( lin[0] != COMT_1 &&                     
            strncasecmp( lin, COMT_2, strlen( COMT_2 ) ) != 0 ) { 
            if( lin[0] != ' ' ) {                   
                tree_b = tree_w;                    
                tree_w = (struct tree_for *)malloc( 
                                  sizeof(struct tree_for) );
                if ( tree_w == NULL ) {             
                    strcpy( err_call, "malloc" );   
                    err_code = 0;                   
                    msg_text( MSG_MEM_OVER_TREE, t_msg, 0x00 ); 
                    fclose( fp_tree );              
                    err_exec( LOG_EX_TREEF, 0x00 ); 
                } else {                            
                    tree_flg = F_ON;                
                    memset( tree_w, 0x00,           
                            sizeof( struct tree_for ) ); 
                    T_W.owner_u = T_W.owner_g =     
                            -1;                     
                    T_W.other_f = T_W.mask_f = F_OFF; 
                    if( ( chk_p = strchr( lin, '\n' ) ) 
                         == NULL )                  
                        tree_err( lin_cnt );        
                    chk_p[0] = (char)0x00;          
                    if( ( chk_p = strtok( lin, " " ) ) 
                        == NULL )                   
                        tree_err( lin_cnt );        
                    if ( chk_count(lin,strlen(lin)) == -1 ) 
                        tree_err( lin_cnt );        
                    T_W.level = atoi( chk_p );      
                    if( T_W.level > DIR_LEVEL_MAX ) 
                        tree_err( lin_cnt );        
                    if( ( chk_p = strtok( NULL, " " ) ) 
                        == NULL )                   
                        tree_err( lin_cnt );        
                    if( strlen( chk_p ) > 255 )     
                        tree_err( lin_cnt );        
                    strcpy( T_W.path, chk_p );      
                    if( ( chk_p = strtok( NULL, " " ) ) 
                        == NULL )                   
                        tree_err( lin_cnt );        
                    if( strcasecmp( chk_p, DF_DIR_C ) == 0 ) 
                        T_W.type = DF_DIR;          
                    else if( strcasecmp( chk_p, DF_FIL_C ) == 0 ) 
                        T_W.type = DF_FIL;          
                    else                            
                        tree_err( lin_cnt );        
                    if( ( chk_p = strtok( NULL, "\0" ) ) 
                        != NULL ) {                 
                        if( strlen(chk_p) < 24 )    
                            tree_err( lin_cnt );    
                        strcpy( (char *)&(T_W.time), 
                                chk_p );            
                    }                               
                    if ( tree_top == TREE_CLR ) {   
                        if ( T_W.level != 1 )       
                            tree_err( lin_cnt );    
                        tree_top = tree_w;          
                        T_W.up = T_W.down =         
                        T_W.back = T_W.next =       
                            TREE_CLR;               
                    } else {                        
                        if( T_B.level < T_W.level ) {   
                            if(T_B.level<(T_W.level-1)) { 
                                tree_err( lin_cnt );
                            }                       
                            if( T_B.type == DF_FIL ) 
                                tree_err( lin_cnt );
                            T_W.up = tree_b;        
                            T_B.down = tree_w;      
                            T_W.down = T_W.back =   
                                T_W.next = TREE_CLR;    
                        } else                      
                        if( T_B.level == T_W.level ) {  
                            T_W.up = T_B.up;        
                            T_W.down = TREE_CLR;    
                            T_W.back = tree_b;      
                            T_B.next = tree_w;      
                            T_W.next = TREE_CLR;    
                        } else                      
                        if( T_B.level > T_W.level ) {   
                            while( T_B.level != T_W.level ) 
                                tree_b = T_B.up;    
                            T_W.up = T_B.up;        
                            T_W.down = TREE_CLR;    
                            T_W.back = tree_b;      
                            T_B.next = tree_w;      
                            T_W.next = TREE_CLR;    
                        }                           
                    }                               
                    tree_flg = F_OFF;               
                }                                   
                lin_flg = LIN_2;                    
            } else {                                
                if( lin_flg == LIN_1 )              
                    tree_err( lin_cnt );            
                if( ( chk_p = strchr( lin, '\n' ) ) 
                     == NULL )                      
                    tree_err( lin_cnt );            
                chk_p[0] = (char)0x00;              
                chk_p = lin;                        
                for( i=0 ; lin[i] == ' ' ; i++ );   
                if( ( chk_p = strtok( &(lin[i]), " " ) ) 
                    == NULL )                       
                    tree_err( lin_cnt );            
                while( chk_p[0] != (char)0x00 ) {   
                    if( ( chk_e = strchr( chk_p, '=' ) ) 
                         == NULL )                  
                        tree_err( lin_cnt );        
                    for( j=0 ;                      
                            strncasecmp( chk_p, dir_key[j].key, 
                                    strlen(dir_key[j].key) ) != 0 && 
                            dir_key[j].key[0] != (char)0xff  
                             ; j++ );               
                    if(  dir_key[j].key[0] == (char)0xff ) 
                        tree_err( lin_cnt );        
                    chk_p = chk_e + 1;              
                    if( chk_p[0] == (char)0x00 )    
                        tree_err( lin_cnt );        
                    switch( j ) {                   
                        case    T_LEN_P :           
                            if( T_W.len_f == F_ON ) 
                                tree_err( lin_cnt ); 
                            if ( T_W.type == DF_DIR )   
                                tree_err( lin_cnt ); 
                            if( chk_count( chk_p, strlen(chk_p) ) )
                                tree_err( lin_cnt ); 
                            T_W.len = (long)atoi(chk_p); 
                            if( T_W.len < FILE_MIX || 
                                T_W.len > FILE_MAX )  
                                tree_err( lin_cnt ); 
                            T_W.len_f = F_ON;       
                            break;                  
                        case    T_TYPE_P :          
                            if( T_W.bt_type_f == F_ON ) 
                                tree_err( lin_cnt ); 
                            if ( T_W.type == DF_DIR )   
                                tree_err( lin_cnt ); 
                            if( strcasecmp( chk_p, BT_BINARY_C ) == 0 ) 
                                T_W.bt_type = BT_BINARY; 
                            else if( strcasecmp(chk_p,BT_TEXT_C) == 0 ) 
                                T_W.bt_type = BT_TEXT;  
                            else                    
                                tree_err( lin_cnt ); 
                            T_W.bt_type_f = F_ON;   
                            break;                  
                        case    T_USER0_P  :        
                        case    T_USER1_P  :        
                        case    T_USER2_P  :        
                        case    T_USER3_P  :        
                        case    T_USER4_P  :        
                        case    T_USER5_P  :        
                        case    T_USER6_P  :        
                        case    T_USER7_P  :        
                        case    T_USER8_P  :        
                        case    T_USER9_P  :        
                            j -= T_USER0_P;         
                            if( T_W.user_f[j] == F_ON ) 
                                tree_err( lin_cnt ); 
                            if( con_key[j+DCEUSER0_P].flg 
                                     == F_OFF )     
                                tree_err( lin_cnt ); 
                            if( chk_p[0] == '*' ) { 
                                if( T_W.owner_u == -1 ) 
                                    T_W.owner_u = j;
                                else                
                                    tree_err( lin_cnt ); 
                                chk_p++;            
                            }                       
                            chk_right( T_W.user[j], 
                                    chk_p, lin_cnt );   
                            T_W.user_f[j] = F_ON;   
                            break;                  
                        case    T_GROUP0_P :        
                        case    T_GROUP1_P :        
                        case    T_GROUP2_P :        
                        case    T_GROUP3_P :        
                        case    T_GROUP4_P :        
                        case    T_GROUP5_P :        
                        case    T_GROUP6_P :        
                        case    T_GROUP7_P :        
                        case    T_GROUP8_P :        
                        case    T_GROUP9_P :        
                            j -= T_GROUP0_P;        
                            if( T_W.group_f[j] == F_ON ) 
                                tree_err( lin_cnt ); 
                            if( con_key[j+DCEGROUP0_P].flg 
                                     == F_OFF )     
                                tree_err( lin_cnt ); 
                            if( chk_p[0] == '*' ) { 
                                if( T_W.owner_g == -1 ) 
                                    T_W.owner_g = j;
                                else                
                                    tree_err( lin_cnt ); 
                                chk_p++;            
                            }                       
                            chk_right( T_W.group[j],
                                    chk_p, lin_cnt );   
                            T_W.group_f[j] = F_ON;  
                            break;                  
                        case    T_OTHER_P  :        
                            if( T_W.other_f == F_ON ) 
                                tree_err( lin_cnt ); 
                            chk_right( T_W.other,   
                                    chk_p, lin_cnt );   
                            T_W.other_f = F_ON;     
                            break;                  
                        case    T_MASK_P   :        
                            if( T_W.mask_f == F_ON ) 
                                tree_err( lin_cnt ); 
                            chk_right( T_W.mask,    
                                    chk_p, lin_cnt );   
                            T_W.mask_f = F_ON;      
                            break;                  
                        default:                    
                            tree_err( lin_cnt );    
                            break;                  
                    }                               
                    chk_p = strtok( NULL, " " );    
                }                                   
            }                                       
        }                                           
    }                                               
    if ( feof( fp_tree ) == 0 ) {                   
        strcpy( err_call, "fgets" );                
        err_code = errno;                           
        msg_text( MSG_TREEF_READ_ERR, t_msg, 0x00 );
        fclose( fp_tree );                          
        err_exec( LOG_EX_TREEF, 0x00 );             
    }                                               
    if( fclose( fp_tree ) == EOF ) {                
        strcpy( err_call, "fclose" );               
        err_code = errno;                           
        msg_text( MSG_TREEF_READ_ERR, t_msg, 0x00 ); 
        err_exec( LOG_EX_TREEF, 0x00 );             
    }                                               
    fp_tree = NULL;                                 
    rc = logfile_put( LOG_MD_OK, LOG_EX_TREEF,      
                  t_msg, 0x00 );                    
    if( rc != 0 ) {                                 
        printf("%s", t_msg );                       
        msg_text( MSG_END, t_msg, 0x00 );           
        printf("%s", t_msg );                       
        EXIT;                                       
    }                                               
}                                                   

/*****************************************************************************/
/*  ID   = tree_create()                                                     */
/*                                                                           */
/*  name = Making the tree generation file                                   */
/*  func = With using the inputted information of the tree generation file,  */
/*         makes a test tree under the designated directory (configuration   */
/*         file TOPDIR=item).                                                */
/*                                                                           */
/*  i/o    = none                                                            */
/*  return = none (exit() terminates the process if an error occurred)       */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 17, 1995  by Kentarou Chikura                            */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
void    tree_create( void )
{
    char    flg;                                    
    int     i,j;                                    
    int     rc;                                     
    error_status_t  api_rc;                         
    char    log_ex_flg;                             

    strcpy( err_mcall, "tree_create" );             
    msg_text( MSG_TREE_CREATE, t_msg, 0x00 );       
    printf("%s", t_msg );                           
    tree_w = tree_b = tree_top;                     
    if( chdir( config[TOPDIR_P] ) == -1 ) {         
        strcpy( err_call, "chdir" );                
        err_code = errno;                           
        msg_text( MSG_DIR_CREATE_ERR, t_msg,        
                     config[TOPDIR_P] );            
        err_exec( LOG_EX_DIRC, config[TOPDIR_P] );  
    }                                               
    while( tree_w != TREE_CLR ) {                   
        time( &timer );                             
        memset( st_time, 0x00, 25 );                
        memcpy( st_time, ctime(&timer), 24 );       
        if( T_B.level != T_W.level ) {              
            if( T_B.level < T_W.level ) {           
                if( chdir( T_B.path ) == -1 ) {     
                    strcpy( err_call, "chdir" );    
                    err_code = errno;               
                    if( T_W.type == DF_DIR )        
                        msg_text( MSG_DIR_CREATE_ERR, t_msg, T_W.path );
                    else                            
                        msg_text( MSG_FILE_CREATE_ERR, t_msg, T_W.path );
                    err_exec( LOG_EX_DIRC, T_W.path ); 
                }                                   
            } else {                                
                for( j=T_B.level ; j > T_W.level    
                                 ; j-- ) {          
                    if( chdir( ".." ) == -1 ) {     
                        strcpy( err_call, "chdir" );
                        err_code = errno;           
                        if( T_W.type == DF_DIR )    
                            msg_text(MSG_DIR_CREATE_ERR,t_msg,T_W.path);
                        else                        
                            msg_text(MSG_FILE_CREATE_ERR,t_msg,T_W.path);
                        err_exec( LOG_EX_DIRC, T_W.path ); 
                    }                               
                }                                   
            }                                       
        }                                           
        if( T_W.type == DF_DIR ) {                  
            msg_text( MSG_DIR_CREATE, t_msg, T_W.path );
            log_ex_flg = LOG_EX_DIRC;               
            rc = mkdir( T_W.path, S_IRUSR|S_IWUSR|S_IXUSR|  
                            S_IRGRP|S_IWGRP|S_IXGRP|    
                            S_IROTH|S_IWOTH|S_IXOTH );  
            if( rc == -1 ) {                        
                strcpy( err_call, "mkdir" );        
                err_code = errno;                   
                msg_text( MSG_DIR_CREATE_ERR, t_msg, T_W.path );
                err_exec( log_ex_flg, T_W.path );   
            }                                       
        } else {                                    
            msg_text( MSG_FILE_CREATE, t_msg, T_W.path ); 
            log_ex_flg = LOG_EX_FILEC;              
            if( ( w_fp = fopen( T_W.path, "w" ) ) == NULL ) {
                strcpy( err_call, "fopen" );        
                err_code = errno;                   
                msg_text(MSG_FILE_CREATE_ERR,t_msg,T_W.path); 
                err_exec( log_ex_flg, T_W.path );   
            }                                       
            if( T_W.bt_type == BT_BINARY ) {        
                j = (int)0x00;                      
            } else {                                
                j = (int)0x30;                      
            }                                       
            for( i=0 ; i < T_W.len ; i++ ) {        
                if( T_W.bt_type == BT_BINARY ) {    
                    if( j > (int)0xff )             
                        j = (int)0x00;              
                } else {                            
                    if( j > (int)0x39 )             
                        j = (int)0x30;              
                }                                   
                if( ( rc = fputc( j, w_fp ) ) == EOF ) { 
                    strcpy( err_call, "fputc" );    
                    err_code = errno;               
                    if( errno == EFBIG ||
                        errno == EDQUOT ||
                        errno == ENOSPC )
                        msg_text( MSG_FILE_SIZE_ERR,   
                                  t_msg,T_W.path);      
                    else
                        msg_text( MSG_FILE_WRITE_ERR,   
                                  t_msg,T_W.path);      
                    fclose( w_fp );                 
                    err_exec( log_ex_flg, T_W.path ); 
                }                                   
                j++;                                
            }                                       
            if( fclose( w_fp ) == EOF ) {           
                strcpy( err_call, "fclose" );       
                err_code = errno;                   
                msg_text(MSG_FILE_CREATE_ERR,t_msg,T_W.path); 
                err_exec( log_ex_flg, T_W.path );   
            }                                       
        }                                           
        if( T_W.owner_g != -1 ) {                   
            api_rc = acl_owner( T_W.path,           
                                config[T_W.owner_g+DCEGROUP0_P], 
                                FLAG_GROUP );       
            if( api_rc != error_status_ok ){        
                strcpy( err_call, "acl_owner" );    
                err_code = api_rc;                  
                if( T_W.type == DF_DIR )            
                    msg_text( MSG_DIR_OWNER_G_ERR, t_msg, T_W.path );
                else                                
                    msg_text( MSG_FIL_OWNER_G_ERR, t_msg, T_W.path );
                err_exec( log_ex_flg, T_W.path );   
            }                                       
        }                                           
        memset( (void*)&acl_w, 0x00,                
                 sizeof(struct acl_list)*23 ) ;     
        j = 0;                                      
        acl_w[j].next = ACL_CLR;                    
        acl_w[j].name = config[LOGINUSER_P];        
        acl_w[j].t_list = ALL_RIGHT;                
        acl_w[j].flag = FLAG_USER;                  
        j++;                                        
        for( i=0 ; i < 10 ; i++ ) {                 
            if( config[i+DCEUSER0_P][0] != 0x00 &&  
                T_W.user_f[i] == F_ON           &&  
                T_W.owner_u != i                ) { 
                acl_w[j].next = ACL_CLR;            
                acl_w[j].name = config[i+DCEUSER0_P];
                acl_w[j].t_list = T_W.user[i];      
                acl_w[j].flag = FLAG_USER;          
                acl_w[j-1].next = &acl_w[j];        
                j++;                                
            }                                       
            if( config[i+DCEGROUP0_P][0] != 0x00 && 
                          T_W.group_f[i] == F_ON ) { 
                acl_w[j].next = ACL_CLR;            
                acl_w[j].name = config[i+DCEGROUP0_P];
                acl_w[j].t_list = T_W.group[i];     
                acl_w[j].flag = FLAG_GROUP;         
                acl_w[j-1].next = &acl_w[j];        
                j++;                                
            }                                       
        }                                           
                                                    
        acl_w[j].next = ACL_CLR;                    
        acl_w[j].name = (char *)0x00;               
        if( T_W.other_f == F_OFF )                  
            strcpy( T_W.other, DEF_OTHER );         
        acl_w[j].t_list = T_W.other;                
        acl_w[j].flag = FLAG_OTHER;                 
        acl_w[j-1].next = &acl_w[j];                
        j++;                                        
                                                    
        acl_w[j].next = ACL_CLR;                    
        acl_w[j].name = (char *)0x00;               
        if( T_W.mask_f == F_OFF )                   
            strcpy( T_W.mask, DEF_MASK );           
        acl_w[j].t_list = T_W.mask;                 
        acl_w[j].flag = FLAG_MASK;                  
        acl_w[j-1].next = &acl_w[j];                
        api_rc = acl_set( T_W.path, &acl_w[0] );    
        if( api_rc != error_status_ok ){            
            strcpy( err_call, "acl_set" );          
            err_code = api_rc;                      
            if( T_W.type == DF_DIR )                
                msg_text( MSG_DIR_ACL_ERR, t_msg, T_W.path );
            else                                    
                msg_text( MSG_FILE_ACL_ERR, t_msg, T_W.path );
            err_exec( log_ex_flg, T_W.path );       
        }                                           
        printf("%s", t_msg );                       
        rc = logfile_put( LOG_MD_OK, log_ex_flg,    
                 t_msg, T_W.path );                 
        if( rc != 0 ) {                             
            printf("%s", t_msg );                   
            msg_text( MSG_END, t_msg, 0x00 );       
            printf("%s", t_msg );                   
            EXIT;                                   
        }                                           
        tree_b = tree_w;                            
        if( T_W.down != TREE_CLR ) {                
            tree_w = T_W.down;                      
        } else if( T_W.next != TREE_CLR ) {         
            tree_w = T_W.next;                      
        } else if( T_W.up != TREE_CLR ) {           
            for( tree_w = T_W.up ; tree_w != TREE_CLR && 
                                 T_W.next == TREE_CLR ; 
                                 tree_w = T_W.up ); 
            if( tree_w != TREE_CLR )                
                tree_w = T_W.next;                  
        } else {                                    
            tree_w = TREE_CLR;                      
        }                                           
    }                                               
    tree_free();                                    
    if( err_flg == F_ON ) {                         
        msg_text( MSG_END, t_msg, 0x00 );           
        printf("%s", t_msg );                       
        EXIT;                                       
    }                                               
    msg_text( MSG_TREE_CREATE_END, t_msg, 0x00 );   
    printf("%s", t_msg );                           
}
/*****************************************************************************/
/*  ID   = main()                                                            */
/*                                                                           */
/*  name = Test suite for Agent (main routine)                               */
/*  func = Invokes appropriate functions.                                    */
/*                                                                           */
/*  i/o    = argc:int    :INP: the number of the parameters                  */
/*           argv:char **:INP: the parameters (character strings)            */
/*                                                                           */
/*  return =  0: normally ended                                              */
/*           -1: forcedly terminated                                         */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 17, 1995  by Kentarou Chikura                            */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
int main(   int argc,   char **argv  )
{
    int     i, j;                                   
    int     rc;                                     
    error_status_t  api_rc;                         

    fp = fp_con = fp_tree = w_fp = NULL;            
    memset( config[0], 0x00, CONFIG_LEN*CONFIG_CONT ); 
    login_flg = F_OFF;                              
    for( i=0; con_key[i].key[0] != '\xff' ; i++ )   
        con_key[i].flg = F_OFF;                     
    log_flg = F_OFF;                                
    strcpy( err_mcall, "main" );                    
    tree_flg = F_OFF;                               
    tree_top = tree_w = tree_b = TREE_CLR;          

    conf_get();                                     
    strcpy( err_mcall, "main" );                    
    time( &timer );                                 
    memset( st_time, 0x00, 25 );                    
    memcpy( st_time, ctime(&timer), 24 );           
    api_rc = dce_login( config[LOGINUSER_P],        
                        config[LOGINPASSWORD_P] );  
    if ( api_rc != error_status_ok ) {              
        strcpy( err_call, "dce_login" );            
        err_code = api_rc;                          
        msg_text( MSG_LOGIN_ERROR, t_msg, 0x00 );   
        printf("%s", t_msg );                       
        rc = logfile_put( LOG_MD_NG, LOG_EX_LOGIN,  
                    t_msg, 0x00 );                  
        msg_text( MSG_END, t_msg, 0x00 );           
        printf("%s", t_msg );                       
        EXIT;                                       
    }                                               
    login_flg = F_ON;                               
    err_flg = F_OFF;                                
    rc = logfile_put( LOG_MD_OK, LOG_EX_LOGIN,      
                  t_msg, 0x00 );                    
    if( rc != 0 ) {                                 
        printf("%s", t_msg );                       
        err_flg = F_ON;                             
        EXIT;                                       
    }                                               
    tree_get();                                     
    tree_create();                                  
    EXIT;                                           
}
