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
static char *RCSID_agt_login_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_login.c,v 1.1.2.5 1996/03/12 02:36:56 takano Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_login.c,v $
 * Revision 1.1.2.5  1996/03/12  02:36:56  takano
 * 	Delete 2byte code.
 * 	[1996/03/12  02:36:19  takano]
 *
 * Revision 1.1.2.4  1996/03/09  20:44:06  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:02  marty]
 * 
 * Revision 1.1.2.3  1996/03/04  01:43:12  takano
 * 	Delete MergePart
 * 	[1996/03/04  01:42:44  takano]
 * 
 * Revision 1.1.2.2  1996/02/16  06:23:21  dce-sdc
 * 	Merged with changes from 1.1.2.1
 * 	[1996/02/16  06:23:15  dce-sdc]
 * 
 * 	First Submit
 * 	[1996/02/16  06:06:04  dce-sdc]
 * 
 * Revision 1.1.2.10  96/01/12  04:36:18  04:36:18  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:36:10  kishi]
 * 
 * Revision 1.1.2.9  1995/11/13  07:53:45  takano
 * 	Mistake(Error Check).
 * 	[1995/11/02  07:48:04  takano]
 * 
 * Revision 1.1.2.8  1995/11/01  08:04:08  takano
 * 	Error Check.
 * 	[1995/11/01  08:03:51  takano]
 * 
 * Revision 1.1.2.7  1995/10/26  00:53:11  kishi
 * 	Change Copyright.
 * 	[1995/10/26  00:52:48  kishi]
 * 
 * Revision 1.1.2.6  1995/10/13  09:09:29  kishi
 * 	Add header comment.
 * 	[1995/10/13  09:09:18  kishi]
 * 
 * Revision 1.1.2.5  1995/09/28  10:58:48  takano
 * 	Comment out "Check Masterkey".
 * 	[1995/09/28  10:58:04  takano]
 * 
 * Revision 1.1.2.4  95/09/27  01:19:45  01:19:45  kishi (Y.Kishi)
 * 	Change header comment of LinGetId().
 * 	[1995/09/27  01:19:33  kishi]
 * 
 * Revision 1.1.2.3  1995/09/23  09:52:34  kishi
 * 	Add header comment.
 * 	[1995/09/23  08:42:41  kishi]
 * 
 * Revision 1.1.2.2  1995/09/22  02:06:21  ichikawa
 * 	  Add error handler for sec_rgy_object_not_found.
 * 	[1995/09/22  02:06:07  ichikawa]
 * 
 * Revision 1.1.2.1  1995/09/09  06:52:33  kishi
 * 	Initial revision
 * 	[1995/09/09  06:35:12  kishi]
 * 
 * Revision 1.1.2.1  1996/02/16  06:06:32  dce-sdc
 * 	First Submit
 * 	[1996/02/16  06:06:04  dce-sdc]
 * 
 * Revision 1.1.2.10  96/01/12  04:36:18  04:36:18  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:36:10  kishi]
 * 
 * Revision 1.1.2.9  1995/11/13  07:53:45  takano
 * 	Mistake(Error Check).
 * 	[1995/11/02  07:48:04  takano]
 * 
 * Revision 1.1.2.8  1995/11/01  08:04:08  takano
 * 	Error Check.
 * 	[1995/11/01  08:03:51  takano]
 * 
 * Revision 1.1.2.7  1995/10/26  00:53:11  kishi
 * 	Change Copyright.
 * 	[1995/10/26  00:52:48  kishi]
 * 
 * Revision 1.1.2.6  1995/10/13  09:09:29  kishi
 * 	Add header comment.
 * 	[1995/10/13  09:09:18  kishi]
 * 
 * Revision 1.1.2.5  1995/09/28  10:58:48  takano
 * 	Comment out "Check Masterkey".
 * 	[1995/09/28  10:58:04  takano]
 * 
 * Revision 1.1.2.4  95/09/27  01:19:45  01:19:45  kishi (Y.Kishi)
 * 	Change header comment of LinGetId().
 * 	[1995/09/27  01:19:33  kishi]
 * 
 * Revision 1.1.2.3  1995/09/23  09:52:34  kishi
 * 	Add header comment.
 * 	[1995/09/23  08:42:41  kishi]
 * 
 * Revision 1.1.2.2  1995/09/22  02:06:21  ichikawa
 * 	Add error handler for sec_rgy_object_not_found.
 * 	[1995/09/22  02:06:07  ichikawa]
 * 
 * Revision 1.1.2.1  1995/09/09  06:52:33  kishi
 * 	Initial revision
 * 	[1995/09/09  06:35:12  kishi]
 * 
 * $EndLog$
 */

#include <pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<memory.h>
#include<syslog.h>
#include<errno.h>
#include<dirent.h>
#include<pwd.h>
#include<netdb.h>
#include<signal.h>

#include<dce/sec_login.h>

#include "agt_agent.h"
#include "agt_timer.h"
#include "agt_msg.h"
#include "agt_usrc.h"
#include "agt_trc.h"
#include "agt_ipc.h"
#include "agt_login.h"
#include "agt_crypt.h"
#include "agt_sck.h"
#include "agt_errno.h"
#include "agt_errid.h"

#define LOOP 8

sec_login_handle_t 	login_context;
struct d_login_info	user_info;
struct d_crypt_info	crypt_info;
struct agt_timer	al_timep;
int			al_errno;
int                     con_errno;
char			al_trc[D_AL_TRC_SIZE];
char			status_data[D_RESP_LEN];
long		 	TGTRefreshTime;

static int	DCELogin(char *login_name, char *passwd);
static int	DCELogout(void);
static int	PutDfamKey(char *gateway, unsigned long key[]);
static int	GetDfamKey(char *gateway, unsigned long mkey[]);
static void	Creat64bitKey(char *buf);


void		ResetTGT(void);
static void	LOGINTrace(int status);
static void	LOGOUTTrace(int status);
static void	SECAPITrace(char *api, int status);
static off_t 	setup_client_msg(char *gwmsg, int type, int code, int client[], int num);
static void 	DumFunc(char *data, unsigned long datalen, unsigned long key[]);

struct d_crypt_tbl crypt_tbl[] = {
	{0x00000000, DumFunc, DumFunc},
};

/**************************************************************************/
/* ID 	= LinGetId							  */
/* name = Log in process(1)						  */
/* func = This is the first function to be called during the login.       */
/*        Get the master key corresponding to the Distributed File Access */
/*        Manager gateway which made the request. The master key is taken */
/*        from the master key file. Get user-ID from master key and       */
/*        de-encrypt together with random number rand1. Create random     */
/*        number rand2 and encrypt with rand1. The encrypted result is    */
/*        sent to gateway                                                 */ 
/* i/o =								  */
/* 	data:char *:IN:server name and corresponding encrypted user-ID and*/
/*                 random number rand1					  */
/* return = 								  */
/*	0x00041000:normal end						  */
/*	0x00042000:abnormal end						  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int LinGetId(register char *data)
{
	int		status[2];
	int 		rtn_datalen;
	char 		*gwname;
	short 		datalen;
	unsigned long	cryptlen;
	unsigned long 	mkey[2];
	register char	*cp;
	struct hostent 	*hp;
	struct hostent 	hst;

	TGTRefreshTime = 600;
	al_errno = 0;

	memset((char *)&user_info, 0, sizeof(user_info));
	user_info.conn = sck.gwconnid;


	data += 4;


	memcpy((char *)&datalen, data, sizeof(datalen));
	datalen = ntohl(datalen);
	data += sizeof(datalen);


	if((hp = gethostbyname(sck.gwname)) == NULL){
		if(errno == ECONNREFUSED){
			al_errno = DEINVALGTWY;
			goto err_rtn;
		}
		else{
			sprintf(msg.wkbuf, "%d, %d", D_EID_26000, errno);
			if(memcmp(agt.proctype, "USRM", 4) == 0){
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			}
			else{
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			}
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
	        }
	}

	memcpy((char *)&hst, (char *)hp, sizeof(struct hostent));


	gwname = hst.h_name;

	while(gwname != NULL){

		if(!GetDfamKey(gwname, mkey)){


			strcat(user_info.nwsrvname, gwname);


			if(datalen%D_KEY_SIZE){
				cryptlen = datalen / D_KEY_SIZE;
				cryptlen = D_KEY_SIZE * (cryptlen + 1);
			}
			else
				cryptlen = datalen;


			(*crypt_info.decrypt)(data, cryptlen, mkey);


			datalen -= D_KEY_SIZE;
			strncat(user_info.uname, data, (unsigned long)datalen);
			data += datalen;


			memcpy((char *)user_info.nextkey, data, D_KEY_SIZE);


			Creat64bitKey((char *)user_info.randval);


			user_info.randval[0] = htonl(user_info.randval[0]);
			user_info.randval[1] = htonl(user_info.randval[1]);

			status[0] = htonl(D_LOGIN_OK);
			if (trnserr(&al_errno,&con_errno) == FALSE){
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26059,al_errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",sizeof(agt.proctype)) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			}
			status[1] = htonl(con_errno);
			rtn_datalen = htonl(2*D_KEY_SIZE);


			cp = status_data;
			memcpy(cp, (char *)status, sizeof(status));
			cp += sizeof(status);
			memcpy(cp, (char *)&rtn_datalen, sizeof(rtn_datalen));
			cp += sizeof(rtn_datalen);
			memcpy(cp, (char *)user_info.nextkey, D_KEY_SIZE);
			cp += D_KEY_SIZE;
			memcpy(cp, (char *)user_info.randval, D_KEY_SIZE);


			user_info.nextkey[0] = ntohl(user_info.nextkey[0]);
			user_info.nextkey[1] = ntohl(user_info.nextkey[1]);


			(*crypt_info.encrypt)(status_data + 12, D_KEY_SIZE * 2, user_info.nextkey);


			agt_usrc_send_data(status_data, (D_KEY_SIZE * 2 + 12), DFAM_LinGetId_R);


			user_info.randval[0] = ntohl(user_info.randval[0]);
			user_info.randval[1] = ntohl(user_info.randval[1]);

			return(D_MAN_ST_LGP);
		}
		gwname = *(hst.h_aliases);
		(hst.h_aliases) ++;
	}

err_rtn:


	status[0] = htonl(D_LOGIN_ERROR);
	if (trnserr(&al_errno,&con_errno) == FALSE){
		sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26060,al_errno,user_info.uid);
		if (memcmp(agt.proctype,"USRM",sizeof(agt.proctype)) == 0)
			sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
		else
			sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
		AgentMsg(LOG_ERR,msg.buf);
		agt_abort();
	}
	status[1] = htonl(con_errno);

	agt_usrc_send_data((char *)status, sizeof(status), DFAM_LinGetId_R);

	LOGINTrace(al_errno);
	return(D_MAN_ST_EXIT);
}


/**************************************************************************/
/* ID =	LinGetPasswd						    	  */
/* name = Log in process(2)						  */
/* func = This is the second function to be called during login. 	  */
/*        Encrypt password and random number rand2 based on rand2. Compare*/
/*        rand2 with original number and if it matches, acknoledge the    */
/*        gateway. Perform the DCE login. The result is sent back to      */
/*        gateway							  */
/* i/o =								  */
/*	data:char *:IN:encrypted rand2 and password			  */
/* return =								  */
/*	0x00044000:normal end						  */
/*	0x00042000:abnormal end						  */	
/* note = It is assumed that LinGetID had been executed before to allocate*/
/*        area for DCE user-ID and rand2				  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int LinGetPasswd(register char *data)
{
	int 			status[2];
	unsigned long 		datalen;
	unsigned long		randkeyval[2];
	short 			pswdlen;
	char 			gwmsg[D_GWMSG_MAX];
 	off_t			oft;
	struct timeval 		ts;
	long			interval;

	al_errno = 0;
	memcpy((char *)&datalen, data, sizeof(datalen));
	datalen = ntohl(datalen);
	data += sizeof(datalen);


	(*crypt_info.decrypt)(data, datalen, user_info.randval);


	memcpy((char *)randkeyval, data, D_KEY_SIZE);
	data += D_KEY_SIZE;

	randkeyval[0] = ntohl(randkeyval[0]);
	randkeyval[1] = ntohl(randkeyval[1]);

	memcpy((char *)&pswdlen, data, sizeof(pswdlen));
	pswdlen = ntohl(pswdlen);
	data += sizeof(pswdlen);


	strncat(user_info.passwd, data, (unsigned long)pswdlen);


	if(pswdlen%D_KEY_SIZE){
		pswdlen = pswdlen / D_KEY_SIZE;
		pswdlen = D_KEY_SIZE * (pswdlen + 1);
	}
	user_info.pswdlen = pswdlen;


	memset(data, 0, pswdlen);

/*
	if(!memcmp((char *)randkeyval, (char *)user_info.randval, D_KEY_SIZE)){
*/
		if(!DCELogin(user_info.uname, user_info.passwd)){
			status[0] = htonl(D_LOGIN_OK);
			if (trnserr(&al_errno,&con_errno) == FALSE){
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26061,al_errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",sizeof(agt.proctype)) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			}
			status[1] = htonl(con_errno);

			if(!gettimeofday(&ts, (struct timezone *)NULL)){
				interval = user_info.TGTlimit - ts.tv_sec - TGTRefreshTime;
				if(interval <= 0){



					oft = setup_client_msg(gwmsg, 4, 1, &(user_info.conn), 1);
					GatewayMsg(oft, gwmsg, 0);


					oft = setup_client_msg(gwmsg, 4, 2, &(user_info.conn), 1);
					memcpy(gwmsg + oft, (char *)&user_info.TGTlimit, sizeof(user_info.TGTlimit));
					GatewayMsg(20, gwmsg, 0);


					oft = setup_client_msg(gwmsg, 4, 4, &(user_info.conn), 1);
					GatewayMsg(oft, gwmsg, 0);

					sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02250, KDDS02250), user_info.uname, ctime(&user_info.TGTlimit), "too small TGT period");

					AgentMsg(LOG_ERR, msg.buf);

				}
				else{

					al_timep.timer_func = ResetTGT;
					al_timep.seconds = (unsigned long)interval;
					AgentSetTimer(&al_timep);
				}
			}
			else{
				sprintf(msg.wkbuf, "%d, %d", D_EID_26001, errno);
				if(memcmp(agt.proctype, "USRM", 4) == 0){
				        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
				}
				else{
				        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
				}
				AgentMsg(LOG_ERR, msg.buf);
				agt_abort();
			}

			agt_usrc_send_data((char *)status,
					   sizeof(status),
					   DFAM_LinGetPasswd_R);
			return(D_MAN_ST_LRK);
		}
/*
	}
	else
		al_errno = DEINVALGTWY;
*/

	if (trnserr(&al_errno,&con_errno) == FALSE){
		sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26062,al_errno,user_info.uid);
		if (memcmp(agt.proctype,"USRM",sizeof(agt.proctype)) == 0)
			sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
		else
			sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
		AgentMsg(LOG_ERR,msg.buf);
		agt_abort();
	}
	status[1] = htonl(con_errno);
	status[0] = htonl(D_LOGIN_ERROR);


	agt_usrc_send_data((char *)status, sizeof(status), DFAM_LinGetPasswd_R);

	LOGINTrace(al_errno);


	return(D_MAN_ST_EXIT);
}

/**************************************************************************/
/* ID	= LinResetKey							  */
/* name = Log in process(3)						  */
/* func = This is the third function to be called during login.	Change    */
/*        master key to rand1 and send the result to gateway. DCE login   */
/*        is completed when this function is executed                     */
/* i/o = none								  */
/* return =								  */
/*	0x00048000:normal end						  */
/*	0x00042000:abnormal end						  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int LinResetKey()
{
	int 	status[2];
	int     uxmsglen;
	char    uxmsg[D_AL_UXMSG_MAX];

	al_errno = 0;


	if(!PutDfamKey(user_info.nwsrvname, user_info.nextkey)){
		status[0] = htonl(D_LOGIN_OK);
		if (trnserr(&al_errno,&con_errno) == FALSE){
			sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26063,al_errno,user_info.uid);
			if (memcmp(agt.proctype,"USRM",sizeof(agt.proctype)) == 0)
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
			else
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
			AgentMsg(LOG_ERR,msg.buf);
			agt_abort();
		}
		status[1] = htonl(con_errno);
		memset(user_info.nextkey, 0, D_KEY_SIZE);


		agt_usrc_send_data((char *)status, sizeof(status), DFAM_LinResetKey_R);


		uxmsglen = strlen(user_info.uname) + 5;
		memset(uxmsg, 0, 3);
		memset(uxmsg + 3, '\x0a', 1);
		memcpy(uxmsg + 4, (char *)&uxmsglen, 4);
		memcpy(uxmsg + 8, (char *)&agt.ownpid, 4);
		strcpy(uxmsg + 12, user_info.uname);

		uxmsglen += 8;

		if(sendto(sck.ux_fd, uxmsg, uxmsglen,
		0, &sck.sysc_adr, sck.sysc_adrlen)!= uxmsglen){
			sprintf(msg.wkbuf, "%d, %d", D_EID_26002, errno);
			if(memcmp(agt.proctype, "USRM", 4) == 0){
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			}
			else{
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			}
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		}
		return(D_MAN_ST_FA);
	}

	status[0] = htonl(D_LOGIN_ERROR);
	if (trnserr(&al_errno,&con_errno) == FALSE){
		sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26064,al_errno,user_info.uid);
		if (memcmp(agt.proctype,"USRM",sizeof(agt.proctype)) == 0)
			sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
		else
			sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
		AgentMsg(LOG_ERR,msg.buf);
		agt_abort();
	}
	status[1] = htonl(con_errno);


	agt_usrc_send_data((char *)status, sizeof(status), DFAM_LinResetKey_R);

	LOGINTrace(al_errno);
	return(D_MAN_ST_EXIT);
}

/**************************************************************************/
/* ID	= Lout								  */
/* name = Logout DCE							  */
/* func = Logout from DCE and send the result to gateway		  */
/* i/o	= none								  */
/* return =								  */
/*	0x00046000:normal end						  */
/*	0x00042000:abnormal end						  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int Lout(void)
{
	int 			status[2];

	al_errno = 0;


	(void)AgentResetTimer(&al_timep);

	if(!DCELogout()){
		status[0] = htonl(D_LOGIN_OK);
		if (trnserr(&al_errno,&con_errno) == FALSE){
			sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26065,al_errno,user_info.uid);
			if (memcmp(agt.proctype,"USRM",sizeof(agt.proctype)) == 0)
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
			else
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
			AgentMsg(LOG_ERR,msg.buf);
			agt_abort();
		}
		status[1] = htonl(con_errno);

		agt_usrc_send_data((char *)status, sizeof(status), DFAM_Lout_R);

		return(D_MAN_ST_LGOUT);
	}

	status[0] = htonl(D_LOGIN_ERROR);
	if (trnserr(&al_errno,&con_errno) == FALSE){
		sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26066,al_errno,user_info.uid);
		if (memcmp(agt.proctype,"USRM",sizeof(agt.proctype)) == 0)
			sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
		else
			sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
		AgentMsg(LOG_ERR,msg.buf);
		agt_abort();
	}
	status[1] = htonl(con_errno);

	agt_usrc_send_data((char *)&status, sizeof(status), DFAM_Lout_R);

	LOGOUTTrace(al_errno);
	return(D_MAN_ST_EXIT);
}

/**************************************************************************/
/* ID	= Creat64bitKey							  */
/* name = Create random number						  */
/* func = Create a 64 bit random key					  */
/* i/o =								  */
/*	buf:char *:OUT:random key					  */
/* return = none							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void Creat64bitKey(char *buf)
{
	struct timeval 	ts;
	long 		seed;
	long 		lval;


	if(gettimeofday(&ts, (struct timezone *)NULL)){
		sprintf(msg.wkbuf, "%d, %d", D_EID_26003, errno);
		if(memcmp(agt.proctype, "USRM", 4) == 0){
		        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
		}
		else{
		        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
		}
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
	}

	seed = ts.tv_usec;


	srand48(seed);

	lval = mrand48();
	memcpy(buf, (char *)&lval, sizeof(lval));
	lval = mrand48();
	memcpy(buf + 4, (char *)&lval, sizeof(lval));

	return;


}

/**************************************************************************/
/* ID	= GetDfamKey							  */
/* name = Getting master key						  */
/* func = Get master key from master key file				  */
/* i/o =								  */
/*	gateway:char *       :IN :gateway name				  */
/*      mkey[] :unsigned long:OUT:master key				  */
/* return =								  */
/*	0 :normal end							  */
/*	-1:abnormal end							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int GetDfamKey(char *gateway, unsigned long mkey[])
{
	int 		fd;
	int 		n;
	int 		left_length;
	int 		line_length;
	register char	*c;
	char		*keyptr;
	char		keybuf[D_KEY_SIZE*2 + 2];
	char		string[D_NWSRV_NAME_MAX];
	char		mkf_name[D_MKF_NAME_MAX];
	struct flock	flk;


	sprintf(mkf_name, "%s%s", D_MASTERKEY_FILE, agt.agent_id);
	while((fd = open(mkf_name, O_RDONLY)) < 0){
		switch(errno){
		case EINTR:

			continue;
		case ENFILE:
			sprintf(msg.wkbuf, "%d, %d", D_EID_26004, fd);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02252, KDDS02252), "open", strerror(errno), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			al_errno = errno;
			break;
		case ENOENT:

			al_errno = DENOAGTKF;
			break;
		default:
			sprintf(msg.wkbuf, "%d, %d", D_EID_26005, errno);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		}
		return(-1);
	}


	flk.l_whence = SEEK_SET;
	flk.l_start = 0;
	flk.l_len = 0;
	flk.l_type = F_RDLCK;


	while(fcntl(fd, F_SETLKW, &flk)){
		switch(errno){
		case EINTR :
			continue;
		case EMFILE :
			sprintf(msg.wkbuf, "%d, %d", D_EID_26006, errno);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02252, KDDS02252), "fcntl", strerror(errno), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			al_errno = errno;
			break;
		default :
			sprintf(msg.wkbuf, "%d, %d", D_EID_26007, errno);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		}
		return(-1);
	}
	flk.l_type = F_UNLCK;

	line_length = 0;
	c = string;
	keyptr = keybuf;
	while((n = read(fd, c, 1)) != 0){
		if(n == -1){
			switch(errno){
				case EINTR:
					continue;
				default:
					sprintf(msg.wkbuf, "%d, %d", D_EID_26008, errno);
					if(memcmp(agt.proctype, "USRM", 4) == 0)
					        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
					else
					        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
					AgentMsg(LOG_ERR, msg.buf);
					agt_abort();
			}
		}
		switch(*c){
		case '\t':
			*c = '\0';
			if(strcmp(string, gateway)){

				line_length = 0;
				c = string;
				break;
			}

			left_length = D_KEY_SIZE*2 + 1;
			while(left_length > 0){

			    	n = read(fd, keyptr, left_length);
				switch(n){
				case -1:
					switch(errno){
					case EINTR:
						continue;
					default:
						sprintf(msg.wkbuf, "%d, %d", D_EID_26009, errno);
						if(memcmp(agt.proctype, "USRM", 4) == 0)
						        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
						else
						        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
						AgentMsg(LOG_ERR, msg.buf);
						agt_abort();
					}
				case 0:
					al_errno = DEAGTKSIZE;
					break;
				default :
					keyptr += n;
					left_length -= n;
					continue;
				}

				while(fcntl(fd, F_SETLKW, &flk)){
					switch(errno){
					case EINTR :
						continue;
					default :
						sprintf(msg.wkbuf, "%d, %d", D_EID_26010, errno);
						if(memcmp(agt.proctype, "USRM", 4) == 0)
						        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
						else
						        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
						AgentMsg(LOG_ERR, msg.buf);
						agt_abort();
					}
				}
				while(close(fd)){
					switch(errno){
					case EINTR:
						continue;
					case ETIMEDOUT:
						sprintf(msg.wkbuf, "%d, %d", D_EID_26011, errno);
						sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02252, KDDS02252), "close", strerror(errno), msg.wkbuf);
						AgentMsg(LOG_ERR, msg.buf);
						break;
					default:
						sprintf(msg.wkbuf, "%d, %d", D_EID_26012, errno);
						if(memcmp(agt.proctype, "USRM", 4) == 0)
						        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
						else
						        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
						AgentMsg(LOG_ERR, msg.buf);
						agt_abort();
					}
				}
				return(-1);
			}


			sscanf(keybuf, "%08x %08x", &mkey[0], &mkey[1]);


			while(fcntl(fd, F_SETLKW, &flk)){
				switch(errno){
				case EINTR :
					continue;
				default :
					sprintf(msg.wkbuf, "%d, %d", D_EID_26013, errno);
					if(memcmp(agt.proctype, "USRM", 4) == 0)
					        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
					else
					        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
					AgentMsg(LOG_ERR, msg.buf);
					agt_abort();
				}
			}
			while(close(fd)){
				switch(errno){
				case EINTR:
					continue;
				case ETIMEDOUT:
					sprintf(msg.wkbuf, "%d, %d", D_EID_26014, errno);
					sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02252, KDDS02252), "close", strerror(errno), msg.wkbuf);
					AgentMsg(LOG_ERR, msg.buf);
					al_errno = errno;
					return(-1);
				default:
					sprintf(msg.wkbuf, "%d, %d", D_EID_26015, errno);
					if(memcmp(agt.proctype, "USRM", 4) == 0)
					        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
					else
					        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
					AgentMsg(LOG_ERR, msg.buf);
					agt_abort();
				}
			}
			return(0);
		case '\n':
			c = string;
			line_length = 0;
			break;
		default:
			c ++;
			line_length ++;
			if(line_length >= D_NWSRV_NAME_MAX){
				c = string;
				line_length = 0;
			}
			break;
		}
	}




	while(fcntl(fd, F_SETLKW, &flk)){
		switch(errno){
		case EINTR :
			continue;
		default :
			sprintf(msg.wkbuf, "%d, %d", D_EID_26016, errno);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		}
	}

	while(close(fd)){
		switch(errno){
		case EINTR:
			continue;
		case ETIMEDOUT:
			sprintf(msg.wkbuf, "%d, %d", D_EID_26017, errno);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02252, KDDS02252), "close", strerror(errno), msg.wkbuf);
			AgentMsg(LOG_NOTICE, msg.buf);
			break;
		default:
			sprintf(msg.wkbuf, "%d, %d", D_EID_26018, errno);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		}
	}

	al_errno = DENOGWENT;
	return(-1);
}

/**************************************************************************/
/* ID	= PutDfamKey							  */
/* name = Registering master key					  */
/* func = Register master key in the master key file			  */
/* i/o =								  */
/*	gateway:char *	     :IN:gateway name				  */
/*      mkey[] :unsigned long:IN:master key				  */
/* return =								  */
/*	0 :normal end							  */
/*	-1:abnormal end							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int PutDfamKey(char *gateway, unsigned long key[])
{
	int 		fd;
	int 		n;
	int		line_length;
	int		left_length;
	register char	*c;
	char		string[D_NWSRV_NAME_MAX];
	char		*keyptr;
	char		mkf_name[D_MKF_NAME_MAX];
	char		wkbuf[D_KEY_SIZE*2 + 2];
	struct flock	flk;


	sprintf(mkf_name, "%s%s", D_MASTERKEY_FILE, agt.agent_id);
	fd = open(mkf_name, O_RDWR);
	while(fd < 0){

		switch(errno){
		case EINTR:

			continue;
		case ENFILE:
			al_errno = errno;
			sprintf(msg.wkbuf, "%d, %d", D_EID_26019, fd);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02252, KDDS02252), "open", strerror(errno), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			break;
		case ENOENT:

			al_errno = DENOAGTKF;
			break;
		default:
			sprintf(msg.wkbuf, "%d, %d", D_EID_26020, errno);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
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
			al_errno = errno;
			sprintf(msg.wkbuf, "%d, %d", D_EID_26021, errno);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02252, KDDS02252), "fcntl", strerror(errno), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			break;
		default :
			sprintf(msg.wkbuf, "%d, %d", D_EID_26022, errno);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
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
				sprintf(msg.wkbuf, "%d, %d", D_EID_26023, errno);
				if(memcmp(agt.proctype, "USRM", 4) == 0)
				        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
				else
				        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
				AgentMsg(LOG_ERR, msg.buf);
				agt_abort();
			}
		}
		switch(*c){
		case '\t':
			*c = '\0';
			if(strcmp(string, gateway)){

				c = string;
				line_length = 0;
				break;
			}

			left_length = D_KEY_SIZE*2+1;
			sprintf(wkbuf, "%08x %08x", key[0], key[1]);
			keyptr = wkbuf;
			while(left_length > 0){

				n = write(fd, keyptr, left_length);
				switch(n){
				case 0:
				case -1:
					switch(errno){
					case EINTR:
						continue;
					case ENOSPC:
						al_errno = errno;
						sprintf(msg.wkbuf, "%d, %d, %d", D_EID_26024, errno, n);
						sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02252, KDDS02252), "write", strerror(errno), msg.wkbuf);
						AgentMsg(LOG_ERR, msg.buf);
						break;
					default:
						sprintf(msg.wkbuf, "%d, %d, %d", D_EID_26025, errno, n);
						if(memcmp(agt.proctype, "USRM", 4) == 0)
						        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
						else
						        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
						AgentMsg(LOG_ERR, msg.buf);
						agt_abort();
					}
					break;


				default :
					keyptr += n;
					left_length -= n;
					continue;
				}


				while(fcntl(fd, F_SETLKW, &flk)){
					switch(errno){
					case EINTR :
						continue;
					default :
						sprintf(msg.wkbuf, "%d, %d", D_EID_26026, errno);
						if(memcmp(agt.proctype, "USRM", 4) == 0)
						        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
						else
						        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);

						AgentMsg(LOG_ERR, msg.buf);
						agt_abort();
					}
				}
				while(close(fd)){
					switch(errno){
					case EINTR:
						continue;
					case ETIMEDOUT:
						sprintf(msg.wkbuf, "%d, %d", D_EID_26027, errno);
						sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02252, KDDS02252), "close", strerror(errno), msg.wkbuf);
						AgentMsg(LOG_ERR, msg.buf);
						break;
					default:
						sprintf(msg.wkbuf, "%d, %d", D_EID_26028, errno);
						if(memcmp(agt.proctype, "USRM", 4) == 0)
						        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
						else
						        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
						AgentMsg(LOG_ERR, msg.buf);
						agt_abort();
					}
				}
				return(-1);
			}


			while(fcntl(fd, F_SETLKW, &flk)){
				switch(errno){
				case EINTR :
					continue;
				default :
					sprintf(msg.wkbuf, "%d, %d", D_EID_26029, errno);
					if(memcmp(agt.proctype, "USRM", 4) == 0)
					        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
					else
					        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
					AgentMsg(LOG_ERR, msg.buf);
					agt_abort();
				}
			}
			while(close(fd)){
				switch(errno){
				case EINTR:
					continue;
				case ETIMEDOUT:
					al_errno = errno;
					sprintf(msg.wkbuf, "%d, %d", D_EID_26030, errno);
					sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02252, KDDS02252), "close", strerror(errno), msg.wkbuf);
					AgentMsg(LOG_ERR, msg.buf);
					return(-1);
				default:
					sprintf(msg.wkbuf, "%d, %d", D_EID_26031, errno);
					if(memcmp(agt.proctype, "USRM", 4) == 0)
					        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
					else
					        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);

					AgentMsg(LOG_ERR, msg.buf);
					agt_abort();
				}
			}
			return(0);
		case '\n':
			c = string;
			line_length = 0;
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


	al_errno = DENOGWENT;

	while(fcntl(fd, F_SETLKW, &flk)){
		switch(errno){
		case EINTR :
			continue;
		default :
			sprintf(msg.wkbuf, "%d, %d", D_EID_26032, errno);

			if(memcmp(agt.proctype, "USRM", 4) == 0)
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		}
	}

	while(close(fd)){
		switch(errno){
		case EINTR:
			continue;
		case ETIMEDOUT:
			sprintf(msg.wkbuf, "%d, %d", D_EID_26033, errno);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02252, KDDS02252), "close", strerror(errno), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			break;
		default:
			sprintf(msg.wkbuf, "%d, %d", D_EID_26034, errno);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		}
		break;
	}
	return(-1);
}


/*****************************************************************************/
/* ID   = DumFunc                                                            */
/*                                                                           */
/* name = Dummy function                                                     */
/* func = Dummy function, that does not encrypt or decrypt the data, but     */
/*        simply returns to the caller without doing nothing at all.         */
/*                                                                           */
/* name = Encryption of a random number                                      */
/* func = Encrypts the random number designated by "data."                   */
/*                                                                           */
/* i/o  = data   :char *        :I/O: random number to be encrypted          */
/*        datalen:unsigned long :INP: size of the data                       */
/*        key[]  :unsigned long :INP: key to be used to encrypt/decrypt the  */
/*                                    random number                          */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Kousuke Takano                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static void DumFunc(char *data, unsigned long datalen, unsigned long key[])
{
	return;
}


/**************************************************************************/
/* ID 	= DCELogin							  */
/* name = DCE login							  */
/* func = Login DCE using login name and password			  */
/* i/o =								  */
/*	login_name:char *:IN:login name					  */
/*	passed    :char *:IN:password					  */
/* return =								  */
/*	0 :normal end							  */
/*	-1:abnormal end							  */
/* note = Send message to gateway if the time validity has expired	  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int DCELogin(char *login_name, char *passwd)
{
	sec_login_auth_src_t 	auth_src;
	sec_passwd_rec_t    	pwd_rec;
	boolean32 		r_passwd;
	boolean32 		rval;
	signed32		expire_time;
	error_status_t		status;
	struct d_passwd		*pwd;
	char 			gwmsg[D_GWMSG_MAX];
	char 			pswdbuf[D_PASSWD_MAX];
 	off_t			oft;
	int 			msgcode;

	al_errno = 0;

	strcpy(pswdbuf, user_info.passwd);

	(*crypt_info.encrypt)(user_info.passwd, user_info.pswdlen, user_info.randval);


	pwd_rec.key.key_type = sec_passwd_plain;
	pwd_rec.key.tagged_union.plain = (unsigned char *)pswdbuf;
        pwd_rec.pepper = NULL;
        pwd_rec.version_number = sec_passwd_c_version_none;


	sec_login_setup_identity((unsigned_char_p_t)login_name,
					        	sec_login_no_flags,
							&login_context,
							&status);

	if(status != error_status_ok){
		SECAPITrace("sec_login_setup_identity", status);
		switch(status){
		case sec_rgy_object_not_found :
			al_errno = DENOACNT;
			break;
		case sec_rgy_server_unavailable :
			al_errno = DEUNAVAILSRV;
			break;
		case sec_login_s_no_memory :
			al_errno = DENOMEM;
			break;
		default :
			al_errno = DEUNKNOWN;

			sprintf(msg.wkbuf, "%d, %d", D_EID_26035, status);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			break;
		}
		return(-1);
	}

	rval = sec_login_valid_and_cert_ident(login_context,
						  &pwd_rec,
						  &r_passwd,
						  &auth_src,
						  &status);

	if(!rval){
		SECAPITrace("sec_login_valid_and_cert_ident", status);
		switch(status){
		case sec_rgy_server_unavailable :
			al_errno = DEUNAVAILSRV;
			break;
		case sec_rgy_passwd_invalid :
			al_errno = DEINVALPSWD;
			break;
		case sec_login_s_null_password :
			al_errno = DENULLPSWD;
			break;
		case sec_login_s_privileged :
			al_errno = DEPERM;
			break;
		case sec_login_s_acct_invalid:
			al_errno = DEINVALACNT;
			break;
		case sec_rgy_object_not_found:
			al_errno = DENOACNT;
			break;
		case sec_login_s_default_use :
		case sec_login_s_already_valid :
		case sec_login_s_unsupp_passwd_type :
			sprintf(msg.wkbuf,"%d, %d", D_EID_26036, status);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		default :
			al_errno = DEUNKNOWN;

			sprintf(msg.wkbuf,"%d, %d", D_EID_26037, status);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			break;
		}
		DCELogout();
		return(-1);
	}

	if(auth_src != sec_login_auth_src_network){
		al_errno = DELOCAL;
		DCELogout();
		return(-1);
	}

	if(r_passwd){

		msgcode = 9;
		oft = setup_client_msg(gwmsg, 4, msgcode, &(user_info.conn), 1);
		GatewayMsg(oft, gwmsg, 0);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02251, KDDS02251), user_info.uname);
		AgentMsg(LOG_NOTICE, msg.buf);
	}

	sec_login_set_context(login_context, &status);
	if(status != error_status_ok){
		SECAPITrace("sec_login_set_context", status);
		switch(status){
		case sec_login_s_default_use :
		case sec_login_s_context_invalid :
			sprintf(msg.wkbuf,"%d, %d",D_EID_26038, status);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
		       		 sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
		       		 sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			DCELogout();
			agt_abort();
		default :
			al_errno = DEUNKNOWN;

			sprintf(msg.wkbuf,"%d, %d",D_EID_26039, status);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			break;
		}
		DCELogout();
		return(-1);
	}


	sec_login_get_pwent(login_context, (void *)&pwd, &status);

	if(status != error_status_ok){
		SECAPITrace("sec_login_get_pwent", status);
		switch(status){
		case sec_login_s_not_certified :
		case sec_login_s_info_not_avail :
		case sec_login_s_default_use :
		case sec_login_s_context_invalid :
			sprintf(msg.wkbuf,"%d, %d", D_EID_26040, status);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
		       		 sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
		       		 sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			DCELogout();
			agt_abort();
		default :
			al_errno = DEUNKNOWN;

			sprintf(msg.wkbuf,"%d, %d", D_EID_26041, status);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			break;
		}
		DCELogout();
		return(-1);
	}


	user_info.linflag = 1;


	user_info.uid = pwd->pw_uid;


	sec_login_get_expiration(login_context, &expire_time, &status);

	if(status != error_status_ok){
		SECAPITrace("sec_login_get_expiration", status);
		switch(status){
		case sec_login_s_not_certified :
		case sec_login_s_context_invalid :
		case sec_login_s_default_use :
		case sec_login_s_no_current_context:
			sprintf(msg.wkbuf,"%d, %d", D_EID_26042, status);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
		       		 sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
		       		 sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		default :
			al_errno = DEUNKNOWN;

			sprintf(msg.wkbuf,"%d, %d", D_EID_26043, status);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			break;
		}
		DCELogout();
		return(-1);
	}

	user_info.TGTlimit = (unsigned long)expire_time;

	return(0);
}

/**************************************************************************/
/* ID 	= DCELogout							  */
/* name = Logout DCE							  */
/* func = DCE logout							  */
/* i/o = none								  */
/* return =								  */
/*	0 :normal end							  */
/*	-1:abnormal end							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int DCELogout(void)
{
	error_status_t 	status;

	sec_login_purge_context(&login_context, &status);
	if(status == error_status_ok){

		user_info.linflag = 0;
		return(0);
	}

	SECAPITrace("sec_login_purge_context", status);
	switch(status){
	case sec_login_s_context_invalid :
	case sec_login_s_default_use :
		sprintf(msg.wkbuf,"%d, %d", D_EID_26044, status);
		if(memcmp(agt.proctype, "USRM", 4) == 0)
	       		 sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
		else
	       		 sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
	default :
		al_errno = DEUNKNOWN;

		sprintf(msg.wkbuf,"%d, %d", D_EID_26045, status);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		break;
	}
	return(-1);
}

/**************************************************************************/
/* ID	= ResetTGT							  */
/* name = Update ticket							  */
/* func = Update expire date of ticket					  */
/* i/o = none								  */
/* return =	none						 	  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void ResetTGT(void)
{
	sec_login_auth_src_t 	auth_src;
	boolean32 		r_passwd;
	signed32		expire_time;
	sec_passwd_rec_t    	pwd_rec;
	error_status_t 		status;
	struct timeval 		ts;
	off_t 			oft;
	int 			msgcode;
	char 			pswdbuf[D_PASSWD_MAX];
	char 			gwmsg[D_GWMSG_MAX];
	long			interval;

	sec_login_refresh_identity(login_context, &status);

	if(status != error_status_ok){
		SECAPITrace("sec_login_refresh_identity", status);
		switch(status){
		case sec_rgy_server_unavailable :
		case sec_rgy_object_not_found :
			msgcode = 3;
			break;
		case sec_login_s_context_invalid :
			msgcode = 7;
			break;
		case sec_login_s_default_use :
			sprintf(msg.wkbuf,"%d, %d", D_EID_26046, status);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		default :

			sprintf(msg.wkbuf,"%d, %d", D_EID_26047, status);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			msgcode = 8;
			break;
		}

		oft = setup_client_msg(gwmsg, 4, 1, &(user_info.conn), 1);
		GatewayMsg(oft, gwmsg, 0);


		oft = setup_client_msg(gwmsg, 4, 2, &(user_info.conn), 1);
		memcpy(gwmsg + oft, (char *)&user_info.TGTlimit, sizeof(user_info.TGTlimit));
		GatewayMsg(oft + 4, gwmsg, 0);


		oft = setup_client_msg(gwmsg, 4, msgcode, &(user_info.conn), 1);
		GatewayMsg(oft, gwmsg, 0);

		sprintf(msg.wkbuf,"sec_login_refresh_identity:status=%d", status);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02250, KDDS02250), user_info.uname, ctime(&user_info.TGTlimit), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		return;
	}


	(*crypt_info.decrypt)(user_info.passwd, user_info.pswdlen, user_info.randval);
	strcpy(pswdbuf, user_info.passwd);

	(*crypt_info.encrypt)(user_info.passwd, user_info.pswdlen, user_info.randval);


	pwd_rec.key.key_type = sec_passwd_plain;
	pwd_rec.key.tagged_union.plain = (unsigned char *)pswdbuf;
        pwd_rec.pepper = NULL;
        pwd_rec.version_number = sec_passwd_c_version_none;

	sec_login_valid_and_cert_ident(login_context,
					&pwd_rec,
					&r_passwd,
					&auth_src,
					&status);

	if(status != error_status_ok){
		SECAPITrace("sec_login_valid_and_cert_ident", status);
		switch(status){
		case sec_rgy_server_unavailable :
		case sec_rgy_object_not_found:
			msgcode = 3;
			break;
		case sec_rgy_passwd_invalid :
			msgcode = 5;
			break;
		case sec_login_s_acct_invalid:
			msgcode = 6;
			break;
		case sec_login_s_privileged :
		case sec_login_s_unsupp_passwd_type :
		case sec_login_s_null_password :
		case sec_login_s_default_use :
		case sec_login_s_already_valid :
			sprintf(msg.wkbuf,"%d, %d", D_EID_26048, status);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		default :
			msgcode = 8;

			sprintf(msg.wkbuf,"%d, %d", D_EID_26049, status);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			break;
		}

		oft = setup_client_msg(gwmsg, 4, 1, &(user_info.conn), 1);
		GatewayMsg(oft, gwmsg, 0);


		oft = setup_client_msg(gwmsg, 4, 2, &(user_info.conn), 1);
		memcpy(gwmsg + oft, (char *)&user_info.TGTlimit, sizeof(user_info.TGTlimit));
		GatewayMsg(oft + 4, gwmsg, 0);


		oft = setup_client_msg(gwmsg, 4, msgcode, &(user_info.conn), 1);
		GatewayMsg(oft, gwmsg, 0);

		sprintf(msg.wkbuf,"sec_login_valid_and_cert_ident:status=%d", status);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02250, KDDS02250), user_info.uname, ctime(&user_info.TGTlimit), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		return;
	}


	sec_login_get_expiration(login_context,
					&expire_time,
					&status);
	if(status != error_status_ok){
		SECAPITrace("sec_login_get_expiration", status);
		switch(status){
		case sec_login_s_not_certified :
		case sec_login_s_context_invalid :
		case sec_login_s_default_use :
			sprintf(msg.wkbuf,"%d, %d", D_EID_26050, status);
			if(memcmp(agt.proctype, "USRM", 4) == 0)
		       		 sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
			else
		       		 sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		default :
			msgcode = 8;

			sprintf(msg.wkbuf,"%d, %d", D_EID_26051, status);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			break;
		}

		oft = setup_client_msg(gwmsg, 4, 1, &(user_info.conn), 1);
		GatewayMsg(oft, gwmsg, 0);


		oft = setup_client_msg(gwmsg, 4, 2, &(user_info.conn), 1);
		memcpy(gwmsg + oft, (char *)&user_info.TGTlimit, sizeof(user_info.TGTlimit));
		GatewayMsg(oft + 4, gwmsg, 0);


		oft = setup_client_msg(gwmsg, 4, msgcode, &(user_info.conn), 1);
		GatewayMsg(oft, gwmsg, 0);

		sprintf(msg.wkbuf,"sec_login_get_expiration:status=%d", status);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02250, KDDS02250), user_info.uname, ctime(&user_info.TGTlimit), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		return;

	}


	if(gettimeofday(&ts, (struct timezone *)NULL)){
		sprintf(msg.wkbuf,"%d, %d", D_EID_26052, status);
		if(memcmp(agt.proctype, "USRM", 4) == 0)
		        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02032, KDDS02032), msg.wkbuf);
		else
		        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
	}


	interval = expire_time - ts.tv_sec - TGTRefreshTime;
	if(interval <= 0){

		sprintf(msg.wkbuf,"%d, %d", D_EID_26045, status);


		oft = setup_client_msg(gwmsg, 4, 1, &(user_info.conn), 1);
		GatewayMsg(oft, msg.buf, 0);


		oft = setup_client_msg(gwmsg, 4, 2, &(user_info.conn), 1);
		memcpy(gwmsg + oft, (char *)&user_info.TGTlimit, sizeof(user_info.TGTlimit));
		GatewayMsg(20, msg.buf, 0);


		oft = setup_client_msg(gwmsg, 4, 4, &(user_info.conn), 1);
		GatewayMsg(oft, msg.buf, 0);

		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02250, KDDS02250), user_info.uname, ctime(&user_info.TGTlimit), "too small TGT period");
		AgentMsg(LOG_ERR, msg.buf);
	}
	else{

		al_timep.seconds = (unsigned long)interval;
		al_timep.timer_func = ResetTGT;
		AgentSetTimer(&al_timep);
	}


	user_info.TGTlimit = (unsigned long)expire_time;
	return;
}

/**************************************************************************/
/* ID	= LOGINTrace							  */
/* name = Trace login process						  */
/* func = Get login information about login process			  */
/* i/o =								  */
/*	status:int:IN:error code					  */
/* return =	none						 	  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void LOGINTrace(int status)
{
	memset(al_trc, 0, D_AL_TRC_SIZE);

	memcpy(al_trc, user_info.uname, strlen(user_info.uname));
	memcpy(al_trc + strlen(user_info.uname), (char *)&status, sizeof(status));
	AgentTrace(TRACEERR,"LOGIN ", strlen(user_info.uname) + 4, al_trc, 0);
}

/**************************************************************************/
/* ID	= setup_client_msg						  */
/* name = Message assembling						  */
/* func = Assemble message to send to client				  */
/* i/o =								  */
/* 	gwmsg	     :char *:IN:message					  */
/*  type	     :int   :IN:message type				  */
/*	code         :int   :IN:message code				  */
/*	client       :int   :IN:information about client		  */
/* 	num_of_client:int   :IN:number of clients			  */
/* return =								  */
/*	4 * (3 + num_of_client)						  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static off_t setup_client_msg(char *gwmsg,
				int type,
				int code,
				int *client,
				int num_of_client)
{

	memset(gwmsg, '\0', D_GWMSG_MAX);

	memcpy(gwmsg, (char *)&type, 4);
	gwmsg += 4;
	memcpy(gwmsg, (char *)&code, 4);
	gwmsg += 4;
	memcpy(gwmsg, (char *)&num_of_client, 4);
	gwmsg += 4;
	memcpy(gwmsg, (char *)client, 4 * num_of_client);

	return(4 * (3 + num_of_client));
}


/**************************************************************************/
/* ID	= LOGOUTTrace							  */
/* name = Trace	logout process						  */
/* func = Get logout information about logout process			  */
/* i/o =								  */
/*	status:int:IN:error code					  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void LOGOUTTrace(int status)
{
	memset(al_trc, 0, D_AL_TRC_SIZE);

	memcpy(al_trc, user_info.uname, strlen(user_info.uname));
	memcpy(al_trc + strlen(user_info.uname), (char *)&status, sizeof(status));
	AgentTrace(TRACEERR,"LOGOUT", strlen(user_info.uname) + 4, al_trc, 0);
}

/**************************************************************************/
/* ID   = SECAPITrace 							  */
/* name = Trace security functions					  */
/* func = Get inforamtion about execution of security functions		  */
/* i/o =								  */
/*	api   :char *:IN:security function name				  */
/*  status:int   :IN:error code						  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void SECAPITrace(char *api, int status)
{
	memset(al_trc, 0, D_AL_TRC_SIZE);

	strcpy(al_trc, api);
	memcpy(al_trc + strlen(api), (char *)&status, sizeof(status));
	AgentTrace(TRACEERR,"SECAPI", strlen(api) + 4, al_trc, 0);
}


