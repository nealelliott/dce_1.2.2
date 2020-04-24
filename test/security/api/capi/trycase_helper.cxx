/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: trycase_helper.cxx,v $
 * Revision 1.1.2.1  1996/11/14  17:13:54  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/14  15:57:17  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/04  22:33 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  22:53 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 * 
 * 	Initial revision
 * 	[1996/10/15  19:28:36  pkdev  1.1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:45 UTC  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:46 UTC  arvind  /main/arvind_capi/1]
 * 
 * $EndLog$
 */
/*
 * @DEC_COPYRIGHT@
 * Copyright (c) 1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 * 
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 * 
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 * 
 * FACILITY:    Certification API	
 * 
 * ABSTRACT:	Helper functions for hierarhical/registry test programs. 
 * 
 *	
 * ENVIRONMENT:	portable
 * 
 * AUTHOR:
 *              Mahesh B. Girkar 
 *		Distributed Processing Engineering (DPE)
 * 
 * DATE:	March 1996
 * 
 */




extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <dce/utc.h>
#include <string.h>

#ifdef ENVIRONMENT_TET
#include <tet_api.h>
#endif

}



#include "common_defines_a.h"

/*
 * find_next_non_blank
 *
 * returns pointer within start to next non-blank char
 */

char *find_next_non_blank (char *start)
{
  char *p = 0;
  if (!start)
    return 0;
 
  p = start;
  while (*p == ' ') p++;  /* skip any preceding blanks */
  if (*p == '\0') {
    return 0;
  }

  return p;
}

/*
 * line_is_empty
 *
 * returns 1 if line is empty; 0 otherwise
 */


int line_is_empty (char *line)
{
  int is_empty = 1;

  while (*line != '\0') {
    if (*line != ' ') {
      is_empty = 0;
      break;
    }
    line++;
  }

  return (is_empty);
}

/*
 * find_next_blank
 *
 * returns pointer within start to next blank char
 */

char *find_next_blank (char *start)
{
  char *p = 0;
  if (!start)
    return 0;
 
  p = start;
  while ((*p != ' ') && (*p != '\0')) p++;  /* skip any preceding blanks */
  if (*p == '\0') return 0;

  return p;
}

/*
 * find_next_blank_or_char
 *
 * returns pointer within start to next blank char or given char
 */

char *find_next_blank_or_char (char *start, char c)
{
  char *p = 0;
  if (!start)
    return 0;
 
  p = start;
  while ((*p != ' ') && (*p != c)) p++;  /* skip any preceding blanks */

  return p;
}

int get_loc_and_princ_attr_value (char *tok,
                         char *loc,
                         char *value)
{
  char *p = 0, *q = 0;
  char line[BUFSIZE]; 

  if (!tok)
    return -1;

  strcpy(line, tok);
  p = &line[0];


  p = find_next_non_blank(&line[0]);
  if (!p) return -1;
  q=p;
  p = find_next_blank (p);
  if (!p) return -1;
  *p = '\0';
  strcpy(loc, q);



  p++;
  p = find_next_non_blank(p);
  if (!p) return -1;
  q = p;
  p = find_next_blank_or_char (p, '\0');
  if (!p) return -1;
  *p = '\0';
  strcpy(value, q);


  return 0;
}


/*
 * get_cds_init_fnames
 */

int get_cds_init_fnames (char *tok,
                         char *config_fname,
                         char *cr_fname,
                         char *delete_fname,
                         char *princ_fname)
{
  char *p = 0, *q = 0;
  char line[BUFSIZE]; 

  if (!tok)
    return -1;

  strcpy(line, tok);
  p = &line[0];


  p = find_next_non_blank(&line[0]);
  if (!p) return -1;
  q=p;
  p = find_next_blank (p);
  if (!p) return -1;
  *p = '\0';
  strcpy(config_fname, q);

  p++;
  p = find_next_non_blank(p);
  if (!p) return -1;
  q = p;
  p = find_next_blank (p);
  if (!p) return -1;
  *p = '\0';
  strcpy(cr_fname, q);


  p++;
  p = find_next_non_blank(p);
  if (!p) return -1;
  q = p;
  p = find_next_blank (p);
  if (!p) return -1;
  *p = '\0';
  strcpy(delete_fname, q);


  p++;
  p = find_next_non_blank(p);
  if (!p) return -1;
  q = p;
  p = find_next_blank_or_char (p, '\0');
  if (!p) return -1;
  *p = '\0';
  strcpy(princ_fname, q);

  return 0;
}
          

/*
 * strip end of line 
 */

void strip_end_of_line (char *p)
{
  if (!p)
    return;

 if (*(p+strlen(p)-1) == '\n')  /* if last is a lin-feed, nuke it */
   *(p+strlen(p)-1) = '\0';
 
 return;
}



/* 
 * return_right_most_rdn
 * returns a pointer to the last rdn in  a name
 */

#if 0
char *return_right_most_rdn (char *princ) 
{
  int count;
  char *t = 0;

  if (!princ)
    return 0;

  count = strlen(princ), tcount = 0; 
  t = princ + count;
  while ((tcount < count) && (*t != '/')) {
    t--; tcount++;
  }

  if (tcount != count)
    t++;
  else
    return 0;

  return t;
}
#endif


/*
 * parse_token: 
 * name is of the form {"X500"/"CDS":/.../a=b/c=d...}
 * memory for  act_name is assumed to be allocated by caller
 */


int parse_token (char *name, int &x500_flag, char * act_name)
{
  char *p = 0;
  char buf[BUFSIZE];

  if (!name) 
    return -1;

  if (strlen (name) > BUFSIZE-1)
    return -1;

  strcpy (buf, name);

  p = &buf[0];
  while ((*p != ':') && (*p != '\0')) p++;
  if (*p == '\0') 
    return -1;

  *p = '\0';
  if (process_x500_cds_flag(&buf[0], x500_flag) != 0)
    return -1;

  p++;
  strcpy (act_name, p);
  return 0;
}

/*
 * process_x500_cds_flag: returns 0 if buf is either XDS_STR or CDS_STR;
 * otherwise, returns 0 and x500_flag is either 1/0;
 */


int process_x500_cds_flag(char *buf, int &x500_flag)
{
  if (!buf) 
    return -1;
  
  if (strcmp(buf, X500_STR) == 0) {
    x500_flag = X500_FLAG;
    return 0;
  }
  if (strcmp (buf, CDS_STR) == 0) {
    x500_flag = CDS_FLAG;
    return 0;
  }

  return -1;
}


/*
 * parse_revoked_cert_token
 * tok is of the form "i_j_k_l.cert/date"
 *
 */

int parse_revoked_cert_token (char *tok, char *cert_fname, char *date)
{
  char *p;
  char buf[BUFSIZE];

  strcpy (buf, tok);
  p = &buf[0];
  
  p = find_next_blank_or_char (p, '/');
  if (!p) return -1;
  *p = '\0';
  p++;
  if (*p == '\0') return -1;
  strcpy (cert_fname, buf);
  strcpy (date, p);
  return 0;
}


/*
 * parse_ca_crl_token
 *  tok is of the form "i_j.pubkey/date/date"
 *
 */

int parse_ca_crl_token(char *tok, char *ca_crl_pubkey, char *thisupdate,
                        char *nextupdate)
{
  char *p;
  char *q ;
  char buf[BUFSIZE];

  strcpy (buf, tok);
  p = &buf[0];

  p = find_next_blank_or_char (p, '/');
  if (!p) return -1;
  *p = '\0';
  /* buf now points to i_j.pubkey */

   p++;
  if (*p == '\0') return -1;
  q = p;
  p = find_next_blank_or_char (p, '/');
  if (!p) return -1;
  *p = '\0';
  /* q now points to first date */

  p++;
  if (*p == '\0') return -1;
  /* p now points to next date */

  strcpy (thisupdate, q);
  strcpy (nextupdate, p);
  strcpy (ca_crl_pubkey, buf);
  return 0; 
}

/*
 * parse_cert_loc_token
 * tok is of the form  {{"cacert"/"usercert"/"crosscert"/"userrevoke"/"carevoke"}:
 *                       certfname:{"X500"/"CDS"}:/.../a=b/c=d...}
 *
 * it is assumed that caller is responsible for allocating storate for
 * certfname, and loc
 */



int parse_cert_loc_token (char *tok, int &attr_flag, char * certfname,
                 int &xds_flag, char *loc)
{
 char buf[BUFSIZE], *p = 0, *q = 0, *t1 = 0, *t2 = 0, *t3 = 0;
 int i = 0, done = 0;

 if (!tok)
   return -1;

  if (strlen (tok) > BUFSIZE-1)
    return -1;
  strcpy (buf, tok);

  p = &buf[0];
  q = &buf[0];
  i = 0;
  for (;;) {

    if (*p == '\0') 
      return -1;

    if (*p == ':') {
      *p = '\0';  
      switch (i) {
        case 0:
          t1 = q;  /* t1 points to cacert/usercert/crosscert etc */
          break;
        case 1:
          t2 = q;  /* t2 points to certfname */
          break;
        case 2:
          t3 = q;  /* t3 points to x500/cds */
          done = 1;
          break;
        default:
          return -1;
      } /* end of switch */

      p++;
      q = p;   /* q points to next part of token */
      if (done)  /* found the third ':' In this case, q points to the fourth part of the token or location */
        break;
      i++;

    } else {  /* if (*p != ':') */
      p++;
    }
 }  /* end of for */

 if (process_attr_flag(t1, attr_flag) != 0)
   return -1;

 if (process_x500_cds_flag(t3, xds_flag) != 0)
   return -1;
 
 strcpy (certfname, t2);
 strcpy (loc, q);
 return 0;
}

/*
 * process_attr_flag
 */


int process_attr_flag(char *t1, int &attr_flag)
{
  if (!t1)
    return -1;

  if (strcmp (t1, USER_CERT_OID_STR) == 0) {
    attr_flag = USER_CERT_OID_FLAG;
    return 0;
  }

  if (strcmp (t1, CA_CERT_OID_STR) == 0) {
    attr_flag = CA_CERT_OID_FLAG;
    return 0;
  }

  if (strcmp (t1, CROSS_CERT_OID_STR) == 0) {
    attr_flag = CROSS_CERT_OID_FLAG;
    return 0;
  }

  if (strcmp (t1, USER_REVOKE_OID_STR) == 0) {
    attr_flag = USER_REVOKE_OID_FLAG;
    return 0;
  }

  if (strcmp (t1, CA_REVOKE_OID_STR) == 0) {
    attr_flag = CA_REVOKE_OID_FLAG;
    return 0;
  }

  return -1;
}

/*
 * parse_test_case_token
 * tok is of the form  "{keycount/keydata}/{exp_status}/{extra_args/.."
 */


int parse_test_case_token(char *tok, int & test_case)
{
  char buf[BUFSIZE];
  char *p, *q;

  strcpy (buf, tok);
  p = &buf[0];
  p = find_next_non_blank(p);
  if (!p) return -1;
  q = p;
  p = find_next_blank_or_char (buf, '/');
  if (!p)
    return -1;

  *p = '\0';
  if (strcmp (q, TEST_KEY_COUNT_STR) == 0) {
    test_case = TEST_KEY_COUNT_FLAG;
    return 0;
  }
  if (strcmp (q, TEST_KEY_DATA_STR) == 0) {
    test_case = TEST_KEY_DATA_FLAG;
    return 0;
  }
  if (strcmp (q, TEST_KEY_TRUST_INFO_STR) == 0) {
    test_case = TEST_KEY_TRUST_INFO_FLAG;
    return 0;
  }

  if (strcmp (q, TEST_KEY_CERT_COUNT_STR) == 0) {
    test_case = TEST_KEY_CERT_COUNT_FLAG;
    return 0;
  }

  if (strcmp (q, TEST_KEY_CERT_INFO_STR) == 0) {
    test_case = TEST_KEY_CERT_INFO_FLAG;
    return 0;
  }

  if (strcmp (q, TEST_KEY_CERT_ALL_COUNT_STR) == 0) {
    test_case = TEST_KEY_CERT_ALL_COUNT_FLAG;
    return 0;
  }

  return -1;
}

/*
 * parse_test_case_token_key_count
 * returns expected_status & expected_key_count
 */


int parse_test_case_token_key_count(char *tok, 
                            int & expected_status, 
                            int &expected_key_count)
{
  char buf[BUFSIZE];
  char *p, *q;

  strcpy (buf, tok);
  p = &buf[0];

  /* ignore TEST_KEY.. string */
  p = find_next_non_blank(p);
  if (!p) return -1;
  p = find_next_blank_or_char (buf, '/');
  if (!p)
    return 0;

  *p = '\0';
  p++;
  
  
  /* find string for expected status */
  p = find_next_non_blank(p);
  if (!p) return -1;
  q = p;
  p = find_next_blank_or_char (p,'/');
  if (!p) return -1;

  *p = '\0';
  p++;
  
  /* p now points to expected key_count */

  sscanf(q, "%ld", &expected_status);
  if (expected_status == 0)
    sscanf(p, "%ld", &expected_key_count);
  return 0;  
}

int parse_test_case_token_key_data(char *tok, 
                            int & expected_status, 
                            int &key_index)
{
  char buf[BUFSIZE];
  char *p, *q;

  strcpy (buf, tok);
  p = &buf[0];

  /* ignore TEST_KEY.. string */
  p = find_next_non_blank(p);
  if (!p) return -1;
  p = find_next_blank_or_char (buf, '/');
  if (!p)
    return 0;

  *p = '\0';
  p++;
  
  
  /* find string for expected status */
  p = find_next_non_blank(p);
  if (!p) return -1;
  q = p;
  p = find_next_blank_or_char (p,'/');
  if (!p) return -1;

  *p = '\0';
  p++;
  
  /* p now points to expected key_count */

  sscanf(q, "%ld", &expected_status);
  sscanf(p, "%ld", &key_index);
  return 0;  
}

/*
 * parse_test_case_token_cert_count_info
 * This is a first pass of the routine that parses a testtoken for
 * test case pkc_get_key_certification_info
 *
 * We need to add a lot more parts to this token
 */

int parse_test_case_token_key_cert_count(char *tok, 
                            int & expected_status, 
                            int &key_index, int &ca_count)
{
  char *p, *tok1, *tok2, *tok3, *tok4;
  char buf[BUFSIZE];

  strcpy (buf, tok);
  p = &buf[0];
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';
  tok1 = &buf[0];

  p++;
  tok2 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';


  p++;
  tok3 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';

  sscanf (tok2, "%ld",  &expected_status);
  sscanf (tok3, "%ld", &key_index);

  if (expected_status != 0)  /* don't parse further, if expected status is no */
    return 0;


  p++;
  tok4 = p;
  if (*p == '\0') return -1;


  sscanf (tok4, "%ld", &ca_count);

  return 0;
}

/*
 * parse_test_case_token_key_cert_info
 * This is a first pass of the routine that parses a testtoken for
 * test case pkc_get_key_certification_info
 *
 * We need to add a lot more parts to this token
 */

int parse_test_case_token_key_cert_info(char *tok, 
                            int & expected_status, 
                            int &key_index, int &cert_index,
                            utc_t *certstart, utc_t *certexp,
                            utc_t *crllastseen, utc_t *nextcrlexpected, 
                            char *certname)
{
  char *p, *tok1, *tok2, *tok3, *tok4, *tok5, *tok6, *tok7, *tok8, *tok9;
  char buf[BUFSIZE];
  unsigned32 status;

  strcpy (buf, tok);
  p = &buf[0];
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';
  tok1 = &buf[0];     /* tok1 is testcase token string */

  p++;
  tok2 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';            /* tok2 is expected status */


  p++;
  tok3 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';

  p++;
  tok4 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';

  sscanf (tok2, "%ld",  &expected_status);
  sscanf (tok3, "%ld", &key_index);
  sscanf (tok4, "%ld", &cert_index);

  if (expected_status != 0)  /* don't parse further, if expected_status is no zero */
    return 0;

  p++;
  tok5 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';


  p++;
  tok6 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';

  p++;
  tok7 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';


  p++;
  tok8 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';


  p++;
  tok9 = p;
  if (*p == '\0') return -1;


  status = utc_mkasctime(certstart, tok5);
  if (status != 0) return -1;
  status = utc_mkasctime(certexp, tok6);
  if (status != 0) return -1;
  status = utc_mkasctime(crllastseen, tok7);
  if (status != 0) return -1;
  status = utc_mkasctime(nextcrlexpected, tok8);
  if (status != 0) return -1;
  strcpy (certname, tok9);

  return 0;
}
  
/*
 * parse_test_case_token_key_trust_info
 * tok is of the form "trustinfo/expected_status/keyindex/m_crl/m_revoked/trusttype" 
 */

int parse_test_case_token_key_trust_info(char *tok, 
                            int & expected_status, 
                            int &key_index, int &m_crl,
                            int & m_revoked, int &trust_type)
{
  char *p, *tok1, *tok2, *tok3, *tok4, *tok5, *tok6;
  char buf[BUFSIZE];

  strcpy (buf, tok);
  p = &buf[0];
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';
  tok1 = &buf[0];

  p++;
  tok2 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';


  p++;
  tok3 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';
  
  sscanf(tok2, "%ld", &expected_status);
  sscanf(tok3, "%ld", &key_index);
  if (expected_status != 0) return 0;

  p++;
  tok4 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';

  p++;
  tok5 = p;
  p = find_next_blank_or_char (p, '/'); 
  if (!p) return -1;
  *p = '\0';

  p++;
  if (*p == '\0') return -1;
  tok6 = p;


  sscanf(tok4, "%ld", &m_crl);
  sscanf(tok5, "%ld", &m_revoked);
  sscanf(tok6, "%ld", &trust_type);

  return 0;
}

int read_principals ()
{
  FILE *fp;
  char filename[BUFSIZE];
  char line [BUFSIZE], x500_name[BUFSIZE], *p = 0;
  char  *q;
  int i = 0, x500_flag;


  strcpy (filename, TEST_DIR);
  strcat (filename, "/");
  strcat (filename, PRINCIPAL_FILE);

  fp = fopen (filename, "rb");
  if (!fp)
    return -1;

  while (fgets (line, BUFSIZE, fp) != 0) {
    strip_end_of_line (line);
    p = find_next_non_blank(line);
    if (p)
      i++;
  }

  fclose (fp);

  total_no_of_principals = i;
  princ_key_arr = (struct principal_and_key_info *) 
           malloc (i* (sizeof (struct principal_and_key_info)));

  if (!princ_key_arr)
    return -1;


  fp = fopen (filename, "rb");
  if (!fp)
    return -1;

  i = 0;
  while (fgets (line, BUFSIZE, fp) != 0) {
    strip_end_of_line (line);
    p = find_next_non_blank(line);
    if (p) {
      q = p;
      p = find_next_blank_or_char(line,'\0');
      if (!p) {
        fclose (fp);
        return -1;
      }

      *p = '\0';


      if (parse_token (q, x500_flag,  x500_name) != 0) {
        fclose (fp);
        return -1;
      }

      princ_key_arr[i].principal = 
                   (char *) malloc (strlen (x500_name) + 1);
      strcpy (princ_key_arr[i].principal,x500_name);

      i++;
    }
  } /* while fgets */

  fclose (fp);
  return 0;
}
  




int read_principals_and_keys ()
{
  FILE *fp;
  char filename[BUFSIZE];
  char line [BUFSIZE], x500_name[BUFSIZE], *p = 0;
  int i = 0, j = 0, x500_flag;
  char  *q;

  strcpy (filename, TEST_DIR);
  strcat (filename, "/");
  strcat (filename, NO_OF_KEYS_FILE);

  keys_per_principal = 1;
  fp = fopen (filename, "rb");
  if (fp) {
    if (fgets (line, BUFSIZE, fp)) {
      strip_end_of_line (line);
      p = find_next_non_blank(line);
      if (p) {
        sscanf(line, "%ld", &keys_per_principal);
      }
    }
    fclose (fp);
  }
    

  strcpy (filename, TEST_DIR);
  strcat (filename, "/");
  strcat (filename, PRINCIPAL_FILE);

  fp = fopen (filename, "rb");
  if (!fp)
    return -1;

  while (fgets (line, BUFSIZE, fp) != 0) {
    strip_end_of_line (line);
    p = find_next_non_blank(line);
    if (p)
      i++;
  }

  fclose (fp);

  total_no_of_principals = i;

  princ_key_arr = (struct principal_and_key_info *) 
           malloc (i* keys_per_principal*(sizeof (struct principal_and_key_info)));

  if (!princ_key_arr)
    return -1;


  fp = fopen (filename, "rb");
  if (!fp)
    return -1;

  i = 0;
  while (fgets (line, BUFSIZE, fp) != 0) {
    strip_end_of_line (line);
    p = find_next_non_blank(line);
    if (p) {
      q = p;
      p = find_next_blank_or_char(line,'\0');
      if (!p) {
        fclose (fp);
        return -1;
      }

      *p = '\0';


      if (parse_token (q, x500_flag, x500_name) != 0) {
        fclose (fp);
        return -1;
      }

      for (j = 0; j < keys_per_principal; j++) {
        princ_key_arr[i*keys_per_principal +j].principal = 
                     (char *) malloc (strlen (x500_name) + 1);
        princ_key_arr[i*keys_per_principal +j].cell = 0;
        strcpy (princ_key_arr[i].principal,x500_name);
      }

      i++;
    }
  } /* while fgets */

  fclose (fp);
  return 0;
}

/*
 *find_princ_index
 */

int   find_princ_index(char *princ)
{
  int i;
    
  for (i = 0; i < total_no_of_principals; i++)
    if (strcmp (princ, princ_key_arr[i].principal) == 0)
      return i;

  return -1;
}

/*
 * find_princ_given_index
 */

int find_princ_given_index (int index, char *princ)
{
  if (index < 0 && index >= total_no_of_principals)
    return -1;

  strcpy (princ, princ_key_arr[index].principal);
  return 0;
}

/*
 * read bytes
 * len is initially size of octetstring_value
 * on return, it is actual bytes read.
 */

int read_bytes_from_file (char *filename,  char *octetstring_value, size_t & len)
{
  FILE *file; int c;
  size_t os_length;

  if (!filename)
    return -1;

  file = fopen (filename, "rb");
  if (!file)
    return -1;

  os_length = 0;
  while  ((c = fgetc(file)) != EOF) {
    octetstring_value[os_length] = (char) c;
    os_length++;
    if (os_length > len) {
      fclose (file);
      return -1;
    };
  }
  len = os_length;
  return 0;
}  /* read_bytes_from_file */


  
/*
 * compare_keydata
 */

int compare_keydata(unsigned char *keydata,
                    size_t keylength, 
                    int princ_index, 
                    int key_index,
                    unsigned char *octetstring_value, 
                    size_t & len)
{
  FILE *fp;
  char filename[BUFSIZE];
  char line [BUFSIZE];
  int i = 0;
  unsigned char keybuf[5000];

  if (princ_index < 0 && princ_index  >= total_no_of_principals)
    return -1;

  strcpy (filename, KEYS_DIR);
  strcat (filename, "/");
  sprintf(line, "%d_%d.pubkey",princ_index, key_index);
  strcat (filename, line);

  if (read_bytes_from_file (filename,  (char *) octetstring_value,  len) != 0)
    return -1;

  if (len != keylength) {
    return -1;
  }

  if (memcmp (keydata, octetstring_value, len) != 0) {
    return -1;
  }
   
  return 0;

}  /* compare_data */

/*
 * parse_princ_key_index
 * q is of the form i_j, where i is princ index and j is key index 
 */


int parse_princ_key_index(char *q, char *x500_user)
{
  char buf[BUFSIZE];
  char *p;
  int index, i;
  
  strcpy (&buf[0], q);
  i = 0;
  while ((buf[i] != '_') && (buf[i] != '\0')) i++;
  if (buf[i] == '\0')
    return -1; 
  buf[i] = '\0';

  sscanf (buf, "%ld", &index);
   
  return (find_princ_given_index (index, x500_user));
}


//
// print_general_1,print_general_2,etc are just 
// tet-aware printf routines.
// There should really be a one routine which uses a va-args
// but for now, this will do.
//


void print_general_1(char *s)
{

#ifdef ENVIRONMENT_TET
  tet_infoline (s);
#else
  printf(s);
#endif
 return;
}

void print_general_2(char *s, unsigned32 st)
{
  char line[5000];
#ifdef ENVIRONMENT_TET
  sprintf(line, s, st);
  tet_infoline (line);
#else
  printf(s, st);
#endif
 return;
}

void print_general_3(char *s, const char *s1)
{
  char line[5000];
#ifdef ENVIRONMENT_TET
  sprintf(line, s, s1);
  tet_infoline (line);
#else
  printf(s, s1);
#endif
 return;
}

void print_general_4(char *s, unsigned32 st, unsigned32 st1)
{
  char line[5000];
#ifdef ENVIRONMENT_TET
  sprintf(line, s, st, st1);
  tet_infoline (line);
#else
  printf(s, st, st1);
#endif
 return;
}
