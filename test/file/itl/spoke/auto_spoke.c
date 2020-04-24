/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * HISTORY
 */

/* auto_spoke.c
 * This file contains functions for automatic spoke execution from HUB.
 * A configure file needs to be set up, which contains all information
 * to initiate spokes, one line in the file describes one spoke. The
 * HUB will inite spokes according to the infor in the file.
 *
 * The format of a line in configure file is:
*<machine-name><binary-path><work-dir><pric-name passwd or "" ""><logfile or ""> 
 * A blank line is allowed and also a line begins with # is a tradition.
 * When the log file is "" then there will be no xterm involved, otherwise
 * windows will be open for spokes.
 * Be sure you have /etc/hosts.equiv set up properly for spokes and HUB.
 */
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dce/rpc.h>
#include <dce/sec_login.h>

#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/defs.h>
#include <utils/itl/itl_spoke.h>
#include <sys/types.h>
#include <dcedfs/osi_net.h>
#include <arpa/inet.h>

typedef struct spoke {
  char *spokeName;
  char *defaultSpokeExec;
  char *workPath;
  char *princName;
  char *princPassWord;
  char *outputFile;
  struct spoke *next;
} spoke;

spoke *spokeListp = NULL;
int autoSpoke=0;
int  numSpokes = 0;

PRIVATE int spoke_StartSpoke _TAKES((
				     IN spoke *spokeP,
				     IN char *listenName,
				     IN int n
				     ));

#define MAX_NUMBER_OF_SPOKES 300
#define MAX_CHARS_IN_LINE 512

#define STDERR_FILENO 2
#define STDOUT_FILENO 1
#define STDIN_FILENO 0


/*  itl_dce_login, when the Principal and PrincipalPasswd calling 
 *  args are supplied to main().  The process will self authenticate 
 *  itself to dce. 
 *  WARNING: when running dfs this trashes your afs pag and you are
 *  no longer authentocated to AFS.
 */
EXPORT int itl_dce_login(princName, princPasswd)
  IN char *princName;
  IN char *princPasswd;
{

    unsigned_char_p_t principal_name; 
    sec_passwd_rec_t passwd;
    sec_login_handle_t login_context;
    boolean32 reset_passwd;
    sec_login_auth_src_t auth_src;
    error_status_t st;

    passwd.version_number = 0;
    passwd.pepper = NULL;
    passwd.key.key_type = 1;

    ITL_ALLOC(passwd.key.tagged_union.plain, 
        unsigned_char_p_t, strlen(princPasswd)+1);
    strcpy((char *)passwd.key.tagged_union.plain,princPasswd);

    ITL_ALLOC(principal_name, unsigned_char_p_t, strlen(princName)+1);
    strcpy((char *)principal_name,princName);

    if(!(sec_login_setup_identity(principal_name,
				   sec_login_no_flags,
 				   &login_context,
				   &st)))
    {
	printf("sec_login_setup_identity returned FALSE");
	DCE_ERROR(st,"sec_login_setup_identity");
	return(-1);
    }
    if (!(sec_login_validate_identity(login_context
				      ,&passwd
				      ,&reset_passwd
				      ,&auth_src,&st)))
    {
	printf("sec_login_validate_identity returned FALSE");
	DCE_ERROR(st,"sec_login_validate_identity");
	return(-1);
    }

    sec_login_set_context(login_context,&st);
    if (st != rpc_s_ok)
    {
	DCE_ERROR(st,"sec_login_set_context");
        printf("Exiting because I can't set login context\n");
	return(-1);
    }

    ITL_FREE(passwd.key.tagged_union.plain);
    ITL_FREE(principal_name);
    return(0);
}

PRIVATE void SpokeFree( spokeP )
  spoke *spokeP;
{
  if ( spokeP ) {
    if ( spokeP->next ) SpokeFree(spokeP->next);
    if ( spokeP->spokeName ) ITL_FREE(spokeP->spokeName);
    if ( spokeP->defaultSpokeExec ) ITL_FREE(spokeP->defaultSpokeExec);
    if ( spokeP->workPath ) ITL_FREE(spokeP->workPath);
    if ( spokeP->princName ) ITL_FREE(spokeP->princName);
    if ( spokeP->princPassWord ) ITL_FREE(spokeP->princPassWord);
    if ( spokeP->outputFile ) ITL_FREE(spokeP->outputFile);
    ITL_FREE(spokeP);
  }
}

/*
 * GetWord( str, wordP, nextWordP)
 * This function will get a word form the str pointer.
 * and returns the next position of the word.
 * 
 */
void GetWord(str, wordP, nextWordP )
  char *str, **wordP, **nextWordP;
{
  *wordP = *nextWordP = NULL;
  if ( str == NULL || *str == '\0' || *str ==  '\n' ) {
    return;
  }
  while( isspace(*str) ) str++;
  *wordP = str;

  if ( *str == '"' ) {
    str++;
    while( *str != '"') str++;
    *wordP = str;
  }

  while ( !isspace(*str) ) str++;
  if( *str == '\n' || *str == '\0') {
     *nextWordP = NULL;
     *(str) = '\0';
     return;
   }
   *(str) = '\0';
   str++;
   while ( isspace(*str) ) str++;
   *nextWordP = str;
   return;
}

/*
 * GetSpoke(buf)
 * This function will get a struct spoke filled up by reading the input buffer.
 *
 */
PRIVATE spoke * GetSpoke( buf)
  char *buf;
{
  char *str;
  char *nextWordp;
  spoke *ptr;
  
  if ( !buf ) return NULL;
  ITL_ALLOC(ptr, spoke *, sizeof(spoke));

  nextWordp = buf;
  GetWord(nextWordp, &str, &nextWordp);
  if ( *str != '"' ) {
    ITL_ALLOC(ptr->spokeName, char *, strlen(str)+1);
    strcpy(ptr->spokeName, str);
  }
  else 
    ptr->spokeName=NULL;
  GetWord(nextWordp, &str, &nextWordp);
  if ( !str ) {
    SpokeFree(ptr);
    return NULL;
  }
  else {
    ITL_ALLOC(ptr->defaultSpokeExec, char *, strlen(str)+1);
    strcpy(ptr->defaultSpokeExec, str);
  }
  GetWord(nextWordp, &str, &nextWordp);
  if ( !str ) {
    SpokeFree(ptr);
    return NULL;
  }
  else {
    ITL_ALLOC(ptr->workPath, char *, strlen(str)+1);
    strcpy(ptr->workPath, str);
  }
  GetWord(nextWordp, &str, &nextWordp);
  if ( *str != '"' ) {
    ITL_ALLOC(ptr->princName, char *, strlen(str)+1);
    strcpy(ptr->princName, str);
  }
  else 
    ptr->princName = NULL;
  GetWord(nextWordp, &str, &nextWordp);
  if ( *str != '"' ) {
    ITL_ALLOC(ptr->princPassWord, char *, strlen(str)+1);
    strcpy(ptr->princPassWord, str);
  }
  else 
    ptr->princPassWord = NULL;
  GetWord(nextWordp, &str, &nextWordp);
  if ( *str != '"' ) {
    ITL_ALLOC(ptr->outputFile, char *, strlen(str)+1);
    strcpy(ptr->outputFile, str);
  }
  else 
    ptr->outputFile = NULL;
  return ptr;
}
  

/* 
 *   spoke_AutoSpoke( fileName, listenName, syncFlag ) --
 *
 *   syncFlag is a indicator if -I and -C are used in same command line,
 *   if it is true then it is neccessary to wait for the complete setup
 *   of all spokes and hub and then run the script.
 *
 *   This function is called when the user provides a config file
 *   with -C option. It will set up remote spokes according to
 *   the content of the configure file.
 *
 *   Returns -1 if anything gos wron.
 *   Return  0 if all spokes requested were started
 *   Returns +number if some spokes were not forked.
 *   NOTE:  if fork fails, This may be caused my exceeding the max
 *          number of processes.
 */
EXPORT int spoke_AutoSpoke(fileName, listenName)
  IN char *fileName;
  IN char *listenName;
{
    spoke *spokeP, *tmpP;
    char lineBuf[MAX_CHARS_IN_LINE];
    int fails;
    FILE *fp;
    int pid;
    int n,i;
    char *machinesInCell, *machine, *nextMachine;
    char buffer[MAX_CHARS_IN_LINE];

    if ( !fileName ) return -1;
    if ( (fp = fopen(fileName, "r") )== NULL  ) {
      itl_Error("Can not open the config file %s\n", fileName);
      return -1;
    }
    autoSpoke = TRUE;
    while( fgets(lineBuf, MAX_CHARS_IN_LINE, fp) != NULL ) {
      if ( lineBuf[0] == '\0' || lineBuf[0] == '\n' || lineBuf[0] == '#' || strlen(lineBuf) == 0 ) continue;

      spokeP = GetSpoke(lineBuf);
      if ( spokeP == NULL ) { 
        itl_Error("Check spoke configure file for syntax\n");
        return -1; 
      }
      if ( spokeListp == NULL ) {
        spokeListp = spokeP;
        spokeP->next = NULL;
        tmpP=spokeP;
      }
      else {
        spokeP->next = NULL;
        tmpP->next = spokeP;
        tmpP = spokeP;
      }
      numSpokes++;
    }

    fclose(fp);
    if ( numSpokes == 0 ) {
      itl_Error("No spoke is found in the config file %s\n", fileName);
      return -1;
    }
    if ( numSpokes >= MAX_NUMBER_OF_SPOKES ) {
      itl_Error("Too many spokes\n");
      return -1;
    }

    fails = 0;
    for(n=0, tmpP = spokeListp; tmpP ;n++, tmpP=tmpP->next ) 
    {

      if ( tmpP->spokeName == NULL ) {
        machinesInCell =  getenv("AUTO_SPOKE_MACHINES");
        if ( machinesInCell == NULL ) {
          itl_Error(
   "No machine name found, check setup file or env for AUTO_SPOKE_MACHINES\n");
          exit(1);
        }
      
        nextMachine = machine = &buffer[0]; 
        strcpy(nextMachine, machinesInCell);
        nextMachine[strlen(nextMachine)+1]='\0';

        i = 0;
        while(*nextMachine != '\0' ) {
          while ( isspace(*nextMachine)) nextMachine++;
          while ( !isspace(*nextMachine) 
            && (*nextMachine != '\0' || *nextMachine != '\0' )) nextMachine++;
            i++;
        }

        i = ( n+1 <= i )? n+1 : (( n  % i) + 1 );

        nextMachine = machine = &buffer[0]; 
        while(machine) {
          GetWord(nextMachine, &machine, &nextMachine);
          i--;
          if ( !i ) break;
        }
        
        if ( machine ) {
          ITL_ALLOC(tmpP->spokeName, char *, strlen(machine)+1);
          strcpy(tmpP->spokeName, machine);
        }
        else  {
           itl_Error("Error to get machine name\n");
           exit(1);
        }

      }
      pid = fork();
      if ( pid ==  -1) {
	    itl_Error("ERROR from fork\n");
            fails++;
      }	
      else if ( pid == 0)  /* if its a child */
	    spoke_StartSpoke( tmpP, listenName,n );
    }
    return(fails);
}


int spoke_findSpokeInArray(name,index)
  char *name;
  int index;
{
  char var_buffer[1024],*ptr,*str;
  char *ipaddrInString, char_buffer[1024];
  struct hostent *hp;
  struct sockaddr_in sin;

  int i;

  if (!(hp = gethostbyname(name))) {
        return numSpokes;
  }
  memcpy(&sin.sin_addr, hp->h_addr, sizeof(sin.sin_addr));
  for ( i = index; i < numSpokes ; i++ ) {
    sprintf((char *)var_buffer,"ITL_spoke[%d]", i);
    ptr = itl_GetStringVariable((char *)var_buffer);

    if ( ptr ) {
      strcpy(char_buffer,ptr);
      str = strchr(char_buffer,'[');
      *str = '\0';
      ipaddrInString = (char *)(strchr(char_buffer,':')) + 1;
    }
    else return numSpokes;
    if ( strcmp(ipaddrInString, inet_ntoa(sin.sin_addr)) == 0 ) {
          free(ptr);
          break; 
    }
    free(ptr);
  }
  return i;
}


/* 
 * This routine does the oerdering according to the sequence of spoke
 * structure so that the order of spokes in the spoke setup file is the
 * order of spoke ip address array (ITL_spoke[i]).
 */
spoke_spokeOrdering()
{
  char *str, *ptr, *ptr1, *ipaddrInString;
  spoke *tempP;
  char var_buffer[1024], char_buffer[1024];
  struct hostent *hp;
  struct sockaddr_in sin;
  int i,j;

  if ( autoSpoke ) {
    for (i = 0, tempP = spokeListp; i < numSpokes ; i++,tempP = tempP->next ) {
      sprintf((char *)var_buffer,"ITL_spoke[%d]", i);
      ptr = itl_GetStringVariable((char *)var_buffer);

      if ( ptr ) {
        strcpy(char_buffer,ptr);
        str = strchr(char_buffer,'[');
        *str = '\0';
        ipaddrInString = (strchr(char_buffer,':')) + 1;
      }
      if ( tempP->spokeName ) {
        if (!(hp = gethostbyname(tempP->spokeName))) {
                exit(1);
        }
        memcpy(&sin.sin_addr, hp->h_addr, sizeof(sin.sin_addr));
        if ( strcmp(ipaddrInString, inet_ntoa(sin.sin_addr)) == 0 ) {
          free(ptr);
          continue; 
        }
        else {
          j=spoke_findSpokeInArray(tempP->spokeName, i+1);
          if ( j != numSpokes ) {
            sprintf((char *)var_buffer,"ITL_spoke[%d]", j); 
            ptr1 = itl_GetStringVariable((char *)var_buffer);
            itl_SetStrVariable((char *)var_buffer, ptr, var_global);
            sprintf((char *)var_buffer,"ITL_spoke[%d]", i); 
            itl_SetStrVariable((char *)var_buffer, ptr1, var_global);
          }
        }
        free(ptr);
        free(ptr1);
      }
    }
  }
}


/* 
 * We need sleep for a few seconds so that all spokees get chance to 
 * register themself to the HUB. Here is the best point to wait.
 */
EXPORT void spoke_syncHubSpoke()
{
    struct timespec t;
    
    t.tv_sec = 2;
    t.tv_nsec = 0;

    for (;;) {
      if ( nspokes == numSpokes ) break;
      pthread_delay_np(&t);
    }
    spoke_spokeOrdering();
}

/*
 * spoke_RedirectOutput,  This is called when main() is invoked with
 * the output to log file option. 
 */
EXPORT int spoke_RedirectOutput(fileName)
  IN char *fileName;
{
    int file_handle;

    file_handle = open(fileName, O_WRONLY| O_CREAT| O_TRUNC, 0744);
    if(file_handle == -1)
    {	
        perror("ERROR opening Output File:");
        return(-1);
    }
    close(STDERR_FILENO);
    close(STDOUT_FILENO);
    dup2(file_handle,STDERR_FILENO);
    dup2(file_handle,STDOUT_FILENO);
    return(0);
}

/* 
 * int spoke_StartSpoke()
 * spoke_StartSpoke only childern execute this code.  This function
 * will rsh a new spoke copy of the test program.  This should not 
 * return until spoke dies or exits.
 * 
 * xterm is always used to the spoke.
*/
PRIVATE int spoke_StartSpoke(spokeP, listenName,n) 
    spoke *spokeP;
    char *listenName;
    int n;
{
    char commandLine[4*MAX_CHARS_IN_LINE];
    int ret;

    if ( spokeP->outputFile ) 
      sprintf(commandLine, "rsh %s ", spokeP->spokeName);
    else
      sprintf(commandLine, "xterm -e rsh %s ", spokeP->spokeName);

    sprintf(commandLine, "%s 'setenv RPC_SUPPORTED_PROTSEQS ncadg_ip_udp;", 
            commandLine);
    sprintf(commandLine, "%s cd %s;%s ",commandLine, 
            spokeP->workPath,spokeP->defaultSpokeExec);
    sprintf(commandLine,"%s -c %s ",commandLine, listenName);

    if ( spokeP->princName && spokeP->princPassWord ) 
      sprintf(commandLine,"%s -P %s %s ", commandLine, 
            spokeP->princName, spokeP->princPassWord);
    if ( getDisplayLines() ) {
      sprintf(commandLine,"%s -D ",commandLine);
    }
    if ( spokeP->outputFile ) {
      sprintf(commandLine,"%s -O %s'",commandLine, spokeP->outputFile);
    }
    else 
      sprintf(commandLine,"%s'",commandLine);
    ret = system(commandLine);
    if ( ret != 0 ) 
      printf("The %dth spoke at machine %s return Error: %d\n",n,spokeP->spokeName,ret);
    printf("Child %d exits\n",n);
    exit(0);
}

EXPORT void auto_spokeCleanup(nspoke)
  IN int nspoke;
{
   
    char var_buffer[MAX_CHARS_IN_LINE];
    char *ptr;
    handle_t h; 
    int i;

    if ( autoSpoke ) {
      for ( i = 0; i < nspoke ; i++ ) {
        sprintf((char *)var_buffer,"ITL_spoke[%d]", i);
        ptr = itl_GetStringVariable((char *)var_buffer);
        if ( ptr ) {
          if ( h = itlSpoke_GetHandleByString(ptr)) 
            spoke_remote_exit(h);
        }
        else break;
      }
   } 
}
