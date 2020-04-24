/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
static char *RCSID_agt_sdk_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/setdfakey/agt_sdk.c,v 1.1.2.3 1996/03/22 05:36:35 takano Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_sdk.c,v $
 * Revision 1.1.2.3  1996/03/22  05:36:35  takano
 * 	Modified the English messages.
 * 	[1996/03/22  05:36:14  takano]
 *
 * Revision 1.1.2.2  1996/03/09  20:44:30  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:20  marty]
 * 
 * Revision 1.1.2.1  1996/02/16  04:57:04  dce-sdc
 * 	First Submit
 * 	[1996/02/16  04:56:33  dce-sdc]
 * 
 * Revision 1.1.2.12  96/01/12  06:57:13  06:57:13  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  06:55:48  kishi]
 * 
 * Revision 1.1.2.11  1995/10/25  12:14:19  kishi
 * 	Change Copyright.
 * 	[1995/10/25  12:14:06  kishi]
 * 
 * Revision 1.1.2.10  1995/10/16  10:07:48  takano
 * 	Change ServerName to lower( option ).
 * 	[1995/10/16  10:07:31  takano]
 * 
 * Revision 1.1.2.9  1995/10/16  09:49:24  takano
 * 	Change ServerName to lower.
 * 	[1995/10/16  09:49:05  takano]
 * 
 * Revision 1.1.2.8  1995/10/13  09:21:32  kishi
 * 	Add header comment.
 * 	[1995/10/13  09:21:21  kishi]
 * 
 * Revision 1.1.2.7  1995/10/13  08:02:34  takano
 * 	Change ServerName Comparison.
 * 	[1995/10/13  08:02:07  takano]
 * 
 * Revision 1.1.2.6  1995/10/06  06:23:18  takano
 * 	Delete Merge Comments
 * 	[1995/10/06  06:22:58  takano]
 * 
 * Revision 1.1.2.5  1995/10/06  06:14:32  takano
 * 	Merged with changes from 1.1.2.4
 * 	[1995/10/06  06:14:29  takano]
 * 
 * 	Change strcmp -> strcasecmp
 * 	[1995/10/06  06:13:35  takano]
 * 
 * Revision 1.1.2.4  1995/09/19  09:16:35  kishi
 * 	Delete merge log.
 * 	[1995/09/19  09:16:17  kishi]
 * 
 * Revision 1.1.2.3  1995/09/19  07:43:21  takano
 * 	Merged with changes from 1.1.2.2
 * 	[1995/09/19  07:43:18  takano]
 * 
 * 	Delete #ifdef
 * 	[1995/09/19  07:42:37  takano]
 * 
 * Revision 1.1.2.2  1995/09/19  07:34:51  kishi
 * 	no call multi.
 * 	[1995/09/19  07:32:46  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:55:11  kishi
 * 	Initial revision
 * 	[1995/09/09  07:54:22  kishi]
 * 
 * $EndLog$
 */

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<pwd.h>
#include<errno.h>
#include<malloc.h>
#include<sys/ioctl.h>
#include<termio.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<netdb.h>
#include<locale.h>
#include<nl_types.h>
#include <signal.h>
#include <sys/types.h>

#include"agt_sdk.h"
#include"agt_msg.h"
#include"agt_login.h"

static void 	turn_off_echo(void);
static void 	turn_on_echo(void);
static void 	chkey(char *name);
static void	deletekey(char *name);
static void	gwlist(void);
static void	mkf_close(int fd);
static int	id_check_and_mkf_open(void);
static void 	set_dfam_id(char *arg);
static int	agt_up_check();
static int 	gwname_check();
static int 	get_cmd_and_dfam_id(char *opt);
static int 	getline(int fd, char *ptr, int maxlen);
static int 	put_master_key(char *gateway, char *key, int flag);
static void agt__sdk_sig_init(void);
static void turn_on_echo_exit(void);

char	dfam_id;
nl_catd sdk_catd;

/*****************************************************************************/
/* ID	= main								     */
/*                                                                           */
/* name = Main function for handling the master key                          */
/* func = Checks the options for the setdfakey command, and executes one of  */
/*        the following operations:                                          */ 
/*          - add a key to the master key file                               */
/*          - delete a key from the master key file                          */
/*          - display the keys in the master key file                        */
/*                                                                           */
/* i/o  = argc :int     :INP: number of parameters for the setdfakey command */
/*	  argv :char ** :INP: information of the setdfakey command           */
/*                                                                           */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
main(argc, argv)
int argc;
char **argv;
{
	int 	rc;
	int 	cmd;
	char 	*argptr;
	int     cmd_counter;

	if(!setlocale(LC_ALL, "")){
		fprintf(stderr, KDDS01309, strerror(errno));
		putenv("LANG=");
	}
	else{
		sdk_catd = catopen(D_CAT_SDKFILE, 0);
	}

	cmd_counter = 0;
	dfam_id = '\0';
	argv++;


	cmd = SDK_ADD;
	argptr = (char *)NULL;


	agt__sdk_sig_init();


	while(*argv != NULL){
		rc = get_cmd_and_dfam_id(*argv);
		switch(rc){
		case SDK_ADD:
			argptr = *++argv;
			cmd = SDK_ADD;
			cmd_counter ++;
			break;
		case SDK_DELETE:
			argptr = *++argv;
			cmd_counter ++;
			cmd = SDK_DELETE;
			break;
		case SDK_LIST:
			cmd = SDK_LIST;
			cmd_counter ++;
			break;
		case DFAM_ID:
			set_dfam_id(*++argv);
			break;
		default:
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01310, KDDS01310));
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG20003, KDDS20003));
			exit(0);
		}
		argv++;
	}


	if(cmd_counter > 1)
	        cmd = NOT_CMD;


	if(agt_up_check()){
		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01307, KDDS01307));
		exit(0);
	}


	switch(cmd){
	case SDK_ADD:
		chkey(argptr);
		break;
	case SDK_DELETE:
		deletekey(argptr);
		break;
	case SDK_LIST:
		gwlist();
		break;
	default:
		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01310, KDDS01310));
		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG20003, KDDS20003));
		break;
	}
	exit(0);
}

/*****************************************************************************/
/* ID	= set_dfam_id							     */
/*                                                                           */
/* name = Extraction of the identifier information                           */
/* func = Extracts the identifier information when the setdfakey command is  */
/*        executed with an identifier.                                       */
/*                                                                           */
/* i/o  = id: char * :INP: information of the identifier                     */
/*                                                                           */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static void set_dfam_id(char *id)
{
	if(id == NULL){
		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01310, KDDS01310));
		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG20003, KDDS20003));
		exit(-1);
	}
	else{
		if((strlen(id) == 1) && (isalnum(*id))){
			dfam_id = *id;
		}
		else{
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01310, KDDS01310));
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG20003, KDDS20003));
			exit(-1);
		}
	}
}

/*****************************************************************************/
/* ID	= get_cmd_and_dfam_id						     */
/*                                                                           */
/* name = Analyzing the command line options                                 */
/* func = Finds out the corresponding option to "opt."                       */
/*                                                                           */
/* i/o  = opt: char * :INP: command line parameter information               */
/*                                                                           */
/* return = SDK_ADD   : adding a key                                         */
/*          SDK_DELETE: deleting a key                                       */
/*          SDK_LIST  : displaying the key list                              */
/*          DFAM_ID   : identifier                                           */
/*          NOT_CMD   : invalid option is detected                           */
/*                                                                           */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static int get_cmd_and_dfam_id(char *opt)
{
	int i;
	static struct {
		char *arg;
		int cmd;
	} cmd_tbl[] = {
		{"-a", SDK_ADD},
		{"-d", SDK_DELETE},
		{"-l", SDK_LIST},
		{"-dfaID", DFAM_ID},
		{"", NOT_CMD}
	};
	for(i = 0; cmd_tbl[i].cmd != NOT_CMD; i++){
		if(strcmp(cmd_tbl[i].arg, opt) == 0){
			return(cmd_tbl[i].cmd);
		}
	}
	return(NOT_CMD);
}

/*****************************************************************************/
/* ID	= agt_up_check							     */
/*                                                                           */
/* name = Checking the Agent status                                          */
/* func = Checks if Agent has started.                                       */
/*                                                                           */
/* i/o  = none                                                               */
/*                                                                           */
/* return =  0: Agent not started yet                                        */
/*          -1: Agent already started                                        */
/*                                                                           */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static int agt_up_check()
{
	struct 	sembuf sops;
	char	ipcpath[64];
	int 	semid;
	int 	rc;
	key_t	key;

	sprintf(ipcpath, "%s%c", D_DFLT_IPCPATH, dfam_id);

	key = ftok(ipcpath, D_ID_SEM);
	if(key == -1)
		return(0);

	semid = semget(key, D_NUM_SEM, D_IPC_ACCESS_MODE | IPC_EXCL);
	if(semid == -1){
#ifdef _DEBUG
		printf("semget = %d\n", errno);
#endif
		switch(errno){
		case ENOENT:

			return(0);
		default:
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}
	}
	sops.sem_num = 0;
	sops.sem_op = 0;
	sops.sem_flg = IPC_NOWAIT;

retry_semop:
	if((semop(semid, &sops, 1)) == -1){
#ifdef _DEBUG
		printf("semop = %d\n",errno);
#endif
		switch(errno){
		case EINTR:
			goto retry_semop;
		case EAGAIN:
			return(-1);
		default:
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}
	}
	return(0);
}

/*****************************************************************************/
/* ID 	= chkey								     */
/*                                                                           */
/* name = Registration of a key with the master key file                     */
/* func = Registers a key designated by the setdfakey command with the       */
/*        master key file.                                                   */
/*                                                                           */
/* i/o  = gateway: char *:INP: Gateway name                                  */
/*                                                                           */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static void chkey(char *gateway)
{
	char		key1[D_INPUTBUF_SIZE];
	char		key2[D_INPUTBUF_SIZE];
	char		buf[D_INPUTBUF_SIZE];
	int		fd;
	int		rc;
	char 		c;
	int 		keylen;
	int 		srvnamelen;
	int		retry = 0;
	unsigned long 	mkey[2];
	int		i;


	fd = id_check_and_mkf_open();

	mkf_close(fd);


	while(gateway == (char *)NULL){
		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG10003, KDDS10003));
		if((srvnamelen = getline(0, buf, D_INPUTBUF_SIZE)) > 0){
			gateway = buf;
		}
	}

	for(i = 0; i < strlen(gateway); i++){
		gateway[i] = tolower(gateway[i]);
	}

	if(gwname_check(gateway)){
		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01306, KDDS01306), gateway);
		exit(0);
	}


	turn_off_echo();


	fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG10000, KDDS10000));
	while(1){

		while(1){
			memset(key1, '\0', D_INPUTBUF_SIZE);
			keylen = getline(0, key1, D_INPUTBUF_SIZE);
			if(keylen >= 1)
				break;
			fprintf(stderr, "\n");
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG10000, KDDS10000));
		}

		fprintf(stderr, "\n");
		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG10001, KDDS10001));


		while(1){
			memset(key2, '\0', D_INPUTBUF_SIZE);
			keylen = getline(0, key2, D_INPUTBUF_SIZE);
			if(keylen >= 1)
				break;
			fprintf(stderr, "\n");
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG10001, KDDS10001));
		}
		fprintf(stderr, "\n");


		if(strncmp(key1, key2, D_KEY_SIZE)){
			if(retry){
			        fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01304, KDDS01304));
				turn_on_echo();
				exit(0);
			}
			else{
			        fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG10002, KDDS10002));
				retry = 1;
			}
		}
		else
			break;
	}


	turn_on_echo();

	memcpy((char *)mkey, key1, D_KEY_SIZE);

	rc = put_master_key(gateway, key1, D_ADDKEY_OFF);
	switch(rc){
	case 0:
		exit(0);
	case 1:

		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG10004, KDDS10004), gateway);
		while((c = getc(stdin)) == EOF){
			switch(errno){
			case EINTR :
				continue;
			default :
				fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
				abort();
			}
		}
		if(c == 'y')
			put_master_key(gateway, key1, D_ADDKEY_ON);


		exit(0);
	case -1:
		exit(-1);
	default:
		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
		abort();

	}
}

/*****************************************************************************/
/* ID	= deletekey							     */
/*                                                                           */
/* name = Deletion of a key from the master key file                         */
/* func = Deletes key information (that corresponds to the Gateway           */
/*        designated by the setdfakey command) from the master key file.     */
/*                                                                           */
/* i/o  = gateway: char *:INP: Gateway name                                  */
/*                                                                           */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static void deletekey(char *gateway)
{
	int	 	fd;
	int	 	nfd;
	int	 	n;
	int	 	flg;
	char 		linebuf[D_LINE_MAX];
	char 		gwbuf[D_LINE_MAX];
	char 		mkf[D_LINE_MAX];

	fd = id_check_and_mkf_open();

	while((nfd = open(D_TMP_FILE, O_RDWR | O_CREAT | O_TRUNC, 600)) < 0){
		switch(errno){
			case EINTR:
				continue;
			case ENFILE:
			case ENOENT:
				fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01300, KDDS01300));
				break;
			default:
				fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
				abort();
		}
		exit(-1);
	}

	flg = 0;

	while((n = getline(fd, linebuf, D_LINE_MAX)) != 0){
		strcpy(gwbuf, linebuf);
		strtok(gwbuf, "\t");
		if(strcasecmp(gateway, gwbuf)){
			strcat(linebuf, "\n");
			while(write(nfd, linebuf, n+1) <= 0){
				switch(errno){
				case EINTR :
					continue;
				case ENOSPC :
					fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01305, KDDS01305), strerror(errno));
					break;
				default :
					fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
					abort();
				}
				exit(-1);
			}
		}
		else{
			flg = 1;
		}
	}

	if(flg == 0)
		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01301, KDDS01301), gateway);


	mkf_close(nfd);


	mkf_close(fd);


	sprintf(mkf, "%s%c", D_MASTERKEY_FILE, dfam_id);
	while(rename(D_TMP_FILE, mkf)){
		switch(errno){
		case EINTR :
			continue;
		case EACCES :
		case EDQUOT :
		case ENOSPC :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01305, KDDS01305), strerror(errno));
			break;
		default :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}
		exit(-1);
	}
	exit(0);
}

/*****************************************************************************/
/* ID	= gwlist							     */
/*                                                                           */
/* name = Displaying the Gateway name                                        */
/* func = Displays the Gateway names registered with the master kaey file.   */
/*                                                                           */
/* i/o    = none                                                             */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static void gwlist(void)
{
	int 		fd;
	char 		*ptr;
	char 		linebuf[D_LINE_MAX];

	fd = id_check_and_mkf_open();


	getline(fd, linebuf, D_LINE_MAX);


	while(getline(fd, linebuf, D_LINE_MAX) != 0){
		ptr = strtok(linebuf, "\t");
		if(ptr != NULL){
			fprintf(stderr, "%s\n", linebuf);
		}
	}

	mkf_close(fd);
	exit(0);
}


/*****************************************************************************/
/* ID	= id_check_and_mkf_open						     */
/*                                                                           */
/* name = Checking the users executing the setdfakey command                 */
/* func = Opens the Master key file, and extracts the Administrator          */
/*        registered with the Master key file.  Checks if the Administrator  */
/*        and the user who executes the setdfakey command are the same.      */
/*        Makes an error return if they are different.                       */
/*                                                                           */
/* i/o    = none                                                             */
/* return = fd: int:INP: file descriptor of the opened Master key file       */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static int id_check_and_mkf_open()
{
	uid_t 		uid;
	int 		fd;
	int 		n;
	char 		linebuf[D_LINE_MAX];
	char 		*dfamadmin;
	char 		mkf_name[100];
	struct passwd	*pwent;


	uid = getuid();

	memset(mkf_name, 0, 100);

	sprintf(mkf_name, "%s%c", D_MASTERKEY_FILE, dfam_id);

	while((fd = open(mkf_name, O_RDWR)) < 0){
		switch(errno){
		case EINTR:
			continue;
		case ENFILE:
		case ENOENT:
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01300, KDDS01300));
			break;
		defalut:
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}
		exit(-1);
	}


	do{
		n = getline(fd, linebuf, D_LINE_MAX);
		if(n == 0){
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01302, KDDS01302));
			exit(-1);
		}
	}while(strncmp("DfaAdmin=", linebuf, 9));

	dfamadmin = linebuf + 9;

	if((pwent = getpwnam((const char *)dfamadmin)) == NULL){

		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01303, KDDS01303));
		exit(-1);
	}

	if(uid != pwent->pw_uid){

		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01303, KDDS01303));
		exit(-1);
	}

	if(lseek(fd, 0, SEEK_SET) == -1){
		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
		abort();
	}

	return(fd);
}

/*****************************************************************************/
/* ID	= mkf_close							     */
/*                                                                           */
/* name = Closing the Master key file                                        */
/* func = Closes the Master key file.                                        */
/*                                                                           */
/* i/o    = fd: int:INP: file descriptor of the Master key file to be closed */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static void mkf_close(int fd)
{
	while(close(fd)){
		switch(errno){
		case EINTR :
			continue;
		case ENOSPC :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01305, KDDS01305), strerror(errno));
			break;
		default :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}
		exit(-1);
	}
}

/*****************************************************************************/
/* ID	= turn_off_echo							     */
/*                                                                           */
/* name = Controlling the character outputting to the display                */
/* func = Suppresses displaying the inputed characters from the keyboard.    */
/*                                                                           */
/* i/o    = none                                                             */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static void turn_off_echo()
{
	struct termio 	ttymode;

	while(ioctl(0 , TCGETA, &ttymode)){
		switch(errno){
		case EINTR :
			continue;
		default :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}

	}
	ttymode.c_lflag &= ~ECHO;

	while(ioctl(0 , TCSETA, &ttymode)){
		switch(errno){
		case EINTR :
			continue;
		default :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}
	}
}

/*****************************************************************************/
/* ID	= turn_on_echo							     */
/*                                                                           */
/* name = Controlling the character outputting to the display                */
/* func = Displays the inputed characters from the keyboard.                 */
/*                                                                           */
/* i/o    = none                                                             */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static void turn_on_echo()
{
	struct termio 	ttymode;

	while(ioctl(0 , TCGETA, &ttymode)){
		switch(errno){
		case EINTR :
			continue;
		default :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}

	}

	ttymode.c_lflag |= ECHO;
	while(ioctl(0 , TCSETA, &ttymode)){
		switch(errno){
		case EINTR :
			continue;
		default :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}
	}
}


/*****************************************************************************/
/* ID	= getline							     */
/*                                                                           */
/* name = Getting information from a standard input or file                  */
/* func = Gets information from a standard input or file                     */
/*                                                                           */
/* i/o  = fd    :int   :INP: file descriptor                                 */
/*        ptr   :char *:OUT: read information                                */
/*        maxlen:int   :INP: maximum size of the read information            */
/*                                                                           */
/* return = n :int :INP: inputed (read) size                                 */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static int getline(int fd, char *ptr, int maxlen)
{
	char 	c;
	int 	rc;
	int 	n;


	for(n = 0;n < maxlen;n++){
		while((rc = read(fd, &c, 1)) < 0){
			switch(errno){
			case EINTR :
				continue;
			default :
				fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
				abort();
			}
		}

		if(rc == 1){
			if(c == '\n')
				break;
			*ptr++ = c;
		}
		else if(rc == 0){
			if(n == 1)
				return(0);
			else
				break;
		}
		else{
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}
	}
	*ptr = 0;
	return(n);

}

/*****************************************************************************/
/* ID	= put_master_key						     */
/*                                                                           */
/* name = Registration of a key with the Master key file                     */
/* func = Registers a key with the Master key file.  If the designated       */
/*        Gateway has already been registered, changes the key information.  */
/*        If not registered, goes for registering the key, of course.        */
/*                                                                           */
/* i/o  = gateway: char *:INP: Gateway name                                  */
/*        key    : char *:INP: key information                               */
/*        flag   : int   :INP: in the case that the designated Gateway is    */
/*                             not registered with the Master key file, this */
/*                             indicates whether or not to register the      */
/*                             Gateway                                       */
/*                                                                           */
/* return =  0: normal end                                                   */
/*          -1: abnormal end                                                 */
/*           1: the designated Gateway is not registered with the Master     */
/*              key file                                                     */
/*                                                                           */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static int put_master_key(char *gateway, char *key, int flag)
{
	int 		fd;
	int 		n;
	int             line_length;
	int             left_length;
	register char	*c;
	char	        *key_ptr;
	char		string[D_NWSRV_NAME_MAX];
	char		mkf_name[D_NWSRV_NAME_MAX];
	unsigned long	mkey[2];
	struct flock	flk;

	memcpy((char *)mkey, key, D_KEY_SIZE);

	sprintf(mkf_name, "%s%c", D_MASTERKEY_FILE, dfam_id);
	fd = open(mkf_name, O_RDWR);
	while(fd < 0){

		switch(errno){
		case EINTR:

			continue;
		case ENFILE:
		case ENOENT:
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01300, KDDS01300));
			break;
		default:
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}
		return(-1);
	}


	flk.l_whence = SEEK_SET;
	flk.l_start = 0;
	flk.l_len = 0;
	flk.l_type = F_WRLCK;


	while(fcntl(fd, F_SETLKW, &flk)){
		switch(errno){
		case EINTR :
			continue;
		case EMFILE :
		case ENOLCK :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01300, KDDS01300));
			break;
		default :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}
		return(-1);
	}
	flk.l_type = F_UNLCK;

	line_length = 0;
	c = string;
	while((n = read(fd, c, 1)) != 0){
		if(n == -1){
			switch(errno){
			case EINTR:
				continue;
			default:
				fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
				abort();
			}
		}
		switch(*c){
		case '\t':
			*c = '\0';
			if(strcasecmp(string, gateway)){

				c = string;
				line_length = 0;
				break;
			}
			sprintf(string, "%08x %08x\n", mkey[0], mkey[1]);
			key_ptr = string;
			left_length = strlen(string);

			while(left_length > 0){
				n = write(fd, key_ptr, left_length);
				switch(n){
				case -1:
					switch(errno){
					case EINTR:
						continue;
					case ENOSPC:
						fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01305, KDDS01305), strerror(errno));
						break;
					default:
						fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
						abort();
					}
					break;
				default :
					left_length -= n;
					key_ptr+= n;
					continue;
				}


				while(fcntl(fd, F_SETLKW, &flk)){
					switch(errno){
					case EINTR :
						continue;
					default :
						fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
						abort();
					}
					break;
				}
				mkf_close(fd);
				return(-1);
			}


			while(fcntl(fd, F_SETLKW, &flk)){
				switch(errno){
				case EINTR :
					continue;
				default :
					fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
					abort();
				}
				mkf_close(fd);
				return(-1);
			}
			mkf_close(fd);
			return(0);
		case '\n':
			line_length = 0;
			c = string;
			break;
		default :
			c ++;
			line_length ++;
			if(line_length >= D_NWSRV_NAME_MAX){
				c = string;
				line_length = 0;
			}
			break;
		}
	}



	if(flag == D_ADDKEY_ON){

		sprintf(string, "%s\t%08x %08x\n", gateway, mkey[0], mkey[1]);
		left_length = strlen(string);
		key_ptr = string;

		while(left_length > 0){
			n = write(fd, key_ptr, left_length);
			switch(n){
			case -1:

				switch(errno){
				    case EINTR:
					continue;
				    case ENOSPC:
					fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01305, KDDS01305), strerror(errno));
					break;
				    default:
					fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
					abort();
				}
				break;
			default :
			        key_ptr += n;
				left_length -= n;
				continue;
			}


			while(fcntl(fd, F_SETLKW, &flk)){
				switch(errno){
				case EINTR :
					continue;
				default :
					fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
					abort();
				}
				break;
			}
			mkf_close(fd);
			return(-1);
		}

		while(fcntl(fd, F_SETLKW, &flk)){
			switch(errno){
			case EINTR :
				continue;
			default :
				fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
				abort();
			}
			mkf_close(fd);
			return(-1);
		}
		mkf_close(fd);
		return(0);
	}


	while(fcntl(fd, F_SETLKW, &flk)){
		switch(errno){
		case EINTR :
			continue;
		default :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}
		mkf_close(fd);
		return(-1);
	}

	mkf_close(fd);
	return(1);
}

/*****************************************************************************/
/* ID	= gwname_check							     */
/*                                                                           */
/* name = Gateway name check                                                 */
/* func = Checks if the designated Gateway name is registered with the       */
/*        /etc/hosts file.                                                   */
/*                                                                           */
/* i/o  = gwname: char *:INP: Gateway name                                   */
/*                                                                           */
/* return =  0: already registered                                           */
/*          -1: not registered yet                                           */
/*                                                                           */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static int gwname_check(char *gwname)
{
	struct hostent	*hp;
	char		*hostname;
	struct hostent hst;

	if((hp = gethostbyname(gwname)) == NULL){

		switch(h_errno){
		case HOST_NOT_FOUND :
			return(-1);
		default:
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(h_errno));
			abort();
		}
	}
	memcpy((char *)&hst, (char *)hp, sizeof(struct hostent));

	hostname = hst.h_name;
	while(hostname != NULL){
		if(!strcasecmp(gwname, hostname))
			return(0);

		hostname = *(hst.h_aliases);
		(hst.h_aliases)++;
	}
	return(-1);
}


/*****************************************************************************/
/* ID	= agt__sdk_sig_init						     */
/*                                                                           */
/* name = Setting the process for the SIGINT signal                          */
/* func = Sets the process for the case that the SIGINT signal is received.  */
/*                                                                           */
/* i/o    = none                                                             */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static void agt__sdk_sig_init(void)
{
	struct sigaction newact;
	struct sigaction oldact;
	void (*turn_on_echo_exit)();

	newact.sa_handler = turn_on_echo_exit;
	sigemptyset(&newact.sa_mask);
	sigemptyset(&oldact.sa_mask);
	newact.sa_flags = 0;
	if (sigaction(SIGINT, &newact, &oldact) == -1) {
		fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308),strerror(errno));
		abort();
	}
}



/*****************************************************************************/
/* ID	= turn_on_echo_exit						     */
/*                                                                           */
/* name = Setting the screen display and terminating the process             */
/* func = Makes that the inputed data from the keyboard can be outputed to   */
/*        the display, then exits the process.                               */
/*                                                                           */
/* i/o    = none                                                             */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static void turn_on_echo_exit(void)
{
	struct termio 	ttymode;

	while(ioctl(0 , TCGETA, &ttymode)){
		switch(errno){
		case EINTR :
			continue;
		default :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}

	}

	ttymode.c_lflag |= ECHO;
	while(ioctl(0 , TCSETA, &ttymode)){
		switch(errno){
		case EINTR :
			continue;
		default :
			fprintf(stderr, catgets(sdk_catd, NL_SETD, MSG01308, KDDS01308), strerror(errno));
			abort();
		}
	}
	printf("\n");
	exit(0);
}
