/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_bind.c,v $
 * Revision 1.1.18.2  1996/02/18  19:30:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:04  marty]
 *
 * Revision 1.1.18.1  1995/12/08  15:12:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:15  root]
 * 
 * Revision 1.1.15.11  1994/08/24  20:27:40  hopkins
 * 	Fix OT # 5834 -- in send_recv_vc(), don't send named
 * 	a buffer that's bigger than the actual question message.
 * 	Also, remember to free the sendbuf before returning.
 * 	[1994/08/24  20:27:06  hopkins]
 * 
 * Revision 1.1.15.10  1994/08/03  19:01:30  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:52:16  mccann]
 * 
 * Revision 1.1.15.9  1994/06/23  19:10:00  jd
 * 	Removed extern evtblk. It no longer exists
 * 	[1994/06/23  18:59:03  jd]
 * 
 * Revision 1.1.15.8  1994/06/23  18:29:57  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:27:13  mccann]
 * 
 * Revision 1.1.15.7  1994/06/09  18:37:56  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:01  devsrc]
 * 
 * Revision 1.1.15.6  1994/05/25  22:22:26  peckham
 * 	Take operations with side-effects out of dce_assert().
 * 	[1994/05/25  22:18:37  peckham]
 * 
 * Revision 1.1.15.5  1994/05/12  21:11:07  peckham
 * 	Change _DNS_DEFAULT_STACK_ reference to _CDS_DEFAULT_STACK_
 * 	Change gettimeofday() call to utc_gettime()/utc_bintime().
 * 	Eliminate dnstables reference.
 * 	[1994/05/12  19:14:36  peckham]
 * 
 * Revision 1.1.15.4  1994/04/28  17:19:22  ohara
 * 	Merged with changes from 1.1.15.3
 * 	[1994/04/28  17:19:10  ohara]
 * 
 * 	start the svc changes for SVR4 code
 * 	[1994/04/25  20:13:09  ohara]
 * 
 * Revision 1.1.15.3  1994/04/13  21:07:45  jd
 * 	Code cleanup munged the protocol sequence strings in sockaddr_to_tower.
 * 	[1994/04/13  20:45:20  jd]
 * 
 * Revision 1.1.15.2  1994/03/16  23:41:47  jd
 * 	Include msgblk to pick up definition of struct evtblk.
 * 	[1994/03/16  21:30:32  jd]
 * 
 * Revision 1.1.15.3  1994/04/13  21:07:45  jd
 * 	Code cleanup munged the protocol sequence strings in sockaddr_to_tower.
 * 	[1994/04/13  20:45:20  jd]
 * 
 * Revision 1.1.15.2  1994/03/16  23:41:47  jd
 * 	Include msgblk to pick up definition of struct evtblk.
 * 	[1994/03/16  21:30:32  jd]
 * 
 * Revision 1.1.15.1  1994/03/12  22:01:27  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:08:18  peckham]
 * 
 * Revision 1.1.7.4  1992/12/30  13:26:57  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:43:22  zeliff]
 * 
 * Revision 1.1.7.3  1992/12/04  19:37:45  marty
 * 	OSC1.1.1 upgrade.
 * 	[1992/12/04  19:23:11  marty]
 * 
 * Revision 1.1.11.2  1992/11/17  02:33:14  weisman
 * 		Corrected cast on bcmp.
 * 	[1992/11/17  02:29:42  weisman]
 * 
 * Revision 1.1.7.2  1992/09/29  19:12:06  devsrc
 * 		Add Dave Hinman's correct #ifdefs around SNI-specific code.
 * 	[1992/09/11  17:09:53  weisman]
 * 
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:11:50  weisman]
 * 
 * Revision 1.1.2.6  1992/06/26  06:39:45  jim
 * 	  Added include msgblk.h. This is not getting included on AIX 3.2
 * 	  for some reason
 * 	[1992/06/23  15:06:39  jim]
 * 
 * Revision 1.1.2.5  1992/06/02  13:55:36  mfox
 * 	gdad was returning bogus towers when trying to convert a string
 * 	binding to a tower if rpc didn't support a protocol sequence. Undo
 * 	previous hack and fix it correctly. Also delete some dead code.
 * 	[1992/06/02  13:53:33  mfox]
 * 
 * Revision 1.1.2.4  1992/05/28  19:22:06  weisman
 * 	Submit Mark Fox' change to not try to use TCP.
 * 	[1992/05/28  19:21:05  weisman]
 * 
 * Revision 1.1.2.3  1992/05/22  14:00:29  mfox
 * 	Format towers in progress record correctly, return error instead of
 * 	aborting silently if BIND not installed and correct typo in string
 * 	binding.
 * 	[1992/05/21  23:41:23  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  21:55:33  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:23:48  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: gda_bind.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
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
 *
 *
 */
/*
 * Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA USA
 * All Rights Reserved
 */

#include <gda.h>
#include <dce/dce_error.h>
#include <dce/uuid.h>
#include <dce/cds_basic_types.h>
#include <dce/assert.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <tower.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <sys/uio.h>


#include <dce/sys_time.h>
#include <gda_main.h>
#include <uc_clerk.h>

#define FIRST_PREFIX 				 1
#define SECOND_PREFIX 				-1
#define EQUAL 			 		 1
#define NOT_EQUAL 		 		 0

#define	BINDQ_EXT_TO_INT_BAD_RESPONSE		-1
#define BINDQ_EXT_TO_INT_DN_EXPAND		-2
#define BINDQ_EXT_TO_INT_FAIL			-3
#define BINDQ_EXT_TO_INT_FIND_STRING_TXT 	-4


#define	WHOLE_LIST_FREE_NL_FAIL			-4

#define	LIST_INSERT_INORDER_NOT_PREFIX		-5

#define	FULLNAME_TO_DOTNAME_FAIL		-6

#define	MAKE_BIND_QUERY_FAIL			-7

#define SPLIT_FULLNAME_BAD_PREFIX		-8

#define	SEND_RECV_VC_SOCKET			-9
#define SEND_RECV_VC_CONNECT			-10
#define SEND_RECV_VC_WRITEV			-11
#define	SEND_RECV_VC_WRITE			-12
#define SEND_RECV_VC_BINDQ			-13
#define SEND_RECV_VC_READ			-14

#define	FIND_STRING_TXT_FAIL			-15

#define	DOTNAME_TO_FULLNAME_FAIL		-16

#define	INIT_CLIENT_FOPEN			-17
#define	INIT_SERVER_FOPEN			-18

#define	T_HEAD					106
#define	T_QUES					107
#define	T_NONE					256
#define	NO_CLASS				256

#define	MAX_HASH_SIZE				 16
#define	MAX_SIMPLE_IN_FULL 			386
#define	BIND_BUF_SIZE 				512

#define	TWICE 					 2

#define	CNS_PTR 				 1

#define	ONE_K 					1024
#define	DATAGRAM 				 0
#define	VIRTUAL_CIRCUIT 			 1
#define BIND_BUF_LEN 				4096

#define	fullname_cmp(X,Y)	fullname_compare(X,Y, 1)
#define	fullname_prefix(X,Y)	fullname_compare(X,Y, 0)
#define cds_COPY_Fullname(from,to) COPY_bytes((from),(to),sizeof(cds_FullName_t))

/*
;; tab-width:
	4
*/


/*
 * Local prototypes..
 */

static void 
makename (char  *,
          char  *);

static void 
fixline (
	 FILE *);

static void 
endline (
	 FILE *);

static int 
init_client (
	     void);



static int 
fullname_compare (cds_FullName_t  *,
                  cds_FullName_t  *,
                  int             );

static int 
list_insert_inorder (
		     rr_el_t         *,
                     list_t          *,
                     cds_FullName_t  *);

static int 
bind_process (
	      question_el_t *);

static void
strip_cds_name (
		cds_FullName_t  *,
                cds_FullName_t  *);

static list_t *
send_recv_vc (
	      char             *,
	      int              , 
	      struct sockaddr  *,
	      int              );

static int 
make_bind_query (
		 cds_FullName_t  *,
                 char            *,
                 int             *);

static int 
fullname_to_dotname (
		     cds_FullName_t  *,
                     unsigned char   *,
                     int             );
static int
init_bind_process (
		   question_el_t *);

static int 
destroy_bind_process (
		      question_el_t *);

static rr_el_t *
find_rr (rr_el_t        *rr,
          question_el_t  *element);

static int 
rr_cmp (rr_el_t  *,
        rr_el_t  *);

static int 
rr_hash (
	 rr_el_t *);

static int 
init_bind (
	   void);

static int 
init_server (
	     void);

static int 
dotname_to_fullname (
		     unsigned char   *,
                     cds_FullName_t  *);

static void  *
recv_bind (
	   int );

static void
init_recv_bind (
		void);

static int 
find_string_txt (
		 unsigned char  **,
                 unsigned char  *,
                 int            *,
                 int            *);

static int 
bindq_ext_to_int (
		  unsigned char  *,
                  int            ,
                  list_t         *);

static void
dotname_backup (
		unsigned char  **,
                unsigned char  *);

static int 
gettoken (
	  FILE *);

static int 
getword (char  *,
         int   ,
         FILE  *);

static int 
whole_list_free_nl (
		    list_t *);

/*
 * uppercase table - index unsigned character
 * to get uppercase equivalent.
*/
unsigned char dns_tt_upper_gda[256] = {
	0,1,2,3,4,5,6,7,8,9,
	10,11,12,13,14,15,16,17,18,19,
	20,21,22,23,24,25,26,27,28,29,
	30,31,32,33,34,35,36,37,38,39,
	40,41,42,43,44,45,46,47,48,49,
	50,51,52,53,54,55,56,57,58,59,
	60,61,62,63,64,65,66,67,68,69,
	70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,
	90,91,92,93,94,95,96,65,66,67,
	68,69,70,71,72,73,74,75,76,77,
	78,79,80,81,82,83,84,85,86,87,
	88,89,90,123,124,125,126,127,128,129,
	130,131,132,133,134,135,136,137,138,139,
	140,141,142,143,144,145,146,147,148,149,
	150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,
	170,171,172,173,174,175,176,177,178,179,
	180,181,182,183,184,185,186,187,188,189,
	190,191,192,193,194,195,196,197,198,199,
	200,201,202,203,204,205,206,207,208,209,
	210,211,212,213,214,215,216,217,218,219,
	220,221,222,223,192,193,194,195,196,197,
	198,199,200,201,202,203,204,205,206,207,
	208,209,210,211,212,213,214,247,216,217,
	218,219,220,221,222,255
};


conduit_el_t	bind_conduit = {
init_bind,
{	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
 	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process },
	{	init_bind_process,	bind_process,	destroy_bind_process } },0
};

extern char		*named_servers;
extern char		*named_client_servers;

/*
 * Map class and type names to number
 */
struct map {
	char	token[8];
	int	val;
};
typedef struct map map;

map m_class[] = {
	"in",		C_IN,
	"chaos",	C_CHAOS,
#ifdef SNI_SVR4
#else
	"hs",		C_HS,
#endif /* SNI_SVR4 */
};
#define NCLASS (sizeof(m_class)/sizeof(map))

#ifdef SNI_SVR4
#define T_TXT 16
#endif /* SNI_SVR4 */

map m_type[] = {
	"a",		T_A,
	"ns",		T_NS,
	"cname",	T_CNAME,
	"soa",		T_SOA,
	"mb",		T_MB,
	"mg",		T_MG,
	"mr",		T_MR,
	"null",		T_NULL,
	"wks",		T_WKS,
	"ptr",		T_PTR,
	"hinfo",	T_HINFO,
	"minfo",	T_MINFO,
	"mx",		T_MX,
	"uinfo",	T_UINFO,
	"uid",		T_UID,
	"gid",		T_GID,
	"txt",		T_TXT,
};
#define NTYPE (sizeof(m_type)/sizeof(map))

char eight_zeros[8] = {
	0,0,0,0,0,0,0,0,
};

#define	NUM_BYTES_TIMESTAMP_LOW		4
#define	NUM_BYTES_TIMESTAMP_HIGH	4
#define	NUM_BYTES_NODE				6

#define	NUM_BYTES_TIMESTAMP			(NUM_BYTES_TIMESTAMP_LOW + NUM_BYTES_TIMESTAMP_HIGH + NUM_BYTES_NODE)


/*
 * Parser token values
 */
#define CURRENT	1
#define DOT	2
#define AT	3
#define DNAME	4
#define INCLUDE	5
#define ORIGIN	6
#define ERROR	7

#define MAXDATA	1024

#define VC_MSG_SIZE_MAX		64 * ONE_K

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
void fullname_print(cds_FullName_t* fname1_p)
{
	int i;
	for (i = 0; i < fname1_p->fn_length; i++)
		fprintf(stderr, "%c", fname1_p->fn_name[i]);
	}

#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */

/*
 * values for bind_client_server below 
 */
#define	BIND_CLIENT	0
#define	BIND_SERVER 1

int	lineno;		/* current line number */

short	ns_port;

list_t				nameser_list;
pthread_mutex_t		recv_bind_mutex;
pthread_cond_t		recv_bind_cond;
int					bind_dg_sock;
uuid_t				null_uuid;
pthread_mutex_t		id_mutex;

int					bind_client_server;


static void
init_recv_bind (void)
{
	char					errbuf[200];
	struct sockaddr_in		gda_bind_addr;

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >init_recv_bind"));

	if (pthread_setspecific(thread_num, (pthread_addr_t)&recv_bind_th_num))
	{
	     dce_svc_printf(CDS_S_PTHREADSETSPECIFIC_MSG, (long)errno);
	     DCE_SVC_DEBUG((
                 cds__svc_handle,
                 cds_svc_gda,
                 svc_c_debug2,
                 " <init_recv_bind: error_exit'ing due to pthread_setspecific failure"));

                error_exit(-1);
	}

	if(pthread_mutex_init(&recv_bind_mutex,pthread_mutexattr_default))
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXINIT_MSG, (long)errno);
	    DCE_SVC_DEBUG((
                 cds__svc_handle,
                 cds_svc_gda,
                 svc_c_debug2,
                 " <init_recv_bind: error_exit'ing due to pthread_mutex_init failure"));

              error_exit(-1);
	}

	if(pthread_cond_init(&recv_bind_cond, pthread_condattr_default))
	{
	    dce_svc_printf(CDS_S_PTHREADCONDINIT_MSG, (long)errno);
	    DCE_SVC_DEBUG((
                 cds__svc_handle,
                 cds_svc_gda,
                 svc_c_debug2,
                 " <init_recv_bind: error_exit'ing due to pthread_cond_init failure"));

              error_exit(-1);
	}

	if ((bind_dg_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
	    dce_svc_printf(CDS_S_GDA_SOCKET_MSG, (long)errno);
	    error_exit(-1);
	}

	gda_bind_addr.sin_family = AF_INET;
	gda_bind_addr.sin_addr.s_addr = INADDR_ANY;
	gda_bind_addr.sin_port = 0;
	memmove( gda_bind_addr.sin_zero,&eight_zeros, 8);

	if(bind(bind_dg_sock, (struct sockaddr *)&gda_bind_addr,
		sizeof(gda_bind_addr)))
	{
	    sprintf(errbuf, "bind(sock %d, %s[%d]): %m",
		    bind_dg_sock, inet_ntoa(gda_bind_addr.sin_addr),
		    ntohs(gda_bind_addr.sin_port));

	    dce_svc_printf(CDS_S_GDA_BIND_EB_MSG, errbuf, (long)errno);

	}

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <init_recv_bind"));
}

static int 
find_string_txt (unsigned char  **src,
                 unsigned char  *dest,
                 int            *partial_left,
                 int            *total_left)
{
	unsigned char	*tmpsrc;
	int				found;

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >find_string_txt"));

	tmpsrc = *src;

	if(*partial_left == 0)
	{
		if(*total_left == 0)
		{
			tmpsrc = *src;
			
			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug2,
			    " <find_string_txt: return(FIND_STRING_TXT_FAIL)"));

			return(FIND_STRING_TXT_FAIL);
		}	

		*partial_left = (int)*tmpsrc++;
		(*total_left)--;
	}

	for(;;)
	{
		for(; (*tmpsrc == '\t' || *tmpsrc == ' ' || *tmpsrc == '\n') &&
			*partial_left > 0 && *total_left > 0;
			tmpsrc++, (*partial_left)--, (*total_left)--);

		if(*partial_left == 0)
		{
			if(*total_left == 0)
			{
			    *src = tmpsrc;

			    DCE_SVC_DEBUG((
				cds__svc_handle,
				cds_svc_gda,
				svc_c_debug2,
				" <find_string_txt: return(FIND_STRING_TXT_FAIL)"));

			    return(FIND_STRING_TXT_FAIL);
			}

			*partial_left = (int)*tmpsrc++;
			(*total_left)--;
		}
		else if(*total_left == 0)
		{
		      DCE_SVC_DEBUG((
			  cds__svc_handle,
			  cds_svc_gda,
			  svc_c_debug5,
			  "find_string_txt: bad txt string: space:"));
		      
		      *src = tmpsrc;

		      DCE_SVC_DEBUG((
			  cds__svc_handle,
			  cds_svc_gda,
			  svc_c_debug2,
			  " <find_string_txt: return(FIND_STRING_TXT_FAIL)"));

			return(FIND_STRING_TXT_FAIL);
		}
		else
		{
			break;
		}
	}

	for(;;)
	{
		for(found = 0;
				*tmpsrc != '\0' && *tmpsrc != '\t' &&
				*tmpsrc !=' ' && *tmpsrc != '\n' &&
				*partial_left > 0 && *total_left > 0;
				tmpsrc++, dest++, (*partial_left)--, (*total_left)--)
		{
			found = 1;
			*dest = *tmpsrc;
		}

		if(*partial_left == 0)
		{
			if(*total_left == 0)
			{
				*src = tmpsrc;
				*dest = '\0';

				if(!found)
				{
				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug2,
					" <find_string_txt: return(FIND_STRING_TXT_FAIL)"));

				    return(FIND_STRING_TXT_FAIL);
				}
				
				else
				{
				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug2,
					" <find_string_txt: return(OK)"));
				    
				    return(OK);
				}
				
			}

			*partial_left = (int)*tmpsrc++;
			(*total_left)--;
		}
		else if(*total_left == 0)
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug5,
			"find_string_txt: bad txt string: string"));
		    
		    *src = tmpsrc;
		    
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
                        " <find_string_txt: return(FIND_STRING_TXT_FAIL)"));

		    return(FIND_STRING_TXT_FAIL);
		}
		else
		{
		    *src = tmpsrc;
		    *dest = '\0';

		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <find_string_txt: return(OK)"));

		    return(OK);
		}
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <find_string_txt"));
}

static int 
bindq_ext_to_int (unsigned char  *answer,
                  int            answer_len,
                  list_t         *query)
{
	unsigned char	*cp = answer;
	HEADER					*hp = (HEADER *)answer;
	unsigned char			*eom = answer + answer_len;
	rr_el_t					*rr;
	rr_el_t					*old_rr;
	rr_el_t					*rr_head;
	char					dname[100];
	int						type;
	int						class;
	u_long					ttl;
	int						dlen;
	int						n;
	char					errbuf[100];
	int						status;
	char					temp_st[100];
	int						total_len;
	int						partial_len;
	error_status_t			uuid_status;
	unsigned char       inq_error_string[dce_c_error_string_len];
	int                 inq_status;
#ifdef SNI_SVR4
	unsigned char       *temp_cp;
	int                 have_seen_txt = 0;
	int                 have_seen_hinfo = 0;
#endif /* SNI_SVR4 */


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
	  " >bindq_ext_to_int"));

	inq_error_string[0]='\0';

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
	fprintf(stderr, "gdad: enter bindq_ext_to_int\n");
	dump_message(answer, answer_len, stderr);
#endif
#endif

	if(!hp->qr)
	{
	    /*
	     * someone sent us a question 
	     */
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug5,
		"bindq_ext_to_int: BAD response: question received"));
	    
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
	       " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_BAD_RESPONSE)"));

	      return(BINDQ_EXT_TO_INT_BAD_RESPONSE);
	}

	if ( (hp->rcode != NOERROR && hp->rcode != NXDOMAIN)
	    || (hp->rcode == NXDOMAIN && !hp->aa) )
	{
	    /*
	     * bad response 
	     */
	    sprintf(errbuf, "BAD response: opcode %d errcode %d",
		    hp->opcode, hp->rcode);

	    DCE_SVC_DEBUG((
		cds__svc_handle,
                cds_svc_gda,
                svc_c_debug2,
		" <bindq_ext_to_int:%s",
		errbuf));

	    return(BINDQ_EXT_TO_INT_BAD_RESPONSE);
	}

	l_init_nl(query);

	for(type = T_HEAD, old_rr = (rr_el_t *)NULL; cp != eom; )
	{
	    if(type != T_QUES && type != T_HEAD)
	    {
		if ((n = dn_expand(answer, eom, cp, (unsigned char *)dname,
				   sizeof(dname))) < 0)
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug5,
	                "bindq_ext_to_int: Error %d from dn_expand failed rr head",
			n));

		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_DN_EXPAND)"));

		    return(BINDQ_EXT_TO_INT_DN_EXPAND);
		}
		cp += n;

		GETSHORT(type, cp);
		GETSHORT(class, cp);
		GETLONG(ttl, cp);
		GETSHORT(dlen, cp);
	    }

#ifdef SNI_SVR4
	    if(type == T_QUES || type == T_HEAD || type == T_HINFO ||
	       type == T_A || type == T_TXT || type == T_NS)
#else
	    if(type == T_QUES || type == T_HEAD ||
	       type == T_A || type == T_TXT || type == T_NS)
#endif /* SNI_SVR4 */
	    {
		if((rr = (rr_el_t *)malloc(sizeof(rr_el_t))) == NULL)
		{
		    dce_svc_printf(CDS_S_ALLOC_MSG);
		    error_exit(-1);
		}	
		ALLOC(rr_el_t, 1)
		
	    }

#ifdef SNI_SVR4
	    if(type == T_A || type == T_TXT || type == T_NS || type == T_HINFO)
#else
	    if(type == T_A || type == T_TXT || type == T_NS)
#endif /* SNI_SVR4 */
	    {
		dotname_to_fullname((unsigned char *)dname, 
				    &rr->dname_rr);
		
		rr->class_rr = class;
	    }

	    switch(type)
	    {
	        case T_HEAD:
			rr->tag_rr = e_header;

			rr->class_rr = NO_CLASS;

			rr->rr.head = *((HEADER *)answer);
			rr_head = rr;
			cp += sizeof(HEADER);

			rr->rr.head.id = ntohs(rr->rr.head.id);
			rr->rr.head.qdcount= ntohs(rr->rr.head.qdcount);
			rr->rr.head.ancount= ntohs(rr->rr.head.ancount);
			rr->rr.head.nscount= ntohs(rr->rr.head.nscount);
			rr->rr.head.arcount= ntohs(rr->rr.head.arcount);

			break;
		 case T_QUES:
			rr->tag_rr = e_question;

			if ((n = dn_expand(answer, eom, cp, 
					   (unsigned char *)dname,
					   sizeof(dname))) < 0)
			{
			    DCE_SVC_DEBUG((
				cds__svc_handle,
				cds_svc_gda,
				svc_c_debug5,
			        "bindq_ext_to_int: dn_expand failed question section"));

			    DCE_SVC_DEBUG((
				cds__svc_handle,
				cds_svc_gda,
				svc_c_debug2,
				" <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_DN_EXPAND)"));

			      return(BINDQ_EXT_TO_INT_DN_EXPAND);
			}
			cp += n;

			dotname_to_fullname((unsigned char *)dname,
					    &rr->dname_rr);

			GETSHORT(rr->rr.ques.type, cp);
			GETSHORT(rr->class_rr, cp);

			if(rr_head->rr.head.ancount == 0 && rr_head->rr.head.aa &&
				rr_head->rr.head.nscount == 0)
			{
				rr->rr.ques.does_not_exist = 1;
			}
			else
			{
				rr->rr.ques.does_not_exist = 0;
			}

			break;
		case T_A:
			rr->tag_rr = e_addr;
			rr->rr.ad.mach_addr.sin_family = AF_INET;
			memmove( (char *)&rr->rr.ad.mach_addr.sin_addr,cp,
			      sizeof(struct sockaddr));
			cp += dlen;
			rr->rr.ad.mach_addr.sin_port = ns_port;
			memmove( rr->rr.ad.mach_addr.sin_zero,&eight_zeros, 8);
			break;
		case T_NS:
			rr->tag_rr = e_nameserver;
			if ((n = dn_expand(answer, eom, cp, 
					   (unsigned char *)dname,
					   sizeof(dname))) < 0)
			{
			    DCE_SVC_DEBUG((
				cds__svc_handle,
				cds_svc_gda,
				svc_c_debug5,
				"bindq_ext_to_int: Error %d from dn_expand which failed ns rr.",
				n));

			    DCE_SVC_DEBUG((
				cds__svc_handle,
				cds_svc_gda,
				svc_c_debug2,
				" <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_DN_EXPAND)"));

			    return(BINDQ_EXT_TO_INT_DN_EXPAND);
			}
			cp += n;

			dotname_to_fullname((unsigned char *)dname,	
					    &rr->rr.ns.mach_name);

			break;
#ifndef SNI_SVR4
		case T_TXT:

			partial_len = 0;
			total_len = dlen;

			if(find_string_txt(&cp, (unsigned char *)temp_st, 
					   &partial_len, &total_len) != OK)
			{
			    DCE_SVC_DEBUG((
				cds__svc_handle,
				cds_svc_gda,
				svc_c_debug5,
				"bindq_ext_to_int: find_string_txt failed"));
			    
			    DCE_SVC_DEBUG((
				cds__svc_handle,
				cds_svc_gda,
				svc_c_debug2,
				" <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));

			      return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
			}

			if((type = atoi(temp_st)) == CNS_PTR)
			{
				rr->tag_rr = e_cds_cell_info_txt;

				if(find_string_txt(&cp, 
						   (unsigned char *)temp_st, 
						   &partial_len, &total_len) 
				   != OK)
				{
				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug5,
					"bindq_ext_to_int: find_string_txt failed"));

				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug2,
					" <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));

				    return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}

				uuid_from_string((unsigned char *)temp_st, 
						 &rr->rr.ci.nsp_uuid,
						 &uuid_status);
				if (uuid_status != uuid_s_ok)
				{

                                      dce_svc_printf(CDS_S_GDA_BADNSUUID_MSG,
						     (long)uuid_status);
				      DCE_SVC_DEBUG((
					     cds__svc_handle,
					     cds_svc_gda,
					     svc_c_debug2,
					     " <bindq_ext_to_int: error in namespace uuid string: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));

				      return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}

				if(find_string_txt(&cp,
						   (unsigned char *)temp_st, 
						   &partial_len, &total_len) != OK)
				{
				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug5,
					"bindq_ext_to_int: find_string_txt failed: replica type"));

				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug2,
					" <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));				      

				    return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}

				temp_st[0] = toupper(temp_st[0]);
				if(temp_st[0] == 'M')
					rr->rr.ci.replica_type = RT_master;
				else if(temp_st[0] == 'R')
					rr->rr.ci.replica_type = RT_readOnly;
				else
				{

                                      DCE_SVC_DEBUG((
					  cds__svc_handle,
					  cds_svc_gda,
					  svc_c_debug5,
					  "bindq_ext_to_int: Replica neither a Master nor Readonly"));

				      DCE_SVC_DEBUG((
					  cds__svc_handle,
					  cds_svc_gda,
					  svc_c_debug2,
					  " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FAIL)"));

				      return(BINDQ_EXT_TO_INT_FAIL);
				}

				if(find_string_txt(&cp, 
						   (unsigned char *)temp_st, 
						   &partial_len, &total_len) 
				   != OK)
				{
				    DCE_SVC_DEBUG((
					  cds__svc_handle,
					  cds_svc_gda,
					  svc_c_debug5,
					  "bindq_ext_to_int: find_string_txt failed"));

				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug2,
					" <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));	

				      return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}

				dotname_to_fullname((unsigned char *)temp_st, 
						    &rr->rr.ci.ch_name);

				if(find_string_txt(&cp, 
						   (unsigned char *)temp_st, 
						   &partial_len, &total_len) 
				   != OK)
				{
				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug5,
					"bindq_ext_to_int: find_string_txt failed"));
				    
				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug2,
					" <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));	

				      return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}

				uuid_from_string((unsigned char *)temp_st, &rr->rr.ci.ch_uuid,
					&uuid_status);

				if (uuid_status != uuid_s_ok)
				{
				    
				    dce_svc_printf(CDS_S_GDA_BADCHUUID_MSG,
					     (long)uuid_status);

				    DCE_SVC_DEBUG((
					cds__svc_handle,
					cds_svc_gda,
					svc_c_debug2,
					" <bindq_ext_to_int: error in clearinghouse uuid string: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));	

				    return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}

				if(find_string_txt(&cp,
						   (unsigned char *)temp_st, 
						   &partial_len, &total_len) 
				   != OK)
				{
                                      DCE_SVC_DEBUG((
					  cds__svc_handle,
					  cds_svc_gda,
					  svc_c_debug5,
					  "bindq_ext_to_int: find_string_txt failed: host name"));
				    
				      DCE_SVC_DEBUG((
                                          cds__svc_handle,
					  cds_svc_gda,
					  svc_c_debug2,
					  " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));	

				      return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);

				}

				dotname_to_fullname((unsigned char *)temp_st,
						    &rr->rr.ci.mach_name);

				break;
			}
			else
			{
				type = T_NONE;
				free((void *)rr);
				FREE(rr_el_t, 1)
			}
#else /* SNI_SVR4 */
		case T_HINFO:
		case T_TXT:

 			if (type == T_HINFO)
			{
 				unsigned char* os_ptr;
 			
 				have_seen_hinfo = 1;
 
 				if (have_seen_txt)
 				{
                                DCE_SVC_DEBUG((
                                          cds__svc_handle,
                                          cds_svc_gda,
                                          svc_c_debug5,
                                          "bindq_ext_to_int: Both TXT and HINFO received; using TXT"));

                                 DCE_SVC_DEBUG((
                                          cds__svc_handle,
                                          cds_svc_gda,
                                          svc_c_debug2,
                                          " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FAIL)"));

 				cp += dlen;
 				break;
 				}
 
 				/* Skip over hardware string to get to OS string */
 				os_ptr = cp + *cp + 1;
 				if ((os_ptr >= cp + dlen) ||
 				    (strncasecmp(os_ptr + 1, "DCECDS", 
 						 *os_ptr) != 0))
 				{
                                      DCE_SVC_DEBUG((
                                          cds__svc_handle,
                                          cds_svc_gda,
                                          svc_c_debug5,
                                          "bindq_ext_to_int: HINFO in wrong format received and ignored"));

                                      DCE_SVC_DEBUG((
                                          cds__svc_handle,
                                          cds_svc_gda,
                                          svc_c_debug2,
                                          " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FAIL)"));

 					cp += dlen;
 					break;
 				}
 			}
 			else
 			{	
 				have_seen_txt = 1;
 				if (have_seen_hinfo)
 				{
 				    sprintf(errbuf, "Both TXT and HINFO received; using TXT");
                                    DCE_SVC_DEBUG((
                                        cds__svc_handle,
                                        cds_svc_gda,
                                        svc_c_debug5,
                                        "bindq_ext_to_int: Both TXT and HINFO received; using TXT"));

                                    DCE_SVC_DEBUG((
                                        cds__svc_handle,
                                        cds_svc_gda,
                                        svc_c_debug2,
                                        " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FAIL)"));
 				}
 			}
 			
  			partial_len = 0;
 			total_len = *cp + 1; /* total_len is length of this char string,
 					      * which can be less than the length of the RR
 					      * if this is an HINFO record */
 			temp_cp = cp;
 			
 			if(find_string_txt(&temp_cp, temp_st, &partial_len, &total_len) != OK)
  			{
                            DCE_SVC_DEBUG((
                                cds__svc_handle,
                                cds_svc_gda,
                                svc_c_debug5,
                                "bindq_ext_to_int: find_string_txt failed"));

                            DCE_SVC_DEBUG((
                                cds__svc_handle,
                                cds_svc_gda,
                                svc_c_debug2,
                                " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));

				return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
			}

			if((type = atoi(temp_st)) == CNS_PTR)
			{
				rr->tag_rr = e_cds_cell_info_txt;

				if(find_string_txt(&temp_cp, temp_st, &partial_len, &total_len) != OK)
				{
                                    DCE_SVC_DEBUG((
                                        cds__svc_handle,
                                        cds_svc_gda,
                                        svc_c_debug5,
                                        "bindq_ext_to_int: find_string_txt failed"));

                                    DCE_SVC_DEBUG((
                                        cds__svc_handle,
                                        cds_svc_gda,
                                        svc_c_debug2,
                                        " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));

                                    return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}

				uuid_from_string((unsigned char *)temp_st, &rr->rr.ci.nsp_uuid,
					&uuid_status);
				if (uuid_status != uuid_s_ok)
				{
				  dce_svc_printf(CDS_S_GDA_BADNSUUID_MSG,
						 (long)uuid_status);
                                      DCE_SVC_DEBUG((
                                             cds__svc_handle,
                                             cds_svc_gda,
                                             svc_c_debug2,
                                             " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));

                                      return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
				fprintf(stderr, "gdad: NS uuid after conversion: ");
				uuid_print(&rr->rr.ci.nsp_uuid);
				fprintf(stderr, "\n");
#endif
#endif

				if(find_string_txt(&temp_cp, temp_st, &partial_len, &total_len) != OK)
				{
                                    DCE_SVC_DEBUG((
                                        cds__svc_handle,
                                        cds_svc_gda,
                                        svc_c_debug5,
                                        "bindq_ext_to_int: find_string_txt failed: replica type"));

                                    DCE_SVC_DEBUG((
                                        cds__svc_handle,
                                        cds_svc_gda,
                                        svc_c_debug2,
                                        " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));


                                    return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}

				temp_st[0] = toupper(temp_st[0]);
				if(temp_st[0] == 'M')
					rr->rr.ci.replica_type = RT_master;
				else if(temp_st[0] == 'R')
					rr->rr.ci.replica_type = RT_readOnly;
				else
				{
                                      DCE_SVC_DEBUG((
                                          cds__svc_handle,
                                          cds_svc_gda,
                                          svc_c_debug5,
                                          "bindq_ext_to_int: Replica neither a Master nor Readonly"));

                                      DCE_SVC_DEBUG((
                                          cds__svc_handle,
                                          cds_svc_gda,
                                          svc_c_debug2,
                                          " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FAIL)"));

                                      return(BINDQ_EXT_TO_INT_FAIL);
				}

				if(find_string_txt(&temp_cp, temp_st, &partial_len, &total_len) != OK)
				{
                                    DCE_SVC_DEBUG((
                                          cds__svc_handle,
                                          cds_svc_gda,
                                          svc_c_debug5,
                                          "bindq_ext_to_int: find_string_txt failed"));

                                    DCE_SVC_DEBUG((
                                        cds__svc_handle,
                                        cds_svc_gda,
                                        svc_c_debug2,
                                        " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));


                                      return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}

				dotname_to_fullname(temp_st, &rr->rr.ci.ch_name);

				if(find_string_txt(&temp_cp, temp_st, &partial_len, &total_len) != OK)
				{
                                   DCE_SVC_DEBUG((
                                        cds__svc_handle,
                                        cds_svc_gda,
                                        svc_c_debug5,
                                        "bindq_ext_to_int: find_string_txt failed"));

                                    DCE_SVC_DEBUG((
                                        cds__svc_handle,
                                        cds_svc_gda,
                                        svc_c_debug2,
                                        " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));


                                      return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}

				uuid_from_string((unsigned char *)temp_st, &rr->rr.ci.ch_uuid,
					&uuid_status);
				if (uuid_status != uuid_s_ok) 
				{
				    dce_svc_printf(CDS_S_GDA_BADCHUUID_MSG,
						   (long)uuid_status);

                                    DCE_SVC_DEBUG((
                                        cds__svc_handle,
                                        cds_svc_gda,
                                        svc_c_debug2,
                                        " <bindq_ext_to_int: bad clearinhouse uuid: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));


                                    return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
				fprintf(stderr, "gdad: CH uuid after conversion: ");
				uuid_print(&rr->rr.ci.ch_uuid);
				fprintf(stderr, "\n");
#endif
#endif
				if(find_string_txt(&temp_cp, temp_st, &partial_len, &total_len) != OK)
				{
                                      DCE_SVC_DEBUG((
                                          cds__svc_handle,
                                          cds_svc_gda,
                                          svc_c_debug5,
                                          "bindq_ext_to_int: find_string_txt failed: host name"));

                                      DCE_SVC_DEBUG((
                                          cds__svc_handle,
                                          cds_svc_gda,
                                          svc_c_debug2,
                                          " <bindq_ext_to_int: return(BINDQ_EXT_TO_INT_FIND_STRING_TXT)"));


                                      return(BINDQ_EXT_TO_INT_FIND_STRING_TXT);
				}

				dotname_to_fullname(temp_st, &rr->rr.ci.mach_name);
				cp += dlen;
				break;
			}
			else
			{
				type = T_NONE;
				free((void *)rr);
				FREE(rr_el_t, 1)
			}
#endif /* SNI_SVR4 */
#ifdef T_AFSDB
		case T_AFSDB:
#endif
			/* this information I will not need because the
				address of the server is in the cell_info
				TXT record. */
		case T_MD:
		case T_MF:
		case T_CNAME:
		case T_SOA:
		case T_MB:
		case T_MG:
		case T_MR:
		case T_NULL:
		case T_WKS:
		case T_PTR:
#ifndef SNI_SVR4
		case T_HINFO:
#endif /* SNI_SVR4 */
		case T_MINFO:
		case T_MX:
		case T_UINFO:
		case T_UID:
		case T_GID:
		case T_UNSPEC:
#ifdef T_UNSPECA
		case T_UNSPECA:
#endif
			sprintf(errbuf, "Unnecessary rr: type %d", type);

			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug5,
			    "bindq_ext_to_int : %s",
			    errbuf));

			cp += dlen;
			break;

		default:
			sprintf(errbuf, "Unknown rr: type %d", type);

			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug1,
			    "bindq_ext_to_int : %s",
			    errbuf));

			cp += dlen;
			break;
		}

#ifdef SNI_SVR4
		if(type == T_QUES || type == T_HEAD || type == T_HINFO ||
		   type == T_A || type == T_TXT || type == T_NS)
#else
		if(type == T_QUES || type == T_HEAD ||
		   type == T_A || type == T_TXT || type == T_NS)
#endif /* SNI_SVR4 */
		{
			l_insert_nl((char **)&rr, query,
				(element_t *)old_rr);
			old_rr = rr;

		}

		if(type == T_QUES)
			type = T_NONE;
		else if(type == T_HEAD)
			type = T_QUES;

	}

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <bindq_ext_to_int: return(OK)"));

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
fprintf(stderr, "gdad: exit bindq_ext_to_int\n");
#endif
#endif
      return(OK);
}

static int 
whole_list_free_nl (list_t *list)
{
	element_t	*el;
	int			status;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug7,
          " >whole_list_free_nl"));

	for(el = (element_t *)l_first_nl(list); el > (element_t *)NULL;
		el = (element_t *)l_first_nl(list))
	{
		l_delete_nl(&el, list);

		free((void *)el);
		FREE(rr_el_t, 2)
	}

	if((status = l_free_nl(list)) != OK)
	{
	    DCE_SVC_DEBUG((
	        cds__svc_handle,
		cds_svc_gda,
		svc_c_debug7,
		" <whole_list_free_nl: return(WHOLE_LIST_FREE_NL_FAIL)"));

	    return(WHOLE_LIST_FREE_NL_FAIL);
	}

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug7,
          " <whole_list_free_nl: return(OK)"));

      return(OK);
}


static void 
*recv_bind (int n)
{
#define	QUES	LIST_REF(question_el_t, ques)
#define	RR_QUES	LIST_REF(rr_el_t, rr_ques)
#define	RR_HEAD	LIST_REF(rr_el_t, rr_head)

	char			answer[BIND_BUF_LEN];
	int				answer_len;
	struct sockaddr	from;
	int				from_len;
	list_t			*recv_answ;
	int				status;
	rr_el_t			*rr_head;
	rr_el_t			*rr_ques;
	int				found;
	question_el_t	*ques;
	int				looper;

/*
bind send_recv thread
	set up comm link
	forever()
		receive answer
		convert answer to internal format
		insert answer in sub-list
		signal waiting thread
*/
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >recv_bind"));

      init_recv_bind();

	for(answer_len = BIND_BUF_LEN;
		;
		answer_len = BIND_BUF_LEN)
	{

              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "recv_bind: before recvfrom"));
#ifdef SNI_SVR4
		from_len = sizeof(from);
#endif /* SNI_SVR4 */
		if((answer_len = recvfrom(bind_dg_sock, answer, answer_len,
			0, &from, &from_len)) == -1)
		{
			if (errno == EINTR)
			{
                              DCE_SVC_DEBUG((
                                  cds__svc_handle,
                                  cds_svc_gda,
                                  svc_c_debug5,
                                  "recv_bind: recvfrom interrupted"));

			      continue;
			}

			dce_svc_printf(CDS_S_GDA_RECVFROM_EB_MSG, (long)errno);
		}
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
		fprintf(stderr, "gdad: receiving bind response from ");
		dump_address(&from, stderr);
		fprintf(stderr, "\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
		if((recv_answ = (list_t *)malloc(sizeof(list_t))) == NULL)
		{
		    dce_svc_printf(CDS_S_ALLOC_A_MSG);
		}
		ALLOC(list_t, 1)

              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
		  "recv_bind: before bindq_ext_to_int"));

	      if((status = bindq_ext_to_int((unsigned char *)answer, 
					    answer_len, recv_answ))
		 != OK)
	      {
		  DCE_SVC_DEBUG((
		      cds__svc_handle,
		      cds_svc_gda,
		      svc_c_debug1,
		      "recv_bind: bindq_ext_to_int returned error: %d",
		      status));

		  /*
		   * who should dealloc half a query list ? 
		   */
		  continue;
	      }	

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
	      fprintf(stderr, "recv_bind(): post bindq_ext_to_int:dumping recv_answ:\n");
	      dump_list(recv_answ, stderr);
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
	      if((rr_head = (rr_el_t *)l_first_nl(recv_answ)) == NULL ||
		 RR_HEAD->tag_rr != e_header)
	      {
                      DCE_SVC_DEBUG((
                          cds__svc_handle,
                          cds_svc_gda,
                          svc_c_debug1,
                          "recv_bind: No header in recv_answ"));

			if(whole_list_free_nl(recv_answ) != OK)
			{
                              DCE_SVC_DEBUG((
                                  cds__svc_handle,
                                  cds_svc_gda,
                                  svc_c_debug1,
                                  "recv_bind: whole_list_free_nl failed"));
			}
			free((void *)recv_answ);
			FREE(list_t, 1);
			continue;
		}

		if((rr_ques = (rr_el_t *)l_next_nl((element_t *)rr_head)) == NULL ||
			RR_QUES->tag_rr != e_question)
		{
                      DCE_SVC_DEBUG((
                          cds__svc_handle,
                          cds_svc_gda,
                          svc_c_debug1,
                          "recv_bind: No question in recv_answ"));

			if(whole_list_free_nl(recv_answ) != OK)
			{
                              DCE_SVC_DEBUG((
                                  cds__svc_handle,
                                  cds_svc_gda,
                                  svc_c_debug1,
                                  "recv_bind: whole_list_free_nl failed"));
			}
			free((void *)recv_answ);
			FREE(list_t, 2);
			continue;
		}

              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "recv_bind: before loop"));

		/*
		 * find query this answer matches with 
		 */
		for(found = 0, ques = (question_el_t *)l_first(&question_list);
			ques > (question_el_t *)NULL;
			ques = (question_el_t *)l_next((element_t *)ques))
		{
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
			fprintf(stderr, "gdad: recv_bind(); comparing question \"");
			fullname_print(&QUES->unresolved_in_q);
			fprintf(stderr, "\" with answer \"");
			fullname_print(&RR_QUES->dname_rr);
			fprintf(stderr, "\"\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
			if((status = fullname_prefix(&RR_QUES->dname_rr,
				&QUES->unresolved_in_q))  >= FIRST_PREFIX)
			{
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
			      fprintf(stderr, "recv_bind(): in matchloop: status = %d\n", status);
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
                              DCE_SVC_DEBUG((
                                  cds__svc_handle,
                                  cds_svc_gda,
                                  svc_c_debug5,
                                  "recv_bind: before inner loop"));

				for(looper = 0;
					looper < QUES->cd_q.bind.num_who;
					looper++)
				{
					if(QUES->cd_q.bind.who[looper].query_num ==
						RR_HEAD->rr.head.id)
					{
						found = 1;
						break;
					}
				}

                              DCE_SVC_DEBUG((
                                  cds__svc_handle,
                                  cds_svc_gda,
                                  svc_c_debug5,
                                  "recv_bind: after inner loop"));

			      if(found)
			      {
				  break;
			      }
			      
			}
		}

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
	      fprintf(stderr, "recv_bind(): after matchloop: found = %d\n", found);
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
		  "recv_bind: after loop"));

		if(ques == (question_el_t *)L_NEXT_FAIL ||
			ques == (question_el_t *)L_FIRST_FAIL)
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
		        cds_svc_gda,
			svc_c_debug1,
			"recv_bind: list error (%d) : %s",
			(int)ques,   
			strerror(errno)));

		    if(whole_list_free_nl(recv_answ) != OK)
		    {
			DCE_SVC_DEBUG((
                            cds__svc_handle,
                            cds_svc_gda,
                            svc_c_debug1,
                            "recv_bind: whole_list_free_nl: returned error"));
		    }
		    free((void *)recv_answ);
		    FREE(list_t, 3);
		    continue;
		}

		if(!found)
		{
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
		      fprintf(stderr, "recv_bind(): Error: Answer received to non-existent question\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
                      DCE_SVC_DEBUG((
                          cds__svc_handle,
                          cds_svc_gda,
                          svc_c_debug1,
                          "recv_bind: Answer received to non-existent question"));

			if(whole_list_free_nl(recv_answ) != OK)
			{
                              DCE_SVC_DEBUG((
                                  cds__svc_handle,
                                  cds_svc_gda,
                                  svc_c_debug1,
                                  "recv_bind: whole_list_free_nl failed"));
			}
			free((void *)recv_answ);
			FREE(list_t, 4);
			continue;
		}

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
	      fprintf(stderr, "recv_bind(): Before mutex lock\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "recv_bind: before mutex lock"));

		if(pthread_mutex_lock(&QUES->cd_q.bind.mutex))
		{
		    /* 
		     * this may occur if the mutex is destroyed while
		     * we are waiting for it 
		     */

		    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_MSG, (long)errno);

		    if(whole_list_free_nl(recv_answ) != OK)
		    {
			DCE_SVC_DEBUG((
                            cds__svc_handle,
                            cds_svc_gda,
                            svc_c_debug1,
                            "recv_bind: whole_list_free_nl failed"));
		    }
		    free((void *)recv_answ);
		    FREE(list_t, 5);

		    l_release_el((element_t *)ques);
		    continue;
		  }

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
	      fprintf(stderr, "recv_bind(): After mutex lock\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "recv_bind: after mutex lock"));

		QUES->cd_q.bind.answer = recv_answ;

		QUES->cd_q.bind.ad_ns_answ = QUES->cd_q.bind.who[looper].ad_ns;

		if(pthread_cond_signal(&QUES->cd_q.bind.cond))
		{
		    dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)errno);
		}

		if(pthread_mutex_unlock(&QUES->cd_q.bind.mutex))
		{
		    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG,(long)errno);
		}

		l_release_el((element_t *)ques);
      }


#undef	QUES
#undef	RR_QUES
#undef	RR_HEAD

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " <recv_bind"));

}

static void
dotname_backup (unsigned char  **dotname,
                unsigned char  *stop)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " >dotname_backup"));

    if(*dotname == stop)
    {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
	    " <dotname_backup: returning early because dotname == stop"));

	return;
    }
    

    do {
	(*dotname)--;
    } while(**dotname != '.' && *dotname != stop);

    if(**dotname == '.')
	(*dotname)++;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " <dotname_backup"));
}

static int 
fullname_compare (cds_FullName_t  *fname1_p,
                  cds_FullName_t  *fname2_p,
                  int             cmp)
{
	char	*fn_name1 = ((char *)fname1_p->fn_name + fname1_p->fn_length);
	char	*fn_name2 =  ((char *)fname2_p->fn_name + fname2_p->fn_length);
	char	*stop1 = (char *)fname1_p->fn_name + 1;
	char	*stop2 =  (char *)fname2_p->fn_name + 1;
	register char	*fn_name1_old;
	register char	*fn_name2_old;
	register int	looper;


	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >fullname_compare"));

	/*
	 * if either is null, ignore the memcmp 
	 */
	for (fn_name1_old = fn_name1, fn_name2_old = fn_name2, looper = 1;
		1;
		fn_name1_old = fn_name1, fn_name2_old = fn_name2, fn_name1 -=1,
		fn_name2 -=1)
	{
		dotname_backup((unsigned char **)&fn_name1, 
			       (unsigned char *)stop1);

		dotname_backup((unsigned char **)&fn_name2, 
			       (unsigned char *)stop2);

		if((fn_name1_old - fn_name1) == 0)
		{
			if ((fn_name2_old - fn_name2) != 0)
			    if(cmp)
			    {
				DCE_SVC_DEBUG((
				    cds__svc_handle,
				    cds_svc_gda,
				    svc_c_debug2,
				    " <fullname_compare: return(NOT_EQUAL)"));

				return(NOT_EQUAL);
			    }
			    else
			    {
				DCE_SVC_DEBUG((
				    cds__svc_handle,
				    cds_svc_gda,
				    svc_c_debug2,
				    " <fullname_compare: return(looper)"));

				return(looper);
			    }
			
			else
			    if (cmp)
			    {
				DCE_SVC_DEBUG((
				    cds__svc_handle,
				    cds_svc_gda,
				    svc_c_debug2,
				    " <fullname_compare: return(EQUAL)"));

				return(EQUAL);
			    }
			    else
			    {
				DCE_SVC_DEBUG((
				    cds__svc_handle,
				    cds_svc_gda,
				    svc_c_debug2,
				    " <fullname_compare: return(looper)"));

				return(looper);
			    }
		}
		else if ((fn_name2_old - fn_name2) == 0)
		{
		    if(cmp)
		    {
			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug2,
			    " <fullname_compare: return(NOT_EQUAL)"));

			return(NOT_EQUAL);
		    }
		    else
		    {
			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug2,
			    " <fullname_compare: return(-looper)"));

			return(-looper);
		    }
		    
		}

		if((fn_name1_old - fn_name1) != (fn_name2_old - fn_name2))
		{
		    if(cmp)
		    {
			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug2,
			    " <fullname_compare: return(NOT_EQUAL)"));

			return(NOT_EQUAL);
		    }
		    else
		    {
			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug2,
			    " <fullname_compare: return(looper)"));

			return(looper);
		    }
		    
		}

		/* BUGFIX: too many chars compared, reduce by one */
		if(strncasecmp(fn_name1, fn_name2, fn_name2_old - fn_name2 - 1))
		{
		    DCE_SVC_DEBUG((
		        cds__svc_handle,
		        cds_svc_gda,
		        svc_c_debug2,
		        " <fullname_compare: return(NOT_EQUAL)"));

		    return(NOT_EQUAL);
		}
		

		looper++;

		if(fn_name1 == (char *)stop1)
		{
		    if(fn_name2 != (char *)stop2)
		    {
			if(cmp)
			{	
			    DCE_SVC_DEBUG((
				cds__svc_handle,
				cds_svc_gda,
				svc_c_debug2,
				" <fullname_compare: return(NOT_EQUAL)"));

			    return(NOT_EQUAL);
			}
			else
			{
			    DCE_SVC_DEBUG((
			        cds__svc_handle,
			        cds_svc_gda,
			        svc_c_debug2,
			        " <fullname_compare: return(looper)"));

			    return(looper);
			}
		    }
		    else
		    {
			if(cmp)
			{
			    DCE_SVC_DEBUG((
				cds__svc_handle,
				cds_svc_gda,
				svc_c_debug2,
				" <fullname_compare: return(EQUAL)"));

			    return(EQUAL);
			}
			else
			{
			    DCE_SVC_DEBUG((
			        cds__svc_handle,
			        cds_svc_gda,
			        svc_c_debug2,
			        " <fullname_compare: return(looper)"));

			    return(looper);
			}
			
		    }
		    
		}
		else if(fn_name2 == (char *)stop2)
		{
		    if(cmp)
		    { 
			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug2,
			    " <fullname_compare: return(NOT_EQUAL)"));

			return(NOT_EQUAL);
		    }
		    else
		    {
			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug2,
			    " <fullname_compare: return(-looper)"));

			return(-looper);
		    }
		    
		}

	}
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <fullname_compare"));
}

static int 
dotname_to_fullname (unsigned char   *dotname,
                     cds_FullName_t  *fullname_st)
{
	register char	*dest;
	register unsigned char	*src;
	register int	len;
	int				src_len;


	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >dotname_to_fullname"));

	src_len = strlen((char *)dotname);

	if(src_len > FULLNAME_MAXLEN)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <dotname_to_fullname: return(DOTNAME_TO_FULLNAME_FAIL)"));

	    return(DOTNAME_TO_FULLNAME_FAIL);
	}
	

	if(src_len == 0)
	{
		fullname_st->fn_length = 0;

		DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_gda,
		   svc_c_debug2,
		   " <dotname_to_fullname: return(OK)"));

		return(OK);
	}

	if(*dotname == '/')
	{
		for(len = src_len, dest = (char *)fullname_st->fn_name,
			src = dotname; len != 0; src++, dest++, len--)
		{
			*dest = *((char *)src);
		}

		fullname_st->fn_length = src_len;
	}
	else
	{
		*fullname_st->fn_name = '/';
		for(len = src_len, dest = ((char *)fullname_st->fn_name + 1),
			src = dotname; len != 0; src++, dest++, len--)
		{
			*dest = *((char *)src);
		}

		fullname_st->fn_length = src_len + 1;
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <dotname_to_fullname: return(OK)"));

	return(OK);
}


/*
 * Get next word, skipping blanks & comments.
 */
static int 
getword (char  *buf,
         int   size,
         FILE  *fp)
{
	register char *cp;
	register int c;


	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >getword"));

	for (cp = buf; (c = getc(fp)) != EOF; ) {
		if (c == ';') {
			while ((c = getc(fp)) != EOF && c != '\n')
				;
			c = '\n';
		}
		if (c == '\n') {
			if (cp != buf)
				(void) ungetc(c, fp);
			else
				lineno++;
			break;
		}
		if (isspace(c)) {
			while (isspace(c = getc(fp)) && c != '\n')
				;
			(void) ungetc(c, fp);
			if (cp != buf)		/* Trailing whitespace */
				break;
			continue;		/* Leading whitespace */
		}
		if (c == '"') {
			while ((c = getc(fp)) != EOF && c != '"' && c != '\n') {
				if (c == '\\') {
					if ((c = getc(fp)) == EOF)
						c = '\\';
					if (c == '\n')
						lineno++;
				}
				if (cp >= buf+size-1)
					break;
				*cp++ = c;
			}
			if (c == '\n') {
				lineno++;
				break;
			}
			continue;
		}
		if (c == '\\') {
			if ((c = getc(fp)) == EOF)
				c = '\\';
			if (c == '\n')
				lineno++;
		}
		if (cp >= buf+size-1)
			break;
		*cp++ = c;
	}
	*cp = '\0';

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <getword: return cp != buf which is : %d",
	    (cp != buf)));

	return (cp != buf);
}


static int 
gettoken (FILE *fp)
{
	register int c;
	char op[32];


	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug7,
            " >gettoken"));

	for (;;) 
	{
	    
		c = (int)getc(fp);
	top:
		switch (c) {
		case EOF:

		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug7,
			" <gettoken (EOF)"));

		    return (EOF);

		case '$':
			if (getword(op, sizeof(op), fp)) {
			    if (!strcasecmp("include", op))
			    {	
				DCE_SVC_DEBUG((
				    cds__svc_handle,
				    cds_svc_gda,
				    svc_c_debug7,
				    " <gettoken (INCLUDE)"));

				return (INCLUDE);
			    }
			    
			    if (!strcasecmp("origin", op))
			    {
				DCE_SVC_DEBUG((
				    cds__svc_handle,
				    cds_svc_gda,
				    svc_c_debug7,
				    " <gettoken (ORIGIN)"));

				return (ORIGIN);
			    }
			    
			}
			
			dce_svc_printf(CDS_S_GDA_OPTION_MSG, lineno, op);
			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug7,
			    " <gettoken '$'(ERROR)"));

			return (ERROR);

		case ';':
			while ((c = getc(fp)) != EOF && c != '\n')
				;
			goto top;

		case ' ':
		case '\t':

			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug7,
			    " <gettoken ';' (CURRENT)"));

			return (CURRENT);

		case '.':

			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug7,
			    " <gettoken '.'(DOT)"));

			return (DOT);

		case '@':

			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug7,
			    " <gettoken '@' (AT) "));

			return (AT);

		case '\n':
			lineno++;
			continue;

		default:
			(void) ungetc(c, fp);

			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug7,
			    " <gettoken (DNAME)"));

			return (DNAME);
		}
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug7,
            " <gettoken"));
}

/*
 * Take name and caonoicalize it according to following rules:
 *
 * "." means root.
 * "@" means current origin.
 * "name." means no changes.
 * "name" means append origin.
 *
 * INPUT: The name to be canonicalized(name) and the known origin(origin).
 * OUTPUT: The canonicalized name(name).
 *
 */
static void 
makename (char  *name,
          char  *origin)
{
	int n;


	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >makename"));

	if (origin[0] == '.')
		origin++;
	n = strlen(name);
	if (n == 1) 
	{
	    if (name[0] == '.') 
	    {
		name[0] = '\0';

		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <makename"));

		return;
	    }
	    if (name[0] == '@') 
	    {
		(void) strcpy(name, origin);

		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <makename"));

		return;
	    }
	}
	if (n > 0) 
	{
	    if (name[n - 1] == '.')
		name[n - 1] = '\0';
	    else 
		if (origin[0] != '\0') 
		{
		    name[n] = '.';
		    (void) strcpy(name + n + 1, origin);
		}
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <makename"));
}

static void 
endline (FILE *fp)
{
     register int c;


     DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_gda,
         svc_c_debug2,
         " >endline"));

     while (c = getc(fp))
	if (c == '\n') {
	    (void) ungetc(c,fp);
	    break;
	} else if (c == EOF)
	    break;

     DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_gda,
         svc_c_debug2,
         " <endline"));
}

static void 
fixline (FILE *fp)
{
    char	c;
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " >fixline"));

    while ((c = getc(fp)) != EOF && c != '\n')
    {
	;
    }

    if (c == '\n')
    {
	lineno++;
    }
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " <fixline"));
}


static int 
init_client (void)
{
	register FILE	*fp;
	register char	*cp;
	char			buf[BUFSIZ];
	rr_el_t			*nameser;
	int				status;


	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >init_client"));

	if ((fp = fopen(named_client_servers, "r")) == NULL)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug1,
		" init_client: fopen failed on %s : %s",
		named_client_servers,
	        strerror(errno)));

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <init_client: return(INIT_CLIENT_FOPEN)"));

	    return(INIT_CLIENT_FOPEN);
	}

	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		if (!strncmp(buf, "nameserver", sizeof("nameserver") - 1))
		{
			cp = buf + sizeof("nameserver") - 1;

			while (*cp == ' ' || *cp == '\t')
				cp++;

			if ((*cp == '\0') || (*cp == '\n'))
				continue;

			if((nameser = (rr_el_t *)malloc(sizeof(rr_el_t))) == NULL)
			{
			    dce_svc_printf(CDS_S_ALLOC_A_MSG);
			}

			ALLOC(rr_el_t, 8)

			dotname_to_fullname((unsigned char *)".", 
					    &nameser->dname_rr);

			nameser->class_rr = C_IN;
			nameser->auth_rr = 1;
			nameser->tag_rr = e_addr_nameserver;

			nameser->rr.adns.mach_addr.sin_family = AF_INET;
			nameser->rr.adns.mach_addr.sin_addr.s_addr = (u_long)inet_addr(cp);
			nameser->rr.adns.mach_addr.sin_port = ns_port;
			memmove( nameser->rr.adns.mach_addr.sin_zero,&eight_zeros, 8);

			nameser->rr.adns.auth = 1;
			nameser->rr.adns.used = 0;
			nameser->rr.adns.path = DATAGRAM;
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
			fprintf(stderr,"gdad: init_server: adding nameserver \"");
			fullname_print(&AD->dname_rr);
			fprintf(stderr,"\"");
			dump_address(&AD->rr.ad.mach_addr,stderr);
			fprintf(stderr,"\n");
#endif
#endif

			l_insert_nl((char **)&nameser, &nameser_list,
				(element_t *)NULL);
		}
	}
	(void) fclose(fp);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <init_client: return (OK)"));

	return (OK);
}

static int 
init_server (void)
{
    
#ifdef 	AD
#undef	AD
#endif
#define	AD	LIST_REF(rr_el_t, ad)
#ifdef	NS
#undef	NS
#endif
#define	NS	LIST_REF(rr_el_t, ns)
	char	domain[MAXDNAME];
	char	origin[MAXDNAME];
	register FILE 	*fp;

	register map	*mp;
	u_char			buf[MAXDATA];
	u_char			data[MAXDATA];
	int				c;
	int				class, type;
	register u_long	n;
	list_t			*ad_list;
	list_t			*ns_list;
	rr_el_t			*nameser;
	rr_el_t			*ad;
	rr_el_t			*ns;
	int				status;
	int				found;


	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >init_server"));

	if ((fp = fopen(named_servers, "r")) == NULL) 
	{
	    
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <init_server: fopen failed on %s : %s - return(INIT_SERVER_FOPEN",
		named_servers,
		strerror(errno)));

	    return(INIT_SERVER_FOPEN);
	}

	if((ad_list = (list_t *)malloc(sizeof(list_t))) == NULL)
	{
	  dce_svc_printf(CDS_S_ALLOC_A_MSG);
	}
	ALLOC(list_t, 2)

	l_init_nl(ad_list);

	if((ns_list = (list_t *)malloc(sizeof(list_t))) == NULL)
	{
	  dce_svc_printf(CDS_S_ALLOC_A_MSG);
	}
	ALLOC(list_t, 3)

	l_init_nl(ns_list);


	lineno = 1;

	class = C_IN;

	/*
	 * Read the file 
	 */
	while ((c = gettoken(fp)) != EOF)
	{
		switch (c) {
		case INCLUDE:
			/* An include entry implies that another file should
			   be read at this point. This is not allowed in
			   by the GDA but is allowed in named.ca files. */

			fixline(fp);

			dce_svc_printf(CDS_S_GDA_INCLUDE_MSG, lineno);
			continue;
		case ORIGIN:
			/* origin lines specify a domain name suffix for
				all lines that follow */
			(void) strcpy((char *)buf, origin);
			if (!getword(origin, sizeof(origin), fp))
			{
				fixline(fp);

				dce_svc_printf(CDS_S_GDA_ORIGIN_MSG, lineno);
				continue;
			}

			makename(origin, (char *)buf);
			continue;

		case DNAME:
			/* dname lines specify a name prefix for the
				current line. */
			if (!getword((char *)domain, sizeof(domain), fp))
			{
				fixline(fp);

				dce_svc_printf(CDS_S_GDA_DNAME_MSG, lineno);
				continue;
			}

			n = strlen(domain) - 1;

			if (domain[n] == '.')
				domain[n] = '\0';
			else if (*origin)
			{
				(void) strcat(domain, ".");
				(void) strcat(domain, origin);
			}
			break;

		case AT:
			/* at lines specify that the current origin, domain
				name suffix should be the domain name for
				the current line */
			(void) strcpy(domain, origin);
			break;

		case DOT:
			/*
			 * special case for DNAME entries 
			 */
			domain[0] = '\0';
			break;

		case ERROR:
			fixline(fp);
			continue;
		}

		if (!getword((char *)buf, sizeof(buf), fp)) {
			if (c != CURRENT)
			{
				fixline(fp);

				dce_svc_printf(CDS_S_GDA_TTL_MSG, lineno);

			}
			continue;
		}

		found = 0;

		if (isdigit(*buf)) {
			if (!getword((char *)buf, sizeof(buf), fp))
			{
				fixline(fp);
				dce_svc_printf(CDS_S_GDA_CLASS_MSG, lineno);
				continue;
			}
		}

		for (mp = m_class; mp < m_class+NCLASS; mp++)
			if (!strcasecmp((char *)buf, mp->token)) {
				class = mp->val;
				(void) getword((char *)buf, sizeof(buf), fp);
				break;
			}

		for (mp = m_type; mp < m_type+NTYPE; mp++)
			if (!strcasecmp((char *)buf, mp->token)) {
				type = mp->val;
				found = 1;
			}

		if(!found)
		{
			fixline(fp);
			dce_svc_printf(CDS_S_GDA_TYPE_MSG, lineno);
			continue;
		}

		if (!getword((char *)buf, sizeof(buf), fp))
		{
			fixline(fp);
			dce_svc_printf(CDS_S_GDA_DATA_MSG, lineno);
			continue;
		}

		/*
		 * Convert the ascii data 'buf' to the internal format
		 * based on the type and place in a list element.
		 */
		switch (type) {
		case T_A:
			if((ad = (rr_el_t *)malloc(sizeof(rr_el_t))) == NULL)
			{
			    dce_svc_printf(CDS_S_ALLOC_A_MSG);
			}
			ALLOC(rr_el_t, 2)

			dotname_to_fullname((unsigned char *)domain, 
					    &ad->dname_rr);

			ad->class_rr = class;
			ad->auth_rr = 1;
			ad->tag_rr = e_addr;

			ad->rr.ad.mach_addr.sin_family = AF_INET;
			ad->rr.ad.mach_addr.sin_addr.s_addr =
				(u_long)inet_addr((char *)buf);
			ad->rr.ad.mach_addr.sin_port = ns_port;
			memmove( ad->rr.ad.mach_addr.sin_zero,&eight_zeros, 8);

			l_insert_nl((char **)&ad, ad_list,
				(element_t *)NULL);

			break;

		case T_NS:
			if((ns = (rr_el_t *)malloc(
					sizeof(rr_el_t))) == NULL)
			{

			    dce_svc_printf(CDS_S_ALLOC_A_MSG);
			}
			ALLOC(rr_el_t, 3)

			(void) strcpy((char *)data, (char *)buf);
			 makename((char *)data, origin);

			dotname_to_fullname((unsigned char *)domain,
					    &ns->dname_rr);
			ns->class_rr = class;
			ns->auth_rr = 1;
			ns->tag_rr = e_nameserver;

			dotname_to_fullname((unsigned char *)data,
					    &ns->rr.ns.mach_name);

			l_insert_nl((char **)&ns, ns_list,
				    (element_t *)NULL);
			break;

		case T_HINFO:
		case T_SOA:
		case T_MINFO:
		case T_UID:
		case T_GID:
		case T_WKS:
		case T_CNAME:
		case T_MB:
		case T_MG:
		case T_MR:
		case T_PTR:
		case T_UINFO:
		case T_MX:
		case T_TXT:
                       case T_UNSPEC:
		default:
			dce_svc_printf(CDS_S_GDA_UNSUPPORTEDDATA_MSG, lineno);
			endline(fp);
			break;
		}
		/*
		 * place data in list 
		 */
	}

	/* search thorough ad_list for machine names that
		match.  Place combined entry in nameser_list */
	for(ad = (rr_el_t *)l_first_nl(ad_list);
		ad > (rr_el_t *)NULL;
		ad = (rr_el_t *)l_next_nl((element_t *)ad) )
	{
		for(ns = (rr_el_t *)l_first_nl((list_t *)ns_list);
			ns > (rr_el_t *)NULL;
			ns = (rr_el_t *)l_next_nl((element_t *)ns) )
		{
			if(fullname_cmp(&NS->rr.ns.mach_name, &AD->dname_rr)
					== EQUAL)
			{
				if((nameser = 
				    (rr_el_t *)malloc(sizeof(rr_el_t))) 
				   == NULL)
				{
				  dce_svc_printf(CDS_S_ALLOC_A_MSG);
				}
				ALLOC(rr_el_t, 4)

				nameser->dname_rr = NS->dname_rr;
				nameser->class_rr = NS->class_rr;
				nameser->auth_rr = NS->auth_rr;
				nameser->tag_rr = e_addr_nameserver;

				nameser->rr.adns.mach_addr = AD->rr.ad.mach_addr;

				nameser->rr.adns.auth = 1;
				nameser->rr.adns.used = 0;
				nameser->rr.adns.path = DATAGRAM;

				l_insert_nl((char **)&nameser,
					&nameser_list, (element_t *)NULL);
			}
		}
	}

	for(ad = (rr_el_t *)l_first_nl(ad_list);
		ad > (rr_el_t *)NULL;
		ad = (rr_el_t *)l_first_nl(ad_list) )
	{
		l_delete_nl((element_t **)&ad, ad_list);

		free((void *)ad);
		FREE(rr_el_t, 3)
	}

	for(ns = (rr_el_t *)l_first_nl(ns_list);
		ns > (rr_el_t *)NULL;
		ns = (rr_el_t *)l_first_nl(ns_list) )
	{
		l_delete_nl((element_t **)&ns, ns_list);

		free((void *)ns);
		FREE(rr_el_t, 4)
	}

	/*
	 * add list delete's 
	 */
	if((status = l_free_nl(ad_list)) != OK)
	{
	    dce_svc_printf(CDS_S_GDA_LIST_A_MSG);
	}

	free((void *)ad_list);
	FREE(list_t, 6)

	if((status = l_free_nl(ns_list)) != OK)
	{
	    dce_svc_printf(CDS_S_GDA_LIST_A_MSG);
	}

	free((void *)ns_list);
	FREE(list_t, 7)

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <init_server: return(OK)"));

	return(OK);

#undef	AD
#undef	NS

}

static int 
init_bind (void)
{
	pthread_t	recv_bind_th;
	pthread_attr_t pattr;
	int			n;
	error_status_t	uuid_status;
	int			status;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >init_bind"));
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
fprintf(stderr, "gdad: enter init_bind\n");
#endif
#endif

	if(pthread_mutex_init(&id_mutex, pthread_mutexattr_default))
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXINIT_MSG, (long)errno);
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <init_bind: error_exit'ing due to pthread_mutex_init failure"));

	    error_exit(-1);
	}

	if (pthread_attr_create(&pattr) < 0) 
	{
	    dce_svc_printf(CDS_S_PTHREADATTRCREATE_MSG, (long)errno);
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <init_bind: error_exit'ing due to pthread_attr_create failure"));

	    error_exit(-1);
	}
	if (pthread_attr_setstacksize(&pattr, _CDS_DEFAULT_STACK_) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADATTRSETSTACKSIZE_MSG, (long)errno);
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <init_bind: error_exit'ing due to pthread_attr_setstacksize failure"));

	    error_exit(-1);
	}
	if (pthread_create(&recv_bind_th, pattr,
		(pthread_startroutine_t)recv_bind, (pthread_addr_t)&n))
	{
	    dce_svc_printf(CDS_S_PTHREADCREATE_MSG, (long)errno);
	    error_exit(-1);
	}

	ns_port = htons(NAMESERVER_PORT);

	uuid_create_nil(&null_uuid, &uuid_status);

	l_init_nl(&nameser_list);

	if((status = init_server()) != OK)
	{
	    if((status = init_client()) != OK)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <init_bind: return(0) in case bind not installed"));

		return(0);	/* in case bind not installed */
	    }
	    bind_client_server = BIND_CLIENT;
	}
	else
	{
	    bind_client_server = BIND_SERVER;
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <init_bind: return(1)"));

	return(1);
}

static int 
rr_hash (rr_el_t *rr)
{
	register unsigned char	*cp;
	register unsigned long	hash;
	register long			n = 0;
	unsigned char			buf[MAX_HASH_SIZE];

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >rr_hash"));

	cp = buf;

	if(rr->dname_rr.fn_length + n > MAX_HASH_SIZE)
	{
		memmove( cp,&rr->dname_rr, MAX_HASH_SIZE - n);
		cp += MAX_HASH_SIZE - n;
	}
	else
	{
		memmove( cp,&rr->dname_rr, (int)rr->dname_rr.fn_length);
		cp += rr->dname_rr.fn_length;
		n += rr->dname_rr.fn_length;

		if(sizeof(rr->tag_rr) + n > MAX_HASH_SIZE)
		{
			memmove( cp,&rr->tag_rr, MAX_HASH_SIZE - n);
			cp += MAX_HASH_SIZE - n;
		}
		else
		{

			memmove( cp,&rr->tag_rr, sizeof(rr->tag_rr));
			cp += sizeof(rr->tag_rr);
			n += sizeof(rr->tag_rr);

			switch(rr->tag_rr)
			{
			case e_header:
				if(sizeof(HEADER) + n > MAX_HASH_SIZE)
				{
					memmove( cp,&rr->rr.head,
						MAX_HASH_SIZE - n);
					cp += MAX_HASH_SIZE - n;
				}
				else
				{
					memmove( cp,&rr->rr.head,
						sizeof(HEADER));
					cp += sizeof(HEADER);
				}
				break;
			case e_question:
				if(sizeof(int) + n > MAX_HASH_SIZE)
				{
					memmove( cp,&rr->rr.ques.type,
						MAX_HASH_SIZE - n);
					cp += MAX_HASH_SIZE - n;
				}
				else
				{
					memmove( cp,&rr->rr.ques.type,
						sizeof(int));
					cp += sizeof(int);
				}

				break;
			case e_addr:
				if(sizeof(struct sockaddr_in) + n > MAX_HASH_SIZE)
				{
					memmove( cp,&rr->rr.ad.mach_addr,
						MAX_HASH_SIZE - n);
					cp += MAX_HASH_SIZE - n;
				}
				else
				{
					memmove( cp,&rr->rr.ad.mach_addr,
						sizeof(struct sockaddr_in));
					cp += sizeof(struct sockaddr_in);
				}

				break;
			case e_nameserver:
				if(rr->rr.ns.mach_name.fn_length + n >MAX_HASH_SIZE)
				{
					memmove( cp,&rr->rr.ns.mach_name,
						MAX_HASH_SIZE - n);
					cp += MAX_HASH_SIZE - n;
				}
				else
				{
					memmove( cp,&rr->rr.ns.mach_name,
						rr->rr.ns.mach_name.fn_length);
					cp += rr->rr.ns.mach_name.fn_length;
				}

				break;
			case e_addr_nameserver:
				if(sizeof(struct sockaddr_in) + n > MAX_HASH_SIZE)
				{
					memmove( cp,&rr->rr.adns.mach_addr,
						MAX_HASH_SIZE - n);
					cp += MAX_HASH_SIZE - n;
				}
				else
				{
					memmove( cp,&rr->rr.adns.mach_addr,
						sizeof(struct sockaddr_in));
					cp += sizeof(struct sockaddr_in);
				}
				break;
			case e_cds_cell_info_txt:
				if(sizeof(uuid_t) + n > MAX_HASH_SIZE)
				{
					memmove( cp,&rr->rr.ci.nsp_uuid,
						MAX_HASH_SIZE - n);
					cp += MAX_HASH_SIZE - n;
					break;
				}
				else
				{
					memmove( cp,&rr->rr.ci.nsp_uuid,
						sizeof(uuid_t));
					cp += sizeof(uuid_t);
					n += sizeof(uuid_t);
				}

				if(sizeof(int) + n > MAX_HASH_SIZE)
				{
					memmove( cp,&rr->rr.ci.replica_type,
						MAX_HASH_SIZE - n);
					cp += MAX_HASH_SIZE - n;
					break;
				}
				else
				{
					memmove( cp,&rr->rr.ci.replica_type,
						sizeof(int));
					cp += sizeof(int);
					n += sizeof(int);
				}

				if((int)rr->rr.ci.ch_name.fn_length + n >
					MAX_HASH_SIZE)
				{
					memmove( cp,rr->rr.ci.ch_name.fn_name,
						MAX_HASH_SIZE - n);
					cp += MAX_HASH_SIZE - n;
					break;
				}
				else
				{
					memmove( cp,rr->rr.ci.ch_name.fn_name,
					    rr->rr.ci.ch_name.fn_length);
					cp += rr->rr.ci.ch_name.fn_length;
					n += rr->rr.ci.ch_name.fn_length;
				}

				if(sizeof(uuid_t) + n > MAX_HASH_SIZE)
				{
					memmove( cp,&rr->rr.ci.ch_uuid, MAX_HASH_SIZE - n);
					cp += MAX_HASH_SIZE - n;
					break;
				}
				else
				{
					memmove( cp,&rr->rr.ci.ch_uuid,
						sizeof(uuid_t));
					cp += sizeof(uuid_t);
					n += sizeof(uuid_t);
				}

				if((int)rr->rr.ci.mach_name.fn_length + n >
					MAX_HASH_SIZE)
				{
					memmove( cp,rr->rr.ci.mach_name.fn_name,
						MAX_HASH_SIZE - n);
					cp += MAX_HASH_SIZE - n;
					break;
				}
				else
				{
					memmove( cp,rr->rr.ci.mach_name.fn_name,
					    rr->rr.ci.mach_name.fn_length);
					cp += rr->rr.ci.mach_name.fn_length;
					n += rr->rr.ci.mach_name.fn_length;
				}
				break;
			default:

				DCE_SVC_DEBUG((
				    cds__svc_handle,
				    cds_svc_gda,
				    svc_c_debug5,
				    "rr_hash:  Bad RR type"));   
			}
		}

	}

	n = cp - buf;
	hash = 0;

	for (cp = buf; n-- > 0; )
	{
		hash <<= 1;
		hash += *cp++;
	}

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <rr_hash: return(hash % NUM_BUCKETS) = %d",
          (hash % NUM_BUCKETS)));

     return (hash % NUM_BUCKETS);
}

static int 
list_insert_inorder (rr_el_t         *rr,
                     list_t          *list,
                     cds_FullName_t  *cmp_name)
{
#define	REFER	LIST_REF(rr_el_t, refer)

	rr_el_t		*refer;
	rr_el_t		*old_refer;
	int			status;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug7,
          " >list_insert_inorder"));

	if((rr->rr.adns.prefix = fullname_prefix(&rr->dname_rr,
		cmp_name)) < FIRST_PREFIX)
	{     
	    DCE_SVC_DEBUG((
	        cds__svc_handle,
		cds_svc_gda,
		svc_c_debug7,
		" <list_insert_inorder"));

	    return(LIST_INSERT_INORDER_NOT_PREFIX);
	}

	for(old_refer = NULL, refer = (rr_el_t *)l_first_nl(list);
			refer > (rr_el_t *)NULL;
			old_refer = refer, refer = (rr_el_t *)l_next_nl((element_t *)refer))
	{
		/* if either rr is a larger prefix of refer or
			if rr's prefix is the same as refer's and
			rr's prefix indicates that rr is authoritative
			then place rr before rr in list ns_addr */

		if(rr->rr.adns.prefix > REFER->rr.adns.prefix)
			break;

		if(rr->rr.adns.prefix == REFER->rr.adns.prefix)
		{
			if(rr->rr.adns.auth || !REFER->rr.adns.auth)
				break;
		}
	}

	refer = old_refer;

	l_insert_nl((char **)&rr, list, (element_t *)refer);

        DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug7,
          " <list_insert_inorder: return(OK)"));

	return(OK);

#undef REFER
}

static int 
rr_cmp (rr_el_t  *rr,
        rr_el_t  *rr2)
{
    error_status_t status;

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >rr_cmp"));

    if(fullname_cmp(&rr->dname_rr, &rr2->dname_rr) == NOT_EQUAL)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <rr_cmp (NOT_EQUAL)"));

	return(NOT_EQUAL);
    }

    if(rr->class_rr != rr2->class_rr)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <rr_cmp (NOT_EQUAL)"));

	return(NOT_EQUAL);
    }
    
    if(rr->tag_rr != rr2->tag_rr)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <rr_cmp (NOT_EQUAL)"));

	return(NOT_EQUAL);
    }
    
    switch(rr->tag_rr)
    {
	case e_header:
	if(memcmp(&rr->rr.head, &rr2->rr.head, sizeof(HEADER)))
	{
	    DCE_SVC_DEBUG((
	        cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <rr_cmp (NOT_EQUAL)"));
	    
	    return(NOT_EQUAL);
	}
	
		break;
	case e_question:
	if(rr->rr.ques.type == rr2->rr.ques.type)
	{
	    DCE_SVC_DEBUG((
	        cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <rr_cmp (NOT_EQUAL)"));

	    return(NOT_EQUAL);
	}
	
		break;
	case e_addr:

		if(memcmp(&rr->rr.ad.mach_addr, &rr2->rr.ad.mach_addr,
			sizeof(struct sockaddr_in)))
		{
		    DCE_SVC_DEBUG((
		        cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <rr_cmp (NOT_EQUAL)"));

		    return(NOT_EQUAL);
		}
	
		break;
	case e_nameserver:

	        if(fullname_cmp(&rr->rr.ns.mach_name, &rr2->rr.ns.mach_name))
		{
		    DCE_SVC_DEBUG((
		        cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <rr_cmp (NOT_EQUAL)"));
		    
		    return(NOT_EQUAL);
		}
	
		break;
	case e_addr_nameserver:
		if(memcmp(&rr->rr.adns.mach_addr, &rr2->rr.adns.mach_addr,
			sizeof(struct sockaddr_in)))
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
		        cds_svc_gda,
		        svc_c_debug2,
			" <rr_cmp (NOT_EQUAL)"));

		    return(NOT_EQUAL);
		}
	
		break;
	case e_cds_cell_info_txt:
		if(!uuid_equal(&rr->rr.ci.nsp_uuid,
                               &rr2->rr.ci.nsp_uuid,
                               &status))
		{
		    DCE_SVC_DEBUG((
		        cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <rr_cmp (NOT_EQUAL)"));

		    return(NOT_EQUAL);
		}
	

	if(rr->rr.ci.replica_type, rr2->rr.ci.replica_type)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <rr_cmp (NOT_EQUAL)"));

	    return(NOT_EQUAL);
	}
	

		if (fullname_cmp(&rr->rr.ci.ch_name, &rr2->rr.ci.ch_name)
		    == NOT_EQUAL)
		{
		    DCE_SVC_DEBUG((
		        cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <rr_cmp (NOT_EQUAL)"));

		    return(NOT_EQUAL);
		}
	

		if(!uuid_equal(&rr->rr.ci.ch_uuid,
                               &rr2->rr.ci.ch_uuid,
                               &status))
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <rr_cmp (NOT_EQUAL)"));

		    return(NOT_EQUAL);
		}
	

		if (fullname_cmp(&rr->rr.ci.mach_name, &rr2->rr.ci.mach_name)
		    == NOT_EQUAL)
		{
		    DCE_SVC_DEBUG((
		        cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <rr_cmp (NOT_EQUAL)"));

		    return(NOT_EQUAL);
		}
	

		break;

	default:	
	        DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <rr_cmp (NOT_EQUAL)"));

	        return(NOT_EQUAL);
	}

      	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <rr_cmp (EQUAL)"));

    return(EQUAL);
}

static rr_el_t 
*find_rr (rr_el_t        *rr,
          question_el_t  *element)
{
	unsigned long	hash;
	rr_el_t			*rr2; 	/* rr2 is not an element in a list */
	rr_el_t *ret_val = NULL; /* ret_val is not an element in a list */

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >find_rr"));

	hash = rr_hash(rr);

	if(element->cd_q.bind.buckets[hash])
	{
		for(rr2 = element->cd_q.bind.buckets[hash];
			rr2 != NULL;
			rr2 = rr2->next_rr)
		{
			if(rr_cmp(rr, rr2) == EQUAL)
			{
				ret_val = rr2;
				break;
			}
		}
	}

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " <find_rr"));

      return(ret_val);
}

static int 
destroy_bind_process (question_el_t *element)
{

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >destroy_bind_process"));

	/*
	 * delete cond 
	 */
	if(pthread_cond_destroy(&element->cd_q.bind.cond))
	{
	    dce_svc_printf(CDS_S_PTHREADCONDDESTROY_MSG, (long)errno);
	    error_exit_th(-1);
	}

	/*
	 * mutex delete 
	 */
	if(pthread_mutex_destroy(&element->cd_q.bind.mutex))
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_MSG, (long)errno);
	    error_exit_th(-1);
	}

	if(whole_list_free_nl(&element->cd_q.bind.answers) != OK)
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug1,
                "destroy_bind_process: whole_list_free_nl failed"));
	}

	if(whole_list_free_nl(&element->cd_q.bind.ad_ns) != OK)
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug1,
                "destroy_bind_process: whole_list_free_nl failed consecutively"));
	}

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <destroy_bind_process: return(OK)"));

	return(OK);
}

static int
init_bind_process (question_el_t *element)
{
#define	RR	LIST_REF(rr_el_t, rr)

	int				looper;
	rr_el_t			*rr;
	rr_el_t			*ad_ns;
	unsigned long	hash;

/*	init
		Add all referals to the ns_addr list;
*/

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >init_bind_process"));

	/*
	 * init element data 
	 */
	if(pthread_cond_init(&element->cd_q.bind.cond,pthread_condattr_default))
	{
	    dce_svc_printf(CDS_S_PTHREADCONDINIT_MSG, (long)errno);
	    error_exit_th(-1);
	}

	if(pthread_mutex_init(&element->cd_q.bind.mutex,
			pthread_mutexattr_default))
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXINIT_MSG, (long)errno);
	    error_exit_th(-1);
	}

	l_init_nl(&element->cd_q.bind.answers);

	l_init_nl(&element->cd_q.bind.ad_ns);

	for(looper = 0; looper < NUM_BUCKETS; looper++)
	{
		element->cd_q.bind.buckets[looper] = (rr_el_t *)NULL;
	}

	element->cd_q.bind.ad_ns_answ = (rr_el_t *)NULL;

	element->cd_q.bind.num_who = 0;

	for(looper = 0; looper < MAX_NUM_Q_PER_QUES; looper++)
	{
		element->cd_q.bind.who[looper].query_num = 0;
		element->cd_q.bind.who[looper].ad_ns = (rr_el_t *)NULL;
	}

	for(rr = (rr_el_t *)l_first_nl(&nameser_list);
		rr > (rr_el_t *)NULL;
		rr = (rr_el_t *)l_next_nl((element_t *)rr))
	{
		if(find_rr(RR, element) != NULL)
			continue;

		if((ad_ns = (rr_el_t *)malloc(sizeof(rr_el_t)))	== NULL)
		{
		  dce_svc_printf(CDS_S_ALLOC_A_MSG);
		}
		ALLOC(rr_el_t, 5)

		*ad_ns = *RR;

		hash = rr_hash(ad_ns);
		if(element->cd_q.bind.buckets[hash])
		{
			ad_ns->next_rr = element->cd_q.bind.buckets[hash];
			element->cd_q.bind.buckets[hash] = ad_ns;
		}
		else
		{
			element->cd_q.bind.buckets[hash] = ad_ns;
			ad_ns->next_rr = (rr_el_t *)NULL;
		}

		(void)list_insert_inorder(ad_ns, &element->cd_q.bind.ad_ns,
			&element->unresolved_in_q);
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " <init_bind_process: return(OK)"));

#undef RR

	return(OK);
}

static int 
fullname_to_dotname (cds_FullName_t  *name,
                     unsigned char   *dotname,
                     int             length)
{
	register unsigned char *dest;
	register char *src;
	register int len;


	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
            " >fullname_to_dotname"));

	if(length < name->fn_length + 1)
	{
	    
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <fullname_to_dotname: return(FULLNAME_TO_DOTNAME_FAIL)"));	

	    return(FULLNAME_TO_DOTNAME_FAIL);
	}
	

	for(len = name->fn_length - 1, src = (char *)(name->fn_name + 1),
		dest = dotname; len != 0; src++, dest++, len--)
	{
		*dest = *((unsigned char *)src);
	}
	*dest = (unsigned char)'\0';

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
	    " <fullname_to_dotname"));	

	return(OK);
}

static int make_query_id = 0;

static int 
make_bind_query (cds_FullName_t  *name,
                 char            *buf,
                 int             *out_id)
{
	register unsigned char	*cp = (unsigned char *)buf;
	register HEADER			*head = (HEADER	*)buf;

	char					dotname[MAXDNAME];
	int						status;
	int						n;


	DCE_SVC_DEBUG((
	       cds__svc_handle,
	       cds_svc_gda,
	       svc_c_debug2,
	       " >make_bind_query"));	

	if(pthread_mutex_lock(&id_mutex))
	{
	    /*
	     * this may occur if the mutex is destroyed while 
	     * we are waiting for it 
	     */

	    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_MSG, (long)errno);
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <make_bind_query: error_exit_th'ing due to pthread_mutex_lock failed : %s",
		strerror(errno)));

	    error_exit_th(0);
	}

	*out_id = make_query_id;

	head->id = htons(make_query_id);

	make_query_id++;

	if(pthread_mutex_unlock(&id_mutex))
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_MSG, (long)errno);
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <make_bind_query: error_exit_th'ing due to pthread_mutex_unlock failed : %s",
		strerror(errno)));

	    error_exit_th(0);
	}

	head->qr = 0;
	head->opcode = QUERY;
	head->aa = 0;
	head->tc = 0;

	if(bind_client_server == BIND_CLIENT)
		head->rd = 1;
	else
		head->rd = 0;

	head->ra = 0;
	head->pr = 0;
	head->rcode = 0;
	head->qdcount = htons(1);
	head->ancount = htons(0);
	head->nscount = htons(0);
	head->arcount = htons(0);

	cp += sizeof(HEADER);

	if((status = fullname_to_dotname(name, 
					 (unsigned char *)dotname, 
					 MAXDNAME)) 
	   == FULLNAME_TO_DOTNAME_FAIL)
	{
	    /*
	     * print error 
	     */
	    DCE_SVC_DEBUG((
	        cds__svc_handle,
	        cds_svc_gda,
	        svc_c_debug2,
	        " <make_bind_query: return(MAKE_BIND_QUERY_FAIL) (1)")); 

	    return(MAKE_BIND_QUERY_FAIL);
	}

	if ((n = dn_comp((unsigned char *)dotname, cp, BIND_BUF_SIZE,
		(unsigned char **)NULL, (unsigned char **)NULL)) < 0)
	{
	    /*
	     * print error 
	     */
	    DCE_SVC_DEBUG((
	        cds__svc_handle,
	        cds_svc_gda,
	        svc_c_debug2,
	        " <make_bind_query: return(MAKE_BIND_QUERY_FAIL) (2)")); 

	    return(MAKE_BIND_QUERY_FAIL);
	}

	cp += n;

	putshort(T_ANY, cp);
	cp += 2;
	putshort(C_IN, cp);
	cp += 2;

	DCE_SVC_DEBUG((
	       cds__svc_handle,
	       cds_svc_gda,
	       svc_c_debug2,
	       " <make_bind_query"));	

	return(cp - (unsigned char *)buf);
}

static int 
sockaddr_to_tower (struct sockaddr_in  *addr,
                   byte_t              *tower)
{
	int i, j;
	byte_t *tp;
	rpc_binding_handle_t binding;
	rpc_tower_vector_t *tower_vector;
	error_status_t rpc_status;
	char str_bind[100];
	int ntowers = 0;
	static char *prot_seq[] = {"ncadg_ip_udp", "ncacn_ip_tcp", ""};


	DCE_SVC_DEBUG((
	       cds__svc_handle,
	       cds_svc_gda,
	       svc_c_debug7,
	       " >sockaddr_to_tower"));	

	tp = tower + sizeof(short);	/* save room for no. of towers */

	for (i = 0; *prot_seq[i]; i++) 
	{
	    sprintf(str_bind, "%s:%s[]", prot_seq[i],
		    inet_ntoa(addr->sin_addr));
	    rpc_binding_from_string_binding ((unsigned char *)str_bind,
					     &binding, &rpc_status);
	    /* Protocol must not be supported */
	    if (rpc_status != rpc_s_ok) 
		continue;

	    rpc_tower_vector_from_binding ( cds_ifspec, binding,
					   &tower_vector, &rpc_status);
	    if (rpc_status != rpc_s_ok) 
	    {
		dce_svc_printf(CDS_S_RPCTOWERFROMBINDING_MSG,(long)rpc_status);
		continue;
	    }
	    for (j = 0; j < tower_vector->count; j++) 
	    {
		PUTL16(tp, tower_vector->tower[j]->tower_length);
		COPY_bytes(&tower_vector->tower[j]->tower_octet_string,
			   tp, tower_vector->tower[j]->tower_length);
		tp += tower_vector->tower[j]->tower_length;
	    }
	    ntowers++;
	}	
	/*
	 * Store the no. of towers at the front of the array of towers
	 */
	INSL16(tower, ntowers);

	DCE_SVC_DEBUG((
	       cds__svc_handle,
	       cds_svc_gda,
	       svc_c_debug7,
	       " <sockaddr_to_tower"));	

	return (ntowers ? tp - tower : 0);
}


split_fullname(oldunresolved, resolved, prefix)
	cds_FullName_t	*oldunresolved;
	cds_FullName_t	*resolved;
	int				prefix;
{
	char	*fn_name_unres;
	register char	*fn_name_unres_end;
	register int	looper;

	if(prefix <= 0)
		return(SPLIT_FULLNAME_BAD_PREFIX);

	if(prefix == 1)
	{
		resolved->fn_length = 0;
		return(OK);
	}

	fn_name_unres_end = (char *)(oldunresolved->fn_name +
				oldunresolved->fn_length);

	for(looper = 1, fn_name_unres = fn_name_unres_end;
			looper < prefix; looper++)
	{
		if(looper != 1)
			fn_name_unres -=1;

		dotname_backup((unsigned char **)&fn_name_unres, oldunresolved->fn_name);
	}

	if(oldunresolved->fn_length != (fn_name_unres_end - fn_name_unres))
	{
		*resolved->fn_name = '/';

		bcopy(fn_name_unres, (char *)(resolved->fn_name + 1),
				fn_name_unres_end - fn_name_unres);

		resolved->fn_length = (fn_name_unres_end - fn_name_unres) + 1;

		oldunresolved->fn_length = (oldunresolved->fn_length -
						resolved->fn_length);
	}
	else
	{
		resolved->fn_length = oldunresolved->fn_length;

		bcopy((char *)oldunresolved->fn_name, 
		      (char *)resolved->fn_name, resolved->fn_length);

		oldunresolved->fn_length = 0;
	}
}


static list_t  *
send_recv_vc (char             *buf,
	      int              length,
	      struct sockaddr  *addr,
	      int              send_len)
{
	register unsigned char	*cp;
	u_short			len;
	struct iovec	iov[2];
	int				vc;
	unsigned char	*sendbuf;
	unsigned char	*recvbuf;
	int				first;
	int				n;
	int				status;
	int				resplen;
	int				anslen;
	list_t			*recv_answ;


	DCE_SVC_DEBUG((
	       cds__svc_handle,
	       cds_svc_gda,
	       svc_c_debug2,
	       " >send_recv_vc"));	

	if((vc = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
              dce_svc_printf(CDS_S_GDA_SOCKET_MSG, (long)errno);

	      DCE_SVC_DEBUG((
		  cds__svc_handle,
		  cds_svc_gda,
		  svc_c_debug2,
		  " <send_recv_vc: return(SEND_RECV_VC_SOCKET)"));

	      return((list_t *)SEND_RECV_VC_SOCKET);
	}

	if(connect(vc, addr, sizeof(struct sockaddr)) < 0)
	{
              dce_svc_printf(CDS_S_GDA_CONNECT_MSG, (long)errno);
	      (void) close(vc);

	      DCE_SVC_DEBUG((
		  cds__svc_handle,
		  cds_svc_gda,
		  svc_c_debug2,
		  " <send_recv_vc: return(SEND_RECV_VC_SOCKET)"));

	      return((list_t *)SEND_RECV_VC_CONNECT);
	}

	if((recvbuf = (sendbuf = (unsigned char *)malloc(send_len))) == NULL)
	{
	  dce_svc_printf(CDS_S_ALLOC_A_MSG);
	}
	ALLOC(sendbuf, 1)

	/*
	 * send length and message  
	 */

	memmove( sendbuf,buf, length);

	len = htons((u_short)length);

	iov[0].iov_base =	(caddr_t)&len;
	iov[0].iov_len =	sizeof(len);
	iov[1].iov_base =	(caddr_t)sendbuf;
	iov[1].iov_len =	length;

	if(send_len <= 4 * ONE_K)
	{
		if((status = writev(vc, iov, 2)) != iov[0].iov_len + iov[1].iov_len)
		{
		  dce_svc_printf(CDS_S_GDA_WRITEV_MSG, (long)errno);
		  
		  (void) close(vc);
		  free((void *)sendbuf);
		  FREE(sendbuf, 1);
		  
		  DCE_SVC_DEBUG((
			  cds__svc_handle,
			  cds_svc_gda,
			  svc_c_debug2,
			  " <send_recv_vc: return(SEND_RECV_VC_WRITEV)"));

		  return((list_t *)SEND_RECV_VC_WRITEV);
		}
	}
	else
	{
		for(first = 1, n = send_len; n >= 4 * ONE_K; n -= 4 * ONE_K)
		{
			if(first)
			{
				if((status = writev(vc, iov, 2)) != 4 * ONE_K)
				{
                                      dce_svc_printf(CDS_S_GDA_WRITEV_MSG, 
						     (long)errno);
				      (void) close(vc);
				      
				      free((void *)sendbuf);
				      FREE(sendbuf, 2);
 
				      DCE_SVC_DEBUG((
					  cds__svc_handle,
					  cds_svc_gda,
					  svc_c_debug2,
					  " <send_recv_vc: return(SEND_RECV_VC_WRITEV)"));

				      return((list_t *)SEND_RECV_VC_WRITEV);
				}
				first = 0;
				cp =	(unsigned char *)iov[1].iov_base +
						((4 * ONE_K) - iov[0].iov_len);
			}
			else
			{
				if((status = write(vc, (char *)cp, 4 * ONE_K)) != 4 * ONE_K)
				{
                                      dce_svc_printf(CDS_S_GDA_WRITE_MSG,
						     (long)errno);

				      (void) close(vc);
				      free((void *)sendbuf);
				      FREE(sendbuf, 3);

				      DCE_SVC_DEBUG((
					  cds__svc_handle,
					  cds_svc_gda,
					  svc_c_debug2,
					  " <send_recv_vc: return(SEND_RECV_VC_WRITEV)"));
				      return((list_t *)SEND_RECV_VC_WRITE);
				}
				cp +=  4 * ONE_K;
			}
		}
	}
	/*
	 * receive length and response 
	 */

	cp = recvbuf;
	len = sizeof(short);

	while (len != 0 && (n = read(vc, (char *)cp, (int)len)) > 0)
	{
		cp += n;
		len -= n;
	}

	if (n <= 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		"send_recv_vc: read number one"));
	    
	    if (n < 0)
	    {
		dce_svc_printf(CDS_S_GDA_READ_MSG, (long)errno);
	    }

	    (void) close(vc);
	    free((void *)sendbuf);
	    FREE(sendbuf, 4);
	    
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <send_recv_vc: return(SEND_RECV_VC_READ)"));

	    return((list_t *)SEND_RECV_VC_READ);
	}

	cp = recvbuf;

	if ((resplen = ntohs(*(u_short *)cp)) > send_len)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug5,
		"send_recv_vc: ntohs returned %d which isn't > send_len(%d)",
		resplen,
		send_len));
	    
	    len = anslen;
	}
	else
	{
		len = resplen;
	}

	while (len != 0 && (n = read(vc, (char *)cp, (int)len)) > 0)
	{
		cp += n;
		len -= n;
	}

	if (n <= 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		"send_recv_vc: read number two."));
	    
	    if (n < 0)
	    {
		dce_svc_printf(CDS_S_GDA_READ_MSG, (long)errno);
	    }

	    (void) close(vc);
	    free((void *)sendbuf);
	    FREE(sendbuf, 5);

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <send_recv_vc: return(SEND_RECV_VC_READ)"));

	    return((list_t *)SEND_RECV_VC_READ);
	}

	/*
	 * close down connection 
	 */
	(void) close(vc);

	if((recv_answ = (list_t *)malloc(sizeof(list_t))) == (list_t *)NULL)
	{
	  dce_svc_printf(CDS_S_ALLOC_A_MSG);
	}
	ALLOC(list_t, 4)

	if((status = bindq_ext_to_int(recvbuf, cp - recvbuf, recv_answ)) != OK)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug5,
		"recv_bind: bindq_ext_to_int returned error: %d",
		status));

	    /*
	     * who should dealloc half a query list ? 
	     */

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <send_recv_vc: return(SEND_RECV_VC_BINDQ)"));
	      
	    free((void *)sendbuf);
	    FREE(sendbuf, 6);

	    return((list_t *)SEND_RECV_VC_BINDQ);
	}

	DCE_SVC_DEBUG((
	     cds__svc_handle,
	     cds_svc_gda,
	     svc_c_debug2,
	     " <send_recv_vc"));

	free((void *)sendbuf);
	FREE(sendbuf, 7);

	return(recv_answ);
}

int x = 22;

static void
strip_cds_name (cds_FullName_t  *cds_name,
                cds_FullName_t  *suffix)
{
	char *where;
	int looper;


	DCE_SVC_DEBUG((
	     cds__svc_handle,
	     cds_svc_gda,
	     svc_c_debug2,
	     " >strip_cds_name"));

	if(cds_name->fn_length > 1)
	{
		for(where = (char *)cds_name->fn_name + 1, looper = 1;
			looper < cds_name->fn_length; where++, looper++)
		{
			if(*where == '/')
			{
				break;
			}
		}

		if(looper < cds_name->fn_length)
		{
			suffix->fn_length = cds_name->fn_length -
								(int)(where - (long)cds_name->fn_name);

			memmove( suffix->fn_name,where, suffix->fn_length);

			cds_name->fn_length = (int)(where - (long)cds_name->fn_name);

			DCE_SVC_DEBUG((
			    cds__svc_handle,
			    cds_svc_gda,
			    svc_c_debug2,
			    " <strip_cds_name"));

			return;
		}
	}
	suffix->fn_length = 0;

	DCE_SVC_DEBUG((
	     cds__svc_handle,
	     cds_svc_gda,
	     svc_c_debug2,
	     " <strip_cds_name"));
}

static int 
bind_process (question_el_t *element)
{
#define	BEST	LIST_REF(rr_el_t, best)
#define	AD_NS	LIST_REF(rr_el_t, ad_ns)
#define	RR		LIST_REF(rr_el_t, rr)
#define	HEAD	LIST_REF(rr_el_t, head)
#define	RR1		LIST_REF(rr_el_t, rr1)

	char				buf[BIND_BUF_SIZE];
	int					auth_match_num;
	int					num_queries;
	rr_el_t				*best;
	rr_el_t				*ad_ns;
	rr_el_t				*ci;
	rr_el_t				*answ;
	rr_el_t				*rr;
	rr_el_t				*head;
	rr_el_t				*rr1;
	int					length;
	int					query_id;
	unsigned long		hash;
	int					status;
	int					found = 0;
	int					prefix;
	int					num_memslots;
	int					curr_memslot;
	int					looper;
	int					truncated;
	int					maximal_prefix;
	int					authoritative_answer;
	int					answer_returned;
	cds_FullName_t		cds_suffix;
	char 				*fn_name;
	int					no_duplicate;
/*
bind answer routine
	init
		Add all referals to the ns_addr list;
		authoritative match number = 0

	grab mutex on data

	Search down the list in order to find a referal which has not been
	used and which has a match number equal or greater than the current
	authoritative match number and which is authoritative.
	(greater ones will be first in the list)
		If none exists, search the list for a referal which has	been
		used and which has a match number greater or equal to the
		current	authoritative match number but which is not
		authoritative.
		(authoritative entries with the same match number will be
		first in the list)
			If none exists, search the list for a referal which has
			been used and which has a match number greater or
			equal to the current authoritative match number but
			which has not yet responded after one try and which
			is authoritative.
				If none exists, search the list for a referal
				which has been used and which has a match
				number greater or equal to the current
				authoritative match number but which has not
				yet responded after one try and which is not
				authoritative.
					If none exist give up.
					format return parameters
					dealloc ns_addr list and answers list
					return results

	use the referal found

*/

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >bind_process"));

	auth_match_num = 0;

	if(pthread_mutex_lock(&element->cd_q.bind.mutex))
	{
	    /* 
	     * this may occur if the mutex is destroyed while
	     * we are waiting for it 
	     */

	    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_MSG, (long)errno);
	    error_exit_th(0);
	}

	strip_cds_name(&element->unresolved_in_q, &cds_suffix);

	maximal_prefix = fullname_prefix(&element->unresolved_in_q,
				&element->unresolved_in_q);

	for(num_queries = 0; num_queries < MAX_NUM_Q_PER_QUES; num_queries++)
	{
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug5,
                  "bind_process: next query"));

		authoritative_answer = 0;
		answer_returned = 0;

		for(best = NULL, ad_ns = (rr_el_t *)l_first_nl(
				&element->cd_q.bind.ad_ns);
			ad_ns > (rr_el_t *)NULL;
			ad_ns = (rr_el_t *)l_next_nl((element_t *)ad_ns))
		{
			if(AD_NS->rr.adns.used == TWICE)
				break;

			if(!best)
			{
				if(AD_NS->rr.adns.prefix < auth_match_num)
				{
					break;
				}

				best = ad_ns;
				continue;
			}

			if(BEST->rr.adns.prefix > AD_NS->rr.adns.prefix)
				break;

			/*
			 * The prefix's must be equal 
			 */
			if(BEST->rr.adns.used < AD_NS->rr.adns.used)
				break;

			if(AD_NS->rr.adns.used < BEST->rr.adns.used)
			{
				best = ad_ns;
				break;
			}

			/*
			 * best has been used as much as AD_NS 
			 */
			if(BEST->rr.adns.auth || !AD_NS->rr.adns.auth)
				break;

			best = ad_ns;
		}

		if(best == NULL)
			break;

/*
	format question (always same but for name)

	send request

	do cond_ timedwait

*/
		if((length = make_bind_query(&element->unresolved_in_q,
				buf, &query_id)) == MAKE_BIND_QUERY_FAIL)
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_gda,
			svc_c_debug2,
			" <bind_process: error_exit_th'ing due to make_bind_query failed : %s",
			strerror(errno)));

		    error_exit_th(0);
		}
		BEST->rr.adns.used++;

		if(BEST->rr.adns.path == VIRTUAL_CIRCUIT)
		{
			if((element->cd_q.bind.answer = send_recv_vc(buf, length,
				(struct sockaddr *)&BEST->rr.adns.mach_addr,
				BEST->rr.adns.msgsize)) < (list_t *)OK)
			{
			    DCE_SVC_DEBUG((
				cds__svc_handle,
				cds_svc_gda,
				svc_c_debug5,
				"bind_process: send_recv_vc returned error %d",    
				element->cd_q.bind.answer));  

			    continue;
			}

			element->cd_q.bind.ad_ns_answ = best;
		}
		else
		{
			utc_t time_utc;
			timespec_t time, inacc;
			long tdf;
			int utc_status;

			element->cd_q.bind.who[element->cd_q.bind.num_who].query_num =
				query_id;
			element->cd_q.bind.who[element->cd_q.bind.num_who].ad_ns = best;
			element->cd_q.bind.num_who = element->cd_q.bind.num_who + 1;
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
			fprintf(stderr, "gdad: sending bind query to ");
			dump_address(&BEST->rr.adns.mach_addr, stderr);
			fprintf(stderr, "\n");
			dump_message(buf, length, stderr);
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
			if (sendto(bind_dg_sock, buf, length, 0,
				(struct sockaddr *)&BEST->rr.adns.mach_addr,
				sizeof(struct sockaddr_in)) < 0)
			{
			    /*
			     * error bad sendto 
			     */
			    dce_svc_printf(CDS_S_GDA_SENDTO_MSG, (long)errno);
			    continue;
			}

			utc_status = utc_gettime(&time_utc);
			dce_assert(cds__svc_handle, 0 <= utc_status);
			utc_status = utc_bintime(&time, &inacc, &tdf, &time_utc);
			dce_assert(cds__svc_handle, 0 <= utc_status);

			time.tv_sec += 10;

			element->cd_q.bind.ad_ns_answ = (rr_el_t *)NULL;
			element->cd_q.bind.answer = (list_t *)NULL;

			if ((pthread_cond_timedwait(&element->cd_q.bind.cond,
				&element->cd_q.bind.mutex, &time) < 0) &&
			    (errno != EAGAIN))
			{
			   dce_svc_printf(CDS_S_PTHREADCONDTIMEDWAIT_MSG,
					  (long)errno);
			   error_exit_th(-1);
			}

                      DCE_SVC_DEBUG((
                          cds__svc_handle,
                          cds_svc_gda,
                          svc_c_debug5,
                          "bind_process: after cond timed wait"));
		}
/*
	Process bind answer
		Place answers in list answers
		Make sure the result is not a duplicate
		On ns record, search for all A records which match.
			Place result in ns_addr list in name distance order
			with authority as a sub-criterion.
			If the NS record does not have a match number
			greater than the authoritative match number, do not
			place it in the list.
		On TXT record, search for an A record in answers that is
			of TXT type and has machine name equal to the domain
			name of the A record.  If such a beast exists,
			place the A record in answers.
		dealloc answer
*/

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
		fprintf(stderr, "bind_process(): About to look at answers\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
		if(element->cd_q.bind.answer == (list_t *)NULL ||
			(head = (rr_el_t *)l_first_nl(element->cd_q.bind.answer)) == NULL)
		{
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
		      fprintf(stderr, "bind_process(): Error: bind.answer list null\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
                      DCE_SVC_DEBUG((
                          cds__svc_handle,
                          cds_svc_gda,
                          svc_c_debug1,
                          "bind_process: bind.answer list null"));

			continue;
		}
		else
		{
			if(HEAD->tag_rr != e_header)
			{
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
			    fprintf(stderr, "bind_process(): Error: header not on front of bind.answer list\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
			    DCE_SVC_DEBUG((
                                cds__svc_handle,
                                cds_svc_gda,
                                svc_c_debug5,
                                "bind_process: header not on front of bind.answer"));

			    continue;
			}

			if(HEAD->rr.head.tc == 1)
			{
				BEST->rr.adns.used--;
				BEST->rr.adns.path = VIRTUAL_CIRCUIT;
				BEST->rr.adns.msgsize = 4 * ONE_K;
				/*
				 * here should dealloc all of the answer list 
				 */
				continue;
			}

			if(HEAD->rr.head.aa && HEAD->rr.head.rcode == NXDOMAIN)
			{
				cds_COPY_Fullname(&element->unresolved_in_q,
						&element->unresolved_out_q);

				element->resolved_out_q.fn_length = 0;

				if(cds_suffix.fn_length)
				{
					memmove( &element->unresolved_out_q.fn_name[element->unresolved_out_q.fn_length],cds_suffix.fn_name,
						cds_suffix.fn_length);
					element->unresolved_out_q.fn_length +=
								cds_suffix.fn_length;
				}

				if(whole_list_free_nl(element->cd_q.bind.answer) != OK)
				{
                                      DCE_SVC_DEBUG((
                                          cds__svc_handle,
                                          cds_svc_gda,
                                          svc_c_debug1,
                                          "bind_process: whole_list_free_nl failed"));
				}
				free((void *)element->cd_q.bind.answer);
				FREE(list_t, 9);

				/*
				 * mutex unlock 
				 */
				if(pthread_mutex_unlock(&element->cd_q.bind.mutex))
				{
				    DCE_SVC_DEBUG((
                                        cds__svc_handle,
                                        cds_svc_gda,
                                        svc_c_debug2,
                                        " <bind_process: error_exit_th'ing due to pthread_mutex_unlock failed : %s",
					strerror(errno)));
				    
				    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_MSG, 
						   (long)errno);
				    error_exit_th(0);
				}

                              DCE_SVC_DEBUG((
                                  cds__svc_handle,
                                  cds_svc_gda,
                                  svc_c_debug2,
                                  " <bind_process: returning DNS_UNKNOWNENTRY"));

				return(DNS_UNKNOWNENTRY);
			}
		}

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
		fprintf(stderr, "bind_process(): About to look at head list\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
	      for(truncated = 0, rr = (rr_el_t *)l_next_nl((element_t *)head);
		  rr > (rr_el_t *)NULL;
		  rr = (rr_el_t *)l_next_nl((element_t *)head))
		  {
		    if(find_rr(RR, element) != NULL)
			continue;
		    
		    if(!truncated)
			{
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
                        fprintf(stderr, "bind_process(): in trucate test loop;tag_rr=%d\n", RR->tag_rr);
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
			switch (RR->tag_rr) {
		       case e_addr:
			  break;
			case e_nameserver:
			  for(rr1 = (rr_el_t *)l_next_nl((element_t *)rr);
			      rr1 > (rr_el_t *)NULL;
			      rr1 = (rr_el_t *)l_next_nl((element_t *)rr1))
			      {
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
				fprintf(stderr, "bind_process(): in e_nameserver;tag_rr=%d\n", RR1->tag_rr);
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
				switch (RR1->tag_rr) {
				case e_addr:
				  if(fullname_cmp(&RR->rr.ns.mach_name, &RR1->dname_rr) == EQUAL)
				      {
					if((ad_ns = (rr_el_t *)malloc(sizeof(rr_el_t)))
					   == NULL)
					    {
					      
					      dce_svc_printf(CDS_S_ALLOC_A_MSG);
					    }
					ALLOC(rr_el_t, 6)
					    
					ad_ns->dname_rr = RR->dname_rr;
					ad_ns->class_rr = RR->class_rr;
					ad_ns->auth_rr = RR->auth_rr;
					ad_ns->tag_rr = e_addr_nameserver;
					
					ad_ns->rr.adns.mach_addr = RR1->rr.ad.mach_addr;
					ad_ns->rr.adns.auth = HEAD->rr.head.aa;
					ad_ns->rr.adns.used = 0;
					ad_ns->rr.adns.path = DATAGRAM;
					
					/* fix auth entry according to auth bit */
					if(find_rr(ad_ns, element) != NULL)
					    {
					      free((void *)ad_ns);
					      FREE(rr_el_t, 5);
					      continue;
					    }
					
					hash = rr_hash(ad_ns);
					
					if(element->cd_q.bind.buckets[hash])
					    {
					      ad_ns->next_rr =
						  element->cd_q.bind.buckets[hash];
					      element->cd_q.bind.buckets[hash] = ad_ns;
					    }
					else
					    {
					      element->cd_q.bind.buckets[hash] = ad_ns;
					      ad_ns->next_rr = (rr_el_t *)NULL;
					    }
					
					if((status = list_insert_inorder(ad_ns,
									 &element->cd_q.bind.ad_ns,
									 &element->unresolved_in_q))
					   != OK)
					    {
					      dce_svc_printf(CDS_S_GDA_INORDER_MSG, (long)status);
					      error_exit_th(-1);
					    }
				      }
				  break;
				      default:
				  break;
				}
			      }
			  break;
			      case e_cds_cell_info_txt:
			  for(rr1 = (rr_el_t *)l_next_nl((element_t *)rr),
			      ci = NULL;
			      rr1 > (rr_el_t *)NULL;
			      rr1 = (rr_el_t *)l_next_nl((element_t *)rr1))
			      {
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
				fprintf(stderr, "bind_process(): in e_cds_cell_info_txt;tag_rr=%d\n", RR1->tag_rr);
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
				switch (RR1->tag_rr) {
				      case e_addr:
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
				  fprintf(stderr, "bind_process(): in e_cds_cell_info_txt/e_addr: n1=\"");
				  fullname_print(&RR->rr.ci.mach_name);
				  fprintf(stderr,"\";n2=\"");
				  fullname_print(&RR1->dname_rr);
				  fprintf(stderr,"\"\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
				  if(fullname_cmp(&RR->rr.ci.mach_name, &RR1->dname_rr)
				     == EQUAL)
				      {
					if(ci == NULL)
					    {
					      if((ci = (rr_el_t *)malloc(sizeof(rr_el_t)))
						 == NULL)
						  {
						    dce_svc_printf(CDS_S_ALLOC_A_MSG);
						  }
					      ALLOC(rr_el_t, 7)
						  
						  *ci = *RR;
					      ci->rr.ci.num_addr = 0;
					    }
					
					no_duplicate = OK;
					
					for(looper = 0; looper < ci->rr.ci.num_addr;
					    looper++)
					    {
					      if(!memcmp(&(ci->rr.ci.mach_addr[looper]),
							 &(RR1->rr.ad.mach_addr),
							 sizeof(struct sockaddr_in)))
						  {
						    no_duplicate = OK - 1;
						  }
					    }
					
					if(no_duplicate == OK)
					    {
					      ci->rr.ci.mach_addr[ci->rr.ci.num_addr] =
						  RR1->rr.ad.mach_addr;
					      ci->rr.ci.num_addr++;
					    }
				      }
				  break;
				      default:
				  break;
				}
				
				if(ci != NULL && ci->rr.ci.num_addr == MAX_NUM_ADDR_CI)
				    break;
			      }
			  
			  if(ci != NULL && ci->rr.ci.num_addr > 0)
			      {
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
				fprintf(stderr, "bind_process(): Truncation free\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
				l_insert_nl((char **)&ci, 
					    &element->cd_q.bind.answers,
					    (element_t *)NULL);
			      }
			  else
			      {
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
				fprintf(stderr, "bind_process(): Truncation found\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
				/* The message might have been truncated without having
				   the tc bit set.  The doaddinfo routine may not have
				   been able to add all of the data that could have
				   been added.  This will not trigger the truncation
				   switch because it is not data that I requested. */
				
				if(BEST->rr.adns.path != VIRTUAL_CIRCUIT)
				    {
				      BEST->rr.adns.path = VIRTUAL_CIRCUIT;
				      BEST->rr.adns.msgsize = 4 * ONE_K;
				      BEST->rr.adns.used--;
				    }
				else if(BEST->rr.adns.msgsize < VC_MSG_SIZE_MAX)
				    {
				      BEST->rr.adns.msgsize = BEST->rr.adns.msgsize * 4;
				      BEST->rr.adns.used--;
				    }
				truncated = 1;
				for(rr1 = (rr_el_t *)l_first_nl(
								&element->cd_q.bind.answers);
				    rr1 > (rr_el_t *)NULL;
				    rr1 = (rr_el_t *)l_first_nl(
								&element->cd_q.bind.answers))
				    {
				      l_delete_nl((element_t **)&rr1, &element->cd_q.bind.answers);
				    }
			      }
			  
			  break;
			default:
			  break;
			}
			
		      }
		    /*
		     * dealloc entry 
		     */
		    l_delete_nl((element_t **)&rr, element->cd_q.bind.answer);
		    free((void *)rr);
		    FREE(rr_el_t, 6);
		  }
	      
	      if(HEAD->rr.head.aa)
		  authoritative_answer = 1;
	      
	      if(HEAD->rr.head.ancount)
		  answer_returned = 1;
	      
	      l_delete_nl((element_t **)&head, element->cd_q.bind.answer);
	      free((void *)head);
	      FREE(rr_el_t, 7);
	      
	      /*
	       * delete answer list 
	       */
	      if((status = l_free_nl(element->cd_q.bind.answer)) != OK)
		{
		    dce_svc_printf(CDS_S_GDA_LIST_A_MSG);
		}

	      free((void *)element->cd_q.bind.answer);
	      FREE(list_t, 8);
	      
/*
  Search answers for answer (only if answers changed?)
  If the question is answered
  format return parameters
  dealloc ns_addr list and answers list
  return results
  Otherwise,
  Look from the top of the list for another referal
*/


		for(found = 0,
			rr = (rr_el_t *)l_first_nl(&element->cd_q.bind.answers),
			element->replicas_q = NULL;
			rr > (rr_el_t *)NULL;
			rr = (rr_el_t *)l_next_nl((element_t *)rr))
		{
			if(!found)
			{
				if((prefix = fullname_prefix(&RR->dname_rr,
					&element->unresolved_in_q)) < FIRST_PREFIX)
				{
					continue;
				}
				else
				{
#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
					fprintf(stderr, "bind_process(): seemingly, a match was found; elem:");
					fullname_print(&element->unresolved_in_q);
					fprintf(stderr, "\"\n");
					fprintf(stderr,"NS uuid: ");
					uuid_print(&RR->rr.ci.nsp_uuid);
					fprintf(stderr, "\nCH uuid: ");
					uuid_print(&RR->rr.ci.ch_uuid);
					fprintf(stderr, "\n");

#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
					found = 1;
					answ = RR;
					num_memslots = 0;
					curr_memslot = 0;

					/*
					 * set the nsp uuid equal to the nsp uuid in the rr
					 */
					element->nsp_uuid_q = RR->rr.ci.nsp_uuid;

					/*
					 * set the dir uuid equal to the nsp uuid 
					 */
					element->dir_uuid_q = RR->rr.ci.nsp_uuid;
					/*
					 * set the dir name to null 
					 */
					dotname_to_fullname(
							 (unsigned char *)"\0",
							&element->dir_name_q);

				 	/*
					 *format resolved and unresolved names 
					 */
					cds_COPY_Fullname(&element->unresolved_in_q,
							&element->resolved_out_q);

					element->unresolved_out_q.fn_length = 0;
				}
			}
			else
			{
				if(fullname_cmp(&RR->dname_rr, &answ->dname_rr)
					!= EQUAL)
				{
					continue;
				}
			}

                      DCE_SVC_DEBUG((
                          cds__svc_handle,
                          cds_svc_gda,
                          svc_c_debug5,
                          "bind_process: answer was found"));

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
		      fprintf(stderr, "bind_process(): an answer was found\n");
#endif /* SNI_INTENSE_GDA_DEBUG */
#endif /* SNI_SVR4 */
			/*
			 * an answer was found, format return parameters 
			 */
			if(curr_memslot == num_memslots)
			{
				if(element->replicas_q == NULL)
				{
					if((element->replicas_q = (cds_ReplicaPointer_t **)
							malloc(5 * sizeof(cds_ReplicaPointer_t *))
								) == (cds_ReplicaPointer_t **)NULL)
					{
					  dce_svc_printf(CDS_S_ALLOC_A_MSG);
					}
					ALLOC(replicas_q, 1)
				}
				else
				{
					if((element->replicas_q = (cds_ReplicaPointer_t **)
							realloc(element->replicas_q,
								(num_memslots + 5) *
									sizeof(cds_ReplicaPointer_t *)))
							== (cds_ReplicaPointer_t **)NULL)
					{
					    dce_svc_printf(CDS_S_ALLOC_A_MSG);
					}
					ALLOC(realloc_replicas_q, 1)
				}

				num_memslots += 5;
			}

#define MAX_REP_LEN	sizeof(cds_ReplicaPointer_t) + 2000

			element->replicas_q[curr_memslot] =
					(cds_ReplicaPointer_t *)rpc_ss_allocate(MAX_REP_LEN);

			if (element->replicas_q[curr_memslot] == NULL) 
			    dce_svc_printf(CDS_S_ALLOC_A_MSG);

			ALLOC(ss_cds_ReplicaPointer_t, 1)

			element->replicas_q[curr_memslot]->rp_type =
				(ndr_usmall_int)RR->rr.ci.replica_type;

			/*
			 * set the ch uuid 
			 */
			element->replicas_q[curr_memslot]->rp_chid = RR->rr.ci.ch_uuid;

			if(memcmp("/...", RR->rr.ci.ch_name.fn_name, 4)) 
			{
				fn_name = (char *)element->replicas_q[curr_memslot]->
														rp_chname.fn_name;

				/* The name stored in the rr is a simplename.  Convert it to
					a fullname and store it in the element. */

				/* copy the simple name to the fullname place /... on the front
					of the name and the name of the clearinghouse on back */

				/*
				 * /... on front first 
				 */

				memmove( fn_name,"/...", 4);
				fn_name += 4;

				/*
				 * place the fullname in the middle 
				 */
				memmove( fn_name,element->resolved_out_q.fn_name,
								element->resolved_out_q.fn_length);

				fn_name += element->resolved_out_q.fn_length;

				/*
				 * add the clearing house name on the end 
				 */
				memmove( fn_name,RR->rr.ci.ch_name.fn_name,
						  RR->rr.ci.ch_name.fn_length);
				element->replicas_q[curr_memslot]->rp_chname.fn_length =
						4 + element->resolved_out_q.fn_length +
						RR->rr.ci.ch_name.fn_length;

			}
			else
			{
				/* The clearing house name is a fullname.  Just copy it
					to the element */

				memmove(
						element->replicas_q[curr_memslot]->rp_chname.fn_name,RR->rr.ci.ch_name.fn_name,
						RR->rr.ci.ch_name.fn_length);
				element->replicas_q[curr_memslot]->rp_chname.fn_length =
						RR->rr.ci.ch_name.fn_length;
			}

			/*
			 * Insert the uuid into the chname 
			 */
			COPY_ObjUID(cdsWildRoot,
				&element->replicas_q[curr_memslot]->rp_chname.fn_root);

			if ((element->replicas_q[curr_memslot]->rp_length =
					sockaddr_to_tower(&RR->rr.ci.mach_addr[looper],
						element->replicas_q[curr_memslot]->rp_towers)) == 0) {
				found = 0;
				break;	/*
					 * bogus addresses - try next replica 
					 */
			}

			curr_memslot++;
		}

		if(found)
		{
			element->num_replicas_q = curr_memslot;
			break;
		}
		else if(authoritative_answer && answer_returned)
		{
			break;
		}
	}

	/*
	 * free memory 
	 */

	/*
	 * mutex unlock 
	 */
	if(pthread_mutex_unlock(&element->cd_q.bind.mutex))
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <bind_process: error_exit_th'ing due to pthread_mutex_unlock failed : %s",
		strerror(errno)));

	    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_MSG, (long)errno);
	    error_exit_th(0);
	}

	if(found)
	{
		if(cds_suffix.fn_length)
		{
			memmove( &element->unresolved_out_q.fn_name[element->unresolved_out_q.fn_length],cds_suffix.fn_name,
				cds_suffix.fn_length);
			element->unresolved_out_q.fn_length += cds_suffix.fn_length;
		}

              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug2,
                  " <bind_process: return(DNS_SUCCESS)"));

		return(DNS_SUCCESS);
	}
	else if(authoritative_answer && answer_returned)
	{
		cds_COPY_Fullname(&element->unresolved_in_q,
				&element->unresolved_out_q);

		element->resolved_out_q.fn_length = 0;

		if(cds_suffix.fn_length)
		{
			memmove( &element->unresolved_out_q.fn_name[element->unresolved_out_q.fn_length],cds_suffix.fn_name,
				cds_suffix.fn_length);
			element->unresolved_out_q.fn_length += cds_suffix.fn_length;
		}

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
fprintf(stderr, "gdad: Error: bind_process:DNS_UNKNOWNENTRY\n");
#endif
#endif
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug2,
                  " <bind_process: return(DNS_UNKNOWNENTRY)"));

		return(DNS_UNKNOWNENTRY);
	}
	else
	{
		if(cds_suffix.fn_length)
		{
			memmove( &element->unresolved_out_q.fn_name[element->unresolved_out_q.fn_length],cds_suffix.fn_name,
				cds_suffix.fn_length);
			element->unresolved_out_q.fn_length += cds_suffix.fn_length;
		}

#ifdef SNI_SVR4
#ifdef SNI_INTENSE_GDA_DEBUG
fprintf(stderr, "gdad: Error: bind_process:DNS_NAMESERVERERRORS\n");
#endif
#endif
              DCE_SVC_DEBUG((
                  cds__svc_handle,
                  cds_svc_gda,
                  svc_c_debug2,
                  " <bind_process: return(DNS_NAMESERVERERRORS)"));

		return(DNS_NAMESERVERERRORS);
	}
#undef	BEST
#undef	AD_NS
#undef	RR
#undef	HEAD
#undef	RR1

	DCE_SVC_DEBUG((
	       cds__svc_handle,
	       cds_svc_gda,
	       svc_c_debug2,
	       " <bind_process"));
}


