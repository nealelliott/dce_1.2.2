/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dump.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:20  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:06:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:25  root]
 * 
 * Revision 1.1.8.2  1994/05/10  15:48:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:07:21  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  15:22:07  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  08:51:53  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  11:23:47  marrek
 * 	July 1993 code drop.
 * 	[1993/07/28  16:58:09  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  16:34:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:51:34  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:15:53  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:17:44  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dump.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:37:31 $";
#endif

 
/* ****************************************************************** */
/*                                                                    */
/*   COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONSSYSTEME AG   */
/*                           AND INRIA                                */
/*   All rights reserved                                              */
/*                                                                    */
/*   This software is furnished under licence and may be used only    */
/*   in accordance with the terms of that licence and with the        */
/*   inclusion of the above copyright notice.                         */
/*   This software may not be provided or otherwise made available    */
/*   to, or used by, any other person. No title to or ownership of    */
/*   the software is hereby  transferred.                             */
/*                                                                    */
/* ****************************************************************** */

/*********************************************************************\
* 								      *
* asn1_dump(out,zz,zm,n)					      *
* 								      *
* Dumps an ASN-1 encoded area in [zz,zm[ onto the text file ``out''.  *
* ``n'' is a recursion level indicator, which should be set to 0 by   *
* the caller.							      *
* 								      *
\*********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <dce/asn1.h>

char * asn1_dumpstring[4] = {
	"UNIVERSAL ",
	"APPLICATION ",
	"",
	"PRIVATE "};

static asn1 asn1_dump_item (FILE *, asn1, asn1, int);

static asn1 asn1_dump_item(out,zz,zm,n)
FILE * out;
register asn1 zz, zm;
int n;
{	register asn1 pt, qt;
	int i,s,m,t,l, test;

	s = (*zz)&0x20;
	m = (*zz)>>6;
	if ((t = ((*zz++)&0x1F)) == 0x1F){
		t = 0;
		while (zz < zm){
			t = (t<<7) + (*zz &0x7F);
			if ((*zz++ &0x80)==0) break;
		}
	}
	(void) putc('\n',out);
	for (i=0; i <n; i++) (void) putc(' ',out);
	(void) fprintf(out,"[%s%d]%s", asn1_dumpstring[m],t,(s)?"{":"");
	if (*zz == 0x80){
		zz++;
		if (s){
			for(;;){
				zz = asn1_dump_item(out,zz,zm,n+1);
				if (zz >= zm){
					(void) fprintf(out,
					"--COMPONENT TOO LONG!-- }");
					return(zm);}
				if (*zz == 0){
					zz += 2;
					(void) putc('}',out); break;}
				(void) putc(',',out);
			}
		}else{
			(void) putc('\n',out);
			for (i = 0; i<=n; i++) (void) putc(' ',out);
			(void) fprintf(out,
			"--INDEFINITE LENGTH FOR PRIMITIVE COMPONENT--");
			return(zm);
		}
	}else{
		if ((l = *zz++)&0x80){
			t = l&0x7F; l = 0;
			if (zz + t > zm){
				(void) putc('\n',out);
				for (i = 0; i<=n; i++) (void) putc(' ',out);
				(void) fprintf(out,
				"-- INCORRECT LENGTH OF LENGTH(%d > %d)--",
				t,zm-zz);
				return(zm);
			}
			while (t--) l = (l<<8) + *zz++;
		}
		if (zz+l > zm){
			(void) putc('\n',out);
			for (i = 0; i<=n; i++) (void) putc(' ',out);
			(void) fprintf(out,"--INCORRECT LENGTH (%d > %d) --",
			l,zm-zz);
			if ((l = zm - zz)<0) return(zm);
		}
		zm = zz+l;
		if (s){
			while (zz < zm){
				zz = asn1_dump_item(out,zz,zm,n+1);
				if (zz < zm) (void) putc(',',out);
			}
			(void) putc('}',out);
		}else if ((zm - zz) > 16 && (*zz)&32 &&
		asn1_check(zz,zm) == zm){
			(void)fprintf(out," * --Embedded ASN1--");
			zz = asn1_dump_item(out,zz,zm,n+3);
			(void)fprintf(out,"--End of embedded ASN1--");
		}else{
			while (zz < zm){
				(void) putc('\n',out);
				for (i = 0; i<=n; i++) (void) putc(' ',out);
				qt = zz + ((72 - i)>>1);
				if (qt > zm) qt = zm;
				for(pt = zz; pt < qt; pt++){
					if (test=
					((*pt<32 || *pt>=127) && *pt != '\n' 
					&& *pt != '\r' && *pt != '\t'))
						break;
				}
				if (test == 0 && *zz >= 32 && *zz < 127){
					(void) putc('"',out);
					while
					(*zz >= 32 && *zz < 127 
					&& i < 72  && zz < zm){
						(void) putc(*zz++,out); i++;}
					(void) putc('"',out);
				}else{
					(void) fprintf(out,"\'");
					while
					((test || *zz < 32 || *zz >= 127)
					&& i < 72  && zz < zm){
						(void) fprintf(out,"%2x",*zz++); 
						i+=2;}
					(void) fprintf(out,"\'16");
				}
				if (zz < zm) (void) fprintf(out," &");
			}
		}
	}
	return(zz);
}

unsigned char * asn1_dump(out,zz,zm)
FILE * out;
unsigned char * zz, * zm;
{	unsigned char * z;

	z = asn1_dump_item(out,zz,zm,0);
	(void) fprintf(out,"\n");
	return(z);
}
