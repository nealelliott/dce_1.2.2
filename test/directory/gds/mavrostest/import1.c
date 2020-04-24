/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: import1.c,v $
 * Revision 1.1.11.2  1996/02/18  00:24:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:30  marty]
 *
 * Revision 1.1.11.1  1995/12/08  22:11:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:44:49  root]
 * 
 * Revision 1.1.9.1  1994/02/22  18:12:44  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  11:55:14  marrek]
 * 
 * Revision 1.1.7.2  1993/08/12  11:28:22  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:14:22  marrek]
 * 
 * Revision 1.1.5.3  1993/01/14  17:20:18  marrek
 * 	remove #ifndef _IBMR2
 * 	[1993/01/14  13:29:53  marrek]
 * 
 * Revision 1.1.5.2  1993/01/04  17:02:37  htf
 * 	Embedded copyright notice
 * 	[1993/01/04  16:15:33  htf]
 * 
 * Revision 1.1.2.2  1992/06/11  19:51:45  melman
 * 	 gds drop
 * 	[1992/06/11  19:17:59  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: import1.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 00:24:07 $";
#endif

#include <dce/asn1.h>
#include <test.h>

long strtol();

asn1 import1_cod(z,m,n,f)
int m,n,f;
asn1 z;
{	char buf[16];
	int l;

	if (m == -1) *z++ = 67;
	else	z = asn1_type_cod(z,m,n&~ASN1_X509,0);
	sprintf(buf,"%d",f);
	*z++= l = strlen(buf);

	ASN1_MOVE(buf,z,l);
	z += l;
	return(z);
}

asn1 import1_lwc(z,f)
int f;
asn1 z;
{	char buf[16];
	int l;

	sprintf(buf,"%d",f);
	l = strlen(buf);
	ASN1_INTLW(z,l); z+= 4;
	ASN1_MOVE(buf,z,l);
	z += l;
	return(z);
}

asn1 import1_dec(z,zm,f)
int * f;
asn1 z, zm;
{	asn1 z2;
	char buf[256];
	int l;

	if ((z = asn1_skiptype(z,zm))
	&& (z = asn1_length(z,zm,&z2))){
		if (z2 == 0) ASN1_CKERR(ASN1_ERR_LDEF,z);
		if ((l= z2 - z) > 255) l = 255;
		ASN1_MOVE(z,buf,l);
		buf[l]=0;
		sscanf(buf,"%d",f);
		return(z2);
	}else	return((asn1)0);
}


asn1 import1_lwd(z,zm,f)
int * f;
asn1 z, zm;
{	char buf[256];
	int l;

	l = ASN1_LWINT(z);
	z += 4;
	if (l > 255){
		ASN1_MOVE(z,buf,255);
		buf[255]=0;
	}else{	ASN1_MOVE(z,buf,l);
		buf[l]=0;
	}
	sscanf(buf,"%d",f);
	return(z + l);
}

char * import1_out(s,f)
char * s;
int f;
{
	sprintf(s,"%d",f);
	return(s + strlen(s));
}

import1_olen(f)
int f;
{
	return(8);
}

char * import1_in(s,sm,f)
char * s, *sm;
int * f;
{	char * s2;

	*f = strtol(s,&s2,0);
	return(s2);
}

import1_cpy(f1,f2)
int *f1, f2;
{	*f1 = f2;
}
