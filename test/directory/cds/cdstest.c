/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdstest.c,v $
 * Revision 1.1.14.2  1996/02/18  00:23:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:22  marty]
 *
 * Revision 1.1.14.1  1995/12/08  22:08:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:43:26  root]
 * 
 * Revision 1.1.11.3  1994/06/23  18:30:58  mccann
 * 	fix to match new message names
 * 	[1994/06/23  15:49:47  mccann]
 * 
 * Revision 1.1.11.2  1994/03/17  14:47:42  jd
 * 	include <rpc.h> -> include <dce/rpc.h>
 * 	[1994/03/17  14:47:08  jd]
 * 
 * Revision 1.1.11.1  1994/03/12  22:14:17  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  20:00:24  peckham]
 * 
 * Revision 1.1.8.3  1993/08/19  21:22:47  pwang
 * 	Included cdsclerk.h from exported area and included
 * 	cds_clerkserver.h from the test's obj area
 * 	[OT#6773] [OT#7835]
 * 
 * Revision 1.1.8.2  1993/08/18  14:10:09  pwang
 * 	Adjusted the size of input buffer in do-while loop
 * 	[OT#6103]
 * 
 * Revision 1.1.5.4  1993/02/24  13:20:55  jd
 * 	New version.
 * 	[1993/02/23  22:59:58  jd]
 * 
 * Revision 1.1.5.3  1993/01/12  18:25:58  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/07  01:10:25  bbelch]
 * 
 * Revision 1.1.5.2  1992/12/10  15:39:29  mfox
 * 	Replace usage of obsolete utc functions
 * 	[1992/12/10  15:37:02  mfox]
 * 
 * Revision 1.1.3.3  1992/06/09  14:27:45  mfox
 * 	Add test to verify canonicalization of typed names & fix buffering bugs.
 * 	[1992/06/09  14:25:01  mfox]
 * 
 * Revision 1.1.3.2  1992/04/02  15:07:22  grober
 * 	Fix from DEC to help resolve unresolved reference bug from
 * 	defect 2431.  The undefined symbol was cdsCacheDir.
 * 	[1992/04/02  15:05:11  grober]
 * 
 * Revision 1.1  1992/01/19  14:32:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcecdsmsg.h>
#include <dce/dce_error.h>
#include <dce/dcesvcmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/rpc.h>
#include <dce/cdsclerk.h>
#ifdef DCE_SEC
#include <dce/sec_login.h>
#endif
#include <dce/cds_clerkserver.h>

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>



#define MAX_STRLEN	1000

#ifdef DCE_SEC
int Authentication = 0;
#endif

int cumulative_status = CDS_SUCCESS;

extern rpc_if_handle_t cds_ifspec;
struct cdsFlagStat flags;
u_char partialRes[MAX_STRLEN];
cds_timeout_t timeout;


/*
** Local Prototypes
*/
int 
get_func(void);

void 
addatt(void);

void 
addmember(void);

void 
addrep(void); 

void 
cachedir(void);

void 
createdir(void); 

void 
creategrp(void); 

void 
createlink(void); 

void 
createobj(void);

void 
cdsname(void); 

void 
gdsname(void);

void 
rmatt(void); 

void 
deletedir(void); 

void 
deletegrp(void); 

void 
deletemember(void);

void 
deletelink(void); 

void 
deleteobj(void); 

void 
remrep(void); 

void 
rmattval(void); 

void 
enumatt(void);

void 
enumchild(void); 

void 
enumgroup(void);

void 
enumobj(void);

void 
enumsoft(void);

void 
expand_name(void);

void 
readatt(void); 

void 
readmember(void); 

void 
resname(void); 

void 
skulk(void);

void 
testatt(void); 

void 
testmember(void); 

void 
strtotower(void); 

void 
quit(void);


struct {
	char *func_prompt;
	void (*func_ptr) ();
} funcs[] = {
	"AV = cdsAddAttrValue(entry, etype, attribute, atype, value)", addatt,
	"AM = cdsAddMbr(group, member, type)", addmember,
	"AR = cdsAddReplica(directory, clearinghouse, replicaType)", addrep,
	"CD = cdsCreateDir(directory, clearinghouse)", createdir,
	"CG = cdsCreateGrp(groupname)", creategrp,
	"CS = cdsCreateSLink(linkName, targetName)", createlink,
	"CO = cdsCreateObj(objectName, class, version)", createobj,
	"CN = cdsCvtFullToOpq(name)", cdsname,
	"DA = cdsRmAttr(entry, etype, attribute, atype)", rmatt,
	"DD = cdsDelDir(directory)", deletedir,
	"DG = cdsDelGrp(groupname)", deletegrp,
	"DM = cdsRmMbr(group, member, type)", deletemember,
	"DS = cdsDelSLink(linkName)", deletelink,
	"DO = cdsDelObj(objectName)", deleteobj,
	"DR = cdsRmReplica(directory, clearinghouse)", remrep,
	"DV = cdsRmAttrValue(entry, etype, attribute, value)", rmattval,
	"EA = cdsEnumAttr(entry, etype)", enumatt,
	"EC = cdsEnumChild(directory, wildcard)", enumchild,
	"EG = cdsEnumGrp(directory, wildcard)", enumgroup,
	"EO = cdsEnumObj(directory, wildcard, class)", enumobj,
	"ES = cdsEnumSLink(directory, wildcard)", enumsoft,
	"GN = dns_CvtCDSFullToGDS(name)", gdsname,
	"NE = cdsExpandStrFull(fullname1)", expand_name,
	"RV = cdsReadAttrValue(entry, etype, attribute)", readatt,
	"RM = cdsReadMbr(group)", readmember,
	"VN = cdsResolveName(linkName)", resname,
	"SD = cdsSkulkDir(directory)", skulk,
	"TV = cdsTestAttrValue(entry, etype, attribute, value)", testatt,
	"TM = cdsTestMbr(group, member)", testmember,
	"ST = cdsCvtStrToTower(binding_p, towerset_p)", strtotower,
	"ZZ = exit", quit,
	"", (void (*)(void))( 0)
};


int batch;

void 
cds_event (char            *call,
           error_status_t  status)
{
	unsigned char partial[MAX_STRLEN];
	int bufferLen = sizeof(partial);
	int stat;


	fprintf (stderr, "\n%s status = ", call );
	dce_fprintf(stderr, status);

	if ( flags.fsCDSStat == CDS_UNKNOWNENTRY - CDS_NAMESERVERERRORS ) {
	    stat = cdsCvtFullToStr(flags.fsPartialRes, partial, &bufferLen);
	    if ( stat == CDS_SUCCESS ) {
	        fprintf (stderr, "\nPartial result: %s\n", partial );
	    }
	    else {
		flags.fsCDSStat = stat;	/* avoid infinite recursion */
		cds_event ( "cdsCvtFullToStr", stat);
	    }
	}
}

#ifdef DCE_SEC
/*
 * Determine if authentication is required.
 */
void 
do_login (void)
{

  error_status_t sec_status;
  sec_login_handle_t login_context_h;
  static char txt_buf[200] = "sec_login_get_current_context";

  /* Get current context and save it */
  sec_login_get_current_context(&login_context_h, &sec_status);
  if (sec_status == error_status_ok)
    Authentication = 1;
  else {
    fprintf(stderr, "sec_login_get_current_context: status = ");
    dce_fprintf(stderr, sec_status);
    Authentication = 0;
 }
}

#endif  /* DCE_SEC */


int 
main (void)
{
	int i;
        error_status_t     status;
	dce_error_string_t error_text;  
	int                temp_status;


        /*
         * Exit if we fail to define the default message table
         */
        dce_msg_define_msg_table(cds_msg_table,
                                 sizeof(cds_msg_table) / sizeof(cds_msg_table[0]),
                                 &status);
	/*
	 * Set up interface spec. for cdsCvtStrToTower
	 */
	cds_ifspec = cds_clerkserver_v1_0_c_ifspec;

#ifdef DCE_SEC
	do_login();   /* set up a secure login */
#endif
	batch = !isatty(fileno(stdin));
	if (batch) {
		setvbuf(stdin, NULL, _IOLBF, BUFSIZ);
		setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
		setvbuf(stderr, NULL, _IOLBF, BUFSIZ);
	}

	while(1) {
		i = get_func();
		(*funcs[i].func_ptr)();
	}
}


void 
prompt (char *str)
{
	if (!batch) {
                if (*str == '\n')
                        putchar(*str);
                else
                        printf("%s> ", str);
        }
}


char 
*get_ent (char *string)
{
	char *val, *vp;
	int len, slen, needmore;

	/*
	 * issue the prompt (unless in batch mode)
	 */
	prompt(string);
	/*
	 * allocate an initial buffer
	 */
	len = 32;
	val = (char *)malloc(len);
	vp = val;
	do {
		/*
		 * get the input string but terminate if an error occurred
		 */
#ifndef ultrix
		if (fgets(vp, len, stdin) == NULL) {
			prompt("\n");
			quit();
		}
#else
		/*
		 * fgets on ULTRIX is brain-dead when reading from pipes.
		 * It returns spurious NULLs in 4.2
		 */

		int e, i;

		memset(vp, 0, len);
		for (i = 0; i < len; i++) {
			e = read(fileno(stdin), vp, 1);
			if (e == 0) {
				prompt("\n");
				quit();
			} else if (e < 0) {
				perror("read");
				prompt("\n");
				quit();
			}
			if (*vp == '\n')
				break;
			else
				vp++;
		}
		vp = val;
#endif	/* ultrix */

                /*
                 * echo the input if stdin is NOT a tty since this
                 * makes scripts that pipe commands to cdstest much
                 * more meaningful
                 */
                if (batch)
                    fputs(vp, stdout);

		/*
		 * if the last char (before the terminating null) is a
		 * newline we have the entire string; otherwise, reallocate
		 * a larger buffer and get some more
		 */
		slen = strlen(val);
		if (*(val + slen - 1) == '\n')
			needmore = 0;
		else {
			len *= 2;
			val = (char *)realloc(val, len);
			vp = val + slen;
			needmore = 1;
		}
	} while (needmore);
	/*
	 * lose the newline and return the string
	 */
	*(val + slen - 1) = 0;
	return(val);
}


int 
get_num (char *string)
{
	char *num;
	int n;

	num = get_ent(string);
	n = atoi(num);
	free(num);
	return(n);
}

cds_timeout_t 
*get_timeout (void)
{
  char *expire_p, *extend_p;

  while (1) {
    expire_p = get_ent("Expiration Date ");
    if (strlen(expire_p) == 0)
      return((cds_timeout_t *)NULL);
    if (utc_mkasctime(&timeout.expire, expire_p) >= 0) break;
    fprintf(stderr, "invalid utc\n");
    }

  while (1) {
    extend_p = get_ent("Extension Date ");
    if (utc_mkascreltime(&timeout.extend, extend_p) >= 0) break;
    fprintf(stderr, "invalid utc\n");
  }

  free(expire_p);
  free(extend_p);
  return(&timeout);
}

cds_attrvalue_t 
*get_val (char *string)
{
	static char *s;
	static cds_attrvalue_t	val;

	if (s)
		free(s);
	s = get_ent(string);
	if (isdigit(*s)) {
		val.val_syntax = cds_long;
		val.val_u.val_long = atol(s);
	} else {
		val.val_syntax = cds_char;
		val.val_u.val_char.length = strlen(s) + 1;
		val.val_u.val_char.char_p = s;
	}
	return(&val);
}

int 
match (char  *a,
       char  *b)
{
	if (!isalpha(*a) || !isalpha(*(a+1)))
		return(0);
	return(toupper(*a) == *b && toupper(*(a+1)) == *(b+1));
}


int 
get_func (void)
{
	char *res;
	int i;

	while(1) {
		res = get_ent("CDS>");
		for (i = 0; funcs[i].func_ptr != NULL; i++)
			if (match(res, funcs[i].func_prompt)) {
				free(res);
				return(i);
			}
		if (*res == 'H' || *res == 'h' || *res == '?')
			for (i = 0; *funcs[i].func_prompt != 0; i++)
				puts(funcs[i].func_prompt);
		else
			puts("You need help; type h to get it\n");
		free(res);
	}
}


void 
fzero (void)
{
	memset ((char *) &flags, 0, sizeof flags);
	flags.fsPartialRes = (cds_full_name_t *)partialRes;
	flags.fsResLength = sizeof(partialRes);
	flags.fsNameType = cdsStrDCE;
	flags.fsCacheTimeout = 120;  /* 2 minutes */
#ifdef DCE_SEC
	flags.fsUnAuth = flags.fsTrustAll = !Authentication;
#endif
}


void 
outlink (void)
{
	if ( flags.fsFromCache )
	    fprintf(stdout, "Call used cache\n");
	if ( flags.fsOutLink )
	    fprintf(stdout, "Call traversed softlinks\n");
}

#ifdef VMS

#define gtime(a)
#define timeit()

#else

void 
gtime (struct timeval *t)
{
	struct timezone tz;

	gettimeofday(t, &tz);
}


struct timeval start, finish;

void 
timeit (void)
{
	register int interval;
	static int b_count = 0;
	static int p_count = 0;

	interval = ( finish.tv_sec - start.tv_sec ) * 1000;
	interval += ( finish.tv_usec - start.tv_usec ) / 1000;
	b_count += interval;
	p_count++;
	fprintf(stdout, "CLERK time = %d ms\n", interval);
	if(p_count >= 10) {
	    fprintf(stdout, "CLERK avg. time = %d ms\n", b_count / p_count );
	    p_count = 0;
	    b_count = 0;
	}
}
#endif /* VMS */

void 
quit (void)
{
    exit ( cumulative_status == CDS_SUCCESS ? 0 : 1 );
}


void 
expand_name (void)
{
	u_char *full_name;
	u_char strname[1024];
	u_char *strname_p = strname;
	int strnamelen = sizeof(strname);
	int *strnamelen_p = &strnamelen;
	int ret;

	full_name = (u_char *)get_ent("FullName");

	ret = cdsExpandStrFull(full_name, strname_p, strnamelen_p);

	if (ret == CDS_SUCCESS) {
		fprintf(stdout, "Expanded \"%s\" to", full_name);
		if (strname_p)
			fprintf(stdout, " \"%s\"", strname_p);
		else
			fprintf(stdout, " -");
		if (strnamelen_p)
			fprintf(stdout, "(%d)\n", *strnamelen_p);
		else
			fprintf(stdout, "(-)\n");
	} else
		fprintf(stderr, "cdsExpandStrFull failed: len %d\n", strnamelen);

	free(full_name);
}


void 
testmember (void)
{
	cds_full_name_t *group;
	cds_full_name_t *member;
	int ret;

	group = (cds_full_name_t *)get_ent("Group");
	member = (cds_full_name_t *)get_ent("Member");

	fzero();

	gtime(&start);
	ret = cdsTestMbr(group,			/* group name */
			 member, 		/* membername */
			 (int *) NULL,		/* direct */
			 (cds_timeout_t *) NULL,/* timeout */
			 (cds_id_t *) NULL,	/* loopdetect */
			 &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    fprintf(stdout, "A match %s found\n", ret == CDS_SUCCESS ? "was" : "wasn't");
	    if (((ret == CDS_SUCCESS) && (flags.fsLocStat == CDS_SUCCESS_STALE)) ||
		((ret == CDS_NOTFND) && (flags.fsLocStat == CDS_NOTFND_STALE)))
	        fprintf(stdout, "May be stale\n");
	    outlink();
	    }
	else cds_event ( "cdsTestMbr", flags.fsLocStat);


	free(group);
	free(member);
}

void 
addmember (void)
{
	cds_full_name_t *group;
	cds_full_name_t *member;
	int type;
	enum cdsGrpMbrType mtype;
	int ret;

	group = (cds_full_name_t *)get_ent("Group");
	member = (cds_full_name_t *)get_ent("Member");
	type = get_num("Type");

	mtype = (enum cdsGrpMbrType) type;
	fzero();

	gtime(&start);
	ret = cdsAddMbr(group,		/* group name */
			member, 	/* membername */
			mtype,		/* principal or group member */
			&flags);
	gtime(&finish);

	if (ret != CDS_SUCCESS)
		cds_event ( "cdsAddMbr", flags.fsLocStat);
	timeit();
	outlink();

	free(group);
	free(member);
}


void 
deletemember (void)
{
	cds_full_name_t *group;
	cds_full_name_t *member;
	int type;
	enum cdsGrpMbrType mtype;
	int ret;

	group = (cds_full_name_t *)get_ent("Group");
	member = (cds_full_name_t *)get_ent("Member");
	type = get_num("Type");

	mtype = (enum cdsGrpMbrType) type;
	fzero();

	gtime(&start);
	ret = cdsRmMbr(group,		/* group name */
		       member, 		/* membername */
		       mtype,		/* principal or group member */
		       &flags);
	gtime(&finish);

	if (ret != CDS_SUCCESS)
		cds_event ( "cdsRmMbr", flags.fsLocStat);
	timeit();
	outlink();

	free(group);
	free(member);
}

void 
skulk (void)
{
	cds_full_name_t *dirname;
	int ret;

	dirname = (cds_full_name_t *)get_ent("Directory");

	fzero();

	gtime(&start);
	ret = cdsSkulkDir(dirname,	/* directory to skulk */
			  &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
            outlink();
	}
	else cds_event ( "cdsSkulkDir", flags.fsLocStat);

	free(dirname);
}


void 
testatt (void)
{
	cds_full_name_t *entry;
	cds_attr_name_t *attribute;
	cds_attrvalue_t *value;
	int ret;
	int etype;

	entry = (cds_full_name_t *)get_ent("Entry");
	etype = get_num("Type=1:D,2:O,3:CP,4:SL,5:CH,7:{D|O} =");
	attribute = (cds_attr_name_t *)get_ent("Attribute");
	value = get_val("Value");

	fzero();

	gtime(&start);
	ret = cdsTestAttrValue(entry,		/* name of object entry */
			       attribute,	/* attribute name */
			       etype,		/* object entries only */
			       value,		/* attribute value */
			       &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    fprintf(stdout, "A match %s found\n", ret == CDS_SUCCESS ? "was" : "wasn't");
	    if (((ret == CDS_SUCCESS) && (flags.fsLocStat == CDS_SUCCESS_STALE)) ||
		((ret == CDS_NOTFND) && (flags.fsLocStat == CDS_NOTFND_STALE)))
	        fprintf(stdout, "May be stale\n");
	    outlink();
	    }
	else cds_event ( "cdsTestAttrValue", flags.fsLocStat);
	free(entry);
	free(attribute);
}


void 
resname (void)
{
	cds_full_name_t *linkname;
	u_char target[MAX_STRLEN];
	int len, ret;

	linkname = (cds_full_name_t *)get_ent("Link Name");

	fzero();

	gtime(&start);
	len = sizeof(target);
	ret = cdsResolveName(linkname,	/* name of link */
			     (cds_full_name_t *)target,	/* resolved name */
			     &len,	/* buffer len */
			     &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    fprintf(stdout, "Returned name is \"%s\"\n", target);
	    outlink();
	}
	else cds_event ( "cdsResolveName", flags.fsLocStat);
	free(linkname);
}


void 
remrep (void)
{
	cds_full_name_t *directory;
	cds_full_name_t *clearinghouse;
	int ret;

	directory = (cds_full_name_t *)get_ent("Directory");
	clearinghouse = (cds_full_name_t *)get_ent("Clearinghouse");

	gtime(&start);
	ret = cdsRmReplica(directory, clearinghouse, &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    outlink();
	}
	else cds_event ( "cdsRmReplica", flags.fsLocStat);
	free(directory);
	free(clearinghouse);
}


void 
addatt (void)
{
	cds_full_name_t *entry;
	cds_attr_name_t *attribute;
	cds_attrvalue_t *value;
	int type,etype;
	int ret;
	enum cdsAttrType atype;

	entry = (cds_full_name_t *)get_ent("Entry");
	etype = get_num("Type=1:D,2:O,3:CP,4:SL,5:CH,7:{D|O} =");
	attribute = (cds_attr_name_t *)get_ent("Attribute");
	type = get_num("Type");
	value = get_val("Value");

	atype = (type > 0) ? (enum cdsAttrType) type : cdsSet;
	fzero();

	gtime(&start);
	ret = cdsAddAttrValue(entry,		/* entry name */
			      attribute,	/* attribute name */
			      atype,		/* set or single valued */
			      etype,		/* object entries only */
			      value,		/* attribute value */
			      (cds_cts_t *) NULL, /* CTS */
			      &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    outlink();
	}
	else cds_event ( "cdsAddAttrValue", flags.fsLocStat);
	free(entry);
	free(attribute);
}



void 
rmattval (void)
{
	cds_full_name_t *entry;
	cds_attr_name_t *attribute;
	cds_attrvalue_t *value;
	int ret,etype;

	entry = (cds_full_name_t *)get_ent("Entry");
	etype = get_num("Type=1:D,2:O,3:CP,4:SL,5:CH,7:{D|O} =");
	attribute = (cds_attr_name_t *)get_ent("Attribute");
	value = get_val("Value");

	fzero();

	gtime(&start);
	ret = cdsRmAttrValue(entry,		/* entry name */
			     attribute,		/* attribute name */
			     etype,		/* object entries only */
			     value,		/* attribute value */
			     (cds_cts_t *) NULL,/* CTS */
			     &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    outlink();
	}
	else cds_event ( "cdsRmAttrValue", flags.fsLocStat);

	free(entry);
	free(attribute);
}


void 
rmatt (void)
{
	cds_full_name_t *entry;
	cds_attr_name_t *attribute;
	int type;
	int etype,ret;
	enum cdsAttrType atype;

	entry = (cds_full_name_t *)get_ent("Entry");
	etype = get_num("Type=1:D,2:O,3:CP,4:SL,5:CH,7:{D|O} =");
	attribute = (cds_attr_name_t *)get_ent("Attribute");
	type = get_num("Type");

	atype = (type > 0) ? (enum cdsAttrType) type : cdsSet;

	gtime(&start);
	ret = cdsRmAttr (entry,			/* entry name */
			 attribute,		/* attribute name */
			 atype,			/* attribute type */
			 etype, 		/* always object entry */
			 (cds_cts_t *) NULL,	/* CTS */
			 &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    outlink();
	}
	else cds_event ( "cdsRmAttr", flags.fsLocStat);

	free(entry);
	free(attribute);
}


void 
enumchild (void)
{
	cds_full_name_t *dirname;
	cds_simple_name_t *wildcard;
	int len, found_one, status;
	cds_handle_t handle;
	u_char child[1024];

	dirname = (cds_full_name_t *)get_ent("Directory");
	wildcard = (cds_simple_name_t *)get_ent("Wildcard");

	fzero();
	handle = cdsGetHandle();
	if (handle == NULL) {
		fprintf(stderr, "cdsGetHandle returned NULL\n");
		free(dirname);
		free(wildcard);
		return;
	}
	found_one = 0;
	do {
		/*
		 * Get next name
		 */
		len = sizeof(child);
		status = cdsEnumChild(dirname,	/* dir */
				      handle,	/* handle of context info */
				      wildcard,	/* wildcard */
				      (void *) NULL,	/* result */
				      child,	/* string result */
                                      &len,	/* buffer len */
				      &flags);
		if (status == CDS_SUCCESS) {
			fprintf(stdout, "Child name=%s\n", child);
			found_one = 1;
		}
	} while (status == CDS_SUCCESS);

	if (status == CDS_EMPTY) {
	    if ( !found_one ) cds_event ("cdsEnumChild", status);
	    else outlink();
	}
	else cds_event ( "cdsEnumChild", flags.fsLocStat);

	cdsFreeHandle(handle);
	free(dirname);
	free(wildcard);
}


void 
deleteobj (void)
{
	cds_full_name_t *object;
	int ret;

	object = (cds_full_name_t *)get_ent("Object Name");

	fzero();

	gtime(&start);
	ret = cdsDelObj(object, &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    outlink();
	}
	else cds_event ( "cdsDelObj", flags.fsLocStat);

	free(object);
}


void 
creategrp (void)
{
	cds_full_name_t *group;
	int ret;

	group = (cds_full_name_t *)get_ent("Group Name");

	fzero();

	gtime(&start);
	ret = cdsCreateGrp(group, &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    outlink();
	}
	else cds_event ( "cdsCreateGrp", flags.fsLocStat);

	free(group);
}



void 
deletegrp (void)
{
	cds_full_name_t *group;
	int ret;

	group = (cds_full_name_t *)get_ent("Group Name");

	fzero();

	gtime(&start);
	ret = cdsDelGrp(group, &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    outlink();
	}
	else cds_event ( "cdsDelGrp", flags.fsLocStat);

	free(group);
}


void 
deletelink (void)
{
	cds_full_name_t *linkname;
	int ret;

	linkname = (cds_full_name_t *)get_ent("Link Name");

	fzero();

	gtime(&start);
	ret = cdsDelSLink(linkname, &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    outlink();
	}
	else cds_event ( "cdsDelSLink", flags.fsLocStat);

	free(linkname);
}


void 
deletedir (void)
{
	cds_full_name_t *dir;
	int ret;

	dir = (cds_full_name_t *)get_ent("Directory");

	fzero();

	gtime(&start);
	ret = cdsDelDir(dir, &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    outlink();
	}
	else cds_event ( "cdsDelDir", flags.fsLocStat);

	free(dir);
}


void 
createobj (void)
{
	cds_full_name_t *name;
	cds_simple_name_t *class;
	u_char *version;
	char *m;
	int ret;
	struct cdsVersion cvers;

	fzero();
	flags.fsConf = cdsConfHi;

	name = (cds_full_name_t *)get_ent("Object Name");
	class = (cds_simple_name_t *)get_ent("Class");
	/* Optional */
	if (strlen((char *)class) == 0) {
	  gtime(&start);
	  ret = cdsCreateObj(name,		/* object name */
			    NULL,		/* class name */
			    NULL,		/* class version */
			   (cds_id_t *)NULL,     /* object uid */
			   (cds_cts_t *) NULL,	/* CTS */
			   &flags);
	 } else {
	   version = (u_char *)get_ent("Version");

	   cvers.cdsMajVer = atoi(strtok((char *)version, "."));
	   if ((m = strtok((char *) NULL, ".")) != NULL)
	     cvers.cdsMinVer = atoi(m);
	   else
	     cvers.cdsMinVer = 0;

	   gtime(&start);
	   ret = cdsCreateObj(name,		/* object name */
			      class,		/* class name */
			      &cvers,		/* class version */
			      (cds_id_t *)NULL, /* object uid */
			      (cds_cts_t *) NULL,	/* CTS */
			      &flags);
	 }
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    outlink();
	}
	else cds_event ( "cdsCreateObj", flags.fsLocStat);

	free(name);
	free(class);
	free(version);
}


void 
createlink (void)
{
	cds_full_name_t *linkname;
	cds_full_name_t *targetname;
	int ret;
        cds_timeout_t *timeout;

	linkname = (cds_full_name_t *)get_ent("Link Name");
	targetname = (cds_full_name_t *)get_ent("Target Name");
	timeout = get_timeout();
	fzero();

	gtime(&start);
	ret = cdsCreateSLink(linkname,			/* link name */
			     targetname,		/* target name */
			     timeout,           	/* no timeout */
			     (cds_cts_t *) NULL,	/* CTS */
			     &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    outlink();
	}
	else cds_event ( "cdsCreateSLink", flags.fsLocStat);

	free(linkname);
	free(targetname);
}

void 
createdir (void)
{
	cds_full_name_t *directory;
	cds_full_name_t *clearinghouse;
	int length, ret;
	char *cp;
	cds_id_t cts;

	directory = (cds_full_name_t *)get_ent("Directory");
	clearinghouse = (cds_full_name_t *)get_ent("Clearinghouse");
	fzero();

        gtime(&start);
        if (strlen((char *)clearinghouse) == 0)
	    ret = cdsCreateDir(directory,	/* directory name */
			     NULL,
			     &cts,	/* returned CTS */
			     &flags);
	else
	    ret = cdsCreateDir(directory,	/* directory name */
				 clearinghouse, /* clearinghouse name */
				 &cts,	/* returned CTS */
				 &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {

	    timeit();
	    outlink();

	    fprintf(stdout, "UUID returned was: ");
	    cp = (char *) &cts;
	    for (length = 0; length < sizeof cts; length++)
		fprintf(stdout,"%02x", (*cp++)&0xFF);
	    fprintf(stdout,"\n");
	}
	else cds_event ( "cdsCreateDir", flags.fsLocStat);

	free(directory);
	free(clearinghouse);
}


void 
addrep (void)
{
	cds_full_name_t *directory;
	cds_full_name_t *clearinghouse;
	int replicatype;
	int ret;
	enum cdsReplicaType rtype;

	directory = (cds_full_name_t *)get_ent("Directory");
	clearinghouse = (cds_full_name_t *)get_ent("Clearinghouse");
	replicatype = get_num("replicaType");

	rtype = (enum cdsReplicaType) replicatype;
	fzero();

	gtime(&start);
	ret = cdsAddReplica(directory,	/* directory name */
			    clearinghouse,	/* clearinghouse to put it in */
			    rtype,	/* master, secondary, r/o */
			    &flags);
	gtime(&finish);

	if (ret == CDS_SUCCESS) {
	    timeit();
	    outlink();
	}
	else cds_event ( "cdsAddReplica", flags.fsLocStat);

	free(directory);
	free(clearinghouse);
}


void 
enumobj (void)
{
	cds_full_name_t *dirname;
	cds_simple_name_t *wildcard;
	cds_simple_name_t *class;
	int olen, clen, found_one, status, getclass;
	cds_handle_t handle;
	u_char object[MAX_STRLEN];
	u_char objclass[MAX_STRLEN];

	dirname = (cds_full_name_t *)get_ent("Directory");
	wildcard = (cds_simple_name_t *)get_ent("Wildcard");
	class = (cds_simple_name_t *)get_ent("Class");

	fzero();
	handle = cdsGetHandle();
	if (handle == NULL) {
		fprintf(stderr, "cdsGetHandle returned NULL\n");
		free(dirname);
		free(wildcard);
		free(class);
		return;
	}
	found_one = 0;
	do {
		/*
		 * Get next name
		 */
		getclass = 1;
		olen = sizeof(object);
		clen = sizeof(objclass);
		status = cdsEnumObj(dirname,	/* dir */
				    handle,		/* context info */
				    class,	/* class filter */
				    wildcard,	/* opaque wildcard */
				    &getclass,	/* return class too */
				    (cds_simple_name_t **) NULL, /* opq res */
				    object,	/* string result */
                                    &olen,	/* buffer len */
				    (cds_simple_name_t **) NULL, /* opq class */
				    objclass,	/* rtd str class */
                                    &clen,	/* buffer len */
				    &flags);
		if (status == CDS_SUCCESS) {
			if (!getclass)
				strcpy((char *)objclass, "N/A");
			fprintf(stdout,"Object name=\"%s\", class=\"%s\"\n",
				object, objclass);
			found_one = 1;
		}
	} while (status == CDS_SUCCESS);

	if (status == CDS_EMPTY) {
	    if ( !found_one ) cds_event ( "cdsEnumObj", status);
	    else outlink();
	}
	else cds_event ( "cdsEnumObj", flags.fsLocStat);

	cdsFreeHandle(handle);
	free(dirname);
	free(wildcard);
	free(class);
}


void 
enumsoft (void)
{
	cds_full_name_t *dirname;
	cds_simple_name_t *wildcard;
	int len, found_one, status;
	cds_handle_t handle;
	u_char link[MAX_STRLEN];

	dirname = (cds_full_name_t *)get_ent("Directory");
	wildcard = (cds_simple_name_t *)get_ent("Wildcard");

	fzero();
	handle = cdsGetHandle();
	if (handle == NULL) {
		fprintf(stderr, "cdsGetHandle returned NULL\n");
		free(dirname);
		free(wildcard);
		return;
	}
	found_one = 0;
	do {
		/*
		 * Get next name
		 */
		len = sizeof(link);
		status = cdsEnumSLink(dirname,	/* opaque dir */
				      handle,	/* handle of context info */
				      wildcard,	/* opaque wildcard */
				      (void *)NULL,	/* opaque result */
				      link,	/* string result */
				      &len,	/* buffer len */
				      &flags);
		if (status == CDS_SUCCESS) {
			fprintf(stdout, "Link name=\"%s\"\n", link);
			found_one = 1;
		}
	} while (status == CDS_SUCCESS);

	if (status == CDS_EMPTY) {
	    if ( !found_one ) cds_event ( "cdsEnumSLink", status);
	    else outlink();
	}
	else cds_event ( "cdsEnumSLink", flags.fsLocStat);

	cdsFreeHandle(handle);
	free(dirname);
	free(wildcard);
}


void 
enumgroup (void)
{
	cds_full_name_t *dirname;
	cds_simple_name_t *wildcard;
	int len, found_one, status;
	cds_handle_t handle;
	u_char group[MAX_STRLEN];

	dirname = (cds_full_name_t *)get_ent("Directory");
	wildcard = (cds_simple_name_t *)get_ent("Wildcard");

	fzero();
	handle = cdsGetHandle();
	if (handle == NULL) {
		fprintf(stderr, "cdsGetHandle returned NULL\n");
		free(dirname);
		free(wildcard);
		return;
	}
	found_one = 0;
	do {
		/*
		 * Get next name
		 */
		len = sizeof(group);
		status = cdsEnumGrp(dirname,	/* opaque dir */
				    handle,	/* handle of context info */
				    wildcard,	/* opaque wildcard */
				    (void *) NULL,	/* opaque result */
				    group,	/* string result */
				    &len,	/* buffer len */
				    &flags);
		if (status == CDS_SUCCESS) {
			fprintf(stdout,"Group name=\"%s\"\n", group);
			found_one = 1;
		}
	} while (status == CDS_SUCCESS);

	if (status == CDS_EMPTY) {
	    if ( !found_one ) cds_event ( "cdsEnumGrp", status);
	    else outlink();
	}
	else cds_event ( "cdsEnumGrp", flags.fsLocStat);

	cdsFreeHandle(handle);
	free(dirname);
	free(wildcard);
}


void 
enumatt (void)
{
	cds_full_name_t *entry;
	int len, found_one, status;
	enum cdsAttrType atype;
	cds_handle_t handle;
	u_char attribute[MAX_STRLEN];
	int etype;

	entry = (cds_full_name_t *)get_ent("Entry");
	etype = get_num("Type=1:D,2:O,3:CP,4:SL,5:CH,7:{D|O} =");

	fzero();
	handle = cdsGetHandle();
	if (handle == NULL) {
		fprintf(stderr, "cdsGetHandle returned NULL\n");
		free(entry);
		return;
	}
	found_one = 0;
	do {
		/*
		 * Get next name
		 */
		len = sizeof(attribute);
		status = cdsEnumAttr(entry,	/* entry name */
				     handle,	/* handle of context info */
				     etype,	/* object entries only */
				     &atype,	/* returned attr type */
				     (void *)NULL, /* rtd opq name */
				     attribute, /* returned string name */
				     &len,	/* buffer len */
				     &flags);
		if (status == CDS_SUCCESS) {
			fprintf(stdout,"Attribute name=\"%s\", type=%d\n",
					attribute, atype);
			found_one = 1;
		}
	} while (status == CDS_SUCCESS);

	if (status == CDS_EMPTY) {
	    if ( !found_one ) cds_event ( "cdsEnumAttr", status);
	    else outlink();
	}
	else cds_event ( "cdsEnumAttr", flags.fsLocStat);

	cdsFreeHandle(handle);
	free(entry);
}


void 
readatt (void)
{
	cds_full_name_t *entry;
	cds_attr_name_t *attribute;
	int found_one, status;
	cds_handle_t handle;
	cds_attrvalue_t *value;
	char string[100];
	int etype;
	size_t stringL;

	entry = (cds_full_name_t *)get_ent("Entry");
	etype = get_num("Type=1:D,2:O,3:CP,4:SL,5:CH,7:{D|O} =");
	attribute = (cds_attr_name_t *)get_ent("Attribute");

	fzero();
	handle = cdsGetHandle();
	if (handle == NULL) {
		fprintf(stderr, "cdsGetHandle returned NULL\n");
		free(attribute);
		free(entry);
		return;
	}
	found_one = 0;
	do {
		/*
		 * Get an attribute value
		 */
		status = cdsReadAttrValue(entry,/* entry name */
				handle,		/* handle of context info */
				attribute,	/* attribute name */
				etype,		/* object entries only */
				(void *) NULL,	/* no CH */
				&value,		/* returned value */
				(cds_cts_t **) NULL, /* CTS */
				(void *) NULL,	/* CH tower */
				(int *) NULL,	/* tower len */
				&flags);
		if (status == CDS_SUCCESS) {
			fprintf(stdout,"Object value type=%d, value=",
						value->val_syntax);
			switch (value->val_syntax) {
			case cds_long:
				fprintf(stdout,"%d\n", value->val_u.val_long);
				break;
			case cds_byte:
				fprintf(stdout,"\"%.*s\", ",
					value->val_u.val_byte.length,
						value->val_u.val_byte.byte_p);
				fprintf(stdout,"length=%d\n",
						value->val_u.val_byte.length);
				break;
			case cds_char:
				fprintf(stdout,"\"%.*s\", ",
					value->val_u.val_char.length,
						value->val_u.val_char.char_p);
				fprintf(stdout,"length=%d\n",
						value->val_u.val_char.length);
				break;
			case cds_Timeout:
				stringL = sizeof(string);
				status = utc_asclocaltime(string, stringL,
				     &value->val_u.val_timeout.expire);
				fprintf(stdout,"%s ", string);

				stringL = sizeof(string);
				status = utc_ascreltime(string, stringL,
				     &value->val_u.val_timeout.extend);
				fprintf(stdout,"%s\n", string);

			default:
				puts("???");
			}
			if (flags.fsLocStat == CDS_SUCCESS_STALE)
			  fprintf(stdout,"May be stale\n");
			found_one = 1;
		}
	} while (status == CDS_SUCCESS);

	if (status == CDS_EMPTY) {
	    if ( !found_one ) cds_event ( "cdsReadAttrValue", status);
	    else outlink();
	}
	else cds_event ( "cdsReadAttrValue", flags.fsLocStat);

	cdsFreeHandle(handle);
	free(attribute);
	free(entry);
}


void 
readmember (void)
{
	cds_full_name_t *group;
	int len, found_one, status;
	cds_handle_t handle;
	u_char member[MAX_STRLEN];
	enum cdsGrpMbrType mtype;

	group = (cds_full_name_t *)get_ent("Group");

	fzero();
	handle = cdsGetHandle();
	if (handle == NULL) {
		fprintf(stderr, "cdsGetHandle returned NULL\n");
		free(group);
		return;
	}
	found_one = 0;
	do {
		/*
		 * Get a group member
		 */
		len = sizeof(member);
		status = cdsReadMbr(group,	/* group name */
				    handle,	/* handle of context info */
				    (void *) NULL, /* retd. opq. member name */
				    &mtype,	/* retd. member type */
				    member,	/* retd. string name */
				    &len,	/* buffer len */
				    &flags);
		if (status == CDS_SUCCESS) {
			fprintf(stdout,"Member name=\"%s\"", member);
			if (mtype == cdsGroup)
				fprintf(stdout," group\n");
			else
				fprintf(stdout," principal\n");
			found_one = 1;
		}
	} while (status == CDS_SUCCESS);

	if (status == CDS_EMPTY) {
	    if ( !found_one ) cds_event ( "cdsReadMbr", status);
	    else outlink();
	}
	else cds_event ( "cdsReadMbr", flags.fsLocStat);

	cdsFreeHandle(handle);
	free(group);
}

cds_full_name_t 
*cds_full2str (cds_full_name_t *opqname)
{
	static unsigned char strname[1024];
	int len = sizeof strname;

	(void) cdsCvtFullToStr(opqname, strname, &len);
	return((cds_full_name_t *)strname);
}


char 
*strip_left (cds_full_name_t *fname)
{
	int rlen, slen;
	cds_full_name_t rname;
	cds_simple_name_t sname;
	static unsigned char strname[CDS_STR_SNAME_MAX];

	rlen = sizeof(rname);
	slen = sizeof(sname);
	(void) cdsScatter(fname, 0,
			  (cds_full_name_t *)NULL, (int *)NULL,
			  &sname, &slen,
			  &rname, &rlen);
	memcpy((char *)fname, (char *)&rname, rlen);
	slen = sizeof(strname);
	(void) cdsCvtSimpleToStr(&sname, strname, &slen);
	return((char *)strname);
}



void 
cdsname (void)
{
	u_char *name;
	int status, flen, i, nnames;
	cds_full_name_t fname;

	name = (u_char *)get_ent("Fullname");
	flen = sizeof(fname);
	status = cdsCvtFullToOpq(name, &fname, &flen);
	if (status != CDS_SUCCESS)
	    cds_event ( "cdsCvtFullToOpq", status);
	else
	    fprintf(stdout,"Successfully converted \"%s\"\n", cds_full2str(&fname));
	nnames = cdsCountSimple(&fname);
	fprintf(stdout,"%d simple name(s):\n", nnames);
	for (i = 1; i <= nnames; i++)
		fprintf(stdout,"\t%d: \"%s\"\n", i, strip_left(&fname));
}

void 
gdsname (void)
{
	u_char *name;
	int status, flen, i, nnames;
	cds_full_name_t fname;

	name = (u_char *)get_ent("Fullname");
	flen = sizeof(fname);
	status = dns_CvtCDSFullToGDS(name, 1, 0, &fname, &flen);
	if (status != CDS_SUCCESS)
	    cds_event ("dns_CvtCDSFullToGDS", status);
	else
	    fprintf(stdout,"Successfully converted \"%s\"\n", cds_full2str(&fname));
	nnames = cdsCountSimple(&fname);
	fprintf(stdout,"%d simple name(s):\n", nnames);
	for (i = 1; i <= nnames; i++)
		fprintf(stdout,"\t%d: \"%s\"\n", i, strip_left(&fname));

	status = dns_CvtCDSFullToGDS(name, 0, 0, &fname, &flen);
	if (status != CDS_SUCCESS)
	    cds_event ( "dns_CvtCDSFullToGDS", status);
	else
	    fprintf(stdout,"Successfully converted \"%s\"\n", cds_full2str(&fname));
	nnames = cdsCountSimple(&fname);
	fprintf(stdout,"%d simple name(s):\n", nnames);
	for (i = 1; i <= nnames; i++)
		fprintf(stdout,"\t%d: \"%s\"\n", i, strip_left(&fname));
}


void 
strtotower (void)
{
    char *binding_p[2], **binding_p_out;
    char *towerset_p;
    int status;

    binding_p[0] = get_ent("Tower");
    binding_p[1] = NULL;

    status = cdsCvtStrToTower(binding_p, &towerset_p);
    if (status!=CDS_SUCCESS)
	fprintf(stdout,"cdsCvtStrToTower failed, status %d\n", status);
    else {
	status = cdsCvtTowerToStr(towerset_p, &binding_p_out);
	if (status!=CDS_SUCCESS)
	    fprintf(stdout,"cdsCvtStrToTower failed, status %d\n", status);
	else
	    fprintf(stdout,"Converted string is %s\n", *binding_p_out);
    }
}
