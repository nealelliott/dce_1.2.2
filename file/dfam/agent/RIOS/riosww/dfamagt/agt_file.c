/*
 * @HITACHI_COPYRIGHT@
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
static char *RCSID_agt_file_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_file.c,v 1.1.2.4 1996/04/17 17:28:03 root Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: agt_file.c,v $
 * Revision 1.1.2.4  1996/04/17  17:28:03  root
 * 	OT 13430 Submit
 * 	[1996/04/17  17:27:40  root]
 *
 * Revision 1.1.2.13  1996/04/04  06:24:52  ichikawa
 * 	Add treatment of sec_rgy_site_close.
 * 	[1996/04/04  06:24:41  ichikawa]
 * 
 * Revision 1.1.2.12  1996/02/06  00:56:53  ichikawa
 * 	Directory table check was changed.
 * 	[1996/02/06  00:56:40  ichikawa]
 * 
 * Revision 1.1.2.11  1996/01/24  03:14:48  ichikawa
 * 	Change from sec_id_gen_name to sec_rgy_pgo_id_to_name,and from sec_id_parse_name to sec_rgy_pgo_name_to_id.
 * 	[1996/01/24  03:14:37  ichikawa]
 * 
 * Revision 1.1.2.10  1996/01/12  04:31:46  kishi
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:31:36  kishi]
 * 
 * Revision 1.1.2.9  1995/12/21  01:53:26  ichikawa
 * 	Change errno from DENAMETOOLONG to ENAMETOOLONG.
 * 	[1995/12/21  01:52:44  ichikawa]
 * 
 * Revision 1.1.2.8  1995/11/13  02:37:50  ichikawa
 * 	Add dispositon of ntohl.
 * 	[1995/11/13  02:37:33  ichikawa]
 * 
 * Revision 1.1.2.7  1995/11/09  06:05:04  ichikawa
 * 	Disposition of pathname length is changed.
 * 	[1995/11/09  06:04:52  ichikawa]
 * 
 * Revision 1.1.2.6  1995/10/26  00:38:59  kishi
 * 	Change Copyright.
 * 	[1995/10/26  00:38:52  kishi]
 * 
 * Revision 1.1.2.5  1995/09/29  00:51:20  kishi
 * 	Add header comment.
 * 	[1995/09/29  00:51:07  kishi]
 * 
 * Revision 1.1.2.4  1995/09/28  06:37:40  ichikawa
 * 	Add close system call error action.
 * 	[1995/09/28  06:37:27  ichikawa]
 * 
 * Revision 1.1.2.3  1995/09/23  09:53:38  kishi
 * 	Add header comment. Delete Japanese comment.
 * 	[1995/09/23  07:53:20  kishi]
 * 
 * Revision 1.1.2.2  1995/09/14  05:24:23  ichikawa
 * 	Verst mode is change.
 * 	[1995/09/14  05:24:07  ichikawa]
 * 
 * Revision 1.1.2.1  1995/09/09  06:53:39  kishi
 * 	Initial revision
 * 	[1995/09/09  06:20:03  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dce/dce_cf.h>
#include <dce/binding.h>
#include <dce/secidmap.h>
#include <dce/pgo.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <syslog.h>
#include <errno.h>
#include <memory.h>
#include <utime.h>
#include <limits.h>

#include "agt_agent.h"
#include "agt_msg.h"
#include "agt_acs.h"
#include "agt_file.h"
#include "agt_login.h"
#include "agt_sysid.h"
#include "agt_errid.h"

static struct pathlist *pathlisth = NULL;
static struct fdlist *fdlisth = NULL;
sec_rgy_handle_t context;
static int cpwriteflag = 0;
static unsigned int cp_write_datalen = 0;
extern struct d_login_info user_info;
error_status_t status;

static int agt__file_getcontext(sec_rgy_handle_t *context);
static void agt__file_free_fdlist(int fildes);
static int agt__file_fdcheck(int fildes);
static int agt__file_dirpcheck(DIR *dirp);
static int agt__file_context_delete(sec_rgy_handle_t *context);

/**************************************************************************/
/* ID 	= agt_file_open							  */
/* name = Open file							  */
/* func = Unpack data received from gateway and open the file. Share-mode */
/*        and open-type options sent by the gateway are used to lock the  */
/*        the file. The result of the openning process is sent back to    */
/*        gateway                                                         */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information                 */
/*	   :      :  : open-type option				  	  */
/*     :      :  : share-mode of NetWare to open file                     */
/*     :      :  : file name length                         		  */
/*     :      :  : file name						  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_open(char *str)
{
	int Oflag;
	long share;
	unsigned int pathlen;
	char *path;
	int fd;
	char senddata[8];
	static struct flock flk = {0,SEEK_SET,0,0,0};
	struct fdlist *list;


	memcpy((char *)&Oflag,str,sizeof(Oflag));
	str += sizeof(Oflag);
	memcpy((char*)&share,str,sizeof(share));
	str += sizeof(share);
	memcpy((char *)&pathlen,str,sizeof(pathlen));
	str += sizeof(pathlen);


	Oflag = ntohl(Oflag);
	share = ntohl(share);
	pathlen = ntohl(pathlen);


	if ((path = (char *)malloc(pathlen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25020,(pathlen+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(senddata,DFAM_OPEN_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25021,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}


	memcpy(path,str,pathlen);
	*(path+pathlen) = '\0';


	switch(Oflag){
		case O_RDONLY:
		case O_WRONLY:
		case O_RDWR:
			break;
		default:
			d_errno = ECOM;
			goto func_err;
	}


	switch (share){
		case DS_COMPATI:
			if (Oflag == O_RDONLY)
				flk.l_type = F_RDLCK;
			else
				flk.l_type = F_WRLCK;
			break;
		case DS_DENYRW:
		case DS_DENYRD:
		case DS_DENYRW_COM:
		case DS_DENYRD_COM:
			if (Oflag == O_RDONLY){
				if (access(path,W_OK) == 0){
					flk.l_type = F_WRLCK;
					Oflag = O_RDWR;
				}
				else
					flk.l_type = F_RDLCK;
			}
			else
				flk.l_type = F_WRLCK;
			break;
		case DS_DENYWR:
		case DS_NOMODE:
		case DS_DENYWR_COM:
			if (Oflag == O_RDONLY)
				flk.l_type = F_RDLCK;
			else{
				Oflag = O_RDWR;
				flk.l_type = F_WRLCK;
			}
			break;
		default :
			d_errno = ECOM;
			goto func_err;
	}


	while(1){
		if ((fd = open(path,Oflag)) == -1){
			if (errno == EINTR)
				continue;
			d_errno = errno;
			agt_usrc_oserr_trace(DOPEN,d_errno);
			goto func_err;
		}
		break;
	}



	if (fcntl(fd,F_SETLK,&flk) == -1){
		switch(errno){
			case EINVAL:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25022,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DFCNTL,d_errno);
				break;
		}
		while(1){
			if (close(fd) == -1){
				switch(errno){
					case EINTR:
						continue;
					case EBADF:
						sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25023,errno,user_info.uid);
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
						AgentMsg(LOG_ERR,msg.buf);
						agt_abort();
					case ETIMEDOUT:
					case ENOSPC:
					case EDQUOT:
						d_errno = errno;
						cma_unimport_fd(fd);
						agt_usrc_oserr_trace(DCLOSE,d_errno);
						goto func_err;
					default:
						d_errno = errno;
						agt_usrc_oserr_trace(DCLOSE,d_errno);
						goto func_err;
				}
			}
			break;
		}
		goto func_err;
	}


	if (((share == DS_DENYWR) || (share == DS_NOMODE)) && (Oflag != O_RDONLY)){
		flk.l_type = F_RDLCK;
		if (fcntl(fd,F_SETLK,&flk) == -1){
			switch(errno){
				case EINVAL:

					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25024,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
				default:
					d_errno = errno;
					agt_usrc_oserr_trace(DFCNTL,d_errno);
					break;
			}

			while(1){
				if (close(fd) == -1){
					switch(errno){
						case EINTR:
							continue;
						case EBADF:
                                                sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26056,errno,user_info.uid);
                                                sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
                                                AgentMsg(LOG_ERR,msg.buf);
                                                agt_abort();
						case ETIMEDOUT:
						case ENOSPC:
						case EDQUOT:
							d_errno = errno;
							cma_unimport_fd(fd);
							agt_usrc_oserr_trace(DCLOSE,d_errno);
							goto func_err;
						default:
							d_errno = errno;
							agt_usrc_oserr_trace(DCLOSE,d_errno);
							goto func_err;
					}
				}
				break;
			}
			goto func_err;
		}
	}


	if ((list = (struct fdlist *)malloc(sizeof(struct fdlist))) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25025,sizeof(struct fdlist),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				while(1){
					if (close(fd) == -1){
						switch(errno){
							case EINTR:
								continue;
							case EBADF:
								sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25142,errno,user_info.uid);
								sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
								AgentMsg(LOG_ERR,msg.buf);
								agt_abort();
							case ETIMEDOUT:
							case ENOSPC:
							case EDQUOT:
								d_errno = errno;
								cma_unimport_fd(fd);
								agt_usrc_oserr_trace(DCLOSE,d_errno);
								goto func_err;
							default:
								d_errno = errno;
								agt_usrc_oserr_trace(DCLOSE,d_errno);
								goto func_err;
						}
					}
					break;
				}
				goto func_err;
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25026,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}

	list->next = fdlisth;
	fdlisth = list;
	list->fd = fd;


	memset(senddata,0,8);
	fd = htonl(fd);
	memcpy(senddata,(char *)&fd,sizeof(fd));


	agt_usrc_send_data(senddata,8,DFAM_OPEN_R);
	free(path);
        return(0);

func_err:

	memcpy(msg.buf,(char *)&share,4);
	memcpy(&msg.buf[4],(char *)&Oflag,4);
	memcpy(&msg.buf[8],(char *)&user_info.uid,4);
	memcpy(&msg.buf[12],(char *)&d_errno,4);
	memcpy(&msg.buf[16],(char *)&pathlen,4);
	memcpy(&msg.buf[20],path,pathlen);
	AgentTrace(TRACEERR,"DOPEN ",(20+pathlen),msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_OPEN_R);
	free(path);
        return(-1);
}

/**************************************************************************/
/* ID	= agt_file_close						  */
/* name = Close file							  */
/* func = Unpack data received from gateway and close the file. The       */
/*        result of the closing process is sent back to the gateway       */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : file descriptor                                        */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_close(char *str)
{
	int fildes;
	char senddata[8];
	int fdflag;


	memcpy((char *)&fildes,str,sizeof(fildes));


	fildes = ntohl(fildes);


	fdflag = agt__file_fdcheck(fildes);


	if (fdflag == 0){
		d_errno = EBADF;
		goto func_err;
	}


	while(1){
        	if (close(fildes) == -1){
			switch(errno){
				case EINTR:
					continue;
				case EBADF:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26057,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
				case ETIMEDOUT:
				case ENOSPC:
				case EDQUOT:
			 		d_errno = errno;
					cma_unimport_fd(fildes);
					agt__file_free_fdlist(fildes);
					agt_usrc_oserr_trace(DCLOSE,d_errno);
					goto func_err;
				default:
					d_errno = errno;
					agt_usrc_oserr_trace(DCLOSE,d_errno);
					goto func_err;
			}
		}
		break;
        }


	agt__file_free_fdlist(fildes);


	memset(senddata,0,8);
	agt_usrc_send_data(senddata,8,DFAM_CLOSE_R);
        return(0);

func_err:

	memcpy(msg.buf,(char *)&fildes,4);
	memcpy(&msg.buf[4],(char *)&user_info.uid,4);
	memcpy(&msg.buf[8],(char *)&d_errno,4);
	AgentTrace(TRACEERR,"DCLOSE",12,msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_CLOSE_R);
        return(-1);
}

/**************************************************************************/
/* ID	= agt_file_creat						  */
/* name = Creating a file						  */
/* func = Unpack data received from gateway and then create and lock      */
/*        the file. The result of the creating process is sent back to    */
/*        gateway							  */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*	   :      :  : file access mode			  		  */
/*     :      :  : file name length				  	  */
/*     :      :  : file name				  		  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_creat(char *str)
{
	int fd;
	char senddata[8];
	mode_t mode;
	unsigned int pathlen;
	char *path;
	unsigned short moder;

	static struct flock flk = {F_WRLCK,SEEK_SET,0,0,0};
	struct fdlist *list;


	memcpy((char *)&moder,str,sizeof(moder));
	moder = ntohs(moder);
	str += sizeof(moder);
        mode = moder;
	memcpy((char *)&pathlen,str,sizeof(pathlen));
	pathlen = ntohl(pathlen);
	str += sizeof(pathlen);

	if ((path = (char *)malloc(pathlen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25027,(pathlen+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(senddata,DFAM_CREAT_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25028,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}

	memcpy(path,str,pathlen);
	*(path+pathlen) = '\0';


	if ((mode & 0177000) != 0){
		d_errno = ECOM;
		goto func_err;
	}


creat_retry:
	if ((fd = creat(path,mode)) == -1){
		switch(errno){
			case EINTR:
				goto creat_retry;
			case EFAULT:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25029,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DCREAT,d_errno);
				goto func_err;
		}
	}


	if (fcntl(fd,F_SETLK,&flk) == -1){
		switch(errno){
			case EINVAL:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25030,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DFCNTL,d_errno);
				break;
		}
		remove(path);
		goto func_err;
	}


	if ((list = (struct fdlist *)malloc(sizeof(struct fdlist))) == NULL){
		remove(path);
		switch(errno){
			case ENOMEM:
				d_errno = errno;

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25031,sizeof(struct fdlist),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				while(1){
					if (close(fd) == -1){
						switch(errno){
							case EINTR:
								continue;
							case EBADF:
								sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25143,errno,user_info.uid);
								sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
								AgentMsg(LOG_ERR,msg.buf);
								agt_abort();
							case ETIMEDOUT:
							case ENOSPC:
							case EDQUOT:
								d_errno = errno;
								cma_unimport_fd(fd);
								agt_usrc_oserr_trace(DCLOSE,d_errno);
								goto func_err;
							default:
								d_errno = errno;
								agt_usrc_oserr_trace(DCLOSE,d_errno);
								goto func_err;
						}
					}
					break;
				}
				goto func_err;
			default:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25032,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}

	list->next = fdlisth;
	fdlisth = list;
	list->fd = fd;


	memset(senddata,0,8);
	fd = htonl(fd);
	memcpy(senddata,(char *)&fd,sizeof(fd));


	agt_usrc_send_data(senddata,8,DFAM_CREAT_R);
	free(path);
	return(0);

func_err:

	memcpy(msg.buf,(char *)&moder,2);
	memcpy(&msg.buf[2],(char *)&user_info.uid,4);
	memcpy(&msg.buf[6],(char *)&d_errno,4);
	memcpy(&msg.buf[10],(char *)&pathlen,4);
	memcpy(&msg.buf[14],path,pathlen);
	AgentTrace(TRACEERR,"DCREAT",(14+pathlen),msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_CREAT_R);
	free(path);
	return(-1);
}


/**************************************************************************/
/* ID	= agt_file_remove						  */
/* name = Deleting a file 	  					  */
/* func = Unpack data received from gateway and then delete the file. The */
/*        result of the deletion process is sent back to gateway          */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : file name length			  		  */
/*     :      :  : file name				  		  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_remove(char *str)
{
	unsigned long pathlen;
	char *path;
	char senddata[8];
	struct stat buf;


	memcpy((char *)&pathlen,str,sizeof(pathlen));
	pathlen = ntohl(pathlen);
	str += sizeof(pathlen);

	if ((path = (char *)malloc(pathlen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25033,(pathlen+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(senddata,DFAM_REMOVE_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25034,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}

	memcpy(path,str,pathlen);
	*(path+pathlen) = '\0';


	if (stat(path,&buf) == -1){
		switch(errno){
			case EFAULT:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25035,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DSTAT,d_errno);
				goto func_err;
		}
	}

	if ((buf.st_mode & S_IFMT) == S_IFDIR){
		d_errno = EISDIR;
		goto func_err;
	}


	if (remove(path) == -1){
		d_errno = errno;
		agt_usrc_oserr_trace(DREMOVE,d_errno);
		goto func_err;
	}



	memset(senddata,0,8);
	agt_usrc_send_data(senddata,8,DFAM_REMOVE_R);
	free(path);
	return(0);

func_err:

	memcpy(msg.buf,(char *)&user_info.uid,4);
	memcpy(&msg.buf[4],(char *)&d_errno,4);
	memcpy(&msg.buf[8],(char *)&pathlen,4);
	memcpy(&msg.buf[12],path,pathlen);
	AgentTrace(TRACEERR,"DREMV ",12+pathlen,msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_REMOVE_R);
	free(path);
	return(-1);
}

/**************************************************************************/
/* ID 	= agt_file_read							  */
/* name = Reading a file  						  */
/* func = Unpack data received from gateway and then reads only a         */
/*        determined number of bytes. The contents are sent back to       */
/*        gateway                                                         */ 
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : file descriptor                                        */
/*     :      :  : quantity of bytes 					  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_read(char *str)
{
	int fildes;
	size_t nbytes;
	size_t readlen;
	size_t restlen;
	int datalen =0;
	char *readbuf;
	char *workbuf;
	int fdflag;
	char emsg[8];


	memcpy((char *)&fildes,str,sizeof(fildes));
	fildes = ntohl(fildes);
	str += sizeof(fildes);
	memcpy((char *)&nbytes,str,sizeof(nbytes));
	nbytes = ntohl(nbytes);


	fdflag = agt__file_fdcheck(fildes);


        if (fdflag == 0){
		d_errno = EBADF;
		agt_usrc_send_emsg(emsg,DFAM_READ_R);
		return(-1);
        }


	if ((readbuf = (char *)malloc(nbytes+8)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25144,(nbytes+8),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(emsg,DFAM_READ_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25036,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}

	workbuf = (readbuf+8);
	restlen = nbytes;


	while(1){
		if ((readlen = read(fildes,workbuf,restlen)) == -1){
			switch(errno){
				case EINTR:
					continue;
				case EFAULT:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25037,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
				default:
					d_errno = errno;
					agt_usrc_oserr_trace(DREAD,d_errno);
					goto func_err;
			}
		}

		if ((readlen != restlen) && (readlen != 0)){
			workbuf += readlen;
			restlen -= readlen;
			datalen += readlen;
			continue;
		}
		break;
	}
	datalen += readlen;


	memset(readbuf,0,8);
	datalen = htonl(datalen);
	memcpy(readbuf,(char *)&datalen,sizeof(datalen));
	datalen = ntohl(datalen);


	agt_usrc_send_data(readbuf,(8+datalen),DFAM_READ_R);
	free(readbuf);
	return(0);

func_err:
	memcpy(msg.buf,(char *)&fildes,4);
	memcpy(&msg.buf[4],(char *)&nbytes,4);
	memcpy(&msg.buf[8],(char *)&user_info.uid,4);
	memcpy(&msg.buf[12],(char *)&d_errno,4);
	AgentTrace(TRACEERR,"DREAD ",16,msg.buf,0);
	agt_usrc_send_emsg(emsg,DFAM_READ_R);
	free(readbuf);
	return(-1);
}


/**************************************************************************/
/* ID	= agt_file_write						  */
/* name = Writing a file				  		  */
/* func = Unpack data received from gateway and writes only a determined  */
/*        number of bytes. The result of the writing process is sent back */
/*        to gateway                                                      */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : file descriptor        				  */	
/*     :      :  : quantity of bytes				  	  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_write(char *str)
{
	int fildes;
	size_t nbytes;
	size_t restlen;
	int fdflag;
	size_t writelen;
	char senddata[8];


	memcpy((char *)&fildes,str,sizeof(fildes));
	fildes = ntohl(fildes);
	str += sizeof(fildes);
	memcpy((char *)&nbytes,str,sizeof(nbytes));
	nbytes = ntohl(nbytes);
	str += sizeof(nbytes);


	fdflag = agt__file_fdcheck(fildes);


	if (fdflag == 0){
		d_errno = EBADF;
		goto func_err;
	}

	restlen = nbytes;


	while(1){
		if ((writelen = write(fildes,str,restlen)) == -1){
			switch(errno){
				case EINTR:
					continue;
				default:
					d_errno = errno;
					agt_usrc_oserr_trace(DWRITE,d_errno);
					goto func_err;
			}
		}

		if ((writelen != restlen) && (writelen != 0)){
			str += writelen;
			restlen -= writelen;
			continue;
		}
		break;
	}


	memset(senddata,0,8);
	writelen = htonl(writelen);
	memcpy(senddata,(char *)&writelen,sizeof(writelen));


	agt_usrc_send_data(senddata,8,DFAM_WRITE_R);
	return(0);

func_err:

        memcpy(msg.buf,(char *)&fildes,4);
        memcpy(&msg.buf[4],(char *)&nbytes,4);
        memcpy(&msg.buf[8],(char *)&user_info.uid,4);
        memcpy(&msg.buf[12],(char *)&d_errno,4);
	AgentTrace(TRACEERR,"DWRITE",16,msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_WRITE_R);
	return(-1);
}

/**************************************************************************/
/* ID   = agt_file_mkdir						  */
/* name = Creating a directory						  */
/* func = Unpack data received from gateway and create a directory. The   */
/*        result of the creation         process is sent back to gateway  */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : directory name length		  		  */
/*     :      :  : directory name				  	  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_mkdir(char *str)
{
	mode_t mode;
	unsigned long pathlen;
	char *path;
	char senddata[8];
	unsigned short moder;


	memcpy((char *)&moder,str,sizeof(moder));
	moder = ntohs(moder);
	str += sizeof(moder);
	mode = moder;
	memcpy((char *)&pathlen,str,sizeof(pathlen));
	pathlen = ntohl(pathlen);
	str += sizeof(pathlen);


	if ((path = (char *)malloc(pathlen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25038,(pathlen+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(senddata,DFAM_MKDIR_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25039,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				agt_abort();
		}
	}

	memcpy(path,str,pathlen);
	*(path+pathlen) = '\0';


	if ((mode & 0177000) != 0){
		d_errno = ECOM;
		goto func_err;
	}


	if (mkdir(path,mode) == -1){
		d_errno = errno;
		agt_usrc_oserr_trace(DMKDIR,d_errno);
		goto func_err;
	}


	memset(senddata,0,8);
	agt_usrc_send_data(senddata,8,DFAM_MKDIR_R);
	free(path);
	return(0);

func_err:
	memcpy(msg.buf,(char *)&moder,2);
	memcpy(&msg.buf[2],(char *)&user_info.uid,4);
	memcpy(&msg.buf[6],(char *)&d_errno,4);
	memcpy(&msg.buf[10],(char *)&pathlen,4);
	memcpy(&msg.buf[14],path,pathlen);
	AgentTrace(TRACEERR,"DMKDIR",(14+pathlen),msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_MKDIR_R);
	free(path);
	return(-1);
}

/**************************************************************************/
/* ID   = agt_file_rmdir						  */
/* name = Deleting a directory						  */
/* func = Unpack data received from gateway and delete the directory. The */
/*        result of the deletion process is sent back to gateway          */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : directory name length			  	  */
/*     :      :  : directory name			  		  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_rmdir(char *str)
{
	unsigned long pathlen;
	char *path;
	char senddata[8];


	memcpy((char *)&pathlen,str,sizeof(pathlen));
	pathlen = ntohl(pathlen);
	str += sizeof(pathlen);


        if ((path = (char *)malloc(pathlen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25041,(pathlen+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(senddata,DFAM_RMDIR_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25042,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
        }


        memcpy(path,str,pathlen);
        *(path+pathlen) = '\0';


	if(remove(path) == -1){
		d_errno = errno;
		agt_usrc_oserr_trace(DREMOVE,d_errno);
		goto func_err;
	}


	memset(senddata,0,8);
	agt_usrc_send_data(senddata,8,DFAM_RMDIR_R);
	free(path);
	return(0);

func_err:

	agt_usrc_send_emsg(senddata,DFAM_RMDIR_R);
	memcpy(msg.buf,(char *)&user_info.uid,4);
	memcpy(&msg.buf[4],(char *)&d_errno,4);
	memcpy(&msg.buf[8],(char *)&pathlen,4);
	memcpy(&msg.buf[12],path,pathlen);
	AgentTrace(TRACEERR,"DRMDIR",(12+pathlen),msg.buf,0);
	free(path);
	return(-1);
}

/**************************************************************************/
/* ID   = agt_file_rename						  */
/* name = Moving a file					  		  */
/* func = Unpack data received from gateway and move the file. The result */
/*        of the moving process is sent back to gateway                   */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : file name length before moving			  */
/*     :      :  : file name before moving 				  */
/*     :      :  : file name length after  moving			  */
/*     :      :  : file name after  moving 				  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_rename(char *str)
{
	unsigned long soucefilelen;
	unsigned long targetfilelen;
	char *soucefile;
	char *targetfile;
	char senddata[8];


	memcpy((char *)&soucefilelen,str,sizeof(soucefilelen));
	soucefilelen = ntohl(soucefilelen);
	str += sizeof(soucefilelen);


	if ((soucefile = (char *)malloc(soucefilelen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25043,(soucefilelen+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(senddata,DFAM_RENAME_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25044,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();

		}
	}


	memcpy(soucefile,str,soucefilelen);
	*(soucefile+soucefilelen)  = '\0';
	str += soucefilelen;

	memcpy((char *)&targetfilelen,str,sizeof(targetfilelen));
	targetfilelen = ntohl(targetfilelen);
	str += sizeof(targetfilelen);


        if ((targetfile = (char *)malloc(targetfilelen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25045,(targetfilelen+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				free(soucefile);
				agt_usrc_send_emsg(senddata,DFAM_RENAME_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25046,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
        }


        memcpy(targetfile,str,targetfilelen);
        *(targetfile+targetfilelen)  = '\0';


	if (access(targetfile,F_OK) == 0){
		d_errno = EEXIST;
		goto func_err;
	}
	else if (errno != ENOENT){
		switch(errno){
			case EFAULT:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25047,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DACCESS,d_errno);
				goto func_err;
		}
	}


	if (rename(soucefile,targetfile) == -1){
		d_errno = errno;
		agt_usrc_oserr_trace(DRENAME,d_errno);
		goto func_err;
	}


	memset(senddata,0,8);
	agt_usrc_send_data(senddata,8,DFAM_RENAME_R);
	free(soucefile);
	free(targetfile);
	return(0);

func_err:
	memcpy(msg.buf,(char *)&user_info.uid,4);
	memcpy(&msg.buf[4],(char *)&d_errno,4);
	memcpy(&msg.buf[8],(char *)&soucefilelen,4);
	memcpy(&msg.buf[12],soucefile,soucefilelen);
	memcpy(&msg.buf[12+soucefilelen],(char *)&targetfilelen,4);
	memcpy(&msg.buf[16+soucefilelen],targetfile,4);
	AgentTrace(TRACEERR,"DRENAM",(16+soucefilelen+targetfilelen),msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_RENAME_R);
	free(soucefile);
	free(targetfile);
	return(-1);
}

/**************************************************************************/
/* ID	= agt_file_opendir						  */
/* name = Opening a directory					  	  */
/* func = Unpack data received from gateway and open the directory. The   */
/*        result of the opening process is sent back to gateway           */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : directory name length               			  */
/*     :      :  : directory name          				  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_opendir(char *str)
{
	unsigned int dirlen;
	char *dirname;
	char senddata[15];
	struct pathlist *pplist;
	DIR *dir;


	memcpy((char *)&dirlen,str,sizeof(dirlen));
	dirlen = ntohl(dirlen);
	str += sizeof(dirlen);


	if ((dirname = (char *)malloc(dirlen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25049,(dirlen+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(senddata,DFAM_OPENDIR_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25050,errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",4) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}
	memcpy(dirname,str,dirlen);
	*(dirname+dirlen) = '\0';


	if ((dir = opendir(dirname)) == NULL){
		switch(errno){
			case EFAULT:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25051,errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",4) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DOPENDIR,d_errno);
				goto func_err;
		}
	}


	if ((pplist = (struct pathlist *)malloc(sizeof(struct pathlist))) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25052,sizeof(struct pathlist),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				if (closedir(dir) == -1){
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25145,errno,user_info.uid);
					if (memcmp(agt.proctype,"USRM",4) == 0)
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
					else
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
				}
				goto func_err;
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25053,errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",4) == 0)
					 sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}

	if ((pplist->fullpath = (char *)malloc(dirlen)) == NULL){
                switch(errno){
                        case ENOMEM:
				d_errno = errno;
                                sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25054,sizeof(dirlen),user_info.uid);
                                sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
                                AgentMsg(LOG_WARNING,msg.buf);
				if (closedir(dir) == -1){
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25146,errno,user_info.uid);
					if (memcmp(agt.proctype,"USRM",4) == 0)
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
					else
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
				}
				free(pplist);
				goto func_err;
                        default:
                                sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25055,errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",4) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
                                	sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
                                AgentMsg(LOG_ERR,msg.buf);
                                agt_abort();
                }

	}


	pplist->diraddress = dir;
	pplist->pathlen = dirlen;
	memcpy(pplist->fullpath,dirname,dirlen);

	pplist->next = pathlisth;
	pathlisth = pplist;


	memset(senddata,0,16);
	memcpy(&senddata[8],(char *)&dir,sizeof(dir));
	agt_usrc_send_data(senddata,16,DFAM_OPENDIR_R);
	free(dirname);
	return(0);

func_err:

	memcpy(msg.buf,(char *)&user_info.uid,4);
	memcpy(&msg.buf[4],(char *)&d_errno,4);
	memcpy(&msg.buf[8],(char *)&dirlen,4);
	memcpy(&msg.buf[12],dirname,dirlen);
	AgentTrace(TRACEERR,"DDOPEN",(12+dirlen),msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_OPENDIR_R);
	free(dirname);
	return(-1);
}

/**************************************************************************/
/* ID 	= agt_file_closedir						  */
/* name = Closing a directory					  	  */
/* func = Unpack data received from gateway and close the directory. The  */
/*        result of the closing process is sent back to gateway           */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : directory name length               			  */
/*     :      :  : directory name          				  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_closedir(char *str)
{
	DIR *dirp;
	struct pathlist *pplist;
	struct pathlist *old;
	int dirpflag = 0;
	char senddata[8];


	memcpy((char *)&dirp,str,sizeof(dirp));


	dirpflag = agt__file_dirpcheck(dirp);


	if (dirpflag == 0){
		d_errno = EBADF;
		goto func_err;
	}

	if (closedir(dirp) == -1){
		switch(errno){
			case EBADF:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25056,errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",4) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DCLOSEDIR,d_errno);
				goto func_err;
		}
	}


	pplist = old = pathlisth;
	while (pplist != NULL){
		if (memcmp((char *)&pplist->diraddress,(char *)&dirp,sizeof(dirp)) == 0)
		{
			if (pplist == pathlisth)
				pathlisth = pplist->next;
			else
				old->next = pplist->next;


			free(pplist->fullpath);
			free(pplist);
			break;
		}
		old = pplist;
		pplist = pplist->next;
	}



	memset(senddata,0,8);
	agt_usrc_send_data(senddata,8,DFAM_CLOSEDIR_R);
	return(0);

func_err:

	memcpy(&msg.buf,(char *)&user_info.uid,4);
	memcpy(&msg.buf[4],(char *)&d_errno,4);
	AgentTrace(TRACEERR,"DDCLOS",8,msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_CLOSEDIR_R);
	return(-1);
}

/**************************************************************************/
/* ID	= agt_file_readstat						  */
/* name = Getting directory entry information				  */
/* func = Unpack data received from gateway and get entry information.    */
/*        Pick out one of the entries in the directory opened and get all */
/*        related information about this entry. The information is sent   */
/*        back to gateway                                                 */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : DIR structure returned from opendir system call        */
/*     :      :  : address                 				  */
/*     :      :  : option                 				  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_readstat(char *str)
{
	DIR *dirp;
	struct dirent *dp;
	short option;
	int entry_namelen;
	int d_namelen;
	unsigned int datalen=0;
	int dirpflag;
	char senddata[DPATH_MAX+DNAME_MAX+30];
	char *begin_sd;
	char fullpath[PATH_MAX+2];
	int endflag = 0xffffffff;
	unsigned int nameflag = 0;
	struct stat buf;
	uuid_t cell_idp;
	uuid_t pri_uuid;
	sec_rgy_name_t globalname;
 	sec_rgy_handle_t context;
	sec_rgy_name_t cell_namep;
	sec_rgy_name_t princ_namep;
	int prinname_len;
	struct pathlist *pplist;
	unsigned short moder;
	int context_flag = 0;



	memcpy((char *)&dirp,str,sizeof(dirp));
	str += 8;
	memcpy((char *)&option,str,sizeof(option));


	dirpflag = agt__file_dirpcheck(dirp);


	if (dirpflag == 0){
		d_errno = EBADF;
		goto func_err;
	}

	begin_sd = senddata;


	errno = 0;
	while(1){
		if((dp = readdir(dirp)) != NULL){
			if ((strcmp(dp->d_name,".") == 0) ||(strcmp(dp->d_name,"..") ==0))
				continue;

			pplist = pathlisth;
			while (pplist != NULL){
				if (dirp == pplist->diraddress){
					memcpy(fullpath,pplist->fullpath,pplist->pathlen);
					fullpath[pplist->pathlen] = '/';
					break;
				}
				pplist = pplist->next;
			}
			d_namelen = strlen(dp->d_name);

			if ((pplist->pathlen + d_namelen + 1) > PATH_MAX){
				d_errno = ENAMETOOLONG;
				goto func_err;
			}

			memcpy(&fullpath[pplist->pathlen+1],dp->d_name,d_namelen);
			fullpath[pplist->pathlen+d_namelen+1] = '\0';
			if (stat(fullpath,&buf) == -1){
				switch(errno){
					case EFAULT:
						sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25057,errno,user_info.uid);
						if (memcmp(agt.proctype,"USRM",4) == 0)
							sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
						else
							sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
						AgentMsg(LOG_ERR,msg.buf);
						agt_abort();
					default:
						d_errno = errno;
						agt_usrc_oserr_trace(DSTAT,d_errno);
						goto func_err;
				}
			}

			if (((option & 0x0001) == ONLYDIR) && ((buf.st_mode & S_IFMT) != S_IFDIR))
				continue;


			memset(begin_sd,0,8);
			begin_sd += 8;
			datalen += 8;
			entry_namelen = strlen(dp->d_name);
			entry_namelen = htonl(entry_namelen);
			memcpy(begin_sd,(char *)&entry_namelen,sizeof(entry_namelen));
			begin_sd += sizeof(entry_namelen);
			datalen += sizeof(entry_namelen);

			entry_namelen = ntohl(entry_namelen);

			memcpy(begin_sd,dp->d_name,entry_namelen);
			begin_sd += entry_namelen;
			datalen += entry_namelen;

			moder = buf.st_mode;
			moder = htons(moder);
			memcpy(begin_sd,(char *)&moder,sizeof(moder));
			begin_sd += sizeof(moder);
			datalen += sizeof(moder);

			if ((option&0x0006) == 0x0002){
				if (entry_namelen > (DPATH_MAX+DNAME_MAX)){
					d_errno = ENAMETOOLONG;
					goto func_err;
				}
				buf.st_mtime = htonl(buf.st_mtime);
				memcpy(begin_sd,(char *)&buf.st_mtime,sizeof(buf.st_mtime));
				begin_sd += sizeof(buf.st_mtime);
				datalen += sizeof(buf.st_mtime);
			}
			else if((option&0x0006) == 0x0006){

	                	buf.st_mtime = htonl(buf.st_mtime);
       		        	memcpy(begin_sd,(char *)&buf.st_mtime,sizeof(buf.st_mtime));
				begin_sd += sizeof(buf.st_mtime);
	                	datalen += sizeof(buf.st_mtime);


			if (agt__file_getcontext(&context) == -1)
				goto func_err;
			context_flag = 1;

			sec_rgy_pgo_unix_num_to_id(context,sec_rgy_domain_person,buf.st_uid,&pri_uuid,&status);
			if (status != error_status_ok){
				switch(status){
					case sec_rgy_object_not_found:

						nameflag = 1;
						goto name_entry;
					case sec_rgy_server_unavailable:
						d_errno = DEUNAVAILSRV;
						agt_usrc_oserr_trace(SECPGOUNTI,status);
						goto func_err;
					default:
						d_errno = DEUNKNOWN;
						sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25148,status,user_info.uid);
						sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
						AgentMsg(LOG_ERR, msg.buf);

						goto func_err;
				}
			}


			sec_rgy_pgo_id_to_name(context,sec_rgy_domain_person,&pri_uuid,princ_namep,&status);
			if (status != error_status_ok){
                        	switch(status){
                                	case sec_rgy_object_not_found:
                                        	nameflag = 1;
                                        	break;
                                	case sec_rgy_server_unavailable:
                                        	d_errno = DEUNAVAILSRV;
						agt_usrc_oserr_trace(SECIDGENNAME,status);
						goto func_err;
					case sec_id_e_name_too_long:
						d_errno = ENAMETOOLONG;
						agt_usrc_oserr_trace(SECIDGENNAME,status);
						goto func_err;
					case sec_id_e_bad_cell_uuid:
						agt_usrc_oserr_trace(SECIDGENNAME,status);
						sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25149,status,user_info.uid);
						if (memcmp(agt.proctype,"USRM",4) == 0)
							sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
						else
							sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
						AgentMsg(LOG_ERR, msg.buf);
                                                agt_abort();
					default:
						d_errno = DEUNKNOWN;
						sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25163,status,user_info.uid);
						sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
						AgentMsg(LOG_ERR, msg.buf);
						goto func_err;
                        	}
			}

name_entry:
			if (nameflag == 0){
				prinname_len = strlen((char *)princ_namep);
				if ((entry_namelen+prinname_len) > (DPATH_MAX+DNAME_MAX)){
					d_errno = ENAMETOOLONG;
					goto func_err;
				}
			}
			else{
				if (entry_namelen > (DPATH_MAX+DNAME_MAX)){
					d_errno = ENAMETOOLONG;
					goto func_err;
				}
				prinname_len = 0xffffffff;
			}

			prinname_len = htonl(prinname_len);
			memcpy(begin_sd,(char *)&prinname_len,sizeof(prinname_len));
			begin_sd += sizeof(prinname_len);
			datalen += sizeof(prinname_len);


			prinname_len = ntohl(prinname_len);
			if (nameflag == 0){
				memcpy(begin_sd,princ_namep,prinname_len);
				begin_sd += prinname_len;
				datalen += prinname_len;
			}


			buf.st_size = htonl(buf.st_size);
			memcpy(begin_sd,(char *)&buf.st_size,sizeof(buf.st_size));
			begin_sd += sizeof(buf.st_size);
			datalen += sizeof(buf.st_size);
			}
			break;
		}
		else if(errno == 0){
			memset(senddata,0,8);
			endflag = htonl(endflag);
			memcpy(senddata,(char *)&endflag,sizeof(endflag));
			datalen += 8;
			break;
		}
		else if(errno == EBADF){

			sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25058,errno,user_info.uid);
			if (memcmp(agt.proctype,"USRM",4) == 0)
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
			else
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
			AgentMsg(LOG_ERR,msg.buf);
			agt_abort();
		}
		else{
			d_errno = errno;
			agt_usrc_oserr_trace(DREADDIR,d_errno);
			goto func_err;
		}
	}


	if (context_flag == 1){
		if (agt__file_context_delete(&context) != 0){
			context_flag = 0;
			goto func_err;
		}
	}
	agt_usrc_send_data(senddata,datalen,DFAM_READSTAT_R);

	return(0);

func_err:
	if (context_flag == 1)
		agt__file_context_delete(&context) ;
	memcpy(msg.buf,(char *)&dirp,4);
	memcpy(&msg.buf[4],(char *)&option,2);
	memcpy(&msg.buf[6],(char *)&user_info.uid,4);
	memcpy(&msg.buf[10],(char *)&d_errno,4);
	AgentTrace(TRACEERR,"DENTRY",14,msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_READSTAT_R);
	return(-1);
}

/**************************************************************************/
/* ID 	= agt_file_utime						  */
/* name = Updating last modification time of the file                     */
/* func = Unpack data received from gateway and update the last time of   */
/*        file modification. The result of the updating process is sent   */
/*        back to gateway                                                 */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : last time of file access                 		  */
/*     :      :  : last time of file modification           		  */
/*     :      :  : file name length                         		  */
/*     :      :  : file name                                		  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_utime(char *str)
{
	unsigned short opt;
	time_t st_atime;
	time_t st_mtime;
	unsigned long pathlen;
	char *pathname;
	struct utimbuf *ctimes;
	char senddata[8];


	memcpy((char *)&opt,str,sizeof(opt));
	opt = ntohs(opt);
	str += sizeof(opt);
	memcpy((char *)&st_atime,str,sizeof(st_atime));
	st_atime = ntohl(st_atime);
	str += sizeof(st_atime);
	memcpy((char *)&st_mtime,str,sizeof(st_mtime));
	st_mtime = ntohl(st_mtime);
	str += sizeof(st_mtime);
	memcpy((char *)&pathlen,str,sizeof(pathlen));
	pathlen = ntohl(pathlen);
	str += sizeof(pathlen);
	if ((pathname = (char *)malloc(pathlen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25059,(pathlen+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(senddata,DFAM_UTIME_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25060,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
        }
	memcpy(pathname,str,pathlen);
	*(pathname+pathlen) = '\0';


	if ((ctimes = (struct utimbuf *)malloc(sizeof(struct utimbuf))) == NULL){
                switch(errno){
                        case ENOMEM:
				d_errno = errno;
                                sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25061,(pathlen+1),user_info.uid);
                                sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
                                AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(senddata,DFAM_UTIME_R);
				free(pathname);
				return(-1);
                        default:
                                sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25062,errno,user_info.uid);
                                sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
                                AgentMsg(LOG_ERR,msg.buf);
                                agt_abort();
                }
        }


	if (opt == 0){
		ctimes->actime = st_atime;
		ctimes->modtime = st_mtime;
	}
	else if (opt == 0xffff)
	 	ctimes = NULL;
	else{
		d_errno = ECOM;
		goto func_err;
	}

	if (utime(pathname,ctimes) == -1){
		switch(errno){
			case EFAULT:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25063,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DUTIME,d_errno);
				goto func_err;
		}
	}


        memset(senddata,0,8);
        agt_usrc_send_data(senddata,8,DFAM_UTIME_R);
	free(pathname);
	free(ctimes);
        return(0);

func_err:
	memcpy(msg.buf,(char *)&opt,2);
	memcpy(&msg.buf[2],(char *)&st_atime,4);
	memcpy(&msg.buf[6],(char *)&st_mtime,4);
	memcpy(&msg.buf[10],(char *)&user_info.uid,4);
	memcpy(&msg.buf[14],(char *)&d_errno,4);
	memcpy(&msg.buf[18],(char *)&pathlen,4);
	memcpy(&msg.buf[22],pathname,pathlen);
	AgentTrace(TRACEERR,"DUTIME",22+pathlen,msg.buf,0);
        agt_usrc_send_emsg(senddata,DFAM_UTIME_R);
	free(pathname);
	free(ctimes);
        return(-1);
}

/**************************************************************************/
/* ID	= agt_file_chown						  */
/* name = Changing file owner name					  */
/* func = Unpack data received from gateway and change file owner name.   */
/*        The result of the changing process is sent back to gateway      */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : file name length to change owner                       */
/*     :      :  : file name to change owner				  */
/*     :      :  : owner name length					  */
/*     :      :  : owner name						  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
agt_file_chown(char *str)
{
	char *filename;
	char *username;
	unsigned long filenamelen;
	unsigned long usernamelen;
	sec_rgy_handle_t context;
	sec_rgy_name_t globalname;
	sec_rgy_name_t princnamep;
	sec_rgy_name_t cellnamep;
	uuid_t cell_uuid;
	uuid_t princ_uuid;
	long unix_id;
	char * cellname;
	uid_t owner;
	uid_t group;
	sec_rgy_name_t pgo_name;
	char senddata[8];


	memcpy((char *)&filenamelen,str,sizeof(filenamelen));
	filenamelen = ntohl(filenamelen);
	str += sizeof(filenamelen);

	if ((filename = (char *)malloc(filenamelen + 1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25064,(filenamelen+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(senddata,DFAM_CHOWN_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25065,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034));
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}
	memcpy(filename,str,filenamelen);
	str += filenamelen;
	*(filename+filenamelen) = '\0';

	memcpy((char *)&usernamelen,str,sizeof(usernamelen));
	usernamelen = ntohl(usernamelen);
	str += sizeof(usernamelen);

	if ((username = (char *)malloc(usernamelen + 1)) == NULL){
                switch(errno){
                        case ENOMEM:
				d_errno = errno;
                                sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25066,(usernamelen+1),user_info.uid);
                                sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
                                AgentMsg(LOG_WARNING,msg.buf);
				free(filename);
				agt_usrc_send_emsg(senddata,DFAM_CHOWN_R);
				return(-1);
                        default:
                                sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25067,errno,user_info.uid);
                                sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034));
                                AgentMsg(LOG_ERR,msg.buf);
                                agt_abort();
                }

	}
	memcpy(username,str,usernamelen);
	str += usernamelen;
	*(username+usernamelen) = '\0';

	if (usernamelen > 1024){
		d_errno = ENAMETOOLONG;
		goto func_err;
	}

	strcpy((char *)pgo_name,username);

	if (agt__file_getcontext(&context) == -1){
		goto func_err;
	}

	sec_rgy_pgo_name_to_unix_num(context,sec_rgy_domain_person,pgo_name,&unix_id,&status);
	if (status != error_status_ok){
		switch(status){
			case sec_rgy_object_not_found:
				d_errno = DEUNNOWNPRI;
				agt_usrc_oserr_trace(SECIDPARCENAME,status);
				goto func_err;
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECIDPARCENAME,status);
				goto func_err;
			case sec_id_e_name_too_long:
				d_errno = ENAMETOOLONG;
				agt_usrc_oserr_trace(SECIDPARCENAME,status);
				goto func_err;
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25164,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR, msg.buf);
				goto func_err;
		}
	}

	owner = unix_id;


	group = GID_NO_CHANGE;



	if (chown(filename,owner,group) == -1){
		switch(errno){
			case EFAULT:
			case EINVAL:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25068,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DCHOWN,d_errno);
				goto func_err;

		}
	}


	memset(senddata,0,8);
	agt_usrc_send_data(senddata,8,DFAM_CHOWN_R);
	free(filename);
	free(username);
	return(0);

func_err:
	memcpy(msg.buf,(char *)&user_info.uid,4);
	memcpy(&msg.buf[4],(char *)&d_errno,4);
	memcpy(&msg.buf[8],(char *)&filenamelen,4);
	memcpy(&msg.buf[12],filename,filenamelen);
	memcpy(&msg.buf[12+filenamelen],(char *)&usernamelen,4);
	memcpy(&msg.buf[16+filenamelen],username,usernamelen);
	AgentTrace(TRACEERR,"DCHOWN",(16+filenamelen+usernamelen),msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_CHOWN_R);
	free(filename);
	free(username);
	return(-1);
}

/**************************************************************************/
/* ID	= agt_file_stat							  */
/* name = Getting file information					  */
/* func = Unpack data received from gateway and get file status 	  */
/*        information. The result of the getting process is sent back to  */
/*        gateway      							  */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*         :      :  : file name length					  */
/*         :      :  : file name					  */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
agt_file_stat(char *str)
{
	char *pathname;
	struct stat fileinf;
	sec_rgy_name_t cellnamep;
	sec_rgy_name_t princ_name;
	sec_rgy_name_t globalname;
	uuid_t cell_uuid;
	uuid_t princ_uuid;
	uuid_t st_uuid;
	sec_rgy_handle_t context;
	char *senddata;
	char *begin_sd;
	char esend_data[8];
	unsigned long pathlen;
	unsigned int nameflag = 0;
	unsigned long datalen = 0;
	unsigned short moder;
	int context_flag = 0;


	memcpy((char *)&pathlen,str,sizeof(pathlen));
	pathlen = ntohl(pathlen);
	str += sizeof(pathlen);
	if ((pathname = (char *)malloc(pathlen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25069,(pathlen+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno), msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(esend_data,DFAM_STAT_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25070,errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",4) == 0)
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else

				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}
	memcpy(pathname,str,pathlen);
	*(pathname+pathlen) = '\0';


	if (agt__file_getcontext(&context) == -1){
		goto func_err;
	}

	context_flag = 1;


	if (stat(pathname,&fileinf) == -1){
		switch(errno){
			case EFAULT:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25071,errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",4) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DSTAT,d_errno);
				goto func_err;
		}
	}



        sec_rgy_pgo_unix_num_to_id(context,sec_rgy_domain_person,fileinf.st_uid,&st_uuid,&status);
        if (status != error_status_ok){
		switch(status){
			case sec_rgy_object_not_found:
				nameflag = 1;
				goto noentry;
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECUNIXNUMTOID,status);
				goto func_err;
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25152,status,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
				AgentMsg(LOG_ERR, msg.buf);
				goto func_err;
		}
        }


	sec_rgy_pgo_id_to_name(context,sec_rgy_domain_person,&st_uuid,princ_name,&status);
        if (status != error_status_ok){
		switch(status){
			case sec_rgy_object_not_found:
				nameflag = 1;
				break;
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECIDGENNAME,status);
				goto func_err;
			case sec_id_e_name_too_long:
				d_errno = ENAMETOOLONG;
				agt_usrc_oserr_trace(SECIDGENNAME,status);
				goto func_err;
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25165,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR, msg.buf);
				goto func_err;
		}
        }
noentry:

	if (nameflag == 0){
		pathlen = strlen((char *)princ_name);
		if ((senddata = (char *)malloc((8+sizeof(moder)+sizeof(fileinf.st_size)+sizeof(fileinf.st_mtime)+sizeof(pathlen)+pathlen))) == NULL){
			switch(errno){
				case ENOMEM:
					d_errno = errno;
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25230,(8+sizeof(moder)+sizeof(fileinf.st_size)+sizeof(fileinf.st_mtime)+sizeof(pathlen)+pathlen),user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
					AgentMsg(LOG_WARNING,msg.buf);
					goto func_err;
				default:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25231,(8+sizeof(moder)+sizeof(fileinf.st_size)+sizeof(fileinf.st_mtime)+sizeof(pathlen)+pathlen),user_info.uid);
					if (memcmp(agt.proctype,"USRM",4) == 0)
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
					else
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
			}
		}
	}
	else{
		if ((senddata = (char *)malloc((8+sizeof(moder)+sizeof(fileinf.st_size)+sizeof(fileinf.st_mtime)+sizeof(pathlen)))) == NULL){
			switch(errno){
				case ENOMEM:
					d_errno = errno;
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25232,(8+sizeof(moder)+sizeof(fileinf.st_size)+sizeof(fileinf.st_mtime)+sizeof(pathlen)+pathlen),user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
					AgentMsg(LOG_WARNING,msg.buf);
					goto func_err;
				default:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25233,(8+sizeof(moder)+sizeof(fileinf.st_size)+sizeof(fileinf.st_mtime)+sizeof(pathlen)+pathlen),user_info.uid);
					if (memcmp(agt.proctype,"USRM",4) == 0)
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
					else
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
			}
		}
	}

	begin_sd = senddata;

	memset(begin_sd,0,8);
	begin_sd += 8;
	datalen += 8;


	moder = fileinf.st_mode;
	moder = htons(moder);
	memcpy(begin_sd,(char *)&moder,sizeof(moder));
	begin_sd += sizeof(moder);
	datalen += sizeof(moder);


	fileinf.st_size = htonl(fileinf.st_size);
	memcpy(begin_sd,(char *)&fileinf.st_size,sizeof(fileinf.st_size));
	begin_sd += sizeof(fileinf.st_size);
	datalen += sizeof(fileinf.st_size);


	fileinf.st_mtime = htonl(fileinf.st_mtime);
	memcpy(begin_sd,(char *)&fileinf.st_mtime,sizeof(fileinf.st_mtime));
	begin_sd += sizeof(fileinf.st_mtime);
	datalen += sizeof(fileinf.st_mtime);


	if (nameflag == 0){
		pathlen = htonl(pathlen);
		memcpy(begin_sd,(char *)&pathlen,sizeof(pathlen));
		begin_sd += sizeof(pathlen);
		datalen += sizeof(pathlen);
		pathlen = ntohl(pathlen);
		memcpy(begin_sd,(char*)princ_name,pathlen);
		datalen += pathlen;
	}
	else{
		pathlen = 0xffffffff;
		pathlen = htonl(pathlen);
		memcpy(begin_sd,(char *)&pathlen,sizeof(pathlen));
		datalen += sizeof(pathlen);
	}


        if (context_flag == 1){
                if (agt__file_context_delete(&context) != 0){
                        context_flag = 0;
			free(senddata);
                        goto func_err;
                }
        }
	agt_usrc_send_data(senddata,datalen,DFAM_STAT_R);
	free(pathname);
	free(senddata);
	return(0);

func_err:
	if (context_flag == 1)
		agt__file_context_delete(&context);
	memcpy(msg.buf,(char *)&user_info.uid,4);
	memcpy(&msg.buf[4],(char *)&d_errno,4);
	memcpy(&msg.buf[8],(char *)&pathlen,4);
	memcpy(&msg.buf[12],pathname,pathlen);
	AgentTrace(TRACEERR,"DSTAT ",(12+pathlen),msg.buf,0);
	agt_usrc_send_emsg(esend_data,DFAM_STAT_R);
	free(pathname);
	return(-1);
}

/**************************************************************************/
/* ID	= agt__file_getcontext						  */
/* name = Getting information about context in sec_xxx function           */
/* func = Performs the following tasks					  */
/*        1.Get local cell name						  */
/*        2.Get information about context in sec_xxx function		  */
/*        3.Get cell UUID						  */
/*        4.Create global login name					  */
/* i/o  =                                                                 */
/*  dname     :char *            :IN :login user name			  */
/*  context   :sec_rgy_handle_t *:OUT:OPEIQU handle binded to the         */
/*                                    registration list handle            */
/*  globalname:sec_rgy_name_t *  :OUT:global name of login user           */
/*  cell_uuid :uuid_t *	     :OUT:information about cel UUID	          */	
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__file_getcontext(sec_rgy_handle_t *context)
{
	char *cellname;

	dce_cf_get_cell_name(&cellname,(unsigned long *)&status);
	if (status != error_status_ok){
		switch(status){
			case dce_cf_e_no_mem:
				d_errno = DENOMEM;
				sprintf(msg.wkbuf,"%d,%d",D_EID_25073,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(d_errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25154,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR, msg.buf);
				return(-1);
		}
	}


	sec_rgy_site_open((unsigned_char_t *)cellname,context,&status);
	if (status != error_status_ok){
		switch(status){
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECSITEOPEN,status);
				free(cellname);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25155,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				free(cellname);
				return(-1);
		}
	}

	free(cellname);
	return(0);
}

/**************************************************************************/
/* ID	= agt__file_fdcheck						  */
/* name = Checking file descriptor					  */
/* func = Check if the file descriptor has been registered in the         */
/*        file descriptor table						  */
/* i/o  =                                                                 */
/*	fildes:int:IN:file descriptor					  */
/* return = 0: descriptor already registered                              */
/*         -1: descriptor not registered yet                              */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__file_fdcheck(int fildes)
{
	struct fdlist *list = NULL;

	list = fdlisth;
	while (list != NULL){
		if (list->fd == fildes)
			return(1);
		list = list->next;
	}
	return(0);
}

/**************************************************************************/
/* ID	= agt__file_free_fdlist						  */
/* name = Release file descriptor area		     			  */
/* func = Release the file descriptor area from file descriptor table     */
/* i/o  =                                                                 */
/*	fildes:int:IN:file descriptor				          */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__file_free_fdlist(int fildes)
{
	struct fdlist *checklist,*old;

	checklist = old = fdlisth;
	while (checklist != NULL){
		if (checklist->fd == fildes){
			if (checklist == fdlisth)
				fdlisth = checklist->next;
			else
				old->next = checklist->next;

			free(checklist);
			break;
		}
		old = checklist;
		checklist = checklist->next;
	}
}

/**************************************************************************/
/* ID   = agt__file_dirpcheck						  */
/* name = Check information about openned directory                       */
/* func = Check if the directory stream has been registered in the 	  */
/*        directory table						  */
/* i/o  =                                                                 */
/*	dirp:DIR *:IN:directory stream					  */
/* return = 0: stream already registered	                          */
/*         -1: stream not registered yet                                  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__file_dirpcheck(DIR *dirp)
{
	static struct pathlist *list;

	list = pathlisth;
	while (list != NULL){
		if (list->diraddress == dirp)
			return(1);
		list = list->next;
	}
	return(0);
}

/**************************************************************************/
/* ID	= agt_file_lseek						  */
/* name = Move read/write pointer (seek)				  */
/* func = Unpack data received from gateway and move file pointer to the  */
/*        beginning of the file. The result of the moving   process is    */
/*        sent back to gateway      					  */
/* i/o  =                                                                 */
/*	str:char *:IN: Contains the following information:                */
/*     :      :  : file descriptor					  */
/*     :      :  : off-set						  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_file_lseek(char *str)
{
	int fildes;
	off_t offset;
	off_t length;
	int fdflag;
	char senddata[8];


	memcpy((char *)&fildes,str,sizeof(fildes));
	fildes = ntohl(fildes);
	str += sizeof(fildes);
	memcpy((char *)&offset,str,sizeof(offset));
	offset = ntohl(offset);
	str += sizeof(offset);
	memcpy((char *)&length,str,sizeof(length));
	length = ntohl(length);


	fdflag = agt__file_fdcheck(fildes);
	if (fdflag == 0){
		d_errno = EBADF;
		goto func_err;
	}


	if (offset != -1){
		if (lseek(fildes,offset,SEEK_SET) == -1){
			d_errno = errno;
			agt_usrc_oserr_trace(DLSEEK,d_errno);
			goto func_err;
		}
	}

	if (length != 0xffffffff){
		if (ftruncate(fildes,length) == -1){
			d_errno = errno;
			agt_usrc_oserr_trace(DTRUNCATE,d_errno);
			goto func_err;
		}
	}

        memset(senddata,0,8);
        agt_usrc_send_data(senddata,8,DFAM_LSEEK_R);
        return(0);

func_err:

	memcpy(msg.buf,(char *)&fildes,4);
	memcpy(&msg.buf[4],(char *)&offset,4);
	memcpy(&msg.buf[8],(char *)&length,4);
	memcpy(&msg.buf[12],(char *)&d_errno,4);
	AgentTrace(TRACEERR,"DLSEEK",16,msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_LSEEK_R);
	return(-1);
}

/**************************************************************************/
/* ID	= agt_file_cpread						  */
/*                                                                        */
/* name = Reading files by burst mode                                     */
/* func = Reads a file (the inputting size is designated by Gateway),     */
/*        and sends the file data to Gateway.  Repeats this step until    */
/*        all the file information is read.                               */
/*                                                                        */
/* i/o  = str:char *:IN: following information is stored:                 */
/* 	     :      :  :   - file descriptor                              */
/*           :      :  :   - data size to be inputted at a time           */
/* return =  0: normal end                                                */
/*          -1: abnormal end                                              */
/* note = none								  */
/* date = September 21, 1995  by Tetsuya Ichikawa                         */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved. Derechos Reservados. */
/**************************************************************************/
int agt_file_cpread(char *str)
{
	int fd;
	size_t nbytes;
	size_t restlen;
	size_t readlen;
	int fdflag;
	int datalen = 0;
	char *readbuf;
	char *workbuf;
	char emsg[8];
	
	memcpy((char *)&fd,str,sizeof(fd));
	fd = ntohl(fd);
	str += sizeof(fd);
	memcpy((char *)&nbytes,str,sizeof(nbytes));
	nbytes = ntohl(nbytes);
	
	fdflag = agt__file_fdcheck(fd);
	
	if (fdflag == 0){
		d_errno = EBADF;
		agt_usrc_send_emsg(emsg,DFAM_CPREAD_R);
		return(-1);
	}

	
	if ((readbuf = (char *)malloc(nbytes+8)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25200,(nbytes+8),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_usrc_send_emsg(emsg,DFAM_CPREAD_R);	
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25201,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}
		
	restlen = nbytes;
	workbuf = (readbuf + 8);

	while(1){
		if ((readlen = read(fd,workbuf,restlen)) == -1){
			switch(errno){
				case EINTR:
					continue;
				case EFAULT:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25202,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
				default:
					d_errno = errno;
					agt_usrc_oserr_trace(DREAD,d_errno);
					goto func_err;
			}
		}

		datalen += readlen;

		if ((readlen == restlen) || (readlen == 0)){

			memset(readbuf,0,8);
			datalen = htonl(datalen);
			memcpy(readbuf,(char *)&datalen,sizeof(datalen));
			datalen = ntohl(datalen);

			agt_usrc_send_data(readbuf,(8+datalen),DFAM_CPREAD_R);
			if (readlen == 0){
				free(readbuf);
				return(0);
			}
			datalen = 0;
			workbuf = (readbuf + 8);
			restlen = nbytes;
		}
		else {
			workbuf += readlen;
			restlen -= readlen;
		}
		continue;
	}

func_err:
	memcpy(msg.buf,(char *)&fd,4);
	memcpy(&msg.buf[4],(char *)&nbytes,4);
	memcpy(&msg.buf[8],(char *)&user_info.uid,4);
	memcpy(&msg.buf[12],(char *)&d_errno,4);
	AgentTrace(TRACEERR,"DCPRD ",16,msg.buf,0);
	agt_usrc_send_emsg(emsg,DFAM_CPREAD_R);
	free(readbuf);
	return(-1);
}

/**************************************************************************/
/* ID	= agt_file_cpwrite						  */
/*                                                                        */
/* name = Writing to files by burst mode                                  */
/* func = Writes a file the data sent from Gateway.                       */
/*                                                                        */
/* i/o  = str:char *:IN: following information is stored:                 */
/* 	     :      :  :   - file descriptor                              */
/*           :      :  :   - data size                                    */
/*           :      :  :   - data to be outputted to a file               */
/* return = none                                                          */
/* note = none                                                            */
/* date = September 21, 1995  by Tetsuya Ichikawa                         */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved. Derechos Reservados. */
/**************************************************************************/
int agt_file_cpwrite(char *str)
{
	int fd;
	size_t nbytes;
	size_t restlen;
	size_t writelen;
	int fdflag;
	
	
	if (cpwriteflag == 0){
		d_errno = 0;
		cp_write_datalen = 0;	
		cpwriteflag = 1;
	}
	else{
		if (d_errno != 0)
			return(-1);
	}
		
	memcpy((char *)&fd,str,sizeof(fd));
	fd = ntohl(fd);
	str += sizeof(fd);
	memcpy((char *)&nbytes,str,sizeof(nbytes));
	nbytes = ntohl(nbytes);
	str += sizeof(nbytes);

	fdflag = agt__file_fdcheck(fd);

	if (fdflag ==0){
		d_errno = EBADF;
		return(-1);
	}

	restlen = nbytes;

	while(1){
		if ((writelen = write(fd,str,restlen)) == -1){
			switch(errno){
				case EINTR:
					continue;
				default:
					d_errno = errno;
					agt_usrc_oserr_trace(DWRITE,d_errno);
					return(-1);
			}
		}

		if (writelen != restlen){
			str += writelen;
			restlen -= writelen;
			continue;
		}
		break;
	}

	cp_write_datalen += nbytes;
	return(0);
}
	
/**************************************************************************/
/* ID	= agt_file_cpwrite_end						  */
/*                                                                        */
/* name = Termination of the file outputting by burst mode                */
/* func = Checks if the file outputting by burst mode was successful.     */
/*                                                                        */
/* i/o  = str:char *:IN: following information is stored:                 */
/* 	     :      :  :   - file descriptor                              */
/*                                                                        */
/* return =  0: normal end                                                */
/*          -1: abnormal end                                              */
/* note = none								  */
/* date = September 21, 1995  by Tetsuya Ichikawa                         */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved. Derechos Reservados. */
/**************************************************************************/
int agt_file_cpwrite_end(char *str)
{
	int fd;
	int fdflag;
	char senddata[8];

	if (d_errno != 0)
		goto func_err;

	memcpy((char *)&fd,str,sizeof(fd));
	fd = ntohl(fd);

	fdflag = agt__file_fdcheck(fd);
	
	if (fdflag == 0){
		d_errno = EBADF;
		goto func_err;
	}

	memset(senddata,0,8);
	cp_write_datalen = htonl(cp_write_datalen);
	memcpy(senddata,(char *)&cp_write_datalen,sizeof(cp_write_datalen));
	
	agt_usrc_send_data(senddata,8,DFAM_CPWRITEEND_R);
	cpwriteflag = 0;
	return(0);	

func_err:
	memcpy(msg.buf,(char *)&user_info.uid,4);
	memcpy(&msg.buf[4],(char *)&d_errno,4);
	AgentTrace(TRACEERR,"DCPWRE",8,msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_CPWRITEEND_R);
	cpwriteflag = 0;
	return(-1);
}
					
/*****************************************************************************/
/* ID	= agt__file_context_delete				             */	
/* name = Deleting context handle					     */
/* func = This function delete handle of sec_rgy_site_open.		     */
/* i/o  = context:sec_rgy_handle_t *:IN:An opaque handle indicating  	     */
/*                                     :a registry server.                   */
/* return = 0: normal end      						     */
/*         -1: abnormal end                                                  */
/* note = none							             */	
/* date = October 20, 1995 by Tetsuya Ichikawa				     */
/* Trade Secret of Hitachi Ltd.						     */
/* Do not disclose without written concession from Hitachi, Ltd.             */	
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved. Derechos Reservados.    */ 
/*****************************************************************************/
static int agt__file_context_delete(sec_rgy_handle_t *context)
{
	sec_rgy_site_close(*context,&status);
	if (status != error_status_ok){
		switch(status){
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECACLSITECLOSE,DEUNAVAILSRV);
				return(-1);
				break;
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25209,status,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}
	return(0);
}
