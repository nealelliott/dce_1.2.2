/*
 * @HITACHI_COPYRIGHT@
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
static char *RCSID_agt_dsync_c="$Header: /dce11/project/project/dfa/rcs/dfa1.0/RIOS/riosww/dfamagt/agt_dsync.c,v 1.1.2.8 1996/06/04 01:01:17 ichikawa Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_dsync.c,v $
 * Revision 1.1.2.8  1996/06/04  01:01:17  ichikawa
 * 	Delete switch segment in agt__dsync_opendir.
 * 	[1996/06/04  01:01:06  ichikawa]
 *
 * Revision 1.1.2.7  96/01/12  04:30:20  04:30:20  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:30:12  kishi]
 * 
 * Revision 1.1.2.6  1995/11/10  01:10:18  ichikawa
 * 	Change length of path from MAX_PATH_LEN to PATH_MAX.
 * 	[1995/11/10  01:10:04  ichikawa]
 * 
 * Revision 1.1.2.5  1995/10/26  00:36:54  kishi
 * 	Change Copyright.
 * 	[1995/10/26  00:36:44  kishi]
 * 
 * Revision 1.1.2.4  1995/10/13  09:42:46  ichikawa
 * 	When lstat system call is EFAULT error,add agt_abort.
 * 	[1995/10/13  09:42:23  ichikawa]
 * 
 * Revision 1.1.2.3  1995/10/12  09:33:31  ichikawa
 * 	When stat system call error,agt_usrc_oserr_trace was forgot.
 * 	[1995/10/12  09:33:16  ichikawa]
 * 
 * Revision 1.1.2.2  1995/09/23  09:53:51  kishi
 * 	Add header comment.
 * 	[1995/09/23  06:36:04  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:53:54  kishi
 * 	Initial revision
 * 	[1995/09/09  06:16:29  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "agt_agent.h"
#include "agt_msg.h"
#include "agt_acs.h"
#include "agt_usrc.h"
#include "agt_dsync.h"
#include "agt_login.h"
#include "agt_errid.h"
#include "agt_sysid.h"

struct stat filebuf;
unsigned int count_flag;
time_t checktime;
struct stat filebuf;
time_t cktime;
struct fullpathlist *pathinf,*old,*refer;
struct fullpathlist *head = NULL;
struct fullpathinf *fullpathp = NULL;
struct fullpathlist *work = NULL;
extern struct d_login_info user_info;
struct symlist *symhead = NULL;
int rflag = 0;

static void agt__dsycn_send_data(char *fullpath);
static void agt__dsync_sendenddata(char *senddata);
static void agt__dsync_get_fullname_area(char *fullpath);
static int agt__dsync_stat(char *pathname);
static int agt__dsync_opendir(char *dirname);
static int agt__dsync_readdir(void);
static int agt__dsync_free_check(void);
static void agt__dsync_creat_fullpath(char *fullpath);
static void agt__dsync_all_free(void);
static void agt__dsync_sym_free(void);
static int agt__dsync_sym_check(char *path);
static void agt__dsync_creat_fullpathlist(char *pathname);

/**************************************************************************/
/* ID	= agt_dsync_main						  */
/* name = Directory syncronization main function    			  */
/* func = Performs the following tasks:                                   */
/*        1. Unpacks data received from gateway              		  */
/*        2. Checks whether the mount point has changed                   */
/*        3. Checks whether it is the first request for directory         */
/*           syncronization                                               */
/*        4. If it is the first request, calls     function               */
/*           agt__dsync_opendir to open the directory                     */
/*        5. Calls agt_dsync_readdir function to check state of files     */
/*           under the directory                                          */
/*        6. If any error occurs during execution of tasks above, sends   */
/*           error message to gateway                                     */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information                 */
/*	          :      :  : last modification time                      */
/*            :      :  : length of path name                             */
/*            :      :  : mount point full path name                      */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = From the 2nd request on, the last modification time is set to -1*/
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_dsync_main(char *str)
{
	char senddata[12];
	unsigned int mpnamelen;
	char *mpname;



	memcpy((char *)&cktime,str,sizeof(cktime));
	cktime = ntohl(cktime);
	str += sizeof(cktime);


	if ((rflag == 1) && (cktime == 0xffffffff)){
		agt__dsync_sendenddata(senddata);
		rflag = 0;
		return(0);
	}

	memcpy((char *)&mpnamelen,str,sizeof(mpnamelen));
	mpnamelen = ntohl(mpnamelen);
	str += sizeof(mpnamelen);
	if ((mpname = (char *)malloc(mpnamelen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25104,mpnamelen+1,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_exit(D_EXIT_OUTOFRSC);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d,%d",D_EID_25105,mpnamelen+1,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_abort();
		}
	}
	memcpy(mpname,str,mpnamelen);
	*(mpname+mpnamelen) = '\0';


	if (cktime != MIDDLE){
		agt__dsync_all_free();
		checktime = cktime;
		if (agt__dsync_stat(mpname) == -1)
			goto func_err;
		if (filebuf.st_ctime >= checktime){
			agt__dsycn_send_data(mpname);
			rflag = 1;
			free(mpname);
			return(0);
		}

		if (agt__dsync_opendir(mpname) == -1)
			goto func_err;


		agt__dsync_get_fullname_area(mpname);
	}

	if (agt__dsync_readdir() == -1)
		goto func_err;

	free(mpname);
	return(0);
func_err:
	if (cktime != MIDDLE)
		rflag = 1;
	agt_usrc_send_emsg(senddata,DFAM_DIRSYNC_R);
	free(mpname);
	return(-1);
}

/**************************************************************************/
/* ID 	= agt__dsync_readdir						  */
/* name = Cheking directory entry                                         */
/* func = The function checks the date of last modification in directory  */
/*        entry and the date of last time modifictaion sent by gateway.   */
/*        If the date in the directory entry is more recent than the date */
/*        sent by gateway, the directory entry information is sent to the */
/*        gateway       						  */
/* i/o  = none                                                            */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__dsync_readdir(void)
{
	char senddata[12];
	char *nbp;
	char fullpath[PATH_MAX+1];
	struct dirent *dp=NULL;
	int count;
	char *nep;
	unsigned int fullpathlen;
	unsigned int dp_namelen;

	fullpathlen = strlen(fullpathp->fullpath);
	memcpy(fullpath,fullpathp->fullpath,fullpathlen);
	nbp = (fullpath+fullpathlen);
	*nbp++ = '/';

	while(1){

		errno = 0;
		if ((dp = readdir(dirsyncp)) != NULL){
			if ((strcmp(dp->d_name,".") == 0) || (strcmp(dp->d_name,"..") == 0))
				continue;


			dp_namelen = strlen(dp->d_name);
			if ((fullpathlen+dp_namelen+1) > PATH_MAX){
				agt_usrc_oserr_trace(DREADDIR,ENAMETOOLONG);
				continue;
			}
			for (count = 0,nep = nbp;count < dp_namelen;count++)
				*nep++ = dp->d_name[count];
			*nep++ = '\0';


			if (agt__dsync_stat(fullpath) == -1)
				continue;

			if (filebuf.st_ctime >= checktime){
				agt__dsycn_send_data(fullpath);
				return(0);
			}
			else if((filebuf.st_mode & S_IFMT) == S_IFDIR){

				if (agt__dsync_sym_check(fullpath)){

				agt__dsync_creat_fullpathlist(dp->d_name);
				}
			}
		}
		else if (errno != 0){
			switch(errno){
				case EINTR:
					continue;
				default:
					d_errno = errno;

					agt_usrc_oserr_trace(DREADDIR,d_errno);
					goto dirclose;
			}
		}
		else{
dirclose:
			if (closedir(dirsyncp) == -1){
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25112,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			}

			if (head!=NULL){
				if (refer!= NULL){
					if (agt__dsync_free_check() == 1){

						agt__dsync_sym_free();
						agt__dsync_sendenddata(senddata);
						return(0);
					}
				}
				else{
					refer = head;
				}

retry_open:

				agt__dsync_creat_fullpath(fullpath);

				if (agt__dsync_opendir(fullpath) == -1){
					if (agt__dsync_free_check() == 1){

						agt__dsync_sym_free();
						agt__dsync_sendenddata(senddata);
						return(0);
					}
					goto retry_open;
				}

				agt__dsync_get_fullname_area(fullpath);
				fullpathlen = strlen(fullpath);
				nbp = (fullpath+fullpathlen);
				*nbp++ = '/';
			}
			else{

				agt__dsync_sym_free();
				agt__dsync_sendenddata(senddata);
				return(0);
			}
		}
	}
}

/**************************************************************************/
/* ID 	= agt__dsync_stat						  */
/* name = Getting file status                                             */
/* func = The file gets the status of the file specified in patchname     */
/* i/o  =                                                                 */
/* 	pathname:char *:IN: file name					  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__dsync_stat(char *pathname)
{
	if (stat(pathname,&filebuf) == -1){
		if (cktime == MIDDLE){
			switch(errno){
				case EFAULT:

					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25113,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
				default:
					d_errno = errno;
					agt_usrc_oserr_trace(DSTAT,d_errno);
					return(-1);
			}
		}
		else{
                        switch(errno){
                                case EFAULT:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25114,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
                                AgentMsg(LOG_ERR,msg.buf);
                                agt_abort();
                                default:
                                        d_errno = errno;
					agt_usrc_oserr_trace(DSTAT,d_errno);
                                        return(-1);
			}
		}
	}
	return(0);
}

/**************************************************************************/
/* ID 	= agt__dsync_opendir						  */
/* name = Opening the directory                                           */
/* func = The function opens the directory specified in dirname           */
/* i/o  =                                                                 */
/*	dirname:char *:IN:directory name                                  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__dsync_opendir(char *dirname)
{
	if ((dirsyncp = opendir(dirname)) == NULL){
		d_errno = errno;
		agt_usrc_oserr_trace(DOPENDIR,d_errno);
		return(-1);
	}
	return(0);
}

/**************************************************************************/
/* ID	= agt__dsync_sendenddata					  */
/* name = Sending information about directory syncronization to gateway   */
/* func = Sends to gateway the information that directory syncronization  */
/*        was completed from start to the end                     	  */
/* i/o  =                                                                 */
/*	senddata:char *:IN: directory syncronization ended                */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__dsync_sendenddata(char *senddata)
{
	unsigned int endflag = 0xffffffff;

	memset(senddata,0,8);
	endflag = htonl(endflag);
	memcpy((senddata+8),(char *)&endflag,sizeof(endflag));
	agt_usrc_send_data(senddata,8+sizeof(endflag),DFAM_DIRSYNC_R);
	free(head);
}

/**************************************************************************/
/* ID 	= agt__dsycn_send_data						  */
/* name = Sending information about directory entry to the gateway        */
/* func = Pack directory entry status inforamtion and send to gateway     */
/* i/o  =                                                                 */
/*	fullpath:char *:IN: directory entry name                	  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__dsycn_send_data(char *fullpath)
{
	char *senddata;
	char *begin_send;
	int pathlen;
	unsigned int directory = 0xffffffff;


	pathlen = strlen(fullpath);
	if ((senddata = (char *)malloc(pathlen+20)) == NULL){
		switch(errno){
			case ENOMEM:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25117,pathlen+20,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_exit(D_EXIT_OUTOFRSC);
			default:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25118,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}

	begin_send = senddata;


	memset(senddata,0,8);
	senddata += 8;
	pathlen = htonl(pathlen);
	memcpy(senddata,(char *)&pathlen,sizeof(pathlen));
	senddata += sizeof(pathlen);
	pathlen = ntohl(pathlen);
	memcpy(senddata,fullpath,pathlen);
	senddata += pathlen;
	filebuf.st_mtime = htonl(filebuf.st_mtime);
	memcpy(senddata,(char *)&filebuf.st_mtime,sizeof(filebuf.st_mtime));
	senddata += sizeof(filebuf.st_mtime);
	if ((filebuf.st_mode & S_IFMT) == S_IFDIR){
		directory = htonl(directory);
		memcpy(senddata,(char *)&directory,sizeof(directory));
	}
	else{
		filebuf.st_size = htonl(filebuf.st_size);
		memcpy(senddata,(char *)&filebuf.st_size,sizeof(filebuf.st_size));
	}


	agt_usrc_send_data(begin_send,(20+pathlen),DFAM_DIRSYNC_R);
	free(begin_send);
}

/**************************************************************************/
/* ID	= agt__dsync_get_fullname_area					  */
/* name = Memory allocation to store directory full-path-name             */
/* func = The function allocates memory necessary to store the            */
/*        full-path-name of the directory opened                          */
/* i/o  =                                                                 */
/*	fullpath:char *:IN: directory full-path name        	  	  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__dsync_get_fullname_area(char *fullpath)
{
	unsigned int fullpathlen;

	fullpathlen = strlen(fullpath);
	if ((fullpathp = (struct fullpathinf *)malloc(sizeof(struct fullpathinf))) == NULL){
		switch(errno){
			case ENOMEM:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25119,sizeof(struct fullpathinf),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_exit(D_EXIT_OUTOFRSC);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25120,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();

		}
	}

	if ((fullpathp->fullpath = (char *)malloc(fullpathlen +1)) == NULL){
		switch(errno){
			case ENOMEM:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25121,fullpathlen +1,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_exit(D_EXIT_OUTOFRSC);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25122,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}
	strcpy(fullpathp->fullpath,fullpath);
	fullpathp->count = 0;
}

/**************************************************************************/
/* ID	= agt__dsync_free_check						  */
/* name = Checking directory entries under the opened directory 	  */
/* func = Check if there is any entry under the opened directory. If no , */
/*        release the area allocated by that directory                    */
/* i/o  = none                                                            */
/* return =								  */
/* 		0 :There is an  entry 					  */
/*      1 :There are no entries 					  */ 
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static agt__dsync_free_check(void)
{
	if (fullpathp->count == 0){
		free(fullpathp->fullpath);
		free(fullpathp);
	}



	memcpy((char *)&fullpathp,(char *)&refer->addressinf,sizeof(fullpathp));
	fullpathp->count--;
	if (fullpathp->count == 0){
		free(fullpathp->fullpath);
		free(fullpathp);
	}
	head = refer->next;
	free(refer->dirname);
	free(refer);
	refer = head;
	if (head == NULL){
		return(1);
	}
	return(0);
}

/**************************************************************************/
/* ID	= agt__dsync_creat_fullpath					  */
/* name = Creating directory name for retrieval				  */
/* func = Create the name of the directory to be retrieved                */
/* i/o  =                                                                 */
/*	fullpath:char *:OUT: directory name			  	  */
/* return = none			  				  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__dsync_creat_fullpath(char *fullpath)
{
	unsigned int fullpathlen;


	memcpy((char *)&fullpathp,(char *)&refer->addressinf,sizeof(fullpathp));
	fullpathlen = strlen(fullpathp->fullpath);
	strcpy(fullpath,fullpathp->fullpath);
	memcpy((fullpath+fullpathlen),"/",1);
	strcpy((fullpath+fullpathlen+1),refer->dirname);
}

/**************************************************************************/
/* ID	= agt__dsync_all_free						  */
/* name = Releasing area allocated for directory syncronization           */
/* func = Release any area which has been allocated during directory      */
/*        syncronization process                                          */
/* i/o  = none                                                            */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__dsync_all_free(void)
{
	struct fullpathlist *work = NULL;
	struct fullpathinf *work1 = NULL;
	struct symlist *work2 = NULL;

	work = head;
	while (work != NULL){
		head = work->next;

		memcpy((char *)&work1,(char *)&work->addressinf,sizeof(work1));
		if (work1->count == 0){
			free(work1->fullpath);
			free(work1);
		}
		else
			work1->count--;
		free(work->dirname);
		free(work);
		work = head;
	}
	work2 = symhead;
	while(work2 != NULL){
		symhead = work2->next;
		free(work2);
		work2 = symhead;
	}
}

/**************************************************************************/
/* ID	= agt__dsync_sym_free						  */
/* name = Releasing area used for symbolic link				  */
/* func = Release area allocated during directory syncronization to check */
/*        symbolic link                                                   */
/* i/o  = none                                                            */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__dsync_sym_free()
{
	struct symlist *work2 = NULL;

	work2 = symhead;
	while(work2 != NULL){
		symhead = work2->next;
		free(work2);
		work2 = symhead;
	}
}

/**************************************************************************/
/* ID	= agt__dsync_sym_check						  */
/* name = Checking whether the specified file is a symbolic link file     */
/* func = Check if the specified file is a symbolic link file. If YES,    */
/*        check if it has been registered in the symbolic link table.     */
/*        Register the file if it cannot be found in the table            */
/* i/o  =                                                                 */
/*	path:char *:IN: file name					  */
/* return =                                                               */
/*		0: The file is a new symbolic file                        */
/*      1: The file is NOT a new symbolic file 				  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__dsync_sym_check(char *path)
{
	struct symlist *work = NULL;
	int symflag = 0;
	struct stat fbuf;
	char buf[PATH_MAX];

	if (readlink(path,buf,PATH_MAX) != -1){

		if (lstat(path,&fbuf) == -1){
			switch(errno){
				case EFAULT:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25127,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
					default:
						return(0);
			}
		}
		work = symhead;
		while (work != NULL){
			if (work->inode_num == fbuf.st_ino){
				symflag = 1;
				break;
			}
			work = work->next;
		}
		if (symflag == 0){
			if ((work = (struct symlist *)malloc(sizeof(struct symlist))) == NULL){
				switch(errno){
					case ENOMEM:
						sprintf(msg.wkbuf,"%d,%d",D_EID_25125,user_info.uid);
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
						AgentMsg(LOG_WARNING,msg.buf);
						agt_exit(D_EXIT_OUTOFRSC);
					default:
						sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25126,errno,user_info.uid);
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
						AgentMsg(LOG_ERR,msg.buf);
						agt_abort();
				}
			}
			work->next = symhead;
			symhead = work;
			work->inode_num = fbuf.st_ino;
			return(1);
		}
		else
			return(0);
	}
	return(1);
}

/**************************************************************************/
/* ID	= agt__dsync_creat_fullpathlist					  */
/* name = Inclusion of the directory in the table for retrieval           */
/* func = Include necessary directory name in the table to be searched    */
/*        during directory syncronization process                         */
/* i/o  =                                                                 */
/*	pathname:char *:IN: directory name 				  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__dsync_creat_fullpathlist(char *pathname)
{
	unsigned int pathlen;

	if ((pathinf = (struct fullpathlist *)malloc(sizeof(struct fullpathlist))) == NULL){
		switch(errno){
			case ENOMEM:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25108,sizeof(struct fullpathlist),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_exit(D_EXIT_OUTOFRSC);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25109,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}

	pathlen = strlen(pathname);
	if((pathinf->dirname = (char *)malloc(pathlen+1)) == NULL){
		switch(errno){
			case ENOMEM:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25110,sizeof(struct fullpathlist),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_exit(D_EXIT_OUTOFRSC);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25111,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}
	strcpy(pathinf->dirname,pathname);
	memcpy((char *)&pathinf->addressinf,(char *)&fullpathp,sizeof(fullpathp));
	fullpathp->count++;
	if (head == NULL){
		head = pathinf;
		old = head;
		head->next = NULL;
	}
	else{
		old->next = pathinf;
		old = pathinf;
		old->next = NULL;
	}
}

