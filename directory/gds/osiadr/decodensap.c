/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: decodensap.c,v $
 * Revision 1.1.709.2  1996/02/18  18:20:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:31  marty]
 *
 * Revision 1.1.709.1  1995/12/08  16:06:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:56  root]
 * 
 * Revision 1.1.707.2  1994/09/06  12:26:09  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:22:52  keutel]
 * 
 * Revision 1.1.707.1  1994/02/22  16:11:01  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:00:37  marrek]
 * 
 * Revision 1.1.705.2  1993/10/14  17:52:26  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:42:42  keutel]
 * 
 * Revision 1.1.705.1  1993/10/13  17:33:50  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  17:42:23  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  12:34:17  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:29:38  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  20:47:41  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:33:56  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  18:09:58  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	[1992/12/04  14:00:25  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  18:11:55  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  13:34:03  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:00:08  melman
 * 	New GDS merged drop
 * 	[1992/05/28  21:52:05  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: decodensap.c,v $ $Revision: 1.1.709.2 $ $Date: 1996/02/18 18:20:47 $";
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
/* NAME         : decodensap.c                                        */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 30.03.90                                            */
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

#include <gds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dce/cmx_addr.h>
#include <dce/osiaddr.h>


/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d2p_decode_nsap_addr ()                                       */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*    The function decodes a NSAP-address and returns the AFI, the IDI       */
/*    and the components of the DSP.                                         */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*      op_mode     =  Operation mode of the decoding function. The          */
/*                     following values are valid:                           */
/*                      - DEC_IDP_ONLY   - only the IDP is decoded           */
/*                      - DEC_DSP_ONLY   - only the DSP is decoded           */
/*                      - DEC_COMPLETE   - the IDP and the DSP are encoded   */
/*      afi         =  Pointer to the variable at which the authority and    */
/*                     format identifier (AFI) should be stored.             */
/*      idi         =  Pointer to the memory reference at which the decoded  */
/*                     initial domain identifier (IDI) should be stored.     */
/*                     This pointer can be a NULL-pointer, if the operation  */
/*                     mode is equal to DEC_DSP_ONLY.                        */
/*      dsp_parts   =  Pointer to a variable at which the no. of the decoded */
/*                     components of the domain specific part (DSP) should    */
/*                     be stored.                                            */
/*      dsp_values  =  Pointer to an array, which contains itself the        */
/*                     memory references at which the decoded DSP-parts      */
/*                     (strings) should be stored.                           */
/*                     This pointer can be a NULL-pointer, if the operation  */
/*                     mode is equal to DEC_IDP_ONLY.                        */
/*	dsp_types  =   Pointer to an array of short where the type of decoded*/
/*		       parts is stored . The DSP-part types are stored as    */
/*		       described in the "osiaddr.h" header file	     	     */
/*      nsap_addr   =  Pointer to the NSAP-address which should be decoded.  */
/*      nsap_size   =  Size of the NSAP-address which should be decoded.     */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*      *afi        =  Authority and format identifier (AFI).                */
/*      *idi        =  Pointer to the location which contains the decoded    */
/*                     initial domain identifier (IDI) as a string.          */
/*                     If there is no IDI available, this pointer points     */
/*                     to an empty string.                                   */
/*      *dsp_parts  =  No. of the decoded components of the DSP.             */
/*                     If the operation mode is equal to DEC_IDP_ONLY, then  */
/*                     this parameter set to 1 if there is any DSP and it    */
/*                     is set to 0 if the is no DSP.                         */
/*      *dsp_values =  Pointer(s) to the location(s) which contain(s) the    */
/*                     decoded DSP-component(s) (strings).                   */
/*                     NOTE: This output parameter is invalid, if the        */
/*                     output parameter *dsp_parts is equal to zero or if    */
/*                     if the operation mode is equal to DEC_IDP_ONLY.       */
/*	*dsp_types  =   Pointer(s) to the location(s) which contain(s) the    */
/*		       type of the decoded parts as stored in *dsp_values    */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*      NO_ERR      = Decoding of the NSAP-address performed successfully.   */
/*      < 0         = An error is occurred during decoding of the NSAP-      */
/*                    address (in this case, the returned negative value     */
/*                    corresponds to the type of error).                     */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                        DATE: 30.03.90     */
/*exoff **********************************************************************/


signed32 d2p_decode_nsap_addr (signed32 op_mode, signed32 *afi, char *idi,
			       signed32 *dsp_parts, char *dsp_values[],
			       signed32 dsp_types[], byte *nsap_addr,
							   signed32 nsap_size) {

    signed32		i, j, l, insap_size, lidi = 0 ;
    signed32		res, dsp_partsize ;
    signed32		dspsyntax, varsize ;
    char		ld_digit, tbuf[(T_NSAPSIZE * 2) + 1] ;
    char		*tbufptr = tbuf, *p ;
    char		tmpbuf[100], unittype, pstr[100], *temp ; 
    Nsap_idpinfo	*nsap_idpptr = (Nsap_idpinfo *) NULL ;
    Nsap_dspinfo	*nsap_dspptr ;

    /* verify size of NSAP-address */
    if (nsap_size < 1 || nsap_size > T_NSAPSIZE)
	return (NSAP_SIZE_ERR) ;

    /* convert NSAP-address to an ASCII-string */
    insap_size = nsap_size << 1 ;

    decode_semi_octets (tbufptr, insap_size, insap_size, nsap_addr) ;

    /* search NSAP-address format description entry (IDP-description) */
    sscanf (tbufptr, "%2d", afi) ;
    if ((res = d2p_srch_nsap_form (-1, &nsap_idpptr, *afi, (char *) NULL,
		     (Nsap_dspinfo **) NULL, (char *) NULL, (void *) NULL)) < 0)
	return (res) ;

    /* *** decode IDI *** */
    if (nsap_idpptr->idi_size > 0) {
	/* verify NSAP-address size (IDP-part) */
	if (insap_size < nsap_idpptr->idi_size)
	    return (IDI_SIZE_ERR) ;

	/* verify syntax of IDI and skip leading digits */
	ld_digit = (*afi == nsap_idpptr->afi0) ? LZERO : LONE ;
	for (lidi = j = 0, p = tbufptr + 2; lidi < nsap_idpptr->idi_size;
								  lidi++, p++) {
	    if (!isdigit (*p))
		return (IDI_SYNTAX_ERR) ;
	    if (*p == ld_digit && j == lidi)
		j++ ;
	}
	lidi = ( j < lidi) ? lidi - j : 1 ;	
		
 	if (op_mode == DEC_IDP_ONLY || op_mode == DEC_COMPLETE ||
		op_mode == DEC_DISPLAY) {
	    strncpy (idi, p - lidi, lidi) ;
	    *(idi + lidi) = '\0' ;
	}
    } else {
 	if (op_mode == DEC_IDP_ONLY || op_mode == DEC_COMPLETE ||
		op_mode == DEC_DISPLAY)
	    *idi = '\0' ;
    }

    tbufptr += nsap_idpptr->idi_size + 2 ;
    insap_size -= nsap_idpptr->idi_size + 2 ;
    if ((nsap_idpptr->idi_size % 2) != 0) {
	tbufptr++ ;
	insap_size-- ;
    }
	
    *dsp_parts = 0 ;
    if (op_mode == DEC_IDP_ONLY) {
	if (insap_size > 1) {
	    *dsp_parts = 1 ;
	    strcpy (dsp_values[0], tbufptr) ;
	    dsp_types[0] = -1 ;
	}
	return (NO_ERR) ;
    }

    /* *** decode DSP *** */
    if (insap_size > 0) {
	/* search NSAP-address format description entry (DSP-description) */
	DBGPRINT ((DEV, "fname == %s", nsap_dspptr->dsp_fname)) ;
	if ((res = d2p_srch_nsap_form (-1, &nsap_idpptr, (signed32) NULL,
		       (char *) NULL, &nsap_dspptr, idi, (void *) tbufptr)) < 0)
	    return (res) ;

	DBGPRINT ((DEV, "Format Found ....")) ;
	/* decode DSP value(s) from NSAP-address */
	dspsyntax = GET_DSPSYN(nsap_idpptr->geninfo) ;
	if (dspsyntax == ISO_DSP_SYNTAX) 
	    /* ISO 646 character DSP-syntax */
	    /* not supported */
	    return (UNSUPPORTED_SYNTAX_ERR) ;
	else if (dspsyntax == NAT_DSP_SYNTAX)
	    /* National character DSP-syntax */
	    /* not supported */
	    return (UNSUPPORTED_SYNTAX_ERR) ;
	else if (dspsyntax == BIN_DSP_SYNTAX) {
	  l = (signed32) strlen (tbufptr) ;
	  for (j=0, p = tbufptr; j < l; j++, p++)
		if (!isxdigit(*p))
		   return(DSP_SYNTAX_ERR);
	}
	else if (dspsyntax == DEC_DSP_SYNTAX)	{
	  l = (signed32) strlen (tbufptr) ;
	  for (j=0, p = tbufptr; j < l; j++, p++)
		if (!isxdigit(*p))
		   return(DSP_SYNTAX_ERR);
	}
	else
	    /* unknown DSP-syntax */
	    return (UNKNOWN_SYNTAX_ERR) ;


	for (i =0; i < nsap_dspptr->dsp_parts ;i++) {
		dsp_partsize = (signed32) nsap_dspptr->dsp_partsize[i];
	  	if (GET_DSP_OPTIONAL(nsap_dspptr->dspflag[i]) && 
			     (((tbufptr[0] == '\0') 
			   || (tbufptr[0] == ALIGN_VAL_CHAR)
			   || (tbufptr[0] == tolower(ALIGN_VAL_CHAR)))
		        && (tbufptr[1] =='\0') 
			&& (dspsyntax == DEC_DSP_SYNTAX))) 
		        break;
	  	else if (tbufptr[0] == '\0') return(DSP_SYNTAX_ERR);
		unittype = GET_DSP_UNIT(nsap_dspptr->dspflag[i]); 
		varsize  = GET_DSP_VAR_SIZE(nsap_dspptr->dspflag[i]); 
  		if (GET_DSP_VAR_ID(nsap_dspptr->dspflag[i])) {
		      if (varsize == DSP_VAR_SIZE) {
			  if(!vardsppart (tbufptr, nsap_dspptr->dspflag[i],
				     dspsyntax,&temp,dsp_values[i],
				     nsap_dspptr->dsp_partsize[i]))
				return(DSP_VAR_PART_ERR);
		          insap_size -= (size_t)(temp - tbufptr) ;
			  tbufptr = temp;
			}
		      else { 
			   l = (signed32) strlen (tbufptr) ;
			   if ((tbufptr[l-1] == ALIGN_VAL_CHAR
			      || tbufptr[l-1] == 
				    		      tolower(ALIGN_VAL_CHAR))
			      && (dspsyntax == DEC_DSP_SYNTAX))
					tbufptr[l-1] = '\0';  
		       	      strcpy(dsp_values[i],tbufptr);
		              insap_size -= (signed32) strlen(tbufptr) ;
			      tbufptr += strlen(tbufptr);
			   if (tbufptr[0] != '\0') return(DSP_VAR_PART_ERR);
		       }
		    }
		else { /* Not variable part */
		  	if (GET_DSP_OPTIONAL(nsap_dspptr->dspflag[i])) {
			        l = (signed32) strlen (tbufptr) ;
			        if ((tbufptr[l-1] == 
							ALIGN_VAL_CHAR
			         || tbufptr[l-1] == 
				    			tolower(ALIGN_VAL_CHAR))
			         && (dspsyntax == DEC_DSP_SYNTAX))
					tbufptr[l-1] = '\0';  
				strcpy(dsp_values[i],tbufptr);
				tbufptr += strlen(tbufptr);
		  	}
		  	else {
		   		strncpy(dsp_values[i],tbufptr,dsp_partsize);
		   		*(dsp_values[i] + dsp_partsize) = '\0';
		   		tbufptr += dsp_partsize ;
		   	     }
		   DBGPRINT ((DEV, "dsp_val = %s dsp_partsz = %d\n", dsp_values[i], dsp_partsize)) ;
		   insap_size -= dsp_partsize ;
		    if (GET_PADCHAR(nsap_dspptr->dspflag[i]) != '-') {
			 if (unittype == DSP_UNIT_ASCII1)  {
	    		        sprintf (pstr, "%03d",
				    GET_PADCHAR(nsap_dspptr->dspflag[i]));
		       		 removepadstr(dsp_values[i], pstr,
		  		 GET_DSP_JUST(nsap_dspptr->dspflag[i]));
				}
			 else if (unittype == DSP_UNIT_ASCII2)  {
	    		       sprintf (pstr, "%02x", 
				    GET_PADCHAR(nsap_dspptr->dspflag[i]));
		       		 removepadstr(dsp_values[i], pstr,
		  		 GET_DSP_JUST(nsap_dspptr->dspflag[i]));
				}
			 else {
			      if ((unittype != DSP_UNIT_DOT1) && 
		                  (unittype != DSP_UNIT_DOT2) &&
		                  (unittype != DSP_UNIT_SLASH11) &&
		                  (unittype != DSP_UNIT_SLASH12) &&
		                  (unittype != DSP_UNIT_SLASH21) &&
		                  (unittype != DSP_UNIT_SLASH22)) {
		       		    removepad(dsp_values[i],
		  		    GET_PADCHAR(nsap_dspptr->dspflag[i]),
		  		    GET_DSP_JUST(nsap_dspptr->dspflag[i]),
				    unittype);
				 DBGPRINT ((DEV, "Entered removepad\n" )) ;
				 }
			      else if ((unittype == DSP_UNIT_DOT1) ||
				      (unittype == DSP_UNIT_SLASH11))
				    removepadstr(dsp_values[i],
				    "000",DSP_RIGHT_JUST);
			      else if ((unittype == DSP_UNIT_DOT2) ||
				       (unittype == DSP_UNIT_SLASH21))
				    removepadstr(dsp_values[i],
				    "00",DSP_RIGHT_JUST);
			      else if (unittype == DSP_UNIT_SLASH12)
				    removepadstr(dsp_values[i],
				    "00000",DSP_RIGHT_JUST);
			      else if (unittype == DSP_UNIT_SLASH22)
				    removepadstr(dsp_values[i],
				    "0000",DSP_RIGHT_JUST);


			     }
	  	     }
	 	     } /* End of not variable part */
	         if (insap_size < 0) {
		   DBGPRINT ((DEV, "insap_size (%d)\n", insap_size)) ;
		    return(DSP_SYNTAX_ERR);
		}

		DBGPRINT ((DEV, "Remaining string = %s\n", tbufptr)) ;
		DBGPRINT ((DEV, "i = %d dsp_value == %s\n\n", i,dsp_values[i])) ;
		switch (dsp_types[i] = GET_DSP_UNIT(nsap_dspptr->dspflag[i])) {

			case DSP_UNIT_DEC1:
				if (!isdigitstr(dsp_values[i])) 
			     	    return(DSP_SYNTAX_ERR);
	    			if (((dsp_partsize = strlen (dsp_values[i])) > 
	 		     	    nsap_dspptr->dsp_partsize[i]) || 
			  (dsp_partsize < nsap_dspptr->dsp_partsize_min[i]) || 
					dsp_partsize == 0)
				     return (DSP_SIZE_ERR) ;
				 break;

			case DSP_UNIT_DEC2:
		  		if (dspsyntax != BIN_DSP_SYNTAX) 
			     		return(DSP_UNIT_MISMATCH);
				if (!isdigitstr(dsp_values[i])) 
			     		return(DSP_SYNTAX_ERR);
	    			if ((dsp_partsize = strlen (dsp_values[i])) > 
	 		    	 nsap_dspptr->dsp_partsize[i] || 
			   (dsp_partsize < nsap_dspptr->dsp_partsize_min[i]) || 
				dsp_partsize == 0)
			     		return (DSP_SIZE_ERR) ;
		       		if ((dsp_partsize % 2) != 0)
	    				return (DSP_SYNTAX_ERR) ;
				break;

			case DSP_UNIT_HEX1:
		  		if (dspsyntax != BIN_DSP_SYNTAX) 
			     		return(DSP_UNIT_MISMATCH);
				if (!ishexstr(dsp_values[i])) 
			     		return(DSP_SYNTAX_ERR);
	    			if ((dsp_partsize = strlen (dsp_values[i])) > 
	 		     	nsap_dspptr->dsp_partsize[i] || 
			(dsp_partsize < nsap_dspptr->dsp_partsize_min[i]) || 
				dsp_partsize == 0)
			     		return (DSP_SIZE_ERR) ;
				break;

			case DSP_UNIT_HEX2:
		  		if (dspsyntax != BIN_DSP_SYNTAX) 
					return(DSP_UNIT_MISMATCH);
				if (!ishexstr(dsp_values[i])) 
			     		return(DSP_SYNTAX_ERR);
	    			if ((dsp_partsize = strlen (dsp_values[i])) > 
	 		     	nsap_dspptr->dsp_partsize[i] || 
			(dsp_partsize < nsap_dspptr->dsp_partsize_min[i]) || 
				dsp_partsize == 0)
			     	return (DSP_SIZE_ERR) ;
		       		if ((dsp_partsize % 2) != 0)
	    				return (DSP_SYNTAX_ERR) ;
				break;
				
			case DSP_UNIT_ASCII1:
			        if (op_mode == DEC_DISPLAY) {
				 if(hexdec_to_asci(dsp_values[i],tmpbuf,
						DSP_UNIT_ASCII1,
				   nsap_dspptr->dsp_partsize_min[i]) < 0) 
				   return(DSP_CONVERSION_FAILED);
				strcpy(dsp_values[i],tmpbuf);
				}
				break;

			case DSP_UNIT_ASCII2:
			  	if (dspsyntax != BIN_DSP_SYNTAX) 
					return(DSP_UNIT_MISMATCH);
				if (op_mode == DEC_DISPLAY) {
				 if(hexdec_to_asci(dsp_values[i],tmpbuf,
						DSP_UNIT_ASCII2,
				   nsap_dspptr->dsp_partsize_min[i])<0)
				      return(DSP_CONVERSION_FAILED);
				strcpy(dsp_values[i],tmpbuf);
				}
			        break;

			case DSP_UNIT_DOT1:
				if (op_mode == DEC_DISPLAY) {
				 if(hexdec_to_dotdec(dsp_values[i],tmpbuf,
				DSP_UNIT_DOT1,atoi(nsap_dspptr->dsp_lowval[i]),
				atoi(nsap_dspptr->dsp_highval[i]),
				nsap_dspptr->dsp_partsize_min[i]) < 0) 
				      return(DSP_CONVERSION_FAILED);
				strcpy(dsp_values[i],tmpbuf);
				}
				break;

			case DSP_UNIT_DOT2:
			  	if (dspsyntax != BIN_DSP_SYNTAX) 
					return(DSP_UNIT_MISMATCH);
				if (op_mode == DEC_DISPLAY) {
				 if(hexdec_to_dotdec(dsp_values[i],tmpbuf,
				 DSP_UNIT_DOT2,atoi(nsap_dspptr->dsp_lowval[i]),
				 atoi(nsap_dspptr->dsp_highval[i]),
				 nsap_dspptr->dsp_partsize_min[i]) < 0) 
					return(DSP_CONVERSION_FAILED);
				strcpy(dsp_values[i],tmpbuf);
				}
				break;

			case DSP_UNIT_SLASH11:
				if (op_mode == DEC_DISPLAY) {
				 if(hexdec_to_slashdec(dsp_values[i],tmpbuf,
				 DSP_UNIT_SLASH11,
				 atoi(nsap_dspptr->dsp_lowval[i]),
				 atoi(nsap_dspptr->dsp_highval[i]),
				 nsap_dspptr->dsp_partsize_min[i]) < 0) 
				   	return(DSP_CONVERSION_FAILED);
				strcpy(dsp_values[i],tmpbuf);
				 }
				break;

			case DSP_UNIT_SLASH12:
				if (op_mode == DEC_DISPLAY) {
				 if(hexdec_to_slashdec(dsp_values[i],tmpbuf,
				 DSP_UNIT_SLASH12,
				 atoi(nsap_dspptr->dsp_lowval[i]),
				 atoi(nsap_dspptr->dsp_highval[i]),
				 nsap_dspptr->dsp_partsize_min[i]) < 0) 
					return(DSP_CONVERSION_FAILED);
				strcpy(dsp_values[i],tmpbuf);
				}
				break;

			case DSP_UNIT_SLASH21:
			  	if (dspsyntax != BIN_DSP_SYNTAX) 
					return(DSP_UNIT_MISMATCH);
				if (op_mode == DEC_DISPLAY) {
				 if(hexdec_to_slashdec(dsp_values[i],tmpbuf,
				 DSP_UNIT_SLASH21,
				 atoi(nsap_dspptr->dsp_lowval[i]),
				 atoi(nsap_dspptr->dsp_highval[i]),
				 nsap_dspptr->dsp_partsize_min[i]) < 0) 
					return(DSP_CONVERSION_FAILED);
				strcpy(dsp_values[i],tmpbuf);
				 }
			   	break;

			case DSP_UNIT_SLASH22:
			  	if (dspsyntax != BIN_DSP_SYNTAX) 
					return(DSP_UNIT_MISMATCH);
				if (op_mode == DEC_DISPLAY) {
				 if(hexdec_to_slashdec(dsp_values[i],tmpbuf,
				 DSP_UNIT_SLASH22,
				 atoi(nsap_dspptr->dsp_lowval[i]),
				 atoi(nsap_dspptr->dsp_highval[i]),
				 nsap_dspptr->dsp_partsize_min[i]) < 0) 
					return(DSP_CONVERSION_FAILED);
				strcpy(dsp_values[i],tmpbuf);
				 }
				break;

			default :
		        	return(DSP_UNKNOWN_UNIT);
   			 } /* END OF SWITCH */
	     	   (*dsp_parts)++;
		} /* END OF FOR LOOP */
	      } /* END OF IF STAT. */
    return (NO_ERR) ;
}
