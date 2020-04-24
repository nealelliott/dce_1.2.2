/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stdump.c,v $
 * Revision 1.1.4.2  1996/02/18  18:21:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:44:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:08:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:17:56  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:15:22  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:03:23  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  15:57:57  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:46:21  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:02  keutel
 * 	creation
 * 	[1994/03/21  13:15:54  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: stdump.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:21:49 $" } ;
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
static char *sccsid = " @(#) stdump.c 1.1 93/11/29  ST_MAVROS";
/*********************************************************************\
*                                                                     *
* asn1st_dump(out,hdl,offset,endpos)                                  *
*                                                                     *
*   FILE * out;         File must be open;                            *
*   STOREHANDLE hdl;    Store is set active in function               *
*   long offset;        Start of dump.                                *
*   long endpos;        End of dump.                                  *
*                                                                     *
* Dumps an ASN-1 encoded area in STORE, from  [offset,endpos[  onto   *
* the text file ``out''.                                              *
*                                                                     *
* One ASN.1 type is dumped in the form :                              *
* [UNIVERSAL 17]{                                                     *
*   [APPLICATUION 0]{                                                 *
*     [UNIVERSAL 2]                                                   *
*       ' 5'16}}                                                      *
*                                                                     *
* Returns the length of the dumped type.                              *
*                                                                     *
\*********************************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <malloc.h>
#include <rogenrl.h>
#include <rouply0.h>
#include <rostor0.h>

#include <asn1st.h>

asn1 asn1_dump_item( FILE * out, register asn1 zz, register asn1 zm, int n);

int asn1st_dump( FILE * out, STOREHANDLE hdl, long offset, long endpos);

int asn1st_dumpstore( STOREHANDLE hdl);

char * asn1_dumpstring[4] = {
	"UNIVERSAL ",
	"APPLICATION ",
	"",
	"PRIVATE "};

asn1 asn1_dump_item(
FILE * out,
register asn1 zz,
register asn1 zm,
int n)
{       register asn1 pt, qt;
	int i,s,m,t,test;
	long l; 
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
		if ((l = (long)(*zz++)) & 0x80 ){
			t = (int)(l & 0x7F); l = 0;
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
			if ((l = (long)(zm - zz))<0) return(zm);
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
						(void) putc(/* (char) */ *zz,out);
						zz++;
						i++;}
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

int asn1st_dump(
	FILE * out,
	STOREHANDLE hdl,
	long offset,
	long endpos)
{       unsigned char FAR * z;
	long int stlen;
	long int wh,lread;
	/* unsigned char FAR * zzi;   */
	unsigned char FAR * zm;
	unsigned char FAR * zz;
	wh = ro0076_stwhere(hdl);
	ro0066_storepos(hdl,offset);
	stlen = endpos - offset;
	zz = (unsigned char FAR *)malloc((unsigned int )(stlen+1)); 
	if ((lread = ro0071_stgetm(hdl,(char FAR *)zz,stlen)) != stlen) {
	     fprintf(out,"\nasn1st_dump : Error when reading from store \n");
	     fprintf(out,"              from position %d to %d.\n", offset,endpos);
	     fprintf(out,"              Only %d Bytes read.\n",lread);
	}

	zm = (unsigned char FAR *)( zz + stlen );
	z = asn1_dump_item(out,zz,zm,0);
	(void) fprintf(out,"\n");
	ro0066_storepos(hdl,wh);
	free((unsigned char FAR * )zz);
	return((int)(z-(unsigned char FAR *)zz));
}

/************************************************************************/
/* asn1_check(z1,z2)                                                    */
/* Checks whether the area enclosed in [z1,z2[ is encoded according to  */
/* ASN-1. It will return:                                               */
/*      ``z2'' if everything is correct.                                */
/*      ``0'' in case of error.                                         */
/*      In some cases, a value lower than z2 if the area pointed by z1  */
/*      is encoded in ASN-1, but is shorted that z2.                    */
/************************************************************************/

unsigned char FAR * PASCAL asn1_check(
register unsigned char FAR * z1,
register unsigned char FAR *z2)
{       register t,l;
	int s;

	if (z2 - z1 < 2) return(0);
	s = *z1 & 0x20;
	if (((*z1++)&0x1F) == 0x1F){
		for(;;){
			if (z2 - z1 < 2) return(0);
			if ((*z1++ &0x80)==0) break;
		}
	}
	if (*z1 == 0x80){
		if (s == 0) return(0);
		z1++;
		for(;;){
			if (z2 - z1 < 2) return(0);
			if (*z1 == 0){
				z1++;
				if (*z1++ == 0)
					return(z1);
				else    return(0);
			}
			if ((z1 = asn1_check(z1,z2)) == 0) return(0);
		}
	}else{
		if ((l = *z1++) > 128){
			t = l&0x7F; l=0;
			if (z2 - z1 < t) return(0);
			while (t--){
				l = (l<<8) + *z1++;
			}
		}
		if (z2 - z1 < l) return(0);
		z2 = z1+l;
		if (s){
			while (z1 < z2){
				if ((z1 = asn1_check(z1,z2)) == 0) return(0);
			}
		}
		return(z2);
	}
}


/************************************************************************/
/* asn1st_dumpstore(STOREHANDLE hdl)                                    */
/*                                                                      */
/* STOREHANDLE hdl  : Store must be active.                             */
/*                                                                      */
/* Prints an output of the store in the form :                          */
/* a3 12 32 ab 4e a1 b1 c4 98 23 c4 12 43 ab ff 43    ...a.test..12..1  */
/*                                                                      */
/* Every Line 16 byte in Hex and the ASCII character if printable       */
/* else `.` .                                                           */
/*                                                                      */
/* The position of the storepointer after returning is the entry        */
/* position.                                                            */
/*                                                                      */
/* Returns the number of printed bytes.                                 */
/*                                                                      */
/************************************************************************/

int asn1st_dumpstore(
STOREHANDLE hdl)
{
	int i,k,len;
	unsigned char * start;

	long int wh;
	wh = ro0076_stwhere(hdl);
	len = (int)ro0062_storelength(hdl);
	ro0066_storepos(hdl,0);
	printf("\nDump von BER-String aus Store mit Laenge %d",len) ;
	start = (unsigned char *) malloc(len+2);
	ro0071_stgetm(hdl,(char FAR *)start,len);
	for (i=0; i<len ; i += 16) {
	   printf("\n");
	   for (k = 0;( k < 16) && ((i+k) < len) ;k++)
		  printf("%3x",(0xff & start[i+k]));
	   while (k++ < 16) printf("   ") ;
	   printf("      ");
	   for (k = 0; (k < 16) && ((i+k) < len) ;k++) {
		  if (start[i+k] < 125 && start[i+k] >= 32 )
		     printf("%1c",start[i+k]);
		  else
		    printf(".");
	   }
	}
	printf("\nEnde dump \n");
	ro0066_storepos(hdl,wh);
	return(len);
}
