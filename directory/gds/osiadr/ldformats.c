/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ldformats.c,v $
 * Revision 1.1.713.2  1996/02/18  18:21:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:50  marty]
 *
 * Revision 1.1.713.1  1995/12/08  16:07:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:14  root]
 * 
 * Revision 1.1.711.4  1994/09/06  12:26:21  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:24:03  keutel]
 * 
 * Revision 1.1.711.3  1994/07/06  15:08:04  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:18:58  marrek]
 * 
 * Revision 1.1.711.2  1994/05/10  15:57:31  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:43:52  marrek]
 * 
 * Revision 1.1.711.1  1994/02/22  16:11:17  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:02:18  marrek]
 * 
 * Revision 1.1.709.2  1993/10/14  17:52:31  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:43:12  keutel]
 * 
 * Revision 1.1.709.1  1993/10/13  17:34:00  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  17:42:20  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  12:45:09  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:31:42  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  21:30:06  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:34:33  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  21:01:09  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:52:52  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: ldformats.c,v $ $Revision: 1.1.713.2 $ $Date: 1996/02/18 18:21:01 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : ldformats.c                                         */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 07.05.90                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     :                                                     */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES:                                               */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d2p_load_addr_formats ()                                      */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*    The function reads the supported OSI-NSAP-address formats from a file  */
/*    into the global NSAP-address description table.                        */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*                     None                                                  */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      0           = The operation has been performed successfully.         */
/*      < 0         = An error is occurred during reading the address        */
/*                    formats (in this case, the returned negative value     */
/*                    corresponds to the problem).                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 07.05.90     */
/*exoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <d2dir.h>
#include <d27util.h> 
#include <dce/osiaddr.h>


#define		SUCCESS		1
#define		FAIL		0


/* --------------- DECLARATION OF LOCAL DATA ------------------------------- */

static Nsap_idpinfo	*nsap_forminfo ;/* start address NSAP-address */
					/* format description table */
static Nsap_idpinfo	*nsap_mxinfo ;	/* end address of NSAP-address */
					/* format description table */
#ifdef THREADSAFE
					/* 'NSAP-format description loaded'- */
					/* flag */
static pthread_once_t nsap_form_loaded = pthread_once_init ;
#endif

/* --------------- PROTOTYPING OF LOCAL FUNCTIONS -------------------------- */

static signed32 match_encoded_parts (char *[], signed16, Nsap_dspinfo *) ;
static signed32 match_decoded_parts (char *, Nsap_dspinfo *, char) ;
static signed32 formatidcmp (char *, char *, char **) ;
static signed32 prepare_format_id (char, char *, char *, char, signed32, char,
								     signed32) ;

/* ------------------------------------------------------------------------- */

signed32 d2p_load_addr_formats () {

    struct stat	flstat ;
    FILE	*fd ;
    signed32	cnt, nsap_isize ;
    char	*pptr, tmpbuf[D27_LFILE_NAME] ;

    sprintf (tmpbuf, "%s%s%s%s", dcelocal_path, D27_CLIENT_DIR,
				      D27_CONF_DIR, OSIFORM) ;
    /* get size of NSAP-format description file */
    if (stat (tmpbuf, &flstat) == -1)
	return (DESCSIZE_ERR) ;

    if ((pptr = (char *) malloc ((size_t) flstat.st_size)) == (char *) NULL)
	return (DESCMEM_ERR) ;

    if ((fd = fopen (tmpbuf, "r")) == (FILE *) NULL) {
	return (DESCFOPEN_ERR) ;
    }

    if ((cnt = fread (pptr, (size_t) flstat.st_size, 1, fd)) != 1) {
	free (pptr) ;
	fclose (fd) ;
	return (DESCFREAD_ERR) ;
    }
    fclose (fd) ;

    if (strcmp (NSAPFORMVERS, pptr) != 0) {
	free (pptr) ;
	return (DESCVERS_ERR) ;
    }
    nsap_isize = flstat.st_size - D2_ALIGN(sizeof(NSAPFORMVERS)) ;
    nsap_forminfo = (Nsap_idpinfo *) (pptr + D2_ALIGN(sizeof(NSAPFORMVERS))) ;
    nsap_mxinfo = (Nsap_idpinfo *) ((char *) nsap_forminfo + nsap_isize) ;
    return (NO_ERR) ;
}

/* ------------------------------------------------------------------------- */
/*   The following function returns a reference to the NSAP-address format   */
/*   description entry (IDP-description) which corresponds to the supplied   */
/*   AFI resp. NSAP-address format name and/or a reference to the NSAP-      */
/*   address format description entry (DSP-description) which corresponds    */
/*   to the supplied IDI resp. DSP-ID.                                       */
/* ------------------------------------------------------------------------- */

signed32 d2p_srch_nsap_form (signed16 opmode, Nsap_idpinfo **idpptr,
			     signed32 afi, char *nsapnmptr,
			  Nsap_dspinfo **dspptr, char *idiptr, void *dspidptr) {

    Nsap_idpinfo	*iidpptr = *idpptr ;
    Nsap_dspinfo	*idsphptr ;
    signed32		res, idisize, iditype ;
    char		idival[MAX_IDI], dspsyntax ;

    /* load NSAP-address format description table */
#ifdef THREADSAFE
    if ((res = pthread_once (&nsap_form_loaded,
		           (pthread_initroutine_t) d2p_load_addr_formats)) < 0) 
	return (res) ;
#else
    if (nsap_forminfo == (Nsap_idpinfo *) NULL) {
	if ((res = d2p_load_addr_formats ()) < 0)
	    return (res) ;
    }
#endif

    if (iidpptr == (Nsap_idpinfo *) NULL) {
	/* search entry which describes this (AFI specific) NSAP-address */
	iidpptr = nsap_forminfo ;
	while (iidpptr < nsap_mxinfo) {
	    if (GET_ETYPE(iidpptr->geninfo) == IDP_ENTRY) {
		if ((afi != 0 && (iidpptr->afi0 == afi || iidpptr->afi1 == afi)) ||
		   (afi == 0 && strncmp (nsapnmptr, iidpptr->idp_fname,
							       MAX_IDPNM) == 0))
		    break ;
		iidpptr++ ;
	    } else
		iidpptr = (Nsap_idpinfo *) ((Nsap_dspinfo *) iidpptr + 1) ;
	}
	if (iidpptr >= (Nsap_idpinfo *) nsap_mxinfo)
		    return (UNKNOWN_AFI_ERR) ; 
	*idpptr = iidpptr ;
    }
    if (dspptr != (Nsap_dspinfo **) NULL) {
	if (iidpptr->idi_size > 0) {
	    if ((idisize = (size_t)strlen (idiptr)) == 0 ||
						    idisize > iidpptr->idi_size)
		return (IDI_SIZE_ERR) ;

	    memset ((void *) idival, (int) PADCHAR, (size_t) iidpptr->idi_size) ;
	    strncpy (&idival[iidpptr->idi_size-idisize], idiptr,
							     (size_t) idisize) ;
	}
	dspsyntax = GET_DSPSYN(iidpptr->geninfo);
	*dspptr  = (Nsap_dspinfo *) NULL ;
	idsphptr = (Nsap_dspinfo *) (iidpptr + 1) ;
	while (GET_ETYPE(idsphptr->geninfo) == DSP_ENTRY &&
				      idsphptr < (Nsap_dspinfo *) nsap_mxinfo) {
	    iditype = GET_IDITYPE(idsphptr->geninfo) ;
	    if (iditype == IDI_ALL ||
		(iditype == IDI_SINGLE &&
		strncmp (idival, idsphptr->idi_lowval, iidpptr->idi_size) == 0) ||
		(iditype == IDI_RANGE &&
		strncmp(idival, idsphptr->idi_lowval, iidpptr->idi_size) >= 0 &&
		strncmp(idival, idsphptr->idi_highval, iidpptr->idi_size) <= 0)){
		if (opmode < 0) {
		    if (match_decoded_parts ((char *) dspidptr, idsphptr,
								    dspsyntax)){
		        *dspptr = idsphptr;
			break;
		    }
		} else
		    if (match_encoded_parts ((char **) dspidptr, opmode,
								    idsphptr)) {
			*dspptr = idsphptr;
			break;
		    }
	    }
	    if (iditype == IDI_ALL_EXCL) {
		if (opmode < 0) {
		    if (match_decoded_parts ((char *) dspidptr, idsphptr,
								     dspsyntax))
			*dspptr = idsphptr;
		} else
		    if (match_encoded_parts ((char **) dspidptr, opmode,
								      idsphptr))
			*dspptr = idsphptr;
	    } 
	    idsphptr++ ;
	}
	if (*dspptr == (Nsap_dspinfo *) NULL)
	    return (U_DSP_FORMAT_ERR) ;
    }
    return (NO_ERR) ;
}

/*****************************************************************************/
/* The function matches the dsp parts to right DSP structure . The arguments */
/* are as follows :							     */
/* 									     */
/* dspstr : This is array of strings containing the dspparts 		     */
/*									     */
/* numofparts : The total number of DSP parts contained in above array       */
/*									     */
/* dspptr     : The DSP structure that is to matched with given information  */
/*									     */ 
/*****************************************************************************/

static signed32 match_encoded_parts (char *dspstr[], signed16 numofparts,
							 Nsap_dspinfo *dspptr) {

    signed32	i, actnumofparts = 0 ;
    char	dspid_out[MAX_DSP] ;

    for (i = 0; i < numofparts; i++)
	DBGPRINT((DEV,"<partno> = %d <dspstr> = %s \n", i, dspstr[i])); 
    /* Changes made to take care of more then one optional parts in DSP */
    if (dspptr->dsp_parts < numofparts) {
        DBGPRINT((DEV,"Number of parts Error \n")); 
	return (FAIL) ;
    }
    if (dspptr->dsp_parts > numofparts) {
	for (i=0; i<dspptr->dsp_parts; i++) {
	   if (!GET_DSP_OPTIONAL (dspptr->dspflag[i]) )
		++actnumofparts ;
	 }
	if (numofparts < actnumofparts) {
        	DBGPRINT((DEV,"Number of parts Error n")); 
		return (FAIL);
	 }
    }

    for (i = 0;i < (signed32)(dspptr->dsp_parts) && (i < numofparts); i++) {
	if (GET_DSP_FORMAT_ID(dspptr->dspflag[i])) {
	    if(prepare_format_id(GET_DSP_UNIT(dspptr->dspflag[i]), 
				  dspstr[i], dspid_out,
				  GET_PADCHAR(dspptr->dspflag[i]),
		    		  dspptr->dsp_partsize[i] ,
		    		  GET_DSP_JUST(dspptr->dspflag[i]),
				  dspptr->dsp_partsize_min[i]) <0) 
		return(FAIL);
	    if (strcmp(dspptr->dsp_id[i],dspid_out) != 0)
		return(FAIL);
	}
    }
    return(SUCCESS);
}

/*****************************************************************************/
/* This function will try to match the decoded parts depending upon the DSP  */
/* passed in as argument 						     */
/* 									     */
/* dspstr : Input decoded DSP part string				     */
/*									     */
/* dspptr : The pointer to DSP structure that should be used for matching    */
/*								             */	
/* dspsyntax : The DSP SYNTAX						     */
/*****************************************************************************/

static signed32 match_decoded_parts (char *dspstr, Nsap_dspinfo *dspptr,
							       char dspsyntax) {

    signed32	i ;
    char	*nxtptr, *str ;


    str = dspstr;
    DBGPRINT((DEV,"INPUT STRING == %s \n", dspstr)); 
    for (i = 0; i < (signed32) dspptr->dsp_parts; i++) {
	if (GET_DSP_FORMAT_ID(dspptr->dspflag[i])) {
	    if (!formatidcmp (str, dspptr->dsp_id[i], &nxtptr))
		return(FAIL);
	} else {
	    if (GET_DSP_VAR_ID(dspptr->dspflag[i])) {
		if (GET_DSP_OPTIONAL(dspptr->dspflag[i]) && 
						  (i == (dspptr->dsp_parts -1)))
		    return(SUCCESS);
		if (GET_DSP_VAR_SIZE(dspptr->dspflag[i]) == DSP_VAR_SIZE) {
		    if (!vardsppart (str,dspptr->dspflag[i],
		       dspsyntax,&nxtptr,(char *)NULL, dspptr->dsp_partsize[i]))
			return(FAIL);
		} else {
		    if ( i == (dspptr->dsp_parts-1))
			return(SUCCESS);
		    else
			return(FAIL); 
	    	}
	    } else {
		if ((dspptr->dsp_partsize[i]) == 0 )
		    return(FAIL);
		else {
		    if ((size_t)strlen(str) >= dspptr->dsp_partsize[i])
			nxtptr = &(str[dspptr->dsp_partsize[i]]);
		    else if (((size_t)strlen(str) == 0) && 
				GET_DSP_OPTIONAL(dspptr->dspflag[i]))
			return (SUCCESS) ;
		    else if ((strcmp(str,"F") == 0 || strcmp(str,"f") == 0) 
				&& (size_t) strlen(str) == 1
				&& GET_DSP_OPTIONAL(dspptr->dspflag[i]))
			return (SUCCESS) ;
		    else
			return(FAIL);
		}
	    }
	}
	str = nxtptr;
    }
    DBGPRINT((DEV,"FINAL STRING == %s \n",str)); 
    if (((size_t)strlen(str) == 0) || 
	((strcmp(str,"F") == 0 || strcmp(str,"f") == 0 ) &&
						 (size_t) strlen(str) == 1))
	return(SUCCESS);
    else
	return(FAIL);
}

/*****************************************************************************/
/* The function will try to match format identifiers 			     */
/* str : The string that is to be matched with format identifier	     */
/*									     */
/* dspid :   DSP Part identifier					     */
/*									     */
/* nxtptr : In case the format identifier is matched then the character ptr  */
/*	    points to the next character after format identifier	     */
/*****************************************************************************/

static signed32 formatidcmp (char *str, char *dspid, char **nxtptr) {

    if (strncmp (str, dspid, (size_t) strlen (dspid)) != 0)
	return(FAIL);
    *nxtptr = &(str[(size_t)strlen(dspid)]);	
    return (SUCCESS);
}

/*****************************************************************************/
/* This function deals with variable DSP parts , if a variable DSP part then */
/* it is necessary to find length of variable DSP part depending upon unit   */
/* type and accordingly eliminate the number of characters from the input str*/
/* before going for further processing in finding correct DSP structure      */
/*									     */
/* str : Input string to which the variable part belongs 		     */
/*									     */
/* dspflag : The flags corresponding to DSP structure			     */
/*									     */
/* dspsyntax : The DSP SYNTAX						     */
/*									     */
/* nxtptr   : Pointer to character after the variable part		     */
/*									     */
/* dspval   : This will contain the variable DSP part value if the call is   */
/*	      made with valid character array and nothing if made with NULL  */ 
/*									     */
/* partsz   : Size of the DSP part					     */
/*****************************************************************************/

signed32 vardsppart (char *str, signed32 dspflag, char dspsyntax,
				 char **nxtptr, char *dspval, signed32 partsz) {

    char	varsize, unittype ;	
    signed32	dsplength, dspprefix = 0 ;

    varsize = GET_DSP_VAR_SIZE(dspflag);
    if (varsize != DSP_VAR_SIZE) 
	return(FAIL);

    unittype = GET_DSP_UNIT(dspflag);
    switch (unittype) {
    case DSP_UNIT_DEC1:
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= MAX_BIN_VAR_LEN) {
	    	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	     	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	} else { 
	    if (partsz <= MAX_DEC_VAR_LEN) {
	   	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	     	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	}
	break;
    case DSP_UNIT_DEC2:
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    sscanf(str,"%2d",&dsplength);
	    dspprefix = 1;
	} else
	    return(DSP_WRONG_UNIT); 
	dsplength *= 2;
	break;
    case DSP_UNIT_HEX1:
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= MAX_BIN_VAR_LEN) {
	   	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	     	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	} else
	    return(DSP_WRONG_UNIT); 
	break;
    case DSP_UNIT_HEX2:
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    sscanf(str,"%2d",&dsplength);
	    dspprefix = 2;
	} else
	    return(DSP_WRONG_UNIT); 
	dsplength *= 2;
	break;
    case DSP_UNIT_ASCII1:
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= 3*MAX_BIN_VAR_LEN) {
	   	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	   	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	} else { 
	    if (partsz <= 3*MAX_DEC_VAR_LEN) {
	   	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	    	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	}
	dsplength *= 3;
	break;
    case DSP_UNIT_ASCII2:
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= 2*MAX_BIN_VAR_LEN) {
	     	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	     	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	} else
	    return(DSP_WRONG_UNIT); 
	dsplength *= 2;
	break;
    case DSP_UNIT_DOT1:
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= 3*MAX_BIN_VAR_LEN) {
  	  	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	     	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	} else { 
	    if (partsz <= 3*MAX_DEC_VAR_LEN) {
	     	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	   	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	}
	dsplength *= 3;
	break;
    case DSP_UNIT_DOT2:
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= 2*MAX_BIN_VAR_LEN) {
	   	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	     	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	}
	dsplength *= 2;
	break;
    case DSP_UNIT_SLASH11:
    case DSP_UNIT_SLASH12:
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= (unittype == DSP_UNIT_SLASH11) ? 
					3*MAX_BIN_VAR_LEN : 5*MAX_BIN_VAR_LEN) {
	   	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	     	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	} else { 
	    if (partsz <= (unittype == DSP_UNIT_SLASH11) ?
					3*MAX_DEC_VAR_LEN : 5*MAX_BIN_VAR_LEN) {
	  	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	     	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	}
	if (unittype == DSP_UNIT_SLASH11)
	    dsplength *= 3;
	else
	    dsplength *= 5;
	break;
    case DSP_UNIT_SLASH21:
    case DSP_UNIT_SLASH22:
	if (dspsyntax == BIN_DSP_SYNTAX) {
	    if (partsz <= (unittype == DSP_UNIT_SLASH11) ? 
					2*MAX_BIN_VAR_LEN : 4*MAX_BIN_VAR_LEN) {
	   	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	     	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	} else { 
	    if (partsz <= (unittype == DSP_UNIT_SLASH11) ?
					2*MAX_DEC_VAR_LEN : 4*MAX_BIN_VAR_LEN) {
	    	sscanf(str,"%1d",&dsplength);
		dspprefix = 1;
	    } else {
	     	sscanf(str,"%2d",&dsplength);
		dspprefix = 2;
	    }
	}
	if (unittype == DSP_UNIT_SLASH21)
	    dsplength *= 2;
	else
	    dsplength *= 4;
	break;
    default:
	return(FAIL);
    }
    if ((dsplength <= 0) || (dsplength > (MAX_DSP-dspprefix)))
	return(FAIL);
    *nxtptr = &(str[dsplength+dspprefix]);
    if (dspval != (char *)NULL) {
	strncpy(dspval,&(str[dspprefix]),dsplength);
	dspval[dsplength] = '\0';
	fprintf(stderr,"dsplength == %d dspprefix == %d\n",dsplength,dspprefix);
    } else
	fprintf(stderr,"Dsplength == %d dspprefix == %d\n",dsplength,dspprefix);
    return(SUCCESS);
}

/****************************************************************************/
/* This function will validate, convert in internal format and pad the      */
/* character in the format identification inputted by the user		    */
/* Inputs : unittype : Part unittype 				            */
/* 	    dspid_in : Unprocessed ( raw /uncooked !!) DSP format id        */
/* 	    pchar    : Padding character				    */
/*	    partsz   : Expected Size of DSP Part			    */
/* 	    fmt_name : Will contain the name of the DSP format for which the*/
/* 			Processing is going on this argument will be used for*/
/*		       displaying error msg				    */
/* Outputs : dspid_out : Will contain the prepared (and ready to serve !!!) */
/*			 DSP format identification			    */
/****************************************************************************/

static signed32 prepare_format_id (char unittype, char *dspid_in1,
				char *dspid_out, char pchar, signed32 partsz,
						 char just, signed32 minunits) {

    char	pstr[10], dspid_in[40] ;

    strcpy (dspid_in, dspid_in1) ;
    memset ((void *) pstr, (int) '\0', (size_t) 10) ;
    switch (unittype) {
    case DSP_UNIT_DEC1:
	if (!isdigitstr(dspid_in))
	    return(-1);
	if ( (size_t)strlen(dspid_in) > partsz)
	    return(-1);
   	if (((size_t)strlen(dspid_in) < partsz) && (pchar != '-'))
    	    pad (dspid_in, pchar, partsz, just);
   	else
	    if ((size_t)strlen(dspid_in) != partsz)
		return(-1);
	strcpy (dspid_out, dspid_in) ;
	break;
    case DSP_UNIT_DEC2:
	if (!isdigitstr(dspid_in))
	    return(-1);
	if (((size_t)strlen(dspid_in) % 2) != 0 ||
					    ((size_t)strlen(dspid_in) > partsz))
	    return(-1);
   	if (((size_t)strlen(dspid_in) < partsz) && (pchar != '-'))
    	    pad (dspid_in, pchar, partsz, just);
   	else
	    if ((size_t)strlen(dspid_in) != partsz)
		return(-1);
	strcpy (dspid_out, dspid_in) ;
	break;
    case DSP_UNIT_HEX1:
	if (!ishexstr(dspid_in))
	    return(-1);
	if ((size_t)strlen(dspid_in) > partsz)
	    return(-1);
   	if (((size_t)strlen(dspid_in) < partsz) && (pchar != '-'))
    	    pad (dspid_in, pchar, partsz, just);
   	else
	    if ((size_t)strlen(dspid_in) != partsz)
		return(-1);
	strcpy (dspid_out, dspid_in) ;
	break;
    case DSP_UNIT_HEX2:
	if (!ishexstr(dspid_in))
	    return(-1);
	if (((size_t)strlen(dspid_in) % 2) != 0 ||
					    ((size_t)strlen(dspid_in) > partsz))
	    return(-1);
   	if (((size_t)strlen(dspid_in) < partsz)&&(pchar != '-'))
    	    pad (dspid_in, pchar, partsz, just);
   	else
	    if ((size_t)strlen(dspid_in) != partsz) 
		return(-1);
	strcpy (dspid_out,dspid_in);
	break;
    case DSP_UNIT_ASCII1:
	if (asci_to_hexdec(dspid_in, dspid_out, DSP_UNIT_ASCII1, minunits) < 0)
	    return(-1);
	sprintf (pstr, "%03d", (int)pchar) ;
   	if (((size_t)strlen(dspid_out) < partsz)&&(pchar != '-')) {
    	    if(padunit (dspid_out, pstr, partsz, just)<0)
		return(-1);
   	    else
		if ((size_t)strlen(dspid_out) != partsz)
		    return(-1);
	}
	break;
    case DSP_UNIT_ASCII2:
	if (asci_to_hexdec(dspid_in, dspid_out, DSP_UNIT_ASCII2, minunits) < 0)
	    return(-1);
	sprintf (pstr, "%02x", (int)pchar) ;
   	if (((size_t)strlen(dspid_out) < partsz)&&(pchar != '-')) {
    	    if(padunit (dspid_out, pstr, partsz, just)<0)
		return(-1);
   	    else
		if ((size_t)strlen(dspid_out) != partsz)
		    return(-1);
	}
	break;
    default:
	 printf(">>>>>>>SYSTEM ERROR \n");
	 break;
    }
    return(0);
}
