/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: convert.c,v $
 * Revision 1.1.108.2  1996/02/18  18:20:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:43:29  marty]
 *
 * Revision 1.1.108.1  1995/12/08  16:06:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:16:55  root]
 * 
 * Revision 1.1.106.4  1994/09/06  12:26:08  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:22:46  keutel]
 * 
 * Revision 1.1.106.3  1994/06/21  14:47:47  marrek
 * 	June 1994 code submission.
 * 	[1994/06/17  13:00:17  marrek]
 * 
 * Revision 1.1.106.2  1994/06/09  18:47:18  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:20:29  devsrc]
 * 
 * Revision 1.1.106.1  1994/02/22  16:10:58  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:00:21  marrek]
 * 
 * Revision 1.1.104.2  1993/10/14  17:52:24  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:42:36  keutel]
 * 
 * Revision 1.1.104.1  1993/10/13  17:33:48  keutel
 * 	October 1993 code drop
 * 	[1993/10/09  18:51:21  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: convert.c,v $ $Revision: 1.1.108.2 $ $Date: 1996/02/18 18:20:45 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
*****************************************************************************/
/*daton **********************************************************************/
/*                                                                           */
/*   TYPE:    Functions                                                       */
/*                                                                           */
/*   NAME:    convert.c                                                      */
/*                                                                           */
/*   SHORT DESCRIPTION:                                                      */
/*	This file contains many general purpose functions that are described */
/*	at the respective places					     */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*    Author: Kapur Sandeep                               Date: 14-06-93     */
/*datoff *********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dce/osiaddr.h>

/* -----------------------DECLARATION OF CONSTANTS --------------------------*/

#define		FAIL			0
#define		SUCCESS		        1
#define		BUFFER_STR		500
#define		QUOTE			'\''
#define		dotted  		 "."
#define		slashed			 "/"
#define		SLASH			 '/'
#define		DOT	  		 '.'

/* ------------------------------------------------------------------------ */
/*    CONVERT A STRING FROM A DOTTED DECIMAL- INTO A HEX- OR DECIMAL-       */
/*                          REPRESENTATION                                  */
/*     e.g. 127.0.0.1  --->  7F000001   or --->  127000000001               */
/* ------------------------------------------------------------------------ */

signed32 dotdec_to_hexdec (char *pin, char *pout, char unittype, char rangetype,
			       char *lowval, char *highval, signed32 minunits) {

    char	*p, *p1, tmpbuf[10] ;
    signed32	single_value, dspunitlen, nounits = 0 ;

    if (!dotted_dec_str (pin))
	return (DSP_WRONG_UNIT) ;

    minunits /= DOT_UNIT ;
    if (unittype == DSP_UNIT_DOT1)
	dspunitlen = 3 ;
    else 
	dspunitlen = 2 ;

    /* convert input string */
    for (p = pin, p1 = pout; p != (char *) NULL; p1 += dspunitlen) {
	if ((p = strtok ((p1 == pout) ? p : (char *) NULL, dotted)) 
							     != (char *) NULL) {
	    nounits++;
	    if (strlen (p) == (size_t) 0)
		continue ;
	    single_value = atoi (p) ;
	    strcpy (tmpbuf, p) ;
	    if ((size_t) DOT_UNIT < strlen (tmpbuf))
		return (DSP_OUTOF_RANGE); 
	    pad (tmpbuf, LZERO, DOT_UNIT, DSP_RIGHT_JUST) ;
	    if ((signed32) strlen (tmpbuf) > (signed32) strlen (DOT_LOW) || 
		      (signed32) strlen (tmpbuf) > (signed32) strlen (DOT_HIGH))
		return (DSP_OUTOF_RANGE) ;
	    if (strcmp (DOT_LOW, tmpbuf) > 0 || strcmp (DOT_HIGH, tmpbuf) < 0) 
		return (DSP_OUTOF_RANGE) ;
	    if (rangetype == DSP_RANGE) { 
		if (strcmp (lowval, tmpbuf) > 0 || strcmp (highval, tmpbuf) < 0) 
		  return (DSP_OUTOF_RANGE) ;
	    }
	    sprintf (p1, (unittype == DSP_UNIT_DOT2)? "%02x" : "%03d",
								 single_value) ;
	}
    }
    if (nounits < minunits)
	return (DSP_MIN_UNIT_ERR) ;
    return (NO_ERR) ;
}

/* ------------------------------------------------------------------------ */
/* CONVERT A STRING FROM A HEX/DECIMAL- INTO A DOTTED DECIMAL-REPRESENTATION*/
/*           e.g. FF60000A resp. 255096000010 --->  255.96.0.10             */
/* ------------------------------------------------------------------------ */

signed32 hexdec_to_dotdec (char *pin, char *pout, signed32 unittype,
			 signed32 lowval, signed32 highval, signed32 minunits) {

    signed32	single_value, len, nounits = 0 ;
    char	*p, *p1, tmpbuf[BUFFER_STR] ;

    if (unittype == DSP_UNIT_DOT2) {
	/* verify input string syntax */
	if (((len = (signed32) strlen (pin)) % 2) != 0)
	    return (DSP_UNIT_IMPOSSIBLE) ;
	minunits /= 2 ;

	if (!ishexstr (pin))
	    return (DSP_WRONG_CHAR) ;
	/* convert input string */
	for (p = pin, p1 = pout, *p1 = '\0'; *p != '\0'; p += 2,
							    p1 += strlen (p1)) {
	    sscanf (p, "%2x", &single_value) ;
	    if ((p != pin)&&(nounits != 0)) 
		sprintf (p1, "%c%d", DOT, single_value) ;
	    else
		sprintf (p1, "%d", single_value) ;
	    ++nounits ;
	}
    } else {
	/* verify input string syntax */
	if (!isdigitstr (pin))
	    return (DSP_WRONG_CHAR) ;
	minunits /= 3 ;
	/* write leading filler values to string */
	strcpy (tmpbuf, pin) ;

	/* convert input string */
	for (p = tmpbuf, p1 = pout, *p1 = '\0'; *p != '\0';
						    p += 3, p1 += strlen (p1)) {
	    sscanf (p, "%3d", &single_value) ;
	    if ((p != tmpbuf) && (nounits != 0))
		sprintf (p1, "%c%d", DOT, single_value) ;
	    else
		sprintf (p1, "%d", single_value) ;
	    ++nounits;
	}
    }
    if (nounits < minunits)
	return (DSP_MIN_UNIT_ERR) ;
    return (NO_ERR) ;
}

/* ------------------------------------------------------------------------ */
/*    CONVERT A STRING FROM A SLASHED DECIMAL- INTO A HEX- OR DECIMAL-      */
/*                          REPRESENTATION                                  */
/*     e.g. 25/155  --->  199B   or --->  025155  OR	                    */
/*     e.g. 425/1631 ---> 01A9065F   or --->  0042501631	            */
/* ------------------------------------------------------------------------ */

signed32 slashdec_to_hexdec (char *pin, char *pout, char unittype,
	       char rangetype, char *lowval, char *highval, signed32 minunits) {

    char	*p, *p1, tmpbuf[40] ;
    signed32	single_value, nounits = 0, dspunitlen = 0 ;

    if (!slashed_dec_str (pin))
	return (DSP_WRONG_UNIT) ;

    if (unittype == DSP_UNIT_SLASH11) {
	dspunitlen = 3 ;
	minunits /= SLASH1_UNIT ;
    } else {
	if (unittype == DSP_UNIT_SLASH12) {
	    dspunitlen = 5 ;
	    minunits /=  SLASH2_UNIT ;
	} else
	    if (unittype == DSP_UNIT_SLASH21) {
		dspunitlen = 2 ;
		minunits /=  SLASH1_UNIT ;
	    } else
		if (unittype == DSP_UNIT_SLASH22) {
		    dspunitlen = 4 ;
		    minunits /= SLASH2_UNIT ;
    		} else
		    return (DSP_WRONG_UNIT) ;

    }
    /* convert input string */
    for (p = pin, p1 = pout; p != (char *) NULL; p1 += dspunitlen) {
	if ((p = strtok ((p1 == pout) ? p : NULL, slashed)) != (char *) NULL) {
	    if ((size_t)strlen (p) == (size_t) 0)
		continue ;
	    ++nounits;
	    single_value = atoi (p) ;
	    strcpy (tmpbuf, p) ;
	    if (unittype == DSP_UNIT_SLASH11 || unittype == DSP_UNIT_SLASH21) {
		if (strlen (tmpbuf) > (size_t) 3)
		    return (DSP_OUTOF_RANGE) ;
		else
	   	    pad (tmpbuf, LZERO, 3, DSP_RIGHT_JUST) ;
	    } else {
		if (strlen (tmpbuf) > (size_t) 5)
		    return (DSP_OUTOF_RANGE) ;
		else
	   	    pad (tmpbuf, LZERO, 5, DSP_RIGHT_JUST) ;
	    }
	    if ((unittype == DSP_UNIT_SLASH11 || unittype == DSP_UNIT_SLASH21)
		&& ((signed32) strlen (tmpbuf) > (signed32) strlen (SLASH1_LOW) ||
		  (signed32) strlen (tmpbuf) > (signed32) strlen (SLASH1_HIGH)))
	        return (DSP_OUTOF_RANGE) ;
	    else
		if ((unittype == DSP_UNIT_SLASH12 ||
		     unittype == DSP_UNIT_SLASH22) &&
		     ((signed32) strlen (tmpbuf) > (signed32) strlen (SLASH2_LOW) ||
		  (signed32) strlen (tmpbuf) > (signed32) strlen (SLASH2_HIGH)))
		    return (DSP_OUTOF_RANGE) ;

	    if ((unittype == DSP_UNIT_SLASH11 || unittype == DSP_UNIT_SLASH21)
		&& (strlen (tmpbuf) == strlen (SLASH1_LOW) &&
		strcmp (SLASH1_LOW, tmpbuf) > 0) ||
		(strlen (tmpbuf) == strlen (SLASH2_HIGH) &&
					      strcmp (SLASH1_HIGH, tmpbuf) < 0))
	        return (DSP_OUTOF_RANGE) ;
	    else {
		if ((strlen (tmpbuf) == strlen (SLASH2_LOW) &&
		    strcmp (SLASH2_LOW, tmpbuf) > 0) ||
		    (strlen (tmpbuf) == strlen (SLASH2_HIGH) &&
					      strcmp (SLASH2_HIGH, tmpbuf) < 0))
		    return (DSP_OUTOF_RANGE) ;
	    }

	    if (rangetype == DSP_RANGE) { 
		if ((strcmp (lowval, tmpbuf) > 0) ||
						 (strcmp (highval, tmpbuf) < 0))
		    return (DSP_OUTOF_RANGE) ;
	    }
	    sprintf (p1, ((unittype == DSP_UNIT_SLASH21) ||
			  (unittype == DSP_UNIT_SLASH22) ?
			 ((unittype == DSP_UNIT_SLASH21) ? "%02x":"%04x") :
			 ((unittype == DSP_UNIT_SLASH11) ? "%03d":"%05d")),
								 single_value) ;
	}
    }  /* END of for loop */
    if (nounits < minunits)
	return (DSP_MIN_UNIT_ERR) ;
    return (NO_ERR) ;
}

/* ------------------------------------------------------------------------ */
/* CONVERT A STRING FROM A HEX/DECIMAL- INTO A SLASHED DECIMAL-REPRESENTATION*/
/*           e.g. 199B OR 025155 --->  25/155			             */
/*		  01A9065F OR 0042501631 ---> 425/1631			     */
/* ------------------------------------------------------------------------ */

signed32 hexdec_to_slashdec (char *pin, char *pout, char unittype,
			 signed32 lowval, signed32 highval, signed32 minunits) {

    signed32	single_value, len, nounits = 0, numofdigits = 1 ; 
    char	*p, *p1, tmpbuf[BUFFER_STR] ;

    if (unittype == DSP_UNIT_SLASH21 || unittype == DSP_UNIT_SLASH22) {
	/* verify input string syntax */
	if (unittype == DSP_UNIT_SLASH21 &&
				     ((len = (signed32) strlen (pin)) % 2) != 0)
	    return (DSP_UNIT_IMPOSSIBLE) ;
	if (unittype == DSP_UNIT_SLASH22 &&
				     ((len = (signed32) strlen (pin)) % 4) != 0)
	    return (DSP_UNIT_IMPOSSIBLE) ;
	if (unittype == DSP_UNIT_SLASH22) 
	    numofdigits = 4 ;
	else
	    numofdigits = 2 ;

	minunits /= numofdigits ;

	if (!ishexstr (pin))
	    return (DSP_WRONG_CHAR) ;
	/* convert input string */
	for (p = pin, p1 = pout, *p1 = '\0'; *p != '\0';
					  p += numofdigits, p1 += strlen (p1)) {
	    sscanf (p, (unittype == DSP_UNIT_SLASH21) ? "%2x":"%4x",
								&single_value) ;
	    if (p != pin && nounits != 0)
		sprintf (p1, "%c%d", SLASH, single_value) ;
	    else
		sprintf (p1, "%d", single_value) ;
	    ++nounits ;
	}
    } else {
	/* verify input string syntax */
	if (unittype == DSP_UNIT_SLASH11 &&
				     ((len = (signed32) strlen (pin)) % 3) != 0)
	    return (DSP_UNIT_IMPOSSIBLE) ;
	if (unittype == DSP_UNIT_SLASH12 &&
				     ((len = (signed32) strlen (pin)) % 5) != 0)
	    return (DSP_UNIT_IMPOSSIBLE) ;

	if (unittype == DSP_UNIT_SLASH12)
	    numofdigits = 5 ;
	else
	    numofdigits = 3 ;

	minunits /= numofdigits ;

        if (!isdigitstr (pin))
	    return (DSP_WRONG_CHAR) ;
	   
	strcpy (tmpbuf, pin) ;

	/* convert input string */
	for (p = tmpbuf, p1 = pout, *p1 = '\0'; *p != '\0';
					  p += numofdigits, p1 += strlen (p1)) {
	    sscanf (p, (numofdigits == 3) ? "%3d":"%5d", &single_value) ;
	    if (p != tmpbuf && nounits != 0)
		sprintf (p1, "%c%d", SLASH, single_value) ;
	    else
		sprintf (p1, "%d", single_value) ;
	    ++nounits ;
	}
    }
    if (nounits < minunits)
	return (DSP_MIN_UNIT_ERR) ;
    return (NO_ERR) ;
}

/* ------------------------------------------------------------------------ */
/*  CONVERT A STRING FROM AN ASCII- INTO HEXADECIMAL/DECIMAL-REPRESENTATION */
/*         e.g. 'Example' ---> 4578616D706C65 resp. 069120097109112108101   */
/* ------------------------------------------------------------------------ */

signed32 asci_to_hexdec (char *pin, char *pout, char unittype, char minunits) {

    signed32	nounit = 0, i, len = (signed32) strlen (pin) - 1 ;

    if (!isASCIIstr (pin))
	return (DSP_WRONG_UNIT) ;

    if (unittype == DSP_UNIT_ASCII2) {
	minunits /= 2 ;
	for (i = 1, pin++; i < len; i++, pin++) {
	    ++nounit ;
	    sprintf (pout, "%02x", (int) *pin) ;
	    pout += 2 ;
	}
    } else {
	/* decimal DSP-syntax or Binary Syntax  */
	minunits /= 3 ;
	for (i = 1, pin++; i < len; i++, pin++) {
	    ++nounit ;
	    sprintf (pout, "%03d", (signed32) *pin) ;
	    pout += 3 ;
	}
    }
    if (nounit < minunits)
	return (DSP_MIN_UNIT_ERR) ;
    return (NO_ERR) ;
}

/* ------------------------------------------------------------------------ */
/* CONVERT A STRING FROM A HEXADECIMAL/DECIMAL- INTO AN ASCII-REPRESENTATION*/
/*         e.g. 4578616D706C65 resp. 069120097109112108101 ---> 'Example'   */
/* ------------------------------------------------------------------------ */

signed32 hexdec_to_asci (char *pin, char *pout, signed32 unittype,
							    signed32 minunits) {

    signed32	len = (signed32) strlen (pin) ;
    signed32	singlechar, nounits = 0 ;
    char	*p = pin ; 

    *pout++ = QUOTE ;
    if (unittype == DSP_UNIT_ASCII2) {
	if (((len = (signed32) strlen (pin)) % 2) != 0)
	    return (DSP_UNIT_IMPOSSIBLE) ;
	minunits /= 2 ;
	if (!ishexstr (p))
	    return (DSP_WRONG_CHAR) ;
	while (len > 0) {
	    sscanf (pin, "%2x", &singlechar) ;
	    sprintf (pout++, "%c", (char) singlechar) ;
	    nounits++ ;
	    len -= 2 ;
	    pin += 2 ;
	}
    } else {
	/* decimal DSP-syntax */
	if (((len = (signed32) strlen (pin)) % 3) != 0)
	    return (DSP_UNIT_IMPOSSIBLE) ;
	minunits /= 3 ;
	if (!isdigitstr (p))
	    return (DSP_WRONG_CHAR) ;
	while (len > 0) {
	    sscanf (pin, "%3d", &singlechar) ;
	    sprintf (pout++, "%c", (char) singlechar) ;
	    nounits++ ;
	    len -= 3 ;
	    pin += 3 ;
	}
    }
    *pout++ = QUOTE ;
    *pout++ = '\0' ;
    if (nounits < minunits)
	return (DSP_MIN_UNIT_ERR) ;
    return (NO_ERR) ;
}
/*****************************************************************************/
/* These functions will make the various validations .. The names of the     */
/* functions are self explanatory				             */
/*****************************************************************************/

boolean	isprintstr (char *str) {

    signed32 i ;

    for (i = 0; i < (signed32) strlen (str); i++)
	if (!isprint (str[i]))
	    return (FALSE) ;
    return (TRUE) ;
}	

boolean	isdigitstr (char *str) {

    signed32 i ;

    for (i = 0; i< (signed32) strlen (str); i++)
	if (!isdigit (str[i]))
	    return (FALSE) ;
    return (TRUE) ;
}	

boolean	ishexstr (char *str) {

    signed32 i ;

    for (i = 0; i < (signed32) strlen (str); i++)
	if (!isxdigit (str[i]))
	    return (FALSE) ;
    return (TRUE) ;
}	

boolean	isASCIIstr (char *str) {

    char	tmpstr[BUFFER_STR] ;

    if (str[0] != QUOTE)
	return (FALSE) ;
    strncpy (tmpstr, &(str[1]), strlen ((&str[1])) - (size_t) 1) ;
    tmpstr[strlen (str) - (size_t) 2] = '\0' ;
    if (!isprintstr (tmpstr))
	return (FALSE) ;
    if (str[strlen (str) - (size_t) 1] != QUOTE)
	return (FALSE) ;
    return (TRUE) ;
}	

boolean	dotted_dec_str (char *str) {

    char tmpstr[BUFFER_STR], *ptr ;

    strcpy (tmpstr, str) ;

    ptr = strtok (tmpstr, dotted) ;

    while (ptr != (char *) NULL) /* EOL not reached */
	if (!isdigitstr (ptr))
	    return (FALSE) ; /* Not a digit string */
	else 
	    ptr = strtok ((char *) NULL, dotted) ;

    return (TRUE) ;
}

boolean slashed_dec_str (char *str) {

    char tmpstr[BUFFER_STR], *ptr ;

    strcpy (tmpstr, str) ;

    ptr = strtok (tmpstr, slashed) ; /* get first token */

    while (ptr != (char *) NULL) /* EOL not reahed */
	if (!isdigitstr (ptr))
	    return (FALSE) ;
	else
	    ptr = strtok ((char *) NULL, slashed) ;

    return (TRUE) ;
}

/*****************************************************************************/
/* The function will pad the dsp_part depending upon following parameters    */
/* 								             */
/* str : The string that should be padded (INPUT/OUTPUT)	             */
/*									     */
/* pchar : The character that should be used as padding character	     */
/*									     */
/* length : The maximum allowed length of input string i.e. upto this length */
/*	    the padding will be done					     */
/*									     */
/* just : This parameters tells whether the str. should be right justified   */
/*	  or left justified i.e. it should be padded right or left (!!)      */
/*****************************************************************************/

signed32 pad (char *str, char pchar, signed32 length, signed32 just) {

    signed32	i ;
    char	tmp[200] ;

    if (length < (signed32) strlen (str))
	return (-1) ;
    for (i = 0; i < length - (signed32) strlen (str); i++) {
	tmp[i] = pchar ;
    }
    tmp[i] = '\0' ;
    if (just == DSP_LEFT_JUST)
	strcat (str, tmp) ;
    else {
	strcat (tmp, str) ;
	strcpy (str, tmp) ;
    }
    return (0) ;
}

/*****************************************************************************/
/*									     */
/* This function will pad the strings with other strings depending upon foll-*/
/* -owing :								     */
/*									     */
/* str 	: Input string that is to be padded with other string (Padding string*/
/*									     */
/* pstr : The string that should be used as padding string		     */
/*									     */
/* length : The max. length of the input string i.e. the length upto which   */
/*	    the string must be padded 					     */
/*									     */
/* just : Flag to indicate left or rigth padding 			     */
/*									     */
/*****************************************************************************/

signed32 padunit (char *str, char *pstr, signed32 length, signed32 just) {

    signed32	i ;
    char	tmp[200] ;

    memset ((void *) tmp, (int) '\0', (size_t) 200) ;
    if ((length - (signed32) strlen (str)) % (signed32) strlen (pstr) != 0)
	return (-1) ;

    for (i = 0;i < (length -(signed32) strlen (str))/(signed32) strlen (pstr);
 									    i++)
	strcat (tmp, pstr) ;
    if (just == DSP_LEFT_JUST)
	strcat (str, tmp) ;
    else {
	strcat (tmp, str) ;
	strcpy (str, tmp) ;
    }
    return (0) ;
}

/*****************************************************************************/
/*									     */
/* the function will remove if any padding characters from the input string  */
/*									     */
/* str : Input string from which padding characters are to be removed        */
/*									     */
/* pchar : The characters that should be treated as padding character        */
/*									     */
/*									     */
/* just : Whether padding characters are to be removed from left or right    */
/*									     */
/* dspsyntax : DSP SYNTAX OF THE DSP 					     */
/*****************************************************************************/

void removepad (char *str, char pchar, char just, char unittype) {

    signed32	i, len ;
    char	tmp[200] ;

    if (just == DSP_LEFT_JUST) {
	for (i = (signed32) (strlen (str) - (size_t) 1); i >= 0 ; --i) {
	    if (str[i] == pchar)
		 continue;
	    else {
		strncpy (tmp, str, i+1) ;
		tmp[i+1] = '\0' ;
		if (unittype == DSP_UNIT_HEX2 || unittype == DSP_UNIT_DEC2) { 
	 	    if ((strlen (tmp) % (size_t) 2) != (size_t) 0) {
			len = (signed32) strlen (tmp) ;
			tmp[len] = pchar ;
			tmp[len+1] = '\0' ;
		    }
		}
		strcpy (str, tmp) ;
		break;
	    }
	}
	if (i < 0) { 
	    if (unittype == DSP_UNIT_HEX2 || unittype == DSP_UNIT_DEC2)
		strcpy (str, "00") ;
	    else
		strcpy (str, "0") ;
	}
    } else {
	len = (signed32) strlen (str) ;
	for (i = 0; i < (signed32) strlen (str); i++) {
	    if (str[i] == pchar)
		continue;
	    else {
		strcpy (tmp, &(str[i])) ;
	 	if (unittype == DSP_UNIT_HEX2 || unittype == DSP_UNIT_DEC2) { 
	 	    if ((strlen (tmp) % (size_t) 2) != (size_t) 0) {
		      	str[0] = pchar ;
			strcpy (&(str[1]), tmp) ;
			break;
		    }
		}
		strcpy (str, tmp) ;
		break;
	   }
	}
	if (i == len) { 
	    if (unittype == DSP_UNIT_HEX2 || unittype == DSP_UNIT_DEC2)
		strcpy (str, "00") ;
	    else
		strcpy (str, "0") ;
	}
    }
}

/*****************************************************************************/
/* This function will remove the padding strings from the input string       */
/* 									     */
/* str : The string from which the padding string is to be removed           */
/*									     */
/* pstr : The padding string that is to be removed 			     */
/*									     */
/* just : Whether the str is left or right justified			     */
/*									     */
/*****************************************************************************/

void removepadstr (char *str, char *pstr, signed32 just) {

    signed32	i, len ;
    char	tmp[200] ;

    if (just == DSP_LEFT_JUST) {
	for (i = (signed32) (strlen (str) - strlen (pstr)) ; i >= 0 ;
						i -= (signed32) strlen (pstr)) {
	    if (strncmp (&(str[i]), pstr, strlen (pstr)) == 0)
		continue;
	    else {
		strncpy (tmp, str, (size_t) i + strlen (pstr)) ;
		tmp[i + (signed32) strlen (pstr)] = '\0' ;
		strcpy (str, tmp) ;
		break;
	    }
	}
	if (i < 0) {
	    strncpy (tmp, str, (size_t) i + strlen (pstr)) ;
	    tmp[i + (signed32) strlen (pstr)] = '\0' ;
	    strcpy (str, tmp) ;
	}
    } else {
	len = (signed32) strlen (str) ;
	for (i = 0; i <= (signed32) (strlen (str) - strlen (pstr));
						i += (signed32) strlen (pstr)) {
	    if (strncmp (&(str[i]), pstr, strlen (pstr)) == 0)
		continue ;
	    else {
		strcpy (tmp, &(str[i])) ;
		strcpy (str, tmp) ; 
		break ;
	    }
	}
	if (i > len - (signed32) strlen (pstr)) {
	    strcpy (tmp, &(str[i])) ;
	    strcpy (str, tmp) ;
	}
    }
}

/*****************************************************************************/
/* ASCII TO EBCDIC							     */
/*****************************************************************************/

void asc_ebc (char *e, char *a, signed32 len) {

static unsigned char ebcdic[] =
{
        /*       0     1     2     3     4     5     6     7    */
        0x00, 0x01, 0x02, 0x03, 0x37, 0x2D, 0x2E, 0x2F,         /* 0 */
        0x16, 0x05, 0x25, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,         /* 0 */
 
        0x10, 0x11, 0x12, 0x13, 0x3C, 0x3D, 0x32, 0x26,         /* 1 */
        0x18, 0x19, 0x3F, 0x27, 0x1C, 0x1D, 0x1E, 0x1F,         /* 1 */
 
        0x40, 0x5A, 0x7F, 0x7B, 0x5B, 0x6C, 0x50, 0x7D,         /* 2 */
        0x4D, 0x5D, 0x5C, 0x4E, 0x6B, 0x60, 0x4B, 0x61,         /* 2 */
 
        0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,         /* 3 */
        0xF8, 0xF9, 0x7A, 0x5E, 0x40, 0x7E, 0x6E, 0x6F,         /* 3 */
 
        0x7C, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,         /* 4 */
        0xC8, 0xC9, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6,         /* 4 */
 
        0xD7, 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6,         /* 5 */
        0xE7, 0xE8, 0xE9, 0x63, 0xE0, 0x64, 0x6A, 0x6D,         /* 5 */
 
        0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,         /* 6 */
        0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,         /* 6 */
 
        0x97, 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6,         /* 7 */
        0xA7, 0xA8, 0xA9, 0xC0, 0x4F, 0xD0, 0xA1, 0x07          /* 7 */
        /*       8     9     A     B     C     D     E     F    */
} ;
 
    for (; len > 0; len--)
	*e++ = ebcdic[(signed32) (*a++) & 0x7F] ;
}
 
/*exon ***********************************************************************/
/*                                                                           */
/*                            T S - E B C A S C                              */
/*                            -----------------                              */
/*                                                                           */
/*    Funktion zur Konvertierung EBCDIC -> ASCII                             */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*   Autor: R. Horn                                     Datum: 15.11.86      */
/*exoff **********************************************************************/
 
void ebc_asc (char *a, char *e, signed32 len) {
 
static char ascii[] =
{
        /*       0     1     2     3     4     5     6     7   */
        0x00, 0x01, 0x02, 0x03,  ' ', 0x09,  ' ', 0x7F,        /* 0 */
        ' ',  ' ',  ' ', 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,         /* 0 */
 
        0x10, 0x11, 0x12, 0x13,  ' ',  ' ', 0x08,  ' ',        /* 1 */
        0x18, 0x19,  ' ',  ' ', 0x1C, 0x1D, 0x1E, 0x1F,        /* 1 */
 
        ' ',  ' ',  ' ',  ' ',  ' ', 0x0A, 0x17, 0x1B,         /* 2 */
        ' ',  ' ',  ' ',  ' ',  ' ', 0x05, 0x06, 0x07,         /* 2 */
 
        ' ',  ' ', 0x16,  ' ',  ' ',  ' ',  ' ', 0x04,         /* 3 */
        ' ',  ' ',  ' ',  ' ', 0x14, 0x15,  ' ', 0x1A,         /* 3 */
 
        ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',         /* 4 */
        ' ', 0x24,  ' ', 0x2E, 0x3C, 0x28, 0x2B, 0x7C,         /* 4 */
 
        0x26,  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',        /* 5 */
        ' ',  ' ', 0x21, 0x24, 0x2A, 0x29, 0x3B,  ' ',         /* 5 */
 
        0x2D, 0x2F, 0x40, 0x5B, 0x5D,  ' ',  ' ', 0x7E,        /* 6 */
        ' ',  ' ', 0x5E, 0x2C, 0x25, 0x5F, 0x3E, 0x3F,         /* 6 */
 
        ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',         /* 7 */
        ' ', 0x60, 0x3A, 0x23, 0x40, 0x27, 0x3D, 0x22,         /* 7 */
 
        ' ',  'a',  'b',  'c',  'd',  'e',  'f',  'g',         /* 8 */
        'h',  'i',  ' ', 0x53, 0x5C, 0x5D,  ' ',  ' ',         /* 8 */
 
        ' ',  'j',  'k',  'l',  'm',  'n',  'o',  'p',         /* 9 */
        'q',  'r',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',         /* 9 */
 
        ' ', 0x7E,  's',  't',  'u',  'v',  'w',  'x',         /* A */
        'y',  'z',  ' ', 0x7B, 0x7C, 0x7D,  ' ',  ' ',         /* A */
 
        ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',         /* B */
        ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',         /* B */
 
        0x7B,  'A',  'B',  'C',  'D',  'E',  'F',  'G',        /* C */
        'H',  'I',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',         /* C */
 
        0x7D,  'J',  'K',  'L',  'M',  'N',  'O',  'P',        /* D */
        'Q',  'R',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',         /* D */
 
        0x5C,  ' ',  'S',  'T',  'U',  'V',  'W',  'X',        /* E */
        'Y',  'Z',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',         /* E */
 
        '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',         /* F */
        '8',  '9',  ' ',  ' ',  ' ',  ' ',  ' ',  ' '          /* F */
        /*       8     9     A     B     C     D     E     F   */
} ;
 
    for (; len > 0; len--)
	*a++ = ascii[(signed32) (*e++) & 0xFF] ;
}
