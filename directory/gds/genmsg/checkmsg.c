/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: checkmsg.c,v $
 * Revision 1.1.11.2  1996/02/18  18:19:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:42:02  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:58:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:01  root]
 * 
 * Revision 1.1.9.1  1994/02/22  18:47:25  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:41:06  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  14:56:34  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:59:19  marrek]
 * 
 * Revision 1.1.5.3  1992/12/31  20:38:00  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:28:12  bbelch]
 * 
 * Revision 1.1.5.2  1992/09/29  21:11:35  devsrc
 * 	[hinman@sni] - Check in merges from SNI
 * 	[1992/09/14  20:26:19  treff]
 * 
 * Revision 1.1.2.2  1992/06/01  19:41:32  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:49:57  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:39:17  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: checkmsg.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 18:19:15 $";
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
#include <sys/time.h>
#endif /* STOP_TIME */
#include <stdio.h>

/************** DS-external includes                     ****************/

#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d23apdu.h>


/************** global functions  (import)                    ****************/
IMPORT  int     d27_301_oid_str();
/************** global variables  (import)                    ****************/

IMPORT  char           *eptr;
IMPORT  D23_wtrspb      resclnt ;
IMPORT  D2_a_type      *oid;
IMPORT  char           *str_out;

#ifdef STOP_TIME
IMPORT  struct          timeval start, stop ;
#endif /* STOP_TIME */

/* ------------------------- rs_bind_msg --------------------------------------- */

EXPORT void rs_bind_msg()
{
    short   size ;

    size = resclnt.d23_7lnrdt;
    if (size >= D23_S_RSB + D23_S_EOM) {
	fprintf (stdout, "Bind-id: %d\n", ((D23_rsbind *) eptr)->d23_Bhdr.d23_bindid) ;
	switch (((D23_rsbind *) eptr)->d23_Bcrkind) {
	case D23_NO_CRED:
		eptr += D23_S_RSB ;
		fprintf (stdout, "No credentials\n") ;
		fflush (stdout) ;
		break;
	case D23_SIMPLE_CRED:
		eptr += D23_S_RSB ;
		fprintf (stdout, "Simple credentials\n") ;
		fflush (stdout) ;
		eptr += D23_S_SIMCR ;

		if (((D23_val_info *) eptr)->d23_valtag == D23_VALTAG) {
			eptr += D23_S_VAL ;

			if (((D23_tm_info *) eptr)->d23_tmtag == D23_TIMTAG) {
			       fprintf (stdout, "Time1: %s\n", eptr + D23_S_TM) ;
			       eptr += D23_S_TM + ((D23_tm_info *) eptr)->d23_tmoff ;
			}

			if (((D23_tm_info *) eptr)->d23_tmtag == D23_TIMTAG) {
			       fprintf (stdout, "Time2: %s\n", eptr + D23_S_TM) ;
			       eptr += D23_S_TM + ((D23_tm_info *) eptr)->d23_tmoff ;
			}

			if (((D23_bit_info *) eptr)->d23_btag == D23_BITTAG) {
			       fprintf (stdout, "Random1: %s\n", eptr + D23_S_BIT) ;
			       eptr += D23_S_BIT + ((D23_bit_info *) eptr)->d23_boff ;
			}

			if (((D23_bit_info *) eptr)->d23_btag == D23_BITTAG) {
			       fprintf (stdout, "Random2: %s\n", eptr + D23_S_BIT) ;
			       eptr += D23_S_BIT + ((D23_bit_info *) eptr)->d23_boff ;
			}
		}

		if (((D23_ds_name *) eptr)->d23_dsoff != 0)
		{      fprintf (stdout, "DN of administrator: %s\n", eptr + D23_S_NM) ;
		       eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;
		       fprintf (stdout, "Password: %s\n", eptr + D23_S_PW) ;
		       eptr += D23_S_PW + ((D23_pw_info *) eptr)->d23_pwoff ;
		} else {
		       eptr += D23_S_PW ;
		}
		break;
	case D23_STRONG_CRED:
		eptr += D23_S_RSB ;
		fprintf (stdout, "Strong credentials\n") ;
		fflush (stdout) ;
		eptr += D23_S_SSCR ;
		break;
	case D23_EXTERNAL_CRED:
		eptr += D23_S_RSB ;
		fprintf (stdout, "External credentials\n") ;
		fflush (stdout) ;
		eptr += D23_S_EXTCR ;
		break;
	default:
		fprintf (stdout, "credential kind: wrong input\n") ;
		fflush (stdout) ;
		exit () ;
	}
    } else
	fprintf (stdout, "Wrong result message\n") ;
}

/* ------------------------- rs_add_obj_msg ---------------------------------------- */

EXPORT void rs_add_obj_msg()
{
    short   size ;

    size = resclnt.d23_7lnrdt;
    if (size == D23_S_EOM &&
	((D23_eom *) eptr)->d23_eomtag == D23_EOMTAG)
	    fprintf (stdout, "Service request ok\n") ;
    else
	    fprintf (stdout, "Wrong result message\n") ;

}

/* ------------------------- rs_rem_obj_msg ----------------------------------------- */

EXPORT void rs_rem_obj_msg()
{
    short   size ;

    size = resclnt.d23_7lnrdt;
    if (size == D23_S_EOM &&
	((D23_eom *) eptr)->d23_eomtag == D23_EOMTAG)
	    fprintf (stdout, "Service request ok\n") ;
    else
	    fprintf (stdout, "Wrong result message\n") ;

}

/* ------------------------- rs_read_msg ------------------------------------------- */

EXPORT void rs_read_msg()
{
    short   rsize ;
    char    *pin, *pout ;
    char    ibuf[16384] ;
    Ushort  number;
    int     i, j, len;
    int     res;
    Bool    as = D2_TRUE;
    char    str[D2_DNL_MAX];
    Octet_string   p;

    if (resclnt.d23_7rmdat == D23_MRDATA) {
	    rsize = 0 ; pin = ibuf ;
	    do {
		    if (rsize > 0) {
			    if (d23_waitrs (&resclnt) == D23_ERROR) {
				    fprintf (stdout, "error on IPC-client wait result (%d)\n",
				    resclnt.d23_7hdinfo.d2_errvalue) ;
				    fflush (stdout) ;
				    exit () ;
			    }
		    }
		    if (rsize > 16384) {
			    fprintf (stdout,
			    "error on IPC-client wait result (max. buffer size exceeded)\n") ;
			    fflush (stdout) ;
			    exit () ;
		    }
		    for (i = 0, pout = resclnt.d23_7rfrdt; i < resclnt.d23_7lnrdt; i++)
			    *pin++ = *pout++ ;
		    rsize += resclnt.d23_7lnrdt ;
	    } while (resclnt.d23_7rmdat == D23_MRDATA) ;
	    eptr = ibuf ; resclnt.d23_7rltid = rsize ;
#ifdef STOP_TIME
	    gettimeofday (&stop) ;
	    if (stop.tv_usec >= start.tv_usec)
		    fprintf (stdout, "Duration of complete service request: %ld:%03d sec\n",
			     stop.tv_sec - start.tv_sec, (stop.tv_usec - start.tv_usec)/1000L) ;
	    else
		    fprintf (stdout, "Duration of complete service request: %ld:%03d sec\n",
			     stop.tv_sec - start.tv_sec - 1L, 1000L - (start.tv_usec - stop.tv_usec)/1000L) ;
#endif
    }

    if (resclnt.d23_7lnrdt >= (D23_S_CRES + D23_S_EOM)) {
	    if (((D23_en_info *) eptr)->d23_entag == D23_ENTTAG) {
		    eptr += D23_S_EN;
		    if (((D23_ds_name *) eptr)->d23_dstag == D23_NAMTAG) {
			    fprintf (stdout, "Dist. Name: %s\n", eptr + D23_S_NM) ;
			    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;
		    }
		    fprintf (stdout, "Attributes:\n") ;

		    while (((D23_at_type *) eptr)->d23_atttag == D23_ATTTAG) {
			    oid->d2_type = (Ob_id_string)(eptr + D23_S_ATT);
			    oid->d2_typ_len = ((D23_at_type *) eptr)->d23_atlen;
			    d27_301_oid_str (oid, str_out);
			    fprintf (stdout, "\t%.*s:\n", strlen(str_out), str_out);
			    eptr += D23_S_ATT + ((D23_at_type *) eptr)->d23_atoff ;
			    number = ((D23_av_info *) eptr)->d23_avnum;
			    eptr += D23_S_AV;
			    if (number != 0)
			    {   for (i=0; i<(int)number; i++) {
#ifdef OIDTEST
fprintf(stdout, "TEST");
				    for (p = (Octet *)(eptr + D23_S_ATV), len = 0; len < ((D23_av_value *) eptr)->d23_avlen; p+=2, len++)
				    {    if (isascii(p) == D2_NULL)
					 {     as=D2_FALSE;
					       break;
					 }
				    }
				    if (as == D2_TRUE)
				    {    fprintf (stdout, "\t%.*s\n", ((D23_av_value *) eptr)->d23_avlen,
							    eptr + D23_S_ATV );
				    } else
				    {    p = (Octet *)(eptr + D23_S_ATV);
fprintf(stdout, "val_hex: %.*x\n", ((D23_av_value *) eptr)->d23_avlen, p);
					 for (len=0, j=0; len <= ((D23_av_value *) eptr)->d23_avlen; j+=2, len++)
					 {     sscanf(p,"%2x", &res);
					       str[j] = (char) res & 0xFF;
					       str_out += 2;
					       *str_out = (char) res & 0xFF;
fprintf(stdout, "str_out(c): %2c\n", *str_out);
					 }
					 str[j-1]=D2_NULL;
					 fprintf (stdout, "\t%.*s\n", strlen(str_out), str_out);
fprintf(stdout, "str(len): %d\n", strlen(str));
fprintf(stdout, "str_out(len): %d\n", strlen(str_out));
fprintf (stdout, "\t%.*s\n", strlen(str), str);
				    }
#else
				    fprintf (stdout, "\t%.*s\n", ((D23_av_value *) eptr)->d23_avlen, eptr + D23_S_ATV);
#endif
				    eptr += D23_S_ATV + ((D23_av_value *) eptr)->d23_avoff ;
				}
			    }
		    }


	    } else {
		    fprintf (stdout, "Positive result, but no requested attribute found\n") ;
	    }

	    if (((D23_com_res *) eptr)->d23_crtag == D23_CRSTAG) {
		    eptr += D23_S_CRES ;
		    if (((D23_ds_name *) eptr)->d23_dstag == D23_NAMTAG) {
			    fprintf (stdout, "Requ. Name: %s\n", eptr + D23_S_NM) ;
			    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;
		    }
	    }
    } else
	    fprintf (stdout, "Wrong result message\n") ;
}

/* ------------------------- rs_list_msg ------------------------------------------- */

EXPORT void rs_list_msg()
{
    short   rsize ;
    char    *pin, *pout ;
    char    ibuf[16384] ;
    int     i;
    int     nmcnt ;

    if (resclnt.d23_7rmdat == D23_MRDATA) {
	    rsize = 0 ; pin = ibuf ;
	    do {
		    if (rsize > 0) {
			    if (d23_waitrs (&resclnt) == D23_ERROR) {
				    fprintf (stdout, "error on IPC-client wait result (%d)\n",
				    resclnt.d23_7hdinfo.d2_errvalue) ;
				    fflush (stdout) ;
				    exit () ;
			    }
		    }
		    if (rsize > 16384) {
			    fprintf (stdout,
			    "error on IPC-client wait result (max. buffer size exceeded)\n") ;
			    fflush (stdout) ;
			    exit () ;
		    }
		    for (i = 0, pout = resclnt.d23_7rfrdt; i < resclnt.d23_7lnrdt; i++)
			    *pin++ = *pout++ ;
		    rsize += resclnt.d23_7lnrdt ;
	    } while (resclnt.d23_7rmdat == D23_MRDATA) ;
	    eptr = ibuf ; resclnt.d23_7rltid = rsize ;
#ifdef STOP_TIME
	    gettimeofday (&stop) ;
	    if (stop.tv_usec >= start.tv_usec)
		    fprintf (stdout, "Duration of complete service request: %ld:%03d sec\n",
			     stop.tv_sec - start.tv_sec, (stop.tv_usec - start.tv_usec)/1000L) ;
	    else
		    fprintf (stdout, "Duration of complete service request: %ld:%03d sec\n",
			     stop.tv_sec - start.tv_sec - 1L, 1000L - (start.tv_usec - stop.tv_usec)/1000L) ;
#endif
    }

    if (resclnt.d23_7lnrdt >= (D23_S_NM + D23_S_EOM)) {
	    if (((D23_ds_name *) eptr)->d23_dstag == D23_NAMTAG) {
		    fprintf (stdout, "Base object name: %s\n", eptr + D23_S_NM) ;
		    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;
	    }
	    fprintf (stdout, "Objects:\n") ;
	    nmcnt = 0 ;
	    if (((D23_subord_info *) eptr)->d23_sotag == D23_SOITAG) {
		    eptr += D23_S_SUBORD ;
		    while (((D23_ds_name *) eptr)->d23_dstag == D23_NAMTAG) {
			    fprintf (stdout, "\t%3d. %s\n", ++nmcnt, eptr + D23_S_NM) ;
			    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;
		    }
	    }
    } else
	    fprintf (stdout, "Wrong result message\n") ;
}

/* ------------------------- rs_compare_msg ---------------------------------------- */

EXPORT void rs_compare_msg()
{

    if (resclnt.d23_7lnrdt >= (D23_S_COMRS + D23_S_CRES + D23_S_EOM)) {
	    if (((D23_rscompinfo *) eptr)->d23_Lmatsgn == D2_TRUE)
		    fprintf (stdout, "Compare positive\n") ;
	    else
		    fprintf (stdout, "Compare negative\n") ;
    } else
	    fprintf (stdout, "Wrong result message\n") ;
}

/* ------------------------- rs_search_msg ----------------------------------------- */

EXPORT void rs_search_msg()
{
    short   rsize ;
    char    *pin, *pout ;
    char    ibuf[16384] ;
    int     i, novalues, j,len ;
    Ushort *p_val;
    int    *p;
    Bool    as = D2_TRUE;

    if (resclnt.d23_7rmdat == D23_MRDATA) {
	    rsize = 0 ; pin = ibuf ;
	    do {
		    if (rsize > 0) {
			    if (d23_waitrs (&resclnt) == D23_ERROR) {
				    fprintf (stdout, "error on IPC-client wait result (%d)\n",
				    resclnt.d23_7hdinfo.d2_errvalue) ;
				    fflush (stdout) ;
				    exit () ;
			    }
		    }
		    if (rsize > 16384) {
			    fprintf (stdout,
			    "error on IPC-client wait result (max. buffer size exceeded)\n") ;
			    fflush (stdout) ;
			    exit () ;
		    }
		    for (i = 0, pout = resclnt.d23_7rfrdt; i < resclnt.d23_7lnrdt; i++)
			    *pin++ = *pout++ ;
		    rsize += resclnt.d23_7lnrdt ;
	    } while (resclnt.d23_7rmdat == D23_MRDATA) ;
	    eptr = ibuf ; resclnt.d23_7rltid = rsize ;
#ifdef STOP_TIME
	    gettimeofday (&stop) ;
	    if (stop.tv_usec >= start.tv_usec)
		    fprintf (stdout, "Duration of complete service request: %ld:%03d sec\n",
			     stop.tv_sec - start.tv_sec, (stop.tv_usec - start.tv_usec)/1000L) ;
	    else
		    fprintf (stdout, "Duration of complete service request: %ld:%03d sec\n",
			     stop.tv_sec - start.tv_sec - 1L, 1000L - (start.tv_usec - stop.tv_usec)/1000L) ;
#endif
    }

    if (resclnt.d23_7lnrdt > (D23_S_NM + D23_S_EOM)) {
	    if (((D23_ds_name *) eptr)->d23_dstag == D23_NAMTAG) {
		    fprintf (stdout, "Base object name: %s\n", eptr + D23_S_NM) ;
		    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;
	    }
	    while (((D23_en_info *) eptr)->d23_entag == D23_ENTTAG) {
		    eptr += D23_S_EN ;
		    fprintf (stdout, "%s\n", (D23_ds_name *) eptr + 1) ;
		    eptr += D23_S_NM + ((D23_ds_name *) eptr)->d23_dsoff ;
		    fprintf (stdout, "Attributes:\n") ;
		    while (((D23_at_type *) eptr)->d23_atttag == D23_ATTTAG) {
			    oid->d2_type = (Ob_id_string)(eptr + D23_S_ATT);
			    oid->d2_typ_len = ((D23_at_type *) eptr)->d23_atlen;
			    d27_301_oid_str (oid, str_out);
			    fprintf (stdout, "\t%.*s:\n", strlen(str_out), str_out);
			    eptr += D23_S_ATT + ((D23_at_type *) eptr)->d23_atoff ;

			    if (((D23_av_info *) eptr)->d23_avtag == D23_AVTAG) {
				    novalues = ((D23_av_info *) eptr)->d23_avnum ;
				    eptr += D23_S_AV ;
				    while (novalues-- > 0) {
#ifdef OIDTEST
					    for (p = (int *)(eptr + D23_S_ATV), len = 0; len < ((D23_av_value *) eptr)->d23_avlen; p+=2, len++)
					    {    if (isascii(p) == D2_NULL)
						 {     as=D2_FALSE;
						       break;
						 }
					    }
					    if (as == D2_TRUE)
					    {    fprintf (stdout, "\t%.*s\n", ((D23_av_value *) eptr)->d23_avlen,
								    eptr + D23_S_ATV );
					    } else
					    {    p_val = (Ushort *)(eptr + D23_S_ATV);
						 for (len=0, j=1; len < ((D23_av_value *) eptr)->d23_avlen; j+=2, len++)
						 {     sscanf(p_val,"%2x && FF", str_out + j );
						       p_val += 2;
						 }
						 fprintf (stdout, "\t%.*s\n", strlen(str_out), str_out);
					    }
#else
					    fprintf (stdout, "\t%.*s\n", ((D23_av_value *) eptr)->d23_avlen, (D23_av_value *) eptr + 1) ;
#endif
					    eptr += D23_S_ATV + ((D23_av_value *) eptr)->d23_avoff ;
				    }
			    }
		    }
	    }
	    if (((D23_part_oq *) eptr)->d23_oqtag == D23_OQTAG)
	    {
	    }
    } else {
	    if (resclnt.d23_7lnrdt == (D23_S_EOM + D23_S_CRES))
		    fprintf (stdout, "Positive result, but no requested attribute found\n") ;
	    else
		    fprintf (stdout, "Wrong result message\n") ;
    }
}

/* ------------------------- rs_mod_msg --------------------------------------- */

EXPORT void rs_mod_msg()
{
    short   size ;

    size = resclnt.d23_7lnrdt;
    if (size == D23_S_EOM &&
	((D23_eom *) eptr)->d23_eomtag == D23_EOMTAG)
	    fprintf (stdout, "Modify entry ok\n") ;
    else
	    fprintf (stdout, "Wrong result message\n") ;
}

/* ------------------------- rs_repl_rdn_msg --------------------------------------- */

EXPORT void rs_repl_rdn_msg()
{
    short   size ;

    size = resclnt.d23_7lnrdt;
    if (size == D23_S_EOM &&
	((D23_eom *) eptr)->d23_eomtag == D23_EOMTAG)
	    fprintf (stdout, "Replace rdn ok\n") ;
    else
	    fprintf (stdout, "Wrong result message\n") ;
}

/* ------------------------- rs_abandon_msg --------------------------------------- */

EXPORT void rs_abandon_msg()
{
    short   size ;

    size = resclnt.d23_7lnrdt;
    asn1_dump(stderr,eptr,eptr+size);
    if (size == D23_S_EOM &&
	((D23_eom *) eptr)->d23_eomtag == D23_EOMTAG)
	    fprintf (stdout, "Abandon ok\n") ;
    else
	    fprintf (stdout, "Wrong result message\n") ;
}
