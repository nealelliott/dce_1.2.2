/*
 * @HITACHI_COPYRIGHT@
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
static char *RCSID_agt_acl_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_acl.c,v 1.1.2.4 1996/04/17 17:28:02 root Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: agt_acl.c,v $
 * Revision 1.1.2.4  1996/04/17  17:28:02  root
 * 	OT 13430 Submit
 * 	[1996/04/17  17:26:36  root]
 *
 * Revision 1.1.2.28  1996/04/04  01:49:13  ichikawa
 * 	Change treatment of memory free.
 * 	[1996/04/04  01:48:58  ichikawa]
 * 
 * Revision 1.1.2.27  1996/01/24  02:44:59  ichikawa
 * 	Change from sec_id_gen_name to sec_rgy_pgo_id_to_name,and from sec_id_parse_name to sec_rgy_pgo_name_to_id.
 * 	[1996/01/24  02:44:44  ichikawa]
 * 
 * Revision 1.1.2.26  96/01/12  04:27:31  04:27:31  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:27:21  kishi]
 * 
 * Revision 1.1.2.25  1995/12/27  01:06:18  ichikawa
 * 	When DCE login user is cell_admin or root,add sec_acl_perm_control to net_rights.
 * 	[1995/12/27  01:06:07  ichikawa]
 * 
 * Revision 1.1.2.24  1995/12/21  01:51:39  ichikawa
 * 	Change errno from DENAMETOOLONG to ENAMETOOLONG.
 * 	[1995/12/21  01:51:24  ichikawa]
 * 
 * Revision 1.1.2.23  1995/11/13  02:33:35  ichikawa
 * 	Add dispositon of ntohs.
 * 	[1995/11/13  02:33:24  ichikawa]
 * 
 * Revision 1.1.2.22  1995/11/13  00:46:43  ichikawa
 * 	Add check of pathname length.
 * 	[1995/11/13  00:46:28  ichikawa]
 * 
 * Revision 1.1.2.21  1995/11/02  05:46:39  ichikawa
 * 	Change free disposition.
 * 	[1995/11/02  05:46:25  ichikawa]
 * 
 * Revision 1.1.2.20  1995/11/02  01:49:04  ichikawa
 * 	Add disposition of ACL handle and starage free.
 * 	[1995/11/02  01:48:54  ichikawa]
 * 
 * Revision 1.1.2.19  1995/11/01  02:33:22  ichikawa
 * 	Add free disposition of dfausername and dfagroupname.
 * 	[1995/11/01  02:31:33  ichikawa]
 * 
 * Revision 1.1.2.18  1995/10/26  00:32:54  kishi
 * 	Change Copyright.
 * 	[1995/10/26  00:32:47  kishi]
 * 
 * Revision 1.1.2.17  1995/10/24  07:08:17  ichikawa
 * 	Creat agt__acl_handle_delete API.
 * 	[1995/10/24  07:08:06  ichikawa]
 * 
 * Revision 1.1.2.16  1995/10/23  00:11:24  ichikawa
 * 	When stat system call is EFAULT,do agt_abort.
 * 	[1995/10/23  00:11:10  ichikawa]
 * 
 * Revision 1.1.2.15  1995/10/20  09:42:31  ichikawa
 * 	Change output message in security api error.
 * 	[1995/10/20  09:42:20  ichikawa]
 * 
 * Revision 1.1.2.14  1995/10/20  07:33:43  ichikawa
 * 	Delete sec_login_get_pwent in agt__ufs_rights.
 * 	[1995/10/20  07:33:32  ichikawa]
 * 
 * Revision 1.1.2.13  1995/10/20  07:01:00  ichikawa
 * 	Add disposition free add_entry.
 * 	[1995/10/20  07:00:48  ichikawa]
 * 
 * Revision 1.1.2.12  1995/10/20  06:25:27  ichikawa
 * 	Before free add_entry,checking value of findflag .
 * 	[1995/10/20  06:25:16  ichikawa]
 * 
 * Revision 1.1.2.11  1995/10/20  05:43:56  ichikawa
 * 	Add static void agt__acl_context_delete(void);.
 * 	[1995/10/20  05:43:45  ichikawa]
 * 
 * Revision 1.1.2.10  1995/10/20  04:17:31  ichikawa
 * 	Add agt__acl_context_delete API.
 * 	[1995/10/20  04:17:15  ichikawa]
 * 
 * Revision 1.1.2.9  1995/10/19  01:50:16  ichikawa
 * 	Add sec_acl_release_handle API.
 * 	[1995/10/19  01:49:58  ichikawa]
 * 
 * Revision 1.1.2.8  1995/10/13  09:07:04  kishi
 * 	Add header comment.
 * 	[1995/10/13  09:06:53  kishi]
 * 
 * Revision 1.1.2.7  1995/10/12  09:24:55  ichikawa
 * 	When sec_acl_replace is sec_acl_invalid_dfs_acl error and maskflag is't 0,d_errn
 * 	o set DEINVAL.
 * 	[1995/10/12  09:24:42  ichikawa]
 * 
 * Revision 1.1.2.6  1995/10/12  08:49:07  ichikawa
 * 	sec_id_parse_name error disposition was forgot.
 * 	[1995/10/12  08:48:46  ichikawa]
 * 
 * Revision 1.1.2.5  1995/09/23  09:54:18  kishi
 * 	Add header comment. Delete Japanese comment.
 * 	[1995/09/23  05:56:48  kishi]
 * 
 * Revision 1.1.2.4  1995/09/22  03:51:14  kishi
 * 	NULL -> 0.
 * 	[1995/09/22  03:50:59  kishi]
 * 
 * Revision 1.1.2.3  95/09/16  05:28:24  05:28:24  kishi (Y.Kishi)
 * 	Add header comment.
 * 	[1995/09/16  05:28:12  kishi]
 * 
 * Revision 1.1.2.2  1995/09/14  04:54:04  ichikawa
 * 	File owner check is changed.
 * 	[1995/09/14  04:53:15  ichikawa]
 * 
 * Revision 1.1.2.1  1995/09/09  06:54:25  kishi
 * 	Initial revision
 * 	[1995/09/09  06:15:46  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <dce/aclbase.h>
#include <dce/binding.h>
#include <dce/dce_cf.h>
#include <dce/pgo.h>
#include <dce/secidmap.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <syslog.h>
#include <dce/sec_login.h>
#include <memory.h>
#include <pwd.h>

#include "agt_agent.h"
#include "agt_usrc.h"
#include "agt_msg.h"
#include "agt_acl.h"
#include "agt_login.h"
#include "agt_acs.h"
#include "agt_sysid.h"
#include "agt_errid.h"


static int agt__acl_sdata_upack(char *str,unsigned int *pathlen);
static int agt__acl_aclget(char *str);
static int agt__acl_aclset(char *str);
static int agt__acl_acldel(char *str);
static int agt__acl_context_creat(void);
static int agt__acl_setacl_u(uuid_t pri_uuid);
static int agt__acl_setacl_g(uuid_t grp_uuid);
static int agt__acl_setacl_uo(void);
static int agt__acl_setacl_go(void);
static int agt__acl_get_uuid(char *princ_name,uuid_t *princ_idp);
static int agt__acl_modify_acl(char *princ_name);
static int agt__acl_get_entrytype(uuid_t princ_idp);
static int agt__acl_ufs_get();
static int agt__acl_ufs_set(char *princ_name);
static int agt__acl_chang_uid_name(uid_t uid,sec_rgy_name_t princ_name,unsigned int *pnamelen);
static int agt__acl_chang_gid_group(uid_t gid, sec_rgy_name_t group_name,unsigned int *gnamelen);
static int agt__acl_get_manager_types(unsigned32 *size_used,unsigned32 *num_types,uuid_t manager_types[]);
static int agt__acl_get_acl(uuid_t *manager_types,sec_acl_list_t *sec_acl_list);
static sec_acl_permset_t agt__ufs_rights(char *path);
static int agt__acl_handle_creat(void);
static int agt__acl_handle_delete(void);
static void agt__acl_context_delete(void);
static int agt__acl_handle_release(sec_acl_t *sec_acl_p);
extern struct d_login_info user_info;
extern sec_login_handle_t      login_context;
unsigned short user_type;
unsigned short group_type;
int dfauseroflag;
int dfagroupoflag;
int noflag;
int call_type;
unsigned int length;
unsigned short option;
int overflag;
int setentry_type;
int acl_size;
unsigned int trusty_bit;
uuid_t owner_uuid;
uuid_t ownerg_uuid;
sec_acl_handle_t h;



/**************************************************************************/
/* ID   = agt_acl_main							  */
/* name = ACL handling main function                                      */
/* func = Based on the information passed from Gateway, this function     */
/*        invokes functions to get ACL, to set ACL, and to delete an ACL  */
/*        entry.                                                          */
/* i/o  = str  :char * :IN: contains the following data:                  */
/*         :	   :   :  - ACL handling mode                             */
/*         :       :   :  - options                                       */
/*         :	   :   :  - path name length                              */
/*         :	   :   :  - path name                                     */
/*         :	   :   :  - ACL length                                    */
/*         :	   :   :  - ACL                                           */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = The ACL information at IN is used for setting and deleting ACL. */
/*        The ACL length and ACL information at OUT are used for getting  */
/*        ACL.                                                            */
/*                                                                        */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
int agt_acl_main(char *str)
{
	char senddata[8];
	unsigned int pathlen;


	user_type = 0x0001;
	group_type = 0x0002;
	noflag = 0;



	if (agt__acl_context_creat() == -1){
		agt_usrc_send_emsg(senddata,DFAM_SYSCALL_R);
		return(-1);
	}



	if (agt__acl_sdata_upack(str,&pathlen) == -1)
		goto func_err;
	else
		str += 10+pathlen;


	switch(option){
		case USER:
		case GROUP:
		case BOTH:
			break;
		default:
			d_errno = ECOM;
			goto func_err;
	}


	switch(call_type){
		case VNX_OPCODE_GETACL:
			if (agt__acl_aclget(str) == -1){
				agt__acl_context_delete();
				free(cellname);
				return(-1);
			}
			break;
		case VNX_OPCODE_SETACL:
			if (agt__acl_aclset(str) == -1){
				agt__acl_context_delete();
				free(cellname);
				return(-1);
			}
			break;
		case VNX_OPCODE_DELACL:
			if (agt__acl_acldel(str) == -1){
				agt__acl_context_delete();
				free(cellname);
				return(-1);
			}
			break;
		default:
			d_errno = ECOM;
			goto func_err;
	}
	agt__acl_context_delete();
	free(cellname);
	return(0);

func_err:
	agt_usrc_send_emsg(senddata,DFAM_SYSCALL_R);
	agt__acl_context_delete();
	free(cellname);
	return(-1);
}



/**************************************************************************/
/* ID   = agt__acl_aclget						  */
/* name = Getting ACL from the designated file                            */
/* func = Gets ACL from the designated file, and converts the ACL format  */
/*        to the Gateway style.  Calls agt__acl_ufs_get function if the   */
/*        file is a UFS file.  Sends the ACL information to Gateway.      */
/* i/o  = str  :char * :IN: contains the following informartion:          */
/*         :	   :   :  - ACL handling mode                             */
/*         :       :   :  - options                                       */
/*         :	   :   :  - path name length                              */
/*         :	   :   :  - path name                                     */
/*         :	   :   :  - ACL length                                    */
/*         :	   :   :  - ACL                                           */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_aclget(char *str)
{
	int i;
	unsigned int entry_type;
	unsigned int aclnum;
	unsigned int acl_num;
	unsigned int prin_namelen;
	int recall = 1;
	unsigned int pathlen;
	char recallsize[12];
	short type_user_obj = 0x0001;
	short type_user = 0x0002;
	short type_group_obj = 0x0004;
	short type_group = 0x0008;
	short type_other_obj = 0x0010;

	unsigned32 size_used;
	unsigned32 num_types;
	uuid_t manager_types;
	sec_acl_list_t sec_acl_list;
	sec_acl_entry_t *entry_p;
	sec_acl_t *sec_acl_p;
	int st;

	short type_mask_obj = 0x0020;

	overflag = 0;
	length = 0;
	acl_num = 0;



	dfauseroflag = dfagroupoflag = 0;


	memcpy((char *)&acl_size,str,sizeof(acl_size));
	str += sizeof(acl_size);
	acl_size = ntohl(acl_size);
	if (acl_size <= 0){
		d_errno = EINVAL;
		goto func_err;
	}


        if ((sendacl = (char *)malloc(acl_size+12)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25074,(acl_size+8),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				goto func_err;
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25075,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
        }


	begin_sendacl = sendacl;



        memset(begin_sendacl,0,(acl_size+12));



	st = agt__acl_handle_creat();
	if(st == -1){
		free(begin_sendacl);
		goto func_err;
	}
	else if(st == 1){
		if (agt__acl_ufs_get() == -1){
			free(begin_sendacl);
			goto func_err;
		}
		else if(overflag == 1){
			free(begin_sendacl);
			goto send_over;
		}
		goto func_success;
	}


	length += 4;

	sendacl += 16;


	if (agt__acl_get_manager_types(&size_used,&num_types,&manager_types) == -1){
		agt__acl_handle_delete();
		free(begin_sendacl);
		goto func_err;
	}


	if (agt__acl_get_acl(&manager_types,&sec_acl_list) == -1){
		agt__acl_handle_delete();
		free(begin_sendacl);
		goto func_err;
	}

	sec_acl_p = sec_acl_list.sec_acls[0];


	aclnum = sec_acl_p->num_entries;

	for (i = 0; i<aclnum; i++){

		entry_p = &sec_acl_p->sec_acl_entries[i];



		trusty_bit = entry_p->perms;


		entry_type = entry_p->entry_info.entry_type;


		switch(entry_type){
			case sec_acl_e_type_user_obj:
				if ((option == USER) || (option == BOTH)){

					length += (sizeof(trusty_bit)+sizeof(type_user_obj));
					if (length > acl_size){
						overflag = 1;
						goto setuo;
					}
					trusty_bit = htonl(trusty_bit);
					memcpy(sendacl,(char *)&trusty_bit,sizeof(trusty_bit));
					sendacl += sizeof(trusty_bit);


					type_user_obj = htons(type_user_obj);
					memcpy(sendacl,(char *)&type_user_obj,sizeof(type_user_obj));
					sendacl += sizeof(type_user_obj);

setuo:

					if (agt__acl_setacl_uo() == -1){
						agt__acl_handle_release(sec_acl_p);
						agt__acl_handle_delete();
						free(begin_sendacl);
						goto func_err;
					}
					acl_num++;
				}
				break;
			case sec_acl_e_type_group_obj:
				if ((option == GROUP) || (option == BOTH)){

					length += (sizeof(trusty_bit)+sizeof(type_group_obj));
					if (length > acl_size){
						overflag = 1;
						goto setgo;
					}

					trusty_bit = htonl(trusty_bit);
					memcpy(sendacl,(char *)&trusty_bit,sizeof(trusty_bit));
					sendacl += sizeof(trusty_bit);

					type_group_obj = htons(type_group_obj);
					memcpy(sendacl,(char *)&type_group_obj,sizeof(type_group_obj));
					sendacl += sizeof(type_group_obj);
setgo:
					if (agt__acl_setacl_go() == -1){
						agt__acl_handle_release(sec_acl_p);
						agt__acl_handle_delete();
						free(begin_sendacl);
						goto func_err;
					}
					acl_num++;
				}
				break;
			case sec_acl_e_type_other_obj:
				if ((option == GROUP) || (option == BOTH)){
					prin_namelen = strlen(DFA_OTHER_GROUP);
					length += (sizeof(trusty_bit)+sizeof(type_other_obj)+sizeof(prin_namelen)+prin_namelen);
					if (length > acl_size){
						overflag = 1;
						break;
					}

					trusty_bit = htonl(trusty_bit);
					memcpy(sendacl,(char *)&trusty_bit,sizeof(trusty_bit));
					sendacl += sizeof(trusty_bit);

					type_other_obj = htons(type_other_obj);
					memcpy(sendacl,(char *)&type_other_obj,sizeof(type_other_obj));
					sendacl += sizeof(type_other_obj);

					prin_namelen = htonl(prin_namelen);
					memcpy(sendacl,(char *)&prin_namelen,sizeof(prin_namelen));
					sendacl += sizeof(prin_namelen);

					prin_namelen = ntohl(prin_namelen);
					memcpy(sendacl,DFA_OTHER_GROUP,prin_namelen);
					sendacl += prin_namelen;
					acl_num++;
				}
				break;
			case sec_acl_e_type_user:
				if ((option == USER) || (option == BOTH)){

					if (agt__acl_setacl_u(entry_p->entry_info.tagged_union.id.uuid) == -1){
						agt__acl_handle_release(sec_acl_p);
						agt__acl_handle_delete();
						free(begin_sendacl);
						goto func_err;
					}


					if (noflag == 0){
						if (dfauseroflag != 0){
							if (strcmp(dfausername,dfauserobj))
								prin_namelen = strlen(dfausername);
							else{
								free(dfausername);
								continue;
							}
						}
						else
							prin_namelen = strlen(dfausername);
					}
					else
						prin_namelen = 0xffffffff;
					length += (sizeof(trusty_bit)+sizeof(type_user));
					if (length > acl_size){
						overflag = 1;
						goto setu;
					}


					trusty_bit = htonl(trusty_bit);
					memcpy(sendacl,(char *)&trusty_bit,sizeof(trusty_bit));
					sendacl += sizeof(trusty_bit);


					type_user = htons(type_user);
					memcpy(sendacl,(char *)&type_user,sizeof(type_user));
					sendacl += sizeof(type_user);
					
					type_user = ntohs(type_user);

setu:

					if (noflag == 0){
						if ((prin_namelen + sizeof(prin_namelen) + length) > acl_size){
							overflag = 1;
							length += (prin_namelen + sizeof(prin_namelen));
							free(dfausername);
							continue;
						}
					}
					else{
						if ((sizeof(prin_namelen) + length) > acl_size){
							overflag = 1;
							length += sizeof(prin_namelen);
							continue;
						}
					}
					prin_namelen = htonl(prin_namelen);
					memcpy(sendacl,(char *)&prin_namelen,sizeof(prin_namelen));
					sendacl += sizeof(prin_namelen);
					length += sizeof(prin_namelen);
					if (noflag == 0){
						prin_namelen = ntohl(prin_namelen);
						memcpy(sendacl,(char *)dfausername,prin_namelen);
						sendacl += prin_namelen;
						length += prin_namelen;
						free(dfausername);
					}
					acl_num++;
				}
				break;
			case sec_acl_e_type_group:
				if ((option == GROUP) || (option == BOTH)){
					if (agt__acl_setacl_g(entry_p->entry_info.tagged_union.id.uuid) == -1){
						agt__acl_handle_release(sec_acl_p);
						agt__acl_handle_delete();
						free(begin_sendacl);
						goto func_err;
					}

					if (noflag == 0){
						if ((strcmp(dfagroupname,DFA_OTHER_GROUP)) && (strcmp(dfagroupname,DFA_MASK_OBJ))){
							if (dfagroupoflag != 0){
								if (strcmp(dfagroupname,dfagroupobj))
									prin_namelen = strlen(dfagroupname);
								else{
									free(dfagroupname);
									continue;
								}
							}
							else
								prin_namelen = strlen(dfagroupname);
						}
						else{
							free(dfagroupname);
							continue;
						}
					}
					else
						prin_namelen = 0xffffffff;
					length += (sizeof(trusty_bit)+sizeof(type_group));
					if (length > acl_size){
						overflag = 1;
						goto setg;
					}

					trusty_bit = htonl(trusty_bit);
					memcpy(sendacl,(char *)&trusty_bit,sizeof(trusty_bit));
					sendacl += sizeof(trusty_bit);

					type_group = htons(type_group);
					memcpy(sendacl,(char *)&type_group,sizeof(type_group));
					sendacl += sizeof(type_group);

					type_group = ntohs(type_group);

setg:
					if (noflag == 0){
						if ((prin_namelen + sizeof(prin_namelen) + length) > acl_size){
							overflag = 1;
							length += (prin_namelen + sizeof(prin_namelen));
							free(dfagroupname);
							continue;
						}
					}
					else{
						if ((sizeof(prin_namelen) + length) > acl_size){
							overflag = 1;
							length += sizeof(prin_namelen);
							continue;
						}
					}
					prin_namelen = htonl(prin_namelen);
					memcpy(sendacl,(char *)&prin_namelen,sizeof(prin_namelen));
					sendacl += sizeof(prin_namelen);
					length += sizeof(prin_namelen);
					if (noflag == 0){
						prin_namelen = ntohl(prin_namelen);
						memcpy(sendacl,dfagroupname,prin_namelen);
						sendacl += prin_namelen;
						length += prin_namelen;
						free(dfagroupname);
					}
					acl_num++;
				}
				break;
			case sec_acl_e_type_mask_obj:
				if ((option == GROUP) || (option == BOTH)){
					prin_namelen = strlen(DFA_MASK_OBJ);
					length += (sizeof(trusty_bit)+sizeof(type_mask_obj)+sizeof(prin_namelen)+prin_namelen);
					if (length > acl_size){
						overflag = 1;
						break;
					}

					trusty_bit = htonl(trusty_bit);
					memcpy(sendacl,(char *)&trusty_bit,sizeof(trusty_bit));
					sendacl += sizeof(trusty_bit);

					type_mask_obj = htons(type_mask_obj);
					memcpy(sendacl,(char *)&type_mask_obj,sizeof(type_mask_obj));
					sendacl += sizeof(type_mask_obj);

					prin_namelen = htonl(prin_namelen);
					memcpy(sendacl,(char *)&prin_namelen,sizeof(prin_namelen));
					sendacl += sizeof(prin_namelen);

					prin_namelen = ntohl(prin_namelen);
					memcpy(sendacl,DFA_MASK_OBJ,prin_namelen);
					sendacl += prin_namelen;
					acl_num++;
				}
				break;
			case sec_acl_e_type_foreign_user:
				;;
				break;
			case sec_acl_e_type_foreign_group:
				;;
				break;
			case sec_acl_e_type_foreign_other:
				;;
				break;
			case sec_acl_e_type_any_other:
				;;
				break;
			case sec_acl_e_type_extended:
				;;
				break;
			default:
				;;
				break;
		}
	}

        if (agt__acl_handle_release(sec_acl_p) == -1){
                d_errno = DEUNKNOWN;
                agt__acl_handle_delete();
                free(begin_sendacl);
                goto func_err;
        }
	
        if (agt__acl_handle_delete() != 0){
		d_errno = DEUNKNOWN;
                free(begin_sendacl);
                goto func_err;
        }
	
	if (overflag == 1){
		free(begin_sendacl);
		goto send_over;
	}


	acl_num = htonl(acl_num);
	memcpy((begin_sendacl+12),(char *)&acl_num,sizeof(acl_num));


	length = htonl(length);
	memcpy((begin_sendacl+8),(char *)&length,sizeof(length));

func_success:
	memset(begin_sendacl,0,8);
	agt_usrc_send_data(begin_sendacl,length+12,DFAM_SYSCALL_R);
	free(begin_sendacl);
	if (dfauseroflag == 1)
		free(dfauserobj);
	if (dfagroupoflag == 1)
		free(dfagroupobj);
	return(0);
send_over:

	memcpy(msg.buf,(char *)&acl_size,sizeof(acl_size));
	AgentTrace(TRACEDEB,"OVER  ",sizeof(acl_size),msg.buf,0);
	memset(recallsize,0,12);
	recall = htonl(recall);
	memcpy(recallsize,(char *)&recall,sizeof(recall));
	length = htonl(length);
	memcpy(&recallsize[8],(char *)&length,sizeof(length));
	agt_usrc_send_data(recallsize,12,DFAM_SYSCALL_R);
        if (dfauseroflag == 1)
                free(dfauserobj);
        if (dfagroupoflag == 1)
                free(dfagroupobj);
	return(0);
func_err:
	pathlen = strlen(pathname);
	memcpy(msg.buf,(char *)&call_type,sizeof(call_type));
	memcpy(&msg.buf[4],(char *)&option,sizeof(option));
	memcpy(&msg.buf[6],(char *)&acl_size,sizeof(acl_size));
	memcpy(&msg.buf[10],(char *)&user_info.uid,sizeof(user_info.uid));
	memcpy(&msg.buf[14],(char *)&d_errno,sizeof(d_errno));
	strcpy(&msg.buf[18],pathname);
	AgentTrace(TRACEERR,"DACL  ",(18+pathlen),msg.buf,0);
	agt_usrc_send_emsg(recallsize,DFAM_SYSCALL_R);
        if (dfauseroflag == 1)
                free(dfauserobj);
        if (dfagroupoflag == 1)
                free(dfagroupobj);
	return(-1);
}



/**************************************************************************/
/* ID = agt__acl_aclset							  */	
/* name = Checking the information sent from Gateway when setting ACL     */
/* func = When ACL is set, this function unpacks the packed information   */
/*        passed from Gateway, and checks if the contents are valid.      */
/*        If valid, calls agt__acl_modify_acl function to set ACL,        */
/*        and sends the results to Gateway.                               */
/*                                                                        */
/* i/o  = str  :char * :IN: contains the following informartion:          */
/*         :	   :   :  - ACL handling mode                             */
/*         :       :   :  - options                                       */
/*         :	   :   :  - path name length                              */
/*         :	   :   :  - path name                                     */
/*         :	   :   :  - ACL length                                    */
/*         :	   :   :  - ACL                                           */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_aclset(char *str)
{
	char *princ_name;
	unsigned int pnamelen,pathlen;
	int acllength;
	char senddata[8];


	memcpy((char *)&acllength,str,sizeof(acllength));
	str += sizeof(acllength);


	memcpy((char *)&trusty_bit,str,sizeof(trusty_bit));
	str += sizeof(trusty_bit);
	trusty_bit = ntohl(trusty_bit);


	memcpy((char *)&pnamelen,str,sizeof(pnamelen));
	str += sizeof(pnamelen);
	pnamelen = ntohl(pnamelen);


	if ((princ_name = (char *)malloc(pnamelen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25079,(pnamelen+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(senddata,DFAM_SYSCALL_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25080,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}


	memcpy(princ_name,str,pnamelen);
	str += pnamelen;
	*(princ_name+pnamelen) = '\0';


	if ((trusty_bit & 0xFFFFFFC0) != 0){
		d_errno = ECOM;
		goto func_err;
	}


	if (agt__acl_modify_acl(princ_name) == -1){
		goto func_err;
	}


	memset(senddata,0,8);
	agt_usrc_send_data(senddata,8,DFAM_SYSCALL_R);
	free(princ_name);
	return(0);

func_err:
	pathlen = strlen(pathname);
        memcpy(msg.buf,(char *)&call_type,sizeof(call_type));
        memcpy(&msg.buf[4],(char *)&option,sizeof(option));
        memcpy(&msg.buf[6],(char *)&pathname,pathlen);
        memcpy(&msg.buf[10],(char *)&trusty_bit,sizeof(trusty_bit));
        memcpy(&msg.buf[14],(char *)&user_info.uid,sizeof(user_info.uid));
        memcpy(&msg.buf[18],(char *)&d_errno,sizeof(d_errno));
        strcpy(&msg.buf[22],pathname);
        strcpy(&msg.buf[22+pathlen],princ_name);
	AgentTrace(TRACEERR,"DACL  ",(22+pathlen+pnamelen),msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_SYSCALL_R);
	free(princ_name);
	return(-1);
}



/**************************************************************************/
/* ID   = agt__acl_acldel					          */	
/*                                                                        */
/* name = Checking the information sent from Gateway when deleting ACL    */
/* func = When ACL is deleted, this function unpacks the packed           */
/*        information passed from Gateway, and checks if the contents are */
/*        valid.  If valid, calls agt__acl_modify_acl function to delete  */
/*        ACL, and sends the results to Gateway.                          */
/*                                                                        */
/* i/o  = str  :char * :IN: contains the following informartion:          */
/*         :	   :   :  - ACL handling mode                             */
/*         :       :   :  - options                                       */
/*         :	   :   :  - path name length                              */
/*         :	   :   :  - path name                                     */
/*         :	   :   :  - ACL length                                    */
/*         :	   :   :  - ACL                                           */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_acldel(char *str)
{
	unsigned int pnamelen,pathlen;
	char *princ_name;
	int acllength;
	char senddata[8];


        memcpy((char *)&acllength,str,sizeof(acllength));
        str += sizeof(acllength);


	memcpy((char *)&pnamelen,str,sizeof(pnamelen));
	str += sizeof(pnamelen);
	pnamelen = ntohl(pnamelen);

	if ((princ_name = (char *)malloc(pnamelen+1)) == NULL){
		switch(errno){
			case ENOMEM:
				d_errno = errno;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25081,(princ_name+1),user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_usrc_send_emsg(senddata,DFAM_SYSCALL_R);
				return(-1);
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25082,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}


	memcpy(princ_name,str,pnamelen);
	str += pnamelen;
	*(princ_name+pnamelen) = '\0';


	if (agt__acl_modify_acl(princ_name) == -1){
		goto func_err;
	}


	memset(senddata,0,8);
	agt_usrc_send_data(senddata,8,DFAM_SYSCALL_R);
	free(princ_name);
	return(0);

func_err:
	pathlen = strlen(pathname);
	memcpy(msg.buf,(char *)&call_type,sizeof(call_type));
	memcpy(&msg.buf[4],(char *)&option,sizeof(option));
	memcpy(&msg.buf[6],(char *)&user_info.uid,sizeof(user_info.uid));
	memcpy(&msg.buf[10],(char *)&d_errno,sizeof(d_errno));
	strcpy(&msg.buf[14],pathname);
	strcpy(&msg.buf[14+pathlen],princ_name);
	agt_usrc_send_emsg(senddata,DFAM_SYSCALL_R);
	free(princ_name);
	return(-1);
}

/**************************************************************************/
/* ID   = agt__acl_setacl_uo						  */
/* name = Setting file owner's ACL to sendacl (global variable)           */
/* func = This function gets the file owner's name from the file owner's  */
/*        UID, and sets it to sendacl, the global variable.  If the owner */
/*        name was unable to get, set -1 to the owner name length.        */
/* i/o  = none                                                            */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_setacl_uo()
{
	size_t namelen;
	struct stat pathbuf;
	sec_rgy_name_t global_name;
	sec_rgy_name_t cell_namep;
	sec_rgy_name_t princ_namep;
	uuid_t st_uuid;



	noflag = 0;


	if (stat(pathname,&pathbuf) == -1){
		switch(errno){
			case EFAULT:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25083,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DSTAT,d_errno);
				return(-1);
		}
	}


        sec_rgy_pgo_unix_num_to_id(context,sec_rgy_domain_person,pathbuf.st_uid,&st_uuid,&status);
	if (status != error_status_ok){
		switch(status){
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECUNIXNUMTOID,d_errno);
				return(-1);
			case sec_rgy_object_not_found:
				noflag = 1;
				goto noentry;
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25084,status,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				return(-1);
		}
	}


	sec_rgy_pgo_id_to_name(context,sec_rgy_domain_person,&st_uuid,princ_namep,&status);
	if (status != error_status_ok){
		switch(status){
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECIDGENNAME,d_errno);
				return(-1);
			case sec_rgy_object_not_found:
				noflag = 1;
				goto noentry;
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25167,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				return(-1);
		}
	}

noentry:
	if (noflag == 0){
		namelen = strlen((const char *)princ_namep);
		if ((dfauserobj = (char *)malloc(namelen+1)) == NULL){
			switch(errno){
				case ENOMEM:
					d_errno = errno;
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26047,(namelen+1),user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
					AgentMsg(LOG_WARNING,msg.buf);
					return(-1);
				default:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26048,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
			}
		}
		dfauseroflag = 1;
		strcpy(dfauserobj,(char *)princ_namep);
		if ((namelen + sizeof(namelen) +length) > acl_size){
			overflag = 1;
			length += (namelen+sizeof(namelen));
			return(0);
		}
	}
	else{
		dfauseroflag = 0;
		namelen = 0xffffffff;
		if ((sizeof(namelen) + length) > acl_size){
			overflag = 1;
			length += sizeof(namelen);
			return(0);
		}
	}


	namelen = htonl(namelen);
	memcpy(sendacl,(char *)&namelen,sizeof(namelen));
	sendacl += sizeof(namelen);
	length += sizeof(namelen);
	if (noflag == 0){
		namelen = ntohl(namelen);
		memcpy(sendacl,(char *)princ_namep,namelen);
		sendacl += namelen;
		length += namelen;
	}
	return(0);
}


/**************************************************************************/
/* ID   = agt__acl_setacl_go						  */
/* name = Setting file owner group's ACL to sendacl (global variable)     */
/* func = This function gets the file owner group's name from the file    */
/*        owner group's UID, and sets it to sendacl, the global variable. */
/*        If the owner group name was unable to get, set -1 to the owner  */
/*        group name length.                                              */
/* i/o  = none                                                            */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_setacl_go()
{
	struct stat pathbuf;
	size_t namelen;
	uuid_t group_uuid;
	sec_rgy_name_t global_name;
	sec_rgy_name_t cell_namep;
	sec_rgy_name_t group_namep;


	noflag = 0;


	if (stat(pathname,&pathbuf) == -1 ){
                switch(errno){
                        case EFAULT:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25086,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
                                agt_abort();
                        default:
                                d_errno = errno;
				agt_usrc_oserr_trace(DSTAT,d_errno);
                                return(-1);
                }
	}


	sec_rgy_pgo_unix_num_to_id(context,sec_rgy_domain_group,(long)pathbuf.st_gid,&group_uuid,&status);
        if (status != error_status_ok){
		switch(status){
			case sec_rgy_object_not_found:
				noflag = 1;
				goto noentry;
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECUNIXNUMTOID,d_errno);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25087,status,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				return(-1);
		}
        }


	sec_rgy_pgo_id_to_name(context,sec_rgy_domain_group,&group_uuid,group_namep,&status);
        if (status != error_status_ok){
                switch(status){
                        case sec_rgy_object_not_found:
				noflag = 1;
				goto noentry;
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECIDGENGROUP,d_errno);
				return(-1);
			case sec_id_e_name_too_long:
				d_errno = ENAMETOOLONG;
				agt_usrc_oserr_trace(SECIDGENGROUP,d_errno);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25168,status,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				return(-1);
                }
	}

noentry:
	if (noflag == 0){
		namelen = strlen((const char *)group_namep);
		if ((dfagroupobj = (char *)malloc(namelen+1)) == NULL){
			switch(errno){
				case ENOMEM:
					d_errno = errno;
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26049,(namelen+1),user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
					AgentMsg(LOG_WARNING,msg.buf);
					return(-1);
				default:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26050,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034));
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
			}
		}
		dfagroupoflag = 1;
		strcpy(dfagroupobj,(char *)group_namep);

        	if ((namelen + sizeof(namelen) + length) > acl_size){
                	overflag = 1;
			length += (namelen+sizeof(namelen));
			return(0);
        	}
	}
	else{
		namelen = 0xffffffff;
		dfagroupoflag = 0;
		if ((sizeof(namelen) + length) > acl_size){
			overflag = 1;
			length += sizeof(namelen);
			return(0);
		}
	}


	namelen = htonl(namelen);
	memcpy(sendacl,(char *)&namelen,sizeof(namelen));
	sendacl += sizeof(namelen);
	length += sizeof(namelen);
	if (noflag == 0){
		namelen = ntohl(namelen);
		memcpy(sendacl,(char *)group_namep,namelen);
		sendacl += namelen;
		length += namelen;
	}
	return(0);
}



/**************************************************************************/
/* ID   = agt__acl_setacl_u						  */
/* name = Getting user name from uuid                                     */
/* func = This function gets a user name from uuid, and sets the name to  */
/*        dfausername, a global variable.                                 */
/*        If the user name was unable to get, set -1 to the user name     */
/*        length.                                                         */
/* i/o  = pri_uuid:uuid_t :IN: user's UUID                                */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_setacl_u(uuid_t pri_uuid)
{
	unsigned int namelen;
	sec_rgy_name_t global_name;
	sec_rgy_name_t cell_namep;
	sec_rgy_name_t princ_namep;


	noflag = 0;


	sec_rgy_pgo_id_to_name(context,sec_rgy_domain_person,&pri_uuid,princ_namep,&status);
        if (status != error_status_ok){
                switch(status){
			case sec_rgy_object_not_found:
				noflag = 1;
				break;
                        case sec_rgy_server_unavailable:
                                d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECIDGENNAME,d_errno);
                                return(-1);
			case sec_id_e_name_too_long:
				d_errno = ENAMETOOLONG;
				agt_usrc_oserr_trace(SECIDGENNAME,d_errno);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25169,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR, msg.buf);
				return(-1);
                }
        }

	if (noflag == 0){
		namelen = strlen((const char *)princ_namep);
		if ((dfausername = (char *)malloc(namelen+1)) == NULL){
			switch(errno){
				case ENOMEM:
					d_errno = errno;
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26051,(namelen+1),user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
					AgentMsg(LOG_WARNING,msg.buf);
					return(-1);
				default:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26052,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
			}
		}
		strcpy(dfausername,(char *)princ_namep);
	}
	return(0);
}

/**************************************************************************/
/* ID   = agt__acl_setacl_g						  */
/* name = Getting group name from uuid                                    */
/* func = This function gets a group name from uuid, and sets the name to */
/*        dfagroupname, a global variable.                                */
/*        If the group name was unable to get, set -1 to the group name   */
/*        length.                                                         */
/* i/o  = princ_idp:uuid_t :IN: group UUID                                */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_setacl_g(uuid_t grp_uuid)
{
	unsigned int namelen;
	sec_rgy_name_t global_name;
	sec_rgy_name_t cell_namep;
	sec_rgy_name_t group_namep;


	noflag = 0;

	sec_rgy_pgo_id_to_name(context,sec_rgy_domain_group,&grp_uuid,group_namep,&status);
        if (status != error_status_ok){
                switch(status){
                        case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECIDGENGROUP,d_errno);
				return(-1);
			case sec_rgy_object_not_found:
				noflag = 1;
				break;
			case sec_id_e_name_too_long:
				d_errno = ENAMETOOLONG;
				agt_usrc_oserr_trace(SECIDGENGROUP,d_errno);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25170,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				return(-1);
                }
        }


	if (noflag == 0){
		namelen = strlen((const char *)group_namep);
		if ((dfagroupname = (char *)malloc(namelen+1)) == NULL){
			switch(errno){
				case ENOMEM:
					d_errno = errno;
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_26053,(namelen+1),user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
					AgentMsg(LOG_WARNING,msg.buf);
					return(-1);
				default:
					sprintf(msg.wkbuf,"%d,%d",D_EID_26054,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
			}
		}
		strcpy(dfagroupname,(char *)group_namep);
	}
	return(0);
}

/**************************************************************************/
/* ID   = agt__acl_get_entrytype					  */
/* name = Getting entry type                                              */
/* func = This function checks if the file owner (owner group), and sets  */
/*        a proper value to setentry_type.                                */
/* i/o  = princ_idp:uuid_t :IN: user (group) UUID                         */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_get_entrytype(uuid_t princ_idp)
{
	struct stat pathinfo;
	uuid_t princ_uuid;


	if (stat(pathname,&pathinfo) == -1){
                switch(errno){
                        case EFAULT:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25091,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
                                agt_abort();
                        default:
                                d_errno = errno;
				agt_usrc_oserr_trace(DSTAT,d_errno);
                                return(-1);
                }
	}

	if (option == USER){


		sec_rgy_pgo_unix_num_to_id(context,sec_rgy_domain_person,pathinfo.st_uid,&princ_uuid,&status);
		if (status != error_status_ok){
			switch(status){
				case sec_rgy_server_unavailable:
					d_errno = DEUNAVAILSRV;
					agt_usrc_oserr_trace(SECUNIXNUMTOID,d_errno);
					return(-1);
				case sec_rgy_object_not_found:
					d_errno = DEUNNOWNPRI;
					agt_usrc_oserr_trace(SECUNIXNUMTOID,d_errno);
					return(-1);
				default:
					d_errno = DEUNKNOWN;
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25092,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					return(-1);
			}
		}


		if (memcmp((char *)&princ_uuid,(char *)&princ_idp,sizeof(princ_uuid)) == 0)
			setentry_type = sec_acl_e_type_user_obj;
		else
			setentry_type = sec_acl_e_type_user;

	}
	else if( option == GROUP){


		sec_rgy_pgo_unix_num_to_id(context,sec_rgy_domain_group,pathinfo.st_gid,&princ_uuid,&status);
		if (status != error_status_ok){
			switch(status){
                		case sec_rgy_server_unavailable:
					d_errno = DEUNAVAILSRV;
					agt_usrc_oserr_trace(SECUNIXNUMTOID,d_errno);
					return(-1);
				case sec_rgy_object_not_found:
					d_errno = DEUNNOWNPRI;
					agt_usrc_oserr_trace(SECUNIXNUMTOID,d_errno);
					return(-1);
				default:
					d_errno = DEUNKNOWN;
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25093,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					return(-1);
                        }
		}

		if (memcmp((char *)&princ_uuid,(char *)&princ_idp,sizeof(princ_uuid)) == 0)
			setentry_type = sec_acl_e_type_group_obj;
		else
			setentry_type = sec_acl_e_type_group;
	}
	else{
		d_errno = ECOM;
		return(-1);
	}
	return(0);
}

/**************************************************************************/
/* ID   = agt__acl_modify_acl						  */
/* name = ACL modification function                                       */
/* func = This function gets an ACL list from the designated file, adds   */
/*        information to the ACL, and set the modified ACL to the file.   */
/*        If the target file is a UFS file, this function invokes         */
/*        agt__acl_ufs_set.                                               */
/* i/o  = princ_name:char *:IN: principal name                            */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_modify_acl(char *princ_name)
{
	uuid_t princ_idp;
	uuid_t princ_uuid;
	int findflag = 0;
	int aclnum;
 	int i,j;
        int maskflag = 0;
	unsigned int entry_type;

	int st;
	unsigned32 size_used;
	unsigned32 num_types;
	uuid_t manager_types;
	sec_acl_list_t sec_acl_list;
	sec_acl_entry_t *entry_p;
	sec_acl_t *sec_acl_p;
	sec_acl_entry_t *new_sec_acl_entries;
	sec_acl_entry_t *add_entry;
	sec_acl_entry_t *entry_p2;


	setentry_type = -1;

	if ((option == USER) || ((strcmp(princ_name,DFA_OTHER_GROUP) != 0) && (strcmp(princ_name,DFA_MASK_OBJ) != 0))){

		if (agt__acl_get_uuid(princ_name,&princ_uuid) == -1)
			return(-1);


		if (agt__acl_get_entrytype(princ_uuid) == -1)
			return(-1);
	}


	st = agt__acl_handle_creat();
	if (st == -1)
		return(-1);
	else if(st == 1){
		if (call_type == VNX_OPCODE_DELACL){
			d_errno = EOPNOTSUPP;
			return(-1);
		}
		if (agt__acl_ufs_set(princ_name) == -1)
			return(-1);
		else
			return(0);
	}


	if (agt__acl_get_manager_types(&size_used,&num_types,&manager_types) == -1){
		agt__acl_handle_delete();
		return(-1);
	}


	if (agt__acl_get_acl(&manager_types,&sec_acl_list) == -1){
		agt__acl_handle_delete();
		return(-1);
	}

	sec_acl_p = sec_acl_list.sec_acls[0];


	aclnum = sec_acl_p->num_entries;


	for (i=0; i<sec_acl_p->num_entries; i++){
		if (findflag == 1)
			break;

		entry_p = &sec_acl_p->sec_acl_entries[i];


		entry_type = entry_p->entry_info.entry_type;

		switch(entry_type) {
			case sec_acl_e_type_user_obj:
				if ((call_type == VNX_OPCODE_SETACL) && (setentry_type == sec_acl_e_type_user_obj)){
					trusty_bit |= 0x0008;
					entry_p->perms = trusty_bit;
					findflag = 1;
				}
				else if ((call_type == VNX_OPCODE_DELACL) && (setentry_type == sec_acl_e_type_user_obj)){

					d_errno = DENOTDEL;
					agt__acl_handle_release(sec_acl_p);
					agt__acl_handle_delete();
					return(-1);
				}
			break;
			case sec_acl_e_type_group_obj:
				if ((call_type == VNX_OPCODE_SETACL) && (setentry_type == sec_acl_e_type_group_obj)){
					entry_p->perms = trusty_bit;
					findflag = 1;
				}
				else if ((call_type == VNX_OPCODE_DELACL) && (setentry_type == sec_acl_e_type_group_obj)){

					d_errno = DENOTDEL;
					agt__acl_handle_release(sec_acl_p);
					agt__acl_handle_delete();
					return(-1);
				}
			break;
			case sec_acl_e_type_other_obj:
				if ((strcmp(princ_name,DFA_OTHER_GROUP) == 0) && (option == GROUP)){
					if (call_type ==VNX_OPCODE_SETACL){
						entry_p->perms = trusty_bit;
						findflag = 1;
					}
					else if(call_type == VNX_OPCODE_DELACL){
						d_errno = DENOTDEL;
						agt__acl_handle_release(sec_acl_p);
						agt__acl_handle_delete();
						return(-1);
					}
				}
				break;
			case sec_acl_e_type_user:

				princ_idp = entry_p->entry_info.tagged_union.id.uuid;
				if (memcmp((char *)&princ_idp,(char *)&princ_uuid,sizeof(princ_idp))==0){
					if ((call_type == VNX_OPCODE_SETACL) && (setentry_type == sec_acl_e_type_user)){
						entry_p->perms = trusty_bit;
						findflag = 1;
					}
					else if ((call_type == VNX_OPCODE_DELACL) && (setentry_type == sec_acl_e_type_user)){
						entry_p2 = &(sec_acl_p->sec_acl_entries[i]);

						if (entry_p2){
							if (entry_p2->entry_info.tagged_union.id.name)
								free(entry_p2->entry_info.tagged_union.id.name);
						}	

						for (j=i; j<sec_acl_p->num_entries-1; j++)
							sec_acl_p->sec_acl_entries[j] = sec_acl_p->sec_acl_entries[j+1];
						sec_acl_p->num_entries--;
						findflag = 1;
					}
				}
				break;
			case sec_acl_e_type_group:

				princ_idp = entry_p->entry_info.tagged_union.id.uuid;
				if (memcmp((char *)&princ_idp,(char *)&princ_uuid,sizeof(princ_idp)) == 0){
					if ((call_type == VNX_OPCODE_SETACL) && (setentry_type == sec_acl_e_type_group)){
						entry_p->perms = trusty_bit;
						findflag = 1;
					}
					else if ((call_type == VNX_OPCODE_DELACL) && (setentry_type == sec_acl_e_type_group)){
						entry_p2 = &(sec_acl_p->sec_acl_entries[i]);
						if (entry_p2){
							if (entry_p2->entry_info.tagged_union.id.name)
								free(entry_p2->entry_info.tagged_union.id.name);
						}
						for (j=i; j<sec_acl_p->num_entries-1; j++)
							sec_acl_p->sec_acl_entries[j] = sec_acl_p->sec_acl_entries[j+1];
						sec_acl_p->num_entries--;
						findflag = 1;
					}
				}
				break;
			case sec_acl_e_type_mask_obj:
				maskflag = 1;
				if ((strcmp(princ_name,DFA_MASK_OBJ) == 0) && (option == GROUP)){
					if (call_type == VNX_OPCODE_SETACL){
						entry_p->perms = trusty_bit;
						findflag = 1;
					}
					else if(call_type == VNX_OPCODE_DELACL){
						d_errno = DENOTDEL;
						agt__acl_handle_release(sec_acl_p);
						agt__acl_handle_delete();
						return(-1);
					}
				}
				break;
			case sec_acl_e_type_foreign_user:
				break;
			case sec_acl_e_type_foreign_group:
				break;
			case sec_acl_e_type_foreign_other:
				break;
			case sec_acl_e_type_any_other:
				break;
			case sec_acl_e_type_extended:
				break;
			default:
				break;
		}
	}


	if ( findflag == 0 ){
		if ( call_type == VNX_OPCODE_SETACL ){

			if ((new_sec_acl_entries = (sec_acl_entry_t *)malloc((sec_acl_list.sec_acls[0]->num_entries+1)*sizeof(sec_acl_entry_t))) == NULL){
				switch(errno){
					case ENOMEM:
						d_errno = errno;
						sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25189,(sec_acl_list.sec_acls[0]->num_entries+1)*sizeof(sec_acl_entry_t),user_info.uid);
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
						AgentMsg(LOG_WARNING,msg.buf);
						agt__acl_handle_release(sec_acl_p);
						agt__acl_handle_delete();
						return(-1);
					default:
						sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25190,(sec_acl_list.sec_acls[0]->num_entries+1)*sizeof(sec_acl_entry_t),user_info.uid);
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
						AgentMsg(LOG_WARNING,msg.buf);
						agt_abort();
				}
			}

			for (i=0; i<sec_acl_list.sec_acls[0]->num_entries;i++)
				new_sec_acl_entries[i] = sec_acl_list.sec_acls[0]->sec_acl_entries[i];
			if (sec_acl_list.sec_acls[0]->sec_acl_entries){
				free((char *)sec_acl_list.sec_acls[0]->sec_acl_entries);
				sec_acl_list.sec_acls[0]->sec_acl_entries = NULL;	
			}

			if ((add_entry = (sec_acl_entry_t *)malloc(sizeof(sec_acl_entry_t))) == NULL){
				switch(errno){
					case ENOMEM:
						d_errno = errno;
						sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25191,sizeof(sec_acl_entry_t),user_info.uid);
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
						AgentMsg(LOG_WARNING,msg.buf);
						free(new_sec_acl_entries);
						agt__acl_handle_release(sec_acl_p);
						agt__acl_handle_delete();
						return(-1);
					default:
						sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25192,sizeof(sec_acl_entry_t),user_info.uid);
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
						AgentMsg(LOG_WARNING,msg.buf);
						agt_abort();
				}
			}

			add_entry->perms = trusty_bit;
			if ((strcmp(princ_name,DFA_MASK_OBJ)) || (option == USER)){
				add_entry->entry_info.entry_type = setentry_type;
				add_entry->entry_info.tagged_union.id.uuid = princ_uuid;
				if((add_entry->entry_info.tagged_union.id.name = (unsigned_char_p_t)malloc((strlen(princ_name)+1) * sizeof(char))) == NULL){
					switch(errno){
						case ENOMEM:
							d_errno = errno;
							sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25191,(strlen(princ_name)+1),user_info.uid);
							sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
							AgentMsg(LOG_WARNING,msg.buf);
							free(new_sec_acl_entries);
							free(add_entry);
							agt__acl_handle_release(sec_acl_p);
							agt__acl_handle_delete();
							return(-1);
						default:
							sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25191,(strlen(princ_name)+1),user_info.uid);
							sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
							AgentMsg(LOG_WARNING,msg.buf);
							agt_abort();
					}
				}
				strcpy((char *)add_entry->entry_info.tagged_union.id.name,princ_name);
			}
			else
				add_entry->entry_info.entry_type = sec_acl_e_type_mask_obj;

			new_sec_acl_entries[sec_acl_list.sec_acls[0]->num_entries] = *add_entry;
			sec_acl_list.sec_acls[0]->sec_acl_entries = new_sec_acl_entries;
			sec_acl_list.sec_acls[0]->num_entries++;
			free(add_entry);

		}
		else if ( call_type == VNX_OPCODE_DELACL ){
			d_errno = DENOACLPRI;
			agt__acl_handle_release(sec_acl_p);
			agt__acl_handle_delete();
			return(-1);
		}
	}

	sec_acl_replace(h,&manager_types,sec_acl_type_object,&sec_acl_list,&status);
	if (status != 0){
		if (access(pathname,F_OK) != 0){
			switch(errno){
				case EFAULT:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25197,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
				default:
					d_errno = errno;
					agt_usrc_oserr_trace(DACCESS,d_errno);
					agt__acl_handle_release(sec_acl_p);
					agt__acl_handle_delete();
					return(-1);
			}
		}

		switch(status){
			case sec_acl_not_authorized:
				d_errno = EACCES;
				break;
			case sec_acl_invalid_dfs_acl:
				if (maskflag == 0)
					d_errno = DENOMASK;
				else
					d_errno = DEINVAL;
				break;
			default:
				d_errno = DEUNKNOWN;
				break;
		}
		agt_usrc_oserr_trace(SECACLREPLACE,d_errno);
		agt__acl_handle_release(sec_acl_p);
		agt__acl_handle_delete();
		return(-1);
	}

        if (agt__acl_handle_release(sec_acl_p) == -1){
                d_errno = DEUNKNOWN;
                agt__acl_handle_delete();
                return(-1);
        }

        if (agt__acl_handle_delete() != 0){
		d_errno = DEUNKNOWN;
                return(-1);
        }

	return(0);
}

/**************************************************************************/
/* ID   = agt__acl_context_creat					  */
/* name = Making context handle to be used by sec_xxx function            */
/* func = This function does the following processing:                    */
/*         - Gets a local cell name.                                      */
/*         - Gets contents information.                                   */
/*         - Makes login user's global name.                              */
/* i/o  = none                                                            */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_context_creat()
{
        cellname = NULL;
        dce_cf_get_cell_name(&cellname,(unsigned long *)&status);
        if (status != error_status_ok){
		switch(status){
			case dce_cf_e_no_mem:
				sprintf(msg.wkbuf,"%d,%d",D_EID_25096,user_info.uid);
				d_errno = ENOMEM;
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(ENOMEM),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25171,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				return(-1);
		}
        }


	sec_rgy_site_open((unsigned_char_t *)cellname,&context,&status);
        if (status != error_status_ok){
		switch(status){
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECSITEOPEN,d_errno);
				free(cellname);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf, "%d,%d,%d",D_EID_25172,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				free(cellname);
				return(-1);
		}
        }

	return(0);
}



/**************************************************************************/
/* ID   = agt__acl_get_uuid						  */
/* name = Convertion from principal name to UUID                          */
/* func = This function gets a glogal name, and converts it into UUID.    */
/* i/o  = princ_name:char *  :IN : principal name                         */
/*        princ_idp :uuid_t *:OUT: principal's UUID                       */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_get_uuid(char *princ_name,uuid_t *princ_idp)
{
	sec_rgy_name_t princ_namep;
	int namelen;

	namelen = strlen(princ_name);
	if (namelen > 1024){
		d_errno = ENAMETOOLONG;
		return(-1);
	}

	strcpy((char *)princ_namep,princ_name);

	if ( option == USER ){
		sec_rgy_pgo_name_to_id(context,sec_rgy_domain_person,princ_namep,princ_idp,&status);
		if (status != error_status_ok){

			switch(status){
				case sec_rgy_server_unavailable:
					d_errno = DEUNAVAILSRV;
					agt_usrc_oserr_trace(SECIDPARCENAME,d_errno);
					return(-1);
				case sec_rgy_object_not_found:
					d_errno = DEUNNOWNPRI;
					agt_usrc_oserr_trace(SECIDPARCENAME,d_errno);
					return(-1);
				case sec_id_e_name_too_long:
					d_errno = ENAMETOOLONG;
					agt_usrc_oserr_trace(SECIDPARCENAME,d_errno);
					return(-1);
				default:
					d_errno = DEUNKNOWN;
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25175,status,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					return(-1);
			}
		}
	}
	else if( option == GROUP){
		sec_rgy_pgo_name_to_id(context,sec_rgy_domain_group,princ_namep,princ_idp,&status);
		if ( status != error_status_ok){
                        switch(status){
                                case sec_rgy_server_unavailable:
                                        d_errno = DEUNAVAILSRV;
					agt_usrc_oserr_trace(SECIDPARCEGROUP,d_errno);
					return(-1);
                                case sec_rgy_object_not_found:
                                        d_errno = DEUNNOWNPRI;
					agt_usrc_oserr_trace(SECIDPARCEGROUP,d_errno);
					return(-1);
				case sec_id_e_name_too_long:
					d_errno = ENAMETOOLONG;
					agt_usrc_oserr_trace(SECIDPARCEGROUP,d_errno);
					return(-1);
				default:
					d_errno = DEUNKNOWN;
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25176,status,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620), msg.wkbuf);
					AgentMsg(LOG_ERR, msg.buf);
					return(-1);
                        }
		}
	}
	else{
		d_errno = ECOM;
		return(-1);
	}
	return(0);
}

/**************************************************************************/
/* ID   = agt__acl_sdata_upack						  */
/* name = Unpacking the received data                                     */
/* func = When ACL is obtained, set, or deleted, this function unpacks    */
/*        the packed data sent from Gateway.                              */
/*                                                                        */
/* i/o  = str  :char * :IN: contains the following informartion:          */
/*          	          - ACL handling mode                             */
/*                        - options                                       */
/*          	          - path name length                              */
/*          	          - path name                                     */
/*        pathlen:unsigned int *:OUT: path name length                    */
/*                                                                        */
/* return = 0: nornal end                                                 */
/*         -1: abnorname end 						  */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_sdata_upack(char *str,unsigned int *pathlen)
{


        memcpy((char *)&call_type,str,sizeof(call_type));
        call_type = ntohl(call_type);
        str += sizeof(call_type);


        memcpy((char *)&option,str,sizeof(option));
        option = ntohs(option);
        str += sizeof(option);


        memcpy((char *)pathlen,str,sizeof(pathlen));
        *pathlen = ntohl(*pathlen);
        str += sizeof(pathlen);

	if (*pathlen > PATH_MAX){
		d_errno = ENAMETOOLONG;
		return(-1);
	}

        memcpy(pathname,str,*pathlen);
        pathname[*pathlen] = '\0';

	return(0);
}



/**************************************************************************/
/* ID   = agt_acl_rights						  */
/* name = Getting effective rights of login users                         */
/* func = This function unpacks the packed data sent from Gateway.  Gets  */
/*        the effective right information corresponding to the designated */
/*        file name, and sends the information to Gateway.                */
/*                                                                        */
/* i/o  = str  :char * :IN: contains the following informartion:          */
/*          	          - path name length                              */
/*          	          - path name                                     */
/*                                                                        */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
int agt_acl_rights(char *str)
{
	unsigned int pathlen;
	char senddata[8];

	int st;
	sec_acl_permset_t net_rights;
	unsigned32 size_used;
	unsigned32 num_types;
	uuid_t manager_types;


	memcpy((char *)&pathlen,str,sizeof(pathlen));
	pathlen = ntohl(pathlen);
	str += sizeof(pathlen);
	
	if (pathlen > PATH_MAX){
		d_errno = ENAMETOOLONG;
		goto func_err;
	}

	memcpy(pathname,str,pathlen);
	pathname[pathlen] = '\0';


	st = agt__acl_handle_creat();
	if (st == -1)
		goto func_err;
	else if (st == 1){
		if ((net_rights = agt__ufs_rights(pathname)) == -1)
			goto func_err;
		else
			goto func_success;
	}

	if (agt__acl_get_manager_types(&size_used,&num_types,&manager_types) == -1){
		agt__acl_handle_delete();
		goto func_err;
	}


	sec_acl_get_access(h,&manager_types,&net_rights,&status);
	if (status != 0){
		d_errno = DEUNKNOWN;
		sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25193,status,user_info.uid);
		sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
		AgentMsg(LOG_WARNING,msg.buf);
		agt__acl_handle_delete();
		goto func_err;
	}

        if ((user_info.uid == 0) || (user_info.uid == 100))
                net_rights |= sec_acl_perm_control;

        if (agt__acl_handle_delete() != 0)
                goto func_err;

func_success:

	net_rights = htonl(net_rights);
	memset(senddata,0,8);
	memcpy(&senddata,(char *)&net_rights,sizeof(net_rights));
	agt_usrc_send_data(senddata,8,DFAM_RIGHTS_R);
	return(0);

func_err:


	memcpy(msg.buf,(char *)&pathlen,4);
	memcpy(&msg.buf[4],pathname,pathlen);
	AgentTrace(TRACEERR,"DRIGHT",pathlen+4,msg.buf,0);
	agt_usrc_send_emsg(senddata,DFAM_RIGHTS_R);
       	return(-1);
}


/**************************************************************************/
/* ID   = agt__acl_ufs_get						  */
/* name = Getting ACL information                                         */
/* func = This function gets the status information of a UFS file,        */
/*        converts the information into LFS information, and sets the     */
/*        LFS information to sendacl, a global variable.                  */
/* i/o  = none                                                            */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none          						  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_ufs_get()
{
	struct stat buf;
	unsigned int ufstrusty_bit;
	sec_rgy_name_t user_name;
	sec_rgy_name_t group_name;
	unsigned int acl_num;
	size_t namelen;
	unsigned int pnamelen,gnamelen;
	short type_user_obj = 0x0001;
	short type_group_obj = 0x0004;
	short type_other_obj = 0x0010;

	acl_num = 0;


	length += 4;


	sendacl += 16;

	if (stat(pathname,&buf) == -1){
		switch(errno){
			case EFAULT:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25101,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DSTAT,d_errno);
				return(-1);
		}
	}


	ufstrusty_bit = 0;


	if ((option == USER) || (option == BOTH)){


		if ((buf.st_mode & 0100) != 0)
			ufstrusty_bit += 0x0004;
		if ((buf.st_mode & 0200) != 0){
			if ((buf.st_mode & S_IFMT) == S_IFDIR)
				ufstrusty_bit += 0x0032;
			else
				ufstrusty_bit += 0x0002;
		}
		if ((buf.st_mode & 0400) != 0)
			ufstrusty_bit += 0x0001;
		ufstrusty_bit += 0x0008;


		if (agt__acl_chang_uid_name(buf.st_uid,user_name,&pnamelen) == -1)
			return(-1);


		if (pnamelen != 0xffffffff)
			length += sizeof(ufstrusty_bit) + sizeof(user_type) + sizeof(pnamelen) + pnamelen;
		else
			length += sizeof(ufstrusty_bit) + sizeof(user_type) + sizeof(pnamelen);
		if (length > acl_size){
			overflag = 1;
			goto next;
		}


		ufstrusty_bit = htonl(ufstrusty_bit);
		memcpy(sendacl,(char *)&ufstrusty_bit,sizeof(ufstrusty_bit));
		sendacl += sizeof(ufstrusty_bit);

		type_user_obj = htons(type_user_obj);
		memcpy(sendacl,(char *)&type_user_obj,sizeof(type_user_obj));
		sendacl += sizeof(type_user_obj);

		pnamelen = htonl(pnamelen);
		memcpy(sendacl,(char *)&pnamelen,sizeof(pnamelen));
		sendacl += sizeof(pnamelen);
		pnamelen = ntohl(pnamelen);

		if (pnamelen != 0xffffffff){
			memcpy(sendacl,(char *)user_name,pnamelen);
			sendacl += pnamelen;
		}
		acl_num++;
	}

next:
	if ((option == GROUP) || (option == BOTH)){


		ufstrusty_bit = 0;
		if ((buf.st_mode & 0010) != 0)
			ufstrusty_bit += 0x0004;
		if ((buf.st_mode & 0020) != 0){
			if ((buf.st_mode & S_IFMT) == S_IFDIR)
				ufstrusty_bit += 0x0032;
			else
				ufstrusty_bit += 0x0002;
		}
		if ((buf.st_mode & 0040) != 0)
			ufstrusty_bit += 0x0001;


		if (agt__acl_chang_gid_group(buf.st_gid,group_name,&gnamelen) == -1)
			return(-1);


		if (gnamelen != 0xffffffff)
			length += sizeof(ufstrusty_bit) + sizeof(group_type) + sizeof(gnamelen) + gnamelen;
		else
			length += sizeof(ufstrusty_bit) + sizeof(group_type) + sizeof(gnamelen);
		if (length > acl_size){
			overflag = 1;
			goto next2;
		}


		ufstrusty_bit = htonl(ufstrusty_bit);
		memcpy(sendacl,(char *)&ufstrusty_bit,sizeof(ufstrusty_bit));
		sendacl += sizeof(ufstrusty_bit);

		type_group_obj = htons(type_group_obj);
		memcpy(sendacl,(char *)&type_group_obj,sizeof(type_group_obj));
		sendacl += sizeof(type_group_obj);

		gnamelen = htonl(gnamelen);
		memcpy(sendacl,(char *)&gnamelen,sizeof(gnamelen));
		sendacl += sizeof(gnamelen);
		gnamelen = ntohl(gnamelen);

		if (gnamelen != 0xffffffff){
			memcpy(sendacl,(char *)group_name,gnamelen);
			sendacl += gnamelen;
		}

		acl_num++;
next2:


		ufstrusty_bit = 0;
		if ((buf.st_mode & 0001) != 0)
			ufstrusty_bit += 0x0004;
		if ((buf.st_mode & 0002) != 0){
			if ((buf.st_mode & S_IFMT) == S_IFDIR)
				ufstrusty_bit += 0x0032;
			else
				ufstrusty_bit += 0x0002;
		}
		if ((buf.st_mode & 0004) != 0)
			ufstrusty_bit += 0x0001;

		namelen = strlen(DFA_OTHER_GROUP);


		length += sizeof(ufstrusty_bit) + sizeof(group_type) + sizeof(namelen) + namelen;
		if (length > acl_size){
			overflag = 1;
			return(0);
		}


		ufstrusty_bit = htonl(ufstrusty_bit);
		memcpy(sendacl,(char *)&ufstrusty_bit,sizeof(ufstrusty_bit));
		sendacl += sizeof(ufstrusty_bit);

		type_other_obj = htons(type_other_obj);
		memcpy(sendacl,(char *)&type_other_obj,sizeof(type_other_obj));
		sendacl += sizeof(type_other_obj);

		namelen = htonl(namelen);
		memcpy(sendacl,(char *)&namelen,sizeof(namelen));
		sendacl += sizeof(namelen);

		namelen = ntohl(namelen);
		memcpy(sendacl,DFA_OTHER_GROUP,namelen);
		sendacl += namelen;

		acl_num++;
	}


	length = htonl(length);
	memcpy((begin_sendacl+8),(char *)&length,sizeof(length));


	acl_num = htonl(acl_num);
	memcpy((begin_sendacl+12),(char *)&acl_num,sizeof(acl_num));
	return(0);
}


/**************************************************************************/
/* ID   = agt__acl_chang_uid_name					  */ 
/* name = uid conversion to user name                                     */
/* func = This function converts uid information into uuid information,   */
/*        and converts the uuid information into a user name.             */
/* i/o  = uid       :uid_t         :IN : uid information                  */
/*        princ_name:sec_rgy_name_t:OUT: user name                        */
/*        pnamelen  :unsigned int *:OUT: user name length                 */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note =  If uid has been registered to the DCE security registry        */
/*         database, this function sets -1 to pnamelen.                   */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_chang_uid_name(uid_t uid,sec_rgy_name_t princ_name,unsigned int *pnamelen)
{
	uuid_t st_uuid;
	sec_rgy_name_t globalname;
	sec_rgy_name_t cellname;


	sec_rgy_pgo_unix_num_to_id(context,sec_rgy_domain_person,uid,&st_uuid,&status);
	if (status != error_status_ok){
	        switch(status){
			case sec_rgy_object_not_found:
				goto nouser;
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECUNIXNUMTOID,d_errno);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25177,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				return(-1);
		}
	}


	sec_rgy_pgo_id_to_name(context,sec_rgy_domain_person,&st_uuid,princ_name,&status);
	if (status != error_status_ok){
		switch(status){
			case sec_rgy_object_not_found:
				goto nouser;
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECIDGENNAME,d_errno);
				return(-1);
			case sec_id_e_name_too_long:
				d_errno = ENAMETOOLONG;
				agt_usrc_oserr_trace(SECIDGENNAME,d_errno);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25178,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				return(-1);
		}
	}
	*pnamelen = strlen((char *)princ_name);
	return(0);

nouser:
	*pnamelen = 0xffffffff;
	return(0);
}

/**************************************************************************/
/* ID   = agt__acl_chang_gid_group		                          */
/* name = gid conversion to group name                                    */
/* func = This function converts gid information into uuid information    */
/*        and then converts uuid information into a group name            */
/* io =                                                                   */
/*	gid	  :uid_t             :IN :gid information                 */
/*	group_name:sec_rgy_name_t:OUT:group name                          */
/*	gnamelen  :unsigned int	 :OUT:group name length                   */
/* return =                                                               */
/*	0 :normal end                                                     */
/*	-1:abnormal end                                                   */
/* note = If gid has been registered in the security database, gnamelen   */ 
/*        is set to -1.                                                   */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_chang_gid_group(uid_t gid, sec_rgy_name_t group_name,unsigned int *gnamelen)
{
	uuid_t st_uuid;
	sec_rgy_name_t globalname;
	sec_rgy_name_t cellname;


	sec_rgy_pgo_unix_num_to_id(context,sec_rgy_domain_group,gid,&st_uuid,&status);
	if (status != error_status_ok){
		switch(status){
			case sec_rgy_object_not_found:
				goto nogroup;
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECUNIXNUMTOID,d_errno);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25179,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR, msg.buf);
				return(-1);
		}
	}


	sec_rgy_pgo_id_to_name(context,sec_rgy_domain_group,&st_uuid,group_name,&status);
	if (status != error_status_ok){
		switch(status){
			case sec_rgy_object_not_found:
				goto nogroup;
			case sec_rgy_server_unavailable:
				d_errno = DEUNAVAILSRV;
				agt_usrc_oserr_trace(SECIDGENGROUP,d_errno);
				return(-1);
			case sec_id_e_name_too_long:
				d_errno = ENAMETOOLONG;
				agt_usrc_oserr_trace(SECIDGENGROUP,d_errno);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25181,status,user_info.uid);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				return(-1);
		}
	}
	*gnamelen = strlen((char *)group_name);
	return(0);

nogroup:
	*gnamelen = 0xffffffff;
	return(0);
}

/**************************************************************************/
/* ID   = agt__acl_ufs_set						  */
/* name = Changing UFS file mode bit					  */
/* func = If the user is the file owner, the function converts the ACL    */
/*        information about LFS file into the mode bit of the correspon-  */
/*        ding UFS file. Finally, the mode bit of the UFS file is changed */
/* i/o  = princ_name:char *:IN: principal name                            */
/* return = 0: normal end                                                 */
/*         -1: abnormal end                                               */
/* note = none                                   			  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__acl_ufs_set(char *princ_name)
{
	sec_rgy_name_t owner_name;
	sec_rgy_name_t group_name;
	struct stat buf;
	unsigned int pnamelen,gnamelen;

	pnamelen = gnamelen = 0;

	if (stat(pathname,&buf) == -1){
		switch(errno){
			case EFAULT:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25102,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DSTAT,d_errno);
				return(-1);
		}
	}

	if (option == USER){

		if (agt__acl_chang_uid_name(buf.st_uid,owner_name,&pnamelen) == -1)
			return(-1);


		if (pnamelen == 0xffffffff){
			d_errno = EOPNOTSUPP;
			return(-1);
		}
		else if (strcmp(princ_name,(char *)owner_name) != 0){
			d_errno = EOPNOTSUPP;
			return(-1);
		}


		buf.st_mode &= 0177077;


		if ((buf.st_mode & S_IFMT) == S_IFDIR){
			if ((trusty_bit & 0x0001) != 0)
				buf.st_mode |= 0400;
			if ((trusty_bit & 0x0032) != 0)
				buf.st_mode |= 0200;
			if ((trusty_bit & 0x0004) != 0)
				buf.st_mode |= 0100;
		}
		else{
			if ((trusty_bit & 0x0030) != 0){
				d_errno = EOPNOTSUPP;
				sprintf(msg.buf,"trusty_bit contains I or D for UFS File\n");
				AgentTrace(TRACEDEB,"ACL   ",strlen(msg.buf),msg.buf,0);
				return(-1);
			}
			if ((trusty_bit & 0x0001) != 0)
				buf.st_mode |= 0400;
			if ((trusty_bit & 0x0002) != 0)
				buf.st_mode |= 0200;
			if ((trusty_bit & 0x0004) != 0)
				buf.st_mode |= 0100;
		}
		goto change_mode;
	}
	else if(option == GROUP){
		if (strcmp(princ_name,DFA_OTHER_GROUP) == 0){

			buf.st_mode &= 0177770;


			if ((buf.st_mode & S_IFMT) == S_IFDIR){
				if ((trusty_bit & 0x0001) != 0)
					buf.st_mode |= 0004;
				if ((trusty_bit & 0x0032) != 0)
					buf.st_mode |= 0002;
				if ((trusty_bit & 0x0004) != 0)
					buf.st_mode |= 0001;
			}
			else{
				if ((trusty_bit & 0x0030) != 0){
					d_errno = EOPNOTSUPP;
					sprintf(msg.buf,"trusty_bit contains I or D for UFS File\n");
					AgentTrace(TRACEDEB,"ACL   ",strlen(msg.buf),msg.buf,0);
					return(-1);
				}
				if ((trusty_bit & 0x0001) != 0)
					buf.st_mode |= 0004;
				if ((trusty_bit & 0x0002) != 0)
					buf.st_mode |= 0002;
				if ((trusty_bit & 0x0004) != 0)
					buf.st_mode |= 0001;
			}
			goto change_mode;
		}

		if (agt__acl_chang_gid_group(buf.st_gid,group_name,&gnamelen) == -1)
			return(-1);


		if (gnamelen == 0xffffffff){
			d_errno = EOPNOTSUPP;
			return(-1);
		}
		if (strcmp(princ_name,(char *)group_name) != 0){
			d_errno = EOPNOTSUPP;
			return(-1);
		}


		buf.st_mode &= 0177707;


		if ((buf.st_mode & S_IFMT) == S_IFDIR){
			if ((trusty_bit & 0x0001) != 0)
				buf.st_mode |= 0040;
			if ((trusty_bit & 0x0032) != 0)
				buf.st_mode |= 0020;
			if ((trusty_bit & 0x0004) != 0)
				buf.st_mode |= 0010;
		}
		else{
			if ((trusty_bit & 0x0030) != 0){
				d_errno = EOPNOTSUPP;
				sprintf(msg.buf,"trusty_bit contains I or D for UFS File\n");
				return(-1);
			}

			if ((trusty_bit & 0x0001) != 0)
				buf.st_mode |= 0040;
			if ((trusty_bit & 0x0002) != 0)
				buf.st_mode |= 0020;
			if ((trusty_bit & 0x0004) != 0)
				buf.st_mode |= 0010;
		}
	}

change_mode:
	if (chmod(pathname,buf.st_mode) == -1){
		switch(errno){
			case EFAULT:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25103,errno,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DCHMOD,d_errno);
				return(-1);
		}
	}
	return(0);
}

/*****************************************************************************/
/* ID	= agt__acl_handle_creat						     */
/*                                                                           */
/* name = ACL handle getting function                                        */
/* func = Gets the ACL information of the file handle.  If this function     */
/*        failed to get the ACL file handle, but the access system call was  */
/*        successfully, this function assumes that the target file is UFS.   */
/*                                                                           */
/* i/o  = none                                                               */
/*                                                                           */
/* return =  0: normal end                                                   */
/*           1: the accessed file is USF                                     */
/*          -1: abnormal end                                                 */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static int agt__acl_handle_creat()
{
	sec_acl_bind((unsigned char *)pathname,0,&h,&status);
	if (status != 0){

		if (access(pathname,F_OK) != 0){
			switch(errno){
				case EFAULT:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25182,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_WARNING,msg.buf);
					agt_abort();
				default:
					d_errno = errno;
					agt_usrc_oserr_trace(DACCESS,d_errno);
					return(-1);
			}
		}

		switch(status){

			case sec_acl_object_not_found:
			case sec_acl_no_acl_found:
				d_errno = EOPNOTSUPP;
				return(1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25183,status,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				return(-1);
		}
	}
	return(0);
}

/*****************************************************************************/
/* ID	= agt__acl_handle_delete					     */
/*									     */
/* name = ACL handle deleting function                                       */
/* func = This function delete handle of sec_acl_bind.     		     */
/*                                                                           */
/* i/o  = none                                                               */
/*                                                                           */
/* return =  0: normal end                                                   */
/*          -1: abnormal end                                                 */
/* note   = none                                                             */
/* date   = October 24, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static int agt__acl_handle_delete()
{
	sec_acl_release_handle(&h,&status);
	if (status != error_status_ok){
		sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25206,status,user_info.uid);
		sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
		AgentMsg(LOG_ERR,msg.buf);
		return(-1);
	}
	return(0);
}

/*****************************************************************************/
/* ID	= agt__acl_context_delete				             */	
/* name = Deleting context handle					     */
/* func = This function delete handle of sec_rgy_site_open.		     */
/* i/o  = none								     */
/* return = none           						     */
/* note = none							             */	
/* date = October 20, 1995 by Tetsuya Ichikawa				     */
/* Trade Secret of Hitachi Ltd.						     */
/* Do not disclose without written concession from Hitachi, Ltd.             */	
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved. Derechos Reservados.    */ 
/*****************************************************************************/
static void agt__acl_context_delete()
{
	sec_rgy_site_close(context,&status);
	if (status != error_status_ok){
		switch(status){
			case sec_rgy_server_unavailable:
				agt_usrc_oserr_trace(SECACLSITECLOSE,DEUNAVAILSRV);
				break;
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25209,status,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}
}

/*****************************************************************************/
/* ID   = agt__acl_handle_release                                            */
/* name = Releasing ACL strage                                               */
/* func = This function release ACL strage of sec_acl_lookup.                */
/* i/o  = sec_acl_p :sec_acl_t *:IN:Pointer of releasing ACL strage	     */
/* return = none                                                             */
/* note = none                                                               */
/* date = October 31, 1995 by Tetsuya Ichikawa                               */
/* Trade Secret of Hitachi Ltd.                                              */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd. All Rights Reserved. Derechos Reservados.    */
/*****************************************************************************/
static int agt__acl_handle_release(sec_acl_t *sec_acl_p)
{
	sec_acl_release(h,sec_acl_p,&status);
	if (status != error_status_ok){
		sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25198,status,user_info.uid);
		sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
		AgentMsg(LOG_ERR,msg.buf);
		return(-1);
	}
	return(0);
}

/*****************************************************************************/
/* ID	= agt__acl_get_manager_types					     */
/*                                                                           */
/* name = ACL manager type extraction function                               */
/* func = Gets the ACL manager type, that administrates the ACL information  */
/*        of the file.                                                       */
/*                                                                           */
/* i/o  = size_used    :unsigned32 *:OUT: the number of output entries       */
/*                                        returned in manager_types          */
/*	  num_types    :unsigned32 *:OUT: the number of the actual manager   */
/*                                        types returned in manager_types    */
/*        manager_types:uuid_t      :OUT: the array to store UUID (uuid_t    */
/*                                        data type) which identifies the    */
/*                                        type of the ACL manager (this is   */
/*                                        to protect objects)                */
/*                                                                           */
/* return =  0: normal end                                                   */
/*          -1: abnormal end                                                 */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static int agt__acl_get_manager_types(unsigned32 *size_used,unsigned32 *num_types,uuid_t manager_types[])
{
	sec_acl_get_manager_types(h,sec_acl_type_object,(unsigned32)1,size_used,num_types,manager_types,&status);
	if (status != 0){
		d_errno = DEUNKNOWN;
		sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25184,status,user_info.uid);
		sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
		AgentMsg(LOG_ERR,msg.buf);
		return(-1);
	}
	return(0);
}

/*****************************************************************************/
/* ID	= agt__acl_get_acl						     */
/*                                                                           */
/* name = ACL information extraction function                                */
/* func = Gets the ACL information of the file.                              */
/*                                                                           */
/* i/o  = manager_types:uuid_t      :INP: pointer to UUID, that identifies   */
/*                                        the target ACL manager type        */
/*        sec_acl_list :sec_acl_list_t *:OUT: pointer to the structure,      */
/*                                        sec_acl_list_t (for getting the    */
/*                                        ACL information)                   */
/*                                                                           */
/* return =  0: normal end                                                   */
/*          -1: abnormal end                                                 */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static int agt__acl_get_acl(uuid_t *manager_types,sec_acl_list_t *sec_acl_list)
{
	sec_acl_lookup(h,manager_types,sec_acl_type_object,sec_acl_list,&status);
	if (status != 0){
		switch(status){
			case sec_acl_unknown_manager_type:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25185,status,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			case sec_acl_cant_allocate_memory:
				d_errno = ENOMEM;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25186,status,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				return(-1);
			default:
				d_errno = DEUNKNOWN;
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25187,status,user_info.uid);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				return(-1);
		}
	}
	return(0);
}

/*****************************************************************************/
/* ID	= agt__ufs_rights						     */
/*                                                                           */
/* name = Getting effective rights for a UFS file                            */
/* func = Gets effective rights for a UFS file, and maps the extracted       */
/*        information to the effective information of LFS.                   */
/*                                                                           */
/* i/o  = path: char *:INP: the file name, whose effective rights are to be  */
/*                          extracted                                        */
/*                                                                           */
/* return = right: effective rights extracted                                */
/*          -1: abnormal end                                                 */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static sec_acl_permset_t agt__ufs_rights(char *path)
{
	int count,amode;
	struct stat pathinfo;
	sec_acl_permset_t rights = 0;

	if (stat(path,&pathinfo) == -1){
		switch(errno){
			case EFAULT:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25195,errno,user_info.uid,msg.wkbuf);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:
				d_errno = errno;
				agt_usrc_oserr_trace(DSTAT,d_errno);
				return(-1);
		}
	}

	for (count =0; count < 3; count++){
		switch(count){
			case 0:
				amode = R_OK;
				break;
			case 1:
				amode = W_OK;
				break;
			case 2:
				amode = X_OK;	
				break;
		}

		if (access(path,amode) == -1){
			switch(errno){
				case EACCES:
					continue;
				case EFAULT:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25194,errno,user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
				default:
					d_errno = errno;
					return(-1);
			}
		}
		
		switch(amode){
			case R_OK:
				rights |= sec_acl_perm_read;
				break;
			case W_OK:
				if ((pathinfo.st_mode & S_IFMT) == S_IFDIR)
					rights |= (sec_acl_perm_write + sec_acl_perm_insert + sec_acl_perm_delete);
				else
					rights |= sec_acl_perm_write;
					
				break;
			case X_OK:
				rights |= sec_acl_perm_execute;
				break;
		}
	}

	if ((pathinfo.st_uid == user_info.uid) || (user_info.uid == 0) || (user_info.uid == 100)) 
		 rights |= sec_acl_perm_control;

	return(rights);
}
