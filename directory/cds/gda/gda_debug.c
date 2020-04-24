/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_debug.c,v $
 * Revision 1.1.9.2  1996/02/18  19:31:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:12  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:12:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:25  root]
 * 
 * Revision 1.1.6.5  1994/08/03  19:01:36  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:53:51  mccann]
 * 
 * Revision 1.1.6.4  1994/06/23  18:30:01  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:27:31  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:38:03  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:12  devsrc]
 * 
 * Revision 1.1.6.2  1994/05/12  21:11:13  peckham
 * 	Change _DNS_DEFAULT_STACK_ reference to _CDS_DEFAULT_STACK_
 * 	[1994/05/12  19:14:52  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:01:35  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:08:52  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  13:27:05  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:43:38  zeliff]
 * 
 * Revision 1.1.4.2  1992/09/29  19:12:31  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:12:06  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 */

#include <gda.h>
#include <dce/cds_basic_types.h>

#if defined(DCE_CDS_DEBUG)

#include <dce/assert.h>
#include <gda_main.h>
#include <sys/time.h>
#include <dce/utc.h>
#include <errno.h>

/*
 * local prototypes
 */
static void
print_memstat(
	      utc_t *);

static void 
memstat (
	 void);


struct memory_stat	mem_stat;
pthread_mutex_t		memstat_mutex;
pthread_cond_t		memstat_cond;

#endif /* if defined(DCE_CDS_DEBUG) */

int			memstat_interval = 120;

#if defined(DCE_CDS_DEBUG)

#define INIT_COUNTER(X) \
	{ \
		int looper; \
		mem_stat.X.total = 0; \
		for(looper = 0; looper < COUNTER_MAX; \
			mem_stat.X.allocd[looper] = 0, \
			looper++); \
		for(looper = 1; looper < COUNTER_MAX; \
			mem_stat.X.freed[looper] = 0, \
			looper++); \
		if(pthread_mutex_init(&mem_stat.X.mutex, \
			pthread_mutexattr_default)) \
		{ \
		      dce_svc_printf(CDS_S_PTHREADMUTEXINIT_MSG, (long)errno); \
                      error_exit(-1);		\
		} \
	}

#define PRINT_MEM_STAT(X, Y, Z, A) \
	{ \
		int	looper; \
		char	buf[2048]; \
		char	*cp = buf; \
		if(pthread_mutex_lock(&mem_stat.X.mutex)) \
		{ \
		      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_MSG, (long)errno);	\
                      error_exit(-1);		\
		} \
		sprintf(cp,"\n%s alloc %ld free %ld total %ld", \
			A, \
			mem_stat.X.allocd[0], \
			mem_stat.X.freed[0], \
			mem_stat.X.total); \
		cp += strlen(cp); \
		sprintf(cp, "\nALLOC,"); \
		cp += strlen(cp); \
		for(looper = 1; looper <= Y; \
			sprintf(cp, "%ld,", \
				mem_stat.X.allocd[looper]), \
				cp += strlen(cp), \
				looper++); \
		sprintf(cp, "\nFREE,"); \
		cp += strlen(cp); \
		for(looper = 1; looper <= Z; \
			sprintf(cp, "%ld,", \
				mem_stat.X.freed[looper]), \
				cp += strlen(cp), \
				looper++); \
		sprintf(cp, ""); \
		cp += strlen(cp); \
		DCE_SVC_DEBUG(( 	\
		    cds__svc_handle, 	\
		    cds_svc_gda, 	\
		    svc_c_debug7, 	\
		    "print_mem_stat : %s",	\
                    buf)); \
		if(pthread_mutex_unlock(&mem_stat.X.mutex)) \
		{ \
		      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_MSG, (long)errno);	\
                      error_exit(-1);		\
		} \
	}

static void
print_memstat(utc_t *utc_time)
{
    char	buf[128];


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
	" >print_memstat"));

    strcpy(buf, "\nMEM_STAT struct ");
    if (0 <= utc_asclocaltime(&buf[17], sizeof(buf)-17, utc_time))
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug5,
	    "print_memstat: %s",
	    buf));
    }
    
    PRINT_MEM_STAT(rr_el_t,8,7,"rr_el_t")
    PRINT_MEM_STAT(list_t,4,9,"list_t")
    PRINT_MEM_STAT(sendbuf,1,3,"sendbuf")
    PRINT_MEM_STAT(replicas_q,1,1,"replicas_q")
    PRINT_MEM_STAT(realloc_replicas_q,1,0,"realloc_replicas_q")
    PRINT_MEM_STAT(ss_cds_ReplicaPointer_t,1,0,"ss_cds_ReplicaPointer_t")
    PRINT_MEM_STAT(debug_el_t,1,1,"debug_el_t")
    PRINT_MEM_STAT(element_t,2,2,"element_t")
    PRINT_MEM_STAT(ss_cds_Set_t,1,0,"ss_cds_Set_t")
    PRINT_MEM_STAT(question_el_t,1,1,"question_el_t")
    PRINT_MEM_STAT(cds_FullName_t,1,1,"cds_FullName_t")

    DCE_SVC_DEBUG(( 
        cds__svc_handle, 
        cds_svc_gda, 
        svc_c_debug2,
	" <print_memstat"));
}


static void 
memstat (void)
{
      utc_t 	utc_time;
      int 	status;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >memstat"));

	/*
	 * initialize the thread specific data 
	 */
	if(pthread_setspecific(thread_num, (pthread_addr_t)&memstat_th_num))
	{
	    dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	    error_exit(-1);
	}

	if(pthread_mutex_lock(&memstat_mutex))
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_MSG, (long)errno);
	    error_exit(-1);
	}

        status = utc_gettime(&utc_time);
        dce_assert(cds__svc_handle, (0 <= status));

	for(;;)
	{		
	        timespec_t time, inacc;
		long tdf;
		int temp;

		temp = utc_bintime(&time, &inacc, &tdf, &utc_time);
		dce_assert(cds__svc_handle,
			   (0 <= temp));
	        time.tv_sec += memstat_interval;

		if(pthread_cond_timedwait(&memstat_cond,
			&memstat_mutex, &time) && errno != EAGAIN)
		{
		    dce_svc_printf(CDS_S_PTHREADCONDTIMEDWAIT_MSG, (long)errno); 		
		    error_exit(-1);
		}


                status = utc_gettime(&utc_time);
		dce_assert(cds__svc_handle, (0 <= status));

                print_memstat(&utc_time);
	}
	
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <memstat"));
}

void
init_memstat (void)
{
	pthread_t	memstat_th;
	pthread_attr_t  pattr;


	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >init_memstat"));

	INIT_COUNTER(rr_el_t)
	INIT_COUNTER(list_t)
	INIT_COUNTER(sendbuf)
	INIT_COUNTER(replicas_q)
	INIT_COUNTER(realloc_replicas_q)
	INIT_COUNTER(ss_cds_ReplicaPointer_t)
	INIT_COUNTER(debug_el_t)
	INIT_COUNTER(element_t)
	INIT_COUNTER(ss_cds_Set_t)
	INIT_COUNTER(question_el_t)
	INIT_COUNTER(cds_FullName_t)

	if (pthread_attr_create(&pattr) < 0) 
	{
	    dce_svc_printf(CDS_S_PTHREADATTRCREATE_MSG, (long)errno);  
	    error_exit(-1);
	}
	if (pthread_attr_setstacksize(&pattr, _CDS_DEFAULT_STACK_) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADATTRSETSTACKSIZE_MSG,(long)errno);  
	    error_exit(-1);
	}	
	if(pthread_mutex_init(&memstat_mutex, pthread_mutexattr_default))
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXINIT_MSG, (long)errno);  
	    error_exit(-1);
	}

	if(pthread_cond_init(&memstat_cond, pthread_condattr_default))
	{
	    dce_svc_printf(CDS_S_PTHREADCONDINIT_MSG, (long)errno);  
	    error_exit(-1);
	}
	if(pthread_create(&memstat_th, pattr,
		(pthread_startroutine_t)memstat, (pthread_addr_t)NULL))
	{
	    dce_svc_printf(CDS_S_PTHREADCREATE_MSG, (long)errno);  
	    error_exit(-1);
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <init_memstat"));
}

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <stdio.h>

static char *x types[] = {
	"?",
	"A",
	"NS",
	"MD",
	"MF",
	"CNAME",
	"SOA",
	"MB",
	"MG",
	"MR",
	"NULL",
	"WKS",
	"PTR",
	"HINFO",
	"MINFO",
	"MX",
	"TXT"
	};

#if !defined(T_TXT)
# define T_TXT 16
#endif /* !defined(T_TXT) */

static char buffer[512];

static int last_type;

static int ttl_4byte = 1;

/* Return next valid index */
int
my_dn_expand (char* msg, int msglen, int cindex, char* expbuf)
{
	int index;
	int seglen;
	int outindex;
	int result;
	
	index = cindex;
	outindex = 0;
	result = 0;
	
	while (msg[index] != 0)
		{
		seglen = msg[index++];
		if (seglen & 0xc0)
			{
			seglen = seglen &0x3f;
			seglen = seglen << 8;
			seglen |= msg[index++];
			if (result == 0)
				result = index;
			index = seglen;
			seglen = msg[index++];
			}
		while (seglen > 0)
			{
			expbuf[outindex++] = msg[index++];
			seglen--;
			}
		if (msg[index] != 0)
			expbuf[outindex++] = '.';
		}
	if (result == 0)
		result = index + 1;
	expbuf[outindex] = '\0';
	return result;
}

void
dump_type (char* msgbuff, int index, FILE* f)
{
	unsigned int x;
	x = (unsigned int)(msgbuff[index++] << 8);
	x |= (unsigned char)msgbuff[index++];
	
	if (x == T_UINFO)
		fprintf(f, "UINFO;");
	else if (x == T_UID)
		fprintf(f,"UID;");
	else if (x == T_GID)
		fprintf(f,"GID;");
	else if (x == T_UNSPEC)
		fprintf(f,"USPEC;");
	else if (x == T_ANY)
		fprintf(f,"ANY;");
	else if (x > T_TXT)
		fprintf(f,"?type?=%d;", x);
	else
		fprintf(f,"%s;", types[x]);
	last_type = x;
}

void
dump_class (char* msgbuff, int index, FILE* f)
{
	int x;
	x = (msgbuff[index++] << 8);
	x |= msgbuff[index++];
	if (x == 1)
		fprintf(f,"IN\n");
	else
		fprintf(f,"CS\n");
}

int
dump_rr (char* msgbuff, int msglen, int cindex, FILE* f)
{
	int index;
	int x;
	int rdlength;
	int i;
	int all_chars;
	
	index = cindex;
	/* Dump name */
	index = my_dn_expand(msgbuff, msglen, index, buffer);
	if (x >= 0)
		{
		fprintf(f,"%s;", buffer);
		}
	else
		fprintf(f,"?Can't expand name;");

	/* Dump type */
	dump_type(msgbuff, index, f);
	index += 2;
	
	/* Dump class */
	dump_class(msgbuff, index, f);
	index += 2;
	
	/* Dump TTL */
	if (ttl_4byte)
		{
		x = (msgbuff[index] << 24);
		x |= (msgbuff[index + 1] << 16);
		x |= (msgbuff[index + 2] << 8);
		x |= msgbuff[index + 3];
		fprintf(f,"TTL=%d(4byte)\n", x);
		index += 4;
		}
	else
		{
		x = (msgbuff[index] << 8);
		x |= msgbuff[index + 1];
		fprintf(f,"TTL=%d(2byte)\n", x);
		index += 2;
		}

	/* Dump RDATA */
	rdlength = (msgbuff[index] << 8);
	rdlength |= (unsigned char)msgbuff[index + 1];
	fprintf(f,"RDLength=%d;", rdlength);
	index += 2;

	/* If this is an HINFO, be alert for two components */
	if (last_type == T_HINFO)
		{
		int length1;
		int length2;
		
		length1 = (unsigned char)msgbuff[index++];
		fprintf(f,"\"");
		for (i = 0; i < length1; i++)
			fprintf(f,"%c", (unsigned char)msgbuff[index + i]);
		fprintf(f,"\"");
		index += length1;
		if (length1 < rdlength - 1)
			{
			length2 = (unsigned char)msgbuff[index++];
			fprintf(f,",\"");
			for (i = 0; i < length2; i++)
				fprintf(f,"%c", (unsigned char)msgbuff[index + i]);
			fprintf(f,"\"");
			index += length2;
			}
		}
	else
		{
		/* Check to see if this thing can be dumped as a char string */
			printf("\"");
			for (i = 0; i < rdlength; i++)
				{
				if ((msgbuff[i] < ' ') ||
				    (msgbuff[i] > '~'))
					printf("|");
				else
					printf("%c", msgbuff[i]);
				}
			printf("\"\n");
		}

	fprintf(f,"\n");
	return index;
}

void
dump_address (struct sockaddr_in* sp, FILE* f)
{
	short family;
	short port;
	
	port = ntohs(sp->sin_port);
	fprintf(f,"%d.%d.%d.%d[%d]", 
		sp->sin_addr.S_un.S_un_b.s_b1,
		sp->sin_addr.S_un.S_un_b.s_b2,
		sp->sin_addr.S_un.S_un_b.s_b3,
		sp->sin_addr.S_un.S_un_b.s_b4, port);
}

void
dump_message (char* msgbuff, int msglen, FILE* f)
{
	int index;
	int x;
	
	index = 0;
	/* Dump header */
	x = (msgbuff[index] << 8);
	x |= msgbuff[index + 1];
	index += 2;
	fprintf(f,"HID=%d;", x);
	if (msgbuff[index] & 0x80)
		fprintf(f,"RESPONSE;");
	else
		{
		int opcode;
		opcode = (msgbuff[index] >> 3) & 0xf;
		switch (opcode)
			{
		case 0:
			fprintf(f,"QUERY;");
			break;
		case 1:
			fprintf(f,"IQUERY;");
			break;
		case 2:
			fprintf(f,"CQUERYM;");
			break;
		case 3:
			fprintf(f,"CQUERYU;");
			break;
		default:
			fprintf(f,"?QUERY;");
			break;
			}
		}
	if (msgbuff[index] & 0x4)
		fprintf(f,"AA;");
	if (msgbuff[index] & 0x2)
		fprintf(f,"TC;");
	if (msgbuff[index] & 0x1)
		fprintf(f,"RD;");

	index++;
	if (msgbuff[index] & 0x80)
		fprintf(f,"RA;");
	switch(msgbuff[index] & 0xf)
		{
	case 0:
		fprintf(f,"No error;");
		break;
	case 1:
		fprintf(f,"Format error;");
		break;
	case 2:
		fprintf(f,"Server error;");
		break;
	case 3:
		fprintf(f,"Name error;");
		break;
	case 4:
		fprintf(f,"Not implemented;");
		break;
	case 5:
		fprintf(f,"Refused;");
		break;
	default:
		fprintf(f,"?RCODE=%d;", msgbuff[index] & 0xf);
		break;
		}

	index++;
	x = (msgbuff[index++] << 8);
	x |= msgbuff[index++];
	fprintf(f,"QDCOUNT=%d;", x);
	x = (msgbuff[index++] << 8);
	x |= msgbuff[index++];
	fprintf(f,"ANCOUNT=%d;", x);
	x = (msgbuff[index++] << 8);
	x |= msgbuff[index++];
	fprintf(f,"NSCOUNT=%d;", x);
	x = (msgbuff[index++] << 8);
	x |= msgbuff[index++];
	fprintf(f,"ARCOUNT=%d\n", x);
	/* Dump question */
	index = my_dn_expand(msgbuff, msglen, index, buffer);
	if (x >= 0)
		{
		fprintf(f,"%s;", buffer);
		}
	else
		fprintf(f,"?Can't expand name;");

	dump_type(msgbuff, index, f);
	index += 2;
	
	dump_class(msgbuff, index, f);
	index += 2;
	
	/* Dump answer */
	index = dump_rr(msgbuff, msglen, index, f);

	/* Dump authority */
	/* Dump additional */
}

void
dump_towers (char* tower_p, FILE* f)
{
	int len,cnt;
	unsigned char string[100];
	char *binding_p = (char *)string;

	cnt = GETL16(tower_p);	/* number of towers */
	cnt = cnt / 2;
	fprintf(f, "    Towers(%d):\n", cnt);

	for (cnt; cnt > 0; --cnt) 
		{
		len = GETL16(tower_p);      /* size of the tower */
		if ( get_binding_string ( tower_p, binding_p ))
			fprintf(f, "        %s\n", string );
		else 
			{		/* if no protocol binding, dump hex data */
			for ( len; len > 1; --len )
			fprintf(f, "%02x-", *tower_p++);
			fprintf(f, "%02x\n", *tower_p++);	/* Last value without - */
			len = 0;			/* We're pointing to next tower */
			}
		tower_p += len;
		}
}


void
dump_progress_rec (cds_Progress_t* p, FILE* f)
{
	int i;
	cds_SetMember_t *member_p;
	cds_Timestamp_t context;

	fprintf(f, "CDS Progress record<0x%x>:\n", p);
	
	/* Print out returned Progress record */
	fprintf(f, "Flags: ");
	if (p->pr_flags & 1)
	    fprintf(f, "done ");
	if (p->pr_flags & 2)
	    fprintf(f, "up ");
	if (p->pr_flags & 4)
	    fprintf(f, "linked ");
	if (p->pr_flags & 8)
	    fprintf(f, "hitlink ");
	if (p->pr_flags & 16)
	    fprintf(f, "ignorestate");

	/* avoid dns* */
	p->pr_resolved.fn_name[p->pr_resolved.fn_length] = '\0';

	fprintf(f, "\nResolvedName: %s\n", p->pr_resolved.fn_name);

	/* avoid dns* */
	p->pr_unresolved.fn_name[p->pr_unresolved.fn_length] = '\0';

	fprintf(f, "UnResolvedName: %s\n", p->pr_unresolved.fn_name);

	if (p->pr_replicas_p != (cds_Set_t *)NULL) 
		{
		if (p->pr_replicas_p->set_type != 3)
			fprintf(f, "Unexpected set_type %d\n", p->pr_replicas_p->set_type);
		/* display each replica */
		fprintf(f, "Replicas: \n");
		for (i=0; i<(int)p->pr_replicas_p->set_length; ++i) 
			{
			member_p = &p->pr_replicas_p->set_members[i];
			if (member_p->sm_flag != 1)
				fprintf(f, "Bad set member flag for member %d\n", i);
			if (memcmp(&member_p->sm_ts, &context, sizeof(context)) !=0)
				fprintf(f, "Non-null member timestamp for member %d\n",i);
			if (member_p->sm_value.av_valuetype != VT_ReplicaPointer)
				fprintf(f, "Bad value type(%d) for member %d\n",
					member_p->sm_value.av_valuetype, i);
			else 
				{
				}
			}
		} 
	else
		fprintf(f, "No replica set returned\n");
}

void
dump_list (list_t* l, FILE* f)
{
	element_t* eptr;
	
	fprintf(f, ">Dumping list <0x%x>:\n", l);
	eptr = l->head;
	while (eptr != 0)
		{
		fprintf(f, ">>> rec=0x%x;data=0x%x\n", eptr, eptr->data_el);
		eptr = eptr->next_el;
		}
	fprintf(f, ">End of list <0x%x>\n", l);
}


void
debug_dump_list (list_t* l)
{
	dump_list(l, stderr);
}
		

void
uuid_print (uuid_t* uptr)
{
	char* xxx;
	long status;
	uuid_to_string(uptr, &xxx, &status);
	fprintf(stderr, "%s", xxx);
}

#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */

#endif /* if defined(DCE_CDS_DEBUG) */
