/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: createmsg.c,v $
 * Revision 1.1.10.2  1996/02/18  18:19:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:04  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:58:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:03  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:47:37  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:41:23  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:58:03  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:59:40  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:38:22  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:28:16  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  19:41:45  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:50:05  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:39:25  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: createmsg.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:19:17 $";
#endif

/****************************************************************************/
/*                                                                          */
/*               TESTPROGRAM: IPC - MESSAGE GENERATOR                       */
/*               ====================================                       */
/*                                                                          */
/****************************************************************************/

/**************	system-includes				 ****************/

#ifdef STOP_TIME
#include <sys/types.h>
#include <sys/timeb.h>
#endif /* STOP_TIME */
#include <stdio.h>

/************** DS-external includes                     ****************/

#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d23apdu.h>

#define NO_CRKIND   4       /* no. of possible input for cred. kind        */
#define NO_SUBSET   3       /* no. of possible input for subset            */
#define NO_RETVAL   2       /* no. of possible input for req. return value */
#define NO_MODTYP   5       /* no. of possible input for modification type */
#define NO_OPMODE   4       /* no. of possible input for operation mode    */
#define NO_MATCH    6       /* no. of possible input for matching rule     */
#define NO_SUBSPOS  3       /* no. of possible input for substring position*/
#define NO_AT_TYPE 10       /* no. of possible input for attribute type    */
			    /* with DN-syntax                              */


void mk_comarg_msg() ;
void mk_filter_msg() ;

/************** global functions  (import)                    ****************/
IMPORT  int     d27_302_str_oid();

/************** global variables  (import)                    ****************/

IMPORT  FILE       *fdin;
IMPORT  char       *eptr;
IMPORT  D2_a_type  *oid;

char   *crkinds[] = { "NO_CRED", "SIMPLE_CRED", "STRONG_CRED", "EXTERNAL_CRED" } ;
short   crkindc[] = { D23_NO_CRED, D23_SIMPLE_CRED, D23_STRONG_CRED, D23_EXTERNAL_CRED } ;
char   *subsets[] = { "BASE_OBJECT", "ONE_LEVEL", "WHOLE_SUBTREE" } ;
short   subsetc[] = { D2_BASE_OBJECT, D2_ONE_LEVEL, D2_WHOLE_SUBTREE } ;
char   *retvals[] = { "TYP", "VAL_TYP" } ;
short   retvalc[] = { D2_R_TYP, D2_R_VAL_TYP } ;
char   *modtyps[] = { "REP_AV", "ADDATT", "ADD_AV", "DELATT", "DEL_AV" } ;
short   modtypc[] = { D2_REP_AV, D2_ADDATT, D2_ADD_AV, D2_DELATT, D2_DEL_AV } ;
char   *opmodes[] = { "AND", "OR", "NOT", "ITEM" } ;
short   opmodec[] = { D2_AND, D2_OR, D2_NOT, D2_ITEM } ;
char   *matches[] = { "EQUAL", "APPROX", "SUBSTRINGS", "PRESENT", "GTEQ", "LTEQ" } ;
short   matchec[] = { D2_EQUAL, D2_APPROX_MATCH, D2_SUBSTRINGS, D2_PRESENT, D2_GTEQ, D2_LTEQ } ;
char   *subspos[] = { "INITIAL", "ANY", "FINAL" } ;
short   subspoc[] = { D2_INITIAL, D2_ANY, D2_FINAL } ;
char   *at_type[] = { "85.4.1", "85.4.31", "85.4.32", "85.4.33", "85.4.34", "85.4.10001", "85.4.10002", "85.4.10014", "85.4.10016", "85.4.10017" } ;

/* ------------------------- mk_bind_msg ------------------------------------------ */

EXPORT void mk_bind_msg(dir_id)
Ushort dir_id;
{
    char    str_in[20] ;
    Bool    found ;
    int     j, novalues ;
    char    next[D2_DNL_MAX];
    char    *p;
    int     res;
    int     len;

    /* supplying of struct D23_inbind */
    ((D23_inbind *) eptr)->d23_Aversno = D23_APDUV22 ;
    ((D23_inbind *) eptr)->d23_Adirid = dir_id ;
    if (fscanf (fdin, "%s", str_in) != 1) {
	    fprintf (stdout, "error on fscanf -> credential kind\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    for (j=0, found=D2_FALSE; j<NO_CRKIND; j++) {
	    if (strcmp (str_in, crkinds[j]) == 0) {
		    ((D23_inbind *) eptr)->d23_Acrkind = crkindc[j] ;
		    found = D2_TRUE ;
	    }
    }
    if (found != D2_TRUE) {
	    fprintf (stdout, "error:  credential kind -> %s\n", str_in) ;
	    fflush (stdout) ;
	    exit () ;
    }

    switch (((D23_inbind *) eptr)->d23_Acrkind) {
    case D23_NO_CRED:
	    eptr += D23_S_INB ;
	    break;
    case D23_SIMPLE_CRED:
	    eptr += D23_S_INB ;
	    /* supplying of struct D23_simcr_info */
	    ((D23_simcr_info *) eptr)->d23_scrtag = D23_SIMTAG ;
	    eptr += D23_S_SIMCR ;

	    if (fscanf (fdin, "%s", next) != 1) {
		    fprintf (stdout, "error on fscanf -> next\n") ;
		    fflush (stdout) ;
		    exit () ;
	    }
	    if (strcmp(next, "Validity:") == 0) {
		    /* supplying of struct D23_val_info */
		    ((D23_val_info *) eptr)->d23_valtag = D23_VALTAG ;
		    eptr += D23_S_VAL ;
		    if (fscanf (fdin, "%s", next) != 1) {
			    fprintf (stdout, "error on fscanf -> next\n") ;
			    fflush (stdout) ;
			    exit () ;
		    }

		    if (strcmp(next, "Time1:") == 0) {
			    /* supplying of struct D23_tm_info */
			    if (fscanf (fdin, "%s", (D23_tm_info *) eptr + 1) != 1) {
				    fprintf (stdout, "error on fscanf -> tm info\n") ;
				    fflush (stdout) ;
				    exit () ;
			    }
			    ((D23_tm_info *) eptr)->d23_tmtag = D23_TIMTAG ;
			    ((D23_tm_info *) eptr)->d23_tmlen = strlen ((D23_tm_info *) eptr + 1) ;
			    ((D23_tm_info *) eptr)->d23_tmoff = D2_ALIGN (((D23_tm_info *) eptr)->d23_tmlen) ;
			    eptr += D23_S_TM + ((D23_tm_info *) eptr)->d23_tmoff ;
			    if (fscanf (fdin, "%s", next) != 1) {
				    fprintf (stdout, "error on fscanf -> next\n") ;
				    fflush (stdout) ;
				    exit () ;
			    }
		    }

		    if (strcmp(next, "Time2:") == 0) {
			    /* supplying of struct D23_tm_info */
			    if (fscanf (fdin, "%s", (D23_tm_info *) eptr + 1) != 1) {
				    fprintf (stdout, "error on fscanf -> tm info\n") ;
				    fflush (stdout) ;
				    exit () ;
			    }
			    ((D23_tm_info *) eptr)->d23_tmtag = D23_TIMTAG ;
			    ((D23_tm_info *) eptr)->d23_tmlen = strlen ((D23_tm_info *) eptr + 1) ;
			    ((D23_tm_info *) eptr)->d23_tmoff = D2_ALIGN (((D23_tm_info *) eptr)->d23_tmlen) ;
			    eptr += D23_S_TM + ((D23_tm_info *) eptr)->d23_tmoff ;
			    if (fscanf (fdin, "%s", next) != 1) {
				    fprintf (stdout, "error on fscanf -> next\n") ;
				    fflush (stdout) ;
				    exit () ;
			    }
		    }

		    if (strcmp(next, "Random1:") == 0) {
			    /* supplying of struct D23_bit_info */
			    if (fscanf (fdin, "%s", (D23_bit_info *) eptr + 1) != 1) {
				    fprintf (stdout, "error on fscanf -> bit info\n") ;
				    fflush (stdout) ;
				    exit () ;
			    }
			    ((D23_bit_info *) eptr)->d23_btag = D23_BITTAG ;
			    ((D23_bit_info *) eptr)->d23_blen = strlen ((D23_bit_info *) eptr + 1) ;
			    ((D23_bit_info *) eptr)->d23_boff = D2_ALIGN (((D23_bit_info *) eptr)->d23_blen) ;
			    eptr += D23_S_BIT + ((D23_bit_info *) eptr)->d23_boff ;
			    if (fscanf (fdin, "%s", next) != 1) {
				    fprintf (stdout, "error on fscanf -> next\n") ;
				    fflush (stdout) ;
				    exit () ;
			    }
		    }
		    if (strcmp(next, "Random2:") == 0) {
			    /* supplying of struct D23_bit_info */
			    if (fscanf (fdin, "%s", (D23_bit_info *) eptr + 1) != 1) {
				    fprintf (stdout, "error on fscanf -> bit info\n") ;
				    fflush (stdout) ;
				    exit () ;
			    }
			    ((D23_bit_info *) eptr)->d23_btag = D23_BITTAG ;
			    ((D23_bit_info *) eptr)->d23_blen = strlen ((D23_bit_info *) eptr + 1) ;
			    ((D23_bit_info *) eptr)->d23_boff = D2_ALIGN (((D23_bit_info *) eptr)->d23_blen) ;
			    eptr += D23_S_BIT + ((D23_bit_info *) eptr)->d23_boff ;
			    if (fscanf (fdin, "%s", next) != 1) {
				    fprintf (stdout, "error on fscanf -> next\n") ;
				    fflush (stdout) ;
				    exit () ;
			    }
		    }
	    }

	    /* supplying of struct D23_ds_name, D23_pw_info */
	    ((D23_ds_name *) eptr)->d23_dstag = D23_NAMTAG ;
	    if (strcmp (next, "anonym") != 0) {
		    strcpy((D23_ds_name *) eptr + 1, next) ;
		    ((D23_ds_name *) eptr)->d23_dslen = strlen ((D23_ds_name *) eptr + 1) + 1 ;
		    ((D23_ds_name *) eptr)->d23_dsoff = D2_ALIGN (((D23_ds_name *) eptr)->d23_dslen) ;
		    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;
		    if (fscanf (fdin, "%s", (D23_pw_info *) eptr + 1) != 1) {
			    fprintf (stdout, "error on fscanf -> user password\n") ;
			    fflush (stdout) ;
			    exit () ;
		    }
		    ((D23_pw_info *) eptr)->d23_pwtag = D23_PWDTAG ;
		    ((D23_pw_info *) eptr)->d23_pwlen = strlen ((D23_pw_info *) eptr + 1) ;
		    ((D23_pw_info *) eptr)->d23_pwoff = D2_ALIGN (((D23_pw_info *) eptr)->d23_pwlen) ;
		    eptr += D23_S_PW + ((D23_pw_info *) eptr)->d23_pwoff ;
	    } else {
		    ((D23_ds_name *) eptr)->d23_dslen =
		    ((D23_ds_name *) eptr)->d23_dsoff = 0 ;
		    eptr += D23_S_NM ;
		    ((D23_pw_info *) eptr)->d23_pwtag = D23_PWDTAG ;
		    ((D23_pw_info *) eptr)->d23_pwoff =
		    ((D23_pw_info *) eptr)->d23_pwlen = 0 ;
		    eptr += D23_S_PW ;
	    }
	    break;
    case D23_STRONG_CRED:
	    eptr += D23_S_INB ;
	    /* supplying of struct D23_strcr_info */
	    ((D23_strcr_info *) eptr)->d23_strtag = D23_STRTAG ;
	    eptr += D23_S_SSCR ;
	    break;
    case D23_EXTERNAL_CRED:
	    eptr += D23_S_INB ;
	    /* supplying of struct D23_extcr_info */
	    ((D23_extcr_info *) eptr)->d23_exttag = D23_EXTTAG ;
	    eptr += D23_S_EXTCR ;
	    break;
    default:
	    fprintf (stdout, "credential kind: wrong input\n") ;
	    fflush (stdout) ;
	    exit () ;
    }

    /* supplying of struct D23_av_value */
    if (fscanf (fdin, "%s", next) != 1) {
	    fprintf (stdout, "error on fscanf -> attribute value\n") ;
	    fflush (stdout) ;
	    exit () ;
    }

    if (next[0] != '#')
    {    strcpy((D23_av_value *) eptr + 1, next);
	 ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1) + 1;
    } else {
	 p = eptr + D23_S_ATV;
	 for (len=0, j=1; j<strlen(next); j+=2, len++)
	 {    sscanf(&next[j],"%2x",&res);
	      *p++ = (char) res;
	 }
	 ((D23_av_value *) eptr)->d23_avlen = len;
    }
    ((D23_av_value *) eptr)->d23_avoff = D2_ALIGN (((D23_av_value *) eptr)->d23_avlen) ;
    eptr += D23_S_ATV + ((D23_av_value *) eptr)->d23_avoff ;

    /* supplying of struct D23_eom */
    ((D23_eom *) eptr)->d23_eomtag = D23_EOMTAG ;
    eptr += D23_S_EOM ;
}

/* ------------------------- mk_add_obj_msg --------------------------------------- */

EXPORT void mk_add_obj_msg (dir_id)
Ushort   dir_id ;
{
    int     noattr, novalues ;
    char    str_attr[D2_OBJ_IDL_MAX];
    char    str_in[D2_DNL_MAX];
    char    *p;
    int     res;
    int     j;
    int     len;
    Bool    dn_found;

    /* supplying of struct D23_inaddobjt */
    ((D23_inaddobjt *) eptr)->d23_Mhdr.d23_versno = D23_APDUV22 ;
    ((D23_inaddobjt *) eptr)->d23_Mhdr.d23_dirid = dir_id ;
    if (fscanf (fdin, "%d", &((D23_inaddobjt *) eptr)->d23_Mhdr.d23_bindid) != 1) {
	    fprintf (stdout, "error on fscanf -> bind id\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    eptr += D23_S_INADD ;

    /* supplying of struct D23_com_arg, D23_se_param, */
    /* D23_ds_name (DN of requestor) , D23_ext_info  */
    mk_comarg_msg() ;

    /* supplying of struct D23_ds_name (DN of new object) */
    if (fscanf (fdin, "%s", (D23_ds_name *) eptr + 1) != 1) {
	    fprintf (stdout, "error on fscanf -> DN of new object\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_ds_name *) eptr)->d23_dstag = D23_NAMTAG ;
    ((D23_ds_name *) eptr)->d23_dslen = strlen ((D23_ds_name *) eptr + 1) + 1 ;
    ((D23_ds_name *) eptr)->d23_dsoff = D2_ALIGN (((D23_ds_name *) eptr)->d23_dslen) ;
    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;

    if (fscanf (fdin, "%d", &noattr) != 1) {
	    fprintf (stdout, "error on fscanf -> no. of attributes\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    while (noattr-- > 0) {
	    /* supplying of struct D23_at_type */
	    if (fscanf (fdin, "%s", str_attr) != 1) {
		    fprintf (stdout, "error on fscanf -> attribute type\n") ;
		    fflush (stdout) ;
		    exit () ;
	    }
	    d27_302_str_oid(str_attr, oid);
	    memcpy((Octet *) (eptr + D23_S_ATT), oid->d2_type, oid->d2_typ_len);
	    ((D23_at_type *) eptr)->d23_atttag = D23_ATTTAG ;
	    ((D23_at_type *) eptr)->d23_atlen = oid->d2_typ_len;
	    ((D23_at_type *) eptr)->d23_atoff = D2_ALIGN (((D23_at_type *) eptr)->d23_atlen) ;
	    eptr += D23_S_ATT + ((D23_at_type *) eptr)->d23_atoff ;

	    /* supplying of struct D23_av_info */
	    ((D23_av_info *) eptr)->d23_avtag = D23_AVTAG ;
	    if (fscanf (fdin, "%d", &((D23_av_info *) eptr)->d23_asyntx) != 1) {
		    fprintf (stdout, "error on fscanf -> attribute syntax\n") ;
		    fflush (stdout) ;
		    exit () ;
	    }
	    if (fscanf (fdin, "%d", &novalues) != 1) {
		    fprintf (stdout, "error on fscanf -> no. of attribute values\n") ;
		    fflush (stdout) ;
		    exit () ;
	    }
	    ((D23_av_info *) eptr)->d23_avnum = novalues ;
	    eptr += D23_S_AV ;

	    while (novalues-- > 0) {
		    /* supplying of struct D23_av_value */
		    if (fscanf (fdin, "%s", str_in) != 1) {
			    fprintf (stdout, "error on fscanf -> attribute value\n") ;
			    fflush (stdout) ;
			    exit () ;
		    }
		    if (str_in[0] != '#')
		    {    strcpy((D23_av_value *) eptr + 1, str_in);
			 for (j=0, dn_found=D2_FALSE; j<NO_AT_TYPE; j++)
			 {       if (strcmp (str_attr, at_type[j]) == 0)
				 {     dn_found = D2_TRUE ;
				 }
			 }
			 if (dn_found == D2_TRUE)
			 {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1) + 1;
			 } else
			 {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1);
			 }
		    } else {
			 p = eptr + D23_S_ATV;
			 for (len=0, j=1; j<strlen(str_in); j+=2, len++)
			 {    sscanf(&str_in[j],"%2x",&res);
			      *p++ = (char) res;
			 }
			 ((D23_av_value *) eptr)->d23_avlen = len;
		    }
		    ((D23_av_value *) eptr)->d23_avoff = D2_ALIGN (((D23_av_value *) eptr)->d23_avlen) ;
		    eptr += D23_S_ATV + ((D23_av_value *) eptr)->d23_avoff ;
	    }
    }

    /* supplying of struct D23_eom */
    ((D23_eom *) eptr)->d23_eomtag = D23_EOMTAG ;
    eptr += D23_S_EOM ;
}

/* ------------------------- mk_rem_obj_msg --------------------------------------- */

EXPORT void mk_rem_obj_msg (dir_id)
Ushort   dir_id ;
{
    /* supplying of struct D23_inremobjt */
    ((D23_inremobjt *) eptr)->d23_Phdr.d23_versno = D23_APDUV22 ;
    ((D23_inremobjt *) eptr)->d23_Phdr.d23_dirid = dir_id ;
    if (fscanf (fdin, "%d", &((D23_inremobjt *) eptr)->d23_Phdr.d23_bindid) != 1) {
	    fprintf (stdout, "error on fscanf -> bind id\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    eptr += D23_S_REMOBJ ;

    /* supplying of struct D23_com_arg, D23_se_param, */
    /* D23_ds_name (DN of requestor) , D23_ext_info  */
    mk_comarg_msg() ;

    /* supplying of struct D23_ds_name (DN of object) */
    if (fscanf (fdin, "%s", (D23_ds_name *) eptr + 1) != 1) {
	    fprintf (stdout, "error on fscanf -> DN of object which should be removed\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_ds_name *) eptr)->d23_dstag = D23_NAMTAG ;
    ((D23_ds_name *) eptr)->d23_dslen = strlen ((D23_ds_name *) eptr + 1) + 1 ;
    ((D23_ds_name *) eptr)->d23_dsoff = D2_ALIGN (((D23_ds_name *) eptr)->d23_dslen) ;
    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;

    /* supplying of struct D23_eom */
    ((D23_eom *) eptr)->d23_eomtag = D23_EOMTAG ;
    eptr += D23_S_EOM ;
}

/* ------------------------- mk_read_msg --------------------------------------- */

EXPORT void mk_read_msg (dir_id)
Ushort   dir_id ;
{
    char    str_in[20] ;
    char    str_attr[D2_OBJ_IDL_MAX];
    Bool    found ;
    int     j, noattr ;

    /* supplying of struct D23_inread */
    ((D23_inread *) eptr)->d23_Ehdr.d23_versno = D23_APDUV22 ;
    ((D23_inread *) eptr)->d23_Ehdr.d23_dirid = dir_id ;
    if (fscanf (fdin, "%d", &((D23_inread *) eptr)->d23_Ehdr.d23_bindid) != 1) {
	    fprintf (stdout, "error on fscanf -> bind id\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    eptr += D23_S_INR ;

    /* supplying of struct D23_com_arg, D23_se_param, */
    /* D23_ds_name (DN of requestor) , D23_ext_info  */
    mk_comarg_msg() ;

    /* supplying of struct D23_ds_name (DN of object) */
    if (fscanf (fdin, "%s", (D23_ds_name *) eptr + 1) != 1) {
	    fprintf (stdout, "error on fscanf -> DN of object which should be read\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_ds_name *) eptr)->d23_dstag = D23_NAMTAG ;
    ((D23_ds_name *) eptr)->d23_dslen = strlen ((D23_ds_name *) eptr + 1) + 1 ;
    ((D23_ds_name *) eptr)->d23_dsoff = D2_ALIGN (((D23_ds_name *) eptr)->d23_dslen) ;
    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;

    /* supplying of struct D23_rq_info */
    ((D23_rq_info *) eptr)->d23_rqtag = D23_REQTAG ;
    if (fscanf (fdin, "%d", &noattr) != 1) {
	    fprintf (stdout, "error on fscanf -> no. of attributes\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_rq_info *) eptr)->d23_rqnbr = noattr ;
    if (fscanf (fdin, "%s", str_in) != 1) {
	    fprintf (stdout, "error on fscanf -> requested return value\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    for (j=0, found=D2_FALSE; j<NO_RETVAL; j++) {
	    if (strcmp (str_in, retvals[j]) == 0) {
		    ((D23_rq_info *) eptr)->d23_rqreq = retvalc[j] ;
		    found = D2_TRUE ;
	    }
    }
    if (found != D2_TRUE) {
	    fprintf (stdout, "error:  requested return value -> %s\n", str_in) ;
	    fflush (stdout) ;
	    exit () ;
    }
    eptr += D23_S_REQ ;
    if (noattr > 0) {
	    while (noattr-- > 0) {
		    /* supplying of struct D23_at_type */
		    if (fscanf (fdin, "%s", str_attr) != 1) {
			    fprintf (stdout, "error on fscanf -> attribute type\n") ;
			    fflush (stdout) ;
			    exit () ;
		    }
		    d27_302_str_oid(str_attr, oid);
		    memcpy((Octet *) (eptr + D23_S_ATT), oid->d2_type, oid->d2_typ_len);
		    ((D23_at_type *) eptr)->d23_atttag = D23_ATTTAG ;
		    ((D23_at_type *) eptr)->d23_atlen = oid->d2_typ_len;
		    ((D23_at_type *) eptr)->d23_atoff = D2_ALIGN (((D23_at_type *) eptr)->d23_atlen) ;
		    eptr += D23_S_ATT + ((D23_at_type *) eptr)->d23_atoff ;
	    }
    }

    /* supplying of struct D23_eom */
    ((D23_eom *) eptr)->d23_eomtag = D23_EOMTAG ;
    eptr += D23_S_EOM ;
}

/* ------------------------- mk_list_msg ------------------------------------------ */

EXPORT void mk_list_msg(dir_id)
Ushort dir_id;
{

    char    str_dn[D2_DNL_MAX];

    /* supplying of struct D23_inlist */
    ((D23_inlist *) eptr)->d23_Ghdr.d23_versno = D23_APDUV22 ;
    ((D23_inlist *) eptr)->d23_Ghdr.d23_dirid = dir_id ;
    if (fscanf (fdin, "%d", &((D23_inlist *) eptr)->d23_Ghdr.d23_bindid) != 1) {
	    fprintf (stdout, "error on fscanf -> bind id\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    eptr += D23_S_INL ;

    /* supplying of struct D23_com_arg, D23_se_param, */
    /* D23_ds_name (DN of requestor) , D23_ext_info  */
    mk_comarg_msg() ;

    /* supplying of struct D23_ds_name  */
    if (fscanf (fdin, "%s", str_dn) != 1) {
	    fprintf (stdout, "error on fscanf -> DN \n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_ds_name *) eptr)->d23_dstag = D23_NAMTAG ;

    if (strcmp (str_dn, "root") != 0) {
	  strcpy((D23_ds_name *) eptr + 1, str_dn);
	  ((D23_ds_name *) eptr)->d23_dslen = strlen ((D23_ds_name *) eptr + 1) + 1 ;
	  ((D23_ds_name *) eptr)->d23_dsoff = D2_ALIGN (((D23_ds_name *) eptr)->d23_dslen) ;
	  eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;
     } else {
	  ((D23_ds_name *) eptr)->d23_dslen =
	  ((D23_ds_name *) eptr)->d23_dsoff = 0 ;
	  eptr += D23_S_NM ;
    }

    /* supplying of struct D23_eom  */
    ((D23_eom *) eptr)->d23_eomtag = D23_EOMTAG ;
    eptr += D23_S_EOM ;
}

/* ------------------------- mk_compare_msg --------------------------------------- */

EXPORT void mk_compare_msg (dir_id)
Ushort   dir_id ;
{
    int     novalues ;
    char    str_attr[D2_OBJ_IDL_MAX];
    char    str_in[D2_DNL_MAX];
    char    *p;
    int     res;
    int     j;
    int     len;
    Bool    dn_found;

    /* supplying of struct D23_incompare */
    ((D23_incompare *) eptr)->d23_Khdr.d23_versno = D23_APDUV22 ;
    ((D23_incompare *) eptr)->d23_Khdr.d23_dirid = dir_id ;
    if (fscanf (fdin, "%d", &((D23_incompare *) eptr)->d23_Khdr.d23_bindid) != 1) {
	    fprintf (stdout, "error on fscanf -> bind id\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    eptr += D23_S_INCOMP ;

    /* supplying of struct D23_com_arg, D23_se_param, */
    /* D23_ds_name (DN of requestor) , D23_ext_info  */
    mk_comarg_msg() ;

    /* supplying of struct D23_ds_name (DN of object) */
    if (fscanf (fdin, "%s", (D23_ds_name *) eptr + 1) != 1) {
	    fprintf (stdout, "error on fscanf -> DN of object which should be compared\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_ds_name *) eptr)->d23_dstag = D23_NAMTAG ;
    ((D23_ds_name *) eptr)->d23_dslen = strlen ((D23_ds_name *) eptr + 1) + 1 ;
    ((D23_ds_name *) eptr)->d23_dsoff = D2_ALIGN (((D23_ds_name *) eptr)->d23_dslen) ;
    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;

    /* supplying of struct D23_at_type */
    if (fscanf (fdin, "%s", str_attr) != 1) {
	    fprintf (stdout, "error on fscanf -> attribute type\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    d27_302_str_oid(str_attr, oid);
    memcpy((Octet *) (eptr + D23_S_ATT), oid->d2_type, oid->d2_typ_len);
    ((D23_at_type *) eptr)->d23_atttag = D23_ATTTAG ;
    ((D23_at_type *) eptr)->d23_atlen = oid->d2_typ_len;
    ((D23_at_type *) eptr)->d23_atoff = D2_ALIGN (((D23_at_type *) eptr)->d23_atlen) ;
    eptr += D23_S_ATT + ((D23_at_type *) eptr)->d23_atoff ;

    /* supplying of struct D23_av_info */
    ((D23_av_info *) eptr)->d23_avtag = D23_AVTAG ;
    if (fscanf (fdin, "%d", &((D23_av_info *) eptr)->d23_asyntx) != 1) {
	    fprintf (stdout, "error on fscanf -> attribute syntax\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_av_info *) eptr)->d23_avnum = 1 ;
    eptr += D23_S_AV ;

    /* supplying of struct D23_av_value */
    if (fscanf (fdin, "%s", str_in) != 1) {
	    fprintf (stdout, "error on fscanf -> attribute value\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    if (str_in[0] != '#')
    {    strcpy((D23_av_value *) eptr + 1, str_in);
	 for (j=0, dn_found=D2_FALSE; j<NO_AT_TYPE; j++)
	 {       if (strcmp (str_attr, at_type[j]) == 0)
		 {     dn_found = D2_TRUE ;
		 }
	 }
	 if (dn_found == D2_TRUE)
	 {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1) + 1;
	 } else
	 {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1);
	 }
    } else {
	 p = eptr + D23_S_ATV;
	 for (len=0, j=1; j<strlen(str_in); j+=2, len++)
	 {    sscanf(&str_in[j],"%2x",&res);
	      *p++ = (char) res;
	 }
	 ((D23_av_value *) eptr)->d23_avlen = len;
    }
    ((D23_av_value *) eptr)->d23_avoff = D2_ALIGN (((D23_av_value *) eptr)->d23_avlen) ;
    eptr += D23_S_ATV + ((D23_av_value *) eptr)->d23_avoff ;

    /* supplying of struct D23_eom */
    ((D23_eom *) eptr)->d23_eomtag = D23_EOMTAG ;
    eptr += D23_S_EOM ;
}

/* ------------------------- mk_search_msg --------------------------------------- */

EXPORT void mk_search_msg (dir_id)
Ushort   dir_id ;
{
    char    str_in[20] ;
    char    str_attr[D2_OBJ_IDL_MAX];
    char    str_dn[D2_DNL_MAX];
    Bool    found ;
    int     j, noattr ;

    /* supplying of struct D23_insearch */
    ((D23_insearch *) eptr)->d23_Ihdr.d23_versno = D23_APDUV22 ;
    ((D23_insearch *) eptr)->d23_Ihdr.d23_dirid = dir_id ;
    if (fscanf (fdin, "%d", &((D23_insearch *) eptr)->d23_Ihdr.d23_bindid) != 1) {
	    fprintf (stdout, "error on fscanf -> bind id\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    eptr += D23_S_INSRCH ;

    /* supplying of struct D23_com_arg, D23_se_param, */
    /* D23_ds_name (DN of requestor) , D23_ext_info  */
    mk_comarg_msg() ;

    /* supplying of struct D23_srcharg */
    ((D23_srcharg *) eptr)->d23_Itag = D23_ITAG ;
    ((D23_srcharg *) eptr)->d23_Iali = 0 ;
    if (fscanf (fdin, "%s", str_in) != 1) {
	    fprintf (stdout, "error on fscanf -> base object subset\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    for (j=0, found=D2_FALSE; j<NO_SUBSET; j++) {
	    if (strcmp (str_in, subsets[j]) == 0) {
		    ((D23_srcharg *) eptr)->d23_Iobjsub = subsetc[j] ;
		    found = D2_TRUE ;
	    }
    }
    if (found != D2_TRUE) {
	    fprintf (stdout, "error:  base object subset -> %s\n", str_in) ;
	    fflush (stdout) ;
	    exit () ;
    }
    eptr += D23_S_SCHARG ;

    /* supplying of struct D23_ds_name (DN of base object) */
    if (fscanf (fdin, "%s", str_dn) != 1) {
	    fprintf (stdout, "error on fscanf -> DN \n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_ds_name *) eptr)->d23_dstag = D23_NAMTAG ;

    if (strcmp (str_dn, "root") != 0) {
	  strcpy((D23_ds_name *) eptr + 1, str_dn);
	  ((D23_ds_name *) eptr)->d23_dslen = strlen ((D23_ds_name *) eptr + 1) + 1 ;
	  ((D23_ds_name *) eptr)->d23_dsoff = D2_ALIGN (((D23_ds_name *) eptr)->d23_dslen) ;
	  eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;
     } else {
	  ((D23_ds_name *) eptr)->d23_dslen =
	  ((D23_ds_name *) eptr)->d23_dsoff = 0 ;
	  eptr += D23_S_NM ;
    }

    /* supplying of struct D23_rq_info */
    ((D23_rq_info *) eptr)->d23_rqtag = D23_REQTAG ;
    if (fscanf (fdin, "%d", &noattr) != 1) {
	    fprintf (stdout, "error on fscanf -> no. of attributes\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_rq_info *) eptr)->d23_rqnbr = noattr ;
    if (fscanf (fdin, "%s", str_in) != 1) {
	    fprintf (stdout, "error on fscanf -> requested return value\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    for (j=0, found=D2_FALSE; j<NO_RETVAL; j++) {
	    if (strcmp (str_in, retvals[j]) == 0) {
		    ((D23_rq_info *) eptr)->d23_rqreq = retvalc[j] ;
		    found = D2_TRUE ;
	    }
    }
    if (found != D2_TRUE) {
	    fprintf (stdout, "error:  requested return value -> %s\n", str_in) ;
	    fflush (stdout) ;
	    exit () ;
    }
    eptr += D23_S_REQ ;
    if (noattr > 0) {
	    while (noattr-- > 0) {
		    /* supplying of struct D23_at_type */
		    if (fscanf (fdin, "%s", str_attr) != 1) {
			    fprintf (stdout, "error on fscanf -> attribute type\n") ;
			    fflush (stdout) ;
			    exit () ;
		    }
		    d27_302_str_oid(str_attr, oid);
		    memcpy((Octet *) (eptr + D23_S_ATT), oid->d2_type, oid->d2_typ_len);
		    ((D23_at_type *) eptr)->d23_atttag = D23_ATTTAG ;
		    ((D23_at_type *) eptr)->d23_atlen = oid->d2_typ_len;
		    ((D23_at_type *) eptr)->d23_atoff = D2_ALIGN (((D23_at_type *) eptr)->d23_atlen) ;
		    eptr += D23_S_ATT + ((D23_at_type *) eptr)->d23_atoff ;
	    }
    }

    mk_filter_msg () ;

    /* supplying of struct D23_eom */
    ((D23_eom *) eptr)->d23_eomtag = D23_EOMTAG ;
    eptr += D23_S_EOM ;
}

/* ------------------------- mk_repl_rdn_msg -------------------------------------- */

EXPORT void mk_repl_rdn_msg(dir_id)
Ushort dir_id;
{
    /* supplying of struct D23_in_replrdn */
    ((D23_in_replrdn *) eptr)->d23_Ohdr.d23_versno = D23_APDUV22 ;
    ((D23_in_replrdn *) eptr)->d23_Ohdr.d23_dirid = dir_id ;
    if (fscanf (fdin, "%d", &((D23_in_replrdn *) eptr)->d23_Ohdr.d23_bindid) != 1) {
	    fprintf (stdout, "error on fscanf -> bind id\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    eptr += D23_S_RPRDN ;

    /* supplying of struct D23_com_arg, D23_se_param, */
    /* D23_ds_name (DN of requestor) , D23_ext_info  */
    mk_comarg_msg() ;

    /* supplying of struct D23_repinfo */
    ((D23_repinfo *) eptr)->d23_reptag = D23_REPTAG ;
    ((D23_repinfo *) eptr)->d23_delordn = D2_FALSE ;
    eptr += D23_S_REP ;

    /* supplying of struct D23_ds_name (DN of object) */
    if (fscanf (fdin, "%s", (D23_ds_name *) eptr + 1) != 1) {
	    fprintf (stdout, "error on fscanf -> DN of object\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_ds_name *) eptr)->d23_dstag = D23_NAMTAG ;
    ((D23_ds_name *) eptr)->d23_dslen = strlen ((D23_ds_name *) eptr + 1) + 1 ;
    ((D23_ds_name *) eptr)->d23_dsoff = D2_ALIGN (((D23_ds_name *) eptr)->d23_dslen) ;
    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;

    /* supplying of struct D23_ds_name (DN of new object) */
    if (fscanf (fdin, "%s", (D23_ds_name *) eptr + 1) != 1) {
	    fprintf (stdout, "error on fscanf -> DN of new object\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_ds_name *) eptr)->d23_dstag = D23_NAMTAG ;
    ((D23_ds_name *) eptr)->d23_dslen = strlen ((D23_ds_name *) eptr + 1) + 1 ;
    ((D23_ds_name *) eptr)->d23_dsoff = D2_ALIGN (((D23_ds_name *) eptr)->d23_dslen) ;
    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;

    /* supplying of struct D23_eom */
    ((D23_eom *)eptr)->d23_eomtag = D23_EOMTAG;
    eptr += D23_S_EOM ;
}

/* ------------------------- mk_mod_msg ------------------------------------------- */

EXPORT void mk_mod_msg(dir_id)
Ushort dir_id;
{
    char        str_in[D2_DNL_MAX] ;
    char        str_attr[D2_OBJ_IDL_MAX];
    Bool        found, dn_found ;
    int         j, noattr, novalues ;
    D2_attr_mod modtype ;
    char    *p;
    int     res;
    int     len;

    /* supplying of struct D23_inmodentr */
    ((D23_inmodentr *) eptr)->d23_Nhdr.d23_versno = D23_APDUV22 ;
    ((D23_inmodentr *) eptr)->d23_Nhdr.d23_dirid = dir_id ;
    if (fscanf (fdin, "%d", &((D23_inmodentr *) eptr)->d23_Nhdr.d23_bindid) != 1) {
	    fprintf (stdout, "error on fscanf -> bind id\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    eptr += D23_S_INMOD ;

    /* supplying of struct D23_com_arg, D23_se_param, */
    /* D23_ds_name (DN of requestor) , D23_ext_info  */
    mk_comarg_msg() ;

    /* supplying of struct D23_ds_name (DN of object) */
    if (fscanf (fdin, "%s", (D23_ds_name *) eptr + 1) != 1) {
	    fprintf (stdout, "error on fscanf -> DN of object which should be modified\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_ds_name *) eptr)->d23_dstag = D23_NAMTAG ;
    ((D23_ds_name *) eptr)->d23_dslen = strlen ((D23_ds_name *) eptr + 1) + 1 ;
    ((D23_ds_name *) eptr)->d23_dsoff = D2_ALIGN (((D23_ds_name *) eptr)->d23_dslen) ;
    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;

    /* supplying of struct D23_md_info */
    ((D23_md_info *)eptr)->d23_mdtag = D23_MODTAG;
    if (fscanf (fdin, "%s", str_in) != 1) {
	    fprintf (stdout, "error on fscanf -> type of modification\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    for (j=0, found=D2_FALSE; j<NO_MODTYP; j++) {
	    if (strcmp (str_in, modtyps[j]) == 0) {
		    ((D23_md_info *)eptr)->d23_mdmod = modtypc[j] ;
		    found = D2_TRUE ;
	    }
    }
    if (found != D2_TRUE) {
	    fprintf (stdout, "error:  type of modification -> %s\n", str_in) ;
	    fflush (stdout) ;
	    exit () ;
    }
    modtype = ((D23_md_info *)eptr)->d23_mdmod ;
    eptr += D23_S_MOD ;

    switch (modtype) {
    case D2_REP_AV :
	    /* supplying of struct D23_at_type */
	    if (fscanf (fdin, "%s", str_attr) != 1) {
		    fprintf (stdout, "error on fscanf -> attribute type\n") ;
		    fflush (stdout) ;
		    exit () ;
	    }
	    d27_302_str_oid(str_attr, oid);
	    memcpy((Octet *) (eptr + D23_S_ATT), oid->d2_type, oid->d2_typ_len);
	    ((D23_at_type *) eptr)->d23_atttag = D23_ATTTAG ;
	    ((D23_at_type *) eptr)->d23_atlen = oid->d2_typ_len;
	    ((D23_at_type *) eptr)->d23_atoff = D2_ALIGN (((D23_at_type *) eptr)->d23_atlen) ;
	    eptr += D23_S_ATT + ((D23_at_type *) eptr)->d23_atoff ;

	    /* supplying of struct D23_av_info */
	    ((D23_av_info *) eptr)->d23_avtag = D23_AVTAG ;
	    ((D23_av_info *) eptr)->d23_asyntx = D2_DISTNAME ;
	    ((D23_av_info *) eptr)->d23_avnum = 1 ;
	    eptr += D23_S_AV ;

	    /* supplying of struct D23_av_value (old value) */
	    if (fscanf (fdin, "%s", str_in) != 1) {
		    fprintf (stdout, "error on fscanf -> old attribute value\n") ;
		    fflush (stdout) ;
		    exit () ;
	    }
	    if (str_in[0] != '#')
	    {    strcpy((D23_av_value *) eptr + 1, str_in);
		 for (j=0, dn_found=D2_FALSE; j<NO_AT_TYPE; j++)
		 {       if (strcmp (str_attr, at_type[j]) == 0)
			 {     dn_found = D2_TRUE ;
			 }
		 }
		 if (dn_found == D2_TRUE)
		 {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1) + 1;
		 } else
		 {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1);
		 }
	    } else {
		 p = eptr + D23_S_ATV;
		 for (len=0, j=1; j<strlen(str_in); j+=2, len++)
		 {    sscanf(&str_in[j],"%2x",&res);
		      *p++ = (char) res;
		 }
		 ((D23_av_value *) eptr)->d23_avlen = len;
	    }
	    ((D23_av_value *) eptr)->d23_avoff = D2_ALIGN (((D23_av_value *) eptr)->d23_avlen) ;
	    eptr += D23_S_ATV + ((D23_av_value *) eptr)->d23_avoff ;

	    /* supplying of struct D23_av_info */
	    ((D23_av_info *) eptr)->d23_avtag = D23_AVTAG ;
	    ((D23_av_info *) eptr)->d23_asyntx = D2_DISTNAME ;
	    ((D23_av_info *) eptr)->d23_avnum = 1 ;
	    eptr += D23_S_AV ;

	    /* supplying of struct D23_av_value (new value) */
	    if (fscanf (fdin, "%s", str_in) != 1) {
		    fprintf (stdout, "error on fscanf -> old attribute value\n") ;
		    fflush (stdout) ;
		    exit () ;
	    }
	    if (str_in[0] != '#')
	    {    strcpy((D23_av_value *) eptr + 1, str_in);
		 for (j=0, dn_found=D2_FALSE; j<NO_AT_TYPE; j++)
		 {       if (strcmp (str_attr, at_type[j]) == 0)
			 {     dn_found = D2_TRUE ;
			 }
		 }
		 if (dn_found == D2_TRUE)
		 {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1) + 1;
		 } else
		 {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1);
		 }
	    } else {
		 p = eptr + D23_S_ATV;
		 for (len=0, j=1; j<strlen(str_in); j+=2, len++)
		 {    sscanf(&str_in[j],"%2x",&res);
		      *p++ = (char) res;
		 }
		 ((D23_av_value *) eptr)->d23_avlen = len;
	    }
	    ((D23_av_value *) eptr)->d23_avoff = D2_ALIGN (((D23_av_value *) eptr)->d23_avlen) ;
	    eptr += D23_S_ATV + ((D23_av_value *) eptr)->d23_avoff ;
	    break ;
    case D2_ADDATT :
	    if (fscanf (fdin, "%d", &noattr) != 1) {
		    fprintf (stdout, "error on fscanf -> no. of attributes\n") ;
		    fflush (stdout) ;
		    exit () ;
	    }
	    while (noattr-- > 0) {
		    /* supplying of struct D23_at_type */
		    if (fscanf (fdin, "%s", str_attr) != 1) {
			    fprintf (stdout, "error on fscanf -> attribute type\n") ;
			    fflush (stdout) ;
			    exit () ;
		    }
		    d27_302_str_oid(str_attr, oid);
		    memcpy((Octet *) (eptr + D23_S_ATT), oid->d2_type, oid->d2_typ_len);
		    ((D23_at_type *) eptr)->d23_atttag = D23_ATTTAG ;
		    ((D23_at_type *) eptr)->d23_atlen = oid->d2_typ_len;
		    ((D23_at_type *) eptr)->d23_atoff = D2_ALIGN (((D23_at_type *) eptr)->d23_atlen) ;
		    eptr += D23_S_ATT + ((D23_at_type *) eptr)->d23_atoff ;

		    /* supplying of struct D23_av_info */
		    ((D23_av_info *) eptr)->d23_avtag = D23_AVTAG ;
		    ((D23_av_info *) eptr)->d23_asyntx = D2_DISTNAME ;
		    if (fscanf (fdin, "%d", &novalues) != 1) {
			    fprintf (stdout, "error on fscanf -> no. of attribute values\n") ;
			    fflush (stdout) ;
			    exit () ;
		    }
		    ((D23_av_info *) eptr)->d23_avnum = novalues ;
		    eptr += D23_S_AV ;

		    while (novalues-- > 0) {
			    /* supplying of struct D23_av_value */
			    if (fscanf (fdin, "%s", str_in) != 1) {
				    fprintf (stdout, "error on fscanf -> attribute value\n") ;
				    fflush (stdout) ;
				    exit () ;
			    }
			    if (str_in[0] != '#')
			    {    strcpy((D23_av_value *) eptr + 1, str_in);
				 for (j=0, dn_found=D2_FALSE; j<NO_AT_TYPE; j++)
				 {       if (strcmp (str_attr, at_type[j]) == 0)
					 {     dn_found = D2_TRUE ;
					 }
				 }
				 if (dn_found == D2_TRUE)
				 {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1) + 1;
				 } else
				 {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1);
				 }
			    } else {
				 p = eptr + D23_S_ATV;
				 for (len=0, j=1; j<strlen(str_in); j+=2, len++)
				 {    sscanf(&str_in[j],"%2x",&res);
				      *p++ = (char) res;
				 }
				 ((D23_av_value *) eptr)->d23_avlen = len;
			    }
			    ((D23_av_value *) eptr)->d23_avoff = D2_ALIGN (((D23_av_value *) eptr)->d23_avlen) ;
			    eptr += D23_S_ATV + ((D23_av_value *) eptr)->d23_avoff ;
		    }
	    }
	    break ;
    case D2_DELATT :
	    if (fscanf (fdin, "%d", &noattr) != 1) {
		    fprintf (stdout, "error on fscanf -> no. of attributes\n") ;
		    fflush (stdout) ;
		    exit () ;
	    }
	    while (noattr-- > 0) {
		    /* supplying of struct D23_at_type */
		    if (fscanf (fdin, "%s", str_attr) != 1) {
			    fprintf (stdout, "error on fscanf -> attribute type\n") ;
			    fflush (stdout) ;
			    exit () ;
		    }
		    d27_302_str_oid(str_attr, oid);
		    memcpy((Octet *) (eptr + D23_S_ATT), oid->d2_type, oid->d2_typ_len);
		    ((D23_at_type *) eptr)->d23_atttag = D23_ATTTAG ;
		    ((D23_at_type *) eptr)->d23_atlen = oid->d2_typ_len;
		    ((D23_at_type *) eptr)->d23_atoff = D2_ALIGN (((D23_at_type *) eptr)->d23_atlen) ;
		    eptr += D23_S_ATT + ((D23_at_type *) eptr)->d23_atoff ;
	    }
	    break ;
    case D2_ADD_AV :
    case D2_DEL_AV :
	    /* supplying of struct D23_at_type */
	    if (fscanf (fdin, "%s", str_attr) != 1) {
		    fprintf (stdout, "error on fscanf -> attribute type\n") ;
		    fflush (stdout) ;
		    exit () ;
	    }
	    d27_302_str_oid(str_attr, oid);
	    memcpy((Octet *) (eptr + D23_S_ATT), oid->d2_type, oid->d2_typ_len);
	    ((D23_at_type *) eptr)->d23_atttag = D23_ATTTAG ;
	    ((D23_at_type *) eptr)->d23_atlen = oid->d2_typ_len;
	    ((D23_at_type *) eptr)->d23_atoff = D2_ALIGN (((D23_at_type *) eptr)->d23_atlen) ;
	    eptr += D23_S_ATT + ((D23_at_type *) eptr)->d23_atoff ;

	    /* supplying of struct D23_av_info */
	    ((D23_av_info *) eptr)->d23_avtag = D23_AVTAG ;
	    ((D23_av_info *) eptr)->d23_asyntx = D2_DISTNAME ;
	    if (fscanf (fdin, "%d", &novalues) != 1) {
		    fprintf (stdout, "error on fscanf -> no. of attribute values\n") ;
		    fflush (stdout) ;
		    exit () ;
	    }
	    ((D23_av_info *) eptr)->d23_avnum = novalues ;
	    eptr += D23_S_AV ;

	    while (novalues-- > 0) {
		    /* supplying of struct D23_av_value */
		    if (fscanf (fdin, "%s", str_in) != 1) {
			    fprintf (stdout, "error on fscanf -> attribute value\n") ;
			    fflush (stdout) ;
			    exit () ;
		    }
		    if (str_in[0] != '#')
		    {    strcpy((D23_av_value *) eptr + 1, str_in);
			 for (j=0, dn_found=D2_FALSE; j<NO_AT_TYPE; j++)
			 {       if (strcmp (str_attr, at_type[j]) == 0)
				 {     dn_found = D2_TRUE ;
				 }
			 }
			 if (dn_found == D2_TRUE)
			 {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1) + 1;
			 } else
			 {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1);
			 }
		    } else {
			 p = eptr + D23_S_ATV;
			 for (len=0, j=1; j<strlen(str_in); j+=2, len++)
			 {    sscanf(&str_in[j],"%2x",&res);
			      *p++ = (char) res;
			 }
			 ((D23_av_value *) eptr)->d23_avlen = len;
		    }
		    ((D23_av_value *) eptr)->d23_avoff = D2_ALIGN (((D23_av_value *) eptr)->d23_avlen) ;
		    eptr += D23_S_ATV + ((D23_av_value *) eptr)->d23_avoff ;
	    }
	    break ;
    default :
	    fprintf (stdout, "wrong modification type\n") ;
	    fflush (stdout) ;
	    exit () ;
    }

    /* supplying of struct D23_eom */
    ((D23_eom *)eptr)->d23_eomtag = D23_EOMTAG;
    eptr += D23_S_EOM ;
}

/* ------------------------- mk_comarg_msg ------------------------------------------- */

void mk_comarg_msg()
{
    /* supplying of struct D23_com_arg */
    ((D23_com_arg *) eptr)->d23_cmtag = D23_CMTAG ;
    ((D23_com_arg *) eptr)->d23_cmali = D2_NOT_DEFINED ;
    if (fscanf (fdin, "%ld", &((D23_com_arg *) eptr)->d23_cmctr.d23_scopt) != 1) {
	    fprintf (stdout, "error on fscanf -> common args\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_com_arg *) eptr)->d23_cmctr.d23_scpri = D2_PRIO_LOW;
    ((D23_com_arg *) eptr)->d23_cmctr.d23_sctlm = D2_T_UNLIMITED ;
    ((D23_com_arg *) eptr)->d23_cmctr.d23_scslm = D2_S_UNLIMITED ;
    ((D23_com_arg *) eptr)->d23_cmctr.d23_scref = D2_DMD_SCOPE ;
    ((D23_com_arg *) eptr)->d23_cmopr.d23_opnm_rs = D2_NOT_STARTED ;
    ((D23_com_arg *) eptr)->d23_cmopr.d23_op_nxtrdn = D2_NOT_DEFINED ;
    eptr += D23_S_CARG ;

    /* supplying of struct D23_se_param (not used) */

    /* supplying of struct D23_ds_name (DN of requestor) */
    if (fscanf (fdin, "%s", (D23_ds_name *) eptr + 1) != 1) {
	    fprintf (stdout, "error on fscanf -> DN of requestor\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    ((D23_ds_name *) eptr)->d23_dstag = D23_RQTAG ;
    ((D23_ds_name *) eptr)->d23_dslen = strlen ((D23_ds_name *) eptr + 1) + 1 ;
    ((D23_ds_name *) eptr)->d23_dsoff = D2_ALIGN (((D23_ds_name *) eptr)->d23_dslen) ;
    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;

    /* supplying of struct D23_ext_info (not used) */
}

/* ------------------------- mk_filter_msg ------------------------------------------- */

void mk_filter_msg ()
{
    char        str_in[D2_DNL_MAX] ;
    char        str_attr[D2_OBJ_IDL_MAX];
    Bool        found, dn_found ;
    int         j, i ;
    int         nofilter, nosubstr ;
    D2_a_match  match ;
    char    *p;
    int     res;
    int     len;

    /* supplying of struct D23_ft1_info */
    ((D23_ft1_info *) eptr)->d23_fp1tag = D23_PATTAG ;
    if (fscanf (fdin, "%s", str_in) != 1) {
	    fprintf (stdout, "error on fscanf -> operation mode for filters\n") ;
	    fflush (stdout) ;
	    exit () ;
    }

    if (strcmp(str_in, "nothing") != 0)
    {     for (j=0, found=D2_FALSE; j<NO_OPMODE; j++) {
		  if (strcmp (str_in, opmodes[j]) == 0) {
			  ((D23_ft1_info *) eptr)->d23_fp1mod = opmodec[j] ;
			  found = D2_TRUE ;
		  }
	  }
	  if (found != D2_TRUE) {
		  fprintf (stdout, "error:  operation mode for filters -> %s\n", str_in) ;
		  fflush (stdout) ;
		  exit () ;
	  }

	  switch (((D23_ft1_info *) eptr)->d23_fp1mod) {
	  case D2_AND:
	  case D2_OR:
		  if (fscanf (fdin, "%d", &((D23_ft1_info *) eptr)->d23_fp1count) != 1) {
			  fprintf (stdout, "error on fscanf -> no. of filters\n") ;
			  fflush (stdout) ;
			  exit () ;
		  }
		  nofilter = ((D23_ft1_info *) eptr)->d23_fp1count ;
		  eptr += D23_S_FT1 ;
		  for (i=0; i< nofilter; i++) {
			  mk_filter_msg () ;
		  }
		  break ;
	  case D2_NOT:
		  eptr += D23_S_FT1 ;
		  mk_filter_msg () ;
		  break ;
	  case D2_ITEM:
		  eptr += D23_S_FT1 ;
		  /* supplying of struct D23_fi1_info */
		  ((D23_fi1_info *) eptr)->d23_fi1tag = D23_FIITAG ;
		  if (fscanf (fdin, "%s", str_in) != 1) {
			  fprintf (stdout, "error on fscanf -> filter match\n") ;
			  fflush (stdout) ;
			  exit () ;
		  }
		  for (j=0, found=D2_FALSE; j<NO_MATCH; j++) {
			  if (strcmp (str_in, matches[j]) == 0) {
				  ((D23_fi1_info *) eptr)->d23_fi1match = matchec[j] ;
				  found = D2_TRUE ;
			  }
		  }
		  if (found != D2_TRUE) {
			  fprintf (stdout, "error:  filter match -> %s\n", str_in) ;
			  fflush (stdout) ;
			  exit () ;
		  }
		  if (fscanf (fdin, "%d", &((D23_fi1_info *) eptr)->d23_fi1syntx) != 1) {
			  fprintf (stdout, "error on fscanf -> filter syntax\n") ;
			  fflush (stdout) ;
			  exit () ;
		  }
		  match = ((D23_fi1_info *) eptr)->d23_fi1match ;
		  eptr += D23_S_FI1 ;

		  /* supplying of struct D23_at_type */
		  if (fscanf (fdin, "%s", str_attr) != 1) {
			  fprintf (stdout, "error on fscanf -> attribute type\n") ;
			  fflush (stdout) ;
			  exit () ;
		  }
		  d27_302_str_oid(str_attr, oid);
		  memcpy((Octet *) (eptr + D23_S_ATT), oid->d2_type, oid->d2_typ_len);
		  ((D23_at_type *) eptr)->d23_atttag = D23_ATTTAG ;
		  ((D23_at_type *) eptr)->d23_atlen = oid->d2_typ_len;
		  ((D23_at_type *) eptr)->d23_atoff = D2_ALIGN (((D23_at_type *) eptr)->d23_atlen) ;
		  eptr += D23_S_ATT + ((D23_at_type *) eptr)->d23_atoff ;

		  switch (match) {
		  case D2_EQUAL:
		  case D2_APPROX_MATCH:
		  case D2_GTEQ:
		  case D2_LTEQ:
			  /* supplying of struct D23_av_value */
			  if (fscanf (fdin, "%s", str_in) != 1) {
				  fprintf (stdout, "error on fscanf -> attribute value\n") ;
				  fflush (stdout) ;
				  exit () ;
			  }
			  if (str_in[0] != '#')
			  {    strcpy((D23_av_value *) eptr + 1, str_in);
			       for (j=0, dn_found=D2_FALSE; j<NO_AT_TYPE; j++)
			       {       if (strcmp (str_attr, at_type[j]) == 0)
				       {     dn_found = D2_TRUE ;
				       }
			       }
			       if (dn_found == D2_TRUE)
			       {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1) + 1;
			       } else
			       {    ((D23_av_value *) eptr)->d23_avlen = strlen ((D23_av_value *) eptr + 1);
			       }
			  } else {
			       p = eptr + D23_S_ATV;
			       for (len=0, j=1; j<strlen(str_in); j+=2, len++)
			       {    sscanf(&str_in[j],"%2x",&res);
				    *p++ = (char) res;
			       }
			       ((D23_av_value *) eptr)->d23_avlen = len;
			  }
			  ((D23_av_value *) eptr)->d23_avoff = D2_ALIGN (((D23_av_value *) eptr)->d23_avlen) ;
			  eptr += D23_S_ATV + ((D23_av_value *) eptr)->d23_avoff ;
			  break ;
		  case D2_SUBSTRINGS:
			  if (fscanf (fdin, "%d", &nosubstr) != 1) {
				  fprintf (stdout, "error on fscanf -> no. of substring\n") ;
				  fflush (stdout) ;
				  exit () ;
			  }
			  for (i=0; i<nosubstr; i++) {
				  /* supplying of struct D23_fs1_info */
				  ((D23_fs1_info *) eptr)->d23_fs1tag = D23_FISTAG ;
				  if (fscanf (fdin, "%s", str_in) != 1) {
					  fprintf (stdout, "error on fscanf -> position of substring\n") ;
					  fflush (stdout) ;
					  exit () ;
				  }
				  for (j=0, found=D2_FALSE; j<NO_SUBSPOS; j++) {
					  if (strcmp (str_in, subspos[j]) == 0) {
						  ((D23_fs1_info *) eptr)->d23_fs1position = subspoc[j] ;
						  found = D2_TRUE ;
					  }
				  }
				  if (found != D2_TRUE) {
					  fprintf (stdout, "error:  position of substring -> %s\n", str_in) ;
					  fflush (stdout) ;
					  exit () ;
				  }
				  if (fscanf (fdin, "%s", (D23_fs1_info *)eptr + 1) != 1) {
					  fprintf (stdout, "error on fscanf -> substring value\n") ;
					  fflush (stdout) ;
					  exit () ;
				  }
				  ((D23_fs1_info *) eptr)->d23_fs1len = strlen ((D23_fs1_info *) eptr + 1) ;
				  ((D23_fs1_info *) eptr)->d23_fs1off = D2_ALIGN (((D23_fs1_info *) eptr)->d23_fs1len) ;
				  eptr += D23_S_FS1 + ((D23_fs1_info *) eptr)->d23_fs1off ;
			  }
			  break ;
		  case D2_PRESENT:
			  break ;
		  default:
			  fprintf (stdout, "wrong attribute match\n") ;
			  fflush (stdout) ;
			  exit () ;
		  }
		  break ;
	  default:
		  fprintf (stdout, "wrong operation mod\n") ;
		  fflush (stdout) ;
		  exit () ;
	  }
    }
}

/* ------------------------- mk_abandon_msg --------------------------------------- */

EXPORT void mk_abandon_msg (dir_id)
Ushort   dir_id ;
{
    int     novalues ;
    char    str_attr[D2_OBJ_IDL_MAX];
    char    str_in[D2_DNL_MAX];
    char    *p;
    int     res;
    int     j;
    int     len;
    Bool    dn_found;

    /* supplying of struct D23_inabandon */
    ((D23_inabandon *) eptr)->d23_Rhdr.d23_versno = D23_APDUV22 ;
    ((D23_inabandon *) eptr)->d23_Rhdr.d23_dirid = dir_id ;
    if (fscanf (fdin, "%d", &((D23_inabandon *) eptr)->d23_Rhdr.d23_bindid) != 1) {
	    fprintf (stdout, "error on fscanf -> bind id\n") ;
	    fflush (stdout) ;
	    exit () ;
    }

    if (fscanf (fdin, "%ld", &((D23_inabandon *) eptr)->d23_Rinv_id) != 1) {
	    fprintf (stdout, "error on fscanf -> invoke id\n") ;
	    fflush (stdout) ;
	    exit () ;
    }
    eptr += D23_S_INA ;

    /* supplying of struct D23_eom */
    ((D23_eom *) eptr)->d23_eomtag = D23_EOMTAG ;
    eptr += D23_S_EOM ;
}

